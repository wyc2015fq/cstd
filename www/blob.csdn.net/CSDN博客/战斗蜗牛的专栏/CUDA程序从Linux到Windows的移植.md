# CUDA程序从Linux到Windows的移植 - 战斗蜗牛的专栏 - CSDN博客





2012年03月17日 13:18:25[vbskj](https://me.csdn.net/vbskj)阅读数：1556








1.如果用到多GPU线程可以参考“MonteCarloMultiGPU”，其实只需要里面包含<multithreading.h>头文件即可识别cutStartThread等相关函数和数据定义。

2.如果用到原子操作可以参考“simpleAtomicIntrinsics”，只需要工程里不包含cutil、shril等即可；

3.如果用到CUFFT，可以参考“OCEAN FFT”，需要添加“cufft.h”头文件，连接器中增加"cufft.lib"



