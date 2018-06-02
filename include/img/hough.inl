
int sintable[] = {
#include "math\sintable.txt"
};
int costable[] = {
#include "math\costable.txt"
};

int hough_circle(const img_t* src, double* out, int outlen)
{
  img_t dx[1] = {0};
  img_t dy[1] = {0};
  img_t mag[1] = {0};
  int i, j, k, r;
  int r_min = 30;
  int r_max = 130;
  int dr = r_max - r_min;
  int h = src->h, w = src->w;
  int n = h * w * dr;
  int* hough_space = MALLOC(int, n);
  int* idx = MALLOC(int, n);
  cvSobel(src, dx, 1, 0, 3);
  cvSobel(src, dy, 0, 1, 3);
  imsetsize(mag, h, w, sizeof(double), 1);

  for (i = 0; i < h; ++i) {
    const short* pdx = (const short*)(dx->tt.data + dx->s * i);
    const short* pdy = (const short*)(dy->tt.data + dy->s * i);

    for (j = 0; j < w; ++j) {
      int imag = abs(pdx[j]) + abs(pdy[j]);
      mag->f8[i * mag->w + j] = imag;

      for (r = r_min; r < r_max; ++r) {
        for (k = 0; k < 360; ++k) {
          int y = i - ((r * costable[(k)]) >> 16);
          int x = j - ((r * sintable[(k)]) >> 16);

          if (x >= 0 && x < w && y >= 0 && y < h) {
            hough_space[(r - r_min) * (h * w) + y * (w) + x] += imag;
          }
        }
      }
    }
  }

  imshow(mag);
  cvWaitKey(-1);
  pfree(idx);
  pfree(hough_space);
  return 0;
}
