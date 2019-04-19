# Windows Mobile 显示PNG图片----把PNG转成BMP再画----貌似速度更快些 - xqhrs232的专栏 - CSDN博客
2010年07月31日 11:21:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1493标签：[windows																[mobile																[image																[timer																[null																[object](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=mobile&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://blog.csdn.net/ice520301/archive/2009/12/25/5075367.aspx](http://blog.csdn.net/ice520301/archive/2009/12/25/5075367.aspx)
最近在跟长时间Suspend回来后，按Send/End键Sometimes不能用的BUG. 本人认为不关应用什么事， 虽然PhoneCanvas部分是我做的. 但
驱动那边人就是要我来跟， 唉，没办法，写了一个hook Send/End键的应用来跟，看COM有没有打印相关消息，来判断按键消息是否传到了上层, 
所以就有了些时间来搞一下PNG显示. MSFT有IImage来显示图片, 但如果直接Draw出来的话， 每次都要花大量的时间来解码，所以就感觉会慢. Google了一下，有人说把PNG转成PARGB，然后再画出来，会快很多.  所以就自己试了一下这个方法， 感觉还不错哦。
先用IImage接口把PNG转成PARGB Bitmap并存起来， 然后再根据需要将图片画出来就OK了...
#include <imaging.h>
#include <initguid.h>
#include <imgguids.h>
#pragma comment(lib,"Imaging.lib")
#pragma comment(lib,"uuid.lib")
typedef struct _tagPARGBImageInfo
{
    UINT Width;             // the Width of the image
    UINT Height;            // the Height of the image
    HBITMAP hBitMap;    // the Handle of the PARGB image 
}PARGBImageInfo;
PARGBImageInfo g_PARGBImageInfo = {0};
// load png and create a PARGB Bitmap
void Load_PNG2PARGBBitmap()
{
    IImagingFactory *pImgFactory = NULL;
    IImage *pImage                 = NULL;
    // Normally you would only call CoInitialize/CoUninitialize
    // once per thread.  This sample calls CoInitialize in this
    // draw function simply to illustrate that you must call 
    // CoInitialize before calling CoCreateInstance.
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    // Create the imaging factory.
    if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IImagingFactory,
        (void **)&pImgFactory)))
    {
        INT nTimer = GetTickCount();
        // Load the image from the png file and get the image info
        if (SUCCEEDED(pImgFactory->CreateImageFromFile(TEXT("//My Documents//test.png"), &pImage)))
        {
            ImageInfo imgInfo = {0};
            // Get the image info
            if (SUCCEEDED(pImage->GetImageInfo(&imgInfo)))
            {
                BitmapData bmpData           = {0};
                IBitmapImage *pBitmapImage = NULL;
                RECT rc                       = {0, 0, imgInfo.Width, imgInfo.Height};
                // Save the image Width and Height
                g_PARGBImageInfo.Width = imgInfo.Width;
                g_PARGBImageInfo.Height = imgInfo.Height;
                // Create a bitmap image from an IImage object
                if (SUCCEEDED(pImgFactory->CreateBitmapFromImage(pImage, 0, 0, PIXFMT_32BPP_PARGB, InterpolationHintDefault, &pBitmapImage)))
                {
                    // Lock an area of a bitmap image object and access bitmap data in the specified pixel format
                    pBitmapImage->LockBits(&rc, ImageLockModeRead, PIXFMT_32BPP_PARGB, &bmpData);
                    // Create a bitmap with the specified width, height, and bit depth. And Save the Bitmap handle
                    g_PARGBImageInfo.hBitMap = CreateBitmap(imgInfo.Width, imgInfo.Height, 1, GetPixelFormatSize(imgInfo.PixelFormat), bmpData.Scan0);
                    ASSERT(g_PARGBImageInfo.hBitMap);
                    // Unlock an area of the bitmap image object locked by a previous call to the IBitmapImage::LockBits call
                    pBitmapImage->UnlockBits(&bmpData);
                    pBitmapImage->Release(); 
                }
            }
            pImage->Release();
        }
        DEBUGMSG(1, (TEXT("...Load_PNG2PARGBBitmap...Timer = %d....../r/n"), GetTickCount()-nTimer));
        pImgFactory->Release();
    }
    // Close the OLE Component Object Model (COM) library, freeing any resources that it maintains.
    CoUninitialize();
}
// draw PARGB Bitmap
void DrawImage_PARGB(HDC hdc)
{
    INT nTimer = GetTickCount();
    // draw image
    HDC hdcMem                = ::CreateCompatibleDC(hdc);
    HGDIOBJ hGdiObjOld        = ::SelectObject(hdcMem, g_PARGBImageInfo.hBitMap);
    // AlphaBlend the hdcTemp data to the hdc
    BLENDFUNCTION blendFunction = {0};
    blendFunction.AlphaFormat = AC_SRC_ALPHA;
    blendFunction.BlendFlags = 0;
    blendFunction.BlendOp = AC_SRC_OVER;
    blendFunction.SourceConstantAlpha = 255; //0-255 (透明度设置, 可以用于图片的渐变显示 (^v^; )
    // Alpha Blend to hdcTemp
    AlphaBlend(hdc, 0, 0, g_PARGBImageInfo.Width, g_PARGBImageInfo.Height, 
        hdcMem, 0, 0, g_PARGBImageInfo.Width, g_PARGBImageInfo.Height, blendFunction);
    ::SelectObject(hdcMem, hGdiObjOld);
    if (hdcMem)
    {
        DeleteDC(hdcMem);
        hdcMem = NULL;
    }
    DEBUGMSG(1, (TEXT("...DrawImage_PARGB...Draw PNG Timer = %d....../r/n"), GetTickCount()-nTimer));
}
测试DEBUG模式下512X512(186KB) PNG对比:
用直接Draw出来： 
...DrawImage_ImageDraw.....Draw PNG Timer = 124......
// Draw the image.
INT nTimer = GetTickCount();
pImage->Draw(hdc, &rc, NULL);
DEBUGMSG(1, (TEXT("...DrawImage_ImageDraw.....Draw PNG Timer = %d....../r/n"), GetTickCount()-nTimer));
先转换，然后画出来:
...Load_PNG2PARGBBitmap...Timer = 121......
...DrawImage_PARGB...Draw PNG Timer = 15......
// Load and Draw the image.
Load_PNG2PARGBBitmap();
DrawImage_PARGB(GetDC(hWnd));
Merry Christmas :&)
Jacky Lee
2009-12-25
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/ice520301/archive/2009/12/25/5075367.aspx](http://blog.csdn.net/ice520301/archive/2009/12/25/5075367.aspx)
