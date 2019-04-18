# HttpClient连接池抛出大量ConnectionPoolTimeoutException: Timeout waiting for connection异常排查 - z69183787的专栏 - CSDN博客
2018年06月08日 14:11:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：344
[https://blog.csdn.net/shootyou/article/details/6615051](https://blog.csdn.net/shootyou/article/details/6615051)
今天解决了一个HttpClient的异常，汗啊，一个HttpClient使用稍有不慎都会是毁灭级别的啊。
这里有之前因为route配置不当导致服务器异常的一个处理：[http://blog.csdn.net/shootyou/article/details/6415248](http://blog.csdn.net/shootyou/article/details/6415248)
里面的HttpConnectionManager实现就是我在这里使用的实现。
问题表现：
tomcat后台日志发现大量异常
[plain][view plain](https://blog.csdn.net/shootyou/article/details/6615051#)[copy](https://blog.csdn.net/shootyou/article/details/6615051#)
- org.apache.http.conn.ConnectionPoolTimeoutException: Timeout waiting for connection  
时间一长tomcat就无法继续处理其他请求，从假死变成真死了。
linux运行：
[plain][view plain](https://blog.csdn.net/shootyou/article/details/6615051#)[copy](https://blog.csdn.net/shootyou/article/details/6615051#)
- netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'  
发现CLOSE_WAIT的数量始终在400以上，一直没降过。
问题分析：
一开始我对我的HttpClient使用过程深信不疑，我不认为异常是来自这里。
所以我开始从TCP的连接状态入手，猜测可能导致异常的原因。以前经常遇到TIME_WAIT数过大导致的服务器异常，很容易解决，修改下sysctl就ok了。但是这次是CLOSE_WAIT，是完全不同的概念了。
关于TIME_WAIT和CLOSE_WAIT的区别和异常处理我会单独起一篇文章详细说说我的理解。
简单来说CLOSE_WAIT数目过大是由于被动关闭连接处理不当导致的。
我说一个场景，服务器A会去请求服务器B上面的apache获取文件资源，正常情况下，如果请求成功，那么在抓取完资源后服务器A会主动发出关闭连接的请求，这个时候就是主动关闭连接，连接状态我们可以看到是TIME_WAIT。如果一旦发生异常呢？假设请求的资源服务器B上并不存在，那么这个时候就会由服务器B发出关闭连接的请求，服务器A就是被动的关闭了连接，如果服务器A被动关闭连接之后自己并没有释放连接，那就会造成CLOSE_WAIT的状态了。
所以很明显，问题还是处在程序里头。
先看看我的HttpConnectionManager实现：
[java][view plain](https://blog.csdn.net/shootyou/article/details/6615051#)[copy](https://blog.csdn.net/shootyou/article/details/6615051#)
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
看到没MAX_ROUTE_CONNECTIONS 正好是400，跟CLOSE_WAIT非常接近啊，难道是巧合？继续往下看。
然后看看调用它的代码是什么样的：
[java][view plain](https://blog.csdn.net/shootyou/article/details/6615051#)[copy](https://blog.csdn.net/shootyou/article/details/6615051#)
- publicstatic String readNet (String urlPath)  
-     {  
-         StringBuffer sb = new StringBuffer ();  
-         HttpClient client = null;  
-         InputStream in = null;  
-         InputStreamReader isr = null;  
- try
-         {  
-             client = HttpConnectionManager.getHttpClient();  
-             HttpGet get = new HttpGet();  
-             get.setURI(new URI(urlPath));  
-             HttpResponse response = client.execute(get);  
- if (response.getStatusLine ().getStatusCode () != 200) {  
- returnnull;  
-             }  
-             HttpEntity entity =response.getEntity();  
- 
- if( entity != null ){  
-                 in = entity.getContent();  
-                 .....  
-             }  
- return sb.toString ();  
- 
-         }  
- catch (Exception e)  
-         {  
-             e.printStackTrace ();  
- returnnull;  
-         }  
- finally
-         {  
- if (isr != null){  
- try
-                 {  
-                     isr.close ();  
-                 }  
- catch (IOException e)  
-                 {  
-                     e.printStackTrace ();  
-                 }  
-             }  
- if (in != null){  
- try
-                 {  
-                     <span style="color:#ff0000;">in.close ();</span>  
-                 }  
- catch (IOException e)  
-                 {  
-                     e.printStackTrace ();  
-                 }  
-             }  
-         }  
-     }  
很简单，就是个远程读取中文页面的方法。值得注意的是这一段代码是后来某某同学加上去的，看上去没啥问题，是用于非200状态的异常处理：
[java][view plain](https://blog.csdn.net/shootyou/article/details/6615051#)[copy](https://blog.csdn.net/shootyou/article/details/6615051#)
- if (response.getStatusLine ().getStatusCode () != 200) {  
- returnnull;  
-             }  
代码本身没有问题，但是问题是放错了位置。如果这么写的话就没问题：
[java][view plain](https://blog.csdn.net/shootyou/article/details/6615051#)[copy](https://blog.csdn.net/shootyou/article/details/6615051#)
- client = HttpConnectionManager.getHttpClient();  
-             HttpGet get = new HttpGet();  
-             get.setURI(new URI(urlPath));  
-             HttpResponse response = client.execute(get);  
- 
-             HttpEntity entity =response.getEntity();  
- 
- if( entity != null ){  
-                 in = entity.getContent();  
-             ..........  
-             }  
- 
- if (response.getStatusLine ().getStatusCode () != 200) {  
- returnnull;  
-             }  
- return sb.toString ();  
看出毛病了吧。在这篇入门（[HttpClient4.X 升级 入门 + http连接池使用](http://blog.csdn.net/shootyou/article/details/6415248)）里头我提到了HttpClient4使用我们常用的InputStream.close()来确认连接关闭，前面那种写法InputStream in 根本就不会被赋值，意味着一旦出现非200的连接，这个连接将永远僵死在连接池里头，太恐怖了。。。所以我们看到CLOST_WAIT数目为400，因为对一个路由的连接已经完全被僵死连接占满了。。。
其实上面那段代码还有一个没处理好的地方，异常处理不够严谨，所以最后我把代码改成了这样：
[java][view plain](https://blog.csdn.net/shootyou/article/details/6615051#)[copy](https://blog.csdn.net/shootyou/article/details/6615051#)
- publicstatic String readNet (String urlPath)  
-     {  
-         StringBuffer sb = new StringBuffer ();  
-         HttpClient client = null;  
-         InputStream in = null;  
-         InputStreamReader isr = null;  
-         HttpGet get = new HttpGet();  
- try
-         {  
-             client = HttpConnectionManager.getHttpClient();  
-             get.setURI(new URI(urlPath));  
-             HttpResponse response = client.execute(get);  
- if (response.getStatusLine ().getStatusCode () != 200) {  
-                 get.abort();  
- returnnull;  
-             }  
-             HttpEntity entity =response.getEntity();  
- 
- if( entity != null ){  
-                 in = entity.getContent();  
-                 ......  
-             }  
- return sb.toString ();  
- 
-         }  
- catch (Exception e)  
-         {  
-             get.abort();  
-             e.printStackTrace ();  
- returnnull;  
-         }  
- finally
-         {  
- if (isr != null){  
- try
-                 {  
-                     isr.close ();  
-                 }  
- catch (IOException e)  
-                 {  
-                     e.printStackTrace ();  
-                 }  
-             }  
- if (in != null){  
- try
-                 {  
-                     in.close ();  
-                 }  
- catch (IOException e)  
-                 {  
-                     e.printStackTrace ();  
-                 }  
-             }  
-         }  
-     }  
显示调用HttpGet的abort，这样就会直接中止这次连接，我们在遇到异常的时候应该显示调用，因为谁能保证异常是在InputStream in赋值之后才抛出的呢。
好了 ，分析完毕，明天准备总结下CLOSE_WAIT和TIME_WAIT的区别。
