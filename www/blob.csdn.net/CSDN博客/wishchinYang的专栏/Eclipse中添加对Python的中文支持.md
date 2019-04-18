# Eclipse中添加对Python的中文支持 - wishchinYang的专栏 - CSDN博客
2014年08月18日 08:53:33[wishchin](https://me.csdn.net/wishchin)阅读数：1056
原文链接：[http://down.51cto.com/data/751371](http://down.51cto.com/data/751371)
首先要确保eclipse编辑器环境的编码为utf8，这个是大前提；其次如果py文件中含有中文字符的话，需要在py文件中对编码进行声明。
1. 修改eclipse编辑器编码
　　a）设置工作空间编码：窗口->首选项->工作空间->UTF-8
![](http://pic002.cnblogs.com/images/2012/434701/2012081700553925.png)
![](http://pic002.cnblogs.com/images/2012/434701/2012081700560324.png)
　　b) 设置编辑器编码：窗口->首选项->编辑器->文本编辑器->拼写->UTF-8
![](http://pic002.cnblogs.com/images/2012/434701/2012081700570527.png)
　　  c) 设置配置编码：打开eclipse安装目录->eclipse.ini，末行加上   -Dfile.encoding=UTF-8。
　　  d) 修改python文件编码：在py文件首行加上#coding=utf-8，一定要在首行才可以。
