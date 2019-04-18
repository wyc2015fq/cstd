# IntelliJ IDEA 12.0搭建Maven Web SSH2架构项目示例(二) - z69183787的专栏 - CSDN博客
2014年11月25日 15:10:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2288
个人分类：[IDE-Idea](https://blog.csdn.net/z69183787/article/category/2695809)
**3.引入一个现有的Maven-web-application项目**
引入Maven项目和普通的web项目有所不同，如果你通过【File】->【Import Project】的方式来引入一个Maven-web项目的话，其结构项目目录结构会出现混乱。正确的方式如下图：
![](http://www.open-open.com/home/attached/f_201401/20140104163815_10917.png)
![](http://www.open-open.com/home/attached/f_201401/20140104163826_65437.png)
这样才是引入一个已经存在的maven-web项目的正确方式，小小的注意点。
**4.环境配置与执行调试；**
针对一个项目的运行、编译和调试，首先我们引入一个项目，如上两幅图所示，然后用Maven进行编译，如下图所示按住Ctrl键选择clean 和 install两个选项，然后执行：
![](http://www.open-open.com/home/attached/f_201401/20140105053114_11540.png)
这是Run控制台执行成功的结果，代表编译成功。
![](http://www.open-open.com/home/attached/f_201401/20140105053321_52714.png)
第二步，添加Tomcat服务，如下图：
![](http://www.open-open.com/home/attached/f_201401/20140105053225_29830.png)
选择Tomcat安装路径：
![](http://www.open-open.com/home/image/zoom.gif)![](http://www.open-open.com/home/attached/f_201401/20140105053501_39984.png)
第三步，部署 war包，如下图：
![](http://www.open-open.com/home/attached/f_201401/20140105053641_99667.png)
因为是调试，所以选择 FrameModel:war exploded。在你真正发布的程序中是 FrameModel:war这个包，注意分清。然后，进行Debug，如下图：
![](http://www.open-open.com/home/attached/f_201401/20140105053915_53005.png)
调试成功后，控制台结果如下：
![](http://www.open-open.com/home/attached/f_201401/20140105053942_45488.png)
**5.Maven-web-application项目核心文件：pom.xml文件的结构分析**
OK，重头戏开始，Maven的pom.xml文件。pom文件定义了你项目中所有的第三方依赖和编译行为。这里给出一个简单的例子，要注意的地方在代码中都已经指出，代码如下：
**[html]**[view
 plain](http://blog.csdn.net/breatheryang/article/details/17200387#)[copy](http://blog.csdn.net/breatheryang/article/details/17200387#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/131917/fork)
- <spanstyle="font-family:SimSun;"><projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
- <modelVersion>4.0.0</modelVersion>
- <groupId>com.Yangcl.org</groupId>
- <artifactId>FrameModel</artifactId>
- <packaging>war</packaging>**<!--这里定义了你项目的打包方式，有war/bundle等-->**
- <name>FrameModel</name>
- <version>1.0.0-SNAPSHOT</version>
<!--证明这是一个快照版本-->
- 
- <properties>**<!--为pom定义一些常量,在pom中的其它地方可以直接引用-->**
- <spring.version>3.1.1.RELEASE</spring.version>
- <struts.version>2.3.4</struts.version>
- <hibernate.version>3.6.9.Final</hibernate.version>
- <axis2.version>1.6.2</axis2.version>
- <mysql.version>5.1.9</mysql.version>
- <aspectj.version>1.6.9</aspectj.version>
- <slf4j.version>1.6.4</slf4j.version>
- <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
- </properties>
- 
- **<!-- 一个项目所依赖的全部Jar文件信息定义描述,这里不一一列出-->**
- <dependencies>
- <dependency>
- <groupId>org.springframework</groupId>
- <artifactId>spring-core</artifactId>
- <version>${spring.version}</version>
- </dependency>
- <dependency>
- <groupId>org.springframework</groupId>
- <artifactId>spring-web</artifactId>
- <version>${spring.version}</version>
- </dependency>
- <dependency>
- <groupId>org.springframework</groupId>
- <artifactId>spring-beans</artifactId>
- <version>${spring.version}</version>
- </dependency>
- <dependency>
- <groupId>org.springframework</groupId>
- <artifactId>spring-tx</artifactId>
- <version>${spring.version}</version>
- </dependency>
- </dependencies>
- 
- **<!--针对一个项目的编译环节-->**
- <build>
- **<!--finalName 属性是你项目的名字-->**
- <finalName>FrameModel</finalName>
- <plugins>
- 
- **<!--编译操作的定义-->**
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-compiler-plugin</artifactId>
- <version>2.3.2</version>
- <configuration>
- <source>1.6</source>
- <target>1.6</target>
- </configuration>
- </plugin>
- 
- **<!--Maven的打包部署与Debug行为描述-->**
- <plugin>
- <artifactId>maven-war-plugin</artifactId>
- <version>2.1.1</version>
- <configuration>**<!-- 有这个定义，则你调试的时候不会进入target目录下，你就不用总去重启服务，尤其在页面调试的时候很方便 -->**
- <webappDirectory>${basedir}/src/main/webapp</webappDirectory>
- <warSourceDirectory>${basedir}/src/main/webapp</warSourceDirectory>
- </configuration>
- </plugin>
- 
- **<!--maven-surefire-plugin支持JUnit和TestNG-->****            <!--默认情况下，maven-surefire-plugin的test目标会自动执行测试源码--><!--路径下所有以Test开头、Test或TestCase结尾的的Java类。-->**
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-surefire-plugin</artifactId>
- <version>2.7.1</version>
- <configuration>
- <skipTests>true</skipTests>**<!--这里选择跳过测试> **
- </configuration>
- </plugin>
- 
- **<!--Maven Clean 操作-->**
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-clean-plugin</artifactId>
- <version>2.4.1</version>
- <configuration>
- <filesets>
- <fileset>
- <directory>${basedir}/src/main/webapp/WEB-INF/</directory>
- <excludes>**<!--排除哪些内容不被清除-->**
- <excluse>**/web.xml</excluse>
- </excludes>
- </fileset>
- </filesets>
- </configuration>
- </plugin>
- 
- </plugins>
- </build>
- </project>
- 
OK，到现在为止，Maven-web-application的所有实现、调试、细节和原理基本都已经介绍完毕。这里顺便点评一下几个主流的IDE：Eclipse、MyEclipse和IntelliJ IDEA。这三个编辑器里我个人觉得最好用的还是IDEA，毕竟是收费的软件，用户体验就是高。MyEclipse明显有些臃肿，而且针对ExtJS4.0以上的版本，他支持的不好，即使在调节和优化了以后，也还是会出现内存溢出的问题；你可能会说是你自己不会用，我的怎么就没事呢？那是因为你遇到的项目小。同样在对
 Maven 的支持上，Eclipse也明显没有 IDEA 做的好。以前闲来无事，还特意探究了下这两个IDE的插件机制，我发现IDEA的插件机制和Eclipse、MyEclipse区别还是很大，虽然思想都是基于OSGI。我个人觉得 IDEA 十分适合开发Web程序，尤其是Maven Web程序。o(︶︿︶)o 唉...好累，为了改这篇博客我重新装了一个空白的虚拟机，希望能对大家有所帮助吧，下面给大家两个链接，都是做好的Maven web程序，自己可以试一试。
第一个项目工程：[http://download.csdn.net/detail/breatheryang/5294757](http://download.csdn.net/detail/breatheryang/5294757)
第二个项目工程：[http://download.csdn.net/detail/breatheryang/5549101](http://download.csdn.net/detail/breatheryang/5549101)
如果您有什么高见，欢迎给我留言，咱们可以交流下，共同进步！
