# Emacs-043-高亮提示光标所在当前行 - 小灰笔记 - CSDN博客





2018年01月13日 18:41:55[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：634








Emacs-043-高亮提示光标所在当前行

              如果编辑的文件内容太多，想找一下当前光标所在的位置就会有些不方便。虽然有行号提示，但是从我个人来说我不习惯去过多注意下面的提示。

              如果能够迅速找到自己的编辑位置，首先得定位到光标在哪一行。这样，接下来的操作会更加顺利成长。此时，这个高亮当前行的功能就非常有用了。

              接下来通过配置开启相应功能，对比一下不同的效果。首先看看未开启前：

![](https://img-blog.csdn.net/20180113184048473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


              并不是很容易能够注意到光标的位置。接下来在配置文件中增加如下配置并使其生效：

(global-hl-line-mode t)

              生效后的效果：

![](https://img-blog.csdn.net/20180113184127250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


              有一个高亮的条可以提示当前光标所在行的位置，十分醒目。



