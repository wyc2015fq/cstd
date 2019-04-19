# 服务器session和jwt之争 - 三少GG - CSDN博客
2015年12月12日 00:11:15[三少GG](https://me.csdn.net/scut1135)阅读数：15149
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)
**1. session**
session和cookie的目的相同，都是为了克服http协议无状态的缺陷，但完成的方法不同。session通过cookie，在客户端保存session
 id，而将用户的其他会话消息保存在服务端的session对象中，与此相对的，cookie需要将所有信息都保存在客户端。因此cookie存在着一定的安全隐患，例如本地cookie中保存的用户名密码被破译，或cookie被其他网站收集（例如：1. appA主动设置域B cookie，让域B cookie获取；2. XSS，在appA上通过javascript获取document.cookie，并传递给自己的appB）。
**2. jwt:**
**真正讲明白的一篇文章： https://scotch.io/tutorials/the-ins-and-outs-of-token-based-authentication**
# The Ins and Outs of Token Based Authentication
**0. **
# JSON Web Tokens (JWT) 用于用户验证
23 Oct 2015
jwt 不仅可用于验证用户还可用于 server 间通信验证
## 传统验证方式(目前大部分网站使用的方式):
## 现代网页应用验证用户时面临的困难
- app server 可能是分布式的, 有很多 server, 在一个 server 上登录了,
其他的没登陆, 需要额外工具来解决这个问题([sticky sessions](http://stackoverflow.com/questions/10494431/sticky-and-non-sticky-sessions))
- app 使用 RESTfull api 来获取数据, RESTful api 的原则是 stateless, 但使用 session, 使用 session 和 cookies 会引入 state; 另外, 当 API server 与 app server
可能是两个 server, 需要 允许 CORS([Cross-Origin Resource Sharing](https://developer.mozilla.org/en-US/docs/Web/HTTP/Access_control_CORS)), 但是 cookies 只能在同一个 domain 中使用
- app 可能需要下游服务, 每个 server 都要处理 cookie(???)
## 解决办法: 使用 JWT 方式来验证用户
JWT 方案不使用 session 基于 token.
用户注册之后, 服务器生成一个 JWT token返回给浏览器, 浏览器向服务器请求
数据时将 JWT token 发给服务器, 服务器用 signature 中定义的方式解码
JWT 获取用户信息.
一个 JWT token包含3部分:
1. header: 告诉我们使用的算法和 token 类型
2. Payload: 必须使用 `sub` key 来指定用户 ID, 还可以包括其他信息
比如 email, username 等.
3. Signature: 用来保证 JWT 的真实性. 可以使用不同算法
**1. **和Session方式存储id的差异
Session方式存储用户id的最大弊病在于要占用大量服务器内存，对于较大型应用而言可能还要保存许多的状态。一般而言，大型应用还需要借助一些KV数据库和一系列缓存机制来实现Session的存储。
而JWT方式将用户状态分散到了客户端中，可以明显减轻服务端的内存压力。除了用户id之外，还可以存储其他的和用户相关的信息，例如该用户是否是管理员、用户所在的分桶（见[《你所应该知道的A/B测试基础》一文](/2015/08/27/introduction-to-ab-testing/）等。
虽说JWT方式让服务器有一些计算压力（例如加密、编码和解码），但是这些压力相比磁盘I/O而言或许是半斤八两。具体是否采用，需要在不同场景下用数据说话。
**2. **http://blog.rainy.im/2015/06/10/react-jwt-pretty-good-practice/
区别（仔细揣摩）
##1.
这么基础的问题，居然还是没人说到点子上，最关键的一点是： 
*** Session的状态是存储在服务器端，客户端只有session id；而Token的状态是存储在客户端 其它细枝末节的区别，全部是由这一点造成的。 **
就没人想过为什么token-based的authentication需要一堆secret key来干嘛么？ 
因为状态信息全部是放在客户端，为了避免被篡改，于是需要用密码学的方法来签名/加密。 
可以自己去这里玩玩JWT的Debugger： 
[http://jwt.io/](http://jwt.io/)
一进去你就会注意到两点： 
1. Token解码后就包含所有登陆信息 
2. Token你随便改一位都会提示无效 
##2.
||session 和 token 就是个词而已…… 广义来说一切**维护用户状态**的技术都是session，一切动态生成的服务端有能力**鉴别真假**而本身无涵义的字符串都是token|
更多的详见： 
[http://www.slideshare.net/derekperkins/authentication-cookies-vs-jwts-and-why-youre-doing-it-wrong](http://www.slideshare.net/derekperkins/authentication-cookies-vs-jwts-and-why-youre-doing-it-wrong)
# Goodbye PHP Sessions, Hello JSON Web Tokens
17332 views
- [security](https://coderwall.com/trending?search=security)
- [rest](https://coderwall.com/trending?search=rest)
- [json](https://coderwall.com/trending?search=json)
- [javascript](https://coderwall.com/trending?search=javascript)
- [php](https://coderwall.com/trending?search=php)
 on Jun 20, 2014
REST API's are meant to be stateless. What that means is that each request from a client should include all the information needed to process the request.**In other words, if you are writing a REST API in PHP then you should not be using $_SESSION to store data about the client's session. But then how do we remember if a client is logged in or anything else about their state?The only possibility is that the client must be tasked with keeping track of the state.**How could this ever be done securely? The client can't be trusted!
Enter JSON web tokens. A JSON web token is a bit of JSON, perhaps something that looks like this:
```
{
    "user": "alice",
    "email": "test@nospam.com"
}
```
Of course, we can't just give this to a client and have them give it back to us without some sort of assurance that it hasn't been tampered with. After all, what if they edit the token as follows:
```
{
    "user": "administrator",
    "email": "test@nospam.com"
}
```
**The solution to this is that JSON web tokens are signed by the server. If the client tampers with the data then the token's signature will no longer match and an error can be raised.**
The JWT PHP class makes this easy to do. For example, to create a token after the client successfully logs in, the following code could be used:
```php
$token = array();
$token['id'] = $id;
echo JWT::encode($token, 'secret_server_key');
```
And then on later API calls the token can be retrieved and verified by this code:
```
$token = JWT::decode($_POST['token'], 'secret_server_key');
echo $token->id;
```
If the token has been tampered with then $token will be empty there will not be an id available. The JWT class makes sure that invalid data is never made available. If the token is tampered with, it will be unusable. Pretty simple stuff!
其余参考资料：
0. **https://auth0.com/blog/2014/01/07/angularjs-authentication-with-cookies-vs-token/**
What are the benefits of using a token-based approach?
- Cross-domain / CORS: cookies + CORS don't play well across different domains. A token-based approach allows you to make AJAX calls to any server, on any domain because you use an HTTP header to transmit the user information.
- Stateless (a.k.a. Server side scalability): there is no need to keep a session store, the token is a self-contanined entity that conveys all the user information. The rest of the state lives in cookies or local storage on
 the client side.
- CDN: you can serve all the assets of your app from a CDN (e.g. javascript, HTML, images, etc.), and your server side is just the API.
- Decoupling: you are not tied to a particular authentication scheme. The token might be generated anywhere, hence your API can be called from anywhere with a single way of authenticating those calls.
- Mobile ready: when you start working on a native platform (iOS, Android, Windows 8, etc.) cookies are not ideal when consuming a secure API (you have to deal with cookie containers). Adopting a token-based approach simplifies
 this a lot.
- CSRF: since you are not relying on cookies, you don't need to protect against cross site requests (e.g. it would not be possible to `<iframe>`your
 site, generate a POST request and re-use the existing authentication cookie because there will be none).
- Performance: we are not presenting any hard perf benchmarks here, but a network roundtrip (e.g. finding a session on database) is likely to take more time than calculating an [`HMACSHA256`](http://en.wikipedia.org/wiki/Hash-based_message_authentication_code) to
 validate a token and parsing its contents.
- Login page is not an special case: If you are using [Protractor](https://github.com/angular/protractor) to
 write your functional tests, you don't need to handle [any special case for login](https://github.com/angular/protractor/issues/51).
- Standard-based: your API could accepts a standard [JSON
 Web Token](http://tools.ietf.org/html/draft-ietf-oauth-json-web-token)(JWT). This is a standard and there are multiple backend libraries ([.NET](https://www.nuget.org/packages?q=JWT),[Ruby](http://rubygems.org/search?utf8=%E2%9C%93&query=jwt), [Java](https://code.google.com/p/jsontoken/), [Python](https://github.com/davedoesdev/python-jwt), [PHP](https://github.com/firebase/php-jwt))
 and companies backing their infrastructure (e.g. [Firebase](https://www.firebase.com/docs/security/custom-login.html), [Google](https://developers.google.com/accounts/docs/OAuth2ServiceAccount#overview), [Microsoft](https://github.com/MSOpenTech/AzureAD-Node-Sample/wiki/Windows-Azure-Active-Directory-Graph-API-Access-Using-OAuth-2.0)).
 As an example, [Firebase](https://www.firebase.com/docs/security/custom-login.html) allows their customers to use any authentication
 mechanism, as long as you generate a JWT with certain pre-defined properties, and signed with the shared secret to call their API.
1. http://www.slideshare.net/derekperkins/authentication-cookies-vs-jwts-and-why-youre-doing-it-wrong
###### 2. JSON Web Token - 在Web应用间安全地传递信息  http://blog.leapoahead.com/2015/09/06/understanding-jwt/
###### 3. JWT 在前后端分离中的应用与实践 http://ju.outofmemory.cn/entry/188871
###### 为什么选jwt？
To say a few problems…
- Sessions: We need to store our sessions somewhere. By default they are just
 stored on server’s memory and having thousand of them doesn’t help. Redis does help but without sessions there are no problems then :P
- Mobile: Native mobile apps seems to have problems working with cookies so
 if we need to query a remote API, maybe session auth is not the best solution.
- CSRF: If we go down the cookies way, you really need to do `CSRF` to
 avoid cross site requests. That is something we can forget when using JWT as you will see.
- CORS: Have you fight with CORS and cookies? I hope it went right because
 when it doesn’t, we have a problem.
###### 2016.7.2
###### golang 与jwt 
###### https://xiequan.info/go%E4%B8%8Ejson-web-token/
