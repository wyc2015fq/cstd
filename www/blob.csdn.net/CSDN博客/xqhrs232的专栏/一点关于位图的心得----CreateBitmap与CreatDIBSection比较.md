# 一点关于位图的心得----CreateBitmap与CreatDIBSection比较 - xqhrs232的专栏 - CSDN博客
2012年11月05日 17:30:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1354
原文地址::[http://blog.csdn.net/qq867346668/article/details/6256382](http://blog.csdn.net/qq867346668/article/details/6256382)
相关网帖
1.[用二维数组创建显示位图](http://blog.csdn.net/sophistcxf/article/details/7846330)----[http://blog.csdn.net/sophistcxf/article/details/7846330](http://blog.csdn.net/sophistcxf/article/details/7846330)
windows位图分为DDB和DIB，即设备相关位图和设备无关位图。
可以使用CreateBitmap() 和 CreatDIBSection()分别创建。
经过测试得知，使用CreateBitmap创建的位图无法返回颜色信息（应该是存在但windows不会返回给你），和它类似的还有Loadbitmap()和不带LR_CREATEDIBSECTION参数的LoadImage(), 例如：
HBITMAP hbitmp = LoadBitmap(0, "test.bmp"); // test.bmp为32位alpha通道位图
BITMAP bm;
GetObject(hbitmp, sizeof(BITMAP), &bm);
这样得到的bm中，bmBits为空指针。
如果采用带LR_CREATEDIBSECTION参数的LoadImage()后，调用GetObject(), 则可以得到BITMAP的完整信息。
另外一个要说明的地方是，如果使用CreatBitmap或CreatBitmapIndirect()创建位图后， 使用GetObject()返回的指针也为空。
例如：
HBITMAP hbitmp = LoadBitmap(0, "test.bmp");
BITMAP bmold;
BITMAP bmnew;
GetObject(hbitmp, sizeof(BITMAP), &bmold);
bmnew = bmold;
BYTE* newbits = (BYTE*)malloc(bmold.bmWidthBytes * bmold.bmHeight);
for (int i = 0; i != bmold.bmWidthBytes * bmold.bmHeight; ++i)
   newbits[i] = bmold.bmBits[i];
bmnew.bmBits = newbits;
HBITMAP hbitmpnew = CreatBitmapIndirect(&bmnew);
BITMAP bm;
GetObject(hbitmpnew, sizeof(BITMAP), &bm);
解决办法是使用CreateDIBSection(), 代码如下：
HBITMAP hbmpold = LoadImage(0, L"testbmp.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE |                                                         LR_CREATEDIBSECTION);
BITMAPINFOHEADER bmih;
memset(&bmih, 0, sizeof(bmih));
bmih.biSize = sizeof(BITMAPINFOHEADER);
bmih.biWidth = bmold.bmWidth;
bmih.biHeight = bmold.bmHeight;
bmih.biPlanes = bmold.bmPlanes;
bmih.biBitCount = bmold.bmBitsPixel;
bmih.biCompression = BI_RGB;
HBITMAP hbmpnew = CreateDIBSection(NULL, (BITMAPINFO*)&bmih, DIB_RGB_COLORS, (void**)&newbits, 0, 0);
for (int i = 0; i != bmold.bmWidthBytes * bmold.bmHeight; ++i)
   newbits[i] = bmold.bmBits[i];
使用上面的方法，GetObject()可以得到指针信息。
另外需要注意的是，CreateDIBSection（）是从下向上建立位图的， 也就是说newbits指针指向的第一个元素是整个图片最下面一行的第一位。CreateBitmap建立是自上而下的。
