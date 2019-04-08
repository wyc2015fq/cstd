# 图像保边滤波算法集锦--Smart Blur算法与实现(PS2018 Smart Blur)

2018年06月15日 17:19:27

 

Trent1985

 

阅读数：879

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/80707443

新版PS中有个Smart Blur的滤镜，是一个可以保留边缘的滤波器，效果如下所示：

![img](https://img-blog.csdn.net/20180615165003978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



这个滤波器的算法实现如下：

1，Smart Blur有两个参数：Radius和Threshold

Radius是邻域半径，Threshold是当前像素与周围像素的阈值差，范围为[0,255]

2，假设当前像素点为P(x,y)，滤波结果为D，以P为中心，Radius为邻域半径，计算邻域范围内符合条件A的像素点像素值和的均值，即为M；

条件A：像素X的值在区间[P-Threshold,P+Threshold]内

3，滤波结果值D=M；

用公式表示如下：

![img](https://img-blog.csdn.net/20180615165154862?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

本人代码实现如下：

```cpp
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include"f_SmartFilter.h"
#include"TRGB2YCbCr.h"
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)
 
 
int SmartBlurOneChannel(unsigned char* srcData, int width ,int height, int radius, int threshold)
{	
	int len = sizeof(unsigned long) * width * height;
	int i, j;
	int gray = 0;
	unsigned char* tempData = (unsigned char*) malloc(sizeof(unsigned char) * height * width);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * width);
	for(j = 0; j < height; j++ )
	{
		for(i = 0; i < width; i++)
		{
			len = i + j * width;
			gray = tempData[len];
			int low = CLIP3(gray - threshold, 0, 255);
			int high = CLIP3(gray + threshold, 0, 255);
			int sum = 0;
			int count = 0;
			for(int n = -radius; n <= radius; n++)
			{
				for(int m = -radius; m <= radius; m++)
				{
					int x = CLIP3(i + m, 0, width - 1);
					int y = CLIP3(j + n, 0, height - 1);
					int pos = x + y * width;					
					gray = tempData[pos];
					if(gray > low && gray < high)
					{
						sum += gray;
					    count++;
					}
				}
			}
			gray = sum / MAX2(count, 1);						
			srcData[len] = CLIP3(gray, 0, 255);
		}
	}
	free(tempData);
	return 0;
};
 
void f_SmartFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, int threshold)
{
	if (srcData == NULL)
	{
		return;
	}
	if(radius == 0 || threshold == 0)
		return;
	unsigned char* yData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* cbData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* crData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* pSrc = srcData;
	int Y, CB, CR;
	unsigned char* pY = yData;
	unsigned char* pCb = cbData;
	unsigned char* pCr = crData;
	for(int j = 0; j < nHeight; j++)
	{
		for(int i = 0; i < nWidth; i++)
		{
 
			RGBToYCbCr(pSrc[2],pSrc[1],pSrc[0],&Y,&CB,&CR);
			*pY = Y;
			*pCb = CB;
			*pCr = CR;
			pY++;
			pCb++;
			pCr++;
			pSrc += 4;
		}
	}
	SmartBlurOneChannel(yData, nWidth, nHeight, radius, threshold);
	pSrc = srcData;
	pY = yData;
	pCb = cbData;
	pCr = crData;
	int R, G, B;
	for(int j = 0; j < nHeight; j++)
	{
		for(int i = 0; i < nWidth; i++)
		{
			YCbCrToRGB(*pY, *pCb, *pCr, &R, &G, &B);
            pSrc[0] = B;
			pSrc[1] = G;
			pSrc[2] = R;
			pY++;
			pCb++;
			pCr++;
			pSrc += 4;
		}
	}
	free(yData);
	free(cbData);
	free(crData);
}
```

本文算法效果与PS效果对比如下：

![img](https://img-blog.csdn.net/20180615170803988?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最后在放一些效果图：

![img](https://img-blog.csdn.net/20180615171524520?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

本文的DEMO连接：[点击打开链接](https://download.csdn.net/download/trent1985/10481434)

本人QQ1358009172