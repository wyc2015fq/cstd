# GDI位图透明显示 - 长歌行 - CSDN博客





2012年03月06日 10:05:56[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1233









本文介绍两种用GDI函数实现绘制位图时只绘制除指定颜色外的部分，达到“透明”的效果的方法：

1. 用BitBlt实现位图上某种颜色的透明显示

2. 用MaskBlt实现位图上某种颜色的透明显示

为方便起见，用MFC方式讨论。先有如下的定义：

CDC *pDC; // 目标DC，假设已Create，位图已选入

CDC *pActiveDC; // 装载位图的DC。假设已Create，位图已选入

CDC *pMaskDC; // 装有掩码位图的DC

CBitmap bmpMask; // MaskDC上使用的位图

COLORREF crTrans; // pActiveDC上需要被透明处理的颜色

CRect crClient; // pDC的大小。假设已获得

这样，直观的说，本文讨论的目标就是，把pActiveDC绘制到pDC上的时候，不绘制跟crTrans相同的颜色的部分。

1. 用BitBlt API进行透明显示的步骤：

① 处理pMaskDC为黑白DC，使pActiveDC上颜色为crTrans的部分在pMaskDC显示为白色，其余地方显示为黑色。

② 将pActiveDC用BitBlt绘制到pDC上，使用SRCINVERT方式

③ 将pMaskDC用BitBlt绘制到pDC上，使用SRCAND方式

④ 再将pActiveDC用BitBlt绘制到pDC上，使用SRCINVERT方式

ROP中，SRCINVERT是图像间异或处理，SRCAND是图像间与处理。可以简单证明上述的操作过程会得到我们想要的结果：

对于某一个位置，pDC上颜色为B，pActiveDC上颜色为A。

当A == crTrans的时候，pMaskDC上这个位置的颜色M为白色。则上面的②～④步可以表示为：

((B xor A) and M) xor A

⇔ (B xor A) xor A

⇔ B

当A != crTrans的时候，pMaskDC上这个位置的颜色M为黑色。则上面的②～④步可以表示为：


((B xor A) and M) xor A

⇔ 0 xor A

⇔ A

下面是实现代码：


```
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)
// Sample of demonstrating making a color transparent. Pomelo Wu on 29/4/2005
// Make the Mask DC monochrome
pMaskDC->CreateCompatibleDC(pDC);

bmpMask.CreateBitmap(rcClient.Width(), rcClient.Height(), 
1, 1, NULL); // monochrome bitmap
CBitmap * pOldMaskBmp = pMaskDC->SelectObject(&bmpMask);
// Set the mask bitmap
pActiveDC->SetBkColor(crTrans);

pMaskDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pActiveDC,
0, 0, SRC/DownloadFiles\a200507\2005-07-27\COPY);(p>
// Do the painting
pDC->.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pActiveDC,
0, 0, SRC/DownloadFiles\a200507\2005-07-27\INVERT);(p> pDC->.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pMaskDC,
0, 0, SRC/DownloadFiles\a200507\2005-07-27\AND);(p> pDC->.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pActiveDC,
0, 0, SRC/DownloadFiles\a200507\2005-07-27\INVERT);(p> // Omit the resting of destroying GDI object
```


2. 为达到透明的效果，还可以用一种更方便的办法——使用MaskBlt这个API，具体方法如下：

① pMaskDC选入bmpMask。

② 处理pMaskDC，把需要透明的颜色填充满整个pMaskDC。

③ 将pActiveDC用MaskBlt绘制到pDC上，使用ROP code 0xccaa0000方式

实现代码如下：


```
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)
// Sample of demonstrating making a color transparent. Pomelo Wu on 29/4/2005
// ROP code definition
#define ROP_TRANS 0xccaa0000
// handling the mask bitmap
pMaskDC->CreateCompatibleDC(pDC);

bmpMask.CreateBitmap(rcClient.Width(), rcClient.Height(), 
1, 1, NULL); // monochrome bitmap
CBitmap * pOldMaskBmp = pMaskDC->SelectObject(&bmpMask);

pMaskDC-> FillSolidRect(rcClient, crTrans);

pDC->MaskBlt(0, 0, rcClient.Width(), rcClient.Height(), 

pActiveDC, 0, 0, 

bmpMask, 0, 0, ROP_CODE_TRANS);
// Omit the resting of destroying GDI object
```


----------------------------------------------------------------------------------------------------


自己写的代码，实验通过，其中BMP文件中透明部分为白色



```
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)
HDC g_hImgDC,g_hMaskDC;

HBITMAP g_hBitmapBak[2];

void initbitmap()

{

 g_hImgDC = CreateCompatibleDC(0); // 创建兼容dc
 g_hBitmapBak[0] = (HBITMAP)SelectObject(g_hImgDC, (HBITMAP)LoadImage(NULL,TEXT("1.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION));


 g_hMaskDC = CreateCompatibleDC(0);

 g_hBitmapBak[1] = (HBITMAP)SelectObject( g_hMaskDC, CreateBitmap(200, 200, 1, 1, NULL ) );

 

}
void draw(HDC hdc)

{

   BitBlt(g_hMaskDC, 0 , 0, 100, 100 ,g_hImgDC, 0, 0, SRCCOPY);

   BitBlt( hdc, 0, 0, 100, 100, g_hImgDC, 0, 0,SRCINVERT );//将源位图的像素与目标位图的像素进行异或运算
   BitBlt( hdc, 0, 0, 100, 100, g_hMaskDC, 0, 0,SRCAND );

   BitBlt( hdc, 0, 0, 100, 100, g_hImgDC, 0, 0,SRCINVERT );


}

void Release()

{

 g_hBitmapBak[0] = (HBITMAP)SelectObject( g_hMaskDC, g_hBitmapBak[0] );

 DeleteObject( g_hBitmapBak[0]);

 g_hBitmapBak[1] = (HBITMAP)SelectObject( g_hImgDC, g_hBitmapBak[1] );

 DeleteObject( g_hBitmapBak[1]);

 DeleteDC( g_hImgDC );

 DeleteDC( g_hMaskDC );

}
```




另一种方法，关于掩码位图：

设有一个要动的东东，就叫它sprite吧，把它放在一个全黑的背景上，再做一个mask，它是一个和sprite一样大的单色图片，以sprite的轮廓为界，背景是全白的，里面是全黑的。   

  显示时：把要显示地方的背景和mask“与”一下，因为和白（即全1）“与”不变，和黑（即全0）“与”则会变黑，在那就会有一个黑色的sprite轮廓，再把sprite图片和背景“或”一下，因为和全黑“或”不变，所以背景上黑的部分就显示出sprite，而sprite上黑的部分又不会改变背景。   



