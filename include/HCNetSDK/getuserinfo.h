#ifndef GETUSERINFO_H
#define GETUSERINFO_H
#include "libHCNet.h"
#include "../utils.h"
#include "../protocol.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"

char* hcnet_get_user_info(const char* s_ip, int s_port, const char* s_user, const char* s_pass)
{
    char* ip = (char*)malloc(1024);
    sprintf(ip, "%s", s_ip);
    char* user = (char*)malloc(1024);
    sprintf(user, "%s", s_user);
    char* pass = (char*)malloc(1024);
    sprintf(pass, "%s", s_pass);
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
    /* Get device user config parameter */
    NET_DVR_USER_V30 struParams = {0};
    struParams.dwSize = sizeof(NET_DVR_USER_V30);
    iRet = NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_USERCFG_V30, lChannel, \
        &struParams, sizeof(NET_DVR_USER_V30), &uiReturnLen);
    if (!iRet)
    {
        printf("[ERROR]{HCNET}    '%s'@'%s' Get user config parameter error %d\n", ip, user, NET_DVR_GetLastError());
        objs._size = 3;
    	objs._element = (json_obj*)malloc(3*sizeof(json_obj));
        objs._element[0] = create_json("action", "getuserinfo");
        objs._element[1] = create_json("status", "failed");
        objs._element[2] = create_json("detail", "can't get device config parameter, check your device and try again");
        r_return = compose_json(objs);
        return r_return;
    }
    char* listUser = (char*)malloc(1024);
   	for (int i = 0; i < MAX_USERNUM_V30; ++i)
   	{
   		objs._size = 2;
   		objs._element = (json_obj*)malloc(2*sizeof(json_obj));
   		if(struParams.struUser[i].sUserName[0] == NULL)
   			break;
   		objs._element[0] = create_json("username", (char*)(struParams.struUser[i].sUserName));
   		objs._element[1] = create_json("password", (char*)(struParams.struUser[i].sPassword));
   		if (i > 0)
   			sprintf(listUser, "%s,%s", listUser, compose_json(objs));
   		else
   			sprintf(listUser, "[%s", compose_json(objs));
   	}
   	sprintf(listUser, "%s]", listUser);
   	sprintf(r_return, "{\"action\":\"getuserinfo\",\"status\":\"success\",\"detail\":");
   	sprintf(r_return, "%s%s}", r_return, listUser);
    printf("[INFO]{HCNET}    '%s'@'%s' Get user information \n", ip, user);
   	free(listUser);
    free(ip);
    free(user);
    free(pass);
   	return r_return;
}

#endif