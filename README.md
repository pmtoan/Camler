#### About Camler
* Camler is a Rest API server camera controller  
* Base on [Cwes](https://github.com/pmtoan/Cwes) 
* Manufacturer support:  
>* [HIK VISOIN](https://www.hikvision.com/en)  

#### Build and run server
```
g++ -w -L.  -o server server.c -Wl,-rpath=./:./HCNetSDKCom:. -lhcnetsdk  
./server
```

#### Rest API
**Set token key in header**   
```
CwesTokenAPI: [token-key]
```
##### 1. HCNet
###### Scanning, scan ip of HCNet device in a network range
>Method: POST  
>Url: [ip]:[port]/hcnet/scanning  
>Parameters:  
>* iprange: Network ip range for scanning, ex: 172.22.22   

>Example with curl
```
curl -d "iprange=172.22.22" -X POST http://127.0.0.1:30497/hcnet/scanning
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
curl -d "ip=192.168.1.10&user=admin&pass=password" -X POST http://127.0.0.1:30497/hcnet/capture
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
curl -d "ip=192.168.1.10&user=admin&pass=password" -X POST http://127.0.0.1:30497/hcnet/get/userinfo
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
curl -d "ip=192.168.1.10&user=admin&pass=password" -X POST http://127.0.0.1:30497/hcnet/get/netinfo
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
curl -d "ip=192.168.1.10&user=admin&pass=password&newip=192.168.1.12&newmask=255.255.255.0&eport=0" -X POST http://127.0.0.1:30497/hcnet/change/ip
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
curl -d "ip=192.168.1.10&user=admin&pass=password&puser=admin&ppass=password" -X POST http://127.0.0.1:30497/hcnet/change/pass
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
curl -d "ip=192.168.1.10&user=admin&pass=password&dns1=8.8.8.8&dns2=8.8.4.4" -X POST http://127.0.0.1:30497/hcnet/change/dns
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
curl -d "ip=192.168.1.10&user=admin&pass=password&gw=192.168.1.1" -X POST http://127.0.0.1:30497/hcnet/change/gw
```
