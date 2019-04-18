# 图象处理中的边缘检测------canny算子 - 深之JohnChen的专栏 - CSDN博客

2006年02月07日 17:42:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：22222


**图象边缘检测中边界闭合性的分析与探讨**

**摘   要** 在图象边缘检测中往往要求所检测到的边缘具有封闭特性，本文详细地分析了目前常用的两种算法：哈夫变换和Canny边缘检测算法，最后，探讨边缘算子应满足的准则。
**关键词** 边缘检测；闭合性；哈夫变换；Canny算子

**1引言**

      图象的边缘是指图象局部区域亮度变化显著的部分，该区域的灰度剖面一般可以看作是一个阶跃，既从一个灰度值在很小的缓冲区域内急剧变化到另一个灰度相差较大的灰度值。图象的边缘部分集中了图象的大部分信息，图象边缘的确定与提取对于整个图象场景的识别与理解是非常重要的，同时也是图象分割所依赖的重要特征，边缘检测主要是图象的灰度变化的度量、检测和定位，自从1959提出边缘检测以来，经过五十多年的发展，已有许多中不同的边缘检测方法。在我们常用的几种用于边缘检测的算子中Laplace算子常常会产生双边界；而其他一些算子如Sobel算子又往往会形成不闭合区域。本文主要讨论了在边缘检测中，获取封闭边界区域的算法。

**2 图象边缘检测的基本步骤**

     （1）滤波。边缘检测主要基于导数计算，但受噪声影响。但滤波器在降低噪声的同时也导致边缘强度的损失。

     （2）增强。增强算法将邻域中灰度有显著变化的点突出显示。一般通过计算梯度幅值完成。

     （3）检测。但在有些图象中梯度幅值较大的并不是边缘点。最简单的边缘检测是梯度幅值阈值判定。

     （4）定位。精确确定边缘的位置。

![](http://www.ahcit.com/back/editor/UploadFile/20051227115723125.gif)

图1 边缘检测酸法的基本步骤

**3 边界闭合的算法**

**3.1 哈夫变换[3]**

      由于噪声的存在，用各种算子得到的边缘象素不连续，但是由于边缘象素之间有一定的连续性，我们就可以根据边缘象素在梯度幅度或梯度方向上的连续性把他们连接起来。具体说来，如果象素(s,t)在象素(x，y)的领域且它们的梯度幅度与梯度方向在给定的阈值下满足：



T是幅度阈值；A是角度阈值；

![](http://www.ahcit.com/back/editor/UploadFile/20051227115824768.gif)
      那么，如对所有的边缘象素都进行上述的判断和连接就可以得到一个闭合的边界。

哈夫变换方法是利用图像得全局特性而对目标轮廓进行直接检测的方法，在已知区域形状的条件下，哈夫变换可以准确地捕获到目标的边界（连续的获不连续的），并最终以连续曲线的形式输出变换结果，该变换可以从强噪声环境中将已知形状的目标准确得分割提取出来。

     哈夫变换的核心思想是: 点—线的对偶性(duality)。通过变换将图象从图像控件转换到参数空间,在图像空间中一条过点(x,y)的直线方程为y=px+q,通过代数变换可以转换为另一种形式p=-px+y,即参数空间中过点(p,q)的一条直线,如果在图像空间中保持直线的斜率和截距的不变,其在参数空间必定过点(p,q),这也就说明,在图像空间中共线的点对应参数空间共点的线. 哈夫变换就是根据上述点—线的对偶性把在图象空间中存在的直线检测问题转换为参数空间中存在的点检测问题,后者的处理要比前者简单易行得多,只需简单地累加统计即可实现对边缘的检测.

哈夫变换不仅能检测直线等一阶曲线的目标,对于园、椭圆等高阶的曲线都可以检测出来。如圆的方程为：

![](http://www.ahcit.com/back/editor/UploadFile/20051227115929770.gif)
     其参数空间是一个3D空间A(a，b，r)，原理与检测直线上的点相同，只是复杂性增加了。如果圆的半径r己知，则问题又回到了2D空间A(a，b)

     哈夫变换对已知目标的检测过程受随机噪声和曲线中断等不利因素的影响很小，而且分割出的目标是直接放到另一个“干净”的缓存中的，因此可以做到零噪声，是相当有优势的。常规的哈夫变换在理论上能对所有可以写出具体解析表达式的曲线进行目标检测，但是在实际处理时，经常待检测的目标不规则或是很难获取甚至根本没有解析式，此时就要采取广义上的哈夫变换来检测目标， 

**3．2最优的阶梯型边缘检测算法(canny边缘检测)**

     1.Canny边缘检测基本原理

     (1)图象边缘检测必须满足两个条件：一能有效地抑制噪声；二必须尽量精确确定边缘的位置。

     (2)根据对信噪比与定位乘积进行测度，得到最优化逼近算子。这就是Canny边缘检测算子。

     (3)类似与Marr（LoG）边缘检测方法，也属于先平滑后求导数的方法。

     2.Canny边缘检测算法：

     step1:用高斯滤波器平滑图象；

     step2:用一阶偏导的有限差分来计算梯度的幅值和方向；

     step3:对梯度幅值进行非极大值抑制；

     step4:用双阈值算法检测和连接边缘。

     step1:高斯平滑函数

![](http://www.ahcit.com/back/editor/UploadFile/200512271209911.gif)







      step3:非极大值抑制

      仅仅得到全局的梯度并不足以确定边缘，因此为确定边缘，必须保留局部梯度最大的点，而抑制非极大值。（non-maxima suppression,NMS）

解决方法：利用梯度的方向。

![](http://www.ahcit.com/back/editor/UploadFile/2005122712041918.gif)

图2非极大值抑制

四个扇区的标号为0到3，对应3*3邻域的四种可能组合。

      在每一点上，邻域的中心象素M与沿着梯度线的两个象素相比。如果M的梯度值不比沿梯度线的两个相邻象素梯度值大，则令M=0。

即： ![](http://www.ahcit.com/back/editor/UploadFile/2005122712056399.gif)

       step4:阈值化

       减少假边缘段数量的典型方法是对N[i，j]使用一个阈值。将低于阈值的所有值赋零值。但问题是如何选取阈值？

       解决方法：双阈值算法。双阈值算法对非极大值抑制图象作用两个阈值τ1和τ2，且2τ1≈τ2，从而可以得到两个阈值边缘图象N1［i,j］和N2［i，j］。由于N2［i，j］使用高阈值得到，因而含有很少的假边缘，但有间断(不闭合)。双阈值法要在N2［i，j］中把边缘连接成轮廓，当到达轮廓的端点时，该算法就在N1［i,j］的8邻点位置寻找可以连接到轮廓上的边缘，这样，算法不断地在N1［i,j］中收集边缘，直到将N2［i,j］连接起来为止。

**4 边缘算子应满足的准则**

![](http://www.ahcit.com/back/editor/UploadFile/2005122712131555.gif)

![](http://www.ahcit.com/back/editor/UploadFile/2005122712158612.gif)

    若满足此准则，就能保证单边缘只有一个响应。

对一个算法的性能评价可分为两个阶段进行：计算假边缘与丢失边缘的数目；测量用于估计位置和方向的误差(或误差分布)。边缘检测算法的优劣也可用品质因数(Figure of Merit)来描述。Pratt品质因数是其中一种，它着重考虑了丢失了有效的边缘、边缘定位误差和将噪声判断为边缘等三种误差。

![](http://www.ahcit.com/back/editor/UploadFile/2005122712236346.gif)

**5 结束语**

       边缘检测在图象分割、模式识别、机器视觉等中都有重要作用，人们已研究出很多种边缘检测算法，而哈夫变换和canny边缘算子等是最经典的算法，人们已在这些经典算法基础上提出一些新的改进算法。

**参考文献**

[1]贾云德．机器视觉［Ｍ］．北京：科学出版社，2000
[2]章毓晋．图象处理和分析［Ｍ］．北京：清华大学出版社，1999

[3]郎锐．数字图象处理学．北京：希望电子出版社，2002
[4]王娜，李霞．一种新的改进Canny边缘检测算法．深圳大学学报，2005，4（2），149-152

边缘提取以及边缘增强是不少图像处理软件都具有的基本功能，它的增强效果很明显，在用于
识别的应用中，图像边缘也是非常重要的特征之一。图像边缘保留了原始图像中相当重要的部分信息，
而又使得总的数据量减小了很多，这正符合特征提取的要求。在以后要谈到的霍夫变换(检测图像中的几
何形状)中，边缘提取就是前提步骤。

	这里我们只考虑灰度图像，用于图像识别的边缘提取比起仅仅用于视觉效果增强的边缘提取要
复杂一些。要给图像的边缘下一个定义还挺困难的，从人的直观感受来说，边缘对应于物体的边界。图
像上灰度变化剧烈的区域比较符合这个要求，我们一般会以这个特征来提取图像的边缘。但在遇到包含
纹理的图像上，这有点问题，比如说，图像中的人穿了黑白格子的衣服，我们往往不希望提取出来的边
缘包括衣服上的方格。但这个比较困难，涉及到纹理图像的处理等方法。

	好了，既然边缘提取是要保留图像的灰度变化剧烈的区域，从数学上，最直观的方法就是微分
(对于数字图像来说就是差分)，在信号处理的角度来看，也可以说是用高通滤波器，即保留高频信号。
这是最关键的一步，在此之前有时需要对输入图像进行消除噪声的处理。

	用于图像识别的边缘提取往往需要输出的边缘是二值图像，即只有黑白两个灰度的图像，其中
一个灰度代表边缘，另一个代表背景。此外，还需要把边缘细化成只有一个像素的宽度。总的说来边缘
提取的步骤如下：

	1，去噪声
	2，微分运算
	3，2值化处理
	4，细化

	第二步是关键，有不少书把第二步就直接称为边缘提取。实现它的算法也有很多，一般的图像
处理教科书上都会介绍好几种，如拉普拉兹算子，索贝尔算子，罗伯特算子等等。这些都是模板运算，
首先定义一个模板，模板的大小以3*3的较常见，也有2*2，5*5或更大尺寸的。运算时，把模板中心对
应到图像的每一个像素位置，然后按照模板对应的公式对中心像素和它周围的像素进行数学运算，算出
的结果作为输出图像对应像素点的值。

	需要说明的是，模板运算是图像的一种处理手段--邻域处理，有许多图像增强效果都可以采用
模板运算实现，如平滑效果，中值滤波(一种消除噪声的方法)，油画效果，图像的凹凸效果等等。这些
算法都比较简单，为人们常用。

	关于前面提到的几种边缘提取算子(拉普拉兹算子，索贝尔算子，罗伯特算子)，教科书上都有
较为详细的介绍，我这里不多说了，(手头上没有教科书，也懒得翻译英文资料)，如果你们有时间，可
以把这些方法的具体情况仔细介绍一下。这里对拉普拉兹算子和索贝尔算子补充两句。拉普拉兹算子是
2阶微分算子，也就是说，相当于求取2次微分，它的精度还算比较高，但对噪声过于敏感(有噪声的情
况下效果很差)是它的重大缺点，所以这种算子并不是特别常用。索贝尔算子是最常用的算子之一(它是
一种一阶算子)，方法简单效果也不错，但提取出的边缘比较粗，要进行细化处理。另外，索贝尔算子
也可提取出图像边缘的方向信息来，有文章论证过，在不考虑噪声的情况下，它取得的边缘信息误差不
超过7度。

	顺便说一句，往往我们在进行边缘提取时只注意到位置信息，而忽略了边缘的方向。事实上，
图像的边缘总有一定的走向，我们可以用边缘曲线的法线方向(和切线垂直的直线)来代表边缘点的方向
。在图像识别的应用中，这个方向是非常重要的信息。

	上面的几种算子是属于比较简单的方法，边缘提取的精度都不算特别高，下面介绍几种高级算
法。首先是马尔(Marr)算子，马尔是计算机视觉这门学问的奠基人，很了不起，但这些理论很难懂。他
提出的边缘提取方法可以看成两个步骤，一个是平滑作用来消除噪声，另一个是微分提取边缘，也可以
说是由两个滤波器组成，低通滤波去除噪声，高通滤波提取边缘。人们也称这种方法为LOG滤波器，这也
是根据它数学表达式和滤波器形状起的名字。也可以采用模板运算来实现这种算法，但模板的大小一般
要在7*7以上，所以运算复杂程度比索贝尔算子等要大不少，运算时间当然也长许多。

	另外一种非常重要的算法是坎尼(Canny)算子,这是坎尼在1986年写的一篇论文里仔细论述的。
他给出了判断边缘提取方法性能的指标。而坎尼算子也是图像处理领域里的标准方法，也可以说是默认
的方法。比较奇怪的是，国内的图像处理教科书中，介绍坎尼算子的很少。本人见过的书中，郑南宁的
‘计算机视觉与模式识别’(1998年)，算是介绍的比较详细的。坎尼算子在使用时要提供给一些参数，
用于控制算法的性能，实际上，对于不同的图像或不同的边缘提取目的，应该提供不同的参数，以达到
最佳效果。它也有模板运算方法，模板的大小也比较大,和提供的参数有关，标准的大小差不多是17*17
，可以根据算子的可分离性用快速算法(否则就会慢的一塌糊涂)，坎尼算子的2值化也很有特色，具有
一定的智能性。

	还有一种算法：Shen-Castan算子，大概可称为沈峻算子，总之是中国人的成果，效果和坎尼
算子不相上下，这种算法在对边缘提取好坏的判别标准上有些不同。(这种方法我没用过，好象编起程
序来，要比坎尼算子还复杂)

	在实际的图像处理与识别应用中，有时需要根据被处理图像的种类以及实际目的，量身定做算
法，边缘提取也是一样，但是基本原理都是一样的。

canny算子代码

void CreatGauss(double sigma, double **pdKernel, int *pnWidowSize);

void GaussianSmooth(SIZE sz, LPBYTE pGray, LPBYTE pResult, double sigma);

void Grad(SIZE sz, LPBYTE pGray, int *pGradX, int *pGradY, int *pMag);

void NonmaxSuppress(int *pMag, int *pGradX, int *pGradY, SIZE sz, LPBYTE pNSRst);

void EstimateThreshold(int *pMag, SIZE sz, int *pThrHigh, int *pThrLow, LPBYTE pGray, 
          double dRatHigh, double dRatLow);

void Hysteresis(int *pMag, SIZE sz, double dRatLow, double dRatHigh, LPBYTE pResult);

void TraceEdge(int y, int x, int nThrLow, LPBYTE pResult, int *pMag, SIZE sz);

void Canny(LPBYTE pGray, SIZE sz, double sigma, double dRatLow,
       double dRatHigh, LPBYTE pResult);

#include "afx.h"
#include "math.h"
#include "canny.h"

//  一维高斯分布函数，用于平滑函数中生成的高斯滤波系数
void CreatGauss(double sigma, double **pdKernel, int *pnWidowSize)
{

 LONG i;

 //数组中心点
 int nCenter;

 //数组中一点到中心点距离
 double dDis;

 //中间变量
 double dValue;
 double dSum;
 dSum = 0;

 // [-3*sigma,3*sigma] 以内数据，会覆盖绝大部分滤波系数
 *pnWidowSize = 1+ 2*ceil(3*sigma);

 nCenter = (*pnWidowSize)/2;

 *pdKernel = new double[*pnWidowSize];

 //生成高斯数据
 for(i=0;i<(*pnWidowSize);i++)
 {
  dDis = double(i - nCenter);
  dValue = exp(-(1/2)*dDis*dDis/(sigma*sigma))/(sqrt(2*3.1415926)*sigma);
  (*pdKernel)[i] = dValue;
  dSum+=dValue;

 }
 //归一化
 for(i=0;i<(*pnWidowSize);i++)
 {
  (*pdKernel)[i]/=dSum;
 }

}

//用高斯滤波器平滑原图像
void GaussianSmooth(SIZE sz, LPBYTE pGray, LPBYTE pResult, double sigma)
{
 LONG x, y;
 LONG i;

 //高斯滤波器长度
 int nWindowSize;

 //窗口长度
 int nLen;

 //一维高斯滤波器
 double *pdKernel;

 //高斯系数与图像数据的点乘
 double dDotMul;

 //滤波系数总和 
 double dWeightSum;

 double *pdTemp;
 pdTemp = new double[sz.cx*sz.cy];

 //产生一维高斯数据
 CreatGauss(sigma, &pdKernel, &nWindowSize);

 nLen = nWindowSize/2;

 //x方向滤波
 for(y=0;y<sz.cy;y++)
 {
  for(x=0;x<sz.cx;x++)
  {
   dDotMul = 0;
   dWeightSum = 0;
   for(i=(-nLen);i<=nLen;i++)
   {
    //判断是否在图像内部
    if((i+x)>=0 && (i+x)<sz.cx)
    {
     dDotMul+=(double)pGray[y*sz.cx+(i+x)] * pdKernel[nLen+i];
     dWeightSum += pdKernel[nLen+i];
    }
   }
   pdTemp[y*sz.cx+x] = dDotMul/dWeightSum;
  }
 }

 //y方向滤波
 for(x=0; x<sz.cx;x++)
 {
  for(y=0; y<sz.cy; y++)
  {
   dDotMul = 0;
   dWeightSum = 0;
   for(i=(-nLen);i<=nLen;i++)
   {
    if((i+y)>=0 && (i+y)< sz.cy)
    {
     dDotMul += (double)pdTemp[(y+i)*sz.cx+x]*pdKernel[nLen+i];
     dWeightSum += pdKernel[nLen+i];
    }
   }
   pResult[y*sz.cx+x] = (unsigned char)dDotMul/dWeightSum;
  }
 }

 delete []pdKernel;
 pdKernel = NULL;

 delete []pdTemp;
 pdTemp = NULL;

}

// 方向导数,求梯度
void Grad(SIZE sz, LPBYTE pGray,int *pGradX, int *pGradY, int *pMag)
{
 LONG y,x;

 //x方向的方向导数
 for(y=1;y<sz.cy-1;y++)
 {
  for(x=1;x<sz.cx-1;x++)
  {
   pGradX[y*sz.cx +x] = (int)( pGray[y*sz.cx+x+1]-pGray[y*sz.cx+ x-1]  );
  }
 }

 //y方向方向导数
 for(x=1;x<sz.cx-1;x++)
 {
  for(y=1;y<sz.cy-1;y++)
  {
   pGradY[y*sz.cx +x] = (int)(pGray[(y+1)*sz.cx +x] - pGray[(y-1)*sz.cx +x]);
  }
 }

 //求梯度

 //中间变量
 double dSqt1;
 double dSqt2;

 for(y=0; y<sz.cy; y++)
 {
  for(x=0; x<sz.cx; x++)
  {
   //二阶范数求梯度
   dSqt1 = pGradX[y*sz.cx + x]*pGradX[y*sz.cx + x];
   dSqt2 = pGradY[y*sz.cx + x]*pGradY[y*sz.cx + x];
   pMag[y*sz.cx+x] = (int)(sqrt(dSqt1+dSqt2)+0.5);
  }
 }
}

//非最大抑制
void NonmaxSuppress(int *pMag, int *pGradX, int *pGradY, SIZE sz, LPBYTE pNSRst)
{
 LONG y,x;
 int nPos;

 //梯度分量
 int gx;
 int gy;

 //中间变量
 int g1,g2,g3,g4;
 double weight;
 double dTmp,dTmp1,dTmp2;

 //设置图像边缘为不可能的分界点
 for(x=0;x<sz.cx;x++)
 {
  pNSRst[x] = 0;
  pNSRst[(sz.cy-1)*sz.cx+x] = 0;

 }
 for(y=0;y<sz.cy;y++)
 {
  pNSRst[y*sz.cx] = 0;
  pNSRst[y*sz.cx + sz.cx-1] = 0;
 }

 for(y=1;y<sz.cy-1;y++)
 {
  for(x=1;x<sz.cx-1;x++)
  {
   //当前点
   nPos = y*sz.cx + x;

   //如果当前像素梯度幅度为0，则不是边界点
   if(pMag[nPos] == 0)
   {
    pNSRst[nPos] = 0;
   }
   else
   {
    //当前点的梯度幅度
    dTmp = pMag[nPos];

    //x,y方向导数
    gx = pGradX[nPos];
    gy = pGradY[nPos];

    //如果方向导数y分量比x分量大，说明导数方向趋向于y分量
    if(abs(gy) > abs(gx))
    {
     //计算插值比例
     weight = fabs(gx)/fabs(gy);

     g2 = pMag[nPos-sz.cx];
     g4 = pMag[nPos+sz.cx];

     //如果x,y两个方向导数的符号相同
     //C 为当前像素，与g1-g4 的位置关系为：
     //g1 g2
     //      C
     //       g4 g3
     if(gx*gy>0)
     {
      g1 = pMag[nPos-sz.cx-1];
      g3 = pMag[nPos+sz.cx+1];
     }

     //如果x,y两个方向的方向导数方向相反
     //C是当前像素，与g1-g4的关系为：
     //       g2 g1
     //        C
     //    g3 g4
     else
     {
      g1 = pMag[nPos-sz.cx+1];
      g3 = pMag[nPos+sz.cx-1];
     }
    }

    //如果方向导数x分量比y分量大，说明导数的方向趋向于x分量
    else
    {
     //插值比例
     weight = fabs(gy)/fabs(gx);

     g2 = pMag[nPos+1];
     g4 = pMag[nPos-1];

     //如果x,y两个方向的方向导数符号相同
     //当前像素C与 g1-g4的关系为
     //  g3
     //  g4 C g2
     //       g1
     if(gx * gy > 0)
     {
      g1 = pMag[nPos+sz.cx+1];
      g3 = pMag[nPos-sz.cx-1];
     }

     //如果x,y两个方向导数的方向相反
     // C与g1-g4的关系为
     //   g1
     //    g4 C g2
     //     g3
     else
     {
      g1 = pMag[nPos-sz.cx+1];
      g3 = pMag[nPos+sz.cx-1];
     }
    }

    //利用 g1-g4 对梯度进行插值
    {
     dTmp1 = weight*g1 + (1-weight)*g2;
     dTmp2 = weight*g3 + (1-weight)*g4;

     //当前像素的梯度是局部的最大值
     //该点可能是边界点
     if(dTmp>=dTmp1 && dTmp>=dTmp2)
     {
      pNSRst[nPos] = 128;
     }
     else
     {
      //不可能是边界点
      pNSRst[nPos] = 0;
     }
    }
   }
  }
 }
}

// 统计pMag的直方图，判定阈值
void EstimateThreshold(int *pMag, SIZE sz, int *pThrHigh, int *pThrLow, LPBYTE pGray, 
          double dRatHigh, double dRatLow)
{
 LONG y,x,k;

 //该数组的大小和梯度值的范围有关，如果采用本程序的算法
 //那么梯度的范围不会超过pow(2,10)
 int nHist[256];

 //可能边界数
 int nEdgeNum;

 //最大梯度数
 int nMaxMag;

 int nHighCount;

 nMaxMag = 0;

 //初始化
 for(k=0;k<256;k++)
 {
  nHist[k] = 0;
 }
 //统计直方图,利用直方图计算阈值
 for(y=0;y<sz.cy;y++)
 {
  for(x=0;x<sz.cx;x++)
  {
   if(pGray[y*sz.cx+x]==128)
   {
    nHist[pMag[y*sz.cx+x]]++;
   }
  }
 }

 nEdgeNum = nHist[0];
 nMaxMag = 0;

 //统计经过“非最大值抑制”后有多少像素
 for(k=1;k<256;k++)
 {
  if(nHist[k] != 0)
  {
   nMaxMag = k;
  }

  //梯度为0的点是不可能为边界点的
  //经过non-maximum suppression后有多少像素
  nEdgeNum += nHist[k];

 }

 //梯度比高阈值*pThrHigh 小的像素点总书目
 nHighCount = (int)(dRatHigh * nEdgeNum + 0.5);

 k=1;
 nEdgeNum = nHist[1];

 //计算高阈值
 while((k<(nMaxMag-1)) && (nEdgeNum < nHighCount))
 {
  k++;
  nEdgeNum += nHist[k];
 }

 *pThrHigh = k;

 //低阈值
 *pThrLow = (int)((*pThrHigh) * dRatLow + 0.5);

}

//利用函数寻找边界起点
void Hysteresis(int *pMag, SIZE sz, double dRatLow, double dRatHigh, LPBYTE pResult)
{
 LONG y,x;

 int nThrHigh,nThrLow;

 int nPos;
 //估计TraceEdge 函数需要的低阈值，以及Hysteresis函数使用的高阈值
 EstimateThreshold(pMag, sz,&nThrHigh,&nThrLow,pResult,dRatHigh,dRatLow);

 //寻找大于dThrHigh的点，这些点用来当作边界点，
 //然后用TraceEdge函数跟踪该点对应的边界
 for(y=0;y<sz.cy;y++)
 {
  for(x=0;x<sz.cx;x++)
  {
   nPos = y*sz.cx + x;

   //如果该像素是可能的边界点，并且梯度大于高阈值，
   //该像素作为一个边界的起点
   if((pResult[nPos]==128) && (pMag[nPos] >= nThrHigh))
   {
    //设置该点为边界点
    pResult[nPos] = 255;
    TraceEdge(y,x,nThrLow,pResult,pMag,sz);
   }

  }
 }

 //其他点已经不可能为边界点
 for(y=0;y<sz.cy;y++)
 {
  for(x=0;x<sz.cx;x++)
  {
   nPos = y*sz.cx + x;

   if(pResult[nPos] != 255)
   {
    pResult[nPos] = 0;
   }
  }
 }
}

//根据Hysteresis 执行的结果，从一个像素点开始搜索，搜索以该像素点为边界起点的一条边界的
//一条边界的所有边界点，函数采用了递归算法
//       从（x,y)坐标出发，进行边界点的跟踪，跟踪只考虑pResult中没有处理并且可能是边界
//  点的像素（=128），像素值为0表明该点不可能是边界点，像素值为255表明该点已经是边界点

void TraceEdge(int y, int x, int nThrLow, LPBYTE pResult, int *pMag, SIZE sz)
{
 //对8邻域像素进行查询
 int xNum[8] = {1,1,0,-1,-1,-1,0,1};
 int yNum[8] = {0,1,1,1,0,-1,-1,-1};

 LONG yy,xx,k;

 for(k=0;k<8;k++)
 {
  yy = y+yNum[k];
  xx = x+xNum[k];

  if(pResult[yy*sz.cx+xx]==128 && pMag[yy*sz.cx+xx]>=nThrLow )
  {
   //该点设为边界点
   pResult[yy*sz.cx+xx] = 255;

   //以该点为中心再进行跟踪
   TraceEdge(yy,xx,nThrLow,pResult,pMag,sz);
  }
 }
}

// Canny算子
void Canny(LPBYTE pGray, SIZE sz, double sigma, double dRatLow,
       double dRatHigh, LPBYTE pResult)
{
 //经过高斯滤波后的图像
 LPBYTE pGaussSmooth;

 pGaussSmooth = new unsigned char[sz.cx*sz.cy];

 //x方向导数的指针
 int *pGradX;
 pGradX = new int[sz.cx*sz.cy];

 //y方向
 int *pGradY;
 pGradY = new int[sz.cx*sz.cy];

 //梯度的幅度
 int *pGradMag;
 pGradMag = new int[sz.cx*sz.cy];

 //对原图高斯滤波
 GaussianSmooth(sz,pGray,pGaussSmooth,sigma);

 //计算方向导数和梯度的幅度
 Grad(sz,pGaussSmooth,pGradX,pGradY,pGradMag);

 //应用非最大抑制
 NonmaxSuppress(pGradMag,pGradX,pGradY,sz,pResult);

 //应用Hysteresis，找到所有边界
 Hysteresis(pGradMag,sz,dRatLow,dRatHigh,pResult);

 delete[] pGradX;
 pGradX = NULL;
 delete[] pGradY;
 pGradY = NULL;
 delete[] pGradMag;
 pGradMag = NULL;
 delete[] pGaussSmooth;
 pGaussSmooth = NULL;

}

/*
void CChildWnd::OnCanny() 
{
 if (! m_fOpenFile) 
 {
  return;
 }
 m_fDone = TRUE; 
 RGBToGray(szImg, aRGB, aGray, BPP);
 Canny(aGray,szImg,0.1,0.9,0.76,aBinImg);

 ShowGrayImage("l",szImg,aBinImg);
}
//*/

