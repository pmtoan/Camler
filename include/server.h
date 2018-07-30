#ifndef SERVER_H
#define SERVER_H
/* System libraries */
#include "stdio.h"
#include "stdlib.h"
#include "stdlib.h"
#include "sys/types.h"
#include "signal.h"
#include "unistd.h"
#include "time.h"
/* Main header */
#include "utils.h"
#include "protocol.h"
#include "type.h"
#include "stdsoc.h"
#include "handle.h"
#include "type.h"

/* SDK header */
#include "HCNetSDK/libHCNet.h"

void initServer()
{
	/*
	*	Initialize HCNet SDK
	*/
	NET_DVR_Init();
}
void cleanServer()
{
	/*
	*	Cleanup HCNet SDK
	*/
	NET_DVR_Cleanup();
}
#endif