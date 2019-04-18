# GDI+编程小结 - L_Andy的专栏 - CSDN博客

2013年05月24日 09:24:08[卡哥](https://me.csdn.net/L_Andy)阅读数：4136





GDI+（Graphics Device Interface Plus图形设备接口加）是Windows XP和Windows Server 2003操作系统的子系统，也是.NET框架的重要组成部分，负责在屏幕和打印机上绘制图形图像和显示信息。

GDI+不但在功能上比GDI 要强大很多，而且在代码编写方面也更简单，因此会很快成为Windows图形图像程序开发的首选。

一、              GDI+的特点和新增功能

GDI+与GDI一样，都具有设备无关性。应用程序的程序员可利用GDI+这样的图形设备接口在屏幕或打印机上显示信息，而不需要考虑特定显示设备的具体情况。应用程序的程序员调用GDI+类提供的方法，而这些方法又反过来相应地调用特定的设备驱动程序。GDI+将应用程序与图形硬件隔离，而正是这种隔离允许开发人员创建设备无关的应用程序。



![](http://hi.csdn.net/attachment/201010/28/0_12882705336Tab.gif)

1、  GDI+的功能

GDI+主要提供了以下三种功能：

1）  二维矢量图形

矢量图形包括坐标系统中的系列点指定的绘图基元（如直线、曲线和图形）。例如，直线可通过它的两个端点来指定，而矩形可通过确定其左上角位置的点并给出其宽度和高度的一对数字来指定。简单路径可由通过直线连接的点的数组来指定。贝塞尔样条是由四个控制点指定的复杂曲线。

GDI+提供了存储基元自身相关信息的类（结构）、存储基元绘制方式相关信息的类，以及实际进行绘制的类。例如，Rectangle结构存储矩形的位置和尺寸；Pen类存储有关线条颜色、线条粗细和线型的信息；而Graphics类具有用于绘制直线、矩形、路径和其它图形的方法（类似于GDI中的CDC类）。还有几种Brush类，它们存储有关如何使用颜色或图案来填充封闭图形和路径的信息。

用户可以在图元文件中记录矢量图像（图形命令的序列）。GDI+提供了Metafile类，可用于记录、显示和保存图元文件。MetafileHeader和MetaHeader类允许您检查图元文件头中存储的数据。

2）  图像处理

某些种类的图片很难或者根本无法用矢量图形技术来显示。例如，工具栏按钮上的图片和显示为图标的图片就难以指定为直线和曲线的集合。拥挤的棒球运动场的高分辨率数字照片会更难以使用矢量技术来制作。这种类型的图像可存储为位图，即代表屏幕上单个点颜色的数字数组。

GDI+提供了Image、Bitmap和Metafile类，可用于显示、操作和保存位图。它们支持众多的图像文件格式，还可以进行多种图像处理的操作。

3）  文字显示版式

就是使用各种字体、字号和样式来显示文本。GDI +为这种复杂任务提供了大量的支持。GDI+中的新功能之一是子像素消除锯齿，它可以使文本在LCD 屏幕上呈现时显得比较平滑。

2、  GDI+新增特性

1）、渐变画刷

渐变画刷（**gradient brush梯度刷**）通过提供用于填充图形、路径和区域的线性渐变画笔和路径渐变画笔，GDI+扩展了GDI 的功能。渐变画笔还可用于绘制直线、曲线和路径。线性渐变画笔可用于使用颜色来填充图形，画笔在图形中移动时，颜色会逐渐改变。例如，假定通过指定图形左边为蓝色、右边为绿色，创建了一个水平渐变画笔。当用水平渐变画笔填充该图形时，随着画笔从图形的左边移至右边，颜色就会由蓝色逐渐变为绿色。用类似方法定义的垂直渐变画笔填充的图形，颜色从上到下变化。图显示了用水平渐变画笔填充的椭圆和用斜式渐变画笔填充的区域。

![](http://hi.csdn.net/attachment/201010/28/0_1288270645m3Va.gif)

图  水平和斜式渐变画笔

用路径渐变画笔填充图形时，可选择不同的方法来指定当从图形的一部分至另一部分移动画笔时颜色的变化方式。一种选择是指定中心颜色和边缘颜色，在从图形中间向外边缘移动画笔时，像素逐渐从一种颜色变化到另一种颜色。图显示了用路径渐变画笔填充的路径（该路径是用一对贝塞尔样条创建的）。

![](http://hi.csdn.net/attachment/201010/28/0_1288270654hi66.gif)

图  路径渐变画笔

2）、基数样条函数

GDI+支持在GDI 中不支持的基数样条（cardinal spines）。基数样条是一连串单独的曲线，这些曲线连接起来形成一条较长的光滑曲线。样条由点的数组指定，并通过该数组中的每一个点。基数样条平滑地（没有锐角）通过数组中的每一个点，因此，比通过连接直线创建的路径更光滑精准。图显示了两个路径：一个以基数样条的形式创建；另一个通过连接直线创建。

![](http://hi.csdn.net/attachment/201010/28/0_1288270662VGr8.gif)

图  基数样条路径和折线路径

3）、持久路径对象

在GDI 中，路径属于设备上下文，并且会在绘制时被毁坏。利用GDI +，绘图由Graphics对象执行，可以创建并维护几个与Graphics对象分开的持久的路径对象（persistent path object）——**GraphicsPath对象**。绘图操作不会破坏**GraphicsPath 对象**，因此可以多次使用同一个GraphicsPath 对象来绘制路径。

4）、变换和矩阵对象

GDI+提供了**Matrix（矩阵） 对象**，它是一种可以使（缩放、旋转和平移等）变换（transformation）简易灵活的强大工具。**矩阵对象**一般与**变换对象**联合使用。例如，GraphicsPath 对象具有Transform 方法，此方法接收Matrix 对象作为参数。单一的3×3矩阵可存储一种变换或一个变换序列。图显示了一个路径在执行两种变换前后的情况。

![](http://hi.csdn.net/attachment/201010/28/0_12882706679U3y.gif)

图  路径的变换

5）、可伸缩区域

GDI+ 通过对**可伸缩区域（Scalable Regions）**的支持极大地扩展了GDI。在GDI 中，区域被存储在设备坐标中，而且，可应用于区域的惟一变换是平移。而GDI+在全局坐标中存储区域，并且允许区域发生任何可存储在变换矩阵中的变换（如缩放和旋转）。图显示一个区域在执行三种变换（缩放、旋转和平移）前后的情况。

![](http://hi.csdn.net/attachment/201010/28/0_12882706721SzD.gif)

图  区域的三种变换（**缩放、旋转和平移**）

6）、α混色

在下图中，可以在变换区域（用蓝色阴影画笔填充）中看到未变换区域（用红色填充）。这是由GDI+支持的α混色（Alpha Blending，透明混合）实现的。使用α混色，可以指定填充颜色的透明度。透明色与背景色相混合———填充色越透明，透出的背景色就越多。图显示四个用相同颜色（红色）填充、但透明层次不同的椭圆。

![](http://hi.csdn.net/attachment/201010/28/0_1288270683vF2V.gif)

图  不同透明度

7）、丰富的图像格式支持

GDI+提供Image、Bitmap 和Metafile 类，可以用不同的格式加载、保存和操作图像。GDI+支持**BMP、GIF、JPEG、EXIF、PNG、TIFF、ICON、WMF、EMF**共9种常见的图像格式。

8）、GDI+的不足

虽然，相对于GDI来说，GDI+ 确实增加了许多新特性，而且功能更强大，使用也更方便。但是，这并不等于GDI+ 就能够完全代替GDI。

因为GDI+实际上是GDI的封装和扩展，GDI+的执行效率一般要低于GDI的。另外，GDI+不支持图的位运算，那么就不能进行异或绘图等操作。而且在VC中，GDI+ 还不支持双缓存机制（如内存DC和显示DC），这将大大影响GDI+ 在高速图形、图像、动画和视频等方面的应用。

3、  GDI+的使用

1）  GDI+开发包

若采用的是Visual C++ 2008，则已经包含了开发GDI+应用程序所需的所有东西。如果使用的是Visual C++6.0而非VS.Net，我们需要下载微软的GDIPLUS支持包。在微软官方网站下载时需认证Windows为正版，我们可从这个地址下载：http://www.codeguru.com/code/legacy/gdi/GDIPlus.zip。一个完整的GDI+支持包至少包括如下文件：

　　(1)头文件：gdiplus.h

　　(2)动态链接库的.lib文件：gdiplus.lib

　　(3)动态链接库的.dll文件：gdiplus.dll

　　少了(1)、(2)程序不能编译，少了(3)程序能以共享DLL的方式编译但是不能运行，运行时找不到.dll文件。

如果你使用的操作系统是Windows XP或Windows Server 2003，则GDI+所对应的动态链接库，已经被包含在其中。gdiplus.dll一般位于操作系统的WinSxS（Windows side-by-side assembly，视窗并行程序集）目录中，例如：

C:/WINDOWS/WinSxS/x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.0.0_x-ww_8d353f13/gdiplus.dll（1661KB，2002.10.8）

而GDI的动态链接库gdi32.dll，却一般在操作系统的32位系统目录中：

F:/WINDOWS/system32/gdi32.dll（272KB，2004.8.4）

2）  VC使用GDI+初始化准备工作。

#define UNICODE

#ifndef ULONG_PTR

#define ULONG_PTR unsigned long*

#endif

#include "c:/gdiplus/includes/gdiplus.h"

using namespace Gdiplus;

#pragma comment(lib, "c://gdiplus//lib//gdiplus.lib")

//在CWinApp派生类的InitInstance函数中加入：

　//初始化gdiplus的环境

　GdiplusStartupInput gdiplusStartupInput;

　ULONG_PTR gdiplusToken;

　// 初始化GDI+.

　GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

//在CWinApp派生类的ExitInstance函数中加入：

　//关闭gdiplus的环境

　GdiplusShutdown(gdiplusToken);

4、GDI+的组成

GDI+ API包含54个类、12个函数、6类（226个）图像常量、55种枚举和19种结构。

1）、类

GDI+ API中共有54个类，核心类是Graphics，它是实际绘制直线、曲线、图形、图像和文本的类。许多其它GDI+类是与Graphics类一起使用的。例如，DrawLine方法接收Pen对象，该对象中存有所要绘制的线条的属性（颜色、宽度、虚线线型等）。FillRectangle方法可以接收指向LinearGradientBrush对象的指针，该对象与Graphics对象配合工作来用一种渐变色填充矩形。Font和StringFormat对象影响Graphics对象绘制文本的方式。Matrix对象存储并操作Graphics对象的仿射变换——旋转、缩放和翻转图像。

GDI+还提供了用于组织图形数据的几种结构类（例如 Rect、Point和Size）。而且，某些类的主要作用是结构化数据类型。例如，BitmapData类是Bitmap类的帮助器，PathData类是GraphicsPath类的帮助器。

下面是所有GDI+的API类的列表：

**GDI+的API类**（54个）

|**名称**|**类**|**功能**|
|----|----|----|
|调整箭头帽|AdjustableArrowCap|创建自定义箭头线帽|
|位图|Bitmap|提供装入和保存矢量和光栅图像的方法，并可以创建和操作光栅图像|
|位图数据|BitmapData|保存位图的属性|
|模糊|Blur|将高斯模糊效果作用到图像|
|亮度对比度|BrightnessContrast|改变图像的亮度和对比度|
|刷|Brush|定义刷对象|
|缓存图像|CachedBitmap|用为特点设备显示而优化过的格式存储位图|
|字符范围|CharacterRange|指定串内字符位置的范围|
|颜色|Color|保存表示颜色的32位值|
|色平衡|ColorBalance|改变位图的颜色平衡|
|颜色曲线|ColorCurve|可调整位图的曝光度、密度、对比度、加亮、阴影、色调、白饱和和黑饱和。|
|颜色查找表|ColorLUT|用于定制位图的颜色调整|
|颜色矩阵效果|ColorMatrixEffect|对位图进行仿射变换|
|定制线帽|CustomLineCap|封装了自定义线帽|
|效果|Effect|作用于图像的效果和调整类的基类|
|编码器参数|EncoderParameter|保存图像编码器的参数|
|编码器参数组|EncoderParameters|图像编码器参数的数组|
|字体|Font|封装了字体的族系、高度、大小和风格等特性|
|字体集|FontCollection|包含枚举字体集中的字体族系的方法|
|字体族|FontFamily|封装了构成一个字体族的字体集合|
|GDI+基类|GdiplusBase|提供对GDI+对象的存储分配与释放，是其它GDI+类的基类|
|图形|Graphics|提供绘制图形、图像和文本的方法，存储显示设备和被画项目的属性|
|图形路径|GraphicsPath|保存一个供绘图用的直线、曲线和形状序列|
|图形路径迭代器|GraphicsPathIterator|提供从保存在GraphicsPath对象中的路径里选择孤立子集的方法|
|影线刷|HatchBrush|定义具有影线风格和前景色/背景色的矩形刷|
|色调饱和度亮度|HueSaturationLightness|改变位图的色调H、饱和度S和亮度L|
|图像|Image|提供装入和保存矢量和光栅图像的方法|
|图像属性|ImageAttributes|含渲染时如何操作位图和图元文件颜色的信息|
|图像编解码信息|ImageCodecInfo|存储与图像编解码有关的信息|
|图像项数据|ImageItemData|用于存储和获取自定义图像的元数据|
|已装入字体集|InstalledFontCollection|定义表示已装入系统中的字体集|
|级别|Levels|可调整位图的加亮、阴影和色调|
|线形梯度刷|LinearGradientBrush|定义线性渐变刷|
|矩阵|Matrix|表示3×3的仿射变换矩阵|
|图元文件|Metafile|定义包含描述一系列图形API调用记录的图形元文件，可被记录（构造）和回放（显示）|
|图元文件头|MetafileHeader|保存关联图元文件的性质|
|路径数据|PathData|GraphicsPath和GraphicsPathIterator类的助手类，用于获取和设置路径中的数据点及其类型|
|路径梯度刷|PathGradientBrush|保存颜色的梯度属性，用于渐变色填充路径内部|
|笔|Pen|用于绘制直线和曲线的笔对象|
|点|Point|封装2D整数坐标系统中的点|
|浮点点|PointF|封装2D浮点坐标系统中的点|
|专用字体集|PrivateFontCollection|保存用于特定应用程序的字体集，可含未装入系统中的字体|
|特性项|PropertyItem|Image和Bitmap类的助手类，保存一块（piece）图像元数据|
|矩形|Rect|保存矩形的左上角、宽度和高度之对象（整数）|
|浮点矩形|RectF|保存矩形的左上角、宽度和高度之对象（浮点数）|
|红眼校正|RedEyeCorrection|校正有时在闪光照片中出现的红眼|
|区域|Region|描述显示表面的范围，可以是任意形状|
|锐化|Sharpen|调整位图的清晰度|
|大小|Size|封装2D整数坐标系统中的宽和高|
|浮点大小|SizeF|封装2D浮点数坐标系统中的宽和高|
|实心刷|SolidBrush|定义实心颜色的刷子对象|
|串格式|StringFormat|封装文本的格式（layout）信息和显示操作|
|纹理刷|TextureBrush|用于填充的包含图像对象的刷子|
|浓淡|Tint|改变位图的色彩浓淡|

下面是**GDI+ API类的层次结构图**：

GdiplusBase

Graphics

GraphicsPath

GraphicsPathIterator

Pen

Brush

SolidBrush

HatchBrush

TextureBrush

LinearGradientBrush

PathGradientBrush

Image

Bitmap

Metafile

CustomLineCap

AdjustableArrowCap

CachedBitmap

ImageAttributes

FontCollection

InstalledFontCollection

PrivateFontCollection

StringFormat

Region

Font

FontFamily

Matrix

Point

PointF

Size

SizeF

Rect

RectF

Color

Effect

Blur

BrightnessContrast

ColorBalance

ColorCurve

ColorLUT

ColorMatrixEffect

HueSaturationLightness

Levels

RedEyeCorrection

Sharpen

Tint

ImageItemData

BitmapData

MetafileHeader

PropertyItem

EncoderParameter

EncoderParameters

ImageCodecInfo

PathData

CharacterRange

**独立类**

**绘图类**

**效果类**

![](http://hi.csdn.net/attachment/201010/28/0_128827069538NU.gif)

GDI+类的层次结构图

2）、函数

**GDI+命名空间中的函数**（12个）

|**名称**|**函数**|**功能**|
|----|----|----|
|关闭GDI+|GdiplusShutdown|清除GDI+所使用的资源|
|启动GDI+|GdiplusStartup|初始化GDI+|
|获取图像解码器|GetImageDecoders|获取含有可用图像解码器信息的ImageCodecInfo对象数组|
|获取图像解码器的大小|GetImageDecodersSize|获取含有可用图像解码器的数目|
|获取图像编码器|GetImageEncoders|获取含有可用图像编码器信息的ImageCodecInfo对象数组|
|获取图像编码器的大小|GetImageEncodersSize|获取含有可用图像编码器的数目|
|获取像素格式大小|GetPixelFormatSize|返回指定像素格式的每像素二进制位数|
|是否为α像素格式|IsAlphaPixelFormat|确定指定像素格式是否有α分量|
|是否为规范像素格式|IsCanonicalPixelFormat|确定指定像素格式是否为规范格式之一|
|是否为扩展像素格式|IsExtendedPixelFormat|确定指定像素格式是否使用16位色|
|是否为索引像素格式|IsIndexedPixelFormat|确定指定像素格式是否是索引格式|
|对象类型是否有效|ObjectTypeIsValid|确定ObjectType枚举元素是否表示一个有效对象类型|

3）、常量

GDI+中定义了如下**6类图像常量（226个）**：（GdiplusImaging.h）

|类型|常量|说明|
|----|----|----|
|图像文件格式ImageFormat*(11个)|ImageFormatBMP|BMP（BitMaP位图）|
|ImageFormatEMF|EMF（Enhanced MetaFile增强图元文件）| |
|ImageFormatEXIF|Exif（Exchangeable Image File可交换图像文件）| |
|ImageFormatGIF|GIF（Graphics Interchange Format图形交换格式）| |
|ImageFormatIcon|Icon（图标）| |
|ImageFormatJPEG|JPEG（Joint Photographic Experts Group联合图象专家组）| |
|ImageFormatMemoryBMP|从内存位图构造的图像| |
|ImageFormatPNG|PNG（Portable Network Graphics可移植网络图形）| |
|ImageFormatTIFF|TIFF（Tagged Image File Format标签图像文件格式）| |
|ImageFormatUndefined|不能确定格式| |
|ImageFormatWMF|WMF（Windows Metafile Format视窗图元文件格式）| |
|图像帧维|FrameDimensionPage|多帧TIFF图像|
|FrameDimensionTime|多帧GIF图像| |
|图像编码器(13个)|EncoderChrominanceTable|色度表|
|EncoderColorDepth|颜色深度| |
|EncoderColorSpace|颜色空间| |
|EncoderCompression|压缩| |
|EncoderLuminanceTable|亮度表| |
|EncoderQuality|质量| |
|EncoderRenderMethod|渲染方法| |
|EncoderSaveFlag|保存标志| |
|EncoderScanMethod|扫描方法| |
|EncoderTransformation|变换| |
|EncoderVersion|版本| |
|EncoderImageItems|图像项| |
|EncoderSaveAsCMYK|保存为CMYK（Cyan青、Magenta品红、Yellow黄、blacK黑，用于印刷的四分色）| |
|图像像素格式(14个)|PixelFormat1bppIndexed|每像素1位，索引色|
|PixelFormat4bppIndexed|每像素4位，索引色| |
|PixelFormat8bppIndexed|每像素8位，索引色| |
|PixelFormat16bppARGB1555|每像素16位，α分量1位、RGB分量各5位| |
|PixelFormat16bppGrayScale|每像素16位，灰度| |
|PixelFormat16bppRGB555|每像素16位，RGB分量各5位，另1位未用| |
|PixelFormat16bppRGB565|每像素16位，RB分量各5位、G分量6位| |
|PixelFormat24bppRGB|每像素24位，RGB分量各8位| |
|PixelFormat32bppARGB|每像素32位，αRGB分量各8位| |
|PixelFormat32bppPARGB|每像素32位，αRGB分量各8位，RGB分量预乘α分量| |
|PixelFormat32bppRGB|每像素24位，RGB分量各8位，另8位未用| |
|PixelFormat48bppRGB|每像素48位，RGB分量各16位| |
|PixelFormat64bppARGB|每像素64位，αRGB分量各16位| |
|PixelFormat64bppPARGB|每像素64位，αRGB分量各16位，RGB分量预乘α分量| |
|图像特性标志类型(9个)|PixelFormat4bppIndexed|格式为每像素4位，索引色|
|PropertyTagTypeASCII|值数据成员为以null结尾的ASCII字符串| |
|PropertyTagTypeByte|值数据成员为字节数组| |
|PropertyTagTypeLong|值数据成员为32位无符号长整数的数组| |
|PropertyTagTypeRational|值数据成员为32位无符号长整数对的数组，每对数中的第一个整数为分子，第二个整数为分母| |
|PropertyTagTypeShort|值数据成员为16位无符号短整数的数组| |
|PropertyTagTypeSLONG|值数据成员为32位有符号长整数的数组| |
|PropertyTagTypeSRational|值数据成员为32位有符号长整数对的数组，每对数中的第一个整数为分子，第二个整数为分母| |
|PropertyTagTypeUndefined|值数据成员为字节数组，可保存任何数据类型的值| |
|图像特性标志(217个)|PropertyTagGpsVer ~|GPS（Global Positioning Systems全球定位系统）版本|
|PropertyTagGpsDestDist|（0x0000）~ 到目标点的距离（0x001A）（27个）| |
|PropertyTagNewSubfileType ~|子文件数据类型（0x00FE）~| |
|PropertyTagPageNumber|被扫描图像的页数（0x0129）（44个）| |
|PropertyTagTransferFunction|图像传送函数表（0x012D）| |
|PropertyTagSoftwareUsed|指定用于生成图像的设备之软件或固件的名称和版本的以null结尾的字符串（0x0131）| |
|PropertyTagDateTime|图像创建的日期和时间（0x0132）| |
|PropertyTagArtist ~|指定图像创建者姓名的以null结尾的字符串（0x013B）| |
|PropertyTagTileByteCounts|~ 标题的字节数（0x0145）（11个）| |
|PropertyTagInkSet ~|在分开图像中使用的墨水集（0x014C）| |
|PropertyTagNumberOfInks|~ 墨水数目（0x014D）（3个）| |
|PropertyTagDotRange ~|对应于0%点和100%点的颜色分量值（0x0150）~| |
|PropertyTagTransferRange|扩充传送函数范围的值表（0x0156）（7个）| |
|PropertyTagJPEGProc ~|JPEG压缩过程（0x0200）~| |
|PropertyTagImageTitle|图像标题的以null结尾的字符串（0x0320）（17个）| |
|PropertyTagResolutionXUnit ~|显示水平分辨率的单位（0x5001）~（27个）| |
|PropertyTagThumbnailData|RGB或JPEG中的原始缩略图中的位数据（0x501B）| |
|PropertyTagThumbnailImageWidth ~|略图像的每行像素数（0x5020）~（28个）| |
|PropertyTagThumbnailCopyRight|含缩略图像版权信息的以null结尾的字符串（0x503B）| |
|PropertyTagLuminanceTable|亮度表（0x5090）| |
|PropertyTagFrameDelay ~|GIF动画中两帧之间的延时，单位为10毫秒（0x5100）| |
|PropertyTagPaletteHistogram|~ 调色板直方图（0x5113）（9个）| |
|PropertyTagCopyright ~|含版权信息的以null结尾的字符串（0x8298B）~| |
|PropertyTagExifCfaPattern|颜色滤波器数组（0xA302）（48个）| |

4）、枚举

GDI+定义了55种枚举，它们都是相关常数的集合。例如，LineJoin枚举包含元素Bevel、Miter和Round，它们指定可用于连接两个线条的线型。下面是所有枚举类型的列表：

**GDI+枚举类型**（55种）

|**枚举类型**|**名称**||**枚举类型**|**名称**|
|----|----|----|----|----|
|BrushType|刷类型|ImageType|图像类型| |
|ColorAdjustType|颜色调整类型|InterpolationMode|插值类型| |
|ColorChannelFlags|颜色通道标志|ItemDataPosition|项数据位置| |
|ColorMatrixFlags|颜色矩阵标志|LinearGradientMode|线性梯度模式| |
|CombineMode|组合模式|LineCap|线帽| |
|CompositingMode|合成模式|LineJoin|线连接| |
|CompositingQuality|合成质量|MatrixOrder|矩阵序(左右乘)| |
|CoordinateSpace|坐标空间|MetafileFrameUnit|图元文件帧单位| |
|CurveAdjustments|曲线调整|MetafileType|图元文件类型| |
|CurveChannel|曲线通道|ObjectType|对象类型| |
|DashCap|虚线帽|PaletteFlags|调色板标志| |
|DashStyle|虚线风格|PaletteType|调色板类型| |
|DitherType|抖动类型|PathPointType|路径点类型| |
|DriverStringOptions|驱动器串选项|PenAlignment|笔对齐| |
|EmfPlusRecordType|EMF+等图元文件记录类型|PenType|笔类型| |
|EmfToWmfBitsFlags|EMF转WMF的标志位|PixelOffsetMode|像素偏移模式| |
|EmfType|EMF类型|RotateFlipType|旋转翻转类型| |
|EncoderParameterValueType|编码器参数值类型|SmoothingMode|平滑模式| |
|EncoderValue|编码器值|Status|状态| |
|FillMode|填充模式|StringAlignment|串对齐| |
|FlushIntention|刷新意图|StringDigitSubstitute|串数字替换| |
|FontStyle|字体风格|StringFormatFlags|串格式标志| |
|HatchStyle|影线风格|StringTrimming|串修整| |
|HistogramFormat|直方图格式|TextRenderingHint|文本渲染提示| |
|HotkeyPrefix|热键前缀|Unit|单位| |
|ImageCodecFlags|图像编解码标志|WarpMode|弯曲模式| |
|ImageFlags|图像标志|WrapMode|覆盖模式| |
|ImageLockMode|图像加锁模式|| | |

5）、结构

GDI+ API中还定义了19种结构，用于GDI+的各种函数调用中。下面是所有GDI+ API结构的列表：

GDI+ API中的结构（19种）

|**结构**|**名称**|
|----|----|
|BlurParams|模糊参数|
|BrightnessContrastParams|亮度对比度参数|
|ColorBalanceParams|颜色平衡参数|
|ColorCurveParams|颜色曲线参数|
|ColorLUTParams|颜色查找表参数|
|ColorMap|颜色映射|
|ColorMatrix|颜色矩阵|
|ColorPalette|颜色调色板|
|ENHMETAHEADER3|增强图元文件头|
|GdiplusAbort|GDI+异常中断|
|GdiplusStartupInput|GDI+启动输入|
|GdiplusStartupOutput|GDI+启动输出|
|HueSaturationLightnessParams|色调饱和度亮度参数|
|LevelsParams|级别参数|
|PWMFRect16|可定位WMF矩形（INT16整数值）|
|RedEyeCorrectionParams|红眼校正参数|
|SharpenParams|锐化参数|
|TintParams|浓淡参数|
|WmfPlaceableFileHeader|可定位WMF文件头|

6）、GDI+平面API

GDI+暴露出（exposes）一个平面（flat）API，它包含大约600个函数，被实现在Gdiplus.dll中，声明在Gdiplusflat.h内。这些函数被包装到了前面讨论过的GDI+ API的54个C++类的集合之中。不要直接调用这些函数，而推荐用调用类成员方法来替代。因为微软产品支持服务（Microsoft Product Support Services），不会为直接调用平面API的代码提供支持。

作为C++封装的替代方案，微软网络框架（Microsoft .NET Framework），提供了GDI+的一个托管代码封装类集，包含大约60个类、50个枚举和8个结构。它们属于下列命名空间：**// 在C#中使用之**

System.Drawing 

System.Drawing.Drawing2D 

System.Drawing.Imaging 

System.Drawing.Text 

System.Drawing.Printing 

DllExports

System.Drawing[.dll]

Gdiplus

Gdiplus.h

afxwin.h

C++封装

(MFC) 

C++封装

托管代码封装

设备驱动程序

计算机硬件（显示器、打印机等图形设备）

GDI API

GDI+平面API

C++

C#、VB、J#

GDI+ API

GDI+托管类接口

GDI类与结构

Gdi32.dll

WinGDI.h

Gdiplus.dll

GdiplusFlat.h

![](http://hi.csdn.net/attachment/201010/28/0_128827070606vQ.gif)

GDI+的封装与使用

这两种包装（C++和托管代码）都采用了面向对象方法，所以二者在将参数传递给封装的方法和将参数传递给平面API函数的方式上存在差别。

二、GDI+编程

本部分简单介绍GDI+编程中的一些概念与技巧，具体的编程细节请参考《精通GDI+编程》、陈宝楷《GDI+编程》等书籍。

1、Point、浮点数点类PointF；Size、浮点数大小类SizeF；Rect、浮点数矩形类RectF等。

浮点数版的几何对象和绘图函数，是GDI+新增的功能，这些在各种工程技术领域都非常有用。因为一般的实际图形设计，都是基于实数坐标的。包括机械（机床/汽车/轮船/飞机等）、建筑（房屋/桥梁/道路/堤坝等）和图形动画设计（形状/物体/人物/背景/轨迹等）等设计，都必须使用浮点参数和坐标系。

2、Color：在GDI+中，色彩是通过Color(色彩)类来描述的。Color类的构造函数分别为：

Color();

Color(BYTE a,BYTE r,BYTE g,BYTE b);

Color(ARGB argb);

Color(BYTE r,BYTE g,BYTE b);

参数：

a：色彩的透明度(0~255)

r、g、b：红、绿、蓝3种色彩分量值(0~255)

不同于GDI，GDI+在对色彩支持方面主要体现在对色彩的透明度支持。从本质上讲，透明度是像素之间的一种合成运算。它的计算公式是：

输出色彩=前景色*Alpha/255 + 背景色*(255-Alpha)/255

3、Graphics（图形）

图形类Graphics是GDI+的核心，它提供绘制图形、图像和文本的各种方法（操作/函数）（似GDI中的CDC类），还可以存储显示设备和被画项目的属性（到图元文件）。Graphics类及其成员函数都被定义在头文件Gdiplusgraphics.h中。

Graphics类的构造函数有如下4种：

Graphics(Image* image); // 用于绘制图像

Graphics(HDC hdc); // 用于在当前窗口中绘图

Graphics(HDC hdc, HANDLE hdevice); // 用于在指定设备上绘制图形

Graphics(HWND hwnd, BOOL icm = FALSE); // 用于在指定窗口中绘图可以进行颜色调整

其中，最常用的是第二种——在当前视图窗口中绘图的图形类构造函数。

注意，该构造函数的输入参数，是**设备上下文的句柄**，而不是CDC类对象的指针。一般可以由CDC对象得到（**CDC类含有公用数据成员HDC m_hDC;**）

             6种绘制直线和折线的函数：（前三个为整数版，后三个为对应的浮点数版） **// 画折线DrawLines**

Status DrawLine(const Pen* pen, INT x1, INT y1, INT x2, INT y2);

Status DrawLine(const Pen* pen, const Point& pt1, const Point& pt2);

Status **DrawLines**(const **Pen* **pen, **const Point* points**,**INT count**);// 画折线

Status DrawLine(const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2);

Status DrawLine(const Pen* pen, const PointF& pt1, const PointF& pt2);

Status DrawLines(const Pen* pen, const PointF* points, INT count);

6种绘制矩形和矩形组的函数：（也是前三个为整数版，后三个为对应的浮点数版） // Rectangle = rect angle

Status DrawRectangle(const Pen* pen, const Rect& rect);

Status DrawRectangle(const Pen* pen, INT x, INT y, INTwidth, INTheight);

Status DrawRectangles(const Pen* pen, const Rect* rects, INT count);

Status DrawRectangle(const Pen* pen, const RectF& rect);

Status DrawRectangle(const Pen* pen, REAL x, REAL y, REAL width, REAL height);

Status DrawRectangles(const Pen* pen, const RectF* rects, INT count);

绘制椭圆的函数，如果输入参数所确定的外接矩形的宽高相等，则画圆。（也是前两个为整数版，后两个为对应的浮点数版）

Status DrawEllipse(const Pen* pen, 
const Rect& rect);

Status DrawEllipse(const Pen* pen, INT x, INT y, INT**width**, INT**height**)

Status DrawEllipse(const Pen* pen, const RectF& rect);

Status DrawEllipse(const Pen* pen, REAL x, REAL y, REAL width, REAL height);

绘制椭圆弧的函数，如果输入参数所确定的**外接矩形**的宽高相等，则画圆弧。（也是前两个为整数版，后两个为对应的浮点数版）

Status **DrawArc**(const Pen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REALsweepAngle);**// sweep 掠过**

Status DrawArc(const Pen* pen, const Rect& rect, REAL startAngle, REAL sweepAngle);

Status DrawArc(const Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);

Status DrawArc(const Pen* pen, const RectF& rect, REAL startAngle, REAL sweepAngle);

![](http://hi.csdn.net/attachment/201010/28/0_1288270714d11d.gif)

画弧函数的输入参数** // 注意：顺时钟方向**

该函数的功能与GDI的Arc相同：

BOOL **Arc**( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 );

BOOL Arc( LPCRECT lpRect, POINT ptStart, POINT ptEnd );

但是也有区别，主要是，最后的参数不再是弧的终角，而是弧段所对应的**扫描角**。这倒是与另一个GDI画圆弧函数类似（其中(x, y)为圆心、nRadius为半径、fStartAngle为起始角、fSweepAngle也为弧段跨角）：

BOOL **AngleArc**(int x, int y, intnRadius, float fStartAngle, floatfSweepAngle);

当然，GDI+确定矩形的后两个参数也不再是右下角坐标，而改成宽高了，这已经是老问题了。

另外要注意的是，角度的单位是**度**（不是弧度，C++的三角函数采用的是**弧度**单位），而且都必须是实数。**零度角为x轴方向，顺时针方向为正**（这与数学上反时针方向为正刚好相反）。//
 如上图所示。

绘制多边形的函数，第一个为整数版，后一个为对应的浮点数版：

Status DrawPolygon(const Pen* pen, const Point* points, INT count);

Status DrawPolygon(const Pen* pen, const PointF* points, INT count);

其中，各参数的含义同画折线函数DrawLine**s**的，只是Draw**Polygon**函数会将点数组中的**起点**和**终点连接起来**，形成一个**封闭的多边形区域**。

该函数的功能与GDI的Polygon相同：

BOOL Polygon( LPPOINT lpPoints, int nCount );

注意：GDI+中没有提供，与GDI函数RoundRect（圆角矩形）和Chord（弓弦），具有类似功能的绘图函数。可以利用**矩形+椭圆**和**弧+直线**等函数自己来实现。

在GDI+中画填充图，不需像GDI那样得先将刷子选入DC，而是与GDI+画线状图的函数类似，将刷子作为画填充图函数的第一个输入参数。

l**画填充矩形[组] FillRectangle[s]**

Status **Fill**Rectangle(const Brush*brush,**const Rect&** rect);

Status FillRectangle(const Brush* brush, INT x, INT y, INT width, INT height);

Status FillRectangle**s**(const Brush* brush,**const Rect***rects, INT count);

Status FillRectangle(const Brush* brush, const RectF& rect);

Status FillRectangle(const Brush* brush, REAL x, REAL y, REAL width, REAL height);

Status FillRectangles(const Brush* brush, const RectF* rects, INT count);

用指定刷子Brush，填充rect的内部区域，无边线，**填充区域包括矩形的左边界和上边界，但不包括矩形的右边界和下边界。**功能与GDI的FillRect类似：

void FillRect( LPCRECT lpRect, CBrush* pBrush );

但是，GDI中没有同时填充一个矩形数组的函数。不过GDI却有GDI+没有的画填充圆角矩形的函数FillSolidRect。

l         **画填充[椭]圆FillEllipse**

Status **FillEllipse**(const Brush* brush, const Rect& rect);

Status FillEllipse(const Brush* brush, INT x, INT y, INT width, INT height);

Status FillEllipse(const Brush* brush, const RectF& rect);

Status FillEllipse(const Brush* brush, REAL x, REAL y, REAL width, REAL height);

GDI中没有类似函数，但可以用（采用当前刷填充的）Ellipse来代替。

l         **画饼图DrawPie// pie馅饼    DrawPie与FillPie**

Status **DrawPie**(const Pen* pen, const Rect& rect, REAL startAngle, REALsweepAngle);

Status DrawPie(const Pen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);

Status DrawPie(const Pen* pen, const RectF& rect, REAL startAngle, REAL sweepAngle);

Status DrawPie(const Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);

与GDI的下列函数类似，但是部分输入参数的含义有所不同：

    BOOL **Pie**( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 );

BOOL Pie( LPCRECT lpRect, POINT ptStart, POINT 
ptEnd );

l         **画填充多边形FillPolygon**

Status FillPolygon(const Brush* brush, const Point* points, INT count);

Status FillPolygon(const Brush* brush, const Point* points, INT count, 
FillMode fillMode);

Status FillPolygon(const Brush* brush, const PointF* points, INT count);

Status FillPolygon(const Brush* brush, const PointF* points, INT count, FillMode fillMode);

前面讲的各种画线状图或填充图的GDI+函数，虽然在形式上与GDI的有所不同（**函数名前加了Draw或Fill、将笔或刷作为第一个输入参数、部分位置输入参数改成了大小参数、并增加了浮点数版**），但是在功能上却是相同的。

现在要讲的**曲线绘制**，则是GDI+新增加的内容。曲线在机械设计、工程建筑和图形动画等领域，都有十分广泛应用。

常用的曲线有**Bezier（贝塞尔）曲线**和**样条（spline）曲线**。贝塞尔曲线比较简单，适合于画控制点少的曲线。当控制点太多时，要不曲线的次数（比点数少1）太高，要不拼接比较困难，而且没有局部性（即修改一点影响全局），性能不太好。而样条曲线则可以画任意多个控制点的曲线，曲线的次数也可以指定（一般为二次或三次，如TrueType字体采用的是二次B样条曲线），并且具有局部性。贝塞尔曲线特别是样条曲线有很多变种。常见的贝塞尔曲线有普通贝塞尔曲线和有理贝塞尔曲线。常用的样条曲线有：B样条、β样条、Hermite（厄密）样条、**基数样条**、Kochanek-Bartels样条和Catmull-Rom样条等。

GDI+中所实现的是普通贝塞尔曲线（不过控制点，位于控制多边形的凸包之内）和基数样条曲线（过控制点）。

l         **基数样条曲线（cardinal spline curve）  // DrawCurve与DrawClosedCurve**

Status Draw**Curve**(const Pen* pen, const Point* points, INT count); // tension = 0.5f

Status DrawCurve(const Pen* pen, const Point* points, INT count, REAL tension);

Status DrawCurve(const Pen* pen, const Point* points, INT count, INT offset, **INT numberOfSegments**, REAL tension = 0.5f); // 只画部分点

Status DrawCurve(const Pen* pen, const PointF* points, INT count);

Status DrawCurve(const Pen* pen, const PointF* points, INT count, REAL tension);

Status DrawCurve(const Pen* pen, const PointF* points, INT count, INT offset, INT numberOfSegments, REAL tension = 0.5f);

Status Draw**ClosedCurve**(const Pen* pen, const Point* points, INT count);

Status DrawClosedCurve(const Pen *pen, const Point* points, INT count, REAL tension);

Status DrawClosedCurve(const Pen* pen, const PointF* points, INT count);

Status DrawClosedCurve(const Pen *pen, const PointF* points, INT count, REAL tension);

其中：

**参数tension（张力）指定曲线的弯曲程度，tension = 0.0（直线）~ 1.0（最弯曲）**

无张力版的函数的 **tension = 0.5**（缺省值）

第3/6个DrawCurve，只画从points[offset]开始的numberOfSegments个点组成的部分曲线段

DrawClosedCurve函数（连接首尾点）画封闭的基数样条曲线

l         **贝塞尔曲线（Bezier curve） DrawBezier**

Status DrawBezier(const Pen* pen, INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4);

Status DrawBezier(const Pen* pen, const Point& pt1, const Point& pt2, const Point& pt3, const Point& pt4);

Status DrawBeziers(const Pen* pen, const Point* points, INT count);

Status DrawBezier(const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4);

Status DrawBezier(const Pen* pen, const PointF& pt1, const PointF& pt2, const PointF& pt3, const PointF& pt4);

Status DrawBeziers(const Pen* pen, const PointF* points, INT count);

l         **填充封闭基数样条曲线   FillClosedCurve**

Status FillClosedCurve(const Brush* brush, const Point* points, INT count);

Status FillClosedCurve(const Brush* brush, const Point* points, INT count, 
FillMode fillMode, REAL tension = 0.5f);

Status FillClosedCurve(const Brush* brush, const PointF* points, INT count);

Status FillClosedCurve(const Brush* brush, const PointF* points, INT count, FillMode fillMode, REAL tension = 0.5f);

GDI中没有用于清屏的专门函数，得自己用背景色画窗口大小的填充矩形，或者调用窗口类的Invalidate和UpdateWindow函数。现在，GDI+有了**清屏函数Clear**：

Status Clear(const Color &color);

其中的输入参数color，为用户指定的填充背景色。例如：

    Graphics graph(**GetDC()->m_hDC**);

    graph.Clear(Color::White);  **// 使用Graphics类之对象调用**

4、  Pen

与GDI中的一样，GDI+中的笔（pen钢笔/画笔）也是画线状图的工具，但是功能更加强大。例如：**透明笔、图案笔、自定义虚线风格、线帽、笔的缩放和旋转、笔的连接点属性**等。

GDI+中的笔对应于Pen类，被定义在GdiplusPen.h头文件中。

笔的构造函数主要有两个：

Pen(**const Color &**color, REAL width = 1.0); // 单色笔

Pen(const Brush *brush, REAL width = 1.0); // **纹理图案笔**

其中，最常用的是第一个，它构造一个颜色为color，宽度为width（缺省为1）的单色笔。**如果颜色的****α值<255，则所创建的笔就是带透明度的笔**。

5、  Brush

与GDI中的一样，GDI+中的刷（brush画刷/画笔）也是画填充图的工具，GDI+中也有与GDI相对应的实心刷（单色刷）、条纹刷（影线刷）和纹理刷（图像刷）。不过，GDI+又新增加了功能强大的线性渐变刷和路径渐变刷，而且还为所有这些刷各自建立了对应的类，基类是Brush（功能少）。

下面是GDI+中各种刷类的层次结构图：

GdiplusBase

Brush

**SolidBrush**

**HatchBrush**

**TextureBrush**

**LinearGradientBrush**

**PathGradientBrush**

![](http://hi.csdn.net/attachment/201010/28/0_128827072208xm.gif)

GDI+刷类的层次结构

**// (1) SolidBrush实心刷**

**// (2) HatchBrush 条纹刷**

**// (3) TextureBrush 纹理刷**

**// (4) LinearGradientBrush 线性渐变刷   // gradient倾斜的,梯度**

**// (5) PathGradientBrush 路径渐变刷**

所有刷类都被定义在头文件GdiplusBrush.h中。

6、  文字

GDI+的文本排版和字体处理的功能比GDI的更加强大。特别是Windows XP提供了对LCD（液晶）显示器的特殊优化功能，GDI+也提供了对应的ClearType（清晰活字）文字处理技术，以增强字体的清晰度。另外，GDI+还提供了构造专用字体集的功能，可以包含私有的临时字体（不需预先安装到系统中）。

Windows中使用的字体，一般是TrueType（真实活字）字体（TTF = TrueType Font），它是1991年Apple 和Microsoft 联合开发的一种字体技术，采用二次B样条曲线来描述字符的轮廓。

在GDI+中，与文字相关的类有：**字体族类FontFamily**、**字体类Font**和字体集类FontCollection及其两个派生类InstalledFontCollection（已安装字体集）和PrivateFontCollection（专用字体集）。（在GDI中只有CFont一个字体类）

这些类的层次结构为：

FontCollection

InstalledFontCollection

PrivateFontCollection

Font

FontFamily

GdiplusBase

![](http://hi.csdn.net/attachment/201010/28/0_1288270733aEHe.gif)

在GDI中，我们用CDC类的成员函数TextOut、DrawText和ExtTextOut等来输出文本串。在GDI+中，我们则是利用Graphics类的重载成员函数DrawString来绘制文本。

7、  路径

**路径（path）**是一系列相互连接的直线和曲线，由许多不同类型的点所构成，用于表示复杂的不规则图形，也叫做图形路径（graphics path）。路径可以被画轮廓和填充，也可以用于创建区域和路径渐变刷等。

在GDI中也有路径（我们没有讲），但是它只是作为DC的一种状态才能存在。独立的路径对象，则是GDI+的新特点。

8、  区域

区域（region）由若干几何形状所构成的一种封闭图形，主要用于复杂图形的绘制、图形输出的剪裁和鼠标击中的测试。最简单也是最常用的区域是矩形，其次是椭圆和多边形以及它们的组合。这些也正是GDI所支持的区域类型。

GDI+中的区域是一种显示表面的范围（an area of the display surface），可以是任意形状（的图形的组合），边界一般为路径。除了上面所讲的矩形、椭圆、多边形之外，其边界还可以含直线、折线、弧、贝塞尔曲线和样条曲线等开图形，其内容还可以包含饼、闭曲线等闭图形。

在GDI+中，**区域**所对应的类是**Region**，它是一个独立的类（没有基类，也没有派生类）。但是它又若干相关的类，如各种图形类和图形路径类等。

Region类有6个构造函数：

Region(VOID); // 创建一个空区域

**Region**(const **Rect &**rect); // 创建一个整数型矩形区域

Region(const RectF &rect); // 创建一个浮点数型矩形区域

**Region**(const **GraphicsPath ***path); // 由图形路径来创建区域

Region(const **BYTE ***regionData, INT size);// 由（另一）区域的数据构造区域

Region(**HRGN** hRgn); // 由GDI的区域句柄构造区域

其中，创建矩形区域最简单，由路径创建区域最常用。

9、  变换

**变换（transform）**是GDI+新增加的强大功能，包括图形对象的简单变换和基于矩阵的坐标变换、图形变换、图像变换、色彩变换、路径变换和区域变换等。

GDI+的核心——图形类Graphics，提供了3个成员函数，可以对其所绘制的图形进行**平移（translate）、旋转（rotate）和伸缩（scale比例尺/缩放）**等基本的图形变换：（与纹理刷类中的对应函数的原型是一样的）

Status **Translate**Transform(REAL dx, REAL dy, MatrixOrder order = MatrixOrderPrepend);

Status **Rotate**Transform(REAL angle, MatrixOrder order = MatrixOrderPrepend);

Status **Scale**Transform(REAL sx, REAL sy, MatrixOrder order = MatrixOrderPrepend);

其中的最后一个输入参数为矩阵相乘的顺序，取值为矩阵顺序枚举类型MatrixOrder中的符号常量，缺省值都为MatrixOrderAppend（左乘）：

typedef enum {

    MatrixOrder**Prepend** = 0, //矩阵**左乘**（预先序，前置）

    MatrixOrder**Append** = 1 //矩阵**右乘**（追加序，后缀）

} **MatrixOrder**;

因为这些变换都可以用矩阵表示，而且与图形对象已经设置的现有变换矩阵要进行合成（相当于两个变换矩阵进行乘法运算）。

在图形对象的这三种基本变换中，最常用的是第一种——平移变换。我们在前面曾多次使用，避免了重复定义（有坐标平移的）绘图区域的麻烦。

10、              图像

GDI+支持如下9种用于Windows的常见图像格式：

l     BMP——BitMaP（位图），扩展名为.BMP，由Microsoft与IBM于1980年代中期为Windows和PS/2制订的图像格式，一般不压缩。支持黑白、伪彩图、灰度图和真彩图，每像素位数可为1、4、8、16、24、32、64等，常用的是24位位图。

l     GIF——Graphics Interchange Format（可交换图形格式），扩展名为.GIF，由CompuServe公司1987年制定，采用无损的变长LZW压缩算法。只支持伪彩图（最多256索引色），宽高用双字节无符号整数表示（最多64K*64K像素）。可存储多幅图片，常用于简单的网络动画。压缩比较高，使用广泛。

l     JPEG——Joint Photographic Experts Group（联合图象专家组），扩展名为.JPG，是国际标准化组织ISO和IEC于1991年联合制定的静态图像压缩标准，采用以DCT为主的有损压缩方法。支持灰度图和真彩图，但是不支持伪彩图。压缩比高，使用广泛。

l     Exif——EXchangeable Image File Format（可交换图像文件格式），扩展名为.Exit？，由JEIDA（Japan Electronic Industry Development Association日本电子工业发展协会/日本电子情报技术产业协会）于1996年10月制定。用于数码相机，内含JPEG图像，另包含拍摄日期、快门速度、曝光时间、照相机制造厂商和型号等相关信息。

l     PNG——Portable Network Graphic Format（可移植网络图形格式，读成“ping”），扩展名为.png，由W3C(World Wide Web Consortium万维网协会)于1996年10月推出的一种标准图像格式，2003年成为ISO国际标准。PNG采用与GIF一样的无损压缩方法，但是除了伪彩图外，PNG还支持多达16位深度的灰度图像和48位深度的彩色图像，并且还可支持多达16位的α通道数据。

l     TIFF——Tag Image File Format（标签图像文件格式），扩展名为.tif，由Aldus于1986年秋联合多家扫描仪制造商和软件公司共同开发，支持黑白、索引色、灰度、真彩图，可校正颜色和调色温，支持多种压缩编码（如Huffman、LZW、RLE），可存储多幅图片。常用于对质量要求高的专业图像的存储。

l     ICON——icon（图标），扩展名为.ico，由Microsoft与IBM于1980年代中期为Windows和PS/2制订的图标图像格式。图像大小为16*16、32*32或54*64。

l     WMF——Windows MetaFile（视窗元文件），扩展名为.WMF，由Microsoft与IBM于1980年代中期为Windows和PS/2制订的图形文件格式，用于保存GDI的绘图指令记录。

l     EMF——Enhanced Windows MetaFile（增强型视窗元文件），扩展名为.EMF，是微软公司于1993年随32位操作系统Windws NT推出的一种改进的WMF格式，用于Win32。GDI+使用的是扩展EMF格式——EMF+。

GDI+的图像及其处理的功能十分强大，可以用不同的格式加载、保存和操作图像。但由于篇幅所限，本小节只介绍最基本的内容。

GDI+中有三个图像类，其中的**Image（图像）**为基类，其他两个为它的派生类——**Bitmap（位图）**和**Metafile（[图]元文件/矢量图形）**。它们的类层次结构如下图所示：

GdiplusBase

Image

Bitmap

Metafile

![](http://hi.csdn.net/attachment/201010/28/0_12882707408nN1.gif)

图像类的层次结构

除此之外，还有大量与图像处理有关的GDI+类，如Effect类及其11个派生类以及与图像数据和信息有关的7个独立类。由于时间关系，我们只准备介绍上面这三个主要的图像类及其基本操作。

11、              图元文件

从一开始GDI就支持**（图）元文件（metafile）**，早期（1985年）的版本为**WMF（Windows MetaFile视窗元文件）**，主要针对Win16（Win3.x），后来（1990年）也支持Win32（Win95/ 98/Me）。以后（1993年）随Windows NT推出了改进的元文件版本——**EMF（Enhanced Windows MetaFile增强型视窗元文件）**，只支持Win32（Win95/98/Me/NT/2000/XP）。现在（2001年）又随GDI+推出了加强型EMF——**EMF+**，可以同时支持GDI和GDI+。

元文件所支持的GDI类型

|元文件类型|Win16 GDI|Win32 GDI|Win32/64 GDI+|
|----|----|----|----|
|WMF|√|√|×|
|EMF|×|√|×|
|**EMF+**|×|√|√|

虽然在GDI+中，将**图元文件**所对应的类**Metafile**作为Image的派生类，但这只是为了**图元文件**可以同时处理**图形**和**图像**。其实**图元文件**中所包含的就是一系列**绘图**（包括绘制图像）**指令及参数**，属于**矢量图形文件**。它所占空间小、可以任意缩放（不会产生马赛克效应），但是绘制图形需要一定的时间。

在GDI+中，**图元文件**对应的类为**Metafile**，它是**Image类**的派生类。GDI+的Metafile类支持三种类型的图元文件：仅EMF类型、仅EMF+类型、EMF及**EMF+双重类型（缺省值）**。它们对应于枚举类型：

typedef enum {

    EmfTypeEmfOnly = MetafileTypeEmf, // 仅EMF类型

    EmfTypeEmfPlusOnly = MetafileTypeEmfPlusOnly, // 仅EMF+类型

    EmfTypeEmfPlusDual = MetafileTypeEmfPlusDual // EMF及EMF+双重类型

} **EmfType**;  // enhance meta file

Metafile类有13个构造函数：

// 文件型

Metafile(const WCHAR *filename);

Metafile(const WCHAR *fileName, HDC referenceHdc, EmfType type = EmfTypeEmfPlusDual, const WCHAR *description = NULL);

Metafile(const WCHAR *fileName, HDC referenceHdc, const Rect &frameRect, MetaFileFrameUnit frameUnit =  MetafileFrameUnitGdi, EmfType type = EmfTypeEmfPlusDual, const WCHAR *description = NULL);

Metafile(const WCHAR *fileName, HDC referenceHdc, const RectF &frameRect, MetafileFrameUnit frameUnit =  MetafileFrameUnitGdi, EmfType type = EmfTypeEmfPlusDual, const WCHAR *description = NULL);

// 流型

Metafile(IStream *stream);

Metafile(IStream *stream, HDC referenceHdc, EmfType type = EmfTypeEmfPlusDual, const WCHAR *description = NULL);

Metafile(IStream *stream, HDC referenceHdc, const Rect &frameRect, MetafileFrameUnit frameUnit =  MetafileFrameUnitGdi, EmfType type = EmfTypeEmfPlusDual, const WCHAR *description = NULL);

Metafile(IStream *stream, HDC referenceHdc, const RectF &frameRect, MetafileFrameUnit frameUnit =  MetafileFrameUnitGdi, EmfType type = EmfTypeEmfPlusDual, const WCHAR *description = NULL);

// DC句柄型

Metafile(**HDC** referenceHdc, EmfType type = EmfTypeEmfPlusDual, const WCHAR *description = NULL);

Metafile(HDC referenceHdc, const Rect &frameRect, MetafileFrameUnit frameUnit =  MetafileFrameUnitGdi, EmfType type = EmfTypeEmfPlusDual, const WCHAR *description = NULL);

Metafile(HDC referenceHdc, const RectF &frameRect, MetaFileFrameUnit frameUnit =  MetafileFrameUnitGdi, EmfType type = EmfTypeEmfPlusDual, const WCHAR *description = NULL);

// WMF/EMF句柄型

Metafile(HENHMETAFILE hEmf, BOOL deleteEmf = FALSE);

Metafile(HMETAFILE hWmf, const WmfPlaceableFileHeader *wmfPlaceableFileHeader, BOOL deleteWmf = FALSE);

其中用到的枚举类型有：

typedef enum {

    MetafileFrameUnitPixel = UnitPixel, // 象素

    MetafileFrameUnitPoint = UnitPoint, // 点

    MetafileFrameUnitInch = UnitInch, // 英寸

    MetafileFrameUnitDocument = UnitDocument, // 文挡

    MetafileFrameUnitMillimeter = UnitDocument + 1, // 毫米

    MetafileFrameUnitGdi = UnitDocument + 2 // GDI+单位数目

} Metafile**Frame**Unit;

typedef struct {

    UINT32 Key; // 键

    INT16 Hmf; // 

    PWMFRect16 BoundingBox; // 边界盒

    INT16 Inch; // 英寸

    UINT32 Reserved; // 保留

    INT16 Checksum; // 检测和

} Wmf**Placeable**FileHeader;

[http://www.caenet.cn/Forums/](http://www.caenet.cn/Forums/)

其中，最简单常用的构造函数是：// 不带DC参数，只能用于打开已经存在的元文件

**Metafile**(const WCHAR *filename);

它由**文件名**来构造元文件对象。例如：

Metafile mf(L"yyy.emf");

常用且完整的构造函数是：// 带DC参数，只用于创建新图元文件

Metafile(const WCHAR *fileName,**HDC**referenceHdc,**EmfType** type = EmfTypeEmfPlusDual,
 const WCHAR *description = NULL);

它可以指定元文件类型，并加上描述串。例如：

Metafile mf(L"yyy.emf", **GetDC()**->m_hDC, MetafileTypeEmf, L"阴阳鱼");

另一个较为常用的构造函数是：

**Metafile**(**HDC** referenceHdc,**EmfType** type = EmfTypeEmfPlusDual, const WCHAR *description
 = NULL);

它用于构造**内存元文件**。这些内存元文件构造函数还有对应的流构造函数版本。

Metafile类的其他成员函数有： 

// **显示**元文件记录，需要与Graphics类的EnumerateMetafile

Status **PlayRecord**(EmfPlusRecordType recordType, UINT flags, UINT dataSize, const BYTE *data);

// 函数及用户自定义的**回调函数**配套使用（似GDI的）

static UINT **Emf**To**Wmf**Bits(HENHMETAFILE hemf, UINT cbData16, LPBYTE pData16, INT iMapMode, EmfToWmfBitsFlags eFlags); //用于EMF到WMF的转换

H**ENH**METAFILE
Get**HENH**METAFILE(VOID); // 可用于EMF的SDK函数

// 获取和设置底层光栅限制，用于减少刷空间大小  ** // 陈宝楷???**

UINT GetDownLevelRasterizationLimit(VOID);

Status SetDownLevelRasterizationLimit(UINT metafileRasterizationLimitDpi);

// 获取元文件头

Status GetMetafileHeader(MetafileHeader *header) const;

static Status GetMetafileHeader(const WCHAR *filename, MetafileHeader *header);

static Status GetMetafileHeader(IStream *stream, MetafileHeader *header);

static Status GetMetafileHeader(HENHMETAFILE *hEmf, MetafileHeader *header);

static Status GetMetafileHeader(HMETAFILE hWmf, const WmfPlaceableFileHeader *wmfPlaceableFileHeader, MetafileHeader *header);

为了将绘图记录**保存**到图元文件中，需要先创建**元文件对象**，然后用该图元文件对象再来创建**图形对象**，最后调用图形类的各种绘图函数来向图元文件中**添加绘图记录**。

具体方法如下：

可以先使用Metafile类的用于创建新图元文件的构造函数（带DC参数的），如

Metafile(const WCHAR *fileName,**HDC**referenceHdc, EmfType type =

EmfTypeEmfPlusDual, const WCHAR *description = NULL);

来创建元文件对象。

然后使用Graphics类的构造函数（注意，**Metafile是Image的派生类**）

Graphics(**Image* **image);

来创建图形对象。

最后调用各种图形类的图形设置、操作和绘制函数成员函数来向图元文件添加**绘图记录**。

例如：

**Metafile ***myMetafile =**new**Metafile(L"MyDiskFile.emf", GetDC()->m_hDC);

   Graphics *myGraphics = new Graphics(myMetafile);

   // SmoothingMode**::**tiAlias不能在VC中使用，可在C#中使用。

myGraphics->SetSmoothingMode(SmoothingModeAntiAlias);

   myGraphics->RotateTransform(30);

   // Create an elliptical clipping region.

   GraphicsPath myPath;

   myPath.AddEllipse(0, 0, 200, 100);

   Region myRegion(&myPath);

   myGraphics->SetClip(&myRegion);

   Pen myPen(Color(255, 0, 0, 255));

   myGraphics->DrawPath(&myPen, &myPath);

   for(int j=0; j<=300; j+=10) myGraphics->DrawLine(&myPen,0,0,300-j,j);

   delete myGraphics;

**delete**
myMetafile;

可以先使用Metafile类的用于打开已有图元文件的构造函数（不带DC参数的），如

Metafile(const WCHAR *filename);

来创建元文件对象。

然后再调用Graphics类的各种DrawImage成员函数，如：

Status DrawImage(Image *image, INT x, INT y); 

来重画图元文件中的所有绘图记录。

另外，为了获取当前图元文件的边界矩形，可以先调用**Metafile类**的成员函数：

Status GetMetafile**Header**(**MetafileHeader ***header) const;

来获取MetafileHeader对象，然后再用**MetafileHeader类**的成员函数：

void Get**Bounds**(**Rect ***rect);

得到边界矩形。可用于**Graphics类**的DrawImage成员函数：

Draw**Image**(**Image ***image, const Rect &rect);

注意，如果用带DC参数的构造函数来创建Metafile对象，则会**清空**原图元文件（以便重新开始添加记录），不能用于图元文件的播放。

可以利用Metafile类的成员函数

Status Play**Record**(EmfPlusRecordType recordType, UINT flags,

UINT dataSize, const BYTE *data); 

来重画图元文件中**指定记录**。与EMF中讨论的类似，该函数需要与Graphics类的枚举元文件成员函数（共有12个同名的重载函数），如：

Status**Enu**merateMetafile(const Metafile *metafile, const PointF &destPoint,**EnumerateMetafileProc**callback,
 VOID *callbackData = NULL, ImageAttributes *imageAttributes = NULL);

配套使用，该函数遍历图元文件的每个记录，并调用用户自定义的回调函数（该函数可以自己命名）

BOOL **CALLBACK** metaCallback(**EmfPlusRecordType** recordType, unsigned int flags, unsigned int dataSize, const unsigned char* pStr, void* callbackData);

对记录进行各种处理，包括使用元文件的成员函数PlayRecord来绘制（播放）记录。

在GDI+中，想实现**交互绘图时**的窗口动态重画，非常困难。

虽然Metafile类有一个成员函数

HENHMETAFILE GetHENHMETAFILE(VOID);

可以用于获取图元文件的**句柄**，但经过我的实验发现，它只对使用**不带DC**输入参数的构造函数所创建的不能用于添加绘图记录的Metafile对象有效。

另外，虽说可以创建内存Metafile对象，但是GDI+却没有提供任何办法（没有复制、保存、克隆等函数，父类Image的对应函数对写入型Metafile对象都是无效的），可将其保存到图元文件中。因为无法获得用于添加记录的图元文件的句柄，所以各种SDK函数也派不上用场。

因为除了**帮助文档**，几乎无资料可看，唯一的途径就是**编码做试验**。下面是我经过很长时间，好不容易才摸索出来的，一种可行的解决办法（但是很臭。你们可以寻找其他办法，如果有了更好的方法，请与大家共享）：

（说明：为了防止重画图元文件时，图形的位置有偏移或其大小发生变化，可以采用如下的构造函数：

**Metafile**(const WCHAR *fileName, HDC referenceHdc, const Rect &frameRect, MetaFileFrameUnit frameUnit =  MetafileFrameUnitGdi, EmfType type = EmfTypeEmfPlusDual, const WCHAR *description = NULL);

来创建Metafile对象。其中的边框矩形，可以设置为屏幕大小，并使用像素单位。该边框同时还用于进行图元文件重画的Graphics类的DrawImage函数。）

在视图类中定义如下几个类变量：Metafile对象及其对应的Graphics对象的指针、边框矩形、两个图元文件名的宽字符串数组（以便绕开GDI+的文件锁定功能）、以及在这两个文件名中切换的整数。如：

    Metafile *mf;

    Graphics *mfGraph;

    Rect rect0;

**wchar_t ***fns[2];   
// **wchar_t**

    int fni;

在视图类的构造函数中，初始化部分类变量：

    mf = NULL;

    mfGraph = NULL;

    fns[0] = **L**"draw.emf";

    fns[1] = **L**"draw0.emf";

    fni = 0;

在视图类的初始化函数OnInitialUpdate中，计算边框矩形、创建Metafile对象：

    HDC hdcRef = GetDC()->m_hDC; 

    rect0.X = 0;

    rect0.Y = 0;

    rect0.Width = GetDeviceCaps(hdcRef,**HORZRES**);

    rect0.Height = GetDeviceCaps(hdcRef,**VERTRES**);

    mf = new Metafile(fns[fni], hdcRef, rect0, MetafileFrameUnitPixel);

    mfGraph = new Graphics(mf);

在视图类的OnLButtonUp等函数中，利用图元文件所对应的图形对象，向图元文件添加各种绘图记录。如：

    mfGraph->DrawLine(&Pen(Color::Green), p0.x, p0.y, point.x , point.y); 

    ……

在视图类的OnDraw函数中，删除当前元文件对象（系统才会将元文件的内容写入磁盘）和对应的图形对象，打开该磁盘元文件并播放。然后，切换文件名，创建新的元文件对象和对应的图形对象，并将老元文件中现有的记录，通过新元文件所对应的图形对象的图像绘制，加入到新元文件中，最后删除老元文件的句柄。如：

    delete mfGraph;

    delete mf;

    Metafile *mf0 = new Metafile(fns[fni]);

    Graphics graph(pDC->m_hDC);

    graph.DrawImage(mf0, rect0);

fni = !fni; // 相当于if(fni) fni = 0; else fni = 1;

    mf = new Metafile(fns[fni], pDC->m_hDC, rect0, MetafileFrameUnitPixel);

    mfGraph = new Graphics(mf);

    mfGraph->DrawImage(mf0, rect0);

    delete mf0;

最后，在视图类的析构函数中，删除当前元文件对象（系统会将元文件的内容写入磁盘）和对应的图形对象。例如：

    delete mfGraph;

    delete mf;

三、GDI+使用过程中出现的问题：

1）、在VC调用过程中，重绘问题。

GDI+程序往往在窗口被创建时，不能自动重画（没有自动调用OnDraw函数）。解决办法是，在创建图形对象后，自己**调用视图类（基类CWnd）的成员函数RedrawWindow**：

BOOL RedrawWindow(LPCRECT lpRectUpdate = NULL, CRgn* prgnUpdate = NULL,

UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE); 

其中，lpRectUpdate为窗口客户区中需要重画的矩形（NULL表示整个客户区矩形重画）、prgnUpdate表示需要重画的区域（NULL表示整个客户区矩形区域重画）、flags为特征标志（RDW_INVALIDATE指定范围无效、RDW_UPDATENOW立即更新、RDW_ERASE擦除背景）。

例如：

Graphics graph(pDC->m_hDC);

RedrawWindow(); // 一般输入参数取缺省值即可

// 相当于Invalidate(); **UpdateWindow()**;的综合效果 //** this->Invalidate();**

注意：不能在OnDraw和OnPaint函数中调用RedrawWindow，那样会造成反复调用，产生死循环。

其实，只要GDI+的两个初始化语句放置的位置对（必须放在CWinApp:: InitInstance ();语句之前），就不会出现该问题。

2）、new 问题。

**不能使用new来动态创建GDI+对象。**解决办法是：打开（缺省）位于C:/Program Files/Microsoft Visual Studio 8/VC/PlatformSDK/Include目录中的GdiplusBase.h头文件，并注释掉里面GdiplusBase类的内容（该类其实只含new、new[]、delete和delete[]这四个运算符的重载），使其成为一个空类（不要删除整个类）。

对实验室中的写保护机器，不能修改安装目录中的GdiplusBase.h头文件，解决办法是：

l         将该头文件复制到你的项目目录中；

l         注释掉该头文件里面GdiplusBase类的内容（保留类定义）；

l         在你项目中所有的#include <gdiplus.h>语句之前，包含"GdiplusBase.h"头文件，形如：

#include "gdiplusBase.h"

#include <gdiplus.h>

l         则编译系统会优先包含项目目录中的gdiplusBase.h头文件，从而屏蔽掉原来位于平台SDK的Include目录中的同名头文件。 // 技巧

你也可以**在有些使用new的地方改用&**，例如你可以将代码

Pen *pPen = **new Pen**(Color::Red); **// 在C#可运行**

改为

Pen *pPen = **&Pen**(Color::Red); 

又例如，你也可以将代码：

graphics.DrawPolygon(**new** Pen(Color::Green), points, n);

改为

Pen pen(Color::Green);

graphics.DrawPolygon(&pen, points, n);

或直接改为

graphics.DrawPolygon(**&**Pen(Color::Green), points, n);

3)、调试问题

现在版本的VC05存在许多Bug，特别是GDI+程序在调试时的问题就更多。解决办法是：

l     在编译运行时，不使用Debug配置，而改用Release配置；

l     运行时不使用调试运行（F5），而改用不调试直接运行（Ctrl +F5）；

l     最好是用静态链接的MFC库，而不用DLL动态库。

常用的调试方法有：

l     使用MessageBox信息框：

n         在视图类中的常用格式为

MessageBox(L"提示信息");

n         在应用程序类和文档类中的常用格式为

MessageBox(NULL, L"提示信息", L"标题", MB_OK);  // 
Win32 API 

l设置断点，然后逐步运行（F10）或F11。

l运行当前位置，然后逐步运行（F10）

l利用调试界面中的“局部变量”和“监视1”等窗口，来查看变量当前的值

4)、用MFC开发GDI+程序

创建一个名为GDIPlusDemo的MFC单文档应用程序项目。

首先要进行GDI+系统的初始化，这需要在应用程序类CGDIPlusDemoApp中声明一个成员变量：

**ULONG_PTR m_gdiplusToken; **// ULONG PTR 为int64 类型

并在该类的初始化函数CGDIPlusDemoApp::**InitInstance() **中加入以下代码来对GDI+进行初始化：

**GdiplusStartupInput gdiplusStartupInput;**

**GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);**

注意：这两个语句必须加在应用程序类的InitInstance函数中的

**CWinApp:: InitInstance ();   **

语句**之前**，**不然以后会造成视图窗口不能自动重画、程序中不能使用字体等等一系列问题**。

还要在CGDIPlusDemoApp::**ExitInstance() **函数中加入以下代码来关闭GDI +：

GdiplusShutdown(m_gdiplusToken);

上面的InitInstance和ExitInstance都是应用程序类的重写型成员函数。而且，缺省时无ExitInstance，需要自己利用**属性窗口**来添加（不要手工添加）。

接下来就可以利用GDI+进行绘图了。

在OnDraw函数中画图：

CGDIPlusDemoView::OnDraw (CDC* pDC) {

    Graphics graph(pDC->m_hDC); // 创建图形对象

    Pen bluePen(Color(255, 0, 0, 255)); // 创建蓝色笔

    Pen redPen(Color(255, 255, 0, 0)); // 创建红色笔

    int y = 255; // y的初值

    for (int x = 0; x < 256; x += 5) { // 绘制红蓝网线

        graph.DrawLine(&bluePen, 0, y, x, 0);

        graph.DrawLine(&redPen, 255, x, y, 255);

        y -= 5;

    }

    for (y = 0; y < 256; y++) { // 画绿色透明度水平渐变的线（填满正方形）

        Pen pen(Color(y, 0, 255, 0)); // A green pen with shifting alpha

        graph.DrawLine(&pen, 0, y, 255, y);

    }

    for (int x = 0; x < 256; x++) { // 画品红色透明度垂直渐变的线（填满扁矩形）

        Pen pen(Color (x, 255, 0, 255)); // A magenta pen with shifting alpha

        graph.DrawLine(&pen, x, 100, x, 200);

    }

}

运行的结果如图所示。（其中，第一个图为第一个循环所绘制的结果、第二个图为前两个循环所绘制的结果、第三个图为全部循环所绘制的结果）

![](http://hi.csdn.net/attachment/201010/28/0_1288270747BZ3t.gif)

图  透明度的连续变化

5）GDI+帮助文档

GDI+的英文帮助内容，位于VS2008的“目录/Win32和COM开发/Graphics and Multimedia/ GDI+”，主要的参考资料位于其子目录“GDI+ Reference”中。

GDI+的中文帮助内容位于VS2008的“目录/开发工具和语言/Visual Studio文档/基于Windows的应用程序、组件和服务/创建基于 Windows 的应用程序/Windows窗体（Windows Forms）/增强Windows窗体应用程序/Windows窗体中的图形和绘制”，其中包括“图形概述（Windows 窗体）”、“关于 GDI+ 托管代码”和“使用托管图形类”三个子目录。

6）Graphics与GraphicsPath中心点

Graphics中心点是左上点，而GraphicsPath中心点是真正的中心点。如图：

![](http://hi.csdn.net/attachment/201010/28/0_1288270755U76O.gif)

GraphicsPath类的GetBounds等函数得到的点，是转换到Graphics平面上的点，对GraphicsPath进行矩阵操作时，需要就将点坐标平移到GraphicsPath中心。

例如：（大小渐变文字）

**GraphicsPath** path; // 定义路径对象

    path.AddString(L"大小渐变文字测试", -1, // 将文本串加入路径

&**FontFamily**(L"隶书"), FontStyleRegular, 100, Point(0, 0), NULL);

    RectF boundRect;

    path.Get**Bounds**(&boundRect);// 获取路径的界限矩形

**Matrix** M; // 定义矩阵对象（单位阵）

    M.Translate(-(boundRect.X + boundRect.Width / 2),

        -(boundRect.Y + boundRect.Height / 2)); // 平移**原点**到文本路径的中心

    path.Transform(&M); // 更改路径的中心点

    INT n = path.GetPoint**Count**(); //获取路径中的**点数**

    PointF *points = new PointF[n]; // 动态创建点数组

    path.GetPath**Points**(points, n); // 获取路径的点数组

**BYTE ***types =**new** BYTE[n]; // 动态创建类型数组

    path.GetPath**Types**(types, n); //获取**路径类型**数组（用于路径重构）

    for (int i= 0; i < n; i++) // 根据路径点到中心的距离,按比例修改点的y值

points[i].Y *= 2*(boundRect.Width-abs(points[i].X))/boundRect.Width;

**GraphicsPath** newPath(points, types, n); // 用**新的路径点**构造**新路径**

**CRect **crect;

Get**Client**Rect(**&**crect); // CRect

    Graphics graph(pDC->m_hDC); // 将坐标原点移到窗口中心：

    graph.**Translate**Transform(REAL(crect.Width()/2),REAL(crect.Height()/2));

    graph.**Fill**Path(&SolidBrush(Color::Green), &newPath);//填充路径（绘制文本串）

**delete points; **

**delete types;**

7）在保存图象的时候会发生这样的错误：“GDI+发生一般性错误”，一般解决方法参考下面。

“GDI+发生一般性错误”，这样的错误一般可以这样重现：

[Image](http://www.mydown.com/soft/102/102347.html) image = new Bitmap(openFileDialog1 .[FileName](http://www.mydown.com/soft/utilitie/systems/198/433698.shtml) );

　　image.Save(openFileDialog1 .FileName ,System .Drawing .Imaging .ImageFormat .Jpeg );

　　发生这个错误的原因是：

　　从一个文件构造的Bitmap 对象或一个 Image 对象, 在该对象的生存期内该文件处于锁定状态。 因此, 在没有释放这个Image或Bitmap对象前，无法更改图像并将其保存回原文件。

　　解决方法：

　　构造一个新的Image对象，然后把原来的Image对象中的图象通过Graphics的DrawImage()方法，拷贝到新Image对象中，最后通过Dispose()方法释放原来的Image对象：

　　Image image = new Bitmap ( openFileDialog1 . FileName );

　　//新建第二个Image类型的变量newImage，这里是根据程序需要设置自己设置。

　　Image newImage = new Bitmap ( 800 , 600 );

　　//将第一个bmp拷贝到bmp2中

　　Graphics draw = Graphics . FromImage ( newImage);

　　draw . DrawImage ( image , 0 , 0 );

　　//释放第一个Image对象

　　image.Dispose();

　　//保存图象

　　newImage.Save(openFileDialog1.FileName);

8) GDI+ 缩放图片的方法

方法一 : 最简单的 , 使用 GetThumbnailImage, 这个方法的局限性对支持内嵌缩略图的图片文件无效 . 因为 MSDN 中提到 : 如果图片文件有内嵌的缩略图 , 那么就提取这个缩略图返回 , 否则就缩放原图片 , 不过我想对位图还是安全的 :

Bitmap  * image = new Bitmap(L"MagicLinux.bmp");

Image* pScaledImage = NULL;

UINT nWidth = image->GetWidth()/2;

UINT nHeight= image->GetHeight()/2;

pScaledImage = image->GetThumbnailImage(nWidth, nHeight, NULL, NULL);

delete pScaledImage;

delete image;

方法二 :使用 Graphics::DrawImage, 这样还可以控制 InterpolationMode( 插值模式 , 在缩放和旋转时候使用 ), 即可以控制缩放质量高低 . 也可以通过 Graphics 对象做其他的控制 .

Bitmap * ScaleBitmap(Bitmap * pBitmap,UINT nWidth,UINT nHeight)

{

Bitmap * pTemp = new Bitmap(nWidth,nHeight,pBitmap->GetPixelFormat());

if( pTemp )

{

Graphics * g = Graphics::FromImage(pTemp);

if( g )

{

// use the best interpolation mode 

g->SetInterpolationMode(InterpolationModeHighQualityBicubic);

g->DrawImage(pBitmap,0,0,nWidth,nHeight);

delete g;

}

}

return pTemp;

}

pScaledImage = ScaleBitmap(image,nWidth,nHeight); 

保存图片的代码 : 

bool SaveAsImageFile(Image * pImage,LPCWSTR lpszFileName,LPCWSTR lpszImageType)

{

UINT  num = 0;

// number of image encoders

UINT  size = 0;

// size of the image encoder array in bytes

ImageCodecInfo* pImageCodecInfo = NULL;

if(GetImageEncodersSize(&num, &size)!= Ok || size == 0 )

return false;  // Failure

pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

if(pImageCodecInfo == NULL)

return false;  // Failure

if(GetImageEncoders(num, size, pImageCodecInfo)!= Ok )

return false;

bool bOk = false;

for(UINT j = 0; j < num; ++j)

{

if( wcscmp(pImageCodecInfo[j].MimeType, lpszImageType) == 0 )

{

pImage->Save(lpszFileName,&(pImageCodecInfo[j].Clsid));

bOk = true;

break;

}

}

free(pImageCodecInfo);

return bOk;

}

调用象这样就可以保存成各种图片 ( 当然要求有对应的 encoder 才行 ): 

SaveAsImageFile (pScaledImage,L"1.bmp",L"image/bmp");

SaveAsImageFile (pScaledImage,L"1.jpg",L"image/jpeg");

SaveAsImageFile (pScaledImage,L"1.png",L"image/png");

9）GetPixel/SetPixel速度慢问题

GDI+的Bitmap类提供了两个罪恶的函数GetPixel, SetPixel，用来获取某个像素点的颜色值。这个2个函数如果只调用一次两次也就罢了，如果调用多次速度就很慢了，可以使用LockBits方法,就是把图像的内存区域根据格式锁定，拿到那块内存的首地址，通过直接操作内存。比如将图象灰度化：

intGetEncoderClsid(constWCHAR*format,CLSID*
pClsid)

{

UINTnum = 0;         // number of image encoders

UINTsize = 0;        // size of the image encoder array
 in bytes

ImageCodecInfo*pImageCodecInfo =NULL;

GetImageEncodersSize(&num, &size);

if(size == 0)

return -1; // Failure

pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

if(pImageCodecInfo ==NULL)

return -1; // Failure

GetImageEncoders(num,size,pImageCodecInfo);

for(UINTj = 0;j
 < num; ++j)

 {

if(wcscmp(pImageCodecInfo[j].MimeType,format)
 == 0 )

 {

 *pClsid =pImageCodecInfo[j].Clsid;

free(pImageCodecInfo);

returnj; // Success

 }    

 }

free(pImageCodecInfo);

return -1; // Failure

}

//图象灰度处理

voidCGDIPlusTestView::OnImageGray()

{   Graphicsg(this->m_hWnd);

BitmapimLoad(_T("c://0250001207A.bmp"));

longnImageWidth =imLoad.GetWidth();

longnImageHeight =imLoad.GetHeight();

RectrectDest(0,0,400,400);

g.DrawImage(&imLoad,rectDest,0,0,nImageWidth,nImageHeight,UnitPixel,NULL,NULL,NULL);

Bitmap *pGrayImg =newBitmap(nImageWidth,nImageHeight,PixelFormat8bppIndexed);

//生成调色版

ColorPalette*pal = (ColorPalette*)malloc(sizeof(ColorPalette)
 + 256*sizeof(ARGB));

pal->Count = 256;

pal->Flags = 0;

for(intm=0;m<256;m++)

 {

pal->Entries[m]
 = Color::MakeARGB(255,m,m,m);

 }

pGrayImg->SetPalette(pal);

BitmapDatabmGrayData;

pGrayImg->LockBits(newRect(0,0,nImageWidth,nImageHeight),ImageLockModeWrite,PixelFormat8bppIndexed,&bmGrayData);

BitmapDatabmData;

StatusiSucess =imLoad.LockBits(newRect(0,0,nImageWidth,nImageHeight),ImageLockModeRead,PixelFormat24bppRGB,&bmData);

BYTE *p = (BYTE*)bmData.Scan0;

BYTE *q = (BYTE*)bmGrayData.Scan0;

BYTE *pt =p, *qt
 =q;

inti,j;

BYTEval;

for (j = 0;j <nImageHeight;j++)

 {

// Stride是指图像每一行需要占用的字节数。根据BMP格式的标准，Stride一定要是4的倍数。

pt =p +j *bmData.Stride;

qt =q +j *bmGrayData.Stride;

for (i = 0;i <nImageWidth;i++)

 {

val = (*pt)*0.114 + (*(pt+1))*0.587
 + (*(pt+2))*0.299;

if(val>255)

 {

val = 255;

     }

if(val<0)

 {

val = 0;

     }

 *qt =val;

pt += 3;

qt += 1;

 }

 }

imLoad.UnlockBits(&bmData);

pGrayImg->UnlockBits(&bmGrayData);

CLSIDcidImage;

GetEncoderClsid(L"image/tiff", &cidImage);

//保存图像

pGrayImg->Save(L"C://Gray.tif",&cidImage,NULL);

if(pal!=NULL)

 {

free(pal);

pal =NULL;

 }

//显示图像

rectDest.X = 401;

rectDest.Y = 0;

g.DrawImage(pGrayImg,rectDest,0,0,nImageWidth,nImageHeight,UnitPixel,NULL,NULL,NULL);

if(pGrayImg!=NULL)

 {

delete []pGrayImg;

pGrayImg =NULL;

 }

}

10)透明，半透明和不透明

在WinForm/WPF里面我们经常会看到一些关于透明的属性，比如Backcolor里面可以选择Transparant, Form里面有一个叫Opacity的属性。都是和透明以及透明度相关的。在其实是在GDI+应用层上的一些东西，在这里我就不讲了。主要从更基本的地方讲起，其中还包括两块完全不同的内容。在LockBits的时候把PixelFormat设定成为Format24bppRgb。但是如果你仔细研究，会发现其实里面有各种各样的图片格式，其中有一种叫做Format32bppArgb。这个意思是说除了RGB，在图像中还存在一个通道，叫做A。这个A就是用来描述当前像素是透明，半透明，还是全透明的分量。这个通道是2个叫Catmull和Smith在上世纪70年代初发明的。通过这个分量，我们可以进行alpha混合的一些计算。从而使表面的图像和背景图像混合，从而造成透明半透明的效果。在这种格式下A作为一个byte，取值可以从0到255，那么0表示图像完全透明，则完全不可见，255则表示图像完全不透明。每个像素都可以实现这种透明或者半透明的效果。更详细解释可以参考http://en.wikipedia.org/wiki/Alpha_compositing，或者去买本数字图像处理的书回来看。让我们来看看下面这段代码，这个函数可以把图像变成半透明的。

public unsafe Bitmap GenerateBitmap(byte alpha)

{

FileStream fs = new FileStream(image, FileMode.Open, FileAccess.Read);

Image img = Image.FromStream(fs, false, false);

Bitmap bmp = new Bitmap(img);

img.Dispose();

fs.Close();

int width = bmp.Width;

int height = bmp.Height;

BitmapData bmData = bmp.LockBits(

new Rectangle(0, 0, width, height),

ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);

byte* p = (byte*)bmData.Scan0;

int offset = bmData.Stride - width * 4;

for (int j = 0; j < height; j++)

{

for (int i = 0; i < width; i++)

{

p[3] = alpha;

p += 4;

}

     p += offset;

}

bmp.UnlockBits(bmData);

return bmp;

}

大家可以注意一下第17，22和23行，由于图像格式不对了，所以我们计算offset和递加的操作都该了，此外由于用小数端存储方式，Alpha通道在p[3]的位置。

顺便提一句，还有一种格式叫做Format32bppPArgb，这叫做premultiplied alpha，就是说在RGB分量里面，alpha分量的数据已经被预先乘进去了。比如说，一个半透明的红色点，在ARGB下，矢量是(255,0,0,128),而在PARGB下就变成了(128,0,0,128)。这是为了不要每次都做乘法。

还有要注意的是，如果你想把这个Bitmap保存成为一个文件，那么必须用png格式，才能够保存alpha通道的信息。如果你存为JPG/BMP/GIF,那么alpha通道的信息将会被丢失。如果存为BMP,那么文件格式将变成Format32bppRgb，其中1个字节不再使用；如果保存为JPEG，那么是Format24bppRgb；存为GIF，格式将变成Format8bppIndexed。根据标准，BMP/JPG本来就不支持透明通道，所以没有可能保留透明信息。GIF倒是支持透明，但是GIF中颜色的信息都是索引，所以Alpha的解释对GIF完全没有效果，接下去我们来分析怎么样使用GIF的透明。

11) 一个不错的 GDI+ FAQ site :[http://www.bobpowell.net/faqmain.htm](http://www.bobpowell.net/faqmain.htm)

[](http://www.bobpowell.net/faqmain.htm)



