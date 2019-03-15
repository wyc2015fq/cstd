# 图像保边滤波算法集锦--非局部均值NLM滤波器

2018年09月29日 18:02:02

 

Trent1985

 

阅读数：845

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/82900592

本文介绍非局部均值滤波，这种滤波器效果非常好，但是算法耗时严重，这里以效果为先，来给大家讲解。

非局部均值滤波(Non-Local Means,NLM)是Buades等人于2005年在论文“A non-local algorithm for image denoising”中提出的对传统邻域滤波方法的一种改进滤波，考虑到了图像的自相似性质，它充分利用了图像中的冗余信息，在去噪的同时能够最大程度的保持图像的细节特征。

该算法需要计算图像中所有像素与当前像素之间的相似性，考虑到这个计算量与效率的问题，一般会设定两个固定大小的窗口，一个大的搜索窗口(D×D)和一个小的邻域窗口(d×d)，邻域窗口在搜索窗口中进行滑动，根据邻域间的相似性来确定对应中心像素对当前像素的影响度，也就是权值。

下图是NLM算法执行过程，大窗口是以目标像素为中心的搜索窗口，两个灰色小窗口分别是以x，y为中心的邻域窗口。其中以y为中心的邻域窗口在搜索窗口中滑动，通过计算两个邻域窗口间的相似程度为y赋以权值w(x,y) 。

![img](https://img-blog.csdn.net/20180929173020345?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

NLM的算法流程如下：

![img](https://img-blog.csdn.net/2018092917323925?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​    关于NLM的快速算法，可以参考论文：

FromentJ. Parameter-Free Fast Pixelwise Non-Local Means Denoising[J]. Image ProcessingOn Line, 2014, 4: 300-326

本人使用C语言实现代码如下(没有使用快速算法，速度在10S以上，使用DEMO时轻耐心等待)：

```cpp
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include"f_NLM.h"
 
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)
 
 
void NLM(unsigned char* srcData, int width, int height, int D, int d, float h)
{
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * width);
	float sw = 0;
	float sum = 0;
	int px, py, cx, cy;
	float zx;
	float vxsy = 0;
	float DD = d * d;
	float HH = h * h;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			sw = 0;
			zx = 0; 
			sum = 0;
			for(int n = -D; n <= D; n++)
			{
				for(int m = -D; m <= D; m++)
				{			
					vxsy = 0;
					for(int kn = -d; kn <= d; kn++)
					{
						for(int km = -d; km <= d; km++)
						{
							cx = CLIP3(i - d + km, 0, width - 1);
							cy = CLIP3(j - d + kn, 0, height - 1);
							px = CLIP3(i + m + km, 0, width - 1);
							py = CLIP3(j + n + kn, 0, height - 1);
							vxsy += (tempData[px + py * width] - tempData[cx + cy * width]) * (tempData[px + py * width] - tempData[cx + cy * width]);
						}
					}
					vxsy = vxsy / DD;
					sw = exp(-vxsy / HH);
					zx += sw;
					int ox = CLIP3(i + m, 0, width - 1);
					int oy = CLIP3(j + n, 0, height - 1);
					sum += sw * tempData[ox + oy * width];
				}
			}
			srcData[i + j * width] = zx == 0 ? srcData[i + j * width] : CLIP3(sum / zx, 0, 255);
		}
	}
	free(tempData);
};
 
void f_NLMFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, int dRadius, int sRadius, int h)
{
	if (srcData == NULL)
	{
		return;
	}
	if(dRadius == 0 || sRadius == 0 || h == 0 || dRadius <= sRadius)
		return;
	unsigned char* rData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* gData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* bData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* pSrc = srcData;
	unsigned char* pR = rData;
	unsigned char* pG = gData;
	unsigned char* pB = bData;
	for(int j = 0; j < nHeight; j++)
	{
		for(int i = 0; i < nWidth; i++)
		{
			*pR = pSrc[2];
			*pG = pSrc[1];
			*pB = pSrc[0];
			pR++;
			pG++;
			pB++;
			pSrc += 4;
		}
	}
	NLM(rData, nWidth, nHeight, dRadius, sRadius, h);
	NLM(gData, nWidth, nHeight, dRadius, sRadius, h);
	NLM(bData, nWidth, nHeight, dRadius, sRadius, h);
	pSrc = srcData;
	pR = rData;
	pG = gData;
	pB = bData;
	for(int j = 0; j < nHeight; j++)
	{
		for(int i = 0; i < nWidth; i++)
		{
            pSrc[2] = * pR;
			pSrc[1] = * pG;
			pSrc[0] = * pB;
			pR++;
			pG++;
			pB++;
			pSrc += 4;
		}
	}
	free(rData);
	free(gData);
	free(bData);
}
```

非局部均值滤波的效果如下图所示：

![img](https://img-blog.csdn.net/20180929173444845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![img](https://img-blog.csdn.net/20180929173411633?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![img](https://img-blog.csdn.net/20180929173513147?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![img](https://img-blog.csdn.net/2018092917353081?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面就是LNM滤波器的效果了，从效果上看，用来做美颜磨皮完全没有问题，但是由于速度限制，基本上没有用它来做的，当然，作为算法学习，还是可以的。

最后给出一个源码+DEMO链接：[NLM滤波](https://download.csdn.net/download/trent1985/10696388)

注意，本文算法部分参考了网上博客内容，测试图片也均来自网络，若有侵权，敬请告知，谢谢。

本人QQ1358009172， 公众号：SF图像算法