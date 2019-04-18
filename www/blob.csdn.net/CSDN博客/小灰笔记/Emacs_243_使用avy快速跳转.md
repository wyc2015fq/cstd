# Emacs_243_使用avy快速跳转 - 小灰笔记 - CSDN博客





2018年09月05日 22:04:26[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：186








接触到了这个功能才第一次让我觉得原来键盘真的可以比鼠标高效。

假如有如下代码：

![](https://img-blog.csdn.net/20180905220402896?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们跳转的方式可以分为几种：
- 跳转到行
- 跳转到词
- 跳转到字符



SPC j l：跳转到行，假设我想跳转到第19行.按下上面的跳转到行的组合键:

![](https://img-blog.csdn.net/20180905220402907?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



出现了很多字母开头的编号,接下来按下kj既可以跳转到19行了。

再看一下跳转到单词：

假设我们想跳转到demo_tag:

SPC jw

接着按下d，出现如下提示：

![](https://img-blog.csdn.net/20180905220402916?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       接着按a就可以跳转到相应的词了：

![](https://img-blog.csdn.net/20180905220402948?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



       这个确实是很方便，而且已经集成到了spacemacs中，后期可能会被我重度使用。



