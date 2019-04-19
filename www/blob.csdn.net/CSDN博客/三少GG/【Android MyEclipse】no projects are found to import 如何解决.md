# 【Android MyEclipse】no projects are found to import 如何解决 - 三少GG - CSDN博客
2011年09月20日 14:38:06[三少GG](https://me.csdn.net/scut1135)阅读数：5721标签：[import																[myeclipse																[android																[eclipse																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=myeclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
                
no projects are found to import 如何解决
方法1.（经本人验证可用20110920）
记得以前直接import，import exsiting projects就可以，不知道为什么这次导入的时候，总是出现这个，在网上搜索，学习http://www.devdiv.net/bbs/thread-34441-1-1.html，才知道，**在new Android Project里面换一个选项create project from existing，即可。。学习了，嘿嘿。。**
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
方法2. 新建一个项目，
然后直接复制于此项目即可！
NND,这个居然让哥纠结了半天哎！！
或者：
你是在试图导入一个项目吧。
这是因为你导入的目录下面没有一个**隐藏的.project文件**，所以不能作为一个java项目导入。
解决办法是新建一个项目，然后选择引用一个已有的资源。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

