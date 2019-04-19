# 【nginx】CentOs7下安装nginx以及配置负载均衡 - Big Smile - CSDN博客
2018年08月12日 10:50:07[王啸tr1912](https://me.csdn.net/tr1912)阅读数：508
首先我们会去官网下载最新的nginx的tar.gz的包：[http://nginx.org/en/download.html](http://nginx.org/en/download.html)
我们在官网会看到有如下的三个部分的安装包可以提供下载：
![](https://img-blog.csdn.net/20180812094733299?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
       其中mainline version就是我们的主线版本，也就是刚刚开发上线完成的版本，可能存在bug或者未知的使用漏洞，所以不推荐安装。
       Stable version，就是稳定版的意思，生产线上推荐使用此版本的。
       Legacy versions，就是历史版本，如果有特殊需要的话可以在这里找到需要的旧版本。
下面还有source code版本这里不再细说，我们使用他发布好的这个版本当做例子来进行编译安装。
# 一、准备环境
        因为我用的是centos7系统，所以基本的c和c++的编译环境已经具备了，有使用低版本系统的同志自行安装：
`yum install gcc gcc-c++ -y`
由于我们在期间还需要进行初始化配置这一步，所以还需要安装pcre和zlib这两个包：
```
yum install -y pcre-devel
yum install -y zlib-devel
```
# **二、安装**
        安装过程非常简单我们需要把下载的tar.gz包上传到服务器上，我们默认上传到了/opt 目录，然后使用tar命令进行解压缩：
`tar -xvzf nginx-1.14.0.tar.gz`
![](https://img-blog.csdn.net/20180812100620261?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
解压完成是这个样子的。然后我们进入解压完成的目录：
`cd nginx-1.14.0`
然后执行配置的命令：
`./configure --prefix=/opt/nginx`
其中，prefix后面跟的路径是你想吧代码编译之后放置的位置，默认为/usr/local/nginx，这里我修改了在opt目录下
如果没有装准备过程中的两个插件的话会出现以下错误，导致配置写入失败：
![](https://img-blog.csdn.net/20180812101613772?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 这个是没有安装pcre的库
![](https://img-blog.csdn.net/2018081210165537?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这个是缺少zlib的库
安装完即可成功配置。
然后我们进行代码的编译，还是在解压缩之后的这个nginx目录：
`make && make install`
然后我们会在opt目录下发现新的文件夹：
![](https://img-blog.csdn.net/20180812101841508?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样我们就安装成功了，现在只要进入sbin目录启动，我们的nginx就可以启动了：
```
cd nginx/sbin
./nginx
```
# 三、配置反向代理
   有两个服务器，都配置了tomcat，分别是192.168.65.136:8080   和  192.168.65.135:8080     这两个地址，我们首先进入nginx配置文件：
`vi /opt/nginx/conf/nginx.conf`
然后再http的第一个server块上面写入以下代码：
```
upstream tomcat_pool  {
   #server tomcat地址:端口号 weight表示权值，权值越大，被分配的几率越大;
   server 192.168.65.136:8080 weight=1;
   server 192.168.65.135:8080 weight=1;
}
```
![](https://img-blog.csdn.net/20180812103006896?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样我们的负载均衡后台服务就配置好了，然后我们需要在server中配置一个location进行监听访问，并转发到我们配置的stream上：
```
location / {
   proxy_pass http://tomcat_pool;    #转向tomcat处理
   proxy_redirect     off;#是否跳转
   proxy_set_header   Host             $host; #请求要转发的host
   proxy_set_header   X-Real-IP        $remote_addr;#请求的远程地址    这些在浏览器的header都可看，不一一解释
   proxy_set_header   X-Forwarded-For  $proxy_add_x_forwarded_for;
   proxy_next_upstream error timeout invalid_header http_500 http_502 http_503 http_504;
   proxy_max_temp_file_size 0;
   proxy_connect_timeout      90; #连接前面的服务器超时时间
   proxy_send_timeout         90;#请求转发数据报文的超时时间
   proxy_read_timeout         90;#读取超时时间
   proxy_buffer_size          4k; # 缓冲区的大小
   proxy_buffers              4 32k; #
   proxy_busy_buffers_size    64k; # #proxy_buffers缓冲区，网页平均在32k以下的
   proxy_temp_file_write_size 64k; ##高负荷下缓冲大小（proxy_buffers*2）
}
```
然后输入英文 :wq就可以了.
这个时候我们去sbin目录下进行重新启动：
```
cd /opt/nginx/sbin
./nginx -s reload
```
就可以看到效果了。
# 四、nginx配置服务和开机自启
nginx的官网提供了这个配置服务的脚本了，在这个地址：
[https://www.nginx.com/resources/wiki/start/topics/examples/redhatnginxinit/](https://www.nginx.com/resources/wiki/start/topics/examples/redhatnginxinit/)
这里我直接粘过来：
```
#!/bin/sh
#
# nginx - this script starts and stops the nginx daemon
#
# chkconfig:   - 85 15
# description:  NGINX is an HTTP(S) server, HTTP(S) reverse \
#               proxy and IMAP/POP3 proxy server
# processname: nginx
# config:      /etc/nginx/nginx.conf
# config:      /etc/sysconfig/nginx
# pidfile:     /var/run/nginx.pid
# Source function library.
. /etc/rc.d/init.d/functions
# Source networking configuration.
. /etc/sysconfig/network
# Check that networking is up.
[ "$NETWORKING" = "no" ] && exit 0
nginx="/usr/sbin/nginx"
prog=$(basename $nginx)
NGINX_CONF_FILE="/etc/nginx/nginx.conf"
[ -f /etc/sysconfig/nginx ] && . /etc/sysconfig/nginx
lockfile=/var/lock/subsys/nginx
make_dirs() {
   # make required directories
   user=`$nginx -V 2>&1 | grep "configure arguments:.*--user=" | sed 's/[^*]*--user=\([^ ]*\).*/\1/g' -`
   if [ -n "$user" ]; then
      if [ -z "`grep $user /etc/passwd`" ]; then
         useradd -M -s /bin/nologin $user
      fi
      options=`$nginx -V 2>&1 | grep 'configure arguments:'`
      for opt in $options; do
          if [ `echo $opt | grep '.*-temp-path'` ]; then
              value=`echo $opt | cut -d "=" -f 2`
              if [ ! -d "$value" ]; then
                  # echo "creating" $value
                  mkdir -p $value && chown -R $user $value
              fi
          fi
       done
    fi
}
start() {
    [ -x $nginx ] || exit 5
    [ -f $NGINX_CONF_FILE ] || exit 6
    make_dirs
    echo -n $"Starting $prog: "
    daemon $nginx -c $NGINX_CONF_FILE
    retval=$?
    echo
    [ $retval -eq 0 ] && touch $lockfile
    return $retval
}
stop() {
    echo -n $"Stopping $prog: "
    killproc $prog -QUIT
    retval=$?
    echo
    [ $retval -eq 0 ] && rm -f $lockfile
    return $retval
}
restart() {
    configtest || return $?
    stop
    sleep 1
    start
}
reload() {
    configtest || return $?
    echo -n $"Reloading $prog: "
    killproc $nginx -HUP
    RETVAL=$?
    echo
}
force_reload() {
    restart
}
configtest() {
  $nginx -t -c $NGINX_CONF_FILE
}
rh_status() {
    status $prog
}
rh_status_q() {
    rh_status >/dev/null 2>&1
}
case "$1" in
    start)
        rh_status_q && exit 0
        $1
        ;;
    stop)
        rh_status_q || exit 0
        $1
        ;;
    restart|configtest)
        $1
        ;;
    reload)
        rh_status_q || exit 7
        $1
        ;;
    force-reload)
        force_reload
        ;;
    status)
        rh_status
        ;;
    condrestart|try-restart)
        rh_status_q || exit 0
            ;;
    *)
        echo $"Usage: $0 {start|stop|status|restart|condrestart|try-restart|reload|force-reload|configtest}"
        exit 2
esac
```
我们把这个保存到  **/etc/init.d/nginx**  文件中
然后需要修改文件中的：
- nginx=”/opt/nginx/sbin/nginx” 修改成nginx执行程序的路径。
- NGINX_CONF_FILE=”/opt/nginx/conf/nginx.conf” 修改成配置文件的路径。
然后需要把这个文件变为可执行文件：
`chmod 775 /etc/init.d/nginx`
这样这个文件就是可执行文件了，这个时候我们需要把这个文件加入chkconfig进行管理：
`chkconfig --add /etc/init.d/nginx`
这个时候我们就可以用service 命令进行nginx的操作了。
最后我们需要让chkconfig配置nginx为开机自动启动模式：
`chkconfig --level 3 nginx on `
这样就搞定了。
