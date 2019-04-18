# ubuntu下安装telnet服务器（测试过...） - nosmatch的专栏 - CSDN博客
2011年08月23日 15:30:19[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：629标签：[ubuntu																[服务器																[测试																[stream																[server																[socket](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
1. sudo apt-get install xinetd telnetd 
2. 安装成功后，系统也会有相应提示， 
sudo vi /etc/inetd.conf并加入以下一行 
telnet stream tcp nowait telnetd /usr/sbin/tcpd /usr/sbin/in.telnetd 
3. sudo vi /etc/xinetd.conf并加入以下内容： 
# Simple configuration file for xinetd 
# 
# Some defaults, and include /etc/xinetd.d/ 
defaults 
{ 
# Please note that you need a log_type line to be able to use log_on_success 
# and log_on_failure. The default is the following : 
# log_type = SYSLOG daemon info 
instances = 60 
log_type = SYSLOG authpriv 
log_on_success = HOST PID 
log_on_failure = HOST 
cps = 25 30 
} 
includedir /etc/xinetd.d 
4. sudo vi /etc/xinetd.d/telnet并加入以下内容： 
# default: on 
# description: The telnet server serves telnet sessions; it uses \ 
# unencrypted username/password pairs for authentication. 
service telnet 
{ 
disable = no 
flags = REUSE 
socket_type = stream 
wait = no 
user = root 
server = /usr/sbin/in.telnetd 
log_on_failure += USERID 
} 
5. 重启机器或重启网络服务sudo /etc/init.d/xinetd restart 
6. 使用TELNET客户端远程登录即可进行非root用户访问。 
7.使用root登录： mv /etc/securetty /etc/securetty.bak 这样root可以登录了。也可这样: 
修改/etc/pam.d/login这个文件。只需将下面一行注释掉即可。 
#auth required lib/security/pam_securetty.so 
8. 详细配制/etc/xinetd.d/telnet 
service telnet 
{ 
disable =no 
bind =192.168.1.2 
only_from=192.168.1.0/24 
#上面这两行说明仅提供内部网段！ 
Instance =UNLIMITED 
Nice =0 
Flags =REUSE 
socket_type=stream 
wait =no 
user =root 
#server =/usr/sbin/telnetd 
server =/usr/sbin/in.telnetd 
server_args =-a none 
log_on_failure +=USERID 
} 
service telnet 
{ 
disable =no 
bind =140.116.142.196 
only_from=140.116.0.0/16 
no_access=140.116.32.{10,26} 
#上面三行设置外部较为严格的限制 
instance =10 
umask =022 
nice =10 
flags =REUSE 
socket_type=stream 
wait =no 
user =root 
#server =/usr/sbin/telnetd 
server =/usr/sbin/in.telnetd 
log_on_failure +=USERID 
} 
9.加设防火墙iptables： 
如果想要针对192.168.0.0/24这个网段及61.xxx.xxx.xxx这个IP进行telnet开放，可以增加下面几行规则： 
/sbin/iptables -A INPUT -p tcp -i eth0 -s 192.168.0.0/24 --dport 23 -j ACCEPT 
/sbin/iptables -A INPUT -p tcp -i eth0 -s 61.xxx.xxx.xxx --dport 23 -j ACCEPT 
/sbin/iptables -A INPUT -p tcp -i eth0 --dport 23 -j DROP 
10.加设防火墙/etc/hosts.allow(deny)机制： 
上面开放了192.168.0.0/24这个网段，但是如果您只想让其中的192.168.0.1~192.168.0.5进入，可以设置如下 ： 
vi /etc/hosts.allow 
in.telnetd:192.168.0.1,192.168.0.2,192.168.0.3,192.168.0.4,192.168.0.5:allow 
