#if 0
C语言俄罗斯方块（简易版）
本俄罗斯方块全部用绘图函数实现方块的绘制，没有一点资源，因此源代码很小巧，整个程序编译链接好，也就10K多吧。非常小巧的俄罗斯方块。
设计思想：
1、将游戏区域划分为18行 * 10列的棋盘，设立一个布尔型的二维数组变量，以表示棋盘上各个地方是否有方块。
2、用4个顶点代表各种不同形状的方块，一旦方块在游戏区域中，就把对应的布尔型二维数组变量置为真，表示该方格已经有方块了。
3、如上做方便方块移动是否会碰撞的判断。
4、代码已经修正了一个小BUG。
5、压缩包中的文件是未经修改的源代码，此处的代码为最新。
6、方向键上为改变形状，下为直接落到底部。p键为暂停（或者Pause键） 标签： < 无 >
#endif
#define BLOCKWIDTH 20 //单个方块大小
#define NUMLINEBLOCKS 18 //行数
#define NUMCOLUMNBLOCKS 10 //列数
#define ID_TIMER 1 //定时器ID
#define BLOCKSTYLES (sizeof (tetris_blocks) / sizeof (tetris_blocks[0])) //方块的种类数
#define GAME_STOP 0
#define GAME_RUN 1
typedef struct test_tetris_t {
  int inited;
  int cxChar, cyChar;
  IPOINT Block[4], NextBlock[4];
  BOOL pause;
  int score; //得分
  int game_time;
  int stage;
  //游戏区各方格顶点布尔值，代表该方格是否有方块
  BOOL client[NUMCOLUMNBLOCKS][NUMLINEBLOCKS];
  int F, S, cF, cS; //随机方块图形对应的第一、二纬
  double start_time;
} test_tetris_t;
//定义各方块形状，以点表示
const IPOINT tetris_blocks[][4][4] = {
  //正7
  0, 0, 1, 0, 1, 1, 1, 2, 2, 0, 0, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 2, 2, 2, 0, 1, 1, 1, 2, 1, 0, 2,
  //反7
  1, 0, 2, 0, 1, 1, 1, 2, 0, 1, 1, 1, 2, 1, 2, 2, 1, 0, 1, 1, 0, 2, 1, 2, 0, 0, 0, 1, 1, 1, 2, 1,
  //1
  1, 0, 1, 1, 1, 2, 1, 3, 0, 1, 1, 1, 2, 1, 3, 1, 1, 0, 1, 1, 1, 2, 1, 3, 0, 1, 1, 1, 2, 1, 3, 1,
  //Z
  0, 0, 1, 0, 1, 1, 2, 1, 2, 0, 1, 1, 2, 1, 1, 2, 0, 0, 1, 0, 1, 1, 2, 1, 2, 0, 1, 1, 2, 1, 1, 2,
  //反Z
  1, 0, 2, 0, 0, 1, 1, 1, 1, 0, 1, 1, 2, 1, 2, 2, 1, 0, 2, 0, 0, 1, 1, 1, 1, 0, 1, 1, 2, 1, 2, 2,
  //田字
  0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1,
  //尖头
  1, 0, 0, 1, 1, 1, 2, 1, 0, 0, 0, 1, 1, 1, 0, 2, 0, 0, 1, 0, 2, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 2
};
//判断方块是否可以下落
BOOL CanDown(test_tetris_t* s, const IPOINT* pt)
{
  int i;
  BOOL result = TRUE;
  //将方块所在格子先假设指定为无方块
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
  }
  for (i = 0; i < 4; ++i) {
    //假如继续落下超过下底边界，返回FALSE；或者假如该小方块下落一格已经有方块，结果为FALSE
    if (pt[i].y + 1 == NUMLINEBLOCKS || s->client[pt[i].x][pt[i].y + 1]) {
      result = FALSE;
      break;
    }
  }
  //恢复方块所在格子为有方块
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = TRUE;
  }
  return result;
}
//判断是否可以左移
BOOL CanLeft(test_tetris_t* s, IPOINT* pt)
{
  int i;
  BOOL result = TRUE;
  //将方块所在格子先假设指定为无方块
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
  }
  for (i = 0; i < 4; ++i) {
    //假如继续左移超过左边边界，返回FALSE；或者假如该小方块左移一格已经有方块，结果为FALSE
    if (!pt[i].x || s->client[pt[i].x - 1][pt[i].y]) {
      result = FALSE;
      break;
    }
  }
  //恢复方块所在格子为有方块
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = TRUE;
  }
  return result;
}
//判断是否可以右移
BOOL CanRight(test_tetris_t* s, IPOINT* pt)
{
  int i;
  BOOL result = TRUE;
  //将方块所在格子先假设指定为无方块
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
  }
  for (i = 0; i < 4; ++i) {
    //假如继续左移超过左边边界，返回FALSE；或者假如该小方块左移一格已经有方块，结果为FALSE
    if (pt[i].x + 1 == NUMCOLUMNBLOCKS || s->client[pt[i].x + 1][pt[i].y]) {
      result = FALSE;
      break;
    }
  }
  //恢复方块所在格子为有方块
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = TRUE;
  }
  return result;
}
//判断是否可以旋转
BOOL CanChange(test_tetris_t* s, IPOINT* pt)
{
  int i, k, t;
  BOOL result = TRUE;
  //将方块所在格子先假设指定为无方块
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
  }
  t = (s->cS + 1) % 4;
  for (k = 0; k < 4; ++k) {
    int x = tetris_blocks[s->cF][t][k].x - tetris_blocks[s->cF][s->cS][k].x,
        y = tetris_blocks[s->cF][t][k].y - tetris_blocks[s->cF][s->cS][k].y;
    if (s->client[pt[k].x + x][pt[k].y + y] || //该方格已经有方块
        pt[k].x + x > NUMCOLUMNBLOCKS - 1 || //x坐标超越了右边界
        pt[k].x + x < 0 || //x坐标超越了左边界
        pt[k].y + y > NUMLINEBLOCKS - 1) { //y坐标超越了下底边界
      result = FALSE;
      break;
    }
  }
  //恢复方块所在格子为有方块
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = TRUE;
  }
  return result;
}
int GameClient1[NUMCOLUMNBLOCKS* NUMLINEBLOCKS];
//实现旋转
int Change(test_tetris_t* s, IPOINT* pt)
{
  int i;
  int t = (s->cS + 1) % 4;
  for (i = 0; i < 4; ++i) {
    int x = tetris_blocks[s->cF][t][i].x - tetris_blocks[s->cF][s->cS][i].x,
        y = tetris_blocks[s->cF][t][i].y - tetris_blocks[s->cF][s->cS][i].y;
    s->client[pt[i].x][pt[i].y] = FALSE;
    pt[i].x += x;
    pt[i].y += y;
    s->client[pt[i].x][pt[i].y] = TRUE;
  }
  s->cS = t;
  if (0) {
    for (i = 0; i < NUMLINEBLOCKS; ++i) {
      int j;
      for (j = 0; j < NUMCOLUMNBLOCKS; ++j) {
        GameClient1[i * NUMCOLUMNBLOCKS + j] = s->client[j][i];
      }
    }
    //cvShowMat("GameClient1", "%d", NUMLINEBLOCKS, NUMCOLUMNBLOCKS, GameClient1, NUMCOLUMNBLOCKS*sizeof(BOOL), sizeof(BOOL), -1);
    //cvWaitKey(-1);
  }
  return 0;
}
//实现右移
int Right(test_tetris_t* s, IPOINT* pt)
{
  int i, k;
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
    ++pt[i].x;
  }
  for (k = 0; k < 4; ++k) {
    s->client[pt[k].x][pt[k].y] = TRUE;
  }
  return 0;
}
//实现左移
int Left(test_tetris_t* s, IPOINT* pt)
{
  int i, k;
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
    --pt[i].x;
  }
  for (k = 0; k < 4; ++k) {
    s->client[pt[k].x][pt[k].y] = TRUE;
  }
  return 0;
}
//实现方块的下落
int Down(test_tetris_t* s, IPOINT* pt)
{
  int i, k;
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
    ++pt[i].y;
  }
  for (k = 0; k < 4; ++k) {
    s->client[pt[k].x][pt[k].y] = TRUE;
  }
  return 0;
}
//随机数函数定制版
unsigned Random(int n)
{
  SYSTEMTIME st;
  GetLocalTime(&st);
  srand(st.wMilliseconds);
  return rand() % n;
}
//消行处理以及分数结算
int DelSqure(test_tetris_t* s)
{
  int x, y;
  int line = 0, temp;
  for (x = NUMLINEBLOCKS - 1; x >= 0; --x) {
    BOOL result = TRUE;
    for (y = 0; y < NUMCOLUMNBLOCKS; ++y) {
      if (!s->client[y][x]) {
        result = FALSE;
        break;
      }
    }
    //判断是否可以消行
    if (result) {
      temp = x;
      ++line;
      while (x > 0) {
        for (y = 0; y < NUMCOLUMNBLOCKS; ++y) {
          s->client[y][x] = s->client[y][x - 1];
        }
        --x;
      }
      for (y = 0; y < NUMCOLUMNBLOCKS; ++y) {
        s->client[y][0] = FALSE;
      }
      x = temp + 1;
    }
  }
  if (line) {
    s->score += (line - 1) * 2 + 1;
  }
  return 0;
}
double dtime()
{
  return sys_utime_counter() * 1. / sys_utime_frequency();
}
static int test_tetris()
{
  GETGUIIO2();
  static test_tetris_t s[1] = {0};
  IRECT rc;
  int x, y;
  int i;
  int m_text_height = 0;
  if (!s->inited) {
    s->inited = 1;
    s->stage = GAME_RUN;
    //初始化第一个出现的方块
    s->cS = Random(4);
    s->cF = Random(BLOCKSTYLES);
    for (i = 0; i < 4; ++i) {
      s->Block[i].x = tetris_blocks[s->cF][s->cS][i].x + 4;
      s->Block[i].y = tetris_blocks[s->cF][s->cS][i].y;
      s->client[s->Block[i].x][s->Block[i].y] = TRUE;
    }
    s->S = Random(4);
    s->F = Random(BLOCKSTYLES);
    for (i = 0; i < 4; ++i) {
      s->NextBlock[i].x = tetris_blocks[s->F][s->S][i].x;
      s->NextBlock[i].y = tetris_blocks[s->F][s->S][i].y;
    }
    s->start_time = dtime();
  }
  s->pause = FALSE; //暂停
  if (1) {
    ISIZE sz = imuimeasureText("H", NULL, io->FontID, io->layoutflags, NULL);
    x = (NUMCOLUMNBLOCKS + 10) * BLOCKWIDTH;
    y = (NUMLINEBLOCKS + 3) * BLOCKWIDTH;
    if (!calc_rect(x, y, io->layoutflags, &rc)) {
      return 0;
    }
    m_text_height = sz.h + 2;
  }
  if (1) {
    double d = dtime();
    if (d - s->start_time > 0.1) {
      s->start_time = d;
      s->game_time++;
      if (s->stage == GAME_RUN && 0 == (s->game_time) % 5) {
        if (s->pause) {
          return 0;
        }
        if (CanDown(s, s->Block)) {
          Down(s, s->Block);
        }
        else {
          //不能下移，需要处理消行判断（结合分数），还需要处理下一个显示，和当前显示的方块
          DelSqure(s);
          for (i = 0; i < 4; ++i) {
            s->Block[i].x = s->NextBlock[i].x + 4;
            s->Block[i].y = s->NextBlock[i].y;
            if (s->client[s->Block[i].x][s->Block[i].y]) {
              s->stage = GAME_STOP;
            }
            else {
              s->client[s->Block[i].x][s->Block[i].y] = TRUE;
            }
          }
          s->cS = s->S;
          s->cF = s->F;
          s->S = Random(4);
          s->F = Random(BLOCKSTYLES);
          for (i = 0; i < 4; ++i) {
            s->NextBlock[i].x = tetris_blocks[s->F][s->S][i].x;
            s->NextBlock[i].y = tetris_blocks[s->F][s->S][i].y;
          }
        }
      }
    }
  }
  if (ISHOVEREDWIN2()) {
    switch (io->lastMsg) {
    case MSG_KEYDOWN:
      if (s->pause && io->lastChar != VK_PAUSE) {
      }
      else {
        switch (io->lastChar) {
        case VK_LEFT:
          if (CanLeft(s, s->Block)) {
            Left(s, s->Block);
          }
          force_redraw(c);
          break;
        case VK_RIGHT:
          if (CanRight(s, s->Block)) {
            Right(s, s->Block);
          }
          force_redraw(c);
          break;
        case VK_UP:
          printf("111111111\n");
          if (CanChange(s, s->Block)) {
            Change(s, s->Block);
          }
          printf("2222222222\n");
          force_redraw(c);
          break;
        case VK_DOWN:
          while (CanDown(s, s->Block)) {
            Down(s, s->Block);
          }
          force_redraw(c);
          break;
        case VK_PAUSE:
          s->pause = !s->pause;
          break;
        default:
          break;
        }
      }
      break;
    case MSG_CHAR:
      if (io->lastChar == 'p') {
        s->pause = !s->pause;
      }
      else if (io->lastChar == 'r') {
        s->score = 0;
        for (x = 0; x < NUMCOLUMNBLOCKS; ++x) {
          for (y = 0; y < NUMLINEBLOCKS; ++y) {
            s->client[x][y] = FALSE;
          }
        }
        s->cS = Random(4);
        s->cF = Random(BLOCKSTYLES);
        for (i = 0; i < 4; ++i) {
          s->Block[i].x = tetris_blocks[s->cF][s->cS][i].x + 4;
          s->Block[i].y = tetris_blocks[s->cF][s->cS][i].y;
          s->client[s->Block[i].x][s->Block[i].y] = TRUE;
        }
        s->S = Random(4);
        s->F = Random(BLOCKSTYLES);
        for (i = 0; i < 4; ++i) {
          s->NextBlock[i].x = tetris_blocks[s->F][s->S][i].x;
          s->NextBlock[i].y = tetris_blocks[s->F][s->S][i].y;
        }
        s->pause = FALSE;
        force_redraw(c);
      }
    }
  }
  if (1) {
    //if(pause) return 0;
    char szBufferScore[32];
    int fmt = TF_CENTER;
    IRECT rc1 = iRECT2(BLOCKWIDTH, BLOCKWIDTH, NUMCOLUMNBLOCKS * BLOCKWIDTH, NUMLINEBLOCKS * BLOCKWIDTH);
    ImDraw_edgeRect1(ctx, rc, Color_White, 0, 0);
    //画俄罗斯方块游戏的边框
    ImDraw_edgeRect1(ctx, rc1, Color_White, Color_Black, 1);
    //输出“下一个”字符串
    rc1 = iRECT2((NUMCOLUMNBLOCKS + 2) * BLOCKWIDTH, BLOCKWIDTH, 400, 300);
    gcSolidText(ctx, rc1, "下一个：", 0, io->FontID, fmt, Color_Black);
    //输出“得分”字符串
    _snprintf(szBufferScore, 32, "得分： %d", s->score);
    rc1 = iRECT2((NUMCOLUMNBLOCKS + 2) * BLOCKWIDTH, 6 * BLOCKWIDTH, 400, 300);
    gcSolidText(ctx, rc1, szBufferScore, 0, io->FontID, fmt, Color_Black);
    //显示游戏区的方块
    for (x = 0; x < NUMCOLUMNBLOCKS; ++x) {
      for (y = 0; y < NUMLINEBLOCKS; ++y) {
        if (s->client[x][y]) {
          rc1 = iRECT2((x + 1) * BLOCKWIDTH, (y + 1) * BLOCKWIDTH, BLOCKWIDTH, BLOCKWIDTH);
          ImDraw_edgeRect1(ctx, rc1, Color_Black, 0, 0);
        }
      }
    }
    //显示下一个方块区域的方块
    for (i = 0; i < 4; ++i) {
      rc1 = iRECT2((s->NextBlock[i].x + NUMCOLUMNBLOCKS + 2) * BLOCKWIDTH, (s->NextBlock[i].y + 2) * BLOCKWIDTH, BLOCKWIDTH, BLOCKWIDTH);
      ImDraw_edgeRect1(ctx, rc1, Color_Black, 0, 0);
    }
  }
  return 0;
}

