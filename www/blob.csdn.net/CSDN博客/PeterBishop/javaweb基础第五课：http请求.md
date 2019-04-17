# javaweb基础第五课：http请求 - PeterBishop - CSDN博客





2018年12月21日 18:16:10[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：19
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课我们专门来讲http请求



  在第三课我们讲过，浏览器可以给服务器发送http请求，那么这个请求到底是什么，包含了了些信息？



![](https://img-blog.csdnimg.cn/20181221181159879.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



这个就是上节课我们发送的/test请求所包含的请求头(request header)信息



General是常规的信息

Request URL:请求路径

Request Method: 请求方式

Source Code: 请求响应状态

Remote Address: 远程地址(不用管)

Referrer Policy: 不用管



Response Headers是响应头信息

Content-Length: 内容长度

Date: 响应日期



Request Headers 是请求头信息

Accept: 接受类型

Accept-Encoding: 接受编码

Accept-Language: 接受语言

Connection: 连接状态

Cookie: 后面讲

Host: 主机

Referer: 不用管

Upgrade-Insecure-Request: 不用管

User-Agent: 浏览器的信息



form-data  是form表单数据，也就是请求所传递的参数



以下是重点:

Request URL: 请求路径必须和web.xml里servlet-mapping下的url-pattern相同

Request-Method: 请求方式

http请求方式分为get和post两种(当然以后还会讲其他的请求方式，目前先学这两种)

get方式:

该方式的请求会将参数写在url中，如/test?user=1001&psword=123456

该方式的url长度有限制，即所带参数的数量有限制，最长是512字节

该方式的请求安全度较低

该方式的请求效率较高(也就是速度比较快)

Post 方式:

该方式的请求的参数会存于form-data中,不会显示出现在url中

该方式的请求安全度叫高

该方式的请求效率较低



发送get请求的方式:

点击a标签默认是发送get请求

form标签里有个属性叫method,可设置为get来发送get请求



发送post请求的方式:

form标签里有个属性叫method,可设置为post来发送post请求



Source Code:

![](https://img-blog.csdnimg.cn/20181221181159880.png)

![](https://img-blog.csdnimg.cn/20181221181159854.png)

![](https://img-blog.csdnimg.cn/20181221181159853.png)

![](https://img-blog.csdnimg.cn/20181221181159851.png)



![](https://img-blog.csdnimg.cn/20181221181159855.png)

![](https://img-blog.csdnimg.cn/20181221181159877.png)

![](https://img-blog.csdnimg.cn/20181221181159876.png)



