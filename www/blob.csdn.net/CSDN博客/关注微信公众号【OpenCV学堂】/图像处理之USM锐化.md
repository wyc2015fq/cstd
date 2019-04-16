# 图像处理之USM锐化 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年07月09日 00:46:36[gloomyfish](https://me.csdn.net/jia20003)阅读数：10185
所属专栏：[Java数字图像处理与特效](https://blog.csdn.net/column/details/image-process.html)









一：基本原理

图像卷积处理实现锐化有一种常用的算法叫做Unsharpen Mask方法，这种锐化的方法就是对原图像先做一个高斯模糊，然后用原来的图像减去一个系数乘以高斯模糊之后的图像，然后再把值Scale到0～255的RGB像素值范围之内。基于USM锐化的方法可以去除一些细小的干扰细节和噪声，比一般直接使用卷积锐化算子得到的图像锐化结果更加真实可信。USM锐化公式表示如下：

（源图像– w*高斯模糊）/（1-w）；其中w表示权重（0.1～0.9），默认为0.6



二：实现步骤

1.      读入图像的像素数据

2.      对图像像素数据实现高斯模糊，

3.      根据输入参数w，对图像上的每个像素点，使用USM锐化公式计算每个像素点锐化之后的像素

4.      构建一张新的输出图像，返回显示



三：运行效果

![](https://img-blog.csdn.net/20160709004138970)


四：代码实现

高斯模糊代码如下：



```java
int width = image.getWidth();
		int height = image.getHeight();
		// generateKerneal();
		generateKerneal2D();
		int radius = (int)(this.sigma*2);
		// 二维高斯模糊
		int[] pixels = new int[width*height];
		int[] outPixels = new int[width*height];
		getRGB(image, 0, 0 , width, height, pixels);
		int r=0, g=0, b=0;
		int r1=0, g1=0, b1=0;
		for(int row=0; row<height; row++) {
			int offset = row*width;
			for(int col=1; col<width-1; col++) {
				double sr=0, sg=0, sb=0; 
				// 二维高斯窗口
				for(int i=-radius; i<=radius; i++) {
					int roffset = row + i;
					if(roffset < 0) {
						roffset = 0;
					}
					if(roffset >= height) {
						roffset = height-1;
					}
					int offset1 = roffset*width;
					for(int j=-radius; j<=radius; j++) {
						int coffset = j+col;
						if(coffset < 0 ) {
							coffset = 0;
						}
						if(coffset >= width) {
							coffset = width-1;
						}
						r1 = (pixels[offset1+coffset]>>16)&0xff;
						g1 = (pixels[offset1+coffset]>>8)&0xff;
						b1 = (pixels[offset1+coffset]&0xff);
						sr += kernel2D[i+radius][j+radius]*r1;
						sg += kernel2D[i+radius][j+radius]*g1;
						sb += kernel2D[i+radius][j+radius]*b1;
						
					}
				}
				r = (int)sr;
				g = (int)sg;
				b = (int)sb;
				outPixels[offset+col]=(0xff<<24) | (r<<16) | (g << 8) | b;
			}
		}
```

基于高斯模糊的代码是USM锐化的代码如下：




```java
int width = image.getWidth();
		int height = image.getHeight();

		int[] pixels1 = new int[width*height];
		int[] pixels2 = new int[width*height];
		getRGB(image, 0, 0 , width, height, pixels1);
		
		// 高斯模糊
		BufferedImage blurImage = super.process(image);
		getRGB(blurImage, 0, 0 , width, height, pixels2);
		
		// USM 锐化
		int[] output = new int[width*height];
		int r=0, g=0, b=0;
		int r1=0, g1=0, b1=0;
		int r2=0, g2=0, b2=0;
		for(int i=0; i<pixels1.length; i++) {
			r1 = (pixels1[i] >> 16)&0xff;
			g1 = (pixels1[i] >> 8)&0xff;
			b1 = pixels1[i]&0xff;
			
			r2 = (pixels2[i] >> 16)&0xff;
			g2 = (pixels2[i] >> 8)&0xff;
			b2 = pixels2[i]&0xff;
			
			r = (int)((r1-weight*r2)/(1-weight));
			g = (int)((g1-weight*g2)/(1-weight));
			b = (int)((b1-weight*b2)/(1-weight));
			
			output[i]=(0xff<<24) | (clamp(r)<<16) | (clamp(g) << 8) | clamp(b);
		}
		
		BufferedImage dest = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
		setRGB(dest, 0, 0, width, height, output);
		return dest;
```
其中BufferedImage blurImage = super.process(image);调用的代码为高斯模糊







