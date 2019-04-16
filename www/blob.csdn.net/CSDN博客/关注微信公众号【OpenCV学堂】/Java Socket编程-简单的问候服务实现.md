# Java Socket编程 - 简单的问候服务实现 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年11月02日 23:07:34[gloomyfish](https://me.csdn.net/jia20003)阅读数：4824
个人分类：[Java Scoket编程](https://blog.csdn.net/jia20003/article/category/1270931)









**服务器端：**

实现一个最简单的Hello服务，打印输出客户端IP地址到控制台，对任何连接的客户端都

会发送一串字符(Hello, Java Socket)然后关闭与客户端连接。等待下一个客户端的连接

请求到来。

**客户端：**

实现一个最简单的Socket连接到Hello服务器端，接受服务器端发送过来的字节数据

打印并输出内容到控制台。

**关键技巧：**

由于JAVA中提供非常多的输入与输出流API，导致很多初学者接触JAVA SOCKET编程

时，由于对网络字节通信的本质缺乏了解，直接一个readline()去接受Socket字节流。

但是由于发送一方并没有发送/r/n导致一直无法读到数据，这个是最常见的一个错误。

另外一些常见的错误包括没有初始化接受缓冲区导致字符乱码，没有按照读入接受到

字节数重新组装，导致接受到的数据异常。所以代码演示了什么叫按字节发送与按字

节接受，这个是网络编程中非常重要的概念与原则。让那些输入流println()方法与输

出流readline()方法见鬼去吧，不是不好用，而是我建议你不要用，因为那些会妨碍你

的网络编程本质的认识与理解。另外我要特别说明一下：有时候flush()真的不是必须的

除非你使用了带有缓冲的输入与输出流来读写字节。

服务器端代码：



```java
package com.gloomyfish.socket.tutorial.two;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class HelloService extends Thread {
	private ServerSocket serverSocket;
	public HelloService(int port) throws IOException {
		serverSocket = new ServerSocket(port);
	}

	public void run()
	{
		try {
			while(true)
			{
				System.out.println("Waiting for client on port " + serverSocket.getLocalPort());
				Socket client = serverSocket.accept(); // blocked & waiting for income socket
				System.out.println("Just connected to " + client.getRemoteSocketAddress());
				DataOutputStream dos = new DataOutputStream(client.getOutputStream());
				byte[] hello = "Hello, Java Socket".getBytes();
				dos.write(hello, 0, hello.length);
				dos.close();
				client.close();
			}
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
		try {
			HelloService service = new HelloService(9999);
			service.start();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
```
服务器在端口9999监听并等待连接，使用Java Thread来实现服务器端启动。

客户端代码如下：



```java
package com.gloomyfish.socket.tutorial.two;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

public class HelloClient {
	private int clientNumber;
	private SocketAddress address;
	public HelloClient(int clientNum) {
		clientNumber = clientNum;
	}
	
	public void setupClients(String serverHostName, int port) throws IOException {
		address = new InetSocketAddress(serverHostName, port);
		for(int i=0; i<clientNumber; i++) {
			System.out.println();
			System.out.println("start client No. " + (i+1));
			Socket socket = new Socket();
			socket.connect(address);
			DataInputStream bufferedReader = new DataInputStream(socket.getInputStream());
			byte[] cbuff = new byte[256];
			char[] charBuff = new char[256];
			int size = 0;
			while( (size = bufferedReader.read(cbuff))> 0) {
				convertByteToChar(cbuff, charBuff, size);
				System.out.println(charBuff);
			}
			bufferedReader.close();
			socket.close();
		}
	}
	
	private void convertByteToChar(byte[] cbuff, char[] charBuff, int size) {
		for(int i=0; i<charBuff.length; i++) {
			if(i < size) {
				charBuff[i] = (char)cbuff[i];
			} else {
				charBuff[i] = ' ';
			}
		}
		
	}

	public static void main(String[] args) {
		try {
			HelloClient client = new HelloClient(10);
			client.setupClients("localhost", 9999);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
```
启动10个客户端去连接服务器端，接受到服务器端问候以后客户端关闭连接。

**特别提示：**一定要初始化缓冲区charBuff

程序运行结果：

![](https://img-my.csdn.net/uploads/201211/02/1351869919_5928.png)





