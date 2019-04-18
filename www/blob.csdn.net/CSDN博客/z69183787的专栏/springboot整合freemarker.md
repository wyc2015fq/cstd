# springboot整合freemarker - z69183787的专栏 - CSDN博客
2017年06月28日 17:02:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10628
[http://www.cnblogs.com/nannan0226/p/6400273.html](http://www.cnblogs.com/nannan0226/p/6400273.html)
[http://412887952-qq-com.iteye.com/blog/2335218](http://412887952-qq-com.iteye.com/blog/2335218)
前提：
开发工具：idea
框架：spring boot、maven
1、pom文件添加依赖
```
<dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-freemarker</artifactId>
            <version>1.4.1.RELEASE</version>
        </dependency>
```
2、新建spring web项目，会自动生成application.properties.
使用application.properties配置文件之后，spring boot启动时，会自动把配置信息读取到spring容器中，并覆盖spring boot的默认配置。
3、在配置文件中，设置以下两个参数
```
#设定ftl文件路径
spring.freemarker.template-loader-path=classpath:/templates
#设定静态文件路径，js,css等
spring.mvc.static-path-pattern=/static/**
```
controller跳转页面如下：
templates/action/list.ftl页面
![](http://images2015.cnblogs.com/blog/816787/201702/816787-20170215103829269-1581468016.png)
  最近有好久没有更新博客了，感谢小伙伴的默默支持，不知道是谁又打赏了我一个小红包，谢谢。
       今天我们讲讲怎么在Spring Boot中使用模板引擎freemarker，先看看今天的大纲：
写道
(1) freemarker介绍；
(2) 新建spring-boot-freemarker工程；
(3) 在pom.xml引入相关依赖；
(4) 编写启动类；
(5) 编写模板文件hello.ftl;
(6) 编写访问类HelloController;
(7) 测试；
(8) freemarker配置；
(9) freemarker常用语法；
(10) freemarkerlayout
**(1) freemarker介绍；**
       FreeMarker是一款模板引擎： 即一种基于模板和要改变的数据，   并用来生成输出文本（HTML网页、电子邮件、配置文件、源代码等）的通用工具。       它不是面向最终用户的，而是一个Java类库，是一款程序员可以嵌入他们所开发产品的组件。
**(2) 新建spring-boot-freeMarker工程；**
       我们新建一个maven工程，取名为：spring-boot-freeMarker
**(3) 在pom.xml引入相关依赖；**
       这里使用freeMarker需要引入相关依赖包：spring-boot-starter-freeMarker，
<projectxmlns=*"http://maven.apache.org/POM/4.0.0"*xmlns:xsi=*"http://www.w3.org/2001/XMLSchema-instance"*
xsi:schemaLocation=*"http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd"*>
<modelVersion>4.0.0</modelVersion>
<groupId>com.kfit</groupId>
<artifactId>spring-boot-velocity</artifactId>
<version>0.0.1-SNAPSHOT</version>
<packaging>jar</packaging>
<name>spring-boot-velocity</name>
<url>http://maven.apache.org</url>
<properties>
<project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
<!-- jdk版本号，angel在这里使用1.8，大家修改为大家本地配置的jdk版本号即可 -->
<java.version>1.8</java.version>
</properties>
<!--
       spring boot 父节点依赖,
引入这个之后相关的引入就不需要添加version配置，
       spring boot会自动选择最合适的版本进行添加。
     -->
<parent>
<groupId>org.springframework.boot</groupId>
<artifactId>spring-boot-starter-parent</artifactId>
<version>1.4.1.RELEASE</version><!--
 1.4.1.RELEASE , 1.3.3.RELEASE-->
</parent>
<dependencies>
<dependency>
<groupId>junit</groupId>
<artifactId>junit</artifactId>
<scope>test</scope>
</dependency>
<!-- spring boot web支持：mvc,aop... -->
<dependency>
<groupId>org.springframework.boot</groupId>
<artifactId>spring-boot-starter-web</artifactId>
</dependency>
<!-- 引入freeMarker的依赖包.
 -->
<dependency>
<groupId>org.springframework.boot</groupId>
<artifactId>spring-boot-starter-freeMarker</artifactId>
</dependency>
</dependencies>
</project>
**(4) 编写启动类；**
       启动类没有什么特别之处，不过多介绍，请看代码：
**package** com.kfit;
**import** org.springframework.boot.SpringApplication;
**import** org.springframework.boot.autoconfigure.SpringBootApplication;
/**
 *
 * **@author** Angel --守护天使
 * **@version** v.0.1
 * **@date** 2016年10月4日
 */
@SpringBootApplication
**public class** App {
**publicstaticvoid** main(String[] args)
 {
       SpringApplication.*run*(App.**class**, args);
    }
}
**(5) 编写模板文件hello.ftl;**
       编写一个hello.ftl文件，此文件的路径在src/main/resources/templates**（默认路径）**下，其中hello.ftl文件的内容如下：
<html> 
<body> 
    welcome ${name}  to freemarker!
</body> 
</html>
**(6) 编写访问类HelloController;**
       有了模板文件之后，我们需要有个Controller控制类，能够访问到hello.ftl文件，这里也很简单，具体看如下代码：
**package** com.kfit.demo.web;
**import** java.util.Map;
**import** org.springframework.stereotype.Controller;
**import** org.springframework.web.bind.annotation.RequestMapping;
/**
 * 测试velocity;
 * **@author** Angel --守护天使
 * **@version** v.0.1
 * **@date** 2016年10月4日
 */
@Controller
**public class** HelloController {
@RequestMapping("/hello")
**public** String hello(Map<String,Object> map){
map.put("name", "[Angel -- 守护天使]");
**return **"hello";
    }
}
**(7) 测试；**
       好了，到这里，我们就可以启动我们的程序进行测试了，访问地址：
[http://127.0.0.1:8080/hello](http://127.0.0.1:8080/hello) ，如果你在浏览器中看到如下信息：
welcome [Angel -- 守护天使] to freemarker!
那么说明你的demo ok 了。
**(8) freemarker配置；**
       在spring boot的application.properties属性文件中为freemarker提供了一些常用的配置,如下：
########################################################
###FREEMARKER (FreeMarkerAutoConfiguration)
########################################################
spring.freemarker.allow-request-override=false
spring.freemarker.cache=true
spring.freemarker.check-template-location=true
spring.freemarker.charset=UTF-8
spring.freemarker.content-type=text/html
spring.freemarker.expose-request-attributes=false
spring.freemarker.expose-session-attributes=false
spring.freemarker.expose-spring-macro-helpers=false
#spring.freemarker.prefix=
#spring.freemarker.request-context-attribute=
#spring.freemarker.settings.*=
#spring.freemarker.suffix=.ftl
#spring.freemarker.template-loader-path=classpath:/templates/ #comma-separated list
#spring.freemarker.view-names= # whitelist of view names that can be resolved
**(9) freemarker常用语法；**
       freemarker的语法并不是本节的重点，这里还是简单的介绍下几个常用的if else,list；
       首先我们改造下HelloController的hello方法
@RequestMapping("/hello")
**public** String hello(Map<String,Object> map){
map.put("name", "[Angel -- 守护天使]");
map.put("gender",1);//gender:性别，1：男；0：女；
       List<Map<String,Object>> friends =**new** ArrayList<Map<String,Object>>();
       Map<String,Object> friend = **new** HashMap<String,Object>();
friend.put("name", "张三");
friend.put("age", 20);
friends.add(friend);
friend = **new** HashMap<String,Object>();
friend.put("name", "李四");
friend.put("age", 22);
friends.add(friend);
map.put("friends", friends);
**return **"hello";
    }
       这里我们返回了gender和friends的列表；
      接下来我们看看怎么在freemarker进行展示呢?
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:th="http://www.thymeleaf.org"
      xmlns:sec="http://www.thymeleaf.org/thymeleaf-extras-springsecurity3">
    <head>
        <title>Hello World!</title>
    </head>
    <body>
       <p>
           welcome ${name}  to freemarker!
       </p>      
       <p>性别：
           <#if gender==0>
              女
           <#elseif gender==1>
              男
           <#else>
              保密   
           </#if>
        </p>
       <h4>我的好友：</h4>
       <#list friends as item>
           姓名：${item.name} , 年龄${item.age}
           <br>
       </#list>
    </body>
</html>
**(10) freemarker layout**
       freemarker layout主要处理具有相同内容的页面，比如每个网站的header和footer页面。
       freemarker 的布局主要常见的两种方式是#import(“文件路径”)和#include(“文件路径”)，其中import和include的区别在于，include常用于公共部分的页面，如果要使用<#assign username=“张三”>涉及到内部函数以及变量声明之类的，使用import进行导入，如果在import中的页面含有页面当前将不会进行渲染。   我们编写一个header和footer，其中的header使用include引入，footer页面也使用include引入。（当然freemarker 还有别的布局方式，这里只是介绍一种，请自行学习研究）
       header.ftl内容：
<header>
    This is a header,welcome  ${name} to my web site!
</header>
<hr>
       footer.ftl内容：
<hr>
<footer>
    This is a footer,welcome  ${name} to my web site!
</footer>
       修改hello.ftl：
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:th="http://www.thymeleaf.org"
      xmlns:sec="http://www.thymeleaf.org/thymeleaf-extras-springsecurity3">
    <head>
        <title>Hello World!</title>
    </head>
    <body>
        <#include "/header.ftl" >
       <p>
           welcome ${name}  to freemarker!
       </p>      
       <p>性别：
           <#if gender==0>
              女
           <#elseif gender==1>
              男
           <#else>
              保密   
           </#if>
        </p>
       <h4>我的好友：</h4>
       <#list friends as item>
           姓名：${item.name} , 年龄${item.age}
           <br>
       </#list>
       <#include "/footer.ftl" >
    </body>
</html>
       到这里就ok了，我们访问/hello页面，应该会看到如下图的效果：
![](http://dl2.iteye.com/upload/attachment/0121/0203/67ee19e6-6b1d-3680-9e30-f89198caa8fc.png)
