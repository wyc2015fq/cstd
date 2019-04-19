# Socket与TCP/UDP编程 - 建建的博客 - CSDN博客
2017年02月20日 16:29:44[纪建](https://me.csdn.net/u013898698)阅读数：307
原文
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/xn4545945/article/details/8098646
http://blog.csdn[.Net](http://lib.csdn.net/base/dotnet)/qinpeng100423/article/details/8980423
**一、简介**
套接字（socket）是通信的基石，是支持TCP/IP协议的网络通信的基本操作单元。它是网络通信过程中端点的抽象表示，包含进行网络通信必须的五种信息：连接使用的协议，本地主机的IP地址，本地进程的协议端口，远地主机的IP地址，远地进程的协议端口。创建Socket连接时，可以指定使用的传输层协议，Socket可以支持不同的传输层协议（TCP或UDP），当使用TCP协议进行连接时，该Socket连接就是一个TCP连接。关于Socket的更多内容请参考《[Socket基本原理](http://blog.csdn.net/hudashi/article/details/50790002)》
**二、基于TCP的socket编程**
[Java](http://lib.csdn.net/base/javase)中用于TCP协议的socket编程的主要API有：
java.net.ServerSocket是用来创建服务器端的套接字socket。
java.net.Socket是用来创建客户端的套接字socket。
InetAddress（java.net.InetAddress）类：用来表示IP地址。
![](https://img-blog.csdn.net/20160304151015663)
凡是基于TCP创建的套接字可以叫做流套接字。
 服务器端相当于一个监听器，用来监听端口。
 服务器与客服端之间的通讯都是输入输出流来实现的。
服务器端代码如下：
**[java]**[view
 plain](http://blog.csdn.net/hudashi/article/details/50802544#)[copy](http://blog.csdn.net/hudashi/article/details/50802544#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1597159/fork)
- import java.net.*;  
- import java.io.*;  
- 
- class SocketTCPServer extends Thread//让类继承为线程类
- {  
- private Socket s;  
-     SocketTCPServer(Socket s)  
-     {  
- this.s = s;  
-     }  
- 
- publicstaticvoid main(String []args)  
-     {  
-             server();  
-     }  
- 
- publicvoid run()//这个就是线程方法了
-     {  
- try
-         {//当然当不想直接发送数据，就会去创建一个带缓冲的流
-             OutputStream os=s.getOutputStream();  
-             BufferedOutputStream bos = new BufferedOutputStream(os);  
- //os.write("my name is xuneng!".getBytes());
-             bos.write("my name is xuneng!".getBytes());  
- 
-             InputStream is=s.getInputStream();  
- byte [] buf =newbyte[100];//别忘了加new
- int len=is.read(buf);  
-             System.out.println(new String(buf,0,len));  
- 
-             bos.close();  
-             is.close();  
-             os.close();  
-             s.close();  
-         }  
- catch(Exception e)  
-         {  
-             e.printStackTrace();  
-         }  
- 
-     }  
- 
- publicstaticvoid server()//完成之后一定要记得关闭各种流于套接字
-     {  
- try
-         {  
-             ServerSocket ss = new ServerSocket(8000);//自定义的一个端口
- while(true)//服务器端会老这样启动着。
-             {  
-                 System.out.println("the server is starting .......");  
-                 Socket s=ss.accept();            //一直等待着接收消息
- new SocketTCPServer(s).start();//当接受到请求的时候，就返回一个套接字，创建一个线程    
-             }  
-         }  
- catch(Exception e)  
-         {  
-             e.printStackTrace();  
-         }  
- 
-     }  
- 
- }  
客户端代码如下：
**[java]**[view
 plain](http://blog.csdn.net/hudashi/article/details/50802544#)[copy](http://blog.csdn.net/hudashi/article/details/50802544#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1597159/fork)
- import java.net.*;  
- import java.io.*;  
- 
- class SocketTCPClient  
- {  
- private Socket s;  
-     SocketTCPClient(Socket s)  
-     {  
- this.s = s;  
-     }  
- 
- publicstaticvoid main(String []args)  
-     {  
-             client();  
-     }  
- 
- publicstaticvoid client()  
-     {  
- try
-         {  
-             Socket s = new Socket(InetAddress.getByName("localhost"),8000);//端口号要一致。
- 
-             OutputStream os = s.getOutputStream();  
-             os.write("Hello World!".getBytes());  
- 
-             InputStream is = s.getInputStream();  
- byte [] buf = newbyte[100];  
- int len = is.read(buf);  
-             System.out.println(new String(buf,0,len));  
- 
-             os.close();  
-             is.close();  
-             s.close();  
- 
-         }  
- catch(Exception e)  
-         {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
三、基于UDP的socket编程。
1、UDP协议定义
UDP协议的全称是用户数据报，在网络中它与TCP协议一样用于处理数据包。在OSI模型中，在第四层——传输层，处于IP协议的上一层。UDP有不提供数据报分组、组装和不能对数据包的排序的缺点，也就是说，当报文发送之后，是无法得知其是否安全完整到达的。
2、使用UDP的原因
它不属于连接型协议，因而具有资源消耗小，处理速度快的优点，所以通常音频、视频和普通数据在传送时使用UDP较多，因为它们即使偶尔丢失一两个数据包，也不会对接收结果产生太大影响。比如我们聊天用的ICQ和OICQ就是使用的UDP协议。在选择使用协议的时候，选择UDP必须要谨慎。在网络质量令人不十分满意的环境下，UDP协议数据包丢失会比较严重。
3、在Java中使用UDP协议编程的相关类
（1）. InetAddress
用于描述和包装一个Internet IP地址。有如下方法返回实例：
getLocalhost()：返回封装本地地址的实例。
getAllByName(String host)：返回封装Host地址的InetAddress实例数组。
getByName(String host)：返回一个封装Host地址的实例。其中，Host可以是域名或者是一个合法的IP地址。
InetAddress.getByAddress(addr)：根据地址串返回InetAddress实例。
InetAddress.getByAddress(host, addr)：根据主机地符串和地址串返回InetAddress实例。
（2）. DatagramSocket
用于接收和发送UDP的Socket实例。该类有3个构造函数：
DatagramSocket()：通常用于客户端编程，它并没有特定监听的端口，仅仅使用一个临时的。程序会让[操作系统](http://lib.csdn.net/base/operatingsystem)分配一个可用的端口。
DatagramSocket(int port)：创建实例，并固定监听Port端口的报文。通常用于服务端
DatagramSocket(int port, InetAddress localAddr)：这是个非常有用的构建器，当一台机器拥有多于一个IP地址的时候，由它创建的实例仅仅接收来自LocalAddr的报文。
DatagramSocket具有的主要方法如下：
1）receive(DatagramPacket d)：接收数据报文到d中。receive方法产生一个“阻塞”。“阻塞”是一个专业名词，它会产生一个内部循环，使程序暂停在这个地方，直到一个条件触发。
2）send(DatagramPacket dp)：发送报文dp到目的地。
3）setSoTimeout(int timeout)：设置超时时间，单位为毫秒。
4）close()：关闭DatagramSocket。在应用程序退出的时候，通常会主动释放资源，关闭Socket，但是由于异常地退出可能造成资源无法回收。所以，应该在程序完成时，主动使用此方法关闭Socket，或在捕获到异常抛出后关闭Socket。
（3）. DatagramPacket
用于处理报文，它将Byte数组、目标地址、目标端口等数据包装成报文或者将报文拆卸成Byte数组。应用程序在产生数据包是应该注意，TCP/IP规定数据报文大小最多包含65507个，通常主机接收548个字节，但大多数平台能够支持8192字节大小的报文。DatagramPacket类的构建器共有4个：
DatagramPacket(byte[] buf, int length)：将数据包中Length长的数据装进Buf数组，一般用来接收客户端发送的数据。
DatagramPacket(byte[] buf, int offset, int length)：将数据包中从Offset开始、Length长的数据装进Buf数组。
DatagramPacket(byte[] buf, int length, InetAddress clientAddress, int clientPort)：从Buf数组中，取出Length长的数据创建数据包对象，目标是clientAddress地址，clientPort端口,通常用来发送数据给客户端。
DatagramPacket(byte[] buf, int offset, int length, InetAddress clientAddress, int clientPort)：从Buf数组中，取出Offset开始的、Length长的数据创建数据包对象，目标是clientAddress地址，clientPort端口，通常用来发送数据给客户端。
主要的方法如下：
1）getData(): 从实例中取得报文的Byte数组编码。
2）setDate(byte[] buf)：将byte数组放入要发送的报文中。
![](https://img-blog.csdn.net/20160304151039226)
发送端代码如下：
**[java]**[view
 plain](http://blog.csdn.net/hudashi/article/details/50802544#)[copy](http://blog.csdn.net/hudashi/article/details/50802544#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1597159/fork)
- import java.net.*;  
- import java.io.*;  
- /*
- *发送端, 相当于客户端。
- */
- class SocketUDPSend  
- {  
- publicstaticvoid main(String[]args)  
-     {  
-             sed();  
-     }  
- 
- publicstaticvoid sed()  
-     {  
- try
-         {  
-             DatagramSocket ds = new DatagramSocket();  
-             String str = "haha, my name is xuneng!";  
-             DatagramPacket dp = new DatagramPacket(str.getBytes(),0,str.length(),  
-                                                 InetAddress.getByName("localhost"),8600);//发送给本机的地址，端口为8600
-                     ds.send(dp);  
- 
- //演示接受返回回来的数据。
- byte[] buf = newbyte[100];  
-             DatagramPacket dp2 = new DatagramPacket(buf,100);//字节数组，长度
-             ds.receive(dp2);  
-             System.out.println(new String(buf,0,dp2.getLength()));  
-             ds.close();  
-         }  
- catch(Exception e)  
-         {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
接收代码如下：
**[java]**[view
 plain](http://blog.csdn.net/hudashi/article/details/50802544#)[copy](http://blog.csdn.net/hudashi/article/details/50802544#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1597159/fork)
- import java.net.*;    
- import java.io.*;    
- /* 
- *接受端，也就是服务器端。一直在监听端口。 
- */
- class SocketUDPRecv     
- {    
- publicstaticvoid main(String[]args)    
-     {    
-             recv();    
-     }    
- 
- publicstaticvoid recv()        
-     {    
- try
-         {    
-             DatagramSocket ds = new DatagramSocket(8600);    
- byte [] buf = newbyte[100];    
-             DatagramPacket dp = new DatagramPacket(buf,100);    
-             ds.receive(dp);     
-             System.out.println(new String(buf,0,dp.getLength()));    
- 
- //演示给发送端返回数据,需要发送端去接受  
-             String str = "Yes , I received!";    
-             DatagramPacket dp1 = new DatagramPacket(str.getBytes(),str.length(),    
-                                               dp.getAddress(),dp.getPort());    
-             ds.send(dp1);    
- 
-             ds.close();         
-         }    
- catch(Exception e)    
-         {    
-             e.printStackTrace();    
-         }    
-     }    
- }   
结束
