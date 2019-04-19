# android-opencv【 undefined reference to 】 - 三少GG - CSDN博客
2012年07月29日 17:07:00[三少GG](https://me.csdn.net/scut1135)阅读数：2936
**1. opencv不同版本废弃的方法**
2.3.1版本无此方法！！！
** cvCopyImage**(_candiImg, *candiImg);         更改为:  cvCopy
参考文章：[OpenCV中的cvCloneImage、cvCopyImage和cvCloneMat、cvCopyMat的误区](http://blog.csdn.net/jajen/article/details/5711678)
你好,为什么我的opencv2.1中使用cvCopyImage显示此函数没有定义??难道是在新的版本里面去除了?
问题解决了,现在只有**cvCopy**可以用
## [cvCopy与cvCloneImage的区别](http://www.cnblogs.com/zhangzhi/archive/2009/09/19/1569891.html)
/* Copies source array to destination array */
CVAPI(void)  **cvCopy**( const CvArr* src, CvArr* dst,
                     const CvArr* mask CV_DEFAULT(NULL) );
/* Creates a copy of IPL image (widthStep may differ) */
CVAPI(IplImage*) **cvCloneImage**( const IplImage* image );
如果设定了ROI等参数的时候，**cvCopy只是复制被设定的区域**，复制到一个和所设定参数相吻合的新的IplImage中
而cvCloneImage则是将整个IplImage结构复制到新的IplImage中，其中的ROI等参数也会一同复制。新的IplImage将会和原来的一模一样。
cvCopy的原型是：
void cvCopy( const CvArr* src, CvArr* dst, const CvArr* mask=NULL );
在使用这个函数之前，你必须用cvCreateImage（）一类的函数先开一段内存，然后传递给dst。cvCopy会把src中的数据复制到dst的内存中。
cvCloneImage的原型是：
IplImage* cvCloneImage( const IplImage* image );
在使用函数之前，不用开辟内存。该函数会自己开一段内存，然后复制好image里面的数据，然后把这段内存中的数据返回给你。
clone是把所有的都复制过来，也就是说不论你是否设置Roi,Coi等影响copy的参数，clone都会原封不动的克隆过来。
copy就不一样，只会复制ROI区域等。用clone复制之后，源图像在内存中消失后，复制的图像也变了，而用copy复制，源图像消失后，复制的图像不变
**2.opencv C和 C++ 引用文件的不同～～～ 以及extern "C"知识的引申**
应该为 [https://www.oschina.net/code/explore/OpenCV-2.1.0/interfaces/swig/filtered/cv.h](https://www.oschina.net/code/explore/OpenCV-2.1.0/interfaces/swig/filtered/cv.h)
```cpp
extern "C" void cvLine( CvArr* img, CvPoint pt1, CvPoint pt2,
	                     CvScalar color, int thickness = 1,
	                     int line_type = 8, int shift = 0 );
```
编译出问题的几个函数均为C语言版本的， 
C: int cvNamedWindow(const char* name, int flags)
C: void cvShowImage(const char* winname, const CvArr* image)
C: int cvWaitKey(int delay=0 )
C: void cvDestroyWindow(const char* name)
函数原型分别位于  
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SIOX.cpp:(.text._ZN3org4siox4Siox17extractForegroundERKNS0_9SioxImageEjP9_IplImagePS6_+0xace): undefined reference to `cvSmooth'
SIOX.cpp:(.text._ZN3org4siox4Siox17extractForegroundERKNS0_9SioxImageEjP9_IplImagePS6_+0xb72): undefined reference to `cvConvexHull2'
SIOX.cpp:(.text._ZN3org4siox4Siox17extractForegroundERKNS0_9SioxImageEjP9_IplImagePS6_+0xc2a): undefined reference to `cvLine'
这几个函数该如何链接呢？？？
在 <opencv2/core/core_c.h>中，
CVAPI(void)  cvLine( CvArr* img, CvPoint pt1, CvPoint pt2,
                     CvScalar color, int thickness CV_DEFAULT(1),
                     int line_type CV_DEFAULT(8), int shift CV_DEFAULT(0) );
解嵌套宏之后，CVAPI(void)就是用extern "C" CV_EXPORTS void __cdecl替换就行，其中，CV_EXPORTS因为定义的时候为空，所以在这里只是占位，没有用到，可以不考虑。
（小插曲：[http://hi.baidu.com/%C1%EE%BA%FC%B1%CF%BA%E8/blog/item/4dfaefdec3686d245882dd7e.html](http://hi.baidu.com/%C1%EE%BA%FC%B1%CF%BA%E8/blog/item/4dfaefdec3686d245882dd7e.html)
在cv.h中看到如下这样一些函数声明 ：
CVAPI(void) cvGetHuMoments( CvMoments* ...)
CVAPI(int) cvSampleLine( const CvArr* ...)
CVAPI(double) cvGetSpatialMoment( CvMoments* ...)
不知道这个CVAPI(type)是什么类型，转到定义
在cxtypes.h中看到它的定义是一个比较奇怪的宏：
#ifndef CVAPI
    #define CVAPI(rettype) CV_EXTERN_C CV_EXPORTS rettype CV_CDECL
#endif
）
类似问题描述（但并未解决）：
[Open CV - not building shared library](https://groups.google.com/forum/#!msg/android-opencv/FHsooOTlv2Y/FwDyo9BKT_MJ)
"undefined reference to" is a linker error. You probably forgot to pass a library to link in on the command line. – [Daniel
 Fischer](http://stackoverflow.com/users/1011995/daniel-fischer)[Jul 19 at 13:28](http://stackoverflow.com/questions/11561626/android-ndkundefined-reference-to#comment15292880_11561626)
> 
LOCAL_STATIC_LIBRARIES: The list of static libraries modules (built with BUILD_STATIC_LIBRARY) that should be linked to this module. This only makes sense in shared library modules.
LOCAL_SHARED_LIBRARIES: The list of shared libraries *modules* this module depends on at runtime. This is necessary at link time and to embed the corresponding information in the generated file.
怀疑：
Smooth
Smooths the image in one of several ways.
C: void cvSmooth(const CvArr* src, CvArr* dst, int smoothtype=CV_GAUSSIAN, int param1=3, int
param2=0, double param3=0, double param4=0)
Note: The function is now** obsolete.** （**过时的**，废弃的，不使用的）Use GaussianBlur(), blur(), medianBlur() or bilateralFilter().
++++++++++++++++++++++++++
**真相大白**解决方案：（2012.7.29晚）
Android.mk文件中：
LOCAL_STATIC_LIBRARIES += libsioxdetection opencv_calib3d opencv_features2d opencv_flann opencv_imgproc opencv_core opencv_highgui
。。。include $(BUILD_SHARED_LIBRARY)
需要手动添加opencv的静态库！！！
反思：
siox文件已成功编译，故不应是此部分的编译错误。
本应立刻确定为**链接问题**。故应考虑是否是编译好的库未能正常设置！！！
灵感参考：
# [OpenCV undefined references](http://stackoverflow.com/questions/10234894/opencv-undefined-references)
## [【原创】“collect2: ld returned 1 exit status”错误](http://www.cnblogs.com/IamEasy_Man/archive/2011/01/26/1945609.html)
      今天在编译android时碰到个问题，错误log如下：
      “out/target/product/xxxx/obj/lib/xxxxxx.so: undefined reference to `xxxxxxxx'” 
      “collect2: ld returned 1 exit status”，这个错误时因为在使用xxxxxx.so库时，库中的xxxxxxxx函数需要其他库文件或者没有实现。所以只需要将相应的库文件加到编译的路徑中.
++++++++++++++++++++++++++
**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**
普通情况下解答：
**/cygdrive/f/Class/CV/Exercise/test_opencv/main.cpp:13: undefined reference to `_cvLoadImage'/cygdrive/f/Class/CV/Exercise/test_opencv/main.cpp:19: undefined reference to `cv::Mat::Mat(_IplImage const*, bool)'/cygdrive/f/Class/CV/Exercise/test_opencv/main.cpp:35: undefined reference to `cv::cvtColor(cv::Mat const&, cv::Mat&, int, int)'/cygdrive/f/Class/CV/Exercise/test_opencv/main.cpp:93: undefined reference to `cv::cvtColor(cv::Mat const&, cv::Mat&, int, int)'/cygdrive/f/Class/CV/Exercise/test_opencv/main.cpp:96: undefined reference to `cv::namedWindow(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int)'/cygdrive/f/Class/CV/Exercise/test_opencv/main.cpp:100: undefined reference to `_cvShowImage'/cygdrive/f/Class/CV/Exercise/test_opencv/main.cpp:104: undefined reference to `cv::waitKey(int)'**
这个是一个常见的错误，通常就是由于没有包含一些库文件引起的，下面的步骤将解决这个问题
[http://stackoverflow.com/questions/7816607/opencv-2-3-compiling-issue-undefined-refence-ubuntu-11-10](http://stackoverflow.com/questions/7816607/opencv-2-3-compiling-issue-undefined-refence-ubuntu-11-10)
I am guessing that at least some of the libraries in the output of
```
pkg-config opencv --libs
```
are archive libraries. It is incorrect to put archive libraries before sources that need them (`test_1.cpp` in this case): the order of sources and libraries on the link line[matters](http://webpages.charter.net/ppluzhnikov/linker.html).
Try
`g++ -o test_1 test_1.cpp `pkg-config opencv --cflags --libs` `
cvNamedWindow cvShowImage等也被更改～～～
