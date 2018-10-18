#ifndef CAMLER_HOOK_H
#define CAMLER_HOOK_H
/*  Define hook functions make a response text/html base on request
*	Hook function build a dynamic response base on request
*/
#include "utils.h"
#include "protocol.h"
#include "unistd.h"
#include "time.h"
#include "HCNetSDK/capture.h"
#include "HCNetSDK/scanning.h"
#include "HCNetSDK/getuserinfo.h"
#include "HCNetSDK/changepass.h"
#include "HCNetSDK/getnetinfo.h"
#include "HCNetSDK/changeip.h"
#include "HCNetSDK/changedns.h"
#include "HCNetSDK/changegw.h"

/*  Return response with a payload as parameter
	This function for dynamic body respnose
*/
http_res hook_dynamic(const char* payload);

/* Scanning network, detect ip and info of HCNet camera */
http_res hook_scanning(const char* parameter);

/* Take a picture with HIK VISION Cammera */
http_res hook_hcnet_capture(const char* parameter);

/* Get user account infomation of device configuration */
http_res hook_hcnet_get_user_info(const char* parameter);

/* Get user account infomation of device configuration */
http_res hook_hcnet_change_pass(const char* parameter);

/* Get user account infomation of device configuration */
http_res hook_hcnet_get_net_info(const char* parameter);

/* Set new ip address of device */
http_res hook_hcnet_change_ip(const char* parameter);

/* Set new DNS server */
http_res hook_hcnet_change_dns(const char* parameter);

/* Set new gateway address */
http_res hook_hcnet_change_gateway(const char* parameter);

http_res hook_dynamic(const char* payload)
{
	http_res res;
	res = alloc_http_res();
	strcpy(res._http_version, "HTTP/1.1");
	res._is_valid = 1;
	res._status_code = 200;
	strcpy(res._status_mean, "OK");
	strcpy(res._server, "Cwes");
	strcpy(res._content_type, "application/json");
	strcpy(res._body, payload);
	res._content_length = strlen(payload);
	return res;
}

http_res hook_scanning(const char* parameter)
{
	json_objs objs = parse_post_parameter_to_json(parameter);
	json_obj objIp = get_json(objs, "iprange");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0)
		return hook_dynamic(UNAUTHORIZED);
	return hook_dynamic(hcnet_scanning(objIp._value, 2, 254));
}

http_res hook_hcnet_capture(const char* parameter)
{
	json_objs objs = parse_post_parameter_to_json(parameter);
	json_obj objIp = get_json(objs, "ip");	/* Get ip from request */
	json_obj objUser = get_json(objs, "user");	/* Get username form request */
	json_obj objPass = get_json(objs, "pass");	/* Get password from request */
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0)
		/* 
		*	If failed to get ip or user or pass, it mean a bad request
		*  	, and return a UNAUTHORIZED response, UNAUTHORIZED defined in type.h
		*/
		return hook_dynamic(UNAUTHORIZED);
	/* If parameters is valid, call SDK and return response */
	return hook_dynamic(hcnet_capture(objIp._value, DEFAULT_HCNET_PORT, objUser._value, objPass._value));
}

http_res hook_hcnet_get_user_info(const char* parameter)
{
	json_objs objs = parse_post_parameter_to_json(parameter);
	json_obj objIp = get_json(objs, "ip");
	json_obj objUser = get_json(objs, "user");
	json_obj objPass = get_json(objs, "pass");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0)
		return hook_dynamic(UNAUTHORIZED);
	return hook_dynamic(hcnet_get_user_info(objIp._value, DEFAULT_HCNET_PORT, objUser._value, objPass._value));
}

http_res hook_hcnet_change_pass(const char* parameter)
{
	json_objs objs = parse_post_parameter_to_json(parameter);
	json_obj objIp = get_json(objs, "ip");
	json_obj objUser = get_json(objs, "user");
	json_obj objPass = get_json(objs, "pass");
	json_obj objPUser = get_json(objs, "puser");
	json_obj objPPass = get_json(objs, "ppass");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0
		|| strlen(objPUser._value)==0 || strlen(objPUser._key) == 0
		|| strlen(objPPass._value)==0 || strlen(objPPass._key) == 0)
		return hook_dynamic(UNAUTHORIZED);
	return hook_dynamic(hcnet_change_pass(objIp._value, 
		DEFAULT_HCNET_PORT, objUser._value, objPass._value,
		objPUser._value, objPPass._value));
}

http_res hook_hcnet_get_net_info(const char* parameter)
{
	json_objs objs = parse_post_parameter_to_json(parameter);
	json_obj objIp = get_json(objs, "ip");
	json_obj objUser = get_json(objs, "user");
	json_obj objPass = get_json(objs, "pass");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0)
		return hook_dynamic(UNAUTHORIZED);
	return hook_dynamic(hcnet_get_net_info(objIp._value, DEFAULT_HCNET_PORT, objUser._value, objPass._value));
}

http_res hook_hcnet_change_ip(const char* parameter)
{
	json_objs objs = parse_post_parameter_to_json(parameter);
	json_obj objIp = get_json(objs, "ip");
	json_obj objUser = get_json(objs, "user");
	json_obj objPass = get_json(objs, "pass");
	json_obj objNewIp = get_json(objs, "newip");
	json_obj objNewMask = get_json(objs, "newmask");
	json_obj objEtherPort = get_json(objs, "eport");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0
		|| strlen(objNewIp._value)==0 || strlen(objNewIp._key) == 0
		|| strlen(objNewMask._value)==0 || strlen(objNewMask._key) == 0
		|| strlen(objEtherPort._value)==0 || strlen(objEtherPort._key) == 0)
		return hook_dynamic(UNAUTHORIZED);
	return hook_dynamic(hcnet_change_ip(objIp._value, DEFAULT_HCNET_PORT, 
		objUser._value, objPass._value, objNewIp._value, objNewMask._value, atoi(objEtherPort._value)));
}

http_res hook_hcnet_change_dns(const char* parameter)
{
	json_objs objs = parse_post_parameter_to_json(parameter);
	json_obj objIp = get_json(objs, "ip");
	json_obj objUser = get_json(objs, "user");
	json_obj objPass = get_json(objs, "pass");
	json_obj objDNS1 = get_json(objs, "dns1");
	json_obj objDNS2 = get_json(objs, "dns2");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0
		|| strlen(objDNS1._value)==0 || strlen(objDNS1._key) == 0
		|| strlen(objDNS2._value)==0 || strlen(objDNS2._key) == 0)
		return hook_dynamic(UNAUTHORIZED);
	return hook_dynamic(hcnet_change_dns(objIp._value, DEFAULT_HCNET_PORT, objUser._value, objPass._value, objDNS1._value, objDNS2._value));
}

http_res hook_hcnet_change_gateway(const char* parameter)
{
	json_objs objs = parse_post_parameter_to_json(parameter);
	json_obj objIp = get_json(objs, "ip");
	json_obj objUser = get_json(objs, "user");
	json_obj objPass = get_json(objs, "pass");
	json_obj objGW = get_json(objs, "gw");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0
		|| strlen(objGW._value)==0 || strlen(objGW._key) == 0)
		return hook_dynamic(UNAUTHORIZED);
	return hook_dynamic(hcnet_change_gateway(objIp._value, DEFAULT_HCNET_PORT, objUser._value, objPass._value, objGW._value));
}

#endif