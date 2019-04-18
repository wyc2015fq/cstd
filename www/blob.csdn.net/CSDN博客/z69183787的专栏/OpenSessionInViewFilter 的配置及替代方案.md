# OpenSessionInViewFilter 的配置及替代方案 - z69183787的专栏 - CSDN博客
2013年12月05日 09:48:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2076
Spring 为我们提供了一个叫做 OpenSessionInViewFilter 的过滤器，他是标准的 Servlet Filter 所以我们把它按照规范配置到 web.xml 中方可使用。使用中我们必须配合使用 Spring 的 HibernateDaoSupport 来进行开发，也就是说，我们的dao层的类都要继承于 HibernateDaoSupport，从中由 Spring 来控制 Hibernate 的 Session 在请求来的时候开启，走的时候关闭，保证了我们访问数据对象时的稳定性。
　　1. 在 web.xml 中加入对应过滤器配置文件
Java代码  ![收藏代码](http://justsee.iteye.com/images/icon_star.png)
- <!-- Spring的OpenSessionInView实现 -->  
- <filter>  
-     <filter-name>openSessionInViewFilter</filter-name>  
-     <filter-class>        org.springframework.orm.hibernate3.support.OpenSessionInViewFilter  
-     </filter-class>  
- <!-- singleSession默认为true,若设为false则等于没用OpenSessionInView 。所以默认可以不写-->  
-      <init-param>  
-        <param-name>singleSession</param-name>  
-        <param-value>true</param-value>   
-      </init-param>   
- <!--   
- 指定org.springframework.orm.hibernate3.LocalSessionFactoryBean在spring配置文件中的名称,默认值为sessionFactory。 如果LocalSessionFactoryBean在spring中的名称不是sessionFactory,该参数一定要指定,否则会出现找不到sessionFactory的例外。所以默认可以不写  
- -->  
-  <init-param>  
-      <param-name>sessionFactoryBean</param-name>  
-    <param-value>sessionFactory</param-value>  
-   </init-param>   
- </filter>  
- <filter-mapping>  
-     <filter-name>openSessionInViewFilter</filter-name>  
-     <url-pattern>/*</url-pattern>  
- </filter-mapping>  
　2. 在我们访问持久层数据是使用 Spring 为我们的 HibernateDaoSupport 的支持，并使用其中的对应方法操作我们的持久层数据
Java代码  ![收藏代码](http://justsee.iteye.com/images/icon_star.png)
- import org.springframework.orm.hibernate3.support.HibernateDaoSupport;  
- 
- publicclass XxxDAO extends HibernateDaoSupport {  
- 
- publicvoid save(Xxx transientInstance) {  
- try {  
-             getHibernateTemplate().save(transientInstance);  
-         } catch (RuntimeException re) {  
- throw re;  
-         }  
-     }  
- }  
OpenSessionInViewFilter的主要功能是用来把一个Hibernate Session和一次完整的请求过程对应的线程相绑定。Open Session In View在request把session绑定到当前thread期间一直保持hibernate session在open状态，使session在request的整个期间都可以使用，如在View层里PO也可以lazy loading数据，如 ${ company.employees }。当View 层逻辑完成后，才会通过Filter的doFilter方法或Interceptor的postHandle方法自动关闭session。
　　很多人在使用OpenSessionInView过程中提及一个错误：
org.springframework.dao.InvalidDataAccessApiUsageException: Write operations are not allowed in read-only mode (FlushMode.NEVER) – turn your Session into FlushMode.AUTO or remove ‘readOnly’ marker
 from transaction definition
　　看看OpenSessionInViewFilter里的几个方法：
Java代码  ![收藏代码](http://justsee.iteye.com/images/icon_star.png)
- protectedvoid doFilterInternal(HttpServletRequest request,HttpServletResponse response,FilterChain filterChain)  
- throws ServletException, IOException {  
- 
- 　SessionFactory sessionFactory = lookupSessionFactory();   
- 　logger.debug("Opening Hibernate Session in OpenSessionInViewFilter");   
- 　Session session = getSession(sessionFactory);   
- 　TransactionSynchronizationManager.bindResource(   
- 　sessionFactory, new SessionHolder(session));   
- 
- try {  
- 
-     filterChain.doFilter(request, response);  
- 
-   }   
- finally {  
- 
- 　  TransactionSynchronizationManager.unbindResource(sessionFactory);   
-    logger.debug("Closing Hibernate Session in OpenSessionInViewFilter");   
-    closeSession(session, sessionFactory);  
- 
- 　}  
- 
- }  
Java代码  ![收藏代码](http://justsee.iteye.com/images/icon_star.png)
- protected Session getSession(SessionFactory sessionFactory)  
- 
- throws DataAccessResourceFailureException {  
- 
- Session session = SessionFactoryUtils.getSession(sessionFactory, true);  
- 
- 　 session.setFlushMode(FlushMode.NEVER);  
- 
- return session;  
- 
- }   
Java代码  ![收藏代码](http://justsee.iteye.com/images/icon_star.png)
- protectedvoid closeSession(Session session, SessionFactory sessionFactory)  
- 
- throws CleanupFailureDataAccessException {  
- 
- 　 SessionFactoryUtils.closeSessionIfNecessary(session, sessionFactory);  
- 
- }  
　可以看到OpenSessionInViewFilter在getSession的时候,会把获取回来的session的flush mode 设为FlushMode.NEVER。然后把该sessionFactory绑定到 TransactionSynchronizationManager，使request的整个过程都使用同一个session，在请求过后再解除该 sessionFactory的绑定，最后closeSessionIfNecessary根据该 session是否已和transaction绑定来决定是否关闭session。在这个过程中，若HibernateTemplate
 发现自当前session有不是readOnly的transaction，就会获取到FlushMode.AUTO Session，使方法拥有写权限。
Java代码  ![收藏代码](http://justsee.iteye.com/images/icon_star.png)
- publicstaticvoid closeSessionIfNecessary(Session session, SessionFactory  sessionFactory)   
- 
- throws CleanupFailureDataAccessException {   
- 
- if (session == null ||  
- 
- TransactionSynchronizationManager.hasResource(sessionFactory)) {  
- 
- return;   
- 
- }   
- 
- logger.debug("Closing Hibernate session");  
- 
- try {   
- 
- session.close();   
- 
- }   
- 
- catch (JDBCException ex) {  
- 
- // SQLException underneath 
- 
- thrownew CleanupFailureDataAccessException("Could not close Hibernate session", ex.getSQLException());   
- 
- }   
- 
- catch (HibernateException ex) {  
- 
- thrownew CleanupFailureDataAccessException("Could not close Hibernate session",  ex);   
- 
- }  
- 
- }   
- 
也即是，如果有不是readOnly的transaction就可以由Flush.NEVER转为Flush.AUTO,拥有 insert,update,delete操作权限，如果没有transaction，并且没有另外人为地设flush model的话，则doFilter的整个过程都是Flush.NEVER。所以受transaction保护的方法有写权限，没受保护的则没有。
解决：
采用spring的事务声明,使方法受transaction控制
Xml代码  ![收藏代码](http://justsee.iteye.com/images/icon_star.png)
- <beanid="baseTransaction"
- 
- class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean"
- 
- abstract="true">
- 
- <propertyname="transactionManager"ref="transactionManager"/>
- 
- <propertyname="proxyTargetClass"value="true"/>
- 
- <propertyname="transactionAttributes">
- 
- <props>
- 
- <propkey="get*">PROPAGATION_REQUIRED,readOnly</prop>
- 
- <propkey="find*">PROPAGATION_REQUIRED,readOnly</prop>
- 
- <propkey="load*">PROPAGATION_REQUIRED,readOnly</prop>
- 
- <propkey="save*">PROPAGATION_REQUIRED</prop>
- 
- <propkey="add*">PROPAGATION_REQUIRED</prop>
- 
- <propkey="update*">PROPAGATION_REQUIRED</prop>
- 
- <propkey="remove*">PROPAGATION_REQUIRED</prop>
- 
- </props>
- 
- </property>
- 
- </bean>
- 
- <beanid="userService"parent="baseTransaction">
- <propertyname="target">
- 
- <beanclass="com.phopesoft.security.service.impl.UserServiceImpl"/>
- 
- </property>
- 
- </bean>
- 
对于上例，则以save,add,update,remove开头的方法拥有可写的事务，如果当前有某个方法，如命名为importExcel()，则因没有transaction而没有写权限，这时若方法内有insert,update,delete操作的话，则需要手动设置flush model为Flush.AUTO,如
1  session.setFlushMode(FlushMode.AUTO);
2  session.save(user);
3  session.flush();
或可以
在web.xml中过滤器openSession修改初始参数：
Java代码  ![收藏代码](http://justsee.iteye.com/images/icon_star.png)
- <!-- 在项目中使用Spring+Hibernate的时候，会开启OpenSessionInViewFilter来阻止延迟加载的错误，但是在我们开启OpenSessionInViewFilter这个过滤器的时候FlushMode就已经被默认设置为了MANUAL，如果FlushMode是MANUAL或NEVEL,在操作过程中 hibernate会将事务设置为readonly，所以在增加、删除或修改操作过程中-->   
- lt;init-param>    
-  <param-name>flushMode</param-name>     
-  <param-value>AUTO</param-value>     
-  </init-param>  
　　从上述代码其实可以得到一些对我们的开发有帮助的结论： 
　　1）如果使用了OpenSessionInView模式，那么Spring会帮助你管理Session的开和关，从而你在你的DAO中通过HibernateDaoSupport拿到的getSession()方法，都是绑定到当前线程的线程安全的Session，即拿即用，最后会由Filter统一关闭。 
　　2）由于拿到的Hibernate的Session被设置了session.setFlushMode(FlushMode.NEVER); 所以，除非你直接调用session.flush()，否则Hibernate session无论何时也不会flush任何的状态变化到数据库。因此，数据库事务的配置非常重要。（我们知道，在调用org.hibernate.Transaction.commit()的时候会触发session.flush()）我曾经见过很多人在使用OpenSessionInView模式时，都因为没有正确配置事务，导致了底层会抛出有关FlushMode.NEVER的异常。
　　总结：
　　OpenSessionInView这个模式使用比较简单，也成为了大家在Web开发中经常使用的方法，不过它有时候会带来一些意想不到的问题，这也是在开发中需要注意的。 
　　1. 在Struts＋Spring＋Hibernate环境中，由于配置的问题导致的模式失效这个问题以前论坛曾经讨论过，可以参考一下下面这个帖子：[http://www.javaeye.com/topic/15057](http://www.javaeye.com/topic/15057)
　　2. OpenSessionInView的效率问题 
　　这个问题也有人在论坛提出过，Robbin曾经做过具体的测试，可以具体参考一下下面这个帖子： [http://www.javaeye.com/topic/17501](http://www.javaeye.com/topic/17501)
　　3. 由于使用了OpenSessionInView模式后造成了内存和数据库连接问题 
　　这个问题是我在生产环境中碰到的一个问题。由于使用了OpenSessionInView模式，Session的生命周期变得非常长。虽然解决了Lazy Load的问题，但是带来的问题就是Hibernate的一级缓存，也就是Session级别的缓存的生命周期会变得非常长，那么如果你在你的Service层做大批量的数据操作时，其实这些数据会在缓存中保留一份，这是非常耗费内存的。还有一个数据库连接的问题，存在的原因在于由于数据库的Connection是和Session绑在一起的，所以，Connection也会得不到及时的释放。因而当系统出现业务非常繁忙，而计算量又非常大的时候，往往数据连接池的连接数会不够。这个问题我至今非常头痛，因为有很多客户对数据连接池的数量会有限制，不会给你无限制的增加下去。 
　　4. 使用了OpenSessionInView模式以后取数据的事务问题 
　　在使用了OpenSessionInView以后，其实事务的生命周期比Session的生命周期来得短，就以为着，其实有相当一部分的查询是不被纳入到事务的范围内的，此时是否会读到脏数据？这个问题我至今不敢确认，有经验的朋友请指教一下。 
　　最后提一下OpenSessionInView模式的一些替代方案，
1
可以使用OpenSessionInViewInterceptor来代替这个Filter，此时可以使用Spring的AOP配置，将这个Interceptor配置到你所需要的层次上去。
在application.xml配置
Xml代码  ![收藏代码](http://justsee.iteye.com/images/icon_star.png)
- <!-- Spring的OpenSessionInView实现 -->
- <filter>
- <filter-name>openSessionInViewFilter</filter-name>
- <filter-class>
-         org.springframework.orm.hibernate3.support.OpenSessionInViewFilter  
- </filter-class>
- </filter>
- <filter-mapping>
- <filter-name>openSessionInViewFilter</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
2
使用最古老的Hibernate.initialize()方法进行初始化了。
感谢：
[http://www.cnblogs.com/children/archive/2010/05/01/1725419.html](http://www.cnblogs.com/children/archive/2010/05/01/1725419.html)
 OpenSessionInViewFilter作用及配置：[http://www.yybean.com/opensessioninviewfilter-role-and-configuration](http://www.yybean.com/opensessioninviewfilter-role-and-configuration)；
OpenSessionInView详解：[http://www.javaeye.com/topic/32001](http://www.javaeye.com/topic/32001)
[http://schnauzer.iteye.com/blog/160024](http://schnauzer.iteye.com/blog/160024)
