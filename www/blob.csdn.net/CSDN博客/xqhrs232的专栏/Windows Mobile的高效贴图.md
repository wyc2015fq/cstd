# Windows Mobile的高效贴图 - xqhrs232的专栏 - CSDN博客
2012年10月25日 23:13:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：654
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://blog.csdn.net/jiftlixu/article/details/6972299](http://blog.csdn.net/jiftlixu/article/details/6972299)

Windows Mobile的高效贴图 
原文：[http://www.cppblog.com/guogangj/archive/2010/06/20/118316.html](http://www.cppblog.com/guogangj/archive/2010/06/20/118316.html)
位图这个概念对于计算机图形学来说是个至关重要的概念，我们在屏幕上看到的任何东西，对计算机来说，其实都是位图，简单地说，无论你是想显示文字，还是线条，抑或bmp，png，jpg和gif等图像文件，最终都是要直接或间接转变为计算机显示设备所认识的位图，才能显示在屏幕上。
我最近接手了一个项目，是Windows Mobile平台的，主要做UI美化，贴图是其中的一大块，我遇到的最大的问题就是贴图的效率问题，如何将一张内存里的图片高效绘制出来，实现平滑流畅的UI动画效果。我尝试了许多办法，甚至DirectDraw，但我发觉在硬件不支持的情况下，DirectDraw除了让代码变得更复杂之外，没有任何优点。好，接下去我们来分析一下如何尽量发挥GDI的威力。
跟贴图相关的函数有几个：
BitBlt：最基本的块传输函数。
StretchBlt：比同BitBlt，它支持图像的拉伸和压缩，当不需要拉伸和压缩时候，它的效果和BitBlt并无二致。
TransparentBlt：比同StretchBlt，它多了个“抠色”（Color Keying）功能，能把某种颜色或者某个范围的颜色抠去而不作块传输处理，以此来绘制不规则图像。
AlphaBlend：比同StretchBlt，它支持Alpha混合，即“半透明”效果，效果比简单的“抠色”更好。
这几个函数是一个比一个强，但也意味着效率一个比一个低，总体上看差不多是这样的，运算量越大，当然就越慢，但测试下来发觉这其实并不绝对，后面会提到。
了解了这几个函数之后，我们开始加载一张图片来观察效果，我准备的是一张320*320的png图片，利用Windows Mobile 6.0提供的IImage接口来加载它，并把它转变为位图，获得HBITMAP。加载png的代码可以通过搜索引擎搜索“IImage用法”等关键词来获取，此处略过。
加载好图片后，创建一个和设备显示设备兼容的DC（Device Context），选入上面加载的位图，用BitBlt绘制，代码如下（为简洁起见，只贴出关键代码，并且不考虑资源释放）：
HDC hWndDC = GetDC(hWnd);
HDC hMemDC = CreateCompatibleDC(hWndDC);
SelectObject(hMemDC, hBitmap); //hBitmap是前面加载的图片
BitBlt(hWndDC, 0, 0, iWidth, iHeight, hMemDC, 0, 0, SRCCOPY);
我们通过添加一些debug代码来观察BitBlt的执行时间，在我的模拟器上大约是50 - 60ms，我发觉这个速度并不快，按道理说，BitBlt应该可以在极短的时间之内完成的（1 - 2ms），也只有这样才能实现“流畅”的UI动画效果，否则图一旦多起来，岂不是更慢。
![](http://hi.csdn.net/attachment/201111/15/0_1321327557a406.gif)
我尝试修改BitBlt的最后一个参数，我发觉换成NOTSRCCOPY，速度更慢，变成了70多ms，说明运算量更大了，这不是简单的内存拷贝；而当我把最后一个参数换成BLACKNESS或者WHITENESS的时候，速度则很快，1ms-2ms即可完成，很显然，对BitBlt来说，把目标全部置为黑色或者白色，运算量远少于像素传送。在实验的时候把BitBlt替换为另外的几个函数，效果和预期的相差不大，如果图像需要拉伸，则执行得更慢一些，但如果图像不是拉伸，而是压缩，即缩小显示，执行速度居然比较快，有些意外，这是因为压缩后图像变小，需要传输的像素变少的缘故。
我考虑如何提高绘图效率，经过很多次尝试，终于有所突破，我最后发现：如果先把位图存放在一个和DC兼容的位图中，再用这个位图对目标设备进行像素传输，速度十分理想。代码：
HDC hWndDC = GetDC(hWnd);
HDC hMemDC = CreateCompatibleDC(hWndDC);
HDC hMemDCToLoad = CreateCompatibleDC(hWndDC);
HBITMAP hMemBmp = CreateCompatibleBitmap(hWndDC, iWidth, iHeight); // The compatible bitmap
HGDIOBJ hOldBmp = SelectObject(hMemDC, hMemBmp);
SelectObject(hMemDCToLoad, hBitmap);
BitBlt(hMemDC, 0, 0, iWidth, iHeight, hMemDCToLoad, 0, 0, SRCCOPY); //Do this in initialization
BitBlt(hWndDC, 0, 0, iWidth, iHeight, hMemDC, 0, 0, SRCCOPY); //This BitBlt's speed is very fast
第一次调用BitBlt，可以看作是初始化，我们计算第二个BitBlt的耗时，只有1 - 2ms，非常不错，经过分析，我认为原因应该是这样（不一定全对，如有不妥请读者指出）：
只有在位图格式完全一致的情况下，BitBlt才能执行真正的内存拷贝，否则是要经过转换的，转换是需要消耗CPU时间的，所以慢。
那如何知道位图的格式呢？用GetObject可以看出来：
![](http://hi.csdn.net/attachment/201111/15/0_1321327482P2Ft.gif)
如上图所示，bmp是从png文件加载进来的位图的信息，而bmp2是用CreateCompatibleBitmap创建的位图的信息，从这我们能看到，前者是24bit位图，即一个像素用3个字节表示，而后者是16bit位图，一个像素用两个字节来表示，这个BitBlt执行过程中，就需要转换了，因此耗时。而实际上位图的差别可能比这个还要复杂些，如果再讨论设备无关位图，那就说不完了……
总而言之，为了提高效率，我们要想方设法把加载进来的位图转变为设备兼容位图，绘制的时候直接BitBlt这些设备兼容位图，来实现位图的高效绘制。
前面讨论的主要是BitBlt，那对于别的几个Blt函数呢？我都尝试过了，除了AlphaBlend之外，兼容位图到设备的Blt速度上都有显著的提高，而AlphaBlend则无法正常工作，因为兼容位图不带Alpha通道，而AlphaBlend貌似需要32bit的ARGB格式的位图方可正常工作，这个问题我思考了好久都无解，如果哪位读者对提高AlphaBlend的工作效率有心得，不妨跟我联系下，我正急需这方面的技术资料。
因此，我给出这样的结论，阶段性结论：从文件（或资源）加载位图后，把位图转为设备兼容位图，这样使得BitBlt在执行SRCCOPY的时候直接使用内存拷贝，速度很快，即便需要拉伸压缩或者抠色等运算，使用兼容位图的速度也是相当不错的，而使用AlphaBlend的时候，如果需要较高的效率，就应从设计上避免绘制大幅位图，改用小幅位图，在不必要对每一帧都执行Alpha混合的时候，就避免执行，以免影响画面的流畅性。
