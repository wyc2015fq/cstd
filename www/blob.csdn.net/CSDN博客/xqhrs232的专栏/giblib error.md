# giblib error - xqhrs232的专栏 - CSDN博客
2016年12月14日 17:07:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：839
原文地址::[http://blog.csdn.net/zhuyingqingfen/article/details/6711636](http://blog.csdn.net/zhuyingqingfen/article/details/6711636)
相关文章
1、[gentoo 屏幕截图工具 scrot](http://blog.csdn.net/zhuyingqingfen/article/details/6711312)----[http://blog.csdn.net/zhuyingqingfen/article/details/6711312](http://blog.csdn.net/zhuyingqingfen/article/details/6711312)
2、[SOLVED] monitorix - libpng error: Write Error----[http://www.linuxquestions.org/questions/linux-server-73/monitorix-libpng-error-write-error-4175526691/](http://www.linuxquestions.org/questions/linux-server-73/monitorix-libpng-error-write-error-4175526691/)

在gentoo 上安装scrot时候，出现如下错误
giblib error: Saving to file foo.png failed
解决办法
Just remergemedia-libs/imlib2 with png support!
就是在make.conf 中的use中添加png
