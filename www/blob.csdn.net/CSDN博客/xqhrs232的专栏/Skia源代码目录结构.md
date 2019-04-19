# Skia源代码目录结构 - xqhrs232的专栏 - CSDN博客
2011年06月25日 11:02:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1122标签：[symbian																[library																[android																[图形																[listview																[events](https://so.csdn.net/so/search/s.do?q=events&t=blog)](https://so.csdn.net/so/search/s.do?q=listview&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=symbian&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.cnblogs.com/zwxx/archive/2011/01/13/1934316.html](http://www.cnblogs.com/zwxx/archive/2011/01/13/1934316.html)
# Eclipse for c++ 安装SVN插件并从Google Code下载SKIA源码
# [http://jzxt.52biancheng.com/article/20110528/newpr-213.html](http://jzxt.52biancheng.com/article/20110528/newpr-213.html)
**animator 目录**
顾名思意，这部分是用来实现Skia的动画效果。主要包含79个CPP文件。从文件名来看，主要分为以下几类：
以SkAnimator开头的一些文件，SkAnimatorActive,SkAnimatorBase,SkAnimatorField,SkAnimatorSet,SkAnimatorScript等。
以SkDisplay开头的一些文件，SkDisplayAdle, SkDisplayAdd, SkDisplayEvents, SkDisplayList, SkDisplayInput, SkDisplayMovie等等。
以SkDraw开头的一些文件，SkDraw3D, SkDrawBitmap, SkDrawClip, SkDrawColor, SkDrawBlur, SkDrawLine, SkDrawPaint, SkDrawPath, SkDrawPoint等。
以SkScript开头的一些文件，包括SkScript, SkScriptRuntime, SkScriptDecompile等。
这里，skia实现了一个脚本语言，可能是用于控制动画。
skia的这个脚本语言包括基本的数学运算：加(ADD)、减(SUB)、乘(MUL)、除(DIV)、模(MOD)；基本逻辑及判断：大于（GT）、小 于（LT）、等于（EQ）、与（AND）、或（OR）、非（NOT）等；还包括基本的位运算（bit operation）。
在这个脚本中，主要有以下几个[**数据**]()类型：NoType, Int, Scalar（等同于Float）, String, Array, Object。
这个目录下还包含一些辅助性的文件，如SkBase64, SkTime等。
**core 目录**
这就是skia的核心了，主要包含89个CPP文件。
基本都是一些图形绘制函数。
主要包括SkColor, SkCanvas,SkBitmap,SkPicture,SkBlitter,SkCordic,SkPath,SkPoint,SkRect,SkRegion,SkMask 等等图形绘制类，还包括一些辅助性的类文件，sk64, skbuffer, skutils等。
skia是分层次绘制的，当需要绘制时，通知每个layer上对象的canvas来绘制。
**effects 目录**
这是skia实现一些图形图像效果的部分
主要包括20个CPP文件。
主要实现了以下特效：遮罩、浮雕、模糊、滤镜、渐变色、离散、透明等。
并且实现了PATH的各种特效。
**gl 目录**
从名字看，并不知道这是做什么用的，gl=graphics library，图形库。
主要包括7个CPP文件。
开打SkGL.h，可以发现
#ifdef SK_BUILD_FOR_MAC
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
    #include <AGL/agl.h>
    // use FBOs for devices
    #define SK_GL_DEVICE_FBO
#elif defined(ANDROID)
    #include <GLES/gl.h>
    #include <EGL/egl.h>
#endif
所以，这部分是skia调用OpenGL或OpenGL ES来实现3D效果。
如果定义了MAC，则使用OpenGL，如果定义了Android，则使用嵌入式[**系统**]()上的esgl三维图形库。porting的时候要加上#elif defined(SYMBIAN)之类的，防止别的地方也有android宏定义。
打开SkGL，发现是几个基本的材质贴图函数。
**images目录**
这部分是用来处理images的，共23个CPP文件和一些头文件。
主要是SkImageDecoder和SkImageEncoder以及SkMovie。
Skia能处理的图像类型包括以下
BMP
JPEG/PVJPEG
PNG
ICO
另外，skia还能处理PDF文档。
使用了一个fpdfemb库。
而SkMovie就是处理gif动画。
**ports目录**
这部分是skia的一些接口在不同系统上的实现，共24个CPP文件。
主要包括几个部分：Font，Event，File，Thread，Time，XMLParser
这些与Skia的接口，需要针对不同的[**操作系统**]()实现。
**svg 目录**
这个目录包含28个CPP文件，从文件名看，都是一些画图文件。
SkSVGPath, SkSVGPolyline, SkSVGRect, SkSVGText, SkSVGLine, SkSVGImage, SkSVGEllipse等等。
**utils 目录**
包含9个CPP文件，是一些辅助工具类。
SkCamera, SkColorMatrix,SkOSFile,SkProxyCanvas,SkInterpolator等文件
**views目录**
这个目录包含24个CPP文件。
skia构建了一套界面UI库。
组件包括Window,Menu, TextBox, ListView, ProgressBar, Widget, ScrollBar,TagList,Image等。
还有组件的事件类。
window对象负责接收事件，并传递给当前focus对象，若无focus对象，则focus自身。focus对象处理相应的key events和mouse events。
在SkiView基类类中定义并实现了draw, acceptFocus,moveFocus,click等方法。
**xml 目录**
当然，这是处理xml数据的部分了。
skia在这里只是对xml解析器做了一层包装，具体的xml解析器的实现需要根据不同的操作系统及宿主程序来实现。
third-party library
**以上就是所有的skia source code了，当然，仅仅依靠这些是不可能编译通过了，skia还使用了一些third-party library以及包含了不少[linux]()上的头文件。**
**通过分析skia源程序，发现skia主要使用以下几个第三方库：**
Zlib，处理数据的压缩和解压缩
Jpeglib，处理jpeg图像的编码解码
Pnglib，处理png图像的编码解码
giflib，处理gif图像
fpdfemb，处理pdf文档
**skia还需要一些linux/unix下的头文件（可能还需要更多）：**
stdint.h
unistd.h
features.h
cdefs.h
stubs.h
posix_opt.h
types.h
wordsize.h
typesizes.h
confname.h
getopt.h
mman.h
OS
1. Android & Linux
天然的支持Skia
2. Symbian
Symbian 3rd开始支持部分POSIX标准，所以以上的头文件函数实现，可能有部分不为Symbian所支持，不支持的必须自行实现。
参见：[http://innovator.samsungmobile.com/cn/cms/cnts/category.main.list.do?platformId=1&cateId=159](http://innovator.samsungmobile.com/cn/cms/cnts/category.main.list.do?platformId=1&cateId=159)
►C标准库和POSIX转义符（equivalents）
Symbian OS包括ANSI C / POSIX标准库[应用]()，提供了stdlib子集以支持将C和混合C / C++代码移植到Symbian OS。
它还包括P.I.P.S. POSIX库。如果您有一个现成应用， P.I.P.S.尤其可以使核心应用逻辑的移植容易得多。 P.I.P.S. 被专门的Wiki全面涵盖。
3.[Windows]()Mobile & Windows CE
支持POSIX，所以应该也可以支持SKIA
使用vs编译所有的[**skia**]()[**文件**]()，然后会得到大概3500条错误消息。这很可能
 是某些宏定义不正确，或者某些编译开关没有正确[**设置**]()，或者某些接口没有实现等。但编译器给出的错误消息往往对[**解决**]()问题没有帮助，编译器给出的绝大部分错误
 都是不可能出错的地方，标准的C++语法。于是只能先看一下SKIA的结构。
使用svn从[http://code.google.com/p/skia/](http://code.google.com/p/skia/)下载最新的代码，没有任何文档信息。skia的源文件及部分头文件都在skia/src目录下，导出的头文件在skia/include 目录下。打开Skia源文件夹，主要有以下部分：
