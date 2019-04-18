# Maven零散笔记——常用配置(tomcat-maven-plugin) - z69183787的专栏 - CSDN博客
2015年07月29日 18:14:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：13155
Maven用了一段时间了，基本上被我摆平了。结合Eclipse跟Bamboo，Maven还是相当给力的！![](http://snowolf.iteye.com/images/smiles/icon_biggrin.gif)
先整理下已知内容，以免过早遗忘，甚至丢失！![](http://snowolf.iteye.com/images/smiles/icon_smile.gif)
相关链接: 
[Maven零散笔记——常用配置](http://snowolf.iteye.com/blog/953735)
[Maven零散笔记——配置Nexus](http://snowolf.iteye.com/blog/1605811)
pom.xml基础配置： 
maven中，最让我迷惑的还是那一堆配置！![](http://snowolf.iteye.com/images/smiles/icon_cry.gif)
就拿这个属性配置来说： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <properties>
- <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
- <spring.framework.version>3.0.5.RELEASE</spring.framework.version>
- <cxf.version>2.3.2</cxf.version>
- <tiles.version>2.2.2</tiles.version>
- </properties>
我需要让整个项目统一字符集编码，就需要设定<project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>；如果我需要让spring统一版本号，可以设置这么个变量<spring.framework.version>3.0.5.RELEASE</spring.framework.version>，当然，maven不会那么乖乖的自动识别这些配置。当然，<project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>默认还是可以识别的！ 
插件配置： 
资源配置插件： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-resources-plugin</artifactId>
- <version>2.4.3</version>
- <configuration>
- <encoding>${project.build.sourceEncoding}</encoding>
- </configuration>
- </plugin>
xml、properties文件都是资源文件，编码的时候遇到中文总要进行转码！用什么编码？UTF-8，那就记得强制<encoding>${project.build.sourceEncoding}</encoding>，虽然<project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>默认可以识别，但是我还是觉得不放心。 
这些插件有时候即便是不配置，它也是默认存在的！ 
编译插件： 
这个插件就是个默认配置，不过我还是谨慎的配置了jdk编译版本跟字符集设置： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-compiler-plugin</artifactId>
- <version>2.0.2</version>
- <configuration>
- <source>1.6</source>
- <target>1.6</target>
- <encoding>${project.build.sourceEncoding}</encoding>
- </configuration>
- </plugin>
<source>1.6</source>：源代码编译版本； 
<target>1.6</target>：目标平台编译版本； 
<encoding>${project.build.sourceEncoding}</encoding>：字符集编码。 
如果你用eclipse建立maven项目时，新建的项目往往JDK版本很低——1.5！怎么办？修改settings.xml文件： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <profile>
- <id>jdk-1.6</id>
- <activation>
- <activeByDefault>true</activeByDefault>
- <jdk>1.6</jdk>
- </activation>
- <properties>
- <maven.compiler.source>1.6</maven.compiler.source>
- <maven.compiler.target>1.6</maven.compiler.target>
- <maven.compiler.compilerVersion>1.6</maven.compiler.compilerVersion>
- </properties>
- </profile>
eclipse会很听话的构建一个JDK1.6版本的maven项目！![](http://snowolf.iteye.com/images/smiles/icon_biggrin.gif)
测试插件： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-surefire-plugin</artifactId>
- <version>2.7.2</version>
- <configuration>
- <parallel>methods</parallel>
- <threadCount>10</threadCount>
- <argLine>-Dfile.encoding=UTF-8</argLine>
- <!-- <skip>true</skip> -->
- <!-- <testFailureIgnore>true</testFailureIgnore> -->
- </configuration>
- </plugin>
<parallel>methods</parallel>：方法级并发； 
<threadCount>10</threadCount>：是线程数，当前是10； 
<argLine>-Dfile.encoding=UTF-8</argLine>：就是参数命令行，这一行很关键。测试插件有个小问题，它不依赖项目的字符集设置，它依赖于操作系统。如果你用命令行操作，看中文提示测试结果还好。但在win+eclipse（UTF-8设置）下，那就是乱码了； 
<skip>true</skip>是忽略测试； 
<testFailureIgnore>true</testFailureIgnore>：忽略测试异常； 
测试报告插件： 
要测试，就要有报告，知晓测试覆盖率，这就需要clover： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <plugin>
- <groupId>maven</groupId>
- <artifactId>maven-clover-plugin</artifactId>
- <version>2.4.2</version>
- <configuration>
- <encoding>${project.build.sourceEncoding}</encoding>
- </configuration>
- </plugin>
安全起见，我还是要强制写遍字符集编码设置：<encoding>${project.build.sourceEncoding}</encoding>
可以使用 mvn cobertura:cobertura 产生测试覆盖率报告！ 
eclipse插件： 
在eclipse下进行maven项目开发，还是需要eclipse相应的插件： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-eclipse-plugin</artifactId>
- <version>2.8</version>
- <configuration>
- <wtpmanifest>true</wtpmanifest>
- <wtpapplicationxml>true</wtpapplicationxml>
- <wtpversion>2.0</wtpversion>
- <downloadSources>true</downloadSources>
- <downloadJavadocs>true</downloadJavadocs>
- </configuration>
- </plugin>
<wtpmanifest>true</wtpmanifest>：产生manifest文件 
<wtpapplicationxml>true</wtpapplicationxml>：产生web.xml文件 
<wtpversion>2.0</wtpversion>：这是eclipse里面的wtp版本 
<downloadSources>true</downloadSources>：下载源代码 
<downloadJavadocs>true</downloadJavadocs>：下载源码文档 
虽然这么配置了，但是由maven项目转为eclipse的wtp类型的项目时，所产生的文件根本没有实际意义。web.xml文件的servlet版本很低，也许是我没有配置。对应的xml字符集的头标识也没有。 
war包插件： 
既然是web项目，就需要打war包，那就需要这个插件： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-war-plugin</artifactId>
- <version>2.1.1</version>
- <configuration>
- <encoding>${project.build.sourceEncoding}</encoding>
- <warName>platform</warName>
- <webappDirectory>${project.build.directory}/platform</webappDirectory>
- <warSourceDirectory>WebContent</warSourceDirectory>
- </configuration>
- </plugin>
<encoding>${project.build.sourceEncoding}</encoding>强制字符集编码 
<warName>platform</warName>war包名字——platform.war 
<webappDirectory>${project.build.directory}/platform</webappDirectory>产生war前，用于存放构建war包的目录——target/platform。 
<warSourceDirectory>WebContent</warSourceDirectory>：我把web工程搞成了eclipse下的WTP类型。我不喜欢maven产生的webapp目录，更喜欢WebContent！ 
tomcat插件： 
我习惯于用tomcat作为JSP容器，对jetty多少有点陌生。不但如此，我还通过在Bamboo中配置命令让这个应用可以在tomcat中定时部署： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <plugin>
- <groupId>org.codehaus.mojo</groupId>
- <artifactId>tomcat-maven-plugin</artifactId>
- <configuration>
- <charset>${project.build.sourceEncoding}</charset>
- <url>http://localhost:8080/manager</url>
- <server>tomcat.server</server>
- <path>/platform</path>
- <port>8080</port>
- <warFile>${project.build.directory}/platform.war</warFile>
- <warSourceDirectory>WebContent</warSourceDirectory>
- <uriEncoding>${project.build.sourceEncoding}</uriEncoding>
- </configuration>
- </plugin>
<charset>${project.build.sourceEncoding}</charset>：字符集强制编码 
<url>http://localhost:8080/manager/text</url>：我这里已经使用了Tomcat7，如果是Tomcat6就不需要/text！ 
<server>tomcat.server</server>这是个用户名设置，需要配置maven的settings.xml文件： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <servers>
- <server>
- <id>tomcat.server</id>
- <username>admin</username>
- <password>123456</password>
- </server>
- </servers>
别急，这时候还没完！ 
在tomcat的tomcat-users.xml中补充相关内容： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <rolerolename="manager-gui"/>
- <rolerolename="magager-script"/>
- <userusername="admin"password="123456"roles="manager-gui,manager-script"/>
这是Tomat7的配置，在Tomcat6中应该是： 
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <rolerolename="manager"/>
- <userusername="admin"password="123456"roles="manager"/>
<port>8080</port>:运行时端口 
<path>/platform</path>：是运行时路径——http://host:port/platform 
<warFile>${project.build.directory}/platform.war</warFile>：我们以war包方式发布，需要指定war包路径。
<warSourceDirectory>WebContent</warSourceDirectory>：同时要指定war包源码路径。这里我使用eclipse的WTP类型工程WebContent目录。 
<uriEncoding>${project.build.sourceEncoding}</uriEncoding>：强制字符集编码！ 
插件命令： 
Shell代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- #Run Tomcat  
- mvn tomcat:run  
- #Stop Tomcat  
- #Deploy Tomcat  
- mvn tomcat:deploy  
- #Undeploy Tomcat  
- mvn tomcat:undeploy  
- #Redeploy Tomcat  
- mvn tomcat:redeploy  
对于我来说，我最常用的就是mvn tomcat:run 跟 mvn
 tomcat:redeploy，而且我会把他们配置在eclipse中： 
![](http://dl.iteye.com/upload/attachment/433884/c3087e8c-009d-3569-b74d-8f3d347956a8.jpg)
加上个-X，我可以清楚的知道每一个插件的配置，以及详细的Debug信息！ 
先说这么多～～～以后分块整理！![](http://snowolf.iteye.com/images/smiles/icon_biggrin.gif)
相关链接: 
[Maven零散笔记——常用配置](http://snowolf.iteye.com/blog/953735)
[Maven零散笔记——配置Nexus](http://snowolf.iteye.com/blog/1605811)
