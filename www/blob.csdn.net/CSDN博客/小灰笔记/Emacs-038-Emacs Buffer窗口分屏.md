# Emacs-038-Emacs Buffer窗口分屏 - 小灰笔记 - CSDN博客





2018年01月13日 18:30:56[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：666








Emacs-038-Emacs Buffer窗口分屏

            需要同时编辑少数多文件的时候，能够实现分屏显示会是一个很好的功能。Emacs中对此有很好的支持。

            窗口Buffer分屏其实是进行了Buffer的增加，增加的方式有两种：1，在下面增加；2，在右边增加。

            没有分屏之前的一个显示如下：

![](https://img-blog.csdn.net/20180113183014735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            在下面增加的方法：C-x2

![](https://img-blog.csdn.net/20180113183027602?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            如果要退出这种模式，只留下一个，可以使用C-x1。Emacs会关闭其他的Buffer，只留下当前光标所在的那个。

            在右边增加分屏，操作：C-x3

![](https://img-blog.csdn.net/20180113183036820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            这种分屏效果可以不断分下去，而且两种分屏可以协同使用。不过C-x1的操作会关掉其他所有的Buffer而不仅仅是最后分出来的那个。



