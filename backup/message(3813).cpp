#include <iostream>
#include <string.h>
#include <stdio.h>
#include "message.h"

message::message()
{
	int i;
	msgid=-1;
	my_msgbuf.type=0;
	for(i=0;i<DATA_LEN;i++)
	{
		my_msgbuf.text[i]=0x00;
	}
	err=0;
}

message::~message()
{
	msg_delete();
}

int message::msg_create(char *keypath)
{
	key=ftok(keypath,'a');
	if(key==-1)
	{
		perror("ERROR:message ftok error");
		return -1;
	}
	msgid=msgget(key,IPC_CREAT);
	if(msgid==-1)
	{
		perror("ERROR:process msgget error");
		return -1;
	}
	printf("message create successful msgid:%d\n",msgid);
	return msgid;
}

int message::send_data(int msg_id,int msg_type, int msg_data[], int len)
{
	int i;
	msgid=msg_id;
	printf("message send msgid:%d\n",msg_id);
	memcpy(my_msgbuf.text,msg_data,len);
	printf("message send_data :");
	for(i=0;i<len;i++)
	{
		printf("%02X ",my_msgbuf.text[i]);
	}
	printf("\n");
	printf("message send msglen:%d\n",len);
	printf("message send msgtype:%d\n",msg_type);
	my_msgbuf.type=msg_type;
	err=msgsnd(msgid,&my_msgbuf,len,0 );
	if(err==-1)
	{
		perror("ERROR:message msgend error");
		return -1;
	}
	printf("message msgsnd success\n");
	return 0;
}

int message::recv_data(int msg_id,int msg_type,int msg_data[], int len)
{
	int i;
	msgid=msg_id;
	my_msgbuf.type=msg_type;
	err=msgrcv(msgid,&my_msgbuf,len,my_msgbuf.type,0);
	if(err==-1)
	{
		perror("ERROR:message msgrcv error");
		return -1;
	}
	/*
	printf("message msgrcv msg_text:");
	for(i=0;i<len;i++)
	{
		printf("%02X ",my_msgbuf.text[i]);
	}
	printf("\n");
*/
	memcpy(msg_data,my_msgbuf.text,len);
	printf("message msgrcv :");
	for(i=0;i<len;i++)
	{
		printf("%02X ",msg_data[i]);
	}
	printf("\n");
	return 0;
}

int message::msg_delete()
{
	err=msgctl(msgid,IPC_RMID,NULL);
	if(err==-1)
	{
		perror("ERROR:message delete error");
		return -1;
	}
	printf("messag :%d  delete success\n",msgid);
	return 0;
}
