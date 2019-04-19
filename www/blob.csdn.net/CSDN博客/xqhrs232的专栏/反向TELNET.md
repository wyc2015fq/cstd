# 反向TELNET - xqhrs232的专栏 - CSDN博客
2017年04月06日 10:31:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1075
原文地址::[http://blog.sina.com.cn/s/blog_4a023ebb010005gc.html](http://blog.sina.com.cn/s/blog_4a023ebb010005gc.html)
相关文章
1、[反向telnet配置 （留着自己以后用 呵呵）](http://blog.csdn.net/sorrowegg/article/details/5774147)----[http://blog.csdn.net/sorrowegg/article/details/5774147](http://blog.csdn.net/sorrowegg/article/details/5774147)
2、反向Telnet 
 ----[http://mzhehome.blog.163.com/blog/static/207728820112805253826/](http://mzhehome.blog.163.com/blog/static/207728820112805253826/)
3、Reverse(反向) telnet----[http://www.clnchina.com.cn/professional_certs/2011/0627/11301.shtml](http://www.clnchina.com.cn/professional_certs/2011/0627/11301.shtml)
反向tenlnet 是借助其他的设备 在tenlent到别的设备上去
具体的说就是 你先tenlnet到一台设备上 在从那台设备上再tenlnet到另外其他的设备上
这就是反向tenlnet
因为 一台设备可以如果接口够的话 可以接多台设备
这样可以给我们现实当中带了很多的方便的之处
网上的资料：
The term reverse Telnet means that you are initiating a Telnet session out the asynchronous line, instead of accepting a connection into the line (which is a forward connection).
Establishing a Reverse Telnet Session to a Modem To establish a reverse Telnet session to a modem, determine the IP address of your LAN (Ethernet) interface, then enter a Telnet command to port 2000 + n on the access server, where n is the line number to which
 the modem is connected. For example, to connect to the modem attached to line 1, enter the following command from an EXEC session on the access server:
router# telnet 172.16.1.10 2001
Trying 172.16.1.10, 2001 ... Open
then Test the Modem Connection
at
OK
.......
.......
Suspend and Terminate Telnet Sessions
Step 1 Suspend the Telnet session by entering Ctrl-Shift-6 x:
- suspend keystroke -
router#
Step 2 To disconnect (completely close) a session, issue the disconnect EXEC command:
router# disconnect line 1
Closing connection to 172.16.1.10 [confirm] y
router#
-------------------------------
总结： 反向Telnet 就是使网络上的用户能够通过服务器登录到没有直接连在Ethernet网络上的计算机或设备。 这里的服务器就是 路由器，设备就是 MODEM。
不过我看了还是有点头蒙，这样做的目的是什么？
vilian
2006-03-27, 10:33
最 近做了一个CISCO实验室，用到了反向telnet，用一台2610上NM-32A模块，配合CAB-OCTAL-ASYNC线缆，连接其它交换路由设 备的配置口，在2610上做反向telnet，而其余设备不做任何配置，就可以管理其它的交换路由设备，免去了来回插拔配置线的麻烦。
2610上的配置如下：
2610#sh run
Building configuration...
Current configuration : 1756 bytes
!
version 12.3
service timestamps debug datetime msec
service timestamps log datetime msec
no service password-encryption
!
hostname 2610
!
boot-start-marker
boot-end-marker
!
enable password cisco
!
no network-clock-participate slot 1
no network-clock-participate wic 0
no aaa new-model
ip subnet-zero
ip cef
!
!
!
ip host jg4-fw 2060 172.16.1.1
ip host jg1-r1 2033 172.16.1.1
ip host jg1-r2 2034 172.16.1.1
ip host jg1-r3 2035 172.16.1.1
ip host jg1-j1 2036 172.16.1.1
ip host jg1-j2 2037 172.16.1.1
ip host jg1-j3 2038 172.16.1.1
ip host jg2-r1 2041 172.16.1.1
ip host jg2-r2 2042 172.16.1.1
ip host jg2-r3 2043 172.16.1.1
ip host jg2-fw 2044 172.16.1.1
ip host jg2-j1 2045 172.16.1.1
ip host jg2-j2 2046 172.16.1.1
ip host jg2-j3 2047 172.16.1.1
ip host jg3-r1 2049 172.16.1.1
ip host jg3-r2 2050 172.16.1.1
ip host jg3-r3 2051 172.16.1.1
ip host jg3-2610 2052 172.16.1.1
ip host jg3-j1 2053 172.16.1.1
ip host jg3-j2 2054 172.16.1.1
ip host jg3-j3 2055 172.16.1.1
ip host jg4-r1 2057 172.16.1.1
ip host jg4-r2 2058 172.16.1.1
ip host jg4-r3 2059 172.16.1.1
ip host jg4-j1 2061 172.16.1.1
ip host jg4-j2 2062 172.16.1.1
ip host jg4-j3 2063 172.16.1.1
no ftp-server write-enable
!
!
!
!
interface Loopback1
ip address 172.16.1.1 255.255.0.0
!interface FastEthernet0/0
description to-jg3-j1-fa0/7
ip address 192.168.41.2 255.255.255.0
duplex auto
speed auto
!
router rip
version 2
network 172.16.0.0
network 192.168.41.0
!
ip default-gateway 192.168.41.1
ip classless
ip route 0.0.0.0 0.0.0.0 192.168.41.1
ip http server
!
!
line con 0
exec-character-bits 8
transport preferred all
transport output all
line 33 64
session-timeout 1
exec-timeout 0 0
no exec
transport input all
line aux 0
line vty 0 4
password cisco
login
!
!
!
end
2610#
那你的八爪鱼的线要很长很长，呵呵
反向telnet主要是用在调试设备的时候
因为一台还未做任何配置的设备是无法进行telnet等其他远程登录手段在一台终端对其进行控制的，唯一的方法就是使用console或者aux口进行配置，但是每一个设备有一个控制口，在配置联调过程中非常麻烦，要来回拔插console线缆或者使用多个终端。
通过使用一台2509或者2511或者使用NM-32A这种模块，下联八爪鱼，每个line对应不同设备的控制口，可以在一台设备上实现对多台设备的配置访问，这样省去了不少麻烦。当然在实际工作中的时候，用的不多，没有必要研究很透，知道怎么配置就ok
