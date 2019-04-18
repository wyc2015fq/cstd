# Maven学习四：使用mvn install创建自己的repository - z69183787的专栏 - CSDN博客
2014年12月18日 21:26:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2263
     mvn install 这个命令会把当前项目或模块打包之后注入到本地Repository中，以供引用。我们平时做项目的时候，也常把项目编译后打成jar包，让其他项目引用。maven提供的install命令更方便快捷的实现了这个操作，并且可供maven模块化管理，mvn
 package 可以生成相应的jar包。
      新建maven project-02 之前，要先在project-01的根目录下执行mvn clean install，然后可以在本地Repository中找到刚刚注入进来的mevan project-01，如下图：
![](https://img-blog.csdn.net/20131002210403250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGZzZnh5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
      那如何在新建project中引用该项目呢？仿照引用junit项目的方法，即可。也就是要修改pom.xml，新增一个dependency依赖项即可。如下，新建project-02的pom.xml:
**[html]**[view
 plain](http://blog.csdn.net/lfsfxy9/article/details/12201049#)[copy](http://blog.csdn.net/lfsfxy9/article/details/12201049#)[print](http://blog.csdn.net/lfsfxy9/article/details/12201049#)[?](http://blog.csdn.net/lfsfxy9/article/details/12201049#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/193663/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
- <modelVersion>4.0.0</modelVersion>
- 
- <groupId>jizg.study.maven.world</groupId>
- <artifactId>hello-second</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- 
- <dependencies>
- <dependency>
- <groupId>junit</groupId>
- <artifactId>junit</artifactId>
- <version>4.10</version>
- <scope>test</scope>
- </dependency>
- <dependency>
- <groupId>jizg.study.maven.hello</groupId>
- <artifactId>hello-first</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- </dependency>
- </dependencies>
- </project>
   在编写类文件的时候，只需要 import jizg.study.maven.hello.*， 即可完成hello包的引入。mvn install这个命令有助于我们积累和建立个人资源库，很好的并且有序的管理项目模块。
