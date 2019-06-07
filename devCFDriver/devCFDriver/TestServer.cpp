#include"iCFDriver.h"
using namespace std;

int main()
{	
	char c = 'a';
	cout << "Hello World!\n" << endl;
	InitServer(5099);

	//s.send_cmd(c);
	printf("SetTexture id:%d\n");
	printf("%c", c);

	//cout << "end" << endl;
}