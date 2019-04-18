# Emacs-144-mini buffer中使用命令安装package - 小灰笔记 - CSDN博客





2018年03月14日 23:48:37[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：45
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









![](https://img-blog.csdn.net/20180314234737571?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



            在minibuffer中输入package-install命令，之后回车。之前，这方面我有点理解错误，总是直接输入package-installpackage-name的方式尝试。结果总是失败。而实际的操作应该是先输入命令，然后输入package的名称。


![](https://img-blog.csdn.net/20180314234759189?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



            安装首先会联网，查找服务器。自然，这个服务器的源也是可以自己配置的。

![](https://img-blog.csdn.net/20180314234823469?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



            查看MessageBuffer可以看到undo-tree安装的过程以及结果。





