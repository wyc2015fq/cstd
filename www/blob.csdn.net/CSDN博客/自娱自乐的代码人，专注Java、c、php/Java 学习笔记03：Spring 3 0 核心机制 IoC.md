# Java 学习笔记03：Spring 3.0 核心机制 IoC - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月15日 13:56:22[initphp](https://me.csdn.net/initphp)阅读数：2029
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- 什么是Spring的IOC

> 
Spring的核心机制是IOC。通常被称为“控制反转”，也被叫做“依赖注入”。它是一种设计模式，由框架或者容器将被调用的类注入给调用对象，以此来解除调用对象和被调用类之间的依赖关系。这种模式主要关注组件的依赖性，配置以及组件的生命周期。

PS:看到上面一堆名词估计都闷了，其实IOC就是例如PHP中的工厂函数，所有的类的实例化或者使用都通过统一的接口函数，而Spring通过配置XML（你妈，XML反人类）配置，然后知道需要实例化哪个类。




- 创建一个Spring IoC简单示例
- 创建目录和文件。创建com.spring.main这个包，里面创建一个Index.java程序入口类。创建com.spring.user这个包，里面创建一个User.java的User类，主要处理用户业务逻辑。创建一个bean.xml配置文件，这个配置文件主要放置Spring框架IoC的配置。

![](https://img-my.csdn.net/uploads/201211/15/1352958238_2036.jpg)


- 在User.java中，创建一个say的方法。

```java
package com.spring.user;

public class User {
	
	public void say() {
		System.out.print("Hello Spring");
	}
}
```


- 在Index.java中，创建一个main程序入口方法，并且使用原始的方式实例化User类，调用say方法。

```java
package com.spring.main;

import com.spring.user.User;

public class Index {
	
	public static void main(String args[]) {
		//原始方法实例化User类
		User user = new User();
		user.say();
	}
}
```


- 我要使用Spring的IoC调用一个类，那么我们先配置bean.xml文件，代码中，id代表需要调用的名称，class是需要对应调用的类，Spring的IOC通过这个XML配置就能轻松映射调用的对象。


```
<?xml version="1.0" encoding="UTF-8"?>
<beans  
    xmlns="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
    xsi:schemaLocation="http://www.springframework.org/schema/beans 

http://www.springframework.org/schema/beans/spring-beans-2.0.xsd">  
    
    <bean id="User" class="com.spring.user.User"></bean>
</beans>
```



- 我们继续在我们的入口文件中修改，采用Spring的IOC机制调用类。


```java
package com.spring.main;

import org.springframework.beans.factory.BeanFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.spring.user.User;

public class Index {
	
	public static void main(String args[]) {
		//原始方法实例化User类
		User user = new User();
		user.say();
		
		//使用Spring的 IOC方法
		BeanFactory Factory = null;
		//调用src/bean.xml文件
		Factory = new ClassPathXmlApplicationContext("bean.xml"); 
		//getBean方法调用对象实例，参数User为xml文件中对应的id
		User User = (User) Factory.getBean("User"); 
		User.say();
	}
}
```


- 最后运行配置，可以看到输出结果
![](https://img-my.csdn.net/uploads/201211/15/1352959114_7523.jpg)





> 
点评：Spring的 IOC并没有想象中的那么难，其实就是我们PHP中的Common::load这样的调用类的工厂函数。只不过Spring中使用getBean("User")这样的函数来调用了，而且它还依赖XML配置而已。




- Spring IoC深入理解。
- Spring Ioc最基本的接口是BeanFactory，提供了四个主要方法：

containsBean() 判断容器中是否包含id为name的Bean定义，返回Bool值

getBean(String name) 获取容器中id为name的Bean

getBean(Stringname, Class Type) 返回容器中id为name的并且类型为type的Bean

getType 返回容器id为name的Bean类型


```java
//使用Spring的 IOC方法
		ApplicationContext Factory = null;
		//调用src/bean.xml文件
		Factory = new ClassPathXmlApplicationContext("bean.xml"); 
		if (Factory.containsBean("User")) {
			System.out.print("Yes User is exist");
		}
```

- 一般不太直接使用BeanFactory，会使用它的子类ApplicationContext，ApplicationContext提供两个方法：

FileSystemXmlApplicationContext : 可以指定XML路径，一般bean.xml没有放在CLASSPATH目录下的时候会使用。

ClassPathXmlApplicationContext:以CLASSPATH路径下的XML配置文件创建。


```java
package com.spring.main;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.context.support.FileSystemXmlApplicationContext;

import com.spring.user.User;

public class Index {
	
	public static void main(String args[]) {
		//原始方法实例化User类
		User user = new User();
		user.say();
		
		//使用Spring的 IOC方法
		ApplicationContext Factory = null;
		//调用src/bean.xml文件
		Factory = new ClassPathXmlApplicationContext("bean.xml"); //直接在src目录下就可以使用
		Factory = new FileSystemXmlApplicationContext("src/bean.xml");//需要详细路径
		if (Factory.containsBean("User")) {
			System.out.print("Yes User is exist");
		}
		//getBean方法调用对象实例，参数User为xml文件中对应的id
		User User = (User) Factory.getBean("User"); 
		User.say();
	}
}
```


- 类的实例化是动态的，所以会自动实例化。




