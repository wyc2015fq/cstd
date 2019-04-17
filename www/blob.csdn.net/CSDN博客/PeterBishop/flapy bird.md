# flapy bird - PeterBishop - CSDN博客





2018年03月12日 21:07:21[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：126








```cpp
#include <graphics.h>
#include <conio.h>
#include<stdlib.h>
#include<stdio.h>

#pragma comment(lib,"Winmm.lib")


IMAGE img_bk, img_bd1, img_bd2, img_bar_up1, img_bar_up2, img_bar_down1, img_bar_down2;
int bird_x;
int bird_y;
int bar_x;
int bar_up_y, bar_down_y;
bool dead;
int score;
int v = 1;
void startup()
{
	mciSendString("open F:\\游戏素材\\小鸟\\background.mp3 alias bkmusic", NULL, 0, NULL);//背景音乐
	mciSendString("play bkmusic repeat", NULL, 0, NULL);  // 循环播放


	initgraph(200, 400);
	loadimage(&img_bk, "F:\\游戏素材\\小鸟\\background.jpg");
	loadimage(&img_bd1, "F:\\游戏素材\\小鸟\\bird1.jpg");
	loadimage(&img_bd2, "F:\\游戏素材\\小鸟\\bird2.jpg");
	loadimage(&img_bar_up1, "F:\\游戏素材\\小鸟\\bar_up1.gif", 140, 250);
	loadimage(&img_bar_up2, "F:\\游戏素材\\小鸟\\bar_up2.gif", 140, 250);
	loadimage(&img_bar_down1, "F:\\游戏素材\\小鸟\\bar_down1.gif", 140, 250);
	loadimage(&img_bar_down2, "F:\\游戏素材\\小鸟\\bar_down2.gif", 140, 250);
	bird_x = 50;
	bird_y = 200;
	bar_x = 150;
	bar_up_y = -50, bar_down_y = 280;
	dead = false;
	score = 0;
	BeginBatchDraw();
}

void show()
{
	putimage(0, 0, &img_bk);	// 显示背景	
	if (!dead)
	{
		putimage(bar_x, bar_up_y, &img_bar_up1, NOTSRCERASE); // 显示上一半的障碍物
		putimage(bar_x, bar_up_y, &img_bar_up2, SRCINVERT);
		putimage(bar_x, bar_down_y, &img_bar_down1, NOTSRCERASE); // 显示下一半的障碍物
		putimage(bar_x, bar_down_y, &img_bar_down2, SRCINVERT);
		putimage(bird_x, bird_y, &img_bd1, NOTSRCERASE); // 显示小鸟	
		putimage(bird_x, bird_y, &img_bd2, SRCINVERT);
		outtextxy(200 * 0.40, 400 * 0.95, "得分：");
		char s[5];
		sprintf(s, "%d", score);
		outtextxy(200 * 0.65, 400 * 0.95, s);
		FlushBatchDraw();
		Sleep(50);
	}
	else
	{
		outtextxy(200 * 0.40, 400 * 0.95, "得分：");
		char s[5];
		sprintf(s, "%d", score);
		outtextxy(200 * 0.65, 400 * 0.95, s);
		outtextxy(200 * 0.30, 200, "GAME OVER!");
		FlushBatchDraw();
	}

}

void updateWithoutInput()
{
	if (bird_y<380)
		bird_y = bird_y + 3;
	if (bar_x >= -140)
		bar_x-=v;
	else
	{
		bar_x = 200;
		bar_up_y = rand() % 200 - 200;
		bar_down_y = rand() % 200 + 200;
	}
	if ((((bird_x >= bar_x) && bird_x <= bar_x + 140) && bird_y<bar_up_y + 250) || (((bird_x >= bar_x) && bird_x <= bar_x + 140) && bird_y>bar_down_y))
		dead = true;
	if (bird_x == bar_x&&!dead)
	{
		score++;
	}
	if (score % 5 == 0&&score)
		v++;
}

void updateWithInput()
{
	char input;
	if (kbhit())  // 判断是否有输入
	{
		input = getch();
		if (input == ' ' && bird_y>20)
		{
			bird_y = bird_y - 30;

			mciSendString("close jpmusic", NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString("open F:\\游戏素材\\小鸟\\Jump.mp3 alias jpmusic", NULL, 0, NULL); // 打开跳动音乐
			mciSendString("play jpmusic", NULL, 0, NULL); // 仅播放一次

		}
	}
}
void gameover()
{
	EndBatchDraw();
	getch();
	closegraph();
}

int main()
{
	startup();  // 数据初始化	
	while (1)  //  游戏循环执行
	{
		show();  // 显示画面
		updateWithoutInput();  // 与用户输入无关的更新
		updateWithInput();     // 与用户输入有关的更新
	}
	gameover();     // 游戏结束、后续处理
	return 0;
}
```




