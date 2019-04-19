# linux用tar 解压tar文件报错 - fanyun的博客 - CSDN博客
2017年03月31日 12:35:27[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2928
所属专栏：[Linux系统开发维护实战](https://blog.csdn.net/column/details/linux-01.html)
出现的问题：
用tar 解压 tar.bz2文件出错
debian:/usr/src# tar jxf linux-2.6.26.tar.bz2
tar: bzip2: Cannot exec: No such file or directory
tar: Error is not recoverable: exiting now
tar: Child returned status 2
tar: Error exit delayed from previous errors
解决办法
安装bzip2软件包:
![](https://img-blog.csdn.net/20170324124751425?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
安装完后，解压成功！！
