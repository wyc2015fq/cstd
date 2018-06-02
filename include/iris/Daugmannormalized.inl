
// nx, ny, nr 为对应的内圆的直角坐标以及其半径大小
// wx, wy, wr 为对应的外圆的直角坐标以及其半径大小
// v_normalize 为对应的内圆与外圆边缘的中间部分的极坐标图像，其中心点为？？

int Daugmannormalized(int* nc, int* wc, const img_t* I, const int* y_top, const int* y_bot, uchar* v_normalize)
{
  int nx = nc[0], ny = nc[1], nr = (nc[2] + 1);
  int wx = wc[0], wy = wc[1], wr = (wc[2] - 1);
  int i, j, h = I->h, w = I->w, xi, yi, xo, yo, ti, to, e, f;
  int x, y, x1, y1, s = I->s, cnt_zero = 0;
  short tti[IRIS_H] = {0};
  const uchar* ptr = 0;

  //bf_imsetsize(v_normalize, IRIS_H, IRIS_W, 1, 1);
  for (j = 0; j < IRIS_H; ++j) {
    tti[j] = (j << 8) / IRIS_H;
  }

  for (i = 0; i < IRIS_W; ++i) {
    int co = cos360[i], si = sin360[i];
    xi = nx + ((nr * co) >> 16);
    yi = ny + ((nr * si) >> 16);
    xo = wx + ((wr * co) >> 16);
    yo = wy + ((wr * si) >> 16);

    for (j = 0; j < IRIS_H; ++j) {
      int t = 0;
      ti = tti[j], to = 256 - tti[j];
      x = (to * xi + ti * xo) >> 8;
      y = (to * yi + ti * yo) >> 8;
      x1 = x >> 8, y1 = y >> 8;
      x1 = BOUND(x1, 0, w - 2);
      y1 = BOUND(y1, 0, h - 2);

      if (y_top[x1] <= y1 && y1 <= y_bot[x1]) {
        ptr = I->tt.data + I->s * y1 + x1;
        e = ptr[0] + (((ptr[1] - ptr[0]) * (uchar)(x & 0xff)) >> 8);
        f = ptr[s] + (((ptr[s + 1] - ptr[s]) * (uchar)(x & 0xff)) >> 8);
        t = e + (((f - e) * (y & 0xff)) >> 8);
        t = BOUND(t, 1, 255);
      }
      else {
        cnt_zero++;
      }

      v_normalize[IRIS_W * j + i] = t;
      //v_normalize->tt.data[v_normalize->s * j + i] = ptr[0];
    }
  }

  return cnt_zero;
}

int Daugmannormalized1(int* nc, int* wc, const img_t* I, img_t* v_normalize)
{
  int nx = nc[0], ny = nc[1], nr = nc[2] + 1;
  int wx = wc[0], wy = wc[1], wr = wc[2] - 1;
  int pic_ysize, pic_xsize, np, R, i, j;
  int xi[400] = {0};
  int yi[400] = {0};
  int xo[400] = {0};
  int yo[400] = {0};
  double i_pi, a_OIA, IO, a_OAI, a_IOA, a_AIO, a_IAO, a_AOI, IA, a, r;
  ASSERT(I->c == 1);
  pic_ysize = I->h, pic_xsize = I->w;
  np = IRIS_W;
  R = 30;     // R 代表把 虹膜分成多少份  4-19
  //v_normalize = zeros(R, np);
  imsetsize(v_normalize, R, np, 1, 1);
  //求瞳孔和虹膜边缘点
#define zeros(_X, _N)  FILL(_N+1, _X, 0)
  zeros(xi, np);
  zeros(yi, np);
  zeros(xo, np);
  zeros(yo, np);

  //第一种情况：ny = wy
  if ((ny == wy) && (nx == wx)) {
    for (i = 0; i < np; ++i) {
      i_pi = i * PI / 180;
      xi[i] = round(nx + nr * cos(i_pi));
      yi[i] = round(ny + nr * sin(i_pi));
      xo[i] = round(wx + wr * cos(i_pi));
      yo[i] = round(wy + wr * sin(i_pi));
    }
  }

  if ((ny == wy) && (nx < wx)) {
    for (i = 0; i < np; ++i) {
      i_pi = i * PI / 180;
      a_OIA = PI / 2 + i_pi;
      IO = sqrt(SQR(nx - wx) + SQR(ny - wy));
      a_OAI = asin(IO * sin(a_OIA) / wr);
      a_IOA = PI - a_OIA - a_OAI;
      IA = sqrt(SQR(wr) + SQR(IO) - 2 * wr * IO * cos(a_IOA));
      xi[i] = round(nx + nr * cos(i_pi));
      yi[i] = round(ny + nr * sin(i_pi));
      xo[i] = round(wx + IA * cos(i_pi));
      yo[i] = round(wy + IA * sin(i_pi));
    }
  }

  if ((ny == wy) && (nx > wx)) {
    for (i = 0; i < np; ++i) {
      i_pi = i * PI / 180;
      a_OIA = PI / 2 - i_pi;
      IO = sqrt(SQR(nx - wx) + SQR(ny - wy));
      a_OAI = asin(IO * sin(a_OIA) / wr);
      a_IOA = PI - a_OIA - a_OAI;
      IA = sqrt(SQR(wr) + SQR(IO) - 2 * wr * IO * cos(a_IOA));
      xi[i] = round(nx + nr * cos(i_pi));
      yi[i] = round(ny + nr * sin(i_pi));
      xo[i] = round(wx + IA * cos(i_pi));
      yo[i] = round(wy + IA * sin(i_pi));
    }
  }

  //第二种情况：(ny > wy) && (nx >= wx)
  if ((ny > wy) && (nx >= wx)) {
    for (i = 0; i < np; ++i) {
      i_pi = i * PI / 180;
      a = atan(fabs((nx - wx) / (ny - wy)));
      a_OIA = PI - a - i_pi;
      IO = sqrt(SQR(nx - wx) + SQR(ny - wy));
      a_OAI = asin(IO * sin(a_OIA) / wr);
      a_IOA = PI - a_OIA - a_OAI;
      IA = sqrt(SQR(wr) + SQR(IO) - 2 * wr * IO * cos(a_IOA));
      xi[i] = round(nx + nr * cos(i_pi));
      yi[i] = round(ny + nr * sin(i_pi));
      xo[i] = round(wx + IA * cos(i_pi));
      yo[i] = round(wy + IA * sin(i_pi));
    }
  }

  //第三种情况：(ny > wy) && (nx <= wx)
  if ((ny > wy) && (nx <= wx)) {
    for (i = 0; i < np; ++i) {
      i_pi = i * PI / 180;
      a = atan(fabs((nx - wx) / (ny - wy)));
      a_OIA = PI - i_pi + a;
      a_OAI = asin(nr * sin(a_OIA) / wr);
      a_IOA = PI - a_OIA - a_OAI;
      IO = sqrt(SQR(nx - wx) + SQR(ny - wy));
      IA = sqrt(SQR(IO) + SQR(wr) - 2 * wr * IO * cos(a_IOA));
      xi[i] = round(nx + nr * cos(i_pi));
      yi[i] = round(ny + nr * sin(i_pi));
      xo[i] = round(wx + IA * cos(i_pi));
      yo[i] = round(wy + IA * sin(i_pi));
    }
  }

  //第四种情况：(ny < wy) && (nx >= wx)
  if ((ny < wy) && (nx >= wx)) {
    for (i = 0; i < np; ++i) {
      i_pi = i * PI / 180;
      a = atan(fabs((nx - wx) / (ny - wy)));
      a_AIO = i_pi - a;
      IO = sqrt(SQR(nx - wx) + SQR(ny - wy));
      a_IAO = asin(IO * sin(a_AIO) / wr);
      a_AOI = PI - a_AIO - a_IAO;
      IA = sqrt(SQR(IO) + SQR(wr) - 2 * IO * wr * cos(a_AOI));
      xi[i] = round(nx + nr * cos(i_pi));
      yi[i] = round(ny + nr * sin(i_pi));
      xo[i] = round(wx + IA * cos(i_pi));
      yo[i] = round(wy + IA * sin(i_pi));
    }
  }

  //第五种情况：(ny < wy) && (nx <= wx)
  if ((ny < wy) && (nx <= wx)) {
    for (i = 0; i < np; ++i) {
      i_pi = i * PI / 180;
      a = atan(fabs((nx - wx) / (ny - wy)));
      a_AIO = a + i_pi;
      IO = sqrt(SQR(nx - wx) + SQR(ny - wy));
      a_IAO = asin(IO * sin(a_AIO) / wr);
      a_IOA = PI - a_AIO - a_IAO;
      IA = sqrt(SQR(IO) + SQR(wr) - 2 * wr * IO * cos(a_IOA));
      xi[i] = round(nx + nr * cos(i_pi));
      yi[i] = round(ny + nr * sin(i_pi));
      xo[i] = round(wx + IA * cos(i_pi));
      yo[i] = round(wy + IA * sin(i_pi));
    }
  }

  for (i = 0; i < R; ++i) {
    r = 1.*i / (R);

    for (j = 0; j < np; ++j) {
      double x, y, e, f;
      int x1, y1, x2, y2;
      const uchar* ptr;
      //求直角坐标中对应的点
      x = (1 - r) * xi[j] + r * xo[j];
      y = (1 - r) * yi[j] + r * yo[j];
      //在直角坐标中的周围四点
      x1 = (int)floor(x);
      y1 = (int)floor(y);
      x2 = x1 + 1;
      y2 = y1 + 1;

      //处理越界情况
      x1 = BOUND(x1, 0, pic_xsize - 2);
      x2 = BOUND(x2, 1, pic_xsize - 1);
      y1 = BOUND(y1, 0, pic_ysize - 2);
      y2 = BOUND(y2, 1, pic_ysize - 1);
      ptr = I->tt.data + I->w * y1 + x1;

      //在X和Y方向分别进行线性插值
      e = ptr[0] * (x2 - x) + ptr[1] * (x - x1);
      f = ptr[I->w] * (x2 - x) + ptr[I->w + 1] * (x - x1);
      v_normalize->tt.data[v_normalize->w * i + j] = (int)(e * (y2 - y) + f * (y - y1));
    }
  }

  //imshow(v_normalize); cvWaitKey(-1);
  //显示极坐标下的图像
  // v_normalize = mat2gray(v_normalize);
  // figure;imshow(v_normalize);
  return 0;
}
