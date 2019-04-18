# web.xml中出现default是什么意思？ - z69183787的专栏 - CSDN博客
2017年10月17日 10:14:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3438
转：http://blog.csdn.net/hello5orld/article/details/9407905
在web.xml文件中经常看到这样的配置<servlet-name>default</servlet-name>，这个配置的作用是：对客户端请求的静态资源如图片、JS文件等的请求交由默认的servlet进行处理，如下所示：
[html] [view plain](http://blog.csdn.net/wxwzy738/article/details/19072909#)[copy](http://blog.csdn.net/wxwzy738/article/details/19072909#)
- **<****servlet-mapping****>**
- **<****servlet-name****>**default**</****servlet-name****>**
- **<****url-pattern****>***.jpg**</****url-pattern****>**
- **</****servlet-mapping****>**
- **<****servlet-mapping****>**
- **<****servlet-name****>**default**</****servlet-name****>**
- **<****url-pattern****>***.png**</****url-pattern****>**
- **</****servlet-mapping****>**
- **<****servlet-mapping****>**
- **<****servlet-name****>**default**</****servlet-name****>**
- **<****url-pattern****>***.gif**</****url-pattern****>**
- **</****servlet-mapping****>**
- **<****servlet-mapping****>**
- **<****servlet-name****>**default**</****servlet-name****>**
- **<****url-pattern****>***.ico**</****url-pattern****>**
- **</****servlet-mapping****>**
- **<****servlet-mapping****>**
- **<****servlet-name****>**default**</****servlet-name****>**
- **<****url-pattern****>***.gif**</****url-pattern****>**
- **</****servlet-mapping****>**
- **<****servlet-mapping****>**
- **<****servlet-name****>**default**</****servlet-name****>**
- **<****url-pattern****>***.js**</****url-pattern****>**
- **</****servlet-mapping****>**
- **<****servlet-mapping****>**
- **<****servlet-name****>**default**</****servlet-name****>**
- **<****url-pattern****>***.css**</****url-pattern****>**
- **</****servlet-mapping****>**
下面是从[http://elf8848.iteye.com/blog/875830](http://elf8848.iteye.com/blog/875830)中摘录的一些资料
如何你的DispatcherServlet拦截"*.do"这样的有后缀的URL，就不存在访问不到静态资源的问题。
如果你的DispatcherServlet拦截"/"，为了实现REST风格，拦截了所有的请求，那么同时对*.js,*.jpg等静态文件的访问也就被拦截了。
我们要解决这个问题。
目的：可以正常访问静态文件，不可以找不到静态文件报404。
方案一：激活Tomcat的defaultServlet来处理静态文件
[html] [view plain](http://blog.csdn.net/wxwzy738/article/details/19072909#)[copy](http://blog.csdn.net/wxwzy738/article/details/19072909#)
- **<****servlet-mapping****>**
- **<****servlet-name****>**default**</****servlet-name****>**
- **<****url-pattern****>***.jpg**</****url-pattern****>**
- **</****servlet-mapping****>**
- **<****servlet-mapping****>**
- **<****servlet-name****>**default**</****servlet-name****>**
- **<****url-pattern****>***.js**</****url-pattern****>**
- **</****servlet-mapping****>**
- **<****servlet-mapping****>**
- **<****servlet-name****>**default**</****servlet-name****>**
- **<****url-pattern****>***.css**</****url-pattern****>**
- **</****servlet-mapping****>**
- 要配置多个，每种文件配置一个     
要写在DispatcherServlet的前面， 让 defaultServlet先拦截请求，这样请求就不会进入Spring了，我想性能是最好的吧。
Tomcat, Jetty, JBoss, and GlassFish 自带的默认Servlet的名字 -- "default"
Google App Engine 自带的 默认Servlet的名字 -- "_ah_default"
Resin 自带的 默认Servlet的名字 -- "resin-file"
WebLogic 自带的 默认Servlet的名字  -- "FileServlet"
WebSphere  自带的 默认Servlet的名字 -- "SimpleFileServlet" 
方案二： 在spring3.0.4以后版本提供了mvc:resources ，  使用方法：
[html] [view plain](http://blog.csdn.net/wxwzy738/article/details/19072909#)[copy](http://blog.csdn.net/wxwzy738/article/details/19072909#)
- <!-- 对静态资源文件的访问 -->
- **<****mvc:resources**mapping="/images/**"location="/images/"**/>**
/images/**映射到ResourceHttpRequestHandler进行处理，location指定静态资源的位置.可以是web application根目录下、jar包里面，这样可以把静态资源压缩到jar包中。cache-period 可以使得静态资源进行web cache 
如果出现下面的错误，可能是没有配置<mvc:annotation-driven />的原因。 
报错WARNING: No mapping found for HTTP request with URI [/mvc/user/findUser/lisi/770] in DispatcherServlet with name 'springMVC'
使用<mvc:resources/>元素,把mapping的URI注册到SimpleUrlHandlerMapping的urlMap中,
key为mapping的URI pattern值,而value为ResourceHttpRequestHandler,
这样就巧妙的把对静态资源的访问由HandlerMapping转到ResourceHttpRequestHandler处理并返回,所以就支持classpath目录,jar包内静态资源的访问.
另外需要注意的一点是,不要对SimpleUrlHandlerMapping设置defaultHandler.因为对static uri的defaultHandler就是ResourceHttpRequestHandler,
否则无法处理static resources request.
方案三 ，使用<mvc:default-servlet-handler/>
[html] [view plain](http://blog.csdn.net/wxwzy738/article/details/19072909#)[copy](http://blog.csdn.net/wxwzy738/article/details/19072909#)
- **<****mvc:default-servlet-handler****/>**
会把"/**" url,注册到SimpleUrlHandlerMapping的urlMap中,把对静态资源的访问由HandlerMapping转到org.springframework.web.servlet.resource.DefaultServletHttpRequestHandler处理并返回.
DefaultServletHttpRequestHandler使用就是各个Servlet容器自己的默认Servlet.
补充说明：多个HandlerMapping的执行顺序问题：
DefaultAnnotationHandlerMapping的order属性值是：0
<mvc:resources/ >自动注册的 SimpleUrlHandlerMapping的order属性值是： 2147483646
<mvc:default-servlet-handler/>自动注册 的SimpleUrlHandlerMapping 的order属性值是： 2147483647
spring会先执行order值比较小的。当访问一个a.jpg图片文件时，先通过 DefaultAnnotationHandlerMapping 来找处理器，一定是找不到的，因为我们没有叫a.jpg的Action。然后再按order值升序找，由于最后一个 SimpleUrlHandlerMapping 是匹配 "/**"的，所以一定会匹配上，就可以响应图片。
访问一个图片，还要走层层匹配。不知性能如何？
最后再说明一下，方案二、方案三 在访问静态资源时，如果有匹配的(近似)总拦截器，就会走拦截器。如果你在拦截中实现权限检查，要注意过滤这些对静态文件的请求。
如何你的DispatcherServlet拦截 *.do这样的URL后缀，就不存上述问题了。还是有后缀方便。
