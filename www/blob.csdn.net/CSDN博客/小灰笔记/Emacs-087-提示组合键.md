# Emacs-087-提示组合键 - 小灰笔记 - CSDN博客





2018年01月29日 20:00:43[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：106
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









Emacs-084-提示组合键

       组合键比较多的时候，加上这么一个功能还是很有用的。这需要增加一个插件which-key。安装完插件之后，增加如下配置：

(which-key-mode t)

(which-key-setup-side-window-right)

       其中，第二个并不是必须的。第二个配置的作用是为了把提示转移到窗口右侧。如果是在右侧，按下SPC之后的提示状态：

![](https://img-blog.csdn.net/20180129200020595?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       如果只有第一个配置，那么按下SPC的状态：

![](https://img-blog.csdn.net/20180129200029649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       处于习惯性，我觉得在下面显示还要好一点。除此之外，如果设置在右边显示，有时候会因为显示版面不够而异常。



