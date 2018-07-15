#ifndef MESSAGE_H
#define MESSAGE_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY_PATH "./key1"
#define DATA_LEN 3
class message
{
	private:
		key_t key;
		int msgid;
		struct msg_buf
		{
			long type;
			char text[DATA_LEN];
		};
		char *buff;
		struct msg_buf my_msgbuf;
		int err;
	public:
		message();
		int msg_create();
		int send_data(int msg_id,int mtype,char *msg_data);
		int recv_data(int msg_id,int mtype,char *msg_data);
		int msg_delete();
};
#endif
