# JWT简介json web token  bear token - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月11日 08:59:08[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：312
[https://jwt.io/introduction/](https://jwt.io/introduction/)
一个JWT实际上就是一个字符串，它由三部分组成，**头部**、**载荷****Payload**与**签名**。
 JWTs can be signed using a secret (with the **HMAC** algorithm)
 or a public/private key pair using **RSA**.
/*
|--------------------------------------------------------------------------
| Required Claims
|--------------------------------------------------------------------------
|
| Specify the required claims that
must exist in any token.
| A TokenInvalidException will be thrown if any of these claims are not
| present in the payload.
|
*/
'required_claims' => [
'iss',
'iat',
'exp',
'nbf',
'sub',
'jti',
],
示例：
{
"iss": "http://laravel_api.app/api/register",
"iat": 
1502356728,
"exp": 1502360328,（相差3600s）
"nbf": 
1502356728,
"jti": "R0Gd00AvOW259LGo",
"sub": 11,
"prv": "3787fba1618a930526aca6c8bb9b44b83f297726"
}
这里面的前6个字段都是由JWT的标准所定义的。
- sub: 该JWT所面向的用户
- iss: 该JWT的签发者
- iat(issued at): 在什么时候签发的token
- exp(expires): token什么时候过期
- nbf(not before)：token在此时间之前不能被接收处理
- jti：JWT ID为web token提供唯一标识
iss是issuer的简写，表明请求的实体，可以是发出请求的用户的信息。
There are three types of claims: *reserved*, *public*,
 and *private*claims.
像上面的是reserved:
**iss** (issuer), **exp** (expiration
 time), **sub**(subject), **aud** (audience),
 and others.
An example of payload could be:{
 "sub": "1234567890", "name":
"John Doe", "admin":
true}

{
"iss": "John Wu JWT",
"iat": 1441593502,
"exp": 1441594722,
"aud": "www.example.com",
"sub": "jrocket@example.com",
"from_user": "B",
"target_user": "A"
}
这里面的前五个字段都是由JWT的标准所定义的。
aud: 接收该JWT的一方
将上面的JSON对象进行[base64编码]可以得到下面的字符串。这个字符串我们将它称作JWT的**Payload**（载荷）。
|1|eyJpc3MiOiJKb2huIFd1IEpXVCIsImlhdCI6MTQ0MTU5MzUwMiwiZXhwIjoxNDQxNTk0NzIyLCJhdWQiOiJ3d3cuZXhhbXBsZS5jb20iLCJzdWIiOiJqcm9ja2V0QGV4YW1|

JWT还需要一个头部，头部用于描述关于该JWT的最基本的信息，例如其类型以及签名所用的算法等。这也可以被表示成一个JSON对象。
|1234|{"typ": "JWT","alg": "HS256"|
对它也要进行Base64编码，之后的字符串就成了JWT的**Header**（头部）。
|1|eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9|
将上面的两个编码后的字符串用句号.连接在一起（头部在前），就形成了
|1|eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJmcm9tX3VzZXIiOiJCIiwidGFyZ2V0X3VzZXIiOiJBIn0|
最后，我们将上面拼接完的字符串用HS256算法进行加密。在加密的时候，我们还需要提供一个密钥（secret）
得到我们加密后的内容
|1|rSWamyAYwuHCo7IFAgd1oRpSP7nzL7BF5t7ItqpKViM|
最后将这一部分签名也拼接在被签名的字符串后面(2个句号)，我们就得到了完整的JWT
|1|eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJmcm9tX3VzZXIiOiJCIiwidGFyZ2V0X3VzZXIiOiJBIn0.rSWamyAYwuHCo7IFAgd1oRpSP7nzL7BF5t7ItqpKViM|
[https://your.awesome-app.com/make-friend/?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJmcm9tX3VzZXIiOiJCIiwidGFyZ2V0X3VzZXIiOiJBIn0.rSWamyAYwuHCo7IFAgd1oRpSP7nzL7BF5t7ItqpKViM](https://your.awesome-app.com/make-friend/?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJmcm9tX3VzZXIiOiJCIiwidGFyZ2V0X3VzZXIiOiJBIn0.rSWamyAYwuHCo7IFAgd1oRpSP7nzL7BF5t7ItqpKViM)
多点登录”
Set-Cookie: jwt=lll.zzz.xxx;
HttpOnly; max-age=980000; domain=.taobao.com
必须使用HttpOnly属性来防止Cookie被JavaScript读取，从而避免[跨站脚本攻击（XSS攻击）](http://www.cnblogs.com/bangerlee/archive/2013/04/06/3002142.html)。
注意domain必须设置为一个点加顶级域名，即.taobao.com。这样，taobao.com和*.taobao.com就都可以接受到这个Cookie，并获取JWT了。
一般放在HTTP的headers 参数里面的authorization里面，值的前面加Bearer关键字和空格。除此之外，也可以在url和request
 body中传递。
Authorization: Bearer {yourtokenhere}

- OAuth是一个授权的开放网络标准，最终目的是取得token（令牌）
- Token 令牌，视为用户登录成功，通行的唯一令牌
- JWT是生成token的轻巧规范，可以结合一起使用
[如何防止第三方网站私自调用网站的后端接口](https://segmentfault.com/q/1010000004234305)

[http://qtsite.cn/post/61.html](http://qtsite.cn/post/61.html)
[http://blog.csdn.net/hooloo/article/details/49714259](http://blog.csdn.net/hooloo/article/details/49714259)
**Unresolvable dependency resolving [Parameter #0 [ $app ]] in class Illuminate\Cache\CacheManager**
Dependency Bind from
$app->bind(Illuminate\Cache\CacheManager::class, function ($app) { return
 new Illuminate\Cache\CacheManager($app);});
Unresolvable dependency resolving [Parameter #0 [ <required> $app ]] in class Illuminate\Auth\AuthManager
$app->bind(Illuminate\Auth\AuthManager::class, function ($app) { return new
 Illuminate\Auth\AuthManager($app);});

[http://www.jianshu.com/p/576dbf44b2ae](http://www.jianshu.com/p/576dbf44b2ae)
[http://laravelacademy.org/post/3640.html](http://laravelacademy.org/post/3640.html)
[http://blog.leapoahead.com/2015/09/06/understanding-jwt/](http://blog.leapoahead.com/2015/09/06/understanding-jwt/)
[https://www.zhihu.com/question/36135526](https://www.zhihu.com/question/36135526)
