# Emacs-019-文件保存与关闭 - 小灰笔记 - CSDN博客





2018年01月04日 23:43:27[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：640








Emacs-019-文件保存与关闭

       考虑这个功能，一定程度上还是对标VIM。VIM中有w有q，自然还有wq。我使用的Emacs是一个GUI版本的，其实保存退出即使不了解处于习惯也能够解决。简单的保存退出自然是图形化的操作即可：

![](https://img-blog.csdn.net/20180104234341423?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       其实，在上面的界面中也能够看到相应的组合键操作了。C-x C-c。进一步看看这个组合键的定义，关联了什么命令：

![](https://img-blog.csdn.net/20180104234357096?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       其实，关联的是保存缓冲区并且杀死终端的意思。描述的似乎是命令行模式下的Emacs，但是GUI的软件中大致应该也是类似的了。



