# faial: refusing to merge unrelated histories - 博客堂 - CSDN博客





2017年05月03日 10:03:04[最小森林](https://me.csdn.net/u012052268)阅读数：1790








**本文解决在git中遇到的问题**：

faial: refusing to merge unrelated histories

**原因**：

在首次提交代码之前，必须首先pull一下。但是因为两个仓库不同，发现refusing to merge unrelated histories，无法pull。

这是因为他们是两个不同的项目，要把两个不同的项目合并，git需要添加一句代码，在git pull后面。这句代码是在git 2.9.2版本发生的，最新的版本需要添加`--allow-unrelated-histories`

**解决**： 

所以我们想要pull的时候 ，应该写如下git命令： 

假如我们的本地源是origin，github上的分支是master，那么我们 需要这样写：
`git pull origin master --allow-unrelated-histories`

（一定要注意是两个减号！！！）




