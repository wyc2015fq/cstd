# Emacs-035-关闭Emacs的欢迎界面 - 小灰笔记 - CSDN博客





2018年01月08日 23:52:54[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：948








                
Emacs-035-关闭Emacs的欢迎界面
不断的关闭启动，总觉得Emacs的启动速度一般。既然这样，就得考虑关闭一些不需要的启动项。
最初，欢迎界面中的信息对一点基础都没有的新手来说很有指导意义。现在我觉得我基本可以摆脱那个界面，开始自己的自由Hack了。这一次就关闭一下这个界面吧！
查了一下，相应的配置实现十分简单，只需要在Emacs的配置文件中增加如下配置即可：

(setq inhibit-splash-screen t)

配置完成后，重启Emacs的效果如下：

![](https://img-blog.csdn.net/20180108235308148?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


那个已经看了不知道多少遍的欢迎界面已经没有了！
            


