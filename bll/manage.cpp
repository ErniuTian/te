#include <iostream>
//#include "tcp_link.h"
//#include "process.h"
//#include "message.h"
#include "manage.h"
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
	char *keypath1,*keypath2;
	char key1[]="./key1";
	char key2[]="./key2";
	keypath1=key1;
	keypath2=key2;
	err=server_sock.tcp_create();
	if(err==-1)
	{
		printf("ERROR:manage tcp link create error\n");
		return -1;
	}
	printf("manage tcp create successful\n");

	child_msgid=child_msg.msg_create(keypath1);
	if(child_msgid==-1)
	{
		printf("ERROR:manage get a bad child_msgid\n");
		return -1;
	}
	printf("manage child msg create successful:%d\n",child_msgid);
	
	parent_msgid=parent_msg.msg_create(keypath2);
	if(parent_msgid==-1)
	{
		printf("ERROR:manage get a bad parent_msgid\n");
		return -1;
	}
	printf("manage parent msg create successful:%d\n",parent_msgid);
	/*
	msgid=&parent_msgid;
	while(1)
	{
		
		msg_handle();
	}
	return 0;
	*/
	return parent_msgid;
}


int manage::msg_handle()
{
	
	int ret=0;
	err=parent_msg.recv_data(parent_msgid,1,parent_data);
	if(err==-1)
	{
		printf("ERROR:manage msg handle msg recv data error\n");
		return -1;
	}
	switch(parent_data[0])
	{
		case '0':ret=client_add();break;
		case '1':ret=talkback();break;
		case '2':ret=client_delete();break;
		case '3':manage_exit();
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
		return -1;
	}
	printf("manage child add sucessful\n");
	memset(parent_back_data,'0',PARENT_DATA_LEN);
	parent_back_data[0]='0';
	parent_back_data[1]=(char)num;
	
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

int manage::client_delete()
{
	err=talkback();
	if(err==-1)
	{
		return -1;
	}
	client_fd[parent_data[1]]==-1;
	return 0;
}

int manage::manage_exit()
{
	/*
	关闭所有子进程
	删除消息队列
	关闭tcp服务器端口
	*/
	server_sock.tcp_close();
}


int manage::num_get()
{
	int i;
	for(i=0;i<CLIENT_NUM;i++)
	{
		if(client_fd[i]==-1)
			return i;
	}
	return -1;
}

