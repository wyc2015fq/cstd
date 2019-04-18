# CxImage类库的技术资料 - 深之JohnChen的专栏 - CSDN博客

2005年11月29日 14:27:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3443


CxImage类库是一个优秀的图像操作类库。它可以快捷地存取、显示、转换各种图像。有的读者可能说，有那么多优秀的图形库，如OpenIL,FreeImage,PaintLib等等，它们可谓是功能强大，齐全，没必要用其它的类库。但我要说，这些类库基本上没有免费的，使用这些类库，你要被这样那样的许可协议所束缚。在这点上，CxImage类库是完全免费的。另外，在使用上述类库时，你会遇到重重麻烦。因为它们大部分是平台无关的，且用C语言写成，有的还夹杂着基本的C++ wrapper和成堆德编译选项的声明需要你去处理。而CxImage类库在这方面做得很好。还有让我最看好的，就是作者完全公开了源代码。相对于那些封装好的图形库和GDI+来说，这一点使我们可以进一步学习各种编解码技术，而不再浮于各种技术的表面。如果想下载CxImage，可以[http://www.codeproject.com](http://www.codeproject.com)上下载。 CxImage类库的结构：

一个CxImage对象是一个扩展了的位图。作者只是在位图结构上添加了一些起存储信息作用的成员变量。一个CxImage对象（同时）也是一组层。每个层只有在需要时才会分配相应的缓冲区。CxImage::pDib代表着背景图像，CxImage::pAlpha代表着透明层，CxImage::pSelection代表着被选中的层，被用来创建图像处理时让用户感兴趣的区域。在这三个特殊层面的基础上，你可以增加一些额外的层，这些层可以存储在CxImage::pLayers中。一般说来，层是一个完整的CxImage对象。因此，你可以构造很复杂的嵌套层。下面是CxImage的一些成员变量:

需要大家注意的是:整个CxImage类库非常大。如果你只需要能处理其中的几种格式，你可以在主要的头文件ximage.h中找到一些开关选项来关闭一些图像库。JPG、PNG、TIFF中的每一个库，都会向最终程序增加约100KB的内容。而CxImage类库压缩后只有约60KB。所以，你需要谨慎挑选一些你真正需要的类库。作者提供的示例工程在编译后，你会发现如下一些文件: ·CxImage : cximage.lib - static library ·CxImageCrtDll : cximagecrt.dll - DLL not using mfc ·CxImageMfcDll : cximage.dll - DLL using mfc ·Demo : demo.exe - program linked with cximage.lib and the C libraries ·DemoDll : demodll.exe - program linked with cximagecrt.dll ·j2k,jasper,jbig,jpeg,png,tiff,zlib : static C libraries 构建这些工程需要耗费几分钟的时间（中间文件可达60MB）。下面则是使用CxImage类库前必须设置的一些参数： 

