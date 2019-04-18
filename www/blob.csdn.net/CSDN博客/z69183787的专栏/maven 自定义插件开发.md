# maven 自定义插件开发 - z69183787的专栏 - CSDN博客
2016年10月31日 14:39:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6097
1、
创建maven工程
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.qizj.mu</groupId>
  <artifactId>maven_plugin_test</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>maven-plugin</packaging>
  <name>maven_plugin_test Maven Plugin</name>
  <!-- FIXME change it to the project's website -->
  <url>http://maven.apache.org</url>
  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  </properties>
  <dependencies>
    <dependency>
      <groupId>org.apache.maven</groupId>
      <artifactId>maven-plugin-api</artifactId>
      <version>2.0</version>
    </dependency>
    <dependency>
      <groupId>org.apache.maven.plugin-tools</groupId>
      <artifactId>maven-plugin-annotations</artifactId>
      <version>3.2</version>
      <scope>provided</scope>
    </dependency>
    <dependency>
      <groupId>org.codehaus.plexus</groupId>
      <artifactId>plexus-utils</artifactId>
      <version>3.0.8</version>
    </dependency>
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.8.2</version>
      <scope>test</scope>
    </dependency>
  </dependencies>
  <build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-plugin-plugin</artifactId>
        <version>3.2</version>
        <configuration>
          <goalPrefix>zhangsan</goalPrefix>
        </configuration>
        <executions>
          <execution>
            <id>mojo-descriptor</id>
            <goals>
              <goal>descriptor</goal>
            </goals>
          </execution>
          <execution>
            <id>help-goal</id>
            <goals>
              <goal>helpmojo</goal>
            </goals>
          </execution>
        </executions>
      </plugin>
    </plugins>
  </build>
</project>
```
创建插件类
```java
package com.plugin;
import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;
/**
 * @goal generator
 */
public class GeneratorCode extends AbstractMojo {
    @Override
    public void execute() throws MojoExecutionException {
        getLog().info(“11111”);
    }
}
```
发布maven install
在其他的maven项目中添加plugin
```
<build>
    <plugins>
        <plugin>
            <groupId>com.qizj.mu</groupId>
            <artifactId>maven_plugin_test</artifactId>
            <version>0.0.1-SNAPSHOT</version>
        </plugin>
    </plugins>
</build>
```
右键项目 run as --> maven build -->goals中输入zhangsan:generator-->run
```
[INFO] Scanning for projects...
[INFO]                                                                         
[INFO] ------------------------------------------------------------------------
[INFO] Building Io 0.0.1-SNAPSHOT
[INFO] ------------------------------------------------------------------------
[INFO] 
[INFO] --- maven_plugin_test:0.0.1-SNAPSHOT:generator (default-cli) @ Io ---
[INFO] 111
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 0.365 s
[INFO] Finished at: 2016-08-04T13:13:52+08:00
[INFO] Final Memory: 6M/123M
[INFO] ------------------------------------------------------------------------
```
2、
首先导入maven的api
```
<dependencies>
      <dependency>
          <groupId>org.apache.maven</groupId>
          <artifactId>maven-plugin-api</artifactId>
          <version>3.3.9</version>
      </dependency>
  </dependencies>
```
修改POM文件的packaging为maven-plugin
`<packaging>maven-plugin</packaging>`
继承AbstractMojo 并重写execute方法
```java
import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
/**
 * @goal CustomMavenMojo
 * @phase compile
 * @requiresProject false
 */
public class CustomMavenMojo extends AbstractMojo {
    /**
     * @parameter expression="${name}"
     * @required
     */
    private String name;
    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {
        getLog().info("自定义插件成功,参数name:" + name);
    }
}
```
其中注释就是maven插件很重要的元数据
/**
 * @goal CustomMavenMojo：表示该插件的服务目标
 * @phase compile：表示该插件的生效周期阶段
 * @requiresProject false：表示是否依托于一个项目才能运行该插件
 * @parameter expression="${name}"：表示插件参数，使用插件的时候会用得到
 * @required:代表该参数不能省略
 */
4.install项目并测试
mvn install项目之后
再执行
com.wangnian:Custom
`com.wangnian:CustomMaven-plugin:CustomMavenMojo -Dname=wangnian`
com.wangnian  包名
CustomMaven-plugin maven 项目名
CustomMavenMojo 注释上的 @goal制定的名
-Dname=wangnian 注释@parameter expression="${name}制定的参数
![](http://static.oschina.net/uploads/space/2016/1014/131253_eePG_2408834.png)
在其他项目中依赖该插件
```
<plugin>
	<groupId>com.wangnian</groupId>
	<artifactId>CustomMaven-plugin</artifactId>
          <version>1.0</version>
	<executions>
		<execution>
			<!--<phase>process-classes</phase>-->
			<goals>
			    <goal>CustomMavenMojo</goal>
		    </goals>
		</execution>
	</executions>
</plugin>
```
