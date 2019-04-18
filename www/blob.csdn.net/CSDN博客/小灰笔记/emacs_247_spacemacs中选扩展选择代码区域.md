# emacs_247_spacemacs中选扩展选择代码区域 - 小灰笔记 - CSDN博客





2018年10月08日 22:30:20[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：91








         之前这个功能一直都是使用我自己网上找来的一段函数来实现，后来在通读spacemacs文档的时候发现这个功能其实在spacemacs中内置了。

         演示一下这个功能，假设有如下代码：

![](https://img-blog.csdn.net/20181008223005761?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

         光标现在停在了else后边的大括号上，此时SPC v可以选中相应的整个大括号中的内容。效果：

![](https://img-blog.csdn.net/20181008223005867?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

         再按一次v，选中的区域进行扩展，效果：

![](https://img-blog.csdn.net/20181008223005900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20181008223005899?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

         继续按v，选中区域继续扩展，持续下去最终会选中整个文件（buffer）内容。

         如果是在大括号外，选择的时候首先的最小区域是单词，然后扩展到语句，最后继续扩展到代码块。因此，在很多时候，这个选择方式可以用来选择一个变量。效果：

![](https://img-blog.csdn.net/20181008223005848?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

         关于emacs的技巧，我现在掌握的还是远远不够。现在的技能只能说是勉强够用，类似的技巧还需要继续扩充。不知道再过一年半，到我最初设定的时间点的时候，我的emacs操作技能会不会有一个更加显著的飞跃。



