# 使用 Spring 容器管理 Filter - z69183787的专栏 - CSDN博客
2017年12月19日 17:51:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：623
    如一般的 J2EE 配置一样，Spring 在 web.xml 中的配置：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/7191546#)[copy](http://blog.csdn.net/defonds/article/details/7191546#)
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>/WEB-INF/applicationContext*.xml</param-value>
- </context-param>
        如一般的 J2EE 配置一样，Spring 在 applicationContext-service.xml 中定义我们的业务逻辑处理类：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/7191546#)[copy](http://blog.csdn.net/defonds/article/details/7191546#)
- <beanid="logService"
- class="com.defonds.cds.system.log.impl.LogServiceImpl"
- parent="baseService"scope="singleton"init-method="init"destroy-method="destroy">
- </bean>
        如同一般的 Struts1/2 的 action 一样在我们的 Filter 中注入 service：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/7191546#)[copy](http://blog.csdn.net/defonds/article/details/7191546#)
- private LogService logService;  
- public LogService getLogService() {  
- return logService;  
- }  
- 
- publicvoid setLogService(LogService logService) {  
- this.logService = logService;  
- }     
        在 Filter 中如同一般的 Struts1/2 的 action 一样调用 service：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/7191546#)[copy](http://blog.csdn.net/defonds/article/details/7191546#)
- String ip = logService.getIpAddr(request);  
        如同一般的 Filter 我们的这个 Filter 需要实现 javax.servlet.Filter 接口：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/7191546#)[copy](http://blog.csdn.net/defonds/article/details/7191546#)
- publicclass BlockedIpFilter implements Filter {  
        根据自己的业务需要去实现 init、doFilter 和 destroy 方法：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/7191546#)[copy](http://blog.csdn.net/defonds/article/details/7191546#)
- @Override
- publicvoid doFilter(ServletRequest servletRequest, ServletResponse servletResponse,  
-         FilterChain filterChain) throws IOException, ServletException {  
- if (ifIpBlocked) {  
-         HttpServletRequest request = (HttpServletRequest)servletRequest;  
-         HttpServletResponse response = (HttpServletResponse) servletResponse;    
-         String ip = logService.getIpAddr(request);  
- 
- if (logService.ifBlocked(ip)) {  
-             response.getWriter().write("error");  
-             response.getWriter().close();  
-         } else {  
-             filterChain.doFilter(request, response);  
-         }  
-     } else {  
-         filterChain.doFilter(servletRequest, servletResponse);  
-     }  
- 
- }  
- 
- @Override
- publicvoid init(FilterConfig arg0) throws ServletException {  
- // TODO Auto-generated method stub
- }  
- 
- @Override
- publicvoid destroy() {  
- // TODO Auto-generated method stub
- }  
        新建一个 Filter 代理类，这个类类似于 DelegatingFilterProxy 那样的代理，通过代理根据配置来找到实际的 Filter，完成业务逻辑功能：
[java][view
 plain](http://blog.csdn.net/defonds/article/details/7191546#)[copy](http://blog.csdn.net/defonds/article/details/7191546#)
- package com.defonds.cds.system.log.filter;  
- 
- import java.io.IOException;  
- 
- 
- import javax.servlet.Filter;  
- import javax.servlet.FilterChain;  
- import javax.servlet.FilterConfig;  
- import javax.servlet.ServletContext;  
- import javax.servlet.ServletException;  
- import javax.servlet.ServletRequest;  
- import javax.servlet.ServletResponse;  
- 
- import org.springframework.web.context.WebApplicationContext;  
- import org.springframework.web.context.support.WebApplicationContextUtils;  
- 
- 
- //Filter 的代理类。系统所有的 Filter 共用此一个
- publicclass DelegatingFilterProxy implements Filter {  
- private String targetFilterBean;  
- private Filter proxy;  
- 
- @Override
- publicvoid doFilter(ServletRequest servletRequest, ServletResponse servletResponse,  
-             FilterChain filterChain) throws IOException, ServletException {  
-         proxy.doFilter(servletRequest, servletResponse, filterChain);  
-     }  
- 
- @Override
- publicvoid init(FilterConfig config) throws ServletException {  
- this.targetFilterBean = config.getInitParameter("targetFilterBean");  
-         ServletContext servletContext = null;  
-         servletContext = config.getServletContext();  
-         WebApplicationContext wac = null;  
-         wac = WebApplicationContextUtils.getRequiredWebApplicationContext(servletContext);  
- this.proxy = (Filter) wac.getBean(this.targetFilterBean);  
- this.proxy.init(config);  
-     }  
- 
- @Override
- publicvoid destroy() {}  
- }  
        不同于一般的 Filter 在 web.xml 中的配置，需要配置的是 Filter 代理类，而非 Filter：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/7191546#)[copy](http://blog.csdn.net/defonds/article/details/7191546#)
- <filter>
- <filter-name>proxyBlockedIpFilterBean</filter-name>
- <filter-class>com.defonds.cds.system.log.filter.DelegatingFilterProxy</filter-class>
- <init-param>
- <param-name>targetFilterBean</param-name>
- <param-value>blockedIpFilterBean</param-value>
- </init-param>
- </filter>
- <filter-mapping>
- <filter-name>proxyBlockedIpFilterBean</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
        最后在 applicationContext-service.xml 中将 Filter 及其业务对象的依赖关系配置到 Spring 容器管理：
[html][view
 plain](http://blog.csdn.net/defonds/article/details/7191546#)[copy](http://blog.csdn.net/defonds/article/details/7191546#)
- <beanid="blockedIpFilterBean"class="com.defonds.cds.system.log.filter.BlockedIpFilter">
- <propertyname="logService">
- <refbean="logService"/>
- </property>
- </bean>
        注意这里的 blockedIpFilterBean 要和 web.xml 配置的 blockedIpFilterBean 对应，logService 要和 Filter 中 get/set 得到的 logService bean 对应。
