# Emacs-081-实现buffer窗口快速切换 - 小灰笔记 - CSDN博客





2018年01月29日 19:42:54[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：300








Emacs-081-实现buffer窗口快速切换

       这个其实还是前面提到的evil-leader的功能，只是多了一个buffer的切换。更加明确窗口的切换。插件装好了之后，还是前面的配置：

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

       其实，说起来我觉得wM的组合键使用并不顺手，Emacs默认的组合键C-x 1在操作上感觉还要便捷一些。上面的配置在练习的时候按照别人提供的方式练习了一下，至于我自己的使用至少在这个上是放弃了，还是原来的方式更加顺手一些。

       窗口切换方式：按一下SPC，然后按相应的数字即可。



