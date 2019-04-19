# 将HDC保存为BMP文件 - xqhrs232的专栏 - CSDN博客
2012年10月14日 22:37:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：663
原文地址::[http://blog.csdn.net/norains/article/details/4594514](http://blog.csdn.net/norains/article/details/4594514)
//========================================================================
//TITLE:
//    将HDC保存为BMP文件
//AUTHOR:
//    norains
//DATE:
//    Friday 25-September-2009
//Environment:
//    WINDOWS CE 5.0
//========================================================================
    HDC在MSDN中的全称为：The handle of device context。通常，我们都是用来做相应的显示操作。
    熟悉WIN32的朋友对于其应该不会陌生，经常采用GetDC，GetWindowDC等等来获取其句柄。而用得最多的，可能就是BeginPaint，如：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- case WM_PAINT:  
- HDC hdc = BeginPaint(hWnd,&ps);  
-     ...  
-     EndPaint(hdc,&ps);  
- break;   
   使用起来非常简单，但如果想将其内容保存为普通的图像文档，可就没那么容易。确切地说，在只知道HDC句柄的情况下，我们是无法保存其内容的；但我们可以剑走偏锋，将HDC的内容写到一个缓存中，然后我们再保存该缓存的内容即可。
   听起来很简单，却又像很复杂，不是么？没关系，我们现在一步一步来。
   首先，我们需要一个HDC的句柄。如同前面所说，你可以有多种方法，比如GetDC，GetWindowDC，甚至是CreateDC。反正呢，你用什么方法我不管，我只要有一个HDC的句柄就好了。
   有了HDC的句柄，接下来我们所需要做的是，知道这HDC的大小，也就是宽度和长度。这个不难，我们只要简单地调用GetDeviceCaps，然后参数给予HORZRES或VERTRES即可：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- int iWidth = GetDeviceCaps(hdc,HORZRES);  
- int iHeight = GetDeviceCaps(hdc,VERTRES);  
    为什么要知道大小呢？因为我们要用它来创建缓存。而这缓存，说白了，其实就是一个BMP格式的数据结构而已。
    为了创建这个关键的缓存，我们必须调用CreateDIBSection函数，而该函数形参又用到BITMAPINFOHEADER，所以我们的一切，就先从填充该结构体开始。
    该结构体定义如下：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- typedefstruct tagBITMAPINFO  
- {   
-   BITMAPINFOHEADER bmiHeader;   
-   RGBQUAD bmiColors[1];   
- } BITMAPINFO;  
    结构体里面还有一个BITMAPINFOHEADER，其定义如下：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- typedefstruct tagBITMAPINFOHEADER   
- {   
- DWORD biSize;   
- LONG biWidth;   
- LONG biHeight;   
- WORD biPlanes;   
- WORD biBitCount   
- DWORD biCompression;   
- DWORD biSizeImage;   
- LONG biXPelsPerMeter;   
- LONG biYPelsPerMeter;   
- DWORD biClrUsed;   
- DWORD biClrImportant;   
- } BITMAPINFOHEADER;  
    这么多变量，是不是有点头晕？大可不必紧张，其实我们只需要填充其中几个，其它统统置为0即可：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- BITMAPINFO bmpInfo = {0};  
- bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  
- bmpInfo.bmiHeader.biWidth = iWidth;  
- bmpInfo.bmiHeader.biHeight = iHeight;  
- bmpInfo.bmiHeader.biPlanes = 1;  
- bmpInfo.bmiHeader.biBitCount = 24;  
    一切从最简单做起，对于BMP而言，最简单的自然是24位位图，这就是为什么biPlanes和biBitCount分别设置为1和24的原因。
    填充完BITMAPINFO结构，我们还是不能马上调用CreateDIBSection，因为形参中还有一个HDC。虽然我们可以直接采用已知的HDC句柄，但接下来还要将创建的HBITMAP和HDC相连接，所以我们还是先创建一个缓存DC：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- HDC hdcMem = CreateCompatibleDC(hdc);  
    一切准备就绪之后，就调用CreateDIBSection吧：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- BYTE *pData = NULL;  
- hBmp = CreateDIBSection(hdcMem,&bmpInfo,DIB_RGB_COLORS,reinterpret_cast<VOID **>(&pData),NULL,0);  
    pData是分配的一个内存空间，将来用来存储HDC的内容，只不过现在一切都是空的。如果你将这数据保存出来，你会发现一团漆黑。
    将HBITMAP和HDC结合：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- hOldObj = SelectObject(hdcMem, hBmp);  
    至此为止，我们前期工作已经准备就绪，我们只需要将HDC的内容用BitBlt绘制到缓存中即可：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- BitBlt(hdcMem,  
-        0,  
-        0,  
-        iWidth,  
-        iHeight,  
-        hdc,  
-        0,  
-        0,  
-        SRCCOPY);  
  这里其实还有一个小技巧，如果你是想绘制HDC的某个区域，你只需要用StretchBlt替代即可：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- StretchBlt(hdcMem,  
-             0,  
-             0,  
-             iWidth,  
-             iHeight,  
-             hdc,  
-             rcDC.left,  
-             rcDC.top,  
-             rcDC.right - rcDC.left + 1,  
-             rcDC.bottom - rcDC.top + 1,  
-             SRCCOPY);  
    喜欢追究问题的你，也许会发现，在调用该函数之后，pData所指向的内存缓冲区已经改变。是的，没错，这些改变的数据就是我们所需要的。接下来我们所需要做的仅仅是，将这数据按BMP文件的格式，保存下来即可。
    BMP文件格式其实很简单，最开始是文件头信息，然后是图片信息，接下来是数据。我们只需要按照这格式，顺序将数据写入即可。
    文件头信息和图片信息，微软已经为我们定义好了相应的结构体：
    BMP信息：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- typedefstruct tagBITMAPINFOHEADER   
- {   
- DWORD biSize;   
- LONG biWidth;   
- LONG biHeight;   
- WORD biPlanes;   
- WORD biBitCount   
- DWORD biCompression;   
- DWORD biSizeImage;   
- LONG biXPelsPerMeter;   
- LONG biYPelsPerMeter;   
- DWORD biClrUsed;   
- DWORD biClrImportant;   
- } BITMAPINFOHEADER;  
    文件头信息：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- typedefstruct tagBITMAPFILEHEADER   
- {  
- WORD bfType;   
- DWORD bfSize;   
- WORD bfReserved1;   
- WORD bfReserved2;   
- DWORD bfOffBits;   
- } BITMAPFILEHEADER;   
    我们首先填充这两个结构体数值：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- BITMAPINFOHEADER bmInfoHeader = {0};  
- bmInfoHeader.biSize = sizeof(BITMAPINFOHEADER);  
- bmInfoHeader.biWidth = iWidth;  
- bmInfoHeader.biHeight = iHeight;  
- bmInfoHeader.biPlanes = 1;  
- bmInfoHeader.biBitCount = 24;  
- 
- //Bimap file header in order to write bmp file
- BITMAPFILEHEADER bmFileHeader = {0};  
- bmFileHeader.bfType = 0x4d42;  //bmp  
- bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
- bmFileHeader.bfSize = bmFileHeader.bfOffBits + ((bmInfoHeader.biWidth * bmInfoHeader.biHeight) * 3); ///3=(24 / 8)
    接下来的事情，估计大家都轻车熟路了。创建文件，然后写入数据，保存，完毕。
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- HANDLE hFile = CreateFile(strFile.c_str(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);  
- DWORD dwWrite = 0;  
- WriteFile(hFile,&bmFileHeader,sizeof(BITMAPFILEHEADER),&dwWrite,NULL);  
- WriteFile(hFile,&bmInfoHeader, sizeof(BITMAPINFOHEADER),&dwWrite,NULL);  
- WriteFile(hFile,&vtData[0], vtData.size(),&dwWrite,NULL);  
- CloseHandle(hFile);  
    文章的最后，是参考源代码:
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- #ifdef UNICODE
-     #ifndef TSTRING
-         #define TSTRING std::wstring
-     #endif
- #else
-     #ifndef TSTRING
-         #define TSTRING std::string
-     #endif
- #endif
- 
- BOOL WriteBmp(const TSTRING &strFile,const std::vector<BYTE> &vtData,const SIZE &sizeImg);  
- BOOL WriteBmp(const TSTRING &strFile,HDC hdc);  
- BOOL WriteBmp(const TSTRING &strFile,HDC hdc,const RECT &rcDC);  
- 
- BOOL WriteBmp(const TSTRING &strFile,const std::vector<BYTE> &vtData,const SIZE &sizeImg)   
- {     
- 
-     BITMAPINFOHEADER bmInfoHeader = {0};  
-     bmInfoHeader.biSize = sizeof(BITMAPINFOHEADER);  
-     bmInfoHeader.biWidth = sizeImg.cx;  
-     bmInfoHeader.biHeight = sizeImg.cy;  
-     bmInfoHeader.biPlanes = 1;  
-     bmInfoHeader.biBitCount = 24;  
- 
- //Bimap file header in order to write bmp file
-     BITMAPFILEHEADER bmFileHeader = {0};  
-     bmFileHeader.bfType = 0x4d42;  //bmp  
-     bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
-     bmFileHeader.bfSize = bmFileHeader.bfOffBits + ((bmInfoHeader.biWidth * bmInfoHeader.biHeight) * 3); ///3=(24 / 8)
- 
- HANDLE hFile = CreateFile(strFile.c_str(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);  
- if(hFile == INVALID_HANDLE_VALUE)  
-     {  
- return FALSE;  
-     }  
- 
- DWORD dwWrite = 0;  
-     WriteFile(hFile,&bmFileHeader,sizeof(BITMAPFILEHEADER),&dwWrite,NULL);  
-     WriteFile(hFile,&bmInfoHeader, sizeof(BITMAPINFOHEADER),&dwWrite,NULL);  
-     WriteFile(hFile,&vtData[0], vtData.size(),&dwWrite,NULL);  
- 
- 
-     CloseHandle(hFile);  
- 
- return TRUE;  
- }   
- 
- 
- BOOL WriteBmp(const TSTRING &strFile,HDC hdc)  
- {  
- int iWidth = GetDeviceCaps(hdc,HORZRES);  
- int iHeight = GetDeviceCaps(hdc,VERTRES);  
-     RECT rcDC = {0,0,iWidth,iHeight};  
- 
- return WriteBmp(strFile,hdc,rcDC);    
- }  
- 
- BOOL WriteBmp(const TSTRING &strFile,HDC hdc,const RECT &rcDC)  
- {  
- BOOL bRes = FALSE;  
-     BITMAPINFO bmpInfo = {0};  
- BYTE *pData = NULL;  
-     SIZE sizeImg = {0};  
- HBITMAP hBmp = NULL;  
-     std::vector<BYTE> vtData;  
- HGDIOBJ hOldObj = NULL;  
- HDC hdcMem = NULL;  
- 
- //Initilaize the bitmap information 
-     bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  
-     bmpInfo.bmiHeader.biWidth = rcDC.right - rcDC.left;  
-     bmpInfo.bmiHeader.biHeight = rcDC.bottom - rcDC.top;  
-     bmpInfo.bmiHeader.biPlanes = 1;  
-     bmpInfo.bmiHeader.biBitCount = 24;  
- 
- //Create the compatible DC to get the data
-     hdcMem = CreateCompatibleDC(hdc);  
- if(hdcMem == NULL)  
-     {  
- goto EXIT;  
-     }  
- 
- //Get the data from the memory DC   
-     hBmp = CreateDIBSection(hdcMem,&bmpInfo,DIB_RGB_COLORS,reinterpret_cast<VOID **>(&pData),NULL,0);  
- if(hBmp == NULL)  
-     {  
- goto EXIT;  
-     }  
-     hOldObj = SelectObject(hdcMem, hBmp);  
- 
- //Draw to the memory DC
-     sizeImg.cx = bmpInfo.bmiHeader.biWidth;  
-     sizeImg.cy = bmpInfo.bmiHeader.biHeight;  
-     StretchBlt(hdcMem,  
-                 0,  
-                 0,  
-                 sizeImg.cx,  
-                 sizeImg.cy,  
-                 hdc,  
-                 rcDC.left,  
-                 rcDC.top,  
-                 rcDC.right - rcDC.left + 1,  
-                 rcDC.bottom - rcDC.top + 1,  
-                 SRCCOPY);  
- 
- 
-     vtData.resize(sizeImg.cx * sizeImg.cy * 3);  
-     memcpy(&vtData[0],pData,vtData.size());  
-     bRes = WriteBmp(strFile,vtData,sizeImg);  
- 
-     SelectObject(hdcMem, hOldObj);  
- 
- 
- EXIT:  
- if(hBmp != NULL)  
-     {  
-         DeleteObject(hBmp);  
-     }  
- 
- if(hdcMem != NULL)  
-     {  
-         DeleteDC(hdcMem);  
-     }  
- 
- return bRes;  
- }  
    一共有三个WriteBmp函数，使用上非常简单。
    比如，我想保存一个HDC，只需要简单地调用：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- HDC hdc = GetDC(NULL);  
- WriteBmp(TEXT("//NAND//DCSave.bmp"));  
- ReleaseDC(NULL,hdc);  
    如果想保存HDC的某一个部分，同样也很简单：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- HDC hdc = GetDC(NULL);  
- RECT rcDC = {0,0,100,100};  
- WriteBmp(TEXT("//NAND//DCSavePart.bmp"),rcDC);  
- ReleaseDC(NULL,hdc);  
    这个函数还能做到一个很有意思的功能，就是截取屏幕。对于屏幕来说，也是一个HDC，我们只要获取屏幕的HDC句柄，剩下的就没有什么难度了：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4594514#)[copy](http://blog.csdn.net/norains/article/details/4594514#)
- HDC hdc = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);  
- WriteBmp(TEXT("//NAND//ScreenCapture.BMP"),hdc);  
- DeleteDC(hdc);  
