# Git深度使用经验总结 - z69183787的专栏 - CSDN博客
2015年05月27日 13:39:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：792
      已经开始使用Git了，有些时候一般的问题只能Google就能找到答案，但是有些时候不能找到答案，因为遇到的问题都不知道该用什么关键字搜索。很多是在Git中文的QQ群中讨论出来的，把一些经验总结一下：
**1.Git子模块**
使用情况：例如Android系统代码和app之间可以使用这种submodule的方法来管理。
快速上手:
$ git submodule add url path/to/name 
$ git submodule init
$ git submodule foreach git pull
其它情况下submodule不适用，可以用subtree来代替。
clone代码以及pull时候要这样更新：
git submodule update
**2.同步一个COMMIT**
$ git cherry-pick xxx
可以同步一个commit到本分支
**3.Git打TAG**
打TAG也就是发布版本
$ git tag -a v1.2 -m "version 1.4"
$ git push --tags
如果还不能理解可以到这里看看是linus是怎么给linux内核打的TAG，TAG看起来像什么：[https://github.com/torvalds/linux/releases](https://github.com/torvalds/linux/releases)
**4.查看status详情**
$ git add xxx
$ git diff --cached
这样可以在commit之前先看一下修改详情。
**5.不产生无用的merge的同步**
有这么一种情况，用一个分支专门同步代码提供商的代码的时候，如果一般的pull会不断的产生一个merge看起来会很烦，用下边的使用添加一个--rebase就不会产生无用的merge了
$ git pull --rebase origin master
**6.关于stash**
适用情况：做了修改后，还没有add commit等等后续工作，现在突然要切换分支做其它事情，默认情况下你在这个分支修改的代码会被带到切换过去的分支中。可以先把你修改的保存起来。这些修改可以再还原过来。
$ git stash -u
$ xxxx 随便你的操作
$ git stash pop
注意:-u是代表是也把添加的新文件（术语是未跟踪）也藏起来，一般是要有这个u的。
**7.恢复一个COMMIT**
如果一个COMMIT你不想要了，想要去除，可以考虑使用以下的方法；
$ git revert xxxx
这个就可以去掉这个COMMIT的改动，这个是明式的去掉，如果你又后悔了，还可以再次恢复。
**8.安装最新Git版本**
能轻易安装的一般不会是最新的，用apt-get install便是如此。下面用离线的方式安装Git。
$ sudo apt-get install libssl-dev libcurl4-openssl-dev libexpat1-dev
a先到[https://github.com/git/git/releases](https://github.com/git/git/releases)看最新版本，然后复制链接。红色随具体版本变化。
$ apt-get remove git # 卸载现有
$ sudo apt-get install libssl-dev libcurl4-openssl-dev libexpat1-dev #安装必要的库
$ wget https://github.com/git/git/releases/tag/v1.8.4.3 (链接地址)
$ tar xvzf v1.8.4.3.tar.gz
$ cd git-v1.8.4.3
$ make prefix=/opt/git all
$ make prefix=/opt/git install
$ 添加/opt/git/bin到环境变量中 /etc/environment中，或者其它。
**9.安装subtree**
$ git clone https://github.com/apenwarr/git-subtree
$ cp git-subtree/git-subtree.sh /opt/git/bin/git-subtree
关于使用的subtree的几个帖子：
《[使用git subtree集成项目到子目录](http://aoxuis.me/2013/08/07/git-subtree/)》
《[一个成功的Git分支模型](http://www.juvenxu.com/category/scm/)》
**10.Git之本地忽略**
这个分同种情况：a是本地永久忽略，效果的gitignore一样，只不过不适于写到gitignore中而已，可以自己建立一个本地独享的gitignore，然后git config --global core.excludesfile  文件的绝对路径，也可以直接将本地要忽略的文件添加到.git/info/exclude中。不过上述都是针对没有跟踪的文件来说的，如果文件已经被跟踪了你如果在本地想要忽略它的改动，就不能使用以上的方法了。这正情况b。通俗地讲比如一个编译Android的脚本在其它电脑上都是使用的-j32来编译的，但是你的电脑配置没有别人的好，不能开到-j32，但是这个脚本是已经跟踪过的，你修改了就会在每次的git
 status中看到。对于这种情况Git有一个忽略改动的方法：
$ git update-index --assume-unchanged /path/to/file       #忽略跟踪
$ git update-index --no-assume-unchanged /path/to/file  #恢复跟踪
![](https://img-blog.csdn.net/20131108175710640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2FuZ2Vhcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
之后你在本地修改/path/to/file这个文件，Git也不管它了。就实现了本地忽略。
**11.Gitlab用户头像的问题**
如果装的Git服务器是GitLab的话，是可以使用用户头像的，GitLab管理头像的一般方法是用gravatar，这一点和github一样，是关联email地址的，先在上边注册一个账户，上传一个头像并关联某一个邮箱，Gitlab或者Github会自动去gravatar上拉取你的个人头像。
**12.Git打包源码**
对Git管理的源码进行压缩打包，如果使用tar xvzf xxxx.tag.gz xxxxx的话并不是一个很好的选择，因为会将.git/目录下的中间文件全部压缩，如果只想要某一个版本的源码。Git提供了archive.它会给打包一份纯净的代码。当然这个只适用于发部一个版本的源码，而不是备份Git管理的整套源码。
$ git archive xxxx
xxx代表SHA-1 Hash值。
**13.Git反向打补丁**
一套没有管理有源码，中间有功能A，且也有功能A的补丁文件，可以使用补丁文件进行反打补丁来实现去掉功能A。
$ git apply -R
注：只是收集，还没有完整测试[这里](http://www.longene.org/forum/viewtopic.php?f=17&t=4195)。
**13.关于Git服务器选择和安装**
选择建议GitLab，安装我参考加总结这几个帖子：
1.《 [gitlabhq / doc / install / installation.md](https://github.com/kangear/gitlabhq/blob/master/doc/install/installation.md)》根据国情为官方的安装方法添加了说明的安装文档。
2.《 [在 Ubuntu 12.04 上安装 GitLab6.x](http://blog.csdn.net/xiaobin_hlj80/article/details/10244967)》一个对安装过程打印信息都记录的安装文档，安装过程中可以对比安装过程。
3.《[重设MySQL root密码](http://easwy.com/blog/archives/reset_mysql_root_password/)》中间遇到的问题MySQL密码找不到了，这篇文章给了答案。
**14.合并commit**
1.《[使用git合并多个提交](http://www.708luo.com/?p=17)》
2.《[git如何合并只有两个commit到一个？](http://blog.csdn.net/lynnos/article/details/6287135)》
**15.Git只clone最新版本代码不要.git目录**
这个也是在群中出现了问题，我曾经也有过这样的需求，想法是好的，但是理解是错误的。如果只要最后一个版本的代码，而不要.git目录中的东西，有办法：
`git clone --depth=1 git://someserver/somerepo dirformynewrepo`
`rm -rf !$/.git`
但是这又说明不是要参与开发的，是使用的。使用一般是要使用代码开发最新发布版本，如果只要最新的提交版本是没有用处的。因为它不是稳定的。这样就引入了**发布版本**这一说明，Git中是打Tag，一个tag是一个版本，可以以刚才的需求去找最后一个tag版本。github上是可以直接下载某一个tag版本的。如果要用git
 clone来完成，会复杂一些。现在的github是结合了web的，例如github或者是自己搭建的gitlab都是可以直接从web上下载指定的tag版本。而没有用于版本控制的.git目录。
**16.关于pull和merge**
都在提倡添加**--no-ff**选项。
**17.删除子模块-解决子模块自动找上门的问题**
    也许你并不想要用子模块，但是有时它会自动找上门，那么你要了解一下怎么去除它了。怎么个自动上门，且听我慢慢道来。
    在项目A中，如果用git clone了另外一个项目B，或许你只是想把项目B的代码添加到项目A中，但是这时执行git status/add的时候会发现项目B中的代码，并不会被添加，也就是说不能被跟踪。这是为什么呢，这里因为会自动将项目B做为了子模块管理，就是因为它是一个包含.git目录的完整的工程。解决这个问题有同种方法：
1.在clone项目b的后立即将项目B中的.git目录删除
2.git rm --cached path/to/B
然后再执行git status/add 之类的命令就会可以正常的跟踪了。
**18.恢复一个文件到之前的某个提交**
    这个种情况一般是出在某个bug解决后整理规范代码的时候，解决这个bug的时候可能添加了不少的调试代码，最后解决后，有些调试信息只为了了解代码流程，对以后并没有保留的意义，所以就要将其恢复开始解决这个bug之前的提交。xxxxx代表开始解决这个bug之前的一个提交的哈希码。
对于已经跟踪的文件：
1.git reset xxxxxx path/to/file
2.git checkout path/to/file
对于没有跟踪的文件：
1.git rm path/to/file
**19.合并时只产生合并提交**
    还有其它说法：合并时不合并历史；合并时将所有commit合为一个。
git pull --squash another
    话说一般用于master分支，像linus的[linux内核](https://github.com/torvalds/linux/commits/master)一样。
更多信息见《[Git
 merge no history commit](http://stackoverflow.com/questions/20260732/git-merge-no-history-commit)》《[git merge –squash介绍](http://alpha-blog.wanglianghome.org/2010/08/05/git-merge-squash/)》
**20.关于GUI Clients**
这里《[GUI Clients](http://git-scm.com/downloads/guis)》。
**21.git pull 前简单审查**
情况是这样的：本人管理的是develop分支，其他人管理的他们自己的分支。所以应该每次改动的都应该是package/apps下的文件，但是有一种情况是我担心他们会不小心改到系统代码并提交了。在合并之前查看都改了哪些文件。这样就能有效地防止误修改。
和老友沟通后，得出以下结论：
１.让他们添加本地忽略，忽略他们一定不会改的目录。
２.**在每次合并代码的时候，看一下每个分支都修改了哪些文件，有没有系统文件被修改**。
３.使用grrit。
４.先git fetch 再merge。
最后选择了第二种。因为git pull后，提交的时候会显示出这次合并代码都有哪些修改。
**22.Gitk 不错的查看Log的工具**
**23.Git遇上代码规范**
当Git遇上代码规范会有些事情要处理好，这里写先写个经典的：
1.检测自己要提交的代码中有无table
一般多人开发的程序中不准使用table代替空格的，以下是[Google的Java代码规范](http://google-styleguide.googlecode.com/svn/trunk/javaguide.html)([中文](http://www.cnblogs.com/lanxuezaipiao/p/3534447.html))中的截图：
![](https://img-blog.csdn.net/20140508234211828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2FuZ2Vhcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
所以在提交代码前自检一下自己代码中有有无table是很有必要的。
$ git diff | grep -P “\t” | grep “+”
如果tab已经存在，[这里](http://stackoverflow.com/questions/2316677/can-git-automatically-switch-between-spaces-and-tabs)有很好的解决方案，所以项目代码不要急于commit前期工作很重要。
**24.You are in the middle of an am session.**
$ git status
On branch hardware
You are in the middle of an am session.
  (fix conflicts and then run "git am --continue")
  (use "git am --skip" to skip this patch)
  (use "git am --abort" to restore the original branch)
nothing to commit, working directory clean
$ 
对于这个情况，我是在接手一个别人的仓库的时候遇到的，我目前没有更好的方法，也不知道做了哪些事情，我采取的是 重新clone一份代码。
## 25.Git镜像-简易仓库
    working tree/bare tree/remote tree。
    一般情况下没有这个需求，做镜像一般是中转服务器。
    $ git clone --mirror xxx.git
    执行以上命令将会clone一个裸版库，正常clone下来的是代码版本的库。前者不是用来编辑的，它本身是一个Git服务器上的仓库，可以供客户端进行pull和push。这其实也算是一个简易的Git服务器了。
在这个裸版库中，没有pull和push，有的只是
$ git remote update
会和远程库更新为远程仓库的内容。如果裸版仓库中比远程更新，那么会执行类似git reset --hard的指令强制更新为远程的。所以执行这个命令的时候要小心。因为它会洗掉本地与远端不同的内容「[1](http://www.douban.com/group/topic/35757419/)」。在bare tree也没有什么类似git pull的功能。
26.获取远程tags.
git fetch --tags
## 27.添加空目录
    $ find . -type d -empty -exec touch {}/.gitignore \;
    执行以上命令将会在空目录中添加一个.gitignore文件，这样就可以保证空目录不被忽略，且不影响原代码的结构。
    来自：[git 如何添加空目录](http://blog.chinaunix.net/uid-20782369-id-3424530.html)
## 28.新建立android系统源码仓库时要加--force选项以强制添加已经被忽略的文件
gitignore 新建立android系统源码仓库注意事项
第一次添加文件是一定要加上--force选项 添加所有文件(包括被忽略的文件)，因为子目录中会有.gitignore忽略一些对Android系统很必要的文件。如[external/dbus](https://github.com/android/platform_external_dbus)会忽略config.h，但这个文件很重要。没有它就编译不过。也有可能是dbus子项目的.gitignore写错了，也有可能是故意忽略。
总之强制添加config.h后就不会再被删除。
说Android只是一个例子，大的项目都需要考虑一下。
关于config.h为什么会被忽略，前边加有说明 对Android很必要，因为Android编译和普通linux系统上编译库的方法不一样。android不是通过./configure自动生成config.h。Android系统是要一个已经好的config.h通过Android.mk的方式进行编译。
## 29."get_tz_offset" is not exported by the Git module
执行git cvsimport -v test_project时出现的错误，升级为最新版本2.2.1没有问题了。
## 30.Git打包解包
免不了要做这些事情，直接将项目所在目录打包是最直观的，但是对于超级大的项目来说会很占空间。
而事实上只需要.git目录就可以将所有源码进行还原，以下是打包解包过程。
打包
tar cvzf xxx.tar.gz .git
解包
tar xvf xxx.tar.gz
mv xxx .git
git fsck --full #该命令的作用是从.git还原为最新源码
