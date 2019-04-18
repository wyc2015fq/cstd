# Emacs-155-确认询问功能 - 小灰笔记 - CSDN博客





2018年03月16日 23:44:09[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：60
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









            在使用Emacs的时候，经常遇到需要输入yes或者no来确认某项功能，相应的实现方式是调用了一个函数。init.el中加入一个例子如下：

  (yes-or-no-p"do you want to startup emacs?")

            emacs启动效果：

![](https://img-blog.csdn.net/20180316234401832)

            询问交互已经出来了，函数后面需要提供一个字符串用于在此处的显示。如果此时输入yes，回车后emacs即可正常启动。




