# Java 学习笔记06：Spring 基于注解（Annotation）的AOP - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月19日 13:08:03[initphp](https://me.csdn.net/initphp)阅读数：2807
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- 基于注解的AOP

> 
AOP，意为面向切面编程。它可以在不修改源代码的情况下，给一个类中的函数动态添加程序的一种技术实现方式。

例如这样一个需求，我们需要对用户的增删改查操作进行日志的写入。

普通的做法是，我们写一个日志的类，然后再所有增删改查的操作中添加一个日志写入的方法。但是这样做的弊端就是你需要对程序所有的地方都加上增删改查的地方写上这个日志写入操作。如果程序非常庞大，那么我们就改死吧！

Java的AOP面向切面编程，很好的解决了这一个尴尬的问题。我们只需要写一个独立的类，经过一些注解等方式，Spring框架能自动根据注解在程序运行的时候帮助我们添加到上面的增删改查操作中加入日志写入操作。




- 一个简单的基于注解的AOP实现
- 需要额外依赖的jar包
aspectjrt.jar，cglib-nodep-2.1_3.jar，aspectjweaver-1.6.7.jar，aopalliance.jar，这几个依赖包你需要手动下载，然后才能通过注解的方式实现AOP，Spring没有自带这几个第三方的扩展包。


- 配置Bean.xml文件。必须要添加的是： xmlns:aop="http://www.springframework.org/schema/aop"，http://www.springframework.org/schema/aop

    http://www.springframework.org/schema/aop/spring-aop-2.5.xsd，<aop:aspectj-autoproxy/>

<bean id="UserAop" class="com.spring.aop.UserAop" ></bean> 这个是我们的一个AOP的类。



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
    <bean id="User" class="com.spring.user.User" ></bean>
    <bean name="vip" id="vip" class="com.spring.user.Vip" ></bean>
   <bean id="UserAop" class="com.spring.aop.UserAop" ></bean>
</beans>
```


- UserAop类的实现。



```java
package com.spring.aop;

import org.aspectj.lang.annotation.After;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;

//@Aspect 这个注解声明了UserAop类是一个切面的类
@Aspect 
public class UserAop {
	
	//@Beafre 这个标注的意思是在com.spring.user.User类的userInfo()方法之后执行
	//execution(public void com.spring.user.User.userInfo()) 是一个语法规则，表明这个beforeMethod
	//要在哪个类的方法执行之前运行
	@Before("execution(public void com.spring.user.User.userInfo())")
	public void beforeMethod() {
		System.out.print("\r\nAOP:Yes it's Before!\r\n");
	}
	
	//@After 这个标注的意思是在com.spring.user.User类的sayUser()方法之后执行
	//并且args(message) 传递进来message的参数
	@After("execution(public void com.spring.user.User.sayUser()) && args(message)")
	public void afterMethod(String message) {
		System.out.print("Yes it's After" + message);
	}
}
```


```java
package com.spring.user;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;

import org.springframework.beans.factory.annotation.Autowired;

public class User {
	
	private String username;
	
	private int age;
	
	private int phone;
	
	@Autowired
	private Vip vip;

	
	public void setUsername(String username) {
		this.username = username;
	}
	
	public String getUsername() {
		return username;
	}
	
	public void setAge(int age) {
		this.age = age;
	}
	
	public int getAge() {
		return age;
	}
	
	public void setPhone(int phone) {
		this.phone = phone;
	}
	
	public int getPhone() {
		return phone;
	}
	
	//com.spring.user.User类中的userInfo方法
	public void userInfo() {
		System.out.print("用户姓名：" + this.username + "\r\n"
	+ "用户年龄" + this.age + "\r\n" + "用户电话：" + this.phone);
		this.vip.say();
		String message = "\r\nAOP:this is args Message";
		this.sayHello(message);
	}
	
	//com.spring.user.User类中的sayHello方法
	public void sayHello(String message) {
		System.out.print(message);
	}
	
	@PostConstruct
	public void init() {
		System.out.printf("init a class");
	}
	@PreDestroy
	public void destroy() {
		System.out.print("destory a class");
	}
	
}
```


```java
package com.spring.main;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.spring.user.User;

public class Index {
	
	//程序入口文件，执行User类中的userInfo方法
	public static void main(String args[]) {
		ApplicationContext Factory = null;
		Factory = new ClassPathXmlApplicationContext("bean.xml");
		User User = (User)Factory.getBean("User");
		User.setUsername("zhuli");
		User.setAge(100);
		User.setPhone(138581025);
		User.userInfo();
	}
}
```


执行结果：



```
init a class
AOP:Yes it's Before!
用户姓名：zhuli
用户年龄100
用户电话：138581025I am Vip
AOP:this is args Message
```

- AOP注解语法：
- AOP的注解方式的差不多，@Before(execution (xxxxxx))


- 通知类型表：

|类型|说明|
|----|----|
|@Before|在函数执行前执行|
|@AfterReturn|在函数执行完Return后执行|
|@AfterThrow|在函数执行完trow中的代码后执行|
|@After|在函数抛出异常，执行完finally后执行|
|@Around|详单与Before和AfterReturn|


- 常用execution表达式

|实例|说明|
|----|----|
|execution(public * * (..))|表示所有的方法都植入|
|execution(* set * (..))|都植入setter方法中|
|execution(* com.spring.user. * (..))|植入到com.spring.user包中所有类的任意方法|
|execution(* com.spring.user.User. * (..))|植入到com.spring.user.User类中的所有方法|


- 如果需要带上参数：

```java
//使用&&符号连接，args(参数名称)
@After("execution(public void com.spring.user.User.sayUser()) && args(message)")
```






