# AOP 之 6.4 基于@AspectJ的AOP ——跟我学spring3 - z69183787的专栏 - CSDN博客
2013年12月05日 15:49:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：625
个人分类：[Spring-Aop & 代理](https://blog.csdn.net/z69183787/article/category/2175403)

       Spring除了支持Schema方式配置AOP，还支持注解方式：使用@AspectJ风格的切面声明。
### 6.4.1  启用对@AspectJ的支持
       Spring默认不支持@AspectJ风格的切面声明，为了支持需要使用如下配置：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- <aop:aspectj-autoproxy/>  
这样Spring就能发现@AspectJ风格的切面并且将切面应用到目标对象。
### 6.4.2  声明切面
       @AspectJ风格的声明切面非常简单，使用@Aspect注解进行声明：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @Aspect()  
- Public class Aspect{  
- ……  
- }  
       然后将该切面在配置文件中声明为Bean后，Spring就能自动识别并进行AOP方面的配置：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- <bean id="aspect"class="……Aspect"/>  
       该切面就是一个POJO，可以在该切面中进行切入点及通知定义，接着往下看吧。
### 6.4.3  声明切入点
       @AspectJ风格的命名切入点使用org.aspectj.lang.annotation包下的@Pointcut+方法（方法必须是返回void类型）实现。
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @Pointcut(value="切入点表达式", argNames = "参数名列表")  
- publicvoid pointcutName(……) {}  
- 
value：指定切入点表达式；
argNames：指定命名切入点方法参数列表参数名字，可以有多个用“，”分隔，这些参数将传递给通知方法同名的参数，同时比如切入点表达式“args(param)”将匹配参数类型为命名切入点方法同名参数指定的参数类型。
pointcutName：切入点名字，可以使用该名字进行引用该切入点表达式。
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @Pointcut(value="execution(* cn.javass..*.sayAdvisorBefore(..)) && args(param)", argNames = "param")  
- publicvoid beforePointcut(String param) {}  
定义了一个切入点，名字为“beforePointcut”，该切入点将匹配目标方法的第一个参数类型为通知方法实现中参数名为“param”的参数类型。
### 6.4.4  声明通知
       @AspectJ风格的声明通知也支持5种通知类型：
一、前置通知：使用org.aspectj.lang.annotation 包下的@Before注解声明；
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @Before(value = "切入点表达式或命名切入点", argNames = "参数列表参数名")  
value：指定切入点表达式或命名切入点；
argNames：与Schema方式配置中的同义。
接下来示例一下吧：
1、定义接口和实现，在此我们就使用Schema风格时的定义；
2、定义切面：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- package cn.javass.spring.chapter6.aop;  
- import org.aspectj.lang.annotation.Aspect;  
- @Aspect
- publicclass HelloWorldAspect2 {  
- 
- }  
3、定义切入点：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @Pointcut(value="execution(* cn.javass..*.sayAdvisorBefore(..)) && args(param)", argNames = "param")  
- publicvoid beforePointcut(String param) {}  
4、定义通知：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @Before(value = "beforePointcut(param)", argNames = "param")  
- publicvoid beforeAdvice(String param) {  
-     System.out.println("===========before advice param:" + param);  
- }  
5、在chapter6/advice2.xml配置文件中进行如下配置：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- <?xml version="1.0" encoding="UTF-8"?>  
- <beans  xmlns="http://www.springframework.org/schema/beans"
-         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-         xmlns:aop="http://www.springframework.org/schema/aop"
-         xsi:schemaLocation="  
-            http://www.springframework.org/schema/beans
-            http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
-            http://www.springframework.org/schema/aop
-            http://www.springframework.org/schema/aop/spring-aop-3.0.xsd">
- 
-   <aop:aspectj-autoproxy/>  
-   <bean id="helloWorldService"
- class="cn.javass.spring.chapter6.service.impl.HelloWorldService"/>  
- 
-   <bean id="aspect"
- class="cn.javass.spring.chapter6.aop.HelloWorldAspect2"/>  
- 
- </beans>  
- 
6、测试代码cn.javass.spring.chapter6.AopTest:
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @Test
- publicvoid testAnnotationBeforeAdvice() {  
-     System.out.println("======================================");  
-     ApplicationContext ctx = new ClassPathXmlApplicationContext("chapter6/advice2.xml");  
-     IHelloWorldService helloworldService = ctx.getBean("helloWorldService", IHelloWorldService.class);  
-     helloworldService.sayBefore("before");  
-     System.out.println("======================================");  
- }  
将输出：
==========================================
===========before advice param:before
============say before
==========================================

切面、切入点、通知全部使用注解完成：
       1）使用@Aspect将POJO声明为切面；
       2）使用@Pointcut进行命名切入点声明，同时指定目标方法第一个参数类型必须是java.lang.String，对于其他匹配的方法但参数类型不一致的将也是不匹配的，通过argNames = "param"指定了将把该匹配的目标方法参数传递给通知同名的参数上；
       3）使用@Before进行前置通知声明，其中value用于定义切入点表达式或引用命名切入点；
       4）配置文件需要使用<aop:aspectj-autoproxy/>来开启注解风格的@AspectJ支持；
       5）需要将切面注册为Bean，如“aspect”Bean；
       6）测试代码完全一样。
二、后置返回通知：使用org.aspectj.lang.annotation 包下的@AfterReturning注解声明；
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @AfterReturning(  
- value="切入点表达式或命名切入点",  
- pointcut="切入点表达式或命名切入点",  
- argNames="参数列表参数名",  
- returning="返回值对应参数名")  
value：指定切入点表达式或命名切入点；
pointcut：同样是指定切入点表达式或命名切入点，如果指定了将覆盖value属性指定的，pointcut具有高优先级；
argNames：与Schema方式配置中的同义；
returning：与Schema方式配置中的同义。
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @AfterReturning(  
-     value="execution(* cn.javass..*.sayBefore(..))",  
-     pointcut="execution(* cn.javass..*.sayAfterReturning(..))",  
-     argNames="retVal", returning="retVal")  
- publicvoid afterReturningAdvice(Object retVal) {  
-     System.out.println("===========after returning advice retVal:" + retVal);  
- }  
其中测试代码与Schema方式几乎一样，在此就不演示了，如果需要请参考AopTest.java中的testAnnotationAfterReturningAdvice测试方法。
三、后置异常通知：使用org.aspectj.lang.annotation 包下的@AfterThrowing注解声明；
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @AfterThrowing (  
- value="切入点表达式或命名切入点",  
- pointcut="切入点表达式或命名切入点",  
- argNames="参数列表参数名",  
- throwing="异常对应参数名")  
- 
value：指定切入点表达式或命名切入点；
pointcut：同样是指定切入点表达式或命名切入点，如果指定了将覆盖value属性指定的，pointcut具有高优先级；
argNames：与Schema方式配置中的同义；
throwing：与Schema方式配置中的同义。
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @AfterThrowing(  
-     value="execution(* cn.javass..*.sayAfterThrowing(..))",  
-     argNames="exception", throwing="exception")  
- publicvoid afterThrowingAdvice(Exception exception) {  
-     System.out.println("===========after throwing advice exception:" + exception);  
- }  
其中测试代码与Schema方式几乎一样，在此就不演示了，如果需要请参考AopTest.java中的testAnnotationAfterThrowingAdvice测试方法。
四、后置最终通知：使用org.aspectj.lang.annotation 包下的@After注解声明；
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @After (  
- value="切入点表达式或命名切入点",  
- argNames="参数列表参数名")  
value：指定切入点表达式或命名切入点；
argNames：与Schema方式配置中的同义；
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @After(value="execution(* cn.javass..*.sayAfterFinally(..))")  
- publicvoid afterFinallyAdvice() {  
-     System.out.println("===========after finally advice");  
- }  
其中测试代码与Schema方式几乎一样，在此就不演示了，如果需要请参考AopTest.java中的testAnnotationAfterFinallyAdvice测试方法。
五、环绕通知：使用org.aspectj.lang.annotation 包下的@Around注解声明；
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @Around (  
- value="切入点表达式或命名切入点",  
- argNames="参数列表参数名")  
value：指定切入点表达式或命名切入点；
argNames：与Schema方式配置中的同义；
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @Around(value="execution(* cn.javass..*.sayAround(..))")  
- public Object aroundAdvice(ProceedingJoinPoint pjp) throws Throwable {  
-     System.out.println("===========around before advice");  
-     Object retVal = pjp.proceed(new Object[] {"replace"});  
-     System.out.println("===========around after advice");  
- return retVal;  
- }  
其中测试代码与Schema方式几乎一样，在此就不演示了，如果需要请参考AopTest.java中的annotationAroundAdviceTest测试方法。
### 6.4.5  引入
       @AspectJ风格的引入声明在切面中使用org.aspectj.lang.annotation包下的@DeclareParents声明：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @DeclareParents(  
- value=" AspectJ语法类型表达式",  
- defaultImpl=引入接口的默认实现类)  
- private Interface interface;  
value：匹配需要引入接口的目标对象的AspectJ语法类型表达式；与Schema方式中的types-matching属性同义；
       private Interface interface：指定需要引入的接口；
       defaultImpl：指定引入接口的默认实现类，没有与Schema方式中的delegate-ref属性同义的定义方式；
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2471.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2471.html#)[打印](http://sishuok.com/forum/blogPost/list/2471.html#)
- @DeclareParents(  
-     value="cn.javass..*.IHelloWorldService+", defaultImpl=cn.javass.spring.chapter6.service.impl.IntroductiondService.class)  
- private IIntroductionService introductionService;  
- 
       其中测试代码与Schema方式几乎一样，在此就不演示了，如果需要请参考AopTest.java中的testAnnotationIntroduction测试方法。
原创内容，转载请注明出处【[http://sishuok.com/forum/blogPost/list/0/2471.html](http://sishuok.com/forum/blogPost/list/0/2471.html#7143)】
