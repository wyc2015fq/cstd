# Git历险记（五）——Git里的分支＆合并 - weixin_33985507的博客 - CSDN博客
2015年12月27日 19:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
## 分支与合并
在Git里面我们可以创建不同的分支，来进行调试、发布、维护等不同工作，而互不干扰。下面我们还是来创建一个试验仓库，看一下Git分支运作的台前幕后：
$rm -rf test_branch_proj 
$mkdir test_branch_proj 
$cd test_branch_proj 
$git init 
Initialized empty Git repository in /home/test/test_branch_proj/.git/ 
我们如以往一样，创建一个“readme.txt”文件并把它提交到仓库中：
$echo "hello, world" > readme.txt 
$git add readme.txt 
$git commit -m "project init" 
[master (root-commit) 0797f4f] project init  1 files changed, 1 insertions(+), 0 deletions(-)  create mode 100644 readme.txt
我们来看一下工作目录（working tree）的当前状态:
$git status 
# On branch master 
nothing to commit (working directory clean) 
大家如果注意的话，可以看到“# On branch master”这么一行，这表示我们现在正在主分支（master）上工作。当我们新建了一个本地仓库，一般就是默认处在主分支（master）上。下面我们一起看一下Git是如何存储一个分支的：
$cd .git 
$cat HEAD 
ref: refs/heads/master 
“.git/HEAD”这个文件里保存的是我们当前在哪个分支上工作的信息。
在Git中，分支的命名信息保存在“.git/refs/heads”目录下：
$ls refs/heads 
master 
我们可以看到目录里面有一个名叫“master”文件，我们来看一下里面的内容：
$cat refs/heads/master 
12c875f17c2ed8c37d31b40fb328138a9027f337 
大家可以看到这是一个“SHA1哈希串值”，也就是一个对象名，我们再看看这是一个什么类型的对象：
$cat refs/heads/master | xargs git cat-file -t 
commit 
是的，这是一个提交（commit），“master”文件里面存有主分支（master）最新提交的“对象名”；我们根据这个“对象名”就可以可找到对应的树对象（tree）和二进制对象（blob），简而言之就是我能够按“名”索引找到这个分支里所有的对象。
读者朋友把我们文章里的示例在自己的机器上执行时会发现，“cat refs/heads/master”命令的执行结果和和文章中的不同。在本文里这个提交（commit）的名字是： “12c875f17c2ed8c37d31b40fb328138a9027f337”，前面我讲Git是根据对象的内容生成“SHA1哈希串值”作为 名字，只要内容一样，那么的对应的名字肯定是一样的，为什么这里面会不一样呢？　Git确实根据内容来生成名字的，而且同名（SHA1哈希串值）肯定会有 相同内容，但是提交对象（commit）和其它对象有点不一样，它里面会多一个时间戳（timestamp），所以在不同的时间生成的提交对象，即使内容 完全一样其名字也不会相同。
下面命令主是查看主分支最新提交的内容:
$cat refs/heads/master | xargs git cat-file -p 
tree 0bd1dc15d804534cf25c5cb53260fd03c84fd4b9 
author liuhui998 <liuhui998@nospam.com> 1300697913 +0800 
committer liuhui998 <liuhui998@nospam.com> 1300697913 +0800     project init 
“1300697913 +0800”这就是时间戳（timestamp）。
现在查看此分支里面所包含的数据（blob）
$cat refs/heads/master | xargs git cat-file -p | head -n 1 | cut -b6-15 | xargs git cat-file -p 
100644 blob 4b5fa63702dd96796042e92787f464e28f09f17d  readme.txt 
查看当前的readme.txt
$git cat-file -p 4b5fa63 
hello, world 
$cd .. 
好的，前面是在主分支（master）里面玩，下面我们想要创建一个自己的测试分支来玩一下。[git branch](http://www.kernel.org/pub/software/scm/git/docs/git-branch.html)命令可以创建一个新的分支，也可以查看当前仓库里有的分支。下面先创建一个叫“test”的分支： $git branch test　
再来看一下当前项目仓库中有几个分支：
$git branch 
* master   test 
我们现在签出“test”分支到工作目录里：
$git checkout test 
现在再来看一下我们处在哪个分支上：
$git branch   master 
* test 
好的，我们现在在“test”分支里面了，那么我们就修改一下“readme.txt”这个文件，再把它提交到本地的仓库里面支:
$echo "In test branch" >> readme.txt 
$git add readme.txt 
$git commit -m "test branch modified" 
[test 7f3c997] test branch modified  1 files changed, 1 insertions(+), 0 deletions(-) 
当看当前版本所包含的blob：
$git cat-file -p HEAD | head -n 1 | cut -b6-15 | xargs git cat-file -p 
我们现在再像前面一样的看看Git如何存储“test”这个分支的，先来看看“.git/HEAD”这个文件是否指向了新的分支：
$cd .git 
$cat HEAD 
ref: refs/heads/test 
没错，“.git/HEAD”确实指向的“test”分支。再来看看“.git/refs/heads”目录里的内容：
$ls refs/heads 
master 
test 
我们可以看到目录里面多了一个名叫“test”文件，我们来看一下里面的内容：
$cat refs/heads/test 
7f3c9972577a221b0a30b58981a554aafe10a104 
查看测试分支（test）最新提交的内容：
$cat refs/heads/test | xargs git cat-file -p 
tree 7fa3bfbeae072063c32621ff08d51f512a3bac53 
parent b765df9edd4db791530f14c2e107aa40907fed1b 
author liuhui998 <liuhui998@nospam.com> 1300698655 +0800 
committer liuhui998 <liuhui998@nospam.com> 1300698655 +0800      test branch modified 
再来查看此分支里面所包含的数据（blob）：
$cat refs/heads/test | xargs git cat-file -p | head -n 1 | cut -b6-15 | xargs git cat-file -p 
100644 blob ebe01d6c3c2bbb74e043715310098d8da2baa4bf  readme.txt 
查看当前”readme.txt”文件里的内容：
$git cat-file -p ebe01d6 
hello, world 
In test branch 
cd .. 
我们再回到主分支里面：
$git checkout master 
Switched to branch 'master' 
$git checkout master 
$cat readme.txt 
hello, world 
如我们想看看主分支（master）和测试分支（test）之间的差异，可以使用[git diff](http://www.kernel.org/pub/software/scm/git/docs/git-diff.html)命令来查看它们之间的[diff](http://en.wikipedia.org/wiki/Diff)：
$git diff test
diff --git a/readme.txt b/readme.txt 
index ebe01d6..4b5fa63 100644 
--- a/readme.txt 
+++ b/readme.txt 
@@ -1,2 +1 @@  hello, world 
-In test branch 
大家可以以到当前分支与测试分支（test）相比，少了一行内容：“-In test branch”。
如果执行完[git diff](http://www.kernel.org/pub/software/scm/git/docs/git-diff.html)命令后认为测试分支（test）的修改无误，能合并时，可以用[git merge](http://www.kernel.org/pub/software/scm/git/docs/git-merge.html)命令把它合并到主分支（master）中：
$git merge test 
Updating b765df9..7f3c997 
Fast-forward  readme.txt |  1 +  1 files changed, 1 insertions(+), 0 deletions(-) 
“Updating b765df9..7f3c997”表示现在正在更新合并“b765df9”和“7f3c997”两个提交（commit）之间的内容；“b765df9”代表着主分支（master），“7f3c997”代表测试分支（test）。
“Fast-forward”在这里可以理解为顺利合并，没有冲突。“readme.txt | 1 +”表示这个文件有一行被修改，“1 files changed, 1 insertions(+), 0 deletions(-)”，表示这一次合并只有一个文件被修改，一行新数据插入，0　行被删除。
我们现在看一下合并后的“readme.txt”的内容：
$cat readme.txt 
hello, world 
In test branch 
内容没有错，是“master”分支和“test”分支合并后的结果，再用“git status”看一下，当前工作目录的状态也是干净的（clean）。
$git status 
# On branch master 
nothing to commit (working directory clean) 
好的，现在测试分支（test）结束了它的使命，没有存在的价值的，可以用“git branch -d”命令把这个分支删掉：
$git branch -d test 
Deleted branch test (was 61ce004). 
如果你想要删除的分支还没有被合并到其它分支中去，那么就不能用“git branch -d”来删除它，需要改用“git branch -D”来强制删除。
## 如何处理冲突（conflict）
前面说了分支的一些事情，还简单地合并了一个分支。但是平时多人协作的工作过程中，几乎没有不碰到冲突（conflict）的情况，下面的示例就是剖析一下冲突成因及背后的故事：
还是老规矩，新建一个空的Git仓库作试验：
$rm -rf test_merge_proj 
$mkdir test_merge_proj 
$cd test_merge_proj 
$git init 
Initialized empty Git repository in /home/test/test_merge_proj/.git/ 
在主分支里建一个“readme.txt”的文件，并且提交本地仓库的主分支里（master）:
$echo "hello, world" > readme.txt 
$git add readme.txt 
$git status 
# On branch master 
# 
# Initial commit 
# 
# Changes to be committed: 
# (use "git rm --cached <file>..." to unstage) 
# 
# new file: readme.txt 
# 
git commit -m "project init" 
[master (root-commit) d58353e] project init  1 files changed, 1 insertions(+), 0 deletions(-)  create mode 100644 readme.txt 
当看当前版本所包含的blob:
$git cat-file -p HEAD | head -n 1 | cut -b6-15 | xargs git cat-file -p 
100644 blob 4b5fa63702dd96796042e92787f464e28f09f17d  readme.txt 
虽然前面把“readme.txt”这个文件提交了，但是暂存区里还是会暂存一下，直到下次“git add”时把它冲掉：
$git ls-files --stage 
100644 4b5fa63702dd96796042e92787f464e28f09f17d 0 readme.txt 
然后再创建测试分支（test branch），并且切换到测试分支下工作：
$git branch test 
$git checkout test 
Switched to branch 'test' 
再在测试分支里改写“readme.txt”的内容，并且提交到本地仓库中：
$echo "hello, mundo" > readme.txt 
$git add readme.txt 
$git commit -m "test branch modified" 
[test 7459649] test branch modified  1 files changed, 1 insertions(+), 1 deletions(-) 
现在看一下当前分支里的“readme.txt”的“SHA1哈希串值”确实不同了：
$git cat-file -p HEAD | head -n 1 | cut -b6-15 | xargs git cat-file -p 
100644 blob 034a81de5dfb592a22039db1a9f3f50f66f474dd  readme.txt 
暂存区里的东东也不一样了：
$git ls-files --stage 
100644 034a81de5dfb592a22039db1a9f3f50f66f474dd 0 readme.txt  
现在我们切换到主分支（master）下工作，再在“readme.txt”上作一些修改，并把它提交到本地的仓库里面：
$git checkout master 
Switched to branch 'master' 
$git add readme.txt 
echo "hola,world" > readme.txt 
$git add readme.txt 
$git commit -m "master branch modified" 
[master 269ef45] master branch modified  1 files changed, 1 insertions(+), 1 deletions(-) 
现在再来看一下当前分支里的“readme.txt”的“SHA1哈希串值”：
$git cat-file -p HEAD | head -n 1 | cut -b6-15 | xargs git cat-file -p 
100644 blob aac629fb789684a5d9c662e6548fdc595608c002  readme.txt 
暂存区里的内容也改变了：
$git ls-files --stage 
100644 aac629fb789684a5d9c662e6548fdc595608c002 0 readme.txt 
主分支（master） 和测试分支（test）里的内容已经各自改变了（diverged），我们现在用“git merge”命令来把两个分支合一下看看：
$git merge test 
Auto-merging readme.txt 
CONFLICT (content): Merge conflict in readme.txt 
Automatic merge failed; fix conflicts and then commit the result. 
合并命令的执行结果不是“Fast-foward”，而是“CONFLICT”。是的，两个分支的内容有差异，致使它们不能自动合并（Auto-merging）。
还是先看一下工作目录的状态：
$git status 
# On branch master 
# Unmerged paths: 
# (use "git add/rm <file>..." as appropriate to mark resolution) 
# 
# both modified:    readme.txt 
# 
no changes added to commit (use "git add" and/or "git commit -a") 
现在Git提示当前有一个文件“readme.txt”没有被合并，原因是“both modified”。
再看一下暂存区里的内容：
$git ls-files --stage 
100644 4b5fa63702dd96796042e92787f464e28f09f17d 1 readme.txt 
100644 aac629fb789684a5d9c662e6548fdc595608c002 2 readme.txt 
100644 034a81de5dfb592a22039db1a9f3f50f66f474dd 3 readme.txt 
看一下里面的每个blob对象的内容：
$git cat-file -p 4b5fa6 
hello, world 
$git cat-file -p aac629 
hola,world 
$git cat-file -p 034a81 
hello, mundo 
我们不难发现，“aac629”是当前主分支的内容，“034a81”是测试分支里的内容，而“4b5fa6”是它们共同父对象（Parent）里的内容。因为在合并过程中出现了错误，所以Git把它们三个放到了暂存区了。
现在我们再来看一下工作目录里的“readme.txt”文件的内容：
$cat readme.txt 
<<<<<<< HEAD 
hola,world 
======= 
hello, mundo 
>>>>>>> test
“<<<<<<< HEAD“下面就是当前版本里的内容；而“=======”之下，“>>>>>>> test”之上则表示测试分支里与之对应的有冲突的容。修复冲突时我们要做的，一般就是把“ <<<<<<< HEAD”，“=======”和“ >>>>>>> test”这些东东先去掉，然后把代码改成我们想要的内容。
假设我们用编辑器把“readme.txt“改成了下面的内容：
$cat readme.txt 
hola, mundo 
然再把改好的“readme.txt”用“git add”添加到暂存区中，最后再用“git commit”提交到本地仓库中，这个冲突（conflict）就算解决了：
$git add readme.txt 
$git commit -m "fix conflict" 
[master ebe2f18] fix conflict 
这里看起来比较怪异的地方是Git解决了冲突的办法：怎么用“git add”添加到暂存区去，“git add”不是用来未暂存文件的吧，怎么又来解决冲突了。不过我想如果你仔细读过上一篇文章的话就不难理解，因为Git是一个“snapshot”存储系 统，所有新增加的内容都是直接存储的，而不是和老版本作一个比较后存储新旧版本间的差异。
Git里面合并两个版本之间的同一文件，如果两者间内容相同则不作处理，两者间内容不同但是可以合并则产生一个新的blob对象，两者间内容不同但 是合并时产生了冲突，那么我们解决了冲突后要把文件“git add”到暂存区中再“git commit”提交到本地仓库即可，这就和前面一样产生一个新的blob对象。
假设我们对合并的结果不满意，可以用下面的命令来撤消前面的合并：
$git reset --hard HEAD^ 
HEAD is now at 050d890 master branch modified 
从[git reset](http://www.kernel.org/pub/software/scm/git/docs/git-reset.html)[（2）](http://gitbook.liuhui998.com/4_9.html)命令的输出结果可以看到，主分支已经回到了合并前的状态了。
我们再用下面的命令看一下“readme.txt”文件，确认一下文件改回来没有：
$cat readme.txt 
hola,world 
## 小结
由于Git采用了“SHA1哈希串值内容寻值”、“快照存储（snapshot）”等方法, Git中创建分支代价是很小的速度很快；也这是因为如此，它处理合并冲突的方法与众不同。
在这里我想起了“C语言就是汇编（计算机硬件）的一个马甲”这句话，其实Git也就是底层文件系统的一个马甲，只不过它带了版本控制功能，而且更加 高效。Git里有些命令可能不是很好理解（如解决合并冲突用git add），但是对于系统层而言，它是最高效的，就像是C语言的数组下标从0开始一样。
