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
	/*
	for(i=0;i<DATA_LEN;i++)
	{
		data[i]=0x00;
	}
	*/
}

int usr::init()
{
	printf("usr initing...\n");
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
//		while(1)
//		{
		printf("usr msg_handle1\n");
		manager.msg_handle();
			sleep(1);
		printf("usr msg_handle2\n");
			manager.msg_handle();
//		}
		exit(0);
	}
	printf("usr init successful\n");
	return u_msgid;
}

int usr::box_add()
{
	int data[DATA_LEN];
	int i;
	for(i=0;i<DATA_LEN;i++)
	{
		data[i]=0x00;
	}
	err=main_msg.send_data(u_msgid, 1, data,DATA_LEN);
	if(err==-1)
	{
		printf("ERROR:usr box_add msg send_data error\n");
		return -1;
	}
	printf("usr box_add msg send data:");
	for(i=0;i<DATA_LEN;i++)
	{
		printf("%02x ",data[i]);
	}
	printf("\n");

	err=main_msg.recv_data(u_msgid, 2, data,DATA_LEN);
	if(err==-1)
	{
		printf("ERROR:usr box_add msg recv_data error\n");
		return -1;
	}
	printf("usr box_add msg recv:");
	for(i=0;i<DATA_LEN;i++)
	{
		printf("%02x ",data[i]);
	}
	printf("\n");

	if(data[0]==0x01)
	{
		printf("ERROR:usr box add error\n");
		return -1;
	}
	printf("usr box add successful\n");
	return data[1];
}

int usr::connect_test(int box_id)
{
	int data[DATA_LEN];
	int i;
	for(i=0;i<DATA_LEN;i++)
	{
		data[i]=0x00;
	}
	data[0]=0x01;
	data[1]=box_id;
	data[2]=0x02;
	data[3]=0x3a;
	data[4]=0x3a;
	data[5]=0xff;
	data[6]=0x01;
	data[7]=0xff;
	data[8]=0x0d;
	data[9]=0x0a;
	
	err=main_msg.send_data(u_msgid, 1, data,DATA_LEN);
	if(err==-1)
	{
		printf("usr connect_test send data error\n");
		return -1;
	}
	printf("usr connect test send msg:");
	for(i=0;i<DATA_LEN;i++)
	{
		printf("%02x ",data[i]);
	}
	printf("\n");

	err=main_msg.recv_data(u_msgid, 2, data,DATA_LEN);
	if(err==-1)
	{
		printf("usr connect_test recv msg error\n");
		return -1;
	}
	printf("usr connect test recv msg:");
	for(i=0;i<DATA_LEN;i++)
	{
		printf("%02x ",data[i]);
	}
	printf("\n");

	if(data[0]==0x01)
	{
		printf("usr box %d connect test error\n",box_id);
		return -1;
	}
	printf("usr box %d connect test succesful\n",box_id);
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

