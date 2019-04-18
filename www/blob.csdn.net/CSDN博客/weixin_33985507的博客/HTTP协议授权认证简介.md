# HTTP协议授权认证简介 - weixin_33985507的博客 - CSDN博客
2016年09月06日 11:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：16
我们平时在打开网页时或者在用浏览器访问你局域网的路由器时，经常会出现弹出登陆页面框要进行登陆授权后才能继续访问。比如如下提示：
![1432482-8177187b36d18700.png](https://upload-images.jianshu.io/upload_images/1432482-8177187b36d18700.png)
HTTP授权验证
上面图片展示的一个场景是客户端在使用HTTP协议和服务端通信时，服务器需要对客户端进行授权认证，客户端输入正确的用户密码后才能继续进行访问。那这个流程是如何实现的呢？
1.客户端请求需要授权认证的URL地址。
```
GET / HTTP/1.1
Accept: */*
Accept-Language: zh-cn
UA-CPU: x86
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; InfoPath.2; .NET CLR 2.0.50727)
Host: 192.168.1.1
```
2.服务端返回401，告诉客户端这个请求需要授权认证，并且指定授权认证的方式。（在IE中系统会弹出对话框让输入用户和密码）
```
HTTP/1.1 401 Unauthorized
Server: Router
Connection: close
WWW-Authenticate: Basic realm="TP-LINK Wireless N Router WR740N"
[response body]
```
3.客户端按服务器要求的授权方式，拼装用户和密码，放入到请求头中，并再次发送。
```
GET / HTTP/1.1
Accept: */*
Accept-Language: zh-cn
UA-CPU: x86
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; InfoPath.2; .NET CLR 2.0.50727)
Host: 192.168.1.1
Authorization: Basic YWRtaW46YWRtaW4=
```
4.服务端根据提供用户的密码，来决定返回成功还是失败。
```
HTTP/1.1 200 OK
Server: Router
Connection: close
[successful response body]
```
我们称上面这种授权认证机制叫challange/response机制，客户端发起请求，服务器发起挑战(challange)，客户端接收挑战，服务器返回挑战结果。
在HTTP协议1.0系统中只支持：基本认证(Basic realm)，而在1.1中支持摘要认证:(Digest realm)，和WSSE(WS-Security)认证  至于使用什么认证由服务端在返回的401响应中的**WWW-Authenticate**中指定。
1.基本认证(Basic realm)方式下，客户端接受挑战的方式是用:  BASE64(用户:密码)的结果返回给服务端。这种方法比较简单，而且在网络传输中很容易被破解。上面的例子用的就是基本认证方式，这时候客户端只需要在请求头中带上：
**Authorization: [认证的方式] [BASE64(用户:密码)]**
2.摘要认证(Digest realm)方式下，服务端在响应401时会返回一个随机数。 客户端 把MD5(用户，密码，随机数）后的值，连同用户一起通过Authorization:  请求头发送给服务端。服务端则根据用户取出对应的密码，随机数同样用MD5(用户，密码，随机数)， 如果两者相等则认为认证通过，否则认证失败。[查看具体交互过程](https://link.jianshu.com?t=http://blog.163.com/hlz_2599/blog/static/1423784742013415101252410/)
3.WSSE(WS-Security)认证。主要用于webservice服务的授权认证，具体请参考[WSSE](https://link.jianshu.com?t=https://www.oasis-open.org/committees/download.php/16782/wss-v1.1-spec-os-UsernameTokenProfile.pdf)
4.对于我们有时候访问第三方需要授权的资源时，我们采用OAuth协议来让第三方进行授权认证，因此在我们没有登录前，访问这些资源时服务端也可以返回401。同时它还带了如下的响应：
**WWW-Authenticate: OAuth realm=<your_realm>**
这样客户端就知道这种资源是需要OAuth认证的，这时候客户端应该启用Aauth认证机制，也就是OAuth协议可以用http认证的扩展来传输各种数据，也可以自己定义post方式来进行传输，具体参考OAUTH协议。
