# git的学习笔记整理 - DoubleLi - 博客园






**[Git](http://lib.csdn.net/base/git)学习较好的网址：http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/001373962845513aefd77a99f4145f0a2c7a7ca057e7570000**

**开源中国的git站点：http://git.oschina[.NET](http://lib.csdn.net/base/dotnet)/oschina/git-osc/wikis/Home**



**msysgit是Windows版的Git，从[http://msysgit.github.io/](http://msysgit.github.io/)下载**

**安装选择默认安装就可以了。**

**对于任何一个文件，在 Git 内都只有三种状态：已提交（committed），已修改（modified）和已暂存（staged）。已提交表示该文件已经被安全地保存在本地[数据库](http://lib.csdn.net/base/mysql)中了；已修改表示修改了某个文件，但还没有提交保存；已暂存表示把已修改的文件放在下次提交时要保存的清单中。**



1安装完成后，还需要最后一步设置，在命令行输入：

```
$ git config --global user.name "Your Name"
$ git config --global user.email "email@example.com"
```

注意*git config*命令的--global参数，用了这个参数，表示你这台机器上所有的Git仓库都会使用这个配置，当然也可以对某个仓库指定不同的用户名和Email地址

**2.创建版本库**

**首先，选择一个合适的地方，创建一个空目录：**



```
$ mkdir learngit            //电脑的路径+文件名
$ cd learngit
$ pwd
```

pwd命令用于显示当前目录



**第二步，通过git init命令把这个目录变成Git可以管理的仓库：**



```
$ git init
```







言归正传，现在我们编写一个readme.txt文件，内容如下：

```
Git is a version control system.
Git is free software.
```

一定要放到learngit目录下（子目录也行），因为这是一个Git仓库，放到其他地方Git再厉害也找不到这个文件。

和把大象放到冰箱需要3步相比，把一个文件放到Git仓库只需要两步

第一步，用命令*git add*告诉Git，把文件添加到仓库：



```
$ git add readme.txt
```

第二步，用命令*git commit*告诉Git，把文件提交到仓库：





```
$ git commit -m "wrote a readme file"
[master (root-commit) cb926e7] wrote a readme file
 1 file changed, 2 insertions(+)
 create mode 100644 readme.txt
```





**git commit命令，-m后面输入的是本次提交的说明，可以输入任意内容，当然最好是有意义的，这样你就能从历史记录里方便地找到改动记录。**

***git status*命令可以让我们时刻掌握仓库当前的状态，上面的命令告诉我们，readme.txt被修改过了，但还没有准备提交的修改。**




`$ git diff readme.txt `
*git diff*顾名思义就是查看difference，显示的格式正是Unix通用的diff格式





**在实际工作中，我们脑子里怎么可能记得一个几千行的文件每次都改了什么内容，不然要[版本控制](http://lib.csdn.net/base/git)系统干什么。版本控制系统肯定有某个命令可以告诉我们历史记录，在Git中，我们用git log命令查看：**


`$ git log`
如果嫌输出信息太多，看得眼花缭乱的，可以试试加上 --pretty=oneline参数：





```
$ git log --pretty=oneline
```

```
$ git reset --hard HEAD^      回退到上一个版本
```

```

```

```
$ git reset --hard 3628164回到指定的id，窗口没关闭的情况下
```







```
$ cat readme.txt查看文件的内容
```

```

```

Git提供了一个命令*git reflog*用来记录你的每一次命令：





```
$ git reflog
```
- 
穿梭前，用*git log*可以查看提交历史，以便确定要回退到哪个版本。

- 
要重返未来，用*git reflog*查看命令历史，以便确定要回到未来的哪个版本。


**工作区**（Working Directory）：就是你在电脑里能看到的目录，比如我的learngit文件夹就是一个工作区：





****版本库**（Repository）：工作区有一个隐藏目录“.git”，这个不算工作区，而是Git的版本库。**



前面讲了我们把文件往Git版本库里添加的时候，是分两步执行的：

第一步是用“git add”把文件添加进去，实际上就是把文件修改添加到暂存区；

第二步是用“git commit”提交更改，实际上就是把暂存区的所有内容提交到当前分支。

因为我们创建Git版本库时，Git自动为我们创建了唯一一个master分支，所以，现在，commit就是往master分支上提交更改。



**提交后，用“git diff HEAD -- readme.txt”命令可以查看工作区和版本库里面最新版本的区别：**



命令*git checkout -- readme.txt*意思就是，把readme.txt文件在工作区的修改全部撤销，这里有两种情况：

一种是readme.txt自修改后还没有被放到暂存区，现在，撤销修改就回到和版本库一模一样的状态；

一种是readme.txt已经添加到暂存区后，又作了修改，现在，撤销修改就回到添加到暂存区后的状态。

总之，就是让这个文件回到最近一次git commit或git add时的状态。





场景1：当你改乱了工作区某个文件的内容，想直接丢弃工作区的修改时，用命令*git checkout -- file*。

场景2：当你不但改乱了工作区某个文件的内容，还添加到了暂存区时，想丢弃修改，分两步，第一步用命令*git reset HEAD file*，就回到了场景1，第二步按场景1操作。

场景3：已经提交了不合适的修改到版本库时，想要撤销本次提交，参考[版本回退](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/0013744142037508cf42e51debf49668810645e02887691000)一节，不过前提是没有推送到远程库。





一般情况下，你通常直接在文件管理器中把没用的文件删了，或者用rm命令删了：

```
$ rm test.txt
```



一是确实要从版本库中删除该文件，那就用命令*git rm*删掉，并且commit：



```
$ git rm test.txt
rm 'test.txt'
$ git commit -m "remove test.txt"
[master d17efd8] remove test.txt
 1 file changed, 1 deletion(-)
 delete mode 100644 test.txt
```

另一种情况是删错了，因为版本库里还有呢，所以可以很轻松地把误删的文件恢复到最新版本：



```
$ git checkout -- test.txt
```

命令*git rm*用于删除一个文件。如果一个文件已经被提交到版本库，那么你永远不用担心误删，但是要小心，你只能恢复文件到最新版本，你会丢失**最近一次提交后你修改的内容**。



**第1步：创建SSH Key**

**在用户主目录下，看看有没有.ssh目录，如果有，再看看这个目录下有没有id_rsa和id_rsa.pub这两个文件，如果已经有了，可直接跳到下一步。如果没有，打开Shell（Windows下打开Git Bash），创建SSH Key：**



```
$ ssh-keygen -t rsa -C "youremail@example.com"
```







```
$ git remote add origin git@github.com:michaelliao/learngit.git
```





请千万注意，把上面的michaelliao替换成你自己的GitHub账户名（比如如果是开源中国的那么
`git@github.com:michaelliao/learngit.git要改成http://git.oschina.net/wj903829182/learngit.git（这是我的）`
），否则，你在本地关联的就是我的远程库，关联没有问题，但是你以后推送是推不上去的，因为你的SSH Key公钥不在我的账户列表中。



添加后，远程库的名字就是*origin*，这是Git默认的叫法，也可以改成别的，但是*origin*这个名字一看就知道是远程库。

下一步，就可以把本地库的所有内容推送到远程库上：

```
$ git push -u origin master
Counting objects: 19, done.
Delta compression using up to 4 threads.
Compressing objects: 100% (19/19), done.
Writing objects: 100% (19/19), 13.73 KiB, done.
Total 23 (delta 6), reused 0 (delta 0)
To git@github.com:michaelliao/learngit.git
 * [new branch]      master -> master
Branch master set up to track remote branch master from origin.
```





如果输入$ git remote add origin [git@github.com:djqiang（github帐号名）/gitdemo（项目名）.git](mailto:git@github.com:djqiang/gitdemo.git)

    提示出错信息：fatal: remote origin already exists.



    解决办法如下：

    1、先输入$ git remote rm origin

    2、再输入$ git remote add origin [git@github.com:djqiang/gitdemo.git](mailto:git@github.com:djqiang/gitdemo.git) 就不会报错了！

    3、如果输入$ git remote rm origin 还是报错的话，error: Could not remove config section 'remote.origin'. 我们需要修改gitconfig文件的内容

    4、找到你的github的安装路径，我的是C:\Users\ASUS\AppData\Local\GitHub\PortableGit_ca477551eeb4aea0e4ae9fcd3358bd96720bb5c8\etc

    5、找到一个名为gitconfig的文件，打开它把里面的`[remote "origin"]那一行`删掉就好了！




The authenticity of host 'github.com (192.30.252.131)' can't be established.

**解决：执行ssh  -o StrictHostKeyChecking=no  172.18.19.XXX　就OK**





要关联一个远程库，使用命令*git remote add origin git@server-name:path/repo-name.git*；

(git@server-name:path/repo-name.git这个应该是你申请的git的项目地址比如我的：http://git.oschina[.Net](http://lib.csdn.net/base/dotnet)/wj903829182/learngit.git)

关联后，使用命令*git push -u origin master*第一次推送master分支的所有内容；

此后，每次本地提交后，只要有必要，就可以使用命令*git push origin master*推送最新修改；

分布式版本系统的最大好处之一是在本地工作完全不需要考虑远程库的存在，也就是有没有联网都可以正常工作，而SVN在没有联网的时候是拒绝干活的！当有网络的时候，再把本地提交推送一下就完成了同步，真是太方便了！





#### 从远程库克隆


登陆GitHub，创建一个新的仓库，名字叫gitskills：



**我们勾选Initialize this repository with a README，这样GitHub会自动为我们创建一个README.md文件。创建完毕后，可以看到README.md文件：**



现在，远程库已经准备好了，下一步是用命令*git clone*克隆一个本地库：

```
$ git http://git.oschina.net/wj903829182/gitskills.git
Cloning into 'gitskills'...
remote: Counting objects: 3, done.
remote: Total 3 (delta 0), reused 0 (delta 0)
Receiving objects: 100% (3/3), done.

$ cd gitskills
$ ls
README.md
```





首先，我们创建dev分支，然后切换到dev分支：

```
$ git checkout -b dev
Switched to a new branch 'dev'
```

*git checkout*命令加上-b参数表示创建并切换，相当于以下两条命令：

```
$ git branch dev
$ git checkout dev
Switched to branch 'dev'
```

然后，用*git branch*命令查看当前分支：

```
$ git branch
* dev
  master
```

*git branch*命令会列出所有分支，当前分支前面会标一个*号。

然后，我们就可以在dev分支上正常提交，比如对readme.txt做个修改，加上一行：

```
Creating a new branch is quick.
```

然后提交：

```
$ git add readme.txt 
$ git commit -m "branch test"
[dev fec145a] branch test
 1 file changed, 1 insertion(+)
```

现在，dev分支的工作完成，我们就可以切换回master分支：

```
$ git checkout master
Switched to branch 'master'
```





现在，我们把dev分支的工作成果合并到master分支上：

```
$ git merge dev
Updating d17efd8..fec145a
Fast-forward
 readme.txt |    1 +
 1 file changed, 1 insertion(+)
```

*git merge*命令用于合并指定分支到当前分支。合并后，再查看readme.txt的内容，就可以看到，和dev分支的最新提交是完全一样的。

注意到上面的*Fast-forward*信息，Git告诉我们，这次合并是“快进模式”，也就是直接把master指向dev的当前提交，所以合并速度非常快。

当然，也不是每次合并都能*Fast-forward*，我们后面会将其他方式的合并。

合并完成后，就可以放心地删除dev分支了：

```
$ git branch -d dev
Deleted branch dev (was fec145a).
```

删除后，查看branch，就只剩下master分支了：

```
$ git branch
* master
```

因为创建、合并和删除分支非常快，所以Git鼓励你使用分支完成某个任务，合并后再删掉分支，这和直接在master分支上工作效果是一样的，但过程更安全。

Git鼓励大量使用分支：

查看分支：git branch

创建分支：git branch *name*

切换分支：git checkout *name*

创建+切换分支：git checkout -b *name*

合并某分支到当前分支：git merge *name*

删除分支：git branch -d *name*





软件开发中，bug就像家常便饭一样。有了bug就需要修复，在Git中，由于分支是如此的强大，所以，每个bug都可以通过一个新的临时分支来修复，修复后，合并分支，然后将临时分支删除。

当你接到一个修复一个代号101的bug的任务时，很自然地，你想创建一个分支issue -101来修复它，但是，等等，当前正在dev上进行的工作还没有提交：

```
$ git status
# On branch dev
# Changes to be committed:
#   (use "git reset HEAD <file>..." to unstage)
#
#       new file:   hello.py
#
# Changes not staged for commit:
#   (use "git add <file>..." to update what will be committed)
#   (use "git checkout -- <file>..." to discard changes in working directory)
#
#       modified:   readme.txt
#
```

并不是你不想提交，而是工作只进行到一半，还没法提交，预计完成还需1天时间。但是，必须在两个小时内修复该bug，怎么办？

幸好，Git还提供了一个stash功能，可以把当前工作现场“储藏”起来，等以后恢复现场后继续工作：

```
$ git stash
Saved working directory and index state WIP on dev: 6224937 add merge
HEAD is now at 6224937 add merge
```

现在，用*git status*查看工作区，就是干净的（除非有没有被Git管理的文件），因此可以放心地创建分支来修复bug。

首先确定要在哪个分支上修复bug，假定需要在master分支上修复，就从master创建临时分支：

```
$ git checkout master
Switched to branch 'master'
Your branch is ahead of 'origin/master' by 6 commits.
$ git checkout -b issue-101
Switched to a new branch 'issue-101'
```

现在修复bug，需要把“Git is free software ...”改为“Git is a free software ...”，然后提交：

```
$ git add readme.txt 
$ git commit -m "fix bug 101"
[issue-101 cc17032] fix bug 101
 1 file changed, 1 insertion(+), 1 deletion(-)
```

修复完成后，切换到master分支，并完成合并，最后删除issue-101分支：

```
$ git checkout master
Switched to branch 'master'
Your branch is ahead of 'origin/master' by 2 commits.
$ git merge --no-ff -m "merged bug fix 101" issue-101
Merge made by the 'recursive' strategy.
 readme.txt |    2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)
$ git branch -d issue-101
Deleted branch issue-101 (was cc17032).
```

太棒了，原计划两个小时的bug修复只花了5分钟！现在，是时候接着回到dev分支干活了！

```
$ git checkout dev
Switched to branch 'dev'
$ git status
# On branch dev
nothing to commit (working directory clean)
```

工作区是干净的，刚才的工作现场存到哪去了？用*git stash list*命令看看：

```
$ git stash list
stash@{0}: WIP on dev: 6224937 add merge
```

工作现场还在，Git把stash内容存在某个地方了，但是需要恢复一下，有两个办法：

一是用*git stash apply*恢复，但是恢复后，stash内容并不删除，你需要用*git stash drop*来删除；

另一种方式是用*git stash pop*，恢复的同时把stash内容也删了：

```
$ git stash pop
# On branch dev
# Changes to be committed:
#   (use "git reset HEAD <file>..." to unstage)
#
#       new file:   hello.py
#
# Changes not staged for commit:
#   (use "git add <file>..." to update what will be committed)
#   (use "git checkout -- <file>..." to discard changes in working directory)
#
#       modified:   readme.txt
#
Dropped refs/stash@{0} (f624f8e5f082f2df2bed8a4e09c12fd2943bdd40)
```

再用*git stash list*查看，就看不到任何stash内容了：

```
$ git stash list
```

你可以多次stash，恢复的时候，先用*git stash list*查看，然后恢复指定的stash，用命令：

```
$ git stash apply stash@{0}
```







### 小结

修复bug时，我们会通过创建新的bug分支进行修复，然后合并，最后删除；

当手头工作没有完成时，先把工作现场*git stash*一下，然后去修复bug，修复后，再*git stash pop*，回到工作现场。








当你从远程仓库克隆时，实际上Git自动把本地的master分支和远程的master分支对应起来了，并且，远程仓库的默认名称是origin。

要查看远程库的信息，用*git remote*：

```
$ git remote
origin
```

或者，用*git remote -v*显示更详细的信息：

```
$ git remote -v
origin  git@github.com:michaelliao/learngit.git (fetch)
origin  git@github.com:michaelliao/learngit.git (push)
```

上面显示了可以抓取和推送的origin的地址。如果没有推送权限，就看不到push的地址。

### 推送分支

推送分支，就是把该分支上的所有本地提交推送到远程库。推送时，要指定本地分支，这样，Git就会把该分支推送到远程库对应的远程分支上：

```
$ git push origin master
```

如果要推送其他分支，比如dev，就改成：

```
$ git push origin dev
```

但是，并不是一定要把本地分支往远程推送，那么，哪些分支需要推送，哪些不需要呢？
- 
master分支是主分支，因此要时刻与远程同步；

- 
dev分支是开发分支，团队所有成员都需要在上面工作，所以也需要与远程同步；

- 
bug分支只用于在本地修复bug，就没必要推到远程了，除非老板要看看你每周到底修复了几个bug；

- 
feature分支是否推到远程，取决于你是否和你的小伙伴合作在上面开发。


总之，就是在Git中，分支完全可以在本地自己藏着玩，是否推送，视你的心情而定！







### 抓取分支

多人协作时，大家都会往master和dev分支上推送各自的修改。

现在，模拟一个你的小伙伴，可以在另一台电脑（注意要把SSH Key添加到GitHub）或者同一台电脑的另一个目录下克隆：

```
$ git clone git@github.com:michaelliao/learngit.git
Cloning into 'learngit'...
remote: Counting objects: 46, done.
remote: Compressing objects: 100% (26/26), done.
remote: Total 46 (delta 16), reused 45 (delta 15)
Receiving objects: 100% (46/46), 15.69 KiB | 6 KiB/s, done.
Resolving deltas: 100% (16/16), done.
```

当你的小伙伴从远程库clone时，默认情况下，你的小伙伴只能看到本地的master分支。不信可以用*git branch*命令看看：

```
$ git branch
* master
```

现在，你的小伙伴要在dev分支上开发，就必须创建远程origin的dev分支到本地，于是他用这个命令创建本地dev分支：

```
$ git checkout -b dev origin/dev
```

现在，他就可以在dev上继续修改，然后，时不时地把dev分支push到远程：

```
$ git commit -m "add /usr/bin/env"
[dev 291bea8] add /usr/bin/env
 1 file changed, 1 insertion(+)
$ git push origin dev
Counting objects: 5, done.
Delta compression using up to 4 threads.
Compressing objects: 100% (2/2), done.
Writing objects: 100% (3/3), 349 bytes, done.
Total 3 (delta 0), reused 0 (delta 0)
To git@github.com:michaelliao/learngit.git
   fc38031..291bea8  dev -> dev
```







你的小伙伴已经向origin/dev分支推送了他的提交，而碰巧你也对同样的文件作了修改，并试图推送：

```
$ git add hello.py 
$ git commit -m "add coding: utf-8"
[dev bd6ae48] add coding: utf-8
 1 file changed, 1 insertion(+)
$ git push origin dev
To git@github.com:michaelliao/learngit.git
 ! [rejected]        dev -> dev (non-fast-forward)
error: failed to push some refs to 'git@github.com:michaelliao/learngit.git'
hint: Updates were rejected because the tip of your current branch is behind
hint: its remote counterpart. Merge the remote changes (e.g. 'git pull')
hint: before pushing again.
hint: See the 'Note about fast-forwards' in 'git push --help' for details.
```

推送失败，因为你的小伙伴的最新提交和你试图推送的提交有冲突，解决办法也很简单，Git已经提示我们，先用*git pull*把最新的提交从origin/dev抓下来，然后，在本地合并，解决冲突，再推送：

```
$ git pull
remote: Counting objects: 5, done.
remote: Compressing objects: 100% (2/2), done.
remote: Total 3 (delta 0), reused 3 (delta 0)
Unpacking objects: 100% (3/3), done.
From github.com:michaelliao/learngit
   fc38031..291bea8  dev        -> origin/dev
There is no tracking information for the current branch.
Please specify which branch you want to merge with.
See git-pull(1) for details

    git pull <remote> <branch>

If you wish to set tracking information for this branch you can do so with:

    git branch --set-upstream dev origin/<branch>
```

*git pull*也失败了，原因是没有指定本地dev分支与远程origin/dev分支的链接，根据提示，设置dev和origin/dev的链接：

```
$ git branch --set-upstream dev origin/dev
Branch dev set up to track remote branch dev from origin.
```

再pull：

```
$ git pull
Auto-merging hello.py
CONFLICT (content): Merge conflict in hello.py
Automatic merge failed; fix conflicts and then commit the result.
```

这回*git pull*成功，但是合并有冲突，需要手动解决，解决的方法和分支管理中的[解决冲突](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/001375840202368c74be33fbd884e71b570f2cc3c0d1dcf000)完全一样。解决后，提交，再push：

```
$ git commit -m "merge & fix hello.py"
[dev adca45d] merge & fix hello.py
$ git push origin dev
Counting objects: 10, done.
Delta compression using up to 4 threads.
Compressing objects: 100% (5/5), done.
Writing objects: 100% (6/6), 747 bytes, done.
Total 6 (delta 0), reused 0 (delta 0)
To git@github.com:michaelliao/learngit.git
   291bea8..adca45d  dev -> dev
```







因此，多人协作的工作模式通常是这样：
- 
首先，可以试图用*git push origin branch-name*推送自己的修改；

- 
如果推送失败，则因为远程分支比你的本地更新，需要先用*git pull*试图合并；

- 
如果合并有冲突，则解决冲突，并在本地提交；

- 
没有冲突或者解决掉冲突后，再用*git push origin branch-name*推送就能成功！


如果*git pull*提示“no tracking information”，则说明本地分支和远程分支的链接关系没有创建，用命令*git branch --set-upstream branch-name origin/branch-name*。

这就是多人协作的工作模式，一旦熟悉了，就非常简单。

### 小结
- 
查看远程库信息，使用*git remote -v*；

- 
本地新建的分支如果不推送到远程，对其他人就是不可见的；

- 
从本地推送分支，使用*git push origin branch-name*，如果推送失败，先用*git pull*抓取远程的新提交；

- 
在本地创建和远程分支对应的分支，使用*git checkout -b branch-name origin/branch-name*，本地和远程分支的名称最好一致；

- 
建立本地分支和远程分支的关联，使用*git branch --set-upstream branch-name origin/branch-name*；

- 
从远程抓取分支，使用*git pull*，如果有冲突，要先处理冲突。

- 


























**git主要的重点学习笔记**

**$ pwdpwd命令用于显示当前目录第一步，用命令git add告诉Git，把文件添加到仓库：$ git add readme.txt第二步，用命令git commit告诉Git，把文件提交到仓库：$ git commit -m "wrote a readme file"$ git statusgit status命令可以让我们时刻掌握仓库当前的状态$ git diff readme.txt 能看看具体修改了什么内容，自然是很好的。比如你休假两周从国外回来，第一天上班时，已经记不清上次怎么修改的readme.txt，所以，需要用git diff这个命令看看git diff顾名思义就是查看difference，显示的格式正是Unix通用的diff格式，可以从上面的命令输出看到，我们在第一行添加了一个“distributed”单词。$ git log版本控制系统肯定有某个命令可以告诉我们历史记录，在Git中，我们用git log命令查看：git log命令显示从最近到最远的提交日志如果嫌输出信息太多，看得眼花缭乱的，可以试试加上?--pretty=oneline参数：$ git log --pretty=oneline在Git中，用HEAD表示当前版本，也就是最新的提交“ 3628164...882e1e0”（注意我的提交ID和你的肯定不一样），上一个版本就是HEAD^，上上一个版本就是HEAD^^，当然往上100个版本写100个^比较容易数不过来，所以写成HEAD~100。现在，我们要把当前版本“append GPL”回退到上一个版本“add distributed”，就可以使用git reset命令：$ git reset --hard HEAD^$ git reset --hard 3628164$ cat readme.txt    看看readme.txt的内容Git提供了一个命令git reflog用来记录你的每一次命令当你用$ git reset --hard HEAD^回退到“add distributed”版本时，再想恢复到“append GPL”，就必须找到“append GPL”的commit id穿梭前，用git log可以查看提交历史，以便确定要回退到哪个版本。要重返未来，用git reflog查看命令历史，以便确定要回到未来的哪个版本。提交后，用“git diff HEAD -- readme.txt”命令可以查看工作区和版本库里面最新版本的区别：$ git diff HEAD -- readme.txt命令git checkout -- readme.txt意思就是，把readme.txt文件在工作区的修改全部撤销，这里有两种情况：一种是readme.txt自修改后还没有被放到暂存区，现在，撤销修改就回到和版本库一模一样的状态；一种是readme.txt已经添加到暂存区后，又作了修改，现在，撤销修改就回到添加到暂存区后的状态。总之，就是让这个文件回到最近一次git commit或git add时的状态。git reset HEAD file可以把暂存区的修改撤销掉（unstage），重新放回工作区：$ git reset HEAD readme.txt还记得如何丢弃工作区的修改吗？$ git checkout -- readme.txt你通常直接在文件管理器中把没用的文件删了，或者用rm命令删了：$ rm test.txt这个时候，Git知道你删除了文件，因此，工作区和版本库就不一致了，git status命令会立刻告诉你哪些文件被删除了：现在你有两个选择，一是确实要从版本库中删除该文件，那就用命令git rm删掉，并且commit：$ git rm test.txt$ git commit -m "remove test.txt"另一种情况是删错了，因为版本库里还有呢，所以可以很轻松地把误删的文件恢复到最新版本：$ git checkout -- test.txtgit checkout其实是用版本库里的版本替换工作区的版本，无论工作区是修改还是删除，都可以“一键还原”。$ git remote add origin git@github.com:michaelliao/learngit.git请千万注意，把上面的michaelliao替换成你自己的GitHub账户名，否则，你在本地关联的就是我的远程库，关联没有问题，但是你以后推送是推不上去的，因为你的SSH Key公钥不在我的账户列表中。添加后，远程库的名字就是origin，这是Git默认的叫法，也可以改成别的，但是origin这个名字一看就知道是远程库。下一步，就可以把本地库的所有内容推送到远程库上：$ git push -u origin master把本地库的内容推送到远程，用git push命令，实际上是把当前分支master推送到远程。从现在起，只要本地作了提交，就可以通过命令：$ git push origin master把本地master分支的最新修改推送至GitHub，现在，你就拥有了真正的分布式版本库！要关联一个远程库，使用命令git remote add origin git@server-name:path/repo-name.git；关联后，使用命令git push -u origin master第一次推送master分支的所有内容；此后，每次本地提交后，只要有必要，就可以使用命令git push origin master推送最新修改；用命令git clone克隆一个本地库：$ git clone git@github.com:michaelliao/gitskills.git首先，我们创建dev分支，然后切换到dev分支：$ git checkout -b devgit checkout命令加上-b参数表示创建并切换，相当于以下两条命令：$ git branch dev$ git checkout dev然后，用git branch命令查看当前分支：$ git branchgit branch命令会列出所有分支，当前分支前面会标一个*号。dev分支的工作完成，我们就可以切换回master分支：$ git checkout master现在，我们把dev分支的工作成果合并到master分支上：$ git merge devgit merge命令用于合并指定分支到当前分支合并完成后，就可以放心地删除dev分支了：$ git branch -d dev查看分支：git branch创建分支：git branch name切换分支：git checkout name创建+切换分支：git checkout -b name合并某分支到当前分支：git merge name删除分支：git branch -d name带参数的git log也可以看到分支的合并情况$ git log --graph --pretty=oneline --abbrev-commitGit会用“Fast forward”模式，但这种模式下，删除分支后，会丢掉分支信息。如果要强制禁用“Fast forward”模式，Git就会在merge时生成一个新的commit，这样，从分支历史上就可以看出分支信息。准备合并dev分支，请注意--no-ff参数，表示禁用“Fast forward”：$ git merge --no-ff -m "merge with no-ff" dev在实际开发中，我们应该按照几个基本原则进行分支管理：首先，master分支应该是非常稳定的，也就是仅用来发布新版本，平时不能在上面干活；那在哪干活呢？干活都在dev分支上，也就是说，dev分支是不稳定的，到某个时候，比如1.0版本发布时，再把dev分支合并到master上，在master分支发布1.0版本；你和你的小伙伴们每个人都在dev分支上干活，每个人都有自己的分支，时不时地往dev分支上合并就可以了。所以，团队合作的分支看起来就像这样：Git分支十分强大，在团队开发中应该充分应用。合并分支时，加上--no-ff参数就可以用普通模式合并，合并后的历史有分支，能看出来曾经做过合并，而fast forward合并就看不出来曾经做过合并。Git还提供了一个stash功能，可以把当前工作现场“储藏”起来，等以后恢复现场后继续工作：$ git stash刚才的工作现场存到哪去了？用git stash list命令看看：$ git stash list工作现场还在，Git把stash内容存在某个地方了，但是需要恢复一下，有两个办法：一是用git stash apply恢复，但是恢复后，stash内容并不删除，你需要用git stash drop来删除；另一种方式是用git stash pop，恢复的同时把stash内容也删了：$ git stash pop你可以多次stash，恢复的时候，先用git stash list查看，然后恢复指定的stash，用命令：$ git stash apply stash@{0}修复bug时，我们会通过创建新的bug分支进行修复，然后合并，最后删除；当手头工作没有完成时，先把工作现场git stash一下，然后去修复bug，修复后，再git stash pop，回到工作现场。个分支还是必须就地销毁：$ git branch -d feature-vulcan销毁失败。Git友情提醒，feature-vulcan分支还没有被合并，如果删除，将丢失掉修改，如果要强行删除，需要使用命令git branch -D feature-vulcan。现在我们强行删除：$ git branch -D feature-vulcan开发一个新feature，最好新建一个分支；如果要丢弃一个没有被合并过的分支，可以通过git branch -D name强行删除。要查看远程库的信息，用git remote：$ git remote或者，用git remote -v显示更详细的信息：$ git remote -v推送分支，就是把该分支上的所有本地提交推送到远程库。推送时，要指定本地分支，这样，Git就会把该分支推送到远程库对应的远程分支上：$ git push origin master如果要推送其他分支，比如dev，就改成：$ git push origin dev但是，并不是一定要把本地分支往远程推送，那么，哪些分支需要推送，哪些不需要呢？master分支是主分支，因此要时刻与远程同步；dev分支是开发分支，团队所有成员都需要在上面工作，所以也需要与远程同步；bug分支只用于在本地修复bug，就没必要推到远程了，除非老板要看看你每周到底修复了几个bug；feature分支是否推到远程，取决于你是否和你的小伙伴合作在上面开发。总之，就是在Git中，分支完全可以在本地自己藏着玩，是否推送，视你的心情而定！克隆：$ git clone git@github.com:michaelliao/learngit.git你的小伙伴要在dev分支上开发，就必须创建远程origin的dev分支到本地，于是他用这个命令创建本地dev分支：$ git checkout -b dev origin/dev你的小伙伴已经向origin/dev分支推送了他的提交，而碰巧你也对同样的文件作了修改，并试图推送：推送失败，因为你的小伙伴的最新提交和你试图推送的提交有冲突，解决办法也很简单，Git已经提示我们，先用git pull把最新的提交从origin/dev抓下来，然后，在本地合并，解决冲突，再推送：$ git pullgit pull也失败了，原因是没有指定本地dev分支与远程origin/dev分支的链接，根据提示，设置dev和origin/dev的链接：$ git branch --set-upstream dev origin/dev再pull：$ git pull这回git pull成功，但是合并有冲突，需要手动解决，解决的方法和分支管理中的解决冲突完全一样。解决后，提交，再push：因此，多人协作的工作模式通常是这样：首先，可以试图用git push origin branch-name推送自己的修改；如果推送失败，则因为远程分支比你的本地更新，需要先用git pull试图合并；如果合并有冲突，则解决冲突，并在本地提交；没有冲突或者解决掉冲突后，再用git push origin branch-name推送就能成功！如果git pull提示“no tracking information”，则说明本地分支和远程分支的链接关系没有创建，用命令git branch --set-upstream branch-name origin/branch-name。这就是多人协作的工作模式，一旦熟悉了，就非常简单。查看远程库信息，使用git remote -v；本地新建的分支如果不推送到远程，对其他人就是不可见的；从本地推送分支，使用git push origin branch-name，如果推送失败，先用git pull抓取远程的新提交；在本地创建和远程分支对应的分支，使用git checkout -b branch-name origin/branch-name，本地和远程分支的名称最好一致；建立本地分支和远程分支的关联，使用git branch --set-upstream branch-name origin/branch-name；从远程抓取分支，使用git pull，如果有冲突，要先处理冲突。在Git中打标签非常简单，首先，切换到需要打标签的分支上：$ git branch$ git checkout master然后，敲命令git tag name就可以打一个新标签：$ git tag v1.0可以用命令git tag查看所有标签：$ git tag默认标签是打在最新提交的commit上的。有时候，如果忘了打标签，比如，现在已经是周五了，但应该在周一打的标签没有打，怎么办？方法是找到历史提交的commit id，然后打上就可以了：比方说要对“add merge”这次提交打标签，它对应的commit id是“6224937”，敲入命令：$ git tag v0.9 6224937注意，标签不是按时间顺序列出，而是按字母排序的。可以用git show tagname查看标签信息：还可以创建带有说明的标签，用-a指定标签名，-m指定说明文字：$ git tag -a v0.1 -m "version 0.1 released" 3628164命令git tag name用于新建一个标签，默认为HEAD，也可以指定一个commit id；-a tagname -m "blablabla..."可以指定标签信息；-s tagname -m "blablabla..."可以用PGP签名标签；命令git tag可以查看所有标签如果标签打错了，也可以删除：$ git tag -d v0.1如果要推送某个标签到远程，使用命令git push origin tagname：$ git push origin v1.0或者，一次性推送全部尚未推送到远程的本地标签：$ git push origin --tags如果标签已经推送到远程，要删除远程标签就麻烦一点，先从本地删除：$ git tag -d v0.9然后，从远程删除。删除命令也是push，但是格式如下$ git push origin :refs/tags/v0.9让Git显示颜色，会让命令输出看起来更醒目：$ git config --global color.ui true原文来自：http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/001373962845513aefd77a99f4145f0a2c7a7ca057e7570000本文为原创整理的一些git学习笔记原网址为：http://blog.csdn.net/j903829182/article/details/34436641希望能帮助你**









