# Spring系列学习之使用Spring Boot构建RESTful Web服务 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 01:43:09[boonya](https://me.csdn.net/boonya)阅读数：1193








英文原文：[https://spring.io/guides/gs/rest-service/](https://spring.io/guides/gs/rest-service/)

**目录**

[你要构建什么](#%E4%BD%A0%E8%A6%81%E6%9E%84%E5%BB%BA%E4%BB%80%E4%B9%88)

[你需要什么](#%E4%BD%A0%E9%9C%80%E8%A6%81%E4%BB%80%E4%B9%88)

[如何完成本指南](#%E5%A6%82%E4%BD%95%E5%AE%8C%E6%88%90%E6%9C%AC%E6%8C%87%E5%8D%97)

[使用Gradle构建](#%E4%BD%BF%E7%94%A8Gradle%E6%9E%84%E5%BB%BA)

[使用Maven构建](#%E4%BD%BF%E7%94%A8Maven%E6%9E%84%E5%BB%BA)

[IDE构建](#IDE%E6%9E%84%E5%BB%BA)

[创建资源表示类](#%E5%88%9B%E5%BB%BA%E8%B5%84%E6%BA%90%E8%A1%A8%E7%A4%BA%E7%B1%BB)

[创建资源控制器](#%E5%88%9B%E5%BB%BA%E8%B5%84%E6%BA%90%E6%8E%A7%E5%88%B6%E5%99%A8)

[使应用程序可执行](#%E4%BD%BF%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%E5%8F%AF%E6%89%A7%E8%A1%8C)

[构建可执行的JAR](#%E6%9E%84%E5%BB%BA%E5%8F%AF%E6%89%A7%E8%A1%8C%E7%9A%84JAR)

[测试服务](#%E6%B5%8B%E8%AF%95%E6%9C%8D%E5%8A%A1)

[总结](#%E6%80%BB%E7%BB%93)

[更多](#%E5%8F%82%E8%80%83)

本指南将引导您完成使用Spring创建“hello world”[RESTful](https://spring.io/understanding/REST)Web服务的过程。

## 你要构建什么



您将在以下位置构建一个接受HTTP GET请求的服务：
`http://localhost:8080/greeting`
并使用[JSON](https://spring.io/understanding/JSON)表示问候语：
`{"id":1,"content":"Hello, World!"}`
您可以使用查询字符串中的可选name参数自定义问候语：
`http://localhost:8080/greeting?name=User`
name参数值将覆盖默认值“World”并反映在响应中：
`{"id":1,"content":"Hello, User!"}`


## 你需要什么
-      大约15分钟
-      最喜欢的文本编辑器或IDE
- [JDK 1.8](http://www.oracle.com/technetwork/java/javase/downloads/index.html)或更高版本
- [Gradle 4+](http://www.gradle.org/downloads)或[Maven 3.2+](https://maven.apache.org/download.cgi)
-      您还可以将代码直接导入IDE：       [Spring Tools Suite（STS）](https://spring.io/guides/gs/sts)[IntelliJ IDEA](https://spring.io/guides/gs/intellij-idea/)



## 如何完成本指南



与大多数[Spring入门指南](https://spring.io/guides)一样，您可以从头开始并完成每个步骤，或者您可以绕过您已熟悉的基本设置步骤。 无论哪种方式，您最终都会使用工作代码。


要从头开始，请继续使用[Gradle构建](https://spring.io/guides/gs/rest-service/#scratch)。


要跳过基础知识，请执行以下操作：
- [下载](https://github.com/spring-guides/gs-rest-service/archive/master.zip)并解压缩本指南的源存储库，或使用Git克隆它：
`git clone https://github.com/spring-guides/gs-rest-service.git`

- cd到gs-rest-service / initial
- 跳转到[创建资源表示类](https://spring.io/guides/gs/rest-service/#initial)。

完成后，您可以根据代码检查结果
`gs-rest-service/complete`
### 使用Gradle构建

首先，设置一个基本的构建脚本。 在使用Spring构建应用程序时，您可以使用任何您喜欢的构建系统，但此处包含了使用Gradle和Maven所需的代码。 如果您不熟悉这两者，请参阅使用Gradle构建Java项目或使用Maven构建Java项目。
**创建目录结构**

└── src
    └── main
        └── java
            └── hello
在您选择的项目目录中，创建以下子目录结构; 例如，在* nix系统上使用mkdir -p src/main/java/hello：

**创建Gradle构建文件**


下面是[最初的Gradle构建文件](https://github.com/spring-guides/gs-rest-service/blob/master/initial/build.gradle)。

*build.gradle*

```
buildscript {
    repositories {
        mavenCentral()
    }
    dependencies {
        classpath("org.springframework.boot:spring-boot-gradle-plugin:2.0.5.RELEASE")
    }
}

apply plugin: 'java'
apply plugin: 'eclipse'
apply plugin: 'idea'
apply plugin: 'org.springframework.boot'
apply plugin: 'io.spring.dependency-management'

bootJar {
    baseName = 'gs-rest-service'
    version =  '0.1.0'
}

repositories {
    mavenCentral()
}

sourceCompatibility = 1.8
targetCompatibility = 1.8

dependencies {
    compile("org.springframework.boot:spring-boot-starter-web")
    testCompile('org.springframework.boot:spring-boot-starter-test')
}
```

[Spring Boot gradle插件](https://docs.spring.io/spring-boot/docs/current/gradle-plugin/reference/html)提供了许多方便的功能：
-      它收集类路径上的所有jar并构建一个可运行的“über-jar”，这使得执行和传输服务更加方便。
-      它搜索public static void main（）方法以标记为可运行的类。
-      它提供了一个内置的依赖项解析器，它设置版本号以匹配[Spring Boot依赖项](https://github.com/spring-projects/spring-boot/blob/master/spring-boot-project/spring-boot-dependencies/pom.xml)。 您可以覆盖任何您希望的版本，但它将默认为Boot的所选版本集。 



### 使用Maven构建

首先，设置一个基本的构建脚本。 在使用Spring构建应用程序时，您可以使用任何您喜欢的构建系统，但此处包含了使用Maven所需的代码。 如果您不熟悉Maven，请参阅使用Maven构建Java项目。
**创建目录结构**

└── src
    └── main
        └── java
            └── hello
在您选择的项目目录中，创建以下子目录结构; 例如，在* nix系统上使用mkdir -p src/main/java/hello：

*pom.xml*

```
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>org.springframework</groupId>
    <artifactId>gs-rest-service</artifactId>
    <version>0.1.0</version>

    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.0.5.RELEASE</version>
    </parent>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
        <dependency>
            <groupId>com.jayway.jsonpath</groupId>
            <artifactId>json-path</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>

    <properties>
        <java.version>1.8</java.version>
    </properties>


    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>

</project>
```

[Spring Boot Maven插件](https://docs.spring.io/spring-boot/docs/current/maven-plugin)提供了许多方便的功能：

     它收集类路径上的所有jar并构建一个可运行的“über-jar”，这使得执行和传输服务更加方便。

     它搜索public static void main（）方法以标记为可运行的类。

     它提供了一个内置的依赖项解析器，它设置版本号以匹配[Spring Boot依赖项](https://github.com/spring-projects/spring-boot/blob/master/spring-boot-project/spring-boot-dependencies/pom.xml)。 您可以覆盖任何您希望的版本，但它将默认为Boot的所选版本集。

### IDE构建

 阅读如何将本指南直接导入[Spring Tool Suite](https://spring.io/guides/gs/sts/)。


 阅读[IntelliJ IDEA](https://spring.io/guides/gs/intellij-idea)中如何使用本指南。

## 创建资源表示类



现在您已经设置了项目和构建系统，您可以创建Web服务。


通过考虑服务交互来开始这个过程。


该服务将处理/ greeting的GET请求，可选地在查询字符串中使用name参数。 GET请求应返回200 OK响应，其中JSON位于表示问候语的正文中。 它应该看起来像这样：

```
{
    "id": 1,
    "content": "Hello, World!"
}
```

id字段是问候语的唯一标识符，content是问候语的文本表示。


要为问候语表示建模，请创建资源表示形式类。 提供一个普通的旧java对象，其中包含id和content数据的字段，构造函数和访问器：

*`src/main/java/hello/Greeting.java`*

```java
package hello;

public class Greeting {

    private final long id;
    private final String content;

    public Greeting(long id, String content) {
        this.id = id;
        this.content = content;
    }

    public long getId() {
        return id;
    }

    public String getContent() {
        return content;
    }
}
```

正如您在下面的步骤中看到的，Spring使用[Jackson JSON](https://github.com/FasterXML/jackson)库自动封送Greeting类型为JSON的实例。

接下来，您将创建将用于提供这些问候语的资源控制器。

## 创建资源控制器

在Spring构建RESTful Web服务的方法中，HTTP请求由控制器处理。 这些组件可以通过@RestController注释轻松识别，下面的GreetingController通过返回Greeting类的新实例来处理/ greeting的GET请求：

*`src/main/java/hello/GreetingController.java`*

```java
package hello;

import java.util.concurrent.atomic.AtomicLong;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class GreetingController {

    private static final String template = "Hello, %s!";
    private final AtomicLong counter = new AtomicLong();

    @RequestMapping("/greeting")
    public Greeting greeting(@RequestParam(value="name", defaultValue="World") String name) {
        return new Greeting(counter.incrementAndGet(),
                            String.format(template, name));
    }
}
```

这个控制器简洁而简单，但这里还有很多东西。 让我们一步一步地分解它。

@RequestMapping注释确保对/ greeting的HTTP请求映射到greeting（）方法。

上面的示例未指定GET与PUT，POST等，因为@RequestMapping默认映射所有HTTP操作。 使用@RequestMapping（method = GET）缩小此映射范围。

@RequestParam将查询字符串参数名称的值绑定到greeting（）方法的name参数中。如果请求中不存在name参数，则使用“World”的defaultValue。


方法体的实现基于计数器的下一个值创建并返回具有id和content属性的新Greeting对象，并使用greeting模板格式化给定名称。


传统MVC控制器和上面的RESTful Web服务控制器之间的关键区别在于创建HTTP响应主体的方式。这个RESTful Web服务控制器只是填充并返回一个Greeting对象，而不是依靠视图技术来执行将问候数据的服务器端呈现为HTML。对象数据将作为JSON直接写入HTTP响应。


此代码使用Spring 4的新@RestController注释，该注释将类标记为控制器，其中每个方法都返回一个域对象而不是视图。这是@Controller和@ResponseBody合作的简写。


Greeting对象必须转换为JSON。由于Spring的HTTP消息转换器支持，您无需手动执行此转换。因为Jackson 2在类路径上，所以会自动选择Spring的MappingJackson2HttpMessageConverter将Greeting实例转换为JSON。



## 使应用程序可执行



虽然可以将此服务打包为传统的[WAR](https://spring.io/understanding/WAR)文件以部署到外部应用程序服务器，但下面演示的更简单的方法创建了一个独立的应用程序。 您将所有内容打包在一个可执行的JAR文件中，由一个好的旧Java main（）方法驱动。 在此过程中，您使用Spring的支持将Tomcat servlet容器嵌入为HTTP运行时，而不是部署到外部实例。

*`src/main/java/hello/Application.java`*

```java
package hello;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}
```

@SpringBootApplication是一个便利注释，添加了以下所有内容：
- @Configuration将类标记为应用程序上下文的bean定义源。
- @EnableAutoConfiguration告诉Spring Boot根据类路径设置，其他bean和各种属性设置开始添加bean。
-      通常你会为Spring MVC应用程序添加@EnableWebMvc，但Spring Boot会在类路径上看到spring-webmvc时自动添加它。 这会将应用程序标记为Web应用程序并激活关键行为，例如设置DispatcherServlet。
- @ComponentScan告诉Spring在hello包中寻找其他组件，配置和服务，允许它找到控制器。

main（）方法使用Spring Boot的SpringApplication.run（）方法来启动应用程序。 您是否注意到没有一行XML？ 也没有web.xml文件。 此Web应用程序是100％纯Java，您无需处理配置任何管道或基础结构。



## 构建可执行的JAR



您可以使用Gradle或Maven从命令行运行该应用程序。 或者，您可以构建一个包含所有必需依赖项，类和资源的可执行JAR文件，并运行该文件。 这使得在整个开发生命周期中，跨不同环境等将服务作为应用程序发布，版本和部署变得容易。


如果您使用的是Gradle，则可以使用./gradlew bootRun运行该应用程序。 或者您可以使用./gradlew build构建JAR文件。 然后你可以运行JAR文件： 
`java -jar build/libs/gs-rest-service-0.1.0.jar`
如果您使用的是Maven，则可以使用./mvn spring-boot：run运行该应用程序。 或者，您可以使用./mvn clean包构建JAR文件。 然后你可以运行JAR文件：
`java -jar target/gs-rest-service-0.1.0.jar`
上面的过程将创建一个可运行的JAR。 您也可以选择构建经典WAR文件。

显示记录输出。 该服务应在几秒钟内启动并运行。

## 测试服务



现在该服务已启动，请访问[http://localhost:8080/greeting](http://localhost:8080/greeting)，其中显示：
`{"id":1,"content":"Hello, World!"}`
使用[http://localhost:8080/greeting?name=User](http://localhost:8080/greeting?name=User)提供名称查询字符串参数。 注意content属性的值如何从“Hello，World！”改变。 “Hello，User！”：
`{"id":2,"content":"Hello, User!"}`
此更改表明GreetingController中的@RequestParam排列正在按预期工作。 name参数的默认值为“World”，但始终可以通过查询字符串显式覆盖。


另请注意id属性如何从1更改为2.这证明您正在跨多个请求对抗同一个GreetingController实例，并且其计数器字段在每次调用时按预期递增。

## 总结



恭喜！ 您刚刚使用Spring开发了RESTful Web服务。

## 更多



以下指南也可能有所帮助：
- 
[Accessing GemFire Data with REST](https://spring.io/guides/gs/accessing-gemfire-data-rest/)

- 
[Accessing MongoDB Data with REST](https://spring.io/guides/gs/accessing-mongodb-data-rest/)

- 
[Accessing data with MySQL](https://spring.io/guides/gs/accessing-data-mysql/)

- 
[Accessing JPA Data with REST](https://spring.io/guides/gs/accessing-data-rest/)

- 
[Accessing Neo4j Data with REST](https://spring.io/guides/gs/accessing-neo4j-data-rest/)

- 
[Consuming a RESTful Web Service](https://spring.io/guides/gs/consuming-rest/)

- 
[Consuming a RESTful Web Service with AngularJS](https://spring.io/guides/gs/consuming-rest-angularjs/)

- 
[Consuming a RESTful Web Service with jQuery](https://spring.io/guides/gs/consuming-rest-jquery/)

- 
[Consuming a RESTful Web Service with rest.js](https://spring.io/guides/gs/consuming-rest-restjs/)

- 
[Securing a Web Application](https://spring.io/guides/gs/securing-web/)

- 
[Building REST services with Spring](https://spring.io/guides/tutorials/bookmarks/)

- 
[React.js and Spring Data REST](https://spring.io/guides/tutorials/react-and-spring-data-rest/)

- 
[Building an Application with Spring Boot](https://spring.io/guides/gs/spring-boot/)

- 
[Creating API Documentation with Restdocs](https://spring.io/guides/gs/testing-restdocs/)

- 
[Enabling Cross Origin Requests for a RESTful Web Service](https://spring.io/guides/gs/rest-service-cors/)

- 
[Building a Hypermedia-Driven RESTful Web Service](https://spring.io/guides/gs/rest-hateoas/)

- 
[Circuit Breaker](https://spring.io/guides/gs/circuit-breaker/)


想要撰写新指南或为现有指南做出贡献？ 查看我们的[贡献指南](https://github.com/spring-guides/getting-started-guides/wiki)。

 所有指南均附有代码的ASLv2许可证，以及[Attribution，NoDerivatives](https://creativecommons.org/licenses/by-nd/3.0/)创作公共许可证。



