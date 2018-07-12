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
		message main_msg;
		

	public:
		usr();
		int init(int *msgid); 
		int connect_test(char box_id);
		int box_add(char box_addr,char *param);
		int box_delete(char box_id);
		int param_set(char box_id,char *param);
		int sensor_add(char box_id,char sensor_id,char *param,char *data);
		int sensor_delete(char box_id,char sensor_id);
		int data_get(char box_id,char sensor_id,char *data);
		int data_get_loop(char box_id,char sensor_id,char *data,int time);
}

#endif