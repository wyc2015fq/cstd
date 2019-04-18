# git 学习笔记 二 (命令行大全) - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月18日 23:37:36[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：19


**Table of Contents**

[feature分支](#feature%E5%88%86%E6%94%AF)

[多人协作](#%E5%A4%9A%E4%BA%BA%E5%8D%8F%E4%BD%9C%C2%A0)

[Rebase](#Rebase)

[标签](#%E6%A0%87%E7%AD%BE)

[码云](#%E7%A0%81%E4%BA%91)

[忽略特殊文件](#%E5%BF%BD%E7%95%A5%E7%89%B9%E6%AE%8A%E6%96%87%E4%BB%B6)

[配置别名](#%E9%85%8D%E7%BD%AE%E5%88%AB%E5%90%8D)

### **feature分支**

接收到一个新任务就创建一个新的feature分支用来完成任务
`$ git checkout feature-任务名`
我们在feature分支上完成了工作，并提交了，但在合并到master上之前，上级说该功能不要了，为了安全起见，必须将feature分支删除

```
$ git branch -d feature-vulcan
error: The branch 'feature-vulcan' is not fully merged.
If you are sure you want to delete it, run 'git branch -D feature-vulcan'.
```

提示我们还没有合并，使用  $ git branch -D feature-vulcan  来强制删除分支

### **多人协作 **

查看远程库信息

```
$ git remote
origin
```

查看远程库详细信息

```
$ git remote -v
origin  git@github.com:zzh546934282/learngit.git (fetch)
origin  git@github.com:zzh546934282/learngit.git (push)
```

向远程库推送信息
`$ git push origin master`
推送其他分支

```
$ git push origin feature-vulcan
```

那么在实际开发中需要向远程库推送哪些内容呢，根据下图我们来分析 

![](https://img-blog.csdn.net/2018091809375846?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- master  主分支，代码发布版，必须时刻与远程库保持同步。
- dev   程序员都是在dev分支上创建各自的分支来工作的，也就是说干完工作都需要提交到dev上，这样同事可以看到，所以            dev也必须时刻与远程库保持同步
-  michael 和 bob   程序员自己的开发分支，如需合作开发则需同步到远程库，如无需则不需要同步。
-  bug   程序员自己创建的用来改bug的分支，无需上传到远程库

场景描述：

同事A 在电脑A上，将master分支和dev分支push到了远程库中，同事B在电脑B上，通过clone命令从github库pull代码到本地，但是同事B只能看到master这个分支，看不到dev分支，他需要执行$ git checkout -b dev origin/dev 代码从而获得github上的dev分支。然后他就可以切换到这个dev分支，然后在dev分支上面创建一个feature分支供自己开发功能，开发完毕以后他回到dev分支上将feature分支合并，然后再将dev分支同步到github上。同事B上传到github以后，此时同事A也要将自己在dev分支上的改动上传到github，就会导致冲突。下面我用代码来演示上述场景。

```
首先查看git连接的远程库是不是我们想要传代码的库
$ git remote -v
origin  git@github.com:zzh546934282/learngit.git (fetch)
origin  git@github.com:zzh546934282/learngit.git (push)
```

```
确认github上的repository无误后 A员工在电脑A上，分别master 和 dev 分支上传到 github
$ git push origin master
$ git push origin dev
```

```
员工B 在电脑B上 先将git与github建立连接，由于我是在同一台电脑上模拟的，所以无需再建立连接。
建立连接过程 ： 1、生成密钥  
               2、把公钥复制到github上  
               3、$ git remote add origin git@github.com:aaa/bbb.git 

下面从github获取文件开始

$ git clone git@github.com:zzh546934282/learngit.git 克隆用于第一次从远程库下载文件

$ cd learngit/    进入下载的文件夹中即reposity中

$ git branch   查看分支，可以发现员工B，只能查看到master分支，并没有 dev 分支
* master

$ git checkout -b dev origin/dev   员工B通过该命令创建一份远程库中dev分支的拷贝
Switched to a new branch 'dev'
Branch 'dev' set up to track remote branch 'dev' from 'origin'.

$ git branch
* dev         * 代表当前所在分支
  master

$ git add readme.txt     修改内容后进行提交，并将dev分支的代码同步到github
$ git commit -m 'commit'
$ git push origin dev
```

 此时 员工B已经将dev分支内容修改并同步到了github上，下面我们在模拟员工A在电脑A上面的操作。

```
$ git add readme.txt       员工A在本地对文件做了修改

$ git commit -m 'commit'   将工作空间的文件提交到分支dev上面
[dev 7a0ddf9] commit 
 1 file changed, 1 insertion(+), 2 deletions(-)

$ git push origin dev    将dev分支同步到github上出现错误，告诉我们内容冲突了，可以使用git pull解决
To github.com:zzh546934282/learngit.git
 ! [rejected]        dev -> dev (fetch first)
error: failed to push some refs to 'git@github.com:zzh546934282/learngit.git'
hint: Updates were rejected because the remote contains work that you do
hint: not have locally. This is usually caused by another repository pushing
hint: to the same ref. You may want to first integrate the remote changes
hint: (e.g., 'git pull ...') before pushing again.
hint: See the 'Note about fast-forwards' in 'git push --help' for details.

  

$ git pull       该命令也可能会出现如下错误，代表没有指定从github上的哪个分支pull数据
There is no tracking information for the current branch.
Please specify which branch you want to merge with.
See git-pull(1) for details.

    git pull <remote> <branch>

If you wish to set tracking information for this branch you can do so with:

    git branch --set-upstream-to=origin/<branch> dev

$ git branch --set-upstream-to=origin/dev dev  通过该命令可以进行设置


$ git pull   git pull 实际上是将github上最新的文件内容更新到本地，如果本地本身没有github上的文件，则使用clone，文件已存在就使用pull
remote: Counting objects: 3, done.
remote: Compressing objects: 100% (2/2), done.
remote: Total 3 (delta 1), reused 3 (delta 1), pack-reused 0
Unpacking objects: 100% (3/3), done.
From github.com:zzh546934282/learngit
   4503b2b..0ec2976  dev        -> origin/dev
Auto-merging readme.txt
CONFLICT (content): Merge conflict in readme.txt
Automatic merge failed; fix conflicts and then commit the result.


可以看出上面告诉我们更新文件内容以后出现了冲突

员工A进入文件对比以后最终决定用自己修改后的版本

于是他改完工作空间中的文件重新提交到了电脑A中的dev分支

$ git add readme.txt

$ git commit -m 'commit'
[dev a6334a3] commit

再同步到github
$ git push origin dev
Counting objects: 4, done.
Delta compression using up to 2 threads.
Compressing objects: 100% (4/4), done.
Writing objects: 100% (4/4), 456 bytes | 456.00 KiB/s, done.
Total 4 (delta 1), reused 0 (delta 0)
remote: Resolving deltas: 100% (1/1), completed with 1 local object.
To github.com:zzh546934282/learngit.git
   0ec2976..a6334a3  dev -> dev
```

### **Rebase**

**Rebase能够使错综复杂的分支图变成一条直线**

下面叙述个情景作为引入

情景：员工A向github中push了dev分支，员工B更改了自己电脑上的文件，并提交到了自己电脑上的dev分支中，查看分之图如下，origin/dev 代表github上面的dev分支的Head所指向的提交点，HEAD -> dev 代表 当前电脑上也就是员工B自己电脑上的本地git仓库中的dev分支的Head目前所指向的commit点。可以发现本地所指向的点要比github上快一部，因为员工B在提交自己所更改的内容到本地git之前，并没有进行 从github上 pull 文件。

![](https://img-blog.csdn.net/20180918142827246?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

员工B此时将自己的本地的dev分支 push到github上面，果不其然会出现问题，git提示必须执行pull操作，员工B 执行了 git pull，将github的dev分支更新到了本地，但是又出现了冲突，于是他解决了冲突并提交了代码到本地git仓库中

```
$ git commit -m 'commit'
[dev 4d12985] commit
```

此时我们再次查看分支图，如下 

![](https://img-blog.csdn.net/20180918142730478?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

执行 git commit -m 'commit'  命令前 分支图 ，如下 

![](https://img-blog.csdn.net/20180918144158669?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以发现，员工B在解决冲突后的commit的之前，分支图中并没有出现新的commit点。一旦commit以后出现了3个commit点，其中最上面的4d12985 是员工B在 本地的git仓库的dev分支中操作的， 而 a6334a3 和 7a0ddf9 是自动出现的，至于两个commit做了什么我目前还不清楚。

其中 0ec2976 后面的 origin/dev 是我在执行 git pull 操作之后消失的，当解决完冲突并提交工作空间文件到本地git仓库的dev分支后，这个origin/dev 又出来了

总的来说：

1、当我们从github上 pull 获取代码发生冲突手动解决然后commit以后，会多出3个commit节点，最后一个节点对应的是我们在本地git仓库中的commit的操作

2、当我们从github 上 pull 获取代码时，github上的 origin/dev 指针会消失，当你commit本地代码以后，它又会出现。

现在我们使用git status 来查看一下状态

```
user@user-PC MINGW64 /d/B/learngit (dev)
$ git status
On branch dev
Your branch is ahead of 'origin/dev' by 2 commits.
  (use "git push" to publish your local commits)

nothing to commit, working tree clean
```

 上面指出 本地git 比 github 快2个提交点，这个是没问题的，因为本地git因为对文件做了修改，然后提交了，这就是本身就多了一个提交点，然后上面又从github上 pull 文件，然后解决冲突又提交了一次，所以是两个提交点。

然后我们也可以发现，上面的分支图实在是太乱了，所以rebase出现了，我们可以通过rebase使分支图尽量简化。

```
$ git rebase
```

### **标签**

**添加标签**

$ git tag 标签名        给当前分支最新提交点 设置标签

$ git tag v0.9 commit id       给当前分支的某个提交点设置标签， 通过 commit id 来指定

$ git tag  查看标签 ()

$ git show <标签名>    查看标签信息

$ git tag -a v0.1 -m "这里用来填写标签说明" commit id   给指定提交点创建带说明的标签

$ git push origin v1.0   将本地的标签 推送到远程库

**删除标签**

$ git tag -d v1.1     删除指定标签

$ git push origin :refs/tags/v1.1  删除远程库的标签，注意要先删除本地标签，再删除远程库标签

### **码云**

码云是一个跟github类似的国内git托管仓库，国内使用这个网速会比较顺畅。

用法跟github也一样，把公钥粘贴到SSH设置中，然后创建连接。

如果我们本地的git 想同时 配置 码云 和 github 那么就不能够使用 远程仓库的默认名字origin，应用以下命令分别创建连接。

$ git remote add gitee git@gitee.com:zzh666666/learngit.git   创建 码云 连接

$ git remote add github git@github.com:zzh546934282/learngit.git  创建 github 连接

$ git remote -v  查看被创建连接的远程库

$ git remote rm 连接名  删除指定远程库的连接

$ git push 仓库名 分支名   向远程仓库传代码

如果出现  fatal: refusing to merge unrelated histories  错误 ，请输入下面的命令

$ git pull gitee master --allow-unrelated-histories

原因是 远程仓库与本地仓库 没有一个共同的commit点，git认为我们传错了远程仓库，上面的代码代表关闭该检测。

### 忽略特殊文件

如果被上传的本地仓库中有某些文件不想上传到类似于github或码云这种公开的远程仓库中，如连接数据库的配置文件，其包含链接地址用户名和密码，但是如果你不上传的话每次git status 又会提示 Untracked files ...。 为了解决这个问题我们可以在本地仓库中 加入 一个名为 .gitignore 文件，在这个文件中我们可以配置不想被上传的文件，这样的话 我们git status 就会忽略掉这些文件了。  .gitignore 不需要自己从头写，github提供了很多模板，我们根据自己的需求对其加以改动就行了。

模板 [在这里](https://github.com/github/gitignore/blob/master/Java.gitignore)

忽略文件的原则是：
- 忽略操作系统自动生成的文件，比如缩略图等；
- 忽略编译生成的中间文件、可执行文件等，也就是如果一个文件是通过另一个文件自动生成的，那自动生成的文件就没必要放进版本库，比如Java编译产生的`.class`文件；
- 忽略你自己的带有敏感信息的配置文件，比如存放口令的配置文件。

记得要把`.`gitignore 也提交到Git。当然检验`.`gitignore 的标准是`git status`命令是不是说`working directory clean`。

使用Windows的童鞋注意了，如果你在资源管理器里新建一个`.`gitignore 文件，它会非常弱智地提示你必须输入文件名，但是在文本编辑器里“保存”或者“另存为”就可以把文件保存为`.`gitignore 了。

有些时候，你想添加一个文件到Git，但发现添加不了，原因是这个文件被`.`gitignore 忽略了：

```
$ git add App.class
The following paths are ignored by one of your .gitignore files:
App.class
Use -f if you really want to add them.
```

如果你确实想添加该文件，可以用`-f`强制添加到Git：

```
$ git add -f App.class
```

或者你发现，可能是`.`gitignore 写得有问题，需要找出来到底哪个规则写错了，可以用`git check-ignore`命令检查：

```
$ git check-ignore -v App.class
.gitignore:3:*.class    App.class
```

Git会告诉我们，`.`gitignore 的第3行规则忽略了该文件，于是我们就可以知道应该修订哪个规则。

### 配置别名

我们可以给git中的命令起一个别的名字，从而方便使用

$ git config --global alias.别名 原名    

举例如下

![](https://img-blog.csdn.net/20180918220740985?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**配置文件 **

 我们上面配置的别名会保存在一个配置文件中，注意上面的命令中我们使用了 --global ，意思设置对电脑中的所有本地git仓库均有效，如果没有加--global 就仅仅对当前仓库有效。

如果**使用了 --global **那么配置信息会存在 用户根目录下的.gitconfig文件中

![](https://img-blog.csdn.net/20180918221449396?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果没使用 --global 那么配置信息在当前仓库中的 .git 文件夹下的config文件中

![](https://img-blog.csdn.net/20180918221621249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

要想删除某个别名直接删除文件中[alias] 下的配置信息就行了。

![](https://img-blog.csdn.net/20180918221838127?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

