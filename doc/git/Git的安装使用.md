# Git的安装使用



# 1.什么是Git

Git是一个自由和开源的分布式版本管理工具，用于有效、高速的处理任何或大或小的项目。最初由Linux Torvalds编写，用于帮助管理Linux内核开发而开发的一个开放源码的版本管理软件。在推出后，Git在其它项目中也取得了很大成功，尤其是在Ruby社区中。目前，包括Rubinius和Merb在内的很多知名项目都使用了Git。Git同样可以被诸如Capistrano和Vlad the Deployer这样的部署工具所使用。

# 2.Git的功能

Git最大的特点在于它的分布式，开发者可以在本地进行提交，每个开发者机器上都有一个服务器的数据库。

从一般开发者的角度来看，Git包含以下功能：

1. 从服务器上克隆数据库（包括版本代码和信息）到单机；
2. 在自己的机器上创建分支，修改代码；
3. 在单机上自己创建的分支上提交代码；
4. 在单机上合并分支；
5. 新建一个分支，把服务器上最新版的代码fetch下来，然后和自己的主分支合并；
6. 生成补丁patch，把补丁发给主开发者；
7. 看主开发者的反馈，如果主开发者发现两个一般开发者之间有冲突，就会要求他们先解决冲突，然后再由其中一个人提交。如果主开发者可以自己解决或没有冲突则通过；
8. 一般开发直接解决冲突的方法，开发者之间可以使用pull命令解决冲突，解决完冲突之后再向开发者提交补丁；

从主开发者的角度来看（假设主开发者不用编写代码），Git包含以下功能：

1. 通过查看邮件或其它方式查看一般开发者的提交状态；
2. 打上补丁，解决冲突；
3. 向公共服务器提交结果，然后通知所有开发人员；
4. 首先进入Git的官方网站下载页面<http://git-scm.com/download/win>，根据操作系统选择相应的版本。下载完成之后进行安装，直接点击下一步即可。

# 3.Git的安装

\1. 首先进入Git的官方网站下载页面<http://git-scm.com/download/win>，根据操作系统选择相应的版本。下载完成之后进行安装，直接点击下一步即可。此处安装的版本为Git-1.9.0-preview20140217。

\2. 然后创建Git版本库

启动git-bash，进入创建Git版本库的目录，这里我们选择版本库的目录为E:\Git_Repo，

在此文件夹中创建project_git版本库。

只需输入命令：$ git init

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809164402370-900235921.png)

如图所示，project_git版本库已创建成功。

\3. 创建文件README，命令如下：

$ touch README

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809164515777-186623178.png)

\4. 编辑文件README，命令如下：

$ vim README

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809164530433-1874159334.png)

输入内容后，先点击Esc退出插入模式，然后输入:wq保存内容。

\5. 提交README文件，命令如下：

$ git commit -m “first commit”

此时一般会出现”Please tell me who you are”，我们需要设置用户邮箱和用户名：

​       $ git config --global user.email “aaaa@163.com”

​       $ git config --global user.name “aaaa”

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809164850042-2096098101.jpg)

添加用户邮箱和用户名后，输入git commit 提交：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809164911011-715608493.png)

\6. 向版本库推送，完成版本库的初始化

$ git push origin master

# 4.Git的基本使用

## 4.1.从版本库下载文件

命令为：

$ git clone ssh://bumengru@10.1.1.156:e:/git_repo/project_git

这样远程服务器上面的库就clone到本地了。

## 4.2.提交文件

$ git add <filename>

$ git commit -m “输入提交信息”

$ git push origin master

## 4.3.更新文件

将本地下载的源码库更新至版本库的最新版本：

$ git pull

## 4.4.替换本地改动

当我们对某个文件修改错误，可以利用如下命令替换到本地改动：

$ git checkout -- <filename>

此命令会使用Head中的最新内容替换到工作目录中的文件，已添加到缓存区的改动以及新文件都不会受影响。

假如想要丢弃到所有的本地改动与提交，可以到服务器上获取最新的版本并将本地主分支指向它：

$ git fetch origin

$ git reset --hard

## 4.5.合并分支

当发生冲突时，不会自动提交，需要进行合并

$ git merge <branchname>



分类: [配置管理](https://www.cnblogs.com/bumengru/category/1047152.html)