# IBATIS事务处理 - z69183787的专栏 - CSDN博客
2015年05月31日 22:08:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：512
个人分类：[对象映射框架-Ibatis/Mybatis](https://blog.csdn.net/z69183787/article/category/2261201)
      iBATIS事务处理是和Dao紧密相联的。
        在使用Dao时，如以下代码，先插入新记录，再进行更新：
        UserDao.insertUser (user); // Starts transaction 
        user.setName("wh"); 
        UserDao.updateUser (user); // Starts a new transaction
       因为没有显式地启动事务，iBatis会认为这是两次事务，分别从连接池中取两次Connection。我们所写的Dao子类（继承自com.ibatis.dao.client.template.SqlMapDaoTemplate）的每一个Dao方法已经默认为一个事务（通过动态代理）。这样的事务是隐式事务.
        iBatis是通过DaoManager类来统管Dao子类的事务，
        众Dao子类由DaoManager产生,如下：
        Reader reader =Resources.getResourceAsReader("dao.xml"); 
        DaoManager  daoManager =DaoManagerBuilder.buildDaoManager(reader); 
        UserDao userDao = (UserDao) daoManager.getDao(UserDao.class);
        UserDao是用户自己定义的接口，获得的其实是在dao.xml中指定的相对应的 SqlMapDao实现类，从而实现了松藕合。在良好的分层设计中，
        但是一般的事务需要放到业务层,因为一个业务需要具有原子性,事务放到Dao层是不能达到业务一致的效果的,那么如果想要把事务放到业务层,就需要在业务层使用显示事务进行声明处理.
        显式地声明事务处理语句，如下：
        try { 
                daoManager.startTransaction(); 
                UserDao.insertUser (user);  
                user.setName("wh"); 
                UserDao.updateUser(user);  
                otherDao.doSomething(other); 
                 ... 
                daoManager.commitTransaction(); 
       } finally { 
                daoManager.endTransaction(); 
       }
       这样就保持了原子性，整体为一个事务，要么全部执行成功，否则回滚。
       现在唯一的问题就是，dao层的事务是否已经放弃，否则产生事务嵌套问题对性能会有影响.
       当然，iBatis 完全可以这么做：建一个声明式接口:IService,再使用动态代理，将用户自己的Serivce子类通过动态代理自动包上事务处理的代码，默认每一个业务方法为一个事务。
       大师的心如果能轻易揣测，就是不大师了:),估计大师认为这样属于过度设计，他认为把这种灵活性交给用户是合适的，相当多的service 方法只调用一个Dao方法，例如CRUD操作。
       再补充一下，iBatis中对事务的处理是可配置的，最常用的Type是"JDBC",也可以声明为"JTA"或"EXTERNAL".
       项目里常用Spring与Ibatis配合使用,这样可以在Spring里配置事务管理,可以省去业务层的显示事务代码.
        spring的配置文件的基本写法为:
**[html]**[view
 plain](http://blog.csdn.net/wanghuan203/article/details/8456517#)[copy](http://blog.csdn.net/wanghuan203/article/details/8456517#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlnsbeansxmlns="http://www.springframework.org/schema/beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:aop="http://www.springframework.org/schema/aop"xmlns:tx="http://www.springframework.org/schema/tx"
- xmlns:context="http://www.springframework.org/schema/context"
- xsi:schemaLocation="    
-           http://www.springframework.org/schema/beans     
-           http://www.springframework.org/schema/beans/spring-beans-2.0.xsd  
-           http://www.springframework.org/schema/aop     
-           http://www.springframework.org/schema/aop/spring-aop-2.0.xsd    
-           http://www.springframework.org/schema/tx     
-           http://www.springframework.org/schema/tx/spring-tx-2.0.xsd">
- 
- 
- <beanid="dataSource"class="org.apache.commons.dbcp.BasicDataSource" >
- <propertyname="driverClassName"value="com.mysql.jdbc.Driver" />
- <propertyname="url"value="jdbc:mysql://10.11.0.145:3306/carrefour?characterEncoding=gb2312"/>
- <propertyname="username"value="dev01"/>
- <propertyname="password"value="123456"/>
- </bean>
- <beanid="sqlClient"class="org.springframework.orm.ibatis.SqlMapClientFactoryBean">
- <propertyname="dataSource">
- <reflocal="dataSource"/>
- </property>
- <propertyname="configLocation">
- <value>classpath:sqlmaps.xml</value>
- </property>
- </bean>
- <!--配置事务管理器-->
- <beanid="transactionManager"class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
- <propertynamepropertyname="dataSource"ref="dataSource"></property>
- </bean>
- <!--配置哪些方法,什么情况下需要回滚-->
- <tx:adviceid="serviceAdvice"transaction-manager="transactionManager">
- <tx:attributes>
- <!--当代理的service层中的方法抛出异常的时候才回滚,必须加rollback-for参数-->
- <tx:methodnametx:methodname="insert*"propagation="REQUIRED"rollback-for="Throwable"/>
- <tx:methodnametx:methodname="del*"propagation="REQUIRED"rollback-for="Throwable"/>
- <tx:methodnametx:methodname="update*"propagation="REQUIRED"rollback-for="Throwable"/>
- <!--除了上面标识的方法,其他方法全是只读方法-->
- <tx:methodnametx:methodname="*"read-only="true"/>
- </tx:attributes>
- </tx:advice>
- <!-- 配置哪些类的方法需要进行事务管理 -->
- <aop:configproxy-target-class="true">
- <aop:pointcutid="servicePointcut"expression="execution(* com.wh.service.*.*(..))"/>
- <aop:advisorpointcut-ref="servicePointcut"advice-ref="serviceAdvice"/>
- </aop:config>
 这是在spring+Ibatis的情况下, ,通过aop控制需要事务的包和具体方法,将事务控制在service层,来达到事务在业务层提交和回滚.保持业务的原子性.
