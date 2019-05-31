#include "stdafx.h"

#include "CFDriver.h"
using namespace std;

 string CFDriver::init_server(){
    struct sockaddr_in serv_addr, cli_addr;
    int portno;
    int clilen;

    int serverSockId = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSockId == -1)    return("Error creating socket\n");
    printf("Server socket created\n");
    portno = 5099;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(serverSockId, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) return("ERROR on binding");
    listen(serverSockId,2);
    clilen = sizeof(cli_addr);
    printf("Waiting for client\n");
    SockId = accept(serverSockId, (struct sockaddr *) &cli_addr, &clilen);
    printf("Client: %s connected\n", inet_ntoa(cli_addr.sin_addr));


    return "Server stand by";
}