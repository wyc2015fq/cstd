# SpringMVC之自定义类型转换WebBindingInitializer@initBinder - 零度的博客专栏 - CSDN博客
2017年01月06日 10:22:10[零度anngle](https://me.csdn.net/zmx729618)阅读数：4835
 在使用SpringMVC的时候，经常会遇到表单中的日期字符串和JavaBean的Date类型的转换，而SpringMVC默认不支持这个格式的转换，所以需要手动配置，自定义数据的绑定才能解决这个问题。类型转换器常用于转换double、float、date等类型，SpringMVC在支持自身的转换器框架的同时也支持JavaBean的PropertyEditor，可以通过在控制器中使用@@initBinder 添加自定义的编辑器，也可以通过WebBindingInitializer装配全局的类型编辑器。WebDataBinder是用来绑定请求参数到指定的属性编辑器.由于前台传到controller里的值是String类型的，当往Model里Set这个值的时候，如果set的这个属性是个对象，Spring就会去找到对应的editor进行转换，然后再SET进去。下面介绍如何使用自定义的类型编辑器
        功能说明：
       当表单提交double、date等类型时，我们需要将其转换为java可识别的date，double等，如在浏览器中输入：
[http://localhost:8080/SpringMVCTest/test/conversionTest.action?person=zhangsan:666:ssss:3.1415](http://localhost:8080/SpringMVCTest/test/conversionTest.action?person=zhangsan:666:ssss:3.1415) 时，需要将其转化到Person对象中，此时需要对double类型进行转换。
       1、 Person实体类：
```
package zwc.framwork;
public class Person {
	private String username;	
	private String passwd;
	private String realName;
	private Double price;
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	public String getPasswd() {
		return passwd;
	}
	public void setPasswd(String passwd) {
		this.passwd = passwd;
	}
	public String getRealName() {
		return realName;
	}
	public void setRealName(String realName) {
		this.realName = realName;
	}
	public Double getPrice() {
		return price;
	}
	public void setPrice(Double price) {
		this.price = price;
	}		
}
```
        2、自定义转换器
```
package com.ll.model;
import java.beans.PropertyEditorSupport;
public class PersonEditor extends PropertyEditorSupport {
	@Override
	public void setAsText(String text) throws IllegalArgumentException {
		Person p = new Person();
		if (text != null) {
			String[] items = text.split(":");
			p.setUsername(items[0]+"by propertyeEditor");
			p.setPasswd(items[1]);
			p.setRealName(items[2]);
			p.setPrice(Double.parseDouble(items[3]));
		}
		setValue(p);
	}
	
	@Override
	public String getAsText() {
		return getValue().toString();
	}   
}
```
3、注册自定义的编辑器
```
/**
	 * 控制器初始化时调用
	 * SpringMVC 使用WebDataBinder处理<请求消息,方法入参>的绑定工作
	 * @param binder
	 */
	@initBinder 
        public void initBinder(WebDataBinder binder){
		
		binder.registerCustomEditor(Person.class, new PersonEditor()); 
		//binder.setValidator(this.validator);  //
	}
```
4、类型装换测试
```
/**
	 * 类型转换测试
	 * @param person
	 * @param modelMap
	 * @return
	 */
	@RequestMapping("/test/conversionTest")
	public String conversionTest(@RequestParam("person")Person person,ModelMap modelMap){
		
		System.out.println(person.getUsername()+" "+person.getPasswd()+" "+person.getRealName()+" "+ person.getPrice());
		modelMap.put("person", person);
		return "/test/test";
	}
```
5、运行测试
在浏览器中输入：
![技术分享](http://images0.cnblogs.com/blog/731047/201506/251054583142293.png)
类型转换器会自动将"person=zhangsan:666:ssss:3.1415"转换为Person对象；
![技术分享](http://images0.cnblogs.com/blog/731047/201506/251054589396934.png)
        上述给出了通过@initBinder注册自定义控制器，通常情况下我们也可以通过实现WebBindingInitializer实现一个全局的自定义类型转换器，如下：CustomBindInitializer.java
```
package org.nercita.core.web.springmvc;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.text.SimpleDateFormat;
import java.util.Date;
import org.springframework.beans.PropertyEditorRegistrar;
import org.springframework.beans.propertyeditors.CustomDateEditor;
import org.springframework.beans.propertyeditors.CustomNumberEditor;
import org.springframework.beans.propertyeditors.StringTrimmerEditor;
import org.springframework.core.convert.ConversionService;
import org.springframework.validation.BindingErrorProcessor;
import org.springframework.validation.MessageCodesResolver;
import org.springframework.validation.Validator;
import org.springframework.web.bind.WebDataBinder;
import org.springframework.web.bind.support.WebBindingInitializer;
import org.springframework.web.context.request.WebRequest;
/**
 * Created by zhangwc.
 * 全局类型转换器
 * Date:2016-12-12.
 */
public class CustomBindInitializer implements WebBindingInitializer {
    private String format = "yyyy-MM-dd";
    private boolean autoGrowNestedPaths = true;
    private boolean directFieldAccess = false;
    private MessageCodesResolver messageCodesResolver;
    private BindingErrorProcessor bindingErrorProcessor;
    private Validator validator;
    private ConversionService conversionService;
    private PropertyEditorRegistrar[] propertyEditorRegistrars;
    public void setAutoGrowNestedPaths(boolean autoGrowNestedPaths) {
        this.autoGrowNestedPaths = autoGrowNestedPaths;
    }
    public boolean isAutoGrowNestedPaths() {
        return this.autoGrowNestedPaths;
    }
    public final void setDirectFieldAccess(boolean directFieldAccess) {
        this.directFieldAccess = directFieldAccess;
    }
    public boolean isDirectFieldAccess() {
        return directFieldAccess;
    }
    public final void setMessageCodesResolver(MessageCodesResolver messageCodesResolver) {
        this.messageCodesResolver = messageCodesResolver;
    }
    public final MessageCodesResolver getMessageCodesResolver() {
        return this.messageCodesResolver;
    }
    public final void setBindingErrorProcessor(BindingErrorProcessor bindingErrorProcessor) {
        this.bindingErrorProcessor = bindingErrorProcessor;
    }
    public final BindingErrorProcessor getBindingErrorProcessor() {
        return this.bindingErrorProcessor;
    }
    public final void setValidator(Validator validator) {
        this.validator = validator;
    }
    public final Validator getValidator() {
        return this.validator;
    }
    public final void setConversionService(ConversionService conversionService) {
        this.conversionService = conversionService;
    }
    public final ConversionService getConversionService() {
        return this.conversionService;
    }
    public final void setPropertyEditorRegistrar(PropertyEditorRegistrar propertyEditorRegistrar) {
        this.propertyEditorRegistrars = new PropertyEditorRegistrar[]{propertyEditorRegistrar};
    }
    public final void setPropertyEditorRegistrars(PropertyEditorRegistrar[] propertyEditorRegistrars) {
        this.propertyEditorRegistrars = propertyEditorRegistrars;
    }
    public final PropertyEditorRegistrar[] getPropertyEditorRegistrars() {
        return this.propertyEditorRegistrars;
    }
    public void initBinder(WebDataBinder binder, WebRequest request) {
        binder.setAutoGrowNestedPaths(this.autoGrowNestedPaths);
        SimpleDateFormat sf = new SimpleDateFormat(format);
        sf.setLenient(false);
        binder.registerCustomEditor(Date.class, new CustomDateEditor(sf, true));
        binder.registerCustomEditor(String.class, new StringTrimmerEditor(false));
        binder.registerCustomEditor(Short.class, new CustomNumberEditor(Short.class, true));
        binder.registerCustomEditor(Integer.class, new CustomNumberEditor(Integer.class, true));
        binder.registerCustomEditor(Long.class, new CustomNumberEditor(Long.class, true));
        binder.registerCustomEditor(Float.class, new CustomNumberEditor(Float.class, true));
        binder.registerCustomEditor(Double.class, new CustomNumberEditor(Double.class, true));
        binder.registerCustomEditor(BigDecimal.class, new CustomNumberEditor(BigDecimal.class, true));
        binder.registerCustomEditor(BigInteger.class, new CustomNumberEditor(BigInteger.class, true));
        if (this.directFieldAccess) {
            binder.initDirectFieldAccess();
        }
        if (this.messageCodesResolver != null) {
            binder.setMessageCodesResolver(this.messageCodesResolver);
        }
        if (this.bindingErrorProcessor != null) {
            binder.setBindingErrorProcessor(this.bindingErrorProcessor);
        }
        if ((this.validator != null) && (binder.getTarget() != null) &&
                (this.validator.supports(binder.getTarget().getClass()))) {
            binder.setValidator(this.validator);
        }
        if (this.conversionService != null) {
            binder.setConversionService(this.conversionService);
        }
        if (this.propertyEditorRegistrars != null)
            for (PropertyEditorRegistrar propertyEditorRegistrar : this.propertyEditorRegistrars)
                propertyEditorRegistrar.registerCustomEditors(binder);
    }
    public void setFormat(String format) {
        this.format = format;
    }
}
```
在SpringMVC配置文件中注入：
```
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:mvc="http://www.springframework.org/schema/mvc" xmlns:context="http://www.springframework.org/schema/context"
	xmlns:p="http://www.springframework.org/schema/p" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.0.xsd     
	http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.0.xsd 
	http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd">
    
    <!-- 静态资源 不拦截 -->
	<mvc:resources location="/common/"   mapping="/common/**"/>
	<mvc:resources location="/images/"   mapping="/images/**" />
	<mvc:resources location="/js/"       mapping="/js/**" /> 
	<mvc:resources location="/css/"      mapping="/css/**" />
	<mvc:resources location="/styles/"   mapping="/styles/**" />
	<mvc:resources location="/htm/"      mapping="/htm/**" />
	<mvc:resources location="/upload/"   mapping="/upload/**" />
    
    <import resource="view-controller.xml" />
	
	<context:annotation-config/>
	
	<context:component-scan base-package="org.nercita.ntp" use-default-filters="false">
		<context:include-filter expression="org.springframework.stereotype.Controller" type="annotation" />
	</context:component-scan>
		
	<bean id="handlerMapping"  class="org.springframework.web.servlet.mvc.annotation.DefaultAnnotationHandlerMapping"/>  
	<bean id="handlerAdapter"  class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter">	
		<property name="messageConverters">
			<list>
				<bean class="org.nercita.core.web.springmvc.StringHttpMessageConverter" />
				<ref bean="msgConverter"/>
			</list>
		</property>
		<property name="webBindingInitializer">
			<bean class="org.nercita.core.web.springmvc.CustomBindInitializer">		   
 
				<property name="validator" ref="validator" />
				<property name="conversionService" ref="conversionService" /> 
 
			</bean> 
		</property>
	</bean>	
	
 	<bean id="msgConverter" class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
        <property name="supportedMediaTypes">
            <list>
                <value>text/html;charset=UTF-8</value>
                <value>text/json;charset=UTF-8</value>
                <value>application/json;charset=UTF-8</value>
            </list>
        </property>
    </bean>  
    
   
    <bean id="validator" class="org.springframework.validation.beanvalidation.LocalValidatorFactoryBean"/>
    <bean id="conversionService" class="org.springframework.format.support.FormattingConversionServiceFactoryBean"/> 
 
    <!-- 根据客户端的不同的请求决定不同的view进行响应, 如 /blog/1.json /blog/1.xml 	-->
    <bean class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
        <property name="ignoreAcceptHeader" value="true"/>
        <property name="defaultContentType" value="text/html"/>
        <property name="mediaTypes">
            <map>
                <entry key="json" value="application/json"/>
                <entry key="xml" value="application/xml"/>
            </map>
        </property>
        <property name="favorParameter" value="false"/>
        <property name="viewResolvers">
            <list>
                <bean class="org.springframework.web.servlet.view.BeanNameViewResolver"/>
                <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
                    <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>
                    <property name="prefix" value="/WEB-INF/views/"/>
                    <property name="suffix" value=".jsp"/>
                </bean>
            </list>
        </property>
        <property name="defaultViews">
            <list>
                <bean class="org.springframework.web.servlet.view.json.MappingJackson2JsonView"/>
                <bean class="org.springframework.web.servlet.view.xml.MarshallingView">
                    <property name="marshaller">
                        <bean class="org.springframework.oxm.xstream.XStreamMarshaller"/>
                    </property>
                </bean>
            </list>
        </property>
    </bean> 
    
    <bean class="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">
		<property name="defaultErrorView" value="error" />
		<property name="exceptionMappings">
			<props>
				<prop key=".DataAccessException">dataAccessFailure</prop>
				<prop key=".NoSuchRequestHandlingMethodException">resourceNotFound</prop>
				<prop key=".TypeMismatchException">resourceNotFound</prop>
				<prop key=".lang.Exception">uncaughtException</prop>
			</props>
		</property>
	</bean>   
    <!-- 文件上传-->
    <bean id="multipartResolver" class="org.springframework.web.multipart.commons.CommonsMultipartResolver">
        <property name="maxUploadSize" value="104857600"/>
        <property name="maxInMemorySize" value="4096"/>
    </bean>
	
	<!-- 处理国际化资源文件 -->  
    <bean id="messageSource" class="org.springframework.context.support.ResourceBundleMessageSource">    
       <property name="basename" value="message/message-info" />              
       <property name="useCodeAsDefaultMessage" value="true" />       
    </bean>
</beans>
```

