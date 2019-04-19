# Java Web 工程目录结构 - xqhrs232的专栏 - CSDN博客
2018年10月25日 14:28:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：35
原文地址::[https://www.cnblogs.com/reghao/p/8490855.html](https://www.cnblogs.com/reghao/p/8490855.html)
相关文章
1、[JavaWeb工程 目录结构](https://www.cnblogs.com/zhanghengscnc/p/8449894.html)----[https://www.cnblogs.com/zhanghengscnc/p/8449894.html](https://www.cnblogs.com/zhanghengscnc/p/8449894.html)
为了使 Web 容器顺利地执行 Web 应用，开发者需要以一种标准的方式将 Web 项目中的资源（servlets、JSP 等）打包。一个 Web 项目的目录结构可分为两种：
- 
发布目录结构
> 
Web 容器加载 Web 应用的统一标准目录。
- 
编译目录结构
> 
一种方便 Web 工程编写、修改的临时结构，需要通过构建工具（ant、maven 等）编译生成最终的发布目录结构。因此编译目录结构并不统一，与所使用的编译工具（以及配置）有关。
### 发布目录结构
Web 项目的发布目录结构如下所示：
```
HelloWorld/
├── index.jsp
├── META-INF
│   └── MANIFEST.MF
└── WEB-INF
    ├── classes
    ├── lib
    └── web.xml
```
- 
index.jsp
> 
通过 [http://localhost:8080/HelloWorld/](http://localhost:8080/HelloWorld/) 访问的即是该文件，此 Web 应用的根目录名为 HelloWorld。
静态资源（jsp、html、css、js 以及 png 等）可直接放在 HelloWorld/ 中，也可在 HelloWorld/ 下创建相应的目录分别放置。
- 
META-INF 目录
> 
MANIFEST.MF 包含 Web 应用的版本等信息。
- 
WEB-INF 目录
> 
classes 目录包含 Web 应用中所有的已编译 Java 类文件（以及配置文件等），是 Web 应用的核心部分，实现具体的业务逻辑。
lib 目录包含 Web 应用使用的所有类库文件（即第三方 JAR 文件）。
web.xml 是该 Web 项目的配置文件，用来供 Web 容器部署和执行 Web 应用。
### 编译目录结构
Web 项目的[编译目录结构](http://maven.apache.org/guides/introduction/introduction-to-the-standard-directory-layout.html)（使用 maven 构建）如下所示：
```
my-app/
├── pom.xml
├── src
│   ├── main
│   │   ├── java
│   │   │   └── mycompany
│   │   │       └── app
│   │   │           └── App.java
│   │   ├── resources
│   │   └── webapp
│   └── test
│       ├── java
│       │   └── com
│       │       └── mycompany
│       │           └── app
│       │               └── AppTest.java
│       └── resources
└── target
```
- 
pom.xml
> 
项目的配置文件，包含构建一个项目所需的大多数信息（主要是依赖的类库）。
- 
src/main/java 目录
> 
放置项目的源码。
- 
src/main/resources 目录
> 
放置项目的资源。
- 
src/main/webapp 目录
> 
放置 Web 项目的发布目录结构，其中包括 META-INF 以及 WEB-INF 目录。
> 
web.xml 文件放置在 WEB-INF 目录中。
- 
src/test/java 目录
> 
放置项目测试的源码。
- 
src/test/resources 目录
> 
放置项目测试的资源。
- 
target 目录
> 
放置构建项目生成的所有输出。
