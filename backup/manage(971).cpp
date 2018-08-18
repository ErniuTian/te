#include <iostream>
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
	for(i=0;i<CHILD_DATA_LEN;i++)
	{
		child_data[i]=0x00;
	}
	for(i=0;i<DATA_LEN;i++)
	{
		parent_data[i]=0x00;
		//parent_back_data[i]=0x00;
	}
	child_msgid=-1;
	parent_msgid=-1;
	
}
/*
manage::~manage()
{

}
*/
/*create a tcp link*/
int manage::create()
{
	char *keypath1,*keypath2;
	char key1[]="./key5";
	char key2[]="./key6";
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
	return parent_msgid;
}


int manage::msg_handle()
{
	printf("manage msg recving...\n");	
	int ret=0;
	int i;
	err=parent_msg.recv_data(parent_msgid,1,parent_data,DATA_LEN);
	if(err==-1)
	{
		printf("ERROR:manage msg handle msg recv data error\n");
		return -1;
	}
	printf("manage msg_handle recv parent data:");
	for(i=0;i<err;i++)
	{
		printf("%02X ",parent_data[i]);
	}
	printf("\n");
	switch(parent_data[0])
	{
		case 0x00:ret=client_add();break;
		case 0x01:ret=talkback();break;
		case 0x02:ret=client_delete();break;
		case 0x03:manage_exit();
		default:break;//parent_back_data[0]=0x02;
	}
	/* replaced by function data_back
	if(ret==-1)
	{
		parent_back_data[0]=0x01;
	}
	else
		parent_back_data[0]=0x00;
		
	err=parent_msg.send_data(parent_msgid, 2, parent_back_data,DATA_LEN);
	if(err==-1)
	{
		printf("ERROR:manage msg send data error\n");
		return -1;
	}
	printf("manage msg_handle send parent back data:");
	for(i=0;i<DATA_LEN;i++)
	{
		printf("%02X ",parent_back_data[i]);
	}
	printf("\n");
	*/
	return 0;
}

int manage::data_back(int len,char data[])
{
	int i;
	err=parent_msg.send_data(parent_msgid, 2, data,len);
	if(err==-1)
		{
		printf("ERROR:manage data_back send error\n");
		return -1;
		}
	
	printf("manage data_back:");
	for(i=0;i<len;i++)
		{
		printf("%02X ",data[i]);
		}
	printf("\n");
	return 0;
}



/*accpet a new client and create a child process*/
/*time out ?*/
int manage::client_add()
{
	int back_data_len=5;
	char parent_back_data[back_data_len];
	memset(parent_back_data,0,back_data_len);
	num=num_get();
	int i;
	if(num==-1)
	{
		printf("manage:client num has reached max\n");
		parent_back_data[0]=0x01;
		return -1;
	}
	client_fd[num]=server_sock.tcp_accept();
	if(client_fd[num]==-1)
	{
		printf("ERROR:manage no available client\n");
		parent_back_data[0]=0x01;
		return -1;
	}
	printf("manage get  client sockfd:%d\n",client_fd[num]);
	err=child[num].p_fork(client_fd[num],child_msgid,num);
	if(err==-1)
	{
		printf("ERROR:manage client_add error\n");
		parent_back_data[0]=0x01;
		return -1;
	}
	printf("manage child add sucessful\n");
	parent_back_data[1]=num;

	err=data_back(back_data_len, parent_back_data);
	if(err==-1)
		{
		printf("ERROR:manage clien add data back error\n");
		return -1;
		}

	return 0;
}

int manage::talkback()
{
	/*
	printf("manage talkbacking...\n");
	int i;
	num=(int)parent_data[1];
	memcpy(child_data,parent_data+2,CHILD_DATA_LEN);
	err=child_msg.send_data(child_msgid, num, child_data,CHILD_DATA_LEN);
	if(err==-1)
	{
		printf("manage childmsg send data error\n");
		return -1;
	}
	printf("manage childmsg send data:");
	for(i=0;i<CHILD_DATA_LEN;i++)
	{
		printf("%02X ",child_data[i]);
	}
	printf("\n");

	err=child_msg.recv_data(child_msgid, num+9, child_data,CHILD_DATA_LEN);
	if(err==-1)
	{
		printf("manage childmsg recv data error\n");
		return -1;
	}
	printf("manage childmsg recv data:");
	for(i=0;i<CHILD_DATA_LEN;i++)
	{
		printf("%02X ",child_data[i]);
	}
	printf("\n");

	memcpy(parent_back_data+2,child_data,CHILD_DATA_LEN);
	parent_back_data[1]=(char)num;
	*/
	return 0;
}

int manage::client_delete()
{
	err=talkback();
	if(err==-1)
	{
		return -1;
	}
	client_fd[parent_data[1]]=-1;
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
	for(i=1;i<CLIENT_NUM;i++)
	{
		if(client_fd[i]==-1)
			return i;
	}
	return -1;
}



