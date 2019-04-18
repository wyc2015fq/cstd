# Spring学习 一 DI(依赖注入) - weixin_33985507的博客 - CSDN博客
2018年04月13日 15:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
##### 通过set方法给对象赋值
```
<!-- 通过set方法给属性赋值 -->
<bean id="user" class="hello.spring.User">
    <property name="id" value="1"></property>
    <property name="name" value="wangjian"></property>
</bean>
```
实际案例:
三层模型中.Dao、 Service 、Action 是解耦的,下面是他们的调用关系
Dao类:
```
public class UserDao {
    public void save(){
        System.out.println("DB: 保存到数据库");
    }
}
```
Service类:
```
public class UserService {
    private UserDao userDao;
    
    public void setUserDao(UserDao userDao) {
        this.userDao = userDao;
    }
    
    public void save(){
        userDao.save();
    }
}
```
Action类:
```
public class UserAction {
    private UserService userService;
    
    public void setUserService(UserService userService) {
        this.userService = userService;
    }
    
    public String execute(){
        userService.save();
        return null;
    }
}
```
上面这种情况是我们在使用Struts2时经常看到的情况,这种情况我们用Spring时需要在applicationContext中做如下配置:
```
<!-- 实际应用场景 -->
<bean id="userDao" class="hello.spring.UserDao"></bean>
<!-- service 实例 -->
<bean id="userService" class="hello.spring.UserService">
    <property name="userDao" ref="userDao"></property>
</bean>
<!-- action 实例 -->
<bean id="userAction" class="hello.spring.UserAction">
    <property name="userService" ref="userService"></property>
</bean>
```
还有一种私有写法:
```
<!-- 私有bean -->
<bean id="userAction" class="hello.spring.UserAction">
    <property name="userService">
        <bean class="hello.spring.UserService">
            <property name="userDao">
                <bean class="hello.spring.UserDao"></bean>
            </property>
        </bean>
    </property>
</bean>
// 这种写法不够灵活,只适用于Dao、Service、Action只用一次的情况下,这样写也不好维护
```
###### 使用 **P** 名称空间声明:
这种方式是在Spring 3.0 以后版本才可以使用的
```
<bean id="userDao" class="hello.spring.UserDao"></bean>
<bean id="userService" class="hello.spring.UserService" p:userDao-ref="userDao"></bean>
<bean id="userAction" class="hello.spring.UserAction" p:userService-ref="userService"></bean>
```
p:userDao属性和UserDao-ref属性的区别在于 userDao是直接复制时使用,类似于value属性.
### 注解
- **优点 :** 简化配置
- **缺点 :** 不利于维护
使用注解的步骤:
- 引入Context名称空间
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
    xmlns:p="http://www.springframework.org/schema/p"
    xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="
            http://www.springframework.org/schema/beans
            http://www.springframework.org/schema/beans/spring-beans.xsd
            http://www.springframework.org/schema/context
            http://www.springframework.org/schema/context/spring-*context.xsd">
    // 必须有这个名称空间才可以使用注解
    <!-- http://www.springframework.org/schema/context/spring-*context.xsd" -->
</beans>
```
- 开启注解扫描
```
<context:component-scan base-package="hello.spring"></context:component-scan>
```
**base-package :** 该属性指定扫描指定包、及其子包下多有的类
- 使用注解
原理: 通过注解的方式,把对象加入IOC容器
类的注解:
```
@Component(value = "userDao")
public class UserDao {
    public void save(){
        System.out.println("DB: 保存到数据库");
    }
}
    // 可以直接写 @Component("userDao")
    // 这条注解 == <bean id="userDao" class="hello.spring.UserDao">
```
字段(成员变量)的注解:
```
@Component("userService")
public class UserService {
    /**
     * 该注解会从IOC容器中找到userDao对象,注入到当前字段
     * 使用Resource注解相当于:
     * 
     * <bean id="" class="">
     *      <property name="" ref="">   // 相当于这一句
     * </bean>
     */
    @Resource(name = "userDao")
    private UserDao userDao;
    
            // 使用注解后,set方法可以放弃使用
    // public void setUserDao(UserDao userDao) {
    //  this.userDao = userDao;
    // }
    
    public void save(){
        userDao.save();
    }
}
```
除了Component、Resource注解外,Spring还提供了专门为MVC每层提供了专门的注解,用于区分:
```
@Repository : // 在持久层(Dao层)可以选择在类上用这个注解,和Component功能相同.
@Service : // 在业务逻辑层(Service层)可以使用这个
@Controller : // 这个注解使用在控制层的
```
