# WEB服务器安装oracle jdbc - 左直拳的马桶_日用桶 - CSDN博客
2015年11月23日 19:09:06[左直拳](https://me.csdn.net/leftfist)阅读数：2251
WEB服务器，如果想采用jdbc访问另一台Oracle数据库服务器，那么它应该先安装Oracle客户端，或者要安装oracle jdbc。
那么怎样安装oracle jdbc呢？
1、到oracle下载jdbc包。我们那台WEB服务器装的是JDK6,那么下载的是ojdbc6.jar
2、将ojdbc6拷贝到 $JAVA_HOME\jre\lib\ext ，重启服务器
搞定
