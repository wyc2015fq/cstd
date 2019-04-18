# Socket - weixin_33985507的博客 - CSDN博客
2017年01月11日 16:37:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
**[Socket](https://link.jianshu.com?t=http://www.cnblogs.com/skynet/archive/2010/12/12/1903949.html)**
**socket是套接字，多指传输层网络接口。**
**Socket和SocketServer是基于套接字的服务端和客户端实现**
**webSocket，是一个应用层协议，说的是，目前浏览器实现的一套通信协议，用来解决之前HTTP，请求响应模型不合适的场合。**
**Socket和SocketServer是服务端和客户端的区别**
1. 服务器端程序设计
在服务器端，利用ServerSocket类的构造函数ServerSocket(int port)创建一个ServerSocket类的对象，port参数传递端口，这个端口就是服务器监听连接请求的端口，如果在这时出现错误将抛出IOException异常对象，否则将创建ServerSocket对象并开始准备接收连接请求。
服务程序从调用ServerSocket的accept()方法开始，直到连接建立。在建立连接后，accept()返回一个最近创建的Socket对象，该Socket对象绑定了客户程序的IP地址或端口号。
2.客户端程序设计
当客户程序需要与服务器程序通信时，需在客户机创建一个Socket对象。Socket类有构造函数Socket(InetAddress addr，int port)和Socket(String host，intport)，两个构造函数都创建了一个基于Socket的连接服务器端流套接字的流套接字。对于第一个InetAd-dress子类对象通过addr参数获得服务器主机的IP地址，对于第二个函数host参数包被分配到InetAddress对象中，如果没有IP地址与host参数相一致，那么将抛出UnknownHostException异常对象。两个函数都通过参数port获得服务器的端口号。假设已经建立连接了，网络API将在客户端基于Socket的流套接字中捆绑客户程序的IP地址和任意一个端口号，否则两个函数都会抛出一个IOException对象。
如果创建了一个Socket对象，那么它可通过get-InputStream()方法从服务程序获得输入流读传送来的信息，也可通过调用getOutputStream()方法获得输出流来发送消息。在读写活动完成之后，客户程序调用close()方法关闭流和流套接字。
