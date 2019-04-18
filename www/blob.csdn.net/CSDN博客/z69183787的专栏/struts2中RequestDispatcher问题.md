# struts2中RequestDispatcher问题 - z69183787的专栏 - CSDN博客
2015年07月18日 20:51:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3155
web.xml 2.4版本里，默认的filter只拦截request。 
如果使用request.getRequestDispatcher("xx").forward(request,response)，属于服务器内部forward，不会被拦截器拦截，所以你需要为filter配置拦截forward才行。 
struts1用的是servlet，forward也会起作用。struts2里的filter的确是个大问题哟。
用filter配置拦截fowward要写成
    <filter>
        <filter-name>struts2</filter-name>
        <filter-class>org.apache.struts2.dispatcher.FilterDispatcher</filter-class>
    </filter>
    <filter-mapping>
        <filter-name>struts2</filter-name>
        <url-pattern>/*</url-pattern>
        <dispatcher>REQUEST</dispatcher>
<dispatcher>FORWARD</dispatcher>
   </filter-mapping>
在servlet
 2.4之后的规范中，加入了对 filter-mapping的dispatch的规范。 
意思就是 什么样的请求 会被过滤。 
除了通过路径之外还有另外的几种方式。 
<filter-mapping> 
        <filter-name>struts2Filter</filter-name> 
        <url-pattern>*.action</url-pattern> 
        <dispatcher>REQUEST</dispatcher> 
        <dispatcher>FORWARD</dispatcher> 
</filter-mapping> 
这个意思就是，以*.action结尾的请求，并且是通过request
 dispatcher的forward方法传递过来或者直接从客户端传递过来的，则必须经过这个过滤器。 
除了这个之外， 
filter 
可以作用于直接从客户端过来的request，对应dispatch为REQUEST，默认是这个 
通过forward过来的request，lz的例子就是这个意思。 
通过include过来的request，处理通过request dispatcher的include方法传递过来的请求。 
通过<error-page>过来的request。 
具体LZ可以看下servlet 2.4的specification。 
一般都是不写，使用默认值，也就是request过来的请求。 
