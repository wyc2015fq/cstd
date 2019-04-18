# spring-boot5 - weixin_33985507的博客 - CSDN博客
2018年03月15日 21:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
```
Spring Boot集成MyBatis：
（1）新建maven project;取名为：spring-boot-mybatis
（2）在pom.xml文件中引入相关依赖；
（3）创建启动类App.java
（4）在application.properties添加配置文件；
（5）编写Demo测试类;
（6）编写DemoMapper；
（7）编写DemoService
（8）编写DemoController;
（9）加入PageHelper
（10）获取自增长ID;
在pom.xml文件中引入相关依赖
（1）基本依赖，jdk版本号；
（2）mysql驱动，mybatis依赖包，mysql分页PageHelper:
<!-- mysql 数据库驱动. -->
    <dependency>
        <groupId>mysql</groupId>
        <artifactId>mysql-connector-java</artifactId>
</dependency>
```
```
<!--          spring-boot mybatis依赖：
         请不要使用1.0.0版本，因为还不支持拦截器插件，
            1.1.1 是博主写帖子时候的版本，大家使用最新版本即可
         -->
    <dependency>
        <groupId>org.mybatis.spring.boot</groupId>
        <artifactId>mybatis-spring-boot-starter</artifactId>
        <version>1.1.1</version>
    </dependency>
<!-- 
        MyBatis提供了拦截器接口，我们可以实现自己的拦截器，
        将其作为一个plugin装入到SqlSessionFactory中。 
        Github上有位开发者写了一个分页插件，我觉得使用起来还可以，挺方便的。 
        Github项目地址： https://github.com/pagehelper/Mybatis-PageHelper
     -->    
    <dependency>
        <groupId>com.github.pagehelper</groupId>
        <artifactId>pagehelper</artifactId>
        <version>4.1.0</version>
    </dependency>
```
```
创建启动类App.java
@SpringBootApplication
@MapperScan("com.kfit.*.mapper")
public class App {
    public static void main(String[] args) {
        SpringApplication.run(App.class, args);
    }
}
//这里和以往不一样的地方就是MapperScan的注解，这个是会扫描该包下的接口
```
```
在application.properties添加配置文件
###datasource
spring.datasource.url = jdbc:mysql://localhost:3306/test
spring.datasource.username = root
spring.datasource.password = root
spring.datasource.driverClassName = com.mysql.jdbc.Driver
spring.datasource.max-active=20
spring.datasource.max-idle=8
spring.datasource.min-idle=8
spring.datasource.initial-size=10
```
```
编写Demo测试类
public class Demo {
    private long id;
    private String name;
      //省略getter and setter….
}
编写DemoMapper
public interface DemoMappper {
    
    @Select("select *from Demo where name = #{name}")
    public List<Demo> likeName(String name);
    
    @Select("select *from Demo where id = #{id}")
    public Demo getById(long id);
    
    @Select("select name from Demo where id = #{id}")
    public String getNameById(long id);
}
```
```
编写DemoService
@Service
public class DemoService {
    @Autowired
    private DemoMappper demoMappper;
    
    public List<Demo> likeName(String name){
        return demoMappper.likeName(name);
    }
}
编写DemoController
@RestController
public class DemoController {
    @Autowired
    private DemoService demoService;
    @RequestMapping("/likeName")
    public List<Demo> likeName(String name){
        return demoService.likeName(name);
    }
}
//运行访问：http://127.0.0.1:8080/likeName?name=张三  就可以看到返回的数据了
```
```
加入PageHelper
@Configuration
public class MyBatisConfiguration {
    @Bean
    public PageHelper pageHelper() {
System.out.println("MyBatisConfiguration.pageHelper()");
        PageHelper pageHelper = new PageHelper();
        Properties p = new Properties();
        p.setProperty("offsetAsPageNum", "true");
        p.setProperty("rowBoundsWithCount", "true");
        p.setProperty("reasonable", "true");
        pageHelper.setProperties(p);
        return pageHelper;
    }
}
```
```
加入PageHelper
@RequestMapping("/likeName")
public List<Demo> likeName(String name){
         PageHelper.startPage(1,1);
         return demoService.likeName(name);
}
获取自增长ID
@Insert("insert into Demo(name,password) values(#{name},#{password})")
public long save(Demo name);
获取自增长ID
@Options(useGeneratedKeys = true, keyProperty = "id", keyColumn = "id")
```
![](https://images2018.cnblogs.com/blog/486468/201803/486468-20180315214857051-1554086144.png)
端口在使用，是因为之前的eclips应用没有关闭，
![](https://images2018.cnblogs.com/blog/486468/201803/486468-20180315214922978-1851395016.png)
![](https://images2018.cnblogs.com/blog/486468/201803/486468-20180315214932768-892086330.png)
