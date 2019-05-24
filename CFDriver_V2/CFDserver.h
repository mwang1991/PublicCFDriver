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
    ///Send a file.
    int send_file(std::string file);
    ///Send integer or float data chain
    int send_data(void *buf, int len, std::string name, int type);
    ///Initialize the server
    int init_server();
    ///close the server
    void close_server();
    //void enable_send();
};

void* pth_recv(void* obj) ;
int recv_data(int sockId, int type);
