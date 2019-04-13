
# java入门教程-12.1Java网络编程之IP地址和InetAddress类 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月29日 07:45:34[seven-soft](https://me.csdn.net/softn)阅读数：190个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



Java语言的优势之一是Java程序能访问网络资源。Java提供一系列的类支持Java程序访问网络资源。
## TCP/IP协议和IP地址
为了进行网络通信,通信双方必须遵守通信协议.目前最广泛使用的是TCP/IP协议,它是Internet中各方所遵循的公共协议.TCP(Transport Control Protocol)是一种传输控制协议,IP(Internet
 Protocol)是一种网际协议，TCP/IP代表这两个协议的。
TCP/IP分为四个层次：网络接口层：负责接收和发送物理帧；
网络层：负责相邻节点之间的通信；
传输层：负责起点到终点的通信；
应用层：提供诸如文件传输、电子邮件等应用程序。
TCP协议将任何网络信息传输当作信息流。例如，机器A上的一个长报文发送到机器B，发送端A需要将数据分片，把一片片数据分别打包发送。数据包有一个头，指明该数据包发往何处、凶中数据在接收序列中所处的位置。每个包都按照IP地址提供的目的地从一个台机器传送到另一台机器，或从一个网络节点传送到另一个网络节点。在接收端B，这些数据包都能够按照正确的顺序重新组装起来。
TCP/IP协议是一个协议族，由一组协议组成，主要包含以下更具体的协议：Telnet（远程登录）：允许一台计算机用户登录到另一台远程计算机上，使远程操作如同在本地计算机上操作一样。
FTP（File Transfer protocol，文件传输协议）：允许用户将远程主机上的文件复制到自己的计算机上。
SMTP（simple Mail Transfer Protocol，简单邮件传输协议）：用于传输电子邮件。
NFS（Network file Server，网络文件服务器）：使多台计算机透明地访问彼此的目录。
HTTP：一种超文本传输协议，它是基于TCP/IP协议的，是WWW浏览器和服务器之间应用层的通信协议。HTTP是一种通用、无状态、面向对象的协议。HTTP会话（事务）包括四个步骤：连接（Connection）、请求（Request）、应答（Response）和关闭（Close）。
Java语言可编写低层的网络应用。例如，传输文件，建立邮件控制器，处理网络数据等。Java语言支持的Internet协议有ftp、telnet、www等，支持网络通信的软件都在java.net包中，例如，java.net.ftp、java.net.www等。
IP地址用于指明因特网上的一台计算机在网络中的地址，用32位二进制代码表示一个网络地址。地址分A、B、C、D、E五类，常用的是A、B、C三类：A（1.0.0.0-126.255.255.255）：0,7位网络号，后24位为主机号；
B（128.0.0.0-191.255.255.255）：10,14位网络号,后16位为主机号；
C（192.0.0.0-223.255.255.255）：110,21位网络号,后8位为主机号；
D（224.0.0.0-239.255.255.255）：1110,28位多点广播组标号；
E（240.0.0.0-254.255.255.255）：1111,保留试验使用。
通常，IP地址用四段十进制数表示（8位一段）。例如：
116.255.226.187
或用文字域名表示。例如：
www.weixueyuan.net
在因特网上，域名服务器（Domain Name Server,DNS）执行文字名称到二进制网络地址的映射。
## InetAddress类
Java.net包中有InetAddress类的定义，InetAddress类的对象用于IP地址和域名，该类提供以下方法：getByName(String s)：获得一个InetAddress 类的对象，该对象中含有主机的IP地址和域名，该对象用如下格式表示它包含的信息：www.sina.com.cn/202.108.37.40；
String getHostName()：获取InetAddress对象的域名；
String getHostAddress()：获取InetAddress对象的IP地址；
getLocalHost()：获得一个InetAddress对象，该对象含有本地机的域名和IP地址。
【例 13-1】 说明Inetaddress类的用法的应用程序。程序演示如何获取www.weixueyuan.net的域名和IP地址。
Import java.net.*;
Class Example10_1{
Public static void main(String args[]){
Try{ //以下代码通过域名建立InetAddress对象：
InetAddress addr = InetAddress.getByname(“www.weixueyuan.net”);
String domainName = addr.getHostName();//获得主机名
String IPName = addr.getHostAddress();//获得IP地址
System.out.println(domainName);
System.out.println(IPName);
}catch(UnknownHostException e){
e.printStackTrace();
}
}
}
运行结果为：
www.weixueyuan.net
116.255.226.187

