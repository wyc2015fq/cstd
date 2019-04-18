# OPENCV+JAVA 人脸识别 - z69183787的专栏 - CSDN博客
2017年08月17日 16:37:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9785
[http://opencv.org/releases.html](http://opencv.org/releases.html)
官方下载 安装文件 ，以win7为例，下载opencv-2.4.13.3-vc14.exe
安装后，在build目录下 D:\opencv\build\java，获取opencv-2413.jar，copy至项目目录
同时需要dll文件 与 各 识别xml文件，进行不同特征的识别（人脸，侧脸，眼睛等）
dll目录：D:\opencv\build\java\x64\opencv_java2413.dll
xml目录：D:\opencv\sources\data\haarcascades\haarcascade_frontalface_alt.xml（目录中有各类识别文件）
项目结构：
![](https://img-blog.csdn.net/20170817163521930?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
具体代码：由于需要用到 opencv 的dll文件，故要么放在java library path 中，或放在jre lib 中，windows下可放在System32目录下
也可以在代码中 动态加载，如下：
```java
package opencv;
import com.sun.scenario.effect.ImageData;
import org.opencv.core.*;
import org.opencv.core.Point;
import org.opencv.highgui.Highgui;
import org.opencv.imgproc.Imgproc;
import org.opencv.objdetect.CascadeClassifier;
import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.Vector;
/**
 * Created by Administrator on 2017/8/17.
 */
public class Test {
    static{
        // 导入opencv的库
        String opencvpath = System.getProperty("user.dir") + "\\opencv\\x64\\";
        String libPath = System.getProperty("java.library.path");
        String a = opencvpath + Core.NATIVE_LIBRARY_NAME + ".dll";
        System.load(opencvpath + Core.NATIVE_LIBRARY_NAME + ".dll");
    }
    public static String getCutPath(String filePath){
        String[] splitPath = filePath.split("\\.");
        return splitPath[0]+"Cut"+"."+splitPath[1];
    }
    public static void process(String original,String target) throws Exception {
        String originalCut = getCutPath(original);
        String targetCut = getCutPath(target);
        if(detectFace(original,originalCut) && detectFace(target,targetCut)){
        }
    }
    public static boolean detectFace(String imagePath,String outFile) throws Exception
    {
        System.out.println("\nRunning DetectFaceDemo");
        // 从配置文件lbpcascade_frontalface.xml中创建一个人脸识别器，该文件位于opencv安装目录中
        CascadeClassifier faceDetector = new CascadeClassifier(
                "C:\\Users\\Administrator\\Desktop\\opencv\\haarcascade_frontalface_alt.xml");
        Mat image = Highgui.imread(imagePath);
        // 在图片中检测人脸
        MatOfRect faceDetections = new MatOfRect();
        faceDetector.detectMultiScale(image, faceDetections);
        System.out.println(String.format("Detected %s faces",
                faceDetections.toArray().length));
        Rect[] rects = faceDetections.toArray();
        if(rects != null && rects.length > 1){
            throw new RuntimeException("超过一个脸");
        }
        // 在每一个识别出来的人脸周围画出一个方框
        Rect rect = rects[0];
        Core.rectangle(image, new Point(rect.x-2, rect.y-2), new Point(rect.x
                + rect.width, rect.y + rect.height), new Scalar(0, 255, 0));
        Mat sub = image.submat(rect);
        Mat mat = new Mat();
        Size size = new Size(300, 300);
        Imgproc.resize(sub, mat, size);//将人脸进行截图并保存
        return Highgui.imwrite(outFile, mat);
        // 将结果保存到文件
//        String filename = "C:\\Users\\Administrator\\Desktop\\opencv\\faceDetection.png";
//        System.out.println(String.format("Writing %s", filename));
//        Highgui.imwrite(filename, image);
    }
    public static void setAlpha(String imagePath,String outFile) {
        /**
         * 增加测试项
         * 读取图片，绘制成半透明
         */
        try {
            ImageIcon imageIcon = new ImageIcon(imagePath);
            BufferedImage bufferedImage = new BufferedImage(imageIcon.getIconWidth(),imageIcon.getIconHeight()
                    , BufferedImage.TYPE_4BYTE_ABGR);
            Graphics2D g2D = (Graphics2D) bufferedImage.getGraphics();
            g2D.drawImage(imageIcon.getImage(), 0, 0,
                    imageIcon.getImageObserver());
            //循环每一个像素点，改变像素点的Alpha值
            int alpha = 100;
            for (int j1 = bufferedImage.getMinY(); j1 < bufferedImage.getHeight(); j1++) {
                for (int j2 = bufferedImage.getMinX(); j2 < bufferedImage.getWidth(); j2++) {
                    int rgb = bufferedImage.getRGB(j2, j1);
                    rgb = ( (alpha + 1) << 24) | (rgb & 0x00ffffff);
                    bufferedImage.setRGB(j2, j1, rgb);
                }
            }
            g2D.drawImage(bufferedImage, 0, 0, imageIcon.getImageObserver());
            //生成图片为PNG
            ImageIO.write(bufferedImage, "png",  new File(outFile));
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
    private static void watermark(String a,String b,String outFile, float alpha) throws IOException {
        // 获取底图
                 BufferedImage buffImg = ImageIO.read(new File(a));
                 // 获取层图
                 BufferedImage waterImg = ImageIO.read(new File(b));
                 // 创建Graphics2D对象，用在底图对象上绘图
                 Graphics2D g2d = buffImg.createGraphics();
                 int waterImgWidth = waterImg.getWidth();// 获取层图的宽度
                 int waterImgHeight = waterImg.getHeight();// 获取层图的高度
                 // 在图形和图像中实现混合和透明效果
                 g2d.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_ATOP, alpha));
                 // 绘制
                 g2d.drawImage(waterImg, 0, 0, waterImgWidth, waterImgHeight, null);
                 g2d.dispose();// 释放图形上下文使用的系统资源
        //生成图片为PNG
        ImageIO.write(buffImg, "png",  new File(outFile));
    }
    public static boolean mergeSimple(BufferedImage image1, BufferedImage image2, int posw, int posh, File fileOutput) {
        //合并两个图像
        int w1 = image1.getWidth();
        int h1 = image1.getHeight();
        int w2 = image2.getWidth();
        int h2 = image2.getHeight();
        BufferedImage imageSaved = new BufferedImage(w1, h1, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = imageSaved.createGraphics();
        // 增加下面代码使得背景透明
        g2d.drawImage(image1, null, 0, 0);
        image1 = g2d.getDeviceConfiguration().createCompatibleImage(w1, w2, Transparency.TRANSLUCENT);
        g2d.dispose();
        g2d = image1.createGraphics();
        // 背景透明代码结束
//        for (int i = 0; i < w2; i++) {
//            for (int j = 0; j < h2; j++) {
//                int rgb1 = image1.getRGB(i + posw, j + posh);
//                int rgb2 = image2.getRGB(i, j);
//
//                if (rgb1 != rgb2) {
//                    //rgb2 = rgb1 & rgb2;
//                }
//                imageSaved.setRGB(i + posw, j + posh, rgb2);
//            }
//        }
        boolean b = false;
        try {
            b = ImageIO.write(imageSaved, "png", fileOutput);
        } catch (IOException ie) {
            ie.printStackTrace();
        }
        return b;
    }
    public static void main(String[] args) throws Exception {
        String a,b,c,d;
        a = "C:\\Users\\Administrator\\Desktop\\opencv\\zzl.jpg";
        d = "C:\\Users\\Administrator\\Desktop\\opencv\\cgx.jpg";
        //process(a,d);
        a = "C:\\Users\\Administrator\\Desktop\\opencv\\zzlCut.jpg";
        d = "C:\\Users\\Administrator\\Desktop\\opencv\\cgxCut.jpg";
        CascadeClassifier faceDetector = new CascadeClassifier(
                "C:\\Users\\Administrator\\Desktop\\opencv\\haarcascade_frontalface_alt.xml");
        CascadeClassifier eyeDetector1 = new CascadeClassifier(
                "C:\\Users\\Administrator\\Desktop\\opencv\\haarcascade_eye.xml");
        CascadeClassifier eyeDetector2 = new CascadeClassifier(
                "C:\\Users\\Administrator\\Desktop\\opencv\\haarcascade_eye_tree_eyeglasses.xml");
        Mat image = Highgui.imread("C:\\Users\\Administrator\\Desktop\\opencv\\gakki.jpg");
        // 在图片中检测人脸
        MatOfRect faceDetections = new MatOfRect();
        //eyeDetector2.detectMultiScale(image, faceDetections);
        Vector<Rect> objects;
        eyeDetector1.detectMultiScale(image, faceDetections, 2.0,1,1,new Size(20,20),new Size(20,20));
        Rect[] rects = faceDetections.toArray();
        Rect eyea,eyeb;
        eyea = rects[0];eyeb = rects[1];
         System.out.println("a-中心坐标 " + eyea.x + " and " + eyea.y);
        System.out.println("b-中心坐标 " + eyeb.x + " and " + eyeb.y);
        //获取两个人眼的角度
        double dy=(eyeb.y-eyea.y);
        double dx=(eyeb.x-eyea.x);
        double len=Math.sqrt(dx*dx+dy*dy);
        System.out.println("dx is "+dx);
        System.out.println("dy is "+dy);
        System.out.println("len is "+len);
        double angle=Math.atan2(Math.abs(dy),Math.abs(dx))*180.0/Math.PI;
        System.out.println("angle is "+angle);
        for(Rect rect:faceDetections.toArray()) {
            Core.rectangle(image, new Point(rect.x, rect.y), new Point(rect.x
                    + rect.width, rect.y + rect.height), new Scalar(0, 255, 0));
        }
        String filename = "C:\\Users\\Administrator\\Desktop\\opencv\\ouput.png";
        System.out.println(String.format("Writing %s", filename));
        Highgui.imwrite(filename, image);
//        watermark(a,d,"C:\\Users\\Administrator\\Desktop\\opencv\\zzlTm2.jpg",0.7f);
//
//        // 读取图像，不改变图像的原始信息
//        Mat image1 = Highgui.imread(a);
//        Mat image2 = Highgui.imread(d);
//        Mat mat1 = new Mat();Mat mat2 = new Mat();
//        Size size = new Size(300, 300);
//        Imgproc.resize(image1, mat1, size);
//        Imgproc.resize(image2, mat2, size);
//        Mat mat3 = new Mat(size,CvType.CV_64F);
//        //Core.addWeighted(mat1, 0.5, mat2, 1, 0, mat3);
//
//        //Highgui.imwrite("C:\\Users\\Administrator\\Desktop\\opencv\\add.jpg", mat3);
//
//        mergeSimple(ImageIO.read(new File(a)),
//                ImageIO.read(new File(d)),0,0,
//                new File("C:\\Users\\Administrator\\Desktop\\opencv\\add.jpg"));
    }
}
```
最终效果：人脸旁有绿色边框，可以将绿色边框图片截取，生成人脸图
