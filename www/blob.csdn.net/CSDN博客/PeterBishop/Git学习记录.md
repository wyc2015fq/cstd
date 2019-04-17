# Git学习记录 - PeterBishop - CSDN博客





2018年08月23日 22:59:25[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：50








# 目录
- [Git学习记录 力做全网最强入门教程](https://www.cnblogs.com/Chicago/p/9527027.html#git%E5%AD%A6%E4%B9%A0%E8%AE%B0%E5%BD%95-%E5%8A%9B%E5%81%9A%E5%85%A8%E7%BD%91%E6%9C%80%E5%BC%BA%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B)- [什么是GitHub?](https://www.cnblogs.com/Chicago/p/9527027.html#%E4%BB%80%E4%B9%88%E6%98%AFgithub1)- [什么是Git？](https://www.cnblogs.com/Chicago/p/9527027.html#%E4%BB%80%E4%B9%88%E6%98%AFgit2)

- [Git的配置](https://www.cnblogs.com/Chicago/p/9527027.html#git%E7%9A%84%E9%85%8D%E7%BD%AE)- [Git的安装(只介绍windos操作系统下)](https://www.cnblogs.com/Chicago/p/9527027.html#git%E7%9A%84%E5%AE%89%E8%A3%85%E5%8F%AA%E4%BB%8B%E7%BB%8Dwindos%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E4%B8%8B)
- [Git的配置](https://www.cnblogs.com/Chicago/p/9527027.html#git%E7%9A%84%E9%85%8D%E7%BD%AE-1)

- [至此我们的入门教程到此结束，更新中级教程要等欧](https://www.cnblogs.com/Chicago/p/9527027.html#%E8%87%B3%E6%AD%A4%E6%88%91%E4%BB%AC%E7%9A%84%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B%E5%88%B0%E6%AD%A4%E7%BB%93%E6%9D%9F%E6%9B%B4%E6%96%B0%E4%B8%AD%E7%BA%A7%E6%95%99%E7%A8%8B%E8%A6%81%E7%AD%89%E6%AC%A7)


# Git学习记录 力做全网最强入门教程

## 什么是[GitHub](https://baike.baidu.com/item/GitHub)?

若您在不知道Git是什么的情况下，一上来就想知道GitHub是什么肯定会一头雾水的。

### 什么是[Git](https://baike.baidu.com/item/Git/12647237)？

如果要用一句话来概述：Git是目前世界上最先进的分布式版本控制系统（没有之一）

相信您还是一头雾水，->“分布式...控制系统”是什么鬼？下面我们来举个例子。
- 当您写论文的时候是不是会无限的改来改去，于是就有了一个个副本![](https://cdn.liaoxuefeng.com/cdn/files/attachments/0013848606651673ff1c83932d249118bf8fd5c58c15ca2000/0)
- 我们写程序的时候当然也是这样的一个过程，但是我们又不希望看到一个个凡人的副本，于是git便可以实现这样一个功能![](https://i.loli.net/2018/08/23/5b7eb3fcb8817.png)
- 这仅是Git的一个功能呢
- 隔壁站台给了很好的讲解右转高速列车->[GitHub](https://mp.weixin.qq.com/s/5ED1mPEUAilHBEIdFeOFYQ)

	了解了这些相信您现在已经迫不及待上车了

## Git的配置

### Git的安装(只介绍windos操作系统下)

前置技能:会电脑的极本操作
- 从官方下载对应您的版本(32 or 64) [https://git-scm.com/download/win](https://git-scm.com/download/win) (这里会自动下载)
- 列表项
- 如果没有错的话，您打开下载完的文件是这个样子
![](https://i.loli.net/2018/08/23/5b7eb7fc40bd4.png)
- 之后一路next就行了
- 您电脑正常的话，win栏里会出现这个东西
![](https://i.loli.net/2018/08/23/5b7eb8b5b0fda.png)
- 打开Git Bash，没有意外，安装完成

### Git的配置

前置技能：>1. 安装好了Git >2.会打代码
- 打开Git Bash，`cd`到你想创建Git的呢个目录
- 输入`pwd`查看目录是否正确

	如图：![](https://i.loli.net/2018/08/23/5b7ebaafca854.png)
- 如果目录正确，再输入指令`git init`便创建成功了
- 这时候我们用notepad++在当前目录创建一个hello_git.txt文档来测试

第一次编辑

hello git!

I am coming.
- 我们现在保存到Git缓存区中 `git add hello_git.txt`
- 键入指令`git commit`完成上传
![](https://i.loli.net/2018/08/23/5b7ebe5c8f3fc.png)
- 这是您可能会好奇`-m 。。。`是什么，请您忍住好奇，我们先往下
- 列表项

这是我们编辑第二遍

```
第二次编辑
1
hello git!
I am coming.

2
//git add hello_git
//git commit -m "我随便写写"
我们现在在改一下这个文档
来试验一下git

初始化一个Git仓库，使用git init命令。
添加文件到Git仓库，分两步：
使用命令git add <file>，注意，可反复多次使用，添加多个文件；
使用命令git commit -m <message>，完成。
```
- 此时我们键入指令`git status`效果如下图，注意红色区域
![](https://i.loli.net/2018/08/23/5b7ec022e6f21.png)

	这是的`git status`作用
- 
它提示我们，红色区域的呢个文档没有保存

git add hello git.txt

	git commit -m "增加一些操作过的指令"

- 我们现在在学一个比较实用的，也是git的特点`git log`
![](https://i.loli.net/2018/08/23/5b7ec2e568847.png)
- 
这时您知道前面 -m 的操作是干嘛了么


## 至此我们的入门教程到此结束，更新中级教程要等欧



