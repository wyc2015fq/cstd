# spring中的@Transactional注解 - Big Smile - CSDN博客
2017年08月06日 20:22:25[王啸tr1912](https://me.csdn.net/tr1912)阅读数：223
        今天验收代码的时候，坤哥给我们详细的讲解了一下@Transactional注解的使用方法，由此来总结一下这种可以多地方引用的注解是什么原理。
# 一、JPA和事务管理
        我们使用一般JPA规范进行数据查询管理的时候其实框架并没有为我们提供事务管理，也就是JPA本身并不提供任何类型的声明式事务管理。如果在依赖注入容器之外使用JPA，事务处理必须由开发人员（我们自己）编程实现。
```java
UserTransaction utx = entityManager.getTransaction(); 
 
    try { 
        utx.begin(); 
 
        businessLogic();
 
        utx.commit(); 
    } catch(Exception ex) { 
        utx.rollback(); 
        throw ex; 
    }
```
       这种方式的事务管理使事务范围可以在代码中很清晰地表达出来，但它有以下缺点：
- 容易出现重复代码和错误。
- 任何错误可能产生较大的影响。
- 错误难以调试和复现。
- 降低了代码库的可读性。
- 如果该方法调用了其他的事务方法则很难处理事务。
# 二、@Transactional基于spring   
         我们都知道，这个@Transactional标签是在spring的框架中自带的，所以在研究这个注解之前我们需要先了解一下spring都包括什么。
         整个spring框架包含如下特征：
- **轻量**——从大小与开销两方面而言Spring都是轻量的。完整的Spring框架可以在一个大小只有1MB多的JAR文件里发布。并且Spring所需的处理开销也是微不足道的。此外，Spring是非侵入式的：典型地，Spring应用中的对象不依赖于Spring的特定类。
- **控制反转**——Spring通过一种称作控制反转（IoC）的技术促进了低耦合。当应用了IoC，一个对象依赖的其它对象会通过被动的方式传递进来，而不是这个对象自己创建或者查找依赖对象。你可以认为IoC与JNDI相反——不是对象从容器中查找依赖，而是容器在对象初始化时不等对象请求就主动将依赖传递给它。
- **面向切面**——Spring提供了面向切面编程的丰富支持，允许通过分离应用的业务逻辑与系统级服务（例如审计（auditing）和事务（transaction）管理）进行内聚性的开发。应用对象只实现它们应该做的——完成业务逻辑——仅此而已。它们并不负责（甚至是意识）其它的系统级关注点，例如日志或事务支持。
- **容器**——Spring包含并管理应用对象的配置和生命周期，在这个意义上它是一种容器，你可以配置你的每个bean如何被创建——基于一个可配置原型（prototype），你的bean可以创建一个单独的实例或者每次需要时都生成一个新的实例——以及它们是如何相互关联的。然而，Spring不应该被混同于传统的重量级的EJB容器，它们经常是庞大与笨重的，难以使用。
- **框架**——Spring可以将简单的组件配置、组合成为复杂的应用。在Spring中，应用对象被声明式地组合，典型地是在一个XML文件里。Spring也提供了很多基础功能（事务管理、持久化框架集成等等），将应用逻辑的开发留给了你。
- **MVC**——Spring的作用是整合，但不仅仅限于整合，Spring 框架可以被看做是一个企业解决方案级别的框架。客户端发送请求，服务器控制器（由DispatcherServlet实现的)完成请求的转发，控制器调用一个用于映射的类HandlerMapping，该类用于将请求映射到对应的处理器来处理请求。HandlerMapping 将请求映射到对应的处理器Controller（相当于Action）在Spring
 当中如果写一些处理器组件，一般实现Controller 接口，在Controller 中就可以调用一些Service 或DAO 来进行数据操作 ModelAndView 用于存放从DAO 中取出的数据，还可以存放响应视图的一些数据。 如果想将处理结果返回给用户，那么在Spring 框架中还提供一个视图组件ViewResolver，该组件根据Controller 返回的标示，找到对应的视图，将响应response 返回给用户。
        我们用到的@Transactional就是很好的体现了这个里面的特性，包括IOC，面向切面和容器。了解了注解的同志们肯定都知道，这个是一个进行IOC配置的一个很好的方法，并且通过注解，加上了类似于MVC.net中拦截器的效果的东西，在spring中这个就是利用AOP和IOC容器统一构造的一个可以进行事务控制的一个AOP程序，只要我们在类或者是一个方法上面加上了@Transactional这个注解，那么我们就相当于在这个方法上加上了事务开和事务关以及事务处理的一系列代码。
# 三、Transactional
       关于@Transactional，关键点之一是要考虑两个独立的概念，它们都有各自的范围和生命周期：
- persistence context(持久化上下文)
- database transaction（事务）
        @Transactional本身定义了单个事务的范围。这个事务在persistence context的范围内。
        持久化上下文仅仅是一个同步对象，它记录了有限集合的Java对象的状态，并且保证这些对象的变化最终持久化到数据库。
        这里需要注意的是，我们在这里想要触发事务，必须触发整个Exception的执行，如果你在try catch了一个Exception之后又throw了一个Exception，那么位于本方法的事务是不会被触发的。
        还有一点，如果你的类中有两个方法，并且他们是调用与被调用的关系的话，那么处于被调用一方的@Transactional事务是不会被触发的，这一点需要注意。
```java
public class tran{
      @Transactional
      public int a(){
             return b();
      }
      @Transactional
      public int b(){
      }
}
```
          这样的话b方法中的@Transactional是不会被触发的。
