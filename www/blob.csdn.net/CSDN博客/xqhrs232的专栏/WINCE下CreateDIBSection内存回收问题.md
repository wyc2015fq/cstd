# WINCE下CreateDIBSection内存回收问题 - xqhrs232的专栏 - CSDN博客
2013年03月15日 18:04:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1047
原文地址::[http://blog.csdn.net/wetnt/article/details/5942517](http://blog.csdn.net/wetnt/article/details/5942517)
相关网帖
1.CE6下使用CreateDIBSection( )用了DeleteObject还是删除不了对象----[http://bbs.csdn.net/topics/270062578](http://bbs.csdn.net/topics/270062578)
2.关于WINCE内存泄漏和清理系统内存----[http://bbs.csdn.net/topics/380162968](http://bbs.csdn.net/topics/380162968)
3.错误： 内存泄漏，当您使用 GetDC 方法和 ReleaseDC 方法的 CWnd 类版本----[http://support.microsoft.com/kb/819635/zh-cn](http://support.microsoft.com/kb/819635/zh-cn)
[CreateDIBSection](http://www.cnblogs.com/Jade2009/archive/2009/05/11/1454221.html) 函数在WinXP系列下是会自己回收内存空间，但在wince下却不会自己回收内存，需要手动回收。
在程序启动时，一次性申请空间更为合适。
/////////////////////////////////////////////////////////////////////
理解分辨率
我们常说的屏幕分辨率为640×480，刷新频率为70Hz，意思是说每行要扫描640个象素，一共有480行，每秒重复扫描屏幕70次。
理解调色板
有一个长宽各为200个象素，颜色数为16色的彩色图，每一个象素都用R、G、B三个分量表示。因为每个分量有256个级别，要用8位(bit)，即一个字节(byte)来表示，所以每个象素需要用3个字节。整个图象要用200×200×3，约120k字节，可不是一个小数目呀！如果我们用下面的方法，就能省的多。
因为是一个16色图，也就是说这幅图中最多只有16种颜色，我们可以用一个表：表中的每一行记录一种颜色的R、G、B值。这样当我们表示一个象素的颜色时，只需要指出该颜色是在第几行，即该颜色在表中的索引值。举个例子，如果表的第0行为255，0，0(红色)，那么当某个象素为红色时，只需要标明0即可。
让我们再来计算一下：16种状态可以用4位(bit)表示，所以一个象素要用半个字节。整个图象要用200×200×0.5，约20k字节，再加上表占用的字节为3×16=48字节.整个占用的字节数约为前面的1/6，省很多吧？
这张R、G、B的表，就是我们常说的调色板(Palette)，另一种叫法是颜色查找表LUT(Look
 Up Table)，似乎更确切一些。Windows位图中便用到了调色板技术。其实不光是Windows位图，许多图象文件格式如pcx、tif、gif等都用到了。所以很好地掌握调色板的概念是十分有用的。
有一种图，它的颜色数高达256×256×256种，也就是说包含我们上述提到的R、G、B颜色表示方法中所有的颜色，这种图叫做真彩色图(true
 color)。真彩色图并不是说一幅图包含了所有的颜色，而是说它具有显示所有颜色的能力，即最多可以包含所有的颜色。表示真彩色图时，每个象素直接用R、G、B三个分量字节表示，而不采用调色板技术。原因很明显：如果用调色板，表示一个象素也要用24位，这是因为每种颜色的索引要用24位(因为总共有224种颜色，即调色板有224行)，和直接用R，G，B三个分量表示用的字节数一样，不但没有任何便宜，还要加上一个256×256×256×3个字节的大调色板。所以真彩色图直接用R、G、B三个分量表示，它又叫做24位色图。
**bmp****文件格式**
介绍完位图和调色板的概念，下面就让我们来看一看Windows的位图文件(.bmp文件)的格式是什么样子的。
bmp文件大体上分成四个部分，如图1.3所示。
实际的位图数据ImageDate
**图1.3     Windows位图文件结构示意图**
第一部分为位图文件头**BITMAPFILEHEADER**，是一个结构，其定义如下：
typedef struct tagBITMAPFILEHEADER {
WORD           bfType;
DWORD bfSize;
WORD           bfReserved1;
WORD           bfReserved2;
DWORD bfOffBits;
} BITMAPFILEHEADER;
这个结构的长度是固定的，为14个字节(WORD为无符号16位整数，DWORD为无符号32位整数)，各个域的说明如下：
**bfType**
指定文件类型，必须是0x424D，即字符串“BM”，也就是说所有.bmp文件的头两个字节都是“BM”。
**bfSize**
指定文件大小，包括这14个字节。
**bfReserved1，bfReserved2     **
为保留字，不用考虑
**bfOffBits**
为从文件头到实际的位图数据的偏移字节数，即图1.3中前三个部分的长度之和。
第二部分为位图信息头**BITMAPINFOHEADER**，也是一个结构，其定义如下：
typedef struct tagBITMAPINFOHEADER{
DWORD  biSize;
LONG            biWidth;
LONG            biHeight;
WORD           biPlanes;
WORD           biBitCount
DWORD  biCompression;
DWORD  biSizeImage;
LONG            biXPelsPerMeter;
LONG            biYPelsPerMeter;
DWORD  biClrUsed;
DWORD  biClrImportant;
} BITMAPINFOHEADER;
这个结构的长度是固定的，为40个字节(LONG为32位整数)，各个域的说明如下：
**biSize**
指定这个结构的长度，为40。
**biWidth**
指定图象的宽度，单位是象素。
**biHeight**
指定图象的高度，单位是象素。
**biPlanes**
必须是1，不用考虑。
**biBitCount**
指定表示颜色时要用到的位数，常用的值为1(黑白二色图), 4(16色图), 8(256色), 24(真彩色图)(新的.bmp格式支持32位色，这里就不做讨论了)。
**biCompression**
指定位图是否压缩，有效的值为BI_RGB，BI_RLE8，BI_RLE4，BI_BITFIELDS(都是一些Windows定义好的常量)。要说明的是，Windows位图可以采用RLE4，和RLE8的压缩格式，但用的不多。我们今后所讨论的只有第一种不压缩的情况，即biCompression为BI_RGB的情况。
**biSizeImage**
指定实际的位图数据占用的字节数，其实也可以从以下的公式中计算出来：
biSizeImage=biWidth’ × biHeight
要注意的是：上述公式中的biWidth’必须是4的整倍数(所以不是biWidth，而是biWidth’，表示大于或等于biWidth的，最接近4的整倍数。举个例子，如果biWidth=240，则biWidth’=240；如果biWidth=241，biWidth’=244)。
如果biCompression为BI_RGB，则该项可能为零
**biXPelsPerMeter**
指定目标设备的水平分辨率，单位是每米的象素个数，关于分辨率的概念，我们将在第4章详细介绍。
**biYPelsPerMeter**
指定目标设备的垂直分辨率，单位同上。
**biClrUsed**
指定本图象实际用到的颜色数，如果该值为零，则用到的颜色数为2biBitCount。
**biClrImportant**
指定本图象中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的。
第三部分为调色板**Palette**，当然，这里是对那些需要调色板的位图文件而言的。有些位图，如真彩色图，前面已经讲过，是不需要调色板的，BITMAPINFOHEADER后直接是位图数据。
调色板实际上是一个数组，共有biClrUsed个元素(如果该值为零，则有2biBitCount个元素)。数组中每个元素的类型是一个RGBQUAD结构，占4个字节，其定义如下：
typedef struct tagRGBQUAD {
BYTE    rgbBlue; //该颜色的蓝色分量
BYTE    rgbGreen; //该颜色的绿色分量
BYTE    rgbRed; //该颜色的红色分量
BYTE    rgbReserved; //保留值
} RGBQUAD;
第四部分就是实际的图象数据了。对于用到调色板的位图，图象数据就是该象素颜在调色板中的索引值。对于真彩色图，图象数据就是实际的R、G、B值。下面针对2色、16色、256色位图和真彩色位图分别介绍。
对于2色位图，用1位就可以表示该象素的颜色(一般0表示黑，1表示白)，所以一个字节可以表示8个象素。
对于16色位图，用4位可以表示一个象素的颜色，所以一个字节可以表示2个象素。
对于256色位图，一个字节刚好可以表示1个象素。
对于真彩色图，三个字节才能表示1个象素，哇，好费空间呀！没办法，谁叫你想让图的颜色显得更亮丽呢，有得必有失嘛。
要注意两点：
(1)每一行的字节数必须是4的整倍数，如果不是，则需要补齐。这在前面介绍biSizeImage时已经提到了。
(2)一般来说，.bMP文件的数据从下到上，从左到右的。也就是说，从文件中最先读到的是图象最下面一行的左边第一个象素，然后是左边第二个象素……接下来是倒数第二行左边第一个象素，左边第二个象素……依次类推 ，最后得到的是最上面一行的最右一个象素。
下面的函数将pBuffer指向的内存块中的位图数据写入文件中，lBufferLen参数为pBuffer指向的内存块的大小，注意必须先指定位图的BITMAPFILEHEADER结构和BITMAPINFOHEADER结构。
STDMETHODIMP CSampleGrabberCallback::BufferCB(double time,BYTE* pBuffer,long lBufferLen)
{
    if(!g_bSnap)
        return E_FAIL;
    BOOL bWrite=FALSE;
    HANDLE hFile=CreateFile("E://Test.bmp",GENERIC_WRITE,
        FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return E_FAIL;
}
//首先初始化位图文件头结构（BITMAPFILEHEADER），并将其写入文件。
    BITMAPFILEHEADER bmpFileHeader;
    //memset(&bmpFileHeader,0,sizeof(bmpFileHeader));
    ZeroMemory(&bmpFileHeader,sizeof(bmpFileHeader));
    bmpFileHeader.bfType='MB';
    bmpFileHeader.bfSize=sizeof(bmpFileHeader)+lBufferLen+sizeof(BITMAPINFOHEADER);
    bmpFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
    DWORD dwWritten=0;
    bWrite=WriteFile(hFile,&bmpFileHeader,sizeof(bmpFileHeader),&dwWritten,NULL);
    if(!bWrite)
    {
        MessageBox(0,TEXT("fail to write"),TEXT("Error"),MB_OK);
    }
//初始化BITMAPINFOHEADER结构并将其写入文件。
    //VIDEOINFOHEADER*viInfoHeader=(VIDEOINFOHEADER*) g_media_type.pbFormat;
    //FreeMediaType(g_media_type);
    BITMAPINFOHEADER bmpInfoHeader;
    ZeroMemory(&bmpInfoHeader,sizeof(bmpInfoHeader));
    //memset(&bmpInfoHeader,0,sizeof(bmpInfoHeader));
    bmpInfoHeader.biSize=sizeof(bmpInfoHeader);
    bmpInfoHeader.biWidth=lWidth;
    bmpInfoHeader.biHeight=lHeight;
    bmpInfoHeader.biPlanes=1;
    bmpInfoHeader.biBitCount=16;//???24 8
    dwWritten=0;
    bWrite=WriteFile(hFile,&bmpInfoHeader,sizeof(bmpInfoHeader),&dwWritten,NULL);
    if(!bWrite)
    {
        MessageBox(0,TEXT("fail to write"),TEXT("Error"),MB_OK);
    }
//最后将位图的主要数据写入文件。
    dwWritten=0;
    bWrite=WriteFile(hFile,pBuffer,lBufferLen,&dwWritten,NULL);
    if(!bWrite)
    {
        MessageBox(0,TEXT("fail to write"),TEXT("Error"),MB_OK);
    }
    CloseHandle(hFile);
    CWnd* pMainWnd=theApp.GetMainWnd();
    CDfgDlg* pDfg=(CDfgDlg*)pMainWnd;
    HWND hwnd=pDfg->m_picture.GetSafeHwnd();
    RECT rc;
    ::GetWindowRect(hwnd,&rc);
    long lStillWidth=rc.right-rc.left;
    long lStillHeight=rc.bottom-rc.top;
    HDC hdcStill=GetDC(hwnd);
    PAINTSTRUCT ps;
    BeginPaint(hwnd,&ps);
SetStretchBltMode(hdcStill,COLORONCOLOR);
 StretchDIBits(hdcStill,0,0,lStillWidth,lStillHeight,0,0,lWidth,lHeight,pBuffer,(BITMAPINFO*)&bmpInfoHeader,DIB_RGB_COLORS,SRCCOPY);
    EndPaint(hwnd,&ps);
    ReleaseDC(hwnd,hdcStill);
    g_bSnap=!g_bSnap;
    return S_OK;   
}
关于CreateDIBSection函数：
**HBITMAP CreateDIBSection(**** HDC*** hdc***,                 **// handle to DC** CONST BITMAPINFO****pbmi***, **// bitmap data** UINT*** iUsage***,             **// data type indicator** VOID*****ppvBits***,          **// bit values** HANDLE*** hSection***,         **// handle to file mapping object** DWORD*** dwOffset    *// offset to bitmap bit values**);**
CreateDIBSection函数会根据位图结构信息（*pbmi*）分配内存空间，你不用为它分配内存，这块内存也不需要你释放，系统会自己释放的。
然后将位图中的图像数据读入这个内存地址，显示即可。
  LPBYTE   lpBits; 
  HBITMAP   hBmp=::CreateDIBSection(dcMem.m_hDC,lpBitmap,DIB_PAL_COLORS,   &lpBits,NUL      L,0);
//将图像数据填充到得到的内存地址中
  file.ReadHuge(lpBits,dwBitlen);   
  pDC->StretchBlt(0,0,bmp.bmWidth,bmp.bmHeight,&dcMem,0,0,   
  bmp.bmWidth,bmp.bmHeight,SRCCOPY);  
首先让我们检查一下如何简化CreateDIBSection，并正确地使用它。首先，把最後两个参数hSection和dwOffset，分别设定为NULL和0，我将在本章最後讨论这些参数的用法。第二，仅在fColorUse参数设定为DIB_
 PAL_COLORS时，才使用hdc参数，如果fColorUse为DIB_RGB_COLORS（或0），hdc将被忽略（这与CreateDIBitmap不同，hdc参数用於取得与DDB相容的设备的色彩格式，CreateDIBitmap创建的是DDB（设备相关位图，CreateDIBSection创建设备无关位图），因此必须指定与位图所关联的设备，即hdc，位图根据hdc所代表的设备来取得位图的色彩格式）。
因此，CreateDIBSection最简单的形式仅需要第二和第四个参数。第二个参数是指向BITMAPINFO结构的指标，
BITMAPINFOHEADER         bmih ;
BYTE                           * pBits ;
HBITMAP                          hBitmap ;
现在初始化BITMAPINFOHEADER结构的栏位
bmih->biSize                  = sizeof (BITMAPINFOHEADER) ;
bmih->biWidth                 = 384 ;
bmih->biHeight                = 256 ;
bmih->biPlanes                = 1 ;
bmih->biBitCount              = 24 ;
bmih->biCompression           = BI_RGB ;
bmih->biSizeImage             = 0 ;
bmih->biXPelsPerMeter         = 0 ;
bmih->biYPelsPerMeter         = 0 ;
bmih->biClrUsed               = 0 ;
bmih->biClrImportant          = 0 ;
在基本准备後，我们呼叫该函式：
hBitmap = CreateDIBSection (NULL, (BITMAPINFO *)  &bmih, 0, &pBits, NULL, 0) ;
这是函式呼叫所做的：CreateDIBSection检查BITMAPINFOHEADER结构并配置足够的记忆体块来载入DIB图素位元。（在这个例子里，记忆体块的大小为384×256×3位元组。）它在您提供的pBits参数中储存了指向此记忆体块的指标。
然而，我们还没有做完，点阵图图素是未初始化的。如果正在读取DIB档案，可以简单地把pBits参数传递给ReadFile函式并读取它们。或者可以使用一些程式码「人工」设定。
注意：使用CreateDIBSection函数获得的内存块指针（输出的第四个参数）所指向的地址中是没有内容的，我们必须向里面写入图像数据，然后才能够显示图像。
//====================================================
备注::
1.[CreateDIBSection](http://www.cnblogs.com/Jade2009/archive/2009/05/11/1454221.html) 函数在WinXP系列下是会自己回收内存空间，但在wince下却不会自己回收内存，需要手动回收。
在程序启动时，一次性申请空间更为合适。
不知道上面的结论是不是正确的，但我DEBUG了一下，使用DeleteObject(
 )函数删除CreateDIBSection函数创建的HBITMAP对象后，其内存空间是没被置0x00/0xff。并且下次重新分配的地址空间跟上次的不一样。按这样的话，就是DeleteObject(
 )函数删除后，对象的内存还是不能被回收！！！但如果内存负责CreateDIBSection函数创建的这一块的内存管理，前后地址不一样也情有可原！！！
<应该是有释放的，就看DeleteObject有没执行成功了，成功了的话对应的内存空间会被置为0X00,看到的地址不一样那时是HBITMAP分配的指针地址>
 2013--3--20 上面这个结论是错误的，本人亲自实验过是可以释放的，不需要人为的去释放！！！

2.[http://bbs.csdn.net/topics/380234897](http://bbs.csdn.net/topics/380234897)
1.把hBmp 释放与 DC释放 顺序调换。因为你先释放bitmap的时候 ，可能是不成功的，因为你的bmp还在DC内存里面，处于被占用状态。
2.释放时候使用Win32API 释放HDC，不要用CDC对象。这个是老问题了，wince的GDI内存泄漏bug。
上面这段话不知道真假，明天试一下看看！！！试了一下，觉得跟hBmp 释放与 DC释放 顺序无关！！！
3.[http://www.idcrx.com/zhishiku/windowszhuanqu/2012/0603/167062.html](http://www.idcrx.com/zhishiku/windowszhuanqu/2012/0603/167062.html)
找到问题所在了,CreateDIBSection所引用的内存是No
 Cache的,所以非常的慢,
new出来的内存是可以Cache的,所以很快
,如果解决还是不知道,

