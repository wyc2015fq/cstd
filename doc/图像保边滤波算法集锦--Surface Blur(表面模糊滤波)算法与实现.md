# 图像保边滤波算法集锦--Surface Blur(表面模糊滤波)算法与实现

2018年06月22日 16:14:53

 

Trent1985

 

阅读数：2367

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/80775011

本文介绍经典的表面模糊Surface Blur算法与实现。

表面模糊是PS里一个重要的保边滤波器，它的算法很简单，公式如下所示：

![img](https://img-blog.csdn.net/20180622160322845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

主要思想还是计算当前像素X的邻域范围内不同像素的加权求和，边缘地方的像素，加权比较大，平滑的地方加权比较小，以此来保留边缘信息，平滑平坦区域；

效果如下图所示：

![img](https://img-blog.csdn.net/20180622161418511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

代码实现如下：

```cpp
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)
void f_SurfaceBlur(unsigned char* srcData, int width, int height, int stride, int radius, int threshold)
{
	if (srcData == NULL)
	{
		return;
	}
	float sumr = 0, sumrw = 0, sumg = 0, sumgw = 0, sumb = 0, sumbw = 0, k = 0;
	int pos = 0, pos0 = 0;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * stride);
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			pos = i * 4 + j * stride;
			sumr = sumrw = sumg = sumgw = sumb = sumbw = 0;
			for(int n = -radius; n <= radius; n++)
			{
				for(int m = -radius; m <= radius; m++)
				{
					int x = CLIP3(i + m, 0, width - 1);
					int y = CLIP3(j + n, 0, height - 1);
					pos0 = x * 4 + y * stride;
					k = 1.0f - abs(tempData[pos0] - tempData[pos]) / (2.5f * threshold);
					sumb += k * tempData[pos0];
					sumbw += k;
 
					k = 1.0f - abs(tempData[pos0 + 1] - tempData[pos + 1]) / (2.5f * threshold);
					sumg += k * tempData[pos0 + 1];
					sumgw += k;
 
					k = 1.0f - abs(tempData[pos0 + 2] - tempData[pos + 2]) / (2.5f * threshold);
					sumr += k * tempData[pos0 + 2];
					sumrw += k;
				}
			}
			srcData[pos]     = sumbw == 0 ? srcData[pos]     : CLIP3(sumb / sumbw, 0, 255);
			srcData[pos + 1] = sumgw == 0 ? srcData[pos + 1] : CLIP3(sumg / sumgw, 0, 255);
			srcData[pos + 2] = sumrw == 0 ? srcData[pos + 2] : CLIP3(sumr / sumrw, 0, 255);
		}
	}
	free(tempData);
}
```

代码没有经过优化，是完全按照公式实现的，如果考虑优化，可以考虑使用多线程，或者是YCbCr颜色空间只对Y处理，以此加速，效果会比较明显。

最后给出一个Y通道处理的DEMO：[点击打开链接](https://download.csdn.net/download/trent1985/10493397)

本人QQ： 1358009172