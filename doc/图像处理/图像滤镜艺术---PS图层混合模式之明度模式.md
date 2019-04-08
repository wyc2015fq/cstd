# 图像滤镜艺术---PS图层混合模式之明度模式

2015年12月26日 10:52:19

 

Trent1985

 

阅读数：3169

更多

所属专栏： [图像滤镜艺术](https://blog.csdn.net/column/details/zphotoimagefilter.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/50407604

本文将介绍PS图层混合模式中比较复杂 的“明度”模式的算法原理及代码实现内容。

说到PS的图层混合模式，计算公式都有，具体代码实现也能找到，但是，都没有完整介绍所有图层混合模式的代码，比如“明度”模式，公式如下：

假设两张图的HSY颜色模式分别为： Hb,Sb,Yb---Hm,Sm,Ym

明度混合结果HSY = HbSbYm

这个公式很简单，无非就是原图的H，S分量+混合图的Y分量而已，但是具体代码如何实现，却很少有人分享，今天，我将给大家分享本人的代码。

HSY模式是一种彩色传输模型，传输基本的色差和亮度信号。如果直接使用HSY颜色空间，这个颜色空间好像很少见，具体HSY计算公式如下：



Y = 0.299R + 0.587G + 0.114B；

Cr = R - Y;

Cb = B - Y;

H = arctan(Cr/Cb);

S = sqrt(Cr * Cr + Cb * Cb);



大家可以看到，这个公式中运算复杂，但是是基于Cr, Cb分量计算的，而且，明度图层混合模式结果中实际上只改变了Y分量，因此，我们这里可以使用YCbCr颜色空间来代替HSY颜色空间实现这个功能。

YCbCr与RGB转换公式如下：



Y   = 0.257*R+0.564*G+0.098*B+16
Cb = -0.148*R-0.291*G+0.439*B+128
Cr  = 0.439*R-0.368*G-0.071*B+128



R = 1.164*(Y-16)+1.596*(Cr-128)
G = 1.164*(Y-16)-0.392*(Cb-128)-0.813*(Cr-128)
B = 1.164*(Y-16)+2.017*(Cb-128)



因此，按照上面的公式我们编码实现如下：



```cpp
#include"TRGB2YCbCr.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <string.h>
 
const float YCbCrYRF = 0.299F;              // RGB转YCbCr的系数(浮点类型）
const float YCbCrYGF = 0.587F;
const float YCbCrYBF = 0.114F;
const float YCbCrCbRF = -0.168736F;
const float YCbCrCbGF = -0.331264F;
const float YCbCrCbBF = 0.500000F;
const float YCbCrCrRF = 0.500000F;
const float YCbCrCrGF = -0.418688F;
const float YCbCrCrBF = -0.081312F;
 
const float RGBRYF = 1.00000F;            // YCbCr转RGB的系数(浮点类型）
const float RGBRCbF = 0.0000F;
const float RGBRCrF = 1.40200F;
const float RGBGYF = 1.00000F;
const float RGBGCbF = -0.34414F;
const float RGBGCrF = -0.71414F;
const float RGBBYF = 1.00000F;
const float RGBBCbF = 1.77200F;
const float RGBBCrF = 0.00000F;
 
const int Shift = 20;
const int HalfShiftValue = 1 << (Shift - 1);
 
const int YCbCrYRI = (int)(YCbCrYRF * (1 << Shift) + 0.5);         // RGB转YCbCr的系数(整数类型）
const int YCbCrYGI = (int)(YCbCrYGF * (1 << Shift) + 0.5);
const int YCbCrYBI = (int)(YCbCrYBF * (1 << Shift) + 0.5);
const int YCbCrCbRI = (int)(YCbCrCbRF * (1 << Shift) + 0.5);
const int YCbCrCbGI = (int)(YCbCrCbGF * (1 << Shift) + 0.5);
const int YCbCrCbBI = (int)(YCbCrCbBF * (1 << Shift) + 0.5);
const int YCbCrCrRI = (int)(YCbCrCrRF * (1 << Shift) + 0.5);
const int YCbCrCrGI = (int)(YCbCrCrGF * (1 << Shift) + 0.5);
const int YCbCrCrBI = (int)(YCbCrCrBF * (1 << Shift) + 0.5);
 
const int RGBRYI = (int)(RGBRYF * (1 << Shift) + 0.5);              // YCbCr转RGB的系数(整数类型）
const int RGBRCbI = (int)(RGBRCbF * (1 << Shift) + 0.5);
const int RGBRCrI = (int)(RGBRCrF * (1 << Shift) + 0.5);
const int RGBGYI = (int)(RGBGYF * (1 << Shift) + 0.5);
const int RGBGCbI = (int)(RGBGCbF * (1 << Shift) + 0.5);
const int RGBGCrI = (int)(RGBGCrF * (1 << Shift) + 0.5);
const int RGBBYI = (int)(RGBBYF * (1 << Shift) + 0.5);
const int RGBBCbI = (int)(RGBBCbF * (1 << Shift) + 0.5);
const int RGBBCrI = (int)(RGBBCrF * (1 << Shift) + 0.5);
 
void RGBToYCbCr(int R, int G, int B, int*Y,int*Cb, int* Cr)
{
	*Y  = ((YCbCrYRI * R + YCbCrYGI * G + YCbCrYBI * B + HalfShiftValue) >> Shift);
    *Cb = (128 + ((YCbCrCbRI * R + YCbCrCbGI * G + YCbCrCbBI * B + HalfShiftValue) >> Shift));
    *Cr = (128 + ((YCbCrCrRI * R + YCbCrCrGI * G + YCbCrCrBI * B + HalfShiftValue) >> Shift));
}
 
void YCbCrToRGB(int Y, int Cb, int Cr, int*R,int*G, int* B)
{
    Cb = Cb - 128; Cr = Cr - 128;
    *R = Y + ((RGBRCrI * Cr + HalfShiftValue) >> Shift);
    *G = Y + ((RGBGCbI * Cb + RGBGCrI * Cr + HalfShiftValue) >> Shift);
    *B = Y + ((RGBBCbI * Cb + HalfShiftValue) >> Shift);
    if (*R > 255) *R = 255; else if (*R < 0) *R = 0;
    if (*G > 255) *G = 255; else if (*G < 0) *G = 0;   
    if (*B > 255) *B = 255; else if (*B < 0) *B = 0;
}
int ColorBlendModeBrightness(unsigned char* baseData, unsigned char* mixData, int width, int height, int stride)
{
	int i, j, pos;
	int bY, bCb, bCr, mY, mCb, mCr, br, bg, bb, mr, mg, mb;
	unsigned char* pBase = baseData;
	unsigned char* pMix = mixData;
	int offset = stride - width * 4;
	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			bb = pBase[0];
			bg = pBase[1];
			br = pBase[2];
			mb = pMix[0];
			mg = pMix[1];
			mr = pMix[2];
			RGBToYCbCr(mr,mg,mb,&mY,&mCb,&mCr);
			RGBToYCbCr(br,bg,bb,&bY,&bCb,&bCr);
			YCbCrToRGB((mY+bY)/2, bCb, bCr, &br,&bg,&bb);//(mY+bY)/2表示透明度为50%
			pBase[0] = bb;
			pBase[1] = bg;
			pBase[2] = br;
			pBase += 4;
			pMix  += 4;
		}
		pBase += offset;
		pMix  += offset;
	}
	return 0;
 
}
```

这个就是所有编码了，而且这个颜色空间转换的代码已经经过优化，大家可以直接使用，下面我给出效果图(该效果是按照明度图层混合模式50%透明度设置得到的)：![img](https://img-blog.csdn.net/20151226105740076?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20151226105758228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

大家可以看到，效果图和PS的效果图几乎一致，最后，给出一个DEMO下载链接：[点击打开链接](http://download.csdn.net/detail/trent1985/9377913)