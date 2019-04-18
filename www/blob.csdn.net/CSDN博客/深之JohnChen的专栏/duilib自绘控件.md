# duilib自绘控件 - 深之JohnChen的专栏 - CSDN博客

2019年04月14日 23:09:34[byxdaz](https://me.csdn.net/byxdaz)阅读数：20


        有时候我们在使用duilib的同时，可能想自己在窗体上绘制一些图形、线条等等，但是发现绘制不上去。其实是绘制上去然后又被duilib自身的绘制机制给擦掉了应该。

那么正确的绘制姿势呢？

方法1：从CControlUI派生一个子类，然后直接重写DoPaint接口，在里面绘制即可。这样有1个限制，duilib默认是后添加的控件Z序高，而实际绘制全部控件的时候，Z序高的控件在上层。因此如果这个控件的Z序比较低，是有可能被其他控件盖住的，这个要看具体的布局了。

方法2：从CControlUI派生一个子类，然后直接重写DoPostPaint接口。然后调用PaintManager的AddPostPaint接口将这个控件指针加入的m_aPostPaintControls数组即可。此时在全部控件绘制完毕后，会触发DoPostPaint接口的调用，在里面绘制即可。这种方法与1的不同之处在于，仍然保留了原先控件的绘制。并且是在所有控件绘制之后绘制的，因此它的绘制好像是在最上层的，不会由于控件的Z序等导致绘制被其他控件覆盖。redrain大佬有一篇控件拖拽的文章有利用，感兴趣的可以看看。

duilib 圆形头像控件

https://blog.csdn.net/HiZhanYue/article/details/80455191

HBitmap转Bitmap

```
using namespace Gdiplus;
Bitmap* CreateBitmapFromHBITMAP(IN HBITMAP hBitmap)
{
	BITMAP bmp = { 0 };
	if (0 == GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bmp))
	{
		return FALSE;
	}
 
	// Although we can get bitmap data address by bmp.bmBits member of BITMAP   
	// which is got by GetObject function sometime,  
	// we can determine the bitmap data in the HBITMAP is arranged bottom-up   
	// or top-down, so we should always use GetDIBits to get bitmap data.  
	BYTE *piexlsSrc = NULL;
	LONG cbSize = bmp.bmWidthBytes * bmp.bmHeight;
	piexlsSrc = new BYTE[cbSize];
 
	BITMAPINFO bmpInfo = { 0 };
	// We should initialize the first six members of BITMAPINFOHEADER structure.  
	// A bottom-up DIB is specified by setting the height to a positive number,   
	// while a top-down DIB is specified by setting the height to a negative number.  
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = bmp.bmWidth;
	bmpInfo.bmiHeader.biHeight = bmp.bmHeight; // 正数，说明数据从下到上，如未负数，则从上到下  
	bmpInfo.bmiHeader.biPlanes = bmp.bmPlanes;
	bmpInfo.bmiHeader.biBitCount = bmp.bmBitsPixel;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
 
	HDC hdcScreen = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	LONG cbCopied = GetDIBits(hdcScreen, hBitmap, 0, bmp.bmHeight,
		piexlsSrc, &bmpInfo, DIB_RGB_COLORS);
	DeleteDC(hdcScreen);
	if (0 == cbCopied)
	{
		delete[] piexlsSrc;
		return FALSE;
	}
 
	// Create an GDI+ Bitmap has the same dimensions with hbitmap  
	Bitmap *pBitmap = new Bitmap(bmp.bmWidth, bmp.bmHeight, PixelFormat32bppPARGB);
 
	// Access to the Gdiplus::Bitmap's pixel data  
	BitmapData bitmapData;
	Rect rect(0, 0, bmp.bmWidth, bmp.bmHeight);
	if (Ok != pBitmap->LockBits(&rect, ImageLockModeRead,
		PixelFormat32bppPARGB, &bitmapData))
	{
		if (pBitmap)
		{
			delete pBitmap;
			pBitmap = NULL;
		}
		return NULL;
	}
 
	BYTE *pixelsDest = (BYTE*)bitmapData.Scan0;
	int nLinesize = bmp.bmWidth * sizeof(UINT);
	int nHeight = bmp.bmHeight;
 
	// Copy pixel data from HBITMAP by bottom-up.  
	for (int y = 0; y < nHeight; y++)
	{
		// 从下到上复制数据，因为前面设置高度时是正数。  
		memcpy_s(
			(pixelsDest + y * nLinesize),
			nLinesize,
			(piexlsSrc + (nHeight - y - 1) * nLinesize),
			nLinesize);
	}
 
	// Copy the data in temporary buffer to pBitmap  
	if (Ok != pBitmap->UnlockBits(&bitmapData))
	{
		delete pBitmap;
	}
 
	delete[] piexlsSrc;
	return pBitmap;
}

void CXXX::SetBkImage(LPCTSTR pStrImage)
{
	__super::SetBkImage(pStrImage);
	
	TImageInfo* data = NULL;
	DWORD mask = 0;
	data = CRenderEngine::LoadImage(pStrImage, NULL, mask);
	if (data)
	{
		m_bkGDIPimage = CreateBitmapFromHBITMAP(data->hBitmap);
	}
	delete data;
}
```

