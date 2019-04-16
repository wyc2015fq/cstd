# git代码版本控制 - youbingchen的博客 - CSDN博客





2016年05月26日 13:53:06[youbingchen](https://me.csdn.net/youbingchen)阅读数：464标签：[git](https://so.csdn.net/so/search/s.do?q=git&t=blog)
个人分类：[git](https://blog.csdn.net/youbingchen/article/category/6246384)









事先申明一下：网上很有很多git的教程，我写这个博客并不是自己有什么独特的东东，只是为了复习git，因为git上有些东西长久不用，有些忘记，所以借此机会复习一下，如果大家想学这个东东的话（我非常推荐廖雪峰的博客）（本人也是引用这个博客比较多的东西） 
[git教程](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/0013744142037508cf42e51debf49668810645e02887691000)

# 工作区和版本库

工作区就是电脑能看到的目录（文件夹） 

版本库（在工作区有一个隐藏目录`.git`） 

git的版本库里面存了很多东西，其中最重要的就是称为`stage`(或者称为`index`)的暂存区，还有`Git`为我们自动创建的第一个分支`master`，以及指向`master`的一个指针为`HEAD`
![这里写图片描述](https://img-blog.csdn.net/20160526144122451)
```
git add something   // 实际上把文件添加到暂存区（第一步）
git commit -m "say something" //提交更改，实际上就是把暂存区的所有内容提交到当前分支
```

因为我们创建`Git`版本库时，`Git`自动为我们创建了唯一一个`master`分支，所以，现在，`git commit`就是往`master`分支上提交更改。

# 撤销修改
`git checkout -- 文件名  // 只是撤销工作区的修改`
命令`git checkout -- 文件名`意思就是把`文件名`在工作区的修改全部撤销，这里有两种情况： 

1. `文件`自修改后还没有添加到暂存区，撤销修改就回到和版本库一模一样的状态 

2. `文件`已经被添加到暂存区，又作了修改，撤销修改就回到添加到暂存区后的状态 
**总结：就是让文件回到最近一次`git commit`或`git add`时的状态**
如果在工作区修改后，又提交到暂存区这时候可以使用命令
`git reset HEAD file //可以把暂存区的修改撤销掉，重新放回工作区 接下来可以采用上面撤销工作区的修改`
如果把修改之后的内容提交`commit` 但是还没`push`到远程库可以采用版本回退
`git reset -- hard commit_id`- `HEAD`指向的版本就是当前版本，使用命令`git reset -- hard commit_id`
- 在版本回退之前，可以使用`git log`查看提交命令，确定要回退到哪个版本
- 也可以重返未来，用`用 git relog`查看命令历史，以便确定要回到未来哪个版本

# 删除文件

在工作区
`rm 文件`
接下来有两种选择 

1.确实删除
`git rm 文件`
2.工作区删除错误（这很适合那些粗心的同学）
`git checkout -- 文件 //恢复工作区文件（建议在一些重要文件夹下采用git，备份，以免误删）`
# 创建与合并分支

在`Git`里，主分支被称为`master`分支。`HEAD`严格来说并不是指向提交，而是指向`master`，`master`才是指向提交的，所以`HEAD`指向当前分支 
![这里写图片描述](https://img-blog.csdn.net/20160526140931373)

当我们创建的分支，例如`dev`,指向`master`相同的提交，再把HEAD指向`dev`,就表示当前分支在`dev`
![这里写图片描述](https://img-blog.csdn.net/20160526142922044)
`Git`创建一个分支很快，因为除了增加一个`dev`指针，改改`HEAD`的指向，工作区的文件都没有任何变化！ 

从现在开始，对工作区的修改和提交就是针对`dev`分支了，比如新提交一次后，`dev`指针往前移动一步，而`master`指针不变： 
![这里写图片描述](https://img-blog.csdn.net/20160526143036381)

加入我们在`dev`上的工作完成了，就可以把`dev`合并在`master`,怎么合并呢，最简单的方法就是直接把`master`指向的当前提交，就完成合并： 
![这里写图片描述](https://img-blog.csdn.net/20160526143204532)

所以`Git`合并分支也很快！就改改指针，工作区内容也不变！
合并完分支后，甚至可以删除`dev`分支。删除`dev`分支就是把`dev`指针给删掉，删掉后，我们就剩下了一条`master`分支： 
![这里写图片描述](https://img-blog.csdn.net/20160526143454536)

首先，我们创建`dev`分支，然后切换到分支：
```
git branch //查看当前分支
git checkout -b dev
```

`git check`命令加上-b参数表示创建并切换，相当于一下两条命令

```
git branch dev
git checkout dev
```

然后在`dev`分支上修改，提交

```
git add
git commit -m ""
```

现在，`dev`分支的工作完成，就可以切换回`master`分支
`git checkout master`
此时的状态为： 
![这里写图片描述](https://img-blog.csdn.net/20160526143708024)

现在,把`dev`分支的工作成果合并于`master`分支上：`git merge dev`
`git merge`命令用于合并指定分支到当前分支 

现在可以安心删除`dev`分支

```bash
git branch -d dev
```

# 解决冲突

假设`master`分支和`feature1`分支各自分别有新的提交 
![这里写图片描述](https://img-blog.csdn.net/20160526143807759)
**这时，不能直接利用`git merge`直接合并 解决方案就是在`git merge`之后，再利用`git add `和`git commit -m ""`用带参数的git log也可以看到分支的合并情况：`git log -- graph --pretty=oneline --abbrev-commit`![这里写图片描述](https://img-blog.csdn.net/20160526143938260)# 分支策略在实际开发中，我们应该按照几个基本原则进行分支管理：首先，`master`分支应该是非常稳定的，也就是仅用来发布新版本，平时不能在上面干活；那在哪干活呢？干活都在`dev`分支上，也就是说，`dev`分支是不稳定的，到某个时候，比如1.0版本发布时，再把`dev`分支合并到`master`上，在`master`分支发布1.0版本；你和你的小伙伴们每个人都在`dev`分支上干活，每个人都有自己的分支，时不时地往`dev`分支上合并就可以了。所以，团队合作的分支看起来就像这样: ![这里写图片描述](https://img-blog.csdn.net/20160526144100885)# 多人协作**多人协作时，大家都会往`master`和`dev`分支上推送各自的修改。你的小伙伴已经向`origin/dev`分支推送了他的提交，而碰巧你也对同样的文件作了修改，并试图推送：推送失败，因为你的小伙伴的最新提交和你试图推送的提交有冲突，解决办法也很简单，`Git`已经提示我们，先用`git pull`把最新的提交从origin/dev抓下来，然后，在本地合并，解决冲突，再推送：**如果`git pull`也失败了，原因是没有指定本地`dev`分支与远程`origin/dev`分支的链接，根据提示，设置`dev`和`origin/dev`的链接.`git branch --set-upstream dev origin/dev`再pull:`git pull``git pull`成功，但是合并有冲突，需要手动解决，解决的方法和分支管理中的解决冲突完全一样。解决后，提交，再push：# 小结- 查看远程库信息，使用`git remote -v`- 本地新建的分支如果不推送到远程，对其他人就是不可见的；- 从本地推送分支，使用`git push origin branch-name`，如果推送失败，先用`git pull`抓取远程的新提交；- 在本地创建和远程分支对应的分支，使用`git checkout -b branch-name origin/branch-name`，本地和远程分支的名称最好一致；- 建立本地分支和远程分支的关联，使用`git branch --set-upstream branch-name origin/branch-name`；- 从远程抓取分支，使用`git pull`，如果有冲突，要先处理冲突。**








