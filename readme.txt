Step 1: Compile
    cd $HOME/camler
	g++ -w -L.  -o server server.c -Wl,-rpath=./:./HCNetSDKCom:. -lhcnetsdk

Step 2: Run
    Run server default port 30497
        cd $HOME/camler
        ./server

    Run server with other port
        ./server [port]
