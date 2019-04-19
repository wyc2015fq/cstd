# 解决Wince6.0编译错误方法 - xqhrs232的专栏 - CSDN博客
2016年07月26日 09:04:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：515
原文地址::[http://blog.chinaunix.net/uid-17188120-id-2820887.html](http://blog.chinaunix.net/uid-17188120-id-2820887.html)
相关文章
1、WINCE 6 编译时遇到的EDITBIN : fatal error LNK1104: cannot open file----[http://bbs.csdn.net/topics/330061782](http://bbs.csdn.net/topics/330061782)
出现编译错误如下：
EDITBIN : fatal error LNK1104: cannot open file 'C:\WINCE600\OSDesigns\TCC89XX\TCC89XX\Wince600\MAGELLAN_ARMV4I\cesysgen\oak\target\ARMV4I\retail\ws2.dll'
NMAKE : fatal error U1077: 'editbin' : return code '0x450'
Stop.
NMAKE : fatal error U1077: 'C:\WINCE600\sdk\bin\i386\nmake.exe' : return code '0x2'
Stop.
NMAKE : fatal error U1077: 'C:\WINCE600\sdk\bin\i386\nmake.exe' : return code '0x2'
Stop.
解决办法：
关闭杀毒软件。
//=========================================================================================================
备注::
1>最近在WIN7下编译WINCE60也遇到这个问题！！！

