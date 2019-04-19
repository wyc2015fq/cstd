# 搭建Keepalived + Nginx + Tomcat的高可用负载均衡架构 - 文章 - 伯乐在线
原文出处： [FEINIK](https://my.oschina.net/feinik/blog/1590941)
# 1 概述
初期的互联网企业由于业务量较小，所以一般单机部署，实现单点访问即可满足业务的需求，这也是最简单的部署方式，但是随着业务的不断扩大，系统的访问量逐渐的上升，单机部署的模式已无法承载现有的业务量，需要进行服务集群化部署，本文主要介绍服务端Tomcat多实例部署，以及如何保证web服务的高可用方案。
- Nginx 是一个高性能的 HTTP反向代理服务器
- Keepalived 是一个基于VRRP协议来实现的LVS服务高可用方案，可以利用其来避免服务的单点故障
- Tomcat 是一个免费的开放源代码的Web 应用服务器，属于轻量级应用服务器。
# 2 Nginx的高可用负载均衡架构
如下图：为典型的Tomcat服务多实例部署的架构图
![](http://wx1.sinaimg.cn/mw690/63918611gy1fml0ntgngoj20f70evmxu.jpg)
（1）用户通过域名请求到DNS，由DNS解析域名后返回对应的IP地址，该IP及为Keepalived映射服务器的虚拟IP
（2）通过该虚拟IP访问到对应的负载均衡器（Nginx），这里Nginx部署两个，然后通过Keepalived来保证NG的高可用，正常情况下由Keepalived-M将虚拟IP映射转发至Nginx-M，如果Nginx-M出现故障，此时Keepalived会切换至Keepalived-S开始工作，从而保证了NG的单点故障问题。
（3）通过Nginx负载均衡器，将请求路由到对应的Tomcat服务。
# 3 搭建Keepalived + Nginx + Tomcat的高可用负载均衡架构
## 3.1 需要准备的软件
（1）apache-tomcat-8.5.16.tar.gz
（2）nginx-1.12.2.tar.gz
（3）keepalived-1.3.9.tar.gz
## 3.2 服务器准备
两台服务器如：192.168.10.11，192.168.10.12
## 3.3 安装需要的依赖包


```
$ yum -y install gcc gcc-c++ automake pcre pcre-devel zlib zlib-devel open openssl-devel
```
## 3.4 安装
### 3.4.1 安装Tomcat
（1）分别在两台服务器中安装Tomcat，解压apache-tomcat-8.5.16.tar.gz及可完成安装。
### 3.4.2 安装Nginx
（1）解压安装包：tar -zxvf nginx-1.12.2.tar.gz
（2）进入到nginx-1.12.2目录：cd nginx-1.12.2
（3）编译：
a) ./configure –with-http_stub_status_module –with-http_ssl_module  –prefix=/usr/local/nginx
b) sudo make && sudo make install
### 3.4.3 安装Keepalived
（1）解压安装包：tar -zxvf keepalived-1.3.9.tar.gz
（2）进入到keepalived-1.3.9目录：cd keepalived-1.3.9
（3）执行编译：
a）./configure –prefix=/usr/local/keepalived –sysconf=/etc
b）sudo make && sudo make install
## 3.5 配置
### 3.5.1 分别配置两台服务器的Nginx
（1）分别修改两台服务器nginx配置文件，vi /usr/local/nginx/conf/nginx.conf
（2）内容如下：


```
#nginx进程数
worker_processes  1;
#单个进程最大连接数
events {
    worker_connections  1024;
}
#http服务器配置
http {
    include       mime.types;
    default_type  application/octet-stream;
    sendfile        on;
	#长连接超时时间，单位是秒
    keepalive_timeout  65;
	#upstream负载均衡配置，配置路由到tomcat的服务地址以及权重
    upstream localhost{
       server 192.168.10.11:8080 weight=2;
       server 192.168.10.12:8080 weight=2;
    }
	
	#虚拟主机的配置
    server {
	    #监听端口
        listen       80;
		 #域名可以有多个，用空格隔开
        server_name  localhost;
        location / {
            root   html;
            index  index.html index.htm;
			#nginx跟后端服务器连接超时时间(代理连接超时)
            proxy_connect_timeout 3;
			#后端服务器数据回传时间(代理发送超时)
            proxy_send_timeout 30;
			#连接成功后，后端服务器响应时间(代理接收超时)
            proxy_read_timeout 30;
            proxy_pass http://localhost;
        }
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
    }
}
```
### 3.5.2 主Keepalived配置
（1）修改11服务器的keepalived配置文件，vi /etc/keepalived/keepalived.conf
（2）内容如下：


```
! Configuration File for keepalived
#全局配置
global_defs {
   #keepalived切换的时候，发消息到指定的email，可配置多个email
   notification_email {
     feinik1@foxmail.com
	 feinik2@foxmail.com
   }
   #通知邮件从哪个地址发出
   notification_email_from feinik@foxmail.com
   #通知邮件的smtp地址
   smtp_server smtp.exmail.qq.com
   #连接smtp服务器的超时时间，单位秒
   smtp_connect_timeout 30
   #Keepalived的机器标识，一个网络内保持唯一
   router_id nginx-master
}
#执行脚本配置
vrrp_script chk_nginx {
	#脚本所在路径
    script "/home/project/keepalived/check_nginx.sh"
	#脚本执行间隔时间，秒
    interval 2
	#优先级
    weight 2
}
#keepalived实例配置
vrrp_instance VI_1 {
	#指定实例的初始状态，MASTER或BACKUP两种状态，并且需要大写
    state MASTER
	#实例绑定的网卡
    interface ens33
	#虚拟路由标识，是一个数字，整个VRRP内唯一，如果keepalived配置了主备，需要相同
    virtual_router_id 51
	#优先级，数值愈大，优先级越高
    priority 100
	#MASTER与BACKUP之间同步检查的时间间隔，单位为秒
    advert_int 1
	#通信验证
    authentication {
        auth_type PASS
        auth_pass feinik
    }
	#追踪外围脚本
    track_script {
	    #这里配置vrrp_script的名称
        chk_nginx
    }
	#虚拟ip配置，可配置多个
    virtual_ipaddress {
        192.168.10.200
    }
}
```
### 3.5.3 备Keepalived配置
（1）修改12服务器的keepalived配置文件，vi /etc/keepalived/keepalived.conf
（2）内容如下：


```
! Configuration File for keepalived
#全局配置
global_defs {
   #keepalived切换的时候，发消息到指定的email，可配置多个email
   notification_email {
     feinik1@foxmail.com
	 feinik2@foxmail.com
   }
   #通知邮件从哪个地址发出
   notification_email_from feinik@foxmail.com
   #通知邮件的smtp地址
   smtp_server smtp.exmail.qq.com
   #连接smtp服务器的超时时间，单位秒
   smtp_connect_timeout 30
   #Keepalived的机器标识，一个网络内保持唯一
   router_id nginx-master
}
#执行脚本配置
vrrp_script chk_nginx {
	#脚本所在路径
    script "/home/project/keepalived/check_nginx.sh"
	#脚本执行间隔时间，秒
    interval 2
	#优先级
    weight 2
}
#keepalived实例配置
vrrp_instance VI_1 {
	#指定实例的初始状态，MASTER或BACKUP两种状态，并且需要大写
    state BACKUP
	#实例绑定的网卡
    interface ens33
	#虚拟路由标识，是一个数字，整个VRRP内唯一，如果keepalived配置了主备，需要相同
    virtual_router_id 51
	#优先级，数值愈大，优先级越高
    priority 99
	#MASTER与BACKUP之间同步检查的时间间隔，单位为秒
    advert_int 1
	#通信验证
    authentication {
        auth_type PASS
        auth_pass feinik
    }
	#追踪外围脚本
    track_script {
	    #这里配置vrrp_script的名称
        chk_nginx
    }
	#虚拟ip配置，可配置多个
    virtual_ipaddress {
        192.168.10.200
    }
}
```
### 3.5.4 Nginx状态检查脚本创建
（1）新建Nginx的状态检查脚本：check_nginx.sh
（2）内容如下：


```
#!/bin/sh
NGINX=/usr/common/nginx/sbin/nginx
PORT=80
nmap localhost -p $PORT | grep "$PORT/tcp open"
#echo $?
if [ $? -ne 0 ];then
    $NGINX -s stop
    #这里再次尝试启动NG
    $NGINX
    sleep 5
    nmap localhost -p $PORT | grep "$PORT/tcp open"
    [ $? -ne 0 ] && cd /usr/common/keepalived/sbin && pkill keepalived
    echo "stoped"
fi
```
# 4 运行测试
（1）为了更直观的查看到keepalived切换的效果，将11服务器中的nginx的upstream服务只配置11的tomcat服务地址，12服务器中的upstream服务只配置12的tomcat服务地址，这样只需要观察将11服务器中的nginx关闭看使用虚拟ip是否可以访问到12服务器的tomcat。
（2）分别启动两个服务器中的tomcat、nginx、keepalived，访问虚拟ip：192.168.10.200，可以查看到访问的是主keepalived服务器的tomcat
![](http://wx3.sinaimg.cn/mw690/63918611gy1fml0nugsrvj211u0bv467.jpg)
（3）关闭11服务器的nginx，nginx -s stop，再次访问虚拟ip，如下：说明主keepalived通过配置的脚本检测到了本服务的nginx服务挂掉了，所以立马切换至了备的keepalived，这时12服务器的keepalived升为了主，所以就访问到了12服务器的tomcat。
![](http://wx1.sinaimg.cn/mw690/63918611gy1fml0nuzmagj211v0c1ahf.jpg)
