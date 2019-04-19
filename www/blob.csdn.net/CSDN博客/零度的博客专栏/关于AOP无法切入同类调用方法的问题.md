# 关于AOP无法切入同类调用方法的问题 - 零度的博客专栏 - CSDN博客
2016年05月04日 14:20:27[零度anngle](https://me.csdn.net/zmx729618)阅读数：7160
最近在开发中遇到一个问题，当在Service中定义了一个方法并且切入之后，从Controller里面调用该方法可以实现切入，但是当在同一个Service中实现另一方法并调用改方法时却无法切入。代码类似于：
1、service
```
package zmx.spring.aop.test2;
import org.springframework.aop.framework.AopContext;
public class TestService {
	
	public void callMethodA(){
		System.out.println("Method A  is  called");
		callMethodB(); //----------->AOP不能
		//((TestService) AopContext.currentProxy()).callMethodB();  
		System.out.println("Method A  is  called  over");
	}
	
	
	public void callMethodB(){
		
		System.out.println("Method B  is  called");
	}
}
```
2、aspect
```
package zmx.spring.aop.test2;
import org.aspectj.lang.annotation.AfterReturning;
import org.aspectj.lang.annotation.Aspect;
@Aspect
public class TestAspect {
	
	@AfterReturning("execution(* zmx.spring.aop.test2.TestService.callMethodB(..))")  
	public void after() {  
	System.out.println("after call and do something.");
	}
}
```
3、mainTest
```
package zmx.spring.aop.test2;
import org.junit.Test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
public class MainTest {
	
	@Test
	public void testCallMethodA(){
		
		    ApplicationContext ctx = new ClassPathXmlApplicationContext("zmx/spring/applicationContext.xml");
		    TestService testService = ctx.getBean("testService", TestService.class);  
		    testService.callMethodA();		    		   		   
		
	}
}
```
4、applicationContext.xml
```
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:aop="http://www.springframework.org/schema/aop"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:jee="http://www.springframework.org/schema/jee" 
       xmlns:tx="http://www.springframework.org/schema/tx"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
      xsi:schemaLocation="http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.0.xsd   
       http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd   
       http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.0.xsd   
       http://www.springframework.org/schema/jee http://www.springframework.org/schema/jee/spring-jee-3.0.xsd   
       http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd">
```
```
<!-- 开启支持@AspectJ风格的切面声明 -->
      <aop:aspectj-autoproxy/>  
     <!--<aop:aspectj-autoproxy proxy-target-class="true" expose-proxy="true" />  -->      
     
    <!-- 目标对象 --> 
    <bean id="testService"  class="zmx.spring.aop.test2.TestService"  />
    
    <!-- 切面 --> 
    <bean id="testAspect"  class="zmx.spring.aop.test2.TestAspect"  />
    
    
    
 
    
    
</beans>
```
总之这样的调用根本不会进去切面，找了很久资料都不知道怎么回事，果然我还是太弱了。后来有幸得问高人，指点说Service中如此调用并非调用的是代理类中的方法，是不会被切进去的。换言之，必须要调用代理类才会被切进去。 那么应该怎么破呢？既然只有调用代理类的方法才能切入，那我们拿到代理类不就好了嘛。尝试性的在IDE里面搜Aop相关的类，一眼就看到一个叫AopContext的东西，看来游戏啊，里面有一个方法叫做currentProxy()，返回一个Object。怎么样都要试一下，果断的把这个object打印出来。报错了……
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/45622821#)[copy](http://blog.csdn.net/z69183787/article/details/45622821#)[print](http://blog.csdn.net/z69183787/article/details/45622821#)[?](http://blog.csdn.net/z69183787/article/details/45622821#)
- java.lang.IllegalStateException: Cannot find current proxy: Set 'exposeProxy' property on Advised to 'true' to make it available.  
```java
<span style="font-size:12px;">java.lang.IllegalStateException: Cannot find current proxy: Set 'exposeProxy' property on Advised to 'true' to make it available.
</span>
```
等等，这个错有点眼熟，之前傻傻找不到问题的时候看过Spring的配置。
**[xml]**[view plain](http://blog.csdn.net/z69183787/article/details/45622821#)[copy](http://blog.csdn.net/z69183787/article/details/45622821#)[print](http://blog.csdn.net/z69183787/article/details/45622821#)[?](http://blog.csdn.net/z69183787/article/details/45622821#)
- <aop:aspectj-autoproxyproxy-target-class="true"expose-proxy="true"/>
<span style="font-size:12px;"><aop:aspectj-autoproxy proxy-target-class="true"expose-proxy="true"/>
</span>
把expose-proxy设成true，再跑一次，终于没有报错了，而且打印出了令人欣喜的结果，currentProxy()的结果就是当前Service的代理类，那么事情简单了。修改Service的callMethodA()方法如下：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/45622821#)[copy](http://blog.csdn.net/z69183787/article/details/45622821#)[print](http://blog.csdn.net/z69183787/article/details/45622821#)[?](http://blog.csdn.net/z69183787/article/details/45622821#)
- ((Service) AopContext.currentProxy()).callMethodB();  
```java
<span style="font-size:12px;">((Service) AopContext.currentProxy()).callMethodB();
</span>
```
成功了，方法成功的切入进去了，等下只要再把获取代理类的方法独立处理就好了。
