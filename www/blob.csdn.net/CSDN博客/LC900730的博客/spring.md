# spring - LC900730的博客 - CSDN博客
2017年08月07日 15:26:33[lc900730](https://me.csdn.net/LC900730)阅读数：145标签：[spring](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
核心配置文件：
### 从容器中获取对象
IOC容器=factory+applicationContext.xml
```
public void testIOC() throws Exception{
        //创建对象
        //  User user=new User();
        //现在 把创建对象交给spring的IOC容器来做
        Resource resource=new ClassPathResource("cn/itcast/a_hello/applicationContext.xml");
        //创建容器对象(Bean工厂)
        BeanFactory factory=new XmlBeanFactory(resource);
        User user=(User)factory.getBean("user");
        System.out.println(user);   
    }
```
方式二： 
    得到容器对象，从容器中获取bean
```java
public void testAc() throws Exception{
    ApplicationContext ac=new ClassPathXmlApplicationContext("cn/itcast/a_hello/applicationContext.xml");
    User user=(User) ac.getBean("user");
}
```
1. 
    创建对象，默认是单例 
    即scope=”singleton”  【service/dao/工具类】 
        scope=”prototype” 【Action对象】 
2.什么时候创建 
    如果指定scope=”prototype”,那么在用到对象的时候才会创建对象。 
    如果是scope=“singleton”,那么在容器对象初始化之前就已经创建bean,且整个应用只有一个。 
    只对singleton有效 
    1.lazy-init=”default”即默认的false，不延迟创建，即启动的时候就创建对象 
    2.lazy-init=”true”,    延迟初始化，先创建容器，在用到对象的时候在创建对象 
因为多例只有在使用的时候在创建。
```
<bean id="user" class="cn.itcast.a_hello.User" init-method="init_user" destroy-method="destroy_user">
 </bean>
```
### Spring IOC容器
Spring IOC容器是spring核心内容 
作用：创建对象&处理对象的依赖关系 
容器创建对象： 
    1.调用无参数构造器 
    2.带参数构造器
```xml
<!--默认无参数构造器  -->
        <bean id="user" class="cn.itcast.b_create.User" init-method="init_user"
        destroy-method="destroy_user"></bean>
 <!--  带参数构造器-->
        <bean id="user" class="cn.itcast.b_create.User" init-method="init_user"
        destroy-method="destroy_user">
        <constructor-arg value="10"> </constructor-arg>
        <constructor-arg value="Jack"> </constructor-arg>
        </bean>
        <constructor-arg value="XX"  index="XX" type="Java,lang.String">
    //基本类型直接写，引用类型写全名
    //ref：有些值是对象
    //如定义一个字符串，值是“Jack”
        String s=new String("Jack");    
        <bean id="str" class="java.lang.String">
             <constructor-arg value="Jack"> </constructor-arg>
        </bean>
    那么在其他的bean中，字符串可以不用写死
    如
    <bean id="user" class="cn.itcast.User">
             <constructor-arg value="100"> </constructor-arg>
                 //引用上面的str字符串
             <constructor-arg ref="str"> </constructor-arg>
    </bean>
```
```
3.工厂创建对象
    工厂类:静态方法创建对象
    工厂类:非静态方法创建对象
```
如果文件放在src下面，那么直接写文件名称就可以，如果是放在包下的话，那么 
需要写包名加类 
ApplicationContext ac=new ClassPathXmlApplicationContext(“cn/itcast/b_create/bean.xml”)
#### 工厂类静态方法
//调用实例方法，一定要写factroy-bean
//调用静态方法，就一定要指定工厂类型
此时factroy-method一定是工厂里面的静态方法 
## 对象之间的依赖关系
```
如何给对象的属性赋值？
    1.通过构造函数
    2.通过set方法给属性注入(最常用)
    3.p名称空间
    4.自动装配
    5.注解
```
#### 通过set方法
```
//通过容器注入属性值
```
```xml
//user对象必须提供setId、setName方法
    <bean id="user" class="cn.itcast.c_property.User" >
                <property name="id" value="101"> </property>
                <property name="name" value="Jack"> </property>
        </bean>
```
```xml
<bean id="userDao" class="cn.itcast.c_property.UserDao"></bean>
          <!--Service instance  -->
         <bean id="userService" class="cn.itcast.c_property.UserService">
            <!-- 引用一个已经实例化的dao -->
            <property name="userDao" ref="userDao"></property>
         </bean>
         <!-- action instance -->
         <bean id="userAction" class="cn.itcast.c_property.UserAction">
            <!-- 引用一个已经实例化的dao -->
            <property name="userService" ref="userService"></property>
         </bean>
```
### 内部bean
```xml
<bean id="userAction" class="cn.itcast.c_property.UserACtion">
    <property name="userService">
        <bean class="cn.itcast.c_property.UserService">
            <property name="userDao">
                <bean class="cn.itcast.c_property.UserDao">
                </bean>
            </property>
        </bean>
    </property>
</bean>
```
上述联系的区别： 
    1.都可以创建action对象，解决了依赖关系 
    2.但是下面这种写法，无法让其他的action重复使用userDao，userService等
#### p名称空间给对象属性注入值
```xml
<!--Dao instance  -->
         <bean id="userDao" class="cn.itcast.c_property.UserDao"></bean>
          <!--Service instance  -->
         <bean id="userService" class="cn.itcast.c_property.UserService" p:userDao-ref="userDao">
         </bean>
         <!-- action instance -->
         <bean id="userAction" class="cn.itcast.c_property.UserAction" p:userService-ref="userService" >
         </bean>
```
如果是p:userDao-ref=”userDao”那么其必须有一个属性是userDao
### 自动装配
根据名称自动装配，userAction注入的属性，会去ioc容器中自动查找与属性同名的引用的对象，并自动注入 
和对象同名的对象自动装配，需要配置 
autowire=”byName”
```
<bean id="userDao" class="cn.itcast.d_auto.UserDao"></bean>
    //userService有一个属性，名为userDao，其自动找到
    <bean id="userService" class="cn.itcast.d_auto.UserService" autowire="byName"></bean>
    <bean id="userAction" class="cn.itcast.d_auto.UserAction" autowire="byName"></bean>
或者在beans 最后一行加上default-autowire="byName",那么就全局自动装配
autowire="byName" /byType
//类型是cn.itcast.d_auto.UserService
public class UserAction{ 
    private UserService userService;(需要的类型是这个)
}
```
如果根据类型自动装配，必须确保容器中只有一个该类型的对象
```
<bean id="userAction" class="cn.itcast.d_auto.UserAction" autowire="byType"></bean>
<bean id="userAction_test" class="cn.itcast.d_auto.UserAction" autowire="byType"></bean>
```
那么此时会报错，因为上面已经有一个该类型的对象，且使用了根据类型自动装配
### 注解
自动装配简化了配置，但是不利于后期维护，使用注解 
主要目的是简化配置。 
注解： 
    使用注解 
    1.先引入context名称空间 
    2.开启注解扫描 
    3.使用注解 
        通过注解的方式把对象加入ioc容器
//把当前对象加入ioc容器 
@Componen(“userDao” )    bean.xml()
处理依赖关系的时候 
@Component(“userService”); 
public class UserService{ 
//会从ioc容器中找userDao对象，并且注入到当前字段 
    @Resource（name=”userDao”）; 
    private UserDao userDao; 
    private void setUserDao(UserDao userDao){ 
        this.userDao=userDao; 
    } 
} 
以@Component方式加入IOC容器的时候，会有默认名称，默认与类名一致，且第一个字母小写 
@Resource（name=”XXX”）//根据名称去查找 
以@Resource 
private UserDao userDao；那么去IOC中找UserDao类型的变量，找到就赋值
@Repository   //持久层 
@Service        //业务逻辑层 
@Controller   //控制层
**如果是@Resource，即省略name=”XXX”,有多个相同类型的对象，那么会报错**
