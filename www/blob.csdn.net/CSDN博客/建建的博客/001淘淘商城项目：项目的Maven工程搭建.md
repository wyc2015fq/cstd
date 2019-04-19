# 001淘淘商城项目：项目的Maven工程搭建 - 建建的博客 - CSDN博客
2018年01月01日 18:43:25[纪建](https://me.csdn.net/u013898698)阅读数：60
个人分类：[java学习路线](https://blog.csdn.net/u013898698/article/category/7152763)
开始一个新的项目，特此记录，资料全部来源于传智播客，感谢。
我们要做一个类似电商的项目。用maven做管理。
maven里面主要分为三种工程：
1：pom工程：用在父级工程，聚合工程中
2：war工程：主要用作网站。
3：jar工程：就是当做jar用的。
先给出总的项目结构：
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161225233026870-1623892874.png)
解释如下：
1：taotao-parent
taotao-parent公司级别的maven工程。主要做的功能是统一公司做各种项目时用的jar包的版本。比如我们做淘淘商城，或者别的什么项目都要去继承这个项目。
注意我们选择的是Pom工程。
这个工程建立步骤如下：
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161225233337229-1797343470.png)
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161225233348307-1360199900.png)
然后我们修改pom.xml文件，如下：
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.taotao</groupId>
  <artifactId>taotao-parent</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>pom</packaging>
      <!-- 集中定义依赖版本号 -->
    <properties>
        <junit.version>4.12</junit.version>
        <spring.version>4.1.3.RELEASE</spring.version>
        <mybatis.version>3.2.8</mybatis.version>
        <mybatis.spring.version>1.2.2</mybatis.spring.version>
        <mybatis.paginator.version>1.2.15</mybatis.paginator.version>
        <mysql.version>5.1.32</mysql.version>
        <slf4j.version>1.6.4</slf4j.version>
        <jackson.version>2.4.2</jackson.version>
        <druid.version>1.0.9</druid.version>
        <httpclient.version>4.3.5</httpclient.version>
        <jstl.version>1.2</jstl.version>
        <servlet-api.version>2.5</servlet-api.version>
        <jsp-api.version>2.0</jsp-api.version>
        <joda-time.version>2.5</joda-time.version>
        <commons-lang3.version>3.3.2</commons-lang3.version>
        <commons-io.version>1.3.2</commons-io.version>
        <commons-net.version>3.3</commons-net.version>
        <pagehelper.version>3.4.2-fix</pagehelper.version>
        <jsqlparser.version>0.9.1</jsqlparser.version>
        <commons-fileupload.version>1.3.1</commons-fileupload.version>
        <jedis.version>2.7.2</jedis.version>
        <solrj.version>4.10.3</solrj.version>
    </properties>
    <!-- 只定义依赖的版本，并不实际依赖 -->
  
  <!-- 只定义版本，并不实际依赖-->
   <dependencyManagement>
  <dependencies>
            <!-- 时间操作组件 -->
            <dependency>
                <groupId>joda-time</groupId>
                <artifactId>joda-time</artifactId>
                <version>${joda-time.version}</version>
            </dependency>
            <!-- Apache工具组件 -->
            <dependency>
                <groupId>org.apache.commons</groupId>
                <artifactId>commons-lang3</artifactId>
                <version>${commons-lang3.version}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.commons</groupId>
                <artifactId>commons-io</artifactId>
                <version>${commons-io.version}</version>
            </dependency>
            <dependency>
                <groupId>commons-net</groupId>
                <artifactId>commons-net</artifactId>
                <version>${commons-net.version}</version>
            </dependency>
            <!-- Jackson Json处理工具包 -->
            <dependency>
                <groupId>com.fasterxml.jackson.core</groupId>
                <artifactId>jackson-databind</artifactId>
                <version>${jackson.version}</version>
            </dependency>
            <!-- httpclient -->
            <dependency>
                <groupId>org.apache.httpcomponents</groupId>
                <artifactId>httpclient</artifactId>
                <version>${httpclient.version}</version>
            </dependency>
            <!-- 单元测试 -->
            <dependency>
                <groupId>junit</groupId>
                <artifactId>junit</artifactId>
                <version>${junit.version}</version>
                <scope>test</scope>
            </dependency>
            <!-- 日志处理 -->
            <dependency>
                <groupId>org.slf4j</groupId>
                <artifactId>slf4j-log4j12</artifactId>
                <version>${slf4j.version}</version>
            </dependency>
            <!-- Mybatis -->
            <dependency>
                <groupId>org.mybatis</groupId>
                <artifactId>mybatis</artifactId>
                <version>${mybatis.version}</version>
            </dependency>
            <dependency>
                <groupId>org.mybatis</groupId>
                <artifactId>mybatis-spring</artifactId>
                <version>${mybatis.spring.version}</version>
            </dependency>
            <dependency>
                <groupId>com.github.miemiedev</groupId>
                <artifactId>mybatis-paginator</artifactId>
                <version>${mybatis.paginator.version}</version>
            </dependency>
            <dependency>
                <groupId>com.github.pagehelper</groupId>
                <artifactId>pagehelper</artifactId>
                <version>${pagehelper.version}</version>
            </dependency>
            <!-- MySql -->
            <dependency>
                <groupId>mysql</groupId>
                <artifactId>mysql-connector-java</artifactId>
                <version>${mysql.version}</version>
            </dependency>
            <!-- 连接池 -->
            <dependency>
                <groupId>com.alibaba</groupId>
                <artifactId>druid</artifactId>
                <version>${druid.version}</version>
            </dependency>
            <!-- Spring -->
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-context</artifactId>
                <version>${spring.version}</version>
            </dependency>
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-beans</artifactId>
                <version>${spring.version}</version>
            </dependency>
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-webmvc</artifactId>
                <version>${spring.version}</version>
            </dependency>
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-jdbc</artifactId>
                <version>${spring.version}</version>
            </dependency>
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-aspects</artifactId>
                <version>${spring.version}</version>
            </dependency>
            <!-- JSP相关 -->
            <dependency>
                <groupId>jstl</groupId>
                <artifactId>jstl</artifactId>
                <version>${jstl.version}</version>
            </dependency>
            <dependency>
                <groupId>javax.servlet</groupId>
                <artifactId>servlet-api</artifactId>
                <version>${servlet-api.version}</version>
                <scope>provided</scope>
            </dependency>
            <dependency>
                <groupId>javax.servlet</groupId>
                <artifactId>jsp-api</artifactId>
                <version>${jsp-api.version}</version>
                <scope>provided</scope>
            </dependency>
            <!-- 文件上传组件 -->
            <dependency>
                <groupId>commons-fileupload</groupId>
                <artifactId>commons-fileupload</artifactId>
                <version>${commons-fileupload.version}</version>
            </dependency>
            <!-- Redis客户端 -->
            <dependency>
                <groupId>redis.clients</groupId>
                <artifactId>jedis</artifactId>
                <version>${jedis.version}</version>
            </dependency>
            <!-- solr客户端 -->
            <dependency>
                <groupId>org.apache.solr</groupId>
                <artifactId>solr-solrj</artifactId>
                <version>${solrj.version}</version>
            </dependency>
        </dependencies>
  
  
  </dependencyManagement>
  
  <build>
        <finalName>${project.artifactId}</finalName>
        <plugins>
            <!-- 资源文件拷贝插件 -->
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-resources-plugin</artifactId>
                <version>2.7</version>
                <configuration>
                    <encoding>UTF-8</encoding>
                </configuration>
            </plugin>
            <!-- java编译插件 -->
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.2</version>
                <configuration>
                    <source>1.7</source>
                    <target>1.7</target>
                    <encoding>UTF-8</encoding>
                </configuration>
            </plugin>
        </plugins>
        <pluginManagement>
            <plugins>
                <!-- 配置Tomcat插件 -->
                <plugin>
                    <groupId>org.apache.tomcat.maven</groupId>
                    <artifactId>tomcat7-maven-plugin</artifactId>
                    <version>2.2</version>
                </plugin>
            </plugins>
        </pluginManagement>
    </build>
  
</project>
```
上面用的就是我们公司用的所有的jar包，注意这个taotao-parent的maven的工程的意义在于统一公司所有项目用的jar包的版本。所以他的特点就是不实现具体的jar包依赖，
只实现版本的控制：
 <!-- 只定义版本，并不实际依赖-->
   <dependencyManagement>
加了这句话，我们就可以只控制版本，不实现具体的依赖。我们知道Maven的依赖需要三个坐标：
```
<groupId></groupId>
<artifactId></artifactId>
<version></version>
```
如果我们的taotao项目继承了这个taotao-parent工程，我们以后在taotao里面去依赖jar包只需要两个坐标：
<groupId></groupId>
<artifactId></artifactId>
就不需要版本了。
2:taotao-comon:
这个工程是存放公用的工具类：比如什么单元测试啊，json包什么的。因为公司的项目不会只有我们这么一个taotao项目用这些工具类，别的项目也会有用的，所以我们呢把这些工具类统一出来。
建立过程如下：注意我们用的是jar.而且继承之前的taotao-parent工程。
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161225235553292-564486621.png)
 我们修改pom.xml文件：注意我们在引入jar是maven的三个坐标中<version></version>坐标已经不写了。因为我们继承了taotao-parent工程啊，已经指定了版本了。
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>com.taotao</groupId>
        <artifactId>taotao-parent</artifactId>
        <version>0.0.1-SNAPSHOT</version>
    </parent>
    <groupId>com.taotao</groupId>
    <artifactId>taotao-common</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <!-- jar包的依赖 -->
    <dependencies>
        <!-- 时间操作组件 -->
注意我们在引入jar是maven的三个坐标中<version></version>坐标已经不写了。因为我们继承了taotao-parent工程啊，已经指定了版本了
        <dependency>
            <groupId>joda-time</groupId>
            <artifactId>joda-time</artifactId>
        </dependency>
        <!-- Apache工具组件 -->
        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-lang3</artifactId>
        </dependency>
        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-io</artifactId>
        </dependency>
        <dependency>
            <groupId>commons-net</groupId>
            <artifactId>commons-net</artifactId>
        </dependency>
        <!-- Jackson Json处理工具包 -->
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
        </dependency>
        <!-- httpclient -->
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpclient</artifactId>
        </dependency>
        <!-- 单元测试 -->
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <scope>test</scope>
        </dependency>
        <!-- 日志处理 -->
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-log4j12</artifactId>
        </dependency>
    </dependencies>
</project>
```
好了，我们之前已经建立了两个工程。
3现在我们正式做taotao的工程：
我们这个taotao工程做成一个聚合工程：
建立一个父工程，然后在父工程里面建立model工程，分为pojo工程，mapper工程，service工程，和contral（web）工程。就是把每个模块都独立建成工程。项目依赖。
我么先讲taotao-manager的建立过程：注意选择的是pom工程。并且继承了taotao-parent工程。
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161226000612620-1043828273.png)
我们这个工程肯定是要依赖之前的taotao-comon工程的，所以我们在修改pom文件时要加入这句。
pom.xml文件如下：
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>com.taotao</groupId>
    <artifactId>taotao-parent</artifactId>
    <version>0.0.1-SNAPSHOT</version>
  </parent>
  <groupId>com.taotao</groupId>
  <artifactId>taotao-manager</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>pom</packaging>
  <dependencies>
  <dependency>
  <!-- 依赖taotao-comon这个工程，引入这三个坐标就好了 -->
      <groupId>com.taotao</groupId>
      <artifactId>taotao-comon</artifactId>
      <version>0.0.1-SNAPSHOT</version>
  </dependency>
  
  </dependencies>
  <modules>
      <module>taotao-manager-pojo</module>
      <module>taotao-manager-mapper</module>
      <module>taotao-manager-service</module>
      <module>taotao-manager-web</module>
  </modules>
</project>
```
好了，建立了这个taotao聚合工程之后我们就要根据里面的pojo,srvice,control模块来单独的建立maven工程了。
如下：
4:我们先建立taotao-manager-pojo工程：选择的是jar.
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161226001431401-953826355.png)
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161226001441386-1801476274.png)
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161226001459823-720416259.png)
 pom.xml文件：
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>com.taotao</groupId>
    <artifactId>taotao-manager</artifactId>
    <version>0.0.1-SNAPSHOT</version>
  </parent>
  <artifactId>taotao-manager-pojo</artifactId>
</project>
```
因为我们的pojo类不需要依赖什么jar包，所以不需要修改pom文件。
5:我们再创建taotao-manager-mapper项目：选择jar.
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161226002545136-380399302.png)
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161226002557948-416374744.png)
 修改pom.xml文件，因为mapper需要依赖Mybatsi什么的。
如下：
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>com.taotao</groupId>
        <artifactId>taotao-manager</artifactId>
        <version>0.0.1-SNAPSHOT</version>
    </parent>
    <artifactId>taotao-manager-mapper</artifactId>
    <!-- 依赖管理 -->
    <dependencies>
        <dependency>
            <groupId>com.taotao</groupId>
            <artifactId>taotao-manager-pojo</artifactId>
            <version>0.0.1-SNAPSHOT</version>
        </dependency>
        <!-- Mybatis -->
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
        </dependency>
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis-spring</artifactId>
        </dependency>
        <dependency>
            <groupId>com.github.miemiedev</groupId>
            <artifactId>mybatis-paginator</artifactId>
        </dependency>
        <dependency>
            <groupId>com.github.pagehelper</groupId>
            <artifactId>pagehelper</artifactId>
        </dependency>
        <!-- MySql -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
        </dependency>
        <!-- 连接池 -->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>druid</artifactId>
        </dependency>
    </dependencies>
</project>
```
我们看一下工程。发现里面多了好多的jar包：
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161226002837182-581477260.png)
 其中的taotao-comom是因为我们的taotao-manager-mapper是继承taotao-manager的。而taotao-manager是依赖taotao-common的。所以taotao-manager会有taotao-common里面的jar包。
6同理我们创建taotao-service工程：也是jar型的maven工程
taotao-service工程的pom.xml文件如下：
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>com.taotao</groupId>
    <artifactId>taotao-manager</artifactId>
    <version>0.0.1-SNAPSHOT</version>
  </parent>
  <artifactId>taotao-manager-service</artifactId>
  <dependencies>
  <!-- service要运行肯定要依赖Mapper啊 -->
  <dependency>
   <groupId>com.taotao</groupId>
   <artifactId>taotao-manager-mapper</artifactId>
    <version>0.0.1-SNAPSHOT</version>
  </dependency>
  <!-- 还要依赖Spring -->
      <!-- Spring -->
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-context</artifactId>
            </dependency>
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-beans</artifactId>
            </dependency>
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-webmvc</artifactId>
            </dependency>
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-jdbc</artifactId>
            </dependency>
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-aspects</artifactId>
            </dependency>
  </dependencies>
</project>
```
7:最后我们创建taotao-web其实也就是对应的Contral（控制层）。
如下：
我们选择的是war工程。因为我们最后聚合后要打成war包的，至少要有一个war工程。
如下：
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161226003412432-1475633487.png)
 修改Pom.xml文件。如下：
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>com.taotao</groupId>
        <artifactId>taotao-manager</artifactId>
        <version>0.0.1-SNAPSHOT</version>
    </parent>
    <artifactId>taotao-manager-web</artifactId>
    <packaging>war</packaging>
    <!-- 依赖管理 -->
    <dependencies>
        <dependency>
            <groupId>com.taotao</groupId>
            <artifactId>taotao-manager-service</artifactId>
            <version>0.0.1-SNAPSHOT</version>
        </dependency>
        <!-- JSP相关 -->
        <dependency>
            <groupId>jstl</groupId>
            <artifactId>jstl</artifactId>
        </dependency>
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>servlet-api</artifactId>
            <scope>provided</scope>
        </dependency>
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>jsp-api</artifactId>
            <scope>provided</scope>
        </dependency>
        <!-- 文件上传组件 -->
        <dependency>
            <groupId>commons-fileupload</groupId>
            <artifactId>commons-fileupload</artifactId>
        </dependency>
    </dependencies>
</project>
```
到这里我们的工程就搭建好了。
我们看一下效果：
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161226003753276-1802227246.png)
我们看到在taotao-manager这个父pom工程里面有四个文件。这是因为我们这四个都是model项目。是下面的子文件。
在taotao-manager的pom.xml有如下一段话：
```
<modules>
      <module>taotao-manager-pojo</module>
      <module>taotao-manager-mapper</module>
      <module>taotao-manager-service</module>
      <module>taotao-manager-web</module>
  </modules>
```
最后我们在taotao-manager-web这个工程下面创建一个WEB-INF文件和web.xml.(因为没有自动生成，所以我们要手工创建了)
![](http://images2015.cnblogs.com/blog/996738/201612/996738-20161226004145745-2095511834.png)
 web.xml内容如下:
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns="http://java.sun.com/xml/ns/javaee" xmlns:web="http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
    xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
    id="taotao" version="2.5">
    <display-name>taotao-manager</display-name>
    <welcome-file-list>
        <welcome-file>index.html</welcome-file>
        <welcome-file>index.htm</welcome-file>
        <welcome-file>index.jsp</welcome-file>
        <welcome-file>default.html</welcome-file>
        <welcome-file>default.htm</welcome-file>
        <welcome-file>default.jsp</welcome-file>
    </welcome-file-list>
</web-app>
```
到这里我们整个maven工程全部搭好了。
