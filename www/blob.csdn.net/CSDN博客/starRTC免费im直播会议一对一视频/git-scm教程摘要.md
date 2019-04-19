# git-scm教程摘要 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年11月08日 17:14:31[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：53
![](http://note.youdao.com/yws/res/107641/7E2751A4DE1841A8B65D26DC9A8C1AB8)
Git 有三种状态
已提交（committed）、已修改（modified）和已暂存（staged）
已提交表示数据已经安全的保存在本地数据库中。
已修改表示修改了文件，但还没保存到数据库中。
已暂存表示对一个已修改文件的当前版本做了标记，使之包含在下次提交的快照中。
由此引入 Git 项目的三个工作区域的概念：Git 仓库、工作目录以及暂存区域。
![](http://note.youdao.com/yws/res/107533/24588668C327416B819591648AD3AC0A)
Git 仓库目录是 Git 用来保存项目的元数据和对象数据库的地方。 这是 Git 中最重要的部分，从其它计算机克隆仓库时，拷贝的就是这里的数据。
工作目录是对项目的某个版本独立提取出来的内容。 这些从 Git 仓库的压缩数据库中提取出来的文件，放在磁盘上供你使用或修改。
暂存区域是一个文件，保存了下次将提交的文件列表信息，一般在 Git 仓库目录中。 有时候也被称作`‘索引’'，不过一般说法还是叫暂存区域。
基本的 Git 工作流程如下：
- 在工作目录中修改文件。
- 暂存文件，将文件的快照放入暂存区域。
- 提交更新，找到暂存区域的文件，将快照永久性存储到 Git 仓库目录。
如果 Git 目录中保存着的特定版本文件，就属于已提交状态。
如果作了修改并已放入暂存区域，就属于已暂存状态。
如果自上次取出后，作了修改但还没有放到暂存区域，就是已修改状态。 
安装完 Git 应该做的第一件事就是设置你的用户名称与邮件地址。
$ git config --global user.name "John Doe" $ git config --global user.email [johndoe@example.com](mailto:johndoe@example.com)
想要检查你的配置，可以使用 git config --list 命令
工作目录下的每一个文件都不外乎这两种状态：已跟踪或未跟踪。 
 已跟踪的文件是指那些被纳入了版本控制的文件，在上一次快照中有它们的记录，在工作一段时间后，它们的状态可能处于未修改，已修改或已放入暂存区。
工作目录中除已跟踪文件以外的所有其它文件都属于未跟踪文件Untracked ，它们既不存在于上次快照的记录中，也没有放入暂存区。 初次克隆某个仓库的时候，工作目录中的所有文件都属于已跟踪文件，并处于未修改状态。
![](http://note.youdao.com/yws/res/107553/90E4643F10C0489C8875F0FC337EC971)
使用命令 git add 开始跟踪一个文件。 所以，要跟踪 README 文件，运行：
$ git add README
此时再运行 git status 命令，会看到 README 文件已被跟踪，并处于暂存状态：
$ git status On branch master Changes to be committed: (use "git reset HEAD <file>..." to unstage) new file: README
只要在 Changes to be committed 这行下面的，就说明是已暂存状态。
出现在 Changes not staged for commit 这行下面，说明已跟踪文件的内容发生了变化，但还没有放到暂存区。要暂存这次更新，需要运行 git add 命令（“添加内容到下一次提交中”）。
一个文件运行了 git add 之后又作了修订，需要重新运行 git add 把最新版本重新暂存起来：
每一次运行提交commit操作，都是对你项目作一次快照，以后可以回到这个状态
怎样跳过暂存git add直接提交ci？
跳过使用暂存区域
在提交的时候，给 git commit 加上 -a 选项,会自动把所有已经跟踪过的文件暂存起来一并提交，从而跳过 git add 步骤
撤消操作
[https://git-scm.com/book/zh/v2/Git-%E5%9F%BA%E7%A1%80-%E6%92%A4%E6%B6%88%E6%93%8D%E4%BD%9C](https://git-scm.com/book/zh/v2/Git-%E5%9F%BA%E7%A1%80-%E6%92%A4%E6%B6%88%E6%93%8D%E4%BD%9C)
取消暂存的文件
撤消对文件的修改
origin - 这是 Git 给你克隆的仓库服务器的默认名字：
添加远程仓库
git remote add <shortname> <url> 添加一个新的远程 Git 仓库，同时指定一个你可以轻松引用的简写：
$ git remote origin $ git remote add pb https://github.com/paulboone/ticgit $ git remote -v origin https://github.com/schacon/ticgit (fetch) origin https://github.com/schacon/ticgit (push) pb https://github.com/paulboone/ticgit (fetch) pb https://github.com/paulboone/ticgit (push)
如果你想拉取 Paul 的仓库中有但你没有的信息，可以运行 git fetch pb：
$ git fetch pb remote: Counting objects: 43, done. remote: Compressing objects: 100% (36/36), done. remote: Total 43 (delta 10), reused 31 (delta 5) Unpacking objects: 100% (43/43), done. From https://github.com/paulboone/ticgit * [new branch] master -> pb/master * [new branch] ticgit -> pb/ticgit
现在 Paul 的 master 分支可以在本地通过 pb/master 访问到 - 你可以将它合并到自己的某个分支中，或者如果你想要查看它的话，可以检出一个指向该点的本地分支。
从远程仓库中抓取与拉取
$ git fetch [remote-name]
这个命令会访问远程仓库，从中拉取所有你还没有的数据。执行完成后，你将会拥有那个远程仓库中所有分支的引用，可以随时合并或查看。
如果你使用 clone 命令克隆了一个仓库，命令会自动将其添加为远程仓库并默认以 “origin” 为简写。 
所以，git fetch origin 会抓取克隆（或上一次抓取）后新推送的所有工作。 
 git fetch 命令会将数据拉取到你的本地仓库 - 它并不会自动合并或修改你当前的工作。
如果你有一个分支设置为跟踪一个远程分支，可以使用 git pull命令来自动的抓取然后合并远程分支到当前分支。
git clone 命令会自动设置本地 master 分支跟踪克隆的远程仓库的 master 分支（或不管是什么名字的默认分支）。
推送到远程仓库
git push [remote-name] [branch-name]。  当你想要将 master 分支推送到 origin 服务器时,简写
$ git push origin master
之前没有人推送过时，这条命令才能生效。
当你和其他人在同一时间克隆，他们先推送到上游然后你再推送到上游，你的推送就会毫无疑问地被拒绝。 你必须先将他们的工作拉取下来并将其合并进你的工作后才能推送。
查看远程仓库
如果想要查看某一个远程仓库的更多信息，可以使用 git remote show [remote-name] 命令。 
git remote show origin
分支
使用分支意味着你可以把你的工作从开发主线上分离开来，以免影响开发主线。
Git 的 “master” 分支并不是一个特殊分支。 它就跟其它分支完全没有区别。 之所以几乎每一个仓库都有 master 分支，是因为 git init 命令默认创建它，并且大多数人都懒得去改动它。
创建一个 testing 分支， 你需要使用 git branch 命令：
$ git branch testing
![](http://note.youdao.com/yws/res/107625/790D57DEEE0F42F39FFE8DE926047E5D)
Git 又是怎么知道当前在哪一个分支上呢？ 也很简单，它有一个名为 HEAD 的特殊指针。指向当前所在的本地分支，将 HEAD 想象为当前分支的别名
![](http://note.youdao.com/yws/res/107630/555014C240E047B7B7F9B135994A882F)
分支切换
现在切换到新创建的 testing 分支去：
$ git checkout testing
这样 HEAD 就指向 testing 分支了。
![](http://note.youdao.com/yws/res/107634/5305FAA1DC4C44A2A461871C8C85662D)
分支切换会改变你工作目录中的文件
在切换分支时，一定要注意你工作目录里的文件会被改变。
分支的新建与合并
首先，我们假设你正在你的项目上工作，并且已经有一些提交。
![](http://note.youdao.com/yws/res/107643/BD04A581156844599D7509A7A749CCFB)
 想要新建一个分支并同时切换到那个分支上，你可以运行一个带有 -b 参数的 git checkout 命令：
$ git checkout -b iss53 Switched to a new branch "iss53"
它是下面两条命令的简写：
$ git branch iss53 $ git checkout iss53
![](http://note.youdao.com/yws/res/107649/68BBCB6B2D0B45258A229E707C9E2705)
![](http://note.youdao.com/yws/res/107651/876B3C36F14C458E98D7F475B1C621A3)
现在你接到那个电话，有个紧急问题等待你来解决。你不必把这个紧急问题和 iss53 的修改混在一起，你也不需要花大力气来还原关于 53# 问题的修改，然后再添加关于这个紧急问题的修改，最后将这个修改提交到线上分支。 你所要做的仅仅是切换回 master 分支。
但是，在你这么做之前，要留意你的工作目录和暂存区里那些还没有被提交的修改，它可能会和你即将检出的分支产生冲突从而阻止 Git 切换到该分支。 最好的方法是，在你切换分支之前，保持好一个干净的状态。 有一些方法可以绕过这个问题（即，保存进度（stashing） 和 修补提交（commit amending）），
 现在，我们假设你已经把你的修改全部提交了，这时你可以切换回 master 分支了：
$ git checkout master Switched to branch 'master'
接下来，你要修复这个紧急问题。 让我们建立一个针对该紧急问题的分支（hotfix branch），在该分支上工作直到问题解决：
$ git checkout -b hotfix Switched to a new branch 'hotfix' $ vim index.html $ git commit -a -m 'fixed the broken email address' [hotfix 1fb7853] fixed the broken email address 1 file changed, 2 insertions(+)
![](http://note.youdao.com/yws/res/107658/2B7E1D27763C421B9ED39F55839DB4D4)
然后将其合并回你的 master 分支来部署到线上。 你可以使用 git merge 命令来达到上述目的：
$ git checkout master //checkout是切换分支！ $ git merge hotfix Updating f42c576..3a0874c Fast-forward index.html | 2 ++ 1 file changed, 2 insertions(+)
“快进（fast-forward）”
![](http://note.youdao.com/yws/res/107664/359FFDC2D25248728B829F3CA3DDC2E3)
关于这个紧急问题的解决方案发布之后，你准备回到被打断之前时的工作中。 然而，你应该先删除 hotfix分支，因为你已经不再需要它了 。 你可以使用带 -d 选项的 git branch 命令来删除分支：
$ git branch -d hotfix Deleted branch hotfix (3a0874c).
现在你可以切换回你正在工作的分支继续你的工作，
$ git checkout iss53 Switched to branch "iss53" $ vim index.html $ git commit -a -m 'finished the new footer [issue 53]'
![](http://note.youdao.com/yws/res/107670/F7C5509760274E13A09AA397BC8E683A)
你在 hotfix 分支上所做的工作并没有包含到 iss53 分支中。 如果你需要拉取 hotfix 所做的修改，你可以使用 git merge master 命令将 master 分支合并入 iss53 分支，
或者你也可以等到 iss53 分支完成其使命，再将其合并回 master 分支。
分支的合并
假设你已经修正了 #53 问题，并且打算将你的工作合并入 master 分支。 为此，你需要合并 iss53 分支到 master 分支，这和之前你合并 hotfix 分支所做的工作差不多。
$ git checkout master Switched to branch 'master' $ git merge iss53
Git 做了一个新的快照并且自动创建一个新的提交指向它。 这个被称作一次合并提交，
![](http://note.youdao.com/yws/res/107681/730E395CA1A34E4B8498A8D07C800723)
删除这个分支。
$ git branch -d iss53
遇到冲突时的分支合并
 如果你在两个不同的分支中，对同一个文件的同一个部分进行了不同的修改，Git 就没法干净的合并它们。 如果你对 #53 问题的修改和有关 hotfix 的修改都涉及到同一个文件的同一处，在合并它们的时候就会产生合并冲突：
$ git merge iss53 Auto-merging index.html CONFLICT (content): Merge conflict in index.html Automatic merge failed; fix conflicts and then commit the result.
此时 Git 做了合并，但是没有自动地创建一个新的合并提交。（如上图中的C6）
 Git 会暂停下来，等待你去解决合并产生的冲突。 你可以使用 git status 命令来查看：
$ git status On branch master You have unmerged paths. (fix conflicts and run "git commit") Unmerged paths: (use "git add <file>..." to mark resolution) both modified: index.html no changes added to commit (use "git add" and/or "git commit -a")
出现冲突的文件会包含一些特殊区段，看起来像下面这个样子：
<<<<<<< HEAD:index.html <div id="footer">contact : email.support@github.com</div> ======= <div id="footer"> please contact us at support@github.com </div> >>>>>>> iss53:index.html
这表示 HEAD 所指示的版本（也就是你的 master 分支所在的位置，因为你在运行 merge 命令的时候已经检出到了这个分支）在这个区段的上半部分（======= 的上半部分），
而 iss53 分支所指示的版本在 ======= 的下半部分。 为了解决冲突，你必须选择使用由 ======= 分割的两部分中的一个，或者你也可以自行合并这些内容。 例如，你可以通过把这段内容换成下面的样子来解决冲突：
<div id="footer"> please contact us at email.support@github.com </div>
 在你解决了所有文件里的冲突之后，对每个文件使用 git add 命令来将其标记为冲突已解决。（暂存）
你可以再次运行 git status 来确认所有的合并冲突都已被解决
输入 git commit 来完成合并提交。
分支管理
不加任何参数运行它，会得到当前所有分支的一个列表：
$ git branch iss53 * master testing
注意 master 分支前的 * 字符：它代表现在检出的那一个分支（也就是说，当前 HEAD 指针所指向的分支）。 
 如果需要查看每一个分支的最后一次提交，可以运行 git branch -v 命令：
请牢记，当你做这么多操作的时候，这些分支全部都存于本地。 当你新建和合并分支的时候，所有这一切都只发生在你本地的 Git 版本库中 —— 没有与服务器发生交互。
远程分支
[https://git-scm.com/book/zh/v2/Git-%E5%88%86%E6%94%AF-%E8%BF%9C%E7%A8%8B%E5%88%86%E6%94%AF](https://git-scm.com/book/zh/v2/Git-%E5%88%86%E6%94%AF-%E8%BF%9C%E7%A8%8B%E5%88%86%E6%94%AF)
如果你想要看，你最后一次与远程仓库 origin 通信时 master分支的状态，你可以查看 origin/master 分支。
你与同事合作解决一个问题并且他们推送了一个 iss53 分支，你可能有自己的本地 iss53 分支；但是在服务器上的分支会指向 origin/iss53 的提交。
假设你的网络里有一个在 git.ourcompany.com 的 Git 服务器。 如果你从这里克隆，Git 的 clone 命令会为你自动将其命名为 origin，拉取它的所有数据，创建一个指向它的 master 分支的指针，并且在本地将其命名为 origin/master。
Git 也会给你一个与 origin 的 master 分支指向同一个地方的本地 master 分支
远程仓库名字 “origin” 与分支名字 “master” 一样，在 Git 中并没有任何特别的含义一样。
“origin” 是当你运行 git clone 时默认的远程仓库名字。
 如果你运行 git clone -o booyah，那么你默认的远程分支名字将会是 booyah/master。
![](http://note.youdao.com/yws/res/107724/0C80EDE45E824245AA539AF6144B5DEA)
如果你在本地的 master 分支做了一些工作，然而在同一时间，其他人推送提交到 git.ourcompany.com 并更新了它的 master 分支
![](http://note.youdao.com/yws/res/107727/C83CAD0849944C6294EE32B3CAC38453)
如果要同步你的工作，运行 git fetch origin 命令。 这个命令查找 “origin” 是哪一个服务器（在本例中，它是 git.ourcompany.com），从中抓取本地没有的数据，并且更新本地数据库，移动 origin/master指针指向新的、更新后的位置。
![](http://note.youdao.com/yws/res/107732/DDC6B4665F9446BD8798687A7605D988)
git fetch 更新你的远程仓库引用
为了演示有多个远程仓库与远程分支的情况，我们假定你有另一个内部 Git 服务器，仅用于你的 sprint 小组的开发工作。 这个服务器位于 git.team1.ourcompany.com。 你可以运行 git remote add 命令添加一个新的远程仓库引用到当前的项目，将这个远程仓库命名为 teamone，将其作为整个 URL 的缩写
![](http://note.youdao.com/yws/res/107740/C1B79C3ECF0945EFBEB31F24CB97C180)
现在，可以运行 git fetch teamone 来抓取远程仓库 teamone 有而本地没有的数据。
因为那台服务器上现有的数据是 origin 服务器上的一个子集，所以 Git 并不会抓取数据而是会设置远程跟踪分支 teamone/master 指向 teamone 的 master 分支。
![](http://note.youdao.com/yws/res/107745/22A12C623A7D4BF681E33C187E3B7E60)
推送
如果希望和别人一起在名为 serverfix 的分支上工作，你可以像推送第一个分支那样推送它。 运行 git push (remote) (branch):
$ git push origin serverfix
“推送本地的 serverfix 分支来更新远程仓库上的 serverfix 分支。
也可以运行 git push origin serverfix:serverfix，它会做同样的事
 如果并不想让远程仓库上的分支叫做 serverfix，可以运行 git push origin serverfix:awesomebranch 来将本地的 serverfix 分支推送到远程仓库上的 awesomebranch 分支。
下一次其他协作者从服务器上抓取数据时，他们会在本地生成一个远程分支 origin/serverfix，指向服务器的 serverfix 分支的引用：
$ git fetch origin remote: Counting objects: 7, done. remote: Compressing objects: 100% (2/2), done. remote: Total 3 (delta 0), reused 3 (delta 0) Unpacking objects: 100% (3/3), done. From https://github.com/schacon/simplegit * [new branch] serverfix -> origin/serverfix
要特别注意的一点是当抓取到新的远程跟踪分支时，本地不会自动生成一份可编辑的副本（拷贝）。 
可以运行 git merge origin/serverfix 将这些工作合并到当前所在的分支。 
 如果想要在自己的 serverfix 分支上工作，可以将其建立在远程跟踪分支之上：
$ git checkout -b serverfix origin/serverfix Branch serverfix set up to track remote branch serverfix from origin. Switched to a new branch 'serverfix'
这会给你一个用于工作的本地分支，并且起点位于 origin/serverfix。
跟踪分支
从一个远程跟踪分支检出一个本地分支会自动创建一个叫做 “跟踪分支”
跟踪分支是与远程分支有直接关系的本地分支。 如果在一个跟踪分支上输入 git pull，Git 能自动地识别去哪个服务器上抓取、合并到哪个分支。
当克隆一个仓库时，它通常会自动地创建一个跟踪 origin/master 的 master 分支。
如果你愿意的话可以设置其他的跟踪分支，或者不跟踪 master 分支。
运行 git checkout -b [branch] [remotename]/[branch]。 如：
git checkout -b serverfix origin/serverfix
这是一个十分常用的操作所以 Git 提供了 --track 快捷方式：
git checkout --track origin/serverfix Branch serverfix set up to track remote branch serverfix from origin. Switched to a new branch 'serverfix'
如果想要将本地分支与远程分支设置为不同名字：
$ git checkout -b sf origin/serverfix Branch sf set up to track remote branch serverfix from origin. Switched to a new branch 'sf'
现在，本地分支 sf 会自动从 origin/serverfix 拉取。
设置已有的本地分支跟踪一个刚刚拉取下来的远程分支，或者想要修改正在跟踪的上游分支，你可以在任意时间使用 -u 或 --set-upstream-to 选项运行 git branch 来显式地设置。
$ git branch -u origin/serverfix Branch serverfix set up to track remote branch serverfix from origin.
如果想要查看设置的所有跟踪分支，可以使用 git branch 的 -vv 选项。 这会将所有的本地分支列出来并且包含更多的信息，如每一个分支正在跟踪哪个远程分支与本地分支是否是领先、落后或是都有。
$ git branch -vv iss53 7e424c3 [origin/iss53: ahead 2] forgot the brackets master 1ae2a45 [origin/master] deploying index fix * serverfix f8674d9 [teamone/server-fix-good: ahead 3, behind 1] this should do it testing 5ea463a trying something new
这里可以看到 iss53 分支正在跟踪 origin/iss53 并且 “ahead” 是 2，意味着本地有两个提交还没有推送到服务器上。
也能看到 master 分支正在跟踪 origin/master 分支并且是最新的。
接下来可以看到 serverfix 分支正在跟踪 teamone 服务器上的 server-fix-good 分支并且领先 3 落后 1，意味着服务器上有一次提交还没有合并入同时本地有三次提交还没有推送。 最后看到 testing 分支并没有跟踪任何远程分支。
需要重点注意的一点是这些数字的值来自于你从每个服务器上最后一次抓取的数据。 这个命令并没有连接服务器，它只会告诉你关于本地缓存的服务器数据。 如果想要统计最新的领先与落后数字，需要在运行此命令前抓取所有的远程仓库。 可以像这样做：$ git fetch --all; git branch -vv
拉取
当 git fetch 命令从服务器上抓取本地没有的数据时，它并不会修改工作目录中的内容。 它只会获取数据然后让你自己合并。
git pull 会查找当前分支所跟踪的服务器与分支，从服务器上抓取数据然后尝试合并入那个远程分支。
由于 git pull 的魔法经常令人困惑所以通常单独显式地使用 fetch 与 merge 命令会更好一些。
变基
在 Git 中整合来自不同分支的修改主要有两种方法：merge 以及 rebase。
![](http://note.youdao.com/yws/res/107797/67884C2C1A2F4D3896E5930CA73513ED)
整合分支最容易的方法是 merge 命令：生成了一个新的提交C5
![](http://note.youdao.com/yws/res/107800/35FDAED45E0E4C9A83BFCE28FF72F88A)
其实，还有一种方法：
你可以提取在 C4 中引入的补丁和修改，然后在 C3 的基础上应用一次。这种操作就叫做 变基。 
可以使用 rebase 命令将提交到某一分支上的所有修改都移至另一分支上
git checkout experiment
git rebase master
First, rewinding head to replay your work on top of it... Applying: added staged command
它的原理是首先找到这两个分支（即当前分支 experiment、变基操作的目标基底分支 master）的最近共同祖先 C2，然后对比当前分支相对于该祖先的历次提交，提取相应的修改并存为临时文件，然后将当前分支指向目标基底 C3, 最后以此将之前另存为临时文件的修改依序应用。（译注：写明了 commit id，以便理解，下同）
将 C4 中的修改变基到 C3 上：
![](http://note.youdao.com/yws/res/107816/405FA982C2CE4A3FA5A5930F360695EA)
现在回到 master 分支，进行一次快进合并。
$ git checkout master $ git merge experiment
![](http://note.youdao.com/yws/res/107821/7740D1AC7BC24DEDA20CEB348BFFA091)
 这两种整合方法的最终结果没有任何区别，但是变基使得提交历史更加整洁。
尽管实际的开发工作是并行的，但它们看上去就像是串行的一样，提交历史是一条直线没有分叉。
一般我们这样做的目的是为了确保在向远程分支推送时能保持提交历史的整洁：
例如向某个其他人维护的项目贡献代码时。 在这种情况下，你首先在自己的分支里进行开发，当开发完成时你需要先将你的代码变基到 origin/master 上，然后再向主项目提交修改。 这样的话，该项目的维护者就不再需要进行整合工作，只需要快进合并便可。
更有趣的变基例子
你创建了一个特性分支 server，为服务端添加了一些功能，提交了 C3 和 C4。 然后从 C3 上创建了特性分支 client，为客户端添加了一些功能，提交了 C8 和 C9。 最后，你回到 server 分支，又提交了 C10。
![](http://note.youdao.com/yws/res/107835/D9F123D2AF974BC9A81DCECAA8F17C8F)
假设你希望将 client 中的修改合并到主分支并发布，但暂时并不想合并 server 中的修改
这时，你就可以使用 git rebase 命令的 --onto 选项
，选中在 client 分支里但不在 server 分支里的修改（即 C8 和 C9），将它们在 master 分支上重放：
git rebase --onto master server client
以上命令的意思是：“取出 client 分支，找出处于 client 分支和 server 分支的共同祖先之后的修改，然后把它们在 master 分支上重放一遍”。 这理解起来有一点复杂，不过效果非常酷。
![](http://note.youdao.com/yws/res/107843/9CBBC6CAE3EF4D3789A334F236BD46F5)
现在可以快进合并 master 分支了。 [快进合并 master 分支，使之包含来自 client 分支的修改](https://git-scm.com/book/zh/v2/ch00/rrbdiag_g)）
$ git checkout master $ git merge client
![](http://note.youdao.com/yws/res/107848/4EC0D6807FD24411BE2DFB9F006491E3)
接下来你决定将 server 分支中的修改也整合进来。
使用 git rebase [basebranch] [topicbranch] 命令可以直接将特性分支（即本例中的 server）变基到目标分支（即 master）上。
这样做能省去你先切换到 server 分支，再对其执行变基命令的多个步骤。
git rebase master server
![](http://note.youdao.com/yws/res/107855/2EB61C178E83405CA060925E31D010C3)
然后就可以快进合并主分支 master 了：
$ git checkout master $ git merge server
至此，client 和 server 分支中的修改都已经整合到主分支里了，你可以删除这两个分支
$ git branch -d client $ git branch -d server
![](http://note.youdao.com/yws/res/107860/D779F802F6374D9CBECE6CF79A948AFB)
变基的风险.
不要对在你的仓库外有副本的分支执行变基。
假设你从一个中央服务器克隆然后在它的基础上进行了一些开发。 你的提交历史如图所示：
![](http://note.youdao.com/yws/res/107865/55869C6CAEBE49DEACD87D757241772A)
然后，某人又向中央服务器提交了一些修改，其中还包括一次合并。 你抓取了这些在远程分支上的修改，并将其合并到你本地的开发分支，然后你的提交历史就会变成这样：
![](http://note.youdao.com/yws/res/107869/A74D35D87782413EA9B18899996B239B)
接下来，这个人又决定把合并操作回滚，改用变基；继而又用 git push --force 命令覆盖了服务器上的提交历史。 之后你从服务器抓取更新，会发现多出来一些新的提交。
![](http://note.youdao.com/yws/res/107873/2D292598935F4E818166056253D2B230)
Figure 46. 有人推送了经过变基的提交，并丢弃了你的本地开发所基于的一些提交
结果就是你们两人的处境都十分尴尬。 如果你执行 git pull 命令，你将合并来自两条提交历史的内容，生成一个新的合并提交，最终仓库会如图所示：
![](http://note.youdao.com/yws/res/107874/BB8978C91C25498BB6E6B1F512BA3926)
Figure 47. 你将相同的内容又合并了一次，生成了一个新的提交
此时如果你执行 git log 命令，你会发现有两个提交的作者、日期、日志居然是一样的，这会令人感到混乱。 此外，如果你将这一堆又推送到服务器上，你实际上是将那些已经被变基抛弃的提交又找了回来，这会令人感到更加混乱。 很明显对方并不想在提交历史中看到 C4 和 C6，因为之前就是他把这两个提交通过变基丢弃的。
用变基解决变基
你需要做的就是检查你做了哪些修改，以及他们覆盖了哪些修改。
只要你把变基命令当作是在推送前清理提交使之整洁的工具，并且只在从未推送至共用仓库的提交上执行变基命令，就不会有事。
只对尚未推送或分享给别人的本地修改执行变基操作清理历史，从不对已推送至别处的提交执行变基操作，这样，你才能享受到两种方式带来的便利。
git stash 命令用来临时地保存一些还没有提交的工作，以便在分支上不需要提交未完成工作就可以清理工作目录。
[https://git-scm.com/book/zh/v2/Git-%E5%B7%A5%E5%85%B7-%E5%82%A8%E8%97%8F%E4%B8%8E%E6%B8%85%E7%90%86#r_git_stashing](https://git-scm.com/book/zh/v2/Git-%E5%B7%A5%E5%85%B7-%E5%82%A8%E8%97%8F%E4%B8%8E%E6%B8%85%E7%90%86#r_git_stashing)
