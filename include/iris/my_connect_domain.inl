
// 8连通域寻找
int my_connect_domain(const img_t* I_bw, img_t* I_ind)
{
  int i, ii, jj, row, col, Top_left, ind;
  int ve[4];
  int vect[4];
  row = I_bw->h, col = I_bw->w;
  // 因为I_bw的周边行列均为0  这是前面算出的先验条件

  // 从左上角开始扫描
  ind = 1;
  ve[0] = -I_ind->s - 1, ve[1] = -I_ind->s, ve[2] = -I_ind->s + 1, ve[3] = -1;

  for (ii = 2; ii < row - 1; ++ii) {
    for (jj = 2; jj < col - 1; ++jj) {
      if (I_bw->tt.data[ii * I_bw->s + jj] == 255) {
        uchar* pos = I_ind->tt.data + ii * I_ind->s + jj;
        Top_left = pos[ve[0]] + pos[ve[1]] + pos[ve[2]] + pos[ve[3]];

        if (Top_left) {
          for (i = 0; i < 4; ++i) {
            vect[i] = pos[ve[i]];

            if (!vect[i]) {
              vect[i] = 255;
            }
          }

          *pos = MIN4(vect[0], vect[1], vect[2], vect[3]);
        }
        else {
          *pos = ind;
          ind = ind + 1;
        }
      }
    }
  }

  // 从右下角开始扫描
  ve[0] = I_ind->s - 1, ve[1] = I_ind->s, ve[2] = I_ind->s + 1, ve[3] = 1;

  for (ii = row - 2; ii > 1; --ii) {
    for (jj = col - 2; jj > 1; --jj) {
      if (I_bw->tt.data[ii * I_bw->s + jj] == 255) {
        uchar* pos = I_ind->tt.data + ii * I_ind->s + jj;
        //Top_left = I_ind(ii+1,jj-1) + I_ind(ii+1,jj) + I_ind(ii+1,jj+1) + I_ind(ii,jj+1);
        Top_left = pos[ve[0]] + pos[ve[1]] + pos[ve[2]] + pos[ve[3]];

        if (Top_left) {
          for (i = 0; i < 4; ++i) {
            vect[i] = pos[ve[i]];

            if (!vect[i]) {
              vect[i] = 255;
            }
          }

          *pos = MIN4(vect[0], vect[1], vect[2], vect[3]);
        }
      }
    }
  }

  //FPRINT2D("aaa.txt", "%1d,", "\r\n", row, col, I_ind->tt.data, col, 1);
  return ind;
}
