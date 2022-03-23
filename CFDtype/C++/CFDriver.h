#ifdef _WIN64
#include <winsock2.h> 
#pragma once

#pragma comment(lib,"ws2_32.lib")  
#endif
#ifdef linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <fcntl.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string> 
#include <string.h> 
#include <iostream>
#include <fstream>


#define BUFLEN 1024

class CFDriver
{
public:
//	CFDriver();
//	~CFDriver();


	void init_server(int port);
	int init_client(const char* adress, int port);
	void send_cmd(char command);
	void recv_cmd(char & command);
	void send_data(void *buf, int len, int size);
	void recv_data(void *buf, int &len, int size);
	void send_file(const char* file);
	void recv_file();
	void disconnect();

private:
#ifdef linux
	int sockId;
#endif
#ifdef _WIN64

	SOCKET serversoc;
	SOCKET clientsoc;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
	WSADATA wsa;
	bool init = true;
#endif

	int cfdsend(const char* buf, int len);
	int cfdrecv(char* buf, int len);
};

