# Spring个人笔记 - westbrook1998的博客 - CSDN博客





置顶2018年09月23日 22:40:20[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：176标签：[spring](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[杂七杂八																[Spring](https://blog.csdn.net/westbrook1998/article/category/7573505)](https://blog.csdn.net/westbrook1998/article/category/7458454)








# Spring笔记

## 什么是Spring(简要概括)

> 
`Spring`是分层的`JavaSE/EE`应用一站式的轻量级开源框架，以`IoC(Inverse of Control，控制反转)`和`AOP(Aspect Oriented Programming，切面编程)`为核心，提供展示层`Spring MVC`，持久层`Spring JDBC`及业务层事务管理等一站式的企业级应用技术。


## 什么是Ioc(Inverse of Control，控制反转)

> 
将某个接口的具体实现类的选择控制权从调用类中移除，转交给第三方决定


## 什么又是DI(Dependency Injection，依赖注入)

> 
让调用类对某一接口实现类的依赖关系由第三方注入，以移除调用类对实现类的依赖


> 
**可以说，IoC是一个比较抽象的概念而DI是具体的实现形式**


## 为什么要IoC和DI

> 
首先从最原始的代码说起，电影类中的一个场景(方法)里，需要男主角(`LiuDeHua`对象)，直接new一个对象，然后调用方法(表演)


```java
```java
public class Movie{
    public void Scenes{
       LiuDeHua ldh=new LiuDeHua();
       ldh.action(); 
    }
}
```
```

> 
这样的缺点就是耦合度太大，如果需要临时换演员，那这段代码就要修改了，所以这里这个场景应该是围绕这故事的主角(接口)而不是具体的演员(具体实现类)


```java
```java
public interface Actor{
    void action();
}
public class LiuDeHua implements Actor{
    void action(){
        //...
    }
}
public class Movie{
    public void Scenes{
        //这样这里依赖的就只是Actor这个接口
        Actor act=new LiuDeHua();
        ldh.action(); 
    }
}
```
```

> 
当然这样，还没有达到彻底解偶的效果，`Scenes`方法中仍然同时依赖`Actor`接口和`LiuDeHua`类，所以此时我们需要使用依赖注入


### 构造函数注入

```java
```java
public class Movie{
    private Actor act;
    public Movie(Actor act){
        this.act=act;
    }
    public void Scenes{
        act.action();
    }
}
```
```

> 
这样，`Movie`类不用关心谁是`Actor`也能拍好这一场景，而具体演员就由第三方来安排


```java
```java
public class Director{
    public void direct(){
        Actor act=new LiuDeHua();
        Movie movie=new Movie(act);
        movie.Scenes();
    }
}
```
```

### 属性注入

> 
类似于构造器注入


```java
```java
public class Movie{
    private Actor act;
    public setAct(Actor act){
        this.act=act;
    }
    public void Scenes{
        act.action();
    }
}
```
```

```java
```java
public class Director{
    public void direct(){
        Actor act=new LiuDeHua();
        Movie movie=new Movie();
        movie.setAct(act);
        movie.Scenes();
    }
}
```
```

### 接口注入

> 
将依赖注入的方法抽取到一个接口中，与属性注入并无本质区别，一般不考虑接口注入


```java
```java
public interface ActorArrangable{
    void injectActor(Actor act);
}
public class Movie implements ActorArrangable{
    private Actor act;
    public injectActor(Actor act){
        this.act=act;
    }
    public void Scenes{
        act.action();
    }
}
```
```

## Spring中的资源访问

> 
`Spring`中提供了更强大的访问资源的接口`Resource`，(对jdk所提供的访问资源的类进行进一步的封装和加强)


### Resource接口的主要继承关系

```
graph LR
ByteArrayResource-->AbstractResource
ClassPathResource-->AbstractResource
DescriptiveResource-->AbstractResource
InputStreamResource-->AbstractResource
PortletContextResource-->AbstractResource
ServletContextResource-->AbstractResource
UrlResource-->AbstractResource
PathResource-->AbstractResource
PathResource==>WritableResource
FileSystemResource-->AbstractResource
FileSystemResource==>WritableResource
AbstractResource==>Resource
WritableResource-->Resource
```

> 
`Source`接口还有一个父接口`InputStreamSource`，只提供了`getInputStream()`这个方法


> 
`WritableResource`接口继承了`Source`接口，是Spring3.1之后增加的接口，有`PathResource(Spring4.0)`和`FileSystemResource`两个实现类，两个实现类的用法基本一致，都是能够读取绝对路径的资源文件，通过`getOutputStream()`获取输出流输出到文件，通过`getInputStream()`获取输入流，并通过`ByteArrayOutputStream`类的实例来将每一个字节读取入缓冲区，最后再转化为字符串输出


```java
```java
public class WritableResourceTest {
    public static void main(String[] args) {
        String filePath1="/home/russell/IdeaProjects/SpringLearning/src/main/resources/filePath1.txt";
        String filePath2="/home/russell/IdeaProjects/SpringLearning/src/main/resources/filePath2.txt";
        try {
            //Main
            WritableResource res1 = new FileSystemResource(filePath1);
            WritableResource res2 = new PathResource(filePath2);
            OutputStream stream1 = res1.getOutputStream();
            OutputStream stream2 = res2.getOutputStream();
            stream1.write("使用FileSystemResource写入".getBytes());
            stream2.write("使用PathResoruce写入".getBytes());
            InputStream stream3=res1.getInputStream();
            InputStream stream4=res2.getInputStream();
            ByteArrayOutputStream baos1=new ByteArrayOutputStream();
            int i;
            while((i=stream3.read())!=-1){
                baos1.write(i);
            }
            System.out.println(baos1.toString());
            ByteArrayOutputStream baos2=new ByteArrayOutputStream();
            while((i=stream4.read())!=-1){
                System.out.println(i);
                baos2.write(i);
            }
            System.out.println(baos2.toString());
            //Others
            //父接口Resource没有getPath()这个方法，因此要强转子类类型
            System.out.println(((FileSystemResource) res1).getPath());
            System.out.println(((PathResource) res2).getPath());
            //Resource接口有contentLength()这个方法
            System.out.println(res1.contentLength());
            System.out.println(res2.contentLength());
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
```
```

> 
使用`ClassPathResource`类读取类路径(classes)下的资源文件，只能读取，不能写入


```java
```java
public class ClassPathResourceTest {
    public static void main(String[] args) {
        String filePath="/filePath3.txt";
        try{
            //使用ClassPathResource读取的资源文件要放在类路径下(classes)
            Resource res=new ClassPathResource(filePath);
            InputStream stream=res.getInputStream();
            ByteArrayOutputStream baos=new ByteArrayOutputStream();
            int i;
            while((i=stream.read())!=-1){
                baos.write(i);
            }
            System.out.println(baos.toString());
            System.out.println(((ClassPathResource) res).getPath());
            System.out.println(res.getURL());
            System.out.println(res.contentLength());
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
```
```

> 
`ServletContextResource`是以相对于`Web`应用根目录的方式访问资源，即项目中web文件夹下


> 
读取资源默认使用系统编码进行读取，如果资源文件有特殊编码，可以使用`EncodedResource`对资源进行编码


```java
```java
...
Resource res=new ClassPathResource(filePath);
EncodedResource res1=new EncodedResource(res,"UTF-8");
String content=FileCopyUtils.copyToString(res1.getReader());
System.out.println(content);
...
```
```

### 加载不同资源

> 
因为使用`Resource`接口加载资源需要根据不同的资源类型和不同位置选择不同的实现类，比较麻烦，所以`Spring`提供了另一个加载资源的机制，可以通过不同地址前缀识别不同资源，并使用Ant风格的通配符匹配

|地址前缀|示例|对应资源|
|----|----|----|
|classpath:|classpath:com/zxc/beanfactory/beans.xml|从类路径下加载资源|
|file:|file:/conf/com/zxc/beanfactory/beans.xml|从文件系统中加载资源，可用绝对或相对路径|
|http://|[http://www.zxc.com/resource/beans.xml](http://www.zxc.com/resource/beans.xml)|从Web服务器中加载资源|
|ftp://|[ftp://www.zxc.com/resource/beans.xml](ftp://www.zxc.com/resource/beans.xml)|从Ftp服务器中加载资源|
|无|com/zxc/beanfactory/beans.xml|根据ApplicationContext具体实现类采用对应类型Resource|

> 
Ant风格

|通配符|效果|
|----|----|
|?|匹配一个字符|
|*|匹配任意数量字符|
|**|匹配多层路径|

#### classpath: 与 classpath*:的区别

> 
classpath:只会在第一个加载的com.zxc包中查找，而classpath*:会扫描所有jar包类路径下的com.zxc包


#### `ResourceLoader`的继承关系如下

```
graph LR
PathMatchingResourcePatternResolver==>ResourcePatternResolver
ResourcePatternResolver-->ResourceLoader
ResourceLoader-->Resource
```

> 
定义一个`PathMatchingResourcePatternResolver`(`Spring`提供的标准实现类)，再通过`getResources()`方法获取对应通配符路径下的所有资源，再使用资源的对应方法进行处理


```java
```java
public class ResourceLoaderTest {
    public static void main(String[] args) {
        try{
            ResourcePatternResolver resolver=new PathMatchingResourcePatternResolver();
            Resource resources[]=resolver.getResources("classpath*:com/zxc/**/*.txt");
            for(Resource resource : resources){
                System.out.println(resource.getDescription());
            }
        }catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
```

### BeanFactory和Application

> 
`Spring`通过一个描述`bean`和`bean`之间依赖关系的配置文件，利用反射实例化了`bean`并建立之间的依赖关系
`BeanFactory`和`Application`就是这样两个用来创建`bean`，处理和`bean`有关事务的接口
`BeanFactory`更多是面向`Spring`本身，更底层

而`ApplicationContext`是面向应用者，面向应用
#### BeanFactory简单使用

##### 先写一个实体类User

```java
```java
package com.zxc.demo.bean;

public class User {
    private String name;

    public void setName(String name) {
        this.name = name;
    }
}
```
```

##### 创建xml文件(这里创建在类路径下)

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:p="http://www.springframework.org/schema/p"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="user1" class="com.zxc.demo.bean.User" p:name="java"/>
</beans>
```

##### 测试(长长的类名…)

```java
```java
package com.zxc.demo.bean;

import org.springframework.beans.factory.support.DefaultListableBeanFactory;
import org.springframework.beans.factory.xml.XmlBeanDefinitionReader;
import org.springframework.core.io.Resource;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.core.io.support.ResourcePatternResolver;

public class BeanTest {

    public static void main(String[] args)throws Throwable {
    	//加载类路径下的资源
        ResourcePatternResolver resolver=new PathMatchingResourcePatternResolver();
        Resource res=resolver.getResource("classpath:beanfactory/beans.xml");
        //创建bean工厂
        DefaultListableBeanFactory factory=new DefaultListableBeanFactory();
        //解析xml配置文件，这里相当于充当一个资源文件和bean工厂的连接
        XmlBeanDefinitionReader reader=new XmlBeanDefinitionReader(factory);
        reader.loadBeanDefinitions(res);
        //获取bean
        User user1=factory.getBean("user1",User.class);
        System.out.println(user1);
        User user2=factory.getBean("user1",User.class);
        System.out.println(user2);
    }
}
```
```

暂时记住`ResourcePatternResolver PathMatchingResourcePatternResolver DefaultListableBeanFactory XmlBeanDefinitionReader`这几个接口和类

#### ApplicationContext的简单使用

##### 写一个java config类(当然也可以是xml文件)

```java
```java
package com.zxc.demo.bean;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class Beans {
    @Bean("user")
    public User getUser() {
        User user = new User();
        user.setName("java");
        return user;
    }
}
```
```

##### 测试

```java
```java
package com.zxc.demo.bean;

import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class BeanTest2 {

    public static void main(String[] args) {
        ApplicationContext ctx=new AnnotationConfigApplicationContext(Beans.class);
        User user=ctx.getBean("user",User.class);
        System.out.println(user);
    }
}
```
```

#### BeanFactory和ApplicationContext在使用上有一个区别就是BeanFactory在初始化容器时并没有实例化Bean，只有在第一次访问才初始化，而ApplicationContext在初始化上下文时就实例化所有单实例的Bean

#### Web应用的ApplicationContext

WebApplicationContext的初始化与前面两个有所不同，它的初始化需要ServletContext实例，也就是需要web容器，所以需要在web.xml文件中配置

```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_4_0.xsd"
         version="4.0">
    <!--指定资源配置文件-->
    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>WEB-INF/test.xml</param-value>
    </context-param>

    <!--声明Web容器监听器-->
    <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>
</web-app>
```

### Bean的生命周期

#### BeanFactory中的生命周期

![在这里插入图片描述](https://images0.cnblogs.com/i/580631/201405/181453414212066.png)
![在这里插入图片描述](https://images0.cnblogs.com/i/580631/201405/181454040628981.png)
##### 实例

###### User

```java
```java
package com.zxc.demo;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.*;

public class User implements BeanFactoryAware,BeanNameAware,InitializingBean,DisposableBean {
    private String name;
    private int age;

    private BeanFactory beanFactory;
    private String beanName;

    public User(){
        System.out.println("调用构造器User()");
    }

    public void setName(String name){
        this.name=name;
        System.out.println("调用setName()设置属性");
    }

    public String getName() {
        return name;
    }

    public void setAge(int age) {
        this.age = age;
        System.out.println("调用setAge()设置属性");
    }

    public int getAge() {
        return age;
    }

    public void introduce(){
        System.out.println("I'm "+name+" I'm "+age+" years old");
    }

    public void setBeanFactory(BeanFactory beanFactory) throws BeansException {
        System.out.println("调用BeanFactoryAware.setBeanFactory()");
        this.beanFactory=beanFactory;
    }

    public void setBeanName(String s) {
        System.out.println("调用BeanNameAware.setBeanName()");
        this.beanName=s;
    }

    public void destroy() throws Exception {
        System.out.println("调用DisposableBean.destroy()");
    }

    public void afterPropertiesSet() throws Exception {
        System.out.println("调用InitializingBean.afterPropertiesSet()");
    }

    public void myInit(){
        System.out.println("调用<bean>的init-method属性所指定的方法");
        this.age=50;
    }

    public void myDestory(){
        System.out.println("调用了<bean>的destroy-method属性所指定的方法");
    }
}
```
```

###### MyInstantiationAwareBeanPostProcessor

```java
```java
package com.zxc.demo;

import org.springframework.beans.BeansException;
import org.springframework.beans.PropertyValues;
import org.springframework.beans.factory.config.InstantiationAwareBeanPostProcessorAdapter;

import java.beans.PropertyDescriptor;

public class MyInstantiationAwareBeanPostProcessor extends InstantiationAwareBeanPostProcessorAdapter {

    @Override
    public Object postProcessBeforeInstantiation(Class<?> beanClass, String beanName) throws BeansException {
        if("user".equals(beanName)){
            System.out.println("InstantiationAware BeanPostProcessor.postProcessBeforeInstantiation");
        }
        return null;
    }

    @Override
    public boolean postProcessAfterInstantiation(Object bean, String beanName) throws BeansException {
        if("user".equals(beanName)){
            System.out.println("InstantiationAware BeanPostProcessor.postProcessAfterInstantiation");
        }
        return true;
    }

    @Override
    public PropertyValues postProcessPropertyValues(PropertyValues pvs, PropertyDescriptor[] pds, Object bean, String beanName) throws BeansException {
        if("user".equals(beanName)){
            System.out.println("InstantiationAware BeanPostProcessor.postProcessPropertyValues");
        }
        return pvs;
    }
}
```
```

###### MyBeanPostProcessor

```java
```java
package com.zxc.demo;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanPostProcessor;

public class MyBeanPostProcessor implements BeanPostProcessor {
    public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
        if("user".equals(beanName)){
            User user=(User)bean;
            if(user.getName()==null){
                System.out.println("调用BeanPostProcessor.postProcessBeforeInitialization设置属性");
                user.setName("Mike");
            }
        }
        return bean;
    }

    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        if("user".equals(beanName)){
            User user=(User)bean;
            if(user.getAge()>25){
                System.out.println("调用BeanPostProcessor.postProcessAfterInitialization调整属性");
                user.setAge(25);
            }
        }
        return bean;
    }
}
```
```

###### Beans.xml

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:p="http://www.springframework.org/schema/p"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="user" class="com.zxc.demo.User" init-method="myInit" destroy-method="myDestory" p:age="50"/>
</beans>
```

###### BeanLifeCycle(测试Bean的生命周期)

```java
```java
package com.zxc.demo;

import org.springframework.beans.factory.BeanFactory;
import org.springframework.beans.factory.config.ConfigurableBeanFactory;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;
import org.springframework.beans.factory.xml.XmlBeanDefinitionReader;
import org.springframework.core.io.ClassPathResource;
import org.springframework.core.io.Resource;


public class BeanLifeCycle {
    private static void LifeCycleInBeanFactory(){
        //加载资源文件并启动容器(BeanFactory)
        Resource res=new ClassPathResource("com/zxc/demo/beanfactory/beans.xml");
        BeanFactory bf=new DefaultListableBeanFactory();
        XmlBeanDefinitionReader reader=new XmlBeanDefinitionReader((DefaultListableBeanFactory)bf);
        reader.loadBeanDefinitions(res);
        //向容器中注册MyBeanPostProcessor后处理器
        ((ConfigurableBeanFactory)bf).addBeanPostProcessor(new MyBeanPostProcessor());
        //向容器中注册NyInstantiationAwareBeanPostProcessor后处理器
        ((ConfigurableBeanFactory)bf).addBeanPostProcessor(new MyInstantiationAwareBeanPostProcessor());

        //第一次从容器中getBean，引发Bean生命周期的开始
        System.out.println("-------------------------------");
        //最先是调用了MyInstantiationAwareBeanPostProcessor的两个方法，实例化前和实例化后，中间是调用构造器实例化了一个空对象
        //然后再调用postProcessPropertyValues，然后他就凉了
        //然后就是User类的setBeanName()和setBeanFactory()
        //然后是MyBeanPostProcessor的postProcessBeforeInitialization来设置属性，设置为Mike，属性设置完就是User的afterPropertiesSet
        //然后再解析xml文件中Bean，再调用User的myInit方法，然后再设置xml文件中的age属性，通过postProcessAfterInitialization设置
        User user1=(User)bf.getBean("user");
        user1.introduce();
        user1.setName("Kitty");
        User user2=(User)bf.getBean("user");
        System.out.println("user1==user2 "+(user1==user2));
        System.out.println("-------------------------------");
        //关闭容器
        ((DefaultListableBeanFactory)bf).destroySingletons();
    }

    public static void main(String[] args) {
        LifeCycleInBeanFactory();
    }
}

###### 输出
```java
-------------------------------
InstantiationAware BeanPostProcessor.postProcessBeforeInstantiation
调用构造器User()
InstantiationAware BeanPostProcessor.postProcessAfterInstantiation
InstantiationAware BeanPostProcessor.postProcessPropertyValues
调用setAge()设置属性
调用BeanNameAware.setBeanName()
调用BeanFactoryAware.setBeanFactory()
调用BeanPostProcessor.postProcessBeforeInitialization设置属性
调用setName()设置属性
调用InitializingBean.afterPropertiesSet()
调用<bean>的init-method属性所指定的方法
调用BeanPostProcessor.postProcessAfterInitialization调整属性
调用setAge()设置属性
I'm Mike I'm 25 years old
user1==user2 true
-------------------------------
调用DisposableBean.destroy()
调用了<bean>的destroy-method属性所指定的方法
```
```

其实我也不知道在写什么…

java真是世界上最垃圾的语言，Spring就是世界上最垃圾的框架，没有之一

方法名长得跟鼻屎一样
#### ApplicationContext的生命周期

```java
```java
package com.zxc.demo;

import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.beans.factory.config.BeanFactoryPostProcessor;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;

public class MyBeanFactoryPostProcessor implements BeanFactoryPostProcessor {
    public void postProcessBeanFactory(ConfigurableListableBeanFactory bf){
        BeanDefinition bd=bf.getBeanDefinition("user");
        bd.getPropertyValues().addPropertyValue("name","Simon");
        System.out.println("调用BeanFactoryPostProcessor.postProcessBeanFactory()");
    }
}
```
```

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:p="http://www.springframework.org/schema/p"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="user" class="com.zxc.demo.User" init-method="myInit" destroy-method="myDestory"/>
    <bean id="myBeanPostProcessor" class="com.zxc.demo.MyBeanPostProcessor"/>
    <bean id="myBeanFactoryPostProcessor" class="com.zxc.demo.MyBeanFactoryPostProcessor"/>
</beans>
```

分界线，再看了一遍花了很久重新实现了一下

先给代码

User类
```java
```java
package com.zxc.demo.easyTest;

import org.springframework.beans.BeansException;
import org.springframework.beans.PropertyValues;
import org.springframework.beans.factory.*;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.beans.factory.config.InstantiationAwareBeanPostProcessor;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;

import java.beans.PropertyDescriptor;

public class User implements BeanNameAware, BeanFactoryAware, ApplicationContextAware, InitializingBean, DisposableBean, InstantiationAwareBeanPostProcessor, BeanPostProcessor {
    private String name;
    private int age;

    //2
    public User(String name) {
        this.name = name;
        System.out.println("2:构造器");
    }

    //5
    public void setName(String name) {
        System.out.println("5:设置name");
        this.name = name;
    }

    public void setAge(int age) {
        System.out.println("5:设置age");
        this.age = age;
    }

    //用来使Bean获得BeanName
    //6
    public void setBeanName(String s) {
        System.out.println("6:setBeanName");
    }

    //用来使Bean获得IoC容器(BeanFactory和ApplicationContext)
    //7
    public void setBeanFactory(BeanFactory beanFactory) throws BeansException {
        System.out.println("7:setBeanFactory");
    }

    //8
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        System.out.println("8:setApplicationContext");
    }


    //10
    public void afterPropertiesSet() throws Exception {
        System.out.println("10:afterPropertiesSet");
    }

    //11
    public void Myinit() {
        System.out.println("11:Init methods");
    }

    //到这里，如果Bean的作用范围是prototype,那么Bean将被返回给调用者,Spring不再管理该Bean
    //如果作用范围是singleton(默认),那么Spring将Bean放入缓存池并返回给调用者，并负责后期生命周期管理

    //只有singleton的Bean才会调用以下两个方法,
    //13
    public void destroy() throws Exception {
        System.out.println("13:destroy");
    }

    //14
    public void Mydestroy() {
        System.out.println("14:Destroy methods");
    }

    public String getName() {
        return name;
    }

    public int getAge() {
        return age;
    }
}
```
```

然后是两个自定义后处理器类

这里有很多坑…一个是实现接口，一个是继承父类，然后都要实现Ordered接口（解决相同接口实现类优先级问题）才可以，然后就应该在配置文件中分别配置这样两个bean，这样容器启动时ApplicationContext会自动扫描并注册这两个处理器类

```java
```java
package com.zxc.demo.easyTest;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.core.Ordered;

public class myBeanPostProcessor implements BeanPostProcessor,Ordered {
    public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
        System.out.println("9:自定义后处理器,postProcessBeforeInitialization方法");
        return null;
    }

    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        System.out.println("12:自定义后处理器,postProcessAfterInitialization方法");
        return null;
    }

    //要实现这个方法容器才能扫描到处理器
    public int getOrder() {
        return 1;
    }
}
```
```

```java
```java
package com.zxc.demo.easyTest;

import org.springframework.beans.BeansException;
import org.springframework.beans.PropertyValues;
import org.springframework.beans.factory.config.InstantiationAwareBeanPostProcessor;
import org.springframework.beans.factory.config.InstantiationAwareBeanPostProcessorAdapter;
import org.springframework.core.Ordered;

import java.beans.PropertyDescriptor;

//不直接实现InstantiationAwareBeanPostProcessor接口而是继承了InstantiationAwareBeanPostProcessorAdapter类
public class myInstantiationAwareBeanPostProcessor extends InstantiationAwareBeanPostProcessorAdapter implements Ordered {

    @Override
    public Object postProcessBeforeInstantiation(Class<?> beanClass, String beanName) throws BeansException {
        System.out.println("1:自定义后处理器myInstantiationAwareBeanPostProcessor的postProcessBeforeInstantiation");
        return super.postProcessBeforeInstantiation(beanClass, beanName);
    }

    @Override
    public PropertyValues postProcessPropertyValues(PropertyValues pvs, PropertyDescriptor[] pds, Object bean, String beanName) throws BeansException {
        System.out.println("4:自定义后处理器myInstantiationAwareBeanPostProcessor的postProcessPropertyValues");
        return super.postProcessPropertyValues(pvs, pds, bean, beanName);
    }

    @Override
    public boolean postProcessAfterInstantiation(Object bean, String beanName) throws BeansException {
        System.out.println("3:自定义后处理器myInstantiationAwareBeanPostProcessor的postProcessAfterInitialization");
        return super.postProcessAfterInstantiation(bean, beanName);
    }

    public int getOrder() {
        return 1;
    }
}
```
```

然后是配置文件

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
    <bean id="user" class="com.zxc.demo.easyTest.User" init-method="Myinit" destroy-method="Mydestroy" p:age="20">
        <constructor-arg value="hello"/>
    </bean>
    <bean id="myBeanPostProcessor" class="com.zxc.demo.easyTest.myBeanPostProcessor"/>
    <bean id="myInstantiationAwareBeanPostProcessor" class="com.zxc.demo.easyTest.myInstantiationAwareBeanPostProcessor"/>
</beans>
```

最后就是测试类了，测试ApplicationContext容器的getBean

```java
```java
package com.zxc.demo.easyTest;


import org.junit.Test;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class BeanTest {
    @Test
    public void test(){
        //通过ApplicationContext容器装配bean，观察bean的声明周期
        ClassPathXmlApplicationContext ctx=new ClassPathXmlApplicationContext("bean.xml");
        User user=ctx.getBean("user",User.class);
        //user.setName("nonono");
        System.out.println(user);
        ctx.close();
    }
}
```
```

## Bean的具体装配(重点)

上个部分基本都是讲一些基础概念和比较玄学的代码

这节主要将Bean具体如何装配

### XML配置









