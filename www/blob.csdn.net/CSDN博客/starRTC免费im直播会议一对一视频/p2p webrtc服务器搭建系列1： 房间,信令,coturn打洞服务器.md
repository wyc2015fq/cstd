# p2p webrtc服务器搭建系列1： 房间,信令,coturn打洞服务器 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月03日 15:41:44[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：5244
中继（relay）
在RTCPeeConnection中，使用ICE框架来保证RTCPeerConnection能实现NAT穿越
ICE，全名叫交互式连接建立（Interactive Connectivity Establishment）,一种综合性的NAT穿越技术，它是一种框架，可以整合各种NAT穿越技术如STUN(Simple
 Traversal of UDP over NATs,NAT 的UDP简单穿越); 、TURN（Traversal Using Relay NAT 中继NAT实现的穿透）。ICE会先使用STUN，尝试建立一个基于UDP的连接，如果失败了，就会去TCP（先尝试HTTP，然后尝试HTTPS），如果依旧失败ICE就会使用一个中继的TURN服务器。
STUN协议可以很好的解决一般家用(NAT)路由器环境的打洞问题,但是对于大部分的企业的网络环境就不是很好了.这时需要一个新的解决方案:TURN，  TURN是一个Client-Server协议
ICE协议就是综合前面2种协议的综合性NAT穿越解决方案.
DataChannel是建立在PeerConnection上的，不能单独使用
尽管WebRTC允许点对点的通信，但它依然需要服务器：
* 信令传输：建立点对点的连接需要传输一些媒体和网络相关的元数据信息，需要通过服务器
* NAT和防火墙穿透：我们需要通过ICE框架来建立点与点之间的网络路径。可以使用STUN服务器（确定双方的可公开访问你的IP地址和端口）以及TURN服务器（如果直接连接失败，就必须数据中继了）

docker run -it --name apprtc -d -p 8080:8080 -p 8443:8443 -p 8081:80 -p 443:443 ubuntu /bin/bash
工具：
apt-get update
apt-get -y install autoconf automake build-essential libtool wget git vim unzip
Google_App_Engine[https://cloud.google.com/appengine/downloads#Google_App_Engine_SDK_for_Python](https://cloud.google.com/appengine/downloads#Google_App_Engine_SDK_for_Python)
解压并设置环境变量即可：
wget
[https://storage.googleapis.com/appengine-sdks/featured/google_appengine_1.9.55.zip](https://storage.googleapis.com/appengine-sdks/featured/google_appengine_1.9.55.zip)
设置环境变量
export PATH=$PATH:/root/google_appengine/
[Node.js](https://nodejs.org/)
Node.js 就是运行在服务端的 JavaScript。采用C++语言编写而成，是一个Javascript的运行环境。
apt-get
 install nodejs nodejs-legacy 
npm python-webtest 
-y
npm install -g npm
在安装 Grunt 前，请确保当前环境中所安装的 [npm](https://npmjs.org/) 已经是最新版本，
执行 npm
 update -g npm指令进行升级，查看版本
 npm -v
安装npm install -g grunt-cli
java -version
ImportError: No module named requests
pip install requests
 if you have pip installed
[Grunt](http://gruntjs.com/).
基于Node.js的项目构建工具,可以自动运行你所设定的任务
JavaScript 世界的构建工具
Grunt和 Grunt 插件是通过 [npm](https://npmjs.org/) 安装并管理的，npm是 [Node.js](http://nodejs.org/) 的包管理器。
房间服务器：
项目源码地址: [https://github.com/webrtc/apprtc](https://github.com/webrtc/apprtc)
git clone [https://github.com/webrtc/apprtc](https://github.com/webrtc/apprtc)
cd apprtc
修改配置后编译：
配置apprtc/src/app_engine/constants
TURN_BASE_URL
//turn打洞服务器配置
TURN_URL_TEMPLATE
CEOD_KEY = 和coturn turnserver.conf  static-auth-secret一致
    WSS_INSTANCES = [{
WSS_INSTANCE_HOST_KEY: '192.168.214.129:8089',
                   WSS_INSTANCE_NAME_KEY: 'wsserver-std',
                   WSS_INSTANCE_ZONE_KEY: 'us-central1-a'
                  }, {
WSS_INSTANCE_HOST_KEY: '192.168.214.129:8089',
                   WSS_INSTANCE_NAME_KEY: 'wsserver-std-2',
                   WSS_INSTANCE_ZONE_KEY: 'us-central1-f'
            }]
ICE_SERVER_BASE_URL
执行npm
 install命令安装项目依赖的库，解压压缩包到当前项目的node_modules目录。
再执行下面命令编译出apprtc这个GAE
 app.
grunt build
编译之后会多出out目录

dev_appserver.py --host=0.0.0.0 /root/apprtc/out/app_engine/
[http://tmp.starconnect.cn:8080/](http://tmp.starconnect.cn:8080/)
错误：google appengine BindError: Unable to bind 8080
重启
2,信令服务器
用GO语言编写的基于websocket的信令服务器:Collider.
1，安装go，参见我的安装文档
使用go version查看go环境是否配置正确
2，
mkdir -p /root/goWorkspace/src
export GOPATH=/root/goWorkspace/
存放Collider的go代码程序。
cp -rf `pwd`/apprtc/src/collider/* $GOPATH/src
编辑collidermain/main.go,修改设置自己的房间服务器URL:
[http://192.168.1.103:8080/](http://192.168.1.103:8080/)
cd goWorkspace/src
- install dependencies
go get collidermain
- Install collidermain
go install collidermain
成功编译后会在gopath目录下生成bin和pkg目录，执行文件在bin下
Testing
go test collider
编辑$GOPATH/src/collider/collider.go,设置信令服务器所需要用的HTTPS的证书文件
/cert/cert.pem and /cert/key.pem作用
**运行**
$GOPATH/bin/collidermain -port=8089 -tls=true
**可以设置下path= /root/goWorkspace/bin**
打洞服务器STUN/TURN/ICE服务器的搭建：
选择有更丰富功能的coTurn作为我们的NAT穿越打洞服务器,该项目是一个C/C++语言的开源项目
主要是配置cert和pkey
turnserver.conf中的cert和pkey配置的自签名证书用Openssl命令生成:
openssl req -x509 -newkey rsa:2048 -keyout
/etc/turn_server_pkey.pem -out /etc/turn_server_cert.pem
-days 99999 -nodes
pkey应该是private key的缩写吧
安装libevent:
Before installing and configuring the TURN server, the latest stable libevent library should be downloaded, built and installed:
wget https://github.com/downloads/libevent/libevent/libevent-2.0.21-stable.tar.gz
As root, build and install the library with the usual:
tar xvfz libevent-2.0.21-stable.tar.gz cd libevent-2.0.21-stable ./configure make && make install

Download the TURN server from [https://github.com/coturn/coturn/wiki/Downloads](https://github.com/coturn/coturn/wiki/Downloads).
 Build and install with:
apt-get install libssl-dev
apt-get install libsqlite3 (or
sqlite3)：
apt-get install libsqlite3-dev
(or sqlite3-dev)
apt-get install
sqlite3 libsqlite3-dev
apt-get install libevent-dev
wget
[http://turnserver.open-sys.org/downloads/v4.5.0.6/turnserver-4.5.0.6.tar.gz](http://turnserver.open-sys.org/downloads/v4.5.0.6/turnserver-4.5.0.6.tar.gz)
tar xvfz turnserver-<...>.tar.gz ./configure make && make install
Check the INSTALL and README （有多个README文件）files with the server. They contain lots of useful information.

By default, coturn uses
SQLite
database for users and settings. That database is automatically created (empty) when the process turnserver starts the first time.
port
3478
(STUN)  should be opened for TCP and UDP.
3478、3479:tcp&udp - coTurn 的监听端口。
59000~65000:tcp&udp - coTurn 的服务端口。
min-port=59000
max-port=65000
- 
Change WSS_INSTANCE_HOST_KEY to the hostname and port 
Collider is listening too, e.g. localhost:8089 or otherHost:443.
mv /usr/local/share/examples/turnserver/etc/turnserver.conf /etc/turnserver.conf
turnserver -L <public_ip_address> -a -f -r <realm-name>
Or, start it as a daemon with:
$
turnserver
-L <public_ip_address>
-o -a -f -r <realm-name>
-L, --listening-ip Listener IP address of relay server.
Multiple listeners can be specified, for example:
-L ip1 -L ip2 -L ip3
If no IP(s) specified, then all IPv4 and
IPv6 system IPs will be used for listening.
The same ip(s) can be used as both listening and relay ip(s).
-a, --lt-cred-mech Use long-term credentials mechanism (this one you need for WebRTC usage).
-f, --fingerprint Use fingerprints in the TURN messages. If an incoming request
contains a fingerprint, then TURN server will always add
fingerprints to the messages in this session, regardless of the
per-server setting.
-r, --realm The default realm to be used for the users when no explicit
origin/realm relationship was found in the database, or if the TURN
server is not using any database (just the commands-line settings
and the userdb file). Must be used with long-term credentials
mechanism or with TURN REST API.
-o, --daemon Run server as daemon.

参考：
[https://github.com/coturn/coturn/wiki/CoturnConfig](https://github.com/coturn/coturn/wiki/CoturnConfig)
[http://www.jianshu.com/p/3a43233b9c39](http://www.jianshu.com/p/3a43233b9c39)
[http://blog.csdn.net/u010657219/article/details/54613298](http://blog.csdn.net/u010657219/article/details/54613298)
user=jalon:0xa9f776c6470bab44567b2584863e7345 是通过
# $ turnadmin -k -u jalon -r north.gov -p jalon#
 Output: 0xa9f776c6470bab44567b2584863e7345 生成的
参考：
旧[http://io.diveinedu.com/2015/02/05/第六章-WebRTC服务器搭建.html](http://io.diveinedu.com/2015/02/05/%E7%AC%AC%E5%85%AD%E7%AB%A0-WebRTC%E6%9C%8D%E5%8A%A1%E5%99%A8%E6%90%AD%E5%BB%BA.html)
新：
[http://io.diveinedu.com/2016/03/18](http://io.diveinedu.com/2016/03/18)/WebRTC之AppRTC与信令服务器构建.html
[http://blog.csdn.net/s569646547/article/details/50780682](http://blog.csdn.net/s569646547/article/details/50780682)
 原创！
需要自己实现coTurn连接信息接口，主要返回用户名、密码和turn配置信息，通常叫做TURN REST API,不实现这个接口的话AppRTCDemo连不上服务器，浏览器访问的话可以正常访问。
 TURN REST API [标准参考文档](http://tools.ietf.org/html/draft-uberti-behave-turn-rest-00)
返回json结果示例：
       {"username":"1456904882:1231244","password":"jAph7EHMLuPJuxLLC1uRiI3kvq4=","ttl":86400,"uris":["turn:192.168.214.129:3478?transport=udp","turn:192.168.214.129:3478?transport=tcp","turn:192.168.214.129:3479?transport=udp","turn:192.168.214.129:3479?transport=tcp"]}

[https://segmentfault.com/a/1190000000733779](https://segmentfault.com/a/1190000000733779)
