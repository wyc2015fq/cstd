# Git常用操作整理 - 长歌行 - CSDN博客





2017年07月29日 22:50:57[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：331








更多更新的内容请见[handbook/git](https://github.com/knowledge-warehousing/handbook/tree/master/git)

## 常用命令

详细文档 
[https://git-scm.com/docs](https://git-scm.com/docs)

*Just get these from TortoiseGit client*

### git默认使用分页器
- “q” 退出分页器
- “h” 显示帮助
- ” ” 下一页

禁用分页器 

 $export LESS=FRX

### git add

添加至暂存区

### git branch

分支管理

### git checkout

切换到远端的分支
`git checkout    -b new_branch_name remotes/origin/new_branchename --`
-b 创建一个新分支

–  表示想签出的文件。 此处应该表示所有文件

### Pull 其他分支代码
`git.exe pull -v --progress       "origin" will8372/Popup_optimization`
-v git-fetch and git-merge

–progress will report progress

### git citool

图形化提交，相当于git gui

### git clean

清楚工作区未跟踪文件

### git commit

提交

### git config

查询和修改配置

### git diff

差异比较

### git fetch

获取远程版本库的的提交

### git init

版本库初始化

### git mv

重命名

### git pull

获取最近的提交

### git push

推送提交

### git reset

重置改变分支”游标”指向

### git rm

删除文件

### git status

当前版本库的状态

### git stash

保存和恢复进度

### git tag

里程碑管理

## 工作流程的例子

### Generating a new SSH key

`$ ssh-keygen -t rsa -b 4096 -C "your_email@example.com"# Creates a new ssh key, using the provided email as a labelGenerating public/private rsa key pair.`

### Adding your SSH key

```
$ eval "$(ssh-agent -s)"Agent pid 59566
$ ssh-add ~/.ssh/id_rsa
```

### Config

```
$ git config --global user.name "bentan2013"
$ git config --global user.email "aa@bb.com"
```

### add commit and push in one commands

cd to your repo  

add this line 
`git config alias.acp '! git commit -a -m "commit" && git push'`
then use `git acp` instead of 

```
git commit -a -m "Changed something"
git push
```

or

```
git add .
git commit -a -m "commit" (do not need commit message either)
git push
```

### Create a new repository

```
echo "# test" >> README.md
git init
git add README.md
git commit -m "first commit"
git remote add origin git@github.com:bentan2013/test.git
git push -u origin master
```

### Push an existing repository

```
git remote add origin git@github.com:bentan2013/test.git
git push -u origin master
```

### Most common workflows

““

$ git clone ${repo_url}

$ git branch -a  $ git branch dev 

$ git checkout dev

$ git add filename  $ git add .

$ git status  $ git commit -a -m “add 3 files” 

$ git push origin dev:dev

$ git diff dev master

$ git checkout master  $ git merge -m “msg” dev

$ git branch -d dev #remove branch

```
### Reivew history
```

git reflog

D:\WORK_HOME\learn-git>git reflog

2bc3879 HEAD@{0}: commit: Changes to be committed:

HEAD@{1}: commit: new file: doc/test.md

HEAD@{2}: clone: from git@github.com:bentan2013/learn-git.git

```
Then if we add a new line to test.txt then use git reflog, we will get this:
```

D:\WORK_HOME\learn-git>git reflog

37293e6 HEAD@{0}: commit: modified: test.txt

HEAD@{1}: commit: Changes to be committed:

HEAD@{2}: commit: new file: doc/test.md

HEAD@{3}: clone: from git@github.com:bentan2013/learn-git.git

```
### Use git status view the current status
```

git status

D:\WORK_HOME\learn-git>git status

On branch master

Your branch is up-to-date with ‘origin/master’.

Changes not staged for commit:

(use “git add …” to update what will be committed)

(use “git checkout – …” to discard changes in working directory)

modified: test.txt

```
### Discard changes
git checkout -- file可以丢弃工作区的修改
```

git checkout – filename

```
git checkout -- file命令中的“--”很重要，没有“--”，就变成了“创建一个新分支”的命令.

用命令git reset HEAD file可以把暂存区的修改撤销掉（unstage），重新放回工作区
```

git reset HEAD  filename

```
git reset命令既可以回退版本，也可以把暂存区的修改回退到工作区。当我们用HEAD时，表示最新的版本。

场景1：当你改乱了工作区某个文件的内容，想直接丢弃工作区的修改时，用命令git checkout -- file。

场景2：当你不但改乱了工作区某个文件的内容，还添加到了暂存区时，想丢弃修改，分两步，第一步用命令git reset HEAD file，就回到了场景1，第二步按场景1操作。

场景1：当你改乱了工作区某个文件的内容，想直接丢弃工作区的修改时，用命令git checkout -- file。
```

D:\WORK_HOME\learn-git>git checkout – test.txt

```
场景2：当你不但改乱了工作区某个文件的内容，还添加到了暂存区时，想丢弃修改，分两步，第一步用命令git reset HEAD file，就回到了场景1，第二步按场景1操作。
```

D:\WORK_HOME\learn-git>git reset HEAD test.txt

D:\WORK_HOME\learn-git>vim test.txt

D:\WORK_HOME\learn-git>git checkout – test.txt

D:\WORK_HOME\learn-git>git status

On branch master

Your branch is ahead of ‘origin/master’ by 1 commit.

(use “git push” to publish your local commits)

nothing to commit, working directory clean

```
确实要从版本库中删除该文件，那就用命令git rm删掉，并且commit
```

git rm test.txt 

git commit -m “remove test.txt”

```
另一种情况是删错了，因为版本库里还有呢，所以可以很轻松地把误删的文件恢复到最新版本
```

git checkout – test.txt

```
git checkout其实是用版本库里的版本替换工作区的版本，无论工作区是修改还是删除，都可以“一键还原”。








### Remove files
```

在git rm -rf –cached CCB.xcodeproj/project.xcworkspace/xcuserdata/ 

–cached 本地保留服务器删除

```
Examples:

https://github.com/skywinder/github-changelog-generator#output-example



Tips

Use .gitignore to ignore files
```

text.txt

*html

!test.html

*.[oa] 

“`

### Change remote repository

`git remote set-url origin new-repo-url`




