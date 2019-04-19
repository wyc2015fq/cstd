# android 的图形引擎 skia - xqhrs232的专栏 - CSDN博客
2011年06月21日 16:06:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1255
原文地址::[http://blog.csdn.net/linweig/archive/2009/12/19/5040029.aspx](http://blog.csdn.net/linweig/archive/2009/12/19/5040029.aspx)
简介1
Google Android 使用 skia 作为其核心图形引擎。skia 也是 Google Chrome 的图形引擎。
Skia 图形渲染引擎最初由 Skia 公司开发，该公司于 2005 年被 Google 收购。
Skia 与 Openwave's (现在叫 Purple Labs)V7 vector graphics engine 非常 类似，它们都来自于 Mike Reed 的公司。
如果你正在做移动设备上的矢量图形或者图像渲染相关的工作，你应该 checkout skia 的源代码仔细研究下。
Skia is quite elegant in its simplicity and power, a great combination.
"By adding Skia engine to Chrome, Google can ensure good graphics performance on devices that don't have a graphics processing unit."5
Features4
skia 有大概 80,000 行代码，基于 C++ 开发，主要特点包括：
Optimised software-based rasteriser (module sgl/); 
Optional GL-based acceleration of certain graphics operations including shader support and textures (module gl/);
Animation capabilities (module animator/); 
Some built-in SVG support (module (svg/) 
Built-in image decoders: PNG, JPEG, GIF, BMP, WBMP, ICO (modules images/); 
Text capabilities (no built-in support for complex scripts); 
Some awareness of higher-level UI toolkit constructs (platform windows, platform events): Mac, Unix (sic. X11, incomplete), Windows, wxwidgets
Performace features
Copy-on-write for images and certain other data types; 
Extensive use of the stack, both internally and for API consumers to avoid needless allocations and memory fragmentation;
Thread-safety to enable parallelisation. 
The library is portable and has (optional) platform-specific backends:
Fonts: Android / Ascender, FreeType, Windows (GDI); 
Threading: pthread, Windows; 
XML: expat, tinyxml; 
Android shared memory (ashmem) for inter-process image data references; 
Google Chrome 选择 Skia 的理由6
为什么不用 OpenGL 或者 DirectX 来加速渲染？
数据从 video card 读出，然后在另一个进程中再拷贝回 video card，这种 情况下不值得用硬件加速渲染； 
相对而言，图形绘制只占很少时间，大部分时间是计算页面元素的位置、风 格、输出文本，即使用了 3D 加速也不会有明显改善； 
为什么不用别的图形库？
Windows GDI：Microsoft Windows 的底层图形 API，相对而言只具备基 本的绘制功能，像 <canvas>, SVG 需要单独实现。
GDI+：Windows上更高级的 API，GDI+ 使用的是设备独立的 metrics，这会 使 Chrome中的 text and letter spacing 看以来与别的 Windows 应用不同 (which measure and draw text tailored to the screen device)。而且， 微软当时也推荐开发人员使用新的图形 API，GDI+ 的技术支持和维护可能有 限。
Cairo：一个开源 2D 图形库，已经在 firefox 3 中使用。 
最终选择 Skia 是因为：
skia is cross-platform; 
there was already a high-quality WebKit port using it created for Android's browser;
we had in-house expertise. 
[http://blog.chromium.org/2008/10/graphics-in-google-chrome.html](http://blog.chromium.org/2008/10/graphics-in-google-chrome.html)
源码下载
[http://src.chromium.org/viewvc/chrome/trunk/src/skia/](http://src.chromium.org/viewvc/chrome/trunk/src/skia/)
Hello World4
In this simple example we draw a few rectangles to a memory-based image buffer. This also demonstrates how one might integrate with the platform graphics system to get something on screen, though in this case we’re using Cairo to save the resulting image to
 disk:
#include "SkBitmap.h"
#include "SkDevice.h"
#include "SkPaint.h"
#include "SkRect.h"
#include <cairo.h>
int main()
{
  SkBitmap bitmap;
  bitmap.setConfig(SkBitmap::kARGB_8888_Config, 100, 100);
  bitmap.allocPixels();
  SkDevice device(bitmap);
  SkCanvas canvas(&device);
  SkPaint paint;
  SkRect r;
  paint.setARGB(255, 255, 255, 255);
  r.set(10, 10, 20, 20);
  canvas.drawRect(r, paint);
  paint.setARGB(255, 255, 0, 0);
  r.offset(5, 5);
  canvas.drawRect(r, paint);
  paint.setARGB(255, 0, 0, 255);
  r.offset(5, 5);
  canvas.drawRect(r, paint);
  {
    SkAutoLockPixels image_lock(bitmap);
    cairo_surface_t* surface = cairo_image_surface_create_for_data(
        (unsigned char*)bitmap.getPixels(), CAIRO_FORMAT_ARGB32,
        bitmap.width(), bitmap.height(), bitmap.rowBytes());
    cairo_surface_write_to_png(surface, "snapshot.png");
    cairo_surface_destroy(surface);
  }
  return 0;
}
You can build this example for yourself linking statically to the libskia.a object file generated during the Chrome build process on Linux.
Standalone skia2
网上有人尝试了 stand-alone 版的 skia，具体内容参考：
[http://vuhung16.blogspot.com/2008/10/standalone-skia.html](http://vuhung16.blogspot.com/2008/10/standalone-skia.html)
Open Skia3
Google Code 上发现一个 openskia 项目，从作者看跟上面的 standalone skia 是一个人。
[http://code.google.com/p/openskia/](http://code.google.com/p/openskia/)
openskia is a graphics engine based Skia Graphics Engine that was developed by Skia inc., a company Google acquired in 2005.
This vector graphics engine software makes highend visual effects possible on feature phone. It is tiny in size and is capable of delivering very high quality. Skia's engine is the graphics core of both Google Android and Google Chrome.
Reference
Skia Source Code Released 
Standalone skia 
openskia 
Skia graphics library in Chrome: First impressions 
Chrome's secret: The Skia engine 
Graphics in Google Chrome 
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/linweig/archive/2009/12/19/5040029.aspx](http://blog.csdn.net/linweig/archive/2009/12/19/5040029.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/linweig/archive/2009/12/19/5040029.aspx](http://blog.csdn.net/linweig/archive/2009/12/19/5040029.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/linweig/archive/2009/12/19/5040029.aspx](http://blog.csdn.net/linweig/archive/2009/12/19/5040029.aspx)
