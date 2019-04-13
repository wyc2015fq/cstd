
# 在android 中发送和解析json ,android处理json字符串 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月19日 18:23:39[Jlins](https://me.csdn.net/dyllove98)阅读数：1198


在我开发的应用程序，我想以JSON对象的形式将消息发送到一个Django服务器和解析来自服务器的JSON响应，并填充自对应的视图列表。
我以前对json懂一点，我想从服务器返回如下这种格式
{
"post": {
"username": "someusername",
"message": "this is a sweet message",
"image": "[http://localhost/someimage.jpg](http://localhost/someimage.jpg)",
"time":  "present time"
},
}
如果想要完成这些功能，我要学习点什么？从哪个地方开始入手？有没有人能提供我一些教程或者一些教程链接？最好有完整解决方案

### 处理方法
你可以从如下两个开源软件来入手完成  gson和 Jackson
Jackson主要使用ObjectMapper对象来进行序列化以及反序列化操作。
使用Jackson可以毫无压力的将复杂对象进行完全序列化，也可以将各种类型的对象反序列化,Jackson 可以完美的将 map list 等对象序列化成json对象..

原文地址：[http://www.itmmd.com/201410/90.html ](http://www.itmmd.com/201410/90.html)
该文章由[萌萌的IT人](http://www.itmmd.com/)整理发布,转载须标明出处。

