# Centos 安装jdk jre - starRTC免费im直播会议一对一视频 - CSDN博客
2018年11月13日 18:27:28[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：31
下载rpm [http://www.oracle.com/technetwork/cn/java/javase/downloads/jdk7-downloads-1880260.html](http://www.oracle.com/technetwork/cn/java/javase/downloads/jdk7-downloads-1880260.html)
wget [http://download.oracle.com/otn-pub/java/jdk/8u191-b12/2787e4a523244c269598db4e85c51e0c/jdk-8u191-linux-x64.rpm](http://download.oracle.com/otn-pub/java/jdk/8u191-b12/2787e4a523244c269598db4e85c51e0c/jdk-8u191-linux-x64.rpm)
168M
检验系统原版本
[root@admin ~]# java -version
进一步查看JDK信息：
[root@admin ~]# rpm -qa | grep java
如果之前安装过jre,需要卸载
rpm -e --nodeps jre-1.7.0_25-fcs.x86_64
否则会出现下述错误：
conflicts with file from package jre-1.7.0_25-fcs.x86_64
安装JDK
[root@admin local]# rpm -ivh jdk-7-linux-x64.rpm
JDK默认安装在/usr/java中。
运行javac试试
