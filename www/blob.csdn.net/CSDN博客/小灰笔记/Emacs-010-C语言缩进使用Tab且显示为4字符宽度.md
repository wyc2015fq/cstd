# Emacs-010-C语言缩进使用Tab且显示为4字符宽度 - 小灰笔记 - CSDN博客





2017年12月27日 20:39:50[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：613








Emacs-010-C语言缩进使用Tab且显示为4字符宽度

       Emacs的缩进可真让我头疼，昨天晚上看了半晚上，今天白天又花了不少时间还是没有做到完美。但是，在C语言中这部分功能已经初步实现了我想要的结果。

       实现目标：

1.    输入Tab键后不会立即被空格替换；

2.    Tab的宽度显示为4个字符宽度；

3.    自动缩进修整的功能尽量关闭，这样能够在退格的时候一次把Tab缩进全部删除。

这样，需要实现的功能如下：

1．  输入Tab键的手插入的只有一个Tab

最初这部分我尝试了几个别人的配置没有弄明白，后来找到了一篇不错的文章找到了相应的解释如下：

What should be inserted when I press the TAB key?

    True TAB characters only

    Spaces only

    A combination of TABs and spaces (default)

Tab按键输入的结果不过是上面的几种，我需要的自然是第一个。具体的实现方式如下：

(global-set-key(kbd "TAB") 'self-insert-command)

2. 接下来，把Tab设置为显示为4个字符宽度。实现方式：


`(setqdefault-tab-width 4)`

3. 上面的方式实现后，编辑一般文件已经可以。但是编辑C文件会被传说中的智能缩进给破坏。解决方式是配置C语言的编辑模式，实现方式如下：



```
(defunmy-c-mode-hook ()
  (setq c-basic-offset 4
        c-indent-level 4
        c-default-style "bsd"))
(add-hook'c-mode-common-hook 'my-c-mode-hook)
```


4. 经过上面三部，在编辑缩进上已经基本实现了想要的效果。但是退格删除依然需要四次，接下来再进行一次改进修改。增加下面的配置：


`(setqbackward-delete-char-untabify-method nil)`

这样，整个的配置模块就是如下的情况：



```
(global-set-key(kbd "TAB") 'self-insert-command)
(setqdefault-tab-width 4)
(setqbackward-delete-char-untabify-method nil)
(defunmy-c-mode-hook ()
  (setq c-basic-offset 4
        c-indent-level 4
        c-default-style "bsd"))
(add-hook'c-mode-common-hook 'my-c-mode-hook)
```


经过10次配置之后，我的Emacs配置文件如下：



```
(custom-set-variables
 ;;custom-set-variables was added by Custom.
 ;;If you edit it by hand, you could mess it up, so be careful.
 ;;Your init file should contain only one such instance.
 ;;If there is more than one, they won't work right.
 )
(custom-set-faces
 ;;custom-set-faces was added by Custom.
 ;;If you edit it by hand, you could mess it up, so be careful.
 ;;Your init file should contain only one such instance.
 ;;If there is more than one, they won't work right.
 '(default ((t (:family "Courier New":foundry "outline" :slant normal :weight normal :height 98 :widthnormal)))))
;; Disable files backup function
(setq make-backup-files nil)
(setq line-number-mode t)
(global-linum-mode t)
;; Config Tab
;;;; This will force Tab to be just oneTab.
(global-set-key (kbd "TAB")'self-insert-command)
(setq default-tab-width 4)
(setq backward-delete-char-untabify-methodnil)
(defun my-c-mode-hook ()
 (setq c-basic-offset 4
       c-indent-level 4
       c-default-style "bsd"))
(add-hook 'c-mode-common-hook'my-c-mode-hook)
```





