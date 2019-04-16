# 图像处理之Zhang Suen细化算法 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年08月07日 16:14:35[gloomyfish](https://me.csdn.net/jia20003)阅读数：9116








在二值图像处理特别是OCR识别与匹配中，都要通过对字符进行细化以便获得图像的骨架，通过zhang-suen细化算法获得图像，作为图像的特征之一，常用来作为识别或者模式匹配。

**一：算法介绍**

Zhang-Suen细化算法通常是一个迭代算法，整个迭代过程分为两步：

Step One：循环所有前景像素点，对符合如下条件的像素点标记为删除：

1.      2 <= N(p1) <=6

2.      S(P1) = 1

3.      P2 * P4 * P6 = 0

4.      P4 * P6 * P8 = 0

其中N(p1)表示跟P1相邻的8个像素点中，为前景像素点的个数

S(P1)表示从P2 ～ P9 ～ P2像素中出现0～1的累计次数，其中0表示背景，1表示前景

完整的P1 ～P9的像素位置与举例如下：

![](https://img-blog.csdn.net/20160807160046220)




其中 N(p1) = 4, S(P1) = 3, P2*P4*P6=0*0*0=0, P4*P6*P8=0*0*1=0, 不符合条件，无需标记为删除。

Step Two：跟Step One很类似，条件1、2完全一致，只是条件3、4稍微不同，满足如下条件的像素P1则标记为删除，条件如下：

1.      2 <= N(p1) <=6

2.      S(P1) = 1

3.      P2 * P4 * P8 = 0

4.      P2 * P6 * P8 = 0

循环上述两步骤，直到两步中都没有像素被标记为删除为止，输出的结果即为二值图像细化后的骨架。

二：代码实现步骤

1.      二值化输入图像，初始化图像像素对应的标记映射数组



```java
BufferedImage binaryImage = super.process(image);
		int width = binaryImage.getWidth();
		int height = binaryImage.getHeight();
		int[] pixels = new int[width*height];
		int[] flagmap = new int[width*height];
		getRGB(binaryImage, 0, 0, width, height, pixels);
		Arrays.fill(flagmap, 0);
```

2.      迭代细化算法(Zhang-Suen)

a.      Step One



```java
private boolean step1Scan(int[] input, int[] flagmap, int width, int height) {
		boolean stop = true;
		int bc = 255 - fcolor;
		int p1=0, p2=0, p3=0;
		int p4=0, p5=0, p6=0;
		int p7=0, p8=0, p9=0;
		int offset = 0;
		for(int row=1; row<height-1; row++) {
			offset = row*width;
			for(int col=1; col<width-1; col++) {
				p1 = (input[offset+col]>>16)&0xff;
				if(p1 == bc) continue;
				p2 = (input[offset-width+col]>>16)&0xff;
				p3 = (input[offset-width+col+1]>>16)&0xff;
				p4 = (input[offset+col+1]>>16)&0xff;
				p5 = (input[offset+width+col+1]>>16)&0xff;
				p6 = (input[offset+width+col]>>16)&0xff;
				p7 = (input[offset+width+col-1]>>16)&0xff;
				p8 = (input[offset+col-1]>>16)&0xff;
				p9 = (input[offset-width+col-1]>>16)&0xff;
				// match 1 - 前景像素  0 - 背景像素
				p1 = (p1 == fcolor) ? 1 : 0;
				p2 = (p2 == fcolor) ? 1 : 0;
				p3 = (p3 == fcolor) ? 1 : 0;
				p4 = (p4 == fcolor) ? 1 : 0;
				p5 = (p5 == fcolor) ? 1 : 0;
				p6 = (p6 == fcolor) ? 1 : 0;
				p7 = (p7 == fcolor) ? 1 : 0;
				p8 = (p8 == fcolor) ? 1 : 0;
				p9 = (p9 == fcolor) ? 1 : 0;
				
				int con1 = p2+p3+p4+p5+p6+p7+p8+p9;
				String sequence = "" + String.valueOf(p2) + String.valueOf(p3) + String.valueOf(p4) + String.valueOf(p5) +
						String.valueOf(p6) + String.valueOf(p7) + String.valueOf(p8) + String.valueOf(p9) + String.valueOf(p2);
				int index1 = sequence.indexOf("01");
				int index2 = sequence.lastIndexOf("01");
				
				int con3 = p2*p4*p6;
				int con4 = p4*p6*p8;
				
				if((con1 >= 2 && con1 <= 6) && (index1 == index2) && con3 == 0 && con4 == 0) {
					flagmap[offset+col] = 1;
					stop = false;
				}

			}
		}
		return stop;
	}
```
b.      Step Two


```java
private boolean step2Scan(int[] input, int[] flagmap, int width, int height) {
		boolean stop = true;
		int bc = 255 - fcolor;
		int p1=0, p2=0, p3=0;
		int p4=0, p5=0, p6=0;
		int p7=0, p8=0, p9=0;
		int offset = 0;
		for(int row=1; row<height-1; row++) {
			offset = row*width;
			for(int col=1; col<width-1; col++) {
				p1 = (input[offset+col]>>16)&0xff;
				if(p1 == bc) continue;
				p2 = (input[offset-width+col]>>16)&0xff;
				p3 = (input[offset-width+col+1]>>16)&0xff;
				p4 = (input[offset+col+1]>>16)&0xff;
				p5 = (input[offset+width+col+1]>>16)&0xff;
				p6 = (input[offset+width+col]>>16)&0xff;
				p7 = (input[offset+width+col-1]>>16)&0xff;
				p8 = (input[offset+col-1]>>16)&0xff;
				p9 = (input[offset-width+col-1]>>16)&0xff;
				// match 1 - 前景像素  0 - 背景像素
				p1 = (p1 == fcolor) ? 1 : 0;
				p2 = (p2 == fcolor) ? 1 : 0;
				p3 = (p3 == fcolor) ? 1 : 0;
				p4 = (p4 == fcolor) ? 1 : 0;
				p5 = (p5 == fcolor) ? 1 : 0;
				p6 = (p6 == fcolor) ? 1 : 0;
				p7 = (p7 == fcolor) ? 1 : 0;
				p8 = (p8 == fcolor) ? 1 : 0;
				p9 = (p9 == fcolor) ? 1 : 0;
				
				int con1 = p2+p3+p4+p5+p6+p7+p8+p9;
				String sequence = "" + String.valueOf(p2) + String.valueOf(p3) + String.valueOf(p4) + String.valueOf(p5) +
						String.valueOf(p6) + String.valueOf(p7) + String.valueOf(p8) + String.valueOf(p9) + String.valueOf(p2);
				int index1 = sequence.indexOf("01");
				int index2 = sequence.lastIndexOf("01");
				
				int con3 = p2*p4*p8;
				int con4 = p2*p6*p8;
				
				if((con1 >= 2 && con1 <= 6) && (index1 == index2) && con3 == 0 && con4 == 0) {
					flagmap[offset+col] = 1;
					stop = false;
				}

			}
		}
		return stop;
	}
```
c.      检查如果上述两部没有任何像素被标记，则停止迭代，否则继续执行a, b
3.      返回细化后的图像，并显示

**三：运行效果**

![](https://img-blog.csdn.net/20160807160207570)




**四：完整的Zhang-suen算法代码实现：**

```java
import java.awt.image.BufferedImage;
import java.util.Arrays;

public class ZhangSuenThinFilter extends BinaryFilter {
	private int fcolor;
	public ZhangSuenThinFilter() {
		fcolor = 0;
	}

	public int getFcolor() {
		return fcolor;
	}


	public void setFcolor(int fcolor) {
		this.fcolor = fcolor;
	}
	
	@Override
	public BufferedImage process(BufferedImage image) {
		BufferedImage binaryImage = super.process(image);
		int width = binaryImage.getWidth();
		int height = binaryImage.getHeight();
		int[] pixels = new int[width*height];
		int[] flagmap = new int[width*height];
		getRGB(binaryImage, 0, 0, width, height, pixels);
		Arrays.fill(flagmap, 0);
		
		
		// 距离变化
		boolean stop = false;
		while(!stop) {
			// step one
			boolean s1 = step1Scan(pixels, flagmap, width, height);
			deletewithFlag(pixels, flagmap);
			Arrays.fill(flagmap, 0);
			// step two
			boolean s2 = step2Scan(pixels, flagmap, width, height);
			deletewithFlag(pixels, flagmap);
			Arrays.fill(flagmap, 0);
			if(s1 && s2) {
				stop = true;
			}
		}

		// 结果
		BufferedImage bi = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
		setRGB(bi, 0, 0, width, height, pixels);
		return bi;
	}
	
	private void deletewithFlag(int[] pixels, int[] flagmap) {
		int bc = 255 - fcolor;
		for(int i=0; i<pixels.length; i++) {
			if(flagmap[i] == 1) {
				pixels[i] = (0xff << 24) | ((bc&0xff) << 16) | ((bc&0xff) << 8) | (bc&0xff);
			}
		}
		
	}

	private boolean step1Scan(int[] input, int[] flagmap, int width, int height) {
		boolean stop = true;
		int bc = 255 - fcolor;
		int p1=0, p2=0, p3=0;
		int p4=0, p5=0, p6=0;
		int p7=0, p8=0, p9=0;
		int offset = 0;
		for(int row=1; row<height-1; row++) {
			offset = row*width;
			for(int col=1; col<width-1; col++) {
				p1 = (input[offset+col]>>16)&0xff;
				if(p1 == bc) continue;
				p2 = (input[offset-width+col]>>16)&0xff;
				p3 = (input[offset-width+col+1]>>16)&0xff;
				p4 = (input[offset+col+1]>>16)&0xff;
				p5 = (input[offset+width+col+1]>>16)&0xff;
				p6 = (input[offset+width+col]>>16)&0xff;
				p7 = (input[offset+width+col-1]>>16)&0xff;
				p8 = (input[offset+col-1]>>16)&0xff;
				p9 = (input[offset-width+col-1]>>16)&0xff;
				// match 1 - 前景像素  0 - 背景像素
				p1 = (p1 == fcolor) ? 1 : 0;
				p2 = (p2 == fcolor) ? 1 : 0;
				p3 = (p3 == fcolor) ? 1 : 0;
				p4 = (p4 == fcolor) ? 1 : 0;
				p5 = (p5 == fcolor) ? 1 : 0;
				p6 = (p6 == fcolor) ? 1 : 0;
				p7 = (p7 == fcolor) ? 1 : 0;
				p8 = (p8 == fcolor) ? 1 : 0;
				p9 = (p9 == fcolor) ? 1 : 0;
				
				int con1 = p2+p3+p4+p5+p6+p7+p8+p9;
				String sequence = "" + String.valueOf(p2) + String.valueOf(p3) + String.valueOf(p4) + String.valueOf(p5) +
						String.valueOf(p6) + String.valueOf(p7) + String.valueOf(p8) + String.valueOf(p9) + String.valueOf(p2);
				int index1 = sequence.indexOf("01");
				int index2 = sequence.lastIndexOf("01");
				
				int con3 = p2*p4*p6;
				int con4 = p4*p6*p8;
				
				if((con1 >= 2 && con1 <= 6) && (index1 == index2) && con3 == 0 && con4 == 0) {
					flagmap[offset+col] = 1;
					stop = false;
				}

			}
		}
		return stop;
	}
	
	private boolean step2Scan(int[] input, int[] flagmap, int width, int height) {
		boolean stop = true;
		int bc = 255 - fcolor;
		int p1=0, p2=0, p3=0;
		int p4=0, p5=0, p6=0;
		int p7=0, p8=0, p9=0;
		int offset = 0;
		for(int row=1; row<height-1; row++) {
			offset = row*width;
			for(int col=1; col<width-1; col++) {
				p1 = (input[offset+col]>>16)&0xff;
				if(p1 == bc) continue;
				p2 = (input[offset-width+col]>>16)&0xff;
				p3 = (input[offset-width+col+1]>>16)&0xff;
				p4 = (input[offset+col+1]>>16)&0xff;
				p5 = (input[offset+width+col+1]>>16)&0xff;
				p6 = (input[offset+width+col]>>16)&0xff;
				p7 = (input[offset+width+col-1]>>16)&0xff;
				p8 = (input[offset+col-1]>>16)&0xff;
				p9 = (input[offset-width+col-1]>>16)&0xff;
				// match 1 - 前景像素  0 - 背景像素
				p1 = (p1 == fcolor) ? 1 : 0;
				p2 = (p2 == fcolor) ? 1 : 0;
				p3 = (p3 == fcolor) ? 1 : 0;
				p4 = (p4 == fcolor) ? 1 : 0;
				p5 = (p5 == fcolor) ? 1 : 0;
				p6 = (p6 == fcolor) ? 1 : 0;
				p7 = (p7 == fcolor) ? 1 : 0;
				p8 = (p8 == fcolor) ? 1 : 0;
				p9 = (p9 == fcolor) ? 1 : 0;
				
				int con1 = p2+p3+p4+p5+p6+p7+p8+p9;
				String sequence = "" + String.valueOf(p2) + String.valueOf(p3) + String.valueOf(p4) + String.valueOf(p5) +
						String.valueOf(p6) + String.valueOf(p7) + String.valueOf(p8) + String.valueOf(p9) + String.valueOf(p2);
				int index1 = sequence.indexOf("01");
				int index2 = sequence.lastIndexOf("01");
				
				int con3 = p2*p4*p8;
				int con4 = p2*p6*p8;
				
				if((con1 >= 2 && con1 <= 6) && (index1 == index2) && con3 == 0 && con4 == 0) {
					flagmap[offset+col] = 1;
					stop = false;
				}

			}
		}
		return stop;
	}

}
```
**觉得不错请支持一下！**


