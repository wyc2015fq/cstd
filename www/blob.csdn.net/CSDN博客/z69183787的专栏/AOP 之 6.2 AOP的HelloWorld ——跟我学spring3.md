# AOP 之 6.2 AOP的HelloWorld ——跟我学spring3 - z69183787的专栏 - CSDN博客
2013年12月05日 10:58:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：777
个人分类：[Spring-Aop & 代理](https://blog.csdn.net/z69183787/article/category/2175403)

### 6.2.1  准备环境
       首先准备开发需要的jar包，请到spring-framework-3.0.5.RELEASE-dependencies.zip和spring-framework-3.0.5.RELEASE-with-docs中查找如下jar包：
org.springframework.aop-3.0.5.RELEASE.jar
com.springsource.org.aspectj.weaver-1.6.8.RELEASE.jar
com.springsource.org.aopalliance-1.0.0.jar
com.springsource.net.sf.cglib-2.2.0.jar

将这些jar包添加到“Build Path”下。
### 6.2.2  定义目标类
       1）定义目标接口：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2467.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2467.html#)[打印](http://sishuok.com/forum/blogPost/list/2467.html#)
- package cn.javass.spring.chapter6.service;  
- publicinterface IHelloWorldService {  
- publicvoid sayHello();  
- }  
       2）定义目标接口实现：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2467.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2467.html#)[打印](http://sishuok.com/forum/blogPost/list/2467.html#)
- package cn.javass.spring.chapter6.service.impl;  
- import cn.javass.spring.chapter6.service.IHelloWorldService;  
- publicclass HelloWorldService implements IHelloWorldService {  
- @Override
- publicvoid sayHello() {  
-         System.out.println("============Hello World!");  
-     }  
- }  
- 
       注：在日常开发中最后将业务逻辑定义在一个专门的service包下，而实现定义在service包下的impl包中，服务接口以IXXXService形式，而服务实现就是XXXService，这就是规约设计，见名知义。当然可以使用公司内部更好的形式，只要大家都好理解就可以了。
### 6.2.2  定义切面支持类
       有了目标类，该定义切面了，切面就是通知和切入点的组合，而切面是通过配置方式定义的，因此这定义切面前，我们需要定义切面支持类，切面支持类提供了通知实现：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2467.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2467.html#)[打印](http://sishuok.com/forum/blogPost/list/2467.html#)
- package cn.javass.spring.chapter6.aop;  
- publicclass HelloWorldAspect {  
- //前置通知
- publicvoid beforeAdvice() {  
-         System.out.println("===========before advice");  
- }  
- //后置最终通知
- publicvoid afterFinallyAdvice() {  
-         System.out.println("===========after finally advice");  
-     }  
- }  
       此处HelloWorldAspect类不是真正的切面实现，只是定义了通知实现的类，在此我们可以把它看作就是缺少了切入点的切面。
       注：对于AOP相关类最后专门放到一个包下，如“aop”包，因为AOP是动态织入的，所以如果某个目标类被AOP拦截了并应用了通知，可能很难发现这个通知实现在哪个包里，因此推荐使用规约命名，方便以后维护人员查找相应的AOP实现。
### 6.2.3  在XML中进行配置
       有了通知实现，那就让我们来配置切面吧：
       1）首先配置AOP需要aop命名空间，配置头如下：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2467.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2467.html#)[打印](http://sishuok.com/forum/blogPost/list/2467.html#)
- <?xml version="1.0" encoding="UTF-8"?>  
- <beans  xmlns="http://www.springframework.org/schema/beans"
-         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-         xmlns:aop="http://www.springframework.org/schema/aop"
-         xsi:schemaLocation="  
-            http://www.springframework.org/schema/beans
-            http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
-            http://www.springframework.org/schema/aop
-            http://www.springframework.org/schema/aop/spring-aop-3.0.xsd">
- </beans>  
- 
       2）配置目标类：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2467.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2467.html#)[打印](http://sishuok.com/forum/blogPost/list/2467.html#)
- <bean id="helloWorldService"
- class="cn.javass.spring.chapter6.service.impl.HelloWorldService"/>  
- 
       3）配置切面：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2467.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2467.html#)[打印](http://sishuok.com/forum/blogPost/list/2467.html#)
- <bean id="aspect"class="cn.javass.spring.chapter6.aop.HelloWorldAspect"/>  
- <aop:config>  
- <aop:pointcut id="pointcut" expression="execution(* cn.javass..*.*(..))"/>  
-     <aop:aspect ref="aspect">  
-         <aop:before pointcut-ref="pointcut" method="beforeAdvice"/>  
-         <aop:after pointcut="execution(* cn.javass..*.*(..))" method="afterFinallyAdvice"/>  
-     </aop:aspect>  
- </aop:config>  
       切入点使用<aop:config>标签下的<aop:pointcut>配置，expression属性用于定义切入点模式，默认是AspectJ语法，“execution(* cn.javass..*.*(..))”表示匹配cn.javass包及子包下的任何方法执行。
切面使用<aop:config>标签下的<aop:aspect>标签配置，其中“ref”用来引用切面支持类的方法。
前置通知使用<aop:aspect>标签下的<aop:before>标签来定义，pointcut-ref属性用于引用切入点Bean，而method用来引用切面通知实现类中的方法，该方法就是通知实现，即在目标类方法执行之前调用的方法。
最终通知使用<aop:aspect>标签下的<aop:after >标签来定义，切入点除了使用pointcut-ref属性来引用已经存在的切入点，也可以使用pointcut属性来定义，如pointcut="execution(* cn.javass..*.*(..))"，method属性同样是指定通知实现，即在目标类方法执行之后调用的方法。
### 6.2.4    运行测试
测试类非常简单，调用被代理Bean跟调用普通Bean完全一样，Spring AOP将为目标对象创建AOP代理，具体测试代码如下：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2467.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2467.html#)[打印](http://sishuok.com/forum/blogPost/list/2467.html#)
- package cn.javass.spring.chapter6;  
- import org.junit.Test;  
- import org.springframework.context.ApplicationContext;  
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- import cn.javass.spring.chapter6.service.IHelloWorldService;  
- import cn.javass.spring.chapter6.service.IPayService;  
- publicclass AopTest {  
- @Test
- publicvoid testHelloworld() {  
-         ApplicationContext ctx =  new ClassPathXmlApplicationContext("chapter6/helloworld.xml");  
-         IHelloWorldService helloworldService =  
-         ctx.getBean("helloWorldService", IHelloWorldService.class);  
-         helloworldService.sayHello();  
-     }  
- }  
- 
       该测试将输出如下如下内容：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2467.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2467.html#)[打印](http://sishuok.com/forum/blogPost/list/2467.html#)
- ===========before advice  
- ============Hello World!  
- ===========after finally advice  
       从输出我们可以看出：前置通知在切入点选择的连接点（方法）之前允许，而后置通知将在连接点（方法）之后执行，具体生成AOP代理及执行过程如图6-4所示。
![](http://sishuok.com/forum/upload/2012/2/21/b362f09e4812acff043a4c8bfc2ce1d0__1.JPG)
图6-4 Spring AOP框架生成AOP代理过程
原创内容 转自请注明出处【[http://sishuok.com/forum/blogPost/list/2467.html](http://sishuok.com/forum/blogPost/list/2467.html)】
