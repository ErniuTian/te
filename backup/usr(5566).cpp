#include "usr.h"

usr::usr()
{
	err=0;
	u_msgid=-1;
	int i;
	err_num=0x00;
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
		while(1)
		{
		printf("usr msg_handle1\n");
		err=manager.msg_handle();
		if(err==-1)
			break;
		}
		exit(0);
	}
	printf("usr init successful\n");
	return u_msgid;
}

int usr::data_transmit(int len_snd,char data_snd[],int len_rcv, char data_rcv[])
{
	int i;
	err=main_msg.send_data(u_msgid, 1, data_snd,len_snd);
	if(err==-1)
	{
		printf("ERROR:usr msg send_data error\n");
		return -1;
	}
	printf("usr msg send data:");
	for(i=0;i<len_snd;i++)
	{
		printf("%02x ",data_snd[i]);
	}
	printf("\n");

	err=main_msg.recv_data(u_msgid, 2, data_rcv,len_rcv);
	if(err==-1)
	{
		printf("ERROR:usr  msg recv_data error\n");
		return -1;
	}
	printf("usrmsg recv data:");
	for(i=0;i<err;i++)
	{
		printf("%02x ",data_rcv[i]);
	}
	printf("\n");

	return err;
}


char usr::box_add()
{
	int i;
	int data_snd_len,data_rcv_len;
	data_snd_len=5;
	data_rcv_len=5;
	char data_snd[data_snd_len];
	char data_rcv[data_rcv_len];
	memset(data_snd,0,data_snd_len);
	memset(data_rcv,0,data_rcv_len);
	err=data_transmit(data_snd_len, data_snd, data_rcv_len, data_rcv);
	if(err==-1)
		{
		printf("ERROR:usr box add error\n");
		err_num=0x09;
		return 0;
		}
	printf("usr box add rcv data:");
	for(i=0;i<err;i++)
	{
		printf("%02X ",data_rcv[i]);
	}
	printf("\n");
	if(data_rcv[0]==0x01)
		{
		printf("FAIL:usr box add failed!\n");
		err_num=data_rcv[0];
		return 0;
		}
	printf("usr box add successful\n");
	return data_rcv[1];
}

int usr::connect_test(char box_id)
{
	char data[10];
	int i;
	for(i=0;i<10;i++)
	{
		data[i]=0x00;
	}
	data[0]=0x01;
	data[1]=box_id;
	data[2]=0x02;
	data[3]=0x3a;
	data[4]=0x3a;
	data[5]=0x0f;
	data[6]=0x01;
	data[7]=0x0f;
	data[8]=0x0d;
	data[9]=0x0a;
	
	err=main_msg.send_data(u_msgid, 1, data,10);
	if(err==-1)
	{
		printf("usr connect_test send data error\n");
		return -1;
	}
	printf("usr connect test send msg:");
	for(i=0;i<10;i++)
	{
		printf("%02X ",data[i]);
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
		printf("%02X ",data[i]);
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
	int i;
	char data[DATA_LEN];
	memset(data,0,DATA_LEN);
	data[0]=0x02;
	data[1]=box_id;
	data[2]=0x01;

	err=main_msg.send_data(u_msgid, 1, data,DATA_LEN);
	if(err==-1)
	{
		printf("usr box_delete send data error\n");
		return -1;
	}
	printf("usr box_delete send msg:");
	for(i=0;i<DATA_LEN;i++)
	{
		printf("%02X ",data[i]);
	}
	printf("\n");

	err=main_msg.recv_data(u_msgid, 2, data,DATA_LEN);
	if(err==-1)
	{
		printf("usr box_delete recv msg error\n");
		return -1;
	}
	printf("usr box_delete recv msg:");
	for(i=0;i<DATA_LEN;i++)
	{
		printf("%02X ",data[i]);
	}
	printf("\n");

	if(data[0]==0x00)
	{
		printf("usr box:%d delete successful\n",box_id);
		return 0;
	}
	printf("ERROR:usr box:%d delete error\n",box_id);
	return -1;
}
int usr::param_set(char box_id,char *param)
{
	int i;
	char data[DATA_LEN];
	memset(data,0,DATA_LEN);
	memcpy(data+7,param,40);
	data[0]=0x01;
	data[1]=box_id;
	data[2]=0x03;
	data[3]=0x3a;
	data[4]=0x3a;
	data[5]=0x0f;
	data[6]=0x07;
	data[48]=0x0f;
	data[49]=0x0d;
	data[50]=0x01;

	err=main_msg.send_data(u_msgid, 1, data,DATA_LEN);
	if(err==-1)
	{
		printf("usr param_set send data error\n");
		return -1;
	}
	printf("usr param_set send msg:");
	for(i=0;i<DATA_LEN;i++)
	{
		printf("%02X ",data[i]);
	}
	printf("\n");
	err=main_msg.recv_data(u_msgid, 2, data,DATA_LEN);
	if(err==-1)
	{
		printf("usr param_set recv msg error\n");
		return -1;
	}
	printf("usr param_set recv msg:");
	for(i=0;i<DATA_LEN;i++)
	{
		printf("%02X ",data[i]);
	}
	printf("\n");

	if(data[0]==0x00)
	{
		printf("usr box:%d param_set  successful\n",box_id);
		return 0;
	}
	printf("ERROR:usr box:%d param_set error\n",box_id);
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

