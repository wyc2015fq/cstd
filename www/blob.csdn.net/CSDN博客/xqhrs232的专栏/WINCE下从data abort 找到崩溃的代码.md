# WINCE下从data abort 找到崩溃的代码 - xqhrs232的专栏 - CSDN博客
2013年04月16日 11:01:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：827
原文地址::[http://blog.csdn.net/itqiangren/article/details/3885265](http://blog.csdn.net/itqiangren/article/details/3885265)
编译的时候注意先在project settings里的Link页里勾选“Generate mapfile”，会生成一个map文件。
把编译出来的RELEASE版本可执行文件放到CE5平台下运行。出错的时候串口打印了一句
Data Abort: Thread=83ad1d38 Proc=820266d0 'walzer_leak.exe'
AKY=00000021 PC=00011008(walzer_leak.exe+0x00001008) RA=00011030(walzer_leak.exe+0x00001030) BVA=81000000 FSR=0000000d
这句是系统自动输出的。我们得到了一个关键的信息：PC指针。和PC指针在walzer_leak.exe中的偏移量。然后打开编译时生成的walzer_leak.map文件
，
OK，首先，里面有一句“Preferred load address is 00010000”，这意味着DATA ABORT那句的PC=00011008(walzer_leak.exe+0x00001008) 我们必须把括
号里的0x1008加上这个load address的偏移量，得到0x11008（注意不能直接用PC，一会儿再给个案例就知道了），然后我们在函数偏移列表里看Rva+Base
这栏，找到0x11008落在了DoLeak函数的地址范围里，所以是DoLeak函数泄漏了
//========================================================================
备注：：
1》很多时候用PC/RA的其中一个就可以定位到异常的大致位置的！！！
