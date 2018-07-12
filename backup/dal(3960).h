

class daq
{
	private:
		int err;
		int box_id[CLIENT_MAX];
		char data[DATA_LEN];
		int msgid;

	public:
		daq();
		int init();
		int connect_test();
		int box_add();
		int box_delete();
		int param_set();
		int sensor_add();
		int sensor_delete();
		int data_get();
		int data_get_loop();

}
