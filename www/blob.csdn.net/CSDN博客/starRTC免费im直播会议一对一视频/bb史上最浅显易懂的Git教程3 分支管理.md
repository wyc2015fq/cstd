# bb史上最浅显易懂的Git教程3 分支管理 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月12日 09:42:00[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：136
假设你准备开发一个新功能，但是需要两周才能完成，第一周你写了50%的代码，如果立刻提交，由于代码还没写完，不完整的代码库会导致别人不能干活了。如果等代码全部写完再一次提交，又存在丢失每天进度的巨大风险。
有了分支，就不用怕了。你创建了一个属于你自己的分支，别人看不到，还继续在原来的分支上正常工作，而你在自己的分支上干活，想提交就提交，直到开发完毕后，再一次性合并到原来的分支上，这样，既安全，又不影响别人工作。
当我们创建新的分支，例如dev时，Git新建了一个指针叫dev，指向master相同的提交，再把HEAD指向dev，就表示当前分支在dev上：
从现在开始，对工作区的修改和提交就是针对dev分支了，比如新提交一次后，dev指针往前移动一步，而master指针不变：

假如我们在dev上的工作完成了，就可以把dev合并到master上。Git怎么合并呢？最简单的方法，就是直接把master指向dev的当前提交，就完成了合并：
所以Git合并分支也很快！就改改指针，工作区内容也不变！
合并完分支后，甚至可以删除dev分支。删除dev分支就是把dev指针给删掉，删掉后，我们就剩下了一条master分支：
创建dev分支，然后切换到dev分支：$
git checkout -b dev
-b参数表示创建并切换,相当于以下两条命令：
$
git branch dev$
git checkout dev
然后，用git branch命令查看当前分支：
$
git branch* dev master
git branch命令会列出所有分支，当前分支前面会标一个*号。
在dev分支上正常add,commit,最后，
dev分支的工作完成，我们就可以切换回master分支：
$
git checkout master
切换回master分支后，可以查看一个readme.txt文件，刚才添加的内容不见了！因为那个提交是在dev分支上，而master分支此刻的提交点并没有变：
现在，我们把dev分支的工作成果合并到master分支上：
$
git merge dev
git merge命令用于合并指定分支到当前分支。
注意到输出的Fast-forward信息，Git告诉我们，这次合并是“快进模式”，也就是直接把master指向dev的当前提交，所以合并速度非常快。
当然，也不是每次合并都能Fast-forward，我们后面会讲其他方式的合并。
合并完成后，就可以放心地删除dev分支了：
$
git branch -d dev
Git鼓励你使用分支完成某个任务，合并后再删掉分支，这和直接在master分支上工作效果是一样的，但过程更安全。
解决冲突！！！
master分支和feature1分支各自都分别有新的提交
这种情况下，Git无法执行“快速合并”，只能试图把各自的修改合并起来，但这种合并就可能会有冲突，我们试试看：
$
git merge feature1
果然冲突了！Git告诉我们，readme.txt文件存在冲突，必须手动解决冲突后再提交。git
 status也可以告诉我们冲突的文件：
查看readme.txt
Git tracks changes of files.<<<<<<<
HEADCreatinganewbranchisquick &
simple.=======CreatinganewbranchisquickANDsimple.>>>>>>>
 feature1
Git用<<<<<<<，=======，>>>>>>>标记出不同分支的内容
修改完冲突后，再add,commit
master分支和feature1分支变成了下图所示：
最后，删除feature1分支：
$
git branch -d feature1
--no-ff参数，表示禁用Fast
 forward：Git就会在merge时生成一个新的commit，这样，从分支历史上就可以看出分支信息， 因为本次合并要创建一个新的commit，所以加上-m参数，把commit描述写进去。能看出来曾经做过合并，而fast
 forward合并就看不出来曾经做过合并。
$
git merge --no-ff -m
"merge with no-ff" dev
不使用Fast forward模式，merge后就像这样：
在实际开发中，我们应该按照几个基本原则进行分支管理：
首先，master分支应该是非常稳定的，也就是仅用来发布新版本，平时不能在上面干活；
干活都在dev分支上，也就是说，dev分支是不稳定的，到某个时候，比如1.0版本发布时，再把dev分支合并到master上，
 在master分支发布1.0版本；
Bug分支
每个bug都可以通过一个新的临时分支来修复，修复后，合并分支，然后将临时分支删除。
你接到一个修复一个代号101的bug的任务时，很自然地，你想创建一个分支issue-101来修复它，但是，等等，当前正在dev上进行的工作还没有提交：
并不是你不想提交，而是工作只进行到一半，还没法提交，预计完成还需1天时间。但是，必须在两个小时内修复该bug，怎么办？
幸好，Git还提供了一个stash功能，可以把当前工作现场“储藏”起来，等以后恢复现场后继续工作：
$
git stash
现在，用git status查看工作区，就是干净的，可以放心地创建分支来修复bug。

首先确定要在哪个分支上修复bug，假定需要在master分支上修复，就从master创建临时分支：
$
git checkout masterSwitched
 to branch 'master'Your
 branch is ahead of 'origin/master'
 by 6 commits.$
git checkout -b issue-101Switched
 to a new branch 'issue-101'
修复bug。。。。。
然后add ,commit
修复完成后，切换到master分支，并完成合并，最后删除issue-101分支：
$
git checkout master
$
git merge --no-ff -m
"merged bug fix 101" issue-101
git
 branch -d issue-101
现在，是时候接着回到dev分支干活了！
$
git stash liststash@{0}:WIP on
dev:6224937 add merge
工作现场还在，Git把stash内容存在某个地方了，需要恢复一下，有两个办法：
git stash pop
再用git stash list查看，就看不到任何stash内容了：
$
git stash list
3,Feature分支
每添加一个新功能，最好新建一个feature分支，在上面开发，完成后，合并，最后，删除该feature分支。
如果开发完了，不想要这个分支，即不想合并，怎么删除：用-D选项
git branch -D
 feature-vulcan
即丢弃一个没有被合并过的分支，可以通过git
 branch -D <name>强行删除。

4，多人协作
当你从远程仓库克隆时，实际上Git自动把本地的master分支和远程的master分支对应起来了，并且，远程仓库的默认名称是origin。
要查看远程库的信息，用git remote：
$
git remoteorigin
或者，用git remote -v显示更详细的信息：
$
git remote -vorigin git@github.com:michaelliao/learngit.git
 (fetch)origin git@github.com:michaelliao/learngit.git
 (push)
上面显示了可以抓取和推送的origin的地址。如果没有推送权限，就看不到push的地址。
推送分支
就是把该分支上的所有本地提交推送到远程库。推送时，要指定本地分支，这样，Git就会把该分支推送到远程库对应的远程分支上：
$
git push origin master
如果要推送其他分支，比如dev，就改成：
$
git push origin dev
并不是一定要把本地分支往远程推送，那么，哪些分支需要推送，哪些不需要呢？
- master分支是主分支，因此要时刻与远程同步；
- dev分支是开发分支，团队所有成员都需要在上面工作，所以也需要与远程同步；
- bug分支只用于在本地修复bug，就没必要推到远程了
feature分支是否推到远程，取决于你是否和你的小伙伴合作在上面开发。
多人协作时，大家都会往master和dev分支上推送各自的修改。
你的小伙伴要在dev分支上开发，就必须创建远程origin的dev分支到本地，于是他用这个命令创建本地dev分支：
$
git checkout -b dev
origin/dev
现在，他就可以在dev上继续修改，然后，时不时地把dev分支push到远程：git
 push origin dev
你的小伙伴已经向origin/dev分支推送了他的提交，而碰巧你也对同样的文件作了修改，并试图推送：
推送失败，因为你的小伙伴的最新提交和你试图推送的提交有冲突，解决办法也很简单，Git已经提示我们，先用git
 pull把最新的提交从origin/dev抓下来，然后，在本地合并，解决冲突，再推送：
$ git pull
git pull也失败了，原因是没有指定本地dev分支与远程origin/dev分支的链接，根据提示，设置dev和origin/dev的链接：
$ git branch
*--set-upstream dev origin/dev*
再pull：
$
git pull
这回git pull成功，但是合并有冲突，需要手动解决，解决后，提交，再push：
本地新建的分支如果不推送到远程，对其他人就是不可见的；
4，标签管理
发布一个版本时，我们通常先在版本库中打一个标签（tag），这样，就唯一确定了打标签时刻的版本。将来无论什么时候，取某个标签的版本，就是把那个打标签的时刻的历史版本取出来。所以，标签也是版本库的一个快照。
Git的标签虽然是版本库的快照，但其实它就是指向某个commit的指针（跟分支很像对不对？但是分支可以移动，标签不能移动），所以，创建和删除标签都是瞬间完成的。
tag就是一个让人容易记住的有意义的名字，它跟某个commit绑在一起。
首先，切换到需要打标签的分支上：
$
git branch* dev master$
git checkout master
$
git tag v1.0
可以用命令git tag查看所有标签
默认标签是打在最新提交的commit上的。有时候，如果忘了打标签，比如，现在已经是周五了，但应该在周一打的标签没有打，怎么办？
方法是找到历史提交的commit id，然后打上就可以了：
$
git log --pretty=oneline --abbrev-commit
$
git tag v0.96224937
注意，标签不是按时间顺序列出，而是按字母排序的。可以用git
 show <tagname>查看标签信息
$ git
**show** v0.9
还可以创建带有说明的标签，用-a指定标签名，-m指定说明文字：
$
git tag -a v0.1
 -m "version 0.1 released"3628164
如果标签打错了，也可以删除：
$
git tag -d v0.1
因为创建的标签都只存储在本地，不会自动推送到远程。所以，打错的标签可以在本地安全删除。
如果要推送某个标签到远程，使用命令git
 push origin <tagname>：
$
git push origin v1.0
或者，一次性推送全部尚未推送到远程的本地标签：
$ git push origin --tags
如果标签已经推送到远程，要删除远程标签就麻烦一点，先从本地删除：
$
git tag -d v0.9
然后，从远程删除。删除命令也是push，但是格式如下：
$
git push origin
:refs/tags/v0.9
配置git
让Git显示颜色，会让命令输出看起来更醒目：
$
git config --global color.ui
**true**
忽略特殊文件
根目录下创建一个特殊的.gitignore文件，然后把要忽略的文件名填进去，Git就会自动忽略这些文件。
不需要从头写.gitignore文件，GitHub已经为我们准备了各种配置文件，只需要组合一下就可以使用了。所有配置文件可以直接在线浏览：[https://github.com/github/gitignore](https://github.com/github/gitignore)
忽略文件的原则是：
- 忽略操作系统自动生成的文件，比如缩略图等；
- 忽略编译生成的中间文件、可执行文件等，也就是如果一个文件是通过另一个文件自动生成的，那自动生成的文件就没必要放进版本库，比如Java编译产生的.class文件；
- 忽略你自己的带有敏感信息的配置文件，比如存放口令的配置文件。
假设你在Windows下进行Python开发，Windows会自动在有图片的目录下生成隐藏的缩略图文件，如果有自定义目录，目录下就会有Desktop.ini文件，因此你需要忽略Windows自动生成的垃圾文件：
*# Windows:*Thumbs.dbehthumbs.dbDesktop.ini
然后，继续忽略Python编译产生的.pyc、.pyo、dist等文件或目录：
*# Python:**.py[cod]*.so*.egg*.egg-infodistbuild
有些时候，你想添加一个文件到Git，但发现添加不了，原因是这个文件被.gitignore忽略了：
$
git add
App.**class**
可以用-f强制添加到Git：
$ git add -f App.**class**
可以用git check-ignore命令检查：
$
git check-ignore -v
App.**class****.gitignore:3:*.class App.class**
Git会告诉我们，.gitignore的第3行规则忽略了该文件，于是我们就可以知道应该修订哪个规则。
配置别名
git config --global alias.st status //敲git
 st就表示git status
git
 config --global **alias**.co
 checkout$
git config --global
**alias**.ci commit$
git config --global
**alias**.br branch
我们知道，命令git reset HEAD
 file可以把暂存区的修改撤销掉（unstage），重新放回工作区。既然是一个unstage操作，就可以配置一个unstage别名：
$
git config --global
**alias**.unstage
'reset HEAD'
当你敲入命令：
$
git unstage test.py
实际上Git执行的是：
$ git re**set**
 HEAD test.py
配置一个git last，让其显示最后一次提交信息：
$
git config --global
**alias**.last
'log -1'
这样，用git last就能显示最近一次的提交：
甚至还有人丧心病狂地把lg配置成了：
git config --global
**alias**.lg
"log --color --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit"
来看看git lg的效果：
每个仓库的Git配置文件都放在.git/config文件中
而当前用户的全局Git配置文件放在用户主目录下的一个隐藏文件.gitconfig中
参考：
[http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/0013743862006503a1c5bf5a783434581661a3cc2084efa000](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/0013743862006503a1c5bf5a783434581661a3cc2084efa000)
