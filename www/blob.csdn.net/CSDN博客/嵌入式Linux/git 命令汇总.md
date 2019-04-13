
# git 命令汇总 - 嵌入式Linux - CSDN博客

2019年01月03日 11:38:56[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：37个人分类：[嵌入式学习																](https://blog.csdn.net/weiqifa0/article/category/1385985)



## 瞎扯
最近有几个留言想让写下git的内容，git是一个工具，主要是用来管理码农的代码的，理由很简单，码农写的代码太多，自己都不知道可能哪里出现了Crash。Linux也是因为git的出现，可以让世界上越来越多的人维护修改bug，现在的linux内核才发展的越来越快。

## git基础
我觉得，要学习git，首先要了解几个概念
![](https://pic3.zhimg.com/80/v2-56bf539b1db69bd0a12cae39e32d2bfa_hd.jpg)
working dir 这个是工作区，我们一般都处在工作区中
index 这个是缓存区，可以临时保存你的改动
HEAD 指向你最新提交的结果（上图没显示）
哈希号 上面那个...... 表示每次提交生成一次哈希号，可以认为是某次提交的身份证号码(上图没显示)
local repository : 本地仓库
remote repository:远程仓库

## 安装初始化git
正常我们都是在Linux下开发和使用git，如果在windows和苹果下开发，请自行百度
安装git
> \#sudo apt-get install git

> \#sudo apt-get install git-doc git-svn git-email git-gui gitk
初始化git
> ssh-keygen \#生成git key 默认在.ssh目录下

> git init \# 在当前目录新建一个Git代码库

> git init [project-name] \# 新建一个目录，将其初始化为Git代码库

> git clone [url]\# 下载一个项目和它的整个代码历史

> git config --list\# 显示当前的Git配置

> git config -e [--global]\# 编辑Git配置文件

> \# 设置提交代码时的用户信息

> git config --global
> [user.email](http://link.zhihu.com/?target=http%3A//user.email/)
> ["you@example.com](mailto:%22you@example.com)
> "

> git config --global
> [user.name](http://link.zhihu.com/?target=http%3A//user.name/)
> "Your Name"

## 代码提交相关命令
> git clone
> [https://github.com/username/repo](http://link.zhihu.com/?target=https%3A//github.com/username/repo)
> name.git \# 从远程仓库下载代码

> git status . \# 查看当前工作区的修改

> git add * \# 跟踪新文件

> git add -u [path] \# 添加[指定路径下]已跟踪文件

> rm *&git rm * \# 移除文件

> git rm -f * \# 移除文件

> git rm --cached * \# 停止追踪指定文件，但该文件会保留在工作区

> git mv file_from file_to \# 重命名跟踪文件

> git log \# 查看提交记录

> git commit \# 提交更新

> git commit [file1] [file2] ... \# 提交指定文件

> git commit -m 'message'

> git commit -a \# 跳过使用暂存区域，把所有已经跟踪过的文件暂存起来一并提交

> git commit --amend\#修改最后一次提交

> git commit -v \# 提交时显示所有diff信息

> git reset HEAD *\#取消已经暂存的文件

> git reset --mixed HEAD *\#同上

> git reset --soft HEAD *\#重置到指定状态，不会修改索引区和工作树

> git reset --hard HEAD *\#重置到指定状态，会修改索引区和工作树

> git reset -- files\#重置index区文件

> git revert HEAD \#撤销前一次操作

> git revert HEAD~ \#撤销前前一次操作

> git revert commit \#\# 撤销指定操作

> git checkout -- file\#取消对文件的修改（从暂存区——覆盖worktree file）

> git checkout branch|tag|commit -- file_name\#从仓库取出file覆盖当前分支

> git checkout -- .\#从暂存区取出文件覆盖工作区

> git diff file \#查看指定文件的差异

> git diff --stat \#查看简单的diff结果

> git diff \#比较Worktree和Index之间的差异

> git diff --cached \#比较Index和HEAD之间的差异

> git diff HEAD \#比较Worktree和HEAD之间的差异

> git diff branch \#比较Worktree和branch之间的差异

> git diff branch1 branch2 \#比较两次分支之间的差异

> git diff commit commit \#比较两次提交之间的差异

> git log --grep=aplog \#查找关键字aplog

> git log \#查看最近的提交日志

> git log --pretty=oneline \#单行显示提交日志

> git log --graph \# 图形化显示

> git log --abbrev-commit \# 显示log id的缩写

> git log -num \#显示第几条log（倒数）

> git log --stat \# 显示commit历史，以及每次commit发生变更的文件

> git log --follow [file] \# 显示某个文件的版本历史，包括文件改名

> git log -p [file] \# 显示指定文件相关的每一次diff

> git log --oneline \#只用一行显示每次的提交包含哈希号和commit内容

> git stash \#将工作区现场（已跟踪文件）储藏起来，等以后恢复后继续工作。

> git stash list \#查看保存的工作现场

> git stash apply \#恢复工作现场

> git stash drop \#删除stash内容

> git stash pop \#恢复的同时直接删除stash内容 最好不要这么干，除非你下次确认不会再使用缓存数据

> git stash apply stash@{0} \#恢复指定的工作现场，当你保存了不只一份工作现场时。

## 分支操作
> git branch \#列出本地分支

> git branch -r\#列出远端分支 为了方便查看，前面会有一个*号显示

> git branch -a\#列出所有分支

> git branch -v\#查看各个分支最后一个提交对象的信息

> git branch --merge\#查看已经合并到当前分支的分支

> git branch --no-merge\#查看为合并到当前分支的分支

> git branch test\#新建test分支

> git branch branch [branch|commit|tag] \# 从指定位置出新建分支

> git branch --track branch remote-branch \# 新建一个分支，与指定的远程分支建立追踪关系

> git branch -m old new \#重命名分支

> git branch -d test \#删除test分支

> git branch -D test \#强制删除test分支 可以多分支同时删除

> git branch --set-upstream dev origin/dev \#将本地dev分支与远程dev分支之间建立链接

> git checkout test\#切换到test分支

> git checkout -b test\#新建+切换到test分支

> git checkout -b test dev\#基于dev新建test分支，并切换

> git merge test\#将test分支合并到当前分支

> git merge --squash test \#\# 合并压缩，将test上的commit压缩为一条

> git cherry-pick commit \#拣选合并，将commit合并到当前分支，比如我们之前在dev分支下做了一个修改，然后切换到master下面来，同步了最新的代码，可以通过cherry-pick把之前的提交同步过来

> git cherry-pick -n commit \#拣选多个提交，合并完后可以继续拣选下一个提交

> git rebase master\#将master分之上超前的提交，变基到当前分支

> git rebase --onto master 169a6 \#限制回滚范围，rebase当前分支从169a6以后的提交

> git rebase --interactive \#交互模式

> git rebase --continue\# 处理完冲突继续合并

> git rebase --skip\# 跳过

> git rebase --abort\# 取消合并
![](https://pic2.zhimg.com/80/v2-00a8db22cfc3c0d72b07965a35e013e5_hd.jpg)
## 远程分支相关命令
> git fetch origin remotebranch[:localbranch]\# 从远端拉去分支[到本地指定分支]

> git merge origin/branch\#合并远端上指定分支

> git pull origin remotebranch:localbranch\# 拉去远端分支到本地分支

> git push origin branch\#将当前分支，推送到远端上指定分支

> git push origin localbranch:remotebranch\#推送本地指定分支，到远端上指定分支

> git push origin :remotebranch \# 删除远端指定分支

> git push origin remotebranch --delete \# 删除远程分支

> git branch -dr branch \# 删除本地和远程分支

> git checkout -b [--track] test origin/dev\#基于远端dev分支，新建本地test分支[同时设置跟踪]

## git远程源
git是一个分布式代码管理工具，所以可以支持多个仓库，在git里，服务器上的仓库在本地称之为remote。
个人开发时，多源用的可能不多，但多源其实非常有用。
正常的时候，我们在一个远程仓库作为本地代码仓库的源，但是有时候我们远程仓库的地址改变了，本地不管提交还是拉取代码都会出错，这时候就需要更新源。
> git remote add origin1 git@github.com:weiqifa/demo.git \#添加源

> git remote\#显示全部源

> git remote -v\#显示全部源+详细信息

> git remote rename origin1 origin2\#重命名

> git remote rm origin\#删除

> git remote show origin\#查看指定源的全部信息

## 想清楚了再使用
> gitpush-f origin master
> \#强制提交，这个命令你领导估计也不知道你干嘛了，当然你爽了，别人就瞎逼了
同步其他分支的某个文件
git show yan3_dev:scripts/resource_tool>scripts/resource_tool
把yan3_dev 分支下的resource_tool 同步到当前分支下

## 打包项目源码命令相关
git clone –bare kernel/ 这样生成kernel.git 这里会包含.git文件夹
git clone kernel.git/ 这样来解包

## 打补丁命令相关
git diff ./ > *.patch
patch -p1 < xxx.patch

## 查看历史分支图
git log –graph –all

## git 忽略文件相关
[Git 忽略一些文件的提交 - 嵌入式Linux - CSDN博客](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/44217867)

## 学会查找资料
总结的命令不一定会都用到，但是学会如何查找资料解决自己的问题非常关键，很多时候我们提交可能提示出错，很多的时候都是分支跟master有冲突，或者从错误日志上可以看出问题，多动动脑子，很快就可以解决了。
> git help commit \#查看commit命令的帮助信息

## 参考
[Git 命令 - 嵌入式Linux - CSDN博客​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/44563585)
[https://blog.csdn.net/weiqifa0/article/details/11655049​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/11655049)
[GIT安装和配置 - 嵌入式Linux - CSDN博客​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/11655049)
[GIT安装和配置 - 嵌入式Linux - CSDN博客​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/11655049)
[https://blog.csdn.net/weiqifa0/article/details/51917879​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/51917879)
[ubuntu下面的git服务器搭建 - 嵌入式Linux - CSDN博客​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/51917879)
[ubuntu下面的git服务器搭建 - 嵌入式Linux - CSDN博客​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/51917879)
[【Github教程】史上最全github使用方法：github入门到精通 - 嵌入式Linux - CSDN博客​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/44250311)
[https://blog.csdn.net/weiqifa0/article/details/10107375​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/10107375)
[GIT非常好的整理 - 嵌入式Linux - CSDN博客​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/10107375)
[GIT非常好的整理 - 嵌入式Linux - CSDN博客​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/10107375)
[在git上push代码时缺少Change-Id - 嵌入式Linux - CSDN博客​blog.csdn.net](http://link.zhihu.com/?target=https%3A//blog.csdn.net/weiqifa0/article/details/83415963)
[https://stackoverflow.com/questions/1337320/how-to-grep-git-commit-diffs-or-contents-for-a-certain-word​stackoverflow.com](http://link.zhihu.com/?target=https%3A//stackoverflow.com/questions/1337320/how-to-grep-git-commit-diffs-or-contents-for-a-certain-word)[How to grep Git commit diffs or contents for a certain word?​stackoverflow.com](http://link.zhihu.com/?target=https%3A//stackoverflow.com/questions/1337320/how-to-grep-git-commit-diffs-or-contents-for-a-certain-word)
[How to grep Git commit diffs or contents for a certain word?​stackoverflow.com](http://link.zhihu.com/?target=https%3A//stackoverflow.com/questions/1337320/how-to-grep-git-commit-diffs-or-contents-for-a-certain-word)
[Git工具详解以及与GitHub的配合使用_Linux教程_Linux公社-Linux系统门户网站​www.linuxidc.com](http://link.zhihu.com/?target=https%3A//www.linuxidc.com/Linux/2017-05/144168.htm)[我的git笔记​yanhaijing.com](http://link.zhihu.com/?target=https%3A//yanhaijing.com/git/2014/11/01/my-git-note/)
![](https://pic2.zhimg.com/80/v2-c9ebe693e321af24ee72cc3d301d76b1_hd.jpg)

