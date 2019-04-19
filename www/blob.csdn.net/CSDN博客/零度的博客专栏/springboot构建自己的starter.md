# springboot构建自己的starter - 零度的博客专栏 - CSDN博客
2018年04月16日 17:31:53[零度anngle](https://me.csdn.net/zmx729618)阅读数：173标签：[spring-boot																[自定义starter](https://so.csdn.net/so/search/s.do?q=自定义starter&t=blog)](https://so.csdn.net/so/search/s.do?q=spring-boot&t=blog)
个人分类：[spring boot](https://blog.csdn.net/zmx729618/article/category/7595492)
前言
下面我们来通过示例来构建自己的starter pom并完成自动配置，以此来更深入的理解spring boot的工作原理。
需求
该starter提供PersonService，并实现自动配置PersonService
一、新建一个Maven工程
工程pom依赖如下：
**[java]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- <?xml version="1.0" encoding="UTF-8"?>  
- <project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-     xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">  
-     <modelVersion>4.0.0</modelVersion>  
- 
-     <groupId>com.chhliu.springboot.starter.helloworld</groupId>  
-     <artifactId>springboot-starter-helloworld</artifactId>  
-     <version>0.0.1-SNAPSHOT</version>  
-     <packaging>jar</packaging>  
-     <properties>  
-         <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>  
-         <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>  
-         <java.version>1.8</java.version>  
-     </properties>  
- 
-     <dependencies>  
-         <dependency><!-- 以下两个依赖是自动配置的依赖 -->  
-             <groupId>org.springframework.boot</groupId>  
-             <artifactId>spring-boot-autoconfigure</artifactId>  
-             <version>1.4.3.RELEASE</version>  
-         </dependency>  
- 
-         <dependency>  
-             <groupId>org.springframework.boot</groupId>  
-             <artifactId>spring-boot-configuration-processor</artifactId>  
-             <version>1.4.3.RELEASE</version>  
-             <optional>true</optional>  
-         </dependency>  
-     </dependencies>  
- 
-     <build>  
-         <plugins>  
-             <plugin>  
-                 <groupId>org.springframework.boot</groupId>  
-                 <artifactId>spring-boot-maven-plugin</artifactId>  
-             </plugin>  
-         </plugins>  
-     </build>  
- </project>  
二、创建自动配置类
有对这部分不了解的，可以参考我的另一篇博客[spring boot中多环境配置支持](http://blog.csdn.net/liuchuanhong1/article/details/54694458)
**[java]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- package com.chhliu.springboot.starter.helloworld;  
- 
- import org.springframework.boot.context.properties.ConfigurationProperties;  
- 
- /**
-  * 描述：人员信息自动配置属性类
-  * @author chhliu
-  * 创建时间：2017年2月13日 下午9:05:34
-  * @version 1.2.0
-  */
- @ConfigurationProperties(prefix="person.proterties.set")// 定义application.properties配置文件中的配置前缀
- publicclass PersonServiceProperties {  
- 
- // 姓名
- private String name;  
- // 年龄
- privateint age;  
- // 性别，不配置的时候默认为person.proterties.set=man
- private String sex = "man";  
- // 身高
- private String height;  
- // 体重
- private String weight;  
- 
-     ……省略getter，setter方法……  
- }  
三、编写服务类
**[java]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- package com.chhliu.springboot.starter.helloworld.service;  
- 
- import com.chhliu.springboot.starter.helloworld.PersonServiceProperties;  
- 
- publicclass PersonService {  
- 
- private PersonServiceProperties properties;  
- 
- public PersonService(PersonServiceProperties properties){  
- this.properties = properties;  
-     }  
- 
- public PersonService(){  
- 
-     }  
- 
- public String getPersonName(){  
- return properties.getName();  
-     }  
- 
- publicint getPersonAge(){  
- return properties.getAge();  
-     }  
- 
- public String getPersonSex(){  
- return properties.getSex();  
-     }  
- }  
四、自动配置类
**[java]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- package com.chhliu.springboot.starter.helloworld.configuration;  
- 
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.boot.autoconfigure.condition.ConditionalOnClass;  
- import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;  
- import org.springframework.boot.autoconfigure.condition.ConditionalOnProperty;  
- import org.springframework.boot.context.properties.EnableConfigurationProperties;  
- import org.springframework.context.annotation.Bean;  
- import org.springframework.context.annotation.Configuration;  
- 
- import com.chhliu.springboot.starter.helloworld.PersonServiceProperties;  
- import com.chhliu.springboot.starter.helloworld.service.PersonService;  
- 
- @Configuration// 配置注解
- @EnableConfigurationProperties(PersonServiceProperties.class) // 开启指定类的配置
- @ConditionalOnClass(PersonService.class)// 当PersonService这个类在类路径中时，且当前容器中没有这个Bean的情况下，开始自动配置
- @ConditionalOnProperty(prefix="person.proterties.set", value="enabled", matchIfMissing=true)// 指定的属性是否有指定的值
- publicclass PersonServiceAutoConfiguration {  
- @Autowired
- private PersonServiceProperties properties;  
- 
- @Bean
- @ConditionalOnMissingBean(PersonService.class)// 当容器中没有指定Bean的情况下，自动配置PersonService类
- public PersonService personService(){  
-         PersonService personService = new PersonService(properties);  
- return personService;  
-     }  
- }  
五、注册配置
1、在src/main/resources新建META-INF文件夹
2、在META-INF文件夹下新建spring.factories文件
注意：spring.factories并非是必须的，可以在启动类上添加如下注解进行自动配置
**[java]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- @ImportAutoConfiguration({PersonServiceAutoConfiguration.class})  
3、注册配置自动配置类
**[java]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- org.springframework.boot.autoconfigure.EnableAutoConfiguration=com.chhliu.springboot.starter.helloworld.configuration.PersonServiceAutoConfiguration  
六、将上面构建的starter安装到本地
**[java]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- 安装命令如下：  
- mvn clean install  
七、新建spring boot项目，并将上面的starter作为依赖
**[html]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
- <modelVersion>4.0.0</modelVersion>
- 
- <groupId>com.chhliu.springboot.starter.person</groupId>
- <artifactId>springboot-starter-person</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <packaging>jar</packaging>
- <parent>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-starter-parent</artifactId>
- <version>1.5.1.RELEASE</version>
- <relativePath/><!-- lookup parent from repository -->
- </parent>
- 
- <properties>
- <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
- <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
- <java.version>1.8</java.version>
- </properties>
- 
- <dependencies>
- <dependency>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-starter</artifactId>
- </dependency>
- 
- <dependency>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-starter-web</artifactId>
- </dependency>
- 
- <dependency><!-- 引入自己的starter -->
- <groupId>com.chhliu.springboot.starter.helloworld</groupId>
- <artifactId>springboot-starter-helloworld</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- </dependency>
- 
- <dependency>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-starter-test</artifactId>
- <scope>test</scope>
- </dependency>
- </dependencies>
- 
- <build>
- <plugins>
- <plugin>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-maven-plugin</artifactId>
- </plugin>
- </plugins>
- </build>
- </project>
八、添加属性到application.properties配置文件中
**[java]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- person.proterties.set.name=chhliu  
- person.proterties.set.age=28
- person.proterties.set.sex=woman  
- person.proterties.set.height=160
- person.proterties.set.weight=50kg  
九、编写Controller
**[java]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- package com.chhliu.springboot.starter.person.controller;  
- 
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.web.bind.annotation.GetMapping;  
- import org.springframework.web.bind.annotation.RestController;  
- 
- import com.chhliu.springboot.starter.helloworld.service.PersonService;  
- 
- @RestController
- publicclass PersonServiceController {  
- @Autowired// 自动注入PersonService
- private PersonService personService;  
- 
- @GetMapping("get/name")  
- public String getPersonName(){  
- return personService.getPersonName();// 调用PersonService服务的方法
-     }  
- 
- @GetMapping("get/sex")  
- public String getPersonSex(){  
- return personService.getPersonSex();  
-     }  
- }  
十、测试
在浏览器中输入http://localhost:8080/get/sex
测试结果如下：
**[java]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- woman  
发现我们在使用我们自定义的starter过程中，并没有创建PersonService实例对象，直接通过@Autowired注解就实现了自动注入，而真正的自动注入过程则是在starter中实现的，通过上面的几个步骤，就实现了自己构建自己的starter，在spring boot中，除了官方提供的starter之外，第三方也提供了不少的starter，通过这种方式，极大的对spring boot进行的扩展
十一、延伸知识
在构建starter的过程中，涉及到一些注解，这里也做一个统一的说明
**[java]**[view plain](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)[copy](https://blog.csdn.net/liuchuanhong1/article/details/55057135#)
- @ConditionalOnBean:当容器中有指定的Bean的条件下  
- @ConditionalOnClass：当类路径下有指定的类的条件下  
- @ConditionalOnExpression:基于SpEL表达式作为判断条件  
- @ConditionalOnJava:基于JVM版本作为判断条件  
- @ConditionalOnJndi:在JNDI存在的条件下查找指定的位置  
- @ConditionalOnMissingBean:当容器中没有指定Bean的情况下  
- @ConditionalOnMissingClass:当类路径下没有指定的类的条件下  
- @ConditionalOnNotWebApplication:当前项目不是Web项目的条件下  
- @ConditionalOnProperty:指定的属性是否有指定的值  
- @ConditionalOnResource:类路径下是否有指定的资源  
- @ConditionalOnSingleCandidate:当指定的Bean在容器中只有一个，或者在有多个Bean的情况下，用来指定首选的Bean  
- @ConditionalOnWebApplication:当前项目是Web项目的条件下  
