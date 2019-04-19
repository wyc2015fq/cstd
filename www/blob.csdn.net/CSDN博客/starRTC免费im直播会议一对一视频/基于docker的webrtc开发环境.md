# 基于docker的webrtc开发环境 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年09月18日 16:22:56[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：983
在root目录下先放好自定义的constants.py文件再docker
 run
同时记得修改PUBLIC_IP
可以跨wifi通信
**docker**
 pull piasy/apprtc-server
docker run --name apprtc -p 8080:8080 -p 8089:8089 -p 3478:3478
-p 3478:3478/udp -p 3033:3033 --expose=59000-65000 -e PUBLIC_IP=47.88.63.197 -v /root:/apprtc_configs -it piasy/apprtc-server
其中;
-e 设置环境变量的，
-v, --volume=[host-src:]container-dest[:<options>]: Bind mount a volume.映射与共享用

附： -v选项
运行命令：docker run -it -v /home/xqh/myimage:/data ubuntu
 /bin/bash
其中的 -v 标记 在容器中设置了一个挂载点 /data（就是容器中的一个目录），并将主机上的 /home/xqh/myimage 目录中的内容关联到 /data下。
这样在容器中对/data目录下的操作，还是在主机上对/home/xqh/myimage的操作，都是完全实时同步的，因为这两个目录实际都是指向主机目录。
AppRTC 需要三个 Server：
- Room Server: 负责处理加入房间、获取配置等请求；
- Signal Server: 长连接服务器，用于聊天过程中实时下发信息；
- TURN/STUN Server: 打洞服务器，用于 NAT 打洞；
About port expose:
- 8080 is used for room server;
- 
8089 is used for signal server;
- 3033 is used for ICE server;
- 
3478 and 59000-65000 is used for TURN/STUN server;
**注意一定要开放 3478 udp 端口，否则 STUN Binding 请求进不去**；

AppRTC Android demo 中，会尝试从房间配置中读取 pc_config 域，以取得
 TURN/STUN Server 信息；如果没有获取到，就会向配置中的 ice_server_url 指向的服务器获取 TURN/STUN Server
 信息。但 apprtc 的配置说明中，让我们把 TURN/STUN Server 配置写在 TURN_SERVER_OVERRIDE中，这是不行的，因为客户端的逻辑并不会读取 TURN_SERVER_OVERRIDE 这个域，此外
 Android demo 读取 pc_config 的代码中，无法正确把 TURN Server 的用户名解析出来，所以我们不得不自己搭一个
 ICE Server，这会多一次网络请求
示例的constants.py:
ICE_SERVER_BASE_URL, ICE_SERVER_URL_TEMPLATE and WSS_INSTANCES
has been modified.
因此我们还需要第四个 Server：ICE Server。它用于下发 TURN/STUN Server 配置信息，代码如下
几点说明：
- 这里面有两个配置写死了：key = '4080218913'，用户名 ninefingers，它们都是在部署
 Coturn 时配置的；
- username/credential 并不是
 Coturn 创建用户的 username/password，而是按照上述逻辑计算出来的值，否则 Coturn 会报错 401 Unauthorized；
- 上面的 ICE Server 部署之后，Android demo 请求时会报 404，最终定位是因为 demo 用 HttpURLConnection 时设置了 connection.setDoOutput(true);，注释掉就好了，[详见这个
 commit](https://github.com/Piasy/AppRTC-Android/commit/88f18b8142392ec683c6df4db64d004632b60af7)；

附：运行日志
log file opened: /var/log/turn_15_2017-09-12.log
RFC 3489/5389/5766/5780/6062/6156 STUN/TURN Server
Version Coturn-4.5.0.6 'dan Eider'
Max number of open files/sockets allowed for this process: 65536
Due to the open files/sockets limitation,
max supported number of TURN Sessions possible is: 32500 (approximately)
==== Show him the instruments, Practical Frost: ====
0: TLS supported
0: DTLS supported
0: DTLS 1.2 supported
0: TURN/STUN ALPN supported
0: Third-party authorization (oAuth) supported
0: GCM (AEAD) supported
0: OpenSSL compile-time version: OpenSSL 1.0.2g 1 Mar 2016 (0x1000207f)
0: SQLite is not supported
0: Redis is not supported
0: PostgreSQL is not supported
0: MySQL is not supported
0: MongoDB is not supported
0: Default Net Engine version: 3 (UDP thread per CPU core)
=====================================================
0: 
Listener address to use: 0.0.0.0
0: Domain name:
0: Default realm: apprtc
0: 
Relay address to use: 0.0.0.0
0: pid file created: /var/run/turnserver.pid
0: IO method (main listener thread): epoll (with changelist)
0: WARNING: I cannot support STUN CHANGE_REQUEST functionality because only one IP address is provided
0: Wait for relay ports initialization...
0: relay 0.0.0.0 initialization...
0: relay 0.0.0.0 initialization done
0: Relay ports initialization done
0: IO method (general relay thread): epoll (with changelist)
0: 
turn server id=0 created
0: IPv4. SCTP listener opened on : 0.0.0.0:3478
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=1 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=2 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=3 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=4 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=5 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=6 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=7 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=8 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=9 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=10 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=11 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=12 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=13 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=14 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=15 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=16 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=17 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=18 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=19 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=20 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=21 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=22 created
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: IO method (general relay thread): epoll (with changelist)
0: turn server id=23 created
0: IPv4. UDP listener opened on: 0.0.0.0:3478
0: IPv4. TCP listener opened on : 0.0.0.0:3478
0: Total General servers: 24
0: IO method (auth thread): epoll (with changelist)
0: IO method (auth thread): epoll (with changelist)
0: IO method (auth thread): epoll (with changelist)
0: IO method (auth thread): epoll (with changelist)
0: IO method (auth thread): epoll (with changelist)
0: IO method (auth thread): epoll (with changelist)
0: IO method (auth thread): epoll (with changelist)
0: IO method (auth thread): epoll (with changelist)
0: IO method (auth thread): epoll (with changelist)
0: IO method (auth thread): epoll (with changelist)
0: IO method (auth thread): epoll (with changelist)
0: IO method (auth thread): epoll (with changelist)
0: IO method (admin thread): epoll (with changelist)
server started
参考：
[https://blog.piasy.com/2017/06/17/out-of-the-box-webrtc-dev-env/](https://blog.piasy.com/2017/06/17/out-of-the-box-webrtc-dev-env/)
[https://github.com/Piasy/WebRTC-Docker/tree/master/apprtc-server](https://github.com/Piasy/WebRTC-Docker/tree/master/apprtc-server)
