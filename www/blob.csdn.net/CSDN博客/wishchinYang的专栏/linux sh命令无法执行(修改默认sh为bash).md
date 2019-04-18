# linux sh命令无法执行(修改默认sh为bash) - wishchinYang的专栏 - CSDN博客
2019年02月08日 13:00:45[wishchin](https://me.csdn.net/wishchin)阅读数：232
个人分类：[QT./Linux](https://blog.csdn.net/wishchin/article/category/1721683)
原文链接：[Ubuntu sh命令无法正确执行](http://www.cnblogs.com/xudong-bupt/p/7883040.html) ！
新安装的ubuntu，执行shell命令，都怀疑自己的shell水平了。
原来, ubuntu默认的是dash, 需要手动执行命令将dash改为bash.
命令: sudo dpkg-reconfigure dash。
然后出现的界面中**选择 NO**, **！！！**之后就正常啦。
