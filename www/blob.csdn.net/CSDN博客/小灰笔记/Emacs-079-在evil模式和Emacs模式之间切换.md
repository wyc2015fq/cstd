# Emacs-079-在evil模式和Emacs模式之间切换 - 小灰笔记 - CSDN博客





2018年01月25日 23:52:17[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：583








Emacs-079-在evil模式和Emacs模式之间切换

       默认使用evil模式的时候，想要使用Emacs模式无须改变配置文件并重启Emacs。插件安装完成后，默认有一个切换的功能。默认的状态下，功能绑定到了C-z按键。

       如果默认开启了evil模式，在没有进入到插入模式的时候按下C-z可以切换到Emacs模式。再次按下C-z则又进入到evil模式。

       使用例子，在开启了evil默认使用的时候，Emacs启动后进入配置文件编辑：

![](https://img-blog.csdn.net/20180125235152837?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


       此时是Evil模式，使用Vim的操作都会有相应的行为模式响应。如果按下C-z，则出现如下状态：

![](https://img-blog.csdn.net/20180125235205794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       看得见状态条上的提示变化。此时是进入了Emacs模式，同时Vim的操作方法全都失效。如果想要回到evil，再次C-z即可，这是一个开关切换型的操作。



