# 将本地的项目上传到github上面的方法教程 - 博客堂 - CSDN博客





2017年05月03日 10:39:13[最小森林](https://me.csdn.net/u012052268)阅读数：1026标签：[github																[git																[数据																[服务器																[上传](https://so.csdn.net/so/search/s.do?q=上传&t=blog)
个人分类：[git](https://blog.csdn.net/u012052268/article/category/6897281)





# 将本地的项目上传到github上面的方法教程



- [将本地的项目上传到github上面的方法教程](#将本地的项目上传到github上面的方法教程)- [注册账户以及创建仓库](#1注册账户以及创建仓库)
- [安装git客户端](#2安装git客户端)
- [配置Git](#3配置git)- [1设定主目录](#1设定主目录)
- [2在本地创建ssh key](#2在本地创建ssh-key)
- [3验证是否成功](#3验证是否成功)
- [4设置username和email](#4设置username和email)
- [5连接本地与github](#5连接本地与github)

- [提交上传](#4提交上传)- [1在本地仓库添加一些文件](#1在本地仓库添加一些文件)
- [2上传到本地缓存服务器](#2上传到本地缓存服务器)
- [3上传到github](#3上传到github)- [1拉取pull服务器数据](#1拉取pull服务器数据)
- [2 上传到github](#2-上传到github)


- [常用Git命令](#5常用git命令)





## 1.注册账户以及创建仓库

要想使用github第一步当然是注册github账号了。之后就可以创建仓库了。步骤如下：

打开[https://github.com](https://github.com) 直接在首页上注册

![image](http://1ke.co/files/course/2015/06-10/170548c27e1b854772.png?4.9.3)

点按钮注册成功之后，在页面上方用户菜单上选择 “+”->New repository 创建一个新的仓库。

![image](http://1ke.co/files/course/2015/06-10/17095640f213924273.png?4.9.3)

为仓库取一个名字，点击创建仓库按钮 你将成功创建一个仓库。

![image](http://1ke.co/files/course/2015/06-10/17132882066d167248.png?4.9.3)

## 2.安装git客户端

github是服务端，要想在自己电脑上使用git我们还需要一个git客户端，

windows用户请下载 [http://msysgit.github.com/](http://msysgit.github.com/)

mac用户请下载 [http://code.google.com/p/tortoisegit/](http://code.google.com/p/tortoisegit/)

一路next，安装成功后， 回到C盘，或任何文件夹下，点鼠标右键会多出一些菜单 

如 Git Bash、Git Init Hear（新版本消失）、Git Gui ， 说明安装成功。

## 3.配置Git

我们先在电脑硬盘里找一块地方存放本地仓库，比如我们把本地仓库建立在C:\MyRepository\1ke_test文件夹下（自己选择文件夹，可以在D盘，推荐在github安装目录下）

进入1ke_test文件夹 鼠标右键操作如下步骤：

### 1）设定主目录

在本地仓库里右键选择Git Bash进入git命令行，截图效果如下：

![image](http://1ke.co/files/course/2015/06-10/175108c483fb671945.png?4.9.3)

为了设定此目录为主目录！！我们先执行git init命令（github将以此文件夹为据点）
`$ git init`
（输入时 去掉$号） 
![image](http://1ke.co/files/course/2015/06-10/18283539cfad664272.png?4.9.3)
为了把本地的仓库传到github，还需要配置ssh key！！！！用于建立本地到github的连接.

### 2）在本地创建ssh key
`$ ssh-keygen -t rsa -C "your_email@youremail.com"`
后面的your_email@youremail.com改为你的邮箱。我的邮箱是lilu@1ke.co，也是在github上注册的那个邮箱： 
![image](http://1ke.co/files/course/2015/06-10/18094049ed5a347715.png?4.9.3)
直接点回车，说明会在默认文件id_rsa上生成ssh key。 

然后系统要求输入密码，直接按回车表示不设密码 
![image](http://1ke.co/files/course/2015/06-10/17570733fea2742062.png?4.9.3)
重复密码时也是直接回车，之后提示你shh key已经生成成功。

![image](http://1ke.co/files/course/2015/06-10/1811142d4ce9788031.png?4.9.3)

然后我们进入提示的地址下查看ssh key文件。 我的电脑的地址是C:\Users\lilu\.ssh ，其中lilu是我的电脑的名称

![image](http://1ke.co/files/course/2015/06-10/18003428276f033698.png?4.9.3)

打开id_rsa.pub，复制里面的key。里面的key是一对看不懂的字符数字组合，不用管它，直接复制。

回到github网站，进入Account Settings，左边选择SSH Keys，Add SSH Key,

![image](http://1ke.co/files/course/2015/06-10/18040663f13e163573.png?4.9.3)

title随便填，粘贴key。

![image](http://1ke.co/files/course/2015/06-10/18124918c556633334.png?4.9.3)

### 3）验证是否成功

在git bash下输入
`$ ssh -T git@github.com`
回车就会看到：You’ve successfully authenticated, but GitHub does not provide shell access 。这就表示已成功连上github。

![image](http://1ke.co/files/course/2015/06-10/18141358d223257547.png?4.9.3)

### 4）设置username和email

接下来我们要做的就是把本地仓库传到github上去，在此之前还需要设置username和email，因为github每次commit都会记录他们。

```
$ git config --global user.name "your name"
$ git config --global user.email "your_email@youremail.com"
```

分别输入上述命令行 回车， 我的界面显示如下 
![image](http://1ke.co/files/course/2015/06-10/1816524de546595658.png?4.9.3)
### 5）连接本地与github

进入要上传的仓库，右键git bash，添加远程地址
`$ git remote add origin git@github.com:yourName/yourRepo.git`
后面的yourName和yourRepo表示你再github的用户名和刚才新建的仓库，加完之后进入.git，打开config，这里会多出一个remote “origin”内容，这就是刚才添加的远程地址，也可以直接修改config来配置远程地址。

![image](http://1ke.co/files/course/2015/06-10/1821542ecfe9083022.png?4.9.3)

与github上的对应

![image](http://1ke.co/files/course/2015/06-10/1823037aa1ed043887.png?4.9.3)

## 4.提交上传

### 1）在本地仓库添加一些文件

接下来在本地仓库里添加一些文件，比如README

在本地新建一个README文件（或者复制你的代码项目）

![image](http://1ke.co/files/course/2015/06-10/183003b8e527974368.png?4.9.3)

### 2）上传到本地缓存服务器

在命令行输入一下命令

```
$ git add README

$ git commit -m "first commit"
```

执行界面如下

![image](http://1ke.co/files/course/2015/06-10/183203309ad2517953.png?4.9.3)

### 3）上传到github

如果这是第一次上传（push）到github的这个项目里。

上传之前，需要先将github上已有数据的拉取到本地，这样才能比较你上传过后的与之前的数据的区别。

#### 1)拉取（pull）服务器数据

指令：
`git pull origin master --allow-unrelated-histories`
此时，我们发现，将服务器上的readme给下载下来了。正和我们意。

#### 2) 上传到github

指令：
`$ git push origin master`
git push命令会将本地仓库推送到远程服务器。

git pull命令则相反。

## 5.常用Git命令

查看、添加、提交、删除、找回，重置修改文件

```
git help <command> # 显示command的help

git show # 显示某次提交的内容 git show $id

git co -- <file> # 抛弃工作区修改

git co . # 抛弃工作区修改

git add <file> # 将工作文件修改提交到本地暂存区

git add . # 将所有修改过的工作文件提交暂存区

git rm <file> # 从版本库中删除文件

git rm <file> --cached # 从版本库中删除文件，但不删除文件

git reset <file> # 从暂存区恢复到工作文件

git reset -- . # 从暂存区恢复到工作文件

git reset --hard # 恢复最近一次提交过的状态，即放弃上次提交后的所有本次修改

git ci <file> git ci . git ci -a # 将git add, git rm和git ci等操作都合并在一起做　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　git ci -am "some comments"

git ci --amend # 修改最后一次提交记录

git revert <$id> # 恢复某次提交的状态，恢复动作本身也创建次提交对象

git revert HEAD # 恢复最后一次提交的状态
```

查看文件diff

```
git diff <file> # 比较当前文件和暂存区文件差异 git diff

git diff <id1><id2> # 比较两次提交之间的差异

git diff <branch1>..<branch2> # 在两个分支之间比较

git diff --staged # 比较暂存区和版本库差异

git diff --cached # 比较暂存区和版本库差异

git diff --stat # 仅仅比较统计信息
```

查看提交记录

```
git log git log <file> # 查看该文件每次提交记录

git log -p <file> # 查看每次详细修改内容的diff

git log -p -2 # 查看最近两次详细修改内容的diff

git log --stat #查看提交统计信息
```

tig

**Mac上可以使用tig代替diff和log，brew install tig**

Git 本地分支管理 

查看、切换、创建和删除分支

```bash
git br -r # 查看远程分支

git br <new_branch> # 创建新的分支

git br -v # 查看各个分支最后提交信息

git br --merged # 查看已经被合并到当前分支的分支

git br --no-merged # 查看尚未被合并到当前分支的分支

git co <branch> # 切换到某个分支

git co -b <new_branch> # 创建新的分支，并且切换过去

git co -b <new_branch> <branch> # 基于branch创建新的new_branch

git co $id # 把某次历史提交记录checkout出来，但无分支信息，切换到其他分支会自动删除

git co $id -b <new_branch> # 把某次历史提交记录checkout出来，创建成一个分支

git br -d <branch> # 删除某个分支

git br -D <branch> # 强制删除某个分支 (未被合并的分支被删除的时候需要强制)
```

分支合并和rebase

```
git merge <branch> # 将branch分支合并到当前分支

git merge origin/master --no-ff # 不要Fast-Foward合并，这样可以生成merge提交

git rebase master <branch> # 将master rebase到branch，相当于： git co <branch> && git rebase master && git co master && git merge <branch>
```

Git补丁管理(方便在多台机器上开发同步时用)

```
git diff > ../sync.patch # 生成补丁

git apply ../sync.patch # 打补丁

git apply --check ../sync.patch #测试补丁能否成功
```

Git暂存管理

```php
git stash # 暂存

git stash list # 列所有stash

git stash apply # 恢复暂存的内容

git stash drop # 删除暂存区

Git远程分支管理

git pull # 抓取远程仓库所有分支更新并合并到本地

git pull --no-ff # 抓取远程仓库所有分支更新并合并到本地，不要快进合并

git fetch origin # 抓取远程仓库更新

git merge origin/master # 将远程主分支合并到本地当前分支

git co --track origin/branch # 跟踪某个远程分支创建相应的本地分支

git co -b <local_branch> origin/<remote_branch> # 基于远程分支创建本地分支，功能同上
```

git push # push所有分支

```
git push origin master # 将本地主分支推到远程主分支

git push -u origin master # 将本地主分支推到远程(如无远程主分支则创建，用于初始化远程仓库)

git push origin <local_branch> # 创建远程分支， origin是远程仓库名

git push origin <local_branch>:<remote_branch> # 创建远程分支

git push origin :<remote_branch> #先删除本地分支(git br -d <branch>)，然后再push删除远程分支
```

Git远程仓库管理

```
git remote -v # 查看远程服务器地址和仓库名称

git remote show origin # 查看远程服务器仓库状态

git remote add origin git@ github:robbin/robbin_site.git # 添加远程仓库地址

git remote set-url origin git@ github.com:robbin/robbin_site.git # 设置远程仓库地址(用于修改远程仓库地址) git remote rm <repository> # 删除远程仓库
```

创建远程仓库

```
git clone --bare robbin_site robbin_site.git # 用带版本的项目创建纯版本仓库

scp -r my_project.git git@ git.csdn.net:~ # 将纯仓库上传到服务器上

mkdir robbin_site.git && cd robbin_site.git && git --bare init # 在服务器创建纯仓库

git remote add origin git@ github.com:robbin/robbin_site.git # 设置远程仓库地址

git push -u origin master # 客户端首次提交

git push -u origin develop # 首次将本地develop分支提交到远程develop分支，并且track

git remote set-head origin master # 设置远程仓库的HEAD指向master分支
```

也可以命令设置跟踪远程库和本地库

```
git branch --set-upstream master origin/master

git branch --set-upstream develop origin/develop


此文章属转载，原文地址：http://1ke.co/course/194

另附：
 解决  在使用git 对源代码进行push到gitHub时可能会出错，error: failed to push some refs to git。
出现错误的主要原因是github中的README.md文件不在本地代码目录中
可以通过如下命令进行github与本地代码合并:  git pull --rebase origin master
重新执行之前的git push 命令，成功！
```](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=数据&t=blog)](https://so.csdn.net/so/search/s.do?q=git&t=blog)](https://so.csdn.net/so/search/s.do?q=github&t=blog)




