# SpringAOP实例之简单实现（xml形式） - 零度的博客专栏 - CSDN博客
2016年04月21日 09:31:56[零度anngle](https://me.csdn.net/zmx729618)阅读数：404标签：[AOP](https://so.csdn.net/so/search/s.do?q=AOP&t=blog)
个人分类：[Spring](https://blog.csdn.net/zmx729618/article/category/6168885)
#### 1  准备环境
       首先准备开发需要的jar包，将这些jar包添加到“Build Path”下。
####  2  定义目标类
       1）定义目标接口：
```
<span style="font-size:12px;">package zmx.spring.aop.test;
public interface  IHelloWorldService {
	
	public void sayHello(); 
}
</span>
```
       2）定义目标接口实现：
```
<span style="font-size:12px;">package zmx.spring.aop.test;
public class HelloWorldService implements IHelloWorldService {
	@Override
	public void sayHello() {
		 System.out.println("============Hello World!================");  	
	}
	
	
}
</span>
```
       注：在日常开发中最后将业务逻辑定义在一个专门的service包下，而实现定义在service包下的impl包中，服务接口以IXXXService形式，而服务实现就是XXXService，这就是规约设计，见名知义。当然可以使用公司内部更好的形式，只要大家都好理解就可以了。
####  3  定义切面支持类
       有了目标类，该定义切面了，切面就是通知和切入点的组合，而切面是通过配置方式定义的，因此这定义切面前，我们需要定义切面支持类，切面支持类提供了通知实现：
```
<span style="font-size:12px;">package zmx.spring.aop.test;
public class HelloWorldAspect {
	
    //前置通知  
    public void beforeAdvice() {  
        System.out.println("===========before advice");  
    }  
    //后置通知  
    public void afterAdvice() {  
        System.out.println("===========after advice");  
    }
}
</span>
```
       此处HelloWorldAspect类不是真正的切面实现，只是定义了通知实现的类，在此我们可以把它看作就是缺少了切入点的切面。
       注：对于AOP相关类最后专门放到一个包下，如“aop”包，因为AOP是动态织入的，所以如果某个目标类被AOP拦截了并应用了通知，可能很难发现这个通知实现在哪个包里，因此推荐使用规约命名，方便以后维护人员查找相应的AOP实现。
###  4  在XML中进行配置
       有了通知实现，那就让我们来配置切面吧：
       1）首先配置AOP需要aop命名空间，配置头如下：
```
<span style="font-size:12px;"><?xml version="1.0" encoding="UTF-8" standalone="no"?>
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
       http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd"></span>
```
       2）配置目标类：
```
<span style="font-size:12px;">    <!-- 目标对象 --> 
    <bean id="helloWorldService"  class="zmx.spring.aop.test.HelloWorldService"  /></span>
```
       3）配置对象：
```
<span style="font-size:12px;">    <!-- 切面对象 -->
    <bean id="aspect" class="zmx.spring.aop.test.HelloWorldAspect"/></span>
```
       4）配置切面：
```
<span style="font-size:12px;">	<!-- 配置切面 -->
	<aop:config>  
	    <aop:pointcut id="pointcut" expression="execution(* zmx.spring.aop.test..*.*(..))"/>  
	    <aop:aspect ref="aspect">  
	        <aop:before pointcut-ref="pointcut" method="beforeAdvice"/>  
	        <aop:after  pointcut="execution(* zmx.spring.aop.test..*.*(..))" method="afterAdvice"/>  
	    </aop:aspect>  
	</aop:config></span>
```
       切入点使用<aop:config>标签下的<aop:pointcut>配置，expression属性用于定义切入点模式，默认是AspectJ语法，“execution(* cn.javass..*.*(..))”表示匹配cn.javass包及子包下的任何方法执行。
切面使用<aop:config>标签下的<aop:aspect>标签配置，其中“ref”用来引用切面支持类的方法。
前置通知使用<aop:aspect>标签下的<aop:before>标签来定义，pointcut-ref属性用于引用切入点Bean，而method用来引用切面通知实现类中的方法，该方法就是通知实现，即在目标类方法执行之前调用的方法。
最终通知使用<aop:aspect>标签下的<aop:after >标签来定义，切入点除了使用pointcut-ref属性来引用已经存在的切入点，也可以使用pointcut属性来定义，如pointcut="execution(* cn.javass..*.*(..))"，method属性同样是指定通知实现，即在目标类方法执行之后调用的方法。
### 5 运行测试
测试类非常简单，调用被代理Bean跟调用普通Bean完全一样，Spring AOP将为目标对象创建AOP代理，具体测试代码如下：
```
<span style="font-size:12px;">package zmx.spring.aop.test;
import org.junit.Test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import zmx.spring.Person;
public class MainTest {
	@Test
	public void testHelloWorld(){
		
		    ApplicationContext ctx = new ClassPathXmlApplicationContext("zmx/spring/applicationContext.xml");
		    IHelloWorldService helloworldService = ctx.getBean("helloWorldService", IHelloWorldService.class);  
		    helloworldService.sayHello();  
		
	}
	
}</span>
```
       该测试将输出如下如下内容：
```
<span style="font-size:12px;">===========before advice
============Hello World!================
===========after finally advice</span>
```
