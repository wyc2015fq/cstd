# Emacs-018-实现光标跳转到指定行 - 小灰笔记 - CSDN博客





2018年01月04日 23:25:48[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：670








Emacs-018-实现光标跳转到指定行

       其实这个是我在VIM中经常使用的一个功能，而VIM中的实现方式也不止一种。我自己喜欢用的是直接输入:行号后回车。现在使用Emacs也想有这么一个功能，但是找来找去一下子找不到一个现成的快捷方式。

       相应的功能还是有的，需要使用一个goto-line的命令。输入之后会被要求输入行号，接着回车即可让光标跳转到指定的行。

       输入截图：

![](https://img-blog.csdn.net/20180104232608940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       实现的效果如下：

![](https://img-blog.csdn.net/20180104232626180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



