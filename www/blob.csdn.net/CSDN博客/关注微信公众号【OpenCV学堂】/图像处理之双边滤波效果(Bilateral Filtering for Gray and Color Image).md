# 图像处理之双边滤波效果(Bilateral Filtering for Gray and Color Image) - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年07月12日 15:52:08[gloomyfish](https://me.csdn.net/jia20003)阅读数：22132
所属专栏：[Java数字图像处理与特效](https://blog.csdn.net/column/details/image-process.html)









### 图像处理之双边滤波效果(Bilateral Filtering for Gray and Color Image)

**基本介绍：**

普通的时空域的低通滤波器，在像素空间完成滤波以后，导致图像的边缘部分也变得不那么明显，

整张图像都变得同样的模糊，图像边缘细节丢失。双边滤波器（A**Bilateral Filter**）可以很好的保

留边缘的同时消除噪声。双边滤波器能做到这些原因在于它不像普通的高斯/卷积低通滤波，只考

虑了位置对中心像素的影响，它还考虑了卷积核中像素与中心像素之间相似程度的影响，根据位置

影响与像素值之间的相似程度生成两个不同的权重表(WeightTable)，在计算中心像素的时候加以

考虑这两个权重，从而实现双边低通滤波。据说AdobePhotoshop的高斯磨皮功能就是应用了

双边低通滤波算法实现。

![](https://img-my.csdn.net/uploads/201207/12/1342078704_9839.png)


![](https://img-my.csdn.net/uploads/201207/12/1342078723_2014.jpg)


![](https://img-my.csdn.net/uploads/201207/12/1342078813_2502.png)


**程序效果：**

![](https://img-my.csdn.net/uploads/201207/12/1342078852_8793.png)


**看我们的美女lena应用双边滤镜之后**

![](https://img-my.csdn.net/uploads/201207/12/1342078869_9126.png)


![](https://img-my.csdn.net/uploads/201207/12/1342081250_2497.png)


**程序关键代码解释：**

**建立距离高斯权重表(Weight Table)如下：**



```java
private void buildDistanceWeightTable() {
	int size = 2 * radius + 1;
	cWeightTable = new double[size][size];
	for(int semirow = -radius; semirow <= radius; semirow++) {
		for(int semicol = - radius; semicol <= radius; semicol++) {
			// calculate Euclidean distance between center point and close pixels
			double delta = Math.sqrt(semirow * semirow + semicol * semicol)/ds;
			double deltaDelta = delta * delta;
			cWeightTable[semirow+radius][semicol+radius] = Math.exp(deltaDelta * factor);
		}
	}
}
```
**建立RGB值像素度高斯权重代码如下：**




```java
private void buildSimilarityWeightTable() {
	sWeightTable = new double[256]; // since the color scope is 0 ~ 255
	for(int i=0; i<256; i++) {
		double delta = Math.sqrt(i * i ) / rs;
		double deltaDelta = delta * delta;
		sWeightTable[i] = Math.exp(deltaDelta * factor);
	}
}
```
**完成权重和计算与像素×权重和计算代码如下：**




```java
for(int semirow = -radius; semirow <= radius; semirow++) {
	for(int semicol = - radius; semicol <= radius; semicol++) {
		if((row + semirow) >= 0 && (row + semirow) < height) {
			rowOffset = row + semirow;
		} else {
			rowOffset = 0;
		}
		
		if((semicol + col) >= 0 && (semicol + col) < width) {
			colOffset = col + semicol;
		} else {
			colOffset = 0;
		}
		index2 = rowOffset * width + colOffset;
		ta2 = (inPixels[index2] >> 24) & 0xff;
        tr2 = (inPixels[index2] >> 16) & 0xff;
        tg2 = (inPixels[index2] >> 8) & 0xff;
        tb2 = inPixels[index2] & 0xff;
        
        csRedWeight = cWeightTable[semirow+radius][semicol+radius]  * sWeightTable[(Math.abs(tr2 - tr))];
        csGreenWeight = cWeightTable[semirow+radius][semicol+radius]  * sWeightTable[(Math.abs(tg2 - tg))];
        csBlueWeight = cWeightTable[semirow+radius][semicol+radius]  * sWeightTable[(Math.abs(tb2 - tb))];
        
        csSumRedWeight += csRedWeight;
        csSumGreenWeight += csGreenWeight;
        csSumBlueWeight += csBlueWeight;
        redSum += (csRedWeight * (double)tr2);
        greenSum += (csGreenWeight * (double)tg2);
        blueSum += (csBlueWeight * (double)tb2);
	}
}
```
**完成归一化，得到输出像素点RGB值得代码如下：**




```java
tr = (int)Math.floor(redSum / csSumRedWeight);
tg = (int)Math.floor(greenSum / csSumGreenWeight);
tb = (int)Math.floor(blueSum / csSumBlueWeight);
outPixels[index] = (ta << 24) | (clamp(tr) << 16) | (clamp(tg) << 8) | clamp(tb);
```
**关于什么卷积滤波，请参考：**


[http://blog.csdn.net/jia20003/article/details/7038938](http://blog.csdn.net/jia20003/article/details/7038938)


**关于高斯模糊算法，请参考：**
[http://blog.csdn.net/jia20003/article/details/7234741](http://blog.csdn.net/jia20003/article/details/7234741)
**最后想说，不给出源代码的博文不是好博文，基于Java完成的双边滤波速度有点慢**

**可以自己优化，双边滤镜完全源代码如下：**



```java
package com.gloomyfish.blurring.study;
/**
 *  A simple and important case of bilateral filtering is shift-invariant Gaussian filtering
 *  refer to - http://graphics.ucsd.edu/~iman/Denoising/
 *  refer to - http://homepages.inf.ed.ac.uk/rbf/CVonline/LOCAL_COPIES/MANDUCHI1/Bilateral_Filtering.html
 *  thanks to cyber
 */
import java.awt.image.BufferedImage;

public class BilateralFilter extends AbstractBufferedImageOp {
	private final static double factor = -0.5d;
	private double ds; // distance sigma
	private double rs; // range sigma
	private int radius; // half length of Gaussian kernel Adobe Photoshop 
	private double[][] cWeightTable;
	private double[] sWeightTable;
	private int width;
	private int height;
	
	public BilateralFilter() {
		this.ds = 1.0f;
		this.rs = 1.0f;
	}
	
	private void buildDistanceWeightTable() {
		int size = 2 * radius + 1;
		cWeightTable = new double[size][size];
		for(int semirow = -radius; semirow <= radius; semirow++) {
			for(int semicol = - radius; semicol <= radius; semicol++) {
				// calculate Euclidean distance between center point and close pixels
				double delta = Math.sqrt(semirow * semirow + semicol * semicol)/ds;
				double deltaDelta = delta * delta;
				cWeightTable[semirow+radius][semicol+radius] = Math.exp(deltaDelta * factor);
			}
		}
	}
	
	/**
	 * for gray image
	 * @param row
	 * @param col
	 * @param inPixels
	 */
	private void buildSimilarityWeightTable() {
		sWeightTable = new double[256]; // since the color scope is 0 ~ 255
		for(int i=0; i<256; i++) {
			double delta = Math.sqrt(i * i ) / rs;
			double deltaDelta = delta * delta;
			sWeightTable[i] = Math.exp(deltaDelta * factor);
		}
	}
	
	public void setDistanceSigma(double ds) {
		this.ds = ds;
	}
	
	public void setRangeSigma(double rs) {
		this.rs = rs;
	}

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		width = src.getWidth();
        height = src.getHeight();
        //int sigmaMax = (int)Math.max(ds, rs);
        //radius = (int)Math.ceil(2 * sigmaMax);
        radius = (int)Math.max(ds, rs);
        buildDistanceWeightTable();
        buildSimilarityWeightTable();
        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );

        int[] inPixels = new int[width*height];
        int[] outPixels = new int[width*height];
        getRGB( src, 0, 0, width, height, inPixels );
        int index = 0;
		double redSum = 0, greenSum = 0, blueSum = 0;
		double csRedWeight = 0, csGreenWeight = 0, csBlueWeight = 0;
		double csSumRedWeight = 0, csSumGreenWeight = 0, csSumBlueWeight = 0;
        for(int row=0; row<height; row++) {
        	int ta = 0, tr = 0, tg = 0, tb = 0;
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		ta = (inPixels[index] >> 24) & 0xff;
                tr = (inPixels[index] >> 16) & 0xff;
                tg = (inPixels[index] >> 8) & 0xff;
                tb = inPixels[index] & 0xff;
                int rowOffset = 0, colOffset = 0;
                int index2 = 0;
                int ta2 = 0, tr2 = 0, tg2 = 0, tb2 = 0;
        		for(int semirow = -radius; semirow <= radius; semirow++) {
        			for(int semicol = - radius; semicol <= radius; semicol++) {
        				if((row + semirow) >= 0 && (row + semirow) < height) {
        					rowOffset = row + semirow;
        				} else {
        					rowOffset = 0;
        				}
        				
        				if((semicol + col) >= 0 && (semicol + col) < width) {
        					colOffset = col + semicol;
        				} else {
        					colOffset = 0;
        				}
        				index2 = rowOffset * width + colOffset;
        				ta2 = (inPixels[index2] >> 24) & 0xff;
        		        tr2 = (inPixels[index2] >> 16) & 0xff;
        		        tg2 = (inPixels[index2] >> 8) & 0xff;
        		        tb2 = inPixels[index2] & 0xff;
        		        
        		        csRedWeight = cWeightTable[semirow+radius][semicol+radius]  * sWeightTable[(Math.abs(tr2 - tr))];
        		        csGreenWeight = cWeightTable[semirow+radius][semicol+radius]  * sWeightTable[(Math.abs(tg2 - tg))];
        		        csBlueWeight = cWeightTable[semirow+radius][semicol+radius]  * sWeightTable[(Math.abs(tb2 - tb))];
        		        
        		        csSumRedWeight += csRedWeight;
        		        csSumGreenWeight += csGreenWeight;
        		        csSumBlueWeight += csBlueWeight;
        		        redSum += (csRedWeight * (double)tr2);
        		        greenSum += (csGreenWeight * (double)tg2);
        		        blueSum += (csBlueWeight * (double)tb2);
        			}
        		}
        		
				tr = (int)Math.floor(redSum / csSumRedWeight);
				tg = (int)Math.floor(greenSum / csSumGreenWeight);
				tb = (int)Math.floor(blueSum / csSumBlueWeight);
				outPixels[index] = (ta << 24) | (clamp(tr) << 16) | (clamp(tg) << 8) | clamp(tb);
                
                // clean value for next time...
                redSum = greenSum = blueSum = 0;
                csRedWeight = csGreenWeight = csBlueWeight = 0;
                csSumRedWeight = csSumGreenWeight = csSumBlueWeight = 0;
                
        	}
        }
        setRGB( dest, 0, 0, width, height, outPixels );
        return dest;
	}
	
	public static int clamp(int p) {
		return p < 0 ? 0 : ((p > 255) ? 255 : p);
	}

	public static void main(String[] args) {
		BilateralFilter bf = new BilateralFilter();
		bf.buildSimilarityWeightTable();
	}
}
```
****


**感觉不错，请顶一下！**
**转载文章请务必注明出自本博客**











