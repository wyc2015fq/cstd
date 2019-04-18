# Spring AOP + AspectJ framework - z69183787的专栏 - CSDN博客
2012年12月10日 10:00:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：625
个人分类：[Spring-Aop & 代理																[Spring-知识](https://blog.csdn.net/z69183787/article/category/2175163)](https://blog.csdn.net/z69183787/article/category/2175403)
 转载出处：[http://www.blogjava.net/fancydeepin](http://www.blogjava.net/fancydeepin) ]
AOP（Aspect Orient Programming），也就是常说的面向方面编程，它是作为面向对象编程的一种补充，专门用于处理系统中分布于各个模块（不同方法）
中的交叉关注点的问题，在 Java EE 应用中，常常通过 AOP 来处理一些具有横切性质的系统级服务，如事务管理、安全检查、缓存、对象池管理等。
简单点来说，它就是一个拦截器可以拦截一些进程，例如，当某个方法执行时，Spring AOP 可以拦截该执行的方法，并允许在方法执行之前或之后添加额外的功能，
以上如若解释的不好，勿喷 -_- ||
AspectJ 是一个基于 Java 语言的 AOP 框架，提供了强大的 AOP 功能，Spring 从 2.0 起，对 AspectJ 功能都提供了支持 .
几个常用的 AspectJ 注解 : 
    @Before   在方法执行前被执行
    @After     在方法执行后被执行
    @AfterReturning     在方法执行后被执行，并同时拦截方法返回的结果
    @AfterThrowing      在方法抛出异常时候被执行，若方法不抛出异常，则不会被执行
    @Around   这个，不知道要怎么解释了，比较不好解释，就像你拦截了一个方法，并在适当的时候给予放行，放行前后可以做额外的处理，下面看示例就很容易明白了
环境 : 
eclipse  3.6
maven  3.0.4
spring   3.0.5
aspectj 1.6.11
pom.xml 清单 : 
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
<modelVersion>4.0.0</modelVersion>
<groupId>com.fancy</groupId>
<artifactId>spring-aop</artifactId>
<packaging>war</packaging>
<version>1.0</version>
<name>spring-aop Maven Webapp</name>
<url>http://maven.apache.org</url>
<properties>
<spring.version>3.0.5.RELEASE</spring.version>
<aspectj.version>1.6.11</aspectj.version>
</properties>
<dependencies>
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-core</artifactId>
<version>${spring.version}</version>
</dependency>
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-context</artifactId>
<version>${spring.version}</version>
</dependency>
<!-- Spring AOP + AspectJ -->
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-aop</artifactId>
<version>${spring.version}</version>
</dependency>
<dependency>
<groupId>org.aspectj</groupId>
<artifactId>aspectjrt</artifactId>
<version>${aspectj.version}</version>
</dependency>
<dependency>
<groupId>org.aspectj</groupId>
<artifactId>aspectjweaver</artifactId>
<version>${aspectj.version}</version>
</dependency>
<dependency>
<groupId>junit</groupId>
<artifactId>junit</artifactId>
<version>4.7</version>
<scope>test</scope>
</dependency>
</dependencies>
<build>
<finalName>spring-aop</finalName>
</build>
</project>
applicationContext.xml 清单 : 
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:p="http://www.springframework.org/schema/p" xmlns:tx="http://www.springframework.org/schema/tx"
  xmlns:aop="http://www.springframework.org/schema/aop" xmlns:context="http://www.springframework.org/schema/context"
  xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
  http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd
  http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.0.xsd
  http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.0.xsd">
<!-- 开启注解 -->
<context:annotation-config/>
<!-- 自动扫描 -->
<context:component-scan base-package="com.fancy"/>
<!-- 启动 AspectJ 支持 -->
<aop:aspectj-autoproxy />
</beans>
还是来编写 HelloWorld : 
1 . 编写 HelloWorld 接口
package com.fancy.service;
publicinterface HelloWorld {
publicvoid sayHi();
publicvoid sayHiAround(String username);
publicvoid sayHiThrowException() throws Exception;
public String  sayHiReturnValue();
}
2 . 编写 HelloWorld 接口的实现，并将其注解成 spring 的一个组件
package com.fancy.service.impl;
import com.fancy.service.HelloWorld;
import org.springframework.stereotype.Component;
@Component
publicclass HelloWorldImpl implements HelloWorld {
publicvoid sayHi() {
        System.out.println("sayHi ---->> Hi fancy !");
    }
publicvoid sayHiAround(String username) {
        System.out.println("sayHiAround ---->> Hi "+ username +" !");
    }
publicvoid sayHiThrowException() throws Exception {
        System.out.println("sayHiThrowException ---->> Hi fancy !");
thrownew Exception("Throw an exception here !!!!");
    }
public String sayHiReturnValue() {
        System.out.println("sayHiReturnValue ---->> Hi fancy !");
return"fancy";
    }
}
3 . 编写方面代码 : 
AspectJ @Before 示例
package com.fancy.aspect;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.springframework.stereotype.Component;
@Aspect
@Component
publicclass MyAspect {
    @Before("execution (* com.fancy.service.HelloWorld.sayHi(..))")
publicvoid logBefore(JoinPoint   joinPoint){
        System.out.println("logBefore() Method Invoke!");
        System.out.println("Hijack Method Name : "+ joinPoint.getSignature().getName());
    }
}
其中，@Before("execution (* com.fancy.service.HelloWorld.sayHi(..))") 中的 execution (* com.fancy.service.HelloWorld.sayHi(..)) 是切入点表达式，
更多的帮助信息可以查看 spring 的帮助文档，spring 3.0.5 的帮助文档中是在 第 7  章的 7.2.3.4 小节，因为文档上说的也不是太清楚，在这里我也不好说话，
其中的 execution 是用于匹配方法执行的连接点，那个 * 号所占的位不知道是不是代表方法的访问权限，文档上没说，网上也没找到相关解释，哪位知道的望告知啊~~
接下来的 com.fancy.service.HelloWorld.sayHi 就很明显了，就是切入点方法名，再接下来的是 (..)，(..) 代表匹配任意数量的参数，可以是 0 个也可以是多个 ;
如果你确定这个方法不需要参数，可以直接使用 ()，还可以使用 (*) 来匹配一个任意类型的参数，还可以使用 (* , String)，这样代表匹配两个参数，第二个参数必须是
String 类型的参数，这些在 spring 帮助文档的 7.2.3.4 小节都有说到，在这里就不多说了，可以自己去看，英文的看起来更带劲 *_*
再接下来的是 JoinPoint 接口，org.aspectj.lang.JoinPoint 接口表示的是目标类连接点对象，这个我也找不到相关的 API，只能手工整理一下了 : 
JoinPoint     API 
     java.lang.Object                getThis() ：获取代理对象本身；
     java.lang.Object           getTarget() ：获取连接点所在的目标对象； 
    Signature                   getSignature() ：获取连接点的方法签名对象； 
    java.lang.Object[]             getArgs()：获取连接点方法运行时的入参列表； 
Junit 测试
package junit.test;
import org.junit.Test;
import org.junit.BeforeClass;
import com.fancy.service.HelloWorld;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
publicclass TestApp {
privatestatic ApplicationContext context =null;
    @BeforeClass
publicstaticvoid setUpBeforeClass() throws Exception {
        context =new ClassPathXmlApplicationContext("applicationContext.xml");
    }
    @Test
publicvoid testMethod() throws Exception{
        HelloWorld helloworld = (HelloWorld)context.getBean("helloWorldImpl");
        helloworld.sayHi();
    }
}
后台输出 : 
logBefore() Method Invoke!
Hijack Method Name : sayHi
sayHi ---->> Hi fancy !
AspectJ @After 示例
package com.fancy.aspect;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.After;
import org.aspectj.lang.annotation.Aspect;
import org.springframework.stereotype.Component;
@Aspect
@Component
publicclass MyAspect {
    @After("execution (* com.fancy.service.HelloWorld.sayHi(..))")
publicvoid logAfter(JoinPoint   joinPoint){
        System.out.println("logAfter() Method Invoke!");
        System.out.println("Hijack Method Name : "+ joinPoint.getSignature().getName());
    }
}
Junit 测试
@Test
publicvoid testMethod() throws Exception{
        HelloWorld helloworld = (HelloWorld)context.getBean("helloWorldImpl");
        helloworld.sayHi();
    }
后台输出 : 
sayHi ---->> Hi fancy !
logAfter() Method Invoke!
Hijack Method Name : sayHi
AspectJ @AfterReturning 示例
package com.fancy.aspect;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.AfterReturning;
import org.aspectj.lang.annotation.Aspect;
import org.springframework.stereotype.Component;
@Aspect
@Component
publicclass MyAspect {
    @AfterReturning(pointcut ="execution (* com.fancy.service.HelloWorld.sayHiReturnValue(..))", returning ="returnValue")
publicvoid logAfterReturning(JoinPoint   joinPoint, Object/*String*/ returnValue){
        System.out.println("logAfterReturning() Method Invoke!");
        System.out.println("Hijack Method Name : "+ joinPoint.getSignature().getName());
        System.out.println("The Return Value Is : "+ returnValue);
    }
}
Junit 测试
@Test
publicvoid testMethod() throws Exception{
        HelloWorld helloworld = (HelloWorld)context.getBean("helloWorldImpl");
        helloworld.sayHiReturnValue();
    }
后台输出 : 
sayHiReturnValue ---->> Hi fancy !
logAfterReturning() Method Invoke!
Hijack Method Name : sayHiReturnValue
The Return Value Is : fancy
AspectJ @AfterThrowing 示例
package com.fancy.aspect;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.AfterThrowing;
import org.aspectj.lang.annotation.Aspect;
import org.springframework.stereotype.Component;
@Aspect
@Component
publicclass MyAspect {
    @AfterThrowing(pointcut ="execution (* com.fancy.service.HelloWorld.sayHiThrowException(..))", throwing ="error")
publicvoid logAfterThrowing(JoinPoint joinPoint, Throwable error){
        System.out.println("logAfterThrowing() Method Invoke!");
        System.out.println("Hijack Method Name : "+ joinPoint.getSignature().getName());
        System.out.println("Exception Message  :"+ error);
    }
}
Junit 测试
@Test
publicvoid testMethod() throws Exception{
        HelloWorld helloworld = (HelloWorld)context.getBean("helloWorldImpl");
        helloworld.sayHiThrowException();
    }
后台输出 : 
sayHiThrowException ---->> Hi fancy !
logAfterThrowing() Method Invoke!
Hijack Method Name : sayHiThrowException
Exception Message  :java.lang.Exception: Throw an exception here !!!!
若将 HelloWorldImpl 类中 sayHiThrowException 方法的异常抛出注释掉，
publicvoid sayHiThrowException() throws Exception {
        System.out.println("sayHiThrowException ---->> Hi fancy !");
//throw new Exception("Throw an exception here !!!!");
    }
其余不变，再次执行 Junit 测试，后台输出 : 
sayHiThrowException ---->> Hi fancy !
这就说明，当该方法能够运行正常的时候，没有抛出异常，则，logAfterThrowing 不会被执行 .
AspectJ @Around 示例
package com.fancy.aspect;
import java.util.Arrays;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.springframework.stereotype.Component;
@Aspect
@Component
publicclass MyAspect {
    @Around("execution (* com.fancy.service.HelloWorld.sayHiAround(..))")
publicvoid logAround(ProceedingJoinPoint   joinPoint) throws Throwable {
        System.out.println("logAround() Method Invoke!");
        System.out.println("Hijack Method Name  : "+ joinPoint.getSignature().getName());
        System.out.println("Hijack Arguments Is : "+ Arrays.toString(joinPoint.getArgs()));
        System.out.println("Around before : can do something here !");
        joinPoint.proceed(); //放行
        System.out.println("Around after  : can do something here !");
    }
}
Junit 测试
@Test
publicvoid testMethod() throws Exception{
        HelloWorld helloworld = (HelloWorld)context.getBean("helloWorldImpl");
        helloworld.sayHiAround("fancy");
    }
后台输出 : 
logAround() Method Invoke!
Hijack Method Name  : sayHiAround
Hijack Arguments Is : [fancy]
Around before : can do something here !
sayHiAround ---->> Hi fancy !
Around after  : can do something here !
其中，需要提一下 ProceedingJoinPoint 接口 :
ProceedingJoinPoint 继承于 JoinPoint，是其子接口，它新增了两个用于执行连接点方法的方法： 
     java.lang.Object proceed() throws java.lang.Throwable：通过反射执行目标对象的连接点处的方法； 
     java.lang.Object proceed(java.lang.Object[] args) throws java.lang.Throwable：通过反射执行目标对象连接点处的方法，不过使用新的入参替换原来的入参。
最后附上 spring 3.0.5 帮助文档中的一些信息 : 
Some examples of common pointcut expressions are given below.
- 
the execution of any public method:
execution(public * *(..))
- 
the execution of any method with a name beginning with "set":
execution(* set*(..))
- 
the execution of any method defined by the `AccountService` interface:
execution(* com.xyz.service.AccountService.*(..))
- 
the execution of any method defined in the service package:
execution(* com.xyz.service.*.*(..))
- 
the execution of any method defined in the service package or a sub-package:
execution(* com.xyz.service..*.*(..))
- 
any join point (method execution only in Spring AOP) within the service package:
within(com.xyz.service.*)
- 
any join point (method execution only in Spring AOP) within the service package or a sub-package:
within(com.xyz.service..*)
- 
any join point (method execution only in Spring AOP) where the proxy implements the `AccountService` interface:
this(com.xyz.service.AccountService)
*'this' is more commonly used in a binding form :- see the following section on advice for how to make the proxy object available in the advice body.*
- 
any join point (method execution only in Spring AOP) where the target object implements the `AccountService` interface:
target(com.xyz.service.AccountService)
*'target' is more commonly used in a binding form :- see the following section on advice for how to make the target object available in the advice body.*
- 
any join point (method execution only in Spring AOP) which takes a single parameter, and where the argument passed at runtime is `Serializable`:
args(java.io.Serializable)
*'args' is more commonly used in a binding form :- see the following section on advice for how to make the method arguments available in the advice body.*
Note that the pointcut given in this example is different to `execution(* *(java.io.Serializable))`: the args version matches if the argument passed at runtime is Serializable,
 the execution version matches if the method signature declares a single parameter of type `Serializable`.
- 
any join point (method execution only in Spring AOP) where the target object has an `@Transactional` annotation:
@target(org.springframework.transaction.annotation.Transactional)
*'@target' can also be used in a binding form :- see the following section on advice for how to make the annotation object available in the advice body.*
- 
any join point (method execution only in Spring AOP) where the declared type of the target object has an `@Transactional` annotation:
@within(org.springframework.transaction.annotation.Transactional)
*'@within' can also be used in a binding form :- see the following section on advice for how to make the annotation object available in the advice body.*
- 
any join point (method execution only in Spring AOP) where the executing method has an `@Transactional` annotation:
@annotation(org.springframework.transaction.annotation.Transactional)
*'@annotation' can also be used in a binding form :- see the following section on advice for how to make the annotation object available in the advice body.*
- 
any join point (method execution only in Spring AOP) which takes a single parameter, and where the runtime type of the argument passed has the`@Classified` annotation:
@args(com.xyz.security.Classified)
*'@args' can also be used in a binding form :- see the following section on advice for how to make the annotation object(s) available in the advice body.*
- 
any join point (method execution only in Spring AOP) on a Spring bean named '`tradeService`':
bean(tradeService)
- 
any join point (method execution only in Spring AOP) on Spring beans having names that match the wildcard expression '`*Service`':
bean(*Service)
