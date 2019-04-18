# Struts2 静态资源映射 - z69183787的专栏 - CSDN博客
2015年08月27日 12:53:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4381
在struts2框架中有一些它所需要的公共的静态内容，比如说js文件和一些css文件。当框架需要这些静态内容的时候，FilterDidpatcher会自动提供给我们。那么FilterDidpatcher是如何知道我们在请求静态内容的呢？任何请求只要以“/struts/”开头，那么FilterDidpatcher就会认为它是在请求静态内容。在识别出请求是请求静态资源后FilterDidpatcher如何去匹配寻找静态资源呢？这里有两个关键点：
1．确定所要请求的资源路径。FilterDidpatcher会截取/struts/后面的内容作为所要请求的资源。比如说现在请求是/struts/xhtml/styles.css，那么FilterDidpatcher就会把xhtml/styles.css作为我们所要请求的资源的路径：xhtml目录下面的styles.css文件。
2．到哪儿去寻找所请求的静态内容。默认情况下FilterDidpatcher会随意的org.apache.struts2.static和template这两个包中去寻找。如果我们还想在别的其它包中寻找静态内容的话，那就需要在web.xml配置中FilterDidpatcher时，给它添加一个参数”packages”，然后把它的值设置为一系列以逗号或者空格分隔的包名，如下面所示：
<filter>
 <filter-name>Struts2</filter-name>
<filter-class>org.apache.struts2.dispatcher.FilterDispatcher</filter-class>
<init-param>
 <param-name>packages</param-name>
 <param-value>com.mangocity.static,hust.cm</param-value>
</init-param>
 </filter>
 <filter-mapping>
 <filter-name>Struts2</filter-name>
 <url-pattern>/*</url-pattern>
 </filter-mapping>
    描述：web应用下有一个目录“static”，现在要访问其中的“top.html”文件，即访问“localhost:8080/static/top.html”，服务器总是抱404错误。 
    原因：在struts2的FilterDispatcher类的doFilter方法中，如果请求的是静态资源，struts2会判断该请求是否可以处理，这里的代码如下： 
Java代码   ![收藏代码](http://static.oschina.net/uploads/img/201307/06205655_F4Mm.png)
- String resourcePath = RequestUtils.getServletPath(request);  
- if ("".equals(resourcePath) && null != request.getPathInfo()) {  
-     resourcePath = request.getPathInfo();  
- }  
- if (staticResourceLoader.canHandle(resourcePath)) {  
-     staticResourceLoader.findStaticResource(resourcePath, request, response);  
- } else {  
- // this is a normal request, let it pass through
-      chain.doFilter(request, response);  
- }  
- // The framework did its job here
- return;  
    其中，在DefaultStaticContentLoader类的canHandle方法中会对请求路径进行判断： 
Java代码   ![收藏代码](http://static.oschina.net/uploads/img/201307/06205655_F4Mm.png)
- publicboolean canHandle(String resourcePath) {  
- return serveStatic &&   
-       (resourcePath.startsWith("/struts") || resourcePath.startsWith("/static"));  
- }  
    这里，serveStatic的值为true，再加上要访问的资源以“/static”开头，所以这里返回true。 
    然后，会进入DefaultStaticContentLoader类的findStaticResource方法，该方法的第一行语句是： 
Java代码   ![收藏代码](http://static.oschina.net/uploads/img/201307/06205655_F4Mm.png)
- String name = cleanupPath(path);  
    这里，cleanupPath方法的定义如下： 
Java代码   ![收藏代码](http://static.oschina.net/uploads/img/201307/06205655_F4Mm.png)
- /**
-  * @param path requested path
-  * @return path without leading "/struts" or "/static"
-  */
- protected String cleanupPath(String path) {  
- //path will start with "/struts" or "/static", remove them
- return path.substring(7);  
- }  
    struts2把“/static”截掉了，这样，后面再进行解析的时候，就变成了解析对“/top.html”的请求，所以会报404错误。 
    总结：悲剧的错误，还以为是自己程序的bug，改了半天。需要加强对开源程序中具体实现的了解。 
