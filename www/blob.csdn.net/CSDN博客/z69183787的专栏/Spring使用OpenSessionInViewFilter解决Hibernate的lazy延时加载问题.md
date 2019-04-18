# Spring使用OpenSessionInViewFilter解决Hibernate的lazy延时加载问题 - z69183787的专栏 - CSDN博客
2014年03月21日 00:02:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1579
Spring为我们解决最让人头痛的难题之一，Hibernate的Session的关闭与开启问题。
当hibernate+spring配合使用的时候，如果设置了lazy=true,那么在读取数据的时候，当读取了父数据后，hibernate会自动关闭session，这样，当要使用子数据的时候，系统会抛出lazyinit的错误。
Hibernate 允许对关联对象、属性进行延迟加载，但是必须保证延迟加载的操作限于同一个 Hibernate Session范围之内进行。如果 Service 层返回一个启用了延迟加载功能的领域对象给 Web 层，当 Web层访问到那些需要延迟加载的数据时，由于加载领域对象的 Hibernate Session已经关闭，这些导致延迟加载数据的访问异常。而Spring为我们提供的OpenSessionInViewFilter过滤器为我们很好的解决了这个问题。
OpenSessionInViewFilter主要是保持Session状态知道request将全部页面发送到客户端，这样就可以解决延迟加载带来的问题。
如果应用中使用了OpenSessionInViewFilter或者OpenSessionInViewInterceptor，所有打开的session会被保存在一个线程变量里。在线程退出前通过OpenSessionInViewFilter或者OpenSessionInViewInterceptor断开这些session。为什么这么做？这主要是为了实现Hibernate的延迟加载功能。基于一个请求一个hibernate session的原则。
 它有两种配置方式OpenSessionInViewInterceptor和OpenSessionInViewFilter(具体参看SpringSide)，功能相同，只是一个在web.xml配置，另一个在application.xml配置而已。
spring中对OpenSessionInViewFilter的描述如下：
它是一个Servlet2.3过滤器，用来把一个Hibernate Session和一次完整的请求过程对应的线程相绑定。目的是为了实现"Open Session in View"的模式。
例如： 它允许在事务提交之后延迟加载显示所需要的对象。
这个过滤器和 HibernateInterceptor 有点类似：它是通过线程实现的。无论是没有事务的应用，还是有业务层事务的应用（通过HibernateTransactionManager 或
JtaTransactionManager的方式实现）它都适用。在后一种情况下，事务会自动采用由这个filter绑定的Session来进行相关的操作以及根据实际情况完成提交操作。
**如果使用struts2，此监听器应该在struts2的监听器前面**
<!--OpenSessionInViewFilter 解决延迟加载问题-->  
        <filter>  
                <filter-name>OpenSessionInViewFilter</filter-name>  
                <filter-class>  
                         org.springframework.orm.hibernate3.support.OpenSessionInViewFilter
                </filter-class>  
                <!-- singleSession默认为true,若设为false则等于没用OpenSessionInView -->  
                <init-param>  
                        <param-name>singleSession</param-name>  
                        <param-value>true</param-value>  
                </init-param>  
        </filter>  
        <filter-mapping>  
                <filter-name>OpenSessionInViewFilter</filter-name>  
                <url-pattern>/*</url-pattern>  
        </filter-mapping>  
对于OpenSessionInView的配置中，singleSession应该设置为true，表示一个request只能打开一个 session，如果设置为false的话，session可以被打开多个，这时在update、delete的时候会出现打开多个session的异常。但是当设置为true的时候，系统的性能会因为用户的网络状况受到影响，当request在生成页面完成后，session才会被释放，所以如果用户的网络状况比较差，那么连接池中的链接会迟迟不被回收，造成内存增加，系统性能受损。但是如果不用这种方法的话，lazy模式有体现不出它的优点，用？不用？两难啊。。。。。。
    尽管Open Session In View看起来还不错，其实副作用不少。看回上面OpenSessionInViewFilter的doFilterInternal方法代码，这个方法实际上是被父类的doFilter调用的，因此，我们可以大约了解的OpenSessionInViewFilter调用流程:**request(请求)->open session并开始transaction->controller->View(Jsp)->结束transaction并close session.**
     一切看起来很正确，尤其是在本地开发测试的时候没出现问题，但试想下如果流程中的某一步被阻塞的话，那在这期间connection就一直被占用而不释放。最有可能被阻塞的就是在写Jsp这步，一方面可能是页面内容大，response.write的时间长，另一方面可能是网速慢，服务器与用户间传输时间久。当大量这样的情况出现时，就有连接池连接不足，造成页面假死现象。
Open Session In View是个双刃剑，放在公网上内容多流量大的网站请慎用。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
由于OpenSessionInViewFilter把session绑在当前线程上，导致session的生命周期比事务要长，这期间所有事务性操作都在复用这同一个session，由此产生了一些“怪问题”：
例如出现如下错误：
     org.springframework.dao.InvalidDataAccessApiUsageException: Write operations are not allowed in read-only mode (FlushMode.NEVER) - turn your Session into FlushMode.AUTO or remove 'readOnly' marker from transaction definition
分析原因：OpenSessionInViewFilter 在把session绑在当前线程上的时候,会把session的flush mode 设为FlushMode.NEVER，因此，如果某个方法没有事务或者有只读事务，则不能对session做insert,update,delete操作，除非事先把session的flush mode手动设为auto
方案：
1、将singleSession设为false，这样只要改 web.xml，缺点是Hibernate Session的Instance可能会大增，使用的JDBC Connection量也会大增，如果Connection Pool的maxPoolSize设得太小，很容易就出问题。<!-- singleSession默认为true,若设为false则等于没用OpenSessionInView -->
2、在控制器中自行管理Session的FlushMode，麻烦的是每个有Modify的Method都要多几行程式
session.setFlushMode(FlushMode.AUTO);
session.update(user);
session.flush();
3、Extend OpenSessionInViewFilter，Override protected Session getSession(SessionFactory sessionFactory)，将FlushMode直接改为Auto。
4、让方法受Spring的事务控制（service和配置文件对应）
