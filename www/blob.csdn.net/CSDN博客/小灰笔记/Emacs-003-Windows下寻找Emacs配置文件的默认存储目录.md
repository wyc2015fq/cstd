# Emacs-003-Windows下寻找Emacs配置文件的默认存储目录 - 小灰笔记 - CSDN博客





2017年12月25日 23:26:25[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：322








Emacs-003-配置文件默认存储目录寻找

       不像Linux和Mac，配置文件在~目录下，非常好找。在Windows下有时候找起来会很没头绪，不同电脑由于环境变量信息的不一致也会导致一定的偏差。如何定位Emacs配置文件的位置呢？

       我们可以借助于GUI的部分操作，在提示中寻找Emacs配置文件的位置。简单的两个步骤就能够找到配置文件：

1.    修改一个Emacs配置，比如我这里修改了一下字体大小

2.    点击保存配置，具体的位置如下图

![](https://img-blog.csdn.net/20171225232531427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




完成上面的操作后，在最下面的命令交互位置会提示配置文件的存储位置。

![](https://img-blog.csdn.net/20171225232543950?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




上面这个目录就是我的配置文件的存放位置，而这个位置是我自己通过环境变量模拟的Linux~目录。



