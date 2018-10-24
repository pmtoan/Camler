#ifndef CAMLER_SERVER_H
#define CAMLER_SERVER_H
/* System libraries
*/
#include "stdio.h"
#include "stdlib.h"
#include "stdlib.h"
#include "sys/types.h"
#include "signal.h"
#include "unistd.h"
#include "time.h"

/* Main header
*/
#include "utils.h"
#include "protocol.h"
#include "stdsoc.h"
#include "handle.h"

/* SDK header
*/
#include "HCNetSDK/libHCNet.h"

void initServer()
{
	/* Initialize SDK
	*/
	NET_DVR_Init();
}
void cleanServer()
{
	/* Cleanup SDK
	*/
	NET_DVR_Cleanup();
}
#endif