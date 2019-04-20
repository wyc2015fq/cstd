# 学习GDI+绘图 - u014677855的博客 - CSDN博客
2018年09月04日 17:35:09[xiaobigben](https://me.csdn.net/u014677855)阅读数：2237
个人分类：[C#](https://blog.csdn.net/u014677855/article/category/7899454)
项目至今已经初步完成基本功能，这几天抽空把运用到的知识点总结一下，温故而知新。当前项目中最重要的功能模块是几何参数功能模块，大量运用到绘图的技术。要实现的功能是将读取到的几何参数以曲线的形式在页面中画出波形图。
## 目录
- - [目录](#目录)
- [一、GDI+绘图基础](#一gdi绘图基础)
- [二、GDI+概述](#二gdi概述)
- [三、Graphics类](#三graphics类)- [3.1创建Graphics对象](#31创建graphics对象)
- [3.2 Graphics类的方法成员](#32-graphics类的方法成员)
- [3.3 常用画图对象](#33-常用画图对象)- [3.3.1 Pen类](#331-pen类)
- [3.3.2 Color类](#332-color类)
- [3.3.3 Font类](#333-font类)
- [3.3.4 Brush类](#334-brush类)
- [3.3.5 Rectangle类](#335-rectangle类)
- [3.3.6 Point类](#336-point类)
- [四、绘图实例](#四绘图实例)
## 一、GDI+绘图基础
编写图形程序时需要使用GDI（图形设备接口），从程序设计的角度看，GDI包括两个部分：GDI对象和GDI函数。 
GDI对象定义了GDI函数使用的工具和环境变量，GDI函数对象绘制各种图形。在C#中，进行图形程序编写时用到的都是GDI+版本，是GDI的进一步扩展，它使我们变成更加方便。
## 二、GDI+概述
GDI+是微软提供的新的图形设备接口，通过托管代码的类来展现。GDI+主要提供了三类服务：
- 二维矢量图形
- 图像处理
- 
文字显示 
GDI+比GDI的优越性主要展现在2个方面：
- 
扩展了新功能
- 变成更加简易灵活
## 三、Graphics类
Graphics类封装一个GDI+绘图页面，提供将对象绘制到显示设备的方法，与特定的设备上下文关联。所有的画图方法都被包括在Graphics类中，在绘制任何对象时，我们首先要创建一个Ggraphics实例，这个实例相当于创建了一块画布，有了画布才可以使用各种画图方法绘图。 
 绘图程序的设计一般分为两个步骤： 
 （一）创建Graphics对象； 
 （二）使用创建的Graphics对象的方法绘图、显示文本或处理图像。 
### 3.1创建Graphics对象
- 1、利用Paint事件中的PaintEventArgs在窗体或控件的Paint事件中接收对图像对象的引用。在为控件创建绘制代码时，通常用此方法。
```
private void Form1_Paint(object sender, PaintEventArgs e)
{
     //通过e接收对图形对象的引用
     Graphics g = e.Graphics;//创建了Graphics类的实例
 }
```
- 2、调用控件或窗体的CreatGraphics方法 
调用控件或窗体的CreateGraphics方法获取对象的引用，创建Graphics的实例。如果想在已存在的窗体或控件上画图，通常使用此方法创建实例。
```
//创建绘图图面，在当前窗体上画图。
 Graphics g = this.CreateGraphics();
```
- 3、调用Graphics类的FromImage()方法。在需要更改已存在的图像时，通常用此方法。
```
Image img = Image.FromFile("g1.jpg");
 Graphics g1 = Graphics.FromImage(img);
```
### 3.2 Graphics类的方法成员
创建了Graphics类的对象引用后，就可以利用该对象的成员绘制各种各样的图形，下表列出了Graphics类的常用方法成员。
|方法名称|说明|
|----|----|
|DrawArc|画弧形|
|DrawBezier|画立体的贝尔赛曲线|
|DrawClosedCurve|画闭合曲线|
|DrawCurve|画曲线|
|DrawEllipse|画椭圆|
|DrawImage|画图像|
|DrawLine|画线|
|DrawPie|画饼状图|
|DrawPolygon|画多边形|
|DrawRectangle|画矩形|
|DrawString|绘制文本|
|FillEllipse|填充椭圆|
|FillPath|填充路径|
|FillPie|填充饼图|
|FillRectangle|填充矩形|
|FillRegion|填充区域|
在.NET中，GDI+的所有绘图功能都包括在System.Drawing、System.Drawing.Imaging、System.Drawing.Drawing2D和System.Drawing.Text中。
### 3.3 常用画图对象
在创建了Graphics对象后，就可以用它来画图了，可以画线、填充图形、显示文本等，其中主要用到的画图对象有：
- Color：填充颜色
- Pen：画笔，画线和多边形
- Font：给文字设置样式
- Brush：描述颜色
- Rectangle：在窗体上画矩形
- Point：描述一对有序的（x，y）坐标对
#### 3.3.1 Pen类
Pen类用来绘制指定宽度和样式的直线。使用DashStyle属性绘制几种虚线，可以得到各种填充样式（包括纯色和纹理）来填充Pen绘制的直线。 
Pen常用的属性有以下几个，如下表：
|属性名称|描述|
|----|----|
|Alignment|获得或者设置画笔的对齐方式|
|Brush|获得或者设置画笔的属性|
|Color|获得或设置画笔的颜色|
|Width|获得或设置画笔的宽度|
#### 3.3.2 Color类
封装对颜色的定义，静态方法如下表：
|名称|描述|
|----|----|
|FromArgb|从四个8位分量值创建Color对象|
|FromKnowColor|从指定的预定义颜色创建一个Color对象|
|FromName|从预定义颜色的指定名称创建一个Color对象|
在图像处理中一般需要获取或设置像素的颜色值，获取一幅图像的某个像素颜色值的具体步骤如下： 
（1）定义Bitmap 
Bitmap myBitmap = new Bitmap(“c:\MyImages\TestImage.bmp”); 
（2）定义一个颜色变量把在指定位置所取得的像素值存入颜色变量中 
Color c = new Color(); 
c = myBitmap.GetPixel(10,10);//获取此Bitmap中指定像素的颜色。 
（3）将颜色值分解出单色分量值 
        int r,g,b;  
        r= c.R;  
        g=c.G;  
        b=c.B;
#### 3.3.3 Font类
Font类定义特定文本格式，包括字体、字号和字形属性。Font类的常用构造函数是`public Font(string字体名,float字号，FontStyle字形)`，其中字号和字体为可选项和`public Font(string字体名,float 字号)`，其中“字体名”为Font的FontFamily的字符串表示形式。下面是定义一个Font对象的例子代码：
```
FontFamily fontFamily = new FontFamily("Arial");  
Font font = new Font(fontFamily,16,FontStyle.Regular,GraphicsUnit.Pixel);
```
#### 3.3.4 Brush类
Brush类是一个抽象的基类，因此它不能被实例化，我们总是用它的派生类进行实例化一个画刷对象，当我们对图形内部进行填充操作时就会用到画刷。
#### 3.3.5 Rectangle类
存储一组整数，共四个，表示一个矩形的位置和大小。矩形结构通常用来在窗体上画矩形，除了利用它的构造函数构造矩形对象外，还可以使用Rectangle结构的属性成员，其属性成员如下表所示：
|名称|说明|
|----|----|
|Bottom|底端坐标|
|Height|矩形高|
|IsEmpty|测试矩形宽和高是否为0|
|Left|矩形左边坐标|
|Location|矩形的位置|
|Right|矩形右边坐标|
|Size|矩形尺寸|
|Top|矩形顶端坐标|
|Width|矩形宽|
|X|矩形左上角顶点X坐标|
|Y|矩形左上角顶点Y坐标|
Retangle结构的构造函数有以下两个： 
1、用指定的位置和大小初始化Rectangle类的新实例。  
public Retangle(Point,Size);//Size结构存储一个有序整数对，通常为矩形的宽度和高度。 
2、使用矩形4个点的坐标初始化Rectangle类的新实例。 
public Rectangle(int,int,int,int);
#### 3.3.6 Point类
用指定坐标初始化Point类的新实例。这个结构很像C++中的Point结构，它描述了一对有序的x,y两个坐标值，其构造函数为：public Point(int x,int y)；其中x为该点的水平位置；y为该点的水垂直位置。 
下面是构造Point对象的例子代码： 
Point pt1=new Point(30,30);  
Point pt2=new Point(110,100);
## 四、绘图实例
前面介绍了GDI+的基础知识，我们知道了用GDI绘图的步骤是首先创建一个Graphics对象，然后用对象进行绘制图像 。 
现在我们通过下面的实例来复习一下GDI+相关的知识。我们再Winform窗体中来绘制所需的图像。 
准备工作： 
1、首先准备一个画板； 
2、准备只笔 
3、开始画画
```
private void button2_Click(object sender, EventArgs e)
{
    //在button控件上创建绘图对象
    Graphics g = button1.CreateGraphics();
    //创建画笔，颜色为红色
    Pen p = new Pen(Color.FromArgb(255, 0, 0));
    //创建画刷
    Brush brush = new SolidBrush(Color.FromArgb(0, 255, 0));
    //画椭圆到按钮控件上
    g.DrawEllipse(p, 50, 50, 100, 100);
    Graphics g1 = pictureBox1.CreateGraphics();
    g1.DrawEllipse(p, 50, 50, 100, 100);
    g1.FillEllipse(brush, 50, 50, 100, 100);
}
```
运行结果： 
![这里写图片描述](https://img-blog.csdn.net/20180904173409702?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
上面的实例说明了在窗体或者控件上画画的基本步骤。
