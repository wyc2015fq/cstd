# OpenCV各个模块功能详细简介



标签： [Opencv3新模块](https://so.csdn.net/so/search/s.do?q=Opencv3%E6%96%B0%E6%A8%A1%E5%9D%97&t=blog)[Opencv3-2](https://so.csdn.net/so/search/s.do?q=Opencv3-2&t=blog)[Opencv对比](https://so.csdn.net/so/search/s.do?q=Opencv%E5%AF%B9%E6%AF%94&t=blog)[Opencv模块](https://so.csdn.net/so/search/s.do?q=Opencv%E6%A8%A1%E5%9D%97&t=blog)更多

个人分类： [opencv](https://blog.csdn.net/zmdsjtu/article/category/6371625)[-----读书笔记-----](https://blog.csdn.net/zmdsjtu/article/category/6439646)



版权声明：本文为博主原创文章，转载请告知一声	https://blog.csdn.net/zmdsjtu/article/details/54924727

Contrib模块见 ： [地址](https://blog.csdn.net/zmdsjtu/article/details/80736258)

感谢之前一个博友的留言说工程实际中Opencv各个模块封装成一个world的dll会太过冗余，所以今天来把几个主要的功能模块的主要功能整理下，方便之后的拆分调用。

如果对于某些模块有疑问，欢迎留言交流，之后或可以重点再去补充文章中某些部分的内容。

Opencv官方首页给的Reference说明文档是2.4.13.2版本的…………[官网说明链接](http://docs.opencv.org/2.4/modules/refman.html)

3.2版本的模块说明：[Opencv3.2模块](http://docs.opencv.org/trunk/)

首先打开opencv_modules.hpp文件，可以看到对于各个功能模块的定义如下： 
//功能模块和3.1一样…

```
/*
 *      ** File generated automatically, do not modify **
 *
 * This file defines the list of modules available in current build configuration
 *
 *
*/

#define HAVE_OPENCV_CALIB3D
#define HAVE_OPENCV_CORE
#define HAVE_OPENCV_FEATURES2D
#define HAVE_OPENCV_FLANN
#define HAVE_OPENCV_HIGHGUI
#define HAVE_OPENCV_IMGCODECS
#define HAVE_OPENCV_IMGPROC
#define HAVE_OPENCV_ML
#define HAVE_OPENCV_OBJDETECT
#define HAVE_OPENCV_PHOTO
#define HAVE_OPENCV_SHAPE
#define HAVE_OPENCV_STITCHING
#define HAVE_OPENCV_SUPERRES
#define HAVE_OPENCV_VIDEO
#define HAVE_OPENCV_VIDEOIO
#define HAVE_OPENCV_VIDEOSTAB
#define HAVE_OPENCV_WORLD
1234567891011121314151617181920212223242526
```

接着按照上述功能模块对照官方说明进行介绍。

------

## 3D相关CALIB3D

全称是Camera Calibration and 3D Reconstruction，也就是所谓的摄像机标定与三维重建。 
包括： 
1.基本多视角几何算法 
2.单立体摄像机标定 
3.物体位姿估计 
4.三维重建要素

```
calib3d - basic multiple-view geometry algorithms, single and stereo camera calibration, object pose estimation, stereo correspondence algorithms, and elements of 3D reconstruction.1
```

------

## 最基础部分CORE

定义了Opencv最为基础的数据结构，是一个内容非常详实与紧凑的模块。 
包括了： 
**1.最基础的结构体** 
Basic Structures 
Basic C Structures and Operations 
**2.动态结构**（Opencv1用得多，新版的不需要关注这些，利用std::vector或其他更高级的结构即可） 
Dynamic Structures 
**3.数组操作**，包括了abs,absdiff,add,addWeighted,bitwise_and….等等一些列的数组操作运算 
详情见地址：[官方说明文档](http://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html)//有空或许会把这个写一篇博客…… 
Operations on Arrays 
**4.绘画的功能**，画点，线，圆，椭圆，方框等等…… 
Drawing Functions 
**5.XML/YAML文件进行存储或调用Opencv的各种数据结构**,我之前有篇博客进行表情识别的时候就利用了XML进行特征的提取，还是非常方便的，[XML的使用案例](http://blog.csdn.net/zmdsjtu/article/details/53667929) 
XML/YAML Persistence 
XML/YAML Persistence (C API) 
**6.聚类**：K-Mean聚类以及分割的API 
Clustering 
**7.辅助功能与系统函数和宏** 
Utility and System Functions and Macros 
**8.OpenGL交互相关** 
OpenGL interoperability 
首先想进行交互需要自己编译Opencv把OPENGL的状态设置为ON。2.4版本的只支持基于windows或linux的win32,GTK，Qt的组合……安卓和macOS的不支持… 
更为详细的信息见链接： 
[OpenCV+OpenGL2.4版本](http://docs.opencv.org/2.4/modules/core/doc/opengl_interop.html)

[OpenCV+OpenGL3.2版本](http://docs.opencv.org/trunk/d2/d3c/group__core__opengl.html)

—————————–3.2与2.4相比增添的内容： 
1.Intel IPP Asynchronous C/C++ Converters 
2.Optimization Algorithms 
3.DirectX interoperability 
4.Eigen support 
5.OpenCL support：仅限Linux使用 
6.Intel VA-API/OpenCL (CL-VA) interoperability 
7.Hardware Acceleration Layer

```
core - a compact module defining basic data structures, including the dense multi-dimensional array Mat and basic functions used by all other modules.1
```

------

## 2D特征FEATURES2D

显著的特征描述符，描述符匹配器和探测器，包括以下几块内容：

**1.特征的检测以及描述**：包括Fast算法，MSER，ORB，BRISK，FREAK 
Feature Detection and Description 
**2.特征检测器的接口** 
Common Interfaces of Feature Detectors 
**3.描述符提取器的接口** 
Common Interfaces of Descriptor Extractors 
**4.描述符匹配器的接口** 
Common Interfaces of Descriptor Matchers 
**5.通用描述符匹配器的接口** 
Common Interfaces of Generic Descriptor Matchers 
**6.关键点以及匹配的绘制函数** 
Drawing Function of Keypoints and Matches 
**7.物体分类**：这是基于local 2D features的物体分类，有BOW训练器以及BOW分类器两个模块 
Object Categorization

—————3.2里本质一样，分类看上去更为清爽一点： 
Feature Detection and Description

Descriptor Matchers

Drawing Function of Keypoints and Matches

Object Categorization

------

## 近邻搜索FLANN

FLANN库全称是Fast Library for Approximate Nearest Neighbors，它是目前最完整的（近似）最近邻开源库。不但实现了一系列查找算法，还包含了一种自动选取最快算法的机制。 摘自：[FLANN使用说明](http://www.mamicode.com/info-detail-495502.html) 
主要分为： 
1.快速近似最近邻搜索FLANN 
2.聚类Clustering

------

## 高层级的交互HIGHGUI

High-level GUI and Media I/O 高层级的用户交互以及媒体的IO接口，主要包括以下几个方面： 
**1.用户交互**：包括显示图片，窗口的操作，鼠标的操作等等。 
**2.读写图片或者视频**：顾名思义就是对于图片或者视频的一些列操作。 
**3.Qt新功能**：//这个之后一定要试试，[使用说明地址](http://docs.opencv.org/2.4/modules/highgui/doc/qt_new_functions.html#loadwindowparameters)或者[Opencv3.2的使用说明](http://docs.opencv.org/trunk/dc/d46/group__highgui__qt.html) 
结合Qt可以弄成类似于MFC的有按钮等等功能的界面，大致如下： 
![这里写图片描述](http://docs.opencv.org/2.4/_images/qtgui.png)

—————–3.2与2.4相比在highgui多了OpenGL的支持模块，详细功能函数如下：

```
void    cv::imshow (const String &winname, const ogl::Texture2D &tex)
    Displays OpenGL 2D texture in the specified window. More...

void    cv::setOpenGlContext (const String &winname)
    Sets the specified window as current OpenGL context. More...

void    cv::setOpenGlDrawCallback (const String &winname, OpenGlDrawCallback onOpenGlDraw, void *userdata=0)
    Sets a callback function to be called to draw on top of displayed image. More...

void    cv::updateWindow (const String &winname)
    Force window to redraw its context and call draw callback ( See cv::setOpenGlDrawCallback ).1234567891011
```

------

## 图片的读写IMGCODECS

用于图片的读写。//2.4里没有这块

OpenCV3开始图片、视频编解码从highgui模块分离出来,组成了imgcodecs和videoio。Linux环境下需要注意一下，其他没啥。

------

## 图片的处理IMGPROC

**1.图像滤波Image Filtering**：线性和非线性，Sobel,Smooth等等一些列实用的功能函数

详细介绍如下（如果感兴趣的话）：

> Functions and classes described in this section are used to perform various linear or non-linear filtering operations on 2D images (represented as Mat’s). It means that for each pixel location (x,y) in the source image (normally, rectangular), its neighborhood is considered and used to compute the response. In case of a linear filter, it is a weighted sum of pixel values. In case of morphological operations, it is the minimum or maximum values, and so on. The computed response is stored in the destination image at the same location (x,y). It means that the output image will be of the same size as the input image. Normally, the functions support multi-channel arrays, in which case every channel is processed independently. Therefore, the output image will also have the same number of channels as the input one. 
> Another common feature of the functions and classes described in this section is that, unlike simple arithmetic functions, they need to extrapolate values of some non-existing pixels. For example, if you want to smooth an image using a Gaussian 3×3 filter, then, when processing the left-most pixels in each row, you need pixels to the left of them, that is, outside of the image. You can let these pixels be the same as the left-most image pixels (“replicated border” extrapolation method), or assume that all the non-existing pixels are zeros (“constant border” extrapolation method), and so on. OpenCV enables you to specify the extrapolation method. For details, see cv::BorderTypes

**2.图像的几何变换Geometric Image Transformations**： 
这一部分提供2D图像的很多几何变换操作，例如非常实用的Resize等等。

**3.各种图片形式的转换Miscellaneous Image Transformations**： 
提供threshold，cvtColor等等12个功能函数

**4.直方图Histograms** 
提供便于计算的一些接口

**5.结构分析和形状描述Structural Analysis and Shape Descriptors**： 
计算所有多边形或者栅格化形状的特证矩moment，例如可以提取下图的手的凸陷部分：//看着非常实用的感觉 
![这里写图片描述](http://docs.opencv.org/2.4/_images/defects.png)

**6.动作分析以及物体追踪Motion Analysis and Object Tracking**： 
顾名思义……

**7.特征检测Feature Detection** 
（找边界）canny 
（找角）cornerEigenValsAndVecs，cornerHarris，cornerMinEigenVal，cornerSubPix，(预处理)preCornerDetect 
goodFeaturesToTrack 
（找圆）HoughCircles 
（找线）HoughLines，HoughLinesP

**8.物体检测Object Detection** 
就是模板匹配

——–3.2与2.4相比增添了以下内容： 
**1.Drawing Functions绘画的功能**： 
最实用的应该就是透明背景了吧，通过Scalar增加alpha位，之前所有的画圆，方，点线面的函数都可以直接调用cv::某个函数()在颜色位加上最后一位来实现半透明的效果。

```
Scalar(blue_component,green_component,red_component[,alpha_component]) 1
```

**2.给灰度图重新上色ColorMaps in OpenCV** 
功能函数为：`void cv::applyColorMap (InputArray src, OutputArray dst, int colormap)` 
附上颜色对照表

![这里写图片描述](https://img-blog.csdn.net/20170208145002600?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem1kc2p0dQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**3.平面细分Planar Subdivision** 
cv::Subdiv2D 
用于三维定位等等… 
![平面细分](http://docs.opencv.org/trunk/delaunay_voronoi.png) 
**4.C API** 
**5.Hardware Acceleration Layer**

------

## 机器学习ML

Machine Learning机器学习模块，基本就是统计学上回归，分类以及聚类的，大致分为以下几个部分： 
1.统计模型Statistical Models 
2.一般贝叶斯分类器Normal Bayes Classifier 
3.K-近邻K-Nearest Neighbors 
4.支持向量机SVM Support Vector Machines：之前尝试过的案例：[SVM案例](http://blog.csdn.net/zmdsjtu/article/details/53610244) 
5.决策树Decision Trees 
6.级联分类器Boosting 
7.梯度树Gradient Boosted Trees 
8.随机树Random Trees 
9.超随机树Extremely randomized trees 
10.期望最大化Expectation Maximization 
11.神经网络Neural Networks 
12.机器学习数据的形式相关MLData

如何训练以及使用见地址：[机器学习官方说明](http://docs.opencv.org/trunk/dc/dd6/ml_intro.html) 
————-3.2与2.4相比新增内容 
**1.Logistic Regression**：类似于SVM也是一种多分类器

------

## 物体检测OBJDETECT

物体检测 
Opencv2.4包含Cascade Classification（级联分类）和Latent SVM这两个部分 
Opencv3.2所采用的方法为 
Haar Feature-based Cascade Classifier for Object Detection

------

## 图像降噪克隆与NPR渲染PHOTO

也就是 Computational Photography，Opencv2.4包括以下两个部分： 
1.图像修复Inpainting

```
void inpaint(InputArray src, InputArray inpaintMask, OutputArray dst, double inpaintRadius, int flags)1
```

2.图像降噪Denoising

———–Opencv3.2模块 
**1.降噪Denoising**相比于2.4功能函数更多了 
**2.高范围动态成像HDR imaging**利用HDR算法，有一系列相关的算法函数……2.4里并没有 
**3.无缝克隆Seamless Cloning** 
如图，强大一览无余，图片摘自vsooda的[博客](http://blog.csdn.net/vsooda/article/details/38823745) 
![这里写图片描述](https://img-blog.csdn.net/20140825173841749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdnNvb2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center) 
**4.非真实感渲染Non-Photorealistic Rendering** 
看名字很玄乎……大概动画里用得很多，把一个图片渲染得不像真的，如图所示，左侧为一般渲染，右侧为NPR渲染（摘自[维基百科](https://en.wikipedia.org/wiki/Non-photorealistic_rendering)） 
![这里写图片描述](https://upload.wikimedia.org/wikipedia/commons/thumb/1/1a/Toon_Shader.jpg/220px-Toon_Shader.jpg) 
**5.C API**

------

## 形状的匹配以及距离计算SHAPE

Shape Distance and Matching，形状的距离以及匹配 
//对这部分了解不是很多，只能形而上学地复制过来了 
Opencv3特有的部分，Opencv2.4并没有… 
1. AffineTransformer仿射变换算法的包装类 
2. HistogramCostExtractor 
3. HausdorffDistanceExtractor 
4. ShapeContextDistanceExtractor 
5. ThinPlateSplineShapeTransformer

------

## 图像拼接STITCHING

图像拼接模块，流程如下，可以去掉或者单独使用其中某些模块。 
![这里写图片描述](http://docs.opencv.org/trunk/StitchingPipeline.jpg)

主要包括以下内容： 
**1.拼接管道Stitching Pipeline** 
**2.特征点提取以及图片匹配Features Finding and Images Matching** 
**3.旋转估计Rotation Estimation** 
**4.自动校准Autocalibration** 
**5.图片倾斜旋转相关的包Images Warping** 
**6.缝隙估计Seam Estimation** 
**7.曝光补偿Exposure Compensation** 
**8.图片混合Image Blenders**

------

## 超分辨率SUPERRES

Super Resolution,包含了一系列的超分辨率问题的处理方法 
//对这边毫无了解，只能先形而上学……

------

## 视频分析VIDEO

Video Analysis视频分析，包括以下两块内容： 
1.动作分析Motion Analysis 
2.物体追踪Object Tracking

------

## 视频读写VIDEOIO

OpenCV3特有的仅用来读写视频的模块

------

## 视频稳定系VIDEOSTAB

Video Stabilization、视频稳定模块包含一组可以用来解决视频稳定问题的函数和类。2.4基本没提及，3.2里内容比较详实。包括以下两个内容： 
**1.全局运动估计Global Motion Estimation** 
包括一些列功能和类用以进行两张图或者两个点云阵的全局动作估计。功能函数都被封装成了类。 
**2.快速步进法Fast Marching Method**

------

## 打包WORLD

终于到最后一个了，这是Opencv3特有的，把所有东西打包在一个dll里，配置环境的时候很方便，具体工程实际就要分情况了。

最后放上Opencv3.2以及Opencv2.4模块的对比图方便调用： 
![这里写图片描述](https://img-blog.csdn.net/20170208170706843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem1kc2p0dQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
其中Opencv2.4特有的几个模块功能说明如下： 
//参考了易术军的[博客](http://www.cnblogs.com/yishujun/archive/2014/03/27/3629004.html) 
**OPENCV_CONTRIB**：也就是Contributed/Experimental Stuf的缩写， 该模块包含了一些最近添加的不太稳定的可选功能，不用去多管。2.4.8里的这个模块有新型人脸识别，立体匹配，人工视网膜模型等技术。 
**OPENCV_GPU**：运用GPU加速的计算机视觉模块 
**OPENCV_LEGACY**：一些已经废弃的代码库，保留下来作为向下兼容，3里以及全部删除。 
**OPENCV_NONFREE**: 也就是一些具有专利的算法模块 ，包含特征检测和GPU相关的内容，在3里删去了。 
**OPENCV_OCL** ：即OpenCL-accelerated Computer Vision，运用OpenCL加速的计算机视觉组件模块，在3里整个并入了Core. 
**OPENCV_TS**：测试用。

从早上到这会儿（在女朋友实验室）终于把这些整理完了，最后祝大家使用Opencv愉快~