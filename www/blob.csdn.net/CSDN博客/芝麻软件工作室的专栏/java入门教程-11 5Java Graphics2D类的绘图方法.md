
# java入门教程-11.5Java Graphics2D类的绘图方法 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:58:48[seven-soft](https://me.csdn.net/softn)阅读数：1619


Java语言在Graphics类提供绘制各种基本的几何图形的基础上,扩展Graphics类提供一个Graphics2D类,它拥用更强大的二维图形处理能力,提供、坐标转换、颜色管理以及文字布局等更精确的控制。
## 绘图属性
Graphics2D定义了几种方法，用于添加或改变图形的状态属性。可以通过设定和修改状态属性，指定画笔宽度和画笔的连接方式；设定平移、旋转、缩放或修剪变换图形；以及设定填充图形的颜色和图案等。图形状态属性用特定的对象存储。
1. stroke属性
stroke属性控制线条的宽度、笔形样式、线段连接方式或短划线图案。该属性的设置需要先创建BasicStroke对象，再调用setStroke()方法来设置。创建BasicStroke对象的方法有：BasicStroke(float w)：指定线条宽w。
BasicStroke(float w,int cap, int join)：cap是端点样：CAP_BUTT(无修饰)，CAP_ROUND(半圆形末端)，CAP_SQUARE(方形末端，默认值)。
Join定义两线段交汇处的连接方式：JOIN_BEVEL(无修饰),JOIN_MTTER(尖形末端，默认值),JOIN_ROUND(圆形末端)。

2. paint属性
paint属性控制填充效果。先调用以下方法确定填充效果，理用setPaint()方法设置。GradientPaint(float x1,float y1,Color c1,float x2,flaot y2,Color c2)：从(x1,y1)到(x2,y2)颜色从c1渐变到c2。其中：参数c1,c2决定这个渐变色是从颜色c1渐变到颜色c2。参数x1,y1,x2,y2决定了渐变的强弱，即要求从点(x1,y1)出发到达点(x2,y2)，颜色从c1变成c2。
GradientPaint(float x1,float y1,Color c1,float x2,float y2,Color c2,Boolean cyclic)：如果希望渐变到终点又是起点的颜色，应将cyclic设置为true。
3. transform属性
transform 属性用来实现常用的图形平移、缩放和斜切等变换操作。首先创建AffineTransform对象，然后调用setTransform()方法设置transform属性。最后，用具有指定属性的Graphics2D对象绘制图形。创建AffineTransform对象的方法有：getRotateinstrance(double theta)：旋转theta弧度。
getRotateInstance(double theta,dioble x,double y)：绕旋转中心(x,y)旋转。
getScaleInstance(double sx,double sy)：x和y 方向分别按sx,sy比例变换。
getTranslateInstance(double tx,double ty)：平移变换。
getShearInstance(double shx,double shy)：斜切变换，shx和shy指定斜拉度。
也可以先创建一个没有transform属性的AffineTransform对象，然后用以下方法指定图形平移、旋转、缩放变换属性。transelate(double dx,double dy)：将图形在x轴方向平移dx像素。
scale(double sx,double sy)：图形在x轴方向缩放sx倍，纵向缩放sy倍。
rotate(double arc,double x, double y)：图形以点(x,y)为轴点，旋转arc弧度。
例如，创建AffineTransform对象：
AffineTransform trans = new AffineTransform();
为AffineTransform对象指定绕点旋转变换属性：
Trans.rotate(50.0*3.1415927/180.0,90,80);
接着为Graphics2D 的对象g2d设置具有上述旋转变换功能的“画笔”：
Graphics2D g2d = (Graphics2D)g;g2d.setTranstorm(trans);
最后，以图形对象为参数调用具有变换功能的Graphics2D 对象的draw()方法。例如，设已有一个二次曲线对象curve，以下代码实现用上述旋转功能的g2d对象绘制这条二次曲线：
g2d.draw(curve);
4. clip属性
clip属性用于实现剪裁效果。设置剪裁属性可调用setClip()方法确定剪裁区的Shape。连续多个setClip()得到它们交集的剪裁区。
5. composit属性
composit属性设置图形重叠区域的效果。先用方法AlphaComposite.getInstance(int rule, float alpha)得到AlphaComposite对象，再通过setComposite()方法设置混合效果。Alpha值的范围为0.0f(完全透明)-0.1f(完全不透明)。
## Graphics2D类的绘图方法
Graphics2D类仍然保留Graphics类的绘图方法，同时增加了许多新方法。新方法将几何图形(线段、圆等)作为一个对象来绘制。在java.awt.geom包中声明的一系列类，分别用于创建各种身体图形对象。主要有：
Line2D线段类，RoundRectangle2D圆角矩形类，Ellipse2D椭圆类，Arc2D圆弧类，QuadCurve2D二次曲线类，CubicCurve2D三次曲线类。
要用Graphics2D类的新方法画一个图形。先在重画方法paintComponent()或paint()中，把参数对象g强制转换成Graphics2D对象；然后，用上述图形类提供的静态方法Double()创建该图形的对象；最后，以图形对象为参数调用Graphics2D对象的draw()方法绘制这个图形。例如以下代码用Graphics2D的新方法绘制线段和圆角矩形：
Graphics2D g2d = (Graphics2D)g;//将对象g类型从Graphics转换成Graphics2D
Line2D line = new Line2D.Double(30.0,30.0,340.0,30.0);
g2d.draw(line);
RoundRectangle2D rRect = new RoundRectangle2D.Double(13.0,30.0,100.0,70.0,40.0,20.0);
g2d.draw(rRect);
也可以先用java.awt.geom包提供的Shape对象，并用单精度Float坐标或双精度Double坐标创建Shape对象，然后再用draw()方法绘制。例如，以下代码先创建圆弧对象，然后绘制圆弧：
Shape arc = new Arc2D.Float(30,30,150,150,40,100,Arc2D.OPEN);
g2d.draw(arc)/绘制前面创建的图形对象arc
## Graphics2D的几何图形类
线段
Line2D line = new Line2D.Double(2,3,200,300);//声明并创建线段对象
//起点是(2，3)，终点是(200，300)
矩形
Rectangle2D rect = new Rectangle2D.Double(20,30,80,40);//声明并创建矩形对象，矩形的左上角是(20，30)，宽是300，高是40
圆角矩形
RoundRectangle2D rectRound = new RoundRectangle2D.Double(20,30,130,100,18,15);
//左上角是(20，30)，宽是130，高是100，圆角的长轴是18，短轴是15。
椭圆
Ellipse2D ellipse = new Ellipse2D.Double(20,30,100,50);
//左上角 (20，30)，宽是100，高是50
圆弧
Arc2D arc1 = new Arc2D.Double(8,30,85,60,5,90,Arc2D.OPEN);
//外接矩形的左上角(10，30)，宽85，高60，起始角是5度，终止角是90度
Arc2D arc2 = new Arc2D.Double(20,65,90,70,0,180,Arc2D.CHORD);
Arc2D arc3 = new Arc2D.Double(40,110,50,90,0,270,Arc2D.PIE);
参数Arc2D.OPEN、Arc2D.CHORD、Arc2D.PIE分别表示圆弧是开弧、弓弧和饼弧。
二次曲线
二次曲线用二阶多项式表示：
y(x)=ax2+bx+c
一条二次曲线需要三个点确定：始点、控制点和终点。
QuadCurve2D curve1 = new QuadCurver2D.Double(20,10,90,65,55,115);
QuadCurve2D curve2 = new QuadCurver2D.Double(20,10,15,63,55,115);
QuadCurve2D curve3 = new QuadCurver2D.Double(20,10,54,64,55,115);
方法Double()中的6个参数分别是二次曲线的始点、控制点和终点。以上3条二次曲线的开始点和终点分别相同。
三次曲线
三次曲线用三阶多项式表示：
y(x)=ax3+bx2+cx+d
一条三次曲线需要四个点确定：始点、两个控制点和终点。
CubicCurve2D curve1 = new CubicCurve2D.Double(12,30,50,75,15,15,115,93);
CubicCurve2D curve2 = new CubicCurve2D.Double(12,30,15,70,20,25,35,94);
CubicCurve2D curve3 = new CubicCurve2D.Double(12,30,50,75,20,95,95,95);
方法Double()中的8个参数分别是三次曲线的始点、两个控制点和终点。
一般的方程曲线的绘制过程用一个循环控制。通过循环产生自变量的值，按照方程计算出函数值，再作必要的坐标转换：原点定位的平移变换，图像缩小或放大的缩放变换，得到曲线的图像点，并绘制这个点。以绘制以下曲线方程为例：
Y=sin(x)+cos(x),x
绘制的部分代码可以写成如下：
double x0,y0,x1,y1,x2,y2,scale;
x0=100;y0=80;
scale =20.0;
for(x1=-3.1415926d;x1<=2*3.1415926d;x1+=0.01d){
y1=Math.sin(x1)+Math.cos(x1);
x2=x0+x1*scale;y2=y0+y1*scale;//(x2,y2)是图像点
g.fillOval((int)x2,(int)y2,1,1);//画一个圆点作为图像点
}

