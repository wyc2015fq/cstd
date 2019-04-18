# Emacs-050-关闭Emacs滚动条 - 小灰笔记 - CSDN博客





2018年01月14日 16:15:24[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：250








Emacs-050-关闭Emacs滚动条

代码编辑器中，我使用滚动条的概率几乎为零。不过我从来没有主动考虑过隐藏滚动条或者关闭这个功能。看到了别人的Emacs中没有滚动条，界面看上去似乎就多了几分清爽。于是也找到了相应的配置实现了一下。



上面这是有滚动条的时候，有一个大竖条。增加如下配置并使其生效：

(set-scroll-bar-modenil)

生效后的效果如下：

![](https://img-blog.csdn.net/20180114161445425?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




这样看上去的效果好多了！

![](https://img-blog.csdn.net/20180114161500028?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




