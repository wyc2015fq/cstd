# linux下包管理命令yum与apt-get以及开发环境配置 - DoubleLi - 博客园






一般来说市面上常见的Linux系统分为两大类：

RH类：Redhat、centOS和Fedora等

[Debian](http://so.21ops.com/cse/search?s=9181936462520079739&entry=1&q=Debian)类：[ubuntu](http://so.21ops.com/cse/search?s=9181936462520079739&entry=1&q=ubuntu)、Debian等。

上述两类系统对应的包管理工具命令分别是[yum](http://so.21ops.com/cse/search?s=9181936462520079739&entry=1&q=yum)和[apt-get](http://so.21ops.com/cse/search?s=9181936462520079739&entry=1&q=apt-get)，而安装包类型则对应为[rpm](http://so.21ops.com/cse/search?s=9181936462520079739&entry=1&q=rpm)包和deb包。

# 软件安装和卸载：

一般来说最基本的安装rpm包和deb包时选用的命令参数都是-i，即是：

```
```bash
rpm -i xxxx.rpm
```



```bash
dpkg -i xxxx.deb
```
```

卸载一般为：

```
```bash
rpm -e packName
```



```bash
dpkg -r packName
```
```

还有一种两类系统都支持的安装方式是先将源码打包成tar格式，解压之后按照./[configure](http://so.21ops.com/cse/search?s=9181936462520079739&entry=1&q=configure), make, make install.的顺序进行安装。当然不是每种软件都可以套用，只是笼统地说说比较常见的安装包。

还有更多用法可以通过-h , -?或者man手册获得。

# 软件更新：

我比较常用的系统是ubuntu，偶尔会玩一下[fedora](http://so.21ops.com/cse/search?s=9181936462520079739&entry=1&q=fedora)，yum和apt-get包管理软件使用方法上其实是大同小异的，下面只做ubuntu下的软件安装笔记。

第一个比较好用的管理软件：[aptitude](http://so.21ops.com/cse/search?s=9181936462520079739&entry=1&q=aptitude)。

如果你只是查看已经安装的软件的话，在控制台输入aptitude即可，它会弹出如下界面：

不过一般来说我们不会去用这个东西，一般习惯使用的都是apt-get进行包管理，管理安装包需要管理员权限，它的一般语法是：

```
```bash
apt-get [options]
```

```bash
install
```

```bash
|remove pkg1 [pkg2 ...]
```
```

安装软件则对应：

```
```bash
sudo
```

```bash
apt-get
```

```bash
install
```

```bash
pkgName
```
```

如果被提示没有该名称的软件，第一可能是名字不对；第二可能是[软件源](http://so.21ops.com/cse/search?s=9181936462520079739&entry=1&q=%E8%BD%AF%E4%BB%B6%E6%BA%90)未更新。

对于第一种情况，可以百度一下，比如安装mysql，实际上源上的名字是mysql-server：

![linux下包管理命令yum与apt-get以及开发环境配置的照片 - 1](http://img01.21ops.com/images/2013/12/25/111.bmp)

第二种情况，可以先更新一下软件源：

```
```bash
sudo
```

```bash
apt-get update
```
```

完成之后也可以顺便更新一下那些还不是最新版本的软件：

```
```bash
sudo
```

```bash
apt-get upgrade
```
```

包管理器会自动将所有软件升级到最新版。

apt-get常用的命令列表如下：

![linux下包管理命令yum与apt-get以及开发环境配置的照片 - 2](http://img01.21ops.com/images/2013/12/25/222.bmp)

# 基础开发环境搭建：

## 安装C／C＋＋程序的开发环境 

```
```bash
1、
```

```bash
sudo
```

```bash
apt-get
```

```bash
install
```

```bash
build-essential
```

```bash
//
```

```bash
安装主要编译工具 gcc, g++,
```

```bash
make
```



```bash
2、
```

```bash
sudo
```

```bash
apt-get
```

```bash
install
```

```bash
autoconf automake1.9
```



```bash
3、
```

```bash
sudo
```

```bash
apt-get
```

```bash
install
```

```bash
manpages-dev
```

```bash
//
```

```bash
安装C语言函数帮助文档
```



```bash
4、
```

```bash
sudo
```

```bash
apt-get
```

```bash
install
```

```bash
binutils-doc cpp-doc gcc-doc glibc-doc stl-manual
```

```bash
//
```

```bash
安装其他相关帮助文档 ，如C++，STL等
```
```

## 安装JAVA开发环境 

如果想不经过配置直接就可以用的话，不妨去ubuntu的软件中心找eclipse，直接安装它，然后它会帮你把环境那些全部都配置好，装好就直接可以用了。

不过后来因为我想练习一下[J2EE](http://so.21ops.com/cse/search?s=9181936462520079739&entry=1&q=J2EE)开发，就另外下了个j2ee版本的eclipse，另外还要搭tomcat服务器，想用jdk1.7，发现从软件中心安装的eclipse它默认给配的是jdk1.6 。 

所以重新给配一下：

1、先下载好自己要用的jdk，比如这里要用jdk1.7，下载下来以后解包，一般来说为了方便统一管理，最好解压到系统默认的文件夹里去。

好像下载的可以有tar包的吧，解压到/usr/lib/[jvm](http://so.21ops.com/cse/search?s=9181936462520079739&entry=1&q=jvm)文件夹里面即可：

```
```bash
tar
```

```bash
-xzvf archive.
```

```bash
tar
```

```bash
/usr/lib/jvm
```
```

![linux下包管理命令yum与apt-get以及开发环境配置的照片 - 3](http://img01.21ops.com/images/2013/12/25/333.bmp)

2、在这里我们想要使用jdk7，那么就做如下配置：

输入：

sudo vi /etc/environment 

或者

sudo gedit /etc/environment 

在文件尾添加：

```
```bash
export
```

```bash
JAVA_HOME=
```

```bash
/usr/lib/jvm/java-7-openjdk-i386
```



```bash
export
```

```bash
JRE_Home=
```

```bash
/usr/lib/jvm/java-7-openjdk-i386/jre
```



```bash
export
```

```bash
CLASSPATH=$CLASSPATH:$JAVA_HOME
```

```bash
/lib
```

```bash
:$JAVA_HOME
```

```bash
/jre/lib
```
```

输入：

sudo vi /etc/profile 

或者

sudo gedit /etc/profile

文件尾添加以下语句：

```
```bash
export
```

```bash
JAVA_HOME=
```

```bash
/usr/lib/jvm/java-7-openjdk-i386
```



```bash
export
```

```bash
JRE_HOME=
```

```bash
/usr/lib/jvm/java-7-openjdk-i386/jre
```



```bash
export
```

```bash
CLASSPATH=.:$JAVA_HOME
```

```bash
/lib
```

```bash
:$JRE_HOME
```

```bash
/lib
```

```bash
:$CLASSPATH
```



```bash
export
```

```bash
PATH=$JAVA_HOME
```

```bash
/bin
```

```bash
:$JRE_HOME
```

```bash
/bin
```

```bash
:$PATH
```
```

至此，基础开发环境搭建完毕。如果你需要重新选择自己的jdk路径的话，可以输入如下命令进行修改：

```
```bash
sudo
```

```bash
update-alternatives --config java
```
```

通常会给出两个或两个以上的路径给你选择，选择自己需要的那个即可。

如果你只配置过一个，那就没必要了。

最后给出一个总结yum和apt-get相关知识的博文：

http://hi.baidu.com/code_slave/item/ce8ec33d17e9e4bf633aff39

个人觉得写得有点儿太冗杂了，毕竟使用方法上使用man和-h或-？选项都能找到帮助信息。









