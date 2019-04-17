# Emacs-039-给打开最近浏览功能设置按键操作 - 小灰笔记 - CSDN博客





2018年01月13日 18:33:29[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：99








Emacs-039-给打开最近浏览功能设置按键操作

            一直都看到网上的帖子中说Emacs的按键绑定功能十分强大，但是我自己没有去设置过按键的功能。这一次使用的是一个按键定义功能，我看了一下应该还有一个按键的绑定功能，不知道两者之间有什么关系或差异。暂且实现这个定义功能吧！

            前面刚刚设置了最近浏览文件的支持，接下来给打开最近浏览的文件定义一个按键操作。可以在Emacs的配置文件中增加如下配置：

![](https://img-blog.csdn.net/20180113183257878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


;; key defination to open recent files

(global-set-key "\C-x\ \C-r" 'recentf-open-files)

            令其生效后，使用C-xC-r命令可以实现如下效果：

            接下来打开相应的文件可以使用鼠标点击等操作进行，相对来说方便一些。



