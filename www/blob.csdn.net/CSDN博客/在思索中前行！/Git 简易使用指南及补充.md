# Git  简易使用指南及补充 - 在思索中前行！ - CSDN博客





2014年10月19日 20:02:19[_Tham](https://me.csdn.net/txl16211)阅读数：905








## Git最简易的使用指南



助你开始使用 [git](http://www.nowamagic.net/academy/tag/git) 的简易指南，木有高深内容，;)

#### 安装
- [下载 git OSX 版](http://code.google.com/p/git-osx-installer/downloads/list?can=3)
- [下载 git Windows 版](http://code.google.com/p/msysgit/downloads/list?can=3)
- [下载 git Linux 版](http://book.git-scm.com/2_installing_git.html)

#### 创建新仓库


创建新文件夹，打开，然后执行：



|`1`|```git init```|
|----|----|





以创建新的 git 仓库。

#### 检出仓库


执行如下命令以创建一个本地仓库的克隆版本：



|`1`|```git clone /path/to/repository```|
|----|----|





如果是远端服务器上的仓库，你的命令会是这个样子：



|`1`|```git clone username@host:/path/to/repository```|
|----|----|




#### 工作流


你的本地仓库由 git 维护的三棵“树”组成。第一个是你的 工作目录，它持有实际文件；第二个是 缓存区（Index），它像个缓存区域，临时保存你的改动；最后是 HEAD，指向你最近一次提交后的结果。

![](http://www.nowamagic.net/librarys/images/201401/2014_01_06_05.png)

#### 添加与提交


你可以计划改动（把它们添加到缓存区），使用如下命令：



|`1`|```git add <filename>```|
|----|----|


|`2`|```git add *```|
|----|----|





这是 git 基本工作流程的第一步；使用如下命令以实际提交改动：



|`1`|```git commit -m````"代码提交信息"`|
|----|----|





现在，你的改动已经提交到了 HEAD，但是还没到你的远端仓库。

#### 推送改动


你的改动现在已经在本地仓库的 HEAD 中了。执行如下命令以将这些改动提交到远端仓库：



|`1`|```git push origin master```|
|----|----|





可以把 master 换成你想要推送的任何分支。


如果你还没有克隆现有[仓库](http://www.nowamagic.net/academy/tag/%E4%BB%93%E5%BA%93)，并欲将你的仓库连接到某个远程服务器，你可以使用如下命令添加：



|`1`|```git remote add origin <server>```|
|----|----|





如此你就能够将你的改动推送到所添加的服务器上去了。

#### 分支


分支是用来将特性开发绝缘开来的。在你创建仓库的时候，master 是“默认的”。在其他分支上进行开发，完成后再将它们合并到主分支上。

![](http://www.nowamagic.net/librarys/images/201401/2014_01_06_06.png)


创建一个叫做“feature_x”的分支，并切换过去：



|`1`|```git checkout -b feature_x```|
|----|----|





切换回主分支：



|`1`|```git checkout master```|
|----|----|





再把新建的分支删掉：



|`1`|```git branch -d feature_x```|
|----|----|





除非你将分支推送到远端仓库，不然该分支就是 不为他人所见的



|`1`|```git push origin <branch>```|
|----|----|




#### 更新与合并


要更新你的本地仓库至最新改动，执行：



|`1`|```git pull```|
|----|----|





以在你的工作目录中 获取（fetch） 并 合并（merge） 远端的改动。


要合并其他分支到你的当前分支（例如 master），执行：



|`1`|```git merge <branch>```|
|----|----|





两种情况下，git 都会尝试去自动合并改动。不幸的是，自动合并并非次次都能成功，并可能导致 冲突（conflicts）。 这时候就需要你修改这些文件来人肉合并这些 冲突（conflicts） 了。改完之后，你需要执行如下命令以将它们标记为合并成功：



|`1`|```git add <filename>```|
|----|----|





在合并改动之前，也可以使用如下命令查看：



|`1`|```git diff <source_branch> <target_branch>```|
|----|----|




#### 标签


在软件发布时创建标签，是被推荐的。这是个旧有概念，在 SVN 中也有。可以执行如下命令以创建一个叫做 1.0.0 的标签：



|`1`|```git tag 1.0.0 1b2e1d63ff```|
|----|----|





1b2e1d63ff 是你想要标记的提交 ID 的前 10 位字符。使用如下命令获取提交 ID：



|`1`|`git ``log`|
|----|----|





你也可以用该提交 ID 的少一些的前几位，只要它是唯一的。

#### 替换本地改动


假如你做错事（自然，这是不可能的），你可以使用如下命令替换掉本地改动：



|`1`|```git checkout -- <filename>```|
|----|----|





此命令会使用 HEAD 中的最新内容替换掉你的工作目录中的文件。已添加到缓存区的改动，以及新文件，都不受影响。


假如你想要丢弃你所有的本地改动与提交，可以到服务器上获取最新的版本并将你本地主分支指向到它：



|`1`|```git fetch origin```|
|----|----|


|`2`|```git reset --hard origin/master```|
|----|----|




#### 有用的贴士


内建的图形化 git：



|`1`|`gitk`|
|----|----|





彩色的 git 输出：



|`1`|```git config color.ui````true`|
|----|----|





显示历史记录时，只显示一行注释信息：



|`1`|```git config format.pretty oneline```|
|----|----|





交互地添加文件至缓存区：



|`1`|```git add -i```|
|----|----|




## 补充一些Git的基本用法



某天，微博上有人发了这么一张图求职：

![](http://www.nowamagic.net/librarys/images/201401/2014_01_09_01.jpg)


后面还得到了阿里巴巴招聘部等众人转发，不知道最后是否求职成功，但是我们可以看看在这张图里包含了什么信息。


首先是 git log 打印出日志，介绍了他做的事情。然后 git push 项目给淘宝……


嘛，这也是一个不错的求职方式。


活用好 [Git](http://www.nowamagic.net/academy/tag/Git)，你也可以装一装。这里再补充一下


1. 初始化 git 仓库



|`1`|```mkdir project # 创建项目目录```|
|----|----|


|`2`|```cd project # 进入到项目目录```|
|----|----|


|`3`|```git init # 初始化 git 仓库。此命令会在当前目录新建一个 .git 目录，用于存储 git 仓库的相关信息```|
|----|----|





2. 初始化提交



|`1`|```touch README```|
|----|----|


|`2`|```git add . # 将当前目录添加到 git 仓库中， 使用 git add -A 则是添加所有改动的文档```|
|----|----|


|`3`|```git commit -m``````"Initial commit"```|
|----|----|


|`4`|```git remote add origin git@github.com:nowamagic/repo.git # 设置仓库```|
|----|----|





3. 修补提交（修补最近一次的提交而不创建新的提交）



|`1`|```git commit --amend -m``````"commit message."```|
|----|----|





4. 提交冲突时可以合并后再推送



|`1`|```git pull # 获取远程版本库提交与本地提交进行合并```|
|----|----|


|`2`|```git push # 提交```|
|----|----|





5. 使用别人的[仓库](http://www.nowamagic.net/academy/tag/%E4%BB%93%E5%BA%93)



|`1`|```git clone http:``````//path/to/git.git # clone 的内容会放在当前目录下的新目录```|
|----|----|





6. 将代码从本地回传到仓库



|`1`|```git push -u origin master```|
|----|----|





7. 使用 git status 查看文件状态



|`1`|```git status```|
|----|----|





8. 查看提交日志



|`1`|`git ``log````# 查看提交信息```|
|----|----|


|`2`|`git ``log````--pretty=oneline # 以整洁的单行形式显示提交信息```|
|----|----|


|`3`|`git ``log````--stat # 查看提交信息及更新的文件```|
|----|----|





9. Git 分支



|`1`|```git branch # 查看分支```|
|----|----|


|`2`|```git branch 6.x-1.x # 添加分支 6.x-1.x```|
|----|----|


|`3`|```git branch checkout master # 切换到主分支```|
|----|----|


|`4`|```git branch -d 6.x-1.x # 删除分支 6.x-1.x```|
|----|----|


|`5`|```git push origin :branchname # 删除远端分支```|
|----|----|





10. Git 标签



[view
 source](http://www.nowamagic.net/academy/detail/48160304#viewSource)

[print](http://www.nowamagic.net/academy/detail/48160304#printSource)[?](http://www.nowamagic.net/academy/detail/48160304#about)



|`1`|```git tag # 查看分支```|
|----|----|


|`2`|```git tag 6.x-1.0 # 添加标签 6.x-1.0```|
|----|----|


|`3`|```git show 6.x-1.0 # 查看标签 6.x-1.0 的信息```|
|----|----|


|`4`|```git tag -a 6.x-1.0 965e066 # 为之前提交的信息记录 965e066 加上标签```|
|----|----|


|`5`|```git push --tags # 提交时带上标签信息```|
|----|----|


|`6`|```git push origin :/refs/tags/tagname # 删除远端标签```|
|----|----|





11. 从 git 仓库中导出项目



|`1`|```git archive --format tar --output /path/to/file.tar master # 将 master 以 tar 格式打包到指定文件```|
|----|----|




## 了解并创建Git仓库，以及添加文件到仓库



先来理解一个概念，repository。你可以简单将它理解成一个目录，这个目录里面的所有文件都可以被Git管理起来，每个文件的修改、删除，Git都能跟踪，以便任何时刻都可以追踪历史，或者在将来某个时刻可以“还原”。这个目录通常称为[仓库](http://www.nowamagic.net/academy/tag/%E4%BB%93%E5%BA%93)，或者叫版本库。


通过 git init 命令把这个目录变成 Git 可以管理的仓库：



|`1`|```git init```|
|----|----|





瞬间Git就把仓库建好了，而且告诉你是一个空的仓库（empty Git repository），细心的读者可以发现当前目录下多了一个.git的目录，这个目录是Git来跟踪管理版本库的，没事千万不要手动修改这个目录里面的文件，不然改乱了，就把Git仓库给破坏了。


也不一定必须在空目录下创建Git仓库，选择一个已经有东西的目录也是可以的。不过，不建议你使用自己正在开发的公司项目来学习Git，否则造成的一切后果概不负责。

[初始化](http://www.nowamagic.net/academy/tag/%E5%88%9D%E5%A7%8B%E5%8C%96)后，在当前目录下会出现一个名为
 .git 的目录，所有 Git 需要的数据和资源都存放在这个目录中。不过目前，仅仅是按照既有的结构框架初始化好了里边所有的文件和目录，但我们还没有开始跟踪管理项目中的任何一个文件。

#### 仓库的文件结构


我们看看这个目录下的文件结构：



|`01`|```$ ls```|
|----|----|


|`02`|`HEAD`|
|----|----|


|`03`|`config`|
|----|----|


|`04`|`description`|
|----|----|


|`05`|`index`|
|----|----|


|`06`|`branches/`|
|----|----|


|`07`|`hooks/`|
|----|----|


|`08`|`info/`|
|----|----|


|`09`|`objects/`|
|----|----|


|`10`|`refs/`|
|----|----|





该目录下有可能还有其他文件，但这是一个全新的 git init 生成的库，所以默认情况下这些就是你能看到的结构。新版本的 Git 不再使用 branches 目录，description 文件仅供 GitWeb 程序使用，所以不用关心这些内容。config 文件包含了项目特有的配置选项，info 目录保存了一份不希望在 .gitignore 文件中管理的忽略模式 (ignored patterns) 的全局可执行文件。hooks 目录保存了客户端或服务端钩子脚本。


另外还有四个重要的文件或目录：HEAD 及 index 文件，objects 及 refs 目录。这些是 Git 的核心部分。
- 
objects 目录存储所有数据内容
- 
refs 目录存储指向数据 (分支) 的提交对象的指针
- 
HEAD 文件指向当前分支
- 
index 文件保存了暂存区域信息

#### 添加文件到仓库


首先这里再明确一下，所有的版本控制系统，其实只能跟踪文本文件的改动，比如TXT文件，网页，所有的程序代码等等，Git也不例外。版本控制系统可以告诉你每次的改动，比如在第5行加了一个单词“Linux”，在第8行删了一个单词“Windows”。而图片、视频这些二进制文件，虽然也能由版本控制系统管理，但没法跟踪文件的变化，只能把二进制文件每次改动串起来，也就是只知道图片从100KB改成了120KB，但到底改了啥，版本控制系统不知道，也没法知道。


现在我们编写一个readme.txt文件，内容如下：



|`1`|```Git is a version control````system``.`|
|----|----|


|`2`|```Git is````free``software.`|
|----|----|





一定要放到NowaMagicOnGit目录下（子目录也行），因为这是一个Git仓库，放到其他地方Git再厉害也找不到这个文件。


和把大象放到冰箱需要3步相比，把一个文件放到Git仓库只需要两步。


第一步，用命令git add告诉Git，把文件添加到仓库：



|`1`|```$ git add readme.txt```|
|----|----|





执行上面的命令，没有任何显示，这就对了，Unix的哲学是“没有消息就是好消息”，说明添加成功。


第二步，用命令git commit告诉Git，把文件提交到仓库：



|`1`|```$ git commit -m``````"wrote a readme file"```|
|----|----|


|`2`|```[master (root-commit) cb926e7] wrote a readme file```|
|----|----|


|`3`|` ````1 file changed, 2 insertions(+)```|
|----|----|


|`4`|` ````create mode 100644 readme.txt```|
|----|----|





简单解释一下git commit命令，-m后面输入的是本次提交的说明，可以输入任意内容，当然最好是有意义的，这样你就能从历史记录里方便地找到改动记录。


嫌麻烦不想输入-m "xxx"行不行？确实有办法可以这么干，但是强烈不建议你这么干，因为输入说明对自己对别人阅读都很重要。实在不想输入说明的童鞋请自行Google，我不告诉你这个参数。


git commit命令执行成功后会告诉你，1个文件被改动（我们新添加的readme.txt文件），插入了两行内容（readme.txt有两行内容）。


为什么Git添加文件需要add，commit一共两步呢？因为commit可以一次提交很多文件，所以你可以多次add不同的文件，比如：



|`1`|```$ git add file1.txt```|
|----|----|


|`2`|```$ git add file2.txt```|
|----|----|


|`3`|```$ git add file3.txt```|
|----|----|


|`4`|```$ git commit -m``````"add 3 files."```|
|----|----|




#### 小结


现在总结一下今天学的两点内容：


1. 初始化一个Git仓库，使用git init命令。并且了解到一个仓库的文件组成结构。


2. 添加文件到Git仓库，分两步：
- 
第一步，使用命令git add ，注意，可反复多次使用，添加多个文件；
- 
第二步，使用命令git commit，完成。







