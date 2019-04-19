# 网路编程-TCP-Socket - 三少GG - CSDN博客
2012年04月09日 15:31:44[三少GG](https://me.csdn.net/scut1135)阅读数：1247

### [](http://xpws2006.blog.163.com/blog/static/954385772008101233827804/)
### [java tcp编程](http://xpws2006.blog.163.com/blog/static/954385772008101233827804/)
传输控制协议是一种基于流的网络通讯方法，它与其它的任何协议都有很大的不同。本文讨论TCP流以及在Java中怎样操作它。
一、概述
TCP提供的网络通讯接口与用户数据报协议（UDP）截然不同。TCP的特性使网络编程很具魅力，而且它删除了UDP的很多干扰部分（例如数据包的排序 和丢失），简化了网络通讯。UDP关心的是数据包的传输，而TCP关注的是建立网络连接，并在网络连接中发送和接收字节流。
数据包可以通过网络用多种方法发送，并且它们到达的时间可能不同。这有利于性能的提高和程序的健壮性，因为单个包的丢失不一定干扰其它包的传输。但是，这 样的系统使程序员必须作更多的工作，他们必须保证数据的送达（delivery）。TCP通过对发送和次序的保证消除了这些额外的工作，为客户端和支持两 路（two-way）通讯的服务器之间提供了可靠的字节通讯流。它在两台计算机之间建立了"虚拟连接"，可以通过虚拟连接发送数据流。
图1：TCP建立虚拟连接传输数据
TCP使用更低层的（lower-level）的IP通讯协议在两台计算机之间建立连接。这种连接提供了一个允许字节流发送和接收的接口，并且采用透明 的方式把数据转换为IP数据报。数据报（datagram）的问题之一是不能保证数据包到达目的地。TCP解决了这个问题，它提供了有保证的数据字节的送 达。当然，网络错误影响了送达也是可能的，但是TCP通过类似重新发送数据包解决了这种实现的问题，并且只在情况很严重（例如没有到网络主机的路由或连接 丢失了）的时候才提醒程序员。
四、Socket类
Socket类表现了客户端套接字，它是属于一台或两台计算机的 两个TCP通讯端口之间的通讯通道。端口可以连接到本地系统的另一个端口，这样可以避免使用另一台计算机，但是大多数网络软件将使用两台计算机。但是 TCP套接字不能与两台以上的计算机通讯。如果需要这种功能，客户端应用程序必须建立多个套接字连接，每台计算机一个套接字。
构造函数
java.net.Socket类有几个构造函数。其中两个构造函数允许使用布尔型参数指定是否使用UDP或TCP套接字，我们不赞成使用它们。这儿没有使用这两个构造函数，并且没有列举在此处--如果需要UDP功能，请使用DatagramSocket。
try
{
// 连接到指定的主机和端口
Socket mySocket = new Socket ( "www.awl.com", 80);
// ......
}
catch (Exception e)
{
System.err.println ("Err - " + e);
}
但是还有很多构造函数可以用于不同的情形。除非特别指出，所有的构造函数都是公共的。
· protected Socket ()-使用当前套接字产生组件提供的默认实现建立不连接的套接字。开发者一般不应该使用这个方法，因为它不允许指定主机名称和端口。
· Socket (InetAddress address, int port)产生 java.io.IOException异常。
· java.lang.SecurityException-建立连接到指定的IP地址和端口的套接字。如果不能建立连接，或连接到主机违反了安全性约束条件（例如某个小的服务程序试图连接到某台计算机而不是载入它的计算机时），就产生这种异常。
· Socket (InetAddress address, int port, InetAddress localAddress, int localPort)产生java.io.IOException、java.lang.SecurityException异常-建立连接到指定的地址 和端口的套接字，并把它绑定到特定的本地地址和本地端口。默认情况下，使用一个自由（空）的端口，但是在多地址主机环境（例如本地主机有两个或多个的计算 机）中，该方法也允许你指定一个特定的端口号、地址。
· protected Socket (SocketImpl implementation)--使用特定的套接字的实现（implementation）建立未连接的套接字。通常情况下开发者不应该使用这个方法，因为它允许指定主机名称和端口。
· Socket (String host, int port)产生java.net.UnknownHostException、java.io.IOException、 java.lang.SecurityException异常--建立连接到特定主机和端口的套接字。这个方法允许指定一个字符串而不是一个 InetAddress。如果指定的主机名称不能够解析，就不能建立连接，如果违反了安全性约束条件就产生异常。
· Socket (String host, int port, InetAddress localAddress, int localPort)产生java.net.UnknownHostException、java.io.IOException、 java.lang.SecurityException异常--建立连接到特定主机和端口的套接字，并绑定到特定的本地端口和地址。它允许指定字符串形 式的主机名称，而不是指定InetAddress实例，同时它允许指定一个将绑定的本地地址和端口。这些本地参数对于多地址主机（如果可以通过两个或更多
 IP地址访问的计算机）是有用的。如果主机名称不能解析，就不能建立连接，如果违反了安全性约束条件会产生异常。
五、建立TCP客户端
讨论了套接字类的功能后，我们将分析一个完整的TCP客户端程序。此处我们将看到的客户端程序是一个daytime客户端，它连接到一个daytime 服务器程序以读取当前的日期和时间。建立套接字连接并读取信息是一个相当简单的过程，只需要少量的代码。默认情况下daytime服务运行在13端口上。 并非每台计算机都运行了daytime服务器程序，但是Unix服务器是客户端运行的很好的系统。
DaytimeClient的代码
import java.net.*
import java.io.*;
public class DaytimeClient
{
public static final int SERVICE_PORT = 13;
public static void main(String args[])
{
// 检查主机名称参数
if (args.length != 1)
{
System.out.println ("Syntax - DaytimeClient host");
return;
}
// 获取服务器程序的主机名称
String hostname = args[0];
try
{
// 获取一个连接到daytime服务的套接字
Socket daytime = new Socket (hostname,
SERVICE_PORT);
System.out.println ("Connection established");
// 在服务器程序停止的情况下设置套接字选项
daytime.setSoTimeout ( 2000 );
// 从服务器程序读取信息
BufferedReader reader = new BufferedReader (
new InputStreamReader
(daytime.getInputStream()
));
System.out.println ("Results : " +
reader.readLine());
// 关闭连接
daytime.close();
}
catch (IOException ioe)
{
System.err.println ("Error " + ioe);
}
}
}
DaytimeClient是如何工作的
Daytime应用程序是很容易理解的，它使用了文章前面谈到的概念。建立套接字、获取输入流，在很少的事件中（在连接时像daytime一样简单的服 务器程序失败）激活超时设置。不是连接已筛选过的流，而是把有缓冲的读取程序连接到套接字输入流，并且把结果显示给用户。最后，在关闭套接字连接后客户端 终止。这是你可能得到的最简单的套接字应用程序了--复杂性来自实现的网络协议，而不是来自具体网络的编程。
运行DaytimeClient
运行上面的应用程序很简单。简单地把运行daytime服务的计算机的主机名称作为命令行参数指定并运行它就可以了。如果daytime服务器程序使用了非标准的端口号，记得需要改变端口号并重新编译。
例如，如果服务器程序在本机上，为了运行客户端将使用下面的命令：
java DaytimeClient localhost
注意
Daytime服务器程序必须正在运行中，否则该客户端程序将不能建立连接。例如如果你正在使用Wintel系统而不是Unix，那么你需要运行DaytimeServer。
Java API中封装了大量的函数,供编写[网络](http://action.vogate.com/c/c.php?r=http%3A//www.baidu.com/s%3Fie%3Dgb2312%26bs%3DTCP%25B1%25E0%25B3%25CC%26sr%3D%26z%3D%26cl%3D3%26f%3D8%26wd%3DTCP%25B1%25E0%25B3%25CC+java%26ct%3D0&aid=829&sid=6235007045041189&click=1&url=http%3A//www.17coming.com/html/code/index.html&v=0&s=http%3A//www.programfan.com/article/showarticle.asp%3Fid%3D2796&rn=483003&k=%25u7F51%25u7EDC)通信程序时使用.
这使得java在网络方面具有强大的功能.
用java编写TCP方式的通信程序比较简单,但也有一些问题需要注意.
以下为监听主程序,监听程序在发现客户端连接后,启动一个会话socket线程,以实现实时发送,接收信息
和多客户端同时工作.
import java.io.*;
import java.lang.*;
import java.net.ServerSocket;
import java.net.Socket;
//主程序一直处于监听状态，有连接则启动一个线程进行处理，以实现多个客户端
public class listenserve
{
private ServerSocket ss;
private boolean listening=true;
public listenserve()
{
   Init();//初始化
   lisn();//启动监听
}
public void Init()
{
   try
   {
    ss=new ServerSocket(10015,10);
   }
   catch(IOException ie)
   {
     System.out.println("无法在10015端口监听");
     ie.printStackTrace();
   }
}
public void lisn()
{
   try
   {
    while(listening)
     new Thread(new dialogserve(ss.accept())).start();
     }
    catch(IOException ie)
    {ie.printStackTrace();}
}
public static void main(String args[])
{
   new listenserve();
}
}
//以下为会话主程序
应该特别注意,如果客户端先关闭,会话socket中可能抛出socketexception:connection reset
这应该在程序中进行处理,这也是较易忽略的问题.
import java.io.*;
import java.lang.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
public class dialogserve implements Runnable
{
private Socket s;
private InputStream in;
private String rev,temp;
private byte b[];
private int len;
public dialogserve(Socket ss)
{
   s=ss;
   b=new byte[1024];
   try
   {
   in=s.getInputStream();
   }catch(IOException ie)
   {
    ie.printStackTrace();
    }
   rev="";
}
public void run()
{
   try
   {
    while(s.isConnected()==true)
    {
     if((len=in.read(b))!=-1)
     {
      temp=new String(b,0,len);
       rev+=temp;
       System.out.print(rev);
       temp=null;
       Thread.sleep(1000);
     }
    }
    in.close();
    s.close();
    System.out.println("会话socket已断开！");
   }
   catch(SocketException se)
   {
    System.out.println("客户端已断开！");
     System.exit(0);
   }
   catch(IOException io)
   {
    io.printStackTrace();
    System.exit(0);
   }
   catch(InterruptedException ire)
   { ire.printStackTrace();}
}
}
//以下为客户端主程序
import java.io.*;
import java.net.Socket;
import java.lang.*;
public class client
{
private Socket con;//客户端连接socket
private OutputStream out;
private String sen;
private byte b[];
public client()
{
   clientInit();
}
public void clientInit()
{
   try
   {
    con=new Socket("localhost",10015);
    con.setSoTimeout(10000);
    b=new byte[1024];
    OutputStream out=con.getOutputStream();
    sen="hello serve,以TCP方式发送数据！";
    b=sen.getBytes();
    out.write(b);
    out.flush();
    out.close();
    con.close();
   }
   catch(IOException ie)
   {
    ie.toString();
   }
}
public static void main(String args[])
{
   new client();
}
}
总的来说,以上所列代码较为简单,但已基本反映出java编写简单tcp通信程序的原理.
java基于TCP的socket编程简单实现[代码实践过]
1.什么是socket
　　所谓socket通常也称作"套接字"，用于描述IP地址和端口，是一个通信链的句柄。应用程序通常通过"套接字"向网络发出请求或者应答网络请求。
　　以J2SDK-1.3为例，Socket和ServerSocket类库位于java.net包中。ServerSocket用于服务器端，Socket是建立网络连接时使用的。在连接成功时，应用程序两端都会产生一个Socket实例，操作这个实例，完成所需的会话。对于一个网络连接来说，套接字是平等的，并没有差别，不因为在服务器端或在客户端而产生不同级别。不管是Socket还是ServerSocket它们的工作都是通过SocketImpl类及其子类完成的。
　　重要的Socket API：
　　java.net.Socket继承于java.lang.Object，有八个构造器，其方法并不多，下面介绍使用最频繁的三个方法，其它方法大家可以见JDK-1.3文档。
　　. Accept方法用于产生"阻塞"，直到接受到一个连接，并且返回一个客户端的Socket对象实例。"阻塞"是一个术语，它使程序运行暂时"停留"在这个地方，直到一个会话产生，然后程序继续；通常"阻塞"是由循环产生的。
　　. getInputStream方法获得网络连接输入，同时返回一个IutputStream对象实例，。
　　. getOutputStream方法连接的另一端将得到输入，同时返回一个OutputStream对象实例。
　　注意：其中getInputStream和getOutputStream方法均会产生一个IOException，它必须被捕获，因为它们返回的流对象，通常都会被另一个流对象使用。
　　2.如何开发一个Server-Client模型的程序
　　开发原理：
　　服务器，使用ServerSocket监听指定的端口，端口可以随意指定（由于1024以下的端口通常属于保留端口，在一些操作系统中不可以随意使用，所以建议使用大于1024的端口），等待客户连接请求，客户连接后，会话产生；在完成会话后，关闭连接。
　　客户端，使用Socket对网络上某一个服务器的某一个端口发出连接请求，一旦连接成功，打开会话；会话完成后，关闭Socket。客户端不需要指定打开的端口，通常临时的、动态的分配一个1024以上的端口。
多个客户同时连接：
主程序监听一端口，等待客户接入；同时构造一个线程类，准备接管会话。当一个Socket会话产生后，将这个会话交给线程处理，然后主程序继续监听。运用Thread类或Runnable接口来实现是不错的办法。
现在实现服务器端编程：（基于TCP方式）
MyServer .java
---------------------------------------------------
package server;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;
//这个简单的socket服务器端主要是说句Hello XXX,采用多线程实现
public class MyServer extends Thread {
private Socket s;
public static void main(String[] args) {
     MyServer.server();//main入口，一运行机器，就开通server服务。
}
public MyServer(Socket s) {
     this.s = s;
}
public void run() {
     try {
      OutputStream os = s.getOutputStream();
      InputStream is=s.getInputStream();
      byte[] bu=new byte[20];
      int len=is.read(bu);//从客户端读取消息
      String userName=new String(bu,0,len);
      Date d=new Date();
      System.out.println(userName+" is coming here at "+d.toLocaleString());//写在服务器端控制台的日志显示
      String welcome="hello,"+userName+"!";
      //System.out.println(welcome);
      os.write(welcome.getBytes());//返回给客户端的欢迎信息
      os.close();
      is.close();
      s.close();
     } catch (Exception e) {
      e.printStackTrace();
     }
}
public static void server() {
     try {
      ServerSocket ss = new ServerSocket(9527); //服务器的套接字，端口为9527
      while (true) {
       Socket s = ss.accept();
       new MyServer(s).start(); //用户一进来就运行一个线程
      }
     } catch (Exception e) {
      e.printStackTrace();
     }
}
}
客户端编程：
MyClient .java
---------------------------------
package client;
import java.io.*;
import java.net.*;
public class MyClient {
public static void main(String[] args) {
     //这里先强迫用户输入用户名，调用client(str)方法。
     String str = "";
     try {
      while (str.length() < 1) {
       System.out.println("请输入您的名字");
       str = new DataInputStream(new BufferedInputStream(System.in))
         .readLine();
      }
     } catch (Exception e) {
      e.printStackTrace();
     }
     MyClient.client(str);
}
public static void client(String userName) {
     try {
      Socket s = new Socket(InetAddress.getByName("127.0.0.1"), 9527);//套接字的IP地址和端口号
      OutputStream os = s.getOutputStream();
      InputStream is = s.getInputStream();
      byte[] Name = userName.getBytes("ISO-8859-1"); //这个是为了中文乱码问题
      os.write(Name);//向服务器发送消息
      byte[] serverSay = new byte[100];
      int len = is.read(serverSay);//接受服务器消息
      System.out.println(new String(serverSay, 0, len));//客户端控制台显示服务器返回的信息
      is.close();
      os.close();
      s.close();//记住一定要关闭这些输入，输出流和套接字
     } catch (Exception e) {
      e.printStackTrace();
     }
}
}
### [Socket连接实例（TCP/UDP](http://blog.csdn.net/ibone/article/details/4328675))
1  服务器建立了一个监听指定端口号的服务器端Socket连接
2 客户端发送连接
3 服务器为每一个连接启动一个新的线程
4 信息交换
5 关闭连接
服务器：
```java
package net;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
public class MulSocketServer {
	public static void main(String[] args) {
		ServerSocket serverSocket = null;
		Socket socket = null;
		OutputStream os = null;// 输出流
		InputStream is = null;// 输入流
		int port = 10000;// 监听端口号
		try {
			serverSocket = new ServerSocket(port);
			System.out.println(":已经启动");
			while (true) {// 接收到一个连接、启动一个线程
				socket = serverSocket.accept();
				new LogicThread(socket);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				os.close();
				is.close();
				socket.close();
				serverSocket.close();
			} catch (Exception e) {
			}
		}
	}
}
class LogicThread extends Thread {
	Socket socket;
	InputStream is;
	OutputStream os;
	public LogicThread(Socket socket) {
		this.socket = socket;
		start(); // 启动线程
	}
	public void run() {
		byte[] b = new byte[1024];
		try {
			os = socket.getOutputStream();// 初始化流
			is = socket.getInputStream();
			for (int i = 0; i < 3; i++) {
				int n = is.read(b);// 读取数据
				byte[] response = logic(b, 0, n); // 逻辑处理
				os.write(response); // 反馈数据
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			close();
		}
	}
	/**
	 * 关闭流和连接
	 */
	private void close() {
		try {
			// 关闭流和连接
			os.close();
			is.close();
			socket.close();
		} catch (Exception e) {
		}
	}
	/**
	 * 逻辑处理方法,实现echo逻辑
	 * 
	 * @param b
	 *            客户端发送数据缓冲区
	 * @param off
	 *            起始下标
	 * @param len
	 *            有效数据长度
	 * @return
	 */
	private byte[] logic(byte[] b, int off, int len) {
		byte[] response = new byte[len];
		// 将有效数据拷贝到数组response中
		System.arraycopy(b, 0, response, 0, len);
		return response;
	}
}
```
客户端：
```java
package net;
import java.io.*;
import java.net.*;
public class MulSocketClient {
	public static void main(String[] args) {
		Socket socket = null;
		InputStream is = null;
		OutputStream os = null;
		String serverIP = "127.0.0.1"; // 服务器端IP地址
		int port = 10000; // 服务器端端口号
		String data[] = { "First", "Second", "Third" }; // 发送内容
		try {
			socket = new Socket(serverIP, port); // 建立连接
			os = socket.getOutputStream(); // 初始化流
			is = socket.getInputStream();
			byte[] b = new byte[1024];
			for (int i = 0; i < data.length; i++) {
				os.write(data[i].getBytes()); // 发送数据
				int n = is.read(b); // 接收数据
				System.out.println("服务器反馈：" + new String(b, 0, n)); // 输出反馈数据
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				is.close();
				os.close();
				socket.close();
			} catch (Exception e2) {
			}
		}
	}
}
```
一直没事动手敲敲 今天补上
UDP连接实例：
较TCP而言传输不可考，无需建立虚拟连接，服务器压力较小。
传输一般步骤：
**客户端：**
1 建立DatagramSocket连接
2 然后构造一个DatagramPacket对象，此对象包括内容，接收方IP，端口号。
3 用DatagramSocket对象的send()方法发送DatagramPacket数据包。
4 用DatagramSocket对象的receive()方法接收DatagramPacket返回的数据包。
5 关闭DatagramSocket连接
**服务器端：**
1 建立DatagramSocket连接，监听端口
2 使用DatagramSocket的receive（）方法接收发来的数据
3 关闭连接
服务器端应为每一个连接起一个线程，方法同TCP的服务器端。
