#ifndef	PROCESS_H
#define PROCESS_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include "message.h"


class Process
{
	private:
		int err;
		int msgid;
		int p_mtype;
		char msg_data[CHILD_DATA_LEN];
		char msg_back_data[CHILD_DATA_LEN];
		message p_msg;
		pid_t pid;
		int sv[2];
		int CONTROL_LEN;
		int client_fd;
		struct msghdr msg;
		struct iovec iov[1];
		struct cmsghdr cmsg;
		//char *buf;

		int msg_handle();
		int talkback();
		int link_test();
	
	public:
		Process();
		~Process();
		int p_fork(int client_fd,int msg_id,int msg_type);
		int p_sendfd(int ssv, int fd);
		int p_recvfd(int rsv);
		int p_close();
		
};
#endif
