# WINCE60模拟器跑3D的应用程序 - xqhrs232的专栏 - CSDN博客
2009年09月30日 16:55:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1437
其实在WINCE60模拟器下也是可以跑3D应用程序的，只要把对应的驱动程序加进了NK。
跟3D相关的几个DLL是
1.上层的d3dm.dll----实现Direct3DMobileCreate( )接口
2.下层的d3dmref.dll++++d3dm_nulldrv.dll
其中下层的驱动，不同的硬件估计实现会不一样，所以不同的硬件厂家会提供对应的下层驱动。比如三星公司对6410提供的下层驱动就是d3dm_sedrv.dll。
三星提供的这个驱动跟d3dmref.dll++++d3dm_nulldrv.dll的功能是类似的。都是实现了D3DM_Initialize( )函数接口。
当然模拟器的速度肯定不能跟实际的硬件相比，模拟器跑3D应用程序还是很慢很卡的。但至少在没有实际硬件的条件下可以先玩一玩还是不错。
但觉得奇怪的是那些DDRAW的例子程序在模拟器里面跑不起来,怪怪!!!
