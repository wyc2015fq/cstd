# Windows Mobile下画透明PNG图片的解决方案----把PNG转成BMP再画 - xqhrs232的专栏 - CSDN博客
2010年07月31日 10:51:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1449
原文地址::[http://blog.csdn.net/hhygcy/archive/2009/04/29/4138195.aspx](http://blog.csdn.net/hhygcy/archive/2009/04/29/4138195.aspx)
最近一直看到有人在问这个PNG的东西,正好在工作的时候也用得到,就找了几个图片试了一下.从wince开始引入了IImage的支持,这样我们可以用IImage来load 这些图片. 大致的流程很多地方都有:
CoCreateInstance创建IImagingFactory, 然后用CreateImageFromFile创建IImage的接口,最后把这个IImage用draw的接口画到DC.但是问题在于这个Draw是没有设置transparent属性的? 这个东西应该如何操作呢? 在网上看到了一个解决方案. 就是把这个png图片转化成一张32位的bitmap,然后再透明的画到DC上.这里有一段调试成功的代码:
view plaincopy to clipboardprint?
HBITMAP LoadPngImage2 (HDC hdc, LPCTSTR filename)   
{   
    IImagingFactory* pImageFactory = 0;   
    IImage* pImage = 0;   
    ImageInfo imageInfo;   
    CoInitializeEx(0, COINIT_MULTITHREADED);   
    HBITMAP hBitmap = 0;   
    LPBYTE lpByte;   
    if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, 0, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&pImageFactory)))  
    {   
        if (SUCCEEDED(pImageFactory->CreateImageFromFile(filename, &pImage))&& SUCCEEDED(pImage->GetImageInfo(&imageInfo)))  
        {   
            HDC bmpDC = CreateCompatibleDC(hdc);   
            //LPBYTE lpByte;   
            BITMAPINFO *pbinfo ;   
            pbinfo = (BITMAPINFO *)calloc(1, sizeof(BITMAPINFO) + 4 * sizeof(INT)) ;  
            if(!pbinfo)   
                return FALSE ;   
            pbinfo->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);   
            pbinfo->bmiHeader.biWidth = imageInfo.Width ;   
            pbinfo->bmiHeader.biHeight = imageInfo.Height ;   
            pbinfo->bmiHeader.biPlanes = 1;   
            pbinfo->bmiHeader.biBitCount = 32;   
            pbinfo->bmiHeader.biCompression = BI_ALPHABITFIELDS;   
            pbinfo->bmiHeader.biSizeImage = 0 ;   
            pbinfo->bmiHeader.biXPelsPerMeter = 11811;   
            pbinfo->bmiHeader.biYPelsPerMeter = 11811;   
            pbinfo->bmiHeader.biClrUsed = 0;   
            pbinfo->bmiHeader.biClrImportant = 0;   
            int *pMask = (int*)&(pbinfo->bmiColors[0]) ;   
            *pMask++ = 0x00FF0000 ;   
            *pMask++ = 0x0000FF00 ;   
            *pMask++ = 0x000000FF ;   
            *pMask++ = 0xFF000000 ;   
            hBitmap = CreateDIBSection(NULL, pbinfo, DIB_RGB_COLORS, (void **)&lpByte, NULL, 0) ;  
            free(pbinfo) ;   
            if(!hBitmap || !lpByte)   
                return FALSE ;   
            RECT rect = {0, 0, imageInfo.Width, imageInfo.Height};   
            IBitmapImage *pBitmapImage;   
            BitmapData bitmapData;   
            bitmapData.Width = imageInfo.Width;   
            bitmapData.Height = imageInfo.Height;   
            bitmapData.PixelFormat = imageInfo.PixelFormat;   
            pBitmapImage = NULL;   
            pImageFactory->CreateBitmapFromImage(pImage, imageInfo.Width, imageInfo.Height, PIXFMT_32BPP_ARGB,  
                InterpolationHintDefault, &pBitmapImage);   
            pBitmapImage->LockBits(&rect, ImageLockModeRead,PIXFMT_32BPP_ARGB, &bitmapData);  
            //transferring the pixels   
            memcpy(lpByte, bitmapData.Scan0, imageInfo.Width * imageInfo.Height * 4);  
            pBitmapImage->UnlockBits(&bitmapData);   
            pBitmapImage->Release();   
            pImage->Release();   
            DeleteDC(bmpDC);   
        }   
        pImageFactory->Release();   
    }   
    CoUninitialize();   
    //ProcessThePixelsWithAlphaChannel Here        
    // vertical flip and ProcessThePixelsWithAlphaChannel here   
    for (UINT y=0; y<imageInfo.Height/2; y++)   
    {   
        BYTE * pPixel = (BYTE *) lpByte + imageInfo.Width * 4 * y;   
        BYTE * pDstPixel = (BYTE*) lpByte + imageInfo.Width * 4 * (imageInfo.Height-y-1);   
        for (UINT x=0; x<imageInfo.Width; x++)   
        {   
            pPixel[0] = pPixel[0] * pPixel[3] / 255;   
            pPixel[1] = pPixel[1] * pPixel[3] / 255;   
            pPixel[2] = pPixel[2] * pPixel[3] / 255;   
            pDstPixel[0] = pDstPixel[0] * pDstPixel[3] / 255;   
            pDstPixel[1] = pDstPixel[1] * pDstPixel[3] / 255;   
            pDstPixel[2] = pDstPixel[2] * pDstPixel[3] / 255;   
            INT* pOrigin = (INT*)pPixel;   
            INT* pDst    = (INT*)pDstPixel;   
            INT temp = *pOrigin;   
            *pOrigin = *pDst;   
            *pDst = temp;   
            pPixel += 4;   
            pDstPixel += 4;   
        }   
    }   
    return hBitmap;   
}  
HBITMAP LoadPngImage2 (HDC hdc, LPCTSTR filename)
{
 IImagingFactory* pImageFactory = 0;
 IImage* pImage = 0;
 ImageInfo imageInfo;
 CoInitializeEx(0, COINIT_MULTITHREADED);
 HBITMAP hBitmap = 0;
 LPBYTE lpByte;
 if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, 0, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&pImageFactory)))
 {
  if (SUCCEEDED(pImageFactory->CreateImageFromFile(filename, &pImage))&& SUCCEEDED(pImage->GetImageInfo(&imageInfo)))
  {
   HDC bmpDC = CreateCompatibleDC(hdc);
   //LPBYTE lpByte;
   BITMAPINFO *pbinfo ;
   pbinfo = (BITMAPINFO *)calloc(1, sizeof(BITMAPINFO) + 4 * sizeof(INT)) ;
   if(!pbinfo)
    return FALSE ;
   pbinfo->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
   pbinfo->bmiHeader.biWidth = imageInfo.Width ;
   pbinfo->bmiHeader.biHeight = imageInfo.Height ;
   pbinfo->bmiHeader.biPlanes = 1;
   pbinfo->bmiHeader.biBitCount = 32;
   pbinfo->bmiHeader.biCompression = BI_ALPHABITFIELDS;
   pbinfo->bmiHeader.biSizeImage = 0 ;
   pbinfo->bmiHeader.biXPelsPerMeter = 11811;
   pbinfo->bmiHeader.biYPelsPerMeter = 11811;
   pbinfo->bmiHeader.biClrUsed = 0;
   pbinfo->bmiHeader.biClrImportant = 0;
   int *pMask = (int*)&(pbinfo->bmiColors[0]) ;
   *pMask++ = 0x00FF0000 ;
   *pMask++ = 0x0000FF00 ;
   *pMask++ = 0x000000FF ;
   *pMask++ = 0xFF000000 ;
   hBitmap = CreateDIBSection(NULL, pbinfo, DIB_RGB_COLORS, (void **)&lpByte, NULL, 0) ;
   free(pbinfo) ;
   if(!hBitmap || !lpByte)
    return FALSE ;
   RECT rect = {0, 0, imageInfo.Width, imageInfo.Height};
   IBitmapImage *pBitmapImage;
   BitmapData bitmapData;
   bitmapData.Width = imageInfo.Width;
   bitmapData.Height = imageInfo.Height;
   bitmapData.PixelFormat = imageInfo.PixelFormat;
   pBitmapImage = NULL;
   pImageFactory->CreateBitmapFromImage(pImage, imageInfo.Width, imageInfo.Height, PIXFMT_32BPP_ARGB,
    InterpolationHintDefault, &pBitmapImage);
   pBitmapImage->LockBits(&rect, ImageLockModeRead,PIXFMT_32BPP_ARGB, &bitmapData);
   //transferring the pixels
   memcpy(lpByte, bitmapData.Scan0, imageInfo.Width * imageInfo.Height * 4);
   pBitmapImage->UnlockBits(&bitmapData);
   pBitmapImage->Release();
   pImage->Release();
   DeleteDC(bmpDC);
  }
  pImageFactory->Release();
 }
 CoUninitialize();
 //ProcessThePixelsWithAlphaChannel Here  
 // vertical flip and ProcessThePixelsWithAlphaChannel here
 for (UINT y=0; y<imageInfo.Height/2; y++)
 {
  BYTE * pPixel = (BYTE *) lpByte + imageInfo.Width * 4 * y;
  BYTE * pDstPixel = (BYTE*) lpByte + imageInfo.Width * 4 * (imageInfo.Height-y-1);
  for (UINT x=0; x<imageInfo.Width; x++)
  {
   pPixel[0] = pPixel[0] * pPixel[3] / 255;
   pPixel[1] = pPixel[1] * pPixel[3] / 255;
   pPixel[2] = pPixel[2] * pPixel[3] / 255;
   pDstPixel[0] = pDstPixel[0] * pDstPixel[3] / 255;
   pDstPixel[1] = pDstPixel[1] * pDstPixel[3] / 255;
   pDstPixel[2] = pDstPixel[2] * pDstPixel[3] / 255;
   INT* pOrigin = (INT*)pPixel;
   INT* pDst    = (INT*)pDstPixel;
   INT temp = *pOrigin;
   *pOrigin = *pDst;
   *pDst = temp;
   pPixel += 4;
   pDstPixel += 4;
  }
 }
 return hBitmap;
}
类似的源码可以在MSDN论坛的某一个帖子中找到.但是不是非常完整且正确. 这里做了一些修正. 略去之前的那些过程,但看最后的这个for循环, 实际上做了一个垂直方向的180度翻转. 这就是说这里的接口memcpy拷贝过去的图片像素信息实际上是被翻转过的, 这个在很多camera的帧里面都有体现.
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/hhygcy/archive/2009/04/29/4138195.aspx](http://blog.csdn.net/hhygcy/archive/2009/04/29/4138195.aspx)
