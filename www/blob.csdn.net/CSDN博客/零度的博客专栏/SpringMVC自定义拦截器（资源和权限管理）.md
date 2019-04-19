# SpringMVC自定义拦截器（资源和权限管理） - 零度的博客专栏 - CSDN博客
2017年11月08日 14:28:31[零度anngle](https://me.csdn.net/zmx729618)阅读数：3655
**1.DispatcherServlet**
    SpringMVC具有统一的入口DispatcherServlet，所有的请求都通过DispatcherServlet。
    DispatcherServlet是前置控制器，配置在web.xml文件中的。拦截匹配的请求，Servlet拦截匹配规则要自已定义，把拦截下来的请求，依据某某规则分发到目标Controller来处理。  所以我们现在web.xml中加入以下配置：
**[html]**[view
 plain](http://blog.csdn.net/tonytfjing/article/details/39207551#)[copy](http://blog.csdn.net/tonytfjing/article/details/39207551#)
[print](http://blog.csdn.net/tonytfjing/article/details/39207551#)[?](http://blog.csdn.net/tonytfjing/article/details/39207551#)
- <!-- 初始化 DispatcherServlet时，该框架在 web应用程序WEB-INF目录中寻找一个名为[servlet-名称]-servlet.xml的文件，  
-          并在那里定义相关的Beans，重写在全局中定义的任何Beans -->
- <servlet>
- <servlet-name>springMybatis</servlet-name>
- <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
- <load-on-startup>1</load-on-startup>
- </servlet>
- <servlet-mapping>
- <servlet-name>springMybatis</servlet-name>
- <!-- 所有的的请求，都会被DispatcherServlet处理 -->
- <url-pattern>/</url-pattern>
- </servlet-mapping>
**2.静态资源不拦截**
       如果只配置拦截类似于*.do格式的url，则对静态资源的访问是没有问题的，但是如果配置拦截了所有的请求（如我们上面配置的“/”），就会造成js文件、css文件、图片文件等静态资源无法访问。
      一般实现拦截器主要是为了权限管理，主要是拦截一些url请求，所以不对静态资源进行拦截。要过滤掉静态资源一般有两种方式，
第一种是采用<mvc:default-servlet-handler />，（一般Web应用服务器默认的Servlet名称是"default"，所以这里我们激活Tomcat的defaultServlet来处理静态文件，在web.xml里配置如下代码即可：）
**[html]**[view
 plain](http://blog.csdn.net/tonytfjing/article/details/39207551#)[copy](http://blog.csdn.net/tonytfjing/article/details/39207551#)
[print](http://blog.csdn.net/tonytfjing/article/details/39207551#)[?](http://blog.csdn.net/tonytfjing/article/details/39207551#)
- <!--　该servlet为tomcat,jetty等容器提供,将静态资源映射从/改为/static/目录，如原来访问　http://localhost/foo.css　,现在http://localhost/static/foo.css　-->
- <!-- 不拦截静态文件 -->
- <servlet-mapping>
- <servlet-name>default</servlet-name>
- <url-pattern>/js/*</url-pattern>
- <url-pattern>/css/*</url-pattern>
- <url-pattern>/images/*</url-pattern>
- <url-pattern>/fonts/*</url-pattern>
- </servlet-mapping>
        Tomcat, Jetty, JBoss, and GlassFish  默认 Servlet的名字 -- "default"
        Resin 默认 Servlet的名字 -- "resin-file"
        WebLogic 默认 Servlet的名字  -- "FileServlet"
        WebSphere  默认 Servlet的名字 -- "SimpleFileServlet"
       如果你所有的Web应用服务器的默认Servlet名称不是"default"，则需要通过default-servlet-name属性显示指定：
**[html]**[view
 plain](http://blog.csdn.net/tonytfjing/article/details/39207551#)[copy](http://blog.csdn.net/tonytfjing/article/details/39207551#)
[print](http://blog.csdn.net/tonytfjing/article/details/39207551#)[?](http://blog.csdn.net/tonytfjing/article/details/39207551#)
- <mvc:default-servlet-handlerdefault-servlet-name="所使用的Web服务器默认使用的Servlet名称"/>
      第二种是采用<mvc:resources />，在springmvc的配置文件中加入以下代码：
**[html]**[view
 plain](http://blog.csdn.net/tonytfjing/article/details/39207551#)[copy](http://blog.csdn.net/tonytfjing/article/details/39207551#)
[print](http://blog.csdn.net/tonytfjing/article/details/39207551#)[?](http://blog.csdn.net/tonytfjing/article/details/39207551#)
- <mvc:resourcesmapping="/js/**"location="/static_resources/javascript/"/>
- <mvc:resourcesmapping="/styles/**"location="/static_resources/css/"/>
- <mvc:resourcesmapping="/images/**"location="/static_resources/images/"/>
**3.自定义拦截器**
    SpringMVC的拦截器HandlerInterceptorAdapter对应提供了三个preHandle，postHandle，afterCompletion方法。preHandle在业务处理器处理请求之前被调用，
    postHandle在业务处理器处理请求执行完成后,生成视图之前执行，afterCompletion在DispatcherServlet完全处理完请求后被调用,可用于清理资源等 。所以要想实现自己的权限管理逻辑，需要继承HandlerInterceptorAdapter并重写其三个方法。
    首先在springmvc.xml中加入自己定义的拦截器我的实现逻辑CommonInterceptor，
**[html]**[view
 plain](http://blog.csdn.net/tonytfjing/article/details/39207551#)[copy](http://blog.csdn.net/tonytfjing/article/details/39207551#)
[print](http://blog.csdn.net/tonytfjing/article/details/39207551#)[?](http://blog.csdn.net/tonytfjing/article/details/39207551#)
- <!--配置拦截器, 多个拦截器,顺序执行 -->
- <mvc:interceptors>
- <mvc:interceptor>
- <!-- 匹配的是url路径， 如果不配置或/**,将拦截所有的Controller -->
- <mvc:mappingpath="/"/>
- <mvc:mappingpath="/user/**"/>
- <mvc:mappingpath="/test/**"/>
- <beanclass="com.alibaba.interceptor.CommonInterceptor"></bean>
- </mvc:interceptor>
- <!-- 当设置多个拦截器时，先按顺序调用preHandle方法，然后逆序调用每个拦截器的postHandle和afterCompletion方法 -->
- </mvc:interceptors>
    我的拦截逻辑是“在未登录前，任何访问url都跳转到login页面；登录成功后跳转至先前的url”，具体代码如下：
**[java]**[view
 plain](http://blog.csdn.net/tonytfjing/article/details/39207551#)[copy](http://blog.csdn.net/tonytfjing/article/details/39207551#)
[print](http://blog.csdn.net/tonytfjing/article/details/39207551#)[?](http://blog.csdn.net/tonytfjing/article/details/39207551#)
- /**
-  * 
-  */
- package com.alibaba.interceptor;  
- 
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- import org.slf4j.Logger;  
- import org.slf4j.LoggerFactory;  
- import org.springframework.web.servlet.ModelAndView;  
- import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;  
- 
- import com.alibaba.util.RequestUtil;  
- 
- 
- /**
-  * @author tfj
-  * 2014-8-1
-  */
- publicclass CommonInterceptor extends HandlerInterceptorAdapter{  
- privatefinal Logger log = LoggerFactory.getLogger(CommonInterceptor.class);  
- publicstaticfinal String LAST_PAGE = "com.alibaba.lastPage";  
- /*
-      * 利用正则映射到需要拦截的路径    
- 
-     private String mappingURL;
- 
-     public void setMappingURL(String mappingURL) {    
-                this.mappingURL = mappingURL;    
-     }   
-   */
- /** 
-      * 在业务处理器处理请求之前被调用 
-      * 如果返回false 
-      *     从当前的拦截器往回执行所有拦截器的afterCompletion(),再退出拦截器链
-      * 如果返回true 
-      *    执行下一个拦截器,直到所有的拦截器都执行完毕 
-      *    再执行被拦截的Controller 
-      *    然后进入拦截器链, 
-      *    从最后一个拦截器往回执行所有的postHandle() 
-      *    接着再从最后一个拦截器往回执行所有的afterCompletion() 
-      */
- @Override
- publicboolean preHandle(HttpServletRequest request,    
-             HttpServletResponse response, Object handler) throws Exception {    
- if ("GET".equalsIgnoreCase(request.getMethod())) {  
-             RequestUtil.saveRequest();  
-         }  
-         log.info("==============执行顺序: 1、preHandle================");    
-         String requestUri = request.getRequestURI();  
-         String contextPath = request.getContextPath();  
-         String url = requestUri.substring(contextPath.length());  
- 
-         log.info("requestUri:"+requestUri);    
-         log.info("contextPath:"+contextPath);    
-         log.info("url:"+url);    
- 
-         String username =  (String)request.getSession().getAttribute("user");   
- if(username == null){  
-             log.info("Interceptor：跳转到login页面！");  
-             request.getRequestDispatcher("/WEB-INF/jsp/login.jsp").forward(request, response);  
- returnfalse;  
-         }else
- returntrue;     
-     }    
- 
- /**
-      * 在业务处理器处理请求执行完成后,生成视图之前执行的动作   
-      * 可在modelAndView中加入数据，比如当前时间
-      */
- @Override
- publicvoid postHandle(HttpServletRequest request,    
-             HttpServletResponse response, Object handler,    
-             ModelAndView modelAndView) throws Exception {     
-         log.info("==============执行顺序: 2、postHandle================");    
- if(modelAndView != null){  //加入当前时间  
-             modelAndView.addObject("var", "测试postHandle");    
-         }    
-     }    
- 
- /** 
-      * 在DispatcherServlet完全处理完请求后被调用,可用于清理资源等  
-      *  
-      * 当有拦截器抛出异常时,会从当前拦截器往回执行所有的拦截器的afterCompletion() 
-      */
- @Override
- publicvoid afterCompletion(HttpServletRequest request,    
-             HttpServletResponse response, Object handler, Exception ex)    
- throws Exception {    
-         log.info("==============执行顺序: 3、afterCompletion================");    
-     }    
- 
- }    
    注：上述代码里我写了一个RequestUtil,主要实现获取当前Request、Session对象，保存和加密页面，取出等功能。
至此，拦截器已经实现了，效果如图：
我直接访问/test/hello，会被拦截
![](https://img-blog.csdn.net/20140911161213765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9ueXRmamluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
登录成功后会跳转至/test/hello对应的页面
![](https://img-blog.csdn.net/20140911161416531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9ueXRmamluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
