

// 参数:
//   double* f        - 指向时域值的指针
//   double* F        - 指向频域值的指针
//   r                - 2的幂数
// 说明:
//   该函数用来实现快速沃尔什-哈达玛变换。
VOID WINAPI WALSH(const double* f, double* F, int r)
{
  // 沃尔什-哈达玛变换点数
  LONG  count;

  // 循环变量
  int   i, j, k;

  // 中间变量
  int   bfsize, p;

  double* X1, *X2, *X;

  // 计算快速沃尔什变换点数
  count = 1 << r;

  // 分配运算所需的数组
  X1 = new double[count];
  X2 = new double[count];

  // 将时域点写入数组X1
  memcpy(X1, f, sizeof(double) * count);

  // 蝶形运算
  for (k = 0; k < r; k++) {
    for (j = 0; j < 1 << k; j++) {
      bfsize = 1 << (r - k);

      for (i = 0; i < bfsize / 2; i++) {
        p = j * bfsize;
        X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
        X2[i + p + bfsize / 2] = X1[i + p] - X1[i + p + bfsize / 2];
      }
    }

    // 互换X1和X2
    X = X1;
    X1 = X2;
    X2 = X;
  }

  // 调整系数
  for (j = 0; j < count; j++) {
    p = 0;

    for (i = 0; i < r; i++) {
      if (j & (1 << i)) {
        p += 1 << (r - i - 1);
      }
    }

    F[j] = X1[p] / count;
  }

  // 释放内存
  delete X1;
  delete X2;
}


// 参数:
//   LPSTR lpDIBBits    - 指向源DIB图像指针
//   LONG  lWidth       - 源图像宽度（象素数）
//   LONG  lHeight      - 源图像高度（象素数）
//
// 返回值:
//   BOOL               - 成功返回TRUE，否则返回FALSE。
//
// 说明:
//   该函数用来对图像进行沃尔什-哈达玛变换。于上面不同的是，此处是将二维
// 矩阵转换成一个列向量，然后对该列向量进行一次一维沃尔什-哈达玛变换。
BOOL WINAPI DIBWalsh1(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
  // 指向源图像的指针
  unsigned char*  lpSrc;

  // 循环变量
  LONG  i;
  LONG  j;

  // 进行付立叶变换的宽度和高度（2的整数次方）
  LONG  w;
  LONG  h;

  // 中间变量
  double  dTemp;

  int   wp;
  int   hp;

  // 图像每行的字节数
  LONG  lLineBytes;

  // 计算图像每行的字节数
  lLineBytes = WIDTHBYTES(lWidth * 8);

  // 赋初值
  w = 1;
  h = 1;
  wp = 0;
  hp = 0;

  // 计算进行离散余弦变换的宽度和高度（2的整数次方）
  while (w * 2 <= lWidth) {
    w *= 2;
    wp++;
  }

  while (h * 2 <= lHeight) {
    h *= 2;
    hp++;
  }

  // 分配内存
  double* f = new double[w * h];
  double* F = new double[w * h];

  // 列
  for (i = 0; i < w; i++) {
    // 行
    for (j = 0; j < h; j++) {
      // 指向DIB第j行，第i个象素的指针
      lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;

      // 给时域赋值
      f[j + i * w] = *(lpSrc);
    }
  }

  // 调用快速沃尔什－哈达玛变换
  WALSH(f, F, wp + hp);

  // 列
  for (i = 0; i < w; i++) {
    // 行
    for (j = 0; j < h; j++) {
      // 计算频谱
      dTemp = fabs(F[i * w + j] * 1000);

      // 判断是否超过255
      if (dTemp > 255) {
        // 对于超过的，直接设置为255
        dTemp = 255;
      }

      // 指向DIB第j行，第i个象素的指针
      lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;

      // 更新源图像
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  //释放内存
  delete f;
  delete F;

  // 返回
  return TRUE;
}
