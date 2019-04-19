# 解决不能产生EBOOT.NB0文件的问题 - xqhrs232的专栏 - CSDN博客
2010年04月29日 16:16:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1257标签：[wince																[磁盘																[api																[os																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=os&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
我的一个同事遇到过在编译EBOOT的时候不能产生EBOOT.NB0文件的问题，在EBOOT的CONFIG.BIB文件中也对ROMSTART、ROMSIZE、ROMWIDTH这几个选项做了设置也不行。而我的PB在编译同一个BSP的时候是没问题的，后来终于弄明白了。我用的是PB5，这个问题在PB4中也有，如果是PB4可以用下面的方法1解决。
解决办法：
方法1、把工程目录放在和PB安装目录相同的磁盘下。
方法2、使用WinHex或UltraEdit等十六进制编辑器修改
    “/WINCE500/PUBLIC/COMMON/OAK/BIN/I386/romimage.exe”文件，
    把偏移地址0x25a7b上的“6A   01”改成“6A   03”。
PB6中的解决方法：
    “/WINCE600/PUBLIC/COMMON/OAK/BIN/I386/romimage.exe”文件，
    把偏移地址0x20B4C上的“6A 01”改成“6A 03”。
原因：
在romimage.exe文件中的compact_bin函数里使用了MoveFileEx Win32API来移动文件，
但是在第一个参数中没有使用MOVEFILE_COPY_ALLOWED标记，所以在不同的磁盘之间移动文件会报错。
方法2中 “6A 01”就是把MoveFileEx的第一个参数压堆栈的机器码，改成“6A 03”就是直接把MOVEFILE_COPY_ALLOWED标记对应的bit加上了。
关于“MoveFileEx”可以看看MSDN相关的说明。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/fengwx/archive/2009/06/17/4275133.aspx](http://blog.csdn.net/fengwx/archive/2009/06/17/4275133.aspx)
//##########################################################################
自己的说明::
 1>在PB6下面修改romimage.exe再编译会产生不了NK.NB0-----编译直接报错!!!----上面提到的第2个方法不可行!!!
 2>在WINCE600下把OS工程放到WINCE600安装的C盘下是可以编译产生对应的NB0文件的----上面提到的第1个方法可行!!!
