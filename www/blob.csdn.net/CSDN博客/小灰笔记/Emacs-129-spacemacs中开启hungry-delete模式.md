# Emacs-129-spacemacs中开启hungry-delete模式 - 小灰笔记 - CSDN博客





2018年03月11日 16:39:13[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：118
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









            这个需求其实还是来自于emacs中的缩进风格，默认情况下，emacs会把TAB展开成空格。这样，按照文档描述的好处是在什么编辑器中看到的效果都一样。但是在我自己的体验看来，编辑有时候会麻烦。尤其是删除TAB的时候，每次按TAB删除需要4次退格。

            好在还有一个hungry-delete可以用用，效果不是很完美，但是已经提高了一下编辑体验。

            默认情况下，spacemacs中已经安装了hungry-delete的插件，只需要增加如下配置即可：

![](https://img-blog.csdn.net/20180311163900270)



            增加的部分为332、333行配置。





