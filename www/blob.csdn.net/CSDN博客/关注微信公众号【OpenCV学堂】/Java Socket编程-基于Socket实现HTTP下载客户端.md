# Java Socket编程 - 基于Socket实现HTTP下载客户端 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年12月08日 17:51:28[gloomyfish](https://me.csdn.net/jia20003)阅读数：8253








没有借助任何第三方库，完全基于JAVA Socket实现一个最小化的HTTP文件下载客

户端。完整的演示如何通过Socket实现下载文件的HTTP请求(request header)发送

如何从Socket中接受HTTP响应(Response header, Response body)报文并解析与

保存文件内容。如何通过SwingWork实现UI刷新，实时显示下载进度。

**首先看一下UI部分：**

![](https://img-my.csdn.net/uploads/201212/08/1354957740_6179.png)




【添加下载】按钮：

点击弹出URL输入框，用户Copy要下载文件URL到输入框以后，点击[OK]按钮即开始

下载
![](https://img-my.csdn.net/uploads/201212/08/1354957768_8405.png)



【清除完成】按钮：

清除所有已经下载完成的文件列表

文件下载状态分为以下几种：

```java
package com.gloomyfish.socket.tutorial.http.download;

public enum DownLoadStatus {
	NOT_STARTED,
	IN_PROCESS,
	COMPLETED,
	ERROR
}
```

UI部分主要是利用Swing组件完成。点击【添加下载】执行的代码如下：

```java
final JDialog dialog = new JDialog(this,"Add File Link",true);
dialog.getContentPane().setLayout(new BorderLayout());
// dialog.setSize(new Dimension(400,200));
final URLFilePanel panel = new URLFilePanel();
panel.setUpListener(new ActionListener(){
	@Override
	public void actionPerformed(ActionEvent e) {
		if("OK".equals(e.getActionCommand())){
			if(panel.validateInput()) {
				DownloadDetailStatusInfoModel data = new DownloadDetailStatusInfoModel(panel.getValidFileURL());
				tableModel.getData().add(data);
				startDownlaod();
				refreshUI();
			}
			dialog.setVisible(false);
			dialog.dispose();
		} else if("Cancel".equals(e.getActionCommand())) {
			dialog.setVisible(false);
			dialog.dispose();
		}
	}});

dialog.getContentPane().add(panel, BorderLayout.CENTER);
dialog.pack();
centre(dialog);
dialog.setVisible(true);
```
【清除完成】按钮执行的代码如下：


```java
private void clearDownloaded() {
	List<DownloadDetailStatusInfoModel> downloadedList = new ArrayList<DownloadDetailStatusInfoModel>();
	for(DownloadDetailStatusInfoModel fileStatus : tableModel.getData()) {
		if(fileStatus.getStatus().toString().equals(DownLoadStatus.COMPLETED.toString())) {
			downloadedList.add(fileStatus);
		}
	}
	tableModel.getData().removeAll(downloadedList);
	refreshUI();
}
```
让JFrame组件居中显示的代码如下：




```java
public static void centre(Window w) {
		Dimension us = w.getSize();
		Dimension them = Toolkit.getDefaultToolkit().getScreenSize();
		int newX = (them.width - us.width) / 2;
		int newY = (them.height - us.height) / 2;
		w.setLocation(newX, newY);
	}
```





HTTP协议实现部分：

概述：HTTP请求头与相应头报文基本结构与解释

HTTP请求：一个标准的HTTP请求报文如

![](https://img-my.csdn.net/uploads/201212/08/1354959957_1059.png)


其中请求头可以有多个，message-body可以没有，不是必须的。请求行的格式如下：

Request-Line = Method SP Request-URI SPHTTP-Version CRLF 举例说明如下：

Request-Line = GET http://www.w3.org/pub/WWW/TheProject.htmlHTTP/1.1\r\n

其中SP表示空格， CRLF表示回车换行符\r\n

当你想要上传文件时候，使用Post方式来填写数据到message-body中即可。发送一个

简单的HTTP请求报文如下：

GET /pub/WWW/TheProject.html HTTP/1.1\r\n

Host: [www.w3.org\r\n](http://www.w3.org/r/n)

\r\n



HTTP响应：一个标准的HTTP响应报文如下

![](https://img-my.csdn.net/uploads/201212/08/1354960020_3451.png)


最先得到是状态行，其格式如下：

Status-Line = HTTP-Version SP Status-CodeSP Reason-Phrase CRLF, 一个状态行的

简单例子如下：Status-Line = HTTP/1.1 200 OK一般大家最喜欢的就是Status-Code会

给你很多提示，最常见的就是404，500等状态码。状态码的意思可以参考RFC2616中

的解释。下载文件最要紧是的检查HTTP响应头中的Content-Length与Content-Type两

个中分别声明了文件的长度与文件的类型。其它如Accept-Ranges表示接受多少到多少

的字节。可能在多线程下载中使用。搞清楚了HTTP请求与响应的报文格式以后，我们

就可以通过Socket按照报文格式解析内容，发送与读取HTTP请求与响应。具体步骤

如下：

**一：根据用户输入的文件URL建立Socket连接**



```java
URL url = new URL(fileInfo.getFileURL());
String host = url.getHost();
int port = (url.getPort() == -1)  ? url.getDefaultPort():url.getPort();
System.out.println("Host Name = " + host);
System.out.println("port = " + port);
System.out.println("File URI = " + url.getFile());

// create socket and start to construct the request line
Socket socket = new Socket();
SocketAddress address = new InetSocketAddress(host, port);
socket.connect(address);
```
用了URL类来把用户输入的url string变成容易解析一点的URL。


**二：构造HTTP请求**



```java
BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream(), "UTF8"));
String requestStr = "GET " + url.getFile() + " HTTP/1.1\r\n"; // request line

// construct the request header - 构造HTTP请求头(request header)
String hostHeader = "Host: " + host + "\r\n";
String acceptHeader = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
String charsetHeader = "Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n";
String languageHeader = "Accept-Language: zh-CN,zh;q=0.8\r\n";
String keepHeader = "Connection: close\r\n";
```
三：发送HTTP请求
```java
// 发送HTTP请求
bufferedWriter.write(requestStr);
bufferedWriter.write(hostHeader);
bufferedWriter.write(acceptHeader);
bufferedWriter.write(charsetHeader);
bufferedWriter.write(languageHeader);
bufferedWriter.write(keepHeader);
bufferedWriter.write("\r\n"); // 请求头信息发送结束标志
bufferedWriter.flush();
```



四：接受HTTP响应并解析内容，写入创建好的文件

```java
// 准备接受HTTP响应头并解析
CustomDataInputStream input = new CustomDataInputStream(socket.getInputStream());
File myFile = new File(fileInfo.getStoreLocation() + File.separator + fileInfo.getFileName());
String content = null;
HttpResponseHeaderParser responseHeader = new HttpResponseHeaderParser();
BufferedOutputStream output = new BufferedOutputStream(new FileOutputStream(myFile));
boolean hasData = false;
while((content = input.readHttpResponseHeaderLine()) != null) {
	System.out.println("response header contect -->> " + content);
	responseHeader.addResponseHeaderLine(content);
	if(content.length() == 0) {
		hasData = true;
	}
	if(hasData) {
		int totalBytes = responseHeader.getFileLength();
		if(totalBytes == 0) break; // no response body and data
		int offset = 0;
		byte[] myData = null;
		if(totalBytes >= 2048) {
			myData = new byte[2048];
		} else {
			myData = new byte[totalBytes];
		}
		int numOfBytes = 0;
		while((numOfBytes = input.read(myData, 0, myData.length)) > 0 && offset < totalBytes) {
			offset += numOfBytes;
			float p = ((float)offset) / ((float)totalBytes) * 100.0f;
			if(offset > totalBytes) {
				numOfBytes = numOfBytes + totalBytes - offset;
				p = 100.0f;
			}
			output.write(myData, 0, numOfBytes);
			updateStatus(p);
		}
		hasData = false;
		break;
	}
}
```
简单的HTTP响应头解析类HttpResponseHeaderParser代码如下：



```java
package com.gloomyfish.socket.tutorial.http.download;

import java.util.HashMap;
import java.util.Map;

/**
 * it can parse entity header, response head
 * and response line <status code, CharSet, ect...>
 * refer to RFC2616，关于HTTP响应头，请看RFC文档，描写的很详细啊！！
 * 
 * @author fish
 *
 */
public class HttpResponseHeaderParser {
	public final static String CONTENT_LENGTH = "Content-Length";
	public final static String CONTENT_TYPE = "Content-Type";
	public final static String ACCEPT_RANGES = "Accetp-Ranges";
	
	private Map<String, String> headerMap;
	public HttpResponseHeaderParser() {
		headerMap = new HashMap<String, String>();
	}
	/**
	 * <p> get the response header key value pair </p>
	 * @param responseHeaderLine
	 */
	public void addResponseHeaderLine(String responseHeaderLine) {
		if(responseHeaderLine.contains(":")) {
			String[] keyValue = responseHeaderLine.split(": ");
			if(keyValue[0].equalsIgnoreCase(CONTENT_LENGTH)) {
				headerMap.put(CONTENT_LENGTH, keyValue[1]);
			} else if(keyValue[0].equalsIgnoreCase(CONTENT_TYPE)) {
				headerMap.put(CONTENT_TYPE, keyValue[1]);
			} else {
				headerMap.put(keyValue[0], keyValue[1]);
			}
		}
	}
	
	public int getFileLength() {
		if(headerMap.get(CONTENT_LENGTH) == null){
			return 0;
		}
		return Integer.parseInt(headerMap.get(CONTENT_LENGTH));
	}
	
	public String getFileType() {
		return headerMap.get(CONTENT_TYPE);
	}
	public Map<String, String> getAllHeaders() {
		return headerMap;
	}

}
```



**可执行的Jar文件下载地址(这次我要点分)：**

http://download.csdn.net/detail/jia20003/4862076


**转载请务必注明**






