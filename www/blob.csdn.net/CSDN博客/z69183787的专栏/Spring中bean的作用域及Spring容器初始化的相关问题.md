# Spring中bean的作用域及Spring容器初始化的相关问题 - z69183787的专栏 - CSDN博客
2012年11月24日 21:25:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2609
**Spring中bean的作用域：**
        在Spring容器中，bean的作用域范围默认是单例模式的，singleton，单例模式可以由多个线程并发访问，如果
我们想改变bean的作用域范围，将其改变为多例模式，即每次从容器中获取相同的bean时，都是新的bean，而不
是获得两次相同的bean，其内存地址相同，为true。那么在配置文件中设置bean的作用域范围，Spring容器通过
scope="prototype"来将bean的作用域设置为多例模式。如下：
<bean id="userDAO" class="com.aptech.dao.impl.UserDAOImpl" scope="prototype"></bean>
**Spring容器初始化相关问题：**
        在Spring容器中，默认情况下，容器启动时初始化所有容器中的bean，但是我们可以指定bean节点的lazy-
init="true"来延迟初始化bean，这时只有在获取该bean时，才会初始化bean。如：
<bean id="userDAO" class="com.aptech.dao.impl.UserDAOImpl" lazy-init="true"></bean>
  如果想对所有Spring容器中的bean都应用延迟初始化，可以在根节点<beans/>设置default-lazy-
init="true".如下：
<beans default-lazy-init="true" />
        在Spring中，当bean的作用域范围是默认的单例模式时，bean会在Spring容器启动时就初始化，如果bean的作用域范围是多例模式的，bean的初始化是在调用该bean的实例时，由beanfactory初始化。如果想在容器启动时，延迟单例模式bean的初始化，则应该按照上一点的方法。
        如果想在bean被初始化的同时，让bean所对应的类的相应方法执行，比如：调用该类的初始化方法，或者打开某一数据库连接等，应该怎么办呢？在bean节点中为我们提供了init-method属性，该属性用来指定bean初始化时
，同时该bean所对应的类的某一方法同时执行，该属性的值是对应类中的某一方法。如下：<bean id="userDAO"
class="com.aptech.dao.impl.UserDAOImpl" init-method="getConn"></bean>
当然，如果我们想在gaibean销毁时，也同时执行gaibean对应类的某一方法，Spring也提供了一个属性：
destroy-method，该方法将在bean被销毁之前调用，该属性的值也是对应类的某一方法，如：
<bean id="userDAO" class="com.aptech.dao.impl.UserDAOImpl" destroy-method="closeAll"></bean>
        怎样测试bean在容器启动时是否初始化？因为大多数bean的创建都是通过类构造器来实例化的，所以在bean对应的类中的构造函数中输出一句话，之后创建一个Spring容器，看是否输出构造方法中的话，如果输出则证明在
Sping容器启动时bean就初始化了，如果没有输出则……
思考：bean什么时候被销毁呢？ 在Spring容器关闭的时候被销毁。
      那怎样关闭Spring容器呢？使用一个抽象类AbstractApplicationContext ，如下：
 AbstractApplicationContext context = new ClassPathXmlApplicationContext
("applicationContext.xml");
 // 关闭Spring容器
 context.close();
