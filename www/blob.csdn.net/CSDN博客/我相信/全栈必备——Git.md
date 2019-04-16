# 全栈必备——Git - 我相信...... - CSDN博客





2016年07月02日 12:02:58[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：22027
所属专栏：[全栈之路](https://blog.csdn.net/column/details/12818.html)









# 为什么使用Git

Git 是 Linus Torvalds 为了帮助管理 Linux 内核开发而开发的一个开放源码的版本控制软件。大神就是大神，在开发了Linux之后，Git 是又一抗鼎之作。这是唯一的理由么？

## Git在软件开发中位置——配置管理SCM

Software configuration management (SCM, or just plain CM) is an organizational framework — that is, a discipline — for managing the evolution of computer systems throughout all stages of systems development.

软件配置管理:通过执行版本控制、变更控制的规程，以及使用合适的配置管理软件，来保证所有配置资源的完整性和可跟踪性。配置管理是对工作成果的一种有效保护。没有软件配置管理，最大的麻烦是工作成果无法回溯。

### 配置管理的内容和目标

配置管理的内容：

一类是属于产品的组成部分，例如需求文档、设计文档、源代码、测试用例等等； 

另一类是在管理过程中产生的文档，例如各种计划、报告等

软件配置管理是在贯穿整个软件生命周期中建立和维护项目产品的完整性。它的基本目标包括： 

1. 软件配置管理的各项工作是有计划进行的。 

2. 被选择的项目产品得到识别，控制并且可以被相关人员获取。 

3. 已识别出的项目产品的更改得到控制。 

4. 使相关组别和个人及时了解软件基准的状态和内容。   
### 配置管理的主要任务

软件配置管理的主要任务也就归结为以下几条： 

（1）制定项目的配置计划； 

（2）对配置项进行标识； 

（3）对配置项进行版本控制； 

（4）对配置项进行变更控制； 

（5）定期进行配置审计； 

（6）向相关人员报告配置的状态。
## 版本控制

版本控制是软件配置管理的核心功能。所有位于配置资源库中的元素都应自动予以版本的标识，并保证版本命名的唯一性。版本在生成过程中，自动依照设定的使用模型自动分支、演进。

版本控制(Revision control)确保由不同人所编辑的同一档案都得到更新。

### 版本控制中的基本概念

1）签入，提交，检出 

2）冲突，解决，合并 

3）分支，版本 

4）锁定，hook    
### 常见的版本控制工具

作为一个老码农，枚举一下曾经使用过的版本控制工具。 

1. VSS： visual source safe, 微软的东东，97年写VC程序时使用，人多的时候性能较差，不知道现在的升级版怎样了 

2. clearcase： 99年开发Unix 上分布式式应用时使用，功能强大，不只限于版本控制，有钱的大团队才去用 

3. CVS: 02年在互联网热潮的时候使用，开源产品，当时“Copy-Modify-Merge”开发模型眼前一亮。 

4. SVN：曾经的挚爱，在曾工作的合资公司使用，权限管理和分支合并等方面做的很出色，并在多个公司推广使用。还记得TortoiseSVN么?那只可爱的小乌龟。 

5. perforce:是一款具有轻便快速的SCM工具、真正的客户端/服务器系统等特点的商业软件。高通内部使用的版本管理工具。确实不错。 

6. git：现在的最爱…… 

比较一下cvs,svn,和git：
![开源版本控制工具比较](https://img-blog.csdn.net/20160630161418228)

# Git 简要

GIT 是一款免费的、开源的、分布式的版本控制系统。每一个GIT克隆都是一个完整的文件库，含有全部历史记录和修订追踪能力。其最大特色就是“分支”及“合并”操作快速、简便。支持离线工作，GIT是整个项目范围的原子提交，而且GIT中的每个工作树都包含一个具有完整项目历史的仓库。

## 核心特点：
- Git 底层自行维护的存储文件系统：存储的是文件快照，即整个文件内容，并保存指向快照的索引    
- 去中心化的分布式控制   

## 优缺点：

优点：
- 适合分布式开发，强调个体。   
- 公共服务器压力和数据量都不会太大， 速度快、灵活。   
- 任意两个开发者之间可以很容易的解决冲突。      
- 离线工作。    

缺点：
- 学习周期相对而言比较长。
- 不符合常规思维。
- 代码保密性差，一旦开发者把整个库克隆下来就可以完全公开所有代码和版本信息。

# Git 原理

## Git的目录结构

不论通过git init 还是克隆下来的git 仓库，都有如下的目录结构： 
![这里写图片描述](https://img-blog.csdn.net/20160630161635135)
主要目录结构描述见下表：
|子目录名|简要描述|
|----|----|
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
branchesGit 项目分支信息，新版 Git 已经不再使用该目录config  Git 项目配置信息description Git 项目描述信息HEAD指向 Git 项目当前分支的头指针hooks默认的”hooks”脚本，被特定事件发生前后触发。info里面含一个 exclude 文件，指 Git 项目要忽略的文件。objectsGit 的数据对象，包括：commits, trees, blobs, tags。refs指向所有 Git 项目分支的指针
所有的分支指针保存在 .git/refs/heads 目录下，HEAD 在 .git/HEAD 目录下，标签在 .git/refs/tags 目录下。

## 快照

例如： 一个工程中有两个文件A和B， 有3个版本: 

V1.0 A和B，V1.5 A1和B，V2.0 A1和B1 

在Git 的实际存储中实际存了3个快照 4个文件。
Git对文件进行 SHA-1 计算作为文件的唯一ID，并校验了文件完整性。  

SHA-1 算法将文件中的内容通过计算生成一个 40 位的 Hash 值。SHA-1 算法的特点： 

由文件内容计算出的 Hash 值；Hash 值相同，文件内容相同。 
![这里写图片描述](https://img-blog.csdn.net/20160630161604385)
使用 SHA-1 的前两位创建了文件夹，剩下的 38 位为文件名。

这些 Obj 文件，其实分为四种类型，分别是 Blob、Tree、Commit、Tag。

### Blob

用来存放项目文件的内容，但是不包括文件的路径、名字、格式等其它描述信息。项目的任意文件的任意版本都是以 Blob 的形式存放的。

### Tree

Tree 用来表示目录。我们知道项目就是一个目录，目录中有文件、有子目录。因此 Tree 中有 Blob、子 Tree，且都是使用 SHA-1值引用的。这是与目录对应的。从顶层的 Tree 纵览整个树状的结构，叶子结点就是 Blob，表示文件的内容，非叶子结点表示项目的目录，顶层的 Tree 对象就代表了当前项目的快照。

### Commit

表示一次提交，有 Parent 字段，用来引用父提交。指向了一个顶层  Tree，表示了项目的快照，还有一些其它的信息，比如上一个提交 Committer、Author、Message 等信息。

## 存储区

Git中有4个类型的存储区：远程仓库，工作区，本地仓库和缓存区。

暂存区的好处：
- 为了能够实现部分提交
- 为了不在工作区创建状态文件、会污染工作区。
- 暂存区记录文件的修改时间等信息，提高文件比较的效率。 

暂存区是用来构建项目快照的区域。暂存区是一个文件，路径为： .Git/index

![暂存区的样子](https://img-blog.csdn.net/20160702111816870)

它是一个二进制文件，第四列是文件名，第三列是文件的冲突状态，第二列指的是文件的 Blob。

Commit 命令，将暂存区的内容永久保存到本地仓库。提交时 Git 会使用暂存区的这些信息生成 Tree 对象，也就是项目快照，永久保存到数据库中。

文件的状态可以分为两类。一类是暂存区与本地仓库比较得出的状态，另一类是工作区与暂存区比较得出的状态。为什么要分成两类的愿意也很简单，因为第一类状态在提交时，会直接写入本地仓库。而第二种则不会。一个文件可以同时拥有两种状态。

## 分支

分支的目的是让我们可以并行的进行开发。 .Git/HEAD 文件，它保存了当前的分支。

![分支简要](https://img-blog.csdn.net/20160702115311634)

分支指向了一次提交，也是 Git 中的分支为什么这么轻量的原因。   

因为分支就是指向了一个 Commit 的指针，当提交新的 Commit，这个分支的指向只需要跟着更新就可以了，而创建分支仅仅是创建一个指针。

# Git 必备技能

## 常见命令速查

![常见命令速查表](https://img-blog.csdn.net/20160702112906733)

## git add 和 git commit

Add 操作是将修改保存到暂存区，Commit 是将暂存区的内容永久保存到本地仓库。

每当将修改的文件加入到暂存区，Git 都会根据文件的内容计算出 SHA-1，并将内容转换成 Blob，写入数据库。然后使用 SHA-1 值更新该列表中的文件项。

在暂存区的文件列表中，每一个文件名，都会对应一个 SHA-1 值，用于指向文件的实际内容。最后提交的那一刻，Git 会将这个列表信息转换为项目的快照，也就是 Tree 对象。写入数据库，并再构建一个 Commit 对象，写入数据库。然后更新分支指向。

## 分支合并: merge 和rebase

冲突的状态
- DELETED_BY_THEM;
- DELETED_BY_US;
- BOTH_ADDED;
- BOTH_MODIFIED

遇到不可自动合并冲突时，Git 会将这些状态写入到暂存区。

### merge

在解决完冲突后，我们可以将修改的内容提交为一个新的提交。这就是 Merge。 

Merge 之后仍可以做出新的提交。

![merge](https://img-blog.csdn.net/20160702114105649)

### rebase

Rebase 会把从 Merge Base 以来的所有提交，以补丁的形式一个一个重新达到目标分支上。这使得目标分支合并该分支的时候会直接 Fast Forward，即不会产生任何冲突。

![rebase](https://img-blog.csdn.net/20160702114232488)

Rebase 主要在 .Git/Rebase-Merge 下生成了两个文件，分别为 Git-Rebase-todo 和 Done 文件，Git-Rebase-todo 存放了 Rebase 将要操作的 Commit。而 Done 存放正在操作或已经操作完毕的 Commit。

Rebase 的一个缺点，那就是修改了分支的历史提交。如果已经将分支推送到了远程仓库，会导致无法将修改后的分支推送上去，必须使用 -f 参数（Force）强行推送。

所以使用 Rebase 最好不要在公共分支上进行操作。  

### checkout

经常用来切换分支、或者切换到某一次提交。

Checkout 找到目标提交（Commit），目标提交中的快照也就是 Tree 对象就是我们要检出的项目版本。

Checkout 首先根据Tree生成暂存区的内容，再根据 Tree 与其包含的 Blob 转换成我们的项目文件。然后修改 HEAD 的指向，表示切换分支。

Checkout 并没有修改提交的历史记录。只是将对应版本的项目内容提取出来。

### revert

revert 实现了反向提交，就是旧版本添加了的内容，要在新版本中删除；旧版本中删除了的内容，要在新版本中添加。这在分支已经推送到远程仓库的情境下非常有用。

Revert 也不会修改历史提交记录，实际的操作相当于是检出目标提交的项目快照到工作区与暂存区，然后用一个新的提交完成版本的“回退”。

### reset

在当前分支进行版本的“回退”，Reset 是会修改历史提交记录的。

Reset 常用的选项有三个，分别是 —Soft, —Mixed, —Hard。他们的作用域依次增大。 

Soft 会仅仅修改分支指向。而不修改工作区与暂存区的内容， 

Mixed 比 Soft 的作用域多了一个 暂存区。实际上 Mixed 选项与 Soft 只差了一个 Add 操作。 

Hard 会比 Mixed作用域又多了一个工作区。
注意：在丢失后可以使用 Git Reset –Hard ORIG_HEAD 立即恢复，或者使用 reflog 命令查看之前分支的引用

### stash

有时，在一个分支上做了一些工作，修改了很多代码，而这时需要切换到另一个分支干点别的事。但又不想将只做了一半的工作提交。

Stash 将工作区与暂存区中的内容做一个提交，保存起来，然后使用Reset Hard 选项恢复工作区与暂存区内容。我们可以随时使用 Stash Apply 将修改应用回来。

Stash 实现思路将我们的修改提交到本地仓库，使用特殊的分支指针（.Git/refs/Stash）引用该提交，然后在恢复的时候，将该提交恢复即可。

# Git 典型实践

一个典型的git 并行开发的流程模型如下：

![git 典型实践](https://img-blog.csdn.net/20160702110937772)

## 主要分支

把origin/master作为主要分支，源码的HEAD总是表示production-ready(可随时部署)状态。 
![master和develop](https://img-blog.csdn.net/20160702111535880)
origin/develop上的代码是为下一次的代码发布准备的。每日构建也是基于此分支。 

当develop分支达到了一个稳定状态并准备发布时，所有的改变都要合并到master分支，并标上版本号。

## 辅助分支

### Feature branches

继承与合并都与develop 分支相关，用来开发新特性的(短期，远期都可以)。 
![feature和develop](https://img-blog.csdn.net/20160702113149638)
当要创建一个新特性时，从develop分支上再创建一个 Feature branch。 
`$ git checkout -b myfeature develop`

合并feature 到develop  
```
$ git checkout develop
Switched to branch 'develop'
$ git merge --no-ff myfeature
Updating ea1b82a..05e9557 (Summary of changes)
$ git branch -d myfeature
Deleted branch myfeature (was 05e9557).
$ git push origin develop
```

![merge without FF](https://img-blog.csdn.net/20160702115037711)

### Release branches

继承分支: develop 

合并分支：develop 和 master 

命名规范：release-*   
#### 创建一个release 分支

Release branch是通过develop分支而创建.   

```
$ git checkout -b release-1.2 develop    
Switched to a new branch "release-1.2"

$ ./bump-version.sh 1.2
Files modified successfully, version bumped to 1.2.

$ git commit -a -m "Bumped version number to 1.2"
[release-1.2 74d9424] Bumped version number to 1.2
1 files changed, 1 insertions(+), 1 deletions(-)
```

#### 完成一个release 分支

当release branch已经准备就绪，需要做几件事。   
- release分支被合并到master分支上(每一个提交到master上的commit都是一个新版 本，切记)。   
- master上的commit都要添加tag，方便将来查看和回滚。
- release上所做的修改必须合并到develop分支上，保证bug已被修补。 

前两个步骤：

```
$ git checkout master
Switched to branch 'master'
$ git merge --no-ff release-1.2
Merge made by recursive.
(Summary of changes)
$ git tag -a 1.2
```

为了把release上的改变保存到develop，需要合并到develop。

```
$ git checkout develop
Switched to branch 'develop'
$ git merge --no-ff release-1.2
Merge made by recursive.
(Summary of changes)
```

这个步骤可能会导致冲突，如果这样的话，解决冲突，然后再提交。 

最后，可以删除release 分支。

```
$ git branch -d release-1.2
Deleted branch release-1.2 (was ff452fe).
```

### Hotfix branches

继承分支: master 

合并分支：develop 和 master 

命名规范：hotfix-* 

运行过程中发现了bug，就必须快速解决，这时就可以创建一个Hotfix branch，解决完后合并到master分支上。好处是开发人员可以继续工作，有专人来负责搞定这个bug。
#### 创建hotfix分支

```
$ git checkout -b hotfix-1.2.1 master
Switched to a new branch "hotfix-1.2.1"
$ ./bump-version.sh 1.2.1
Files modified successfully, version bumped to 1.2.1.
$ git commit -a -m "Bumped version number to 1.2.1"
[hotfix-1.2.1 41e61bb] Bumped version number to 1.2.1
1 files changed, 1 insertions(+), 1 deletions(-)
```

#### fix bug, 解决问题

需要一次或几次commit

```
$ git commit -m "Fixed severe production problem"
[hotfix-1.2.1 abbe5d6] Fixed severe production problem
5 files changed, 32 insertions(+), 17 deletions(-)
```

### 完成Hotfix branch

当结束时，bugfix要被合并到master，同时也要合并到develop，保证下个版本发布时该bug已被修复。这跟release branch完成时一样。 

首先更新master和tag release  

```
$ git checkout master
Switched to branch 'master'
$ git merge --no-ff hotfix-1.2.1
Merge made by recursive.
(Summary of changes)
$ git tag -a 1.2.1
```

接下来与develop合并

```
$ git checkout develop
Switched to branch 'develop'
$ git merge --no-ff hotfix-1.2.1
Merge made by recursive.
(Summary of changes)
```

有一个例外，就是当一个release branch存在时，bugfix要被合并到release而不是develop，因为release最终会被合并到develop。

#### 最后移除branch

```bash
$ git branch -d hotfix-1.2.1 
Deleted branch hotfix-1.2.1 (was abbe5d6).
```

# 总结

了解Git 在软件工程及敏捷开发中的地位，明白git与其他版本控制工具之间的区别，掌握Git 工作的基本原理和必备操作，复杂问题可以查找git的相关命令，应用git开发的流程模型，让Git 成为我们的真正利器。

参考资料： 

1）[http://nvie.com/posts/a-successful-git-branching-model/](http://nvie.com/posts/a-successful-git-branching-model/)

2）[https://community.qingcloud.com/topic/457/%E6%8A%80%E6%9C%AF%E5%9F%B9%E8%AE%AD-git-%E4%BD%A0%E7%9C%9F%E7%9A%84%E4%BC%9A%E7%94%A8%E4%B9%88/2](https://community.qingcloud.com/topic/457/%E6%8A%80%E6%9C%AF%E5%9F%B9%E8%AE%AD-git-%E4%BD%A0%E7%9C%9F%E7%9A%84%E4%BC%9A%E7%94%A8%E4%B9%88/2)
















