# TCP与UDP初探 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年04月21日 21:42:41[boonya](https://me.csdn.net/boonya)阅读数：1138
个人分类：[PROTOCOL](https://blog.csdn.net/boonya/article/category/1400121)









前言：我最讨厌的分享方式是一段代码直接甩上来，看上去没头没脑的；标题初看确实很丰满，结果内容却很可怜，不能达到总结记忆的目的。

**1、什么是TCP、UDP**

TCP (Transmission Control Protocol)和UDP(User Datagram Protocol)协议属于传输层协议。其中TCP提供IP环境下的数据可靠传输，

它提供的服务包括数据流传送、可靠性、有效流控、全双工操作和多路复用。通过面向连接、端到端和可靠的数据包发送。通俗说，

它是事先为所发送的数据开辟出连接好的通道，然后再进行数据发送；而UDP则不为IP提供可络文件系统）、SNMP（简单网络管理协议）

、DNS（主域名称系统）、TFTP（通用文件传输协议）等。两种协议都是传输层协议，为应用层提供信息载体。TCP协议是基于连接的可

靠协议，有流量控制和差错控制，也正因为有可靠性的保证和控制手段，所以传输效率比UDP低；UDP协议是基于无连接的不可靠协议，

没有控制手段，仅仅是将数据发送给对方，因此效率比TCP要高。


**2、TCP、UDP的适用场景**

基于1所述述特性，不难得到结论，TCP协议适用于对效率要求相对低，但对准确性要求相对高的场景下，或者是有一种连接概念的场景下；

而UDP协议适用于对效率要求相对高，对准确性要求相对低的场景。

TCP一般用于文件传输（FTP HTTP 对数据准确性要求高，速度可以相对慢），发送或接收邮件（POP IMAP SMTP 对数据准确性要求高，非紧急应用），

远程登录（TELNET SSH 对数据准确性有一定要求，有连接的概念）等等；UDP一般用于即时通信（QQ聊天 对数据准确性和丢包要求比较低，但速度必须快），

在线视频（RTSP 速度一定要快，保证视频连续，但是偶尔花了一个图像帧，人们还是能接受的），网络语音电话（VoIP 语音数据包一般比较小，

需要高速发送，偶尔断音或串音也没有问题）等等。

**3、TCP与UDP实例3.1、TCP实例**

TCP客户端程序：

```java
package com.boonya.tcp.client;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.Socket;

public class TCPClient {
 public static final String IP = "192.168.0.157";
 public static final int port = 8962;
 private Socket s;

 public TCPClient() throws IOException {
  try {
   s = new Socket(IP, port);

  } catch (IOException e) {

   e.printStackTrace();
  }
 }

 public void setConnection() throws IOException {

  InputStream is;

  try {
   is = s.getInputStream();
   BufferedReader br = new BufferedReader(new InputStreamReader(is));

   System.out.println(br.readLine());

  } catch (IOException e) {

   e.printStackTrace();
  }

 }

 public static void main(String args[]) throws IOException {

  TCPClient mc = new TCPClient();
  mc.setConnection();

 }

}
```

TCP服务端程序：

```java
package com.boonya.tcp.server;
import java.io.IOException;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socketpublic class TCPServer {
 // 建立ServerSocket，并设置其端口号
 private ServerSocket ss;

 public static final int port = 8962;

 public TCPServer() {
  try {
   ss = new ServerSocket(port);
  } catch (IOException e) {
   e.printStackTrace();
  }
 }

 public void setConnection() throws IOException {
  // 建立服务器端的Socket
  Socket s;
  OutputStream os;
  try { // ServerSocke.accept()t返回一个Socket对象

   s = ss.accept();
   os = s.getOutputStream();
   os.write("hello".getBytes());
   os.close();
   s.close();
  } catch (IOException e) {

   e.printStackTrace();
  }

 }

 public static void main(String[] args) throws IOException {

  TCPServer ms = new TCPServer();
  ms.setConnection();

 }

}
```

**3.2、UDP实例**

UDP服务端程序：

```java
package com.boonya.udp.server;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class UDPServer {
 
 public static void main(String[] rrgs) throws IOException {
  
  DatagramSocket socket = new DatagramSocket(14567);
  while (true) {
   
   byte data[] = new byte[1024];
   // 创建一个空的DatagramPacket对象
   DatagramPacket packet = new DatagramPacket(data, data.length);
   // 使用receive方法接收客户端所发送的数据，
   // 如果客户端没有发送数据，该进程就停滞在这里
   socket.receive(packet);
   String result = new String(packet.getData(), packet.getOffset(),
     packet.getLength());
   System.out.println("result--->" + result);
  }

 }

}
```

UDP客户端程序：

```java
package com.boonya.udp.client;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UDPClient {
 
 public static void main(String[] args) {
  try {
   // 首先创建一个DatagramSocket对象
   DatagramSocket socket = new DatagramSocket();
   // 创建一个InetAddree
   InetAddress serverAddress = InetAddress.getByName("192.168.0.157");
   String str = "hello"; // 这是要传输的数据
   byte data[] = str.getBytes(); // 把传输内容分解成字节
   // 创建一个DatagramPacket对象，并指定要讲这个数据包发送到网络当中的哪个、地址，以及端口号
   DatagramPacket packet = new DatagramPacket(data, data.length,
     serverAddress, 14567);
   // 调用socket对象的send方法，发送数据
   socket.send(packet);
  } catch (Exception e) {
   // TODO Auto-generated catch block
   e.printStackTrace();
  }
 }

}
```







