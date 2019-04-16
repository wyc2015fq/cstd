# git 命令 - ggabcda的博客 - CSDN博客





2017年09月12日 12:10:02[ggabcda](https://me.csdn.net/ggabcda)阅读数：120








mkdir learngit 　　　　　　　　　　　　　　　　　　创建文件夹

cd learngit 　　　　　　　　　　　　　　　　　   　 进入文件夹

pwd 　　　　　　　　　　　　　　　　　　      　　 pwd命令用于显示当前目录

git init 　　　　　　　　　　　　　　　　　　   　　 git init命令把这个目录变成Git可以管理的仓库

git add <filename> 　　　　　　　　　　　　　    　 git add指令告诉git，把文件添加到仓库,filename可以有多个

git commit 　　　　　　　　　　　　　　　　   　　 git commit指令告诉git，把文件提交到仓库

git commit -m 　　　　　　　　　　　　　　　     　 -m后面输入的是提交的说明

git status　　　　　　　　　　　　　　　　　  　 　 git status命令可以让我们时刻掌握仓库当前的状态

git diff <filename>　　　　　　　　　　　　　　　　顾名思义就是查看difference(差异)

git log 　　　　　　　　　　　　　　　　　         　 命令显示从最近到最远的提交日志

git log --pretty=oneline　　　　　　　　　　              命令--pretty=oneline只显示提交的操作

HEAD 　　　　　　　　　　　　　　　　　　　      HEAD指向的版本就是当前版本，上一个版本HEAD^，上上一个版本HEAD^^，上100个版本HEAD~100

git reset 　　　　　　　　　　　　　　　　　　  　 回退版本

git reset --hard <edition> 　　　　　　　　　　　　  回退版本指令，edition为版本号或者HEAD指令

git reflog 　　　　　　　　　　　　　　　　    　　 命令git reflog用来记录你的每一次命令

cat <filename> 　　　　　　　　　　　                    查看文件内容

git diff HEAD -- <filename> 　　　　　　　　　　　 命令git diff HEAD -- <filename>可以查看工作区和版本库里面最新版本的区别

git checkout -- <filename> 　　　　　　　　　　      把<filename>文件在工作区的修改全部撤销（可用于还原删除的文件）,两种情况

　　　　　　　　　　　　　　　　　　　　　　          一种是<filename>自修改后还没有被放到暂存区，现在，撤销修改就回到和版本库一模一样的状态；

　　　　　　　　　　　　　　　　　　　　　          　一种是<filename>已经添加到暂存区后，又作了修改，现在，撤销修改就回到添加到暂存区后的状态。

git reset HEAD <filename> 　　　　　　　　　　     命令git reset HEAD <filename>可以把暂存区的修改撤销掉（unstage），重新放回工作区

rm <filename> 　　　　　　　　　　　　　　　　   直接在文件管理器中把<filename>文件删除

git rm <filename> 　　　　　　　　　　　　            从版本库中删除该文件，并且git commit

ssh-keygen -t rsa -C "youremail@example.com"       创建SSH Key,主目录下，看看有没有.ssh目录，这个目录下有没有id_rsa和id_rsa.pub这两个文件，有则不必设置

Add SSH Key（github）                                             添加id_rsa.pub文件的key

Create a new repo（github）                                     创建远程仓

git remote add origin git@github.com:

michaelliao/learngit.git michaelliao                             替换成你自己的GitHub账户名，origin是远程库的名字

git push                                                                       命令git push把本地库的内容推送到远程，实际上是把当前分支master推送到远程

git push -u origin master                                             第一次推送使用-u参数，Git不但会把本地的master分支内容推送的远程新的master分支，还会把本地的master分支和远程的       master分支关联起来，在以后的推送或者拉取时就可以简化命令

git push origin master                                                 命令把本地master分支的最新修改推送至GitHub

Initialize this repository with a 

README（github）                                                   创建一个新的仓库，勾选Initialize this repository with a README

git clone git@github.com:

michaelliao/gitskills.git                                                命令git clone克隆一个本地库，michaelliao替换成你自己的GitHub账户名，gitskills替换成远程仓名称，Git支持多种协议，包括   https，但通过ssh支持的原生git协议速度最快。

git checkout -b <branchname>                                   git checkout命令加上-b参数表示创建分支并切换,相当于以下两条语句

git branch <branchname>                                          创建分支

git checkout <branchname>                                       进入/切换分支

git branch                                                                   git branch命令会列出所有分支，当前分支前面会标一个*号

git merge <branchname>                                           git merge命令用于合并指定分支到当前分支

git branch -d <branchname>                                      -d指令用于删除分支

git log --graph --abbrev-commit                                  --abbrev-commit 可以为你的 SHA-1 值生成出简短且唯一的缩写 --graph命令可以看到分支合并图

git merge --no-ff -m "<message>" <branchname>     --no-ff参数，表示禁用Fast forward，合并分支时，如果可能，Git会用Fast forward模式，但这种模式下，删除分支后，会丢掉分 支信息

git stash                                                                      stash功能，可以把当前工作现场“储藏”起来

git stash list                                                                 查看stash列表

git stash pop 　　　　　　　　　　　　　　　　     恢复工作区同时把stash内容也删除

git stash apply 　　　　　　　　　　　　　　　　  恢复工作区

git stash drop 　　　　　　　　　　　　　　　　　把stash内容删除

git branch -D <branchname> 　　　　　　　　　　 丢弃一个没有被合并过的分支，可以通过git branch -D <branchname>强行删除

git remote -v 　　　　　　　　　　　　　　　　　  指令git remote 用于查看远程库的信息

git checkout -b dev origin/<branchname> 　　　　　本地创建和远程分支对应的分支

git pull 　　　　　　　　　　　　　　　　　　　　 git pull指令抓取远程的新提交

git branch --set-upstream <branchname>

origin/<branchname>                                                   建立本地分支和远程分支的关联

git push origin <branchname> 　　　　　　　　       本地推送分支

git tag <tagname> <commit id> 　　　　　　　　     命令git tag <tagname>就可以打一个新标签

git tag 　　　　　　　　　　　　　　　　　　　　  命令git tag查看标签

git show <tagname> 　　　　　　　　　　　　　　 命令git show <tagname>查看标签信息

git tag -a <tagname> -m "<message>" <commit id>   创建带有说明的标签，用-a指定标签名，-m指定说明文字 

git tag -a <tagname> -s "<message>" <commit id> 　通过-s用私钥签名一个PGP签名的标签

git show <tagname> 　　　　　　　　　　　　        命令git show <tagname>可以看到PGP签名信息,PGP签名的标签是不可伪造的，因为可以验证PGP签名.


git tag -d <tagname>　　　　　　　　　　　　　　 删除标签

git push origin <tagname> 　　　　　　　　　　　  命令git push origin <tagname>，推送某个标签到远程

git push origin --tags 　　　　　　　　　　　　　　 一次性推送全部尚未推送到远程的本地标签

git push origin :refs/tags/<tagname> 　　　　　　　 删除远程标签：先从本地删除git tag -d <tagname>，然后，从远程删除

git config --global color.ui true 　　　　　　　　　　 命令让Git显示颜色，会让命令输出看起来更醒目

.gitignore 　　　　　　　　　　　　　　                   文件填写忽略提交的特殊文件，然后提交到git

git add -f <filename>　　　　　　　　　　　　　      -f 强制提交被.gitignore忽略的文件

git config --global alias.<alias> <commandname> 　  命令用于设置别名,<commandname>设置别名为<alias>



