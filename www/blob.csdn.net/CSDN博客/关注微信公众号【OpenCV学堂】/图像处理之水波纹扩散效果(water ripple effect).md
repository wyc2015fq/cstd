# 图像处理之水波纹扩散效果(water ripple effect) - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年10月26日 22:38:47[gloomyfish](https://me.csdn.net/jia20003)阅读数：14711








**Water Ripple Effect - 水波纹效果**

一：原理

模拟水波纹效果，最常见的是sine或者cosn的函数，周期性变化，贴近自然

当水波纹中中间开始向四周扩散的时候，一般都是慢慢的失去能量，振幅也是

越来越小，所以程序要模拟这个过程时候，要加上一个能量递减因子。然后用

公式 y = a*sine(bx + c)来表示波纹公式。

二：程序实现

最重要的一步是计算水波纹的振幅。在任意一点确定水波的中心位置，可以是

鼠标随机选取，对半径范围内的像素位置实现水波生成，然后转换为位置，对

位置实现浮点数取整，然后使用适当的插值算法，本例使用双线性插值。

三：程序效果

![](https://img-blog.csdn.net/20131026223400250)


**四：滤镜完全源代码，这次我写了些中文注解，不给源代码的博文不是好博文**



```java
package com.gloomyfish.filter.study;

import java.awt.image.BufferedImage;

public class WaterFilter extends AbstractBufferedImageOp {
	private float wavelength = 16;
	private float amplitude = 10;
	private float phase = 0;
	private float centreX = 0.5f;
	private float centreY = 0.5f;
	private float radius = 50;

	private float radius2 = 0;
	private float icentreX;
	private float icentreY;
	
	public WaterFilter() {

	}

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();

        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );

        int[] inPixels = new int[width*height];
        int[] outPixels = new int[width*height];
        getRGB( src, 0, 0, width, height, inPixels );
		icentreX = width * centreX;
		icentreY = height * centreY;
		if ( radius == 0 )
			radius = Math.min(icentreX, icentreY);
		radius2 = radius*radius;
        int index = 0;
        float[] out = new float[2];
        for(int row=0; row<height; row++) {
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		
        		// 获取水波的扩散位置，最重要的一步
        		generateWaterRipples(col, row, out);
				int srcX = (int)Math.floor( out[0] );
				int srcY = (int)Math.floor( out[1] );
				float xWeight = out[0]-srcX;
				float yWeight = out[1]-srcY;
				int nw, ne, sw, se;
				
				// 获取周围四个像素，插值用，
				if ( srcX >= 0 && srcX < width-1 && srcY >= 0 && srcY < height-1) {
					// Easy case, all corners are in the image
					int i = width*srcY + srcX;
					nw = inPixels[i];
					ne = inPixels[i+1];
					sw = inPixels[i+width];
					se = inPixels[i+width+1];
				} else {
					// Some of the corners are off the image
					nw = getPixel( inPixels, srcX, srcY, width, height );
					ne = getPixel( inPixels, srcX+1, srcY, width, height );
					sw = getPixel( inPixels, srcX, srcY+1, width, height );
					se = getPixel( inPixels, srcX+1, srcY+1, width, height );
				}
				
				// 取得对应的振幅位置P(x, y)的像素，使用双线性插值
				/*if(xWeight >=0 || yWeight >= 0)
				{
					outPixels[index] = ImageMath.bilinearInterpolate(xWeight, yWeight, nw, ne, sw, se);					
				}
				else 
				{
					outPixels[index] = inPixels[index];
				}*/
				outPixels[index] = ImageMath.bilinearInterpolate(xWeight, yWeight, nw, ne, sw, se);
        	}
        }

        setRGB( dest, 0, 0, width, height, outPixels );
        return dest;
	}

	private int getPixel(int[] pixels, int x, int y, int width, int height) {
		if (x < 0 || x >= width || y < 0 || y >= height) {
			return 0; // 有点暴力啦，懒得管啦
		}
		return pixels[ y*width+x ];
	}

	protected void generateWaterRipples(int x, int y, float[] out) {
		float dx = x-icentreX;
		float dy = y-icentreY;
		float distance2 = dx*dx + dy*dy;
		// 确定 water ripple的半径，如果在半径之外，就直接获取原来位置，不用计算迁移量
		if (distance2 > radius2) { 
			out[0] = x;
			out[1] = y;
		} else {
			// 如果在radius半径之内，计算出来
			float distance = (float)Math.sqrt(distance2);
			// 计算改点振幅
			float amount = amplitude * (float)Math.sin(distance / wavelength * ImageMath.TWO_PI - phase);
			// 计算能量损失， 
			amount *= (radius-distance)/radius; // 计算能量损失，
			if ( distance != 0 )
				amount *= wavelength/distance;
			// 得到water ripple 最终迁移位置
			out[0] = x + dx*amount;
			out[1] = y + dy*amount;
		}
	}
	
}
```
****


**觉得不错请顶一下，谢谢！**

转载请务必注明出处！谢谢！



