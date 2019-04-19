# git系列1 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月11日 10:14:09[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：141

git clone支持多种协议，除了HTTP(s)以外，还支持SSH、Git、本地文件协议等，下面是一些例子。
$ git clone http[s]://example.com/path/to/repo.git/$
 git clone ssh://example.com/path/to/repo.git/$
 git clone git://example.com/path/to/repo.git/$
 git clone /opt/git/project.git
 $ git clone file:///opt/git/project.git$
 git clone ftp[s]://example.com/path/to/repo.git/$
 git clone rsync://example.com/path/to/repo.git/
SSH协议还有另一种写法。
$ git clone
[user@]example.com:path/to/repo.git/
git remote
使用-v选项，可以参看远程主机的网址
克隆版本库的时候，所使用的远程主机自动被Git命名为origin。
git remote add命令用于添加远程主机。$
 git remote add <主机名><网址>
git remote rm命令用于删除远程主机。
$ git remote rm
<主机名>
一旦远程主机的版本库有了更新（Git术语叫做commit），需要将这些更新取回本地，这时就要用到git fetch命令。
$ git fetch
<远程主机名>
上面命令将某个远程主机的更新，全部取回本地。
git fetch命令通常用来查看其他人的进程，因为它取回的代码对你本地的开发代码没有影响。
默认情况下，git
 fetch取回所有分支（branch）的更新。如果只想取回特定分支的更新，可以指定分支名。
$ git fetch
<远程主机名><分支名>
比如，取回origin主机的master分支。
$ git fetch origin master
git branch命令的-r选项，可以用来查看远程分支，-a选项查看所有分支。
$ git branch
-rorigin/master$
 git branch -a*
 master remotes/origin/master
上面命令表示，本地主机的当前分支是master，远程分支是origin/master。
取回远程主机的更新以后，可以在它的基础上，使用git
 checkout命令创建一个新的分支。
$ git checkout
-b newBrach origin/master
上面命令表示，在origin/master的基础上，创建一个新分支。
此外，也可以使用git
 merge命令或者git rebase命令，在本地分支上合并远程分支。
$ git merge origin/master#
 或者$ git rebase origin/master
上面命令表示在当前分支上，合并origin/master。
git pull命令的作用是，取回远程主机某个分支的更新，再与本地的指定分支合并。它的完整格式稍稍有点复杂。
$ git pull
<远程主机名><远程分支名>:<本地分支名>
比如，取回origin主机的next分支，与本地的master分支合并，需要写成下面这样。
$ git pull origin next:master
如果远程分支是与当前分支合并，则冒号后面的部分可以省略。
$ git pull origin next
上面命令表示，取回origin/next分支，再与当前分支合并。实质上，这等同于先做git
 fetch，再做git merge。
$ git fetch origin$ git merge origin/next
在某些场合，Git会自动在本地分支与远程分支之间，建立一种追踪关系（tracking）。比如，在git clone的时候，所有本地分支默认与远程主机的同名分支，建立追踪关系，也就是说，本地的master分支自动"追踪"origin/master分支。
Git也允许手动建立追踪关系。
git branch
--set-upstream
 master origin/next
上面命令指定master分支追踪origin/next分支。
如果当前分支与远程分支存在追踪关系，git
 pull就可以省略远程分支名。
$ git pull origin
上面命令表示，本地的当前分支自动与对应的origin主机"追踪分支"（remote-tracking
 branch）进行合并。
如果当前分支只有一个追踪分支，连远程主机名都可以省略。
$ git pull
上面命令表示，当前分支自动与唯一一个追踪分支进行合并。
git push命令用于将本地分支的更新，推送到远程主机。它的格式与git
 pull命令相仿。
$ git push
<远程主机名><本地分支名>:<远程分支名>
注意，分支推送顺序的写法是<来源地>:<目的地>，所以git
 pull是<远程分支>:<本地分支>，而git push是<本地分支>:<远程分支>。
如果省略远程分支名，则表示将本地分支推送与之存在"追踪关系"的远程分支（通常两者同名），如果该远程分支不存在，则会被新建。
$ git push origin master
上面命令表示，将本地的master分支推送到origin主机的master分支。如果后者不存在，则会被新建。
如果当前分支与远程分支之间存在追踪关系，则本地分支和远程分支都可以省略。
$ git push origin
上面命令表示，将当前分支推送到origin主机的对应分支。
如果当前分支只有一个追踪分支，那么主机名都可以省略。
$ git push
**-u**
**--set-upstream**
For every branch that is up to date or successfully pushed, add upstream (tracking) reference, used by argument-less[git-pull](git-pull.html)(1)
 and other commands. For more information, see branch.<name>.merge in [git-config](git-config.html)(1).
如果当前分支与多个主机存在追踪关系，则可以使用-u选项指定一个默认主机，这样后面就可以不加任何参数使用git
 push。
$ git push
-u origin master
上面命令将本地的master分支推送到origin主机，同时指定origin为默认主机，后面就可以不加任何参数使用git
 push了。
不带任何参数的git
 push，默认只推送当前分支，

还有一种情况，就是不管是否存在对应的远程分支，将本地的所有分支都推送到远程主机，这时需要使用--all选项。
$ git push
--all origin
上面命令表示，将所有本地分支都推送到origin主机。
git push不会推送标签（tag），除非使用--tags选项。
$ git push origin
--tags
参考：
[http://www.ruanyifeng.com/blog/2014/06/git_remote.html](http://www.ruanyifeng.com/blog/2014/06/git_remote.html)
