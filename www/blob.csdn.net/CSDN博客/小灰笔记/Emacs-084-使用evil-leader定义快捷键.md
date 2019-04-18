# Emacs-084-使用evil-leader定义快捷键 - 小灰笔记 - CSDN博客





2018年01月29日 19:53:00[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：292
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









Emacs-080-使用evil-leader定义快捷键

     这一次的配置是针对更便捷的快捷键配置，需要用到的插件是evil-leader。安装完成之后，配置例子如下：

(evil-leader/set-key

 "ff" 'find-file

 "fr" 'recentf-open-files

 "bb" 'switch-to-buffer

 "bk" 'kill-buffer

 "0" 'select-window-0

 "1" 'select-window-1

 "2" 'select-window-2

 "3" 'select-window-3

 "4" 'select-window-4

 "w/" 'split-window-right

 "w-" 'split-window-below

 ":"  'counsel-M-x

 "wM" 'delete-other-windows)

       可以根据自己习惯快速便捷的配置部分功能。个人觉得比较方便直观的在于窗口的导航切换。按下space按键之后，可以通过数字直接进行窗口的导航。

       上面的分屏等配置都是抄的别人的，我觉得这种方式也比较好记忆，在一定程度上有一点形象。



