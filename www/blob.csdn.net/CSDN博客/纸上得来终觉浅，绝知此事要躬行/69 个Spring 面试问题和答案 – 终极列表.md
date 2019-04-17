# 69 个Spring 面试问题和答案 – 终极列表 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 21:11:21[boonya](https://me.csdn.net/boonya)阅读数：1204标签：[javaee																[spring mvc](https://so.csdn.net/so/search/s.do?q=spring mvc&t=blog)](https://so.csdn.net/so/search/s.do?q=javaee&t=blog)
个人分类：[面试求职](https://blog.csdn.net/boonya/article/category/3093871)








这篇文章是关于Spring框架的一些最重要问题的总结，在面试或笔试过程中可能被问到! 不用担心您的下一个面试的的测试，因为Java Code Geeks会为您服务！您可能被问到的大部分问题都列在了这里。所有的核心模块, 从基础的Spring功能例如Spring Beans到 [Spring MVC](http://examples.javacodegeeks.com/enterprise-java/spring/mvc/spring-mvc-hello-world-example/) 框架都被列在了这里而且简单的介绍了。在查看了这些面试问题后，您可以查看我们的 [Spring教程页面](http://www.javacodegeeks.com/?page_id=16571)。那么，我们开始吧…！



## 目录
- [Spring 概述](http://www.javacodegeeks.com/zh-hans/2015/03/69-spring-%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#1)
- [依赖注入](http://www.javacodegeeks.com/zh-hans/2015/03/69-spring-%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#2)
- [Spring Beans](http://www.javacodegeeks.com/zh-hans/2015/03/69-spring-%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#3)
- [Spring 注解](http://www.javacodegeeks.com/zh-hans/2015/03/69-spring-%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#4)
- [Spring 数据访问](http://www.javacodegeeks.com/zh-hans/2015/03/69-spring-%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#5)
- [Spring 面向方面编程 (AOP)](http://www.javacodegeeks.com/zh-hans/2015/03/69-spring-%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#6)
- [Spring 模型视图控制器 (MVC)](http://www.javacodegeeks.com/zh-hans/2015/03/69-spring-%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#7)

## Spring 概述

### 1. Spring是什么?



Spring是针对[企业版 Java](http://www.javacodegeeks.com/tutorials/java-tutorials/enterprise-java-tutorials/)的开源的开发框架 。Spring框架的核心功能能被用于开发任何的Java程序，但有针对基于Java EE平台上的Web应用程序的扩展。Spring框架的目标是使JavaEE的开发更加容易并通过使用[基于POJO的编程模型](http://www.javacodegeeks.com/2012/09/how-to-write-better-pojo-services.html)来促进编程最佳实践的应用。

### 2. Spring 框架有什么优势?
- 轻量级: 在尺寸和透明度方面Spring 是轻量级的。spring框架的基础版本只有大约 2MB 大小。
- 控制反转 (IOC): 通过[控制反转技术](http://www.javacodegeeks.com/2011/08/what-is-dependency-inversion-is-it-ioc.html)，Spring实现了松耦合。对象间只是给出了依赖性，而不是创建或寻找依赖对象。
- 面向切面 (AOP): [Spring 支持面向切面编程](http://www.javacodegeeks.com/2011/01/aspect-oriented-programming-spring-aop.html) 而且把应用的业务逻辑从系统服务分离开了。
- 容器: Spring 包含并管理应用对象的生命周期和配置。
- MVC 框架: Spring的 web 框架是一个设计良好的 [web MVC 框架](http://www.javacodegeeks.com/2011/02/spring-mvc-development-tutorial.html), 它可以作为其他web框架的替代。
- 事务管理: Spring 提供了一个一致性的事务管理接口，它可以向下缩减为一个本地服务，也可以扩展为全局事务(JTA)。
- 异常处理: Spring 提供了方便的服务来翻译技术特定的异常(由JDBC，Hibernate或JTO抛出的)到一致性的，非检查的异常。

### 3.  Spring框架的模块都有哪些?

Spring 框架的基础模块都有:
- 核心模块
- Bean 模块
- 上下文模块
- 表达式语言模块
- [JDBC 模块](http://examples.javacodegeeks.com/enterprise-java/spring/jdbc/spring-jdbctemplate-example/)
- [ORM 模块](http://examples.javacodegeeks.com/enterprise-java/spring/jpaorm/spring-hibernate-mysql-and-maven-showcase/)
- OXM 模块
- Java 消息服务(JMS) 模块
- 事务模块
- Web 模块
- Web-Servlet 模块
- Web-Struts 模块
- Web-Portlet 模块

### 4. 解释核心容器(应用上下文)模块

这是Spring的基础模块，它提供了Spring框架的基本功能。`BeanFactory 是任何基于Spring的应用的核心。`Spring 框架建立在本模块上，它提供了Spring容器。

### 5. BeanFactory – BeanFactory 实现例子

BeanFactory是工厂模式的一个实现，它应用了控制反转来分离应用配置和来自实际应用代码的依赖。最常使用的BeanFactory实现类是XmlBeanFactory类。

### 6. XMLBeanFactory

最常用的是org.springframework.beans.factory.xml.XmlBeanFactory，它根据XML文件的定义来加载bean。容器从xml文件读取配置元数据，来创建一个完整配置的系统或应用。

### 7. 解释AOP 模块

AOP模块用于为使用Spring的应用来开发基于切面的应用。AOP联盟提供了很多的支持来保证[Spring 和其他AOP 框架](http://www.javacodegeeks.com/2014/02/applying-aspect-oriented-programming.html)的交互性。本模块也使用了元数据编程。

### 8. 解释JDBC 抽象和DAO 模块

使用 [JDBC 抽象和DAO](http://examples.javacodegeeks.com/enterprise-java/spring/jdbc/spring-jdbctemplate-example/)模块我们能确保数据库代码整洁和简单，并避免由于数据库资源关闭引起的问题。它提供了建立在数据库服务器错误消息之上的一层有意义的异常。它也使用了Spring的AOP模块来为Spring应用程序提供事务管理服务。

### 9. 解释对象/关系映射集成模块

Spring 也通过提供ORM模块来支持使用[对象/关系映射(ORM)](http://www.javacodegeeks.com/2011/12/persistence-layer-with-spring-31-and_14.html)工具而不是直接的JDBC 。 Spring为多种常见的ORM框架(包含[Hibernate](http://www.javacodegeeks.com/2010/05/jboss-42x-spring-3-jpa-hibernate.html), JDO, 和[iBATIS SQL Maps](http://www.javacodegeeks.com/2012/02/mybatis-3-spring-integration-tutorial.html))的集成提供支持。Spring的事务管理支持这些ORM框架中的每一种，也支持JDBC。

### 10. 解释web模块

[Spring web](http://examples.javacodegeeks.com/enterprise-java/spring/mvc/spring-mvc-hello-world-example/)模块建立在应用上下文模块之上，它提供了一个适合基于web的应用的上下文。本模块也包含对数种基于web的任务的支持，比如用于文件上传的multipart 的请求和对请求参数到业务对象的编程方式的绑定。它也包含了对Jakarta Struts的集成支持。

### 11. 解释Spring MVC模块

MVC 框架是由Spring为web应用提供的框架。Spring 能容易的集成到其他的MVC 框架，但是[Spring的 MVC](http://www.javacodegeeks.com/2012/09/spring-adding-spring-mvc-part-1.html)框架是更好的选择，因为它使用了IoC 来提供了控制器逻辑和业务对象之间的分离。使用Spring MVC，能为你的业务对象声明式的绑定请求参数。

### 12. Spring 配置文件

Spring 的配置文件是一个XML 文件。此文件包含了类信息并描述了这些类如何配置的和如何相互引用的。

### 13. 什么是Spring IoC容器?

Spring IoC容器负责创建对象，管理它们（使用依赖注入（DI）），装配它们，配置它们和管理它们的完整的生命周期。.

### 14. 使用IOC的好处有哪些?

ICO或依赖注入能减少一个程序中的代码量。它使得对程序的测试变得简单，因为在单元测试中不需要单例或JNDI查找。通过最小的努力和最小的侵入机制提高了松耦合。IOC容器支持服务的立即实例化或懒加载。

### 15. ApplicationContext的常见实现有哪些?

FileSystemXmlApplicationContext 容器从XML文件中加载bean的定义。XML bean的配置文件的完全路径必须提供给构造器。ClassPathXmlApplicationContext 容器也能从XML文件中加载bean的定义。这样，你必须合理的设置CLASSPATH ，因为容器会从CLASSPATH 中查找bean的定义。WebXmlApplicationContext容器为一个web应用加载拥有所有的bean的定义的XML文件。

### 16. Bean Factory 和ApplicationContext有什么区别?

应用上下文为解析文本消息提供了一种方法，一种加载文件资源（比如图片）的通用方法，它们可以给注册为监听器的bean发布事件。另外，必须通过bean 工厂的编程方式来处理的容器上的操作或容器中的bean，能够在应用上下文中以声明式的方式处理。应用上下文实现了MessageSource，一个用于获得本地消息的接口，它的实际实现是可插拔的。

### 17. Spring的应用有哪些特征?
- 用于定义功能的接口。
- 实现包含getter和setter方法，以及功能实现方法等。
- [Spring AOP](http://examples.javacodegeeks.com/enterprise-java/spring/aop/spring-aop-example/)
- Spring XML配置文件。
- 使用功能的客户端程序。



## 依赖注入

### 18. 什么是Spring中的依赖注入?

[依赖注入](http://www.javacodegeeks.com/2014/02/dependency-injection-options-for-java.html), 控制反转 (IoC)的一方面，是一个通用的概念， 可以用多种不同的方式来表达。它是指你不创建对象，而是描述它们应该如何被创建。你并不直接在代码中将组件和服务的联系写出来，而是在配置文件中描述哪些组件需要哪些服务。一个容器（IOC容器）然后就负责将它们全部关联起来。

### 19. 有哪些不同类型的IoC（依赖注入）?
- 基于构造器的依赖注入:基于构造器的依赖注入是通过调用拥有多个参数的类构造器来实现的，每一个参数都代表一个对其他来的依赖。
- 基于Setter的依赖注入: 基于Setter的依赖注入是在容器调用无参数的构造器或无参数的静态工厂方法来实例化一个bean后，然后调用bean方法上的setter方法来实现的。

### 20. 你推荐那种依赖注入，基于构造器的还是基于setter方法的?

两种依赖注入都可以使用。最好的方案是为强制的依赖使用构造器参数，而对可选的依赖使用setter方法。

## Spring Beans

### 21. 什么是 Spring beans?

[Spring Beans](http://examples.javacodegeeks.com/enterprise-java/spring/beans-spring/spring-3-bean-reference-example/)是构成一个Spring应用程序基础的java对象。 它们被Spring IoC容器实例化，组装并管理。这些bean是基于提供给容器的配置元信息创建的，例如，以XML的<bean/>标签定义的方式。Spring框架中定义的bean是单例化的bean。bean标签有一个名为”singleton”的属性，如果为true，bean就为单例的，如果设置为false，bean就是一个原型bean，这个属性默认为true，所以spring框架中的所有bean默认都是单例的。

### 22. Spring的bean的定义包含些什么?

Spring的bean的定义包含容器需要的如何创建bean，它的生命周期的细节和它的依赖的所有配置元数据。

### 23. 如何给Spring容器提供配置元数据?

有三种重要的方式来提供配置元数据给Spring容器:
- 基于XML的配置文件。
- 基于注解的配置。
- [基于Java的配置](http://examples.javacodegeeks.com/enterprise-java/spring/beans-spring/spring-3-java-config-example/)。

### 24. 如何定义一个bean的作用域?

当在Spring中定义<bean>时，我们也可以为bean声明作用域。它可以通过bean定义的scope属性来定义。例如，当Spring在每次需要一个bean时都要新建时，bean的scope属性就是prototype。相反的是，如果在每次需要一个bean时都返回同一个bean的实例，scope属性就必须为singleton。

### 25. 解释Spring所支持的bean的作用域

Spring框架支持如下五种作用域:
- singleton 作用域, Spring限制对于bean的定义，每一个Spring IoC容器只有一个实例。
- prototype 作用域, 一个bean的定义有多个对象实例。
- request 作用域, 一个bean为一个HTTP请求定义。这个作用域只有对web方面的Spring应用上下文起作用。
- session作用域, 一个bean为一个HTTP会话定义。这个作用域也只对web方面的Spring应用上下文起作用。
- global-session作用域, 一个bean定义对应一个全局的HTTP会话。这个作用域也只对web方面的Spring应用上下文起作用。

Spring的bean的默认作用域是`Singleton。`

### 26. Spring框架中的单例bean是否线程安全?

不安全。

### 27. 解释Spring框架中bean生命周期
- Spring容器从XML文件中查找bean的定义，然后实例化bean。
- Spring设置所有有bean定义所指定的属性。
- 如果bean实现了BeanNameAware接口，spring将bean的id发送到`setBeanName()方法。`
- 如果bean实现了BeanFactoryAware接口，spring发送beanfactory到setBeanFactory()方法。
- 如果有BeanPostProcessors与bean关联，spring调用postProcesserBeforeInitialization()方法。
- 如果bean实现了IntializingBean，它的`afterPropertySet()方法会被调用。如果bean有init方法的声明，指定的初始化方法会被调用。`
- 如果有BeanPostProcessors与bean关联，postProcessAfterInitialization()方法会被调用。
- 如果bean实现了DisposableBean，它会调用`destroy()方法。`

### 28. 哪些是重要的bean的生命周期方法？能够覆盖它们吗？

有两个重要的bean的生命周期方法。 第一个是setup，它在bean被加载进容器时被调用。第二个teardown方法，它在bean从容器中卸载时被调用。bean标签有两个重要的属性(`init-method` 和`destroy-method`) ，使用它们你就可以定义你自己的初始化和destroy 方法。它们也有相应的注解(`@PostConstruct` 和`@PreDestroy`)。

### 29. 什么是Spring中的内部bean？

当一个bean只是作为另一个bean的属性时，它可以被定义为有一个内部bean。Spring的基于XML的配置元数据提供了在bean定义的`<property/>` 或`<constructor-arg/>元素下`使用<bean/>元素的方式，来定义所谓的内部bean。内部bean都是匿名的，它们的作用域都是prototype。

### 30. 在Spring中如何注入一个Java集合?

Spring 提供了以下类型的[集合配置元素](http://examples.javacodegeeks.com/enterprise-java/spring/beans-spring/spring-collections-list-set-map-and-properties-example/):
- `<list>` 类型用于注入值的list，允许重复的值。
- `<set>` 类型用于写入值的set，不允许重复的值。
- `<map>` 类型用于注入键值对的集合，键和值可以是任意类型。
- `<props>` 类型用于注入键值对的集合，键和值都必须是String。

### 31. 什么是bean的装配（Wiring）？

装配（Wiring）, 或者说bean 的装配是Spring容器中一些bean联合起来使用。当装配bean的时候，Spring容器需要知道哪些bean是需要的，以及容器应该如何使用依赖注入来将它们联系在一起。

### 32. 什么是bean的自动装配（auto wiring）?

Spring 容器能够 [自动装配](http://examples.javacodegeeks.com/enterprise-java/spring/beans-spring/spring-autowire-example/)需要相互合作的bean之间的联系。这意味着可以自动地让Spring通过分析BeanFactory 的内容来解析一个bean的协作者（其他的bean），而不用使用 `<constructor-arg>` and `<property>元素。`

### 33. 解释自动装配的不同模式？

自动装配功能有五种模式，可以用这些模式指导Spring容器使用自动装配来依赖注入：
- no: 默认的设置，使用显示的bean引用来装配。
- byName: 当使用byName来自动装配时，Spring容器在XML配置文件中查找那些autowire 属性被设置为byName 的bean。然后它尝试匹配并使用配置文件中的同名bean来装配它们的属性。
- byType: 当使用datatype来自动装配时，Spring容器在XML配置文件中查找那些autowire 属性被设置为byType的bean。然后如果属性类型和配置文件中的bean的名称完全匹配，它就尝试匹配并装配这个属性。如果多个这样的bean存在，会抛出一个严重的异常。
- constructor: 这种模式与 `byType相似，` 但是 type 适用于 constructor 参数。如果容器中不存在具有构造器参数类型的bean，会抛出一个严重的异常。
- autodetect: Spring 首先尝试使用constructor来装配, 如果行不通, 它就会尝试使用byType来自动装配。

### 34. 自动装配有限制吗？

自动装配的限制有:
- 覆盖（Overriding）: 你仍然可以使用`<constructor-arg>` and `<property>来`指定依赖，这些设置总是会覆盖自动装配。
- 原始数据类型:你不能自动装配简单的属性，比如原始数据类型，字符串类型和Class类型。
- 迷惑性（Confusing nature）: 自动装配没有显示装配精确，所以如果可能的话尽量使用显示装配。

### 35. 在Spring中能够注入null和空字符串吗?

可以。

## Spring 注解

### 36. 什么是Spring基于Java的配置? 举一些注解的例子。

[基于Java的配置](http://www.javacodegeeks.com/2013/04/spring-java-configuration.html) 方式允许你不使用XML方式来配置，而是使用基于Java的注解。一个例子就是 `@Configuration注解`, 它表示这个类可以被Spring IoC容器作为bean定义的来源使用。另一个例子是`@Bean` 注解的方法，它会返回一个应该在Spring应用上下文中注册的对象。

### 37. 什么是基于注解的容器配置?

基于XML配置的一种替代方式是基于注解的配置，它依赖于字节码的元数据来装配组件，而不是基于标签的声明。不使用XML来描述bean的装配，开发者将配置搬到组件类中来对相关的类、方法和字段声明进行注解。

### 38. 如何打开注解装配?

注解装配在Spring中不是默认被打开的。要使用基于注解的装配，我们必须在Spring配置文件中通过配置`<context:annotation-config/>` 元素来打开。

### 39. @Required 注解

此注解只是表示被注解的bean的属性必须在配置时组装，是通过bean定义中一个显示的属性值 或自动装配来完成的。如果被注解的bean属性还没有被组装，容器就会抛出BeanInitializationException 。

### 40. @Autowired 注解

`@Autowired注解提供了更多对何处与如何完成自动装配的细粒度的控制` 。它可以被用来在setter方法上使用任意的名称和/或多个参数自动装配一个bean，就像`@Required` 注解在构造器上，在一个属性上，在pn方法上一样。

### 41. @Qualifier 注解

当有多个相同类型的bean存在，而仅有一个需要装配到一个属性上时，`@Qualifier` 注解被用来和@Autowired一起通过准确的指定哪一个bean会被装配来消除歧义。

## Spring数据访问

### 42. Spring框架中JDBC如何能被更高效的使用?

当使用Spring的JDBC框架时，资源管理和错误处理的负担减轻了。所以开发者仅需要写statement和查询来来操作数据库。在Spring框架提供的模板类 `JdbcTemplate` ([例子](http://examples.javacodegeeks.com/enterprise-java/spring/jdbc/spring-jdbctemplate-example/)).的帮助下，JDBC 能被更高效的使用。

### 43. JdbcTemplate

`JdbcTemplate` 类提供了许多便利的方法，可以用来将数据库数据转化为原始类型数据或对象，执行preparedstatements或 callablestatements以及提供自定义的数据库错误处理。

### 44. Spring DAO 支持

[Spring数据访问对象 (DAO) 支持](http://www.javacodegeeks.com/2012/09/spring-dao-and-service-layer.html)的目的是使数据访问技术比如JDBC，Hibernate或JDO以一种一致性的方式工作。这允许我们在不同的持久化技术之间相当容易的切换，而且在编码时不需要关心每一种技术特定的异常的捕获 。

### 45. 使用Spring如何访问Hibernate?

使用Spring有两种方式来访问Hibernate:
- 使用Hibernate模板（Template）和回调（Callback）来反转控制。
- `扩展HibernateDAOSupport` 并使用AOP拦截节点。

### 46. ORM的Spring支持

Spring 支持如下的ORM:
- Hibernate
- iBatis
- JPA (Java 持久化API)
- TopLink
- JDO (Java 数据对象)
- OJB

### 47. 如何使用HibernateDaoSupport来集成Spring和Hibernate?

使用Spring的SessionFactory 即LocalSessionFactory. 集成过程由三步组成:
- 配置Hibernate的SessionFactory
- 从HibernateDaoSupport扩展一个DAO实现
- 使用AOP装配事务支持

### 48. Spring支持的事务管理的类型

Spring支持两种的事务管理类型:
- 编程式事务管理: 意味着你使用编程的方式来管理事务。这给你很大的灵活性，但是维护困难。
- 声明式事务管理: 意味着你从业务代码分离了[事务管理](http://www.javacodegeeks.com/2011/09/spring-declarative-transactions-example.html)。仅仅使用注解或基于XML的配置来管理事务。

### 49. Spring框架的事务管理有哪些好处?
- 它提供了跨不同事务API（比如JTA，JDBC，Hibernate，JPA和JDO）的一致性的编程模型。
- 提供了一个简单的编程式事务管理的API，而不是一些复杂的事务管理API比如JTA。
- 它支持声明式事务管理。
- 它很好的集成了Spring的不同的数据访问抽象。

### 50. 哪一种事务管理类型更可取?

大多数的Spring框架的用户更倾向于声明式事务管理，因为这种方式对应用代码的影响最小，所以最符合非侵入式轻量级容器的理念。而声明式事务管理虽然优于编程式事务管理，但因为不允许使用代码来控制事务，声明式事务管理不如编程式事务管理灵活。

## Spring 面向切面编程 (AOP)

### 51. 解释 AOP

[面向切面编程](http://www.javacodegeeks.com/2014/02/applying-aspect-oriented-programming.html), 即AOP, 是允许程序员模块化切面关注的编程技术，或者是横切典型职责分区的行为，例如日志或事务管理。

### 52.切面

AOP的核心构造是切面，它封装了影响多个类的行为到可复用的模块。它是一个提供一些列横切需求的API的模块。比如，日志模块又被称为针对日志的AOP切面。一个应用根据需求的不同可能有任意多个的切面。在Spring AOP中，切面是使用@Aspect（`@AspectJ` 风格）注解来实现的普通类。

### 53. 在Spring AOP中，关注点和横切关注点有何区别？

关注点是一个程序中我们希望的一个模块的行为。一个关注点可能被定义为我们想实现的一个功能。横切关注点是在贯穿在程序中都适用的关注点，并且影响整个应用。比如，日志，[安全](http://www.javacodegeeks.com/2013/04/spring-aop-in-security-controlling-creation-of-ui-components-via-aspects.html)和数据通信都是应用中几乎每一个模块都需要的关注点，所以他们是横切关注点。

### 54. 连接点（ join point）

连接点（ join point）是指应用中可以插入AOP切面的一个点。它是应用中适用Spring AOP框架的动作执行的实际位置。

### 55. 通知（Advice）

通知是在方法调用前或后实际会被执行的动作。 这是在程序运行时由Spring AOP框架执行的实际代码。Spring切面可以与五种通知一起工作:
- before: 在方法执行前运行通知。
- after: 在一个方法执行后运行通知，而不用管它的结果。
- after-returning: 在一个方法执行后运行通知，仅在方法成功执行完成时生效。
- after-throwing: 在一个方法执行后运行通知，仅在方法抛出一个异常时生效。
- around: 在方法调用前后都被调用。

### 56. 切入点（Pointcut）

切入点（pointcut）是一系列的一个或多个连接点，在这些地方通知应该被执行。可以使用表达式或模式来指定切入点。

### 57. 什么是引入（ Introduction）?

引入（Introduction）允许我们向已有类中添加新方法或属性。

### 58. 什么是目标（Target）对象?

目标（ target） 对象是被一个或多个切面所通知的对象。它会始终都是代理对象。它也被称为被通知对象。

### 59. 什么是代理（ Proxy）?

代理是在应用通知到目标对象后产生的对象。当考虑客户端对象时，目标对象和代理对象是相同的。

### 60. 有哪些不同类型的自动代理（AutoProxying）?
- BeanNameAutoProxyCreator
- DefaultAdvisorAutoProxyCreator
- Metadata autoproxying

### 61. 什么是织入（Weaving）? 可以应用织入的不同点有哪些?

Weaving织入是连接切面和其他应用程序类型或对象连接起来并创建一个被通知（advised ）的对象的过程。织入可以在编译时，加载时或运行时完成。

### 62. 解释基于XML Schema的切面实现?

在这种实现中, 切面是用普通的类和基于XML的配置一起实现的。

### 63.解释基于注解(@AspectJ) 的切面实现。

这种实现指的是将切面声明为被Java5注解所修饰的普通类。

## Spring 模型 视图 控制器 (MVC)

### 64. 什么是Spring MVC框架?

Spring 拥有 [针对构建web应用的全功能的 MVC框架](http://examples.javacodegeeks.com/enterprise-java/spring/mvc/spring-mvc-hello-world-example/)。即使Spring能容易的与其他MVC框架集成，比如Struts，Spring的MVC框架使用IoC来提供一个清晰的控制逻辑与业务对象的分离。它也允许声明式的绑定请求参数到业务对象。

### 65. DispatcherServlet

Spring Web MVC框架是围绕一个处理所有HTTP请求和结果的DispatcherServlet 设计的。

### 66. WebApplicationContext

`WebApplicationContext`是普通的`ApplicationContext` 的一个扩展，它有一些web应用所必需的额外功能。它与一个普通的`ApplicationContext` 的区别是，它能解析主题（themes），而且它知道和那个servlet关联。

### 67. Spring MVC 框架中的控制器是什么?

控制器提供了对典型的由服务接口所定义的应用行为的访问。控制器解释用户输入并将它传送到模型，而模型是由视图展示给用户的。Spring以一种很抽象的方式实现控制器，这允许用户创建多种的控制器。

### 68. @Controller 注解

`@Controller` 注解表示一个特定的类的角色是控制器。Spring不需要你扩展任何的控制器基类或引用Servlet API。

### 69. @RequestMapping 注解

`@RequestMapping` 注解用于映射一个URL到一整个类或一个特定的处理方法。

好了, 现在你已经为面试做好了准备！不要忘记查看我们的 [Spring 教程](http://www.javacodegeeks.com/tutorials/java-tutorials/enterprise-java-tutorials/spring-tutorials/)! 如果您喜欢本文, 请[订阅我们的 newsletter](http://eepurl.com/xRlkj) 来获得每周的更新和补充白皮书！另外，关注 [JCG Academy](http://academy.javacodegeeks.com/) 来获得更多的培训资料! 欢迎您的评论，我们会将它包含在本文中！
|**Translated by: Qiuxiang Yang**|
|----|
|This post is a translation of [69 Spring Interview Questions and Answers – The ULTIMATE List](http://www.javacodegeeks.com/2014/05/spring-interview-questions-and-answers.html) from [Konstantina Dimtsa](http://www.javacodegeeks.com/author/theodora-fragkouli/)|

======================================读开涛博客总结===========================

Spring IOC管理容器：IoC是容器管理概念而非技术实指，它使用ApplicationContext和 ClassPathApplicationContext 加载xml配置，决定怎样管理创建Java bean,ApplicationContext继承自BeanFactory。

Spring DI依赖注入：采用对象组合方式，类与类之间只有依赖没有强制关系，提高了Bean的可复用性，实现对象动态注入，代码解耦，对象注入可以通过setter注入、方法注入、构造器注入，包括对属性方法的注入。

Spring AOP 面向切面编程：分离我们的组件，使组件完全不耦合，不违背开闭原则（扩展开发，修改关闭），可扩展模块，可针对类、方法、参数等进行有效切入，做到代码非入侵但又能实现类、方法、参数的关注，能够做到不同阶段切入处理。

Spring Resource：ByteArrayResource、InputStreamResource 、FileSystemResource 、UrlResource 、ClassPathResource、ServletContextResource、VfsResource

Spring JDBC:JdbcTemplate 简化对JDBC数据库的数据源访问封装。

Spring ORM：全称对象关系映射（Object/Relation Mapping），指将Java对象状态自动映射到关系数据库中的数据上，从而提供透明化的持久化支持，即把一种形式转化为另一种形式。ORM框架如：Hibernate,Mybatis。

Spring Transaction: ACID（原子性，一致性，隔离性，持久性）。




