#include "usr.h"

usr::usr()
{
	err=0;
	u_msgid=-1;
	int i;
	for(i=0;i<CLIENT_NUM;i++)
	{
		box_id[i]=-1;
	}
}

int usr::init()
{
	pid_t pid;
		u_msgid=manager.create();
		if(u_msgid==-1)
		{
			printf("ERROR:usr get a bad u_msgid\n");
			exit(-1);
		}
		printf("usr manage create successful u_msgid:%d\n",u_msgid);
	pid=fork();
	if(pid==-1)
	{
		printf("ERROR:usr fork error\n");
		return -1;
	}

	if(pid==0)
	{
		printf("usr waiting for msg...\n");
	//	while(1)
	//	{
			manager.msg_handle();
	//	}
		exit(0);
	}
	printf("usr init successful\n");
	return 0;
	
}

int usr::box_add(char *box_id)
{
	char data[PARENT_DATA_LEN];
	memset(data,'0',PARENT_DATA_LEN);
	printf("usr box_add data:%s\n",data);
	//memset(box_id,'0')
	err=main_msg.send_data(u_msgid, 1, data);
	if(err==-1)
	{
		printf("ERROR:usr box_add msg send_data error\n");
		return -1;
	}
	err=main_msg.recv_data(u_msgid, 2, data);
	if(err==-1)
	{
		printf("ERROR:usr box_add msg recv_data error\n");
		return -1;
	}
	if(data[0]=='1')
	{
		printf("ERROR:usr box add error\n");
		return -1;
	}
	printf("box add successful\n");
	strncpy(box_id,data+1,1);
	//strncpy(box_addr,data+5,1);
	return 0;
}

int usr::connect_test(char box_id)
{
	char data[PARENT_DATA_LEN];
	memset(data,'0',PARENT_DATA_LEN);
	data[0]='1';
	data[2]=box_id;
	data[3]='2';
	data[4]=':';
	data[5]=':';
	data[6]='f';
	data[7]='1';
	data[8]='f';
	data[9]='\n';
	data[10]='\r';
	main_msg.send_data(u_msgid, 1, data);
	main_msg.recv_data(u_msgid, 2, data);
	if(data[0]=='1')
	{
		printf("box %c connect test error\n",box_id);
		return -1;
	}
	printf("box %c connect test succesful\n",box_id);
	return 0;
}


int usr::box_delete(char box_id)
{
/*
子进程关闭客户端
子进程退出
*/
}
int usr::param_set(char box_id,char *param)
{
}

int usr::sensor_add(char box_id,char sensor_id,char *param,char *data)
{
}
int usr::sensor_delete(char box_id,char sensor_id)
{
}

int usr::data_get(char box_id,char sensor_id,char *data)
{
}

int usr::data_get_loop(char box_id,char sensor_id,char *data,int time)
{
}

