#ifndef MANAGE_H
#define MANAGE_H

#include <iostream>
#include "server.h"
#include "process.h"
#include "message.h"
#include <string.h>

#define CLIENT_NUM 5

class manage
{
	private:
		int pid;
		int num;
		int client_fd[CLIENT_NUM];
		int child_msgid;
		int parent_msgid;
		char child_data[DATA_LEN];
		char parent_data[DATA_LEN];
		tcp_link server_sock;
		process child[CLIENT_NUM];
		message child_msg;
		message parent_msg;

		int msg_handle();
		int client_add();//tcp_accept  p_fork
		int client_delete();
		int talkback();
		int num_get();

	public:
		manage();
		int create();

}

#endif
