# 数字图像处理领域的二十四个典型算法及vc实现、第二章 - 结构之法 算法之道 - CSDN博客





2011年03月06日 15:35:00[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：23544标签：[图像处理																[算法																[float																[null																[winapi																[byte](https://so.csdn.net/so/search/s.do?q=byte&t=blog)
个人分类：[02.Algorithms（后续）																[26.Image Processing](https://blog.csdn.net/v_JULY_v/article/category/792352)](https://blog.csdn.net/v_JULY_v/article/category/772685)





** 数字图像处理领域的二十四个典型算法及vc实现、第二章**

作者:July   二零一一年二月二十六日。
参考：百度百科、维基百科、vc数字图像处理。
----------------------------------------------
数字图像处理领域的二十四个典型算法及vc实现、第一章
一、256色转灰度图
二、Walsh变换
三、二值化变换
四、阈值变换
五、傅立叶变换
六、离散余弦变换
**数字图像处理领域的二十四个典型算法及vc实现、第二章七、高斯平滑八、图像平移九、图像缩放十、图像旋转**数字图像处理领域的二十四个典型算法及vc实现、第三章
数字图像处理领域的二十四个典型算法及vc实现、第四章


**前期回顾：**    在上一章，数字图像处理领域的二十四个典型算法及vc实现、第一章中，我们介绍和实现了256色转灰度图、Walsh变换、二值化变换、阈值变换、傅立叶变换、离散余弦变换等数字图像处理领域中的6个典型算法。这一篇接上一篇，继续阐述数字图像处理领域的典型算法。
    注，有兴趣具体深入研究的朋友可参考国内外有关此类图像处理算法的优秀论文。

**七、高斯平滑****算法描述：**    在图像预处理中，对图像进行平滑，去除噪声，恢复原始图像是一个重要内容。本文设计了一个平滑尺度和模板大小均可以改变的高斯滤波器，用它对多幅加入各种噪声后的图像进行平滑，经过对各个结果图像的对比可知高斯滤波对服从正态分布的噪声去除效果比较好，并且相比各个不同参数，在平滑尺度为2，模板大小为7时效果最佳。 

**程序实现：**

函数名称：Template：
参数：HDIB    hDIB         －图像的句柄
double  *tem         －指向模板的指针
int  tem_w           －模板的宽度
int  tem_h           －模板的高度
double xishu         －模板的系数
功能：对图像进行模板操作
说明：为处理方便起见，模板的宽度和高度都应为奇数
HDIB Template(HDIB hDIB,double * tem ,int tem_w,int tem_h,double xishu)
{
    //统计中间值
    double sum;
    //指向图像起始位置的指针
    BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) hDIB);
 //指向象素起始位置的指针
 BYTE *pScrBuff =(BYTE*)::FindDIBBits((char*)lpDIB);
 //获取图像的颜色信息
    int numColors=(int) ::DIBNumColors((char *)lpDIB);
    //如果图像不是256色返回
     if (numColors!=256) 
  {   
        //解除锁定
    ::GlobalUnlock((HGLOBAL) hDIB);
  //返回
  return(hDIB);
  }

    //将指向图像象素起始位置的指针，赋值给指针变量
    BYTE* oldbuf = pScrBuff;

    //循环变量
    int i,j,m,n;
 int w, h, dw;

 //获取图像的宽度
 w = (int) ::DIBWidth((char *)lpDIB);
 //获取图像的高度
 h = (int) ::DIBHeight((char *)lpDIB);
 //计算图像每行的字节数
 dw = (w+3)/4*4;      
 //建立一个和原图像大小相同的25色灰度位图
    HDIB newhDIB=NewDIB(w,h,8);  
 //指向新的位图的指针
 BYTE *newlpDIB=(BYTE*)::GlobalLock((HGLOBAL) newhDIB);
 //指向新的位图的象素起始位置的指针 
    BYTE *destBuf = (BYTE*)FindDIBBits((char *)newlpDIB); 
   //将指向新图像象素起始位置的指针，赋值给指针变量
    BYTE *newbuf=destBuf; 

 //对图像进行扫描
 //行 
    for(i=0;i<h;i++)
    {  
  //列
    for(j=0;j<w;j++)
    {   
     //为统计变量赋初始值
        sum=0;
         //对于图像的4个边框的象素保持原灰度不变
      if( j<((tem_w-1)/2) || j>(w-(tem_w+1)/2) || i<((tem_h-1)/2) || i>(h-

(tem_h+1)/2) )
       *(newbuf+i*dw+j)=*(oldbuf+i*dw+j);  
         //对于其他的象素进行模板操作
   else 
         { 
          //将点（i,j）点作为模板的中心
          for(m=i-((tem_h-1)/2);m<=i+((tem_h-1)/2);m++)
          {
       for(n=j-((tem_w-1)/2);n<=j+((tem_w-1)/2);n++)   
             //将以点（i，j）为中心，与模板大小相同的范围内的象素与模板对用位置的

系数
    //进行相乘并线形叠加
       sum+=*(oldbuf+m*dw+n)* tem[(m-i+((tem_h-1)/2))*tem_w+n-j+

((tem_w-1)/2)]; 
    }

    //将结果乘上总的模板系数
          sum=(int)sum*xishu;
    //计算绝对值
    sum = fabs(sum);

    //如果小于0，强制赋值为0
          if(sum<0)     
          sum=0;
    //如果大于255，强制赋值为255
          if(sum>255)
    sum=255;
    //将计算的结果放到新的位图的相应位置
       *(newbuf+i*dw+j)=sum;
   }
    }
 } 

 //解除锁定
 ::GlobalUnlock((HGLOBAL)hDIB); 
 //返回新的位图的句柄
    return(newhDIB);
}

**变换效果（**图像右边部分即为某一算法的变换效果，下同**）：**

**![](http://hi.csdn.net/attachment/201103/5/8394323_1299332234CHLX.gif)![](http://hi.csdn.net/attachment/201103/5/8394323_12993322119037.gif)**



**八、图像平移****算法描述：**

我想，图像平移，就不必过多介绍了。无非就是通过坐标的增或减的变化，来达到图像在屏幕上的左移、右移、上移、下移的效果。

**程序实现：**

TranslationDIB--该函数用来水平移动DIB图像。函数不会改变图像的大小，移出的部分图像

将截去，空白部分用白色填充。
下面的左移，右移，上移，下移，各自都调用了此TranslationDIB函数。
**//图像平移函数。**BOOL WINAPI **TranslationDIB**(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, LONG 

lXOffset, LONG lYOffset)
{
 // 指向源图像的指针
 LPSTR lpSrc;
 // 指向要复制区域的指针
 LPSTR lpDst;
 // 指向复制图像的指针
 LPSTR lpNewDIBBits;
 HLOCAL hNewDIBBits;
 // 象素在新DIB中的坐标
 LONG i;
 LONG j;
 // 象素在源DIB中的坐标
 LONG i0;
 LONG j0;
 // 图像每行的字节数
 LONG lLineBytes;
 // 计算图像每行的字节数
 lLineBytes = WIDTHBYTES(lWidth * 8);

 // 暂时分配内存，以保存新图像
 hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
 if (hNewDIBBits == NULL)
 {
  // 分配内存失败
  return FALSE;
 }

 // 锁定内存
 lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

 // 每行
 for(i = 0; i < lHeight; i++)
 {
  // 每列
  for(j = 0; j < lWidth; j++)
  {
   // 指向新DIB第i行，第j个象素的指针
   // 注意由于DIB中图像第一行其实保存在最后一行的位置，因此

lpDst
   // 值不是(char *)lpNewDIBBits + lLineBytes * i + j，而是
   // (char *)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + 

j
   lpDst = (char *)lpNewDIBBits + lLineBytes * (lHeight - 1 - 

i) + j;

   // 计算该象素在源DIB中的坐标
   i0 = i - lXOffset;
   j0 = j - lYOffset;

   // 判断是否在源图范围内
   if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < 

lHeight))
   {
    // 指向源DIB第i0行，第j0个象素的指针
    // 同样要注意DIB上下倒置的问题
    lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight 

- 1 - i0) + j0;

    // 复制象素
    *lpDst = *lpSrc;
   }
   else
   {
    // 对于源图中没有的象素，直接赋值为255
    * ((unsigned char*)lpDst) = 255;
   }

  }
 }

 // 复制平移后的图像
 memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

 // 释放内存
 LocalUnlock(hNewDIBBits);
 LocalFree(hNewDIBBits);

 // 返回
 return TRUE;
}

**//向左平移，注：在本程序中，移出去的部分，填充以白色。下同。**void CMyDIPView::OnMenuitem32780() 
{
 // 平移位图
 // 获取文档
 CMyDIPDoc* pDoc = GetDocument(); 
 // 指向DIB的指针
 LPSTR lpDIB;
 // 指向DIB象素指针
 LPSTR   lpDIBBits;
 // 锁定DIB
 lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

 // 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平移，其它的可以类

推）
 if (::DIBNumColors(lpDIB) != 256)
 {
  // 提示用户
  MessageBox("目前只支持256色位图的平移！", "系统提示" , 

MB_ICONINFORMATION | MB_OK);

  // 解除锁定
  ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

  // 返回
  return;
 }

 LONG lXOffset;
 LONG lYOffset;
 // 平移量
 lXOffset = -10;
 lYOffset = 0;


 // 更改光标形状
 BeginWaitCursor();

 // 找到DIB图像象素起始位置
 lpDIBBits = ::FindDIBBits(lpDIB);

**//** 调用TranslationDIB()函数平移DIB
 if (TranslationDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 

lXOffset, lYOffset))
 {

  // 设置脏标记
  pDoc->SetModifiedFlag(TRUE);

  // 更新视图
  pDoc->UpdateAllViews(NULL);
 }
 else
 {
  // 提示用户
  MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | 

MB_OK);
 }

 // 解除锁定
 ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

 // 恢复光标
 EndWaitCursor();
}

**//向右平移**void CMyDIPView::OnMenuitem32781() 
{
 // 平移位图

 // 获取文档
 CMyDIPDoc* pDoc = GetDocument();

 // 指向DIB的指针
 LPSTR lpDIB;

 // 指向DIB象素指针
 LPSTR   lpDIBBits;

 // 锁定DIB
 lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

 // 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平移，其它的可以类

推）
 if (::DIBNumColors(lpDIB) != 256)
 {
  // 提示用户
  MessageBox("目前只支持256色位图的平移！", "系统提示" , 

MB_ICONINFORMATION | MB_OK);

  // 解除锁定
  ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

  // 返回
  return;
 }

 LONG lXOffset;
 LONG lYOffset;
 // 平移量
 lXOffset = 10;
 lYOffset = 0;


 // 更改光标形状
 BeginWaitCursor();

 // 找到DIB图像象素起始位置
 lpDIBBits = ::FindDIBBits(lpDIB);

// 调用TranslationDIB()函数平移DIB
 if (TranslationDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 

lXOffset, lYOffset))
 {

  // 设置脏标记
  pDoc->SetModifiedFlag(TRUE);

  // 更新视图
  pDoc->UpdateAllViews(NULL);
 }
 else
 {
  // 提示用户
  MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | 

MB_OK);
 }

 // 解除锁定
 ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

 // 恢复光标
 EndWaitCursor();
}

**//向上平移**void CMyDIPView::OnMenuitem32779() 
{
 // 平移位图

 // 获取文档
 CMyDIPDoc* pDoc = GetDocument();

 // 指向DIB的指针
 LPSTR lpDIB;

 // 指向DIB象素指针
 LPSTR   lpDIBBits;

 // 锁定DIB
 lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

 // 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平移，其它的可以类

推）
 if (::DIBNumColors(lpDIB) != 256)
 {
  // 提示用户
  MessageBox("目前只支持256色位图的平移！", "系统提示" , 

MB_ICONINFORMATION | MB_OK);

  // 解除锁定
  ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

  // 返回
  return;
 }

 LONG lXOffset;
 LONG lYOffset;
 // 平移量
 lXOffset = 0;
 lYOffset = 10;


 // 更改光标形状
 BeginWaitCursor();

 // 找到DIB图像象素起始位置
 lpDIBBits = ::FindDIBBits(lpDIB);

// 调用TranslationDIB()函数平移DIB
 if (TranslationDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 

lXOffset, lYOffset))
 {

  // 设置脏标记
  pDoc->SetModifiedFlag(TRUE);

  // 更新视图
  pDoc->UpdateAllViews(NULL);
 }
 else
 {
  // 提示用户
  MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | 

MB_OK);
 }

 // 解除锁定
 ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

 // 恢复光标
 EndWaitCursor();
}

**//向下平移**void CMyDIPView::OnMenuitem32778() 
{
 // 平移位图

 // 获取文档
 CMyDIPDoc* pDoc = GetDocument();

 // 指向DIB的指针
 LPSTR lpDIB;

 // 指向DIB象素指针
 LPSTR   lpDIBBits;

 // 锁定DIB
 lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

 // 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平移，其它的可以类

推）
 if (::DIBNumColors(lpDIB) != 256)
 {
  // 提示用户
  MessageBox("目前只支持256色位图的平移！", "系统提示" , 

MB_ICONINFORMATION | MB_OK);

  // 解除锁定
  ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

  // 返回
  return;
 }

 LONG lXOffset;
 LONG lYOffset;
 // 平移量
 lXOffset = 0;
 lYOffset = -10;


 // 更改光标形状
 BeginWaitCursor();

 // 找到DIB图像象素起始位置
 lpDIBBits = ::FindDIBBits(lpDIB);

// 调用TranslationDIB()函数平移DIB
 if (TranslationDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 

lXOffset, lYOffset))
 {

  // 设置脏标记
  pDoc->SetModifiedFlag(TRUE);

  // 更新视图
  pDoc->UpdateAllViews(NULL);
 }
 else
 {
  // 提示用户
  MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | 

MB_OK);
 }

 // 解除锁定
 ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

 // 恢复光标
 EndWaitCursor();
}


** 变化效果：![](http://hi.csdn.net/attachment/201103/5/8394323_12993331380hGH.jpg)**



**九、图像缩放****算法描述：**

**    I、最邻近插值（近邻取样法）：**　　最临近插值的的思想很简单，对于通过反向变换得到的的一个浮点坐标，对其进行简单的取整，得到一个整数型坐标，这个整数型坐标对应的像素值就是目的像素的像素值，也就是说，取浮点坐标最邻近的左上角点（对于DIB是右上角，因为它的扫描行是逆序存储的）对应的像素值。可见，最邻近插值简单且直观，但得到的图像质量不高。

**II、双线性内插值：**　　对于一个目的像素，设置坐标通过反向变换得到的浮点坐标为(i+u,j+v)，其中i、j均为非负整数，u、v为[0,1)区间的浮点数，则这个像素得值 f(i+u,j+v) 可由原图像中坐标为 (i,j)、(i+1,j)、(i,j+1)、(i+1,j+1)所对应的周围四个像素的值决定，即：
　　　　f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1)

其中f(i,j)表示源图像(i,j)处的的像素值，以此类推
　　这就是双线性内插值法。双线性内插值法计算量大，但缩放后图像质量高，不会出现像素值不连续的的情况。由于双线性插值具有低通滤波器的性质，使高频分量受损，所以可能会使图像轮廓在一定程度上变得模糊。

**III、三次卷积法**能够克服以上两种算法的不足，计算精度高，但计算量大，他考虑一个浮点坐标(i+u,j+v)周围的16个邻点，目的像素值f(i+u,j+v)可由如下插值公式得到：
　　　　f(i+u,j+v) = [A] * [B] * [C]

[A]=[ S(u + 1)  S(u + 0)  S(u - 1)  S(u - 2) ]

　　┏ f(i-1, j-1)  f(i-1, j+0)  f(i-1, j+1)  f(i-1, j+2) ┓
[B]=┃ f(i+0, j-1)  f(i+0, j+0)  f(i+0, j+1)  f(i+0, j+2) ┃
　　┃ f(i+1, j-1)  f(i+1, j+0)  f(i+1, j+1)  f(i+1, j+2) ┃
　　┗ f(i+2, j-1)  f(i+2, j+0)  f(i+2, j+1)  f(i+2, j+2) ┛

　　┏ S(v + 1) ┓
[C]=┃ S(v + 0) ┃
　　┃ S(v - 1) ┃
　　┗ S(v - 2) ┛

　　 ┏ 1-2*Abs(x)^2+Abs(x)^3           , 0<=Abs(x)<1
S(x)=｛ 4-8*Abs(x)+5*Abs(x)^2-Abs(x)^3  , 1<=Abs(x)<2
　　 ┗ 0                               , Abs(x)>=2
S(x)是对 Sin(x*Pi)/x 的逼近（Pi是圆周率——π）

    总结：最邻近插值（近邻取样法）、双线性内插值、三次卷积法 等插值算法对于旋转变换、错切变换、一般线性变换 和 非线性变换 都适用。

**程序实现：**

**//该函数用来缩放DIB图像，返回新生成DIB的句柄。**HGLOBAL WINAPI **ZoomDIB**(LPSTR lpDIB, float fXZoomRatio, float fYZoomRatio)
{ 
 // 源图像的宽度和高度
 LONG lWidth;
 LONG lHeight;
 // 缩放后图像的宽度和高度
 LONG lNewWidth;
 LONG lNewHeight;

 // 缩放后图像的宽度（lNewWidth'，必须是4的倍数）
 LONG lNewLineBytes;
 // 指向源图像的指针
 LPSTR lpDIBBits;
 // 指向源象素的指针
 LPSTR lpSrc;
 // 缩放后新DIB句柄
 HDIB hDIB;
 // 指向缩放图像对应象素的指针
 LPSTR lpDst;
 // 指向缩放图像的指针
 LPSTR lpNewDIB;
 LPSTR lpNewDIBBits;
 // 指向BITMAPINFO结构的指针（Win3.0）
 LPBITMAPINFOHEADER lpbmi; 
 // 指向BITMAPCOREINFO结构的指针
 LPBITMAPCOREHEADER lpbmc;

 // 循环变量（象素在新DIB中的坐标）
 LONG i;
 LONG j;

 // 象素在源DIB中的坐标
 LONG i0;
 LONG j0;

 // 图像每行的字节数
 LONG lLineBytes;

 // 找到源DIB图像象素起始位置
 lpDIBBits = ::FindDIBBits(lpDIB);

 // 获取图像的宽度
 lWidth = ::DIBWidth(lpDIB);
 // 计算图像每行的字节数
 lLineBytes = WIDTHBYTES(lWidth * 8); 
 // 获取图像的高度
 lHeight = ::DIBHeight(lpDIB);

 // 计算缩放后的图像实际宽度
 // 此处直接加0.5是由于强制类型转换时不四舍五入，而是直接截去小数部分
 lNewWidth = (LONG) (::DIBWidth(lpDIB) * fXZoomRatio + 0.5);

 // 计算新图像每行的字节数
 lNewLineBytes = WIDTHBYTES(lNewWidth * 8);

 // 计算缩放后的图像高度
 lNewHeight = (LONG) (lHeight * fYZoomRatio + 0.5);

 // 分配内存，以保存新DIB
 hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + *(LPDWORD)

lpDIB + ::PaletteSize(lpDIB));

 // 判断是否内存分配失败
 if (hDIB == NULL)
 {
  // 分配内存失败
  return NULL;
 }

 // 锁定内存
 lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);

 // 复制DIB信息头和调色板
 memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));

 // 找到新DIB象素起始位置
 lpNewDIBBits = ::FindDIBBits(lpNewDIB);

 // 获取指针
 lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
 lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

 // 更新DIB中图像的高度和宽度
 if (IS_WIN30_DIB(lpNewDIB))
 {
  // 对于Windows 3.0 DIB
  lpbmi->biWidth = lNewWidth;
  lpbmi->biHeight = lNewHeight;
 }
 else
 {
  // 对于其它格式的DIB
  lpbmc->bcWidth = (unsigned short) lNewWidth;
  lpbmc->bcHeight = (unsigned short) lNewHeight;
 }

 // 针对图像每行进行操作
 for(i = 0; i < lNewHeight; i++)
 {
  // 针对图像每列进行操作
  for(j = 0; j < lNewWidth; j++)
  {

   // 指向新DIB第i行，第j个象素的指针
   // 注意此处宽度和高度是新DIB的宽度和高度
   lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight 

- 1 - i) + j;

   // 计算该象素在源DIB中的坐标
   i0 = (LONG) (i / fYZoomRatio + 0.5);
   j0 = (LONG) (j / fXZoomRatio + 0.5);

   // 判断是否在源图范围内
   if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < 

lHeight))
   {

    // 指向源DIB第i0行，第j0个象素的指针
    lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight 

- 1 - i0) + j0;

    // 复制象素
    *lpDst = *lpSrc;
   }
   else
   {
    // 对于源图中没有的象素，直接赋值为255
    * ((unsigned char*)lpDst) = 255;
   }  
  }
 } 
 // 返回
 return hDIB;
}

**//缩小图像**void CMyDIPView::OnMenuitem32778() 
{
 // 图像缩放
 // 获取文档
 CMyDIPDoc* pDoc = GetDocument();
 // 指向DIB的指针
 LPSTR lpDIB;

 // 锁定DIB
 lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

 // 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的缩放，其它的可以类

推）
 if (::DIBNumColors(lpDIB) != 256)
 {
  // 提示用户
  MessageBox("目前只支持256色位图的缩放！", "系统提示" , 

MB_ICONINFORMATION | MB_OK);

  // 解除锁定
  ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

  // 返回
  return;
 }

 // 缩放比率
 float fXZoomRatio;
 float fYZoomRatio;

 //缩放量
 fXZoomRatio = 0.8;  //缩小的比率
 fYZoomRatio = 0.8;


 // 创建新DIB
 HDIB hNewDIB = NULL; 
 // 更改光标形状
 BeginWaitCursor();

// 调用ZoomDIB()函数转置DIB
 hNewDIB = (HDIB) ZoomDIB(lpDIB, fXZoomRatio, fYZoomRatio);

 // 判断缩放是否成功
 if (hNewDIB != NULL)
 {

  // 替换DIB，同时释放旧DIB对象
  pDoc->ReplaceHDIB(hNewDIB);

  // 更新DIB大小和调色板
  pDoc->InitDIBData();

  // 设置脏标记
  pDoc->SetModifiedFlag(TRUE);

  // 重新设置滚动视图大小
  SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

  // 更新视图
  pDoc->UpdateAllViews(NULL);
 }
 else
 {
  // 提示用户
  MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | 

MB_OK);
 }

 // 解除锁定
 ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

 // 恢复光标
 EndWaitCursor();

}

**//放大图像**void CMyDIPView::OnMenuitem32779() 
{
 // 图像缩放
 // 获取文档
 CMyDIPDoc* pDoc = GetDocument();
 // 指向DIB的指针
 LPSTR lpDIB; 
 // 锁定DIB
 lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

 // 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的缩放，其它的可以类

推）
 if (::DIBNumColors(lpDIB) != 256)
 {
  // 提示用户
  MessageBox("目前只支持256色位图的缩放！", "系统提示" , 

MB_ICONINFORMATION | MB_OK);

  // 解除锁定
  ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

  // 返回
  return;
 }

 // 缩放比率
 float fXZoomRatio;
 float fYZoomRatio;

 //缩放量
 fXZoomRatio = 1.25;  //放大的比率
 fYZoomRatio = 1.25;


 // 创建新DIB
 HDIB hNewDIB = NULL;

 // 更改光标形状
 BeginWaitCursor();

 // 调用ZoomDIB()函数转置DIB
 hNewDIB = (HDIB) ZoomDIB(lpDIB, fXZoomRatio, fYZoomRatio);

 // 判断缩放是否成功
 if (hNewDIB != NULL)
 {

  // 替换DIB，同时释放旧DIB对象
  pDoc->ReplaceHDIB(hNewDIB);

  // 更新DIB大小和调色板
  pDoc->InitDIBData();

  // 设置脏标记
  pDoc->SetModifiedFlag(TRUE);

  // 重新设置滚动视图大小
  SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

  // 更新视图
  pDoc->UpdateAllViews(NULL);
 }
 else
 {
  // 提示用户
  MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | 

MB_OK);
 }

 // 解除锁定
 ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

 // 恢复光标
 EndWaitCursor();
}



**变换效果（**找到参照物判断**）：**

**![](http://hi.csdn.net/attachment/201103/6/8394323_12993943698TaM.jpg)**


**十、图像旋转**

**算法描述：**图像旋转算法原理，可参见：[http://hi.baidu.com/wangguang246/blog/item/124b9219981f530d35fa41ca.html](http://hi.baidu.com/wangguang246/blog/item/124b9219981f530d35fa41ca.html)。

**程序实现：**

//角度到弧度转化的宏
#define RADIAN(angle) ((angle)*PI/180.0) 
函数名称：RotateDIB()
参数:
LPSTR lpDIB  - 指向源DIB的指针
int iRotateAngle - 旋转的角度（0-360度）
返回值:HGLOBAL            - 旋转成功返回新DIB句柄，否则返回NULL。
说明:该函数用来以图像中心为中心旋转DIB图像，返回新生成DIB的句柄。调用该函数会自动

扩大图像以显示所有的象素。函数中采用最邻近插值算法进行插值。
HGLOBAL WINAPI **RotateDIB**(LPSTR lpDIB, int iRotateAngle)
{ 
 // 源图像的宽度和高度
 LONG lWidth;
 LONG lHeight; 
 // 旋转后图像的宽度和高度
 LONG lNewWidth;
 LONG lNewHeight; 
 // 图像每行的字节数
 LONG lLineBytes; 
 // 旋转后图像的宽度（lNewWidth'，必须是4的倍数）
 LONG lNewLineBytes;
 // 指向源图像的指针
 LPSTR lpDIBBits; 
 // 指向源象素的指针
 LPSTR lpSrc;
 // 旋转后新DIB句柄
 HDIB hDIB;
 // 指向旋转图像对应象素的指针
 LPSTR lpDst;

 // 指向旋转图像的指针
 LPSTR lpNewDIB;
 LPSTR lpNewDIBBits; 
 // 指向BITMAPINFO结构的指针（Win3.0）
 LPBITMAPINFOHEADER lpbmi; 
 // 指向BITMAPCOREINFO结构的指针
 LPBITMAPCOREHEADER lpbmc;

 // 循环变量（象素在新DIB中的坐标）
 LONG i;
 LONG j;

 // 象素在源DIB中的坐标
 LONG i0;
 LONG j0;

 // 旋转角度（弧度）
 float fRotateAngle; 
 // 旋转角度的正弦和余弦
 float fSina, fCosa; 
 // 源图四个角的坐标（以图像中心为坐标系原点）
 float fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
 // 旋转后四个角的坐标（以图像中心为坐标系原点）
 float fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
 // 两个中间常量
 float f1,f2;
 // 找到源DIB图像象素起始位置
 lpDIBBits = ::FindDIBBits(lpDIB); 
 // 获取图像的"宽度"（4的倍数）
 lWidth = ::DIBWidth(lpDIB);
 // 计算图像每行的字节数
 lLineBytes = WIDTHBYTES(lWidth * 8);
 // 获取图像的高度
 lHeight = ::DIBHeight(lpDIB);

 // 将旋转角度从度转换到弧度
 fRotateAngle = (float) RADIAN(iRotateAngle);
 // 计算旋转角度的正弦
 fSina = (float) sin((double)fRotateAngle);
 // 计算旋转角度的余弦
 fCosa = (float) cos((double)fRotateAngle);

 // 计算原图的四个角的坐标（以图像中心为坐标系原点）
 fSrcX1 = (float) (- (lWidth  - 1) / 2);
 fSrcY1 = (float) (  (lHeight - 1) / 2);
 fSrcX2 = (float) (  (lWidth  - 1) / 2);
 fSrcY2 = (float) (  (lHeight - 1) / 2);
 fSrcX3 = (float) (- (lWidth  - 1) / 2);
 fSrcY3 = (float) (- (lHeight - 1) / 2);
 fSrcX4 = (float) (  (lWidth  - 1) / 2);
 fSrcY4 = (float) (- (lHeight - 1) / 2);

 // 计算新图四个角的坐标（以图像中心为坐标系原点）
 fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
 fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
 fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
 fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
 fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
 fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
 fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
 fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

 // 计算旋转后的图像实际宽度
 lNewWidth  = (LONG) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) 

+ 0.5); 
 // 计算新图像每行的字节数
 lNewLineBytes = WIDTHBYTES(lNewWidth * 8);
 // 计算旋转后的图像高度
 lNewHeight = (LONG) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  

+ 0.5);

 // 两个常数，这样不用以后每次都计算了
 f1 = (float) (-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * 

fSina
  + 0.5 * (lWidth  - 1));
 f2 = (float) ( 0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * 

fCosa
  + 0.5 * (lHeight - 1));

 // 分配内存，以保存新DIB
 hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + *(LPDWORD)

lpDIB + ::PaletteSize(lpDIB));

 // 判断是否内存分配失败
 if (hDIB == NULL)
 {
  // 分配内存失败
  return NULL;
 }

 // 锁定内存
 lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);

 // 复制DIB信息头和调色板
 memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
 // 找到新DIB象素起始位置
 lpNewDIBBits = ::FindDIBBits(lpNewDIB);

 // 获取指针
 lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
 lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

 // 更新DIB中图像的高度和宽度
 if (IS_WIN30_DIB(lpNewDIB))
 {
  // 对于Windows 3.0 DIB
  lpbmi->biWidth = lNewWidth;
  lpbmi->biHeight = lNewHeight;
 }
 else
 {
  // 对于其它格式的DIB
  lpbmc->bcWidth = (unsigned short) lNewWidth;
  lpbmc->bcHeight = (unsigned short) lNewHeight;
 }

 // 针对图像每行进行操作
 for(i = 0; i < lNewHeight; i++)
 {
  // 针对图像每列进行操作
  for(j = 0; j < lNewWidth; j++)
  {
   // 指向新DIB第i行，第j个象素的指针
   // 注意此处宽度和高度是新DIB的宽度和高度
   lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight 

- 1 - i) + j;

   // 计算该象素在源DIB中的坐标
   i0 = (LONG) (-((float) j) * fSina + ((float) i) * fCosa + 

f2 + 0.5);
   j0 = (LONG) ( ((float) j) * fCosa + ((float) i) * fSina + 

f1 + 0.5);

   // 判断是否在源图范围内
   if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < 

lHeight))
   {
    // 指向源DIB第i0行，第j0个象素的指针
    lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight 

- 1 - i0) + j0;

    // 复制象素
    *lpDst = *lpSrc;
   }
   else
   {
    // 对于源图中没有的象素，直接赋值为255
    * ((unsigned char*)lpDst) = 255;
   }   
  } 
 }

 // 返回
 return hDIB;
}

**//顺时针旋转**void CMyDIPView::OnMenuitem32778() 
{
 // 图像旋转
 // 获取文档
 CMyDIPDoc* pDoc = GetDocument(); 
 // 指向DIB的指针
 LPSTR lpDIB;

 // 锁定DIB
 lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

 // 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的旋转，其它的可以类

推）
 if (::DIBNumColors(lpDIB) != 256)
 {
  // 提示用户
  MessageBox("目前只支持256色位图的旋转！", "系统提示" , 

MB_ICONINFORMATION | MB_OK);

  // 解除锁定
  ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

  // 返回
  return;
 }

 // 旋转角度
 int iRotateAngle;
 iRotateAngle=15;

 // 创建新DIB
 HDIB hNewDIB = NULL;

 // 更改光标形状
 BeginWaitCursor();

// 调用RotateDIB()函数旋转DIB
 hNewDIB = (HDIB) RotateDIB(lpDIB, iRotateAngle);

 // 判断旋转是否成功
 if (hNewDIB != NULL)
 {

  // 替换DIB，同时释放旧DIB对象
  pDoc->ReplaceHDIB(hNewDIB);
  // 更新DIB大小和调色板
  pDoc->InitDIBData();  
  // 设置脏标记
  pDoc->SetModifiedFlag(TRUE);  
  // 重新设置滚动视图大小
  SetScrollSizes(MM_TEXT, pDoc->GetDocSize());  
  // 更新视图
  pDoc->UpdateAllViews(NULL);
 }
 else
 {
  // 提示用户
  MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | 

MB_OK);
 }

 // 解除锁定
 ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

 // 恢复光标
 EndWaitCursor();
}

**//逆时针旋转**void CMyDIPView::OnMenuitem32779() 
{
 // 图像旋转
 // 获取文档
 CMyDIPDoc* pDoc = GetDocument();

 // 指向DIB的指针
 LPSTR lpDIB;
 // 锁定DIB
 lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

 // 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的旋转，其它的可以类

推）
 if (::DIBNumColors(lpDIB) != 256)
 {
  // 提示用户
  MessageBox("目前只支持256色位图的旋转！", "系统提示" , 

MB_ICONINFORMATION | MB_OK);

  // 解除锁定
  ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

  // 返回
  return;
 }

 // 旋转角度
 int iRotateAngle;
 iRotateAngle=-15;

 // 创建新DIB
 HDIB hNewDIB = NULL;

 // 更改光标形状
 BeginWaitCursor();

 // 调用RotateDIB()函数旋转DIB
 hNewDIB = (HDIB) RotateDIB(lpDIB, iRotateAngle);

 // 判断旋转是否成功
 if (hNewDIB != NULL)
 {

  // 替换DIB，同时释放旧DIB对象
  pDoc->ReplaceHDIB(hNewDIB);
  // 更新DIB大小和调色板
  pDoc->InitDIBData();  
  // 设置脏标记
  pDoc->SetModifiedFlag(TRUE);  
  // 重新设置滚动视图大小
  SetScrollSizes(MM_TEXT, pDoc->GetDocSize());  
  // 更新视图
  pDoc->UpdateAllViews(NULL);
 }
 else
 {
  // 提示用户
  MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | 

MB_OK);
 }

 // 解除锁定
 ::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

 // 恢复光标
 EndWaitCursor();
}

**变化效果：**

**![](http://hi.csdn.net/attachment/201103/6/8394323_1299395320LINv.jpg)**















**版权所有，侵权必究。若需转载，请注明出处，谢谢。--结构之法 算法之道。**](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)




