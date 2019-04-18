# 使用UrlRewrite的完整步骤，页面静态化 - z69183787的专栏 - CSDN博客
2015年07月19日 20:05:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4400
最近接到一个任务关于实现访问路径静态化，首先百度了他的定义：　UrlRewriteFilter是一个用于改写URL的Web过滤器，类似于Apache的mod_rewrite。适用于任何Web应用服务器（如Resin，Orion，Tomcat等）。其典型应用就把动态URL静态化，便于搜索引擎爬虫抓取你的动态网页。 其主要作用就是优化访问，便于搜索引擎收录。因为其他朋友说的很迷糊或不够简单明了，所以自己写了一份！
1、下载urlrewritefilter 解压 将urlrewrite-3.2.0.jar 放于lib下 ,urlrewrite.xml  放于WEB-INF  （[http://www.tuckey.org/urlrewrite/#download](http://www.tuckey.org/urlrewrite/#download) 
 点击[urlrewritefilter-3.2.0-src.zip](http://urlrewritefilter.googlecode.com/files/urlrewritefilter-3.2.0-src.zip) ）
2、配置web.xml
<filter>
<filter-name>UrlRewriteFilter</filter-name>
<filter-class>org.tuckey.web.filters.urlrewrite.UrlRewriteFilter</filter-class>
</filter>
<filter-mapping>
<filter-name>UrlRewriteFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping> 
3、简单测试
1、打开urlrewrite.xml  主题内容在 <urlrewrite> 以下的每一个<role>是一个访问路径的过滤，只要符合这个过滤条件都会被过滤
<rule><!-- a-zA-Z0-9]+) 通配符 通配小写/打些字母数字组合成的名称  基本就是所有访问名称了，这个配置的意思是 ：地址栏/项目名称/XXX.html 会被跳转到下面路径 -->
   <from>/([a-zA-Z0-9]+).html</from>   <!--  这里是访问路径-->
   <to>/index.jsp</to>                            <!-- 这里是跳转路径-->
 </rule>
部署项目，在IE上访问 /项目名称/XXX.html  都会被跳转到/index.jsp
2、这当然不是我们要的结果，我们还要从静态页面跳转到Action方法的，而且可能有的方法还带参数，
<rule>
   <from>/([a-zA-Z0-9]+)/([a-zA-Z0-9]+)/([a-zA-Z0-9]+).html</from>
<!-- h[ttp://localhost:8080/GDIDI/themeSorts/selectSortByjeId/10.html](http://localhost:8080/GDIDI/themeSorts/selectSortByjeId/10.html)-->
   <to>/$1.do?method=$2&jeId=$3</to>   <!-- 这里$1$2是对应上面访问路径的通配符位置,&符号用&转译，可以有多个参数-->
<!--实际跳转[http://localhost:8080/GDIDI/themeSorts.do?mwthod=selectSortByjeId&jeId=10](http://localhost:8080/GDIDI/themeSorts.do?mwthod=selectSortByjeId&jeId=10)-->
 </rule>
因为跳转路径采用了占位符$1$2..所以在配置路径的时候 一定要对应上(你也可以不用占位符，直接写死哪个Action，看个人需求) 而且名称不能出错，否则就会找不到路径.
**！注意**：以上方法适用servlet 、Struts 1 等框架（暂时只测那么多）
Struts2 须在web中多加配置 （此问题曾困扰许久）
<filter-mapping>   <!--注意 是Struts 不是urlrewrite-->
<filter-name>struts2 </filter-name> 
<url-pattern>*.do </url-pattern> 
<dispatcher>REQUEST </dispatcher> 
<dispatcher>FORWARD </dispatcher> 
<dispatcher>INCLUDE </dispatcher> 
</filter-mapping>
