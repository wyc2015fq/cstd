# VC读写TXT文件和CStdioFile读写乱码----本人亲自测试可行 - xqhrs232的专栏 - CSDN博客
2017年08月20日 22:47:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2272
原文地址::[http://blog.csdn.net/callmezane/article/details/7172920](http://blog.csdn.net/callmezane/article/details/7172920)
相关文章
1、[[C++]
 cout、wcout无法正常输出中文字符问题的深入调查（1）：各种编译器测试](http://www.cnblogs.com/zyl910/archive/2013/01/20/wchar_crtbug_01.html)----[http://www.cnblogs.com/zyl910/archive/2013/01/20/wchar_crtbug_01.html](http://www.cnblogs.com/zyl910/archive/2013/01/20/wchar_crtbug_01.html)

 最近突然想用用TXT文件存储数据，网上找了段源码，采用CStdioFile类简单方便，但出现一个很奇怪的问题，从TXT读取的文件一旦遇到中文就变成乱码，可重新写进TXT文档却又全部正常，后来发现是ANSI格式UNICODE格式兼容问题，貌似VC2005以上版本都有这毛病，网上找了很多解决方法，不过某位大神设置语言区域的方法最简单实用，源码贴出来供大家参考
![](http://hi.csdn.net/attachment/201201/5/0_1325741094tI5j.gif)
![](http://hi.csdn.net/attachment/201201/5/0_132574111819Hj.gif)
![](http://hi.csdn.net/attachment/201201/5/0_1325741129oZXt.gif)
