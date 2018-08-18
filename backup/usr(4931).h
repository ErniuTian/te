#ifndef USR_H
#define USR_H

#include <iostream>
#include "../bll/manage.h"

class usr
{
	private:
		int err;
		int u_msgid;
		char box_id[CLIENT_NUM];
		char err_num;
		
		manage manager;
		message main_msg;

		int data_transmit(int len_snd,char data_snd[],int len_rcv, char data_rcv[]);		

	public:
		usr();
		int init(); 
		int connect_test(char box_id);
		char box_add();
		int box_delete(char box_id);
		int param_set(char box_id,char channel, char param);
		int sensor_add(char box_id,char sensor_id,char *param,char *data);
		int sensor_delete(char box_id,char sensor_id);
		int data_get(char box_id,char sensor_id,char *data);
		int data_get_loop(char box_id,char sensor_id,char *data,int time);
};

#endif
