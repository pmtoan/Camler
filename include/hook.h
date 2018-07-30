#ifndef HOOK_H
#define HOOK_H
/*
*	Define hook functions make a response text/html base on request
*	Hook function build a dynamic response base on request
*/
#include "utils.h"
#include "protocol.h"
#include "type.h"
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

HTTPRes hookStatic(const char* page)
{
	/*
	*	Return website base on html filepath
	*	page is file path to static html file
	*/
	HTTPRes res;
	res = allocHTTPRes();
	strcpy(res._httpVersion, "HTTP/1.1");
	res._isValid = 1;
	res._statusCode = 200;
	strcpy(res._statusMean, "OK");
	strcpy(res._server, "Cwes");
	strcpy(res._contentType, "text/html");
	char** data = readFromFile(page);
	char* body = (char*)malloc(8*KBYTE_T);
	int i = 0;
	while(data[i] != NULL)
	{
		if (i==0)
			strcpy(body, data[i]);
		else
			strcat(body, data[i]);
		i++;
	}
	strcpy(res._body, body);
	res._contentLength = strlen(body);
	return res;
}

HTTPRes hookDynamic(const char* payload)
{
	/*
	*	Return response with a payload as parameter
	*	This function for dynamic body respnose
	*/
	HTTPRes res;
	res = allocHTTPRes();
	strcpy(res._httpVersion, "HTTP/1.1");
	res._isValid = 1;
	res._statusCode = 200;
	strcpy(res._statusMean, "OK");
	strcpy(res._server, "Cwes");
	strcpy(res._contentType, "application/json");
	strcpy(res._body, payload);
	res._contentLength = strlen(payload);
	return res;
}

HTTPRes hookRoot()
{
	/*
	*	Return root page, public/welcome.html
	*/
	return hookStatic("public/welcome.html");
}

HTTPRes hookLogin(const char* parameter)
{
	/*
	*	Handle login request
	*	Source:			public/welcome.html
	*	Destination:	public/index.html or public/deny.html
	*/
	JSONObjs objs = parsePostParameterToJSONObj(parameter);
	JSONObj objKey = getJSONObj(objs, "key");
	JSONObj objUser = getJSONObj(objs, "user");
	JSONObj objPass = getJSONObj(objs, "pass");
	if ((countStrStr(objUser._value, ADMIN_USER))
		&& (countStrStr(objPass._value, ADMIN_PASS)))
		return hookStatic("public/index.html");
	else
		return hookStatic("public/deny.html");
}

HTTPRes hookScanning(const char* parameter)
{
	/*
	*	Scanning network, detect ip and info of HCNet camera
	*/
	JSONObjs objs = parsePostParameterToJSONObj(parameter);
	JSONObj objIp = getJSONObj(objs, "iprange");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0)
		return hookDynamic(UNAUTHORIZED);
	return hookDynamic(HCNetScan(objIp._value, 2, 254));
}

HTTPRes hookHCNetCapture(const char* parameter)
{
	/*
	*	Take a picture with HIK VISION Cammera
	*/
	JSONObjs objs = parsePostParameterToJSONObj(parameter);
	JSONObj objIp = getJSONObj(objs, "ip");	/* Get ip from request */
	JSONObj objUser = getJSONObj(objs, "user");	/* Get username form request */
	JSONObj objPass = getJSONObj(objs, "pass");	/* Get password from request */
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0)
		/* 
		*	If failed to get ip or user or pass, it mean a bad request
		*  	, and return a UNAUTHORIZED response, UNAUTHORIZED defined in type.h
		*/
		return hookDynamic(UNAUTHORIZED);
	/* If parameters is valid, call SDK and return response */
	return hookDynamic(HCNetCapture(objIp._value, DEFAULT_HCNET_PORT, objUser._value, objPass._value));
}

HTTPRes hookHCNetGetUserInfo(const char* parameter)
{
	/*
	*	Get user account infomation of device configuration
	*/
	JSONObjs objs = parsePostParameterToJSONObj(parameter);
	JSONObj objIp = getJSONObj(objs, "ip");
	JSONObj objUser = getJSONObj(objs, "user");
	JSONObj objPass = getJSONObj(objs, "pass");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0)
		return hookDynamic(UNAUTHORIZED);
	return hookDynamic(HCNetGetUserInfo(objIp._value, DEFAULT_HCNET_PORT, objUser._value, objPass._value));
}

HTTPRes hookHCNetChangePass(const char* parameter)
{
	/*
	*	Get user account infomation of device configuration
	*/
	JSONObjs objs = parsePostParameterToJSONObj(parameter);
	JSONObj objIp = getJSONObj(objs, "ip");
	JSONObj objUser = getJSONObj(objs, "user");
	JSONObj objPass = getJSONObj(objs, "pass");
	JSONObj objPUser = getJSONObj(objs, "puser");
	JSONObj objPPass = getJSONObj(objs, "ppass");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0
		|| strlen(objPUser._value)==0 || strlen(objPUser._key) == 0
		|| strlen(objPPass._value)==0 || strlen(objPPass._key) == 0)
		return hookDynamic(UNAUTHORIZED);
	return hookDynamic(HCNetChangePass(objIp._value, 
		DEFAULT_HCNET_PORT, objUser._value, objPass._value,
		objPUser._value, objPPass._value));
}

HTTPRes hookHCNetGetNetInfo(const char* parameter)
{
	/*
	*	Get user account infomation of device configuration
	*/
	JSONObjs objs = parsePostParameterToJSONObj(parameter);
	JSONObj objIp = getJSONObj(objs, "ip");
	JSONObj objUser = getJSONObj(objs, "user");
	JSONObj objPass = getJSONObj(objs, "pass");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0)
		return hookDynamic(UNAUTHORIZED);
	return hookDynamic(HCNetGetNetInfo(objIp._value, DEFAULT_HCNET_PORT, objUser._value, objPass._value));
}

HTTPRes hookHCNetChangeIp(const char* parameter)
{
	/*
	*	Set new ip address of device
	*/
	JSONObjs objs = parsePostParameterToJSONObj(parameter);
	JSONObj objIp = getJSONObj(objs, "ip");
	JSONObj objUser = getJSONObj(objs, "user");
	JSONObj objPass = getJSONObj(objs, "pass");
	JSONObj objNewIp = getJSONObj(objs, "newip");
	JSONObj objNewMask = getJSONObj(objs, "newmask");
	JSONObj objEtherPort = getJSONObj(objs, "eport");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0
		|| strlen(objNewIp._value)==0 || strlen(objNewIp._key) == 0
		|| strlen(objNewMask._value)==0 || strlen(objNewMask._key) == 0
		|| strlen(objEtherPort._value)==0 || strlen(objEtherPort._key) == 0)
		return hookDynamic(UNAUTHORIZED);
	return hookDynamic(HCNetChangeIp(objIp._value, DEFAULT_HCNET_PORT, 
		objUser._value, objPass._value, objNewIp._value, objNewMask._value, atoi(objEtherPort._value)));
}

HTTPRes hookHCNetChangeDNS(const char* parameter)
{
	/*
	*	Set new DNS server
	*/
	JSONObjs objs = parsePostParameterToJSONObj(parameter);
	JSONObj objIp = getJSONObj(objs, "ip");
	JSONObj objUser = getJSONObj(objs, "user");
	JSONObj objPass = getJSONObj(objs, "pass");
	JSONObj objDNS1 = getJSONObj(objs, "dns1");
	JSONObj objDNS2 = getJSONObj(objs, "dns2");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0
		|| strlen(objDNS1._value)==0 || strlen(objDNS1._key) == 0
		|| strlen(objDNS2._value)==0 || strlen(objDNS2._key) == 0)
		return hookDynamic(UNAUTHORIZED);
	return hookDynamic(HCNetChangeDNS(objIp._value, DEFAULT_HCNET_PORT, objUser._value, objPass._value, objDNS1._value, objDNS2._value));
}

HTTPRes hookHCNetChangeGW(const char* parameter)
{
	/*
	*	Set new gateway address
	*/
	JSONObjs objs = parsePostParameterToJSONObj(parameter);
	JSONObj objIp = getJSONObj(objs, "ip");
	JSONObj objUser = getJSONObj(objs, "user");
	JSONObj objPass = getJSONObj(objs, "pass");
	JSONObj objGW = getJSONObj(objs, "gw");
	if (strlen(objIp._value)==0 || strlen(objIp._key) == 0
		|| strlen(objUser._value)==0 || strlen(objUser._key) == 0
		|| strlen(objPass._value)==0 || strlen(objPass._key) == 0
		|| strlen(objGW._value)==0 || strlen(objGW._key) == 0)
		return hookDynamic(UNAUTHORIZED);
	return hookDynamic(HCNetChangeGW(objIp._value, DEFAULT_HCNET_PORT, objUser._value, objPass._value, objGW._value));
}

#endif