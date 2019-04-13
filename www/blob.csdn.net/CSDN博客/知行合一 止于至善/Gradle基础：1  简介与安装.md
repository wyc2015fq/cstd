
# Gradle基础：1: 简介与安装 - 知行合一 止于至善 - CSDN博客

2018年11月18日 05:31:25[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2797


![gradle](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
这篇文章主要介绍一下Grale的主要特性以及安装方式。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 什么是Gradle
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)Gradle是一个开源的自动构建工具，在Apache Ant和Apache Maven的相关概念基础上发展而来，与Maven最大的区别之一在于引入了基于Groovy的DSL语言而非使用繁琐复杂的XML来进行项目的配置。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)Gradle的第一个版本与2007年发布，截止到目前为止最新版本已经是5.0 RC3，稳定版本在4.10.2（2018/09/20）
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 概要信息
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)|项目|说明|
|---|---|
|官网|[https://gradle.org/](https://gradle.org/)|
|开源/闭源|开源|
|License类别|Apache License 2.0|
|代码管理地址|[https://github.com/gradle/gradle](https://github.com/gradle/gradle)|
|开发语言|Java, Groovy, Kotlin|
|当前稳定版本|5.0 RC 3（2018/11/15）|
|更新频度|平均每月数次|
# 特点
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)最为显著的特点是使用Groovy作为脚本构建语言，具有更好的扩展性
*支持多个工程
与Maven和Ivy兼容
通过task执行具体操作
解决多模块问题
解决依赖问题
支持多种语言的编译（Android的官方构建工具）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181117194718712.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
Gradle vs Maven
关于哪个更好的问题，有人说Maven更好
[https://www.softwareyoga.com/10-reasons-why-we-chose-maven-over-gradle/](https://www.softwareyoga.com/10-reasons-why-we-chose-maven-over-gradle/)
也有人说Gradle更好，比如Gradle的官方比较：
[https://gradle.org/maven-vs-gradle/](https://gradle.org/maven-vs-gradle/)
而stackshare.io目前的调查结果是这样的：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181117170828869.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
在来看一下一个稍微有点旧的统计数据，它显示了Gradle的上升趋势，Ant已经日薄西山这是事实。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181117171410860.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
安装
这系列文章以目前稳定版本4.10.2为例进行演示。
前提条件
基于java的跨平台特性，Gradle可以运行在大部分主流的操作系统上，所需要的前提仅仅是JDK或者JRE：
版本依赖： JDK或JRE7及以上
Groovy：   Gradle自带Groovy，无需自行安装，如果已经安装也会被Gradle所忽略。
安装方式
可以使用一些Package Manager进行安装，比如MacOS：
brew install gradle
或者使用sdk
sdk install gradle 4.10.2
也可以使用Gradle发布的二进制包，本文使用此种方式
比如当前4.10.2的官方下载路径：
[https://gradle.org/next-steps/?version=4.10.2&format=bin](https://gradle.org/next-steps/?version=4.10.2&format=bin)
如果速度很慢的话，试一下这个：
[http://services.gradle.org/distributions/gradle-4.10.2-bin.zip](http://services.gradle.org/distributions/gradle-4.10.2-bin.zip)
在Mac上下载并解压之后，如果所示，也可以放到你中意的目录下，只是后续的环境变量设定保持一致即可。
liumiaocn:gradle liumiao$ ls
gradle-4.10.2         gradle-4.10.2-bin.zip
liumiaocn:gradle liumiao$ ls gradle-4.10.2
LICENSE              bin                  init.d               media
NOTICE               getting-started.html lib
liumiaocn:gradle liumiao$环境变量设定
将gradle文件添加到PATH搜索路径中即可，比如可以如下方式写入profile中。
export GRADLE_HOME=/Library/gradle/gradle-4.10.2
export PATH=$PATH:${GRADLE_HOME}/bin版本确认
使用gradle -v即可确认gradle版本和相关信息
liumiaocn:~ liumiao$ gradle -v
Welcome to Gradle 4.10.2!
Here are the highlights of this release:
 - Incremental Java compilation by default
 - Periodic Gradle caches cleanup
 - Gradle Kotlin DSL 1.0-RC6
 - Nested included builds
 - SNAPSHOT plugin versions in the `plugins {}` block
For more details see https://docs.gradle.org/4.10.2/release-notes.html

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
liumiaocn:~ liumiao$参考内容
[https://docs.gradle.org/2.4/userguide/installation.html](https://docs.gradle.org/2.4/userguide/installation.html)
[https://stackshare.io/stackups/bazel-vs-gradle-vs-maven](https://stackshare.io/stackups/bazel-vs-gradle-vs-maven)

