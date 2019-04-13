
# Git 命令 - 嵌入式Linux - CSDN博客

2015年03月23日 14:17:40[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：598个人分类：[Git																](https://blog.csdn.net/weiqifa0/article/category/1438593)



GIT命令
作者：韦启发
1、GIT代码提取
git clonegit@192.168.1.190:GH700C.git
2、查看远程分支
gitbranch –a
3、删除本地分支
gitbranch –d GH710C_BASE
D是强制删除 d是里面内容是空的才可以删除
4、从700项目切换到710项目的分支上去
git checkoutGH710C_BASE
5、同步分支代码
gitpull origin GH710C_BASE
6、在本地仓库去掉某个提交（用来验证某个提交是否有问题）
git revert反转提交
git revert1e2194aaed47f118f626a905149c71fad0295f0b
7、把本地修改放入堆栈
git stash放入栈顶
git stash list:查看git备份栈
git stash pop stash@{1}：调出此节点的工作内容，并在git栈中删除此节点记录
git stash apply stash@{1}:调出此节点的工作内容，但在git栈中保留此节点记录
git stash clear清除stash list
8、克隆一个git仓库，然后解压一个仓库
gitclone --bare GH700C/打包
gitclone GH700C.git解压
9、在同一个代码库中的A分支同步B分支的代码
gitcherry-pick 415ab046aed30a25903c7824679bb7dad0c27f41
gitpush
10、获取远程分支的代码放到本地
”git branch–r”来查看
git checkout-b本地分支名远程分支名
其中，远程分支名为git branch –r所列出的分支名，一般是诸如“origin/分支名”的样子。如果本地分支名已经存在，则不需要“-b”参数。
**11、**git revert**commitid**
**把某次提交从代码库中去掉的命令,但是要是有冲突的话要手动矫正**
**git log --name-status已经可以详细列出某个hast号修改、增加、删除的文件**


$**git log -p -2**
此外，还有许多摘要选项可以用，比如**--stat**，**仅简要的显示 文件 增改行数统计，****每个提交都列出了修改过的文件****，**以及其中添加和移除的行数，并在最后列出所有增减行数小计。
$**git log --stat**
**12、git branch**
这个命令很少能用好
git branch --help 这个显示git branch的功能　在里面虽然都是英文的，但是大部分基本的功能我们都能够看懂的
git
 branch -v
这个显示我们有多少分支，然后把每个分支的最新提交列出来

```python
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/tm100$ git branch -v
  master          36b47c1 Modify gitignore.
* mid713l_lp_lvds 5202474 Merge branch 'tm100' of ts-server:/source/tm100 into mid713l_lp_lvds
  origin/tm100    36b47c1 Modify gitignore.
  tm100           6d0be94 [问题描述]:修改mkkernel 读取makeMtk.ini获取项目名称，这样就不用每次编译的时候输入项目名称，但是保留原来输入项目名称的功能 [评审人]:韦启发
```
git branch -vv
**这个功能比上面的功能更加强大，可以显示提交的头，还可以看出我们的这个代码分支是从哪里checkout 出来的****，是不是很叼？**

```python
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/tm100$ git branch -vv
  master          36b47c1 [origin/master] Modify gitignore.
* mid713l_lp_lvds 5202474 Merge branch 'tm100' of ts-server:/source/tm100 into mid713l_lp_lvds
  origin/tm100    36b47c1 Modify gitignore.
  tm100           6d0be94 [remotes/origin/tm100] [问题描述]:修改mkkernel 读取makeMtk.ini获取项目名称，这样就不用每次编译的时候输入项目名称，但是保留原来输入项目名称的功能 [评审人]:韦启发
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/tm100$
```
13、git checkout
gitcheckout 这个命令的功能也是非常厉害的，新建一个分支，切换一个分支，或者清除修改都要用到这个命令
git
 checkout --help 这里面的介绍比我说的好。
来看一个例子，强制新建一个分支git checkout -B xxx remotes/origin/tm100  git checkout -b 是在原来的基础上新建个分支，这两个用起来用乱了容易出现问题。
```python
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/tm100$ git checkout -b mid713l_lp_lvds
M    kernel/mediatek/custom/out
Switched to a new branch 'mid713l_lp_lvds'
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/tm100$ git branch
  master
* mid713l_lp_lvds
  tm100
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/tm100$ git status .
On branch mid713l_lp_lvds
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)
    modified:   kernel/mediatek/custom/out
Untracked files:
  (use "git add <file>..." to include in what will be committed)
    bootable/bootloader/lk/out
    makeMtk.ini
    mediatek/kernel/trace32/
    mediatek/preloader/preloader_mid713l_lp_lvds.bin
no changes added to commit (use "git add" and/or "git commit -a")
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/tm100$
```

14、目标服务器修改了IP地址怎么办？
git remote -v
git remote remove origin
git remote add origin git@192.168.0.123:/home/git/.projects/nrf51822.git
最后那条命令就是修改新的服务器ip地址

15、服务器建立仓库
git init --bare qtMacScan.git
chown git:git qtMacScan.git/  -R
在本地执行
git clone git@192.168.0.26:/home/git/source/qtMacScan.git
git status .
git add .
git commit -m "Init update"
git push


