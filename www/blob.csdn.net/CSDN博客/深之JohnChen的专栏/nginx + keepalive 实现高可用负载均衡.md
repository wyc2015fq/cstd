# nginx + keepalive 实现高可用负载均衡 - 深之JohnChen的专栏 - CSDN博客

2013年12月31日 21:55:24[byxdaz](https://me.csdn.net/byxdaz)阅读数：4962



![nginx + keepalive - zhuzhu - 五事九思  （大连Linux主机维护）](http://img0.ph.126.net/5Ud1mIaLmmZ-DagYjQrEAg==/1170091478203277930.jpg)

一、拓扑环境：

主nginx负载均衡器 192.168.166.203 

辅nginx负载均衡器 192.168.166.177

VIP地址 192.168.166.178

[http://www.keepalived.org/download.html](http://blog.chinaunix.net/link.php?url=http://www.keepalived.org%2Fdownload.html)

tar -zxvf keepalived-1.2.1.tar.gz

cd keepalived-1.2.1

./configure --prefix=/usr/local/keepalived --sysconf=/etc

make && make install

cp /usr/local/keepalived/etc/rc.d/init.d/keepalived /etc/rc.d/init.d/

cp /usr/local/keepalived/etc/sysconfig/keepalived /etc/sysconfig/

cp /usr/local/keepalived/sbin/keepalived /bin/

chkconfig --add keepalived

chmod 755 /etc/init.d/keepalived

chkconfig keepalived on

service keepalived start

二．修改配置文件为以下内容:　[master slave]
**MASTER**vim /etc/keepalived/keepalived.conf

! Configuration File for keepalived

global_defs {

notification_email {
[xxx@xxx.com](mailto:xxx@xxx.com)

}

notification_email_from [xxx@xxx.com](mailto:xxx@xxx.com)

smtp_server xxx.smtp.com

smtp_connect_timeout 30

router_id LVS_DEVEL

}

vrrp_script Monitor_Nginx {

script "/root/monitor_nginx.sh"

interval 2

weight 2

}

vrrp_instance VI_1 {

state MASTER #(主机为MASTER，备用机为BACKUP)

interface eth0 #(HA监测网络接口)

virtual_router_id 61 #(主、备机的virtual_router_id必须相同)

#mcast_src_ip 192.168.166.203 #(多播的源IP，设置为本机外网IP，与VIP同一网卡)此项可不设置

priority 90 #(主、备机取不同的优先级，主机值较大，备份机值较小,值越大优先级越高)

advert_int 1 #(VRRP Multicast广播周期秒数)

authentication {

auth_type PASS #(VRRP认证方式)

auth_pass 1234 #(密码)

}

track_script {

Monitor_Nginx #(调用nginx进程检测脚本)

}

virtual_ipaddress {

192.168.166.178 #(VRRP HA虚拟地址)

}

}

###BACKUP方面只需要修改state为BACKUP , priority比MASTER稍低就OK了

三：注：monitor_nginx.sh为监控nginx进程的脚本,内容如下

#!/bin/bash

if [ "$(ps -ef | grep "nginx: master process"| grep -v grep )" == "" ]

then

/usr/local/nginx/sbin/nginx

sleep 5

if [ "$(ps -ef | grep "nginx: master process"| grep -v grep )" == "" ]

then

killall keepalived

fi

fi

#chmod +x monitor_nginx.sh

**四.启动keepalived**service keepalived start

或

/usr/local/keepalived/sbin/keepalived -D -f /etc/keepalived/keepalived.conf

五．测试步骤

1． 访问VIP看是否能够正常访问后端的web

2． 停止其中一个web看是否能将访问转到另一台上

3． 停止两台nginx上任何一个nginx进程看监控进程脚本是否会自动启动nginx

4． 停止任何一台nginx上的keepalived进程看另一台是否接管vip

track_interface的意思是将Linux中你想监控的网络接口卡监控起来，当其中的一块出现故障是keepalived都将视为路由器出现故障。

这里请注意： virtual_router_id 61，同一组master/backup中，也就是如果你设置了多个VRRP，同一组这个ID必须相同，不然启动的时候主从都会把VIP给启动了

keepalived 通过发送和接收组播包中的同一个virtual_router_id 的中的成员的存活，来确定对方的不可用，一旦检测到对方的不可用，即会切换它的备份角色为主。

即：当真实机192.168.166.177上的keepalived 检测到 真实机192.168.166.203上的keepalived 不可用时，177上将使用vip:192.168.166.178对外服务并由backup角色转变为master



keepalived+nginx实现 HA与负载均衡中keepalived负责实现HA功能，控制VIP(虚拟网络地址)转移功能，nginx实现对realserver七层负载均衡功能，nginx提供代理服务和cache功能，DNS服务可以负责前段VIP的负载均衡。

![nginx + keepalive - zhuzhu - 五事九思  （大连Linux主机维护）](http://blogold.chinaunix.net/photo/106233_110413101711.jpg)

keepalived的配置

192.168.200.1 keepalived配置

! Configuration File for keepalived

global_defs {

notification_email {
[xxx@xxx.com](mailto:xxx@xxx.com)

}

notification_email_from [xxx@xxx.com](mailto:xxx@xxx.com)

smtp_server 127.0.0.1

smtp_connect_timeout 30

router_id LVS_TEST_YDQ

}

vrrp_instance VI_1 {

state MASTER

interface eth0

virtual_router_id 61

priority 150

advert_int 1

authentication {

auth_type PASS

auth_pass 1111

}

virtual_ipaddress {

192.168.200.101/24 dev eth0 label eth0:0

}

}

vrrp_instance VI_2 {

state BACKUP

interface eth0

virtual_router_id 62

priority 100

advert_int 1

authentication {

auth_type PASS

auth_pass 2222

}

virtual_ipaddress {

192.168.200.102/24 dev eth0 label eth0:1

}

}

vrrp_instance VI_3 {

state BACKUP

interface eth0

virtual_router_id 63

priority 50

advert_int 1

authentication {

auth_type PASS

auth_pass 3333

}

virtual_ipaddress {

192.168.200.103/24 dev eth0 label eth0:1

}

}

nginx配置 ：

user nobody nobody;                                                          

worker_processes 1;                                                           

#error_log logs/error.log;                                                    

#error_log logs/error.log notice;                                             

error_log logs/error.log debug;                                               

pid logs/nginx.pid;                                                           

events {                                                                      

use epoll;                                                                    

worker_connections 1024;                                                      

}                                                                             

http {                                                                        

include mime.types;                                                           

default_type application/octet-stream;                                        

log_format main '$remote_addr - $remote_user [$time_local] "$request" '       

'$status $body_bytes_sent "$http_referer" '                                   

'"$http_user_agent" "$http_x_forwarded_for"';                                 

access_log logs/access.log main;                                              

sendfile on;                                                                  

#tcp_nopush on;                                                               

#keepalive_timeout 0;                                                         

keepalive_timeout 65;                                                         

gzip on;                                                                      

gzip_min_length 1k;                                                           

gzip_buffers 6 16k;                                                           

gzip_http_version 1.1;                                                        

gzip_comp_level 2;                                                            

gzip_types text/plain application/x-javascript text/css application/xml;      

gzip_vary on;                                                                 

# location / {                                                                

# root html;                                                                  

# index index.html index.htm;                                                 

# }                                                                           

error_page 404 /404.html;                                                     

upstream [www.test.com](http://www.test.com) {                                                      

server 192.168.11.37:80;                                                      

server 192.168.11.38:80;                                                      

}                                                                             

server                                                                        

{                                                                             

listen 80;                                                                    

server_name [www.test.com](http://www.test.com) 192.168.166.178;                                    

index index.htm index.html;                                                   

#root /web/wwwroot;                                                           

location / {                                                                  

proxy_pass [http://www.test.com](http://www.test.com);                                              

proxy_next_upstream http_500 http_502 http_503 error timeout invalid_header;  

proxy_set_header Host $host;                                                  

proxy_set_header X-Real-IP $remote_addr;                                      

proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;                  

include proxy.conf;                                                           

}                                                                             

log_format blog_test_com '$remote_addr - $remote_user [$time_local] $request '

'"$status" $body_bytes_sent "$http_referer" '                                 

'"$http_user_agent" "$http_x_forwarded_for"';                                 

access_log logs/blog_test_com;                                                

}                                                                             

}                                                                             

