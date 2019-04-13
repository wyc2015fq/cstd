
# docker基础：私库系列：再探Harbor：(8) https方式下的客户端docker login连接方法 - 知行合一 止于至善 - CSDN博客

2019年01月16日 22:34:04[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1090所属专栏：[DevOps系列之：镜像私库](https://blog.csdn.net/column/details/26478.html)



这篇文章整理一下https方式搭建的harbor服务器，客户端如何使用docker login进行连接。
# 环境准备
需要准备两台机器，本文环境设定和配置信息如下所示：
|机器名称|操作系统|docker 版本|作用|
|---|---|---|---|
|platform|CentOS 7.3|>=1.13.1|以https方式安装harbor|
|kong|CentOS 7.3|>=1.13.1|作为连接harbor的客户端|
# https方式的harbor设定
本文使用harbor版本为：v1.6.0
offline安装包下载地址：[https://storage.googleapis.com/harbor-releases/release-1.6.0/harbor-offline-installer-v1.6.0.tgz](https://storage.googleapis.com/harbor-releases/release-1.6.0/harbor-offline-installer-v1.6.0.tgz)
安装与设定可参照：[https://blog.csdn.net/liumiaocn/article/details/81813693](https://blog.csdn.net/liumiaocn/article/details/81813693)
# 服务器端确认
`[root@platform harbor]# docker-compose ps
       Name                     Command               State                                 Ports                                
--------------------------------------------------------------------------------------------------------------------------------
harbor-adminserver   /harbor/start.sh                 Up                                                                         
harbor-db            /entrypoint.sh postgres          Up      5432/tcp                                                           
harbor-jobservice    /harbor/start.sh                 Up                                                                         
harbor-log           /bin/sh -c /usr/local/bin/ ...   Up      127.0.0.1:1514->10514/tcp                                          
harbor-ui            /harbor/start.sh                 Up                                                                         
nginx                nginx -g daemon off;             Up      0.0.0.0:32031->443/tcp, 0.0.0.0:4443->4443/tcp, 0.0.0.0:80->80/tcp 
redis                docker-entrypoint.sh redis ...   Up      6379/tcp                                                           
registry             /entrypoint.sh /etc/regist ...   Up      5000/tcp                                                           
[root@platform harbor]#`
# docker login连接确认：服务器端
服务器端连接自身结果确认：
`[root@platform cert]# ip ad |grep 118
    inet 192.168.163.118/24 brd 192.168.163.255 scope global enp0s3
[root@platform cert]# docker login 192.168.163.118:32031
Username (admin): admin
Password: 
Login Succeeded
[root@platform cert]#`
# docker login连接确认：client端
这里使用ip为192.168.163.117的机器（机器名为kong）作为client端进行验证。连接之后提示如下信息：
`[root@kong tmp]# docker login 192.168.163.118:32031
Username: admin
Password: 
Error response from daemon: Get https://192.168.163.118:32031/v1/users/: x509: certificate signed by unknown authority
[root@kong tmp]#`这是因为需要签发的证书信息，将其加入到OS的认证列表中，然后重启docker即可解决此问题，解决方式如下所示：
`[root@kong tmp]# chmod 644 /etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem
[root@kong tmp]# cat 192.168.163.118.crt >> /etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem
[root@kong tmp]# chmod 444 /etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem
[root@kong tmp]#`而关于192.168.163.118.crt证书的生成方法，请参看上文中提到的文档进行确认。可以看到，设定之后，需要重启docker才能生效
`[root@kong tmp]# docker login 192.168.163.118:32031
Username: admin
Password: 
Error response from daemon: Get https://192.168.163.118:32031/v1/users/: x509: certificate signed by unknown authority
[root@kong tmp]# 
[root@kong tmp]# systemctl restart docker
[root@kong tmp]# docker login 192.168.163.118:32031
Username: admin
Password: 
Login Succeeded
[root@kong tmp]#`
# 其他问题
## certificate has expired or is not yet valid
`[root@kong ~]# docker login 192.168.163.118:32031
Username: admin
Password: 
Error response from daemon: Get https://192.168.163.118:32031/v1/users/: x509: certificate has expired or is not yet valid
[root@kong ~]#`
## 可能原因和对应方法
客户端和服务器端的时间相差太大也可能会出现这个问题，只需要使用相同的ntp源进行对时即可解决这个问题，简单来说，比如两台机器都执行ntpdate 0.cn.pool.ntp.org命令之后再重启docker，这个问题可能就会被解决。

