# Emacs-191-设定Emacs启动时候的长和宽 - 小灰笔记 - CSDN博客





2018年03月24日 21:16:03[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：204








       默认情况下Emacs启动时候的窗口可能偏小，如果想要扩大一下启动后的默认窗口大小可以使用如下配置：

(add-to-list 'default-frame-alist '(width.90))

(add-to-list 'default-frame-alist '(height.40))

       以上配置，可以把Emacs启动后的默认窗口大小设置为90 * 40




