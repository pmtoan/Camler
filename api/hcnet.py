import requests

class Hcnet():
	"""docstring for Hcnet, HIK VISION camera firmware"""
	def __init__(self, gatewayIP, gatewayPort, gatewayTokenAPI):
		self._gatewayIP = gatewayIP
		self._gatewayPort = gatewayPort
		self._gatewayTokenAPI = gatewayTokenAPI
		self._uri = {
			"hcnetScan": "/hcnet/scanning",
			"hcnetCapture": "/hcnet/capture",
			"hcnetGetAccountInfo": "/hcnet/get/userinfo",
			"hcnetGetNetworkInfo": "/hcnet/get/netinfo",
			"hcnetChangeNetworkIP": "/hcnet/change/ip",
			"hcnetChangeNetworkDNS": "/hcnet/change/dns",
			"hcnetChangeNetworkGW": "/hcnet/change/gw",
			"hcnetChangePassword": "/hcnet/change/pass"
		}
		self._protocol = "http://"
		self._serverError = "{\"status\":\"no\", \"detail\":\"API server problem\"}"
	def scanning(self, ipRange):
		if (ipRange.count('.') == 3):
			(subnet1, subnet2, subnet3, subnet4) = ipRange.split('.')
			ipRange = str(subnet1) + "." + str(subnet2) + "." + str(subnet3)
		try:
			url = self._protocol + self._gatewayIP + ":" + str(self._gatewayPort) + self._uri["hcnetScan"]
			r = requests.post(
				url,
				headers = {
					'TokenAPI': self._gatewayTokenAPI,
				},
				data="iprange=" + str(ipRange)
			);
			return r.text
		except Exception as e:
			return self._serverError
	def capture(self, cameraIP, usr, pwd):
		try:
			url = self._protocol + self._gatewayIP + ":" + str(self._gatewayPort) + self._uri["hcnetCapture"]
			r = requests.post(
				url,
				headers = {
					'TokenAPI': self._gatewayTokenAPI,
				},
				data="ip=" + str(cameraIP) + "&user=" + str(usr) + "&pass=" + str(pwd)
			);
			return r.text
		except Exception as e:
			return self._serverError
	def getInformation(self, whichInfo, cameraIP, usr, pwd):
		if (whichInfo == "NETWORK"):
			url = self._protocol + self._gatewayIP + ":" + str(self._gatewayPort) + self._uri["hcnetGetNetworkInfo"]
		elif (whichInfo == "ACCOUNT"):
			url = self._protocol + self._gatewayIP + ":" + str(self._gatewayPort) + self._uri["hcnetGetAccountInfo"]
		else:
			return self._serverError
		try:
			r = requests.post(
				url,
				headers = {
					'TokenAPI': self._gatewayTokenAPI,
				},
				data="ip=" + str(cameraIP) + "&user=" + str(usr) + "&pass=" + str(pwd)
			);
			return r.text
		except Exception as e:
			return self._serverError
	def changeConfiguration(self, whatConfig, newConfig, cameraIP, usr, pwd):
		if (whatConfig == "NETWORK_IP"):
			url = self._protocol + self._gatewayIP + ":" + str(self._gatewayPort) + self._uri["hcnetChangeNetworkIP"]
			parameters = "ip=" + str(cameraIP) \
				+ "&user=" + str(usr) \
				+ "&pass=" + str(pwd) \
				+ "&newip=" + str(newConfig["newip"]) \
				+ "&newmask=" + str(newConfig["newmask"]) \
				+ "&eport=" + str(newConfig["eport"])
		elif (whatConfig == "NETWORK_DNS"):
			url = self._protocol + self._gatewayIP + ":" + str(self._gatewayPort) + self._uri["hcnetChangeNetworkDNS"]
			parameters = "ip=" + str(cameraIP) \
				+ "&user=" + str(usr) \
				+ "&pass=" + str(pwd) \
				+ "&dns1=" + str(newConfig["dns1"]) \
				+ "&dns2=" + str(newConfig["dns1"])
		elif (whatConfig == "NETWORK_GATEWAY"):
			url = self._protocol + self._gatewayIP + ":" + str(self._gatewayPort) + self._uri["hcnetChangeNetworkGW"]
			parameters = "ip=" + str(cameraIP) \
				+ "&user=" + str(usr) \
				+ "&pass=" + str(pwd) \
				+ "&gw=" + str(newConfig["gw"])
		elif (whatConfig == "PASSWORD"):
			url = self._protocol + self._gatewayIP + ":" + str(self._gatewayPort) + self._uri["hcnetChangePassword"]
			parameters = "ip=" + str(cameraIP) \
				+ "&user=" + str(usr) \
				+ "&pass=" + str(pwd) \
				+ "&puser=" + str(newConfig["puser"]) \
				+ "&ppass=" + str(newConfig["ppass"])
		try:
			r = requests.post(
				url,
				headers = {
					'TokenAPI': self._gatewayTokenAPI,
				},
				data=parameters
			);
			return r.text
		except Exception as e:
			return self._serverError