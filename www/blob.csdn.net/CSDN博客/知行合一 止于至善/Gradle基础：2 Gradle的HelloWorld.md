
# Gradle基础：2:Gradle的HelloWorld - 知行合一 止于至善 - CSDN博客

2018年11月19日 06:15:53[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1585


![gradle](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
这篇文章使用最简单的HelloWorld例子对Gradle的使用进行概要性的说明。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 事前准备
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)事前安装和设定好gradle
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle --version
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
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 第一个HelloWorld
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)准备一个groovy文件，具体信息如下
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ ls
build.gradle
liumiaocn:hello liumiao$ cat build.gradle 
println "hello gradle"
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)执行gradle命令
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle
> Configure project :
hello gradle
> Task :help
Welcome to Gradle 4.10.2.
To run a build, run gradle <task> ...
To see a list of available tasks, run gradle tasks
To see a list of command-line options, run gradle --help
To see more detail about a task, run gradle help --task <task>
For troubleshooting, visit https://help.gradle.org
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)这个例子看起来更像是groovy的hello world，接下来先对gradle的基本概念等信息进行展开后，对此例子再进行进一步地解释。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 基础概念
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)Gradle有两个最基础的概念：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)Task：在构建中Gradle执行和管理的原子粒度的最小单元，比如上例中的输出hello的语句，在实际的使用中比如编译或者执行单体测试等操作均可作为task。
Project：可以理解为Project就是存放Task的场所，Project可由一个或多个Task所组成。而构建也可以由多个Project所组成，这样就能实现对不同项目的适应。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 两个gradle文件
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)在例子中使用了一个build.gradle文件，还有一个settings.gradle文件在最初使用时会经常见到：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)build.gradle: 用于存放构建相关的Task，单个工程（Project）与build.gradle的关系是一对一的关系，如果是多个工程的情况下，类似与maven的module的方式，构建根目录下的build.gradle起到总纲的作用。
settings.gradle: 用于存放设定相关的信息，单工程时不是必须的，多工程是必须的，一般用于引入多个工程，在项目初期化的时候会根据此文件生成一个Settings实例用于执行。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# Projects的确认
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)使用gradle projects可以查看工程下的详细信息
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle projects
> Configure project :
hello gradle
> Task :projects
------------------------------------------------------------
Root project
------------------------------------------------------------
Root project 'hello'
No sub-projects
To see a list of the tasks of a project, run gradle <project-path>:tasks
For example, try running gradle :tasks
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)可以看到，Root project 'hello’的提示，以及没有子工程（No sub-projects）的提示。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# Tasks的确认
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)通过gradle tasks可以查看task的详细信息
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle tasks
> Configure project :
hello gradle
> Task :tasks
------------------------------------------------------------
All tasks runnable from root project
------------------------------------------------------------
Build Setup tasks
-----------------
init - Initializes a new Gradle build.
wrapper - Generates Gradle wrapper files.
Help tasks
----------
buildEnvironment - Displays all buildscript dependencies declared in root project 'hello'.
components - Displays the components produced by root project 'hello'. [incubating]
dependencies - Displays all dependencies declared in root project 'hello'.
dependencyInsight - Displays the insight into a specific dependency in root project 'hello'.
dependentComponents - Displays the dependent components of components in root project 'hello'. [incubating]
help - Displays a help message.
model - Displays the configuration model of root project 'hello'. [incubating]
projects - Displays the sub-projects of root project 'hello'.
properties - Displays the properties of root project 'hello'.
tasks - Displays the tasks runnable from root project 'hello'.
To see all tasks and more detail, run gradle tasks --all
To see more detail about a task, run gradle help --task <task>
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 改进的HelloWorld
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)上述的HelloWorld例子过于简单, 在settings.gradle中添加一句println来确认一下执行顺序，然后设定project的名称，而不是使用缺省被指定的hello
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ cat settings.gradle 
println "setting gradle ..."
rootProject.name='helloPorject'
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)同样，对task也做一些简单的设定，详细后续在task的使用方式中展开
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ cat build.gradle 
task helloGradle {
  println "hello gradle"
}
project.task('helloTask',group:'helloGradle',description:'hello gradle task')
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 确认project
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle projects
setting gradle ...
> Configure project :
hello gradle
> Task :projects
------------------------------------------------------------
Root project
------------------------------------------------------------
Root project 'helloPorject'
No sub-projects
To see a list of the tasks of a project, run gradle <project-path>:tasks
For example, try running gradle :tasks
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 确认task
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle tasks
setting gradle ...
> Configure project :
hello gradle
> Task :tasks
------------------------------------------------------------
All tasks runnable from root project
------------------------------------------------------------
Build Setup tasks
-----------------
init - Initializes a new Gradle build.
wrapper - Generates Gradle wrapper files.
HelloGradle tasks
-----------------
helloTask - hello gradle task
Help tasks
----------
buildEnvironment - Displays all buildscript dependencies declared in root project 'helloPorject'.
components - Displays the components produced by root project 'helloPorject'. [incubating]
dependencies - Displays all dependencies declared in root project 'helloPorject'.
dependencyInsight - Displays the insight into a specific dependency in root project 'helloPorject'.
dependentComponents - Displays the dependent components of components in root project 'helloPorject'. [incubating]
help - Displays a help message.
model - Displays the configuration model of root project 'helloPorject'. [incubating]
projects - Displays the sub-projects of root project 'helloPorject'.
properties - Displays the properties of root project 'helloPorject'.
tasks - Displays the tasks runnable from root project 'helloPorject'.
To see all tasks and more detail, run gradle tasks --all
To see more detail about a task, run gradle help --task <task>
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 执行task
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)task既然有名称了，就可以使用名称来执行了
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle helloGradle
setting gradle ...
> Configure project :
hello gradle
BUILD SUCCESSFUL in 0s
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 总结
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)Task和Project两个基本概念，settings.gradle和build.gradle两个缺省配置文件，这是需要首先掌握的。
[
            ](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

