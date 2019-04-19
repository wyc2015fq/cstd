# 如何查看下载的android源码是哪个版本的 - xqhrs232的专栏 - CSDN博客
2012年01月31日 15:48:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1996
原文地址::[http://blog.chinaunix.net/space.php?uid=24406894&do=blog&id=2608882](http://blog.chinaunix.net/space.php?uid=24406894&do=blog&id=2608882)
如何查看下载的android源码是哪个版本的
编译的时候从makefile的信息中确实可以看到，另外还可以从git(.repo/manifest.xml)中查询，或者到build/core/version_plaform.mk中去查询plaform_version的定义值
