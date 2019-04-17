# Nginx-rtmp模块实现流媒体play、push、pull功能 - DoubleLi - 博客园






官方wiki:[https://github.com/arut/nginx-rtmp-module#readme](https://github.com/arut/nginx-rtmp-module#readme)

## Nginx rtmp 功能特点

1.   支持音视频直播

2.   支持flv/mp4视频格式，输入可以是文件或者HTTP流

3.   支持两种流的分发模式 pushand pull

4.   可以将直播流录制成flv文件

5.   H264/AAC编码

6.   支持在线转码 Onlinetranscoding with FFmpeg

7.   支持HLS (HTTP LiveStreaming)需要 libavformat (>= 8. 53.31.100) from ffmpeg (ffmpeg.org)

8.   HTTPcallbacks (publish/play/record/update etc)

9.   支持外部程序(exec)

10.  HTTPcontrol module for recording audio/video and dropping clients

11.  先进内存控制技术，可以在使用少量内存的情况下完成流畅的直播功能。

12 . 可以和以下协同工作。FMS server（Wirecast, FMS, Wowza,）Player（JWPlayer, FlowPlayer, StrobeMediaPlayback,）外部程序（ffmpeg,avconv,rtmpdump,flvstreamer ）

13. Statistics in XML/XSL in machine- & human- readable form

14.  支持跨平台 Linux/FreeBSD/MacOS

**实现环境：**


|rhel6.1|172.16.6.36|
|----|----|
|rhel6.1|172.16.6.39|

编译环境：安装gcc、openssl-devel、pcre-devel

网络yum源配置参考链接：[http://lyd0721.blog.51cto.com/4894702/1428467](http://lyd0721.blog.51cto.com/4894702/1428467)

下载并解压nginx-rtmp-module模块

编译安装nginx



```
```bash
[root@localhost ~]
```

```bash
# tar xf nginx-1.4.7.tar.gz
```



```bash
[root@localhost ~]
```

```bash
# cd nginx-1.4.7
```



```bash
[root@localhost ~]
```

```bash
# ./configure --prefix=/usr --sbin-path=/usr/sbin/nginx --conf-path=/etc/nginx/nginx.conf --error-log-path=/var/log/nginx/error.log --http-log-path=/var/log/nginx/access.log --pid-path=/var/run/nginx/nginx.pid  --lock-path=/var/lock/nginx.lock --user=nginx --group=nginx --with-http_ssl_module --with-http_flv_module --with-http_stub_status_module --with-http_gzip_static_module --http-client-body-temp-path=/var/tmp/nginx/client/ --http-proxy-temp-path=/var/tmp/nginx/proxy/ --http-fastcgi-temp-path=/var/tmp/nginx/fcgi/ --http-uwsgi-temp-path=/var/tmp/nginx/uwsgi --http-scgi-temp-path=/var/tmp/nginx/scgi --add-module=/root/nginx-rtmp-module-master
```



```bash
[root@localhost ~]
```

```bash
# make && make install
```
```



在/etc/init.d/目录下为nginx提供服务器脚本并赋予其执行权限



```
```bash
#!/bin/sh
```



```bash
#
```



```bash
# nginx - this script starts and stops the nginx daemon
```



```bash
#
```



```bash
# chkconfig:   - 85 15
```



```bash
# description:  Nginx is an HTTP(S) server, HTTP(S) reverse \
```



```bash
#               proxy and IMAP/POP3 proxy server
```



```bash
# processname: nginx
```



```bash
# config:      /etc/nginx/nginx.conf
```



```bash
# config:      /etc/sysconfig/nginx
```



```bash
# pidfile:     /var/run/nginx.pid
```



```bash
# Source function library.
```



```bash
.
```

```bash
/etc/rc
```

```bash
.d
```

```bash
/init
```

```bash
.d
```

```bash
/functions
```



```bash
# Source networking configuration.
```



```bash
.
```

```bash
/etc/sysconfig/network
```



```bash
# Check that networking is up.
```



```bash
[
```

```bash
"$NETWORKING"
```

```bash
=
```

```bash
"no"
```

```bash
] &&
```

```bash
exit
```

```bash
0
```



```bash
nginx=
```

```bash
"/usr/sbin/nginx"
```



```bash
prog=$(
```

```bash
basename
```

```bash
$nginx)
```



```bash
NGINX_CONF_FILE=
```

```bash
"/etc/nginx/nginx.conf"
```



```bash
[ -f
```

```bash
/etc/sysconfig/nginx
```

```bash
] && .
```

```bash
/etc/sysconfig/nginx
```



```bash
lockfile=
```

```bash
/var/lock/subsys/nginx
```



```bash
make_dirs() {
```



```bash
# make required directories
```



```bash
user=`nginx -V 2>&1 |
```

```bash
grep
```

```bash
"configure arguments:"
```

```bash
|
```

```bash
sed
```

```bash
's/[^*]*--user=\([^ ]*\).*/\1/g'
```

```bash
-`
```



```bash
options=`$nginx -V 2>&1 |
```

```bash
grep
```

```bash
'configure arguments:'
```

```bash
`
```



```bash
for
```

```bash
opt
```

```bash
in
```

```bash
$options;
```

```bash
do
```



```bash
if
```

```bash
[ `
```

```bash
echo
```

```bash
$opt |
```

```bash
grep
```

```bash
'.*-temp-path'
```

```bash
` ];
```

```bash
then
```



```bash
value=`
```

```bash
echo
```

```bash
$opt |
```

```bash
cut
```

```bash
-d
```

```bash
"="
```

```bash
-f 2`
```



```bash
if
```

```bash
[ ! -d
```

```bash
"$value"
```

```bash
];
```

```bash
then
```



```bash
# echo "creating" $value
```



```bash
mkdir
```

```bash
-p $value &&
```

```bash
chown
```

```bash
-R $user $value
```



```bash
fi
```



```bash
fi
```



```bash
done
```



```bash
}
```



```bash
start() {
```



```bash
[ -x $nginx ] ||
```

```bash
exit
```

```bash
5
```



```bash
[ -f $NGINX_CONF_FILE ] ||
```

```bash
exit
```

```bash
6
```



```bash
make_dirs
```



```bash
echo
```

```bash
-n $
```

```bash
"Starting $prog: "
```



```bash
daemon $nginx -c $NGINX_CONF_FILE
```



```bash
retval=$?
```



```bash
echo
```



```bash
[ $retval -
```

```bash
eq
```

```bash
0 ] &&
```

```bash
touch
```

```bash
$lockfile
```



```bash
return
```

```bash
$retval
```



```bash
}
```



```bash
stop() {
```



```bash
echo
```

```bash
-n $
```

```bash
"Stopping $prog: "
```



```bash
killproc $prog -QUIT
```



```bash
retval=$?
```



```bash
echo
```



```bash
[ $retval -
```

```bash
eq
```

```bash
0 ] &&
```

```bash
rm
```

```bash
-f $lockfile
```



```bash
return
```

```bash
$retval
```



```bash
}
```



```bash
restart() {
```



```bash
configtest ||
```

```bash
return
```

```bash
$?
```



```bash
stop
```



```bash
sleep
```

```bash
1
```



```bash
start
```



```bash
}
```



```bash
reload() {
```



```bash
configtest ||
```

```bash
return
```

```bash
$?
```



```bash
echo
```

```bash
-n $
```

```bash
"Reloading $prog: "
```



```bash
killproc $nginx -HUP
```



```bash
RETVAL=$?
```



```bash
echo
```



```bash
}
```



```bash
force_reload() {
```



```bash
restart
```



```bash
}
```



```bash
configtest() {
```



```bash
$nginx -t -c $NGINX_CONF_FILE
```



```bash
}
```



```bash
rh_status() {
```



```bash
status $prog
```



```bash
}
```



```bash
rh_status_q() {
```



```bash
rh_status >
```

```bash
/dev/null
```

```bash
2>&1
```



```bash
}
```



```bash
case
```

```bash
"$1"
```

```bash
in
```



```bash
start)
```



```bash
rh_status_q &&
```

```bash
exit
```

```bash
0
```



```bash
$1
```



```bash
;;
```



```bash
stop)
```



```bash
rh_status_q ||
```

```bash
exit
```

```bash
0
```



```bash
$1
```



```bash
;;
```



```bash
restart|configtest)
```



```bash
$1
```



```bash
;;
```



```bash
reload)
```



```bash
rh_status_q ||
```

```bash
exit
```

```bash
7
```



```bash
$1
```



```bash
;;
```



```bash
force-reload)
```



```bash
force_reload
```



```bash
;;
```



```bash
status)
```



```bash
rh_status
```



```bash
;;
```



```bash
condrestart|try-restart)
```



```bash
rh_status_q ||
```

```bash
exit
```

```bash
0
```



```bash
;;
```



```bash
*)
```



```bash
echo
```

```bash
$
```

```bash
"Usage: $0 {start|stop|status|restart|condrestart|try-restart|reload|force-reload|configtest}"
```



```bash
exit
```

```bash
2
```



```bash
esac
```
```





```
```bash
[root@localhost ~]
```

```bash
# chmod +x nginx
```
```



添加nginx至服务列表并设置其开机自动启动



```
```bash
[root@localhost ~]
```

```bash
# chkconfig --add nginx
```



```bash
[root@localhost ~]
```

```bash
# chkconfig nginx on
```
```



备份/etc/nginx配置文件，把nginx-rtmp-module-master/test/nginx.conf复制到/etc/nginx目录下



```
```bash
[root@localhost ~]
```

```bash
# cd /etc/nginx/
```



```bash
[root@localhost nginx]
```

```bash
# mv nginx.conf nginx.conf.bak
```



```bash
[root@localhost ~]
```

```bash
# cd nginx-rtmp-module-master/test/
```



```bash
[root@localhost
```

```bash
test
```

```bash
]
```

```bash
# mv nginx.conf /etc/nginx/
```
```



修改/etc/nginx/nginx.conf配置文件

![wKiom1OnzXOyRIX9AAHj_balFUg805.jpg](http://s3.51cto.com/wyfs02/M00/30/6D/wKiom1OnzXOyRIX9AAHj_balFUg805.jpg)![wKioL1OnxSfjPPJiAAHWz-B29bk638.jpg](http://s3.51cto.com/wyfs02/M01/30/6C/wKioL1OnxSfjPPJiAAHWz-B29bk638.jpg)



172.16.6.39上面的环境与172.16.6.36相同，只不过要在172.16.6.39上实现pull的功能

![wKioL1OnzuajkjZWAAJKzb7F2i4733.jpg](http://s3.51cto.com/wyfs02/M00/30/6D/wKioL1OnzuajkjZWAAJKzb7F2i4733.jpg)



下载一个.flv结尾的视频文件，使用ffmpeg模拟视频直播源进行测试play、push及pull功能是否实现

![wKioL1Onxu3T8Nh2AACcSeSL_cU743.jpg](http://s3.51cto.com/wyfs02/M01/30/6C/wKioL1Onxu3T8Nh2AACcSeSL_cU743.jpg)

使用ffmpeg模拟直播效果，ffmpeg下载链接[http://www.ffmpeg.org/download.html](http://www.ffmpeg.org/download.html)

进入ffmpeg-2.2.1-64bit-static目录下，执行如下命令



```
```bash
[root@localhost ffmpeg-2.2.1-64bit-static]
```

```bash
# ./ffmpeg -re -i ~/2014.flv -f flv rtmp://172.16.6.36/myapp/test1
```
```



然后登录此链接验证能够播放：[http://www.wowza.com/resources/3.5.0/examples/LiveVideoStreaming/FlashRTMPPlayer/player.html](http://www.wowza.com/resources/3.5.0/examples/LiveVideoStreaming/FlashRTMPPlayer/player.html)

![wKiom1OnyHeBLIl1AAIBzWZw8Iw932.jpg](http://s3.51cto.com/wyfs02/M02/30/6D/wKiom1OnyHeBLIl1AAIBzWZw8Iw932.jpg)

验证pull功能是否实现

![wKioL1OnyKmDVW1xAAEq0cnx9ro718.jpg](http://s3.51cto.com/wyfs02/M02/30/6C/wKioL1OnyKmDVW1xAAEq0cnx9ro718.jpg)

验证push功能

使用ffmpeg推流的时候，应该推至/mypush中，/mypush有流之后才能push至172.16.6.39的/myapp中，所以执行如下命令



```
```bash
[root@localhost ffmpeg-2.2.1-64bit-static]
```

```bash
# ./ffmpeg -re -i ~/2014.flv -f flv rtmp://172.16.6.36/mypush/test1
```
```



![wKiom1Onyc3wS1DQAAHQTplCA2I053.jpg](http://s3.51cto.com/wyfs02/M01/30/6D/wKiom1Onyc3wS1DQAAHQTplCA2I053.jpg)











