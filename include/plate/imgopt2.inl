
//#include "img/imgopt.inl"

// 8连通域寻找
int my_connect_domain(const img_t* I_bw, img_t* I_ind)
{
  int i, ii, jj, row, col, Top_left, ind;
  int ve[4];
  int vect[4];
  row = I_bw->h, col = I_bw->w;
  // 因为I_bw的周边行列均为0  这是前面算出的先验条件

  // 从左上角开始扫描
  imsetsize(I_ind, row, col, 1, 1);// 存储为1的标号
  ind = 1;
  ve[0] = -I_ind->s - 1, ve[1] = -I_ind->s, ve[2] = -I_ind->s + 1, ve[3] = -1;

  for (ii = 2; ii <= row - 1; ++ii) {
    for (jj = 2; jj <= col - 1; ++jj) {
      if (I_bw->tt.data[(ii - 1) * I_bw->s + jj - 1] == 255) {
        uchar* pos = I_ind->tt.data + (ii - 1) * I_ind->s + jj - 1;
        Top_left = pos[ve[0]] + pos[ve[1]] + pos[ve[2]] + pos[ve[3]];

        if (Top_left) {
          for (i = 0; i < 4; ++i) {
            vect[i] = pos[ve[i]];

            if (!vect[i]) {
              vect[i] = 255;
            }
          }

          *pos = MIN4(vect[0], vect[1], vect[2], vect[3]);

          if (255 == *pos) {
            int asdf = 0;
          }
        }
        else {
          *pos = ind;
          ind = ind + 1;
        }
      }
    }
  }

  //imshow(I_ind); cvWaitKey(-1);
  // 从右下角开始扫描
  ve[0] = I_ind->s - 1, ve[1] = I_ind->s, ve[2] = I_ind->s + 1, ve[3] = 1;

  for (ii = row - 1; ii >= 2; --ii) {
    for (jj = col - 1; jj >= 2; --jj) {
      if (I_bw->tt.data[(ii - 1) * I_bw->s + jj - 1] == 255) {
        uchar* pos = I_ind->tt.data + (ii - 1) * I_ind->s + jj - 1;
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
  return ind - 1;
}

int juxingkuang(const img_t* e, IRECT* pq, int maxpq)
{
  img_t I_ind[1] = {0};
  int geshu; //矩形框个数
  int row, col, ii, jj; //
  // 矩形框
  //对所有的非严格矩形使用最外接矩形框法获得每个矩形的四个坐标
  //输入： e 二值矩阵，白色区域为非严格矩形区域
  //输出： pq 坐标矩阵，大小为n*4，其中n为矩形个数，4表示矩形的上下左右四个坐标

  // 因为I_bw的周边行列均为0  这是前面算出的先验条件
  row = e->h, col = e->w;
  // 从左上角开始扫描
  geshu = my_connect_domain(e, I_ind);   // 存储为1的标号

  //获得了标记图片，标记的数值表示图像的矩形框个数
  if (geshu == 0) {
    //不存在矩形框
    imfree(I_ind);
    return 0;
  }

  geshu = MIN(geshu, maxpq);

  //方案二，略显复杂  两个方案都测试过了
  for (ii = 0; ii < maxpq; ii++) {
    pq[ii].t = pq[ii].l = 10000;
    pq[ii].b = pq[ii].r = 0;
  }

  for (ii = 2; ii < row; ++ii) {
    for (jj = 2; jj < col; ++jj) {
      int ch = I_ind->tt.data[(ii - 1) * I_ind->s + jj - 1] - 1;

      //由于扩大了边界，所以得到的坐标都要还原1个位置
      if (ch >= 0 && ch < maxpq) {
        pq[ch].l = MIN(pq[ch].l, jj - 2);
        pq[ch].t = MIN(pq[ch].t, ii - 2);
        pq[ch].r = MAX(pq[ch].r, jj);
        pq[ch].b = MAX(pq[ch].b, ii);
      }
    }
  }

  //挑出虚假的标记
  for (ii = 0, jj = 0; ii < geshu; ii++) {
    if (pq[ii].b > pq[ii].t && pq[ii].r > pq[ii].l) {
      pq[jj++] = pq[ii];
    }
  }

  geshu = jj;
  imfree(I_ind);
  return geshu;
}

int juxingkuang2(int h, int w, uchar* img, int al, int cn, CvRect* pq, int maxpq)
{
  img_t im[1] = {0};
  int i, ret = 0;
  IRECT* ss = (IRECT*)pq;
  IMINIT(im, h, w, img, al, cn, 1);
  ret = juxingkuang(im, ss, maxpq);

  for (i = 0; i < ret; ++i) {
    IRECT s0 = ss[i];
    pq[i] = cvRect(s0.l, s0.t, RCW(&s0), RCH(&s0));
  }

  return ret;
}