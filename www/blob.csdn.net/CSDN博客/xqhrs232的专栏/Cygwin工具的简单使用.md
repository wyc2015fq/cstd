# Cygwin工具的简单使用 - xqhrs232的专栏 - CSDN博客
2019年03月26日 15:58:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：32
原文地址::[https://www.cnblogs.com/myidea/p/4858862.html](https://www.cnblogs.com/myidea/p/4858862.html)
相关文章
1、在cygwin中设置环境变量----[https://blog.csdn.net/xujingqing/article/details/50679904](https://blog.csdn.net/xujingqing/article/details/50679904)
### 简介
- 从使用角度来看：Cygwin就是一个windows软件，该软件就是在windows上仿真linux操作系统。简言之，cygwin是一个在windows平台上运行的 linux模拟环境,使用一个Dll(动态链接库)来实现,这样,我们可以开发出Cygwin下的UNIX工具,使用这个DLL运行在Windows下,可以想一下,在运行Windows的同时,也可以使用VI,BASH,TAR,SED等UNIX下的工具,这个VM虚拟机有很相同的原理,但是VM是虚拟多个,而Cygwin是同时使用Windows和UNIX,这样对于那些在Windows和Unix下移植的程序来说是比较简单的事情了.
## 使用
- 使用上的方便性很是不错，启动Cygwin以后，会在Windows下得到一个Bash Shell，由于Cygwin是以Windows下的服务运行的，所以很多情况下和在Linux下有很大的不同，这点上，我是通过直接使用Cygwin来体会。
**1 比如PS，相当于Windows下的TM(任务管理器)，直接Ps的话那么得到的会是Cygwin下的Shell的进程如下**
![](https://images2015.cnblogs.com/blog/814346/201510/814346-20151007161150331-1774290567.png)
**2 如果这个时候你需要在Cygwin下显示Windows下的进程你可以在PS后面加上参数-aW**
![](https://images2015.cnblogs.com/blog/814346/201510/814346-20151007161227940-1651407737.png)
PS的相关用法:
```
Quote 
Usage ps [-aefl] [-u uid]
-f = show process uids, ppids
-l = show process uids, ppids, pgids, winpids
-u uid = list processes owned by uid
-a, -e = show processes of all users
-s = show process summary
-W = show windows as well as cygwin processes
```
**3 有点不一样的地方，在Cygwin下怎么访问Windows下的内容呢？**
- 首先使用DF命令直接查看下本地驱动器，很容易了吧，显示的内容如下:
![](https://images2015.cnblogs.com/blog/814346/201510/814346-20151007161244018-822941764.png)
以上是我硬盘的全部！
- 进D盘
![](https://images2015.cnblogs.com/blog/814346/201510/814346-20151007161318065-1140317133.png)
**4 help命令**
比如我不知道md5sum做什么用的（这个命令是用于效验文件md5值的，主要是为了文件的完整性和安全性）
![](https://images2015.cnblogs.com/blog/814346/201510/814346-20151007161331331-1957820538.png)
`以上仅是cygwin的简单介绍，作为入门的几个命令，重点理解体会该软件的本质内容`
//========================================================================
备注::
1>怎么添加环境变量
`PATH=/my/path:$PATH`

