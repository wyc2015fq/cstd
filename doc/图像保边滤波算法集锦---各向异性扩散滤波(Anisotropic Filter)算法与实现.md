# 图像保边滤波算法集锦---各向异性扩散滤波(Anisotropic Filter)算法与实现

2018年06月11日 11:21:46

 

Trent1985

 

阅读数：3203

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/80625552

本篇集锦要介绍的是各向异性扩散滤波，也叫Anisotropic filter，它克服了高斯模糊的缺陷，具有保留图像边缘细节同时减少噪声的作用，因此，也是可以做人像美颜磨皮算法的；

首先，我们来了解一下各向异性扩散。

各向异性扩散也叫P-M扩散，各向异性扩散(Anisotropic diffusion)的算法可以详见论文：

Scale-space and edge detection using anisotropic diffusion

具体定义如下：

假设灰度图I(x,y)，它的各向异性扩散方程表示如下：

​                              ![img](https://img-blog.csdn.net/2018061110190912)



其中，Δ是Laplacian算子，▽是梯度算子，c(x,y,t)是扩散系数，控制扩散速率，Anisotropic filter就是选取得图像梯度函数，因此可以在扩散的时候保留图像的边缘细节信息，K是热传导系数，用来控制边缘的灵敏度；

对于Anisotropic Filter，我们可以通俗的理解，将整张图像看作是个热量场，每个像素当作热流，热流的流动取决于当前像素与周围像素的关系，如果碰到邻域像素是边缘像素，那么，它的流动扩散系数会比较小，也就是热流不想邻域像素扩散了，或者扩散减弱了，如果不是边缘像素，那扩散系数向着流动的方向改变，流过的地方也就变的平滑，如此一来，就在保留边缘的同时，平滑了噪声区域；

假设图像为I，滤波公式如下：

​      ![img](https://img-blog.csdn.net/20180611102939901)

其中,t代表迭代次数；

△表示梯度算子，四个方向的梯度公式如下：

​                           ![img](https://img-blog.csdn.net/20180611103058444)

c表示扩散系数，四个方向上的扩散系数计算如下：

​                                                                     ![img](https://img-blog.csdn.net/20180611103120722)



注意：

在这里热传导系数K越大，图像越平滑；

K对滤波效果的影响如下(t=7,lambda=0.23,offset=3)：

![img](https://img-blog.csdn.net/20180611110959264)

λ越大，图像越平滑；

λ对滤波效果的影响如下(t=7,K=10,offset=3)：

![img](https://img-blog.csdn.net/20180611112125853)

迭代次数t越多，图像滤波效果越明显；

t值对滤波效果的影响如下所示：

![img](https://img-blog.csdn.net/2018061111044265)

各向异性扩散滤波的代码实现如下：

```cpp
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)
void AnisotropicFilter(unsigned char* srcData, int width ,int height, int stride, int iter, float k, float lambda, int offset)
{
	int i, j, pos1, pos2, pos3, pos4, n, pos_src;
	int NI, SI, EI, WI;
	float cN, cS, cE, cW;
	unsigned char* grayData = (unsigned char*)malloc(sizeof(unsigned char) * stride * height);
	unsigned char* pSrc = srcData;	
	float MAP[512];
	float kk = 1.0f / (k * k);
	for(i = -255; i <= 255; i++)
	{
		MAP[i + 255] = exp(- i * i * kk) * lambda * i; 
	}
	int r, g, b;
	for(n = 0; n < iter; n++)
	{
		memcpy(grayData, srcData, sizeof(unsigned char) * height * stride);
		pSrc = srcData;
		for(j = 0; j < height; j++)
		{
			for(i = 0; i < width; i++)
			{
				pos_src = (i << 2) + j * stride;
				pos1 = (i << 2) + CLIP3((j - offset), 0, height - 1) * stride;				
				pos2 = (i << 2) + CLIP3((j + offset), 0, height - 1) * stride;				
				pos3 = (CLIP3((i - offset), 0, width - 1) << 2) + j * stride;			
				pos4 = (CLIP3((i + offset), 0, width - 1) << 2) + j * stride;
				b = grayData[pos_src];
				NI = grayData[pos1] - b;
				SI = grayData[pos2] - b;
				EI = grayData[pos3] - b;
				WI = grayData[pos4] - b;
				cN = MAP[NI + 255];// opt:exp(-NI*NI / (k * k));
				cS = MAP[SI + 255];
				cE = MAP[EI + 255];
				cW = MAP[WI + 255];
				pSrc[0] = (int)(CLIP3((b + (cN + cS + cE + cW)), 0, 255));				
 
				pos_src = pos_src + 1;
				pos1 = pos1 + 1;
				pos2 = pos2 + 1;
				pos3 = pos3 + 1;
				pos4 = pos4 + 1;
				g = grayData[pos_src];
				NI = grayData[pos1] - g;
				SI = grayData[pos2] - g;
				EI = grayData[pos3] - g;
				WI = grayData[pos4] - g;
				cN = MAP[NI + 255];
				cS = MAP[SI + 255];
				cE = MAP[EI + 255];
				cW = MAP[WI + 255];
				pSrc[1] = (int)(CLIP3((g + (cN + cS + cE + cW)), 0, 255));
 
				pos_src = pos_src + 1;
				pos1 = pos1 + 1;
				pos2 = pos2 + 1;
				pos3 = pos3 + 1;
				pos4 = pos4 + 1;
				r = grayData[pos_src];
				NI = grayData[pos1] - r;
				SI = grayData[pos2] - r;
				EI = grayData[pos3] - r;
				WI = grayData[pos4] - r;
				cN = MAP[NI + 255];
				cS = MAP[SI + 255];
				cE = MAP[EI + 255];
				cW = MAP[WI + 255];
				pSrc[2] = (int)(CLIP3((r + (cN + cS + cE + cW)), 0, 255));
				pSrc += 4;
			}
		}
	}
	free(grayData);
};
```

效果如下所示：



注：本集锦所述保边滤波器均为人像美颜测试；

由结果图可以看到，人脸的皱纹和褐斑被磨掉了，但是五官边缘还在，因此该滤波器也是可以做磨皮测试的！至于速度方面，没有太复杂的操作，因此速度也比较快，相对于标准的双边滤波和表面模糊，速度要快上好多倍的。

最后给出对应的DEMO：[点击打开链接](https://download.csdn.net/download/trent1985/10471564)

本人QQ1358009172