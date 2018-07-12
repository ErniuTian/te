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
int manage::create(int *msgid)
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
	msgid=&parent_msgid;
	while(1)
	{
		
		msg_handle();
	}
	return 0;
}


int manege::msg_handle()
{
	int ret=0;
	err=parent_msg.recv_data(parent_msgid,1,parent_data);
	if(err==-1)
	{
		printf("ERROR:manage msg handle msg recv data error\n");
		return -1;
	}
	swicth(parent_data[0])
	{
		case '0':ret=client_add();break;
		case '1':ret=talkback();break;
		case '2':ret=client_delete();break;
		default:parent_back_data[0]='2';
	}
	
	if(ret==-1)
	{
		parent_back_data[0]='1';
	}
	else
		parent_back_data[0]='0';
		
	err=parent_msg.send_data(parent_msgid, 2, parent_back_data);
	if(err==-1)
	{
		printf("ERROR:manage msg send data error\n");
		return -1;
	}
	return 0;
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
	
	return 0;
}

int manage::talkback()
{
	num=parent_data[1];
	child_msg.send_data(child_msgid, num, child_data);
	child_msg.recv_data(child_msgid, num+9, child_data);
	parent_back_data[1]=num;
	strncpy(parent_back_data+2,child_data,CHILD_DATA_LEN);
	return 0;
}

int manege::client_delete()
{
	err=talkback();
	if(err==-1)
	{
		return -1;
	}
	client_fd[parent_data[1]]==-1;
	return 0;
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

