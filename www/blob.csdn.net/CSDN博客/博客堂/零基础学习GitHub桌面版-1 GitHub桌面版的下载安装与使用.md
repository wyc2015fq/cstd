# 零基础学习GitHub桌面版-1 GitHub桌面版的下载安装与使用 - 博客堂 - CSDN博客





2017年08月30日 19:19:52[最小森林](https://me.csdn.net/u012052268)阅读数：6964
所属专栏：[零基础学习GitHub桌面版](https://blog.csdn.net/column/details/17109.html)









![](http://ww1.sinaimg.cn/large/006qDjsOgy1g0tdplblplj30ne07q7cc.jpg)




### 文章目录
- [GitHub桌面版的操作](#GitHub_3)
- [1 下载GitHub桌面版](#1_GitHub_7)
- [2 安装](#2__16)
- [3 github客户端的使用](#3_github_22)
- [3.1创建库](#31_24)
- [3.2修改文件](#32_35)
- [3.3提交版本conmmit](#33conmmit_46)
- [3.4同步到云端publish](#34publish_56)
- [3.5 Github主页上查看Test](#35_GithubTest_66)
- [4总结](#4_70)




# GitHub桌面版的操作

GitHub桌面版对于个人用户非常方便，**不用去记忆那么多的命令**，只需要懂得一些概念，然后点击界面即可。

## 1 下载GitHub桌面版

下载客户端，这里推荐大家去官网下载：[https://desktop.github.com/](https://desktop.github.com/)

点击download即可:

![image](http://images2015.cnblogs.com/blog/1111888/201703/1111888-20170310213732264-358487632.jpg)

（官网只提供了windows和mac版本，暂时没有linux版，让我们一起期待吧~）

## 2 安装

下载好后双击运行，这个安装程序比较奇葩，是强制安装路径的（与谷歌chrome很像），运行后什么界面都没有，后台安装中间会重启一次，大概等到你觉得它已经安装好后，双击桌面的github就可以了。

![image](https://img-blog.csdn.net/20170703074508918?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE1ODMyNDUxNTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 3 github客户端的使用

### 3.1创建库

先来创建一个仓库，仓库里的文件变动都会被github记录下来

点击又上角的File，然后有一个New repository

给仓库取个名字，给它在本地找一个地址，

然后把第三行的单选框勾上，相当于一个初始化，会创建一个readme文件，这个文件一半是用来介绍自己项目是做什么的，怎么用。

![image](https://img-blog.csdn.net/20170703074711557?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE1ODMyNDUxNTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 3.2修改文件

现在来进行仓库的修改

进入仓库的目录，新建一个txt文件，此时，github上就有显示了

（或者直接用文本编辑器修改TXT，GitHub也能检测到）

（或者将你原来写好的程序组复制进去，GitHub也能检测到）

![image](https://img-blog.csdn.net/20170703074754728?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE1ODMyNDUxNTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 3.3提交版本conmmit

commit的意思是“版本”，你修改了依稀代码，那么此时此刻这个项目就进入了新的“版本”，所以如果你确定刚刚做的修改，就应该点击conmmit，提交当前版本。

对修改进行适当地解释，并点击左下角 commit to master

![image](https://img-blog.csdn.net/20170703074815613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE1ODMyNDUxNTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在History历史界面里，你可以看到自己过去的修改，什么时间修改的，修改了几行代码等等。那么等你修改好了几次之后，觉的差不多世纪成熟可以上传了，就可以进入下一步 “同步到云端publish”

### 3.4同步到云端publish

以上修改和提交版本，都是在本地进行的，云端的github网站上并没有变化。所以如果你确定你在本地的修改，就应该推送publish到云端。
- 
如果是云端仓库已经存在当前项目，点击publish就会同步你本地的修改到云端，并刷新云端数据

- 
如果是你第一次publish，点击之后就会弹出以下界面，问你是否在云端创建与本地相同的仓库，当然选择是喽~


![image](https://img-blog.csdn.net/20170703074840880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE1ODMyNDUxNTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单选框不要选，只有付费用户才可以创建私有仓库，然后点击publish respository仓库就同步到云端了。
### 3.5 Github主页上查看Test

现在到自己的Github主页上查看Test仓库，可以看到我们新建的"test.txt"已经出现在个人主页上了。
![image](http://images2015.cnblogs.com/blog/1111888/201703/1111888-20170311114708342-1550743006.png)
## 4总结

**注意**：你随时可以修改自己的本地代码，然后按照上面的流程走一遍，云端的代码就会更新，来让我们一起读一遍上述流程：modify -> conmmit -> publish -> view

中文就是：修改 -> 提交版本 -> 发布到云端 -> 在网站上查看

怎么样，用客户端玩转GitHub是不是超简单？继续跟着我一起探索GitHub桌面版吧~





