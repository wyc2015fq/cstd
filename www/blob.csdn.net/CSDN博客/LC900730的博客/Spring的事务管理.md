# Spring的事务管理 - LC900730的博客 - CSDN博客
2017年08月09日 21:20:13[lc900730](https://me.csdn.net/LC900730)阅读数：214标签：[spring																[事务](https://so.csdn.net/so/search/s.do?q=事务&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## Spring中的事务
- Spring声明式事务管理
```
1.  XML配置
    2.注解方式
```
- Spring与Hibernate整合
- SSH整合
### 程序中事务控制
用户访问->action->Service->Dao 
一个业务成功，调用的service是执行成功的，意味着service中调用的所有dao是执行成功的 。 
事务应该在service层统一控制。 
模拟：在service中调用2次dao，希望其中一个dao执行失败，整个操作要回滚
开发步骤： 
    后台环境准备 
    1.数据表：entity/dao/service 
    2.dao的实现 
    3.对象创建都由Spring容器完成
### 事务控制
一：编程式事务控制
```
自己手动控制事务，就叫做编程式事务控制
//设置手动控制事务
    jdbc代码：jdbc是拿到connection对象，然后setAutoCommit(false)
Hibernate:
    Session.beginTransaction();
```
细粒度的事务控制，可以对指定的方法、指定的几行添加事务控制 
二：声明式事务控制 
    Spring提供了事务的管理，这个就叫做声明式事务管理。 
    即定义了一个事务。 
    Spring提供了对事务控制的实现，用户如果想实现Spring声明式事务管理，只需要在配置文件中配置即可，不想使用直接移除即可，这个实现了对事务控制最大程度的 解耦 
    Spring声明式事务管理，核心实现就是基于Aop； 
    只能对整个方法应用事务，不能对方法的某几行应用事务，因为Aop拦截的是方法
### Spring声明式事务管理器类
### 一：配置实现
bean.xml
```
配置事务管理器类
```
```xml
<!--5. Spring事务管理配置 -->
         <!--5.1 配置事务管理器类 -->
         <bean id="txManager"  class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
            <property name="dataSource" ref="dataSource"></property>
         </bean>
         <!--5.2配置事务增强  -->
         <tx:advice id="txAdvice" transaction-manager="txManager">
            <tx:attributes>
                <tx:method name="get*" read-only="true"/>
                <tx:method name="find*" read-only="true"/>
                    <tx:method name="save*" read-only="false" rollback-for="java.lang.Exception"/>
            </tx:attributes>
         </tx:advice>
         <!-- 5.3AOP配置 拦截哪些方法+应用上面的事务增强配置-->
         <aop:config>
            <aop:pointcut expression="execution(* cn.itcast.a_tx.DeptService.*(..))" id="pt"/>
            <aop:advisor advice-ref="txAdvice" pointcut-ref="pt"/>
         </aop:config>
```
//事务失败的原因： 
1.save方法有参数，在切入点配置execution()时候DeptService.*(..)时候没有加..，拦截的是不带参数的save(),而不是save(Dept dept),因此事务执行失败 
2.jdbc技术：DataSourceTransactionManager 
Hibernate技术：HibernateTransaction
### 二：注解方式实现
使用注解实现Spring 
1.必须引入aop相关jar文件 
2.bean.xml指定注解方式实现声明式事务管理及应用的事务管理器类 
3.在需要添加事务控制的地方，写上transactional
@Transactional注解： 
1.应用事务的注解 
2.定义到方法上，当前方法应用spring的声明式事务 
3.定义到类上，当前类的所有方法都应用spring声明式事务管理 
4.定义到父类上，当执行父类时候应用事务
```xml
bean.xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:p="http://www.springframework.org/schema/p"
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:aop="http://www.springframework.org/schema/aop"
    xmlns:tx="http://www.springframework.org/schema/tx" 
    xsi:schemaLocation="
        http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context
        http://www.springframework.org/schema/context/spring-context.xsd
        http://www.springframework.org/schema/aop
        http://www.springframework.org/schema/aop/spring-aop.xsd
        http://www.springframework.org/schema/tx 
        http://www.springframework.org/schema/tx/spring-tx-2.5.xsd">
      <!--数据源对象：c3p0连接池 -->
      <bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource">
        <property name="driverClass" value="com.mysql.cj.jdbc.Driver"></property>
        <property name="jdbcUrl" value="jdbc:mysql://localhost:3306/test?characterEncoding=utf8&useSSL=false"></property>
        <property name="user" value="root"></property>
            <property name="password" value="qqaazz"></property>
        <property name="initialPoolSize" value="3"></property>
            <property name="maxPoolSize" value="10"></property>
        <property name="maxStatements" value="100"></property>
            <property name="acquireIncrement" value="2"></property>
      </bean>
      <!-- jdbctemplate工具类实例 -->
      <bean id="jdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
            <property name="dataSource" ref="dataSource"></property> 
      </bean>
      <!-- 事务管理器类 -->
      <bean id="txManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="dataSource"></property>
      </bean>
      <!--开启注解扫描  -->
      <context:component-scan base-package="cn.itcast.b_anno">
      </context:component-scan>
     <!-- 注解方式实现事务，指定注解方式实现事务 -->
     <tx:annotation-driven transaction-manager="txManager"/>
</beans>
```
DeptDao.java
```java
package cn.itcast.b_anno;
import javax.annotation.Resource;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;
/**
 * 使用spring的jdbc
 */
@Repository
public class DeptDao {
    @Resource
    private JdbcTemplate jdbcTemplate;
    public void setJdbcTemplate(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    }
    public JdbcTemplate getJdbcTemplate() {
        return jdbcTemplate;
    }
    public void save(Dept dept){
        String sql="insert into t_dept(deptName) values(?);";
        jdbcTemplate.update(sql,dept.getDeptName());
    }
}
```
DeptService.java
```java
package cn.itcast.b_anno;
import javax.annotation.Resource;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
@Service
public class DeptService {
    @Resource
        private DeptDao deptDao;    
        public DeptDao getDeptDao() {
            return deptDao;
        }
        public void setDeptDao(DeptDao deptDao) {
            this.deptDao = deptDao;
        }
        @Transactional
        public void save(Dept dept) {
            //第一次调用         
            deptDao.save(dept);
//          int i=1/0;
            //第二次调用
            deptDao.save(dept);
        }
}
```
测试类App.java
```java
package cn.itcast.b_anno;
import org.junit.Test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
public class App {
    @Test
    public void testApp() {
        ApplicationContext ac1=new ClassPathXmlApplicationContext("cn/itcast/b_anno/bean.xml");
        Dept dept=new Dept();
        dept.setDeptName("测试部门");
        DeptService deptService=(DeptService)ac1.getBean("deptService");
        deptService.save(dept);
    }
}
```
@Transactional( 
    readonly=false, 
    timeout=-1       //事务超时时间，但是底层超时是由数据库决定的 
    noRollbackFor=ArithmeticException.class    //遇到这种数学异常不回滚 
    isolation=Isolation.Default,   数据库的隔离级别 
    propadation=Propagation.REQUIRED   //事务的传播行为 
        REQUIRED：业务方法需要在一个事务中运行，如果方法运行时，已经处在一个事务中那么加入到该事务，否则为自己创建一个新的 事务； 
        REQUIRED_NEW：属性表明不管是否存在事务，业务方法总会为自己发起一个新的事务，如果方法已经运行在一个事务中，则原有的事务会被挂起，新的事务会被创建，直到方法执行结束，新事务才算结束，原先的事务才会恢复执行。
)
```
class Log{
        Propagation.REQUIRED;
        insertLog();
    }
    void saveDept(){
        // 本身有事务，定义了一个required，那么假如同一个事务。但是方法执行失败也需要假如日志，所以需要把insertLog()写成
        Propagation.REQUIED_NEW;
        //那么外面saveDept是一个事务，里面的insertLog也是一个事务
        insertLog();
        //此时发生异常的话，如果是REQUIRED会回滚，如果是REQUIRED_NEW那么不会回滚。
        saveDept();
    }
```
### 了解容器的相关方法
1.根据bean.xml配置，创建容器对象 
//根据多个配置文件的路径，创建容器对象 
1️⃣ApplicationContext ac=new ClassPathXmlApplicationContext(new String[]{}) 
2️⃣ApplicationContext ac=new ClassPathXmlApplicationContext(“cn/bean.xml”)
2.从容器中获取指定名称的bean对象 
DeptDao deptdao=(DeptDao)ac.getbean(“depedao”); 
还可以根据类型获取 
//该类型在IOC容器中只能有一个，否则报错。 
DeptDao deptDao=ac.getBean(DeptDao.class); 
//使用泛型,不需要强转 
DeptDao deptDao=ac.getBean(“deptDao”,DeptDao.class);
4.获取容器中bean对象的数量 
ac.getBeanDefinitionCount();
5.获取容器中bean对象的名字 
String[] names=ac.getBeanDefinitionNames();
