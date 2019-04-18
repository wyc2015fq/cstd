# SpringMVC同时支持多视图(JSP,Velocity,Freemarker等)的一种思路实现 - z69183787的专栏 - CSDN博客
2014年10月24日 14:08:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16217
**第一种方式：**
在基于SpringMVC的项目中有时需要同时使用多种视图格式，如jsp,velocity及freemarker等，通过不同的请求路径配置规则，映射到不同的视图文件。下面我提供一种思路，通过视图模板文件后缀名的方式来进行处理。例如：
@RequestMapping(value = "/hello") 
public String jspTest(HttpServletRequest request, ModelMap map) { 
    return "demo.jsp"; 
} 
将通过”demo.jsp”的后缀自动使用jsp的视图解析器。
下面描述下详细思路：
### 一.自定义视图解析器
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.jeedev.common.web.springmvc.view;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.web.servlet.View;
import org.springframework.web.servlet.ViewResolver;
/**
 * 自定义视图解析(通过配置实现多视图整合,如jsp,velocity,freemarker,pdf,excel...)
 * @author huligong
 *
 */
public class JeeDevViewResolver implements ViewResolver  {
    
    private static Log logger = LogFactory.getLog(JeeDevViewResolver.class);
    public View resolveViewName(String viewName, Locale locale) throws Exception {
       for(Map.Entry<Set<String>, ViewResolver> map : viewResolverMap.entrySet()){
           Set<String> suffixs = map.getKey();
           for(String suffix : suffixs){
               if (viewName.endsWith(suffix)){
                   ViewResolver viewResolver = map.getValue();
                   if(null != viewResolver){
                       if (logger.isDebugEnabled()) {
                            logger.debug("found viewResolver '" + viewResolver + "' for viewName '" + viewName+ "'");
                        }
                       return viewResolver.resolveViewName(viewName, locale);
                   }
               }
           }
       }
       
       if(defaultViewResolver != null){
           return defaultViewResolver.resolveViewName(viewName, locale);
       }
       // to allow for ViewResolver chaining
       return null;
    }
    
    
    private Map<Set<String>,ViewResolver> viewResolverMap = new HashMap<Set<String>,ViewResolver>();
    
    private ViewResolver defaultViewResolver = null;
    public Map<Set<String>, ViewResolver> getViewResolverMap() {
        return viewResolverMap;
    }
    public void setViewResolverMap(Map<Set<String>, ViewResolver> viewResolverMap) {
        this.viewResolverMap = viewResolverMap;
    }
    public ViewResolver getDefaultViewResolver() {
        return defaultViewResolver;
    }
    public void setDefaultViewResolver(ViewResolver defaultViewResolver) {
        this.defaultViewResolver = defaultViewResolver;
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
在自定义视图解析器(其实是视图中转器)中，通过对视图文件的后缀判断(而不是请求地址的后缀)而返回给不同的视图解析器处理。
### 二.视图映射配置dispather-servlet.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<bean id="viewResolver" class="com.jeedev.common.web.springmvc.view.JeeDevViewResolver">
        <property name="defaultViewResolver" ref="beanNameViewResolver"/>
        <property name="viewResolverMap">
            <map>
                <entry>
                    <key>
                        <set>
                            <value>.jsp</value>
                        </set>
                    </key>
                    <ref bean="jspViewResolver"/>
                </entry>
                <entry>
                    <key>
                        <set>
                            <value>.vm</value>
                            <value>.htm</value>
                        </set>
                    </key>
                    <ref bean="velocityViewResolver"/>
                </entry>
                <entry>
                    <key><value>.ftl</value></key>
                    <ref bean="freeMarkerViewResolver"/>
                </entry>
            </map>
        </property>
    </bean>
    
    <bean id="beanNameViewResolver" class="org.springframework.web.servlet.view.BeanNameViewResolver"/>
    
    <bean id="jspViewResolver" class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>
        <property name="prefix" value="/WEB-INF/jsp/"/>
        <!--<property name="suffix" value=".jsp"/>-->
    </bean>
    
    <!--  Velocity ViewResolver Configuration -->
    <bean id="velocityViewResolver" class = "org.springframework.web.servlet.view.velocity.VelocityViewResolver">
         <property name="order" value="0" />
         <property name="contentType" value="text/html;charset=UTF-8" />
         <property name="requestContextAttribute" value="req"/>
    </bean>
    <!-- Velocity Configuration -->     
    <bean id="velocityConfig" class = "org.springframework.web.servlet.view.velocity.VelocityConfigurer">
         <property name="configLocation" value="/WEB-INF/velocity.properties"/>
         <property name="resourceLoaderPath" value="/" />
    </bean>
    
    <!--  FreeMarker ViewResolver Configuration -->
    <bean id="freeMarkerViewResolver" class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">  
        <property name="order" value="0" />
        <property name="viewClass" value="org.springframework.web.servlet.view.freemarker.FreeMarkerView"/>  
    </bean> 
    
    
    <!-- FreeMarker Configuration -->     
    <bean id="freemarkerConfig"  class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">  
       <property name="configLocation" value="/WEB-INF/freemarker.properties"/>
        <property name="templateLoaderPath" value="/WEB-INF/ftl/"/>  
         <property name="freemarkerVariables">
             <map>
                <entry key="xml_escape" value-ref="fmXmlEscape"/>
            </map>
         </property>
    </bean>   
    
    <bean id="fmXmlEscape" class="freemarker.template.utility.XmlEscape"/>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
这里我配置成所有返回.jsp的视图模板请求都转到JSP视图解析器jspViewResolver中去处理，返回.htm,.vm视图模板请求都转到velocity视图解析器中去处理。
### 三.控制器代码示例
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.jeedev.demo.view;
import javax.servlet.http.HttpServletRequest;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
/**
 * 多视图解析器支持示例
 * @author huligong
 *
 */
@Controller
@RequestMapping(value = "/demo")
public class JeeDevViewResolverTestController {
    private static Log logger = LogFactory.getLog(JeeDevViewResolverTestController.class);
    
    @RequestMapping(value = "/test1")
    public String test1(HttpServletRequest request, ModelMap map) {
        logger.info("使用JSP视图解析器");
        map.put("name", "hello world");
        return "jspTest.jsp";
    }
    
    @RequestMapping(value = "/test2")
    public String test2(HttpServletRequest request, ModelMap map) {
        logger.info("使用Freemarker视图解析器");
        map.put("name", "hello world");
        return "hello.ftl";
    }
    
    @RequestMapping(value = "/test3")
    public String test3(HttpServletRequest request, ModelMap map) {
        logger.info("使用Velocity视图解析器");
        map.put("name", "hello world");
        return "/html/demo.htm";
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
### 四.测试请求
我在web.xml里给DispatcherServlet添加了/view/*，所以，只要在请求前加上/view就可以了。
[http://localhost:8080/view/demo/test1](http://localhost:8080/view/demo/test1)
[http://localhost:8080/view/demo/test2](http://localhost:8080/view/demo/test2)
[http://localhost:8080/view/demo/test3](http://localhost:8080/view/demo/test3)
皆验证通过。
第二种方式
[说明一下目录结构
![](http://www.blogjava.net/images/blogjava_net/sxyx2008/springmvc.jpg)
Spring配置文件](http://blog.csdn.net/truong/article/details/17339907)
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:p="http://www.springframework.org/schema/p"
    xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="
    http://www.springframework.org/schema/beans
    http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
    http://www.springframework.org/schema/context
    http://www.springframework.org/schema/context/spring-context-3.0.xsd">
<context:component-scan
base-package="com.spring.action"/>
<!--
        org.springframework.web.servlet.view.ResourceBundleViewResolver
        用于多个视图集成时,ResourceBundleViewResolver是通过解析资源文件来解析请求输出文件的。
        <property name="basename" value="views"></property>,即表示在/WEB-INF/classes路径下有一个
        views.properties文件,本例中views.properties的内容为
        welcome.(class)=org.springframework.web.servlet.view.velocity.VelocityView
        welcome.url=welcome.vm
        freemarker.(class)=org.springframework.web.servlet.view.freemarker.FreeMarkerView
        freemarker.url=freemarker.ftl
-->
<bean class="org.springframework.web.servlet.view.ResourceBundleViewResolver">
<property name="basename" value="views"></property>
<!--
            <property name="order" value="0"></property>
-->
</bean>
<!-- jsp视图解析器 -->
<bean id="jspViewResolver" class="org.springframework.web.servlet.view.InternalResourceViewResolver">
<property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>
<property name="prefix" value="/"/>
<property name="suffix" value=".jsp"/>
</bean>
<!-- velocity视图解析器 -->
<bean id="velocityViewResolver" class="org.springframework.web.servlet.view.velocity.VelocityViewResolver">
<property name="cache" value="true"/>
<property name="prefix" value="/"/>
<property name="suffix" value=".vm"/>
</bean>
<!-- velocity环境配置 -->
<bean id="velocityConfig" class="org.springframework.web.servlet.view.velocity.VelocityConfigurer">
<!-- velocity配置文件路径 -->
<property name="configLocation" value="/WEB-INF/velocity.properties"/>
<!-- velocity模板路径 -->
<property name="resourceLoaderPath" value="/WEB-INF/velocity/"/>
</bean>
<!-- FreeMarker环境配置 -->
<bean id="freemarkerConfig" class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">
<!-- freemarker模板位置 -->
<property name="templateLoaderPath" value="/WEB-INF/freemarker/"/>
</bean>
<!-- FreeMarker视图解析 -->
<bean id="freeMarkerViewResolver" class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
<property name="cache" value="true"/>
<property name="prefix" value="/"/>
<property name="suffix" value=".ftl"/>
</bean>
</beans>
views.properties
#welcome为modelAndView.setViewName("welcome");中的welcome   .(class)固定写法
welcome.(class)=org.springframework.web.servlet.view.velocity.VelocityView
#welcome.url 路径 welcome.vm模板名称
welcome.url=welcome.vm
#freemarker为modelAndView.setViewName("freemarker");中的freemarker   .(class)固定写法
freemarker.(class)=org.springframework.web.servlet.view.freemarker.FreeMarkerView
#freemarker.url 路径 freemarker.ftl模板名称
freemarker.url=freemarker.ftl
**第三种方式：自适应匹配，不存在则找下一种，使用orders**
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context"
	xmlns:mvc="http://www.springframework.org/schema/mvc"
	xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.2.xsd
        http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc-3.2.xsd">
	<context:component-scan base-package="test.**.controller"
		use-default-filters="false">
		<context:include-filter type="annotation"
			expression="org.springframework.stereotype.Controller" />
	</context:component-scan>
	<mvc:default-servlet-handler />
	<mvc:resources location="/resources/" mapping="/resources/**" />
	<mvc:annotation-driven>
		<!-- <mvc:argument-resolvers> <bean class="org.springframework.data.web.PagedResourcesAssemblerArgumentResolver" 
			/> </mvc:argument-resolvers> -->
		<mvc:message-converters>
			<bean class="org.springframework.http.converter.StringHttpMessageConverter">
				<property name="supportedMediaTypes">
					<list>
						<value>text/plain;charset=UTF-8</value>
						<value>text/html;charset=UTF-8</value>
					</list>
				</property>
			</bean>
			<bean
				class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
				<property name="supportedMediaTypes">
					<list>
						<value>text/html;charset=UTF-8</value>
						<value>application/json;charset=UTF-8</value>
					</list>
				</property>
			</bean>
		</mvc:message-converters>
	</mvc:annotation-driven>
	<bean id="multipartResolver"
		class="org.springframework.web.multipart.commons.CommonsMultipartResolver" />
	<!-- FreeMarker视图解析 如返回student。。在这里配置后缀名ftl和视图解析器。。-->
	<bean id="freemarkerConfig"
		class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">
		<property name="templateLoaderPath">  
            <value>/WEB-INF/ftl/</value>  
        </property>  
		<property name="defaultEncoding" value="UTF-8" />
		<property name="freemarkerVariables">
			<map>
				<entry key="xml_escape" value-ref="fmXmlEscape" />
			</map>
		</property>
		<property name="freemarkerSettings">
			<props>
				<!--<prop key="template_update_delay">3600</prop> -->
				<prop key="tag_syntax">auto_detect</prop>
				<!-- 设置标签类型 两种：[] 和 <> 。[] 这种标记解析要快些 -->
				<prop key="default_encoding">UTF-8</prop>
				<prop key="output_encoding">UTF-8</prop>
				<prop key="locale">zh_CN</prop>
				<prop key="date_format">yyyy-MM-dd</prop>
				<prop key="time_format">HH:mm:ss</prop>
				<prop key="datetime_format">yyyy-MM-dd HH:mm:ss</prop>
				<prop key="number_format">#.##</prop>
				<!-- 设置数字格式 以免出现 000.00 -->
				<prop key="classic_compatible">true</prop>
				<!-- 可以满足一般需要。默认情况变量为null则替换为空字符串，如果需要自定义，写上${empty!"EmptyValue of fbysss"}的形式即可 -->
				<prop key="template_exception_handler">html_debug</prop>
				<!-- ignore,debug,html_debug,rethrow -->
			</props>
		</property>
	</bean>
	<bean id="fmXmlEscape" class="freemarker.template.utility.XmlEscape" />
	
	<bean id="viewResolver"
		class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
		<property name="viewClass"
			value="org.springframework.web.servlet.view.freemarker.FreeMarkerView"></property>
		<property name="suffix" value=".ftl" />
		<property name="contentType" value="text/html;charset=utf-8" />
		<property name="exposeRequestAttributes" value="true" />
		<property name="exposeSessionAttributes" value="true" />
		<property name="exposeSpringMacroHelpers" value="true" />
		<property name="order" value="0" />
	</bean>
	
	<!-- --> 
	
	<!-- Velocity  -->
	<bean id="velocityConfig"
		class="org.springframework.web.servlet.view.velocity.VelocityConfigurer">
		<property name="configLocation" value="/WEB-INF/classes/velocity.properties" />
		<property name="resourceLoaderPath" value="/WEB-INF/vm/" />
	</bean>
	
	<bean id="velocityViewResolver"
		class="org.springframework.web.servlet.view.velocity.VelocityViewResolver">
		<property name="order" value="1" />
		<property name="contentType" value="text/html;charset=UTF-8" />
		<property name="requestContextAttribute" value="req" />
        <property name="suffix" value=".vm"/>
	</bean>
	<!-- --> 
	
	<bean id="beanNameViewResolver"
		class="org.springframework.web.servlet.view.BeanNameViewResolver">
		<property name="order" value="6"/>
	</bean>
	
	<bean id="JSPviewResolver"
		class="org.springframework.web.servlet.view.InternalResourceViewResolver">
		<property name="viewClass"
			value="org.springframework.web.servlet.view.JstlView" />
		<property name="prefix" value="/WEB-INF/jsp/" />
		<property name="suffix" value=".jsp" />
		<property name="order" value="10"/>
	</bean>
 	<!-- <bean id="viewResolver"
		class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
		<property name="defaultContentType" value="application/json" />
		<property name="mediaTypes">
			<map>
				<entry key="html" value="text/html" />
				<entry key="json" value="application/json" />
				<entry key="xml" value="application/xml" />
				<entry key="excel" value="application/excel" />
				<entry key="pdf" value="application/pdf" />
			</map>
		</property>
		<property name="viewResolvers">
			<list>
				<bean
					class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
					<property name="viewClass"
						value="org.springframework.web.servlet.view.freemarker.FreeMarkerView"></property>
					<property name="suffix" value=".ftl" />
					<property name="contentType" value="text/html;charset=utf-8" />
					<property name="exposeRequestAttributes" value="true" />
					<property name="exposeSessionAttributes" value="true" />
					<property name="exposeSpringMacroHelpers" value="true" />
				</bean>
				<bean
					class="org.springframework.web.servlet.view.InternalResourceViewResolver">
					<property name="viewClass"
						value="org.springframework.web.servlet.view.JstlView" />
					<property name="prefix" value="/WEB-INF/jsp/" />
					<property name="suffix" value=".jsp" />
				</bean>
				
			</list> 
		</property>
		<property name="defaultViews">
			<list>
				<bean
					class="org.springframework.web.servlet.view.json.MappingJackson2JsonView">
					<property name="prettyPrint" value="true" />
					<property name="contentType" value="text/plain" />
				</bean>
			</list>
		</property>
	</bean>
 -->
	<!--定义异常处理页面 -->
	<bean id="exceptionResolver"
		class="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">
		<property name="exceptionMappings">
			<props>
				<prop key="java.lang.Exception">outException</prop>
			</props>
		</property>
	</bean>
	
</beans>
```
