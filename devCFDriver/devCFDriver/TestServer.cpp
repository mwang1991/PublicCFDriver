#include"iCFDriver.h"
using namespace std;

int main()
{	
	char c = 'a';
	cout << "Hello World!\n" << endl;
	InitServer(5099);
	RecvCommand(c);
	switch (c)
	{
	case '0':
		printf("Command for simulation initialization received\n");
		break;
	default:
		printf("No command from client\n");
		break;
	}

}