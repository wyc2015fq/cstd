# Emacs-208-搜索工程中的文件 - 小灰笔记 - CSDN博客





2018年03月28日 23:06:48[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：129








       这个功能最初是在spacemacs上看到的，但是当时也没有怎么弄明白它的具体用法。自然，上面的功能不一定是使用projectile来实现的。

       如果使用projectile进行工程管理，那么使用projectile-find-file命令即可实现对工程内的文件的搜索。

       默认情况下，这个函数功能绑定到了C-c pf的组合键上。模仿了一点spacemacs，我自己重新修改了组合按键为SPC pf。

       进行文件搜索的效果如下：

![](https://img-blog.csdn.net/20180328230634825)

       匹配成功之后的效果：

![](https://img-blog.csdn.net/20180328230641301)

       此时，回车即可跳转到相应的工程文件。这个功能其实我在Source Insight中用的比较频繁，一直以来我也想让我的Emacs拥有这么一个功能。如今的这个功能已经基本上能够满足我的需要，虽然Source Insight的目录树的搜索方式有着更加好的交互感觉。但是Emacs能够更加亲近键盘，实现一个比较高效快速的文件搜索与跳转，也算是能够满足我的需要和喜好了。




