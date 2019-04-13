
# python网络编程之数据传输UDP实例分析 - jiahaowanhao的博客 - CSDN博客


2018年08月07日 21:41:04[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：84


**[python网络编程之数据传输UDP实例分析](http://cda.pinggu.org/view/26306.html)**
本文实例讲述了python网络编程之数据传输UDP实现方法。分享给大家供大家参考。具体分析如下：
一、问题：
你觉得网络上像msn，qq之类的工具在多台机器之间互相传输数据神秘吗？你也想玩一下在两台机器之间传数据吗？今天让python告诉我们基本原理吧，当然只是做简单的了解，实际情况复杂的多。
我们今天用python实现一个简单的udp程序。
二、程序实现：
1) 使用模块
（socket）套接字模块： 套接字模块是一个非常简单的基于对象的接口，它提供对低层BSD套接字样式网络的访问 。使用该模块可以实现客户机和服务器套接字。
所谓的套接字是为特定网络协议（例如TCP/IP，ICMP/IP，UDP/IP等）套件对上的网络应用程序提供者提供当前可移植标准的对象。 它们允许程序接受并进行连接，如发送和接受数据。为了建立通信通道，网络通信的每个端点拥有一个套接字对象极为重要 。
2) 建立服务器
第1步：创建socket对象
s = socket.socket(family,type)
family 的值可以是AF_UNIX（unix域，用于同一台机器上的进程间通讯），也可以是AF_INET（对应于ip协议的TCP或UDP）
Type参数的值可以是： SOCK_STREAM（流套接字）或者 SOCK_DGRAM（数据报文套接字）,SOCK_RAW（raw套接字）。
我们这里使用udp数据报文套接字;
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
第2步：将socket绑定到指定地址上：s.bind(address),address 必须是一个双元素组，（host，port）
我们这里绑定本机8081端口： s.bind(("", port))
第3步：调用recvfrom方法接收客户端发来的数据，recvfrom方法需指定可接收的最大数据量。将返回数据内容和客户端地址；
import socket
port = 8081
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(("", port))
print "waiting on port:", port
while True:
data, addr = s.recvfrom(1024)
print "Received:", data, "from", addr
3） 客户端建立
建立客户端很简单，只用创建套接字对象，指定端口，调用sendto方法传输数据就ok了，如下；
import socket
port = 8081
host = "localhost"
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.sendto("Hello! It's working.", (host, port))
使用数据报方式发送简短的文本信息是非常简单的，如果你想要可靠的传输数据，这种方法是不可行，如果服务器不可用，信息就会丢失，下一节将学习如何使用tcp协议来传输信息。

