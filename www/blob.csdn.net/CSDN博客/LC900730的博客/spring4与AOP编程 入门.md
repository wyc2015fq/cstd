# spring4与AOP编程:入门 - LC900730的博客 - CSDN博客
2017年08月08日 21:16:10[lc900730](https://me.csdn.net/LC900730)阅读数：221
## AOP概述
Aop: aspect object programing 面向切面编程 
    功能：让关注点代码与业务代码分离 
关注点：重复代码就叫做关注点 
切面：  
    关注点形成的类就叫做切面类 
    面向切面编程就是指很多功能都有重复的代码抽取，在运行的时候往业务方面上动态植入“切面类代码” 
切入点： 
    执行目标对象方法，动态植入切面代码。 
    可以通过切入点表达式，指定拦截哪些类的哪些方法；给指定的类在运行的时候植入切面类代码
### 用途
程序的事务控制，可以使用 aop实现。即只需要执行一次，运行时候动态植入到业务方法中。 
Spring提供了事务支持，开发者只需要按照spring的方式去做。aop来实现的
### 引入的jar
```
spring-core：
    commons-logging.jar;
    spring-beans-4.0.5.RELEASE.jar
    spring-context-4.0.5.RELEASE.jar
    spring-core-4.0.5.RELEASE.jar
    spring-expression-4.0.5.RELEASE.jar
    spring-aop-4.0.5.RELEASE.jar
    spring-aspects-4.0.5.RELEASE.jar
```
以上是spring4里面的jar包，aop的jar需要单独下载     
```
aop
    aopaliance-1.0.jar
    aspectjrt-1.8.8.jar
    aspectjweaver-1.8.8.jar
```
以上是需要引入的jar
代码：
```java
//文件：src/cn.itcast.e_myaop1/Aop.java
package cn.itcast.e_myaop1;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.stereotype.Component;
@Component
@Aspect
public class Aop {
    //指定切入点表达式，拦截哪些方法，即为哪些类生成代理对象
    @Pointcut("execution(* cn.itcast.e_myaop1.UserDao.save(..))")
    public void begin(){
        System.out.println("开始事务"); 
    }
    @Pointcut("execution(* cn.itcast.e_myaop1.UserDao.save(..))")
    public void commit(){
        System.out.println("提交事务");
    }
}
```
目标对象有实现接口，spring会选择JDK代理(即动态代理)
```
@Pointcut("execute(* cn.itcast.e_dao.*.*(..))")
public void pointCut_(){}
//前置通知，在执行目标方法之前执行
@Before("pointCut_()")
public void begin(){
    System.out.println("开始事务/异常");
}
//后置通知，执行目标方法之后执行
@After("pointCut_()")
public void begin(){
    System.out.println("结束事务/异常");
} 
//返回后通知，在调用目标方法之后执行()
@AfterReturning("pointCut_()")
public void after(){
    System.out.println("afterReturning");
}
//异常通知，目标方法执行异常后执行此关注点代码
//目标方法内执行异常
@AfterThrowing("pointCut_()")
public void after(){
    System.out.println("AfterThrowing");
}
//环绕
@Around("pointCut_()")
public void around(ProcedingJoinPoint pjp){
    System.out.println("环绕前");
    Object returnValue=pjp.proceed();
    System.out.println("环绕后。。。");
}
环绕=前置+后置
```
### XML配置AOP
```xml
<bean id="userDao" class="cn.f_myaop.UserDao"></bean>
<bean id="OrderDao" class="cn.f_myaop.OrderDao"></bean>
         <!--Aop配置-->
         <aop:config>
            <!--定义一个切入点表达式，拦截哪些方法-->
            <!--此处定义，那么下文中的pointcut都可以引用-->
            <aop:pointcut expression="execution(* cn.itcast.f_myaopxml.*.*(..))" id="pt"/>
            <!-- 切面 -->
            <aop:aspect ref="aop">
                <!--前置通知，在目标代码执行前执行begin()-->
                <aop:before method="begin" pointcut-ref="pt"/>
                <!--后置通知，在目标代码执行之后执行-->
                <!--在目标代码执行之后执行aop的commit() -->
                <aop:after method="commit" pointcut-ref="pt"/>
             </aop:aspect>
         </aop:config>
```
### 环绕方法的拦截
```
public void around(ProceedingJoinPoint pjp) throws Throwable {
        System.out.println("环绕前");
        Object returnValue=pjp.proceed();
        System.out.println("环绕前");
    }
```
### 切换点表达式
```
可以对指定的"方法"进行拦截，从而给指定的方法所在的类生成代理对象。
execution(modifiers-pattern?ret-type-pattern  declaring-type-pattern?name-pattern(param-pattern)   throws-pattern?)
    1.如拦截所有的public方法
    execution(public * * (..))
    2.如拦截所有的save开头的方法   
    execution(* save*(..))
    3.如拦截修饰符是public ，返回值是*
    execution(public * cn.itcast.g_pointcut.UserDao.save(..))
    4.如拦截包及其子包，所有类方法所有方法
    execution(* cn..*.*(..))
    5.拦截某几个包的
    execution(* cn.itcast.g_pointcut.UserDao.save())||execution(* cn.itcast.g_pointcut.OrderDao.save())
||可以换成or
6.取非值(不拦截这个OrderDao)(或者 not，not前面加空格)
！execution(* cn.itcast.g_pointcut.OrderDao.save())
```
