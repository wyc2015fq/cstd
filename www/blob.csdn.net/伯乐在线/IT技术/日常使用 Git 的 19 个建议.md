# 日常使用 Git 的 19 个建议 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [zaishaoyi](http://www.jobbole.com/members/zaishaoyi) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[Alex](http://www.alexkras.com/19-git-tips-for-everyday-use/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
我使用Git已经有4年之久，在这里想分享一些实用的小技巧，希望能对大家有所帮助。
如果你对git一无所知，那么我建议先去读一下[Git 常用命令速查](http://www.alexkras.com/getting-started-with-git/)。本篇文章主要适合有一定 git 使用基础的人群。
目录：
- 日志输出参数
- 查看文件的详细变更
- 查看文件中指定位置的变更
- 查看尚未合并（merge）的变更
- 查看其他分支中的文件
- 关于变更基线(rebase)的几点说明
- 本地合并之后保留分支结构
- 修复而非新建提交
- 的三种状态以及它们的相互转换
- 优雅地回退
- 使用第三方工具查看整个项目（而非单独文件）的变更
- 忽略空格变更
- 追加文件中的部分变更
- 发现并清理无用分支
- 暂存部分文件
- 如何写好提交信息
- 自动补全
- 创建常用命令的别名
- 快速定位问题版本
# 1. 日志输出参数
命令示例：


```
git log --oneline --graph
```
也许你用过git log。它支持很多命令行参数，将这些参数结合起来使用，功能尤为强大。下面是我经常使用的一些参数：
- –author=“Alex Kras” ——只显示某个用户的提交任务
- –name-only ——只显示变更文件的名称
- –oneline——将提交信息压缩到一行显示
- –graph ——显示所有提交的依赖树
- –reverse ——按照逆序显示提交记录（最先提交的在最前面）
- –after ——显示某个日期之后发生的提交
- –before ——显示发生某个日期之前的提交
例如，曾经有位主管要求在每周五提交周报。所以我每周五都运行一下这个指令： `git log --author="Alex Kras" --after="1 week ago" --oneline，然后将输出结果编辑一下，发送给主管审核。`
Git有很多命令行参数，使用起来非常方便。运行 `man git log ，来看一下这些参数的作用。`
如果这些都不好用，git还有一个 `--pretty 参数，可以用来创建高级自定义输出。`
![git-log-oneline](http://jbcdn2.b0.upaiyun.com/2015/09/e1be9c2f4a330419974265145b44a776-1024x600.png)
# 2. 查看文件的详细变更
命令示例：

Shell
```
git -log -p filename
```
git log -p 或者 git log -p filename 不仅显示提交说明、提交者以及提交日期，还会显示这每次提交实际修改的内容。
然后你就可以使用 `less 中常用的检索命令即“斜杠”后面加检索词/{{在此处添加你的检索词}}），在变更内容日志中检索指定的关键词（使用小写的n跳到下一条检索结果，大写的N跳到上一条检索结果）。`
![git-log-search](http://jbcdn2.b0.upaiyun.com/2015/09/d90e7422af3e80e0a749fa011ff7f18b-1024x599.png)
# 3. 查看文件中指定位置的变更
命令示例：

Shell
```
git log -L 1,1:some-file.txt
```
你可以使用 `git blame filename 追查出文件中每一行是由谁变更的。`
![git-blame](http://jbcdn2.b0.upaiyun.com/2015/09/14a5c5235bd1c981b5682e1ac31f8eee-1024x328.png)
`git blame 是一个非常强大的工具，但是又是无法提供足够的信息。`
`git log 提供了一个 `-L 的选项。这个选项允许指定文件中的某些行。Git只会输出与这些行的变更日志。这有点像带焦点的 `git log -p 。```

Shell
```
git log -L 1,1:some-file.txt
```
![git-log-lines](http://jbcdn2.b0.upaiyun.com/2015/09/c32c6d93afe9f8b4ec55b906a72f3cfd-1024x345.png)
# 4. 查看尚未合并的变更
命令示例：

Shell
```
git log --no-merges master..
```
如果你曾经与很多小伙伴工作在同一个持久分支上，也许会有这样的经历，父分支（例如：master）上的大量合并同步到你当前的分支。这使得我们很难分辨哪些变更时发生主分支，哪些变更发生在当前分支，尚未合并到master分支。
`git log --no-merges master..可以解决这个问题。注意`--no-merges 标志意味着只显示没有合并到任何分支的变更，**master..**选项，意思是指显示没有合并到master分支的变更（在master后面必须有..）。``
你也可以运行 `git show --no-merges master.. 或者 `git log -p --no-merges master.. 命令（输出结果相同）来查看一下尚未合并的文件变更。``
# 5. 查看其他分支中的文件
示例：


```
git show some-branch:some-file.js
```
用这个命令可以很方便地查看其他分支上的文件而无需切换到那个分支。
当然你也可以通过 `git show some-branch-name:some-file-name.js 命令在终端中显示指定的文件.`
你还可以将输出重定向到一个临时文件，这样你就可以再指定的编辑器中，以并排视图来查看它了。

Shell
```
git show some-branch-name:some-file-name.js > deleteme.js
```
如果你想查看另一个分支上文件与当前分支上文件的差异，只要运行下面的命令就可以了：


```
git diff some-branch some-filename.js
```
# 6.关于变更基线的几点说明
示例：

Shell
```
git pull --rebase
```
之前我们说过在远程分支上工作会有大量的合并提交。使用 `git rebase 可以避免这些提交。`
总的来说我认为变更基线是高级特征，最好是留到另一篇文章中详细介绍。
甚至在git book中也有这样的论述：
> 
但是，令人狂喜的变更基线并不是任何情况下都适用，一言以蔽之：
若是工作区中存在尚未提交到仓库的变更，请不要使用变更基线。
如果遵照这条指南，不会有什么问题。不然，你可能会招致厌恶与谩骂。
https://git-scm.com/book/en/v2/Git-Branching-Rebasing#The-Perils-of-Rebasing
也就是说，变更基线本身并不可怕，关键在于使用方式。
或许，最好的方法是使用交互式变更基线，调用命令为 `git rebase -i {{某个提交序列号}}。运行这条命令，会打开一个带有自解释指令的编辑器。由于变更基线不在本文的叙述范围之内，我们就此而止，不再深究。`
![git-rebase-i](http://jbcdn2.b0.upaiyun.com/2015/09/6b0f5e4f32904ab630233e6a1eac0146-1024x839.png)
### 变更基线一个非常有用的特殊用法 git pull –rebase。
举个例子，假设你正在master分支的一个本地版本上工作，你已经向仓库提交了一小部分变更。与此同时，也有人向master分支提交了他一周的工作成果。当你尝试推送本地变更时，git提示你需要先运行一下 `git pull ， 来解决冲突。作为一个称职的工程师，你运行了一下 `git pull ，并且git自动生成了如下的提交信息。``
> 
Merge remote-tracking branch ‘origin/master’
尽管这不是什么大问题，也完全安全，但是不太有利于历史记录的聚合。
这种情况下，`git pull --rebase 是一个不错的选择。`
这个命令会迫使git将远程分支上的变更同步到本地，然后将尚未推送的提交重新应用到这个最新版本，就好象它们刚刚发生一样。这样就可以避免合并以及随之而来的丑陋的合并信息了。
# 7. 本地合并之后保留分支结构
示例：

Shell
```
git merge --no-ff
```
我喜欢为每一个bug或者特征创建一个新的分支。最大的好处就是，可以清楚地知道一系列的提交与某个任务的关系。如果你曾经合并过github 或者类似工具上的同步请求，那么可以通过运行 `git log --oneline --graph 显示的视图，清楚地看到合并分支的历史。`
如果你试图合并一个本地分支到另一个本地分支，也许会注意到git将两个分支平滑地衔接在一起，在git历史中看到的是一条直线。。
如果你想强迫git保存分支的历史，与合并同步请求的状况类似，你可以加一个 `--no-ff 标志, 最后可以看到非常清楚的提交历史树。`
git merge –no-ff some-branch-name
![git-merge-no-ff](http://jbcdn2.b0.upaiyun.com/2015/09/3519d57c3f66b63de65cdc41108d04e9-1024x603.png)
# 8. 修复而非新建提交
示例：

Shell
```
git commit --amend
```
这个指令顾名思义。
假设提交之后，你意识到自己犯了一个拼写错误。你可以重新提交一次，并附上描述你的错误的提交信息。但是，还有一个更好的方法：
如果提交尚未推送到远程分支，那么按照下面步骤简单操作一下就可以了：
- 修复你的拼写错误
- 将修正过的文件暂存，通过git add some-fixed-file.js
- 运行 git commit –amend 命令，将会把最近一次的变更追加到你最新的提交。同时也会给你一个编辑提交信息的机会。
- 准备好之后，将干净的分支推送到远程分支。
![](http://www.alexkras.com/wp-content/uploads/git-commit-amend.gif)
如果你工作在自己的分支，甚至可以在已经推送之后修正提交，你需要使用 git push -f （-f 代表强制执行），这条指令可以重写历史。但是，不要试图在一个很多人共同工作的分支（正如我们在变更基线那一部分讨论的分支）上这样做。此时，新建一次提交，在提交信息中描述错误，应该是最好的补救措施。
# 9. Git 中的三种状态以及它们之间的转换
示例：

Shell
```
git reset --hard HEAD 与 git status -s
```
目前你或许已经了解，git中的文件可以有三种不同的状态：
- 没有暂存
- 暂存并准备提交
- 已经提交
通过运行 git status可以看到关于文件的描述以及文件的状态。 运行 git add filename.js 命令可以将文件从未暂存状态移动到暂存并准备提交的状态， 或者使用 git add . 命令一次性暂存所有的文件。
通过运行 git status -s 命令可以看到状态图，其中 -s 是简短（short）的意思（个人认为），最终输出结果如图所示：
![git-stages](http://jbcdn2.b0.upaiyun.com/2015/09/b04073ef85c8e1d217400bcf20de86fd-1024x280.png)
显然，git status 不显示已经提交了的文件，你可以使用 git log 命令来查看。
若要将文件在不同阶段之间转换，有很多可以用的命令供你选择。
### 重置文件
在git中，有3种类型的重置。重置是让文件回到git历史中的一个特定版本。
- git reset –hard {{some-commit-hash}} —— 回退到一个特定的历史版本。丢弃这次提交之后的所有变更。
- git reset {{some-commit-hash}}—— 回滚到一个特定的历史版本。将这个版本之后的所有变更移动到“未暂存”的阶段。这也就意味着你需要运行 git add . 和 git commit 才能把这些变更提交到仓库.
- git reset –soft {{some-commit-hash}} ——回滚到一个特定的历史版本。将这次提交之后所有的变更移动到暂存并准备提交阶段。意味着你只需要运行 git commit 就可以把这些变更提交到仓库。
这些命令似乎并没有什么用处，但当你尝试着将文件在不同版本间移动时，使用它们会非常方便。
我平时使用重置的一些用例如下：
- 如果想清除变更记录，可以使用清理命令——git reset –hard HEAD （最常用）
- 如果想编辑，以不同的顺序，重新暂存，重新提交文件—— git reset {{some-start-point-hash}}
- git reset –soft {{some-start-point-hash}}如果想把之前3次的提交，作为一次提交 git reset –soft {{some-start-point-hash}}
### 签出部分文件
如果你想取消某些文件在本地的变更，而同时保留另外一些文件在本地的变更，一个比较简单的方法是通过 git checkout forget-my-changes.js签出那些你想取消本地的变更的文件。
正如前面提到的那样，你也可以从其他分支或者之前的提交中签出文件的不同版本。
git checkout some-branch-name file-name.js 和 git checkout {{some-commit-hash}} file-name.js
你应该注意到了签出的文件处于“暂存并准备提交”的状态。如果想回到未暂存的状态，需要执行一下 git reset HEAD file-name.js。然后再次执行 git checkout file-name.js,文件回到了初始状态。
注意，运行 git reset –hard HEAD file-name.js 不起作用。总而言之，在git的不同阶段之间移动有点复杂，没有一个清晰的模式，我希望能通过这一部分有所改观。
# 10. 撤销而不产生提交信息
示例：

Shell
```
git revert -n
```
如果打算撤销之前一次或者两次的提交，查看这些提交都做了哪些变更，哪些变更又有可能引发问题，这个命令非常方便。
通常，git revert 会自动将回退的文件提交到仓库，需要你写一个新的提交信息。-n 标志告诉git先别急着提交，因为我只是想看一眼罢了。
# 11.用第三方差异工具查看整个工程而非单个目录的差异
示例：


```
git difftool -d
```
我最喜欢的差异工具是[Meld](http://www.alexkras.com/how-to-run-meld-on-mac-os-x-yosemite-without-homebrew-macports-or-think/)。我在使用Linux的时候就开始使用它，并且一直持续到现在。
我并不是要推销Meld。假设你已经选好了比较工具，并且git能够将它作为一个合并和差异工具使用。接下来需要运行一下下面的命令，注意用你选择的差异工具的名字代替Meld：

Shell
```
git config --global diff.tool.meld
git config --global merge.tool meld
```
之后你就可以运行s run git difftool some-file.js 来查看文件的差异了。
但是，有些比较工具（例如meld）支持全路径比较。
如果你调用 git difftool 时加 -d 标志，将会对整个文件夹进行比较。有时会非常有用。
git difftool -d
![git-difftool-d](http://jbcdn2.b0.upaiyun.com/2015/09/dd0f0dd205988266a55b34f20ef8cba7-1024x697.png)
# 12. 忽略空格变更
示例：

Shell
```
git diff -w 或者 git blame -w
```
你是否遇到这样的情况：直到git blame 显示你应该为文件中的一切负责时才意识到自己重新调整了文件的缩进或者格式？
结果证明，git足够聪明来分辨文件中不同类型的变更。你可以调用许多命令（例如:：git diff, git blame），加一个-w 标志，git将会忽略空白的变更。
# 13. 追加文件中的部分变更
示例：


```
git add -p
```
在git上一定有些人非常喜欢-p 标志，因为它总是带来某些非常方便的功能。
使用 git add，允许你交互地选择你想要提交的内容。这样你就可以以简单易读的方式按照一定的逻辑组织提交。
![git-add-p](http://jbcdn2.b0.upaiyun.com/2015/09/5ebe859da567a509d851ae10c9b5c95d-1024x996.png)
# 14. 发现并清理无用分支
示例：


```
git branch -a
```
仓库中存在大量远程分支的现象非常常见，甚至其中某些分支已经被合并到了master分支。如果你跟我一样有洁癖（至少有代码洁癖），这些分支可能会令你难以忍受。
你可以通过运行git branch查看所有的远程分支，还可以带有 -a 标志（显示所有的分支），或者带上 –merged标志 只显示那些完全合并到master分支的分支。
你或许首先想到的是运行git fetch -p （获取和清除旧数据），来确保你的数据是最新的。
![git-branch-a-merged](http://jbcdn2.b0.upaiyun.com/2015/09/a18b4f672088f339b75fad19206ff545-1024x632.png)
如果你要获取真正的fancy，你可以得到一个所有远程分支的列表，以及这些分支最后一次提交的列表，通过运行： 
git for-each-ref –sort=committerdate –format=’%(refname:short) * %(authorname) * %(committerdate:relative)’ refs/remotes/ | column -t -s ‘*’.
![fancy-branch-view](http://jbcdn2.b0.upaiyun.com/2015/09/41bab984431d20afb94861372d9fce4c-1024x398.png)
不幸地是，没有简单的方法（至少我不知道）可能只显示合并过的分支。所以你可能不得不比较两个输出或者写一个脚本来做这些事情。
# 15. 暂存部分文件
示例：

Shell
```
git stash --keep-index 或者 git stash -p
```
如果你还不了解 git stash 的功能，只是把当前工作区中的变更保存到一个有序的“git stack”。之后你可以用 git stash pop ，恢复你的变更。你也可以使用 git stash list 查看git栈里面你做的所有备份。通过 man git stash 查看更多可以用的选项。
常规 git stash 的一个限制是它会一下暂存所有的文件。有时，只备份某些文件更为方便，让另外一些与代码库保持一致。
还记得神奇的 -p命令吗？是的，它与 git stash 一起用会非常方便。你现在或许已经猜到了，它会询问你想备份哪些文件的变更。
为了确认一下，点击 ? 你可以看到所有可用的选项。
![git-stash-p](http://jbcdn2.b0.upaiyun.com/2015/09/49039ae6b4150bdac7cef09ce34bb4e6-1024x939.png)
另一个非常有用的技巧，用来备份部分文件：
- add 那些你不想备份的文件（例如： git add file1.js, file2.js）
- 调用 git stash –keep-index。只会备份那些没有被add的文件。
- 调用 git reset 取消已经add的文件的备份，继续自己的工作。
# 16. 写好提交信息
刚刚读过一篇很好的文章，关于如何写好提交信息，点击这个链接阅读：[How to Write a Git Commit Message](http://chris.beams.io/posts/git-commit/)
有一个规则真的是为我量身订做的：“每一个好的提交应该能完善下面的这个句子”
应用到实际中，提交信息应该是这样的：{{你的提交信息}}
例如：
—应用这次提交，可以**更新****README文件** 
—应用这次提交:**为调用GET/user/:id API追加确认**
—应用这次提交：**会回退到****12345版本**
# 17. Git 自动补全
某些操作系统（例如：Ubuntu）的git包自带并且默认开启自动补全。如果你的操作系统没有这个功能（Mac就没有），你可以按照下面的指南为自己添加。
[https://git-scm.com/book/en/v1/Git-Basics-Tips-and-Tricks#Auto-Completion](https://git-scm.com/book/en/v1/Git-Basics-Tips-and-Tricks#Auto-Completion)
# 18. 创建常用命令的别名
常用的较长的git命令应该使用git或者bash别名
使用Git最好的方式是通过命令行，学习命令行的最好方式就是先用最困难的方法做每一件事。（把一切都打印出来）。
然而，一段时间之后，最好将你常用的命令总结出来，为它们创建一个简单的别名。
Git 支持别名，例如，你可以运行一下下面的命令：

Shell
```
git config --global alias.l "log --oneline --graph"
```
这个命令行会创建一个新的git别名l，你可以使用：
git l 代替 git log –oneline –graph。
注意你可以在alias后面附加其他的参数（例如：git l –author =“Alex”）
其他的选项，是好的就得Bash别名
例如，在我的.bashrc文件中有下面的词条：
Alias gil=”git log –oneline -graph”，允许我使用gil代替长命令，甚至比git l还要少两个字母
# 19. 快速定位故障版本
示例：

Shell
```
git bisect
```
git bisect 使用分治算法查找出错版本号。
假设休假一周回来，你看了一下最新代码，发现走之前完全正常的代码现在出问题了。
你查看了一下休假之前最后一次提交的代码，功能尚且正常。不幸的是，你离开的这段时间，已经有上百次提交记录，你无法找到那一次提交导致了这个问题。
![pulling-out-hair](http://jbcdn2.b0.upaiyun.com/2015/09/417416f6afe024100efe66294820d72d.jpg)
这时你或许想找到破坏功能的bug，然后对该文件使用git blame 命令，找出并指责破坏者。
如果bug很难定位，那么或许你可以去看一下提交历史，试一下看能不能找到出问题的版本。
另一种快捷的方式则是使用git bisect，可以快速找到出问题的版本。
### 那么git bitsect是如何做的呢？
指定了已知的正常版本和问题版本之后，git bisectit bisect会把指定范围内的提交信息从中间一分为二，并会根据最中间的提交信息创建一个新的分支， 你可以检查这个版本是否有问题。
假设这个中间版本依然可以正常运行。你可以通过git bisect good命令告诉git。然后，你就只有剩下的一半的版本需要测试。
Git会继续分割剩下的版本，将中间版本再次到处让你测试。
Git bisect会继续用相似的方式缩小版本查找范围，直到第一个出问题的版本被找到。
因为你每次将版本分为两半，所以可以用log(n)次查找到问题版本（时间复杂度为“big O”，非常快）。
### 运行整个git bisect的过程中你会用到的所有命令如下：
- git bisect start ——通知git你开始二分查找。
- git bisect good {{some-commit-hash}} ——反馈给git 这个版本是没有问题的（例如：你休假之前的最后一个版本）。
- git bisect bad {{some-commit-hash}} ——告诉git 已知的有问题的版本（例如master分支中的HEAD）。git bisect bad HEAD （HEAD 代表最新版本）。
- 这时git 会签出中间版本，并告诉你去测试这个版本。
- git bisect bad ——通知git当前版本是问题版本。
- git bisect good ——通知git当前签出的版本没有问题。
- 当找到第一个问题版本后，git会告诉你。这时， git bisect 结束了。
- git bisect reset——返回到 git bisect进程的初始状态（例如，master分支的HEAD版本）。
- git bisect log ——显示最后一次完全成功的 git bisect日志。
你也可以给git bisect提供一个脚本，自动执行这一过程。详细内容请点击： [http://git-scm.com/docs/git-bisect#_bisect_run](http://git-scm.com/docs/git-bisect#_bisect_run)
