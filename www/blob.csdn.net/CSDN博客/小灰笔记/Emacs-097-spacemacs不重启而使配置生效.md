# Emacs-097-spacemacs不重启而使配置生效 - 小灰笔记 - CSDN博客





2018年01月31日 19:01:53[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：332








Emacs-097-spacemacs不重启而使配置生效

       这个功能我觉得算是我比较需要的一个功能，之前接触spacemacs之前也有类似的需要，当时通过简单的语句执行来实现。不过功能有限，操作也不方便。sapcemacs中的使用方式为：

       SPCfeR

测试如下：

1， 把行号显示注释掉

![](https://img-blog.csdn.net/20180131190047489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2， 执行上面的操作，具体效果如下图。这是一个很有意思的现象，居然没有生效。或许是个小bug，功能减少的时候并不生效。不过在使用的过程中确实是看到了配置文件重新加载的行为。

![](https://img-blog.csdn.net/20180131190107035?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3， 重启emacs，查看效果：

![](https://img-blog.csdn.net/20180131190116050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

     重启之后这部分确实是生效了，看得出这个功能的设计似乎确实是有问题。

4， 重新把行号功能打开，再次执行上面的操作，效果如下：

![](https://img-blog.csdn.net/20180131190127601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       执行速度略微缓慢，看得出配置文件加载的过程。同时，很明显看得出这一次的配置没有重启emacs就已经生效。对比一下再次小结，或许这真是一个Bug。



