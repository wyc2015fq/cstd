# git 删除远程分支 - z69183787的专栏 - CSDN博客
2015年05月21日 14:53:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：574
个人分类：[版本管理-Git](https://blog.csdn.net/z69183787/article/category/2857957)
一不小心把本地的临时分支push到server上去了，想要删除。
一开始用
```
git branch -r -d origin/branch-name
```
不成功，发现只是删除的本地对该远程分支的track，正确的方法应该是这样：
```
git push origin :branch-name
```
冒号前面的空格不能少，原理是把一个空分支push到server上，相当于删除该分支。
在Git v1.7.0 之后，可以使用这种语法删除远程分支：
$ git push origin --delete <branchName>
删除tag这么用：
git push origin --delete tag <tagname>
