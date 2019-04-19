# 【SpringBoot】java配置方式 - Big Smile - CSDN博客
2018年01月21日 23:13:49[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1017
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
资源地址：[http://download.csdn.net/download/tr1912/10214314](http://download.csdn.net/download/tr1912/10214314)
在了解SpringBoot之前，我们首先了解一下java的配置方式。
        回顾以前我们想要配置一个项目要如何配置呢？是不是需要写一堆配置文件，比如web.xml，configuration.xml，等等，这个时候，我们如果想要创建一个spring的项目的话，比如一个简单的ssh项目，需要写这些文件，并且需要运行一下空项目，以保证我们的构建能够正常进行之后才能上手使用。但是这些配置文件的配置只要有一个地方写错了，那么就会导致项目跑不起来了。
![](https://img-blog.csdn.net/20180121224045549?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        然而在最新版的java中，我们可以不使用这些配置文件进行配置了，转而把这些配置项写入.java的类文件中进行管理，这是怎么实现的呢？
        要实现这个方式配置我们的项目，我们需要用到新的注解特性，@Configuration，@ComponentScan，@Bean这些，还有可能使用到@value的注解。
      了解这些之前，我们先看一个项目，项目在连接中，关于maven项目的pom配置这里不再多说了，核心的就是一个普通的spring框架的配置：
```
<dependency>
	<groupId>org.springframework</groupId>
	<artifactId>spring-webmvc</artifactId>
	<version>4.3.7.RELEASE</version>
</dependency>
```
然后我们构建一个最最简单的三层查询模型：
实体User：
```java
public class User {
    private String username;
    private String password;
    private Integer age;
    public String getUsername() {
        return username;
    }
    public void setUsername(String username) {
        this.username = username;
    }
    public String getPassword() {
        return password;
    }
    public void setPassword(String password) {
        this.password = password;
    }
    public Integer getAge() {
        return age;
    }
    public void setAge(Integer age) {
        this.age = age;
    }
}
```
service层：
```java
@Service
public class UserService {
    @Autowired // 注入Spring容器中的bean对象
    private UserDAO userDAO;
    public List<User> queryUserList() {
        // 调用userDAO中的方法进行查询
        return this.userDAO.queryUserList();
    }
}
```
DAO层：
```java
public class UserDAO {
    public List<User> queryUserList(){
        List<User> result = new ArrayList<User>();
        // 模拟数据库的查询
        for (int i = 0; i < 10; i++) {
            User user = new User();
            user.setUsername("username_" + i);
            user.setPassword("password_" + i);
            user.setAge(i + 1);
            result.add(user);
        }
        return result;
    }
}
```
这里为了简化查询过程，所以没有连接真正的数据库，直接返回的数据集合。
        这里我们可以看到，在service层的class上有一个@service的注解，标志着这个类已经加载为了spring的一个bean，它里面调用了dao层，但是dao层并没有被spring所管理，这个时候要如何配置他去被spring管理呢？
以前的做法，在applicationContext.xml里面加上：
```java
<bean id="userDao" class="包名.UserDao">
	<property name="sessionFactory" ref="sessionFactory"></property>
</bean>
```
但是现在我们有更加简单的方法去创建这个，通过在java语句中添加一个配置的java文件来实现：
```java
@Configuration //通过该注解来表明该类是一个Spring的配置，相当于一个xml文件
@ComponentScan(basePackages = "cn.itcast.springboot.javaconfig") //配置扫描包
public class SpringConfig {
    
    @Bean // 通过该注解来表明是一个Bean对象，相当于xml中的<bean>
    public UserDAO getUserDAO(){
        return new UserDAO(); // 直接new对象做演示
    }
    
}
```
我们可以注意到class前面的两个注解，一个是@configuration，标志这个java文件为java配置项文件，而@ComponentScan标志的是我们创建的以下bean都来自于哪个包。
        然后我们可以在配置类里面声明一个getDAO层对象的方法，返回一个dao对象，同时在上面加上了@Bean注解，和上面的老方法在xml文件中写bean标签进行dao层对象的bean创建是一个作用，这时我们就可以把这些不是非得都放在xml中的配置直接在java文件中用程序的方法写出来了，再也不用担心写错大小写而导致的配置不可用，或者bean创建不成功的错误了。（因为都是引入的关联）。
最后我们可以写一个main入口函数来调用service层，看是否可以调用到dao层：
```java
public class Main {
    
    public static void main(String[] args) {
        // 通过Java配置来实例化Spring容器
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(SpringConfig.class);
        
        // 在Spring容器中获取Bean对象
        UserService userService = context.getBean(UserService.class);
        
        // 调用对象中的方法
        List<User> list = userService.queryUserList();
        for (User user : list) {
            System.out.println(user.getUsername() + ", " + user.getPassword() + ", " + user.getPassword());
        }
        
        // 销毁该容器
        context.destroy();
    }
}
```
这样，最后就可以把写在dao的数据输出了，表明已经成功创建了dao的bean，被service使用了。
![](https://img-blog.csdn.net/20180121231314013?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
