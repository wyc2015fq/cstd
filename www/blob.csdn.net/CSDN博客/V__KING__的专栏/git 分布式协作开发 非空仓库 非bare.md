# git 分布式协作开发 非空仓库 非bare - V__KING__的专栏 - CSDN博客





2017年11月27日 21:48:29[v__king__](https://me.csdn.net/V__KING__)阅读数：261









参考：[http://www.xuebuyuan.com/1775952.html](http://www.xuebuyuan.com/1775952.html)









场景：

    1. 在没有服务器的情况下，几个人合作开发。 完全体现git分布式版本管理。每个人的git init example(不是用git --bare init example.git) 都可以作为分布式的节点。


    2. 一般git教程都是推荐使用git --bare init example.git  来建立空仓库，集中式管理。


    注意：


        你要push到别人的机器上，需要经过别人机器的同意。需要在被push的机器上：git config receive.denyCurrentBranch warn






场景演练1：A:在master分支 ， B在master分支，都在master分支上


A机器上：


    1. git init example


    2. git touch README.md


    3. git add .


    4. git commit -m"first commit"


B机器上：


    1. git clone git@192.168.1.200:/home/git/example


    2. git touch 1.txt


    3. git add .


    4. git commit -m"add 1.txt"


    5. git remote add origin git@192.168.1.200:/home/git/example


可能出现的问题：
    1. Q:  A怎么知道B已经push了?会不会当Apush后

        A:  A机器git status时，提示有modfy。但是这个modfy是在缓存里面的。而没有悄悄的覆盖工作目录。当你切换branch时，会自动的将缓存里的modfy恢复到master分支。





所以正确的工作方式：


    1. 不能直接在master分支上进行操作，把master分支作为服务中心站。


    2. 新建一个develop分支作为主要工作分支








git push origin master出现如下错误：
Counting objects: 3, done.
Writing objects: 100% (3/3), 226 bytes, done.
Total 3 (delta 0), reused 0 (delta 0)
remote: error: refusing to update checked out branch: refs/heads/master
remote: error: By default, updating the current branch in a non-bare repository
remote: error: is denied, because it will make the index and work tree inconsistent
remote: error: with what you pushed, and will require 'git reset --hard' to match
remote: error: the work tree to HEAD.
remote: error: 
remote: error: You can set 'receive.denyCurrentBranch' configuration variable to
remote: error: 'ignore' or 'warn' in the remote repository to allow pushing into
remote: error: its current branch; however, this is not recommended unless you
remote: error: arranged to update its work tree to match what you pushed in some
remote: error: other way.
remote: error: 
remote: error: To squelch this message and still keep the default behaviour, set
remote: error: 'receive.denyCurrentBranch' configuration variable to 'refuse'.
To git@192.168.45.42:teamwork.git
 ! [remote rejected] master -> master (branch is currently checked out)
error: failed to push some refs to 'git@192.168.45.42:teamwork.git'
执行命令解决问题
git config receive.denyCurrentBranch ignore
git pull出现如下错误：
You asked me to pull without telling me which branch you
want to merge with, and 'branch.master.merge' in
your configuration file does not tell me, either. Please
specify which branch you want to use on the command line and
try again (e.g. 'git pull <repository> <refspec>').
See git-pull(1) for details.

If you often merge with the same branch, you may want to
use something like the following in your configuration file:

    [branch "master"]
    remote = <nickname>
    merge = <remote-ref>

    [remote "<nickname>"]
    url = <url>
    fetch = <refspec>

See git-config(1) for details.

执行命令：
$ git config branch.master.remote origin
$ git config branch.master.merge refs/heads/master
解决问题







