# LaTeX 如何取消自动断字 - 家家的专栏 - CSDN博客





2012年09月23日 18:42:15[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3482








【问题描述】

期刊规定段落边上不能出现hyphen, 所以， 我想问一下， 怎么可以取消或是阻止latex的自动断词功能呢？


【解决方案】

可以将下面语句放在导言区：

\tolerance=1

\emergencystretch=\maxdimen

\hyphenpenalty=10000

\hbadness=10000




选自：
[http://sumanta679.wordpress.com/2009/05/20/latex-justify-without-hyphenation](http://sumanta679.wordpress.com/2009/05/20/latex-justify-without-hyphenation/)[/](http://sumanta679.wordpress.com/2009/05/20/latex-justify-without-hyphenation/)

转自：[http://blog.sina.com.cn/s/blog_5e16f1770102dv97.html](http://blog.sina.com.cn/s/blog_5e16f1770102dv97.html)



