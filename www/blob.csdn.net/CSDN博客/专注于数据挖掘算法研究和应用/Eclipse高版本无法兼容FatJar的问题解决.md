# Eclipse高版本无法兼容FatJar的问题解决 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年06月01日 16:51:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：13833








                
发现eclipse打包jar无法连带打包第三方lib，于是选择安装插件fatjar，现在说明fatjar安装过程：


1、安装方法：

   1）下载安装：

   https://sourceforge.net/projects/fjep/files/

   解压下载的ZIP包，将里面的Jar解压到Eclipse安装路径下的plugins文件夹中；

   重启eclipse没安装成功，删除eclipse下\configuration\org.eclipse.update\platform.xml重启也无效；

   2）在线安装：

   eclipse菜单栏 help->install new software...->work with

   新增：fatjar - http://kurucz-grafika.de/fatjar

   安装失败提示：Failed to prepare partial IU: [R]net.sf.fjep.fatjar 0.0.31.



2、网上说，目前fatjar版本只支持eclipse 3.4前，我的elipse版本是：Version: Mars.1 Release (4.5.1)

   CSDN和OSChina上都有人提供Fatjar_0.0.32.zip下载，说可以解决该问题，不过未试验；



3、采用下面这个方法解决eclipse和fatjar版本不兼容的问题：

   进入Help -> Install New Software...,

   选择Work with列表下的The Eclipse Project Updates - http://download.eclipse.org/eclipse/updates/4.5

   在弹出的可安装组件中选择：

   Eclipse Tests, Examples, and Extras 

   打开下拉选择：Eclipse 2.0 Style Plugin Support安装；



4、重新启动eclipse后再通过在线安装fatjar成功，重启eclipse，右键项目，通过Build Fat Jar打包；

      或者右键项目选择Export->other->Fat Jar Exporter打包项目。



5、Fatjar打包项目过程，命名jar以及选择Main-class，勾选One-JAR，next后可以看到要打包的第三方lib，finish即可。



6、运行jar包，命令：java -jar xxx.jar即可。
            


