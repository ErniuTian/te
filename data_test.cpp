#include <stdio.h>
#include <string.h>
#include <math.h>


int main()
{
	int i,j;
	double data[2];
	char data_rcv[21];
	int point[2];
	float temp;
	data[0]=0;
	data[1]=0;

//	data[0]=pow(10,3);
//	printf("data:%f\n",data[0]);
	data_rcv[0]=0x00;
	data_rcv[1]=0x01;
	data_rcv[2]=0x00;
	data_rcv[3]=0x00;
	data_rcv[4]=0x00;
	data_rcv[5]=0x2b;
	data_rcv[6]=0x38;
	data_rcv[7]=0x34;
	data_rcv[8]=0x36;
	data_rcv[9]=0x2e;
	data_rcv[10]=0x32;
	data_rcv[11]=0x34;
	data_rcv[12]=0x32;
	data_rcv[13]=0x2d;
	data_rcv[14]=0x32;
	data_rcv[15]=0x39;
	data_rcv[16]=0x37;
	data_rcv[17]=0x34;
	data_rcv[18]=0x2e;
	data_rcv[19]=0x39;
	data_rcv[20]=0x33;

	
	for(i=0;i<7;i++)
	{
		if(data_rcv[6+i]=='.')
			point[0]=i;
		if(data_rcv[14+i]=='.')
			point[1]=i;
	}
	printf("usr data get point1:%d, point2:%d\n",point[0],point[1]);
	for(j=0;j<2;j++)
	{
		for(i=0;i<point[j];i++)
		{
			data[j]=data[j]+(data_rcv[6+i+8*j]-0x30)*pow(10,(point[j]-i-1));
			printf("data%d:%f\n",j,data[j]);
			printf("\n");
		}
	}
	for(j=0;j<2;j++)
	{
		printf("data%d:%f\n",j,data[j]);
		for(i=0;i<6-point[j];i++)
		{
			temp=(float)(data_rcv[12-i+8*j]-0x30);
			data[j]=data[j]+temp*pow(10,(point[j]-6+i));
			printf("temp:%f\n",temp);
			printf("zhishu:%f\n",pow(10,point[j]-6+i));
			printf("data%d:%f\n",j,data[j]);
			printf("\n");
		}	
	}
	if(data_rcv[5]=='-')
		data[0]=data[0]*(-1);
	if(data_rcv[13]=='-')
		data[1]=data[1]*(-1);

	printf("data1:%f, data2:%f\n",data[0],data[1]);
	return 0;

}
