#pragma once
#include<string>
#include <stdio.h> 
#include <winsock2.h> 
#pragma comment(lib,"ws2_32.lib")  

#define BUFLEN 1024

class CFDriver
{
public:
//	CFDriver();
//	~CFDriver();

//	std::string init_server();

	std::string init_server(int port);
	std::string init_client(const char* adress, int port);
	std::string send_cmd(char command);
	std::string recv_cmd(char & command);
	std::string send_data(void *buf, int len, int size, std::string name);
	std::string recv_data(void *buf, int &len, int size, std::string &name);
	std::string send_file(std::string file);
private:
#ifdef linux
	int sockID;
#endif
#ifdef _WIN64

	SOCKET serversoc;
	SOCKET clientsoc;
	SOCKET soc;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
	WSADATA wsa;
#endif

	int cfdsend(const char* buf, int len);
	int cfdrecv(char* buf, int len);
};

