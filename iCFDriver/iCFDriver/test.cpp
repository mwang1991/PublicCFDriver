#include "iCFDriver.h"
#include <vector>

using namespace std;

int main(int argc,char **argv)
{
	int* ids;
	int sock_id;
   	char file[30];
   	vector<int> t;
   	for(int i = 0; i < 10000; i++)
   		t.push_back(i);
   	InitServer();
   /*
   	CFDriver s;


	for(int i = 1; i<300; i++){
   		memset(&file, '\0', sizeof(file));
   		strcat(file,"test_页面_");
   		if (i<100) strcat(file,"0");
   		if (i<10) strcat(file,"0");
	   	
	   	char temp[4];
	   	sprintf(temp, "%d", i);
	   	strcat(file,temp);
	   	strcat(file,".jpg");
	   	std::cout<<file<<std::endl;
		sendFile(file);
	}
   	//cout<<t.size()<<endl<<sizeof(t)<<endl;
	//send_file("test_002.jpg");
	float a[2000];
	for(int i = 1; i < 1000; i++) a[i] = i + i *0.001;
	//int a[] = {1,2,3};
	float c[] = {3.1,2.1,1.1};
	
	
	//strcpy(file, "012345678901234567890123456789");
	//send_buf(file, sizeof(file));
	
	s.init_server();
	//s.send_data(c, 3,"adfsa", s.typeFloat);
	s.recv_cmd();
	//sleep(10);
	//s.send_data(c, 3, s.typeFloat);
	//s.send_data(a, 1000, "int", s.typeFloat);
	//s.send_file("test_002.jpg");
	s.close_server();*/
	//send_file(newsock_id, "test_002.jpg");
	return 0;
}
