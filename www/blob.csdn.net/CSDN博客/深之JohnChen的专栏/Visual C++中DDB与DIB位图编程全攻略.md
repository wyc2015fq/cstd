# Visual C++中DDB与DIB位图编程全攻略 - 深之JohnChen的专栏 - CSDN博客

2006年03月10日 09:13:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3870标签：[c++																[编程																[null																[delete																[存储																[command](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[图像处理与识别](https://blog.csdn.net/byxdaz/article/category/144582)


Visual C++中DDB与DIB位图编程全攻略
来源: 天极网

**1. 基本概念**先来用通俗的语句讲解位图和调色板的概念。
我们知道，自然界中的所有颜色都可以由红、绿、蓝(R，G，B)三基色组合而成。针对含有红、绿、蓝色成分的多少，可以对其分别分成0～255个等级，而红、绿、蓝的不同组合共有256×256×256种，因此约能表示1600万种颜色。对于人眼而言，这已经是"真彩色"了。

对每个像素进行了（R，G，B）量化的图像就是位图，其在计算机中对应文件的扩展名一般为.bmp。既然用R，G，B的量化值就可以直接记录一张位图的所有像素，那我们需要调色板干什么呢？

首先，我们可以计算完全利用（R，G，B）组合来存储一个800×600的位图所需要的空间为：

800×600×3 = 1440000（字节）＝ 1.37M（字节）

惊人的大！因此，调色板横空出世了，它的功能在于缓解位图文件存储空间过大的问题。

假设一个位图为16色，其像素总数为800×600。我们只需要用4个bit就可以存储这个位图的每个像素在16种颜色中所处的等级，然后调色板提供了这16种等级对应的（R，G，B）值，这样，存储这个16色位图只需要：

800×600×4/8 = 240000（字节）= 0.22 M（字节）

额外的存储R，G，B表的开销（即调色板Palette，也称为颜色查找表LUT）仅仅为16×3＝48字节。

存储空间被大为减少！

常见的位图有单色、16色、256色、16位及24位真彩色5种，对于前三者（即不大于256色）都可以调色板方式进行存储，而对16位及24位真彩色以调色板进行存储是不划算的，它们直接按照R，G，B分量进行存储。

在此基础上我们来分析DDB位图（Device-dependent bitmap，与设备相关的位图）与DIB位图（Device-independent bitmap，与设备无关的位图）的概念以及二者的区别。

DDB依赖于具体设备，它只能存在于内存中（视频内存或系统内存），其颜色模式必须与特定的输出设备相一致，使用系统调色板。一般只能载入色彩较简单的DDB位图，对于颜色较丰富的位图，需使用DIB才能长期保存。

DIB不依赖于具体设备，可以用来永久性地保存图象。DIB一般是以*.BMP文件的形式保存在磁盘中的，有时也会保存在*.DIB文件中。 DIB位图的特点是将颜色信息储存在位图文件自身的颜色表中，应用程序要根据此颜色表为DIB创建逻辑调色板。因此，在输出一幅DIB位图之前，程序应该将其逻辑调色板选入到相关的设备上下文并实现到系统调色板中。

**2. 例程简述**
本文后续的讲解都基于这样的一个例子工程，它是一个基于对话框的MFC应用程序，包括2个父菜单：

（1） DDB位图

DDB位图父菜单又包括两个子菜单：

a. ID：IDM_LOADDDBPIC caption：加载

单击事件：加载资源中的DDB位图并显示之

b. ID：IDM_MARK_DDBPIC caption：标记

单击事件：在DIB位图中透明地添加天极网logo

（2） DIB位图

DIB位图父菜单又包括两个子菜单：

a. ID：IDM_OPENDIBPIC caption：打开

单击事件：弹出文件对话框，打开.bmp位图文件，并显示

b. ID：IDM_MARK_DIBPIC caption：标记

单击事件：在DIB位图中透明地添加天极网logo

工程中还包含下列位图资源：

（1）IDB_LOADED_BITMAP：要加载的位图资源

（2）IDB_YESKY_BITMAP：天极网logo

后续篇章将集中在对4个子菜单单击事件消息处理函数的讲解，下面的代码是整个对话框类CBitMapExampleDlg的消息映射：

```
BEGIN_MESSAGE_MAP(CBitMapExampleDlg, CDialog)
//{{AFX_MSG_MAP(CBitMapExampleDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_COMMAND(IDM_LOADDDBPIC, OnLoadddbpic)
ON_COMMAND(IDM_MARK_DDBPIC, OnMarkDdbpic)
ON_COMMAND(IDM_OPENDIBPIC, OnOpendibpic)
ON_COMMAND(IDM_MARK_DIBPIC,OnMarkDibpic) //}}AFX_MSG_MAP
END_MESSAGE_MAP()
```

**3. DDB位图编程**先看DDB加载按钮的单击事件代码：

```
void CBitMapExampleDlg::OnLoadddbpic() 
{
1:　CBitmap bmpDraw;
2: bmpDraw.LoadBitmap( IDB_LOADED_BITMAP );//装入要加载的DDB位图 
3: BITMAP bmpInfo;
4: bmpDraw.GetBitmap( &bmpInfo ); //获取要加载DDB位图的尺寸 
5: CDC memDC;//定义一个兼容DC
6: CClientDC dc( this );
7: memDC.CreateCompatibleDC( &dc );//创建兼容DC
8: CBitmap* pbmpOld = memDC.SelectObject( &bmpDraw );//保存原有DDB，并选入新DDB入DC

9: dc.BitBlt( 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &memDC, 0, 0, SRCCOPY );

10:　memDC.SelectObject( pbmpOld );//选入原DDB
}
```

上述代码将产生如图1所示的效果，位图被安置在对话框（0,0）坐标开始的位置上。

![](http://cimg.163.com/catchpic/F/F8/F8F94958CE8F80443F1BDF1E43BFEA87.jpg)

图1 加载DDB位图资源

我们来逐行解析上述代码是怎样产生图1的效果的。

第1、2行定义了一个CBitmap对象，并调用其成员函数LoadBitmap加载工程中的位图资源IDB_LOADED_BITMAP。第3、4行定义了BITMAP结构体的实例并调用CBitmap的成员函数GetBitmap获得位图信息，BITMAP结构体定义在头文件中，其形式为：

```
/* Bitmap Header Definition */
typedef struct tagBITMAP
{
LONG bmType; //必需为0
LONG bmWidth; //位图的宽度(以像素为单位)
LONG bmHeight; //位图的高度(以像素为单位)
LONG bmWidthBytes; //每一扫描行所需的字节数，应是偶数
WORD bmPlanes; //色平面数
WORD bmBitsPixel; //色平面的颜色位数
LPVOID bmBits; //指向存储像素阵列的数组
} BITMAP, *PBITMAP, NEAR *NPBITMAP, FAR *LPBITMAP;
```

第5~8行的作用是：构建一个CDC对象，调用CDC::CreateCompatibleDC创建一个兼容的内存设备上下文，接着调用CDC::SelectObject将DDB选入内存设备上下文中。

第9行调用函数CDC::BitBlt绘制位图，CDC::BitBlt的原型为：

`CDC::BitBlt(int x, int y, int nWidth, int nHeight, CDC *pSrcDC, int xSrc, int ySrc, DWORD dwRop)`
CDC::BitBlt执行的操作为将源DC中位图复制到目的DC中。其中前四个参数为目的区域的坐标（x,y）及长度和宽度（Width, nHeight），第五个参数是源DC指针，接下来的参数是源DC中的起始坐标，最后一个参数为光栅操作的类型。

第10行调用CDC::SelectObject把原来的DDB选入到内存设备上下文中并使新DDB脱离出来。

与CDC::BitBlt对应的还有另一个函数CDC::StretchBlt，它具有缩放功能，其原型为：

```
BOOL CDC::StretchBlt(int x, int y, int nWidth, int nHeight, CDC *pSrcDC, int
xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);
```

该函数把位图从源矩形拷贝到目的矩形中，如果源和目的矩形尺寸不同，那么将缩放位图的功能以适应目的矩形的大小。函数的大部分参数与BitBlt的相同，但多了两个参数nSrcWidth和nSrcHeight用来指定源矩形的宽和高。

如果我们将函数CBitMapExampleDlg::OnLoadddbpic() 中的第9行改为：

```
CRect clientRect;
GetClientRect(&clientRect); //获得对话框窗口的大小
dc.StretchBlt(0, 0, clientRect.right, clientRect.bottom, &memDC, 0, 0,
bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
```

则单击加载按钮后的对话框如图2所示，位图被拉伸至整个对话框的范围。

![](http://cimg.163.com/catchpic/4/4F/4F91CC4A105C69E77279A52F4FB2A9EE.jpg)

图2 拉伸位图

CDC::BitBlt和dc.StretchBlt函数中的dwRop参数较为有用，它定义光栅操作的类型。请看"DDB位图"父菜单下"标记"子菜单单击事件的消息处理函数代码：

```
void CBitMapExampleDlg::OnMarkDdbpic()
{
CBitmap bmpDraw;
bmpDraw.LoadBitmap(IDB_YESKY_BITMAP); //装入天极网logo DDB位图资源
BITMAP bmpInfo;
bmpDraw.GetBitmap(&bmpInfo); //获取天极网logo位图的尺寸 

CDC memDC; //定义一个兼容DC
CClientDC dc(this);
memDC.CreateCompatibleDC(&dc); //创建DC

CBitmap *pbmpOld = memDC.SelectObject(&bmpDraw);
//保存原有DDB，并选入天极网logo位图入DC
dc.BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &memDC, 0, 0, SRCAND);
memDC.SelectObject(pbmpOld); //选入原DDB 
}
```

单击该按钮后，将产生如图3的效果，天极网的logo被透明地添加到了位图中！

![](http://cimg.163.com/catchpic/A/AE/AE56DF89FB6B969070E26137BC6C5041.jpg)

图3 在DDB位图中加入天极网logo

能产生这个效果的原因在于我们在代码行：

`dc.BitBlt ( 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &memDC, 0, 0, SRCAND );`

```
中使用了参数SRCAND（不同于先前代码中SRCCOPY，它仅仅意味着复制源位图到目的位图），它的含义为源和目的间进行AND操作。我们不知道天极网的编辑同志是怎么为文章中的图片加logo的，有可能他们就使用了具有自动AND功能的图像加logo批处理软件。的确，我们可以利用例程中的原理写一个批处理软件，一次对一堆图片自动添加logo。

参数dwRop除了可以为SRCAND和SRCCOPY外，还可以有如下取值：

BLACKNESS：输出区域为黑色

DSTINVERT：反转目的位图 

MERGECOPY：用与操作把图案(Pattern)与源位图融合起来 

MERGEPAINT：用或操作把反转的源位图与目的位图融合起来 

NOTSRCCOPY：把源位图反转然后拷贝到目的地 

NOTSRCERASE：用或操作融合源和目的位图，然后再反转 

PATCOPY：把图案拷贝到目的位图中 

PATINVERT：用异或操作把图案与目的位图相融合 

PATPAINT：用或操作融合图案和反转的源位图，然后用或操作把结果与目的位图融合 

SRCERASE：先反转目的位图，再用与操作将其与源位图融合 

SRCINVERT：用异或操作融合源位图和目的位图 

SRCPAINT：用或操作融合源位图和目的位图 

WHITENESS：输出区域为白色

合理利用这些取值将帮助我们制作出特定要求的图像处理软件。

从上述实例我们可以看出，在VC中使用CBitmap类，必须将位图放入工程的资源中，并使用类 CBitmap的成员函数LoadBitmap加载之，再通过CDC类的成员函数BitBlt进行DC拷贝等操作达到显示的目的。CBitmap有显示的不足：

（1） 位图需要放入工程资源中，这将导致工程的可执行文件变大；

（2） 因为位图需放入工程资源中，而资源中不能无穷无尽地包含位图，应用程序无法自适应地选取其它位图，能使用的位图十分有限的；

（3） 类CBitmap只是DDB位图操作API的封装，不能独立于平台。

DIB位图则可以解决上述问题，其特点是以.BMP位图文件格式存储独立于平台的图像数据，下面我们来详细分析。
**4. DIB位图编程**4.1位图文件格式

先来分析DIB位图文件的格式。位图文件分为四部分： 

（1）位图文件头BITMAPFILEHEADER

位图文件头BITMAPFILEHEADER是一个结构体，长度为14字节，定义为：

```
typedef struct tagBITMAPFILEHEADER
{
WORD bfType; //文件类型，必须是0x424D，即字符串"BM"
DWORD bfSize; //文件大小，包括BITMAPFILEHEADER的14个字节
WORD bfReserved1; //保留字
WORD bfReserved2; //保留字
DWORD bfOffBits; //从文件头到实际的位图数据的偏移字节数
} BITMAPFILEHEADER;
```

（2）位图信息头BITMAPINFOHEADER

位图信息头BITMAPINFOHEADER也是一个结构体，长度为40字节，定义为：

```
typedef struct tagBITMAPINFOHEADER
{
DWORD biSize; //本结构的长度，为40
LONG biWidth; //图象的宽度，单位是象素
LONG biHeight; //图象的高度，单位是象素
WORD biPlanes; //必须是1
WORD biBitCount;
//表示颜色时要用到的位数，1(单色), 4(16色), 8(256色), 24(真彩色)
DWORD biCompression;
//指定位图是否压缩，有效的值为BI_RGB，BI_RLE8，BI_RLE4，BI_BITFIELDS等，BI_RGB表示不压缩
DWORD biSizeImage;
//实际的位图数据占用的字节数，即 biSizeImage=biWidth’ × biHeight，biWidth’是biWidth 按照4的整倍数调整后的结果 
LONG biXPelsPerMeter; //目标设备的水平分辨率，单位是每米的象素个数
LONG biYPelsPerMeter; //目标设备的垂直分辨率，单位是每米的象素个数
DWORD biClrUsed; //位图实际用到的颜色数，0表示颜色数为2biBitCount
DWORD biClrImportant; //位图中重要的颜色数，0表示所有颜色都重要
} BITMAPINFOHEADER;
```

（3）调色板Palette

调色板Palette针对的是需要调色板的位图，即单色、16色和256色位图。对于不以调色板方式存储的位图，则无此项信息。调色板是一个数组，共有biClrUsed个元素(如果该值为0，则有2biBitCount个元素)。数组中每个元素是一个RGBQUAD结构体，长度为4个字节，定义为：

```
typedef struct tagRGBQUAD
{
BYTE rgbBlue; //蓝色分量
BYTE rgbGreen; //绿色分量
BYTE rgbRed; //红色分量
BYTE rgbReserved; //保留值
} RGBQUAD;
```
```

```
```
（4）实际的位图数据ImageDate

对于用到调色板的位图，实际的图象数据ImageDate为该象素的颜色在调色板中的索引值；对于真彩色图，图象数据则为实际的R、G、B值：

a.单色位图：用1bit就可以表示象素的颜色索引值；

b.16色位图：用4bit可以表示象素的颜色索引值；

c. 256色位图：1个字节表示1个象素的颜色索引值； 

d.真彩色：3个字节表示1个象素的颜色R，G，B值。

此外，位图数据每一行的字节数必须为4的整倍数，如果不是，则需要补齐。奇怪的是，位图文件中的数据是从下到上（而不是从上到下）、从左到右方式存储的。
4.2位图的显示

Visual C++ MFC中没有提供一个专门的类来处理DIB位图，因此，为了方便地使用位图文件，我们有必要派生一个CDib类。类的源代码如下：

(1) CDib类的声明

```
// DIB.h：类CDib声明头文件
#ifndef __DIB_H__
#define __DIB_H__
#include 
class CDib
{
public:
CDib();
~CDib();

BOOL Load( const char * );
BOOL Save( const char * );
BOOL Draw( CDC *, int nX = 0, int nY = 0, int nWidth = -1, int nHeight = -1, int mode = SRCCOPY);
BOOL SetPalette( CDC * );

private:
CPalette m_Palette;
unsigned char *m_pDib, *m_pDibBits;
DWORD m_dwDibSize;
BITMAPINFOHEADER *m_pBIH;
RGBQUAD *m_pPalette;
int m_nPaletteEntries;
};
#endif
```

(2) CDib类的实现

```
// DIB.cpp：类CDib实现文件
#include "stdafx.h"
#include "DIB.h"

CDib::CDib()
{
m_pDib = NULL;
}

CDib::~CDib()
{
// 如果位图已经被加载，释放内存
if (m_pDib != NULL)
delete []m_pDib;
}
```

下面这个函数非常重要，其功能为加载位图，类似于CBitmap类的LoadBitmap函数：

```
BOOL CDib::Load(const char *pszFilename)
{
CFile cf;

// 打开位图文件
if (!cf.Open(pszFilename, CFile::modeRead))
return (FALSE);

// 获得位图文件大小，并减去BITMAPFILEHEADER的长度
DWORD dwDibSize;
dwDibSize = cf.GetLength() - sizeof(BITMAPFILEHEADER);

// 为DIB位图分配内存
unsigned char *pDib;
pDib = new unsigned char[dwDibSize];
if (pDib == NULL)
return (FALSE);

BITMAPFILEHEADER BFH;

// 读取位图文件数据
try
{
// 文件格式是否正确有效
if ( cf.Read(&BFH, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER) ||
BFH.bfType != ’MB’ || cf.Read(pDib, dwDibSize) != dwDibSize)
{
delete []pDib;
return (FALSE);
}
}
catch (CFileException *e)
{
e->Delete();
delete []pDib;
return (FALSE);
}

// delete先前加载的位图
if (m_pDib != NULL) 
delete m_pDib;

// 将临时Dib数据指针和Dib大小变量赋给类成员变量
m_pDib = pDib;
m_dwDibSize = dwDibSize;

// 为相应类成员变量赋BITMAPINFOHEADER和调色板指针
m_pBIH = (BITMAPINFOHEADER*)m_pDib;
m_pPalette = (RGBQUAD*) &m_pDib[sizeof(BITMAPINFOHEADER)];

// 计算调色板中实际颜色数量
m_nPaletteEntries = 1 << m_pBIH->biBitCount;
if (m_pBIH->biBitCount >8)
m_nPaletteEntries = 0;
else if (m_pBIH->biClrUsed != 0)
m_nPaletteEntries = m_pBIH->biClrUsed;

// 为相应类成员变量赋image data指针
m_pDibBits = &m_pDib[sizeof(BITMAPINFOHEADER) + m_nPaletteEntries * sizeof (RGBQUAD)];

// delete先前的调色板
if (m_Palette.GetSafeHandle() != NULL)
m_Palette.DeleteObject();

// 如果位图中存在调色板，创建LOGPALETTE 及CPalette
if (m_nPaletteEntries != 0)
{
LOGPALETTE *pLogPal = (LOGPALETTE*)new char[sizeof(LOGPALETTE) + m_nPaletteEntries *sizeof(PALETTEENTRY)];

if (pLogPal != NULL)
{
pLogPal->palVersion = 0x300;
pLogPal->palNumEntries = m_nPaletteEntries;

for (int i = 0; i < m_nPaletteEntries; i++)
{
pLogPal->palPalEntry[i].peRed = m_pPalette[i].rgbRed;
pLogPal->palPalEntry[i].peGreen = m_pPalette[i].rgbGreen;
pLogPal->palPalEntry[i].peBlue = m_pPalette[i].rgbBlue;
}

//创建CPalette并释放LOGPALETTE的内存
m_Palette.CreatePalette(pLogPal);
delete []pLogPal;
}
}

return (TRUE);
}

//函数功能：保存位图入BMP文件
BOOL CDib::Save(const char *pszFilename)
{
if (m_pDib == NULL)
return (FALSE);

CFile cf;
if (!cf.Open(pszFilename, CFile::modeCreate | CFile::modeWrite))
return (FALSE);

try
{
BITMAPFILEHEADER BFH;
memset(&BFH, 0, sizeof(BITMAPFILEHEADER));
BFH.bfType = ’MB’;
BFH.bfSize = sizeof(BITMAPFILEHEADER) + m_dwDibSize;
BFH.bfOffBits = sizeof(BITMAPFILEHEADER) + 
sizeof(BITMAPINFOHEADER) + m_nPaletteEntries *sizeof(RGBQUAD);

cf.Write(&BFH, sizeof(BITMAPFILEHEADER));
cf.Write(m_pDib, m_dwDibSize);
}
catch (CFileException *e)
{
e->Delete();
return (FALSE);
}
return (TRUE);
}
```

下面这个函数也非常重要，其功能为在pDC指向的CDC中绘制位图，起点坐标为(nX,nY)，绘制宽度和高度为nWidth、nHeight，最后一个参数是光栅模式：

```
BOOL CDib::Draw(CDC *pDC, int nX, int nY, int nWidth, int nHeight, int mode)
{
if (m_pDib == NULL)
return (FALSE);

// 获取位图宽度和高度赋值
if (nWidth == - 1)
nWidth = m_pBIH->biWidth;
if (nHeight == - 1)
nHeight = m_pBIH->biHeight;

// 绘制位图
StretchDIBits(pDC->m_hDC, nX, nY, nWidth, nHeight, 0, 0, m_pBIH->biWidth, m_pBIH->biHeight, m_pDibBits, (BITMAPINFO*)m_pBIH, BI_RGB, mode);

return (TRUE);
}

//函数功能：设置调色板
BOOL CDib::SetPalette(CDC *pDC)
{
if (m_pDib == NULL)
return (FALSE);

// 检查当前是否有一个调色板句柄，对于大于256色的位图，为NULL
if (m_Palette.GetSafeHandle() == NULL)
return (TRUE);

// 选择调色板，接着实施之，最后恢复老的调色板
CPalette *pOldPalette;
pOldPalette = pDC->SelectPalette(&m_Palette, FALSE);
pDC->RealizePalette();
pDC->SelectPalette(pOldPalette, FALSE);

return (TRUE);
}
```

从整个CDib类的代码中我们可以看出，DIB位图的显示需遵循如下步骤：

（1）读取位图，本类中使用pDib = new unsigned char[dwDibSize]为位图中的信息分配内存，另一种方法是调用API函数CreateDIBSection，譬如：

```
m_hBitmap = ::CreateDIBSection(pDC->GetSafeHdc(), 
(LPBITMAPINFO) m_lpBMPHdr, DIB_RGB_COLORS,
(LPVOID*) &m_lpDIBits, NULL, 0);
```

m_hBitmap定义为：

`HBITMAP m_hBitmap;`
（2）根据读取的位图信息，计算出调色板大小，然后创建调色板；

（3）调用CDib::SetPalette( CDC *pDC )设置调色板，需要用到CDC::SelectPalette及CDC::RealizePalette两个函数；

（4）调用CDib::Draw(CDC *pDC, int nX, int nY, int nWidth, int nHeight, int mode)函数绘制位图。在此函数中，真正发挥显示位图作用的是对StretchDIBits API函数的调用。StretchDIBits函数具有缩放功能，其最后一个参数也是光栅操作的模式。

下面给出DIB位图的打开及显示并在其中加入天极网logo的函数源代码。"DIB位图"父菜单下"打开"子菜单的单击事件消息处理函数为（其功能为打开位图并显示之）： 

```
void CBitMapExampleDlg::OnOpendibpic()
{
// 弹出文件对话框，让用户选择位图文件
CFileDialog fileDialog(TRUE, "*.BMP", NULL, NULL,"位图文件(*.BMP)|*.bmp;*.BMP|");
if (IDOK == fileDialog.DoModal())
{
// 加载位图并显示之
CDib dib;
if (dib.Load(fileDialog.GetPathName()))
{
CClientDC dc(this);
dib.SetPalette(&dc);
dib.Draw(&dc);
}
}
}
```

"DIB位图"父菜单下"标记"子菜单的单击事件消息处理函数为（其功能为给位图加上天极网logo）：

```
void CBitMapExampleDlg::OnMarkDibpic()
{
// 弹出文件对话框，让用户选择标记logo
CFileDialog fileDialog(TRUE, "*.BMP", NULL, NULL, "标记位图文件(*.BMP)|*.bmp;*.BMP|");
if (IDOK == fileDialog.DoModal())
{
// 加载标记logo位图并与目标位图相与
CDib dib;
if (dib.Load(fileDialog.GetPathName()))
{
CClientDC dc(this);
dib.SetPalette(&dc);
dib.Draw(&dc, 0, 0, - 1, - 1, SRCAND);
}
}
}
```

图4显示了DIB位图加载天极网logo后的效果，要好于图3中加天极网logo后的DDB位图。图4显示的是真彩色位图相互与的结果，而图3中的图像颜色被减少了。
```
```

![](http://cimg.163.com/catchpic/E/E6/E683A3F6D12F0BFB25E9FC01B42FAC52.jpg)

图4 在DIB位图中加入天极网logo

**5. 结束语**
本文介绍了位图及调色板的概念，并讲解了DDB位图与DIB位图的区别。在此基础上，本文以实例讲解了DDB位图和DIB位图的操作方式。DDB位图的处理相对比较简单，对于DIB位图，我们需要定义一个MFC所没有的新类CDib，它屏蔽位图信息的读取及调色板创建的技术细节，应用程序可以方便地使用之。

本文中的所有程序在Visual C++6.0及Windows XP平台上调试通过。


