#ifndef CHANGEGW_H
#define CHANGEGW_H
#include "libHCNet.h"
#include "../utils.h"
#include "../protocol.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"
char* hcnet_change_gateway(const char* s_ip, int s_port, const char* s_user, const char* s_pass, 
	const char* gate_way)
{
	/*
	*	Change gateway addres of device
	*	gate_way:	new gateway address 
	*/
	char* ip = (char*)malloc(1024);
    sprintf(ip, "%s", s_ip);
    char* user = (char*)malloc(1024);
    sprintf(user, "%s", s_user);
    char* pass = (char*)malloc(1024);
    sprintf(pass, "%s", s_pass);
    char* newgw = (char*)malloc(1024);
    sprintf(newgw, "%s", gate_way);
	char* r_return = (char*)malloc(1024);
	/* Login device */
	int iRet = -1;
    DWORD uiReturnLen = 0;
    LONG lChannel = 0;
    NET_DVR_SetConnectTime(300, 1);
    NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	LONG lUserID = NET_DVR_Login_V30(ip, s_port, user, pass, &struDeviceInfo);
	json_objs objs;
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
    	objs._element = (json_obj*)malloc(3*sizeof(json_obj));
        objs._element[0] = create_json("action", "login");
        objs._element[1] = create_json("status", "failed");
        objs._element[2] = create_json("detail", "check your IP address of device or username and password");
        r_return = compose_json(objs);
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
    	objs._element = (json_obj*)malloc(3*sizeof(json_obj));
        objs._element[0] = create_json("action", "getnetinfo");
        objs._element[1] = create_json("status", "failed");
        objs._element[2] = create_json("detail", "can't get device config parameter, check your device and try again");
        r_return = compose_json(objs);
        return r_return;
    }
    /* Set new DNS address */
    memcpy(struParams.struGatewayIpAddr.sIpV4, newgw, 16);
    iRet = NET_DVR_SetDVRConfig(lUserID, NET_DVR_SET_NETCFG_V30, lChannel, \
        &struParams, sizeof(NET_DVR_NETCFG_V30));
    if (!iRet)
    {
        printf("[ERROR]{HCNET}    '%s'@'%s' Can't set new gateway address, error %d\n", ip, user, NET_DVR_GetLastError());
        objs._size = 3;
        objs._element = (json_obj*)malloc(3*sizeof(json_obj));
        objs._element[0] = create_json("action", "changedns");
        objs._element[1] = create_json("status", "failed");
        objs._element[2] = create_json("detail", "check your device or new gateway address and try again");
        r_return = compose_json(objs);
        return r_return;
    }
    sprintf(r_return, "{\"action\":\"changeip\",\"status\":\"success\",\"detail\":{\"gateway\":\"%s\"}}", newgw);
    printf("[INFO]{HCNET}    '%s'@'%s' Change device gateway\n", ip, newgw);
	free(ip);
    free(user);
    free(pass);
    free(newgw);
    return r_return;
}
#endif