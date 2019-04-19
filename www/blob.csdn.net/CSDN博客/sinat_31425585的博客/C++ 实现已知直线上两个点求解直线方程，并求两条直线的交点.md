# C++ 实现已知直线上两个点求解直线方程，并求两条直线的交点 - sinat_31425585的博客 - CSDN博客
2017年07月11日 13:52:36[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：5792
已知点A（x1，y1）、B（x2，y2）为直线L1上两个点，点C（x3，y3）、D（x4，y4）为直线L2上两个点，求解两个直线的方程以及交点坐标，直接贴代码
```cpp
#include <iostream>
using namespace std;
//定义Point2f结构体  
struct Point2f
{
	float x;
	float y;
};
// 定义直线参数结构体  
struct LinePara
{
	float k;
	float b;
};
// 获取直线参数  
void getLinePara(float& x1, float& y1, float& x2, float& y2, LinePara & LP)
{
	double m = 0;
	// 计算分子  
	m = x2 - x1;
	if (0 == m)
	{
		LP.k = 10000.0;
		LP.b = y1 - LP.k * x1;
	}
	else
	{
		LP.k = (y2 - y1) / (x2 - x1);
		LP.b = y1 - LP.k * x1;
	}
}
// 获取交点  
bool getCross(float & x1, float &y1, float & x2, float & y2, float & x3, float &y3, float & x4, float & y4,  Point2f & pt){
	LinePara para1, para2;
	getLinePara(x1, y1, x2, y2, para1);
	getLinePara(x3, y3, x4, y4, para2);
	// 判断是否平行  
	if (abs(para1.k - para2.k) > 0.5)
	{
		pt.x = (para2.b - para1.b) / (para1.k - para2.k);
		pt.y = para1.k * pt.x + para1.b;
		return true;
	}
	else
	{
		return false;
	}
}
void main()
{
	Point2f pt1, pt2, pt3, pt4, pt;
	pt1.x = 1.0;
	pt1.y = 1.0;
	pt2.x = 2.0;
	pt2.y = 2.0;
	pt3.x = 0.0;
	pt3.y = 2.0;
	pt4.x = 2.0;
	pt4.y = 0.0;
	getCross(pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y, pt4.x, pt4.y, pt);
	cout << pt.x << " , " << pt.y << endl;
}
```
直接来个完整版的，ctrl+F5，运行效果如下，打完收工！
在这里感谢大佬给的建议，将int m改成double m，谢谢~
