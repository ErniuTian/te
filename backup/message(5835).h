#ifndef MESSAGE_H
#define MESSAGE_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY_PATH "./key1"
#define DATA_LEN 51
#define CHILD_DATA_LEN DATA_LEN-2
#define BOX_DATA_LEN DATA_LEN-3 
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
		~message();
		int msg_create(char *keypath);
		int send_data(int msg_id,int mtype,char msg_data[], int len);
		int recv_data(int msg_id,int mtype,char msg_data[], int len);
		int msg_delete();
};
#endif
