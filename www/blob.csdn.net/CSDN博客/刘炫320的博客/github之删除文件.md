# github之删除文件 - 刘炫320的博客 - CSDN博客
2017年07月09日 17:00:24[刘炫320](https://me.csdn.net/qq_35082030)阅读数：3482
所属专栏：[github官方指南](https://blog.csdn.net/column/details/16517.html)
# 1. 前言
如果我们学习数据库的话，我们要学的是什么？——“增删查改”。
如果我们学习WebAPI的话，我们要学的是什么？——“增删查改”。
那么，如果我们学习github的话，我们要学的是什么？没错——“增删查改”。
在这一章中，我们将讲解github中的删除的部分命令操作。
# 2. Git rm命令
这时用来删除文件的命令，如同Linux里的命令一样。
`$ git rm file1.txt`
这个命令必须是在提交到版本库中以后才能够执行的,否则的话：
1 如果这个文件没有提交到暂存区，它是不会被发现的：
```
$ git rm file1.txt
fatal: pathspec 'file1.txt' did not match any files
```
2 如果提交到暂存区了以后，还是没办法删除：
```
$ git rm file1.txt
error: the following file has changes staged in the index:
    file1.txt
(use --cached to keep the file, or -f to force removal)
```
3 如果这个文件与版本库中不一致，就删除不了，除非强制删除，如下图所示：
```
$ git rm index2.html
error: the following file has staged content different from both the file and the HEAD:
    index2.html
(use -f to force removal)
```
如果你只有一个文件想要删除，使用这个命令可以真正的从文件系统中删除了文件，并且它会暂存这个文件已经被删除的事实，如果你提交了，这个文件不会从之前的历史中消失，但会从未来的提交中消失。
## 2.1 参数–cached
如果你想删除这样一个文件，不想从文件系统中真正的删除它，就是用下面的命令：
`$ git rm --cached file3.txt`
换个说法，你想告诉Git，不再跟踪这个文件，但是把它保留在工作树中，这可能是一种情况，你以外地提交了一个文件，你并不想让他成为历史的一部分。它暂存了删除，但是，他在那里保存了文件，现在，在你的工作树中，作为一个不被追踪的文件。
# 4. 小结
在这一章中，我们讲解了如何删除文件的操作，当然还有像`git ignore`这种操作我们将会在另一讲中讲解。下一章我们将介绍git的文件的修改，下章见。
