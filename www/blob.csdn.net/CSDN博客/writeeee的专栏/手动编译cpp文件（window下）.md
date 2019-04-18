# 手动编译cpp文件（window下） - writeeee的专栏 - CSDN博客
2015年07月09日 08:52:26[writeeee](https://me.csdn.net/writeeee)阅读数：1471
在windows的编译工具是cl
安装完vc6.0后目录为  C:\Program Files (x86)\Microsoft Visual Studio\VC98\Bin 下。 【cl程序是绿色的，但不清楚其他依赖项】
开始的时候用editPlus 编辑cpp文件，然后用该工具去cmd中编译，编译命令为    F:\test> cl -GX myTest.cpp
[由于环境配置，这样直接执行编译命令很可能会出现链接错误]
解决办法
1.配置上环境变量
2.进入cmd命令 ，执行C:\Program Files (x86)\Microsoft Visual Studio\VC98\Bin\VCVARS32.BAT (直接拖进去回车)
这样找不到lib的链接错误就没有了。
