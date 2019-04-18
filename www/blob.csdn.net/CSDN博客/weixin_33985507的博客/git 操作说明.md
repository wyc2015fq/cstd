# git 操作说明 - weixin_33985507的博客 - CSDN博客
2018年12月22日 17:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
# git 操作说明
## 1 新建仓库
```
初始化 一个git仓库：    使用命令   ***git init **
```
## 2 提交文件
分两步执行
### 2.1 添加到暂存区
```
使用命令见文件添加到缓存区： **git add ( fileName )** 
      多个文件用逗号隔开 ： **git add   文件名1 ， 文件名2  **
       直接添加所有 ：   **git add  .**
```
### 2.2 提交到本地仓库
```
使用命令 ： git  commit -m "提交说明"
多次修改本地文件 然后 git add  . 会将多次提交的文件变化一次性提交到本地仓库
```
## 3 查看提交文件
### 3.1 查看当前仓库状态
> 
git status
```
如果没有提交的文件，会显示  working tree clean ， 表示工作目录是干净的！
```
### 3.2  查看当前文件修改详情
> 
git   diff  文件名
```
diff  ： difference
```
### 3.3 查看版本历史记录
> 
git log
`git log`命令显示从最近到最远的提交日志记录。
```
如果嫌输出信息太多，看得眼花缭乱的，试试这个命令：  git log --pretty=oneline
```
### 3.4 退回版本
#### 3.4.1  退回版本时候必须得知道当前版本
> 
用`HEAD`表示当前版本
HEAD^ 表示上一个版本
HEAD^^表示上上一个版本
```
以此类推.....
    表示第100个版本我门可以用 HEAD~100
```
#### 3.4.2 **退回到上一个版本**
> 
git reset   --hard  HEAD^
#### 3.4.3 不想退回上一个版本，想回到之气前版本
> 
git reset   --hard 版本号（也就是一串很长的版本id）
```
如果在恢复到最新版本，但是又忘记 commit id 了, 可以试试这个命令会记录每一次你使用的每一次命令！
```
> 
gti  reflog
​
### 3.5 撤销修改
丢弃工作区区的修改（也可以手动删除）
```
$ git checkout -- 文件名
```
命令`git checkout -- readme.txt`意思就是，把`readme.txt`文件在工作区的修改全部撤销，这里有两种情况：
> 
一种是`readme.txt`自修改后还没有被放到暂存区，现在，撤销修改就回到和版本库一模一样的状态；
一种是`readme.txt`已经添加到暂存区后，又作了修改，现在，撤销修改就回到添加到暂存区后的状态。
`
### 3.6 删除文件
> 
git rm 文件名
### 3.7 恢复文件
```
git checkout -- test.txt
```
### 3.8 分支管理
**新建分支**
> 
git branch 分支名
**切换分支**
> 
git checkout 分支名
**查看分支**
> 
git branch
当前分支会显示带 *
**合并分支**
git merge  命令用于合并指定分支到当前分支(切换后的分支)
> 
git merge 分支名
**git log --graph`命令可以看到分支合并图。**
删除分支
> 
git  branch -d   分支名
### 3.9 bug分支
修复bug时，我们会通过创建新的bug分支进行修复，然后合并，最后删除；
当手头工作没有完成时，先把工作现场`git stash`（会将本地工作区修改的文件变化隐藏起来，git status命令是无法看到的）一下，然后去修复bug，修复后，再`git stash pop`，回到工作现场。
> 
查看bug分支     git  stash list
### 3.10 新功能分支feature
开发一个新功能分支
开发一个新feature，最好新建一个分支；
如果要丢弃一个没有被合并过的分支，可以通过`git branch -D <name>`强行删除。
### 3.11 多人协作
- 查看远程库信息，使用`git remote -v`；
- 本地新建的分支如果不推送到远程，对其他人就是不可见的；
- 从本地推送分支，使用`git push origin branch-name`，如果推送失败，先用`git pull`抓取远程的新提交；
- 在本地创建和远程分支对应的分支，使用`git checkout -b branch-name origin/branch-name`，本地和远程分支的名称最好一致；
- 建立本地分支和远程分支的关联，使用`git branch --set-upstream branch-name origin/branch-name`；
- 从远程抓取分支，使用`git pull`，如果有冲突，要先处理冲突。
**重要的总结**
> - `HEAD`指向的版本就是当前版本，因此，Git允许我们在版本的历史之间穿梭，使用命令`git reset --hard commit_id`。
- 穿梭前，用`git log`可以查看提交历史，以便确定要回退到哪个版本。
- 要重返未来，用`git reflog`查看命令历史，以便确定要回到未来的哪个版本。
git reset`命令既可以回退版本，也可以把暂存区的修改回退到工作区。当我们用`HEAD`时，表示最新的版本。
> 
场景1：当你改乱了工作区某个文件的内容，想直接丢弃工作区的修改时，用命令`git checkout -- file`。
场景2：当你不但改乱了工作区某个文件的内容，还添加到了暂存区时，想丢弃修改，分两步，第一步用命令`git reset HEAD <file>`，就回到了场景1，第二步按场景1操作。
**查看工作目录和暂存区去的差别**
> 
git diff
**工作目录和本地仓库差别**
> 
git diff HEAD
**暂存区和本地仓库差别**
> 
git diff --cached
详细请查看  [原文](https://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000)
