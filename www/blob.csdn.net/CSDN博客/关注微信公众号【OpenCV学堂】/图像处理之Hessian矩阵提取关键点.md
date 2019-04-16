# 图像处理之Hessian矩阵提取关键点 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年11月22日 00:06:13[gloomyfish](https://me.csdn.net/jia20003)阅读数：26557








一：大致的算法流程

1. 对每个像素点计算图像在X方向Y方向的二阶偏导数，计算图像的XY方向的导数

2. 根据第一步的计算结果，有Hessian Matrix计算**D(h) = Ixx*Iyy - Ixy*Ixy**

**![](https://img-blog.csdn.net/20131122000136203)**

其中Ixx表示X方向的二阶偏导数

Iyy表示Y方向的二阶偏导数

Ixy表XY方向的二阶导数

3. 根据第二步计算出来的值使用3×3窗口实现非最大信号压制，

我的做法， 直接给了threshold值，这个其实不很对，真的懒，不想弄啦！

二：导数计算实现

关于一阶与二阶高斯偏导数计算请看这里：

[http://blog.csdn.net/jia20003/article/details/16369143](http://blog.csdn.net/jia20003/article/details/16369143)


三：程序效果

![](https://img-blog.csdn.net/20131122000305609)


四：算法代码



```java
package com.gloomyfish.image.harris.corner;

import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;

import com.gloomyfish.filter.study.AbstractBufferedImageOp;

public class HessianFeatureDetector extends AbstractBufferedImageOp {
	
	private GaussianDerivativeFilter gdFilter;
	private double minRejectThreshold = 4.1; // (r+1)^2/r
	private List<HessianMatrix> pixelMatrixList;
	
	public HessianFeatureDetector()
	{
		gdFilter = new GaussianDerivativeFilter();
		pixelMatrixList = new ArrayList<HessianMatrix>();
	}
	
	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();
        initSettings(height, width);
        if ( dest == null )
            dest = createCompatibleDestImage( src, null );

        int[] inPixels = new int[width*height];
        gdFilter.setDirectionType(GaussianDerivativeFilter.XX_DIRECTION);
        BufferedImage bixx = gdFilter.filter(src, null);
        getRGB( bixx, 0, 0, width, height, inPixels );
        extractPixelData(inPixels, GaussianDerivativeFilter.XX_DIRECTION, height, width);
        
        // YY Direction
        gdFilter.setDirectionType(GaussianDerivativeFilter.YY_DIRECTION);
        BufferedImage biyy = gdFilter.filter(src, null);
        getRGB( biyy, 0, 0, width, height, inPixels );
        extractPixelData(inPixels, GaussianDerivativeFilter.YY_DIRECTION, height, width);
        
        // XY Direction
        gdFilter.setDirectionType(GaussianDerivativeFilter.XY_DIRECTION);
        BufferedImage bixy = gdFilter.filter(src, null);
        getRGB( bixy, 0, 0, width, height, inPixels );
        extractPixelData(inPixels, GaussianDerivativeFilter.XY_DIRECTION, height, width);
        
        int[] outPixels = new int[width*height];
        int index = 0;
        for(int row=0; row<height; row++) {
        	int ta = 0, tr = 0, tg = 0, tb = 0;
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		ta = 255;
        		HessianMatrix hm = pixelMatrixList.get(index);
        		double[] t = hm.getThreshold();
        		if(t[0] > minRejectThreshold)
        		{
        			tr = 127;
        		}
        		else
        		{
        			tr = 0;
        		}
        		if(t[1] > minRejectThreshold)
        		{
        			tg = 127;
        		}
        		else
        		{
        			tg = 0;
        		}
        		if(t[2] > minRejectThreshold)
        		{
        			tb = 127;
        		}
        		else
        		{
        			tb = 0;
        		}
                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
        	}
        }

        setRGB( dest, 0, 0, width, height, outPixels );
        return dest;
	}
	
	private void initSettings(int height, int width)
	{
        int index = 0;
        for(int row=0; row<height; row++) {
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
                HessianMatrix matrix = new HessianMatrix();
                pixelMatrixList.add(index, matrix);
        	}
        }
	}
	
	private void extractPixelData(int[] pixels, int type, int height, int width)
	{
        int index = 0;
        for(int row=0; row<height; row++) {
        	int ta = 0, tr = 0, tg = 0, tb = 0;
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		ta = (pixels[index] >> 24) & 0xff;
                tr = (pixels[index] >> 16) & 0xff;
                tg = (pixels[index] >> 8) & 0xff;
                tb = pixels[index] & 0xff;
                HessianMatrix matrix = pixelMatrixList.get(index);
                if(type == GaussianDerivativeFilter.XX_DIRECTION)
                {
                	matrix.setXx(new double[]{tr, tg, tb});
                }
                if(type == GaussianDerivativeFilter.YY_DIRECTION)
                {
                	matrix.setYy(new double[]{tr, tg, tb});
                }
                if(type == GaussianDerivativeFilter.XY_DIRECTION)
                {
                	matrix.setXy(new double[]{tr, tg, tb});
                }
        	}
        }
	}

}
```
**转载请注明！**




