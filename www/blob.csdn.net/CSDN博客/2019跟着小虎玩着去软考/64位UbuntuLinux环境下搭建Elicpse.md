# 64位UbuntuLinux环境下搭建Elicpse - 2019跟着小虎玩着去软考 - CSDN博客
2012年07月17日 11:32:05[littletigerat](https://me.csdn.net/littletigerat)阅读数：12944
**64****位****UbuntuLinux****环境下搭建****Elicpse**
# 一、系统版本
笔者Linux系统版本
Linuxversion 3.2.0-26-generic (buildd@batsu) (gcc version 4.6.3(Ubuntu/Linaro 4.6.3-1ubuntu5) ) #41-Ubuntu SMP Thu Jun 14 17:49:24UTC 2012
Linuxversion 3.2.0-26-generic #41-Ubuntu SMP Thu Jun 14 17:49:24 UTC 2012x86_64 x86_64 x86_64 GNU/Linux
小结：64位的3.2.0版本UbuntuLinux系统
# 二、下载
## 1.JRE
JREJava运行环境的软件包的下载地址：
[http://java.com/zh_CN/download/linux_manual.jsp?locale=zh_CN](http://java.com/zh_CN/download/linux_manual.jsp?locale=zh_CN)
下载该文件
jre-7u5-linux-x64.tar.gz
## 2.Eclipse
软件下载地址：
[http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/)
下载 Eclipse IDE for C/C++ Developers软件版本，意即
eclipse-cpp-juno-linux-gtk-x86_64.tar.gz
# 三、安装
## 1.JRE
安装步骤：
1.以高级管理root用户，进行操作，使用命令sudo-su
 root。
2.在/usr/java目录下解压jre-7u5-linux-x64.tar.gz，使用命令：
tar zvxf jre-7u5-linux-x64.tar.gz
3.在/etc目录下修改profile，添加如下三句话。
exportJAVA_HOME=/usr/java/jre1.7.0_05
exportPATH=$JAVA_HOME/bin:$PATH
exportCLASSPATH=$JAVA_HOME/lib:$CLASSPATH
目的就是：配置JAVA运行路径，JAVA库的所在路径，简而言之，JAVA运行环境参数配置。
4.一定要重启机器，然后运行java-version命令，以进行测试JRE是否安装好没有。
运行java-version命令，出现如下提示，说明JREJava运行环境安装正确。
javaversion "1.7.0_05"
Java(TM)SE Runtime Environment (build 1.7.0_05-b06)
JavaHotSpot(TM) 64-Bit Server VM (build 23.1-b03, mixed mode)
## 2.eclipse
首先打开终端，然后转到压缩包所在目录下，输入指令tarzvxfeclipse-cpp-juno-linux-gtk-x86_64.tar.gz解压得到Eclipse文件夹，转到该目录下执行./eclipse
**如果出现以下错误提示信息：**
![](https://img-my.csdn.net/uploads/201207/17/1342496277_3297.png)
说明目前的Linux机器上没有Java运行环境，需要安装JRE（Java运行环境）或者说JDK（Java开发包）。
# 四、小结
需要安装JRE即Java运行环境安装包，以及Eclipse应用软件两大软件。
千万记得安装完Java运行环境安装包，要重新启动机器，让配置生效。
