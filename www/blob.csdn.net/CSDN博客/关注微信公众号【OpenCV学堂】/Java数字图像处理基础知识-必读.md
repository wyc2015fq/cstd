# Java数字图像处理基础知识 - 必读 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年02月21日 15:11:36[gloomyfish](https://me.csdn.net/jia20003)阅读数：57443










写了很多篇关于图像处理的文章，没有一篇介绍Java 2D的图像处理API，文章讨论和提及的

API都是基于JDK6的，首先来看Java中如何组织一个图像对象BufferedImage的，如图：

![](http://hi.csdn.net/attachment/201202/21/0_13298081690mwi.gif)


一个BufferedImage的像素数据储存在Raster中，ColorModel里面储存颜色空间，类型等

信息，当前Java只支持一下三种图像格式- JPG,PNG,GIF,如何向让Java支持其它格式，首

先要 完成Java中的图像读写接口，然后打成jar，加上启动参数- Xbootclasspath/p 

newimageformatIO.jar即可。



**Java中如何读写一个图像文件，使用ImageIO对象即可。读图像文件的代码如下：**



```java
File file = new File("D:\\test\\blue_flower.jpg");
BufferedImage image = ImageIO.read(file);
```




**写图像文件的代码如下：**



```java
File outputfile = new File("saved.png");
ImageIO.write(bufferedImage, "png",outputfile);
```




**从BufferedImage对象中读取像素数据的代码如下：**



```java
int type= image.getType();
if ( type ==BufferedImage.TYPE_INT_ARGB || type == BufferedImage.TYPE_INT_RGB )
     return (int [])image.getRaster().getDataElements(x, y, width, height, pixels );
else
    return image.getRGB( x, y, width, height, pixels, 0, width );
```



首先获取图像类型，如果不是32位的INT型数据，直接读写RGB值即可，否则需要从Raster

对象中读取。



**往BufferedImage对象中写入像素数据同样遵守上面的规则。代码如下：**



```java
int type= image.getType();
if ( type ==BufferedImage.TYPE_INT_ARGB || type == BufferedImage.TYPE_INT_RGB )
   image.getRaster().setDataElements(x, y, width, height, pixels );
else
   image.setRGB(x, y, width, height, pixels, 0, width );
```





**读取图像可能因为图像文件比较大，需要一定时间的等待才可以，Java Advance Image**

**Processor API提供了MediaTracker对象来跟踪图像的加载，同步其它操作，使用方法如下：**



```java
MediaTracker tracker = new MediaTracker(this); //初始化对象
tracker.addImage(image_01, 1); // 加入要跟踪的BufferedImage对象image_001
tracker.waitForID(1, 10000) // 等待10秒，让iamge_01图像加载
```




**从一个32位int型数据cARGB中读取图像RGB颜色值的代码如下：**



```java
int alpha = (cARGB >> 24)& 0xff; //透明度通道
int red = (cARGB >> 16) &0xff;
int green = (cARGB >> 8) &0xff;
int blue = cARGB & 0xff;
```




**将RGB颜色值写入成一个INT型数据cRGB的代码如下：**



```java
cRGB = (alpha << 24) | (red<< 16) | (green << 8) | blue;
```




**创建一个BufferedImage对象的代码如下：**



```java
BufferedImage image = newBufferedImage(256, 256, BufferedImage.TYPE_INT_ARGB);
```




**一个完整的源代码Demo如下：**



```java
package com.gloomyfish.swing;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JComponent;
import javax.swing.JFrame;

public class PlasmaDemo extends JComponent {  
  
    /** 
     *  
     */  
    private static final long serialVersionUID = -2236160343614397287L;  
    private BufferedImage image = null;  
    private int size = 256;
      
    public PlasmaDemo() {  
        super();  
        this.setOpaque(false);  
    }  
      
    protected void paintComponent(Graphics g) {  
        Graphics2D g2 = (Graphics2D)g;  
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);  
        g2.drawImage(getImage(), 5, 5, image.getWidth(), image.getHeight(), null);  
    }  
      
    private BufferedImage getImage() {  
        if(image == null) {  
            image = new BufferedImage(size, size, BufferedImage.TYPE_INT_ARGB);  
            int[] rgbData = new int[size*size];  
            generateNoiseImage(rgbData);  
            setRGB(image, 0, 0, size, size, rgbData);
            File outFile = new File("plasma.jpg");
            try {
				ImageIO.write(image, "jpg", outFile);
			} catch (IOException e) {
				e.printStackTrace();
			}
        }  
        return image;  
    }  
      
    public void generateNoiseImage(int[] rgbData) {  
        int index = 0;  
        int a = 255;  
        int r = 0;  
        int g = 0;  
        int b = 0;  
 
        for(int row=0; row<size; row++) {  
            for(int col=0; col<size; col++) {  
                // set random color value for each pixel  
                r = (int)(128.0 + (128.0 * Math.sin((row + col) / 8.0)));  
                g = (int)(128.0 + (128.0 * Math.sin((row + col) / 8.0)));  
                b = (int)(128.0 + (128.0 * Math.sin((row + col) / 8.0)));  
                  
                rgbData[index] = ((clamp(a) & 0xff) << 24) |  
                                ((clamp(r) & 0xff) << 16)  |  
                                ((clamp(g) & 0xff) << 8)   |  
                                ((clamp(b) & 0xff));  
                index++;  
            }  
        }  
          
    }  
      
    private int clamp(int rgb) {  
        if(rgb > 255)  
            return 255;  
        if(rgb < 0)  
            return 0;  
        return rgb;  
    }    
  
    public void setRGB( BufferedImage image, int x, int y, int width, int height, int[] pixels ) {  
        int type = image.getType();  
        if ( type == BufferedImage.TYPE_INT_ARGB || type == BufferedImage.TYPE_INT_RGB )  
            image.getRaster().setDataElements( x, y, width, height, pixels );  
        else  
            image.setRGB( x, y, width, height, pixels, 0, width );  
    }  
      
    public static void main(String[] args) {  
        JFrame frame = new JFrame("Noise Art Panel");  
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
        frame.getContentPane().setLayout(new BorderLayout());  
          
        // Display the window.  
        frame.getContentPane().add(new PlasmaDemo(), BorderLayout.CENTER);  
        frame.setPreferredSize(new Dimension(400 + 25,450));  
        frame.pack();  
        frame.setVisible(true);  
    }  
}
```
视频教程 - [**《Java 数字图像处理-基础入门》**](http://edu.51cto.com/course/course_id-6335.html)





