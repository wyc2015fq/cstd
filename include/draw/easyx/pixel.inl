// 程序名称：椭圆组合而成的抽象图案
// 编译环境：Visual C++ 6.0/2010，EasyX 2011惊蛰版
// 最初编写：1998-2-21，by yw80@qq.com（QuickBasic 版本）
// 最后修改：2011-3-23，by yw80@qq.com
//
#include <conio.h>
#include <math.h>

#define	PI	3.1415926536

// 四舍五入
int round1(double x)
{
	return (int)(x < 0 ? x - 0.5 : x + 0.5);
}

// 主函数
void test_pixel()
{
	// 初始化
	initgraph(640, 480);		// 创建绘图窗口
	setorigin(320, 240);		// 设置原点为屏幕中央
	setaspectratio(1, -1);		// 设置 y 轴向上为正

	double r = 58;
	double csin = sin(2 * PI / 200);
	double ccos = cos(2 * PI / 200);
	for(int j = 0; j < 100; j++, r -= 0.9)
	{
		double tsin = sin(2 * PI * j / 100);
		double tcos = cos(2 * PI * j / 100);
		double x = 0;
		double y = r;
		for(int i = 0; i < 200; i++)
		{
			double temp = x;
			x = x * ccos + y * csin;
			y = y * ccos - temp * csin;
			int scrx = round1(x * tcos + y * 4 * tsin);
			int scry = round1(y * 4 * tcos - x * tsin);
			putpixel(scrx, scry, GREEN);
		}

		Sleep(20);
	}

	getch();
	closegraph();
}