# 菜鸟必知的 Maven 知识 - weixin_33985507的博客 - CSDN博客
2016年07月26日 15:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
> 
相信看到这篇文章的人中 80% 都从各类求职网站的 JD 中看到过这么一条，“熟练使用 Maven 等构建工具”。这句话包括了两个意思，“1.何为构建工具”，“ 2.为什么是 Maven”。
　　笔者在日常生产代码的过程中也经常使用到 Maven，但是只停留于 pom.xml 以及 mvn clean / mvn exec:exec complie 的模糊概念。但是作为有志于成为一名资（diao）深（si）码农的我，是不会放过任何增加技能点的机会的！
### 1 - 什么是构建？
我们都知道，写完代码之后需要进行编译和运行，以笔者自身为例，使用 Eclipse 写完代码，需要进行编译，再生成 war 包，以便部署到 Tomcat。
　　在编写 Java 代码的时候，我们除了需要调用 jdk 的 api，还需要调用许多第三方的 api，加入没有构建工具，你需要把这些 jar 包下载到本地，然后添加进入工程，在 IDE 中进行添加设置。这种方式非常繁琐，并且在遇到版本升级，git 同步等时候，程序会变得非常脆弱，极易产生未知错误。所以便有了构建工具的产生，它可以让我们专注于写代码，而不需要考虑如何导入 jar 包，如何升级 jar 包版本，以及 git 多人协作等等问题。这是在编译过程中的优势，在运行和发布的过程中，构建工具依然可以帮助我们将工程生成指定格式的文件。
### 2 - Maven 介绍
#### 2.1 pom.xml
Maven项目的核心是pom.xml。POM( Project Object Model，项目对象模型)定义了项目的基本信息，用于描述项目如何构建，声明项目依赖等。
```
<project xmlns="http://maven.apache.org/POM/4.0.0" 
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 
        http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <groupId>com.binbinjiang</groupId>
    <artifactId>binbinjiang-jianshu</artifactId>
    <version>1.0-SNAPSHOT</version>
    <packaging>war</packaging>
    <name>my-jianshu</name>
...
</project>
```
- 第一行是 XML 头，指定了该 xml 文档的版本和编码方式；
- 最重要的是 groupId、artifactId、version 这三行
- **groupId**：定义当前 Maven 项目隶属的实际项目，一个实际项目下会有多个 Maven 项目。如一家公司开发的产品，groupId 就是公司/个人域名，但会包括众多的 Maven 项目；
- **artifactId**： 定义实际项目中的一个 Maven 项目，一般用实际项目名称作为 artifactId 的前缀；
- **version**：指定 binbinjiang-jianshu 工程的版本 - 1.0 SNAPSHOT，SNAPSHOT 意为快照，说明在该项目还处于开发中；
- **packaging**：定义 Maven 项目的打包方式，有 JAR 、WAR 和 EAR 三种格式（[基于Java的打包jar、war、ear包的作用与区别详解](https://link.jianshu.com?t=http://www.jb51.net/article/35881.htm)）
```
<dependencies>
    <dependency>
        <groupId>com.google.guava</groupId>
        <artifactId>guava</artifactId>
        <version>18.0</version>
    </dependency>
    <dependency>
        <groupId>javax</groupId>
        <artifactId>javaee-web-api</artifactId>
        <version>6.0</version>
        <scope>provided</scope>
    </dependency>
        ...
<dependencies>
```
- 这串代码就是 Maven 最核心的功能——**依赖管理**。本例调用了google guava 18.0 和 javax 6.0 的 jar 包
- **dependencies**：所有的依赖在此范围内添加，包含诸多 dependency；
- **groupId，artifactId，version**：定位到了需要调用 jar 包的坐标；
- **scope**：定义依赖范围
- *compile *：编译范围（默认），在所有的 classpath 中可用；
- *provided *：已提供范围，provided 依赖只有在当JDK 或者一个容器已提供该依赖之后才使用
- *test *：测试范围，在一般的编译和运行时都不需要，它们只有在测试编译和测试运行阶段可用
- *runtime *：运行时范围，在运行和测试系统的时候需要，但在编译的时候不需要。如在编译的时候只需要JDBC api jar，而只有在运行的时候才需要JDBC
- *system *：系统范围，依赖与provided 类似，但是你必须显式的提供一个对于本地系统中JAR 文件的路径
#### 2.2 Maven 仓库
这一节我将教你如何搜索你想要的 jar 包，我以搜索 Google guava 为例
- 首先，你需要在 google 上搜索 [Maven Repository](https://link.jianshu.com?t=https://www.google.com.hk/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0ahUKEwivovOfy5DOAhVEgI8KHSybAx0QFggaMAA&url=https%3A%2F%2Fmvnrepository.com%2F&usg=AFQjCNEi0Iwvab0wNQ1QMj56wB6GH3HZMQ&cad=rja)；
- 
然后，在搜索框输入 guava；
![1935978-387ac25a3763c730.png](https://upload-images.jianshu.io/upload_images/1935978-387ac25a3763c730.png)
图1 - 搜索 Google guava(1)
- *Guava: Google Core Libraries For Java* 就是官网的 api，点击后进入下图；
![1935978-03f6524848ce78c9.png](https://upload-images.jianshu.io/upload_images/1935978-03f6524848ce78c9.png)
图2 - 搜索 Google guava(2)
- 你需要选择相应的版本，如果你没有明确的版本要求，你可以选择 Usages 最多的或者是版本最新的，本工程便是选择 Usages 最多的 18.0 版本（使用者多代表着更加稳定），继续点击；
- 进入下图的页面后，你便可以将 <dependency></dependency> 内的内容全部复制到你的 pom.xml 中了，如果你还不知道具体放在 pom.xml 文件的哪个地方，你可以回到 **2.1 节**再去看一下。
![1935978-c3a71a4ff7f1e30e.png](https://upload-images.jianshu.io/upload_images/1935978-c3a71a4ff7f1e30e.png)
图3 - 搜索 Google guava(3)
#### 2.3 Maven 指令
Maven 的生命周期包括编译，运行，测试，打包，在不同的周期中，需要使用不同的指令来执行相应的功能，下面例举了一些常用的 Maven 指令以供参考。
- 编译源代码
mvn compile
- 发布项目
mvn deploy
- 编译测试源代码
mvn test-compile
- 运行应用程序中的单元测试
mvn test
- 生成项目相关信息的网站
mvn site
- 编译源代码
mvn compile
- 清除项目目录中的生成结果
mvn clean
- 根据项目生成的jar
mvn package
- 在本地Repository中安装jar
mvn install
- 生成eclipse项目文件
mvn eclipse:eclipse
- 启动jetty服务
mvn jetty:run
- 启动tomcat服务
mvn tomcat:run
- 清除以前的包后重新打包，跳过测试类
mvn clean package -Dmaven.test.skip=true
### *Thanks for reading ...   　　　_*
