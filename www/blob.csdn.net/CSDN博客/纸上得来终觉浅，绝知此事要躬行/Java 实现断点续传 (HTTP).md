# Java 实现断点续传 (HTTP) - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年12月17日 23:04:12[boonya](https://me.csdn.net/boonya)阅读数：3585








公司需要用Java做断点续传的实现，没有接触过，不过根据自己的理解就是文件接着上次传输的继续完成传输，具体的操作看到IBM这位仁兄的例子。

原文地址：[http://www.ibm.com/developerworks/cn/java/joy-down/index.html](http://www.ibm.com/developerworks/cn/java/joy-down/index.html)



### 1、断点续传的原理




其实断点续传的原理很简单，就是在 Http 的请求上和一般的下载有所不同而已。 

打个比方，浏览器请求服务器上的一个文时，所发出的请求如下： 

假设服务器域名为 wwww.sjtu.edu.cn，文件名为 down.zip。 


```
GET /down.zip HTTP/1.1 
Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms- 
excel, application/msword, application/vnd.ms-powerpoint, */* 
Accept-Language: zh-cn 
Accept-Encoding: gzip, deflate 
User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0) 
Connection: Keep-Alive
```




服务器收到请求后，按要求寻找请求的文件，提取文件的信息，然后返回给浏览器，返回信息如下：



```
200 
Content-Length=106786028 
Accept-Ranges=bytes 
Date=Mon, 30 Apr 2001 12:56:11 GMT 
ETag=W/"02ca57e173c11:95b" 
Content-Type=application/octet-stream 
Server=Microsoft-IIS/5.0 
Last-Modified=Mon, 30 Apr 2001 12:56:11 GMT
```





所谓断点续传，也就是要从文件已经下载的地方开始继续下载。所以在客户端浏览器传给 Web 服务器的时候要多加一条信息 -- 从哪里开始。 

下面是用自己编的一个"浏览器"来传递请求信息给 Web 服务器，要求从 2000070 字节开始。 


```
GET /down.zip HTTP/1.0 
User-Agent: NetFox 
RANGE: bytes=2000070- 
Accept: text/html, image/gif, image/jpeg, *; q=.2, */*; q=.2
```




仔细看一下就会发现多了一行 RANGE: bytes=2000070- 

这一行的意思就是告诉服务器 down.zip 这个文件从 2000070 字节开始传，前面的字节不用传了。 

服务器收到这个请求以后，返回的信息如下： 


```
206 
Content-Length=106786028 
Content-Range=bytes 2000070-106786027/106786028 
Date=Mon, 30 Apr 2001 12:55:20 GMT 
ETag=W/"02ca57e173c11:95b" 
Content-Type=application/octet-stream 
Server=Microsoft-IIS/5.0 
Last-Modified=Mon, 30 Apr 2001 12:55:20 GMT
```




和前面服务器返回的信息比较一下，就会发现增加了一行： 

Content-Range=bytes 2000070-106786027/106786028 

返回的代码也改为 206 了，而不再是 200 了。


知道了以上原理，就可以进行断点续传的编程了。

### 2、Java 实现断点续传的关键几点
- 
(1) 用什么方法实现提交 RANGE: bytes=2000070-。 

当然用最原始的 Socket 是肯定能完成的，不过那样太费事了，其实 Java 的 net 包中提供了这种功能。代码如下： 


```cpp
URL url = new URL("http://www.sjtu.edu.cn/down.zip"); 
HttpURLConnection httpConnection = (HttpURLConnection)url.openConnection(); 

// 设置 User-Agent 
httpConnection.setRequestProperty("User-Agent","NetFox"); 
// 设置断点续传的开始位置 
httpConnection.setRequestProperty("RANGE","bytes=2000070"); 
// 获得输入流 
InputStream input = httpConnection.getInputStream();
```



从输入流中取出的字节流就是 down.zip 文件从 2000070 开始的字节流。 大家看，其实断点续传用 Java 实现起来还是很简单的吧。 接下来要做的事就是怎么保存获得的流到文件中去了。

- 
保存文件采用的方法。 

我采用的是 IO 包中的 RandAccessFile 类。 

操作相当简单，假设从 2000070 处开始保存文件，代码如下： 

```java
RandomAccess oSavedFile = new RandomAccessFile("down.zip","rw"); 
long nPos = 2000070; 
// 定位文件指针到 nPos 位置 
oSavedFile.seek(nPos); 
byte[] b = new byte[1024]; 
int nRead; 
// 从输入流中读入字节流，然后写到文件中 
while((nRead=input.read(b,0,1024)) > 0) 
{ 
oSavedFile.write(b,0,nRead); 
}
```



 接下来要做的就是整合成一个完整的程序了。包括一系列的线程控制等等。

### 3、断点续传内核的实现


主要用了 6 个类，包括一个测试类：

SiteFileFetch.java         负责整个文件的抓取，控制内部线程 (FileSplitterFetch 类 )。 

FileSplitterFetch.java    负责部分文件的抓取。 

FileAccess.java            负责文件的存储。 

SiteInfoBean.java         要抓取的文件的信息，如文件保存的目录，名字，抓取文件的 URL 等。 

Utility.java                    工具类，放一些简单的方法。 

TestMethod.java          测试类。


我这里做了一下整理：

**SiteFileFetch.java **




```java
package com.boonya.http.file;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
/**
 * <li>文件名称: SiteFileFetch</li>
 * <li>文件描述: $负责整个文件的抓取，控制内部线程(FileSplitterFetch类)</li>
 * <li>内容摘要: 包括模块、函数及其功能的说明</li>
 * <li>完成日期：2013-12-17</li>
 * <li>所属作者：boonyachengdu@gmail.com</li>
 * <li>修改记录1：下午10:31:16,修改内容描述</li>
 *
 */
public class SiteFileFetch extends Thread
{
	// 文件信息Bean
	SiteInfoBean siteInfoBean = null; 
	// 开始位置
	long[] nStartPos; 
	// 结束位置
	long[] nEndPos; 
	 // 子线程对象
	FileSplitterFetch[] fileSplitterFetch;
	// 文件长度
	long nFileLength; 
	// 是否第一次取文件
	boolean bFirst = true; 
	// 停止标志
	boolean bStop = false; 
	// 文件下载的临时信息
	File tmpFile; 
	// 输出到文件的输出流
	DataOutputStream output; 

	public SiteFileFetch(SiteInfoBean bean) throws IOException
	{
		siteInfoBean = bean;
		// tmpFile = File.createTempFile ("zhong","1111",new
		// File(bean.getSFilePath()));
		tmpFile = new File(bean.getSFilePath() + File.separator + bean.getSFileName() + ".info");
		if (tmpFile.exists())
		{
			bFirst = false;
			read_nPos();
		} else
		{
			nStartPos = new long[bean.getNSplitter()];
			nEndPos = new long[bean.getNSplitter()];
		}
	}

	public void run()
	{
		// 获得文件长度
		// 分割文件
		// 实例FileSplitterFetch
		// 启动FileSplitterFetch线程
		// 等待子线程返回
		try
		{
			if (bFirst)
			{
				nFileLength = getFileSize();
				if (nFileLength == -1)
				{
					System.err.println("File Length is not known!");
				} else if (nFileLength == -2)
				{
					System.err.println("File is not access!");
				} else
				{
					for (int i = 0; i < nStartPos.length; i++)
					{
						nStartPos[i] = (long) (i * (nFileLength / nStartPos.length));
					}
					for (int i = 0; i < nEndPos.length - 1; i++)
					{
						nEndPos[i] = nStartPos[i + 1];
					}
					nEndPos[nEndPos.length - 1] = nFileLength;
				}
			}
			// 启动子线程
			fileSplitterFetch = new FileSplitterFetch[nStartPos.length];
			for (int i = 0; i < nStartPos.length; i++)
			{
				fileSplitterFetch[i] = new FileSplitterFetch(siteInfoBean.getSSiteURL(), siteInfoBean.getSFilePath() + File.separator + siteInfoBean.getSFileName(), nStartPos[i], nEndPos[i], i);
				Utility.log("Thread " + i + " , nStartPos = " + nStartPos[i] + ", nEndPos = " + nEndPos[i]);
				fileSplitterFetch[i].start();
			}
			// fileSplitterFetch[nPos.length-1] = new
			// FileSplitterFetch(siteInfoBean.getSSiteURL(),
			// siteInfoBean.getSFilePath() + File.separator +
			// siteInfoBean.getSFileName(),nPos[nPos.length-1],nFileLength,nPos.length-1);
			// Utility.log("Thread " + (nPos.length-1) + " , nStartPos = " +
			// nPos[nPos.length-1] + ", 	nEndPos = " + nFileLength);
			// fileSplitterFetch[nPos.length-1].start();
			// 等待子线程结束
			// int count = 0;
			// 是否结束while循环
			boolean breakWhile = false;
			while (!bStop)
			{
				write_nPos();
				Utility.sleep(500);
				breakWhile = true;
				for (int i = 0; i < nStartPos.length; i++)
				{
					if (!fileSplitterFetch[i].bDownOver)
					{
						breakWhile = false;
						break;
					}
				}
				if (breakWhile)
					break;
				// count++;
				// if(count>4)
				// siteStop();
			}
			System.err.println("文件下载结束！");
		} catch (Exception e)
		{
			e.printStackTrace();
		}
	} // 获得文件长度

	public long getFileSize()
	{
		int nFileLength = -1;
		try
		{
			URL url = new URL(siteInfoBean.getSSiteURL());
			HttpURLConnection httpConnection = (HttpURLConnection) url.openConnection();
			httpConnection.setRequestProperty("User-Agent", "NetFox");
			int responseCode = httpConnection.getResponseCode();
			if (responseCode >= 400)
			{
				processErrorCode(responseCode);
				return -2; // -2 represent access is error
			}
			String sHeader;
			for (int i = 1;; i++)
			{
				// DataInputStream in = new
				// DataInputStream(httpConnection.getInputStream ());
				// Utility.log(in.readLine());
				sHeader = httpConnection.getHeaderFieldKey(i);
				if (sHeader != null)
				{
					if (sHeader.equals("Content-Length"))
					{
						nFileLength = Integer.parseInt(httpConnection.getHeaderField(sHeader));
						break;
					}
				} else
					break;
			}
		} catch (IOException e)
		{
			e.printStackTrace();
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		Utility.log(nFileLength);
		return nFileLength;
	}

	// 保存下载信息（文件指针位置）
	private void write_nPos()
	{
		try
		{
			output = new DataOutputStream(new FileOutputStream(tmpFile));
			output.writeInt(nStartPos.length);
			for (int i = 0; i < nStartPos.length; i++)
			{
				// output.writeLong(nPos[i]);
				output.writeLong(fileSplitterFetch[i].nStartPos);
				output.writeLong(fileSplitterFetch[i].nEndPos);
			}
			output.close();
		} catch (IOException e)
		{
			e.printStackTrace();
		} catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	// 读取保存的下载信息（文件指针位置）
	private void read_nPos()
	{
		try
		{
			DataInputStream input = new DataInputStream(new FileInputStream(tmpFile));
			int nCount = input.readInt();
			nStartPos = new long[nCount];
			nEndPos = new long[nCount];
			for (int i = 0; i < nStartPos.length; i++)
			{
				nStartPos[i] = input.readLong();
				nEndPos[i] = input.readLong();
			}
			input.close();
		} catch (IOException e)
		{
			e.printStackTrace();
		} catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	private void processErrorCode(int nErrorCode)
	{
		System.err.println("Error Code : " + nErrorCode);
	}

	// 停止文件下载
	public void siteStop()
	{
		bStop = true;
		for (int i = 0; i < nStartPos.length; i++)
			fileSplitterFetch[i].splitterStop();
	}

}
```

**FileSplitterFetch.java**





```java
package com.boonya.http.file;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
/**
 * <li>文件名称: FileSplitterFetch</li>
 * <li>文件描述: $负责部分文件的抓取</li>
 * <li>内容摘要: 包括模块、函数及其功能的说明</li>
 * <li>完成日期：2013-12-17</li>
 * <li>所属作者：boonyachengdu@gmail.com</li>
 * <li>修改记录1：下午10:33:39,修改内容描述</li>
 *
 */
public class FileSplitterFetch extends Thread
{
	// File URL
	String sURL; 
	// File Snippet Start Position
	long nStartPos;
	// File Snippet End Position
	long nEndPos;
	// Thread's ID
	int nThreadID; 
	// Downing is over
	boolean bDownOver = false; 
	// Stop identical
	boolean bStop = false; 
	// File Access interface
	FileAccess fileAccessI = null; 

	public FileSplitterFetch(String sURL, String sName, long nStart, long nEnd, int id) throws IOException
	{
		this.sURL = sURL;
		this.nStartPos = nStart;
		this.nEndPos = nEnd;
		nThreadID = id;
		fileAccessI = new FileAccess(sName, nStartPos);
	}

	public void run()
	{
		while (nStartPos < nEndPos && !bStop)
		{
			try
			{
				URL url = new URL(sURL);
				HttpURLConnection httpConnection = (HttpURLConnection) url.openConnection();
				httpConnection.setRequestProperty("User-Agent", "NetFox");
				String sProperty = "bytes=" + nStartPos + "-";
				httpConnection.setRequestProperty("RANGE", sProperty);
				Utility.log(sProperty);
				InputStream input = httpConnection.getInputStream();
				// logResponseHead(httpConnection);
				byte[] b = new byte[1024];
				int nRead;
				while ((nRead = input.read(b, 0, 1024)) > 0 && nStartPos < nEndPos && !bStop)
				{
					nStartPos += fileAccessI.write(b, 0, nRead);
					// if(nThreadID == 1)
					// Utility.log("nStartPos = " + nStartPos + ", nEndPos = " +
					// nEndPos);
				}
				Utility.log("Thread " + nThreadID + " is over!");
				bDownOver = true;
				// nPos = fileAccessI.write (b,0,nRead);
			} catch (Exception e)
			{
				e.printStackTrace();
			}
		}
	}

	// 打印回应的头信息
	public void logResponseHead(HttpURLConnection con)
	{
		for (int i = 1;; i++)
		{
			String header = con.getHeaderFieldKey(i);
			if (header != null)
				// responseHeaders.put(header,httpConnection.getHeaderField(header));
				Utility.log(header + " : " + con.getHeaderField(header));
			else
				break;
		}
	}

	public void splitterStop()
	{
		bStop = true;
	}

}
```
**FileAccess.java**





```java
package com.boonya.http.file;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.io.Serializable;
/**
 * <li>文件名称: FileAccess</li>
 * <li>文件描述: $负责文件的存储</li>
 * <li>内容摘要: 包括模块、函数及其功能的说明</li>
 * <li>完成日期：2013-12-17</li>
 * <li>所属作者：boonyachengdu@gmail.com</li>
 * <li>修改记录1：下午10:35:47,修改内容描述</li>
 *
 */
public class FileAccess implements Serializable
{
	private static final long serialVersionUID = -6335788938054788024L;
	
	RandomAccessFile oSavedFile;
	
	long nPos;

	public FileAccess() throws IOException
	{
		this("", 0);
	}

	public FileAccess(String sName, long nPos) throws IOException
	{
		oSavedFile = new RandomAccessFile(sName, "rw");
		this.nPos = nPos;
		oSavedFile.seek(nPos);
	}

	public synchronized int write(byte[] b, int nStart, int nLen)
	{
		int n = -1;
		try
		{
			oSavedFile.write(b, nStart, nLen);
			n = nLen;
		} catch (IOException e)
		{
			e.printStackTrace();
		}
		return n;
	}

}
```
**SiteInfoBean.java**





```java
package com.boonya.http.file;
/**
 * <li>文件名称: SiteInfoBean</li>
 * <li>文件描述: $要抓取的文件的信息，如文件保存的目录，名字，抓取文件的URL等</li>
 * <li>内容摘要: 包括模块、函数及其功能的说明</li>
 * <li>完成日期：2013-12-17</li>
 * <li>所属作者：boonyachengdu@gmail.com</li>
 * <li>修改记录1：下午10:36:20,修改内容描述</li>
 *
 */
public class SiteInfoBean
{
	// Site's URL
	private String sSiteURL; 
	// Saved File's Path
	private String sFilePath;
	// Saved File's Name
	private String sFileName; 
	// Count of Splited Downloading File
	private int nSplitter; 

	public SiteInfoBean()
	{
		// default value of nSplitter is 5
		this("", "", "", 5);
	}

	public SiteInfoBean(String sURL, String sPath, String sName, int nSpiltter)
	{
		sSiteURL = sURL;
		sFilePath = sPath;
		sFileName = sName;
		this.nSplitter = nSpiltter;
	}

	public String getSSiteURL()
	{
		return sSiteURL;
	}

	public void setSSiteURL(String value)
	{
		sSiteURL = value;
	}

	public String getSFilePath()
	{
		return sFilePath;
	}

	public void setSFilePath(String value)
	{
		sFilePath = value;
	}

	public String getSFileName()
	{
		return sFileName;
	}

	public void setSFileName(String value)
	{
		sFileName = value;
	}

	public int getNSplitter()
	{
		return nSplitter;
	}

	public void setNSplitter(int nCount)
	{
		nSplitter = nCount;
	}

}
```
**Utility.java**





```java
package com.boonya.http.file;
/**
 * <li>文件名称: Utility</li>
 * <li>文件描述: $工具类，放一些简单的方法</li>
 * <li>内容摘要: 包括模块、函数及其功能的说明</li>
 * <li>完成日期：2013-12-17</li>
 * <li>所属作者：boonyachengdu@gmail.com</li>
 * <li>修改记录1：下午10:36:44,修改内容描述</li>
 *
 */
public class Utility
{
	public Utility()
	{
	}

	public static void sleep(int nSecond)
	{
		try
		{
			Thread.sleep(nSecond);
		} catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	public static void log(String sMsg)
	{
		System.err.println(sMsg);
	}

	public static void log(int sMsg)
	{
		System.err.println(sMsg);
	}

}
```
**TestMethod.java**



```java
package com.boonya.http.file;
/**
 * <li>文件名称: TestMethod</li>
 * <li>文件描述: $测试类</li>
 * <li>内容摘要: 包括模块、函数及其功能的说明</li>
 * <li>完成日期：2013-12-17</li>
 * <li>所属作者：boonyachengdu@gmail.com</li>
 * <li>修改记录1：下午10:38:43,修改内容描述</li>
 *
 */
public class TestMethod
{
	public TestMethod()
	{ 
		// /xx/weblogic60b2_win.exe
		try
		{
			SiteInfoBean bean = new SiteInfoBean("http://localhost/xx/weblogic60b2_win.exe", "D:\\temp", "weblogic60b2_win.exe", 5);
			SiteFileFetch fileFetch = new SiteFileFetch(bean);
			fileFetch.start();
		} catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	public static void main(String[] args)
	{
		new TestMethod();
	}

}
```
这个例子很好理解，断点续传就行了，确实帮了大忙了，感谢仁兄！




