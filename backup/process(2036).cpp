#include <iostream>
#include "process.h"

Process::Process()
{
	int i;
	client_fd=-1;
	CONTROL_LEN=CMSG_LEN(sizeof(int));
	msgid=-1;
	p_mtype=-1;
	msg_data[0]='\0';
	for(i=0;i<CHILD_DATA_LEN;i++)
	{
		msg_data[i]=0x00;
		msg_back_data[i]=0x00;
	}
	
	sv[0]=-1;
	sv[1]=-1;
}

int Process::p_fork(int sock_fd,int msg_id,int msg_type)
{
	msgid=msg_id;
	p_mtype=msg_type;
	client_fd=sock_fd;
	printf("process client socjfd:%d\n",client_fd);
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
		//printf("process child received a fd:%d\n",fd);
		char data[10];
		
		//while(1)
		//{
			msg_handle();
		//}
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
		printf("process parent send a fd\n");
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

	printf("process fd recving...\n");
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
	printf("process received a client_sockfd:%d\n",client_fd);
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
	printf("process fd sending...\n");
	if(sendmsg(ssv,&msg,0)==-1)
	{
		perror("ERROR:send fd error");
		return -1;
	}
	printf("process sendfd send a client sockfd %d \n",fd);
	return 0;
}

int Process::msg_handle()
{
	int ret=0;
	int i;
	printf("process msg_handle recving...\n");
	err=p_msg.recv_data(msgid,p_mtype,msg_data,CHILD_DATA_LEN);
	if(err==-1)
	{
		printf("ERROR:process msg recv error\n");
		return -1;
	}
	printf("process msg_handle received:");
	for(i=0;i<CHILD_DATA_LEN;i++)
	{
		printf("%02X ",msg_data[i]);
	}
	printf("\n");
	switch(msg_data[0])
	{
		case 0x01:p_close();
		case 0x02:link_test();break;
		case 0x03:talkback();break;
		default:msg_back_data[0]=0x02;
	}
	err=p_msg.send_data(msgid,9+p_mtype,msg_back_data,CHILD_DATA_LEN);
	if(err==-1)
	{
		printf("ERROR:child msg send data error\n");
		return -1;
	}
	printf("process msg_handle send data");
	for(i=0;i<CHILD_DATA_LEN;i++)
	{
		printf("%02x ",msg_back_data[i]);
	}
	printf("\n");
	return 0;
}

int Process::talkback()
{
	int buff[BOX_DATA_LEN];
	memcpy(buff,msg_data+1,BOX_DATA_LEN);
	err=write(client_fd,buff,BOX_DATA_LEN);
	if(err==-1)
	{
		perror("ERROR:child write to client error");
		msg_back_data[0]=0x01;
		return -1;
	}
	printf("child %d has write to client successful\n",client_fd);
	printf("child %d reading...\n",client_fd);
	err=read(client_fd,buff,BOX_DATA_LEN);
	if(err==-1)
	{
		printf("ERROR:child read from %d error\n",client_fd);
		msg_back_data[0]=0x01;
		return -1;
	}
	memcpy(msg_back_data+1,buff,BOX_DATA_LEN);
	return 0;
}

int Process::link_test()
{
	printf("process link testing...\n");
	int buff[BOX_DATA_LEN];
	memcpy(buff,msg_data+1,BOX_DATA_LEN);
	int test[3];
	test[0]=0x01;
	test[1]=0x02;
	test[3]=0x03;
	//err=write(client_fd,test,3);
	err=write(client_fd,buff,BOX_DATA_LEN);
	if(err==-1)
	{
		perror("ERROR:child write to client error\n");
		msg_back_data[0]=0x01;
		return -1;
	}
	printf("child %d has write to client successful\n",client_fd);
	int i;
	/*
	for(i=0;i<3;i++)
	{
		printf("%02X ",test[i]);
	}
	printf("\n");
*/
	for(i=0;i<BOX_DATA_LEN;i++)
	{
		printf("%02X ",buff[i]);
	}
	printf("\n");
	printf("child %d reading...\n",client_fd);
	err=read(client_fd,buff,6);
	if(err==-1)
	{
		printf("ERROR:child read from %d error\n",client_fd);
		msg_back_data[0]=0x01;
		return -1;
	}
	memcpy(msg_back_data+2,buff,6);
	msg_back_data[1]=0x00;
	msg_back_data[0]=0x00;
	return 0;
}

int Process::p_close()
{
	close(client_fd);
	printf("child has closed client\n");
	exit(0);
}

Process::~Process()
{
	p_close();
}

