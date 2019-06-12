#include"iCFDriver.h"
using namespace std;

int main()
{	
	cout << "Hello World!\n" << endl;
	InitServer(5099);


	while(1){
		char c;

		RecvCommand(c);
		switch (c)
		{
		case '0':
			printf("Command for simulation to initialize received\n");
			break;
		case '1':
			printf("Command for simulation to start received\n");
			break;
		case '2':
			printf("Command for simulation to stop received\n");
			break;
		case '3':
			printf("Command for simulation to continue received\n");
			break;
		case 'f':
			RecvFile();
			return 0;
			break;
		default:
			printf("No command from client\n");
			break;
		}
	}



}