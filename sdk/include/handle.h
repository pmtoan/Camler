#ifndef CAMLER_HANDLE_H
#define CAMLER_HANDLE_H
#include "stdsoc.h"
#include "protocol.h"
#include "utils.h"
#include "hook.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

http_res handle_request(http_req req)
{
	/* 	Main server handler
	*	Check request from client and return response 
	*/
	if (strstr(req._method, "POST")
		|| strstr(req._method, "post")
		||strstr(req._method, "Post"))
	{
		/*  Get POST parameter
		*	parameter is last char* from req._other_fields
		*/
		int i = 0;
		char* tokenKey = (char*)malloc(1024);
		while(!strstr(req._other_fields[i], NOTSET))
		{
			if (strstr(req._other_fields[i], "TokenAPI:"))
				strcpy(tokenKey, string_split(req._other_fields[i], ' ', 1)[1]);
			i++;
		}
		if (compare_str(tokenKey, API_TOKEN) == 0)
			return hook_dynamic(UNAUTHORIZED);
		else if (!strstr(req._other_fields[i-1], "="))
			return hook_dynamic(UNAUTHORIZED);
		else if (compare_str(req._request_uri, "/hcnet/scanning") == 1)
			return hook_scanning(req._other_fields[i-1]);
		else if (compare_str(req._request_uri, "/hcnet/capture") == 1)
			return hook_hcnet_capture(req._other_fields[i-1]);
		else if (compare_str(req._request_uri, "/hcnet/get/userinfo") == 1)
			return hook_hcnet_get_user_info(req._other_fields[i-1]);
		else if (compare_str(req._request_uri, "/hcnet/change/pass") == 1)
			return hook_hcnet_change_pass(req._other_fields[i-1]);
		else if (compare_str(req._request_uri, "/hcnet/get/netinfo") == 1)
			return hook_hcnet_get_net_info(req._other_fields[i-1]);
		else if (compare_str(req._request_uri, "/hcnet/change/ip") == 1)
			return hook_hcnet_change_ip(req._other_fields[i-1]);
		else if (compare_str(req._request_uri, "/hcnet/change/dns") == 1)
			return hook_hcnet_change_dns(req._other_fields[i-1]);
		else if (compare_str(req._request_uri, "/hcnet/change/gw") == 1)
			return hook_hcnet_change_gateway(req._other_fields[i-1]);
		else
			return hook_dynamic(UNAUTHORIZED);
	}
	else
		return hook_dynamic(UNAUTHORIZED);
}

#endif