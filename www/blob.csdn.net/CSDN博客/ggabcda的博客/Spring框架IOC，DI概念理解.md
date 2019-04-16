# Spring框架IOC，DI概念理解 - ggabcda的博客 - CSDN博客





2017年08月30日 13:13:32[ggabcda](https://me.csdn.net/ggabcda)阅读数：128








1.什么是框架？

框架是一种重复使用的解决方案，针对某个软件开发的问题提出的。

 Spring框架，它是一个大型的包含很多重复使用的某个领域的解决方案。

 Spring的理念：不要重复发明轮子。



2.Spring的理解？



首先，Spring是一个容器。它是装对象的。主要就是通过搜索class的路径。找出bean对象，实际就是根据反射来获取这个bean对象的：



```
Class<?> classit=Class.forName("com.jinglin.model.Person");
         Person p =(Person)classit.newInstance();
```



3.IOC的理解？

控制反转的容器。

把对象的获取的主动权交出去

4.DI:

依赖注入，通过在构建bean对象的时候，把数据加入到对象中，所谓的依赖的意思就是这里的数据有可能是通过其它的bean对象得到的。

5.对于Spring中的bean的注入方式

1）set方式注入，注入数据的前提是必须要有一个set方法，在bean的类里。是通过属性的注入方式：

java类：



```
public class Boy {
    public String name;
    public Girl girlfriend;
    public List<String> habbies;
    public Map<String,Float> subject;
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public Girl getGirlfriend() {
        return girlfriend;
    }
    public void setGirlfriend(Girl girlfriend) {
        this.girlfriend = girlfriend;
    }
    public List<String> getHabbies() {
        return habbies;
    }
    public void setHabbies(List<String> habbies) {
        this.habbies = habbies;
    }
    public Map<String, Float> getSubject() {
        return subject;
    }
    public void setSubject(Map<String, Float> subject) {
        this.subject = subject;
    }
    
}
```



```
public class Girl {
    public String name;
    public Boy boyfriend;
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public Boy getBoyfriend() {
        return boyfriend;
    }
    public void setBoyfriend(Boy boyfriend) {
        this.boyfriend = boyfriend;
    }
    
}
```



配置文件：

```
<!-- 把所有的类都生成bean对象,scope默认作用域是共享模式(prototype为非共享模式，生成的对象的地址不一样 )-->
    <bean id="boy" class="com.jinglin.model.Boy">
        <!-- 属性注入值 -->
        <property name="name" value="zhangsan"></property>
        <!-- 关联到另外一个bean对象 -->
        <property name="girlfriend" ref="girl"></property>
        <!-- 集合属性注入 -->
        <property name="habbies">
            <!--注入的list的集合值  -->
            <list>
                <value>网球</value>
                <value>乒乓球</value>
                <value>网游</value>
            </list>
        </property>
        <!-- 键值对的集合的数据注入 -->
        <property name="subject">
            <!-- 给map这种集合数据类型注入 -->
            <map>
                <entry>
                    <!-- 表示键 -->
                    <key><value>英语</value></key>
                    <!-- 表示值 -->
                    <value>98</value>
                </entry>
                <entry>
                    <key><value>数学</value></key>
                    <value>99</value>
                </entry>
            </map>
        </property>
    </bean>
    <bean id="girl" class="com.jinglin.model.Girl">
        <!-- 属性注入值 -->
        <property name="name" value="lily"></property>
        <!-- 关联到另外一个bean对象 -->
        <property name="boyfriend" ref="boy"></property>
    </bean>
```

2）构造函数的注入，初始化对象的时候通过构造函数传入数据：

有参构造：

```
public Person(String name, String age) {
        this.name = name;
        this.age = age;
    }
```

配置文件：

```
<!-- bean相当于是一个装对象的容器 -->
    <bean id="person" class="com.jinglin.model.Person">
        <!--通过构造参数的方式注入值  -->
        <constructor-arg value="zhangsan"></constructor-arg>
        <constructor-arg value="14"></constructor-arg>
    </bean>
```

6.那么由spring给我们提供的bean对象的作用域？

 1）默认情况下，spring提供的bean对象是共享模式的。

在内存中只出现一个实例化对象。

默认的bean的作用域：scope="singleton"

2）如果更改bean的作用域，就是非共享模式，

scope="prototype"

7.自动装配+注解，简化spring中的bean对象的开发。

 Spring中的bean对象自动从容器里搜索和自己的属性字段名一致的情况，如果有，就自动匹配。

开发步骤，首先明确，要将所有的包扫描的spring容器里。

 1）在applicationContext.xml里做spring的beans的声明。



```
<beans
    xmlns="http://www.springframework.org/schema/beans"   
    xmlns:p="http://www.springframework.org/schema/p"   
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
    xmlns:context="http://www.springframework.org/schema/context" 
    xsi:schemaLocation="http://www.springframework.org/schema/beans   
    http://www.springframework.org/schema/beans/spring-beans-3.0.xsd   
    http://www.springframework.org/schema/context
    http://www.springframework.org/schema/context/spring-context.xsd">
```



2)将各个包扫描到spring里。



```
<!-- 将包扫描到spring组件里 -->
    <context:component-scan base-package="com.jinglin.dao">
    </context:component-scan>
    <context:component-scan base-package="com.jinglin.service">
    </context:component-scan>
```



3）给每个类加入注解，告诉spring。将其加入到spring的容器里。



```
@Component("goodsInfoDao")
public class GoodsInfoDao {
   public void insertitem(){
       System.out.println("这是商品信息的数据插入");
   }
}
```



4）为了简化配置，采取的自动装配。一般都是通过名字自动装配的：

在配置头文件末尾加上



```
default-autowire="byName"
```



5）在开发的时候，对于同spring中相同的bean的id，那么采取自动装配：



```
@Component("userInfoService")
public class UserInfoService {
    
    @Autowired
    private UserInfoDao userInfoDao;

    public void additem(){
        System.out.println("这是用户调用的业务层");
        userInfoDao.insertitem();
    }
}
```



8.Spring的延迟加载，当我们需要这个spring提供的bean对象的时候，这个时候spring才会去生成这个对象（默认情况下，当spring加载它的文件的时候，就全部将spring里配置的bean对象生成）。

 1）在applicationContext配置头文件末尾加上



```
default-lazy-init="true"
```



缺点：无法对单个的bean对象提供延迟加载配置。

2）针对单个的bean对象：



```
<bean id="person" lazy-init="true"  class="com.jinglin.model.Person">
        <!-- 通过构造参数的方式注入值 -->
        <constructor-arg value="zhangsan"></constructor-arg>
        <constructor-arg value="14"></constructor-arg>
    </bean>
```



3）直接在类中加注解，表示就是延迟加载。



```
@Component("goodsInfoService")
@Lazy
public class GoodsInfoService {
    @Autowired
    private GoodsInfoDao goodsInfoDao;
    public void additem(){
        System.out.println("这是商品信息的业务处理方法");
       goodsInfoDao.insertitem();
    }
    
    public GoodsInfoService(){
        System.out.println("这是goodsInfoService的无参构造函数");
    }
   
}
```



























default-lazy-init=*"true"*



