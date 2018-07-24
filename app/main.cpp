#include "usr.h"

int main()
{
	usr my;
	int box_id;
	int err;

	err=my.init();
	if(err==-1)
	{
		printf("ERROR:main my init error\n");
		return -1;
	}
	printf("main my init successful\n");
	sleep(3);

	box_id=my.box_add();
	if(box_id==-1)
	{
		printf("ERROR:main box add error\n");
		return -1;
	}
	printf("main my box_add %d successful\n",box_id);

	sleep(1);
	printf("main box:%d connect testing...\n",box_id);
	err=my.connect_test(box_id);
	if(err==-1)
	{
		printf("ERROR:main connect test error\n");
		return -1;
	}
	printf("main my connect_test successful\n");
	/*
	box_id=my.box_add();
	if(box_id==-1)
	{
		printf("ERROR:main box add error\n");
		return -1;
	}
	printf("main my box_add %d successful\n",box_id);

	sleep(1);
	printf("main box:%d connect testing...\n",box_id);
	err=my.connect_test(box_id);
	if(err==-1)
	{
		printf("ERROR:main connect test error\n");
		return -1;
	}
	printf("main my connect_test successful\n");
*/
	return 0;
}
