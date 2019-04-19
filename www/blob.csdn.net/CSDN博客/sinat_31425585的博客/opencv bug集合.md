# opencv bug集合 - sinat_31425585的博客 - CSDN博客
2017年08月15日 15:33:59[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：1486
个人分类：[OpenCV](https://blog.csdn.net/sinat_31425585/article/category/7072509)
1、使用Opencv中matchTemplate函数API，结果出现如下错误：
```
OpenCV Error: Assertion failed(corrsize.height <= img.rows + templ.rows - 1 &&
corrsize.width <= img.cols + templ.cols- 1) in cv::crossCorr, file C:\builds\ma
ster_PackSlave-win64-vc12-shared\opencv\modules\imgproc\src\templmatch.cpp,line
 658
```
经过查找分析，发现是由于模版的名字拼写错了，将模版名字改对就行了~
2、调试的过程中出现安全内存溢出错误，如下：
`0x000007FEE3EC89EE (opencv_world310.dll)ntll.dll (fast_ellipse_detector.exe 中)处有未经处理的异常:`` 0xC0000005:  读取位置 0x0000000003CADFC8 时发生访问冲突。`
经过分析将“属性->链接器->常规->启用增量” 链接设置为否就行了。
3、opencv 使用videocapture 打开摄像头出现问题：
![](https://img-blog.csdn.net/20170925172121184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
解决问题方法，设置vs的权限，右键vs图标，兼容性，选择以管理员身份运行！！！
4、访问越界
![](https://img-blog.csdn.net/20171117105855823?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
解决方法：定位错误位置，设置界限判定条件。
5、.exe触发了一个断点。
参考博客：[点击打开链接](http://blog.csdn.net/gavinv/article/details/51669354)
解决方法：将附加链接项中，opencv_world310d.lib与opencv_world310.lib交换位置~
6、**vs2010 出错：error LNK1123: 转换到 COFF 期间失败: 文件无效或损坏**
**参考博客：[点击打开链接](https://blog.csdn.net/iracer/article/details/51138135)**
解决方法：从安装vs的目录下找到vs->bin，将bin文件夹下的cvtres.exe文件直接删除或重新命名都行。
7、SVM的predict出现错误如下：
```cpp
OpenCV(3.4.1) Error: Assertion failed (samples.cols == var_count && samples.type
() == 5) in cv::ml::SVMImpl::predict, file F:\opencv\sources\modules\ml\src\svm.
cpp, line 2005
```
解决方法，检查一下样本是否为行向量以及样本的数据类型是否为CV_32FC1
8、严重性代码错误 C2039  “type”: 不是“cv::DataType<T>”的成员
解决方法：打开include/opencv2/core/traist.hpp文件，在第115行添加
#define OPENCV_TRAITS_ENABLE_DEPRECATED
