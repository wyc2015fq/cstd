
//#include "../math/math2d.inl"
//#include "../math/imath.inl"

int pupil_to_polar_coordinate(buf_t* bf, const img_t* I, int x0, int y0, int Rmax, img_t* polarv)
{
  // 定义内圆 pupil极坐标图像大小 与 相位间距（分360份）
  int i, j, row, col, np = 360;
  row = I->h, col = I->w;

  bf_imsetsize(bf, polarv, np, Rmax, 1, 1);

  for (i = 0; i < np; ++i) {
    for (j = 0; j < Rmax; ++j) {
#if 0
      {
        uchar* pos;
        double x, y, theta = (-i + 90) * PI / 180;
        double e, f;
        int x1, y1, x2, y2;
        // 求直角坐标中对应的点
        x = x0 + j * cos(theta);
        y = y0 + j * sin(theta);

        if (x < 0) {
          x = 0;
        }
        else if (x >= col) {
          x = col - 1;
        }

        if (y < 0) {
          y = 1;
        }
        else if (y >= row) {
          y = row - 1;
        }

        //在直角坐标中的周围四点
        x1 = (int)floor(x);
        y1 = (int)floor(y);
        x2 = x1 + 1;
        y2 = y1 + 1;
        pos = I->tt.data + y1 * I->s + x1;
        // 处理越界情况
        value_yue_jie(x1, x2, y1, y2, col, row);
        //在X和Y方向分别进行线性插值
        e = pos[0] * (x - x1) + pos[1] * (x2 - x);
        f = pos[I->s] * (x - x1) + pos[I->s + 1] * (x2 - x);
        polarv->tt.data[polarv->s * i + j] = (int)(e * (y - y1) + f * (y2 - y));
      }
#else
      {

        const uchar* pos;
        int x, y, theta = (-i + 90 + 360) % 360;
        int e, f, x1, y1, x2, y2;
        // 求直角坐标中对应的点
        x = (x0 << 16) + j * cos360[(theta)];
        y = (y0 << 16) + j * sin360[(theta)];

        if (x < 0) {
          x = 0;
        }
        else if (x > ((col - 1) << 16)) {
          x = (col - 1) << 16;
        }

        if (y < 0) {
          y = 0;
        }
        else if (y > (row - 1) << 16) {
          y = (row - 1) << 16;
        }

        //在直角坐标中的周围四点
        x1 = (x) >> 16;
        y1 = (y) >> 16;
        x2 = x1 + 1;
        y2 = y1 + 1;
        pos = I->tt.data + y1 * I->s + x1;
        // 处理越界情况
        value_yue_jie(x1, x2, y1, y2, col, row);
        //在X和Y方向分别进行线性插值
        e = (pos[0] * (x & 0xffff) + pos[1] * ((1 << 16) - (x & 0xffff))) >> 16;
        f = (pos[I->s] * (x & 0xffff) + pos[I->s + 1] * (0xffff - (x & 0xffff))) >> 16;
        polarv->tt.data[polarv->s * i + j] = (e * (y & 0xffff) + f * (0xffff - (y & 0xffff))) >> 16;
      }
#endif
    }
  }

  //imshow(polarv); cvWaitKey(-1);
  return 0;
}
