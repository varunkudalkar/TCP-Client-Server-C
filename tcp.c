#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

// error handling
void error(char *msg)
{
    perror(msg);
    exit(0);
}

// void ReadMessage(int socket,char * buffer) {
//     int i,n;

//     n = recv(socket,buffer,256,MSG_PEEK);
//     for(i=0;i<256;i++){
// 	    if(strcmp(&buffer[i],"\0") == 0) break;
//     }
//     n = recv(socket,buffer,i+1,0);
//     if (n < 0) 
//          error("ERROR reading from socket");
//     //printf("ReadMessage:%s %d\n",buffer,n);
// }


// setup client socket connection
int ClientSocket(int argc, char** argv)
{
    int sockfd;
    int portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    // setup client socket connection
    // if (argc < 3) {
    //    fprintf(stderr,"usage: %s hostname port\n", argv[0]);
    //    exit(0);
    // }
    // server name
    server = gethostbyname("34.224.38.188"/*argv[1]*/);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    // port no.
    portno = 8090;//atoi(argv[2]);
    // socket file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    // initiate struct socketaddr_in
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    printf("address: %d\n",(int)serv_addr.sin_port);
    // cast sockaddr_in to sockaddr
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    return sockfd;
}

int SendMessage(int socketfd, char *message) 
{
    int n;
    strcat(message, "\0000");
    // Write message back to client
    //n = write(socketfd,message,(strlen(message)+1));
    n = send(socketfd,message,(strlen(message)+1),0);
    if (n<0)
        error("Error sending to server\n");
    else if(n == 0)
        printf("Server closed the socket.\n");
    
    return n;
}

// main client socket
int main(int argc, char *argv[])
{
    char buffer[] = "{\"type\": \"publish\", \"address\": \"panda.12345\", \"headers\": {\"type\": \"text\", \"size\": \"small\"}, \"body\": {\"key\": \"value\"}}";

    // setup client socket connection
    int sockfd;
    sockfd = ClientSocket(argc, argv);
    printf("SendMessage: %d\n",SendMessage(sockfd, buffer));
    // while(1){
	//     printf("Please enter the message: ");
	//     bzero(buffer,256);
	//     fgets(buffer,255,stdin);
	//     SendMessage(sockfd, buffer);
    // }

    return 0;
}
