# Servlet中的过滤器Filter详解 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年11月04日 16:31:15[boonya](https://me.csdn.net/boonya)阅读数：566标签：[servlet																[filter																[web.xml](https://so.csdn.net/so/search/s.do?q=web.xml&t=blog)
个人分类：[Servlet/JSP](https://blog.csdn.net/boonya/article/category/2864195)






web.xml中元素执行的顺序listener->filter->struts拦截器->servlet。


1.过滤器的概念

Java中的Filter 并不是一个标准的Servlet ，它不能处理用户请求，也不能对客户端生成响应。 主要用于对HttpServletRequest 进行预处理，也可以对HttpServletResponse 进行后处理，是个典型的处理链。


优点：过滤链的好处是，执行过程中任何时候都可以打断，只要不执行chain.doFilter()就不会再执行后面的过滤器和请求的内容。而在实际使用时，就要特别注意过滤链的执行顺序问题


2.过滤器的作用描述


- 在HttpServletRequest 到达Servlet 之前，拦截客户的HttpServletRequest 。 
- 根据需要检查HttpServletRequest ，也可以修改HttpServletRequest 头和数据。 
- 在HttpServletResponse 到达客户端之前，拦截HttpServletResponse 。 
- 根据需要检查HttpServletResponse ，可以修改HttpServletResponse 头和数据。




3.过滤器的执行流程

![](https://img-my.csdn.net/uploads/201211/30/1354261672_8450.jpg)![](https://img-my.csdn.net/uploads/201211/30/1354261343_9300.png)




4.Filter接口

> 

1.如何驱动




> 
> 

在 web 应用程序启动时，web 服务器将根据 web.xml 文件中的配置信息来创建每个注册的 Filter 实例对象，并将其保存在服务器的内存中





> 

2.方法介绍





- init()  Init 方法在 Filter 生命周期中仅执行一次，web 容器在调用 init 方法时
- destory()  在Web容器卸载 Filter 对象之前被调用。该方法在Filter的生命周期中仅执行一次。在这个方法中，可以释放过滤器使用的资源。
- doFilter() Filter 链的执行 




5.FilterChain接口

> 

1.如何实例化




> 
> 
代表当前 Filter 链的对象。由容器实现，容器将其实例作为参数传入过滤器对象的doFilter()方法中



> 

2.作用




> 
> 
调用过滤器链中的下一个过滤器



**filter实例：**


web.xml配置





**[html]**[view
 plain](http://blog.csdn.net/techbirds_bao/article/details/8243800#)[copy](http://blog.csdn.net/techbirds_bao/article/details/8243800#)



- <!-- 编码过滤器 -->
- <filter>
- <filter-name>setCharacterEncoding</filter-name>
- <filter-class>com.company.strutstudy.web.servletstudy.filter.EncodingFilter</filter-class>
- <init-param>
- <param-name>encoding</param-name>
- <param-value>utf-8</param-value>
- </init-param>
- </filter>
- <filter-mapping>
- <filter-name>setCharacterEncoding</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- 
- <!-- 请求url日志记录过滤器 -->
- <filter>
- <filter-name>logfilter</filter-name>
- <filter-class>com.company.strutstudy.web.servletstudy.filter.LogFilter</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>logfilter</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>



编码拦截器：







**[java]**[view
 plain](http://blog.csdn.net/techbirds_bao/article/details/8243800#)[copy](http://blog.csdn.net/techbirds_bao/article/details/8243800#)



- publicclass EncodingFilter implements Filter {  
- private String encoding;  
- private Map<String, String> params = new HashMap<String, String>();  
- // 项目结束时就已经进行销毁
- publicvoid destroy() {  
-         System.out.println("end do the encoding filter!");  
-         params=null;  
-         encoding=null;  
-     }  
- publicvoid doFilter(ServletRequest req, ServletResponse resp,  
-             FilterChain chain) throws IOException, ServletException {  
- //UtilTimerStack.push("EncodingFilter_doFilter:");
-         System.out.println("before encoding " + encoding + " filter！");  
-         req.setCharacterEncoding(encoding);  
- // resp.setCharacterEncoding(encoding);
- // resp.setContentType("text/html;charset="+encoding);
-         chain.doFilter(req, resp);        
-         System.out.println("after encoding " + encoding + " filter！");  
-         System.err.println("----------------------------------------");  
- //UtilTimerStack.pop("EncodingFilter_doFilter:");
-     }  
- 
- // 项目启动时就已经进行读取
- publicvoid init(FilterConfig config) throws ServletException {  
-         System.out.println("begin do the encoding filter!");  
-         encoding = config.getInitParameter("encoding");  
- for (Enumeration e = config.getInitParameterNames(); e  
-                 .hasMoreElements();) {  
-             String name = (String) e.nextElement();  
-             String value = config.getInitParameter(name);  
-             params.put(name, value);  
-         }  
-     }  
-  }  


日志拦截器：



**[java]**[view
 plain](http://blog.csdn.net/techbirds_bao/article/details/8243800#)[copy](http://blog.csdn.net/techbirds_bao/article/details/8243800#)



- publicclass LogFilter implements Filter {  
-     FilterConfig config;  
- 
- publicvoid destroy() {  
- this.config = null;  
-     }  
- 
- publicvoid doFilter(ServletRequest req, ServletResponse res,  
-             FilterChain chain) throws IOException, ServletException {  
- // 获取ServletContext 对象，用于记录日志
-         ServletContext context = this.config.getServletContext();  
- //long before = System.currentTimeMillis();
-         System.out.println("before the log filter!");  
- //context.log("开始过滤");
- // 将请求转换成HttpServletRequest 请求
-         HttpServletRequest hreq = (HttpServletRequest) req;  
- // 记录日志
-         System.out.println("Log Filter已经截获到用户的请求的地址:"+hreq.getServletPath() );  
- //context.log("Filter已经截获到用户的请求的地址: " + hreq.getServletPath());
- try {  
- // Filter 只是链式处理，请求依然转发到目的地址。
-             chain.doFilter(req, res);  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-         System.out.println("after the log filter!");  
- //long after = System.currentTimeMillis();
- // 记录日志
- //context.log("过滤结束");
- // 再次记录日志
- //context.log(" 请求被定位到" + ((HttpServletRequest) req).getRequestURI()
- //      + "所花的时间为: " + (after - before));
-     }  
- 
- publicvoid init(FilterConfig config) throws ServletException {  
-         System.out.println("begin do the log filter!");  
- this.config = config;  
-     }  
- 
-  }  


HelloServlet类：



**[java]**[view
 plain](http://blog.csdn.net/techbirds_bao/article/details/8243800#)[copy](http://blog.csdn.net/techbirds_bao/article/details/8243800#)



- publicclass HelloWorldServlet extends HttpServlet{  
- 
- /**
-      * 查看httpservlet实现的service一看便知，起到了一个controll控制器的作用(转向的)
-      * 之后便是跳转至我们熟悉的doget,dopost等方法中 
-      */
- @Override
- protectedvoid service(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
-         System.out.println("doservice..."+this.getInitParameter("encoding"));  
- 
- super.service(req, resp);  
-     }  
- 
- @Override
- protectedvoid doGet(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
-         System.out.println("doget...");  
-         doPost(req, resp);  
-     }  
- 
- @Override
- protectedvoid doPost(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
-         System.out.println("dopost...");  
-     }  
- 
- 
- 
-  }  


结果：





**[plain]**[view
 plain](http://blog.csdn.net/techbirds_bao/article/details/8243800#)[copy](http://blog.csdn.net/techbirds_bao/article/details/8243800#)



- before encoding utf-8 filter！  
-   before the log filter!  
-   Log Filter已经截获到用户的请求的地址:/hello  
-   doservice...UTF-8  
-   doget...  
-   dopost...  
-   after the log filter!  
-   after encoding utf-8 filter！  
-   ----------------------------------------  




总结：

> 
1.过滤器执行流程

2.常用过滤器




**[html]**[view
 plain](http://blog.csdn.net/techbirds_bao/article/details/8243800#)[copy](http://blog.csdn.net/techbirds_bao/article/details/8243800#)



- <prename="code"class="plain"><pre></pre><prename="code"class="plain"></pre><pre></pre>
- <pre></pre>
- <pre></pre>
- <pre></pre>
- <pre></pre>
- <pre></pre>
- <pre></pre>
- 
- </pre>](https://so.csdn.net/so/search/s.do?q=filter&t=blog)](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)




