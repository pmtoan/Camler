/*
*	Main source file
*/
#include "include/server.h"

void signal_handler(int sig);
int server; /* Main server docket, define as global variable */
int main(int argc, char* argv[])
{
	signal(SIGINT,signal_handler); /* Define a signal handler to press Ctrl-C for exit */
	server = initSocket();	/* Create and initialize main server socker */
	/* 
	*	If pass a argument commandline, set it to port
	*	Otherwise, port is default 30497
	*/
	int port = DEFAULT_PORT;
	if (argc > 1)
		port = atoi(argv[1]);
	if (bindToPort(server, port) == -1)
	{
		printf("[ERROR]{Cwes}   Can't bind to port %d\n", port);
		exit(-1);
	}
	listenClient(server, 1000);
	printf("[INFO]{Cwes}   HIK Vision camera controller running on 0::%d\n", port);
	struct sockaddr_storage client_addr;
	unsigned int address_size = sizeof(client_addr);
	int connect; /* Variable store socker from client */
	char* message = (char*)malloc(4*KBYTE_T);
	initServer();	/* Initialize sdk, libraries */
	pid_t pid;
	while(1)
	{
		/* Accept connection from client */
		connect = accept(server,(struct sockaddr *)&client_addr,&address_size);
		/* Get client information, ip address and port */
		struct sockaddr_in *coming = (struct sockaddr_in *)&client_addr;
		unsigned char *ip = (unsigned char *)&coming->sin_addr.s_addr;
		unsigned short port = coming->sin_port;
		if (connect < 0)
		{
			/* Can't accept connect from client */
			printf("[ERROR]{Cwes}   Can't connect with %d.%d.%d.%d, step socket accept\n", 
				ip[0], ip[1], ip[2], ip[3]);
			exit(-1);
		}
		pid = fork();	/* Create a child process */
		if (pid == 0)
		{
			/*
			*	This is child process
			*	Child process keep connection with client
			*/
			signal(SIGINT, SIG_DFL);	/* Clear signal handler in child process, when parent exit, child will be exit too */
			hear(connect, message, 4*KBYTE_T);	/* Recive message from client */
			HTTPReq req = parseHTTPReq(message);	/* Parse message to http request */
			HTTPRes res = handleRequest(req);	/* Handle request */
			printf("[INFO]{Cwes}   %s %s %s @ %d.%d.%d.%d:%d\n", 
				req._method, req._requestURI, req._httpVersion,
				ip[0], ip[1], ip[2], ip[3], port);
			printf("[INFO]{Cwes}   %s %d %s @ %d.%d.%d.%d:%d\n", 
				res._httpVersion, res._statusCode, res._statusMean,
				ip[0], ip[1], ip[2], ip[3], port);
			if (res._isValid == 1)
				/* If request is valid, send message response to client */
				say(connect, composeHTTPRes(res));
			exit(0);
		}
		/*
		*	Because child process stay connection with client, 
		*	Program should close connection with client in parent process 
		*/
		close(connect);
	}
	free(message);
	cleanServer();	/* Free memory for sdk and libraries */
	return 0;
}

void signal_handler(int sig)
{
	/* Server exiting */
	printf("\n[INFO]{Cwes}   HIK Vision camera controller exit\n");
	close(server);
	exit(1);
}