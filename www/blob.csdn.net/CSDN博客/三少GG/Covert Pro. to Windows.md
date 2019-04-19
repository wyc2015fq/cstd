# Covert Pro. to Windows - 三少GG - CSDN博客
2013年01月23日 19:04:43[三少GG](https://me.csdn.net/scut1135)阅读数：2250
个人分类：[PKU Research](https://blog.csdn.net/scut1135/article/category/801457)
SimpleOpt
http://code.jellycan.com/simpleopt-doc/html/files.html
FFTW
 3.3.3 Windows DLLs
[http://www.fftw.org/install/windows.html](http://www.fftw.org/install/windows.html)
Eigen
http://eigen.tuxfamily.org/dox/GettingStarted.html
libxml2
You can find all the
 history of libxml(2) and libxslt releases in the [old](http://xmlsoft.org/sources/old/) directory.
 The precompiled Windows binaries made by Igor Zlatovic are available in the [win32](http://xmlsoft.org/sources/win32/) directory.
小记：ICONV库，开源的编码转换工具 
    ICONV库是GNU提供的开源编码转换库，在LINUX下已经默认包含于系统中。对于开发者来说，已经存在/usr/include/iconv.h文件，链接时无需加入库名称。 在WINDOWS下需要下载库的安装程序：LibIconv
 for Windows库的网址为：http://gnuwin32.sourceforge.net/packages/libiconv.htm 
### [解决VC不包含stdint.h头文件问题](http://blog.csdn.net/rodmanwu/article/details/6846788)
stdint.h是C99的标准，主要用于统一跨平台数据定义。
MSVC中不带有这个头文件，直到VS2010（[新闻](http://choorucode.wordpress.com/2010/04/13/visual-studio-2010-stdint-h/)）。
在之前的版本里面，我们可以：
（1）下载这个头文件
> 
download a MS version of this header from:
> 
[http://msinttypes.googlecode.com/svn/trunk/stdint.h](http://msinttypes.googlecode.com/svn/trunk/stdint.h)
A portable one can be found here:
[http://www.azillionmonkeys.com/qed/pstdint.h](http://www.azillionmonkeys.com/qed/pstdint.h)
（2）将头文件放到（以VS2008为例）：
> 
C:\Program Files\Microsoft Visual Studio 9.0\VC\include

M_PI 是一个宏定义，圆周率的定义
- 
          C/C++ code
#define
 M_PI        3.14159265358979323846
此宏定义和编译器有关，TC中M_PI宏就定义在<math.h>里面。
但vc的<math.h>中没有了M_PI的宏定义。因此编译时会出错，提示：“M_PI”: 未声明的标识符。
- 解决方法：自己定义。
## error C2719: '_Val': formal parameter with __declspec(align('16')) won't be aligned
英文提示:error C2719: 'p': formal parameter with __declspec(align('16')) won't be aligned
中文提示:error C2719: “p”: 具有 __declspec(align('16')) 的形参将不被对齐
导致整个现象的主要原因是使用了Eigen库，Eigen为了使用SSE加速，所以内存分配上使用了128位的指针。
更加准确的说法：
“First, "fixed-size" should be clear: an [Eigen](http://eigen.tuxfamily.org/dox/namespaceEigen.html) object has fixed size if its number of rows and its
 number of columns are fixed at compile-time. So for example Matrix3f has fixed size, but MatrixXf doesn't (the opposite of fixed-size is dynamic-size).
The array of coefficients of a fixed-size [Eigen](http://eigen.tuxfamily.org/dox/namespaceEigen.html) object is a plain "static array", it is not dynamically
 allocated. For example, the data behind a Matrix4f is just a "float array[16]".
Fixed-size objects are typically very small, which means that we want to handle them with zero runtime overhead -- both in terms of memory usage and of speed.
Now, vectorization (both SSE and AltiVec) works with 128-bit packets. Moreover, for performance reasons, these packets need to be have 128-bit alignment.
So it turns out that the only way that fixed-size [Eigen](http://eigen.tuxfamily.org/dox/namespaceEigen.html) objects can be vectorized, is if their size
 is a multiple of 128 bits, or 16 bytes. [Eigen](http://eigen.tuxfamily.org/dox/namespaceEigen.html) will then request 16-byte alignment for these objects,
 and henceforth rely on these objects being aligned so no runtime check for alignment is performed.”
解决方案：
分为四种情况：
- [Cause 1: Structures having Eigen objects as members](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c1)
- [Cause 2: STL Containers](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c2)
- [Cause 3: Passing Eigen objects by value](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c3)
- [Cause 4: Compiler making a wrong assumption on stack alignment (for instance GCC on Windows)](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c4)
每种情况可以对照官方的说法。可以参考如下链接
[http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html)
在此不再重复表述。
**'dummy_precision' : is not a member of 'Eigen::NumTraits<T>'**
## [help,vs2008 complier error,but gcc is right?](http://forum.kde.org/viewtopic.php?f=74&t=107892&sid=6c5284cefac29cf424f5e7c772cf98d0)
原因找到了，
You define arrays where each item is an array and somewhere in your code, you try to call methods which
 are defined only on **scalars,** like dummy_precision() (See the output from the compiler)
但是如何解决？？？
貌似是eigen的bug，哎 如何是好~~~
参见对正在编译的类 模板 实例化
这个问题我也遇到了. 经过反复检查, 其实是个小问题,问题的原因在于变量赋值的类型不匹配.顺着这个思路查查吧
**// doesn't work**
[http://www.mrpt.org/Matrices_vectors_arrays_and_Linear_Algebra_MRPT_and_Eigen_classes](http://www.mrpt.org/Matrices_vectors_arrays_and_Linear_Algebra_MRPT_and_Eigen_classes)
**STL containers**
If you have errors like:
error C2719: '_Val': formal parameter with __declspec(align('16')) won't be aligned
around a STL container, then it means that you must use Eigen's special aligned memory allocator for the container. This Eigen's [webpage](http://eigen.tuxfamily.org/dox-devel/StlContainers.html) explains
 this problem in detail.
But, in short these are the required changes:
|`01`|`Let ``"TYPE"````be any````"problematic"``class``or ``struct``.`|
|`02`||
|`03`|```// vector```|
|`04`|```std::vector<TYPE> v;``````// ERROR.```|
|`05`|```std::vector<TYPE, Eigen::aligned_allocator<TYPE> > v;``````// OK, or alternatively```|
|`06`|```mrpt::aligned_containers<TYPE>::vector_t v;``````// this version is a short-cut.```|
|`07`||
|`08`|```// map```|
|`09`|```std::map<KEY,VALUE> m;``````// ERROR.```|
|`10`|```std::map<KEY,VALUE, std::less<KEY>, Eigen::aligned_allocator<std::pair<````const``KEY,VALUE>`|
|`11`|```> > m;``````// OK, or alternatively```|
|`12`|```mrpt::aligned_containers<KEY,VALUE>::map_t  m;``````// this version is a short-cut.```|
Notice that MRPT provides [mrpt::aligned_containers](http://reference.mrpt.org/svn/structmrpt_1_1aligned__containers.html) to ease the declaration of such STL containers.
[http://forum.kde.org/viewtopic.php?f=74&t=102061](http://forum.kde.org/viewtopic.php?f=74&t=102061)
According to [http://msdn.microsoft.com/en-us/library
 ... y1(v=vs.71](http://msdn.microsoft.com/en-us/library/373ak2y1(v=vs.71)).aspx Visual C does not allow the align argument in a function call. However, the type I'm placing
 into the Eigen Matrix has to be aligned. I
I
 prepared a patch there:
[http://eigen.tuxfamily.org/bz/show_bug.cgi?id=482](http://eigen.tuxfamily.org/bz/show_bug.cgi?id=482)
maybe the same changes will be needed for the other modules...

