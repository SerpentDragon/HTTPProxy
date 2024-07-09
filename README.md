# HTTP Proxy

This application is an http-proxy server which is used to connect to the server, read and download video streams with splitting them into chunks of fixed size (60 seconds)

The proxy and the client are the main components of this app. Proxy receives requests from the client like that:
```
GET "http://<ip_addr>:<port_num>?url=udp://<ip_addr>:<port_num>"
```
http://<ip_addr>:<port_num> is for the address of the proxy  
url=udp://<ip_addr>:<port_num> is the address of the server from which the stream is downloaded

Proxy can process multiple clients simultaneously. The folder is created for every client and all downloaded data are located in that very folder

Logging is maintained

The connection between proxy and the client is establihed vit TCP protocol. To connect proxy and server the protocol UDP is in use

FFMPEG API (libavformat, avcodec, avutils libraries) are used to split the stream into chunks

### Dependencies
To use this application you must have [Boost](https://www.boost.org/) and [FFMPEG](https://ffmpeg.org/) libraries installed on your PC

<b>Hot to install ffmpeg:</b>
```
sudo apt update && sudo apt upgrade
sudo apt install ffmpeg
```
or
```
sudo add-apt-repository ppa:jonathonf/ffmpeg-4
sudo apt update
sudo apt install ffmpeg
```

<b>How to install Boost:</b>
```
sudo apt update
sudo apt install libboost-all-dev
```

### Build & Run
To build & Run the app you have to
```
git clone git@github.com:SerpentDragon/HTTPProxy.git
cd HTTPProxy
mkdir build
cd build
cmake ..
cmake --build .
```

To run the proxy:
```
./proxy <proxy_host> <proxy_port>
```
<proxy_host> & <proxy_port> are for ip-address and port number on which the proxy will be launched

To run the client:
```
./client <proxy_host> <proxy_port> <server_host> <server_port>
```
<proxy_host> & <proxy_port> - ip-address & port of the proxy  
<server_host> & <server_port> - ip-address & port of the server from which the video streams will be dowloaded

### Note
Because I have no appropriate server, I used that command to imitate it
```
ffmpeg -re -i <path_to_video> -c copy -f mpegts udp://<host>:<port>
```
