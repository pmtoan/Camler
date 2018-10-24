Http API

Function				Method			URL							Parameter
Capture					POST			/hcnet/capture				ip, user, pass
Scanning				POST			/hcnet/scanning 			iprange
Get account info 		POST			/hcnet/get/userinfo			ip, user, pass
Change password			POST			/hcnet/change/pass			ip, user, pass, puser, ppass
Get net info 			POST			/hcnet/get/netinfo			ip, user, pass
Change ip address		POST			/hcnet/change/ip			ip, user, pass, newip, newmask, eport
Change DNS server		POST			/hcnet/change/dns			ip, user, pass, dns1, dns2
Change Gateway address	POST 			/hcnet/change/gw			ip, user, pass, gw
