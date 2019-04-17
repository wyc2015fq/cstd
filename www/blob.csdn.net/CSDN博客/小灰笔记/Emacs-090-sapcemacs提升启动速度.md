# Emacs-090-sapcemacs提升启动速度 - 小灰笔记 - CSDN博客





2018年01月30日 21:14:35[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：229








Emacs-090-sapcemacs提升启动速度

       spacemacs在启动的时候卡住了不动或者很慢，可能的原因从FAQ找到如下：

![](https://img-blog.csdn.net/20180130211422404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       按照第一种配置，做了相应的修改。现在没有出现卡顿，但是启动的速度上改善有限。现在我电脑上的spacemacs启动需要8秒钟多，将近9秒。确实是赶得上操作系统的启动了，够慢！

       再次尝试了第二种，启动速度上依然是没有改善。从启动时候的提示看，这个应该是启动时候插件的加载速度慢而不是联网。



