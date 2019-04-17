# 使用Gradle 快速构建项目 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年01月21日 09:43:32[boonya](https://me.csdn.net/boonya)阅读数：2296















![Gradle](http://gradle.wpengine.netdna-cdn.com/wp-content/uploads/2015/10/gradle-logo-horizontal2.svg)


Gradle 是一款基于 Groovy 语言、免费开源的构建工具，它既保持了 Maven 的优点，又通过使用 Groovy 定义的 DSL 克服了 Maven 中使用 XML 繁冗以及不灵活的缺点。

Gradle 官方网站：[http://www.gradle.org/downloads](http://www.gradle.org/downloads)


**功能**

Ø gradle对多工程的构建支持很出色，工程依赖是gradle的第一公民。

Ø gradle支持局部构建。

　　Ø 支持多方式依赖管理：包括从[maven](http://baike.baidu.com/edit/gradle/3066272?dl=1#)远程仓库、[nexus](http://baike.baidu.com/edit/gradle/3066272?dl=1#)私服、ivy仓库以及本地文件系统的jars或者dirs

Ø gradle是第一个构建集成工具（the first build integration tool），与ant、maven、ivy有良好的相容相关性。

Ø 轻松迁移：gradle适用于任何结构的工程（Gradle can adapt to any structure you have.）。你可以在同一个开发平台平行构建原工程和gradle工程。通常要求写相关测试，以保证开发的插件的相似性，这种迁移可以减少破坏性，尽可能的可靠。这也是重构的最佳实践。

Ø gradle的整体设计是以作为一种语言为导向的，而非成为一个严格死板的框架。

Ø 免费开源

**gradle提供了什么**

1.一种可切换的，像maven一样的基于约定的构建框架，却又从不锁住你（约定优于配置）

Switchable, build-by-convention frameworks a la Maven. But we never lock you in!

2. 强大的支持多工程的构建

3. 强大的依赖管理（基于Apache Ivy），提供最大的便利去构建你的工程

Language for dependency based programming

4. 全力支持已有的Maven或者Ivy仓库基础建设

5. 支持传递性依赖管理，在不需要远程仓库和pom.xml和ivy配置文件的前提下

6 基于groovy脚本构建，其build脚本使用groovy语言编写

7 具有广泛的领域模型支持你的构建A rich domain model for describing your build.


# Installing Gradle








## Prerequisites




Gradle requires a Java JDK or JRE to be installed, version 6 or higher (to check, use**`java -version`**). Gradle ships with its own Groovy library, therefore Groovy does not need to be installed. Any existing Groovy
 installation is ignored by Gradle.

Gradle uses whatever JDK it finds in your path. Alternatively, you can set the`JAVA_HOME` environment variable to point to the installation directory of the desired JDK.






## Download




You can download one of the Gradle distributions from the [Gradle web site](http://www.gradle.org/downloads).






## Unpacking




The Gradle distribution comes packaged as a ZIP. The full distribution contains:

- 
The Gradle binaries.

- 
The user guide (HTML and PDF).

- 
The DSL reference guide.

- 
The API documentation (Javadoc and Groovydoc).

- 
Extensive samples, including the examples referenced in the user guide, along with some complete and more complex builds you can use as a starting point for your own build.

- 
The binary sources. This is for reference only. If you want to build Gradle you need to download the source distribution or checkout the sources from the source repository. See the[Gradle
 web site](http://www.gradle.org/development) for details. 








## Environment variables




For running Gradle, add `*`GRADLE_HOME`*/bin` to your`PATH` environment variable. Usually, this is sufficient to run Gradle.






## Running and testing your installation




You run Gradle via the **gradle** command. To check if Gradle is properly installed just type**gradle -v**. The output shows the Gradle version and also the local environment
 configuration (Groovy, JVM version, OS, etc.). The displayed Gradle version should match the distribution you have downloaded.






## JVM options




JVM options for running Gradle can be set via environment variables. You can use either`GRADLE_OPTS`or`JAVA_OPTS`, or both.`JAVA_OPTS` is by convention an environmentvariable shared
 by many Java applications. A typical use case would be to set the HTTP proxy in`JAVA_OPTS`and the memory options in`GRADLE_OPTS`. Those variables can also be set at the beginningof the**gradle**
 or**gradlew** script.

Note that it's not currently possible to set JVM options for Gradle on the command line.






## Getting help




You might check the user guide at `*`GRADLE_HOME`*/docs/userguide/userguide.html`.It is also available on the[Gradle
 web site](http://www.gradle.org/documentation.html).Typing **gradle help** prints the command line help. Typing**gradle tasks** shows all thetasks of a Gradle build.

# 实践操作

## 设置环境变量

第一步：


![](https://img-blog.csdn.net/20160121144116213?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

第二步：

![](https://img-blog.csdn.net/20160121144151338?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 测试环境安装

![](https://img-blog.csdn.net/20160121144414951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 编译构建项目

在test项目下添加build.gradle文件，并添加如下内容：


`apply plugin:'java'`


在项目路径下，输入gradle build进行项目构建


![](https://img-blog.csdn.net/20160121144514714?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

构建完成后可以看到如下目录：

![](https://img-blog.csdn.net/20160121144915130?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

想了解更多知识参考：
[Gradle User Guide](http://www.gradle.org/docs/current/userguide/userguide_single.html)
[Groovy入门教程](http://blog.csdn.net/kmyhy/article/details/4200563)




[Gradle替代maven](http://my.oschina.net/enyo/blog/369843)
[Groovy 闭包深入浅出](http://romejiang.iteye.com/blog/214812)





