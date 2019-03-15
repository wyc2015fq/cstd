# ImageWarping变形算法研究---反距离加权插值（IDW）

2018年03月15日 13:11:07 [Trent1985](https://me.csdn.net/Trent1985) 阅读数：2599



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/79565274

参考论文：Image Warping with Scattered Data Interpolation

Inverse distance weighted interpolation算法(IDW)实际上就是根据给定的控制点对和控制点对的位移矢量(方向和距离)，计算控制点对周围像素的反距离加权权重影响，从而实现图像每一个像素点的位移。



![img](https://img-blog.csdn.net/20180315112451360)

代码实现如下：

```cpp
//IDW:Inverse distgance weighted interpolation method
//Reference:Image Warping with Scattered Data Interpolation
int f_IDW(unsigned char* srcData, int width, int height, int stride, int inputPoints[], int outputPoints[], int pointNum)
{
	int ret = 0; 
	unsigned char* pSrc = srcData;
	int aa, bb, cc, dd, pos, pos1, xx, yy;
    double r1, r2, r3;
	unsigned char *pSrcL1;
    unsigned char *pSrcL2;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char)* height * stride);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * stride);
	unsigned char* p = tempData;
	double w = 0, x_in, y_in, sumw, v;
	int u = 3;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			x_in = 0, y_in = 0;	
			//F function compute
			sumw = 0;
			for(int k = 0; k < pointNum; k++)
			{
				int t = (k << 1);
				v = 1.0 / (pow((double)((i - inputPoints[t]) * (i - inputPoints[t]) + (j - inputPoints[t + 1]) * (j - inputPoints[t + 1])),u));
				sumw += v;
				if(i == inputPoints[t] && j == inputPoints[t + 1])
					w = 1.0;
				else
				    w = v;		
				x_in += w * (outputPoints[t] + i - inputPoints[t]);
				y_in += w * (outputPoints[t + 1] + j - inputPoints[t + 1]);
			}
			w = 1.0 / sumw;
			x_in = x_in * w;
			y_in = y_in * w;
			//interpolation
			x_in = CLIP3(x_in, 0, width - 1);
			y_in = CLIP3(y_in, 0, height - 1);
			xx = (int)x_in;
            yy = (int)y_in; 
			pSrcL1 = p + yy * stride;
            pSrcL2 = p + CLIP3((yy + 1), 0, height - 1) * stride;
			pos = (xx << 2);
            aa = pSrcL1[pos];
            bb = pSrcL1[pos + 4];
            cc = pSrcL2[pos];
            dd = pSrcL2[pos + 4];
            r1 = aa + (bb - aa) * (x_in - xx);
            r2 = cc + (dd - cc) * (x_in - xx);
            r3 = r1 + (r2 - r1) * (y_in - yy);
            pSrc[0]=(unsigned char)(CLIP3(r3, 0, 255));//B
            aa = pSrcL1[pos + 1];
            bb = pSrcL1[pos + 4 +1];
            cc = pSrcL2[pos + 1];
            dd = pSrcL2[pos + 4 + 1];
            r1 = aa + (bb - aa) * (x_in - xx);
            r2 = cc + (dd - cc) * (x_in - xx);
            r3 = r1 + (r2 - r1) * (y_in - yy);
            pSrc[1]=(unsigned char)(CLIP3(r3, 0, 255));//G
            aa = pSrcL1[pos + 2];
            bb = pSrcL1[pos + 4 + 2];
            cc = pSrcL2[pos + 2];
            dd = pSrcL2[pos + 4 + 2];
            r1 = aa + (bb - aa) * (x_in - xx);
            r2 = cc + (dd - cc) * (x_in - xx);
            r3 = r1 + (r2 - r1) * (y_in - yy);
            pSrc[2]=(unsigned char)(CLIP3(r3, 0, 255));//R
            aa = pSrcL1[pos + 3];
            bb = pSrcL1[pos + 4 + 3];
            cc = pSrcL2[pos + 3];
            dd = pSrcL2[pos + 4 + 3];
            r1=aa + (bb - aa) * (x_in - xx);
            r2=cc + (dd - cc) * (x_in - xx);
            r3=r1 + (r2 - r1) * (y_in - yy);
            pSrc[3]=(unsigned char)(CLIP3(r3, 0, 255));//A
		   pSrc += 4;
		}
	}
	free(tempData);
	return ret;
};
```

这里本人以瘦脸效果为例：

![img](https://img-blog.csdn.net/20180315112646761)



这张图中蓝色的点是原始点对，红色点是控制点，这里只对人脸脸庞的两个蓝色点进行了偏移，得到了红色控制点，经过IDW算法处理之后的效果如下所示：

![img](https://img-blog.csdn.net/20180315112819391)

大家可以看到，人脸变瘦了，而其他的地方变化比较小(距离人脸越远，变化越小)。

以上就是IDW算法的全过程，IDW算法不单单可以做瘦脸，还可以做一些五官变形特效，具体不再展示。

关于图片，来自网络，若有侵权敬请告知。

本人QQ1358009172