
#ifdef linux
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>


#endif

 
#define    MAXLINE        1024
class CFDriver{
private:
    int serverSockId;
    pthread_t pth;

public:
    const int typeInt = 1;
    const int typeFloat = 2;
    char recvCmd;
    int sockId;
    bool treatCmd = false;
    bool sendData = true;

    void error(const char *msg);
    //static void recv_cmd();
    int send_file(std::string file);
    int send_data(void *buf, int len, std::string name, int type);
    int init_server();
    void close_server();
    void enable_send();
    void recv_cmd();
};

void* pth_recv(void* obj) ;
int recv_data(int sockId, int type);
