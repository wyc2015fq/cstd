# urlrewrite使用介绍 - z69183787的专栏 - CSDN博客
2015年07月19日 20:07:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2207
urlrewrite是一个类似于apache的mod_rewrite模块的基于web服务器的filter之上的开源java工具 
主要功能： 
1.实现url地址的伪静态化，有利于搜索引擎收录 
例如可以把http://www.flyox.com/showBlog.jspx?id=55 的请求重定向或转发到http://www.flyox.com/blog/55， 
这样做就避免了url中出现?&之类的符号。而且页面伪静态化之后也增加了被搜索引擎找到的概率. 
google虽然可以抓取动态页面，但是google对动态页面的评分一般低于静态页面。所以，对大量信息发布的网站，把网站地址改变成静态的绝对是值得的。这条应该是最有说服力的理由了. 
2.并且可以使url变得更加整齐，隐藏技术实现，使url看不出是基于那种框架(JSP, Servlet, Struts etc.)提高网站的移植性 
比如http://www.flyox.com/admin/login.jsp 使用urlrewrite可以转变为http://www.flyox.com/admin/login 
良好的url设计给用户带来的非常好的体验。如果每个页面都挂着鲜明的.asp/.jsp这种开发语言的标记，可以一眼让人看出你的网站使用什么语言做的。而且在改变网站的语言的时候，你需要改动大量的链接。 
3.根据HTTP headers （比如user-agent，charset）日期等转向不同的url 
rewrite技术可以在代理服务器实现，或者可以在tomcat之类的java web服务器实现。 Apache HTTP Server 2.x 和 mod_proxy提供了不错的Rewrite处理方法。如果使用apache，则直接在apache中配置Rewrite即可。但是对于没有使用代理，或者不完全要在代理服务器配置Rewrite的web应用，通过第三方工具包也是可以实现很很好的rewrite技术的。 
这个第三方包的地址是：http://tuckey.org/urlrewrite/ 
下面主要介绍一下这个 
这个包使用正则表达式仿照了apache rewrite的做法。而实现原理也是很明晰的：filter,forward,redirect机制组合：
在web.xml配置一个UrlRewriteFilter，拦截所有进来的请求，对服务要求的请求进行rewrite。比如，对http: //www.flyox.com/user/([0-9]+)请求，forward到/profile.do?id=$1。这就是一条规则。在这个第三方包中的规则文件urlrewrite.xml中可以这样定义： 
代码 
    <rule>  
        <from>^/user/([0-9]+)/?$</from>  
        <to type="forward">/profile.do?id=$1</to>  
    </rule>  
对我来说，一般我会配置一个通用的rule，使所有没有包含"."的url forward到.do的action上来 
代码 
    <rule>  
        <note>  
            - 这是一个通用请求url rewrite  
            - 将请求forword url加上“.do”的处理器上。  
            - 例：  
            -   请求 http://{domain}/user/login    将被forward到 http://{domain}/user/login.do  
            -   请求 http://{domain}/user/login/   将被forward到 http://{domain}/user/login.do  
            -   请求 http://{domain}/user/logout/  将被forward到 http://{domain}/user/logout.do  
        </note>  
       <from>^/([_a-zA-Z]+[_0-9a-zA-Z-/]*[_0-9a-zA-Z]+)/?$</from>  
       <to type="forward">/$1.do</to>  
   </rule>  
上面只是介绍个大概，并不是urlrewriter这个jar包的使用指南。如果你觉得这种rewrite做法不错的话，不如直接访问这个网站吧 http://tuckey.org/urlrewrite/ 
第一步：从http://tuckey.org/urlrewrite/ 上下载urlrewrite-3.0.4.jar包，放到webapp classpath下 
第二步：在web.xml配置UrlRewriteFilter： 
代码 
   1. <filter>  
   2.     <filter-name>UrlRewriteFilter</filter-name>  
   3.     <filter-class>  
   4.         org.tuckey.web.filters.urlrewrite.UrlRewriteFilter  
   5.     </filter-class>  
   6.     <init-param>  
   7.         <param-name>logLevel</param-name>  
   8.         <param-value>WARN</param-value>  
   9.     </init-param>  
  10. </filter>  
  11. <filter-mapping>  
  12.     <filter-name>UrlRewriteFilter</filter-name>  
  13.     <url-pattern>/*</url-pattern>  
  14. </filter-mapping>  
第三步：在web-inf下新建urlrewrite.xml文件，简单填入以下内容： 
代码 
   1. <?xml version="1.0" encoding="utf-8"?>  
   2. <!DOCTYPE urlrewrite PUBLIC "-//tuckey.org//DTD UrlRewrite 3.0//EN"  
   3.     "http://tuckey.org/res/dtds/urlrewrite3.0.dtd">  
   4.   
   5. <urlrewrite>  
   6.     <rule>  
   7.         <note>  
   8.             - 这是一个通用请求url rewrite  
   9.             - 将请求forword url加上“.do”的处理器上。  
  10.             - 例：  
  11.             -   请求 http://{domain}/user/login    将被forward到 http://{domain}/user/login.do  
  12.             -   请求 http://{domain}/user/login/   将被forward到 http://{domain}/user/login.do  
  13.             -   请求 http://{domain}/user/logout/  将被forward到 http://{domain}/user/logout.do  
  14.         </note>  
  15.         <from>^/([_a-zA-Z]+[_0-9a-zA-Z-/]*[_0-9a-zA-Z]+)/?$</from>  
  16.         <to type="forward">/$1.do</to>  
  17.     </rule>  
  18.   
  19. </urlrewrite>  
第四步：启动web应用，将原来需要.do的请求改为去掉.do后请求，看看结果。 
第五步：更详细的规则定义，请看http://tuckey.org/urlrewrite/manual/3.0/ 
我举个例子： 
你的请求可能是http://www.flyox.com/user/somepath?name=hello 
可以这样配置： 
代码 
   1. <rule>    
   2.     <from>^/user/somepath$</from>    
   3.     <to type="forward">/user/anotherpath.do</to>    
   4. </rule>   
这样，http://www.flyox.com/user/somepath?name=hello 便可以rewrite到anotherpath.do请求，并且因为使用的是forward类型的Type，所以在anotherpath.do一样可以获得name的参数。 
不仅仅如此，还可以配置多加参数如下： 
代码 
   1. <rule>    
   2.     <from>^/user/somepath$</from>    
   3.     <to type="forward">/user/anotherpath.do?action=hehe</to>    
   4. </rule>   
在anotherpathe.do中可同时取得action和name这两个参数。 
另外，tomcat自 5.0.15 后，提供了一个类似rewrite做法(for simple balance): 
filter + 自带或定制的rule规则实现 + redirect重定向(没有forward，故url将被改为被重定向后的地址)。 
在rewrite处理上，tomcat提供的功能是比较一般的，不及上面介绍的jar包。 
但是tomcat的做法是根据具体的HttpServletRequest来做判断的，不仅仅是进来的请求的url，所以，各有千秋。 
apache 的mod_rewrite已经很成熟了,功能,或者效率上都非常的优秀，对于多个应用使用起来更方便。 
urlrewrite filter只是给那些不打算使用apache的应用提供了另外一种选择。
