# Emacs-058-使用yn取代yes和no - 小灰笔记 - CSDN博客





2018年01月16日 00:11:18[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：162
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









Emacs-058-使用yn取代yes和no

    在使用很多操作的时候有时候会被询问是否确定修改等操作，这时候很多时候要求输入的是yes或者no。虽说是比较人性化，但是交互起来还是有一点不便捷。如果是使用y或者n在操作上或许会更加便捷一些。

    使用下面的配置可以实现这个操作的转变：

(fset 'yes-or-no-p'y-or-n-p)

    增加上面的配置并且生效后，再做相应的操作的时候，出现询问的时候mini-buffer中出现的提示将会是y或者n分别代表确认或者否认操作。



