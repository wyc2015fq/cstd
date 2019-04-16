# 几句话理解Struts, JSP & Servlet - 沧海一粟 —— 技术随手记 - CSDN博客





2012年02月01日 21:48:28[慢游](https://me.csdn.net/eengel)阅读数：818标签：[struts																[jsp																[servlet																[mvc																[java																[html](https://so.csdn.net/so/search/s.do?q=html&t=blog)
个人分类：[Web Application Framework](https://blog.csdn.net/eengel/article/category/784428)





之前在 [当Struts taglib遇上Java Bean](http://blog.csdn.net/eengel/article/details/6633097) 一文中介绍了Struts和Java Bean之间的一个小问题，这会儿却是来从头回顾一下Struts, JSP和Servlet的相关概念。

去年的这个时候，当时在使用Java开发Web应用之前，还通过 [MVC框架中的模型-视图分离问题](http://blog.csdn.net/eengel/article/details/6182844) 一系列文章分析了MVC框架中的分离相关问题。匆匆一年过去，就当做是对使用Struts的一些总结吧。





程序员都是最懒的一群人，Struts, JSP & Servlet其实也不是很复杂的概念。虽然在之前的 [MVC框架中的模型-视图分离问题（二） —— 问题产生之所在](http://blog.csdn.net/eengel/article/details/6183553) 一文中对JSP和Servlet已经有所介绍，但这儿还是觉得有必要言简意赅地说明Struts, JSP & Servlet都是干什么的。最多再补充些信息。





**Servlet     97年**

一个具有特殊功能的Java类，通常运行于Web服务器端，负责响应HTTP请求并返回相应的web页面。

说明：随着Servlet的产生，Java终于被应用到Web领域了。




**JSP    99年**

一种在HTML等Web页面中结合进Java代码的技术，用于实现Web页面的动态性。

说明：Servlet要产生动态Web页面，需要将HTML代码嵌入到Java代码；而JSP做同样的事情，是将Java代码嵌入到HTML代码。相比较而言，后者更简单，效率更高。




**Struts   00年**

一个基于Servlet的Web应用框架，将以前Servlet和JSP做的事按照MVC的架构进行了有效的组织和分离。

说明：就象 [MVC框架中的模型-视图分离问题（三） —— 分离原动力](http://blog.csdn.net/eengel/article/details/6185199) 中介绍的，Servlet和JSP让两种语言嵌来嵌去，而且Java代码还能肆无忌惮地操作底层业务逻辑和数据库，有诸多弊端。Struts针对这个问题：

1. Struts封装了Servlet，提供了controller的功能，让Servlet更专注于做HTML页面和底层数据库操作之间的桥梁。

2. Struts提供了一组tag，用于替代直接在HTML页面中嵌入的Java代码，并限制了这些tag能做的事情，将业务逻辑隔绝在页面之外。


这样一来，开发人员只要负责写业务逻辑代码，编写HTML页面并用Struts tag实现动态内容，并创建一个`struts-config.xml`文件将MVC三者联系起来就可以了。




参考文献：

1. http://en.wikipedia.org/wiki/Servlet

2. http://en.wikipedia.org/wiki/JavaServer_Pages

3. http://en.wikipedia.org/wiki/Struts](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=mvc&t=blog)](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




