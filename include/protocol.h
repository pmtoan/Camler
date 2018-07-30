#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "utils.h"

#define GET "GET"
#define POST "POST"

/*
*	Define structure of http packet include header and payload
*	Define functions analysis, parse and compose http message
*/

typedef struct HTTPRequest
{
	/*
	*	Request-Line = Method SP Request-URI SP HTTP-Version CRLF
	*	- Example: GET / HTTP/1.1
	*	- Method - Description
	*		1. GET -	The GET method is used to retrieve information from the given server using a given URI. 
	*					Requests using GET should only retrieve data and should have no other effect on the data. 
	*		2. POST -	A POST request is used to send data to the server, for example, customer information, 
	*					file upload, etc. using HTML forms.
	*	- RequestURI
	*		The most common form of Request-URI is that used to identify a resource on an origin server or gateway.
	*	- HTTP-Version
	*		Example: HTTP/1.1
	*	Request Header Fields
	*	- Accept-Charset, Accept-Encoding, Accept-Language,User-Agent,  Host
	*	- If-Modified-Since, If-None-Match, If-Range, If-Unmodified-Since, Max-Forwards
	*	- Proxy-Authorization, Range, Referer, TE, User-Agent, Authorization, Expect, From, If-Match
	*	Example HTTP Request
	*		GET /hello.htm HTTP/1.1
	*		User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
	*		Host: www.tutorialspoint.com
	*		Accept-Language: en-us
	*		Accept-Encoding: gzip, deflate
	*		Connection: Keep-Alive
	*/

	char* _method;	/*	Define http request method */
	char* _requestURI;	/*	Define request uri */
	char* _httpVersion;	/*	Define http version */
	char* _host;	/*	Define server host */
	char* _userAgent;	/*	Define user-agent */
	char** _otherFields;	/*	Define other http request header fields */
}HTTPReq;

typedef struct HTTPResponse
{
	/*
	*	Message Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	*	- Example: HTTP/1.1 200 OK
	*	- HTTP Version
	*		HTTP Version supported by server, for example HTTP/1.1
	*	- Status Code
	*		1xx: Informational
	*			It means the request was received and the process is continuing.
	*		2xx: Success
	*			It means the action was successfully received, understood, and accepted.
	*		3xx: Redirection
	*			It means further action must be taken in order to complete the request.
	*		4xx: Client Error
	*			It means the request contains incorrect syntax or cannot be fulfilled.
	*		5xx: Server Error
	*			It means the server failed to fulfill an apparently valid request.
	*	Response Header Fields
	*	- Accept-Ranges, Age, ETag, Location, Proxy-Authenticate, Retry-After, Server, Vary, WWW-Authenticate
	*	Example HTTP Response
	*		HTTP/1.1 200 OK
	*		Date: Mon, 27 Jul 2009 12:28:53 GMT
	*		Server: Apache/2.2.14 (Win32)
	*		Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
	*		Content-Length: 88
	*		Content-Type: text/html
	*		Connection: Closed
	*
	*		<html>
	*		<body>
	*		<h1>Hello, World!</h1>
	*		</body>
	*		</html>
	*/
	int _isValid;	/* equal 1 if request valid, equal 0 if request not valid */
	char* _httpVersion;	/* Example: HTTP/1.1 */
	int _statusCode;	/* Example: 404 */
	char* _statusMean;	/* Example: Not Found */
	char* _server;	/* Example: Apache/2.2 */
	int _contentLength;
	char* _contentType;	/* Example: text/plain, text/html, ... */
	char* _body;	/* Response content*/
	char** _otherFields;
}HTTPRes;

HTTPReq allocHTTPReq()
{
	/* 
	*	Define function for Allocate memory for HTTPRequest structure 
	*/
	HTTPReq req; /* for return */
	req._method = (char*)malloc(4);	/* For GET and POST only */
	req._requestURI = (char*)malloc(1024);
	req._httpVersion = (char*)malloc(10);
	req._host = (char*)malloc(32);
	req._userAgent = (char*)malloc(1024);
	int i; /* for loop allocate memory */
	req._otherFields = (char**)malloc(32*sizeof(char*)); /* max other fileds is 32 */
	for(i=0;i<16;i++)
	{
		req._otherFields[i] = (char*)malloc(512);
		strcpy(req._otherFields[i], NOTSET);
	}
	return req;
}

HTTPRes allocHTTPRes()
{
	/* 
	*	Define function for Allocate memory for HTTPResponse structure 
	*/
	HTTPRes res; /* for return */
	res._isValid = 0;
	res._httpVersion = (char*)malloc(10);
	res._statusMean = (char*)malloc(32);
	res._server = (char*)malloc(32);	/* Web Server service name */
	res._contentType = (char*)malloc(32);
	res._body = (char*)malloc(4096);	/* Max body size is 4096 bytes */
	int i; /* for loop allocate memory */
	res._otherFields = (char**)malloc(32*sizeof(char*)); /* max other fileds is 32 */
	for(i=0;i<16;i++)
	{
		res._otherFields[i] = (char*)malloc(512);
		strcpy(res._otherFields[i], NOTSET);
	}
	return res;
}

void deallocHTTPReq(HTTPReq req)
{
	/* 
	*	Define function for De-allocate memory for HTTPRequest structure 
	*/
	free(req._method);
	free(req._requestURI);
	free(req._httpVersion);
	free(req._host);
	free(req._userAgent);
	int i; /* for loop de-allocate memory */
	for(i=0;i<16;i++)
		free(req._otherFields[i]);
	free(req._otherFields);
}

void deallocHTTPRes(HTTPRes res)
{
	/* 
	*	Define function for De-allocate memory for HTTPResponse structure 
	*/
	free(res._httpVersion);
	free(res._statusMean);
	free(res._server);
	free(res._contentType);
	free(res._body);
	int i; /* for loop de-allocate memory */
	for(i=0;i<16;i++)
		free(res._otherFields[i]);
	free(res._otherFields);
}

void printHTTPReq(HTTPReq req)
{
	/*
	*	Define function for Output HTTPRequest Information 
	*/
	printf("%s %s %s\n", req._method, req._requestURI, req._httpVersion);
	printf("Host: %s\n", req._host);
	printf("User-Agent: %s\n", req._userAgent);
	int i; /* for loop */
	i = 0;
	while(!strstr(req._otherFields[i], NOTSET))
	{
		printf("%s\n", req._otherFields[i]);
		i++;
	}
}

void printHTTPRes(HTTPRes res)
{
	/* 
	*	Define function for Output HTTPResponse Information 
	*/
	printf("%s %d %s\n", res._httpVersion, res._statusCode, res._statusMean);
	printf("%s\n", res._server);
	printf("%d\n", res._contentLength);
	printf("%s\n", res._contentType);
	printf("%s\n", res._body);
	int i;
	i = 0;
	while(!strstr(res._otherFields[i], NOTSET))
	{
		printf("%s\n", res._otherFields[i]);
		i++;
	}
}

char* composeHTTPReq(HTTPReq req)
{
	/*
	*	Compose a HTTPRequest structure to string format
	*/
	char* message;
	message = (char*)malloc(8192);	/* Max message size is 8192 bytes */
	sprintf(
		message, 
		"%s %s %s\r\nHost: %s\r\nUser-Agent: %s\r\n", 
		req._method,
		req._requestURI,
		req._httpVersion,
		req._host,
		req._userAgent
	);
	int i;	/* for loop */
	i = 0;
	while(!strstr(req._otherFields[i], NOTSET))
	{
		strcat(message, req._otherFields[i]);
		strcat(message, "\r\n");
		i++;
	}
	return message;
}

char* composeHTTPRes(HTTPRes res)
{
	/*
	*	Compose a HTTPResponse structure to string format
	*/
	char* message;
	message = (char*)malloc(8192);	/* Max message size is 8192 bytes */
	sprintf(
		message, 
		"%s %d %s\r\nServer: %s\r\nConnection: keep-alive\r\nContentLength: %d\r\nContentType: %s\r\n", 
		res._httpVersion,
		res._statusCode,
		res._statusMean,
		res._server,
		res._contentLength,
		res._contentType
	);
	int i;	/* for loop */
	i = 0;
	while(!strstr(res._otherFields[i], NOTSET))
	{
		strcat(message, res._otherFields[i]);
		strcat(message, "\r\n");
		i++;
	}
	strcat(message, "\r\n");	/* Split header with payload by /r/n/r/n */
	strcat(message, res._body);
	strcat(message, "\n");
	return message;
}

HTTPReq parseHTTPReq(char* message)
{
	/*
	*	Parse message request form client to HTTPReq structure
	*/
	HTTPReq req;
	req = allocHTTPReq();
	char* buffer = (char*)malloc(1024);	/* line string buffer */
	int mes_index = 0;	/* index of message */
	int req_otherFileds_index = 0;	/* index for HTTPReq _otherFileds array */ 
	while(mes_index < strlen(message))
	{
		int buf_index = 0;
		while((message[mes_index] != '\r') && (mes_index < strlen(message)))
		{
			buffer[buf_index] = message[mes_index];
			buf_index++;
			mes_index++;
		}
		buffer[buf_index] = '\0';
		if (strstr(buffer, "HTTP"))
		{
			char** line = stringSplit(buffer, ' ', 2);
			strcpy(req._method, line[0]);
			strcpy(req._requestURI, line[1]);
			strcpy(req._httpVersion, line[2]);
			free(line);
		}
		else if (strstr(buffer, "Host:"))
		{
			char** line = stringSplit(buffer, ' ', 1);
			strcpy(req._host, line[1]);
			free(line);
		}
		else if (strstr(buffer, "User-Agent:"))
		{
			char** line = stringSplit(buffer, ' ', 1);
			strcpy(req._userAgent, line[1]);
			free(line);
		}
		else
		{
			strcpy(req._otherFields[req_otherFileds_index], buffer);
			req_otherFileds_index++;
		}
		while((message[mes_index] == '\r') 
			|| (message[mes_index] == '\n')
			|| (message[mes_index] == '\0')
			/*|| (mes_index < strlen(message))*/)
			mes_index++;
	}
	free(buffer);
	return req;
}

HTTPRes parseHTTPRes(const char* message)
{
	HTTPRes res;
	res = allocHTTPRes();

	return res;
}

JSONObjs parsePostParameterToJSONObj(const char* parameter)
{
	/*
	*	Parse a string parameter post method to JSONObj*
	*	Example:
	*		user=name&pass=password123 -> {"user":"name","pass":"password"}
	*/
	JSONObjs objs;
	int count = countStrStr(parameter, "&");
	if (strlen(parameter) != 0)
		objs._size = count+1;
	else
		objs._size = 0;
	objs._element = (JSONObj*)malloc(objs._size);
	int i = 0;
	int objs_index = 0;
	char key[32];
	char value[1024];
	int j;
	while(count >= 0)
	{
		j = 0;
		while((parameter[i] != '=') 
			&& (parameter[i] != '&')
			&& (parameter[i] != '\0'))
		{
			key[j] = parameter[i];
			i++;
			j++;
		}
		key[j] = '\0';
		j = 0;
		i++;
		while((parameter[i] != '=')
			&& (parameter[i] != '&')
			&& (parameter[i] != '\0'))
		{
			value[j] = parameter[i];
			i++;
			j++;
		}
		value[j] = '\0';
		objs._element[objs_index] = createJSONObj(key, value);
		objs_index++;
		i++;
		if (i >= strlen(parameter))
			break;
		count--;
	}
	return objs;
}

#endif