# 图像处理之透明混合 - Alpha Blending效果 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年11月09日 22:15:22[gloomyfish](https://me.csdn.net/jia20003)阅读数：8753








基本原理：

图像的透明混合有个专属名词– Alpha Blending



对任意两张图像可以合成为一张图像，合成图像的像素取值根据数学公式：

RGB3 = (1- a) * RGB1 + a * RGB2

其中a为混合透明度取值范围[0, 1]之间, RGB3为目标像素值, RGB1与RGB2的值分别来自两

张不同的图像。




两张源图像分别为：

![](http://hi.csdn.net/attachment/201111/9/0_1320847524xDId.gif)


第二张源图像是房屋设计图

![](http://hi.csdn.net/attachment/201111/9/0_1320847531ut27.gif)





三：最终程序效果如下

![](http://hi.csdn.net/attachment/201111/9/0_1320847673wD07.gif)





四：程序关键代码及解释

获取BufferedImage对象中像素数据的代码如下：



```java
public void getRGB(BufferedImage img, int x, int y, int width, int height, int[] pixelsData) {
		int type = img.getType();
		if (type == BufferedImage.TYPE_INT_ARGB || type == BufferedImage.TYPE_INT_RGB) {
			img.getRaster().getDataElements(x, y, width, width, pixelsData);
		} else {
			img.getRGB(x, y, width, height, pixelsData, 0, img.getWidth());
		}
	}
```


将处理后的像素数组写到新创建的BufferedImage对象中的代码如下：



```java
public void setRGB(BufferedImage img, int x, int y, int width, int height, int[] pixelsData) {
		int type = img.getType();
		if (type == BufferedImage.TYPE_INT_ARGB || type == BufferedImage.TYPE_INT_RGB) {
			img.getRaster().setDataElements(x, y, width, height, pixelsData);
		} else {
			img.setRGB(x, y, width, height, pixelsData, 0, width);
		}
	}
```


创建一个新BufferedImage对象代码如下：



```java
alphaBlendingImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
```


实现Alpha Blending的代码如下：



```java
float blendingRate = 0.5f;
	private void processPixels(int[] inPixelsOne, int[] inPixelsTwo, int[] outPixelsData, int width, int height) {
		int index = 0;
		for(int row=0; row<height; row++) {
			for(int col=0; col<width; col++) {
				int ta = 0, tr = 0, tg = 0, tb = 0;
				int rgb1 = inPixelsOne[index];
				int rgb2 = inPixelsTwo[index];
				ta = ((rgb1 >> 24) & 0xff) + ((rgb2 >> 24) & 0xff);
                tr = ((rgb1 >> 16) & 0xff) + ((rgb2 >> 16) & 0xff);
                tg = ((rgb1 >> 8) & 0xff) + ((rgb2 >> 8) & 0xff);
                tb = (rgb1 & 0xff) + (rgb2 & 0xff);
                
                int a = 0, r=0, g=0, b=0;
                a = (int)(blendingRate *(float)ta);
                r = (int)(blendingRate *(float)tr);
                g = (int)(blendingRate *(float)tg);
                b = (int)(blendingRate *(float)tb);
                
                outPixelsData[index] = ((a << 24) & 0xFF000000)
				| ((r << 16) & 0x00FF0000)
				| ((g << 8) & 0x0000FF00)
				| ((b) & 0x000000FF);
                index++;
			}
		}
		
	}
```


本例中，为了简化计算假设alpah blending系数为0.5

加载/读取图像文件的代码如下：



```java
if (srcImageOne == null) {
					File file_001 = new File("D:\\resource\\350_001.png");
					srcImageOne = ImageIO.read(file_001);
				}
				if (srcImageTwo == null) {
					File file_002 = new File("D:\\resource\\350_002.png");
					srcImageTwo = ImageIO.read(file_002);
				}
```



