# Java 学习笔记04：Spring XML配置文件Bean - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月15日 15:21:32[initphp](https://me.csdn.net/initphp)阅读数：1550
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- Bean是什么？

> 
Bean是Spring容器初始化，装配和管理的对象，主要是我们在使用Spring IoC开发的时候需要配置的一个XML文件中的<bean>元素。



```
<?xml version="1.0" encoding="UTF-8"?>
<beans  
    xmlns="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
    xsi:schemaLocation="http://www.springframework.org/schema/beans 

http://www.springframework.org/schema/beans/spring-beans-2.0.xsd">  
    
    <bean id="User" class="com.spring.user.User" init-method="init" destroy-method="destroy">
    <property name="username">
    	<value>zhulixx</value>
    </property>
    <property name="age">
    	<value>50</value>
    </property>
    </bean>
</beans>
```



- Bean元素表：


|属性名称|说明|
|----|----|
|id|Bean的唯一标识|
|class|类名称，例如com.spring.user.User|
|name|别名|
|singleton|是否是单例模式，默认true|
|autowire|属性装配方式|
|scope|Bean的作用域，singleton单例，prototype原型，request每次HTTP请求都生成一个新的，session|
|init-method|默认初始化调用的方法名称|
|destroy-method|默认自动销毁的方法名称|
|abstract|如果为抽象的，则不能创建实例|
|parent|该类的父类标识或别名，即父类id或者name属性|
- 实例
- 给类赋默认值。一般调用的类中需要有setUsername这样的方法才可以赋值。下面<property name="username"> 就是给User类中username变量赋上默认的值。

```
<bean id="User" class="com.spring.user.User" init-method="init" destroy-method="destroy">
    <property name="username">
    	<value>zhulixx</value>
    </property>
    <property name="age">
    	<value>50</value>
    </property>
    </bean>
```

```java
//User类中有set值
       public void setUsername(String username) {
		this.username = username;
	}
	public void setAge(int age) {
		this.age = age;
	}
```



- 初始化方法和结束后销毁的方法。主要是init-method方法和destroy-method方法
```
<bean id="User" class="com.spring.user.User" init-method="init" destroy-method="destroy">
    <property name="username">
    	<value>zhulixx</value>
    </property>
    <property name="age">
    	<value>50</value>
    </property>
    </bean>
```


```java
//java代码中有初始化和销毁的具体方法	
public void init() {
		System.out.printf("init a class");
	}
	
	public void destroy() {
		System.out.print("destory a class");
	}
```


- 默认将一个对象赋值给该对象中的一个变量的方法
`<property name="impObj" ref="imp">`









