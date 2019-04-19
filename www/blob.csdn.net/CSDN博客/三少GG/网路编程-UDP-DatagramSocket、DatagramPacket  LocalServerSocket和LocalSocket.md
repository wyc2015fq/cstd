# 网路编程-UDP-DatagramSocket、DatagramPacket ||LocalServerSocket和LocalSocket - 三少GG - CSDN博客
2012年04月09日 15:11:27[三少GG](https://me.csdn.net/scut1135)阅读数：3754
JavaUDP通信(DatagramPacket类、DatagramSocket类、InetAddress类)
2008-03-2409:55
**一、****DatagramPacket****类：**
如果把DatagramSocket比作创建的港口码头，那么DatagramPacket就是发送和接收数据的集装箱。
构造函数：一个用来接收数据，一个用来发送数据
publicDatagramPacket(byte[] buf,int length) //接收数据
构造DatagramPacket用来接收长度为 length的包。 
publicDatagramPacket(byte[] buf,int length,InetAddress address,intport)
构造数据报文包用来把长度为length的包传送到指定宿主的指定的端口号。
getAddress()
返回接收或发送此数据报文的机器的IP地址。 
getData()
返回接收的数据或发送出的数据。
getLength()
返回发送出的或接收到的数据的长度。
getPort()
返回接收或发送该数据报文的远程主机端口号。
**二、****DatagramSocket****类**
此类表示用来发送和接收数据报包的套接字。数据报套接字是包投递服务的发送或接收点。
DatagramSocket(intport)    创建数据报套接字并将其绑定到本地主机上的指定端口。
DatagramSocket(intport, InetAddress laddr)    创建数据报套接字，将其绑定到指定的本地地址。
receive(DatagramPacketp) 
从此套接字接收数据报包。
voidsend(DatagramPacket p) 
从此套接字发送数据报包。
bind(SocketAddressaddr) 
将此 DatagramSocket绑定到特定的地址和端口。
voidclose() 
关闭此数据报套接字。
voidconnect(InetAddress address, int port) 
将套接字连接到此套接字的远程地址。
voidconnect(SocketAddress addr) 
将此套接字连接到远程套接字地址（IP地址 +端口号）。 
voiddisconnect() 
断开套接字的连接。
getInetAddress()
返回此套接字连接的地址。
InetAddressgetLocalAddress() 
获取套接字绑定的本地地址。
**三、****InetAddress****类**
InetAddress用于表示计算机IP地址的一个类，而在日常应用中的IP地址用"192.168.0.1",
"[WWW.it315.org](http://www.it315.org/)"等字符串格式表示的。
getByName方法
getHostAddress方法
**四、****UDP****通信示例代码：**
发送端代码UdpSend.java：
import java.net.*;
publicclass UdpSend {
         public static void main(String[] args) throwsException{
           DatagramSocket ds=new DatagramSocket();
          String strInfo="Hello Taohx";
          ds.send(newDatagramPacket(strInfo.getBytes(),strInfo.length(),
                                   InetAddress.getByName("59.64.157.93"),3000));
          ds.close();            
      }
}
接收代码UdpRecv.java：
importjava.net.*;
public class UdpRecv {
      public static void main(String[] args) throwsException{
          DatagramSocket ds=new DatagramSocket(3000);
          byte [] buf=new byte[1024];
          DatagramPacket dp=new DatagramPacket(buf,1024);
          ds.receive(dp);
         String strInfo= newString(dp.getData(),0,dp.getLength())+
           " from"+dp.getAddress().getHostAddress()+":"+dp.getPort();
          System.out.println(strInfo);
          ds.close();
      }
}
**注意：**若发送中文信息时,因为中文字符占两个字节，故发送代码ds.send要修改如下才可正确地发送中文信息，
ds.send(newDatagramPacket(strInfo.getBytes(),**strInfo.getBytes().length,**
                                  InetAddress.getByName("59.64.157.93"),3000));
***********************************************************************************************************
**在****Java****中操纵****UDP**
　　使用位于JDK中Java.net包下的DatagramSocket和DatagramPacket类，可以非常方便地控制用户数据报文。
　　在描述它们之前，必须了解位于同一个位置的InetAddress类。InetAddress实现了Java.io.Serializable接口，不允许继承。它用于描述和包装一个InternetIP地址，通过三个方法返回InetAddress实例：
getLocalhost()：返回封装本地地址的实例。
getAllByName(Stringhost)：返回封装Host地址的InetAddress实例数组。
getByName(Stringhost)：返回一个封装Host地址的实例。其中，Host可以是域名或者是一个合法的IP地址。
DatagramSocket类用于创建接收和发送UDP的Socket实例。和Socket类依赖SocketImpl类一样，DatagramSocket类的实现也依靠专门为它设计的DatagramScoketImplFactory类。DatagramSocket类有3个构建器：
DatagramSocket()：创建实例。这是个比较特殊的用法，通常用于客户端编程，它并没有特定监听的端口，仅仅使用一个临时的。
DatagramSocket(intport)：创建实例，并固定监听Port端口的报文。
DatagramSocket(intport, InetAddresslocalAddr)：这是个非常有用的构建器，当一台机器拥有多于一个IP地址的时候，由它创建的实例仅仅接收来自LocalAddr的报文。
值得注意的是，在创建DatagramSocket类实例时，如果端口已经被使用，会产生一个SocketException的异常抛出，并导致程序非法终止，这个异常应该注意捕获。DatagramSocket类最主要的方法有4个：
Receive(DatagramPacketd)：接收数据报文到d中。receive方法产生一个“阻塞”。
Send(DatagramPacketd)：发送报文d到目的地。
SetSoTimeout(inttimeout)：设置超时时间，单位为毫秒。
Close()：关闭DatagramSocket。在应用程序退出的时候，通常会主动释放资源，关闭Socket，但是由于异常地退出可能造成资源无法回收。所以，应该在程序完成时，主动使用此方法关闭Socket，或在捕获到异常抛出后关闭Socket。
“阻塞”是一个专业名词，它会产生一个内部循环，使程序暂停在这个地方，直到一个条件触发。
DatagramPacket类用于处理报文，它将Byte数组、目标地址、目标端口等数据包装成报文或者将报文拆卸成Byte数组。应用程序在产生数据包是应该注意，TCP/IP规定数据报文大小最多包含65507个，通常主机接收548个字节，但大多数平台能够支持8192字节大小的报文。DatagramPacket类的构建器共有4个：
DatagramPacket(byte[]buf, int length, InetAddress addr, intport)：从Buf数组中，取出Length长的数据创建数据包对象，目标是Addr地址，Port端口。
DatagramPacket(byte[]buf, int offset, int length, InetAddress address, intport)：从Buf数组中，取出Offset开始的、Length长的数据创建数据包对象，目标是Addr地址，Port端口。
DatagramPacket(byte[]buf, int offset, int length)：将数据包中从Offset开始、Length长的数据装进Buf数组。
DatagramPacket(byte[]buf, int length)：将数据包中Length长的数据装进Buf数组。
DatagramPacket类最重要的方法就是getData()了，它从实例中取得报文的Byte数组编码。
简单的实例说明
{接收数据的服务器}
byte[] buf = new byte[1000];
DatagramSocket ds= new DatagramSocket(12345);
//开始监视12345端口
DatagramPacketip = new DatagramPacket(buf, buf.length);
//创建接收数据报的实例
while(true)
{
ds.receive(ip);
//阻塞，直到收到数据报后将数据装入IP中
System.out.println(newString(buf));
}
{发送数据的客户端}
InetAddress target =InetAddress.getByName(“www.xxx.com“);
//得到目标机器的地址实例
DatagramSocketds = new DatagramSocket(9999);
//从9999端口发送数据报
Stringhello = “Hello, I am come in!”;
//要发送的数据    
byte[]buf = hello.getBytes();
//将数据转换成Byte类型
op= new DatagramPacket(buf, buf.length, target,12345);
//将BUF缓冲区中的数据打包
ds.send(op);
//发送数据
ds.close();
//关闭连接
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DatagramSocket
**17.4.2 使用DatagramSocket发送、接收数据**
DatagramSocket本身只是码头，不维护状态，不能产生IO流，它的唯一作用就是接收和发送数据报，Java使用DatagramPacket来代表数据报，DatagramSocket接收和发送的数据都是通过DatagramPacket对象完成的。
先看一下DatagramSocket的构造器：
DatagramSocket()：创建一个DatagramSocket实例，并将该对象绑定到本机默认IP地址、本机所有可用端口中随机选择的某个端口。
DatagramSocket(int prot)：创建一个DatagramSocket实例，并将该对象绑定到本机默认IP地址、指定端口。
DatagramSocket(int port, InetAddress laddr)：创建一个DatagramSocket实例，并将该对象绑定到指定IP地址、指定端口。
通过上面三个构造器中任意一个构造器即可创建一个DatagramSocket实例，通常在创建服务器时，我们创建指定端口的 DatagramSocket实例——这样保证其他客户端可以将数据发送到该服务器。一旦得到了DatagramSocket实例之后，就可以通过如下两 个方法来接收和发送数据：
receive(DatagramPacket p)：从该DatagramSocket中接收数据报。
send(DatagramPacket p)：以该DatagramSocket对象向外发送数据报。
从上面两个方法可以看出，使用DatagramSocket发送数据报时，DatagramSocket并不知道将该数据报发送到哪里，而是由 DatagramPacket自身决定数据报的目的。就像码头并不知道每个集装箱的目的地，码头只是将这些集装箱发送出去，而集装箱本身包含了该集装箱的 目的地。
当Client/Server程序使用UDP协议时，实际上并没有明显的服务器和客户端，因为两方都需要先建立一个DatagramSocket对 象，用来接收或发送数据报，然后使用DatagramPacket对象作为传输数据的载体。通常固定IP、固定端口的DatagramSocket对象所 在的程序被称为服务器，因为该DatagramSocket可以主动接收客户端数据。
下面看一下DatagramPacket的构造器：
DatagramPacket(byte buf[],int length)：以一个空数组来创建DatagramPacket对象，该对象的作用是接收DatagramSocket中的数据。
DatagramPacket(byte buf[], int length, InetAddress addr, int port)：以一个包含数据的数组来创建DatagramPacket对象，创建该DatagramPacket时还指定了IP地址和端口——这就决定了 该数据报的目的。
DatagramPacket(byte[] buf, int offset, int length)：以一个空数组来创建DatagramPacket对象，并指定接收到的数据放入buf数组中时从offset开始，最多放length个字节。
DatagramPacket(byte[] buf, int offset, int length, InetAddress address, int port)：创建一个用于发送的DatagramPacket对象，也多指定了一个offset参数。
在接收数据前，应该采用上面的第一个或第三个构造器生成一个DatagramPacket对象，给出接收数据的字节数组及其长度。然后调用 DatagramSocket 的方法receive()等待数据报的到来，receive()将一直等待（也就是说会阻塞调用该方法的线程），直到收到一个数据报为止。如下代码所示：
//创建接受数据的DatagramPacket对象
DatagramPacket packet=new DatagramPacket(buf, 256);
//接收数据
socket.receive(packet);
发送数据之前，调用第二个或第四个构造器创建DatagramPacket对象，此时的字节数组里存放了想发送的数据。除此之外，还要给出完整的目 的地址，包括IP地址和端口号。发送数据是通过DatagramSocket的方法send()实现的，send()根据数据报的目的地址来寻径以传递数 据报。如下代码所示：
//创建一个发送数据的DatagramPacket对象
DatagramPacket packet = new DatagramPacket(buf, length, address, port);
//发送数据报
socket.send(packet);
当我们使用DatagramPacket来接收数据时，会感觉DatagramPacket设计得过于烦琐。对于开发者而言，只关心该 DatagramPacket能放多少数据，而DatagramPacket是否采用字节数组来存储数据完全不想关心。但Java要求创建接收数据用的 DatagramPacket时，必须传入一个空的字节数组，该数组的长度决定了该DatagramPacket能放多少数据，这实际上暴露了 DatagramPacket的实现细节。接着**DatagramPacket又提供了一个getData()方法，该方法又可以返回 DatagramPacket对象里封装的字节数组，该方法更显得有些多余：如果程序需要获取DatagramPacket里封装的字节数组，直接访问传 给 DatagramPacket构造器的字节数组实参即可，无须调用该方法。**
当服务器（也可以客户端）接收到一个DatagramPacket对象后，如果想向该数据报的发送者“反馈”一些信息，但由于UDP是面向非连接 的，所以接收者并不知道每个数据报由谁发送过来，但程序可以调用DatagramPacket的如下三个方法来获取发送者的IP和端口：
InetAddress getAddress()：返回某台机器的 IP 地址，当程序准备发送次数据报时，该方法返回此数据报的目标机器的IP地址；当程序刚刚接收到一个数据报时，该方法返回该数据报的发送主机的IP地址。
int getPort()：返回某台机器的端口，当程序准备发送此数据报时，该方法返回此数据报的目标机器的端口；当程序刚刚接收到一个数据报时，该方法返回该数据报的发送主机的端口。
SocketAddress getSocketAddress()：返回完整SocketAddress，通常由IP地址和端口组成。当程序准备发送此数据报时，该方法返回此数据报 的目标SocketAddress；当程序刚刚接收到一个数据报时，该方法返回该数据报是源SocketAddress。
上面getSocketAddress方法的返回值是一个SocketAddress对象，该对象实际上就是一个IP地址和一个端口号，也就是说 SocketAddress对象封装了一个InetAddress对象和一个代表端口的整数，所以使用SocketAddress对象可以同时代表IP地 址和端口。
下面程序使用DatagramSocket实现Server/Client结构的网络通信程序，本程序的服务器端使用循环1000次来读取DatagramSocket中的数据报，每当读到内容之后便向该数据报的发送者送回一条信息。服务器端代码如下：
程序清单：codes/17/17-4/UdpServer.java
```java
public class UdpServer {
	 public static final int PORT = 30000; 
	 //定义每个数据报的最大大小为4K 
	 private static final int DATA_LEN = 4096; 
	 //定义该服务器使用的DatagramSocket 
	 private DatagramSocket socket = null; 
	 //定义接收网络数据的字节数组 
	 byte[] inBuff = new byte[DATA_LEN]; 
	 //以指定字节数组创建准备接受数据的DatagramPacket对象 
	 private DatagramPacket inPacket =  new DatagramPacket(inBuff , inBuff.length); 
	 //定义一个用于发送的DatagramPacket对象 
	 private DatagramPacket outPacket; 
	 //定义一个字符串数组，服务器发送该数组的的元素 
	 String[] books = new String[] { "轻量级J2EE企业应用实战", "基于J2EE的Ajax宝典", "Struts2权威指南", "ROR敏捷开发最佳实践" }; 
	 
	 public void init()throws IOException { 
		 try { 
			 //创建DatagramSocket对象 
			 socket = new DatagramSocket(PORT); 
			 //采用循环接受数据 
			 for (int i = 0; i < 1000 ; i++ ) 
			 { 
				 //读取Socket中的数据，读到的数据放在inPacket所封装的字节数组里。
				 socket.receive(inPacket); 
				 //判断inPacket.getData()和inBuff是否是同一个数组 
				 System.out.println(inBuff == inPacket.getData()); 
				 //将接收到的内容转成字符串后输出 
				 System.out.println(new String(inBuff , 0 , inPacket.getLength())); 
				 //从字符串数组中取出一个元素作为发送的数据 
				 byte[] sendData = books[i % 4].getBytes(); 
				 //以指定字节数组作为发送数据、以刚接受到的DatagramPacket的 
				 //源SocketAddress作为目标SocketAddress创建DatagramPacket。 
				 outPacket = new DatagramPacket(sendData , sendData.length , inPacket.getSocketAddress()); 
				 //发送数据
				 socket.send(outPacket);  
			  }
		 }
		 //使用finally块保证关闭资源 
		 finally { 
			 if (socket != null) { 
				 socket.close(); 
			 } 
		 }
	} 
	 public static void main(String[] args)  throws IOException {
		 
		 new UdpServer().init(); 
	}
}
```
上面程序中粗体字代码就是使用DatagramSocket发送、接收DatagramPacket的关键代码，该程序可以接受1000个客户端发送过来的数据。
程序客户端代码也与此类似，客户端采用循环不断地读取用户键盘输入，每当读到用户输入内容后就将该内容封装成DatagramPacket数据报， 再将该数据报发送出去；接着把DatagramSocket中的数据读入接收用的DatagramPacket中（实际上是读入该 DatagramPacket所封装的字节数组中）。客户端代码如下：
程序清单：codes/17/17-4/UdpClient.java
```java
public class UdpClient { 
	//定义发送数据报的目的地 
	public static final int DEST_PORT = 30000; 
	public static final String DEST_IP = "127.0.0.1"; 
	//定义每个数据报的最大大小为4K 
	private static final int DATA_LEN = 4096; 
	//定义该客户端使用的DatagramSocket 
	private DatagramSocket socket = null; 
	//定义接收网络数据的字节数组 byte[] 
	inBuff = new byte[DATA_LEN]; 
	//以指定字节数组创建准备接受数据的DatagramPacket对象 
	private DatagramPacket inPacket =  new DatagramPacket(inBuff , inBuff.length); 
	//定义一个用于发送的DatagramPacket对象 
	private DatagramPacket outPacket = null; 
	
	public void init() throws IOException {
		try { 
			//创建一个客户端DatagramSocket，使用随机端口 
			socket = new DatagramSocket(); 
			//初始化发送用的DatagramSocket，它包含一个长度为0的字节数组 
			outPacket = new DatagramPacket(new byte[0] , 0 , InetAddress.getByName(DEST_IP) , DEST_PORT); 
			//创建键盘输入流 
			Scanner scan = new Scanner(System.in); 
			//不断读取键盘输入 
			while(scan.hasNextLine()) { 
				//将键盘输入的一行字符串转换字节数组 
				byte[] buff = scan.nextLine().getBytes(); 
				//设置发送用的DatagramPacket里的字节数据 
				outPacket.setData(buff); 
				//发送数据报 
				socket.send(outPacket); 
				//读取Socket中的数据，读到的数据放在inPacket所封装的字节数组里。 
				socket.receive(inPacket); 
				System.out.println(new String(inBuff , 0 ,  inPacket.getLength())); 				
			}
	    }
			//使用finally块保证关闭资源 
		finally { 
			if (socket != null) {
				socket.close(); 
			}
		}	
	}
	public static void main(String[] args)  throws IOException {
		new UdpClient().init();  
	}
}
```
上面程序的粗体字代码同样也是通过DatagramSocket发送、接收DatagramPacket的关键代码，这些代码与服务器的代码基本相 似。而客户端与服务器端的唯一区别在于：服务器所在IP地址、端口是固定的，所以客户端可以直接将该数据报发送给服务器，而服务器则需要根据接收到的数据 报来决定将“反馈”数据报的目的地。
读者可能会发现，使用DatagramSocket进行网络通信时，服务器端无须、也无法保存每个客户端的状态，客户端把数据报发送到服务器后，完全有可能立即退出。但不管客户端是否退出，服务器无法知道客户端的状态。
当使用UDP协议时，如果想让一个客户端发送的聊天信息可被转发到其他所有客户端则比较困难，可以考虑在服务器使用Set来保存所有客户端信息，每 当接收到一个客户端的数据报之后，程序检查该数据报的源SocketAddress是否在Set集合中，如果不在就将该SocketAddress添加到 该Set集合中，但这样一来又涉及一个问题：可能有些客户端发送一个数据报之后永久性地退出了程序，但服务器端还将该客户端的SocketAddress
 保存在Set集合中……总之，这种方式需要处理的问题比较多，编程比较烦琐。
**幸好Java为UDP协议提供了MulticastSocket类，通过该类 可以轻松实现多点广播。**
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [网路编程-UDP-DatagramSocket、DatagramPacket](http://xiumuyigen-sina-com.iteye.com/blog/909996)
UDP是一个[无连接协议](http://xiumuyigen-sina-com.iteye.com/view/732127.htm)，传输数据之前源端和终端不建立连接，当它想传送时就简单地去抓取来自应用程序的数据，并尽可能快地把它扔到网络上。在发送端，UDP传送数据的速度仅仅是受应用程序生成数据的速度、计算机的能力和传输带宽的限制；在接收端，UDP把每个消息段放在队列中，应用程序每次从队列中读一个消息段。
一个互相发送、接受的 简单程序：
（部分代码：具体请下载下面文件）
--------------发送程序部分代码------------
while (true) {
   String str = getInputStr();
   try {
    DatagramSocket ds = new DatagramSocket();
    DatagramPacket dp = new DatagramPacket(str.getBytes(), 0, str
      .getBytes().length, InetAddress
      .getByName("192.168.1.199"), 6000);
    ds.send(dp);
    ds.close();
   } catch (Exception e) {
    // TODO Auto-generated catch block
    e.printStackTrace();
   }
   if ("quit".equals(str)) {
    System.out.println("本第一端发送退出。。。");
    break;
   }
  }
----接受程序部分代码-----------
while (true) {
    try {
     DatagramSocket ds = new DatagramSocket(3000);
     byte[] b = new byte[1024];
     DatagramPacket dp = new DatagramPacket(b, 1024);
     ds.receive(dp);
     String s = new String(dp.getData(), 0, dp.getLength());
     if ("quit".equals(s)) {
      System.out.println("第二端发送已退出..。");
      System.out.println("本第一端接收程序退出..");
      break;
     }
     String rece = s + "    From Receive --"
       + dp.getAddress().getHostAddress() + " : "
       + dp.getPort();
     System.out.println(rece);
     ds.close();
    } catch (Exception e) {
     // TODO Auto-generated catch block
     e.printStackTrace();
    }
   }
**由于每个端都具有发送接受功能   所以要采用到线程。**详细见下载资料中。
- [UDP相互发送接受.rar](http://dl.iteye.com/topics/download/e668f228-178f-3c8a-b446-d5aa51f58fb6) (1.9 KB)
- 
下载次数: 7
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# [DatagramSocket和DatagramPacket的用法](http://www.shouyanwang.org.sixxs.org/thread-43-1-1.html)
下面给大家介绍下android中的DatagramSocket和DatagramPacket的用法。
关于数据报包的相关知识，请大家看我的另一篇文章:
[http://www.shouyanwang.org.sixxs.org/thread-4-1-1.html](http://www.shouyanwang.org.sixxs.org/thread-4-1-1.html)
DatagramSocket的API文档解释是:
此类表示用来发送和接收数据报包的套接字。  数据报套接字是包投递服务的发送或接收点。每个在数据报套接字上发送或接收的包都是单独编址和路由的。从一台机器发送到另一台机器的多个包可能选择不同的路由，也可能按不同的顺序到达.
DatagramPacket在API文档中的解释是:
此类表示数据报包。  **数据报包用来实现****无连接****包投递服务**。每条报文仅根据该包中包含的信息从一台机器路由到另一台机器。从一台机器发送到另一台机器的多个包可能选择不同的路由，也可能按不同的顺序到达。不对包投递做出保证。 
API文档解释咱们先看下，下面跟大家说下我实际操作得出的结论:
DatagramSocket其实就是一个发射器，专门用来发射DatagramPacket,DatagramPacket自己的内部就有一个数据缓冲区，咱们可以直接操作这个缓冲区的,像这个缓冲区中写数据，读数据等操作。
下面贴一段代码，实现的功能就是通过的IP和PORT给自己传递一些数据：
数据发送：
> 
- 
DatagramSocket udpSocket = new DatagramSocket();
- 
byte[]outBuf = "this is a text".getBytes();
- 
DatagramPacket dataPacket = new DatagramPacket(outBuf,
- 
outBuf.length);
- 
dataPacket.setAddress(InetAddress.getByName("127.0.0.1"));
- 
dataPacket.setPort(2110);
- 
- 
Log.e("SendLength", ""+outBuf.length);
- 
- 
dataPacket.setLength(outBuf.length);
- 
Log.e("", "start send");
- 
udpSocket.send(dataPacket);
- 
Log.e("", "send over");
复制代码
数据接收端:
> 
- 
//作为数据接受端我们只需要监控相应的端口就行了
- 
//host默认为本机host，手机自己里面为127.0.0.1
- 
DatagramSocket udpSocket = new DatagramSocket(2110);
- 
DatagramPacket dataPacket = new DatagramPacket(framedata,
- 
framedata.length);
- 
- 
Log.e("", "start receiver");
- 
//这个是使用DatagramActivity的核心啊
- 
//该方法为阻塞的，会一直阻塞知道有数据过来
- 
udpSocket.receive(dataPacket);
- 
Log.e("", "receiver ok");
- 
- 
int iLen = dataPacket.getLength();
- 
Log.e("receiveLength", ""+iLen);
- 
String str = new String(framedata,0,iLen);
- 
Log.e("receiveString", str);
复制代码
数据接收只需要对端口进行监听就行了，host自动绑定本地host，
receive函数是核心，因为该方法会一直阻塞直到数据过来，源码里面我是先让发送端休眠了3秒，也就是让接收端先接收，因为发送端要3秒后才会发送，所以接收端肯定会先阻塞，大家看下下面的效果图.
Log打印图:
![ff.jpg](http://www.shouyanwang.org.sixxs.org/data/attachment/forum/201103/27/223958pceujbnebdpe8b4u.jpg)
2011-3-27 22:39 上传
[下载附件(26.7
 KB)](http://www.shouyanwang.org.sixxs.org/forum-attachment-aid-MzB8ODVhM2M5MmZ8MTMzMzk1NjAwMnww-nothumb-yes.html)
项目源码下载:
![](http://www.shouyanwang.org.sixxs.org/static/image/filetype/rar.gif)[DatagramDemo.rar](http://www.shouyanwang.org.sixxs.org/forum-attachment-aid-MzF8MjIxMWQ3NTJ8MTMzMzk1NjAwMnww.html)(26.99
 KB, 下载次数: 473) 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# 在Android系统使用socket在Java层和native之间数据通信
Android是基于Linux的系统，系统底层机制基本上是相同的，因为分本地代码和java代码，并且是java代码通过jni调用本地代码执行。
因为UDP报文发送是有局限的，局限就是当发送方的速度远远的超过接受方的时候，底层会大量的丢失数据，而且用UDP还会比较容易引起端口的阻塞，因此需要更加强大的类来实现类似的功能。LocalServerSocket和LocalSocket正是在这样的情况下产生的，是以UNIX空间为缓冲区来进行数据的存储的.
实例一：Native本地代码作客户端发送请求，Java端作服务器端创建socket,并监听处理socket请求：
import android.net.LocalServerSocket;
import android.net.LocalSocket;
class SocketListener extends Thread {
        @Override
        public void run() {
            try {
                LocalServerSocket server = new LocalServerSocket("com.jenny.vnc.localsocket");
                while (true) {    
                    LocalSocket receiver = server.accept();
                    if (receiver != null) {
                        InputStream input = receiver.getInputStream();
                        int readed = input.read();
                        ...........//handle    
                    }
                }
 server.close();
            } catch (IOException e) {
                Log.e(getClass().getName(), e.getMessage());
            }
        }
    }
JNI 的Native端代码做客户端：
#include <sys/socket.h>
void send_remote_request(char *msg)
{
    int localsocket, len;
    struct sockaddr_un remote;
    if ((localsocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    exit(1);
    }
    char *name="com.jenny.vnc.localsocket";//与java上层相同哦
    remote.sun_path[0] = '\0';  /* abstract namespace */
    strcpy(remote.sun_path+1, name);
    remote.sun_family = AF_UNIX;
    int nameLen = strlen(name);
    len = 1 + nameLen + offsetof(struct sockaddr_un, sun_path);
    if (connect(localsocket, (struct sockaddr *)&remote, len) == -1) {
    return;
    }
    if (send(localsocket, msg, strlen(msg),0) == -1) {
    return;
    }
    close(localsocket);
}
实例二、Java作客户端发送请求，Native本地代码作服务器端监听处理socket请求：
Java层的主要代码：
   1. LocalSocket so = null;
   2. LocalSocketAddress addr;
   3. so = new LocalSocket();
   4. addr = new LocalSocketAddress(SOCKET_NAME, LocalSocketAddress.Namespace.RESERVED);
   5. so.connect(addr);
如果能正常connect到addr,那就可以像一般文件操作那样进行io读写了。
native层的主要代码：
   1. cli_fd = android_get_control_socket(SOCKET_NAME);
   2. retval = listen(cli_fd, backlog);
   3. cli_fd_cmd = accept(cli_fd, (sockaddr *)&peeraddr, &socklen);
如果能够正常accept到java端的连接，那就可以用标准的io读写操作来读写数据了。
# [整理下android LocalServerSocket LocalSocket的用法](http://www.shouyanwang.org.sixxs.org/thread-93-1-1.html)
LocalServerSocket和LocalSocket主要能够实现什么功能呢？
其实类似数据报包一方发送，一方接受类似，详情请看：
[http://www.shouyanwang.org.sixxs.org/thread-43-1-1.html](http://www.shouyanwang.org.sixxs.org/thread-43-1-1.html)
在上面的文章中，我们可以借助手机里面的端口来实现数据的边发送边接受，但是运用UDP也是有局限的，局限就是当发送方的速度远远的超过接受方的时候，底层会大量的给你丢数据，而且用UDP还会比较容易引起端口的阻塞，因此需要更加强大的类来实现类似的功能。
LocalServerSocket和LocalSocket正是在这样的情况下产生的，是以UNIX空间为缓冲区来进行数据的存储的，什么是UNIX，请高手指教，我自己都没弄清楚，但是这也不要紧，有些时候，只要会用就行，没必要深入研究，LocalServerSocket和LocalSocket应该算是这类的，下面直接发我写好的代码和包吧：
- 
package org.shouyanwang.localSocket;
- 
- 
import java.io.IOException;
- 
import java.io.InputStream;
- 
import java.io.OutputStream;
- 
- 
import android.app.Activity;
- 
import android.net.LocalServerSocket;
- 
import android.net.LocalSocket;
- 
import android.net.LocalSocketAddress;
- 
import android.os.Bundle;
- 
import android.util.Log;
- 
import android.view.View;
- 
import android.widget.Button;
- 
- 
public class localSocketDemo extends Activity {
- 
- 
Button button ;
- 
Button button2;
- 
- 
LocalSocket receiver,sender;
- 
LocalServerSocket lss;
- 
- 
/** Called when the activity is first created. */
- 
@Override
- 
public void onCreate(Bundle savedInstanceState) {
- 
super.onCreate(savedInstanceState);
- 
setContentView(R.layout.main);
- 
- 
button = (Button)this.findViewById(R.id.test_id);
- 
button.setOnClickListener(new Button.OnClickListener(){
- 
public void onClick(View v) {
- 
- 
receiver = new LocalSocket();
- 
try {
- 
lss = new LocalServerSocket("mytest");
- 
receiver.connect(new LocalSocketAddress("mytest"));
- 
receiver.setReceiveBufferSize(500000);
- 
receiver.setSendBufferSize(500000);
- 
sender = lss.accept();
- 
sender.setReceiveBufferSize(500000);
- 
sender.setSendBufferSize(500000);
- 
- 
running = true;
- 
- 
new Thread (local_send).start();
- 
new Thread (local_receive).start();
- 
- 
} catch (IOException e1) {
- 
}
- 
}
- 
});
- 
- 
- 
button2 = (Button)this.findViewById(R.id.test_2);
- 
button2.setOnClickListener(new Button.OnClickListener(){
- 
public void onClick(View v) {
- 
running = false;
- 
}});
- 
}
- 
- 
- 
private boolean running = false;
- 
- 
Thread local_send = new Thread (){
- 
public void run(){
- 
- 
OutputStream m_Send;
- 
try {
- 
m_Send = sender.getOutputStream();
- 
- 
while(running)
- 
{
- 
byte[] data = "this is a test".getBytes();
- 
sender.setSendBufferSize(data.length);
- 
sender.setReceiveBufferSize(data.length);
- 
m_Send.write(data);
- 
m_Send.flush();
- 
- 
Thread.sleep(100);
- 
}
- 
- 
m_Send.close();
- 
sender.close();
- 
} catch (IOException e) {
- 
e.printStackTrace();
- 
} catch (InterruptedException e) {
- 
e.printStackTrace();
- 
}
- 
}
- 
};
- 
- 
- 
Thread local_receive = new Thread(){
- 
public void run(){
- 
try {
- 
InputStream m_Rece = receiver.getInputStream();
- 
byte[] data;
- 
int receiveLen = 0;
- 
while(running)
- 
{
- 
receiveLen = receiver.getReceiveBufferSize();
- 
data = new byte[receiveLen];
- 
m_Rece.read(data);
- 
Log.e("", "---"+new String(data));
- 
}
- 
m_Rece.close();
- 
receiver.close();
- 
} catch (IOException e) {
- 
e.printStackTrace();
- 
}
- 
}
- 
};
- 
}
复制代码
工程代码:
![](http://www.shouyanwang.org.sixxs.org/static/image/filetype/zip.gif)[localSocketDemo.zip](http://www.shouyanwang.org.sixxs.org/forum-attachment-aid-Njh8ZTc5YjdhYzN8MTMzMzk1MDUyNnww.html)(50.57
 KB, 下载次数: 680) 
有的手机会在进行LocalSocket的初始化的时候报哈is in use之类的，因为这个是Android系统特有的，并非在J2SE公共JAVA包中，因为可能就是你的手机不支持了。
接受数据情况，如图所是：
![1.jpg](http://www.shouyanwang.org.sixxs.org/data/attachment/forum/201104/14/194413lpbl89mm9rvbnlmm.jpg)
2011-4-14 19:44 上传
[下载附件(104.85
 KB)](http://www.shouyanwang.org.sixxs.org/forum-attachment-aid-Njl8MjRiNzhkYmF8MTMzMzk1MDUyNnww-nothumb-yes.html)
有点小BUG，大家自己去处理吧
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://www.iandroid.cn/bbs/viewthread.php?tid=442](http://www.iandroid.cn/bbs/viewthread.php?tid=442)
sipdroid的视频通话部分使用的是一方发送，一方接收的形式。发送方使用LocalServerSocket("Sipdroid")生成一个虚拟文件，并于接收端机器建立21070端口上的UDP连接，通过建立的连接不停的发送视频数据给接收方。
接收方收到一个大于200字节的数据后，调用如下代码切换到android自带播放器播放视频：
Intenti = new Intent(Intent.ACTION_VIEW,Uri.parse("rtsp://"+Receiver.engine(mContext).getRemoteAddr()+"/"+Receiver.engine(mContext).getRemoteVideo()+"/Sipdroid"));
startActivity(i);
从代码上看使用的是RTSP协议，
