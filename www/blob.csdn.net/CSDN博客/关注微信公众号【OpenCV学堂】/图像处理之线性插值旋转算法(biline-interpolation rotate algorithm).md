# 图像处理之线性插值旋转算法(biline-interpolation rotate algorithm) - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年11月07日 21:13:15[gloomyfish](https://me.csdn.net/jia20003)阅读数：9667
所属专栏：[Java数字图像处理与特效](https://blog.csdn.net/column/details/image-process.html)









图像处理之线性插值旋转算法

**基本数学知识：**

1.      三角函数基本知识，sin, cosin

2.      反三角函数基本知识，知道任意一点坐标P(x, y)求取该点的角度a = atag2(y/x)

3.      极坐标与笛卡尔坐标系转换知识

图像旋转矩阵：由此可以计算图像旋转以后的新的高度与宽度。

![](https://img-my.csdn.net/uploads/201211/07/1352293787_9806.png)


**相关算法：**

双线性插值算法，实现图像旋转反锯齿效果，同时是一种高质量的图像图像旋转方法，

缺点是计算量比较大。但是对现在的计算机硬件来说，速度还可以。

**关于角度旋转：**

1.      90度，180度，270度可以直接旋转坐标取得，像素直接映射取得。

2.      对于任何角度angle可以如下处理n = mod(angle, 90) = 1, 2, 3, 然后

        将角度旋转90，180，270然后再旋转角度(angle– n * 90)。

**程序实现：**

1.      首先根据输入角度参数angle, 背景填充颜色bgcolor初始化

2.      计算出旋转以后的图像width与height

3.      循环每个输出像素，计算机坐标

4.      反旋转输入角度到输入的目标像素浮点数坐标

5.      使用双线性插值完成目标像素填充，如果不在范围之内填充背景色。

6.      得到输出像素数据，返回旋转后图像

原图：

![](https://img-my.csdn.net/uploads/201211/07/1352293914_3836.jpg)


旋转45度的效果，背景填充为灰色：

![](https://img-my.csdn.net/uploads/201211/07/1352293955_5742.png)


**程序代码（特殊角度旋转自己实现吧，有点懒）**



```java
package com.gloomyfish.filter.study;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;

public class RotateFilter extends AbstractBufferedImageOp {
	private double angle;
	private Color background;
	private int outw;
	private int outh;
	
	public RotateFilter() {
		this.angle = (45.0d/180.0d) * Math.PI;
		background = Color.BLACK;
		outw = -1;
		outh = -1;
	}
	
	public void setDegree(double angle) {
		this.angle = (angle/180.0d) * Math.PI;
	}
	
	public void setBackgroud(Color background) {
		this.background = background;
	}
	
    public BufferedImage createCompatibleDestImage(BufferedImage src, ColorModel dstCM) {
        if ( dstCM == null )
            dstCM = src.getColorModel();
        return new BufferedImage(dstCM, dstCM.createCompatibleWritableRaster(outw, outh), dstCM.isAlphaPremultiplied(), null);
    }

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();

        int[] inPixels = new int[width*height];
        outw = (int)(width*Math.cos(angle)+height*Math.sin(angle)); 
        outh = (int)(height*Math.cos(angle)+width*Math.sin(angle));
        System.out.println("after rotate, new width : " + outw);
        System.out.println("after rotate, new height: " + outh);
        
        int[] outPixels = new int[outw*outh];
        getRGB( src, 0, 0, width, height, inPixels );
        int index = 0;
        
        int centerPixel = inPixels[height/2 * width + width/2];
        
        // calculate new center coordinate
        float centerX = outw / 2.0f + 0.5f;
        float centerY = outh /2.0f + 0.5f;
        
        // calculate the original center coordinate
        float ocenterX = width / 2.0f + 0.5f;
        float ocenterY = height /2.0f + 0.5f;
        
        float rx =0, ry = 0; //after rotated coordinate
        float px = 0, py = 0; // original coordinate
        float prow = 0, pcol = 0;
        for(int row=0; row<outh; row++) {
        	for(int col=0; col<outw; col++) {
        		rx = col - centerX;
        		ry = centerY - row;
        		float fDistance = (float)Math.sqrt(rx * rx + ry * ry);
        		float fPolarAngle = 0; //;
        		if(rx != 0) {
        			fPolarAngle = (float)Math.atan2((double)ry, (double)rx);
        		} else {
        			if(rx == 0) {
        				if(ry == 0) {
        					outPixels[index] = centerPixel;
        					continue; 
        				} 
        				else if(ry < 0) {
            				fPolarAngle = 1.5f * (float)Math.PI;
            			} else {
            				fPolarAngle = 0.5f * (float)Math.PI;
            			}
        			}
        		}
        		
        		// "reverse" rotate, so minus instead of plus
                fPolarAngle -= angle;
                px = fDistance * (float)Math.cos(fPolarAngle);
                py = fDistance * (float)Math.sin(fPolarAngle);

                // get original pixel float point
                prow = ((float)ocenterY) - py;
                pcol = ((float)ocenterX) + px;

                // now start the biline-interpolation algorithm here!!!
                int[] rgb = bilineInterpolation(inPixels, width, height, prow, pcol);
                
                index = row * outw + col;
                outPixels[index] = (255 << 24) | (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
                
        	}
        }

        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );
        setRGB( dest, 0, 0, outw, outh, outPixels );
        return dest;
	}

	private int[] bilineInterpolation(int[] input, int width, int height, float prow, float pcol) {
		double row = Math.floor(prow);
		double col = Math.floor(pcol);
		if(row < 0 || row >= height) {
			return new int[]{background.getRed(), background.getGreen(), background.getBlue()};
		}
		if(col < 0 || col >= width) {
			return new int[]{background.getRed(), background.getGreen(), background.getBlue()};
		}
		
		int rowNext = (int)row + 1, colNext = (int)col + 1;
		if((row + 1) >= height) {
			rowNext = (int)row;
		}
		
		if((col + 1) >= width) {
			colNext = (int)col;
		}
		double t = prow - row;
		double u = pcol - col;
		double coffiecent1 = (1.0d-t)*(1.0d-u);
		double coffiecent2 = (t)*(1.0d-u);
		double coffiecent3 = t*u;
		double coffiecent4 = (1.0d-t)*u;
		
		int index1 = (int)(row * width + col);
		int index2 = (int)(row * width + colNext);
		
		int index3 = (int)(rowNext * width + col);
		int index4 = (int)(rowNext * width + colNext);
		int tr1, tr2, tr3, tr4;
		int tg1, tg2, tg3, tg4;
		int tb1, tb2, tb3, tb4;
		
        tr1 = (input[index1] >> 16) & 0xff;
        tg1 = (input[index1] >> 8) & 0xff;
        tb1 = input[index1] & 0xff;
        
        tr2 = (input[index2] >> 16) & 0xff;
        tg2 = (input[index2] >> 8) & 0xff;
        tb2 = input[index2] & 0xff;
        
        tr3 = (input[index3] >> 16) & 0xff;
        tg3 = (input[index3] >> 8) & 0xff;
        tb3 = input[index3] & 0xff;
        
        tr4 = (input[index4] >> 16) & 0xff;
        tg4 = (input[index4] >> 8) & 0xff;
        tb4 = input[index4] & 0xff;

        int tr = (int)(tr1 * coffiecent1 + tr2 * coffiecent4 + tr3 * coffiecent2 + tr4 * coffiecent3);
        int tg = (int)(tg1 * coffiecent1 + tg2 * coffiecent4 + tg3 * coffiecent2 + tg4 * coffiecent3);
        int tb = (int)(tb1 * coffiecent1 + tb2 * coffiecent4 + tb3 * coffiecent2 + tb4 * coffiecent3);

		return new int[]{tr, tg, tb};
	}

}
```
**转载请务必注明**


