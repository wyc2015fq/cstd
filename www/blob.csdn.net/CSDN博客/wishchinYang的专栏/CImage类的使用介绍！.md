# CImage类的使用介绍！ - wishchinYang的专栏 - CSDN博客
2013年10月22日 14:39:15[wishchin](https://me.csdn.net/wishchin)阅读数：1653
链接参考：[http://www.cnblogs.com/juncheng/articles/1600730.html](http://www.cnblogs.com/juncheng/articles/1600730.html)
CImage是MFC和ATL共享的新类，它能从外部磁盘中调入一个JPEG、GIF、BMP和PNG格式的图像文件加以显示，而且这些文件格式可以相互转换。例如通过简单的几句,就可以实现CImage类和CBitmap类实例的转换:
HBITMAP hBitmap=image.Detach();
CBitmap bmp;
bmp.Attach(hBitmap);
这样一来,就又回归到以前操纵CBitmap的方式了.CImage本身封装了DIB(设备无关位图)的功能，因而能够处理每个位图像素。
它具有下列最酷特性：
　　1、AlphaBlend支持像素级的颜色混合，从而实现透明和半透明的效果。
　　2、PlgBlt能使一个矩形区域的位图映射到一个平行四边形区域中，而且还可能使用位屏蔽操作。
　　3、TransparentBlt在目标区域中产生透明图像，SetTransparentColor用来设置某种颜色是透明色。
　　4、MaskBlt在目标区域中产生源位图与屏蔽位图合成的效果。
由于CImage在不同的Windows操作系统中其某些性能是不一样的，因此在使用时要特别注意。例如，CImage::PlgBlt和CImage::MaskBlt只能在 Windows NT 4.0 或更高版本中使用，但不能运行在Windows 95/98 应用程序中。CImage::AlphaBlend和CImage::TransparentBlt也只能在 Windows 2000/98或其更高版本中使用。即使在Windows 2000运行程序还必须将stdafx.h文件中的WINVER和_WIN32_WINNT的预定义修改成0x0500才能正常使用。
(1):[MFC中显示图像的类CImage](http://blog.csdn.net/huangxy10/article/details/7105799):
我使用的函数：
```cpp
void CAviTestDlg::showCimage(CImage &image)
{
//CImage image; //创建图片类
//image.Load(rphoto); //根据图片路径加载图片
CRect rect;//定义矩形类
int cx = image.GetWidth();//获取图片宽度
int cy = image.GetHeight();//获取图片高度
GetDlgItem(IDC_SHOWPIC)->GetWindowRect(&rect);//将窗口矩形选中到picture控件上
ScreenToClient(&rect);//将客户区选中到Picture控件表示的矩形区域内
GetDlgItem(IDC_SHOWPIC)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);//将窗口移动到Picture控
CWnd *pWnd=GetDlgItem(IDC_SHOWPIC);//获得pictrue控件窗口的句柄
pWnd->GetClientRect(&rect);//获得pictrue控件所在的矩形区域
CDC *pDC=pWnd->GetDC();//获得pictrue控件的DC
image.Draw(pDC->m_hDC, rect); //将图片画到Picture控件表示的矩形区域
ReleaseDC(pDC);//释放picture控件的DC
}
```
（2）：CImage（MFC） 加载剪贴板图像数据
```cpp
CView *cv=this; 
CImage* m_image; 
OpenClipboard(NULL); 
HANDLE h=::GetClipboardData(CF_BITMAP);    
if(h==0) 
{ 
CloseClipboard();  
return; 
} 
BITMAP bm;    
    GetObject(h,sizeof(bm),&bm);   
if(m_image!=NULL) 
{ 
delete m_image; 
m_image=NULL; 
}  
m_image=new CImage(); 
m_image->Create(bm.bmWidth,bm.bmHeight,32,0); 
HDC hdc=m_image->GetDC(); 
CDC imgDC; 
imgDC.Attach(hdc); 
CBitmap *pBmp=new CBitmap;    
pBmp->Attach(h);    
CClientDC dc(cv);     
CDC memDC;    
memDC.CreateCompatibleDC(&dc);     
CBitmap *pOldBitmap=memDC.SelectObject(pBmp);   
imgDC.BitBlt(0,0,bm.bmWidth,bm.bmHeight,&memDC,0,0,SRCCOPY);   
memDC.SelectObject(pOldBitmap);    
pBmp->Detach(); 
delete pBmp;    
imgDC.Detach(); 
m_image->ReleaseDC();
```
（3）：微软官方说明：[http://technet.microsoft.com/zh-cn/magazine/k826sz5e(en-us,VS.80).aspx](http://technet.microsoft.com/zh-cn/magazine/k826sz5e(en-us,VS.80).aspx)
类的使用：[http://msdn.microsoft.com/zh-cn/library/bwea7by5(v=vs.80)](http://msdn.microsoft.com/zh-cn/library/bwea7by5(v=vs.80))
CImage::Create 
Creates a **CImage** bitmap and attach it to the previously constructed **CImage** object.
BOOL Create(
   int nWidth,
   int nHeight,
   int nBPP,
   DWORD dwFlags = 0 
) throw( );
#### Parameters
- nWidth
The width of the **CImage** bitmap, in pixels.
- nHeight
The height of the **CImage** bitmap, in pixels. If nHeight is
 positive, the bitmap is a bottom-up DIB and its origin is the lower left corner. If nHeight is
 negative, the bitmap is a top-down DIB and its origin is the upper left corner.
- nBPP
The numbers of bits per pixel in the bitmap. Usually 4, 8, 16, 24, or 32. Can be 1 for monochrome bitmaps or masks.
- dwFlags
Specifies if the bitmap object has an alpha channel. Can be a combination of zero or more of the following values:
- 
**createAlphaChannel   **Can only be used if nBPP is 32, and eCompression is **BI_RGB**.
 If specified, the created image has an alpha (transparency) value for each pixel, stored in the 4th byte of each pixel (unused in a non-alpha 32-bit image). This alpha channel is automatically used when calling[CImage::AlphaBlend](http://technet.microsoft.com/zh-cn/magazine/x2zk0d0f(en-us,VS.80).aspx).
