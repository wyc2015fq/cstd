
// cantor三分集
int imdraw_fractal_canto(IMARGDEF, double x1, double y1, double x2, double y2, COLOR _clr)
{
  double _x0, _y0;
  double c = 10;

  if ((x2 - x1) < c) {
    TMoveTo(im, x1, y1);
    _LineTo(im, x2, y2);
  }
  else {
    double x3, y3, x4, y4;
    TMoveTo(im, x1, y1);
    _LineTo(im, x2, y2);
    x3 = x1 + (x2 - x1) / 3;
    y3 = y1 + 50;
    x4 = x2 - (x2 - x1) / 3;
    y4 = y2 + 50;
    y1 = y1 + 50;
    y2 = y2 + 50;
    imdraw_fractal_canto(im, x1, y1, x3, y3, _clr);
    imdraw_fractal_canto(im, x4, y4, x2, y2, _clr);
  }

  return 0;
}
// Koch曲线
int imdraw_fractal_koch(IMARGDEF, double ax, double ay, double bx, double by, COLOR _clr)
{
  double _x0, _y0;
  double c = 10;

  if (((bx - ax) * (bx - ax) + (by - ay) * (by - ay)) < c) {
    TMoveTo(im, ax, ay);
    _LineTo(im, bx, by);
  }
  else {
    double cx, cy, dx, dy, ex, ey, l, alfa;
    cx = ax + (bx - ax) / 3;
    cy = ay + (by - ay) / 3;
    ex = bx - (bx - ax) / 3;
    ey = by - (by - ay) / 3;
    imdraw_fractal_koch(im, ax, ay, cx, cy, _clr);
    imdraw_fractal_koch(im, ex, ey, bx, by, _clr);
    l = sqrt((ex - cx) * (ex - cx) + (ey - cy) * (ey - cy));
    alfa = atan((ey - cy) / (ex - cx));

    if ((alfa >= 0 && (ex - cx) < 0) || (alfa < 0 && (ex - cx) < 0)) {
      alfa = alfa + PI;
    }

    dx = cx + cos(alfa + PI / 3) * l;
    dy = cy + sin(alfa + PI / 3) * l;
    imdraw_fractal_koch(im, cx, cy, dx, dy, _clr);
    imdraw_fractal_koch(im, dx, dy, ex, ey, _clr);
  }

  return 0;
}

// Koch曲线
int vcgen_fractal_koch_recursive(DVC* vc, double x1, double y1, double x2, double y2, int level)
{
  double c = 10;
  
  if (level<=0 || ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) < c) {
    dVC_add(vc, x2, y2);
  }
  else {
    double x3, y3, x4, y4, x5, y5, l, alfa;
    x3 = x1 + (x2 - x1) / 3;
    y3 = y1 + (y2 - y1) / 3;
    x5 = x2 - (x2 - x1) / 3;
    y5 = y2 - (y2 - y1) / 3;
    l = sqrt((x5 - x3) * (x5 - x3) + (y5 - y3) * (y5 - y3));
    alfa = atan((y5 - y3) / (x5 - x3));
    
    if ((alfa >= 0 && (x5 - x3) < 0) || (alfa < 0 && (x5 - x3) < 0)) {
      alfa = alfa + PI;
    }
    
    x4 = x3 + cos(alfa + PI / 3) * l;
    y4 = y3 + sin(alfa + PI / 3) * l;
    --level;
    vcgen_fractal_koch_recursive(vc, x1, y1, x3, y3, level);
    vcgen_fractal_koch_recursive(vc, x3, y3, x4, y4, level);
    vcgen_fractal_koch_recursive(vc, x4, y4, x5, y5, level);
    vcgen_fractal_koch_recursive(vc, x5, y5, x2, y2, level);
  }
  
  return 0;
}

int vcgen_fractal_koch(DPOINT* pt, int maxn, double x1, double y1, double x2, double y2) {
  DVC vc[1] = {0};
  int level = (int)(log(maxn-1)/log(4));
  dVC_init(vc, pt, maxn);
  dVC_add(vc, x1, y1);
  vcgen_fractal_koch_recursive(vc, x1, y1, x2, y2, level);
  return vc->n;
}


void imdraw_fractal_arboresent_recursion(IMARGDEF, double x, double y, double size, double prate, double alfa, double theta, double t, double level, COLOR _clr)
{
  double _x0, _y0;
  double xa, ya;
  double xb, yb;
  double xc, yc;
  double xd, yd;
  double xe, ye;
  double l;
  l = size / prate;
  xe = x;
  ye = y;
  xa = x + size * cos(alfa);
  ya = y + size * sin(alfa);
  xb = x + l * cos(alfa - theta);
  yb = y + l * sin(alfa - theta);
  xc = x + l * cos(alfa - theta - t);
  yc = y + l * sin(alfa - theta - t);
  xd = x + size * cos(alfa - theta * 2 - t);
  yd = y + size * sin(alfa - theta * 2 - t);

  if (level <= 1) {
    TMoveTo(im, xa, ya);
    _LineTo(im, xe, ye);
    _LineTo(im, xd, yd);
    _LineTo(im, xc, yc);
    _LineTo(im, xe, ye);
    _LineTo(im, xb, yb);
    _LineTo(im, xa, ya);
    return;
  }

  imdraw_fractal_arboresent_recursion(im, xb, yb, l, prate, alfa - theta + PI, theta, t, level - 1, _clr);
  imdraw_fractal_arboresent_recursion(im, xc, yc, l, prate, alfa - theta - t + PI, -theta, -t, level - 1, _clr);
}
// Arboresent肺
int imdraw_fractal_arboresent(IMARGDEF, double x0, double y0, double x1, double y1, COLOR _clr)
{
  double prate;
  double theta;
  double alfa;
  double t;
  int level;
  double size;
  double x = x0, y = y0;
  prate = 1.5;
  theta = atan(sqrt(4 / prate / prate - 1));
  t = PI - 4 * theta;
  alfa = atan2(y1 - y0, x1 - x0);
  level = 7;
  size = calc_distance(x0, y0, x1, y1);
  imdraw_fractal_arboresent_recursion(im, x, y, size, prate, alfa, theta, t, level, _clr);
  return 0;
}
#define PTCENTOR(a, b) dPOINT((a.x+b.x)/2, (a.y+b.y)/2)
#define sqr(x)  ((x)*(x))
// Sierpinski三角形
int imdraw_fractal_sierpinski(IMARGDEF, DPOINT a, DPOINT b, DPOINT c, COLOR _clr)
{
  int C = 100;
  double _x0, _y0;
  DPOINT _a, _b, _c;

  if ((sqr(a.x - b.x) + sqr(a.y - b.y)) < C) {
    return 0;
  }

  _a = (PTCENTOR(a, b));
  _b = (PTCENTOR(a, c));
  _c = (PTCENTOR(b, c));
  TMoveTo(im, _a.x, _a.y);
  _LineTo(im, _b.x, _b.y);
  _LineTo(im, _c.x, _c.y);
  _LineTo(im, _a.x, _a.y);
  imdraw_fractal_sierpinski(im, a, dPOINT((a.x + b.x) / 2, (a.y + b.y) / 2), dPOINT((a.x + c.x) / 2, (a.y + c.y) / 2), _clr);
  imdraw_fractal_sierpinski(im, dPOINT((a.x + b.x) / 2, (a.y + b.y) / 2), b, dPOINT((b.x + c.x) / 2, (b.y + c.y) / 2), _clr);
  imdraw_fractal_sierpinski(im, dPOINT((a.x + c.x) / 2, (a.y + c.y) / 2), dPOINT((b.x + c.x) / 2, (b.y + c.y) / 2), c, _clr);
  return 0;
}
// Sierpinski地毯
int imdraw_fractal_sierpinski_sier(IMARGDEF, double x1, double y1, double x2, double y2, COLOR _clr)
{
  double W, L, t, d;
  MINMAX(x1, x2, t);
  MINMAX(y1, y2, t);
  d = calc_distance(x1, y1, x2, y2);

  if (d > 10) {
    L = (x2 - x1) / 3;
    W = (y2 - y1) / 3;
    imdrawaa_rect(im, (x1 + L), (y1 + W), (x2 - L), (y2 - W), _clr, 0, 0);
    imdraw_fractal_sierpinski_sier(im, x1, y1, x1 + L, y1 + W, _clr);
    imdraw_fractal_sierpinski_sier(im, x1 + L, y1, x2 - L, y1 + W, _clr);
    imdraw_fractal_sierpinski_sier(im, x2 - L, y1, x2, y1 + W, _clr);
    imdraw_fractal_sierpinski_sier(im, x1, y1 + W, x1 + L, y2 - W, _clr);
    imdraw_fractal_sierpinski_sier(im, x2 - L, y1 + W, x2, y2 - W, _clr);
    imdraw_fractal_sierpinski_sier(im, x1, y2 - W, x1 + L, y2, _clr);
    imdraw_fractal_sierpinski_sier(im, x1 + L, y2 - W, x2 - L, y2, _clr);
    imdraw_fractal_sierpinski_sier(im, x2 - L, y2 - W, x2, y2, _clr);
  }

  return 0;
}

#define randomize() srand((size_t)time(0))
#define random(x) (rand()%(x))
int imdraw_leaf1(IMARGDEF, double x1, double y1, double x2, double y2, COLOR color)
{
  // {(unsigned char)color.val[0], (unsigned char)color.val[1], (unsigned char)color.val[2], (unsigned char)color.val[3]};
  int i, ran_number = 90;
  double cx = x2 - x1, cy = y2 - y1;
  double a, b, c, d, e, f;
  double x, y, x_pre, y_pre;
  double disp_x, disp_y;
  x = y = x_pre = y_pre = 0;
  randomize();

  for (i = 0; i < 1000; ++i) {
    ran_number = random(100) + 1;

    if (ran_number == 1) {
      a = b = c = e = f = 0, d = 0.15;
    }
    else if (ran_number > 1 && ran_number <= 86) {
      a = 0.87, b = 0.014, c = - 0.014, d = 0.87, e = 0, f = 1.6;
    }
    else if (ran_number > 86 && ran_number <= 93) {
      a = 0.26, b = 0.472, c = 0.772, d = 0.34, e = 0, f = 1.6;
    }
    else {
      a = 0.28, b = 0.867, c = - 0.478, d = 0.4, e = 0, f = 0.44;
    }

    x = a * x_pre * cos(b) - d * sin(c) * y_pre + e;
    y = c * x_pre * sin(b) + d * cos(c) * y_pre + f;
    x_pre = x;
    y_pre = y;
    disp_x = (0.35 * x + 1.) * cx + x1;
    disp_y = (1. - 0.08 * y) * cy + y1;
    _SetPixel(im, (int)disp_x, (int)disp_y, color);
  }
  return 0;
}

// 正叶线
int imdraw_fractal_leaf(IMARGDEF, double x0, double y0, COLOR clr)
{
  double _x0, _y0;
  double a, n, cx, cy, gx, gy, flag, k;
  double r, p, th, x, y, pi;
  a = 160;
  n = 2;
  pi = 3.14159;
  cx = x0;
  cy = y0;
  flag = 0;
  k = 200;

  for (p = 1; p >= 0.2; p -= 0.2) {
    for (th = 0; th <= 2 * pi + 0.1; th += pi / k) {
      r = fabs((a * cos(n * th) * p));
      x = (r * cos(th));
      y = (r * sin(th));
      gx = (cx + x);
      gy = (cy + y);

      if (flag == 0) {
        _x0 = gx, _y0 = gy;
      }

      flag = 1;
      imdrawaa_line(im, _x0, _y0, gx, gy, clr, 1);
      _x0 = gx, _y0 = gy;
    }

    flag = 0;
  }

  return 0;
}

// 蝴蝶结
int imdraw_fractal_rose(IMARGDEF, double x, double y, COLOR clr)
{
  double _x0, _y0;
  double d, x1, x2, y1, y2;
  double a, e;
  d = 80;

  for (a = 0; a <= 2 * PI; a += PI / 360) {
    e = d * (1 + 0.25 * sin(4 * a));
    e = e * (1 + sin(8 * a));
    x1 = (x + e * cos(a));
    x2 = (x + e * cos(a + PI / 8)); //相对x1旋转π/8
    y1 = (y + e * sin(a));
    y2 = (y + e * sin(a + PI / 8)); //相对y1旋转π/8
    TMoveTo(im, x1, y1);
    TLineTo(im, x2, y2, clr);
  }

  return 0;
}
// couch曲线
int imdraw_fillzzz(IMARGDEF, COLOR clr)
{
  int width = im->w / 2, height = im->h / 2;
  int x, y;
  static int zoom = 0;
  zoom++;
  zoom = zoom % 29;

  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      long g = x * x + y * y;
      g = g * g;
      g = g >> zoom;
      g = g & 0xFf;

      if (g & 0x80)
        // if (g > 0x7f) g = 0x7f - g;
      {
        g = 0x7f ^ g;
      }

      g = g << 1;
      //g = g > 255? 255 : g;
      _SetPixel(im, width + x, height + y, g);
      _SetPixel(im, width - x, height + y, g);
      _SetPixel(im, width + x, height - y, g);
      _SetPixel(im, width - x, height - y, g);
    }
  }

  return 0;
}

int imdraw_fractal_couch(IMARGDEF, double x1, double y1, double x2, double y2, int n, COLOR clr)
{
  //im是画图的设备上下文的指针
  //x1,y1,x2,y2是起始的两点
  //其中参数n是递归的层数
  double x3, y3, x4, y4, x5, y5;
  double _x0, _y0;
  //以下是根据空间几何计算出来的坐标
  x3 = x1 + (x2 - x1) / 3;
  y3 = y1 + (y2 - y1) / 3;
  x4 = x1 + (x2 - x1) * 2 / 3;
  y4 = y1 + (y2 - y1) * 2 / 3;
  x5 = x3 + (x4 - x3) / 2 + (sqrt(3) * (y4 - y3) / 2);
  y5 = y3 - (sqrt(3) * (x4 - x3) / 2) + (y4 - y3) / 2;

  //递归最后一层,递归的出口
  if (n == 1) {
    TMoveTo(im, x1, y1);
    TLineTo(im, x3, y3, clr);
    TLineTo(im, x5, y5, clr);
    TLineTo(im, x4, y4, clr);
    TLineTo(im, x2, y2, clr);
  }
  else {
    //递归画图
    imdraw_fractal_couch(im, x1, y1, x3, y3, n - 1, clr);
    imdraw_fractal_couch(im, x3, y3, x5, y5, n - 1, clr);
    imdraw_fractal_couch(im, x5, y5, x4, y4, n - 1, clr);
    imdraw_fractal_couch(im, x4, y4, x2, y2, n - 1, clr);
  }

  return 0;
}
//斯宾斯基篓垫的画法
int imdraw_fractal_seripinski_floor(img_t* im, int x1, int y1, int x2, int y2, int x3, int y3, int n, COLOR clr)
{
  //im是画图的设备上下文的指针
  //x1,y1,x2,y2,x3,y3是起始的三角形的三点坐标
  //其中参数n是递归的层数
  int x11, x22, x33, y11, y22, y33;
  int _x0, _y0;
  //以下是根据空间几何计算出来的坐标
  x11 = (x2 + x3) / 2;
  y11 = (y2 + y3) / 2;
  x22 = (x1 + x3) / 2;
  y22 = (y1 + y3) / 2;
  x33 = (x1 + x2) / 2;
  y33 = (y1 + y2) / 2;
  TMoveTo(im, x11, y11);
  TLineTo(im, x22, y22, clr);
  TMoveTo(im, x11, y11);
  TLineTo(im, x33, y33, clr);
  TMoveTo(im, x22, y22);
  TLineTo(im, x33, y33, clr);

  //递归最后一层,递归的出口
  if (n == 1) {
    TMoveTo(im, x11, y11);
    TLineTo(im, x22, y22, clr);
    TLineTo(im, x33, y33, clr);
    TLineTo(im, x11, y11, clr);
  }
  else {
    //递归画图
    imdraw_fractal_seripinski_floor(im, x1, y1, x33, y33, x22, y22, n - 1, clr);
    imdraw_fractal_seripinski_floor(im, x33, y33, x2, y2, x11, y11, n - 1, clr);
    imdraw_fractal_seripinski_floor(im, x22, y22, x11, y11, x3, y3, n - 1, clr);
  }
  return 0;
}
//分形矩形的画法
int imdraw_fractal_rect(img_t* im, int x1, int y1, int x2, int y2, int n, COLOR clr)
{
  //im是画图的设备上下文的指针
  //x1,y1,x2,y2是起始矩形坐标
  //其中参数n是递归的层数
  int x3, y3, x4, y4, x5, y5, x6, y6;
  int _x0, _y0;
  //以下是根据空间几何计算出来的坐标
  x3 = x1 + (x2 - x1) / 3;
  y3 = y1 + (y2 - y1) / 3;
  x4 = x1 + (x2 - x1) * 2 / 3;
  y4 = y1 + (y2 - y1) * 2 / 3;
  x5 = x3 + (y4 - y3);
  y5 = y3 - (x4 - x3);
  x6 = x4 - (y3 - y4);
  y6 = y4 + (x3 - x4);
  TMoveTo(im, x1, y1);
  TLineTo(im, x3, y3, clr);
  TMoveTo(im, x4, y4);
  TLineTo(im, x2, y2, clr);

  //递归最后一层,递归的出口
  if (n == 1) {
    TMoveTo(im, x1, y1);
    TLineTo(im, x3, y3, clr);
    TLineTo(im, x5, y5, clr);
    TLineTo(im, x6, y6, clr);
    TLineTo(im, x4, y4, clr);
  }
  else {
    //递归画图
    imdraw_fractal_rect(im, x3, y3, x5, y5, n - 1, clr);
    imdraw_fractal_rect(im, x5, y5, x6, y6, n - 1, clr);
    imdraw_fractal_rect(im, x6, y6, x4, y4, n - 1, clr);
  }
  return 0;
}
//分形树的画法，其中参数n是递归的层数
int imdraw_FractalTree(img_t* im, double x1, double y1, double x2, double y2, int n, COLOR clr)
{
  //im是画图的设备上下文的指针
  //x1,y1,x2,y2是起始矩形坐标
  //其中参数n是递归的层数
  double x3, y3, x4, y4, x5, y5;
  double _x0, _y0;
  //以下是根据空间几何计算出来的坐标
  x3 = x1 + (x2 - x1) / 3;
  y3 = y1 + (y2 - y1) / 3;
  x4 = x3 + ((x1 - x3) * cos(5 * PI / 6)) - ((y1 - y3) * sin(5 * PI / 6));
  y4 = y3 + ((x1 - x3) * sin(5 * PI / 6)) + ((y1 - y3) * cos(5 * PI / 6));
  x5 = x3 + ((x1 - x3) * cos(5 * PI / 6)) + ((y1 - y3) * sin(5 * PI / 6));
  y5 = y3 - ((x1 - x3) * sin(5 * PI / 6)) + ((y1 - y3) * cos(5 * PI / 6));
  TMoveTo(im, x1, y1);
  TLineTo(im, x2, y2, clr);

  //递归最后一层,递归的出口
  if (n == 1) {
    TMoveTo(im, x3, y3);
    TLineTo(im, x4, y4, clr);
    TMoveTo(im, x3, y3);
    TLineTo(im, x5, y5, clr);
  }
  else {
    //递归画图
    imdraw_FractalTree(im, x3, y3, x2, y2, n - 1, clr);
    imdraw_FractalTree(im, x3, y3, x4, y4, n - 1, clr);
    imdraw_FractalTree(im, x3, y3, x5, y5, n - 1, clr);
  }
  return 0;
}
// cantor三分集
int imdraw_FractalCanto(img_t* im, int ax, int ay, int bx, int by, COLOR clr)
{
  int c = 10;
  int _x0, _y0;

  if ((bx - ax) < c) {
    TMoveTo(im, ax, ay);
    TLineTo(im, bx, by, clr);
  }
  else {
    int cx, cy, dx, dy;
    TMoveTo(im, ax, ay);
    TLineTo(im, bx, by, clr);
    cx = ax + (bx - ax) / 3;
    cy = ay + 50;
    dx = bx - (bx - ax) / 3;
    dy = by + 50;
    ay = ay + 50;
    by = by + 50;
    imdraw_FractalCanto(im, ax, ay, cx, cy, clr);
    imdraw_FractalCanto(im, dx, dy, bx, by, clr);
  }
  return 0;
}
// Koch曲线
int imdraw_FractalKoch(img_t* im, double ax, double ay, double bx, double by, COLOR clr)
{
  double c = 100;
  double _x0, _y0;

  if (((bx - ax) * (bx - ax) + (by - ay) * (by - ay)) < c) {
    TMoveTo(im, ax + 100, 600 - ay);
    TLineTo(im, bx + 100, 600 - by, clr);
  }
  else {
    double cx, cy, dx, dy, ex, ey, l, alfa;
    cx = ax + (bx - ax) / 3;
    cy = ay + (by - ay) / 3;
    ex = bx - (bx - ax) / 3;
    ey = by - (by - ay) / 3;
    imdraw_FractalKoch(im, ax, ay, cx, cy, clr);
    imdraw_FractalKoch(im, ex, ey, bx, by, clr);
    l = sqrt((ex - cx) * (ex - cx) + (ey - cy) * (ey - cy));
    alfa = atan((ey - cy) / (ex - cx));

    if ((alfa >= 0 && (ex - cx) < 0) || (alfa < 0 && (ex - cx) < 0)) {
      alfa = alfa + PI;
    }

    dx = cx + cos(alfa + PI / 3) * l;
    dy = cy + sin(alfa + PI / 3) * l;
    imdraw_FractalKoch(im, cx, cy, dx, dy, clr);
    imdraw_FractalKoch(im, dx, dy, ex, ey, clr);
  }
  return 0;
}
//Koch雪花
int imdraw_FractalSnow(img_t* im, double ax, double ay, double bx, double by, COLOR clr)
{
  int c = 100;
  double _x0, _y0;

  if (((bx - ax) * (bx - ax) + (by - ay) * (by - ay)) < c) {
    TMoveTo(im, (ax), (ay));
    TLineTo(im, (bx), (by), clr);
  }
  else {
    double cx, cy, dx, dy, ex, ey, l, alfa;
    cx = ax + (bx - ax) / 3;
    cy = ay + (by - ay) / 3;
    ex = bx - (bx - ax) / 3;
    ey = by - (by - ay) / 3;
    imdraw_FractalSnow(im, ax, ay, cx, cy, clr);
    imdraw_FractalSnow(im, ex, ey, bx, by, clr);
    l = sqrt((ex - cx) * (ex - cx) + (ey - cy) * (ey - cy));
    alfa = atan((ey - cy) / (ex - cx));

    if ((alfa >= 0 && (ex - cx) < 0) || (alfa < 0 && (ex - cx) < 0)) {
      alfa = alfa + PI;
    }

    dx = cx + cos(alfa + PI / 3) * l;
    dy = cy + sin(alfa + PI / 3) * l;
    imdraw_FractalSnow(im, cx, cy, dx, dy, clr);
    imdraw_FractalSnow(im, dx, dy, ex, ey, clr);
  }
  return 0;
}
// Arboresent肺
int imdraw_FractalArboresent1(img_t* im, double x, double y, double size, double prate, double alfa, double theta, double t, double level, COLOR clr)
{
#define L_X 200
#define L_Y 230
  double xe, ye;
  double xa, ya;
  double xb, yb;
  double xc, yc;
  double xd, yd;
  double l;
  double _x0, _y0;
  l = size / prate;
  xe = x;
  ye = y;
  xa = x + size * cos(alfa);
  ya = y + size * sin(alfa);
  xb = x + l * cos(alfa - theta);
  yb = y + l * sin(alfa - theta);
  xc = x + l * cos(alfa - theta - t);
  yc = y + l * sin(alfa - theta - t);
  xd = x + size * cos(alfa - theta * 2 - t);
  yd = y + size * sin(alfa - theta * 2 - t);

  if (level <= 1) {
    TMoveTo(im, L_X - xa / 15, L_Y - ya / 15);
    TLineTo(im, L_X - xe / 15, L_Y - ye / 15, clr);
    TLineTo(im, L_X - xd / 15, L_Y - yd / 15, clr);
    TLineTo(im, L_X - xc / 15, L_Y - yc / 15, clr);
    TLineTo(im, L_X - xe / 15, L_Y - ye / 15, clr);
    TLineTo(im, L_X - xb / 15, L_Y - yb / 15, clr);
    TLineTo(im, L_X - xa / 15, L_Y - ya / 15, clr);
    return 0;
  }

  imdraw_FractalArboresent1(im, xb, yb, l, prate, alfa - theta + PI, theta, t, level - 1, clr);
  imdraw_FractalArboresent1(im, xc, yc, l, prate, alfa - theta - t + PI, - theta, - t, level - 1, clr);
#undef L_X
#undef L_Y
  return 0;
}
int imdraw_FractalArboresent(img_t* im, COLOR clr)
{
  double prate;
  double theta;
  double alfa;
  double t;
  int level;
  double size;
  double x, y;
  prate = 1.5;
  theta = atan(sqrt(4 / prate / prate - 1));
  t = PI - 4 * theta;
  alfa = 55 * PI / 180;
  level = 7;
  x = - 2500;
  y = 0;
  size = 3000;
  imdraw_FractalArboresent1(im, x, y, size, prate, alfa, theta, t, level, clr);
  imdraw_FractalArboresent1(im, x + size * prate * cos(alfa - theta - t), y + size * prate * sin(alfa - theta - t), size, prate, alfa + 2 * theta - t, - theta, - t, level, clr);
  return 0;
}
// Sierpinski地毯
int imdraw_fractal_sierpinski_sier2(img_t* im, double x1, double y1, double x2, double y2, int n, COLOR clr)
{
  double W, L;

  if (n > 1) {
    //int _x0, _y0;
    L = x2 - x1;
    W = y2 - y1;
    //TMoveTo(im,(x1+L/3),(y1+W/3));
    
    imdrawaa_rect(im, x2 - L / 3, y2 - W / 3, x1 + L / 3, y1 + W / 3, clr, 0, 0);
    
    imdraw_fractal_sierpinski_sier2(im, x1, y1, x1 + L / 3, y1 + W / 3, n - 1, clr);
    imdraw_fractal_sierpinski_sier2(im, x1 + L / 3, y1, x2 - L / 3, y1 + W / 3, n - 1, clr);
    imdraw_fractal_sierpinski_sier2(im, x2 - L / 3, y1, x2, y1 + W / 3, n - 1, clr);
    imdraw_fractal_sierpinski_sier2(im, x1, y1 + W / 3, x1 + L / 3, y2 - W / 3, n - 1, clr);
    imdraw_fractal_sierpinski_sier2(im, x2 - L / 3, y1 + W / 3, x2, y2 - W / 3, n - 1, clr);
    imdraw_fractal_sierpinski_sier2(im, x1, y2 - W / 3, x1 + L / 3, y2, n - 1, clr);
    imdraw_fractal_sierpinski_sier2(im, x1 + L / 3, y2 - W / 3, x2 - L / 3, y2, n - 1, clr);
    imdraw_fractal_sierpinski_sier2(im, x2 - L / 3, y2 - W / 3, x2, y2, n - 1, clr);
  }
  return 0;
}

typedef struct _DrawFractalPeanoInfo {
  double cx, cy; //初始点坐标
  int n; //递归深度
  int len; //单位线段长度
  double CurrentX, CurrentY;
  img_t* im;
  COLOR clr;
} imdraw_FractalPeanoInfo;
int imdraw_fractal_peano_recursive(int k, int i, imdraw_FractalPeanoInfo* info)
{
  static const int a = 0, b = 1, c = 2, d = 3;
  int L = info->len;
  //Peano-Hilbert曲线的四种状态之一的递归调用
  int tt[40] = { d, a, a, b, - L, 0, 0, L, L, 0, c, b, b, a, 0, - L, L, 0, 0, L, b, c, c, d, L, 0, 0, - L, - L, 0, a, d, d, c, 0, L, - L, 0, 0, - L,};
  int* t = tt + k * 10;
  
#define plot() TDrawLine(info->im, info->CurrentX,info->CurrentY,info->cx,info->cy, info->clr), info->CurrentX = info->cx, info->CurrentY = info->cy
  if (i > 0) {
    imdraw_fractal_peano_recursive(t[0], i - 1, info);
    info->cx += t[4];
    info->cy += t[5];
    plot();
    imdraw_fractal_peano_recursive(t[1], i - 1, info);
    info->cx += t[6];
    info->cy += t[7];
    plot();
    imdraw_fractal_peano_recursive(t[2], i - 1, info);
    info->cx += t[8];
    info->cy += t[9];
    plot();
    imdraw_fractal_peano_recursive(t[3], i - 1, info);
  }

  return 0;
}
#undef plot
// peano曲线
int imdraw_fractal_peano(IMARGDEF, double x, double y, int len, int n, COLOR clr)
{
  imdraw_FractalPeanoInfo info[1] = {0};
  info->cx = x;
  info->cy = y;
  info->len = len;
  info->CurrentX = info->cx;
  info->CurrentY = info->cy;
  info->im = im;
  info->clr = clr;
  imdraw_fractal_peano_recursive(0, n, info);
  return 0;
}
//C曲线递归过程
int imdraw_fractal_c_curve(img_t* im, int n, double x1, double y1, double x2, double y2, COLOR clr)
{
  double x3, y3;
  double _x0, _y0;

  if (n > 0) {
    x3 = (x1 + y1 + x2 - y2) / 2;
    y3 = (x2 + y2 + y1 - x1) / 2;
    imdraw_fractal_c_curve(im, n - 1, x1, y1, x3, y3, clr);
    imdraw_fractal_c_curve(im, n - 1, x3, y3, x2, y2, clr);
    TMoveTo(im, x1, y1);
    TLineTo(im, x2, y2, clr);
  }
  return 0;
}
//分形树
int imdraw_fractal_tree2(img_t* pdc, double px, double py, double ang, double l, short width, COLOR clr)
{
  static const double arg = 15 * (PI / 180); //偏移的角度
  static const int len = 75; //根父枝的长度
  static const double dt_x = 0.8; //子枝是父枝的倍数.由x,y合成
  static const double dt_y = 0.8;
  //static const double ang=90*(PI/180); //树的倾斜度
  static const short wid = 12;
  static double sqt;
  static int inited = 0;
  double rn = rand() % 10 * (PI / 180);
  INT rn2 = rand() % 9;
  double x, y;
  double _x0, _y0;

  if (rn2 > 5) {
    rn = 0;
  }

  if (0 == inited) {
    inited = 1;
    sqt = sqrt((dt_x * dt_x + dt_y * dt_y) / 2);
  }

  if (width < 1) {
    width = 1;
  }

  x = px + (l * cos(ang));
  y = py - (l * sin(ang));
  TMoveTo(pdc, px, py);
  TLineTo(pdc, x, y, clr);

  if (l < 20) {
    return 0;
  }

  imdraw_fractal_tree2(pdc, x, y, ang - arg + rn, l * sqt, width - 2, clr);
  imdraw_fractal_tree2(pdc, x, y, ang + arg + rn, l * sqt, width - 2, clr);
  return 0;
}
// 分形树
int imdraw_FractalTree3(img_t* dc, double x, double y, double angle, int length, int minlength, COLOR clr)
{
  double x1, y1;
  double _x0, _y0;

  if (length > minlength) {
    x1 = (cos(angle) * length + x);
    y1 = (sin(angle) * length + y);
    TMoveTo(dc, x, y);
    TLineTo(dc, x1, y1, clr);
    imdraw_FractalTree3(dc, (x + x1) / 2, (y1 + y) / 2, angle - PI / 8, length / 2, minlength, clr);
    imdraw_FractalTree3(dc, x1, y1, angle + PI / 8, length / 2, minlength, clr);
  }
  return 0;
}
int imdraw_FractalLeaf(img_t* dc, double x, double y, double L, double A, COLOR clr)
{
  double _x0, _y0;
  double B, C, s1, s2, s3;
  double x1, y1, x1L, y1L, x1R, y1R, x2, y2, x2R, y2R, x2L, y2L;
  B = 50 * PI / 180;
  C = 9 * PI / 180;
  s1 = 2;
  s2 = 3;
  s3 = 1.3;

  if (L > s1) {
    x2 = (x + L * cos(A));
    y2 = (y + L * sin(A));
    x2R = (x2 + L / s2 * cos((A + B)));
    y2R = (y2 + L / s2 * sin((A + B)));
    x2L = (x2 + L / s2 * cos((A - B)));
    y2L = (y2 + L / s2 * sin((A - B)));
    x1 = (x + L / s2 * cos(A));
    y1 = (y + L / s2 * sin(A));
    x1L = (x1 + L / s2 * cos((A - B)));
    y1L = (y1 + L / s2 * sin((A - B)));
    x1R = (x1 + L / s2 * cos((A + B)));
    y1R = (y1 + L / s2 * sin((A + B)));
    TMoveTo(dc, x, y);
    TLineTo(dc, x2, y2, clr);
    TLineTo(dc, x2R, y2R, clr);
    TMoveTo(dc, x2, y2);
    TLineTo(dc, x2L, y2L, clr);
    TMoveTo(dc, x1, y1);
    TLineTo(dc, x1L, y1L, clr);
    TMoveTo(dc, x1, y1);
    TLineTo(dc, x1R, y1R, clr);
    imdraw_FractalLeaf(dc, (x2), (y2), (L / s3), A + C, clr);
    imdraw_FractalLeaf(dc, (x2R), (y2R), (L / s2), A + B, clr);
    imdraw_FractalLeaf(dc, (x2L), (y2L), (L / s2), A - B, clr);
    imdraw_FractalLeaf(dc, (x1L), (y1L), (L / s2), A - B, clr);
    imdraw_FractalLeaf(dc, (x1R), (y1R), (L / s2), A + B, clr);
  }
  return 0;
}
int imdraw_FractalTree4_(img_t* dc, double x, double y, double L, double a, double tt, COLOR clr)
{
  int i;
  double X1, Y1;
  double _x0, _y0;

  for (i = - 1; i <= 1; i++) {
    X1 = (x + L * cos((a + i * (((rand() * 1.) / RAND_MAX * 40) + 20)) * PI / 180));
    Y1 = (y + L * sin((a + i * (((rand() * 1.) / RAND_MAX * 40) + 20)) * PI / 180));
    TMoveTo(dc, x, y);
    TLineTo(dc, X1, Y1, clr);

    if (L > 10) {
      imdraw_FractalTree4_(dc, X1, Y1, L - 8, (a + i * ((rand() * 1.) / RAND_MAX * 40) + 10), tt, clr);
    }
    else if (tt == 1) {
      // 画果子
      imdrawaa_ellipse(dc, X1, Y1, 2, 2, clr, 0, 0);
    }
  }
  return 0;
}
int imdraw_FractalTree4(img_t* dc, int x, int y, int L, int a, int tt, COLOR clr)
{
  int _x0, _y0;
  TMoveTo(dc, x, y);
  TLineTo(dc, x, y - 120, clr);
  imdraw_FractalTree4_(dc, x, y - 120, L, a, tt, clr);
  return 0;
}
typedef struct CKNOT {
  double x, y;
  double direction;
} CKNOT;
// L系统
//oang 初始角度
//angle 转角
//degree 迭代次数
// ox oy 原点坐标
// len 线段长度
int imdraw_Fractal_Lsystem(img_t* im, int ox, int oy, int len, double oang, double angle, int degree, char* unit0, char** unit, int m, COLOR clr)
{
  char a = 'F';
  char tree[102400];
  char formtree[102400];
  int i;
  CKNOT stack[1024];
  int stackpushpos = 0;
  int _x0, _y0;

  //unit[0] = "F";//初始公理
  //unit[1] = "F-F++F-F"; // 生成规则
  //unit[2] = "FF-[-F+F+F]+[+F-F-F]";
  //len =10; //线段长度
  // 初始化
  for (i = 0; i < 1024; i++) {
    stack[i].x = 0;
    stack[i].direction = 0;
  }

  stackpushpos = 0;
  tree[0] = 0;
  formtree[0] = 0;
  strcpy(formtree, unit0 ? unit0 : unit[rand() % m]); //公理

  for (i = 1; i <= degree; i++) {
    int curlen = strlen(formtree);
    int pos = 0, j = 0;

    while (j < curlen) {
      if (formtree[j] == a) {
        int k = rand() % m;
        strcat(tree, unit[k]);
        j++;
        pos = strlen(tree) - 1;
      }
      else {
        char aa[2] = {
          0
        };
        aa[0] = formtree[j], aa[1] = 0;
        strcat(tree, aa);
        pos++;
        j++;
      }
    }

    strcpy(formtree, tree);
    tree[0] = 0;
  }

  strcpy(tree, formtree);

  if (strlen(tree) <= 0) {
    return 0;
  }
  else {
    int i = 0;
    CKNOT mNextKnot, mCurKnot;
    mCurKnot.x = ox;
    mCurKnot.y = oy;
    mCurKnot.direction = oang;
    TMoveTo(im, ox, oy);

    while (i < (int)strlen(tree)) {
      switch (tree[i]) {
      case 'F':
        mNextKnot.x = mCurKnot.x + len * cos(mCurKnot.direction * PI / 180);
        mNextKnot.y = mCurKnot.y - len * sin(mCurKnot.direction * PI / 180);
        mNextKnot.direction = mCurKnot.direction;
        TLineTo(im, (int)mNextKnot.x, (int)mNextKnot.y, clr);
        mCurKnot = mNextKnot;
        break;

      case '[':
        stack[stackpushpos] = mCurKnot;
        stackpushpos++;
        break;

      case ']':
        mCurKnot = stack[stackpushpos - 1];
        stackpushpos--;
        TMoveTo(im, (int)mCurKnot.x, (int)mCurKnot.y);
        break;

      case '+':
        mCurKnot.direction = mCurKnot.direction + angle;
        break;

      case '-':
        mCurKnot.direction = mCurKnot.direction - angle;
        break;

      default:
        ;
      }

      i++;
    }
  }
  return 0;
}
// IFS系统
int imdraw_Fractal_Ifs(img_t* im)
{
  double x0, y0;
  double x = 0; // '仿射变换中的自变量
  double y = 0;
  double newx, newy; //'仿射变换产生的新点
  double a, b, c, d, e, f; //'仿射变幻中的系数
  long n = 100000; // '迭代次数
  double R; //'随机变量
  double m[7][7] = {0}; // '存放IFS码
  COLOR clr;
  //'IFS码赋值
  m[0][0] = 0.5f;
  m[0][1] = 0;
  m[0][2] = 0;
  m[0][3] = 0.5;
  m[0][4] = 0;
  m[0][5] = 0;
  m[0][6] = 0.333f;
  m[1][0] = 0.5f;
  m[1][1] = 0;
  m[1][2] = 0;
  m[1][3] = 0.5;
  m[1][4] = 0.5;
  m[1][5] = 0;
  m[1][6] = 0.333f;
  m[2][0] = 0.5f;
  m[2][1] = 0;
  m[2][2] = 0;
  m[2][3] = 0.5;
  m[2][4] = 0.25;
  m[2][5] = 0.5;
  m[2][6] = 0.334f;

  //'循环迭代，在不同的概率空间下，赋不同的IFS码值
  while (n > 0) {
    R = (double)rand() / RAND_MAX;

    if (R <= m[0][6]) {
      a = m[0][0];
      b = m[0][1];
      c = m[0][2];
      d = m[0][3];
      e = m[0][4];
      f = m[0][5];
    }
    else if (R <= m[0][6] + m[1][6]) {
      a = m[1][0];
      b = m[1][1];
      c = m[1][2];
      d = m[1][3];
      e = m[1][4];
      f = m[1][5];
    }
    else if (R <= m[0][6] + m[1][6] + m[2][6]) {
      a = m[2][0];
      b = m[2][1];
      c = m[2][2];
      d = m[2][3];
      e = m[2][4];
      f = m[2][5];
    }
    else {
      a = m[3][0];
      b = m[3][1];
      c = m[3][2];
      d = m[3][3];
      e = m[3][4];
      f = m[3][5];
    }

    newx = (a * x) + (b * y) + e;
    newy = (c * x) + (d * y) + f;
    x = newx;
    y = newy;
    clr = _RGB(x * 500 * R, R * 100, y * 2000 * R);
    x0 = (1000 + 7000 * x) / 15;
    y0 = (6500 - 6000 * y) / 15;
    _SetPixel(im, x0, y0, clr);
    n--;
  }
  return 0;
}
int test_drawLsystem(IMARGDEF, COLOR clr)
{
  int l = 0;
  char* uni[] = {
    "F-F++F-F", "F+F-F-FFF+F+F-F", "FF-F-F-F-F-F+F", "F[+F]F[-F]F", "F[+F]F[-F[+F]]", "FF-[-F+F+F]+[+F-F-F]"
  };
  imdraw_Fractal_Lsystem(im, 900, 450, 10, 180, 60, 6, "F", uni + l, 1, clr); //Koch曲线
  imdraw_Fractal_Lsystem(im, 900, 150, 5, 180, 60, 4, "F++F++F", uni + l, 1, clr); //Koch雪花
  imdraw_Fractal_Lsystem(im, 900, 450, 5, 180, 60, 4, "F--F--F", uni + l, 1, clr); //Koch反雪花
  ++l;
  imdraw_Fractal_Lsystem(im, 900, 250, 5, 180, 90, 3, "F+F+F+F", uni + l, 1, clr); //“止”曲线
  ++l;
  imdraw_Fractal_Lsystem(im, 900, 250, 5, 180, 90, 3, "F-F-F-F-", uni + l, 1, clr); //环形肠
  ++l;
  imdraw_Fractal_Lsystem(im, 450, 450, 3, 90, 30, 4, 0, uni + l++, 3, clr);
  return 0;
}
//serpirski曲线
int imdraw_Fractal_Serpirski(img_t* im, double a, double b, double c, double d, int n, int m, int r)
{
  static int p, q;
  double x, y, x0, y0;
  int k;
  COLOR clr = _RGB(0, 0, 250);

  for (p = 0; p < m - 1; p++) {
    x0 = a + (c - a) * p / m;

    for (q = 0; q < m - 1;) {
      y0 = b + (d - b) * q / m;
      x = x0;
      y = y0;

      for (k = 1; k < n; k++) {
        if (y > 0.5) {
          x = 2 * x;
          y = 2 * y - 1;
        }
        else if (x >= 0.5) {
          x = 2 * x - 1;
          y = 2 * y;
        }
        else {
          x = 2 * x;
          y = 2 * y;
        }

        if (x * x + y * y > r) {
          goto np;
        }
      }

      _SetPixel(im, 2 * p + 50, 2 * q + 50, clr); //修改值2，可得到不同的放大效果
np:
      q++;
    }

    //Sleep (SLEEPTIME) ;
  }
  return 0;
}

int test_drawfractal(IMARGDEF)
{
  enum {
    MAX_points = 40
  };
  int len, i = 0;
  DPOINT2 pt2[MAX_points] = { 100, 10, 10, 200, 100, 50, 300, 300 };
  COLOR clr = _RGB(255, 255, 255);
  RandPoints(pt2, MAX_points, im->w, im->h);
  len = MAX_points;
  test_drawLsystem(im, clr);
#if 0
  imdraw_Fractal_Ifs(im);
  imdraw_Fractal_Serpirski(im, 0, 0, 1, 1, 12, 200, 200); //
  imdraw_FractalPeano(im, pt2[i].x, pt2[i].y, 4, 6, clr);
  imdraw_FractalTree4(im, pt2[i].x, pt2[i].y, 50, - 90, 1, clr);
  imdraw_FractalTree3(im, 400, 600, - PI / 2, 300, 1, clr);
  //DrawFractalTree2(im,pt2[i].x, pt2[i].y, PI/2, 75, 12, clr);
  imdraw_FractalLeaf(im, pt2[i].x, pt2[i].y, 120, 270 * PI / 180, clr);
  imdraw_FractalC(im, 15, pt2[i], pt2[i + 1], clr);
  imdraw_fractal_sierpinski_sier2(im, pt2[i].x, pt2[i].y, pt2[i + 1].x, pt2[i + 1].y, 6, clr);
  i += 2;
  imdraw_FractalArboresent(im, clr);
  imdraw_FractalSnow(im, pt2[i].x, pt2[i].y, pt2[i + 1].x, pt2[i + 1].y, clr);
  imdraw_FractalCanto(im, 50, 100, 750, 100, clr);
  i += 2;
  imdraw_FractalKoch(im, 10, 300, 790, 300, clr);
  i += 2;
  imdraw_fractal_couch(im, pt2[i].x, pt2[i].y, pt2[i + 1].x, pt2[i + 1].y, 4, clr);
  i += 2;
  imdraw_FractalTree(im, pt2[i].x, pt2[i].y, pt2[i + 1].x, pt2[i + 1].y, 4, clr);
  i += 2;
  imdraw_fractal_rect(im, pt2[i].x, pt2[i].y, pt2[i + 1].x, pt2[i + 1].y, 4, clr);
  i += 2;
  imdraw_fractal_seripinski_floor(im, pt2[i].x, pt2[i].y, pt2[i + 1].x, pt2[i + 1].y, pt2[i + 2].x, pt2[i + 2].y, 4, clr);
  i += 3;
#endif
  return 0;
}
