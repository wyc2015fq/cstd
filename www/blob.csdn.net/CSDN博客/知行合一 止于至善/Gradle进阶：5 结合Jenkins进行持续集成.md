
# Gradle进阶：5:结合Jenkins进行持续集成 - 知行合一 止于至善 - CSDN博客

2018年12月26日 07:18:55[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1039


![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
结合Jenkins进行gradle的持续构建有多种方式，可以在Jenkins的容器种直接安装gradle并直接使用，也可以通过jenkins的gradle插件进行使用，也可以通过调用gradle的镜像来进行使用。这篇文章以jenkins的gradle插件使用方式为例，介绍如何在jenkins中使用gradle。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# Jenkins的gradle插件
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181224220421270.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 插件安装方式
[可以通过命令行install-plugins.sh](http://xn--install-plugins-7d6xqa333uenc5s43b626afrb.sh)gradle方式进行插件安装，也可以通过Jenkins的界面直接安装gradle
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181224220528100.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 安装gradle
gradle插件只是负责调用gradle，当然也可以通过gradle插件安装，无论哪种方式，在jenkins中能够调用到gradle这件事情是整个集成的前提和条件。因为这里示例的jenkins是运行在容器中的，所以在jenkins的镜像中需要确保能够执行gradle的命令（如果不是镜像方式，只需要保证安装在同一台机器上，并且具有权限即可）
`~ # /usr/local/share/gradle/bin/gradle --version
------------------------------------------------------------
Gradle 5.0
------------------------------------------------------------
Build time:   2018-11-26 11:48:43 UTC
Revision:     7fc6e5abf2fc5fe0824aec8a0f5462664dbcd987
Kotlin DSL:   1.0.4
Kotlin:       1.3.10
Groovy:       2.5.4
Ant:          Apache Ant(TM) version 1.9.13 compiled on July 10 2018
JVM:          1.8.0_121 (Oracle Corporation 25.121-b13)
OS:           Linux 3.10.0-693.el7.x86_64 amd64
~ #`
# 设定gradle
在Jenkins的Global Tool Configuration中设定如下Gradle的信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181226070334422.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 使用jenkins执行构建
## gradle的spring boot的demo应用
将gradle的demo应用上传到自行搭建的gitlab，包括gitlab和svn相关的版本管理等请参看：[https://blog.csdn.net/liumiaocn/article/details/82286778](https://blog.csdn.net/liumiaocn/article/details/82286778)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181226070906550.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 创建一个free style的jenkins的job
创建一个free style的jenkins的job，注意如下两项设定：
设定gitlab的代码仓库信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181226071200480.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
设定gradle信息
注意此处选定上文中已经设定的名为gradle的工具信息，而此gradle的HOME目录则位于jenkins的/usr/local/share/gradle/下。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181226071358383.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
执行构建
这样即可执行构建，执行构建的日志信息如下所示：
Started by user root
Building in workspace /data/jenkins/workspace/gradle-demo-freestyle
 > git rev-parse --is-inside-work-tree \# timeout=10
Fetching changes from the remote Git repository
 > git config remote.origin.url http://192.168.163.118:32001/root/gradle-demo \# timeout=10
Fetching upstream changes from http://192.168.163.118:32001/root/gradle-demo
 > git --version \# timeout=10
using GIT_ASKPASS to set credentials 
 > git fetch --tags --progress http://192.168.163.118:32001/root/gradle-demo +refs/heads/*:refs/remotes/origin/*
 > git rev-parse refs/remotes/origin/master^{commit} \# timeout=10
 > git rev-parse refs/remotes/origin/origin/master^{commit} \# timeout=10
Checking out Revision f01485b7b76e88ddb1ab17c0afffa6c358f822c8 (refs/remotes/origin/master)
 > git config core.sparsecheckout \# timeout=10
 > git checkout -f f01485b7b76e88ddb1ab17c0afffa6c358f822c8
Commit message: "init"
 > git rev-list f01485b7b76e88ddb1ab17c0afffa6c358f822c8 \# timeout=10
[Gradle] - Launching build.
[gradle-demo-freestyle] $ /usr/local/share/gradle/bin/gradle build
Starting a Gradle Daemon (subsequent builds will be faster)
> Task :compileJava
> Task :processResources
> Task :classes
> Task :bootJar
> Task :jar SKIPPED
> Task :assemble
> Task :compileTestJava
> Task :processTestResources NO-SOURCE
> Task :testClasses
> Task :test
2018-12-26 5:41:47.383  INFO 1170 --- [       Thread-6] o.s.s.concurrent.ThreadPoolTaskExecutor  : Shutting down ExecutorService 'applicationTaskExecutor'
> Task :jacocoTestReport
> Task :check
> Task :build
BUILD SUCCESSFUL in 3m 54s
6 actionable tasks: 6 executed
Build step 'Invoke Gradle script' changed build result to SUCCESS
Finished: SUCCESS确认结果
进到jenkins的构建目录中可以确认，build相关的目录下的内容均已完成，包括spring boot的jar文件以及jacoco的单体测试覆盖率报告的信息等。
/data/jenkins/workspace/gradle-demo-freestyle \# ls
build            build.gradle     settings.gradle  src
/data/jenkins/workspace/gradle-demo-freestyle \# find build -type d
build
build/classes
build/classes/java
build/classes/java/main
build/classes/java/main/com
build/classes/java/main/com/liumiaocn
build/classes/java/main/com/liumiaocn/springbootdemo
build/classes/java/test
build/classes/java/test/com
build/classes/java/test/com/liumiaocn
build/classes/java/test/com/liumiaocn/springbootdemo
build/tmp
build/tmp/compileJava
build/tmp/bootJar
build/tmp/compileTestJava
build/tmp/expandedArchives
build/tmp/expandedArchives/org.jacoco.agent-0.8.2.jar_2aca8b620b19ecd063f63feff8caaa38
build/tmp/expandedArchives/org.jacoco.agent-0.8.2.jar_2aca8b620b19ecd063f63feff8caaa38/META-INF
build/tmp/expandedArchives/org.jacoco.agent-0.8.2.jar_2aca8b620b19ecd063f63feff8caaa38/META-INF/maven
build/tmp/expandedArchives/org.jacoco.agent-0.8.2.jar_2aca8b620b19ecd063f63feff8caaa38/META-INF/maven/org.jacoco
build/tmp/expandedArchives/org.jacoco.agent-0.8.2.jar_2aca8b620b19ecd063f63feff8caaa38/META-INF/maven/org.jacoco/org.jacoco.agent
build/tmp/expandedArchives/org.jacoco.agent-0.8.2.jar_2aca8b620b19ecd063f63feff8caaa38/org
build/tmp/expandedArchives/org.jacoco.agent-0.8.2.jar_2aca8b620b19ecd063f63feff8caaa38/org/jacoco
build/tmp/expandedArchives/org.jacoco.agent-0.8.2.jar_2aca8b620b19ecd063f63feff8caaa38/org/jacoco/agent
build/resources
build/resources/main
build/libs
build/test-results
build/test-results/test
build/test-results/test/binary
build/jacoco
build/reports
build/reports/tests
build/reports/tests/test
build/reports/tests/test/classes
build/reports/tests/test/packages
build/reports/tests/test/css
build/reports/tests/test/js
build/reports/jacoco
build/reports/jacoco/test
build/reports/jacoco/test/html
build/reports/jacoco/test/html/jacoco-resources
build/reports/jacoco/test/html/com.liumiaocn.springbootdemo
/data/jenkins/workspace/gradle-demo-freestyle \#

