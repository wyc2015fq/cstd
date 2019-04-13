
# Gradle进阶：1: 结合spring boot进行web开发 - 知行合一 止于至善 - CSDN博客

2018年12月11日 07:26:39[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：384


![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
在前面的文章中介绍了如何使用gradle的基础知识，这篇文章在某种程度上才是真正意义上的第一个hello world，这里使用一个简单的spring boot的例子，通过gradle进行编译和构建以及确认。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 事前准备
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 环境准备
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)|组件/框架|版本|
|---|---|
|JDK|1.8.0|
|Gradle|4.10.2|
|Spring Boot|2.1.1|
`liumiaocn:springboot liumiao$ gradle -v
------------------------------------------------------------
Gradle 4.10.2
------------------------------------------------------------
Build time:   2018-09-19 18:10:15 UTC
Revision:     b4d8d5d170bb4ba516e88d7fe5647e2323d791dd
Kotlin DSL:   1.0-rc-6
Kotlin:       1.2.61
Groovy:       2.4.15
Ant:          Apache Ant(TM) version 1.9.11 compiled on March 23 2018
JVM:          1.8.0_191 (Oracle Corporation 25.191-b12)
OS:           Mac OS X 10.14 x86_64
liumiaocn:springboot liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## spring boot demo应用
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
### 目录结构
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)使用https://start.spring.io/或者自己手动创建如下结构的目录结构
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:springboot liumiao$ tree 
.
├── build.gradle
├── settings.gradle
└── src
    └── main
        ├── java
        │   └── com
        │       └── liumiaocn
        │           └── springbootdemo
        │               └── SpringbootdemoApplication.java
        └── resources
            └── application.properties
7 directories, 4 files
liumiaocn:springboot liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
### spring boot相关说明
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)详细请参看spring boot相关说明，本文不再赘述：
[https://blog.csdn.net/liumiaocn/article/details/83548217](https://blog.csdn.net/liumiaocn/article/details/83548217)
`liumiaocn:springboot liumiao$ cat src/main/resources/application.properties 
liumiaocn:springboot liumiao$ cat src/main/java/com/liumiaocn/springbootdemo/SpringbootdemoApplication.java 
package com.liumiaocn.springbootdemo;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.bind.annotation.RequestMapping;
@RestController
@SpringBootApplication
public class SpringbootdemoApplication {
        @RequestMapping("/")
        String home() {
          return "Hello, Spring Boot 2";
        }
	public static void main(String[] args) {
		SpringApplication.run(SpringbootdemoApplication.class, args);
	}
}
liumiaocn:springboot liumiao$`
### gradle设定: settings.gradle
settings.gradle中仅设定`rootProject.name`信息，用于作为缺省的jar文件的名称等用处。
`liumiaocn:springboot liumiao$ cat settings.gradle 
rootProject.name = 'springbootdemo'
liumiaocn:springboot liumiao$`
### gradle设定：build.gradle
build.gradle中做如下设定，简单说明如下：
ext: 用于定义自定义属性，属性的多种使用方式在基础篇进行过详细介绍，这里不再赘述
repositories：用于设定maven仓库，这里的例子使用了mavenCentral，如果使用其他的私库，可使用maven{ url ‘[http://xxx](http://xxx)’}的方式替换即可
dependencies：比照spring boot的maven的写法，可看到这里spring-boot-gradle-plugin和spring-boot-starter-web的引入
GAV：maven坐标，在gradle里可以通过group和version进行直接设定
sourceCompatibility：用于设定java版本
apply plugin：引入了java/org.springframework.boot/io.spring.dependency-management
`liumiaocn:springboot liumiao$ cat build.gradle 
buildscript {
	ext {
		springBootVersion = '2.1.1.RELEASE'
	}
	repositories {
		mavenCentral()
	}
	dependencies {
		classpath("org.springframework.boot:spring-boot-gradle-plugin:${springBootVersion}")
	}
}
apply plugin: 'java'
apply plugin: 'org.springframework.boot'
apply plugin: 'io.spring.dependency-management'
group = 'com.liumiaocn'
version = '0.0.1-SNAPSHOT'
sourceCompatibility = 1.8
repositories {
	mavenCentral()
}

dependencies {
	implementation('org.springframework.boot:spring-boot-starter-web')
}
liumiaocn:springboot liumiao$`可以通过gradle tasks查看apply plugin所引入的task信息
`liumiaocn:springboot liumiao$ gradle tasks
> Task :tasks
------------------------------------------------------------
All tasks runnable from root project
------------------------------------------------------------
Application tasks
-----------------
bootRun - Runs this project as a Spring Boot application.
Build tasks
-----------
assemble - Assembles the outputs of this project.
bootJar - Assembles an executable jar archive containing the main classes and their dependencies.
build - Assembles and tests this project.
buildDependents - Assembles and tests this project and all projects that depend on it.
buildNeeded - Assembles and tests this project and all projects it depends on.
classes - Assembles main classes.
clean - Deletes the build directory.
jar - Assembles a jar archive containing the main classes.
testClasses - Assembles test classes.
Build Setup tasks
-----------------
init - Initializes a new Gradle build.
wrapper - Generates Gradle wrapper files.
Documentation tasks
-------------------
javadoc - Generates Javadoc API documentation for the main source code.
Help tasks
----------
buildEnvironment - Displays all buildscript dependencies declared in root project 'springbootdemo'.
components - Displays the components produced by root project 'springbootdemo'. [incubating]
dependencies - Displays all dependencies declared in root project 'springbootdemo'.
dependencyInsight - Displays the insight into a specific dependency in root project 'springbootdemo'.
dependencyManagement - Displays the dependency management declared in root project 'springbootdemo'.
dependentComponents - Displays the dependent components of components in root project 'springbootdemo'. [incubating]
help - Displays a help message.
model - Displays the configuration model of root project 'springbootdemo'. [incubating]
projects - Displays the sub-projects of root project 'springbootdemo'.
properties - Displays the properties of root project 'springbootdemo'.
tasks - Displays the tasks runnable from root project 'springbootdemo'.
Verification tasks
------------------
check - Runs all checks.
test - Runs the unit tests.
Rules
-----
Pattern: clean<TaskName>: Cleans the output files of a task.
Pattern: build<ConfigurationName>: Assembles the artifacts of a configuration.
Pattern: upload<ConfigurationName>: Assembles and uploads the artifacts belonging to a configuration.
To see all tasks and more detail, run gradle tasks --all
To see more detail about a task, run gradle help --task <task>
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:springboot liumiao$`
# 构建
通过使用gradle build或者bootJar进行构建
`liumiaocn:springboot liumiao$ ls
build.gradle    settings.gradle src
liumiaocn:springboot liumiao$ gradle build
BUILD SUCCESSFUL in 1s
3 actionable tasks: 3 executed
liumiaocn:springboot liumiao$ ls
build           build.gradle    settings.gradle src
liumiaocn:springboot liumiao$`可以看到，gradle build生成了一个build的目录，此构建结果目录详细信息如下：
`liumiaocn:springboot liumiao$ tree build
build
├── classes
│   └── java
│       └── main
│           └── com
│               └── liumiaocn
│                   └── springbootdemo
│                       └── SpringbootdemoApplication.class
├── libs
│   └── springbootdemo-0.0.1-SNAPSHOT.jar
├── resources
│   └── main
│       └── application.properties
└── tmp
    ├── bootJar
    │   └── MANIFEST.MF
    └── compileJava
12 directories, 4 files
liumiaocn:springboot liumiao$`
# 结果确认
## 启动spring boot应用
使用java -jar 启动生成的springboot的jar文件，也可以使用插件封装的bootRun任务来执行，这里使用gradle bootRun来执行
`liumiaocn:springboot liumiao$ gradle bootRun
> Task :bootRun
  .   ____          _            __ _ _
 /\\ / ___'_ __ _ _(_)_ __  __ _ \ \ \ \
( ( )\___ | '_ | '_| | '_ \/ _` | \ \ \ \
 \\/  ___)| |_)| | | | | || (_| |  ) ) ) )
  '  |____| .__|_| |_|_| |_\__, | / / / /
 =========|_|==============|___/=/_/_/_/
 :: Spring Boot ::        (v2.1.1.RELEASE)
2018-12-11 07:24:42.354  INFO 49488 --- [           main] c.l.s.SpringbootdemoApplication          : Starting SpringbootdemoApplication on liumiaocn with PID 49488 (/Users/liumiao/gradle/springboot/build/classes/java/main started by liumiao in /Users/liumiao/gradle/springboot)
2018-12-11 07:24:42.362  INFO 49488 --- [           main] c.l.s.SpringbootdemoApplication          : No active profile set, falling back to default profiles: default
2018-12-11 07:24:44.593  INFO 49488 --- [           main] o.s.b.w.embedded.tomcat.TomcatWebServer  : Tomcat initialized with port(s): 8080 (http)
2018-12-11 07:24:44.649  INFO 49488 --- [           main] o.apache.catalina.core.StandardService   : Starting service [Tomcat]
2018-12-11 07:24:44.649  INFO 49488 --- [           main] org.apache.catalina.core.StandardEngine  : Starting Servlet Engine: Apache Tomcat/9.0.13
2018-12-11 07:24:44.674  INFO 49488 --- [           main] o.a.catalina.core.AprLifecycleListener   : The APR based Apache Tomcat Native library which allows optimal performance in production environments was not found on the java.library.path: [/Users/liumiao/Library/Java/Extensions:/Library/Java/Extensions:/Network/Library/Java/Extensions:/System/Library/Java/Extensions:/usr/lib/java:.]
2018-12-11 07:24:44.876  INFO 49488 --- [           main] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext
2018-12-11 07:24:44.876  INFO 49488 --- [           main] o.s.web.context.ContextLoader            : Root WebApplicationContext: initialization completed in 2420 ms
2018-12-11 07:24:45.398  INFO 49488 --- [           main] o.s.s.concurrent.ThreadPoolTaskExecutor  : Initializing ExecutorService 'applicationTaskExecutor'
2018-12-11 07:24:45.869  INFO 49488 --- [           main] o.s.b.w.embedded.tomcat.TomcatWebServer  : Tomcat started on port(s): 8080 (http) with context path ''
2018-12-11 07:24:45.876  INFO 49488 --- [           main] c.l.s.SpringbootdemoApplication          : Started SpringbootdemoApplication in 4.26 seconds (JVM running for 5.059)
<=========----> 75% EXECUTING [17s]
> :bootRun`
## 确认结果
可以通过页面访问确认应用信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121011112279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
也可以通过curl来确认
[
](https://img-blog.csdnimg.cn/2018121011112279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:~ liumiao$ curl http://localhost:8080/
Hello, Spring Boot 2liumiaocn:~ liumiao$`[
](https://img-blog.csdnimg.cn/2018121011112279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 其他章节
[
](https://img-blog.csdnimg.cn/2018121011112279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)[https://blog.csdn.net/liumiaocn/article/details/84341611](https://blog.csdn.net/liumiaocn/article/details/84341611)
[
](https://img-blog.csdnimg.cn/2018121011112279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 总结
[
](https://img-blog.csdnimg.cn/2018121011112279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)通过使用spring boot和java的相关插件，可以很容易地使用gradle来进行spring boot的web项目的构建，在下篇文章中将结合Junit的实施方式进行示例说明。
[
            ](https://img-blog.csdnimg.cn/2018121011112279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

