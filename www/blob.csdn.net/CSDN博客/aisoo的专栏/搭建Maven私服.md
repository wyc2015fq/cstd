# 搭建Maven私服 - aisoo的专栏 - CSDN博客
2012年12月12日 09:17:39[aisoo](https://me.csdn.net/aisoo)阅读数：588
用Nexus来搭建本地的Maven服务器
下载地址是：[http://nexus.sonatype.org/downloads/](http://nexus.sonatype.org/downloads/)
我下载的是：[nexus-oss-webapp-1.8.0.1-bundle.tar.gz](http://nexus.sonatype.org/downloads/nexus-oss-webapp-1.8.0.1-bundle.tar.gz)
**一、安装服务器**
解压下载的文件：
# tar xzvf nexus-oss-webapp-1.8.0.1-bundle.tar.gz
解压后目录：
nexus-oss-webapp-1.8.0.1 包含了nexus的运行环境和应用程序
sonatype-work 包含了你自己的配置和数据
启动nexus
进入nexus-oss-webapp-1.8.1目录
# cd nexus-oss-webapp-1.8.0.1/bin/jsw
根据自己机器选择版本
![](http://dl.iteye.com/upload/attachment/404781/5c304e1f-3610-33db-8f27-cc418389c9d8.jpg)
我选择的是 linux-x86-32
# cd linux-x86-32/
linux-x86-32目录结构 ：lib  nexus  nexus.pid  platform.conf  wrapper
nexus就是启动命令
# ./nexus
执行上面的命令，可以得到nexus命令的用法提示：start 命令启动，stop命令停止。下面启动nexus：
# ./nexus start
![](http://dl.iteye.com/upload/attachment/404785/5528c8d3-d158-34b9-8d42-f6d8c994ef2c.jpg)
以上表示启动成功！
通过log文件查看更详细的信息
# cd ~/nexus-oss-webapp-1.8.0.1/log
# tail -f wrapper.log
在log中可以看到nexus默认监听的端口是8081。那么我们就可以在浏览器中访问:http://host:8081/nexus
默认用户名是admin密码是admin123
![](http://dl.iteye.com/upload/attachment/404787/8b4884f3-9ea0-3aa6-8441-6946e8d7e2f4.jpg)
**二、配置Nexus**
由于在新搭建的nexus环境中只是一个空的仓库，所以第一步就是要和远程的Maven中心仓库进行同步。
![](http://dl.iteye.com/upload/attachment/404789/9de90a24-99f9-3b83-a37c-adf3601a8dcd.jpeg)
![](http://dl.iteye.com/upload/attachment/404791/8a7c41e9-9051-3d44-a2a3-d11527ee159f.png)
如果在Reindex之后，并没有同步到远程的仓库，可以检查每个仓库的设置。下面是Maven Central的设置：
![](http://dl.iteye.com/upload/attachment/404793/16f77ccc-346e-3809-b55e-ba424d940623.jpg)
**三、在项目中使用私服**
在完成了上面的配置后，就可以将项目中默认的Repository切换为本地的私服了，只需要在pom.xml中增加repositories就可以了：
Java代码  ![收藏代码](http://lichunlong0630.iteye.com/images/icon_star.png)
- <project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-   xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">  
-   <modelVersion>4.0.0</modelVersion>  
- 
-   <groupId>com.mycom.test</groupId>  
-   <artifactId>simple</artifactId>  
-   <version>0.0.1-SNAPSHOT</version>  
-   <packaging>jar</packaging>  
- 
-   <name>simple</name>  
-   <url>http://maven.apache.org</url>
- 
-   <repositories>    
-     <repository>    
-       <snapshots>    
-         <enabled>true</enabled>    
-       </snapshots>    
-       <id>public</id>    
-       <name>Public Repositories</name>    
-       <url>http://192.168.1.7:8081/nexus/content/groups/public/</url>  
-     </repository>    
-   </repositories>  
-   <pluginRepositories>    
-     <pluginRepository>    
-       <id>public</id>    
-       <name>Public Repositories</name>    
-       <url>http://192.168.1.7:8081/nexus/content/groups/public//</url>  
-     </pluginRepository>    
-   </pluginRepositories>    
-   <properties>  
-     <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>  
-   </properties>  
- 
-   <dependencies>  
-     <dependency>  
-       <groupId>junit</groupId>  
-       <artifactId>junit</artifactId>  
-       <version>3.8.1</version>  
-       <scope>test</scope>  
-     </dependency>  
-   </dependencies>  
- </project>  
将pom.xml保存后，再回过头来了看去nexus管理界面看，就会发现junit已经被下载到本地的nexus服务器中了。
