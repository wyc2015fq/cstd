# Java Spring AOP用法 - 建建的博客 - CSDN博客
2018年01月10日 09:06:52[纪建](https://me.csdn.net/u013898698)阅读数：117
# Java Spring AOP用法
## Spring AOP
[Java web 环境搭建](http://www.cnblogs.com/flowwind/p/4751938.html)
[Java web 项目搭建](http://www.cnblogs.com/flowwind/p/4751984.html)
[Java Spring IOC用法](http://www.cnblogs.com/flowwind/p/4772375.html)
spring提供了两个核心功能，一个是IoC(控制反转)，另外一个便是Aop(面向切面编程)，IoC有助于应用对象之间的解耦，AOP则可以实现横切关注点（如日志、安全、缓存和事务管理）与他们所影响的对象之间的解耦。
### 1.简介
AOP主要包含了通知、切点和连接点等术语，介绍如下
- 
通知(Advice)
通知定义了切面是什么以及何时调用，何时调用包含以下几种
> 
Before 在方法被调用之前调用通知
After 在方法完成之后调用通知，无论方法执行是否成功
After-returning 在方法成功执行之后调用通知
After-throwing 在方法抛出异常后调用通知
Around 通知包裹了被通知的方法，在被通知的方法调用之前和调用之后执行自定义的行为
- 
切点(PointCut)
通知定义了切面的什么和何时，切点定义了何处，切点的定义会匹配通知所要织入的一个或多个连接点，我们通常使用明确的类的方法名称来指定这些切点，或是利用正则表达式定义匹配的类和方法名称来指定这些切点。
切点的格式如下
```
execution(*
 com.ganji.demo.service.user.UserService.GetDemoUser (..) )
```
![aop切点定位表达式](http://7xipae.com1.z0.glb.clouddn.com/aop%E5%88%87%E7%82%B9%E5%AE%9A%E4%BD%8D%E8%A1%A8%E8%BE%BE%E5%BC%8F.png)
- 
连接点(JoinPoint)
连接点是在应用执行过程中能够插入切面的一个点，这个点可以是调用方法时，抛出异常时，甚至是修改一个字段时，切面代码可以利用这些连接点插入到应用的正常流程中，并添加新的行为，如日志、安全、事务、缓存等。
现阶段的AOP框架
AOP框架除了[Spring AOP](http://www.springframework.org/)之外，还包括[AspectJ](http://eclipse.org/aspectj)、[JBoss
 AOP](http://www.jboss.org/jbossaop)；
上述框架的区别是Spring AOP只支持到方法连接点，另外两个还支持字段和构造器连接点。
### 2.用法
同依赖注入一样，AOP在spring中有两种配置方式，一是xml配置的方式，二是自动注解的模式。
- 
#### 2.1 xml中声明切面
- 2.1.1 AOP配置元素
在xml中，我们使用如下AOP配置元素声明切面
```
AOP配置元素 | 描述 
------------ | -------------
`<aop:advisor>` | 定义AOP通知器
`<aop:after>`  | 定义AOP后置通知（不管该方法是否执行成功）
`<aop:after-returning>` | 在方法成功执行后调用通知
`<aop:after-throwing>` | 在方法抛出异常后调用通知
`<aop:around>` | 定义AOP环绕通知
`<aop:aspect>` | 定义切面
`<aop:aspect-autoproxy>` | 定义`@AspectJ`注解驱动的切面
`<aop:before>` | 定义AOP前置通知
`<aop:config>` | 顶层的AOP配置元素，大多数的<aop:*>包含在<aop:config>元素内
`<aop:declare-parent>` | 为被通知的对象引入额外的接口，并透明的实现
`<aop:pointcut>` | 定义切点
```
- 2.1.2 定义切面
我们在service层添加`com.ganji.demo.service.aspect.XmlAopDemoUserLog`类,里面实现了拦截方法，具体如下
```java
```java
package com.ganji.demo.service.aspect;
import org.aspectj.lang.ProceedingJoinPoint;
/**
 * Created by admin on 2015/9/2.
 */
public class XmlAopDemoUserLog {
//    方法执行前通知
    public void beforeLog() {
        System.out.println("开始执行前置通知  日志记录");
    }
//    方法执行完后通知
    public void afterLog() {
        System.out.println("开始执行后置通知 日志记录");
    }
//    执行成功后通知
    public void afterReturningLog() {
        System.out.println("方法成功执行后通知 日志记录");
    }
//    抛出异常后通知
    public void afterThrowingLog() {
        System.out.println("方法抛出异常后执行通知 日志记录");
    }
//    环绕通知
    public Object aroundLog(ProceedingJoinPoint joinpoint) {
        Object result = null;
        try {
            System.out.println("环绕通知开始 日志记录");
            long start = System.currentTimeMillis();
            //有返回参数 则需返回值
            result =  joinpoint.proceed();
            long end = System.currentTimeMillis();
            System.out.println("总共执行时长" + (end - start) + " 毫秒");
            System.out.println("环绕通知结束 日志记录");
        } catch (Throwable t) {
            System.out.println("出现错误");
        }
        return result;
    }
}
```
```
- 2.1.3 xml声明切面并调用
我们在web层，`web-inf/dispatcher-servlet.xml`中定义切面，具体如下
```xml
<!--定义切面 指定拦截方法时 做什么-->
<bean id="xmlAopDemoUserLog" class="com.ganji.demo.service.aspect.XmlAopDemoUserLog"></bean>
<aop:config>
    <aop:aspect ref="xmlAopDemoUserLog"> <!--指定切面-->
        <!--定义切点-->
        <aop:pointcut id="logpoint" expression="execution(* com.ganji.demo.service.user.UserService.GetDemoUser(..))"></aop:pointcut>
        <!--定义连接点-->
        <aop:before pointcut-ref="logpoint" method="beforeLog"></aop:before>
        <aop:after pointcut-ref="logpoint" method="afterLog"></aop:after>
        <aop:after-returning pointcut-ref="logpoint" method="afterReturningLog"></aop:after-returning>
        <aop:after-throwing pointcut-ref="logpoint" method="afterThrowingLog"></aop:after-throwing>
    </aop:aspect>
</aop:config>
```
在controller下调用，调用具体如下
```
DemoUserEntity
 demoUser=userService.GetDemoUser(1);
```
这是运行起来 我们将看到打印出如下日志
> 
开始执行前置通知 日志记录
开始执行后置通知 日志记录
方法成功执行后通知 日志记录
- 2.1.4 小结
如果通过xml配置，我们还可以实现环绕通知，环绕通知的目的是把前置通知和后置通知的信息共享起来。同时还可以为通知传递方法的参数,在切面拦截中验证参数的有效性。
- 
#### 2.2 自动注解AOP
在上述2.1中我们通过xml配置的形式 实现了AOP编程，现在我们通过不配置xml，配置注解的形式实现AOP。
- 2.2.1 配置自动代理
使用配置注解，首先我们要将切面在spring上下文中声明成自动代理bean，我们需要在web层的`web-inf/dispatcher-servlet.xml`文件中配置如下一句话即可
```
<aop:aspectj-autoproxy
 />
```
当然我们需要在xml的根目录beans下引用aop的命名空间和xsi
```xml
xmlns:aop="http://www.springframework.org/schema/aop"
xsi:schemaLocation="
  http://www.springframework.org/schema/aop
  http://www.springframework.org/schema/aop/spring-aop-3.0.xsd"
```
- 2.2.2 使用`@Aspect`注解
声明一个切面，只需要在类名上添加`@Aspect`属性即可，具体的连接点，我们用`@Pointcut`和`@Before`、`@After`等标注。具体如下
在声明前 我们需要依赖配置pom
```xml
<dependency>
    <groupId>org.aspectj</groupId>
    <artifactId>aspectjrt</artifactId>
    <version>1.6.11</version>
</dependency>
<dependency>
    <groupId>org.aspectj</groupId>
    <artifactId>aspectjweaver</artifactId>
    <version>1.6.11</version>
</dependency>
```
声明切面类，包含了注解`@Aspect`以及何时(如`@Before`)执行通知
```java
```java
package com.ganji.demo.service.aspect;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.*;
import org.springframework.stereotype.Service;
/**
 * Created by admin on 2015/9/2.
 */
@Aspect
@Service
public class XmlAopDemoUserLog {
// 配置切点 及要传的参数   
    @Pointcut("execution(* com.ganji.demo.service.user.UserService.GetDemoUser(..)) && args(id)")
    public void pointCut(int id)
    {
    }
// 配置连接点 方法开始执行时通知
    @Before("pointCut(id)")
    public void beforeLog(int id) {
        System.out.println("开始执行前置通知  日志记录:"+id);
    }
//    方法执行完后通知
    @After("pointCut(id)")
    public void afterLog(int id) {
        System.out.println("开始执行后置通知 日志记录:"+id);
    }
//    执行成功后通知
    @AfterReturning("pointCut(id)")
    public void afterReturningLog(int id) {
        System.out.println("方法成功执行后通知 日志记录:"+id);
    }
//    抛出异常后通知
    @AfterThrowing("pointCut(id)")
    public void afterThrowingLog(int id) {
        System.out.println("方法抛出异常后执行通知 日志记录"+id);
    }
//    环绕通知
    @Around("pointCut(id)")
    public Object aroundLog(ProceedingJoinPoint joinpoint,int id) {
        Object result = null;
        try {
            System.out.println("环绕通知开始 日志记录"+id);
            long start = System.currentTimeMillis();
            //有返回参数 则需返回值
            result =  joinpoint.proceed();
            long end = System.currentTimeMillis();
            System.out.println("总共执行时长" + (end - start) + " 毫秒");
            System.out.println("环绕通知结束 日志记录");
        } catch (Throwable t) {
            System.out.println("出现错误");
        }
        return result;
    }
}
```
```
- 2.2.3 总结
按照上述两个步骤，使用注解实现Aop即可，这里依赖了IOC。
