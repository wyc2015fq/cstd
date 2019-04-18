# 使用Maven - z69183787的专栏 - CSDN博客
2015年09月02日 21:26:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：473
个人分类：[项目构建-Maven](https://blog.csdn.net/z69183787/article/category/2265961)
**1.使用属性properties和exclusions**
**如果要阻断一些传递的依赖，可以添加：exclusions，这样可以不在构建的时候往我们的构件离添加太多无用的jar，前提是经过测试，阻断依赖后程序功能无异常，或添加了其它相同功能的依赖，如阻断收费版，换成依赖免费版。**
Xml代码  ![收藏代码](http://juvenshun.iteye.com/images/icon_star.png)
- **<dependencies>**
- **<dependency>**
- **<groupId>**org.spring.framework**</groupId>**
- **<artifactId>**spring-core**</artifactId>**
- **<version>**${spring.version}**</version>**
- **</dependency>**
- **<dependency>**
- **<groupId>**org.spring.framework**</groupId>**
- **<artifactId>**spring-beans**</artifactId>**
- **<version>**${spring.version}**</version>**
- **</dependency>**
- **<dependency>**
- **<groupId>**org.spring.framework**</groupId>**
- **<artifactId>**spring-web**</artifactId>**
- **<version>**${spring.version}**</version>**
- **</dependency>**
- **<dependency>**
- **<groupId>**org.spring.framework**</groupId>**
- **<artifactId>**spring-mock**</artifactId>**
- **<version>**${spring.version}**</version>**
- **</dependency>**
- **</dependencies>**
- 
- **<properties>**
- **<spring.version>**2.5**</spring.version>**
- **</properties>**
这里我们定义了一个Maven属性，其名称为spring.version，值是2.5。在这个POM中，我们就能用${spring.version}的方式来引用该属性。我们看到，所有spring相关的依赖的version元素现在都成了${spring.version}，当Maven运行的时候，它会自动用值2.5来替换这个引用。
当我们需要升级spring的时候，只要更改一个地方便可，而且，你现在能很高的保证所有的spring依赖包都是同一个版本。
### 2.依赖范围(scope)
Xml代码  ![收藏代码](http://juvenshun.iteye.com/images/icon_star.png)
- **<dependency>**
- **<groupId>**junit**</groupId>**
- **<artifactId>**junit**</artifactId>**
- **<version>**4.4**</version>**
- **<scope>**test**</test>**
- **</dependency>**
Xml代码  ![收藏代码](http://juvenshun.iteye.com/images/icon_star.png)
- **<dependency>**
- **<groupId>**javax.servlet**</groupId>**
- **<artifactId>**servlet-api**</artifactId>**
- **<version>**2.4**</version>**
- **<scope>**provided**</scope>**
- **</dependency>**
主要的依赖范围以及作用：
|依赖范围（scope）|主源码classpath可用|测试源码classpath可用|会被打包|
|----|----|----|----|
|compile 缺省值|TRUE|TRUE|TRUE|
|test|FALSE|TRUE|FALSE|
|runtime|FALSE|TRUE|TRUE|
|provided|TRUE|TRUE|FALSE|
### 3.分类器（classifer）
GAV（groupid、artifactid、version）是Maven坐标最基本最重要的组成部分，但GAV不是全部。还有一个元素叫做分类器（classifier），90%的情况你不会用到它，但有些时候，分类器非常不可或缺的，因为有写jar的同一个gae会根据环境的不同如jdk版本不同会有差异，例如：json-lib
 的2.2.2版本有jdk13的版本，也有jdk15的发布板，如果不再依赖上添加<classifier>jdk15</classifier> 就会报jar找不到的情况，可以查看依赖库，发现它是由这个分类的。
我们也可以为自己的jar分类，比如按系统或jdk版本分类等待。
同样的如果要阻止一些依赖传递，可以添加：exclusions，这样可以不在构建的时候往我们的构件离添加太多无用的jar，前提是经过测试，阻断依赖后程序功能无异常，或添加了其它相同功能的依赖，如阻断收费版，换成依赖免费版。
**4.依赖管理（dependencyManagement）**
实际的项目中，你会有一大把的Maven模块，而且你往往发现这些模块有很多依赖是完全项目的，A模块有个对spring的依赖，B模块也有，它们的依赖配置一模一样，**同样的groupId, artifactId, version，或者还有exclusions, classifer**。细心的分会发现这是一种重复，重复就意味着潜在的问题，Maven提供的dependencyManagement就是用来消除这种重复的。
正确的做法是：
1. 在**父模块中使用dependencyManagement**配置依赖
2. 在子模块中使用dependencies添加依赖
**dependencyManagement实际上不会真正引入任何依赖，dependencies才会。但是，当父模块中配置了某个依赖之后，子模块只需使用简单groupId和artifactId就能自动继承相应的父模块依赖配置。**
这里是一个来自于《Maven权威指南》的例子：
父模块中如此声明：
Xml代码  ![收藏代码](http://juvenshun.iteye.com/images/icon_star.png)
- **<project>**
- **<modelVersion>**4.0.0**</modelVersion>**
- **<groupId>**org.sonatype.mavenbook**</groupId>**
- **<artifactId>**a-parent**</artifactId>**
- **<version>**1.0.0**</version>**
-   ...  
- **<dependencyManagement>**
- **<dependencies>**
- **<dependency>**
- **<groupId>**mysql**</groupId>**
- **<artifactId>**mysql-connector-java**</artifactId>**
- **<version>**5.1.2**</version>**
- **</dependency>**
-       ...  
- **<dependencies>**
- **</dependencyManagement>**
子模块中如此声明：
Xml代码  ![收藏代码](http://juvenshun.iteye.com/images/icon_star.png)
- **<project>**
- **<modelVersion>**4.0.0**</modelVersion>**
- **<parent>**
- **<groupId>**org.sonatype.mavenbook**</groupId>**
- **<artifactId>**a-parent**</artifactId>**
- **<version>**1.0.0**</version>**
- **</parent>**
- **<artifactId>**project-a**</artifactId>**
-   ...  
- **<dependencies>**
- **<dependency>**
- **<groupId>**mysql**</groupId>**
- **<artifactId>**mysql-connector-java**</artifactId>**
- **</dependency>**
- **</dependencies>**
- **</project>**
你依赖配置越复杂，依赖管理所起到的作用就越大，它不仅能够帮助你简化配置，它还能够帮你巩固依赖配置，也就是说，在整个项目中，对于某个构件（如mysql）的依赖配置只有一种，这样就能避免引入不同版本的依赖，避免依赖冲突。
**5.插件管理<pluginManagement>**
它的功能与作用与dependencyManagement类似，一般也是在父pom中定义插件的版本等配置信息，子pom只需要声明groupid和artifactid就可以继承使用所有的配置信息，在多模块中这是非常有用的，所以良好的pom编写习惯都应用在父pom中定义这些依赖和插件的管理，他们都不会引入依赖或插件，而只是定义了一些配置模板而已。
**6.资源(resources)和java属性文件application.properties**
我们知道maven的pom中定义了很多内置和自定义的properties如**${project.name} ${project.baseDir} ${project.baseDir} ${project.artifactId} ${project.build.testSourceDirectory}** 等，那么我们如何在java代码中使用呢？
maven已经帮我们做了，在main/resources下建立application.properties，内容如下：
**[plain]**[view
 plain](http://blog.csdn.net/conquer0715/article/details/9225735#)[copy](http://blog.csdn.net/conquer0715/article/details/9225735#)
- # in ava code, you can read this application.properties for getting the properties difined in maven pom  
- application.name=${pom.name}  
- application.version=${pom.version}  
- message=${my.filter.value}  
- 
- # do mvn process-resources "-Dcommand.line.prop=hello again" can get init the value of key mycommandkey！！！  
- mycommandkey=${command.line.prop}  
需要在pom中添加：
**[plain]**[view
 plain](http://blog.csdn.net/conquer0715/article/details/9225735#)[copy](http://blog.csdn.net/conquer0715/article/details/9225735#)
- <build>  
-         <resources>  
-             <resource>  
-                 <directory>src/main/resources</directory>  
-                 <filtering>true</filtering>  
-             </resource>  
-         </resources>  
-     </build>  
目的是告诉maven，这个文件下的资源需要处理，即将资源文件中的 pom 变量引用替换成实际的值，我们可以运行mvn process-resources然后再编译好的classes文件夹中查看转换后的application.properties，java代码中只需要读取这个属性文件就可以了。也可以往属性文件中添加需要命令行指定的参数，如上面的 **mycommandkey=${command.line.prop}**，赋值方式：mvn
 process-resources "-Dcommand.line.prop=hello"
**7.使用Profile，不同环境不同配置**
**这块内容详细：[http://haohaoxuexi.iteye.com/blog/1900568](http://haohaoxuexi.iteye.com/blog/1900568)**
profile可以让我们定义一系列的配置信息，然后指定其激活条件。这样我们就可以定义多个profile，然后每个profile对应不同的激活条件和配置信息，从而达到不同环境使用不同配置信息的效果。比如说，我们可以通过profile定义在jdk1.5以上使用一套配置信息，在jdk1.5以下使用另外一套配置信息；或者有时候我们可以通过操作系统的不同来使用不同的配置信息，比如windows下是一套信息，linux下又是另外一套信息，等等，示例：
**[plain]**[view
 plain](http://blog.csdn.net/conquer0715/article/details/9225735#)[copy](http://blog.csdn.net/conquer0715/article/details/9225735#)
- <profiles>    
-         <profile>    
-              <id>profileTest1</id>    
-              <properties>    
-                     <hello>world</hello>    
-              </properties>    
-              <activation>    
-                     <activeByDefault>true</activeByDefault>    
-              </activation>    
-         </profile>    
- 
-         <profile>    
-              <id>profileTest2</id>    
-              <properties>    
-                     <hello>andy</hello>    
-              </properties>    
-         </profile>    
-  </profiles>    
定义在pom.xml中的profile可以定义更多的信息。主要有以下这些：
l  <repositories>
l  <pluginRepositories>
l  <dependencies>
l  <plugins>
l  <properties>
l  <dependencyManagement>
l  <distributionManagement>
l  还有build元素下面的子元素，主要包括：
<defaultGoal>
<resources>
<testResources>
<finalName>
Maven给我们提供了多种不同的profile激活方式。比如我们可以使用-P参数显示的激活一个profile，也可以根据环境条件的设置让它自动激活等，它的激活方式非常灵活，这里不详细解释。
