# RESTful Web Service(一):概述 - z69183787的专栏 - CSDN博客
2012年10月27日 12:22:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1126
个人分类：[WebService																[Restful-Api](https://blog.csdn.net/z69183787/article/category/2175699)](https://blog.csdn.net/z69183787/article/category/2175701)
本节介绍sun的RESTful Webservice的实现框架jersey,以及RESTful Webservice的理论知识。做为该系列的第一篇，本节会介绍后续的内容。
### 什么是RESTful Web Services？
RESTful web services是表象状态传输的web服务。REST的基础是适用http创建一个简单的web服务体系架构。它为http数据传输提供了统一的接口。rest有五个关键性原则：
- 所有资源都有唯一的URI：所有的数据都在唯一的URI标识。
- 将所有的事物链接在一起：由于是URI，在任何的可能的情况下，适用链接指向被标识的事物。
- 适用标准方法：同一个资源适用不同的verb可以获得不同的结果，在接下来的例子中可以得到验证。这里的verb除了常用的GET和POST，还有PUT,DELETE。每一个动词刚好对应着数据库的基本操作：GET对应获取数据，就是查询；POST对应的更新数据UPDATE；PUT对应着insert；DELETE就对应这数据库删除。这样的定义不仅能更语义化动词，更重要的是他定义的一种标准，有了这个标准，可以跨应用，跨域，跨系统的调用。
- 资源的多重表述：资源的获取会随着请求的head而不同。
- 无状态通信：REST要求状态要么被放入资源状态中，要么保存在客户端上。换句话说，服务端不能保持除了单词请求之外的，任何与其通信的客户端的通信状态。这样做的理由一是可伸缩性，他可以使服务端的变化对客户端是不可见的。
### jersey介绍
jersey是sun的RESTful web service实现。他使用jdk5的新特征anotation。其参考站点是[https://jersey.dev.java.net/](https://jersey.dev.java.net/)其他的参考资料：
rest介绍：[http://www.infoq.com/cn/articles/rest-introduction](http://www.infoq.com/cn/articles/rest-introduction)
下面几个家伙就是jersey的开发者
■ Earthly Powers, by Paul Sandoz, at[http://blogs.sun.com/sandoz/category/REST](http://blogs.sun.com/sandoz/category/REST).
■ MarcHadley's Blog, at [http://weblogs.java.net/blog/mhadley/](http://weblogs.java.net/blog/mhadley/)
■ Japod's Blog, by Jakub Podlesak, at[http://blogs.sun.com/japod/category/REST](http://blogs.sun.com/japod/category/REST).
