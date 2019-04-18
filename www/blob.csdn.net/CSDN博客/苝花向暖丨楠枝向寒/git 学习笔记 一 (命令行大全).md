# git 学习笔记 一 (命令行大全) - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月17日 23:51:35[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：42


**Table of Contents**

[杂项](#%E6%9D%82%E9%A1%B9)

[删除文件](#%E5%88%A0%E9%99%A4%E6%96%87%E4%BB%B6%C2%A0)

[恢复删除的文件](#%E6%81%A2%E5%A4%8D%E5%88%A0%E9%99%A4%E7%9A%84%E6%96%87%E4%BB%B6%C2%A0)

[添加文件](#%E6%B7%BB%E5%8A%A0%E6%96%87%E4%BB%B6%C2%A0)

[回退或撤回到某版本](#%E5%9B%9E%E9%80%80%E6%88%96%E6%92%A4%E5%9B%9E%E5%88%B0%E6%9F%90%E7%89%88%E6%9C%AC%C2%A0)

[在github上创建远程仓库](#%E5%9C%A8github%E4%B8%8A%E5%88%9B%E5%BB%BA%E8%BF%9C%E7%A8%8B%E4%BB%93%E5%BA%93)

[从github将代码下载到本地](#%E4%BB%8Egithub%E5%B0%86%E4%BB%A3%E7%A0%81%E4%B8%8B%E8%BD%BD%E5%88%B0%E6%9C%AC%E5%9C%B0)

[创建与合并分支](#%E5%88%9B%E5%BB%BA%E4%B8%8E%E5%90%88%E5%B9%B6%E5%88%86%E6%94%AF)

[解决冲突](#%E8%A7%A3%E5%86%B3%E5%86%B2%E7%AA%81)

[分支策略](#%E5%88%86%E6%94%AF%E7%AD%96%E7%95%A5)

[Bug分支](#Bug%E5%88%86%E6%94%AF)

![](https://img-blog.csdn.net/20180917134241582?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 杂项

$ git config --global user.name "Your Name"       设置用户名

$ git config --global user.email "email@example.com"    设置邮箱

$ git config --global color.ui true   使git命令执行结果 颜色区别更大

$ git init     把文件夹变成仓库

$ ls -ah     ah参数可查看到当前目录下的隐藏文件

$ git status      查看状态，主要用来查看是否有文件被修改且没有被提交

$ git diff readme.txt    暂存区为空，就会比较工作区和分支的区别，暂存区不为空，就比较较暂存区和工作区的区别

$ git diff HEAD -- readme.txt     查看工作区和版本库里面最新版本的区别

$ cat readme.txt      查看文本内容

### **删除文件 **

如下：

$ rm test.txt     删除工作空间中的文件

$ git rm test.txt    将删除信息放到 暂存区中

$ git commit -m "remove a file"    将删除信息提交到分支，从而将分支中的文件也删除

### **恢复删除的文件 **

如下：

$ git checkout -- <file name> 

$ git reset HEAD a.txt

如果你仅仅执行了  $ rm test.txt  那么就可以直接通过 $ git checkout -- <file name> 恢复工作空间中被删除的文件

如果你执行了$ rm test.txt 又 执行了$ git rm test.txt  ，即将删除信息放入暂存区，那么想要恢复就必须先执行

$ git reset HEAD a.txt  将暂存区的内容与分支的信息统一    再执行   $ git checkout -- <file name> 

### **添加文件 **

如下：

$ git add readme.txt      将工作区文件放到git的暂存区

$ git commit -m "wrote a readme file"      将暂存区的文件提交到分支上

### **回退或撤回到某版本 **

如下：

$ git log     查看提交记录，显示结果中的一大长串数字和字母的结合代表commit id，通过指定commit id可以回退到指定版本

$ git log --pretty=oneline       git log 命令显示内容的简化版

$ git reflog   如果窗口被关了，再次打开 git log将无法显示提交记录，如想查看commit id 可使用 git reflog 会显示提交和回退记录

$ git reset --hard HEAD^      回退到 上一个版本， HEAD^^ 代表上上个版本，再往上用数字表示，如：HEAD~100

$ git reset --hard 3f291        根据commit id回退或撤回到指定版本，注意commit id 不用写全，写 5个左右就可以自动找到

### **在github上创建远程仓库**

如下：

首先在github上创建一个新仓库，注意如果是向仓库中上传代码则不需要勾选** Initialize this repository with a README **选项

$ ssh-keygen -t rsa -C "youremail@example.com"          替换成自己注册github所用的邮箱

一直按回车即可

生成的私钥和公钥 在 C:\Users\user\.ssh 目录下

公钥的文件后缀是 pub，把公钥粘贴到github的 SSH setting中, 然后在github上建立仓库

然后就可以通过如下命令将本地git仓库与远程的 github建立连接并上传代码

$ git remote add origin git@github.com:aaa/bbb.git          aaa 替换成 github账户名， bbb 替换成 github建立的仓库名

$ git push -u origin master      将本地仓库内容传到github上面，第一次提交需要加上 -u 参数，且第一次会出现如下提示

```bash
$ git push -u origin master
The authenticity of host 'github.com (xxx.xxx.xxx.xxx)' can't be established.
RSA key fingerprint is xxx.
Are you sure you want to continue connecting (yes/no)?
```

其目的是让你确认github的公钥是不是和你的一致

直接输入yes 就行

之后本地代码文件有更新，直接执行

git push origin master 就会进行同步

### **从github将代码下载到本地**

如下：

$ git clone git@github.com:aaa/bbb.git

aaa 替换为 github用户名，bbb 替换为 github 仓库名

第一次使用 clone 也会出现如下提示

```bash
$ git clone git@github.com:aaa/bbbb.git
Cloning into 'gitskills'...
The authenticity of host 'github.com (xxx.xxx.xxx.xxx)' can't be established.
RSA key fingerprint is xxx.
Are you sure you want to continue connecting (yes/no)?
```

输入yes即可 

命令执行完毕以后就会将 代码下载到当前所在目录。

### **创建与合并分支**

$ git checkout -b dev      创建名为 dev的分支，创建完分支以后 dev 会与master 指向同一个提交点，Head指向dev

上面的代码相当于下面两行命令

```
$ git branch dev
$ git checkout dev
```

其实 checkout 命令本身的含义是 切换分支，但是加上不同的参数有不同的效果，如上面 git chekout -- <file name> 恢复文件

创建分支之前

![](https://img-blog.csdn.net/20180918093057979?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

创建分支之后

![](https://img-blog.csdn.net/20180918093126109?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

$ git branch   查看当前分支

你必须在当前分支提交过文件这个命令才可以显示出当前分支，还有如果你在仓库为空的情况下去创建新的分支，那么原来的master分支会消失。

提交任务以后指针如下

![](https://img-blog.csdn.net/20180918093201690?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过 $ git checkout master 可以再切换回master分支，如下

![](https://img-blog.csdn.net/20180918093304183?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在 master分支下 执行 $ git merge dev ，将dev与master进行合并 如下

```bash
$ git merge dev
Updating 12d3c01..3b535e2
Fast-forward
 a.txt | 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)
```

![](https://img-blog.csdn.net/20180918093335393?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

即将master的指针指向 dev所执行的提交点，上面有一行Fast-forward代表合并是“快进模式”，即上述这种通过直接移动指针来达到合并效果。

在通过 $ git branch -d dev 将 dev删除 如下

![](https://img-blog.csdn.net/20180918093356170?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

但是 Fast-forward 有一个问题，就是如上图所示，分支信息丢失。我们通过 $ git log --graph --pretty=oneline --abbrev-commit  命令可以查看分支合并信息。如果说想保留分支信息的话，我可以通过 $ git merge --no-ff -m "merge with no-ff" dev 命令来禁用 Fast-forward 模式，效果如下图所示

![](https://img-blog.csdn.net/20180918093445724?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### **解决冲突**

在master下创建一个分支 feature1，切换到feature1，修改文件，然后提交，再切换为master，修改文件，然后提交，在合并dev，就会出现冲突，因为分别改动了文件，改动又不一样，所以git不知道该如何合并，这时需要我们手动解决。

打开 文件 

![](https://img-blog.csdn.net/20180917222459637?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

git 会给我们显示出来 两次 改动的 区别，我们改为dev的结果，然后再重新 add 和 commit 提交就行了.

图片演示分支状态如下：

maseter提交后解决冲突前是这样

![](https://img-blog.csdn.net/20180918093613708?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 解决冲突以后再提交是这样

![](https://img-blog.csdn.net/20180918093652603?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

$ git log --graph --pretty=oneline --abbrev-commit   查看分支合并信息

由于我们再次通过commit 提交了文件，且最终结果以feature1的改动为准，所以最终 master又指向了feature1分支。

如果是上面的 Fast-forward(快进模式) 合并的，那么通过 $ git log --graph --pretty=oneline --abbrev-commit 查看到的就是一条直线，看不到feature1的信息

### **分支策略**

在实际开发中，我们应该按照几个基本原则进行分支管理：

首先，`master`分支应该是非常稳定的，也就是仅用来发布新版本，平时不能在上面干活；

那在哪干活呢？干活都在`dev`分支上，也就是说，`dev`分支是不稳定的，到某个时候，比如1.0版本发布时，再把`dev`分支合并到`master`上，在`master`分支发布1.0版本；

你和你的小伙伴们每个人都在`dev`分支上干活，每个人都有自己的分支，时不时地往`dev`分支上合并就可以了。

所以，团队合作的分支看起来就像这样：

![](https://img-blog.csdn.net/2018091809375846?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### **Bug分支**

先叙述一个场景，我们在master上创建了一个dev分支，用来自己开发，这是测试说了，你合并到master上的代码有bug，你得马上改一下，此时我们就需要放下当前手头的事，然后切换到master分支上，再创建一个editbug分支，用来改bug，改完以后再切换回master分支，进行合并，再继续做手头的工作。

代码示意如下

其中   $ git stash  用来保存当前工作状态

          $ git stash list  查看保存的工作状态

          $ git stash pop  恢复工作状态同时删除 list 中的内容

          $ git stash apply  恢复工作状态

          $ git stash drop  删除 list中的内容

```bash
$ git checkout -b dev
Switched to a new branch 'dev'

$ git status
On branch dev
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

        modified:   a.txt

no changes added to commit (use "git add" and/or "git commit -a")

$ git stash
Saved working directory and index state WIP on dev: 9f88915 commit

$ git checkout master
Switched to branch 'master'

$ git checkout -b editbug
Switched to a new branch 'editbug'

$ git status
On branch editbug
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

        modified:   a.txt

no changes added to commit (use "git add" and/or "git commit -a")

$ git add a.txt

$ git commit -m "resolve"
[editbug 987f590] resolve
 1 file changed, 1 insertion(+), 1 deletion(-)

$ git checkout master
Switched to branch 'master'

$ git merge --no-ff -m "merged bug" editbug
Merge made by the 'recursive' strategy.
 a.txt | 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

$ git checkout dev
Switched to branch 'dev'

$ git stash list
stash@{0}: WIP on dev: 9f88915 commit

$ git stash pop
On branch dev
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

        modified:   a.txt

no changes added to commit (use "git add" and/or "git commit -a")
Dropped refs/stash@{0} (8dbc8e4c9f4a746b81e4c387e55acafd6c0eb2dc)

$ git add a.txt

$ git commit -m "commit"
[dev 6e36c48] commit
 1 file changed, 1 insertion(+), 1 deletion(-)

$ git checkout master
Switched to branch 'master'

$ git merge dev
Auto-merging a.txt
CONFLICT (content): Merge conflict in a.txt
Automatic merge failed; fix conflicts and then commit the result.
```

到此为止 我们已经修改完bug并提交，然后回过把自己之前的工作完成并提交，再回到master进行合并，然后产生了冲突。

冲突内容如下 

![](https://img-blog.csdn.net/20180917232522759?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们保留dev的结果

![](https://img-blog.csdn.net/20180917232614385?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 提交代码

```bash
$ git status
On branch master
You have unmerged paths.
  (fix conflicts and run "git commit")
  (use "git merge --abort" to abort the merge)

Unmerged paths:
  (use "git add <file>..." to mark resolution)

        both modified:   a.txt

no changes added to commit (use "git add" and/or "git commit -a")

$ git add a.txt

$ git commit -m 'resolove conflict'
[master 542a0a3] resolove conflict
```

查看分支图

![](https://img-blog.csdn.net/2018091723324681?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

$ git stash pop  和 $ git stash apply  支持 带参数操作，如果我们保存了多个工作状态，可以指定恢复到某个。

如   $ git stash pop stash@{0}

而且 恢复工作状态，实际上也是一个合并操作，也会产生冲突，看如下示例

我分别在 a.txt 末尾 加了 数字 1 然后保存了一次，把1换成2又保存了一次

```
$ git stash list
stash@{0}: WIP on dev: 6e36c48 commit
stash@{1}: WIP on dev: 6e36c48 commit
```

   然后我先 运行代码

```bash
$ git stash pop stash@{0}
On branch dev
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

        modified:   a.txt

Untracked files:
  (use "git add <file>..." to include in what will be committed)

        a.txt.bak

no changes added to commit (use "git add" and/or "git commit -a")
Dropped stash@{0} (4ef10dd44d0ccd929f3a99ec0850eb25b51efcb8)
```

 恢复到了之前的工作状态，在运行代码

```bash
$ git stash list
stash@{0}: WIP on dev: 6e36c48 commit
```

  可以看出 stash@{1} 由于 stash@{0} 恢复了，其编号变成了   stash@{0}，再运行代码

```bash
$ git stash pop stash@{0}
error: Your local changes to the following files would be overwritten by merge:
        a.txt
Please commit your changes or stash them before you merge.
Aborting
```

它提示我们本地仓库中的文件被修改后还未提交，请先提交

```bash
$ git status
On branch dev
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

        modified:   a.txt

Untracked files:
  (use "git add <file>..." to include in what will be committed)

        a.txt.bak

no changes added to commit (use "git add" and/or "git commit -a")

$ git add a.txt

$ git commit -m 'commit'
[dev 185acdd] commit
 1 file changed, 1 insertion(+), 1 deletion(-)
```

提交过后再次 恢复工作状态

```bash
$ git stash list
stash@{0}: WIP on dev: 6e36c48 commit

$ git stash pop stash@{0}
Auto-merging a.txt
CONFLICT (content): Merge conflict in a.txt
```

提示冲突了，查看一下文件

![](https://img-blog.csdn.net/20180917234849644?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

综上所述，恢复工作状态，实际上也是一种合并。

