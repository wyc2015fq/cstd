# Tomcat中文乱码处理之URIEncoding、useBodyEncodingForURI及CharacterEncodingFilter - 零度的博客专栏 - CSDN博客
2016年06月08日 16:15:00[零度anngle](https://me.csdn.net/zmx729618)阅读数：2265
       大家知道tomcat5.0开始，对网页的中文字符的post或者get，经常会出现乱码现象。具体是因为Tomcat默认是按ISO-8859-1进行URL解码，ISO-8859-1并未包括中文字符，这样的话中文字符肯定就不能被正确解析了。常见的解决方法是在tomcat的server.xml下的connetor属性中增加URIEncoding或者useBodyEncodingForURI属性。但是，这两种方式有什么区别呢？
       简单谈一下自己的理解：
       按照tomcat-docs/config/http.html文档的说明：
       URIEncoding：This specifies the character encoding used to decode the URI bytes, after %xx decoding the URL. If not specified, ISO-8859-1 will be used. 
       useBodyEncodingForURI：This specifies if the encoding specified in contentType should be used for URI query parameters, instead of using the URIEncoding.
       也就是说，useBodyEncodingForURI参数表示是否用request.setCharacterEncoding参数对URL提交的数据和表单中GET方式提交的数据进行重新编码，默认情况下，该参数为false。URIEncoding参数指定对所有GET方式请求进行统一的重新编码（解码）的编码。
        URIEncoding和useBodyEncodingForURI区别是：URIEncoding是对所有GET方式的请求的数据进行统一的重新编码； 而useBodyEncodingForURI则是根据响应该请求的页面的request.setCharacterEncoding参数对数据进行的重新编码，不同的页面可以有不同的重新编码的编码。
下面详细分析Tomcat的两个参数设置URIEncoding和useBodyEncodingForURI。 对于一个请求，常用的有两种编码方式，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <!DOCTYPE html>  
- <html>  
-     <head>  
-         <meta charset="utf-8" />  
-         <title></title>  
-     </head>  
-     <body>  
-         <form action="http://127.0.0.1:8080/string?name=中国" method="post">  
-             <input type="text" name="user" value="张三"/>  
-             <input type="submit" value="提交"/>  
-         </form>  
-     </body>  
- </html>  
       首先说说结论： 
**       上述请求有两处含有中文，一处是请求参数中，即?name='中国'，另一处是请求体中，即user='张三'。对于这两处tomcat7是分两种编码方式的。URIEncoding就是针对请求参数的编码设置的，而filter的request.setCharacterEncoding('UTF-8')或者请求header中的content-type中的编码都是针对请求体的。 **
useBodyEncodingForURI=true是说，请求参数的编码方式要采用请求体的编码方式。当useBodyEncodingForURI=true时，若请求体采用utf-8解析，则请求参数也要采用utf-8来解析。这两个属性值的设置在tomcat的conf/server.xml文件中配置，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <Service name="Catalina">  
- 
-    <!--The connectors can use a shared executor, you can define one or more named thread pools-->  
-    <!--  
-    <Executor name="tomcatThreadPool" namePrefix="catalina-exec-"
-        maxThreads="150" minSpareThreads="4"/>  
-    -->  
- 
- 
-    <!-- A "Connector" represents an endpoint by which requests are received  
-         and responses are returned. Documentation at :  
-         Java HTTP Connector: /docs/config/http.html (blocking & non-blocking)  
-         Java AJP  Connector: /docs/config/ajp.html  
-         APR (HTTP/AJP) Connector: /docs/apr.html  
-         Define a non-SSL HTTP/1.1 Connector on port 8080
-    -->  
-    <Connector port="8080" protocol="HTTP/1.1"
-               connectionTimeout="20000"
-               redirectPort="8443" useBodyEncodingForURI='true' URIEncoding='UTF-8' />  
-    <!-- A "Connector" using the shared thread pool-->  
        这样写只是说明这两者的配置位置，并不是两个属性要同时配置，不要理解错了。 继续说CharacterEncodingFilter的作用。 使用方式，将如下代码加入web.xml文件中：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <filter>  
-         <filter-name>encoding</filter-name>  
-         <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>  
-         <init-param>  
-             <param-name>encoding</param-name>  
-             <param-value>UTF-8</param-value>  
-         </init-param>  
-         <init-param>  
-             <param-name>forceEncoding</param-name>  
-             <param-value>true</param-value>  
-         </init-param>  
-     </filter>  
- 
-     <filter-mapping>  
-         <filter-name>encoding</filter-name>  
-         <url-pattern>/*</url-pattern>  
-     </filter-mapping>  
       作用是，当forceEncoding为false的前提下（默认为false），当request没有指定content-type或content-type不含编码时，该filter将会为这个request设定请求体的编码为filter的encoding值。 当forceEncoding为true的前提下，就会为request的请求体和response都设定为这个filter的encoding值。 
CharacterEncodingFilter源码如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass CharacterEncodingFilter extends OncePerRequestFilter {  
- 
- private String encoding;  
- 
- privateboolean forceEncoding = false;  
- 
- 
- /**
-      * Set the encoding to use for requests. This encoding will be passed into a
-      * {@link javax.servlet.http.HttpServletRequest#setCharacterEncoding} call.
-      * <p>Whether this encoding will override existing request encodings
-      * (and whether it will be applied as default response encoding as well)
-      * depends on the {@link #setForceEncoding "forceEncoding"} flag.
-      */
- publicvoid setEncoding(String encoding) {  
- this.encoding = encoding;  
-     }  
- 
- /**
-      * Set whether the configured {@link #setEncoding encoding} of this filter
-      * is supposed to override existing request and response encodings.
-      * <p>Default is "false", i.e. do not modify the encoding if
-      * {@link javax.servlet.http.HttpServletRequest#getCharacterEncoding()}
-      * returns a non-null value. Switch this to "true" to enforce the specified
-      * encoding in any case, applying it as default response encoding as well.
-      * <p>Note that the response encoding will only be set on Servlet 2.4+
-      * containers, since Servlet 2.3 did not provide a facility for setting
-      * a default response encoding.
-      */
- publicvoid setForceEncoding(boolean forceEncoding) {  
- this.forceEncoding = forceEncoding;  
-     }  
- 
- 
- @Override
- protectedvoid doFilterInternal(  
-             HttpServletRequest request, HttpServletResponse response, FilterChain filterChain)  
- throws ServletException, IOException {  
- 
- if (this.encoding != null && (this.forceEncoding || request.getCharacterEncoding() == null)) {  
-             request.setCharacterEncoding(this.encoding);  
- if (this.forceEncoding) {  
-                 response.setCharacterEncoding(this.encoding);  
-             }  
-         }  
-         filterChain.doFilter(request, response);  
-     }  
- 
- }  
        这个filter有两个属性,encoding和forceEncoding，我们可以在web.xml文件中来设定这两个属性值。 
        每次request请求到来执行方法doFilterInternal，首先调用request.getCharacterEncoding()，本质就是从请求header content-type中获取编码值，如果没有，则调用request.setCharacterEncoding(this.encoding)将该filter的encoding值设置为请求体的编码方式，记住该编码方式只对请求体，不针对请求参数。当forceEncoding=true时，不管请求header content-type有没有编码方式，始终将该filter的encoding值设置到request和response中，同样此设置只针对request的请求体。 
