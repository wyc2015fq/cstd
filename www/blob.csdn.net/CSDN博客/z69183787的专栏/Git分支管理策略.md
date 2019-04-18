# Git分支管理策略 - z69183787的专栏 - CSDN博客
2016年09月08日 15:09:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：375
个人分类：[版本管理-Git](https://blog.csdn.net/z69183787/article/category/2857957)
[Vincent Driessen](http://nvie.com/)提出了一个分支管理的[策略](http://nvie.com/posts/a-successful-git-branching-model/)，我觉得非常值得借鉴。它可以使得版本库的演进保持简洁，主干清晰，各个分支各司其职、井井有条。理论上，这些策略对所有的版本管理系统都适用，Git只是用来举例而已。如果你不熟悉Git，跳过举例部分就可以了。
一、主分支Master
首先，代码库应该有一个、且仅有一个主分支。所有提供给用户使用的正式版本，都在这个主分支上发布。
![](http://image.beekka.com/blog/201207/bg2012070503.png)
Git主分支的名字，默认叫做Master。它是自动建立的，版本库初始化以后，默认就是在主分支在进行开发。
二、开发分支Develop
主分支只用来分布重大版本，日常开发应该在另一条分支上完成。我们把开发用的分支，叫做Develop。
![](http://image.beekka.com/blog/201207/bg2012070504.png)
这个分支可以用来生成代码的最新隔夜版本（nightly）。如果想正式对外发布，就在Master分支上，对Develop分支进行"合并"（merge）。
Git创建Develop分支的命令：
> 
　　git checkout -b develop master
将Develop分支发布到Master分支的命令：
> 
　　# 切换到Master分支
　　git checkout master
　　# 对Develop分支进行合并
　　git merge --no-ff develop
这里稍微解释一下，上一条命令的--no-ff参数是什么意思。默认情况下，Git执行"快进式合并"（fast-farward merge），会直接将Master分支指向Develop分支。
![](http://image.beekka.com/blog/201207/bg2012070505.png)
使用--no-ff参数后，会执行正常合并，在Master分支上生成一个新节点。为了保证版本演进的清晰，我们希望采用这种做法。关于合并的更多解释，请参考Benjamin Sandofsky的[《Understanding
 the Git Workflow》](http://sandofsky.com/blog/git-workflow.html)。
![](http://image.beekka.com/blog/201207/bg2012070506.png)
三、临时性分支
前面讲到版本库的两条主要分支：Master和Develop。前者用于正式发布，后者用于日常开发。其实，常设分支只需要这两条就够了，不需要其他了。
但是，除了常设分支以外，还有一些临时性分支，用于应对一些特定目的的版本开发。临时性分支主要有三种：
> 
　　* 功能（feature）分支
　　* 预发布（release）分支
　　* 修补bug（fixbug）分支
这三种分支都属于临时性需要，使用完以后，应该删除，使得代码库的常设分支始终只有Master和Develop。
四、 功能分支
接下来，一个个来看这三种"临时性分支"。
第一种是功能分支，它是为了开发某种特定功能，从Develop分支上面分出来的。开发完成后，要再并入Develop。
![](http://image.beekka.com/blog/201207/bg2012070507.png)
功能分支的名字，可以采用feature-*的形式命名。
创建一个功能分支：
> 
　　git checkout -b feature-x develop
开发完成后，将功能分支合并到develop分支：
> 
　　git checkout develop
　　git merge --no-ff feature-x
删除feature分支：
> 
　　git branch -d feature-x
五、预发布分支
第二种是预发布分支，它是指发布正式版本之前（即合并到Master分支之前），我们可能需要有一个预发布的版本进行测试。
预发布分支是从Develop分支上面分出来的，预发布结束以后，必须合并进Develop和Master分支。它的命名，可以采用release-*的形式。
创建一个预发布分支：
> 
　　git checkout -b release-1.2 develop
确认没有问题后，合并到master分支：
> 
　　git checkout master
　　git merge --no-ff release-1.2
　　# 对合并生成的新节点，做一个标签
　　git tag -a 1.2
再合并到develop分支：
> 
　　git checkout develop
　　git merge --no-ff release-1.2
最后，删除预发布分支：
> 
　　git branch -d release-1.2
六、修补bug分支
最后一种是修补bug分支。软件正式发布以后，难免会出现bug。这时就需要创建一个分支，进行bug修补。
修补bug分支是从Master分支上面分出来的。修补结束以后，再合并进Master和Develop分支。它的命名，可以采用fixbug-*的形式。
![](http://image.beekka.com/blog/201207/bg2012070508.png)
创建一个修补bug分支：
> 
　　git checkout -b fixbug-0.1 master
修补结束后，合并到master分支：
> 
　　git checkout master
　　git merge --no-ff fixbug-0.1
　　git tag -a 0.1.1
再合并到develop分支：
> 
　　git checkout develop
　　git merge --no-ff fixbug-0.1
最后，删除"修补bug分支"：
> 
　　git branch -d fixbug-0.1
（完）
本文介绍一种使用Git进行源代码管理的分支模型，着重于如何使用Git更好的管理我们的源代码。
我假定您对Git有一定了解，会使用基本的Git命令进行一些简单的源代码管理工作。这不是一篇Git使用教程。
文章的主要思想源自以下链接：
[http://nvie.com/posts/a-successful-git-branching-model/](http://nvie.com/posts/a-successful-git-branching-model/)
根据自己的使用情况进行了补充。
我们知道Git是一个不需要中心服务器就能工作的源代码管理系统；但我仍然建议你至少保持一个逻辑上的中心服务器来存放你的长期分支（后文我们会说到什么是“长期分支”，什么是“临时分支”）。我说“逻辑上”的中心服务器，意思是你不必搭建一个真实的服务器——当你只是在实现一个小型应用时，可能开发人员只有你一个人，这个时候你完全可以将长期分支就放在你的项目目录里。
首先看一张图：
![](http://pic002.cnblogs.com/images/2012/453229/2012112819382145.jpg)
这张图上有这么几个分支：master，develop，feather，hotfix，release。它们之间存在着branch和merge的关系。我们从master和develop开始。
![image](http://images.cnblogs.com/cnblogs_com/byeyear/201211/201211282044353638.png)
master和develop分支都是长期分支，它们存在于整个项目存续期内。
**master分支是整个项目的主分支。**
所有其他分支都直接或间接源自master。master分支是可直接用于产品发布的代码。
**develop分支反映最新的开发进程。**
develop中的代码总是可以完整build的。当develop中的代码进入稳定状态（修复了绝大多数bug）准备release时，所有develop中的更改将通过release branch最终merge到master。
除master和develop以外的分支都是临时分支。当这些临时分支完成其使命，就可以删除它们。我们先看feather分支。
**feather分支用于某个新feather的开发，源自develop，并最终merge到develop。**
feather分支最终的结局要么合并到develop branch，要么被抛弃。feather分支用如下命令创建：
# git checkout –b my feather develop
完成后将这个feather合并到develop：
# git checkout develop
# git merge --no-ff myfeather
# git branch –d myfeather
# git push origin develop
合并时--no-ff选项避免fast forward。使用该选项和不使用该选项得到的分支路线图分别如下：
![image](http://images.cnblogs.com/cnblogs_com/byeyear/201211/201211282044378820.png)
**release分支用于准备新版本的发布。源自develop，merge到develop和master。**
release分支仅修复小的bug，完成准备版本号，build date等工作。而develop分支可以同时开始新feather的开发。该分支上修复的bug需要merge到develop，并在该分支完成时merge到master。此时需要给master打上tag，标记这个新的release。
创建release branch：
# git checkout –b release-x.y develop
完成release branch
# git checkout master
# git merge --no-ff release-x.y
# git tag –a x.y
# git checkout develop
# git merge --no-ff release-x.y
# git branch –d release-1.2
**Hotfix分支用于紧急bug修复，源自master，merge到develop和master。**
对于已发布的产品，可能有意外的紧急bug需要修复。hotfix branch可以避免修复bug的工作影响develop branch。
创建hotfix branch：
# git checkout -b hotfix-x.y master
完成hotfix branch：
# git checkout master
# git merge --no-ff hotfix-x.y
# git tag –a x.y.z
# git checkout develop
# git merge --no-ff hotfix-x.y
# git branch –d hotfix-x.y
这里有个例外就是，如果hotfix发生时有正在进行的release branch，那么将hotfix merge到release，而release最终会merge到develop和master。
到此我们的分支模型介绍就完毕了。我使用这个branch model有几个月了，感觉是：Great！
