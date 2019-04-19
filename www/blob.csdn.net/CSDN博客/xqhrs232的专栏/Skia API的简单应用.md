# Skia API的简单应用 - xqhrs232的专栏 - CSDN博客
2011年06月23日 15:58:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2125
原文地址::[http://blog.csdn.net/cuiyan0214/archive/2010/10/25/5964739.aspx](http://blog.csdn.net/cuiyan0214/archive/2010/10/25/5964739.aspx)
1 Skia 绘图概述 
Skia 是 Google 一个底层的图形、文本、图像、动画等多方面的图形库，是 Android 中图形系统的引擎。 Skia 作为第三方软件放在 external 目录下： external/skia/ 。 skia 的源文件及部分头文件都在 src 目录下，导出的头文件在 include 目录下。
使用 Skia 的 API 进行图形绘制时主要会用到一下几个类： 
SkBitmap 、 SkCanvas 、 SkPaint 和 SkRect ，其中 SkBitmap 用来设置像素， SkCanvas 写入位图， SkPaint 设置颜色和样式， SkRect 用来绘制矩形。其实现代码主要在 src/core 目录下。
2 使用 Skia 绘图的步骤 
a) 定义一个位图 32 位像素并初始化 
SkBitmap bitmap; 
bitmap.setConfig(SkBitmap::kARGB_8888_Config, 200, 200); 
其中 setConfig 为设置位图的格式，原型为 void setConfig(Config, int width, int height, int rowBytes = 0)
Config 为一个数据结构 
enum Config { 
kNo_Config, // 不确定的位图格式 
kA1_Config, //1 位 ( 黑 , 白 ) 位图 
kA8_Config, //8 位 ( 黑 , 白 ) 位图 
kIndex8_Config, // 类似 windows 下的颜色索引表，具体请查看 SkColorTable 类结构 
kRGB_565_Config, //16 位象素 565 格式位图，详情请查看 SkColorPriv.h 文件 
kARGB_4444_Config, //16 位象素 4444 格式位图，详情请查看 SkColorPriv.h 文件 
kARGB_8888_Config, //32 位象素 8888 格式位图，详情请查看 SkColorPriv.h 文件 
kRLE_Index8_Config, 
kConfigCount 
}; 
b) 分配位图所占的空间 
bitmap.allocPixels() 
其实 allocPixels 为重载函数，原型为 bool allocPixels(SkColorTable* ctable = NULL) 
参数 ctable 为颜色索引表，一般情况下为 NULL 。 
c) 指定输出设备 
SkCanvas canvas(new SkDevice(bitmap)); 
其中 canvas 为一个多构造函数，原型为 
explicit SkCanvas(const SkBitmap& bitmap) ， 
explicit SkCanvas(SkDevice* device = NULL) 
explicit 关健字的意思为：不允许类型转换 
输出设备可以为一个上下文 Device, 也可以指定为一张位图。 
d) 设备绘制的风格 
Paint paint; 
SkRect r; 
paint.setARGB(255, 255, 0, 0); 
r.set(25, 25, 145, 145); 
canvas.drawRect(r, paint); 
paint 可以指定绘图的颜色，文本的大小及对齐方式，编码格式等等，因为以前位图的格式设置为 kARGB_8888_Config ，所以这里要设置绘制的颜色 setARGB(255, 255, 0, 0) ，第一位参数为透明颜色通道，其它三位分别为 R 、 G 、 B 。 r 设置要绘制的范围，最后通过 drawRect 绘制出指定区域的一个方形。
这样，一个红色的矩形就绘制成功了。 
SkCanvas 主要完成三种绘制功能： 
a 基本图形绘制 ( 如 drawARGB,drawLine 函数 ) 
b 图像文件绘制（ drawBitmap 函数） 
c 文本绘制（ drawText 函数） 
相关 API 有： 
canvas.drawRect(rect, paint);
         canvas.drawOval(oval, paint);
         canvas.drawCircle(x, y, radius, paint);
         canvas.drawRoundRect(rect, rx, ry, paint);
         canvas.drawPath(path, paint);
         canvas.drawBitmap(bitmap, x, y, &paint);
         canvas.drawBitmapRect(bitmap, &srcRect, dstRect, &paint);
         canvas.drawBitmapMatrix(bitmap, matrix, &paint);
         canvas.drawText(text, length, x, y, paint);
         canvas.drawPosText(text, length, pos[], paint);
         canvas.drawTextOnPath(text, length, path, paint);
e)
例程
i ）画点、线、圆、文字 
#include "SkBitmap.h" 
#include "SkDevice.h" 
#include "SkPaint.h" 
#include "SkRect.h" 
#include "SkImageEncoder.h" 
#include "SkTypeface.h" 
using namespace std; 
int main() 
{ 
SkBitmap bitmap; 
bitmap.setConfig(SkBitmap::kARGB_8888_Config,320,240); 
bitmap.allocPixels(); 
SkCanvas canvas(new SkDevice(bitmap)); 
SkPaint paint; 
// draw points with red. 
paint.setARGB(255, 255, 0, 0); 
paint.setStrokeWidth(4); 
canvas.drawPoint(40,30, paint); 
canvas.drawPoint(80,60, paint); 
canvas.drawPoint(120,90, paint); 
//draw a line with green. 
paint.setARGB(255, 0, 255, 0); 
paint.setStrokeWidth(4); 
canvas.drawLine(160,10,320,110,paint); 
//draw a circle with bule. 
paint.setARGB(255, 0, 0, 255); 
canvas.drawCircle(80,180,50,paint); 
//draw text with red 
SkTypeface *font = SkTypeface::CreateFromFile("simkai.ttf"); 
if ( font ) 
{ 
paint.setARGB(255, 255, 0, 0); 
paint.setTypeface( font ); 
paint.setTextSize(24); 
canvas.drawText("HELLO!:)", 8, 200, 180, paint); 
} 
SkImageEncoder::EncodeFile("snapshot.png", bitmap,SkImageEncoder::kPNG_Type,100);
return 0; 
} 
程序执行后，得到如下输出结果： 
ii) 图像的编解码 
该例程目前测试只支持 .png 格式的图片， .jpg 还不支持，还未找到原因。 
#include "SkBitmap.h" 
#include "SkDevice.h" 
#include "SkPaint.h" 
#include "SkRect.h" 
#include "SkImageEncoder.h" 
#include "SkImageDecoder.h" 
#include <iostream> 
using namespace std; 
int main() 
{ 
int ret = -1; 
SkBitmap bitmap; 
//SkImageDecoder 
ret = SkImageDecoder::DecodeFile("./old.png", &bitmap); 
cout<< "get the decode type = "<< bitmap.config() << endl; 
//SkImageEncoder 
ret = SkImageEncoder::EncodeFile("new1.png",bitmap,SkImageEncoder::kPNG_Type,100);
cout<< "encode data to png result = "<< ret<< endl; 
return 0; 
} 
SkImageDecoder::DecodeFile("./old.png", &bitmap); 
将 png 转换成位图格式，并将数据放到 bitmap 变量中 
SkImageEncoder::EncodeFile("snapshot.png", bitmap,SkImageEncoder::kPNG_Type,/* Quality ranges from 0..100 */ 100);
将 bitmap 中的数据编码输出为 .png 格式，第一位参数为 png 文件路径，第二位为指定的输出位图，第三位为文件的类型，第四位参数指定了输出位图的质量，范围为 0..100 ，默认为 80 。
3 图形图像特效 
src/effects 目录的文件主要实现一些图形图像的特效，包括 遮罩、浮雕、模糊、滤镜、渐变色、离散、透明以及 PATH 的各种特效等。 
4 动画 
src/animator 目录的文件主要实现了 Skia 的动画效果，Android不支持。 
5 界面 UI 库 
src/view 目录 构建了一套界面 UI 库。 
组件包括 Window,Menu, TextBox, ListView, ProgressBar, Widget, ScrollBar,TagList,Image 等。
6 其它 
a) src/gl 目录： 这部分是 skia 调用 OpenGL 或 OpenGL ES 来实现 3D 效果。 
如果定义了 MAC ，则使用 OpenGL ，如果定义了 Android ，则使用嵌入式 系统 上的 esgl 三维图形库。 
b)src/images 目录： 主要是 SkImageDecoder 和 SkImageEncoder 以及 SkMovie 。主要是用来处理 images 的，能处理的图像类型包括： BMP 、
JPEG/PVJPEG 、 PNG 、 ICO ，而 SkMovie 是用来处理 gif 动画的。 
c) src/opts 目录：性能优化的代码。 
d) src/pdf 目录： 处理 PDF 文档，用了一个 fpdfemb 库。 
e) src/ports 目录： 这部分是 skia 的一些接口在不同系统上的实现，平台相关的代码，比如字体、线程、时间等， 主要包括几个部分： Font ， Event ， File ， Thread ， Time ， XMLParser
这些与 Skia 的接口，需要针对不同的 操作系统 实现。 
f) src/svg 目录：   矢量图像，Android不支持。 
SkSVGPath, SkSVGPolyline, SkSVGRect, SkSVGText, SkSVGLine, SkSVGImage, SkSVGEllipse 等等。
g) src/text 目录：？？？ 
h) src/utils 目录： 是一些辅助工具类。 
SkCamera, SkColorMatrix,SkOSFile,SkProxyCanvas,SkInterpolator 等文件。 
i) src/xml ： 这是处理 xml 数据的部分， skia 在这里只是对 xml 解析器做了一层包装，具体的 xml 解析器的实现需要根据不同的操作系统及宿主程序来实现。
j) Third-party library 
除了自身的所有文件外， skia 还使用了一些 third-party library 以及包含了不少 linux 上的头文件。 
通过分析 skia 源程序，发现 skia 主要使用以下几个第三方库： 
Zlib ，处理数据的压缩和解压缩 
Jpeglib ，处理 jpeg 图像的编码解码 
Pnglib ，处理 png 图像的编码解码 
giflib ，处理 gif 图像 
fpdfemb ，处理 pdf 文档 
skia 还需要一些 linux/unix 下的头文件（可能还需要更多）： 
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
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/cuiyan0214/archive/2010/10/25/5964739.aspx](http://blog.csdn.net/cuiyan0214/archive/2010/10/25/5964739.aspx)
