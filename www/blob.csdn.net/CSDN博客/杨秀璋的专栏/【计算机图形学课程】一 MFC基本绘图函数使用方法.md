# 【计算机图形学课程】一.MFC基本绘图函数使用方法 - 杨秀璋的专栏 - CSDN博客





2016年11月16日 04:45:51[Eastmount](https://me.csdn.net/Eastmount)阅读数：26042标签：[MFC																[绘制图形																[计算机图形学																[LineTo																[CDC](https://so.csdn.net/so/search/s.do?q=CDC&t=blog)
个人分类：[MFC图像知识																[MFC基础知识](https://blog.csdn.net/Eastmount/article/category/1374312)](https://blog.csdn.net/Eastmount/article/category/1858073)

所属专栏：[MFC应用及图像处理](https://blog.csdn.net/column/details/eastmount-mfc.html)





        这是最近我《计算机图形学》课程实践编程课介绍的相关知识，主要是想通过MFC C++绘图，让学生体会下图形学相关的编程及简单的图形绘制，同时非常佩服学生的想象力，他们做得真的不错。希望这篇基础文章对你有所帮助吧！尤其是有这门课程的学生或编程爱好者，如果文章存在错误或不足之处，还请海涵。
        参考书籍：孔令德·《计算机图形学基础教程（Visual C++版）》
        学生绘制的图形还是非常有创新的，表示很满意，哈哈哈~


![](https://img-blog.csdn.net/20161116041708059)![](https://img-blog.csdn.net/20161116043259812)






## 一. MFC绘图基础知识 CDC类

        PS：这部分主要引入孔令德老师的知识，这篇文章以后面的编程为主。
        VC++具有强大的绘图功能，虽然基于对话框的应用我推荐大家使用C# Winform程序，但是计算机图形和图像的基础知识，还是强烈推荐使用VC++ MFC实现。这有助于让你深入的理解图形变换、图像处理等知识。
        在Windows平台下，GDI（Graphics Device Interface）图形设备接口被抽象为上下文CDC类（Device Context，DC）。Windows平台直接接收图形数据信息的不是显示器和打印机等硬件设备，而是CDC对象。MFC中，CDC类定义设备上下文对象的基类，封装了所需的成员函数，调用CDC类的成员函数，绘制和打印图形及文字。

        CDC类派生出CClientDC类、CMetaFileDC类、CPaintDC类和CWindowDC类，请读者自行学习，同时推荐阅读原书。
        MFC常用CPoint、CRect、CSize等数据类型。
        (1) CPoint类：存放点坐标(x，y)；
        (2) CRect类：存放矩形左上顶点和右下角顶点的坐标(top、left、right、bottom)，其中(top，left)为矩形的左上角点，(right，bottom)为矩形的右下角点；
        (3) CSzie类：存放矩形的宽度和高度的坐标(cx，cy)，其中cx为矩形的宽度，cy为矩形的高度。



![](https://img-blog.csdn.net/20161116004712724)

        MFC绘图工具类包括CGdiObject、CBitmap、CBrush、CFont、CPallette、CPen和CRgn等。常用的包括：
        (1) CBitmap：封装了一个GDI位图，提供位图操作的接口；
        (2) CFont：封装了GDI字体，可以选作设备上下文中的当前字体；
        (3) CBrush：封装了GDI画刷，选作设备上下文的当前画刷，画刷用于填充图形内部；
        (4) CPen：封装了GDI画笔，选作设备上下文的当前画笔，画笔用于绘制图形边界线；
        (5) CPallette：封装了GDI调色板，提供应用程序和显示器之间的颜色接口；
        (6) CGdiObject：GDI绘图工具的基类，一般不能直接使用。






## 二. MFC单文档编程介绍

        首先创建MFC项目，选择"MFC AppWizard"工程，命名为"test01"。



![](https://img-blog.csdn.net/20161116022257614)




        然后选择"单文档"格式，其中"基于对话框"是Dialog对话框工程。




![](https://img-blog.csdn.net/20161116022406461)

        创建完工程，在工作区间主要包括：ClassView（类视图）、ResourceView（资源视图）和FileView（文件视图）。其中类视图主要包含各个类，文件视图包括源文件.cpp和头文件.h。


![](https://img-blog.csdn.net/20161116022547256)![](https://img-blog.csdn.net/20161116022519149)

        打开资源ResourceView视图如下所示：


![](https://img-blog.csdn.net/20161116022806856)

       MFC写代码通常在xxxView.cpp文件下，在"test01View.cpp"中找到OnDraw()函数，用于绘图：

```cpp
/////////////////////////////////////////////////////////////////////////////
// CTest01View drawing

void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}
```
       下面将详细介绍MFC基础绘图函数。






## 三. MFC绘图函数及扩展

### 1.MFC绘图函数

**(1)绘制直线        CDC::MoveTo(int x, int y)**
        将画笔移动到当前位置，即坐标(x, y)处，并没有画线。
**CDC::LineTo(int x, int y)**
        画笔从当前位置绘制一条子线到(x, y)点，但不包含(x, y)点。


```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDC->MoveTo(100,150);
	pDC->LineTo(300,400);

	// TODO: add draw code for native data here
}
```
        绘制图形如下所示，坐标(100, 150)表示距离左边100，距离顶部150。


![](https://img-blog.csdn.net/20161116025721065)

        注意：绘制图形主要调用CDC* pDC方法实现，MFC可以补充提示函数。


![](https://img-blog.csdn.net/20161116031458701)


**(2) 设置画笔**
        通常可以设置绘制图形的颜色及线条属性，函数为：
**CPen::CreatePen(int nPenStyle, int nWidth, COLORREF color)**
       其中第一个参数为画笔的风格，实现、虚线等，第二个参数为画笔粗细，第三个参数使画笔的颜色，采用RGB(255, 255, 255)赋值。


![](https://img-blog.csdn.net/20161116030056055)

**CPen::SelectObject(Cpen *pen)**
        调用CPen选中画笔对象指针，即将画笔指向新画笔，同时指向指针。

```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//绘制直线
	pDC->MoveTo(100,150);
	pDC->LineTo(300,400);

	//定义画笔绘制直线
	CPen pen(PS_DASH, 4, RGB(255,0,0)); //虚线 粗4 红色
	pDC->SelectObject(&pen);
	pDC->MoveTo(100,150);
	pDC->LineTo(400,300);

	//方法二 CreatePen定义画笔
	CPen pen2;   
	pen2.CreatePen(PS_DASHDOTDOT, 1, RGB(0,255,0)); //双点画线 粗2 绿色
	pDC->SelectObject(&pen2);
	pDC->MoveTo(100,150);
	pDC->LineTo(500,200);

}
```
        运行结果如下图所示，注意定义画笔后需要选择画笔SelectObject()，才能使用。


![](https://img-blog.csdn.net/20161116031028069)


**        (3) 绘制矩形        CDC::Rectangle(int x1, int y1, int x2, int y2)**
        参数x1、y1表示矩形左上角坐标，参数x2、y2表示矩形右下角坐标。

```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//定义画笔绘制矩形
	CPen pen(PS_DASH, 2, RGB(0,0,255)); //虚线 粗2 蓝色
	pDC->SelectObject(&pen);

	//定义坐标点
	CPoint point1(100,150);
	CPoint point2(400,300);
	
	//绘制矩形
	pDC->Rectangle(point1.x, point1.y, point2.x, point2.y);

}
```
        运行结果如下图所示，同时定义点是CPoint，可以调用point.x和point.y获取坐标。


![](https://img-blog.csdn.net/20161116031651732)

**        (4) 设置画刷填颜色        CBrush::CreateSolidBrush(COLORREF crColor) **
        参数为画刷颜色，主要用于填充图形。
**        CBrush::SelectObject(CBrush* pBrush)**
        选择画刷，填充颜色，参数pBrush为选中CBrush对象的指针。
**        CGdiObject::DelectObject()**
        把已成自由状态的画刷从系统内存中清除，此函数同删除画笔函数。

```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//定义画笔绘制矩形
	CPen pen(PS_DASH, 2, RGB(0,0,255)); //虚线 粗2 蓝色
	pDC->SelectObject(&pen);
	CBrush bush;
	bush.CreateSolidBrush(RGB(255,0,0));
	pDC->SelectObject(bush);

	//定义坐标点
	CPoint point1(100,150);
	CPoint point2(400,300);
	
	//绘制矩形
	pDC->Rectangle(point1.x, point1.y, point2.x, point2.y);

}
```
        选择画刷填充如下图所示：


![](https://img-blog.csdn.net/20161116032302819)


**(5) 清除画笔及画刷**
        真实操作中，通常会在画笔和画刷使用完毕时，把已成为自由状态的画笔和画刷从系统内存中删除。

```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//定义画笔绘制矩形
	CPen MyPen, *OldPen; 
	MyPen.CreatePen(PS_DASH, 2, RGB(0,0,255)); //虚线 粗2 蓝色
	OldPen = pDC->SelectObject(&MyPen);        //旧画笔赋值

	//画刷
	CBrush MyBrush, *OldBrush;
	MyBrush.CreateSolidBrush(RGB(255,0,0));
	OldBrush = pDC->SelectObject(&MyBrush);

	//定义坐标点
	CPoint point1(100,150);
	CPoint point2(400,300);
	
	//绘制矩形
	pDC->Rectangle(point1.x, point1.y, point2.x, point2.y);

	//清除
	pDC->SelectObject(OldPen);
	MyPen.DeleteObject();
	pDC->SelectObject(OldBrush);
	MyBrush.DeleteObject();

}
```

**        (6) 绘制椭圆函数        CDC::Ellipse(int x1, int y1, int x2, int y2)**
        参数x1、y1是绘制椭圆外接矩形左上角的坐标，x2、y2是外接矩形的右下角坐标。当绘制的外接矩形长和宽相同，即绘制的是圆。

```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//定义画笔
	CPen MyPen, *OldPen; 
	MyPen.CreatePen(PS_DASH, 2, RGB(0,0,255)); //虚线 粗2 蓝色
	OldPen = pDC->SelectObject(&MyPen);        //旧画笔赋值

	//画刷
	CBrush MyBrush, *OldBrush;
	MyBrush.CreateSolidBrush(RGB(255,0,0));
	OldBrush = pDC->SelectObject(&MyBrush);

	//定义坐标点
	CPoint point1(100,150);
	CPoint point2(400,300);
	
	//绘制椭圆
	pDC->Ellipse(point1.x, point1.y, point2.x, point2.y);

	//绘制圆
	pDC->Ellipse(0, 0, 100, 100);

	//清除
	pDC->SelectObject(OldPen);
	MyPen.DeleteObject();
	pDC->SelectObject(OldBrush);
	MyBrush.DeleteObject();

}
```
        输出如下图所示：


![](https://img-blog.csdn.net/20161116033533521)

        注意：还有些方法，包括绘制圆弧、绘制多边形，方法类似，只是注意下参数即可。

**        (7) 绘制文字        CDC::TextOut(int x, int y, const CString & str)**
        参数x、y是文本的起点坐标，参数str是CString对象，文字内容。同时可以结合Format格式控制字符串输出变量。

```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//定义画笔
	CPen MyPen, *OldPen; 
	MyPen.CreatePen(PS_DASH, 2, RGB(0,0,255)); //虚线 粗2 蓝色
	OldPen = pDC->SelectObject(&MyPen);        //旧画笔赋值

	//画刷
	CBrush MyBrush, *OldBrush;
	MyBrush.CreateSolidBrush(RGB(255,0,0));
	OldBrush = pDC->SelectObject(&MyBrush);

	//定义坐标点
	CPoint point1(100,150);
	CPoint point2(400,300);
	
	//绘制椭圆
	pDC->Ellipse(point1.x, point1.y, point2.x, point2.y);
	pDC->TextOut(405,305,_T("绘制椭圆"));

	//绘制圆
	pDC->Ellipse(0, 0, 100, 100);
	//使用Format写文字
	CString str1 = "绘制圆 半径=";
	int r = 50;
	CString data;
	data.Format("%s %d", str1,r);
	pDC->TextOut(105,105,data);

	//清除
	pDC->SelectObject(OldPen);
	MyPen.DeleteObject();
	pDC->SelectObject(OldBrush);
	MyBrush.DeleteObject();

}
```
        输出如下图所示：


![](https://img-blog.csdn.net/20161116034535871)





### 2.扩展知识

        第一个扩展是，绘制直线过程中，如果围绕一个圆心进行循环绘制，可以得到很好看的圆形直线。


```cpp
#include <math.h>

void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//定义画笔并选择
	CPen pen(PS_SOLID,4,RGB(120,32,240));
	pDC->SelectObject(&pen);

	//定义点绘制一条竖直直线
	CPoint p0(300,300);
	CPoint p1(300,550);
	pDC->MoveTo(p0);
	pDC->LineTo(p1);

	//半径和PI
	int R = 250;
	float pi = 3.14f;

	//定义150个点 循环按照圆形绘制
	CPoint p[100];
	for(int i=0;i<100;i++)
	{
		p[i].x = int(p0.x+R*sin((pi+i+1)/20));  //x坐标 sin涉及数学知识
		p[i].y = int(p0.y+R*cos((pi+i+1)/20));  //y坐标

		//先移动到圆形p0(300,300) 再绘制直线
		pDC->MoveTo(p0);
		pDC->LineTo(p[i]);
	}

}
```
输出如下图所示：


![](https://img-blog.csdn.net/20161116040101535)

        该内容就是常见的图形旋转知识，核心内容：计算新坐标，通过sin和cos数学知识，同时圆形不变，每次循环先MoveTo(p0)即可。
        第二段代码涉及图形平移，就是绘制矩形图形平移操作。

```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//定义画笔并选择
	CPen pen(PS_SOLID,4,RGB(120,32,240));
	pDC->SelectObject(&pen);

	CBrush brush(RGB(250,12,30));
	pDC->SelectObject(&brush);

	//循环绘制矩形
	int x1=100,y1=100,x2=300,y2=400;
	for(int j=0; j<100; j=j+3)
	{
		pDC->Rectangle(x1+j, y1+j, x2+j, y2+j);
	}

}
```
        输出如下所示：

![](https://img-blog.csdn.net/20161116040456325)







## 四. 学生成果及创新

        下面的代码是做得比较好的同学的，感觉还是不错的。
        夏KH同学：


```cpp
#include <math.h>

void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	int d,k,x1,x2,y1,y2;
	float pi,a,e;
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(155,0,0));
	CPen *pOldPen = pDC->SelectObject(&pen);
	pi = 3.1415926f;
	d = 80;
	for (a = 0; a<=2 * pi; a+= pi/360)
	{
		e = d * (1+0.25*sin(4*a));
		e = e * (1 + sin(8*a));
		x1 = int(320+e*cos(a));
		x2 = int(320+e*cos(a + pi/8));
		y1 = int(200+e*sin(a));
		y2 = int(200+e*sin(a + pi/8));
		pDC->MoveTo(x1,y1);
		pDC->LineTo(x2,y2);
	}

}
```
        输出如下所示：

![](https://img-blog.csdn.net/20161116041708059)

        李N同学：

```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	//定义画笔
	CPen pen(PS_SOLID,2,RGB(0,255,0));
	pDC->SelectObject(&pen);

	CPoint p0(400,200);
	int R=200;
	CPoint p1(400,600);

	pDC->MoveTo(p0);
	pDC->LineTo(p1);

	float pi=3.14f;

	CPoint p[126];
	for(int i=0;i<126;i++)
	{
		p[i].x=int(p0.x + R*sin((pi+i+1)/20));
		p[i].y=int(p0.y + R*cos((pi+i+1)/20));

		pDC->MoveTo(p0);
		pDC->LineTo(p[i]);
	}

	CBrush bush(RGB(255,255,0));
	pDC->SelectObject(&bush);

	int x1=700,y1=100,x2=900,y2=400;
	for(int j=0;j<50;j++)
	{
		pDC->Rectangle(x1+j,y1+j, x2+j, y2+j);
	}
}
```
        输出如下所示：

![](https://img-blog.csdn.net/20161116041511837)


        杨J同学：

```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CPen pen(PS_SOLID,1,RGB(255,0,0));
	pDC->SelectObject(&pen);
   
	CPoint p0(400,300);
	int R=200;
	CPoint p1;
	p1.x=p0.x;
	p1.y=p0.y+R;
 
	pDC->MoveTo(p0);
	pDC->LineTo(p1);

	CPoint p2;
	float pi=3.14f;
	p2.x=int(p0.x+R*sin(pi/20));
	p2.y=int(p0.y+R*cos(pi/20));
	pDC->MoveTo(p0);
	pDC->LineTo(p2);

	CPoint p[1000];
	for(int i=0;i<1000;i++){
		p[i].x=int(p0.x+R*sin((pi+i+1)/20));
		p[i].y=int(p0.y+R*cos((pi+i+1)/20));

		pDC->MoveTo(p0);
		pDC->LineTo(p[i]);
	}

	CBrush bush(RGB(0,255,255));
	pDC->SelectObject(&bush);
	for(int j=0;j<200;j=j+3)
	{
		pDC->Rectangle(600+j,100+j,800+j,400+j);
	}

}
```
        输出如下图所示：

![](https://img-blog.csdn.net/20161116042102220)
        张J同学：

```cpp
#include <math.h>

void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CPen pen(PS_SOLID,1,RGB(255,80,0));
	pDC->SelectObject(&pen);
	CPoint p0(200,200);
	CPoint p1;
	int R=200,P=100;
	p1.x=p0.x;
	p1.y=p0.y+R;
	pDC->MoveTo(p0);
	pDC->LineTo(p1);
	float pi=3.14;
	CPoint p[200];
	for(int i=0,h=1;i<200,i<200;i=i+2,h=h+2)
	{
		p[i].x=int(R*sin((pi+i)/20)+p0.x);
		p[i].y=int(R*cos((pi+i)/20)+p0.y);
        p[h].x=int(P*sin((pi+h)/20)+p0.x);
		p[h].y=int(P*cos((pi+h)/20)+p0.y);
		pDC->MoveTo(p0);
		pDC->LineTo(p[i]);
		pDC->MoveTo(p0);
		pDC->LineTo(p[h]);
	}

	CBrush bush(RGB(255,190,0));
	pDC->SelectObject(&bush);
	int x1=500,y1=100,x2=600,y2=300;
	for(int j=0;j<100;j++)
	{
		pDC->Rectangle(500-j,100+j,600+j,300-j);
	}
    int x3=700,y3=100,x4=800,y4=300;
	for(int k=0;k<100;k++)
	{
		pDC->Ellipse(700-k,100+k,800+k,300-k);
	}
	int x5=900,y5=100,x6=1000,y6=300;
	for(int l=0;l<100;l++)
	{
		pDC->Rectangle(900+l,100+l,1000+l,300+l);
	}

}
```
        输出如下图所示：

![](https://img-blog.csdn.net/20161116042302059)




        郑DD同学：


```cpp
#include <math.h>

void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CPen pen(PS_SOLID,1,RGB(0,255,0,));
	pDC->SelectObject(&pen);
	CPoint p1,p2;
	int R=200;
	p1.x=200;
	p1.y=300;
	p2.x=100;
	p2.y=p1.y+R;
	pDC->MoveTo(p1);
	pDC->MoveTo(p2);

	float pi=3.14f;

	CPoint p[400];

	for(int i=0;i<400;i++)
	{
		p[i].x = int(R*sin((pi+i+1)/3)+p1.x);
		p[i].y = int(R*cos((pi+i+1)/6)+p1.y);
		pDC->MoveTo(p1);
		pDC->LineTo(p[i]);
	}
	pDC->TextOut(190,280,_T("wow")),RGB(255,0,0);

	float ph=3.14f;
    for(int h=0;h<400;h++)
	{
		p[h].x = int(R*sin((ph+h+1)/6)+p1.x);
		p[h].y = int(R*cos((ph+h+1)/3)+p1.y);
		pDC->MoveTo(p1);
		pDC->LineTo(p[i]);
	}

}
```
        输出如下图所示：

![](https://img-blog.csdn.net/20161116042501184)




        赵BL同学：

```cpp
#include <math.h>

void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	//定义画笔
	CPen pen(PS_SOLID,2,RGB(255,32,150));
    pDC->SelectObject(&pen);

	//定义点
	CPoint p0(300,400);
	CPoint p1(300,600);

	pDC->MoveTo(p0);
	pDC->LineTo(p1);

	float pi=3.14f;
	int R=200;

	CPoint p[249];
	for(int i=0; i<249; i++){

		p[i].x = int(p0.x + R*cos((pi+i+1)/20));
		p[i].y = int(p0.y + R*tan((pi+i+1)/20));

		pDC->MoveTo(p0);
		pDC->LineTo(p[i]);
	}


	CBrush bush(RGB(255,166,123));
	pDC->SelectObject(bush);
	int x=600,y=100;
	int x2=900,y2=300;
	pDC->Rectangle(x,y,x2,y2);

	for(int j=0;j<100;j=j+4){
	
		pDC->Rectangle(x+j,y+j,x2+j,y2+j);
	}
}
```
        输出如下图所示：

![](https://img-blog.csdn.net/20161116042753605)

        陈Y同学：

```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CPen pen(PS_SOLID,8,RGB(132,255,255));
	pDC->SelectObject(&pen);

	CPoint p0(220,300);
	int R=200;
	CPoint p1;
	p1.x=p0.x;
	p1.y=p0.y+R;

	pDC->MoveTo(p0);
	pDC->LineTo(p1);

	CPoint p2;
	float pie=3.14f;

	CPoint p[45];
	for(int i=0;i<60;i++) {
	 
		p2.x=int(p0.x+R*sin((pie-i)/10));
		p2.y=int(p0.y+R*tan((pie-i)/10));
	
		pDC->MoveTo(p0);
		 pDC->LineTo(p2);

	}
}
```
        输出如下图所示：


![](https://img-blog.csdn.net/20161116043030905)


        文FB同学代码：

```cpp
#include <math.h>

void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CPen pen(PS_SOLID, 3, RGB(0, 0, 255));
	pDC->SelectObject(&pen);
	CPoint p0(350, 200);
	int r = 200;
	CPoint p1(350, 300 + r);

	pDC->MoveTo(p0);
	pDC->LineTo(p1);

	CPoint p2;
	float PI = 3.14f;

	p2.x = int(p0.x + r*sin(PI / 10));
	p2.y = int(p0.y + r*cos(PI / 10));
	pDC->MoveTo(p0);
	pDC->LineTo(p2);

	CPoint p[1000];
	for (int i = 0; i <300; i++)
	{
		
		p[i].x = int(p0.x + r*sin((PI + i + 1) / 10));
		p[i].y = int(p0.y + r*cos((PI + i + 1) / 10));
		r--;
		pDC->MoveTo(p0);
		pDC->LineTo(p[i]);
	}
}
```
        输出如下图所示：

![](https://img-blog.csdn.net/20161116043259812)


        刘JL同学代码如下：

```cpp
void CTest01View::OnDraw(CDC* pDC)
{
	CTest01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CPen pen(PS_SOLID,2,RGB(255,0,0));
	pDC->SelectObject(&pen);
	CPoint p0(300,200);
	int R=170;
	CPoint p1(350,400);
	pDC->MoveTo(p0);
	pDC->LineTo(p1);
	
	
	float pi=3.14f;
	CPoint p[45];
	for(int i=0;i<45;i++)
	{
		p[i].x=int(p0.x*sin((pi+i)/20));
		p[i].y=int(p0.y*cos((pi+i)/20)+R);
		pDC->MoveTo(p0);
		pDC->LineTo(p[i]);
	}
}
```
        输出如下图所示：

![](https://img-blog.csdn.net/20161116043535110)


        希望文章对你有所帮助，上课内容还需要继续探索，这篇文章主要讲述MFC绘制图形的基础知识，再结合计算机图形学的旋转和平移应用进行扩展。原来课程可以这么上，挺有意思的，同时学生的想象力真实厉害，而且也很优秀。下周专家就来了，真的很忙，但是再忙，写博客的时候自己都是最放松最享受的时候，上课也是一样。加油~
(By:Eastmount 2016-11-16 半夜4点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))](https://so.csdn.net/so/search/s.do?q=LineTo&t=blog)](https://so.csdn.net/so/search/s.do?q=计算机图形学&t=blog)](https://so.csdn.net/so/search/s.do?q=绘制图形&t=blog)](https://so.csdn.net/so/search/s.do?q=MFC&t=blog)




