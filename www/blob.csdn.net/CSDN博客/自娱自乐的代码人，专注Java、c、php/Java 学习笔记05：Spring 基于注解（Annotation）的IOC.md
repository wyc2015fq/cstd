# Java 学习笔记05：Spring 基于注解（Annotation）的IOC - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月16日 15:12:15[initphp](https://me.csdn.net/initphp)阅读数：1590
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- @Resource 使用，修饰setter方法，将目标对象注入到依赖的Bean。说白了，就是你在一个User类中 比如要调用 Vip类，你用了注解，就不需要每次实例化来调用这个Vip类然后再User类中使用，只要标志上注解，Spring框架会自动调用。

- 入口文件，仍然是通过bean.xml文件，调用一个User类，并执行userInfo方法：
```java
package com.spring.main;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.spring.user.User;

public class Index {
	
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


- User类和Vip类，User类中定义了一个private私有的vip对象，通过@Resource注解的方式，直接将Vip实例化对象赋给User类中的vip对象：

```java
//User 类
package com.spring.user;
import javax.annotation.Resource;


public class User {
	
	private String username;
	
	private int age;
	
	private int phone;
	
	private Vip vip;
	
	@Resource
	public void setVip(Vip vip) {
		this.vip = vip;
	}

	
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
	
	
	public void userInfo() {
		System.out.print("用户姓名：" + this.username + "\r\n"
	+ "用户年龄" + this.age + "\r\n" + "用户电话：" + this.phone);
		this.vip.say();
	}
	
	public void init() {
		System.out.printf("init a class");
	}
	
	public void destroy() {
		System.out.print("destory a class");
	}
	
}


//Vip类
package com.spring.user;

public class Vip {
	public void say() {
		System.out.print("I am Vip");
	}
}
```



- bean配置文件，beans的头部必须添加````
 xmlns:context="http://www.springframework.org/schema/context"

http://www.springframework.org/schema/context

http://www.springframework.org/schema/context/spring-context-2.5.xsd

<context:annotation-config/>

如果bean上面没有这些参数，注解@Resource将无法执行。

```
<?xml version="1.0" encoding="UTF-8"?>
<beans  
    xmlns="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
    xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="http://www.springframework.org/schema/beans 
http://www.springframework.org/schema/beans/spring-beans-2.0.xsd
http://www.springframework.org/schema/context
http://www.springframework.org/schema/context/spring-context-2.5.xsd">
      <context:annotation-config/>
    <bean id="User" class="com.spring.user.User" ></bean>
    <bean name="vip" id="vip" class="com.spring.user.Vip" ></bean> 
</beans>
```


- @Autowired 自动装载使用，基本和上面的差不多，Autowired会使用比较多，xml配置如上例。



```java
//直接在变量对象上面使用即可
        @Autowired
	private Vip vip;
```

- @PostConstruct和@PreDestroy，相当于<bean>元素中的init-method和destroy-method。相当于类初始化和类销毁前会执行的方法。



```java
@PostConstruct
	public void init() { //初始化的时候运行
		System.out.printf("init a class");
	}
	@PreDestroy
	public void destroy() { //类销毁的时候运行
		System.out.print("destory a class");
	}
```




