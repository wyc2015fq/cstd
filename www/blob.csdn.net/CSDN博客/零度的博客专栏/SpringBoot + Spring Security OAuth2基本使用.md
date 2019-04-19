# SpringBoot + Spring Security OAuth2基本使用 - 零度的博客专栏 - CSDN博客
2018年08月23日 17:53:29[零度anngle](https://me.csdn.net/zmx729618)阅读数：775
原文出处：[https://blog.csdn.net/u013435893/article/details/79735097](https://blog.csdn.net/u013435893/article/details/79735097)
# OAuth2.0基本知识
网上关于OAuth2.0的介绍已经很多了，这里就不做过多的介绍，不太了解的朋友可以参考[理解OAuth 2.0](http://www.ruanyifeng.com/blog/2014/05/oauth_2_0.html)
# Spring Security OAuth2
## 基本配置
这里依然使用maven来做管理
```xml
<dependency>
    <groupId>org.springframework.security.oauth</groupId>
    <artifactId>spring-security-oauth2</artifactId>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-security</artifactId>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```
## 认证服务器
### Authorization Server
在过OAuth2.0有了基本概念后，我们会知道其中有一个服务提供商，我们就先来完成它。
这里只需要新建一个类，并添加相应的注释就可以了
```java
@Configuration
@EnableAuthorizationServer
public class MyAuthorizationServerConfig {
}
```
随后我们启动项目，就会发现控制台有如下的语句打印：
![Authorization Server](https://img-blog.csdn.net/20180327220956785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM0MzU4OTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这就表明Authorization Server已经建立起来了。 我们可根据OAuth的规则来访问相应的接口。
### 第三方应用 User authenticates
在有了服务提供商之后，我们就可以根据OAuth的规则，来要求用户给予授权。 这里我们以code模式为例。
所以这里需要第三方应用去调用接口
> 
[http://localhost:8080/oauth/authorize?response_type=code&client_id=3aa1f466-c67d-4f72-a8a8-62ed94d7d638&redirect_uri=http://www.baidu.com&scope=all](http://localhost:8080/oauth/authorize?response_type=code&client_id=3aa1f466-c67d-4f72-a8a8-62ed94d7d638&redirect_uri=http://www.baidu.com&scope=all)
这里对接口参数做一个简单的介绍。
- localhost:8080这里是我服务的地址以及端口，根据每个人的情况是不同的
- /oauth/authorize这个是Spring Security OAuth2默认提供的接口
- response_type：表示授权类型，必选项，此处的值固定为”code”
- client_id：表示客户端的ID，必选项。这里使用的是项目启动时，控制台输出的security.oauth2.client.clientId，当然该值可以在配置文件中自定义
- redirect_uri：表示重定向URI，可选项。即用户授权成功后，会跳转的地方，通常是第三方应用自己的地址
- scope：表示申请的权限范围，可选项。这一项用于服务提供商区分提供哪些服务数据
- state：表示客户端的当前状态，可以指定任意值，认证服务器会原封不动地返回这个值。这里没有使用到该值
这里我们访问到接口后，会出现如下的界面
![用户登录](https://img-blog.csdn.net/20180328211747412?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM0MzU4OTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
该界面主要是用于用户登录的，不然怎么知道想要哪个用户的数据呢？
在登录成功后，来到如下界面
![授权界面](https://img-blog.csdn.net/20180328211835948?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM0MzU4OTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这里就是要求用户授权的界面了，有点类似于我们使用QQ进行第三方登录时候的界面。上面写有了是哪一个第三方应用需要哪些数据。
我们这里就点确认授权，这里就会根据配置的`redirect_uri`进行跳转，并且是带有一个参数的。
这里我们跳转到了:[https://www.baidu.com/?code=XKxYIx](https://www.baidu.com/?code=XKxYIx)。
这个code就是下一步第三方应用向服务器申请令牌使用的
### 请求Token
这里我们拿着上一步获取到的code，以及项目初始化时打印的clientId和secret去获取Token。
这里需要使用POST方法，
> 
POST /oauth/token HTTP/1.1
Host: localhost:8082
Authorization: Basic czZCaGRSa3F0MzpnWDFmQmF0M2JW
Content-Type: application/x-www-form-urlencoded
```
grant_type=authorization_code&code=SplxlOBeZQQYbYS6WxSbIA
&redirect_uri=https%3A%2F%2Fclient%2Eexample%2Ecom%2Fcb
```
请求的Header中有一个Authorization参数，该参数的值是Basic + （clientId:secret Base64值）
- grant_type：表示使用的授权模式，必选项，此处的值固定为”authorization_code”。
- code：表示上一步获得的授权码，必选项。
- redirect_uri：表示重定向URI，必选项，且必须与A步骤中的该参数值保持一致。
- client_id：表示客户端ID，必选项。
如果请求成功，就可以顺利的拿到Token
### 获取到Token
请求Token成功后，认证服务器发送的HTTP回复
> 
HTTP/1.1 200 OK
Content-Type: application/json;charset=UTF-8
Cache-Control: no-store
Pragma: no-cache
```
{
   "access_token":"2YotnFZFEjr1zCsicMWpAA",
   "token_type":"example",
   "expires_in":3600,
   "refresh_token":"tGzv3JOkF0XG5Qx2TlKWIA",
   "example_parameter":"example_value"
 }
```
- access_token：表示访问令牌，必选项。
- token_type：表示令牌类型，该值大小写不敏感，必选项，可以是bearer类型或mac类型。
- expires_in：表示过期时间，单位为秒。如果省略该参数，必须其他方式设置过期时间。
- refresh_token：表示更新令牌，用来获取下一次的访问令牌，可选项。
- scope：表示权限范围，如果与客户端申请的范围一致，此项可省略。
## 资源服务器
和认证服务器一样，这里实现资源服务器就很容易了
```java
@Configuration
@EnableResourceServer
public class MyResourceServerConfig {
}
```
这样我们就可以用Token来访问接口了。
例如：
> 
GET /user HTTP/1.1
Host: localhost:8082
Authorization: bearer 9b2aaea4-d161-4636-8883-6756a372e735
这里Authorization中，bearer 是上一步返回的token_type。
# 遗留问题
目前基本功能是实现了，但是还有两个遗留问题需要解决：
1. 现在的Token是存在Session中的，服务器重启后原来客户端的Token就失效了。
2. Token现在是自动生成的，是否可以用JWT来自定义生成呢？
# 代码下载
[Spring-Security](https://github.com/whyalwaysmea/Spring-Security)
