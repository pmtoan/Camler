#ifndef CHANGEIP_H
#define CHANGEIP_H
#include "libHCNet.h"
#include "../type.h"
#include "../utils.h"
#include "../protocol.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"

char* HCNetChangeIp(const char* s_ip, int s_port, const char* s_user, const char* s_pass, 
	const char* new_ip, const char* new_mask, int ethernet_port)
{
	/*
	*	Change ip address and subnet mask of device
	*	new_ip:	New ip address of device
	*	new_mask:	New subnet mask of device
	*	ethernet_port:	Ethernet port of device, set to port 0 or 1, default is 0
	*/
	char* ip = (char*)malloc(1024);
    sprintf(ip, "%s", s_ip);
    char* user = (char*)malloc(1024);
    sprintf(user, "%s", s_user);
    char* pass = (char*)malloc(1024);
    sprintf(pass, "%s", s_pass);
    char* newip = (char*)malloc(1024);
    sprintf(newip, "%s", new_ip);
    char* newmask = (char*)malloc(1024);
    sprintf(newmask, "%s", new_mask);
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
    /* Set new ip address */
    memcpy(struParams.struEtherNet[ethernet_port].struDVRIP.sIpV4, newip, 16);
    memcpy(struParams.struEtherNet[ethernet_port].struDVRIPMask.sIpV4, newmask, 16);
    iRet = NET_DVR_SetDVRConfig(lUserID, NET_DVR_SET_NETCFG_V30, lChannel, \
        &struParams, sizeof(NET_DVR_NETCFG_V30));
    if (!iRet)
    {
        printf("[ERROR]{HCNET}    '%s'@'%s' Can't set new ip address %d\n", ip, user, NET_DVR_GetLastError());
        objs._size = 3;
        objs._element = (JSONObj*)malloc(3*sizeof(JSONObj));
        objs._element[0] = createJSONObj("action", "changeip");
        objs._element[1] = createJSONObj("status", "failed");
        objs._element[2] = createJSONObj("detail", "check your device or new ip address and try again");
        r_return = composeJSONObj(objs);
        return r_return;
    }
    sprintf(r_return, "{\"action\":\"changeip\",\"status\":\"success\",\"detail\":{\"ip\":\"%s\",\"netmask\":\"%s\"}}", newip, newmask);
    NET_DVR_RebootDVR(lUserID);
    printf("[INFO]{HCNET}    Change device ip from '%s' to '%s'\n", ip, newip);
	free(ip);
    free(user);
    free(pass);
    free(newip);
    free(newmask);
    return r_return;
}

#endif