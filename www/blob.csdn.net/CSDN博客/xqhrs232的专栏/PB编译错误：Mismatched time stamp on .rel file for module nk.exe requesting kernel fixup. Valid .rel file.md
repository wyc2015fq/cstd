# PB编译错误：Mismatched time stamp on .rel file for module nk.exe requesting kernel fixup. Valid .rel file - xqhrs232的专栏 - CSDN博客
2014年05月17日 14:21:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：702
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/armeasy/article/details/5683817](http://blog.csdn.net/armeasy/article/details/5683817)

如题，PB编译完后make时出现如下错误：
Mismatched time stamp on .rel file for module nk.exe requesting kernel fixup.  Valid .rel file is required for kernel fixups.
先Copy一下，再Make即可。
总结：很多情况下，在PB工程编译完后没有错误，而在Make时就出现错误了，如果这个错误一时无法找到原因，不管三七二十一，先
Copy一下再Make，试了再说。十有八九这个错误就会解决。
