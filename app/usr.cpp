#include <usr.h>

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
	pid=fork();
	if(pid==-1)
	{
		printf("ERROR:usr fork error\n");
		return -1;
	}
	if(pid==0)
	{
		err=manager.create(&u_msgid);
		if(err==-1)
		{
			printf("ERROR:usr manage create error\n");
			return -1;
		}
		if(u_msgid==-1)
		{
			printf("ERROR:usr get a bad u_msgid\n");
			return -1;
		}

		while(1)
		{
			manager.msg_handle();
		}
		return 0;
	}
	printf("usr init successful\n");
	return 0;
	
}

int usr::box_add(char *box_id)
{
	char data[PARENT_DATA_LEN];
	memset(data,'0',PARENT_DATA_LEN);
	//memset(box_id,'0')
	main_msg.send_data(u_msgid, 1, data);
	main_msg.recv_data(u_msgid, 2, data);
	if(data[0]=='1')
	{
		printf("box add error\n");
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
	data[6]='ff';
	data[7]='01';
	data[8]='ff';
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


int usr::box_delete(int box_id)
{
}
int usr::param_set(int box_id,char *param)
{
}

int usr::sensor_add(int box_id,int sensor_id,char *param,char *data)
{
}
int usr::sensor_delete(int box_id,int sensor_id)
{
}

int usr::data_get(int box_id,int sensor_id,char *data)
{
}

int usr::data_get_loop(int box_id,int sensor_id,char *data,int time)
{
}

