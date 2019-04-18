# 使用Spring的注解方式实现AOP - z69183787的专栏 - CSDN博客
2016年06月30日 11:05:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：976
个人分类：[Spring-Aop & 代理](https://blog.csdn.net/z69183787/article/category/2175403)
Spring对AOP的实现提供了很好的支持。下面我们就使用Spring的注解来完成AOP做一个例子。
首先，为了使用Spring的AOP注解功能，必须导入如下几个包。aspectjrt.jar,aspectjweaver.jar,cglib-nodep.jar.
然后我们写一个接口
**[java]**[view
 plain](http://blog.csdn.net/a352193394/article/details/7345860#)[copy](http://blog.csdn.net/a352193394/article/details/7345860#)
[print](http://blog.csdn.net/a352193394/article/details/7345860#)[?](http://blog.csdn.net/a352193394/article/details/7345860#)
- package com.bird.service;  
- 
- publicinterface PersonServer {  
- 
- publicvoid save(String name);  
- publicvoid update(String name, Integer id);  
- public String getPersonName(Integer id);  
- 
- }  
和一个接口实现类
**[java]**[view
 plain](http://blog.csdn.net/a352193394/article/details/7345860#)[copy](http://blog.csdn.net/a352193394/article/details/7345860#)
[print](http://blog.csdn.net/a352193394/article/details/7345860#)[?](http://blog.csdn.net/a352193394/article/details/7345860#)
- package com.bird.service.impl;  
- 
- import com.bird.service.PersonServer;  
- 
- publicclass PersonServiceBean implements PersonServer{  
- 
- @Override
- publicvoid save(String name) {  
- 
-         System.out.println("我是save方法");  
- //  throw new RuntimeException();
-     }  
- 
- @Override
- publicvoid update(String name, Integer id) {  
- 
-         System.out.println("我是update()方法");  
-     }  
- 
- @Override
- public String getPersonName(Integer id) {  
- 
-         System.out.println("我是getPersonName()方法");  
- return"xxx";  
-     }  
- 
- }  
下面使用Spring注解方式对这个Bean进行方法拦截
**[java]**[view
 plain](http://blog.csdn.net/a352193394/article/details/7345860#)[copy](http://blog.csdn.net/a352193394/article/details/7345860#)
[print](http://blog.csdn.net/a352193394/article/details/7345860#)[?](http://blog.csdn.net/a352193394/article/details/7345860#)
- package com.bird.service;  
- 
- import org.aspectj.lang.ProceedingJoinPoint;  
- import org.aspectj.lang.annotation.After;  
- import org.aspectj.lang.annotation.AfterReturning;  
- import org.aspectj.lang.annotation.AfterThrowing;  
- import org.aspectj.lang.annotation.Around;  
- import org.aspectj.lang.annotation.Aspect;  
- import org.aspectj.lang.annotation.Before;  
- import org.aspectj.lang.annotation.Pointcut;  
- 
- /**
-  * 切面
-  * @author Bird
-  *
-  */
- @Aspect
- publicclass MyInterceptor {  
- @Pointcut("execution(* com.bird.service.impl.PersonServiceBean.*(..))")  
- privatevoid anyMethod(){}//定义一个切入点
- 
- @Before("anyMethod() && args(name)")  
- publicvoid doAccessCheck(String name){  
-         System.out.println(name);  
-         System.out.println("前置通知");  
-     }  
- 
- @AfterReturning("anyMethod()")  
- publicvoid doAfter(){  
-         System.out.println("后置通知");  
-     }  
- 
- @After("anyMethod()")  
- publicvoid after(){  
-         System.out.println("最终通知");  
-     }  
- 
- @AfterThrowing("anyMethod()")  
- publicvoid doAfterThrow(){  
-         System.out.println("例外通知");  
-     }  
- 
- @Around("anyMethod()")  
- public Object doBasicProfiling(ProceedingJoinPoint pjp) throws Throwable{  
-         System.out.println("进入环绕通知");  
-         Object object = pjp.proceed();//执行该方法
-         System.out.println("退出方法");  
- return object;  
-     }  
- }  
**[java]**[view
 plain](http://blog.csdn.net/a352193394/article/details/7345860#)[copy](http://blog.csdn.net/a352193394/article/details/7345860#)
[print](http://blog.csdn.net/a352193394/article/details/7345860#)[?](http://blog.csdn.net/a352193394/article/details/7345860#)
- @Pointcut("execution(* com.bird.service.impl.PersonServiceBean.*(..))")  
这句话是方法切入点，execution为执行的意思，*代表任意返回值，然后是包名，.*意思是包下面的所有子包。(..)代
表各种方法.
然后下面的注解就比较简单了，就是在使用方法前和中，还有环绕拦截/
然后在Spring的配置文件中继续配置Bean,需要打开AOP命名空间
**[java]**[view
 plain](http://blog.csdn.net/a352193394/article/details/7345860#)[copy](http://blog.csdn.net/a352193394/article/details/7345860#)
[print](http://blog.csdn.net/a352193394/article/details/7345860#)[?](http://blog.csdn.net/a352193394/article/details/7345860#)
- <?xml version="1.0" encoding="UTF-8"?>  
- <beans xmlns="http://www.springframework.org/schema/beans"
-        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-        xmlns:aop="http://www.springframework.org/schema/aop"
-        xsi:schemaLocation="  
-        http://www.springframework.org/schema/beans 
-        http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
-        http://www.springframework.org/schema/aop 
-        http://www.springframework.org/schema/aop/spring-aop-2.5.xsd">
- 
-         <aop:aspectj-autoproxy/>  
-     <bean id="personServiceBean"class="com.bird.service.impl.PersonServiceBean"/>  
-     <bean id="myInterceptor"class="com.bird.service.MyInterceptor"/>  
- 
- </beans>  
然后建立一个Junit测试
**[java]**[view
 plain](http://blog.csdn.net/a352193394/article/details/7345860#)[copy](http://blog.csdn.net/a352193394/article/details/7345860#)
[print](http://blog.csdn.net/a352193394/article/details/7345860#)[?](http://blog.csdn.net/a352193394/article/details/7345860#)
- package junit.test;  
- 
- import org.junit.Test;  
- import org.springframework.context.ApplicationContext;  
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- 
- import com.bird.service.PersonServer;  
- 
- publicclass SpringAOPTest {  
- 
- @Test
- publicvoid inteceptorTest(){  
-         ApplicationContext ctx = new ClassPathXmlApplicationContext("beanAop.xml");  
-         PersonServer bean = (PersonServer)ctx.getBean("personServiceBean");  
-         bean.save(null);  
-     }  
- 
- 
- }  
测试结果为
**[java]**[view
 plain](http://blog.csdn.net/a352193394/article/details/7345860#)[copy](http://blog.csdn.net/a352193394/article/details/7345860#)
[print](http://blog.csdn.net/a352193394/article/details/7345860#)[?](http://blog.csdn.net/a352193394/article/details/7345860#)
- 2012-3-1218:08:39 org.springframework.context.support.AbstractApplicationContext prepareRefresh  
- 信息: Refreshing org.springframework.context.support.ClassPathXmlApplicationContext@dd20f6: display name [org.springframework.context.support.ClassPathXmlApplicationContext@dd20f6]; startup date [Mon Mar 1218:08:39 CST 2012]; root of context hierarchy  
- 2012-3-1218:08:40 org.springframework.beans.factory.xml.XmlBeanDefinitionReader loadBeanDefinitions  
- 信息: Loading XML bean definitions from class path resource [beanAop.xml]  
- 2012-3-1218:08:40 org.springframework.context.support.AbstractApplicationContext obtainFreshBeanFactory  
- 信息: Bean factory for application context [org.springframework.context.support.ClassPathXmlApplicationContext@dd20f6]: org.springframework.beans.factory.support.DefaultListableBeanFactory@b0bad7
- 2012-3-1218:08:40 org.springframework.beans.factory.support.DefaultListableBeanFactory preInstantiateSingletons  
- 信息: Pre-instantiating singletons in org.springframework.beans.factory.support.DefaultListableBeanFactory@b0bad7: defining beans [org.springframework.aop.config.internalAutoProxyCreator,personServiceBean,myInterceptor]; root of factory hierarchy  
- null
- 前置通知  
- 进入环绕通知  
- 我是save方法  
- 后置通知  
- 退出方法  
- 最终通知  
- 
