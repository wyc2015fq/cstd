
#include "../img/filterrect.def"

//#define error(xx) printf
int adjgamma_fix(const img_t* im, double g, img_t* newim)
{
  int i, n = im->h * im->w, mi = 0, ma = 0;
  int mind, maxd;
  int powmap[256] = {0};

  if (g <= 0) {
    //printf("Gamma value must be > 0");
    return 0;
  }

  for (i = 0; i < 256; ++i) {
    powmap[i] = (int)(256 * pow(i / 256., (1. / g)));
  }

  mi = 0, ma = 0;
  MINMAX_ELEMENT(n, im->tt.u2, mi, ma);
  mind = im->tt.u2[mi], maxd = im->tt.u2[ma];

  // rescale range 0-1
  if (maxd > mind) {
    for (i = 0; i < n; ++i) {
      int x = im->tt.u2[i];
      x = powmap[(x - mind) * 255 / (maxd - mind)]; // Apply gamma function
      newim->tt.u1[i] = x;
    }
  }

  return 0;
}


int canny_fix(buf_t* bf, const img_t* im, double sigma, double scaling, double vert, double horz, double g, double radius, img_t* I_edge)
{
  int ii, jj, row, col;
  int xscaling = (int)vert, yscaling = (int)horz, isigma = 6 * (int)sigma;
  img_t im1[1] = {0}, im2[1] = {0};
  img_t gradient[1] = {0};
  img_t inimage[1] = {0};
  img_t dx[1] = {0};
  img_t dy[1] = {0};
  int* buf = 0;
  int buflen;
  //ASSERT(im->w < 256);
  //imwrite3(im);
  row = im->h, col = im->w;
  bf_imsetsize(bf, I_edge, row, col, 1, 1);
  buflen = bf->len;
  bf_imsetsize(bf, im1, row, col, 1, 1);
  bf_imsetsize(bf, im2, row, col, 1, 1);
  imclone(im, im1);

  BFMALLOC(bf, buf, col);

  if (sigma == 1) {
    GAUSS7(row, col, im->tt.data, im->s, 1, im1->tt.data, im1->s, 1, buf);
  }
  else if (sigma == 2) {
    GAUSS13(row, col, im->tt.data, im->s, 1, im1->tt.data, im1->s, 1, buf);
  }
  else {
    ASSERT(0 && "sigma");
  }

  BFFREE(bf, buf, col);

#if 1

  for (ii = 0; ii < row; ++ii) {
    for (jj = 0; jj < col; ++jj) {
      if ((ii < isigma || ii > (row - isigma)) && (jj < isigma || jj > (col - isigma))) {
        im1->tt.data[im->s * ii + jj] = im->tt.data[im->s * ii + jj];
      }
    }
  }

#endif

  //printf(" %.4f", utime_elapsed(_start_time));}
  //FPRINT1D("im1.txt", "%f\r\n", im1->w, (im1->tt.f8+im1->w*57), 1);
  imclone(im1, im2);
  //imshow(im2);  cvWaitKey(-1);
  bf_imsetsize(bf, gradient, row, col, sizeof(short), 1);
  bf_imsetsize(bf, dx, row, col, sizeof(short), 1);
  bf_imsetsize(bf, dy, row, col, sizeof(short), 1);

  //h =[-1 0 1] v =[-1;0;1]
  //
  //   [-1 0 0]    [ 0 0 1]
  //d1=[ 0 0 0] d2=[ 0 0 0]
  //   [ 0 0 1]    [-1 0 0]
  ii = 0;

  for (jj = 1; jj < col - 1 ; ++jj) {
    int h, v, d1, d2, grad, X, Y;
    int im2l = im2->w;
    uchar* pos = im2->tt.u1 + im2->w * ii + jj;
    h = pos[1] - 0;
    v = pos[im2l] - 0;
    d1 = pos[im2l + 1] - 0;
    d2 = 0 - pos[im2l - 1];
    X = (h + (d1 + d2) / 2) * xscaling;
    Y = (v + (d1 - d2) / 2) * yscaling;
#if 0
    grad = sqrt(X * X + Y * Y);
#else
    grad = FastDistance2D(X, Y);
#endif
    gradient->tt.u2[gradient->w * ii + jj] = grad;
    dx->tt.s2[dx->w * ii + jj] = X;
    dy->tt.s2[dy->w * ii + jj] = Y;
  }

  ii = row - 1;

  for (jj = 1; jj < col - 1 ; ++jj) {
    int h, v, d1, d2, grad, X, Y;
    int im2l = im2->w;
    uchar* pos = im2->tt.u1 + im2->w * ii + jj;
    h = 0 - pos[-1];
    v = 0 - pos[-im2l];
    d1 = 0 - pos[-im2l - 1];
    d2 = pos[-im2l + 1] - 0;
    X = (h + (d1 + d2) / 2) * xscaling;
    Y = (v + (d1 - d2) / 2) * yscaling;
#if 0
    grad = sqrt(X * X + Y * Y);
#else
    grad = FastDistance2D(X, Y);
#endif
    gradient->tt.u2[gradient->w * ii + jj] = grad;
    dx->tt.s2[dx->w * ii + jj] = X;
    dy->tt.s2[dy->w * ii + jj] = Y;
  }

  for (ii = 1; ii < row - 1; ++ii) {
    for (jj = 1; jj < col - 1 ; ++jj) {
      int h, v, d1, d2, grad, X, Y;
      int im2l = im2->w;
      uchar* pos = im2->tt.u1 + im2->w * ii + jj;
      h = pos[1] - pos[-1];
      v = pos[im2l] - pos[-im2l];
      d1 = pos[im2l + 1] - pos[-im2l - 1];
      d2 = pos[-im2l + 1] - pos[im2l - 1];
      X = (h + (d1 + d2) / 2) * xscaling;
      Y = (v + (d1 - d2) / 2) * yscaling;
#if 0
      grad = sqrt(X * X + Y * Y);
#else
      grad = FastDistance2D(X, Y);
#endif
      gradient->tt.u2[gradient->w * ii + jj] = grad;
      dx->tt.s2[dx->w * ii + jj] = X;
      dy->tt.s2[dy->w * ii + jj] = Y;
    }
  }

  bf_imsetsize(bf, inimage, row, col, 1, 1);
  adjgamma_fix(gradient, g, inimage);
#if 0

  if (0) {
    img_t I41[1] = {0};
    img_t I44[1] = {0};
    int i = 0;
    double d = 0;
    imsetsize(I41, gradient->h, gradient->w, sizeof(double), 1);

    for (i = 0; i < im->h * im->w; ++i) {
      I41->tt.f8[i] = gradient->tt.u2[i];
    }

    adjgamma(I41, 1.9, I44);

    for (i = 0; i < inimage->h * inimage->w; ++i) {
      if (inimage->tt.u1[i] > 0) {
        int asdf = 0;
      }

      d += ABSSUB(I44->tt.f8[i], inimage->tt.u1[i] / 256.);
    }

    printf("d = %f\n", d);
    imfree(I41);
    imfree(I44);
  }

#endif

  for (ii = 1; ii < row - 1; ++ii) {
    for (jj = 1; jj < col - 1 ; ++jj) {
      int X = dx->tt.s2[dx->w * ii + jj];
      int Y = dy->tt.s2[dy->w * ii + jj];
      int grad = inimage->tt.u1[inimage->w * ii + jj];
      int ix = IABS(X);
      int iy = IABS(Y);
      int sx = CV_SIGN(X);
      int sy = CV_SIGN(Y);
      int v1, v2;
      //int x = 0, y = 0, fx = 0, cx = 0, fy = 0, cy = 0;

      //int tl, tr, bl, br, upperavg, loweravg;
      if (!grad) {
        continue;
      }

#if 1

      if (ix > iy) {
        v1 = (ix * inimage->tt.u1[inimage->w * (ii) + (jj + sx)] + iy * inimage->tt.u1[inimage->w * (ii + sy) + (jj + sx)]) / (ix + iy);
      }
      else {
        v1 = (ix * inimage->tt.u1[inimage->w * (ii + sy) + (jj + sx)] + iy * inimage->tt.u1[inimage->w * (ii + sy) + (jj)]) / (ix + iy);
      }

      if (grad >= v1) {
        if (ix > iy) {
          v2 = (ix * inimage->tt.u1[inimage->w * (ii) + (jj - sx)] + iy * inimage->tt.u1[inimage->w * (ii - sy) + (jj - sx)]) / (ix + iy);
        }
        else {
          v2 = (ix * inimage->tt.u1[inimage->w * (ii - sy) + (jj - sx)] + iy * inimage->tt.u1[inimage->w * (ii - sy) + (jj)]) / (ix + iy);
        }

        if (grad >= v2) {
          I_edge->tt.u1[inimage->w * (ii) + (jj)] = grad;
        }
      }

#else
      x = (jj << 8) + X * ((3 << 8) / 2) / grad; // x, y location on one side of the point in question
      y = (ii << 8) + Y * ((3 << 8) / 2) / grad;
      fx = x >> 8;
      cx = (x + 255) >> 8;
      fy = (y >> 8);
      cy = (y + 255) >> 8;
      tl = inimage->tt.u1[inimage->w * fy + fx]; // Value at top left integer pixel location.
      tr = inimage->tt.u1[inimage->w * fy + cx]; // top right
      bl = inimage->tt.u1[inimage->w * cy + fx]; // bottom left
      br = inimage->tt.u1[inimage->w * cy + cx]; // bottom right
      upperavg = tl + (((x & 0xff) * (tr - tl)) >> 8); // Now use bilinear interpolation to
      loweravg = bl + (((x & 0xff) * (br - bl)) >> 8); // estimate value at x,y
      v1 = upperavg + (((y & 0xff) * (loweravg - upperavg)) >> 8);

      if (grad > v1) {
        x = (jj << 8) - X * ((3 << 8) / 2) / grad; // x, y location on one side of the point in question
        y = (ii << 8) - Y * ((3 << 8) / 2) / grad;
        fx = x >> 8;
        cx = (x + 255) >> 8;
        fy = (y >> 8);
        cy = (y + 255) >> 8;
        tl = inimage->tt.u1[inimage->w * fy + fx]; // Value at top left integer pixel location.
        tr = inimage->tt.u1[inimage->w * fy + cx]; // top right
        bl = inimage->tt.u1[inimage->w * cy + fx]; // bottom left
        br = inimage->tt.u1[inimage->w * cy + cx]; // bottom right
        upperavg = tl + (((x & 0xff) * (tr - tl)) >> 8); // Now use bilinear interpolation to
        loweravg = bl + (((x & 0xff) * (br - bl)) >> 8); // estimate value at x,y
        v2 = upperavg + (((y & 0xff) * (loweravg - upperavg)) >> 8);

        if (grad >= v2) {
          I_edge->tt.u1[inimage->w * (ii) + (jj)] = grad;
        }
      }

#endif
    }
  }

  //imshow(I_edge); cvWaitKey(-1);

  //imshow(inimage); cvWaitKey(-1);
  bf_imfree(bf, inimage);
  bf_imfree(bf, dy);
  bf_imfree(bf, dx);
  bf_imfree(bf, gradient);
  bf_imfree(bf, im2);
  bf_imfree(bf, im1);
  assert(buflen == bf->len);
  return 0;
}
