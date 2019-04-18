# Emacs-046-smex的使用 - 小灰笔记 - CSDN博客





2018年01月14日 16:07:40[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：418








Emacs-046-smex的使用

Smex是M-x的一个辅助增强版本，主要的功能还是在于命令交互的使用。相比内置的M-x的方式，smex在使用上更加直观。

使用方法：
- 
首先在利用MELPA安装smex插件；

- 
在Emacs的配置文件中增加如下配置：





(require'smex) ; Not needed if you use
 package.el

(smex-initialize); Can be omitted. This
 might cause a (minimal) delay

;when Smex is auto-initialized on its first
 run.




(global-set-key(kbd
 "M-x") 'smex)

(global-set-key(kbd
 "M-X") 'smex-major-mode-commands)

;;This
 is your old M-x.

(global-set-key(kbd
 "C-c C-c M-x") 'execute-extended-command)




相对具体的一个说明可以在Github上找到，具体的地址：[https://github.com/nonsequitur/smex](https://github.com/nonsequitur/smex)

从上面的配置可以看得出，原来的M-x功能被修改了。如果要使用，需要先输入两次机C-c。安装并且配置成功后，输入M-x的效果如下：

![](https://img-blog.csdn.net/20180114160707679?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




可以看得到，minibuffer中多了很多提示，通过C-s可以进行候选项的切换。如果输入一定的字符，常识自动补全可能会看到下面的界面：

![](https://img-blog.csdn.net/20180114160722627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




能够给出比较多并且比较直观的选项，可以直接进行候选项的选择。



