# HttpClient 4.3.X教程（转载） - 零度的博客专栏 - CSDN博客
2016年07月01日 16:38:39[零度anngle](https://me.csdn.net/zmx729618)阅读数：1171
前言
Http协议应该是互联网中最重要的协议。持续增长的web服务、可联网的家用电器等都在继承并拓展着Http协议，向着浏览器之外的方向发展。
虽然jdk中的java.net包中提供了一些基本的方法，通过http协议来访问网络资源，但是大多数场景下，它都不够灵活和强大。HttpClient致力于填补这个空白，它可以提供有效的、最新的、功能丰富的包来实现http客户端。
为了拓展，HttpClient即支持基本的http协议，还支持http-aware客户端程序，如web浏览器，Webservice客户端，以及利用or拓展http协议的分布式系统。
1、HttpClient的范围/特性
- 是一个基于HttpCore的客户端Http传输类库
- 基于传统的（阻塞）IO
- 内容无关
2、HttpClient不能做的事情
- HttpClient不是浏览器，它是一个客户端http协议传输类库。HttpClient被用来发送和接受Http消息。HttpClient不会处理http消息的内容，不会进行javascript解析，不会关心content type，如果没有明确设置，httpclient也不会对请求进行格式化、重定向url，或者其他任何和http消息传输相关的功能。
# 第一章 基本概念
## 1.1. 请求执行
HttpClient最基本的功能就是执行Http方法。一个Http方法的执行涉及到一个或者多个Http请求/Http响应的交互，通常这个过程都会自动被HttpClient处理，对用户透明。用户只需要提供Http请求对象，HttpClient就会将http请求发送给目标服务器，并且接收服务器的响应，如果http请求执行不成功，httpclient就会抛出异样。
下面是个很简单的http请求执行的例子：
- CloseableHttpClient httpclient = HttpClients.createDefault();  
- HttpGet httpget = new HttpGet("http://localhost/");  
- CloseableHttpResponse response = httpclient.execute(httpget);  
- try {  
-     <...>  
- } finally {  
-     response.close();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 1.1.1. HTTP请求
所有的Http请求都有一个请求行(request line)，包括方法名、请求的URI和Http版本号。
HttpClient支持HTTP/1.1这个版本定义的所有Http方法：`GET`,`HEAD`,`POST`,`PUT`,`DELETE`,TRACE`和`OPTIONS`。对于每一种http方法，HttpClient都定义了一个相应的类：`HttpGet`, `HttpHead`, `HttpPost`, `HttpPut`, `HttpDelete`, `HttpTrace`和`HttpOpquertions。
Request-URI即统一资源定位符，用来标明Http请求中的资源。Http request URIs包含协议名、主机名、主机端口（可选）、资源路径、query（可选）和片段信息（可选）。
- HttpGet httpget = new HttpGet(  
- "http://www.google.com/search?hl=en&q=httpclient&btnG=Google+Search&aq=f&oq=");  
![](http://static.blog.csdn.net/images/save_snippets.png)
HttpClient提供`URIBuilder`工具类来简化URIs的创建和修改过程。
- URI uri = new URIBuilder()  
-         .setScheme("http")  
-         .setHost("www.google.com")  
-         .setPath("/search")  
-         .setParameter("q", "httpclient")  
-         .setParameter("btnG", "Google Search")  
-         .setParameter("aq", "f")  
-         .setParameter("oq", "")  
-         .build();  
- HttpGet httpget = new HttpGet(uri);  
- System.out.println(httpget.getURI());  
![](http://static.blog.csdn.net/images/save_snippets.png)
上述代码会在控制台输出：
- http://www.google.com/search?q=httpclient&btnG=Google+Search&aq=f&oq=
![](http://static.blog.csdn.net/images/save_snippets.png)
### 1.1.2. HTTP响应
服务器收到客户端的http请求后，就会对其进行解析，然后把响应发给客户端，这个响应就是HTTP response.HTTP响应第一行是协议版本，之后是数字状态码和相关联的文本段。
- HttpResponse response = new BasicHttpResponse(HttpVersion.HTTP_1_1,   
- HttpStatus.SC_OK, "OK");  
- 
- System.out.println(response.getProtocolVersion());  
- System.out.println(response.getStatusLine().getStatusCode());  
- System.out.println(response.getStatusLine().getReasonPhrase());  
- System.out.println(response.getStatusLine().toString());  
![](http://static.blog.csdn.net/images/save_snippets.png)
上述代码会在控制台输出：
- HTTP/1.1
- 200
- OK  
- HTTP/1.1200 OK  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 1.1.3. 消息头
一个Http消息可以包含一系列的消息头，用来对http消息进行描述，比如消息长度，消息类型等等。HttpClient提供了方法来获取、添加、移除、枚举消息头。
- HttpResponse response = new BasicHttpResponse(HttpVersion.HTTP_1_1,   
-     HttpStatus.SC_OK, "OK");  
- response.addHeader("Set-Cookie",   
- "c1=a; path=/; domain=localhost");  
- response.addHeader("Set-Cookie",   
- "c2=b; path=\"/\", c3=c; domain=\"localhost\"");  
- Header h1 = response.getFirstHeader("Set-Cookie");  
- System.out.println(h1);  
- Header h2 = response.getLastHeader("Set-Cookie");  
- System.out.println(h2);  
- Header[] hs = response.getHeaders("Set-Cookie");  
- System.out.println(hs.length);  
![](http://static.blog.csdn.net/images/save_snippets.png)
上述代码会在控制台输出：
- Set-Cookie: c1=a; path=/; domain=localhost  
- Set-Cookie: c2=b; path="/", c3=c; domain="localhost"
- 2
![](http://static.blog.csdn.net/images/save_snippets.png)
最有效的获取指定类型的消息头的方法还是使用`HeaderIterator`接口。
- HttpResponse response = new BasicHttpResponse(HttpVersion.HTTP_1_1,   
-     HttpStatus.SC_OK, "OK");  
- response.addHeader("Set-Cookie",   
- "c1=a; path=/; domain=localhost");  
- response.addHeader("Set-Cookie",   
- "c2=b; path=\"/\", c3=c; domain=\"localhost\"");  
- 
- HeaderIterator it = response.headerIterator("Set-Cookie");  
- 
- while (it.hasNext()) {  
-     System.out.println(it.next());  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
上述代码会在控制台输出：
- Set-Cookie: c1=a; path=/; domain=localhost  
- Set-Cookie: c2=b; path="/", c3=c; domain="localhost"
![](http://static.blog.csdn.net/images/save_snippets.png)
HeaderIterator也提供非常便捷的方式，将Http消息解析成单独的消息头元素。
- HttpResponse response = new BasicHttpResponse(HttpVersion.HTTP_1_1,   
-     HttpStatus.SC_OK, "OK");  
- response.addHeader("Set-Cookie",   
- "c1=a; path=/; domain=localhost");  
- response.addHeader("Set-Cookie",   
- "c2=b; path=\"/\", c3=c; domain=\"localhost\"");  
- 
- HeaderElementIterator it = new BasicHeaderElementIterator(  
-     response.headerIterator("Set-Cookie"));  
- 
- while (it.hasNext()) {  
-     HeaderElement elem = it.nextElement();   
-     System.out.println(elem.getName() + " = " + elem.getValue());  
-     NameValuePair[] params = elem.getParameters();  
- for (int i = 0; i < params.length; i++) {  
-         System.out.println(" " + params[i]);  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
上述代码会在控制台输出：
- c1 = a  
- path=/  
- domain=localhost  
- c2 = b  
- path=/  
- c3 = c  
- domain=localhost  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 1.1.4. HTTP实体
Http消息可以携带http实体，这个http实体既可以是http请求，也可以是http响应的。Http实体，可以在某些http请求或者响应中发现，但不是必须的。Http规范中定义了两种包含请求的方法：POST和PUT。HTTP响应一般会包含一个内容实体。当然这条规则也有异常情况，如Head方法的响应，204没有内容，304没有修改或者205内容资源重置。
HttpClient根据来源的不同，划分了三种不同的Http实体内容。
- streamed流式: 内容是通过流来接受或者在运行中产生。特别是，streamed这一类包含从http响应中获取的实体内容。一般说来，streamed实体是不可重复的。
- self-contained自我包含式:内容在内存中或通过独立的连接或其它实体中获得。self-contained类型的实体内容通常是可重复的。这种类型的实体通常用于关闭http请求。
- wrapping包装式: 这种类型的内容是从另外的http实体中获取的。
当从Http响应中读取内容时，上面的三种区分对于连接管理器来说是非常重要的。对于由应用程序创建而且只使用HttpClient发送的请求实体，streamed和self-contained两种类型的不同就不那么重要了。这种情况下，建议考虑如streamed流式这种不能重复的实体，和可以重复的self-contained自我包含式实体。
#### 1.1.4.1. 可重复的实体
一个实体是可重复的，也就是说它的包含的内容可以被多次读取。这种多次读取只有self contained（自包含）的实体能做到（比如`ByteArrayEntity`或者`StringEntity`)。
#### 1.1.4.2. 使用Http实体
由于一个Http实体既可以表示二进制内容，又可以表示文本内容，所以Http实体要支持字符编码（为了支持后者，即文本内容）。
当需要执行一个完整内容的Http请求或者Http请求已经成功，服务器要发送响应到客户端时，Http实体就会被创建。
如果要从Http实体中读取内容，我们可以利用`HttpEntity`类的`getContent`方法来获取实体的输入流（`java.io.InputStream`)，或者利用`HttpEntity`类的`writeTo(OutputStream)`方法来获取输出流，这个方法会把所有的内容写入到给定的流中。
当实体类已经被接受后，我们可以利用`HttpEntity`类的`getContentType()`和`getContentLength()`方法来读取`Content-Type`和`Content-Length`两个头消息（如果有的话）。由于`Content-Type`包含mime-types的字符编码，比如text/plain或者text/html,`HttpEntity`类的`getContentEncoding()`方法就是读取这个编码的。如果头信息不存在，`getContentLength（）`会返回-1,`getContentType()`会返回NULL。如果`Content-Type`信息存在，就会返回一个`Header`类。
当为发送消息创建Http实体时，需要同时附加meta信息。
- StringEntity myEntity = new StringEntity("important message",   
-    ContentType.create("text/plain", "UTF-8"));  
- 
- System.out.println(myEntity.getContentType());  
- System.out.println(myEntity.getContentLength());  
- System.out.println(EntityUtils.toString(myEntity));  
- System.out.println(EntityUtils.toByteArray(myEntity).length);  
![](http://static.blog.csdn.net/images/save_snippets.png)
上述代码会在控制台输出：
- Content-Type: text/plain; charset=utf-8
- 17
- important message  
- 17
![](http://static.blog.csdn.net/images/save_snippets.png)
### 1.1.5. 确保底层的资源连接被释放
为了确保系统资源被正确地释放，我们要么管理Http实体的内容流、要么关闭Http响应。
- CloseableHttpClient httpclient = HttpClients.createDefault();  
- HttpGet httpget = new HttpGet("http://localhost/");  
- CloseableHttpResponse response = httpclient.execute(httpget);  
- try {  
-     HttpEntity entity = response.getEntity();  
- if (entity != null) {  
-         InputStream instream = entity.getContent();  
- try {  
- // do something useful
-         } finally {  
-             instream.close();  
-         }  
-     }  
- } finally {  
-     response.close();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
关闭Http实体内容流和关闭Http响应的区别在于，前者通过消耗掉Http实体内容来保持相关的http连接，然后后者会立即关闭、丢弃http连接。
请注意`HttpEntity`的`writeTo(OutputStream)`方法，当Http实体被写入到OutputStream后，也要确保释放系统资源。如果这个方法内调用了`HttpEntity`的`getContent()`方法，那么它会有一个`java.io.InpputStream`的实例，我们需要在finally中关闭这个流。
但是也有这样的情况，我们只需要获取Http响应内容的一小部分，而获取整个内容并、实现连接的可重复性代价太大，这时我们可以通过关闭响应的方式来关闭内容输入、输出流。
- CloseableHttpClient httpclient = HttpClients.createDefault();  
- HttpGet httpget = new HttpGet("http://localhost/");  
- CloseableHttpResponse response = httpclient.execute(httpget);  
- try {  
-     HttpEntity entity = response.getEntity();  
- if (entity != null) {  
-         InputStream instream = entity.getContent();  
- int byteOne = instream.read();  
- int byteTwo = instream.read();  
- // Do not need the rest
-     }  
- } finally {  
-     response.close();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
上面的代码执行后，连接变得不可用，所有的资源都将被释放。
### 1.1.6. 消耗HTTP实体内容
HttpClient推荐使用`HttpEntity`的`getConent()`方法或者`HttpEntity`的`writeTo(OutputStream)`方法来消耗掉Http实体内容。HttpClient也提供了`EntityUtils`这个类，这个类提供一些静态方法可以更容易地读取Http实体的内容和信息。和以`java.io.InputStream`流读取内容的方式相比，EntityUtils提供的方法可以以字符串或者字节数组的形式读取Http实体。但是，强烈不推荐使用`EntityUtils`这个类，除非目标服务器发出的响应是可信任的，并且http响应实体的长度不会过大。
- CloseableHttpClient httpclient = HttpClients.createDefault();  
- HttpGet httpget = new HttpGet("http://localhost/");  
- CloseableHttpResponse response = httpclient.execute(httpget);  
- try {  
-     HttpEntity entity = response.getEntity();  
- if (entity != null) {  
- long len = entity.getContentLength();  
- if (len != -1 && len < 2048) {  
-             System.out.println(EntityUtils.toString(entity));  
-         } else {  
- // Stream content out
-         }  
-     }  
- } finally {  
-     response.close();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
有些情况下，我们希望可以重复读取Http实体的内容。这就需要把Http实体内容缓存在内存或者磁盘上。最简单的方法就是把Http Entity转化成`BufferedHttpEntity`，这样就把原Http实体的内容缓冲到了内存中。后面我们就可以重复读取BufferedHttpEntity中的内容。
- CloseableHttpResponse response = <...>  
- HttpEntity entity = response.getEntity();  
- if (entity != null) {  
-     entity = new BufferedHttpEntity(entity);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 1.1.7. 创建HTTP实体内容
HttpClient提供了一些类，这些类可以通过http连接高效地输出Http实体内容。HttpClient提供的这几个类涵盖的常见的数据类型，如String，byte数组，输入流，和文件类型：`StringEntity`,`ByteArrayEntity`,`InputStreamEntity`,`FileEntity`。
- File file = new File("somefile.txt");  
- FileEntity entity = new FileEntity(file,   
-     ContentType.create("text/plain", "UTF-8"));          
- 
- HttpPost httppost = new HttpPost("http://localhost/action.do");  
- httppost.setEntity(entity);  
![](http://static.blog.csdn.net/images/save_snippets.png)
请注意由于`InputStreamEntity`只能从下层的数据流中读取一次，所以它是不能重复的。推荐，通过继承`HttpEntity`这个自包含的类来自定义HttpEntity类，而不是直接使用`InputStreamEntity`这个类。`FileEntity`就是一个很好的起点（FileEntity就是继承的HttpEntity）。
#### 1.7.1.1. HTML表单
很多应用程序需要模拟提交Html表单的过程，举个例子，登陆一个网站或者将输入内容提交给服务器。HttpClient提供了`UrlEncodedFormEntity`这个类来帮助实现这一过程。
- List<NameValuePair> formparams = new ArrayList<NameValuePair>();  
- formparams.add(new BasicNameValuePair("param1", "value1"));  
- formparams.add(new BasicNameValuePair("param2", "value2"));  
- UrlEncodedFormEntity entity = new UrlEncodedFormEntity(formparams, Consts.UTF_8);  
- HttpPost httppost = new HttpPost("http://localhost/handler.do");  
- httppost.setEntity(entity);  
![](http://static.blog.csdn.net/images/save_snippets.png)
`UrlEncodedFormEntity`实例会使用所谓的Url编码的方式对我们的参数进行编码，产生的结果如下：
- param1=value1&m2=value2  
![](http://static.blog.csdn.net/images/save_snippets.png)
#### 1.1.7.2. 内容分块
一般来说，推荐让HttpClient自己根据Http消息传递的特征来选择最合适的传输编码。当然，如果非要手动控制也是可以的，可以通过设置`HttpEntity`的`setChunked()`为true。请注意：HttpClient仅会将这个参数看成是一个建议。如果Http的版本（如http 1.0)不支持内容分块，那么这个参数就会被忽略。
- StringEntity entity = new StringEntity("important message",  
-         ContentType.create("plain/text", Consts.UTF_8));  
- entity.setChunked(true);  
- HttpPost httppost = new HttpPost("http://localhost/acrtion.do");  
- httppost.setEntity(entity);  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 1.1.8.RESPONSE HANDLERS
最简单也是最方便的处理http响应的方法就是使用`ResponseHandler`接口，这个接口中有`handleResponse(HttpResponse response)`方法。使用这个方法，用户完全不用关心http连接管理器。当使用`ResponseHandler`时，HttpClient会自动地将Http连接释放给Http管理器，即使http请求失败了或者抛出了异常。
- CloseableHttpClient httpclient = HttpClients.createDefault();  
- HttpGet httpget = new HttpGet("http://localhost/json");  
- 
- ResponseHandler<MyJsonObject> rh = new ResponseHandler<MyJsonObject>() {  
- 
- @Override
- public JsonObject handleResponse(  
- final HttpResponse response) throws IOException {  
-         StatusLine statusLine = response.getStatusLine();  
-         HttpEntity entity = response.getEntity();  
- if (statusLine.getStatusCode() >= 300) {  
- thrownew HttpResponseException(  
-                     statusLine.getStatusCode(),  
-                     statusLine.getReasonPhrase());  
-         }  
- if (entity == null) {  
- thrownew ClientProtocolException("Response contains no content");  
-         }  
-         Gson gson = new GsonBuilder().create();  
-         ContentType contentType = ContentType.getOrDefault(entity);  
-         Charset charset = contentType.getCharset();  
-         Reader reader = new InputStreamReader(entity.getContent(), charset);  
- return gson.fromJson(reader, MyJsonObject.class);  
-     }  
- };  
- MyJsonObject myjson = client.execute(httpget, rh);  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 1.2. HttpClient接口
对于Http请求执行过程来说，`HttpClient`的接口有着必不可少的作用。`HttpClient`接口没有对Http请求的过程做特别的限制和详细的规定，连接管理、状态管理、授权信息和重定向处理这些功能都单独实现。这样用户就可以更简单地拓展接口的功能（比如缓存响应内容）。
一般说来，HttpClient实际上就是一系列特殊的handler或者说策略接口的实现，这些handler（测试接口）负责着处理Http协议的某一方面，比如重定向、认证处理、有关连接持久性和keep alive持续时间的决策。这样就允许用户使用自定义的参数来代替默认配置，实现个性化的功能。
- ConnectionKeepAliveStrategy keepAliveStrat = new DefaultConnectionKeepAliveStrategy() {  
- 
- @Override
- publiclong getKeepAliveDuration(  
-             HttpResponse response,  
-             HttpContext context) {  
- long keepAlive = super.getKeepAliveDuration(response, context);  
- if (keepAlive == -1) {  
- // Keep connections alive 5 seconds if a keep-alive value
- // has not be explicitly set by the server
-             keepAlive = 5000;  
-         }  
- return keepAlive;  
-     }  
- 
- };  
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .setKeepAliveStrategy(keepAliveStrat)  
-         .build();  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 1.2.1.HTTPCLIENT的线程安全性
`HttpClient`已经实现了线程安全。所以希望用户在实例化HttpClient时，也要支持为多个请求使用。
### 1.2.2.HTTPCLIENT的内存分配
当一个`CloseableHttpClient`的实例不再被使用，并且它的作用范围即将失效，和它相关的连接必须被关闭，关闭方法可以调用`CloseableHttpClient`的`close()`方法。
- CloseableHttpClient httpclient = HttpClients.createDefault();  
- try {  
-     <...>  
- } finally {  
-     httpclient.close();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 1.3.Http执行上下文
最初，Http被设计成一种无状态的、面向请求-响应的协议。然而，在实际使用中，我们希望能够在一些逻辑相关的请求-响应中，保持状态信息。为了使应用程序可以保持Http的持续状态，HttpClient允许http连接在特定的Http上下文中执行。如果在持续的http请求中使用了同样的上下文，那么这些请求就可以被分配到一个逻辑会话中。HTTP上下文就和一个`java.util.Map<String, Object>`功能类似。它实际上就是一个任意命名的值的集合。应用程序可以在Http请求执行前填充上下文的值，也可以在请求执行完毕后检查上下文。
`HttpContext`可以包含任意类型的对象，因此如果在多线程中共享上下文会不安全。推荐每个线程都只包含自己的http上下文。
在Http请求执行的过程中，HttpClient会自动添加下面的属性到Http上下文中：
- `HttpConnection`的实例，表示客户端与服务器之间的连接
- `HttpHost`的实例，表示要连接的目标服务器
- `HttpRoute`的实例，表示全部的连接路由
- `HttpRequest`的实例，表示Http请求。在执行上下文中，最终的HttpRequest对象会代表http消息的状态。Http/1.0和Http/1.1都默认使用相对的uri。但是如果使用了非隧道模式的代理服务器，就会使用绝对路径的uri。
- `HttpResponse`的实例，表示Http响应
- `java.lang.Boolean`对象，表示是否请求被成功的发送给目标服务器
- `RequestConfig`对象，表示http request的配置信息
- `java.util.List<Uri>`对象，表示Http响应中的所有重定向地址
我们可以使用`HttpClientContext`这个适配器来简化和上下文交互的过程。
- HttpContext context = <...>  
- HttpClientContext clientContext = HttpClientContext.adapt(context);  
- HttpHost target = clientContext.getTargetHost();  
- HttpRequest request = clientContext.getRequest();  
- HttpResponse response = clientContext.getResponse();  
- RequestConfig config = clientContext.getRequestConfig();  
![](http://static.blog.csdn.net/images/save_snippets.png)
同一个逻辑会话中的多个Http请求，应该使用相同的Http上下文来执行，这样就可以自动地在http请求中传递会话上下文和状态信息。
在下面的例子中，我们在开头设置的参数，会被保存在上下文中，并且会应用到后续的http请求中。
- CloseableHttpClient httpclient = HttpClients.createDefault();  
- RequestConfig requestConfig = RequestConfig.custom()  
-         .setSocketTimeout(1000)  
-         .setConnectTimeout(1000)  
-         .build();  
- 
- HttpGet httpget1 = new HttpGet("http://localhost/1");  
- httpget1.setConfig(requestConfig);  
- CloseableHttpResponse response1 = httpclient.execute(httpget1, context);  
- try {  
-     HttpEntity entity1 = response1.getEntity();  
- } finally {  
-     response1.close();  
- }  
- HttpGet httpget2 = new HttpGet("http://localhost/2");  
- CloseableHttpResponse response2 = httpclient.execute(httpget2, context);  
- try {  
-     HttpEntity entity2 = response2.getEntity();  
- } finally {  
-     response2.close();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 1.4. 异常处理
HttpClient会被抛出两种类型的异常，一种是`java.io.IOException`，当遇到I/O异常时抛出（socket超时，或者socket被重置）;另一种是`HttpException`,表示Http失败，如Http协议使用不正确。通常认为，I/O错误时不致命、可修复的，而Http协议错误是致命了，不能自动修复的错误。
### 1.4.1.HTTP传输安全
Http协议不能满足所有类型的应用场景，我们需要知道这点。Http是个简单的面向协议的请求/响应的协议，当初它被设计用来支持静态或者动态生成的内容检索，之前从来没有人想过让它支持事务性操作。例如，Http服务器成功接收、处理请求后，生成响应消息，并且把状态码发送给客户端，这个过程是Http协议应该保证的。但是，如果客户端由于读取超时、取消请求或者系统崩溃导致接收响应失败，服务器不会回滚这一事务。如果客户端重新发送这个请求，服务器就会重复的解析、执行这个事务。在一些情况下，这会导致应用程序的数据损坏和应用程序的状态不一致。
即使Http当初设计是不支持事务操作，但是它仍旧可以作为传输协议为某些关键程序提供服务。为了保证Http传输层的安全性，系统必须保证应用层上的http方法的幂等性。
### 1.4.2.方法的幂等性
HTTP/1.1规范中是这样定义幂等方法的，Methods can also have the property of “idempotence” in that (aside from error or expiration issues) the side-effects of N > 0 identical requests is the same as for a single request。用其他话来说，应用程序需要正确地处理同一方法多次执行造成的影响。添加一个具有唯一性的id就能避免重复执行同一个逻辑请求，问题解决。
请知晓，这个问题不只是HttpClient才会有，基于浏览器的应用程序也会遇到Http方法不幂等的问题。
HttpClient默认把非实体方法`get`、`head`方法看做幂等方法，把实体方法`post`、`put`方法看做非幂等方法。
### 1.4.3.异常自动修复
默认情况下，HttpClient会尝试自动修复I/O异常。这种自动修复仅限于修复几个公认安全的异常。
- HttpClient不会尝试修复任何逻辑或者http协议错误（即从HttpException衍生出来的异常）。
- HttpClient会自动再次发送幂等的方法（如果首次执行失败)。
- HttpClient会自动再次发送遇到transport异常的方法，前提是Http请求仍旧保持着连接（例如http请求没有全部发送给目标服务器，HttpClient会再次尝试发送）。
### 1.4.4.请求重试HANDLER
如果要自定义异常处理机制，我们需要实现`HttpRequestRetryHandler`接口。
- HttpRequestRetryHandler myRetryHandler = new HttpRequestRetryHandler() {  
- 
- publicboolean retryRequest(  
-             IOException exception,  
- int executionCount,  
-             HttpContext context) {  
- if (executionCount >= 5) {  
- // Do not retry if over max retry count
- returnfalse;  
-         }  
- if (exception instanceof InterruptedIOException) {  
- // Timeout
- returnfalse;  
-         }  
- if (exception instanceof UnknownHostException) {  
- // Unknown host
- returnfalse;  
-         }  
- if (exception instanceof ConnectTimeoutException) {  
- // Connection refused
- returnfalse;  
-         }  
- if (exception instanceof SSLException) {  
- // SSL handshake exception
- returnfalse;  
-         }  
-         HttpClientContext clientContext = HttpClientContext.adapt(context);  
-         HttpRequest request = clientContext.getRequest();  
- boolean idempotent = !(request instanceof HttpEntityEnclosingRequest);  
- if (idempotent) {  
- // Retry if the request is considered idempotent
- returntrue;  
-         }  
- returnfalse;  
-     }  
- };  
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .setRetryHandler(myRetryHandler)  
-         .build();  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 1.5.终止请求
有时候由于目标服务器负载过高或者客户端目前有太多请求积压，http请求不能在指定时间内执行完毕。这时候终止这个请求，释放阻塞I/O的进程，就显得很必要。通过HttpClient执行的Http请求，在任何状态下都能通过调用`HttpUriRequest`的`abort()`方法来终止。这个方法是线程安全的，并且能在任何线程中调用。当Http请求被终止了，本线程（即使现在正在阻塞I/O）也会通过抛出一个`InterruptedIOException`异常，来释放资源。
## 1.6. Http协议拦截器
HTTP协议拦截器是一种实现一个特定的方面的HTTP协议的代码程序。通常情况下，协议拦截器会将一个或多个头消息加入到接受或者发送的消息中。协议拦截器也可以操作消息的内容实体—消息内容的压缩/解压缩就是个很好的例子。通常，这是通过使用“装饰”开发模式，一个包装实体类用于装饰原来的实体来实现。一个拦截器可以合并，形成一个逻辑单元。
协议拦截器可以通过共享信息协作——比如处理状态——通过HTTP执行上下文。协议拦截器可以使用Http上下文存储一个或者多个连续请求的处理状态。
通常，只要拦截器不依赖于一个特定状态的http上下文，那么拦截执行的顺序就无所谓。如果协议拦截器有相互依赖关系，必须以特定的顺序执行，那么它们应该按照特定的顺序加入到协议处理器中。
协议处理器必须是线程安全的。类似于servlets，协议拦截器不应该使用变量实体，除非访问这些变量是同步的（线程安全的）。
下面是个例子，讲述了本地的上下文时如何在连续请求中记录处理状态的：
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .addInterceptorLast(new HttpRequestInterceptor() {  
- 
- publicvoid process(  
- final HttpRequest request,  
- final HttpContext context) throws HttpException, IOException {  
-                 AtomicInteger count = (AtomicInteger) context.getAttribute("count");  
-                 request.addHeader("Count", Integer.toString(count.getAndIncrement()));  
-             }  
- 
-         })  
-         .build();  
- 
- AtomicInteger count = new AtomicInteger(1);  
- HttpClientContext localContext = HttpClientContext.create();  
- localContext.setAttribute("count", count);  
- 
- HttpGet httpget = new HttpGet("http://localhost/");  
- for (int i = 0; i < 10; i++) {  
-     CloseableHttpResponse response = httpclient.execute(httpget, localContext);  
- try {  
-         HttpEntity entity = response.getEntity();  
-     } finally {  
-         response.close();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
上面代码在发送http请求时，会自动添加Count这个header，可以使用wireshark抓包查看。
### 1.7.1. 重定向处理
HttpClient会自动处理所有类型的重定向，除了那些Http规范明确禁止的重定向。See Other (status code 303) redirects on POST and PUT requests are converted to GET requests as required by the HTTP specification. 我们可以使用自定义的重定向策略来放松Http规范对Post方法重定向的限制。
- LaxRedirectStrategy redirectStrategy = new LaxRedirectStrategy();  
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .setRedirectStrategy(redirectStrategy)  
-         .build();  
![](http://static.blog.csdn.net/images/save_snippets.png)
HttpClient在请求执行过程中，经常需要重写请求的消息。 HTTP/1.0和HTTP/1.1都默认使用相对的uri路径。同样，原始的请求可能会被一次或者多次的重定向。最终结对路径的解释可以使用最初的请求和上下文。`URIUtils`类的`resolve`方法可以用于将拦截的绝对路径构建成最终的请求。这个方法包含了最后一个分片标识符或者原始请求。
- CloseableHttpClient httpclient = HttpClients.createDefault();  
- HttpClientContext context = HttpClientContext.create();  
- HttpGet httpget = new HttpGet("http://localhost:8080/");  
- CloseableHttpResponse response = httpclient.execute(httpget, context);  
- try {  
-     HttpHost target = context.getTargetHost();  
-     List<URI> redirectLocations = context.getRedirectLocations();  
-     URI location = URIUtils.resolve(httpget.getURI(), target, redirectLocations);  
-     System.out.println("Final HTTP location: " + location.toASCIIString());  
- // Expected to be an absolute URI
- } finally {  
-     response.close();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
# 第二章 连接管理
## 2.1.持久连接
两个主机建立连接的过程是很复杂的一个过程，涉及到多个数据包的交换，并且也很耗时间。Http连接需要的三次握手开销很大，这一开销对于比较小的http消息来说更大。但是如果我们直接使用已经建立好的http连接，这样花费就比较小，吞吐率更大。
HTTP/1.1默认就支持Http连接复用。兼容HTTP/1.0的终端也可以通过声明来保持连接，实现连接复用。HTTP代理也可以在一定时间内保持连接不释放，方便后续向这个主机发送http请求。这种保持连接不释放的情况实际上是建立的持久连接。HttpClient也支持持久连接。
## 2.2.HTTP连接路由
HttpClient既可以直接、又可以通过多个中转路由（hops）和目标服务器建立连接。HttpClient把路由分为三种plain（明文 ），tunneled（隧道）和layered（分层）。隧道连接中使用的多个中间代理被称作代理链。
客户端直接连接到目标主机或者只通过了一个中间代理，这种就是Plain路由。客户端通过第一个代理建立连接，通过代理链tunnelling，这种情况就是Tunneled路由。不通过中间代理的路由不可能时tunneled路由。客户端在一个已经存在的连接上进行协议分层，这样建立起来的路由就是layered路由。协议只能在隧道—>目标主机，或者直接连接（没有代理），这两种链路上进行分层。
### 2.2.1.路由计算
`RouteInfo`接口包含了数据包发送到目标主机过程中，经过的路由信息。`HttpRoute`类继承了`RouteInfo`接口，是`RouteInfo`的具体实现，这个类是不允许修改的。`HttpTracker`类也实现了`RouteInfo`接口，它是可变的，HttpClient会在内部使用这个类来探测到目标主机的剩余路由。`HttpRouteDirector`是个辅助类，可以帮助计算数据包的下一步路由信息。这个类也是在HttpClient内部使用的。
`HttpRoutePlanner`接口可以用来表示基于http上下文情况下，客户端到服务器的路由计算策略。HttpClient有两个`HttpRoutePlanner`的实现类。`SystemDefaultRoutePlanner`这个类基于`java.net.ProxySelector`，它默认使用jvm的代理配置信息，这个配置信息一般来自系统配置或者浏览器配置。`DefaultProxyRoutePlanner`这个类既不使用java本身的配置，也不使用系统或者浏览器的配置。它通常通过默认代理来计算路由信息。
### 2.2.2. 安全的HTTP连接
为了防止通过Http消息传递的信息不被未授权的第三方获取、截获，Http可以使用SSL/TLS协议来保证http传输安全，这个协议是当前使用最广的。当然也可以使用其他的加密技术。但是通常情况下，Http信息会在加密的SSL/TLS连接上进行传输。
## 2.3. HTTP连接管理器
### 2.3.1. 管理连接和连接管理器
Http连接是复杂，有状态的，线程不安全的对象，所以它必须被妥善管理。一个Http连接在同一时间只能被一个线程访问。HttpClient使用一个叫做Http连接管理器的特殊实体类来管理Http连接，这个实体类要实现`HttpClientConnectionManager`接口。Http连接管理器在新建http连接时，作为工厂类;管理持久http连接的生命周期;同步持久连接（确保线程安全，即一个http连接同一时间只能被一个线程访问）。Http连接管理器和`ManagedHttpClientConnection`的实例类一起发挥作用，`ManagedHttpClientConnection`实体类可以看做http连接的一个代理服务器，管理着I/O操作。如果一个Http连接被释放或者被它的消费者明确表示要关闭，那么底层的连接就会和它的代理进行分离，并且该连接会被交还给连接管理器。这是，即使服务消费者仍然持有代理的引用，它也不能再执行I/O操作，或者更改Http连接的状态。
下面的代码展示了如何从连接管理器中取得一个http连接：
- HttpClientContext context = HttpClientContext.create();  
- HttpClientConnectionManager connMrg = new BasicHttpClientConnectionManager();  
- HttpRoute route = new HttpRoute(new HttpHost("localhost", 80));  
- // 获取新的连接. 这里可能耗费很多时间
- ConnectionRequest connRequest = connMrg.requestConnection(route, null);  
- // 10秒超时
- HttpClientConnection conn = connRequest.get(10, TimeUnit.SECONDS);  
- try {  
- // 如果创建连接失败
- if (!conn.isOpen()) {  
- // establish connection based on its route info
-         connMrg.connect(conn, route, 1000, context);  
- // and mark it as route complete
-         connMrg.routeComplete(conn, route, context);  
-     }  
- // 进行自己的操作.
- } finally {  
-     connMrg.releaseConnection(conn, null, 1, TimeUnit.MINUTES);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
如果要终止连接，可以调用`ConnectionRequest`的`cancel()`方法。这个方法会解锁被`ConnectionRequest`类`get()`方法阻塞的线程。
### 2.3.2.简单连接管理器
`BasicHttpClientConnectionManager`是个简单的连接管理器，它一次只能管理一个连接。尽管这个类是线程安全的，它在同一时间也只能被一个线程使用。`BasicHttpClientConnectionManager`会尽量重用旧的连接来发送后续的请求，并且使用相同的路由。如果后续请求的路由和旧连接中的路由不匹配，`BasicHttpClientConnectionManager`就会关闭当前连接，使用请求中的路由重新建立连接。如果当前的连接正在被占用，会抛出`java.lang.IllegalStateException`异常。
### 2.3.3.连接池管理器
相对`BasicHttpClientConnectionManager`来说，`PoolingHttpClientConnectionManager`是个更复杂的类，它管理着连接池，可以同时为很多线程提供http连接请求。Connections are pooled on a per route basis.当请求一个新的连接时，如果连接池有有可用的持久连接，连接管理器就会使用其中的一个，而不是再创建一个新的连接。
`PoolingHttpClientConnectionManager`维护的连接数在每个路由基础和总数上都有限制。默认，每个路由基础上的连接不超过2个，总连接数不能超过20。在实际应用中，这个限制可能会太小了，尤其是当服务器也使用Http协议时。
下面的例子演示了如果调整连接池的参数：
- PoolingHttpClientConnectionManager cm = new PoolingHttpClientConnectionManager();  
- // Increase max total connection to 200
- cm.setMaxTotal(200);  
- // Increase default max connection per route to 20
- cm.setDefaultMaxPerRoute(20);  
- // Increase max connections for localhost:80 to 50
- HttpHost localhost = new HttpHost("locahost", 80);  
- cm.setMaxPerRoute(new HttpRoute(localhost), 50);  
- 
- CloseableHttpClient httpClient = HttpClients.custom()  
-         .setConnectionManager(cm)  
-         .build();  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 2.3.4.关闭连接管理器
当一个HttpClient的实例不在使用，或者已经脱离它的作用范围，我们需要关掉它的连接管理器，来关闭掉所有的连接，释放掉这些连接占用的系统资源。
- CloseableHttpClient httpClient = <...>  
- httpClient.close();  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 2.4.多线程请求执行
当使用了请求连接池管理器（比如`PoolingClientConnectionManager`）后，HttpClient就可以同时执行多个线程的请求了。
`PoolingClientConnectionManager`会根据它的配置来分配请求连接。如果连接池中的所有连接都被占用了，那么后续的请求就会被阻塞，直到有连接被释放回连接池中。为了防止永远阻塞的情况发生，我们可以把`http.conn-manager.timeout`的值设置成一个整数。如果在超时时间内，没有可用连接，就会抛出`ConnectionPoolTimeoutException`异常。
- PoolingHttpClientConnectionManager cm = new PoolingHttpClientConnectionManager();  
- CloseableHttpClient httpClient = HttpClients.custom()  
-         .setConnectionManager(cm)  
-         .build();  
- 
- // URIs to perform GETs on
- String[] urisToGet = {  
- "http://www.domain1.com/",  
- "http://www.domain2.com/",  
- "http://www.domain3.com/",  
- "http://www.domain4.com/"
- };  
- 
- // create a thread for each URI
- GetThread[] threads = new GetThread[urisToGet.length];  
- for (int i = 0; i < threads.length; i++) {  
-     HttpGet httpget = new HttpGet(urisToGet[i]);  
-     threads[i] = new GetThread(httpClient, httpget);  
- }  
- 
- // start the threads
- for (int j = 0; j < threads.length; j++) {  
-     threads[j].start();  
- }  
- 
- // join the threads
- for (int j = 0; j < threads.length; j++) {  
-     threads[j].join();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
即使HttpClient的实例是线程安全的，可以被多个线程共享访问，但是仍旧推荐每个线程都要有自己专用实例的HttpContext。
下面是GetThread类的定义：
- staticclass GetThread extends Thread {  
- 
- privatefinal CloseableHttpClient httpClient;  
- privatefinal HttpContext context;  
- privatefinal HttpGet httpget;  
- 
- public GetThread(CloseableHttpClient httpClient, HttpGet httpget) {  
- this.httpClient = httpClient;  
- this.context = HttpClientContext.create();  
- this.httpget = httpget;  
-     }  
- 
- @Override
- publicvoid run() {  
- try {  
-             CloseableHttpResponse response = httpClient.execute(  
-                     httpget, context);  
- try {  
-                 HttpEntity entity = response.getEntity();  
-             } finally {  
-                 response.close();  
-             }  
-         } catch (ClientProtocolException ex) {  
- // Handle protocol errors
-         } catch (IOException ex) {  
- // Handle I/O errors
-         }  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 2.5. 连接回收策略
经典阻塞I/O模型的一个主要缺点就是只有当组侧I/O时，socket才能对I/O事件做出反应。当连接被管理器收回后，这个连接仍然存活，但是却无法监控socket的状态，也无法对I/O事件做出反馈。如果连接被服务器端关闭了，客户端监测不到连接的状态变化（也就无法根据连接状态的变化，关闭本地的socket）。
HttpClient为了缓解这一问题造成的影响，会在使用某个连接前，监测这个连接是否已经过时，如果服务器端关闭了连接，那么连接就会失效。这种过时检查并不是100%有效，并且会给每个请求增加10到30毫秒额外开销。唯一一个可行的，且does not involve a one thread per socket model for idle connections的解决办法，是建立一个监控线程，来专门回收由于长时间不活动而被判定为失效的连接。这个监控线程可以周期性的调用`ClientConnectionManager`类的`closeExpiredConnections()`方法来关闭过期的连接，回收连接池中被关闭的连接。它也可以选择性的调用`ClientConnectionManager`类的`closeIdleConnections()`方法来关闭一段时间内不活动的连接。
- publicstaticclass IdleConnectionMonitorThread extends Thread {  
- 
- privatefinal HttpClientConnectionManager connMgr;  
- privatevolatileboolean shutdown;  
- 
- public IdleConnectionMonitorThread(HttpClientConnectionManager connMgr) {  
- super();  
- this.connMgr = connMgr;  
-     }  
- 
- @Override
- publicvoid run() {  
- try {  
- while (!shutdown) {  
- synchronized (this) {  
-                     wait(5000);  
- // Close expired connections
-                     connMgr.closeExpiredConnections();  
- // Optionally, close connections
- // that have been idle longer than 30 sec
-                     connMgr.closeIdleConnections(30, TimeUnit.SECONDS);  
-                 }  
-             }  
-         } catch (InterruptedException ex) {  
- // terminate
-         }  
-     }  
- 
- publicvoid shutdown() {  
-         shutdown = true;  
- synchronized (this) {  
-             notifyAll();  
-         }  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 2.6. 连接存活策略
Http规范没有规定一个持久连接应该保持存活多久。有些Http服务器使用非标准的`Keep-Alive`头消息和客户端进行交互，服务器端会保持数秒时间内保持连接。HttpClient也会利用这个头消息。如果服务器返回的响应中没有包含`Keep-Alive`头消息，HttpClient会认为这个连接可以永远保持。然而，很多服务器都会在不通知客户端的情况下，关闭一定时间内不活动的连接，来节省服务器资源。在某些情况下默认的策略显得太乐观，我们可能需要自定义连接存活策略。
- ConnectionKeepAliveStrategy myStrategy = new ConnectionKeepAliveStrategy() {  
- 
- publiclong getKeepAliveDuration(HttpResponse response, HttpContext context) {  
- // Honor 'keep-alive' header
-         HeaderElementIterator it = new BasicHeaderElementIterator(  
-                 response.headerIterator(HTTP.CONN_KEEP_ALIVE));  
- while (it.hasNext()) {  
-             HeaderElement he = it.nextElement();  
-             String param = he.getName();  
-             String value = he.getValue();  
- if (value != null && param.equalsIgnoreCase("timeout")) {  
- try {  
- return Long.parseLong(value) * 1000;  
-                 } catch(NumberFormatException ignore) {  
-                 }  
-             }  
-         }  
-         HttpHost target = (HttpHost) context.getAttribute(  
-                 HttpClientContext.HTTP_TARGET_HOST);  
- if ("www.naughty-server.com".equalsIgnoreCase(target.getHostName())) {  
- // Keep alive for 5 seconds only
- return5 * 1000;  
-         } else {  
- // otherwise keep alive for 30 seconds
- return30 * 1000;  
-         }  
-     }  
- 
- };  
- CloseableHttpClient client = HttpClients.custom()  
-         .setKeepAliveStrategy(myStrategy)  
-         .build();  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 2.7.socket连接工厂
Http连接使用`java.net.Socket`类来传输数据。这依赖于`ConnectionSocketFactory`接口来创建、初始化和连接socket。这样也就允许HttpClient的用户在代码运行时，指定socket初始化的代码。`PlainConnectionSocketFactory`是默认的创建、初始化明文socket（不加密）的工厂类。
创建socket和使用socket连接到目标主机这两个过程是分离的，所以我们可以在连接发生阻塞时，关闭socket连接。
- HttpClientContext clientContext = HttpClientContext.create();  
- PlainConnectionSocketFactory sf = PlainConnectionSocketFactory.getSocketFactory();  
- Socket socket = sf.createSocket(clientContext);  
- int timeout = 1000; //ms
- HttpHost target = new HttpHost("localhost");  
- InetSocketAddress remoteAddress = new InetSocketAddress(  
-         InetAddress.getByAddress(newbyte[] {127,0,0,1}), 80);  
- sf.connectSocket(timeout, socket, target, remoteAddress, null, clientContext);  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 2.7.1. 安全SOCKET分层
`LayeredConnectionSocketFactory`是`ConnectionSocketFactory`的拓展接口。分层socket工厂类可以在明文socket的基础上创建socket连接。分层socket主要用于在代理服务器之间创建安全socket。HttpClient使用`SSLSocketFactory`这个类实现安全socket，`SSLSocketFactory`实现了SSL/TLS分层。请知晓，HttpClient没有自定义任何加密算法。它完全依赖于Java加密标准（JCE）和安全套接字（JSEE）拓展。
### 2.7.2. 集成连接管理器
自定义的socket工厂类可以和指定的协议（Http、Https）联系起来，用来创建自定义的连接管理器。
- ConnectionSocketFactory plainsf = <...>  
- LayeredConnectionSocketFactory sslsf = <...>  
- Registry<ConnectionSocketFactory> r = RegistryBuilder.<ConnectionSocketFactory>create()  
-         .register("http", plainsf)  
-         .register("https", sslsf)  
-         .build();  
- 
- HttpClientConnectionManager cm = new PoolingHttpClientConnectionManager(r);  
- HttpClients.custom()  
-         .setConnectionManager(cm)  
-         .build();  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 2.7.3. SSL/TLS定制
HttpClient使用`SSLSocketFactory`来创建ssl连接。`SSLSocketFactory`允许用户高度定制。它可以接受`javax.net.ssl.SSLContext`这个类的实例作为参数，来创建自定义的ssl连接。
- HttpClientContext clientContext = HttpClientContext.create();  
- KeyStore myTrustStore = <...>  
- SSLContext sslContext = SSLContexts.custom()  
-         .useTLS()  
-         .loadTrustMaterial(myTrustStore)  
-         .build();  
- SSLConnectionSocketFactory sslsf = new SSLConnectionSocketFactory(sslContext);  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 2.7.4. 域名验证
除了信任验证和在ssl/tls协议层上进行客户端认证，HttpClient一旦建立起连接，就可以选择性验证目标域名和存储在X.509证书中的域名是否一致。这种验证可以为服务器信任提供额外的保障。`X509HostnameVerifier`接口代表主机名验证的策略。在HttpClient中，`X509HostnameVerifier`有三个实现类。重要提示：主机名有效性验证不应该和ssl信任验证混为一谈。
- `StrictHostnameVerifier`: 严格的主机名验证方法和java 1.4,1.5,1.6验证方法相同。和IE6的方式也大致相同。这种验证方式符合RFC 2818通配符。The hostname must match either the first CN, or any of the subject-alts. A wildcard can occur in the CN, and in
 any of the subject-alts.
- `BrowserCompatHostnameVerifier`: 这种验证主机名的方法，和Curl及firefox一致。The hostname must match either the first CN, or any of the subject-alts. A wildcard can occur in the CN, and in any of the subject-alts.`StrictHostnameVerifier`和`BrowserCompatHostnameVerifier`方式唯一不同的地方就是，带有通配符的域名（比如*.yeetrack.com),`BrowserCompatHostnameVerifier`方式在匹配时会匹配所有的的子域名，包括 
 a.b.yeetrack.com .
- `AllowAllHostnameVerifier`: 这种方式不对主机名进行验证，验证功能被关闭，是个空操作，所以它不会抛出`javax.net.ssl.SSLException`异常。HttpClient默认使用`BrowserCompatHostnameVerifier`的验证方式。如果需要，我们可以手动执行验证方式。
- SSLContext sslContext = SSLContexts.createSystemDefault();  
- SSLConnectionSocketFactory sslsf = new SSLConnectionSocketFactory(  
-         sslContext,  
-         SSLConnectionSocketFactory.STRICT_HOSTNAME_VERIFIER);  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 2.8. HttpClient代理服务器配置
尽管，HttpClient支持复杂的路由方案和代理链，它同样也支持直接连接或者只通过一跳的连接。
使用代理服务器最简单的方式就是，指定一个默认的proxy参数。
- HttpHost proxy = new HttpHost("someproxy", 8080);  
- DefaultProxyRoutePlanner routePlanner = new DefaultProxyRoutePlanner(proxy);  
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .setRoutePlanner(routePlanner)  
-         .build();  
![](http://static.blog.csdn.net/images/save_snippets.png)
我们也可以让HttpClient去使用jre的代理服务器。
- SystemDefaultRoutePlanner routePlanner = new SystemDefaultRoutePlanner(  
-         ProxySelector.getDefault());  
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .setRoutePlanner(routePlanner)  
-         .build();  
![](http://static.blog.csdn.net/images/save_snippets.png)
又或者，我们也可以手动配置`RoutePlanner`，这样就可以完全控制Http路由的过程。
- HttpRoutePlanner routePlanner = new HttpRoutePlanner() {  
- 
- public HttpRoute determineRoute(  
-             HttpHost target,  
-             HttpRequest request,  
-             HttpContext context) throws HttpException {  
- returnnew HttpRoute(target, null,  new HttpHost("someproxy", 8080),  
- "https".equalsIgnoreCase(target.getSchemeName()));  
-     }  
- 
- };  
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .setRoutePlanner(routePlanner)  
-         .build();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
# 第三章 Http状态管理
最初，Http被设计成一个无状态的，面向请求/响应的协议，所以它不能在逻辑相关的http请求/响应中保持状态会话。由于越来越多的系统使用http协议，其中包括http从来没有想支持的系统，比如电子商务系统。因此，http支持状态管理就很必要了。
当时的web客户端和服务器软件领先者，网景(netscape)公司，最先在他们的产品中支持http状态管理，并且制定了一些专有规范。后来，网景通过发规范草案，规范了这一机制。这些努力促成 RFC standard track制定了标准的规范。但是，现在多数的应用的状态管理机制都在使用网景公司的规范，而网景的规范和官方规定是不兼容的。因此所有的浏览器开发这都被迫兼容这两种协议，从而导致协议的不统一。
## 3.1. Http cookies
所谓的Http cookie就是一个token或者很短的报文信息，http代理和服务器可以通过cookie来维持会话状态。网景的工程师把它们称作“magic cookie”。
HttpClient使用`Cookie`接口来代表cookie。简单说来，cookie就是一个键值对。一般，cookie也会包含版本号、域名、路径和cookie有效期。
`SetCookie`接口可以代表服务器发给http代理的一个set-cookie响应头，在浏览器中，这个set-cookie响应头可以写入cookie，以便保持会话状态。`SetCookie2`接口对`SetCookie`接口进行了拓展，添加了`Set-Cookie2`方法。
`ClientCookie`接口继承了`Cookie`接口，并进行了功能拓展，比如它可以取出服务器发送过来的原始cookie的值。生成头消息是很重要的，因为只有当cookie被指定为`Set-Cookie`或者`Set-Cookie2`时，它才需要包括一些特定的属性。
### 3.1.1 COOKIES版本
兼容网景的规范，但是不兼容官方规范的cookie，是版本0. 兼容官方规范的版本，将会是版本1。版本1中的Cookie可能和版本0工作机制有差异。
下面的代码，创建了网景版本的Cookie：
- BasicClientCookie netscapeCookie = new BasicClientCookie("name", "value");  
- netscapeCookie.setVersion(0);  
- netscapeCookie.setDomain(".mycompany.com");  
- netscapeCookie.setPath("/");  
![](http://static.blog.csdn.net/images/save_snippets.png)
下面的代码，创建标准版本的Cookie。注意，标准版本的Cookie必须保留服务器发送过来的Cookie所有属性。
- BasicClientCookie stdCookie = new BasicClientCookie("name", "value");  
- stdCookie.setVersion(1);  
- stdCookie.setDomain(".mycompany.com");  
- stdCookie.setPath("/");  
- stdCookie.setSecure(true);  
- // Set attributes EXACTLY as sent by the server 
- stdCookie.setAttribute(ClientCookie.VERSION_ATTR, "1");  
- stdCookie.setAttribute(ClientCookie.DOMAIN_ATTR, ".mycompany.com");  
![](http://static.blog.csdn.net/images/save_snippets.png)
下面的代码，创建了`Set-Cookie2`兼容cookie。
- BasicClientCookie2 stdCookie = new BasicClientCookie2("name", "value");  
- stdCookie.setVersion(1);  
- stdCookie.setDomain(".mycompany.com");  
- stdCookie.setPorts(newint[] {80,8080});  
- stdCookie.setPath("/");  
- stdCookie.setSecure(true);  
- // Set attributes EXACTLY as sent by the server 
- stdCookie.setAttribute(ClientCookie.VERSION_ATTR, "1");  
- stdCookie.setAttribute(ClientCookie.DOMAIN_ATTR, ".mycompany.com");  
- stdCookie.setAttribute(ClientCookie.PORT_ATTR, "80,8080");  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 3.2. Cookie规范
`CookieSpec`接口代表了Cookie管理规范。Cookie管理规范规定了：
- 解析`Set-Cookie`和`Set-Cookie2`(可选）头消息的规则
- 验证Cookie的规则
- 将指定的主机名、端口和路径格式化成Cookie头消息
HttpClient有下面几种`CookieSpec`规范：
- Netscape draft: 这种符合网景公司指定的规范。但是尽量不要使用，除非一定要保证兼容很旧的代码。
- Standard: RFC 2965 HTTP状态管理规范
- Browser compatibility: 这种方式，尽量模仿常用的浏览器，如IE和firefox
- Best match: ‘Meta’ cookie specification that picks up a cookie policy based on the format of cookies sent with the HTTP response.它基本上将上面的几种规范积聚到一个类中。
++ Ignore cookies: 忽略所有Cookie
强烈推荐使用Best Match匹配规则，让HttpClient根据运行时环境自己选择合适的规范。
## 3.3. 选择Cookie策略
我们可以在创建Http client的时候指定Cookie测试，如果需要，也可以在执行http请求的时候，进行覆盖指定。
- RequestConfig globalConfig = RequestConfig.custom()  
-         .setCookieSpec(CookieSpecs.BEST_MATCH)  
-         .build();  
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .setDefaultRequestConfig(globalConfig)  
-         .build();  
- RequestConfig localConfig = RequestConfig.copy(globalConfig)  
-         .setCookieSpec(CookieSpecs.BROWSER_COMPATIBILITY)  
-         .build();  
- HttpGet httpGet = new HttpGet("/");  
- httpGet.setConfig(localConfig);  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 3.4. 自定义Cookie策略
如果我们要自定义Cookie测试，就要自己实现`CookieSpec`接口，然后创建一个`CookieSpecProvider`接口来新建、初始化自定义`CookieSpec`接口，最后把`CookieSpecProvider`注册到HttpClient中。一旦我们注册了自定义策略，就可以像其他标准策略一样使用了。
- CookieSpecProvider easySpecProvider = new CookieSpecProvider() {  
- 
- public CookieSpec create(HttpContext context) {  
- 
- returnnew BrowserCompatSpec() {  
- @Override
- publicvoid validate(Cookie cookie, CookieOrigin origin)  
- throws MalformedCookieException {  
- // Oh, I am easy
-             }  
-         };  
-     }  
- 
- };  
- Registry<CookieSpecProvider> r = RegistryBuilder.<CookieSpecProvider>create()  
-         .register(CookieSpecs.BEST_MATCH,  
- new BestMatchSpecFactory())  
-         .register(CookieSpecs.BROWSER_COMPATIBILITY,  
- new BrowserCompatSpecFactory())  
-         .register("easy", easySpecProvider)  
-         .build();  
- 
- RequestConfig requestConfig = RequestConfig.custom()  
-         .setCookieSpec("easy")  
-         .build();  
- 
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .setDefaultCookieSpecRegistry(r)  
-         .setDefaultRequestConfig(requestConfig)  
-         .build();  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 3.5. Cookie持久化
HttpClient可以使用任何存储方式的cookie store，只要这个cookie store实现了`CookieStore`接口。默认的CookieStore通过`java.util.ArrayList`简单实现了`BasicCookieStore`。存在在`BasicCookieStore`中的Cookie，当载体对象被当做垃圾回收掉后，就会丢失。如果必要，用户可以自己实现更为复杂的方式。
- // Create a local instance of cookie store
- CookieStore cookieStore = new BasicCookieStore();  
- // Populate cookies if needed
- BasicClientCookie cookie = new BasicClientCookie("name", "value");  
- cookie.setVersion(0);  
- cookie.setDomain(".mycompany.com");  
- cookie.setPath("/");  
- cookieStore.addCookie(cookie);  
- // Set the store
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .setDefaultCookieStore(cookieStore)  
-         .build();  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 3.6.HTTP状态管理和执行上下文
在Http请求执行过程中，HttpClient会自动向执行上下文中添加下面的状态管理对象：
- `Lookup`对象 代表实际的cookie规范registry。在当前上下文中的这个值优先于默认值。
- `CookieSpec`对象 代表实际的Cookie规范。
- `CookieOrigin`对象 代表实际的origin server的详细信息。
- `CookieStore`对象 表示Cookie store。这个属性集中的值会取代默认值。
本地的`HttpContext`对象可以用来在Http请求执行前，自定义Http状态管理上下文;或者测试http请求执行完毕后上下文的状态。我们也可以在不同的线程中使用不同的执行上下文。我们在http请求层指定的cookie规范集和cookie store会覆盖在http Client层级的默认值。
- CloseableHttpClient httpclient = <...>  
- 
- Lookup<CookieSpecProvider> cookieSpecReg = <...>  
- CookieStore cookieStore = <...>  
- 
- HttpClientContext context = HttpClientContext.create();  
- context.setCookieSpecRegistry(cookieSpecReg);  
- context.setCookieStore(cookieStore);  
- HttpGet httpget = new HttpGet("http://somehost/");  
- CloseableHttpResponse response1 = httpclient.execute(httpget, context);  
- <...>  
- // Cookie origin details
- CookieOrigin cookieOrigin = context.getCookieOrigin();  
- // Cookie spec used
- CookieSpec cookieSpec = context.getCookieSpec();  
![](http://static.blog.csdn.net/images/save_snippets.png)
# 第四章 HTTP认证
HttpClient既支持HTTP标准规范定义的认证模式，又支持一些广泛使用的非标准认证模式，比如NTLM和SPNEGO。
## 4.1.用户凭证
任何用户认证的过程，都需要一系列的凭证来确定用户的身份。最简单的用户凭证可以是用户名和密码这种形式。`UsernamePasswordCredentials`这个类可以用来表示这种情况，这种凭据包含明文的用户名和密码。
这个类对于HTTP标准规范中定义的认证模式来说已经足够了。
- UsernamePasswordCredentials creds = new UsernamePasswordCredentials("user", "pwd");  
- System.out.println(creds.getUserPrincipal().getName());  
- System.out.println(creds.getPassword());  
![](http://static.blog.csdn.net/images/save_snippets.png)
上述代码会在控制台输出：
- user  
- pwd  
![](http://static.blog.csdn.net/images/save_snippets.png)
`NTCredentials`是微软的windows系统使用的一种凭据，包含username、password，还包括一系列其他的属性，比如用户所在的域名。在Microsoft Windows的网络环境中，同一个用户可以属于不同的域，所以他也就有不同的凭据。
- NTCredentials creds = new NTCredentials("user", "pwd", "workstation", "domain");  
- System.out.println(creds.getUserPrincipal().getName());  
- System.out.println(creds.getPassword());  
![](http://static.blog.csdn.net/images/save_snippets.png)
上述代码输出：
- DOMAIN/user  
- pwd  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 4.2. 认证方案
`AutoScheme`接口表示一个抽象的面向挑战/响应的认证方案。一个认证方案要支持下面的功能：
- 客户端请求服务器受保护的资源，服务器会发送过来一个chanllenge(挑战），认证方案（Authentication scheme）需要解析、处理这个挑战
- 为processed challenge提供一些属性值：认证方案的类型，和此方案需要的一些参数，这种方案适用的范围
- 使用给定的授权信息生成授权字符串;生成http请求，用来响应服务器发送来过的授权challenge
请注意：一个认证方案可能是有状态的，因为它可能涉及到一系列的挑战/响应。
HttpClient实现了下面几种`AutoScheme`:
- Basic: Basic认证方案是在RFC2617号文档中定义的。这种授权方案用明文来传输凭证信息，所以它是不安全的。虽然Basic认证方案本身是不安全的，但是它一旦和TLS/SSL加密技术结合起来使用，就完全足够了。
- Digest: Digest（摘要）认证方案是在RFC2617号文档中定义的。Digest认证方案比Basic方案安全多了，对于那些受不了Basic+TLS/SSL传输开销的系统，digest方案是个不错的选择。
- NTLM: NTLM认证方案是个专有的认证方案，由微软开发，并且针对windows平台做了优化。NTLM被认为比Digest更安全。
- SPNEGO: SPNEGO(Simple and Protected GSSAPI Negotiation Mechanism)是GSSAPI的一个“伪机制”，它用来协商真正的认证机制。SPNEGO最明显的用途是在微软的HTTP协商认证机制拓展上。可协商的子机制包括NTLM、Kerberos。目前，HttpCLient只支持Kerberos机制。（原文：The negotiable sub-mechanisms include
 NTLM and Kerberos supported by Active Directory. At present HttpClient only supports the Kerberos sub-mechanism.）
## 4.3. 凭证 provider
凭证providers旨在维护一套用户的凭证，当需要某种特定的凭证时，providers就应该能产生这种凭证。认证的具体内容包括主机名、端口号、realm name和认证方案名。当使用凭据provider的时候，我们可以很模糊的指定主机名、端口号、realm和认证方案，不用写的很精确。因为，凭据provider会根据我们指定的内容，筛选出一个最匹配的方案。
只要我们自定义的凭据provider实现了`CredentialsProvider`这个接口，就可以在HttpClient中使用。默认的凭据provider叫做`BasicCredentialsProvider`，它使用`java.util.HashMap`对`CredentialsProvider`进行了简单的实现。
- CredentialsProvider credsProvider = new BasicCredentialsProvider();  
- credsProvider.setCredentials(  
- new AuthScope("somehost", AuthScope.ANY_PORT),   
- new UsernamePasswordCredentials("u1", "p1"));  
- credsProvider.setCredentials(  
- new AuthScope("somehost", 8080),   
- new UsernamePasswordCredentials("u2", "p2"));  
- credsProvider.setCredentials(  
- new AuthScope("otherhost", 8080, AuthScope.ANY_REALM, "ntlm"),   
- new UsernamePasswordCredentials("u3", "p3"));  
- 
- System.out.println(credsProvider.getCredentials(  
- new AuthScope("somehost", 80, "realm", "basic")));  
- System.out.println(credsProvider.getCredentials(  
- new AuthScope("somehost", 8080, "realm", "basic")));  
- System.out.println(credsProvider.getCredentials(  
- new AuthScope("otherhost", 8080, "realm", "basic")));  
- System.out.println(credsProvider.getCredentials(  
- new AuthScope("otherhost", 8080, null, "ntlm")));  
![](http://static.blog.csdn.net/images/save_snippets.png)
上面代码输出：
- [principal: u1]  
- [principal: u2]  
- null
- [principal: u3]  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 4.4.HTTP授权和执行上下文
HttpClient依赖`AuthState`类去跟踪认证过程中的状态的详细信息。在Http请求过程中，HttpClient创建两个`AuthState`实例：一个用于目标服务器认证，一个用于代理服务器认证。如果服务器或者代理服务器需要用户的授权信息，`AuthScope`、`AutoScheme`和认证信息就会被填充到两个`AuthScope`实例中。通过对`AutoState`的检测，我们可以确定请求的授权类型，确定是否有匹配的`AuthScheme`，确定凭据provider根据指定的授权类型是否成功生成了用户的授权信息。
在Http请求执行过程中，HttpClient会向执行上下文中添加下面的授权对象：
- `Lookup`对象，表示使用的认证方案。这个对象的值可以在本地上下文中进行设置，来覆盖默认值。
- `CredentialsProvider`对象，表示认证方案provider，这个对象的值可以在本地上下文中进行设置，来覆盖默认值。
- `AuthState`对象，表示目标服务器的认证状态，这个对象的值可以在本地上下文中进行设置，来覆盖默认值。
- `AuthState`对象，表示代理服务器的认证状态，这个对象的值可以在本地上下文中进行设置，来覆盖默认值。
- `AuthCache`对象，表示认证数据的缓存，这个对象的值可以在本地上下文中进行设置，来覆盖默认值。
我们可以在请求执行前，自定义本地`HttpContext`对象来设置需要的http认证上下文;也可以在请求执行后，再检测`HttpContext`的状态，来查看授权是否成功。
- CloseableHttpClient httpclient = <...>  
- 
- CredentialsProvider credsProvider = <...>  
- Lookup<AuthSchemeProvider> authRegistry = <...>  
- AuthCache authCache = <...>  
- 
- HttpClientContext context = HttpClientContext.create();  
- context.setCredentialsProvider(credsProvider);  
- context.setAuthSchemeRegistry(authRegistry);  
- context.setAuthCache(authCache);  
- HttpGet httpget = new HttpGet("http://somehost/");  
- CloseableHttpResponse response1 = httpclient.execute(httpget, context);  
- <...>  
- 
- AuthState proxyAuthState = context.getProxyAuthState();  
- System.out.println("Proxy auth state: " + proxyAuthState.getState());  
- System.out.println("Proxy auth scheme: " + proxyAuthState.getAuthScheme());  
- System.out.println("Proxy auth credentials: " + proxyAuthState.getCredentials());  
- AuthState targetAuthState = context.getTargetAuthState();  
- System.out.println("Target auth state: " + targetAuthState.getState());  
- System.out.println("Target auth scheme: " + targetAuthState.getAuthScheme());  
- System.out.println("Target auth credentials: " + targetAuthState.getCredentials());  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 4.5. 缓存认证数据
从版本4.1开始，HttpClient就会自动缓存验证通过的认证信息。但是为了使用这个缓存的认证信息，我们必须在同一个上下文中执行逻辑相关的请求。一旦超出该上下文的作用范围，缓存的认证信息就会失效。
## 4.6. 抢先认证
HttpClient默认不支持抢先认证，因为一旦抢先认证被误用或者错用，会导致一系列的安全问题，比如会把用户的认证信息以明文的方式发送给未授权的第三方服务器。因此，需要用户自己根据自己应用的具体环境来评估抢先认证带来的好处和带来的风险。
即使如此，HttpClient还是允许我们通过配置来启用抢先认证，方法是提前填充认证信息缓存到上下文中，这样，以这个上下文执行的方法，就会使用抢先认证。
- CloseableHttpClient httpclient = <...>  
- 
- HttpHost targetHost = new HttpHost("localhost", 80, "http");  
- CredentialsProvider credsProvider = new BasicCredentialsProvider();  
- credsProvider.setCredentials(  
- new AuthScope(targetHost.getHostName(), targetHost.getPort()),  
- new UsernamePasswordCredentials("username", "password"));  
- 
- // Create AuthCache instance
- AuthCache authCache = new BasicAuthCache();  
- // Generate BASIC scheme object and add it to the local auth cache
- BasicScheme basicAuth = new BasicScheme();  
- authCache.put(targetHost, basicAuth);  
- 
- // Add AuthCache to the execution context
- HttpClientContext context = HttpClientContext.create();  
- context.setCredentialsProvider(credsProvider);  
- context.setAuthCache(authCache);  
- 
- HttpGet httpget = new HttpGet("/");  
- for (int i = 0; i < 3; i++) {  
-     CloseableHttpResponse response = httpclient.execute(  
-             targetHost, httpget, context);  
- try {  
-         HttpEntity entity = response.getEntity();  
- 
-     } finally {  
-         response.close();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 4.7. NTLM认证
从版本4.1开始，HttpClient就全面支持NTLMv1、NTLMv2和NTLM2认证。当人我们可以仍旧使用外部的NTLM引擎（比如Samba开发的JCIFS库）作为与Windows互操作性程序的一部分。
### 4.7.1. NTLM连接持久性
相比`Basic`和`Digest`认证，NTLM认证要明显需要更多的计算开销，性能影响也比较大。这也可能是微软把NTLM协议设计成有状态连接的主要原因之一。也就是说，NTLM连接一旦建立，用户的身份就会在其整个生命周期和它相关联。NTLM连接的状态性使得连接持久性更加复杂，The stateful nature of NTLM connections makes connection persistence more complex, as for the obvious
 reason persistent NTLM connections may not be re-used by users with a different user identity. HttpClient中标准的连接管理器就可以管理有状态的连接。但是，同一会话中逻辑相关的请求，必须使用相同的执行上下文，这样才能使用用户的身份信息。否则，HttpClient就会结束旧的连接，为了获取被NTLM协议保护的资源，而为每个HTTP请求，创建一个新的Http连接。更新关于Http状态连接的信息，点击[此处](http://hc.apache.org/httpcomponents-client-ga/tutorial/html/advanced.html#stateful_conn)。
由于NTLM连接是有状态的，一般推荐使用比较轻量级的方法来处罚NTLM认证（如GET、Head方法），然后使用这个已经建立的连接在执行相对重量级的方法，尤其是需要附件请求实体的请求（如POST、PUT请求）。
- CloseableHttpClient httpclient = <...>  
- 
- CredentialsProvider credsProvider = new BasicCredentialsProvider();  
- credsProvider.setCredentials(AuthScope.ANY,  
- new NTCredentials("user", "pwd", "myworkstation", "microsoft.com"));  
- 
- HttpHost target = new HttpHost("www.microsoft.com", 80, "http");  
- 
- // Make sure the same context is used to execute logically related requests
- HttpClientContext context = HttpClientContext.create();  
- context.setCredentialsProvider(credsProvider);  
- 
- // Execute a cheap method first. This will trigger NTLM authentication
- HttpGet httpget = new HttpGet("/ntlm-protected/info");  
- CloseableHttpResponse response1 = httpclient.execute(target, httpget, context);  
- try {  
-     HttpEntity entity1 = response1.getEntity();  
- } finally {  
-     response1.close();  
- }  
- 
- // Execute an expensive method next reusing the same context (and connection)
- HttpPost httppost = new HttpPost("/ntlm-protected/form");  
- httppost.setEntity(new StringEntity("lots and lots of data"));  
- CloseableHttpResponse response2 = httpclient.execute(target, httppost, context);  
- try {  
-     HttpEntity entity2 = response2.getEntity();  
- } finally {  
-     response2.close();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 4.8. SPNEGO/Kerberos认证
SPNEGO(Simple and Protected GSSAPI Megotiation Mechanism），当双方均不知道对方能使用/提供什么协议的情况下，可以使用SP认证协议。这种协议在Kerberos认证方案中经常使用。It can wrap other mechanisms, however the current version in HttpClient is designed solely with Kerberos in mind.
### 4.8.1. 在HTTPCIENT中使用SPNEGO
SPNEGO认证方案兼容Sun java 1.5及以上版本。但是强烈推荐jdk1.6以上。Sun的JRE提供的类就已经几乎完全可以处理Kerberos和SPNEGO token。这就意味着，需要设置很多的GSS类。`SpnegoScheme`是个很简单的类，可以用它来handle marshalling the tokens and 读写正确的头消息。
最好的开始方法就是从示例程序中找到`KerberosHttpClient.java`这个文件，尝试让它运行起来。运行过程有可能会出现很多问题，但是如果人品比较高可能会顺利一点。这个文件会提供一些输出，来帮我们调试。
在Windows系统中，应该默认使用用户的登陆凭据;当然我们也可以使用`kinit`来覆盖这个凭据，比如`$JAVA_HOME\bin\kinit testuser@AD.EXAMPLE.NET`，这在我们测试和调试的时候就显得很有用了。如果想用回Windows默认的登陆凭据，删除kinit创建的缓存文件即可。
确保在krb5.conf文件中列出`domain_realms`。这能解决很多不必要的问题。
### 4.8.2. 使用GSS/JAVA KERBEROS
下面的这份文档是针对Windows系统的，但是很多信息同样适合Unix。
`org.ietf.jgss`这个类有很多的配置参数，这些参数大部分都在`krb5.conf/krb5.ini`文件中配置。更多的信息，参考[此处](http://web.mit.edu/kerberos/krb5-1.4/krb5-1.4.1/doc/krb5-admin/krb5.conf.html.)。
#### login.conf文件
下面是一个基本的login.conf文件，使用于Windows平台的IIS和JBoss Negotiation模块。
系统配置文件`java.security.auth.login.config`可以指定`login.conf`文件的路径。
`login.conf`的内容可能会是下面的样子：
- com.sun.security.jgss.login {  
-   com.sun.security.auth.module.Krb5LoginModule required client=TRUE useTicketCache=true;  
- };  
- 
- com.sun.security.jgss.initiate {  
-   com.sun.security.auth.module.Krb5LoginModule required client=TRUE useTicketCache=true;  
- };  
- 
- com.sun.security.jgss.accept {  
-   com.sun.security.auth.module.Krb5LoginModule required client=TRUE useTicketCache=true;  
- };  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 4.8.4. KRB5.CONF / KRB5.INI 文件
如果没有手动指定，系统会使用默认配置。如果要手动指定，可以在`java.security.krb5.conf`中设置系统变量，指定`krb5.conf`的路径。`krb5.conf`的内容可能是下面的样子：
- [libdefaults]  
-     default_realm = AD.EXAMPLE.NET  
-     udp_preference_limit = 1
- [realms]  
-     AD.EXAMPLE.NET = {  
-         kdc = KDC.AD.EXAMPLE.NET  
-     }  
- [domain_realms]  
- .ad.example.net=AD.EXAMPLE.NET  
- ad.example.net=AD.EXAMPLE.NET  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 4.8.5. WINDOWS详细的配置
为了允许Windows使用当前用户的tickets，`javax.security.auth.useSubjectCredsOnly`这个系统变量应该设置成`false`，并且需要在Windows注册表中添加`allowtgtsessionkey`这个项，而且要allow session keys to be sent in the Kerberos Ticket-Granting Ticket.
Windows Server 2003和Windows 2000 SP4,配置如下：
- HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Lsa\Kerberos\Parameters  
- Value Name: allowtgtsessionkey  
- Value Type: REG_DWORD  
- Value: 0x01
![](http://static.blog.csdn.net/images/save_snippets.png)
Windows XP SP2 配置如下：
- HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Lsa\Kerberos\  
- Value Name: allowtgtsessionkey  
- Value Type: REG_DWORD  
- Value: 0x01
![](http://static.blog.csdn.net/images/save_snippets.png)
# 第五章 快速API
## 5.1. Easy to use facade API
HttpClient从4.2开始支持快速api。快速api仅仅实现了HttpClient的基本功能，它只要用于一些不需要灵活性的简单场景。例如，快速api不需要用户处理连接管理和资源释放。
下面是几个使用快速api的例子：
- // Execute a GET with timeout settings and return response content as String.
- Request.Get("http://somehost/")  
-         .connectTimeout(1000)  
-         .socketTimeout(1000)  
-         .execute().returnContent().asString();  
![](http://static.blog.csdn.net/images/save_snippets.png)
- // Execute a POST with the 'expect-continue' handshake, using HTTP/1.1,
- // containing a request body as String and return response content as byte array.
- Request.Post("http://somehost/do-stuff")  
-         .useExpectContinue()  
-         .version(HttpVersion.HTTP_1_1)  
-         .bodyString("Important stuff", ContentType.DEFAULT_TEXT)  
-         .execute().returnContent().asBytes();  
![](http://static.blog.csdn.net/images/save_snippets.png)
- // Execute a POST with a custom header through the proxy containing a request body
- // as an HTML form and save the result to the file
- Request.Post("http://somehost/some-form")  
-         .addHeader("X-Custom-header", "stuff")  
-         .viaProxy(new HttpHost("myproxy", 8080))  
-         .bodyForm(Form.form().add("username", "vip").add("password", "secret").build())  
-         .execute().saveContent(new File("result.dump"));  
![](http://static.blog.csdn.net/images/save_snippets.png)
如果需要在指定的安全上下文中执行某些请求，我们也可以直接使用Exector，这时候用户的认证信息就会被缓存起来，以便后续的请求使用。
- Executor executor = Executor.newInstance()  
-         .auth(new HttpHost("somehost"), "username", "password")  
-         .auth(new HttpHost("myproxy", 8080), "username", "password")  
-         .authPreemptive(new HttpHost("myproxy", 8080));  
- 
- executor.execute(Request.Get("http://somehost/"))  
-         .returnContent().asString();  
- 
- executor.execute(Request.Post("http://somehost/do-stuff")  
-         .useExpectContinue()  
-         .bodyString("Important stuff", ContentType.DEFAULT_TEXT))  
-         .returnContent().asString();  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 5.1.1. 响应处理
一般情况下，HttpClient的快速api不用用户处理连接管理和资源释放。但是，这样的话，就必须在内存中缓存这些响应消息。为了避免这一情况，建议使用使用ResponseHandler来处理Http响应。
- Document result = Request.Get("http://somehost/content")  
-         .execute().handleResponse(new ResponseHandler<Document>() {  
- 
- public Document handleResponse(final HttpResponse response) throws IOException {  
-         StatusLine statusLine = response.getStatusLine();  
-         HttpEntity entity = response.getEntity();  
- if (statusLine.getStatusCode() >= 300) {  
- thrownew HttpResponseException(  
-                     statusLine.getStatusCode(),  
-                     statusLine.getReasonPhrase());  
-         }  
- if (entity == null) {  
- thrownew ClientProtocolException("Response contains no content");  
-         }  
-         DocumentBuilderFactory dbfac = DocumentBuilderFactory.newInstance();  
- try {  
-             DocumentBuilder docBuilder = dbfac.newDocumentBuilder();  
-             ContentType contentType = ContentType.getOrDefault(entity);  
- if (!contentType.equals(ContentType.APPLICATION_XML)) {  
- thrownew ClientProtocolException("Unexpected content type:" +  
-                     contentType);  
-             }  
-             String charset = contentType.getCharset();  
- if (charset == null) {  
-                 charset = HTTP.DEFAULT_CONTENT_CHARSET;  
-             }  
- return docBuilder.parse(entity.getContent(), charset);  
-         } catch (ParserConfigurationException ex) {  
- thrownew IllegalStateException(ex);  
-         } catch (SAXException ex) {  
- thrownew ClientProtocolException("Malformed XML document", ex);  
-         }  
-     }  
- 
-     });  
![](http://static.blog.csdn.net/images/save_snippets.png)
# 第六章 HTTP缓存
## 6.1. 基本概念
HttpClient的缓存机制提供一个与HTTP/1.1标准兼容的缓存层 – 相当于Java的浏览器缓存。HttpClient缓存机制的实现遵循责任链（Chain of Responsibility）设计原则，默认的HttpClient是没有缓存的，有缓存机制的HttpClient可以用来临时替代默认的HttpClient，如果开启了缓存，我们的请求结果就会从缓存中获取，而不是从目标服务器中获取。如果在Get请求头中设置了`If-Modified-Since`或者`If-None-Match`参数，那么HttpClient会自动向服务器校验缓存是否过期。
HTTP/1.1版本的缓存是语义透明的，意思是无论怎样，缓存都不应该修改客户端与服务器之间传输的请求/响应数据包。因此，在existing compliant client-server relationship中使用带有缓存的HttpClient也应该是安全的。虽然缓存是客户端的一部分，但是从Http协议的角度来看，缓存机制是为了兼容透明的缓存代理。
最后，HttpClient缓存也支持RFC 5861规定的Cache-Control拓展（`stale-if-error'和`stale-while-revalidate`)。
当开启缓存的HttpClient执行一个Http请求时，会经过下面的步骤：
- 检查http请求是否符合HTTP 1.1的基本要求，如果不符合就尝试修正错误。
- 刷新该请求无效的缓存项。（Flush any cache entries which would be invalidated by this request.）
- 检测该请求是否可以从缓存中获取。如果不能，直接将请求发送给目标服务器，获取响应并加入缓存。
- 如果该请求可以从缓存中获取，HttpClient就尝试读取缓存中的数据。如果读取失败，就会发送请求到目标服务器，如果可能的话，就把响应缓存起来。
- 如果HttpClient缓存的响应可以直接返回给请求，HttpClient就会构建一个包含`ByteArrayEntity`的`BasicHttpResponse`对象，并将它返回给http请求。否则，HttpClient会向服务器重新校验缓存。
- 如果HttpClient缓存的响应，向服务器校验失败，就会向服务器重新请求数据，并将其缓存起来（如果合适的话）。
当开启缓存的HttpClient收到服务器的响应时，会经过下面的步骤：
- 检查收到的响应是否符合协议兼容性
- 确定收到的响应是否可以缓存
- 如果响应是可以缓存的，HttpClient就会尽量从响应消息中读取数据（大小可以在配置文件进行配置），并且缓存起来。
- 如果响应数据太大，缓存或者重构消耗的响应空间不够，就会直接返回响应，不进行缓存。
需要注意的是，带有缓存的HttpClient不是HttpClient的另一种实现，而是通过向http请求执行管道中插入附加处理组件来实现的。
## 6.2. RFC-2616 Compliance
HttpClient的缓存机制和RFC-2626文档规定是无条件兼容的。也就是说，只要指定了`MUST`，`MUST NOT`，`SHOULD`或者`SHOULD NOT`这些Http缓存规范，HttpClient的缓存层就会按照指定的方式进行缓存。即当我们使用HttpClient的缓存机制时，HttpClient的缓存模块不会产生异常动作。
## 6.3. 使用范例
下面的例子讲述了如何创建一个基本的开启缓存的HttpClient。并且配置了最大缓存1000个Object对象，每个对象最大占用8192字节数据。代码中出现的数据，只是为了做演示，而过不是推荐使用的配置。
- CacheConfig cacheConfig = CacheConfig.custom()  
-         .setMaxCacheEntries(1000)  
-         .setMaxObjectSize(8192)  
-         .build();  
- RequestConfig requestConfig = RequestConfig.custom()  
-         .setConnectTimeout(30000)  
-         .setSocketTimeout(30000)  
-         .build();  
- CloseableHttpClient cachingClient = CachingHttpClients.custom()  
-         .setCacheConfig(cacheConfig)  
-         .setDefaultRequestConfig(requestConfig)  
-         .build();  
- 
- HttpCacheContext context = HttpCacheContext.create();  
- HttpGet httpget = new HttpGet("http://www.mydomain.com/content/");  
- CloseableHttpResponse response = cachingClient.execute(httpget, context);  
- try {  
-     CacheResponseStatus responseStatus = context.getCacheResponseStatus();  
- switch (responseStatus) {  
- case CACHE_HIT:  
-             System.out.println("A response was generated from the cache with " +  
- "no requests sent upstream");  
- break;  
- case CACHE_MODULE_RESPONSE:  
-             System.out.println("The response was generated directly by the " +  
- "caching module");  
- break;  
- case CACHE_MISS:  
-             System.out.println("The response came from an upstream server");  
- break;  
- case VALIDATED:  
-             System.out.println("The response was generated from the cache " +  
- "after validating the entry with the origin server");  
- break;  
-     }  
- } finally {  
-     response.close();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 6.4. 配置
有缓存的HttpClient继承了非缓存HttpClient的所有配置项和参数（包括超时时间，连接池大小等配置项）。如果需要对缓存进行具体配置，可以初始化一个`CacheConfig`对象来自定义下面的参数：
- `Cache size`（缓存大小）. 如果后台存储支持，我们可以指定缓存的最大条数，和每个缓存中存储的response的最大size。
- `Public/private cacheing`(公用/私有 缓存）. 默认情况下，缓存模块会把缓存当做公用的缓存，所以缓存机制不会缓存带有授权头消息或者指定`Cache-Control:private`的响应。但是如果缓存只会被一个逻辑上的用户使用（和浏览器饿缓存类似），我们可能希望关闭缓存共享机制。
- `Heuristic caching`（启发式缓存）。即使服务器没有明确设置缓存控制headers信息，每个RFC2616缓存也会存储一定数目的缓存。这个特征在HttpClient中默认是关闭的，如果服务器不设置控制缓存的header信息，但是我们仍然希望对响应进行缓存，就需要在HttpClient中打开这个功能。激活启发式缓存，然后使用默认的刷新时间或者自定义刷新时间。更多启发式缓存的信息，可以参考Http/1.1
 RFC文档的13.2.2小节，13.2.4小节。
- `Background validation`(后台校验）。HttpClient的缓存机制支持RFC5861的`stale-while-revalidate`指令，它允许一定数目的缓存在后台校验是否过期。我们可能需要调整可以在后台工作的最大和最小的线程数，以及设置线程在回收前最大的空闲时间。当没有足够线程来校验缓存是否过期时，我们可以指定排队队列的大小。
## 6.5.存储介质
默认，HttpClient缓存机制将缓存条目和缓存的response放在本地程序的jvm内存中。这样虽然提供高性能，但是当我们的程序内存有大小限制的时候，这就会变得不太合理。因为缓存的生命中期很短，如果程序重启，缓存就会失效。当前版本的HttpClient使用EhCache和memchached来存储缓存，这样就支持将缓存放到本地磁盘或者其他存储介质上。如果内存、本地磁盘、外地磁盘，都不适合你的应用程序，HttpClient也支持自定义存储介质，只需要实现`HttpCacheStorage`接口，然后在创建HttpClient时，使用这个接口的配置。这种情况，缓存会存储在自定义的介质中，但是you
 will get to reuse all of the logic surrounding HTTP/1.1 compliance and cache handling. 一般来说，可以创建出支持任何键值对指定存储（类似Java Map接口）的`HttpCacheStorage`，用于进行原子更新。
最后，通过一些额外的工作，还可以建立起多层次的缓存结构;磁盘中的缓存，远程memcached中的缓存，虚拟内存中的缓存，L1/L2处理器中的缓存等。
# 第七章  高级主题
## 7.1 自定义客户端连接
在特定条件下，也许需要来定制HTTP报文通过线路传递，越过了可能使用的HTTP参数来处理非标准不兼容行为的方式。比如，对于Web爬虫，它可能需要强制HttpClient接受格式错误的响应头部信息，来抢救报文的内容。
通常插入一个自定义的报文解析器的过程或定制连接实现需要几个步骤：
提供一个自定义LineParser/LineFormatter接口实现。如果需要，实现报文解析/格式化逻辑。
- <span style="font-family:SimSun;">class MyLineParser extends BasicLineParser {  
- 
- @Override
- public Header parseHeader(  
-             CharArrayBuffer buffer) throws ParseException {  
- try {  
- returnsuper.parseHeader(buffer);  
-         } catch (ParseException ex) {  
- // Suppress ParseException exception
- returnnew BasicHeader(buffer.toString(), null);  
-         }  
-     }  
- 
- }</span>  
![](http://static.blog.csdn.net/images/save_snippets.png)
提过一个自定义的 HttpConnectionFactory 实现。替换需要自定义的默认请求/响应解析器，请求/响应格式化器。如果需要，实现不同的报文写入/读取代码。
- <span style="font-family:SimSun;">HttpConnectionFactory<HttpRoute, ManagedHttpClientConnection> connFactory =  
- new ManagedHttpClientConnectionFactory(  
- new DefaultHttpRequestWriterFactory(),  
- new DefaultHttpResponseParserFactory(  
- new MyLineParser(), new DefaultHttpResponseFactory()));</span>  
![](http://static.blog.csdn.net/images/save_snippets.png)
为了创建新类的连接，提供一个自定义的ClientConnectionOperator接口实现。如果需要，实现不同的套接字初始化代码。
- <span style="font-family:SimSun;">PoolingHttpClientConnectionManager cm = new PoolingHttpClientConnectionManager(  
-     connFactory);  
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .setConnectionManager(cm)  
-         .build();</span>  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 7.2 有状态的HTTP连接
HTTP规范假设session状态信息通常是以HTTP cookie格式嵌入在HTTP报文中的，因此HTTP连接通常是无状态的，这个假设在现实生活中通常是不对的。也有一些情况，当HTTP连接使用特定的用户标识或特定的安全上下文来创建时，因此不能和其它用户共享，只能由该用户重用。这样的有状态的HTTP连接的示例就是NTLM认证连接和使用客户端证书认证的SSL连接。
### 7.2.1 用户令牌处理器
HttpClient依赖UserTokenHandler接口来决定给定的执行上下文是否是用户指定的。如果这个上下文是用户指定的或者如果上下文没有包含任何资源或关于当前用户指定详情而是null，令牌对象由这个处理器返回，期望唯一地标识当前的用户。用户令牌将被用来保证用户指定资源不会和其它用户来共享或重用。
如果它可以从给定的执行上下文中来获得，UserTokenHandler接口的默认实现是使用主类的一个实例来代表HTTP连接的状态对象。UserTokenHandler将会使用基于如NTLM或开启的客户端认证SSL会话认证模式的用户的主连接。如果二者都不可用，那么就不会返回令牌。
如果默认的不能满足它们的需要，用户可以提供一个自定义的实现：
- <span style="font-family:SimSun;">CloseableHttpClient httpclient = HttpClients.createDefault();  
- HttpClientContext context = HttpClientContext.create();  
- HttpGet httpget = new HttpGet("http://localhost:8080/");  
- CloseableHttpResponse response = httpclient.execute(httpget, context);  
- try {  
-     Principal principal = context.getUserToken(Principal.class);  
-     System.out.println(principal);  
- } finally {  
-     response.close();  
- }</span>  
![](http://static.blog.csdn.net/images/save_snippets.png)
如果默认的不能满足需求，用户可以提供一个特定的实现：
- <span style="font-family:SimSun;">UserTokenHandler userTokenHandler = new UserTokenHandler() {  
- 
- public Object getUserToken(HttpContext context) {  
- return context.getAttribute("my-token");  
-     }  
- 
- };  
- CloseableHttpClient httpclient = HttpClients.custom()  
-         .setUserTokenHandler(userTokenHandler)  
-         .build();</span>  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 7.2.2 持久化有状态的连接
请注意带有状态对象的持久化连接仅当请求被执行时，相同状态对象被绑定到执行上下文时可以被重用。所以，保证相同上下文重用于执行随后的相同用户，或用户令牌绑定到之前请求执行上下文的HTTP请求是很重要的。
- <span style="font-family:SimSun;">CloseableHttpClient httpclient = HttpClients.createDefault();  
- HttpClientContext context1 = HttpClientContext.create();  
- HttpGet httpget1 = new HttpGet("http://localhost:8080/");  
- CloseableHttpResponse response1 = httpclient.execute(httpget1, context1);  
- try {  
-     HttpEntity entity1 = response1.getEntity();  
- } finally {  
-     response1.close();  
- }  
- Principal principal = context1.getUserToken(Principal.class);  
- 
- HttpClientContext context2 = HttpClientContext.create();  
- context2.setUserToken(principal);  
- HttpGet httpget2 = new HttpGet("http://localhost:8080/");  
- CloseableHttpResponse response2 = httpclient.execute(httpget2, context2);  
- try {  
-     HttpEntity entity2 = response2.getEntity();  
- } finally {  
-     response2.close();  
- }</span>  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 7.3. 使用FutureRequestExecutionService
通过使用FutureRequestExecutionService，你可以调度HTTP调用以及把response当作一个Future。这是非常有用的，比如当多次调用一个Web服务。使用FutureRequestExecutionService的优势在于您可以使用多个线程来调度请求同时，对任务设置超时或取消，当response不再需要的时候。
FutureRequestExecutionService用HttpRequestFutureTask（继承FutureTask）包装request。这个类允许你取消Task以及保持跟踪各项指标，如request duration。
### 7.3.1. 构造FutureRequestExecutionService
futureRequestExecutionService的构造方法包括两个参数：httpClient实例和ExecutorService实例。当配置两个参数的时候，您要使用的线程数等于最大连接数是很重要的。当线程比连接多的时候，连接可能会开始超时，因为没有可用的连接。当连接多于线程时，futureRequestExecutionService不会使用所有的连接。
- <span style="font-family:SimSun;">HttpClient httpClient = HttpClientBuilder.create().setMaxConnPerRoute(5).build();  
- ExecutorService executorService = Executors.newFixedThreadPool(5);  
- FutureRequestExecutionService futureRequestExecutionService =  
- new FutureRequestExecutionService(httpClient, executorService);</span>  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 7.3.2. 安排requests
要安排一个请求，只需提供一个HttpUriRequest，HttpContext和ResponseHandler。因为request是由executor service处理的，而ResponseHandler的是强制性的。
- <span style="font-family:SimSun;">privatefinalclass OkidokiHandler implements ResponseHandler<Boolean> {  
- public Boolean handleResponse(  
- final HttpResponse response) throws ClientProtocolException, IOException {  
- return response.getStatusLine().getStatusCode() == 200;  
-     }  
- }  
- 
- HttpRequestFutureTask<Boolean> task = futureRequestExecutionService.execute(  
- new HttpGet("http://www.google.com"), HttpClientContext.create(),  
- new OkidokiHandler());  
- // blocks until the request complete and then returns true if you can connect to Google
- boolean ok=task.get();</span>  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 7.3.3. 取消tasks
预定的任务可能会被取消。如果任务尚未执行，但仅仅是排队等待执行，它根本就不会执行。如果任务在执行中且mayInterruptIfRunning参数被设置为true，请求中的abort（）函数将被调用;否则response会简单地忽略，但该请求将被允许正常完成。任何后续调用task.get()会产生一个IllegalStateException。应当注意到，取消任务仅可以释放客户端的资源。该请求可能实际上是在服务器端正常处理。
- <span style="font-family:SimSun;">task.cancel(true)  
- task.get() // throws an Exception</span>
![](http://static.blog.csdn.net/images/save_snippets.png)
### 7.3.4. 回调
不用手动调用task.get()，您也可以在请求完成时使用FutureCallback实例获取回调。这里采用的是和HttpAsyncClient相同的接口
- <span style="font-family:SimSun;">privatefinalclass MyCallback implements FutureCallback<Boolean> {  
- 
- publicvoid failed(final Exception ex) {  
- // do something
-     }  
- 
- publicvoid completed(final Boolean result) {  
- // do something
-     }  
- 
- publicvoid cancelled() {  
- // do something
-     }  
- }  
- 
- HttpRequestFutureTask<Boolean> task = futureRequestExecutionService.execute(  
- new HttpGet("http://www.google.com"), HttpClientContext.create(),  
- new OkidokiHandler(), new MyCallback());</span>  
![](http://static.blog.csdn.net/images/save_snippets.png)
### 7.3.5. 指标
FutureRequestExecutionService通常用于大量Web服务调用的应用程序之中。为了便于例如监视或配置调整，FutureRequestExecutionService跟踪了几个指标。
HttpRequestFutureTask会提供一些方法来获得任务时间：从被安排，开始，直到结束。此外，请求和任务持续时间也是可用的。这些指标都聚集在FutureRequestExecutionService中的FutureRequestExecutionMetrics实例，可以通过FutureRequestExecutionService.metrics()获取。
- <span style="font-family:SimSun;">task.scheduledTime() // returns the timestamp the task was scheduled
- task.startedTime() // returns the timestamp when the task was started
- task.endedTime() // returns the timestamp when the task was done executing
- task.requestDuration // returns the duration of the http request
- task.taskDuration // returns the duration of the task from the moment it was scheduled
- 
- FutureRequestExecutionMetrics metrics = futureRequestExecutionService.metrics()  
- metrics.getActiveConnectionCount() // currently active connections
- metrics.getScheduledConnectionCount(); // currently scheduled connections
- metrics.getSuccessfulConnectionCount(); // total number of successful requests
- metrics.getSuccessfulConnectionAverageDuration(); // average request duration
- metrics.getFailedConnectionCount(); // total number of failed tasks
- metrics.getFailedConnectionAverageDuration(); // average duration of failed tasks
- metrics.getTaskCount(); // total number of tasks scheduled
- metrics.getRequestCount(); // total number of requests
- metrics.getRequestAverageDuration(); // average request duration
- metrics.getTaskAverageDuration(); // average task duration</span>
![](http://static.blog.csdn.net/images/save_snippets.png)
参考:[http://hc.apache.org/httpcomponents-client-ga/tutorial/html/index.html](http://hc.apache.org/httpcomponents-client-ga/tutorial/html/index.html)




