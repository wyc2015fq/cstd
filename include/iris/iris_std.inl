

int iris_stdimg(buf_t* bf, const irisrecog_t* ir, img_t* gry, uchar* v_normalize)
{
  img_t mask_normalize[1] = {0};
  img_t mask_iris[1] = {0};
  int z_top[1000] = {0};
  int z_bot[1000] = {0};
  int ROI_col = gry->w;
  int i, cnt_zero = 0;

  int nrr[3] = {0};
  int wrr[3] = {0};
  double urr[3] = {0};
  double drr[3] = {0};
  double shift2 = 1 << IRIS_SHIFT2;

  for (i = 0; i < 3; ++i) {
    nrr[i] = (ir->in[i]);
    wrr[i] = (ir->iw[i]);
    urr[i] = (ir->iu[i]) / shift2;
    drr[i] = (ir->id[i]) / shift2;
  }

  {
    int x;
    int u, u0 = ir->iu[0], u1 = ir->iu[1] >> 8, u2 = ir->iu[2] >> 8;
    int d, d0 = ir->id[0], d1 = ir->id[1] >> 8, d2 = ir->id[2] >> 8;

    for (x = 0; x < ROI_col; ++x) {
      z_top[x] = u = (u2 + (u1 + ((u0 * x) >> 8)) * x) >> 8;
      z_bot[x] = d = (d2 + (d1 + ((d0 * x) >> 8)) * x) >> 8;

      if (0 == d) {
        z_bot[x] = gry->h;
      }
    }

    //iris_detect_show(gry, z_top, z_bot, nrr, wrr);
    //imshow(gry);cvWaitKey(-1);
  }

  cnt_zero = Daugmannormalized(nrr, wrr, gry, z_top, z_bot, v_normalize);

  //{img_t im[1]={0}; IMINIT(im, IRIS_H, IRIS_W, v_normalize, IRIS_W, 1, 1), imshow(im); cvWaitKey(-1);}

  //imshow(gry); imshow(mask_iris); imshow(mask_normalize);
  //imshow(v_normalize); cvWaitKey(-1);
  return cnt_zero;
}
