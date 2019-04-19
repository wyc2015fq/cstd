# git简明教程 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月11日 10:52:00[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：126
创建新仓库
git init
检出仓库
创建一个本地仓库的克隆版本：
git clone /path/to/repository
如果是远端服务器上的仓库：
git clone username@host:/path/to/repository

工作流
你的本地仓库由 git 维护的三棵“树”组成。
第一个是你的 工作目录，它持有实际文件；
第二个是 暂存区（Index），它像个缓存区域，临时保存你的改动；
最后是 HEAD，它指向你最后一次提交的结果。
添加和提交
添加到暂存区，使用如下命令：
git add <filename>
git add *
这是 git 基本工作流程的第一步；使用如下命令以实际提交：
git commit -m "代码提交信息"
现在，你的改动已经提交到了 HEAD，但是还没到你的远端仓库。

推送改动
你的改动现在已经在本地仓库的 HEAD 中了。执行如下命令以将这些改动提交到远端仓库：
git push origin master
上面命令表示，将本地的master分支推送到origin主机的master分支。如果后者不存在，则会被新建。
可以把 master 换成你想要推送的任何分支。
如果你还没有克隆现有仓库，并欲将你的仓库连接到某个远程服务器，你可以使用如下命令添加：
git remote add origin <server>
分支
创建仓库的时候，master 是“默认的”分支。在其他分支上进行开发，完成后再将它们合并到主分支上。
创建一个叫做“feature_x”的分支，并切换过去：
git checkout -b feature_x
切换回主分支：
git checkout master
再把新建的分支删掉：
git branch -d feature_x
除非你将分支推送到远端仓库，不然该分支就是 不为他人所见的：
git push origin <branch>

更新与合并
更新你的本地仓库至最新，执行：
git pull
以在你的工作目录中 获取（fetch） 并 合并（merge） 远端的改动。
要合并其他分支到你的当前分支（例如 master），执行：
git merge <branch>
在这两种情况下，git 都会尝试去自动合并。可能出现冲突（conflicts）。
这时候就需要你修改这些文件来手动合并这些冲突（conflicts）。
改完之后，你需要执行如下命令以将它们标记为合并成功：
git add <filename>
在合并改动之前，你可以使用如下命令预览差异：
git diff <source_branch> <target_branch>
标签
可以执行如下命令创建一个叫做 1.0.0 的标签：
git tag 1.0.0 1b2e1d63ff
1b2e1d63ff 是你想要标记的提交 ID 的前 10 位字符。可以使用下列命令获取提交 ID：
git log
如果你想了解本地仓库的历史记录，最简单的命令就是使用:
git log
你可以添加一些参数来修改他的输出，从而得到自己想要的结果。
只看某一个人的提交记录:
git log --author=bob
每一条提交记录只占一行的输出:
git log --pretty=oneline
或者你想通过树形结构来展示所有的分支, 每个分支都标示了他的名字和标签:
git log --graph --oneline --decorate --all
看看哪些文件改变了: 
git log --name-status

替换本地改动
假如你操作失误，你可以使用如下命令替换掉本地改动：
git checkout -- <filename>
此命令会使用 HEAD 中的最新内容替换掉你的工作目录中的文件。已添加到暂存区的改动以及新文件都不会受到影响。
假如你想丢弃你在本地的所有改动与提交，可以到服务器上获取最新的版本历史，并将你本地主分支指向它：
git fetch origin
git reset --hard origin/master
参考：
[http://rogerdudler.github.io/git-guide/index.zh.html](http://rogerdudler.github.io/git-guide/index.zh.html)
[https://github.com/rogerdudler/git-guide](https://github.com/rogerdudler/git-guide)
- [Git 社区参考书](http://book.git-scm.com/)
- [专业 Git](http://progit.org/book/)
- [像 git 那样思考](http://think-like-a-git.net/)
- [GitHub 帮助](http://help.github.com/)
- [图解 Git](http://marklodato.github.io/visual-git-guide/index-zh-cn.html)
