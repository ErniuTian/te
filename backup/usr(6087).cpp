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

int usr::init(int *msgid);
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

int usr::box_add(int *box_id,int *box_addr,char *param)
{
	
}

int usr::connect_test(int box_id)
{
	
}


int usr::box_delete(int box_id);
int usr::param_set(int box_id,char *param);
int usr::sensor_add(int box_id,int sensor_id,char *param,char *data);
int usr::sensor_delete(int box_id,int sensor_id);
int usr::data_get(int box_id,int sensor_id,char *data);
int usr::data_get_loop(int box_id,int sensor_id,char *data,int time);

