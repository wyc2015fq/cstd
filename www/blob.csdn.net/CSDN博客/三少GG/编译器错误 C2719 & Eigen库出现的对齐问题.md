# 编译器错误 C2719 & Eigen库出现的对齐问题 - 三少GG - CSDN博客
2013年02月22日 16:57:48[三少GG](https://me.csdn.net/scut1135)阅读数：4300
![](https://img-my.csdn.net/uploads/201302/22/1361523727_3197.jpg)
**注意收集错误信息，通过“输出”窗口，定位到具体代码行！！！**
**解决方案：**
[**http://eigen.tuxfamily.org/dox/TopicStlContainers.html**](http://eigen.tuxfamily.org/dox/TopicStlContainers.html)
![](https://img-my.csdn.net/uploads/201302/22/1361523567_5102.jpg)
eigen 3.1.2有 bug,   
**更新daily build版本： (官网 https://bitbucket.org/eigen/eigen/)**
hg clone [https://bitbucket.org/eigen/eigen/](https://bitbucket.org/eigen/eigen/)
并修改  **RealScalar p => const RealScalar& p**
关于为何更改的网页找不到了。。。 大致意思是这个：[http://eigen.tuxfamily.org/bz/show_bug.cgi?id=83](http://eigen.tuxfamily.org/bz/show_bug.cgi?id=83)
msvc2008才会出现此错误，2010没有。
![](https://img-my.csdn.net/uploads/201302/22/1361547869_3512.jpg)
解释大致为：
[](http://stackoverflow.com/questions/1281415/error-c2719-val-formal-parameter-with-declspecalign16-wont-be-alig)
# [error C2719: '_Val': formal parameter with __declspec(align('16')) won't be aligned?](http://stackoverflow.com/questions/1281415/error-c2719-val-formal-parameter-with-declspecalign16-wont-be-alig)
It is a [known issue](http://ompf.org/forum/viewtopic.php?f=11&t=686&start=0) that stl::vector
 cannot properly contain aligned data, such as D3DXMATRIXA16. One poster [pinned
 the root cause](http://ompf.org/forum/viewtopic.php?f=11&t=686&start=0#p6563) (or at least, one of them?): the declaration of vector::resize passes the aligned data by value, and not as const reference. Several workarounds were suggested in that thread, the safest being dropping
 stl::vector altogether. You might also want to fix the stl headers yourself and recompile - this actually may be easier than it sounds, but I haven't done so myself.
EDIT: links are now broken (thanks @David Menard), here's [an
 alternative, more elaborate answer](http://thetweaker.wordpress.com/2010/05/05/stdvector-of-aligned-elements/).
The issue is fixed in VS2012RC - here's a link to [a
 corresponding connect issue](http://connect.microsoft.com/VisualStudio/feedback/details/692988). Turns out it was actually an issue in the C++ standard itself, fixed in 2008.
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
### (zz)使用Eigen库出现的对齐问题。
2012-03-13 19:34:36| 分类：[reproduct](http://blog.163.com/gz_ricky/blog/#m=0&t=1&c=fks_084065087087089065083082086095085094083067080095087066092)|
 标签：[eigen](http://blog.163.com/gz_ricky/blog/#m=0&t=3&c=eigen)[vs2010](http://blog.163.com/gz_ricky/blog/#m=0&t=3&c=vs2010)|字号大中小订阅
error C2719: '_Val': formal parameter with __declspec(align('16')) won't be aligned
2012-02-09 12:42
英文提示:error C2719: 'p': formal parameter with __declspec(align('16')) won't be aligned
中文提示:error C2719: “p”: 具有 __declspec(align('16')) 的形参将不被对齐
导致整个现象的主要原因是使用了Eigen库，Eigen为了使用SSE加速，所以内存分配上使用了128位的指针。
更加准确的说法：
“First, "fixed-size" should be clear: an [Eigen](http://eigen.tuxfamily.org/dox/namespaceEigen.html) object has fixed size if its number of rows and its number of columns are fixed at compile-time. So for example Matrix3f has fixed size, but MatrixXf doesn't (the opposite of fixed-size is dynamic-size).
The array of coefficients of a fixed-size [Eigen](http://eigen.tuxfamily.org/dox/namespaceEigen.html) object is a plain "static array", it is not dynamically allocated. For example, the data behind a Matrix4f is just a "float array[16]".
Fixed-size objects are typically very small, which means that we want to handle them with zero runtime overhead -- both in terms of memory usage and of speed.
Now, vectorization (both SSE and AltiVec) works with 128-bit packets. Moreover, for performance reasons, these packets need to be have 128-bit alignment.
So it turns out that the only way that fixed-size [Eigen](http://eigen.tuxfamily.org/dox/namespaceEigen.html) objects can be vectorized, is if their size is a multiple of 128 bits, or 16 bytes.[Eigen](http://eigen.tuxfamily.org/dox/namespaceEigen.html) will then request
 16-byte alignment for these objects, and henceforth rely on these objects being aligned so no runtime check for alignment is performed.”
解决方案：
分为四种情况：
[Cause 1: Structures having Eigen objects as members](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c1)
[Cause 2: STL Containers](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c2)
[Cause 3: Passing Eigen objects by value](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c3)
[Cause 4: Compiler making a wrong assumption on stack alignment (for instance GCC on Windows)](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c4)
每种情况可以对照官方的说法。可以参考如下链接
[http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html)
在此不再重复表述。
[http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c3](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c3)
**Table of contents**
- [Where in my own code is the cause of the problem?](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#where)
- [Cause 1: Structures having Eigen objects as members](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c1)
- [Cause 2: STL Containers](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c2)
- [Cause 3: Passing Eigen objects by value](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c3)
- [Cause 4: Compiler making a wrong assumption on stack alignment (for instance GCC on Windows)](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#c4)
- [Explanation](http://eigen.tuxfamily.org/dox/TopicFixedSizeVectorizable.html#explanation)
- [I don't care about vectorization, how do I get rid of that stuff?](http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html#getrid)

# [编译器错误 C2719](http://msdn.microsoft.com/zh-cn/library/373ak2y1(v=vs.90).aspx)
**Visual Studio 2008 **
[其他版本]()
![](http://i3.msdn.microsoft.com/Areas/Epx/Content/Images/ImageSprite.png)
- [Visual Studio 2012](http://msdn.microsoft.com/zh-cn/library/373ak2y1(v=vs.110).aspx)
- [Visual Studio 2010](http://msdn.microsoft.com/zh-cn/library/373ak2y1(v=vs.100).aspx)
- [Visual Studio 2005](http://msdn.microsoft.com/zh-cn/library/373ak2y1(v=vs.80).aspx)
此主题尚未评级-[评价此主题](http://msdn.microsoft.com/zh-cn/library/373ak2y1(v=vs.90).aspx#feedback)
更新：2007 年 11 月
#### 错误消息
“parameter”: 具有 __declspec(align('#')) 的形参将不被对齐
函数参数中不允许使用 [align](http://msdn.microsoft.com/zh-cn/library/83ythb65(v=vs.90).aspx)__declspec 修饰符。
下面的示例生成 C2719：
[复制]()
// C2719.cpp
void func(int __declspec(align(32)) i);   // C2719
// try the following line instead
// void func(int i);

