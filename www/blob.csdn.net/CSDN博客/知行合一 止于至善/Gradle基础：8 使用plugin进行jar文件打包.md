
# Gradle基础：8:使用plugin进行jar文件打包 - 知行合一 止于至善 - CSDN博客

2018年11月24日 06:58:48[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1339


![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
这篇文章主要目的是用来介绍plugin，为了对本身非常枯燥的内容进行说明，这里特意跟前面的文章一样，准备了一个更加枯燥的例子（如何使用gradle的java plugin来打出一个jar文件），在枯燥的的gradle的学习的旅程中添加一丝不那么烦躁的情绪。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# gradle的plugin是什么
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)回答是什么的定义往往比较复杂，现在我们知道gradle是什么了，一个自动构建工具，但是它是如何支持java和groovy的编译构建呢？这是一个很好的问题，因为答案就是plugin。
不同的plugin提供了不同的功能，从这个角度上来说，gradle就是一个框架，而具体实现则是通过plugin来实现的，从某种意义上来说，maven的设计思路也是如出一辙。
我们不再谈论容易引起争执的设计思路是否一致的问题，至少下面的内容可以使得我们对于gradle的plugin的理解有一个开始：
> gradle提供了一个用于构建的框架结构，具体的实际的构建通过不同的plugin来实现。

# gradle有哪些plugin
相较于是什么的问题，有什么就没有任何分歧了，在当前稳定的gradle的4.10.2版本，gradle官方提供的plugin主要有如下这些：
|用途|Plugin名称|功能说明|
|---|---|---|
|JVM语言和框架|Java|提供java项目的构建支持|
|JVM语言和框架|Java Library|提供java库的构建支持|
|JVM语言和框架|Groovy|提供groovy项目的构建支持|
|JVM语言和框架|Scala|提供scala项目的构建支持|
|JVM语言和框架|Play|提供playframework应用的构建支持|
|JVM语言和框架|ANTLR|提供使用ANTLR生成词法分析器的支持|
|打包与发布|Application|提供构建基于JVM的可运行的应用程序的支持|
|打包与发布|WAR|提供WAR文件结构的打包支持|
|打包与发布|EAR|提供EAR结构的J2EE应用的打包支持|
|打包与发布|OSGi|提供创建OSGi包的支持|
|打包与发布|Maven Publish|提供Maven兼容的仓库的发布支持|
|打包与发布|Ivy Publish|提供Ivy兼容的仓库的发布支持|
|打包与发布|Legacy Maven Plugin|对使用了legacy机制的制品发布到Maven兼容的仓库的支持|
|打包与发布|Distribution|提供ZIP或者tar文件发布方式的支持|
|打包与发布|Java Library Distribution|提供创建包含运行依赖的java库的zip文件的支持|
|代码分析|Checkstyle|可集成使用checkstyle进行代码检查|
|代码分析|FindBugs|可集成使用FindBugs用于代码检查|
|代码分析|PMD|可集成使用PMD用于代码检查|
|代码分析|JDepend|可集成JDepend用于代码检查|
|代码分析|JaCoCo|可集成JaCoCo用于覆盖率数据提供|
|代码分析|CodeNarc|可集成CodeNarc用于groovy代码检查|
|IDE集成|Eclipse|用于创建可以被Eclipse的IDE打开的项目等|
|IDE集成|IntelliJ IDEA|用于创建可以被IntelliJ IDEA的IDE打开的项目|
|Utility|Base|提供生命周期的常用task，比如clean等|
|Utility|Build init|用于在新项目创建gradle骨架结构或者从maven等进行迁移是提供guide|
|Utility|Signing|提供对于生成的文件和制品的数字化签名的支持|
|Utility|Plugin Development|提供更方便的方式用于开发Gradle Plugin|
# 试验示例
这篇文章将通过使用java plugin来对一个简单的java应用进行jar的打包，来说明在gradle中plugin的使用方式。
## 事前准备
做一个简单的HelloWorld的java类，然后将其打成一个jar文件，具体的文件结构如下所示
`liumiaocn:plugin liumiao$ ls
build.gradle    settings.gradle src
liumiaocn:plugin liumiao$ tree .
.
├── build.gradle
├── settings.gradle
└── src
    └── main
        ├── java
        │   └── com
        │       └── liumiaocn
        │           └── HelloPlugin.java
        └── resources
            └── com
                └── liumiaocn
                    └── resource.xml
8 directories, 4 files
liumiaocn:plugin liumiao$`
### settings.gradle
`liumiaocn:plugin liumiao$ cat settings.gradle 
rootProject.name = 'jarPluginSample'
liumiaocn:plugin liumiao$`rootProject.name在创建jar文件时根据约定会被使用。
### build.gradle
`liumiaocn:plugin liumiao$ cat build.gradle 
apply plugin: 'java'
version = '1.0'
jar {
    manifest {
        attributes 'Implementation-Title': 'Gradle plugin sample: create jar distribution',
                   'Implementation-Version': version,
                   'Created-By': 'liumiaocn'
    }
}
liumiaocn:plugin liumiao$`apply plugin: ‘java’：是引入java plugin支持唯一需要做的事情
version：在生成的jar文件中会按照约定生成在jar文件名中
jar{}: jar是java plugin所实现的task之一，这里使用它直接用来生成jar文件
manifest{}：用于设定MANIFEST.MF的内容
### resource文件
`liumiaocn:plugin liumiao$ cat src/main/resources/com/liumiaocn/resource.xml 
<?xml version="1.0" encoding="UTF-8"?>
liumiaocn:plugin liumiao$`仅仅是一个示例，用于演示在jar文件中资源文件也被打包的过程
### HelloWorld类
`liumiaocn:plugin liumiao$ cat src/main/java/com/liumiaocn/HelloPlugin.java 
package com.liumiaocn;
public class HelloPlugin {
    private final String pluginName;
    public HelloPlugin(String name) {
        this.pluginName= name;
    }
    public void grettings() {
      System.out.println("Hello, " + this.pluginName);
    } 
}
liumiaocn:plugin liumiao$`
## 确认project信息
`liumiaocn:plugin liumiao$ gradle projects
> Task :projects
------------------------------------------------------------
Root project
------------------------------------------------------------
Root project 'jarPluginSample'
No sub-projects
To see a list of the tasks of a project, run gradle <project-path>:tasks
For example, try running gradle :tasks
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:plugin liumiao$`
## 打包过程
使用普通方式的jar文件打包，过程繁琐的多，而这里，只需要gradle jar即可
`liumiaocn:plugin liumiao$ ls
build.gradle    settings.gradle src
liumiaocn:plugin liumiao$ gradle jar
BUILD SUCCESSFUL in 0s
3 actionable tasks: 3 executed
liumiaocn:plugin liumiao$`结果确认
可以看到jar文件已经生成在lib下了
liumiaocn:plugin liumiao$ ls
build           build.gradle    settings.gradle src
liumiaocn:plugin liumiao$ tree build
build
├── classes
│   └── java
│       └── main
│           └── com
│               └── liumiaocn
│                   └── HelloPlugin.class
├── libs
│   └── jarPluginSample-1.0.jar
├── resources
│   └── main
│       └── com
│           └── liumiaocn
│               └── resource.xml
└── tmp
    ├── compileJava
    └── jar
        └── MANIFEST.MF
13 directories, 4 files
liumiaocn:plugin liumiao$确认jar文件内容
liumiaocn:plugin liumiao$ zipinfo build/libs/jarPluginSample-1.0.jar 
Archive:  build/libs/jarPluginSample-1.0.jar
Zip file size: 1292 bytes, number of entries: 6
drwxr-xr-x  2.0 unx        0 b- defN 18-Nov-24 06:49 META-INF/
-rw-r--r--  2.0 unx      146 b- defN 18-Nov-24 06:49 META-INF/MANIFEST.MF
drwxr-xr-x  2.0 unx        0 b- defN 18-Nov-24 06:49 com/
drwxr-xr-x  2.0 unx        0 b- defN 18-Nov-24 06:49 com/liumiaocn/
-rw-r--r--  2.0 unx      758 b- defN 18-Nov-24 06:49 com/liumiaocn/HelloPlugin.class
-rw-rw-r--  2.0 unx       39 b- defN 18-Nov-24 06:49 com/liumiaocn/resource.xml
6 files, 943 bytes uncompressed, 606 bytes compressed:  35.7%
liumiaocn:plugin liumiao$注：jar就是一个压缩文件，没有zipinfo，你可以使用大部分解压软件均可看到内部内容
确认MANIFEST.MF
liumiaocn:plugin liumiao$ cat build/tmp/jar/MANIFEST.MF 
Manifest-Version: 1.0
Implementation-Title: Gradle plugin sample: create jar distribution
Implementation-Version: 1.0
Created-By: liumiaocn
liumiaocn:plugin liumiao$其他task
除了jar之外，比如gradle clean的task也被实现了，其实就是删除约定方式申城的构建目录build等。
liumiaocn:plugin liumiao$ ls
build           build.gradle    settings.gradle src
liumiaocn:plugin liumiao$ gradle clean
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:plugin liumiao$ ls
build.gradle    settings.gradle src
liumiaocn:plugin liumiao$除此之外其他的task以及其关联关系，这篇文章不再说明，在后续进一步介绍之后再行展开。
liumiaocn:plugin liumiao$ gradle tasks
> Task :tasks
------------------------------------------------------------
All tasks runnable from root project
------------------------------------------------------------
Build tasks
-----------
assemble - Assembles the outputs of this project.
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
buildEnvironment - Displays all buildscript dependencies declared in root project 'jarPluginSample'.
components - Displays the components produced by root project 'jarPluginSample'. [incubating]
dependencies - Displays all dependencies declared in root project 'jarPluginSample'.
dependencyInsight - Displays the insight into a specific dependency in root project 'jarPluginSample'.
dependentComponents - Displays the dependent components of components in root project 'jarPluginSample'. [incubating]
help - Displays a help message.
model - Displays the configuration model of root project 'jarPluginSample'. [incubating]
projects - Displays the sub-projects of root project 'jarPluginSample'.
properties - Displays the properties of root project 'jarPluginSample'.
tasks - Displays the tasks runnable from root project 'jarPluginSample'.
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
liumiaocn:plugin liumiao$总结
plugin在gradle中是一个非常重要的概念，正是使用了这种方式，才使得gradle有了更好的灵活性，这篇文章通过一个简单的例子介绍了plugin的使用方式，使得我们对其有了一个大体上的认识，详细的说明，后面会进一步展开。
参考文章
[https://docs.gradle.org/current/userguide/plugin_reference.html](https://docs.gradle.org/current/userguide/plugin_reference.html)
[https://www.playframework.com/](https://www.playframework.com/)
[https://www.antlr.org/](https://www.antlr.org/)
[https://guides.gradle.org/migrating-from-maven/](https://guides.gradle.org/migrating-from-maven/)
[https://docs.gradle.org/current/userguide/java_plugin.html](https://docs.gradle.org/current/userguide/java_plugin.html)

