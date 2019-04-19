# 抓屏的各种方法(http://www.codeproject.com/KB/dialog/screencap.aspx) - maopig的专栏 - CSDN博客
2011年06月22日 15:21:00[maopig](https://me.csdn.net/maopig)阅读数：1839标签：[windows																[codec																[null																[api																[video																[byte](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
文章翻译自**[P.GopalaKrishna](http://www.codeproject.com/script/profile/whos_who.asp?id=251226)**的**Various methods for capturing the screen**一文，原版地址见下面。本文章版权归原作者所有。
如果转载该译文,请保证文章的完整性，并注明来自[www.farproc.com](http://www.farproc.com/)
袁晓辉
2005/6/12 
原版地址：[http://www.codeproject.com/dialog/screencap.asp#Windows%20Media%20API%20for%20Capturing%20the%20Screen%20:](http://www.codeproject.com/dialog/screencap.asp%23Windows%20Media%20API%20for%20Capturing%20the%20Screen%20:)
[本文附带源码1下载 39K](http://www.codeproject.com/dialog/screencap/ScreenCap.zip)
[本文附带源码2下载 135.5K](http://www.codeproject.com/dialog/screencap/ScreenCapDx.zip)
[本文附带源码3下载 59.8K](http://www.codeproject.com/dialog/screencap/WMEncScrnCap.zip)
目录：
l导言
l用GID函数抓屏
l用DirectX方式抓屏
l用Windows Media API抓屏
导言
有时候我们需要编程抓取整个屏幕上的内容，下面我将介绍抓屏是如何实现的。典型地，我们可以用GID和DirectX来完成，另外一个选择是Windows Media API，在这篇文章我会逐一加以分析。在每一种方法里，一旦我们把屏幕的内容保存到了程序定义的内存块或bitmap文件里，我们就可以进一步利用它们来生成动画和电影，这个过程你可以参考“[从HBitmap创建电影](http://www.geocities.com/krishnapg/createmovie.html)”一文中，以获得更多的帮助。
用GDI函数抓屏
如果我们不太在意抓屏的效率，并且我们想要的只是一个屏幕快照的话，可以考虑使用GDI方式。这种抓屏机制是以“桌面也是一个窗口，桌面也有一个窗口句柄（HWND）”这个简单的常识为基础的，如果我们得到了桌面的设备上下文（DC），就可以利用blit（复制）它的内容到我们创建的DC中。我们可以用GetDeskWindow（）得到桌面的窗口句柄，从句柄得到DC也是很容易的。具体的实现步骤为：
1.通过GetDesktopWindow（）函数得到桌面的窗口句柄
2.用GetDC（）取得桌面窗口的DC 
3.创建和屏幕DC兼容的位图和DC（CreateCompatibleBitmap （）和CreateCompatibleDC（）），并把这个位图选进该DC（SelectObject（））
4.当你准备好抓屏时，就复制桌面窗口DC的内容到兼容DC，你就完成的抓屏过程，兼容位图中就是抓屏时刻的屏幕内容
5.完成后别忘了释放你创建的对象，内存是宝贵的（对别的程序来说）
示例代码：
void CaptureScreen() 
{ 
int nScreenWidth = GetSystemMetrics(SM_CXSCREEN); 
int nScreenHeight = GetSystemMetrics(SM_CYSCREEN); 
HWND hDesktopWnd = GetDesktopWindow(); 
HDC hDesktopDC = GetDC(hDesktopWnd); 
HDC hCaptureDC = CreateCompatibleDC(hDesktopDC); 
HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, 
nScreenWidth, nScreenHeight); 
SelectObject(hCaptureDC,hCaptureBitmap); 
BitBlt(hCaptureDC,0,0,nScreenWidth,nScreenHeight,hDesktopDC,0,0,SRCCOPY); 
SaveCapturedBitmap(hCaptureBitmap); //Place holder - Put your code 
//here to save the captured image to disk 
ReleaseDC(hDesktopWnd,hDesktopDC); 
DeleteDC(hCaptureDC); 
DeleteObject(hCaptureBitmap); 
} 
上面代码段中，GetSystemMetrics（）返回屏幕的宽度（
```
SM_CXSCREEN
```
）和高度（
```
SM_CYSCREEN
```
）。关于如何保存抓到的位图到文件和如何置到剪贴板，请参看附带的源代码，很简单的。示例代码每隔一段时间就通过上述技术抓屏，并把图像序列保存到动画。
DirectX方式
用DreictX进行抓屏也是很简单的，DirectX提供了很优雅的实现。
每个DirectX程序都包含一个被我们称作缓冲的内存区域，其中保存了和该程序有关的显存内容，这在程序中被称作后台缓冲（Back Buffer），有些程序有不止一个的后台缓冲。还有一个缓冲，在默认情况下每个程序都可以访问－前台缓冲。前台缓冲保存了和桌面相关的显存内容，实质上就是屏幕图像。
我们的程序通过访问前台缓冲就可以捕捉到当前屏幕的内容。由DirectX的底层优化机制做保证，我们的抓屏效率是很高的，至少比GDI方式高。
在DirectX程序中访问前台缓冲是很简单的，
```
IDirect3DDevice8
```
```
接口提供了
```
```
GetFrontBuffer()
```
```
方法，它接收一个
```
```
IDirect3DSurface8
```
```
对象指针做参数，并复制前台缓冲的内容到该
```
```
Surface
```
```
。
```
```
IDirect3DSurfce8
```
```
对象可以用
```
```
IDirect3DDevice8::CreateImageSurface()
```
得到。一旦屏幕内容被保存到了这个surface，我们就可以用
```
D3DXSaveSurfaceToFile()
```
```
方法直接把内容保存到磁盘
```
```
bmp
```
```
文件。示例代码如下：
```
```
```
extern IDirect3DDevice8* g_pd3dDevice; 
Void CaptureScreen() 
{ 
IDirect3DSurface8 * pSurface; 
g_pd3dDeviceàCreateImageSurface(ScreenWidth,ScreenHeight, 
D3DFMT_A8R8G8B8,&pSurface); 
g_pd3dDevice->GetFrontBuffer(pSurface); 
D3DXSaveSurfaceToFile("Desktop.bmp",D3DXIFF_BMP,pSurface, 
NULL,NULL); 
pSurface->Release(); 
} 
上面，
```
g_pd3dDevice
```
是一个初始化好的
```
IDirect3DDevice
```
```
对象，这个例子直接把捕捉到的图像保存到文件。然而，有时候我们想访问直接这个图像中的各个位，我们可以使用
```
```
IDirect3DSurface8::LockRect()
```
```
，它给我们一个执行
```
```
surface
```
```
内存的指针，也就是捕捉到的图像的数据。我们复制这些数据到程序定义的内存中就可以操作它了。看下面的代码：
```
```
```
extern void* pBits; 
extern IDirect3DDevice8* g_pd3dDevice; 
IDirect3DSurface8 * pSurface; 
g_pd3dDeviceàCreateImageSurface(ScreenWidth,ScreenHeight, 
D3DFMT_A8R8G8B8,&pSurface); 
g_pd3dDevice->GetFrontBuffer(pSurface); 
D3DLOCKED_RECT lockedRect; 
pSurfaceàLockRect(&lockedRect,NULL, 
D3DLOCK_NO_DIRTY_UPDATE|D3DLOCK_NOSYSLOCK| 
D3DLOCK_READONLY))); 
for( int i=0 ; i < ScreenHeight ; i++) 
{ 
memcpy( (BYTE*) pBits + i * ScreenWidth * BITSPERPIXEL / 8 , 
(BYTE*) lockedRect.pBits + i* lockedRect.Pitch , 
ScreenWidth * BITSPERPIXEL / 8); 
} 
g_pSurface->UnlockRect(); 
pSurface->Release(); 
上面的pBits是一个void*，请保证为先为它分配组足够的内存空间。BITSPERPIXEL一般用32位色即可，它也取决于你的显示器当前配置。一个需要注意的是，surface的宽度和被捕捉的屏幕宽度不一样。由于内存对齐的原因(按WORD对齐的内存通常在访问时效率较高)，surface在每行结尾处可能会有多余的bits以使它对齐到word边界上。
```
lockedRect.Pitch
```
```
给我们提供了两个连续行的开端之间的字节数。也就是说我们在读取一行时要向后移动指针
```
```
Pitch
```
```
字节而不是
```
```
Width
```
```
字节。你可以用下面的代码反序复制
```
```
surface
```
```
：
```
```
```
for( int i=0 ; i < ScreenHeight ; i++) 
{ 
memcpy((BYTE*) pBits +( ScreenHeight - i - 1) * 
ScreenWidth * BITSPERPIXEL/8 , 
(BYTE*) lockedRect.pBits + i* lockedRect.Pitch , 
ScreenWidth* BITSPERPIXEL/8); 
} 
这对于从top-down位图到bottom-up位图很有用。
我们还可以使用
```
IDirect3DSurface9
```
的
```
GetDC()
```
```
方法取得
```
```
DirectX surface
```
```
的
```
```
GDI
```
```
兼容
```
```
DC
```
```
，然后复制它的内容到我们的兼容
```
```
DC
```
```
。如果你用的是
```
```
DirectX9
```
```
，试试吧。
```
```
```
``
```
最后，需要注意的一点，文档提到：
```
```
FrontBuffer
```
```
是一个比较慢的操作，设计就是如此，所以在效率很关键的程序中应避免使用。已经警告你了！本文附带的源代码用这种技术定时捕捉屏幕，并保存为动画。
```
```
```
``
用Windows Media API抓屏
Windows Media 9.0 支持用Windows Media Encoder 9 API来抓屏。它有一个编码器叫*Windows Media Video 9 Screen codec*，特别为抓屏优化过。Windows Media Encoder API提供了一个IWMEncoder2接口可以用来高效地捕捉屏幕图像。
用这种技术进行抓屏也很简单，首先我们用
```
CoCreateInstance()
```
创建一个IWMEncoder2对象：
IWMEncoder2* g_pEncoder=NULL; 
CoCreateInstance(CLSID_WMEncoder,NULL,CLSCTX_INPROC_SERVER, 
IID_IWMEncoder2,(void**)&g_pEncoder); 
这个Encoder对象包含了捕捉屏幕的所需的全部操作，然而为了正确地工作，编码器对象的行为取决于被称作profile的设置。一个profile只是一个包含了所有控制编码操作设置的文件，我们可以根据被捕捉的数据的特性在运行时创建包含自定义设置的profile。为了在你的抓屏程序中使用profile，我们基于*Windows Media Video 9 Screen codec*来创建自定义的profile。自定义的profile对象从IWMEncProfile2开始就被支持了。我们可以用CoCreateInstance来创建自定义profile 
IWMEncProfile2* g_pProfile=NULL; 
CoCreateInstance(CLSID_WMEncProfile2,NULL,CLSCTX_INPROC_SERVER, 
IID_IWMEncProfile2,(void**)&g_pProfile); 
我需要在profile里指定编码器的听众（audience）。每个profile可以包含多个听众配置，它们是
```
IWMEncAudienceObj
```
```
接口对象。这里我们为
```
```
profile
```
```
使用一个听众。我们可以通过
```
```
IWMEncProfile::AddAudience()
```
```
为我们的
```
```
profile
```
```
创建听众，这个函数返回一个
```
```
IWMEncAudienceObj
```
```
指针，可以用来配置视频编码器
```
(
```
IWMEncAudienceObj::put_VideoCodec()
```
)
```
，视频帧对象
```
(
```
IWMEncAudienceObj::put_VideoHeight()
```
和
```
IWMEncAudienceObj::put_VideoWidth()
```
)我们用下面的代码来配置视频编码器：
extern IWMEncAudienceObj* pAudience; 
#define VIDEOCODEC MAKEFOURCC('M','S','S','2') 
//MSS2 is the fourcc for the screen codec 
long lCodecIndex=-1; 
g_pProfile->GetCodecIndexFromFourCC(WMENC_VIDEO,VIDEOCODEC, 
&lCodecIndex); //Get the Index of the Codec 
pAudience->put_VideoCodec(0,lCodecIndex); 
fourcc是针对每个编码器的唯一的标识，*Windows Media Video 9 Screen codec*的fourcc为MSS2。
```
IWMEncAudienceObj::put_VideoCodec()
```
```
接受
```
```
profile
```
```
索引来组织一个
```
```
profile
```
```
，索引可以用
```
```
IWMEncProfile::GetCodecIndexFromFourCC()
```
取得。
一旦我们配置完毕一个profile对象，我们就可以用
```
IWMEncSourceGroup :: put_Profile()
```
选择这个profile到我们的编码器。一个源组（SourceGruop）是一组视频流来源或音频流来源，或html来源。每个编码器可以使用许多源组，并从中取得输入数据。由于我们的程序仅仅使用视频流中是视频来源。这个视频来源需要用
```
IWMEncVideoSource2::SetInput(BSTR)
```
 Screen Device来配置为输入来源：
extern IWMEncVideoSource2* pSrcVid; 
pSrcVid->SetInput(CComBSTR("ScreenCap://ScreenCapture1"); 
目的输出可以用
```
IWMEncFile::put_LocalFileName()
```
配置为保存到视频文件（wmv文件）。IWMEncFile对象可以用
```
IWMEncoder::get_File()
```
```
得到：
```
```
```
IWMEncFile* pOutFile=NULL; 
g_pEncoder->get_File(&pOutFile); 
pOutFile->put_LocalFileName(CComBSTR(szOutputFileName); 
现在，一旦编码器对象的一切所需配置都完成后，我们就可以用
```
IWMEncoder::Start()
```
```
开始抓屏。
```
```
IWMEncoder::Stop()
```
和
```
IWMEncoder::Pause
```
```
可以用来停止和暂停捕捉。
```
```
```
```
这些适用于全屏捕捉，我们也可以通过调整输入视频来源流的属性来选择一个区域进行捕捉。我们可以用
```
```
IWmEnVideoSource2
```
```
的
```
```
I
```
```
PropertyBag
```
```
接口来实现：
```
```
```
#define WMSCRNCAP_WINDOWLEFT CComBSTR("Left") 
#define WMSCRNCAP_WINDOWTOP CComBSTR("Top") 
#define WMSCRNCAP_WINDOWRIGHT CComBSTR("Right") 
#define WMSCRNCAP_WINDOWBOTTOM CComBSTR("Bottom") 
#define WMSCRNCAP_FLASHRECT CComBSTR("FlashRect") 
#define WMSCRNCAP_ENTIRESCREEN CComBSTR("Screen") 
#define WMSCRNCAP_WINDOWTITLE CComBSTR("WindowTitle") 
extern IWMEncVideoSource2* pSrcVid; 
int nLeft, nRight, nTop, nBottom; 
pSrcVid->QueryInterface(IID_IPropertyBag,(void**)&pPropertyBag); 
CComVariant varValue = false; 
pPropertyBag->Write(WMSCRNCAP_ENTIRESCREEN,&varValue); 
varValue = nLeft; 
pPropertyBag->Write( WMSCRNCAP_WINDOWLEFT, &varValue ); 
varValue = nRight; 
pPropertyBag->Write( WMSCRNCAP_WINDOWRIGHT, &varValue ); 
varValue = nTop; 
pPropertyBag->Write( WMSCRNCAP_WINDOWTOP, &varValue ); 
varValue = nBottom; 
pPropertyBag->Write( WMSCRNCAP_WINDOWBOTTOM, &varValue ); 
本文附带的源码实现此中技术的抓屏。除去生成的动画质量很好外，一个有意思的地方是鼠标指针也被抓到了（GDI和DirectX默认是不抓取鼠标指针的）。
注意，为了适用WindowMedia9.0 API，你的电脑必须安装Windows Media9.0 SDK，你可以用下面地址下载：
- [http://msdn.microsoft.com/library/default.asp?url=/downloads/list/winmedia.asp](http://msdn.microsoft.com/library/default.asp?url=/downloads/list/winmedia.asp)
最终用户必须安装Windows Media Encoder 9 系列才能运行你的程序。在发布基于Windows Media Encoder SDK的程序时，Windows Media Encoder软件也必须附带上去，要么在你的软件安装时自动安装Windows Media Encoder要么让用户自己下载安装。
Windows Encoder 9.0可以从下面地址下载：
[http://www.microsoft.com/windows/windowsmedia/ 9series/encoder/default.aspx](http://www.microsoft.com/windows/windowsmedia/9series/encoder/default.aspx)
结论
上面讨论的各种方法都是基于一个目标－抓取屏幕的内容。然而适用不同的技术，得到的结果也不一样。如果我们需要的只是偶尔的抓屏，GDI方式是个好的选择，因为它简单。然而如果你想得到更专业的结果，可以使用Windows Media。一个可能没有意义的要点是，这些技术捕捉到的内容的质量很大程度上决于你的系统设置，比如进制硬件加速会大大提高抓屏的质量和程序的运行效率。


