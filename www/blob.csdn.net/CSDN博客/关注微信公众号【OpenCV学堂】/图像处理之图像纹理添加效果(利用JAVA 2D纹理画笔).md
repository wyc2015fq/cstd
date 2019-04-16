# 图像处理之图像纹理添加效果(利用JAVA 2D纹理画笔) - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年01月11日 00:27:03[gloomyfish](https://me.csdn.net/jia20003)阅读数：4599








一：基本思想

做法非常简单，首先找到一张你想要的纹理素材图片，然后将该图片通过灰度

转换成一张灰度级别的纹理图，取得像素数据，扫描每个像素的灰度值来设置

纹理图像的每个像素的透明度。灰度值越高越接近白色的透明度越高，灰度

值越小的越接近黑色的透明度越小。然后利用Java 2D的TexturePaint来创建

纹理画笔，最终一步就是在目标图像上填充一个与它大小一致的纹理矩形。

如此便完成。

二：程序运行效果

纹理图：

![](https://img-my.csdn.net/uploads/201301/11/1357834918_4855.png)


**目标图像：**

![](https://img-my.csdn.net/uploads/201212/25/1356365792_1126.png)http://

**效果图：**

![](https://img-my.csdn.net/uploads/201301/11/1357835468_2941.png)


源程序代码：



```java
package com.gloomyfish.filter.study;

import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.TexturePaint;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;

public class TextureAdditionFilter extends AbstractBufferedImageOp {

	private BufferedImage textureImage;
	private float textureTransparency;
	
	public TextureAdditionFilter()
	{
		textureTransparency = 0.9f;
		System.out.println("Java texture Image Addition Filter");
	}
	
	public void setTextureImage(BufferedImage textureImage) {
		this.textureImage = textureImage;
	}

	public void setTextureTransparency(float textureTransparency) {
		this.textureTransparency = textureTransparency;
	}

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		if(textureImage != null) {
			int width = textureImage.getWidth();
	        int height = textureImage.getHeight();
	        BufferedImage grayTextureImage = null;
	        grayTextureImage = new BufferedImage(width, height, BufferedImage.TYPE_4BYTE_ABGR);
	        int[] inPixels = new int[width*height];
	        int[] outPixels = new int[width*height];
	        getRGB( textureImage, 0, 0, width, height, inPixels );
	        int index = 0;
	        for(int row=0; row<height; row++) {
	        	int ta = 0, tr = 0, tg = 0, tb = 0;
	        	for(int col=0; col<width; col++) {
	        		index = row * width + col;
	        		ta = (inPixels[index] >> 24) & 0xff;
	                tr = (inPixels[index] >> 16) & 0xff;
	                tg = (inPixels[index] >> 8) & 0xff;
	                tb = inPixels[index] & 0xff;
	                tr = tg = tb = (int)(0.299 * (double)tr + 0.587 * (double)tg + 0.114 * (double)tb);
	                ta = (int)(textureTransparency * tr);
	                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
	                
	        	}
	        }
	        
	        // then call java 2D API to print texture image on target image
	        // you can see magic result image if you adjust the parameter-textureTransparency
	        // just play with it......
	        setRGB( grayTextureImage, 0, 0, width, height, outPixels );
	        Rectangle2D rectangle = new Rectangle(0, 0, width, height);
	        TexturePaint tp = new TexturePaint(grayTextureImage, rectangle);
	        Graphics2D g2d = src.createGraphics();
	        g2d.setPaint(tp);
	        width = src.getWidth();
	        height = src.getHeight();
	        g2d.fillRect(0, 0, width, height);
		}
		
		return src;
	}

**}**
```
**转载请务必注明**





