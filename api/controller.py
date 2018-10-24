import requests
import hcnet
import rtsp

class Controller():
	"""docstring for Controller"""
	def __init__(self, sdkServerIP, sdkServerPort, sdkServerTokenAPI):
		self._sdkServerIP = sdkServerIP
		self._sdkServerPort = sdkServerPort
		self._gatewayTokenAPI = sdkServerTokenAPI
		self._hcnet = hcnet.Hcnet(self._sdkServerIP, self._sdkServerPort, self._gatewayTokenAPI)
		self._rtsp = rtsp.Rtsp(8900)
	def handleHcnetScanning(self, ipRange):
		return self._hcnet.scanning(ipRange)
	def handleHcnetCapture(self, cameraIP, usr, pwd):
		return self._hcnet.capture(cameraIP, usr, pwd)
	def handleHcnetGetAccountInformation(self, cameraIP, usr, pwd):
		return self._hcnet.getInformation("ACCOUNT", cameraIP, usr, pwd)
	def handleHcnetGetNetworkInformation(self, cameraIP, usr, pwd):
		return self._hcnet.getInformation("NETWORK", cameraIP, usr, pwd)
	def handleHcnetChangeNetworkIP(self, cameraIP, usr, pwd, newip, newmask, eport):
		newConfig = {
			"newip": newip,
			"newmask": newmask,
			"eport": eport
		}
		return self._hcnet.changeConfiguration("NETWORK_IP", newConfig, cameraIP, usr, pwd)
	def handleHcnetChangeNetworkDNS(self, cameraIP, usr, pwd, dns1, dns2):
		newConfig = {
			"dns1": dns1,
			"dns2": dns2
		}
		return self._hcnet.changeConfiguration("NETWORK_DNS", newConfig, cameraIP, usr, pwd)
	def handleHcnetChangeNetworkGateway(self, cameraIP, usr, pwd, gw):
		newConfig = {
			"gw": gw
		}
		return self._hcnet.changeConfiguration("NETWORK_GATEWAY", newConfig, cameraIP, usr, pwd)
	def handleHcnetChangePassword(self, cameraIP, usr, pwd, puser, ppass):
		newConfig = {
			"puser": puser,
			"ppass": ppass
		}
		return self._hcnet.changeConfiguration("PASSWORD", newConfig, cameraIP, usr, pwd)
	def handleHcnetInitRtspLiveView(self, ips):
		return self._rtsp.initHcnetLiveView(ips)
