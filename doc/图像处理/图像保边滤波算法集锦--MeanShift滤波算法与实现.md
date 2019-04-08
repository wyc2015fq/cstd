# 图像保边滤波算法集锦--MeanShift滤波算法与实现

2018年06月19日 17:26:40

 

Trent1985

 

阅读数：1323

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/80736629

本文将尝试使用MeanShift滤波来做磨皮算法；



MeanShift即均值漂移，最早由Fukunage在1975年提出，论文名字为：The Estimation of the Gradient of a density function.

MeanShift一般是指一个迭代的步骤，即先算出当前点的偏移均值，然后以此为新的起始点，继续移动，直到满足一定的结束条件；MeanShift广泛应用于图像聚类、平滑、分割和跟踪方面，本文主要讲的是图像的平滑滤波，尝试应用于人像的磨皮算法中；

我们使用一张图来讲解MeanShift的算法原理(此图来自网络)：

![img](https://img-blog.csdn.net/20180619172110647?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



Fig.1基本MeanShift算法示意图



我们假设起始位置的滤波半径为Radius，也就是图a中的蓝色圆形区域半径，图a为起始位置，假设红色点为目标像素，每个目标像素包含位置特征和像素RGB特征；

1，计算图a起始位置处，半径Radius内目标像素的位置特征和像素RGB特征的均值M，如图c所示；

2，将起始位置的初始特征(位置特征和RGB特征)更新为特征M；

3，计算M处半径Radius区域内，目标像素的均值特征M；

4，按照1-3的过程进行迭代，直到满足一定的迭代次数和限制条件；

5，图a中起始位置的RGB特征值即为迭代完成时M的RGB特征值，如图f所示；

整个过程也叫均值漂移，实际上不是位置从图a起始值漂移到了f图中的位置，而是图a和图f处的特征值归为了一类，当然这里指的是RGB像素值；

这里我们只讲最基本的MeanShift平滑滤波算法，对于改进的MeanShift算法不做讲解；

算法流程如下：

1，假设当前像素点P(i,j)，滤波半径为R，迭代次数阈值为maxIter，像素差值阈值为threshold；

2，计算以P为中心，R为半径的圆形区域S内目标像素的均值特征，包含像素rgb的均值特征和位置的均值特征(质心)，计算公式如下：

​                            ![img](https://img-blog.csdn.net/20180619172222452?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



其中K为核函数，这里取得是|x-y|；

1，将P的特征值M更新为2中计算的新特征值；

2，按照2-3的步骤进行迭代，直到满足迭代次数阈值maxIter停止，P处的像素值即迭代终结时的rgb特征值；

上述即为MeanShift平滑滤波算法的流程，该算法最大缺点为速度慢，本文用它来尝试磨皮效果，采用YCbCr颜色空间，仅对Y通道处理，以此加速；

效果图如下所示：

![img](https://img-blog.csdn.net/2018061917240215?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

完整C代码如下：

```cpp
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include"f_MeanShiftFilter.h"
#include"TRGB2YCbCr.h"
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)
 
 
int MeanShiftOneChannel(unsigned char* srcData, int width ,int height, int radius, int threshold, int maxIter)
{	
	int len = sizeof(unsigned long) * width * height;
	int i, j;
	int gray = 0, sum = 0, srcGray = 0, count = 0;
	unsigned char* tempData = (unsigned char*) malloc(sizeof(unsigned char) * height * width);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * width);
	for(j = 0; j < height; j++ )
	{
		for(i = 0; i < width; i++)
		{
			len = i + j * width;
			int nIter = 0, cx = 0, cy = 0, sumx = 0, sumy = 0;
			srcGray = tempData[len];
			cx = i;
			cy = j;
 
			while(nIter < maxIter)
			{
				sum = 0;
				sumx = 0;
				sumy = 0;
				count = 0;
				for(int y = cy - radius; y <= cy + radius; y++)
				{
					for(int x = cx - radius; x <= cx + radius; x++)
					{
						int px = CLIP3(x, 0, width - 1);
						int py = CLIP3(y, 0, height - 1);
						len = px + py * width;
						gray = tempData[len];
						if(abs(gray - srcGray) < threshold)
						{
							count++;
							sum += gray;
							sumx += x;
							sumy += y;
						}
					}
				}
				if(count == 0)
					break;
				srcGray = sum / count;
				cx = sumx / count;
				cy = sumy / count;
				nIter++;
			}
			srcData[i + j * width] = srcGray;
		}
	}
	free(tempData);
	return 0;
};
 
void f_MeanShiftFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, int threshold, int maxIter)
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
	MeanShiftOneChannel(yData, nWidth, nHeight, radius, threshold, maxIter);
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

代码已经贴出来了，这里就不给DEMO了，大家可以直接使用代码进行测试，代码中YCbCr转换函数前文博客中有，或者大家自己实现，都可以。

本人QQ1358009172