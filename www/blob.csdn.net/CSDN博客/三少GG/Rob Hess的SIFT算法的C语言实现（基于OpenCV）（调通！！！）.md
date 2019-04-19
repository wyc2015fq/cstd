# Rob Hess的SIFT算法的C语言实现（基于OpenCV）（调通！！！） - 三少GG - CSDN博客
2010年12月09日 11:46:00[三少GG](https://me.csdn.net/scut1135)阅读数：2255
顺序：
1. [http://www.opencv.org.cn/forum/viewtopic.php?f=10&t=1472](http://www.opencv.org.cn/forum/viewtopic.php?f=10&t=1472)
2. [http://www.opencv.org.cn/index.php/User:Lmyanglei](http://www.opencv.org.cn/index.php/User:Lmyanglei)
问题解决：
1.我运行时也出现了这种情况，我看了一下函数cvEigenVV的定义，的确是参数的个数少了，原因可能是程序编写时用的opencv的版本和正在使用的opencv版本不同，我的是opencv2.0 ,而程序很可能是用opencv1.0 编写的，所以会出现以上问题，但目前我不太会解决，因为我不知道该加上的参数的值是什么。正在研究中。。。。
已经找到解决方法了，我是基于visual studio 2008版本运行的。
运行siftFeature工程时 出现两个错误，一是 cvSaveImage 输入参数太少，一是cvEigenVV函数输入参数太少，可以在程序中对应的函数名上点右键，选择转到定义，就能看到，opencv2.0中的函数定义参数个数变多了。可以在程序中，将cvSaveImage( out_img_name, img);改成
cvSaveImage( out_img_name, img,0); 将cvEigenVV( &M, &V, &E, DBL_EPSILON);改成
cvEigenVV( &M, &V, &E, DBL_EPSILON,-1,-1);即可。
如果编译时出现不能打开cvd.lib或xxx.lib的情况，原因是opencv2.0版本的cvd.lib已经变成了cv200d.lib,这个变化在 opencv安装目录/lib中可以看到。只需在相应的工程->属性->连接器->输入的debug和release中将相应的xxxd.lib改成xxx200d.lib,在release下是xxx.lib改成xxx200.lib；注意opencv原来有的库文件，opencv2.0可能不存在或改换了别的名，如cvcam.lib,我目前不知道在opencv2.0中，它在哪里，因此，我就将这一各lib从附加项中删掉了，没有影响程序运行。目前程序调试成功。以上是我摸索得到的结果，如果有不正确的地方，欢迎批评指正！
2.
编译还是提示错误，系统文件里面。
大概是说在":"之前缺少“{”，原因是match.c 文件中
既加入了 stdio.h 和 iostream.h （vs中要改成iostream）
把#include <iostream.h> 一句注释掉即可编译通过。
3.
VC下ＧＳＬ的配置问题解决了 ![:)](http://www.opencv.org.cn/forum/images/smilies/icon_smile.gif) ＧＳＬ版本：gsl-1.8.exe
在Tools下选择Option选项，在“Include Files”下添加“C:/Program Files/GNUWIN32/Include”；“Library Files"下添加“C:/Program Files/GnuWin32/lib”　编译运行就OK了 ![:)](http://www.opencv.org.cn/forum/images/smilies/icon_smile.gif) 曾经为此困扰，特写下来，希望对后来人有帮助 ![:)](http://www.opencv.org.cn/forum/images/smilies/icon_smile.gif)
下面就可以编译运行Rob Hess的源码了。
