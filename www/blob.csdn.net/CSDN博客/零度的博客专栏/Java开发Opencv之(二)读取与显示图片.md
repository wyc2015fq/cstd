# Java开发Opencv之(二)读取与显示图片 - 零度的博客专栏 - CSDN博客
2017年09月27日 15:54:45[零度anngle](https://me.csdn.net/zmx729618)阅读数：3601
上一篇介绍了Java版本的OpenCV环境配置以及第一个示例程序，在第一个示例程序中，只是使用了控制台输出的方式，打印图像mat对象的一些信息，没有使用GUI形式展示出来。而且程序的结构以及运行方式等也没有做详细的介绍。这篇文章就这些问题展开详细的说明，为了有直观的认识，先把这篇文章要实现的效果展现出来，如下图，换了一张大图来展示。
![](http://images2015.cnblogs.com/blog/620858/201603/620858-20160328225955441-188476176.png)
　　具体调用的方法如下就两行，new一个对象然后调用imshow()方法：
```
public static void main(String[] args) {
        Mat mat = Imgcodecs.imread("E:/test.png");
        ImageViewer imageViewer = new ImageViewer(mat, "第一幅图片");
        imageViewer.imshow();
    }
```
　　以下是显示该图像的代码，是不是很简单。过程就是将OpenCV中的mat对象中的像素数据转成Java中的Image对象，然后使用Java的界面方法新建一个GUI，将这个Image对象显示出来。
```java
package com.zmx.opencvtest;
import org.opencv.core.Mat;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
public class ImageViewer {
    private JLabel imageView;
    private Mat image;
    private String windowName;
    /**
     * 如果使用junit测试时调用该方法，图像会一闪而过，可通过sleep()等方式暂时显示
     * @param
     */
    public ImageViewer(Mat image) {
        this.image = image;
    }
    /**
     * @param image      要显示的mat
     * @param windowName 窗口标题
     */
    public ImageViewer(Mat image, String windowName) {
        this.image = image;
        this.windowName = windowName;
    }
    /**
     * 图片显示
     */
    public void imshow() {
        setSystemLookAndFeel();
        Image loadedImage = toBufferedImage(image);
        JFrame frame = createJFrame(windowName, image.width(), image.height());
        imageView.setIcon(new ImageIcon(loadedImage));
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);// 用户点击窗口关闭
    }
    private void setSystemLookAndFeel() {
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (UnsupportedLookAndFeelException e) {
            e.printStackTrace();
        }
    }
    private JFrame createJFrame(String windowName, int width, int height) {
        JFrame frame = new JFrame(windowName);
        imageView = new JLabel();
        final JScrollPane imageScrollPane = new JScrollPane(imageView);
        imageScrollPane.setPreferredSize(new Dimension(width, height));
        frame.add(imageScrollPane, BorderLayout.CENTER);
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        return frame;
    }
    private Image toBufferedImage(Mat matrix) {
        int type = BufferedImage.TYPE_BYTE_GRAY;
        if (matrix.channels() > 1) {
            type = BufferedImage.TYPE_3BYTE_BGR;
        }
        int bufferSize = matrix.channels() * matrix.cols() * matrix.rows();
        byte[] buffer = new byte[bufferSize];
        matrix.get(0, 0, buffer); // 获取所有的像素点
        BufferedImage image = new BufferedImage(matrix.cols(), matrix.rows(), type);
        final byte[] targetPixels = ((DataBufferByte) image.getRaster().getDataBuffer()).getData();
        System.arraycopy(buffer, 0, targetPixels, 0, buffer.length);
        return image;
    }
}
```
　　我们再回过头看看整个项目，就目前来说还没用到OpenCV的任何算法，只是用了mat对象作为图像的缓存中转显示，用纯Java方法调用也能达到一样的效果。但是实现的过程却是不同的。我们看读取图像的方法：
```
Mat mat = Imgcodecs.imread("E:/test.png");
```
　　只有一行代码，就将图像读取为Mat对象了，再来看imread()的内部实现代码：
```
1 public class Imgcodecs {
 2 ......
 3     public static Mat imread(String filename)
 4     {
 5         
 6         Mat retVal = new Mat(imread_1(filename));
 7         
 8         return retVal;
 9     }
10 
11 ......
12     private static native long imread_1(String filename);
13 ......
14 }
15 
16 ......
17 
18 public class Mat {
19 
20     public final long nativeObj;
21 
22     public Mat(long addr)
23     {
24         if (addr == 0)
25             throw new java.lang.UnsupportedOperationException("Native object address is NULL");
26         nativeObj = addr;
27     }
28 ......
29 }
```
　　在Imgcodes类中调用了imread()方法，再看该方法内部，new了一个Mat对象，使用了imread_1()方法,却返回的是long给Mat。继续跟踪mat()的构造方法，将long型的addr传给了nativeObj。这么看来这个Mat对象貌似就是个long型的地址似的，实则不然，其实在这里有个最重要的方法，那就是private static native long imread_1(String
 filename); 乍看这个方法跟其他的方法并没有什么不同，但是却不能继续往下跟踪了，无法查看其具体的实现。但跟普通的java方法，多了native这个关键字，那这个关键字又是做什么用的呢？
　　简单来说这个方法就是连通Java代码和C++代码的桥梁，但却是单向的，只能从Java端调用C++程序，当然是编译后的可执行文件。以上面的代码为例，OpenCV官方用C++代码实现了一个imread_1()的方法，参数是文件名(包含路径)，返回值是这个mat对象在内存中的地址，这个方法编译生成.dll文件，所以在程序运行之前需要指定.dll文件的位置（VM选项指定的-Djava.library.path=\opencv\x64）和动态链接库的文件名（System.loadLibrary(Core.NATIVE_LIBRARY_NAME);）。这就是JNI(Java
 Native Interface)，更多关于JNI的知识可详细参考其他资料，这里只是简单介绍Java版本的OpenCV程序的调用过程。
　　到这里大家已经应该明白了，所有的Java版本的OpenCV算法部分的代码都是采用C++写的，然后通过Java的JNI方式来调用的，所以理论上Java版本的程序并不会慢多少。
　　最后介绍一下easy_opencv的项目结构：
![](http://images2015.cnblogs.com/blog/620858/201603/620858-20160330215618566-441610984.jpg)
 　　如上图所示：
- .idea/目录以及easy_opencv.iml是idea项目的配置信息，用户无需关注都是自动生成的，而且在git上我已经屏蔽了这两个本地文件（目录），所以你的电脑上可能跟我的不一致。
- opencv/ 目录下是opencv的动态链接库程序以及jar包。
- src/是我们保存代码的主要目录。java/路径下就是具体的代码。
- resources/ 顾名思义就是资源文件的目录，以后所有程序需要用的图片等资源都将放到该目录下。
- test/是测试目录
- pom.xml就是maven工程的配置文件，一些包的依赖等需要在这里配置。
　　如果对这些有疑问的话，可以上网搜索一下java maven工程，其采用了约定大于配置的方式，方便了应用程序的管理和开发。
