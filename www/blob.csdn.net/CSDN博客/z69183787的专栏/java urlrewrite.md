# java urlrewrite - z69183787的专栏 - CSDN博客
2015年07月19日 20:06:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：540
个人分类：[其他-页面静态化](https://blog.csdn.net/z69183787/article/category/5664615)

这个实现了确实不错，地址栏上看起来很漂亮，比如是××.html?2其实，这个是一个动态页面，比如是JSP，呵呵。 
1 首先下载一个urlrewrite包，我的附件里有是3.2，或者去官方下载一个，http://tuckey.org/urlrewrite/dist/urlrewritefilter-3.2.jar。不过这个官方好像老是打不开。 
2 将这个urlrewrite-3.2.0.jar放到你的项目的WEB-INF/lib/目录下。 
3 在你的项目的web.xml中，在<welcome-file-list>节点后，增加:
|1234567891011121314|<filter>       <filter-name>UrlRewriteFilter</filter-name>       <filter-class>           org.tuckey.web.filters.urlrewrite.UrlRewriteFilter       </filter-class>       <init-param>           <param-name>logLevel</param-name>           <param-value>WARN</param-value>       </init-param>   </filter>   <filter-mapping>       <filter-name>UrlRewriteFilter</filter-name>       <url-pattern>/*</url-pattern>   </filter-mapping>|
4 WEB-INF/lib/目录下新增 urlrewrite.xml文件，内容如下：
|123456789101112131415161718192021|<?xml version="1.0" encoding="utf-8"?>    <!DOCTYPE urlrewrite PUBLIC "-//tuckey.org//DTD UrlRewrite 2.6//EN""http://tuckey.org/res/dtds/urlrewrite2.6.dtd">      <!--      Configuration file for UrlRewriteFilter    http://tuckey.org/urlrewrite/      -->    <urlrewrite>           <rule>          <from>/([a-z]+).shtml$</from>  		<to>/templates/tempIndex.jsp?name=$1</to>      </rule>     <rule>          <from>/list_([0-9]+).html</from>  		<to>/templates/tempList.jsp?id=$1</to>      </rule>       </urlrewrite>|
这个是设置重写的规则的，我写的第一个规则表示，浏览器中命名为 字母.shtml 的请求，都会被提交给当前应用下的templates/tempIndex.jsp这个jsp文件，参数就是请求的名字，也就是字母的值。注意：to的值是以/开头的，/表示，当前应用。 
第二个规则，也就说，浏览器中以list_数字.html的请求，都会被提交到当前应用的templates/tempList.jsp?文件，参数为数字。
