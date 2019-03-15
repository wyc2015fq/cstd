# 数字图像算法研究---PS USM锐化算法详解

2018年09月10日 13:16:15 [Trent1985](https://me.csdn.net/Trent1985) 阅读数：3185



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/82588186

​    图像USM锐化是图像处理软件中常见的功能，而PHOTOSHOP中的USM锐化相对而言效果较好，网上有很多揭秘PhotoShop USM锐化算法的文章，但是，算法效果与PS相比，差距较大，今天本人针对PS的USM锐化，给出一个效果比较接近的算法，跟大家分享一下，有助于大家走出误区。

PS中的USM锐化有三个参数：Radius/Amout/Threshold，如下图1所示：

![img](https://img-blog.csdn.net/20180910130403201?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

网络中流行的算法(来自Imageshop)，连接如下：[Imageshop USM](https://www.cnblogs.com/Imageshop/archive/2013/05/19/3086388.html)

算法过程如下：

![img](https://img-blog.csdn.net/20180910130557702?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

以Lena测试图为例，使用Imgeshop提供的DEMO测试如下图2：

![img](https://img-blog.csdn.net/20180910130644609?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对比PS如下图3：

![img](https://img-blog.csdn.net/20180910130748163?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

问题所在如下图4所示：

![img](https://img-blog.csdn.net/20180910130835410?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如图4中所示，在蓝色标记区域，imageshop所提算法存在块状不连续问题，而PS算法非常自然，过渡平滑。

造成这个问题的原因是在算法3中我们使用了一个简单的阈值Threshold，将图像锐化与非锐化区域分成了两类，两类之间没有过渡，所以出现了这个现象。

这里给出本人的算法：

![img](https://img-blog.csdn.net/20180910130934197?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

代码实现如下：

```cpp
int f_USM(unsigned char* srcData,int width, int height,int stride,int radius, int amount, int threshold)
{
	int ret = 0;
    if(radius == 0)
		return ret;
	radius = CLIP3(radius, 0, 100);
	amount = CLIP3(amount, 0,500);
	threshold = CLIP3(threshold, 0,255);
	unsigned char* gaussData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(gaussData, srcData, sizeof(unsigned char) * height * stride);
	f_FastGaussFilter(gaussData, width, height, stride, radius);
	int i, j, r, g, b, offset;
	offset = stride - width * 3;
	amount = amount * 128 / 100;
	unsigned char* pSrc = srcData;
	unsigned char* pDst = gaussData;
	unsigned char* maskData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	unsigned char* pMask = maskData;
	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			pMask[0] = abs(pSrc[0] - pDst[0]) < threshold ? 0 : 128;
			pMask[1] = abs(pSrc[1] - pDst[1]) < threshold ? 0 : 128;
			pMask[2] = abs(pSrc[2] - pDst[2]) < threshold ? 0 : 128;
			pDst += 3;
			pSrc += 3;
			pMask += 3;
		}
		pDst += offset;
		pSrc += offset;
		pMask += offset;
	}
	pDst = gaussData;
	pSrc = srcData;
	pMask = maskData;
	f_FastGaussFilter(maskData, width, height, stride, radius);
	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			b = pSrc[0] - pDst[0];
			g = pSrc[1] - pDst[1];
			r = pSrc[2] - pDst[2];
 
			b = (pSrc[0] + ((b * amount) >> 7));
			g = (pSrc[1] + ((g * amount) >> 7));
			r = (pSrc[2] + ((r * amount) >> 7));
			
			b = (b * pMask[0] + pSrc[0] * (128 - pMask[0])) >> 7;
			g = (g * pMask[1] + pSrc[1] * (128 - pMask[1])) >> 7;
			r = (r * pMask[2] + pSrc[2] * (128 - pMask[2])) >> 7;
 
			pSrc[0] = CLIP3(b, 0, 255);
			pSrc[1] = CLIP3(g, 0, 255);
			pSrc[2] = CLIP3(r, 0, 255);
			pSrc += 3;
			pDst += 3;
			pMask += 3;
		}
		pSrc += offset;
		pDst += offset;
		pMask += offset;
	}
	free(gaussData);
	free(maskData);
	return ret;
};
```

本文算法的效果如下图5：

![img](https://img-blog.csdn.net/20180910131021566?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在图5中，我们可以看到，之前蓝色区域的像素过渡平滑自然，跟PS效果接近。

上述内容是本人对PS USM锐化算法的一些研究，主要针对网络中比较流行的USM算法进行了对比分析，算法改进，以得到更接近于PS USM锐化的算法，跟大家分享一下！

本人QQ1358009172，微信公众号:SF图像算法