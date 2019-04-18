# 使用 Spring 容器管理 Servlet - z69183787的专栏 - CSDN博客
2017年12月19日 18:00:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：397
 自定义（继承自 javax.servlet.http.HttpServlet）的 Servlet 如何像 Struts1/2 中那样调用 Spring 容器的 service 呢？
        如同 Struts1/2 的配置一样，Spring 在 web.xml 中的配置及其 application*.xml 配置不变：
        web.xml 中：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/6928732#)[copy](http://blog.csdn.net/defonds/article/details/6928732#)
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
[html][view
 plain](http://blog.csdn.net/defonds/article/details/6928732#)[copy](http://blog.csdn.net/defonds/article/details/6928732#)
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>/WEB-INF/applicationContext*.xml</param-value>
- </context-param>
        applicationContext-service.xml 中：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/6928732#)[copy](http://blog.csdn.net/defonds/article/details/6928732#)
- <beanid="operationService"
- class="com.defonds.cds.service.operation.impl.OperationServiceImpl"scope="singleton">
- </bean>
        如同一般的 Servlet 的配置一样，Servlet 在 web.xml 中的配置不变：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/6928732#)[copy](http://blog.csdn.net/defonds/article/details/6928732#)
- <servlet>
- <servlet-name>downloadServlet</servlet-name>
- <servlet-class>com.defonds.cds.common.ArcSyncDownloadServlet</servlet-class>
- </servlet>
- <servlet-mapping>
- <servlet-name>downloadServlet</servlet-name>
- <url-pattern>/file</url-pattern>
- </servlet-mapping>
        如同一般的 Struts1/2 的 action 一样注入 service：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/6928732#)[copy](http://blog.csdn.net/defonds/article/details/6928732#)
- private OperationService operationService = null;  
- public OperationService getOperationService() {  
- return operationService;  
- }  
- 
- publicvoid setOperationService(OperationService operationService) {  
- this.operationService = operationService;  
- }  
        在 Servlet 中如同一般的 Struts1/2 的 action 一样调用 service：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/6928732#)[copy](http://blog.csdn.net/defonds/article/details/6928732#)
- FileInfo fileInfo = this.getOperationService().getFileByFidAndSecret(Long.parseLong(fileId), secret);  
        唯一需要修改的是 Servlet 的 init 方法：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/6928732#)[copy](http://blog.csdn.net/defonds/article/details/6928732#)
- publicvoid init(ServletConfig config) throws ServletException {  
- super.init(config);  
-     ServletContext servletContext = this.getServletContext();  
-     WebApplicationContext wac = null;   
-     wac = WebApplicationContextUtils.getRequiredWebApplicationContext(servletContext);  
- this.setOperationService((OperationService) wac.getBean("operationService"));//Spring 配置 中的 bean id
- }     
- 

 Servlet 可否也能像 Struts1/2 的 action 那样作为一个 javaBean 在 Spring 容器里进行管理呢？答案是肯定的。
        自定义（继承自 javax.servlet.http.HttpServlet）的 Servlet 如何像 Struts1/2 中那样调用 Spring 容器的 service 呢？《[Servlet
 调用 Spring 容器的 service](http://blog.csdn.net/defonds/article/details/6928732)》一文很好地解决了这个问题。美中不足的是，ArcSyncDownloadServlet 在得到其注入的 bean 时，需要显式地写出 bean 在 Spring 配置中的 id 才可以：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/6952963#)[copy](http://blog.csdn.net/defonds/article/details/6952963#)
- this.setOperationService((OperationService) wac.getBean("operationService"));//Spring 配置 中的 bean id   
        这样子违背了 Spring 依赖注入的思想。那么如何才可以不在代码中显式调用这个 bean id，而把 bean id 直接写在配置文件中呢？
        本文用一个项目中使用的例子介绍了将 Servlet 和业务对象的依赖关系使用 Spring 来管理，而不用再在 Servlet 中硬编码要引用对象的名字。
        仍然使用《[Servlet 调用 Spring 容器的 service](http://blog.csdn.net/defonds/article/details/6928732)》一文中的例子。
        与《[Servlet 调用 Spring 容器的 service](http://blog.csdn.net/defonds/article/details/6928732)》的做法相反，web.xml
 和 Spring 的 application*.xml 配置需要改变，而 Servlet 不需要做改变。
        如同 Struts1/2 的配置一样，Spring 在 web.xml 中的配置：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/6952963#)[copy](http://blog.csdn.net/defonds/article/details/6952963#)
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>/WEB-INF/applicationContext*.xml</param-value>
- </context-param>
        如同 Struts1/2 的配置一样，Spring 在 applicationContext-service.xml 中定义我们的业务逻辑处理类：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/6952963#)[copy](http://blog.csdn.net/defonds/article/details/6952963#)
- <beanid="operationService"
- class="com.defonds.cds.service.operation.impl.OperationServiceImpl"scope="singleton">
- </bean>
        如同一般的 Struts1/2 的 action 一样在我们的 Servlet 中注入 service：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/6952963#)[copy](http://blog.csdn.net/defonds/article/details/6952963#)
- private OperationService operationService = null;  
- public OperationService getOperationService() {  
- return operationService;  
- }  
- 
- publicvoid setOperationService(OperationService operationService) {  
- this.operationService = operationService;  
- }  
        在 Servlet 中如同一般的 Struts1/2 的 action 一样调用 service：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/6952963#)[copy](http://blog.csdn.net/defonds/article/details/6952963#)
- FileInfo fileInfo = this.getOperationService().getFileByFidAndSecret(Long.parseLong(fileId), secret);  
        如同一般的 Servlet 我们的这个 Servlet 需要继承 GenericServlet 或者 HttpServlet：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/6952963#)[copy](http://blog.csdn.net/defonds/article/details/6952963#)
- publicclass ArcSyncDownloadServlet extends HttpServlet  
        无须重写Servlet 的 init 方法，当然如果读者的业务需求需要的话除外。
        新建一个 Servlet 代理类，这个类类似于 DelegatingFilterProxy 那样的代理，通过代理根据配置来找到实际的 Servlet，完成业务逻辑功能。
[java][view
 plain](http://blog.csdn.net/defonds/article/details/6952963#)[copy](http://blog.csdn.net/defonds/article/details/6952963#)
- package com.defonds.cds.common;  
- 
- import java.io.IOException;  
- 
- import javax.servlet.Servlet;  
- import javax.servlet.ServletContext;  
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- import org.springframework.web.context.WebApplicationContext;  
- import org.springframework.web.context.support.WebApplicationContextUtils;  
- 
- publicclass DelegatingServletProxy extends HttpServlet {  
- privatestaticfinallong serialVersionUID = 1L;  
- private String targetServletBean;  
- private Servlet proxy;  
- 
- @Override
- publicvoid init() throws ServletException {  
- this.targetServletBean = this.getInitParameter("targetServletBean");  
- this.getServletBean();  
- this.proxy.init(this.getServletConfig());  
-     }  
- 
- @Override
- protectedvoid service(HttpServletRequest request, HttpServletResponse response)  
- throws ServletException, IOException {  
-         proxy.service(request,response);   
-     }  
- 
- privatevoid getServletBean(){  
-         ServletContext servletContext = this.getServletContext();  
-         WebApplicationContext wac = null;   
-         wac = WebApplicationContextUtils.getRequiredWebApplicationContext(servletContext);  
- this.proxy = (Servlet) wac.getBean(targetServletBean);  
-     }  
- }  
        不同于一般的 Servlet 在 web.xml 中的配置，需要配置的是 Servlet 代理类，而非 Servlet：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/6952963#)[copy](http://blog.csdn.net/defonds/article/details/6952963#)
- <servlet>
- <servlet-name>proxyDownloadServletBean</servlet-name>
- <servlet-class>com.defonds.cds.common.DelegatingServletProxy</servlet-class>
- <init-param>
- <param-name>targetServletBean</param-name>
- <param-value>downLoadServletBean</param-value>
- </init-param>
- </servlet>
- <servlet-mapping>
- <servlet-name>proxyDownloadServletBean</servlet-name>
- <url-pattern>/file</url-pattern>
- </servlet-mapping>
        最后在 applicationContext-service.xml 中将 Servlet 及其业务对象的依赖关系配置到 Spring 容器管理：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/6952963#)[copy](http://blog.csdn.net/defonds/article/details/6952963#)
- <beanid="downLoadServletBean"class="com.defonds.cds.common.ArcSyncDownloadServlet">
- <propertyname="operationService">
- <refbean="operationService"/>
- </property>
- </bean>
        注意这里的 downLoadServletBean 要和 web.xml 配置的 downLoadServletBean 对应，operationService 要和 Servlet 中 get/set 得到的 operationService bean 对应。
        如果是多个 Servlet 的话，可以共用同一个代理 Servlet。Servlet 代码和代理 Servlet 代码无须改变，只需要注意一下在 web.xml 里的配置即可：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/6952963#)[copy](http://blog.csdn.net/defonds/article/details/6952963#)
- <servlet>
- <servlet-name>proxyDownloadServletBean</servlet-name>
- <servlet-class>com.defonds.cds.service.operation.servlet.DelegatingServletProxy</servlet-class>
- <init-param>
- <param-name>targetServletBean</param-name>
- <param-value>downLoadServletBean</param-value>
- </init-param>
- </servlet>
- <servlet-mapping>
- <servlet-name>proxyDownloadServletBean</servlet-name>
- <url-pattern>/file</url-pattern>
- </servlet-mapping>
- 
- <servlet>
- <servlet-name>proxyWebServiceServletBean</servlet-name>
- <servlet-class>com.defonds.cds.service.operation.servlet.DelegatingServletProxy</servlet-class>
- <init-param>
- <param-name>targetServletBean</param-name>
- <param-value>webServiceServletBean</param-value>
- </init-param>
- </servlet>
- <servlet-mapping>
- <servlet-name>proxyWebServiceServletBean</servlet-name>
- <url-pattern>/WebService</url-pattern>
- </servlet-mapping>
- 
