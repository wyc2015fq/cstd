
# java入门教程-11.1Java绘图基础 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:55:04[seven-soft](https://me.csdn.net/softn)阅读数：628


要在平面上显示文字和绘图，首先要确定一个平面坐标系。Java语言约定，显示屏上一个长方形区域为程序绘图区域，坐标原点(0,0）位于整个区域的左上角。一个坐标点（x,y）对应屏幕窗口中的一个像素，是整数。如图12.1所示。窗口大小由超文本文件中的width和height指定。例如，以下超文本确定窗口宽为250像素，高为400像素：
<applet code = h.class width=250 height=400></applet>
注：applet是一种嵌入网页的Java小程序，由于目前极少使用，本教程不再进行讲解，这里仅作示例参考。![](http://www.weixueyuan.net/uploads/allimg/130224/8-130224220615W4.png)
图12.1
raphics类的基本功能
在java.awt包中，类Graphics提供的功能有：建立字体、设定显示颜色、显示图像和文本，绘制和填充各种几何图形。可以从图形对象或使用Component的getGraphics()方法得到Graphics对象。Graphics2D类继承Graphics类，并且增加了许多状态属性，使应用程序可以绘制出更加丰富多彩的图形。
在某个组件中绘图，一般应该为这个组件所属的子类重写paint()方法，在该重写的方法中进行绘图。但要在JComponent子类中进行绘图。例如，继承定义一个文本区子类，要在这样的文本区子对象中绘图，就应给这个文本区子类重写paintComponent()。系统自动为程序提供图形对象，并以参数g传递给paint()方法和paintComponent()方法。

