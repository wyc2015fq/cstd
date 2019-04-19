# 【SpringBoot】10分钟快速搭建ssm框架 - Big Smile - CSDN博客
2018年02月25日 21:31:01[王啸tr1912](https://me.csdn.net/tr1912)阅读数：9466
        弄了这么长时间的springboot了，现在来实战一把，搭个框架吧，只要是对于maven项目比较熟悉的人都可以在10分钟左右搭建成功这个框架哟。
       使用工具：Idea，maven3.9
       前提：idea配置好maven仓库以及连接好nexus私服。
# 一、建立一个maven的web项目
        在这里我们用idea自带的spring套件来构建一个项目。首先打开idea，然后点击菜单上的file，new，project。
弹出一个建立项目的页面，我们选择用spring initializr来初始化一个springboot项目
![](https://img-blog.csdn.net/2018022520403031)
然后点击下一步，输入常见的包名，项目名，版本等信息，即可点击下一步：
![](https://img-blog.csdn.net/20180225204413968)
然后我们就可以看到一个选择依赖的页面，里面提供了很多常见的依赖，我们想要建立一个web项目，必须要选择web下面的web，然后就可以引入之前说的springboot-starter里面的web案例了。
![](https://img-blog.csdn.net/20180225204635986)
我选择的是如下几项：
![](https://img-blog.csdn.net/20180225204801189)
然后点击下一步，确认一下包名和项目名，我们就可以直接创建成功项目了，如图为创建的项目目录：
![](https://img-blog.csdn.net/20180225204944382)
# 二、建立数据库和数据依赖
        我们想要搭建好一个完整的项目，需要从请求到数据都要跑通才行，所以第二步我们要从建立数据库和写依赖开始。
我们建立一个数据库，只有一张表，sql如下：
```
SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'id',
  `username` varchar(255) DEFAULT NULL COMMENT '姓名',
  `age` int(11) DEFAULT NULL COMMENT '年龄',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
```
然后录入一条数据：
![](https://img-blog.csdn.net/20180225205450195)
这样数据库和数据我们就弄好了。
        接下来，我们需要建立mybatis需要依赖的接口和mapper的xml文件，这里为了简化，我们就不写service层了，如图我们要建立这些文件。
![](https://img-blog.csdn.net/20180225210454599)
首先是实体user，在domain层：
```java
public class User {
    private Integer id;
    private String username;
    private Integer age;
    public Integer getId() {
        return id;
    }
    public void setId(Integer id) {
        this.id = id;
    }
    public String getUsername() {
        return username;
    }
    public void setUsername(String username) {
        this.username = username;
    }
    public Integer getAge() {
        return age;
    }
    public void setAge(Integer age) {
        this.age = age;
    }
    @Override
    public String toString() {
        return "User{" +
                "id=" + id +
                ", username='" + username + '\'' +
                ", age=" + age +
                '}';
    }
}
```
然后是dao的usermapper.java:
```java
@Mapper     //声明是一个Mapper,与springbootApplication中的@MapperScan二选一写上即可
@Repository
public interface UserMapper {
    User selectUserByName(String name);
}
```
然后是controller层：
```java
@RestController
public class UserController {
    //依赖注入
    @Autowired
    UserMapper userMapper;
    @RequestMapping(value = "pic")
    public User pic() {
        //调用dao层
        User user = userMapper.selectUserByName("wx");
        return user;
    }
}
```
最后是mapper的xml配置，在resources资源文件夹下建立一个mapper文件夹，然后建立文件：
```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
<mapper namespace="com.tr1912.ssmspringboot.springboottest.dao.UserMapper" >
    <select id="selectUserByName"  resultType="User">
        SELECT * FROM user WHERE username = #{name}
    </select>
</mapper>
```
这样我们的代码就写完了。
# 三、数据库连接、服务器配置
       在这里，我需要说明一下springboot的项目结构核心，就是两个文件，而且是必须要有的：
![](https://img-blog.csdn.net/201802252113240)
        第一个就是只有main函数的*application.java文件，这个就是springboot项目的启动入口，我们在这里启动main函数，就可以让整个项目启动了。
        第二个，application.properties，这个配置文件的名字是不能变的，因为这个是springboot的约定，只要是关于springboot自动加载的东西，都是在这里进行配置的，比如数据库连接，mapper的地址，还有tomcat的端口等等。
我们项目可以看一下pom文件，如果在创建项目的时候没有选择sql数据连接和mybatis的话，这里需要加上依赖，如果已经存在请忽略：
```
<dependency>
    <groupId>org.mybatis.spring.boot</groupId>
    <artifactId>mybatis-spring-boot-starter</artifactId>
    <version>1.3.1</version>
    </dependency>
<dependency>
    <groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
    <scope>runtime</scope>
</dependency>
```
然后我们需要在application.properties中配置我们的连接和一些其他参数：
```
#tomcat端口
server.port=8088
#数据连接
spring.datasource.url=jdbc:mysql://   #数据库连接
spring.datasource.username=root
spring.datasource.password=***        #数据库密码
spring.datasource.driverClassName=com.mysql.jdbc.Driver
#Mybatis扫描
mybatis.mapper-locations=classpath*:mapper/*.xml
#起别名。可省略写mybatis的xml中的resultType的全路径
mybatis.type-aliases-package=com.tr1912.ssmspringboot.springboottest.domain
```
数据库连接自己搞一下，然后就结束了。
最后，我们在启动入口的*application.java文件中，直接run javaapplication 就可以了，我这里是：
![](https://img-blog.csdn.net/20180225212905101)
![](https://img-blog.csdn.net/20180225212829831)
然后启动成功之后，在浏览器输入http://127.0.0.1:8088/pic，我们就可以看到查询到的数据了：
![](https://img-blog.csdn.net/20180225213012655)
这样，我们的ssm框架就搭建成功了。
