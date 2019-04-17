# Linux Ubuntu server安装Java JDK1.7_55 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年08月24日 14:24:38[boonya](https://me.csdn.net/boonya)阅读数：977








﻿﻿

### 1.下载

#### 1.1 Oracle官方下载

下载地址：[http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html)

#### 1.2 Linux Wget下载

wget --no-cookies --no-check-certificate --header "Cookie: gpw_e24=http%3A%2F%2Fwww.oracle.com%2F;

oraclelicense=accept-securebackup-cookie" "[http://download.oracle.com/otn-pub/java/jdk/7u55-b13/jdk-7u55-linux-x64.tar.gz](http://download.oracle.com/otn-pub/java/jdk/7u55-b13/jdk-7u55-linux-x64.tar.gz)"

注：因为Oracle有个应用许可认证，同意才能下载，所以不能使用wget url方式直接获取；参考地址：[http://lovelace.blog.51cto.com/1028430/1429832](http://lovelace.blog.51cto.com/1028430/1429832)
Cookie: gpw_e24=http%3A%2F%2Fwww.oracle.com%2F 可以绕过ORACLE的网站验证；

### 2.解压

将下载下来的 .tar.gz 文件解压。

使用如下命令解压：

```
sudo tar zxvf ./jdk-7u55-linux-x64.tar.gz
```



对文件夹重命名

```
sudo mv jdk1.7.0_55/ jdk/
```

将文件jdk移至/usr/local下面；

### 3. 设置环境变量

编辑 .bashrc 文件。

在终端输入如下命令：

```
vi ~/.bashrc
```

在该文件的末尾，加上以上几行代码：

我使用的是Vim编辑的文件，你可以用命令$ sudo apt-get -y install vim进行vim安装：

```
export JAVA_HOME=/usr/local/jdk
export CLASSPATH=${JAVA_HOME}/lib
export PATH=${JAVA_HOME}/bin:$PATH
```



为了让更改立即生效，请在终端执行如下命令：

```
source ~/.bashrc
```



P.S. 在修改 .bashrc 文件之前最好先备份下，备份命令：

```
cp .bashrc .bash_original
```



### 4. 验证

通过以上步骤，JDK已安装完成。

输入以下命令验证** java -version** ，如图：

![](https://img-blog.csdn.net/20150824142211015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



