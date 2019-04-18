# Spring AOP中自我调用的问题 - z69183787的专栏 - CSDN博客
2018年07月27日 18:52:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：127
[https://blog.csdn.net/zknxx/article/details/72585822](https://blog.csdn.net/zknxx/article/details/72585822)
前几天在做项目的时候同事说，在使用AOP进行拦截的时候发现有些方法有时候能输出拦截的日志有时候不输出拦截的日志。发现在单独调用这些方法的时候是有日志输出，在被同一个类中的方法调用的时候没有日志输出。我记得之前看过一篇文章是讲Spring事务自我调用不起作用的问题，应该是同样的问题(如果要观看那篇文章请点击这里[http://jinnianshilongnian.iteye.com/blog/1487235](https://blog.csdn.net/zknxx/article/details/Spring%E4%BA%8B%E5%8A%A1%E5%A4%84%E7%90%86%E6%97%B6%E8%87%AA%E6%88%91%E8%B0%83%E7%94%A8%E7%9A%84%E8%A7%A3%E5%86%B3%E6%96%B9%E6%A1%88%E5%8F%8A%E4%B8%80%E4%BA%9B%E5%AE%9E%E7%8E%B0%E6%96%B9%E5%BC%8F%E7%9A%84%E9%A3%8E%E9%99%A9))。这里先说一下AOP拦截不到自我调用方法的原因：假设我们有一个类是ServiceA，这个类中有一个A方法，A方法中又调用了B方法。当我们使用AOP进行拦截的时候，首先会创建一个ServiceA的代理类，其实在我们的系统中是存在两个ServiceA的对象的，一个是目标ServiceA对象，一个是生成的代理ServiceA对象，如果在代理类的A方法中调用代理类的B方法，这个时候AOP拦截是可以生效的，但是如果在代理类的A方法中调用目标类的B方法，这个时候AOP拦截是不生效的，大多数情况下我们都是在代理类的A方法中直接调用目标类的B方法。那么这种情况我们怎么来解决呢？这里我们简单的写了三种解决方法，但是我们首先先做一些准备动作。
## 先前准备：
### Service类：
```java
package com.zkn.spring.miscellaneous.service;
 
/**
 *  自我调用的服务类
 * @author zkn
 */
public interface SelfCallService {
    /**
     * 方法A
     */
    void selfCallA();
 
    /**
     * 方法B
     */
    void selfCallB();
}
```
### Service的实现类：
```java
package com.zkn.spring.miscellaneous.service.impl;
 
import com.zkn.spring.miscellaneous.service.SelfCallService;
import org.springframework.aop.support.AopUtils;
import org.springframework.stereotype.Service;
 
/**
 * @author zkn
 */
@Service
public class SelfCallServiceImpl implements SelfCallService{
    /**
     * 方法A
     */
    public void selfCallA() {
 
        System.out.println("我是方法A");
        System.out.println("是否是AOP拦截:" + AopUtils.isAopProxy(this));
        this.selfCallB();
    }
 
    /**
     * 方法B
     */
    public void selfCallB() {
        System.out.println("我是方法B");
    }
}
```
### AOP配置类：
```java
package com.zkn.spring.miscellaneous.aop;
 
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.Signature;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.stereotype.Component;
 
/**
 * @author zkn
 */
@Component
@Aspect
public class SelfCallAOP {
 
    @Pointcut("execution(* com.zkn.spring.miscellaneous.service.SelfCallService.*(..))")
    public void pointCut(){
 
    }
    @Around("pointCut()")
    public void aroundAdvice(ProceedingJoinPoint pjp){
        //获取签名的信息
        Signature signature = pjp.getSignature();
        System.out.println("被拦截的方法名为："+signature.getName());
        try {
            pjp.proceed();
            System.out.println("方法执行完成:"+signature.getName());
        } catch (Throwable throwable) {
            throwable.printStackTrace();
        }
    }
}
```
### Controller类：
```java
package com.zkn.spring.miscellaneous.controller;
 
import com.zkn.spring.miscellaneous.service.SelfCallService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
/**
 *
 * @author zkn
 * @date 2017/05/16
 */
@RestController
public class ProcessSelfCallController {
    @Autowired
    private SelfCallService selfCallService;
 
    @RequestMapping("processSelfCallA")
    public String processSelfCallA() {
 
        selfCallService.selfCallA();
        return "处理自我调用!";
    }
 
    @RequestMapping("processSelfCallB")
    public String processSelfCallB() {
        selfCallService.selfCallB();
        return "直接调用方法B!";
    }
}
```
### Spring的配置文件：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:aop="http://www.springframework.org/schema/aop"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context-3.2.xsd
        http://www.springframework.org/schema/aop
        http://www.springframework.org/schema/aop/spring-aop-3.2.xsd"
       default-autowire="byName">
    <!--开启注解-->
    <context:annotation-config/>
    <!--扫描基础包 这里要注意的是如果SpringMVC和Spring扫描的包是一样的话，AOP的配置可能会失效-->
    <context:component-scan base-package="com.zkn.spring.miscellaneous.service"/>
    <context:component-scan base-package="com.zkn.spring.miscellaneous.aop"/>
    <!--配置AOP proxy-target-class为true的时候是用Cglib动态代理，false的时候启用JDK动态代理-->
    <aop:aspectj-autoproxy proxy-target-class="true"/>
</beans>
```
### SpringMVC的配置文件：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/mvc
		http://www.springframework.org/schema/mvc/spring-mvc-3.2.xsd
        http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context-3.2.xsd"
       default-autowire="byName">
    <!--请求解析器-->
    <bean class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping"/>
    <bean class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter">
        <property name="messageConverters">
            <list>
                <bean class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter"/>
                <bean class="org.springframework.http.converter.StringHttpMessageConverter">
                    <property name="supportedMediaTypes">
                        <list>
                            <value>text/plain;charset=utf-8</value>
                            <value>text/html;charset=UTF-8</value>
                        </list>
                    </property>
                </bean>
            </list>
        </property>
    </bean>
    <!--先开启MVC的注解扫描-->
    <mvc:annotation-driven/>
    <!--开启注解扫描-->
    <context:component-scan base-package="com.zkn.spring.miscellaneous.controller"/>
</beans>
```
这里需要注意的是：网上有很多人说自己在SpringMVC配置的AOP不起作用。原因是在SpringMVC的配置文件中开启自动扫描的包和在Spring的配置文件中开启自动扫描的包一样，而SpringMVC的自动扫描覆盖了Spring的自动扫描(子父容器)。所以这里最好SpringMVC只扫描Controller这一层的包，其他的包交给Spring来扫描。
OK我们的准备动作已经完成了下面看我们的解决办法：
## 通过ThreadLocal暴露代理对象
第一种解决方式是通过ThreadLocal暴露代理对象的方式。我们只需要做这两步就行了。第一步：Spring的配置文件中将<aop:aspectj-autoproxy proxy-target-class="true" />改为：
` <aop:aspectj-autoproxy proxy-target-class="true" expose-proxy="true"/>`
第二步：修改Service的实现类，修改为如下：
```java
@Service
public class SelfCallServiceImpl implements SelfCallService {
    /**
     * 方法A
     */
    public void selfCallA() {
        //通过暴露ThreadLocal的方式获取代理对象
        ((SelfCallService)AopContext.currentProxy()).selfCallB();
    }
    /**
     * 方法B
     */
    public void selfCallB() {
        System.out.println("我是方法B");
    }
}
```
下面我们访问以下看看效果如何：http://localhost:8080/processSelfCallA，结果如下所示：
![](https://img-blog.csdn.net/20170520233655083?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemtueHg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
从上图中可以看到selfCallA和selfCallB两个方法都被拦截到了，说明我们的配置生效了。
## 通过初始化方法的方式：
如果我们使用这一种方式的话，那么我们需要做的是需要注入ApplicationContext对象，然后从ApplicationContext对象中获取被代理的类。注意：配置文件不做变动。具体做法如下：
```java
@Service
public class SelfCallServiceImpl implements SelfCallService{
    //注入ApplicationContext对象
    @Autowired
    //(1)
    private ApplicationContext applicationContext;
    //(2)
    private SelfCallService selfCallService;
    //(3)
    //在所有属性被设置完值之后被调用(在Spring容器的声明周期中也只会被调用一次)
    //也可以通过实现InitializingBean接口，实现afterPropertiesSet方法 如果是使用XML配置的话，也可以通过指定init-method的方式
    //执行顺序PostConstruct-->afterPropertiesSet-->init-method
    @PostConstruct
    public void setSelfCallService(){
        selfCallService = applicationContext.getBean(SelfCallServiceImpl.class);
    }
    /**
     * 方法A
     */
    public void selfCallA() {
        //第二种方式 从上下文中获取被代理的对象 标号为(1)、(2)、(3)、(4)的就是第二种实现自我调用的方式
        //这种方式的缺点是：不能解决scope为prototype的bean。
        //(4)
        selfCallService.selfCallB();
    }
    /**
     * 方法B
     */
    public void selfCallB() {
        System.out.println("我是方法B");
    }
}
```
在指定初始化方法这里我们使用了注解的方式，即指定了@PostConstruct这个注解，注意这个注解是JDK提供的，不是Spring提供的。PS：指定初始化方法我们最少有这样三种方式可以达到这样的效果。一：使用@PostConstruct注解；二：实现InitializingBean接口，实现afterPropertiesSet方法(不推荐，对代码的侵入性较强)；三：通过xml配置文件指定init-method的方式。这部分的内容属于Spring Bean生命周期的范围，会在下一篇文章中详细介绍。
效果和上面使用ThreadLocal暴露代理对象是一样的。
## 通过BeanPostProcessor的方式：
这一种方式需要我们定义一个接口，这个接口用来区分和设置被代理对象。具体的做法如下：
### 1、定义一个专门用来处理自我调用的Service：
```java
public interface SelfCallWrapperService {
    /**
     * 设置自我调用的对象
     * @param obj
     */
    void setSelfObj(Object obj);
}
```
### 2、定义一个类用实现BeanPostProcessor接口:
```java
public class BeanPostProcessorSelfCall implements BeanPostProcessor {
 
    public Object postProcessBeforeInitialization(Object bean, String s) throws BeansException {
 
        return bean;
    }
 
    public Object postProcessAfterInitialization(Object bean, String s) throws BeansException {
        if (bean instanceof SelfCallWrapperService) {
            ((SelfCallWrapperService)bean).setSelfObj(bean);
        }
        return bean;
    }
}
```
### 3、让自我调用的Service实现1中的接口：
```java
@Service
public class SelfCallServiceImpl implements SelfCallService, SelfCallWrapperService {
 
    private SelfCallService selfCall;
    /**
     * 方法A
     */
    public void selfCallA() {
        selfCall.selfCallB();
    }
    /**
     * 方法B
     */
    public void selfCallB() {
        System.out.println("我是方法B");
    }
    /**
     * 设置自我调用的对象
     *
     * @param obj
     */
    public void setSelfObj(Object obj) {
        selfCall = (SelfCallService)obj;
    }
}
```
BeanPostProcessor也是Spring Bean生命周期中的内容。同样在下一章会做介绍。
它的效果也是和是用ThreadLocal的方式一样的。
这三种方式各有特点：使用ThreadLocal的方式最为简单，并且各种情况都能适用；使用初始化方法的方式不能解决Bean为prototype的情景(或许配合lookup-method可能解决这个问题)，对于循环依赖的支持也可能会有问题；使用BeanPostProcessor的方式对循环依赖注入的支持会有问题。不过我们在项目中配置循环依赖的情况可能会比较少一些，Bean配置为prototype的情景可能会更少(我只在电票的项目中用过。。。。。)。正常情况下这三种方式我们都是可以正常使用的。
参考:
[http://jinnianshilongnian.iteye.com/blog/1487235](http://jinnianshilongnian.iteye.com/blog/1487235)
