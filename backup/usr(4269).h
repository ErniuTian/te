#ifndef USR_H
#defien USR_H

#include <iostream>
#include "manage.h"

class usr
{
	private:
		int err;
		int u_msgid;
		int box_id[CLIENT_NUM];
		manage manager;
		

	public:
		usr();
		int init(int *msgid); 
		int connect_test(int box_id);
		int box_add(int box_addr,char *param);
		int box_delete(int box_id);
		int param_set(int box_id,char *param);
		int sensor_add(int box_id,int sensor_id,char *param,char *data);
		int sensor_delete(int box_id,int sensor_id);
		int data_get(int box_id,int sensor_id,char *data);
		int data_get_loop(int box_id,int sensor_id,char *data,int time);
}

#endif