#include <iostream>
#include "process.h"

Process::Process()
{
	client_fd=-1;
	CONTROL_LEN=CMSG_LEN(sizeof(int));
	msgid=-1;
	p_mtype=-1;
	msg_data[0]='\0';
}

int Process::p_fork(int sock_fd,int msg_id,int msg_type)
{
	msgid=msg_id;
	p_mtype=msg_type;
	client_fd=sock_fd;
	printf("process client socjfd:%d\n",client_fd);
	if(socketpair(PF_UNIX,SOCK_DGRAM,0,sv)==-1)
	{
		printf("sockpair error\n");
		return -1;
	}
	pid=fork();
	if(pid==-1)
	{
		printf("fork error\n");
		return -1;
	}
	if(pid==0)
	{
		printf("create a new process\n");
		close(sv[1]);
		p_recvfd(sv[0]);
		char data[10];
		
		while(1)
		{
			msg_handle();
		}
		return 0;
	}
	else
	{
		close(sv[0]);
		p_sendfd(sv[1],client_fd);
	}
}

int Process::p_recvfd(int rsv)
{
	iov[1].iov_base=buf;
	iov[1].iov_len=1;

	msg.msg_name=NULL;
	msg.msg_namelen=0;
	msg.msg_iov=iov;
	msg.msg_iovlen=1;
	msg.msg_control=&cmsg;
	msg.msg_controllen=CONTROL_LEN;

	if(recvmsg(rsv,&msg,0)==-1)
	{
		printf("recv fd error\n");
		return -1;
	}
	client_fd=*(int *)CMSG_DATA(&cmsg);
	printf("received a client_sockfd:%d\n",client_fd);
	return 0;
}

int Process::p_sendfd(int ssv, int fd)
{
	iov[1].iov_base=buf;
	iov[1].iov_len=1;
	msg.msg_name=NULL;
	msg.msg_namelen=0;
	msg.msg_iov=iov;
	msg.msg_iovlen=1;
	cmsg.cmsg_len=CONTROL_LEN;
	cmsg.cmsg_level=SOL_SOCKET;
	cmsg.cmsg_type=SCM_RIGHTS;
	msg.msg_control=&cmsg;
	msg.msg_controllen=CONTROL_LEN;
	*(int *)CMSG_DATA(&cmsg)=fd;

	if(sendmsg(ssv,&msg,0)==-1)
	{
		printf("send fd error\n");
		return -1;
	}
	printf("send a client sockfd %d \n",fd);
	return 0;
}

int Process::msg_handle()
{
	int ret=0;
	err=p_msg.recv_data(msgid,p_mtype,msg_data);
	if(err==-1)
	{
		printf("ERROR:process msg recv error\n");
		return -1;
	}
	switch(msg_data[0])
	{
		case '1':p_close();
		case '2':ret=talkback();break;
		default:msg_back_data[0]='2';
	}
	if(ret==-1)
	{
		printf("ERROR:write to client error\n");
		msg_back_data[0]='1';
	}
	err=p_msg.send_data(msgid,9+p_mtype,msg_back_data);
	if(err==-1)
	{
		printf("ERROR:child msg send data error\n");
		return -1;
	}
	return 0;
}

int Process::talkback()
{
	char buff[DATA_LEN];
	strncpy(buff,msg_data+1,DATA_LEN);
	err=write(client_fd,buff,sizeof(buff));
	if(err==-1)
	{
		perror("ERROR:child write to client error");
		//strncpy(msg_data,"error",DATA_LEN);
		return -1;
	}
	printf("child %d has write to client successful\n",client_fd);
	printf("child %d reading...\n",client_fd);
	read(client_fd,buff,sizeof(buff));
	strncpy(msg_back_data+1,buff,DATA_LEN);
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

