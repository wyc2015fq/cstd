# ARM-Linux配置DHCP自动获取IP地址 - DoubleLi - 博客园






备注：
内核版本：2.6.30.9
busybox版本：1.15.2

PC Linux和开发板Linux的工作用户：root


1. 配置内核：
[*] Networking support --->
Networking options --->
<*> Packet socket
<*> Unix domain sockets
[*] TCP/IP networking
[*] IP: kernel level autoconfiguration
[*] IP: DHCP support
[*] Network packet filtering framework (Netfilter) --->
2. 配置busybox:
Networking Utilities --->
[*] udhcp client (udhcpc)
3.建立配置文件：
从busybox的examples/udhcp/下copy simple.script文件到开发板/usr/share/udhcpc/下，并重命名为default.script

4. 测试：
在命令台执行udhcpc，注意：必须确保局域网内存在DHCP服务器，否则udhcp执行失败。
在easy2440上面执行结果如下：
udhcpc (v1.15.2) started
Setting IP address 0.0.0.0 on eth0
Sending discover...
Sending select for 192.168.1.101...
Lease of 192.168.1.101 obtained, lease time 7200
Setting IP address 192.168.1.101 on eth0
Deleting routers
route: SIOCDELRT: No such process
Adding router 192.168.1.1
Recreating /etc/resolv.conf
Adding DNS server 211.148.192.141
Adding DNS server 210.21.196.6

5. 修改系统初始化配置文件，让开发板开机后自动获取IP地址：
修改/etc/init.d/rcS文件在适当位置添加命令: /sbin/udhcpc &

6. Enjoy it!









