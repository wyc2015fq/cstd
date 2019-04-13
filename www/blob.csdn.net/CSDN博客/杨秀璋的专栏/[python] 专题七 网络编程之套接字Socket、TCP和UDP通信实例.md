
# [python] 专题七.网络编程之套接字Socket、TCP和UDP通信实例 - 杨秀璋的专栏 - CSDN博客

2015年10月05日 08:40:19[Eastmount](https://me.csdn.net/Eastmount)阅读数：8596所属专栏：[Python学习系列](https://blog.csdn.net/column/details/eastmount-python.html)



很早以前研究过C\#和C++的网络通信,参考我的文章：
[C\#网络编程之Tcp实现客户端和服务器聊天](http://blog.csdn.net/eastmount/article/details/9389935)
[C\#网络编程之套接字编程基础知识](http://blog.csdn.net/eastmount/article/details/9321153)
[C\#网络编程之使用Socket类Send、Receive方法的同步通讯](http://blog.csdn.net/eastmount/article/details/9409935)
Python网络编程也类似。同时最近找工作笔试面试考察Socket套接字、TCP\UDP区别比较多，所以这篇文章主要精简了《Python核心编程（第二版）》第16章内容。内容包括：服务器和客户端架构、套接字Socket、TCP\UDP通信实例和常见笔试考题。
最后希望文章对你有所帮助，如果有不足之处，还请海涵~

## 一. 服务器和客户端架构
**1.什么是客户端/服务区架构？**
书中的定义是服务器是一个软件或硬件，用于向一个或多个客户端（客户）提供所需要的“服务”。服务器存在的唯一目的就是等待客户的请求，给这些客户服务，然后再等待其他的请求。而客户连接上（预先已知的）服务器，提出自己的请求，发送必要的数据，然后等待服务器完成请求或说明失败原因的反馈。
服务器不停的处理外来的请求，而客户一次只能提出一个服务的请求，等待结果。再结束这个事务。客户之后可以再提出其他的请求，只是这个请求会被视为另一个不同的事务了。
**2.硬件客户端/服务器架构和软件客户端/服务器架构**
硬件的客户端/服务器架构，例如打印服务器、文件服务器（客户可以远程把服务器的磁盘映射到自己本体并使用）；软件客户端/服务器架构主要是程序的运行、数据收发、升级等，最常见的是Web服务器、数据库服务器。如一台机器存放一些网页或Web应用程序，然后启动服务。其服务器的任务就是接受客户端的请求，把网页发给客户端（如用户计算机上的浏览器），然后再等待下一个客户端请求。
**3.客户端/服务器网络编程**
在完成服务之前，服务器必须要先完成一些设置。先要先创建一个通讯端点，让服务器能“监听”请求。你可以把我们服务器比作一个公司的接待员或回答公司总线电话的话务员，一旦电话和设备安装完成，话务员也就到位后，服务就开始了。
同样一旦通信端点创建好之后，我们在“监听”的服务器就可以进入它那等待和处理客户请求的无限循环中了。服务器准备好之后，也要通知潜在的客户，让它们知道服务器已经准备好处理服务了，否则没人会提请求的。所以需要把公司电话公开给客户。
而客户端只要创建一个通信端点，建立到服务器的连接，然后客户端就可以提出请求了。请求中也可以包含必要的数据交互。一旦请求处理完成，客户端收到了结果，通信就结束了。这就是客户端和服务器的简单网络通信。

## 二. 套接字Socket
**1.什么是套接字**
套接字是一种具有之前所说的“通信端点”概念的计算网络数据结构。相当于**电话插口**，没它无法通信，这个比喻非常形象。
套接字起源于20世纪70年代加州伯克利分校版本的Unix，即BSD Unix。又称为“伯克利套接字”或“BSD套接字”。最初套接字被设计用在同一台主机上多个应用程序之间的通讯，这被称为进程间通讯或IPC。
套接字分两种：基于文件型和基于网络的
第一个套接字家族为**AF_UNIX**，表示“地址家族：UNIX”。包括Python在内的大多数流行平台上都使用术语“地址家族”及其缩写AF。由于两个进程都运行在同一台机器上，而且这些套接字是基于文件的，所以它们的底层结构是由文件系统来支持的。可以理解为同一台电脑上，文件系统确实是不同的进程都能进行访问的。
第二个套接字家族为**AF_INET**，表示”地址家族：Internet“。还有一种地址家族AF_INET6被用于网际协议IPv6寻址。Python 2.5中加入了一种Linux套接字的支持：**AF_NETLINK**（无连接）套接字家族，让用户代码与内核代码之间的IPC可以使用标准BSD套接字接口，这种方法更为精巧和安全。
Python只支持AF_UNIX、AF_NETLINK和AF_INET家族。网络编程关注AF_INET。
如果把套接字比作电话的查看——即通信的最底层结构，那主机与端口就相当于区号和电话号码的一对组合。一个因特网地址由网络通信必须的主机与端口组成。
而且另一端一定要有人接听才行，否则会提示”对不起，您拨打的电话是空号，请查询后再拨“。同样你也可能会遇到如”不能连接该服务器、服务器无法响应“等。合法的端口范围是0~65535，其中小于1024端口号为系统保留端口。
**2.面向连接与无连接**
**面向连接：**通信之前一定要建立一条连接，这种通信方式也被成为”虚电路“或”流套接字“。面向连接的通信方式提供了**顺序的、可靠地、不会重复的数据传输**，而且也不会被加上数据边界。这意味着，每发送一份信息，可能会被拆分成多份，每份都会不多不少地正确到达目的地，然后重新按顺序拼装起来，传给正等待的应用程序。
实现这种连接的主要协议就是**传输控制协议TCP**。要创建TCP套接字就得创建时指定套接字类型为**SOCK_STREAM**。TCP套接字这个类型表示它作为流套接字的特点。由于这些套接字使用网际协议IP来查找网络中的主机，所以这样形成的整个系统，一般会由这两个协议（TCP和IP）组合描述，即TCP/IP。
**无连接：**无需建立连接就可以通讯。但此时，数据到达的顺序、可靠性及不重复性就无法保障了。数据报会保留数据边界，这就表示数据是**整个发送**的，不会像面向连接的协议先拆分成小块。它就相当于邮政服务一样，邮件和包裹不一定按照发送顺序达到，有的甚至可能根本到达不到。而且网络中的报文可能会重复发送。
那么这么多缺点，为什么还要使用它呢？由于面向连接套接字要提供一些保证，需要维护虚电路连接，这都是严重的额外负担。数据报**没有这些负担**，所有它会更”便宜“，通常能提供更好的性能，更适合某些场合，如**现场直播**要求的实时数据讲究快等。
实现这种连接的主要协议是**用户数据报协议UDP**。要创建UDP套接字就得创建时指定套接字类型为**SOCK_DGRAM**。这个名字源于datagram（数据报），这些套接字使用网际协议来查找网络主机，整个系统叫UDP/IP。
**3.socket()模块函数**
使用socket模块的socket()函数来创建套接字。语法如下：
socket(socket_family, socket_type, protocol=0)
其中socket_family不是AF_VNIX就是AF_INET，socket_type可以是SOCK_STREAM或者SOCK_DGRAM，protocol一般不填，默认值是0。
创建一个TCP/IP套接字的语法如下：
tcpSock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
同样创建一个UDP/IP套接字的语法如下：
udpSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
由于socket模块中有太多属性，所以使用"from socket import *"语句，把socket模块里面的所有属性都带到命名空间中，大幅缩短代码。调用如下：
tcpSock = socket(AF_INET, SOCK_STREAM)
**4.套接字对象方法**
下面是最常用的套接字对象方法：
**服务器端套接字函数**
|socket|类型
|描述
|
|s.bind()
|绑定地址（主机号 端口号对）到套接字
|
|s.listen()
|开始TCP监听
|
|s.accept()
|被动接受TCP客户端连接，（阻塞式）等待连续的到来
|
**客户端套接字函数**
|socket|类型
|描述
|
|s.connect()
|主动初始化TCP服务器连接
|
|s.connect_ex()
|connect()函数扩展版本，出错时返回出错码而不是跑出异常
|
**公共用途的套接字函数**|socket|类型
|描述
|
|s.recv()
|接受TCP数据
|
|s.send()
|发送TCP数据
|
|s.sendall()
|完整发送TCP数据
|
|s.recvfrom()
|接受UDP数据
|
|s.sendto()
|发送UDP数据
|
|s.getpeername()
|连接到当前套接字的远端地址（TCP连接）
|
|s.getsockname()
|获取当前套接字的地址
|
|s.getsockopt()
|返回指定套接字的参数
|
|s.s|etsockopt|()
|设置指定套接字的参数
|
|s.close()
|关闭套接字
|
**面向模块的套接字函数**
|socket|类型
|描述
|
|s.setblocking()
|设置套接字的阻塞与非阻塞模式
|
|s.settimeout()
|设置阻塞套接字操作的超时时间
|
|s.gettimeout()
|得到阻塞套接字操作的超时时间
|
**面向文件的套接字函数**
|socket|类型
|描述
|
|s.fileno()
|套接字的文件描述符
|
|s.makefile()
|创建一个与套接字关联的文件对象
|
提示：在运行网络应用程序时，如果能够使用在不同的电脑上运行服务器和客户端最好不过，它能让你更好理解通信过程，而更多的是方位localhost或127.0.0.1.

## 三. TCP通信实例
**1.服务器 tcpSerSock.py**
核心操作如下：
ss = socket()\# 创建服务器套接字
ss.bind()\# 地址绑定到套接字上
ss.listen()\# 监听连接
inf_loop:\# 服务器无限循环
cs = ss.accept()\# 接受客户端连接 阻塞式:程序连接之前处于挂起状态
comm_loop:\# 通信循环
cs.recv()/cs.send()\# 对话 接受与发送数据
cs.close()\# 关闭客户端套接字
ss.close()\# 关闭服务器套接字 (可选)

```python
# -*- coding: utf-8 -*- 
from socket import *
from time import ctime
HOST = 'localhost'          #主机名
PORT =  21567               #端口号
BUFSIZE = 1024              #缓冲区大小1K
ADDR = (HOST,PORT)
tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(ADDR)       #绑定地址到套接字
tcpSerSock.listen(5)        #监听 最多同时5个连接进来
while True:                 #无限循环等待连接到来
    try:
        print 'Waiting for connection ....'
        tcpCliSock, addr = tcpSerSock.accept()  #被动接受客户端连接
        print u'Connected client from : ', addr
        while True:
            data = tcpCliSock.recv(BUFSIZE)     #接受数据
            if not data:
                break
            else:
                print 'Client: ',data
            tcpCliSock.send('[%s] %s' %(ctime(),data)) #时间戳
    except Exception,e:
        print 'Error: ',e
tcpSerSock.close()          #关闭服务器
```
**2.客户端 tcpCliSock.py**
核心操作如下：
cs = socket()\# 创建客户端套接字
cs.connect()\# 尝试连接服务器
comm_loop:\# 通讯循环
cs.send()/cs.recv()\# 对话 发送接受数据
cs.close()\# 关闭客户端套接字
```python
# -*- coding: utf-8 -*- 
from socket import *
HOST = 'localhost'          #主机名
PORT =  21567               #端口号 与服务器一致
BUFSIZE = 1024              #缓冲区大小1K
ADDR = (HOST,PORT)
tcpCliSock = socket(AF_INET, SOCK_STREAM)
tcpCliSock.connect(ADDR)    #连接服务器
while True:                 #无限循环等待连接到来
    try:
        data = raw_input('>')
        if not data:
            break
        tcpCliSock.send(data)            #发送数据
        data = tcpCliSock.recv(BUFSIZE)  #接受数据
        if not data:
            break
        print 'Server: ', data
    except Exception,e:
        print 'Error: ',e
        
tcpCliSock.close()          #关闭客户端
```
**3.运行结果及注意**
由于服务器被动地无限循环等待连接，所以需要**先运行服务器，再开客户端。**又因为我的Python总会无法响应，所以采用cmd运行服务器Server程序，Python IDLE运行客户端进行通信。运行结果如下图所示：
如果出现错误[Error] Bad file descriptor表示服务器关闭客户端连接了，删除即可
![](https://img-blog.csdn.net/20151005050518291)
建议：创建线程来处理客户端请求。SocketServer模块是一个基于socket模块的高级别的套接字通信模块，支持新的线程或进程中处理客户端请求。同时建议在退出和调用服务器close()函数时使用try-except语句。

## 四. UDP通信实例
**1.服务器 udpSerSock.py**
核心操作如下：
ss = socket()\#
 创建服务器套接字
ss.bind()   　　              \# 绑定服务器套接字
inf_loop:                       \# 服务器无限循环
cs = ss.recvfrom()/ss.sendto()
\# 对话 接受与发送数据
ss.close()                      \# 关闭服务器套接字

```python
# -*- coding: utf-8 -*- 
from socket import *
from time import ctime
HOST = ''                   #主机名
PORT =  21567               #端口号
BUFSIZE = 1024              #缓冲区大小1K
ADDR = (HOST,PORT)
udpSerSock = socket(AF_INET, SOCK_DGRAM)
udpSerSock.bind(ADDR)       #绑定地址到套接字
while True:                 #无限循环等待连接到来
    try:
        print 'Waiting for message ....'
        data, addr = udpSerSock.recvfrom(BUFSIZE)          #接受UDP
        print 'Get client msg is: ', data
        udpSerSock.sendto('[%s] %s' %(ctime(),data), addr) #发送UDP
        print 'Received from and returned to: ',addr
    except Exception,e:
        print 'Error: ',e
udpSerSock.close()          #关闭服务器
```
**2.客户端 udpCliSock.py**
核心操作如下：
cs = socket()\# 创建客户端套接字
inf_loop:                                  \# 服务器无限循环
cs.sendto()/cs.recvfrom()\#
 对话 接受与发送数据
cs.close()                                 \# 关闭客户端套接字

```python
# -*- coding: utf-8 -*- 
from socket import *
HOST = 'localhost'          #主机名
PORT =  21567               #端口号 与服务器一致
BUFSIZE = 1024              #缓冲区大小1K
ADDR = (HOST,PORT)
udpCliSock = socket(AF_INET, SOCK_DGRAM)
while True:                 #无限循环等待连接到来
    try:
        data = raw_input('>')
        if not data:
            break
        udpCliSock.sendto(data, ADDR)            #发送数据
        data,ADDR = udpCliSock.recvfrom(BUFSIZE)  #接受数据
        if not data:
            break
        print 'Server : ', data
    except Exception,e:
        print 'Error: ',e
        
udpCliSock.close()          #关闭客户端
```
**3.运行结果及注意**
UDP服务器不是面向连接的，所以不需要设置什么东西，直接等待连接就好。同时由于数据报套接字是无连接的，所以无法把客户端连接交给另外的套接字进行后续的通讯，这些服务器只是接受消息，需要的话加时间错后返回一个收到的结果给客户端。
UDP客户端与TCP客户端唯一区别就是不用去UDP服务器建立连接，而是直接把消息发送出去，然后等待服务器回复即可。
运行结果如下图所示：白色为客户端输入消息，黑色为服务器收到消息并回复。当Client输入"Hello, I am Client"时，服务器显示该消息并返回时间戳和收到的信息给客户端。![](https://img-blog.csdn.net/20151005082202694)

**总结：**
后面大家自己可以阅读下SocketServer模块，它是标准库中一个高级别的模块，用于简化实现网络客户端和服务器所需的大量样板代码。该模块中已经实现了一些可供使用的类直接调用几块。
Twisted框架是一个完全事件驱动的网络框架。它允许你使用和开发完全异步的网络应用程序和协议。
这些东西我更倾向于分享原理和底层的一些东西吧！同时最近考到的笔试题包括：TCP和UDP的区别、socket其中的参数含义、TCP三次握手及传递的参数、写个socket通讯伪代码。
总之，希望文章对你有所帮助~
（By：Eastmount 2015-10-5 早上8点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）



