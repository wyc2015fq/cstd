# Spring mvc+freeMarker 使用jsp自定义标签 - z69183787的专栏 - CSDN博客
2016年08月09日 16:15:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4091
web.xml：
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app
        version="3.0"
        xmlns="http://java.sun.com/xml/ns/javaee"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
    <display-name>xxxx</display-name>
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
            classpath*:config/spring/local/emidas-wallet-*.xml,
            classpath*:config/spring/local/appcontext-core.xml
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
    <!-- spring mvc-->
    <servlet>
        <servlet-name>spring</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>classpath:config/spring/local/appcontext-servlet.xml</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
    </servlet>
    <servlet-mapping>
        <servlet-name>spring</servlet-name>
        <url-pattern>/wallet/*</url-pattern>
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
            <bean class="org.springframework.http.converter.ByteArrayHttpMessageConverter"/>
            <bean class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
                <property name="supportedMediaTypes">
                    <list>
                        <value>text/html</value>
                        <value>application/json</value>
                        <value>application/javascript</value>
                        <value>text/plain</value>
                    </list>
                </property>
                <property name="prettyPrint" value="true"></property>
            </bean>
            <!--<bean class="org.springframework.http.converter.xml.MarshallingHttpMessageConverter">-->
                <!--<property name="marshaller" ref="jaxb2Marshaller"></property>-->
            <!--</bean>-->
        </mvc:message-converters>
    </mvc:annotation-driven>
    <!-- 当在web.xml 中   DispatcherServlet使用 <url-pattern>/</url-pattern> 映射时，能映射静态资源 -->
    <mvc:default-servlet-handler/>
    <context:component-scan base-package="com.xxxx.web.controller">
        <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller"/>
        <context:include-filter type="annotation" expression="org.springframework.web.bind.annotation.ControllerAdvice"/>
    </context:component-scan>
    <!-- 静态资源映射 -->
    <!--<mvc:resources mapping="/image/**" location="/WEB-INF/image/"/>-->
    <!--<mvc:resources mapping="/css/**" location="/WEB-INF/css/"/>-->
    <!--<mvc:resources mapping="/js/**" location="/WEB-INF/js/"/>-->
    <!-- 支持上传文件 -->
    <bean id="multipartResolver" class="org.springframework.web.multipart.commons.CommonsMultipartResolver"/>
    <bean id="cnManager" class="org.springframework.web.accept.ContentNegotiationManagerFactoryBean">
        <property name="ignoreAcceptHeader" value="true"/>
        <property name="favorPathExtension" value="true"/>
        <property name="defaultContentType" value="text/html"/>
        <property name="favorParameter" value="true"/>
        <property name="mediaTypes">
            <map>
                <entry key="xml" value="application/xml"/>
                <entry key="json" value="text/plain"/>
                <entry key="xls" value="application/vnd.ms-excel"/>
            </map>
        </property>
    </bean>
    <!-- freemarker config -->
    <bean id="freemarkerConfig" class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">
        <property name="templateLoaderPath" value="/WEB-INF/view/mobile/"/>
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
                    <property name="cache" value="true"/>
                    <property name="order" value="0"/>
                </bean>
                <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
                    <property name="viewClass" value="org.springframework.web.servlet.view.JstlView" />
                    <property name="prefix" value="/WEB-INF/view/mobile/"></property>
                    <property name="suffix" value=".ftl"></property>
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
                </bean>
                <!--<bean class="com.wonders.stpt.match.controller.ExcelView"/>-->
            </list>
        </property>
    </bean>
</beans>
```
freemarker.properties：
```
number_format=#
classic_compatible=true
####add by danson.liu####
default_encoding=UTF-8
template_update_delay=0
#########################
template_exception_handler=rethrow
```
tag.java：
```java
package com.xxxx.web.tag;
import org.apache.commons.lang3.StringUtils;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.BodyTagSupport;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
public class Tag extends BodyTagSupport {
    private static final long serialVersionUID = 1875715349277889045L;
    
    private String href;
    @Override
    public int doStartTag() throws JspException {
        pageContext.getOut().write("test"+href);
        return SKIP_BODY;
    }
   public void setHref(String href) {
        this.href = href;
    }
}
```
test.tld：
```
<?xml version="1.0" encoding="UTF-8"?>
<taglib version="2.0" xmlns="http://java.sun.com/xml/ns/j2ee"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee http://java.sun.com/xml/ns/j2ee/web-jsptaglibrary_2_0.xsd">
	<display-name>"Tags"</display-name>
	<tlib-version>1.0</tlib-version>
	<short-name>pro</short-name>
	<uri>/Test</uri>
	<tag>
		<name>urlScheme</name>
		<tag-class>com.xxxx.Tag</tag-class>
		<body-content>empty</body-content>
		<attribute>
			<name>href</name>
			<required>true</required>
			<rtexprvalue>false</rtexprvalue>
		</attribute>
        <attribute>
            <name>xxxxx</name>
            <required>true</required>
            <rtexprvalue>false</rtexprvalue>
        </attribute>
	</tag>
</taglib>
```
ftl：
```
<#assign testMob=JspTaglibs["/WEB-INF/tld/test.tld"]>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <meta name="viewport" content="initial-scale=1.0, maximum-scale=1.0, user-scalable=0">
    <meta name="test" content="black" />
    <meta name="format-detection" content="telephone=no" />
    <meta name="format-detection" content="email=no" />
    <title>未登录</title>
</head>
<body>
<a href="<@testMob.urlScheme href="/xxxxxxxx"/>"
   class="all">查看全部
    家分店<span class="icon-arrow"></span></a>
<div class="empty-main">
    <i class="icon"></i>
    <p>您还没有登录哦</p>
</div>
</body>
</html>
```
