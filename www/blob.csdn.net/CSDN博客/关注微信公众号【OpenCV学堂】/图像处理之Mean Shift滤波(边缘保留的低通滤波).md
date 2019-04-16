# 图像处理之Mean Shift滤波(边缘保留的低通滤波) - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年11月22日 23:27:53[gloomyfish](https://me.csdn.net/jia20003)阅读数：11190
所属专栏：[Java数字图像处理与特效](https://blog.csdn.net/column/details/image-process.html)









**一：Mean Shift算法介绍**

Mean Shift是一种聚类算法，在数据挖掘，图像提取，视频对象跟踪中都有应用。本文

重要演示Mean Shift算法来实现图像的低通边缘保留滤波效果。其处理以后的图像有点

类似油画一样。Mean Shift算法的输入参数一般有三个：

1.      矩阵半径r，声明大小

2.      像素距离，常见为欧几里德距离或者曼哈顿距离

3.      像素差值value



算法大致的流程如下：

a.      输入像素点P(x, y)

b.      计算该点的像素值pixelv

c.      根据输入的半径r与差值value求出矩阵半径内满足差值像素平均值作为输出像素点值

d.      计算shift与repetition，如果满足条件

e.      继续c ~ d，直到条件不满足退出，得到最终的输出像素值

f.       对输入图像的每个像素重复a ~ e，得到图像输出像素数据

二：色彩空间转换

 本文Mean Shift滤波在YIQ颜色空间上完成，关于RGB与YIQ颜色空间转换可以参考

这里：[http://en.wikipedia.org/wiki/YIQ](http://en.wikipedia.org/wiki/YIQ)我google找来的转换公式截屏：

![](https://img-my.csdn.net/uploads/201211/22/1353598020_3612.png)


三：程序效果

![](https://img-my.csdn.net/uploads/201211/22/1353598184_8433.png)


滤镜源代码：



```java
package com.gloomyfish.filter.study;

import java.awt.image.BufferedImage;

public class MeanShiftFilter extends AbstractBufferedImageOp {
	
	private int radius;
	private float colorDistance;
	
	public MeanShiftFilter() {
		radius = 3; // default shift radius
		colorDistance = 25; // default color distance
	}
	public int getRadius() {
		return radius;
	}

	public void setRadius(int radius) {
		this.radius = radius;
	}

	public float getColorDistance() {
		return colorDistance;
	}

	public void setColorDistance(float colorDistance) {
		this.colorDistance = colorDistance;
	}

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();

        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );

        int[] inPixels = new int[width*height];
        int[] outPixels = new int[width*height];
        getRGB( src, 0, 0, width, height, inPixels);
        
        // convert RGB color space to YIQ color space
        float[][] pixelsf = new float[width*height][3];
        for(int i=0; i<inPixels.length; i++) {
			int argb = inPixels[i];
			int r = (argb >> 16) & 0xff;
			int g = (argb >>  8) & 0xff;
			int b = (argb) & 0xff;
			pixelsf[i][0] = 0.299f  *r + 0.587f *g + 0.114f  *b; // Y
			pixelsf[i][1] = 0.5957f *r - 0.2744f*g - 0.3212f *b; // I
			pixelsf[i][2] = 0.2114f *r - 0.5226f*g + 0.3111f *b; // Q
        }
        
        int index = 0;
        float shift = 0;
        float repetition = 0;
        float radius2 = radius * radius;
        float dis2 = colorDistance * colorDistance;
        for(int row=0; row<height; row++) {
        	int ta = 255, tr = 0, tg = 0, tb = 0;
        	for(int col=0; col<width; col++) {
				int xc = col;
				int yc = row;
				int xcOld, ycOld;
				float YcOld, IcOld, QcOld;
				index = row*width + col;
				float[] yiq = pixelsf[index];
				float Yc = yiq[0];
				float Ic = yiq[1];
				float Qc = yiq[2];

				repetition = 0;
				do {
					xcOld = xc;
					ycOld = yc;
					YcOld = Yc;
					IcOld = Ic;
					QcOld = Qc;

					float mx = 0;
					float my = 0;
					float mY = 0;
					float mI = 0;
					float mQ = 0;
					int num=0;

					for (int ry=-radius; ry <= radius; ry++) {
						int y2 = yc + ry; 
						if (y2 >= 0 && y2 < height) {
							for (int rx=-radius; rx <= radius; rx++) {
								int x2 = xc + rx; 
								if (x2 >= 0 && x2 < width) {
									if (ry*ry + rx*rx <= radius2) {
										yiq = pixelsf[y2*width + x2];

										float Y2 = yiq[0];
										float I2 = yiq[1];
										float Q2 = yiq[2];

										float dY = Yc - Y2;
										float dI = Ic - I2;
										float dQ = Qc - Q2;

										if (dY*dY+dI*dI+dQ*dQ <= dis2) {
											mx += x2;
											my += y2;
											mY += Y2;
											mI += I2;
											mQ += Q2;
											num++;
										}
									}
								}
							}
						}
					}
					float num_ = 1f/num;
					Yc = mY*num_;
					Ic = mI*num_;
					Qc = mQ*num_;
					xc = (int) (mx*num_+0.5);
					yc = (int) (my*num_+0.5);
					int dx = xc-xcOld;
					int dy = yc-ycOld;
					float dY = Yc-YcOld;
					float dI = Ic-IcOld;
					float dQ = Qc-QcOld;

					shift = dx*dx+dy*dy+dY*dY+dI*dI+dQ*dQ; 
					repetition++;
				}
				while (shift > 3 && repetition < 100);
				tr = (int)(Yc + 0.9563f*Ic + 0.6210f*Qc);
				tg = (int)(Yc - 0.2721f*Ic - 0.6473f*Qc);
				tb = (int)(Yc - 1.1070f*Ic + 1.7046f*Qc);     
				outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
        	}
        }
        setRGB( dest, 0, 0, width, height, outPixels );
        return dest;
	}
	
	public String toString() {
		System.out.println("Mean Shift Filter...");
		return "MeanShiftFilter";
	}

}
```
**转载请注明**




