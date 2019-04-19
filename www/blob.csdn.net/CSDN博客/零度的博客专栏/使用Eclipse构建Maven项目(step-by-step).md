# 使用Eclipse构建Maven项目(step-by-step) - 零度的博客专栏 - CSDN博客
2016年05月09日 15:49:36[零度anngle](https://me.csdn.net/zmx729618)阅读数：307标签：[eclipse](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)
Maven这个个项目管理和构建自动化工具，越来越多的开发人员使用它来管理项目中的jar包。本文仅对Eclipse中如何安装、配置和使用Maven进行了介绍。完全step by step。
**如果觉得本文对你有用，请移步到：[http://download.csdn.net/detail/qjyong/5582071](http://download.csdn.net/detail/qjyong/5582071) 下载PDF版，[http://download.csdn.net/detail/qjyong/5582087](http://download.csdn.net/detail/qjyong/5582087) 下载配套示例项目源代码。**
# 1.   Eclipse-jee的安装
进入eclipse官网的下载页面：[http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/)，如下图所示：
![](https://img-blog.csdn.net/20130614214452390)
根据你的操作系统具体情况选择相应的版本。我这里选择的是“Eclipse IDE for JavaEE Developers”的Windows 32 Bit版本。下载到本地磁盘即可。把下载完成的“eclipse-jee-juno-SR2-win32.zip”解压缩到本地磁盘指定目录下。我这里是解压缩在D:盘根目录下。
进入解压缩后的目录中，双击“eclipse.exe”即可打开Eclipse的工作界面。（注：Eclipse正常运行的前提是在系统变量中存在名为“JAVA_HOME”的变量，值为JDK的安装目录）。
# 2.   Maven的安装和配置
## 2.1.  安装
进入Maven官网的下载页面：[http://maven.apache.org/download.cgi](http://maven.apache.org/download.cgi)，如下图所示：
![](https://img-blog.csdn.net/20130614214529546)
选择当前最新版本：“apache-maven-3.0.4-bin.zip”，下载到本地，解压缩到本地磁盘D:下。
![](https://img-blog.csdn.net/20130614214610218)
## 2.2.  配置
进入Maven安装目录下的conf子目录中，打开settings.xml进行配置修改。
### 2.2.1.   修改默认的本地仓库位置
Maven默认的本地仓库位置是当前用户工作目录下的“.m2/repository”，使用过程中这个目录里的文件会比较多，占用空间越来越大。一般建议更换到其它磁盘目录下。如下配置，就把默认的本地仓库更改到D: /.m2/repository这个目录(这个目录结构需要自己创建好)。
<localRepository>d:/.m2/repository</localRepository>
### 2.2.2.   修改默认的中央仓库镜像
Maven默认的中央仓库里的文件不全。所以，都需要自行添加其它的镜像地址。在settings.xml文件中的“<mirrors>”标签里添加如下内容：
     <mirror>
       <id>ibiblio</id>
       <mirrorOf>central</mirrorOf>
       <name>Human Readable Name for this Mirror.</name>
       <url>http://mirrors.ibiblio.org/maven2/</url>
     </mirror>
保存所做的修改，同时还需要把这个settings.xml文件复制一份到“D:/.m2”目录下。
# 3.   Eclipse中m2eclipse插件的安装
Eclipse默认不支持Maven。需要给它添加m2eclipse插件。下面是具体的操作步骤。
## 3.1.  插件安装
依次选择Eclipse菜单栏的“Help”菜单à“Instance NewSoftWare”菜单项，就会弹出如下对话窗口：
![](https://img-blog.csdn.net/20130614214641843)
选择“Add..”按钮，又会弹出如下对话框：
![](https://img-blog.csdn.net/20130614214704968)
         这个对话框就是用于添加一个插件地址的。在“Name”对应的输入框里输入该操作的一个标识名。在“Location”对应的输入框里输入这个插件的安装地址。注：Maven的Eclipse插件地址为：[http://download.eclipse.org/technology/m2e/releases](http://download.eclipse.org/technology/m2e/releases)。输入后，如下图所示：
![](https://img-blog.csdn.net/20130614214726359)
输入完成后，点击右下角的“OK”按钮，就会弹出如下对话框：
![](https://img-blog.csdn.net/20130614214747140)
这里需要选择想要安装的插件的详细内容。选中“Maven Integration for Eclipse”前面的复选框。如下图所示：
![](https://img-blog.csdn.net/20130614214819656)
选择完成后，点击右下方的“Next>”按钮进行安装。安装成功后，会提示需要重启Eclipse来生效这次配置。重启之后，这个Eclipse就可以使用m2eclipse插件功能了。
## 3.2.  配置
插件安装好之后，还需要进行一些配置才能够开始使用Maven的功能。
### 3.2.1.   设置m2eclipse插件所关联的Maven程序
依次选择Eclipse菜单栏的“Window”à“Preferences”菜单项，就会弹出如下对话框：
![](https://img-blog.csdn.net/20130614214843390)
在这个对话框左边的导航栏中展开“Maven”目录结点，并选择“Installations”子节点，如下图所示：
![](https://img-blog.csdn.net/20130614214905906)
这里需要关联上对应的Maven安装程序。具体操作是，在右边面板中选择“Add…”按钮，会弹出如下选择Maven安装目录的选择对话框：
![](https://img-blog.csdn.net/20130614214937953)
通过这个对话框来选择Maven的具体安装目录。选择好之后就点击“确定”按钮。如下图所示：
![](https://img-blog.csdn.net/20130614215001218)
### 3.2.2.   设置自定义的本地仓库
选择如上图对话窗口左边菜单树 “Maven”节点下的“UserSettings”子节点，它默认的配置文件是“C:\Documents and Settings\csdn\.m2\settings.xml”，如下图所示：
![](https://img-blog.csdn.net/20130614215028656)
这边需要修改为我们自定义仓库位置下的settings.xml文件。具体操作是，点击“Browse…”按钮来选择上一步骤修改的本地仓库目录下的settings.xml文件，如下图所示：
![](https://img-blog.csdn.net/20130614215049140)
在上图中选择D:/.m2目录下的“settings.xml”文件，点击“打开”按钮来确认刚才的选择，就会回到如下图的对话窗口中：
![](https://img-blog.csdn.net/20130614215117296)
在上图中点击右下角的“OK”按钮来完成所有的配置修改。
# 4.   使用Maven来构建Web项目
## 4.1.  创建Maven的web项目
m2eclispe插件中默认提供了一些Maven样板项目，可以使用这些样板来创建符合我们要求的项目。具体操作如下步骤：
在eclipse菜单栏中选择“File”à“News”à“Other”菜单项，就会打开如下对话窗口，在窗口中选择“Maven”节点下“Maven Project”条目，如下图所示：
![](https://img-blog.csdn.net/20130614215146265)
点击“Next>”按钮，进入如下窗口：
![](https://img-blog.csdn.net/20130614215221781)
再次选择“Next >”按钮，进入如下窗口：
![](https://img-blog.csdn.net/20130614215245171)
在这个窗口中选择“maven-archetype-webapp”来创建一个Maven的web项目，再选择“Next>”按钮，进入如下图窗口：
![](https://img-blog.csdn.net/20130614215312296)
在上图窗口的“Group Id”中输入项目的基本包名，在“Artifact Id”中输入项目名，其它输入框不用理会。点击“Finish”按钮来完成项目的整个创建过程。
之后，Maven就会根据项目的配置去中央仓库下载相应的文件和jar包。完成后，就有如下项目目录结构：
![](https://img-blog.csdn.net/20130614215337906)
如上图所示，“pom.xml”文件就是这个项目的maven配置文件。打开此文件，并在“<build>”标签内添加如下配置：
<defaultGoal>install</defaultGoal>
<plugins>
         <plugin>
                   <groupId>org.apache.maven.plugins</groupId>
                   <artifactId>maven-surefire-plugin</artifactId>
                   <configuration>
                            <skip>true</skip>
                   </configuration>
         </plugin>
         <plugin>
                   <groupId>org.apache.maven.plugins</groupId>
                   <artifactId>maven-compiler-plugin</artifactId>
                   <version>2.5.1</version>
                   <configuration>
                            <source>1.6</source>
                            <target>1.6</target>
                            <encoding>UTF-8</encoding>
                   </configuration>
         </plugin>
</plugins>
这段配置中“<defaultGoal>”标签指定maven的默认操作是“install”；“maven-surefire-plugin”插件配置了“忽略编译项目时的测试运行”；“maven-compiler-plugin”插件指定了一些编译时的信息配置。
## 4.2.  使用Maven添加项目依赖包
[Java Web](http://lib.csdn.net/base/13)项目中还需要添加ServletAPI、JSP API以及JSTL的jar包，这可以直接在pom.xml文件中添加了。如下pom.xml文件中就添加了Servlet-API
 2.5版本、JSP-API2.1版本、JSTL 1.2版本：
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
         <modelVersion>4.0.0</modelVersion>
         <groupId>com.qiujy</groupId>
         <artifactId>testweb_svn</artifactId>
         <packaging>war</packaging>
         <version>0.0.1-SNAPSHOT</version>
         <name>testweb_svn Maven Webapp</name>
         <url>http://maven.apache.org</url>
         <dependencies>
                   <dependency>
                            <groupId>javax.servlet</groupId>
                            <artifactId>servlet-api</artifactId>
                            <version>2.5</version>
                            <scope>provided</scope>
                   </dependency>
                   <dependency>
                            <groupId>javax.servlet.jsp</groupId>
                            <artifactId>jsp-api</artifactId>
                            <version>2.1</version>
                            <scope>provided</scope>
                   </dependency>
                   <dependency>
                            <groupId>javax.servlet</groupId>
                            <artifactId>jstl</artifactId>
                            <version>1.2</version>
                   </dependency>
         </dependencies>
         <build>
                   <finalName>testweb_svn</finalName>
                   <defaultGoal>install</defaultGoal>
                   <plugins>
                            <plugin>
                                     <groupId>org.apache.maven.plugins</groupId>
                                     <artifactId>maven-surefire-plugin</artifactId>
                                     <configuration>
                                               <skip>true</skip>
                                     </configuration>
                            </plugin>
                   </plugins>
         </build>
</project>
这里说明一下：对Servlet和JSP的API包，在编译web项目时需要使用，而把web项目打包部署到web容器（如Tomcat）中运行时就不需要了。因此，这里把它们的<scope>配置为provided。
## 4.3.  把Maven项目转换成Eclipse工程
为了便于在Eclipse中开发JavaWeb项目，我们需要把Maven项目的目录结构转换成Eclipse工程的目录结构，具体按如下操作步骤进行。
选中项目在右键菜单中选择“Properties”，弹出如下图所示对话窗口：
![](https://img-blog.csdn.net/20130614215417765)
选中左边功能树中的“ProjectFacets”节点，然后在对应的右边面板中点击“Convert to faceted from…”这个超链接，右边面板就会切换成如下图所示：
![](https://img-blog.csdn.net/20130614215445890)
在“Project Facet”中进行相应配置，如下图所示：
![](https://img-blog.csdn.net/20130614215509984)
配置好之后，选择窗口右下角的“OK”按钮来保存配置。这时，这个Maven项目就会转换成Eclipse的web工程目录结构。
![](https://img-blog.csdn.net/20130614215532984)
如果还需要直接在Eclipse中运行这个web工程，还需要把Maven依赖包添加到本项目类路径中：
![](https://img-blog.csdn.net/20130614215559875)
还需要添加部署到Tomcat时的依赖包，如下图：
![](https://img-blog.csdn.net/20130614215759578)
![](https://img-blog.csdn.net/20130614215840687)
![](https://img-blog.csdn.net/20130614215906671)
点击“Finish”来完成后，对话框如下图所示：
![](https://img-blog.csdn.net/20130614215944265)
之后点击“OK”按钮来完成这些设置。
## 4.4.  把web.xml修改成相应版本
由于Maven中的“maven-archetype-webapp”样板项目的“web.xml”是2.3版本的，我们需要替换成新版本：可以把Eclipse的web工程webContent/WEB-INF目录下的“web.xml”文件复制替换掉“src/main/webapp/WEB-INF/”目录下的web.xml文件。
注意：由于Maven项目编译和发布时，它只是把“src”目录下的资源压缩到一个war包中。而使用Eclipse-JEE开发web项目时，都是把页面资源（html/css/js/img/JSP）放置在WebContent目录下。所以，在使用Maven来编译和发布项目前，需要把WebContent目录下的资源复制到“src/main/webapp/”目录下。
# 5.   使用Maven编译和发布项目
在项目中的“pom.xml”文件上点击右键，在弹出的菜单中选择“Run AS”à“Maveninstall”来编译和生成项目。如下图所示：
![](https://img-blog.csdn.net/20130614220021265)
         在编译和生成过程中，可以在“Console”面板中可以看到Maven编译过程的信息，如果编译和生成成功，可以在“Console”面板看到“BUILD SUCCESS”字样。
         另外，在修改pom.xml文件中的相关jar包信息时，可能由于仓库中不存在，就会出现一些错误，这时，项目编译就可能存在一些问题。此时可通过“Maven clean”选项来清除编译，然后再修改pom.xml到正确情况。再通过“Maven install”来编译和生成。
# 6.   示例项目代码
# 7.   附文件
如下是一个SpringMVC3+Spring3+Hibernate4的web项目中的pom.xml文件：
<projectxmlns=*"http://maven.apache.org/POM/4.0.0"*xmlns:xsi=*"http://www.w3.org/2001/XMLSchema-instance"*
xsi:schemaLocation=*"http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd"*>
<modelVersion>4.0.0</modelVersion>
<groupId>com.qiujy</groupId>
<artifactId>site2013</artifactId>
<packaging>war</packaging>
<version>0.0.1-SNAPSHOT</version>
<name>site2013 Maven Webapp</name>
<url>http://maven.apache.org</url>
<properties>
<spring.version>3.2.3.RELEASE</spring.version>
<hibernate.version>4.2.2.Final</hibernate.version>
<slf4j.version>1.7.5</slf4j.version>
<jackson.version>2.2.2</jackson.version>
</properties>
<dependencies>
<!-- servlet/jsp api -->
<dependency>
<groupId>javax.servlet</groupId>
<artifactId>servlet-api</artifactId>
<version>2.5</version>
<scope>provided</scope>
</dependency>
<dependency>
<groupId>javax.servlet.jsp</groupId>
<artifactId>jsp-api</artifactId>
<version>2.1</version>
<scope>provided</scope>
</dependency>
<!-- jstl -->
<dependency>
<groupId>javax.servlet</groupId>
<artifactId>jstl</artifactId>
<version>1.2</version>
</dependency>
       <!-- javaxmail
       <dependency>
           <groupId>javax.mail</groupId>
           <artifactId>mail</artifactId>
           <version>1.4.5</version>
        </dependency>-->
<!-- slf4j -->
<dependency>
<groupId>org.slf4j</groupId>
<artifactId>slf4j-log4j12</artifactId>
<version>${slf4j.version}</version>
</dependency>
<!-- junit -->
<dependency>
<groupId>junit</groupId>
<artifactId>junit</artifactId>
<version>4.8.2</version>
</dependency>
<!-- proxool <dependency> <groupId>proxool</groupId> <artifactId>proxool</artifactId>
           <version>0.8.3</version> </dependency> -->
<!-- [MySQL](http://lib.csdn.net/base/14) driver -->
<dependency>
<groupId>mysql</groupId>
<artifactId>mysql-connector-java</artifactId>
<version>5.1.22</version>
</dependency>
<!-- sql server数据库驱动 <dependency> <groupId>net.sourceforge.jtds</groupId>
           <artifactId>jtds</artifactId> <version>1.3.0</version> </dependency> -->
<!-- oracle数据库驱动：maven仓库中的ojdbc下载需要oracle的官方授权，先在网上找一个ojdbc14.jar包，上传到本地的maven仓库
           mvn install:install-file -Dfile=D:/resources/ojdbc/ojdbc14.jar -DgroupId=com.oracle
           -DartifactId=ojdbc14 -Dversion=10.2.0.1.0 -Dpackaging=jar <dependency> <groupId>com.oracle</groupId>
           <artifactId>ojdbc14</artifactId> <version>10.2.0.4.0</version> </dependency> -->
<!-- fileupload -->
<dependency>
<groupId>commons-fileupload</groupId>
<artifactId>commons-fileupload</artifactId>
<version>1.2.2</version>
</dependency>
<!-- io -->
<dependency>
<groupId>commons-io</groupId>
<artifactId>commons-io</artifactId>
<version>2.4</version>
</dependency>
<!-- c3p0 -->
<dependency>
<groupId>c3p0</groupId>
<artifactId>c3p0</artifactId>
<version>0.9.1.2</version>
</dependency>
<!-- struts2 <dependency> <groupId>org.apache.struts</groupId> <artifactId>struts2-core</artifactId>
           <version>${struts.version}</version> <exclusions> <exclusion> <artifactId>javassist</artifactId>
           <groupId>javassist</groupId> </exclusion> </exclusions> </dependency> <dependency>
           <groupId>org.apache.struts</groupId> <artifactId>struts2-spring-plugin</artifactId>
           <version>${struts.version}</version> <exclusions> <exclusion> <artifactId>spring-core</artifactId>
           <groupId>org.springframework</groupId> </exclusion> </exclusions> </dependency> -->
<!-- spring3 -->
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-beans</artifactId>
<version>${spring.version}</version>
</dependency>
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-context</artifactId>
<version>${spring.version}</version>
</dependency>
<!-- spring需要的aspectj依赖包 -->
<dependency>
<groupId>org.aspectj</groupId>
<artifactId>aspectjweaver</artifactId>
<version>1.7.2</version>
</dependency>
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-tx</artifactId>
<version>${spring.version}</version>
</dependency>
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-jdbc</artifactId>
<version>${spring.version}</version>
</dependency>
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-orm</artifactId>
<version>${spring.version}</version>
</dependency>
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-context-support</artifactId>
<version>${spring.version}</version>
</dependency>
<!-- spring web -->
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-web</artifactId>
<version>${spring.version}</version>
</dependency>
<!-- spring mvc -->
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-webmvc</artifactId>
<version>${spring.version}</version>
</dependency>
<!-- <dependency> <groupId>org.quartz-scheduler</groupId> <artifactId>quartz</artifactId>
           <version>2.1.6</version> </dependency> -->
<!-- hibernate4 -->
<dependency>
<groupId>org.hibernate</groupId>
<artifactId>hibernate-core</artifactId>
<version>${hibernate.version}</version>
<exclusions>
<exclusion>
<groupId>org.slf4j</groupId>
<artifactId>slf4j-api</artifactId>
</exclusion>
</exclusions>
</dependency>
<dependency>
<groupId>org.hibernate</groupId>
<artifactId>hibernate-ehcache</artifactId>
<version>${hibernate.version}</version>
</dependency>
<dependency>
<groupId>com.fasterxml.jackson.core</groupId>
<artifactId>jackson-databind</artifactId>
<version>${jackson.version}</version>
</dependency>
</dependencies>
<!-- ========================================================== -->
<build>
<finalName>site2013</finalName>
<defaultGoal>install</defaultGoal>
<plugins>
<plugin>
              <groupId>org.apache.maven.plugins</groupId>
              <artifactId>maven-surefire-plugin</artifactId>
<configuration>
<skip>true</skip>
</configuration>
</plugin>
<plugin>
<groupId>org.apache.maven.plugins</groupId>
<artifactId>maven-compiler-plugin</artifactId>
<version>2.5.1</version>
<configuration>
<source>1.6</source>
<target>1.6</target>
<encoding>UTF-8</encoding>
</configuration>
</plugin>
</plugins>
</build>
</project>
