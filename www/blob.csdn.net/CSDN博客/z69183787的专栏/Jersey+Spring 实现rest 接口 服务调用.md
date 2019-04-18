# Jersey+Spring 实现rest 接口 服务调用 - z69183787的专栏 - CSDN博客
2014年01月14日 19:30:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：23923
 下载地址：[http://download.java.net/maven/2/com/sun/jersey/contribs/jersey-spring/](http://download.java.net/maven/2/com/sun/jersey/contribs/jersey-spring/)
或官网
[https://maven.java.net/content/repositories/releases/com/sun/jersey/contribs/jersey-spring/1.9/](https://maven.java.net/content/repositories/releases/com/sun/jersey/contribs/jersey-spring/1.9/)
所需包：
jersey-client-1.8.jar
jersey-core-1.8.jar
jersey-json-1.8.jar
jersey-server-1.8.jar
jersey-spring-1.8.jar
所有jar 目录如下：
![](https://img-blog.csdn.net/20140114192845109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
web.xml：
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE web-app PUBLIC "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN" "http://java.sun.com/dtd/web-app_2_3.dtd">
<web-app>
	<display-name>workflowController Application</display-name>
	 
	<context-param>
		<param-name>webAppRootKey</param-name>
		<param-value>workflowController.root</param-value>
	</context-param>
	 
	 <!-- 生产环境设为0 -->
	<context-param>
		<param-name>log4jRefreshInterval</param-name>
		<param-value>0</param-value>
	</context-param>
	<context-param>
		<param-name>log4jConfigLocation</param-name>
		<param-value>/WEB-INF/classes/log4j.properties</param-value>
	</context-param>
	
	<context-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>/WEB-INF/classes/*Context.xml</param-value>
		<!-- classpath:applicationContext.xml,classpath*:*Context.xml -->
	</context-param>
	
	
	
	<servlet>
		<servlet-name>REST Servlet</servlet-name>
		<servlet-class>com.sun.jersey.spi.spring.container.servlet.SpringServlet</servlet-class>
		<load-on-startup>1</load-on-startup>
		<init-param>
		  <param-name>com.sun.jersey.config.property.packages</param-name>
		  <param-value>com.wondersgroup.resource</param-value> 
		</init-param>
	</servlet>
	<servlet-mapping>
		<servlet-name>REST Servlet</servlet-name>
		<url-pattern>/service/*</url-pattern>
	</servlet-mapping>
	
	<servlet>
		<servlet-name>SecheduleServlet</servlet-name>
		<servlet-class>com.wondersgroup.schedule.servlet.ScheduleServlet</servlet-class>
		<load-on-startup>2</load-on-startup>
	</servlet>
	
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
	<filter-mapping>
		<filter-name>EncodingFilter</filter-name>
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
	<!--  
	<filter>
		<filter-name>struts2</filter-name>
		<description>Struts2 Filter</description>
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
		<description>Struts2 CleanUp Filter</description>
		<filter-class>
			org.apache.struts2.dispatcher.ActionContextCleanUp
		</filter-class>
	</filter>
	
	<filter-mapping>
		<filter-name>struts2-cleanup</filter-name>
		<url-pattern>*.action</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>struts2</filter-name>
		<url-pattern>*.action</url-pattern>
	</filter-mapping>
	-->
	<!-- session失效时间 分钟
	<session-config>  
    	<session-timeout>-1</session-timeout>  
	</session-config> 
	 -->
</web-app>
```
sample：
```java
/**
 * 
 */
package com.wondersgroup.workflow.resource;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.ws.rs.Consumes;
import javax.ws.rs.FormParam;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;
import com.google.gson.Gson;
import com.wondersgroup.workflow.controller.WorkflowController;
import com.wondersgroup.workflow.model.bo.WorkflowActivity;
import com.wondersgroup.workflow.model.vo.WorkflowResult;
/** 
 * @ClassName: WorkflowService 
 * @Description: TODO(rest) 
 * @author zhoushun 
 * @date 2013-5-31 09:34:11 
 *  
 */
@Component
@Path("workflow")
@Scope("prototype")
public class WorkflowResource {
	@Context
	public HttpServletRequest request;
    @Context 
    public HttpServletResponse response;
	
	private WorkflowController controller;
	
	public WorkflowController getController() {
		return controller;
	}
	@Autowired(required=false)
	public void setController(@Qualifier("workflowController")WorkflowController controller) {
		this.controller = controller;
	}
	@POST
	@Path("launchProcess")
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	@Produces(MediaType.APPLICATION_JSON)
	public WorkflowResult launchProcess(@FormParam("data") String data){
		System.out.println("data="+ data);
		Gson gson = new Gson();
		WorkflowResult result =  this.controller.saveProcessData(data,2);
		System.out.println(gson.toJson(result));
		if(result.isSuccess()){
			result = this.controller.lauchProcess();
		}
		System.out.println(gson.toJson(result));
		
		return result;
	}
	
	@POST
	@Path("saveProcessStep")
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	@Produces(MediaType.APPLICATION_JSON)
	public WorkflowResult saveProcessStep(@FormParam("data") String data){
		WorkflowResult result = this.controller.saveProcessData(data,0);
		return result;
	}
	
	//
	@POST
	@Path("completeProcessStep")
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	@Produces(MediaType.APPLICATION_JSON)
	public WorkflowResult completeProcessStep(@FormParam("id") String id){
		WorkflowResult result = this.controller.completeProcessStep(id);
		return result;
	}
	
	
	@POST
	@Path("test")
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	@Produces(MediaType.APPLICATION_JSON)
	public WorkflowActivity test(@FormParam("zs") String zs){
		System.out.println("----------------------------");
		System.out.println(zs);
		System.out.println("----------------------------");
		System.out.println(request.getParameter("ss"));
		WorkflowActivity bo = new WorkflowActivity();
		 if (true) {
			  //  throw new CustomNotFoundException(new WorkflowResult());
			  }
		return bo;
		
	}
}
```
