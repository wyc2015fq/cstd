# 环境安装系列 - Linux下 Nginx的安装和简单管理 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月18日 17:23:33[initphp](https://me.csdn.net/initphp)阅读数：1272








Nginx安装前，您需要准备以下包：

> 
1.Nginx安装包：nginx-1.4.7.tar.gz

自己安装的软件一般都会放在 /usr/local/目录下，扩展库都会放在 /usr/local/lib/目录

[下载包](http://initphp.qiniudn.com/Nginx%E5%AE%89%E8%A3%85%E5%8C%85.zip)


- 
您的Linux是否已经安装了gcc等，还没有的话，先用yum安装一下，编译啥的都需要靠这些：

`yum install gcc gcc-c++ gcc-g77 autoconf automake zlib* fiex* libxml* ncurses-devel libmcrypt* libtool-ltdl-devel* libevent pcre pcre-devel`- 安装Nginx包，安装路径/usr/local/nginx-1.4.7

```
sudo  tar zxvf nginx-1.4.7.tar.gz
cd  nginx-1.4.7
sudo ./configure --user=nfsnobody --group=nfsnobody --prefix=/usr/local/nginx-1.4.7 --with-http_stub_status_module --without-http-cache --with-http_ssl_module
sudo  make
sudo  make install
```
- 创建日志目录，www目录：

```
sudo  mkdir -p /usr/local/nginx-1.4.7/logs #日志文件夹
sudo  chmod -R 777 /usr/local/nginx-1.4.7/logs 
sudo  chown -R nfsnobody:nfsnobody /usr/local/nginx-1.4.7/logs 
sudo  mkdir /home/wwwroot   #www目录
sudo  chown -R admin:admin /home/wwwroot #常用的用户组比较方便一些
```
- 拷贝我们已经准备好的配置文件,没有就不需要拷贝

```
sudo  rm -f /usr/local/nginx-1.4.7/conf/nginx.conf 
sudo  cp  nginx.conf  /usr/local/nginx-1.4.7/conf/nginx.conf #配置文件
sudo  cp  fcgi.conf  /usr/local/nginx-1.4.7/conf/fcgi.conf
```
- 启动Nginx
`sudo  /usr/local/nginx-1.4.7/sbin/nginx`- 查看Nginx是否启动，查看Nginx进程和用ip在浏览器中访问一下：
`ps –ef | grep nginx`- 如果已经开启防火墙，那么在防火墙中添加80端口：

```
iptables -I INPUT -p tcp --dport 80 -j ACCEPT 
service iptables save
service iptables restart
```
- Nginx管理：

```
#默认启动方式
sudo  /usr/local/nginx-1.4.7/sbin/nginx
sudo  /usr/local/nginx-1.4.7/sbin/nginx -s stop
sudo  /usr/local/nginx-1.4.7sbin/nginx -s reload

#带上配置文件：
sudo /usr/local/nginx-1.4.7/sbin/nginx -c /usr/local/web/nginx-1.4.7/conf/nginx.conf
 

#带测试配置文件启动方法
sudo  /usr/local/nginx-1.4.7/sbin/nginx  -t
#从容停止
sudo kill -QUIT `cat /usr/local/nginx-1.4.7/nginx.pid`
#快速停止
sudo kill -INT `cat /usr/local/nginx-1.4.7/nginx.pid`
#平滑重启
sudo kill -HUP `cat /usr/local/nginx-1.4.7/nginx.pid`
```










