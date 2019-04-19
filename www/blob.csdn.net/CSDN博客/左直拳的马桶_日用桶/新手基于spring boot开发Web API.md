# 新手基于spring boot开发Web API - 左直拳的马桶_日用桶 - CSDN博客
2018年12月18日 19:18:26[左直拳](https://me.csdn.net/leftfist)阅读数：143
就是很爽，[感觉比asp.net](http://xn--asp-404fr68bxy7b.net) mvc要灵活。
爽在哪里呢？
觉得路由定义比较灵活，而且很强大。我是JAVA初学者，以spring boot提供Web API为例，做一些总结。
**一、生成一个Spring Boot项目**
用idea来创建，好像不成功。原因好像是说有个百度验证，所以生成不了。那就到spring boot官网上生成一个，然后拿回本地，再打开。
**二、代码结构**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218164345781.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
spring boot的源码结构，应该都是
```
/src/main/java
/src/main/resoures
```
这种，估计是约定大于配置吧
**三、添加一个API**
**1、新建一个控制器：HelloWorldController**
控制器嘛，负责与页面（视图）交互，在页面与Model（业务逻辑）之间传递消息，打打酱油。这属于MVC的范畴。当然，在SpringBoot里面，并不是说你这个类名叫"***Controller"就会被自动当成是个控制器，没有这种约定的（[但在asp.net](http://xn--asp-y39dk47a.net) mvc里就有，当然要放在适当的文件夹里才生效）。之所以会被访问到，是注解在起作用。SpringBoot就是喜欢用注解。
```java
package api.controller;
import api.entity.Author;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
@Controller
@RequestMapping(value="/api")
public class HelloWorldController {
    @Autowired
    private Author entity;
    @ResponseBody
    @RequestMapping(value = "/helloworld", method = RequestMethod.GET)
    public String hello(){//返回JSON
        return "Hello World!";
    }
    @ResponseBody
    @RequestMapping(value = "/helloworld2", method = RequestMethod.GET)
    public Author hello2(){//返回JSON
        return entity;
    }
    @RequestMapping(value = "/helloworld3", method = RequestMethod.GET)
    public String hello3(Model model) {//返回页面
        model.addAttribute("loginName", entity.getName());
        model.addAttribute("loginId", "100");
        return "helloworld";
    }
}
```
**2、注解说明**
`@Controller` 与 `@ResetController` 有什么区别？
答曰：@Controller返回页面，而@ResetController返回JSON。并且
```
@ResetController = @Controller + @ResponseBody
```
`@Controller` 与 `@ResetController` 都是类级注解，而`@ResponseBody`可以是类级，也可以是函数级。
所以，如果一个控制器，既有函数返回页面，又有函数返回JSON，那么可以将类加上`@Controller` 注解，想返回JSON的函数再加一个`@ResponseBody`。如上面的代码所示。
**3、json**
我发现spring boot里返回一个json特别简单，直接将一个实体类返回，不用做什么转换。如上面的例子：
```java
@Autowired
    private Author entity;
    @ResponseBody
    @RequestMapping(value = "/helloworld2", method = RequestMethod.GET)
    public Author hello2(){//返回JSON
        return entity;
    }
```
这个author类
```java
package api.entity;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;
import lombok.Getter;
import lombok.Setter;
@Component
public class Author {
    @Value("${author.name:chenqu}")
    private @Getter @Setter String name;
    @Value("${author.desc:stupid}")
    private @Getter @Setter String desc;
}
```
这个实体类：
**1）用到了@Getter和@Setter来简化属性设置**。这是一个第三方的插件。需要修改build.gradle
```
plugins {
    id 'io.franzbecker.gradle-lombok' version '1.8'//lombok，简化get和set
    id 'java'
}
```
**2）用到了配置文件信息**
application.properties
```
author.name = \u9648\u9A71
author.desc = \u50BB\u903C
```
**4、模板文件**
页面用到了模板文件。
这个模板文件要放在`src/main/resources/templates/`下。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218190201258.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
hello.html
```
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml"
      xmlns:th="http://www.thymeleaf.org" xmlns:layout="http://www.ultraq.net.nz/thymeleaf/layout">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></meta>
    <title>im test page</title>
</head>
<body>
<div>
  loginId:<span th:text="${loginId}"></span>
     loginName:<span th:text="${loginName}"></span>
</div>
<div>Hello World!</div>
</body>
</html>
```
如上面所示，控制器代码为：
```java
@RequestMapping(value = "/helloworld3", method = RequestMethod.GET)
    public String hello3(Model model) {//返回页面
        model.addAttribute("loginName", entity.getName());
        model.addAttribute("loginId", "100");
        return "helloworld";
    }
```
模板文件采用的框架是thymeleaf：
```
//thymeleaf
    compile("org.springframework.boot:spring-boot-starter-thymeleaf")
```
**5、运行结果**
1）返回字符串
![在这里插入图片描述](https://img-blog.csdnimg.cn/201812181910170.png)
2）返回json
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218191036188.png)
3）返回页面
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218191100414.png)
**6、完整的application.properties和build.gradle**
**1）application.properties**
```
banner.charset=UTF-8
server.tomcat.uri-encoding=UTF-8
spring.http.encoding.charset=UTF-8
spring.http.encoding.enabled=true
spring.http.encoding.force=true
spring.messages.encoding=UTF-8
spring.profiles.active=prod
server.port=8081
spring.thymeleaf.cache=false
author.name = \u9648\u9A71
author.desc = \u50BB\u903C
spring.jpa.database=oracle
spring.datasource.driver-class-name=oracle.jdbc.driver.OracleDriver
spring.datasource.url=jdbc:oracle:thin:@192.168.0.22:1522/pdbzjfwpt
spring.datasource.username=jczs
spring.datasource.password=jczs
spring.jpa.hibernate.ddl-auto=update
mybatis.typeAliasesPackage=api.entity
```
**2）build.gradle**
```
buildscript {
    repositories {
        jcenter()
    }
    dependencies {
        classpath("org.springframework.boot:spring-boot-gradle-plugin:1.5.2.RELEASE")
    }
}
plugins {
    id 'io.franzbecker.gradle-lombok' version '1.8'//lombok，简化get和set
    id 'java'
}
repositories {
    jcenter()
}
apply plugin: 'java'
apply plugin: 'org.springframework.boot'
apply plugin: 'eclipse'
apply plugin: 'war'//生成war包
sourceCompatibility = 1.8
targetCompatibility = 1.8
task wrapper(type: Wrapper) {
    gradleVersion = '3.0'
}
dependencies {
    compile("org.springframework.boot:spring-boot-starter-web")
    testCompile("org.springframework.boot:spring-boot-starter-test")
    //部署到外部tomcat
    providedCompile("org.springframework.boot:spring-boot-starter-tomcat")
    //thymeleaf
    compile("org.springframework.boot:spring-boot-starter-thymeleaf")
    //oracle
    compile("com.oracle:ojdbc7:12.1.0.1")
    //mybatis
    compile("org.mybatis.spring.boot:mybatis-spring-boot-starter:1.3.0")
    testCompile('org.mybatis.spring.boot:mybatis-spring-boot-starter-test:1.3.0')
}
```
