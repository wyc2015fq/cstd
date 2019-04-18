# Apache HttpClient 4.3开发指南 实现URL重定向 - z69183787的专栏 - CSDN博客
2015年08月26日 15:11:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3497
作者：chszs，转载需注明。博客主页：[http://blog.csdn.net/chszs](http://blog.csdn.net/chszs)
## 一、概述
Apache HttpClient 4系列已经发布很久了，但由于它与HttpClient 3.x版本完全不兼容，以至于业内采用此库的公司较少，在互联网上也少有相关的文档资料分享。
本文旨在写一个简要的Apache HttpClient 4.3开发指南，帮助开发者快速上手Apache HttpClient 4.3.x库。
要注意的是，本文档中的代码在低于HttpClient 4.3版本的地方可能不能运行。
## 二、开发手册
1、创建HTTP客户端
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16854747#)[copy](http://blog.csdn.net/chszs/article/details/16854747#)[print](http://blog.csdn.net/chszs/article/details/16854747#)[?](http://blog.csdn.net/chszs/article/details/16854747#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75375/fork)
- CloseableHttpClient client = HttpClientBuilder.create().build();  
2、发送基本的GET请求
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16854747#)[copy](http://blog.csdn.net/chszs/article/details/16854747#)[print](http://blog.csdn.net/chszs/article/details/16854747#)[?](http://blog.csdn.net/chszs/article/details/16854747#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75375/fork)
- instance.execute(new HttpGet(“http://www.baidu.com”));
3、获取HTTP响应的状态码
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16854747#)[copy](http://blog.csdn.net/chszs/article/details/16854747#)[print](http://blog.csdn.net/chszs/article/details/16854747#)[?](http://blog.csdn.net/chszs/article/details/16854747#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75375/fork)
- String url = “http://www.baidu.com”;
- CloseableHttpResponse response = instance.execute(new HttpGet(url));  
- assertThat(response.getStatusLine().getStatusCode(), equalTo(200));  
4、获取响应的媒体类型
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16854747#)[copy](http://blog.csdn.net/chszs/article/details/16854747#)[print](http://blog.csdn.net/chszs/article/details/16854747#)[?](http://blog.csdn.net/chszs/article/details/16854747#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75375/fork)
- String url = “http://www.baidu.com”;
- CloseableHttpResponse response = instance.execute(new HttpGet(url));  
- String contentMimeType = ContentType.getOrDefault(response.getEntity()).getMimeType();  
- assertThat(contentMimeType, equalTo(ContentType.TEXT_HTML.getMimeType()));  
5、获取响应的BODY部分
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16854747#)[copy](http://blog.csdn.net/chszs/article/details/16854747#)[print](http://blog.csdn.net/chszs/article/details/16854747#)[?](http://blog.csdn.net/chszs/article/details/16854747#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75375/fork)
- String url = “http://www.baidu.com”;
- CloseableHttpResponse response = instance.execute(new HttpGet(url));  
- String bodyAsString = EntityUtils.toString(response.getEntity());  
- assertThat(bodyAsString, notNullValue());  
6、配置请求的超时设置
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16854747#)[copy](http://blog.csdn.net/chszs/article/details/16854747#)[print](http://blog.csdn.net/chszs/article/details/16854747#)[?](http://blog.csdn.net/chszs/article/details/16854747#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75375/fork)
- @Test(expected=SocketTimeoutException.class)  
- publicvoid givenLowTimeout_whenExecutingRequestWithTimeout_thenException() throws ClientProtocolException, IOException{  
-     RequestConfig requestConfig = RequestConfig.custom()  
-     .setConnectionRequestTimeout(50).setConnectTimeout(50)  
-     .setSocketTimeout(50).build();  
-     HttpGet request = new HttpGet(SAMPLE_URL);  
-     request.setConfig(requestConfig);  
-     instance.execute(request);  
- }  
7、发送POST请求
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16854747#)[copy](http://blog.csdn.net/chszs/article/details/16854747#)[print](http://blog.csdn.net/chszs/article/details/16854747#)[?](http://blog.csdn.net/chszs/article/details/16854747#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75375/fork)
- instance.execute(new HttpPost(SAMPLE_URL));  
8、为HTTP请求配置重定向
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16854747#)[copy](http://blog.csdn.net/chszs/article/details/16854747#)[print](http://blog.csdn.net/chszs/article/details/16854747#)[?](http://blog.csdn.net/chszs/article/details/16854747#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75375/fork)
- CloseableHttpClient instance = HttpClientBuilder.create().disableRedirectHandling().build();  
- CloseableHttpResponse response = instance.execute(new HttpGet(SAMPLE_URL));  
- assertThat(reponse.getStatusLine().getStatusCode(), equalTo(301));  
9、配置请求的HEADER部分
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16854747#)[copy](http://blog.csdn.net/chszs/article/details/16854747#)[print](http://blog.csdn.net/chszs/article/details/16854747#)[?](http://blog.csdn.net/chszs/article/details/16854747#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75375/fork)
- HttpGet request = new HttpGet(SAMPLE_URL);  
- request.addHeader(HttpHeaders.ACCEPT, “application/xml”);  
- response = instance.execute(request);  
10、获取响应的HEADER部分
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16854747#)[copy](http://blog.csdn.net/chszs/article/details/16854747#)[print](http://blog.csdn.net/chszs/article/details/16854747#)[?](http://blog.csdn.net/chszs/article/details/16854747#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75375/fork)
- CloseableHttpResponse response = instance.execute(new HttpGet(SAMPLE_URL));  
- Header[] headers = response.getHeaders(HttpHeaders.CONTENT_TYPE);  
- assertThat(headers, not(emptyArray()));  
11、关闭或释放资源
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16854747#)[copy](http://blog.csdn.net/chszs/article/details/16854747#)[print](http://blog.csdn.net/chszs/article/details/16854747#)[?](http://blog.csdn.net/chszs/article/details/16854747#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75375/fork)
- response = instance.execute(new HttpGet(SAMPLE_URL));  
- try{  
-   HttpEntity entity = response.getEntity();  
- if(entity!=null){  
- InputStream instream = entity.getContent();  
- instream.close();  
-   }  
- } finally{  
-   response.close();  
- }  
以上内容涵盖了HttpClient 4.3所有常见的需求，供开发者参考。
# 《用Apache HttpClient实现URL重定向》
作者：chszs，转载需注明。博客主页：[http://blog.csdn.net/chszs](http://blog.csdn.net/chszs)
很多网站都使用了URL重定向技术，把一个原始请求从一个位置路由到另一个位置。原因可能是多方面的，比如域名转发、URL缩写、隐私保护、在同一网站维持相似的域名等。
本文讲述怎样使用Apache HTTPComponents HttpClient实现URL重定向。
本文使用的工具：
1. [Apache HttpComponents Client 4.3.1](http://mirror.bit.edu.cn/apache/httpcomponents/httpclient/binary/httpcomponents-client-4.3.1-bin.zip)
2. JDK 1.7
## 1、创建Java项目
项目我命名为HttpClientTest，导入如下JAR包：
![](https://img-blog.csdn.net/20131128004941234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hzenM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2、开发
1）创建和配置CloseableHttpClient
CloseableHttpClient是线程安全的，单个实例可用于处理多个HTTP请求。Http Client会自动处理所有的重定向，除非明确地使用disableAutomaticRetries()关闭自动重定向。
2）使用链接创建HttpGet实例，获取重定向。
3）创建本地HTTP执行上下文HttpClientContext。
4）使用Http Client并传递本地实例HttpClientContext，执行HttpGet请求。
5）成功执行请求后，使用上下文对象来获取所有的重定向位置。
6）关闭响应CloseableHttpResponse，释放资源。
**[java]**[view
 plain](http://blog.csdn.net/chszs/article/details/16996489#)[copy](http://blog.csdn.net/chszs/article/details/16996489#)[print](http://blog.csdn.net/chszs/article/details/16996489#)[?](http://blog.csdn.net/chszs/article/details/16996489#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/85803/fork)
- package com.ch.net;  
- 
- import java.io.IOException;  
- import java.net.URI;  
- import java.util.List;  
- 
- import org.apache.http.client.ClientProtocolException;  
- import org.apache.http.client.config.CookieSpecs;  
- import org.apache.http.client.config.RequestConfig;  
- import org.apache.http.client.methods.CloseableHttpResponse;  
- import org.apache.http.client.methods.HttpGet;  
- import org.apache.http.client.protocol.HttpClientContext;  
- import org.apache.http.impl.client.CloseableHttpClient;  
- import org.apache.http.impl.client.HttpClients;  
- 
- publicclass UrlRedirectionDemo {  
- // 浏览器Agent
- publicstatic String USER_AGENT = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_3) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1025.151 Safari/535.19";  
- 
- // 创建并配置HttpClient
- privatestaticfinal CloseableHttpClient httpClient = HttpClients  
-             .custom()  
-             .setUserAgent(USER_AGENT)  
-             .setDefaultRequestConfig(  
-                     RequestConfig.custom()  
-                             .setCookieSpec(CookieSpecs.BROWSER_COMPATIBILITY)  
-                             .build()).build();  
- 
- /**
-      * 根据给定的链接获取所有的重定向位置
-      * @param link 给定的链接
-      * @return
-      * @throws ClientProtocolException
-      * @throws IOException
-      */
- public List<URI> getAllRedirectLocations(String link) throws ClientProtocolException, IOException{  
-         List<URI> redirectLocations = null;  
-         CloseableHttpResponse response = null;  
- try{  
-             HttpClientContext context = HttpClientContext.create();  
-             HttpGet httpGet = new HttpGet(link);  
-             response = httpClient.execute(httpGet, context);  
- 
- // 获取所有的重定向位置
-             redirectLocations = context.getRedirectLocations();  
-         } finally{  
- if(response!=null){  
-                 response.close();  
-             }  
-         }  
- return redirectLocations;  
-     }  
- 
- publicstaticvoid main(String[] args) throws ClientProtocolException, IOException{  
- // 输入URL
-         String link = "http://t.cn/zjYwrl3";  
-         UrlRedirectionDemo demo = new UrlRedirectionDemo();  
-         List<URI> allRedirectLocations = demo.getAllRedirectLocations(link);  
- if(allRedirectLocations!=null){  
-             System.out.println(link);  
- for(URI uri : allRedirectLocations){  
-                 System.out.println("|\nv\n" + uri.toASCIIString());  
-             }  
-         } else{  
-             System.out.println("Not found!");  
-         }  
-     }  
- }  
如果使用默认的User-Agent设置，有些网站会返回HTTP 500状态码错误。一旦网站返回200状态码而且返回的HTML的内容是“500 server error”时，为保证兼容性，应该使用标准的Web浏览器的User-Agent字符串。
500 – 服务器内部错误
200 - 服务器成功返回网页
## 3、运行
我在新浪微博中找了个URL缩短的地址作为输入，执行后，果然找到了重定向地址。
控制台输出为：
**[python]**[view
 plain](http://blog.csdn.net/chszs/article/details/16996489#)[copy](http://blog.csdn.net/chszs/article/details/16996489#)[print](http://blog.csdn.net/chszs/article/details/16996489#)[?](http://blog.csdn.net/chszs/article/details/16996489#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/85803/fork)
- http://t.cn/zjYwrl3  
- |  
- v  
- http://hero.pongo.cn/  
## 4、验证
用在线URL重定向检测工具测试：
![](https://img-blog.csdn.net/20131128005353562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hzenM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
验证OK。

最近用到了HttpClient写爬虫，可能我有新版本强迫症，老是喜欢用新版本的东西(虽说新版本不一定好用),然后就用了HttpClient 4.3。HttpClient这货和Lucene一样，每个版本的API都变化很大，这有点让人头疼。就好比创建一个HttpClient对象吧，每一个版本的都不一样，
3.X是这样的
1
```java
HttpClient
 httpClient=
```
```java
new
```
```java
DefaultHttpClient();
```
```java
```
4.3是这样的
CloseableHttpClient
 httpClient = HttpClients.createDefault();
当然，上面这些变化只不过是一些小变化，大家看看API大家就都会了。
我要讲的是超时设置,HttpClient有三种超时设置，最近比较忙，没时间具体归纳总结，以后再补上，我这里就讲一些最简单最易用的超时设置方法。
这是个3.X的超时设置方法
[?](http://my.oschina.net/u/577453/blog/173724#)
```
```java
HttpClient
 client =
```
```java
new
```
```java
HttpClient();
```
```java
client.setConnectionTimeout(
```
```java
30000
```
```java
);
```
```java
client.setTimeout(
```
```java
30000
```
```java
);
```
```
[?](http://my.oschina.net/u/577453/blog/173724#)
```
```java
HttpClient
 httpClient=
```
```java
new
```
```java
HttpClient();
```
```java
httpClient.getHttpConnectionManager().getParams().setConnectionTimeout(
```
```java
5000
```
```java
);
```
```
4.X版本的超时设置(4.3后已过时)
[?](http://my.oschina.net/u/577453/blog/173724#)
```
```java
HttpClient
 httpClient=
```
```java
new
```
```java
DefaultHttpClient();
```
```java
httpClient.getParams().setParameter(CoreConnectionPNames.CONNECTION_TIMEOUT,
```
```java
2000
```
```java
);
```
```java
//连接时间
```
```java
httpClient.getParams().setParameter(CoreConnectionPNames.SO_TIMEOUT,
```
```java
2000
```
```java
);
```
```java
//数据传输时间
```
```
4.3版本超时设置
[?](http://my.oschina.net/u/577453/blog/173724#)
```
```java
CloseableHttpClient
 httpClient = HttpClients.createDefault();
```
```java
HttpGet
 httpGet=
```
```java
new
```
```java
HttpGet(
```
```java
"http://www.baidu.com"
```
```java
);//HTTP
 Get请求(POST雷同)
```
```java
RequestConfig
 requestConfig = RequestConfig.custom().setSocketTimeout(
```
```java
2000
```
```java
).setConnectTimeout(
```
```java
2000
```
```java
).build();
```
```java
//设置请求和传输超时时间
```
```java
httpGet.setConfig(requestConfig);
```
```java
httpClient.execute(httpGet);
```
```java
//执行请求
```
```
BTW,4.3版本不设置超时的话，一旦服务器没有响应，等待时间N久(>24小时)。


