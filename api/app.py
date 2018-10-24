from flask import Flask, request, session, redirect
import controller

app = Flask(__name__)

Conler = controller.Controller(
	"127.0.0.1", \
	30497, \
	"GXwmHg7kuYw0UQv5wRwzsUBKmtXe9TbrvuGYnoLKzDjCqdApPwI4APEI77HiWqklHpRtofrakiECY4541XBgagY3zJ96SMM1slGF10uc3v5a374yjN1v1ycQz6iULwV7" \
)

@app.route("/hcnet/scanning", methods=["POST"])
def hcnetScan():
	return Conler.handleHcnetScanning(request.form.get("iprange"))

@app.route("/hcnet/capture", methods=["POST"])
def hcnetCapture():
	return Conler.handleHcnetCapture( \
		request.form.get("ip"), \
		request.form.get("user"), \
		request.form.get("pass")
	)

@app.route("/hcnet/get/userinfo", methods=["POST"])
def hcnetGetAccountInformation():
	return Conler.handleHcnetGetAccountInformation( \
		request.form.get("ip"), \
		request.form.get("user"), \
		request.form.get("pass")
	)

@app.route("/hcnet/get/netinfo", methods=["POST"])
def hcnetGetNetworkInformation():
	return Conler.handleHcnetGetNetworkInformation( \
		request.form.get("ip"), \
		request.form.get("user"), \
		request.form.get("pass")
	)

@app.route("/hcnet/change/ip", methods=["POST"])
def hcnetChangeNetworkIP():
	return Conler.handleHcnetChangeNetworkIP( \
		request.form.get("ip"), \
		request.form.get("user"), \
		request.form.get("pass"), \
		request.form.get("newip"), \
		request.form.get("newmask"), \
		request.form.get("eport"),
	)

@app.route("/hcnet/change/dns", methods=["POST"])
def hcnetChangeNetworkDNS():
	return Conler.handleHcnetChangeNetworkDNS( \
		request.form.get("ip"), \
		request.form.get("user"), \
		request.form.get("pass"), \
		request.form.get("dns1"), \
		request.form.get("dns2"),	
	)

@app.route("/hcnet/change/gw", methods=["POST"])
def hcnetChangeNetworkGateway():
	return Conler.handleHcnetChangeNetworkGateway( \
		request.form.get("ip"), \
		request.form.get("user"), \
		request.form.get("pass"), \
		request.form.get("gw"),	
	)

@app.route("/hcnet/change/pass", methods=["POST"])
def hcnetChangePassword():
	return Conler.handleHcnetChangePassword( \
		request.form.get("ip"), \
		request.form.get("user"), \
		request.form.get("pass"), \
		request.form.get("puser"), \
		request.form.get("ppass"),	
	)

@app.route("/hcnet/rtsp", methods=["POST"])
def hcnetInitRtspLiveView():
	ips = str(request.form.get("ips")).split()
	return Conler.handleHcnetInitRtspLiveView( \
		ips
	)

@app.route("/<path:path>")
def unsupport(path):
	return "{\"status\":\"unsupport\"}"

if __name__ == '__main__':
	# app.secret_key = 'A0Zr98j/3yX R~XHH!jmN]LWX/,?RT'
	app.run(debug = True, host="0.0.0.0", port=80)