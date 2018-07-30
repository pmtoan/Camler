#ifndef CHANGEDNS_H
#define CHANGEDNS_H
#include "libHCNet.h"
#include "../type.h"
#include "../utils.h"
#include "../protocol.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"

char* HCNetChangeDNS(const char* s_ip, int s_port, const char* s_user, const char* s_pass, 
	const char* new_dns1, const char* new_dns2)
{
	/*
	*	Change DNS address of device
	*	new_dns1:	DNS address 1
	*	new_dns2:	DNS address 2
	*/
	char* ip = (char*)malloc(1024);
    sprintf(ip, "%s", s_ip);
    char* user = (char*)malloc(1024);
    sprintf(user, "%s", s_user);
    char* pass = (char*)malloc(1024);
    sprintf(pass, "%s", s_pass);
    char* newdns1 = (char*)malloc(1024);
    sprintf(newdns1, "%s", new_dns1);
    char* newdns2 = (char*)malloc(1024);
    sprintf(newdns2, "%s", new_dns2);
	char* r_return = (char*)malloc(1024);
	/* Login device */
	int iRet = -1;
    DWORD uiReturnLen = 0;
    LONG lChannel = 0;
    NET_DVR_SetConnectTime(300, 1);
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
    /* Set new DNS address */
    memcpy(struParams.struDnsServer1IpAddr.sIpV4, newdns1, 16);
    memcpy(struParams.struDnsServer2IpAddr.sIpV4, newdns2, 16);
    iRet = NET_DVR_SetDVRConfig(lUserID, NET_DVR_SET_NETCFG_V30, lChannel, \
        &struParams, sizeof(NET_DVR_NETCFG_V30));
    if (!iRet)
    {
        printf("[ERROR]{HCNET}    '%s'@'%s' Can't set new dns address %d\n", ip, user, NET_DVR_GetLastError());
        objs._size = 3;
        objs._element = (JSONObj*)malloc(3*sizeof(JSONObj));
        objs._element[0] = createJSONObj("action", "changedns");
        objs._element[1] = createJSONObj("status", "failed");
        objs._element[2] = createJSONObj("detail", "check your device or new dns address and try again");
        r_return = composeJSONObj(objs);
        return r_return;
    }
    sprintf(r_return, "{\"action\":\"changeip\",\"status\":\"success\",\"detail\":{\"dns1\":\"%s\",\"dns2\":\"%s\"}}", newdns1, newdns2);
    printf("[INFO]{HCNET}    '%s'@'%s'&'%s' Change device dns\n", ip, newdns1, newdns2);
	free(ip);
    free(user);
    free(pass);
    free(newdns1);
    free(newdns2);
    return r_return;
}

#endif