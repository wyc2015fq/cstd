#if 0
C语言俄罗斯方块（简易版）

本俄罗斯方块全部用绘图函数实现方块的绘制，没有一点资源，因此源代码很小巧，整个程序编译链接好，也就10K多吧。非常小巧的俄罗斯方块。

设计思想：
1、将游戏区域划分为18行*10列的棋盘，设立一个布尔型的二维数组变量，以表示棋盘上各个地方是否有方块。
2、用4个顶点代表各种不同形状的方块，一旦方块在游戏区域中，就把对应的布尔型二维数组变量置为真，表示该方格已经有方块了。
3、如上做方便方块移动是否会碰撞的判断。
4、代码已经修正了一个小BUG。
5、压缩包中的文件是未经修改的源代码，此处的代码为最新。
6、方向键上为改变形状，下为直接落到底部。p键为暂停（或者Pause键） 标签： <无>

代码片段

#endif

#include "cstd.h"
#include <Windows.h> //为了使用API函数
#include <time.h>	//为了使用定时器
#include <stdlib.h>  //为了使用随机数

#define BLOCKWIDTH		20  //单个方块大小
#define NUMLINEBLOCKS	18  //行数
#define NUMCOLUMNBLOCKS	10  //列数
#define ID_TIMER		1	//定时器ID
#define BLOCKSTYLES		(sizeof (Blocks) / sizeof (Blocks[0]))  //方块的种类数

//游戏区各方格顶点布尔值，代表该方格是否有方块
BOOL	GameClient[NUMCOLUMNBLOCKS][NUMLINEBLOCKS];
static int		F, S, cF, cS;	//随机方块图形对应的第一、二纬
static int		Score;  //得分

//定义各方块形状，以点表示
struct  
{
	POINT	pt[4];
}
Blocks[][4] = 
{
	//正7
	0, 0, 1, 0, 1, 1, 1, 2,  2, 0, 0, 1, 1, 1, 2, 1,  1, 0, 1, 1, 1, 2, 2, 2,  0, 1, 1, 1, 2, 1, 0, 2,
	//反7
	1, 0, 2, 0, 1, 1, 1, 2,  0, 1, 1, 1, 2, 1, 2, 2,  1, 0, 1, 1, 0, 2, 1, 2,  0, 0, 0, 1, 1, 1, 2, 1,
	//1
	1, 0, 1, 1, 1, 2, 1, 3,  0, 1, 1, 1, 2, 1, 3, 1,  1, 0, 1, 1, 1, 2, 1, 3,  0, 1, 1, 1, 2, 1, 3, 1,
	//Z
	0, 0, 1, 0, 1, 1, 2, 1,  2, 0, 1, 1, 2, 1, 1, 2,  0, 0, 1, 0, 1, 1, 2, 1,  2, 0, 1, 1, 2, 1, 1, 2,
	//反Z
	1, 0, 2, 0, 0, 1, 1, 1,  1, 0, 1, 1, 2, 1, 2, 2,  1, 0, 2, 0, 0, 1, 1, 1,  1, 0, 1, 1, 2, 1, 2, 2,
	//田字
	0, 0, 1, 0, 0, 1, 1, 1,  0, 0, 1, 0, 0, 1, 1, 1,  0, 0, 1, 0, 0, 1, 1, 1,  0, 0, 1, 0, 0, 1, 1, 1,
	//尖头
	1, 0, 0, 1, 1, 1, 2, 1,  0, 0, 0, 1, 1, 1, 0, 2,  0, 0, 1, 0, 2, 0, 1, 1,  1, 0, 0, 1, 1, 1, 1, 2
};

//判断方块是否可以下落
BOOL CanDown(POINT pt[])
{
  int i;
	BOOL result = TRUE;
	//将方块所在格子先假设指定为无方块
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = FALSE;
	for(i = 0; i < 4; ++i)
	{
		//假如继续落下超过下底边界，返回FALSE；或者假如该小方块下落一格已经有方块，结果为FALSE
		if(pt[i].y + 1 == NUMLINEBLOCKS || GameClient[pt[i].x][pt[i].y + 1])
		{
			result = FALSE;
			break;
		}
	}
	//恢复方块所在格子为有方块
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = TRUE;
	return result;
}


//判断是否可以左移
BOOL CanLeft(POINT pt[])
{
  int i;
	BOOL result = TRUE;
	//将方块所在格子先假设指定为无方块
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = FALSE;
	for(i = 0; i < 4; ++i)
	{
		//假如继续左移超过左边边界，返回FALSE；或者假如该小方块左移一格已经有方块，结果为FALSE
		if(!pt[i].x || GameClient[pt[i].x - 1][pt[i].y])
		{
			result = FALSE;
			break;
		}
	}
	//恢复方块所在格子为有方块
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = TRUE;
	return result;
}

//判断是否可以右移
BOOL CanRight(POINT pt[])
{
  int i;
	BOOL result = TRUE;
	//将方块所在格子先假设指定为无方块
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = FALSE;
	for(i = 0; i < 4; ++i)
	{
		//假如继续左移超过左边边界，返回FALSE；或者假如该小方块左移一格已经有方块，结果为FALSE
		if(pt[i].x + 1 == NUMCOLUMNBLOCKS || GameClient[pt[i].x + 1][pt[i].y])
		{
			result = FALSE;
			break;
		}
	}
	//恢复方块所在格子为有方块
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = TRUE;
	return result;
}

//判断是否可以旋转
BOOL CanChange(POINT pt[])
{
  int i, k;
	BOOL result = TRUE;
	//将方块所在格子先假设指定为无方块
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = FALSE;
	int t = (cS + 1) % 4;
	for(k = 0; k < 4; ++k)
	{
		int x = Blocks[cF][t].pt[k].x - Blocks[cF][cS].pt[k].x,
			y = Blocks[cF][t].pt[k].y - Blocks[cF][cS].pt[k].y;
		if(GameClient[pt[k].x + x][pt[k].y + y] ||  //该方格已经有方块
			pt[k].x + x > NUMCOLUMNBLOCKS - 1 ||  //x坐标超越了右边界
			pt[k].x + x < 0 ||   //x坐标超越了左边界
			pt[k].y + y > NUMLINEBLOCKS - 1)  //y坐标超越了下底边界
		{
			result = FALSE;
			break;
		}
	}

	//恢复方块所在格子为有方块
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = TRUE;
	return result;
}

//实现旋转
void Change(POINT pt[])
{
  int i;
	int t = (cS + 1) % 4;
	for(i = 0; i < 4; ++i)
	{
		int x = Blocks[cF][t].pt[i].x - Blocks[cF][cS].pt[i].x,
			y = Blocks[cF][t].pt[i].y - Blocks[cF][cS].pt[i].y;
		GameClient[pt[i].x][pt[i].y] = FALSE;
		pt[i].x += x;
		pt[i].y += y;
		GameClient[pt[i].x][pt[i].y] = TRUE;
	}
	cS = t;
}

//实现右移
void Right(POINT pt[])
{
  int i, k;
	for(i = 0; i < 4; ++i)
	{
		GameClient[pt[i].x][pt[i].y] = FALSE;
		++pt[i].x;
	}
	for(k = 0; k < 4; ++k)
		GameClient[pt[k].x][pt[k].y] = TRUE;
}

//实现左移
void Left(POINT pt[])
{
  int i, k;
	for(i = 0; i < 4; ++i)
	{
		GameClient[pt[i].x][pt[i].y] = FALSE;
		--pt[i].x;
	}
	for(k = 0; k < 4; ++k)
		GameClient[pt[k].x][pt[k].y] = TRUE;
}

//实现方块的下落
void Down(POINT pt[])
{
  int i, k;
	for(i = 0; i < 4; ++i)
	{
		GameClient[pt[i].x][pt[i].y] = FALSE;
		++pt[i].y;
	}
	for(k = 0; k < 4; ++k)
		GameClient[pt[k].x][pt[k].y] = TRUE;
}

//随机数函数定制版
inline unsigned Random(int n)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	srand(st.wMilliseconds);
	return rand() % n;
}

//消行处理以及分数结算
void DelSqure(HWND hwnd)
{
  int x, y;
	int line = 0, temp;
	for(x = NUMLINEBLOCKS - 1; x >= 0; --x)
	{
		BOOL result = TRUE;
		for(y = 0; y < NUMCOLUMNBLOCKS; ++y)
		{
			if(!GameClient[y][x])
			{
				result = FALSE;
				break;
			}
		}
		//判断是否可以消行
		if(result)
		{
			temp = x;
			++line;
			while(x > 0)
			{
				for(y = 0; y < NUMCOLUMNBLOCKS; ++y)
				{
					GameClient[y][x] = GameClient[y][x - 1];
				}
				--x;
			}
			for(y = 0; y < NUMCOLUMNBLOCKS; ++y)
				GameClient[y][0] = FALSE;
			x = temp + 1;
		}
	}
	if(line)
		Score += (line - 1) * 2 + 1;
	InvalidateRect(hwnd, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT		TericsBorder[] = {-1, -1, 
		NUMCOLUMNBLOCKS * BLOCKWIDTH + 1, NUMLINEBLOCKS * BLOCKWIDTH + 1};
	HDC			hdc;
	PAINTSTRUCT	ps;
	TEXTMETRIC	tm;
	TCHAR		szNextTerics[] = TEXT("下一个："), szSCore[] = TEXT("得分：");
	static TCHAR szBufferScore[5];
	static int	cxChar, cyChar;
	static POINT Block[4], NextBlock[4];
	int			x, y;
  int i;
	static BOOL		pause = FALSE;  //暂停

	switch(message)
	{
	case WM_CREATE:
    {
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth * 2;
		cyChar = tm.tmExternalLeading + tm.tmHeight;
		SetTimer(hwnd, ID_TIMER, 600, NULL);
		//初始化第一个出现的方块
		cS = Random(4);
		cF = Random(BLOCKSTYLES);
		for(i = 0; i < 4; ++i)
		{
			Block[i].x = Blocks[cF][cS].pt[i].x + 4;
			Block[i].y = Blocks[cF][cS].pt[i].y;
			GameClient[Block[i].x][Block[i].y] = TRUE;
		}
		S = Random(4);
		F = Random(BLOCKSTYLES);
		for(i = 0; i < 4; ++i)
		{
			NextBlock[i].x = Blocks[F][S].pt[i].x;
			NextBlock[i].y = Blocks[F][S].pt[i].y;
		}

		ReleaseDC(hwnd, hdc);
		return 0;
    }
	case WM_TIMER:
    {
		if(pause) return 0;
		if(CanDown(Block))
		{
			Down(Block);
		}
		//不能下移，需要处理消行判断（结合分数），还需要处理下一个显示，和当前显示的方块
		else
		{
			DelSqure(hwnd);
			for(i = 0; i < 4; ++i)
			{
				Block[i].x = NextBlock[i].x + 4;
				Block[i].y = NextBlock[i].y;
				if(GameClient[Block[i].x][Block[i].y])
				{
					KillTimer(hwnd, ID_TIMER);
				}
				else
					GameClient[Block[i].x][Block[i].y] = TRUE;
			}
			cS = S;  cF = F;
			S = Random(4);
			F = Random(BLOCKSTYLES);
			for(i = 0; i < 4; ++i)
			{
				NextBlock[i].x = Blocks[F][S].pt[i].x;
				NextBlock[i].y = Blocks[F][S].pt[i].y;
			}
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
    }

	case WM_KEYDOWN:
		if(pause && wParam != VK_PAUSE) return 0;
		switch(wParam)
		{
		case VK_LEFT:
			if(CanLeft(Block))
				Left(Block);
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		case VK_RIGHT:
			if(CanRight(Block))
				Right(Block);
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		case VK_UP:
			if(CanChange(Block))
				Change(Block);
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		case VK_DOWN:
			while(CanDown(Block))
				Down(Block);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case VK_PAUSE:
			pause = !pause;
			break;

		default:
			break;
		}
		return 0;

	case WM_CHAR:
		if(wParam == 'p')
			pause = !pause;
		else if(wParam == 'r')
		{
			Score = 0;
			for(x = 0; x < NUMCOLUMNBLOCKS; ++x)
			{
				for(y = 0; y < NUMLINEBLOCKS; ++y)
					GameClient[x][y] = FALSE;
			}
			cS = Random(4);
			cF = Random(BLOCKSTYLES);
			for(i = 0; i < 4; ++i)
			{
				Block[i].x = Blocks[cF][cS].pt[i].x + 4;
				Block[i].y = Blocks[cF][cS].pt[i].y;
				GameClient[Block[i].x][Block[i].y] = TRUE;
			}
			S = Random(4);
			F = Random(BLOCKSTYLES);
			for(i = 0; i < 4; ++i)
			{
				NextBlock[i].x = Blocks[F][S].pt[i].x;
				NextBlock[i].y = Blocks[F][S].pt[i].y;
			}
			pause = FALSE;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		return 0;
		
	case WM_PAINT:
		//if(pause) return 0;
		hdc = BeginPaint(hwnd, &ps);
		SetViewportOrgEx(hdc, BLOCKWIDTH, BLOCKWIDTH, NULL);
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		SelectObject(hdc, GetStockObject(BLACK_PEN));

		//画俄罗斯方块游戏的边框
		Rectangle(hdc, TericsBorder[0].x, TericsBorder[0].y,
			TericsBorder[1].x, TericsBorder[1].y);
		//输出“下一个”字符串
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, 0, szNextTerics, lstrlen(szNextTerics));

		//输出“得分”字符串
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 5 * BLOCKWIDTH,
			szSCore, lstrlen(szSCore));
		//
		SetTextAlign(hdc, TA_RIGHT | TA_TOP);
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH + 3 * cxChar, 2 * cyChar + 5 * BLOCKWIDTH,
			szBufferScore, wsprintf(szBufferScore, TEXT("%d"), Score));
		SetTextAlign(hdc, TA_LEFT | TA_TOP);

		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		SelectObject(hdc, GetStockObject(WHITE_PEN));
		//显示游戏区的方块
		for(x = 0; x < NUMCOLUMNBLOCKS; ++x)
		{
			for(y = 0; y < NUMLINEBLOCKS; ++y)
			{
				if(GameClient[x][y])
				{
					Rectangle(hdc, x * BLOCKWIDTH, y * BLOCKWIDTH,
						(x + 1) * BLOCKWIDTH, (y + 1) * BLOCKWIDTH);
				}
			}
		}
		//显示下一个方块区域的方块
		for(i = 0; i < 4; ++i)
		{
			Rectangle(hdc, (NextBlock[i].x + NUMCOLUMNBLOCKS + 2) * BLOCKWIDTH, NextBlock[i].y * BLOCKWIDTH + cyChar,
				(NextBlock[i].x + NUMCOLUMNBLOCKS + 3) * BLOCKWIDTH, (NextBlock[i].y + 1) * BLOCKWIDTH + cyChar);
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


int test_tetris()
{
  HINSTANCE hInstance=0;
  HINSTANCE hPrevInstance=0;
  LPSTR lpCmdLine=0;
  int nCmdShow=0;
  WNDCLASS wndcls;
  TCHAR szClassName[] = TEXT("Terics"),
		  szWindowName[] = TEXT("Aka's Terics");
  static POINT Block[4];
  
  wndcls.cbClsExtra		= 0;
  wndcls.cbWndExtra		= 0;
  wndcls.hbrBackground	= static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
  wndcls.hCursor			= LoadCursor(hInstance, IDC_ARROW);
  wndcls.hIcon			= LoadIcon(hInstance, IDI_APPLICATION);
  wndcls.hInstance		= hInstance;
  wndcls.lpfnWndProc		= WndProc;
  wndcls.lpszClassName	= szClassName;
  wndcls.lpszMenuName		= NULL;
  wndcls.style			= CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wndcls);
  
  HWND hwnd = CreateWindow(szClassName, szWindowName, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
    CW_USEDEFAULT, CW_USEDEFAULT, (NUMCOLUMNBLOCKS + 10) * BLOCKWIDTH,
    (NUMLINEBLOCKS + 3) * BLOCKWIDTH,
    NULL, NULL, hInstance, NULL);
  ShowWindow(hwnd, SW_SHOWNORMAL);
  UpdateWindow(hwnd);
  
  MSG msg;
  while(GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

