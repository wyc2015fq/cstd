# Linux Ubuntu server 15.04 安装JDK、Maven、findbugs、protobuf - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月17日 18:06:44[boonya](https://me.csdn.net/boonya)阅读数：806








Linux基础环境准备：如JDK、Maven、FindBugs、protobuf(protoc)等等，以后有新的安装再补充。此文为编译Apache Hadoop源码环境而作。

# 0.Linux系统环境

系统版本：



```
root@ubuntu:~# uname -a
Linux ubuntu 3.19.0-15-generic #15-Ubuntu SMP Thu Apr 16 23:32:37 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux
```



内存使用：



```
root@ubuntu:~# free
             total       used       free     shared    buffers     cached
Mem:       2040696     373044    1667652       4888      43932     109308
-/+ buffers/cache:     219804    1820892
Swap:      1046524          0    1046524
root@ubuntu:~#
```



# 1.安装JDK

## **1.1下载JDK**

从Oracle官网下载jdk，下载地址如下：

[http://www.oracle.com/technetwork/java/javase/downloads/](http://www.oracle.com/technetwork/java/javase/downloads/)

选择 jdk-7u79-linux-x64.tar.gz下载。

## **1.2解压JDK**

将下载的文件放置于/usr/local/jdk/目录下执行以下命令解压缩jdk:


`tar -zxvf jdk-7u79-linux-x64.tar.gz`


会生成一个jdk1.7.0_79的文件夹，将文件移动到/usr/local/jdk路径下。


## **1.3设置环境变量**

执行命令： vi /etc/profile，在文件末尾添加如下配置：



```
export JAVA_HOME=/usr/local/jdk/jdk1.7.0_79

export JAVA_OPTS="-Xms1024m-Xmx1024m"

export CLASSPATH=.:$JAVA_HOME/lib/tools.jar:$JAVA_HOME/lib/dt.jar:$CLASSPATH

export PATH=$JAVA_HOME/bin:$PATH
```



## 1.4更新配置

保存退出文件后，执行以下命令


`   source  /etc/profile`


## 1.5验证JDK安装

执行命令：java –version ，看到显示的版本信息即正确。



```
root@ubuntu:/# java -version
java version "1.7.0_79"
Java(TM) SE Runtime Environment (build 1.7.0_79-b15)
Java HotSpot(TM) 64-Bit Server VM (build 24.79-b02, mixed mode)
root@ubuntu:/#
```



# 2.安装maven

Hadoop源码是使用maven组织管理的，必须下载maven。


##  2.1下载maven

从maven官网下载，下载地址是:[http://maven.apache.org/download.cgi](http://maven.apache.org/download.cgi)

选择 apache-maven-3.3.9-bin.tar.gz 下载。

## 2.2解压maven

执行以下命令解压缩maven:


` tar -zxvf  apache-maven-3.3.9-bin.tar.gz`


会生成一个文件夹apache-maven-3.3.9，j将文件夹移至/usr/local/maven/文件下。

## 2.3设置Maven环境

执行命令：vi  /etc/profile，在文件末尾添加如下配置：

```
export MAVEN_HOME=/usr/local/maven/apache-maven-3.3.9

export PATH=$PATH:$MAVEN_HOME/bin
```

## 2.4更新配置

保存退出文件后，执行以下命令


` source  /etc/profile`


## 2.5验证Maven安装

 执行命令：mvn -version ，看到显示的版本信息即正确。



```
root@ubuntu:~# mvn -version
Apache Maven 3.3.9 (bb52d8502b132ec0a5a3f4c09453c07478323dc5; 2015-11-10T08:41:47-08:00)
Maven home: /usr/local/maven/apache-maven-3.3.9
Java version: 1.7.0_79, vendor: Oracle Corporation
Java home: /usr/local/jdk/jdk1.7.0_79/jre
Default locale: en_US, platform encoding: UTF-8
OS name: "linux", version: "3.19.0-15-generic", arch: "amd64", family: "unix"
root@ubuntu:~#
```



# 3.安装findbugs（可选步骤）

findbugs是用于生成文档的。如果不需要编译生成文档，可以不执行该步骤。

## 3.1下载findbugs

从findbugs官网下载findbugs，下载地址是：[https://zh.osdn.net/projects/sfnet_findbugs/releases/](https://zh.osdn.net/projects/sfnet_findbugs/releases/)

选择[findbugs-noUpdateChecks-3.0.1.tar.gz](https://zh.osdn.net/projects/sfnet_findbugs/downloads/findbugs/3.0.1/findbugs-noUpdateChecks-3.0.1.tar.gz)下载。



## 3.2解压findbugs



执行以下命令解压缩findbugs：


`tar -zxvf   findbugs-noUpdateChecks-3.0.1.tar.gz`

会生成一个文件夹findbugs-3.0.1，将文件放置于/usr/local/findbugs路径下。





## 3.3设置环境



执行命令：vi  /etc/profile，在文件末尾添加如下内容：

```
export FINDBUGS_HOME=/usr/local/findbugs/findbugs-3.0.1

export PATH=${PATH}:${FINDBUGS_HOME}/bin
```



## 3.4更新配置



保存退出文件后，执行以下命令


` source  /etc/profile`


## 3.5验证findbugs安装

 执行命令：findbugs -version，看到显示的版本信息即正确。



```
root@ubuntu:/# findbugs -version
3.0.1
root@ubuntu:/#
```



# 4.安装protobuf

Hadoop使用protocol buffer通信。

## 4.1下载protobuf

从protobuf官网下载protobuf，这里提供一个百度云盘的下载地址：[http://pan.baidu.com/s/1pJlZubT](http://pan.baidu.com/s/1pJlZubT)

选择protobuf-2.5.0.tar.gz 下载。

## 4.2安装C运行环境

执行以下命令进行gcc安装：




```
apt-get  install  build-essential #gcc
```



gcc  --verion 测试



```
root@ubuntu:/# gcc --version
gcc (Ubuntu 4.9.2-10ubuntu13) 4.9.2
Copyright (C) 2014 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## 4.3解压文件

然后执行以下命令解压缩protobuf


` tar -zxvf protobuf-2.5.0.tar.gz`


会生成一个文件夹protobuf-2.5.0文件夹。

## 4.4编译protobuf

如果没有安装make命令，安装make：


`apt-get install make`


执行以下命令编译protobuf。



```
cd /usr/local/protobuf/protobuf-2.5.0

 ./configure --prefix=/usr/local/protobuf/protobuf-2.5.0

 make && make install
```
只要不出错就可以了。

执行完毕后，编译后的文件位于/usr/local/protobuf/protobuf-2.5.0目录下。

## 4.5设置环境

执行命令：vi  /etc/profile，编辑结果如下所示：


`export PATH=$PATH:/usr/local/protobuf/protobuf-2.5.0/bin`


## 4.6更新配置

保存退出文件后，执行以下命令


`source  /etc/profile`


## 4.7验证安装

 执行命令： protoc --version，如果看到下面的显示信息，证明配置正确了。



```
root@ubuntu:/usr/local/protobuf/protobuf-2.5.0#  protoc --version
libprotoc 2.5.0
```




