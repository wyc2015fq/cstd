# WINCE 半透明及透明位图的实现技术 - xqhrs232的专栏 - CSDN博客
2010年07月08日 18:33:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1114
原文地址::[http://blog.sina.com.cn/s/blog_5be1061c0100azzz.html](http://blog.sina.com.cn/s/blog_5be1061c0100azzz.html)
绘制半透明位图
有的时侯，我们希望显示一幅半透明的位图。也就是说我们将一幅位图B显示到A位图上，又希望透过B位图看到A位图的一部分图像但不是全部。比如A位图是一幅曲线图，B是一幅提示位图，我们想在显示提示的同时看到已显示的曲线，但不需要曲线的背景，就需有用到半透明位图。曲线看上去就象从B位图中渗透过来，其实半透明技术就是一种渗透技术，渗透公式我们可选用多种，在这里我们选用（A AND 0x7F）OR B。注意，白色不能产生渗透。
//参数说明：
//hDIB -位图句柄
//pPal -位图调色板
//xDest -显示位图的左上角x坐标
//yDest -显示位图的左上角y坐标
void DrawSemiTransparentBitmap(CDC *pDC, int nXDest, int nYDest, HGLOBAL hDIB,CPalette *pPal)
BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
int nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed :
1 << bmInfo.bmiHeader.biBitCount;
int nWidth = bmInfo.bmiHeader.biWidth;
int nHeight = bmInfo.bmiHeader.biHeight;
LPVOID lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
CDC memDC;
memDC.CreateCompatibleDC( pDC );
CBitmap bmp;
bmp.CreateCompatibleBitmap( pDC, nWidth, nHeight );
CBitmap *pOldBitmap = memDC.SelectObject( &bmp );
if( pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE&&nColors<256)
CPalette *pOldMemPalette = memDC.SelectPalette(pPal, FALSE);
memDC.RealizePalette();
::SetDIBitsToDevice(memDC.m_hDC, 0, 0, nWidth, nHeight, 0, 0, 0, nHeight, lpDIBBits, (LPBITMAPINFO)hDIB, DIB_RGB_COLORS);
CDC maskDC;
CBitmap mbm;
maskDC.CreateCompatibleDC(pDC);
mbm.CreateCompatibleBitmap(pDC, nWidth, nHeight);
maskDC.SelectObject(&mbm);
maskDC.FillSolidRect(CRect(0, 0, nWidth, nHeight), RGB(0x7F, 0x7F, 0x7F));
pDC->BitBlt(nXDest, nYDest, nWidth, nHeight, &maskDC, 0, 0, SRCAND);
pDC->BitBlt(nXDest, nYDest, nWidth, nHeight, &memDC, 0, 0, SRCPAINT);
memDC.SelectObject(pOldBitmap);
}　
如何画透明位图
    在WINCE中,透明位图最简单的方法是用TransparentBlt函数,可以直接用。
    以下为自己实现的方法。
　　画透明位图通常的方法是使用遮罩。所谓遮罩就是一张黑白双色的位图，他和要透明的位图是对应的，遮罩描述了位图中需要透明的部分，透明的部分是黑色的，而不透明的是白色的，白色的部分就是透明的部分。假设图A是要画的透明位图，图B是遮罩，图A上是一个大写字母A,字母是红色的，背
景是黑色的，图B背景是白色的，上面有一个黑色的字母A和图A的形状是一样的。比如我们要在一张蓝天白云的背景上透明地画图A，就是只把红色的字母A画上去。我们可以先将图B和背景进行与操作，再把图B和背景进行或操作就可以了。
用VC++ MFC实现的代码如下：
void CDemoDlg::OnPaint()
CPaintDC dc(this);
Cbitmap BmpBack,BmpA,BmpB,*pOldBack,*pOldA,*pOldB;
BmpBack.LoadBitmap(IDB_BACKGROUND); // 载入背景图
BmpA.LoadBitmap(IDB_BITMAPA); //载入图A
BmpB.LoadBitmap(IDB_BITMAPB); //载入图B
CDC dcBack,dcA,dcB; //声明三个内存DC用于画图
dcBack.CreateCompatibleDC(&dc);
dcA.CreateCompatibleDC(&dc);
dcB.CreateCompatibleDC(&dc); //把这三个内存DC创建成和PaintDC兼容的DC
pOldBack=dcBack.SelectObject(&BmpBack);
pOldA=dcA.SelectObject(&BmpA);
pOldB=dcB.SelectObject(&BmpB); //把三个位图选入相应的DC
dc.BitBlt(0,0,100,100,&dcBack,0,0,SRCCOPY); //画背景
dc.BitBlt(0,0,48,48,&dcB,0,0,SRCAND); //用与的方式画遮罩图B
dc.BitBlt(0,0,48,48,&dcA,0,0,SRCPAINT); //用或的方式画遮图A
dcBack.SelectObject(pOldBack);
dcBack.SelectObject(pOldA);
dcBack.SelectObject(pOldB); //从内存DC中删除位图
你会看到红色的字母A透明地画在背景上了。
用遮罩的方法必须事先做好遮罩，遮罩和位图大小一样等于多消耗一倍的资源，比较浪费。还有一种画透明位图的方法，基本原理是一样的，只是不用事先做好遮罩，根据需要动态生成遮罩，但是要求需要透明的位图必须指定一种透明色，凡是这个透明色的地方则画成透明的。
用VC++ MFC实现的代码如下：
void DrawTransparentBitmap(CDC *pDC, UINT IDImage,Crect &rect, COLORREF rgbMask)
CDC ImageDC,MaskDC;
Cbitmap Image,*pOldImage;
Cbitmap maskBitmap,*pOldMaskDCBitmap ;
Image.LoadBitmap(IDImage);
ImageDC.CreateCompatibleDC(pDC);
pOldImage=ImageDC.SelectObject(&Image);
MaskDC.CreateCompatibleDC(pDC);
maskBitmap.CreateBitmap( rect.Width(), rect.Height(), 1, 1, NULL );
pOldMaskDCBitmap = MaskDC.SelectObject( &maskBitmap );
ImageDC.SetBkColor(rgbMask);
MaskDC.BitBlt( 0, 0, rect.Width(), rect.Height(), &ImageDC, 0, 0, SRCCOPY );
ImageDC.SetBkColor(RGB(0,0,0));
ImageDC.SetTextColor(RGB(255,255,255));
ImageDC.BitBlt(0, 0, rect.Width(), rect.Height(), &MaskDC, 0, 0, SRCAND);
pDC->BitBlt(rect.left,rect.top,rect.Width(), rect.Height(), &MaskDC, 0, 0, SRCAND);
pDC->BitBlt(rect.left,rect.top,rect.Width(), rect.Height(), &ImageDC, 0, 0,SRCPAINT);
MaskDC.SelectObject(pOldMaskDCBitmap);
ImageDC.SelectObject(pOldImage);
void CDemoDlg::OnPaint()
CPaintDC dc(this);
Cbitmap BmpBack,*pOldBack,;
BmpBack.LoadBitmap(IDB_BACKGROUND);
CDC dcBack;
dcBack.CreateCompatibleDC(&dc);
pOldBack=dcBack.SelectObject(&BmpBack);
dc.BitBlt(0,0,100,100,&dcBack,0,0,SRCCOPY);
DrawTransparentBitmap(&dc,IDB_BITMAPA,Crect(0,0,48,48),RGB(192,192,0));
dcBack.SelectObject(pOldBack);
