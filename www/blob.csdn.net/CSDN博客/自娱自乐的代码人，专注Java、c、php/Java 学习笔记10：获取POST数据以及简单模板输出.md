# Java 学习笔记10：获取POST数据以及简单模板输出 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月27日 16:43:47[initphp](https://me.csdn.net/initphp)阅读数：1487
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- 获取POST数据

这一节会继续Java学习笔记09。


一般在开发的时候，我们需要获取表单中提交的数据，那么我们必须先创建一个say.jsp，这个jsp的内容是一个非常简单的表单，POST方式提交，提交到/hello/show/路径上。
```java
<form action='/hello/show/' method="post">
<input name="username" type="text" />
<input name="password" type="text" />
<input name="" type="submit" />
</form>
```


> 
然后我们需要一个控制器文件，两个Action，一个是现实say.jsp静态页面，一个是接收处理POST提交过来的数据。

其中sya()方法显示静态页面；show()方法处理POST数据。

show方法两个参数，User user这个对象 Spring会自动将POST数据填充到User这个类上面去，Modelmodel主要用来实现Controller和模板之间数据传递。




```java
package com.mvc.rest;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;

//@Controller 是一个标识这个是控制器类的注解标签，如果是控制器类 都需要有这个注解。
@Controller
//@RequestMapping(value="/hello") 会映射到url /hello/则访问HelloController中的Action
@RequestMapping(value="/hello")
public class HelloController {
	
	//@RequestMapping(value="/say") 会映射到url /hello/say则访问HelloController中的Action
	@RequestMapping(value="/say")
	public void say() {
		System.out.print("this is HelloController And say Action \r\n");
	}
	
	@RequestMapping("/show")
	public String show(User user,Model model) {
		System.out.print(user.getUsername());
		System.out.print(user.getPassword());
		model.addAttribute("user", user);
		return "hello/show";
	}
}
```



User类：



```java
package com.mvc.rest;

public class User {
	private String username;
	
	private String password;

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}
	

}
```




> show.jsp模板类，输出接收到的POST数据：


```java
user:${user.username}<br/>
password:${user.password}
```


bean.xml配置：



```
<?xml version="1.0" encoding="UTF-8"?>
<beans  
    xmlns="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:aop="http://www.springframework.org/schema/aop"
    xsi:schemaLocation="http://www.springframework.org/schema/beans 
    http://www.springframework.org/schema/beans/spring-beans-2.0.xsd
    http://www.springframework.org/schema/context
    http://www.springframework.org/schema/context/spring-context-2.5.xsd
    http://www.springframework.org/schema/aop
    http://www.springframework.org/schema/aop/spring-aop-2.5.xsd
    ">  
    <context:annotation-config/>
    <aop:aspectj-autoproxy/> 
     <bean id="User" class="com.mvc.rest.User" ></bean>
</beans>
```






