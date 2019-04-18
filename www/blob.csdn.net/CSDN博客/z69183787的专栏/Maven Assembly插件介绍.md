# Maven Assembly插件介绍 - z69183787的专栏 - CSDN博客
2015年09月03日 19:22:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1707
你是否想要创建一个包含脚本、配置文件以及所有运行时所依赖的元素（jar）Assembly插件能帮你构建一个完整的发布包。
Assembly插件会生成 “assemblies”， 此特性等同于的Maven 1 distribution plug-in.。该插件不仅支持创建二进制归档文件，也支持创建源码归档文件。这些assemblies定义在一个assembly描述符文件里。你可以选择自定义assembly描述符或者直接使用插件自带的[三个预定义描述符](http://maven.apache.org/plugins/maven-assembly-plugin/descriptor-refs.html)中的任何一个.
目前Assembly插件支持如下格式的归档文件:
- zip
- tar.gz
- tar.bz2
- jar
- dir
- war
- and any other format that the ArchiveManager has been configured for
Maven 2上使用assembly的简单步骤:
- 从预定义描述符里选择一个或者自己编写一个assembly描述符号。
- 工程的pom.xml里配置Assembly插件。
- 在工程根目录下运行”mvn assembly:assembly”命令 。
如何自定义assembly描述符，详见[Assembly Descriptor Format](http://maven.apache.org/plugins/maven-assembly-plugin/assembly.html).
**什么是Assembly?**
“assembly”是把一组文件、目录、依赖元素组装成一个归档文件. 比如, 假设一个 Maven project定义了一个JAR artifact，它包含控制台应用程序和Swing应用程序 。这样一个工程可以定义两套包含描述符，一套给给控制台应用，另一套给Swing应用程序，它们包含各自的脚本、目录和依赖。
Assembly Plugin的描述符可以定义任何一个文件或者目录归档方式。举个例子，如果的你的Maven 2工程包含”src/main/bin”这个目录，你可以指示Assembly插件复制“src/main/bin”目录下所有的文件到bin目录里（归档文件里的目录），并且可以修改它们的权限属性（UNIX mode）。见 [assembly
 descriptor](http://maven.apache.org/plugins/maven-assembly-plugin/assembly.html).
**The Maven Assembly Plugin**
Maven 2.0的Assembly插件目的是提供一个把工程依赖元素、模块、网站文档等其他文件存放到单个归档文件里。
使用任何一个预定义的描述符你可以轻松的构建一个发布包。这些描述符能处理一些常用的操作,如：把依赖的元素的归档到一个jar文件. 当然, 你可以自定义描述符来更灵活的控制依赖，模块，文件的归档方式。
maven-assembly-plugin : 是maven中针对打包任务而提供的标准插件
**(1)、在pom.xml 文件里面的配置说明**
Java代码  ![收藏代码](http://blueram.iteye.com/images/icon_star.png)
- <plugin>  
-     <artifactId>maven-assembly-plugin</artifactId>  
-     <executions>  <!--执行器 mvn assembly:assembly-->  
-         <execution>  
-             <id>make-zip</id><!--名字任意 -->    
-         <phase>package</phase><!-- 绑定到package生命周期阶段上 -->    
-         <goals>    
-            <goal>single</goal><!-- 只运行一次 -->    
-         </goals>    
-             <configuration>  
-                      <descriptors> <!--描述文件路径-->  
-                           <descriptor>src/main/resources/zip.xml</descriptor>  
-                     </descriptors>  
-             </configuration>  
-         </execution>  
-     </executions>  
-  </plugin>  
****(2)、**zip.xml 文件配置如下**
Xml代码  ![收藏代码](http://blueram.iteye.com/images/icon_star.png)
- <assembly
- xmlns="http://maven.apache.org/plugins/maven-assembly-plugin/assembly/1.1.0"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/plugins/maven-assembly-plugin/assembly/1.1.0 http://maven.apache.org/xsd/assembly-1.1.0.xsd">
- <id>release</id>
- <formats>
- <format>zip</format>
- </formats>
- <fileSets>
- <fileSet>
- <directory>${project.basedir}\src\main\config</directory>
- <!-- 过滤 -->
- <excludes>
- <exclude>*.xml</exclude>
- </excludes>
- <outputDirectory>\</outputDirectory>
- </fileSet>
- </fileSets>
- 
- <dependencySets>
- <dependencySet>
- <useProjectArtifact>true</useProjectArtifact>
- <outputDirectory>lib</outputDirectory><!-- 将scope为runtime的依赖包打包到lib目录下。 -->
- <scope>runtime</scope>
- </dependencySet>
- </dependencySets>
- </assembly>
****(3)、**zip.xml 格式属性说明**
打包的文件格式
可以有：tar.zip war zip
<formats>
 <format>zip</format>
</formats>
需要打包的路径
<directory>${project.basedir}</directory>
打包后输出的路径
<outputDirectory>/</outputDirectory>
打包需要包含的文件
 <excludes>
        <exclude>junit:junit</exclude>
        <exclude>commons-lang:commons-lang</exclude>
        <exclude>commons-logging:commons-logging</exclude>
</excludes>
当前项目构件是否包含在这个依赖集合里。
<useProjectArtifact>true</useProjectArtifact>
依赖包打包到目录下
<dependencySets>
  <dependencySet>
   <outputDirectory>lib</outputDirectory><!-- 将scope为runtime的依赖包打包到lib目录下。 -->
   <useProjectArtifact>true</useProjectArtifact>
   <scope>runtime</scope>
  </dependencySet>
</dependencySets>
