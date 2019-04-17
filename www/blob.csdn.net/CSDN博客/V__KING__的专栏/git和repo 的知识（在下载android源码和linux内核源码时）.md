# git和repo 的知识（在下载android源码和linux内核源码时） - V__KING__的专栏 - CSDN博客





2013年10月25日 16:25:11[v__king__](https://me.csdn.net/V__KING__)阅读数：2158标签：[android																[kernel																[python																[源码](https://so.csdn.net/so/search/s.do?q=源码&t=blog)
个人分类：[android](https://blog.csdn.net/V__KING__/article/category/1704545)






原文地址：http://bbs.chinaunix.net/thread-1947726-1-1.html


因为Android是由kernel、Dalvik、Bionic、prebuilt、build等多个Git项目组成，所以Android项目编写了一个名为Repo的Python的脚本来统一管理这些项目的仓库，使得Git的使用更加简单。

这几天William为了拿Android最新的sourcecode，学习了一下git和repo的一些基本操作，整理了一个如何取得Android代码的How-To，今天把他贴上来。
**1、Git的安装**

从http://git-scm.com/下载最新的git源码包：并解压：

#tar -xjf git-1.6.3.3.tar.bz2

编译并安装：

#cd git-1.6.3.3

#./configure

#make;make install

# git --version

git version 1.6.3.3
**2、安装Repo**

首先确保在当前用户的主目录下创建一个/bin目录（如果没有的话），然后把它(~/bin)加到PATH环境变量中

接下来通过cURL来下载Repo脚本，保存到~/bin/repo文件中
**curl http://android.git.kernel.org/repo >~/bin/repo**

别忘了给repo可执行权限
**chmod a+x ~/bin/repo3、初始化版本库**

如果是想把Android当前主线上最新版本的所有的sourcecode拿下来，我们需要repo的帮助。

先建立一个目录，比如~/android，进去以后用repo init命令即可。
**repo init -u git://android.git.kernel.org/platform/manifest.git**

这个过程会持续很长的时间（至少可以好好睡一觉），具体要多少时间就取决于网络条件了

最后会看到 repo initialized in /android这样的提示，就说明本地的版本库已经初始化完毕，并且包含了当前最新的sourcecode。

如果想拿某个branch而不是主线上的代码，我们需要用-b参数制定branch名字，比如：
**repo init -u git://android.git.kernel.org/platform/manifest.git -b cupcake**

另一种情况是，我们只需要某一个project的代码，比如kernel/common，就不需要repo了，直接用Git即可。
**git clone git://android.git.kernel.org/kernel/common.git**

这也需要不少的时间，因为它会把整个Linux Kernel的代码复制下来。

如果需要某个branch的代码，用git checkout即可。比如我们刚刚拿了kernel/common.get的代码，那就先进入到common目录，然后用下面的命令：
**git checkout origin/android-goldfish-2.6.27 -b goldfish**

这样我们就在本地建立了一个名为goldfish的android-goldfish-2.6.27分支，代码则已经与android-goldgish-2.6.27同步。我们可以通过git branch来列出本地的所有分支。
**4、同步版本库**

使用**repo sync**命令，我们把整个Android代码树做同步到本地，同样，我们可以用类似
**repo sync project1 project2 … **

这样的命令来同步某几个项目

如果是同步Android中的单个项目，只要在项目目录下执行简单的
**git pull**

即可。](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=kernel&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




