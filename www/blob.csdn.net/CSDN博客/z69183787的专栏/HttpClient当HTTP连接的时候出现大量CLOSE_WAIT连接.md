# HttpClient当HTTP连接的时候出现大量CLOSE_WAIT连接 - z69183787的专栏 - CSDN博客
2018年06月08日 15:00:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：868
> 
ESTABLISHED 表示正在进行网络连接的数量
TIME_WAIT 表示表示等待系统主动关闭网络连接的数量
CLOSE_WAIT 表示被动等待程序关闭的网络连接数量
上篇文章给出了解决TIME_WAIT太多的方法，本篇文章以HttpClient为例说明解决大量CLOSE_WAIT状态的方法。
![](https://img-blog.csdn.net/20180608150003170?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
HttpClient是大量使用的用于HTTP连接的包，首先需要说明的是HttpClient 3.x和4.x之间API差距很多，不过强烈建议使用4.x的版本。除此之外，4.x中每个x之间也有一些差别（比如一些弃用的类，新增加的类等），这里以4.2.3版本进行说明。
HttpClient使用的HTTP 1.1协议进行连接，相对于HTTP 1.0来说有一个持续连接的增强，为了充分利用持续连接的特性，在一次连接结束之后，即使将HttpResponse使用close方法关闭，并且将调用了HttpGet或HttpPost的releaseConnection方法，示例代码如下：
- HttpGetmethod = null;
- HttpResponseresponse = null;
- try{
- method = newHttpGet(url);
- response = client.execute(method);
- }catch(Exceptione){
- 
- }finally{
- if(response != null){
- EntityUtils.consumeQuietly(response.getEntity());
- }
- if(method != null){
- method.releaseConnection();
- }
- }
这个时候仍然发现连接处于CLOSE_WAIT状态，这是因为HttpClient在执行close的时候，如果发现Response的Header中Connection是Keep-alive则连接不会关闭，以便下次请求相同网站的时候进行复用，这是产生CLOSE_WAIT连接的原因所在。
最简单的一种解决方法在execute方法之前增加Connection: close头信息，HTTP协议关于这个属性的定义如下：
HTTP/1.1 defines the "close" connection option for the sender to signal that the connection will be closed after completion of the response. For example：
	Connection: close 
示例代码如下：
- HttpGetmethod = null;
- HttpResponseresponse = null;
- try{
- method = newHttpGet(url);
- method.setHeader(HttpHeaders.CONNECTION, "close");
- response = client.execute(method);
- }catch(Exceptione){
- 
- }finally{
- if(response != null){
- EntityUtils.consumeQuietly(response.getEntity());
- }
- if(method != null){
- method.releaseConnection();
- }
- }
当然，也有人建议每次请求之后关闭client，但这一点不符合HttpClient设计的原则——复用。如果每次连接完成之后就关闭连接，效率太低了。因此，需要使用PoolingClientConnectionManager，并且设置maxTotal（整个连接池里面最大连接数，默认为20）和defaultMaxPerRoute（每个主机的最大连接数，默认为2），另外client还有一个ClientPNames.CONN_MANAGER_TIMEOUT参数，用来设置当连接不够获取新连接等待的超时时间，默认和CoreConnectionPNames.CONNECTION_TIMEOUT相同。可以根据实际情况对PoolingClientConnectionManager进行设置，以达到效率最优。
还有一种情况也会造成大量CLOSE_WAIT连接，即HttpResponse的状态码不是200的时候，需要及时调用method.abort()方法对连接进行释放，详细可以参考[这篇文章](http://blog.csdn.net/shootyou/article/details/6615051)。
参考资料：
[使用httpclient必须知道的参数设置及代码写法、存在的风险](http://jinnianshilongnian.iteye.com/blog/2089792)
[解决:HttpClient导致应用出现过多Close_Wait的问题](http://www.cnblogs.com/jessezeng/p/5616518.html)
[Using HttpClient Properly to Avoid CLOSE_WAIT TCP Connections](https://doc.nuxeo.com/blog/using-httpclient-properly-avoid-closewait-tcp-connections/)
[close_wait troubleshooting](http://zim.logdown.com/posts/636222)
[Using HttpClient properly to avoid CLOSE_WAIT TCP connections](http://www.cnblogs.com/javawebsoa/p/3223818.html)
[HttpClient连接池抛出大量ConnectionPoolTimeoutException: Timeout waiting for connection异常排查](http://blog.csdn.net/shootyou/article/details/6615051)
[httpclient4.2.1 连接池](https://my.oschina.net/greki/blog/83350)
[爬虫简单示例，用httpClient4.2.1实现（转载）](http://www.cnblogs.com/chenying99/archive/2012/10/24/2737924.html)
