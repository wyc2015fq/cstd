# OpencV使用fitEllipse拟合椭圆后，获取椭圆参数 - sinat_31425585的博客 - CSDN博客
2017年07月14日 12:04:15[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：10346
        使用OpenCV的fitEllipse函数拟合椭圆后，会得到一个RotatedRect类型的返还值，首先介绍一下RotatedRect结构，这个参考的**无左无右**的博客：[点击打开链接](http://blog.csdn.net/yang332233/article/details/70216117?locationNum=4&fps=1)，嫌左右跳麻烦，所以直接贴过来
```cpp
class CV_EXPORTS RotatedRect
{   public:  //构造函数
      RotatedRect();
    RotatedRect(const Point2f& center, const Size2f& size, float angle);
    RotatedRect(const CvBox2D& box);
    void points(Point2f pts[]) const;    //!返回矩形的4个顶点
   	Rect boundingRect() const;  //返回包含旋转矩形的最小矩形
   operator CvBox2D() const;  //!转换到旧式的cvbox2d结构
   Point2f center; //矩形的质心
   Size2f size;    //矩形的边长
    float angle;    //旋转角度，当角度为0、90、180、270等时，矩形就成了一个直立的矩形
};
```
     需要注意的是angle对应角度是角度,如果使用三角函数sin,cos,tan之类的需要先转换成弧度,boundingRect对应水平方向外接矩形而不是椭圆的外接矩形,如果需要获取椭圆的两个半轴长度可以通过size.width和size.height得到,其余的参数看一下上面代码对应注释.
     下面就是怎么通过一个RotatedRect来获取椭圆参数,具体原理可以参考**百度文库**的一个文档:[点击打开链接](https://wenku.baidu.com/view/210f671de87101f69e319560.html),直接截图看一下:
![](https://img-blog.csdn.net/20170714120652901)
因为这里默认的是顺时针旋转为正,而图像里面默认是逆时针旋转为正,只需要把B乘以一个负号就行,感兴趣的同学自己推导一下.
好了，到了上代码的时间了，代码如下：
```cpp
#include <opencv2//opencv.hpp>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;
struct EllipsePara
{
	Point2f c;
	float A;
	float B;
	float C;
	float F;
};
```
```cpp
void getEllipsePara(RotatedRect & ellipsemege, EllipsePara& EP_t)
{
	
	float theta = ellipsemege.angle * CV_PI / 180.0 ;
	float a = ellipsemege.size.width / 2.0;
	float b = ellipsemege.size.height / 2.0;
	EP_t.c.x = ellipsemege.center.x;
	EP_t.c.y = ellipsemege.center.y;
	EP_t.A = a * a * sin(theta) * sin(theta) + b * b * cos(theta) * cos(theta);
	EP_t.B = (-2.0) * (a * a - b * b) * sin(theta) * cos(theta);
	EP_t.C = a * a * cos(theta) * cos(theta) + b * b * sin(theta) * sin(theta);
	EP_t.F = (-1.0) * a * a * b * b;
	//cout << "theta: " << theta << " x: " << EP.c.x << " y: " << EP.c.y << " C: " << EP.C << " F: " << EP.F << endl;
}
void main()
{
	//RotatedRect(const Point2f& center, const Size2f& size, float angle);  
	RotatedRect Rec_t = RotatedRect(Point2f(0,0), Size2f(4,1), 90);
	cout << Rec_t.size.width << " " << Rec_t.size.height << endl;
	cout << Rec_t.boundingRect().width <<  " " << Rec_t.boundingRect().height <<  endl;
	EllipsePara EP_t;
	getEllipsePara(Rec_t, EP_t);
	cout << EP_t.A << " " << EP_t.B << " " << EP_t.C << " " << EP_t.F << endl;
	
	
}
```
得到的椭圆方程为：
![](https://img-blog.csdn.net/20170714120708560)
打完收工！！！
