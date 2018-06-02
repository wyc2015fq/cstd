
#include "img/filter_rect.inl"

int levelOM(const uchar* im, uchar* fim, uchar* ft)
{
  int buf1[360] = {0};
  int i;
  const int kk[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 16, 54, 128, 215, 256, 215, 128, 54, 16, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
    , 0, 1, 3, 7, 12, 14, 12, 7, 3, 1, 0, -2, -6, -14, -24, -28, -24, -14, -6, -2, 0, 1, 3, 7, 12, 14, 12, 7, 3, 1, 0
  };
  int nk = countof(kk) / 2;
  filter_rect(IRIS_H, IRIS_W, im, IRIS_W, 1, fim, IRIS_W, 1, buf1, nk, kk, nk, kk + nk, 8);

  for (i = 0; i < IRIS_H * IRIS_W; ++i) {
    //ft[i] = fim[i] < 128 ? 255:0;
  }

  return 0;
}
