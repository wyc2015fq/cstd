# Spring中AOP的实现 （三） - z69183787的专栏 - CSDN博客
2012年10月22日 12:37:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：872
在Spring 2.0中，Pointcut的定义包括两个部分：Pointcut表示式(expression)和Pointcut签名(signature)。让我们先看看execution表示式的格式：
java 代码
- execution(modifier-pattern?
-           ret-type-pattern
-           declaring-type-pattern?
-           name-pattern(param-pattern)
- throws-pattern?)
括号中各个pattern分别表示修饰符匹配（modifier-pattern?）、返回值匹配（ret-type-pattern）、类路径匹配（declaring-type-pattern?）、方法名匹配（name-pattern）、参数匹配（(param-pattern)）、异常类型匹配（throws-pattern?），其中后面跟着“?”的是可选项。
在各个pattern中可以使用“*”来表示匹配所有。在(param-pattern)中，可以指定具体的参数类型，多个参数间用“,”隔开，各个也可以用“*”来表示匹配任意类型的参数，如(String)表示匹配一个String参数的方法；(*,String)表示匹配有两个参数的方法，第一个参数可以是任意类型，而第二个参数是String类型；可以用(..)表示零个或多个任意参数。
现在来看看几个例子：
1）execution(*
 *(..))
表示匹配所有方法
2）execution(public
 * com. savage.service.UserService.*(..))
表示匹配com.savage.server.UserService中所有的公有方法
3）execution(*
 com.savage.server..*.*(..))
表示匹配com.savage.server包及其子包下的所有方法
除了execution表示式外，还有within、this、target、args等Pointcut表示式。一个Pointcut定义由Pointcut表示式和Pointcut签名组成，例如：
java 代码
- //Pointcut表示式
- @Pointcut("execution(* com.savage.aop.MessageSender.*(..))")
- //Point签名
- privatevoid log(){}                             
然后要使用所定义的Pointcut时，可以指定Pointcut签名，如
java 代码
- @Before("og()")
上面的定义等同与：
java 代码
- @Before("execution(* com.savage.aop.MessageSender.*(..))")
Pointcut定义时，还可以使用&&、||、!运算，如：
java 代码
- @Pointcut("execution(* com.savage.aop.MessageSender.*(..))")
- privatevoid logSender(){}
- 
- @Pointcut("execution(* com.savage.aop.MessageReceiver.*(..))")
- privatevoid logReceiver(){}
- 
- @Pointcut("logSender() || logReceiver()")
- privatevoid logMessage(){}
这个例子中，logMessage()将匹配任何MessageSender和MessageReceiver中的任何方法。
还可以将一些公用的Pointcut放到一个类中，以供整个应用程序使用，如：
java 代码
- package com.savage.aop;
- 
- import org.aspectj.lang.annotation.*;
- 
- publicclass Pointcuts {
- @Pointcut("execution(* *Message(..))")
- publicvoid logMessage(){}
- 
- @Pointcut("execution(* *Attachment(..))")
- publicvoid logAttachment(){}
- 
- @Pointcut("execution(* *Service.*(..))")
- publicvoid auth(){}
- }
在使用这些Pointcut时，指定完整的类名加上Pointcut签名就可以了，如：
java 代码
- package com.savage.aop;
- 
- import org.aspectj.lang.JoinPoint;
- import org.aspectj.lang.annotation.*;
- 
- @Aspect
- publicclass LogBeforeAdvice {
- @Before("com.sagage.aop.Pointcuts.logMessage()")
- publicvoid before(JoinPoint joinPoint) {
-       System.out.println("Logging before " + joinPoint.getSignature().getName());
-    }
- }
当基于XML Sechma实现Advice时，如果Pointcut需要被重用，可以使用<aop:pointcut></aop:pointcut>来声明Pointcut，然后在需要使用这个Pointcut的地方，用pointcut-ref引用就行了，如：
xml 代码
- <aop:config>
- <aop:pointcutid="log"
- expression="execution(* com.savage.simplespring.bean.MessageSender.*(..))"/>
- <aop:aspectid="logging"ref="logBeforeAdvice">
- <aop:beforepointcut-ref="log"method="before"/>
- <aop:after-returningpointcut-ref="log"method="afterReturning"/>
- </aop:aspect>
- </aop:config>
