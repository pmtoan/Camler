import sys
import os

class Rtsp():
	"""docstring for Rtsp"""
	def __init__(self, port):
		self._port = port
	def initHcnetLiveView(self, cameras):
		cmd = "bin/rtspProxy -p " + str(self._port)
		for camera in cameras:
			cmd += " rtsp://" + str(camera)
		cmd += " &"
		os.system(cmd)
		return "{\"status\":\"rtsp://<api-server-ip>:8900/proxyStream\"}"
		