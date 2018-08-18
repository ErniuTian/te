#include <iostream>
#include "process.h"

Process::Process()
{
	int i;
	client_fd=-1;
	msg_data_len=0;
	msg_back_data_len=0;
	box_addr=0x00;
	CONTROL_LEN=CMSG_LEN(sizeof(int));
	msgid=-1;
	p_mtype=-1;
	memset(msg_data,0,CHILD_DATA_LEN);
	memset(msg_back_data,0,CHILD_DATA_LEN);
	sv[0]=-1;
	sv[1]=-1;
}

int Process::p_fork(int sock_fd,int msg_id,int msg_type)
{
	msgid=msg_id;
	p_mtype=msg_type;
	client_fd=sock_fd;
	printf("process client sockfd:%d\n",client_fd);
	if(socketpair(PF_UNIX,SOCK_DGRAM,0,sv)==-1)
	{
		printf("ERROR:process sockpair error\n");
		return -1;
	}
	printf("process create socketpair successful\n");
	pid=fork();
	if(pid==-1)
	{
		perror("ERROR:fork error");
		return -1;
	}
	if(pid==0)
	{
		printf("process create a new process\n");
		close(sv[1]);
		err=p_recvfd(sv[0]);
		if(err==-1)
		{
			printf("ERROR:process p_fork recv fd error\n");
			exit(-1);
				
		}
		
		while(1)
		{
			err=msg_handle();
			if(err==-1)
			{
				printf("ERROR:process msg_handle error\n");
				break;
			}
		}
		exit(0);
	}
	else
	{
		sleep(1);
		close(sv[0]);
		err=p_sendfd(sv[1],client_fd);
		if(err==-1)
		{
			printf("ERROR:process p_fork sendfd error\n");
			return -1;
		}
		//printf("process parent send a fd\n");
		close(client_fd);
		printf("process parent has closed client\n");
		return 0;
	}
}

int Process::p_recvfd(int rsv)
{
	char buf[0];
	iov[0].iov_base=buf;
	iov[0].iov_len=1;

	msg.msg_name=NULL;
	msg.msg_namelen=0;
	msg.msg_iov=iov;
	msg.msg_iovlen=1;
	msg.msg_control=&cmsg;
	msg.msg_controllen=CONTROL_LEN;

	//printf("process fd recving...\n");
	if(recvmsg(rsv,&msg,0)==-1)
	{
		perror("ERROR:recv fd error");
		return -1;
	}
	client_fd=*(int *)CMSG_DATA(&cmsg);
	if(client_fd==-1)
	{
		printf("ERROR:process p_recvfd get a bad client fd\n");
		return -1;
	}
	//printf("process received a client_sockfd:%d\n",client_fd);
	return 0;
}

int Process::p_sendfd(int ssv, int fd)
{
	char buf[0];
	if(buf==NULL)
	{
		printf("ERROR:process sendfd buf is null\n");
		return -1;
	}
	iov[0].iov_base=buf;
	iov[0].iov_len=1;
	msg.msg_name=NULL;
	msg.msg_namelen=0;
	msg.msg_iov=iov;
	msg.msg_iovlen=1;
	cmsg.cmsg_len=CONTROL_LEN;
	cmsg.cmsg_level=SOL_SOCKET;
	cmsg.cmsg_type=SCM_RIGHTS;
	msg.msg_controllen=CONTROL_LEN;
	*(int *)CMSG_DATA(&cmsg)=fd;
	msg.msg_control=&cmsg;
	//printf("process fd sending...\n");
	if(sendmsg(ssv,&msg,0)==-1)
	{
		perror("ERROR:send fd error");
		return -1;
	}
	//printf("process sendfd send a client sockfd %d \n",fd);
	return 0;
}

int Process::msg_handle()
{
	int ret=0;
	int i;
	printf("process msg_handle recving...\n");
	msg_data_len=p_msg.recv_data(msgid,p_mtype,msg_data,CHILD_DATA_LEN);
	if(msg_data_len==-1)
	{
		printf("ERROR:process msg recv error\n");
		return -1;
	}
	printf("process msg_handle received:");
	for(i=0;i<msg_data_len;i++)
	{
		printf("%02X ",msg_data[i]);
	}
	printf("\n");
	switch(msg_data[0])
	{
		case 0x00:err=p_close();break;
		case 0x01:err=link_test();break;
		case 0x07:err=param_set();break;
		case 0x03:err=param_set();break;
		default:msg_back_data[0]=0x02;
	}
	
	if(err==-1)
		{
		printf("ERROR:process msg_handle  error\n");
		return -1;
		}
	return 0;
}


int Process::p_close()
{
	msg_back_data_len=3;
	int i;
	printf("process is going to exit..\n");
	close(client_fd);
	printf("child has closed client\n");
	memset(msg_back_data,0,msg_back_data_len);
	err=parent_back(msg_back_data_len, msg_back_data);
	if(err==-1)
	{
		printf("ERROR:process p_close send data error\n");
		return -1;
	}
	printf("process p_close send data:");
	for(i=0;i<msg_back_data_len;i++)
	{
		printf("%02x ",msg_back_data[i]);
	}
	printf("\n");
	exit(0);
}


int Process::link_test()
{
	msg_back_data_len=5;
	printf("process link testing...\n");
	int box_data_len, box_back_data_len;
	box_data_len=10;
	box_back_data_len=13;
	char box_data[box_data_len];
	char box_back_data[box_back_data_len];
	memset(box_data,0,box_data_len);
	memset(box_back_data,0,box_back_data_len);
	memset(msg_back_data,0,msg_back_data_len);

	box_data[0]=0x3A;//trigger
	box_data[1]=0x3A;//strat
	box_data[2]=0x46;//box addr
	box_data[3]=0x46;//box addr
	box_data[4]=0x30;//cmd h
	box_data[5]=0x31;//cmd l
	box_data[6]=0x46;//checksum
	box_data[7]=0x46;//chencksum
	box_data[8]=0x0D;//end
	box_data[9]=0x0A;//end

	err=talkback(box_data_len, box_data, box_back_data_len, box_back_data);
	if(err==-1)
		{
		printf("process linktest talkback error\n");
		msg_back_data[0]=0x01;
		//return -1;
		}

	box_addr=box_back_data[1]-0x30;
	box_addr=box_addr*10+(box_back_data[2]-0x30);
	msg_back_data[1]=box_addr;
	msg_back_data[2]=0x02;//data len
	msg_back_data[3]=box_back_data[5]-0x30;
	msg_back_data[3]=msg_back_data[3]*10+(box_back_data[6]-0x30);//box type
	msg_back_data[4]=box_back_data[7]-0x30;
	msg_back_data[4]=msg_back_data[4]*10+(box_back_data[8]-0x30);//box version

	err=parent_back(msg_back_data_len,msg_back_data);
	if(err==-1)
		{
		printf("ERROR:process link test parent back error\n");
		return -1;
		}
	return 0;
}

int Process::param_set()
{
	int channelH=0;
	int channelL=0;
	int channel=0;
	msg_back_data_len=3;
	printf("process param seting...\n");
	int box_data_len, box_back_data_len;
	box_data_len=92;
	box_back_data_len=91;
	char box_data[box_data_len];
	char box_back_data[box_back_data_len];
	memset(box_data,0,box_data_len);
	memset(box_back_data,0,box_back_data_len);
	memset(msg_back_data,0,msg_back_data_len);

	channel=(int)msg_data[3];
	channelH=4+2*channel;
	channelL=5+2*channel;

	box_data[0]=0x3A;//trigger
	box_data[1]=0x3A;//strat
	box_data[2]=0x46;//box addr
	box_data[3]=0x46;//box addr
	box_data[4]=0x30;//cmd h
	box_data[5]=0x37;//cmd l
	box_data[channelH]=msg_data[4]/10+30;//param H
	box_data[channelL]=msg_data[4]%10+30;//param L
	box_data[90]=0x0D;//end 1
	box_data[91]=0x0A;//end 2

	err=talkback(box_data_len, box_data, box_back_data_len, box_back_data);
	if(err=-1)
		{
		printf("ERROR:param set talkback error\n");
		msg_back_data[0]=0x01;
		//return -1;
		}
	
	if((box_back_data[channelH-1]==box_data[channelH])&(box_back_data[channelL-1]==box_data[channelL]))
		{
		msg_back_data[2]=0x00;//param set success
		}
	else
		{
		msg_back_data[2]=0x01;	//param set fail
		}

	err=parent_back(msg_back_data_len, msg_back_data);
	if(err==-1)
		{
		printf("ERROR:process param set parent back error\n");
		return -1;
		}

	return 0;
}


int Process::parent_back(int len, char data [ ])
{
	int i;
	err=p_msg.send_data(msgid, p_mtype+9, msg_back_data, len);
	if(err==-1)
		{
		printf("ERROR:process parent back error\n");
		return -1;
		}
	printf("process parent back data:");
	for (i = 0; i < len; i++)
		{
		printf("%02X ",data[i]);
		}
	printf("\n");
	return 0;
}

int Process::talkback(int w_len, char w_data[], int r_len, char r_data[])
{
	int i;
	err=write(client_fd,w_data,w_len);
	if(err==-1)
	{
		perror("ERROR:child write to client error\n");
		msg_back_data[0]=0x01;
		return -1;
	}
	printf("process %d  write to client :",client_fd);
	for(i=0;i<err;i++)
	{
		printf("%02X ",w_data[i]);
	}
	printf("\n");

	printf("child %d reading...\n",client_fd);
	err=read(client_fd,r_data,r_len);
	if(err==-1)
	{
		printf("ERROR:child read froms %d error\n",client_fd);
		msg_back_data[0]=0x01;
		return -1;
	}
	printf("process talkback read from box:");
	for(i=0;i<err;i++)
	{
		printf("%02X ",r_data[i]);
	}
	printf("\n");
	
	return 0;
}

Process::~Process()
{
	//p_close();
}

