# ubuntu rebuild nginx to add stub_status module - Arthur的随笔 - CSDN博客
2013年04月16日 11:07:27[largetalk](https://me.csdn.net/largetalk)阅读数：4687
在Q群里看到人说给nginx的配置文件加上如下行：
```
location /NginxStatus {
          stub_status on;
        }
```
可以看到一些nginx运行的信息，可是我加上后重新reload却出现了错误
```
$ sudo /etc/init.d/nginx reload
Reloading nginx configuration: nginx: [emerg] unknown directive "stub_status" in /etc/nginx/sites-enabled/uts:26
nginx: configuration file /etc/nginx/nginx.conf test failed
```
搜索发现是需要在编译nginx时加上 --with-http_stub_status_module 的配置，网上大多的介绍都是直接下源码编译安装，可是我的系统是ubuntu，而已已经安装nginx，不想将原来的配置丢掉，所以，需要如下操作
```
cd /usr/src
apt-get source nginx
cd nginx-xxx
whereis nginx
/usr/sbin/nginx -V
```
然后copy configure arguments
```
./configure + 粘帖刚才copy的配置  + --with-http_stub_status_module
make 
make install 
make upgrade
```
ok, 重启nginx发现不会报错了。
打开 /NginxStatus
```
Active connections: 1 
server accepts handled requests
 5 5 9 
Reading: 0 Writing: 1 Waiting: 0
```
Active connections:
 对后端发起的活动连接数.
Server accepts handled requests:
 Nginx总共处理了5个连接,成功创建5次握手(证明中间没有失败的),总共处理了9个请求.
Reading:
 Nginx 读取到客户端的Header信息数.
Writing:
 Nginx 返回给客户端的Header信息数.
Waiting:
 开启keep-alive的情况下,这个值等于 active – (reading + writing),意思就是Nginx已经处理完成,正在等候下一次请求指令的驻留连接.
所以,在访问效率高,请求很快被处理完毕的情况下,Waiting数比较多是正常的.如果reading +writing数较多,则说明并发访问量非常大,正在处理过程中.
