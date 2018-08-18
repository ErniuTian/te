#include "usr.h"

int main()
{
	usr my;
	int box_id1;
	int box_id2;
	int err;
	char channel;
	char param;

	err=my.init();
	if(err==-1)
	{
		printf("ERROR:main my init error\n");
		return -1;
	}
	printf("main my init successful\n");
	sleep(3);

	box_id1=my.box_add();
	if(box_id1==0x00)
	{
		printf("ERROR:main box add error\n");
		return -1;
	}
	printf("main my box_add %d successful\n",box_id1);
/*
	box_id2=my.box_add();
	if(box_id2==-1)
	{
		printf("ERROR:main box add error\n");
		return -1;
	}
	printf("main my box_add %d successful\n",box_id2);
	*/
	sleep(1);
	printf("main box:%d connect testing...\n",box_id1);
	err=my.connect_test(box_id1);
	if(err==-1)
	{
		printf("ERROR:box:%d main connect test1 error\n",box_id1);
		return -1;
	}
	printf("main my box:%d connect_test1 successful\n",box_id1);

	sleep(1);
	printf("main box:%d param setting...\n",box_id1);
	channel=0x13;
	param=0x34;
	err=my.param_set(box_id1, channel, param);
	if(err==-1)
		{
		printf("ERROR:main param set error\n");
		return -1;
		}
	printf("main box:%d param set sucessful\n",box_id1);
	/*
	printf("main box:%d connect testing...\n",box_id2);
	err=my.connect_test(box_id2);
	if(err==-1)
	{
		printf("ERROR:main box:%d connect test2 error\n",box_id2);
		return -1;
	}
	printf("main my box:%d connect_test2 successful\n",box_id2);
	*/
	printf("main is going to delete box:%d...\n",box_id1);
	err=my.box_delete(box_id1);
	if(err==-1)
	{
		printf("ERROR:main box:%d delete error\n",box_id1);
		return -1;
	}
	printf("main box:%d delete sucessful\n",box_id1);
	/*
	printf("main is going to delete box:%d...\n",box_id2);
	err=my.box_delete(box_id2);
	if(err==-1)
	{
		printf("ERROR:main box:%d delete error\n",box_id2);
		return -1;
	}
	printf("main box:%d delete sucessful\n",box_id2);
*/
	return 0;
}
