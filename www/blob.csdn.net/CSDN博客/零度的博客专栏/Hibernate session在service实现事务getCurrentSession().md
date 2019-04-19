# Hibernate session在service实现事务getCurrentSession() - 零度的博客专栏 - CSDN博客
2016年12月15日 11:44:23[零度anngle](https://me.csdn.net/zmx729618)阅读数：4524
        我们调用Service层的一个方法的时候，它能够保证我们的这个方法中执行的所有的对数据库的更新操作保持在一个事务中，在事务层里面调用的这些方法要么全部成功，要么全部失败。那么事务的传播特性也是从这里说起的。如果你在你的Service层的这个方法中，除了调用了本类Dao层的方法之外，还调用了其他的Service方法，那么在调用其他的Service方法是需要解决这个问题：“事务是会传播的“。在Spring中有针对传播特性的多种配置，我们大多数情况下只用其中的一种:PROPGATION_REQUIRED，这个配置的意思是说当我调用service层的方法的时候开启一个事务(具体调用那一层的方法开始创建事务，要看你的aop的配置)，,那么在调用这个service层里面的其他的方法的时候,如果当前方法产生了事务就用当前方法产生的事务，否则就创建一个新的事务。这个工作使由Spring来帮助我们完成的。
       以前没有Spring帮助我们完成事务的时候我们必须自己手动的控制事务，例如当我们项目中仅仅使用hibernate，而没有集成进spring的时候，我们在一个service层中调用其他的[业务逻辑](https://www.baidu.com/s?wd=%E4%B8%9A%E5%8A%A1%E9%80%BB%E8%BE%91&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y1rHRzPH7hPWf4uW0zPW6s0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3En101rjc3PWbzPjbzrHTsn1cY)方法，为了保证事物必须也要把当前的hibernate
 session传递到下一个方法中，或者采用ThreadLocal的方法，将session传递给下一个方法，其实都是一个目的。现在这个工作由spring来帮助我们完成，就可以让我们更加的专注于我们的[业务逻辑](https://www.baidu.com/s?wd=%E4%B8%9A%E5%8A%A1%E9%80%BB%E8%BE%91&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y1rHRzPH7hPWf4uW0zPW6s0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3En101rjc3PWbzPjbzrHTsn1cY)。而不用去关心事务的问题。默认情况下当发生RuntimeException的情况下，事务才会回滚，所以要注意一下如果你在程序发生错误的情况下，有自己的异常处理机制定义自己的Exception，必须从RuntimeException类继承这样事务才会回滚！
       当我们在使用Hibernate作为数据库操作的类库时，我们一般在DAO层里与数据库相关的操作，把业务逻辑写在service层里。但是如果我们的项目比较小，那么直接在dao层里写事务也是可以的，这个就是看个人了，没有什么特别的规定。但是如果项目比较大，那么DAO应该只做单纯的数据库的操作，service写事务的操作，即整个业务逻辑。
例如：业务逻辑要求向数据库中的用户表增加一个用户，同时向日志表中加入一条日志，而这需要调用DAO的两个方法（UserDao的saveUser和LogDao的saveLog）。这显然是一个事务，也就是如果一个操作出现了问题，就要回滚到初始的状态。那么如何在Service层控制事务呢，本文就以此例的代码说明。
在DAO进行Session事务出现的问题
我们先看看在DAO层里写Hibernate的session的事务。
package com.xxg;
 import org.hibernate.SessionFactory;
 import org.hibernate.cfg.Configuration;
 public class HibernateUtil {
     private static final SessionFactory sessionFactory = buildSessionFactory();
     private static SessionFactory buildSessionFactory() {
         try {
             // Create the SessionFactory from hibernate.cfg.xml
             return new Configuration().configure().buildSessionFactory();
         }
         catch (Throwable ex) {
             // Make sure you log the exception, as it might be swallowed
             System.err.println("Initial SessionFactory creation failed." + ex);
             throw new ExceptionInInitializerError(ex);
         }
     }
     public static SessionFactory getSessionFactory() {
         return sessionFactory;
     }
 }
创建用户表T_user（id,username）和日志表T_log(id,content)，以及它们对应的实体类User、Log及映射文件，这里就不一一贴出代码。
public class UserDao {
     public void saveUser(User user){
         SessionFactory sessionFactory = HibernateUtil.getSessionFactory(); //获取SessionFactory
         Session session = sessionFactory.openSession();// openSession
         session.beginTransaction(); //开始事务
         session.save(user);
         session.getTransaction().commit(); //事务提交
        session.close(); //关闭session
     }
 }
 public class LogDao {
     public void saveLog(Log log){
         SessionFactory sessionFactory = HibernateUtil.getSessionFactory(); //获取SessionFactory
         Session session = sessionFactory.openSession();// openSession
         session.beginTransaction(); //开始事务
         session.save(log);
         session.getTransaction().commit(); //事务提交
        session.close(); //关闭session
     }
 }
接下来我们看看在service中写一个业务逻辑
public class TestService {
     public void save(User user){
         UserDao userDao = new UserDao();
         userDao.saveUser(user);
         LogDao logDao = new LogDao();
         Log log = new Log();
         log.setContent("插入一个用户");
         logDao.saveLog(log);
     }
 }
可以看到，我们在两个DAO里写了数据库的事务，代码中高亮显示了，session.beginTransaction()显示声明事务的开始。
这样写是不对的，因为这两个事情作为一个事务来进行的，会出现一个事务成功提交，而另外一个可能提交失败，导致不一致的情况，这样这两个操作不算是一个事务transaction，所以这么写就是一个失败的事务。
因此，我们要将事务在service中进行声明。
在service层写session的数据库事务
为了将事务放在service中，我们需要更改HibernateUtil的代码才能实现。否则使用上面的那个不能达到我们的需求。在这个新的HibernateUtil代码中，利用了ThreadLocal的线程内的局部变量来保存hibernate的session对象。这样就可以在不同的class中使用同一个session对象，而不用传递参数。
public class HibernateUtil {
     public static final ThreadLocal session = new ThreadLocal(([www.111cn.net](http://www.111cn.net)));
     public static final SessionFactory sessionFactory;
     static {
         try {
             sessionFactory = new Configuration().configure().buildSessionFactory();
         } catch ( Throwable ex ) {
             throw new ExceptionInInitializerError( ex );
         }
     }
     public static Session currentSession() throws HibernateException
     {
         Session s = session.get();
         if ( s == null )
         {
             s = sessionFactory.openSession();
             session.set( s );
         }
         return(s);
     }
     public static void closeSession() throws HibernateException
     {
         Session s = session.get();
         if ( s != null )
         {
             s.close();
         }
         session.set( null );
     }
 }
接下来，我们将事务放在service中。看代码：
public class TestService {
     public void save(User user){
         SessionFactory sessionFactory = HibernateUtil.getSessionFactory(); //获取SessionFactory
         Session session = sessionFactory.getCurrentSession();//getCurrentSession
         session.beginTransaction();//事务开始
         UserDao userDao = new UserDao();
         userDao.saveUser(user);
         LogDao logDao = new LogDao();
         Log log = new Log();
         log.setContent("插入一个用户");
         logDao.saveLog(log);
         session.getTransaction().commit();//事务提交
    }
 }
public class LogDao {
     public void saveLog(Log log) throws RuntimeException{
         SessionFactory sessionFactory = HibernateUtil.getSessionFactory(); //获取SessionFactory
         Session session = sessionFactory.getCurrentSession(); //getCurrentSession
         session.save(log);
         throw new RuntimeException();
     }
 }
 public class UserDao {
     public void saveUser(User user){
         SessionFactory sessionFactory = HibernateUtil.getSessionFactory(); //获取SessionFactory
         Session session = sessionFactory.getCurrentSession();//getCurrentSession
         session.save(user);
     }
 }
通过getCurrentSession()可以获得当前线程的session对象，通过它来进行共享session。这样事务就从service开始，然后再service结束。
getCurrentSession()与openSession()区别
getCurrentSession创建的session会和绑定到当前线程,而openSession不会。
getCurrentSession创建的线程会在事务回滚或事物提交后自动关闭,而openSession必须手动关闭
采用getCurrentSession()创建的session会绑定到当前线程中，而采用openSession()
创建的session则不会
* 采用getCurrentSession()创建的session在commit或rollback时会自动关闭，而采用openSession()
创建的session必须手动关闭
2、使用getCurrentSession()需要在hibernate.cfg.xml文件中加入如下配置：
* 如果使用的是本地事务（jdbc事务）
<property name=”hibernate.current_session_context_class”>thread</property>
 * 如果使用的是全局事务（jta事务）
<property name=”hibernate.current_session_context_class”>jta</property>
getCurrentSession()与openSession()关联
在 SessionFactory 启动的时候， Hibernate 会根据配置创建相应的 CurrentSessionContext ，在 getCurrentSession() 被调用的时候，实际被执行的方法是 CurrentSessionContext.currentSession() 。在 currentSession() 执行时，如果当前 Session 为空， currentSession 会调用 SessionFactory 的 openSession 。所以 getCurrentSession()
 对于 Java EE 来说是更好的获取 Session 的方法。
from:http://www.111cn.net/jsp/Java/85451.htm
