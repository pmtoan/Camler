#ifndef TYPE_H
#define TYPE_H
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/*
*	HTTP Method handle
*/
#define POST "POST"
#define GET "GET"

/*
*	Size
*/
#define WORD_T 4
#define KBYTE_T 1024

/*
*	Define variables for server default infomation
*/
#define DEFAULT_PORT 30497
#define DEFAULT_STORAGE "storage"
#define DEFAULT_HCNET_PORT 8000

/*
*	Define string value for  char* 
*/
#define NOTSET	"@#notset#@"
#define UNAUTHORIZED "{\"action\":\"access\",\"status\":\"unauthorized\",\"detail\":\"check url or parameter and try again\"}"
#define NOTFOUND "{\"action\":\"access\",\"status\":\"not found\",\"detail\":\"check url and try again\"}"

/*
*	Admin ssername and password
*/
#define ADMIN_USER	"admin"
#define ADMIN_PASS	"admin123"
#define API_TOKEN "GXwmHg7kuYw0UQv5wRwzsUBKmtXe9TbrvuGYnoLKzDjCqdApPwI4APEI77HiWqklHpRtofrakiECY4541XBgagY3zJ96SMM1slGF10uc3v5a374yjN1v1ycQz6iULwV7"

#endif