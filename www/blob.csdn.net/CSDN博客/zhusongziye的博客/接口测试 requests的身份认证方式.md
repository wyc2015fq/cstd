# 接口测试 requests的身份认证方式 - zhusongziye的博客 - CSDN博客





2018年04月26日 21:47:36[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1530标签：[接口测试 requests的身份认证方式](https://so.csdn.net/so/search/s.do?q=接口测试 requests的身份认证方式&t=blog)
个人分类：[接口测试](https://blog.csdn.net/zhusongziye/article/category/7176798)









requests提供多种身份认证方式，包括基本身份认证、netrc 认证、摘要式身份认证、OAuth 1 认证、OAuth 2 与 OpenID 连接认证、自定义认证。这些认证方式的应用场景是什么呢？

### 身份认证的定义

身份认证是使用用户提供的凭证来识别用户。
- 
session会话保存，用来保持会话的状态；

- 
token是对用户进行授权。

- 
身份认证和授权的关系：需要先获取身份信息才能进行授权


### 身份认证的类型

### 1、基本身份认证
- 
HTTP Basic Auth是HTTP1.0提出的认证方式

- 
客户端对于每一个realm，通过提供用户名和密码来进行认证的方式

- 
当认证失败时，服务器收到客户端请求，返回401 UNAUTHORIZED，同时在HTTP响应头的WWW-Authenticate域说明认证方式及认证域

`# 响应头WWW-Authenticate： Basic realm="***"`- 
requests以 HTTP Basic Auth 发送请求,示例：


![](https://img-blog.csdn.net/20180426214406111?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 
事实上，HTTP Basic Auth 如此常见，Requests 就提供了一种简写的使用方式(事实上，很难找到用http基本身份认证方式的网站了）：


```
>>> requests.get('https://api.github.com/user', auth=('user', 'pass'), verify=False)
<Response [200]>
```

### 2、netrc 认证
- 
如果认证方法没有收到 auth 参数，Requests 将试图从用户的 netrc 文件中获取 URL 的 hostname 需要的认证身份

- 
如果找到了 hostname 对应的身份，就会以 HTTP Basic Auth 的形式发送请求

- 
简而言之，需要预先将认证的host及用户名、密码以明文方式存在netrc文件中，这种方式也是非常不安全的


### 3、摘要式身份认证
- 
digest authentication：在HTTP 1.1提出，目的是替代http 1.0提出的基本认证方式

- 
服务器收到客户端请求后返回401 UNAUTHORIZED，同时在WWW-Authenticate字段说明认证方式是Digest，其他信息还有realm域信息、nonce随机字符串、opaque透传字段（客户端会原样返回）等


##### 摘要式身份认证的步骤

参考文章：https://blog.csdn.net/jansony1/article/details/52430577
- 
客户端访问一个受http摘要认证保护的资源。

- 
服务器返回401状态以及nonce等信息，要求客户端进行认证。


```
HTTP/1.1 401 Unauthorized 
WWW-Authenticate: Digest 
realm="testrealm@host.com", 
qop="auth,auth-int", 
nonce="dcd98b7102dd2f0e8b11d0f600bfb0c093", 
opaque="5ccc069c403ebaf9f0171e9517f40e41"
```
- 
客户端将以用户名，密码，nonce值，HTTP方法, 和被请求的URI为校验值基础而加密（默认为MD5算法）的摘要信息返回给服务器。
认证必须的五个情报：


```
・ realm ： 响应中包含信息 
・ nonce ： 响应中包含信息 
・ username ： 用户名 
・ digest-uri ： 请求的URI 
・ response ： 以上面四个信息加上密码信息，使用MD5算法得出的字符串。
```

```
Authorization: Digest 
username="Mufasa", ← 客户端已知信息 
realm="testrealm@host.com", ← 服务器端质询响应信息 
nonce="dcd98b7102dd2f0e8b11d0f600bfb0c093", ← 服务器端质询响应信息 
uri="/dir/index.html", ← 客户端已知信息 
qop=auth, ← 服务器端质询响应信息 
nc=00000001, ← 客户端计算出的信息 
cnonce="0a4f113b", ← 客户端计算出的客户端nonce 
response="6629fae49393a05397450978507c4ef1", ← 最终的摘要信息 ha3 
opaque="5ccc069c403ebaf9f0171e9517f40e41" ← 服务器端质询响应信息
```
- 
如果认证成功，则返回相应的资源。如果认证失败，则仍返回401状态，要求重新进行认证。
注意事项：

※ nonce：随机字符串，每次返回401响应的时候都会返回一个不同的nonce。
※ nounce：随机字符串，每个请求都得到一个不同的nounce。
※ MD5(Message Digest algorithm 5，信息摘要算法)
1)用户名:realm:密码 ⇒ ha1
2)HTTP方法:URI ⇒ ha2
3)ha1:nonce:nc:cnonce:qop:ha2 ⇒ ha3
- 
避免将密码作为明文在网络上传递，相对提高了HTTP认证的安全性。

- 
当用户为某个realm首次设置密码时，服务器保存的是以用户名，realm，密码为基础计算出的哈希值（ha1），而非密码本身。

- 
如果qop=auth-int，在计算ha2时，除了包括HTTP方法，URI路径外，还包括请求实体主体，从而防止PUT和POST请求表示被人篡改。

- 
但是因为nonce本身可以被用来进行摘要认证，所以也无法确保认证后传递过来的数据的安全性。



### requests处理摘要式认证示例




另一种非常流行的 HTTP 身份认证形式是摘要式身份认证，Requests 对它的支持也是开箱即可用的。

![](https://img-blog.csdn.net/20180426214511703?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

- 
可以看到，当认证失败，返回401时，header中包含的信息：

![](https://img-blog.csdn.net/20180426214547779?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





image.png


### 4、OAuth 1 认证




Oauth 是一种常见的 Web API 认证方式。`requests-oauthlib`库可以让 Requests 用户简单地创建 OAuth 认证的请求。
查看OAuth1的原理，可以查看微博的api：http://open.weibo.com/wiki/index.php/Oauth
- 
要使用OAuth验证，首先需要获取到对应的access_token等信息。

- 
官方示例如下：

![](https://img-blog.csdn.net/20180426214608366?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





OAuth1.png


### 5、OAuth 2 与 OpenID 连接认证
- 
OAuth2是OAuth1的升级版，requests-oauthlib 库还可以处理 OAuth 2，OAuth 2 是 OpenID 连接的基础机制。


### 6、requests处理token
- 
每次登录后，返回的token信息都是不同的，因此要再次向服务端传送token信息；

- 
以github接口为例，获得token后，查看用户拥有的权限：




![](https://img-blog.csdn.net/20180426214629712?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

image.png


### 总结
- 
理解基本认证方式和摘要认证方式的原理

- 
OAuth认证是目前大部分api采用的认证方式

- 
token是在服务器端生成的一个令牌，这个令牌有时间期限和权限，就比如皇宫大内中的不同职责的人员拿着令牌能进入的场所不同




