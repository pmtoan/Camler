#ifndef STDSOC_H
#define STDSOC_H
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int initSocket();
int bindToPort(int socket, int port);
void listenClient(int socket, int number);
int say(int socket,const char* message);
int hear(int socket, char *buf, int len);

int initSocket()
{
	/*
	* Create a std socket
	*/
	int s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		puts("[ERROR]{Cwes}   Error start server (step initialize socket), wait a minute or try with another port");
		exit(0);
	}
	return s;
}

int bindToPort(int socket, int port)
{
	/*
	*	Bind a socket with a port number
	*/
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);
    if ((bind(socket, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
    	puts("[ERROR]{Cwes}   Error start server (step bind to port), wait a minute or try with another port");
       	return -1;
    }
    return 1;
}

void listenClient(int socket, int number)
{
	/*
	*	Start listen on socket with number connection
	*/
	listen(socket, number);
}

int say(int socket,const char* message)
{
	/*
	*	Send a string to socket
	*/
	int result = send(socket, message, strlen(message), 0);
	if (result == -1)
	{
		printf("[ERROR]{Cwes}   Error send data to client\n");
	}
	return result;
}
int hear(int socket, char *buf, int len)
{
	/*
	*	Read a string from socket
	*/
	char *s = buf;
	int slen = len;
	int c = recv(socket, s, slen, 0);
	// while ((c > 0) && (s[c-1] != '\n') && (s[c-1] != '\0')) {
	// 	s += c; slen -= c;
	// 	c = recv(socket, s, slen, 0);
	// }
	if (c < 0)
		return c;
	else if (c == 0)
		buf[0] = '\0';
	else
		s[c]='\0';
	return len - slen;
}
#endif