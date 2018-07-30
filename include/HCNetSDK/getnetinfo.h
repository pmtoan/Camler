#ifndef GETNETINFO_H
#define GETNETINFO_H

#include "libHCNet.h"
#include "../type.h"
#include "../utils.h"
#include "../protocol.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"

char* HCNetGetNetInfo(const char* s_ip, int s_port, const char* s_user, const char* s_pass)
{
	char* ip = (char*)malloc(1024);
    sprintf(ip, "%s", s_ip);
    char* user = (char*)malloc(1024);
    sprintf(user, "%s", s_user);
    char* pass = (char*)malloc(1024);
    sprintf(pass, "%s", s_pass);
	char* r_return = (char*)malloc(1024);
	/*
	*	Login device
	*/
	int iRet = -1;
    DWORD uiReturnLen = 0;
    LONG lChannel = 0;
    NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	LONG lUserID = NET_DVR_Login_V30(ip, s_port, user, pass, &struDeviceInfo);
	JSONObjs objs;
    if (lUserID < 0)
    {
        int error = NET_DVR_GetLastError();
        if (error == 1)
            printf("[ERROR]{HCNET}    '%s'@'%s' Login error , username or password is not valid\n", ip, user);
        else if (error==7)
            printf("[ERROR]{HCNET}    '%s'@'%s' Login error , connect to device failed\n", ip, user);
        else if (error==152)
            printf("[ERROR]{HCNET}    '%s'@'%s' Login error , username not exists\n", ip, user);
        else
            printf("[ERROR]{HCNET}    '%s'@'%s' Login error , unknown\n", ip, user);
        objs._size = 3;
    	objs._element = (JSONObj*)malloc(3*sizeof(JSONObj));
        objs._element[0] = createJSONObj("action", "login");
        objs._element[1] = createJSONObj("status", "failed");
        objs._element[2] = createJSONObj("detail", "check your IP address of device or username and password");
        r_return = composeJSONObj(objs);
        return r_return;
    }
    /* Get device net config parameter */
    NET_DVR_NETCFG_V30 struParams = {0};
    struParams.dwSize = sizeof(NET_DVR_NETCFG_V30);
    iRet = NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_NETCFG_V30, lChannel, \
        &struParams, sizeof(NET_DVR_NETCFG_V30), &uiReturnLen);
    if (!iRet)
    {
        printf("[ERROR]{HCNET}    '%s'@'%s' Get user config parameter error %d\n", ip, user, NET_DVR_GetLastError());
        objs._size = 3;
    	objs._element = (JSONObj*)malloc(3*sizeof(JSONObj));
        objs._element[0] = createJSONObj("action", "getnetinfo");
        objs._element[1] = createJSONObj("status", "failed");
        objs._element[2] = createJSONObj("detail", "can't get device config parameter, check your device and try again");
        r_return = composeJSONObj(objs);
        return r_return;
    }
    sprintf(r_return, "{\"action\":\"getnetinfo\",\"status\":\"success\",\"detail\":[");
    char* buffer = (char*)malloc(1024);
    for (int i = 0; i < MAX_ETHERNET; i++)
    {
    	objs._size = 6;
    	objs._element = (JSONObj*)malloc(6*sizeof(JSONObj));
    	objs._element[0] = createJSONObj("ip", struParams.struEtherNet[i].struDVRIP.sIpV4);
    	objs._element[1] = createJSONObj("netmask", struParams.struEtherNet[i].struDVRIPMask.sIpV4);
    	char numport[5];
    	sprintf(numport, "%d", struParams.wHttpPortNo);
    	objs._element[2] = createJSONObj("httpport", numport);
    	objs._element[3] = createJSONObj("dns1", struParams.struDnsServer1IpAddr.sIpV4);
    	objs._element[4] = createJSONObj("dns2", struParams.struDnsServer2IpAddr.sIpV4);
    	objs._element[5] = createJSONObj("gateway", struParams.struGatewayIpAddr.sIpV4);
    	buffer = composeJSONObj(objs);
    	if (i==0)
    		sprintf(r_return, "%s%s", r_return, buffer);
    	else
    		sprintf(r_return, "%s,%s]}", r_return, buffer);
    }
	NET_DVR_Logout_V30(lUserID);
    printf("[INFO]{HCNET}    '%s'@'%s' Get net information\n", ip, user);
    free(ip);
    free(user);
    free(pass);
    return r_return;
}

#endif