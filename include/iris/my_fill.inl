// 2011_7_20 盛先
// 填充二值化图像 当像素为0时，其四周均有一的存在则将其填充为1
int my_fill(buf_t* bf, const img_t* I_bw, img_t* I)
{
  // 空洞填充
  // 先找连通域，然后连通域内的点如果有0点，则填充？？
  // I2 =[0     0     0     0     0;
  //      0     1     1     1     0;
  //      0     1     0     1     0;
  //      0     1     1     1     0;
  //      0     0     0     0     0];
  int i, ii, jj, row, col;
  img_t I_ind[1] = {0};
  int h = I_bw->h, w = I_bw->w;

  bf_imsetsize(bf, I, h, w, 1, 1);
  bf_imsetsize(bf, I_ind, h, w, 1, 1);
  my_connect_domain(I_bw, I_ind);
  imclone(I_bw, I);
  row = I_bw->h, col = I_bw->w;

  for (ii = 2; ii < row - 1; ++ii) {
    //uchar* bw = I_bw->tt.data + ii * I_bw->s;

    for (jj = 2; jj < col - 1; ++jj) {
#if 0

      if (!I_bw->tt.data[ii * I_bw->s + jj]) {
        int left = -1, right = 1, top = -I_ind->s, bottom = I_ind->s;
        uchar* pos = I_ind->tt.data + ii * I_ind->s + jj;
        FINDFIRSTNOZERO(jj - 2, pos, -1, left);
        FINDFIRSTNOZERO(col - jj - 2, pos, 1, right);
        FINDFIRSTNOZERO(ii - 2, pos, -I_ind->s, top);
        FINDFIRSTNOZERO(row - ii - 2, pos, I_ind->s, bottom);

        if (pos[right] && (pos[left] == pos[right]) && (pos[left] == pos[top]) && (pos[left] == pos[bottom])) {
          I->tt.data[ii * I->s + jj] = 255;
        }
      }

#else
      uchar* pos = I_ind->tt.data + ii * I_ind->s;
      uchar* pos2 = I->tt.data + ii * I->s;
      int id = pos[jj];

      if (id) {
        for (i = col - 1; i > jj; --i) {
          if (pos[i] == id) {
            break;
          }
        }

        memset(pos2 + jj, 255, i - jj);
      }

#endif
    }
  }

  bf_imfree(bf, I_ind);
  return 0;
}
