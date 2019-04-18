# 打包Spring Boot工程 - z69183787的专栏 - CSDN博客
2015年06月16日 22:19:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：23114

在intellij idea 中，需要在maven中 新建 Run configuration，对应的command-line 为 ：clean install spring-boot:repackage
Pom.xml代码  ![收藏代码](http://mrlee23.iteye.com/images/icon_star.png)
- <project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-     xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">  
-     <modelVersion>4.0.0</modelVersion>  
-     <groupId>liwei.test</groupId>  
-     <artifactId>spring-boot</artifactId>  
-     <version>0.0.1-SNAPSHOT</version>  
-     <packaging>jar</packaging>  
-     <properties>  
-         <java.version>1.7</java.version>  
-         <start-class>com.test.springboot.Startup</start-class>  
-     </properties>  
-     <!-- Inherit defaults from Spring Boot -->  
-     <parent>  
-         <groupId>org.springframework.boot</groupId>  
-         <artifactId>spring-boot-starter-parent</artifactId>  
-         <version>1.0.1.RELEASE</version>  
-     </parent>  
- 
-     <!-- Add typical dependencies for a web application -->  
-     <dependencies>  
-         <dependency>  
-             <groupId>org.springframework.boot</groupId>  
-             <artifactId>spring-boot-starter-web</artifactId>  
-         </dependency>  
-     </dependencies>  
- 
-     <build>  
-         <pluginManagement>  
-             <plugins>  
-                 <plugin>  
-                     <artifactId>maven-compiler-plugin</artifactId>  
-                     <configuration>  
-                         <source>1.7</source>  
-                         <target>1.7</target>  
-                     </configuration>  
-                 </plugin>  
-             </plugins>  
-         </pluginManagement>  
-     </build>  
- 
-     <repositories>  
-         <repository>  
-             <id>spring-snapshots</id>  
-             <url>http://repo.spring.io/snapshot</url>  
-             <snapshots>  
-                 <enabled>true</enabled>  
-             </snapshots>  
-         </repository>  
-         <repository>  
-             <id>spring-milestones</id>  
-             <url>http://repo.spring.io/milestone</url>  
-         </repository>  
-     </repositories>  
-     <pluginRepositories>  
-         <pluginRepository>  
-             <id>spring-snapshots</id>  
-             <url>http://repo.spring.io/snapshot</url>  
-         </pluginRepository>  
-         <pluginRepository>  
-             <id>spring-milestones</id>  
-             <url>http://repo.spring.io/milestone</url>  
-         </pluginRepository>  
-     </pluginRepositories>  
- </project>  
配置Maven Build-Run Configurations
Goals:org.apache.maven.plugins:maven-jar-plugin:2.4:jar org.springframework.boot:spring-boot-maven-plugin:1.0.1.RELEASE:repackage
必须选择先maven后spring-boot-maven
如果这个地方不配置的话，会出错：
Java代码  ![收藏代码](http://mrlee23.iteye.com/images/icon_star.png)
- [ERROR] Failed to execute goal org.springframework.boot:spring-boot-maven-plugin:1.0.1.RELEASE:repackage (default-cli) on project spring-boot: Execution default-cli of goal org.springframework.boot:spring-boot-maven-plugin:1.0.1.RELEASE:repackage failed: Source must refer to an existing file -> [Help 1]  
- [ERROR]   
- [ERROR] To see the full stack trace of the errors, re-run Maven with the -e switch.  
- [ERROR] Re-run Maven using the -X switch to enable full debug logging.  
- [ERROR]   
- [ERROR] For more information about the errors and possible solutions, please read the following articles:  
- [ERROR] [Help 1] http://cwiki.apache.org/confluence/display/MAVEN/PluginExecutionException
