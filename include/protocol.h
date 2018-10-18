#ifndef CAMLER_PROTOCOL_H
#define CAMLER_PROTOCOL_H
#include "utils.h"

/*	Define structure of http packet include header and payload
*	Define functions analysis, parse and compose http message
*/

typedef struct http_request
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
	char* _request_uri;	/*	Define request uri */
	char* _http_version;	/*	Define http version */
	char* _host;	/*	Define server host */
	char* _user_agent;	/*	Define user-agent */
	char** _other_fields;	/*	Define other http request header fields */
}http_req;

typedef struct http_response
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
	int _is_valid;	/* equal 1 if request valid, equal 0 if request not valid */
	char* _http_version;	/* Example: HTTP/1.1 */
	int _status_code;	/* Example: 404 */
	char* _status_mean;	/* Example: Not Found */
	char* _server;	/* Example: Apache/2.2 */
	int _content_length;
	char* _content_type;	/* Example: text/plain, text/html, ... */
	char* _body;	/* Response content*/
	char** _other_fields;
}http_res;

/* Define function for Allocate memory for http_request structure */
http_req alloc_http_req();

/*	Define function for Allocate memory for http_response structure */
http_res alloc_http_res();

/* Define function for De-allocate memory for http_request structure */
void dealloc_http_req(http_req req);

/* Define function for De-allocate memory for http_response structure */
void dealloc_http_res(http_res res);

/* Define function for Output http_request information */
void print_http_req(http_req req);

/* Define function for Output http_response Information */
void print_http_res(http_res res);

/* Compose a http_request structure to string format*/
char* compose_http_req(http_req req);

/* Compose a http_response structure to string format */
char* compose_http_res(http_res res);

/* Parse message request form client to http_req structure */
http_req parse_http_req(const char* message);

/*
*	Parse a string parameter post method to json_obj*
*	Example:
*		user=name&pass=password123 -> {"user":"name","pass":"password"}
*/
json_objs parse_post_parameter_to_json(const char* parameter);

http_req alloc_http_req()
{
	http_req req; /* for return */
	req._method = (char*)malloc(SIZE_TINY);	/* For GET and POST only */
	req._request_uri = (char*)malloc(SIZE_SMALL);
	req._http_version = (char*)malloc(SIZE_SMALL);
	req._host = (char*)malloc(SIZE_SMALL);
	req._user_agent = (char*)malloc(SIZE_LARGE);
	req._other_fields = (char**)malloc(SIZE_SMALL*sizeof(char*)); /* max other fileds is 32 */
	for(int i=0;i<16;i++)
	{
		req._other_fields[i] = (char*)malloc(SIZE_LARGE);
		sprintf(req._other_fields[i], "%s", NOTSET);
	}
	return req;
}

http_res alloc_http_res()
{
	http_res res; /* for return */
	res._is_valid = 0;
	res._http_version = (char*)malloc(SIZE_SMALL);
	res._status_mean = (char*)malloc(SIZE_SMALL);
	res._server = (char*)malloc(SIZE_SMALL);	/* Web Server service name */
	res._content_type = (char*)malloc(SIZE_SMALL);
	res._body = (char*)malloc(4*SIZE_LARGE);	/* Max body size is 4096 bytes */
	res._other_fields = (char**)malloc(SIZE_SMALL*sizeof(char*)); /* max other fileds is 32 */
	for(int i=0;i<16;i++)
	{
		res._other_fields[i] = (char*)malloc(SIZE_LARGE);
		strcpy(res._other_fields[i], NOTSET);
	}
	return res;
}

void dealloc_http_req(http_req req)
{
	free(req._method);
	free(req._request_uri);
	free(req._http_version);
	free(req._host);
	free(req._user_agent);
	for(int i=0;i<SIZE_SMALL;i++)
		free(req._other_fields[i]);
	free(req._other_fields);
}

void dealloc_http_res(http_res res)
{
	free(res._http_version);
	free(res._status_mean);
	free(res._server);
	free(res._content_type);
	free(res._body);
	for(int i=0;i<SIZE_SMALL;i++)
		free(res._other_fields[i]);
	free(res._other_fields);
}

void print_http_req(http_req req)
{
	printf("%s %s %s\n", req._method, req._request_uri, req._http_version);
	printf("Host: %s\n", req._host);
	printf("User-Agent: %s\n", req._user_agent);
	int i = 0; /* for loop */
	while(!strstr(req._other_fields[i], NOTSET))
	{
		printf("%s\n", req._other_fields[i]);
		i++;
	}
}

void print_http_res(http_res res)
{
	printf("%s %d %s\n", res._http_version, res._status_code, res._status_mean);
	printf("%s\n", res._server);
	printf("%d\n", res._content_length);
	printf("%s\n", res._content_type);
	printf("%s\n", res._body);
	int i = 0;
	while(!strstr(res._other_fields[i], NOTSET))
	{
		printf("%s\n", res._other_fields[i]);
		i++;
	}
}

char* compose_http_req(http_req req)
{
	char* message;
	message = (char*)malloc(4*SIZE_LARGE);	/* Max message size is 4096 bytes */
	sprintf(
		message, 
		"%s %s %s\r\nHost: %s\r\nUser-Agent: %s\r\n", 
		req._method,
		req._request_uri,
		req._http_version,
		req._host,
		req._user_agent
	);
	int i = 0;	/* for loop */
	while(!strstr(req._other_fields[i], NOTSET))
	{
		strcat(message, req._other_fields[i]);
		strcat(message, "\r\n");
		i++;
	}
	return message;
}

char* compose_http_res(http_res res)
{
	char* message;
	message = (char*)malloc(4*SIZE_LARGE);	/* Max message size is 4096 bytes */
	sprintf(
		message, 
		"%s %d %s\r\nServer: %s\r\nConnection: keep-alive\r\nContentLength: %d\r\nContentType: %s\r\n", 
		res._http_version,
		res._status_code,
		res._status_mean,
		res._server,
		res._content_length,
		res._content_type
	);
	int i = 0;	/* for loop */
	while(!strstr(res._other_fields[i], NOTSET))
	{
		strcat(message, res._other_fields[i]);
		strcat(message, "\r\n");
		i++;
	}
	strcat(message, "\r\n");	/* Split header with payload by /r/n/r/n */
	strcat(message, res._body);
	strcat(message, "\n");
	return message;
}

http_req parse_http_req(const char* message)
{
	http_req req;
	req = alloc_http_req();
	char* buffer = (char*)malloc(SIZE_LARGE);	/* line string buffer */
	int mes_index = 0;	/* index of message */
	int req_other_fileds_index = 0;	/* index for http_req _otherFileds array */ 
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
			char** line = string_split(buffer, ' ', 2);
			strcpy(req._method, line[0]);
			strcpy(req._request_uri, line[1]);
			strcpy(req._http_version, line[2]);
			free(line);
		}
		else if (strstr(buffer, "Host:"))
		{
			char** line = string_split(buffer, ' ', 1);
			strcpy(req._host, line[1]);
			free(line);
		}
		else if (strstr(buffer, "User-Agent:"))
		{
			char** line = string_split(buffer, ' ', 1);
			strcpy(req._user_agent, line[1]);
			free(line);
		}
		else
		{
			strcpy(req._other_fields[req_other_fileds_index], buffer);
			req_other_fileds_index++;
		}
		while((message[mes_index] == '\r') 
			|| (message[mes_index] == '\n')
			|| (message[mes_index] == '\0'))
			mes_index++;
	}
	free(buffer);
	return req;
}

json_objs parse_post_parameter_to_json(const char* parameter)
{
	json_objs objs;
	int count = count_str_str(parameter, "&");
	if (strlen(parameter) != 0)
		objs._size = count+1;
	else
		objs._size = 0;
	objs._element = (json_obj*)malloc(objs._size);
	int i = 0;
	int objs_index = 0;
	char key[SIZE_MEDIUM];
	char value[SIZE_MEDIUM];
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
		objs._element[objs_index] = create_json(key, value);
		objs_index++;
		i++;
		if (i >= strlen(parameter))
			break;
		count--;
	}
	return objs;
}

#endif