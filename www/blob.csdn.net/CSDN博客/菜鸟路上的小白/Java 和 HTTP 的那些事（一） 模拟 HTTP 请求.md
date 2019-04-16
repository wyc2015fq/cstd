# Java 和 HTTP 的那些事（一） 模拟 HTTP 请求 - 菜鸟路上的小白 - CSDN博客





2016年09月14日 13:36:54[somnus_小凯](https://me.csdn.net/u012486840)阅读数：1872









最新在学习使用 Java 来写网络爬虫，模拟浏览器发送 HTTP 请求，并抓取返回页面中的信息。由于对 Java 刚接触，以前用 .Net 写的一些网络请求相关的工具类都派不上用场，于是对如何使用 Java 模拟 HTTP 请求潜心研究了一番，在此写下这个《Java 和 HTTP 的那些事》系列的博客，并记录一些我中途遇到了明坑和暗坑，供后来人参考。此为第一篇。

## 一、使用 HttpURLConnection 发送 HTTP 请求


Java 自带的 `java.net` 这个包中包含了很多与网络请求相关的类，但是对于我们来说，最关心的应该是 `HttpURLConnection` 这个类了。

#### 1.1 创建 HTTP 连接对象


要得到一个 `HttpURLConnection` HTTP
 连接对象，首先需要一个 `URL`，代码如下：



```
URL obj =**new**URL(url);



HttpURLConnection con = (HttpURLConnection) obj.openConnection();
```



#### 1.2 添加 HTTP 请求头


得到 HTTP 连接对象之后，我们就可以进行 HTTP 操作了，我们可以添加任意的 HTTP 请求头，然后执行我们需要的 GET 或者 POST 请求。我们像下面这样，添加两个 HTTP 头（User-Agent 和 Accept-Language）：



```
con.setRequestProperty("User-Agent","Mozilla/5.0
 (Windows NT 6.1; WOW64) ...");



con.setRequestProperty("Accept-Language","en-US,en;q=0.5");
```




对于有些爬虫来说，这个设置是必要的，譬如有很多网站会对请求头中的 Referer 进行检查，以此来防爬或者防盗链。又譬如有些网站还会对 User-Agent 进行检查，根据这个字段来过滤一些非浏览器的请求。如果请求头设置不对的话，很可能是爬不下正确的数据的。

#### 1.3 HTTP GET


HTTP 协议中定义了很多种 HTTP 请求方法：GET、POST、PUT、DELETE、OPTIONS 等等，其中最常用到的就是 GET 和 POST，因为在浏览器中大多都是使用这两种请求方法。


使用 `HttpURLConnection` 来发送
 GET 请求是非常简单的，通过上面的代码创建并初始化好一个 HTTP 连接之后，就可以直接来发送 GET 请求了。



```
con.setRequestMethod("GET");



**int**responseCode
 = con.getResponseCode();



String responseBody = readResponseBody(con.getInputStream());
```




可以看到，代码非常简洁，没有任何累赘的代码，甚至没有任何和发送请求相关的代码，请求就是在 `getResponseCode()` 函数中默默的执行了。其中 `readResponseBody()` 函数用于读取流并转换为字符串，具体的实现如下：



```
// 读取输入流中的数据



**private**String
 readResponseBody(InputStream inputStream) **throws**IOException
 {



 



    BufferedReader
 in = **new**BufferedReader(



            **new**InputStreamReader(inputStream));



    String
 inputLine;



    StringBuffer
 response = **new**StringBuffer();



 



    **while**((inputLine
 = in.readLine()) != **null**)
 {



        response.append(inputLine);



    }



    in.close();



     



    **return**response.toString();



}
```



#### 1.4 HTTP POST


使用 `HttpURLConnection` 来模拟
 POST 请求和 GET 请求基本上是一样的，但是有一点不同，由于 POST 请求一般都会向服务端发送一段数据，所以 `HttpURLConnection` 提供了一个方法 `setDoOutput(true)` 来表示有数据要输出给服务端，并可以通过 `getOutputStream()` 得到输出流，我们将要写的数据通过这个输出流
 POST 到服务端。



```
con.setRequestMethod("POST");



con.setDoOutput(**true**);



DataOutputStream wr =**new**DataOutputStream(con.getOutputStream());



wr.writeBytes(parameter);



wr.flush();



wr.close();
```




POST 完成之后，和 GET 请求一样，我们通过 `getInputStream()` 函数来读取服务端返回的数据。

## 二、使用 HttpClient 发送 HTTP 请求


使用 Java 自带的 `HttpURLConnection` 类完全可以满足我们的一些日常需求，不过对于网络爬虫这种高度依赖于
 HTTP 工具类的程序来说，它在有些方面还是显得略为不足（我们之后会讨论到），我们需要一种扩展性定制性更强的类。[Apache 的 HttpClient](http://hc.apache.org/) 就是首选。

`HttpClient` 是
 Apache Jakarta Common 下的子项目，用来提供高效的、最新的、功能丰富的支持 HTTP 协议的客户端编程工具包。它相比传统的 `HttpURLConnection`，增加了易用性和灵活性，它不仅让客户端发送
 HTTP 请求变得更容易，而且也方便了开发人员测试接口（基于 HTTP 协议的），即提高了开发的效率，也方便提高代码的健壮性。


好了，关于 `HttpClient` 介绍的大话空话套话结束，让我们来看一段使用 `HttpClient` 来模拟
 HTTP GET 请求的代码片段：



```
CloseableHttpClient httpclient = HttpClients.createDefault();



HttpGet request =**new**HttpGet(url);



request.setHeader("User-Agent","Mozilla/5.0
 (Windows NT 6.1; WOW64) ...");



CloseableHttpResponse response = httpclient.execute(request);



 



// read response
```



#### 2.1 HTTP GET 与 HTTP POST


上面的示例代码展示了如何使用 `HttpClient` 来模拟
 HTTP GET 请求，可以看出 `HttpClient` 对每一种
 HTTP 方法都准备了一个类，GET 请求使用 `HttpGet` 类，POST
 请求使用 `HttpPost` 类。


和上文中介绍的一样，在发送 POST 请求时，需要向服务端写入一段数据，我们这里使用 `setEntity()` 函数来写入数据：



```
String parameter ="key=value";



HttpPost request =**new**HttpPost(url);



request.setEntity(



    **new**StringEntity(parameter,
 ContentType.create("application/x-www-form-urlencoded"))



);
```




Entity 是 `HttpClient` 中的一个特别的概念，有着各种的
 Entity ，都实现自 HttpEntity 接口，输入是一个 Entity，输出也是一个 Entity 。要注意的是，在这里我采用一种取巧的方式，直接使用 `StringEntity` 来写入
 POST 数据，然后将 Content-type 改成 application/x-www-form-urlencoded ，这样就和浏览器里的表单提交请求一致了。但是我们要知道的是，一般情况下，我们可能还会使用`UrlEncodedFormEntity` 这个类，只是在写爬虫的时候比较繁琐，使用起来像下面这样：



```
List<NameValuePair> nvps =newArrayList
 <NameValuePair>();



nvps.add(newBasicNameValuePair("key","value"));



request.setEntity(newUrlEncodedFormEntity(nvps,
 HTTP.UTF_8));
```



#### 2.2 读取响应


正如上文所说，`HttpClient` 的输入是一个
 Entity，输出也是一个 Entity 。这和 `HttpURLConnection` 的流有些不同，但是基本理念是相通的。对于
 Entity ，`HttpClient` 提供给我们一个工具类 `EntityUtils`，使用它可以很方便的将其转换为字符串。



```
CloseableHttpResponse response = httpclient.execute(request);



String responseBody = EntityUtils.toString(response.getEntity());
```



#### 2.3 HttpEntiy 接口


上面说到了 `HttpClient` 中的 `HttpEntity` 这个接口，这个接口在使用
 HttpClient 的时候相当重要，这里对其略做补充。


大多数的 HTTP 请求和响应都会包含两个部分：头和体，譬如请求头请求体，响应头响应体， Entity 也就是这里的 “体” 部分，这里暂且称之为 “实体” 。一般情况下，请求包含实体的有 POST 和 PUT 方法，而绝大多数的响应都是包含实体的，除了 HEAD 请求的响应，还有 204 No Content、304 Not Modified 和 205 Reset Content 这些不包含实体。

`HttpClient` 将实体分为三种类型：
- 流类型（streamed）：实体内容从流中读取的，通常只能读一次
- 自包含类型（self-contained）：手工创建的，通常可重复读取
- 包装类型（wrapping）：使用一种实体包装另一种实体


上面的例子中我们直接使用工具方法 `EntityUtils.toString()` 将一个
 HttpEntity 转换为字符串，虽然使用起来非常方便，但是要特别注意的是这其实是不安全的做法，要确保返回内容的长度不能太长，如果太长的话，还是建议使用流的方式来读取：



```
CloseableHttpResponse
 response = httpclient.execute(request);



HttpEntity entity
 = response.getEntity();



**if**(entity
 != **null**)
 {



    **long**length
 = entity.getContentLength();



    **if**(length
 != -1&&
 length < 2048)
 {



        String
 responseBody = EntityUtils.toString(entity);



    }



    **else**{



        InputStream
 in = entity.getContent();



        //
 read from the input stream ...



    }



}
```



## 三、关于 HTTPS 请求


到这里为止，我们一直忽略了 HTTP 请求和 HTTPS 请求之间的差异，因为大多数情况下，我们确实不需要关心 URL 是 HTTP 的还是 HTTPS 的，上面给出的代码也都能很好的自动处理这两种不同类型的请求。


但是，我们还是应该注意下这两种请求的差异，后面我们在介绍 HTTP 代理时将会特别看到这两者之间的差异。另外还有一点，在调用`URL` 的 `openConnection()` 方法时，如果
 URL 是 HTTP 协议的，返回的是一个 HttpURLConnection 对象，而如果 URL 是 HTTPS 协议的，返回的将是一个 HttpsURLConnection 对象。



