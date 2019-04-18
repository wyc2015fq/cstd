# Emacs-016-利用company实现C语言的自动补全 - 小灰笔记 - CSDN博客





2018年01月03日 23:24:50[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1036








Emacs-016-利用company实现C语言的自动补全

       折腾了半天company没能够实现C语言的自动补全，突然间想到了前段时间实现Tab缩进功能时候遇到的问题。或许，我的自动补全功能已经实现，只是C语言中不适用。尝试写了个txt文件，果真是被我猜中。

       查看了一下company的几个命令，找到了一个关于clang的。自己尝试做了配置，在配置文件中添加了以下信息：

;; Try to config company for c language

(add-hook 'c-mode-hook 'company-clang)

       再次尝试C语言的编辑，没想到功能真实现了，而且超出了我自己的预期。使用效果如下：

![](https://img-blog.csdn.net/20180103232428315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       这种使用效果有点类似VScode中的效果了，但是响应速度比VSCode还是要差不少。



