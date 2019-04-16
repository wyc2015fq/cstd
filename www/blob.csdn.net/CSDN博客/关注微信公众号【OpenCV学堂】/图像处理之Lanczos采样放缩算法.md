# 图像处理之Lanczos采样放缩算法 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2014年01月05日 00:32:42[gloomyfish](https://me.csdn.net/jia20003)阅读数：15853








**图像处理之Lanczos采样放缩算法**

一:什么是Lanczos采样

参见这里:http://en.wikipedia.org/wiki/Lanczos_resampling

二:大致算法流程

![](https://img-blog.csdn.net/20140105002704718)


三:算法运行结果

1.向下采样, 生成缩略图, 左边为原图,右边为缩略图

![](https://img-blog.csdn.net/20140105002821187)


向上采样,生成放大图像时效果:

![](https://img-blog.csdn.net/20140105002905046)


**算法源代码:**



```java
package com.gloomyfish.zoom.study;

import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;

import com.gloomyfish.filter.study.AbstractBufferedImageOp;

public class LanczosScaleFilter extends AbstractBufferedImageOp {
	// lanczos_size
	private float lanczosSize;
	private float destWidth;
	
	public LanczosScaleFilter()
	{
		lanczosSize = 3;
		destWidth = 100;
	}

	public LanczosScaleFilter(float lobes, int width) {
		this.lanczosSize = lobes;
		this.destWidth = width;
	}

	public void setLanczosSize(float size) {
		this.lanczosSize = size;
	}

	public void setDestWidth(float destWidth) {
		this.destWidth = destWidth;
	}

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
		int height = src.getHeight();
		float ratio = width / this.destWidth;
		float rcp_ratio = 2.0f / ratio;
		float range2 = (float) Math.ceil(ratio * lanczosSize / 2);
		
		// destination image
		int dh = (int)(height * (this.destWidth/width));
		int dw = (int)this.destWidth;

		if (dest == null) {
			ColorModel cMD = src.getColorModel();
			dest = new BufferedImage(src.getColorModel(), cMD.createCompatibleWritableRaster(dw, dh), cMD.isAlphaPremultiplied(), null);
		}

		int[] inPixels = new int[width * height];
		int[] outPixels = new int[dw * dh];
		
		getRGB(src, 0, 0, width, height, inPixels);
		int index = 0;
		float fcy = 0, icy = 0, fcx = 0, icx = 0;
		for (int row = 0; row < dh; row++) {
			int ta = 0, tr = 0, tg = 0, tb = 0;
			fcy = (row + 0.5f) * ratio;
			icy = (float) Math.floor(fcy);
			for (int col = 0; col < dw; col++) {
				fcx = (col + 0.5f) * ratio;
				icx = (float) Math.floor(fcx);

				float sumred = 0, sumgreen = 0, sumblue = 0;
				float totalWeight = 0;
				for (int subcol = (int) (icx - range2); subcol <= icx + range2; subcol++) {
					if (subcol < 0 || subcol >= width)
						continue;
					int ncol = (int) Math.floor(1000 * Math.abs(subcol - fcx));

					for (int subrow = (int) (icy - range2); subrow <= icy + range2; subrow++) {
						if (subrow < 0 || subrow >= height)
							continue;
						int nrow = (int) Math.floor(1000 * Math.abs(subrow - fcy));
						float weight = (float) getLanczosFactor(Math.sqrt(Math.pow(ncol * rcp_ratio, 2)
								+ Math.pow(nrow * rcp_ratio, 2)) / 1000);
						if (weight > 0) {
							index = (subrow * width + subcol);
							tr = (inPixels[index] >> 16) & 0xff;
							tg = (inPixels[index] >> 8) & 0xff;
							tb = inPixels[index] & 0xff;
							totalWeight += weight;
							sumred += weight * tr;
							sumgreen += weight * tg;
							sumblue += weight * tb;
						}
					}
				}
				index = row * dw + col;
				tr = (int) (sumred / totalWeight);
				tg = (int) (sumgreen / totalWeight);
				tb = (int) (sumblue / totalWeight);
				outPixels[index] = (255 << 24) | (clamp(tr) << 16) | (clamp(tg) << 8) | clamp(tb);
				
				// clear for next pixel
				sumred = 0;
				sumgreen = 0;
				sumblue = 0;
				totalWeight = 0;

			}
		}
		setRGB(dest, 0, 0, dw, dh, outPixels);
		return dest;
	}
	
	public static int clamp(int v)
	{
		return v > 255 ? 255 : (v < 0 ? 0 : v);
	}

	private double getLanczosFactor(double distance) {
		if (distance > lanczosSize)
			return 0;
		distance *= Math.PI;
		if (Math.abs(distance) < 1e-16)
			return 1;
		double xx = distance / lanczosSize;
		return Math.sin(distance) * Math.sin(xx) / distance / xx;
	}

}
```



**五:窗口大小对结果的影响**

如果是向下采样生成缩略图的话, 窗口大小设置为3就已经非常清楚了

如果向上采样要放大图像的话, 窗口大小设置要大于6才能获得较好结果,推荐使用窗口

大小为8. 



