# 数字图像处理领域的二十四个典型算法及vc实现、第一章 - 结构之法 算法之道 - CSDN博客





2011年02月26日 18:14:00[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：52925








**数字图像处理领域的二十四个典型算法及vc实现、第一章**

作者:July   二零一一年二月二十六日。
参考：百度百科、维基百科、vc数字图像处理。
--------------------------------------------------
**数字图像处理领域的二十四个典型算法及vc实现、第一章**一、256色转灰度图
二、Walsh变换
三、二值化变换
四、阈值变换
五、傅立叶变换
六、离散余弦变换

[**数字图像处理领域的二十四个典型算法及vc实现、第二章**](http://blog.csdn.net/v_JULY_v/archive/2011/03/06/6227072.aspx)
七、高斯平滑
八、图像平移
九、图像缩放
十、图像旋转
数字图像处理领域的二十四个典型算法及vc实现、第三章



图像处理，是对图像进行分析、加工、和处理，使其满足视觉、心理以及其他要求的技术。图像处理是信号处理在图像域上的一个应用。目前大多数的图像是以数字形式存储，因而图像处理很多情况下指数字图像处理。

      本文接下来，简单粗略介绍下数字图像处理领域中的24个经典算法，然后全部算法用vc实现。由于篇幅所限，只给出某一算法的主体代码。

      ok，请细看。

**一、256色转灰度图**** 算法介绍（**百度百科）**：**
    什么叫灰度图？任何颜色都有红、绿、蓝三原色组成，假如原来某点的颜色为RGB(R，G，B)，那么，我们可以通过下面几种方法，将其转换为灰度： 　　
   1.浮点算法：Gray=R*0.3+G*0.59+B*0.11 　　
   2.整数方法：Gray=(R*30+G*59+B*11)/100 　　
  3.移位方法：Gray =(R*28+G*151+B*77)>>8; 　　
   4.平均值法：Gray=（R+G+B）/3; 　　
   5.仅取绿色：Gray=G； 　　
    通过上述任一种方法求得Gray后，将原来的RGB(R,G,B)中的R,G,B统一用Gray替换，形成新的颜色RGB(Gray,Gray,Gray)，用它替换原来的RGB(R,G,B)就是灰度图了。

灰度分为256阶。所以，用灰度表示的图像称作灰度图。

**程序实现：**    ok，知道了什么叫灰度图，下面，咱们就来实现此256色灰度图。
这个Convert256toGray()，即是将256色位图转化为灰度图：

void **Convert256toGray**(HDIB hDIB)
{
 LPSTR lpDIB; 
 // 由DIB句柄得到DIB指针并锁定DIB
 lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)hDIB); 
 // 指向DIB象素数据区的指针
 LPSTR   lpDIBBits;  
 // 指向DIB象素的指针
 BYTE * lpSrc; 
 // 图像宽度
 LONG lWidth; 
 // 图像高度
 LONG   lHeight;  
 // 图像每行的字节数
 LONG lLineBytes; 
 // 指向BITMAPINFO结构的指针（Win3.0）
 LPBITMAPINFO lpbmi; 
 // 指向BITMAPCOREINFO结构的指针
 LPBITMAPCOREINFO lpbmc;
 // 获取指向BITMAPINFO结构的指针（Win3.0）
 lpbmi = (LPBITMAPINFO)lpDIB;  
 // 获取指向BITMAPCOREINFO结构的指针
 lpbmc = (LPBITMAPCOREINFO)lpDIB; 
 // 灰度映射表
 BYTE bMap[256];

 // 计算灰度映射表（保存各个颜色的灰度值），并更新DIB调色板
 int i,j;
 for (i = 0; i < 256; i ++)
 {
  // 计算该颜色对应的灰度值
  bMap[i] = (BYTE)(0.299 * lpbmi->bmiColors[i].rgbRed +
   0.587 * lpbmi->bmiColors[i].rgbGreen +
   0.114 * lpbmi->bmiColors[i].rgbBlue + 0.5);   
  // 更新DIB调色板红色分量
  lpbmi->bmiColors[i].rgbRed = i; 
  // 更新DIB调色板绿色分量
  lpbmi->bmiColors[i].rgbGreen = i; 
   // 更新DIB调色板蓝色分量
  lpbmi->bmiColors[i].rgbBlue = i;
  // 更新DIB调色板保留位
  lpbmi->bmiColors[i].rgbReserved = 0;
 }
 // 找到DIB图像象素起始位置
 lpDIBBits = ::FindDIBBits(lpDIB);
 // 获取图像宽度
 lWidth = ::DIBWidth(lpDIB); 
 // 获取图像高度
 lHeight = ::DIBHeight(lpDIB); 
 // 计算图像每行的字节数
 lLineBytes = WIDTHBYTES(lWidth * 8); 
 // 更换每个象素的颜色索引（即按照灰度映射表换成灰度值）

 //逐行扫描
 for(i = 0; i < lHeight; i++)
 {
    //逐列扫描
  for(j = 0; j < lWidth; j++)
  {
   // 指向DIB第i行，第j个象素的指针
   lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
   // 变换
   *lpSrc = bMap[*lpSrc];
  }
 } 
 //解除锁定
 ::GlobalUnlock ((HGLOBAL)hDIB);
} 

**变换效果（以下若无特别说明，图示的右边部分都是为某一算法变换之后的效果）：**

![](http://hi.csdn.net/attachment/201102/26/8394323_1298718547M99j.png)


**二、Walsh变换****   算法介绍：**    有关Walsh变换的深入介绍，请看此论文：[http://www.informatics.org.cn/doc/ucit200510/ucit20051005.pdf](http://www.informatics.org.cn/doc/ucit200510/ucit20051005.pdf)

**程序实现:**

函数名称：WALSH()
参数:
double * f    - 指向时域值的指针
double * F    - 指向频域值的指针
r      －2的幂数
返回值:无。
说明:该函数用来实现快速沃尔什-哈达玛变换。
VOID WINAPI **WALSH**(double *f, double *F, int r)
{
 // 沃尔什-哈达玛变换点数
 LONG count;
 // 循环变量
 int  i,j,k;
 // 中间变量
 int  bfsize,p;
 double *X1,*X2,*X;
 // 计算快速沃尔什变换点数
 count = 1 << r;
 // 分配运算所需的数组
 X1 = new double[count];
 X2 = new double[count];
 // 将时域点写入数组X1
 memcpy(X1, f, sizeof(double) * count);

**// 蝶形运算** for(k = 0; k < r; k++)
 {
  for(j = 0; j < 1<<k; j++)
  {
   bfsize = 1 << (r-k);
   for(i = 0; i < bfsize / 2; i++)
   {
    p = j * bfsize;
    X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
    X2[i + p + bfsize / 2] = X1[i + p] - X1[i + p + bfsize / 2];
   }
  }
  // 互换X1和X2  
  X = X1;
  X1 = X2;
  X2 = X;
 }

 // 调整系数
 for(j = 0; j < count; j++)
 {
  p = 0;
  for(i = 0; i < r; i++)
  {
   if (j & (1<<i))
   {
    p += 1 << (r-i-1);
   }
  }

  F[j] = X1[p] / count;
 }

 // 释放内存
 delete X1;
 delete X2;
}

函数名称：DIBWalsh1()
参数:
LPSTR lpDIBBits    - 指向源DIB图像指针
LONG  lWidth       - 源图像宽度（象素数）
LONG  lHeight      - 源图像高度（象素数）
返回值:BOOL               - 成功返回TRUE，否则返回FALSE。
说明:该函数用来对图像进行沃尔什-哈达玛变换。于上面不同的是，此处是将二维
矩阵转换成一个列向量，然后对该列向量进行一次一维沃尔什-哈达玛变换。

BOOL WINAPI **DIBWalsh1**(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
 // 指向源图像的指针
 unsigned char* lpSrc;
 // 循环变量
 LONG i;
 LONG j;
 // 进行付立叶变换的宽度和高度（2的整数次方）
 LONG w;
 LONG h;
 // 中间变量
 double dTemp;
 int  wp;
 int  hp;
  // 图像每行的字节数
 LONG lLineBytes;
 // 计算图像每行的字节数
 lLineBytes = WIDTHBYTES(lWidth * 8);

 // 赋初值
 w = 1;
 h = 1;
 wp = 0;
 hp = 0;

 // 计算进行离散余弦变换的宽度和高度（2的整数次方）
 while(w * 2 <= lWidth)
 {
  w *= 2;
  wp++;
 }

 while(h * 2 <= lHeight)
 {
  h *= 2;
  hp++;
 }

 // 分配内存
 double *f = new double[w * h];
 double *F = new double[w * h];

 // 列
 for(i = 0; i < w; i++)
 {
  // 行
  for(j = 0; j < h; j++)
  {
   // 指向DIB第j行，第i个象素的指针
   lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;

   // 给时域赋值
   f[j + i * w] = *(lpSrc);
  }
 }

 // 调用快速沃尔什－哈达玛变换
 WALSH(f, F, wp + hp);
 // 列
 for(i = 0; i < w; i++)
 {
  // 行
  for(j = 0; j < h; j++)
  {
   // 计算频谱
   dTemp = fabs(F[i * w + j] * 1000);

   // 判断是否超过255
   if (dTemp > 255)
   {
    // 对于超过的，直接设置为255
    dTemp = 255;
   }
   // 指向DIB第j行，第i个象素的指针
   lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;

   // 更新源图像
   * (lpSrc) = (BYTE)(dTemp);
  }
 }

 //释放内存
 delete f;
 delete F;

 // 返回
 return TRUE;
}

**变换效果：**

![](http://hi.csdn.net/attachment/201103/2/8394323_1299045054eOER.png)


**三、二值化变换**** 算法描述：**    二值化是图像分割的一种方法。在二值化图象的时候把大于某个临界灰度值的像素灰度设为灰度極大值，把小于这个值的像素灰度设为灰度極小值，从而实现二值化。
    根据阈值选取的不同，二值化的算法分为固定阈值和自适应阈值。 比较常用的二值化方法则有：双峰法、P参数法、迭代法和OTSU法等。

**程序实现：**

void CMyDIPView::**OnDraw**(CDC* pDC)
{   
 CMyDIPDoc* pDoc = GetDocument();
 ASSERT_VALID(pDoc);
 if(pDoc->m_hDIB == NULL)
  return ;
 // TODO: add draw code for native data here
 int i,j;
    unsigned char *lpSrc;
 LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);
 int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
 int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
 LPSTR lpDIBBits=::FindDIBBits (lpDIB);
 // 计算图像每行的字节数
 long lLineBytes = WIDTHBYTES(cxDIB * 8);
 // 每行
 for(i = 0; i < cyDIB; i++)
 {
  // 每列
  for(j = 0; j < cxDIB; j++)
  {
   // 指向DIB第i行，第j个象素的指针
   lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
   // 计算新的灰度值
   //*(lpSrc) = BYTE(255-*lpSrc);
  }
 }
 ::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
 CRect rect(0,0,cxDIB,cyDIB), rcDIB(0,0,cxDIB,cyDIB);
 ::PaintDIB(pDC->m_hDC, &rect, pDoc->m_hDIB, &rcDIB, pDoc->m_palDIB);
}

void CMyDIPView::**OnMenuitem32778**() 
{
 // TODO: Add your command handler code here
 int i,j;
    unsigned char *lpSrc;
 CMyDIPDoc* pDoc = GetDocument();
 ASSERT_VALID(pDoc);
 if(pDoc->m_hDIB == NULL)
  return ;
 LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);
 LPSTR lpDIBBits=::FindDIBBits (lpDIB);
 int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
 int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
 long lLineBytes = WIDTHBYTES(cxDIB * 8);     // 计算图像每行的字节数
 const float c1=150,c2=2.5;
 // 每行
 for(i = 0; i < cyDIB; i++)
 {
  // 每列
  for(j = 0; j < cxDIB; j++)
  {
   // 指向DIB第i行，第j个象素的指针
   lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;

   // 计算新的灰度值
   if(*lpSrc<122) *lpSrc=BYTE(0);
   else *lpSrc = BYTE(255);
  }
 }
 ::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
    Invalidate(TRUE);
}

**变换效果：**

![](http://hi.csdn.net/attachment/201103/2/8394323_1299044713FnOR.png)


**四、阈值变换 算法描述：**  输入图像像元密度值(灰度、亮度值)按对数函数关系变换为输出图像。 

**程序实现：**

//参数说明:
//LPSTR lpDIBBits:指向源DIB图像指针
//LONG  lWidth:源图像宽度（象素数）
//LONG  lHeight:源图像高度（象素数）
//BYTE  bThre:阈值
//程序说明:
//该函数用来对图像进行阈值变换。对于灰度值小于阈值的象素直接设置
灰度值为0；灰度值大于阈值的象素直接设置为255。
BOOL WINAPI **ThresholdTrans**(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre)
{
 // 指向源图像的指针
 unsigned char* lpSrc;
 // 循环变量
 LONG i;
 LONG j;
 // 图像每行的字节数
 LONG lLineBytes;
 // 计算图像每行的字节数
 lLineBytes = WIDTHBYTES(lWidth * 8);
 // 每行
 for(i = 0; i < lHeight; i++)
 {
  // 每列
  for(j = 0; j < lWidth; j++)
  {
   // 指向DIB第i行，第j个象素的指针
   lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
   // 判断是否小于阈值
   if ((*lpSrc) < bThre)
   {
    // 直接赋值为0
    *lpSrc = 0;
   }
   else
   {
    // 直接赋值为255
    *lpSrc = 255;
   }
  }
 }
 // 返回
 return TRUE;
}


**五、傅立叶变换****  算法描述：**    关于此傅里叶变换算法的具体介绍，请参考本BLOG文章：[**十、从头到尾彻底理解傅里叶变换算法、上**](http://blog.csdn.net/v_JULY_v/archive/2011/02/20/6196862.aspx)。

**  程序实现：**

函数名称：FFT()
参数:
complex<double> * TD - 指向时域数组的指针
complex<double> * FD - 指向频域数组的指针
r      －2的幂数，即迭代次数
返回值:无。
说明:该函数用来实现快速付立叶变换。
VOID WINAPI **FFT**(complex<double> * TD, complex<double> * FD, int r)
{
 // 付立叶变换点数
 LONG count;
 // 循环变量
 int  i,j,k;
 // 中间变量
 int  bfsize,p;
  // 角度
 double angle;
  complex<double> *W,*X1,*X2,*X;
 // 计算付立叶变换点数
 count = 1 << r;

 // 分配运算所需存储器
 W  = new complex<double>[count / 2];
 X1 = new complex<double>[count];
 X2 = new complex<double>[count];

 // 计算加权系数
 for(i = 0; i < count / 2; i++)
 {
  angle = -i * PI * 2 / count;
  W[i] = complex<double> (cos(angle), sin(angle));
 }

 // 将时域点写入X1
 memcpy(X1, TD, sizeof(complex<double>) * count);

 // 采用蝶形算法进行快速付立叶变换
 for(k = 0; k < r; k++)
 {
  for(j = 0; j < 1 << k; j++)
  {
   bfsize = 1 << (r-k);
   for(i = 0; i < bfsize / 2; i++)
   {
    p = j * bfsize;
    X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
    X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2]) * W[i * (1<<k)];
   }
  }
  X  = X1;
  X1 = X2;
  X2 = X;
 }

 // 重新排序
 for(j = 0; j < count; j++)
 {
  p = 0;
  for(i = 0; i < r; i++)
  {
   if (j&(1<<i))
   {
    p+=1<<(r-i-1);
   }
  }
  FD[j]=X1[p];
 }

 // 释放内存
 delete W;
 delete X1;
 delete X2;
}

函数名称：Fourier()
参数:
LPSTR lpDIBBits    - 指向源DIB图像指针
LONG  lWidth       - 源图像宽度（象素数）
LONG  lHeight      - 源图像高度（象素数）
返回值:BOOL               - 成功返回TRUE，否则返回FALSE。
说明:该函数用来对图像进行付立叶变换。
BOOL WINAPI Fourier(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
 // 指向源图像的指针
 unsigned char* lpSrc;
  // 中间变量
 double dTemp;
  // 循环变量
 LONG i;
 LONG j;

 // 进行付立叶变换的宽度和高度（2的整数次方）
 LONG w;
 LONG h;
  int  wp;
 int  hp;

 // 图像每行的字节数
 LONG lLineBytes;

 // 计算图像每行的字节数
 lLineBytes = WIDTHBYTES(lWidth * 8);

 // 赋初值
 w = 1;
 h = 1;
 wp = 0;
 hp = 0;

 // 计算进行付立叶变换的宽度和高度（2的整数次方）
 while(w * 2 <= lWidth)
 {
  w *= 2;
  wp++;
 }

 while(h * 2 <= lHeight)
 {
  h *= 2;
  hp++;
 }

 // 分配内存
 complex<double> *TD = new complex<double>[w * h];
 complex<double> *FD = new complex<double>[w * h];

 // 行
 for(i = 0; i < h; i++)
 {
  // 列
  for(j = 0; j < w; j++)
  {
   // 指向DIB第i行，第j个象素的指针
   lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

   // 给时域赋值
   TD[j + w * i] = complex<double>(*(lpSrc), 0);
  }
 }

 for(i = 0; i < h; i++)
 {
  // 对y方向进行快速付立叶变换
  FFT(&TD[w * i], &FD[w * i], wp);
 }

 // 保存变换结果
 for(i = 0; i < h; i++)
 {
  for(j = 0; j < w; j++)
  {
   TD[i + h * j] = FD[j + w * i];
  }
 }

 for(i = 0; i < w; i++)
 {
  // 对x方向进行快速付立叶变换
  FFT(&TD[i * h], &FD[i * h], hp);
 }

 // 行
 for(i = 0; i < h; i++)
 {
  // 列
  for(j = 0; j < w; j++)
  {
   // 计算频谱
   dTemp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + 
             FD[j * h + i].imag() * FD[j * h + i].imag()) / 100;
   // 判断是否超过255
   if (dTemp > 255)
   {
    // 对于超过的，直接设置为255
    dTemp = 255;
   }
    // 指向DIB第(i<h/2 ? i+h/2 : i-h/2)行，第(j<w/2 ? j+w/2 : j-w/2)个象素的指针
   // 此处不直接取i和j，是为了将变换后的原点移到中心
   //lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
   lpSrc = (unsigned char*)lpDIBBits + lLineBytes * 
    (lHeight - 1 - (i<h/2 ? i+h/2 : i-h/2)) + (j<w/2 ? j+w/2 : j-w/2);

   // 更新源图像
   * (lpSrc) = (BYTE)(dTemp);
  }
 }

 // 删除临时变量
 delete TD;
 delete FD;

 // 返回
 return TRUE;
}

**变换效果：**

![](http://hi.csdn.net/attachment/201103/2/8394323_1299045279HEPY.png)

    July附注：此傅里叶变换算法，在本BLOG内有深入具体的介绍，请参考本BLOG内其它文章。



**六、离散余弦变换****算法描述：**    离散余弦变换（DCT for Discrete Cosine Transform）是与傅里叶变换相关的一种变换，它类似于离散傅里叶变换（DFT for Discrete Fourier Transform），但是只使用实数。
    离散余弦变换相当于一个长度大概是它两倍的离散傅里叶变换，这个离散傅里叶变换是对一个实偶函数进行的（因为一个实偶函数的傅里叶变换仍然是一个实偶函数），在有些变形里面需要将输入或者输出的位置移动半个单位（DCT有8种标准类型，其中4种是常见的）。

**    程序实现：**

函数名称：FFT()
参数:
complex<double> * TD - 指向时域数组的指针
complex<double> * FD - 指向频域数组的指针
r         －2的幂数，即迭代次数
返回值:无。
说明:该函数用来实现快速付立叶变换。
VOID WINAPI FFT(complex<double> * TD, complex<double> * FD, int r)
{
 // 付立叶变换点数
 LONG count;
 // 循环变量
 int  i,j,k;
  // 中间变量
 int  bfsize,p;
  // 角度
 double angle;

 complex<double> *W,*X1,*X2,*X;
  // 计算付立叶变换点数
 count = 1 << r;

 // 分配运算所需存储器
 W  = new complex<double>[count / 2];
 X1 = new complex<double>[count];
 X2 = new complex<double>[count];

 // 计算加权系数
 for(i = 0; i < count / 2; i++)
 {
  angle = -i * PI * 2 / count;
  W[i] = complex<double> (cos(angle), sin(angle));
 }

 // 将时域点写入X1
 memcpy(X1, TD, sizeof(complex<double>) * count);

 // 采用蝶形算法进行快速付立叶变换
 for(k = 0; k < r; k++)
 {
  for(j = 0; j < 1 << k; j++)
  {
   bfsize = 1 << (r-k);
   for(i = 0; i < bfsize / 2; i++)
   {
    p = j * bfsize;
    X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
    X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2]) * W[i * (1<<k)];
   }
  }
  X  = X1;
  X1 = X2;
  X2 = X;
 }

 // 重新排序
 for(j = 0; j < count; j++)
 {
  p = 0;
  for(i = 0; i < r; i++)
  {
   if (j&(1<<i))
   {
    p+=1<<(r-i-1);
   }
  }
  FD[j]=X1[p];
 }

 // 释放内存
 delete W;
 delete X1;
 delete X2;
}

**函数名称：DCT()**参数:
double * f    - 指向时域值的指针
double * F    - 指向频域值的指针
r      －2的幂数
返回值:无。
说明:该函数用来实现快速离散余弦变换，利用2N点的快速付立叶变换来实现离散余弦变换。
VOID WINAPI DCT(double *f, double *F, int r)
{
 // 离散余弦变换点数
 LONG count;
 // 循环变量
 int  i;
  // 中间变量
 double dTemp;

 complex<double> *X;
  // 计算离散余弦变换点数
 count = 1<<r;

 // 分配内存
 X = new complex<double>[count*2];
  // 赋初值为0
 memset(X, 0, sizeof(complex<double>) * count * 2);
  // 将时域点写入数组X
 for(i=0;i<count;i++)
 {
  X[i] = complex<double> (f[i], 0);
 }

 // 调用快速付立叶变换
 FFT(X,X,r+1);
 // 调整系数
 dTemp = 1/sqrt(count);
  // 求F[0]
 F[0] = X[0].real() * dTemp;
 dTemp *= sqrt(2);
 // 求F[u] 
 for(i = 1; i < count; i++)
 {
  F[i]=(X[i].real() * cos(i*PI/(count*2)) + X[i].imag() * sin(i*PI/(count*2))) * dTemp;
 }

 // 释放内存
 delete X;
}

函数名称：DIBDct()
参数:
LPSTR lpDIBBits    - 指向源DIB图像指针
LONG  lWidth       - 源图像宽度（象素数）
LONG  lHeight      - 源图像高度（象素数）
返回值:BOOL               - 成功返回TRUE，否则返回FALSE。
说明:该函数用来对图像进行离散余弦变换。
BOOL WINAPI DIBDct(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
  // 指向源图像的指针
 unsigned char* lpSrc;
  // 循环变量
 LONG i;
 LONG j;
  // 进行付立叶变换的宽度和高度（2的整数次方）
 LONG w;
 LONG h;
  // 中间变量
 double dTemp;
 int  wp;
 int  hp;

 // 图像每行的字节数
 LONG lLineBytes;
  // 计算图像每行的字节数
 lLineBytes = WIDTHBYTES(lWidth * 8);

 // 赋初值
 w = 1;
 h = 1;
 wp = 0;
 hp = 0;

 // 计算进行离散余弦变换的宽度和高度（2的整数次方）
 while(w * 2 <= lWidth)
 {
  w *= 2;
  wp++;
 }

 while(h * 2 <= lHeight)
 {
  h *= 2;
  hp++;
 }
  // 分配内存
 double *f = new double[w * h];
 double *F = new double[w * h];

 // 行
 for(i = 0; i < h; i++)
 {
  // 列
  for(j = 0; j < w; j++)
  {
   // 指向DIB第i行，第j个象素的指针
   lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

   // 给时域赋值
   f[j + i * w] = *(lpSrc);
  }
 }

 for(i = 0; i < h; i++)
 {
  // 对y方向进行离散余弦变换
  DCT(&f[w * i], &F[w * i], wp);
 }

 // 保存计算结果
 for(i = 0; i < h; i++)
 {
  for(j = 0; j < w; j++)
  {
   f[j * h + i] = F[j + w * i];
  }
 }

 for(j = 0; j < w; j++)
 {
  // 对x方向进行离散余弦变换
  DCT(&f[j * h], &F[j * h], hp);
 }
  // 行
 for(i = 0; i < h; i++)
 {
  // 列
  for(j = 0; j < w; j++)
  {
   // 计算频谱
   dTemp = fabs(F[j*h+i]);

   // 判断是否超过255
   if (dTemp > 255)
   {
    // 对于超过的，直接设置为255
    dTemp = 255;
   }

   // 指向DIB第y行，第x个象素的指针
   lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

   // 更新源图像
   * (lpSrc) = (BYTE)(dTemp);
  }
 }

 // 释放内存
 delete f;
 delete F;

 // 返回
 return TRUE;
}


**  变化效果：**

![](http://hi.csdn.net/attachment/201103/4/8394323_1299245977LTpv.jpg)

更多见下一章： [**数字图像处理领域的二十四个典型算法及vc实现、第二章**](http://blog.csdn.net/v_JULY_v/archive/2011/03/06/6227072.aspx)。本文完。

**版权所有，侵权必究。若需转载，请注明出处。谢谢。**



