# 023_emacs git-gutter+报错解决 - 小灰笔记 - CSDN博客





2018年12月03日 20:22:20[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：61
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









         其实这是我emacs上的一个很老的问题了，但是一直没有去解决。总归还是因为没有影响到最终的使用。具体的报错如下：

![](https://img-blog.csdnimg.cn/2018120320215183.png)

         最初的时候，还是直接简单粗暴一点：直接用搜索引擎去找答案，看看别人是否遇到过。很遗憾的是没有一下子找到。但是，简单分析一下其实这个报错还是很明确的，git找不到。但是，我电脑上git一直在用，为什么找不到呢？

         再进一步一想，其实是一个很简单的小问题了，我使用git的时候直接使用git bash，但是git本身的命令却没有添加到环境变量，emacs这样就找到不到。

         添加环境变量之后重启emacs，这下确实是OK了！看上去赏心悦目多了！

![](https://img-blog.csdnimg.cn/20181203202151224.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)





