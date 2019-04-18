# Emacs-048-自动加载磁盘上修改的文件 - 小灰笔记 - CSDN博客





2018年01月14日 16:12:23[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：214








Emacs-048-自动加载磁盘上修改的文件

类似Notepad++，SourceInsight等编辑器，当打开的文件被其他的程序修改了之后都有进行加载更新的功能。Emacs也有相应的功能，需要做出如下配置：

(global-auto-revert-modet)

相应的配置使能之后，可以自动加载磁盘上修改过的文件。而且不会有提示：

![](https://img-blog.csdn.net/20180114161110093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




上面的文件，随便进行一下修改：

![](https://img-blog.csdn.net/20180114161120175?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




回看Emacs：

![](https://img-blog.csdn.net/20180114161142835?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




跟随更改，但是行号没有变化。单击当前Buffer，行号会自动发生变化。

![](https://img-blog.csdn.net/20180114161201801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






