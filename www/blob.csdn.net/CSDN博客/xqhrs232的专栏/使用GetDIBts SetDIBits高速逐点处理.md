# 使用GetDIBts/SetDIBits高速逐点处理 - xqhrs232的专栏 - CSDN博客
2012年10月26日 23:31:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1025
原文地址::[http://blog.csdn.net/stevenkoh/article/details/7900823](http://blog.csdn.net/stevenkoh/article/details/7900823)
转自：[http://hi.baidu.com/shallow_sleep/blog/item/2fde85123c04f256f819b8b5.html](http://hi.baidu.com/shallow_sleep/blog/item/2fde85123c04f256f819b8b5.html)
之前逐点处理像素的时候都用GetPixel和SetPixel，对比了下坂本千寻系列里用的DIB，才知道那速度真不是一般的慢。前者可以看到扫描线，处理一张1024*800的图需要1~2s，但是后者处理相同大小的图几乎是即时的。
关于DIB(设备无关位图）和DDB（设备相关位图）有很多概念，经常弄得我云里雾里，实际上用起来，DDB就是和dc相关的位图，不同情况下用CreateBMP()，CreateCompatibleBMP()，LoadBMP()，LoadImage()等创建的就是DDB。DIB就是一片内存，里面存储着位图掐头去尾，只留下RGB(32位真彩，16位真彩)，或者像素+色板（8位）的信息。
创建DIB也比较晕，坂本千寻系列里，读bmp，png，读像素点等函数都是自己写的。但是现在读写图片有很多好用的库，应该好好利用。所以，我的方法是，用CImage类读图->创建双缓存->读图到离屏dc->读内存bmp到DIB->GetDIBits->处理像素点->SetDIBits到屏幕dc。
//1.创建双缓存，创建DIB
//HDC hdc,memdc, HBITMAP memBmp为成员变量
hdc=::GetDC(m_hWnd);
memdc=CreateCompatibleDC(hdc);
memBmp=CreateCompatibleBitmap(hdc,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
SelectObject(memdc,memBmp);
//CImage *img为成员变量 #include <atlimage.h>
img=new CImage();
img->Load(_T("3.bmp"));
img->Draw(memdc,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),0,0,img->GetWidth(),img->GetHeight());
//从HBITMAP获取BITMAP
CBitmap cbmp;
cbmp.Attach(memBmp);
cbmp.GetBitmap(&bm);
//创建DIB实际上就两步
//1. 在内存new出一片位图大小的空间，new BYTE，new char，GlobalAlloc都有人用
//2. 填写BITMAPINFO结构
DWORD size=bm.bmWidthBytes*bm.bmHeight; //每行像素位*图高
pBuf=new BYTE[size];
ZeroMemory(&binfo,sizeof(BITMAPINFO)); 
binfo.bmiHeader.biBitCount=bm.bmBitsPixel;      //每个像素多少位，也可直接写24(RGB)或者32(RGBA)
binfo.bmiHeader.biCompression=0; 
binfo.bmiHeader.biHeight=bm.bmHeight; 
binfo.bmiHeader.biPlanes=1; 
binfo.bmiHeader.biSizeImage=0; 
binfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER); 
binfo.bmiHeader.biWidth=bm.bmWidth;
//下面就可以逐点处理了
//获取位图到内存DIB
GetDIBits(memdc,memBmp,0,binfo.bmiHeader.biHeight,pBuf,(BITMAPINFO*)&binfo,DIB_RGB_COLORS);
//逐点处理，这里是用来作淡出效果
//这里是一个颜色分量8位，而不是一个像素
for(int i=0;i<binfo.bmiHeader.biSizeImage;i++)
{
   pBuf[i]=pBuf[i]*level/256;
}
//全图处理完毕读出到屏幕dc显示
SetDIBits(hdc,memBmp,0,binfo.bmiHeader.biHeight,pBuf,(BITMAPINFO*)&binfo,DIB_RGB_COLORS);
原来DIB就是这么简单。
//================================
备注::
  1>WINCE下不支持GetDIBits和SetDIBits这两个函数
  2>WINCE下支持通过GetPixel和SetPixel来操作DC的像素点
  3>BITMAP数据结构有一个bmBits指针应该就是指向位图的数据
