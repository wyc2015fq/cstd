
{
  int local_alloc = 0;
  worktype* buf = 0, *buf0 = 0;
  const arrtype* src2;
  arrtype* dst2;
  int buf_size;
  int i, j;
  int W = src_size.w, H = src_size.h;
  int Wd = dst_size.w, Hd = dst_size.h;
  int Wd_, Hd_;
  int Wn = W * channels;
  int bufW;
  int cols, rows; /* columns and rows to modify */

  assert(channels == 1 || channels == 3);

  buf_size = MAX(src_size.w, src_size.h) * sizeof(buf[0]) * 2 * channels;

  if (buf_size > (1 << 14)) {
    buf = (worktype*)cAlloc(buf_size);

    if (!buf) {
      return CC_OUTOFMEM_ERR;
    }
  }
  else
  {
    buf = (worktype*)cvAlignPtr(alloca(buf_size + 8), 8);
    local_alloc = 1;
  }

  buf0 = buf;

  src_step /= sizeof(src[0]);
  dst_step /= sizeof(dst[0]);

  cols = (W & 1) + (Wd * 2 > W);
  rows = (H & 1) + (Hd * 2 > H);

  src2 = src + (H - 1) * src_step;
  dst2 = dst + (Hd - rows) * dst_step;
  src += (W - 1) * channels;
  dst += (Wd - cols) * channels;

  /* part of row(column) from 1 to Wd_(Hd_) is processed using PD_FILTER macro */
  Wd_ = Wd - 1 + (cols == 1 && (W & 1) != 0);
  Hd_ = Hd - 1 + (rows == 1 && (H & 1) != 0);

  bufW = channels * cols;

  /******************* STAGE 1. ******************/

  /* do horizontal convolution of the 1-2 right columns and write results to buffer */
  if (cols > 0) {
    if (W <= 2) {
      assert(Wd == 1);

      for (i = 0; i < H; i++, src += src_step, buf += channels) {
        if (channels == 1) {
          buf[0] = PD_SINGULAR(src[1 - Wn], src[0]);
        }
        else {
          buf[0] = PD_SINGULAR(src[3 - Wn], src[0]);
          buf[1] = PD_SINGULAR(src[4 - Wn], src[1]);
          buf[2] = PD_SINGULAR(src[5 - Wn], src[2]);
        }
      }
    }
    else if (W == 3 && Wd == 1 || W > 3 && !(Wd & 1)) {
      for (i = 0; i < H; i++, src += src_step, buf += channels) {
        if (channels == 1) {
          buf[0] = PD_LT(src[-2], src[-1], src[0]);
        }
        else {
          buf[0] = PD_LT(src[-6], src[-3], src[0]);
          buf[1] = PD_LT(src[-5], src[-2], src[1]);
          buf[2] = PD_LT(src[-4], src[-1], src[2]);
        }
      }
    }
    else if (W == 3) {
      for (i = 0; i < H; i++, src += src_step, buf += channels * 2) {
        if (channels == 1) {
          buf[0] = PD_LT(src[-2], src[-1], src[0]);
          buf[1] = PD_LT(src[0], src[-1], src[-2]);
        }
        else {
          buf[0] = PD_LT(src[-6], src[-3], src[0]);
          buf[1] = PD_LT(src[-5], src[-2], src[1]);
          buf[2] = PD_LT(src[-4], src[-1], src[2]);
          buf[3] = PD_LT(src[0], src[-3], src[-6]);
          buf[4] = PD_LT(src[1], src[-2], src[-5]);
          buf[5] = PD_LT(src[2], src[-1], src[-4]);
        }
      }
    }
    else if (cols == 1) {
      for (i = 0; i < H; i++, src += src_step, buf += channels) {
        if (channels == 1) {
          buf[0] = PD_FILTER(src[-4], src[-3], src[-2], src[-1], src[0]);
        }
        else {
          buf[0] = PD_FILTER(src[-12], src[-9], src[-6], src[-3], src[0]);
          buf[1] = PD_FILTER(src[-11], src[-8], src[-5], src[-2], src[1]);
          buf[2] = PD_FILTER(src[-10], src[-7], src[-4], src[-1], src[2]);
        }
      }
    }
    else {
      for (i = 0; i < H; i++, src += src_step, buf += channels * 2) {
        if (channels == 1) {
          buf[0] = PD_FILTER(src[-4], src[-3], src[-2], src[-1], src[0]);
          buf[1] = PD_LT(src[0], src[-1], src[-2]);
        }
        else {
          buf[0] = PD_FILTER(src[-12], src[-9], src[-6], src[-3], src[0]);
          buf[1] = PD_FILTER(src[-11], src[-8], src[-5], src[-2], src[1]);
          buf[2] = PD_FILTER(src[-10], src[-7], src[-4], src[-1], src[2]);
          buf[3] = PD_LT(src[0], src[-3], src[-6]);
          buf[4] = PD_LT(src[1], src[-2], src[-5]);
          buf[5] = PD_LT(src[2], src[-1], src[-4]);
        }
      }
    }

    buf = buf0;
  }

  src = src2;

  /******************* STAGE 2. ******************/

  /* do vertical convolution of the pre-processed right columns, */
  /* stored in buffer, and write results to the destination */
  /* do vertical convolution of the 1-2 bottom rows */
  /* and write results to the buffer */
  if (H <= 2) {
    if (cols > 0) {
      assert(Hd == 1);

      for (j = 0; j < bufW; j++) {
        dst[j] = (arrtype)_pd_scale_(PD_SINGULAR(buf[j], buf[j + (H - 1) * bufW]));
      }
    }

    if (rows > 0) {
      for (j = 0; j < Wn; j++) {
        buf[j] = PD_SINGULAR(src[j - src_step], src[j]);
      }
    }
  }
  else if (H == 3) {
    if (cols > 0) {
      for (j = 0; j < bufW; j++) {
        dst[j] = (arrtype)_pd_scale_(PD_LT(buf[j], buf[j + bufW], buf[j + bufW * 2]));
      }

      if (Hd == 2) {
        dst += dst_step;

        for (j = 0; j < bufW; j++) {
          dst[j] = (arrtype)_pd_scale_(PD_LT(buf[j + bufW * 2], buf[j + bufW], buf[j]));
        }
      }
    }

    if (Hd == 1) {
      for (j = 0; j < Wn; j++) {
        buf[j] = PD_LT(src[j - src_step * 2], src[j - src_step], src[j]);
      }
    }
    else {
      for (j = 0; j < Wn; j++) {
        buf[j] = PD_LT(src[j - src_step * 2], src[j - src_step], src[j]);
        buf[j + Wn] = PD_LT(src[j], src[j - src_step], src[j - src_step * 2]);
      }
    }
  }
  else
  {
    if (cols > 0) {
      /* top of the right border */
      for (j = 0; j < bufW; j++) {
        dst[j] = (arrtype)_pd_scale_(PD_LT(buf[j], buf[j + bufW], buf[j + bufW * 2]));
      }

      /* middle part of the right border */
      buf += bufW * 2;
      dst += dst_step;

      for (i = 1; i < Hd_; i++, dst += dst_step, buf += bufW * 2) {
        for (j = 0; j < bufW; j++) {
          dst[j] = (arrtype)_pd_scale_(PD_FILTER(buf[j - bufW * 2], buf[j - bufW], buf[j], buf[j + bufW], buf[j + bufW * 2]));
        }
      }

      /* bottom of the right border */
      if (!(H & 1)) {
        for (j = 0; j < bufW; j++) {
          dst[j] = (arrtype)_pd_scale_(PD_RB(buf[j - bufW * 2], buf[j - bufW], buf[j], buf[j + bufW]));
        }
      }
      else if (rows > 1) {
        for (j = 0; j < bufW; j++) {
          dst[j] = (arrtype)_pd_scale_(PD_LT(buf[j - bufW * 2], buf[j - bufW], buf[j]));
        }
      }

      buf = buf0;
    }

    if (rows > 0) {
      if (!(H & 1)) {
        for (j = 0; j < Wn; j++) {
          buf[j] = PD_LT(src[j], src[j - src_step], src[j - src_step * 2]);
        }
      }
      else if (cols == 1) {
        for (j = 0; j < Wn; j++) {
          buf[j] = PD_FILTER(src[j - src_step * 4], src[j - src_step * 3], src[j - src_step * 2], src[j - src_step], src[j]);
        }
      }
      else {
        for (j = 0; j < Wn; j++) {
          buf[j] = PD_FILTER(src[j - src_step * 4], src[j - src_step * 3], src[j - src_step * 2], src[j - src_step], src[j]);
          buf[j + Wn] = PD_LT(src[j], src[j - src_step], src[j - src_step * 2]);
        }
      }
    }
  }


  /******************* STAGE 3. ******************/

  /* do horizontal convolution of the pre-processed bottom rows,*/
  /* stored in buffer, and write results to the destination */
  if (rows > 0) {
    dst = dst2;

    if (W <= 2) {
      assert(Wd == 1);

      for (; rows--; dst += dst_step, buf += Wn) {
        if (channels == 1) {
          dst[0] = (arrtype)_pd_scale_(PD_SINGULAR(buf[0], buf[Wn - 1]));
        }
        else {
          dst[0] = (arrtype)_pd_scale_(PD_SINGULAR(buf[0], buf[Wn - 3]));
          dst[1] = (arrtype)_pd_scale_(PD_SINGULAR(buf[1], buf[Wn - 2]));
          dst[2] = (arrtype)_pd_scale_(PD_SINGULAR(buf[2], buf[Wn - 1]));
        }
      }
    }
    else if (W == 3) {
      if (Wd == 1) {
        for (; rows--; dst += dst_step, buf += Wn) {
          if (channels == 1) {
            dst[0] = (arrtype)_pd_scale_(PD_LT(buf[0], buf[1], buf[2]));
          }
          else {
            dst[0] = (arrtype)_pd_scale_(PD_LT(buf[0], buf[3], buf[6]));
            dst[1] = (arrtype)_pd_scale_(PD_LT(buf[1], buf[4], buf[7]));
            dst[2] = (arrtype)_pd_scale_(PD_LT(buf[2], buf[5], buf[8]));
          }
        }
      }
      else {
        for (; rows--; dst += dst_step, buf += Wn) {
          if (channels == 1) {
            dst[0] = (arrtype)_pd_scale_(PD_LT(buf[0], buf[1], buf[2]));
            dst[1] = (arrtype)_pd_scale_(PD_LT(buf[2], buf[1], buf[0]));
          }
          else {
            dst[0] = (arrtype)_pd_scale_(PD_LT(buf[0], buf[3], buf[6]));
            dst[1] = (arrtype)_pd_scale_(PD_LT(buf[1], buf[4], buf[7]));
            dst[2] = (arrtype)_pd_scale_(PD_LT(buf[2], buf[5], buf[8]));
            dst[3] = (arrtype)_pd_scale_(PD_LT(buf[6], buf[3], buf[0]));
            dst[4] = (arrtype)_pd_scale_(PD_LT(buf[7], buf[4], buf[1]));
            dst[5] = (arrtype)_pd_scale_(PD_LT(buf[8], buf[5], buf[2]));
          }
        }
      }
    }
    else {
      for (; rows--; dst += dst_step, buf += Wn) {
        if (channels == 1) {
          /* left part of the bottom row */
          dst[0] = (arrtype)_pd_scale_(PD_LT(buf[0], buf[1], buf[2]));

          /* middle part of the bottom row */
          for (i = 1; i < Wd_; i++) {
            dst[i] = (arrtype)_pd_scale_(PD_FILTER(buf[i * 2 - 2], buf[i * 2 - 1], buf[i * 2], buf[i * 2 + 1], buf[i * 2 + 2]));
          }

          /* right part of the bottom row */
          if (!(W & 1)) {
            dst[i] = (arrtype)_pd_scale_(PD_RB(buf[i * 2 - 2], buf[i * 2 - 1], buf[i * 2], buf[i * 2 + 1]));
          }
          else if (cols > 1) {
            dst[i] = (arrtype)_pd_scale_(PD_LT(buf[i * 2 - 2], buf[i * 2 - 1], buf[i * 2]));
          }
        }
        else {
          /* left part of the bottom row */
          dst[0] = (arrtype)_pd_scale_(PD_LT(buf[0], buf[3], buf[6]));
          dst[1] = (arrtype)_pd_scale_(PD_LT(buf[1], buf[4], buf[7]));
          dst[2] = (arrtype)_pd_scale_(PD_LT(buf[2], buf[5], buf[8]));

          /* middle part of the bottom row */
          for (i = 3; i < Wd_ * 3; i++) {
            dst[i] = (arrtype)_pd_scale_(PD_FILTER(buf[i * 2 - 6], buf[i * 2 - 3], buf[i * 2], buf[i * 2 + 3], buf[i * 2 + 6]));
          }

          /* right part of the bottom row */
          if (!(W & 1)) {
            dst[i] = (arrtype)_pd_scale_(PD_RB(buf[i * 2 - 6], buf[i * 2 - 3], buf[i * 2], buf[i * 2 + 3]));
            dst[i + 1] = (arrtype)_pd_scale_(PD_RB(buf[i * 2 - 5], buf[i * 2 - 2], buf[i * 2 + 1], buf[i * 2 + 4]));
            dst[i + 2] = (arrtype)_pd_scale_(PD_RB(buf[i * 2 - 4], buf[i * 2 - 1], buf[i * 2 + 2], buf[i * 2 + 5]));
          }
          else if (cols > 1) {
            dst[i] = (arrtype)_pd_scale_(PD_LT(buf[i * 2 - 6], buf[i * 2 - 3], buf[i * 2]));
            dst[i + 1] = (arrtype)_pd_scale_(PD_LT(buf[i * 2 - 5], buf[i * 2 - 2], buf[i * 2 + 1]));
            dst[i + 2] = (arrtype)_pd_scale_(PD_LT(buf[i * 2 - 4], buf[i * 2 - 1], buf[i * 2 + 2]));
          }
        }
      }
    }
  }

  if (!local_alloc) {
    cFree(&buf0);
  }
}
