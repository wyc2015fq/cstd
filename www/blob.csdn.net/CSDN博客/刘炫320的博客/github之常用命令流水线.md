# github之常用命令流水线 - 刘炫320的博客 - CSDN博客
2018年09月03日 11:35:11[刘炫320](https://me.csdn.net/qq_35082030)阅读数：187
所属专栏：[github官方指南](https://blog.csdn.net/column/details/16517.html)
# 1.写在前面
我们接触github的时间也不少了，那么是否可以尝试使用它来工作了呢？
# 2. 流水线
一个文件的流动路径大致如下 
![这里写图片描述](https://img-blog.csdn.net/20180903111255474?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1MDgyMDMw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以看到有在文件系统，工作空间，本地repo和远程repo四个不同的部分，通过不同的操作来进行不同的流转。值得注意的是，我们这里只考虑同一个文件的流动，并不能真正的体现出git的版本控制。
# 3.版本控制
不同于传统的单线版本控制，git可以有多分支，多用户，同时操作。
```
//克隆远程分支
git clone [url] {name}
//切换分支
git checkout [name]
//创建新分支
git checkout -b [new name]
//更新远程分支
git push -u origin [branch name]
```
# 4.处理冲突
有时候由于不规范的操作，我们在进行push和pull的时候会有冲突的情况，那么解决冲突的方法就是手动修改冲突部分（通常这部分不是太多） 
冲突的部分假设会如图所示： 
![这里写图片描述](https://imgsa.baidu.com/exp/w=480/sign=198c20069116fdfad86cc7e6848f8cea/960a304e251f95ca6fab499bc5177f3e67095245.jpg)
只需要保留“我喜欢JAVA”或者“我喜欢PHP”其中的一行，然后删除其余标记符（<,=,>）即可。
# 5.回滚操作
回滚操作分为2种，一种是正常版本迭代，一种是合并前后。 
正常版本迭代可以使用：
```
//回退到这次提交
git reset --hard HEAD
//回退到上一次提交
git reset --hard HEAD^
//回滚到指定版本
git log 
git reset --hard [ID]
//撤销合并状态
git merge --abort
```
# 6.总结
这次我们大致梳理了一下工作中常用的一些操作和整体的工作流。
