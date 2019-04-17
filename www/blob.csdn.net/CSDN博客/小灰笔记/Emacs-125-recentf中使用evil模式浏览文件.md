# Emacs-125-recentf中使用evil模式浏览文件 - 小灰笔记 - CSDN博客





2018年03月09日 19:38:59[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：71
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









![](https://img-blog.csdn.net/20180309193721404)



recent-open-files命令打开的最近浏览记录是Emacs模式，需要切换到Normal模式下才能够使用Evil方式进行浏览。尝试自己写lisp没成功，切换一直失败。最后抄了Spacemacs的配置，它采用的是helm的相应功能，直接使用helm模块的函数。

![](https://img-blog.csdn.net/20180309193803988)



具体的配置为84行。效果：

![](https://img-blog.csdn.net/2018030919383424)



不太一样，但是都能够达到我要的效果。我还是更喜欢后者的。





