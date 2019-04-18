# Emacs-056-整个Buffer重新智能缩进 - 小灰笔记 - CSDN博客





2018年01月14日 22:06:03[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：319








Emacs-056-整个Buffer重新智能缩进

前阵子曾经十分讨厌Emacs的自动缩进功能，因为在删除等功能操作的时候十分别扭。现在实现了hungry模式之后，倒是觉得这个功能可以回归一下。

找到了一段别人的Emacs配置函数，能够实现对整个Buffer进行重新智能缩进排版的功能。使用了一下，十分顺手。这里小结分享一下：



```
;;a batter function for indent
(defunindent-buffer()
(interactive)
(indent-region(point-min) (point-max)))

(defunindent-region-or-buffer()
(interactive)
(save-excursion
(if(region-active-p)
(progn
(indent-region(region-beginning) (region-end))
(message"Indent selected region."))
(progn
(indent-buffer)
(message"Indent buffer.")))))
;;;;Key binding for this better indent function
(global-set-key(kbd "C-M-\\") 'indent-region-or-buffer)
```



上面分别实现了对整个Buffer进行重新智能缩进的排版和按键绑定两个功能，其中按键绑定与Emacs原本的功能相同。

接下来演示一下使用的效果，加入有以下C代码：

![](https://img-blog.csdn.net/20180114220606549?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


看得出，上面代码中第7行是缩进有问题的。接下来按组合键C-M-\，出现的效果如下：

![](https://img-blog.csdn.net/20180114220618871?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


从上面的截图中可以看到，不仅实现了代码的缩进排版，同时在minibuffer中给出了功能调用提示。



