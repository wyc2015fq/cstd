# SpringMVC之类型转换Converter - z69183787的专栏 - CSDN博客
2018年04月03日 15:50:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：180
个人分类：[SpringMvc-原理分析](https://blog.csdn.net/z69183787/article/category/6456963)
[https://www.cnblogs.com/ssslinppp/p/4598102.html](https://www.cnblogs.com/ssslinppp/p/4598102.html)
## **1. 摘要**
在spring 中定义了3中类型转换接口，分别为：
-   Converter接口              ：使用最简单，最不灵活；
-   ConverterFactory接口  ：使用较复杂，比较灵活；
-   GenericConverter接口 ：使用最复杂，也最灵活；
可参考链接： [http://blog.csdn.net/renhui999/article/details/9837897](http://blog.csdn.net/renhui999/article/details/9837897) 。看看他们的介绍；
本文仅介绍Converter接口的使用。
这里仅仅是一个简单的类型转换示例，重点在于说明SpringMVC类型转换的方法、配置及流程；
基本功能说明：
1. 前台传递的数据格式形如：“zhangSan：888”；
[http://localhost:8080/SpringMVCTest/test/conversionTest.action?person=zhangsan:666](http://localhost:8080/SpringMVCTest/test/conversionTest.action?person=zhangsan:666)
2. 将“zhangSan：888”字符串转换为Person对象；
![](https://images0.cnblogs.com/blog/731047/201506/250907328777244.png)
具体过程：
- 定义转换类，实现Converter<S,T>接口；
-  装配自定义的conversionService;
## 2. 定义类型转换类
StringToPersonConverter.java
![](https://images0.cnblogs.com/blog/731047/201506/250907340802543.png)
- `package com.ll.model;`
- `import org.springframework.core.convert.converter.Converter;`
- ``
- `/**`
- ` * @author ssslinppp`
- ` * Spring MVC数据转换-简单示例`
- ` * 将形如：“zhangSan：888”的字符串转换为Person对象`
- ` *`
- ` */`
- `public class StringToPersonConverter implements Converter<String,Person>{`
- `	public Person convert(String source) {`
- `		Person p1 = new Person();`
- `		if(source != null){`
- `			String[] items = source.split(":");`
- `			p1.setUsername(items[0]);`
- `			p1.setPasswd(items[1]);`
- `		}`
- `		return p1;`
- `	}`
- `}`
## **3. 装配自定义的conversionService**
在Spring的web上下文中配置：
![](https://images0.cnblogs.com/blog/731047/201506/250907350494614.png)
spring-servlet.xml
- `<?xml version="1.0" encoding="UTF-8" ?>`
- `<beans xmlns="http://www.springframework.org/schema/beans"`
- `	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"`
- `	xmlns:context="http://www.springframework.org/schema/context" `
- `	xmlns:mvc="http://www.springframework.org/schema/mvc"`
- `	xsi:schemaLocation="http://www.springframework.org/schema/beans`
- `       http://www.springframework.org/schema/beans/spring-beans-3.0.xsd`
- `       http://www.springframework.org/schema/context `
- `       http://www.springframework.org/schema/context/spring-context-3.0.xsd`
- `       http://www.springframework.org/schema/mvc`
- `       http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd">`
- `       `
- `	<!-- 扫描web包，应用Spring的注解 -->`
- `	<context:component-scan base-package="com.ll.web"/>`
- `	`
- `	<mvc:annotation-driven conversion-service="conversionService"/>`
- ``
- `	<bean id="conversionService"`
- `		class="org.springframework.context.support.ConversionServiceFactoryBean">`
- `		<property name="converters">`
- `			<list>`
- `				<bean class="com.ll.model.StringToPersonConverter" />`
- `			</list>`
- `		</property>`
- `	</bean>`
- `	`
- `	<!-- 配置视图解析器，将ModelAndView及字符串解析为具体的页面，默认优先级最低 -->`
- `	<bean`
- `		class="org.springframework.web.servlet.view.InternalResourceViewResolver"`
- `		p:viewClass="org.springframework.web.servlet.view.JstlView" `
- `		p:prefix="/jsp/"`
- `		p:suffix=".jsp" />`
- ``
- `</beans>`
## **4. 控制器**
![](https://images0.cnblogs.com/blog/731047/201506/250907361424712.png)
当前台发送请求：
[http://localhost:8080/SpringMVCTest/test/conversionTest.action?person=zhangsan:666](http://localhost:8080/SpringMVCTest/test/conversionTest.action?person=zhangsan:666) 
 时；
将person=zhangsan:666传递到后台，被StringToPersonConverter转换为Person对象；
跳转到界面：test.jsp
![](https://images0.cnblogs.com/blog/731047/201506/250907369709825.png)
![](https://images0.cnblogs.com/blog/731047/201506/250907375029197.png)
[来自为知笔记(Wiz)](http://www.wiz.cn/i/d44efcf0)
