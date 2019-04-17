# linux安装jdk以及eclipse - 博客堂 - CSDN博客





2017年08月14日 12:49:35[最小森林](https://me.csdn.net/u012052268)阅读数：496









- [linux安装jdk以及eclipse](#linux安装jdk以及eclipse)- [安装jdk](#1安装jdk)- [1源码包准备](#1源码包准备)
- [2解压源码包](#2解压源码包)
- [3设置jdk环境变量](#3设置jdk环境变量)
- [重启linux](#4-重启linux)
- [5检查](#5检查)

- [安装eclipse](#2安装eclipse)- [1](#1)
- [2放入指定文件夹](#2放入指定文件夹)
- [3解压安装包](#3解压安装包)
- [4启动eclipse](#4启动eclipse)





# linux安装jdk以及eclipse

## 1.安装jdk

### 1源码包准备

首先到官网下载jdk，[http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)，我下载jdk-8u25-linux-x64.tar.gz，下载到主目录 
![image](http://www.linuxidc.com/upload/2015_01/15011909588648.png)
### 2、解压源码包

通过终端在/usr/local目录下新建java文件夹，命令行：
`sudo mkdir /usr/local/java`
然后将下载到压缩包拷贝到java文件夹中，命令行：
`cp jdk-8u25-linux-x64.tar.gz /usr/local/java`
然后进入java目录，命令行：

```bash
cd /usr/local/java
```

解压压缩包，命令行：
`sudo tar -zxvf jdk-8u25-linux-x64.tar.gz`
然后可以把压缩包删除，命令行：
`sudo rm jdk-8u25-linux-x64.tar.gz`
### 3、设置jdk环境变量

要对profile进行配置：输入命令:

```bash
sudo vim /etc/profile
```

按i进入插入模式，并写入：

```
export JAVA_HOME=/usr/local/java/jdk1.8.0_144
export JRE_HOME=//usr/local/java/jdk1.8.0_144/jre
export CLASSPATH=.:$JAVA_HOME/lib:$JRE_HOME/lib:$CLASSPATH
export PATH=$JAVA_HOME/bin:$JRE_HOME/bin:$PATH
```

### 4 重启linux

### 5、检查

在终端输入如下命令
`java -version`
## 2.安装eclipse

### 1

下载:eclipse-SDK-3.7.2-linux-gtk.tar.gz

在地址：[http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/)
![image](http://images.cnblogs.com/cnblogs_com/hongten/395172/r_39.png)
![image](http://images.cnblogs.com/cnblogs_com/hongten/395172/r_40.png)

### 2放入指定文件夹

把压缩包放入/home/xqt/software

### 3解压安装包
`  tar -zxvf eclipse-SDK-4.2-linux-gtk.tar.gz`
### 4启动eclipse

进入eclipse文件夹，点击eclipse文件。

并把他固定在启动器上





