# WinCE显示资源中BMP图片的几种方法附自己个人实验数据 - xqhrs232的专栏 - CSDN博客
2012年09月25日 22:23:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1351标签：[wince																[null																[下载工具																[colors																[byte																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=colors&t=blog)](https://so.csdn.net/so/search/s.do?q=下载工具&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://www.cnblogs.com/we-hjb/archive/2010/02/09/1666778.html](http://www.cnblogs.com/we-hjb/archive/2010/02/09/1666778.html)
# 可以下载工具《bmp轻松转为rgb565工具》
来把不是RGB565格式的BMP转为RGB565格式，CSDN上就可以找到
   前面介绍了如何直接操作WinCE的FrameBuffer，这里将介绍一种通过写FrameBuffer显示资源中BMP图片的方法。并与使用GDI做一个比较，看看直接读写FrameBuffer是否能提高效率？
      采用GDI的方法，关键代码如下:
```
[](http://www.cnblogs.com/we-hjb/archive/2010/02/09/1666778.html)
    hScrDC = CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
    hMemDC = CreateCompatibleDC (hScrDC);
    bmp.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP1));
    bmp.GetBitmap(&bmpInfo); 
    SelectObject(hMemDC,bmp);
    BitBlt(hScrDC,0,0,dwWidth,dwHeight,hMemDC,0,0,SRCCOPY);
[](http://www.cnblogs.com/we-hjb/archive/2010/02/09/1666778.html)
```
       采用直接写FrameBuffer的方法，关键代码如下:
```
[](http://www.cnblogs.com/we-hjb/archive/2010/02/09/1666778.html)
    HINSTANCE hInst = (HINSTANCE)::GetModuleHandle(NULL);
    HRSRC hrc = FindResource((HMODULE)hInst,MAKEINTRESOURCE(IDB_BITMAP1),RT_BITMAP);
    HGLOBAL hGlobal = LoadResource(NULL,hrc);
    dwBufSize = ::SizeofResource(NULL,hrc);
    gbmpBuf = (PBYTE)::LockResource(hGlobal);
    memcpy(gpLCDBuf,gbmpBuf,dwBufSize);
[](http://www.cnblogs.com/we-hjb/archive/2010/02/09/1666778.html)
```
       资源中加载的BMP图片是从WinCE的显存中直接保存下来的，如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCEBMP_11665/image_thumb_2.png)
      使用BitBlt显示的效果如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCEBMP_11665/image_thumb.png)
      直接写显存的效果如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCEBMP_11665/image_thumb_1.png)
      可以看到采用BitBlt，资源中的BMP似乎被真实显示。采用直接写屏的方法颜色被还原了，但往右偏了一些。他们分别使用的时间如下图所示，GDI使用了145ms，直接写显存大概1ms。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCEBMP_11665/image_thumb_3.png)
      很明显，直接写显存比使用BitBlt快很多，但位置有偏差。那么用GDI能否有更快的方法？它为什么这么慢。又尝试着用了GDI的另外一种方法显示。代码如下:
```
[](http://www.cnblogs.com/we-hjb/archive/2010/02/09/1666778.html)
    bmi.bmih.biSize = sizeof(bmi.bmih);
    bmi.bmih.biWidth = dwWidth;
    bmi.bmih.biHeight = -dwHeight;
    bmi.bmih.biPlanes = 1;
    bmi.bmih.biBitCount = (BYTE)bmpInfo.bmBitsPixel;
    bmi.bmih.biSizeImage = 0;
    bmi.bmih.biXPelsPerMeter = 0;
    bmi.bmih.biYPelsPerMeter = 0;
    bmi.bmih.biClrUsed = 0;
    bmi.bmih.biClrImportant = 0;
    bmi.bmih.biCompression = BI_BITFIELDS;
    *(DWORD *)(&bmi.rgq[0]) = 0xF800;
    *(DWORD *)(&bmi.rgq[1]) = 0x07E0;
    *(DWORD *)(&bmi.rgq[2]) = 0x001F;
    StretchDIBits (hScrDC, 0, 0, dwWidth, dwHeight, 0, 0, dwWidth, dwHeight,
        gbmpBuf, (PBITMAPINFO)&bmi, DIB_RGB_COLORS, SRCCOPY);
[](http://www.cnblogs.com/we-hjb/archive/2010/02/09/1666778.html)
```
 　　采用这种方法，显示效果和DDraw完全一样，速度也不相上下，同样比BitBlt快很多。这是为什么呢？玄妙就在bmi.rgq，这里设置的BIT MASK与显示驱动中一致，所以，效果相当于直接写屏，颜色也对了，效率也高了。而BitBlt默认的BIT MASK估计与驱动中的不一致，从而导致多做一些内存运算，花费了一些时间。为了验证这个想法，修改bmi.rgq的设置如下:　　
```
*(DWORD *)(&bmi.rgq[0]) = 0xF800;
    *(DWORD *)(&bmi.rgq[1]) = 0x03E0;
    *(DWORD *)(&bmi.rgq[2]) = 0x001F;
```
 　　这种情况下，颜色显示与BitBlt一样，使用的时间也差不多。看来并不是使用GDI就一定会慢，BIT MASK才是关键。16位色的BIT MASK主要有以下几种。
```
// XRRRRRGG.GGGBBBBB 0x7C00 0x03E0 0x1F
    // RRRRRXGG.GGGBBBBB 0xF800 0x03E0 0x1F
    // RRRRRGGG.GGXBBBBB 0xF800 0x07C0 0x1F
    // RRRRRGGG.GGGBBBBB 0xF800 0x07E0 0x1F
```
 　　 在实际开发过程中，这个小细节可以注意一下，也许可以提高一些效率。
//=========================================
备注::
1.
　这种情况下，颜色显示与BitBlt一样，使用的时间也差不多。看来并不是使用GDI就一定会慢，BIT MASK才是关键。16位色的BIT MASK主要有以下几种。
2.
        今天亲自拿6410试了一下，BITBLT一张800*480的图片，所需时间为247MS，太夸张的数据了，看BMP也没什么特别的，也是16位啊，但是不是跟LCD驱动格式是不是一样的就不清楚了（我LCD格式为RGB565），但换了张是RGB565格式的800*480BMP图片来BITBLT，时间减为7MS，提高了35倍！！！太不可思仪了！！！
3.贴一组参数参考一下，在我们MTK平台测试的结果，图片大小都是800*480，都是使用BITBLT贴图，图片都是通过SHLOADDIBITMAP函数来加载的。
  1》xrgb555----115ms
 2》xrgb555 with rgb color platte(rgb555 format----0x7c3e1f)----0ms----但颜色显示不对，显示为黑色
 3》rgb565----112ms
  4》rgb565 with rgb color platte(rgb565 format----0xf87e1f)-----5ms
4.贴一组参数参考一下，在我们MTK平台测试的结果，图片大小都是800*480，都是使用ALPHABLEND贴图，图片都是通过加载PNG图片来转换来的。
   1》rgba32(通过加载32位的PNG转到BMP)-------200MS
5.贴一组参数参考一下，在wince60模拟器测试的结果，图片大小都是800*480，都是使用BITBLT贴图，图片都是通过SHLOADDIBITMAP函数来加载的。
  1》xrgb555----
 2》xrgb555 with rgb color platte(rgb555 format----0x7c3e1f)----
 3》rgb565----
 4》rgb565 with rgb color platte(rgb565 format----0xf87e1f)-----5ms------既然跟实际的机器差不多的时间！！！

