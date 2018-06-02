//力学：流体（Liquid）（VC版本）

//这个程序原本是测试 HTML 5 效果的一个演示程序，动态效果很是漂亮。现在，这个精彩的程序经 krissi 移植到了 vc 上。

///////////////////////////////////////////////////
// 程序名称：流体(Liquid)
// 编译环境：Visual C++ 6.0 / 2010，EasyX 20120404(beta)
// 原 作 品：http://spielzeugz.de/html5/liquid-particles.html (HTML5)
// 移植作者：krissi <zh@easyx.cn>
// 最后修改：2012-4-5
//

#define NUM_MOVERS  600     // 小球数量
#define FRICTION  0.96f   // 摩擦力


// 定义小球结构
typedef struct Mover {
  COLORREF  color;      // 颜色
  double   x,  y;      // 坐标
  double   vX, vY;     // 速度
} Mover;

// 定义全局变量
Mover movers[NUM_MOVERS];     // 小球数组
int   mouseX,   mouseY;     // 当前鼠标坐标
int   mouseVX,  mouseVY;    // 鼠标速度
int   prevMouseX, prevMouseY;   // 上次鼠标坐标
BOOL  isMouseDown;        // 鼠标左键是否按下


// 初始化
int init_Liquid()
{
  int i;
  img_t im[1] = {0};
  GetImageBuffer(im);

  // 初始化小球数组
  for (i = 0; i < NUM_MOVERS; i++) {
    movers[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
    movers[i].x   = im->w * 0.5;
    movers[i].y   = im->h * 0.5;
    movers[i].vX  = (double)(cos((double)(i))) * (rand() % 34);
    movers[i].vY  = (double)(sin((double)(i))) * (rand() % 34);
  }

  // 初始化鼠标变量
  mouseX = prevMouseX = im->w / 2;
  mouseY = prevMouseY = im->h / 2;

  // 设置随机种子
  srand((unsigned int)time(NULL));
  return 0;
}

// 绘制动画(一帧)
int run()
{
  int i;
  double toDist, stirDist, blowDist;
  img_t im[1] = {0};
  DWORD*  pBuffer;
  // 获取显存指针
  pBuffer = GetImageBuffer(im);

  // 全屏变暗 50%
  for (i = im->w * im->h - 1; i >= 0; i--) {
    if (pBuffer[i] != 0) {
      pBuffer[i] = _RGB(GetRV(pBuffer[i]) >> 1, GetGV(pBuffer[i]) >> 1, GetBV(pBuffer[i]) >> 1);
    }
  }

  mouseVX    = mouseX - prevMouseX;
  mouseVY    = mouseY - prevMouseY;
  prevMouseX = mouseX;
  prevMouseY = mouseY;

  toDist   = im->w * 0.86f;
  stirDist = im->w * 0.125f;
  blowDist = im->w * 0.5f;

  for (i = 0; i < NUM_MOVERS; i++) {
    double avgVX, avgVY, avgV, sc, x, y, vX, vY, dX, dY, d, nextX, nextY;
    x  = movers[i].x;
    y  = movers[i].y;
    vX = movers[i].vX;
    vY = movers[i].vY;

    dX = x - mouseX;
    dY = y - mouseY;
    d  = (double)sqrt(dX * dX + dY * dY);
    dX = d ? dX / d : 0;
    dY = d ? dY / d : 0;

    if (isMouseDown && d < blowDist) {
      double blowAcc = (1 - (d / blowDist)) * 14;
      vX += dX * blowAcc + 0.5f - (double)(rand()) / RAND_MAX;
      vY += dY * blowAcc + 0.5f - (double)(rand()) / RAND_MAX;
    }

    if (d < toDist) {
      double toAcc = (1 - (d / toDist)) * im->w * 0.0014f;
      vX -= dX * toAcc;
      vY -= dY * toAcc;
    }

    if (d < stirDist) {
      double mAcc = (1 - (d / stirDist)) * im->w * 0.00026f;
      vX += mouseVX * mAcc;
      vY += mouseVY * mAcc;
    }

    vX *= FRICTION;
    vY *= FRICTION;

    avgVX = fabs(vX);
    avgVY = fabs(vY);
    avgV  = (avgVX + avgVY) * 0.5f;

    if (avgVX < 0.1) {
      vX *= (double)(rand()) / RAND_MAX * 3;
    }

    if (avgVY < 0.1) {
      vY *= (double)(rand()) / RAND_MAX * 3;
    }

    sc = avgV * 0.45f;
    sc = max(min(sc, 3.5f), 0.4f);

    nextX = x + vX;
    nextY = y + vY;

    if (nextX > im->w) {
      nextX = im->w;
      vX *= -1;
    }
    else if (nextX < 0)   {
      nextX = 0;
      vX *= -1;
    }

    if (nextY > im->h) {
      nextY = im->h;
      vY *= -1;
    }
    else if (nextY < 0)   {
      nextY = 0;
      vY *= -1;
    }

    movers[i].vX = vX;
    movers[i].vY = vY;
    movers[i].x  = nextX;
    movers[i].y  = nextY;

    // 画小球
    setcolor(movers[i].color);
    setfillcolor(movers[i].color);
    fillcircle((nextX + 0.5), (nextY + 0.5), (sc + 0.5));
  }

  return 0;
}


// 主函数
int test_liquid()
{
  // 鼠标消息变量
  MOUSEMSG m;

  // 创建绘图窗口
  initgraph(640, 480, 0);
  // 启用批绘图模式
  BeginBatchDraw();

  // 初始化
  init_Liquid();

  while (1) {
    // 处理鼠标消息
    if (GetMouseMsg(&m, 20)) {
      switch (m.uMsg) {
      case WM_MOUSEMOVE:
        mouseX = m.x;
        mouseY = m.y;
        break;

      case WM_LBUTTONDOWN:
        isMouseDown = TRUE;
        break;

      case WM_LBUTTONUP:
        isMouseDown = FALSE;
        break;
      }
    }

    // 绘制动画
    run();

    // 显示缓存的绘制内容
    FlushBatchDraw();
  }

  // 关闭
  EndBatchDraw();
  closegraph();
}