# 五种基于RGB色彩空间统计的皮肤检测算法 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年06月26日 22:22:27[gloomyfish](https://me.csdn.net/jia20003)阅读数：6887
所属专栏：[Java数字图像处理与特效](https://blog.csdn.net/column/details/image-process.html)









最近一直在研究多脸谱识别以及如何分辨多个皮肤区域是否是人脸的问题


网上找了很多资料，看了很多篇文章，将其中基于RGB色彩空间识别皮肤

的统计算法做了一下总结，统计识别方法主要是简单相比与很多其它基于

机器学习的算法，本人总结了五种RGB色彩空间的统计算法源码如下：

**Skin Filter1：**



```java
public class SkinFilter1 extends AbstractBufferedImageOp {

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();

        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );

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
                
                // detect skin method...
                double sum = tr + tg + tb;
                if (((double)tr/(double)tb > 1.185) && 
                	((double)(tr*tb)/(double)(sum*sum)>0.107) &&
                	((double)(tr*tg)/(double)(sum*sum)>0.112))
                {
                	tr = tg = tb = 0; // black - skin detected!!
                } else {
                	tr = tg = tb = 255; // white color means non-skin pixel
                }
                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
        	}
        }
        setRGB( dest, 0, 0, width, height, outPixels );
        return dest;
	}
}
```
**Skin Filter2：**


```java
public class SkinFilter2 extends AbstractBufferedImageOp {

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();

        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );

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
                double sum = tr + tg + tb;
                
                
                if(((double)3*tb*tr*tr/(double)(sum*sum*sum)>0.1276)&&
                	((double)(tr*tb+tg*tg)/(double)(tg*tb)>2.14)&&
                	((double)(sum)/(double)(3*tr)+(double)(tr-tg)/(double)(sum)<2.7775))
                {
                	tr = tg = tb = 0;
                } else {
                	tr = tg = tb = 255;
                }
                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
        	}
        }
        setRGB( dest, 0, 0, width, height, outPixels );
        return dest;
	}
}
```
**Skin Filter3：**


```java
public class SkinFilter3 extends AbstractBufferedImageOp {

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();

        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );

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
                
                // detect skin method...
                double sum = tr + tg + tb;
                if (((double)tg / (double)tg - (double)tr / (double)tb <= -0.0905) &&
                	((double)(sum) / (double)(3 * tr) + (double)(tr - tg) / (double)(sum) <= 0.9498))
                {
                	tr = tg = tb = 0;
                } else {
                	tr = tg = tb = 255;
                }
                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
        	}
        }
        setRGB( dest, 0, 0, width, height, outPixels );
        return dest;
	}
}
```
**Skin Filter4：**


```java
import java.awt.image.BufferedImage;
/**
 * this skin detection is absolutely good skin classification,
 * i love this one very much
 * 
 * this one should be always primary skin detection 
 * from all five filters
 * 
 * @author zhigang
 *
 */
public class SkinFilter4 extends AbstractBufferedImageOp {

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();

        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );

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
                
                // detect skin method...
                double sum = tr + tg + tb;
                if (((double)tb/(double)tg<1.249) &&
	                ((double)sum/(double)(3*tr)>0.696) &&
	                (0.3333-(double)tb/(double)sum>0.014) &&
	                ((double)tg/(double)(3*sum)<0.108))
                {
                	tr = tg = tb = 0;
                } else {
                	tr = tg = tb = 255;
                }
                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
        	}
        }
        setRGB(dest, 0, 0, width, height, outPixels);
        return dest;
	}
}
```
**Skin Filter5：**


```java
import java.awt.image.BufferedImage;
/**
 * this is very good skin detection
 * get real skin segmentation correctly....
 * ohh... cool
 * 
 * @author zhigang
 *
 */
public class SkinFilter5 extends AbstractBufferedImageOp {

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();

        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );

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
                
                // detect skin method...
                double sum = tr + tg + tb;
                if (((double)tg/(double)tb - (double)tr/(double)tg<=-0.0905)&&
                ((double)(tg*sum)/(double)(tb*(tr-tg))>3.4857)&&
                ((double)(sum*sum*sum)/(double)(3*tg*tr*tr)<=7.397)&&
                ((double)sum/(double)(9*tr)-0.333 > -0.0976))
                {
                	tr = tg = tb = 0;
                } else {
                	tr = tg = tb = 255;
                }
                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
        	}
        }
        setRGB( dest, 0, 0, width, height, outPixels );
        return dest;
	}
}
```
**总结一下：**
似乎Filter3的效果与Filter1的效果不是很好，Filter5, Filter4的效果感觉

还是很好的，基本上可以符合实际要求。










