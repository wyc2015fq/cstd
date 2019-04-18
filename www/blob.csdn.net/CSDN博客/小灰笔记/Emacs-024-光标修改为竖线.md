# Emacs-024-光标修改为竖线 - 小灰笔记 - CSDN博客





2018年01月06日 13:44:00[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：928








Emacs-024-光标修改为竖线

       这个问题最初在VIM中也遇到过，习惯了之后或许两个没有什么差异。但是不同人的习惯或者说喜欢的风格不一样，我个人是喜欢竖线而不喜欢闪烁的小块。

       如果使用小块的方式，小块一般是覆盖在一个字符上。这样有时候会让我觉得有点弄混插入位置的概念。小块的模式是默认的，状态示例如下：

![](https://img-blog.csdn.net/20180106134324167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       如果要改成竖线，可以在配置文件中增加如下配置。

(setq-default cursor-type 'bar)

       修改后的效果如下：

![](https://img-blog.csdn.net/20180106134334526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       我觉得美观度上两者没有什么好纠结的，我之所以喜欢这种细线模式还是前面我说的，这种模式让我觉得对插入位置的理解更加自然。



