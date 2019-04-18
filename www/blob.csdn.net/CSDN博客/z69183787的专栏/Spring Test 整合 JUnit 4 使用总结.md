# Spring Test 整合 JUnit 4 使用总结 - z69183787的专栏 - CSDN博客
2014年08月28日 09:25:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：33557
这两天做Web开发，发现通过Spring进行对象管理之后，做测试变得复杂了。因为所有的Bean都需要在applicationContext.xml中加载好，之后再通过@Resource去取得。如果每次都要整个业务流做的差不多了再去测试，这样效率很低，也很麻烦。如果单独去写一个测试用类，这样太繁琐了。于是想起Spring有一个测试框架，能够整合JUnit进行测试，于是便拿出来试试~
## 1. 加入依赖包
　　使用Spring的测试框架需要加入以下依赖包：
- JUnit 4 （官方下载：[http://www.junit.org/](http://www.junit.org/)）
- Spring Test （Spring框架中的test包）
- Spring 相关其他依赖包（不再赘述了，就是context等包）
## 2. 创建测试源目录和包
在此，推荐创建一个和src平级的源文件目录，因为src内的类都是为日后产品准备的，而此处的类仅仅用于测试。而包的名称可以和src中的目录同名，这样由于在test源目录中，所以不会有冲突，而且名称又一模一样，更方便检索。
## 3. 创建测试类
创建一个测试用的类，推荐名称为 “被测试类名称 + Test”。
测试类应该继承与 AbstractJUnit4SpringContextTests 或 AbstractTransactionalJUnit4SpringContextTests
对于 AbstractJUnit4springcontextTests 和 AbstractTransactionalJUnit4SpringContextTests 类的选择：
　　如果再你的测试类中，需要用到事务管理（比如要在测试结果出来之后回滚测试内容），就可以使用AbstractTransactionalJUnit4SpringTests类。事务管理的使用方法和正常使用Spring事务管理是一样的。再此需要注意的是，如果想要使用声明式事务管理，即使用AbstractTransactionalJUnitSpringContextTests类，请在applicationContext.xml文件中加入transactionManager
 bean：
```
<bean id="transactionManager"
class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
    <property name="dataSource" ref="dataSource" />
</bean>
```
　如果没有添加上述bean，将会抛出NoSuchBeanDefinitionException，指明
 No bean named 'transactionManager' is definded.
## 4. 配置测试类
添加如下内容在class前，用于配置applicationContext.xml文件的位置。
```
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
```
## 5. 创建测试方法
创建测试用方法，推荐名称为 “被测方法名称+ Test”。
测试方法上方加入 @Test
## 6. 通过JUnit 4 执行
右键方法名，选择则“Run As”→“JUnit Test”即可
## 附录1：整体测试类文件
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/* @(#) UserDaoTest.java
 * 
*/
package com.phj.dao;
import javax.annotation.Resource;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import com.phj.entity.User;
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
public class UserDaoTest extends AbstractJUnit4SpringContextTests {
    @Resource
private UserDaoInterface userDao;
    @Test
public void saveTest() {
        User user1 = new User();
        user1.setUsername("tom");
        user1.setPassword("123456");
        user1.setNickName("tom");
        user1.setEmail("tom@gmail.com");
        User user2 = new User();
        user2.setUsername("admin");
        user2.setPassword("123456");
        user2.setNickName("admin");
        user2.setEmail("admin@admin.com");
        User user3 = new User();
        user3.setUsername("feihong");
        user3.setPassword("123456");
        user3.setNickName("phj");
        user3.setEmail("test@gmail.com");
        userDao.save(user1);
        userDao.save(user2);
        userDao.save(user3);
    }
}
```
　　OK，到此为止就可以使用Spring的测试框架了。
使用注解时,Junit报错：
**[java]**[view
 plain](http://blog.csdn.net/linminqin/article/details/6410177#)[copy](http://blog.csdn.net/linminqin/article/details/6410177#)
- java.lang.NoSuchFieldError: NULL  
-     at org.junit.runners.ParentRunner.<init>(ParentRunner.java:48)  
-     at org.junit.runners.BlockJUnit4ClassRunner.<init>(BlockJUnit4ClassRunner.java:59)  
-     at org.springframework.test.context.junit4.SpringJUnit4ClassRunner.<init>(SpringJUnit4ClassRunner.java:104)  
-     at sun.reflect.NativeConstructorAccessorImpl.newInstance0(Native Method)  
-     at sun.reflect.NativeConstructorAccessorImpl.newInstance(NativeConstructorAccessorImpl.java:39)  
-     at sun.reflect.DelegatingConstructorAccessorImpl.newInstance(DelegatingConstructorAccessorImpl.java:27)  
-     at java.lang.reflect.Constructor.newInstance(Constructor.java:513)  
-     at org.junit.internal.requests.ClassRequest.buildRunner(ClassRequest.java:33)  
-     at org.junit.internal.requests.ClassRequest.getRunner(ClassRequest.java:28)  
-     at org.eclipse.jdt.internal.junit4.runner.JUnit4TestReference.<init>(JUnit4TestReference.java:28)  
经检查是jar包冲突,我在建项目的时候,myeclipse有自动加入junit4,而后面我又自己加入Junit4.8.1。
如果报以下错误，则是junit包版本太低，需要4.5或以上的版本
**[java]**[view
 plain](http://blog.csdn.net/linminqin/article/details/6410177#)[copy](http://blog.csdn.net/linminqin/article/details/6410177#)
- Caused by: java.lang.ClassNotFoundException: org.junit.runners.BlockJUnit4ClassRunner    
-     at java.net.URLClassLoader$1.run(URLClassLoader.java:200)    
-     at java.security.AccessController.doPrivileged(Native Method)    
-     at java.net.URLClassLoader.findClass(URLClassLoader.java:188)    
-     at java.lang.ClassLoader.loadClass(ClassLoader.java:307)    
-     at sun.misc.Launcher$AppClassLoader.loadClass(Launcher.java:301)    
-     at java.lang.ClassLoader.loadClass(ClassLoader.java:252)    
-     at java.lang.ClassLoader.loadClassInternal(ClassLoader.java:320)  
- 
其他问题：
@ContextConfiguration({"classpath:applicationContext.xml","classpath:spring/buyer/applicationContext-service.xml"})
导入配置文件，这里我的applicationContext配置文件是根据模块来分类的。如果有多个模块就引入多个“applicationContext-service.xml”文件。如果所有的都是写在“applicationContext。xml”中则这样导入： 
@ContextConfiguration(locations = "classpath:applicationContext.xml")
@RunWith(SpringJUnit4ClassRunner.class)SpringJUnit支持，由此引入Spring-Test框架支持！ 
@Transactional这个非常关键，如果不加入这个注解配置，事务控制就会完全失效！ 
**@TransactionConfiguration(transactionManager = "transactionManager", defaultRollback = true)**这里的事务关联到配置文件中的事务控制器（transactionManager
 = "transactionManager"），同时指定自动回滚（defaultRollback = true）。这样做操作的数据才不会污染数据库！ 
AbstractTransactionalDataSourceSpringContextTests要想构建这一系列的无污染纯绿色事务测试框架就必须找到这个基类！（即所有事务均不生效）
```java
import com.wonders.frame.ias.service.IasScheduleTask;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.stereotype.Component;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.transaction.annotation.Transactional;
import javax.annotation.Resource;
/**
 * Created with IntelliJ IDEA.
 * User: zhoushun
 * Date: 2015/3/30
 * Time: 12:40
 * To change this template use File | Settings | File Templates.
 */
@Component
@Transactional
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath*:applicationContext.xml")
//public class IasTest extends AbstractTransactionalJUnit4SpringContextTests{
public class IasTest extends AbstractJUnit4SpringContextTests {
    @Resource
    private IasScheduleTask task;
    @Test
    public void taskTest(){
        task.urgeTask();
    }
}
```
