# git reset soft,hard,mixed之区别深解 - z69183787的专栏 - CSDN博客
2017年01月17日 22:05:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：806
GIT reset命令，似乎让人很迷惑，以至于误解，误用。但是事实上不应该如此难以理解，只要你理解到这个命令究竟在干什么。
首先我们来看几个术语
- 
HEAD
这是当前分支版本顶端的别名，也就是在当前分支你最近的一个提交
- 
Index
index也被称为staging area，是指一整套即将被下一个提交的文件集合。他也是将成为HEAD的父亲的那个commit
- 
Working Copy
working copy代表你正在工作的那个文件集
- 
Flow
当你第一次checkout一个分支，HEAD就指向当前分支的最近一个commit。在HEAD中的文件集（实际上他们从技术上不是文件，他们是blobs（一团），但是为了讨论的方便我们就简化认为他们就是一些文件）和在index中的文件集是相同的，在working copy的文件集和HEAD,INDEX中的文件集是完全相同的。所有三者(HEAD,INDEX(STAGING),WORKING COPY)都是相同的状态，GIT很happy。
当你对一个文件执行一次修改，Git感知到了这个修改，并且说：“嘿，文件已经变更了！你的working copy不再和index,head相同！”，随后GIT标记这个文件是修改过的。
然后，当你执行一个git add,它就stages the file in the index，并且GIT说：“嘿，OK，现在你的working copy和index区是相同的，但是他们和HEAD区是不同的！”
当你执行一个git commit,GIT就创建一个新的commit，随后HEAD就指向这个新的commit，而index,working copy的状态和HEAD就又完全匹配相同了，GIT又一次HAPPY了。
- 
Reset
如果你仔细研究reset命令本身就知道，它本身做的事情就是重置HEAD(当前分支的版本顶端）到另外一个commit。假设我们有一个分支（名称本身无所谓，所以我们就简单称为"super-duper-feature”分支吧），图形化表示如下：
![技术分享](http://images0.cnblogs.com/blog2015/737565/201505/182225251979460.png)
如果我们执行：
```
git reset HEAD
```
任何事情都不会发生，这是因为我们告诉GIT重置这个分支到HEAD，而这个正是它现在所在的位置。
```
git reset HEAD~1
```
当我们再执行上面的命令时（HEAD~1是“the commit right before HEAD”的别名，或者说：put differently "HEAD‘s parent"），我们的分支将会如下所示
![技术分享](http://images0.cnblogs.com/blog2015/737565/201505/182229495415187.png)
如果我们执行git reset HEAD~2,则意味着将HEAD从顶端的commit往下移动两个更早的commit。
- 
Parameters
- 
soft
--soft参数告诉Git重置HEAD到另外一个commit，但也到此为止。如果你指定--soft参数，Git将停止在那里而什么也不会根本变化。这意味着index,working copy都不会做任何变化，所有的在original HEAD和你重置到的那个commit之间的所有变更集仍然在stage(index)区域中。
![技术分享](http://images0.cnblogs.com/blog2015/737565/201505/182237338854646.png)
　　2.hard
--hard参数将会blow out everything.它将重置HEAD返回到另外一个commit(取决于~12的参数），重置index以便反映HEAD的变化，并且重置working copy也使得其完全匹配起来。这是一个比较危险的动作，具有破坏性，数据因此可能会丢失！如果真是发生了数据丢失又希望找回来，那么只有使用：[git
 reflog](http://blog.csdn.net/ibingow/article/details/7541402)命令了![技术分享](http://images0.cnblogs.com/blog2015/737565/201505/182238305101646.png)
　　3.mixed(default）
--mixed是reset的默认参数，也就是当你不指定任何参数时的参数。它将重置HEAD到另外一个commit,并且重置index以便和HEAD比配，但是也到此为止。working copy不会被更改。所以所有从original HEAD到你重置到的那个commit之间的所有变更仍然保存在working copy中，被标示为已变更，但是并未staged的状态
![技术分享](http://images0.cnblogs.com/blog2015/737565/201505/182238202609380.png)
[git reset soft,hard,mixed之区别深解](http://www.bubuko.com/infodetail-814578.html)
标签：[class](http://www.bubuko.com/so/1/class)[style](http://www.bubuko.com/so/1/style)[log](http://www.bubuko.com/so/1/log)[com](http://www.bubuko.com/so/1/com)[使用](http://www.bubuko.com/so/1/%E4%BD%BF%E7%94%A8)[src](http://www.bubuko.com/so/1/src)[http](http://www.bubuko.com/so/1/http)[it](http://www.bubuko.com/so/1/it)[文件](http://www.bubuko.com/so/1/%E6%96%87%E4%BB%B6)
原文：http://www.cnblogs.com/kidsitcn/p/4513297.html
