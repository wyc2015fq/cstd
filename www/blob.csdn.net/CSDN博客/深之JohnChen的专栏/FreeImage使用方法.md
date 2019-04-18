# FreeImage使用方法 - 深之JohnChen的专栏 - CSDN博客

2010年12月05日 18:29:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：32543


**FreeImage****使用方法**

**一、****FreeImage ****简介**

**FreeImage ****是一款免费的、开源的、跨平台（****Windows ****、****Linux ****和****Mac OS X ****）的，支持****20 ****多种图像类型的（如****BMP ****、****JPEG ****、****GIF ****、****PNG ****、****TIFF ****等）图像处理库。其最大优点就是采用插件驱动型架构，具有快速、灵活、简单易用的特点，得到了广泛使用。相关的源码、库文件及帮助文档可以从****http://freeimage.sourceforge.net/ ****免费下载使用。**下载最新的FreeImage 3.14.1 [WIN32], 解压缩该FreeImage3141Win32.zip文件，包含FreeImage.h、FreeImage.lib和 FreeImage.dll**文件。**

**FreeImage ****的主要功能有多格式位图的读写；方便的位图信息获取；位深度转换；位图页面访问；基本的几何变换和点处理；通道合并与混合等。****FreeImage ****暂时不支持矢量图形和高级图像处理，位图绘制需要由用户来完成。**** FreeImage ****中的所有函数都以****FreeImage_ ****开头，如图像文件的读写函数分别为****FreeImage_Load ****和****FreeImage_Save ****。****FIBITMAP ****数据结构保存着位图信息和像素数据，是****FreeImage ****的核心。**

**二、****初始化**** FreeImage**

FreeImage主要包含头文件FreeImage.h 、静态链接库FreeImage.lib 、FreeImaged.lib ，动态链接库FreeImage.dll 和FreeImaged.dll 。其中，以d 结尾的库文件用于调试版，其余用于发行版。为了在VC 中使用FreeImage方法如下：

＃i nclude"FreeImage.h"

#ifdef _DEBUG 

#pragma comment(lib, "FreeImaged.lib") 

#else 

#pragma comment(lib, "FreeImage.lib") 

#endif

默认情况下，FreeImage 被当作动态链接库。若需用作静态链接库，则在应用程序类的InitInstance 中调用FreeImage_Initialise(0) 加载FreeImage 库，重载ExitInstance 并在其中调用FreeImage_DeInitialise() 释放FreeImage 库。

三、读取图像

FreeImage 是插件驱动的，每种位图的读写函数实际上就是一个插件模块。采用插件驱动型构架便于功能扩展，通过创建或引入新的插件，可以支持新的图像类型。为便于维护打开的位图，在文档类添加FIBITMAP 型指针m_pFI ，用以指向打开的位图。在构造函数中将其置为空，在析构函数中调用FreeDIB 将其释放。FreeDIB 定义如下：

void CDIPDoc:: FreeDIB () 

{ 

if(m_pFI) { 

FreeImage_Unload(m_pFI);// 释放位图

m_pFI = NULL; 

} 

} 

在OnNewDocument() 和OnFileOpen() 函数中也应调用FreeDIB ，以确保建立或打开新图像时先释放以前的位图。

读取图像的基本步骤如下：

调用库函数FreeImage_GetFileType 由文件头分析文件类型，若不能识别，则调用库函数FreeImage_GetFIFFromFilename 从文件扩展名判断文件类型。

若是支持的图像文件类型，则调用库函数FreeImage_FIFSupportsReading 判断是否支持该位图类型的读操作，若支持则调用库函数FreeImage_Load 加载位图。

在文档类中添加成员函数LoadDIB ，实现位图读入功能，代码如下：

FIBITMAP* CDIPDoc::LoadDIB(LPCTSTR lpszPathName, int flag){ 

FREE_IMAGE_FORMAT fif = FIF_UNKNOWN; 

fif = FreeImage_GetFileType(lpszPathName); 

if(fif == FIF_UNKNOWN)

fif = FreeImage_GetFIFFromFilename(lpszPathName); 

if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))

{ 

FIBITMAP *dib = FreeImage_Load(fif, lpszPathName, flag); 

return dib; 

} 

return NULL; 

} 

在文档类中重载OnFileOpen ，调用GetFilterString 创建文件类型过滤字符串，调用LoadDIB 加载位图。

void CDIPDoc::OnFileOpen() 

{ 

FIBITMAP* pFI = NULL; 

char szFilter[2048]; 

GetFilterString(szFilter); 

CFileDialog dlg(TRUE,NULL,NULL, 

OFN_HIDEREADONLY, szFilter); 

if(dlg.DoModal() == IDOK)

{ 

CString strPath = dlg.GetPathName(); 

pFI = LoadDIB(strPath, 0); 

if(pFI)

{ 

FreeDIB();// 释放以前的位图

m_pFI = pFI;// 指向新位图

UpdateAllViews (NULL); 

} 

} 

} 

四、显示图像

在视图类的OnDraw 函数中绘制图像。当前源图像与客户区尺寸相同时，调用SetDIBitsToDevice 绘制，这样可以加快绘制速度，否则调用StretchDIBits 按比例缩放位图。

void CDIPView::OnDraw(CDC* pDC) 

{ 

CDIPDoc* pDoc = GetDocument(); 

ASSERT_VALID(pDoc); 

FIBITMAP *pFI = pDoc->m_pFI; 

if(pFI)

{ 

BYTE* pBits = FreeImage_GetBits(pFI); 

BITMAPINFO* pIf=FreeImage_GetInfo(pFI); 

int W = FreeImage_GetWidth(pFI); 

int H = FreeImage_GetHeight(pFI); 

CRect rectClient;GetClientRect(rectClient); 

int dstW = rectClient.Width(); 

int dstH = rectClient.Height(); 

::SetStretchBltMode(pDC->GetSafeHdc(),COLORONCOLOR); 

if(dstW == W && dstH == H)

{ 

::SetDIBitsToDevice(pDC->GetSafeHdc(),0,0, W, H,0,0,0, H,pBits,pIf,DIB_RGB_COLORS); 

} 

else {// 按比例缩放

float fScale=(float)(W*dstH)/(float)(H*dstW); 

if(fScale > 1.0f)

dstH = (int) (dstH / fScale); 

else 

dstW = (int) (dstW * fScale); 

::StretchDIBits(pDC->GetSafeHdc(),0,0,dstW,dstH,0,0,W,H,pBits,pIf,DIB_RGB_COLORS,SRCCOPY); 

} 

} 

} 

五、保存图像

（1 ）调用FreeImage_GetFIFFromFilename 函数从文件名判断期望保存的图像类型，如果不支持则返回；否则转（2 ）。

（2 ）调用FreeImage_GetImageType 判断当前位图数据类型，若为标准位图，则测试是否支持该种图像类型的写操作以及位深度转换，否则调用FreeImage_FIFSupportsExportType 检查是否能够输出当前位图。若测试成功，则调用FreeImage_Save 保存位图。

在文档类中定义成员函数SaveDIB 实现上述功能，代码如下：

BOOL CDIPDOC::SaveDIB(FIBITMAP *dib, LPCTSTR *lpszPath, int flag){ 

FREE_IMAGE_FORMAT fif =FIF_UNKNOWN; 

BOOL bSuccess = FALSE; 

fif=FreeImage_GetFIFFromFilename(lpszPath); 

if(fif != FIF_UNKNOWN ) 

{ 

BOOL bCanSave; 

FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib); 

if(image_type == FIT_BITMAP)

{ 

WORD bpp = FreeImage_GetBPP(dib); 

bCanSave=(FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp)); 

} 

else 

{ 

bCanSave=FreeImage_FIFSupportsExportType(fif, image_type); 

} 

if(bCanSave) bSuccess= 

FreeImage_Save(fif,dib,lpszPath,flag); 

} 

return bSuccess; 

} 

六、一个简单的程序实例

＃i nclude <iostream>

＃i nclude <cassert>

＃i nclude"FreeImage.h"

#pragma comment(lib,"FreeImage.lib")

#define show(a) std::cout<< a <<std::endl

int main(int argc,char *argv)

{

//初始化FreeImage

FreeImage_Initialise(TRUE);

//默认分配一块内存存储影像，其中bpp表示图像深度

//FIBITMAP *bitmap = FreeImage_AllocateT(FIT_BITMAP,Width,height,bpp,

//red_mask,green_mask,blue_mask);

//最后一个参数用来改变函数的行为或激活位图插件的一种特性，每个插件有他自己的参数集

//FreeImage_LoadU参数都是一样，不过只在win32下起作用

FIBITMAP * bmp = FreeImage_Load(FIF_BMP,"cs5.BMP",BMP_DEFAULT);

//FreeImage_SaveU和上面的一样

if( bmp)

{

//FreeImage_Save(FIF_BMP, bmp, "whtm.bmp", BMP_DEFAULT);

}

//获取影像的宽高，都以像素为单位

int Width = FreeImage_GetWidth(bmp);

int Height = FreeImage_GetHeight(bmp);

//获取影像的byte宽度，不对，我上面运行与Width相等

int ByteWidth = FreeImage_GetLine(bmp);

//返回每个像素使用灰度值

show(FreeImage_GetBPP(bmp));

//返回数据类型

show(FreeImage_GetImageType(bmp));

//返回调色板的大小

show(FreeImage_GetColorsUsed(bmp));

show(Width<<" "<<Height<<" "<<ByteWidth);

//assert(ByteWidth == Width*8 );

show(FreeImage_GetDIBSize(bmp));

//有调用load就有unload，否则就会内存泄漏

FreeImage_Unload(bmp);

FreeImage_DeInitialise();

return 0;

}

转载于梅园《FreeImage学习笔记》[http://blog.chinaunix.net/u1/58776/showart_527523.html](http://blog.chinaunix.net/u1/58776/showart_527523.html)

参考资料：

**源代码：****[http://freeimage.sourceforge.net/](http://freeimage.sourceforge.net/)**

FreeImage图形库说明文档(中文版)：[http://download.csdn.net/source/2756601](http://download.csdn.net/source/2756601)


