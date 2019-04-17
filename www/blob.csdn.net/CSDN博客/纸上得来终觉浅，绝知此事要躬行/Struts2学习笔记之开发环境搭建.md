# Struts2学习笔记之开发环境搭建 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年07月15日 22:14:31[boonya](https://me.csdn.net/boonya)阅读数：848标签：[struts																[freemarker																[webwork																[web框架																[mvc](https://so.csdn.net/so/search/s.do?q=mvc&t=blog)
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)





Struts2学习笔记 


第一记：Struts2介绍及开发环境搭建

**（1）      Struts2和Struts1的区别：**
      不同点：           Struts2 和 Struts1 从项目上讲他们都是Struts 但从技术上来讲他们是两个完全  不一样的东西。 Struts2是基于 “Xwork”基础上的WEB框架。 XWork是
从webWork 2 中分离出来的一个单独项目。所以Struts2是基于WebWork2的。     共同点：          Struts2和Struts1一样都是MVC框架。


**（2）      Struts2所包含的主要优点特性：**

       >不依赖于Servlet API和Struts API：

            不依赖于Action，解耦合，非侵入式设计。


     >struts2提供了拦截器：

            利用拦截器可以进行AOP编程，实现权限拦截等功能。

     >struts2提供了类型转换器：

            我们可以把特殊的请求参数转成我们需要的类型。

     >struts2支持多种表现层技术：

            视图展现如：jsp,freeMarker,Vclocity

     >struts2输入校验技术：

            可以对指定方法进行校验，解决了struts1之痛。

     >struts2实现不同范围的资源管理：

            提供了全局范围、包范围和Action范围的国际化资源文件管理的实现。



**（3）      Struts2开发环境的搭建：**

       >添加Struts2所需要的必要jar包：

           您可以在这里下载到所需要的Jar包：[http://struts.apache.org/download.cgi#struts234](http://struts.apache.org/download.cgi#struts234)

           主要使用到的包和用途如下：

  struts2-core-2.x.x.jar， Struts2框架的核心类库。

           xwork.jar xwork类库，Struts2在其基础上构建。

           freemarker-2.3.x.jar，  struts2的UI概念的模板使用的FreeMarker编写。

           common-logging.1.1.x.jar，ASF出品的日志包，支持log4j和JDK1.4以上的日志记录。

      >编写Struts2的配置文件：

           在src路径下创建struts.xml，可以从模板中复制粘贴。 如：

 <?xml version="1.0"encoding="UTF-8" ?>

      <!DOCTYPE struts PUBLIC"-//Apache Software Foundation//DTD Struts Configuration 2.1//EN"      "http://struts.apache.org/dtds/struts-2.1.dtd">

        <struts>

                      ….. //<package >..<action>..</action>...<interceptors>…</interceptors>....</package >

        </struts>  

      >在web.xml中添加Struts2 MVC框架启动配置：

 <filter>

                    <filter-name>struts2</filter-name>

                          <filter-class>

                      org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter

                        </filter-class>

        </filter>



      <filter-mapping>

           <filter-name>struts2</filter-name>

          <url-pattern>/*</url-pattern>

      </filter-mapping>](https://so.csdn.net/so/search/s.do?q=web框架&t=blog)](https://so.csdn.net/so/search/s.do?q=webwork&t=blog)](https://so.csdn.net/so/search/s.do?q=freemarker&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




