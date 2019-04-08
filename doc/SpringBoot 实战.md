# SpringBoot 实战 



# (一) | 如何使用 IDEA 构建 Spring Boot 工程

2019年01月04日 22:41:37

 

一个优秀的废人

 

阅读数：33

更多

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/85805617

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

新年立了个 flag，好好运营这个公众号。具体来说，就是每周要写两篇文章在这个号发表。刚立的 flag 可不能这么快打脸。下面送上本周第一篇。本文我们将介绍嵌入 Intellij IDEA 中的 Spring Initializr 工具，它同Web提供的创建功能一样，可以帮助我们快速的构建出一个基础的Spring Boot工程。

#### 什么是 SpringBoot ？

SpringBoot 官方有一句话可以概括这个问题。那就是「约定大于配置」。这句话什么意思？相信学过 Spring 的人都知道，我们要手动写一大堆的 xml 文件用于配置，集成项目，才能使这个项目具备 web 的功能。而 SpringBoot 做了那些没有它你也会去做的Spring Bean配置。它使用「约定大于配置」的理念让你的项目快速运行起来。使用 Spring Boot 很容易创建一个独立运行（运行jar,内嵌Servlet容器）、准生产级别的基于 Spring 框架的项目，使用 Spring Boot 你可以不用或者只需要很少的Spring配置。

如果说 Spring 是一辆汽车的引擎，那 SpringMVC 就给这辆汽车装上了轮子，而 SpringBoot 的出现就相当于赋予了这辆汽车自动驾驶的功能。

#### 如何使用 IDEA 构建 SpringBoot 工程？

第一步，当然是安装 Intellij IDEA （傻瓜式教程，请自行百度）。

点击菜单栏 File ➤New➤Project ➤ 选择 Spring Initializr 创建界面如下图，可以看到图中 default 指定的 Initializr Service URL 就是 Spring 官方提供的 Spring Initializr 工具地址，一般默认即可，所以这里创建的工程实际上也是基于它的 Web 工具来实现的。

![初始界面](https://upload-images.jianshu.io/upload_images/3282134-9feb015fbaf77d49.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

点击 next 进入下一步，可以看见这里要我们选择的就是关于工程的一些信息：

- Group 顾名思义就是你的公司名，一般是填写com.公司名。
- Artifact groupId 和 artifactId 是maven管理项目包时用作区分的字段，就像是地图上的坐标。这里填写项目名即可。
- Type 就是构建的项目类型，意思就是你希望你的项目使用什么工具构建，可选 maven 和 gradle 一般选 maven。
- Language 顾名思义就是你的项目用啥语言开发，可选 Java、Groovy、Kotlin
- Packaging 就是你希望你的项目打成什么形式的包，可选 Jar、War SpringBoot 项目一般选 Jar
- Java Version 意指项目使用的 java 版本，根据你的需要选择。
- Version 项目的初始版本，默认即可。
- Name 项目名称。
- Description 项目描述，默认即可。
- Package 包名，填完 Group 和 Artifact 后自动生成，默认即可。
  ![下一步](https://upload-images.jianshu.io/upload_images/3282134-2f64dba09fdf023c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

点击 Next 进入下一步，这一步就是选你的项目依赖包，前文所说的「约定大于配置」就体现在这里。进入选择S pring Boot 版本和依赖管理的窗口。在这里值的我们关注的是，它不仅包含了 Spring Boot Starter POMs 中的各个依赖，还包含了 Spring Cloud 的各种依赖。

比如，你需要集成前端模板功能，你就到 Template Engines 选项卡上，勾选你想要访问的前端模板引擎 ,项目需要访问数据库，就到 SQL 选项卡，旋转你项目里使用的数据库类型。选择完成并加以简单的配置，项目就具备了集成前端模板能力与数据库访问能力。

这里注意一下，无论你选择哪些依赖包，其中 web 选项卡下的 Web 是必选的。这个包是整个项目的基础。这个包里面集成了 Spring 、WebMvc 、tomcat 以及其他各种基本能力。

![下一步](https://upload-images.jianshu.io/upload_images/3282134-6c9a8cd19fb181f3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

点击 Next 进入下一步，这一步没啥好说的。就是让你确认自己的项目名以及项目路径。确认无误，点 Finish 完成创建即可。

![项目位置](https://upload-images.jianshu.io/upload_images/3282134-aa5f2d388ef741e6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

Intellij IDEA 中的 Spring Initializr 是基于官方 Web 实现，但是通过工具来进行调用并直接将结果构建到我们的本地文件系统中，让整个构建流程变得更加顺畅。

#### 后语

我为什么要写这种这么简单的教程？
是这样的，我始终认为比我聪明的人有很多，但比我笨的人也不少。在中国有很多你认为众所周知的事，其实有一车人根本不知道，这篇文章哪怕只帮助到一个人，足矣。

之后我打算出一个 SpringBoot 系列的教程，敬请关注与指正，本人也是一个小菜鸟在打怪升级中，如本文有不正确的地方，烦请指正。一起学习一起进步。

以上就是使用 IDEA 创建 SpringBoot 的过程，希望对你们有帮助。最后，对 Python 、Java 感兴趣请长按二维码关注一波，我会努力带给你们价值，如果觉得本文对你哪怕有一丁点帮助，请帮忙点好看，让更多人知道。







#  (二) | 第一个 SpringBoot 工程详解

2019年01月05日 17:05:58

 

一个优秀的废人

 

阅读数：46

更多

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/85860138

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

哎呦喂，按照以往的惯例今天周六我的安排应该是待在家学学猫叫啥的。但是今年这种日子就可能一去不复返了，没法办法啊。前几天年轻，立下了一周至少更两篇文章的 flag 。废话少说，今天接着前文给你们带来了第一个 SpringBoot 工程的详解。

#### 第一个 SpringBoot 工程

前文已经说过了 SpringBoot 工程的创建，这里不再赘述，还不会的朋友，请看下面这篇文章。

[如何使用 IDEA 构建 Spring Boot 工程](https://www.jianshu.com/p/83c0541165cf)

学过编程的都知道，学习一门新语言的第一个项目肯定是 Hello World 。那这里也不例外，我们先创建一个非常简单的 Hello World 工程。给大家讲解 SpringBoot 的项目目录。创建信息如下：

![创建信息](https://upload-images.jianshu.io/upload_images/3282134-95ae6b9752616971.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

由于本文重点旨在讲解 SpringBoot 的项目目录。所以选择的依赖包非常简单，就选择 Web 足矣。

![选择依赖](https://upload-images.jianshu.io/upload_images/3282134-8113198a3592d46a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### SpringBoot 项目目录详解

创建成功之后的 SpringBoot 项目目录如下，以下对各主要目录的作用进行讲解:

- src 是整个工程的根目录，基本上做 web 开发你的代码大部分都放在这里。其中 main 目录下放置的是你的 Java 代码；resource 目录，顾名思义就是放置配置文件、静态资源( static )以及前端模板( template )。
- test 目录就是放置你的单元测试代码。
- target 就是项目编译生成的目录，里面包含代码编译后的 class 文件以及一些静态资源和配置文件。
- External Libraries 这里放置了，pom.xml 导入的依赖包。
- 其他没提到的目录都是不重要的。

![项目目录](https://upload-images.jianshu.io/upload_images/3282134-c8d31cc05551459f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

由上图项目目录，可以看到有几个文件，这些文件有些是我新建的，有些是项目生成的。下面我会讲解：

- pom.xml 这个文件是整个项目最重要的文件，它包含了整个项目的依赖包。Maven 会根据这个文件导入相关的我们开发需要的依赖包。代码如下：

可以看到 pom.xml 中一共有 4 个依赖，其中只有 Junit 是我手动加入的，用于单元测试。

其他的如 Spring Boot 启动父依赖、Spring Boot web依赖 、Spring Boot web test依赖都是创建项目时，勾选 web 选项卡而生成的。这几个依赖包的额作用就是 内嵌 Tomcat 容器，集成各 Spring 组件。比如 如果没有依赖 web 包 ，Spring 的两大核心功能 IOC 和 AOP 将不会生效。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.nasus</groupId>
    <artifactId>helloworld</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>helloworld</name>
    <description>Demo project for Spring Boot</description>

    <properties>
        <java.version>1.8</java.version>
    </properties>

    <!-- Spring Boot 启动父依赖 -->
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.1.1.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>

    <dependencies>
        <!-- Spring Boot web依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>

        <!-- Junit -->
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.12</version>
        </dependency>
    </dependencies>

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

- HelloworldApplication.java 最为重要，它由项目生成，是整个工程的应用启动类 main 函数。代码由项目生成，代码如下：SpringApplication 引导应用，并将 HelloworldApplication 本身作为参数传递给 run 方法。具体 run 方法会启动嵌入式的 Tomcat 并初始化 Spring环境及其各 Spring 组件。

需要注意的是，这个类必须放在其他类的上册目录，拿上述目录举个栗子， 若其他HelloWorldController.java 类位于 com.nasus.controller 下。则 HelloworldApplication.java 类必须放置在 com.nasus 下或者 com 下（层级必须大于其他 java 类）。否则启动项目访问会报 Whitelabel Error Page 错误，原因是项目扫描不到 @RestController、@RequestMapping 等注解配置的方法和类。

![Whitelabel Error Page](https://upload-images.jianshu.io/upload_images/3282134-8e1c2d5d9682b458.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

```java
package com.nasus.helloworld;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class HelloworldApplication {
    public static void main(String[] args) {
        SpringApplication.run(HelloworldApplication.class, args);
    }
}
```

- HelloWorldController 是我手动编写的，代码如下：@RestController 和 @RequestMapping 注解是来自 SpringMVC 的注解，它们不是 SpringBoot 的特定部分。

其中 @RestController 注解的作用是：提供实现了 REST API，可以服务 JSON、XML 或者其他。这里是以 String 的形式渲染出结果。

其中 @RestController 注解的作用是：提供路由信息，"/“路径的HTTP Request都会被映射到sayHello方法进行处理。

具体参考，Spring 官方的文档《[Spring Framework Document](http://docs.spring.io/spring/docs/current/spring-framework-reference/htmlsingle/)》

```java
package com.nasus.controller;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * Project Name:helloworld <br/>
 * Package Name:com.nasus.controller <br/>
 * Date:2019/1/5 13:59 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 * Copyright Notice =========================================================
 * This file contains proprietary information of Eastcom Technologies Co. Ltd.
 * Copying or reproduction without prior written approval is prohibited.
 * Copyright (c) 2019 =======================================================
 */
@RestController
public class HelloWorldController {

    @RequestMapping("/hello")
    public String sayHello() {
        return "Hello,World!";
    }

}
```

写完 Controller 层的代码，我们就可以启动此项目。点击 IDEA 项目启动按钮，效果如下：

![运行结果](https://upload-images.jianshu.io/upload_images/3282134-35e2962c473ef6de.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

- 好的程序必须配备完善的单元测试。HelloWorldControllerTest.java 文件是由我编写的主要作用就是测试 HelloWorldController.java 中的方法。这里用的是 Junit 依赖包进行单元测试，代码如下：这里的逻辑就是测试 HelloWorldController.java 的 sayHello 方法输出的字符是否是 Hello,World!

```java
package com.nasus;

import static org.junit.Assert.assertEquals;
import com.nasus.controller.HelloWorldController;
import org.junit.Test;

/**
 * Project Name:helloworld <br/>
 * Package Name:com.nasus <br/>
 * Date:2019/1/5 14:01 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 * Copyright Notice =========================================================
 * This file contains proprietary information of Eastcom Technologies Co. Ltd.
 * Copying or reproduction without prior written approval is prohibited.
 * Copyright (c) 2019 =======================================================
 */
public class HelloWorldControllerTest {

    @Test
    public void testSayHello() {
        assertEquals("Hello,World!",new HelloWorldController().sayHello());
    }

}
```

编写完成之后，可以通过以下按钮启动单元测试类。

![运行单元测试](https://upload-images.jianshu.io/upload_images/3282134-db855e2b83cd9987.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

测试结果如下：可以看到红圈框住的地方，出现这个绿色标志证明单元测试没问题。sayhello 方法的结果是对的。

![单元测试结果](https://upload-images.jianshu.io/upload_images/3282134-cbc12ad0cbd7c68a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### 后语

我为什么要写这种这么简单的教程？
是这样的，我始终认为比我聪明的人有很多，但比我笨的人也不少。在中国有很多你认为众所周知的事，其实有一车人根本不知道，这篇文章哪怕只帮助到一个人，足矣。

之后我打算出一个 SpringBoot 系列的教程，敬请关注与指正，本人也是一个小菜鸟在打怪升级中，如本文有不正确的地方，烦请指正。一起学习一起进步。

以上就是我对 SpringBoot 工程的理解，希望对你们有帮助。最后，对 Python 、Java 感兴趣请长按二维码关注一波，我会努力带给你们价值，如果觉得本文对你哪怕有一丁点帮助，请帮忙点好看，让更多人知道。









# (三) | 配置文件详解

2019年01月28日 23:06:58

 

一个优秀的废人

 

阅读数：17

 

标签： [SpringBoot](http://so.csdn.net/so/search/s.do?q=SpringBoot&t=blog)[配置文件](http://so.csdn.net/so/search/s.do?q=%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6&t=blog)[properties](http://so.csdn.net/so/search/s.do?q=properties&t=blog)[yaml](http://so.csdn.net/so/search/s.do?q=yaml&t=blog)[JavaWeb](http://so.csdn.net/so/search/s.do?q=JavaWeb&t=blog)更多

个人分类： [SpringBoot](https://blog.csdn.net/turodog/article/category/8585519)

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/86684006

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

如题，今天解析下 SpringBoot 的配置文件。

#### 自定义属性加载

首先构建 SpringBoot 项目，不会的看这篇旧文 [使用 IDEA 构建 Spring Boot 工程](https://www.jianshu.com/p/83c0541165cf)。

首先在项目根目录 src >> resource >> application.properties 文件下加入以下自定义属性：

```ini
# 防止读取乱码
spring.http.encoding.charset=UTF-8
# 项目启动端口
server.port=9999
# 自定义配置
com.nasus.author.name=一个优秀的废人
com.nasus.article.title=SpringBoot配置文件详解
```

使用 @value 注解读取配置文件属性：

```java
package com.nasus.bean;

import lombok.Data;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

/**
 * Project Name:springboot_properties_demo <br/>
 * Package Name:com.nasus.properties <br/>
 * Date:2019/1/28 20:59 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 */

@Data
@Component
public class PropertiesBean {

    @Value("${com.nasus.author.name}")
    private String name;

    @Value("${com.nasus.article.title}")
    private String title;

    @Value("${com.nasus.doing}")
    private String desc;

}
```

之后新建 controller 测试自定义属性加载，代码如下：

```java
package com.nasus.controller;

import com.nasus.bean.PropertiesBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * Project Name:springboot_properties_demo <br/>
 * Package Name:com.nasus.controller <br/>
 * Date:2019/1/28 21:41 <br/>
 * <b>Description:</b> TODO:  测试自定义属性加载<br/>
 *
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 */
@RestController
@RequestMapping("/test")
public class TestController {

    @Autowired
    private PropertiesBean propertiesBean;

    @GetMapping("/getInfo")
    public PropertiesBean getInfo(){
        return propertiesBean;
    }
}
```

访问 <http://localhost:8080/test/getInfo> 查看加载结果：

![测试结果](https://upload-images.jianshu.io/upload_images/3282134-64619c5da7b35488.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

可以看到，加入 @value 注解之后，配置文件的属性都被读取出来了。以前，或许我们还需要专门写一个读取配置文件的工具类才能把属性读取出来，现在有了 Spring ，我们可以直接使用 @value 就能读取了，简直不能太方便。

#### 参数间的引用

配置文件代码如下：

```xml
# 防止读取乱码
spring.http.encoding.charset=UTF-8
# 项目启动端口
server.port=9999
# 自定义配置
com.nasus.author.name=一个优秀的废人
com.nasus.article.title=SpringBoot配置文件详解

com.nasus.doing=${com.nasus.author.name}写文章《${com.nasus.article.title}》
```

可以看到最后一个参数配置使用了前两个的参数配置，测试结果如下：
![测试结果](https://upload-images.jianshu.io/upload_images/3282134-3969892c8ace274d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### 使用随机数

有时项目需求，可能我们需要配置一些随机数，比如说为了安全而随机配置的服务器端口，以及登录密钥。这时我们就可以用 SpringBoot 的 random 属性来配置随机数，比如：

```xml
# 随机字符串
com.nasus.article.value=${random.value}
# 随机int
com.nasus.article.number=${random.int}
# 随机long
com.nasus.article.bignumber=${random.long}
# 10以内的随机数
com.nasus.article.test1=${random.int(10)}
# 10-20的随机数
com.nasus.article.test2=${random.int[10,20]}
```

#### 使用多配置文件

很多时候我们开发项目都需要很多套环境，比如有测试环境，开发环境以及生产环境。不同的环境就需要使用不同的配置文件，为此我们可以根据这 3 个环境分别新建 以下 3 个配置文件。

application-dev.properties：开发环境
application-test.properties：测试环境
application-prod.properties：生产环境

项目中默认的配置文件是 application.properties 。这时我们可以根据自己的环境去使用相应的配置文件，比如说，项目各个环境的端口必须不一样。那我们可以这样配置：
application-dev.properties：开发环境

```xml
server.port=6666
```

application-test.properties：测试环境

```xml
server.port=7777
```

application-prod.properties：生产环境

```xml
server.port=8888
```

假如，现在我打包上线，那就必须用生产环境的配置文件了，这时我们可以在 默认的配置文件 application.properties 中加入以下配置即可

```xml
spring.profiles.active=prod
```

#### 配置数据库

SpringBoot 的配置文件有两种格式，一种是 .properties 格式（以上栗子都是用的这种）还有一种用的是 .yaml 格式。以下是用 yaml 方式配置。这两种格式并无好坏之分，纯看个人使用习惯。我就比较喜欢 yaml 格式，因为看起来比较简洁。

```xml
spring:
  datasource:
    driver-class-name: com.mysql.jdbc.Driver
    url: jdbc:mysql://127.0.0.1:3306/test?useUnicode=true&characterEncoding=utf8&serverTimezone=UTC&useSSL=true
    username: 你的数据库名称
    password: 你的数据库密码

  jpa:
    hibernate:
      ddl-auto: update   #ddl-auto:设为update 表示每次都重新建表
    show-sql: true
```

#### 注意事项

1. 使用 yaml 格式需要注意一点就是 **键值对冒号后面，必须空一格**。
2. application.properties 配置中文值的时候，读取出来的属性值会出现乱码问题。但是 application.yml 不会出现乱码问题。原因是，Spring Boot 是以 iso-8859 的编码方式读取 application.properties 配置文件。
   解决第二点，只需加入 **spring.http.encoding.charset=UTF-8** 配置即可。

#### 后语

以上就是我对 SpringBoot 配置文件的理解与使用，当然以上只是介绍了一下 SpringBoot 配置文件的几个用法，它的用法还有非常多，想要深入使用还是需要各位多多深入实践。最后，对 Python 、Java 感兴趣请长按二维码关注一波，我会努力带给你们价值，如果觉得本文对你哪怕有一丁点帮助，请帮忙点好看，让更多人知道。

另外，关注之后在发送 **1024** 可领取免费学习资料。资料内容详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)







#  (四) | 使用 LomBok

2019年01月24日 09:45:14

 

一个优秀的废人

 

阅读数：22

 

标签： [SpringBoot](http://so.csdn.net/so/search/s.do?q=SpringBoot&t=blog)[java](http://so.csdn.net/so/search/s.do?q=java&t=blog)[bean](http://so.csdn.net/so/search/s.do?q=bean&t=blog)[LomBok](http://so.csdn.net/so/search/s.do?q=LomBok&t=blog)[IDEA](http://so.csdn.net/so/search/s.do?q=IDEA&t=blog) 更多

个人分类： [SpringBoot](https://blog.csdn.net/turodog/article/category/8585519)

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/86620779

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

上周去了开年会，去的地方是温泉度假村。老实说，我是无感的，90% 是因为没中奖（老板太抠，两百人只抽三个奖），10 % 是因为从小泡温泉长大没啥感觉。

#### LomBok 是什么？

LomBok 是一个插件，它能通过注解帮我们消除那些必须要写但是重复的代码，比如setter，getter，构造函数之类的方法。

试想一个场景，在项目开发中，我们往往需要定义大量的数据库实体或者某些工具 Bean ，每一个 Bean 都需要我们编写 getter 、setter 方法、构造方法以及 toString 等方法。这是一个非常繁琐的过程。程序员准则，做三遍以上的重复事情就必须想办法让它自动化了。所以今天给你们介绍一个牛逼的插件「LomBok」。

#### LomBok 常用注解简介

```java
@Data：注解在类上，将类提供的所有属性都添加get、set方法，并添加、equals、canEquals、hashCode、toString方法
@Setter：注解在类上，为所有属性添加set方法、注解在属性上为该属性提供set方法
@Getter：注解在类上，为所有的属性添加get方法、注解在属性上为该属性提供get方法
@NotNull：在参数中使用时，如果调用时传了null值，就会抛出空指针异常
@Synchronized 用于方法，可以锁定指定的对象，如果不指定，则默认创建一个对象锁定
@Log作用于类，创建一个log属性
@Builder：使用builder模式创建对象
@NoArgsConstructor：创建一个无参构造函数
@AllArgsConstructor：创建一个全参构造函数
@ToStirng：创建一个toString方法
@Accessors(chain = true)使用链式设置属性，set方法返回的是this对象。
@RequiredArgsConstructor：创建对象
@UtilityClass:工具类
@ExtensionMethod:设置父类
@FieldDefaults：设置属性的使用范围，如private、public等，也可以设置属性是否被final修饰。
@Cleanup: 关闭流、连接点。
@EqualsAndHashCode：重写equals和hashcode方法。
@toString：创建toString方法。
```

#### 如何安装？

1、直接从 <http://plugins.jetbrains.com/> 下载，然后放到IDEA 安装文件下面的 plugins，然后重启 IDEA。

2、在 IDEA 的 settings（windows）或者Preferences（mac），下找到 plugins 菜单，点击 Browse repositories，如下图

![Browse repositories](https://upload-images.jianshu.io/upload_images/3282134-e72cc6cd084be840.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

第二步搜索 LomBok 点击 Install （我这里已经安装了，但是有更新。所以显示的是 Update 按钮）重启 IDEA 。

![搜索 LomBok](https://upload-images.jianshu.io/upload_images/3282134-9c2f6ba3a55ddbb5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### 代码演示

新建 SpringBoot 项目，不会构建的看这篇文章 [使用 IDEA 构建 Spring Boot 工程](https://www.jianshu.com/p/83c0541165cf) ，构建时勾选 web 依赖和 Lombok 依赖，完整 pom 如下：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.1.2.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <groupId>com.nasus</groupId>
    <artifactId>lombok</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>lombok</name>
    <description>lombok_demo project for Spring Boot</description>

    <properties>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <optional>true</optional>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>

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

新建一个 Student 类，这是使用 lombok 的类，代码如下：

```java
package com.nasus.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * Project Name:springboot_lomnok_demo <br/>
 * Package Name:com.nasus.entity <br/>
 * Date:2019/1/23 0023 14:32 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="mailto:chenzy@eastcom-sw.com">chenzy</a><br/>
 * Copyright Notice =========================================================
 * This file contains proprietary information of Eastcom Technologies Co. Ltd.
 * Copying or reproduction without prior written approval is prohibited.
 * Copyright (c) 2019 =======================================================
 */
@Data //自动生成set/get方法，toString方法，equals方法，hashCode方法，不带参数的构造方法 
@AllArgsConstructor //自动生成构造方法 
@NoArgsConstructor //自动生成构造方法 
public class Student {

    private String id;
    private String name;
    private int age;

}
```

新建一个 StudentNoLombok 类，这是没有使用 lombok 的类（此类在项目中无用处，只为与使用 Lombok 的类对比，突出使用 LomBok 类代码的简洁），代码如下：

```java
package com.nasus.entity;

import java.util.Objects;

/**
 * Project Name:springboot_lomnok_demo <br/>
 * Package Name:com.nasus.entity <br/>
 * Date:2019/1/23 0023 14:34 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="mailto:chenzy@eastcom-sw.com">chenzy</a><br/>
 * Copyright Notice =========================================================
 * This file contains proprietary information of Eastcom Technologies Co. Ltd.
 * Copying or reproduction without prior written approval is prohibited.
 * Copyright (c) 2019 =======================================================
 */
public class StudentNoLombok {

    private String id;
    private String name;
    private int age;

    public StudentNoLombok() {
    }

    public StudentNoLombok(String id, String name, int age) {
        this.id = id;
        this.name = name;
        this.age = age;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (!(o instanceof StudentNoLombok)) {
            return false;
        }
        StudentNoLombok that = (StudentNoLombok) o;
        return age == that.age &&
                Objects.equals(id, that.id) &&
                Objects.equals(name, that.name);
    }

    @Override
    public int hashCode() {

        return Objects.hash(id, name, age);
    }

    @Override
    public String toString() {
        return "StudentNoLombok{" +
                "id='" + id + '\'' +
                ", name='" + name + '\'' +
                ", age=" + age +
                '}';
    }
}
```

从上面两个类对比可以看出，使用 LomBok 插件的类比不使用简洁美观得多。

新建 StudentController 类，代码如下：

```java
package com.nasus.controller;

import com.nasus.entity.Student;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * Project Name:springboot_lomnok_demo <br/>
 * Package Name:com.nasus.controller <br/>
 * Date:2019/1/23 0023 14:37 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="mailto:chenzy@eastcom-sw.com">chenzy</a><br/>
 * Copyright Notice =========================================================
 * This file contains proprietary information of Eastcom Technologies Co. Ltd.
 * Copying or reproduction without prior written approval is prohibited.
 * Copyright (c) 2019 =======================================================
 */
@RestController
@RequestMapping("/student")
public class StudentController {

    @GetMapping("/testLombok")
    public Student getStudent(){
        Student student = new Student();
        student.setId("6666666666");
        student.setAge(24);
        student.setName("陈志远");
        System.out.println(student.toString());
        return student;
    }

}
```

启动项目，访问地址 <http://localhost:8080/student/testLombok> 浏览器输出如下：

![浏览器输出](https://upload-images.jianshu.io/upload_images/3282134-11dbac59e7133ee4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

控制台输出如下：

![控制台输出](https://upload-images.jianshu.io/upload_images/3282134-9ba76adbc75a8496.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

由上面两个输出结果可以看出，LomBok 插件起作用了。即使 Student 没有重写 toString 方法，依然可以调用，而且结果和重写了 toString 方法一样。

当项目很庞大的时候，往往基础的 Bean 也非常多。建议看到这篇文章的你可以在项目中把 LomBok 用起来。

最后奉上 [项目完整代码](https://github.com/turoDog/Demo/tree/master/springboot_lomnok_demo)

#### 后语

以上就是我对 LomBok 插件的理解与使用，希望对你们有帮助。最后，对 Python 、Java 感兴趣请长按二维码关注一波，我会努力带给你们价值，如果觉得本文对你哪怕有一丁点帮助，请帮忙点好看，让更多人知道。







# (五) | 集成 Swagger2 构建强大的 RESTful API 文档

2019年01月24日 23:59:11

 

一个优秀的废人

 

阅读数：20

 

标签： [SpringBoot](http://so.csdn.net/so/search/s.do?q=SpringBoot&t=blog)[Java](http://so.csdn.net/so/search/s.do?q=Java&t=blog)[Swagger2](http://so.csdn.net/so/search/s.do?q=Swagger2&t=blog)[Api 文档](http://so.csdn.net/so/search/s.do?q=Api%20%E6%96%87%E6%A1%A3&t=blog)[接口文档](http://so.csdn.net/so/search/s.do?q=%E6%8E%A5%E5%8F%A3%E6%96%87%E6%A1%A3&t=blog)更多

个人分类： [SpringBoot](https://blog.csdn.net/turodog/article/category/8585519)

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/86635140

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

快过年了，不知道你们啥时候放年假，忙不忙。反正我是挺闲的，所以有时间写 blog。今天给你们带来 SpringBoot 集成 Swagger2 的教程。

#### 什么是 Swagger2

Swagger 是一个规范和完整的框架，用于生成、描述、调用和可视化 RESTful 风格的 Web 服务。

#### 为什么使用 Swagger2 ？

相信刚开始不熟悉 web 开发的时候，大家都有手写 Api 文档的时候。而手写 Api 文档主要有以下几个痛点：

1. 文档需要更新的时候，需要再次发送一份给前端，也就是文档更新交流不及时。
2. 接口返回结果不明确。
3. 不能直接在线测试接口，通常需要使用工具，比如 postman。
4. 接口文档太多，不好管理。

这些痛点在前后端分离的大型项目上显得尤为烦躁。而 Swagger2 的出现恰好能个解决这些痛点。因为 Swagger2 有以下功能：

1. 文档自动更新，只要生成 Api 的网址没变，基本不需要跟前端沟通。
2. 接口返回结果非常明确，包括数据类型，状态码，错误信息等。
3. 可以直接在线测试文档，而且还有实例提供给你。
4. 只需要一次配置便可使用，之后只要会有一份接口文档，非常易于管理。

#### 集成演示

首先新建一个 SpringBoot 项目，还不会的参考我这篇旧文—— [如何使用 IDEA 构建 Spring Boot 工程](https://www.jianshu.com/p/83c0541165cf)

构建时，在选择依赖那一步勾选 Web、LomBok、JPA 和 Mysql 依赖。其中 Mysql 可以不勾，因为我这里用于操作实际的数据库，所以我勾选了。

生成 SpringBoot 后的 Pom 文件依赖如下：这里使用的是 2.4.0 的 Swagger2 版本。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.1.2.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <groupId>com.nasus</groupId>
    <artifactId>swagger2</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>swagger2</name>
    <description>Demo project for Swagger2</description>

    <properties>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-jpa</artifactId>
        </dependency>

        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>

        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>

        <dependency>
            <groupId>io.springfox</groupId>
            <artifactId>springfox-swagger2</artifactId>
            <version>2.4.0</version>
        </dependency>

        <dependency>
            <groupId>io.springfox</groupId>
            <artifactId>springfox-swagger-ui</artifactId>
            <version>2.4.0</version>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>

</project>
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566
```

第二步，在 SpringBoot 启动类（Application）的同级目录新建一个 Swagger 配置类，注意 Swagger2 配置类必须与项目入口类 Application 位于同一级目录，否则生成 Api 文档失败，代码如下：

```java
package com.nasus;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import springfox.documentation.builders.ApiInfoBuilder;
import springfox.documentation.builders.PathSelectors;
import springfox.documentation.builders.RequestHandlerSelectors;
import springfox.documentation.service.ApiInfo;
import springfox.documentation.service.Contact;
import springfox.documentation.spi.DocumentationType;
import springfox.documentation.spring.web.plugins.Docket;
import springfox.documentation.swagger2.annotations.EnableSwagger2;

/**
 * Project Name:swagger2-demo <br/>
 * Package Name:com.nasus <br/>
 * Date:2019/1/22 22:52 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 * Copyright Notice =========================================================
 * This file contains proprietary information of Eastcom Technologies Co. Ltd.
 * Copying or reproduction without prior written approval is prohibited.
 * Copyright (c) 2019 =======================================================
 */
@Configuration
// 启用 Swagger2
@EnableSwagger2
public class Swagger2 {

    @Bean
    public Docket createRestApi() {
        return new Docket(DocumentationType.SWAGGER_2)
                // 文档信息对象
                .apiInfo(apiInfo())
                .select()
                // 被注解的包路径
                .apis(RequestHandlerSelectors.basePackage("com.nasus.controller"))
                .paths(PathSelectors.any())
                .build();
    }
    private ApiInfo apiInfo() {
        return new ApiInfoBuilder()
                // 标题
                .title("springboot 利用 swagger 构建 API 文档")
                // Api 文档描述
                .description("简单优雅的 restful 风格，https://blog.csdn.net/turodog/")

                .termsOfServiceUrl("https://blog.csdn.net/turodog/")
                // 文档作者信息
                .contact(new Contact("陈志远", "https://github.com/turoDog", "turodog@foxmail.com"))
                // 文档版本
                .version("1.0")
                .build();
    }
}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556
```

第三步，配置被注解的 Controller 类，编写各个接口的请求参数，返回结果，接口描述等等，代码如下：

```java
package com.nasus.controller;

import com.nasus.entity.Student;
import com.nasus.service.StudentService;
import io.swagger.annotations.Api;
import io.swagger.annotations.ApiImplicitParam;
import io.swagger.annotations.ApiImplicitParams;
import io.swagger.annotations.ApiOperation;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import springfox.documentation.annotations.ApiIgnore;

/**
 * Project Name:swagger2-demo <br/>
 * Package Name:com.nasus.controller <br/>
 * Date:2019/1/22 22:07 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 * Copyright Notice =========================================================
 * This file contains proprietary information of Eastcom Technologies Co. Ltd.
 * Copying or reproduction without prior written approval is prohibited.
 * Copyright (c) 2019 =======================================================
 */
@RestController
@RequestMapping("/student")
// @Api：修饰整个类，描述Controller的作用
@Api("StudentController Api 接口文档")
public class StudentController {

    @Autowired
    private StudentService studentService;

    // @ApiOperation：描述一个类的一个方法，或者说一个接口
    @ApiOperation(value="获取所有学生列表", notes="获取所有学生列表")
    @RequestMapping(value={""}, method= RequestMethod.GET)
    public List<Student> getStudent() {
        List<Student> list = studentService.findAll();
        return list;
    }

    @ApiOperation(value="添加学生信息", notes="添加学生信息")
    // @ApiImplicitParam：一个请求参数
    @ApiImplicitParam(name = "student", value = "学生信息详细实体", required = true, dataType = "Student")
    @PostMapping("/save")
    public Student save(@RequestBody Student student){
        return studentService.save(student);
    }

    @ApiOperation(value="获学生信息", notes="根据url的id来获取学生详细信息")
    @ApiImplicitParam(name = "id", value = "ID", required = true, dataType = "Integer",paramType = "path")
    @GetMapping("/{id}")
    public Student findById(@PathVariable("id") Integer id){
        return studentService.findById(id);
    }

    @ApiOperation(value="删除学生", notes="根据url的id来指定删除的学生")
    @ApiImplicitParam(name = "id", value = "学生ID", required = true, dataType = "Integer",paramType = "path")
    @DeleteMapping("/{id}")
    public String deleteById(@PathVariable("id") Integer id){
        studentService.delete(id);
        return "success";
    }

    @ApiOperation(value="更新学生信息", notes="根据url的id来指定更新学生信息")
    // @ApiImplicitParams：多个请求参数
    @ApiImplicitParams({
            @ApiImplicitParam(name = "id", value = "学生ID", required = true, dataType = "Integer",paramType = "path"),
            @ApiImplicitParam(name = "student", value = "学生实体student", required = true, dataType = "Student")
    })
    @PutMapping(value="/{id}")
    public String updateStudent(@PathVariable Integer id, @RequestBody Student student) {
        Student oldStudent = this.findById(id);
        oldStudent.setId(student.getId());
        oldStudent.setName(student.getName());
        oldStudent.setAge(student.getAge());
        studentService.save(oldStudent);
        return "success";
    }

    // 使用该注解忽略这个API
    @ApiIgnore
    @RequestMapping(value = "/hi", method = RequestMethod.GET)
    public String  jsonTest() {
        return " hi you!";
    }
}
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596
```

第四步，启动项目，访问 <http://localhost:8080/swagger-ui.html> 地址，结果如下图：

![运行结果](https://upload-images.jianshu.io/upload_images/3282134-f0c4fba4cfb8bea5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### 项目源代码

[github](https://github.com/turoDog/Demo/tree/master/swagger2-demo)

#### 图解接口

![参数解释](https://upload-images.jianshu.io/upload_images/3282134-3c2ede36fba222a6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![参数解释](https://upload-images.jianshu.io/upload_images/3282134-7a9ce0d9db18ec07.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![测试结果](https://upload-images.jianshu.io/upload_images/3282134-1f77497f8dde56c3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### Swagger2 常用注解简介

```java
@ApiOperation：用在方法上，说明方法的作用
  1.value: 表示接口名称
  2.notes: 表示接口详细描述 
@ApiImplicitParams：用在方法上包含一组参数说明
@ApiImplicitParam：用在@ApiImplicitParams注解中，指定一个请求参数的各个方面
  1.paramType：参数位置
  2.header 对应注解：@RequestHeader
  3.query 对应注解：@RequestParam
  4.path  对应注解: @PathVariable
  5.body 对应注解: @RequestBody
  6.name：参数名
  7.dataType：参数类型
  8.required：参数是否必须传
  9.value：参数的描述
  10.defaultValue：参数的默认值
@ApiResponses：用于表示一组响应
@ApiResponse：用在@ApiResponses中，一般用于表达一个错误的响应信息
  1.code：状态码
  2.message：返回自定义信息
  3.response：抛出异常的类
@ApiIgnore: 表示该接口函数不对swagger2开放展示
@Api：修饰整个类，描述Controller的作用
@ApiParam：单个参数描述
@ApiModel：用对象来接收参数
@ApiProperty：用对象接收参数时，描述对象的一个字段
@ApiIgnore：使用该注解忽略这个API
@ApiError ：发生错误返回的信息
```

#### 注意事项

@ApiImplicitParam 注解下的 paramType 属性,会影响接口的测试,如果设置的属性跟spring 的注解对应不上,会获取不到参数,例如 paramType=path ,函数内却使用@RequestParam 注解,这样,可能会获取不到传递进来的参数,需要按照上面进行对应,将 @RequestParam 注解改为 @PathVariable 才能获取到对应的参数。

#### 后语

以上就是我对 Swagger2 的理解与使用，以上只是教大家入门 Swagger2 ，想要深入使用还是希望自行查阅官方文档。最后，对 Python 、Java 感兴趣请长按二维码关注一波，我会努力带给你们价值，如果觉得本文对你哪怕有一丁点帮助，请帮忙点好看，让更多人知道。









#  (六) | 用 JdbcTemplates 访问 Mysql

2019年02月17日 15:23:55

 

一个优秀的废人

 

阅读数：20

 

标签： [Spring](http://so.csdn.net/so/search/s.do?q=Spring&t=blog)[jdbctemplate](http://so.csdn.net/so/search/s.do?q=jdbctemplate&t=blog)[SpringBoot](http://so.csdn.net/so/search/s.do?q=SpringBoot&t=blog)[MySQL](http://so.csdn.net/so/search/s.do?q=MySQL&t=blog)[数据库访问](http://so.csdn.net/so/search/s.do?q=%E6%95%B0%E6%8D%AE%E5%BA%93%E8%AE%BF%E9%97%AE&t=blog) 更多

个人分类： [SpringBoot](https://blog.csdn.net/turodog/article/category/8585519)

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/87535867

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

如题，今天介绍 springboot 通过jdbc访问关系型 mysql,通过 spring 的 JdbcTemplate 去访问。

#### 准备工作

- SpringBoot 2.x
- jdk 1.8
- maven 3.0
- idea
- mysql

构建 SpringBoot 项目，不会的朋友参考旧文章：[如何使用 IDEA 构建 Spring Boot 工程](https://www.jianshu.com/p/83c0541165cf)

#### 项目目录结构

![项目目录](https://upload-images.jianshu.io/upload_images/3282134-1d689f7c27783610.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### pom 文件引入依赖

```xml
<dependencies>
        <!-- jdbcTemplate 依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-jdbc</artifactId>
        </dependency>
        <!-- 开启web: -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <!-- mysql连接类 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>
        <!-- https://mvnrepository.com/artifact/com.alibaba/druid -->
        <!-- druid 连接池-->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>druid</artifactId>
            <version>1.1.13</version>
        </dependency>

     <dependency>
           <groupId>org.springframework.boot</groupId>
           <artifactId>spring-boot-starter-test</artifactId>
           <scope>test</scope>
    </dependency>
</dependencies>
1234567891011121314151617181920212223242526272829303132
```

#### application.yaml 配置数据库信息

```yaml
spring:
  datasource:
    driver-class-name: com.mysql.jdbc.Driver
    url: jdbc:mysql://127.0.0.1:3306/test?useUnicode=true&characterEncoding=utf8&serverTimezone=UTC&useSSL=true
    username: 数据库用户名
    password: 数据库密码
123456
```

#### 实体类

```java
package com.nasus.domain;

/**
 * Project Name:jdbctemplate_demo <br/>
 * Package Name:com.nasus.domain <br/>
 * Date:2019/2/3 10:55 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 * Copyright Notice =========================================================
 * This file contains proprietary information of Eastcom Technologies Co. Ltd.
 * Copying or reproduction without prior written approval is prohibited.
 * Copyright (c) 2019 =======================================================
 */
public class Student {

    private Integer id;

    private String name;

    private Integer age;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    @Override
    public String toString() {
        return "Student{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", age=" + age +
                '}';
    }
}
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455
```

#### dao 层

```java
package com.nasus.dao;

import com.nasus.domain.Student;
import java.util.List;

/**
 * Project Name:jdbctemplate_demo <br/>
 * Package Name:com.nasus.dao <br/>
 * Date:2019/2/3 10:59 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 */
public interface IStudentDao {

    int add(Student student);

    int update(Student student);

    int delete(int id);

    Student findStudentById(int id);

    List<Student> findStudentList();

}
12345678910111213141516171819202122232425
```

具体实现类：

```java
package com.nasus.dao.impl;

import com.nasus.dao.IStudentDao;
import com.nasus.domain.Student;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

/**
 * Project Name:jdbctemplate_demo <br/>
 * Package Name:com.nasus.dao.impl <br/>
 * Date:2019/2/3 11:00 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 */
@Repository
public class IStudentDaoImpl implements IStudentDao{

    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Override
    public int add(Student student) {
        return jdbcTemplate.update("insert into student(name, age) values(?, ?)",
                student.getName(),student.getAge());
    }

    @Override
    public int update(Student student) {
        return jdbcTemplate.update("UPDATE  student SET NAME=? ,age=? WHERE id=?",
                student.getName(),student.getAge(),student.getId());
    }

    @Override
    public int delete(int id) {
        return jdbcTemplate.update("DELETE from TABLE student where id=?",id);
    }

    @Override
    public Student findStudentById(int id) {
        // BeanPropertyRowMapper 使获取的 List 结果列表的数据库字段和实体类自动对应
        List<Student> list = jdbcTemplate.query("select * from student where id = ?", new Object[]{id}, new BeanPropertyRowMapper(Student.class));
        if(list!=null && list.size()>0){
            Student student = list.get(0);
            return student;
        }else{
            return null;
        }
    }

    @Override
    public List<Student> findStudentList() {
        // 使用Spring的JdbcTemplate查询数据库，获取List结果列表，数据库表字段和实体类自动对应，可以使用BeanPropertyRowMapper
        List<Student> list = jdbcTemplate.query("select * from student", new Object[]{}, new BeanPropertyRowMapper(Student.class));
        if(list!=null && list.size()>0){
            return list;
        }else{
            return null;
        }
    }

}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465
```

#### service 层

```java
package com.nasus.service;

import com.nasus.domain.Student;
import java.util.List;

/**
 * Project Name:jdbctemplate_demo <br/>
 * Package Name:com.nasus.service <br/>
 * Date:2019/2/3 11:17 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 */
public interface IStudentService {

    int add(Student student);

    int update(Student student);

    int delete(int id);

    Student findStudentById(int id);

    List<Student> findStudentList();

}
1234567891011121314151617181920212223242526
```

实现类：

```java
package com.nasus.service.impl;

import com.nasus.dao.IStudentDao;
import com.nasus.domain.Student;
import com.nasus.service.IStudentService;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

/**
 * Project Name:jdbctemplate_demo <br/>
 * Package Name:com.nasus.service.impl <br/>
 * Date:2019/2/3 11:18 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 * Copyright Notice =========================================================
 * This file contains proprietary information of Eastcom Technologies Co. Ltd.
 * Copying or reproduction without prior written approval is prohibited.
 * Copyright (c) 2019 =======================================================
 */
@Repository
public class IStudentServiceImpl implements IStudentService {

    @Autowired
    private IStudentDao iStudentDao;

    @Override
    public int add(Student student) {
        return iStudentDao.add(student);
    }

    @Override
    public int update(Student student) {
        return iStudentDao.update(student);
    }

    @Override
    public int delete(int id) {
        return iStudentDao.delete(id);
    }

    @Override
    public Student findStudentById(int id) {
        return iStudentDao.findStudentById(id);
    }

    @Override
    public List<Student> findStudentList() {
        return iStudentDao.findStudentList();
    }

}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253
```

#### controller 构建 restful api

```java
package com.nasus.controller;

import com.nasus.domain.Student;
import com.nasus.service.IStudentService;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * Project Name:jdbctemplate_demo <br/>
 * Package Name:com.nasus.controller <br/>
 * Date:2019/2/3 11:21 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 *
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 */
@RestController
@RequestMapping("/student")
public class StudentController {

    @Autowired
    private IStudentService iStudentService;

    @PostMapping("")
    public int addStudent(@RequestBody Student student){
        return iStudentService.add(student);
    }

    @PutMapping("/{id}")
    public String updateStudent(@PathVariable Integer id, @RequestBody Student student){
        Student oldStudent = new Student();
        oldStudent.setId(id);
        oldStudent.setName(student.getName());
        oldStudent.setAge(student.getAge());
        int t = iStudentService.update(oldStudent);
        if (t == 1){
            return student.toString();
        }else {
            return "更新学生信息错误";
        }
    }

    @GetMapping("/{id}")
    public Student findStudentById(@PathVariable Integer id){
        return iStudentService.findStudentById(id);
    }

    @GetMapping("/list")
    public List<Student> findStudentList(){
        return iStudentService.findStudentList();
    }

    @DeleteMapping("/{id}")
    public int deleteStudentById(@PathVariable Integer id){
        return iStudentService.delete(id);
    }
}
```

#### 演示结果

![查询全部学生结果](https://upload-images.jianshu.io/upload_images/3282134-72c670280eda0e2c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

其他的 api 测试可以通过 postman 测试。我这里已经全部测试通过，请放心使用。

源码下载：<https://github.com/turoDog/Demo/tree/master/jdbctemplate_demo>

#### 后语

以上SpringBoot 用 JdbcTemplates 访问Mysql 的教程。最后，对 Python 、Java 感兴趣请长按二维码关注一波，我会努力带给你们价值，如果觉得本文对你哪怕有一丁点帮助，请帮忙点好看，让更多人知道。

另外，关注之后在发送 **1024** 可领取免费学习资料。资料内容详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)







# (七) | 默认日志配置

2019年02月18日 23:55:03

 

一个优秀的废人

 

阅读数：49

更多

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/87656826

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

如题，今天介绍 springboot 默认日志的配置。

#### 默认日志 Logback

默认情况下，Spring Boot 用 Logback 来记录日志，并用 INFO 级别输出到控制台。如果你在平常项目中用过 Spring Boot，你应该已经注意到很多 INFO 级别的日志了。默认日志长这样：

```
2019-02-18 22:02:14.907  INFO 23384 --- [           main] org.hibernate.Version                    : HHH000412: Hibernate Core {5.3.7.Final}
2019-02-18 22:02:14.907  INFO 23384 --- [           main] org.hibernate.cfg.Environment            : HHH000206: hibernate.properties not found
2019-02-18 22:02:15.110  INFO 23384 --- [           main] o.hibernate.annotations.common.Version   : HCANN000001: Hibernate Commons Annotations {5.0.4.Final}
```

从上面的日志可以看到，日志输出内容元素具体如下：

- 时间日期：精确到毫秒
- 日志级别：ERROR, WARN, INFO, DEBUG or TRACE
- 进程 ID
- 分隔符：— 标识实际日志的开始
- 线程名：方括号括起来（可能会截断控制台输出）
- Logger 名：通常使用源代码的类名
- 日志内容

#### 日志依赖

Logback 日志框架依赖于 spring-boot-starter-logging 包，但我们并不需要在 maven 中加入这个依赖，因为 spring-boot-starter其中包含了 spring-boot-starter-logging，该依赖内容就是 Spring Boot 默认的日志框架 logback。

#### 控制台输出

在 Spring Boot 中默认配置了 ERROR、WARN 和 INFO 级别的日志输出到控制台。

我们可以通过两种方式切换至 DEBUG 级别：

- 在运行命令后加入 **–debug** 标志，如：$ java -jar myapp.jar --debug
- 在 application.properties 中配置 debug=true ，该属性置为 true 的时候，核心 Logger（包含嵌入式容器、hibernate、spring）会输出更多内容，但是你自己应用的日志并不会输出为 DEBUG 级别。

#### 多彩输出

如果你的终端支持ANSI，设置彩色输出会让日志更具可读性。通过在 application.properties 中设置 spring.output.ansi.enabled 参数来支持。

- NEVER：禁用 ANSI-colored 输出（默认项）
- DETECT：会检查终端是否支持 ANSI，是的话就采用彩色输出（推荐项）
- ALWAYS：总是使用 ANSI-colored 格式输出，若终端不支持的时候，会有很多干扰信息，不推荐使用

#### 文件输出

Spring Boot默认配置只会输出到控制台，并不会记录到文件中，但是我们通常生产环境使用时都需要以文件方式记录。

若要增加文件输出，需要在 application.properties 中配置 logging.file 或 logging.path属性。

logging.file，设置文件，可以是绝对路径，也可以是相对路径。如：logging.file=my.log
logging.path，设置目录，会在该目录下创建spring.log文件，并写入日志内容，如：logging.path=/var/log

> 注：二者不能同时使用，如若同时使用，则只有logging.file生效
> 默认情况下，日志文件的大小达到 10MB 时会切分一次，产生新的日志文件，默认级别为：ERROR、WARN、INFO

#### 级别控制

在 Spring Boot 中只需要在 application.properties 中进行配置完成日志记录的级别控制。

配置格式：logging.level.*=LEVEL

logging.level：日志级别控制前缀，*为包名或Logger名
LEVEL：选项 TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF

举例：

logging.level.com.nasus=DEBUG：com.nasus 包下所有 class 以 DEBUG 级别输出
logging.level.root=WARN：root日志以 WARN 级别输出

#### 自定义日志配置

根据不同的日志系统，你可以按如下规则组织配置文件名，就能被正确加载：

Logback：logback-spring.xml, logback-spring.groovy, logback.xml, logback.groovy
Log4j：log4j-spring.properties, log4j-spring.xml, log4j.properties, log4j.xml
Log4j2：log4j2-spring.xml, log4j2.xml
JDK (Java Util Logging)：logging.properties

Spring Boot 官方推荐优先使用带有 **-spring** 的文件名作为你的日志配置（如使用 logback-spring.xml，而不是 logback.xml），命名为 logback-spring.xml 的日志配置文件，spring boot 可以为它添加一些 spring boot 特有的配置项（下面会提到）。
默认的命名规则，并且放在 src/main/resources 下面即可

如果你即想完全掌控日志配置，但又不想用 logback.xml 作为 Logback 配置的名字，application.yml 可以通过 logging.config 属性指定自定义的名字：

```properties
logging.config=classpath:logging-config.xml
```

虽然一般并不需要改变配置文件的名字，但是如果你想针对不同运行时 Profile 使用不同的日志配置，这个功能会很有用。

一般不需要这个属性，而是直接在 logback-spring.xml 中使用 springProfile 配置，不需要 logging.config 指定不同环境使用不同配置文件。springProfile 配置在下面介绍。

#### 多环境日志输出

logback-spring.xml :

```xml
<configuration>
    ...

    <!-- 测试环境+开发环境. 多个使用逗号隔开. -->
    <springProfile name="test,dev">
        <logger name="com.example.demo.controller" level="DEBUG" additivity="false">
            <appender-ref ref="consoleLog"/>
        </logger>
    </springProfile>

    <!-- 生产环境. -->
    <springProfile name="prod">
        <logger name="com.example.demo.controller" level="INFO" additivity="false">
            <appender-ref ref="consoleLog"/>
        </logger>
    </springProfile>
</configuration>
```

application.yml 增加环境选择的配置 active: dev

```yaml
spring:
  profiles:
    active: dev
  datasource:
    url: jdbc:mysql://localhost:3306/test?characterEncoding=utf8
    username: root
    password: 123456
```

根据 active 的环境，自动采用上面配置的 springProfile 的 logger 日志。

#### 后语

以上 SpringBoot 默认日志的配置教程。最后，对 Python 、Java 感兴趣请长按二维码关注一波，我会努力带给你们价值，如果觉得本文对你哪怕有一丁点帮助，请帮忙点好看，让更多人知道。

另外，关注之后在发送 **1024** 可领取免费学习资料。资料内容详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)







# (八) | 使用 Spring Data JPA 访问 Mysql 数据库

2019年02月19日 23:15:06

 

一个优秀的废人

 

阅读数：21

 

标签： [SpringBoot](http://so.csdn.net/so/search/s.do?q=SpringBoot&t=blog)[Spring Data JPA](http://so.csdn.net/so/search/s.do?q=Spring%20Data%20JPA&t=blog)[MySql](http://so.csdn.net/so/search/s.do?q=MySql&t=blog)[数据库](http://so.csdn.net/so/search/s.do?q=%E6%95%B0%E6%8D%AE%E5%BA%93&t=blog)[ORM](http://so.csdn.net/so/search/s.do?q=ORM&t=blog)更多

个人分类： [SpringBoot](https://blog.csdn.net/turodog/article/category/8585519)

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/87742048

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

如题，今天介绍 Spring Data JPA 的使用。

#### 什么是 Spring Data JPA

在介绍 Spring Data JPA 之前，首先介绍 Hibernate 。 Hibernate 使用 O/R 映射 （Object-Relation Mapping） 技术实现数据访问， O/R 映射即将领域模型类与数据库的表进行映射，通过程序操作对象而实现表数据操作的能力，让数据访问操作无需关注数据库相关技术。

Hibernate 主导了 EJB 3.0 的 JPA 规范， JPA 即 Java Persistence API。JPA 是一个基于 O/R 映射的标准协议（目前最新版本是 JPA 2.1）。所谓规范即只定义标准规制（如注解、接口），不提供实现，软件提供商可以按照标准规范来实现，而使用者只需按照规范中定义的方式来使用，而不用和软件提供商的实现打交道。JPA 的主要实现由 Hibernate 、 EclipseLink 和 OpenJPA 等完成，我们只要使用 JPA 来开发，无论是哪一个开发方式都是一样的。

Spring Data JPA 是 Spring Data 的一个子项目，它通过基于 JPA 的 Repository 极大地减少了 JPA 作为数据访问方案的代码量。

**简而言之，JPA 是一种 ORM 规范，但并未提供 ORM 实现，而 Hibernate 是一个 ORM 框架，它提供了 ORM 实现。**

#### 准备工作

- IDEA
- JDK1.8
- SpringBoot 2.1.3

pom.xml 文件引入的依赖如下：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.1.3.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <groupId>com.nasus</groupId>
    <artifactId>jpa</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>jpa</name>
    <description>jpa Demo project for Spring Boot</description>

    <properties>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>

        <!-- JPA 依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-jpa</artifactId>
        </dependency>
        <!-- web 依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <!-- mysql 连接类 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>
        <!-- lombok 依赖 -->
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <optional>true</optional>
        </dependency>
        <!-- 单元测试依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>

</project>
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263
```

简单说下，加入 JPA 依赖；mysql 连接类用于连接数据；web 启动类，但凡是 web 应用都需要依赖它；lombok 用于简化实体类。不会的看这篇旧文介绍：[SpringBoot 实战 (三) | 使用 LomBok](https://www.jianshu.com/p/d8b206ec1f7d)

#### application.yaml 配置文件

```yaml
spring:
# 数据库相关
  datasource:
    driver-class-name: com.mysql.jdbc.Driver
    url: jdbc:mysql://127.0.0.1:3306/test?useUnicode=true&characterEncoding=utf8&serverTimezone=UTC&useSSL=true
    username: root
    password: 123456
# JPA 相关
  jpa:
    hibernate:
      ddl-auto: update   #ddl-auto:设为 create 表示每次都重新建表
    show-sql: true
123456789101112
```

#### repository (dao) 层

```java
package com.nasus.jpa.repository;

import com.nasus.jpa.entity.Student;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

/**
 * Project Name:springboot_jpa_demo <br/>
 * Package Name:com.nasus.jpa.repository <br/>
 * Date:2019/2/19 21:37 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 */
@Repository
public interface StudentRepository extends JpaRepository<Student,Integer>, CrudRepository<Student, Integer> {
}
1234567891011121314151617
```

![JPARepository 继承关系](https://upload-images.jianshu.io/upload_images/3282134-1dc0b41dd0f7067c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

从上图，可以看出 JpaRepository 继承于 PangingAndSortingRepository 继承于 CrudRepository 。

CrudRepository 提供基本的增删改查PagingAndSortingRepository 提供分页和排序方法；JpaRepository 提供 JPA 需要的方法。在使用的时候，可以根据具体需要选中继承哪个接口。

使用这些接口的好处有：

1. 继承这些接口，可以使Spring找到自定义的数据库操作接口，并生成代理类，后续可以注入到Spring容器中；
2. 可以不写相关的sql操作，由代理类生成

#### service 层

```java
package com.nasus.jpa.service;

import com.nasus.jpa.entity.Student;
import java.util.List;

/**
 * Project Name:springboot_jpa_demo <br/>
 * Package Name:com.nasus.jpa.service <br/>
 * Date:2019/2/19 21:41 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 */
public interface StudentService {

    Student save(Student student);

    Student findStudentById(Integer id);

    void delete(Integer id);

    void updateStudent(Student student);

    List<Student> findStudentList();
}
123456789101112131415161718192021222324
```

实现类：

```java
package com.nasus.jpa.service.impl;

import com.nasus.jpa.entity.Student;
import com.nasus.jpa.repository.StudentRepository;
import com.nasus.jpa.service.StudentService;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Project Name:springboot_jpa_demo <br/>
 * Package Name:com.nasus.jpa.service.impl <br/>
 * Date:2019/2/19 21:43 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 */
@Service
public class StudentServiceImpl implements StudentService {

    @Autowired
    private StudentRepository studentRepository;

    /**
     * 保存学生信息
     * @param student
     * @return
     */
    @Override
    public Student save(Student student) {
        return studentRepository.save(student);
    }

    /**
     * 根据 Id 查询学生信息
     * @param id
     * @return
     */
    @Override
    public Student findStudentById(Integer id) {
        return studentRepository.findById(id).get();
    }

    /**
     * 删除学生信息
     * @param id
     */
    @Override
    public void delete(Integer id) {
        Student student = this.findStudentById(id);
        studentRepository.delete(student);
    }

    /**
     * 更新学生信息
     * @param student
     */
    @Override
    public void updateStudent(Student student) {
        studentRepository.save(student);
    }

    /**
     * 查询学生信息列表
     * @return
     */
    @Override
    public List<Student> findStudentList() {
        return studentRepository.findAll();
    }
}
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364656667686970
```

#### controller 层构建 restful API

```java
package com.nasus.jpa.controller;

import com.nasus.jpa.entity.Student;
import com.nasus.jpa.service.StudentService;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * Project Name:springboot_jpa_demo <br/>
 * Package Name:com.nasus.jpa.controller <br/>
 * Date:2019/2/19 21:55 <br/>
 * <b>Description:</b> TODO: 描述该类的作用 <br/>
 * @author <a href="turodog@foxmail.com">nasus</a><br/>
 */
@RestController
@RequestMapping("/student")
public class StudentController {

    @Autowired
    private StudentService studentService;

    @PostMapping("/save")
    public Student saveStudent(@RequestBody Student student){
        return studentService.save(student);
    }

    @GetMapping("/{id}")
    public Student findStudentById(@PathVariable("id") Integer id){
        return studentService.findStudentById(id);
    }

    @GetMapping("/list")
    public List<Student> findStudentList(){
        return studentService.findStudentList();
    }

    @DeleteMapping("/{id}")
    public void deleteStudentById(@PathVariable("id") Integer id){
        studentService.delete(id);
    }

    @PutMapping("/update")
    public void updateStudent(@RequestBody Student student){
        studentService.updateStudent(student);
    }
}
```

#### 测试结果

![根据 Id 查询学生信息的测试结果](https://upload-images.jianshu.io/upload_images/3282134-cdb343b52234ff9e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

其他接口已通过 postman 测试，无问题。

源码下载：<https://github.com/turoDog/Demo/tree/master/springboot_jpa_demo>

#### 后语

以上为 SpringBoot 使用 Spring Data JPA 访问 Mysql 数据库的教程。最后，对 Python 、Java 感兴趣请长按二维码关注一波，我会努力带给你们价值，如果觉得本文对你哪怕有一丁点帮助，请帮忙点好看，让更多人知道。

另外，关注之后在发送 **1024** 可领取免费学习资料。资料内容详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)







# (九) | 整合 Mybatis

2019年02月20日 22:07:39

 

一个优秀的废人

 

阅读数：18

 

标签： [SpringBoot](http://so.csdn.net/so/search/s.do?q=SpringBoot&t=blog)[Mybatis](http://so.csdn.net/so/search/s.do?q=Mybatis&t=blog)[JavaWeb](http://so.csdn.net/so/search/s.do?q=JavaWeb&t=blog)[Spring](http://so.csdn.net/so/search/s.do?q=Spring&t=blog)[MySQL](http://so.csdn.net/so/search/s.do?q=MySQL&t=blog)更多

个人分类： [SpringBoot](https://blog.csdn.net/turodog/article/category/8585519)

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/87827054

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

如题，今天介绍 SpringBoot 与 Mybatis 的整合以及 Mybatis 的使用，本文通过注解的形式实现。

#### 什么是 Mybatis

MyBatis 是支持定制化 SQL、存储过程以及高级映射的优秀的持久层框架。MyBatis 避免了几乎所有的 JDBC 代码和手动设置参数以及获取结果集。MyBatis 可以对配置和原生 Map 使用简单的 XML 或注解，将接口和 Java 的 POJOs(Plain Old Java Objects,普通的 Java 对象)映射成数据库中的记录。

优点：

- 简单易学：本身就很小且简单。没有任何第三方依赖，最简单安装只要两个 jar 文件+配置几个 sql 映射文件易于学习，易于使用，通过文档和源代码，可以比较完全的掌握它的设计思路和实现。
- 灵活：mybatis 不会对应用程序或者数据库的现有设计强加任何影响。 sql 写在 xml 里，便于统一管理和优化。通过 sql 基本上可以实现我们不使用数据访问框架可以实现的所有功能，或许更多。
- 解除 sql 与程序代码的耦合：通过提供 DAL 层，将业务逻辑和数据访问逻辑分离，使系统的设计更清晰，更易维护，更易单元测试。sql 和代码的分离，提高了可维护性。
- 提供映射标签，支持对象与数据库的 orm 字段关系映射
- 提供对象关系映射标签，支持对象关系组建维护
- 提供xml标签，支持编写动态 sql。

缺点：

- 编写 SQL 语句时工作量很大，尤其是字段多、关联表多时，更是如此。
- SQL 语句依赖于数据库，导致数据库移植性差，不能更换数据库。
- 框架还是比较简陋，功能尚有缺失，虽然简化了数据绑定代码，但是整个底层数据库查询实际还是要自己写的，工作量也比较大，而且不太容易适应快速数据库修改。
- 二级缓存机制不佳

#### 准备工作

- IDEA
- JDK1.8
- SpringBoot 2.1.3

sql 语句，创建表，插入数据：

```sql
CREATE TABLE `student` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(20) NOT NULL,
  `age` double DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
INSERT INTO `student` VALUES ('1', 'aaa', '21');
INSERT INTO `student` VALUES ('2', 'bbb', '22');
INSERT INTO `student` VALUES ('3', 'ccc', '23');
123456789
```

#### pom.xml 文件配置依赖

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.1.3.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <groupId>com.nasus</groupId>
    <artifactId>mybatis</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>mybatis</name>
    <description>mybatis Demo project for Spring Boot</description>

    <properties>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>
        <!-- 启动 web -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <!-- mybatis 依赖 -->
        <dependency>
            <groupId>org.mybatis.spring.boot</groupId>
            <artifactId>mybatis-spring-boot-starter</artifactId>
            <version>2.0.0</version>
        </dependency>
        <!-- mysql 连接类 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>
        <!-- druid 数据库连接池-->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>druid</artifactId>
            <version>1.1.9</version>
        </dependency>
        <!-- lombok 插件 用于简化实体代码 -->
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <optional>true</optional>
        </dependency>
        <!-- 单元测试 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
      
        <dependency>
            <groupId>org.hibernate.javax.persistence</groupId>
            <artifactId>hibernate-jpa-2.1-api</artifactId>
            <version>1.0.0.Final</version>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>
</project>
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364656667686970717273
```

#### application.yaml 配置文件

```yaml
spring:
  datasource:
    url: jdbc:mysql://localhost:3306/test
    username: root
    password: 123456
    driver-class-name: com.mysql.cj.jdbc.Driver
123456
```

#### 实体类

```java
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Student {

    @Id
    @GeneratedValue
    private Integer id;

    private String name;

    private Integer age;

}
1234567891011121314151617181920
```

使用了 lombok 简化了代码。

#### dao 层

```java
import com.nasus.mybatis.domain.Student;
import java.util.List;
import org.apache.ibatis.annotations.Delete;
import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

@Mapper
public interface StudentMapper {

    @Insert("insert into student(name, age) values(#{name}, #{age})")
    int add(Student student);

    @Update("update student set name = #{name}, age = #{age} where id = #{id}")
    int update(@Param("name") String name, @Param("age") Integer age, @Param("id") Integer id);

    @Delete("delete from student where id = #{id}")
    int delete(int id);

    @Select("select id, name as name, age as age from student where id = #{id}")
    Student findStudentById(@Param("id") Integer id);

    @Select("select id, name as name, age as age from student")
    List<Student> findStudentList();
}
123456789101112131415161718192021222324252627
```

这里有必要解释一下，@Insert 、@Update、@Delete、@Select 这些注解中的每一个代表了执行的真实 SQL。 它们每一个都使用字符串数组 (或单独的字符串)。如果传递的是字符串数组，它们由每个分隔它们的单独空间串联起来。这就当用 Java 代码构建 SQL 时避免了“丢失空间”的问题。 然而，如果你喜欢，也欢迎你串联单独 的字符串。属性:value，这是字符串 数组用来组成单独的 SQL 语句。

@Param 如果你的映射方法的形参有多个，这个注解使用在映射方法的参数上就能为它们取自定义名字。若不给出自定义名字，多参数（不包括 RowBounds 参数）则先以 “param” 作前缀，再加上它们的参数位置作为参数别名。例如 #{param1}，#{param2}，这个是默认值。如果注解是 @Param(“id”)，那么参数就会被命名为 #{id}。

#### service 层

```java
import com.nasus.mybatis.domain.Student;
import java.util.List;

public interface StudentService {

    int add(Student student);

    int update(String name, Integer age, Integer id);

    int delete(Integer id);

    Student findStudentById(Integer id);

    List<Student> findStudentList();

}
12345678910111213141516
```

实现类：

```java
import com.nasus.mybatis.dao.StudentMapper;
import com.nasus.mybatis.domain.Student;
import com.nasus.mybatis.service.StudentService;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class StudentServiceImpl implements StudentService {

    @Autowired
    private StudentMapper studentMapper;

    /**
     * 添加 Student
     * @param name
     * @param age
     * @return
     */
    @Override
    public int add(Student student) {
        return studentMapper.add(student);
    }

    /**
     * 更新 Student
     * @param name
     * @param age
     * @param id
     * @return
     */
    @Override
    public int update(String name, Integer age, Integer id) {
        return studentMapper.update(name,age,id);
    }

    /**
     * 删除 Student
     * @param id
     * @return
     */
    @Override
    public int delete(Integer id) {
        return studentMapper.delete(id);
    }

    /**
     * 根据 id 查询 Student
     * @param id
     * @return
     */
    @Override
    public Student findStudentById(Integer id) {
        return studentMapper.findStudentById(id);
    }

    /**
     * 查询所有的 Student
     * @return
     */
    @Override
    public List<Student> findStudentList() {
        return studentMapper.findStudentList();
    }
}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465
```

#### controller 层构建 restful API

```java
import com.nasus.mybatis.domain.Student;
import com.nasus.mybatis.service.StudentService;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/Student")
public class StudentController {

    @Autowired
    private StudentService studentService;

    @PostMapping("")
    public int add(@RequestBody Student student){
        return studentService.add(student);
    }

    @PutMapping("/{id}")
    public int updateStudent(@PathVariable("id") Integer id, @RequestParam(value = "name", required = true) String name,
            @RequestParam(value = "age", required = true) Integer age){
        return studentService.update(name,age,id);
    }

    @DeleteMapping("/{id}")
    public void deleteStudent(@PathVariable("id") Integer id){
        studentService.delete(id);
    }

    @GetMapping("/{id}")
    public Student findStudentById(@PathVariable("id") Integer id){
        return studentService.findStudentById(id);
    }

    @GetMapping("/list")
    public List<Student> findStudentList(){
        return studentService.findStudentList();
    }
}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647
```

#### 测试结果

![查询全部学生信息结果](https://upload-images.jianshu.io/upload_images/3282134-daab175b17d0255d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

其他接口已通过 postman 测试，无问题。

源码下载：[github 地址](https://github.com/turoDog/Demo/tree/master/springboot_mybatis_demo)

#### 后语

以上为 SpringBoot 实战 (九) | 整合 Mybatis 的教程，除了注解方式实现以外，Mybatis 还提供了 XML 方式实现。想了解更多用法请移步[官方文档](http://blog.mybatis.org/)。

最后，对 Python 、Java 感兴趣请长按二维码关注一波，我会努力带给你们价值，如果觉得本文对你哪怕有一丁点帮助，请帮忙点好看，让更多人知道。

另外，关注之后在发送 **1024** 可领取免费学习资料。资料内容详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)









# (十) | 声明式事务

2019年02月21日 21:36:02

 

一个优秀的废人

 

阅读数：22

 

标签： [SpringBoot](http://so.csdn.net/so/search/s.do?q=SpringBoot&t=blog)[声明式事务](http://so.csdn.net/so/search/s.do?q=%E5%A3%B0%E6%98%8E%E5%BC%8F%E4%BA%8B%E5%8A%A1&t=blog)[JavaWeb](http://so.csdn.net/so/search/s.do?q=JavaWeb&t=blog)[Transaction](http://so.csdn.net/so/search/s.do?q=Transaction&t=blog)更多

个人分类： [SpringBoot](https://blog.csdn.net/turodog/article/category/8585519)

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/87868994

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

如题，今天介绍 SpringBoot 的 声明式事务。

#### Spring 的事务机制

所有的数据访问技术都有事务处理机制，这些技术提供了 API 用于开启事务、提交事务来完成数据操作，或者在发生错误时回滚数据。

而 Spring 的事务机制是用统一的机制来处理不同数据访问技术的事务处理，Spring 的事务机制提供了一个 PlatformTransactionManager 接口，不同的数据访问技术的事务使用不同的接口实现，如下表：

| 数据访问技术 | 实现                         |
| ------------ | ---------------------------- |
| JDBC         | DataSourceTransactionManager |
| JPA          | JPATransactionManager        |
| Hibernate    | HibernateTransactionManager  |
| JDO          | JdoTransactionManager        |
| 分布式事务   | JtaTransactionManager        |

#### 声明式事务

Spring 支持声明式事务，即使用注解来选择需要使用事务的方法，他使用 @Transactional 注解在方法上表明该方法需要事务支持。被注解的方法在被调用时，Spring 开启一个新的事务，当方法无异常运行结束后，Spring 会提交这个事务。如：

```java
@Transactional
public void saveStudent(Student student){
        // 数据库操作
}
1234
```

注意，@Transactional 注解来自于 org.springframework.transcation.annotation 包，而不是 javax.transaction。

Spring 提供一个 @EnableTranscationManagement 注解在配置类上来开启声明式事务的支持。使用了 @EnableTranscationManagement 后，Spring 容器会自动扫描注解 @Transactional 的方法与类。@EnableTranscationManagement 的使用方式如下：

```java
@Configuration
@EnableTranscationManagement 
public class AppConfig{

}
12345
```

#### 注解事务行为

@Transactional 有如下表所示的属性来定制事务行为。

| 属性                   | 含义                                                         |
| ---------------------- | ------------------------------------------------------------ |
| propagation            | 事务传播行为                                                 |
| isolation              | 事务隔离级别                                                 |
| readOnly               | 事务的读写性，boolean型                                      |
| timeout                | 超时时间，int型，以秒为单位。                                |
| rollbackFor            | 一组异常类，遇到时回滚。（rollbackFor={SQLException.class}） |
| rollbackForCalssName   | 一组异常类名，遇到回滚，类型为 string[]                      |
| noRollbackFor          | 一组异常类，遇到不回滚                                       |
| norollbackForCalssName | 一组异常类名，遇到时不回滚。                                 |

#### 类级别使用 @Transactional

@Transactional 不仅可以注解在方法上，还可以注解在类上。注解在类上时意味着此类的所有 public 方法都是开启事务的。如果类级别和方法级别同时使用了 @Transactional 注解，则使用在类级别的注解会重载方法级别的注解。

#### SpringBoot 的事务支持

1. 自动配置的事务管理器
   在使用 JDBC 作为数据访问技术时，配置定义如下：

```java
@Bean
@ConditionalOnMissingBean
@ConditionalOnBean(DataSource.class)
public PlatformTransactionManager transactionManager(){
    return new DataSourceTransactionManager(this.dataSource)
}
123456
```

在使用 JPA 作为数据访问技术时，配置定义如下：

```java
@Bean
@ConditionalOnMissingBean(PlatformTransactionManager.class)
public PlatformTransactionManager transactionManager(){
    return new JpaTransactionManager()
}
12345
```

1. 自动开启注解事务的支持
   SpringBoot 专门用于配置事务的类为 org.springframework.boot.autoconfigure.transcation.TransactionAutoConfiguration，此配置类依赖于 JpaBaseConfiguration 和 DataSourceTransactionManagerAutoConfiguration。
   而在 DataSourceTransactionManagerAutoConfiguration 配置里还开启了对声明式事务的支持，代码如下：

```java
@ConditionalOnMissingBean(AbstractTransactionManagementConfiguration.class)
@Configuration
@EnableTransactionManagement
protected static class TransactionManagementConfiguration{

}
123456
```

所以在 SpringBoot 中，无须显式开启使用 @EnableTransactionManagement 注解。

#### 实战

演示如何使用 Transactional 使用异常导致数据回滚与使用异常导致数据不回滚。

1. 准备工作：
   SpringBoot 2.1.3
   JDK 1.8
   IDEA
2. pom.xml 依赖：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.1.3.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <groupId>com.nasus</groupId>
    <artifactId>transaction</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>transaction</name>
    <description>transaction Demo project for Spring Boot</description>

    <properties>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>
        <!-- JPA 相关 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-jpa</artifactId>
        </dependency>
        <!-- web 启动类 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <!-- mysql 连接类 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>
        <!-- lombok 插件，简化实体代码 -->
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <version>1.16.20</version>
        </dependency>
        <!-- 单元测试 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>
</project>
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960
```

代码注释很清楚，没啥好说的。

1. application.yaml 配置：

```yaml
spring:
  # \u6570\u636E\u5E93\u76F8\u5173
  datasource:
    driver-class-name: com.mysql.cj.jdbc.Driver
    url: jdbc:mysql://127.0.0.1:3306/test?useUnicode=true&characterEncoding=utf8&serverTimezone=UTC&useSSL=true
    username: root
    password: 123456
  # jpa \u76F8\u5173
  jpa:
    hibernate:
      ddl-auto: update   # ddl-auto:\u8BBE\u4E3A create \u8868\u793A\u6BCF\u6B21\u90FD\u91CD\u65B0\u5EFA\u8868
    show-sql: true
123456789101112
```

1. 实体类：

```java
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@Entity
@AllArgsConstructor
@NoArgsConstructor
public class Student {

    @Id
    @GeneratedValue
    private Integer id;

    private String name;

    private Integer age;
}
123456789101112131415161718192021
```

1. dao 层

```java
import com.nasus.transaction.domain.Student;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface StudentRepository extends JpaRepository<Student, Integer> {
}
1234567
```

1. service 层

```java
import com.nasus.transaction.domain.Student;

public interface StudentService {

    Student saveStudentWithRollBack(Student student);

    Student saveStudentWithoutRollBack(Student student);

}
123456789
```

实现类：

```java
import com.nasus.transaction.domain.Student;
import com.nasus.transaction.repository.StudentRepository;
import com.nasus.transaction.service.StudentService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@Service
public class StudentServiceImpl implements StudentService {

    @Autowired
    // 直接注入 StudentRepository 的 bean
    private StudentRepository studentRepository;

    // 使用 @Transactional 注解的 rollbackFor 属性，指定特定异常时，触发回滚
    @Transactional(rollbackFor = {IllegalArgumentException.class})
    @Override
    public Student saveStudentWithRollBack(Student student) {
        Student s = studentRepository.save(student);
        if ("高斯林".equals(s.getName())){
            //硬编码，手动触发异常
            throw new IllegalArgumentException("高斯林已存在，数据将回滚");
        }
        return s;
    }

    // 使用 @Transactional 注解的 noRollbackFor 属性，指定特定异常时，不触发回滚
    @Transactional(noRollbackFor = {IllegalArgumentException.class})
    @Override
    public Student saveStudentWithoutRollBack(Student student) {
        Student s = studentRepository.save(student);
        if ("高斯林".equals(s.getName())){
            throw new IllegalArgumentException("高斯林已存在，数据将不会回滚");
        }
        return s;
    }

}
1234567891011121314151617181920212223242526272829303132333435363738
```

代码注释同样很清楚，没啥好说的。

1. controller 层

```java
import com.nasus.transaction.domain.Student;
import com.nasus.transaction.service.StudentService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/student")
public class StudentController {

    // 注入 studentservice 的 bean
    @Autowired
    private StudentService studentService;

    // 测试回滚情况
    @PostMapping("/withRollBack")
    public Student saveStudentWithRollBack(@RequestBody Student student){
        return studentService.saveStudentWithRollBack(student);
    }

    // 测试不回滚情况
    @PostMapping("/withOutRollBack")
    public Student saveStudentWithoutRollBack(@RequestBody Student student){
        return studentService.saveStudentWithoutRollBack(student);
    }
}
12345678910111213141516171819202122232425262728
```

#### Postman 测试结果

为了更清楚地理解回滚，以 debug (调试模式) 启动程序。并在 StudentServiceImpl 的 saveStudentWithRollBack 方法上打上断点。

测试前数据库结果：
![测试前数据库结果](https://upload-images.jianshu.io/upload_images/3282134-0033788ae5d56b79.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

1. Postman 测试回滚
   ![Postman 测试异常导致数据回滚](https://upload-images.jianshu.io/upload_images/3282134-01d78cef0f9a7782.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
   debug 模式下可见数据已保存，且获得 id 为 1。：
   ![回滚](https://upload-images.jianshu.io/upload_images/3282134-508aecc3e4be804e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

继续执行抛出异常 IllegalArgumentException，将导致数据回滚：
![导致数据回滚，控制台打印出信息](https://upload-images.jianshu.io/upload_images/3282134-b53f913f194e7ac5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

测试后数据库结果：并无新增数据，回滚成功。
![测试后数据库结果](https://upload-images.jianshu.io/upload_images/3282134-0033788ae5d56b79.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

1. Postman 测试不回滚

测试前数据库结果：
![测试前数据库结果](https://upload-images.jianshu.io/upload_images/3282134-0033788ae5d56b79.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

遇到 IllegalArgumentException 异常数据不会回滚：
![数据不回滚，控制台打印信息](https://upload-images.jianshu.io/upload_images/3282134-7e0a089a63e70b96.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

测试后数据库结果：新增数据，数据不回滚。
![新增数据，数据不回滚](https://upload-images.jianshu.io/upload_images/3282134-93d745e71807f510.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### 源码下载

<https://github.com/turoDog/Demo/tree/master/springboot_transaction_demo>

#### 后语

以上为 SpringBoot 声明式事务的教程。最后，对 Python 、Java 感兴趣请长按二维码关注一波，我会努力带给你们价值，如果觉得本文对你哪怕有一丁点帮助，请帮忙点好看，让更多人知道。

另外，关注之后在发送 **1024** 可领取免费学习资料。资料内容详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)







# (十一) | 整合数据缓存 Cache

2019年02月23日 00:34:01

 

一个优秀的废人

 

阅读数：12

 

标签： [SpringBoot](http://so.csdn.net/so/search/s.do?q=SpringBoot&t=blog)[cache](http://so.csdn.net/so/search/s.do?q=cache&t=blog)[EhCache](http://so.csdn.net/so/search/s.do?q=EhCache&t=blog)[JavaWeb](http://so.csdn.net/so/search/s.do?q=JavaWeb&t=blog)[Spring](http://so.csdn.net/so/search/s.do?q=Spring&t=blog)更多

个人分类： [SpringBoot](https://blog.csdn.net/turodog/article/category/8585519)

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/87887814

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

如题，今天介绍 SpringBoot 的数据缓存。做过开发的都知道程序的瓶颈在于数据库，我们也知道内存的速度是大大快于硬盘的，当需要重复获取相同数据时，一次又一次的请求数据库或者远程服务，导致大量时间耗费在数据库查询或远程方法调用上，导致性能的恶化，这便是数据缓存要解决的问题。

#### Spring 的缓存支持

Spring 定义了 org.springframework.cache.CacheManager 和 org.springframework.cache.Cache 接口用于统一不同的缓存技术。其中，CacheManager 是 Spring 提供的各种缓存技术的抽象接口，Cache 接口则是包含了缓存的各种操作（增加，删除，获取缓存，一般不会直接和此接口打交道）。

*1、*Spring 支持的 CacheManager

针对不同的缓存技术，实现了不同的 CacheManager ，Spring 定义了下表所示的 CacheManager：

| CacheManager              | 描述                                                         |
| ------------------------- | ------------------------------------------------------------ |
| SimpleCacheManager        | 使用简单的 Collection 来存储缓存，主要用于测试               |
| ConcurrentMapCacheManager | 使用 ConcurrentMap 来存储缓存                                |
| NoOpCacheManager          | 仅测试用途，不会实际缓存数据                                 |
| EhCacheCacheManager       | 使用 EhCache 作为缓存技术                                    |
| GuavaCacheManager         | 使用 Google Guava 的 GuavaCache 作为缓存技术                 |
| HazelcastCacheManager     | 使用 Hazelcast 作为缓存技术                                  |
| JCacheCacheManager        | 支持 JCache(JSR-107) 标准的实现作为缓存技术，如 ApacheCommonsJCS |
| RedisCacheManager         | 使用 Redis 作为缓存技术                                      |

在使用以上任意一个实现的 CacheManager 的时候，需注册实现的 CacheManager 的 Bean，如：

```java
@Bean
public EhCacheCacheManager cacheManager(CacheManager 
 ehCacheCacheManager){
    return new  EhCacheCacheManager(ehCacheCacheManager);
}
12345
```

注意，每种缓存技术都有很多的额外配置，但配置 cacheManager 是必不可少的。

*2、*声明式缓存注解

Spring 提供了 4 个注解来声明缓存规则（又是使用注解式的 AOP 的一个例子）。4 个注解如下表示：

| 注解        | 解释                                                         |
| ----------- | ------------------------------------------------------------ |
| @Cacheable  | 在方法执行前 Spring 先查看缓存中是否有数据，若有，则直接返回缓存数据；若无数据，调用方法将方法返回值放入缓存中 |
| @CachePut   | 无论怎样，都会将方法的返回值放到缓存中。                     |
| @CacheEvict | 将一条或多条数据从缓存中删除                                 |
| @Caching    | 可以通过 @Caching 注解组合多个注解策略在一个方法上           |

@Cacheable、@CachePut、@CacheEvict 都有 value 属性，指定的是要使用的缓存名称；key 属性指定的是数据在缓存中存储的键。

*3、*开启声明式缓存支持

开启声明式缓存很简单，只需在配置类上使用 @EnableCaching 注解即可，例如：

```java
@Configuration
@EnableCaching
public class AppConfig{

}
12345
```

#### SpringBoot 的支持

在 Spring 中使用缓存技术的关键是配置 CacheManager ，而 SpringBoot 为我们配置了多个 CacheManager 的实现。

它的自动配置放在 org.springframework.boot.autoconfigure.cache 包中。

在不做任何配置的情况下，默认使用的是 SimpleCacheConfiguration ，即使用 ConcurrentMapCacheManager。SpringBoot 支持以前缀来配置缓存。例如：

```yaml
spring.cache.type= # 可选 generic、ehcache、hazelcast、infinispan、jcache、redis、guava、simple、none
spring.cache.cache-names= # 程序启动时创建的缓存名称
spring.cache.ehcache.config= # ehcache 配置文件的地址
spring.cache.hazelcast.config= # hazelcast配置文件的地址
spring.cache.infinispan.config= # infinispan配置文件的地址
spring.cache.jcache.config= # jcache配置文件的地址
spring.cache.jcache.provider= # 当多个 jcache 实现在类路径的时候，指定 jcache 实现
# 等等。。。
12345678
```

在 SpringBoot 环境下，使用缓存技术只需要在项目中导入相关缓存技术的依赖包，并在配置类中使用 @EnableCaching 开启缓存支持即可。

#### 代码实现

本文将以 SpringBoot 默认的 ConcurrentMapCacheManager 作为缓存技术，演示 @Cacheable、@CachePut、@CacheEvict。

*1、*准备工作

- IDEA
- JDK 1.8
- SpringBoot 2.1.3

*2、*Pom.xml 文件依赖

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.1.3.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <groupId>com.nasus</groupId>
    <artifactId>cache</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>cache</name>
    <description>cache Demo project for Spring Boot</description>

    <properties>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>
        <!-- cache 依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-cache</artifactId>
        </dependency>
        <!-- JPA 依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-jpa</artifactId>
        </dependency>
        <!-- web 启动类 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <!-- mysql 数据库连接类 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>
        <!-- lombok 依赖，简化实体 -->
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <optional>true</optional>
        </dependency>
        <!-- 单元测试类 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>

</project>
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566
```

注释很清楚，无需多言。不会就谷歌一下。

*3、*Application.yaml 文件配置

```yaml
spring:
  # 数据库相关
  datasource:
    driver-class-name: com.mysql.cj.jdbc.Driver
    url: jdbc:mysql://127.0.0.1:3306/test?useUnicode=true&characterEncoding=utf8&serverTimezone=UTC&useSSL=true
    username: root
    password: 123456
  # jpa 相关
  jpa:
    hibernate:
      ddl-auto: update   # ddl-auto: 设为 create 表示每次都重新建表
    show-sql: true
123456789101112
```

*4、*实体类

```java
package com.nasus.cache.entity;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@Entity
@AllArgsConstructor
@NoArgsConstructor
public class Student {

    @Id
    @GeneratedValue
    private Integer id;

    private String name;

    private Integer age;

}
123456789101112131415161718192021222324
```

*5、*dao 层

```java
package com.nasus.cache.repository;

import com.nasus.cache.entity.Student;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface StudentRepository extends JpaRepository<Student,Integer> {
}
123456789
```

*6、*service 层

```java
package com.nasus.cache.service;

import com.nasus.cache.entity.Student;

public interface StudentService {

    public Student saveStudent(Student student);

    public void deleteStudentById(Integer id);

    public Student findStudentById(Integer id);

}
12345678910111213
```

实现类：

```java
package com.nasus.cache.service.impl;

import com.nasus.cache.entity.Student;
import com.nasus.cache.repository.StudentRepository;
import com.nasus.cache.service.StudentService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.cache.annotation.CacheEvict;
import org.springframework.cache.annotation.CachePut;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.stereotype.Service;

@Service
public class StudentServiceImpl implements StudentService {

    // 使用 slf4j 作为日志框架
    private static final Logger LOGGER = LoggerFactory.getLogger(StudentServiceImpl.class);

    @Autowired
    private StudentRepository studentRepository;

    @Override
    @CachePut(value = "student",key = "#student.id")
    // @CachePut 缓存新增的或更新的数据到缓存，其中缓存名称为 student 数据的 key 是 student 的 id
    public Student saveStudent(Student student) {
        Student s = studentRepository.save(student);
        LOGGER.info("为id、key 为{}的数据做了缓存", s.getId());
        return s;
    }

    @Override
    @CacheEvict(value = "student")
    // @CacheEvict 从缓存 student 中删除 key 为 id 的数据
    public void deleteStudentById(Integer id) {
        LOGGER.info("删除了id、key 为{}的数据缓存", id);
        //studentRepository.deleteById(id);
    }

    @Override
    @Cacheable(value = "student",key = "#id")
    // @Cacheable 缓存 key 为 id 的数据到缓存 student 中
    public Student findStudentById(Integer id) {
        Student s = studentRepository.findById(id).get();
        LOGGER.info("为id、key 为{}的数据做了缓存", id);
        return  s;
    }
}
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748
```

代码讲解看注释，很详细。

*7、*controller 层

```java
package com.nasus.cache.controller;

import com.nasus.cache.entity.Student;
import com.nasus.cache.service.StudentService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.Mapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/student")
public class StudentController {

    @Autowired
    private StudentService studentService;

    @PostMapping("/put")
    public Student saveStudent(@RequestBody Student student){
        return studentService.saveStudent(student);
    }

    @DeleteMapping("/evit/{id}")
    public void deleteStudentById(@PathVariable("id") Integer id){
        studentService.deleteStudentById(id);
    }

    @GetMapping("/able/{id}")
    public Student findStudentById(@PathVariable("id") Integer id){
        return studentService.findStudentById(id);
    }

}
12345678910111213141516171819202122232425262728293031323334353637
```

*8、*application 开启缓存功能

```java
package com.nasus.cache;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cache.annotation.EnableCaching;

@EnableCaching // 开启缓存功能
@SpringBootApplication
public class CacheApplication {

    public static void main(String[] args) {
        SpringApplication.run(CacheApplication.class, args);
    }
}
1234567891011121314
```

#### 测试

测试前，先看一眼数据库当前的数据，如下：

![原始数据库数据](https://upload-images.jianshu.io/upload_images/3282134-315130125a1e2c8b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

*1、*测试 @Cacheable

访问 <http://localhost:8080/student/able/2> 控制台打印出了 SQL 查询语句，以及指定日志。说明这一次程序是直接查询数据库得到的结果。

```
2019-02-21 22:54:54.651  INFO 1564 --- [nio-8080-exec-1] o.s.web.servlet.DispatcherServlet        : Completed initialization in 11 ms
Hibernate: select student0_.id as id1_0_0_, student0_.age as age2_0_0_, student0_.name as name3_0_0_ from student student0_ where student0_.id=?
2019-02-21 22:54:59.725  INFO 1564 --- [nio-8080-exec-1] c.n.c.service.impl.StudentServiceImpl    : 为id、key 为2的数据做了缓存
123
```

postman 第一次测试结果 ：

![postman 第一次测试结果](https://upload-images.jianshu.io/upload_images/3282134-62444c02894988e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

再次访问 <http://localhost:8080/student/able/2> 结果如下图。但控制台无 SQL 语句打印，也无**为id、key 为2的数据做了缓存**这句话输出。

说明 @Cacheable 确实做了数据缓存，第二次的测试结果是从数据缓存中获取的，并没有直接查数据库。

![postman 第二次测试结果](https://upload-images.jianshu.io/upload_images/3282134-62444c02894988e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

*2、*测试 @CachePut

如下图，postman 访问 <http://localhost:8080/student/put> 插入数据：
![postman 测试 @CachePut](https://upload-images.jianshu.io/upload_images/3282134-4a00d3b053f50bb8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

下面是控制台打印出了 SQL Insert 插入语句，以及指定日志。说明程序做了缓存。

```
Hibernate: insert into student (age, name, id) values (?, ?, ?)
2019-02-21 23:12:03.688  INFO 1564 --- [nio-8080-exec-8] c.n.c.service.impl.StudentServiceImpl    : 为id、key 为4的数据做了缓存
12
```

插入数据返回的结果：

![postman 第一次测试结果](https://upload-images.jianshu.io/upload_images/3282134-9f14487d78038c92.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

数据库中的结果：

![新增的数据](https://upload-images.jianshu.io/upload_images/3282134-40df0b75f93edce1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

访问 <http://localhost:8080/student/able/4> Postman 结果如下图。控制台无输出，验证了 @CachePut 确实做了缓存，下图数据是从缓存中获取的。

![@Cacheable 测试结果](https://upload-images.jianshu.io/upload_images/3282134-0c054e4e37aa05ef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

*3、*测试 @CacheEvict

postman 访问 <http://localhost:8080/student/able/3> 为 id = 3 的数据做缓存。

postman 再次访问 <http://localhost:8080/student/able/3> 确认数据是从缓存中获取的。

postman 访问 <http://localhost:8080/student/evit/3>

![postman 访问 http://localhost:8080/student/evit/3 ](https://upload-images.jianshu.io/upload_images/3282134-1fc6d1eef21a174b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

从缓存中删除 key 为 3 的缓存数据：

```
Hibernate: select student0_.id as id1_0_0_, student0_.age as age2_0_0_, student0_.name as name3_0_0_ from student student0_ where student0_.id=?
2019-02-21 23:26:08.516  INFO 8612 --- [nio-8080-exec-2] c.n.c.service.impl.StudentServiceImpl    : 为id、key 为3的数据做了缓存
2019-02-21 23:27:01.508  INFO 8612 --- [nio-8080-exec-4] c.n.c.service.impl.StudentServiceImpl    : 删除了id、key 为3的数据缓存
123
```

再次 postman 访问 <http://localhost:8080/student/able/3> 观察后台，重新做了数据缓存：

```
Hibernate: select student0_.id as id1_0_0_, student0_.age as age2_0_0_, student0_.name as name3_0_0_ from student student0_ where student0_.id=?
2019-02-21 23:27:12.320  INFO 8612 --- [nio-8080-exec-5] c.n.c.service.impl.StudentServiceImpl    : 为id、key 为3的数据做了缓存
12
```

这一套测试流程下来，证明了 @CacheEvict 确实删除了数据缓存。

#### 源码下载

<https://github.com/turoDog/Demo/tree/master/springboot_cache_demo>

#### 切换缓存技术

切换缓存技术除了在 pom 文件加入相关依赖包配置以外，使用方式与上面的代码演示一样。

*1、*切换 EhCache
在 pom 中添加 Encache 依赖：

```
<!-- EhCache 依赖 -->
<dependency>
     <groupId>net.sf.ehcache</groupId>
     <artifactId>ehcache</artifactId>
</dependency>
12345
```

Ehcache 所需配置文件 ehcache.xml 只需放在类路径（resource 目录）下，SpringBoot 会自动扫描，如：

```
<?xml version="1.0" encoding="UTF-8">
<ehcache>
    <cache name="student" maxElementsInMmory="1000">
<ehcache>
1234
```

SpringBoot 会自动配置 EhcacheManager 的 Bean。

*2、*切换 Guava

只需在 pom 中加入 Guava 依赖即可：

```
<!-- GuavaCache 依赖 -->
<dependency>
     <groupId>com.google.guava</groupId>
     <artifactId>guava</artifactId>
     <version>18.0</version>
</dependency>
123456
```

SpringBoot 会自动配置 GuavaCacheManager 的 Bean。

*3、*切换 RedisCache

与 Guava 一样，只需在 pom 加入依赖即可：

```
<!-- cache 依赖 -->
<dependency>         
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-redis</artifactId>
</dependency>
12345
```

SpringBoot 会自动配置 RedisCacheManager 以及 RedisTemplate 的 Bean。

此外，切换其他缓存技术的方式也是类似。这里不做赘述。

#### 后语

以上为 SpringBoot 数据缓存的教程。

如果本文对你哪怕有一丁点帮助，请帮忙点好看。

你的好看是我坚持写作的动力。

另外，关注之后在发送 **1024** 可领取免费学习资料。资料内容详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)







# (十二) | 整合 thymeleaf

2019年02月25日 23:00:21

 

一个优秀的废人

 

阅读数：22

更多

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/87926882

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

如题，今天介绍 Thymeleaf ，并整合 Thymeleaf 开发一个简陋版的学生信息管理系统。

SpringBoot 提供了大量模板引擎，包含 Freemarker、Groovy、Thymeleaf、Velocity 以及 Mustache，SpringBoot 中推荐使用 Thymeleaf 作为模板引擎，因为 Thymeleaf 提供了完美的 SpringMVC 支持。Thymeleaf 是新一代 Java 模板引擎，在 Spring 4 后推荐使用。

#### 什么是模板引擎？

Thymeleaf 是一种模板语言。那模板语言或模板引擎是什么？常见的模板语言都包含以下几个概念：数据（Data）、模板（Template）、模板引擎（Template Engine）和结果文档（Result Documents）。

- 数据
  数据是信息的表现形式和载体，可以是符号、文字、数字、语音、图像、视频等。数据和信息是不可分离的，数据是信息的表达，信息是数据的内涵。数据本身没有意义，数据只有对实体行为产生影响时才成为信息。
- 模板
  模板，是一个蓝图，即一个与类型无关的类。编译器在使用模板时，会根据模板实参对模板进行实例化，得到一个与类型相关的类。
- 模板引擎
  模板引擎（这里特指用于Web开发的模板引擎）是为了使用户界面与业务数据（内容）分离而产生的，它可以生成特定格式的文档，用于网站的模板引擎就会生成一个标准的HTML文档。
- 结果文档
  一种特定格式的文档，比如用于网站的模板引擎就会生成一个标准的HTML文档。

模板语言用途广泛，常见的用途如下：

- 页面渲染
- 文档生成
- 代码生成
- 所有 “数据+模板=文本” 的应用场景

#### Thymeleaf 简介

Thymeleaf 是一个 Java 类库，它是一个 xml/xhtml/html5 的模板引擎，可以作为 MVC 的 web 应用的 View 层。

Thymeleaf 还提供了额外的模块与 SpringMVC 集成，所以我们可以使用 Thymeleaf 完全替代 JSP 。

#### Thymeleaf 语法

博客资料：<http://www.cnblogs.com/nuoyiamy/p/5591559.html>
官方文档：<http://www.thymeleaf.org/documentation.html>

#### SpringBoot 整合 Thymeleaf

下面使用 SpringBoot 整合 Thymeleaf 开发一个简陋版的学生信息管理系统。

1、准备工作

- IDEA
- JDK1.8
- SpringBoot2.1.3

2、pom.xml 主要依赖

```xml
<dependencies>
        <!-- JPA 数据访问 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-jpa</artifactId>
        </dependency>
        <!-- thymeleaf 模板引擎 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-thymeleaf</artifactId>
        </dependency>
        <!-- web 启动类 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <!-- mysql 数据库连接类 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>
</dependencies>
1234567891011121314151617181920212223
```

3、application.yaml 文件配置

```yml
spring:
  # 数据库相关
  datasource:
    driver-class-name: com.mysql.cj.jdbc.Driver
    url: jdbc:mysql://127.0.0.1:3306/test?useUnicode=true&characterEncoding=utf8&serverTimezone=UTC&useSSL=true
    username: root
    password: 123456
  # jpa 相关
  jpa:
    hibernate:
      ddl-auto: update   # ddl-auto: 第一次启动项目设为 create 表示每次都重新建表，之后设置为 update
    show-sql: true
123456789101112
```

4、实体类

```java
@Data
@Entity
@AllArgsConstructor
@NoArgsConstructor
public class Student {

    @Id
    @GeneratedValue
    /**
     * 主键
     */
    private Long id;

    /**
     * 主键
     */
    private Long studentId;

    /**
     * 姓名
     */
    private String name;

    /**
     * 年龄
     */
    private Integer age;

    /**
     * 专业
     */
    private String major;

    /**
     * 宿舍
     */
    private String dormitory;

    /**
     * 籍贯
     */
    private String city;

    /*@Temporal(TemporalType.TIMESTAMP)//将时间戳，转换成年月日时分秒的日期格式
    @Column(name = "create_time",insertable = false, updatable=false, columnDefinition = "timestamp default current_timestamp comment '注册时间'")
    private Date createDate;

    @Temporal(TemporalType.TIMESTAMP)//将时间戳，转换成年月日时分秒的日期格式
    @Column(name = "update_time",insertable = false, updatable=true, columnDefinition = "timestamp default current_timestamp comment '修改时间'")
    private Date updateDate;*/

}
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152
```

5、dao 层

```java
@Repository
public interface StudentRepository extends JpaRepository<Student, Long> {
}
123
```

6、service 层

```java
public interface StudentService {

    List<Student> findStudentList();

    Student findStudentById(Long id);

    Student saveStudent(Student student);

    Student updateStudent(Student student);

    void deleteStudentById(Long id);

}
12345678910111213
```

实现类：

```java
@Service
public class StudentServiceImpl implements StudentService {

    @Autowired
    private StudentRepository studentRepository;

    /**
     * 查询所有学生信息列表
     * @return
     */
    @Override
    public List<Student> findStudentList() {
        Sort sort = new Sort(Direction.ASC,"id");
        return studentRepository.findAll(sort);
    }

    /**
     * 根据 id 查询单个学生信息
     * @param id
     * @return
     */
    @Override
    public Student findStudentById(Long id) {
        return studentRepository.findById(id).get();
    }

    /**
     * 保存学生信息
     * @param student
     * @return
     */
    @Override
    public Student saveStudent(Student student) {
        return studentRepository.save(student);
    }

    /**
     * 更新学生信息
     * @param student
     * @return
     */
    @Override
    public Student updateStudent(Student student) {
        return studentRepository.save(student);
    }

    /**
     * 根据 id 删除学生信息
     * @param id
     * @return
     */
    @Override
    public void deleteStudentById(Long id) {
        studentRepository.deleteById(id);
    }
}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556
```

7、controller 层 (Thymeleaf) 使用

controller 层将 view 指向 Thymeleaf：

```java
@Controller
@RequestMapping("/student")
public class StudentController {

    @Autowired
    private StudentService studentService;

    /**
     * 获取学生信息列表
     * @param map
     * @return
     */
    @GetMapping("/list")
    public String findStudentList(ModelMap map) {
        map.addAttribute("studentList",studentService.findStudentList());
        return "studentList";
    }

    /**
     * 获取保存 student 表单
     */
    @GetMapping(value = "/create")
    public String createStudentForm(ModelMap map) {
        map.addAttribute("student", new Student());
        map.addAttribute("action", "create");
        return "studentForm";
    }

    /**
     * 保存学生信息
     * @param student
     * @return
     */
    @PostMapping(value = "/create")
    public String saveStudent(@ModelAttribute Student student) {
        studentService.saveStudent(student);
        return "redirect:/student/list";
    }

    /**
     * 根据 id 获取 student 表单，编辑后提交更新
     * @param id
     * @param map
     * @return
     */
    @GetMapping(value = "/update/{id}")
    public String edit(@PathVariable Long id, ModelMap map) {
        map.addAttribute("student", studentService.findStudentById(id));
        map.addAttribute("action", "update");
        return "studentForm";
    }

    /**
     * 更新学生信息
     * @param student
     * @return
     */
    @PostMapping(value = "/update")
    public String updateStudent(@ModelAttribute Student student) {
        studentService.updateStudent(student);
        return "redirect:/student/list";
    }

    /**
     * 删除学生信息
     * @param id
     * @return
     */
    @GetMapping(value = "/delete/{id}")
    public String deleteStudentById(@PathVariable Long id) {
        studentService.deleteStudentById(id);
        return "redirect:/student/list";
    }
}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768697071727374
```

简单说下，ModelMap 对象来进行数据绑定到视图。return 字符串，该字符串对应的目录在 resources/templates 下的模板名字。 @ModelAttribute 注解是用来获取页面 Form 表单提交的数据，并绑定到 Student 数据对象。

8、studentForm 表单

定义了一个 Form 表单用于注册或修改学生信息。

```html
<form th:action="@{/student/{action}(action=${action})}" method="post" class="form-horizontal">

        <div class="form-group">
            <label for="student_Id" class="col-sm-2 control-label">学号:</label>
            <div class="col-xs-4">
                <input type="text" class="form-control" id="student_Id" name="name" th:value="${student.studentId}"
                       th:field="*{student.studentId}"/>
            </div>
        </div>

        <div class="form-group">
            <label for="student_name" class="col-sm-2 control-label">姓名:</label>
            <div class="col-xs-4">
                <input type="text" class="form-control" id="student_name" name="name" th:value="${student.name}"
                       th:field="*{student.name}"/>
            </div>
        </div>

        <div class="form-group">
            <label for="student_age" class="col-sm-2 control-label">年龄:</label>
            <div class="col-xs-4">
                <input type="text" class="form-control" id="student_age" name="name" th:value="${student.age}"
                       th:field="*{student.age}"/>
            </div>
        </div>

        <div class="form-group">
            <label for="student_major" class="col-sm-2 control-label">专业:</label>
            <div class="col-xs-4">
                <input type="text" class="form-control" id="student_major" name="name" th:value="${student.major}"
                       th:field="*{student.major}"/>
            </div>
        </div>

        <div class="form-group">
            <label for="student_dormitory" class="col-sm-2 control-label">宿舍:</label>
            <div class="col-xs-4">
                <input type="text" class="form-control" id="student_dormitory" name="name" th:value="${student.dormitory}"
                       th:field="*{student.dormitory}"/>
            </div>
        </div>

        <div class="form-group">
            <label for="student_city" class="col-sm-2 control-label">籍贯:</label>
            <div class="col-xs-4">
                <input type="text" class="form-control" id="student_city" name="writer" th:value="${student.city}"
                       th:field="*{student.city}"/>
            </div>
        </div>

        <div class="form-group">
            <div class="col-sm-offset-3 col-sm-10">
                <input class="btn btn-primary" type="submit" value="提交"/>&nbsp;&nbsp;
                <input class="btn" type="button" value="返回" onclick="history.back()"/>
            </div>
        </div>
    </form>
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657
```

9、studentList 学生列表

用于展示学生信息：

```
<table class="table table-hover table-condensed">
        <legend>
            <strong>学生信息列表</strong>
        </legend>
        <thead>
        <tr>
            <th>学号</th>
            <th>姓名</th>
            <th>年龄</th>
            <th>专业</th>
            <th>宿舍</th>
            <th>籍贯</th>
            <th>管理</th>
        </tr>
        </thead>
        <tbody>
        <tr th:each="student : ${studentList}">
            <th scope="row" th:text="${student.studentId}"></th>
            <td><a th:href="@{/student/update/{studentId}(studentId=${student.id})}" th:text="${student.name}"></a></td>
            <td th:text="${student.age}"></td>
            <td th:text="${student.major}"></td>
            <td th:text="${student.dormitory}"></td>
            <td th:text="${student.city}"></td>
            <td><a class="btn btn-danger" th:href="@{/student/delete/{studentId}(studentId=${student.id})}">删除</a></td>
        </tr>
        </tbody>
    </table>
123456789101112131415161718192021222324252627
```

#### 页面效果

列表页面：点击按钮可注册学生信息

![初始页面](https://upload-images.jianshu.io/upload_images/3282134-83e6744d643f26a8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

注册/修改学生信息页面：点提交保存学生信息到数据库并返回列表页面

![注册/修改学生信息页面](https://upload-images.jianshu.io/upload_images/3282134-7145b52d243444af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

有数据的列表页面：点击名字跳到注册/修改页面可修改学生信息，点击删除可删除学生信息

![列表页面](https://upload-images.jianshu.io/upload_images/3282134-85e8092975e81b62.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### 后语

如果本文对你哪怕有一丁点帮助，请帮忙点好看。你的好看是我坚持写作的动力。

另外，关注之后在发送 **1024** 可领取免费学习资料。资料内容详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)







# (十三) | 整合 MyBatis (XML 版)

2019年03月01日 00:21:54

 

一个优秀的废人

 

阅读数：16

 

标签： [SpringBoot](http://so.csdn.net/so/search/s.do?q=SpringBoot&t=blog)[Mybatis](http://so.csdn.net/so/search/s.do?q=Mybatis&t=blog)[Mybatis-Generator](http://so.csdn.net/so/search/s.do?q=Mybatis-Generator&t=blog) 更多

个人分类： [SpringBoot](https://blog.csdn.net/turodog/article/category/8585519)

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/88047817

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

## 前言

如题，今天介绍 SpringBoot 与 Mybatis 的整合以及 Mybatis 的使用，之前介绍过了 SpringBoot 整合MyBatis 注解版的使用，上一篇介绍过 MyBatis 的理论，今天这篇就不介绍 MyBatis 的理论了，有兴趣的跳转阅读：[SpringBoot 实战 (十三) | 整合 MyBatis (注解版)](https://www.jianshu.com/p/6050a6614af2)

## 准备工作

- SpringBoot 2.1.3
- IDEA
- JDK 8

## 创建表

```sql
CREATE TABLE `student`  (
  `id` int(32) NOT NULL AUTO_INCREMENT,
  `student_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '学号',
  `name` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '姓名',
  `age` int(11) NULL DEFAULT NULL COMMENT '年龄',
  `city` varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '所在城市',
  `dormitory` varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '宿舍',
  `major` varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '专业',
  PRIMARY KEY (`id`) USING BTREE
)ENGINE=INNODB AUTO_INCREMENT=1000 DEFAULT CHARSET=utf8;
12345678910
```

## 引入依赖

```xml
<dependencies>
        <!-- jdbc 连接驱动 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-jdbc</artifactId>
        </dependency>
        <!-- web 启动类 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <!-- mybatis 依赖 -->
        <dependency>
            <groupId>org.mybatis.spring.boot</groupId>
            <artifactId>mybatis-spring-boot-starter</artifactId>
            <version>2.0.0</version>
        </dependency>
        <!-- druid 数据库连接池 -->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>druid</artifactId>
            <version>1.1.14</version>
        </dependency>
        <!-- Mysql 连接类 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>5.1.47</version>
            <scope>runtime</scope>
        </dependency>
        <!-- 分页插件 -->
        <dependency>
            <groupId>com.github.pagehelper</groupId>
            <artifactId>pagehelper-spring-boot-starter</artifactId>
            <version>1.2.5</version>
        </dependency>
        <!-- test 依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <!-- springboot maven 插件 -->
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
            <!-- mybatis generator 自动生成代码插件 -->
            <plugin>
                <groupId>org.mybatis.generator</groupId>
                <artifactId>mybatis-generator-maven-plugin</artifactId>
                <version>1.3.2</version>
                <configuration>
                    <configurationFile>${basedir}/src/main/resources/generator/generatorConfig.xml</configurationFile>
                    <overwrite>true</overwrite>
                    <verbose>true</verbose>
                </configuration>
            </plugin>
        </plugins>
    </build>
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364
```

代码解释很详细了，但这里提一嘴，mybatis generator 插件用于自动生成代码，pagehelper 插件用于物理分页。

## 项目配置

```yaml
server:
  port: 8080

spring:
    datasource:
        name: test
        url: jdbc:mysql://127.0.0.1:3306/test
        username: root
        password: 123456
        #druid相关配置
        type: com.alibaba.druid.pool.DruidDataSource
        driver-class-name: com.mysql.jdbc.Driver
        filters: stat
        maxActive: 20
        initialSize: 1
        maxWait: 60000
        minIdle: 1
        timeBetweenEvictionRunsMillis: 60000
        minEvictableIdleTimeMillis: 300000
        validationQuery: select 'x'
        testWhileIdle: true
        testOnBorrow: false
        testOnReturn: false
        poolPreparedStatements: true
        maxOpenPreparedStatements: 20

## 该配置节点为独立的节点，有很多同学容易将这个配置放在spring的节点下，导致配置无法被识别
mybatis:
  mapper-locations: classpath:mapping/*.xml  #注意：一定要对应mapper映射xml文件的所在路径
  type-aliases-package: com.nasus.mybatisxml.model  # 注意：对应实体类的路径

#pagehelper分页插件
pagehelper:
    helperDialect: mysql
    reasonable: true
    supportMethodsArguments: true
    params: count=countSql
12345678910111213141516171819202122232425262728293031323334353637
```

## mybatis generator 配置文件

这里要注意，配置 pom.xml 中 generator 插件所对应的配置文件时，在 Pom.xml 加入这一句，说明 generator 插件所对应的配置文件所对应的配置文件路径。这里已经在 Pom 中配置了，请见上面的 Pom 配置。

```xml
${basedir}/src/main/resources/generator/generatorConfig.xml
1
```

generatorConfig.xml ：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE generatorConfiguration
        PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"
        "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">
<generatorConfiguration>
    <!-- 数据库驱动:选择你的本地硬盘上面的数据库驱动包-->
    <classPathEntry  location="D:\repository\mysql\mysql-connector-java\5.1.47\mysql-connector-java-5.1.47.jar"/>
    <context id="DB2Tables"  targetRuntime="MyBatis3">
        <commentGenerator>
            <property name="suppressDate" value="true"/>
            <!-- 是否去除自动生成的注释 true：是 ： false:否 -->
            <property name="suppressAllComments" value="true"/>
        </commentGenerator>
        <!--数据库链接URL，用户名、密码 -->
        <jdbcConnection driverClass="com.mysql.jdbc.Driver" connectionURL="jdbc:mysql://127.0.0.1/test" userId="root" password="123456">
        </jdbcConnection>
        <javaTypeResolver>
            <property name="forceBigDecimals" value="false"/>
        </javaTypeResolver>
        <!-- 生成模型的包名和位置-->
        <javaModelGenerator targetPackage="com.nasus.mybatisxml.model" targetProject="src/main/java">
            <property name="enableSubPackages" value="true"/>
            <property name="trimStrings" value="true"/>
        </javaModelGenerator>
        <!-- 生成映射文件的包名和位置-->
        <sqlMapGenerator targetPackage="mapping" targetProject="src/main/resources">
            <property name="enableSubPackages" value="true"/>
        </sqlMapGenerator>
        <!-- 生成DAO的包名和位置-->
        <javaClientGenerator type="XMLMAPPER" targetPackage="com.nasus.mybatisxml.mapper" targetProject="src/main/java">
            <property name="enableSubPackages" value="true"/>
        </javaClientGenerator>
        <!-- 要生成的表 tableName是数据库中的表名或视图名 domainObjectName是实体类名-->
        <table tableName="student" domainObjectName="Student" enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false" selectByExampleQueryId="false"></table>
    </context>
</generatorConfiguration>
123456789101112131415161718192021222324252627282930313233343536
```

代码注释很详细，不多说。

## 生成代码过程

第一步：选择编辑配置

![添加配置](https://upload-images.jianshu.io/upload_images/3282134-5a511f571b701528.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

第二步：选择添加 Maven 配置

![选择 Maven](https://upload-images.jianshu.io/upload_images/3282134-87e7d49139c991d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

第三步：添加命令 mybatis-generator:generate -e 点击确定

![添加命令](https://upload-images.jianshu.io/upload_images/3282134-75f9c3f506a9d434.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

第四步：运行该配置，生成代码

**特别注意！！！同一张表一定不要运行多次，因为 mapper 的映射文件中会生成多次的代码，导致报错，切记。如要运行多次，请把上次生成的 mapper 映射文件代码删除再运行。**

![运行该配置](https://upload-images.jianshu.io/upload_images/3282134-ef075134ab35d445.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

第五步：检查生成结果

![生成的代码](https://upload-images.jianshu.io/upload_images/3282134-48178df707391593.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 遇到的问题

请参照别人写好的遇到问题的解决方法，其中我就遇到**数据库时区不对**以及**只生成 Insert 方法**这两个问题。都是看以下这篇文章解决的：

[Mybatis Generator自动生成代码以及可能出现的问题](https://blog.csdn.net/exalgentle/article/details/80844294)

## 生成的代码

1、实体类：Student.java

```java
package com.nasus.mybatisxml.model;

public class Student {

    private Long id;

    private Integer age;

    private String city;

    private String dormitory;

    private String major;

    private String name;

    private Long studentId;

   // 省略 get 和 set 方法
}
1234567891011121314151617181920
```

2、mapper 接口：StudentMapper.java

```java
package com.nasus.mybatisxml.mapper;

import com.nasus.mybatisxml.model.Student;
import java.util.List;
import org.apache.ibatis.annotations.Mapper;

@Mapper
public interface StudentMapper {
    int deleteByPrimaryKey(Long id);

    int insert(Student record);

    int insertSelective(Student record);

    Student selectByPrimaryKey(Long id);

    // 我添加的方法，相应的要在映射文件中添加此方法
    List<Student> selectStudents();

    int updateByPrimaryKeySelective(Student record);

    int updateByPrimaryKey(Student record);
}
1234567891011121314151617181920212223
```

3、映射文件：StudentMapper.xml

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
<mapper namespace="com.nasus.mybatisxml.mapper.StudentMapper" >
  <resultMap id="BaseResultMap" type="com.nasus.mybatisxml.model.Student" >
    <id column="id" property="id" jdbcType="BIGINT" />
    <result column="age" property="age" jdbcType="INTEGER" />
    <result column="city" property="city" jdbcType="VARCHAR" />
    <result column="dormitory" property="dormitory" jdbcType="VARCHAR" />
    <result column="major" property="major" jdbcType="VARCHAR" />
    <result column="name" property="name" jdbcType="VARCHAR" />
    <result column="student_id" property="studentId" jdbcType="BIGINT" />
  </resultMap>

  <sql id="Base_Column_List" >
    id, age, city, dormitory, major, name, student_id
  </sql>

  <select id="selectByPrimaryKey" resultMap="BaseResultMap" parameterType="java.lang.Long" >
    select 
    <include refid="Base_Column_List" />
    from student
    where id = #{id,jdbcType=BIGINT}
  </select>

  <delete id="deleteByPrimaryKey" parameterType="java.lang.Long" >
    delete from student
    where id = #{id,jdbcType=BIGINT}
  </delete>

  <insert id="insert" parameterType="com.nasus.mybatisxml.model.Student" >
    insert into student (id, age, city, 
      dormitory, major, name, 
      student_id)
    values (#{id,jdbcType=BIGINT}, #{age,jdbcType=INTEGER}, #{city,jdbcType=VARCHAR}, 
      #{dormitory,jdbcType=VARCHAR}, #{major,jdbcType=VARCHAR}, #{name,jdbcType=VARCHAR}, 
      #{studentId,jdbcType=BIGINT})
  </insert>

  <insert id="insertSelective" parameterType="com.nasus.mybatisxml.model.Student" >
    insert into student
    <trim prefix="(" suffix=")" suffixOverrides="," >
      <if test="id != null" >
        id,
      </if>
      <if test="age != null" >
        age,
      </if>
      <if test="city != null" >
        city,
      </if>
      <if test="dormitory != null" >
        dormitory,
      </if>
      <if test="major != null" >
        major,
      </if>
      <if test="name != null" >
        name,
      </if>
      <if test="studentId != null" >
        student_id,
      </if>
    </trim>
    <trim prefix="values (" suffix=")" suffixOverrides="," >
      <if test="id != null" >
        #{id,jdbcType=BIGINT},
      </if>
      <if test="age != null" >
        #{age,jdbcType=INTEGER},
      </if>
      <if test="city != null" >
        #{city,jdbcType=VARCHAR},
      </if>
      <if test="dormitory != null" >
        #{dormitory,jdbcType=VARCHAR},
      </if>
      <if test="major != null" >
        #{major,jdbcType=VARCHAR},
      </if>
      <if test="name != null" >
        #{name,jdbcType=VARCHAR},
      </if>
      <if test="studentId != null" >
        #{studentId,jdbcType=BIGINT},
      </if>
    </trim>
  </insert>

  <update id="updateByPrimaryKeySelective" parameterType="com.nasus.mybatisxml.model.Student" >
    update student
    <set >
      <if test="age != null" >
        age = #{age,jdbcType=INTEGER},
      </if>
      <if test="city != null" >
        city = #{city,jdbcType=VARCHAR},
      </if>
      <if test="dormitory != null" >
        dormitory = #{dormitory,jdbcType=VARCHAR},
      </if>
      <if test="major != null" >
        major = #{major,jdbcType=VARCHAR},
      </if>
      <if test="name != null" >
        name = #{name,jdbcType=VARCHAR},
      </if>
      <if test="studentId != null" >
        student_id = #{studentId,jdbcType=BIGINT},
      </if>
    </set>
    where id = #{id,jdbcType=BIGINT}
  </update>

  <update id="updateByPrimaryKey" parameterType="com.nasus.mybatisxml.model.Student" >
    update student
    set age = #{age,jdbcType=INTEGER},
      city = #{city,jdbcType=VARCHAR},
      dormitory = #{dormitory,jdbcType=VARCHAR},
      major = #{major,jdbcType=VARCHAR},
      name = #{name,jdbcType=VARCHAR},
      student_id = #{studentId,jdbcType=BIGINT}
    where id = #{id,jdbcType=BIGINT}
  </update>

  <!-- 我添加的方法 -->
  <select id="selectStudents" resultMap="BaseResultMap">
    SELECT
    <include refid="Base_Column_List" />
    from student
  </select>
</mapper>
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131
```

## serviec 层

1、接口：

```java
public interface StudentService {

    int addStudent(Student student);

    Student findStudentById(Long id);

    PageInfo<Student> findAllStudent(int pageNum, int pageSize);

}
123456789
```

2、实现类

```java
@Service
public class StudentServiceImpl implements StudentService{

    //会报错，不影响
    @Resource
    private StudentMapper studentMapper;

    /**
     * 添加学生信息
     * @param student
     * @return
     */
    @Override
    public int addStudent(Student student) {
        return studentMapper.insert(student);
    }

    /**
     * 根据 id 查询学生信息
     * @param id
     * @return
     */
    @Override
    public Student findStudentById(Long id) {
        return studentMapper.selectByPrimaryKey(id);
    }

    /**
     * 查询所有学生信息并分页
     * @param pageNum
     * @param pageSize
     * @return
     */
    @Override
    public PageInfo<Student> findAllStudent(int pageNum, int pageSize) {
        //将参数传给这个方法就可以实现物理分页了，非常简单。
        PageHelper.startPage(pageNum, pageSize);
        List<Student> studentList = studentMapper.selectStudents();
        PageInfo result = new PageInfo(studentList);
        return result;
    }
}
123456789101112131415161718192021222324252627282930313233343536373839404142
```

## controller 层

```java
@RestController
@RequestMapping("/student")
public class StudentController {

    @Autowired
    private StudentService studentService;

    @GetMapping("/{id}")
    public Student findStidentById(@PathVariable("id") Long id){
        return studentService.findStudentById(id);
    }

    @PostMapping("/add")
    public int insertStudent(@RequestBody Student student){
        return studentService.addStudent(student);
    }

    @GetMapping("/list")
    public PageInfo<Student> findStudentList(@RequestParam(name = "pageNum", required = false, defaultValue = "1") int pageNum,
            @RequestParam(name = "pageSize", required = false, defaultValue = "10") int pageSize){
        return studentService.findAllStudent(pageNum,pageSize);
    }
}
1234567891011121314151617181920212223
```

## 启动类

```java
@SpringBootApplication
@MapperScan("com.nasus.mybatisxml.mapper") // 扫描 mapper 接口，必须加上
public class MybatisxmlApplication {

    public static void main(String[] args) {
        SpringApplication.run(MybatisxmlApplication.class, args);
    }
}
12345678
```

提一嘴，@MapperScan(“com.nasus.mybatisxml.mappe”) 这个注解非常的关键，这个对应了项目中 mapper(dao) 所对应的包路径，必须加上，否则会导致异常。

## Postman 测试

1、插入方法：

![插入方法结果](https://upload-images.jianshu.io/upload_images/3282134-8a8734491812c28b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

2、根据 id 查询方法：

![根据 id 查询结果](https://upload-images.jianshu.io/upload_images/3282134-a9961739f4898d51.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

3、分页查询方法：

![分页查询结果](https://upload-images.jianshu.io/upload_images/3282134-cb8ac5353ca9735b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 源码下载

<https://github.com/turoDog/Demo/tree/master/springboot_mybatisxml_demo>

帮忙点个 star 可好？

## 后语

如果本文对你哪怕有一丁点帮助，请帮忙点好看。你的好看是我坚持写作的动力。

另外，关注之后在发送 **1024** 可领取免费学习资料。资料内容详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)







# (十四) | 统一处理异常

2019年03月03日 00:51:04

 

一个优秀的废人

 

阅读数：12

更多

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/88083963

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

## 前言

如题，今天介绍 SpringBoot 是如何统一处理全局异常的。SpringBoot 中的全局异常处理主要起作用的两个注解是 **@ControllerAdvice** 和 **@ExceptionHandler** ，其中 @ControllerAdvice 是组件注解，添加了这个注解的类能够拦截 Controller 的请求，而 ExceptionHandler 注解可以设置全局处理控制里的异常类型来拦截要处理的异常。 比如：@ExceptionHandler(value = NullPointException.class) 。

## 准备工作

- SpringBoot 2.1.3
- IDEA
- JDK 8

## 依赖配置

```xml
<dependencies>
        <!-- JPA 依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-jpa</artifactId>
        </dependency>
        <!-- web 依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <!-- mysql 连接类 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>
        <!-- lombok 依赖 -->
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <optional>true</optional>
        </dependency>
        <!-- 单元测试依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>
12345678910111213141516171819202122232425262728293031
```

## 配置文件

```yaml
spring:
  # 数据库相关
  datasource:
    driver-class-name: com.mysql.jdbc.Driver
    url: jdbc:mysql://127.0.0.1:3306/test?useUnicode=true&characterEncoding=utf8&serverTimezone=UTC&useSSL=true
    username: root
    password: 123456

  jpa:
    hibernate:
      ddl-auto: update   #ddl-auto:设为 create 表示每次都重新建表
    show-sql: true
123456789101112
```

## 返回的消息类

```java
public class Message<T> implements Serializable {

    /**
     * 状态码
     */
    private Integer code;

    /**
     * 返回信息
     */
    private String message;

    /**
     * 返回的数据类
     */
    private T data;

    /**
     * 时间
     */
    private Long time;

    // getter、setter 以及 构造方法略。。。
}
123456789101112131415161718192021222324
```

## 工具类

用于处理返回的数据以及信息类，代码注释很详细不说了。

```java
public class MessageUtil {

    /**
     * 成功并返回数据实体类
     * @param o
     * @param <E>
     * @return
     */
    public static <E>Message<E> ok(E o){
        return new Message<>(200, "success", o, new Date().getTime());
    }

    /**
     * 成功，但无数据实体类返回
     * @return
     */
    public static <E>Message<E> ok(){
        return new Message<>(200, "success", null, new Date().getTime());
    }

    /**
     * 失败，有自定义异常返回
     * @param code
     * @param msg
     * @return
     */
    public static <E>Message<E> error(Integer code,String msg){
        return new Message<>(code, msg, null, new Date().getTime());
    }
}
123456789101112131415161718192021222324252627282930
```

## 自定义异常

通过继承 RuntimeException ，声明 code 用于定义不同类型的自定义异常。主要是用于异常拦截出获取 code 并将 code 设置到消息类中返回。

```java
public class CustomException extends RuntimeException{

    /**
     * 状态码
     */
    private Integer code;

    public Integer getCode() {
        return code;
    }

    public void setCode(Integer code) {
        this.code = code;
    }

    public CustomException(Integer code, String message){

        super(message);
        this.code = code;

    }
}
12345678910111213141516171819202122
```

## 异常拦截类

通过加入 @RestControllerAdvice 来声明该类可拦截 Controller 请求，同时在 handle方法加入 @ExceptionHandler 并在该注解中指定要拦截的异常类。

```java
@RestControllerAdvice // 控制器增强处理(返回 JSON 格式数据)，添加了这个注解的类能被 classpath 扫描自动发现
public class ExceptionHandle {

    @ExceptionHandler(value = Exception.class) // 捕获 Controller 中抛出的指定类型的异常，也可以指定其他异常
    public <E>Message<E> handler(Exception exception){

        if (exception instanceof CustomException){
            CustomException customException = (CustomException) exception;
            return MessageUtil.error(customException.getCode(), customException.getMessage());
        } else {
            return MessageUtil.error(120, "异常信息：" + exception.getMessage());
        }
    }
}
1234567891011121314
```

这里只对自定义异常以及未知异常进行处理，如果你在某方法中明确知道可能会抛出某个异常，可以加多一个特定的处理。比如说你明确知道该方法可能抛出 NullPointException 可以追加 NullPointException 的处理：

```java
if (exception instanceof CustomException){
     CustomException customException = (CustomException) exception;
     return MessageUtil.error(customException.getCode(), customException.getMessage());
} else if (exception instanceof NullPointException ){
     return MessageUtil.error(500, "空指针异常信！");
} else {
     return MessageUtil.error(120, "异常信息：" + exception.getMessage());
}
12345678
```

## controller 层

```java
@RestController
@RequestMapping("/student")
public class StudentController {

    @Autowired
    private StudentService studentService;

    @GetMapping("/{id}")
    public Message<Student> findStudentById(@PathVariable("id") Integer id){

        if (id < 0){
            //测试自定义错误
            throw new CustomException(110, "参数不能是负数！");

        } else if (id == 0){
            //硬编码，为了测试
            Integer i = 1/id;
            return null;
        } else {
            Student student = studentService.findStudentById(id);
            return MessageUtil.ok(student);
        }
    }
}
123456789101112131415161718192021222324
```

## 完整代码

<https://github.com/turoDog/Demo/tree/master/springboot_exception_demo>

如果觉得对你有帮助，请给个 Star 再走呗，非常感谢。

## Postman 测试

访问 <http://localhost:8080/student/5> 测试正常返回数据结果。

![img](https://upload-images.jianshu.io/upload_images/3282134-ae960c4fbd2ca3ab.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

访问 <http://localhost:8080/student/0> 测试未知异常的结果。

![img](https://upload-images.jianshu.io/upload_images/3282134-d2f9f7757027f50a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

访问 <http://localhost:8080/student/-11> 测试自定义异常的结果。

![img](https://upload-images.jianshu.io/upload_images/3282134-bc6e163538230f2b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 后语

如果本文对你哪怕有一丁点帮助，请帮忙点好看。你的好看是我坚持写作的动力。

另外，关注之后在发送 **1024** 可领取免费学习资料。资料内容详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)







# (十五) | 服务端参数校验之一

2019年03月03日 22:25:24

 

一个优秀的废人

 

阅读数：12

更多

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/88095340

## 前言

估计很多朋友都认为参数校验是客户端的职责，不关服务端的事。其实这是错误的，学过 Web 安全的都知道，客户端的验证只是第一道关卡。它的参数验证并不是安全的，一旦被有心人抓到可乘之机，他就可以有各种方法来摸拟系统的 Http 请求，访问数据库的关键数据。轻则导致服务器宕机，重则泄露数据。所以，这时就需要设置第二道关卡，服务端验证了。

## 老项目的服务端校验

```java
@RestController
@RequestMapping("/student")
public class ValidateOneController {

    @GetMapping("/id")
    public Student findStudentById(Integer id){
        if(id == null){
              logger.error("id 不能为空！")；
              throw new NullPointerException("id 不能为空");
        }
        return studentService.findStudentById(id);
    }
}
12345678910111213
```

看以上代码，就一个的校验就如此麻烦。那我们是否有好的统一校验方法呢？鉴于 SpringBoot 无所不能。答案当然是有的。

其中，Bean Validator 和 Hibernate Validator 就是两套用于验证的框架，二者都遵循 JSR-303 ，可以混着用，鉴于二者的某些 Validator 注解有差别，例如 @Length 在 Bean Validator 中是没有的，所以这里我选择混合用。

## JSR-303

JSR-303 是JAVA EE 6 中的一项子规范，叫做 Bean Validation，Hibernate Validator 是 Bean Validation 的参考实现， Hibernate Validator 提供了 JSR 303 规范中所有内置 Constraint(约束) 的实现，除此之外还有一些附加的 Constraint 。这些 Constraint (约束) 全都通过注解的方式实现，请看下面两个表。

Bean Validation 中内置的约束：

| 注解                       | 作用                                                         |
| -------------------------- | ------------------------------------------------------------ |
| @Null                      | 被注解参数必须为空                                           |
| @NotNull                   | 被注解参数不能为空                                           |
| @AssertTrue                | 被注解参数必须为 True                                        |
| @AssertFalse               | 被注解参数必须为 False                                       |
| @Min(value)                | 被注解参数必须是数字，且其值必须大于等于 value               |
| @Max(value)                | 被注解参数必须是数字，且其值必须小于等于 value               |
| @DecimaMin(value)          | 被注解参数必须是数字，且其值必须大于等于 value               |
| @DecimaMax(value)          | 被注解参数必须是数字，且其值必须小于等于 value               |
| @Size(max, min)            | 被注解参数大小必须在指定范围内                               |
| @Past                      | 被注解参数必须是一个过去的日期                               |
| @Future                    | 被注解参数必须是一个将来的日期                               |
| @Pattern(value)            | 被注解参数必须符合指定的正则表达式                           |
| @Digits(integer, fraction) | 被注解参数必须是数字，且其值必须在可接受范围内               |
| @NotBlank                  | 被注解参数的值不为空（不为 null、去除首位空格后长度为 0），不同于 @NotEmpty，@NotBlank 只应用于字符串且在比较时会去除字符串的空格 |

Hibernate Validator 附加的约束：

| 注解      | 作用                                                         |
| --------- | ------------------------------------------------------------ |
| @NotEmpty | 被注解参数的值不为 null 且不为空（字符串长度不为0、集合大小不为0） |
| @Email    | 被注解参数必须是电子邮箱地址                                 |
| @Length   | 被注解的字符串长度必须在指定范围内                           |
| @Range    | 被注解的参数必须在指定范围内                                 |

## 准备工作

- SpringBoot 2.1.3
- IDEA
- JDK8

## Pom 文件依赖

```xml
<!-- web 启动类 -->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
<!-- test 单元测试类 -->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-test</artifactId>
    <scope>test</scope>
</dependency>
<!-- lombok 依赖用于简化 bean -->
<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <optional>true</optional>
</dependency>
1234567891011121314151617
```

## 实体类

用于测试，加入了参数校验规则。

```java
@Data
@AllArgsConstructor
@NoArgsConstructor
public class Student {

    private Integer id;

    @NotBlank(message = "学生名字不能为空")
    @Length(min = 2, max = 10, message = "name 长度必须在 {min} - {max} 之间")
    private String name;

    @NotNull(message = "年龄不允许为空")
    @Min(value = 0, message = "年龄不能低于 {value} 岁")
    private Integer age;
}
123456789101112131415
```

## Controller 层

写了两个方法，一个用于校验普通参数，一个用于校验对象

```java
@Validated //开启数据校验，添加在类上用于校验方法，添加在方法参数中用于校验参数对象。(添加在方法上无效)
@RestController
@RequestMapping("/student")
public class ValidateOneController {

    /**
     * 普通参数校验
     * @param name
     * @return
     */
    @GetMapping("/name")
    public String findStudentByName(@NotBlank(message = "学生名字不能为空")
    @Length(min = 2, max = 10, message = "name 长度必须在 {min} - {max} 之间")String name){
        return "success";
    }

    /**
     * 对象校验
     * @param student
     * @return
     */
    @PostMapping("/add")
    public String addStudent(@Validated @RequestBody Student student){
        return "success";
    }
}
1234567891011121314151617181920212223242526
```

## Postman 测试

校验普通参数测试结果：

下图可以看见，我没有在 <http://localhost:8080/student/name> 地址后添加 name 参数，传到后台马上就校验出异常了。而这个异常信息就是我定义的校验异常信息。

![img](https://upload-images.jianshu.io/upload_images/3282134-9ba1e5fda9f8db76.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

校验对象测试结果：

![img](https://upload-images.jianshu.io/upload_images/3282134-b91974dbe9e76eac.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

结果有点长：

下图可以看见，我访问 <http://localhost:8080/student/add> 传入了参数对象，但对象是不能通过校验规则的，比如 age 参数为负数，name 参数长度太大，传到后台马上就校验出异常了。而这个异常信息就是我定义的校验异常信息。

![img](https://upload-images.jianshu.io/upload_images/3282134-86a7fbe0bdce99b5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 完整代码

<https://github.com/turoDog/Demo/tree/master/springboot_validateone_demo>

如果觉得对你有帮助，请给个 Star 再走呗，非常感谢。

## 后语

如果本文对你哪怕有一丁点帮助，请帮忙点好看。你的好看是我坚持写作的动力。

另外，关注之后在发送 **1024** 可领取免费学习资料。

资料详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)





# (十六) | 整合 WebSocket 基于 STOMP 协议实现广播消息

2019年03月05日 00:32:58

 

一个优秀的废人

 

阅读数：11

更多

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/88148944

## 前言

如题，今天介绍的是 SpringBoot 整合 WebSocket 实现广播消息。

## 什么是 WebSocket ？

WebSocket 为浏览器和服务器提供了双工异步通信的功能，即浏览器可以向服务器发送信息，反之也成立。

WebSocket 是通过一个 socket 来实现双工异步通信能力的，但直接使用 WebSocket ( 或者 SockJS：WebSocket 协议的模拟，增加了当前浏览器不支持使用 WebSocket 的兼容支持) 协议开发程序显得十分繁琐，所以使用它的子协议 STOMP。

## STOMP 协议简介

它是高级的流文本定向消息协议，是一种为 MOM (Message Oriented Middleware，面向消息的中间件) 设计的简单文本协议。

它提供了一个可互操作的连接格式，允许 STOMP 客户端与任意 STOMP 消息代理 (Broker) 进行交互，类似于 OpenWire (一种二进制协议)。

由于其设计简单，很容易开发客户端，因此在多种语言和多种平台上得到广泛应用。其中最流行的 STOMP 消息代理是 Apache ActiveMQ。

STOMP 协议使用一个基于 (frame) 的格式来定义消息，与 Http 的 request 和 response 类似 。

## 广播

接下来，实现一个广播消息的 demo。即服务端有消息时，将消息发送给所有连接了当前 endpoint 的浏览器。

## 准备工作

- SpringBoot 2.1.3
- IDEA
- JDK8

## Pom 依赖配置

```xml
<dependencies>
        <!-- thymeleaf 模板引擎 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-thymeleaf</artifactId>
        </dependency>
        <!-- web 启动类 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <!-- WebSocket 依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-websocket</artifactId>
        </dependency>
        <!-- test 单元测试 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>
1234567891011121314151617181920212223
```

代码注释很详细，不多说。

## 配置 WebSocket

实现 WebSocketMessageBrokerConfigurer 接口，注册一个 STOMP 节点，配置一个广播消息代理

```java
@Configuration
// @EnableWebSocketMessageBroker注解用于开启使用STOMP协议来传输基于代理（MessageBroker）的消息，这时候控制器（controller）
// 开始支持@MessageMapping,就像是使用@requestMapping一样。
@EnableWebSocketMessageBroker
public class WebSocketConfig implements WebSocketMessageBrokerConfigurer {

    @Override
    public void registerStompEndpoints(StompEndpointRegistry registry) {
        //注册一个 Stomp 的节点(endpoint),并指定使用 SockJS 协议。
        registry.addEndpoint("/endpointNasus").withSockJS();
    }

    @Override
    public void configureMessageBroker(MessageBrokerRegistry registry) {
        // 广播式配置名为 /nasus 消息代理 , 这个消息代理必须和 controller 中的 @SendTo 配置的地址前缀一样或者全匹配
        registry.enableSimpleBroker("/nasus");
    }
}
123456789101112131415161718
```

## 消息类

客户端发送给服务器：

```java
public class Client2ServerMessage {

    private String name;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
123456789101112
```

服务器发送给客户端：

```java
public class Server2ClientMessage {

    private String responseMessage;

    public Server2ClientMessage(String responseMessage) {
        this.responseMessage = responseMessage;
    }

    public String getResponseMessage() {
        return responseMessage;
    }

    public void setResponseMessage(String responseMessage) {
        this.responseMessage = responseMessage;
    }
}
12345678910111213141516
```

## 演示控制器代码

```java
@RestController
public class WebSocketController {

    @MessageMapping("/hello") // @MessageMapping 和 @RequestMapping 功能类似，浏览器向服务器发起消息，映射到该地址。
    @SendTo("/nasus/getResponse") // 如果服务器接受到了消息，就会对订阅了 @SendTo 括号中的地址的浏览器发送消息。
    public Server2ClientMessage say(Client2ServerMessage message) throws Exception {
        Thread.sleep(3000);
        return new Server2ClientMessage("Hello," + message.getName() + "!");
    }

}
1234567891011
```

## 引入 STOMP 脚本

将 stomp.min.js (STOMP 客户端脚本) 和 sockJS.min.js (sockJS 客户端脚本) 以及 Jquery 放在 resource 文件夹的 static 目录下。

## 演示页面

```html
<!DOCTYPE html>
<html xmlns:th="http://www.thymeleaf.org">
<head>
    <meta charset="UTF-8" />
    <title>Spring Boot+WebSocket+广播式</title>

</head>
<body onload="disconnect()">
<noscript><h2 style="color: #ff0000">貌似你的浏览器不支持websocket</h2></noscript>
<div>
    <div>
        <button id="connect" onclick="connect();">连接</button>
        <button id="disconnect" disabled="disabled" onclick="disconnect();">断开连接</button>
    </div>
    <div id="conversationDiv">
        <label>输入你的名字</label><input type="text" id="name" />
        <button id="sendName" onclick="sendName();">发送</button>
        <p id="response"></p>
    </div>
</div>
<script th:src="@{sockjs.min.js}"></script>
<script th:src="@{stomp.min.js}"></script>
<script th:src="@{jquery.js}"></script>
<script type="text/javascript">
    var stompClient = null;

    function setConnected(connected) {
        document.getElementById('connect').disabled = connected;
        document.getElementById('disconnect').disabled = !connected;
        document.getElementById('conversationDiv').style.visibility = connected ? 'visible' : 'hidden';
        $('#response').html();
    }
	
    function connect() {
        // 连接 SockJs 的 endpoint 名称为 "/endpointNasus"
        var socket = new SockJS('/endpointNasus'); 
        // 使用 STOMP 子协议的 WebSocket 客户端
        stompClient = Stomp.over(socket); 
        stompClient.connect({}, function(frame) {
            setConnected(true);
            console.log('Connected: ' + frame);
            // 通过 stompClient.subscribe 订阅 /nasus/getResponse 目标发送的信息，对应控制器的 SendTo 定义
            stompClient.subscribe('/nasus/getResponse', function(respnose){
            // 展示返回的信息，只要订阅了 /nasus/getResponse 目标，都可以接收到服务端返回的信息
            showResponse(JSON.parse(respnose.body).responseMessage);
            });
        });
    }
	
	
    function disconnect() {
        // 断开连接
        if (stompClient != null) {
            stompClient.disconnect();
        }
        setConnected(false);
        console.log("Disconnected");
    }

    function sendName() {
        // 向服务端发送消息
        var name = $('#name').val();
        // 通过 stompClient.send 向 /hello （服务端）发送信息，对应控制器 @MessageMapping 中的定义
        stompClient.send("/hello", {}, JSON.stringify({ 'name': name }));
    }

    function showResponse(message) {
          // 接收返回的消息
          var response = $("#response");
          response.html(message);
    }
</script>
</body>
</html>
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768697071727374
```

## 页面 Controller

注意，这里使用的是 @Controller 注解，用于匹配 html 前缀，加载页面。

```java
@Controller
public class ViewController {

    @GetMapping("/nasus")
    public String getView(){
        return "nasus";
    }
}
12345678
```

## 测试结果

打开三个窗口访问 <http://localhost:8080/nasus> ，初始页面长这样：

![初始页面](https://upload-images.jianshu.io/upload_images/3282134-b4dad77f5ba3655e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

三个页面全部点连接，点击连接订阅 endpoint ，如下图：

![点击连接订阅 endpoint ](https://upload-images.jianshu.io/upload_images/3282134-99e402e2b8ca8947.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![点击连接订阅 endpoint ](https://upload-images.jianshu.io/upload_images/3282134-99e402e2b8ca8947.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![点击连接订阅 endpoint ](https://upload-images.jianshu.io/upload_images/3282134-99e402e2b8ca8947.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在第一个页面，输入名字，点发送 ，如下图：

![输入名字，点发送](https://upload-images.jianshu.io/upload_images/3282134-09daec1e2db75481.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在第一个页面发送消息，等待 3 秒，结果是 3 个页面都接受到了服务端返回的信息，广播成功。

![第一个页面结果](https://upload-images.jianshu.io/upload_images/3282134-dbe795674a9b4471.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![第二个页面结果](https://upload-images.jianshu.io/upload_images/3282134-892726c20e95f1fd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![第三个页面结果](https://upload-images.jianshu.io/upload_images/3282134-892726c20e95f1fd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

源码下载：

<https://github.com/turoDog/Demo/tree/master/springboot_websocket_demo>

如果觉得对你有帮助，请给个 Star 再走呗，非常感谢。

## 后语

如果本文对你哪怕有一丁点帮助，请帮忙点好看。你的好看是我坚持写作的动力。

另外，关注之后在发送 **1024** 可领取免费学习资料。

资料详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)







# (十七) | 整合 WebSocket 实现聊天室

2019年03月05日 23:14:23

 

一个优秀的废人

 

阅读数：13

更多

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/88211827

> 微信公众号：一个优秀的废人。如有问题，请后台留言，反正我也不会听。

## 前言

昨天那篇介绍了 WebSocket 实现广播，也即服务器端有消息时，将消息发送给所有连接了当前 endpoint 的浏览器。但这无法解决消息由谁发送，又由谁接收的问题。所以，今天写一篇实现一对一的聊天室。

今天这一篇建立在昨天那一篇的基础之上，为便于更好理解今天这一篇，推荐先阅读：「[SpringBoot 整合WebSocket 实现广播消息](https://www.jianshu.com/p/aea406f24b77) 」

## 准备工作

- Spring Boot 2.1.3 RELEASE
- Spring Security 2.1.3 RELEASE
- IDEA
- JDK8

## pom 依赖

因聊天室涉及到用户相关，所以在上一篇基础上引入 Spring Security 2.1.3 RELEASE 依赖

```xml
<!-- Spring Security 依赖 -->
<dependency>
     <groupId>org.springframework.boot</groupId>
     <artifactId>spring-boot-starter-security</artifactId>
</dependency>
12345
```

## Spring Security 的配置

虽说涉及到 Spring Security ，但鉴于篇幅有限，这里只对这个项目相关的部分进行介绍，具体的 Spring Security 教程，后面会出。

这里的 Spring Security 配置很简单，具体就是设置登录路径、设置安全资源以及在内存中创建用户和密码，密码需要注意加密，这里使用 BCrypt 加密算法在用户登录时对密码进行加密。 代码注释很详细，不多说。

```java
package com.nasus.websocket.config;

import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.builders.WebSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;

@Configuration
// 开启Spring Security的功能
@EnableWebSecurity
public class WebSecurityConfig extends WebSecurityConfigurerAdapter{

    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http.authorizeRequests()
             // 设置 SpringSecurity 对 / 和 "/login" 路径不拦截
            .mvcMatchers("/","/login").permitAll()
            .anyRequest().authenticated()
            .and()
            .formLogin()
            // 设置 Spring Security 的登录页面访问路径为/login
            .loginPage("/login")
            // 登录成功后转向 /chat 路径
            .defaultSuccessUrl("/chat")
            .permitAll()
            .and()
            .logout()
            .permitAll();
    }

    @Override
    protected void configure(AuthenticationManagerBuilder auth) throws Exception {
        auth.inMemoryAuthentication()
            // 在内存中分配两个用户 nasus 和 chenzy ，用户名和密码一致
            // BCryptPasswordEncoder() 是 Spring security 5.0 中新增的加密方式
            // 登陆时用 BCrypt 加密方式对用户密码进行处理。
            .passwordEncoder(new BCryptPasswordEncoder())
            .withUser("nasus")
            // 保证用户登录时使用 bcrypt 对密码进行处理再与内存中的密码比对
            .password(new BCryptPasswordEncoder().encode("nasus")).roles("USER")
            .and()
            // 登陆时用 BCrypt 加密方式对用户密码进行处理。
            .passwordEncoder(new BCryptPasswordEncoder())
            .withUser("chenzy")
            // 保证用户登录时使用 bcrypt 对密码进行处理再与内存中的密码比对
            .password(new BCryptPasswordEncoder().encode("chenzy")).roles("USER");
    }

    @Override
    public void configure(WebSecurity web) throws Exception {
        // /resource/static 目录下的静态资源，Spring Security 不拦截
        web.ignoring().antMatchers("/resource/static**");
    }
}
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657
```

## WebSocket 的配置

在上一篇的基础上另外注册一个名为 “/endpointChat” 的节点，以供用户订阅，只有订阅了该节点的用户才能接收到消息；然后，再增加一个名为 “/queue” 消息代理。

```java
@Configuration
// @EnableWebSocketMessageBroker 注解用于开启使用 STOMP 协议来传输基于代理（MessageBroker）的消息，这时候控制器(controller)
// 开始支持@MessageMapping,就像是使用 @requestMapping 一样。
@EnableWebSocketMessageBroker
public class WebSocketConfig implements WebSocketMessageBrokerConfigurer {


    @Override
    public void registerStompEndpoints(StompEndpointRegistry registry) {
        //注册一个名为 /endpointNasus 的 Stomp 节点(endpoint),并指定使用 SockJS 协议。
        registry.addEndpoint("/endpointNasus").withSockJS();
        //注册一个名为 /endpointChat 的 Stomp 节点(endpoint),并指定使用 SockJS 协议。
        registry.addEndpoint("/endpointChat").withSockJS();
    }

    @Override
    public void configureMessageBroker(MessageBrokerRegistry registry) {
        // 广播式配置名为 /nasus 消息代理 , 这个消息代理必须和 controller 中的 @SendTo 配置的地址前缀一样或者全匹配
        // 点对点增加一个 /queue 消息代理
        registry.enableSimpleBroker("/queue","/nasus/getResponse");
    }
}
12345678910111213141516171819202122
```

## 控制器 controller

指定发送消息的格式以及模板。详情见，代码注释。

```java
@Autowired
//使用 SimpMessagingTemplate 向浏览器发送信息
private SimpMessagingTemplate messagingTemplate;

@MessageMapping("/chat")
public void handleChat(Principal principal,String msg){
    // 在 SpringMVC 中，可以直接在参数中获得 principal，principal 中包含当前用户信息
    if (principal.getName().equals("nasus")){
        // 硬编码，如果发送人是 nasus 则接收人是 chenzy 反之也成立。
        // 通过 messageingTemplate.convertAndSendToUser 方法向用户发送信息，参数一是接收消息用户，参数二是浏览器订阅地址，参数三是消息本身
        messagingTemplate.convertAndSendToUser("chenzy",
                "/queue/notifications",principal.getName()+"-send:" + msg);
    } else {
        messagingTemplate.convertAndSendToUser("nasus",
               "/queue/notifications",principal.getName()+"-send:" + msg);
    }
}
1234567891011121314151617
```

## 登录页面

```
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:th="http://www.thymeleaf.org"
      xmlns:sec="http://www.thymeleaf.org/thymeleaf-extras-springsecurity3">
<meta charset="UTF-8" />
<head>
    <title>登陆页面</title>
</head>
<body>
<div th:if="${param.error}">
    无效的账号和密码
</div>
<div th:if="${param.logout}">
    你已注销
</div>
<form th:action="@{/login}" method="post">
    <div><label> 账号 : <input type="text" name="username"/> </label></div>
    <div><label> 密码: <input type="password" name="password"/> </label></div>
    <div><input type="submit" value="登陆"/></div>
</form>
</body>
</html>
123456789101112131415161718192021
```

## 聊天页面

```
<!DOCTYPE html>

<html xmlns:th="http://www.thymeleaf.org">
<meta charset="UTF-8" />
<head>
    <title>Home</title>
    <script th:src="@{sockjs.min.js}"></script>
    <script th:src="@{stomp.min.js}"></script>
    <script th:src="@{jquery.js}"></script>
</head>
<body>
<p>
    聊天室
</p>

<form id="nasusForm">
    <textarea rows="4" cols="60" name="text"></textarea>
    <input type="submit"/>
</form>

<script th:inline="javascript">
    $('#nasusForm').submit(function(e){
        e.preventDefault();
        var text = $('#nasusForm').find('textarea[name="text"]').val();
        sendSpittle(text);
    });

    // 连接 SockJs 的 endpoint 名称为 "/endpointChat"
    var sock = new SockJS("/endpointChat");
    var stomp = Stomp.over(sock);
    stomp.connect('guest', 'guest', function(frame) {
        // 订阅 /user/queue/notifications 发送的消息，这里与在控制器的
        // messagingTemplate.convertAndSendToUser 中订阅的地址保持一致
        // 这里多了 /user 前缀，是必须的，使用了 /user 才会把消息发送到指定用户
        stomp.subscribe("/user/queue/notifications", handleNotification);
    });



    function handleNotification(message) {
        $('#output').append("<b>Received: " + message.body + "</b><br/>")
    }

    function sendSpittle(text) {
        stomp.send("/chat", {}, text);
    }
    $('#stop').click(function() {sock.close()});
</script>

<div id="output"></div>
</body>
</html>
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152
```

## 页面控制器 controller

```
@Controller
public class ViewController {

    @GetMapping("/nasus")
    public String getView(){
        return "nasus";
    }

    @GetMapping("/login")
    public String getLoginView(){
        return "login";
    }

    @GetMapping("/chat")
    public String getChatView(){
        return "chat";
    }

}
12345678910111213141516171819
```

## 测试

预期结果应该是：两个用户登录系统，可以互相发送消息。但是同一个浏览器的用户会话的 session 是共享的，这里需要在 Chrome 浏览器再添加一个用户。

具体操作在 Chrome 的 设置–>管理用户–>添加用户:

![谷歌浏览器添加用户](https://upload-images.jianshu.io/upload_images/3282134-18ca878f096cdd7a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

两个用户分别访问 <http://localhost:8080/login> 登录系统，跳转至聊天界面：

![img](https://upload-images.jianshu.io/upload_images/3282134-73cb5c58988a94fc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

相互发送消息：

![img](https://upload-images.jianshu.io/upload_images/3282134-d2df60abb27c0ccc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 完整代码

<https://github.com/turoDog/Demo/tree/master/springboot_websocket_demo>

如果觉得对你有帮助，请给个 Star 再走呗，非常感谢。

## 后语

如果本文对你哪怕有一丁点帮助，请帮忙点好看。你的好看是我坚持写作的动力。

另外，关注之后在发送 **1024** 可领取免费学习资料。

资料详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)







# (十八) | 整合 MongoDB

2019年03月09日 19:14:47

 

一个优秀的废人

 

阅读数：7

更多

所属专栏： [陈大志的 SpringBoot 实战教程](https://blog.csdn.net/column/details/34053.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/88370059

> 微信公众号：一个优秀的废人。如有问题，请后台留言，反正我也不会听。

![广东清远](https://upload-images.jianshu.io/upload_images/3282134-d204d8b21013796b.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 前言

如题，今天介绍下 SpringBoot 是如何整合 MongoDB 的。

## MongoDB 简介

MongoDB 是由 C++ 编写的非关系型数据库，是一个基于分布式文件存储的开源数据库系统，它将数据存储为一个文档，数据结构由键值 (key=>value) 对组成。MongoDB 文档类似于 JSON 对象。字段值可以包含其他文档，数组及文档数组，非常灵活。存储结构如下：

```json
{
	"studentId": "201311611405",
	"age":24,
	"gender":"男",
	"name":"一个优秀的废人"
}
123456
```

## 准备工作

- SpringBoot 2.1.3 RELEASE
- MongnDB 2.1.3 RELEASE
- MongoDB 4.0
- IDEA
- JDK8
- 创建一个名为 test 的数据库，不会建的。参考菜鸟教程：
  <http://www.runoob.com/mongodb/mongodb-tutorial.html>

## 配置数据源

```yaml
spring:
  data:
    mongodb:
      uri: mongodb://localhost:27017/test
1234
```

以上是无密码写法，如果 MongoDB 设置了密码应这样设置：

```
spring:
  data:
    mongodb:
      uri: mongodb://name:password@localhost:27017/test
1234
```

## pom 依赖配置

```xml
<!-- mongodb 依赖 -->
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-data-mongodb</artifactId>
</dependency>
<!-- web 依赖 -->
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-web</artifactId>
</dependency>
<!-- lombok 依赖 -->
<dependency>
	<groupId>org.projectlombok</groupId>
	<artifactId>lombok</artifactId>
	<optional>true</optional>
</dependency>
<!-- test 依赖（没用到） -->
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-test</artifactId>
	<scope>test</scope>
</dependency>
12345678910111213141516171819202122
```

## 实体类

```java
@Data
public class Student {

    @Id
    private String id;

    @NotNull
    private String studentId;

    private Integer age;

    private String name;

    private String gender;

}
12345678910111213141516
```

## dao 层

和 JPA 一样，SpringBoot 同样为开发者准备了一套 Repository ，只需要继承 MongoRepository 传入实体类型以及主键类型即可。

```java
@Repository
public interface StudentRepository extends MongoRepository<Student, String> {
}
123
```

## service 层

```java
public interface StudentService {

    Student addStudent(Student student);

    void deleteStudent(String id);

    Student updateStudent(Student student);

    Student findStudentById(String id);

    List<Student> findAllStudent();

}
12345678910111213
```

实现类：

```java
@Service
public class StudentServiceImpl implements StudentService {

    @Autowired
    private StudentRepository studentRepository;

    /**
     * 添加学生信息
     * @param student
     * @return
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public Student addStudent(Student student) {
        return studentRepository.save(student);
    }

    /**
     * 根据 id 删除学生信息
     * @param id
     */
    @Override
    public void deleteStudent(String id) {
        studentRepository.deleteById(id);
    }

    /**
     * 更新学生信息
     * @param student
     * @return
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public Student updateStudent(Student student) {
        Student oldStudent = this.findStudentById(student.getId());
        if (oldStudent != null){
            oldStudent.setStudentId(student.getStudentId());
            oldStudent.setAge(student.getAge());
            oldStudent.setName(student.getName());
            oldStudent.setGender(student.getGender());
            return studentRepository.save(oldStudent);
        } else {
            return null;
        }
    }

    /**
     * 根据 id 查询学生信息
     * @param id
     * @return
     */
    @Override
    public Student findStudentById(String id) {
        return studentRepository.findById(id).get();
    }

    /**
     * 查询学生信息列表
     * @return
     */
    @Override
    public List<Student> findAllStudent() {
        return studentRepository.findAll();
    }
}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465
```

## controller 层

```java
@RestController
@RequestMapping("/student")
public class StudentController {

    @Autowired
    private StudentService studentService;

    @PostMapping("/add")
    public Student addStudent(@RequestBody Student student){
        return studentService.addStudent(student);
    }

    @PutMapping("/update")
    public Student updateStudent(@RequestBody Student student){
        return studentService.updateStudent(student);
    }

    @GetMapping("/{id}")
    public Student findStudentById(@PathVariable("id") String id){
        return studentService.findStudentById(id);
    }

    @DeleteMapping("/{id}")
    public void deleteStudentById(@PathVariable("id") String id){
        studentService.deleteStudent(id);
    }

    @GetMapping("/list")
    public List<Student> findAllStudent(){
        return studentService.findAllStudent();
    }

}
123456789101112131415161718192021222324252627282930313233
```

## 测试结果

![MongoDB 增删改查](https://upload-images.jianshu.io/upload_images/3282134-2fd381b95f3d7b9f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

Postman 测试已经全部通过，这里仅展示了保存操作。

![数据库结果](https://upload-images.jianshu.io/upload_images/3282134-927ac7262152df6e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

这里推荐一个数据库可视化工具 Robo 3T 。下载地址：<https://robomongo.org/download>

## 完整代码

<https://github.com/turoDog/Demo/tree/master/springboot_mongodb_demo>

如果觉得对你有帮助，请给个 Star 再走呗，非常感谢。

## 后语

如果本文对你哪怕有一丁点帮助，请帮忙点好看。你的好看是我坚持写作的动力。

另外，关注之后在发送 **1024** 可领取免费学习资料。

资料详情请看这篇旧文：[Python、C++、Java、Linux、Go、前端、算法资料分享](https://www.jianshu.com/p/ed43dc8aa11e)













