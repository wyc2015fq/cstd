# Spring学习（一）ioc理解 - 建建的博客 - CSDN博客
2018年01月05日 09:30:13[纪建](https://me.csdn.net/u013898698)阅读数：54
个人分类：[java学习路线](https://blog.csdn.net/u013898698/article/category/7152763)
> 
对Spring有所了解的都应该知道，spring贯穿始终的两个重要的思想，一个是IOC（控制反转）,一个是DI（依赖注入），对于新手而言，这两个概念比较难理解，我用自己的想法来解释下。
## 首先我们要知道在程序开发中为什么要使用spring框架。
这里先引入两个概念：耦合性、侵入性。简单的讲：耦合性，在java中表现为类之间的关系，耦合性强说明类之间的依赖关系强；侵入性：框架对代码的侵入，比如你项目用了struts1，要改框架时发现改的东西太多了，比如actionForm等，所以struts1对代码的侵入性是很高的。 
在传统的java开发中具有高度的耦合性和侵入型。一个项目中，一般一个类都要依赖很多其他的类来完成自己的操作，我们往往采用new这个类的对象来调用他的方法，这样就造成了两个类的依赖关系太强，改一个地方，往往牵扯很多类牵扯大量的代码。侵入性上一段的例子可以明白。 
当然EJB也可以解决耦合性和侵入性的问题，但是ejb太依赖服务器，属于重量级的框架。 
可以说在这样的背景下，spring应运而生，一个轻量级的框架，解决传统企业开发的复杂性；使用普通的javaBean代替EJB技术。可以管理对象和对象之间的依赖关系，我们不需要自己建立对象，把这部分工作全部转交给容器完成，具有低耦合，对代码没有侵略性，对服务器没有依赖性特点的框架。 
而这个容器，即IOC.
## 如何理解IOC思想？
一个很流行的例子，我换了种说法，这样更好理解：好比找女朋友，普通的方式是我们依赖各种关系找到这个女朋友（相当于new了对象），有一天分手了，而以前的那些关系也没有了，要想找到新的女朋友就要依赖新的关系（重新 new另外一个对象）,可以想象这个过程很麻烦。于是有了一种新的方式找对象——婚介所，这里就是我们所说的IOC方式，你把你要求的对象的特征告诉婚介所，他会直接给你找到一对象，没有中间复杂的过程，你只管相处的事情就好了，new的过程不用管，不需要任何依赖关系，即使哪天要换了，再把需要的对象提交给婚介所就行了，你就会得到你想要的新的对象了。 
事实上，使用ioc方式创建对象的目的，是为了以“被动”的方式形成对象之间的依赖关系。传统的开发过程中，不管是new，还是普通工厂，都需要目标对象主动创建、主动查找其需要的依赖对象 ， 目标对象会将他的精力分散在不必要的非业务逻辑方面。IOC通过DI(依赖注入)把建立好的对象注入到目标对象中。
## Spring IOC具体如何实现？
上文中的婚介所就是ioc管理对象的容器，实际上是一个xml文件，将对象配置在xml里面，通过spring的工厂类进行解析，以“反射”的方式创建对象。 
spring IOC容器的关键点： 
* 必须将被管理的对象定义到spring配置文件中 
* 必须定义构造函数或setter方法，让spring将对象注入过来 
我们可以通过下面这里例子理解spring ioc的实现。本例使用spring 3.2 
1.配置applicationContext.xml
```xml
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
    http://www.springframework.org/schema/beans/spring-beans-3.2.xsd">
    <bean id="userDao4MySqlImpl" class="com.bjsxt.spring.dao.UserDao4MySqlImpl"/>
    <bean id="userDao4OracleImpl" class="com.bjsxt.spring.dao.UserDao4OracleImpl"/>
    <bean id="userManager" class="com.bjsxt.spring.manager.UserManagerImpl">
        <!-- 构造方法注入 
        <constructor-arg ref="userDao4OracleImpl"/>
         -->
         <!-- setter方法注入 -->
         <property name="userDao" ref="userDao4OracleImpl"/>
    </bean>
</beans>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
2.注入的类：
```
package com.bjsxt.spring.dao;
public interface UserDao {
    public void save(String username, String password);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
```java
package com.bjsxt.spring.dao;
public class UserDao4MySqlImpl implements UserDao {
    public void save(String username, String password) {
        System.out.println("--------UserDao4MySqlImpl.save()-------");
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
```java
package com.bjsxt.spring.dao;
public class UserDao4OracleImpl implements UserDao {
    public void save(String username, String password) {
        System.out.println("--------UserDao4OracleImpl.save()-------");
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
3.被注入的类：
```
package com.bjsxt.spring.manager;
public interface UserManager {
    public void save(String username, String password);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
```java
package com.bjsxt.spring.manager;
import com.bjsxt.spring.dao.UserDao;
public class UserManagerImpl implements UserManager {
    /**
     * 两种方式：如果这个类中需要注入对象，先建立对象属性，
     *      在写构造方法或者settet方法。
     * 
     */
    private UserDao userDao;
/*  public UserManagerImpl(UserDao userDao) {
        this.userDao = userDao;
    } */
    public void save(String username, String password) {
        this.userDao.save(username, password);
    }
    public void setUserDao(UserDao userDao) {
        this.userDao = userDao;
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
4.测试类：
```java
package com.bjsxt.spring.client;
import org.springframework.beans.factory.BeanFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import com.bjsxt.spring.manager.UserManager;
public class Client {
    public static void main(String[] args) {
/*  传统的通过new对象建立类之间的关系
 * UserManager userManager = new UserManagerImpl(new UserDao4OracleImpl());
        UserManager userManager = new UserManagerImpl(new UserDao4MySqlImpl());
        userManager.save("张三", "123");*/
/**
 * IOC思想     通过工厂类解析xml文件，以“反射”的方式创建对象：
 */
        BeanFactory factory = new ClassPathXmlApplicationContext("applicationContext.xml");
        UserManager userManager = (UserManager)factory.getBean("userManager");
        userManager.save("张三", "123");
/**
 * IOC思想   实际的执行过程，这也是为什么需要setter方法或构造方法的原因：        
 */
//      UserManagerImpl userManager = new UserManagerImpl();
//      userManager.setUserDao(new UserDao4MySqlImpl());
//      userManager.save("张三", "123");
    }
}
```
