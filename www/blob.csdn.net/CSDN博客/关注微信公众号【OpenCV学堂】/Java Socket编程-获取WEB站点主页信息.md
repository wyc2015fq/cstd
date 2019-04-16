# Java Socket编程 - 获取WEB站点主页信息 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年10月31日 23:15:50[gloomyfish](https://me.csdn.net/jia20003)阅读数：4089








早就有想写一个Java Socket编程系列的文章，由于自己懒一直没动。决定从

今天开始，每周写一篇，从最简单的socket连接，到文件传输到协议实现，到

远程桌面控制，全面讲述Java socket编程的各个方面与各种技巧，从普通

socket到Java NIO。




本文算是第一篇，简述如何通过Java Socket获取网站的主页内容其中包括

如何获取本地机器名与IP地址，如何建立一个socket连接到远程WEB站点

如何使用HTTP GET方法获取网站主页内容与HTTP响应头。程序运行结果：

![](https://img-my.csdn.net/uploads/201210/31/1351696543_2243.png)




从中我们可以知道csdn网站是基于什么语言开发，服务器是什么，版本是多少。

具体的做法请看源代码：

```java
package com.gloomyfish.socket.tutorial.one;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.UnknownHostException;

public class HomePageVisitor {
	private Socket client;
	private SocketAddress address;
	public HomePageVisitor(String hostName, int port) throws UnknownHostException, IOException {
		client = new Socket();
		address = new InetSocketAddress(hostName, port);
	}
	// GET http://localhost:8080/blank/Welcome.do HTTP/1.1\r\n
	public void printHomePageInfo(String httpString)
	{
		try {
			// Get local host info
	        InetAddress inetAddress = InetAddress.getLocalHost();
	        System.out.println("Host Address = " + inetAddress.getHostAddress());
	        System.out.println("Hostname = " + inetAddress.getHostName());
	        
	        // visit the WEBSITE
			client.connect(address);
			BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(client.getOutputStream(), "UTF8"));
			bufferedWriter.write(httpString);
			
			// -- HTTP/1.1 400 Bad Request if not send \r\n
			bufferedWriter.write("\r\n");
			bufferedWriter.flush();
			System.out.println("http request send out");
			
			BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(client.getInputStream()));
			String content = null;
			while((content = bufferedReader.readLine()) != null) {
				if(getStatusCde(content) == 400) {
					System.err.println("HTTP/1.1 | HTTP/1.0 400 Bad Request");
					break;
				}
				System.out.println("response contect -->> " + content);
			}
		} catch(Exception e) {
			e.printStackTrace();
		}
		
	}
	
	/**
	 * <p> try to parse the HTTP response content</p>
	 * @param content
	 * @return status of HTTP
	 */
	private static int getStatusCde(String content) {
		int status = 200; // default OK response
		if(content == null || "".equals(content))
			return status;
		else if (!content.contains("HTTP/1.1")&& !content.contains("HTTP/1.0"))
			return status;
		else if(content.contains("400") && content.contains("HTTP/1.1")
				|| content.contains("HTTP/1.0")) {
			status = 400;
		}
		return status;
	}
	
	public static void main(String[] args) {
		try {
			HomePageVisitor httpVisitor = new HomePageVisitor("www.csdn.net", 80);
			httpVisitor.printHomePageInfo("GET http://www.csdn.net HTTP/1.0\r\n");
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
```
如果使用了代理，在初始化Address时候使用HTTP代理的地址与端口
接口，其它不变，代理会自动转发HTTP GET请求。代码如下：



```java
address = new InetSocketAddress(http_proxy, proxy_port)
```





