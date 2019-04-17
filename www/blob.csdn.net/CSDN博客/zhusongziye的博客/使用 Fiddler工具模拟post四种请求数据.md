# 使用 Fiddler工具模拟post四种请求数据 - zhusongziye的博客 - CSDN博客





2018年11月22日 21:35:51[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：170








## 前言：

Fiddler是一个简单的http协议调试代理工具，它界面友好，易于操作，是模拟http请求的利器之一。

在接口测试中，接口通常是get请求或者post请求。get请求的测试一般较为简单，只需设置好相关的请求头，url写正确即可。但是在测试post请求时，请求数据格式的设置往往就稍显复杂。尤其是在开发人员的接口文档描述不清楚的情况下，会影响到测试效率。

故而本文总结了下post请求常见的四种数据格式和对应的fiddler模拟请求的构造方法。

# post请求主体详解：

对于get请求来说没有请求主体entity-body。对于post请求而言，不会对发送请求的数据格式进行限制，理论上你可以发任意数据，但是服务器能不能处理就是另一回事了。服务器收到数据后，如何解析数据呢？

它会以请求头中的Content-Type设置的内容来进行数据解析。确定好Content-Type的格式之后，请求主体的数据格式也就确定下来了。

Content-Type的格式有四种：分别是application/x-www-form-urlencoded（这也是默认格式）、application/json、text/xml以及multipart/form-data格式。

接下来就详细地介绍每一种数据格式对应的fiddler请求模拟的实现情况。

# 四种Post请求数据格式：

（一）application/x-www-form-urlencoded数据格式

这是post请求最常见也是默认的数据提交格式。它要求数据名称（name）和数据值（value）之间以等号相连，与另一组name/value值之间用&相连。例如：parameter1=12345&meter2=23456。将请求的内容进行格式化了，其实这个方法同时简化的客户端发送，也简化了服务器端获取，服务器通过getParameters（String name）即可获取到传送来的信息。

用fiddler模拟请求的话，请求头和请求主体的内容可以这样构造：

![](https://img-blog.csdnimg.cn/2018112221322633.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

模拟请求之后，从返回结果可以查看到我们的请求数据：



![](https://img-blog.csdnimg.cn/20181122213242266.png)

（二）application/json数据格式



application/json格式的请求头是指用来告诉服务端post过去的消息主体是序列化后的 JSON 字符串。

用fiddler模拟请求的话，请求头和请求主体的内容可以这样构造：

![](https://img-blog.csdnimg.cn/20181122213253279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

模拟请求之后，从返回结果可以看到我们的请求数据：

![](https://img-blog.csdnimg.cn/20181122213344645.png)



（三）text/xml数据格式

用fiddler模拟请求的话，请求头和请求主体的内容可以这样构造：

![](https://img-blog.csdnimg.cn/20181122213359396.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

模拟请求之后，从返回结果可以看到我们的请求数据：



![](https://img-blog.csdnimg.cn/20181122213416940.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

（四）multipart/form-data数据格式

除了传统的application/x-www-form-urlencoded表单，我们另一个经常用到的是上传文件用的表单，这种表单的类型为multipart/form-data。

用fiddler模拟请求的话，请求头和请求主体的内容可以这样构造：

第一步，先设置好请求头格式，然后点击upload file...

![](https://img-blog.csdnimg.cn/2018112221343812.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

第二步，上传你的文件，这里我上传一个png的图片



![](https://img-blog.csdnimg.cn/20181122213447481.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

这是fiddler根据我们上传的文件自动调整生成的请求，在请求头中看到，我们需要选择一段数据作为“分割边界”（boundary属性），这个“边界数据”不能在内容其他地方出现，一般来说使用一段从概率上说“几乎不可能”的数据即可。

每次post浏览器都会生成一个随机的30-40位长度的随机字符串，浏览器一般不会遍历这次post的所有数据找到一个不可能出现在数据中的字符串，一般都是随机生成。选择了这个边界之后，浏览器便把它放在Content-Type 里面传递给服务器，服务器根据此边界解析数据。

下面的数据便根据boundary划分段，每一段便是一项数据。(每个field被分成小部分，而且包含一个value是"form-data"的"Content-Disposition"的头部；一个"name"属性对应field的ID等等，文件的话包括一个filename)

模拟请求之后，从返回结果可以看到我们的请求数据：

![](https://img-blog.csdnimg.cn/20181122213511931.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



图文来源网络，如有侵权联系删除



