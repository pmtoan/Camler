#ifndef CAPTURE_H
#define CAPTURE_H

#include "libHCNet.h"
#include "../type.h"
#include "../utils.h"
#include "../protocol.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"

char* HCNetCapture(const char* s_ip, int s_port, const char* s_user, const char* s_pass)
{
    char* ip = (char*)malloc(1024);
    sprintf(ip, "%s", s_ip);
    char* user = (char*)malloc(1024);
    sprintf(user, "%s", s_user);
    char* pass = (char*)malloc(1024);
    sprintf(pass, "%s", s_pass);
	char* r_return = (char*)malloc(1024);
    NET_DVR_SetConnectTime(300, 1);
    long lUserID;
    NET_DVR_DEVICEINFO_V30 struDeviceInfo;
    lUserID = NET_DVR_Login_V30(ip, s_port, user, pass, &struDeviceInfo);
    JSONObjs objs;
    objs._size = 3;
    objs._element = (JSONObj*)malloc(3*sizeof(JSONObj));
    if (lUserID < 0)
    {
        int error = NET_DVR_GetLastError();
        if (error == 1)
            printf("[ERROR]{HCNET}    '%s'@'%s' Login error, username or password is not valid\n", ip, user);
        else if (error==7)
            printf("[ERROR]{HCNET}    '%s'@'%s' Login error, connect to device failed\n", ip, user);
        else if (error==152)
            printf("[ERROR]{HCNET}    '%s'@'%s' Login error, username not exists\n", ip, user);
        else
            printf("[ERROR]{HCNET}    '%s'@'%s' Login error, unknown\n", ip, user);
        objs._element[0] = createJSONObj("action", "login");
        objs._element[1] = createJSONObj("status", "failed");
        objs._element[2] = createJSONObj("detail", "check your IP address of device or username and password");
        r_return = composeJSONObj(objs);
        return r_return;
    }
    NET_DVR_JPEGPARA strPicPara = {0};
    strPicPara.wPicQuality = 2;
    strPicPara.wPicSize = 0;
    int iRet;
    char* default_path = (char*)malloc(1024);
    sprintf(default_path, "%s/%ld.jpeg", DEFAULT_STORAGE, timestamp());
    iRet = NET_DVR_CaptureJPEGPicture(lUserID, struDeviceInfo.byStartChan, &strPicPara, default_path);
    if (!iRet)
    {
        printf("[ERROR]{HCNET}    '%s'@'%s' NET_DVR_CaptureJPEGPicture error, %d\n", ip, user, NET_DVR_GetLastError());
        objs._element[0] = createJSONObj("action", "capture");
        objs._element[1] = createJSONObj("status", "failed");
        objs._element[2] = createJSONObj("detail", "capture failed, check your device or try again");
        r_return = composeJSONObj(objs);
        return r_return;
    }
    objs._element[0] = createJSONObj("action", "capture");
    objs._element[1] = createJSONObj("status", "suscess");
    sprintf(r_return, "%s", default_path);
    objs._element[2] = createJSONObj("detail", r_return);
    r_return = composeJSONObj(objs);
    NET_DVR_Logout_V30(lUserID);
    printf("[INFO]{HCNET}    '%s'@'%s' Capture JPEG picture from device\n", ip, user);    
    free(default_path);
    free(ip);
    free(user);
    free(pass);
    return r_return;
}
#endif