# Spring中AOP的实现 （二） - z69183787的专栏 - CSDN博客
2012年10月22日 12:37:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：761
在Spring 2.0中，除了传统的通过实现AOP
 AIP的方式来实现Advice之外，还提供了两种更加简便的方式来实现Advice：1）基于XML
 Schema的设置；2）基于Annotation的支持，采用这两种方式，Advice将不用实现特定的接口。现在让我们来看看如何使用这两种方式来分别实现Before
 Advice、After Advice、Around
 Advice、Throwing Advice。
一、Before Advice：基于XML
 Schema
当基于XML Schema实现Before
 Advice时，你的Advice类不用实现org.springframework.aop.MethodBeforeAdvice接口，例如：
java 代码
- package com.savage.aop;  
- 
- import org.aspectj.lang.JoinPoint;  
- 
- publicclass LogBeforeAdvice {  
- publicvoid before(JoinPoint joinPoint) {  
-         System.out.println("Logging before " + joinPoint.getSignature().getName());  
-     }  
- }  
          before方法是在目标对象上的方法被执行前要执行的方法，before方法中的JoinPoint参数是可选项，你可以根据需要决定是否需要JoinPoint参数，通过JoinPoint对象，你可以获得目标对象（getTarget()）、目标方法上的参数（getArgs()）等信息。
     然后在XML中为目标对象指定LogBeforeAdvice代理：
xml 代码
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-     http://www.springframework.org/schema/beans/spring-beans-2.0.xsd  
-     http://www.springframework.org/schema/aop  
-     http://www.springframework.org/schema/aop/spring-aop-2.0.xsd">
- <beanid="messageSender"class="com.savage.aop.HttpMessageSender"></bean>
- 
- <beanid="logBeforeAdvice"class="com.savage.aop.LogBeforeAdvice"></bean>
- 
- <aop:config>
- <aop:aspectid="logBefore"ref="logBeforeAdvice">
- <aop:beforepointcut="execution(* com.savage.aop.MessageSender.*(..))"
-                         method="before"/>
- </aop:aspect>
- </aop:config>
- </beans>
     如上所示，在Spring 2.0中要使用基于XML Sechma声明AOP的方式，需要在XML中加入aop的名称空间。当基于XML
 Sechma实现AOP时，所有的AOP都是在<aop:config></aop:config>标签中声明的，<aop:aspect></aop:aspect>用于定义Advice实例。<aop:before></aop:before>表示当前实例用于实现Before
 Advice；pointcut属性用于指定pointcut表示式，上面的例子表示此Advice将应用于com.savage.aop.MessageSender接口中的任何方法；method属性表示Advice上要调用的方法。
     现在调用任何MessageSender接口上的方法之前都会执行LogBeforeAdvice的before方法，例如：
java 代码
- package com.savage.aop;  
- 
- import org.springframework.context.ApplicationContext;  
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- 
- publicclass AdviceDemo {  
- publicstaticvoid main(String[] args) {  
-         ApplicationContext context = 
-                            new ClassPathXmlApplicationContext("beans-config.xml");  
-         MessageSender sender = (MessageSender)context.getBean("messageSender");  
-         sender.sendMessage("message");  
-     }  
- }  
     二、Before Advice：基于Annotation
使用Annotation来实现Advice，在XML文件上的定义要比基于XML Sechema的方法要简便的多，但在实现Before Advice类时，则需要使用到@Aspect、@Before标识，并需要引入org.aspectj.lang.annotation包中的类。还以LogBeforeAdvice为例，LogBeforeAdvice类需要改为：
java 代码
- package com.savage.aop;  
- 
- import org.aspectj.lang.JoinPoint;  
- import org.aspectj.lang.annotation.Aspect;  
- import org.aspectj.lang.annotation.Before;  
- 
- @Aspect
- publicclass LogBeforeAdvice {  
- @Before("execution(* com.savage.aop.MessageSender.*(..))")  
- publicvoid before(JoinPoint joinPoint) {  
-         System.out.println("Logging before " + joinPoint.getSignature().getName());  
-     }  
- }  
     如上所示，通过@Aspect将一个类声明为Aspect类，通过@Before将方法声明Before Advice，方法中的JoinPoint同样是可选的。然后在XML文件中做如下定义：
xml 代码
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-     http://www.springframework.org/schema/beans/spring-beans-2.0.xsd  
-     http://www.springframework.org/schema/aop  
-     http://www.springframework.org/schema/aop/spring-aop-2.0.xsd">
- <beanid="messageSender"class="com.savage.aop.HttpMessageSender"></bean>
- 
- <beanid="logBeforeAdvice"class="com.savage.aop.LogBeforeAdvice"></bean>
- 
- <aop:aspectj-autoproxy/>
- </beans>
     所有基于Annotation实现的Advice，在XML文件中都只要使用<aop:aspectj-autoproxy></aop:aspectj-autoproxy>进行设置就可以了，非常简单。
     三、After Advice：基于XML Sechma
和Before Advice一样，基于XML Sechma实现After Returning Advice时，不再需要org.springframework.aop.AfterReturningAdvice接口：
java 代码
- package com.savage.aop;  
- 
- import org.aspectj.lang.JoinPoint;  
- 
- publicclass LogAfterReturningAdvice {  
- publicvoid afterReturning(JoinPoint joinPoint) {  
-         System.out.println("Logging after " + joinPoint.getSignature().getName());  
-     }  
- }  
然后在XML中做如下设置：
xml 代码
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-     http://www.springframework.org/schema/beans/spring-beans-2.0.xsd  
-     http://www.springframework.org/schema/aop  
-     http://www.springframework.org/schema/aop/spring-aop-2.0.xsd">
- <beanid="messageSender"class="com.savage.aop.HttpMessageSender"></bean>
- 
- <beanid="logAfterReturningAdvice"
-           class="com.savage.aop.LogAfterReturningAdvice"></bean>
- 
- <aop:config>
- <aop:aspectid="logAfterReturning"ref="logAfterReturningAdvice">
- <aop:after-returning
- pointcut="execution(* com.savage.aop.MessageSender.*(..))"
- method="logAfterReturning"/>
- </aop:aspect>
- </aop:config>
- </beans>
    四、After Advice：基于Annotation
和Before Advice相似，使用@AfterReturning来表示After Returning Advice：
java 代码
- package com.savage.aop;  
- 
- import org.aspectj.lang.JoinPoint;  
- import org.aspectj.lang.annotation.Aspect;  
- import org.aspectj.lang.annotation.AfterReturning;  
- 
- @Aspect
- publicclass AfterReturningAdvice {  
- @AfterReturning(pointcut="execution(* com.savage.aop.MessageSender.*(..))", 
-                     returning="retVal")  
- publicvoid afterReturning(JoinPoint joinPoint, Object retVal) {  
-         System.out.println("Logging after " + joinPoint.getSignature().getName());  
-     }  
- }  
     这里和Before Advice有点不同的是，在定义Poincut表示式时，多了一个returning属性，用于指定目标方法执行完后的返回值。
XML文件中的设置与LogBeforeAdvice的相似（将logBeforeAdvice的定义改为logAfterReturning的定义），不再列举。
     五、Around Advice：基于XML Sechma
在Spring 2.0中，Around Advice不用实现org.aoplliance.intercept.MethodInterceptor接口，但Advice的方法必须返回对象，并且必须定义一个ProceedingJoinPoint参数，例如：
java 代码
- package com.savage.aop;  
- 
- import org.aspectj.lang.ProceedingJoinPoint;  
- 
- publicclass LogAroundAdvice {  
- publicvoid invoke(ProceedingJoinPoint joinPoint) {  
-         System.out.println("Logging before " + joinPoint.getSignature().getName());  
-         Object retVal = joinPoint.proceed();  
-         System.out.println("Logging after " + joinPoint.getSignature().getName());  
- return retVal;  
-     }  
- }  
XML中的设置如下：
xml 代码
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-     http://www.springframework.org/schema/beans/spring-beans-2.0.xsd  
-     http://www.springframework.org/schema/aop  
-     http://www.springframework.org/schema/aop/spring-aop-2.0.xsd">
- <beanid="messageSender"class="com.savage.aop.HttpMessageSender"></bean>
- 
- <beanid="logAroundAdvice"class="com.savage.aop.LogAroundAdvice"></bean>
- 
- <aop:config>
- <aop:aspectid="logAround"ref="logAroundAdvice">
- <aop:around
- pointcut="execution(* com.savage.aop.MessageSender.*(..))"
- method="invoke"/>
- </aop:aspect>
- </aop:config>
- </beans>
     六、Around Advice：基于Annotation
和Before Advice相似，使用@Around来表示Around Advice：
java 代码
- package com.savage.aop;  
- 
- import org.aspectj.lang.ProceedingJoinPoint;  
- import org.aspectj.lang.annotation.Aspect;  
- import org.aspectj.lang.annotation.Around;  
- 
- @Aspect
- publicclass AfterReturningAdvice {  
- @Around("execution(* com.savage.aop.MessageSender.*(..))")  
- publicvoid invoke(ProceedingJoinPoint joinPoint) {  
-         System.out.println("Logging before " + joinPoint.getSignature().getName());  
-         Object retVal = joinPoint.proceed();  
-         System.out.println("Logging after " + joinPoint.getSignature().getName());  
- return retVal;  
-     }  
- }  
XML文件中的设置与LogBeforeAdvice的相似（将logBeforeAdvice的定义改为logAroundAdvice的定义），不再列举。
     七、Throw Advice：基于XML Sechma
在Spring 2.0中，Throw Advice不用实现org.springframework.aop.ThrowsAdvice接口，但Advice的方法必须定义Throwable（或其子类）参数，例如：
java 代码
- package com.savage.aop;  
- 
- import org.aspectj.lang.JoinPoint;  
- 
- publicclass LogThrowingAdvice {  
- publicvoid afterThrowing (JoinPoint joinPoint, Throwable throwable) {  
-         System.out.println("Logging when throwing " + joinPoint.getSignature().getName());  
-     }  
- }  
在XML的设置如下：
xml 代码
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-     http://www.springframework.org/schema/beans/spring-beans-2.0.xsd  
-     http://www.springframework.org/schema/aop  
-     http://www.springframework.org/schema/aop/spring-aop-2.0.xsd">
- <beanid="messageSender"class="com.savage.aop.HttpMessageSender"></bean>
- 
- <beanid="logThrowingAdvice"class="com.savage.aop.LogThrowingAdvice"></bean>
- 
- <aop:config>
- <aop:aspectid="logThrowing"ref="logThrowingAdvice">
- <aop:after-throwing
- pointcut="execution(* com.savage.aop.MessageSender.*(..))"
- throwing="throwable"
- method="afterThrowing"/>
- </aop:aspect>
- </aop:config>
- </beans>
     在<aop:after-throwing></aop:after-throwing>中必须定义throwing属性，指定方法中的throwable参数。Spring将根据异常类型决定是否调用afterThrowing方法。
     八、Throw Advice：基于Annotation
java 代码
- package com.savage.aop;  
- 
- import org.aspectj.lang.JoinPoint;  
- import org.aspectj.lang.annotation.Aspect;  
- import org.aspectj.lang.annotation.AfterThrowing;  
- 
- @Aspect
- publicclass AfterThrowingAdvice {  
- @AfterThrowing(pointcut="execution(* com.savage.aop.MessageSender.*(..))", 
-                    throwing="throwable")  
- publicvoid afterThrowing(JoinPoint joinPoint, Throwable throwable) {  
-         System.out.println("Logging when throwing "
-                            + joinPoint.getSignature().getName());  
-     }  
- }  
       XML文件中的设置与LogBeforeAdvice的相似（将logBeforeAdvice的定义改为logThrowingAdvice的定义），不再列举。
            
