# 搭建Java ME 开发环境 - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年09月11日 14:51:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2622








搭建Java ME 开发环境：
　1)http://www.newsmth.net/pc/pccon.php?id=2206&nid=216771  //J2ME 开发环境部署
  2)JDK官方下载JDK1.5
  3)安装JDK，环境变量的设置
    "JAVA_HOME"=C:/Program Files/Java/jdk1.5.0_02
    "CLASSPATH"=%JAVA_HOME%/lib
    "Path"=%JAVA_HOME%/bin
  4)安装eclipse3.2；
  5)安装EclipseME插件：
    a.下载地址：[http://sourceforge.net/projects/eclipseme/files/](http://sourceforge.net/projects/eclipseme/files/)
       下载eclipseme.feature_1.7.9_site.zip版本
    b.安装方法：[http://blog.163.com/zblin_87/blog/static/4624467120088883019390/](http://blog.163.com/zblin_87/blog/static/4624467120088883019390/)
    c.开发J2ME时要首先启动手机模拟器，在Debug模式的Eclipse默认设置不等到模拟器启动就会失败，
      修改方法：在Window ->Preference->Java->Debug中，取消暂停选项，还应该把超时时间提高到20000毫秒；
  6)安装无线开发包与手机模拟器
    a.下载Sun WTK：WTK(Wireless toolkit)是Sun为无线开发者提供的一个无线开发实用包；
      最新下载地址：[http://www.oracle.com/technetwork/java/index-jsp-137162.html](http://www.oracle.com/technetwork/java/index-jsp-137162.html)
      这里采用WTK2.5.2，需安装JDK1.5上版本；
    b.将WTK绑定到EclipseME：eclipse->window->preferences->J2ME 绑定WTK2.5.2
  7)加载Nokia Developer's Suite：Developer's Suite本身是一个可以独立运行的工具包，像WTK一样；
    这里先采用WTK入门开发，后续可以加载支持J2ME开发的手机型号；
  8)加载混淆器：[http://sourceforge.net/projects/proguard/files/](http://sourceforge.net/projects/proguard/files/)
  9)创建工程：J2ME->J2ME Midlet Suite开发；



