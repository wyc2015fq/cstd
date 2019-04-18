# Skywind Inside » MINI-FOOLTRIS 游戏代码
## MINI-FOOLTRIS 游戏代码
April 14th, 2005[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
不到两百行的俄罗斯方块游戏，教学用：
```cpp
/*=====================================================================
// FOOLTRS.C: Fool tetris game v1.0 by skywind
// 最简俄罗斯，游戏设计入门傻瓜版，林伟 1999年8月11日
//
// 游戏并无任何扩展，仅以最简单的方式向人说明俄罗斯方块的基本功能实现
// 程序运行于MS-DOS，虽然现在写游戏的平台早已变化了许多，
// 但是我尽量容入了一些不变理论，希望才入门的游戏设计者能得到一些帮助
// 用方向键/空格游戏，代码不超过两百行，初学C者皆能读懂，请用TC20编译
//
//=====================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
/*---------------------------------------------------------------------
// 基本定义：地图和砖块
//---------------------------------------------------------------------*/
int map[26][10]; /* 游戏地图定义: 所有砖块将被描述在地图中 ............*/
typedef struct { int d[4][4]; } Block;  /* 砖块结构定义 ...............*/
int BlockList[7][4][4] = {      /* 七种传统的方块定义 .................*/
  { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 1, 0 } },
  { { 0, 0, 0, 0 }, { 0, 0, 2, 0 }, { 0, 0, 2, 0 }, { 0, 2, 2, 0 } },
  { { 0, 3, 0, 0 }, { 0, 3, 0, 0 }, { 0, 3, 0, 0 }, { 0, 3, 0, 0 } },
  { { 0, 4, 0, 0 }, { 0, 4, 4, 0 }, { 0, 0, 4, 0 }, { 0, 0, 0, 0 } },
  { { 0, 0, 5, 0 }, { 0, 5, 5, 0 }, { 0, 5, 0, 0 }, { 0, 0, 0, 0 } },
  { { 0, 6, 6, 6 }, { 0, 0, 6, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
  { { 0, 0, 0, 0 }, { 0, 7, 7, 0 }, { 0, 7, 7, 0 }, { 0, 0, 0, 0 } }};
unsigned char cmap[]={0,0x72,0x30,0x47,0x57,0x27,0x60,0x74}; /* 颜色表 */
long GameMode = 0, GameSpeed = -1, GameScore = 0; /* 全局定义：各状态 .*/
/*---------------------------------------------------------------------
// 视频相关数据定义
//---------------------------------------------------------------------*/
char far* TextBuf = (char far*)0xb8000000l; /* 文本状态的显存地址 .....*/
char TextBck[20][10];                       /* 文本状态的二级缓存 .....*/
#define SCREEN(x,y) (TextBuf[((y) * 160) + ((x) * 2)])   /* 显存中字符 */
#define SCOLOR(x,y) (TextBuf[((y) * 160) + ((x) * 2) + 1]) /* 屏幕颜色 */
/*---------------------------------------------------------------------
// 基本砖块操作函数
//---------------------------------------------------------------------*/
void Rotate(const Block* src, Block* dest)  /* 旋转砖块src到dest ......*/
{
    int i, j;
    for (j = 0; j < 4; j++) for (i = 0; i < 4; i++)
    dest->d[i][j] = src->d[j][3 - i];
}
int MapCheck(int x, int y) /* 检测地图中某坐标是否有砖块 ..............*/
{
    if (x < 0 || x >=10 || y < 0 || y >= 26) return -1;
    return map[y][x];
}
int BlockCheck(int x, int y, const Block* src) /* 检测砖块位置合法性 ..*/
{
    int i, j;
    for (j = 0; j < 4; j++) for (i = 0; i < 4; i++)
        if (MapCheck(x + j, y - i) && src->d[i][j]) return 0;
    return 1;
}
void CreateBlock(Block* src) /* 随机创建一个砖块，然后保存到src .......*/
{
    int i,j,k = rand() % 7;
    Block t;
    for (i = 0; i < 4; i++) for (j = 0; j < 4; j++)
        src->d[i][j] = BlockList[k][i][j];
    for (i = rand() % 4; i > 0; i--, *src = t) Rotate(src, &t);
}
long TimePass = 0, TimeExit = -1, GameTime = -1, w;
void GameMain(void);
/*---------------------------------------------------------------------
// 程序入口函数
//---------------------------------------------------------------------*/
void main(void)
{
    clrscr();                               /* 初始化：清屏 ...........*/
    randomize();                            /* 初始化：随机数种子 .....*/
    for (w = 0; w < 26; w++) memset(map[w], 0, sizeof(int) * 10);
    for (w = 0; w < 22; w++)                /* 初始化：绘制边框 .......*/
        SCREEN(w,0)=254, SCOLOR(w,0)=8, SCREEN(w,21)=254, SCOLOR(w,21)=8,
        SCREEN(0,w)=254, SCOLOR(0,w)=8, SCREEN(21,w)=254, SCOLOR(21,w)=8;
    GameSpeed = 2;                          /* 初始化：设置速度 .......*/
    GameMain();                             /* 进入游戏主程序 .........*/
}
int readkey(void)           /* 无等待的接收一个键盘消息 ...............*/
{
    if (!kbhit()) return 0; /* 如果当前没有按键消息则直接返回 .........*/
    if ((w = getch()) == 0) return getch();   /* 返回非控制键 .........*/
    return (int)w;
}
int SpeedX[10] = { 17, 15, 13, 11, 9, 7, 5, 4, 3, 2 }; /* 速度级别 ....*/
int x = 3, y = 20, v = 0, level;    /* 全局定义：砖块的坐标，速度 .....*/
Block A, B, C;                      /* 全局定义：A, B, C三个活动砖块 ..*/
void GameDriver(void);
void GameView(void);
/*---------------------------------------------------------------------
// 游戏逻辑核心(重要): 每秒循环18.2次，处理砖块移动/下落/消除等逻辑
//---------------------------------------------------------------------*/
void GameMain(void)
{
    long key, i, j, m, n, old_time = 0;         /* 定义各变量 .........*/
    CreateBlock(&A); CreateBlock(&C);           /* 初始化砖块 .........*/
    for (GameMode = 0, v = SpeedX[GameSpeed]; GameMode == 0; TimePass++)
    {
        while (clock() == old_time);   /* 速度控制，clock()为 18.2Hz ..*/
        old_time = clock();
        nosound();
        key = readkey();               /* 读入一个键盘消息 ............*/
        if (key == 75) if (BlockCheck(x - 1, y, &A)) x--;   /* 左移 ...*/
        if (key == 77) if (BlockCheck(x + 1, y, &A)) x++;   /* 右移 ...*/
        if (key == 80) v = 0;                   /* 下落: SET v = 0 ....*/
        if (key == ' ') {                       /* 直接下落 ...........*/
            for (i = y; i >= 0; i--) if (!BlockCheck(x, i - 1, &A)) break;
            y = i; v = 0;
        }
        if (key == 27) GameMode = 1;            /* ESC: 退出游戏 ......*/
        if (key == 72) {                        /* UP: 转动砖块 .......*/
            Rotate(&A, &B);
            if (BlockCheck(x, y, &B)) A = B;
        }
        if (--v <= 0) {                     /* 到达了下落的时间 .......*/
            v = SpeedX[GameSpeed];          /* 重置砖块速度 ...........*/
            if (BlockCheck(x, y - 1, &A) == 0) {/* 如果到达map底部 ....*/
                for (i=0, m=y; i<4; i++,m--) for (j=0,n=x; j<4; j++,n++) 
                    if (MapCheck(n,m)==0 && A.d[i][j]) /* 将A复制到map */
                        map[m][n] = A.d[i][j];
                A = C;                          /* 将C复制到A .........*/
                CreateBlock(&C);                /* 创建新的C砖块 ......*/
                if (y >= 20) GameMode = -1;     /* 判断死亡 ...........*/
                else y = 20, x = 3;
                GameDriver();                   /* 消除完成的行 .......*/
            }   else y--;                       /* 否则：下降 .........*/
        }
        GameView();                             /* 显示: 更新屏幕 .....*/
    }
    while (kbhit()) getch();                    /* 按任意键结束 .......*/
    while (!kbhit());
    nosound();                                  /* 关闭扬声器 .........*/
}
void GameDriver(void)      /* 处理消除行 ..............................*/
{
    int inc[5]={10,100,300,600,1000}, count = 0, x, y, k, i, j;
    for (y = 0; y < 25; y++) for (; ; count++) { /* 扫描map测试消行 ...*/
        for (x = 0, k = 0; x < 10; x++) k += (map[y][x])? 1 : 0;
        if (k < 10) break;              /* 计算一行是否可以消去 .......*/
        for (j = y; j < 25; j++) memcpy(map[j], map[j+1], 10 * sizeof(int));
    }
    if ((GameScore / 10000) != ((GameScore + inc[count]) / 10000))
    GameSpeed = (GameSpeed + 1) % 10;       /* 计算速度升级 ...........*/
    GameScore += inc[count];                /* 计算新的分数 ...........*/
    sound((count == 0)? 75 : 900);
}
void GameView(void)        /* 绘制：将map里面的砖块绘制到屏幕 .........*/
{
    int i, j, m, n;
    for (i=0; i<10; i++) for (j=0; j<20; j++) /* 更新TextBck二级缓存 ..*/
        TextBck[j][i] = map[j][i];
    for (i=0, n=x; i<4; i++, n++) for (j = 0, m=y; j < 4; j++, m--)
        if (m>=0&&m<20 && n>=0&&n<10 && A.d[j][i]) TextBck[m][n] = A.d[j][i];
    for (i = 0; i < 10; i++) for (j = 0; j < 20; j++) { /* 显示地图 ...*/
        m = TextBck[19 - j][i], n = i * 2;
        SCREEN(n+1,j+1) = (m)? '[':'.', SCOLOR(n+1,j+1) = (m)? cmap[m] : 8;
        SCREEN(n+2,j+1) = (m)? ']':'.', SCOLOR(n+2,j+1) = (m)? cmap[m] : 8;
    }
    for (j = 0; j < 4; j++) for (i = 0; i < 4; i++) {   /* 显示下个砖块*/
        m = C.d[j][i], n = 160 * (j + 8) + 4 * i + 58;
        TextBuf[n+0] = (m)? '[':'.', TextBuf[n+1] = (m)? cmap[m] : 8;
        TextBuf[n+2] = (m)? ']':'.', TextBuf[n+3] = (m)? cmap[m] : 8;
    }
    if (TimePass & 3) return;
    textcolor(0xa-8);
    gotoxy(30,1); printf("POSITION: [%2d,%2d]", x, y);  /* 显示分数等 .*/
    gotoxy(30,2); printf("SPEED: %d", GameSpeed);
    gotoxy(30,6); printf("SCORE: "); cprintf("%ld", GameScore);
    gotoxy(30,5); printf("TIME:  ");                    /* 显示时间 ...*/
    m = (int)(TimePass / 18.2) / 60, n = (int)(TimePass / 18.2) % 60;
    cprintf("%d%d:%d%d", (m/10), (m%10), (n/10), (n%10));
    gotoxy(1, 23);
}
```
