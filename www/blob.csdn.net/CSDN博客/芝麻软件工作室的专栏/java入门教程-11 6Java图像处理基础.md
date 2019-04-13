
# java入门教程-11.6Java图像处理基础 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:59:39[seven-soft](https://me.csdn.net/softn)阅读数：540


图像是由一组像素构成，用二进制形式保存的图片。java语言支持GIF、JPEG和BMP这3种主要图像文件格式。java语言的图像处理功能被封装在Image类中。
## 图像载入和输出
在java程序中，图像也是对象，所以载入图像时，先要声明Image对象，然后，利用getImage()方法把Image对象与图像文件联系起来。载入图像文件的方法有两个：Image getImage(URL url),url指明图像所在位置和文件名。
Image getImage(URL url,String name),url指明图像所在位置，name是文件名。
例如，以下代码声明Image对象，并用getImage()对象与图像文件联系起来：
Image img = getImage(getCodeBase(),”family.jpg”);
URL(uniform Resource Location 统一资源定位符)对象用于标识资源的名字和地址，在WWW客户机访问Internet网上资源时使用。确定图像位置的方法有两种：绝对位置与相对位置。取相对位置的方法有：URL getCodeBase(),取小应用程序文件所在的位置。
URL getDocumentBase(),取HTML文件所在的位置。
例如，代码：
URL picURLA = new URL(getDocumentBase(),”imageSample1.gif”),
picURLB = new URL(getDocumentBase(),”pictures/imageSample.gif”);
Image imageA = getImage(picURLA),imageB = getImage(picURLB);
获取图像信息(属性)的方法有：getWidth(ImageObserver observer)：取宽度；
getHeight(ImageObserver observer)：取高度。
输出图像的代码写在paint()方法中，有4种显示图像的方法：boolean drawImage(Image img,int x,int y,ImageObserver observer)
boolean drawImage(Image img,int x,int y,Color bgcolor,ImageObserver observer)
boolean drawImage(Image img,int x,int y,int width,int height,ImageObsever observer)
boolean drawImage(Image img,int x,int y,int width,int height,Color bgcolor,ImageObsever observer)
参数img是Image对象，x,y是绘制图像矩形的左上角位置，observer是加载图像时的图像观察器，bgcolor是显示图像用的底色，width和height是显示图像的矩形区域，当这个区域与图像的大小不同时，显示图像就会有缩放处理。
Applet类也实现ImageObserver接口，常用this作为实参。参见以下代码及注释：(1) g.drawImage(image1,0,0,this);//原图显示
(2) g.drawImage(image2,10,10,Color.red,this);//图形加底色显示
注意：如原图的大小与给定的范围不同，系统会自动缩放
(3) g.drawImage(labImag,0,0,this);// 原图显示
(4) g.grawImage(labImag,0,120,100,100,this);//缩放显示
(5) g.grawImage(labImag,0,240,500,100,this);//缩放显示
【例 12-5】小应用程序用init()或start()方法下载(获取)图像，用paint()方法显示得到的图像。
import java.applet.*;import java.awt.*;
public class Example7_5 extends Applet{
Image myImag;
public void start(){
myImag = getImage(getCodeBase(),”myPic.jpg”);
}
public void paint(Graphics g){
g.drawImage(myImg,2,2,this);
}
}
由于在Frame、JFrame和JPanel等类中没有提供getImage()方法，它们载入图像需要使用java.awt.Toolkit中的Toolkit抽象类，该类有载入图像文件的方法：Image.getImage(String name)：按指定的文件名载入图像文件。
Image.getImage(URL url)：统一资源定位符载入图像文件。
这样，各种组件可以用getToolkit()方法得到Toolkit对象，然后在组件的paint()方法中通过Toolkit对象显示图像。以下代码示意这样的用法：
Toolkit tool = getToolkit();
URL url = new URL(http://www.weixueyuan.net/image.gif);
Image img = tool.getImage(url);
组件也可以使用Toolkit提供的静态方法getDefaultToolkit()获得一个缺省的Toolkit对象,并用它加载图像。此时，载入图像的代码常写成这样：
Image img = Toolkit.getDefaultToolkit().getImage(url);

