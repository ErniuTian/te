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
	char id;
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
	
	if(data_rcv[0]==0x00)
		{
		printf("usr box add successful\n");
		id=data_rcv[1];
		return id;
		}

	printf("FAIL:usr box add failed!\n");
	err_num=data_rcv[0];
	return 0;
}

int usr::connect_test(char box_id)
{
	int i;
	int data_snd_len,data_rcv_len;
	data_snd_len=5;
	data_rcv_len=7;

	char data_snd[data_snd_len];
	char data_rcv[data_rcv_len];
	memset(data_snd,0,data_snd_len);
	memset(data_rcv,0,data_rcv_len);
	data_snd[0]=0x01;	//tell manage what to do
	data_snd[1]=box_id;
	data_snd[2]=0x01;	//tell process what to do
	data_snd[3]=0x00;	//box_addr 
	data_snd[4]=0x00;	//length of the data that need to be transfered to box

	err=data_transmit(data_snd_len, data_snd, data_rcv_len, data_rcv);

	if(data_rcv[0]==0x01)
	{
		printf("usr box %d connect test error\n",box_id);
		return -1;
	}
	printf("usr box %d connect test succesful,box_type:%02X,box_vertion:%02X\n",box_id,data_rcv[5],data_rcv[6]);
	return 0;
}


int usr::box_delete(char box_id)
{
	int i;
	char id;
	int data_snd_len,data_rcv_len;
	data_snd_len=5;
	data_rcv_len=5;

	char data_snd[data_snd_len];
	char data_rcv[data_rcv_len];
	memset(data_snd,0,data_snd_len);
	memset(data_rcv,0,data_rcv_len);

	data_snd[0]=0x02;
	data_snd[1]=box_id;

	err=data_transmit(data_snd_len, data_snd, data_rcv_len, data_rcv);
	if(err==-1)
		{
		printf("ERROR:usr box_delete data transimit error\n");
		return -1;
		}
	
	if(data_rcv[0]==0x00)
	{
		printf("usr box:%d delete successful\n",box_id);
		return 0;
	}
	printf("ERROR:usr box:%d delete error\n",box_id);
	return -1;
}

int usr::param_set(char box_id,char channel, char param)
{
	int i;
	int data_snd_len,data_rcv_len;
	data_snd_len=7;
	data_rcv_len=5;

	char data_snd[data_snd_len];
	char data_rcv[data_rcv_len];
	memset(data_snd,0,data_snd_len);
	memset(data_rcv,0,data_rcv_len);

	data_snd[0]=0x01;
	data_snd[1]=box_id;
	data_snd[2]=0x07;
	data_snd[3]=0x00;
	data_snd[4]=0x02;
	data_snd[5]=channel;
	data_snd[6]=param;	

	err=data_transmit(data_snd_len, data_snd, data_rcv_len, data_rcv);
	if(err==-1)
		{
		printf("ERROR:usr param set error\n");
		return -1;
		}
	if(data_rcv[0]==0x00)
		{
		printf("usr param ser successful\n");
		return 0;
		}
	else 
		{
		printf("FAIL:usr param set failed\n");
		err_num=data_rcv[0];
		return -1;
		}
}

int usr::params_set(char box_id, char param[40])
{
	int i;
	int data_snd_len,data_rcv_len;
	data_snd_len=45;
	data_rcv_len=5;

	char data_snd[data_snd_len];
	char data_rcv[data_rcv_len];
	memset(data_snd,0,data_snd_len);
	memset(data_rcv,0,data_rcv_len);

	data_snd[0]=0x01;
	data_snd[1]=box_id;
	data_snd[2]=0x77;
	data_snd[3]=0x00;
	data_snd[4]=0x02;
	memcpy(data_snd+5,param,40);

	err=data_transmit(data_snd_len, data_snd, data_rcv_len, data_rcv);
	if(err==-1)
		{
		printf("ERROR:usr param set error\n");
		return -1;
		}
	if(data_rcv[0]==0x00)
		{
		printf("usr param ser successful\n");
		return 0;
		}
	else 
		{
		printf("FAIL:usr param set failed\n");
		err_num=data_rcv[0];
		return -1;
		}
}


int usr::sensor_add(char box_id,char sensor_id,char *param,char *data)
{
}
int usr::sensor_delete(char box_id,char sensor_id)
{
}

int usr::data_get(char box_id,char sensor_id,float data[])
{
	int i,j,point[2];
	int data_snd_len,data_rcv_len;
	data_snd_len=6;
	data_rcv_len=21;

	data[0]=0;
	data[1]=0;
	char data_snd[data_snd_len];
	char data_rcv[data_rcv_len];
	memset(data_snd,0,data_snd_len);
	memset(data_rcv,0,data_rcv_len);

	data_snd[0]=0x01;
	data_snd[1]=box_id;
	data_snd[2]=0x21;
	data_snd[3]=0x00;
	data_snd[4]=0x01;
	data_snd[5]=sensor_id;

	err=data_transmit(data_snd_len, data_snd, data_rcv_len, data_rcv);
	if(err==-1)
		{
		printf("ERROR:usr param set error\n");
		return -1;
		}
	if(data_rcv[0]==0x00)//data_rcv[5]-data_rcv[12]  data_rcv[13]-data_rcv[20]
		{
		printf("usr param ser successful\n");
		for(i=0;i<7;i++)
			{
			if(data_rcv[6+i]=='.')
				point[0]=i;
			if(data_rcv[14+i]=='.')
				point[1]=i;
			}
		for(j=0;j<2;j++)
			{
			for(i=0;i<point[j];i++)
				{
				data[j]=data[j]+(data_rcv[6+i]-0x30)*(10^(point[j]-i));
				}
			}
		if(data_rcv[5]=='-')
			data[0]=data[0]*(-1);
		if(data_rcv[13]=='-')
			data[1]=data[1]*(-1);
		
		return 0;
		}
	else 
		{
		printf("FAIL:usr param set failed\n");
		err_num=data_rcv[0];
		return -1;
		}
}

int usr::data_get_loop(char box_id,char sensor_id,char *data,int time)
{
}

