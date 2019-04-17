# 接口测试 requests中Cookie和会话对象session处理 - zhusongziye的博客 - CSDN博客





2018年04月20日 21:52:36[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：3555








要知道为什么会有cookie和session，需要先理解http的特点：http协议是无状态的协议。因此，cookie和session存在的作用是进行状态管理。

### Cookie和Session的简单理解

我从哪里来，我在哪里，我到哪里去
- 
Cookie是由服务端生成，存储在响应头中，返回给客户端，客户端会将cookie存储下来




- 
Session是由服务端生成，存储在服务器端的内存、缓存、数据库等地方




- 
在客户端发送请求时，user-agent会自动获取本地存储的cookie，将cookie信息存储在请求头中，发送给服务端




- 
请求都是由客户端发起的，当服务端生成了session，客户端怎么会知道呢？客户端怎么能对上这个session暗号？


1）、在客户端给服务端发送请求后，服务端会根据请求信息生成session，同时生成一个session_id，通过cookie返回给客户端；


2）、客户端再次向服务端发送请求时，会通过cookie将这个session_id发送给服务端，这样就对上了session的暗号。




- 
cookie可以作为管理session的一种方式，当cookie被禁用时，可以通过表单或重写url传送session_id








### requests实现cookie操作



- 
cookie包含在响应中，要获取响应中的cookie信息，可以用`r.cookies.get_dict()`获取到响应的cookies信息，并传递给后续请求。




- 
找一个不会被进行验证的网站进行练习，从输出信息中，可以看到，第2个请求使用了第一个请求响应头中的cookies信息，保持了连接




![](https://img-blog.csdn.net/2018042021515251?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


cookies.png




- 
在已知cookie信息时，需要构造成对应的cookie对象传递给请求时，可以采用Cookie 的返回对象为RequestsCookieJar或者采用字典构造。




- 
RequestsCookieJar构造cookies是已知登录响应返回的cookies信息，通过创建对象，设置对应的值进行构造传递。




![](https://img-blog.csdn.net/20180420215205575?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


RequestsCookieJar.png




- 
采用字典构造cookies信息，将构造方式更改为：


```
# 构造cookies

cookies = dict(zentaosid = '5qm86pvshjm3s8u729cmc0mb15')
```

### 会话对象session

会话对象让你能够跨请求保持某些参数。它也会在同一个 Session 实例发出的所有请求之间保持 cookie。所以如果你向同一主机发送多个请求，底层的 TCP 连接将会被重用，从而带来显著的性能提升。



- 
session会自动管理cookie，一个session对象会保持同一个会话中的所有请求之间的cookie信息。





![](https://img-blog.csdn.net/20180420215220105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


session.png

### 总结
- 
会话对象session比cookies更方便管理请求的cookie信息




- 
服务器返回的cookie信息通常在响应头中




- 
如果需要将响应体中的部分信息传递给后续请求的cookie，可以构造cookie信息并传递

- 






