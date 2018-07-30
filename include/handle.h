#ifndef HANDLE_H
#define HANDLE_H
#include "stdsoc.h"
#include "protocol.h"
#include "utils.h"
#include "type.h"
#include "hook.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

HTTPRes handleRequest(HTTPReq req)
{
	/*
	*	Main server handler
	*	Check request from client and return response 
	*/
	HTTPRes res;
	if((req._requestURI[0] == '/')
		/*
		*	Handle request root page
		*/
		&& (strlen(req._requestURI) == 1)
		&& (strstr(req._method, "GET")))
		res = hookRoot();
	else if (strstr(req._method, POST))
	{
		/*
		*	Get POST parameter
		*	parameter is last char* from req._otherFields
		*/
		int i = 0;
		char* tokenKey = (char*)malloc(1024);
		while(!strstr(req._otherFields[i], NOTSET))
		{
			if (strstr(req._otherFields[i], "CwesTokenAPI:"))
				strcpy(tokenKey, stringSplit(req._otherFields[i], ' ', 1)[1]);
			i++;
		}
		if (compareStrStr(tokenKey, API_TOKEN) == 0)
			res = hookDynamic(UNAUTHORIZED);
		else if (!strstr(req._otherFields[i-1], "="))
			res = hookDynamic(UNAUTHORIZED);
		else if (compareStrStr(req._requestURI, "/login") == 1)
			res = hookLogin(req._otherFields[i-1]);
		else if (compareStrStr(req._requestURI, "/hcnet/scanning") == 1)
			res = hookScanning(req._otherFields[i-1]);
		else if (compareStrStr(req._requestURI, "/hcnet/capture") == 1)
			res = hookHCNetCapture(req._otherFields[i-1]);
		else if (compareStrStr(req._requestURI, "/hcnet/get/userinfo") == 1)
			res = hookHCNetGetUserInfo(req._otherFields[i-1]);
		else if (compareStrStr(req._requestURI, "/hcnet/change/pass") == 1)
			res = hookHCNetChangePass(req._otherFields[i-1]);
		else if (compareStrStr(req._requestURI, "/hcnet/get/netinfo") == 1)
			res = hookHCNetGetNetInfo(req._otherFields[i-1]);
		else if (compareStrStr(req._requestURI, "/hcnet/change/ip") == 1)
			res = hookHCNetChangeIp(req._otherFields[i-1]);
		else if (compareStrStr(req._requestURI, "/hcnet/change/dns") == 1)
			res = hookHCNetChangeDNS(req._otherFields[i-1]);
		else if (compareStrStr(req._requestURI, "/hcnet/change/gw") == 1)
			res = hookHCNetChangeGW(req._otherFields[i-1]);
	}
	else
		res = hookDynamic(NOTFOUND);
	return res;
}

#endif