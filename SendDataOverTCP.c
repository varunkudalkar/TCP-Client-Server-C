#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MSG 1024
#define SERVER_ADDR "34.224.38.188"
#define CLIENT_ADDR "34.224.38.188"
#define CLIENT_PORT 8090
#define SERVER_PORT 8090

int main() {
	int sd, newSd, n, cliLen;
  int r;
	//socklen_t cliLen;
	struct sockaddr_in cliAddr, servAddr;
	char line[MAX_MSG];
	
	// build server address structure
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	servAddr.sin_port = htons(SERVER_PORT);
	
	// build client address structure
	bzero((char *)&cliAddr, sizeof(cliAddr));
	cliAddr.sin_family = AF_INET;
	cliAddr.sin_addr.s_addr = inet_addr(CLIENT_ADDR);
	cliAddr.sin_port = htons(CLIENT_PORT);
	
	// create stream socket
	sd = socket(AF_INET, SOCK_STREAM, 0);
	printf("successfully created stream socket \n");
	
	// bind local port number
	bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
	printf("bound local port to client successfully\n");
	
	connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
	
	// char *message = "{\"type\": \"publish\",\"address\": \"trackmelayer.12345\",\"body\":{\"message\":\"your message\"}}";
	char *message = "{\"type\":\"publish\",\"address\":\"trackmelayer.12345\",\"headers\":{\"type\":\"text\",\"size\":\"small\"},\"body\":{\"key\":\"value\"}}";
  char buffer[4];
  int length=(int)strlen(message);
  buffer[0] = length >> 24;
  buffer[1] = length >> 16;
  buffer[2] = length >> 8;
  buffer[3] = length;

	send(sd,buffer,4,0);
	send(sd,message,length,0);
	printf("Sent: %s\n",message);
	
  while(1)
  {
    int n = read(sd, line, 256);
    printf("n: %d",n);
    line[n] = 0;
    if(fputs(line, stdout) == EOF)
    {
      printf("\n Error : Fputs error");
    }
    printf("\n");
  }

	printf("Closing connection with the server");
	return 0;
}
