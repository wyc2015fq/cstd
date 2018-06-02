// 深度图简易填充
// 以深度图中0值点为中心，划取(2*BRsize+1) x (2*BRsize+1)大小的块，求取其中非零均值作为中心点的深度值
// DI : 原始深度图
// BRsize : 块半径，这里限制了块大小，所以BRsize应该小于或者等于12
// 对于某些边角空洞，可能无法填充
// edit time: 2013.07.05
#define DEPTH_TYPE unsigned short
int depthIrepair(int h, int w, const DEPTH_TYPE* DI, int DI_step, int BRsize,
    DEPTH_TYPE* RI, int RI_step)
{
  int kstep[1024], kn = 0, i, j, k, n = h * w;

  if (BRsize > 12) {
    ASSERT("块大小超过程序所设定的上限，块大小应该小于或者等于12");
  }

  if (BRsize < 0) {
    ASSERT("块大小不符合要求，块大小应该大于0");
  }

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      RI[i * RI_step + j] = DI[i * DI_step + j];
    }
  }

  for (i = 0; i < (2 * BRsize + 1); ++i) {
    for (j = 0; j < (2 * BRsize + 1); ++j) {
      kstep[kn] = (i - BRsize) * RI_step + (j - BRsize);
      ++kn;
    }
  }

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      int pos = i * RI_step + j;
      int t = RI[pos], tn = 0;

      if (0 == t || t < 33) {
        for (k = 0; k < kn; ++k) {
          int pos1 = pos + kstep[k];

          if (pos1 >= 0 && pos1 < n && RI[pos1] > 33) {
            t += RI[pos1];
            ++tn;
          }
        }

        if (tn) {
          t /= tn;
        }

        RI[pos] = t;
      }
    }
  }

  return 0;
}
