#include <iostream>
#include "tcp_link.h"
#include "process.h"
#include "message.h"
#include <string.h>

manage::manage()
{
	num=-1;
	int i;
	for(i=0;i<CLIENT_NUM;i++)
	{
		client_fd[i]=-1;
	}
	child_msgid=-1;
	parent_msgid=-1;
	
}

/*create a tcp link*/
int manage::create()
{
	err=server_sock.tcp_create();
	printf("ERROR:manage tcp link create error\n");
	retrun -1;

	child_msgid=child_msg.msg_create();
	if(child_msgid==-1)
	{
		printf("ERROR:manage get a bad child_msgid\n");
		return -1;
	}
	parent_msgid=parent_msg.msg_create();
	if(parent_msgid==-1)
	{
		printf("ERROR:manage get a bad parent_msgid\n");
		return -1;
	}

	while(1)
	{
		parent_msg.data_recv(parent_msgid,);
		msg_handle();
	}
	
}


int manege::msg_handle()
{
	
}

/*accpet a new client and create a child process*/
/*time out ?*/
int manage::client_add()
{
	num=num_get();
	if(num==-1)
	{
		printf("manage:client num has reached max\n");
		return -1;
	}
	client_fd[num]=server_sock.tcp_accept();
	if(client_fd[num]==-1)
	{
		printf("ERROR:manage no available client\n");
		return -1;
	}
	printf("manage get  client sockfd:%d\n",client_fd[num]);
	err=child[num].p_fork(client_fd[num],child_msgid,num);
	if(err==-1)
	{
		printf("ERROR:manage client_add error\n");
		retrun -1;
	}
	printf("manage child add sucessful\n");
}

int manage::talkback()
{
}

int manege::client_delete()
{
}

int manege::num_get()
{
	int i;
	for(i=0;i<CLIENT_NUM;i++)
	{
		if(client_fd[i]!=-1)
			return i;
	}
	return -1;
}

