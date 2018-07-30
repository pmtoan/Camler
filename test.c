#include "stdio.h"
#include "stdlib.h"
#include "string.h"
// #include "include/server.h"
#include "include/protocol.h"
#include "include/utils.h"
// #include "include/HCNetSDK/capture.h"
// #include "include/HCNetSDK/scanning.h"
// #include "include/HCNetSDK/getuserinfo.h"
// #include "include/HCNetSDK/changepass.h"
// #include "include/HCNetSDK/getnetinfo.h"
// #include "include/HCNetSDK/changeip.h"
// #include "include/HCNetSDK/changedns.h"
// #include "include/HCNetSDK/changegw.h"

int main(int argc, char const *argv[])
{	
	printf("%d\n", compareStrStr(argv[1], argv[2]));
	return 0;
}