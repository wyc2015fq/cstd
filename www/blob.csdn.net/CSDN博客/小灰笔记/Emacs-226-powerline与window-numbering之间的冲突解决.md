# Emacs-226-powerline与window-numbering之间的冲突解决 - 小灰笔记 - CSDN博客





2018年04月10日 22:50:32[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：139
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









       之前看别人的教程的时候看到过类似的问题，使用powerline之后，window-numbering效果居然没了。

       我自己使用的时候也遇到了这个问题，但是考虑到了这个问题可能是配置效果覆盖的问题。尝试了一下，果真我考虑的一样。

       其实，相应的问题就是因为powerline的配置生效在window-numbering之后，说起来也算是一个Bug。

       修改之后正常效果如下：

![](https://img-blog.csdn.net/20180410225024626)






