# HttpClient4.X 连接池配置项 说明 - z69183787的专栏 - CSDN博客
2018年06月08日 14:12:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：100
[https://blog.csdn.net/shootyou/article/details/6415248](https://blog.csdn.net/shootyou/article/details/6415248)
在一次服务器异常的排查过程当中（服务器异常排查的过程我会另起文章），我们决定使用HttpClient4.X替代HttpClient3.X或者HttpConnection。
为什么使用HttpClient4？主要是HttpConnection没有连接池的概念，多少次请求就会建立多少个IO，在访问量巨大的情况下服务器的IO可能会耗尽。
HttpClient3也有连接池的东西在里头，使用MultiThreadedHttpConnectionManager，大致过程如下：
[java][view plain](https://blog.csdn.net/shootyou/article/details/6415248#)[copy](https://blog.csdn.net/shootyou/article/details/6415248#)
- MultiThreadedHttpConnectionManager connectionManager = new MultiThreadedHttpConnectionManager();  
- HttpClient client = new HttpClient(connectionManager);...// 在某个线程中。
- GetMethod get = new GetMethod("http://jakarta.apache.org/");  
- try {  
- client.executeMethod(get);// print response to stdout
- System.out.println(get.getResponseBodyAsStream());  
- } finally {  
- // be sure the connection is released back to the connection 
- managerget.releaseConnection();  
- }  
可以看出来，它的方式与jdbc连接池的使用方式相近，我觉得比较不爽的就是需要手动调用releaseConnection去释放连接。对每一个HttpClient.executeMethod须有一个method.releaseConnection()与之匹配。
HttpClient4在这点上做了改进，使用我们常用的InputStream.close()来确认连接关闭（4.1版本之前使用entity.consumeContent()来确认内容已经被消耗关闭连接）。具体方式如下：
[java][view plain](https://blog.csdn.net/shootyou/article/details/6415248#)[copy](https://blog.csdn.net/shootyou/article/details/6415248#)
- ...HttpClient client = null;InputStream in = null;  
- try{  
- client = HttpConnectionManager.getHttpClient();  
- HttpGet get = new HttpGet();  
- get.setURI(new URI(urlPath));  
- HttpResponse response = client.execute(get);  
- HttpEntity entity =response.getEntity();  
- if( entity != null ){   
-  in = entity.getContent();  
-  ....  
- }catch (Exception e){  
- ....  
- }finally{  
- if (in != null){  
- try{in.close ();}catch (IOException e){  
- e.printStackTrace ();  
- }  
- }  
- }  
### 2012-03-06更新：
有网友提出调用in.close()是否会关闭底层socket，事情是这样的：
[plain][view plain](https://blog.csdn.net/shootyou/article/details/6415248#)[copy](https://blog.csdn.net/shootyou/article/details/6415248#)
- 回复kangkang203：感谢你提出的这个问题。  
- 首先我文中提出的方法in.close()它会触发一个连接的释放这个连接将重新被连接管理器收回，官网的原文是这么说的：“Closing the input stream will trigger connection release...the underlying connection gets released back to the connection manager”。但是底层的socket是否会被关闭是不一定的，我看了部分源码（EofSensorInputStream）发现，大多数情况socket并不会关闭，而是否关闭socket貌似是由一个Watcher去决定的。所以in.close的调用不会引起socket的关闭。  
- 另外，由于http本身我们把它当做“短连接”，所以在一次请求交互完成后仍然打开socket的意义不是很大，毕竟它不像长连接那样在一个连接建立之后会有很多次数据交互。我们试用连接管理器的更多意义在于它对连接的管理。  
好说完了连接池的使用流程，现在来说一说连接池在使用时最重要的几个参数。我用4.1的版本实现了一个简单的HttpConnectionManager，代码如下：
[java][view plain](https://blog.csdn.net/shootyou/article/details/6415248#)[copy](https://blog.csdn.net/shootyou/article/details/6415248#)
- publicclass HttpConnectionManager {   
- 
- privatestatic HttpParams httpParams;  
- privatestatic ClientConnectionManager connectionManager;  
- 
- /**
-      * 最大连接数
-      */
- publicfinalstaticint MAX_TOTAL_CONNECTIONS = 800;  
- /**
-      * 获取连接的最大等待时间
-      */
- publicfinalstaticint WAIT_TIMEOUT = 60000;  
- /**
-      * 每个路由最大连接数
-      */
- publicfinalstaticint MAX_ROUTE_CONNECTIONS = 400;  
- /**
-      * 连接超时时间
-      */
- publicfinalstaticint CONNECT_TIMEOUT = 10000;  
- /**
-      * 读取超时时间
-      */
- publicfinalstaticint READ_TIMEOUT = 10000;  
- 
- static {  
-         httpParams = new BasicHttpParams();  
- // 设置最大连接数
-         ConnManagerParams.setMaxTotalConnections(httpParams, MAX_TOTAL_CONNECTIONS);  
- // 设置获取连接的最大等待时间
-         ConnManagerParams.setTimeout(httpParams, WAIT_TIMEOUT);  
- // 设置每个路由最大连接数
-         ConnPerRouteBean connPerRoute = new ConnPerRouteBean(MAX_ROUTE_CONNECTIONS);  
-         ConnManagerParams.setMaxConnectionsPerRoute(httpParams,connPerRoute);  
- // 设置连接超时时间
-         HttpConnectionParams.setConnectionTimeout(httpParams, CONNECT_TIMEOUT);  
- // 设置读取超时时间
-         HttpConnectionParams.setSoTimeout(httpParams, READ_TIMEOUT);  
- 
-         SchemeRegistry registry = new SchemeRegistry();  
-         registry.register(new Scheme("http", PlainSocketFactory.getSocketFactory(), 80));  
-         registry.register(new Scheme("https", SSLSocketFactory.getSocketFactory(), 443));  
- 
-         connectionManager = new ThreadSafeClientConnManager(httpParams, registry);  
-     }  
- 
- publicstatic HttpClient getHttpClient() {  
- returnnew DefaultHttpClient(connectionManager, httpParams);  
-     }  
- 
- }  
最大连接数、获取连接的最大等待时间、读取超时时间 这些配置应该比较容易理解，一般的连接池都会有这些配置，比较特别的是 每个路由(route)最大连接数 。
什么是一个route？
这里route的概念可以理解为 运行环境机器 到 目标机器的一条线路。举例来说，我们使用HttpClient的实现来分别请求 www.baidu.com 的资源和 www.bing.com 的资源那么他就会产生两个route。
这里为什么要特别提到route最大连接数这个参数呢，因为这个参数的默认值为2，如果不设置这个参数值默认情况下对于同一个目标机器的最大并发连接只有2个！这意味着如果你正在执行一个针对某一台目标机器的抓取任务的时候，哪怕你设置连接池的最大连接数为200，但是实际上还是只有2个连接在工作，其他剩余的198个连接都在等待，都是为别的目标机器服务的。
怎么样蛋疼吧，我是已经有过血的教训了，在切换到HttpClient4.1的起初没有注意到这个配置，最后使得服务承受的压力反而不如从前了，所以在这里特别提醒大家注意。
HttpClient4.X 教程下载：
[http://svn.apache.org/repos/asf/httpcomponents/httpclient/trunk/httpclient-contrib/docs/translated-tutorial/httpclient-tutorial-simplified-chinese.pdf](http://svn.apache.org/repos/asf/httpcomponents/httpclient/trunk/httpclient-contrib/docs/translated-tutorial/httpclient-tutorial-simplified-chinese.pdf)
关于版本的补充：
网友w2449008821提醒之后我才发现在HttpClient4.1+的版本ConnManagerParams已经被Deprecated了。
我在写这篇日志的时候时候的httpclient 版本是4.0.3，从4.0版本之后ConnManagerParams被Deprecated，没想到一个小版本升级会有这么大变化。
官网教程举例了新的连接池设置：
[java][view plain](https://blog.csdn.net/shootyou/article/details/6415248#)[copy](https://blog.csdn.net/shootyou/article/details/6415248#)
- SchemeRegistry schemeRegistry = new SchemeRegistry();  
- schemeRegistry.register(  
- new Scheme("http", 80, PlainSocketFactory.getSocketFactory()));  
- schemeRegistry.register(  
- new Scheme("https", 443, SSLSocketFactory.getSocketFactory()));  
- 
- ThreadSafeClientConnManager cm = new ThreadSafeClientConnManager(schemeRegistry);  
- // Increase max total connection to 200
- cm.setMaxTotalConnections(200);  
- // Increase default max connection per route to 20
- cm.setDefaultMaxPerRoute(20);  
- // Increase max connections for localhost:80 to 50
- HttpHost localhost = new HttpHost("locahost", 80);  
- cm.setMaxForRoute(new HttpRoute(localhost), 50);  
- 
- HttpClient httpClient = new DefaultHttpClient(cm);  
ConnManagerParams的功能被挪到了 ThreadSafeClientConnManager 和 HttpConnectionParams两个类：
|`static [ConnPerRoute](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/conn/params/ConnPerRoute.html)`|`[getMaxConnectionsPerRoute](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/conn/params/ConnManagerParams.html#getMaxConnectionsPerRoute(org.apache.http.params.HttpParams))([HttpParams](http://hc.apache.org/httpcomponents-core-ga/httpcore/apidocs/org/apache/http/params/HttpParams.html?is-external=true) params)`Deprecated.use [`ThreadSafeClientConnManager.getMaxForRoute(org.apache.http.conn.routing.HttpRoute)`](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/impl/conn/tsccm/ThreadSafeClientConnManager.html#getMaxForRoute(org.apache.http.conn.routing.HttpRoute))|
|----|----|
|`static int`|`[getMaxTotalConnections](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/conn/params/ConnManagerParams.html#getMaxTotalConnections(org.apache.http.params.HttpParams))([HttpParams](http://hc.apache.org/httpcomponents-core-ga/httpcore/apidocs/org/apache/http/params/HttpParams.html?is-external=true) params)`Deprecated.use [`ThreadSafeClientConnManager.getMaxTotal()`](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/impl/conn/tsccm/ThreadSafeClientConnManager.html#getMaxTotal())|
|`static long`|`[getTimeout](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/conn/params/ConnManagerParams.html#getTimeout(org.apache.http.params.HttpParams))([HttpParams](http://hc.apache.org/httpcomponents-core-ga/httpcore/apidocs/org/apache/http/params/HttpParams.html?is-external=true) params)`Deprecated.use [`HttpConnectionParams.getConnectionTimeout(HttpParams)`](http://hc.apache.org/httpcomponents-core-ga/httpcore/apidocs/org/apache/http/params/HttpConnectionParams.html?is-external=true#getConnectionTimeout(org.apache.http.params.HttpParams))|
|`static void`|`[setMaxConnectionsPerRoute](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/conn/params/ConnManagerParams.html#setMaxConnectionsPerRoute(org.apache.http.params.HttpParams,%20org.apache.http.conn.params.ConnPerRoute))([HttpParams](http://hc.apache.org/httpcomponents-core-ga/httpcore/apidocs/org/apache/http/params/HttpParams.html?is-external=true) params, [ConnPerRoute](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/conn/params/ConnPerRoute.html) connPerRoute)`Deprecated.use [`ThreadSafeClientConnManager.setMaxForRoute(org.apache.http.conn.routing.HttpRoute, int)`](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/impl/conn/tsccm/ThreadSafeClientConnManager.html#setMaxForRoute(org.apache.http.conn.routing.HttpRoute,%20int))|
|`static void`|`[setMaxTotalConnections](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/conn/params/ConnManagerParams.html#setMaxTotalConnections(org.apache.http.params.HttpParams,%20int))([HttpParams](http://hc.apache.org/httpcomponents-core-ga/httpcore/apidocs/org/apache/http/params/HttpParams.html?is-external=true) params, int maxTotalConnections)`Deprecated.use [`ThreadSafeClientConnManager.setMaxTotal(int)`](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/impl/conn/tsccm/ThreadSafeClientConnManager.html#setMaxTotal(int))|
|`static void`|`[setTimeout](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/conn/params/ConnManagerParams.html#setTimeout(org.apache.http.params.HttpParams,%20long))([HttpParams](http://hc.apache.org/httpcomponents-core-ga/httpcore/apidocs/org/apache/http/params/HttpParams.html?is-external=true) params, long timeout)`Deprecated.use [`HttpConnectionParams.setConnectionTimeout(HttpParams, int)`](http://hc.apache.org/httpcomponents-core-ga/httpcore/apidocs/org/apache/http/params/HttpConnectionParams.html?is-external=true#setConnectionTimeout(org.apache.http.params.HttpParams,%20int))|
参考：[http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/conn/params/ConnManagerParams.html](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/conn/params/ConnManagerParams.html)
[http://hc.apache.org/httpcomponents-client-ga/tutorial/html/connmgmt.html#d4e638](http://hc.apache.org/httpcomponents-client-ga/tutorial/html/connmgmt.html#d4e638)
