# java框架篇---spring aop两种配置方式 - z69183787的专栏 - CSDN博客
2016年08月15日 11:56:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：450
个人分类：[Spring-Aop & 代理](https://blog.csdn.net/z69183787/article/category/2175403)

- publicvoid add(User user) { 
-         userDAO.save(user); 
-     } 
- public UserDAO getUserDAO() { 
- return userDAO; 
-     } 
- 
- @Resource(name="u") 
- publicvoid setUserDAO( UserDAO userDAO) { 
- this.userDAO = userDAO; 
-     } 
- 
- publicvoid destroy() { 
-         System.out.println("destroy"); 
-     } 
- } 
加入aop
- package com.bjsxt.aop; 
- 
- import org.aspectj.lang.annotation.After; 
- import org.aspectj.lang.annotation.AfterReturning; 
- import org.aspectj.lang.annotation.AfterThrowing; 
- import org.aspectj.lang.annotation.Aspect; 
- import org.aspectj.lang.annotation.Before; 
- import org.aspectj.lang.annotation.Pointcut; 
- import org.springframework.stereotype.Component; 
- 
- @Aspect
- @Component
- publicclass LogInterceptor { 
- @Pointcut("execution(public * com.bjsxt.service..*.add(..))") 
- publicvoid myMethod(){}; 
- 
- /*@Before("execution(public void com.bjsxt.dao.impl.UserDAOImpl.save(com.bjsxt.model.User))")*/
- @Before("myMethod()") 
- publicvoid before() { 
-         System.out.println("method staet"); 
-     }  
- @After("myMethod()") 
- publicvoid after() { 
-         System.out.println("method after"); 
-     }  
- @AfterReturning("execution(public * com.bjsxt.dao..*.*(..))") 
- publicvoid AfterReturning() { 
-         System.out.println("method AfterReturning"); 
-     }  
- @AfterThrowing("execution(public * com.bjsxt.dao..*.*(..))") 
- publicvoid AfterThrowing() { 
-         System.out.println("method AfterThrowing"); 
-     }  
- } 
配置文件
- <?xml version="1.0" encoding="UTF-8"?> 
- <beans xmlns="http://www.springframework.org/schema/beans"
-        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-        xmlns:context="http://www.springframework.org/schema/context"
-        xmlns:aop="http://www.springframework.org/schema/aop"
-        xsi:schemaLocation="http://www.springframework.org/schema/beans
-            http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
-            http://www.springframework.org/schema/context
-            http://www.springframework.org/schema/context/spring-context-2.5.xsd
-            http://www.springframework.org/schema/aop            
-            http://www.springframework.org/schema/aop/spring-aop-3.1.xsd  
-            "><!-- 要添加最后2行 --> 
- 
-     <context:annotation-config /> 
-     <context:component-scan base-package="com.bjsxt"/>  <!-- 自动扫描 --> 
-     <aop:aspectj-autoproxy/>  <!-- 要添加本行 --> 
- </beans> 
**测试类：**
- package com.bjsxt.service; 
- import org.junit.Test; 
- import org.springframework.context.ApplicationContext; 
- import org.springframework.context.support.ClassPathXmlApplicationContext; 
- 
- import com.bjsxt.model.User; 
- 
- //Dependency Injection
- //Inverse of Control
- publicclass UserServiceTest { 
- 
- @Test
- publicvoid testAdd() throws Exception { 
-         ClassPathXmlApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml"); 
- 
- 
-         UserService service = (UserService)ctx.getBean("userService"); 
-         System.out.println(service.getClass()); 
-         service.add(new User()); 
-         System.out.println("###"); 
- 
-         ctx.destroy(); 
- 
-     } 
- 
- } 
**结果：**
class com.bjsxt.service.UserService$$EnhancerByCGLIB$$7b201784
method staet
user save11d!
method AfterReturning
method after
###
**注意：**
@Aspect：意思是这个类为切面类 
@Componet：因为作为切面类需要 Spring 管理起来，所以在初始化时就需要将这个类初始化加入 Spring 的管理； 
@Befoe：切入点的逻辑(Advice)
execution…:切入点语法
**第二种：xml配置aop**
实例同上：只是配置文件不同
- <?xml version="1.0" encoding="UTF-8"?> 
- <beans xmlns="http://www.springframework.org/schema/beans"
-        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-        xmlns:context="http://www.springframework.org/schema/context"
-        xmlns:aop="http://www.springframework.org/schema/aop"
-        xsi:schemaLocation="http://www.springframework.org/schema/beans
-            http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
-            http://www.springframework.org/schema/context
-            http://www.springframework.org/schema/context/spring-context-2.5.xsd
-            http://www.springframework.org/schema/aop            
-            http://www.springframework.org/schema/aop/spring-aop-3.1.xsd  
-            "><!-- 要添加最后2行 --> 
- 
-     <context:annotation-config /> 
-     <context:component-scan base-package="com.bjsxt"/> 
-     <bean id="logInterceptor"class="com.bjsxt.aop.LogInterceptor"></bean> 
-     <aop:config> 
-         <aop:pointcut expression="execution(public * com.bjsxt.service..*.add(..))"
-         id="servicePointcut"/> 
-         <aop:aspect id="logAspect" ref="logInterceptor"> 
-             <aop:before method="before"  pointcut-ref="servicePointcut" /> 
-         </aop:aspect> 
- 
-     </aop:config> 
- </beans> 
下面的<beans>是Spring的配置标签，beans里面几个重要的属性：
xmlns：
是默认的xml文档解析格式，即spring的beans。地址是http://www.springframework.org/schema/beans。
通过设置这个属性，所有在beans里面声明的属性，可以直接通过<>来使用，比如<bean>等等。
xmlns:xsi：
是xml需要遵守的规范，通过URL可以看到，是w3的统一规范，后面通过xsi:schemaLocation来定位所有的解析文件。
xmlns:aop：
这个是重点，是我们这里需要使用到的一些语义规范，与面向切面AOP相关。
xmlns:tx:
Spring中与事务相关的配置内容。
一个XML文件，只能声明一个默认的语义解析的规范。
例如上面的xml中就只有beans一个是默认的，其他的都需要通过特定的标签来使用，比如aop，它自己有很多的属性，如果要使用，前面就必须加上aop:xxx才可以。比如上面的aop:config。
类似的，如果默认的xmlns配置的是aop相关的语义解析规范，那么在xml中就可以直接写config这种标签了。
- publicvoid add(User user) { 
-         userDAO.save(user); 
-     } 
- public UserDAO getUserDAO() { 
- return userDAO; 
-     } 
- 
- @Resource(name="u") 
- publicvoid setUserDAO( UserDAO userDAO) { 
- this.userDAO = userDAO; 
-     } 
- 
- publicvoid destroy() { 
-         System.out.println("destroy"); 
-     } 
- } 
加入aop
- package com.bjsxt.aop; 
- 
- import org.aspectj.lang.annotation.After; 
- import org.aspectj.lang.annotation.AfterReturning; 
- import org.aspectj.lang.annotation.AfterThrowing; 
- import org.aspectj.lang.annotation.Aspect; 
- import org.aspectj.lang.annotation.Before; 
- import org.aspectj.lang.annotation.Pointcut; 
- import org.springframework.stereotype.Component; 
- 
- @Aspect
- @Component
- publicclass LogInterceptor { 
- @Pointcut("execution(public * com.bjsxt.service..*.add(..))") 
- publicvoid myMethod(){}; 
- 
- /*@Before("execution(public void com.bjsxt.dao.impl.UserDAOImpl.save(com.bjsxt.model.User))")*/
- @Before("myMethod()") 
- publicvoid before() { 
-         System.out.println("method staet"); 
-     }  
- @After("myMethod()") 
- publicvoid after() { 
-         System.out.println("method after"); 
-     }  
- @AfterReturning("execution(public * com.bjsxt.dao..*.*(..))") 
- publicvoid AfterReturning() { 
-         System.out.println("method AfterReturning"); 
-     }  
- @AfterThrowing("execution(public * com.bjsxt.dao..*.*(..))") 
- publicvoid AfterThrowing() { 
-         System.out.println("method AfterThrowing"); 
-     }  
- } 
配置文件
- <?xml version="1.0" encoding="UTF-8"?> 
- <beans xmlns="http://www.springframework.org/schema/beans"
-        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-        xmlns:context="http://www.springframework.org/schema/context"
-        xmlns:aop="http://www.springframework.org/schema/aop"
-        xsi:schemaLocation="http://www.springframework.org/schema/beans
-            http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
-            http://www.springframework.org/schema/context
-            http://www.springframework.org/schema/context/spring-context-2.5.xsd
-            http://www.springframework.org/schema/aop            
-            http://www.springframework.org/schema/aop/spring-aop-3.1.xsd  
-            "><!-- 要添加最后2行 --> 
- 
-     <context:annotation-config /> 
-     <context:component-scan base-package="com.bjsxt"/>  <!-- 自动扫描 --> 
-     <aop:aspectj-autoproxy/>  <!-- 要添加本行 --> 
- </beans> 
**测试类：**
- package com.bjsxt.service; 
- import org.junit.Test; 
- import org.springframework.context.ApplicationContext; 
- import org.springframework.context.support.ClassPathXmlApplicationContext; 
- 
- import com.bjsxt.model.User; 
- 
- //Dependency Injection
- //Inverse of Control
- publicclass UserServiceTest { 
- 
- @Test
- publicvoid testAdd() throws Exception { 
-         ClassPathXmlApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml"); 
- 
- 
-         UserService service = (UserService)ctx.getBean("userService"); 
-         System.out.println(service.getClass()); 
-         service.add(new User()); 
-         System.out.println("###"); 
- 
-         ctx.destroy(); 
- 
-     } 
- 
- } 
**结果：**
class com.bjsxt.service.UserService$$EnhancerByCGLIB$$7b201784
method staet
user save11d!
method AfterReturning
method after
###
**注意：**
@Aspect：意思是这个类为切面类 
@Componet：因为作为切面类需要 Spring 管理起来，所以在初始化时就需要将这个类初始化加入 Spring 的管理； 
@Befoe：切入点的逻辑(Advice)
execution…:切入点语法
**第二种：xml配置aop**
实例同上：只是配置文件不同
- <?xml version="1.0" encoding="UTF-8"?> 
- <beans xmlns="http://www.springframework.org/schema/beans"
-        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-        xmlns:context="http://www.springframework.org/schema/context"
-        xmlns:aop="http://www.springframework.org/schema/aop"
-        xsi:schemaLocation="http://www.springframework.org/schema/beans
-            http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
-            http://www.springframework.org/schema/context
-            http://www.springframework.org/schema/context/spring-context-2.5.xsd
-            http://www.springframework.org/schema/aop            
-            http://www.springframework.org/schema/aop/spring-aop-3.1.xsd  
-            "><!-- 要添加最后2行 --> 
- 
-     <context:annotation-config /> 
-     <context:component-scan base-package="com.bjsxt"/> 
-     <bean id="logInterceptor"class="com.bjsxt.aop.LogInterceptor"></bean> 
-     <aop:config> 
-         <aop:pointcut expression="execution(public * com.bjsxt.service..*.add(..))"
-         id="servicePointcut"/> 
-         <aop:aspect id="logAspect" ref="logInterceptor"> 
-             <aop:before method="before"  pointcut-ref="servicePointcut" /> 
-         </aop:aspect> 
- 
-     </aop:config> 
- </beans> 
下面的<beans>是Spring的配置标签，beans里面几个重要的属性：
xmlns：
是默认的xml文档解析格式，即spring的beans。地址是http://www.springframework.org/schema/beans。
通过设置这个属性，所有在beans里面声明的属性，可以直接通过<>来使用，比如<bean>等等。
xmlns:xsi：
是xml需要遵守的规范，通过URL可以看到，是w3的统一规范，后面通过xsi:schemaLocation来定位所有的解析文件。
xmlns:aop：
这个是重点，是我们这里需要使用到的一些语义规范，与面向切面AOP相关。
xmlns:tx:
Spring中与事务相关的配置内容。
一个XML文件，只能声明一个默认的语义解析的规范。
例如上面的xml中就只有beans一个是默认的，其他的都需要通过特定的标签来使用，比如aop，它自己有很多的属性，如果要使用，前面就必须加上aop:xxx才可以。比如上面的aop:config。
类似的，如果默认的xmlns配置的是aop相关的语义解析规范，那么在xml中就可以直接写config这种标签了。
