# 图像保边滤波算法集锦--EPM滤波算法与实现

2018年06月25日 15:37:03

 

Trent1985

 

阅读数：1147

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/80802144

EPM滤波即Edge preserved mean filter，算法本身来自于一篇公开专利，专利号为CN102509266：一种图像快速保边滤波方法；

该专利设计了一种快速保边滤波器，可以达到双边滤波的效果，而时间复杂度为O(l)，实际上该算法也是基于局部均值方差信息，来进行边缘保留的。个人觉得算法介于局部均值滤波和导向滤波之间，有类似之处，大家可以对比这三种算法以做深入了解。

算法流程如下：

![img](https://img-blog.csdn.net/20180625150732743?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

整个算法计算了一次均值，一次方差，同时算法有两个可控参数：邻域半径r和方差调整变量delta，通过控制delta的大小，来控制图像的平滑程度与细节保留程度，当delta=0时，即为原图；

同时，专利指出该算法除了具有速度快，保边效果好的优点外，还避免了一般保边滤波器可能会出现的光晕现象，大家可以自己根据本人的DEMO进行测试，这里不再累赘。

这里在给出导向滤波的算法计算流程：

![img](https://img-blog.csdn.net/20180625150845823?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

大家可以看一下，EPM滤波算法对比导向滤波，要少了不少的计算，主要包括三次均值计算和一次方差计算，因此耗时也要少很多；

本人用C代码实现的效果图如下：

![img](https://img-blog.csdn.net/20180625151258336?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过效果对比，可以看到该算法具有一定的保边能力，可以做美颜磨皮算法。

该算法对比双边滤波效果图如下：

![img](https://img-blog.csdn.net/20180625152910349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

本人C代码如下：

```cpp
//Edge Preserved mean filter
int EPMFilter(unsigned char* srcData, int width ,int height, int radius, float delta)
{
	float *data = (float*)malloc(sizeof(float) * width * height);
	float *meanIP = (float*)malloc(sizeof(float) * width * height);
	float *corrIP = (float*)malloc(sizeof(float) * width * height);
	for(int i = 0; i < width * height; i++)
	{
		data[i] = (float)srcData[i] / 255.0;
	}
	//mean and cov compute
	MeanCovMapCalculate(data, width, height, meanIP, radius);
	for(int i = 0; i < width * height; i++)
	{
		data[i] *= data[i];
	}
	//mean and cov compute
	MeanCovMapCalculate(data, width, height, corrIP, radius);
	for(int i = 0; i < width * height; i++)
	{
		corrIP[i] = corrIP[i] - meanIP[i] * meanIP[i];
	}	
	for(int i = 0; i < width * height; i++)
	{
		float t = meanIP[i] + (corrIP[i] * (srcData[i] / 255.0f - meanIP[i]) / (corrIP[i] + delta));
		srcData[i] = (unsigned char)(CLIP3(t * 255.0f, 0, 255));
	}
	free(data);
	free(meanIP);
	free(corrIP);
	return 0;
};
//4通道处理
void f_EPMFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, float delta)
{
	if (srcData == NULL)
	{
		return;
	}
	unsigned char* rData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* gData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* bData = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * nHeight);
	unsigned char* pSrc = srcData;
	int Y, CB, CR;
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
	//并行处理
	#pragma omp parallel sections  num_threads(omp_get_num_procs())
	{
		#pragma omp  section					
			EPMFilter(rData, nWidth, nHeight, radius, delta);
		#pragma omp  section
			EPMFilter(gData, nWidth, nHeight, radius, delta);
		#pragma omp  section
			EPMFilter(bData, nWidth, nHeight, radius, delta);
	}
	pSrc = srcData;
	pR = rData;
	pG = gData;
	pB = bData;
	int R, G, B;
	for(int j = 0; j < nHeight; j++)
	{
		for(int i = 0; i < nWidth; i++)
		{
            pSrc[0] = *pB;
			pSrc[1] = *pG;
			pSrc[2] = *pR;
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
//单通道处理
void f_EPMFilterOneChannel(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, float delta)
{
	if (srcData == NULL)
	{
		return;
	}
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
	EPMFilter(yData, nWidth, nHeight, radius, delta);
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
	}free(yData);free(cbData);free(crData);
}
```

EPM滤波算法由于具有保留边缘的作用，而且速度快，因此，在美颜磨皮算法中可以考虑。

最后给出一个测试DEMO：[点击打开链接](https://download.csdn.net/download/trent1985/10498116)

本人QQ：1358009172