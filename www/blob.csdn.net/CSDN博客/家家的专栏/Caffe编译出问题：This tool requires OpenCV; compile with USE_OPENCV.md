# Caffe编译出问题：This tool requires OpenCV; compile with USE_OPENCV. - 家家的专栏 - CSDN博客





2015年11月12日 21:49:11[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4012








解决方法：

在Makefile.config文件中，添加：

USE_OPENCV := 0





即可正常编译。




参考文件：

https://groups.google.com/forum/m/#!topic/caffe-users/-zvZ9_RQapI


https://github.com/BVLC/caffe/blob/master/Makefile.config.example




