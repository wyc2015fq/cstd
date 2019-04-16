# OpenCV For Java环境搭建与功能演示 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2017年04月01日 14:47:04[gloomyfish](https://me.csdn.net/jia20003)阅读数：29171
个人分类：[图像处理](https://blog.csdn.net/jia20003/article/category/879324)









### OpenCV概述

OpenCV做为功能强大的计算机视觉开源框架，包含了500多个算法实现，而且还在不断增加，其最新版本已经更新到3.2。其SDK支持Android与Java平台开发，对于常见的图像处理需求几乎都可以满足，理应成为广大Java与Android程序员的首先的图像处理框架。Java中使用OpenCV的配置及其简单，可以毫不客气的说几乎是零配置都可以。

#### 一：配置

配置引入OpenCV相关jar包，首先要下载OpenCV的自解压版本，下载地址： 
[http://opencv.org/opencv-3-2.html](http://opencv.org/opencv-3-2.html)

然后拉到网页的最下方，下载Windows自解压开发包 
![这里写图片描述](https://img-blog.csdn.net/20170401141425687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下载好了双击解压缩之后找到build路径，显示如下： 
![这里写图片描述](https://img-blog.csdn.net/20170401141457360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
双击打开Java文件夹， 
![这里写图片描述](https://img-blog.csdn.net/20170401141542877?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
里面有一个jar直接导入到Eclipse中的新建项目中去， 然后把x64里面的dll文件copy到Eclipse中使用的Java JDK bin和jre/bin目录下面即可。环境就配置好啦，简单吧！配置好的最终项目结构： 
![这里写图片描述](https://img-blog.csdn.net/20170401141610471?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### 二：加载图像与像素操作

读入一张图像 -, 一句话搞定

```java
Mat src = Imgcodecs.imread(imageFilePath);
if(src.empty()) return;
```

将Mat对象转换为BufferedImage对象

```
public BufferedImage conver2Image(Mat mat) {
    int width = mat.cols();
    int height = mat.rows();
    int dims = mat.channels();
    int[] pixels = new int[width*height];
    byte[] rgbdata = new byte[width*height*dims];
    mat.get(0, 0, rgbdata);
    BufferedImage image = new BufferedImage(width, height, 
                            BufferedImage.TYPE_INT_ARGB);
    int index = 0;
    int r=0, g=0, b=0;
    for(int row=0; row<height; row++) {
        for(int col=0; col<width; col++) {
            if(dims == 3) {
                index = row*width*dims + col*dims;
                b = rgbdata[index]&0xff;
                g = rgbdata[index+1]&0xff;
                r = rgbdata[index+2]&0xff;
                pixels[row*width+col] = ((255&0xff)<<24) | 
                ((r&0xff)<<16) | ((g&0xff)<<8) | b&0xff;    
            }
            if(dims == 1) {
                index = row*width + col;
                b = rgbdata[index]&0xff;
                pixels[row*width+col] = ((255&0xff)<<24) | 
                ((b&0xff)<<16) | ((b&0xff)<<8) | b&0xff;    
            }
        }
    }
    setRGB( image, 0, 0, width, height, pixels);
    return image;
}
```

将BufferedImage对象转换为Mat对象

```
public Mat convert2Mat(BufferedImage image) {
    int width = image.getWidth();
    int height = image.getHeight();
    Mat src = new Mat(new Size(width, height), CvType.CV_8UC3);
    int[] pixels = new int[width*height];
    byte[] rgbdata = new byte[width*height*3];
    getRGB( image, 0, 0, width, height, pixels );
    int index = 0, c=0;
    int r=0, g=0, b=0;
    for(int row=0; row<height; row++) {
        for(int col=0; col<width; col++) {
            index = row*width + col;
            c = pixels[index];
            r = (c&0xff0000)>>16;
            g = (c&0xff00)>>8;
            b = c&0xff;

            index = row*width*3 + col*3;
            rgbdata[index] = (byte)b;
            rgbdata[index+1] = (byte)g;
            rgbdata[index+2] = (byte)r;
        }
    }

    src.put(0, 0, rgbdata);
    return src;
}
```

特别要说明一下，BufferedImage与Mat的RGB通道顺序是不一样，正好相反，在Mat对象中三通道的顺序为BGR而在BufferedImage中为RGB。

从Mat中读取全部像素(其中image为Mat类型数据)

```
int width = image.cols();
int height = image.rows();
int dims = image.channels();
byte[] data = new byte[width*height*dims];
image.get(0, 0, data);
```

遍历像素操作与保存改变

```
int index = 0;
int r=0, g=0, b=0;
for(int row=0; row<height; row++) {
    for(int col=0; col<width*dims; col+=dims) {
        index = row*width*dims + col;
        b = data[index]&0xff;
        g = data[index+1]&0xff;
        r = data[index+2]&0xff;

        r = 255 - r;
        g = 255 - g;
        b = 255 - b;

        data[index] = (byte)b;
        data[index+1] = (byte)g;
        data[index+2] = (byte)r;
    }
}
image.put(0, 0, data);
```

保存Mat对象为图像文件 - 一句话可以搞定
`Imgcodecs.imwrite(filePath, src);           `
#### OpenCV代码运行与测试
- 
调节明暗程度 - 亮度降低 
![这里写图片描述](https://img-blog.csdn.net/20170401142923644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 
调节明暗程度 - 亮度提升 
![这里写图片描述](https://img-blog.csdn.net/20170401142842195?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 
高斯模糊 
![这里写图片描述](https://img-blog.csdn.net/20170401142952165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 
锐化 
![这里写图片描述](https://img-blog.csdn.net/20170401143031662?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 
梯度 
![这里写图片描述](https://img-blog.csdn.net/20170401143058303?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 
灰度化 
![这里写图片描述](https://img-blog.csdn.net/20170401143145085?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上述效果完整Java代码如下：

```java
package com.gloomyfish.opencvdemo;

import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

public class ImageFilters {

    /** - 反色处理 - */
    public Mat inverse(Mat image) {
        int width = image.cols();
        int height = image.rows();
        int dims = image.channels();
        byte[] data = new byte[width*height*dims];
        image.get(0, 0, data);

        int index = 0;
        int r=0, g=0, b=0;
        for(int row=0; row<height; row++) {
            for(int col=0; col<width*dims; col+=dims) {
                index = row*width*dims + col;
                b = data[index]&0xff;
                g = data[index+1]&0xff;
                r = data[index+2]&0xff;

                r = 255 - r;
                g = 255 - g;
                b = 255 - b;

                data[index] = (byte)b;
                data[index+1] = (byte)g;
                data[index+2] = (byte)r;
            }
        }

        image.put(0, 0, data);
        return image;
    }

    public Mat brightness(Mat image) {
        // 亮度提升
        Mat dst = new Mat();
        Mat black = Mat.zeros(image.size(), image.type());
        Core.addWeighted(image, 1.2, black, 0.5, 0, dst);
        return dst;
    }

    public Mat darkness(Mat image) {
        // 亮度降低
        Mat dst = new Mat();
        Mat black = Mat.zeros(image.size(), image.type());
        Core.addWeighted(image, 0.5, black, 0.5, 0, dst);
        return dst;
    }

    public Mat gray(Mat image) {
        // 灰度
        Mat gray = new Mat();
        Imgproc.cvtColor(image, gray, Imgproc.COLOR_BGR2GRAY);
        return gray;
    }

    public Mat sharpen(Mat image) {
        // 锐化
        Mat dst = new Mat();
        float[] sharper = new float[]{0, -1, 0, -1, 5, -1, 0, -1, 0};
        Mat operator = new Mat(3, 3, CvType.CV_32FC1);
        operator.put(0, 0, sharper);
        Imgproc.filter2D(image, dst, -1, operator);
        return dst;
    }

    public Mat blur(Mat image) {
        // 高斯模糊
        Mat dst = new Mat();
        Imgproc.GaussianBlur(image, dst, new Size(15, 15), 0);
        return dst;
    }


    public Mat gradient(Mat image) {
        // 梯度
        Mat grad_x = new Mat();
        Mat grad_y = new Mat();
        Mat abs_grad_x = new Mat();
        Mat abs_grad_y = new Mat();

        Imgproc.Sobel(image, grad_x, CvType.CV_32F, 1, 0);
        Imgproc.Sobel(image, grad_y, CvType.CV_32F, 0, 1);
        Core.convertScaleAbs(grad_x, abs_grad_x);
        Core.convertScaleAbs(grad_y, abs_grad_y);
        grad_x.release();
        grad_y.release();
        Mat gradxy = new Mat();
        Core.addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 10, gradxy);
        return gradxy;
    }
}
```

可以说简单到哭，此外OpenCV For Java支持各种的图像处理包括形态学操作，二值图像分析、图像特征检测与识别、模板匹配、直方图相关功能等等。常见的机器学习算法与图像分析方法。可以说是功能最强大的图像处理SDK与开发平台之一，本人继续发掘分享！

**特别注意**
在调用之前，一定要加上这句话
`System.loadLibrary(Core.NATIVE_LIBRARY_NAME);`
目的是加载OpenCV API相关的DLL支持，没有它是不会正确运行的。以上代码与功能实现是基于JDK8 64位与OpenCV 3.2版本。

**欢迎大家继续关注本博客！**



















