# Sitemesh和Struts2结合时Filter的配制顺序 - z69183787的专栏 - CSDN博客
2015年07月18日 21:01:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：690
个人分类：[其他-Sitemesh网页模板																[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)](https://blog.csdn.net/z69183787/article/category/5662947)
    Sitemesh和Struts2结合时要特别注意它们各自Filter的顺序，否则会导致双方都不能正常工作。而又由于Struts2在2.1.3之前使用的是FilterDispatcher，2.1.3之后推荐使用StrutsPrepareFilter和StrutsExecuteFilter替代FilterDispatcher，因此配制上又会有所不同。
      2.1.3之前的版本，sitemesh的filter要介于StrutsPrepareFilter和FilterDispatcher之间，配制如下：
**[xhtml]**[view
 plain](http://blog.csdn.net/bluishglc/article/details/5969769#)[copy](http://blog.csdn.net/bluishglc/article/details/5969769#)
- <filter>
- <filter-name>struts-cleanup</filter-name>
- <filter-class>org.apache.struts2.dispatcher.ActionContextCleanUp</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>struts-cleanup</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- 
- <filter>
- <filter-name>sitemesh</filter-name>
- <filter-class>com.opensymphony.module.sitemesh.filter.PageFilter</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>sitemesh</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- 
- <filter>
- <filter-name>struts</filter-name>
- <filter-class>org.apache.struts2.dispatcher.FilterDispatcher</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>struts</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
       2.1.3之后的版本，sitemesh的filter要介于StrutsPrepareFilter和StrutsExecuteFilter之间，配制如下：
**[xhtml]**[view
 plain](http://blog.csdn.net/bluishglc/article/details/5969769#)[copy](http://blog.csdn.net/bluishglc/article/details/5969769#)
- <filter>
- <filter-name>struts-cleanup</filter-name>
- <filter-class>org.apache.struts2.dispatcher.ActionContextCleanUp</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>struts-cleanup</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- <filter>
- <filter-name>struts-prepare</filter-name>
- <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareFilter</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>struts-prepare</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- <filter>
- <filter-name>sitemesh</filter-name>
- <filter-class>com.opensymphony.module.sitemesh.filter.PageFilter</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>sitemesh</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- 
- <filter>
- <filter-name>struts-execute</filter-name>
- <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsExecuteFilter</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>struts-execute</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
