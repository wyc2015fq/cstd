# linux下简单好用的端口映射转发工具rinetd 转-----只支持TCP端口转发 - xqhrs232的专栏 - CSDN博客
2018年07月25日 17:00:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：320
原文地址::[https://www.cnblogs.com/clds/p/5732370.html](https://www.cnblogs.com/clds/p/5732370.html)
相关文章
1、[利用rinetd将tcp端口转发到任意的机器上](http://chasewinds.iteye.com/blog/813892)----[http://chasewinds.iteye.com/blog/813892](http://chasewinds.iteye.com/blog/813892)
2、Linux端口转发-rinted工具部署、配置、使用----[http://blog.51cto.com/nanfeibobo/2116934](http://blog.51cto.com/nanfeibobo/2116934)
linux下简单好用的工具rinetd，实现端口映射/转发/重定向
官网地址[http://www.boutell.com/rinetd](http://www.boutell.com/rinetd)
软件下载
wget [http://www.boutell.com/rinetd/http/rinetd.tar.gz](http://www.boutell.com/rinetd/http/rinetd.tar.gz)
解压安装
tar zxvf rinetd.tar.gz
make
make install
编辑配置
vi /etc/rinetd.conf
0.0.0.0 8080 172.19.94.3 8080
0.0.0.0 2222 192.168.0.103 3389
1.2.3.4 80     192.168.0.10 80
说明一下（0.0.0.0表示本机绑定所有可用地址）
将所有发往本机8080端口的请求转发到172.19.94.3的8080端口
将所有发往本机2222端口的请求转发到192.168.0.103的3389端口
将所有发往1.2.3.4的80端口请求转发到192.168.0.10的80端口
命令格式是
bindaddress bindport connectaddress connectport
绑定的地址  绑定的端口  连接的地址  连接的端口
或
[Source Address] [Source Port] [Destination Address] [Destination Port]
源地址  源端口   目的地址  目的端口
启动程序
pkill rinetd  ##关闭进程
rinetd -c /etc/rinetd.conf  ##启动转发
把这条命令加到/etc/rc.local里面就可以开机自动运行
查看状态
netstat -antup
需要注意
1.rinetd.conf中绑定的本机端口必须没有被其它程序占用
2.运行rinetd的系统防火墙应该打开绑定的本机端口
例如：
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 1111 -j ACCEPT
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 2222 -j ACCEPT
