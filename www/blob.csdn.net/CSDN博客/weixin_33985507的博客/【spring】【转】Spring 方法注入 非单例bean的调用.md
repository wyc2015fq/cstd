# 【spring】【转】Spring 方法注入 非单例bean的调用 - weixin_33985507的博客 - CSDN博客
2013年04月01日 12:15:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
http://flysnow.iteye.com/blog/733785#
引用
在大部分情况下，容器中的bean都是singleton类型的。如果一个singleton bean要引用另外一个singleton bean，或者一个非singleton bean要引用另外一个非singleton bean时，通常情况下将一个bean定义为另一个bean的property值就可以了。不过对于具有不同生命周期的bean来说这样做就会有问题了，比如在调用一个singleton类型bean A的某个方法时，需要引用另一个非singleton（prototype）类型的bean B，对于bean A来说，容器只会创建一次，这样就没法在需要的时候每次让容器为bean A提供一个新的的bean B实例
对于上面的问题Spring提供了三种解决方案：
- 放弃控制反转。通过实现ApplicationContextAware接口让bean A能够感知bean
容器，并且在需要的时候通过使用getBean("B")方式向容器请求一个新的bean B实例。
- 
Lookup方法注入。Lookup方法注入利用了容器的覆盖受容器管理的bean方法的能力，从而返回指定名字的bean实例。
- 自定义方法的替代方案。该注入能使用bean的另一个方法实现去替换自定义的方法。
这里只说前两种方案的实现，第三种方案因为不常用，略过不提，有兴趣的可以了解一下。
**一：实现环境**
- Eclipse3.4
- JDK1.5
- Spring3.0.3
- Junit 4测试框架
- 
依赖jar有log4j-1.2.16.jar，commons-logging-api-1.1.1.jar，cglib-nodep-2.2.jar。
**二：通过实现ApplicationContextAware接口以编程的方式实现**
ApplicationContextAware和BeanFactoryAware差不多，用法也差不多，实现了
ApplicationContextAware接口的对象会拥有一个ApplicationContext的引用，这样我们就可以已编程的方式操作
ApplicationContext。看下面的例子。
```
1 package com.flysnow.injection;
 2 
 3 import org.springframework.beans.BeansException;
 4 import org.springframework.context.ApplicationContext;
 5 import org.springframework.context.ApplicationContextAware;
 6 
 7 import com.flysnow.injection.command.Command;
 8 
 9 /**
10  * 命令管理器
11  * @author 飞雪无情
12  *
13  */
14 public class CommandManager implements ApplicationContextAware {
15     //用于保存ApplicationContext的引用，set方式注入
16     private ApplicationContext applicationContext;
17     //模拟业务处理的方法
18     public Object process(){
19         Command command=createCommand();
20         return command.execute();
21     }
22     //获取一个命令
23     private Command createCommand() {
24         return (Command) this.applicationContext.getBean("asyncCommand"); //
25     }
26 
27     public void setApplicationContext(ApplicationContext applicationContext)
28             throws BeansException {
29         this.applicationContext=applicationContext;//获得该ApplicationContext引用
30     }
31 
32 }
```
下面定义Command接口和其实现类AsyncCommand。
```
1 package com.flysnow.injection.command;
 2 
 3 /**
 4  * 一个命令接口
 5  * @author 飞雪无情
 6  *
 7  */
 8 public interface Command {
 9     /**
10      * 执行命令
11      * @return
12      */
13     public Object execute();
14 }
```
```
1 package com.flysnow.injection.command;
 2 
 3 /**
 4  * 一个异步处理命令的实现
 5  * @author 飞雪无情
 6  *
 7  */
 8 public class AsyncCommand implements Command {
 9 
10     /* (non-Javadoc)
11      * @see com.flysnow.lookup.command.Command#execute()
12      */
13     public Object execute() {
14         //返回自身实例，是为了测试的时候好看出每次返回的不是同一个实例
15         return this;
16     }
17 
18 }
```
Bean配置文件如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans-3.0.xsd">
        <!-- 通过scope="prototype"界定该bean是多例的 -->
        <bean id="asyncCommand" class="com.flysnow.injection.command.AsyncCommand" scope="prototype"></bean>
        <bean id="commandManager" class="com.flysnow.injection.CommandManager">
        </bean>
</beans>
```
以上主要是单例Bean commandManager的process()方法需要引用一个prototype(非单例)的bean，所以在调用process的时候先通过 createCommand方法从容器中取得一个Command，然后在执行业务计算，代码中有注释，很简单。 
测试类如下：
```
package com.flysnow.injection;
import org.junit.Before;
import org.junit.Test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import com.flysnow.injection.CommandManager;
public class TestCommandManager {
    private ApplicationContext context;
    @Before
    public void setUp() throws Exception {
        context=new ClassPathXmlApplicationContext("beans.xml");
    }
    @Test
    public void testProcess() {
        CommandManager manager=context.getBean("commandManager", CommandManager.class);
        System.out.println("第一执行process,Command的地址是:"+manager.process());
        System.out.println("第二执行process,Command的地址是:"+manager.process());
    }
}
```
可以通过控制台输出看到两次的输出借中的Command的地址是不一样的，因为我们为asyncCommand配置了scope="prototype" 属性，这种方式就是使得每次从容器中取得的bean实例都不一样。通过这样方式我们实现了单例bean(commandManager)中的方法 (process方法)引用非单例的bean(asyncCommand)。虽然我们实现了，但是这不是一种好的方法，因为我们的业务代码和Spring Framework产生了耦合。下面介绍Spring提供的另外一种干净的实现方式，就是Lookup方法注入。 
**三：通过Lookup方法注入来实现**
使用这种方式很简单，因为Spring已经为我们做了很大一部分工作，我们要做的就是bean配置和业务类。
- 首先修改CommandManager类为abstract的，修改createCommand方法也为abstract的。
- 去掉ApplicationContextAware的实现及相关set方法和applicationContext变量定义
- 修改bean配置文件，在commandManager Bean中增加<lookup-method name="createCommand" bean="asyncCommand"/>。
- 其他保持不变
修改后的CommandManager和bean配置文件如下：
```
1 public abstract class CommandManager {
2     //模拟业务处理的方法
3     public Object process(){
4         Command command=createCommand();
5         return command.execute();
6     }
7     //获取一个命令
8     protected abstract Command createCommand();
9 }
```
```
<bean id="commandManager" class="com.flysnow.injection.CommandManager">
            <lookup-method name="createCommand" bean="asyncCommand"/>
        </bean>
```
运行测试，控制台打印出的两个Command的地址不一样，说明我们实现了。 
<lookup-method>标签中的name属性就是commandManager 
Bean的获取Command实例(AsyncCommand)的方法，也就createCommand方法，bean属性是要返回哪种类型的
Command的，这里是AsyncCommand。
这里的createCommand方法就成为被注入方法，他的定义形式必须为：
```
<public|protected> [abstract] <return-type> theMethodName(no-arguments);
```
被注入方法不一定是抽象的，如果被注入方法是抽象的，动态生成的子类(这里就是动态生成的CommandManager的子类)会实现该方法。否则，动态 生成的子类会覆盖类里的具体方法。为了让这个动态子类得以正常工作，需要把CGLIB的jar文件放在classpath里，这就是我们引用cglib包 的原因。还有，Spring容器要子类化的类(CommandManager)不能是final的，要覆盖的方法(createCommand)也不能是 final的。 
**四：小结**
Lookup方法注入干净整洁，易于扩展，更符合Ioc规则，所以尽量采用这种方式。
