# Github第一个仓库 - 刘炫320的博客 - CSDN博客
2017年07月05日 22:54:58[刘炫320](https://me.csdn.net/qq_35082030)阅读数：199
所属专栏：[github官方指南](https://blog.csdn.net/column/details/16517.html)
# 1. 前言
今天我们来讲解如何创建第一个仓库以及如何commit（提交）。
# 2. 第一个仓库
仓库说白了就是工程文件夹，里面包含了工程文件以及说明文档。它分为本地仓库和网上仓库，本地仓库适合自己编程使用，网上仓库则是同其他人协作一起开发使用。
## 2.1 本地仓库
先说本地仓库，本地仓库的创建比较简单，进入到要创建仓库的文件夹，无论是有无工程，均可创建。如果是目标文件夹下就是我们的仓库位置，则只需要进入到文件夹中，右键打开git bash，然后输入以下命令：
`$ git init`
即可完成本地仓库的创建，同时在该文件夹下有一个隐藏的.git文件夹，里面包含了git的内容。有兴趣的同学可以自己查阅。 
如果是要在该文件夹下新创建一个工程文件夹，例如创建一个“website”的工程，则只需下面命令：
`$ git init website`
这样可以看到在这个文件夹下多出了一个website的文件夹。 
当然，如果我们使用eclipse、pycharm和VS的时候，编译器都会询问是否创建一个git工程，点击是就可以创建一个本地项目仓库了。
## 2.2 网上仓库
网上仓库更加简单，只需要登录github.com，然后点击右上角的加号，选择”new repository”即可，然后输入项目名点击创建即可完成，创建后，会弹出一个帮助文档，告诉你如何创建一个新的仓库以及push一个现有的仓库或者是从其他仓库中导入代码等。
# 3 commit的三种方式
## 3.1 第一种方式命令行
命令行的方式是在本地进行操作的。只需要打开你的本地仓库，然后打开gitbash窗口。首先应当使用以下命令查看当前的状态：
`$ git status`
如果你没有任何改动的话，应当是：
```
On branch master
Initial commit
nothing to commit (create/copy files and use "git add" to track)
```
而如果是有改动的话，例如我改动了README.md和index.html文件，则就会是这样子的。
```
On branch master
Initial commit
Untracked files:
  (use "git add <file>..." to include in what will be committed)
        README.md
        index.html
nothing added to commit but untracked files present (use "git add" to track)
```
此时，我们就需要使用`git add`命令来把这些改动添加到“暂存区”中，github有一个暂存区，用来审批已经改动后的文件。于是就有下面这个操作：
```
$ git add index.html
$ git add README.md
```
看起来没有任何回应，但是没有关系，我们再次使用`git status`命令查看后，就会发现已经发生了变化：
```
On branch master
Initial commit
Changes to be committed:
  (use "git rm --cached <file>..." to unstage)
        new file:   README.md
        new file:   index.html
```
原来的红色字也变成了绿色字了，还记得我们上一章中讲的颜色吗？而且github还非常贴心的给出了如何把添加到暂存区里的文件再撤销：
`git rm --cached <file>...`
## 3.2 第二种方式网站和第三种方式桌面版
如果要从网站上提交和从桌面版提交就很easy的多了，只需要点击要修改的文件，修改完毕后，在后面写上本次修改的内容，点击提交即可。桌面版也是一样，因此很容易，我们不再赘述了。
# 4 小结
本章中我们主要是介绍了如何创建一个仓库以及如何进行提交，记住了这里只是提交到暂存区，还是有撤销的余地的。在下面一讲中，我们将介绍真正的与其他人合作的相关操作。 
感谢《[github官方视频教程](https://www.nowcoder.com/courses/2/1/4)》,另外推荐一个github学习指南：《[git教程](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000)》。供大家学习和参考。
