
# SpringMVC源码分析系列[转] - wangzhiguo9261的博客 - CSDN博客


2018年03月05日 15:36:31[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：59个人分类：[Spring](https://blog.csdn.net/wangzhiguo9261/article/category/7483196)


说到java的mvc框架，struts2和springmvc想必大家都知道，struts2的设计基本上完全脱离了Servlet容器，而springmvc是依托着Servlet容器元素来设计的，同时springmvc基于Spring框架，Spring框架想必搞java的同学都很熟悉。
![](http://images.cnitblog.com/i/411512/201406/252357308306593.png)
一进Spring的[官网](http://spring.io/)就发现了这样一排醒目的文字， spring可以让我们构造简单的、便携的、又快又易于扩展的基于jvm的系统和应用程序。
没错，基于Spring的MVC框架SpringMVC同样也可以构造具有这些特性的系统。

楼主从5.1开始写的第一篇《SpringMVC入门》开始，到现在差不多将近2个月的时间，之后陆陆续续写了几篇关于SpringMVC的源码分析文章。
有同学想研究SpringMVC源码或想深入学习一下SpringMVC的话，可以参考本系列文章。
废话不多说，本系列的文章都是基于Spring4.0.2版本的，楼主写的时候出到了4.0.2，也是最新的，现在的最新版本是4.0.5。
文章阅读顺序：
1.[SpringMVC入门](http://www.cnblogs.com/fangjian0423/p/springMVC-introduction.html)
SpringMVC的入门文章， 对于某些没接触过SpringMVC的同学来说，可以阅读以下，了解这个框架的结构以及使用，以入门的同学可以选择不看~
2.[SpringMVC核心分发器DispatcherServlet分析](http://www.cnblogs.com/fangjian0423/p/springMVC-dispatcherServlet.html)
SpringMVC入口Servlet -> DispatcherServlet的分析，解释了DispatcherServlet的父类FrameworkServlet以及FrameworkServlet的父类HttpServletBean的作用
3.[详解SpringMVC请求的时候是如何找到正确的Controller](http://www.cnblogs.com/fangjian0423/p/springMVC-request-mapping.html)
想知道http请求是如何找到对应Controller中的方法的吗，这个过程当中到底发生了什么，你知道吗？ 本篇将为你一一解答。
4.[详解SpringMVC中Controller的方法中参数的工作原理](http://www.cnblogs.com/fangjian0423/p/springMVC-request-param-analysis.html)
同学们有想过http请求找到对应Controller中的方法之后，方法中的参数是如何被注入进来的吗？比如：
```python
public
```
```python
String
```
```python
method
```
```python
(HttpServletRequest request, String name) {
         ...
        }
```
这里的HttpServletRequest类型的request参数以及String类型的name参数是如何自动被注入到method方法中的？  主要是两个重要的接口的作用：HandlerMethodArgumentResolver(请求参数处理)和HandlerMethodReturnValueHandler(返回值处理)。文中还写了一个自定义的HandlerMethodArgumentResolver接口实现类FormObjArgumentResolver，该类处理Controller方法中多个自定义类参数的处理(建议看完类型转换文章后再看这个类的具体实现代码)。
5.[SpringMVC关于json、xml自动转换的原理研究](http://www.cnblogs.com/fangjian0423/p/springMVC-xml-json-convert.html)
通过json、xml的自动转换巩固第四篇文章的知识，自动转换由RequestResponseBodyMethodProcessor处理，该类实现了HandlerMethodArgumentResolver和HandlerMethodReturnValueHandler接口。
6.[SpringMVC类型转换、数据绑定详解](http://www.cnblogs.com/fangjian0423/p/springMVC-databind-typeconvert.html)
介绍了属性编辑器的概念以及Spring对属性编辑器的支持，本文知识消化之后可以回过头看第4篇中FormObjArgumentResolver的实现。
7.[SpringMVC拦截器详解](http://www.cnblogs.com/fangjian0423/p/springMVC-interceptor.html)
解释了SpringMVC拦截器的设计原理。
8.[SpringMVC视图机制详解](http://www.cnblogs.com/fangjian0423/p/springMVC-view-viewResolver.html)
分析了SpringMVC的视图机制，主要也就是讲解View和ViewResolver这两个接口的作用。写完之后还写了篇关于重定向视图RedirectView的小吐槽：[SpringMVC重定向视图RedirectView小分析](http://www.cnblogs.com/fangjian0423/p/springMVC-redirectView-analysis.html)。
9.[SpringMVC异常处理机制详解](http://www.cnblogs.com/fangjian0423/p/springMVC-exception-analysis.html)
解释了SpringMVC异常机制的设计原理。
额外知识：
[Spring中Ordered接口简介](http://www.cnblogs.com/fangjian0423/p/spring-Ordered-interface.html)
有同学想了解本系列没讲到的知识的话，如果觉得合适，楼主会继续写关于这些内容的博客的。
**目前这些博客已经放到了github上：**[https://github.com/fangjian0423/springmvc-source-minibook](https://github.com/fangjian0423/springmvc-source-minibook)
**也可以在线阅读，或者下载pdf，moni，epub：**[https://www.gitbook.com/book/fangjian0423/springmvc-source-minibook/details](https://www.gitbook.com/book/fangjian0423/springmvc-source-minibook/details)
最后附上Spring4.0.2官方文档：[http://docs.spring.io/spring/docs/4.0.2.RELEASE/spring-framework-reference/htmlsingle/](http://docs.spring.io/spring/docs/4.0.2.RELEASE/spring-framework-reference/htmlsingle/)


