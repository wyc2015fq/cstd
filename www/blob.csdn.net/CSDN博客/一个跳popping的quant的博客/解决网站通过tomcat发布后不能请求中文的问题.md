# 解决网站通过tomcat发布后不能请求中文的问题 - 一个跳popping的quant的博客 - CSDN博客





2017年10月20日 15:58:12[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1848








之前通过JFinal的框架，给学院写了一个学生的管理系统，今天在上传了学生信息之后，发现搜索的功能不能使用中文作为搜索的关键字，问了别人一下，可能是因为tomcat没用配置中文的编码，于是就百度了一波，找到了这篇文章，成功解决了问题。[http://blog.csdn.net/u010391029/article/details/47082733](http://blog.csdn.net/u010391029/article/details/47082733)

   1.首先我们看表单get方式提交 

   浏览器根据页面的charset编码方式对页面进行编码，然后提交至服务器，首先进入对应的字符编码过滤器(如果有的话)，不过Tomcat6.0对于get提交方式采用的是server.xml文件中的URIEncoding编码方式，而并不会采用过滤器中设置的编码，那么根据我的环境设置，jsp页面都使用UTF-8的编码，Servlet容器的URIEncoding也设置为UTF-8，则servlet不用进行转码即可正确解码，获得正常的中文字符串。那么，响应页面的中文因为页面的统一编码(UTF-8)自然也会正常显示。当然，如果我们Tomcat的URIEncoding设置为其他非UTF-8的编码方式时，页面的内容进入Tomcat解析时，因为Tomcat和页面的编码不统一，就需要转码
》》》》》修改tomcat默认编码格式：

修改tomcat下的conf/server.xml文件，找到如下代码：

```xml
<Connector port="8080" protocol="HTTP/1.1"
               connectionTimeout="20000"
               redirectPort="8443" />
```

这段代码规定了Tomcat监听HTTP请求的端口号等信息。可以在这里添加一个属性：URIEncoding，将该属性值设置为UTF-8，即可让Tomcat（默认ISO-8859-1编码）以UTF-8的编码处理get请求。更改后的代码如下所示： 

```xml
<Connector port="8080" protocol="HTTP/1.1"
               connectionTimeout="20000"
              URIEncoding="UTF-8"
               redirectPort="8443" />
```

以上，本人虽然没有遇到过这种乱码问题，但是还是在网上查了一下，导致中文乱码的几种情况，首先看下字符编码有哪几种呢？

1、ASCII字码

2、IS0-8859-1

3、GB2312和GBK

4、Unicode编码

5、UTF-8

导致中文乱码的可能情况：

1、表单的get提交

2、表单的post提交

3、页面链接传递中文参数

4、地址栏中参数直接输入中文提交

对于get方式提交的乱码，可以通过修改tomcat配置文件来修改，对于post方式提交的乱码可以通过设置编码，request.setCharacterEncoding(“UTF-8”)方法进行编码设置

以上，由于本人没有遇到过这类问题，网络搜集，仅供参考

2.表单的post提交

对于这种方式的请求，request.setCharacterEncoding(“一般来自于web.xml中过滤器设置的参数”)方法进行编码设置将会产生作用，struts的表单提交方式默认为post方式，那么按照上面我的环境设置，页面，容器，都采用UTF-8编码方式，就不会产生中文乱码问题。

添加过滤器，调用request.setCharacterEncoding(“utf-8”)方法将request的字符集设定为utf-8，解决请求以post方式提交的乱码问题。




