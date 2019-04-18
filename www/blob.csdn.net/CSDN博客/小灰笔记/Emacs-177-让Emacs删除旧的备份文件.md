# Emacs-177-让Emacs删除旧的备份文件 - 小灰笔记 - CSDN博客





2018年03月21日 21:25:36[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：85








![](https://img-blog.csdn.net/20180321212519611)

       这是一个变量，把它给设置成t，之后emacs可以实现旧备份文件的删除。

       具体的配置如下：

![](https://img-blog.csdn.net/20180321212527659)

       25行：

(setq delete-old-versions t)




