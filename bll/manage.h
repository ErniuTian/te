#ifndef MANAGE_H
#define MANAGE_H

#include <iostream>
#include "../dal/tcp_link.h"
#include "../dal/process.h"
#include "../dal/message.h"
#include <string.h>

#define CLIENT_NUM 5

class manage
{
	private:
		int err;
		int pid;
		int num;
		int parent_data_len;
		int parent_back_data_len;
		int client_fd[CLIENT_NUM];
		int child_msgid;
		int parent_msgid;
		//char child_data[CHILD_DATA_LEN];
		char parent_data[DATA_LEN];
		char parent_back_data[DATA_LEN];
		tcp_link server_sock;
		Process child[CLIENT_NUM];
		message child_msg;
		message parent_msg;

		
		int client_add();//tcp_accept  p_fork
		int client_delete();
		int talkback();
		int manage_exit();
		int num_get();
		int data_back(int len,char data[]);

	public:
		manage();
		~manage();
		int create();
		int msg_handle();

};

#endif
