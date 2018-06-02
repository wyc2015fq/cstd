
// PhotoShop算法原理解析系列 - 风格化---》查找边缘。
// Sobel边缘算子的结果进行反色即可
int FindEdges(int h, int w, const unsigned char* a, int al, unsigned char* b, int bl)
{
  int i, j;
  enum {bufsize = 65025};
  static unsigned char SqrValue[bufsize];
  static int SqrValue_inited = 0;

  if (0 == SqrValue_inited) {
    SqrValue_inited = 1;

    for (i = 0; i < bufsize; ++i) {
      SqrValue[i] = (int)sqrt(i);
    }
  }

  for (i = 0; i < h; ++i) {
    const unsigned char* a1 = a + i * al;
    const unsigned char* a0 = a1 + MAX(i - 1, 0) * al;
    const unsigned char* a2 = a1 + MIN(i + 1, h - 1) * al;

    for (j = 0; j < w; ++j) {
      int j1 = j;
      int j0 = MIN(j - 1, 0);
      int j2 = MIN(j + 1, w - 1);
      //   -1 0 1      1  2  1
      //Gx=-2 0 2  Gy= 0  0  0
      //   -1 0 1     -1 -2 -1
      int Gx = a0[j2] - a0[j0] + a2[j2] - a2[j0] + ((a1[j2] - a1[j0]) << 1);
      int Gy = a0[j0] - a2[j0] + a0[j2] - a2[j2] + ((a0[j1] - a2[j1]) << 1);
      int G, G_2 = Gx * Gx + Gy * Gy;
      G_2 = MIN(G_2, bufsize - 1);
      G = SqrValue[G_2];
    }
  }

  return 0;
}
