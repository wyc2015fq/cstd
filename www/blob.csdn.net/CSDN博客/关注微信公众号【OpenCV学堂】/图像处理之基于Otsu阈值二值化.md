# 图像处理之基于Otsu阈值二值化 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年02月23日 12:33:29[gloomyfish](https://me.csdn.net/jia20003)阅读数：15396








**图像处理之基于Otsu阈值实现图像二值化**

**一：基本原理**

该方法是图像二值化处理常见方法之一，在Matlab与OpenCV中均有实现。

Otsu Threshing方法是一种基于寻找合适阈值实现二值化的方法，其最重

要的部分是寻找图像二值化阈值，然后根据阈值将图像分为前景（白色）

或者背景（黑色）。假设有6x6的灰度图像，其像素数据及其对应的直方

图如下图：

![](https://img-blog.csdn.net/20150223122638812)




阈值寻找方法首先假设是为T=3，则背景像素的比重、均值、方差的计算

结果如下：

![](https://img-blog.csdn.net/20150223122945854)




根据前景像素直方图，计算比重、均值、方差的过程如下：
![](https://img-blog.csdn.net/20150223123039224)

上述整个计算步骤与结果是假设阈值T=3时候的结果，同样计算假设阈值为

T=0、T=1、T=2、T=4、T=5的类内方差，比较类内方差之间的值，最小类

内方差使用的阈值T即为图像二值化的阈值。上述是假设图像灰度值级别为

0～5六个值，实际中图像灰度值取值范围为0～255之间，所以要循环计算

使用每个灰度值作为阈值，得到类内方差，最终取最小类内方差对应的灰度

值作为阈值实现图像二值化即可。

**二：代码实现**

```java
package com.gloomyfish.filter.study;

import java.awt.image.BufferedImage;

public class OtsuBinaryFilter extends AbstractBufferedImageOp {
	
	public OtsuBinaryFilter()
	{
		System.out.println("Otsu Threshold Binary Filter...");
	}

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();

        if ( dest == null )
            dest = createCompatibleDestImage( src, null );
        // 图像灰度化
        int[] inPixels = new int[width*height];
        int[] outPixels = new int[width*height];
        getRGB( src, 0, 0, width, height, inPixels );
        int index = 0;
        for(int row=0; row<height; row++) {
        	int ta = 0, tr = 0, tg = 0, tb = 0;
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		ta = (inPixels[index] >> 24) & 0xff;
                tr = (inPixels[index] >> 16) & 0xff;
                tg = (inPixels[index] >> 8) & 0xff;
                tb = inPixels[index] & 0xff;
				int gray= (int)(0.299 *tr + 0.587*tg + 0.114*tb);
				inPixels[index]  = (ta << 24) | (gray << 16) | (gray << 8) | gray;
        	}
        }
        // 获取直方图
        int[] histogram = new int[256];
        for(int row=0; row<height; row++) {
        	int tr = 0;
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
                tr = (inPixels[index] >> 16) & 0xff;
                histogram[tr]++;
        	}
        }
        // 图像二值化 - OTSU 阈值化方法
        double total = width * height;
        double[] variances = new double[256];
        for(int i=0; i<variances.length; i++)
        {
        	double bw = 0;
        	double bmeans = 0;
        	double bvariance = 0;
        	double count = 0;
        	for(int t=0; t<i; t++)
        	{
        		count += histogram[t];
        		bmeans += histogram[t] * t;
        	}
        	bw = count / total;
        	bmeans = (count == 0) ? 0 :(bmeans / count);
        	for(int t=0; t<i; t++)
        	{
        		bvariance += (Math.pow((t-bmeans),2) * histogram[t]);
        	}
        	bvariance = (count == 0) ? 0 : (bvariance / count);
        	double fw = 0;
        	double fmeans = 0;
        	double fvariance = 0;
        	count = 0;
        	for(int t=i; t<histogram.length; t++)
        	{
        		count += histogram[t];
        		fmeans += histogram[t] * t;
        	}
        	fw = count / total;
        	fmeans = (count == 0) ? 0 : (fmeans / count);
        	for(int t=i; t<histogram.length; t++)
        	{
        		fvariance += (Math.pow((t-fmeans),2) * histogram[t]);
        	}
        	fvariance = (count == 0) ? 0 : (fvariance / count);
        	variances[i] = bw * bvariance + fw * fvariance;
        }

        // find the minimum within class variance
        double min = variances[0];
        int threshold = 0;
        for(int m=1; m<variances.length; m++)
        {
        	if(min > variances[m]){
        		threshold = m;
        		min = variances[m];
        	}
        }
        // 二值化
        System.out.println("final threshold value : " + threshold);
        for(int row=0; row<height; row++) {
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
                int gray = (inPixels[index] >> 8) & 0xff;
                if(gray > threshold)
                {
                	gray = 255;
                	outPixels[index]  = (0xff << 24) | (gray << 16) | (gray << 8) | gray;
                }
                else
                {
                	gray = 0;
                	outPixels[index]  = (0xff << 24) | (gray << 16) | (gray << 8) | gray;
                }
				
        	}
        }
        setRGB(dest, 0, 0, width, height, outPixels );
        return dest;
	}

}
```

**运行效果图：**

![](https://img-blog.csdn.net/20150223125748972)


**转载文章请注明，博主在此祝各位2015年工作顺利，请继续关注！**



