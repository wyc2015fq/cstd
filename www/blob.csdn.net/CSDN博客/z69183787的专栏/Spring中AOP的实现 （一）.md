# Spring中AOP的实现 （一） - z69183787的专栏 - CSDN博客
2012年10月22日 12:34:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1012标签：[spring																[aop																[object																[encoding																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=aop&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[Spring-Aop & 代理](https://blog.csdn.net/z69183787/article/category/2175403)
在Spring1.2或之前的版本中，实现AOP的传统方式就是通过实现Spring的AOP
 API来定义Advice，并设置代理对象。Spring根据Adivce加入到业务流程的时机的不同，提供了四种不同的Advice：Before Advice、After Advice、Around Advice、Throw Advice。
1、Before Advice
顾名思义，Before Advice会在目标对象的方法执行之前被调用，您可以通过实现org.springframework.aop.MethodBeforeAdvice接口来实现Before Advice的逻辑，接口定义如下：
java 代码
- package org.springframework.aop;
- 
- publicinterface MethodBeforeAdvice extends BeforeAdvice
 {
- void before(Method method, Object[] args, Object target) throws Throwable;
- }
其中BeforeAdvice继承自Adivce接口，这两者都是标签接口，并没有定义任何具体的方法。before方法会在目标对象的指定方法执行之前被执行，在before方法种，你可以取得指定方法的Method实例、参数列表和目标对象，在before方法执行完后，目标对象上的方法将会执行，除非在before方法种抛出异常。
下面通过例子来说明Before Advice的使用方法。首先定义目标对象所要实现的接口：
java 代码
- package com.savage.aop
- 
- publicinterface MessageSender {
- void send(String message);
- }
接着实现MessageSender接口：
java 代码
- package com.savage.aop;
- 
- publicclass HttpMessageSender implements MessageSender
 {
- publicvoid send(String message) {
- System.out.println("Send Message[" + message + "] by http.");
- }
- }
OK，我们的业务代码实现完了，现在如果要在不改变我们的业务代码的前提下，在执行业务代码前要记录一些日志，这时就可以通过实现MethodBeforeAdvice接口来实现，如：
java 代码
- package com.savage.aop;
- 
- import java.lang.reflect.Method;
- 
- import org.springframework.aop.framework.MethodBeforeAdvice;
- 
- publicclass LogBeforeAdvice implements MethodAdvice
 {
- publicvoid before(Method method, Object[] args, Object target) throws Throwable
 {
-       System.out.println("Log before " + method + " by LogBeforeAdvice.");
-    }
- }
然后再在XML进行如下定义：
xml 代码
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans    
- http://www.springframework.org/schema/beans/spring-beans-2.0.xsd">
- 
- <beanid="messageSenderImpl"class="com.savage.aop.HttpMessageSender"></bean>
- 
- <beanid="logBeforeAdvice"class="com.savage.aop.LogBeforeAdvice"></bean>
- 
- <beanid="messageSender"class="org.springframework.aop.framework.ProxyFactoryBean">
- <propertyname="proxyInterfaces"value="com.savage.aop.MessageSender"/>
- <propertyname="target"ref="messageSenderImpl"/>
- <propertyname="interceptorNames">
- <list>
- <value>logBeforeAdvice</value>
- </list>
- </property>
- </bean>
- </beans>
这样我们就为MessageSender对象指定了Before Advice对象。在这里，我们分别定义了一个MessageSender对象（messageSenderImpl）和一个Before Advice对象（logBeforeAdvice），并定义了一个org.springframework.aop.framework.ProxyFactoryBean对象（messageSender），FactoryBean或ApplicationContext将使用ProxyFactoryBean来建立代理对象，在这里就是messageSenderImpl建立代理对象。在ProxyFactoryBean的定义中，proxyInterfaces属性指定了要代理的接口；target指定了要建立代理的目标对象；interceptorNames则指定了应用与指定接口上的Advices对象列表，spring将根据列表中定义的顺序在执行目标对象的方法前、后执行Advice中定义的方法。
现在我们写一个程序来验证下：
java 代码
- package com.savage.aop;
- 
- import org.springframework.context.ApplicationContext;
- import org.springframework.context.support.ClassPathXmlApplication;
- 
- publicclass AdviceDemo {
- publicvoid main(String[] args) {
-       ApplicationContext context = new ClassPathXmlApplicationContext("beans-config.xml");
-       MessageSender sender = (MessageSender)context.getBean("messageSender");
-       sender.send("message");
-   }
- }
执行结果：
***Log before public abstract void com.savage.simplespring.bean.MessageSender.send(java.lang.String) by LogBeforeAdvice.Send Message[message] by http.***
正如你所看到的，在执行MessageSender的send方法前先执行了LogBeforeAdvice的方法！在这个例子中，记录日志的代码并没有横切到我们的业务代码中，LogBeforeAdvice和HttpMessageSender彼此不知道对方的存在，而且我们的应用程序AdviceDemo对LogBeforeAdvice的存在也是一无所知。假如有一天我们的应用程序不需要再业务代码执行前记录日志了，只需要修改XML文件中的定义，而不用更改AdviceDemo的代码：
xml 代码
- <beanid="messageSender"class="com.savage.aop.HttpMessageSender"><!---->bean>
2、After Advice
After Advice会在目标对象的方法执行完后执行，你可以通过实现org.springframework.aop.AfterReturingAdvice接口来实现After Advice的逻辑，AfterReturingAdvice接口定义如下：
java 代码
- package org.springframework.aop;
- 
- publicinterface AfterReturningAdvice {
- void afterReturning(Object returnValue, Method method, Object[] args, Object target) throws Throwable;
- }
在afterReturning方法中，你可以获得目标方法执行后的返回值、目标方法对象、目标方法的参数以及目标对象。
继续以上面的例子为例，如果要在MessageSender的send方法执行完后，要再记录日志，那么我们可以先实现AfterReturningAdvice接口：
java 代码
- package com.savage.aop;
- 
- import org.springframework.aop;
- 
- public LogAfterAdvice implements AfterReturningAdvice {
- publicvoid afterReturning(Object returnValue, Method method, Object[] args, Object
 target) throws Throwable {
-       System.out.println("Log after " + method + " by LogAfterAdvice.");
-    }
- }
然后在XML文件中指定LogAfterAdvice的实例：
xml 代码
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans    
- http://www.springframework.org/schema/beans/spring-beans-2.0.xsd">
- 
- <beanid="messageSenderImpl"class="com.savage.aop.HttpMessageSender"></bean>
- 
- <beanid="logBeforeAdvice"class="com.savage.aop.LogBeforeAdvice"></bean>
- 
- <beanid="messageSender"class="org.springframework.aop.framework.ProxyFactoryBean">
- <propertyname="proxyInterfaces"value="com.savage.aop.MessageSender"/>
- <propertyname="target"ref="messageSenderImpl"/>
- <propertyname="interceptorNames">
- <list>
- <value>logAfterAdvice</value>
- </list>
- </property>
- </bean>
- </beans>
在前面Before Advice的基础上，我们为MessageSender再指定了一个LogAfterAdvice的服务。运行前面的AdviceDemo，结果如下：
***Send Message[message] by http.Log after public abstract void com.savage.simplespring.bean.MessageSender.send(java.lang.String) by LogAfterAdvice.***
3、Around Advice
在上面的LogAfterAdvice例子中，我们通过指定BeforeAdvice和AfterReturingAdvice，在MessageSender的send方法前后执行额外的业务。实际上，如果需要在业务代码执行前后增加额外的服务，你可以直接通过实现org.aopalliance.intercept.MethodInterceptor接口来达到这一目的，MethodInterceptor定义如下：
java 代码
- package org.aopalliance.intercept;
- 
- publicinterface MethodInterceptor {
- public Object invoke(MethodInvocation methodInvocation) throws Throwable;
- }
例如：
java 代码
- package com.savage.aop;
- 
- import org.aopalliance.intercept.MethodInterceptor;
- import org.aopalliance.intercept.MethodInvocation;
- 
- publicclass LogAdvice implements MethodInterceptor
 {
- public Object invoke(MethodInvocation methodInvocation) throws Throwable {
-       System.out.println("Log before " + methodInvocation.getMethod() + " by LogAdvice.");
-       Object retValue = methodInvocation.proceed();
-       System.out.println("Log after " + methodInvocation.getMethod() + " by LogAdvice.");
- return retValue;
-    }
- }
正如上面所示，在MethodInterceptor中你得自行决定是否调用MethodInvocation的proceed()方法来执行目标对象上的方法，proceed()方法在执行完后会返回目标对象上方法的执行结果。
MethodInterceptor在XML文件中的定义如下： 
xml 代码
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans    
- http://www.springframework.org/schema/beans/spring-beans-2.0.xsd">
- 
- <beanid="messageSenderImpl"class="com.savage.aop.HttpMessageSender"></bean>
- 
- <beanid="logAdvice"class="com.savage.aop.LogAdvice"></bean>
- 
- <beanid="messageSender"class="org.springframework.aop.framework.ProxyFactoryBean">
- <propertyid="proxyInterfaces"value="com.savage.aop.MessageSender"/>
- <propertyid="target"ref="messageSenderImpl"/>
- <propertyid="interceptorNames">
- <list>
- <value>logAdvice</value>
- </list>
- </property>
- </bean>
- </beans>
Spring在真正执行目标对象的方法前，会执行interceptorNames中执行的Advice，每个Advice在执行完自己的业务后，会调用MethodInvocation的proceed()方法，将执行的主动权移交给下一个Advice，直到没有下一个Advice为止，在执行完目标对象的方法后，Spring会再以相反的顺序一层层的返回。例如：
xml 代码
- <beanid="messageSender"class="org.springframework.aop.framework.ProxyFactoryBean">
- <propertyid="proxyInterfaces"value="com.savage.aop.MessageSender"/>
- <propertyid="target"ref="messageSenderImpl"/>
- <propertyid="interceptorNames">
- <list>
- <value>logBeforeAdvice</value>
- <value>logAdvice</value>
- <value>logAfterAdvice</value>
- </list>
- </property>
- </bean>
象上面这个例子，logBeforeAdvice先会被执行，然后执行logAdvice，接着执行logAfterAdvice，最后又返回到了logAdvice。
现在我们把LogAdvice作一下简单的修改，增加一个id属性，用以在后面查看Advice的调用顺序：
java 代码
- package com.savage.aop;
- 
- import org.aopalliance.intercept.MethodInterceptor;
- import org.aopalliance.intercept.MethodInvocation;
- 
- publicclass LogAdvice implements MethodInterceptor
 {
- privatestaticint INSTANCE_NUM
 = 0;
- 
- privateint id;
- 
- public LogAdvice() {
-       id = ++INSTANCE_NUM;
-    }
- 
- public Object invoke(MethodInvocation methodInvocation) throws Throwable {
-       System.out.println("Log before " + methodInvocation.getMethod() + " by LogAdvice[" + id + "].");
-       Object retValue = methodInvocation.proceed();
-       System.out.println("Log after " + methodInvocation.getMethod() + " by LogAdvice[" + id + "].");
- return retValue;
-    }
- }
同时把XML中的定义改为：
xml 代码
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans    
- http://www.springframework.org/schema/beans/spring-beans-2.0.xsd">
- 
- <beanid="messageSenderImpl"class="com.savage.aop.HttpMessageSender"></bean>
- 
- <beanid="logBeforeAdvice"class="com.savage.aop.LogBeforeAdvice"></bean>
- <beanid="logAfterAdvice"class="com.savage.aop.LogAfterAdvice"></bean>
- <beanid="logAdvice1"class="com.savage.aop.LogAdvice"></bean>
- <beanid="logAdvice2"class="com.savage.aop.LogAdvice"></bean>
- 
- <beanid="messageSender"class="org.springframework.aop.framework.ProxyFactoryBean">
- <propertyid="proxyInterfaces"value="com.savage.aop.MessageSender"/>
- <propertyid="target"ref="messageSenderImpl"/>
- <propertyid="interceptorNames">
- <list>
- <value>logBeforeAdvice</value>
- <value>logAdvice1</value>
- <value>logAfterAdvice</value>
- <value>logAdvice2</value>
- </list>
- </property>
- </bean>
- </beans>
现在再执行AdviceDemo，得到如下结果：
***Log before public abstract void com.savage.simplespring.bean.MessageSender.send(java.lang.String) by LogBeforeAdvice.Log before public abstract void com.savage.simplespring.bean.MessageSender.send(java.lang.String) by LogAdvice[1].Log before public abstract void com.savage.simplespring.bean.MessageSender.send(java.lang.String) by LogAdvice[2].Send Message[message] by http.Log after public abstract void com.savage.simplespring.bean.MessageSender.send(java.lang.String) by LogAdvice[2].Log after public abstract void com.savage.simplespring.bean.MessageSender.send(java.lang.String) by LogAfterAdvice.Log after public abstract void com.savage.simplespring.bean.MessageSender.send(java.lang.String) by LogAdvice[1].***
4、Throw Advice
如果想要在异常发生时执行某些业务，你可以通过实现org.springframework.aop.ThrowsAdvice接口，这是一个标签接口，没有定义任何方法，你可以在当中为每个你需要处理的异常类定义afterThrowing方法，当程序出现异常时，spring会根据异常的类型调用对应的afterThrowing方法。AfterThrowing的格式如下：
java 代码
- afterThrowing([Method],[args],[target],subClassOfThrowable);
方括号[]中的参数为可选项，但方法中必须有subClassOfThrowable，且必须是Throwable的子类。
Spring在调用完afterThrowing方法后，原先的异常会继续在程序中传播，如果象要终止程序对异常的处理，只能在afterThrowing方法中抛出其他异常。
