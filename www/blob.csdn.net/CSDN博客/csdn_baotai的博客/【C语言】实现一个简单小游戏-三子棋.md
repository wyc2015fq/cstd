# 【C语言】实现一个简单小游戏-三子棋 - csdn_baotai的博客 - CSDN博客

2018年04月23日 23:40:59[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：31
个人分类：[C语言](https://blog.csdn.net/csdn_baotai/article/category/7659809)



# 实现一个简单小游戏-三子棋

## 头文件：game.h

```cpp
#ifndef __GAME_H__
#define __GAME_H__

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>

#define ROW 3
#define COL 3

void game();
#endif
```

## 主函数：main.c

```cpp
#include "game.h"

void menu()
{
	printf("************************\n");
	printf("****1.play****2.exit****\n");
	printf("************************\n");
}
int main()
{
	int input = 0;
	do
	{
		menu();
		printf("请选择：>");
		scanf("%d", &input);
		switch (input)//游戏入口
		{
		case 1:		   game();		 break;
		case 2:         exit(0);
		default:printf("选择错误！\n");          break;
		}
	} while (input);
	system("pause");
	return 0;

}
```

## 游戏实现函数：game.h

```cpp
#include"game.h"
void displayBoard(char board[ROW][COL], int col)//显示棋盘函数
{
	int i = 0;
	for (i = 0; i < ROW; i++)
	{
		printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
		if (i < (ROW - 1))
		{
			printf("---|---|---\n");
		}
	}
}

void playerMove(char board[][COL], int row)//玩家落子
{
	//int flag=0;
	int x, y;
	do
	{
		printf("Please enter <x，y>:");
		scanf("%d,%d", &x, &y);
		if (x > 0 && x < 4 && y > 0 && y < 4)
		{
			if (board[x - 1][y - 1] == ' ')
			{
				board[x - 1][y - 1] = 'x';
				//		flag = 1;
				break;
			}
			else
			{
				printf("This Place Is Already Occupied,Please Select Anothor!\n");
			}
		}
		else
		{
			printf("Error input!try again\n");
		}
	} while (1);
}

void computerMove(char board[][COL], int row)//电脑随机落子
{
	srand((unsigned long)time(NULL));
	do
	{
		int x = rand() % ROW;
		int y = rand() % COL;
		if (board[x][y] == ' ')
		{
			board[x][y] = 'o';
			break;
		}
	} while (1);
}
int isFull(char board[][COL], int row)//检查棋盘是否已下满
{
	int i = 0;
	for (; i < row; i++)
	{
		int j = 0;
		for (; j < COL; j++)
		{
			if (board[i][j] == ' ')
			{
				return 0;
			}
		}
	}
	return 1;
}
char isWin(char board[][COL], int row)//检查是否分出胜负
{
	int i = 0;
	for (; i < ROW; i++)
	{
		if (board[i][0] == board[i][1] && \
			board[i][1] == board[i][2] && \
			board[i][1] != ' ')
			return board[i][0];
	}
	for (i = 0; i < COL; i++)
	{
		if (board[0][i] == board[1][i] && \
			board[1][i] == board[2][i] && \
			board[2][i] != ' ')
			return board[2][i];
	}
	if (board[0][0] == board[1][1] && \
		board[1][1] == board[2][2] && \
		board[1][1] != ' ')
		return board[1][1];
	if (board[0][2] == board[1][1] && \
		board[1][1] == board[2][0] && \
		board[1][1] != ' ')
		return board[1][1];
	if (isFull(board, ROW))
		return 'q';
	return ' ';
}

void game()//游戏函数
{
	char ret;
	char board[ROW][COL];
	memset(board, ' ', ROW*COL);
	do
	{
		system("CLS");
		displayBoard(board, ROW);  //显示游戏面板
		playerMove(board, ROW);    //玩家先走
		ret = isWin(board, ROW);   //将玩家的棋子通过isWin函数返回给ret   x代表玩家，o代表电脑 ，q代表平局
		if (ret != ' ')
		{
			break;
		}
		computerMove(board, ROW);
		ret = isWin(board, ROW);

	} while (ret == ' ');         //如果棋盘上有空位则继续循环，直到棋盘下满为止
	if (ret == 'q')
	{
		printf("哈哈，平局！\n");
	}
	else if (ret == 'x')
	{
		printf("恭喜！你赢了\n");
	}
	else if (ret == 'o')
	{
		printf("很遗憾，电脑赢了！\n");
	}
	else
	{
		printf("debug!\n");
	}
}
```

