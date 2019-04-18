# 基于Schema的AOP - z69183787的专栏 - CSDN博客
2013年12月05日 10:22:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：680
  基于Schema的AOP从Spring2.0之后通过“aop”命名空间来定义切面、切入点及声明通知。
    在Spring配置文件中，所以AOP相关定义必须放在<aop:config>标签下，该标签下可以有<aop:pointcut>、<aop:advisor>、<aop:aspect>标签，配置顺序不可变。
- <aop:pointcut>：用来定义切入点，该切入点可以重用；
- <aop:advisor>：用来定义只有一个通知和一个切入点的切面；
- <aop:aspect>：用来定义切面，该切面可以包含多个切入点和通知，而且标签内部的通知和切入点定义是无序的；和advisor的区别就在此，advisor只包含一个通知和一个切入点。
![](http://sishuok.com/forum/upload/2012/2/21/a384cf881a9a348b1bf2146232744f36__1.JPG)
**6.3.1  声明切面**
    切面就是包含切入点和通知的对象，在Spring容器中将被定义为一个Bean，Schema方式的切面需要一个切面支持Bean，该支持Bean的字段和方法提供了切面的状态和行为信息，并通过配置方式来指定切入点和通知实现。
    切面使用<aop:aspect>标签指定，ref属性用来引用切面支持Bean。
    切面支持Bean“aspectSupportBean”跟普通Bean完全一样使用，切面使用“ref”属性引用它。
### 6.3.2  声明切入点
    切入点在Spring中也是一个Bean，Bean定义方式可以有很三种方式：
1）在<aop:config>标签下使用<aop:pointcut>声明一个切入点Bean，该切入点可以被多个切面使用，对于需要共享使用的切入点最好使用该方式，该切入点使用id属性指定Bean名字，在通知定义时使用pointcut-ref属性通过该id引用切入点，expression属性指定切入点表达式：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:config>  
-  <aop:pointcut id="pointcut" expression="execution(* cn.javass..*.*(..))"/>  
-  <aop:aspect ref="aspectSupportBean">  
-     <aop:before pointcut-ref="pointcut" method="before"/>  
-  </aop:aspect>  
- </aop:config>  
2）在<aop:aspect>标签下使用<aop:pointcut>声明一个切入点Bean，该切入点可以被多个切面使用，但一般该切入点只被该切面使用，当然也可以被其他切面使用，但最好不要那样使用，该切入点使用id属性指定Bean名字，在通知定义时使用pointcut-ref属性通过该id引用切入点，expression属性指定切入点表达式：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:config>  
-  <aop:aspect ref="aspectSupportBean">  
-     <aop:pointcut id=" pointcut" expression="execution(* cn.javass..*.*(..))"/>  
-     <aop:before pointcut-ref="pointcut" method="before"/>  
-  </aop:aspect>  
- </aop:config>  
3）匿名切入点Bean，可以在声明通知时通过pointcut属性指定切入点表达式，该切入点是匿名切入点，只被该通知使用：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:config>  
-  <aop:aspect ref="aspectSupportBean">  
-      <aop:after pointcut="execution(* cn.javass..*.*(..))" method="afterFinallyAdvice"/>  
-  </aop:aspect>  
- </aop:config>  
### 6.3.3  声明通知
基于Schema方式支持前边介绍的5中通知类型：
一、前置通知：在切入点选择的方法之前执行，通过<aop:aspect>标签下的<aop:before>标签声明：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:before pointcut="切入点表达式"  pointcut-ref="切入点Bean引用"
- method="前置通知实现方法名"
- arg-names="前置通知实现方法参数列表参数名字"/>  
pointcut和pointcut-ref：二者选一，指定切入点；
method：指定前置通知实现方法名，如果是多态需要加上参数类型，多个用“，”隔开，如beforeAdvice(java.lang.String)；
arg-names：指定通知实现方法的参数名字，多个用“，”分隔，可选，类似于【3.1.2 构造器注入】中的参数名注入限制：在class文件中没生成变量调试信息是获取不到方法参数名字的，因此只有在类没生成变量调试信息时才需要使用arg-names属性来指定参数名，如arg-names="param"表示通知实现方法的参数列表的第一个参数名字为“param”。
首先在cn.javass.spring.chapter6.service.IhelloWorldService定义一个测试方法：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- publicvoid sayBefore(String param);  
其次在cn.javass.spring.chapter6.service.impl. HelloWorldService定义实现
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Override
- publicvoid sayBefore(String param) {  
-     System.out.println("============say " + param);  
- }  
第三在cn.javass.spring.chapter6.aop. HelloWorldAspect定义通知实现：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- publicvoid beforeAdvice(String param) {  
-     System.out.println("===========before advice param:" + param);  
- }  
最后在chapter6/advice.xml配置文件中进行如下配置：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <bean id="helloWorldService"class="cn.javass.spring.chapter6.service.impl.HelloWorldService"/>  
- <bean id="aspect"class="cn.javass.spring.chapter6.aop.HelloWorldAspect"/>  
- <aop:config>  
-     <aop:aspect ref="aspect">  
-         <aop:before pointcut="execution(* cn.javass..*.sayBefore(..)) and args(param)"
-                            method="beforeAdvice(java.lang.String)"
-                            arg-names="param"/>  
-     </aop:aspect>  
- </aop:config>  
测试代码cn.javass.spring.chapter6.AopTest:
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Test
- publicvoid testSchemaBeforeAdvice(){  
-      System.out.println("======================================");  
-      ApplicationContext ctx = new ClassPathXmlApplicationContext("chapter6/advice.xml");  
-      IHelloWorldService helloworldService = ctx.getBean("helloWorldService", IHelloWorldService.class);  
-      helloworldService.sayBefore("before");  
-     System.out.println("======================================");  
- }  
将输入：
==========================================
===========before advice param:before
============say before
==========================================

分析一下吧：
1）切入点匹配：在配置中使用“execution(* cn.javass..*.sayBefore(..)) ”匹配目标方法sayBefore，且使用“args(param)”匹配目标方法只有一个参数且传入的参数类型为通知实现方法中同名的参数类型；
2）目标方法定义：使用method=" beforeAdvice(java.lang.String) "指定前置通知实现方法，且该通知有一个参数类型为java.lang.String参数；
3）目标方法参数命名：其中使用arg-names=" param "指定通知实现方法参数名为“param”，切入点中使用“args(param)”匹配的目标方法参数将自动传递给通知实现方法同名参数。
二、后置返回通知：在切入点选择的方法正常返回时执行，通过<aop:aspect>标签下的<aop:after-returning>标签声明：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:after-returning pointcut="切入点表达式"  pointcut-ref="切入点Bean引用"
-     method="后置返回通知实现方法名"
-     arg-names="后置返回通知实现方法参数列表参数名字"
-     returning="返回值对应的后置返回通知实现方法参数名"
- />  
pointcut和pointcut-ref：同前置通知同义；
method：同前置通知同义；
arg-names：同前置通知同义；
returning：定义一个名字，该名字用于匹配通知实现方法的一个参数名，当目标方法执行正常返回后，将把目标方法返回值传给通知方法；returning限定了只有目标方法返回值匹配与通知方法相应参数类型时才能执行后置返回通知，否则不执行，对于returning对应的通知方法参数为Object类型将匹配任何目标返回值。
首先在cn.javass.spring.chapter6.service.IhelloWorldService定义一个测试方法：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- publicboolean sayAfterReturning();  
- 
其次在cn.javass.spring.chapter6.service.impl. HelloWorldService定义实现
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Override
- publicboolean sayAfterReturning() {  
-     System.out.println("============after returning");  
- returntrue;  
- }  
第三在cn.javass.spring.chapter6.aop. HelloWorldAspect定义通知实现：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- publicvoid afterReturningAdvice(Object retVal) {  
-     System.out.println("===========after returning advice retVal:" + retVal);  
- }  
最后在chapter6/advice.xml配置文件中接着前置通知配置的例子添加如下配置：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:after-returning pointcut="execution(* cn.javass..*.sayAfterReturning(..))"
-                                 method="afterReturningAdvice"
-                                arg-names="retVal"
-                                returning="retVal"/>  
测试代码cn.javass.spring.chapter6.AopTest:
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Test
- publicvoid testSchemaAfterReturningAdvice() {  
-     System.out.println("======================================");  
-     ApplicationContext ctx = new ClassPathXmlApplicationContext("chapter6/advice.xml");  
-     IHelloWorldService helloworldService = ctx.getBean("helloWorldService", IHelloWorldService.class);  
-     helloworldService.sayAfterReturning();      
-     System.out.println("======================================");  
- }  
将输入：
======================================
============after returning
===========after returning advice retVal:true
======================================

分析一下吧：
1）切入点匹配：在配置中使用“execution(* cn.javass..*.sayAfterReturning(..)) ”匹配目标方法sayAfterReturning，该方法返回true；
2）目标方法定义：使用method="afterReturningAdvice"指定后置返回通知实现方法；
3）目标方法参数命名：其中使用arg-names="retVal"指定通知实现方法参数名为“retVal”；
4）返回值命名：returning="retVal"用于将目标返回值赋值给通知实现方法参数名为“retVal”的参数上。
三、后置异常通知：在切入点选择的方法抛出异常时执行，通过<aop:aspect>标签下的<aop:after-throwing>标签声明：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:after-throwing pointcut="切入点表达式"  pointcut-ref="切入点Bean引用"
-                                 method="后置异常通知实现方法名"
-                                 arg-names="后置异常通知实现方法参数列表参数名字"
-                                 throwing="将抛出的异常赋值给的通知实现方法参数名"/>  
pointcut和pointcut-ref：同前置通知同义；
method：同前置通知同义；
arg-names：同前置通知同义；
throwing：定义一个名字，该名字用于匹配通知实现方法的一个参数名，当目标方法抛出异常返回后，将把目标方法抛出的异常传给通知方法；throwing限定了只有目标方法抛出的异常匹配与通知方法相应参数异常类型时才能执行后置异常通知，否则不执行，对于throwing对应的通知方法参数为Throwable类型将匹配任何异常。
首先在cn.javass.spring.chapter6.service.IhelloWorldService定义一个测试方法：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- publicvoid sayAfterThrowing();  
- 
其次在cn.javass.spring.chapter6.service.impl. HelloWorldService定义实现
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Override
- publicvoid sayAfterThrowing() {  
-     System.out.println("============before throwing");  
- thrownew RuntimeException();  
- }  
第三在cn.javass.spring.chapter6.aop. HelloWorldAspect定义通知实现：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- publicvoid afterThrowingAdvice(Exception exception) {  
-   System.out.println("===========after throwing advice exception:" + exception);  
- }  
最后在chapter6/advice.xml配置文件中接着前置通知配置的例子添加如下配置：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:after-throwing pointcut="execution(* cn.javass..*.sayAfterThrowing(..))"
-                                 method="afterThrowingAdvice"
-                                 arg-names="exception"
-                                 throwing="exception"/>  
测试代码cn.javass.spring.chapter6.AopTest:
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Test(expected = RuntimeException.class)  
- publicvoid testSchemaAfterThrowingAdvice() {  
-     System.out.println("======================================");  
-     ApplicationContext ctx = new ClassPathXmlApplicationContext("chapter6/advice.xml");  
-     IHelloWorldService helloworldService = ctx.getBean("helloWorldService", IHelloWorldService.class);  
-     helloworldService.sayAfterThrowing();  
-     System.out.println("======================================");  
- }  
将输入：
======================================
============before throwing
===========after throwing advice exception:java.lang.RuntimeException
======================================

分析一下吧：
1）切入点匹配：在配置中使用“execution(* cn.javass..*.sayAfterThrowing(..))”匹配目标方法sayAfterThrowing，该方法将抛出RuntimeException异常；
2）目标方法定义：使用method="afterThrowingAdvice"指定后置异常通知实现方法；
3）目标方法参数命名：其中使用arg-names="exception"指定通知实现方法参数名为“exception”；
4）异常命名：returning="exception"用于将目标方法抛出的异常赋值给通知实现方法参数名为“exception”的参数上。
四、后置最终通知：在切入点选择的方法返回时执行，不管是正常返回还是抛出异常都执行，通过<aop:aspect>标签下的<aop:after >标签声明：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:after pointcut="切入点表达式"  pointcut-ref="切入点Bean引用"
-                   method="后置最终通知实现方法名"
-                   arg-names="后置最终通知实现方法参数列表参数名字"/>  
pointcut和pointcut-ref：同前置通知同义；
method：同前置通知同义；
arg-names：同前置通知同义；
首先在cn.javass.spring.chapter6.service.IhelloWorldService定义一个测试方法：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- publicboolean sayAfterFinally();  
其次在cn.javass.spring.chapter6.service.impl. HelloWorldService定义实现
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Override
- publicboolean sayAfterFinally() {  
-         System.out.println("============before finally");  
- thrownew RuntimeException();  
- }  
第三在cn.javass.spring.chapter6.aop. HelloWorldAspect定义通知实现：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- publicvoid afterFinallyAdvice() {  
-         System.out.println("===========after finally advice");  
- }  
最后在chapter6/advice.xml配置文件中接着前置通知配置的例子添加如下配置：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:after pointcut="execution(* cn.javass..*.sayAfterFinally(..))"
-          method="afterFinallyAdvice"/>  
测试代码cn.javass.spring.chapter6.AopTest:
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Test(expected = RuntimeException.class)  
- publicvoid testSchemaAfterFinallyAdvice() {  
-     System.out.println("======================================");  
-     ApplicationContext ctx = new ClassPathXmlApplicationContext("chapter6/advice.xml");  
-     IHelloWorldService helloworldService = ctx.getBean("helloWorldService", IHelloWorldService.class);  
-     helloworldService.sayAfterFinally();  
-     System.out.println("======================================");  
- }  
将输入：
======================================
============before finally
===========after finally advice
======================================

分析一下吧：
1）切入点匹配：在配置中使用“execution(* cn.javass..*.sayAfterFinally(..))”匹配目标方法sayAfterFinally，该方法将抛出RuntimeException异常；
2）目标方法定义：使用method=" afterFinallyAdvice "指定后置最终通知实现方法。
五、环绕通知：环绕着在切入点选择的连接点处的方法所执行的通知，环绕通知非常强大，可以决定目标方法是否执行，什么时候执行，执行时是否需要替换方法参数，执行完毕是否需要替换返回值，可通过<aop:aspect>标签下的<aop:around >标签声明：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:around pointcut="切入点表达式"  pointcut-ref="切入点Bean引用"
-                      method="后置最终通知实现方法名"
-                      arg-names="后置最终通知实现方法参数列表参数名字"/>  
pointcut和pointcut-ref：同前置通知同义；
method：同前置通知同义；
arg-names：同前置通知同义；
环绕通知第一个参数必须是org.aspectj.lang.ProceedingJoinPoint类型，在通知实现方法内部使用ProceedingJoinPoint的proceed()方法使目标方法执行，proceed 方法可以传入可选的Object[]数组，该数组的值将被作为目标方法执行时的参数。
首先在cn.javass.spring.chapter6.service.IhelloWorldService定义一个测试方法：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- publicvoid sayAround(String param);  
其次在cn.javass.spring.chapter6.service.impl. HelloWorldService定义实现
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Override
- publicvoid sayAround(String param) {  
-    System.out.println("============around param:" + param);  
- }  
第三在cn.javass.spring.chapter6.aop. HelloWorldAspect定义通知实现：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- public Object aroundAdvice(ProceedingJoinPoint pjp) throws Throwable {  
-     System.out.println("===========around before advice");  
-     Object retVal = pjp.proceed(new Object[] {"replace"});  
-     System.out.println("===========around after advice");  
- return retVal;  
- }  
最后在chapter6/advice.xml配置文件中接着前置通知配置的例子添加如下配置：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:around pointcut="execution(* cn.javass..*.sayAround(..))"
-            method="aroundAdvice"/>  
测试代码cn.javass.spring.chapter6.AopTest:
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Test
- publicvoid testSchemaAroundAdvice() {  
-     System.out.println("======================================");  
-     ApplicationContext ctx = new ClassPathXmlApplicationContext("chapter6/advice.xml");  
-     IHelloWorldService helloworldService =  
-     ctx.getBean("helloWorldService", IHelloWorldService.class);  
-     helloworldService.sayAround("haha");  
-     System.out.println("======================================");  
- }  
将输入：
======================================
===========around before advice
============around param:replace
===========around after advice
======================================

分析一下吧：
1）切入点匹配：在配置中使用“execution(* cn.javass..*.sayAround(..))”匹配目标方法sayAround；
2）目标方法定义：使用method="aroundAdvice"指定环绕通知实现方法，在该实现中，第一个方法参数为pjp，类型为ProceedingJoinPoint，其中“Object retVal = pjp.proceed(new Object[] {"replace"});”，用于执行目标方法，且目标方法参数被“new Object[] {"replace"}”替换，最后返回“retVal
 ”返回值。
3）测试：我们使用“helloworldService.sayAround("haha");”传入参数为“haha”，但最终输出为“replace”，说明参数被替换了。
### 6.3.4  引入
    Spring引入允许为目标对象引入新的接口，通过在< aop:aspect>标签内使用< aop:declare-parents>标签进行引入，定义方式如下：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:declare-parents  
-           types-matching="AspectJ语法类型表达式"
-           implement-interface=引入的接口"               
- default-impl="引入接口的默认实现"
-           delegate-ref="引入接口的默认实现Bean引用"/>  
types-matching：匹配需要引入接口的目标对象的AspectJ语法类型表达式；
implement-interface：定义需要引入的接口；
default-impl和delegate-ref：定义引入接口的默认实现，二者选一，default-impl是接口的默认实现类全限定名，而delegate-ref是默认的实现的委托Bean名；
接下来让我们练习一下吧：
    首先定义引入的接口及默认实现：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- package cn.javass.spring.chapter6.service;  
- publicinterface IIntroductionService {  
- publicvoid induct();  
- }  
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- package cn.javass.spring.chapter6.service.impl;  
- import cn.javass.spring.chapter6.service.IIntroductionService;  
- publicclass IntroductiondService implements IIntroductionService {  
- @Override
- publicvoid induct() {  
-         System.out.println("=========introduction");  
-     }  
- }  
其次在chapter6/advice.xml配置文件中接着前置通知配置的例子添加如下配置：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:declare-parents  
-     types-matching="cn.javass..*.IHelloWorldService+"
-     implement-interface="cn.javass.spring.chapter6.service.IIntroductionService"
- default-impl="cn.javass.spring.chapter6.service.impl.IntroductiondService"/>  
- 
最后测试一下吧，测试代码cn.javass.spring.chapter6.AopTest：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Test
- publicvoid testSchemaIntroduction() {  
-     System.out.println("======================================");  
-     ApplicationContext ctx = new ClassPathXmlApplicationContext("chapter6/advice.xml");  
-     IIntroductionService introductionService =  
-     ctx.getBean("helloWorldService", IIntroductionService.class);  
-     introductionService.induct();  
-     System.out.println("======================================");  
- }  
- 
将输入：
======================================
=========introduction
======================================
分析一下吧：
1）目标对象类型匹配：使用types-matching="cn.javass..*.IHelloWorldService+"匹配IHelloWorldService接口的子类型，如HelloWorldService实现；
2）引入接口定义：通过implement-interface属性表示引入的接口，如“cn.javass.spring.chapter6.service.IIntroductionService”。
3）引入接口的实现：通过default-impl属性指定，如“cn.javass.spring.chapter6.service.impl.IntroductiondService”，也可以使用“delegate-ref”来指定实现的Bean。
4）获取引入接口：如使用“ctx.getBean("helloWorldService", IIntroductionService.class);”可直接获取到引入的接口。
### 6.3.5 Advisor
Advisor表示只有一个通知和一个切入点的切面，由于Spring AOP都是基于AOP联盟的拦截器模型的环绕通知的，所以引入Advisor来支持各种通知类型（如前置通知等5种），Advisor概念来自于Spring1.2对AOP的支持，在AspectJ中没有相应的概念对应。
Advisor可以使用<aop:config>标签下的<aop:advisor>标签定义：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:advisor pointcut="切入点表达式" pointcut-ref="切入点Bean引用"
-                      advice-ref="通知API实现引用"/>  
pointcut和pointcut-ref：二者选一，指定切入点表达式；
advice-ref：引用通知API实现Bean，如前置通知接口为MethodBeforeAdvice；
接下来让我们看一下示例吧：
首先在cn.javass.spring.chapter6.service.IhelloWorldService定义一个测试方法：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- publicvoid sayAdvisorBefore(String param);  
其次在cn.javass.spring.chapter6.service.impl. HelloWorldService定义实现
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Override
- publicvoid sayAdvisorBefore(String param) {  
-     System.out.println("============say " + param);  
- }  
第三定义前置通知API实现：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- package cn.javass.spring.chapter6.aop;  
- import java.lang.reflect.Method;  
- import org.springframework.aop.MethodBeforeAdvice;  
- publicclass BeforeAdviceImpl implements MethodBeforeAdvice {  
- @Override
- publicvoid before(Method method, Object[] args, Object target) throws Throwable {  
-         System.out.println("===========before advice");  
-     }  
- }  
在chapter6/advice.xml配置文件中先添加通知实现Bean定义：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- 
- <bean id="beforeAdvice"class="cn.javass.spring.chapter6.aop.BeforeAdviceImpl"/>  
- 
然后在<aop:config>标签下，添加Advisor定义，添加时注意顺序：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- <aop:advisor pointcut="execution(* cn.javass..*.sayAdvisorBefore(..))"
-                      advice-ref="beforeAdvice"/>  
- 
测试代码cn.javass.spring.chapter6.AopTest:
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2469.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2469.html#)[打印](http://sishuok.com/forum/blogPost/list/2469.html#)
- @Test
- publicvoid testSchemaAdvisor() {  
-    System.out.println("======================================");  
-    ApplicationContext ctx = new ClassPathXmlApplicationContext("chapter6/advice.xml");  
-    IHelloWorldService helloworldService =  
-    ctx.getBean("helloWorldService", IHelloWorldService.class);  
-    helloworldService.sayAdvisorBefore("haha");  
-    System.out.println("======================================");  
- }   
将输入：
======================================
===========before advice
============say haha
======================================

在此我们只介绍了前置通知API，其他类型的在后边章节介绍。
    不推荐使用Advisor，除了在进行事务控制的情况下，其他情况一般不推荐使用该方式，该方式属于侵入式设计，必须实现通知API。
原创内容 转自请注明出处【[http://sishuok.com/forum/blogPost/list/2469.html](http://sishuok.com/forum/blogPost/list/2469.html)】
