# Spring3核心技术之AOP配置 - z69183787的专栏 - CSDN博客
置顶2012年12月20日 09:53:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7026
在Spring配置文件中，所有AOP相关定义必须放在<aop:config>标签下，该标签下可以有<aop:pointcut>、<aop:advisor>、<aop:aspect>标签，配置顺序不可变。 
![](http://dl.iteye.com/upload/attachment/0073/2650/5aede57e-ae09-3156-b0ec-47969b5e3075.jpg)
**● <aop:pointcut>**：用来定义切入点，该切入点可以重用； 
**● <aop:advisor>**：用来定义只有一个通知和一个切入点的切面； 
**● <aop:aspect>**：用来定义切面，该切面可以包含多个切入点和通知，而且标签内部的通知和切入点定义是无序的；和advisor的区别就在此，advisor只包含一个通知和一个切入点。
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- publicclass Interceptor {  
- 
- publicvoid beforeDomain() {  
-         System.out.println("This is beforeDomain....");  
-     }  
- 
- publicvoid afterDomain() {  
-         System.out.println("This is afterDomain....");  
-     }  
- 
- publicvoid afterReturning() {  
-         System.out.println("This is afterReturning....");  
-     }  
- 
- publicvoid afterThrowing() {  
-         System.out.println("This is afterThrowing....");  
-     }  
- 
- public Object around(ProceedingJoinPoint pjp) throws Throwable {    
-         System.out.println("===========around before advice");    
-         Object retVal = pjp.proceed(new Object[] {"【环绕通知】"});  
-         System.out.println("===========around after advice");    
- return retVal;    
-     }  
- }  
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- .....  
- 
- <beanid="aspectBean"class="com.chou.spring.domain.Interceptor"/>
- 
- <aop:configproxy-target-class="false">
- <aop:aspectref="aspectBean">
- <!-- 定义切入点 -->
- <aop:pointcutid="myAspect"
- expression="execution(public * com.chou.spring.bean..*.domain(..))"/>
- 
- <!-- 前置通知 -->
- <aop:beforepointcut-ref="myAspect"method="prepareDomain"/>
- 
- <!-- 后置通知 -->
- <aop:after-returningpointcut-ref="myAspect"method="afterReturning"/>
- <aop:after-throwingpointcut-ref="myAspect"method="afterThrowing"/>
- <aop:afterpointcut-ref="myAspect"method="afterDomain"/>
- 
- <!-- 环绕通知 -->
- <aop:aroundmethod="around"
- pointcut="execution(* com.chou.spring.bean..*.sayAround(..))"/>
- </aop:aspect>
- </aop:config>
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- publicinterface MyBean {  
- publicvoid domain();  
- }  
- 
- 
- publicclass MyBeanA{  
- publicvoid domain() {  
-         System.out.println("MyBeanA is executing...");  
-     }  
- 
- publicvoid sayAround(String param) {    
-           System.out.println("around param:" + param);    
-     }  
- }  
- 
- publicclass MyBeanB implements MyBean{  
- publicvoid domain() {  
-         System.out.println("MyBeanB is executing...");  
- //throw new RuntimeException("This is a RuntimeException");
-     }  
- }  
- 
- //main方法....
- String[] configs = new String[] {"applicationContext-aop.xml"};  
- ApplicationContext cxt = new ClassPathXmlApplicationContext(configs);  
- //如果Bean有interface那么就用JDK的Proxy.newProxyInstance得到代理对象进行aop
- MyBean b = (MyBean)cxt.getBean("beanB");  
- b.domain();  
- //如果Bean没有实现任何interface那么就用CGLIB得到代理对象进行aop
- MyBeanA a = cxt.getBean("beanA",MyBeanA.class);  
- a.domain();  
- a.sayAround("jjjjjjjjjjjjjjjjjjj");  
**声明切面**
    切面就是包含切入点和通知的对象，在Spring容器中将被定义为一个Bean，xml形式的切面需要一个切面支持Bean，该支持Bean的字段和方法提供了切面的状态和行为信息，并通过配置方式来指定切入点和通知实现。 
    切面使用<aop:aspect>标签指定，ref属性用来引用切面支持Bean。 
    切面支持Bean“aspectSupportBean”跟普通Bean完全一样使用，切面使用“ref”属性引用它。 
**声明切入点**
    切入点在Spring中也是一个Bean，Bean定义方式可以有很三种方式： 
● 在<aop:config>标签下使用<aop:pointcut>声明一个切入点Bean，该切入点可以被多个切面使用，对于需要共享使用的切入点最好使用该方式，该切入点使用id属性指定Bean名字，在通知定义时使用pointcut-ref属性通过该id引用切入点，expression属性指定切入点表达式。 
● 在<aop:aspect>标签下使用<aop:pointcut>声明一个切入点Bean，该切入点可以被多个切面使用，但一般该切入点只被该切面使用，当然也可以被其他切面使用，但最好不要那样使用，该切入点使用id属性指定Bean名字，在通知定义时使用pointcut-ref属性通过该id引用切入点，expression属性指定切入点表达式 
● 匿名切入点Bean，可以在声明通知时通过pointcut属性指定切入点表达式，该切入点是匿名切入点，只被该通知使用
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <aop:config>
- <aop:aspectref="aspectSupportBean">
- <aop:afterpointcut="execution(* cn.javass..*.*(..))"method="afterAdvice"/>
- </aop:aspect>
- </aop:config>
**关于切入点的expression表达式用法可以参考这个博客(xml和注解形式都通用)**： 
[http://jinnianshilongnian.iteye.com/blog/1415606](http://jinnianshilongnian.iteye.com/blog/1415606)
**声明通知：(前置通知，后置通知，环绕通知)**
**一、前置通知**:在切入点选择的连接点处的方法之前执行的通知，该通知不影响正常程序执行流程（除非该通知抛出异常，该异常将中断当前方法链的执行而返回）。 
Spring中在切入点选择的方法之前执行，通过<aop:aspect>标签下的<aop:before>标签声明：
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <aop:beforepointcut="切入点表达式"pointcut-ref="切入点Bean引用"
- method="前置通知实现方法名"arg-names="前置通知实现方法参数列表参数名字"/>
● pointcut和pointcut-ref：二者选一，指定切入点； 
● method：指定前置通知实现方法名，如果是多态需要加上参数类型，多个用“，”隔开，如beforeAdvice(java.lang.String)； 
● arg-names：指定通知实现方法的参数名字，多个用“，”分隔，可选，切入点中使用“args(param)”匹配的目标方法参数将自动传递给通知实现方法同名参数。 
关于arg-names具体用法可以参考博客：[http://jinnianshilongnian.iteye.com/blog/1418598](http://jinnianshilongnian.iteye.com/blog/1418598)
**二、后置通知**:在切入点选择的连接点处的方法之后执行的通知，包括如下类型的后置通知： 
● 后置返回通知:在切入点选择的连接点处的方法正常执行完毕时执行的通知，必须是连接点处的方法没抛出任何异常正常返回时才调用后置通知。 
在切入点选择的方法正常返回时执行，通过<aop:aspect>标签下的<aop:after-returning>标签声明：
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <aop:after-returningpointcut="切入点表达式"pointcut-ref="切入点Bean引用"
- method="后置返回通知实现方法名"
- arg-names="后置返回通知实现方法参数列表参数名字"
- returning="返回值对应的后置返回通知实现方法参数名"
- />
● 后置异常通知:在切入点选择的连接点处的方法抛出异常返回时执行的通知，必须是连接点处的方法抛出任何异常返回时才调用异常通知。 
在切入点选择的方法抛出异常时执行，通过<aop:aspect>标签下的<aop:after-throwing>标签声明：
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <aop:after-throwingpointcut="切入点表达式"pointcut-ref="切入点Bean引用"
- method="后置异常通知实现方法名"
- arg-names="后置异常通知实现方法参数列表参数名字"
- throwing="将抛出的异常赋值给的通知实现方法参数名"/>
● 后置最终通知:在切入点选择的连接点处的方法返回时执行的通知，不管抛没抛出异常都执行，类似于Java中的finally块。 
在切入点选择的方法返回时执行，不管是正常返回还是抛出异常都执行，通过<aop:aspect>标签下的<aop:after >标签声明：
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <aop:afterpointcut="切入点表达式"pointcut-ref="切入点Bean引用"
- method="后置最终通知实现方法名"
- arg-names="后置最终通知实现方法参数列表参数名字"/>
**三、环绕通知**：环绕着在切入点选择的连接点处的方法所执行的通知，环绕通知可以在方法调用之前和之后自定义任何行为，并且可以决定是否执行连接点处的方法、替换返回值、抛出异常等等。 
环绕着在切入点选择的连接点处的方法所执行的通知，环绕通知非常强大，可以决定目标方法是否执行，什么时候执行，执行时是否需要替换方法参数，执行完毕是否需要替换返回值，可通过<aop:aspect>标签下的<aop:around >标签声明：
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <aop:aroundpointcut="切入点表达式"pointcut-ref="切入点Bean引用"
- method="后置最终通知实现方法名"
- arg-names="后置最终通知实现方法参数列表参数名字"/>
环绕通知第一个参数必须是org.aspectj.lang.ProceedingJoinPoint类型，在通知实现方法内部使用ProceedingJoinPoint的proceed()方法使目标方法执行，proceed 方法可以传入可选的Object[]数组，该数组的值将被作为目标方法执行时的参数。 
**四、引入**
    Spring允许为目标对象引入新的接口，通过在< aop:aspect>标签内使用< aop:declare-parents>标签进行引入，定义方式如下：
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <aop:declare-parents
- types-matching="AspectJ语法类型表达式"
- implement-interface=引入的接口"                 
- default-impl="引入接口的默认实现"
- delegate-ref="引入接口的默认实现Bean引用"/>
具体用法请参考博客：[http://jinnianshilongnian.iteye.com/blog/1418598](http://jinnianshilongnian.iteye.com/blog/1418598)
**五、Advisor**
Advisor表示只有一个通知和一个切入点的切面，由于Spring AOP都是基于AOP的拦截器模型的环绕通知的，所以引入Advisor来支持各种通知类型（如前置通知等5种），Advisor概念来自于Spring1.2对AOP的支持，在AspectJ中没有相应的概念对应。 
Advisor可以使用<aop:config>标签下的<aop:advisor>标签定义： 
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <aop:advisorpointcut="切入点表达式"pointcut-ref="切入点Bean引用"
- advice-ref="通知API实现引用"/>
- 
- <beanid="beforeAdvice"class="cn.javass.spring.chapter6.aop.BeforeAdviceImpl"/>
- <aop:advisorpointcut="execution(* cn.javass..*.sayAdvisorBefore(..))"
- advice-ref="beforeAdvice"/>
**除了在进行事务控制的情况下，其他情况一般不推荐使用该方式，该方式属于侵入式设计，必须实现通知API**
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <!-- 事务管理器配置,单数据源事务 -->
- <beanid="transactionManager"
- class="org.springframework.orm.hibernate3.HibernateTransactionManager">
- <propertyname="sessionFactory"ref="sessionFactory"/>
- </bean>
- 
- <aop:config>
- <aop:advisorpointcut="execution(* com.spring.test.service..*.*(..))"
- advice-ref="txAdvice"/>
- </aop:config>
- 
- <tx:adviceid="txAdvice"transaction-manager="transactionManager">
- <tx:attributes>
- <tx:methodname="get*"read-only="true"/>
- <tx:methodname="find*"read-only="true"/>
- <tx:methodname="list*"read-only="true"/>
- <tx:methodname="save*"/>
- <tx:methodname="update*"/>
- <tx:methodname="delete*"/>
- </tx:attributes>
- </tx:advice>
- 
