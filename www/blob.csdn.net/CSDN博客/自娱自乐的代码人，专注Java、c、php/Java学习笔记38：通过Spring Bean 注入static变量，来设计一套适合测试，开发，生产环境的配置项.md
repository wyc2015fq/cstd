# Java学习笔记38：通过Spring Bean 注入static变量，来设计一套适合测试，开发，生产环境的配置项 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年04月22日 15:38:55[initphp](https://me.csdn.net/initphp)阅读数：14621
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









> 
这边文章的目的主要是为了在spring开发web项目的时候，让我们的测试，开发，生产环境的配置项

- **.properties作为配置文件。**

> 
我们首先需要建立一个config文件夹，然后创建开发，测试，生产环境的.properties配置项文件。

![](https://img-my.csdn.net/uploads/201304/22/1366616938_8503.jpg)

例如，dev.properties文件为开发环境，pre.properties文件为生产环境。

dev.properties配置内容为：

```
#test
test.username=initphp
```




那么，我们这个.properties文件的配置，如何打入xml文件中呢？别急，其实很简单，我们需要修改pom.xml




- **配置pom.xml**




```
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">



	<!-- pom详细参数配置 -->
	<modelVersion>4.0.0</modelVersion>
	<artifactId>xxxx.server</artifactId>
	<name>xxxx server</name>
	<packaging>war</packaging>
	<version>1.0.0</version>

	<!-- 自定义的参数变量，使用 例如：${org.springframework-version} -->
	<properties>
		<java-version>1.6</java-version>
		<org.springframework-version>3.1.1.RELEASE</org.springframework-version>
		<org.aspectj-version>1.6.10</org.aspectj-version>
		<org.slf4j-version>1.6.6</org.slf4j-version>
		<project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
	</properties>

	<!-- 配置文件 -->
	<profiles>
		<profile>
			<id>dev</id>
			<properties>
				<env>dev</env>
			</properties>
			<activation>
				<activeByDefault>true</activeByDefault>
			</activation>
		</profile>
		<profile>
			<id>qa</id>
			<properties>
				<env>qa</env>
			</properties>
		</profile>
		<profile>
			<id>pre</id>
			<properties>
				<env>pre</env>
			</properties>
		</profile>
		<profile>
			<id>prod</id>
			<properties>
				<env>prod</env>
			</properties>
		</profile>
	</profiles>
	
	<!-- 依赖的包 -->
	<dependencies>
	
		
	</dependencies>
	
	<!-- 编译设置 -->
	<build>
		<filters>
			<filter>config/${env}.properties</filter>
		</filters>
		<resources>
			<resource>
				<directory>src/main/resources</directory>
				<filtering>true</filtering>
			</resource>
		</resources>

		<plugins>
			<plugin>
				<artifactId>maven-eclipse-plugin</artifactId>
				<version>2.9</version>
				<configuration>
					<additionalProjectnatures>
						<projectnature>org.springframework.ide.eclipse.core.springnature</projectnature>
					</additionalProjectnatures>
					<additionalBuildcommands>
						<buildcommand>org.springframework.ide.eclipse.core.springbuilder</buildcommand>
					</additionalBuildcommands>
					<downloadSources>true</downloadSources>
					<downloadJavadocs>true</downloadJavadocs>
				</configuration>
			</plugin>
			<plugin>
				<groupId>org.apache.maven.plugins</groupId>
				<artifactId>maven-compiler-plugin</artifactId>
				<version>2.5.1</version>
				<configuration>
					<source>1.6</source>
					<target>1.6</target>
					<compilerArgument>-Xlint:all</compilerArgument>
					<showWarnings>true</showWarnings>
					<showDeprecation>true</showDeprecation>
				</configuration>
			</plugin>
			<plugin>
				<groupId>org.codehaus.mojo</groupId>
				<artifactId>exec-maven-plugin</artifactId>
				<version>1.2.1</version>
				<configuration>
					<mainClass>org.test.int1.Main</mainClass>
				</configuration>
			</plugin>
		</plugins>
	</build>
</project>
```

> 
主要<profiles>中配置了各种环境的配置项参数。<filters>中，用于读取和替换配置。




- **替换XML中的配置项变量，我们通过spring bean注入的方式，将配置项参数注入Config.java类中的静态变量。**

> 
例如我们主要注入到com.xxxx.xxxx.common.bo.ConfigBo，我们预先定义的一个配置类中。先看一下ConfigBo这个类：



```java
/**
 * 常量配置
 * @author hanqi
 */
public class ConfigBo {

    /** 每页显示页数 */
    public static final int PAGE_SIZE              = 20;
    
    public static String    USERNAME         = ""; //被注入的static变量，不能有final终态关键字

    //bean注入 USERNAME 必须要用 set函数
    public void setUserName(String userName) {
        USERNAME = userName;
    }

}
```


我们需要修改xml文件，添加注入的Bean：

```
<bean class="com.xxxx.xxxx.common.bo.ConfigBo">
		<property name="userName" value="${test.username}" />
	</bean>
```




这个时候，我们就能在项目中，静态调用ConfigBo.USERNAME这个常量了，而不需要关心测试环境，开发环境以及正式环境的区别了。

xml中的${test.username}实际上是将properties配置文件中的选项值进行了替换，所以在mvn install后，target目录下，查看xml文件，就可以看到userName的值就是"initphp"










