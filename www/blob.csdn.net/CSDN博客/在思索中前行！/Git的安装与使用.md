# Git的安装与使用 - 在思索中前行！ - CSDN博客





2014年08月19日 21:35:42[_Tham](https://me.csdn.net/txl16211)阅读数：3973








[Git
 Tool1](http://blog.chinaunix.net/uid-25806493-id-3324285.html)


[http://www.cnblogs.com/chenkai/archive/2012/01/11/2319775.html](http://www.cnblogs.com/chenkai/archive/2012/01/11/2319775.html)




[Git Tool2](http://blog.chinaunix.net/uid-25806493-id-3324303.html)


[http://www.cnblogs.com/chenkai/archive/2012/01/13/2321722.html](http://www.cnblogs.com/chenkai/archive/2012/01/13/2321722.html)







**Win7上Git安装及配置过程**
|文档名称|Win7上Git安装及配置过程|
|----|----|
|创建时间|2012/8/20|
|修改时间|2012/8/20|
|创建人|Baifx|
|简介（收获）|1、在win7上安装msysgit步骤；2、在win7上安装TortoiseGit步骤；3、在VS2010中集成Git方法和步骤。|
|参考源|Git的配置与使用http://wenku.baidu.com/view/929d7b4e2e3f5727a5e962a8.html|

一、安装说明

1、Git在windows平台上安装说明。

         Git 是 Linux Torvalds 为了帮助管理 Linux 内核开发而开发的一个开放源码的版本控制软件。目前Git已经可以在windows下使用，主要方法有二：msysgit和Cygwin。Cygwin和Linux使用方法类似，Windows版本的Git提供了友好的GUI(图形界面)，安装后很快可以上手，此处我们主要讨论基于msysgit的Git安装和使用。

         TortoiseGit是TortoiseSVN的Git版本，TortoiseGit用于迁移TortoiseSVN到TortoiseGit。一直以来Git在Windows平台没有好用GUI客户端，现在TortoiseGit的出现给Windows开发者带来福音。**我们将在64位win7操作系统上安装Git，并使用GUI界面，则需同时安装msysGit和TortoiseGit。**

2、阅读TortoiseGit官方安装说明：

[http://code.google.com/p/tortoisegit/wiki/SetupHowTo](http://code.google.com/p/tortoisegit/wiki/SetupHowTo)

**System prerequisites**
- For the latest version of TortoiseGit Windows XP SP3 or newer is required.
- Admin privileges for the installation
- [msysGit](http://code.google.com/p/msysgit) is
 required by TortoiseGit- You do not need to download the whole msysGit development package, the ["Full
 installer for official Git for Windows"](http://code.google.com/p/msysgit/downloads/list?can=2&q=%22Full+installer+for+official+Git+for+Windows%22) download package is sufficient
- msysGit 1.7.10+ is recommended for TortoiseGit 1.7.9+ (msysGit 1.7.10 adds utf-8 support and is compatible to *nix git)
- minimum compatible version is 1.6.1 (for TortoiseGit < 1.7.9 you should use msysGit 1.7.6)


**Installation**

Just [download](http://code.google.com/p/tortoisegit/wiki/Download) the
 setup package for your system and install it. If you are running a 64 bit system, you do not need to download and install the 32 bit version: The 32 bit shell extension is included in the 64 bit installer since TortoiseGit 1.7.3.0.

**Windows 2000**

If you want to use TortoiseGit in a Win2K environment (only 1.6.5 and below support Win2K), please install [GDI+](http://www.microsoft.com/download/en/details.aspx?id=18909) before
 you install TortoiseGit. However, running these old versions is not recommended (no utf-8 and separate-git-dir support).

**Upgrade**

Before upgrading you should read the [ReleaseNotes](http://code.google.com/p/tortoisegit/wiki/ReleaseNotes).

Just [download](http://code.google.com/p/tortoisegit/wiki/Download) the
 setup package for your system and install it. The old version will be replaced automatically.

If you are upgrading from 1.7.3.0 or older and you have installed the 32-bit version on a 64-bit system you have to deinstall the 32-bit version first.

**Common problems (installer aborts with an error message)**

**"This installation package is not supported by this processor type. Contact your product vendor."**

This means you are trying to install the 64-bit version of TortoiseGit on a normal 32-bit operating system. You need to download and use the correct msi file for
 your OS. For normal 32-bit OS, make sure the msi filename does not have "64-bit" in it.

**"Please wait while the installer finishes determining your disk space requirements."**

Cleanup/empty the temp-directory (e.g. C:\Users\<your user>\AppData\Local\Temp, C:\User and Settings\<your user>\Local Settings\Temp, c:\Windows\Temp\).

由如上说明，我们寻找要下载的对应安装包，如下。



二、下载安装包

1、TortoiseGit下载地址：

[http://code.google.com/p/tortoisegit/downloads/list](http://code.google.com/p/tortoisegit/downloads/list)

本次下载版本——TortoiseGit-1.7.12.0-64bit.msi 

TortoiseGit 1.7.12.0 64bit  

x64 Featured 

2、msysgit下载地址：

[http://code.google.com/p/msysgit/downloads/list](http://code.google.com/p/msysgit/downloads/list)

本次下载版本——Git-1.7.11-preview20120710.exe 

Full installer for official Git for Windows 1.7.11  

Featured Beta 



三、安装过程

安装顺序：首先安装msysgit；然后安装TortoiseGit。

1、安装msysgit。

a、安装包下载完成后，双击进入安装界面，如下图：

![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448588GYQU.jpg)



b、两步next后选择安装目录，如下图：

![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448612JGYY.jpg)



c、next进入Git安装模块选择，默认，如下图：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448629K4tk.jpg)

d、next进入Git setup界面，“Select start menu
 folder”，默认，如下图：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448644MkSs.jpg)

e、next进入Git Setup界面，“Adjusting your PATH
 environment”，选择默认值“Use Git Bash only”，如下图所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448675558S.jpg)

f、next进入Git Setup界面，“Configuring the
 line ending conversions”，选择换行格式，选择“Checkout as-is, commit Unix-style line endings”，如下图所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_134544868979mf.jpg)

g、next进入安装界面，完成安装，如下图所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448703ptCm.jpg)

这个时候已经可以使用git了，打开Git
 Bash可以进入linux shell，可以使用git命令进行各种操作，由于大家都习惯使用图形界面的 TortoiseSVN，下面介绍使用 TortoiseSVN的类似软件 TortoiseGit，使用习惯相同,大家应该比较容易使用。

2、安装TortoiseGit。

a、双击安装程序，进入安装界面，如下如所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448720y5gE.jpg)

b、两步next进入“Choose SSH Client”选择界面，选择“OpenSSH，Git
 default SSH Client”，如下图所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448734Iw1N.jpg)

c、next进入“Custom Setup”界面，选择默认值，如下图所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448746it6o.jpg)

d、next，进入“Ready to Install”界面，选择“Install”按钮开始安装过程，完成安装。如下图所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448769uBXG.jpg)

e、至此，TortoiseGit安装完成。在桌面空白处点击右键，右键菜单中会加入TortoiseGit快捷键，如下图所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448849xWUP.jpg)

f、选择“Settings”，进入“Settings-TortoiseGit”界面，选择“General”选项卡，设置本机器的git路径，如下图所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448872uJHP.jpg)

g、同时选择“Network”选项卡，设置SSH路径。SSH默认在安装Git时就安装了，在如下图所示的路径中。如下图所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448884pA3L.jpg)

h、选择“Git”选项卡，设置用户名、邮箱和key。如下图所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448898GyQW.jpg)

注：如果暂时在本地使用就只需将用户名和邮箱添加，而“Signing key”会自动生成。

至此，TortoiseGit设置完成。



3、下载代码。

a、桌面空白处右键，选择git clone添加版本库地址URL和本地文件夹。如下图所示：



![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345448911Ik9q.jpg)

点击ok即可下载一份新版本库。

未完，待续。













四、在VS2010中集成Git。

1、要想在Visual Studio中直接使用Git（即将Git集成到VS中）需要安装插件Git Source Control Provider，下载地址如下：

[http://visualstudiogallery.msdn.microsoft.com/63a7e40d-4d71-4fbb-a23b-d262124b8f4c](http://visualstudiogallery.msdn.microsoft.com/63a7e40d-4d71-4fbb-a23b-d262124b8f4c)

也可通过Visual Studio2010来下载安装。方法：在vs菜单中点击“工具”的“扩展管理器”，进入“扩展管理器”界面。在右上角搜索框中直接输入“Git”搜索，第一项即是“Git Source Control Provider”，然后点击“下载”，下载完成后直接安装，安装后重启VS2010即可。如下图所示：

![](http://blog.chinaunix.net/attachment/201208/20/25806493_1345451138Cu1u.jpg)

2、安装过程。双击“GitSccProvider.vsix”，进入安装界面。如下图所示：

![](http://blog.chinaunix.net/attachment/201208/20/25806493_13454489299auj.jpg)



默认会勾选你所安装的Visual Studio的版本。点击“安装”按钮开始安装。安装完成后根据提示，关闭所有打开的Visual Studio实例，然后重新打开它们以使更改生效。

3、在Visual Studio实例中直接使用Git。







**Git命令行介绍和使用说明（持续更新）**
|文档名称|Git命令行介绍和使用说明会|
|----|----|
|创建时间|2012/8/23|
|修改时间|2012/8/23|
|创建人|Baifx|
|简介（收获）||

参见：《Git 中文简体教程》

一、

命令“git”或者“git help”查询常用命令

![](http://blog.chinaunix.net/attachment/201208/27/25806493_1346055603O5Oh.jpg)





【add】：

“git add”——不但是用来添加不在版本控制中的新文件，也用于添加已在版本控制中但是刚修改过的文件; 在这两种情况下, Git都会获得当前文件的快照并且把内容暂存(stage)到索引中，为下一次commit做好准备。**Git**跟踪的是内容不是文件。

【bisect】

【branch】：

“git branch”——会得到当前仓库中存在的所有分支列表。其中，“master”分支是Git系统默认创建的主分支。星号(“*”)标识了你当工作在哪个分支下。

“git branch -d branchname”——git branch -d只能删除那些已经被当前分支的合并的分支。

“git branch –D branchname”——git branch –D可以强制删除某个分支的话就用。



”git branch -人“——查看所有分支。



【checkout】：



“git checkout brancename”——切换到“brancename”分支。

【clone】：



注：**Clone**一个仓库。为了得一个项目的拷贝(copy),我们需要知道这个项目仓库的地址(Git URL). Git能在许多协议下使用，所以Git
 URL可能以ssh://, http(s)://, git://,或是只是以一个用户名（git 会认为这是一个ssh 地址）为前辍. 有些仓库可以通过不只一种协议来访问，例如，Git本身的源代码你既可以用 git:// 协议来访问：git
 clone git://git.kernel.org/pub/scm/git/git.git

也可以通过http 协议来访问:git clone http://www.kernel.org/pub/scm/git/git.git

git://协议较为快速和有效,但是有时必须使用http协议,比如你公司的防火墙阻止了你的非http访问请求.如果你执行了上面两行命令中的任意一个,你会看到一个新目录:
 'git',它包含所有的Git源代码和历史记录.

在默认情况下，Git会把"Git
 URL"里目录名的'.git'的后辍去掉,做为新克隆(clone)项目的目录名: (例如. *git clone*

*http://git.kernel.org/linux/kernel/git/torvalds/linux-2.6.git *会建立一个目录叫'linux-2.6')



“git clone –bare test test.git”——选项bare的意思就是用现有的被控制仓库创建一个GIT仓库。取代了创建目录和初始化两步，而且使得新生成的目录自身就是仓库目录，而这个仓库目录中是没有像工作目录中的源文件的，只有镜像文件（即跟原来的.git目录下的内容差不多）。这样的目录一般就是服务器存储的内容，可以称之为镜像目录。bare后的参数一是本地目录，参数二是远程（也可是本地）镜像目录，这样就可以是想将本地目录test推送到服务器目录。

--bare

Make a *bare* GIT repository. That is, instead of creating <directory> and placing the administrative files in <directory>/.git, make the<directory> itself the $GIT_DIR. This obviously implies
 the -n because there is nowhere to check out the working tree. Also the branch heads at the remote are copied directly to corresponding local branch heads, without mapping them to refs/remotes/origin/. When this option is used, neither remote-tracking branches
 nor the related configuration variables are created.



“git clone test.git test”——与“git clone --bare”的过程相反，就是利用远程镜像来恢复本地源文件。例子的意思就是从远程镜像test.git中克隆出源文件到本地目录test中。





【commit】：



“git commit”——把暂存区中记录的要提交的内容提交到Git的对象数据库。这会提示你输入本次修改的注释，完成后就会记录一个新的项目版本。



“git commit –a”这会自动把所有内容被修改的文件(不包括新创建的文件)都添加到索引中，并且同时把它们提交。

这里有一个关于写commit注释的技巧和大家分享:commit注释最好以一行短句子作为开头，来简要描述一下这次commit所作的修改(最好不要超过50个字符)；然后空一行再把详细的注释写清楚。这样就可以很方便的用工具把commit注释变成email通知，第一行作为标题，剩下的部分就作email的正文。



“git commit -m”——后跟参数表示要接提交内容说明的信息。



【config】：

“git config –username ”——

“git config --get remote.origin.url”——查看本地配置的远程仓库地址。

“git config --global core.editor "notepad"”——将Git默认的Vi编辑器换成记事本，如果想再改成Vim则使用下面命令行：

git config --global core.editor "\"C:/Program Files (x86)/GitExtensions/GitExtensions.exe\" fileeditor"



【diff】：



“git diff”——比较自己项目中任意两个版本的差异，或是用来查看别人提交进来的新分支。显示在当前的工作目录里的，没有staged(添加到索引中)，且在下次提交时不会被提交的修改。如有合并的文件有冲突，输入此命令就可以查看当前有哪些文件产生了冲突。

“git diff test”——这会显示你当前工作目录与另外一个叫'test'分支的差别。你也以加上路径限定符，来只比较某一个文件或目录。

“git diff master..test”——这条命令只显示两个分支间的差异，如果你想找出“git diff master...test”——这条命令显示‘master’,‘test’的共有父分支和'test'分支之间的差异，用3个‘.'来取代前面的两个'.' 。

“git diff --cached”——看看哪些文件将被提交(commit)。显示你当前的索引和上次提交间的差异；这些内容在不带"-a"参数运行 "git
 commit"命令时就会被提交。

“git diff HEAD”——上面这条命令会显示你工作目录与上次提交时之间的所有差别，这条命令所显示的内容都会在执行"git commit -a"命令时被提交。

“git diff HEAD -- ./lib”——上面这条命令会显示你当前工作目录下的lib目录与上次提交之间的差别(或者更准确的说是在当前分支)。

“git diff --stat”——不查看每个文件的详细差别，只是统计一下有哪些文件被改动，有多少行被改动。





【fetch】

【grep】

【init】：



“git init”——初始化一个新的仓库，将它置于Git的版本控制管理之下。



【log】：

“git log”——可以显示所有的提交。

$ git log test..master # commits reachable from master but not test

$ git log master..test # commits reachable from test but not master

$ git log master...test # commits reachable from either test or master, but not both

$ git log --since="2 weeks ago" # commits from the last 2 weeks

$ git log Makefile # commits that modify Makefile

$ git log fs/ # commits that modify any file under fs/

$ git log -S'foo()' # commits that add or remove any file data

# matching the string 'foo()'

$ git log --no-merges # dont show merge commits

当然你也可以组合上面的命令选项；下面的命令就是找出所有从"v2.5“开始在fs目录下的所有Makefile的修改。

$ git log v2.5.. Makefile fs/

Git会根据git log命令的参数，按时间顺序显示相关的提交(commit)。



“git log -p”——显示补丁(patchs)。

“git log --stat”——日志统计。如果用--stat选项使用“git log”，它会显示在每个提交(commit)中哪些文件被修改了, 这些文件分别添加或删除了多少行内容。



“git log --pretty”——格式化日志。你可以按你的要求来格式化日志输出。“—pretty”参数可以使用若干表现格式，如“oneline”：

$ git log --pretty=oneline

a6b444f570558a5f31ab508dc2a24dc34773825f dammit, this is the second time this has reverted

49d77f72783e4e9f12d1bbcacc45e7a15c800240 modified index to create refs/heads if it is not

9764edd90cf9a423c9698a2f1e814f16f0111238 Add diff-lcs dependency

e1ba1e3ca83d53a2f16b39c453fad33380f8d1cc Add dependency for Open4

0f87b4d9020fff756c18323106b3fd4e2f422135 merged recent changes: * accepts relative alt pat

f0ce7d5979dfb0f415799d086e14a8d2f9653300 updated the Manifest file

或者你也可以使用 'short' 格式:

$ git log --pretty=short

commit a6b444f570558a5f31ab508dc2a24dc34773825f

Author: Scott Chacon <schacon@gmail.com>

dammit, this is the second time this has reverted

commit 49d77f72783e4e9f12d1bbcacc45e7a15c800240

Author: Scott Chacon <schacon@gmail.com>

modified index to create refs/heads if it is not there

commit 9764edd90cf9a423c9698a2f1e814f16f0111238

Author: Hans Engel <engel@engel.uk.to>

Add diff-lcs dependency

你也可用‘medium','full','fuller','email' 或‘raw'. 如果这些格式不完全符合你的相求，你也可以用‘--pretty=format'参数(参见：git
 log)来创建你自己的"格式“.

$ git log --pretty=format:'%h was %an, %ar, message: %s'

a6b444f was Scott Chacon, 5 days ago, message: dammit, this is the second time this has re

49d77f7 was Scott Chacon, 8 days ago, message: modified index to create refs/heads if it i

9764edd was Hans Engel, 11 days ago, message: Add diff-lcs dependency

e1ba1e3 was Hans Engel, 11 days ago, message: Add dependency for Open4

0f87b4d was Scott Chacon, 12 days ago, message: merged recent changes:

另一个有趣的事是：你可以用'--graph'选项来可视化你的提交图(commit graph),就像下面这样:

$ git log --pretty=format:'%h : %s' --graph

* 2d3acf9 : ignore errors from SIGCHLD on trap

* 5e3ee11 : Merge branch 'master' of git://github.com/dustin/grit

|\

| * 420eac9 : Added a method for getting the current branch.

* | 30e367c : timeout code and tests

* | 5a09431 : add timeout protection to grit

* | e1193f8 : support for heads with slashes in them

|/

* d6016bc : require time for xmlschema

它会用ASCII字符来画出一个很漂亮的提交历史(commit history)线。



**日志排序**

你也可以把日志记录按一些不同的顺序来显示。注意，git日志从最近的提交(commit)开始，并且从这里开始向它们父分支回溯。然而git历史可能包括多个互不关联的开发线路，这样有时提交(commits)显示出来就有点杂乱。

如果你要指定一个特定的顺序，可以为git log命令添加顺序参数(ordering option).

按默认情况，提交(commits)会按逆时间(reverse chronological)顺序显示。

但是你也可以指定‘--topo-order'参数，这就会让提交(commits)按拓朴顺序来显示(就是子提交在它们的父提交前显示). 如果你用git
 log命令按拓朴顺序来显示git仓库的提交日志，你会看到“开发线"(development lines)都会集合在一起。

$ git log --pretty=format:'%h : %s' --topo-order --graph

* 4a904d7 : Merge branch 'idx2'

|\

| * dfeffce : merged in bryces changes and fixed some testing issues

| |\

| | * 23f4ecf : Clarify how to get a full count out of Repo#commits

| | * 9d6d250 : Appropriate time-zone test fix from halorgium

| | |\

| | | * cec36f7 : Fix the to_hash test to run in US/Pacific time

| | * | decfe7b : fixed manifest and grit.rb to make correct gemspec

| | * | cd27d57 : added lib/grit/commit_stats.rb to the big list o' files

| | * | 823a9d9 : cleared out errors by adding in Grit::Git#run method

| | * | 4eb3bf0 : resolved merge conflicts, hopefully amicably

| | |\ \

| | | * | d065e76 : empty commit to push project to runcoderun

| | | * | 3fa3284 : whitespace

| | | * | d01cffd : whitespace

| | | * | 7c74272 : oops, update version here too

| | | * | 13f8cc3 : push 0.8.3

| | | * | 06bae5a : capture stderr and log it if debug is true when running commands

| | | * | 0b5bedf : update history

| | | * | d40e1f0 : some docs

| | | * | ef8a23c : update gemspec to include the newly added files to manifest

| | | * | 15dd347 : add missing files to manifest; add grit test

| | | * | 3dabb6a : allow sending debug messages to a user defined logger if provided; tes

| | | * | eac1c37 : pull out the date in this assertion and compare as xmlschemaw, to avoi

| | | * | 0a7d387 : Removed debug print.

| | | * | 4d6b69c : Fixed to close opened file description.

你也可以用'--date-order'参数，这样显示提交日志的顺序主要按提交日期来排序。这个参数和'--topo-order'有一点像，没有父分支会在它们的子分支前显示，但是其它的东东还是按交时间来排序显示。你会看到"开发线"(development
 lines)没有集合一起，它们会像并行开发(parallel development)一样跳来跳去的：

$ git log --pretty=format:'%h : %s' --date-order --graph

* 4a904d7 : Merge branch 'idx2'

|\

* | 81a3e0d : updated packfile code to recognize index v2

| * dfeffce : merged in bryces changes and fixed some testing issues

| |\

| * | c615d80 : fixed a log issue

|/ /

| * 23f4ecf : Clarify how to get a full count out of Repo#commits

| * 9d6d250 : Appropriate time-zone test fix from halorgium

| |\

| * | decfe7b : fixed manifest and grit.rb to make correct gemspec

| * | cd27d57 : added lib/grit/commit_stats.rb to the big list o' file

| * | 823a9d9 : cleared out errors by adding in Grit::Git#run method

| * | 4eb3bf0 : resolved merge conflicts, hopefully amicably

| |\ \

| * | | ba23640 : Fix CommitDb errors in test (was this the right fix?

| * | | 4d8873e : test_commit no longer fails if you're not in PDT

| * | | b3285ad : Use the appropriate method to find a first occurrenc

| * | | 44dda6c : more cleanly accept separate options for initializin

| * | | 839ba9f : needed to be able to ask Repo.new to work with a bar

| | * | d065e76 : empty commit to push project to runcoderun

* | | | 791ec6b : updated grit gemspec

* | | | 756a947 : including code from github updates

| | * | 3fa3284 : whitespace

| | * | d01cffd : whitespace

| * | | a0e4a3d : updated grit gemspec

| * | | 7569d0d : including code from github updates

最后，你也可以用 ‘--reverse'参数来逆向显示所有日志。



【merge】：

“git merge branchname”——这个命令把分支"branchname"合并到了当前分支里面。如有冲突(冲突--同一个文件在远程分支和本地分支里按不同的方式被修改了）；那么命令的执行输出就像下面一样：

$ git merge next

100% (4/4) done

Auto-merged file.txt

CONFLICT (content): Merge conflict in file.txt

Automatic merge failed; fix conflicts and then commit the result.

在有问题的文件上会有冲突标记，在你手动解决完冲突后就可以把此文件添加到索引(index)中去，用“git commit”命令来提交，就像平时修改了一个文件一样。

如果你用gitk来查看commit的结果，你会看到它有两个父分支：一个指向当前的分支，另外一个指向刚才合并进来的分支。

**解决合并中的冲突**

如果执行自动合并没有成功的话，git会在索引和工作树里设置一个特殊的状态，提示你如何解决合并中出现的冲突。

有冲突(conflicts)的文件会保存在索引中，除非你解决了问题并且更新了索引，否则执行git commit都会失败：

$ git commit

file.txt: needs merge

如果执行git status会显示这些文件没有合并(unmerged)，这些有冲突的文件里面会添加像下面的冲突标识符：

<<<<<<< HEAD:file.txt

Hello world

=======

Goodbye

>>>>>>> 77976da35a11db4580b80ae27e8d65caf5208086:file.txt

你所需要的做是就是编辑解决冲突，（接着把冲突标识符删掉），再执行下面的命令:

$ git add file.txt

$ git commit

注意：提交注释里已经有一些关于合并的信息了，通常是用这些默认信息，但是你可以添加一些你想要的注释。

上面这些就是你要做一个简单合并所要知道的，但是git提供更多的一些信息来帮助解决冲突。

**快速向前合并**

还有一种需要特殊对待的情况，在前面没有提到。通常，一个合并会产生一个合并提交(commit), 把两个父分支里的每一行内容都合并进来。

但是，如果当前的分支和另一个分支没有内容上的差异，就是说当前分支的每一个提交(commit)都已经存在另一个分支里了，git 就会执行一个“快速向前"(fast
 forward)操作；git不创建任何新的提交(commit)，只是将当前分支指向合并进来的分支。



【mv】

【pull】

【push】

【rebase】

【remote】

【reset】：



“git reset --hard HEAD”——撤销当前合并。如果你觉得你合并后的状态是一团乱麻，想把当前的修改都放弃，你可以用下面的命令回到合并之前的状态。

“git reset --hard ORIG_HEAD”——撤销已提交（合并后）代码的合并。但是这条命令在某些情况会很危险，如果你把一个已经被另一个分支合并的分支给删了，那么以后在合并相关的分支时会出错。

【rm】

【show】

【status】：



“git status”——获得当前项目的一个状况。

使用“git status”命令是查看索引内容的最简单办法. 你运行 git status命令, 就可以看到: 哪些文件被暂存了(就是在你的Git索引中), 哪些文件被修改了但是没有暂存, 还有哪些文件没有被跟踪(untracked).

【tag】

“git tag”——不带任何参数创建一个标签(tag)指定某个提交(commit)。

$ git tag stable-1 1b2e1d63ff

这样，我们可以用stable-1 作为提交(commit) "1b2e1d63ff" 的代称(refer)。前面这样创建的是一个“轻量级标签"，这种分支通常是从来不移动的。如果你想为一个标签(tag)添加注释，或是为它添加一个签名(sign
 it cryptographically), 那么我们就需要创建一个 ”标签对象"。



【gitk】：

执行了gitk后会有一个很漂亮的图形的显示项目的历史。






# [Git的安装与使用](http://www.cnblogs.com/Bonker/p/3441781.html)





 1，下载git [https://code.google.com/p/msysgit/downloads/list](https://code.google.com/p/msysgit/downloads/list)

 2，安装git ，我们选择命令行形式，这样无论在window下还是在linux下 都可以。 

![git命令行](http://images.cnblogs.com/cnblogs_com/Bonker/536068/o_QQ%e5%9b%be%e7%89%8720131125160400.jpg)

3，一直next下去就安装完成了。

4，git与bitbucket关联，产生一个公钥（类似与seesion，下回我们git 项目时就不用登录了。）。 在开始->所有程序->git->Git Bash  打开gitbash。

会显示如下界面。 

![git配置公钥](http://images.cnblogs.com/cnblogs_com/Bonker/536068/o_1.jpg)

5，输入 ssh-keygen ，按enter，一直按enter 直到出现下面

![git输入](http://images.cnblogs.com/cnblogs_com/Bonker/536068/o_2.jpg)

6，上图中会提示公钥生成在 C:\Documents and Settings\Administrator\.ssh 目录中，找到目录 ，里面有id_rsa.pub文件，用记事本打开，然后把里面的东西 全选复制。

7，登录bitbucket页面，点击头像，选择帐号管理。然后点选如图SSH密钥，然后有个他添加Key，把刚才复制的key复制进去。添加就完成了。

![他](http://images.cnblogs.com/cnblogs_com/Bonker/536068/o_3.jpg)

8，git命令行入门开始了。打开cmd，输入git。然后cd进入本机项目要存放的目录（也就是你想把项目放到你本机那个位置）。

9，然后输入git clone命令（把项目copy下来）， git clone git@bitbucket.org:bonker/test.git  这样就把git@bitbucket.org:bonker/test.git 的项目复制到本机了。

10，更新文件，我们如果更改了其中一个文件的东西，先输入 git add  xxx.txt  告诉git 我们要知提交这个文件.这是会进入vim对更改写注释的界面。vim使用很简单，按a是在一行后面append追加。i是inert在光标的地方添加。添加完成输入-s就是保存，-q是离开。一般我们都是-sq保存后离开。  

11，如果提交所有的文件git commit -a。

12.上面所有的保存都只是保存在本机。也就是对.git文件夹的修改。还没有把数据提交到远程服务器。

13，git remote add hellogit git@bitbucket.org:bonker/hello.git,定义一个远程服务器是hellogit。

14，这是使用git remote就可以看到我们目前的远程服务器有哪些。

15，F:\Workplace\hellogit>git push hellogit master:master 把我们本机的master主分支的hellogit 提交到服务器master的hellogit目录下



git其他命令
- 创建新分支并命名，此处我们创建名为 new_stuff 的分支 


|`1`|```git branch new_stuff```|
|----|----|






- 移到新分支上


|`1`|```git checkout new_stuff```|
|----|----|




- 开始你的工作并保存结果

各种工具.....

- 添加所改动的文件以便提交


|`1`|```git add .```|
|----|----|




- 提交改动


|`1`|```git commit -m``````"made some changes"```|
|----|----|




- 回到 master 主分支


|`1`|```git checkout master```|
|----|----|




- 合并到主分支








|`1`|```git merge new_stuff```|
|----|----|
[http://www.cnblogs.com/likebeta/archive/2013/01/22/2871573.html](http://www.cnblogs.com/likebeta/archive/2013/01/22/2871573.html)




Git常用操作命令收集：

1) 远程仓库相关命令

检出仓库：$ git clone git://github.com/jquery/jquery.git

查看远程仓库：$ git remote -v

添加远程仓库：$ git remote add [name] [url]

删除远程仓库：$ git remote rm [name]

修改远程仓库：$ git remote set-url --push[name][newUrl]

拉取远程仓库：$ git pull [remoteName] [localBranchName]

推送远程仓库：$ git push [remoteName] [localBranchName]



2）分支(branch)操作相关命令

查看本地分支：$ git branch

查看远程分支：$ git branch -r

创建本地分支：$ git branch [name] ----注意新分支创建后不会自动切换为当前分支

切换分支：$ git checkout [name]

创建新分支并立即切换到新分支：$ git checkout -b [name]

删除分支：$ git branch -d [name] ---- -d选项只能删除已经参与了合并的分支，对于未有合并的分支是无法删除的。如果想强制删除一个分支，可以使用-D选项

合并分支：$ git merge [name] ----将名称为[name]的分支与当前分支合并

创建远程分支(本地分支push到远程)：$ git push origin [name]

删除远程分支：$ git push origin :heads/[name]


我从master分支创建了一个issue5560分支，做了一些修改后，使用git push origin master提交，但是显示的结果却是'Everything up-to-date'，发生问题的原因是git
 push origin master 在没有track远程分支的本地分支中默认提交的master分支，因为master分支默认指向了origin master 分支，这里要使用git push origin issue5560：master 就可以把issue5560推送到远程的master分支了。
    如果想把本地的某个分支test提交到远程仓库，并作为远程仓库的master分支，或者作为另外一个名叫test的分支，那么可以这么做。

$ git push origin test:master         // 提交本地test分支作为远程的master分支 //好像只写这一句，远程的github就会自动创建一个test分支

$ git push origin test:test              // 提交本地test分支作为远程的test分支

如果想删除远程的分支呢？类似于上面，如果:左边的分支为空，那么将删除:右边的远程的分支。

$ git push origin :test              // 刚提交到远程的test将被删除，但是本地还会保存的，不用担心
3）版本(tag)操作相关命令

查看版本：$ git tag

创建版本：$ git tag [name]

删除版本：$ git tag -d [name]

查看远程版本：$ git tag -r

创建远程版本(本地版本push到远程)：$ git push origin [name]

删除远程版本：$ git push origin :refs/tags/[name]



4) 子模块(submodule)相关操作命令

添加子模块：$ git submodule add [url] [path]

如：$ git submodule add git://github.com/soberh/ui-libs.git src/main/webapp/ui-libs

初始化子模块：$ git submodule init ----只在首次检出仓库时运行一次就行

更新子模块：$ git submodule update ----每次更新或切换分支后都需要运行一下

删除子模块：（分4步走哦）

1)$ git rm --cached [path]

2) 编辑“.gitmodules”文件，将子模块的相关配置节点删除掉

3) 编辑“.git/config”文件，将子模块的相关配置节点删除掉

4) 手动删除子模块残留的目录



5）忽略一些文件、文件夹不提交

在仓库根目录下创建名称为“.gitignore”的文件，写入不需要的文件夹名或文件，每个元素占一行即可，如

target

bin

*.db



2. git pull：相当于是从远程获取最新版本并merge到本地

git pull origin master
















