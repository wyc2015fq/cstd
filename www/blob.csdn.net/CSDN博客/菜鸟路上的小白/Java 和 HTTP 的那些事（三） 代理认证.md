# Java 和 HTTP 的那些事（三） 代理认证 - 菜鸟路上的小白 - CSDN博客





2016年09月14日 13:40:03[somnus_小凯](https://me.csdn.net/u012486840)阅读数：6605









前面一篇博客介绍了在 Java 中使用 `HttpURLConnection` 和 `HttpClient` 通过代理访问
 HTTP 站点的方法，但是可以看到代码中使用的代理都是免费公开的代理，不需要用户名密码就能直接访问。由于互联网上公开的代理安全性不能保证，这种代理随便用用即可，如果要慎重起见，我推荐大家还是自己搭建代理服务器。但是有一点要特别注意，如果自己搭建代理服务器的话，一定不要公开，要设置用户名密码，一般情况下，我们使用简单的基本身份认证就可以了。如果你不设置密码的话，没过几天你就会发现你的服务器会卡到爆，登上去使用 netstat 一看，几百上千个连接，服务器带宽全占满了。这是因为互联网上有着大量的代理扫描程序在没日没夜的扫描，你搭建的代理服务器没设密码，或者弱密码，都会被扫出来，而扫出来的后果就是，你的代理服务器被公开到各大免费代理站点，然后所有人都来连你的代理服务器，直到把你的带宽流量耗尽。

## 一、关于 HTTP 的身份认证


我们这里给代理服务器设置了用户名和密码之后，无论在程序中，还是在浏览器里使用该代理时，都需要进行身份认证了。HTTP 协议最常见的认证方式有两种：基本认证（Basic authentication）和摘要认证（Digest authentication）。HTTP 的认证模型非常简单，就是所谓的质询/响应（challenge/response）框架：当用户向服务器发送一条 HTTP 请求报文时，服务器首先回复一个“认证质询”响应，要求用户提供身份信息，然后用户再一次发送 HTTP 请求报文，这次的请求头中附带上身份信息（用户名密码），如果身份匹配，服务器则正常响应，否则服务器会继续对用户进行质询或者直接拒绝请求。


摘要认证的实现比基本认证要复杂一点，在平时的使用中也并不多见，这里忽略，如果想详细了解它，可以查看维基百科上关于 [HTTP
 摘要认证](https://zh.wikipedia.org/wiki/HTTP%E6%91%98%E8%A6%81%E8%AE%A4%E8%AF%81) 的解释。这里重点介绍下 [HTTP
 基本认证](https://zh.wikipedia.org/wiki/HTTP%E5%9F%BA%E6%9C%AC%E8%AE%A4%E8%AF%81)，因为无论是代理服务器对用户进行认证，还是 Web 服务器对用户进行认证，最常用的手段都是 HTTP 基本认证，它实现简单，容易理解，几乎所有的服务器都能支持它。


一个典型的 HTTP 基本认证，如下图所示，图片摘自[《HTTP 权威指南》](http://book.douban.com/subject/10746113/)：

![http-basic-auth.png](https://img-blog.csdn.net/20160921153257868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


用户第一次向服务器发起请求时，服务器会返回一条 401 Unauthorized 响应，如果用户是使用浏览器访问的话，浏览器会弹出一个密码提示框，提醒用户输入用户名和密码，于是用户重新发起请求，在第二次请求中将在 Authorization 头部添加上身份信息，这个身份信息其实只是简单的对用户输入的用户名密码做了 [Base64
 编码](https://zh.wikipedia.org/wiki/Base64) 处理，服务器对用户的认证成功之后，返回 200 OK 。

## 二、使用基本认证

### 2.1 区分 Proxy 认证 和 WWW 认证


这篇博客本来是介绍代理认证的，但是代理认证其实只是 HTTP 身份认证中的一种而已，所以上面大部分内容对于代理认证来说是一样的，包括质询/响应框架以及身份认证的基本流程。不过要在代码里实现这两种认证，细节方面会有所不同，下面是两种认证的一个对比。
- 根本区别
- WWW 认证：指的是 Web 服务器对客户端发起的认证
- Proxy 认证：指的是代理服务器对客户端发起的认证

- 响应的状态码不同
- WWW 认证：第一次访问时响应 401 Unauthorized
- Proxy 认证：第一次访问时响应 407 Unauthorized

- 认证头部不同
- WWW 认证：WWW-Authenticate, Authorization, Authentication-Info
- Proxy 认证：Proxy-Authenticate, Proxy-Authorization, Proxy-Authentication-Info


### 2.2 手工设置认证头部


通过上面的介绍我们了解到，要实现 HTTP 身份认证，无论是 WWW 认证也好，Proxy 认证也罢，其实只需要在 HTTP 的请求头部添加一个认证的头部（`Authorization` 或者 `Proxy-Authorization`）。认证头部的信息就是用户名和密码，将用户名和密码使用冒号分割，然后再对其进行
 Base64 编码即可，我们使用 `HttpURLConnection` 来模拟这个过程，如下：



```
URL obj =**new**URL(url);



HttpURLConnection con = (HttpURLConnection)
 obj.openConnection();



 



// 设置认证头部



**final**String
 userName = "username";



**final**String
 password = "password";



String nameAndPass = userName +":"+
 password;



String encoding =**new**String(Base64.encodeBase64(nameAndPass.getBytes()));



con.setRequestProperty("Authorization","Basic
 " 
+ encoding);



 



con.setRequestMethod("GET");



String responseBody = readResponseBody(con.getInputStream());
```




如果是代理认证的话，设置头部的代码改成下面这样：



```
con.setRequestProperty("Proxy-Authorization","Basic
 " 
+ encoding);
```




这种方式最为原始，也最为简单直白，几乎没什么好解释的。


但是在我使用这种方式来访问 HTTPS 站点的时候却遇到了问题：第一种情况是访问需要进行基本身份认证的 HTTPS 站点，测试通过；第二种情况是访问 HTTPS 站点，通过一个代理，代理需要进行基本身份认证，测试失败，返回下面的错误：

> 
java.io.IOException: Unable to tunnel through proxy. Proxy returns "HTTP/1.0 407 Proxy Authentication Required"

at sun.net.www.protocol.http.HttpURLConnection.doTunneling(HttpURLConnection.java:2085)

at sun.net...https.AbstractDelegateHttpsURLConnection.connect(AbstractDelegateHttpsURLConnection.java:183)



我的第一直觉是通过代理访问 HTTPS 站点时，代理的认证信息应该没有发出去，果不其然，使用 Wireshark 截获了两次请求的数据包，第二次请求里没有我们加的 `Proxy-Authorization` 头部。

在 Google 上搜索这个问题，发现早在 2000 年（那可是 16 年前，当时 Java 的版本还是 1.0 呢）就有人在 JDK 的 bug database 里提交了这个问题（[JDK-4323990](http://bugs.java.com/bugdatabase/view_bug.do?bug_id=4323990)），看这个问题的更新状态应该是在
 JDK 1.4 版本里已经修复了，但是为啥我这里还是测试不通过呢！


下面是测试的完整代码，始终不理解为什么通不过，还需要继续研究下 `HttpURLConnection` 中的实现细节，如果有高人知道，还请多多指教。



```
@Test



**publicvoid**basicHttpsGetWithProxyNeedAuthUsingBase64Basic()**throws**Exception
 {



     



    String
 url = "[https://www.baidu.com](https://www.baidu.com/)";



    Proxy
 proxy = **new**Proxy(Proxy.Type.HTTP,**new**InetSocketAddress("139.132.22.90",8213));



    **final**String
 proxyUserName = "username";



    **final**String
 proxyPassword = "password";



     



    URL
 obj = **new**URL(url);



    HttpsURLConnection
 con = (HttpsURLConnection) obj.openConnection(proxy);



     



    String
 nameAndPass = proxyUserName + ":"+
 proxyPassword;



    String
 encoding = **new**String(Base64.encodeBase64(nameAndPass.getBytes()));



    con.setRequestProperty("Proxy-Authorization","Basic
 " 
+ encoding);



     



    con.setRequestMethod("GET");



    String
 responseBody = readResponseBody(con.getInputStream());



    System.out.println(responseBody);



}
```



### 2.3 实现 Authenticator


自己手工设置认证头部虽然简单，而且在某些情况下可以达到意想不到的效果。但是使用这种方法可能会出现问题（像上面提到的访问 HTTPS 站点时遇到的问题），而且 Proxy 认证和 WWW 认证这两种情形还需要分别处理，不是很方便。除了可以自己手工拼 HTTP 请求头部之外，其实还有另一种更简单的方法，那就是 java.net 提供的 `Authenticator` 类。`Authenticator` 类是一个抽象类，必须先定义一个类来继承它，然后重写它的 `getPasswordAuthentication()` 这个方法，定义新类比较繁琐，我们可以直接使用匿名类，如下：



```
Authenticator authenticator =**new**Authenticator()
 {



    **public**PasswordAuthentication
 getPasswordAuthentication() {



        **returnnew**PasswordAuthentication(userName,
 password.toCharArray());



    }



};



Authenticator.setDefault(authenticator);
```




请求部分代码还是一样，如果使用代理，`openConnection()` 方法就加个代理参数。这种方式不区分是
 Proxy 认证还是 WWW 认证，如果是 Proxy 认证，userName 和 password 就设置成代理的用户名密码，如果是 WWW 认证，则设置成 Web 服务器认证的用户名密码。


要特别注意的一点是，通过这种方式设置认证方式是 JVM 全局的，同一个 JVM 下的所有应用程序都会受影响。


然后抱着实验精神，和 2.2 节一样，我也做了几个测试，看看 `Authenticator` 类对
 HTTPS 的支持情况，发现无论是带认证的 HTTPS 站点，还是通过带认证的代理去访问 HTTPS 站点，都没问题。不过在测试的过程中还是发现了一些有趣的现象，在使用正确的用户名和密码时都可以成功认证，但是在使用错误的用户名和密码时，不同情况下的错误情形略有不同。有些情况可能报 407 错误，有些情况可能报 401 错误，有些情况还可能报 “java.net.ProtocolException: Server redirected too many times (20)”。虽然这可能并没有什么卵用，我还是在这里记录一下吧，算是做个总结。

![http-auth-example.png](https://img-blog.csdn.net/20160921153319055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：上图是使用 [Lucidchart](https://www.lucidchart.com/) 在线画的，强大的在线版 Visio ，推荐！

### 2.4 使用 HttpClient 的 CredentialsProvider


和 `Authenticator` 类似，HttpClient
 也提供了一个类 `CredentialsProvider` 来实现
 HTTP 的身份认证，它的子类`BasicCredentialsProvider` 用于基本身份认证。和 `Authenticator` 不一样的是，这种方法不再是全局的，而是针对指定的
 HttpClient 实例有效，可以根据需要来设置。这里不再多述，示例代码如下：



```
CredentialsProvider credentialsProvider
 = **new**BasicCredentialsProvider();



credentialsProvider.setCredentials(AuthScope.ANY,**new**UsernamePasswordCredentials(userName,
 password));



 



CloseableHttpClient httpclient = HttpClients.custom()



    .setDefaultCredentialsProvider(credentialsProvider)



    .build();
```



## 



