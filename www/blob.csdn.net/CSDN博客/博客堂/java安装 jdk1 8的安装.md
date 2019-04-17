# java安装:jdk1.8的安装 - 博客堂 - CSDN博客





2015年03月27日 18:14:24[最小森林](https://me.csdn.net/u012052268)阅读数：500








1.JDK目前大家都是1.8以上了，版本：jdk1.8.0_201

  下载地址：[http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html)，点击如下下载按钮：

![](http://www.runoob.com/wp-content/uploads/2013/12/java-download.jpg)



在下载页面中你需要选择接受许可，并根据自己的系统选择对应的版本，本文以 Window 64位系统为例：

![](http://www.runoob.com/wp-content/uploads/2013/12/java-download-1.jpg)



2.安装，下一步，下一步。 安装地址一般D盘下 （eg：D:/java/jdk1.8.0_201）

  第二次出现一个让你选择安装路径的是 JRE。是单独的JRE（JDK中已经包含了一个JRE了），所以这里又安装了一个独立的JRE，具体有什么用 等我问了老师再来补充。

  地址写：D:/java/jre1.8.0_201

  2016年3月18日补充：

  一个是JDK中包含的JRE，一个是独立的JRE。
  编译环境和运行环境是不同的，运行只需要独立的JRE就可以，因此独立发布一个JRE供运行外部程序使用；



3.这样就安装好了，下面开始配置环境变量。

右击“我的电脑”，选择“属性”。

![](http://www.runoob.com/wp-content/uploads/2013/12/win-java1.png)



4.点击“高级”选项卡，选择“环境变量”。

![](http://www.runoob.com/wp-content/uploads/2013/12/java-win2.png)

![](http://www.runoob.com/wp-content/uploads/2013/12/java-win3.png)



5.新建JAVA_HOME地址D:/java/jdk1.8.0_201



6.打开名为“Path”的环境变量，在原来的变量值后面加上一个“;”（分号），然后添加  %JAVA_HOME%\bin;%JAVA_HOME%\jre\bin;



7.打开名为“classpath”的环境变量，在原来的变量值后面加上 .;%JAVA_HOME%\lib;

（.这个“点”代表当前路径，指优先在当前本目录内寻找，所以此点不可省略 ）



8.验证是否成功打开命令行模式(开始菜单>运行>输入"cmd")输入    javac 或者**java -version** 回车。





参考：[http://www.runoob.com/java/java-environment-setup.html](http://www.runoob.com/java/java-environment-setup.html)



