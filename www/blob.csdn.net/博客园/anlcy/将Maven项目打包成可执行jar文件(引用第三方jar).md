
# 将Maven项目打包成可执行jar文件(引用第三方jar) - anlcy - 博客园






# [将Maven项目打包成可执行jar文件(引用第三方jar)](https://www.cnblogs.com/camilla/p/8334148.html)

#### 方法一. mvn assembly 或 mvn package (一个jar包)
把依赖包和自己项目的文件打包如同一个jar包(这种方式对spring的项目不支持)
<build>
<plugins>
<plugin>
<artifactId>maven-assembly-plugin</artifactId>
<configuration>
<archive>
<manifest>
<mainClass>com.tang.CSVUtils</mainClass>
</manifest>
</archive>
<descriptorRefs>
<descriptorRef>
jar-with-dependencies
</descriptorRef>
</descriptorRefs>
</configuration>
<!-- 加上下面这一段,使用 mvn package命令,不加则使用mvn assembly-->
<!-- <executions>
<execution>
<id>make-assemble</id>
<phase>package</phase>
<goals>
<goal>single</goal>
</goals>
</execution>
</executions> -->
</plugin>
</plugins>
</build>
Eclipse 中 Run As--- Maven clean ---Maven assembly:assembly（或 Maven package）
Target目录生成如下文件：
![](http://img.blog.csdn.net/20150822110450932)
其中，
testLog4j-0.1.jar 是不可直接运行的，因为没有带第三方包。
testLog4j-0.1-jar-with-dependencies.jar 就是带有第三方包的可执行 jar 包，
在命令行执行 java -jar testLog4j-0.1-jar-with-dependencies.jar 即可运行
#### 方法二：mvn package(一个jar包和一个lib文件夹)
<build>
<plugins>
<plugin>
<groupId>org.apache.maven.plugins</groupId>
<artifactId>maven-jar-plugin</artifactId>
<version>2.4</version>
<configuration>
<archive>
<manifest>
<addClasspath>true</addClasspath>
<classpathPrefix>lib/</classpathPrefix>
<mainClass>com.tang.CSVUtils</mainClass>
</manifest>
</archive>
</configuration>
</plugin>
<plugin>
<groupId>org.apache.maven.plugins</groupId>
<artifactId>maven-dependency-plugin</artifactId>
<executions>
<execution>
<id>copy</id>
<phase>package</phase>
<goals>
<goal>copy-dependencies</goal>
</goals>
<configuration>
<outputDirectory>${project.build.directory}/lib</outputDirectory>
</configuration>
</execution>
</executions>
</plugin>
</plugins>
</build>
执行maven package 进行打包进入target目录。
其中lib就是第三方jar包的目录，在其他项目引入testLog4j-0.1.jar会自动找到并引入lib里面的包.
只需要拷贝这两个（lib和testLog4j-0.1.jar）即可放到其他地方用了。
在命令行执行：java -jar testLog4j-0.1.jar
![](http://img.blog.csdn.net/20150822111848938)
方法三：mvn package(推荐)(一个zip包，里面有一个jar包，一个lib文件夹，和一个conf文件夹)
(1)conf/package.xml (conf 文件夹和pom.xml在同一级目录)
<assembly>
<id>bin</id>
<!-- 最终打包成一个用于发布的zip文件 -->
<formats>
<format>zip</format>
</formats>
<!-- Adds dependencies to zip package under lib directory -->
<dependencySets>
<dependencySet>
<!--
不使用项目的artifact，第三方jar不要解压，打包进zip文件的lib目录
-->
<useProjectArtifact>false</useProjectArtifact>
<outputDirectory>lib</outputDirectory>
<unpack>false</unpack>
</dependencySet>
</dependencySets>
<fileSets>
<!-- 把项目相关的说明文件，打包进zip文件的根目录 -->
<fileSet>
<directory>${project.basedir}</directory>
<outputDirectory>/</outputDirectory>
<includes>
<include>README*</include>
<include>LICENSE*</include>
<include>NOTICE*</include>
</includes>
</fileSet>
<!-- 把项目的配置文件，打包进zip文件的config目录 -->
<fileSet>
<directory>${project.basedir}\conf</directory>
<outputDirectory>conf</outputDirectory>
<includes>
<include>*.xml</include>
<include>*.properties</include>
<include>*.key</include>
</includes>
</fileSet>
<!-- 把项目的脚本文件目录（ src/main/scripts ）中的启动脚本文件，打包进zip文件的跟目录 -->
<fileSet>
<directory>${project.build.scriptSourceDirectory}</directory>
<outputDirectory></outputDirectory>
<includes>
<include>startup.*</include>
</includes>
</fileSet>
<!-- 把项目自己编译出来的jar文件，打包进zip文件的根目录 -->
<fileSet>
<directory>${project.build.directory}</directory>
<outputDirectory></outputDirectory>
<includes>
<include>*.jar</include>
</includes>
</fileSet>
</fileSets>
</assembly>
(2)pom.xml:
<build>
<plugins>
<plugin>
<groupId>org.apache.maven.plugins</groupId>
<artifactId>maven-compiler-plugin</artifactId>
<version>3.1</version>
<configuration>
<compilerVersion>1.6</compilerVersion>
<source>1.6</source>
<target>1.6</target>
</configuration>
</plugin>
<!-- The configuration of maven-jar-plugin -->
<plugin>
<groupId>org.apache.maven.plugins</groupId>
<artifactId>maven-jar-plugin</artifactId>
<version>2.4</version>
<!-- The configuration of the plugin -->
<configuration>
<!-- Configuration of the archiver -->
<archive>
<!-- do not include pom.xml and pom.properties in the jar package -->
<addMavenDescriptor>false</addMavenDescriptor>
<!-- Manifest specific configuration -->
<manifest>
<!-- put third party jar package into the classpath of manifest -->
<addClasspath>true</addClasspath>
<!-- the prefix of the jar items in the classpath, it depends on the location(folder) of jar files -->
<classpathPrefix>lib/</classpathPrefix>
<!-- main class of the jar package-->
<mainClass>com.tang.your-Main-class</mainClass>
</manifest>
</archive>
<!-- excludes some files -->
<excludes>
<exclude>${project.basedir}/xml/*</exclude>
</excludes>
</configuration>
</plugin>
<!-- The configuration of maven-assembly-plugin -->
<plugin>
<groupId>org.apache.maven.plugins</groupId>
<artifactId>maven-assembly-plugin</artifactId>
<version>2.4</version>
<!-- The configuration of the plugin -->
<configuration>
<!-- Specifies the configuration file of the assembly plugin -->
<descriptors>
<descriptor>conf/package.xml</descriptor>
</descriptors>
</configuration>
<executions>
<execution>
<id>make-assembly</id>
<phase>package</phase>
<goals>
<goal>single</goal>
</goals>
</execution>
</executions>
</plugin>
</plugins>
</build>
mvn install后，此时会在target目录下生成一个zip文件，如：
youArtifactId-0.0.1-bin.zip 解压后有：
conf 文件夹， lib文件夹，  youArtifactId-0.0.1-bin.jar
博客来源：http://blog.csdn.net/qq804702802/article/details/47838241






