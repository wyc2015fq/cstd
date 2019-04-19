# J2EE—JTA的认识和@Transactional - Big Smile - CSDN博客
2017年02月12日 16:37:36[王啸tr1912](https://me.csdn.net/tr1912)阅读数：978
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        小编最近学习了SSH网上商城的诸多内容，回头看看，其实里面也用了很多的J2EE的东西，所以借此机会也来分享一下有关于spring事务和JTA事务的一些事情。
# 一、JTA的定义
        JTA，即Java Transaction API，JTA允许应用程序执行分布式事务处理——在两个或多个网络计算机资源上访问并且更新数据。JDBC驱动程序的JTA支持极大地增强了数据访问能力。
        一般JTA都是应用在了连接数据库查询的地方，和框架并没有什么关系了，主要应用于类似我们在学习.net时候用的sqlHelper中的辅助方法，因为要事先生成Transaction的对象然后才可以进行操作，所以在书写上需要俩联系才可以。
# 二、JTA与JDBC
      JTA事务比JDBC事务更强大。一个JTA事务可以有多个参与者，而一个JDBC事务则被限定在一个单一的数据库连接。下列任一个Java平台的组件都可以参与到一个JTA事务中：JDBC连接、JDO PersistenceManager 对象、JMS 队列、JMS 主题、企业JavaBeans（EJB）、一个用J2EE Connector Architecture 规范编译的资源分配器。    
      所以由上面可知JTA技术一般用于多数据库（服务端）的操作或者是服务的多参与者的操作。最好的例子莫过于银行转账这个经典的例子了：
     将用户 A 账户中的 500 元人民币转移到用户 B 的账户中，其操作流程如下 
       1. 将 A 账户中的金额减少 500
       2. 将 B 账户中的金额增加 500
        这两个操作必须保正 ACID 的事务属性：即要么全部成功，要么全部失败；假若没有事务保障，用户的账号金额将可能发生问题：
        假如第一步操作成功而第二步失败，那么用户 A 账户中的金额将就减少 500 元而用户 B 的账号却没有任何增加（不翼而飞）；同样如果第一步出错 而第二步成功，那么用户 A 的账户金额不变而用户 B 的账号将增加 500 元（凭空而生）。上述任何一种错误都会产生严重的数据不一致问题，事务的缺失对于一个稳定的生产系统是不可接受的。
# 三、Spring中的@Transactional
        在敲SSH网上商城项目的时候，里面的构建完了一条线之后，老师在service层的类的定义上面加上了@Transactional 这个标签，同时整个类引用了  import org.springframework.transaction.annotation.Transactional;
这个包，所以这个是Spring自己带的事务的方法，那么他和JTA有什么不同？为什么要写在Service层呢？
        其中我们要了解的是Spring中的这个事务总的来说是对于逻辑判断之后的操作的一个事务，尤其是针对于数据操作。而且Service层就是我们整个框架中的逻辑判断层，所以只有进行完了逻辑判断的操作后，才能允许提交事务操作，故@Transactional需要写在Service层才能够发挥作用。如银行转账的例子，如果从A处没有扣钱的话，就不会执行向B添加钱的操作了，而service层正是进行这个判断的地方。
        那么写在DAO层会有什么效果呢？直接的效果就是网站在Tomcat上面部署都会出现错误，如图在userDao添加了这个标签之后错误：
![](https://img-blog.csdn.net/20170212162339972?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
16:21:44,970 ERROR ContextLoader:307 - Context initialization failed
org.springframework.beans.factory.BeanCreationException: Error creating bean with name 'userService' defined in class path resource [applicationContext.xml]: Initialization of bean failed; nested exception is org.springframework.beans.ConversionNotSupportedException: Failed to convert property value of type '$Proxy7 implementing org.springframework.beans.factory.InitializingBean,org.springframework.aop.SpringProxy,org.springframework.aop.framework.Advised' to required type 'cn.itcast.shop.user.dao.UserDao' for property 'userDao'; nested exception is java.lang.IllegalStateException: Cannot convert value of type [$Proxy7 implementing org.springframework.beans.factory.InitializingBean,org.springframework.aop.SpringProxy,org.springframework.aop.framework.Advised] to required type [cn.itcast.shop.user.dao.UserDao] for property 'userDao': no matching editors or conversion strategy found
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:532)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:461)
	at org.springframework.beans.factory.support.AbstractBeanFactory$1.getObject(AbstractBeanFactory.java:295)
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:223)
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:292)
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:194)
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.preInstantiateSingletons(DefaultListableBeanFactory.java:607)
	at org.springframework.context.support.AbstractApplicationContext.finishBeanFactoryInitialization(AbstractApplicationContext.java:932)
	at org.springframework.context.support.AbstractApplicationContext.refresh(AbstractApplicationContext.java:479)
	at org.springframework.web.context.ContextLoader.configureAndRefreshWebApplicationContext(ContextLoader.java:383)
	at org.springframework.web.context.ContextLoader.initWebApplicationContext(ContextLoader.java:283)
	at org.springframework.web.context.ContextLoaderListener.contextInitialized(ContextLoaderListener.java:112)
	at org.apache.catalina.core.StandardContext.listenerStart(StandardContext.java:5118)
	at org.apache.catalina.core.StandardContext.startInternal(StandardContext.java:5634)
	at org.apache.catalina.util.LifecycleBase.start(LifecycleBase.java:145)
	at org.apache.catalina.core.ContainerBase.addChildInternal(ContainerBase.java:899)
	at org.apache.catalina.core.ContainerBase.addChild(ContainerBase.java:875)
	at org.apache.catalina.core.StandardHost.addChild(StandardHost.java:652)
	at org.apache.catalina.startup.HostConfig.deployDirectory(HostConfig.java:1260)
	at org.apache.catalina.startup.HostConfig$DeployDirectory.run(HostConfig.java:2002)
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:441)
	at java.util.concurrent.FutureTask$Sync.innerRun(FutureTask.java:303)
	at java.util.concurrent.FutureTask.run(FutureTask.java:138)
	at java.util.concurrent.ThreadPoolExecutor$Worker.runTask(ThreadPoolExecutor.java:886)
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:908)
	at java.lang.Thread.run(Thread.java:619)
```
      其实就是因为在DAO层添加了事务反而让Spring不知道应该做什么了，导致了Spring中Dao层不能注入service层的文件，service层的类创建失败，其次事务出现问题都会有Failed to convert property value of type '$Proxy7 implementing org.springframework.    这句话出现，尤其是Proxy7这个东西。
# 四、总结
        Java中的事务和之前认识的事务有很大的区别，尤其是从数据库操作的事务转到整个业务逻辑操作的事务，不过虽然事务的作用域不同，但是事务的本质的特性是不会变的，尤其是三大特性和其容易出现问题的几个要素。并且从Spring事务和JTA事务的处理来看似乎本质上都是一样的东西。
