# 十全大补:CxImage图像处理类库 - 深之JohnChen的专栏 - CSDN博客

2009年04月10日 09:15:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：5953


** 十全大补:CxImage图像处理类库**

                                                                                                                                   转载IT168 

CxImage是一个可以用于MFC 的C++图像处理类库类，它可以打开，保存，显示，转换各种常见格式的图像文件，比如BMP, JPEG, GIF, PNG, TIFF, MNG, ICO, PCX, TGA, WMF, WBMP, JBG, J2K 等格式的文件。可以实现BMP<->JPG，PNG <>TIFF格式等等的互相转换。

  CxImage不仅可以实现图像文件的类型转换，还可以实现在内存图像数据的类型转换，并且使用很方便。除了文件格式的相互转换，它还提供了很多很多图像后处理的功能，比如图像模糊，锐化等等，功能非常强大和实用。

  CxImage中的x，大约代表了丰富的图像格式支持和丰富的图像处理功能，可以说CxImage是图像处理的十全大补汤。

![](http://image4.it168.com/2009/3/6/38a41412-131f-4af4-b1b2-535da0bb8a10.jpg)
  用CxImage实现的图像处理程序

**  开发环境：**
  VC6, C++, Windows, MFC, Visual Studio, VS6, Dev

**  简介和许可**
  CxImaage是一个可以简便而快速地打开，保存，显示和转换图像文件的C++类库。CxImage类库是免费的。对于TIFF，JPEG，PNG和ZLIB，"如果你在你的产品中使用了这些源代码，虽然致谢不是必须的，但是却会得到众人的欣赏。"

  CxImage是开源的并在zlib许可下发布。简单地说，你可以随意地使用这些代码，只要你不说它是你自己的就行了。

**  文件格式和链接的库 **

  CxImage支持如下这些库的最新版本：
   Zlib (1.2.3), Jasper ( 1.900.1), LibMNG (1.0.10), LibPNG (1.2.24). LibTIFF 3.8.2 
j2k库（现在被称为openjpeg）和相关的类CxImageJ2K已经从项目中删除。JPEG2000格式的文件由Jasper和CximageJAS来支持。

  Cximage 6.00提供了一个新的类CximageRAW和新的库LibDCR，用于读取来自[数码相机](http://product.it168.com/files/0305search.shtml)的RAW图像。常见的文件扩展名有：RAW, CRW, NEF, CR2, DNG, ORF, ARW, ERF, 3FR, DCR, X3F, MEF, RAF, MRW, PEF, SR2。

  CximageRAW提供了对图像的基本解码功能，它唯一的可选参数可以由SetCodecOption进行设置，以对插值质量进行可控制。可选的选项有(DECODE_QUALITY_LIN=0, DECODE_QUALITY_VNG=1, DECODE_QUALITY_PPG=2, DECODE_QUALITY_AHD=3).

  CximagePNG:现在可以读取所有PNG_COLOR_TYPE_类型。但是，每个通道含有超过8位像素深度的PNG文件，会被转换到8位，这是CxImage的一个主要限制。
  CxImageGIF:现在能够更好地支持对动态GIF图像的读取，通过SetRetreiveAllFrames设置属性，可以同时对图像中的所有帧进行解码。在CxImage的示例项目中，展示了如何实现这一选项，并展示了如何播放一副GIF动画。

  CxImageBMP: 支持对32位图像的读写(支持Alpha通道)

  CxImageICO: 支持对Vista PNG图标的读写；添加对多页图标的支持。

  CxImageMNG: 支持对MNG Alpha图层的读取。

  CxImageSKA: 新的用于支持SKA图像格式的类，这种图像通常用在[视频](http://movie.it168.com/)租赁[软件](http://software.it168.com/)中。

  CxImageJPG: 为JPEG图像格式的二次采样提供了 新的选项。ENCODE_SUBSAMPLE_422，ENCODE_SUBSAMPLE_444)，默认情况下是 高采样率的4:1:1 。当然，也可以被设置成中等的4:2:2或者低等的4:4:4。

下图展示了不同采样率下，在压缩图像中所产生的不同数量的伪影。在压缩有着明显轮廓的图像的时候，采用4:4:4的采样率有助于减少轮廓的伪影。 

![](http://image4.it168.com/2009/3/6/f562043a-c33e-4cbc-8404-12a607fcfd40.jpg)
![](http://image4.it168.com/2009/3/6/e6b516d8-904c-44c1-aadf-0eecd7d93005.jpg)

**  可移植性**
  所有的类和项目都通过了不同编译器的测试，包括从VC++6 到VC++2008的微软系列编译器，Borland C++ Builder 3和6，同时部分地支持wxDev-C++和MinGW。

  所有的库都支持UNICODE 和非UNICODE两个版本。（感谢Eric Jesover）

  对于第一次使用的用户，你可以使用一个轻量级的版本(cximage600_lite)。这个版本去除了C库，但是添加了一个简单的示例项目。
  所有控制台示例(/demo2)和CxImageCrtDll项目可以在VC++Express2005和 Microsoft Platform SDK环境下编译通过。如果你在编译链接的时候，遇到像"unresolved external..."(无法找到外部符号…)这样的错误，请检查是否所有C库都被编译了。或者手动地添加gdi32.lib 和user32.lib为项目的依赖库。

  同时，CxImage也可以在Pocket PC 2003上工作。在CxImage中，为VC++ 2005编译器提供了一个可以工作的版本和相应的实例(cximage600_ce)。对于旧式的嵌入式VC编译器，最主要的限制是无法支持对异常的处理。为了解决这个问题，相应的异常处理语句try，throw和catch已经被定义在ximadef.h中的三个宏取代，同时还定义了CXIMAGE_SUPPORT_EXCEPTION_HANDLING。通过这样的方法，取消对异常处理的支持，可以成功编译链接整个库。虽然这个解决办法不是那么优雅，但是当异常处理被取消的时候，对代码的影响是最小的。同时，当异常处理被启用的时候，同样不会造成源代码的修改与变动。

  对于平台之间little-endian和big-endian的兼容性，对于内建支持的图像文件格式(bmp, ico, tga, pcx, gif, ska)，由ntohs和ntohl控制。

**  示例**

  大多数新的特性可以在CxImage的主要示例程序中尝试和体验。这个示例程序只是一个测试平台，虽然它提供很多有用的特性，但是它实在不是一个严谨的应用程序。
  ·CQuantizer : 这个类被DecreaseBpp菜单项所使用。在以前的版本中，这里有一个取整的错误，在某些情况下，白色(255,255,255)会被转换成(254,254,254)。在新的版本中，这个问题已经被修复了。 
  ·Copy/Paste : 现在，新的粘贴功能支持元文件图像（例如，从Office应用程序中复制而来的图像）。这个示例内部使用了自定义的剪切板格式以测试Dump/Undump方法。Copy只工作在当前选区，但是你可以通过CxImage/Remove Selection 复制整幅图像。
  ·FloodFill : (/View/Tools/Flood Fill) 通过一个浮动的对话框，你可以测试色彩填充的颜色，偏差，透明度和是否选择。当透明度为0并且选择区域功能启用的时候，这个功能就变现为"魔法棒"，用于根据颜色选区某个区域。 
  ·Graph data extraction : (/Filters/graph data extraction) 这个示例演示了如何从图像（从[扫描仪](http://product.it168.com/files/0616search.shtml)获取或者从互联网下载所得）中提取数字信息。转换后的数据被粘贴到剪切板，同时可以被保存成文本文件或者Excel表格文件。

![](http://image4.it168.com/2009/3/6/89a0eab4-d90f-474d-87a6-e6f7927729bc.jpg)
数据提取对话框

![](http://image4.it168.com/2009/3/6/89e76d1a-c7f3-48c1-9f4b-b3146830812e.jpg)
数据提取结果

  ·RedEyeRemove : (/Filters/Non Linear/Remove Red Eye) 去除红眼的功能在照片处理中经常用到。你 必须在红眼周围选择一个区域，然后去除红眼的功能就会对红色通道进行过滤，从而去掉照片中的红眼。红眼的选区是一个矩形区域，同时可以包括部分的虹膜(眼黑)，过滤器会在以选区中点为中心的圆心区域工作，不会影响到选中的眼黑部分。

![](http://image4.it168.com/2009/3/6/249d72b9-4d5a-42a6-9f4a-3f1f34e3a7c0.jpg)

  ·SelectiveBlur / UnsharpMask : 这些非线性的滤镜可以增加图像的质量。SelectiveBlur可以去除二次噪声（比如JPEG图像中的伪影或者是[数码相机](http://product.it168.com/files/0305search.shtml)的噪声），从而展示图像中的更多细节。而UnsharpMask可以增强图像的细节，但是却不会添加噪声。

·Custom linear filters : (/Filters/Linear/Custom) 一个用于测试新的滤镜功能核心的图像用户界面。

  ·Histogram : (/Colors/Histogram/...) 为了测试HistogramStretch(直方图)，这个示例中提供了很多菜单项，通过不同的方法(0 = luminance, 1 = linked channels , 2 = independent channels)来测试直方图功能。对于有噪声的图像，阈值(threshold)这个参数增强了算法的健壮性。半饱和度(Half Saturation)和全饱和度(Full Saturation)可以测试转换颜色空间(ConvertColorSpace)，直方图(Histogram)和饱和度(Saturate)在YUV颜色空间对直方图的拉伸效果。

![](http://image4.it168.com/2009/3/6/9603d0d0-988e-4d96-a610-9c03cf16a91c.jpg)

  ·Thresholding : ( /Colors/Threshold... and /Colors/Adaptive Threshold). 透明度阈值(OptimalThreshold)是一个新的用于查找二进制图像透明度阈值的新方法。可选的算法有：
  1 = 最大类间方差 (Otsu);
  2 = 基特勒和伊尔林格沃斯(Kittler & Illingworth);
  3 = 最大熵(maximum entropy);
  4 = 位差(potential difference);
  0 = 平均所有方法 (默认情况下，这也是示例程序中所使用的方法);

  "保留低于阈值的颜色"(preserve colors less than the threshold)的选项，将测试Threshold2方法。这对于过滤带有噪声背景的彩色图像很有用。经过处理，我们将得到一幅带有一致背景的彩色图像(噪声被去除)。AdaptiveThreshold是对方法OptimalThreshold的一个应用，它将创建很多阈值模板。AdaptiveThreshold对于有着不一致的灯光照射效果的图像，非常有用。对于这类图像，我们不能简单地在整幅图像中使用某个单一的阈值。但是，如果我们的参数设置得不对，我们将得到一个非常差劲的处理结果。

![](http://image4.it168.com/2009/3/6/ee5425ee-7bad-41ba-b1e7-998ed02bd93b.jpg)

  ·Add shadow : (/Filters/Add Shadow...) 这个菜单项的功能演示了如何将CxImage的多个小功能(选择(selections)，高斯模糊( GaussianBlur)，混合( Mix))组合使用，以达到常见的图像处理效果。 

![](http://image4.it168.com/2009/3/6/c520b8fe-1ef5-44d3-a41a-138d95da6b4c.jpg)

  ·Text smoothing : DrawStringEx 实现了一个新的选项CXTEXTINFO::smooth。我们可以通过文本工具(位于/View/Tools/Text),，选中抗锯齿("antialias")选项对这个功能进行测试。

  类似的效果也同样可以利用TextBlur (/Filters/Non Linear/Text Blur)进行后处理获得。这是一个非线性的过滤器，它只对角或圆形的边缘起作用，从而不会影响到纵向或横向的线。下图展示了不同平滑方法的实际效果：

![](http://image4.it168.com/2009/3/6/c891c1bb-e733-4605-ac14-43d068a687e3.jpg)

**CxImage库的结构**

  在整个库的继承树中，CxImage位于所有其他模块的顶部。这不是一个完全的符合OOP的方式，但是至少从最初的版本一直到现在，它都工作得很好。现在想要改变整个结构，已经太晚了。但是，你总是可以使用这些继承类来操作相应格式的图像，例如你可以使用CxImageTIF来保存多页的TIFF文件。这样显得非常直观。

  连接所有模块和C库的是CxFile。这是一个虚类，它提供了标准的方法，用于访问[硬盘](http://product.it168.com/list/b/0218_1.shtml)或者[内存](http://product.it168.com/list/b/0205_1.shtml)的文件数据。 

**CxImage的继承树**  一个CxImage对象基本上对应于一副位图，同时添加了一些额外的成员变量用于[存储](http://storage.it168.com/)一些有用的信息。

class CxImage
  {
  ...
protected:
void* pDib;            //包含文件头，调色板和像素数据
  BITMAPINFOHEADER head; //标准文件头
  CXIMAGEINFO info;      //扩展信息
  BYTE* pSelection;      //选区
  BYTE* pAlpha;          //alpha通道
  CxImage** ppLayers;     //普通层
  CxImage** ppFrames;     //动画的帧
  }
CxImage::head 是位图文件的文件头，而CxImage::pDib就是一个普通的位图（就像你在CxImageBMP::Encode中看到的一样）。  
CxImage::info 是一个方便的信息容器。这些信息被不同的文件格式所共享，同时供所有成员函数访问。
typedef struct tagCxImageInfo {
    DWORD   dwEffWidth;       //双字节对齐宽度
    BYTE*   pImage;           //图像数据
void*   pGhost;           //如果这是一个备份(ghost)，
//则pGhost指向它的原始对象 
    DWORD   dwType;           //原始图像格式
char    szLastError[256]; //调试信息，最后的错误信息
long    nProgress;        //进度
long    nEscape;          //取消
long    nBkgndIndex;      //GIF, PNG, MNG使用
    RGBQUAD nBkgndColor;      //RGB透明使用
    BYTE    nQuality;         //JPEG使用
long    nFrame;           //TIF, GIF, MNG使用，表示当前活动帧
long    nNumFrames;       //TIF, GIF, MNG使用，表示总帧数                        
    DWORD   dwFrameDelay;     //GIF, MNG使用，表示帧的延迟
long    xDPI;             //水平分辨率  
long    yDPI;             //垂直分辨率  
    RECT    rSelectionBox;    //对象的外包围框
    BYTE    nAlphaMax;        //最大透明度
bool    bAlphaPaletteEnabled;  //如果调色板中的alpha值是启用的，
//则这个变量为true 
bool    bEnabled;         //启用绘制函数
long    xOffset;
long    yOffset;
DWORD   dwEncodeOption;   //GIF, TIF使用:
// 0=def.1=unc,2=fax3,3=fax4,
// 4=pack,5=jpg
    RGBQUAD last_c;           //用于优化GetNearestIndex
    BYTE    last_c_index;
bool    last_c_isvalid;
long    nNumLayers;
    DWORD   dwFlags;
} CXIMAGEINFO;

**CxImage的不同层**
  CxImage对象同样是图层的集合，每个图层的缓冲区只在需要的时候申请。
  CxImage::pDib是背景图像。 CxImage::pAlpha是透明层。CxImage::pSelection是选区层，用于创建要对图像的感兴趣区域进行处理的选择区域。在这三个特殊层之上，你可以添加通用的其他层，这些层都保存在CxImage::ppLayers中。通用层也是完整的CxImage对象，所以你可以创建复杂的嵌套层。CxImage::ppFrames 为动态图像(GIF)所预留。 

**  CxImage类成员和操作**
  CxImage使用Doxygen 产生文档，但是由于一些历史的原因，很多不太通用的功能还没有文档。类成员的参考资料，发布历史以及许可信息都可以在这里(here )找到。

**  支持的格式和选项**
  整个CxImage库非常大，在主要的头文件ximcfg.h中，你可以找到很多编译选项开关，用于启用或者禁用某些特定的图像格式或者功能。每个JPG, PNG和 TIFF库将使得应用程序增加大约100KB的大小，同时，CxImage将使得应用程序增加大约50KB的大小。所以，为了减小你的应用程序的体积，你应该支持和链接你的应用程序确实需要的格式。

|格式|定义#define|需要的库|尺寸 [KB]|
|----|----|----|----|
|BMPGIFICOTGAPCXWBMPWMFSKA|CXIMAGE_SUPPORT_BMPCXIMAGE_SUPPORT_GIFCXIMAGE_SUPPORT_ICOCXIMAGE_SUPPORT_TGACXIMAGE_SUPPORT_PCXCXIMAGE_SUPPORT_WBMPCXIMAGE_SUPPORT_WMFCXIMAGE_SUPPORT_SKA|内建|24|
|JPEG|CXIMAGE_SUPPORT_JPG|jpeg|88|
|PNG|CXIMAGE_SUPPORT_PNG|png, zlib|104|
|MNG|CXIMAGE_SUPPORT_MNG|mng, zlib, jpeg|148|
|TIFF|CXIMAGE_SUPPORT_TIF|tiff, zlib, jpeg|124|
|JBIG|CXIMAGE_SUPPORT_JBG|jbig|28|
|PNM,PPM,PGMRAS|CXIMAGE_SUPPORT_PNMCXIMAGE_SUPPORT_RAS|jasper|176|
|JPEG-2000|CXIMAGE_SUPPORT_JP2CXIMAGE_SUPPORT_JPCCXIMAGE_SUPPORT_PGX|jasper|176|
|RAW|CXIMAGE_SUPPORT_RAW|libdcr|132|

|选项|定义#define|尺寸[KB]|
|----|----|----|
|CxImage核心|所有开关关闭|20|
|几何变换(geometric transformations)|CXIMAGE_SUPPORT_TRANSFORMATION|16|
|图像处理(image processing)|CXIMAGE_SUPPORT_DSP|24|
|绘制和特殊的窗口函数(drawing and windows specific functions)|CXIMAGE_SUPPORT_WINDOWS|12|
|透明(transparency)|CXIMAGE_SUPPORT_ALPHA|4|
|选择(selections)|CXIMAGE_SUPPORT_SELECTION|4|
|多层支持(multiple layers)|CXIMAGE_SUPPORT_LAYERS|< 4|
|图像格式转换(graphic formats conversion)|CXIMAGE_SUPPORT_DECODECXIMAGE_SUPPORT_ENCODE|< 4|
|插值函数(interpolation functions)|CXIMAGE_SUPPORT_INTERPOLATION|< 4|
|异常处理(exception handling)|CXIMAGE_SUPPORT_EXCEPTION_HANDLING|< 4|

**在你的项目中使用CxImage**如下图所示，CxImgLib.dsw工作空间展示了构建一个包含绝大多数功能和支持大多数图像格式的应用程序(demo.exe)所需要的所有库。你必须先编译所有的库，然后才可以链接你最终的应用程序。

**CxImgLib.dsw工作空间**  在相同的工作空间中，你可以找到用于构建不同库和应用程序的项目： 
  ·CxImage : cximage.lib - 静态库 
  ·CxImageCrtDll : cximagecrt.dll - DLL，不使用MFC 
  ·CxImageMfcDll : cximage.dll - DLL，使用MFC
  ·Demo : 示例程序demo.exe，跟cximage.lib和C库链接。 
  ·DemoDll : demodll.exe - 跟cximagecrt.dll链接 
  ·libdcr,jasper,jbig,jpeg,png,tiff,zlib : 静态的C库 
  构建这些项目需要几分钟时间，当所有项目构建完成后，你可以选择demo项目并执行应用程序。

  要想在你自己的项目中使用CxImage，你必须编辑下面这些项目设置设置(针对Visual C++ 6.0)：

  Project Settings(项目设置)
   |- C/C++
   |   |- Code Generation
   |   |   |- 运行时库 : 多线程DLL(所有链接的库都必须使用相同的设置) 
   |   |   |- 结构成员对齐方式 : 所有链接的库都必须相同 
   |   |- 预编译头文件 : 不使用预编译头文件
   |   |- 预处理器： 
   |       |- 添加额外的包含(Include)路径:  ../cximage
   |- Link
    |- General
        |- Object/library modules(附加的依赖库): ../png/Debug/png.lib  
                                   ../raw/Debug/libdcr.lib
                                   ../jpeg/Debug/jpeg.lib 
                                   ../zlib/Debug/zlib.lib 
                                   ../tiff/Debug/tiff.lib
                                   ../jasper/Debug/jasper.lib
                                   ../cximage/Debug/cximage.lib  ...

  在你的源代码中，你必须引入CxImage的头文件ximage.h。在你的代码中添加如下语句：
#include "ximage.h"。
  注意，不要混合debug和release模块，每个配置必须使用各自相应的库文件。

**  在CxImage中添加自定义的函数 **  为CxImage添加一个新的图像处理函数并不困难。在这里，我将以CxImage::Jitter为例子，描述如何为CxImage添加自定义的函数。虽然这个函数很简单， 但是它却展示了使用CxImage时所需要注意的方方面面。

  首先，我们需要声明这个函数：

bool Jitter(long radius=2)

  在ximage.h头文件的CXIMAGE_SUPPORT_DSP部分，你可以在public区域的任何部分声明这个函数。

  现在，我们开始定义这个函数：

bool CxImage::Jitter(long radius)
{
// 检查图像是否合法，这应当是这个函数的第一行
if (!pDib) returnfalse;

// 局部变量
long nx,ny;

// 临时图像，用于[存储](http://storage.it168.com/)算法的部分结果
    CxImage tmp(*this,pSelection!=0,true,true);

// 限制函数仅仅作用在选区（通过Selection...()函数定义）的最小区域
// 这将加快整个循环的速度，提高算法效率
long xmin,xmax,ymin,ymax;
if (pSelection){
        xmin = info.rSelectionBox.left; xmax = info.rSelectionBox.right;
        ymin = info.rSelectionBox.bottom; ymax = info.rSelectionBox.top;
    } else {
        xmin = ymin =0;
        xmax = head.biWidth; ymax=head.biHeight;
    }

// 主循环 : 垂直方向扫描图像
for(long y=ymin; y <ymax; y++){

// 监视循环的进度
        info.nProgress = (long)(100*y/head.biHeight);

// 检查应用程序是否已经退出
if (info.nEscape) break;

// 主循环 : 水平方向扫描图像
for(long x=xmin; x<xmax; x++){

// 如果选区功能启用了，则仅仅处理选区内部的像素
#if CXIMAGE_SUPPORT_SELECTION
if (SelectionIsInside(x,y))
#endif//CXIMAGE_SUPPORT_SELECTION
            {
// 主算法
                nx=x+(long)((rand()/(float)RAND_MAX -0.5)*(radius*2));
                ny=y+(long)((rand()/(float)RAND_MAX -0.5)*(radius*2));
if (!IsInside(nx,ny)) {
                    nx=x;
                    ny=y;
                }

// 保存结果到临时图像中.
// 如果可以，24位图像请使用PixelColor，
// 而8，4和1位图像请使用PixelIndex，这样可以加快速度。
if (head.biClrUsed==0){
                    tmp.SetPixelColor(x,y,GetPixelColor(nx,ny));
                } else {
                    tmp.SetPixelIndex(x,y,GetPixelIndex(nx,ny));
                }

// 如果启用了透明度功能，则处理透明图层中的像素 
#if CXIMAGE_SUPPORT_ALPHA
                tmp.AlphaSet(x,y,AlphaGet(nx,ny));
#endif//CXIMAGE_SUPPORT_ALPHA

            }
        }
    }

// 保存结果并退出
    Transfer(tmp);
returntrue;
}

**示例: 如何转换一种格式到另外一种格式 **

CxImage  image;
// bmp -> jpg
image.Load("image.bmp", CXIMAGE_FORMAT_BMP);
if (image.IsValid()){
if(!image.IsGrayScale()) image.IncreaseBpp(24);
    image.SetJpegQuality(80);
    image.Save("image.jpg",CXIMAGE_FORMAT_JPG);
}
// png -> tif
image.Load("image.png", CXIMAGE_FORMAT_PNG);
if (image.IsValid()){
    image.Save("image.tif",CXIMAGE_FORMAT_TIF);
}

**  如何从资源中加载图像**

//Load the resource IDR_PNG1 from the PNG resource type
CxImage* newImage =new CxImage();
newImage->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_PNG1),
"PNG"),CXIMAGE_FORMAT_PNG);
或者
//Load the resource IDR_JPG1 from DLL
CxImage* newImage =new CxImage();
HINSTANCE hdll=LoadLibrary("imagelib.dll");
if (hdll){
    HRSRC hres=FindResource(hdll,MAKEINTRESOURCE(IDR_JPG1),"JPG");
    newImage->LoadResource(hres,CXIMAGE_FORMAT_JPG,hdll);
    FreeLibrary(hdll);
}
或者
//Load a bitmap resource;
HBITMAP bitmap = ::LoadBitmap(AfxGetInstanceHandle(),
                           MAKEINTRESOURCE(IDB_BITMAP1)));
CxImage *newImage =new CxImage();
newImage->CreateFromHBITMAP(bitmap);

**  如何解码[内存](http://product.it168.com/list/b/0205_1.shtml)中的图像 **

CxImage image((BYTE*)buffer,size,image_type);
或者
CxMemFile memfile((BYTE*)buffer,size);
CxImage image(&memfile,image_type);
或者
CxMemFile memfile((BYTE*)buffer,size);
CxImage* image =new CxImage();
image->Decode(&memfile,type);

**  如何对内存中的图像编码 **

long size=0;
BYTE* buffer=0;
image.Encode(buffer,size,image_type);
...
image.FreeMemory(buffer);
或者
CxMemFile memfile;
memfile.Open();
image.Encode(&memfile,image_type);
BYTE* buffer = memfile.GetBuffer();
long size = memfile.Size();
...
image.FreeMemory(buffer);

**  如何创建一副多页的TIFF**

CxImage *pimage[3];
pimage[0]=&image1;
pimage[1]=&image2;
pimage[2]=&image3;
FILE* hFile;
hFile = fopen("multipage.tif","w+b");
CxImageTIF multiimage;
multiimage.Encode(hFile,pimage,3);
fclose(hFile);
或者
FILE* hFile;
hFile = fopen("c://multi.tif","w+b");
CxImageTIF image;
image.Load("c://1.tif",CXIMAGE_FORMAT_TIF);
image.Encode(hFile,true);
image.Load("c://2.bmp",CXIMAGE_FORMAT_BMP);
image.Encode(hFile,true);
image.Load("c://3.png",CXIMAGE_FORMAT_PNG);
image.Encode(hFile);
fclose(hFile);

**  如何复制和粘贴图像**

//复制(copy)
HANDLE hDIB = image->CopyToHandle();
if (::OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd())) {
if(::EmptyClipboard()) {
if (::SetClipboardData(CF_DIB,hDIB) == NULL ) {
            AfxMessageBox( "Unable to set Clipboard data" );
}    }    }
CloseClipboard();
//粘贴(paste)
HANDLE hBitmap=NULL;
CxImage *newima =new CxImage();
if (OpenClipboard()) hBitmap=GetClipboardData(CF_DIB);
if (hBitmap) newima->CreateFromHANDLE(hBitmap);
CloseClipboard();

**  如何在图片框控件(Picture Box)中显示图像 **

HBITMAP m_bitmap = NULL;
CxImage image("myfile.png", CXIMAGE_FORMAT_PNG);
...
CDC* hdc = m_picture.GetDC();
HBITMAP m_bitmap = image.MakeBitmap(hdc->m_hDC);
HBITMAP hOldBmp = m_picture.SetBitmap(m_bitmap);
if (hOldBmp) DeleteObject(hOldBmp);
if (hdc->m_hDC) m_picture.ReleaseDC(hdc);
...
if (m_bitmap) DeleteObject(m_bitmap);

**  译后序：**

  关于图像处理库，我相信大家有一个共同的疑问：这么多图像处理库，我该如何选择？在CSDN的blog中有这样一段文字，比较透彻地回答了这个问题，感谢作者的透彻解释：

  "CxImage类库是一 个优秀的图像操作类库。它可以快捷地存取、显示、转换各种图像。有的读者可能说，有那么多优秀的图形库，如OpenIL,FreeImage, PaintLib等等，它们可谓是功能强大，齐全，没必要用其它的类库。但我要说，这些类库基本上没有免费的，使用这些类库，你要被这样那样的许可协议所 束缚。

  在这点上，CxImage类库是完全免费的。另外，在使用上述类库时，你会遇到重重麻烦。因为它们大部分是平台无关的，且用C语言写成，有的还夹杂 着基本的C++ wrapper和成堆德编译选项的声明需要你去处理。而CxImage类库在这方面做得很好。还有让我最看好的，就是作者完全公开了源代码。相对于那些封 装好的图形库和GDI+来说，这一点使我们可以进一步学习各种编解码技术，而不再浮于各种技术的表面。"

