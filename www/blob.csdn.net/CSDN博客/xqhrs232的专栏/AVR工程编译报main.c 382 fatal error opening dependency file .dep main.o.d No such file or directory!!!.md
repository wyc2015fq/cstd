# AVR工程编译报main.c:382: fatal error: opening dependency file .dep/main.o.d: No such file or directory!!! - xqhrs232的专栏 - CSDN博客
2019年03月26日 15:40:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：44
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://www.openedv.com/posts/list/46357.htm](http://www.openedv.com/posts/list/46357.htm)
楼主用的64位的win8系统，使用winAVR的时候出现这个问题，困扰了两天百度了好多都没有发现可以解决的方法，最后无奈用谷歌查找，终于被找到了原因，据说是64位系统下的winAVR的一个bug,具体原因由于本人的英语不是很好，这里直接贴出链接，记得要……你懂的，不然打不开的    http://www.avrfreaks.net/forum/newbie-avr-gcc
			最后我说一下具体解决的方法，就是用附件中的替换你的winAVR安装下的xx:\WinAVR-20100110\utils\bin的msys-1.0.dll这个文件就可以了
![](http://www.openedv.com/static/image/filetype/zip.gif)
[msys-1.0-vista64.zip](http://www.openedv.com/forum.php?mod=attachment&aid=OTAwMXwyZTQ2MzA4MXwxNTUzNTg1NzIwfDB8NDYzNTc%3D)
