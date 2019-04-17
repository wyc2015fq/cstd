# Http协议请求响应处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年01月17日 15:22:02[boonya](https://me.csdn.net/boonya)阅读数：812








## HTTP的基本概念



HTTP（HyperText Transfer Prototcol，超文本传输协议）是Internet上应用最为广泛的一种网络协议，所有的www文件都必须遵循这个标准。设计HTTP最初的目的是为了提供一种发布和接收HTML页面的方法。

### HTTP事务

一个HTTP事务即客户端发出一个“HTTP请求”命令和服务器回复“HTTP响应”结果的过程。



一次HTTP事务完成的过程：也就是一次web服务器的工作过程：
-    建立连接——接受或拒绝客户端连接请求；
-    接收请求——通过网络读取HTTP请求报文；
-    处理请求——解析请求报文并做出相应的动作；
-    访问资源——访问请求报文中相关的资源；
-    构建响应——使用正确的首部生成HTTP响应报文；
-    发送响应——向客户端发送生成的响应报文；
-    记录日志——当已经完成的HTTP事务记录进日志文件；

### HTTP报文

“HTTP请求”和“HTTP响应”均为格式化的数据块，它们被称作HTTP报文。



=============**GET请求例子：=============**


```
GET /css/news070130.css HTTP/1.1
Accept: */*
Referer: http://news.qq.com/a/20081224/000088.htm
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
If-Modified-Since: Thu, 29 Mar 2007 02:05:43 GMT; length=11339
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)
Host: news.qq.com
Connection: Keep-Alive
Cookie: ssid=idlsesels; flv=9.0; icache=ADLFMBGC; adid=adsdifids; isflucky_50829637=0;
```


**=============POST请求例子：=============**

```
POST /Login.php  HTTP/1.1
Accept: image/gif，*/*
Accept-language: zh-cn
Accept-encodeing: gzip
User-Agent: MSIE6.0
Host: www.some.com
Connection: Keep-Alive
Content-Length: 7
Cache-Control:no-cache
Cookie: name1=value1; name2=value2;
```



### HTTP方法

HTTP协议支持多个不同的请求命令，这些请求命令称作HTTP方法。每个HTTP请求报文都必须指定其请求方法，用于通知服务器进行什么样的响应动作。常见的HTTP方法有GET、HEAD、POST、PUT、DELETE、OPTIONS、TRACE等。

（1）OPTIONS：返回服务器针对特定资源所支持的HTTP请求方法，也可利用向web服务器发送“*”的请求来测试服务器的功能性。

（2）HEAD：这一方法可以在不必传输整个响应内容的情况下，就可以获取包含在响应消息头中的元信息。

（3）GET：向特定的资源发送请求。

（4）POST：向指定的资源提交数据进行处理请求（比如提交表单等）。数据被包含在请求体中。这种请求可能会导致新的资源的建立或已有资源的修改。

（5）PUT：向指定资源位置上传其最新内容。

（6）DELETE：删除指定资源

（7）TRACE：回显服务器收到的请求。

**HTTP方法的支持情况：**

![](https://img-blog.csdn.net/20170117145712077)


来自：[https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol)


### HTTP的响应码

HTTP服务器回应客户端状态码主要有一下几个：

1xx: 信息性状态码，这一类状态码代表请求已被接受，需要继续处理。

100：客户端应当继续发送请求。这个临时响应用来通知客户端部分请求已被接受请继续发送请求信息。

101：服务器已经理解了客户端的请求，并将通知客户端采用不同的协议来完成这个请求。

2xx: 成功状态码

200：OK：请求已成功，请求所希望的响应头或数据体将随此响应返回。

3xx: 重定向状态码

301:永久重定向,也就是说被请求的资源已永久移动到新位置； Location响应首部的值仍为当前URL，因此为隐藏重定向;

302:临时重定向，也就是说请求的资源现在临时从不同的URL响应请求；又称显式重定向, Location响应首部的值为新的URL喔。

304：Not Modified如果客户端发送了一个带条件的GET请求且该请求已被允许，而文档中的内容（自上次访问以来或者根据请求的条件）并没有改变，则服务器应当返回这个状态吗。

4xx: 客户端错误状态码

404:Not Found请求失败，请求所希望得到的资源未被在服务器上发现，一般这个状态码被广泛应用于当前服务器不想揭示到底为何请求被拒绝或者没有其他适合的响应可用的情况下。

5xx: 服务器端错误状态码

500:Internal Server Error服务器遇到了一个未曾预料的状况，导致了它无法完成对请求的处理。一般来说，这个问题都会在服务器的程序码出错时出现。

502:Bad Gateway作为网关或者代理工作服务器尝试执行请求时从上游服务器接收到的无效响应。

504：GatewayTimeout：作为网关或者代理服务器尝试执行请求时未能及时从上游服务器或者辅助服务器（比如DNS）收到响应。

目前的web服务器软件非常多，最流行的web服务器包含以下几种：Apache HTTP Server、Nginx（轻量级，更多新技术）、lighttpd（轻量级）等等。

以上内容来自：[http://www.coremail.cn/blog/?p=1697](http://www.coremail.cn/blog/?p=1697)




### HTTP协议是无状的



HTTP协议是无状态的，同一个客户端的这次请求和上次请求是没有对应关系，对http服务器来说，它并不知道这两个请求来自同一个客户端。 为了解决这个问题， Web程序引入了Cookie机制来维护状态.

## HTTP协议报文格式



### **1. 命令行窗口中用telnet测试HTTP协议 **









HTTP消息是由普通ASCII文本组成。消息包括消息头和数据体部分。消息头以行为单位，每行以CRLF（回车和换行）结束，消息头结束后，额外增加一个CRLF，之后内容就是数据体部分了。

我们可以在命令行窗口，通过TELNET，来测试HTTP协议。


1. 使用telnet连接到HTTP服务器，如要从google上请求index.html页面，首先要连接到服务器的80端口 


`telnet www.google.cn 80`

2. 现在已经连接上了服务器，发送http请求消息：

```
GET /index.html HTTP/1.1
connection: close
Host: www.google.cn
```


输入上面内容后，连续敲入两个回车，哈哈，是不是看到返回结果了。

这个请求的意思是：通过1.1版本协议请求index.html页面；connection: close是实用短连接，即服务器返回后就断开连接；Host字段知名页面所在的主机名。

返回结果应该是这样的：


```
HTTP/1.1 200 OK
Cache-Control: private, max-age=0
Date: Fri, 02 Jan 2009 12:26:17 GMT
Expires: -1
Content-Type: text/html; charset=GB2312
Set-Cookie: PREF=ID=7bbe374f53b6c6a8:NW=1:TM=1230899177:LM=1230899177:S=2EGHuZJnrtdQUB_A; expires=Sun, 02-Jan-2011 12:26:17 GMT; path=/; domain=.google.com
Server: gws
Transfer-Encoding: chunked
Connection: Close
```

这里有一个额外的CRLF


```
<html>
html数据
</html>
```




### 2. 请求消息格式

HTTP请求报文由请求行（request line）、请求头部（header）、空行和请求数据4个部分组成，下图给出了请求报文的一般格式。



![](http://pic002.cnblogs.com/images/2012/426620/2012072810301161.png)：



- 请求行 


第一行是请求行：请求方法 URL路径 HTTP版本号，上面的例子中的请求行是：

GET /index.html HTTP/1.1

使用GET方法请求服务器根目录下的index.html文件，约定使用HTTP/1.1版本。

请求方法包括GET、POST、HEAD、OPTIONS、TRACE、PUT等几种，最常用的前2种。

1. GET方法。通过将参数附加在URL后面提交参数。如http://www.google.cn/search?hl=zh-CN&newwindow=1&rls=com.microsoft%3Azh-cn%3AIE-SearchBox&q=%E6%89%8B%E6%9C%BA&meta=&aq=f&oq=

GET

2. POST方法。通过将参数附加在请求的头部行后面（GET方式是没有数据体的）提交参数。GET方式提交参数数据量一般限制在1k一下，而POST是没有限制的。POST一般用在表单数据提交的时候。

3. HEAD方法不让服务器在回应中返回任何实体。对HEAD请求的回应部分来说，它的HTTP标题中包含的元信息与通过GET请求所得到的是相同的。通过使用这种方法，不必传输整个实体主体，就可以得到请求URI所指定资源的元信息。该方法通常用来测试超链接的合法性、可访问性及最近更新。 

- 头部行 


头部行中的内容一般用来描述客户端能兼容的类型。如Accept-Language告诉服务器客户端支持哪些语言；Cache-Control告诉服务器客户端是否支持cache。

- 附属体（数据实体） 


请求的附属体，GET不适用，POST方法提交数据时，用来存放请求的参数。

- 客户端传递参数方式 


1. GET方式传递，直接在URL后面。

2. POST方式传递，请求行告诉服务器参数长度，通过附属体独立传给服务器。

3. Cookie方式传递，不要忘记这种方式了，会话Session功能就是通过这种方式实现的哦。

- 一些常用的头部行请求参数 


**User-Agent**：客户端的浏览器信息。

**Connection**：close表示使用短连接，Keep-Alive表示客户端支持持久连接。

**Date**：消息产生的当前时间。

**Accept-Language**：客户端支持的语言。

**Cache-Contro**l：客户端是否支持cache。no-cache表示客户端不支持cache，如max-age=3600，告诉User Agent 该请求的响应结果在多长时间内有效，在有效期内，当用户再次需要访问时，直接从客户端本地提取，不需要访问服务器。

**Referer**：Referer用以告诉服务器该请求来自于哪个URL，可以用追踪用户的WEB访问路径。

**If-Modified-Since**: 如果请求消息包含If-Modified-Since标题域，GET方法的语法就变成“条件GET”，即“（conditional GET）”。 条件GET方法可以对指定资源进行判断，如果它在If-Modified-Since标题域中的指定日期后发生了更新，才启动传输，否则不传输。这种条件GET允许被缓存的实体在不必经过多次请求或不必要的数据传输就能进行刷新，从而有助于降低网络负载。


**Last-Modified**: Sun, 03 Dec 2008 23:52:56 GMT

**ETag**: “1cbf3-dfd-3a2adcd8″

    一般应用在文件下载上，用于标识响应内容的最后修改时间或内容较验码，当下一次User agent需要时，服务首先判断这二个值是否跟服务器上一致，一样则不需要返回内容。


**Expires**：该请求的响应结果在什么时间失效，在没有失效之前，代理可直接从缓存中返回以前的响应结果。

**Host**：指定请求资源的主机和端口号。HTTP/1.1请求必须指定Host，否则服务器会返回400状态码。

- 请求消息例子 


GET请求例子：


```
GET /css/news070130.css HTTP/1.1
Accept: */*
Referer: http://news.qq.com/a/20081224/000088.htm
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
If-Modified-Since: Thu, 29 Mar 2007 02:05:43 GMT; length=11339
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)
Host: news.qq.com
Connection: Keep-Alive
Cookie: ssid=idlsesels; flv=9.0; icache=ADLFMBGC; adid=adsdifids; isflucky_50829637=0;
```



POST请求例子：


```
POST /Login.php  HTTP/1.1
Accept: image/gif，*/*
Accept-language: zh-cn
Accept-encodeing: gzip
User-Agent: MSIE6.0
Host: www.some.com
Connection: Keep-Alive
Content-Length: 7
Cache-Control:no-cache
Cookie: name1=value1; name2=value2;
```

这里有一个额外的CRLF

`username=b&passwd=d`


### 3. 响应消息格式 

HTTP响应也由三个部分组成，分别是：状态行、消息报头、响应正文。

响应消息包括状态行、若干头部行和附属体（html数据实体）。

- 状态行 


状态行包括：HTTP协议版本号、状态码、状态码的文本描述信息。如：HTTP/1.1 200 OK

状态码由一个三位数组成，状态码大体有5种含义：

1. 1xx。信息，请求收到，继续处理。

2. 2xx。成功。200请求成功；206断点续传。

3. 3xx。重定向。一般跳转到新的地址。

4. 4xx。客户端错误。404文件不存在

5. 5xx。服务器错误。500内部错误。

- 头部行 


**Set-Cookie**：服务器设置客户端Cookie。设置格式是name=value，设置多个参数时中间用分号隔开。Set-Cookie时还会用到几个参数：PATH设置有效的路径，DOMAIN设置cookie生效的域名，Expire设置cookie的有效时间，0表示关闭浏览器就失效。

**Location**：当服务器返回3xx重定向时，该参数实现重定向。广告链接的跳转就使用这种协议。

**Content-Length**：附属体（数据实体）的长度








**HTTP请求格式:**

<request-line>

<headers>

<blank line>

[<request-body>]

说明:第一行必须是一个请求行(request-line),用来说明请求类型,要访问的资源以及所使用的HTTP版本.

      紧接着是一个首部(header)小节,用来说明服务器要使用的附加信息.

      之后是一个空行.

      再后面可以添加任意的其他数据[称之为主体(body)].

例1 GET请求：



```
GET / HTTP/1.1
Accept: */*
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)
Host: www.google.cn
Connection: Keep-Alive
##说明:请求的第一部分说明了该请求是一个GET请求.该行的第二部分是一个斜杠(/),用来说明请求的是该域名的根目录.该行的最后一部分说明使用的是HTTP1.1版本(另一个可选荐是1.0).
      第2行是请求的第一个首部,HOST将指出请求的目的地.User-Agent,服务器端和客户端脚本都能访问它,它是浏览器类型检测逻辑的重要基础.该信息由你的浏览器来定义,并且在每个请求中自动发送.Connection,通常将浏览器操作设置为Keep-Alive
      第三部分,空行,即使不存在请求主体,这个空行也是必需的.
```





例2 POST请求:



```
POST / HTTP1.1
Host:www.wrox.com
User-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)
Content-Type:application/x-www-form-urlencoded
Content-Length:40
Connection: Keep-Alive

name=Professional%20Ajax&publisher=Wiley
##说明:请求行开始处的GET改为POST,以表示不同的请求类型.
      Content-Type说明了请求主体的内容是如何编码的.浏览器始终以application/x-www-form-urlencoded的格式编码来传送数据,这是针对简单URL编码的MIME类型.Content-Length说明了请求主体的字节数.
      最后请求主体.名称-值对的形式.
```





**HTTP响应格式:**<status-line>

<headers>

<blank line>

[<response-body>]

例:

> 
HTTP/1.1 200 OK

Date: Fri, 22 May 2009 06:07:21 GMT

Content-Type: text/html; charset=UTF-8

<html>

      <head></head>

      <body>

            <!--body goes here-->

      </body>

</html>

说明:HTTP状态码200,找到资源,并且一切正常.

      Date:生成响应的日期和时间.

      Content-Type:指定了MIME类型的HTML(text/html),编码类型是UTF-8

      HTML源文体.











以上文字来自：[http://www.cnblogs.com/kabi/p/6285814.html](http://www.cnblogs.com/kabi/p/6285814.html)


## 浏览器端的HTTP请求

### HTTP GET

**HTTP参数：**请求参数

![](https://img-blog.csdn.net/20170117152006984)


**HTTP头：**包括请求头和相应头

![](https://img-blog.csdn.net/20170117152036234)


**HTTP响应：**响应消息

![](https://img-blog.csdn.net/20170117152046385)


**HTTP Cookie：**用于保存用户登录状态的Cookie信息

![](https://img-blog.csdn.net/20170117152101978)




### HTTP POST



**HTTP头：**包括请求头和响应头

![](https://img-blog.csdn.net/20170117150556503)


**`HTTP响应：`**响应消息

![](https://img-blog.csdn.net/20170117150752519)

**HTTP Cookie：**用于保存用户登录状态的Cookie信息

![](https://img-blog.csdn.net/20170117151153708)


## 自定义HTTP请求

### 请求协议



WAAS平台与WMS系统之间采用HTTP协议进行通讯，请求方式为POST请求，数据采用JSON格式。


### 请求头





```
Accpet: application/json
Accpet-Encoding: gzip
Content-Encoding: UTF-8
Content-Type: application/json; charset=UTF-8
PlatformCode:{由平台分配}
MemberCode:{会员编码，对应WMS租户编码,由平台分配}
Token: RSA(PlatformCode$MemberCode$DateTime.Now())//签名：平台代码$会员/租户代码$当前时间，然后进行RSA加密，RSA密钥由平台统一分配。请求当前时间与WAAS服务器时间差不大于10分钟。
Sequence: 传输每次请求唯一UUID，WAAS据此判定请求是否重复
```



### 请求内容



标准JSON格式，如：{ “key”:“value”}


###  响应内容



标准JSON格式，如：{ “key”:“value”}

### Java HTTP客户端工具



```java
package com.wlyd.fmcgwms.util.api;

import java.io.IOException;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;
import org.apache.commons.lang.exception.ExceptionUtils;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.ssl.SSLSocketFactory;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;
import com.alibaba.fastjson.JSONObject;
import com.wlyd.fmcgwms.util.Log;
import com.wlyd.fmcgwms.util.SAASTokenManager;
import com.wlyd.fmcgwms.util.ehcache.EhcacheUtil;
/**
 * Apache HttpClient JSON参数请求方式
 * 
 * @package com.wlyd.fmcgwms.util.api.APIHttpClient
 * @date   2016年11月11日  下午5:46:01
 * @author pengjunlin
 * @comment   
 * @update
 */
public class APIHttpClient {

	// 接口地址
	private String apiURL = "";
	
	private Log logger = Log.getLogger(getClass());
	
	private HttpClient httpClient = null;
	
	private HttpPost method = null;
	
	private long startTime = 0L;
	
	private long endTime = 0L;
	
	private int status = 0;
	
	/**
	 * 设置忽略安全验证
	 * 
	 * @MethodName: initHttps 
	 * @Description: 
	 * @throws
	 */
	private void initHttps(){
		//========================设置忽略访问SSL===================
		// 创建TrustManager
		X509TrustManager xtm = new X509TrustManager() {
			public void checkClientTrusted(X509Certificate[] chain,
					String authType) throws CertificateException {
			}
			
			public void checkServerTrusted(X509Certificate[] chain,
					String authType) throws CertificateException {
			}
			
			public X509Certificate[] getAcceptedIssuers() {
				return new X509Certificate[] {};
			}
		};
		
		SSLContext ctx=null;
		try {
			ctx = SSLContext.getInstance("SSL");
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		// 使用TrustManager来初始化该上下文，TrustManager只是被SSL的Socket所使用
		try {
			ctx.init(null, new TrustManager[] { xtm }, null);
		} catch (KeyManagementException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		SSLSocketFactory sf = new SSLSocketFactory(
				ctx,
				SSLSocketFactory.ALLOW_ALL_HOSTNAME_VERIFIER);
		Scheme sch = new Scheme("https", 443, sf);
		httpClient.getConnectionManager().getSchemeRegistry().register(sch);
	}

	/**
	 * 接口地址
	 * 
	 * @param url
	 */
	public APIHttpClient(String url) {
			this.apiURL = url;
			String platformCode = EhcacheUtil.get("WAAS_PLATFORMCODE").toString();
			String memberCode =  EhcacheUtil.get("WAAS_MEMBERCODE").toString();
			httpClient = new DefaultHttpClient();
			method = new HttpPost(apiURL);
			method.setHeader("Accept", "application/json");
			method.setHeader("Accpet-Encoding", "gzip");
			method.setHeader("Content-Encoding", "UTF-8");
			method.setHeader("Content-Type", "application/json; charset=UTF-8");
			method.setHeader("Token", SAASTokenManager.getToken());
			method.setHeader("PlatformCode", platformCode);
			method.setHeader("MemberCode", memberCode);
			method.setHeader("Sequence", UUID.randomUUID().toString());
			// 设置忽略访问SSL
			initHttps();
	}
	
	/**
	 * 接口地址
	 * 
	 * @param url
	 * @param uuid
	 */
	public APIHttpClient(String url,String uuid) {
			apiURL = url;
			String platformCode = EhcacheUtil.get("WAAS_PLATFORMCODE").toString();
			String memberCode =  EhcacheUtil.get("WAAS_MEMBERCODE").toString();
			httpClient = new DefaultHttpClient();
			method = new HttpPost(apiURL);
			method.setHeader("Accept", "application/json");
			method.setHeader("Accpet-Encoding", "gzip");
			method.setHeader("Content-Encoding", "UTF-8");
			method.setHeader("Content-Type", "application/json; charset=UTF-8");
			method.setHeader("Token", SAASTokenManager.getToken());
			method.setHeader("PlatformCode", platformCode);
			method.setHeader("MemberCode", memberCode);
			method.setHeader("Sequence", uuid);
			// 设置忽略访问SSL
			initHttps();
	}

	/**
	 * 调用 API
	 * 
	 * @param parameters
	 * @return
	 */
	public String post(String parameters) {
		String body = null;
		logger.info("parameters:" + parameters);

		if (method != null & parameters != null
				&& !"".equals(parameters.trim())) {
			JSONObject jsonObject = JSONObject.parseObject(parameters);
			logger.info("json:" + jsonObject.toString());
			try {

				List<NameValuePair> params = new ArrayList<NameValuePair>();
				// 建立一个NameValuePair数组，用于存储欲传送的参数
				params.add(new BasicNameValuePair("data", parameters));
				
				StringEntity entity=new StringEntity(parameters, "UTF-8");
				// 添加参数
				method.setEntity(entity/*new UrlEncodedFormEntity(params, "UTF-8")*/);

				startTime = System.currentTimeMillis();

				// 设置编码
				HttpResponse response = httpClient.execute(method);
				endTime = System.currentTimeMillis();
				int statusCode = response.getStatusLine().getStatusCode();
				logger.info("statusCode:" + statusCode);
				logger.info("调用API 花费时间(单位：毫秒)：" + (endTime - startTime));
				if (statusCode != HttpStatus.SC_OK) {
					logger.error("Method failed:" + response.getStatusLine());
					status = 1;
				}

				// Read the response body
				body = EntityUtils.toString(response.getEntity());

			} catch (IOException e) {
				// 发生网络异常
				logger.error("exception occurred!\n"
						+ ExceptionUtils.getFullStackTrace(e));
				// 网络错误
				status = 3;
			} finally {
				logger.info("调用接口状态：" + status);
			}

		}
		return body;
	}

	/**
	 * 0.成功 1.执行方法失败 2.协议错误 3.网络错误
	 * 
	 * @return the status
	 */
	public int getStatus() {
		return status;
	}

	/**
	 * @param status
	 *            the status to set
	 */
	public void setStatus(int status) {
		this.status = status;
	}

	/**
	 * @return the startTime
	 */
	public long getStartTime() {
		return startTime;
	}

	/**
	 * @return the endTime
	 */
	public long getEndTime() {
		return endTime;
	}

}
```
上面是忽略了HTTPS的访问代码，HTTP可正常访问。











