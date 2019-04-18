# 关于java web中的解码与编码 - z69183787的专栏 - CSDN博客
2014年02月10日 20:16:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：965
个人分类：[编码处理](https://blog.csdn.net/z69183787/article/category/2176995)
一.html表单，用get和post提交信息到servlet
  1.<meta content="text/html; charset=ISO-8859-1>这个html标签里面的属性的是告诉浏览器的编码。个人觉得可以这样理解，你编写一个html文件，然后你在body里面输入了中文。这里的charset的值就是表明html制作使用的语言。那么当浏览器要显示这个html页面的时候，就知道这个页面是用什么字符显示了！就不会出现乱码了！还有一个作用就是页面编码！
   比如这里设置为 charset=ISO-8859-1，那么在编辑这个页面的时候就不能输入中文了。如果这里设置了Big-5，编辑这个页面的时候就不能使用简体中文了！
2.这里面请求的过程主要可以归纳为以下：客户端网页发送一个请求，浏览器将自动根据这个页面所使用的字符集来对汉字进行编码之后（这里就要用到上面说的,meta的页面编码了），传输到服务器。然后服务器通过字符集来解码。然后服务器端再把所需要返回的信息通过字符集编码，再传到客户端的浏览器，浏览器进行解码！这里涉及到了2次解码与编码。
3.get方法请求页面
get方法请求信息的时候是把form表单的信息直接放到Url地址里面的，具体的体现就是在request信息中体现在Query String Parameters中。
![](http://images.cnitblog.com/blog/534921/201307/02225733-94dea4eca357463ea8ea304c67ed6998.png)
①传输的中文信息，通过页面编码格式进行URL编码（也就是meta里面的charset的编码格式）传输到服务端
URL编码规则：
URL编码遵循下列规则： 每对name/value由&；符分开；每对来自[表单](http://baike.baidu.com/view/296684.htm)的name/value由=符分开。如果用户没有输入值给这个name，那么这个name还是出现，只是无值。任何特殊的[字符](http://baike.baidu.com/view/263416.htm)（就是那些不是简单的七位ASCII，如汉字）将以百分符%用[十六进制编码](http://baike.baidu.com/view/8420388.htm)，当然也包括象
 =,&；，和 % 这些特殊的字符。其实url编码就是一个字符ascii码的十六进制。不过稍微有些变动，需要在前面加上“%”。[比如](http://baike.baidu.com/view/6814120.htm)“\”，它的ascii码是92，92的十六进制是5c，所以“\”的url编码就是%5c。那么汉字的url编码呢？很简单，看例子：“胡”的ascii码是-17670，十六进制是BAFA，url编码是“%BA%FA”。
②
String xx = request.getParameter("xx"); 
服务端通过字符集来进行解码，而tomcat用的默认字符集是ISO-8859-1，所以如果这样子对所传过来的十六进制码进行解码的肯定是不对的（因为解码和编码必须对应可以参考[http://wenku.baidu.com/view/14a91366caaedd3383c4d331.html](http://wenku.baidu.com/view/14a91366caaedd3383c4d331.html)）。如果现在直接在控制台输出xx的话得到的将会是乱码。那么该怎么才能获得我们需要的数据呢？
可以这样子
String xx = new String(request.getParameter("text").getBytes("iso-8859-1"),"aa")  （aa为html的页面编码格式）。这样在控制台打出就不会出现乱码了！（注意这里用request.setCharacterEncoding这个函数是不起作用的。）
每次这么弄也麻烦，那么就可以通过改变tomcat的这个默认的字符集编码。可以通过修改server.xml文件中的下面的语句，加上URIEncoding=“xx“（这里的xx为常用的html的页面编码格式）的配置即可：<Connector port="8080" protocol="HTTP/1.1"  connectionTimeout="20000"  redirectPort="8443" URIEncoding="GB18030"/>。
③.服务器把需要传输的信息通过编码传到客户端（浏览器），浏览器通过解码来获取信息！
这里又涉及到一个编码和解码，而在服务器端则默认使用iso-8859-1进行编码成byte，等到了浏览器，发现response的信息header中没有相关编码设置，就会去取window系统的编码格式，中文系统默认为GBK/GB2312。所以，打印出来的页面的浏览器编码格式为GB2312。而如果设置了response的编码格式，那么就算到了浏览器，浏览器解析也会按照设置的字符集解码。这里设置的方法可以有setContentType（）和setCharacterEncoding（）。response.setCharacterEncoding设置HTTP
 响应的编码,如果之前使用response.setContentType设置了编码格式,则使用response.setCharacterEncoding指定的编码格式覆盖之前的设置。
4.post方法请求页面
POST把提交的数据则放置在是HTTP包的包体中具体体现就是request信息中体现在Form Data中。
![](http://images.cnitblog.com/blog/534921/201307/03003005-152e0a83ae844b4dbb99a07273fce208.png)
①传输的中文信息，通过页面编码格式进行URL编码（也就是meta里面的charset的编码格式）传输到服务端
URL编码规则：
URL编码遵循下列规则： 每对name/value由&；符分开；每对来自[表单](http://baike.baidu.com/view/296684.htm)的name/value由=符分开。如果用户没有输入值给这个name，那么这个name还是出现，只是无值。任何特殊的[字符](http://baike.baidu.com/view/263416.htm)（就是那些不是简单的七位ASCII，如汉字）将以百分符%用[十六进制编码](http://baike.baidu.com/view/8420388.htm)，当然也包括象
 =,&；，和 % 这些特殊的字符。其实url编码就是一个字符ascii码的十六进制。不过稍微有些变动，需要在前面加上“%”。[比如](http://baike.baidu.com/view/6814120.htm)“\”，它的ascii码是92，92的十六进制是5c，所以“\”的url编码就是%5c。那么汉字的url编码呢？很简单，看例子：“胡”的ascii码是-17670，十六进制是BAFA，url编码是“%BA%FA”。
②
String xx = request.getParameter("xx"); 
服务端通过字符集来进行解码，而tomcat用的默认字符集是ISO-8859-1，所以如果这样子对所传过来的十六进制码进行解码的肯定是不对的（因为解码和编码必须对应可以参考[http://wenku.baidu.com/view/14a91366caaedd3383c4d331.html](http://wenku.baidu.com/view/14a91366caaedd3383c4d331.html)）。如果现在直接在控制台输出xx的话得到的将会是乱码。那么该怎么才能获得我们需要的数据呢？
可以这样子
String xx = new String(request.getParameter("text").getBytes("iso-8859-1"),"aa")  （aa为html的页面编码格式）。这样在控制台打出就不会出现乱码了！
这里也可以用request.setCharacterEncoding（“aa”）来设置编码格式！（这里修改server.xml就没有用了）
③.服务器把需要传输的信息通过编码传到客户端（浏览器），浏览器通过解码来获取信息！
这里又涉及到一个编码和解码，而在服务器端则默认使用iso-8859-1进行编码成byte，等到了浏览器，发现response的信息header中没有相关编码设置，就会去取window系统的编码格式，中文系统默认为GBK/GB2312。所以，打印出来的页面的浏览器编码格式为GB2312。而如果设置了response的编码格式，那么就算到了浏览器，浏览器解析也会按照设置的字符集解码。这里设置的方法可以有setContentType（）和setCharacterEncoding（）。response.setCharacterEncoding设置HTTP
 响应的编码,如果之前使用response.setContentType设置了编码格式,则使用response.setCharacterEncoding指定的编码格式覆盖之前的设置。
二.jsp作为客户端，用get和post提交信息到servlet
基本上和html的Get和Post的是一样的！但是有一个地方不同！那就是
jps的页面有3处地方涉及到编码与解码：1.pageEncoding 2.contentType 3.meta标签
那么作为客户端提交form表单的话，页面的编码的遵循顺序是contentType>pageEncoding,即如果contentType设置过了的话就用contentType，如果没有设置的话就用pageEncoding。meta标签暂时没有发现在这个过程中起到了什么作用！
三.jsp作为服务端处理信息。
当接受到正确的信息以后jsp服务端，就会对数据进行编码，然后传到浏览器，浏览器进行解码！那么这里面的编码和解码规则是怎么样呢？默认当然是iso-8859-1，和servlet是一样的，可以用setCharacterEncoding（）方法，也可以用pageEncoding和contentType ，但是这里和html中不同的是setContentType（）方法失效！并且setCharacterEncoding（）>contentType>pageEncoding.
     上述只是个人实验的结果，其中还有很多东西都没有弄懂原理，但是个人觉得知道了这些，在web编程里面一般的中文乱码就不会出现！如果有什么错误请指正,如果有什么补充和心得请留言，大家共同进步！不胜感激！
