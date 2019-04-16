# Canny边缘检测高低阈值连接的代码修正 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年05月18日 10:45:40[gloomyfish](https://me.csdn.net/jia20003)阅读数：5628








在《Java数字图像处理-编程技巧与应用实践》一书的第九章讲到了Canny边缘检测的代码实现，在求取梯度与角度处理，非最大信号压制之后，有一步是通过两个阈值（高低阈值）实现边缘断线的连接，得到完整的边缘，之前给出的代码是固定阈值，这个有两个改动：

**改动一：**

改为自动阈值了，高的阈值是梯度的平均值means的两倍，低阈值是平均值的二分之一。

**改动二：**

高低值连接的时候递归程序改为非递归程序，通过图的深度优先队列实现边缘像素连接。

这两个改动的原因是有读者向我反馈说提供的代码在处理大图片的时候递归的代码导致栈溢出错误，如今改为非递归之后，我测试过1600x1200的图片进行边缘提取，完全没有问题。

先看效果：原图：

![](https://img-blog.csdn.net/20160518104207717)


处理之后：

![](https://img-blog.csdn.net/20160518104225895)




源代码改动说明：

不再使用follow(col, row, offset, lowThreshold);方法

直接改为非递归的图的深度优先队列进行处理，对每个像素点构建PixelNode数据结构

```java
package com.book.chapter.nine;

public class PixelNode {
	protected int row;
	protected int col;
	protected int index;
	protected boolean high;
}
```
自动阈值与非递归的代码如下：


```java
// 寻找最大与最小值
		float min = 255;
		float max = 0;
		double sum = 0;
		double count = 0;
		for(int i=0; i<magnitudes.length; i++)
		{
			if(magnitudes[i] == 0) continue;
			min = Math.min(min, magnitudes[i]);
			max = Math.max(max, magnitudes[i]);
			sum += magnitudes[i];
			count++;
		}
		System.out.println("Image Max Gradient = " + max + " Mix Gradient = " + min);
		float means = (float)(sum / count);
		System.out.println("means = " + (sum / count));
		// 通常比值为 TL : TH = 1 : 3， 根据两个阈值完成二值化边缘连接
		// 边缘连接-link edges
		Arrays.fill(data, 0);
		int offset = 0;
		highThreshold =  means * 2;
		lowThreshold = means / 2;
		System.out.println("ddddddddddddddddddddddddd");
		Queue<PixelNode> queue = new LinkedList<PixelNode>();
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				if(magnitudes[offset] >= highThreshold && data[offset] == 0)
				{
					PixelNode pn = new PixelNode();
					pn.row = row;
					pn.col = col;
					pn.index = offset;
					pn.high = true;
					queue.add(pn);
					while(!queue.isEmpty()) {
						PixelNode node = queue.poll();
						int x0 = (node.col == 0) ? node.col : node.col - 1;
						int x2 = (node.col == width - 1) ? node.col : node.col + 1;
						int y0 = node.row == 0 ? node.row : node.row - 1;
						int y2 = node.row == height -1 ? node.row : node.row + 1;
						
						data[node.index] = magnitudes[node.index];
						for (int x = x0; x <= x2; x++) {
							for (int y = y0; y <= y2; y++) {
								int i2 = x + y * width;
								if ((y != node.row || x != node.col)
									&& data[i2] == 0 
									&& magnitudes[i2] >= lowThreshold) {
									PixelNode middleNode = new PixelNode();
									middleNode.col = x;
									middleNode.row = y;
									middleNode.index = i2;
									middleNode.high = false;
									queue.offer(middleNode);
								}
							}
						}
					}
				}
				queue.clear();
				offset++;
			}
		}
```



其它部分的源代码参考这里[http://blog.csdn.net/jia20003/article/details/41173767](http://blog.csdn.net/jia20003/article/details/41173767)

不再赘述！




