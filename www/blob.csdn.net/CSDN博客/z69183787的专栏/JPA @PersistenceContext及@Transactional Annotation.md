# JPA @PersistenceContext及@Transactional Annotation - z69183787的专栏 - CSDN博客
2013年04月17日 13:05:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1784
JPA(*Java* Persistence API )也就是java储存数据API，提供了更为便捷的存储数据的接口，当然在进行一些复杂及查询操作时需要使用Java Persistence query language，这个和sql基本一样，只是需要遵循另一些语法规则，可以参考[http://download.oracle.com/javaee/5/tutorial/doc/bnbuf.html](http://download.oracle.com/javaee/5/tutorial/doc/bnbuf.html) 。
在使用JPA时，比如你的MySql中有定义了一个Lesson表格，现在想对Lesson表格进行添加一个记录，这时候你需要写的代码主要有两个：
   1、写一个Lesson.java，就是一个pojo啦，一个简单的java对象，里面包括了数据库中Lesson表格的所有字段，这里见Lesson.java中的属性名称同数据库的名称保存一致便ok了。然后再User.java类定义前面加上@Entity这个annotation，这样子相当标注了这个Lesson.java类将作为存储时的一个实体来对待。
   2、写一个存储接口，比如LessonServiceImpl.java，这个类里面需要有一个javax.persistence.EntityManager类的属性，比如命名为em，这样相应的数据操作就可以通过这个em来实现，比如要添加一天记录，只需要调用em.persist。更新时em.merge，删除时em.remove。JPA考虑到这些通常的应用因此设计了这些简单的接口方便操作。下面是该类的部分代码
@Transactional
public class LessonServiceImpl implements LessonService {
    private EntityManager em;
    @PersistenceContext
    public void SetEntityManager(EntityManager em)
    {        
        this.em = em;
    }   
    public void remove(int id) {
        // TODO Auto-generated method stub
        Lesson lesson = em.find(Lesson.class, id);
        if(lesson != null)
        {
            em.remove(lesson);
        }
    }
.............
当然需要注意到的是存储的接口LessonServiceImpl.java在设计的时候通常是作为singleton的类型，因此通常是在类工厂里面将来。而且通常LessonServiceImpl.java中的那个EntityManager对象是通过依赖注入的方式在类工厂中直接设置。比如下面的代码spring factory中的例子
................
<bean id="LessonService" class="quickstart.service.LessonServiceImpl" />
<bean id="entityManagerFactory" class="org.springframework.orm.jpa.LocalContainerEntityManagerFactoryBean">
        .......................
</bean>
<bean id="transactionManager"
        class="org.springframework.orm.jpa.JpaTransactionManager">
        <property name="entityManagerFactory" ref="entityManagerFactory" />
</bean>
<tx:annotation-driven transaction-manager="transactionManager" />
...................
这里使用的不是纯粹的JPA，而是使用hibernate-jpa-2.0-api-1.0.0.Final.jar中的JPA。对LessonServiceImpl.java这个类的定义前加入@Transactional,表示这个类需要进行事务管理，而使用的事务则是通过<tx:annotation-driven />来指定的，也就是LessonServiceImpl要通过这个类工厂中定义的transactionManager来进行数据的操作时的事务处理，可以参考[http://static.springsource.org/spring/docs/2.0.x/reference/transaction.html#transaction-declarative](http://static.springsource.org/spring/docs/2.0.x/reference/transaction.html#transaction-declarative) 。
定义一个set函数这个函数传递进一个EntityManager的类参数，通过这个接口类工厂才可以将LessonServiceImpl需要EntityManager由类工厂中注入进去，然后这个set函数需要加上@PersistentContext这个annotation，表示这个就是注入存储上下文对象的接口，可以参考[http://download.oracle.com/javaee/5/tutorial/doc/bnbqw.html](http://download.oracle.com/javaee/5/tutorial/doc/bnbqw.html) 。
当然另外我想最简单清楚表现JPA使用的方法可能是这样，采取直接注入的方式：
<bean id="LessonService" class="quickstart.service.LessonServiceImpl" >
<property name="entityManager" ref="entityManager"/>
</bean>
但是这样一个问题就是所有的访问数据的接口都要注入一次。所以这里通过AOP的方法及结合annotation的方法（常用的是xml方式）来进行依赖注入。对于事务处理通过@Transactional来实现的，所以这里如果去掉@Transactional则相当于去掉事务管理，会导致只能对数据进行查询操作无法删除或更新；而对于管理entity则通过@PersistentContext，这里类工厂会把名称为entityManagerFactory(默认名称)的bean传递给所有工厂中的bean有添加@PersistentContext的接口(这里是LessonService)
