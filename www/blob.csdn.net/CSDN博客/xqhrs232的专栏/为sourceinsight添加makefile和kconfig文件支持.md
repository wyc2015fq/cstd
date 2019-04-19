# 为sourceinsight添加makefile和kconfig文件支持 - xqhrs232的专栏 - CSDN博客
2010年05月13日 14:18:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：5534标签：[makefile																[include																[filter																[汇编																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=filter&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)
个人分类：[SI/Notepad++/EditPlus](https://blog.csdn.net/xqhrs232/article/category/906927)
原文地址::
[http://blog.chinaunix.net/u3/99423/showart_2207081.html](http://blog.chinaunix.net/u3/99423/showart_2207081.html)
大家用source insight看linux源码的时候都遇到这么一个麻烦，makefile和kconfig中的文字在搜索的时候不能找到，这是因为source insight默认是不选makefile的，也没有konfig这种类型的文件。下面就讲一下
如何添加makefile：
打开Source Insight，在新建工程之前，进入
Options -> Document Options-> 点击Document Type的下拉框，然后选择Make File，在右边的File Filter中，在原先的*.mak后面加上一个分号，即多个不同过滤规则以分号间隔开，再加上*makefile，变成 *.mak;*makefile，并且选中Include when adding to projects,这样，以后再新建工程的时候（当前工程同步下makefile就有了），就可以识别makefile或Makefile了（好像此处Source
 Insight并不区分大小写）。
如何添加kconfig：
由于kconfig是sourceinsight默认不支持的格式，所以我们现在先要加入一个类型。
点击 Add Type，填入新文件类型的名字kconfig，File Filter中写上*kconfig，最好再加上kconfig*（因为在linux中有些kconfig是以kconfig.*的形式出现），注意再选中下面的Include when adding to projects,这样就建立了一个新的文件类型， 以后新建工程加入文件时候，系统就能够识别kconfig的文件了。
//============================================
1》还有，si默认的汇编文件后缀不支持.s的，所以要支持arm的汇编文件也需要改一下File filter.2》记得要勾选Include when adding  to  projects项，才会把新建的文件类型加进工程！！！3》Add and remove project files时show only known document types这个复选框不要去勾选，勾了的话有些文件类型会加不进工程
