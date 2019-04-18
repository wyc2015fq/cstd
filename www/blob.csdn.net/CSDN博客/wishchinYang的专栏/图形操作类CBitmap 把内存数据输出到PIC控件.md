# 图形操作类CBitmap 把内存数据输出到PIC控件 - wishchinYang的专栏 - CSDN博客
2013年10月21日 16:01:37[wishchin](https://me.csdn.net/wishchin)阅读数：1379
（1）：复制元数据到 HBItmap
```cpp
HBITMAP CAviTestDlg::BufferToHBITMAP(LPSTR m_pBMPBuffer)
{
HBITMAP                 hShowBMP;
LPSTR                 hDIB,lpBuffer = m_pBMPBuffer;
LPVOID                 lpDIBBits;
BITMAPFILEHEADER     bmfHeader;
DWORD                 bmfHeaderLen;
bmfHeaderLen = sizeof(bmfHeader);
strncpy((LPSTR)&bmfHeader,(LPSTR)lpBuffer,bmfHeaderLen);
if (bmfHeader.bfType != (*(WORD*)"BM")) return NULL;
hDIB = lpBuffer + bmfHeaderLen;//写入文件头
BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
int m_bmW = bmiHeader.biWidth;
int m_bmH = bmiHeader.biHeight;
lpDIBBits=(lpBuffer)+((BITMAPFILEHEADER *)lpBuffer)->bfOffBits;
CClientDC dc(this);
hShowBMP = CreateDIBitmap(dc.m_hDC,&bmiHeader,CBM_INIT,lpDIBBits,&bmInfo,DIB_RGB_COLORS);
return hShowBMP;
}
```
（2）：显示数据到控件
```cpp
LPSTR m_buffer=NULL;
CBitmap   m_bmp;
CRect rect;
DIBSECTION ds;
       HBITMAP hbmp =BufferToHBITMAP(m_buffer);
if( hbmp == NULL )return ;
//该断程序用来取得加载的BMP的信息//
m_bmp.Attach( hbmp );
BITMAPINFOHEADER &bminfo = ds.dsBmih;
m_bmp.GetObject( sizeof(ds), &ds );
int cx=bminfo.biWidth;   //得到图像宽度
int cy=bminfo.biHeight; //得到图像高度
/////////////得到了图像的宽度和高度后,我们就可以对图像大小进行适应,即调整控件的大小,让它正好显示一张图片///////////////////////////
GetDlgItem(IDC_SHOWPIC2)->GetWindowRect(&rect);
ScreenToClient(&rect);
GetDlgItem(IDC_SHOWPIC2)->MoveWindow(rect.left,rect.top,cx,cy,true);//调整大小
//IDC_SHOWPIC2为我的控件ID
```
（3）：我的代码实现！
```cpp
void CAviTestDlg::BYTE2DC(BYTE* pArray, UINT ID)
//void CAviTestDlg::BitMatToDC(cv::Mat img, UINT ID)
{
//if(img.empty())return;
// 获得显示控件的 DC
CDC *cdc = GetDlgItem( ID ) ->GetDC(); 
CDC MemDC;//首先定义一个显示设备对象
CBitmap MemBitmap;//定义一个位图对象
// 获取 HDC(设备句柄) 来进行绘图操作
//HDC hDC = cdc ->GetSafeHdc();///////pDC
CRect rect;
GetDlgItem(ID) ->GetClientRect( &rect );
Gdiplus::Bitmap bitmap(RGB_Width,RGB_Height,RGB_Width*3,  PixelFormat24bppRGB, (BYTE*)pArray);//根据Mat矩阵创建一个GDI+中的Bitmap位图,imageMD.FullYRes()* 3
//随后建立与屏幕显示兼容的内存显示设备
MemDC.CreateCompatibleDC(cdc);
//下面建立一个与屏幕显示兼容的位图，至于位图的大小，可以用窗口的大小
MemBitmap.CreateCompatibleBitmap(cdc, rect.Width(), rect.Height());
//将位图选入到内存显示设备中
//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
//先用背景色将位图清除干净，可以用自己应该用的颜色
MemDC.FillSolidRect(0,0, rect.Width(), rect.Height(),RGB(0,255,255));
//绘图
Gdiplus::Graphics g(MemDC.m_hDC);
Gdiplus::Image *ii = &bitmap; 
g.DrawImage(ii, Gdiplus::Rect(0,0,rect.Width(),rect.Height()));
g.ReleaseHDC(MemDC.m_hDC);
//将内存中的图拷贝到屏幕上进行显
cdc->BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0, 0,SRCCOPY);
//绘图完成后的清理
MemBitmap.DeleteObject();
MemDC.DeleteDC();
ReleaseDC(cdc);
}
```
（4）：微软官方帮助：
# CBitmap Class
封装Windows图形设备接口(GDI)位图并提供成员函数操作位图。
[](http://technet.microsoft.com/zh-cn/subscriptions/6t1yfd35.aspx)
class CBitmap : public CGdiObject
![](http://i.technet.microsoft.com/Areas/Global/Content/clear.gif)成员
### ![6t1yfd35.collapse_all(zh-cn,VS.120).gif](http://i.technet.microsoft.com/dynimg/IC249240.gif)公共构造函数
|名称|说明|
|----|----|
|[CBitmap::CBitmap](http://technet.microsoft.com/zh-cn/subscriptions/af93221e.aspx)|构造 CBitmap 对象。|
### ![6t1yfd35.collapse_all(zh-cn,VS.120).gif](http://i.technet.microsoft.com/dynimg/IC249240.gif)公共方法
|名称|说明|
|----|----|
|[CBitmap::CreateBitmap](http://technet.microsoft.com/zh-cn/subscriptions/432f18e2.aspx)|初始化具有指定的宽度、高度和位组合的设备相关的内存位图的对象。|
|[CBitmap::CreateBitmapIndirect](http://technet.microsoft.com/zh-cn/subscriptions/k781wtwk.aspx)|初始化位图的对象与 BITMAP 结构(如果指定了)生成的宽度、高度和位组合。|
|[CBitmap::CreateCompatibleBitmap](http://technet.microsoft.com/zh-cn/subscriptions/9z2115xa.aspx)|初始化使用位图的对象，以使其与指定的设备兼容。|
|[CBitmap::CreateDiscardableBitmap](http://technet.microsoft.com/zh-cn/subscriptions/w0e47yhs.aspx)|初始化与指定的设备兼容的一discardable位图的对象。|
|[CBitmap::FromHandle](http://technet.microsoft.com/zh-cn/subscriptions/8ydch1a3.aspx)|返回指向 CBitmap 对象，同时使处理Windows HBITMAP 位图。|
|[CBitmap::GetBitmap](http://technet.microsoft.com/zh-cn/subscriptions/279a3c0c.aspx)|用有关位图的信息来加载一 BITMAP 结构。|
|[CBitmap::GetBitmapBits](http://technet.microsoft.com/zh-cn/subscriptions/zy0022z9.aspx)|复制指定的位图的位到指定缓冲区。|
|[CBitmap::GetBitmapDimension](http://technet.microsoft.com/zh-cn/subscriptions/etbdhbss.aspx)|返回位图的宽度和高度。该高度和宽度假定由 [SetBitmapDimension](http://technet.microsoft.com/zh-cn/subscriptions/yzxh3dk7.aspx) 成员函数之前设置。|
|[CBitmap::LoadBitmap](http://technet.microsoft.com/zh-cn/subscriptions/wc99t364.aspx)|通过填写命名位图资源从应用程序的可执行文件和附加位图初始化对象到对象。|
|[CBitmap::LoadMappedBitmap](http://technet.microsoft.com/zh-cn/subscriptions/8t78yyct.aspx)|加载位图和映射颜色设置为当前系统颜色。|
|[CBitmap::LoadOEMBitmap](http://technet.microsoft.com/zh-cn/subscriptions/6243atkt.aspx)|通过加载预定义的Windows位图和附加位图初始化对象到对象。|
|[CBitmap::SetBitmapBits](http://technet.microsoft.com/zh-cn/subscriptions/02a868df.aspx)|将位图的位到指定的位值。|
|[CBitmap::SetBitmapDimension](http://technet.microsoft.com/zh-cn/subscriptions/yzxh3dk7.aspx)|分配宽度和高度到0.1个单位的位图。|
### ![6t1yfd35.collapse_all(zh-cn,VS.120).gif](http://i.technet.microsoft.com/dynimg/IC249240.gif)公共运算符
|名称|说明|
|----|----|
|[CBitmap::operator HBITMAP](http://technet.microsoft.com/zh-cn/subscriptions/25kc2k5s.aspx)|返回Windows处理附加到 CBitmap 对象。|
