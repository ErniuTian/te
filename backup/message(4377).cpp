#include <iostream>
#include <string.h>
#include <stdio.h>
#include "message.h"

message::message()
{
	msgid=-1;
	my_msgbuf.type=0;
	//my_msgbuf.text={'0'};
	strncpy(my_msgbuf.text,"0000",3);
	err=0;
}

int message::msg_create()
{
	key=ftok(KEY_PATH,'a');
	if(key==-1)
	{
		perror("ERROR:messaga ftook error");
		return -1;
	}
	msgid=msgget(key,IPC_CREAT);
	if(msgid==-1)
	{
		perror("ERROR:process msgget error");
		return -1;
	}
	return msgid;
}

int message::send_data(int msg_id,int msg_type, char *msg_data)
{
	msgid=msg_id;
	strncpy(my_msgbuf.text,msg_data,DATA_LEN);
	my_msgbuf.type=msg_type;
	err=msgsnd(msgid,&my_msgbuf,DATA_LEN,0 );
	if(err==-1)
	{
		perror("ERROR:message msgend error");
		return -1;
	}
	printf("message msgsnd success:%s\n",my_msgbuf.text);
	return 0;
}

int message::recv_data(int msg_id,int msg_type,char *msg_data)
{
	msgid=msg_id;
	my_msgbuf.type=msg_type;
	err=msgrcv(msgid,&my_msgbuf,DATA_LEN,my_msgbuf.type,0);
	if(err==-1)
	{
		perror("ERROR:message msgrcv error");
		return -1;
	}
	strncpy(msg_data,my_msgbuf.text,DATA_LEN);
	printf("message msgrcv success:%s\n",msg_data);
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
