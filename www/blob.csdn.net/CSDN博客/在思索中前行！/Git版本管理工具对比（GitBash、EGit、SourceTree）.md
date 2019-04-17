# Git版本管理工具对比（GitBash、EGit、SourceTree） - 在思索中前行！ - CSDN博客





2014年10月21日 16:31:54[_Tham](https://me.csdn.net/txl16211)阅读数：1113









Git管理工具对比（GitBash、EGit、SourceTree）


GitBash是采用命令行的方式对版本进行管理，功能最为灵活强大，但是由于需要手动输入希望修改的文件名，所以相对繁琐。


EGit是Eclipse的Git插件，最为纠结的一个软件，因为开发时直操作很方便，尤其是有svn开发情节的人更热衷于这样，不过EGit中有很多bug和不人性化的东西，让人吐血，所以


一句话EGit并不能解决所有Git问题，开发时必须部分依赖于其他Git管理工具。一会一一列举。


SourceTree是最近应用的一个软件，一句话概括，既有GitBash的命令行，又有EGit的图形化管理，用户界面很人性化，Eclipse+GitBash完全可以应付版本管理。


那么下面我结合项目中开发遇到的问题一一讲解一下：



## **<一>EGit之我见：**


EGit最大的好处是集成在eclipse里面，对于代码修改后提交很方便，其中我最常使用的部分是：


（1）简单的Commit，pull，push。


（2）Git repository（看资源库视图）（包括新建分支，切分支，合并分支）


（3）工程reset hard（强制覆盖本地版本），一些特殊情况会有问题。


（4）工程checkout到某一个分支，可以看当时的文件。


（5）工程show in history （查看工程变更历史），没有时分秒，很不爽。




其他的功能我用的不多，因为EGit确实有很多bug，犯过血淋淋的错误。


这里解释一下，为什么第一条我要加上”简单的”，因为很多时候没有pull下来，或者没有push上去，提示的信息很粗，有时候甚至误以为提交，发现过很多问题，所以如果确定现在没人和你同时改同一个文件，用EGit还是很爽的。



## **<二>GitBash之我见：**




一般我主要通过命令行进行pull push 还有 status的操作，这个详见我前一篇文章


《[Git Bash+EGit在项目中配合使用最常用方法总结](http://blog.csdn.net/hongshan50/article/details/22582049)》，命令行用好了很是很不错的，除了那种需要手写文件名的时候，比较不爽，一两个还可以，多了就吐血了。




## **<三>SourceTree之我见：**


这个是我最近才发现了一个很好的工具SourceTree，可以说SourceTree是EGit和GitBash的合体，既有图形化界面又有git命令行。


（1）基本的commit、pull、push都拥有，而且一旦发生错误的时候提示很准确。


（2）基本的分支管理业有，最挫的Egit删除远程分支是假删除，SourceTree完美解决了这个问题。


（3）拥有EGit中的reset（重置）和checkout（签出）两大功能，而且更加稳定。最令我怀疑的EGit的reset hard也有报错的时候，哎。SourceTree还是很稳定的。


（4） 那么最近用SourceTree，发现了一个很强大的功能，就是“丢弃”这个功能，和checkout其实原理是一样的。不过图形化界面肯定比较方便了。这里具体解释一下这个功能：




换句话说，你改了一批文件，如果想提交一部分，保留一部分，无疑命令行最麻烦。


然后你不满足，其中有几个文件你还想回滚，不想改了。那么SourceTree最容易。


这里需要注意一下命令行中checkout 是指未add的，那么可以回滚到最近的线上的commit状态。如果add之后又修改，那么会回滚到add之后的状态。


这一个原理，在Source体现的更明显，如下：


A. 首先我先将修改后的personRiskBase.jsp拖到缓存区中（1）。


图（1）

![](https://img-blog.csdn.net/20140428095318250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaG9uZ3NoYW41MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




B. 然后再次修改，发现下面的工作区中也有这个页面了（图2）。


（图2）

![](https://img-blog.csdn.net/20140428095329140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaG9uZ3NoYW41MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


C. 现在就可以对此文件进行丢弃了，如果将下面文件丢弃（checkout），那么该文件将变为缓冲区中的文件，如果将缓冲区文件丢弃，实际上回到了最近的commit版本了(reset操作)。


这里注意，如果该文件commit了，那么checkout实际上回不到commit之前的版本的，需要reset。下面详细介绍一下reset命令。





(5) 对于reset功能的应用。这里先普及一下Git理念的事：


A. 明确一点，每一次commit都是对应着一批操作而不是对应一个文件。


这点和SVN的设计理念完全不一样。


这也造成了一个必然的结局：SVN的分支存的是一个工程，所以每签出一个分支实际上都是签出一个工程。


而Git的分支存的是修改的记录，所以每签出一个分支，实际上都是对原工程的一次覆盖。


B.在开发过程中大家可能会遇到这样一个问题：Pull之后，会出现很多别人提交的代码需要你本地再重新提交一次，那么这个原因是这样的，当然这个是我个人的理解：Git会把每一次pull结果做两个处理：


B1.如果pull之后，本地没有任何问题，那么不需要再次提交别人修改的东西了，只需要继续你的修改，push就可以了（正常情况下一版都是这样）。


B2.如果pull之后，本地有问题，大部分情况是冲突的情况，那么Git会把本次当做一次不成功的pull（那么通俗的来讲，git会认为，你认为不成功，那么你把这次版本按照你的想法改一下，再提交吧），所以你做完删减之后，需要把刚才别人的东西再提一次作为一个新的commit。





(6) 对于checkout某一次提交，SourceTree也很人性化，会给出很人性化的提示。注意checkout之后，你的工程当前不属于任何分支，不过可以基于此重新创建一个分支，很方便。





## Git工程迁移方法总结


Git最近准备迁移一下位置，这里采用命令行的方式，做如下操作。


1. git remote set-url origin 资源库地址


这里主要是将本地库与远程的资源库挂钩。


用一个图表示：

![](https://img-blog.csdn.net/20140414104722343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaG9uZ3NoYW41MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





2.接下来面对两种情况，也就是git位置2是否是空地址。


a) 如果是空地址，ok很简单。


用这个命令：git push ，如果不好使可以使用强推 git push -f


当然这是把目前的工程推送到远程默认分支（master）


扩展：如果想把本地包括master在内的所有分支一起推送出去，可以使用以下方法：


（1）git push --all -f （理论可以的）


（2）保险一点采用如下命令:(一定是可以的)


     git checkout 分支名


     git push origin test（本地分支）:test（远程分支）


     如果不好用，确定是要覆盖的话，可以加一个-f


     git push -f origin test（本地分支）:test（远程分支）






b) 如果不是空地址，ok也很简单，先将该地址清空，再push。


这里我采用的清空方式是这样。


首先，在本地创建一个文件夹，然后使用：


git clone 地址


将本地与远程库挂钩，然后使用清除命令。


git rm .


或者git rm -r 文件件名（删除文件夹）


然后再push，这样远程仓库就被清空了。然后再执行a)




