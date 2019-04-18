# SiteMesh使用示例 - z69183787的专栏 - CSDN博客
2015年07月18日 20:25:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1383
**一、SiteMesh介绍**
  SiteMesh是一个网页布局和修饰的框架，利用它可以将网页的内容和页面结构分离，以达到页面结构共享的目的。[来自百度百科]
       通俗的理解就是，SiteMesh把页面中变化的和不变的分离开来，用不变的去装饰各种变化的内容。从而使页面具有统一的布局，而且方便页面的管理。不变的页面称之为装饰页面，内容变化的页面称之为被装饰页面。
      装饰页面一般包括：页面标题、头部、底部、主体以及公共的css、js。
      被装饰页面只需要写它自己需要的内容就可以了。
      根据页面需要，装饰页面可以有多个，被装饰页面也可以有不被装饰而保持自己风格的选择，这只需要在配置文件中配置一下就可以了。
      siteMesh3.0运行环境：servlet、  jdk
**二、SiteMesh使用**
   SiteMesh的使用也非常简单。这里使用的是sitemesh3.0。整个项目结构如图：
![](https://img-blog.csdn.net/20130805220833984)
      1. 下载[sitemesh3.0](https://github.com/sitemesh/sitemesh3/downloads) ，将disk文件夹下的sitemesh-3.0-alpha-2.jar放到lib目录下。
      2. 建立装饰页面，装饰页可以是静态文件，也可以是动态文件，这里用jsp来测试
 (1)带有菜单栏的装饰页面：decorator.jsp
**[html]**[view
 plain](http://blog.csdn.net/liusong0605/article/details/9773723#)[copy](http://blog.csdn.net/liusong0605/article/details/9773723#)
- <htmlxmlns="http://www.w3.org/1999/xhtml">
- <head>
- <sitemesh:writeproperty='head'/>
- <styletype='text/css'>
- .mainBody {    
-     padding: 10px;    
-     border: 1px solid #555555;    
- }   
- .conbgbtm {  
-     width:100%;  
-     min-height:400px;  
-     height:auto;  
-     overflow:hidden;  
-     zoom:1;  
- }  
- </style>
- </head>
- <body>
- <!--头部  -->
- <divalign="center">
- <h1>头部信息：   
- <sitemesh:writeproperty='title'/>
- </h1>
- </div>
- <hr>
- 
- <!--左侧菜单栏  -->
- <divclass="conbgbtm">
- <divclass="leftbox">
- <ul>
- <li><ahref="#">菜单1</a></li>
- <li><ahref="#">菜单2</a></li>
- <li><ahref="#">菜单3</a></li>
- </ul>
- </div>
- <sitemesh:writeproperty='body'></sitemesh:write>
- </div>
- <hr>
- 
- <divalign="center">
- <span>Copyright © 2012-2013 廊坊信息技术提高班 版权所有</span>
- </div>
- 
- </body>
- </html>
 (2)不带菜单栏的装饰页面：registerDecorator.jsp
**[html]**[view
 plain](http://blog.csdn.net/liusong0605/article/details/9773723#)[copy](http://blog.csdn.net/liusong0605/article/details/9773723#)
- <htmlxmlns="http://www.w3.org/1999/xhtml">
- <head>
- <sitemesh:writeproperty='head'/>
- <styletype='text/css'>
- .mainBody {    
-     padding: 10px;    
-     border: 1px solid #555555;    
- }   
- .conbgbtm {  
-     width:100%;  
-     min-height:400px;  
-     height:auto;  
-     overflow:hidden;  
-     zoom:1;  
- }  
- </style>
- </head>
- <body>
- <!--头部  -->
- <divalign="center">
- <h1>头部信息：   
- <sitemesh:writeproperty='title'/>
- </h1>
- </div>
- <hr>
- 
- <!--主体内容  -->
- <divclass="conbgbtm">
- <sitemesh:writeproperty='body'></sitemesh:write>
- </div>
- <hr>
- 
- <!--底部  -->
- <divalign="center">
- <span>Copyright © 2012-2013 廊坊信息技术提高班 版权所有</span>
- </div>
- 
- </body>
- </html>
      3. 建立被装饰页
(1)index首页
**[html]**[view
 plain](http://blog.csdn.net/liusong0605/article/details/9773723#)[copy](http://blog.csdn.net/liusong0605/article/details/9773723#)
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=GB18030">
- <title>SiteMesh3   title</title>
- </head>
- <body>
- <span>sitemesh3 body</span>
- </body>
- </html>
(2)logon.jsp、register.jsp页面
**[html]**[view
 plain](http://blog.csdn.net/liusong0605/article/details/9773723#)[copy](http://blog.csdn.net/liusong0605/article/details/9773723#)
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=GB18030">
- <title></title>
- </head>
- <body>
- <divalign="center">
- <p>用户名：<inputtype="text"id="userName"></p>
- <p>密码：    <inputtype="text"id="pwd"></p>
- <p>验证码：<inputtype="text"id="validate"></p>
- </div>
- </body>
- </html>
      4. web-inf/lib下建立Sitemesh3.xml配置文件
**[html]**[view
 plain](http://blog.csdn.net/liusong0605/article/details/9773723#)[copy](http://blog.csdn.net/liusong0605/article/details/9773723#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <sitemesh>
- <!--register页面的装饰页面为没有菜单栏 的registerDecorator.jsp -->
- <mapping>
- <path>/register.jsp</path>
- <decorator>/decorator/registerDecorator.jsp</decorator>
- </mapping>
- 
- <!--带有菜单栏的装饰页面  -->
- <mappingdecorator="/decorator/decorator.jsp"/>
- 
- <!--登录页面不被装饰。没有配置为true的页面，表示使用装饰页面,例如index.jsp -->
- <mappingpath="/logon.jsp"exclue="true"/>
- </sitemesh>
      5. Web.xml里加载sitemesh模板系统
**[html]**[view
 plain](http://blog.csdn.net/liusong0605/article/details/9773723#)[copy](http://blog.csdn.net/liusong0605/article/details/9773723#)
- <!-- 加载sitemesh模板系统 -->
- <filter>
- <filter-name>sitemesh</filter-name>
- <filter-class>org.sitemesh.config.ConfigurableSiteMeshFilter</filter-class>
- </filter>
- 
- <filter-mapping>
- <filter-name>sitemesh</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
运行效果：
登录页面不被装饰页面装饰
![](https://img-blog.csdn.net/20130805222020234)
首页面经过装饰后显示头部、底部、菜单栏等信息
![](https://img-blog.csdn.net/20130805222154968)
注册页面被只有头部和尾部信息的页面装饰：
![](https://img-blog.csdn.net/20130806093856015)
