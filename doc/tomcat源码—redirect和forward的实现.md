# tomcat源码—redirect和forward的实现





网上已经有很多关于redirect和forward区别的文章，更多的都是只是一些概念上的描述，虽然在大多情况下，知道这些就已经足够了。但也有例外：forward not working for struts2，why？我也是在工作中碰到了这个问题，才特意看了下tomcat有关这部分的源代码。深刻的了解下也无妨。
redirect和forward都是属于servlet规范的，不同的servlet容器的实现可能会有一些区别，但原理都是类似的。

## redirect和forward的定义：

1. redirect（重定向）：服务端发送给客户端一个重定向的临时响应头，这个响应头包含重定向之后的URL，客户端用新的URL重新向服务器发送一个请求。
2. forward（请求转向）：服务器程序内部请求转向，这个特性允许前一个程序用于处理请求，而后一个程序用来返回响应。

Redirect的原理比较简单，它的定义也已经描述的很清楚了，我也不想多讲什么，就贴一段简单的代码吧！

org.apache.catalina.connector.Response#sendRedirect(String)：

```Java

     public void sendRedirect(String location)   
            throws IOException {  
      
            if (isCommitted())  
                throw new IllegalStateException  
                    (sm.getString("coyoteResponse.sendRedirect.ise"));  
      
            // Ignore any call from an included servlet  
            if (included)  
                return;   
      
            // Clear any data content that has been buffered  
            resetBuffer();  
      
            // Generate a temporary redirect to the specified location  
            try {  
                String absolute = toAbsolute(location);  
                setStatus(SC_FOUND);  
                setHeader("Location", absolute);  
            } catch (IllegalArgumentException e) {  
                setStatus(SC_NOT_FOUND);  
            }  
      
            // Cause the response to be finished (from the application perspective)  
            setSuspended(true);  
    }  
```

方法行为：先把相对路径转换成绝对路径，再包装一个包含有新的URL的临时响应头，“SC_FOUND”的值是302，就是重定向临时响应头的状态码。如果传入的“location”值不合法，就包装一个404的响应头。

下面就来看看tomcat是如何实现forward的，forward为什么在struts2下会无效(注解：其实是可以设置的)。

先看下程序是如何调用forward的：
```Java

    req.getRequestDispatcher("testForward").forward(req, resp);  
```

整个过程分两个步骤来执行
1. 得到一个请求调度器
2. 通过调度器把请求转发过去。


第一步骤，获取请求调度器。
org.apache.catalina.connector.Request#getRequestDispatcher(String)
```Java

          
    public RequestDispatcher getRequestDispatcher(String path) {  
      
            if (request == null) {  
                throw new IllegalStateException(  
                                sm.getString(&quot;requestFacade.nullRequest&quot;));  
            }  
      
            if (Globals.IS_SECURITY_ENABLED){  
                return (RequestDispatcher)AccessController.doPrivileged(  
                    new GetRequestDispatcherPrivilegedAction(path));  
            } else {  
                 return request.getRequestDispatcher(path);    
            }  
```

方法行为：把获取RequestDispatcher的任务交个内部的request。它们之间的关系如下所示




org.apache.catalina.connector.RequestFacade和类org.apache.catalina.connector.Request都是实现了javax.servlet.http.HttpServletRequest接口，而RequestFacade内部有包装了个Request，对Request的访问做了些控制，应该是代理模式

org.apache.catalina.connector.Request#getRequestDispatcher(String)
```Java

    public RequestDispatcher getRequestDispatcher(String path) {  
             if (path.startsWith("/"))  
               return (context.getServletContext().getRequestDispatcher(path));  
      
           //省略了部分代码  
           return (context.getServletContext().getRequestDispatcher(relative));   
      
       }  
```

方法行为：把绝对路径转换成相对路径，最终的格式如“/testForward”。若已经是这种格式的相对路径，就无需再转换了。
接下来就转交给ServletContext来处理，ServletContext是web项目的一个上下文，包含所有的Servlet集合，还定义了一些Servlet与容器之间交互的接口。
org.apache.catalina.core.ApplicationContext#getRequestDispatcher(String)

```Java

    public RequestDispatcher getRequestDispatcher(String path) {  
              //省去部分代码  
              context.getMapper().map(uriMB, mappingData);  
              //省去部分代码  
          Wrapper wrapper = (Wrapper) mappingData.wrapper;  
          String wrapperPath = mappingData.wrapperPath.toString();  
          String pathInfo = mappingData.pathInfo.toString();  
      
          mappingData.recycle();  
            
          // Construct a RequestDispatcher to process this request  
          return new ApplicationDispatcher  
              (wrapper, uriCC.toString(), wrapperPath, pathInfo,   
               queryString, null);   
      }  
```

方法行为：根据路径名“path”找到一个包含有Servlet的Wrapper，最后实例化一个ApplicationDispatcher，并且返回该ApplicationDispatcher。

该方法里非常关键的一行：context.getMapper().map(uriMB, mappingData)。
Mapper的类定义我不知道如何描述，就贴上原文吧：Mapper, which implements the servlet API mapping rules (which are derived from the HTTP rules)。
不过只想了解forward的原理，熟悉map函数就够了。

org.apache.tomcat.util.http.mapper.Mapper#map(org.apache.tomcat.util.buf.MessageBytes, org.apache.tomcat.util.http.mapper.MappingData)：
```Java

    public void map(MessageBytes uri, MappingData mappingData)  
        throws Exception {  
      
        uri.toChars();  
        CharChunk uricc = uri.getCharChunk();  
        uricc.setLimit(-1);  
        internalMapWrapper(context, uricc, mappingData);  
      
    }  
```

方法行为：。。。。。。。就介绍下参数吧，uri可以理解是path(“/testforward”)的一个变形,而mappingData用于存储当前线程用到的部分数据。该函数是没有返回值的，处理之后的结果就是存放到mappingData里的。

org.apache.tomcat.util.http.mapper.Mapper#internalMapWrapper(Mapper$Context,org.apache.tomcat.util.buf.CharChunk, org.apache.tomcat.util.http.mapper.MappingData)：
```Java

    private final void internalMapWrapper(Context context, CharChunk path,  
                                             MappingData mappingData)  
           throws Exception {  
      
           int pathOffset = path.getOffset();  
           int pathEnd = path.getEnd();  
           int servletPath = pathOffset;  
           boolean noServletPath = false;  
      
           int length = context.name.length();  
           if (length != (pathEnd - pathOffset)) {  
               servletPath = pathOffset + length;  
           } else {  
               noServletPath = true;  
               path.append('/');  
               pathOffset = path.getOffset();  
               pathEnd = path.getEnd();  
               servletPath = pathOffset+length;  
           }  
      
           path.setOffset(servletPath);  
      
           // Rule 1 -- Exact Match  
           Wrapper[] exactWrappers = context.exactWrappers;  
           internalMapExactWrapper(exactWrappers, path, mappingData);  
      
           // Rule 2 -- Prefix Match  
           boolean checkJspWelcomeFiles = false;  
           Wrapper[] wildcardWrappers = context.wildcardWrappers;  
           if (mappingData.wrapper == null) {  
               internalMapWildcardWrapper(wildcardWrappers, context.nesting,   
                                          path, mappingData);  
               if (mappingData.wrapper != null && mappingData.jspWildCard) {  
                   char[] buf = path.getBuffer();  
                   if (buf[pathEnd - 1] == '/') {  
                       /* 
                        * Path ending in '/' was mapped to JSP servlet based on 
                        * wildcard match (e.g., as specified in url-pattern of a 
                        * jsp-property-group. 
                        * Force the context's welcome files, which are interpreted 
                        * as JSP files (since they match the url-pattern), to be 
                        * considered. See Bugzilla 27664. 
                        */   
                       mappingData.wrapper = null;  
                       checkJspWelcomeFiles = true;  
                   } else {  
                       // See Bugzilla 27704  
                       mappingData.wrapperPath.setChars(buf, path.getStart(),  
                                                        path.getLength());  
                       mappingData.pathInfo.recycle();  
                   }  
               }  
           }  
      
           if(mappingData.wrapper == null && noServletPath) {  
               // The path is empty, redirect to "/"  
               mappingData.redirectPath.setChars  
                   (path.getBuffer(), pathOffset, pathEnd);  
               path.setEnd(pathEnd - 1);  
               return;  
           }  
      
           // Rule 3 -- Extension Match  
           Wrapper[] extensionWrappers = context.extensionWrappers;  
           if (mappingData.wrapper == null && !checkJspWelcomeFiles) {  
               internalMapExtensionWrapper(extensionWrappers, path, mappingData);  
           }  
      
           // Rule 4 -- Welcome resources processing for servlets  
           if (mappingData.wrapper == null) {  
               boolean checkWelcomeFiles = checkJspWelcomeFiles;  
               if (!checkWelcomeFiles) {  
                   char[] buf = path.getBuffer();  
                   checkWelcomeFiles = (buf[pathEnd - 1] == '/');  
               }  
               if (checkWelcomeFiles) {  
                   for (int i = 0; (i < context.welcomeResources.length)  
                            && (mappingData.wrapper == null); i++) {  
                       path.setOffset(pathOffset);  
                       path.setEnd(pathEnd);  
                       path.append(context.welcomeResources[i], 0,  
                                   context.welcomeResources[i].length());  
                       path.setOffset(servletPath);  
      
                       // Rule 4a -- Welcome resources processing for exact macth  
                       internalMapExactWrapper(exactWrappers, path, mappingData);  
      
                       // Rule 4b -- Welcome resources processing for prefix match  
                       if (mappingData.wrapper == null) {  
                           internalMapWildcardWrapper  
                               (wildcardWrappers, context.nesting,   
                                path, mappingData);  
                       }  
      
                       // Rule 4c -- Welcome resources processing  
                       //            for physical folder  
                       if (mappingData.wrapper == null  
                           && context.resources != null) {  
                           Object file = null;  
                           String pathStr = path.toString();  
                           try {  
                               file = context.resources.lookup(pathStr);  
                           } catch(NamingException nex) {  
                               // Swallow not found, since this is normal  
                           }  
                           if (file != null && !(file instanceof DirContext) ) {  
                               internalMapExtensionWrapper(extensionWrappers,  
                                                           path, mappingData);  
                               if (mappingData.wrapper == null  
                                   && context.defaultWrapper != null) {  
                                   mappingData.wrapper =  
                                       context.defaultWrapper.object;  
                                   mappingData.requestPath.setChars  
                                       (path.getBuffer(), path.getStart(),   
                                        path.getLength());  
                                   mappingData.wrapperPath.setChars  
                                       (path.getBuffer(), path.getStart(),   
                                        path.getLength());  
                                   mappingData.requestPath.setString(pathStr);  
                                   mappingData.wrapperPath.setString(pathStr);  
                               }  
                           }  
                       }  
                   }  
      
                   path.setOffset(servletPath);  
                   path.setEnd(pathEnd);  
               }  
                                             
           }  
```

方法行为：通过“path”从“context”里找到对应的Servlet，存放到“mappingData”里。
可以看到这里有7个匹配Servlet规则：
1. Rule 1 -- Exact Match:精确匹配，匹配web.xml配置的格式如“<url-pattern>/testQiu</url-pattern>”的Servlet
2. Rule 2 -- Prefix Matcha:前缀匹配，匹配的Servlet格式如“<url-pattern>/testQiu/*</url-pattern>”
3. Rule 3 -- Extension Match:扩展匹配，匹配jsp或者jspx
4. ---Rule 4a -- Welcome resources processing for exact macth:
5. ---Rule 4b -- Welcome resources processing for prefix match:
6. ---Rule 4c -- Welcome resources processing for physical folder:
7. Rule 7 --如果前面6条都没匹配到，那就返回org.apache.catalina.servlets.DefaultServlet。

其实这里真正的匹配的是Wapper，而不是Servlet，因为Wapper最重要的一个属性就是Servlet，说成“匹配Servlet”是为了更容易的表达。

至此返回RequestDispatcher就结束了。



接下来就是讲解RequestDispatcher.forward了。Forward的就不贴出全部的源代码，只贴一些重要的片段，绝大部分的逻辑都在org.apache.catalina.core.ApplicationDispatcher类里。
先描述下过程：
1. 设置request里的部分属性值，如：请求的路径、参数等。
2. 组装一个FilterChain链，调用doFilter方法。
3. 最后根据实际情况调用Filter的doFilter函数或者Servlet的service函数。

注：FilterChain和Filter是两个不同的接口，两个接口的UML

[点击查看原始大小图片]

org.apache.catalina.core.ApplicationDispatcher#doForward(ServletRequest,ServletResponse)：
```Java

    private void doForward(ServletRequest request, ServletResponse response)  
            throws ServletException, IOException  
             //省略了部分代码  
            // Handle an HTTP named dispatcher forward  
            if ((servletPath == null) && (pathInfo == null)) {  
    //省略了部分代码  
            } else {// Handle an HTTP path-based forward  
                ApplicationHttpRequest wrequest =  
                    (ApplicationHttpRequest) wrapRequest(state);  
                String contextPath = context.getPath();  
                HttpServletRequest hrequest = state.hrequest;  
                if (hrequest.getAttribute(Globals.FORWARD_REQUEST_URI_ATTR) == null) {  
                    wrequest.setAttribute(Globals.FORWARD_REQUEST_URI_ATTR,  
                                          hrequest.getRequestURI());  
                    wrequest.setAttribute(Globals.FORWARD_CONTEXT_PATH_ATTR,  
                                          hrequest.getContextPath());  
                    wrequest.setAttribute(Globals.FORWARD_SERVLET_PATH_ATTR,  
                                          hrequest.getServletPath());  
                    wrequest.setAttribute(Globals.FORWARD_PATH_INFO_ATTR,  
                                          hrequest.getPathInfo());  
                    wrequest.setAttribute(Globals.FORWARD_QUERY_STRING_ATTR,  
                                          hrequest.getQueryString());  
                }  
       
                wrequest.setContextPath(contextPath);  
                wrequest.setRequestURI(requestURI);  
                wrequest.setServletPath(servletPath);  
                wrequest.setPathInfo(pathInfo);  
                if (queryString != null) {  
                    wrequest.setQueryString(queryString);  
                    wrequest.setQueryParams(queryString);  
                }  
      
                processRequest(request,response,state);  
            }  
            }  
    //省略了部分代码  
        }  
```

第1步：设置新的request的属性：
         
```Java

    wrequest.setContextPath(contextPath);  
              wrequest.setRequestURI(requestURI);  
              wrequest.setServletPath(servletPath);  
              wrequest.setPathInfo(pathInfo);  
              if (queryString != null) {  
                  wrequest.setQueryString(queryString);  
                  wrequest.setQueryParams(queryString);  
              }  
```



第2步：组装FitlerChain链，根据web.xml配置信息，是否决定添加Filter----
```
<filter-mapping>
<filter-name>struts2</filter-name>
<url-pattern>/*</url-pattern>
<dispatcher>REQUEST</dispatcher>
</filter-mapping>
```

org.apache.catalina.core.ApplicationFilterFactory#createFilterChain(ServletRequest, Wrapper, Servlet)：
```Java

    public ApplicationFilterChain createFilterChain(ServletRequest request, Wrapper wrapper, Servlet servlet) {  
            //省略部分代码  
                filterChain = new ApplicationFilterChain();  
            }  
      
            filterChain.setServlet(servlet);  
      
            filterChain.setSupport  
                (((StandardWrapper)wrapper).getInstanceSupport());  
      
            // Acquire the filter mappings for this Context  
            StandardContext context = (StandardContext) wrapper.getParent();  
            FilterMap filterMaps[] = context.findFilterMaps();  
      
            // If there are no filter mappings, we are done  
            if ((filterMaps == null) || (filterMaps.length == 0))  
                return (filterChain);  
      
            // Acquire the information we will need to match filter mappings  
            String servletName = wrapper.getName();  
      
            // Add the relevant path-mapped filters to this filter chain  
            for (int i = 0; i < filterMaps.length; i++) {  
                if (!matchDispatcher(filterMaps[i] ,dispatcher)) {  
                    continue;  
                }  
                if (!matchFiltersURL(filterMaps[i], requestPath))  
                    continue;  
                ApplicationFilterConfig filterConfig = (ApplicationFilterConfig)  
                    context.findFilterConfig(filterMaps[i].getFilterName());  
                if (filterConfig == null) {  
                    ;       // FIXME - log configuration problem  
                    continue;  
                }  
                boolean isCometFilter = false;  
                if (comet) {  
                    try {  
                        isCometFilter = filterConfig.getFilter() instanceof CometFilter;  
                    } catch (Exception e) {  
                        // Note: The try catch is there because getFilter has a lot of   
                        // declared exceptions. However, the filter is allocated much  
                        // earlier  
                    }  
                    if (isCometFilter) {  
                        filterChain.addFilter(filterConfig);  
                    }  
                } else {  
                    filterChain.addFilter(filterConfig);  
                }  
            }  
      
           //省略部分代码  
      
            // Return the completed filter chain  
            return (filterChain);  
      
    }  
```



如果是<dispatcher>REQUEST</dispatcher>，那就不添加Filter，默认设置是REQUEST
如果是<dispatcher>FORWARD</dispatcher>，添加Filter到FilterChain。

第3步：调用doFilter或者service,代码删减了很多。

org.apache.catalina.core.ApplicationFilterChain#doFilter(ServletRequest, ServletResponse)：
```Java

      public void doFilter(ServletRequest request, ServletResponse response)throws IOException, ServletException {  
                internalDoFilter(request,response);  
      }  
      
      
    org.apache.catalina.core.ApplicationFilterChain#internalDoFilter(ServletRequest, ServletResponse)  
    private void internalDoFilter(ServletRequest request,   
                                      ServletResponse response)  
            throws IOException, ServletException {  
      
            // Call the next filter if there is one  
            if (pos < n) {  
                        filter.doFilter(request, response, this);  
                return;  
            }  
           servlet.service((HttpServletRequest) request,(HttpServletResponse) response);              
    }  
```





如果我对Filter非常了解的，根本就不需要花那么多时间去查看tomcat源代码。只要在web.xml增加一点配置就OK了。
```Java

    <filter-mapping>  
            <filter-name>struts2</filter-name>  
            <url-pattern>/*</url-pattern>  
            <dispatcher>REQUEST</dispatcher>  
            <dispatcher>FORWARD</dispatcher>  
    </filter-mapping>  
```



