# Emacs-051-popwin提高帮助查询便捷性 - 小灰笔记 - CSDN博客





2018年01月14日 16:16:36[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：172








Emacs-051-popwin提高帮助查询便捷性

Emacs默认的帮助查询功能，弹出的窗口在操作或者退出的时候都需要Buffer的切换。只有光标切换到相应的Buffer才能够继续进行退出等操作。功能的改进与提升可以使用popwin插件。这个插件能够改变弹窗行为的同时把光标转移到新弹出的窗口。

效果如下：

![](https://img-blog.csdn.net/20180114161613864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




可以看得到光标的行为已经发生了变化。如果此时退出，可以有两种方式：第一种是按Q键，第二种是按C-g。由于C-g是通用的命令取消功能键，这能够让人更熟悉地接受确认。



