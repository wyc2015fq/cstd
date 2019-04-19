# gdb 调试coredump文件中烂掉的栈帧的方法____本人备注 - xqhrs232的专栏 - CSDN博客
2016年10月24日 18:03:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1289
原文地址::[http://blog.csdn.net/muclenerd/article/details/48005171](http://blog.csdn.net/muclenerd/article/details/48005171)
相关文章
1、[无符号和栈破坏情况下coredump的分析方法](http://blog.csdn.net/qazxlf/article/details/50385616)----[http://blog.csdn.net/qazxlf/article/details/50385616](http://blog.csdn.net/qazxlf/article/details/50385616)
2、x86ManualBacktrace----[http://devpit.org/wiki/x86ManualBacktrace](http://devpit.org/wiki/x86ManualBacktrace)
3、[c函数调用过程原理及函数栈帧分析](http://blog.csdn.net/qiu265843468/article/details/17844419)----[http://blog.csdn.net/qiu265843468/article/details/17844419](http://blog.csdn.net/qiu265843468/article/details/17844419)
gdb调试core文件，直接backtrace,然后查看用frame n切换栈帧。
info local可以查看当前栈的局部变量。对于烂掉的栈帧，可以参考如下文章来推测对应的符号。
注意编译时打开-fstack-protector
 和 -fstack-protector-all选项，避免栈帧被冲刷掉。
1.关于函数调用栈帧参考如下
http://blog.csdn.net/qiu265843468/article/details/17844419
2.linux下的AT&T汇编参考这篇
http://www.doc88.com/p-378141850932.html
3.函数的调用约定(主要是参数的传递顺序和堆栈清理部分)
http://blog.chinaunix.net/uid-23929779-id-149680.html
4.gdb 调试后info reg后参考如下几篇：
http://devpit.org/wiki/x86ManualBacktrace
http://blog.csdn.net/tobyaries/article/details/8990949
http://blog.csdn.net/tobyaries/article/details/8990869
http://blog.chinaunix.net/uid-16459552-id-3328601.html
主要参考第一篇
5.gdb x的功能介绍
http://www.cnblogs.com/tekkaman/p/3506120.html
linux下按4个字节来显示，用x/2048xw addr即可。
举例子说明吧。
1.打开coredump的core文件生成
ulimit -c unlimited
2.运行程序，产生core文件。将获取到的core文件跟bin文件放在同一个文件夹下，然后打开gdb
gdb visMgr core.25090
3.输入bt命令，看到
(gdb) bt
#0  0x00ccc57f in tc_pvalloc (size=28) at src/tcmalloc.cc:1695
#1  0x00de8d06 in ?? ()
#2  0x0000001c in ?? ()
#3  0xb58cd518 in ?? ()
#4  0x00de9b11 in ?? ()
#5  0x0000000f in ?? ()
#6  0x00000000 in ?? ()
看到一堆0地址和??表明这个函数里的栈帧已经坏掉，无法看到当前执行到那个函数发生问题 。
4. info reg查看ebp的值
eax            0x2436
ecx            0xfffe65534
edx            0x2436
ebx            0xcd8ff413471732
esp            0xb58cd4900xb58cd490
ebp            0xb58cd4e80xb58cd4e8
esi            0x9eb15c0166401472
edi            0xc12
eip            0xccc57f0xccc57f <tc_pvalloc+31>
eflags         0x210206[ PF IF RF ID ]
cs             0x73115
ss             0x7b123
ds             0x7b123
es             0x7b123
fs             0x00
gs             0x3351
5. 查看内存镜像的地址，反推栈帧
从上面的命令得知ebp的地址，用该命令查看内存镜像地址
x/2048xw 0xb58cd4e8
输出如下：
0xb58cd4e8: 0xb58cd508 0x00de8d06 0x0000001c 0xb58cd518
0xb58cd4f8: 0xb58cd538 0x00f21bf5 0x00e3dff4 0x0883085a
0xb58cd508: 0xb58cd548 0x00de9b11 0x0000000f 0x00000000
0xb58cd518: 0xb58cd5af 0x00000000 0x09ef829f 0x0104ac6b
0xb58cd528: 0xb58cd558 0x010548c0 0x09ef8040 0x09ef8000
0xb58cd538: 0xb58cd578 0x00e3dff4 0x0883085a 0xb58cd5a8
0xb58cd548: 0xb58cd568 0x00de9cf6 0x0883085a 0x08830869
0xb58cd558: 0xb58cd5af 0x0087ad00 0x00899ff4 0xb58cdb7c
0xb58cd568: 0xb58cd5c8 0x00885a2d 0xb58cd5a8 0x0883085a
0xb58cd578: 0xb58cd5af 0x00f1c382 0xb58cd5bc 0x00893109
0xb58cd588: 0xb58cd5a8 0x08532bf3 0xb58cdb94 0x00000003
0xb58cd598: 0x00000000 0x00000000 0x00893109 0x00000005
0xb58cd5a8: 0xb58cd5c8 0x08526f6d 0xb58cdb94 0x00000000
0xb58cd5b8: 0x00885a0b 0xb58cdb7c 0xb58cdb7c 0x00000011
0xb58cd5c8: 0xb58cd5e8 0x0851f996 0xb58cdb7c 0x0883085a
0xb58cd5d8: 0x0881c8ba 0x00f1a426 0xb58cdb7c 0xb58cdb7c
0xb58cd5e8: 0xb58cd658 0x0854bbc5 0xb58cdb7c 0x0883085a
0xb58cd5f8: 0x0881c8ba 0x0088cdee 0x09ef8040 0x00893109
0xb58cd608: 0x00000005 0x00000000 0x00000000 0x00000000
0xb58cd618: 0xb58cd638 0x09ef8000 0xb58cdb04 0x001edc66
0xb58cd628: 0x00000000 0x00218ff4 0xb58cd91c 0x09ed6630
0xb58cd638: 0xb58cd678 0x00213145 0x09ed6630 0x00002000
0xb58cd648: 0x00000000 0xb58cdb7c 0xb58cdb7c 0x00000011
0xb58cd658: 0xb58cd688 0x0853806b 0xb58cdb7c 0x089d0a88
0xb58cd668: 0xb58cdbd8 0x0000000a 0x0000001c 0x00000011
0xb58cd678: 0xb58cd6a8 0xb58cdbd8 0xb58cdb7c 0x0000001c
0xb58cd688: 0xb58cd6a8 0x0852a9fe 0xb58cdb7c 0x089d0a88
0xb58cd698: 0xb58cdbd8 0x00000001 0x0000000a 0x0000000a
0xb58cd6a8: 0xb58ddd88 0x0851cc7f 0xb58cdb7c 0x089d0a88
0xb58cd6b8: 0xb58cdbd8 0x0000001c 0x00000011 0x00000001
0xb58cd6c8: 0x0000001d 0x00000181 0xb58eeb70 0xb58ddcec
0xb58cd6d8: 0x00001ce3 0xb58cdbec 0xb58cd5c0 0x00000075
0xb58cd6e8: 0x00000000 0x00000000 0x00000000 0x00000181
0xb58cd6f8: 0x0000001d 0x00000001 0x00000000 0x00000001
0xb58cd708: 0x00000000 0x00000000 0x007a9ae7 0x00000000
0xb58cd718: 0xb58ee0d4 0x09ee8140 0x00000000 0xffffffa8
0xb58cd728: 0xb58cd9f8 0xffffffa8 0x00000000 0x00000000
0xb58cd738: 0x00000001 0x00000000 0x007e53a0 0x00000000
0xb58cd748: 0x00000000 0x00000000 0x00000000 0x00000000
0xb58cd758: 0xb58cd920 0x007a96ef 0x0000000a 0xffffffff
0xb58cd768: 0x00000100 0x00000001 0x00000001 0xb58cd5c0
0xb58cd778: 0xffffffff 0x00000000 0x00000000 0x00000000
0xb58cd788: 0x00000000 0x00000000 0x00000000 0x00000000
0xb58cd798: 0x00000000 0x00000000 0x00000000 0x00000000
0xb58cd7a8: 0x00000000 0x00000000 0x00000000 0x00000000
0xb58cd7b8: 0x00000000 0x00000000 0x00000000 0x00000000
6.反推得到的栈帧如下
0xb58cd5080x00de8d06
0xb58cd5480x00de9b11
0xb58cd5680x00de9cf6
0xb58cd5c80x00885a2d
0xb58cd5e80x0851f996
0xb58cd6580x0854bbc5
0xb58cd6880x0853806b
0xb58cd6a80x0852a9fe
0xb58ddd880x0851cc7f
7. 用info symbol xxx反推得到各个函数名
info symbol0x00de8d06 queryIPVddd
info symbol0x00de9b11 queryIPVccc
info symbol0x00de9cf6 queryIPVbbb
info symbol0x00885a2d queryIPVaaa
info symbol0x0851f996 queryIPVzzz
info symbol0x0854bbc5 queryIPVyyy
info symbol0x0853806b queryIPVxxx
info symbol0x0852a9fe queryIPVtable
info symbol0x0851cc7f addOrUpdateIPVDevice
只要我们够耐心，我们可以查看整个4GB(32-bit机器)的内存镜像，然后一只反推到main函数。
//=========================================================
本人备注::
1>这个方法还是挺强大的
[](http://blog.csdn.net/imturkey/rss/list)

