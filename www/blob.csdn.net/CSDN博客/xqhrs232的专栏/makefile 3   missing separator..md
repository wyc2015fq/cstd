# makefile:3: *** missing separator. - xqhrs232的专栏 - CSDN博客
2016年06月12日 16:21:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1163
原文地址::[http://blog.csdn.net/eifjgoergfgrgr/article/details/18230135](http://blog.csdn.net/eifjgoergfgrgr/article/details/18230135)
在make命令后出现这种错误提示，是提示第2行没有分隔符。
例如：
1 target：prerequisites
2 command
……
改为：
1 target：prerequisites
2    command
……
就可以了。
在Makefile文件中，命令必须以【tab】键开始。
//==================================================================================================================
备注::
1>最好在linux下用gedit命令去编辑makefile文件，在行开始的地方按TAB键，然后命令会变颜色！！！
