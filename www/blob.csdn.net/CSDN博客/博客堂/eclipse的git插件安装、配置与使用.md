# eclipse的git插件安装、配置与使用 - 博客堂 - CSDN博客





2018年01月25日 12:56:51[最小森林](https://me.csdn.net/u012052268)阅读数：38366










- [eclipse的git插件安装与配置](#1-eclipse的git插件安装与配置)- [1 git插件安装](#11-git插件安装)
- [2 git插件卸载](#12-git插件卸载)
- [3 配置git插件](#13-配置git插件)

- [从git云端下载项目到本地](#2-从git云端下载项目到本地)
- [修改并提交自己的代码到云端](#3-修改并提交自己的代码到云端)- [1 把修改好的文件上传到本地仓库](#31-把修改好的文件上传到本地仓库)
- [2 pull一下](#32-pull一下)
- [3 提交代码到云端](#33-提交代码到云端)- [31 简易方法直接上传到master分支](#331-简易方法直接上传到master分支)
- [32 或者上传分支](#332-或者上传分支)


- [小技巧](#4-小技巧)- [1 如何恢复未提交的修改文件](#41-如何恢复未提交的修改文件)
- [2 查看提交记录](#42-查看提交记录)
- [3 更新项目远程仓库](#43-更新项目远程仓库)





# 1. eclipse的git插件安装与配置

## 1.1 git插件安装

新版本的eclipse已经自带了GIt了，就不用安装了。

老版本的eclipse安装Git，和安装一般插件过程一样：
- 点击help——Install new software

![image](https://img-blog.csdn.net/20130828165856968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGhoY2Nja2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在弹出的框里面，输入git下载地址：[http://download.eclipse.org/egit/updates/](http://download.eclipse.org/egit/updates/)
- 点击ok，如果没反应，请点击Add前面小三角，找到刚创建的以git为名字的项，稍等一会儿，出现这个框

![image](https://img-blog.csdn.net/20130828170213625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGhoY2Nja2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

name下面2项勾选起来，点击next，进入安装了。安装好后，会重启eclipse，安装完成

## 1.2 git插件卸载

如果不想要git插件了，或者想重新安装，可以先卸载GIT：
- 
在Eclipse中依次点击菜单”Help”->”About Eclipse”。弹出图1所示对话框，点击”Installation Details”按钮，弹出图2所示对话框。选中要卸载的插件（按住Ctrl可多选），点击”Uninstall…”按钮。

- 
Adt——about adt，如图


![image](https://img-blog.csdn.net/20130828165111359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGhoY2Nja2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击Installation Details

弹出下面框，在里面找到相应的插件进行卸载

![image](https://img-blog.csdn.net/20130828165212562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGhoY2Nja2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

选择好需要卸载的插件后，点击Uninstall，进行卸载。ctrl+鼠标左键可以多选哈

## 1.3 配置git插件

安装好后，进行，一个简单配置，填入我们的用户名和邮箱
- 点击Preferences》Team》Git>Configuration

![image](https://img-blog.csdn.net/20160606144615492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 
点击Add Entry，在弹出框里面输入key和value的值
- 
名字的key ：user.name ； value：是你的github用户名。

- 
邮箱的key：user.email ; value:你的登陆GitHub邮箱账号.



以后你使用git提交代码时候，eclipse会自动提取这些信息，和代码一起发送到git远程仓库。

# 2. 从git云端下载项目到本地
- 创建github账户，并new一个新的库repository，点击clone ，记住克隆的地址（.git结尾的网址），如下图。（或者找到你想复制的别人的库，点击clone并记住库的克隆地址。）

![image](https://img-my.csdn.net/uploads/201302/03/1359856876_7117.png)
- 从Eclipse中选择File-》import-》Git-》Projects from Git

![image](https://img-my.csdn.net/uploads/201302/03/1359856918_4871.png)
- 选择next-》选择URI

![image](https://img-my.csdn.net/uploads/201302/03/1359856965_4239.png)
- 选择next，粘贴如下信息（注意填上您在github上申请的账号密码）：

![image](https://img-my.csdn.net/uploads/201302/03/1359857008_4839.png)
- 选择next，进入分支选择，这里选择的是master

如果是大型项目，可能会有多个分支，选择master即可。

![image](https://img-my.csdn.net/uploads/201302/03/1359857084_9693.png)
- 选择next，配置本地项目存放路径，一般是c:/你的userName/git

![image](https://img-my.csdn.net/uploads/201302/03/1359857140_3138.png)
- 单击Finish，然后选择Import existing projects，然后再单击Finish即可在Eclipse中看到导入的项目了。（项目复制到了eclipse的工作空间中）

![image](https://img-my.csdn.net/uploads/201302/03/1359857335_4462.png)

这里选择Import existing projects可能会出现一个问题：就==是系统提示.project文件已存在==。

**解决办法**：先后退一步；然后进入c:/你的userName/git下载的项目文件夹中，把原项目的.project文件删除（没关系，这个文件不重要），然后再点击Finish

# 3. 修改并提交自己的代码到云端

上面把项目下载并导入到到了本地，自己肯定要修改一下，或者加上一些自己的代码。那么修改好之后怎么发布回GitHub云呢？

## 3.1 把修改好的文件上传到本地仓库

即：把你在eclipse-workspace的项目，提交到C:/../Git/项目 中去。
- 点击项目右键>team>commit

![image](https://img-blog.csdn.net/20160606165843843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 在注释中输入提交信息：记录一下修改了哪些文件/为啥修改；方便以后查阅。

![image](https://img-blog.csdn.net/20160606170048155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 
勾选你需要提交的文件（确定修改的文件），或者右边小勾，点击全选。

- 
执行commit ，提交到本地仓库。这样你的修改就在本地仓库确认了。接下来我们把本地项目提交至远程远程仓库github上。


## 3.2 pull一下

把本地项目提交至远程远程仓库github上之前，我们首先要pull一下，表示从云端拉取最新的版本。

防止在你修改的过程中，云端代码有变化（别人修改）。
`右键点击项目 -> team -> pull`
## 3.3 提交代码到云端

在本地修改好文件，并在本地仓库确认后，就可以把代码上传到云端。有两种办法：

### 3.3.1 简易方法：直接上传到master分支

右键点击项目 -> team -> push Branch master

next 最后点击finish即可。

### 3.3.2 或者上传分支
- 右键点击项目 -> team -> Remot->Push

![image](https://img-blog.csdn.net/20160606171800777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 进行如下设置：

![image](https://img-blog.csdn.net/20160606172142267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

最后点击finish

# 4. 小技巧

## 4.1 如何恢复未提交的修改文件：

有的时候，我修改了代码，但是觉得改的不好，想恢复原装，只需要Replace With–HEAD Revision (命令行为 git checkout head .)：

![image](https://img-blog.csdn.net/20161025140131874?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 4.2 查看提交记录

Team–Show in History   (命令行为：git log)

## 4.3 更新项目远程仓库：

Team– Pull (命令行：git pull [https://github.com/someone/xxx.git](https://github.com/someone/xxx.git))



