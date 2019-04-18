# 碎片图像无缝拼合技术的VC++实现(转载) - 深之JohnChen的专栏 - CSDN博客

2005年12月27日 10:25:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1952标签：[vc++																[microsoft																[null																[colors																[c++																[照片](https://so.csdn.net/so/search/s.do?q=照片&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=colors&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)
个人分类：[图像处理与识别](https://blog.csdn.net/byxdaz/article/category/144582)

**[文章信息] 作者: 中国电子科技集团公司第二十二研究所青岛分所郎锐 时间: 2005-05-18 出处: 天极网 责任编辑: 方舟 [文章导读] 本文讲述了在VC++ 6.0下多幅碎片图像无缝拼合技术的实现原理和过程 **
**摘要**：本文讲述了在Microsoft Visual C++ 6.0下多幅碎片图像无缝拼合技术的实现原理和过程，并给出了部分关键代码以供参考。

**关键字**：Microsoft Visual C++ 6.0、图像、无缝拼合、位图文件 

**一、 引言**

　　在测绘、文博等行业经常会遇到这样一种情况：观测对象比较大，为保证分辨率又不能将其全部照下，只能进行局部照相，事后再将这些局部照相的重合部分去掉，拼合成一幅完整的图像。以前多采用手工拼合，误差较大，往往不能很好的实现无缝拼合，即使有少量的专业设备，成本也普遍较高。其实只需将照片通过扫描仪将其录入到计算机中，通过程序处理，完全能很好的实现多幅图像的无缝拼合，满足实际需要，而且对于文博行业中常会遇到的破碎的、不规则对象如古旧字画残片等也能很好的进行无缝拼合。本文就对针对该程序的实现原理及过程做了简要的介绍。

**二、 程序设计原理**

　　首先我们从实际出发，我们是通过进行局部照相的手段来保存整体的全部信息，而要保证这些局部照片所含的信息之和能包括整体的全部信息就必然的使每两幅邻近的图片有一部分交叠的部分，这样才能保证在将整体对象划分为若干局部照片而后再拼合成整体图像的过程中不遗漏任何信息，即该划分、拼合的整个过程是无损的。既然如此，我们只需能保证让两相邻图片的重叠部分能完全重合，那么我们也就能够肯定在此状态下的这两幅图像实现了无缝拼合。所以，问题就转换为使相邻图片的重叠部分能完全重合，而判断两相同的图像片段是否完全重叠可以用光栅掩码来进行直观的判断，比如我们可以采用"异或"的掩码，当相同位置上的两幅图片的像素相同时就为0即黑色，所以可以对两图片进行移动，只要重叠部分全黑，则表明此时两图像的重叠部分已准确的重合了，而此时也实现了图像的无缝拼合。此后只需再采用"或"的光栅掩码将合并后的图像显示出来，再通过拷屏等手段将其存盘即可。在实现拼合的全过程中主要涉及到图像的拖放、图像文件的读取及显示、光栅掩码、拷屏以及内存位图的保存等多种技术。接下来就对这些技术的具体应用进行介绍。

**三、 程序的具体实现**

　　在进行拼合之前，首先要将从扫描仪录入的图像从文件读取到内存中，并显示出来。由于在拼合时采取的光栅操作掩码是"异或"，所以为保持图像的原始面貌，可以在消息WM_ERASEBKGND 的响应函数中用PatBlt函数将整个客户区的初始背景设定为黑色：

……
pDC->PatBlt(0,0,rect.Width(),rect.Height(), BLACKNESS);
return TRUE;
　　读取位图文件可以用LoadImage函数来实现，m_sPath1指定了文件的路径，LR_LOADFROMFILE属性指定从文件中读取位图，返回值为该位图的句柄：

……
HBITMAP hbitmap;
hbitmap=(HBITMAP)LoadImage(AfxGetInstanceHandle(),
m_sPath1,
IMAGE_BITMAP,0,0,
LR_LOADFROMFILE|LR_CREATEDIBSECTION);
　　之后我们就可以创建一个和当前设备环境兼容的内存设备环境hMemDC1,并将刚才读取到内存的位图放置到该设备环境中：

hMemDC1=::CreateCompatibleDC(NULL);
SelectObject(hMemDC1,hbitmap);
::DeleteObject(hbitmap); //释放掉用过的位图句柄 
Invalidate();
　　至于位图的显示，由于需要频繁的拖动和其他处理，将其放置于OnDraw函数中较为合理，需要更新显示时只需显式地用Invalidate()函数刷新即可。OnDraw()中的显示位图部分最好用BitBlt函数来完成，该函数负责把hMemDC1中的位图放置到pDC页面中以完成内存页面的置换，其处理速度还是比较快的：

……
::BitBlt(pDC->m_hDC,m_nX1,m_nY1, m_nWidth1,m_nHeight1,
hMemDC1,0,0,m_dwRop);
……
　　函数中的m_dwRop变量对光栅操作码进行设置，初始为SRCINVERT即光栅异或操作，当拼合成功需要显示合并后的效果时再将其设定为SRCPAINT光栅或操作。

　　我们可以通过对鼠标消息响应函数的编程来实现在客户区内的位图拖放，按照Windows系统的习惯，首先在鼠标左键的响应函数中通过PtInRect()函数判断鼠标在左键按下时是否是落在位图上，如果是就可以在鼠标左键弹起之前将图片随鼠标拖动了，显然这部分应在WM_MOUSEMOVE消息的响应函数内编写代码：

……
if(m_bCanMove1==true) //在移动之前鼠标左键是在图片上点击的
{
　int dx=m_nOldX1-m_nX1; //计算鼠标距离图片原点的距离
　int dy=m_nOldY1-m_nY1;
　m_nX1=point.x-dx; //计算新的图片原点的坐标（客户区坐标）
　m_nY1=point.y-dy;
　Invalidate(); //更新视图
}
m_nOldX1=point.x; //保存上一次的鼠标位置 
m_nOldY1=point.y;
……
　　到此为止，可以运行程序对多幅碎片图像进行拼合了，用鼠标拖动一幅图像在另一幅图像边缘移动，由于采用了"异或"的光栅掩码，两幅图片交叠的地方颜色会发生改变，但只有完全重合时才会全黑，表明此时的拼合是无缝的，将掩码换为"或"即可将拼合后的图像显示出来。但此时只是保留在内存中，还要经过进一步的处理，才能将合并后的图像存盘保留。

　　首先要对合并后的图像所在的矩形框的位置、大小进行判断，可以用下面的类似代码来完成（本例同时最多能有4幅图像进行拼合）：

……
int temp1,temp2,x0,y0,x1,y1;
temp1=m_nX1<m_nX2?m_nX1:m_nX2;
if(m_sPath3!="")//如果有3幅图片参与拼合
{
　if(m_sPath4!="")//如果有4幅图片参与拼合
　　temp2=m_nX3<m_nX4?m_nX3:m_nX4;
　else
　　temp2=m_nX3;
　　x0=temp1<temp2?temp1:temp2;
}
else
　x0=temp1;
　……
　temp1=m_nX1+m_nWidth1>m_nX2+m_nWidth2?m_nX1+m_nWidth1:m_nX2+m_nWidth2;
　if(m_sPath3!="")
　{
　　if(m_sPath4!="")
　　　temp2=m_nX3+m_nWidth3>m_nX4+m_nWidth4?m_nX3+m_nWidth3:m_nX4+m_nWidth4;
　　else
　　　temp2=m_nX3+m_nWidth3;
　　　x1=temp1>temp2?temp1:temp2;
　}
　else
　　x1=temp1;
　　可以用类似的代码计算出y0和y1。在进行屏幕截图之前必须将由x0,y0,x1,y1构成的矩形由客户坐标转换成屏幕坐标，可以用ClientToScreen()函数来实现。下面是将屏幕指定区域以位图形式拷贝到内存中去的函数的主要实现代码：

HBITMAP CImageView::CopyScreenToBitmap(LPRECT lpRect)
{
　……
　// 确保选定区域不为空矩形 
　if(IsRectEmpty(lpRect)) 
　　return NULL;
　//为屏幕创建设备描述表 
　hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
　//为屏幕设备描述表创建兼容的内存设备描述表 
　hMemDC = CreateCompatibleDC(hScrDC); 
　……
　// 创建一个与屏幕设备描述表兼容的位图
　hBitmap = CreateCompatibleBitmap(hScrDC, lpRect->Width(),lpRect->Height());
　// 把新位图选到内存设备描述表中
　hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); 
　// 把屏幕设备描述表拷贝到内存设备描述表中
　BitBlt(hMemDC, 0, 0, lpRect->Width(),lpRect->Height, hScrDC, lpRect->left lpRect->top, SRCCOPY); 
　//得到屏幕位图的句柄
　hBitmap =(HBITMAP)SelectObject(hMemDC, hOldBitmap); 
　//清除
　DeleteDC(hScrDC);
　DeleteDC(hMemDC);
　……
　// 返回位图句柄 
　return hBitmap;
}

　　当把拼合后的区域拷贝到内存，并获取到该内存位图的句柄后可以将其通过剪贴板传送到其他图形处理软件中进行进一布的处理，也可以按照位图的格式直接将其保存成文件，为方便计，本例采用了后者。其实现过程主要是根据刚才获取到的内存位图句柄按格式填充BMP文件的信息头以及像素阵列，下面就结合实现的关键代码进行介绍：

首先获取设备描述表句柄，并用函数GetDeviceCaps()获取到当前显示分辨率下每个像素所占字节数，并据此计算出调色板的大小：

……
hDC = CreateDC("DISPLAY",NULL,NULL,NULL);
iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
DeleteDC(hDC);
if (iBits <= 1)
　wBitCount = 1; 
else if (iBits<= 4)
　wBitCount = 4; 
else if (iBits<= 8)
　wBitCount = 8;
else if (iBits <= 24)
　wBitCount = 24; //计算调色板大小
……
　　然后就可以设置位图信息头结构了，其中bi 是BITMAPINFOHEADER 结构的实例对象：

……
if (wBitCount <= 8)
　dwPaletteSize = (1<<wBitCount) *sizeof(RGBQUAD); //设置位图信息头结构
　GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
　bi.biSize = sizeof(BITMAPINFOHEADER);
　bi.biWidth = Bitmap.bmWidth;
　bi.biHeight = Bitmap.bmHeight;
　bi.biPlanes = 1;
　bi.biBitCount = wBitCount; 
　bi.biCompression = BI_RGB;
　bi.biSizeImage = 0; 
　bi.biXPelsPerMeter = 0;
　bi.biYPelsPerMeter = 0;
　bi.biClrUsed = 0;
　bi.biClrImportant = 0;
　　用GlobalAlloc()函数根据计算的结果为位图内容分配内存，并返回分配得到的内存句柄hDib，并用GetStockObject()来设置缺省状态下的调色板：

……
dwBmBitsSize = ((Bitmap.bmWidth*wBitCount+31)/32)*4*Bitmap.bmHeight; 
hDib = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
*lpbi = bi; // 处理调色板 
hPal = GetStockObject(DEFAULT_PALETTE); 
if (hPal)
{
　hDC = ::GetDC(NULL);
　hOldPal =SelectPalette(hDC, (HPALETTE)hPal, FALSE);
　RealizePalette(hDC); 
}
// 获取该调色板下新的像素值
GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,
(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,
(BITMAPINFO*)lpbi, DIB_RGB_COLORS);
//恢复调色板 
if (hOldPal)
{
　SelectPalette(hDC,(HPALETTE)hOldPal, TRUE); 
　RealizePalette(hDC);
　::ReleaseDC(NULL,hDC); 
}
……
　　最后的工作就是创建位图文件了，需要把设置好的位图文件头和像素点阵信息依次保存到文件中，其中bmfHdr 是BITMAPFILEHEADER位图文件头结构的实例对象，需要按照BMP位图的存盘格式对其进行设置：

……
fh = CreateFile(lpFileName, 
GENERIC_WRITE, 0, NULL,
CREATE_ALWAYS,
FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
// 设置位图文件头
bmfHdr.bfType = 0x4D42; // "BM"
dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize; 
bmfHdr.bfSize = dwDIBSize;
bmfHdr.bfReserved1 = 0;
bmfHdr.bfReserved2 = 0; 
bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER)+ dwPaletteSize; 
//写入位图文件头
WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
// 写入位图文件其余内容
WriteFile(fh, (LPSTR)lpbi, dwDIBSize,&dwWritten, NULL);
……
**四、程序的实例检测**

　　下面就通过一个实例--拼合一幅古代国画残片来对程序的拼合效果进行检测。其中图一到图三是拼合前的三幅古代国画残片，图四是经过本程序处理后存盘得到的经过无缝合成的图片。经过检测，拼合效果还是相当不错的，在碎片图像的衔接处根本没有接缝的存在：

|![](http://www.yesky.com/image20010518/301598.jpg)图一![](http://www.yesky.com/image20010518/301600.jpg)图二||
|----|----|
|![](http://www.yesky.com/image20010518/301601.jpg)图三![](http://www.yesky.com/image20010518/301602.jpg)图四||
**小结**：

　　本程序通过一个实例讲述了处理图片无缝拼合的一种实用方法，在测绘、勘察、文博等行业均有较大的应用潜力。在理解了程序的设计思路和编程思想的前提下，结合具体的实际需求，通过对本文具体代码的改动可以设计出更适合本单位实际情况的类似软件。另外，本文所讲述的截取并保存屏幕技术在类似程序的编制上也可以提供一定的参考。本程序在Windows 2000 Professional下，由Microsoft Visual C++ 6.0编译通过。            

