

// 程序名称：字符阵
// 编译环境：Visual C++ 6.0，EasyX 2011惊蛰版
// 最后更新：2009-2-22
//

#include <time.h>
#include <conio.h>

void test_chrmatrix()
{
	// 设置随机函数种子
	srand((unsigned) time(NULL));

	// 初始化图形模式
	initgraph(640, 480);

	int x, y;
	char c;

	setfont(16, 8, "Courier");	// 设置字体

	while(!kbhit())
	{
		for (int i=0; i<479; i++)
		{
			setcolor(GREEN);
			for (int j=0; j<3; j++)
			{
				x = (rand() % 80) * 8;
				y = (rand() % 20) * 24;
				c = (rand() % 26) + 65;
				outtextxy(x, y, c);
			}

			setcolor(0);
			line(0,i,639,i);

			Sleep(10);
			if (kbhit()) break;
		}
	}

	// 关闭图形模式
	closegraph();
}

