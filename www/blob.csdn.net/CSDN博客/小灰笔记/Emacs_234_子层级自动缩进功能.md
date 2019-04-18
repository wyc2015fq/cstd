# Emacs_234_子层级自动缩进功能 - 小灰笔记 - CSDN博客





2018年07月24日 21:03:25[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：109








       能够实现这个功能的一个插件是aggressive-indent，我在purcell的配置中看到了这么一个功能。

       具体效过可以直接看看有如下代码：

![](https://img-blog.csdn.net/20180724210315199?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       加入一个条件判断：

![](https://img-blog.csdn.net/20180724210315205?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       从上面可以看得出，虽然还没有输入完成，但是下面的代码已经实现了自动的缩进。这个功能设计的很巧妙，但是最终我没有使用。主要还是在代码块中时常会让后面的大括号出现诡异的乱跳，这让我感觉又很大的视觉不适。



