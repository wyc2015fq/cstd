# 图像处理库的比较：OpenCV,FreeImage,CImg,CxImage - DoubleLi - 博客园







1.对[OpenCV](http://lib.csdn.net/base/opencv) 的印象：功能十分的强大，而且支持目前先进的图像处理技术，体系十分完善，操作手册很详细，手册首先给大家补[计算机视觉](http://lib.csdn.net/base/computervison)的知识，几乎涵盖了近10年内的主流[算法](http://lib.csdn.net/base/datastructure)；

然后将图像格式和矩阵运算，然后将各个算法的实现函数。
我用它来做了一个Harris角点检测器和Canny边缘检测器，总共就花了一个小时（第一次用OpenCV）。
而且该库显示图像极其方便，两句话就可以。
但该库似乎不大稳定，对32F和16S、8U的图像数据支持上bug重重。
我用 cvFilter2D函数进行线性滤波，屡屡出错，后来一查原来是大bug。
后来用cvmGet来取矩阵元素也是频繁出错，仔细检查了N遍确保程序没问题之后在yahoogroup上找到答案：仍然是bug。。。
但好歹该库是开放的，所以自己可以修改；而且支持CVS。另外该库用的是IPL矩阵库，速度奇快～～

[http://sourceforge.net/projects/opencvlibrary/](http://sourceforge.net/projects/opencvlibrary/)

2.对CxImage考察的印象：该开发包完全开放源代码，图像封装为一个类，功能极为强大，与Windows、MFC支持极好
，支持图像的多种操作（线性滤波、中值滤波、直方图操作、旋转缩放、区域选取、阈值处理、膨胀腐蚀、alpha混合等等）
，支持从文件、内存或者win32api 定义的位图图像格式中读取图像，支持将图像显示在任意窗口
，功能可谓很强大了，而且对像素的操作很方便
，另外还有一个界面很强的demo，可以直接在上面进行二次开发，推荐使用！

缺点：里面的子库很多，用起来可能较麻烦；而且感觉速度稍慢，不如后面提到的freeimage
但功能真的十分强大啊！

[http://www.xdp.it/cximage.htm](http://www.xdp.it/cximage.htm)

3. CImg：就一个.h文件所以用起来很简明，但感觉功能上不如CxImage。
可以与CxImage配合使用，因为CImg提供了基于lapack的矩阵运算函数和完善的线性滤波卷积函数，同时CImg做像素运算还是很方便的。
另外，独有Display类可以方便的实现各种显示，包括显示图像、打字、画线等等。还有，该库有个基于光流的多尺度图像配准例子，很好。

[http://cimg.sourceforge.net/](http://cimg.sourceforge.net/)

4.FreeImage：[C语言](http://lib.csdn.net/base/c)的体系，大量使用指针运算速度可以保证，内含先进的多种插值算法。
另外独有的支持meta exif信息的读取。该库最大的特点就是比较简练，只把重点放在对各种格式图像的读取写入支持上，没有显示部分，实际编程的时候还是需要调用API函数进行显示。

[http://freeimage.sourceforge.net/](http://freeimage.sourceforge.net/)














