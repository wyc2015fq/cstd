# 图像处理之角点检测算法(Harris Corner Detection) - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年11月23日 22:55:15[gloomyfish](https://me.csdn.net/jia20003)阅读数：23256
所属专栏：[Java数字图像处理与特效](https://blog.csdn.net/column/details/image-process.html)









图像处理之角点检测算法(Harris Corner Detection)

Harris角点检测是通过数学计算在图像上发现角点特征的一种算法，而且其具有旋转不

变性的特质。OpenCV中的Shi-Tomasi角点检测就是基于Harris角点检测改进算法。

**基本原理：**

角点是一幅图像上最明显与重要的特征，对于一阶导数而言，角点在各个方向的变化是

最大的，而边缘区域在只是某一方向有明显变化。一个直观的图示如下：

**![](https://img-blog.csdn.net/20131123223210125)**

**数学原理：**



基本数学公式如下：

![](https://img-blog.csdn.net/20131123223426734)


其中W(x, y)表示移动窗口，I(x, y)表示像素灰度值强度，范围为0～255。根据泰勒级数

计算一阶到N阶的偏导数，最终得到一个Harris矩阵公式：

![](https://img-blog.csdn.net/20131123223447015)


根据Harris的矩阵计算矩阵特征值![](https://img-blog.csdn.net/20131123223632953),然后计算Harris角点响应值：

![](https://img-blog.csdn.net/20131123223742640)


其中K为系数值，通常取值范围为0.04 ~ 0.06之间。

**算法详细步骤**

第一步：计算图像X方向与Y方向的一阶高斯偏导数Ix与Iy

第二步：根据第一步结果得到Ix^2 , Iy^2与Ix*Iy值

第三步：高斯模糊第二步三个值得到Sxx, Syy, Sxy

第四部：定义每个像素的Harris矩阵，计算出矩阵的两个特质值

第五步：计算出每个像素的R值

第六步：使用3X3或者5X5的窗口，实现非最大值压制

第七步：根据角点检测结果计算，最提取到的关键点以绿色标记，显示在原图上。

****程序关键代码解读**：**

第一步计算一阶高斯偏导数的Ix与Iy值代码如下：

****

```java
filter.setDirectionType(GaussianDerivativeFilter.X_DIRECTION);
		BufferedImage xImage = filter.filter(grayImage, null);
		getRGB( xImage, 0, 0, width, height, inPixels );
		extractPixelData(inPixels, GaussianDerivativeFilter.X_DIRECTION, height, width);
		
		filter.setDirectionType(GaussianDerivativeFilter.Y_DIRECTION);
		BufferedImage yImage = filter.filter(grayImage, null);
		getRGB( yImage, 0, 0, width, height, inPixels );
		extractPixelData(inPixels, GaussianDerivativeFilter.Y_DIRECTION, height, width);
```



关于如何计算高斯一阶与二阶偏导数请看这里：

**[http://blog.csdn.net/jia20003/article/details/16369143](http://blog.csdn.net/jia20003/article/details/16369143)**

[http://blog.csdn.net/jia20003/article/details/7664777](http://blog.csdn.net/jia20003/article/details/7664777)


第三步：分别对第二步计算出来的三个值，单独进行高斯

模糊计算，代码如下：

****

```java
private void calculateGaussianBlur(int width, int height) {
        int index = 0;
        int radius = (int)window_radius;
        double[][] gw = get2DKernalData(radius, sigma);
        double sumxx = 0, sumyy = 0, sumxy = 0;
        for(int row=0; row<height; row++) {
        	for(int col=0; col<width; col++) {        		
        		for(int subrow =-radius; subrow<=radius; subrow++)
        		{
        			for(int subcol=-radius; subcol<=radius; subcol++)
        			{
        				int nrow = row + subrow;
        				int ncol = col + subcol;
        				if(nrow >= height || nrow < 0)
        				{
        					nrow = 0;
        				}
        				if(ncol >= width || ncol < 0)
        				{
        					ncol = 0;
        				}
        				int index2 = nrow * width + ncol;
        				HarrisMatrix whm = harrisMatrixList.get(index2);
        				sumxx += (gw[subrow + radius][subcol + radius] * whm.getXGradient());
        				sumyy += (gw[subrow + radius][subcol + radius] * whm.getYGradient());
        				sumxy += (gw[subrow + radius][subcol + radius] * whm.getIxIy());
        			}
        		}
        		index = row * width + col;
        		HarrisMatrix hm = harrisMatrixList.get(index);
        		hm.setXGradient(sumxx);
        		hm.setYGradient(sumyy);
        		hm.setIxIy(sumxy);
        		
        		// clean up for next loop
        		sumxx = 0;
        		sumyy = 0;
        		sumxy = 0;
        	}
        }		
	}
```



第六步：非最大信号压制(non-max value suppression)

这个在边源检测中是为了得到一个像素宽的边缘，在这里则

是为了得到准确的一个角点像素，去掉非角点值。代码如下：

****

```java
/***
	 * we still use the 3*3 windows to complete the non-max response value suppression
	 */
	private void nonMaxValueSuppression(int width, int height) {
        int index = 0;
        int radius = (int)window_radius;
        for(int row=0; row<height; row++) {
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		HarrisMatrix hm = harrisMatrixList.get(index);
        		double maxR = hm.getR();
        		boolean isMaxR = true;
        		for(int subrow =-radius; subrow<=radius; subrow++)
        		{
        			for(int subcol=-radius; subcol<=radius; subcol++)
        			{
        				int nrow = row + subrow;
        				int ncol = col + subcol;
        				if(nrow >= height || nrow < 0)
        				{
        					nrow = 0;
        				}
        				if(ncol >= width || ncol < 0)
        				{
        					ncol = 0;
        				}
        				int index2 = nrow * width + ncol;
        				HarrisMatrix hmr = harrisMatrixList.get(index2);
        				if(hmr.getR() > maxR)
        				{
        					isMaxR = false;
        				}
        			}       			
        		}
        		if(isMaxR)
        		{
        			hm.setMax(maxR);
        		}
        	}
        }
		
	}
```



运行效果：

![](https://img-blog.csdn.net/20131123224922718)


**程序完整源代码：**



```java
package com.gloomyfish.image.harris.corner;

import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;

import com.gloomyfish.filter.study.GrayFilter;

public class HarrisCornerDetector extends GrayFilter {
	private GaussianDerivativeFilter filter;
	private List<HarrisMatrix> harrisMatrixList;
	private double lambda = 0.04; // scope : 0.04 ~ 0.06
	
	// i hard code the window size just keep it' size is same as 
	// first order derivation Gaussian window size
	private double sigma = 1; // always
	private double window_radius = 1; // always
	public HarrisCornerDetector() {
		filter = new GaussianDerivativeFilter();
		harrisMatrixList = new ArrayList<HarrisMatrix>();
	}

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();
        initSettings(height, width);
        if ( dest == null )
            dest = createCompatibleDestImage( src, null );
        
        BufferedImage grayImage = super.filter(src, null);
        int[] inPixels = new int[width*height];
        
		// first step  - Gaussian first-order Derivatives (3 × 3) - X - gradient, (3 × 3) - Y - gradient
		filter.setDirectionType(GaussianDerivativeFilter.X_DIRECTION);
		BufferedImage xImage = filter.filter(grayImage, null);
		getRGB( xImage, 0, 0, width, height, inPixels );
		extractPixelData(inPixels, GaussianDerivativeFilter.X_DIRECTION, height, width);
		
		filter.setDirectionType(GaussianDerivativeFilter.Y_DIRECTION);
		BufferedImage yImage = filter.filter(grayImage, null);
		getRGB( yImage, 0, 0, width, height, inPixels );
		extractPixelData(inPixels, GaussianDerivativeFilter.Y_DIRECTION, height, width);
				
		// second step - calculate the Ix^2, Iy^2 and Ix^Iy
		for(HarrisMatrix hm : harrisMatrixList)
		{
			double Ix = hm.getXGradient();
			double Iy = hm.getYGradient();
			hm.setIxIy(Ix * Iy);
			hm.setXGradient(Ix*Ix);
			hm.setYGradient(Iy*Iy);
		}
		
		// 基于高斯方法，中心点化窗口计算一阶导数和，关键一步 SumIx2, SumIy2 and SumIxIy, 高斯模糊
		calculateGaussianBlur(width, height);

		// 求取Harris Matrix 特征值 
		// 计算角度相应值R R= Det(H) - lambda * (Trace(H))^2
		harrisResponse(width, height);
		
		// based on R, compute non-max suppression
		nonMaxValueSuppression(width, height);
		
		// match result to original image and highlight the key points
		int[] outPixels = matchToImage(width, height, src);
		
		// return result image
		setRGB( dest, 0, 0, width, height, outPixels );
		return dest;
	}
	
	
	private int[] matchToImage(int width, int height, BufferedImage src) {
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
                HarrisMatrix hm = harrisMatrixList.get(index);
                if(hm.getMax() > 0)
                {
                	tr = 0;
                	tg = 255; // make it as green for corner key pointers
                	tb = 0;
                	outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
                }
                else
                {
                	outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;                	
                }
                
        	}
        }
		return outPixels;
	}
	/***
	 * we still use the 3*3 windows to complete the non-max response value suppression
	 */
	private void nonMaxValueSuppression(int width, int height) {
        int index = 0;
        int radius = (int)window_radius;
        for(int row=0; row<height; row++) {
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		HarrisMatrix hm = harrisMatrixList.get(index);
        		double maxR = hm.getR();
        		boolean isMaxR = true;
        		for(int subrow =-radius; subrow<=radius; subrow++)
        		{
        			for(int subcol=-radius; subcol<=radius; subcol++)
        			{
        				int nrow = row + subrow;
        				int ncol = col + subcol;
        				if(nrow >= height || nrow < 0)
        				{
        					nrow = 0;
        				}
        				if(ncol >= width || ncol < 0)
        				{
        					ncol = 0;
        				}
        				int index2 = nrow * width + ncol;
        				HarrisMatrix hmr = harrisMatrixList.get(index2);
        				if(hmr.getR() > maxR)
        				{
        					isMaxR = false;
        				}
        			}       			
        		}
        		if(isMaxR)
        		{
        			hm.setMax(maxR);
        		}
        	}
        }
		
	}
	
	/***
	 * 计算两个特征值，然后得到R，公式如下，可以自己推导，关于怎么计算矩阵特征值，请看这里：
	 * http://www.sosmath.com/matrix/eigen1/eigen1.html
	 * 
	 * 	A = Sxx;
	 *	B = Syy;
	 *  C = Sxy*Sxy*4;
	 *	lambda = 0.04;
	 *	H = (A*B - C) - lambda*(A+B)^2;
     *
	 * @param width
	 * @param height
	 */
	private void harrisResponse(int width, int height) {
        int index = 0;
        for(int row=0; row<height; row++) {
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		HarrisMatrix hm = harrisMatrixList.get(index);
        		double c =  hm.getIxIy() * hm.getIxIy();
        		double ab = hm.getXGradient() * hm.getYGradient();
        		double aplusb = hm.getXGradient() + hm.getYGradient();
        		double response = (ab -c) - lambda * Math.pow(aplusb, 2);
        		hm.setR(response);
        	}
        }		
	}

	private void calculateGaussianBlur(int width, int height) {
        int index = 0;
        int radius = (int)window_radius;
        double[][] gw = get2DKernalData(radius, sigma);
        double sumxx = 0, sumyy = 0, sumxy = 0;
        for(int row=0; row<height; row++) {
        	for(int col=0; col<width; col++) {        		
        		for(int subrow =-radius; subrow<=radius; subrow++)
        		{
        			for(int subcol=-radius; subcol<=radius; subcol++)
        			{
        				int nrow = row + subrow;
        				int ncol = col + subcol;
        				if(nrow >= height || nrow < 0)
        				{
        					nrow = 0;
        				}
        				if(ncol >= width || ncol < 0)
        				{
        					ncol = 0;
        				}
        				int index2 = nrow * width + ncol;
        				HarrisMatrix whm = harrisMatrixList.get(index2);
        				sumxx += (gw[subrow + radius][subcol + radius] * whm.getXGradient());
        				sumyy += (gw[subrow + radius][subcol + radius] * whm.getYGradient());
        				sumxy += (gw[subrow + radius][subcol + radius] * whm.getIxIy());
        			}
        		}
        		index = row * width + col;
        		HarrisMatrix hm = harrisMatrixList.get(index);
        		hm.setXGradient(sumxx);
        		hm.setYGradient(sumyy);
        		hm.setIxIy(sumxy);
        		
        		// clean up for next loop
        		sumxx = 0;
        		sumyy = 0;
        		sumxy = 0;
        	}
        }		
	}
	
	public double[][] get2DKernalData(int n, double sigma) {
		int size = 2*n +1;
		double sigma22 = 2*sigma*sigma;
		double sigma22PI = Math.PI * sigma22;
		double[][] kernalData = new double[size][size];
		int row = 0;
		for(int i=-n; i<=n; i++) {
			int column = 0;
			for(int j=-n; j<=n; j++) {
				double xDistance = i*i;
				double yDistance = j*j;
				kernalData[row][column] = Math.exp(-(xDistance + yDistance)/sigma22)/sigma22PI;
				column++;
			}
			row++;
		}
		
//		for(int i=0; i<size; i++) {
//			for(int j=0; j<size; j++) {
//				System.out.print("\t" + kernalData[i][j]);
//			}
//			System.out.println();
//			System.out.println("\t ---------------------------");
//		}
		return kernalData;
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
                HarrisMatrix matrix = harrisMatrixList.get(index);
                if(type == GaussianDerivativeFilter.X_DIRECTION)
                {
                	matrix.setXGradient(tr);
                }
                if(type == GaussianDerivativeFilter.Y_DIRECTION)
                {
                	matrix.setYGradient(tr);
                }
        	}
        }
	}
	
	private void initSettings(int height, int width)
	{
        int index = 0;
        for(int row=0; row<height; row++) {
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		HarrisMatrix matrix = new HarrisMatrix();
                harrisMatrixList.add(index, matrix);
        	}
        }
	}

}
```
**最后注意：**


我是把彩色图像变为灰度图像来计算，这个计算量小点

处理容易点，此外很多图像处理软件都会用标记来显示

关键点像素，我没有，只是将关键点像素改为绿色。

所以可以从这些方面有很大的提高空间。



