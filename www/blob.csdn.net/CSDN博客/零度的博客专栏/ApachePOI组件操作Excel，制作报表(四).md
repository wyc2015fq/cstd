# ApachePOI组件操作Excel，制作报表(四) - 零度的博客专栏 - CSDN博客
2016年05月05日 16:14:57[零度anngle](https://me.csdn.net/zmx729618)阅读数：364
[上一篇](http://sarin.iteye.com/blog/853418)我们介绍了如何制作复杂报表的分析和设计，本篇结合Spring MVC框架来说一下如何在Web层使用POI组件来动态生成报表供用户查看或下载。为什么选择Spring MVC框架，理由很简单，因为Spring的MVC框架对Excel视图的扩展使用的正是POI组件，那么使用Spring来整合就非常方便了，当然其它的框架都有自己的处理办法，这个可以具体咨询其对视图扩展支持的配置方法。
    使用Spring对Excel的支持，需要扩展MVC的视图组件，就是要创建一个自定义的View，这个在后面会详细介绍，通过这个view，就可以将Excel报表展现出来了，非常方便。
    基于Spring MVC的程序，那么首先要构建起来Spring MVC的环境，这个很简单，下面我们来看看如何搭建Spring MVC环境。我们使用Spring 3.0.5的库，但是配置方式不用注解，用传统的配置方式来说明。
    使用Maven管理项目，引入必要的依赖，如下： 
![](http://dl.iteye.com/upload/attachment/382126/bda03c55-fd58-31db-ad10-e92a2bf1cdef.jpg)
    并建立如下的项目结构： 
![](http://dl.iteye.com/upload/attachment/382128/6a2ddff9-6574-32f8-8e4f-05355f5bff8d.jpg)
    在web.xml描述符中，首先配置Spring配置文件的路径： 
Xml代码 ![复制代码](http://sarin.iteye.com/images/icon_copy.gif)![收藏代码](http://sarin.iteye.com/images/icon_star.png)![](http://sarin.iteye.com/images/spinner.gif)
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>classpath:org/ourpioneer/spring/*.xml,classpath:org/ourpioneer/spring/*/*.xml</param-value>
- </context-param>
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
	<context-param>
		<param-name>contextConfigLocation</param-name>
	<param-value>classpath:org/ourpioneer/spring/*.xml,classpath:org/ourpioneer/spring/*/*.xml</param-value>
	</context-param>
	<listener>
	<listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
	</listener>
    其次配置一个字符过滤器，使用Spring提供的就好，这样就不用过多考虑字符编码的问题了： 
Xml代码 ![复制代码](http://sarin.iteye.com/images/icon_copy.gif)![收藏代码](http://sarin.iteye.com/images/icon_star.png)![](http://sarin.iteye.com/images/spinner.gif)
- <filter>
- <filter-name>EncodingFilter</filter-name>
- <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
- <init-param>
- <param-name>encoding</param-name>
- <param-value>UTF-8</param-value>
- </init-param>
- <init-param>
- <param-name>forceEncoding</param-name>
- <param-value>true</param-value>
- </init-param>
- </filter>
- <filter-mapping>
- <filter-name>EncodingFilter</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
	<filter>
		<filter-name>EncodingFilter</filter-name>
		<filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
		<init-param>
			<param-name>encoding</param-name>
			<param-value>UTF-8</param-value>
		</init-param>
		<init-param>
			<param-name>forceEncoding</param-name>
			<param-value>true</param-value>
		</init-param>
	</filter>
	<filter-mapping>
		<filter-name>EncodingFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
    最后是Spring MVC的ServletDispatcher配置了： 
Xml代码 ![复制代码](http://sarin.iteye.com/images/icon_copy.gif)![收藏代码](http://sarin.iteye.com/images/icon_star.png)![](http://sarin.iteye.com/images/spinner.gif)
- <servlet>
- <servlet-name>board</servlet-name>
- <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
- <init-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>/WEB-INF/Dispatcher-Servlet.xml</param-value>
- </init-param>
- </servlet>
- <servlet-mapping>
- <servlet-name>board</servlet-name>
- <url-pattern>*.do</url-pattern>
- </servlet-mapping>
	<servlet>
		<servlet-name>board</servlet-name>
	<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
		<init-param>
			<param-name>contextConfigLocation</param-name>
			<param-value>/WEB-INF/Dispatcher-Servlet.xml</param-value>
		</init-param>
	</servlet>
	<servlet-mapping>
		<servlet-name>board</servlet-name>
		<url-pattern>*.do</url-pattern>
	</servlet-mapping>
    我们配置了一个初始化文件，在Servlet加载时会加载这个配置文件，并使用*.do风格的映射地址。在这个初始配置文件中，主要配置了映射和拦截器等信息。内容如下：
Xml代码 ![复制代码](http://sarin.iteye.com/images/icon_copy.gif)![收藏代码](http://sarin.iteye.com/images/icon_star.png)![](http://sarin.iteye.com/images/spinner.gif)
- <!-- 扩展命名拦截器 -->
- <beanid="extensionInterceptor"
- class="org.ourpioneer.excel.web.interceptor.ExtensionInterceptor"></bean>
- <!-- 控制器映射 -->
- <beanclass="org.springframework.web.servlet.handler.SimpleUrlHandlerMapping">
- <propertyname="interceptors">
- <list>
- <refbean="extensionInterceptor"/>
- </list>
- </property>
- <propertyname="mappings">
- <props>
- <propkey="/excel.do">ExcelController</prop>
- </props>
- </property>
- </bean>
		<!-- 扩展命名拦截器 -->
	<bean id="extensionInterceptor"
	class="org.ourpioneer.excel.web.interceptor.ExtensionInterceptor"></bean>
	<!-- 控制器映射 -->
	<bean class="org.springframework.web.servlet.handler.SimpleUrlHandlerMapping">
		<property name="interceptors">
			<list>
				<ref bean="extensionInterceptor" />
			</list>
		</property>
		<property name="mappings">
			<props>
				<prop key="/excel.do">ExcelController</prop>
			</props>
		</property>
	</bean>
    其中扩展名拦截器是对生成Excel文件后的扩展名进行处理的，这个后面会说，下面就是配置映射，这里我们只有一个测试，那么就写一个映射即可，最后使用这个地址来测试，配置好映射信息后，那么就要配置其中的每个映射，那么来看看ExcelController的配置：
Xml代码 ![复制代码](http://sarin.iteye.com/images/icon_copy.gif)![收藏代码](http://sarin.iteye.com/images/icon_star.png)![](http://sarin.iteye.com/images/spinner.gif)
- <beanid="ExcelController"class="org.ourpioneer.excel.web.controller.ExcelController">
- <propertyname="reportService"ref="reportService"/>
- <propertyname="reportView"ref="reportView"/>
- lt;/bean>
		<bean id="ExcelController" class="org.ourpioneer.excel.web.controller.ExcelController">
		<property name="reportService" ref="reportService" />
		<property name="reportView" ref="reportView" />
	</bean>
    这个也很简单，注入两个属性，一个service和一个view。为什么要用view，因为Spring MVC已经和POI进行了整合，这两个bean分别配置一下即可，这里不再说明了。详见附件中的完整源码。下面来看看控制器：
Java代码 ![复制代码](http://sarin.iteye.com/images/icon_copy.gif)![收藏代码](http://sarin.iteye.com/images/icon_star.png)![](http://sarin.iteye.com/images/spinner.gif)
- package org.ourpioneer.excel.web.controller;  
- import java.util.HashMap;  
- import java.util.List;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- import org.ourpioneer.excel.bean.CruiseServiceLocation;  
- import org.ourpioneer.excel.service.ReportService;  
- import org.ourpioneer.excel.web.views.ReportView;  
- import org.springframework.web.servlet.ModelAndView;  
- import org.springframework.web.servlet.mvc.AbstractController;  
- 
- /**
-  * Excel处理控制器
-  * 
-  * @author Nanlei
-  * 
-  */
- publicclass ExcelController extends AbstractController {  
- private ReportView reportView;  
- private ReportService reportService;  
- publicvoid setReportView(ReportView reportView) {  
- this.reportView = reportView;  
-     }  
- publicvoid setReportService(ReportService reportService) {  
- this.reportService = reportService;  
-     }  
- @Override
- protected ModelAndView handleRequestInternal(HttpServletRequest request,  
-             HttpServletResponse response) throws Exception {  
-         HashMap model = new HashMap();  
-         List<CruiseServiceLocation> cslList = reportService  
-                 .getCruiseLocationList();  
-         model.put("cruiseServiceLocationList", cslList);  
- returnnew ModelAndView(reportView, model);   
-     }  
- }  
```java
package org.ourpioneer.excel.web.controller;
import java.util.HashMap;
import java.util.List;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.ourpioneer.excel.bean.CruiseServiceLocation;
import org.ourpioneer.excel.service.ReportService;
import org.ourpioneer.excel.web.views.ReportView;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.web.servlet.mvc.AbstractController;
/**
 * Excel处理控制器
 * 
 * @author Nanlei
 * 
 */
public class ExcelController extends AbstractController {
	private ReportView reportView;
	private ReportService reportService;
	public void setReportView(ReportView reportView) {
		this.reportView = reportView;
	}
	public void setReportService(ReportService reportService) {
		this.reportService = reportService;
	}
	@Override
	protected ModelAndView handleRequestInternal(HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		HashMap model = new HashMap();
		List<CruiseServiceLocation> cslList = reportService
				.getCruiseLocationList();
		model.put("cruiseServiceLocationList", cslList);
		return new ModelAndView(reportView, model); 
	}
}
```
    控制器扩展了AbstractController，覆盖了handleRequestInternal方法，这样就可以编写我们自定义的处理代码了。类开始是对service和view的注入，业务代码中从Service中获取了数据，然后放入model中，这个model是一个HashMap，也是Spring MVC中传递数据的纽带，使用方便。就像Struts2中的值栈一样，不过这个是显式使用，而值栈更像是隐式存在的数据桥梁。那么来看看Service：
Java代码 ![复制代码](http://sarin.iteye.com/images/icon_copy.gif)![收藏代码](http://sarin.iteye.com/images/icon_star.png)![](http://sarin.iteye.com/images/spinner.gif)
- package org.ourpioneer.excel.service;  
- 
- import java.util.Arrays;  
- import java.util.List;  
- 
- import org.ourpioneer.excel.bean.CruiseServiceLocation;  
- 
- /**
-  * 报表数据业务类
-  * 
-  * @author Nanlei
-  * 
-  */
- publicclass ReportService {  
- /**
-      * 获取数据
-      * 
-      * @return
-      */
- public List<CruiseServiceLocation> getCruiseLocationList() {  
-         CruiseServiceLocation csl[] = new CruiseServiceLocation[21];  
-         csl[0] = new CruiseServiceLocation("T001", "北京市", "北京总部", "bj", "清华大学",  
- 20);  
- ...  
-         csl[20] = new CruiseServiceLocation("", "", "", "", "", 0);// 合并算法捕捉最后一行有问题，增补一行无效数据，计算时去除
- 
- return Arrays.asList(csl);  
-     }  
- }  
```java
package org.ourpioneer.excel.service;
import java.util.Arrays;
import java.util.List;
import org.ourpioneer.excel.bean.CruiseServiceLocation;
/**
 * 报表数据业务类
 * 
 * @author Nanlei
 * 
 */
public class ReportService {
	/**
	 * 获取数据
	 * 
	 * @return
	 */
	public List<CruiseServiceLocation> getCruiseLocationList() {
		CruiseServiceLocation csl[] = new CruiseServiceLocation[21];
		csl[0] = new CruiseServiceLocation("T001", "北京市", "北京总部", "bj", "清华大学",
				20);
...
		csl[20] = new CruiseServiceLocation("", "", "", "", "", 0);// 合并算法捕捉最后一行有问题，增补一行无效数据，计算时去除
		return Arrays.asList(csl);
	}
}
```
    这是很简单的类了，数据是我们准备好的，实际应用中就可以操作数据库了，这都没是由什么难度。下面来看一下展示Excel的View： 
Java代码 ![复制代码](http://sarin.iteye.com/images/icon_copy.gif)![收藏代码](http://sarin.iteye.com/images/icon_star.png)![](http://sarin.iteye.com/images/spinner.gif)
- package org.ourpioneer.excel.web.views;  
- /**
-  * 报表的Excel视图
-  * 
-  * @author Nanlei
-  * 
-  */
- publicclass ReportView extends AbstractExcelView {  
- 
- @Override
- protectedvoid buildExcelDocument(Map<String, Object> model,  
-             HSSFWorkbook workbook, HttpServletRequest request,  
-             HttpServletResponse response) throws Exception {  
- // 从model中获取数据对象
-         List<CruiseServiceLocation> cruiseServiceLocationList = (List<CruiseServiceLocation>) model  
-                 .get("cruiseServiceLocationList");  
- …  
-     }  
- }  
```java
package org.ourpioneer.excel.web.views;
/**
 * 报表的Excel视图
 * 
 * @author Nanlei
 * 
 */
public class ReportView extends AbstractExcelView {
	@Override
	protected void buildExcelDocument(Map<String, Object> model,
			HSSFWorkbook workbook, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		// 从model中获取数据对象
		List<CruiseServiceLocation> cruiseServiceLocationList = (List<CruiseServiceLocation>) model
				.get("cruiseServiceLocationList");
…
	}
}
```
    中间的处理代码不贴出了，这和以前的都是一样的。主要来看这个视图的写法，因为要生成Excel，那么就扩展AbstractExcelView，覆盖其中的buildExcelDocument方法，它为我们提供了存储数据的model，创建Excel的POI API，还有请求和响应对象，这都是很熟悉的内容了。首先是从model对象中取出我们放置的数据列表，下面是用POI来制作报表，之前已经写过，都是一模一样的代码，就不贴出了。
    至此，配置结束，就剩下一个问题了，触发该请求后，数据填充完毕要展示给用户看，可此时还没有配置文件类型的信息，要给用户合理的反馈，那么就需要对处理后的结果进行最后的处理，这就是前面提到的拦截器。
Java代码 ![复制代码](http://sarin.iteye.com/images/icon_copy.gif)![收藏代码](http://sarin.iteye.com/images/icon_star.png)![](http://sarin.iteye.com/images/spinner.gif)
- package org.ourpioneer.excel.web.interceptor;  
- import java.net.URLEncoder;  
- import java.text.SimpleDateFormat;  
- import java.util.Date;  
- import java.util.Map;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- import org.apache.commons.lang.StringUtils;  
- import org.springframework.web.servlet.ModelAndView;  
- import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;  
- 
- /**
-  * 扩展名命名处理拦截器
-  * 
-  * @author Nanlei
-  * 
-  */
- publicclass ExtensionInterceptor extends HandlerInterceptorAdapter {  
- /**
-      * 用在流程处理之后
-      */
- @Override
- publicvoid postHandle(HttpServletRequest request,  
-             HttpServletResponse response, Object handler,  
-             ModelAndView modelAndView) throws Exception {  
- // 获取Controller过来的Model对象
-         Map model = modelAndView.getModel();  
- // 获取请求URI
-         String uri = request.getRequestURI();  
- // 生成文件的名称
-         String exportFileName = null;  
- if (uri.endsWith("excel.do")) {  
-             String time = new SimpleDateFormat("yyyyMMddHHmmss")  
-                     .format(new Date());  
-             exportFileName = "报表" + time + ".xls";  
-         }  
- if (StringUtils.isNotEmpty(exportFileName)) {  
-             response.setHeader("Content-Disposition", "attachment; filename="
-                     + URLEncoder.encode(exportFileName, "UTF-8"));  
-         }  
- super.postHandle(request, response, handler, modelAndView);  
-     }  
- }  
```java
package org.ourpioneer.excel.web.interceptor;
import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Map;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.commons.lang.StringUtils;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;
/**
 * 扩展名命名处理拦截器
 * 
 * @author Nanlei
 * 
 */
public class ExtensionInterceptor extends HandlerInterceptorAdapter {
	/**
	 * 用在流程处理之后
	 */
	@Override
	public void postHandle(HttpServletRequest request,
			HttpServletResponse response, Object handler,
			ModelAndView modelAndView) throws Exception {
		// 获取Controller过来的Model对象
		Map model = modelAndView.getModel();
		// 获取请求URI
		String uri = request.getRequestURI();
		// 生成文件的名称
		String exportFileName = null;
		if (uri.endsWith("excel.do")) {
			String time = new SimpleDateFormat("yyyyMMddHHmmss")
					.format(new Date());
			exportFileName = "报表" + time + ".xls";
		}
		if (StringUtils.isNotEmpty(exportFileName)) {
			response.setHeader("Content-Disposition", "attachment; filename="
					+ URLEncoder.encode(exportFileName, "UTF-8"));
		}
		super.postHandle(request, response, handler, modelAndView);
	}
}
```
    Spring的拦截器也相当简练，看看这个方法名就知道这是在流程处理结束之后的拦截器，而流程处理之前可以不配置，也就是拦截器可以配置成前后不同的处理方法，同时允许链接拦截器。它不像Struts2的拦截器，处理前通过1，2，3，处理后再通过3，2，1要走两次相同内容。更为高效。
    在这里，我们可以拿到model对象从其中取出一些控制器传递过来的参数，当然这是根据需要使用的，之后获取到请求uri，来处理这个uri，是excel.do结尾的请求，我们要作为附件下载来体现，那么就要重新命名，然后设置响应对象，这也就是一般下载流程的最后一个步骤，很简单。
    写完代码，在Maven中配置项目 
![](http://dl.iteye.com/upload/attachment/382130/e590c966-129a-33d9-b3c6-c4e698b1cbb9.jpg)
    这部分的介绍就全部结束了。
            
