# git与github远程连接代码库使用笔记 - 素质云笔记/Recorder... - CSDN博客





2018年03月01日 17:08:13[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：286








# 一、安装与账号添加

## 1.git安装

目前windows版本的git有几种实现，但我们选择msysgit发行版，这是目前做得兼容性最好的。下载地址：[http://code.google.com/p/msysgit/downloads/list](http://code.google.com/p/msysgit/downloads/list)

在linux之中：

```
apt-get install openssh-server
apt-get install git
```

详细可参考：
[https://git-scm.com/download/linux](https://git-scm.com/download/linux)

## 2.生成SSH密钥

```
ssh-keygen -C 'XX@gmail.com' -t rsa
```

相关内容会存放在：C:\Users\user.ssh之中，有id_rsa，id_rsa.pub

linux之中：

```
ssh-keygen -t rsa -C "emailaddress"（emailaddree此入添写你自己的邮箱地址）
```

存储在/root/.ssh/id_rsa.pub

里面会有一串文本

## 3.在github中验证

来到自己的github账户：

点击右上角的Edit your profile—> Account Settings—>SSH Public keys —> new ssh key

复制id_rsa.pub文件的内容，进入GitHub网站，打开Account Settings，左边选择SSH Keys，Add SSH Key，，粘贴SSH Key
然后输入`id_rsa.pub`之中：
![这里写图片描述](https://img-blog.csdn.net/20180301143426840?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在git bash中执行以下命令完成：

```
ssh -T git@github.com
```

如果你有多台电脑同步代码，建议将title填写为地名，尽量不要使用中文

key框中，请查看home文件夹中的.ssh/id_rsa.pub 将文件里面的文本全部复制到Key框中即可；

## 4.git本地构建github库

先在自己的github中创建一个`Repository`，然后在本地git中加入自己的信息：

```
git config --global user.name "maz"
git config --global user.email "maz@qq.com"
```

拓展：查看当前用户名或邮箱

```
git config user.name/email
```

如果已经存在的文件上传，需要初始化：

```
git init
```

.

# 二、git的使用

譬如你的项目为：m/project

## 1.添加文件

```
$ mkdir ~/project //创建一个项目hello-world
$ cd ~/project //打开这个项目
$ git init    //初始化 
$ touch README
$ git add README   //更新README文件
$ git commit -m 'first commit' //提交更新，并注释信息“first commit” ！！！ 修改code的关键
$ git remote add origin git@github.com:mattzheng/tensorflow.git //连接远程github项目  
$ git push -u origin master   //将本地项目更新到github项目上去 ,更新+修改
```

`add README` 是把文件上传到一个临时空间中，然后`git commit`才确认更新。

确认更新之后，需要`git push` 才能更新github上的内容。

其中：`master`   是原始的分支，可以直接创建,`origin` 是别名

其中：`git commit -m 'first commit'` 更新并注释
## 2.分支的使用

#### 创建分支

最开始只有一条分支：master

```
# 第一种方法：
git checkout -b dev 创建一个新的分支：dev + 并来到新的分支  
# 第二种方法：
git branch dev 　# 创建
git checkout dev #来到dev分支
git checkout master # 来自master分支
```

#### 查看分支

```
# 查看分支
git branch
git add readme.txt
git commit -m "注释：提交到dev分支"
git push -u origin dev
```

#### 合并分支

要现在原来的分支中提交：

```
git push -u origin dev
```

然后再回到主分支`master`，然后`merge`起来：

```
git merge dev
```

#### 删除分支

```
git branch -d dev
```

### 3.回滚与状态查询

```
git status  # 已加载（staged）和未加载（unstaged）文件的状态、提交等，你可以询问git的状态
```

### 4.取得远程代码库的一份本地拷贝

如果你还没有一份远程代码库的本地版本（例如，如果你在另一台机器上开始工作，这台机器上还没有用过这个项目），你首先需要拷贝（clone）它。去到你的代码库想要拷贝到的文件夹下，并发送：

```
git clone https://your_username@bitbucket.org/your_username/name_of_remote_repository.git
```

### 5.git删除文件

参考：[git 删除文件](https://www.jianshu.com/p/c3ff8f0da85e)

删除本地文件，但是未添加到暂存区；

删除本地文件，并且把删除操作添加到了暂存区；

把暂存区的操作提交到了本地git库；

把本地git库的删除记录推送到了远程服务器github。
删除缓存区一个文件：

```
git rm test.txt
```

删除缓存区一个文件夹：

```
git rm test -r -f
```

同步删除操作到远程分支：

```
git commit -m "delete raindow"
```

### 延伸一：git push origin master 报错：

```
$ git push origin master
To github.com:xiaoyangLee/LearnJava.git
 ! [rejected]        master -> master (fetch first)
error: failed to push some refs to 'git@github.com:xiaoyangLee/LearnJava.git'
hint: Updates were rejected because the remote contains work that you do
hint: not have locally. This is usually caused by another repository pushing
hint: to the same ref. You may want to first integrate the remote changes
hint: (e.g., 'git pull ...') before pushing again.
hint: See the 'Note about fast-forwards' in 'git push --help' for details.
```

原因是远程仓库中的文件和我们本地的仓库有差异，[例如你的远程仓库有个文件Readme.md](http://xn--Readme-ov7i58bbjn0brnp10h20nb4wnfful6evsmjw9g.md)，但是本地仓库却没有，就可能会出现这种情况。

之所以出现这种情况，原因多种多样，例如你直接使用了github上传文件，或者像我一样，这次使用了另外一个系统推送了文件，导致了此问题的出现。解决办法显然有，把本地仓库删了再git clone一个，但是这种方法显然不可取….

解决办法：使用git pull 合并分支

```
git pull --rebase origin master
```

## 参考：

[15 分钟学会使用 Git 和远程代码库](http://blog.jobbole.com/53573/)
[windows下使用git管理github项目（入门）](https://my.oschina.net/psuyun/blog/116163)
[使用git和github管理自己的项目—基础操作学习](https://segmentfault.com/a/1190000003728094)
[Git教程](https://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000)







