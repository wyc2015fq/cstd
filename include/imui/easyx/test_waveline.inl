///////////////////////////////////////////////////
// 程序名称：图片滤镜系列：波浪线叠加效果
// 编译环境：Visual C++ 6.0 / 2010，EasyX 2011惊蛰版
// 作　　者：yangw80 <yw80@qq.com>
// 最后修改：2011-7-25
//
#include <conio.h>
#include <math.h>

#define PI		3.1415926535
#define PEN		0x4d8399		// 绘图颜色(金色)
#define RATIO	GetBValue		// 获取 PEN 的主要颜色分量

IMAGE g_img;					// 保存有图片的全局 IMAGE 对象


// 画点(叠加法) (0 <= alpha <= 1, alpha = 1，表示完全不透明)
void mixpixel(int x, int y, double alpha)
{
	double f1 = (0xff - RATIO(getpixel(x, y))) / (float)(0xff - RATIO(PEN));
	double f = f1 + alpha;
	if (f > 1)	f = 1;

	COLORREF c = RGB(	GetRValue(PEN) + (0xff - GetRValue(PEN)) * (1 - f),
						GetGValue(PEN) + (0xff - GetGValue(PEN)) * (1 - f),
						GetBValue(PEN) + (0xff - GetBValue(PEN)) * (1 - f)	);

	putpixel(x, y, c);
}


// 画点(支持浮点 y 坐标)
void mypixel(int x, double y)
{
	int y1 = (int)floor(y);
	int y2 = y1 + 1;

	mixpixel(x, y1, y2 - y);
	mixpixel(x, y2, y - y1);
}


// 画纵向线
void myline(int x, double y)
{
	// 获取灰度图片指定位置的颜色平均值
	SetWorkingImage(&g_img);
	int sum = 0;
	int count = 0;
	for(int i = (int)(y - 2 + 0.5); i <= (int)(y + 2 + 0.5); i++)
		if (i >= 0)
		{
			count++;
			sum += (0xff & getpixel(x, i));
		}
	if (count > 0)
		sum = (int)(sum / (float)count + 0.5);
	SetWorkingImage(NULL);

	// 根据获取到的颜色值，画等效线
	if (sum >= 192)
		mypixel(x, y);
	else
	{
		int y1 = (int)y;
		sum += (int)(64 * (1 - (y - y1)));
		mixpixel(x, y1, (1 - (y - y1)));
		while(sum < 192)
		{
			sum += 64;
			mixpixel(x, ++y1, 1);
		}
		mixpixel(x, ++y1, 1 - (sum - 191) / 64.0);
	}
}


// 主函数
void test_waveline()
{
	// 初始化
	initgraph(640, 480);				// 创建绘图窗口
	loadimage(&g_img, PICNAME);	// 加载图片
	setbkcolor(0xffffff);
	cleardevice();

	// 将图片转换为灰度图片
	DWORD* buf = GetImageBuffer(&g_img);
	for(int i = 0; i < g_img.getheight() * g_img.getwidth(); i++)
	{
		buf[i] = BGR(RGBtoGRAY(BGR(buf[i])));
	}

	// 画背景正弦曲线，并在此基础上叠加图像
	for(int x = 0; x < 640; x++)
	{
		double y1 = sin(x / 30.0 / PI) * 20.8;
		for(double y = y1 - 20; y < 480; y+=5)
			myline(x, y);
	}

	// 按任意键退出
	getch();
	closegraph();
}