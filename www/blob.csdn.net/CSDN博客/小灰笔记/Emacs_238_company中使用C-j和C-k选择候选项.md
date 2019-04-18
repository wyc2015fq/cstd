# Emacs_238_company中使用C-j和C-k选择候选项 - 小灰笔记 - CSDN博客





2018年07月24日 23:28:45[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：94








         使用Emacs一共6个月，在这过程中几乎一直使用上下箭头按键选择候选项。但是，中间也试用过spacemacs，这里面选择候选项有多种方式，其中一种就是C-k和C-j组合键用来做上下移动。使用的体验非常好，但是由于不是一个硬性需求一直也没有修改自己的配置。

         进来实在是受不了上下箭头了，决定修改，在company-mode中增加如下配置：

(with-eval-after-load 'company

  (define-key company-active-map (kbd "M-n") nil)

  (define-key company-active-map (kbd "M-p") nil)

  (define-key company-active-map (kbd "C-j") #'company-select-next)

  (define-key company-active-map (kbd "C-k") #'company-select-previous))

         使能以后，前面描述的功能可用。



