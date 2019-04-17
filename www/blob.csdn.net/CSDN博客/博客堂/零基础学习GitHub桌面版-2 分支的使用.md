# 零基础学习GitHub桌面版-2 分支的使用 - 博客堂 - CSDN博客





2017年09月01日 21:53:37[最小森林](https://me.csdn.net/u012052268)阅读数：890
所属专栏：[零基础学习GitHub桌面版](https://blog.csdn.net/column/details/17109.html)









![](http://ww1.sinaimg.cn/large/006qDjsOgy1g0tdplblplj30ne07q7cc.jpg)




### 文章目录
- [分支的使用](#_4)
- [1 创建分支](#1__14)
- [2 切换分支](#2__42)
- [3 上传/同步分支](#3__51)
- [4 删除分支](#4__55)
- [5 合并两个分支Merged](#5_Merged_66)




# 分支的使用

创建分支的目的在于删除分支！

当你对某些修改不确定，不想在主分支(master)上修改，就可以新建一个新分支（类似于复制一个分身）。
- 改的不好，或者不想修改了，就把这个分支（分身）删掉。
- 改的好，就可以合并到主分支(master)，进而替代原来的主分支。

所以分支的主要作用是团队合作。也可以用来给别人的库 提意见，别人觉得你的修改很好，就把你的替代到主分支。

下面是详细步骤：

## 1 创建分支

我们创建第一个分支取名为“my-new-branch”,点击Create new branch创建第一个分支。

![](http://ww1.sinaimg.cn/large/006qDjsOly1g0tednwlmoj30qo0ic0u0.jpg)

我们发现此时的分支已经切换到了我们刚刚创建的分支my-new-branch

![](http://ww1.sinaimg.cn/large/006qDjsOly1g0teeirbwaj30qo0icmyb.jpg)

此时，我们修改文件夹中的文件不会影响主分支，相当于创建了一个副本。

我们来修改my-new-branch分支上的内容。

我们仍旧打开FirstDemo.txt进行编辑。输入以下内容

> 
创建的第一个分支。


打开github进行，填写Summary和Description

![](http://ww1.sinaimg.cn/large/006qDjsOly1g0teiz11bgj30qo0icwfd.jpg)

之后我们点击Commit to my-new-master 发布一个版本；

## 2 切换分支

点击图片左上角部分就会出现分支的列表

![](http://ww1.sinaimg.cn/large/006qDjsOly1g0ter63bkoj30qo0icmyy.jpg)

我们点击master就会切换到master主分支。

当你转换分支的时候，本地文件也会跟着变化。也就是说，刚刚在my-new-master版本下做的改变，目前完全不影响主分支。

## 3 上传/同步分支

这个操作和同步仓库是一个操作，点击Publish/Sync上传或同步分支。

## 4 删除分支

首先要把分支切换到你要删除的分支下，如我们要删除new master，将分支切换到new master点击branch菜单栏就会出现Delete

![](http://ww1.sinaimg.cn/large/006qDjsOly1g0tes80b02j30qo0icabu.jpg)

点击Delete new master就会弹出一个对话框，询问删除的内容。

![](http://ww1.sinaimg.cn/large/006qDjsOly1g0tesu1qhfj30qo0icq4t.jpg)

勾选删除remote（远程），同时删除本地 和远程网页分支。

## 5 合并两个分支Merged

当你觉的这个分支不错，可以将其合并（Merged）到主分支中，然后删除这个分支。

将一个分支与master分支进行合并。
- 我们首先把分支切换到master下，
- 点击branch菜单–>点击 merge into current branch 进行把新建的分支 合并到主分支。

![](http://ww1.sinaimg.cn/large/006qDjsOly1g0tex7sthyj30qo0icdhr.jpg)

此时我们查看history目录下就会显示多了一个合并版本。

整个流程如下图所示：

![image](http://www.runoob.com/wp-content/uploads/2014/05/branches.png)



