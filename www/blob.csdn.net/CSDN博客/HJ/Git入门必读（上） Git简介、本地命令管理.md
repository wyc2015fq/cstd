# Git入门必读（上）  Git简介、本地命令管理 - HJ - CSDN博客
2018年10月28日 18:22:37[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：62
一、Git简介
Git是目前世界上最先进的分布式版本控制系统。所谓的分布式版本控制系统，就是存在这么一个软件，不但能自动帮我记录每次文件的改动，还可以让同事协作编辑，这样就不用自己管理一堆类似的文件了，也不用把文件传来传去。如果想查看某次改动，只需要在软件里瞄一眼就可以了，这样非常方便；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181028170418954.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==,size_27,color_FFFFFF,t_70)
git安装（Mac下）
安装homebrew，然后通过homebrew安装Git；
二、Git本地命令管理
1、创建版本库
版本库又名仓库，英文名repository，可以简单理解成一个目录，这个目录里面的所有文件都可以被Git管理起来，每个文件的修改、删除，Git都能跟踪，以便任何时刻都可以追踪历史，或者在将来某个时刻可以“还原”；
```
$mkdir  learngit      ---创建名字为"learngit"的文件
$cd learngit          ---进入该文件
$pwd                  ---查看该文件所在目录
$gid init             ---初始化这个Git仓库
```
然后将文件添加到版本库。需要注意的是，所有的版本控制系统，其实只能跟踪文本文件的改动，版本控制系统可以告诉你每次的改动；
2、更新Git仓库文件
如果此时我想添加一个readme.tet文件，更新到Git仓库去管理，在Mac下操作如下：
```
$vim readme.txt
 ---编辑添加如下内容
    This is the first line in readme.txt
 ---然后更新
 $git add readme.txt
 ---然后告诉Git，把文件提交到仓库
 $git commit -m "v1.0"
```
git commit 命令中 -m后面输入的是本次提交的说明，可以输入任意内容，当然为了方便以后从历史记录里找到改动记录，输入一些有意义的内容，比如"v1.0"代表版本1；
需要注意的是，Git添加文件需要add，commit两步；commit可以一次提交很多文件，所以可以多次add不同的文件；
提交之后，如果需要查看具体修改的内容，可以输入
```
$git status        ---查看Git仓库下修改记录
```
注意，这个修改记录会显示你的提交说明，也就是前面所说的"v1.0"，方便查看；
另外，如果下次你再次打开命令行，不知道上次怎么修改的readme.txt，这个时候需要用
```
$git diff       ---对比查看当前内容与上次内容不同之处
```
3、版本回退
如果我现在不想要当前的版本内容，因为觉得修改不恰当，我需要要前一个或者前几个的版本内容，那么就可以做如下操作：
```
$git log  --pretty=online   ---查看命令的历史记录
  $git reset --hard HEAD^     ---回到前一个版本
```
这里需要说明的是，在git log后面加上的–pretty=oneline是为可以把所有命令的历史记录信息简略表示，每条记录显示一行，如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181028172916491.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==,size_27,color_FFFFFF,t_70)
而第二行git reset -hard HEAD^ 中的HEAD^表示的是当前指针HEAD回退一步，也可以用HEAD~1来替代（这样可以方便处理当需要回退很靠前的版本的情况）；
事实上Git管理是这样的，每提交一个新版本，Git就会把它们自动串成一条时间线；然后会有一个叫做HEAD的指针，指向你当前所在的版本；因此，可以这么说，Git的版本回退速度非常快，当你回退版本的时候，Git仅仅是把HEAD的指向更改而已；
那么如果我现在已经跳到前一个版本了，我觉得还是不行，想要跳回去，怎么办呢？
```
$git reset --hard 1094a
```
这个1094a是后一个版本的一个ID号（暂时这样称呼吧），那么这个版本号其实是可以通过git reflor查看的，这个命令是用来记录你的每一次你命令；
4、暂存区和工作区
工作区：在你的电脑里能够看到的目录，比如说learngit文件夹就是一个工作区；
版本库：工作区里有一个隐藏目录.git，这个不算工作区，而是Git的版本库；Git版本库里存了很多东西，其中最重要的就是称为stage的暂存区，还有Git为我们自动创建的第一个分支master，以及指向master的一个指针叫HEAD；
如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181028174206229.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==,size_27,color_FFFFFF,t_70)
前面我们提到的把文件往Git版本库里添加的时候，是分两步执行的：
```
第一步是用git add把文件添加进去，实际上就是把文件修改添加到暂存区；
第二部是用git commit提交更改，实际上就是把暂存区的所有内容提交到当前分支；
```
因为我们创建Git版本库时，Git自动为我们创建了唯一一个master分支，所以，现在git commit就是往master分支上提交更改；
我们可以简单理解为，需要提交的文件修改统统放到暂存区，然后，一次性提交暂存区的所有修改；
5、管理修改
为什么Git比其他版本控制系统设计得优秀，因为Git跟踪并管理的是修改，而非文件；比如你新增了一行，这就是一个修改，删除了一行，这也是一个修改；
你可以通过 git diff来查看修改；如果我们需要撤销修改，可以使用如下命令行：
```
$git checkout -- readme.txt      ---把readme.txt文件在工作区的修改全部撤销
```
这里有两种情况：
```
一种是readme.txt自修改后还没有被放到暂存区，现在撤销修改就回到和版本库一模一样的状态；
一种是readme.txt已经添加到暂存区后，又作了修改，现在撤销修改就回到添加到暂存区后的状态；
```
总之，就是让这个文件回到最近一次git commit或git add时的状态；
这个时候，相对应的就会有如下几个处理方法；
```
① 当你改乱了工作区某个文件的内容，想直接丢弃工作区的修改时，用命令git checkout -- file；
② 当你不但改乱了工作区某个文件的内容，还添加到了暂存区时，想丢弃修改，分两步，第一步用命令git reset HEAD <file>，就回到了①，第二步按照①操作；
③ 已经提交了不合适的修改到版本库时，想要撤销本次提交，参考如上的"版本回退"，但是前提是没有推送到远程库；
```
6、删除文件
操作如下：
```
$rm readme.txt
```
这个时候，Git知道你删除了文件，因此，工作区和版本库就不一致了，git status命令会立刻告诉你哪些文件被删除了；
当下，你就会有两个选择：
1）一个是确实要从版本库中删除该文件，那么就用git rm删掉，并且git commit；
2）一个是删错了，此时可以用git checkout“一键还原”；其实是因为此时版本库里还有，这个命令是用版本库里的版本替换工作区的版本，因此无论工作区是修改还是删除，都可以还原；
