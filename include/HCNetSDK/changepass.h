#ifndef CHANGEPASS_H
#define CHANGEPASS_H
#include "libHCNet.h"
#include "../type.h"
#include "../utils.h"
#include "../protocol.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"

char* HCNetChangePass(const char* s_ip, int s_port, const char* s_user, const char* s_pass, 
    const char* p_user, const char* p_pass)
{
    /*
    *   Change password of a user account
    *   Login info: s_ip, s_port, s_user, s_pass
    *   User info:
    *       p_user: Username need to change password
    *       p_pass: new password
    */
	char* ip = (char*)malloc(1024);
    sprintf(ip, "%s", s_ip);
    char* user = (char*)malloc(1024);
    sprintf(user, "%s", s_user);
    char* pass = (char*)malloc(1024);
    sprintf(pass, "%s", s_pass);
    char* user_change = (char*)malloc(1024);
    sprintf(user_change, "%s", p_user);
    char* new_pass = (char*)malloc(1024);
    sprintf(new_pass, "%s", p_pass);
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
    /* Get device user config parameter */
    NET_DVR_USER_V30 struParams = {0};
    struParams.dwSize = sizeof(NET_DVR_USER_V30);
    iRet = NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_USERCFG_V30, lChannel, \
        &struParams, sizeof(NET_DVR_USER_V30), &uiReturnLen);
    if (!iRet)
    {
        printf("[ERROR]{HCNET}    '%s'@'%s' Get user config parameter error %d\n", ip, user, NET_DVR_GetLastError());
        objs._size = 3;
    	objs._element = (JSONObj*)malloc(3*sizeof(JSONObj));
        objs._element[0] = createJSONObj("action", "getuserinfo");
        objs._element[1] = createJSONObj("status", "failed");
        objs._element[2] = createJSONObj("detail", "can't get device config parameter, check your device and try again");
        r_return = composeJSONObj(objs);
        return r_return;
    }
   	for (int i = 0; i < MAX_USERNUM_V30; i++)
   	{
        if(struParams.struUser[i].sUserName[0] == NULL)
            break;
        char username[32];
        memcpy(username, struParams.struUser[i].sUserName, sizeof(struParams.struUser[i].sUserName));
        if (strcmp(username, user_change) == 0)
        {
            memcpy(struParams.struUser[i].sPassword, new_pass, NAME_LEN);
            iRet = NET_DVR_SetDVRConfig(lUserID, NET_DVR_SET_USERCFG_V30, lChannel, \
                &struParams, sizeof(NET_DVR_USER_V30));
            if (!iRet)
            {
                printf("[ERROR]{HCNET}    '%s'@'%s' Can't set new password %d\n", ip, user, NET_DVR_GetLastError());
                objs._size = 3;
                objs._element = (JSONObj*)malloc(3*sizeof(JSONObj));
                objs._element[0] = createJSONObj("action", "changepass");
                objs._element[1] = createJSONObj("status", "failed");
                objs._element[2] = createJSONObj("detail", "check your device or try again");
                r_return = composeJSONObj(objs);
                return r_return;
            }
            printf("[INFO]{HCNET}    '%s' - '%s'@'%s' Change password of device\n", ip, user_change, new_pass);
            objs._size = 3;
            objs._element = (JSONObj*)malloc(3*sizeof(JSONObj));
            objs._element[0] = createJSONObj("action", "changepass");
            objs._element[1] = createJSONObj("status", "success");
            objs._element[2] = createJSONObj("detail", new_pass);
            r_return = composeJSONObj(objs);
            return r_return;
        }
   	}
    printf("[ERROR]{HCNET}    '%s'@'%s' Username not exists %d\n", ip, user, NET_DVR_GetLastError());
    objs._size = 3;
    objs._element = (JSONObj*)malloc(3*sizeof(JSONObj));
    objs._element[0] = createJSONObj("action", "changepass");
    objs._element[1] = createJSONObj("status", "failed");
    objs._element[2] = createJSONObj("detail", "username not exists");
    r_return = composeJSONObj(objs);
    free(ip);
    free(user);
    free(pass);
    free(user_change);
    free(new_pass);
    return r_return;
}

#endif