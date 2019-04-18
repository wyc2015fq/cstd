# SpringMVC源码总结（五）Tomcat的URIEncoding、useBodyEncodingForURI和CharacterEncodingFilter - z69183787的专栏 - CSDN博客
2016年10月14日 16:34:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：554
个人分类：[SpringMvc-原理分析](https://blog.csdn.net/z69183787/article/category/6456963)
继续上一章节的乱码问题。上一篇文章仅仅说了设置Tomcat的URIEncoding可以解决乱码问题，这篇文章便会讲述这一背后的内容。首先说明下，光看是没用的，要多实验实验。 
目前我的tomcat版本为：7.0.55，spring所有文章的版本始终为4.0.5 
本文章会从tomcat的源码角度来解析Tomcat的两个参数设置URIEncoding和useBodyEncodingForURI。 
对于一个请求，常用的有两种编码方式，如下：
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
上述请求有两处含有中文，一处是请求参数中，即？name='中国'，另一处是请求体中，即user='张三'。对于这两处tomcat7是分两种编码方式的。URIEncoding就是针对请求参数的编码设置的，而filter的request.setCharacterEncoding('UTF-8')或者请求header中的content-type中的编码都是针对请求体的。不要把他们搞混了。 
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
这样写只是说明这两者的配置位置，并不是两个属性要同时配置，不要理解错了。 
继续说说CharacterEncodingFilter的作用。 
使用方式，将如下代码加入web.xml文件中：
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
作用是，当forceEncoding为false的前提下（默认为false），当request没有指定content-type或content-type不含编码时，该filter将会为这个request设定请求体的编码为filter的encoding值。 
当forceEncoding为true的前提下，就会为request的请求体和response都设定为这个filter的encoding值。 
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
每次request请求到来执行方法doFilterInternal，首先调用request.getCharacterEncoding()，本质就是从请求header content-type中获取编码值，如果没有，则调用request.setCharacterEncoding(this.encoding)将该filter的encoding值设置为请求体的编码方式，记住该编码方式只对请求体，不针对请求参数。当forceEncoding=true时，不管请求header content-type有没有编码方式，始终将该filter的encoding值设置到request和response中，同样只针对request的请求体。 
以上的结论说完了，下面就要看看源代码了。不想看的就算了不影响使用，想看看原理的请继续： 
首先是三个名词： 
org.apache.coyote.Request：这是一个最底层的request，包含各种参数信息。暂且称为coyoteRequest。 
org.apache.catalina.connector.Request：实现了HttpServletRequest接口，称它为request，同时包含了一个coyoteRequest，一个connector,待会你就会发现这个connector的编码传递作用。 
org.apache.catalina.connector.RequestFacade：同样实现了HttpServletRequest接口，它仅仅是一个装饰类，称它为requestFacade，构造函数为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /**
-      * Construct a wrapper for the specified request.
-      *
-      * @param request The request to be wrapped
-      */
- public RequestFacade(Request request) {  
- 
- this.request = request;  
- 
-     }  
该构造函数将一个org.apache.catalina.connector.Request传进来，requestFacade的工作全是靠它内部的org.apache.catalina.connector.Request来完成的，org.apache.catalina.connector.Request又是依据它所包含的org.apache.coyote.Request这个最底层的类来完成的。通过org.apache.catalina.connector.Request，我们可以设定org.apache.coyote.Request的一些工作方式，如通过什么编码来解析数据。 
org.apache.coyote.Request含有的属性： 
String charEncoding：针对请求体的编码(在第一次解析参数时会传递给Parameters的encoding) 
Parameters ：用于处理和存放请求参数和请求体参数的类 
            （1）含String encoding：针对请求体的编码 
            （2）含String queryStringEncoding：针对请求参数的编码 
            （3）含Map<String,ArrayList<String>> paramHashValues：存放解析后的参数 
Parameters的两个编码是最最重要的编码，直接参与解析数据的编码，不像其他对象的编码大部分都是起传递作用，最终作用到Parameters的两个编码上
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass MyCharacterEncodingFilter extends CharacterEncodingFilter{  
- 
- @Override
- protectedvoid doFilterInternal(HttpServletRequest request,  
-             HttpServletResponse response, FilterChain filterChain)  
- throws ServletException, IOException {  
-         request.setCharacterEncoding("UTF-8");  
-         String name=request.getParameter("user");  
-         System.out.println(name);  
-         request.setCharacterEncoding("UTF-8");  
-         String name1=request.getParameter("user");  
-         System.out.println(name1);  
- super.doFilterInternal(request, response, filterChain);  
-     }  
- }  
传给过滤器filter的HttpServletRequest request其实是org.apache.catalina.connector.RequestFacade类型的，我们看下获取参数的具体过程： 
requestFacade.getParameter("user")会传递到org.apache.catalina.connector.Request的相应方法，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- public String getParameter(String name) {  
- 
- if (!parametersParsed) {  
-             parseParameters();  
-         }  
- 
- return coyoteRequest.getParameters().getParameter(name);  
- 
-     }  
parametersParsed是org.apache.catalina.connector.Request的属性，用于标示是否已经解析过参数，如果解析过，便不再解析，直接从coyoteRequest的Parameters参数中取出。所以当已经解析过后，你再去设置编码，会无效的，因为它会直接返回第一次的解析结果。并且解析过程仅仅发生在第一次获取参数的时候。 
我们来看下parseParameters()这个解析参数的过程：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /**
-      * Parse request parameters.
-      */
- protectedvoid parseParameters() {  
- 
- //解析发生后，便将是状态置为已解析
-         parametersParsed = true;  
- 
-         Parameters parameters = coyoteRequest.getParameters();  
- boolean success = false;  
- try {  
- // Set this every time in case limit has been changed via JMX
-             parameters.setLimit(getConnector().getMaxParameterCount());  
- 
- // getCharacterEncoding() may have been overridden to search for
- // hidden form field containing request encoding
- //重点1
-             String enc = getCharacterEncoding();  
- //重点2
- boolean useBodyEncodingForURI = connector.getUseBodyEncodingForURI();  
- if (enc != null) {  
-                 parameters.setEncoding(enc);  
- if (useBodyEncodingForURI) {  
-                     parameters.setQueryStringEncoding(enc);  
-                 }  
-             } else {  
-                 parameters.setEncoding  
-                     (org.apache.coyote.Constants.DEFAULT_CHARACTER_ENCODING);  
- if (useBodyEncodingForURI) {  
-                     parameters.setQueryStringEncoding  
-                         (org.apache.coyote.Constants.DEFAULT_CHARACTER_ENCODING);  
-                 }  
-             }  
- //重点3
-             parameters.handleQueryParameters();  
- 
- if (usingInputStream || usingReader) {  
-                 success = true;  
- return;  
-             }  
- 
- if( !getConnector().isParseBodyMethod(getMethod()) ) {  
-                 success = true;  
- return;  
-             }  
- 
-             String contentType = getContentType();  
- if (contentType == null) {  
-                 contentType = "";  
-             }  
- int semicolon = contentType.indexOf(';');  
- if (semicolon >= 0) {  
-                 contentType = contentType.substring(0, semicolon).trim();  
-             } else {  
-                 contentType = contentType.trim();  
-             }  
- 
- if ("multipart/form-data".equals(contentType)) {  
-                 parseParts();  
-                 success = true;  
- return;  
-             }  
- 
- if (!("application/x-www-form-urlencoded".equals(contentType))) {  
-                 success = true;  
- return;  
-             }  
- 
- int len = getContentLength();  
- 
- if (len > 0) {  
- int maxPostSize = connector.getMaxPostSize();  
- if ((maxPostSize > 0) && (len > maxPostSize)) {  
- if (context.getLogger().isDebugEnabled()) {  
-                         context.getLogger().debug(  
-                                 sm.getString("coyoteRequest.postTooLarge"));  
-                     }  
-                     checkSwallowInput();  
- return;  
-                 }  
- byte[] formData = null;  
- if (len < CACHED_POST_LEN) {  
- if (postData == null) {  
-                         postData = newbyte[CACHED_POST_LEN];  
-                     }  
-                     formData = postData;  
-                 } else {  
-                     formData = newbyte[len];  
-                 }  
- try {  
- if (readPostBody(formData, len) != len) {  
- return;  
-                     }  
-                 } catch (IOException e) {  
- // Client disconnect
- if (context.getLogger().isDebugEnabled()) {  
-                         context.getLogger().debug(  
-                                 sm.getString("coyoteRequest.parseParameters"), e);  
-                     }  
- return;  
-                 }  
- //重点4
-                 parameters.processParameters(formData, 0, len);  
-             } elseif ("chunked".equalsIgnoreCase(  
-                     coyoteRequest.getHeader("transfer-encoding"))) {  
- byte[] formData = null;  
- try {  
-                     formData = readChunkedPostBody();  
-                 } catch (IOException e) {  
- // Client disconnect or chunkedPostTooLarge error
- if (context.getLogger().isDebugEnabled()) {  
-                         context.getLogger().debug(  
-                                 sm.getString("coyoteRequest.parseParameters"), e);  
-                     }  
- return;  
-                 }  
- if (formData != null) {  
-                     parameters.processParameters(formData, 0, formData.length);  
-                 }  
-             }  
-             success = true;  
-         } finally {  
- if (!success) {  
-                 parameters.setParseFailed(true);  
-             }  
-         }  
- 
-     }  
上面有四处我们需要关注的重点。 
重点1：getCharacterEncoding()其实是通过底层的coyoteRequest来获取header content-type中的编码。 
如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /**
-      * Return the character encoding for this Request.
-      */
- @Override
- public String getCharacterEncoding() {  
- return coyoteRequest.getCharacterEncoding();  
-     }  
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- public String getCharacterEncoding() {  
- 
- if (charEncoding != null)  
- return charEncoding;  
- 
-        charEncoding = ContentType.getCharsetFromContentType(getContentType());  
- return charEncoding;  
- 
-    }  
若无，则返回空。 
重点2： 
boolean useBodyEncodingForURI = connector.getUseBodyEncodingForURI();这里就是我们在tomcat的server中配置的useBodyEncodingForURI属性的值。 
常量值org.apache.coyote.Constants.DEFAULT_CHARACTER_ENCODING="ISO-8859-1"； 
当重点1中的enc为空时，则会设置底层coyoteRequest的parameters对象的encoding=s上述"ISO-8859-1"，即请求体采用"ISO-8859-1"来解析。当useBodyEncodingForURI=true时，请求参数和请求体的编码设置的都是同一个值，即"ISO-8859-1"。当useBodyEncodingForURI=false时，不改变queryStringEncoding即请求参数的编码，queryStringEncoding默认是为null的，当解析时碰见queryStringEncoding也会采用默认的编码"ISO-8859-1"，然而我们可以通过org.apache.catalina.connector.Request所包含的connector配置来给queryStringEncoding赋值。如下： 
当你在tomcat的server.xml文件中加入URIEncoding="UTF-8"时，它将会为queryStringEncoding赋值此值。 
在tomcat的server.xml中配置此值
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <Service name="Catalina">  
- 
-     <!--The connectors can use a shared executor, you can define one or more named thread pools-->  
-     <!--  
-     <Executor name="tomcatThreadPool" namePrefix="catalina-exec-"
-         maxThreads="150" minSpareThreads="4"/>  
-     -->  
- 
- 
-     <!-- A "Connector" represents an endpoint by which requests are received  
-          and responses are returned. Documentation at :  
-          Java HTTP Connector: /docs/config/http.html (blocking & non-blocking)  
-          Java AJP  Connector: /docs/config/ajp.html  
-          APR (HTTP/AJP) Connector: /docs/apr.html  
-          Define a non-SSL HTTP/1.1 Connector on port 8080
-     -->  
-     <Connector port="8080" protocol="HTTP/1.1"
-                connectionTimeout="20000"
-                redirectPort="8443" URIEncoding='UTF-8'/>  
connector将这个值为queryStringEncoding赋值的过程如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicvoid log(org.apache.coyote.Request req,  
-            org.apache.coyote.Response res, long time) {  
- 
-        Request request = (Request) req.getNote(ADAPTER_NOTES);  
-        Response response = (Response) res.getNote(ADAPTER_NOTES);  
- 
- if (request == null) {  
- // Create objects
-            request = connector.createRequest();  
-            request.setCoyoteRequest(req);  
-            response = connector.createResponse();  
-            response.setCoyoteResponse(res);  
- 
- // Link objects
-            request.setResponse(response);  
-            response.setRequest(request);  
- 
- // Set as notes
-            req.setNote(ADAPTER_NOTES, request);  
-            res.setNote(ADAPTER_NOTES, response);  
- 
- // Set query string encoding
- //重点重点重点重点重点重点重点重点重点重点重点重点重点重点重点
-            req.getParameters().setQueryStringEncoding  
-                (connector.getURIEncoding());  
-        }  
connector.getURIEncoding()便是我们配置的URIEncoding值 
req.getParameters().setQueryStringEncoding 
                (connector.getURIEncoding()); 
这句代码便是将我们在tomcat的server.xml文件中配置的URIEncoding值设置进最重要的Parameters的queryStringEncoding中。 
当重点1中的enc不为空时,为Parameters请求体的的编码encoding设置为enc。 
至此，Parameters的encoding和queryStringEncoding都有相应的值了，然后便按照对应的编码来解析字节数组。 
重点3和4：有个相应的编码方式，分别执行请求参数的解析过程和请求体的解析过程。 
总结下一些设置的作用： 
request.setCharacterEncoding(encoding) ：暴漏给我们的request为requestFacade，最终调用request->调用coyoteRequest->设置到coyoteRequest的charEncoding，所以coyoteRequest的charEncoding有两种来源，一种可能是content-type中的编码，另一种就是调用request.setCharacterEncoding(encoding) 方法。此方法最好在第一次解析参数之前调用，不然就无效。 
URIEncoding：直接设置Parameters的queryStringEncoding的值。即针对请求参数的编码。 
useBodyEncodingForURI：设置queryStringEncoding的值=encoding的值，即请求参数采用请求体的编码方式。 
