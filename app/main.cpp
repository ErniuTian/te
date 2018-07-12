#include <usr.h>

int main()
{
	usr my;
	char box_id;
	int err;

	err=my.init();
	if(err==-1)
	{
		printf("ERROR:main my init error\n");
		return -1;
	}
	printf("main my init successful\n");
	err=my.box_add(&box_id);
	if(err==-1)
	{
		printf("ERROR:main box add error\n");
		return -1;
	}
	printf("main my box_add successful\n");
	err=my.connect_test(box_id);
	if(err==-1)
	{
		prinft("ERROR:main connect test error\n");
		return -1;
	}
	printf("main my connect_test successful\n");
	return 0;
}