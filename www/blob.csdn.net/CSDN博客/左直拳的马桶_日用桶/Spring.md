# Spring - 左直拳的马桶_日用桶 - CSDN博客
2018年12月13日 11:59:52[左直拳](https://me.csdn.net/leftfist)阅读数：82
个人分类：[系统架构																[学习笔记																[Java](https://blog.csdn.net/leftfist/article/category/2412493)](https://blog.csdn.net/leftfist/article/category/1790155)](https://blog.csdn.net/leftfist/article/category/2567173)
Spring，Java世界里的基本概念，绕不过去的。但它只是一个框架而已。为啥叫Spring呢？春天？弹簧？估计是弹簧。叫春总不大好吧，这么学术性的场合。
然后Spring身后又跟着一大串，SpringMVC，Spring Boot，Spring Cloud。
我的感悟：
```
Spring解决了bean的实例化问题，是基础；
SpringMVC基于Spring的MVC框架；
SpringBoot是Spring的简化，利用注解代替繁冗的XML配置；
Spring Cloud，微服务框架；
```
**一、Spring**
Spring主要解决了Bean的实例化和管理工作。
Bean是啥？是业务逻辑处理对象。
原先在没有Spring之前，java的Web开发经过了一系列的发展过程：
**1、jsp包揽一切**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213104029222.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**2、加入Servlet**
觉得JSP大包大揽，他总是耦合太深，所以分了一些给servlet
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121310503115.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**3、3层**
后来又将JavaBean再一次进行分割，分为业务逻辑、数据持久化
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213105550673.png)
耦合程度越来越低了。但问题是，面向接口编程模式中，要构造非常多的实例，很不方便。这时Spring应运而生，它采用IoC容器来装配、管理各种Bean。
**4、Spring架构**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213105911348.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
其中核心组件是IoC容器，又分为：Context，Beans，Core。可以看下图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213112827591.gif)
参考文章：
[Spring是怎样诞生的？](https://blog.csdn.net/g6U8W7p06dCO99fQ3/article/details/79085158)
[Spring 框架的设计理念与设计模式分析](https://www.ibm.com/developerworks/cn/java/j-lo-spring-principle/index.html)
**二、SpringMVC**
Spring和SpringMVC本质就是两个容器：Spring是根容器，SpringMVC是其子容器。Spring解决的是Bean的治理，而SpringMVC则在此基础上提供MVC架构。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121311385773.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
在Spring MVC框架中，
从“Request（请求）”开始，
依次进入“DispatcherServlet（核心分发器）”
—> “HandlerMapping（处理器映射）”
—> “Controller（控制器）”
—> “ModelAndView（模型和视图）”
—> “ViewResolver（视图解析器）”
—> “View（视图）”
—> “Response（响应）”结束，
其中DispatcherServlet、HandlerMapping和ViewResolver 只需要在XML文件中配置即可，从而大大提高了开发的效率，特别是对于 HandlerMapping 框架为其提供了默认的配置。
参考文章：
[简述Spring容器与SpringMVC的容器的联系与区别](https://blog.csdn.net/justloveyou_/article/details/74295728)
[史上最简单的 Spring MVC 教程（一）](https://blog.csdn.net/qq_35246620/article/details/54704656)
**三、Spring Boot**
Spring用了很多配置，而且各种依赖很多，大多数初级人员在搭建项目时花费了很多时间也未必能搭建明白。Spring Boot提倡“约定大于配置”，把以前的手动配置的过程自动化封装了，提供默认的配置，借用大牛总结的特点就是：
*简单易用，初学者和大牛都可以轻松上手，其中的注解会给使用者提供方便；Spring boot对第三方技术进行了很好的封装和整合，提供了大量第三方接口；可以通过依赖自动配置，不需要XML等配置文件还提供了安全等特性*
总之就是可以快速启动，快速构建项目，在配置文件环节省去大量时间和精力，傻瓜式带你进入项目大门，进行业务逻辑编写。现在跟很多框架整合了 按照规范去配置 分分钟上手写代码。
![在这里插入图片描述]([https://img-blog.csdnimg.cn/20181213115603555.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70](https://img-blog.csdnimg.cn/20181213115603555.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
参考文章：
[Spring、SpringMVC、Spring Boot、Spring Cloud 概念、关系及区别](https://my.oschina.net/u/3959200/blog/2050220)
**四、Spring Cloud**
就是一个微服务架构。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213115727296.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
