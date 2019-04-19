# 如何开发绚丽、高效率的界面(Windows嵌入式系统) - xqhrs232的专栏 - CSDN博客
2012年10月25日 23:03:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：847
原文地址::[http://kb.cnblogs.com/page/49641/](http://kb.cnblogs.com/page/49641/)
相关网帖
1>ALPHABLEND FOR ARM----[http://tctianchi.yo2.cn/articles/alphablend-for-arm.html](http://tctianchi.yo2.cn/articles/alphablend-for-arm.html)
[上篇文章](http://www.cnblogs.com/wangkewei/archive/2009/10/05/1578219.html)中提到用户体验(UE)，并且说到国内有专门去做UE的团队也很少。据我了解Microsoft、Nokia、Google等，还有国内的Baidu是有比较专业的UE团队。对于我们这样的普通团队、普通开发者来说，这样的经验实在太少了。而且普遍更认为UE是UI
 Designer的事情，与我们这样的Developer没有太多关系。
当然不是，UE远超过UI。很多因素造成了UE差，比如一份不正确的数据表明17%的用户认为手机运行速度慢，Windows Mobile手机开机漫长的等待就十分的让我受不了。我们开发的应用是否有过优化?运行效率是否已经很让用户满意了?等等这些问题留在开发中思考吧。
在使用优秀的产品时用心体会、用心观察、用心思考，在此基础上创新。逐渐提高UE设计能力。(等Windows 7正式发布了，我们可以讨论讨论其UE^^)
这篇文章仅仅讨论有关界面开发上Developer涉及到的技术问题，在学习过程中，随着越深越广越觉得自身水平的不足，所以只敢抛砖引玉，更多希望能够引起大家对界面开发技术、对UE的讨论。
上篇文章已经列出目录：
1.相关商用产品一览
2.Windows系统下图形编程的相关基础知识
3.DirectDraw简介
4.DirectDraw驱动开发
5.DirectDraw应用开发
6.一个推荐的入门Sample
第1部分已经在上篇文章讲过，鉴于篇幅的原因2、3、4、5、6部分将在下篇文章介绍。这篇文章先介绍下如何使用Win32下的GDI等接口实现绚丽、高效率的界面。这样我们就能发现GDI等接口的不足，进而引申到DirectDraw上面。(这篇文章默认你有一定的Windows编程基础，熟悉GDI等概念。)
补充内容●如何使用Win32下的GDI等接口实现绚丽、高效的界面
1.如何让界面绚丽?
怎么样的算绚丽?有很漂亮的图片?有Alpha透明?有Animation?
每个人的审美观点都不同，所以如果你的界面很多人认为绚丽那就可以了。设计界面主要是[Designer](http://en.wikipedia.org/wiki/Designer)的工作，包括UI逻辑的设计，色彩搭配设计等，我认为这也可以进一步分工：熟悉用户习惯的Designer、美学Designer等。但是一般情况下这些让程序员给代劳了。
下面介绍Windows提供给开发人员的相关接口，利用这些接口设计你认为绚丽的界面。
2.如何透明?如何半透明?如何颜色渐变?
以下是我使用Imaging COM组件封装的一个函数，可以使用其绘制PNG图片，当然也可以绘制其它图片。绘制带Alpha通道的PNG图片即实现了透明。
#include 
#include 
#include 
#pragma comment(lib, "Imaging.lib")
BOOL DrawPNG(HDC hDC, TCHAR *szPicString, RECT &rcDraw)
{
	BOOL br = FALSE;
	IImagingFactory *pImgFactory = NULL;
	IImage *pImage = NULL;
	ImageInfo sImgInfo;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// Create the imaging factory.if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IImagingFactory,
		(void **)&pImgFactory)))
	{
		// Load the image from the JPG file.if (SUCCEEDED(pImgFactory->CreateImageFromFile(
			szPicString,
			&pImage)))
		{
			// Draw the image.pImage->Draw(hDC, &rcDraw, NULL);
			pImage->Release();
			pImage = NULL;
			br = TRUE;
		}
		pImgFactory->Release();
	}
	CoUninitialize();
	return br;
}
------------------------------------------------------------------------------------------------------
而封装的这个函数实现了将一个DC根据Alpha值半透明绘制到另一个DC上，使用GDI函数AlphaBlend实现。
BOOL AlphaBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest,
			  int nWidthDest, int nHeightDest,
			  HDC hdcSrc, int nXOriginSrc, int nYoriginSrc,
			  int nWidthSrc, int nHeightSrc,
			  BYTE alpha) {
				  BLENDFUNCTION bf;
				  bf.BlendOp = AC_SRC_OVER;
				  bf.BlendFlags = 0;
				  bf.SourceConstantAlpha = alpha;
				  bf.AlphaFormat = 0;
				  returnAlphaBlend(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, 
					  hdcSrc, nXOriginSrc, nYoriginSrc, nWidthSrc, nHeightSrc, bf);
}
如果你的设备支持AlphaBlend硬件加速那将是非常棒的事情，否则软件方式会有点影响性能。
------------------------------------------------------------------------------------------------------
颜色渐变也是直接有API可以支持：
BOOL GradientFill(
  HDC hdc,
  PTRIVERTEX pVertex,
  ULONG nVertex,
  PVOID pMesh,
  ULONG nCount,
  ULONG ulMode
);
hdc 
[in] Handle to the destination device context.
pVertex 
[in] Pointer to an array of TRIVERTEX structures, each of which defines a triangle vertex.
nVertex 
[in] The number of vertices in pVertex.
pMesh 
[in] Array of GRADIENT_RECT structures in rectangle mode.
nCount 
[in] The number of rectangles in pMesh.
ulMode 
[in] Specifies gradient fill mode. The following table shows the possible values for ulMode.
This function fills rectangular regions with a background color that is interpolated from color values specified at the vertices.
不管你使用.Net CF平台调用这些API，还是Win32/MFC/ATL/WTL直接调用这些API，你都是可以实现这些效果的。更多内容请查询开发文档，毕竟那才是最好的参考资料。
3.如何实现动画?
动画的原理就是一帧一帧的画面按照时间轴向后移动，在骗过眼睛之后就成了动画，所以你得到动画最简单的方法就是按照一定间隔将不同图片一张一张绘制到屏幕上，虽然很简单，但是在编程中经常使用这种方法。有时简单的往往是最好的。
这里还有个技巧，比如将每张图片使用Photoshop中的运动滤镜模糊下，这样使用上面方法得到的动画会有种非常快速的感觉。也可以用类似的方法来用2D表现三维的事物，得到3D动画的效果。
还可以使用GIF动画的方式，比如在开机和关机时。以下封装的函数仅供参考，我没用心整理。
BOOL DisplayGIF(TCHAR *szPicString)
{
HANDLE hFile = CreateFile(strFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
if (hFile == INVALID_HANDLE_VALUE)
{
return FALSE;
}
DWORD dwFileSize = GetFileSize(hFile, NULL);
if ( (DWORD)-1 == dwFileSize )
{
CloseHandle(hFile);
return FALSE;
}
HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
if (hGlobal == NULL)
{
CloseHandle(hFile);
return FALSE;
}
LPVOID pvData = GlobalLock(hGlobal);
if (pvData == NULL)
{
GlobalUnlock(hGlobal);
CloseHandle(hFile);
return FALSE;
}
DWORD dwBytesRead = 0;
BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
GlobalUnlock(hGlobal);
CloseHandle(hFile);
if (!bRead)
{
return FALSE;
}
IStream* pStream = NULL;
if ( FAILED(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream)) )
{
return FALSE;
}
if( NULL == pStream )
{
return FALSE;
} IImage *pImage = NULL;
RECT rc;
IImagingFactory *pImgFactory = NULL;
CoInitializeEx(NULL, COINIT_MULTITHREADED);
if ( !SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void **)&pImgFactory)) )
{
return FALSE;
}
IImageDecoder* pDecoder = NULL;
UINT nCount = 0;
if ( !SUCCEEDED(pImgFactory->CreateImageDecoder(pStream, DecoderInitFlagNone, &pDecoder)) )
{
return FALSE;
}
pDecoder->GetFrameDimensionsCount(&nCount);
GUID *pDimensionIDs = (GUID*)new GUID[nCount];
pDecoder->GetFrameDimensionsList(pDimensionIDs,nCount);
TCHAR strGuid[39];
StringFromGUID2(pDimensionIDs[0], strGuid, 39);
UINT frameCount = 0;
pDecoder->GetFrameCount(&pDimensionIDs[0],&frameCount);
UINT iSize = 0;
pDecoder->GetPropertyItemSize(PropertyTagFrameDelay,&iSize);
BYTE* pBuff = new BYTE[iSize];
PropertyItem* pItem = (PropertyItem*)pBuff;
pDecoder->GetPropertyItem(PropertyTagFrameDelay,iSize,pItem);
int fCount = 0;
ImageInfo Info;
pImgFactory->CreateImageFromStream(pStream,&pImage);
pImage->GetImageInfo(&Info);
rc.left = rc.top = 0;
rc.right = Info.Width;
rc.bottom = Info.Height;
HDC   tempDC;
HBITMAP    hbmNew = NULL;
void *     pv;
BITMAPINFO bmi = { 0 };
HBITMAP    hbmOld = NULL;
tempDC = CreateCompatibleDC(NULL);
bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
bmi.bmiHeader.biWidth       = Info.Width;
bmi.bmiHeader.biHeight      = Info.Height;
bmi.bmiHeader.biPlanes      = 1;
bmi.bmiHeader.biBitCount    = (SHORT) max(16, GetDeviceCaps(tempDC, BITSPIXEL));
bmi.bmiHeader.biCompression = BI_RGB;
hbmNew = CreateDIBSection(tempDC, &bmi, DIB_RGB_COLORS, &pv, NULL, 0);
hbmOld = (HBITMAP)SelectObject(tempDC, hbmNew);
pImage->Draw(tempDC, &rc, NULL);
pDecoder->SelectActiveFrame(&pDimensionIDs[0], ++fCount);
BitBlt(g_hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, tempDC, 0, 0, SRCCOPY);
delete []pBuff;
delete []pDimensionIDs;
pDecoder->Release();
pImage->Release();
pImgFactory->Release();
CoUninitialize();
return TRUE;
}
4.如何有较高的运行效率?
后面的内容会介绍到使用GDI这些“较高层次”的接口是很难有较高的运行效率。
但是可以使用一些技巧，比如“空间换取时间”。相信"[Lazy Computation](http://en.wikipedia.org/wiki/Lazy_evaluation)”你有听过，延迟处理这项任务直到真正需要的时候(在编程中我们也会经常用到，需要有这个意识。)这里使用的技巧有点恰恰相反的味道，把用户将来很可能用到的地方先处理好，然后储存起来，而并不是等到用户真正需要的时候才去处理。
比如使用Imaging COM组件绘制PNG图片时，每次都需要加载组件的库文件，然后卸载，界面可能要反复刷新，然后反复绘制PNG图片。这时可以考虑在程序启动的时候使用非界面主线程将绘制好的PNG图片保存起来(比如以Device Context的形式)，界面刷新的时候仅仅是BitBlt到目标设备。BitBlt的效率是比较高的，如果仍然不能满足你的效率要求，可以考虑下面介绍的DirectDraw等技术。
上面的方法对于具有丰富开发经验的应该比较清楚，但是新手往往会忽略。在开发界面时我们要保证一个基本原则：想尽一切办法在现有的条件下提高界面响应用户的速度，界面要以用户为中心。所以开发时需要保持这个意识。
5.如何提高程序启动速度?
第4部分说过，为了提高运行效率，可以将常用的界面在程序启动时一起缓存到内存中，那么程序的启动时间会大大增加，如何解决这个问题?我的建议是UI主线程仅仅加载少量的用户启动后直接就能看到的界面，而另起一个子线程(叫A)用于加载其它界面，其它界面加载完之后这个子线程退出，当用户点击其它界面时，主线程如果发现子线程A并没有退出，说明其它界面还没有加载完，让用户等待。 
这么设计的好处是，将最耗时的任务分摊出去，即能保证了用户快速看到界面，又能在之后的运行中有较高的效率。
6.如何在绚丽和效率之间平衡?
最好的方法是得到界面运行时具体的时间消耗数据，如果必要可以精确到每个函数。得到一份系统正常情况下的数据，得到几份环境恶劣情况下的数据(比如系统非常繁忙、设备电量很少、要处理的数据非常多等)。定量的去分析解决这些问题。如果在恶劣的环境下你的绚丽界面表现的仍然不错，恭喜你，你太棒了!
Windows CE/Windows Mobile也提供了些基本的Performance API(像DirectDraw等技术还有自己的Performance接口和工具)：
BOOL QueryPerformanceCounter(
LARGE_INTEGER* lpPerformanceCount
);- lpPerformanceCount
[in] Pointer to a variable that the function sets, in counts, to the current performance-counter value. If the installed hardware does not support a high-resolution performance
 counter, this parameter can be set to zero.
This function retrieves the current value of the high-resolution performance counter if one is provided by the OEM.
BOOL QueryPerformanceFrequency(
LARGE_INTEGER* lpFrequency
);- lpFrequency
[out] Pointer to a variable that the function sets, in counts per second, to the current performance-counter frequency. If the installed hardware does not support a high-resolution
 performance counter, the value passed back through this pointer can be zero.
This function retrieves the frequency of the high-resolution performance counter if one is provided by the OEM.
上面两个API需要OEM在OAL层提供实现，精度可以低于1ms，否则可以使用下面的API。
DWORD GetTickCount(void);
For Release configurations, this function returns the number of milliseconds since the device booted, excluding any time that the system was suspended. GetTickCount starts at zero on boot and
 then counts up from there.
For debug configurations, 180 seconds is subtracted from the the number of milliseconds since the device booted. This enables code that uses GetTickCount to be easily tested for correct overflow
 handling.
另外优化PNG、Bitmap、GIF等图片，让图片清晰度和大小刚好满足要求。
7.控件为什么如此降低运行效率?怎样减少控件的使用?
手机软件不同于桌面系统软件，一方面手机的处理速度更低、电池容量更小，另一方面用户会使用手机处理更紧急的事情。所以这也是我认为 不应该完全把桌面系统软件开发经验借鉴到手机软件开发上的原因。一个240x320分辨率大小的手机界面，你给放上5、6个控件，甚至更多，这个界面注定不会太高效率，这样的界面也不适合作为用户最常用的界面，比如今日界面。另一方面，Windows的标准、通用控件不会有太绚丽的外观，即使自定义的。但是这些控件能够带来很明显的开发速度。所以我们要协调好。不能为了窗口而窗口，更不能一切皆窗口。 
那么你会问如何协调。我的建议是能不用控件的地方就不要用，大多地方可以直接使用图片，比如实现多状态按钮你可以这样做： 
WM_LBUTTONDOWN消息处理里面先判断Point是否在按钮的Rect中，如果是将按下状态的图片DC BitBlt到屏幕对应位置，WM_LBUTTONUP消息处理里面再BitBlt回来。
8.基于Win32的界面运行效率比基于.Net CF高，但是开发效率低，怎么办?
Win32编程已经很古老、很“落后”了。但是在处理速度还不及奔三的Windows嵌入式设备上有时你不得不选择。把界面常用的功能代码封装成库(类库也可以)，积累这样的资源可以提高团队的开发效率。C++泛型编程就是以牺牲编译时效率换取代码重用，但是不影响运行时效率，值得去深入学习下，而且有现成的库可用，比如STL。
还有其它的技术可供选择：DirectDraw(后面介绍的)、Direct3DM、OpenGL ES等。但是开发难度较高。
9.如何使用[GDI+](http://msdn.microsoft.com/en-us/library/ms533798(VS.85).aspx)(Native/Managed)?
GDI+是GDI的下一个版本，它进行了很好的改进，并且易用性更好。GDI的一个好处就是你不必知道任何关于数据怎样在设备上渲染的细节，GDI+更好的实现了这个优点，也就是说，GDI是一个中低层API，你还可能要知道设备，而GDI+是一个高层的API，你不必知道设备。以下引用自MSDN文档：
"2-D vector graphics involves drawing primitives (such as lines, curves, and figures) that are specified by sets of points on a coordinate system.
For example, the Rect class stores the location and size of a rectangle; the Pen class stores information about line color, line width, and line style; and the Graphics class has methods for drawing lines, rectangles, paths, and other figures. There are also
 several Brush classes that store information about how closed figures and paths are to be filled with colors or patterns.
Certain kinds of pictures are difficult or impossible to display with the techniques of vector graphics. Imaging part will resolve this problem. An example of such a class is CachedBitmap, which
 is used to store a bitmap in memory for fast access and display.
Typography is concerned with the display of text in a variety of fonts, sizes, and styles. One of the new features in GDI+ is subpixel antialiasing. “
Windows CE/Windows Mobile下的GDI+仅仅是Windows桌面系统的一个很小的子集。OpenNETCF中封装了[GDI+](http://community.opennetcf.com/articles/cf/archive/2007/10/31/using-gdi-on-windows-mobile.aspx)，可以为基于.Net
 CF的开发者提供便利，微软提供的Native Code版本就是前面有提到的Imaging COM组件，你也可以直接调用gdiplus.dll里面的类和方法。网上已经有人将Windows CE版本GDI+不支持的部分桌面系统版本GDI+的功能整理进来，你可以使用其提供的Lib库和头文件进行开发。但可能不是很稳定。
Windows Mobile 6中的gdiplus.dll文件：
![image](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/b5372937dbc2_E72E/image_thumb.png)
将上面的dll文件导出得到的函数：
![image](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/b5372937dbc2_E72E/image_thumb_1.png)
10.如何实现透明控件等其它问题?
因为Windows系统目前不支持窗口Alpha透明，所以无法直接使控件背景透明，我们常用的方法是将控件后面的窗口中对应的背景作为控件的背景。
原理说的有点绕，你可以去研究下代码：
[http://www.codeproject.com/KB/mobile/transparent_controls.aspx](http://www.codeproject.com/KB/mobile/transparent_controls.aspx)(C++)
[http://www.codeproject.com/KB/dotnet/TransparentControl.aspx](http://www.codeproject.com/KB/dotnet/TransparentControl.aspx)(C#)
其它参考内容：
[黎波的博客](http://www.cnblogs.com/upto/)
[怎样在Windows Mobile上设计一个美观的用户界面程序(Win32)](http://www.cnblogs.com/wangkewei/archive/2009/03/08/1406354.html)
[Windows Mobile 6.0下实现自绘多种状态按钮(Win32)](http://www.cnblogs.com/wangkewei/archive/2009/02/24/1397490.html)
[Windows Mobile 6.0下实现自绘多种状态按钮(Win32) 续](http://www.cnblogs.com/wangkewei/archive/2009/02/28/1400497.html)
