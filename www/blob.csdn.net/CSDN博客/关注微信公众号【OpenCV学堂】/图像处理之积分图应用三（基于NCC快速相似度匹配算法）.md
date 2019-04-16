# 图像处理之积分图应用三（基于NCC快速相似度匹配算法） - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年11月03日 15:59:14[gloomyfish](https://me.csdn.net/jia20003)阅读数：13733
所属专栏：[Java数字图像处理与特效](https://blog.csdn.net/column/details/image-process.html)









图像处理之积分图应用三（基于NCC快速相似度匹配算法）


基于Normalized cross correlation(NCC)用来比较两幅图像的相似程度已经是一个常见的图像处理手段。在工业生产环节检测、监控领域对对象检测与识别均有应用。NCC算法可以有效降低光照对图像比较结果的影响。而且NCC最终结果在0到1之间，所以特别容易量化比较结果，只要给出一个阈值就可以判断结果的好与坏。传统的NCC比较方法比较耗时，虽然可以通过调整窗口大小和每次检测的步长矩形部分优化，但是对工业生产检测然后不能达到实时需求，通过积分图像实现预计算，比较模板图像与生产出电子版之间的细微差异，可以帮助企业提高产品质量，减少次品出厂率，把控质量。

一：NCC相关的数学知识

什么是NCC - （normalized cross correlation）归一化的交叉相关性，是数学上统计两组数据之间是否有关系的判断方法，貌似搞大数据分析比较流行相关性分析和计算。正常的计算公式如下：

![](https://img-blog.csdn.net/20161103150551593)


mxn表示窗口大小，这样的计算复杂度就为O(m x n x M x N)。从上面公式就可以看出其中均值和平方和可以通过积分图预计算得到，对于模板和目标图像大小一致的应用场景来说


NCC的计算公式可以表示为如下：

![](https://img-blog.csdn.net/20161103150735178)


其中根据积分图像可以提前计算出任意窗口大小和与平方和，这样就对


![](https://img-blog.csdn.net/20161103151258539)


上述两个计算实现了窗口半径无关的常量时间计算，唯一缺少的是下面计算公式


![](https://img-blog.csdn.net/20161103150946504)


通过积分图像建立起来窗口下面的待检测图像与模板图像的和与平方和以及他们的交叉乘积五个积分图索引之后，这样就完成了整个预计算生成。依靠索引表查找计算结果，NCC就可以实现线性时间的复杂度计算，而且时间消耗近似常量跟窗口半径大小无关，完全可以满足实时对象检测工业环境工作条件。



**二：算法步骤**

1. 预计算模板图像和目标图像的积分图

2. 根据输入的窗口半径大小使用积分图完成NCC计算

3. 根据阈值得到匹配或者不匹配区域。

4. 输出结果

为了减小计算量，我们可以要把输入的图像转换为灰度图像，在灰度图像的基础上完成整个NCC计算检测。我们这个给出的基于RGB图像的NCC计算完整代码，读者可以在此基础上修改实现单通道图像检测。

**三： 运行结果：**

输入的模板图像与待检测图像，左边是模板图像，右边是待检测图像，左上角有明显污点。图像显示如下：

![](https://img-blog.csdn.net/20161103160130498)


**输入待检测图像与模板比较以及检测计算出NCC的图像显示如下：**

![](https://img-blog.csdn.net/20161103152027831)


其中左侧是待检测图像，上面有黑色污点，右侧输出的非黑色区域表明，程序已经发现此区域与标准模板不同，越白的区域表示周围跟模板相同位置反差越大，越是可疑的污染点，这样就可以得到准确定位，最终带检测图像绘制最可疑红色矩形窗口区域

**四：相关代码实现**

1. 计算两张图像每个像素交叉乘积的积分图代码如下：



```java
public void caculateXYSum(byte[] x, byte[] y, int width, int height) {
		if(x.length != y.length)
			return;
		xysum = new float[width*height];
		this.width = width;
		this.height = height;
		// rows
		int px = 0, py = 0;
		int offset = 0, uprow=0, leftcol=0;
		float sp2=0, sp3=0, sp4=0;
		for(int row=0; row<height; row++ ) {
			offset = row*width;
			uprow = row-1;
			for(int col=0; col<width; col++) {
				leftcol=col-1;
				px=x[offset]&0xff;
				py=y[offset]&0xff;
				int p1 = px*py;
				// 计算平方查找表
				sp2=(leftcol<0) ? 0:xysum[offset-1]; // p(x-1, y)
				sp3=(uprow<0) ? 0:xysum[offset-width]; // p(x, y-1);
				sp4=(uprow<0||leftcol<0) ? 0:xysum[offset-width-1]; // p(x-1, y-1);
				xysum[offset]=p1+sp2+sp3-sp4;
				offset++;
			}
		}
	}
```
获取任意窗口大小的交叉乘积的代码如下：




```java
public float getXYBlockSum(int x, int y, int m, int n) {
		int swx = x + n/2;
		int swy = y + m/2;
		int nex = x-n/2-1;
		int ney = y-m/2-1;
		float sum1, sum2, sum3, sum4;
		if(swx >= width) {
			swx = width - 1;
		}
		if(swy >= height) {
			swy = height - 1;
		}
		if(nex < 0) {
			nex = 0;
		}
		if(ney < 0) {
			ney = 0;
		}
		sum1 = xysum[ney*width+nex];
		sum4 = xysum[swy*width+swx];
		sum2 = xysum[swy*width+nex];
		sum3 = xysum[ney*width+swx];
		return ((sum1 + sum4) - sum2 - sum3);
	}
```
其余部分的积分图计算，参见本人博客[《图像处理之积分图算法》](http://blog.csdn.net/jia20003/article/details/52710751)
2. 预计算建立积分图索引的代码如下：




```java
// per-calculate integral image for targetImage
		byte[] R = new byte[width * height];
		byte[] G = new byte[width * height];
		byte[] B = new byte[width * height];
		getRGB(width, height, pixels, R, G, B);
		IntIntegralImage rii = new IntIntegralImage();
		rii.setImage(R);
		rii.process(width, height);
		IntIntegralImage gii = new IntIntegralImage();
		gii.setImage(G);
		gii.process(width, height);
		IntIntegralImage bii = new IntIntegralImage();
		bii.setImage(B);
		bii.process(width, height);

		// setup the refer and target image index sum table
		rii.caculateXYSum(R, referRGB[0].getImage(), width, height);
		gii.caculateXYSum(G, referRGB[1].getImage(), width, height);
		bii.caculateXYSum(B, referRGB[2].getImage(), width, height);
		int size = (xr * 2 + 1) * (yr * 2 + 1);
```
3. 通过积分图查找实现快速NCC计算的代码如下：




```java
int r1=0, g1=0, b1=0;
		int r2=0, g2=0, b2=0;
		
		float sr1=0.0f, sg1=0.0f, sb1 = 0.0f;
		float sr2=0.0f, sg2=0.0f, sb2 = 0.0f;
		
		float xyr = 0.0f, xyg = 0.0f, xyb = 0.0f;
		
		for (int row = yr; row < height - yr; row++) {
			for (int col = xr; col < width - xr; col++) {
				
				r1 = rii.getBlockSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				g1 = gii.getBlockSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				b1 = bii.getBlockSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				
				r2 = referRGB[0].getBlockSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				g2 = referRGB[1].getBlockSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				b2 = referRGB[2].getBlockSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				
				sr1 = rii.getBlockSquareSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				sg1 = gii.getBlockSquareSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				sb1 = bii.getBlockSquareSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				
				sr2 = referRGB[0].getBlockSquareSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				sg2 = referRGB[1].getBlockSquareSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				sb2 = referRGB[2].getBlockSquareSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				
				xyr = rii.getXYBlockSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				xyg = gii.getXYBlockSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				xyb = bii.getXYBlockSum(col, row, (yr * 2 + 1), (xr * 2 + 1));
				
				float nccr = calculateNCC(r1, r2, sr1, sr2, xyr, size);
				float nccg = calculateNCC(g1, g2, sg1, sg2, xyg, size);
				float nccb = calculateNCC(b1, b2, sb1, sb2, xyb, size);
				
				outPixels[row * width + col] = (nccr + nccg + nccb);
			}
		}
		
		System.out.println("time consum : " + (System.currentTimeMillis() - time));
```
4. 归一化输出NCC图像与结果代码如下：




```java
// normalization the data
		float max = 0.0f, min = 100.0f;
		for(int i=0; i<outPixels.length; i++) {
			max = Math.max(max, outPixels[i]);
			min = Math.min(min, outPixels[i]);
		}
		
		// create output image 
		float delta = max - min;
		BufferedImage bi = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
		int ry = -1;
		int rx = -1;
		for(int row = 0; row<height; row++) {
			for(int col=0; col<width; col++) {
				int gray = (int)(((outPixels[row*width+col]-min) / delta) *255);
				gray = 255 - gray;
				if(min == outPixels[row*width+col]) {
					bi.setRGB(col, row, Color.RED.getRGB());
					ry = row;
					rx = col;
				} else {
					int color = (0xff << 24) | (gray << 16) | (gray << 8) | gray;
					bi.setRGB(col, row, color);
				}
			}
		}
		if(rx > 0 && ry > 0) {
			Graphics2D g2d = image.createGraphics();
			g2d.setPaint(Color.RED);
			g2d.drawRect(rx-xr, ry-yr, xr*2, yr*2);
		}
```



相比传统的NCC计算方法，此方法的计算效率是传统方法几百倍提升，而且窗口越大效率提升越明显，有人对此作出的统计如下：

![](https://img-blog.csdn.net/20161103155326932)


可见基于积分图快速NCC可以极大提升执行效率减少计算时间，实现窗口半径无关NCC比较。

**最后**

本文是关于积分图使用的第三篇文章，可以说积分图在实际图像处理中应用十分广泛，本人会继续努力深挖与大家分享。**希望各位顶下次文以表支持**， 谢谢！本人坚持分享有用实用的图像处理算法！需要大家多多支持。



