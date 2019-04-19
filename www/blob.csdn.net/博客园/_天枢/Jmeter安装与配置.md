# Jmeter安装与配置 - _天枢 - 博客园
## [Jmeter安装与配置](https://www.cnblogs.com/yhleng/p/8743759.html)
2018-04-08 11:07 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8743759)
**Jmeter下载与安装配置**
**1.下载地址：[https://jmeter.apache.org/](https://jmeter.apache.org/)**
**Apache Jmeter首页，点击 **[Download Releases](https://jmeter.apache.org/download_jmeter.cgi)
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180408105037685-1384081746.png)
** 然后，选择，安装版本，有linux的，有windows的。这里我们要在window中安装，所以选择zip的。**
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180408105142621-929961630.png)
**下载后，触压后，开始配置环境变量**
**JDK:1.8环境变量配置：**
**![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180408111504123-1001131557.png)**
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180408111551810-820807383.png)
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180408111630002-119408464.png)
**配置完成后，cmd下执行：java -version如果正确显示版本，说明jdk没有问题。**
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180408111722947-531526454.png)
**jmeter系统环境变量：JMETER_HOME**
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180408110410774-908908233.png)
**系统环境变量:CLASSPATH下添加以下路径。**
**%JMETER_HOME%\lib\ext\ApacheJMeter_core.jar;%JMETER_HOME%\lib\jorphan.jar;%JMETER_HOME%\lib\logkit-2.0.jar;**
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180408110509590-1083417387.png)
**配置好后，运行/bin/jmeter.bat**
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180408110620494-1535411226.png)
** 启动jmeter会首先出一个cmd窗口，之后会调起jmeter界面。**
**![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180408111923642-896462661.png)**
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180408111935171-1583990115.png)
**end.................**
