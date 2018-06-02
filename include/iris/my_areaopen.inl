
// 2011_7_20 盛先
// 寻找I_bw二值图像中为1的点的区域的个数以及大小8连通
// I_bw = [0 0 0 0 0 0 0 0;
//     0 1 0 1 1 0 0 0;
//     0 1 0 1 0 0 1 0;
//     0 0 1 0 0 1 1 0;
//     0 0 0 0 1 1 0 0;
//     0 0 0 0 0 0 0 0];
// AREA为设定的小于AREA的面积的连通域时 将之置为0
int my_areaopen(buf_t* bf, const img_t* I_bw, int maxarea, img_t* I)
{
  int h = I_bw->h, w = I_bw->w;
  int n = I_bw->h * I_bw->w;
  img_t I_ind[1] = {0};
  int i = 0, count[256] = {0};
  bf_imsetsize(bf, I, h, w, 1, 1);
  bf_imsetsize(bf, I_ind, h, w, 1, 1);
  my_connect_domain(I_bw, I_ind);
  imclone(I_bw, I);
#if 0
  max_ind = 0;
  MAX_ELEMENT(n, I_ind->tt.data, max_ind);
  max_ind = I_ind->tt.data[max_ind];

  // 找出每个连通域的面积大小
  for (ii = 1; ii <= max_ind; ++ii) {
    int area = 0;
    COUNT(n, I_ind->tt.data, ii, area);

    if (area <= maxarea) {
      //I(r,c) = 0;
      int jj = 0;

      for (; jj < n; ++jj) {
        if (I_ind->tt.data[jj] == ii) {
          I->tt.data[jj] = 0;
        }
      }
    }
  }

#else
  memset(count, 0, 256 * sizeof(count[0]));

  for (i = 0; i < n; ++i) {
    int id = I_ind->tt.data[i];
    count[id]++;
  }

  for (i = 0; i < 256; ++i) {
    count[i] = count[i] > maxarea ? 255 : 0;
  }

  count[0] = 0;

  for (i = 0; i < n; ++i) {
    int id = I_ind->tt.data[i];
    I->tt.data[i] = count[id];
  }

#endif
  bf_imfree(bf, I_ind);
  return 0;
}
