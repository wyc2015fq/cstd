# Java 和 HTTP 的那些事（二） 使用代理 - 菜鸟路上的小白 - CSDN博客





2016年09月14日 13:38:43[somnus_小凯](https://me.csdn.net/u012486840)阅读数：8682









在上一篇博客[《模拟 HTTP 请求》](http://blog.csdn.net/u012486840/article/details/52537301l)中，我们分别介绍了两种方法来进行 HTTP 的模拟请求：`HttpURLConnection` 和 `HttpClient` ，到目前为止这两种方法都工作的很好，基本上可以实现我们需要的
 GET/POST 方法的模拟。对于一个爬虫来说，能发送 HTTP 请求，能获取页面数据，能解析网页内容，这相当于已经完成 80% 的工作了。只不过对于剩下的这 20% 的工作，还得花费我们另外 80% 的时间 :-)


在这篇博客里，我们将介绍剩下 20% 的工作中最为重要的一项：如何在 Java 中使用 HTTP 代理，代理也是爬虫技术中的重要一项。你如果要大规模的爬别人网页上的内容，必然会对人家的网站造成影响，如果你太拼了，就会遭人查封。要防止别人查封我们，我们要么将自己的程序分布到大量机器上去，但是对于资金和资源有限的我们来说这是很奢侈的；要么就使用代理技术，从网上捞一批代理，免费的也好收费的也好，或者购买一批廉价的 VPS 来搭建自己的代理服务器。关于如何搭建自己的代理服务器，后面有时间的话我再写一篇关于这个话题的博客。现在有了一大批代理服务器之后，就可以使用我们这篇博客所介绍的技术了。

## 一、简单的 HTTP 代理


我们先从最简单的开始，网上有很多免费代理，直接上百度搜索 “免费代理” 或者 “HTTP 代理” 就能找到很多（虽然网上能找到大量的免费代理，但它们的安全性已经有很多文章讨论过了，也有专门的文章对此进行调研的，譬如[这篇文章](http://www.freebuf.com/news/70733.html)，我在这里就不多作说明，如果你的爬虫爬取的信息并没有什么特别的隐私问题，可以忽略之，如果你的爬虫涉及一些例如模拟登录之类的功能，考虑到安全性，我建议你还是不要使用网上公开的免费代理，而是搭建自己的代理服务器比较靠谱）。

#### 1.1 HttpURLConnection 使用代理


HttpURLConnection 的 `openConnection()` 方法可以传入一个
 Proxy 参数，如下：



```
Proxy proxy =**new**Proxy(Proxy.Type.HTTP,**new**InetSocketAddress("127.0.0.1",9876));



URL obj =**new**URL(url);



HttpURLConnection con = (HttpURLConnection) obj.openConnection(proxy);
```




OK 了，就这么简单！


不仅如此，我们注意到 Proxy 构造函数的第一个参数为枚举类型 `Proxy.Type.HTTP` ，那么很显然，如果将其修改为 `Proxy.Type.SOCKS` 即可以使用
 SOCKS 代理。

#### 1.2 HttpClient 使用代理


由于 `HttpClient` 非常灵活，使用
 HttpClient 来连接代理有很多不同的方法。最简单的方法莫过于下面这样：



```
HttpHost proxy =**new**HttpHost("127.0.0.1",9876,"HTTP");



CloseableHttpClient httpclient = HttpClients.createDefault();



HttpGet request =**new**HttpGet(url);



CloseableHttpResponse response = httpclient.execute(proxy, request);
```




和上一篇中使用 HttpClient 发送请求的代码几乎一样，只是 `httpclient.execute()` 方法多加了一个参数，第一参数为 `HttpHost` 类型，我们这里设置成我们的代理即可。


这里要注意一点的是，虽然这里的 
```
new
 HttpHost()
```
 和上面的 
```
new
 Proxy()
```
 一样，也是可以指定协议类型的，但是遗憾的是 HttpClient 默认是不支持 SOCKS 协议的，如果我们使用下面的代码：



```
HttpHost
 proxy = **new**HttpHost("127.0.0.1",1080,"SOCKS");
```




将会直接报协议不支持异常：

> 
org.apache.http.conn.UnsupportedSchemeException: socks protocol is not supported



如果希望 HttpClient 支持 SOCKS 代理，可以参看这里：[How
 to use Socks 5 proxy with Apache HTTP Client 4?](http://stackoverflow.com/questions/22937983/how-to-use-socks-5-proxy-with-apache-http-client-4) 通过 HttpClient 提供的 ConnectionSocketFactory 类来实现。


虽然使用这种方式很简单，只需要加个参数就可以了，但是其实看 HttpClient 的代码注释，如下：



```
/*



* @param target   
 the target host for the request.



*                 
 Implementations may accept <code>null</code>



*                 
 if they can still determine a route, for example



*                 
 to a default target or by inspecting the request.



* @param request  
 the request to execute



*/
```




可以看到第一个参数 target 并不是代理，它的真实作用是 **执行请求的目标主机**，这个解释有点模糊，什么叫做 **执行请求的目标主机**？代理算不算**执行请求的目标主机**呢？因为按常理来讲，**执行请求的目标主机** 应该是要请求 URL 对应的站点才对。如果不算的话，为什么这里将 target 设置成代理也能正常工作？这个我也不清楚，还需要进一步研究下 HttpClient 的源码来深入了解下。


除了上面介绍的这种方式（自己写的，不推荐使用）来使用代理之外，HttpClient 官网还提供了几个示例，我将其作为推荐写法记录在此。


第一种写法是使用 [RequestConfig
 类](http://hc.apache.org/httpcomponents-client-ga/httpclient/examples/org/apache/http/examples/client/ClientExecuteProxy.java)，如下：



```
CloseableHttpClient httpclient = HttpClients.createDefault();      



HttpGet request =**new**HttpGet(url);



 



request.setConfig(



    RequestConfig.custom()



        .setProxy(**new**HttpHost("45.32.21.237",8888,"HTTP"))



        .build()



);



         



CloseableHttpResponse response = httpclient.execute(request);
```




第二种写法是使用 [RoutePlanner
 类](http://hc.apache.org/httpcomponents-client-4.5.x/tutorial/html/connmgmt.html#d5e485)，如下：



```
HttpHost proxy =**new**HttpHost("127.0.0.1",9876,"HTTP");



DefaultProxyRoutePlanner routePlanner
 = **new**DefaultProxyRoutePlanner(proxy);



CloseableHttpClient httpclient = HttpClients.custom()



        .setRoutePlanner(routePlanner)



        .build();



HttpGet request =**new**HttpGet(url);



CloseableHttpResponse response = httpclient.execute(request);
```



## 二、使用系统代理配置


我们在调试 HTTP 爬虫程序时，常常需要切换代理来测试，有时候直接使用系统自带的代理配置将是一种简单的方法。以前在做 .Net 项目时，程序默认使用 Internet 网络设置中配的代理，遗憾的是，我这里说的系统代理配置指的 JVM 系统，而不是操作系统，我还没找到简单的方法来让 Java 程序直接使用 Windows 系统下的代理配置。


尽管如此，系统代理使用起来还是很简单的。一般有下面两种方式可以设置 JVM 的代理配置：

#### 2.1 System.setProperty


Java 中的 `System` 类不仅仅是用来给我们 `System.out.println()` 打印信息的，它其实还有很多静态方法和属性可以用。其中`System.setProperty()` 就是比较常用的一个。


可以通过下面的方式来分别设置 HTTP 代理，HTTPS 代理和 SOCKS 代理：



```
// HTTP 代理，只能代理 HTTP
 请求



System.setProperty("http.proxyHost","127.0.0.1");



System.setProperty("http.proxyPort","9876");



 



// HTTPS 代理，只能代理 HTTPS
 请求



System.setProperty("https.proxyHost","127.0.0.1");



System.setProperty("https.proxyPort","9876");



 



// SOCKS 代理，支持 HTTP
 和 HTTPS 请求



// 注意：如果设置了 SOCKS 代理就不要设
 HTTP/HTTPS 代理



System.setProperty("socksProxyHost","127.0.0.1");



System.setProperty("socksProxyPort","1080");
```




这里有三点要说明：
- 系统默认先使用 HTTP/HTTPS 代理，如果既设置了 HTTP/HTTPS 代理，又设置了 SOCKS 代理，SOCKS 代理会起不到作用
- 由于历史原因，注意 `socksProxyHost` 和 `socksProxyPort` 中间没有小数点
- HTTP 和 HTTPS 代理可以合起来缩写，如下：



```
// 同时支持代理 HTTP/HTTPS
 请求



System.setProperty("proxyHost","127.0.0.1");



System.setProperty("proxyPort","9876");
```



#### 2.2 JVM 命令行参数


可以使用 `System.setProperty()` 方法来设置系统代理，也可以直接将这些参数通过
 JVM 的命令行参数来指定。如果你使用的是 Eclipse ，可以按下面的步骤来设置：
- 按顺序打开：Window -> Preferences -> Java -> Installed JREs -> Edit
- 在 Default VM arguments 中填写参数： `-DproxyHost=127.0.0.1 -DproxyPort=9876`

![jvm-arguments.jpg](https://img-blog.csdn.net/20160921153401140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 2.3 使用系统代理


上面两种方法都可以设置系统，下面要怎么在程序中自动使用系统代理呢？


对于 `HttpURLConnection` 类来说，程序不用做任何变动，它会默认使用系统代理。但是 `HttpClient` 默认是不使用系统代理的，如果想让它默认使用系统代理，可以通过 `SystemDefaultRoutePlanner` 和 `ProxySelector` 来设置。示例代码如下：



```
```java
SystemDefaultRoutePlanner
 routePlanner =
```

```java
new
```

```java
SystemDefaultRoutePlanner(ProxySelector.getDefault());
```



```java
CloseableHttpClient
 httpclient = HttpClients.custom()
```



```java

```

```java
.setRoutePlanner(routePlanner)
```



```java

```

```java
.build();
```



```java
HttpGet
 request =
```

```java
new
```

```java
HttpGet(url);
```



```java
CloseableHttpResponse
 response = httpclient.execute(request);
```
```





