# Gradle/Maven/Eclipse工程相互转化 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月25日 15:27:43[boonya](https://me.csdn.net/boonya)阅读数：467








Gradle/Maven/Eclipse工程相互转化的前提是安装好相应的工具和插件。

原文地址：[http://blog.csdn.net/earbao/article/details/41550387](http://blog.csdn.net/earbao/article/details/41550387)

## Maven->eclipse

mvn eclipse:eclipse

## eclipse->maven

安装好maven插件后，在eclipse工程右键项目：转换为maven工程即可。

## gradle->eclipse
编辑build.gradle文件，在文件最前面增加一行：
apply plugin: 'eclipse'

gradle eclipse




## eclipse->gradle

打开eclipse，选择主工程，右键 -->Ecport --> Generate Gradle build files ，选择主工程以及相关的工程，Finish。



## maven->gradle
gradle集成了一个很方便的插件：Build Init Plugin，使用这个插件可以很方便地创建一个新的gradle项目，或者将其它类型的项目转换为gradle项目。
gradle init --type pom


## gradle->maven

gradle->eclipse->maven 暂时没有找到直接转换的方法，不过可以先转换成eclipse工程->借助maven或者gradle插件来转换
转换完成后相应的导入jar具体修复。            


