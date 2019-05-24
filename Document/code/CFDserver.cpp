#include "CFDserver.h"
using namespace std;
void CFDserver::error(const char *msg)
{
    perror(msg);
    exit(1);
}

void* pth_recv(void* obj){
    CFDserver *server = (CFDserver*) obj;
    while(true)
    {

        if(server->sendData){
            char revbuf[MAXLINE];
            recv(server->sockId, revbuf, 1, 0);
            server->recvCmd = revbuf[0];
            cout<<revbuf[0]<<endl;
            server->treatCmd = true;
            switch(revbuf[0]){
            case 'i':
                recv_data(server->sockId, 0);
                break;
            case 'f':
                recv_data(server->sockId, 1);
                break;
            }
            //printf("%d", server->sockId);
        }
        sleep(1);
    }
}


int recv_data(int sockId, int type)
{
    int intbuf[MAXLINE/4];
    float floatbuf[MAXLINE/4];
    char name[1024];
    int len[1];
    send(sockId, ".", 1, 0);
    send(sockId, ".", 1, 0);
    cout<<"Ready to receive data"<<endl;
    recv(sockId, len, 4, 0);

    send(sockId, ".", 1, 0);
    cout<<len[0]<<endl;
    int left = len[0];
    int intData[left];
    float floatData[left];
    recv(sockId, name, 1024, 0);
    cout<<name<<endl;
    send(sockId, ".", 1, 0);
    if (type == 0)
    {
        while(left > MAXLINE / 4)
        {
            recv(sockId, intbuf, MAXLINE, 0);
            memcpy(intData + (*len - left) * 4, intbuf, MAXLINE);
            left -= MAXLINE / 4;

        }
        recv(sockId, intbuf, left * 4, 0);
        cout<<intbuf[0]<<endl<<intbuf[1]<<endl<<intbuf[2]<<endl;
        memcpy(intData + (*len - left) * 4, intbuf, left * 4);
    }

        if (type == 1)
    {
        while(left > MAXLINE / 4)
        {
            recv(sockId, floatbuf, MAXLINE, 0);
                    cout<<floatbuf[0]<<endl<<floatbuf[1]<<endl<<floatbuf[2]<<endl;

            memcpy(floatData + (*len - left) * 4, floatbuf, MAXLINE);
            left -= MAXLINE / 4;
        }
        recv(sockId, floatbuf, left * 4, 0);
        cout<<floatbuf[0]<<endl<<floatbuf[1]<<endl<<floatbuf[2]<<endl;
        memcpy(floatData + (*len - left) * 4, floatbuf, left * 4);
    }
    cout<<"All data received"<<endl;

}

int CFDserver::send_file(std::string file){
    int                     read_len;
    int                     send_len;
    FILE                    *fp;
    char                    revbuf[MAXLINE];
    char                    buf[MAXLINE];
    char                    filename[file.length()];
    //printf(ip);
    /* transported the file commented by guoqingbo*/
	//std::cin>>i_ret;
    if(treatCmd){
        printf("Command received, skip sending data");
        return -1;
    }
    sendData = false;
    strcpy(filename, file.c_str());
    //std::cout<<file<<std::endl<<file.length()<<std::endl<<filename<<std::endl<<sizeof(filename)<<std::endl;

    if ((fp = fopen(filename,"r")) == NULL) {
        perror("Open file failed\n");
        exit(0);
    }
    send(sockId, "#", 1, 0);
    recv(sockId, revbuf, 1, 0);

	printf("Ready to send file\n");
	
	//filename[strlen(file)] = '#';
    send(sockId, filename, sizeof(filename), 0);
    recv(sockId, revbuf, sizeof(revbuf), 0);

	printf("%d\n", file.length());
	fseek(fp, 0, SEEK_END);
	int n = ftell(fp);
	//printf("%d\n", sizeof((char*) &n));
	//std::cin>>i_ret;
    char buffer[20];
    sprintf(buffer, "%i", n);
	send(sockId, buffer, strlen(buffer), 0);
    std::cout<<n<<std::endl;
    fclose(fp);

	fp = fopen(filename,"r");
	bzero(buf, MAXLINE);

    while ((read_len = fread(buf, sizeof(char), MAXLINE, fp)) >0 ) {


        send_len = send(sockId, buf, read_len, 0);
        if ( send_len < 0 ) {
            perror("Send file failed\n");
            exit(0);
        }
        bzero(buf, MAXLINE);
    }
    fclose(fp);	    
	
    printf("Send Finish\n");
    sendData = true;
    return 0;
}


int CFDserver::send_data(void *buf, int len, std::string name, int type){
    int     read_len;
    int     send_len;
    int     left;
    int     count = 0;
    char    revbuf[MAXLINE];
    char    dataname[name.length()];


    if(treatCmd){
        printf("Command received, skip sending data");
        return -1;
    }
    sendData = false;
    switch(type){
        case 1: send(sockId, "*i", 2, 0);break;
        case 2: send(sockId, "*f", 2, 0);break;
        default: printf("Wrong data type\n");return 0;
    }
    recv(sockId, revbuf, 2, 0);

    strcpy(dataname, name.c_str());
    send(sockId, dataname, sizeof(dataname), 0);
    recv(sockId, revbuf, sizeof(revbuf), 0);

    left = len * 4;
    char buffer[20];
    sprintf(buffer, "%i", left);
    send(sockId, buffer, strlen(buffer), 0);
    recv(sockId, revbuf, strlen(revbuf), 0);
    send(sockId, ".", 1, 0);
    printf("Ready to send data\n");
    
    while(left > 1024) 
    {
        send(sockId, buf + count, 1024, 0);
        count += 1024;
        left -= 1024;
    }
    send(sockId, buf + count, left, 0);
    //std::cout<<len<<std::endl;
    recv(sockId, revbuf, sizeof(revbuf), 0);
    
    printf("Send Finish\n");
    sendData = true;
    return 0;
}

int CFDserver::init_server(){
    struct sockaddr_in serv_addr, cli_addr;
    int portno;
    socklen_t clilen;

    serverSockId = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSockId == -1)    error("Error creating socket\n");
    printf("Server socket created\n");
    portno = 5099;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(serverSockId, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR on binding");
    listen(serverSockId,2);
    clilen = sizeof(cli_addr);
    printf("Waiting for client\n");
    sockId = accept(serverSockId, (struct sockaddr *) &cli_addr, &clilen);
    printf("Client: %s connected\n", inet_ntoa(cli_addr.sin_addr));
    int ret = pthread_create(&pth, NULL, pth_recv, this);


    return 1;
}

void CFDserver::close_server(){
    send(sockId, "z", 1, 0);
    sleep(1);
    close(sockId);
    close(serverSockId);
    pthread_cancel(pth);
    
    printf("Sockets closed.\n");

}