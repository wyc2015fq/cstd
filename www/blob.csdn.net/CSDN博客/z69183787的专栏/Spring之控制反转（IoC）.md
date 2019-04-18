# Spring之控制反转（IoC） - z69183787的专栏 - CSDN博客
2012年11月24日 20:04:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：990
个人分类：[Spring-知识](https://blog.csdn.net/z69183787/article/category/2175163)
**一、IoC概念**
IoC(Inversion of Control)：中文译为控制反转。
IOC的基本概念是：不创建对象，但是描述创建它们的方式。在代码中不直接与对象和服务连接，但在配置文件中描述哪一个组件需要哪一项服务。容器负责将这些联系在一起。
　　其原理是基于OO设计原则的The Hollywood Principle：Don't call us, we'll call you（别找我，我会来找你的）。也就是说，所有的组件都是被动的（Passive），所有的组件初始化和调用都由容器负责。组件处在一个容器当中，由容器负责管理。
　　简单的来讲，就是由容器控制程序之间的关系，而非传统实现中，由程序代码直接操控。这也就是所谓“控制反转”的概念所在：控制权由应用代码中转到了外部容器，控制权的转移，是所谓反转。
**二、Spring IoC**
         Spring IoC容器实现了IoC设计模式，对外提供两个接口实现对Spring IoC容器的访问：
         1. BeanFactory:位于org.springframework.beans.factory包中。借助于配置文件实现对JavaBean的配置和管理。主要应用于开发Java应用。
         2. ApplicationContext：位于org.springframework.context包中。继承于BeanFactory，增加了IoC集成、处理消息资源、事件等功能。主要用于开发J2EE应用，是Spring推荐使用的接口。
         要获得appcontext.xml中定义的JavaBean，应用代码只需要调用BeanFactory的getBean方法即可。
**Bean的生命周期**         IoC容器定义了Spring配置文件中JavaBean应遵循的规则，即Bean定义（BeanDefinition）。任何处于IoC容器控制下的JavaBean的生命周期都存在4个阶段：
         1.实例化JavaBean。
         2.JavaBean实例的初始化，即通过IoC注入其依赖性。这一阶段将完成JavaBean实例的初始化。
         3.基于Spring应用对JavaBean实例的使用。
         4.Ioc容器销毁JavaBean实例。
**     Bean的创建**
         声明方式：<bean name="fileHelloWorld"  class="testspring.sample.HelloWorld"/>
        可用参数：
              factory-method：声明创建JavaBean的工厂方法。
              factory-bean：声明工厂类。
              <ref>：引用其它资源（bean实例）。
        初始化JavaBean
        1.可以使用autowire属性，使Spring自动将目标JavaBean需要注入的JavaBean找到并注入进来。
        2.使用dependency-check属性，保证各个Spring配置文件中各个JavaBean之间的相互关系。
        3.借助setter方法，将JavaBean的属性值注入进来。这些属性值可以使Java原型、对象类型、其它JavaBean、null等。
        4.如果JavaBean实现了org.springframework.beans.factory.BeanFactoryAware接口，则还需要调用setBeanFactory方法。
        5.Spring框架提供了若干接口，供开发者改变配置在BeanFactory中JavaBean的行为时使用。比如InitializingBean接口。BeanFactory初始化JavaBean时，会调用实现了InitializingBean接口的JavaBean中的aferPropertiesSet()方法。
        6.通过在<bean>元素中包含init-method属性能够达到同InitializingBean一样的目的。即初始化时会自动调用init-method中定义的方法。
        注意：如果5和6两者同时使用，Spring首先执行5中的aferPropertiesSet()方法，然后执行init-method中定义的方法。
**使用JavaBean**
         一旦Spring创建并初始化JavaBean实例后，应用就可以使用JavaBean实例了。
**销毁JavaBean**
         一旦将基于Spring的（Web）应用停止，Spring框架将调用那些JavaBean实例中存在的生命周期方法，比如实现了DisposableBean接口的JavaBean，或者在Spring配置文件中指定了destroy-method苏醒的JavaBean。
         注意：
          1.上述内容只适合于通过“singleton”方式创建的JavaBean实例。对于以“prototype”方式创建的JavaBean实例，Spring并不能够控制其生命周期。
          2.JavaBean创建时间。对于BeanFactory而言，在用户调用getBean()方法时创建JavaBean实例。而对于ApplicationContext而言，一旦ContextLoaderServlet或ContextLoaderListener初始化成功，所有JavaBean实例都将会被创建。
