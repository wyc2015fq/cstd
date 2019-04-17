# python中变量的生命周期 - Machine Learning with Peppa - CSDN博客





2018年04月16日 21:58:28[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：291








最近在写python的时候发现一个好玩的现象，就是在if else重定义的变量，没有声明全局，在外部也可以使用，

这里涉及到一个python变量生命周期的问题。python能够改变变量作用域的代码段是def、class、lamda.
if/elif/else、try/except/finally、for/while 并不能涉及变量作用域的更改，

也就是说他们的代码块中的变量，在外部也是可以访问的
变量搜索路径是：本地变量->全局变量





![](https://img-blog.csdn.net/20161110105647895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



