# spring mvc异常统一处理（使用ControllerAdvice注解） - 零度的博客专栏 - CSDN博客
2017年01月04日 11:59:30[零度anngle](https://me.csdn.net/zmx729618)阅读数：3237
## 1、配置
spring 版本：
**[html]**[view plain](http://blog.csdn.net/chenaschen/article/details/51291566#)[copy](http://blog.csdn.net/chenaschen/article/details/51291566#)[print](http://blog.csdn.net/chenaschen/article/details/51291566#)[?](http://blog.csdn.net/chenaschen/article/details/51291566#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1668701/fork)
- <org.springframework-version>4.1.9.RELEASE</org.springframework-version>
![](http://static.blog.csdn.net/images/save_snippets_01.png)
`<org.springframework-version>4.1.9.RELEASE</org.springframework-version>`spring-servlet.xml，注意必须开启注解，即xml要有<annotation-driven />
**[html]**[view plain](http://blog.csdn.net/chenaschen/article/details/51291566#)[copy](http://blog.csdn.net/chenaschen/article/details/51291566#)[print](http://blog.csdn.net/chenaschen/article/details/51291566#)[?](http://blog.csdn.net/chenaschen/article/details/51291566#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1668701/fork)
- <annotation-driven/>
![](http://static.blog.csdn.net/images/save_snippets_01.png)
`<annotation-driven />`
**[html]**[view plain](http://blog.csdn.net/chenaschen/article/details/51291566#)[copy](http://blog.csdn.net/chenaschen/article/details/51291566#)[print](http://blog.csdn.net/chenaschen/article/details/51291566#)[?](http://blog.csdn.net/chenaschen/article/details/51291566#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1668701/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beans:beansxmlns="http://www.springframework.org/schema/mvc"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:beans="http://www.springframework.org/schema/beans"
- xmlns:context="http://www.springframework.org/schema/context"
- xsi:schemaLocation="http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc.xsd  
-         http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd  
-         http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context.xsd">
- 
-     <!-- DispatcherServlet Context: defines this servlet's request-processing   
-         infrastructure -->
- 
- <!-- Enables the Spring MVC @Controller programming model -->
- <annotation-driven/>
- 
-     <!-- Handles HTTP GET requests for /resources/** by efficiently serving   
-         up static resources in the ${webappRoot}/resources directory -->
- <resourcesmapping="/resources/**"location="/resources/"/>
- 
-     <!-- Resolves views selected for rendering by @Controllers to .jsp resources   
-         in the /WEB-INF/views directory -->
- <beans:bean
- class="org.springframework.web.servlet.view.InternalResourceViewResolver">
- <beans:propertyname="prefix"value="/WEB-INF/views/"/>
- <beans:propertyname="suffix"value=".jsp"/>
- </beans:bean>
- <context:component-scanbase-package="org.as.asjee"use-default-filters="false">
- <context:include-filtertype="annotation"expression="org.springframework.stereotype.Controller"/>
- </context:component-scan>
- 
- </beans:beans>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8"?>
<beans:beans xmlns="http://www.springframework.org/schema/mvc"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:beans="http://www.springframework.org/schema/beans"
	xmlns:context="http://www.springframework.org/schema/context"
	xsi:schemaLocation="http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc.xsd
		http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
		http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context.xsd">
	<!-- DispatcherServlet Context: defines this servlet's request-processing 
		infrastructure -->
	<!-- Enables the Spring MVC @Controller programming model -->
	<annotation-driven />
	<!-- Handles HTTP GET requests for /resources/** by efficiently serving 
		up static resources in the ${webappRoot}/resources directory -->
	<resources mapping="/resources/**" location="/resources/" />
	<!-- Resolves views selected for rendering by @Controllers to .jsp resources 
		in the /WEB-INF/views directory -->
	<beans:bean
		class="org.springframework.web.servlet.view.InternalResourceViewResolver">
		<beans:property name="prefix" value="/WEB-INF/views/" />
		<beans:property name="suffix" value=".jsp" />
	</beans:bean>
	<context:component-scan base-package="org.as.asjee" use-default-filters="false">
		<context:include-filter type="annotation" expression="org.springframework.stereotype.Controller"/>
	</context:component-scan>
</beans:beans>
```
## 2、异常统一处理类
**[java]**[view plain](http://blog.csdn.net/chenaschen/article/details/51291566#)[copy](http://blog.csdn.net/chenaschen/article/details/51291566#)[print](http://blog.csdn.net/chenaschen/article/details/51291566#)[?](http://blog.csdn.net/chenaschen/article/details/51291566#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1668701/fork)
- package org.as.asjee.core.exception;  
- 
- import java.sql.SQLException;  
- 
- import javax.servlet.http.HttpServletRequest;  
- 
- import org.as.asjee.core.log.AsJEELogger;  
- import org.as.asjee.core.log.AsJEELoggerFactory;  
- import org.springframework.web.bind.annotation.ControllerAdvice;  
- import org.springframework.web.bind.annotation.ExceptionHandler;  
- import org.springframework.web.bind.annotation.ResponseBody;  
- import org.springframework.web.servlet.ModelAndView;  
- 
- /**
-  * 捕获异常统一处理
-  * @description TODO
-  * @author chen.gs
-  * @create date 2016年4月28日
-  * @modified by 
-  * @modify date
-  * @version v1.0
-  */
- @ControllerAdvice
- publicclass GlobalExceptionHandler {  
- 
- privatefinalstatic AsJEELogger LOG = AsJEELoggerFactory.getLogger(GlobalExceptionHandler.class);  
- 
- privatefinalstatic String EXPTION_MSG_KEY = "message";  
- 
- @ExceptionHandler(BusinessException.class)  
- @ResponseBody
- publicvoid handleBizExp(HttpServletRequest request, Exception ex){  
-         LOG.info("Business exception handler  " + ex.getMessage() );  
-         request.getSession(true).setAttribute(EXPTION_MSG_KEY, ex.getMessage());  
-     }  
- 
- @ExceptionHandler(SQLException.class)  
- public ModelAndView handSql(Exception ex){  
-         LOG.info("SQL Exception " + ex.getMessage());  
-         ModelAndView mv = new ModelAndView();  
-         mv.addObject("message", ex.getMessage());  
-         mv.setViewName("sql_error");  
- return mv;  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package org.as.asjee.core.exception;
import java.sql.SQLException;
import javax.servlet.http.HttpServletRequest;
import org.as.asjee.core.log.AsJEELogger;
import org.as.asjee.core.log.AsJEELoggerFactory;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;
/**
 * 捕获异常统一处理
 * @description TODO
 * @author chen.gs
 * @create date 2016年4月28日
 * @modified by 
 * @modify date
 * @version v1.0
 */
@ControllerAdvice
public class GlobalExceptionHandler {
	
	private final static AsJEELogger LOG = AsJEELoggerFactory.getLogger(GlobalExceptionHandler.class);
	
	private final static String EXPTION_MSG_KEY = "message";
	
	@ExceptionHandler(BusinessException.class)
	@ResponseBody
	public void handleBizExp(HttpServletRequest request, Exception ex){
		LOG.info("Business exception handler  " + ex.getMessage() );
		request.getSession(true).setAttribute(EXPTION_MSG_KEY, ex.getMessage());
	}
	
	@ExceptionHandler(SQLException.class)
	public ModelAndView handSql(Exception ex){
		LOG.info("SQL Exception " + ex.getMessage());
		ModelAndView mv = new ModelAndView();
		mv.addObject("message", ex.getMessage());
		mv.setViewName("sql_error");
		return mv;
	}
}
```
自定义异常类BussinessException.java
**[java]**[view plain](http://blog.csdn.net/chenaschen/article/details/51291566#)[copy](http://blog.csdn.net/chenaschen/article/details/51291566#)[print](http://blog.csdn.net/chenaschen/article/details/51291566#)[?](http://blog.csdn.net/chenaschen/article/details/51291566#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1668701/fork)
- package org.as.asjee.core.exception;  
- 
- /**
-  * 业务异常
-  * @description TODO
-  * @author chen.gs
-  * @create date 2016年4月28日
-  * @modified by 
-  * @modify date
-  * @version v1.0
-  */
- publicclass BusinessException extends Exception{  
- 
- privatestaticfinallong serialVersionUID = 1L;  
- 
- //业务类型
- private String bizType;  
- //业务代码
- privateint bizCode;  
- //错误信息
- private String message;  
- 
- public BusinessException(String bizType, int bizCode, String message){  
- super(message);  
- this.bizType = bizType;  
- this.bizCode = bizCode;  
- this.message = message;  
-     }  
- 
- public BusinessException(String message){  
- super(message);  
- this.bizType = "";  
- this.bizCode = -1;  
- this.message = message;  
-     }  
- 
- public BusinessException(String bizType, String message){  
- super(message);  
- this.bizType = bizType;  
- this.bizCode = -1;  
- this.message = message;  
-     }  
- 
- public BusinessException(int bizCode, String message){  
- super(message);  
- this.bizType = "";  
- this.bizCode = bizCode;  
- this.message = message;  
-     }  
- 
- public String getBizType() {  
- return bizType;  
-     }  
- 
- publicvoid setBizType(String bizType) {  
- this.bizType = bizType;  
-     }  
- 
- publicint getBizCode() {  
- return bizCode;  
-     }  
- 
- publicvoid setBizCode(int bizCode) {  
- this.bizCode = bizCode;  
-     }  
- 
- public String getMessage() {  
- return message;  
-     }  
- 
- publicvoid setMessage(String message) {  
- this.message = message;  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package org.as.asjee.core.exception;
/**
 * 业务异常
 * @description TODO
 * @author chen.gs
 * @create date 2016年4月28日
 * @modified by 
 * @modify date
 * @version v1.0
 */
public class BusinessException extends Exception{
	private static final long serialVersionUID = 1L;
	
	//业务类型
	private String bizType;
	//业务代码
	private int bizCode;
	//错误信息
	private String message;
	
	public BusinessException(String bizType, int bizCode, String message){
		super(message);
		this.bizType = bizType;
		this.bizCode = bizCode;
		this.message = message;
	}
	public BusinessException(String message){
		super(message);
		this.bizType = "";
		this.bizCode = -1;
		this.message = message;
	}
	public BusinessException(String bizType, String message){
		super(message);
		this.bizType = bizType;
		this.bizCode = -1;
		this.message = message;
	}
	
	public BusinessException(int bizCode, String message){
		super(message);
		this.bizType = "";
		this.bizCode = bizCode;
		this.message = message;
	}
	public String getBizType() {
		return bizType;
	}
	public void setBizType(String bizType) {
		this.bizType = bizType;
	}
	public int getBizCode() {
		return bizCode;
	}
	public void setBizCode(int bizCode) {
		this.bizCode = bizCode;
	}
	public String getMessage() {
		return message;
	}
	public void setMessage(String message) {
		this.message = message;
	}
}
```
## 3、controller
**[java]**[view plain](http://blog.csdn.net/chenaschen/article/details/51291566#)[copy](http://blog.csdn.net/chenaschen/article/details/51291566#)[print](http://blog.csdn.net/chenaschen/article/details/51291566#)[?](http://blog.csdn.net/chenaschen/article/details/51291566#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1668701/fork)
- package org.as.asjee.core.security.web;  
- 
- import java.sql.SQLException;  
- 
- import javax.annotation.Resource;  
- 
- import org.as.asjee.core.exception.BusinessException;  
- import org.as.asjee.core.security.model.User;  
- import org.as.asjee.core.security.service.UserService;  
- import org.as.asjee.core.service.ServiceFacade;  
- import org.as.asjee.core.web.AbstractController;  
- import org.springframework.stereotype.Controller;  
- import org.springframework.web.bind.annotation.RequestMapping;  
- 
- @Controller
- @RequestMapping("/security/user")  
- publicclass UserController  extends AbstractController<User>{  
- 
- @Resource
- private UserService userService;  
- @Resource
- private ServiceFacade serviceFacade;  
- 
- @RequestMapping("login")  
- public String login() {   
- return"login";  
-     }  
- 
- @RequestMapping("login2")  
- public String login2() throws Exception {  
- thrownew SQLException("出错鸟。。。。。。。。。");  
-     }     
- 
- @RequestMapping("login3")  
- public String login3() throws Exception {   
- thrownew BusinessException("业务执行异常");  
-     }  
- 
- //此方法抛出的异常不是由GlobalExceptionHandler处理
- //而是在catch块处理
- @RequestMapping("login4")  
- public String login4() {   
- try {  
- thrownew BusinessException("业务执行异常");  
-         } catch (BusinessException e) {  
-             e.printStackTrace();  
-         }  
- return"login";  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package org.as.asjee.core.security.web;
import java.sql.SQLException;
import javax.annotation.Resource;
import org.as.asjee.core.exception.BusinessException;
import org.as.asjee.core.security.model.User;
import org.as.asjee.core.security.service.UserService;
import org.as.asjee.core.service.ServiceFacade;
import org.as.asjee.core.web.AbstractController;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
@Controller
@RequestMapping("/security/user")
public class UserController  extends AbstractController<User>{
	@Resource
	private UserService userService;
	@Resource
	private ServiceFacade serviceFacade;
	@RequestMapping("login")
	public String login() { 
		return "login";
	}
	
	@RequestMapping("login2")
	public String login2() throws Exception {
		throw new SQLException("出错鸟。。。。。。。。。");
	}	
	
	@RequestMapping("login3")
	public String login3() throws Exception { 
		throw new BusinessException("业务执行异常");
	}
	
	//此方法抛出的异常不是由GlobalExceptionHandler处理
	//而是在catch块处理
	@RequestMapping("login4")
	public String login4() { 
		try {
			throw new BusinessException("业务执行异常");
		} catch (BusinessException e) {
			e.printStackTrace();
		}
		return "login";
	}
	
}
```
## 4、JSP页面
sql_error.jsp
**[html]**[view plain](http://blog.csdn.net/chenaschen/article/details/51291566#)[copy](http://blog.csdn.net/chenaschen/article/details/51291566#)[print](http://blog.csdn.net/chenaschen/article/details/51291566#)[?](http://blog.csdn.net/chenaschen/article/details/51291566#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1668701/fork)
- <%@ page language="java"contentType="text/html; charset=UTF-8" %>
- <!DOCTYPE html">
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=UTF-8">
- <title>Insert title here</title>
- </head>
- <body>
- <h1>ERROR MESSAGE</h1>
- <p>${message}</p>
- </body>
- </html>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<%@ page language="java" contentType="text/html; charset=UTF-8" %>
<!DOCTYPE html">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
	<h1>ERROR MESSAGE</h1>
	<p>${message}</p>
</body>
</html>
```
## 5、简要说明
在Controller中抛出的异常，当没有被catch处理时，GlobalExceptionHandler中定义的处理方法可以起作用，在方法写明注解@ExceptionHandler，并注明其异常类即可。此种方法不仅可以作用于Controller，同样的在DAO层、service层也可，都可以由GlobalExceptionHandler进行处理。此种写法减少代码的入侵，值得推荐。
异常的统一处理只是注解ControllerAdvice用处之一，有兴趣了解更多的，请到spring官网查阅。
