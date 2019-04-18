# Weblogic中 web.xml 使用servlet 2.4声明时Filter报错 - z69183787的专栏 - CSDN博客
2014年08月14日 20:09:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6848
报错如下：
problem: cvc-complex-type.2.4a: Expected elements 'description@http://java.sun.com/xml/ns/javaee display-name@http://java.sun.com/xml/ns/javaee icon@http://java.sun.com/xml/ns/javaee distributable@http://java.sun.com/xml/ns/javaee context-param@http://java.sun.com/xml/ns/javaee
 filter@http://java.sun.com/xml/ns/javaee filter-mapping@http://java.sun.com/xml/ns/javaee listener@http://java.sun.com/xml/ns/javaee servlet@http://java.sun.com/xml/ns/javaee servlet-mapping@http://java.sun.com/xml/ns/javaee session-config@http://java.sun.com/xml/ns/javaee
 mime-mapping@http://java.sun.com/xml/ns/javaee welcome-file-list@http://java.sun.com/xml/ns/javaee problem-page@http://java.sun.com/xml/ns/javaee jsp-config@http://java.sun.com/xml/ns/javaee security-constraint@http://java.sun.com/xml/ns/javaee login-config@http://java.sun.com/xml/ns/javaee
 security-role@http://java.sun.com/xml/ns/javaee env-entry@http://java.sun.com/xml/ns/javaee ejb-ref@http://java.sun.com/xml/ns/javaee ejb-local-ref@http://java.sun.com/xml/ns/javaee service-ref@http://java.sun.com/xml/ns/javaee resource-ref@http://java.sun.com/xml/ns/javaee
 resource-env-ref@http://java.sun.com/xml/ns/javaee message-destination-ref@http://java.sun.com/xml/ns/javaee persistence-context-ref@http://java.sun.com/xml/ns/javaee persistence-unit-ref@http://java.sun.com/xml/ns/javaee post-construct@http://java.sun.com/xml/ns/javaee
 pre-destroy@http://java.sun.com/xml/ns/javaee message-destination@http://java.sun.com/xml/ns/javaee locale-encoding-mapping-list@http://java.sun.com/xml/ns/javaee' instead of 'web-app@http://java.sun.com/xml/ns/javaee' here in element web-app@http://java.sun.com/xml/ns/javaee:<null>
web.xml 中 servlet 2.4 原本过滤器中 filter有节点<description>Struts2 Filter</description>，启动报错，去除后即可。
**应该是由DTD 声明 改为 XSD 规范后 禁止了 description 节点。**
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://java.sun.com/xml/ns/j2ee" 
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
version="2.4" 
xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee   http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd">  
	<display-name>CuteFramework Based Application</display-name>
	<context-param>
		<param-name>webAppRootKey</param-name>
		<param-value>portal.root</param-value>
	</context-param>
	<!-- 生产环境设为0 -->
	<context-param>
		<param-name>log4jRefreshInterval</param-name>
		<param-value>60000</param-value>
	</context-param>
	<context-param>
		<param-name>log4jConfigLocation</param-name>
		<param-value>/WEB-INF/classes/log4j.properties</param-value>
	</context-param>
	<context-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>/WEB-INF/classes/*Context.xml</param-value>
	</context-param>
	<filter>
		<filter-name>AutoLogonFilter</filter-name>
		<filter-class>com.wonders.stpt.core.filter.AutoLogonFilter</filter-class>
		<init-param>
			<param-name>ignoreUrls</param-name>
			<param-value>/,/404.jsp,/shibada/zhonggongshibada.jsp,/caClient.jsp,/xlogin.action,/service/stptSynchronize.jsp,/service/domainCrossParam.jsp,/service/domainCrossAuth.jsp,/login.jsp,/logout.jsp,/publicConn.jsp,/login.action,/validateNumber.action,/gotoValidate.action,/showTuserDepts.action,/processDone/updateProcessDone.action,/processDone/setContractReviewResultDataByWebService.action,/processDone/setDocSendDataByWebService.action,/processDone/setHtxxDataByWebService.action,/processDone/setHtxxBySelfNum.action,/report/changeStatusAuto.action,/report/sendMessage.action,/report/sendMessage2.action,/center/xwzx/xw_index.jsp,/center/xwzx/xw_index_reform.jsp,/center/xwzx/xw_company_reform.jsp,/infoSearch/findStfbNewsByPage.action</param-value>
		</init-param>
	</filter>
	<filter-mapping>
		<filter-name>AutoLogonFilter</filter-name>
		<url-pattern>*.action</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>AutoLogonFilter</filter-name>
		<url-pattern>*.html</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>AutoLogonFilter</filter-name>
		<url-pattern>*.jsp</url-pattern>
	</filter-mapping>
	<filter>
		<filter-name>CommonWebFilter</filter-name>
		<filter-class>com.wonders.stpt.core.filter.CommonWebFilter</filter-class>
		<init-param>
			<param-name>notLoginPage</param-name>
			<param-value>/notlogin.jsp</param-value>
		</init-param>
		<init-param>
			<param-name>ignoreUrls</param-name>
			<param-value>/,/404.jsp,/shibada/zhonggongshibada.jsp,/caClient.jsp,/xlogin.action,/service/stptSynchronize.jsp,/service/domainCrossParam.jsp,/service/domainCrossAuth.jsp,/login.jsp,/logout.jsp,/publicConn.jsp,/login.action,/validateNumber.action,/gotoValidate.action,/showTuserDepts.action,/processDone/updateProcessDone.action,/processDone/setContractReviewResultDataByWebService.action,/processDone/setDocSendDataByWebService.action,/processDone/setHtxxDataByWebService.action,/processDone/setHtxxBySelfNum.action,/report/changeStatusAuto.action,/report/sendMessage.action,/report/sendMessage2.action,/center/xwzx/xw_index.jsp,/center/xwzx/xw_index_reform.jsp,/center/xwzx/xw_company_reform.jsp,/infoSearch/findStfbNewsByPage.action</param-value>
		</init-param>
	</filter>
	<filter-mapping>
		<filter-name>CommonWebFilter</filter-name>
		<url-pattern>*.action</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>CommonWebFilter</filter-name>
		<url-pattern>*.html</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>CommonWebFilter</filter-name>
		<url-pattern>*.jsp</url-pattern>
	</filter-mapping>
	
	<filter>  
        <filter-name>XSS Filter</filter-name>  
        <filter-class>com.wonders.xss.XSSFilter</filter-class>  
    </filter>  
    <filter-mapping>  
        <filter-name>XSS Filter</filter-name>  
        <url-pattern>/*</url-pattern>  
    </filter-mapping>
	<filter>
		<filter-name>struts2</filter-name>
<!--<description>Struts2 Filter</description>--><span style="white-space:pre">		</span>
		<filter-class>
			org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter
		</filter-class>
		<init-param>
			<param-name>packages</param-name>
			<param-value>com.wondersgroup.framework.widget</param-value>
		</init-param>
	</filter>
	<filter>
		<filter-name>struts2-cleanup</filter-name>
		<filter-class>
			org.apache.struts2.dispatcher.ActionContextCleanUp
		</filter-class>
	</filter>
	<filter>
		<filter-name>EncodingFilter</filter-name>
		<filter-class>
			org.springframework.web.filter.CharacterEncodingFilter
		</filter-class>
		<init-param>
			<param-name>encoding</param-name>
			<param-value>utf-8</param-value>
		</init-param>
	</filter>
	<filter>
		<filter-name>widget</filter-name>
		<filter-class>com.wondersgroup.framework.widget.context.ResourceFilter</filter-class>
	</filter>
	<filter-mapping>
		<filter-name>struts2-cleanup</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>struts2</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
<!--
	<filter-mapping>
	  <filter-name>struts2</filter-name>
	  <url-pattern>*.action</url-pattern>
	 </filter-mapping>
	 <filter-mapping>
	  <filter-name>struts2</filter-name>
	  <url-pattern>*.jsp</url-pattern>
	 </filter-mapping>
  -->
	<filter-mapping>
		<filter-name>EncodingFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>widget</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	<listener>
		<listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
	</listener>
	<listener>
		<listener-class>
			org.springframework.web.context.ContextLoaderListener
		</listener-class>
	</listener>
	<welcome-file-list>
		<welcome-file>login.jsp</welcome-file>
	</welcome-file-list>
<!-- 
	<servlet>
		<description>JAX-WS endpoint - HelloWorldService</description>
		<display-name>HelloWorldService</display-name>
		<servlet-name>HelloWorldService</servlet-name>
		<servlet-class>
			com.sun.xml.ws.transport.http.servlet.WSServlet
  		</servlet-class>
		<load-on-startup>1</load-on-startup>
	</servlet>
	<servlet-mapping>
		<servlet-name>HelloWorldService</servlet-name>
		<url-pattern>/zhoushun_cocc</url-pattern>
	</servlet-mapping>
	<servlet-mapping>
		<servlet-name>HelloWorldService</servlet-name>
		<url-pattern>/HelloWorldPort</url-pattern>
	</servlet-mapping>
	<listener>
		<listener-class>
			com.sun.xml.ws.transport.http.servlet.WSServletContextListener
  	</listener-class>
	</listener>
 	<filter>
        <filter-name>openSessionInView</filter-name>
        <filter-class>org.springframework.orm.hibernate3.support.OpenSessionInViewFilter</filter-class>
        <init-param>
            <param-name>singleSession</param-name>
            <param-value>true</param-value>
        </init-param>
    </filter>
    
    <filter-mapping>
        <filter-name>openSessionInView</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
     -->
	<!--
		session失效时间 分钟 <session-config> <session-timeout>-1</session-timeout>
		</session-config>
	-->
	<!--
		同时更改 weblogic.xml
		那么你就能在你的web程序里通过context.lookup("java:comp/env/jdbc/adsl")访问到这个数据源了。
		<resource-ref> <res-ref-name>jdbc/adsl</res-ref-name>
		<res-type>java.sql.DataSource</res-type>
		<res-auth>Container</res-auth>
		<res-sharing-scope>Shareable</res-sharing-scope> </resource-ref>
	-->
	
	<error-page>
		<error-code>404</error-code>
		<location>/404.jsp</location>
	</error-page>
	<error-page>
		<error-code>500</error-code>
		<location>/500.jsp</location>
	</error-page>
	<!--
	<jsp-config> 
    	<el-ignored>false </el-ignored> 
	</jsp-config> 
	-->
</web-app>
```
