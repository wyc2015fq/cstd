
# java入门教程-11.7Java图像缓冲技术 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 09:00:26[seven-soft](https://me.csdn.net/softn)阅读数：257个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



当图像信息量较大，采用以上直接显示的方法，可能前面一部分显示后，显示后面一部分时，由于后面一部分还未从文件读出，使显示呈斑驳现象。为了提高显示效果，许多应用程序都采用图像缓冲技术，即先把图像完整装入内存，在缓冲区中绘制图像或图形，然后将缓冲区中绘制好的图像或图形一次性输出在屏幕上。缓冲技术不仅可以解决闪烁问题，并且由于在计算机内存中创建图像，程序可以对图像进行像素级处理，完成复杂的图像变换后再显示。
【例 12-6】小应用程序程序演示图像缓冲显示技术。程序运行时，当鼠标在图像区域内按下时，图像会出现边框，托动鼠标时，图像也随之移动。抬起鼠标后，边框消失。程序将两种状态的图像先放入两个缓冲区，当鼠标拖动时，不断地在新的位置重绘鼠标按下样式的图像鼠标抬起时，重绘鼠标抬起样式的图像（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/12-6.txt)）。
程序要创建缓冲区图像，需要引入java.awt.image包中的BufferedImage类。要创建一个缓冲区图，可以调用createImage()方法，该方法返回一个Image对象，然后再将它转换成一个BufferedImage对象。例如，代码：
BufferedImage bimage = (BufferedImage)this.createImage(this.getWidth(),this.getHeight());
也可利用以下构造方法来建立。
BufferedImage(int width,int heigh, int imageType);
其中参数 imageType是图像类型。
使用缓冲区显示图像，需先在缓冲区中准备好图像，再将缓冲区中的图像显示在界面上。显示图像需要图形对象Graphics,可以通过以下方法建立：
Graphics2D g2d = bimge.createGraphics();

