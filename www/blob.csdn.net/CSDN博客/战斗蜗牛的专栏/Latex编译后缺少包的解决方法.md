# Latex编译后缺少包的解决方法 - 战斗蜗牛的专栏 - CSDN博客





2014年01月22日 00:40:10[vbskj](https://me.csdn.net/vbskj)阅读数：2729








1)  打开miktex的maintenance（admin），然后search “amsmath”，找到这个包之后，install，然后就ok




2) 出现 File ended while scanning use of \@writefile错误的处理方法

把当前目录中.aux结尾的文件删除，重新进行编译即可



