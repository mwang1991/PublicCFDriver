#include "pch.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "CFDriver.h"
using namespace std;



string CFDriver::init_server()
{
#ifdef _WIN64
	int len;
	WSAStartup(MAKEWORD(1, 1), &wsa);	//initial Ws2_32.dll by a process
	if ((serversoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)	//create a tcp socket
		return("Create socket fail!\n");


	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9102);
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(serversoc, (SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)
		return("Bind fail!\n");
	

	//start listen, maximum length of the queue of pending connections is 1
	printf("Start listen...\n");
	if (listen(serversoc, 1) != 0)
		return("Listen fail!\n");

	len = sizeof(SOCKADDR_IN);

	//waiting for connecting
	if ((clientsoc = accept(serversoc, (SOCKADDR *)&clientaddr, &len)) <= 0)
		return("Accept fail!\n");
	return("Connected\n");
#endif

}

string CFDriver::init_client(const char* adress, int port)
{
#ifdef _WIN64
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return("connect error !");
	}
	while (true) {
		clientsoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (clientsoc == INVALID_SOCKET)
		{
			return("connect error !");

		}
		sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(port);
		serAddr.sin_addr.S_un.S_addr = inet_addr(adress);
		if (connect(clientsoc, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			closesocket(clientsoc);
			return("connect error !");
		}
#endif
		return "Connected";
	}
}

string CFDriver::send_cmd(char command)
{
	char * buf = &command;
	cfdsend( buf, 1);
	return string();
}

string CFDriver::recv_cmd(char & command)
{
	char buf[BUFLEN];
	cfdsend(buf, BUFLEN);
	command = buf[0];
	return string();
}

string CFDriver::send_data( void * buf, int len, int size, string name)
{

	int     left = 0;
	int     count = 0;
	left = len * size;
	//char    revbuf[BUFLEN];

	cfdsend(name.c_str(), name.length() + 1);
	cfdsend((char*)left, 4);

	while (left > 1024)
	{
		cfdsend((char*)buf + count, 1024);
		count += 1024;
		left -= 1024;
	}
	cfdsend((char*)buf + count, left);

	return("Send Finish\n");

}

string CFDriver::recv_data(void * buf, int &len, int size, string &name)
{

	char bufname[BUFLEN];
	int namelen;
	int left = 0;
	int count = 0;

	namelen = cfdrecv(bufname, BUFLEN);
	name = string(bufname);
	cfdrecv((char *)left, 4);

	len = left / size;
	while (left > BUFLEN)
		{
			cfdrecv((char *)buf + count, BUFLEN);

			left -= BUFLEN;
			count += BUFLEN;
		}
		cfdrecv((char *)buf + count, left);

	return "All data received\n";

}

string CFDriver::send_file(string file)
{
	return string();
}

int CFDriver::cfdsend(const char * buf, int len)
{
#ifdef _WIN64
	return send(clientsoc, buf, len, 0);
#endif
}

int CFDriver::cfdrecv(char * buf, int len)
{
#ifdef _WIN64
	return recv(clientsoc, buf, len, 0);
#endif
	
}
