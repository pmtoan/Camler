#### About Camler
* Camler is a Rest API server camera controller  
* Base on [Cwes](https://github.com/pmtoan/Cwes) 
* Manufacturer support:  
>* [HIK VISION](https://www.hikvision.com/en)  

#### Build and run server
```
git clone https://github.com/pmtoan/Camler.git
cd Camler
g++ -w -L.  -o server server.c -Wl,-rpath=./:./HCNetSDKCom:. -lhcnetsdk  
./server
```
#### Rest API
##### Header
* Content-Type: application/x-www-form-urlencoded
* TokenAPI: key in file APIKeyAdmin.txt

##### Body
###### Scanning, scan ip of HCNet device in a network range
>Method: POST  
>Url: [ip]:[port]/hcnet/scanning  
>Parameters:  
>* iprange: Network ip range for scanning, ex: 172.22.22   

>Example with curl
```
curl -H "TokenAPI: 541XBgagY3zJ96SMM1slGF10uc3v5a374yjN1v1ycQz6iULwV7" -d "iprange=172.22.22" -X POST http://127.0.0.1:30497/hcnet/scanning
```  
>JSON response
```
{"action":"access","status":"unauthorized","detail":"check url or parameter and try again"}
{"action":"scan","status":"success","detail":['list ip of up device']}
```
###### Capture picture
>Method: POST  
>Url: [ip]:[port]/hcnet/capture  
>Parameters:  
>* ip: ip of HCNet device  
>* user: username  
>* pass: password  

>Example with curl
```
curl -H "TokenAPI: 541XBgagY3zJ96SMM1slGF10uc3v5a374yjN1v1ycQz6iULwV7" -d "ip=192.168.1.10&user=admin&pass=password" -X POST http://127.0.0.1:30497/hcnet/capture
```
>JSON response
```
{"action":"access","status":"unauthorized","detail":"check url or parameter and try again"}
{"action":"login","status":"failed","detail":"check your IP address of device or username and password"}  
{"action":"capture","status":"failed","detail":"capture failed, check your device or try again"}  
{"action":"capture","status":"success","detail":"storage/picture.jpeg"}
```
###### Get account information
>Method: POST  
>Url: [ip]:[port]/hcnet/get/userinfo  
>Parameters:  
>* ip: ip of HCNet device  
>* user: username  
>* pass: password 

>Example with curl
```
curl -H "TokenAPI: 541XBgagY3zJ96SMM1slGF10uc3v5a374yjN1v1ycQz6iULwV7" -d "ip=192.168.1.10&user=admin&pass=password" -X POST http://127.0.0.1:30497/hcnet/get/userinfo
```
>JSON response
```
{"action":"access","status":"unauthorized","detail":"check url or parameter and try again"}
{"action":"login","status":"failed","detail":"check your IP address of device or username and password"}  
{"action":"getuserinfo","status":"failed","detail":"can't get device config parameter, check your device and try again"}  
{"action":"getuserinfo","status":"success","detail":['list username and password']}
```
###### Get network information
>Method: POST  
>Url: [ip]:[port]/hcnet/get/netinfo  
>Parameters:  
>* ip: ip of HCNet device  
>* user: username  
>* pass: password 

>Example with curl
```
curl -H "TokenAPI: 541XBgagY3zJ96SMM1slGF10uc3v5a374yjN1v1ycQz6iULwV7" -d "ip=192.168.1.10&user=admin&pass=password" -X POST http://127.0.0.1:30497/hcnet/get/netinfo
```
>JSON response
```
{"action":"access","status":"unauthorized","detail":"check url or parameter and try again"}
{"action":"login","status":"failed","detail":"check your IP address of device or username and password"}  
{"action":"getnetinfo","status":"failed","detail":"can't get device config parameter, check your device and try again"}  
{"action":"getnetinfo","status":"success","detail":[ip, netmask, httpport, dns1, dns2, gw]}
```
###### Change ip address of device
>Method: POST  
>Url: [ip]:[port]/hcnet/change/ip  
>Parameters:  
>* ip: ip of HCNet device  
>* user: username  
>* pass: password 
>* newip: new ip address  
>* newmask: new subnet mask of new ip  
>* eport: ethernet port on device, 0 or 1, default 0

>Example with curl
```
curl -H "TokenAPI: 541XBgagY3zJ96SMM1slGF10uc3v5a374yjN1v1ycQz6iULwV7" -d "ip=192.168.1.10&user=admin&pass=password&newip=192.168.1.12&newmask=255.255.255.0&eport=0" -X POST http://127.0.0.1:30497/hcnet/change/ip
```
>JSON response
```
{"action":"access","status":"unauthorized","detail":"check url or parameter and try again"}
{"action":"login","status":"failed","detail":"check your IP address of device or username and password"}  
{"action":"getnetinfo","status":"failed","detail":"can't get device config parameter, check your device and try again"}
{"action":"changeip","status":"failed","detail":"check your device or new ip address and try again"}  
{"action":"changeip","status":"success","detail":newip}
```
###### Change password of account
>Method: POST  
>Url: [ip]:[port]/hcnet/change/pass  
>Parameters:  
>* ip: ip of HCNet device  
>* user: username  
>* pass: password 
>* puser: username change password
>* ppass: new password of puser  

>Example with curl
```
curl -H "TokenAPI: 541XBgagY3zJ96SMM1slGF10uc3v5a374yjN1v1ycQz6iULwV7" -d "ip=192.168.1.10&user=admin&pass=password&puser=admin&ppass=password" -X POST http://127.0.0.1:30497/hcnet/change/pass
```
>JSON response
```
{"action":"access","status":"unauthorized","detail":"check url or parameter and try again"}
{"action":"login","status":"failed","detail":"check your IP address of device or username and password"}  
{"action":"getuserinfo","status":"failed","detail":"can't get device config parameter, check your device and try again"}
{"action":"changepass","status":"failed","detail":"check your device or try again"}  
{"action":"changepass","status":"success","detail":newpass}
```
###### Change DNS server
>Method: POST  
>Url: [ip]:[port]/hcnet/change/dns  
>Parameters:  
>* ip: ip of HCNet device  
>* user: username  
>* pass: password 
>* dns1: IP of main-DNS server
>* dns2: IP of sub-DNS server

>Example with curl
```
curl -H "TokenAPI: 541XBgagY3zJ96SMM1slGF10uc3v5a374yjN1v1ycQz6iULwV7" -d "ip=192.168.1.10&user=admin&pass=password&dns1=8.8.8.8&dns2=8.8.4.4" -X POST http://127.0.0.1:30497/hcnet/change/dns
```
>JSON response
```
{"action":"access","status":"unauthorized","detail":"check url or parameter and try again"}
{"action":"login","status":"failed","detail":"check your IP address of device or username and password"}  
{"action":"getnwtinfo","status":"failed","detail":"can't get device config parameter, check your device and try again"}
{"action":"changedns","status":"failed","detail":"check your device or new dns address and try again"}  
{"action":"changedns","status":"success","detail":{dns1, dns2}}
```
###### Change gateway address
>Method: POST  
>Url: [ip]:[port]/hcnet/change/gw  
>Parameters:  
>* ip: ip of HCNet device  
>* user: username  
>* pass: password 
>* gw: gateway address

>Example with curl
```
curl -H "TokenAPI: 541XBgagY3zJ96SMM1slGF10uc3v5a374yjN1v1ycQz6iULwV7" -d "ip=192.168.1.10&user=admin&pass=password&gw=192.168.1.1" -X POST http://127.0.0.1:30497/hcnet/change/gw
```
>JSON response
```
{"action":"access","status":"unauthorized","detail":"check url or parameter and try again"}
{"action":"login","status":"failed","detail":"check your IP address of device or username and password"}  
{"action":"getnetinfo","status":"failed","detail":"can't get device config parameter, check your device and try again"}
{"action":"changegw","status":"failed","detail":"check your device or new gateway address and try again"}  
{"action":"changegw","status":"success","detail":newgw}
