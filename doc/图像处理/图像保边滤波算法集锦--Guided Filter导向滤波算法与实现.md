# 图像保边滤波算法集锦--Guided Filter导向滤波算法与实现

2018年06月22日 14:36:48

 

Trent1985

 

阅读数：977

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/80773217

本文讲解导向滤波的算法与实现；



导向滤波是何凯明在2010年提出的一种滤波器，论文名称Guided Image filter，算法如下所示：

![img](https://img-blog.csdn.net/20180622142425666?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

整体而言是基于均值和方差的一种保边滤波器，效果如下图所示：

![img](https://img-blog.csdn.net/20180622142950486?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

代码实现如下：

```cpp
int GuidedFilter(unsigned char* srcData, int width ,int height, int radius, float delta)
{
	float *data = (float*)malloc(sizeof(float) * width * height);
	float *meanIP = (float*)malloc(sizeof(float) * width * height);
	float *corrIP = (float*)malloc(sizeof(float) * width * height);
	float *varIP = (float*)malloc(sizeof(float) * width * height);
	float *a = (float*)malloc(sizeof(float) * width * height);
	float *b = (float*)malloc(sizeof(float) * width * height);
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
		varIP[i] = corrIP[i] - meanIP[i] * meanIP[i];
	}
	for(int i = 0; i < width * height; i++)
	{
		a[i] = varIP[i] / (varIP[i] + delta);
		b[i] = meanIP[i] - a[i] * meanIP[i];
	}
	//mean and cov compute
    MeanCovMapCalculate(a, width, height, meanIP, radius);
	MeanCovMapCalculate(b, width, height, corrIP, radius);
	for(int i = 0; i < width * height; i++)
	{
		srcData[i] = (unsigned char)(CLIP3((meanIP[i] * srcData[i]/255.0f + corrIP[i])*255.0f, 0, 255));
	}
	free(data);
	free(meanIP);
	free(corrIP);
	free(varIP);
	free(a);
	free(b);
	return 0;
};
void f_GuidedFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, float delta)
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
	#pragma omp parallel sections  num_threads(omp_get_num_procs())
	{
		#pragma omp  section					
			GuidedFilter(rData, nWidth, nHeight, radius, delta);
		#pragma omp  section
			GuidedFilter(gData, nWidth, nHeight, radius, delta);
		#pragma omp  section
			GuidedFilter(bData, nWidth, nHeight, radius, delta);
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
void f_GuidedFilterOneChannel(unsigned char* srcData, int nWidth, int nHeight, int nStride, int radius, float delta)
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
	GuidedFilter(yData, nWidth, nHeight, radius, delta);
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





本人代码是在YCbCr颜色空间，只对Y分量进行处理的，这样可以尽可能的加快速度，但是会牺牲一点效果，大家可以根据需要自行处理。

最后给出DEMO链接：[点击打开链接](https://download.csdn.net/download/trent1985/10493061)

本人QQ： 1358009172