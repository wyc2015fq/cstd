# Emacs-083-多行屏蔽或解除屏蔽 - 小灰笔记 - CSDN博客





2018年01月29日 19:47:36[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：105
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









Emacs-083-多行屏蔽或解除屏蔽

       这个功能确实是一个让人欣喜的功能，之前这种功能只有在那些庞大的功能复杂的IDE中见过。而且使用的时候并不是很方便，现在接触的这个功能真是太棒了！

       首先，需要安装插件：evil-nerd-commenter

       然后，在Emacs的配置中增加如下配置：

![](https://img-blog.csdn.net/20180129194626999?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       其实，我用得到的似乎只需要第一个。

       我是在evil模式中使用，加入有代码如下：

![](https://img-blog.csdn.net/20180129194641201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       进入可视模式之后，选中几行代码：

![](https://img-blog.csdn.net/20180129194652247?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       按下SPC，然后按下ci（前面定义的第一个屏蔽功能组合键），效果如下：

![](https://img-blog.csdn.net/20180129194705179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       由于刚刚编辑完，此时删除注释自然是可以的通过撤销来做。不过可以尝试再次重复上面的操作，这样可以实现开关式切换：

![](https://img-blog.csdn.net/20180129194722074?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       哇！这功能简直是没的说了！好！



