
# 手把手构建Maven企业级应用（整合SSI框架）《2》 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月03日 08:56:59[seven-soft](https://me.csdn.net/softn)阅读数：615


下面我们构建我们项目的骨架，假定我们的项目为verticalmarket的一个项目，那么我们知道这个父工程下面我们要为其建立子模块，主要是把表示层、业务层及持久层分为不同的模块，其项目结构大致如下：
目录结构代码![复制代码](http://tk-zhang.javaeye.com/images/icon_copy.gif)
verticalmarket-dal
verticalmarket-manager
verticalmarket-service
verticalmarket-config
verticalmarket-web

当然结构可以随意，按照你们自己约定俗成的就可以，这个仅为一个例子，下面就让我们开始创建这个工程，当然Maven的一
些基础知识我不在说明了，可以看下Sonatype公司的教程Maven权威指南。
1、构建verticalmarket的父母录，在你的任意目录下，打开shell窗口（Windows的cmd），输入命令：
命令行代码![复制代码](http://tk-zhang.javaeye.com/images/icon_copy.gif)
mvn archetype:create -DgroupId=com.tkz.verticalmarket -DartifactId=verticalmarket
2、在生成的verticalmarket文件夹下面我们可以看到一个src文件夹和一个pom.xml，删除src文件夹，然后修改pom文件
Xml代码![复制代码](http://tk-zhang.javaeye.com/images/icon_copy.gif)
<?xmlversion="1.0"encoding="UTF-8"?>
<projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
<modelVersion>4.0.0</modelVersion>
<groupId>com.tkz.verticalmarket</groupId>
<artifactId>verticalmarket</artifactId>
<packaging>pom</packaging>
<version>1.0</version>
<name>verticalmarket</name>
<url>http://maven.apache.org</url>
<dependencyManagement>
<dependencies>
<dependency>
<groupId>junit</groupId>
<artifactId>junit</artifactId>
<version>4.0</version>
<scope>test</scope>
</dependency>
</dependencies>
</dependencyManagement>
</project>
**[xml]**[view
 plain](http://blog.csdn.net/huxin1/article/details/5549874#)[copy](http://blog.csdn.net/huxin1/article/details/5549874#)

<?xmlversion="1.0"encoding="UTF-8"?>
<projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
<modelVersion>4.0.0</modelVersion>
<groupId>com.tkz.verticalmarket</groupId>
<artifactId>verticalmarket</artifactId>
<packaging>pom</packaging>
<version>1.0</version>
<name>verticalmarket</name>
<url>http://maven.apache.org</url>
<dependencyManagement>
<dependencies>
<dependency>
<groupId>junit</groupId>
<artifactId>junit</artifactId>
<version>4.0</version>
<scope>test</scope>
</dependency>
</dependencies>
</dependencyManagement>
</project>
其中修改的重点为打包方式改为pom形式，这也就意味这这是一个父工程，另外版本号默认是SNAPSHOT意思是快照的意思，就是项目开发中的意思，你要是看着不爽可以把它删掉，另外需要说明一下dependencyManagement标签，这个标签表示子类可以隐式的继承父pom文件的依赖库，在子pom中不需要指定版本号
，推荐这样，这样可以方便开发，你要修改什么依赖的版本只需要更改父pom就可以了，dependencies是显示继承，你要是在
子pom中声明，就必须写明版本号，不写默认就继承了。
3、下面我们构建子工程项目，进入verticalmarket目录下，输入命令：
命令行代码![复制代码](http://tk-zhang.javaeye.com/images/icon_copy.gif)
mvn archetype:create -DgroupId=com.tkz.verticalmarket -DartifactId=verticalmarket-dal -Dpackage=com.tkz.verticalmarket.dal
然后在父目录下面就生成了verticalmarket-dal子工程，此时父pom文件自动添加项目关系：
Xml代码![复制代码](http://tk-zhang.javaeye.com/images/icon_copy.gif)
<modules>
<module>verticalmarket-dal</module>
</modules>
**[xml]**[view
 plain](http://blog.csdn.net/huxin1/article/details/5549874#)[copy](http://blog.csdn.net/huxin1/article/details/5549874#)

<modules>
<module>verticalmarket-dal</module>
</modules>
这就表示这个工程是verticalmarket子工程，同样道理，其他命令如下：
命令行代码![复制代码](http://tk-zhang.javaeye.com/images/icon_copy.gif)
mvn archetype:create -DgroupId=com.tkz.verticalmarket -DartifactId=verticalmarket-manager -Dpackage=com.tkz.verticalmarket.manager
mvn archetype:create -DgroupId=com.tkz.verticalmarket -DartifactId=verticalmarket-service -Dpackage=com.tkz.verticalmarket.service
mvn archetype:create -DgroupId=com.tkz.verticalmarket -DartifactId=verticalmarket-config -Dpackage=com.tkz.verticalmarket.config
mvn archetype:create -DgroupId=com.tkz.verticalmarket -DartifactId=verticalmarket-web -Dpackage=com.tkz.verticalmarket.web -DarchtypeArtifactId=maven-archetype-webapp
最后的-DarchetypeArtifactId=maven-archtype-webapp，表示这个是web程序结构，其他的默认就是java程序结构
4、然后可以执行命令：
命令行代码![复制代码](http://tk-zhang.javaeye.com/images/icon_copy.gif)
mvn eclipse:clean eclipse:eclipse
即生成了eclipse结构，导入到eclipse中你既可以看到你的一个多模块程序，到目前为止这个程序骨架就算搭建完成了，下一步我们考虑如何整合SSI程序到这个多模块程序中

