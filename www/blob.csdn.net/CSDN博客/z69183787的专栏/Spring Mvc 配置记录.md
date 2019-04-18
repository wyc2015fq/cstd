# Spring Mvc 配置记录 - z69183787的专栏 - CSDN博客
2017年01月20日 16:57:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：459
pom.xml（parent）
```
<project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <useDefaultDelimiters>false</useDefaultDelimiters>
        <servlet.version>2.5</servlet.version>
        <jsp.version>2.1</jsp.version>
        <spring.version>4.1.6.RELEASE</spring.version>
        <mybatis.version>3.2.0</mybatis.version>
        <mybatis-spring.version>1.2.0</mybatis-spring.version>
        <mysql.version>5.1.34</mysql.version>
        <jackson.version>2.4.4</jackson.version>
        <joda.version>2.3</joda.version>
        <freemarker.version>2.3.20</freemarker.version>
        <junit.version>4.11</junit.version>
        <sl4j.version>1.7.7</sl4j.version>
        <log4j.version>1.2.16</log4j.version>
        <aspectj.version>1.7.4</aspectj.version>
        <quartz.version>2.2.1</quartz.version>
        <httpclient.version>4.5</httpclient.version>
        <asm.version>3.3.1</asm.version>
        <cglib.version>3.1</cglib.version>
```
pom.xml（web）
```
<dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>servlet-api</artifactId>
        </dependency>
        <dependency>
            <groupId>javax.servlet.jsp</groupId>
            <artifactId>jsp-api</artifactId>
        </dependency>
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>jstl</artifactId>
            <version>1.2</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
        </dependency>
        <dependency>
            <groupId>org.freemarker</groupId>
            <artifactId>freemarker</artifactId>
        </dependency>
```
pom.xml（service）
```
<dependency>
            <groupId>dom4j</groupId>
            <artifactId>dom4j</artifactId>
            <version>1.6.1</version>
        </dependency>
        <dependency>
            <groupId>jaxen</groupId>
            <artifactId>jaxen</artifactId>
            <version>1.1.6</version>
        </dependency>
        <dependency>
            <groupId>commons-fileupload</groupId>
            <artifactId>commons-fileupload</artifactId>
            <version>1.3.1</version>
        </dependency>
        <dependency>
            <groupId>commons-io</groupId>
            <artifactId>commons-io</artifactId>
            <version>2.4</version>
        </dependency>
        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-lang3</artifactId>
            <version>3.3.2</version>
        </dependency>
        <dependency>
            <groupId>commons-logging</groupId>
            <artifactId>commons-logging</artifactId>
            <version>1.1.3</version>
        </dependency>
        <dependency>
            <groupId>commons-collections</groupId>
            <artifactId>commons-collections</artifactId>
            <version>3.2.1</version>
        </dependency>
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpclient</artifactId>
        </dependency>
        <dependency>
            <groupId>org.quartz-scheduler</groupId>
            <artifactId>quartz</artifactId>
        </dependency>
        <dependency>
            <groupId>org.aspectj</groupId>
            <artifactId>aspectjrt</artifactId>
        </dependency>
        <dependency>
            <groupId>org.aspectj</groupId>
            <artifactId>aspectjweaver</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context-support</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-tx</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-aop</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-oxm</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-orm</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jdbc</artifactId>
        </dependency>
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
        </dependency>
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis-spring</artifactId>
        </dependency>
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
        </dependency>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-core</artifactId>
        </dependency>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
        </dependency>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-annotations</artifactId>
        </dependency>
        <!--<dependency>-->
            <!--<groupId>asm</groupId>-->
            <!--<artifactId>asm</artifactId>-->
        <!--</dependency>-->
        <dependency>
            <groupId>cglib</groupId>
            <artifactId>cglib</artifactId>
        </dependency>
        <dependency>
            <groupId>log4j</groupId>
            <artifactId>log4j</artifactId>
        </dependency>
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-api</artifactId>
        </dependency>
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>jcl-over-slf4j</artifactId>
        </dependency>
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-log4j12</artifactId>
        </dependency>
```
```
<build>
        <resources>
            <resource>
                <directory>src/main/java</directory>
                <includes>
                    <include>**/*.xml</include>
                </includes>
            </resource>
        </resources>
    </build>
```
web.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://java.sun.com/xml/ns/javaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://java.sun.com/xml/ns/javaee
		  http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
         version="2.5">
    <display-name>Z</display-name>
    <description>x</description>
    <!-- Spring 刷新Introspector防止内存泄露 -->
    <listener>
        <listener-class>org.springframework.web.util.IntrospectorCleanupListener</listener-class>
    </listener>
    <listener>
        <listener-class>org.springframework.web.context.request.RequestContextListener</listener-class>
    </listener>
    <!-- Spring配置文件开始  -->
    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>
            classpath*:config/spring/spring-config.xml
        </param-value>
    </context-param>
    <context-param>
        <param-name>log4jConfigLocation</param-name>
        <param-value>classpath:log/log4j.properties</param-value>
    </context-param>
    <context-param>
        <param-name>log4jRefreshInterval</param-name>
        <param-value>60000</param-value>
    </context-param>
    <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>
    <listener>
        <listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
    </listener>
    <!-- Spring配置文件结束 -->
    <!-- error-->
    <error-page>
        <error-code>404</error-code>
        <location>notFound.html</location>
    </error-page>
    <!-- spring mvc-->
    <servlet>
        <servlet-name>spring</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>classpath:config/spring/spring-servlet.xml</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
    </servlet>
    <servlet-mapping>
        <servlet-name>spring</servlet-name>
        <url-pattern>/</url-pattern>
    </servlet-mapping>
    <!-- spring mvc-->
    <!-- encoding-->
    <filter>
        <filter-name>characterEncodingFilter</filter-name>
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
        <filter-name>characterEncodingFilter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
    <!-- login -->
    <filter>
        <filter-name>DelegatingFilterProxy</filter-name>
        <filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
        <init-param>
            <param-name>ignoreUrls</param-name>
            <param-value>/js/**,/app/**,/images/**,/icons/**,/lib/**,/resource/**,/secure/changepassword</param-value>
        </init-param>
        <init-param>
            <param-name>targetBeanName</param-name>
            <param-value>loginFilter</param-value>
        </init-param>
        <init-param>
            <param-name>targetFilterLifecycle</param-name>
            <param-value>true</param-value>
        </init-param>
    </filter>
    <filter-mapping>
        <filter-name>DelegatingFilterProxy</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
</web-app>
```
spring-servlet.xml
```
<?xml version="1.0" encoding="UTF-8" ?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xsi:schemaLocation="http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc-3.2.xsd
		http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.2.xsd
		http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.2.xsd">
    <mvc:annotation-driven content-negotiation-manager="cnManager">
        <mvc:message-converters register-defaults="false">
            <bean class="org.springframework.http.converter.StringHttpMessageConverter"/>
            <bean class="org.springframework.http.converter.ByteArrayHttpMessageConverter"/>
            <bean class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
                <!--<property name="supportedMediaTypes">-->
                    <!--<list>-->
                        <!--<value>text/html;charset=UTF-8</value>-->
                        <!--<value>application/json;charset=UTF-8</value>-->
                        <!--<value>application/javascript;charset=UTF-8</value>-->
                        <!--<value>text/plain;charset=UTF-8</value>-->
                    <!--</list>-->
                <!--</property>-->
                <property name="prettyPrint" value="true"></property>
                <property name="objectMapper">
                    <bean class="com.fasterxml.jackson.databind.ObjectMapper">
                        <property name="dateFormat">
                            <bean class="java.text.SimpleDateFormat">
                                <constructor-arg type="java.lang.String" value="yyyy-MM-dd HH:mm:ss" />
                            </bean>
                        </property>
                    </bean>
                </property>
            </bean>
            <bean class="org.springframework.http.converter.xml.MarshallingHttpMessageConverter">
                <!--<property name="marshaller" ref="jaxb2Marshaller"></property>-->
                <constructor-arg>
                    <bean class="org.springframework.oxm.jaxb.Jaxb2Marshaller">
                        <property name="classesToBeBound">
                            <list>
                                <value>package.Test</value>
                            </list>
                        </property>
                    </bean>
                </constructor-arg>
            </bean>
        </mvc:message-converters>
    </mvc:annotation-driven>
    <!-- 当在web.xml 中   DispatcherServlet使用 <url-pattern>/</url-pattern> 映射时，能映射静态资源 -->
    <mvc:default-servlet-handler/>
    <context:component-scan base-package="package.test.**.controller">
        <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller"/>
        <context:include-filter type="annotation" expression="org.springframework.web.bind.annotation.ControllerAdvice"/>
    </context:component-scan>
    <!-- 静态资源映射 -->
    <mvc:resources mapping="/image/**" location="/WEB-INF/image/"/>
    <mvc:resources mapping="/css/**" location="/WEB-INF/css/"/>
    <mvc:resources mapping="/js/**" location="/WEB-INF/js/"/>
    <!-- 支持上传文件 -->
    <bean id="multipartResolver" class="org.springframework.web.multipart.commons.CommonsMultipartResolver"/>
    <bean id="cnManager" class="org.springframework.web.accept.ContentNegotiationManagerFactoryBean">
        <!--<property name="ignoreAcceptHeader" value="true"/>-->
        <property name="favorPathExtension" value="true"/>
        <!--<property name="defaultContentType" value="text/html"/>-->
        <property name="favorParameter" value="true"/>
        <property name="mediaTypes">
            <map>
                <entry key="xml" value="application/xml"/>
                <entry key="json" value="application/json"/>
                <entry key="xls" value="application/vnd.ms-excel"/>
            </map>
        </property>
    </bean>
    <!-- freemarker config -->
    <bean id="freemarkerConfig" class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">
        <property name="templateLoaderPath" value="/WEB-INF/view/ftl/"/>
        <property name="defaultEncoding" value="UTF-8"/>
        <property name="freemarkerVariables">
            <map>
                <entry key="xml_escape" value-ref="fmXmlEscape"/>
            </map>
        </property>
        <property name="freemarkerSettings">
            <props>
                <!--<prop key="template_update_delay">3600</prop>-->
                <prop key="tag_syntax">auto_detect</prop>
                <!-- 设置标签类型 两种：[] 和 <> 。[] 这种标记解析要快些 -->
                <prop key="default_encoding">UTF-8</prop>
                <prop key="output_encoding">UTF-8</prop>
                <prop key="locale">zh_CN</prop>
                <prop key="date_format">yyyy-MM-dd</prop>
                <prop key="time_format">HH:mm:ss</prop>
                <prop key="auto_import">/spring.ftl as spring</prop>
                <prop key="datetime_format">yyyy-MM-dd HH:mm:ss</prop>
                <prop key="number_format">#</prop>
                <!-- 设置数字格式 以免出现 000.00 -->
                <prop key="classic_compatible">true</prop>
                <!-- 可以满足一般需要。默认情况变量为null则替换为空字符串，如果需要自定义，写上${empty!"EmptyValue of fbysss"}的形式即可  -->
                <prop key="template_exception_handler">html_debug</prop>
                <!-- ignore,debug,html_debug,rethrow -->
                <!-- 修改时间校验 delay;0为不校验每次都重新载入；其余为指定间隔校验ftl模板-->
                <prop key="template_update_delay">0</prop>
            </props>
        </property>
    </bean>
    <bean id="fmXmlEscape" class="freemarker.template.utility.XmlEscape"/>
    <!--Basic application beans. -->
    <bean id="viewResolver"
          class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
        <property name="contentNegotiationManager" ref="cnManager"/>
        <property name="viewResolvers">
            <list>
                <bean class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
                    <property name="viewClass" value="org.springframework.web.servlet.view.freemarker.FreeMarkerView"/>
                    <property name="contentType" value="text/html; charset=utf-8"/>
                    <property name="requestContextAttribute" value="request"/>
                    <property name="suffix" value=".ftl"/>
                    <property name="cache" value="false"/> <!-- 是否缓存ftl视图 -->
                    <property name="order" value="0"/>
                </bean>
                <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
                    <property name="viewClass" value="org.springframework.web.servlet.view.JstlView" />
                    <property name="prefix" value="/WEB-INF/view/jsp/"></property>
                    <property name="suffix" value=".jsp"></property>
                    <property name="order" value="1"/>
                </bean>
            </list>
        </property>
        <property name="defaultViews">
            <list>
                <bean class="org.springframework.web.servlet.view.json.MappingJackson2JsonView">
                    <!-- <property name="extractValueFromSingleKeyModel" value="true"/> -->
                    <property name="prettyPrint" value="true"/>
                    <property name="contentType" value="text/plain"/>
                    <property name="jsonpParameterNames">
                        <set>
                            <value>jsonp</value>
                            <value>callback</value>
                        </set>
                    </property>
                </bean>
                <bean class="org.springframework.web.servlet.view.xml.MarshallingView">
                    <constructor-arg ref="jaxb2Marshaller">
                    </constructor-arg>
                </bean>
                <!--<bean class="com.wonders.stpt.match.controller.ExcelView"/>-->
            </list>
        </property>
    </bean>
    <bean id="jaxb2Marshaller" class="org.springframework.oxm.jaxb.Jaxb2Marshaller">
        <property name="packagesToScan">
            <list>
                <value>package.xxx.**.domain</value>
            </list>
        </property>
        <!--<property name="classesToBeBound">-->
        <!--<list>-->
        <!--<value>com.wonders.stpt.match.domain.MatchTheme</value>-->
        <!--</list>-->
        <!--</property>-->
    </bean>
    <!-- 异常处理 -->
   <bean id="exceptionResolver"
         class="package.xxx.exception.CustomSimpleMappingExceptionResolver">
       <!-- 定义默认的异常处理页面，当该异常类型的注册时使用  -->
        <property name="defaultErrorView" value="error/error-page"></property>
        <!-- 定义异常处理页面用来获取异常信息的变量名，默认名为exception -->
        <property name="exceptionAttribute" value="ex"></property>
        <!-- 定义需要特殊处理的异常，用类名或完全路径名作为key，异常也页名作为值  -->
        <property name="exceptionMappings">
            <props>
                <prop key="org.springframework.security.access.AccessDeniedException">error/error-page</prop>
                <prop key="package.xxx.exception.SystemException">error/error-page</prop>
                <prop key="package.xxx.exception.BusinessException">error/error-page</prop>
                <prop key="java.lang.exception">error/error-page</prop>
            </props>
        </property>
        <!-- 相关状态码对应的错误页面 -->
        <property name="statusCodes">
            <props>
                <prop key="error/error-page">500</prop>
                <!--<prop key="error/error-page">404</prop>-->
            </props>
        </property>
        <!-- 设置日志输出级别，不定义则默认不输出警告等错误日志信息 -->
        <property name="warnLogCategory" value="以下为警告信息"/>
        <!-- 默认HTTP状态码 -->
        <property name="defaultStatusCode" value="500"/>
    </bean>
</beans>
```
配置文件等目录结构：
![](https://img-blog.csdn.net/20170120163325680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Controller demo：
```java
@RestController
public class TestController {
//    @InitBinder
//    public void initBinder(WebDataBinder binder) {
//        DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
//        dateFormat.setLenient(true);
//        SimpleDateFormat timestampFormat = new SimpleDateFormat("yyyy-MM-dd  HH:mm:ss");
//        timestampFormat.setLenient(true);
//        binder.registerCustomEditor(Timestamp.class, new CustomTimestampEditor(timestampFormat, true));
//        binder.registerCustomEditor(Date.class, new CustomDateEditor(dateFormat, true));
//    }
    @ResponseBody
    @RequestMapping(method = RequestMethod.GET, value = "status")
    public Test statusTest(Test t) throws Exception {
        //System.out.println(t);
        //int a= 0/0;
        throw new SystemException();
        //return new Test();
    }
    @ResponseBody
     @RequestMapping(method = RequestMethod.GET, value = "",produces = {"application/json"})
     public Test index(Test t) throws Exception {
        //System.out.println(t);
        System.out.println(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(t.getA()));
        return new Test();
    }
    @ResponseBody
    @RequestMapping(method = RequestMethod.POST, value = "/test2")
    public Test index2(@RequestBody Test t) throws Exception {
        //System.out.println(t);
        System.out.println(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(t.getA()));
        return new Test();
    }
    @RequestMapping(value = "/jsonTest")
    public Test jsonTest(@RequestBody TestDTO testDTO) throws Exception {
        System.out.println(testDTO);
        return new Test();
    }
    @XmlRootElement(name = "zs")
    @XmlAccessorType(XmlAccessType.FIELD)
    public static class Test{
        private Date a = new Date();
        private int age;
        public int getAge() {
            return age;
        }
        public void setAge(int age) {
            this.age = age;
        }
        // @DateTimeFormat(pattern="yyyy-MM-dd HH:mm:ss")
       // @JsonFormat(pattern="yyyy-MM-dd HH:mm:ss",timezone = "GMT+8")
        public Date getA() {
            return a;
        }
        public void setA(Date a) {
            this.a = a;
        }
    }
}
```
全局异常处理：普通请求及ajax；
view ：error/error-page 默认匹配freemarker viewResolver 跳转至 error-page.ftl
String viewName = determineViewName(ex, request);自动根据CustomSimpleMappingExceptionResolver创建异常视图view
```java
public class CustomSimpleMappingExceptionResolver extends SimpleMappingExceptionResolver {
	@Override
	protected ModelAndView doResolveException(HttpServletRequest request,
			HttpServletResponse response, Object handler, Exception ex) {
		// Expose ModelAndView for chosen error view.
		**String viewName = determineViewName(ex, request);**
		if (viewName != null) {// JSP格式返回
            //设置返回类型
			if (!(request.getHeader("accept").indexOf("application/json") > -1 || (request
					.getHeader("X-Requested-With")!= null && request
					.getHeader("X-Requested-With").indexOf("XMLHttpRequest") > -1))) {
				// 如果不是异步请求
				// Apply HTTP status code for error views, if specified.
				// Only apply it if we're processing a top-level request.
				Integer statusCode = determineStatusCode(request, viewName);
				if (statusCode != null) {
                    request.setAttribute("error_code", statusCode);
                    request.setAttribute("stay_time", HttpResponseErrorHandler.getStayTime(statusCode));
                    request.setAttribute("say_word", HttpResponseErrorHandler.getSayWord(statusCode));
                    request.setAttribute("show_words", HttpResponseErrorHandler.getShowWords(statusCode));
                    request.setAttribute("is_tracker_required", HttpResponseErrorHandler.isPageTrackerRequired(request, statusCode));
                    request.setAttribute("root_domain", HttpResponseErrorHandler.getDomain());
                    StringWriter sw = new StringWriter();
                    ex.printStackTrace(new PrintWriter(sw, true));
                    request.setAttribute("exception",sw.toString());
					applyStatusCodeIfPossible(request, response, statusCode);
				}
				return getModelAndView(viewName, ex, request);
			} else {// JSON格式返回
				try {
					PrintWriter writer = response.getWriter();
					writer.write(ex.getMessage());
					writer.flush();
				} catch (IOException e) {
					e.printStackTrace();
				}
				return null;
			}
		} else {
			return null;
		}
	}
}
```
erro-page.ftl：
```
<!Doctype html>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<title>${error_code} | demo</title>
	</head>
	<body>
        <div>${error_code}</div>
        <div>${ex}</div>
        <div>${exception}</div>
	</body>
</html>
```
1、访问
http://localhost:8080
返回：
{
  "a" : "2017-01-20 16:57:22",
  "age" : 0
}
2、访问：
http://localhost:8080/status
返回：
![](https://img-blog.csdn.net/20170120165543590?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
