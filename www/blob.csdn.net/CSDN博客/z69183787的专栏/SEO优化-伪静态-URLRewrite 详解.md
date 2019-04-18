# SEO优化-伪静态-URLRewrite 详解 - z69183787的专栏 - CSDN博客
2015年07月19日 20:14:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1580
转自：[http://hi.baidu.com/allen_zzw/blog/item/07c6b3909ad8eb0bd31b7049.html](http://hi.baidu.com/allen_zzw/blog/item/07c6b3909ad8eb0bd31b7049.html)
下面是文章是我在网上看到的，觉得写的还不错，我拿过来，修改了一些作者没有说到的地方.......
1. 在apache中配置
2. 用URLRewrite(详细：重点是UrlRewrite+Struts2)
1） 从[http://tuckey.org/urlrewrite/](http://tuckey.org/urlrewrite/) 下载最新的jar包下来，现时我用的是Url Rewrite3.2
2） 将urlrewrite.xml拷贝到WEB-INF目录下
3） 在web.xml中配置其拦截器。注意，拦截器应放在strutsFilter的前面：
[一般顺序是这样子的来排版的，字符编码过滤器， Hibernate Open Session In View 过滤器，UrlRewrite过滤器、struts过滤器。假如你有用到这些过滤器的话，就这样子拍，否则有时候会出现一些莫名其妙的问题。。。。。。。。]
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="2.5"
xmlns="[http://java.sun.com/xml/ns/javaee](http://java.sun.com/xml/ns/javaee)"
xmlns:xsi="[http://www.w3.org/2001/XMLSchema-instance](http://www.w3.org/2001/XMLSchema-instance)"
xsi:schemaLocation="[http://java.sun.com/xml/ns/javaee](http://java.sun.com/xml/ns/javaee)
[http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd](http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd)">
<filter>
<filter-name>encodingFilter</filter-name> <filter-class>
org.springframework.web.filter.CharacterEncodingFilter
</filter-class>
<init-param>
<param-name>encoding</param-name>
<param-value>UTF-8</param-value>
</init-param>
<init-param>
<param-name>forceEncoding</param-name>
<param-value>true</param-value>
</init-param>
</filter>
<filter-mapping>
<filter-name>encodingFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
<filter>
<filter-name>UrlRewriteFilter</filter-name>
<filter-class>org.tuckey.web.filters.urlrewrite.UrlRewriteFilter</filter-class>
<init-param>
<param-name>logLevel</param-name>
<param-value>WARN</param-value>
</init-param>
</filter>
<filter-mapping>
<filter-name>UrlRewriteFilter</filter-name>
<url-pattern>/*</url-pattern>
<dispatcher>REQUEST </dispatcher>
<dispatcher>FORWARD </dispatcher>
</filter-mapping>
<filter>
<filter-name>struts2</filter-name>
<filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter</filter-class>
</filter>
<filter-mapping>
<filter-name>struts2</filter-name>
<url-pattern>*.action</url-pattern>
<dispatcher>REQUEST </dispatcher>
<dispatcher>FORWARD </dispatcher>
</filter-mapping>
<filter-mapping>
<filter-name>struts2</filter-name>
<url-pattern>*.jsp</url-pattern>
</filter-mapping>
</web-app>
其中，<dispatcher>REQUEST </dispatcher>和<dispatcher>FORWARD </dispatcher> 主要说明该拦截器拦截的请求包括客户端的请求和从服务器端forward的请求。在很多的网文中，都提到struts2的<url-pattern>/*</url-pattern> 我试过，这样是会报内存溢出异常的。上述的是2.16的版本的配置。
以下是urlrewrite.xml的配置
<rule>
<from>/test.dhtml</from>
<to type="forward">test.action</to>
</rule>
<rule>
<from>^/([a-zA-Z]+)/save/id/([a-zA-Z0-9]*).dhtml</from>
<to type="forward">$1!save.action?id=$2</to>
</rule>
<rule>
<from>^/([a-zA-z]+)/([a-zA-Z0-9]*).dhtml</from>
<to type="forward">%{context-path}/$1!$2.action</to>
</rule>
<rule>
<from>^/content/([a-zA-Z]+.*).html</from>
<to type="forward">/WEB-INF/content/login-error.jsp</to>
</rule>
之前如果在web.xml中设置<dispatcher>的话，则一直都不能forward都对应的action中去。以上 在地址栏中输入 [http://lacalhost:8080/](http://lacalhost:8080/)应用名/test.dhtml 就可以映射到test.action中。如果要映射test!save.action可以写成 test/save.dhtml就可以了。
看 test-access.jsp的代码（这里，我用了Convention的零配置，所以test-access.jsp在 WEB-INF/content目录中）
<s:form action="test/save.dhtml" method="POST">
<s:textfield name="msg" label="输入信息"/>
<s:token name="token"/>
<s:submit value="确定" />
</s:form>
这里加入了 token令牌，主要是防止表单重复提交的。
那么，我们可以将在@Result中的错误页面也可以写成html的形式了，因为在上面配置了。
<rule>
<from>^/content/([a-zA-Z]+.*).html</from>
<to type="forward">/WEB-INF/content/$1.jsp</to>
</rule>
将所有的html转换成.jsp来处理。
那么，在@Result中 我们就可以这样写：
@Result(name="invalid.token",location="/content/error.html",type="redirect")})
在重复提交的情况下，URL会转向到/WEB-INF/content/error.jsp中。
但是上述<s:form action="test/save.dhtml" method="POST">并不是一种好的方式。它在一定程度上依赖了UrlRewrite。我们希望，在<s:form/>中依然写test!save.action 但是在页面中显示的是test/save.dhtml。那么我们可以用<outBund-rule/>标签。
此标签的原理是，页面通过UrlRewrite过滤器后，这个过滤器会遍历这个页面，用设定的正则来替换页面中的URL。如：
<outbound-rule>
<from>/test!save.action</from>
<to type="redirect">/test/save.dhtml</to>
</outbound-rule>
**注意：在uoutbound-rule的<from>标签里面如果出现“？”号，要把它转义，转义后是： “/？“，不然会出现404找不到错误。**
这样，在jsp中写的<s:form action=”test!save.action”> 而通过编译后在客户端中的源码就可以看到：<form id="test" name="test" action="/strutsTest/test/save.dhtml" method="POST">
到此，还是不能进行映射，我们还要进行对test/save.dhtml进行<role/>映射。
<rule>
<from>^/test/save.dhtml</from>
<to type="forward">test!save.action</to>
</rule>
总结：UrlRewrite这种方式虽好，但是他在每次匹配一个页面的时候都会遍历urlrewrite.xml中的所有<role/> 所以，在用的时候，应该注意，尽量将<role/>的个数做到越少。那就需要尽量写出更通用的正则表达式。尽量让代码规范性。这也是高质量代码的要求。
**简单的介绍一下常用的正规表示式：**
. 匹配除换行符以外的任意字符 
\w 匹配字母或数字或下划线或汉字 
\s 匹配任意的空白符 
\d 匹配数字 
\b 匹配单词的开始或结束 
^ 匹配字符串的开始 
$ 匹配字符串的结束
常用的&要用 &来表示。$1,$2代表与你配置正规表达式>/(\w+)/(\w+)/相对应的参数。<to type="forward">默认的是 type="forward".
1.转向HTML页面的url配置规则：
a)没有参数：loginAction是对应转向的Action，login是Action里面执行的方法，Action和方法名之间用下划线连接，$1是Acton名字；$2是方法名。例如：http://localhost:8080/struts2/loginAction_login.html
<rule>
     <from>^/([a-zA-Z]+?)_([a-zA-Z]+?)\.html$</from>
     <to type="forward">/$1!$2.do</to>
</rule>
b)一个参数：33是要传递的参数，同样Action和方法名以及参数之间的连接都是使用下划线，例如：http://localhost:8080/struts2/loginAction_login_33.html
<rule>
<from>
   ^/([a-zA-Z]+?)_([a-zA-Z]+?)_([0-9a-zA-Z]+?)\.html$
</from>
        <to type="forward">/$1!$2.do?id=$3</to>
</rule>
c)多个参数：33，ydf和2012-03-31都是参数，例如http://localhost:8080/struts/login_more_33_ydf_2012-03-31.html
<rule>
<from>  ^/([a-zA-Z]+?)_([a-zA-Z]+?)_([0-9a-zA-Z]+?)_([0-9a-zA-Z]+?)_([0-9a-zA-Z]+?)\.html$
</from>
   <to type="forward">
/$1!$2.do?id=$3&name=$4&date=$5
</to>
</rule>
**注意：**
**1.****在uoutbound-rule的<from>标签里面如果出现“？”号，要把它转义，转义后是： “/？“，不然会出现404找不到错误。**
**2.****如果是多参数配置在url参数拼接时注意&符号的转义：如<to type="forward">/$1!$2.do?id=$3&name=$4&date=$5</to>**
附加：**web.xml**
<?xmlversion=*"1.0"*encoding=*"UTF-8"*?>
<web-appversion=*"2.5"*xmlns=*"http://java.sun.com/xml/ns/javaee"*
xmlns:xsi=*"http://www.w3.org/2001/XMLSchema-instance"*
xsi:schemaLocation=*"http://java.sun.com/xml/ns/javaee*
*http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"*>
<welcome-file-list>
<welcome-file>index.jsp</welcome-file>
</welcome-file-list>
<!-- UrlRewriteFilter filter -->
<filter>
<filter-name>UrlRewriteFilter</filter-name>
<filter-class>org.tuckey.web.filters.urlrewrite.UrlRewriteFilter</filter-class>
<init-param>
<param-name>logLevel</param-name>
<param-value>WARN</param-value>
</init-param>
</filter>
<!-- Struts2 filter -->
<!--
在做上传文件的时候，要在web.xml中增加ActionContextCleanUp这个filter，如果不增加，会发生第一次上传取不到文件的情况
-->
<filter>
<filter-name>struts2-cleanup</filter-name>
<filter-class>org.apache.struts2.dispatcher.ActionContextCleanUp</filter-class>
</filter>
<filter>
<filter-name>struts2</filter-name>
<filter-class>org.apache.struts2.dispatcher.FilterDispatcher</filter-class>
</filter>
<!-- 如果使用Urlrewrite，要指定filter-mapping的dispatcher方式 -->
<filter-mapping>
<filter-name>UrlRewriteFilter</filter-name>
<url-pattern>/*</url-pattern>
<dispatcher>REQUEST</dispatcher>
<dispatcher>FORWARD</dispatcher>
</filter-mapping>
<filter-mapping>
<filter-name>struts2-cleanup</filter-name>
<url-pattern>/*</url-pattern>
<dispatcher>REQUEST</dispatcher>
<dispatcher>FORWARD</dispatcher>
</filter-mapping>
<filter-mapping>
<filter-name>struts2</filter-name>
<url-pattern>/*</url-pattern>
<dispatcher>REQUEST</dispatcher>
<dispatcher>FORWARD</dispatcher>
</filter-mapping>
</web-app>
**urlrewrite.xml**
<?xmlversion=*"1.0"*encoding=*"UTF-8"*?>
<!DOCTYPEurlrewritePUBLIC"-//tuckey.org//DTD UrlRewrite 3.0//EN"
"http://tuckey.org/res/dtds/urlrewrite3.0.dtd">
<urlrewrite>
<rule>
<note>
<!-- 这是一个通用请求url rewrite 
将请求forwordurl加上".action"的处理器上。例：
请求 http://{domain}/user/login    将被forward到 http://{domain}/user/login.do 
请求 http://{domain}/user/login/   将被forward到 http://{domain}/user/login.do 
请求 http://{domain}/user/logout/  将被forward到 http://{domain}/user/logout.do
-->
<!--
<from>^/([_a-zA-Z]+[_0-9a-zA-Z-/]*[_0-9a-zA-Z]+)/?.html$</from>
<to type="forward">/$1.do</to>
-->
</note>
<!-- 匹配URL:login_reg_33.html,执行login Action的reg方法，带一个参数：33
 -->
<from>^/([a-zA-Z]+?)_([a-zA-Z]+?)_([0-9a-zA-Z]+?)\.html$</from>
<totype=*"forward"*>/$1!$2.do?id=$3</to>
</rule>
<!-- 匹配URL:login_reg_33.html,执行login Action的reg方法，多个参数：33,ydf,2012-03-31
 -->
<!-- 注意多个参数之间连接的&需要换为&转移 -->
<rule>
<from>^/([a-zA-Z]+?)_([a-zA-Z]+?)_([0-9a-zA-Z]+?)_([0-9a-zA-Z]+?)_([0-9a-zA-Z]+?)\.html$</from>
<totype=*"forward"*>/$1!$2.do?id=$3&name=$4&date=$5</to>
</rule>
<!-- 匹配URL:login_login.html,执行login Action的login方法 -->
<rule>
<from>^/([a-zA-Z]+?)_([a-zA-Z]+?)\.html$</from>
<totype=*"forward"*>/$1!$2.do</to>
</rule>
<!-- 匹配URL：user3/login/test/baidu/login_login.html,任意长度路径 ,执行login
 Action的login方法 -->
<rule>
<from>^/[_a-zA-Z]+[_0-9a-zA-Z-/]*[_0-9a-zA-Z]+/[^~]+?\/([^\/]+?)_([^\.]+?)\.html$</from>
<totype=*"forward"*>/$1!$2.do</to>
</rule>
