# Emacs-042-显示括号配对 - 小灰笔记 - CSDN博客





2018年01月13日 18:39:24[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：719








Emacs-042-显示括号配对

              在HackEmacs的时候这个功能绝对算得上是一个刚需，因为配置的过程少不了跟elisp打交道。而这种语言恼人的小括号很容易让人看的眼晕。开启配对提示功能以后，当光标移动到一个括号的时候与之配对的括号会有相应的标识。

              对比看一下这个功能，先看一下没有开启提示功能的效果：

![](https://img-blog.csdn.net/20180113183830681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


              这种情况下，光标移动到括号的时候不会有任何变化。接下来把下面屏蔽的提示功能打开并使其生效。如果没有添加也就是需要在配置文件中增加如下配置：

(add-hook 'emacs-lisp-mode-hook 'show-paren-mode)

              配置生效后的效果：

![](https://img-blog.csdn.net/20180113183850972?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


              如果光标此时在右边的括号，也会有同样的提示效果：

![](https://img-blog.csdn.net/20180113183906600?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




