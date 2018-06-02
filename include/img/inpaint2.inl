

// A 原图 R 掩码 C 输出
int inpaint2(int h, int w, const unsigned char* A, int al, const unsigned char* R, int rl, unsigned char* C, int cl)
{
  int j, k, x, y, xl, xr, yt, yd;
  int a1, a2, a3, a4;

  for (y = 0; y < h; ++y) {
    for (x = 0; x < w; ++x) {
      int x0 = x, y0 = y;
      C[y0 * cl + x0] = A[y0 * al + x0];

      if (0 == R[y * rl + x]) {
        continue;
      }

      for (j = x0 - 1; j >= 0; j--) {
        if ((R[y0 * rl + j] == 0) && (R[y0 * rl + j + 1] == 0) && (R[y0 * rl + j + 2] == 1)) {
          xl = j;
          break;
        }
        else {
          continue;
        }
      }

      for (j = x0 + 1; j < w; ++j) {
        if ((R[y0 * rl + j] == 0) && (R[y0 * rl + j - 1] == 0) && (R[y0 * rl + j - 2] == 1)) {
          xr = j;
          break;
        }
        else {
          continue;
        }
      }

      for (k = y0 - 1; k >= 0; --k) {
        if ((R[k * rl + x0] == 0) && (R[(k + 1)*rl + x0] == 0) && (R[(k + 2)*rl + x0] == 1)) {
          yt = k;
          break;
        }
        else {
          continue;
        }
      }

      for (k = y0 + 1; k < h; ++k) {
        if ((R[k * rl + x0] == 0) && (R[(k - 1)*rl + x0] == 0) && (R[(k - 2)*rl + x0] == 1)) {
          yd = k;
          break;
        }
        else {
          continue;
        }
      }

      a1 = A[y0 * al + xl];
      a2 = A[y0 * al + xr];
      a3 = A[yt * al + x0];
      a4 = A[yd * al + x0];
      C[y0 * cl + x0] = (a1 * (xr - x0) + a2 * (x0 - xl)) / (2 * (xr - xl)) + (a3 * (yd - y0) + a4 * (y0 - yt)) / (2 * (yd - yt));
      //C[y0 * cl + x0] = MIN4(a1, a2, a3, a4);
    }
  }

  return 0;
}

// 用上下左右四个边界像素的最小值填充
int inpaint2_min(int h, int w, const unsigned char* A, int al, const unsigned char* R, int rl, unsigned char* C, int cl)
{
  int j, k, x, y, xl, xr, yt, yd;
  int a1, a2, a3, a4;

  for (y = 0; y < h; ++y) {
    for (x = 0; x < w; ++x) {
      int x0 = x, y0 = y;
      C[y0 * cl + x0] = A[y0 * al + x0];

      if (0 == R[y * rl + x]) {
        continue;
      }

      for (j = x0 - 1; j >= 0; j--) {
        if ((R[y0 * rl + j] == 0) && (R[y0 * rl + j + 1] == 0) && (R[y0 * rl + j + 2] == 1)) {
          xl = j;
          break;
        }
        else {
          continue;
        }
      }

      for (j = x0 + 1; j < w; ++j) {
        if ((R[y0 * rl + j] == 0) && (R[y0 * rl + j - 1] == 0) && (R[y0 * rl + j - 2] == 1)) {
          xr = j;
          break;
        }
        else {
          continue;
        }
      }

      for (k = y0 - 1; k >= 0; --k) {
        if ((R[k * rl + x0] == 0) && (R[(k + 1)*rl + x0] == 0) && (R[(k + 2)*rl + x0] == 1)) {
          yt = k;
          break;
        }
        else {
          continue;
        }
      }

      for (k = y0 + 1; k < h; ++k) {
        if ((R[k * rl + x0] == 0) && (R[(k - 1)*rl + x0] == 0) && (R[(k - 2)*rl + x0] == 1)) {
          yd = k;
          break;
        }
        else {
          continue;
        }
      }

      a1 = A[y0 * al + xl];
      a2 = A[y0 * al + xr];
      a3 = A[yt * al + x0];
      a4 = A[yd * al + x0];
      C[y0 * cl + x0] = MIN4(a1, a2, a3, a4);
    }
  }

  return 0;
}
