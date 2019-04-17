# Git 历险记 - 在思索中前行！ - CSDN博客





2014年10月19日 20:14:33[_Tham](https://me.csdn.net/txl16211)阅读数：913








## Git历险记（一）


作为分布式版本控制系统的重要代表——Git已经为越来越多的人所认识，它相对于我们熟悉的CVS、SVN甚至同时分布式控制系统的Mercurial，有哪些优势和不足呢。这次InfoQ中文站有幸邀请到《Git Community Book》的译者刘辉，在InfoQ开辟《Git历险记》专栏，分享他使用Git的经验，以及他对Git的看法。

[Git](http://git-scm.com/)是Linus.Torvald为了管理[Linux内核](http://git.kernel.org/?p=linux/kernel/git/torvalds/linux-2.6.git;a=summary)发起并开发的一个开源分布式版本控件系统（DVCS）。从2002年起，Linux
 内核一直使用[BitKeeper](http://www.bitkeeper.com/)来进行版本管理，但是在2005年BitKeeper和Linux 内核开源社区的合作关系结束，BitKeeper再也不能免费使用了，这迫使Linus决定开发一个开源界自已的版本控制系统。

传统的SVN、CVS 等版本控制系统，只有一个仓库（repository)，用户必须要连上这个仓库才能开始提交；而Git之类的分布式版本控制系统（当然也还包括 BitKeeper、[Mercurial](http://mercurial.selenic.com/)等等），它的每个工作目录都包含一个完整的仓库，它们可以支持离线工作，先把工作提交到本地仓库后再提交上远程的服务器上的仓库里。分布式的处理也让开发更为便捷，开发人员可以很方便的在本地创建分支来进行日常开发，每个人的本地仓库都是平等且独立，不会因为你的本地提交而直接影响别人。

老实说，Git的速度是我用的版本控制系统中最快的（SVN Mercurial Git）。我这里说的速度，包括本地提交(commit)、本地签出(checkout)、提交到远程仓库(git push)和从远程仓库获取（git fetch ，git pull）；它的本地操作速度和本地文件系统在一个级别，远程仓库的操作速度和SFTP文件传输在一个级别。这当然和Git的内部实现机制有关，这里就不多展开了，有兴趣的朋友可以看一下这里：[Git
 is the next Unix](http://www.advogato.org/person/apenwarr/diary/371.html)。

我们在学一门新的语言时，往往是从一个“hello world” 程序开始的，那么Git历程也就从一个“hello Git”开始吧。

在这里假设各位同学的电脑都装好了Git，如果没有装好，可以先看一下这里（[安装Git](http://gitbook.liuhui998.com/2_1.html)）。当然，后面的章节我会专门讲安装可能会碰到的问题。

我们首先打开Git的命令行：windows下是点击“Git Bash 快捷方式”；Linux或是Unix like平台的话就直接打开命令行界面就可以了。

备注：$符号后面的字符串代表的是命令行输入；命令行输入后的以#开始的黑体字符串代表注释；其它的部分则是命令行输出。

我们先用建一个仓库吧：
$mkdir testGit #建立仓库目录
$cd testGit #进入仓库目录
$git init #这会在当前的目录下建一个仓库
Initialized empty Git repository in e:/doc/Git/test/testGit/.git/ 

好的，前面的三行命令就建立了一个本地的Git仓库。这个仓库现在是一个空的仓库。

我们在命令行下执行：
$ git status #查看当前仓库的状态
# On branch master (在master分支上) 
# 
# Initial commit 
# 
nothing to commit (create/copy files and use "git add" to track) 
(现在没有任何台被提交的文件，复制或创建新的文件，再用”git add” 命令添加到暂存区中) 
$ git log #查看当前仓库的历史日志
fatal: bad default revision 'HEAD'
(由于仓库里没有任提交在里面，所以它会报这个错。BTW: 这种提示是不是有点不友好呀：) ) 

现在就让我们在这个仓库里添加点内容吧。
$ echo “hello Git” > readme.txt #建立一个含有 hello Git 的文本文件
$ git add readme.txt #将readme.txt添加到[暂存区](http://www.google.com/url?q=http%3A%2F%2Fprogit.org%2Fbook%2Fzh%2Fch2-2.html&sa=D&sntz=1&usg=AFQjCNEYEQ9ebcoED7X5W658v903gnBUaQ)中
$ git status #查看当前仓库的状态 
# On branch master 
# 
# Initial commit 
# 
# Changes to be committed:(暂存里下次将被提交的修改) 
# (use "git rm --cached <file>..." to unstage) 
# 
# new file: readme.txt 
# 

好的，文件即然被暂存到暂存区中，我们现在就可以把它提交到仓库里面去：）
$ git commit -m "project init" #将刚才的修改提交到本地仓库中 
[master (root-commit) 8223db3] project init 
1 files changed, 1 insertions(+), 0 deletions(-) 
create mode 100644 readme.txt 
$ git status 
# On branch master 
nothing to commit (working directory clean) 
(现在这个工作目录里没有什么要提交的东东，它是整洁的) 

现在你执行一下git log 命令就会看到刚才的提交记录
$ git log 
commit 8223db3b064a9826375041c8fea020cb2e3b17d1 
Author: liuhui998 <liuhui998@gmail.com> 
Date: Sat Jan 1 18:12:38 2011 +0800 
project init 

“8223db3b064a9826375041c8fea020cb2e3b17d1”这一串字符就是我们这次创建的提交的名字。看起来是不是很熟，如果经常用电驴的朋友就会发现它就是和电驴里内容标识符一样，都是SHA1串。Git通过对提交内容进行 SHA1 Hash运算，得到它们的SHA1串值，作为每个提交的唯一标识。根据一般的密码学原理来说，如果两个提交的内容不相同，那么它们的名字就不会相同；反之，如果它们的名字相同，就意味着它们的内容也相同。

现在我想改一下仓库里文件的内容，现提交到仓库中去
$ echo "Git is Cool" >> readme.txt #在文件的最后添加一行 
$ git status #查看当前仓库的状态 
# On branch master 
# Changed but not updated: (修改了，但是还没有暂存的内容) 
# (use "git add <file>..." to update what will be committed) 
# (use "git checkout -- <file>..." to discard changes in working directory) 
# 
# modified: readme.txt 
# 
no changes added to commit (use "git add" and/or "git commit -a") 

(没有修改可以被提交，使用 “git add” 命令添加文件到暂存区，或是使用“git commit -a” 命令强制提交当前目录下的所有文件)

OK，即然我们修改了仓库里被提交的文件，那么我想看一下我们

到底改了哪些地方，再决定是否提交。
$ git diff #查看仓库里未暂存内容和仓库已提交内容的差异 
diff --git a/readme.txt b/readme.txt 
index 7b5bbd9..49ec0d6 100644 
--- a/readme.txt 
+++ b/readme.txt 
@@ -1 +1,2 @@ 
hello Git 
+Git is Cool 

很好，正如我们所愿，我们只是在readme.txt的最后一行添加了一行“Git is Cool”。

好的，我们现在再把 readme.txt放到暂存区里：
$ git add readme.txt 
我们现在看一下仓库的状态：
$ git status 
# On branch master 
# Changes to be committed: 
# (use "git reset HEAD <file>..." to unstage) 
# 
# modified: readme.txt 
# 

可以提交了：
$ git commit -m "Git is Cool" 
[master 45ff891] Git is Cool 
1 files changed, 1 insertions(+), 0 deletions(-) 
(一个文件被修改，一行插入，零行删除) 

再看一下新的日志：
$ git log 
commit 45ff89198f08365bff32364034aed98126009e44 
Author: liuhui998 <liuhui998@gmail.com> 
Date: Sat Jan 1 18:17:07 2011 +0800 
Git is Cool 
commit 8223db3b064a9826375041c8fea020cb2e3b17d1 
Author: liuhui998 <liuhui998@gmail.com> 
Date: Sat Jan 1 18:12:38 2011 +0800 
project init 

“45ff89198f08365bff32364034aed98126009e44” 这个就是我们刚才提交修改时创建的提交。

大家这么一路看过来，是不是有点糊涂了。不过没有关系，如果你的电脑装了Git，那么你把上面的这些命令全部执行一下遍就会对它有感性的认识了。

下面的的章节，我会讲一下如何在windows和Linux安装配置Git，以及需要注意的问题：）

参考

[GitCommunityBook](http://book.git-scm.com/)[中文版](http://gitbook.liuhui998.com/)

[ProGit](http://progit.org/book/)[中文版](http://progit.chunzi.me/zh)

[Git is the next Unix](http://www.advogato.org/person/apenwarr/diary/371.html)




## Git历险记（二）——Git的安装和配置


上回[Git历险记（一）](http://www.infoq.com/cn/news/2011/01/git-adventures-1)讲了一个 “hello Git” 的小故事。有的同学可能是玩过了其它分布式版本控制系统（DVCS），看完之后就触类旁通对Git就了然于胸了；也有的同学可能还如我当初入手Git一样，对它还是摸不着头脑。

从这一篇开始，我就将比较“啰嗦”的和大家一起从零开始经历Git使用的每一步，当然对我而言这也是一个重新认识Git的过程。

使用Git的第一步肯定是安装Git，因为在多数平台上Git是没有预装的。我平时主要的工作环境是windows和Linux（ubuntu），我想看这篇文章的同学多半也是在这两个平台下工作；下面我讲一下如何在这两个平台下安装和配置Git。

BTW：如果是苹果平台的用户的安装可以参看一下这里([1](http://gitbook.liuhui998.com/2_1.html)，[2](http://progit.chunzi.me/zh/ch1-4.html))，配置和命令行的使用与windows、Linux（*nix）平台差别不大。

### Linux (*nix) 平台

Linus开发Git的最初目的就是为了开发Linux内核服务的，自然它对Linux的平台支持也是最棒的。在Linux下安装Git大约有几种方法：

从源代码开始(这种方法也适合于多数*nix平台)

从[Git官网](http://git-scm.com/)的[下载页面](http://git-scm.com/download)下载它最新稳定版的[源代码](http://kernel.org/pub/software/scm/git/git-1.7.3.5.tar.bz2)，就可以从源代码开始编译、安装：
$ wget http://kernel.org/pub/software/scm/git/git-1.7.3.5.tar.bz2
$ tar -xjvf git-1.7.3.5.tar.bz2
$ cd git-1.7.3.5
$ make prefix=/usr all ;# prefix设置你的Git安装目录
$ sudo make prefix=/usr install ;# 以root权限运行

为了编译Git的源代码，我们还需要一些库: [expat](http://expat.sourceforge.net/)、[curl](http://curl.linux-mirror.org/)、 [zlib](http://www.zlib.net/) 和 [openssl](http://www.openssl.org/)；
 除了expat 外，其它的库可能在你的机器上都安装了。

使用安装包管理器（apt 或 yum）

在 fedora 等系统下用[yum](http://fedoraproject.org/wiki/Tools/yum) ：
$ yum install git-core 
在debian, ubuntu等系统下用[apt](http://en.wikipedia.org/wiki/Advanced_Packaging_Tool) ：
$ apt-get install git-core
有时候，你系统里的安装包管理器出现了问题，或是要安装Git的机器不能上网、没有编译器的话，你可以从下面的站点去下载 “.deb” 或 “.rpm”的安装包：
- [RPM Packages](http://kernel.org/pub/software/scm/git/RPMS/)
- [Stable Debs](http://www.backports.org/debian/pool/main/g/git-core/)

### Windows平台

windows平台有两个模拟*nix like运行环境的工具：[cygwin](http://www.cygwin.com/)，[msys](http://www.mingw.org/wiki/MSYS)；Git在[cygwin](http://www.cygwin.com/)，[msys](http://www.mingw.org/wiki/MSYS)下都有相应的移植版本。我个人觉得msys平台下的[msysGit](http://code.google.com/p/msysgit/)最好用，现在我在windows下也是用的这个版本。

很多同学可能要问，现在windows下有那多Git用户，为什么Git不直接出一个windows native版。俺当年翻看了一下[Git的源代码](http://www.kernel.org/pub/software/scm/git/git-0.01.tar.bz2)，它里面使用了大量的*nix平台的native
 api，而这些api在windows下是没有的，所以必须要用cygwin、msys这样的一个中间层来满足软件移植的要求。

下面我“啰嗦”一下如何在windows下安装msysGit。

### 下载

到它的[下载页面](http://code.google.com/p/msysgit/downloads/list)去下载一个最新的完整安装包，笔者在撰写本文时下载的是[这个](http://msysgit.googlecode.com/files/Git-1.7.3.1-preview20101002.exe)。

### 安装

安装的过程没有什么好说的，一般是开始安装后，一路的点击“下一步”。由于windows平台的换行符（CRLF）和Linux(*nix)平台的换行符（LF）不同，那么在windows下开发其它平台软件的朋友有一个地方要注意（见下图)：

![](http://www.infoq.com/resource/news/2011/01/git-adventures-install-config/zh/resources/image1.JPG)

在这里一最好选“Checkout as-is, commit as-is”这个选项，这样，Git就不会修改你代码的换行符风格。

以前有个朋友因为选错了这个选项，以致他在windows平台下的一签出（checkout）其它平台的代码，就会显示”已修改“（modified），不过后来可能msysGit也认识到这个问题了，就把默认选项改成了这个选项。

BTW: 其实前面两项也是有用的，如果对windows和Linux(*nix)平台[如何处理换行符](http://en.wikipedia.org/wiki/Newline)很熟悉的话，也可以尝试一下前面两个选项：）

## 配置Git

在Linux下和windows下配置Git的方法差不多，只是在Linux下，可以在命令行里直接使用[git
 config](http://www.kernel.org/pub/software/scm/git/docs/git-config.html)进行配置, 而在windows下则要先打开“Git Bash”，进入msysGit命令行界面，再用[git
 config](http://www.kernel.org/pub/software/scm/git/docs/git-config.html)命令进行相应的配置操作。

好了，前面安装好了Git，现在我们开始配置：

第一个需要配置的就是用户的用户名和email，因为这些内容会出现在你的每一个提交（commit）里面的，像下面这样：
$ git log #我们用git log查看当前仓库的提交（commit）日志
commit 71948005382ff8e02dd8d5e8d2b4834428eece24
Author: author <author@corpmail.com>
Date: Thu Jan 20 12:58:05 2011 +0800
Project init

下面的这两行命令就是设置用户名和email：
$ git config --global user.name author #将用户名设为author
$ git config --global user.email author@corpmail.com #将用户邮箱设为author@corpmail.com

Git的配置信息分为全局和项目两种，上面命令中带了“--global"参数，这就意味是在进行全局配置，它会影响本机上的每个一个Git项目。

大家看到，上面我们用的是@corpmail（公司邮箱）；但是有时候我们可能也参与了一些开源项目，那么就需要新的用户名和自己的私人邮箱，Git 可以为每个项目设定不同的配置信息。

在命令行环境，进入Git项目所在目录，执行下面的命令：
$ git config　user.name nickname#将用户名设为nickname
$ git config　user.email nickname@gmail.com #将用户邮箱设为nickname@gmail.com

Git的设计哲学和Linux（*nix）一样，尽量的使用[“文本化”（Textuality）](http://www.faqs.org/docs/artu/textualitychapter.html)；它里面尽量用文本化的形式存储信息，对于配置信息也更是如此，用户的这些配置信息全部是存储在文本文件中。Git的全局配置文件是存放在"~/.gitconfig"（用户目录下的.gitconfig）文件中：

我们用cat、head命令查看全局配置信息文件，并假设相关配置信息存储在文件的前3行（当然也有可能不在前3行，这里只是为了方便表示）
$ cat ~/.gitconfig | head -3 
[user]
name = author
email = author@corpmail.com

而项目配置文件是存放在Git项目所在目录的".git/config"文件中，这里也像上面一样用cat、head命令查看一下：
$ cat .git/config | head -3
[user]
name = nickname
email = nickname@gmail.com

如果大家对于Git熟悉后，可以直修改”~/.gitconfig”,”.git/config”这两个文件进行配置。

Git里还有很多可以配置的地方，大家可以参考一下[git
 config](http://www.kernel.org/pub/software/scm/git/docs/git-config.html) 和 [定制git](http://gitbook.liuhui998.com/5_7.html)。

这一篇写起来有点平淡无奇，但这是一个Git用户迈出的第一步。后面我还会有一系列的文章出来，都是我个人使用过程中的感悟。

有朋友问我：“为什么把文章叫作：‘Git历险记’”。这是因为在使用Git的历程中，我碰到过N多的问题；同时也觉得它有点小复杂。但是当这些问题解开后，就有时不得不赞叹它设计的巧妙之处。

如果大家对于我的文章有什么问题和建议，欢迎给我写邮件：![](http://www.infoq.com/resource/news/2011/01/git-adventures-install-config/zh/resources/image2.JPG)

之前我建立了一个 [git中文用户组](https://groups.google.com/group/git123) ，如果大家在使用Git的过程中碰到什么麻烦事，欢迎你在这个用户组里提问。

参考资料：
- [GitCommunityBook](http://book.git-scm.com/)[中文版](http://gitbook.liuhui998.com/)
- [ProGit](http://progit.chunzi.me/zh)[中文版](http://progit.chunzi.me/zh)
- [git config](http://www.kernel.org/pub/software/scm/git/docs/git-config.html)










