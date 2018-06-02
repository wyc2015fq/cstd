
// http://zh.wikipedia.org/wiki/%E6%9E%81%E5%9D%90%E6%A0%87%E7%B3%BB

double RandomX(double low, double high)
{
  return ((rand() / ((double)(RAND_MAX) + 1.0)) * (high - low) + low);
}

int vcgen_rand_line(int n, DPOINT2* pt, double x1, double y1, double x2, double y2, double error)
{
  int i;
  double x;

  for (i = 0; i < n; ++i) {
    pt[i].x = x = RandomX(x1, x2);
    pt[i].y = (x - x1) * (y2 - y1) / (x2 - x1) + y1 + RandomX(-error, error);
  }

  return 0;
}

int vcgen_rand_circle(int n, DPOINT2* pt, double centrex, double centrey, double radius, double error)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = RandomX(0, 2 * PI);
    r = RandomX(radius - error, radius + error);
    pt[i].x = centrex + r * cos(t);
    pt[i].y = centrey + r * sin(t);
  }

  return 0;
}

int vcgen_rand_ellipse(int n, DPOINT2* pt, double centrex, double centrey, double a, double b, double error)
{
  double rotation = RandomX(0, 2 * PI);
  double sinr = sin(rotation), cosr = cos(rotation);
  int i;
  double t, x, y;

  for (i = 0; i < n; ++i) {
    t = RandomX(0, 2 * PI);
    x = RandomX(a - error, a + error) * cos(t);
    y = RandomX(b - error, b + error) * sin(t);
    pt[i].x = cosr * x - sinr * y;
    pt[i].y = sinr * x + cosr * y;
  }

  return 0;
}

//玫瑰线  r(θ) = a sin kθ
//极坐标的玫瑰线（polar rose）是数学曲线中非常著名的曲线，看上去像花瓣，它只能用极坐标方程来描述
int vcgen_polar_rose(int n, DPOINT2* pt, double a, int k)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    r = a * sin(k * i);
    pt[i].x = r * cos(t);
    pt[i].y = r * sin(t);
  }

  return n;
}

//阿基米德螺线(archimedean spiral)  r(θ) = a + bθ
// 改变参数a将改变螺线形状，b控制螺线间距离，通常其为常量。阿基米德螺线有两条螺线，
// 一条θ > 0，另一条θ < 0。两条螺线在极点处平滑地连接。把其中一条翻转 90°/270°得到其镜像，就是另一条螺线
int vcgen_archimedean_spiral(int n, DPOINT2* pt, double a, double b)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    r = a + b * t;
    pt[i].x = r * cos(t);
    pt[i].y = r * sin(t);
  }

  return 0;
}

// 双曲螺线（Hyperbolic spiral）又称倒数螺线（reciprocal spiral）
int vcgen_hyperbolic_spiral(int n, DPOINT2* pt, double c)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    r = c / t;
    pt[i].x = r * cos(t);
    pt[i].y = r * sin(t);
  }

  return 0;
}

// 内摆线（圆内螺线）
// 其中 k 为正实数
int vcgen_hypocycloid_spiral(int n, DPOINT2* pt, double k)
{
  int i;
  double t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    pt[i].x = cos(t) + cos(k * t) / k;
    pt[i].y = sin(t) - sin(k * t) / k;
  }

  return 0;
}
// 费马螺线是螺线的一种，表达式：r^2 = θ
int vcgen_fermat_spiral(int n, DPOINT2* pt, double a, double b)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    r = sqrt(t);
    pt[i].x = r * cos(t);
    pt[i].y = r * sin(t);
  }

  return 0;
}

//圆锥曲线
//圆锥曲线方程：r = l / (1 - e cos theta)
//其中l表示半正焦弦，e表示离心率。 如果e < 1，曲线为椭圆，如果e = 1，曲线为抛物线，如果e > 1，则表示双曲线。
int vcgen_conic(int n, DPOINT2* pt, double l, double e)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    r = l / (1 - e * cos(t));
    pt[i].x = r * cos(t);
    pt[i].y = r * sin(t);
  }

  return 0;
}


