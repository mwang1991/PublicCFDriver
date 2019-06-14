#include "stdafx.h"

#ifdef _WIN64

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#endif
#include "CFDriver.h"

using namespace std;



string CFDriver::init_server(int port)
{

#ifdef _WIN64
	freopen("debug.txt", "w", stdout);
	printf("Starting server...\n");
	fflush(stdout);

	int len;
	WSAStartup(MAKEWORD(1, 1), &wsa);	//initial Ws2_32.dll by a process
	if ((serversoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)	//create a tcp socket
		return("Create socket fail!\n");

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
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
	printf("Client: %s connected\n", inet_ntoa(clientaddr.sin_addr));
	fflush(stdout);
	return("Connected\n");
#endif
#ifdef linux
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;

	int serverSockId = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSockId == -1)    return("Error creating socket\n");
	printf("Server socket created\n");
	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	if (bind(serverSockId, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) return("ERROR on binding");
	listen(serverSockId, 2);
	clilen = sizeof(cli_addr);
	printf("Waiting for client\n");
	sockId = accept(serverSockId, (struct sockaddr *) &cli_addr, &clilen);
	printf("Client: %s connected\n", inet_ntoa(cli_addr.sin_addr));
	int opt = 1;
	return ("Connected\n");
#endif
}

void CFDriver::init_client(const char* adress, int port)
{

#ifdef _WIN64
	freopen("debug.txt", "w", stdout);

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		printf("connect error !\n");
	}
	clientsoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientsoc == INVALID_SOCKET)
	{
		printf("connect error !\n");

	}
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);
	serAddr.sin_addr.S_un.S_addr = inet_addr(adress);
	if (connect(clientsoc, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		closesocket(clientsoc);
		printf("connect error !\n");
	}
#endif
#ifdef linux
    struct sockaddr_in serv_addr; 
    if((sockId = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return("\n Error : Could not create socket \n");
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 

    if(inet_pton(AF_INET, adress, &serv_addr.sin_addr)<=0)
    {
        return("\n inet_pton error occured\n");
    } 

    if( connect(sockId, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       return("\n Error : Connect Failed \n");
    } 
#endif


	printf( "Connected\n");
	
}

string CFDriver::send_cmd(char command)
{	
	char buf[BUFLEN];
	buf[0] = command;
	cfdsend( buf, 1);
	return string();
}

string CFDriver::recv_cmd(char & command)
{
	char buf[BUFLEN];
	cfdrecv(buf, 1);
	command = buf[0];
	return string();
}

string CFDriver::send_data( void * buf, int len, int size)
{

	int     left[1];
	int     count = 0;
	left[0] = len * size;
	//char    revbuf[BUFLEN];

	//cfdsend(name, sizeof(name));
	cfdsend((char*)left, 4);

	while (left[0] > BUFLEN)
	{
		cfdsend((char*)buf + count, BUFLEN);
		count += BUFLEN;
		left[0] -= BUFLEN;
	}
	cfdsend((char*)buf + count, left[0]);

	return("Send Finish\n");

}

string CFDriver::recv_data(void * buf, int &len, int size)
{

	//int namelen = 0;
	int recvlen = 0;
	int left[1];
	int count = 0;

	//namelen = cfdrecv(name, BUFLEN);
	cfdrecv((char*)left, 4);

	len = left[0] / size;
	while (left[0] > 0)
	{
		if (left[0] > BUFLEN)
		{
			recvlen = cfdrecv((char*)buf + count, BUFLEN);
		}
		else 
		{ 
			recvlen = cfdrecv((char *)buf + count, left[0]);
		}

		left[0] -= recvlen;
		count += recvlen;
	}

	return "All data received\n";

}

string CFDriver::send_file(const char* file)
{
	cfdsend(file, BUFLEN);	//send file name

	int filelength[1];
	FILE* fp;
	if (fp = fopen(file, "r"))
	{
		fseek(fp, 0, SEEK_END);
		filelength[0] = ftell(fp);
		fclose(fp);
	}
	else
	{
		printf("File error");
	}
	printf("File length is %d", filelength[0]);
	cfdsend((char*)filelength, 4);

	ifstream sourcefile(file, ios::in | ios::binary);
	int left = filelength[0];
	char filebuf[BUFLEN + 1];
	while (left > BUFLEN)
	{
		sourcefile.read(filebuf, BUFLEN);
		cfdsend(filebuf, BUFLEN);
		left -= BUFLEN;
	}
	sourcefile.read(filebuf, left);
	cfdsend(filebuf, left);
	return("Send Finish\n");
}

std::string CFDriver::recv_file()
{
	char buf[BUFLEN];
	int namelen = 0;
	int recvlen = 0;
	int left = 0;
	int filelength[1];
	char filename[BUFLEN] = { 0 };
	namelen = cfdrecv(buf, BUFLEN);
	int i = 0;
	for (i; i <= namelen; i++)
	{
		filename[i] = buf[i];
	}
	filename[i] = '\0';
	printf("File name is ");
	printf(filename);
	printf("\n");
	
	cfdrecv((char*)filelength, 4);
	printf("File length is %d\n",filelength[0]);

	ofstream rec_file;
	rec_file.open(filename, ios::binary);
	left = filelength[0];
	while (left > 0)
	{
		recvlen = cfdrecv(buf, BUFLEN);
		rec_file.write(buf, recvlen);
		left -= recvlen;
	}

	printf("All data received");

	return std::string();
}

void CFDriver::disconnect()
{
	closesocket(clientsoc);
}



int CFDriver::cfdsend(const char * buf, int len)
{
#ifdef _WIN64
	return send(clientsoc, buf, len, 0);
#endif
#ifdef linux
	return send(sockId, buf, len, 0);
#endif
}

int CFDriver::cfdrecv(char * buf, int len)
{
#ifdef _WIN64
	return recv(clientsoc, buf, len, 0);
#endif
#ifdef linux
	return recv(sockId, buf, len, 0);
#endif
	
}
