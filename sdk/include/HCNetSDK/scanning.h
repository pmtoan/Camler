#ifndef SCANNING
#define SCANNING
#include "libHCNet.h"
#include "../utils.h"
#include "../protocol.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"

char* hcnet_scanning(const char* ip_range, int begin, int end)
{
	char* status = (char*)malloc(4*SIZE_LARGE);
    NET_DVR_SetConnectTime(300, 1);
    NET_DVR_DEVICEINFO_V30 struDeviceInfo;
    char ip[16];
    char ip_up[255][16];
    int i;
    int up_index = 0;
    for (i=begin;i<=end;i++)
    {
        sprintf(ip, "%s.%d", ip_range, i);
        printf("[INFO]{HCNet}    Scanning %s", ip);
        LONG lUserID = NET_DVR_Login_V30(ip, 8000, "scanning", "scanning", &struDeviceInfo);
        if (lUserID < 0)
        {
            int error = NET_DVR_GetLastError();
            printf(" status %d\n", error);
            if (error == 152 || error == 1)
            {
                memcpy(ip_up[up_index], ip, sizeof(ip));
                up_index++;
            }
        }
    }
    sprintf(status, "[");
    for(i=0;i<up_index;i++)
    {
        json_objs objs;
        objs._size = 2;
        objs._element = (json_obj*)malloc(i*sizeof(json_obj));
        objs._element[0] = create_json("ip", ip_up[i]);
        objs._element[1] = create_json("company", "HCNet");
        if (i>0)
            sprintf(status, "%s,%s", status, compose_json(objs));
        else
            sprintf(status, "%s%s", status, compose_json(objs));
    }
    strcat(status, "]");
    char* r_return = (char*)malloc(1024);
    sprintf(r_return, "{\"action\":\"scan\",\"status\":\"success\",\"detail\":%s}", status);
    return r_return;
}

#endif