# CxImage的几种应用举例 - xqhrs232的专栏 - CSDN博客
2012年06月29日 15:26:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1004
原文地址::[http://www.cnblogs.com/thankgoodness/articles/1653094.html](http://www.cnblogs.com/thankgoodness/articles/1653094.html)
相关网帖
1.[CxImage类库使用说明](http://blog.csdn.net/jwybobo2007/article/details/2156730)----[http://blog.csdn.net/jwybobo2007/article/details/2156730](http://blog.csdn.net/jwybobo2007/article/details/2156730)
下面介绍应用它进行图像类型转换的方式：
1.从一种图像文件类型转换为另一种文件类型(convert from a format to another)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif) CxImage  
 image;   // 定义一个CxImage对象
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// 从bmp文件转换为jpg文件(bmp
 -> jpg)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)image.Load("image.bmp",
 CXIMAGE_FORMAT_BMP);   //先装载bmp文件,需要指定文件类型
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// 判断加载的bmp文件是否存在。
if (image.IsValid())...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// Returns
 true if the image has 256 colors   and a linear grey scale palette.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if(!image.IsGrayScale())
 image.IncreaseBpp(24);   // param nbit: 4, 8, 24
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    
 image.SetJpegQuality(99);                // 设置图像的压缩质量参数(从0到100，数值越大，质量越高)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    
 image.Save("image.jpg",CXIMAGE_FORMAT_JPG);          // 把压缩后的图像以jpg文件类型保存起来。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// 从png文件转换为tif文件（png
 -> tif）
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)image.Load("image.png",
 CXIMAGE_FORMAT_PNG);
if (image.IsValid()){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     image.Save("image.tif",CXIMAGE_FORMAT_TIF);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
2。加载程序资源图像(load an image resource)
即从程序的资源图像中构建CxImage对象,有如下几种方式：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// Load
 the resource IDR_PNG1 from the PNG resource type
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CxImage* newImage = new CxImage();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)newImage->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_PNG1),
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)                       "PNG"),CXIMAGE_FORMAT_PNG);
或者
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//Load
 the resource IDR_JPG1 from DLL
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CxImage* newImage = new CxImage();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)HINSTANCE hdll=LoadLibrary("imagelib.dll");
if (hdll){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     HRSRC
 hres=FindResource(hdll,MAKEINTRESOURCE(IDR_JPG1),"JPG");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     newImage->LoadResource(hres,CXIMAGE_FORMAT_JPG,hdll);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     FreeLibrary(hdll);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
或者
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//Load
 a bitmap resource;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)HBITMAP
 bitmap = ::LoadBitmap(AfxGetInstanceHandle(),
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)                              
 MAKEINTRESOURCE(IDB_BITMAP1)));
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CxImage *newImage = new CxImage();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)newImage->CreateFromHBITMAP(bitmap);
3。在内存缓冲中的图像类型转换
（1）把内存缓冲中的数据解码成一个Image对象(decode an image from memory)
有如下几种方式：
------
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CxImage image((BYTE*)buffer,size,image_type);//把内存缓冲buffer中的数据构造成Image对象
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//或:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CxMemFile memfile((BYTE*)buffer,size); // 显式使用CxMemFile对象
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CxImage
 image(&memfile,image_type);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//或：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CxMemFile memfile((BYTE*)buffer,size);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CxImage* image = new CxImage();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)image->Decode(&memfile,type);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
============
（2）把Image编码存放到内存缓冲中（encode an image in memory）
--------
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)long size=0;//得到图像大小
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)BYTE* buffer=0;//存储图像数据的缓冲
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)image.Encode(buffer,size,image_type);//把image对象中的图像以type类型数据copy到buffer
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)...
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)free(buffer);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)或:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CxMemFile memfile;            // 显式使用CxMemFile对象
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)memfile.Open();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)image.Encode(&memfile,image_type);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)BYTE* buffer = memfile.GetBuffer();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)long size = memfile.Size();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)...
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)free(buffer);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
4。处理系统粘贴板中的图像（copy/paste an image）
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//copy（到粘贴板）
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)HANDLE
 hDIB = image->CopyToHandle();
if (::OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd())) {
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)if(::EmptyClipboard()) {
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)if (::SetClipboardData(CF_DIB,hDIB) == NULL
 ) {
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            
 AfxMessageBox( "Unable to set Clipboard data" );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}    
 }     }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CloseClipboard();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//paste（从粘贴板粘贴出来）
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)HANDLE
 hBitmap=NULL;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CxImage *newima = new CxImage();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)if (OpenClipboard())
 hBitmap=GetClipboardData(CF_DIB);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)if (hBitmap)
 newima->CreateFromHANDLE(hBitmap);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CloseClipboard();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
5。在picture box中显示一个png格式的文件
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)HBITMAP m_bitmap = NULL;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CxImage image("myfile.png",
 CXIMAGE_FORMAT_PNG);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)...
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_bitmap = image.MakeBitmap(m_picture.GetDC()->m_hDC);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_picture.SetBitmap(m_bitmap);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)...
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)if (m_bitmap)
 DeleteObject(m_bitmap);
四。其它
一 个CxImage对象是一个扩展了的位图。作者只是在位图结构上添加了一些起存储信息作用的成员变量。一个CxImage对象（同时）也是一组层。每个层 只有在需要时才会分配相应的缓冲区。CxImage::pDib代表着背景图像，CxImage::pAlpha代表着透明层，CxImage:: pSelection代表着被选中的层，被用来创建图像处理时让用户感兴趣的区域。在这三个特殊层面的基础上，你可以增加一些额外的层，这些层可以存储在 CxImage::pLayers中。一般说来，层是一个完整的CxImage对象。因此，你可以构造很复杂的嵌套层。下面是CxImage的一些成员变
 量:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class CxImage
{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)...
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)protected:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)void* pDib;            //包含文件头，调色板等等
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)BITMAPINFOHEADER
 head; //标准的文件头（位图）
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)CXIMAGEINFO
 info;      //扩展了的信息
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)BYTE* pSelection;      //用户选中的区域
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)BYTE* pAlpha;          //alpha通道
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)CxImage** pLayers;     //通用层
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
typedef struct tagCxImageInfo {
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)DWORD   
 dwEffWidth;       //DWORD 扫描线宽
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)BYTE*   
 pImage;           //图像位数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)void*   
 pGhost;           //if this is a ghost, pGhost point to the body
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)DWORD   
 dwType;           //原图像的格式
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)char    
 szLastError[256]; //出错信息
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)long    
 nProgress;        //监视循环的次数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)long    
 nEscape;          //跳出标志
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)long    
 nBkgndIndex;      //GIF, PNG, MNG格式使用
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)RGBQUAD
 nBkgndColor;      //RGB三原色透明度
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)BYTE    
 nQuality;         //JPEG格式使用
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)long    
 nFrame;           //TIF, GIF, MNG使用 :实际的帧数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)long    
 nNumFrames;       //TIF, GIF, MNG使用 :帧总数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)DWORD   
 dwFrameDelay;     //GIF, MNG使用
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)long    
 xDPI;             //水平分辨率
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)long    
 yDPI;             //垂直分辨率
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)RECT    
 rSelectionBox;    //选中的矩形区
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)BYTE    
 nAlphaMax;        //阴影的最大不透明度
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)bool    
 bAlphaPaletteEnabled;  //如果调色板中有Alpha通道则为真
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)bool    
 bEnabled;         //打开绘图函数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)long    
 xOffset;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)long    
 yOffset;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)DWORD   
 dwEncodeOption;   //一些编码选项
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)RGBQUAD
 last_c;           //一些优化选项
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)BYTE    
 last_c_index;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)bool    
 last_c_isvalid;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)long    
 nNumLayers;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)DWORD   
 dwFlags;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)} CXIMAGEINFO;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
