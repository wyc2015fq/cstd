# Ubuntu下eclipse indigo版在线安装struts插件 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年10月09日 10:00:13[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2975标签：[eclipse																[struts																[ubuntu																[mvc																[webapp																[encoding](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)
个人分类：[Linux](https://blog.csdn.net/fjssharpsword/article/category/6480442)





                
Ubuntu下eclipse indigo版在线安装struts插件：


1.打开eclipse，进入Help->Install new software...；


2.添加新站点，命名为：mvc，地址：http://mvcwebproject.sourceforge.net/update/；


3.添加站点后搜索到列表中有：Alveole Studio MVC Web Project项，打勾选中，next下去执行在线安装，根据网速决定安装进度，完成后重启eclipse；


4.新建项目：

  1)先创建一个Dynamic Web Project，命名为fweb；

  2)进入file->new->other...->web选中Alveole Studio MVC Web Project后next；

  3)在Enter or select the parent folder里选择列表里的fweb；

  4)file name输入：struts.aswp，然后next直到finish，fweb项目自动生成struts代码框架；

  5)web.xml配置：

======================================================================

<?xml version="1.0" encoding="UTF-8"?>

<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://java.sun.com/xml/ns/javaee" xmlns:web="http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd" xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
 id="WebApp_ID" version="2.5">

  <display-name>fweb</display-name>

  <filter>

      <filter-name>org.apache.struts2.dispatcher.FilterDispatcher</filter-name>

      <filter-class>org.apache.struts2.dispatcher.FilterDispatcher</filter-class>

  </filter>

  <filter-mapping>

      <filter-name>org.apache.struts2.dispatcher.FilterDispatcher</filter-name>

      <url-pattern>*.action</url-pattern>

  </filter-mapping>

  <welcome-file-list>

    <welcome-file>index.jsp</welcome-file>

  </welcome-file-list>

</web-app>

============================================================================

  6)struts.aswp图形化配置，建立MVC关系；


5.具体操作教程参考地址：

  http://wenku.baidu.com/view/fe540f175f0e7cd184253640.html

  http://mvcwebproject.sourceforge.net/tutorial.html

  http://mvcwebproject.sourceforge.net/tutorials/HelloWorld2.zip](https://so.csdn.net/so/search/s.do?q=webapp&t=blog)](https://so.csdn.net/so/search/s.do?q=mvc&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)




