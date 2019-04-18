# 编写自定义Maven2插件 - z69183787的专栏 - CSDN博客
2016年10月31日 14:42:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：448
### from:[http://jwu.iteye.com/blog/612944](http://jwu.iteye.com/blog/612944)
一、创建一个插件项目 
    > mvn archetype:create -DgroupId=org.sonatype.mavenbook.plugins -DartifactId=first-maven-plugin -DarchetypeGroupId=org.apache.maven.archetypes -DarchetypeArtifactId=maven-archetype-mojo 
    maven会自动到远程库去下载maven-archetype-mojo的插件； 
    创建成功会生成一个first-maven-plugin的文件夹，里有一个pom.xml文件，内容： 
Java代码 ![复制代码](http://cgp17.iteye.com/images/icon_copy.gif)
- <groupId>org.sonatype.mavenbook.plugins</groupId>   
- <artifactId>first-maven-plugin</artifactId>   
- <packaging>maven-plugin</packaging>   
- <version>1.0-SNAPSHOT</version>   
- <name>first-maven-plugin Maven Mojo</name>   
- <url>http://maven.apache.org</url> 
- 
- <dependencies>   
-     <dependency>   
-         <groupId>org.apache.maven</groupId>   
-         <artifactId>maven-plugin-api</artifactId>   
-         <version>2.0</version>   
-     </dependency>   
-     <dependency>   
-         <groupId>junit</groupId>   
-         <artifactId>junit</artifactId>   
-         <version>3.8.1</version>   
-         <scope>test</scope>   
-     </dependency>   
- </dependencies>  
Java代码  ![收藏代码](http://rept.iteye.com/images/icon_star.png)
- <groupId>org.sonatype.mavenbook.plugins</groupId>  
- <artifactId>first-maven-plugin</artifactId>  
- <packaging>maven-plugin</packaging>  
- <version>1.0-SNAPSHOT</version>  
- <name>first-maven-plugin Maven Mojo</name>  
- <url>http://maven.apache.org</url>
- 
- <dependencies>  
-     <dependency>  
-         <groupId>org.apache.maven</groupId>  
-         <artifactId>maven-plugin-api</artifactId>  
-         <version>2.0</version>  
-     </dependency>  
-     <dependency>  
-         <groupId>junit</groupId>  
-         <artifactId>junit</artifactId>  
-         <version>3.8.1</version>  
-         <scope>test</scope>  
-     </dependency>  
- </dependencies>  
二、创建一个MOJO 
   MOJO就是一个供插件调用处理的普通类。 
Java代码 ![复制代码](http://cgp17.iteye.com/images/icon_copy.gif)
- /** 
-  * Echos an object string to the output screen. 
-  * @goal echo 
-  */
- publicclass MyMojo extends AbstractMojo {
   
- /** 
-      * Any Object to print out. 
-      * @parameter expression="${echo.message}" default-value="Hello Maven World..." 
-      */
- private Object message;   
- 
- publicvoid execute() throws MojoExecutionException {
   
-         getLog().info(message.toString());   
-     }  
Java代码  ![收藏代码](http://rept.iteye.com/images/icon_star.png)
- /**
-  * Echos an object string to the output screen.
-  * @goal echo
-  */
- publicclass MyMojo extends AbstractMojo {  
- /**
-      * Any Object to print out.
-      * @parameter expression="${echo.message}" default-value="Hello Maven World..."
-      */
- private Object message;  
- 
- publicvoid execute() throws MojoExecutionException {  
-         getLog().info(message.toString());  
-     }  
} 
创建了一个MyMojo类，必须继承AbstractMojo类，实现execute方法，这个方法就是插件调用的入口； 
三、build, run自定义插件 
   > mvn clean install 
   插件运行遵循groupId:artifactId:version:goal格式； 
   > mvn org.sonatype.mavenbook.plugins:first-maven-plugin:1.0-SNAPSHOT:echo -Decho.message="My first Maven plugin" 
上面命令中，出来一个echo，这个就是goal，在MyMojo类里用注释定义@goal echo； 
可能上面的命令太长，怎样做到像archetype:create一样写法？可以定义前缀; 
四、定义前缀 
   在setting.xml文件加： 
Java代码 ![复制代码](http://cgp17.iteye.com/images/icon_copy.gif)
- <pluginGroups>   
-   <pluginGroup>org.sonatype.mavenbook.plugins</pluginGroup>   
- </pluginGroups>  
Java代码  ![收藏代码](http://rept.iteye.com/images/icon_star.png)
- <pluginGroups>  
-   <pluginGroup>org.sonatype.mavenbook.plugins</pluginGroup>  
- </pluginGroups>  
然后： 
    > mvn first:echo -Decho.message="My first Maven plugin" 
非常简单。 
如果插件的artifactId遵循maven-first-plugin，或者first-maven-plugin模式。Maven就会自动为你的插件赋予前缀first。 
${prefix}-maven-plugin, OR maven-${prefix}-plugin 
也可自定义前缀，在pom.xml加： 
Java代码 ![复制代码](http://cgp17.iteye.com/images/icon_copy.gif)
- <build>   
-   <plugins>   
-     <plugin>   
-       <artifactId>first-maven-plugin</artifactId>   
-       <version>2.3</version>   
-       <configuration>   
-         <goalPrefix>first</goalPrefix>   
-       </configuration>   
-     </plugin>   
-   </plugins>   
- </build>  
Java代码  ![收藏代码](http://rept.iteye.com/images/icon_star.png)
- <build>  
-   <plugins>  
-     <plugin>  
-       <artifactId>first-maven-plugin</artifactId>  
-       <version>2.3</version>  
-       <configuration>  
-         <goalPrefix>first</goalPrefix>  
-       </configuration>  
-     </plugin>  
-   </plugins>  
- </build>  
- 
