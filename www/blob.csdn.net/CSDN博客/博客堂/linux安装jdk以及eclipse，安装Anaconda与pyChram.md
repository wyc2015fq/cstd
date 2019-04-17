# linux安装jdk以及eclipse，安装Anaconda与pyChram - 博客堂 - CSDN博客





2017年08月14日 13:44:02[最小森林](https://me.csdn.net/u012052268)阅读数：1196










- [linux安装jdk以及eclipse](#linux安装jdk以及eclipse)- [安装jdk](#1安装jdk)- [1源码包准备](#1源码包准备)
- [2解压源码包](#2解压源码包)
- [3设置jdk环境变量](#3设置jdk环境变量)
- [重启linux](#4-重启linux)
- [5检查](#5检查)

- [安装eclipse](#2安装eclipse)- [1](#1)
- [2放入指定文件夹](#2放入指定文件夹)
- [3解压安装包](#3解压安装包)
- [4启动eclipse](#4启动eclipse)


- [linux安装Anaconda与pyChram](#linux安装anaconda与pychram)- [1linux安装Anaconda](#1linux安装anaconda)- [下载直接到清华镜像下载](#1-下载直接到清华镜像下载)
- [安装anaconda](#2-安装anaconda)
- [重启linux](#3-重启linux)
- [验证](#4-验证)

- [linux安装pyChram](#linux安装pychram)- [下载](#1-下载)
- [解压](#2-解压)
- [安装](#3-安装)






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

# linux安装Anaconda与pyChram

## 1linux安装Anaconda

### 1 下载（直接到清华镜像下载）

[https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/)

，选择合适的版本。 
![image](https://img-blog.csdn.net/20170411161509240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhZXJ3b2FpbHVv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 2 安装anaconda

进入下载目录，输入命令
`bash Anaconda3-4.3.1-Linux-x86.sh `
在安装的过程中，会问你安装路径，直接回车默认就可以了。有个地方问你是否将anaconda安装路径加入到环境变量（.bashrc)中，输入yes，默认的是no， 

如果10秒内没输入就要配置环境，根据提示，在终端输入

```bash
sudo gedit ~/bashrc
```

打开profile文件。添加语句
`export PATH=/home/xqt/anaconda3/bin:$PATH`
，保存，退出。 （想卸载的时候别忘记把这句话删除）

### 3 重启linux

配置好PATH后，可以通过which conda或conda –version命令检查是否正确。 

输入conda list 就可以查询，你现在安装了哪些库，常用的numpy, scipy名列其中。

如果你还有什么包没有安装上，可以运行conda install * 来进行安装， 

如果某个包版本不是最新的，运行 conda update * 就可以了。

### 4 验证

输入import scipy ,没有报错则安装成功 

打开jupyter notebook 也只需要在终端输入：

jupyter notebook

由于anaconda在linux下是安装在一个文件夹里/home/anaconda ,如果安装过程中出错问题，或者想更新另一个版本，删除anaconda也很方便，执行下面命令 
`rm -rf ~/anaconda`
## linux安装pyChram

### 1 下载

百度搜索pycharm 

然后打开pycharm的官网 

然后在官网首页点击down
### 2 解压

打开刚才下载的目录，复制到/home/xqt/software

右击文件，点击提取到此处（这与Windows的解压是一个意思） 

解压完成后，可以看到文件夹

### 3 安装

打开刚才解压好的文件夹 

然后再打开bin目录

在此处打开终端然后输入：
`./pycharm.sh`
开始安装 

（.sh 是一个脚本文件，相当于win下的.exe） 

回车







