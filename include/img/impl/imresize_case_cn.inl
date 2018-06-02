
if (ah > 0 && aw > 0 && bh > 0 && bw > 0)
{
  int scw = (aw) / bw, sch = (ah) / bh;
  
  if (ah == bh || aw == bw) {
    memcpy2d(dst, bl, A, al, ah, aw * ai);
    type = CC_INTER_DOWN;
  }

  if (CC_INTER_AREA == type && (ah < 2 * bh || aw < 2 * bw)) {
    type = CC_INTER_LINEAR;
  }

  switch (type) {
  case CC_INTER_DOWN:
    break;

  case CC_INTER_AREA: {
    int x, y, i;
    enum {nn = 1 << 13};
    int x_off[nn] = {0};
    int x_buf[nn] = {0};
    int x_cnt[nn] = {0};
    int aw2 = aw * 2;

    for (x = 0; x < aw; ++x) {
      x_off[x] = ((x * 2 + 1) * bw / aw2);
    }

    for (y = 0; y < bh; ++y) {
      uchar* dst0 = dst + y * bl;
      int row0, row1;
      int xc, xo;
      MEMSET(x_buf, 0, bw * bi);
      MEMSET(x_cnt, 0, bw);
      row0 = y * ah / bh;
      row1 = (y + 1) * ah / bh;

      for (i = row0; i < row1; ++i) {
        const uchar* a0 = A + i * al;
        x = 0;
#define _OP(_i)  (x_buf[xo*__cn+_i]+=a0[_i])

        for (; x < aw; ++x, a0 += ai) {
          xo = x_off[x];
          OPTREPT(0, _OP);
          x_cnt[xo]++;
        }

#undef _OP
      }

#define _OP(_i)  (dst0[_i] = x_buf[x*__cn+_i]/xc)

      for (x = 0; x < bw; ++x, dst0 += __cn) {
        xc = x_cnt[x];
        OPTREPT(0, _OP);
      }

#undef _OP
    }
  }
  break;

  case CC_INTER_NN: {
    int x, y;
    int bw2 = bw * 2;
    int* x_off = 0;
    SYSMALLOC(int, x_off, bw);

    for (x = 0; x < bw; ++x) { //Éú³É±í srcx_table
      x_off[x] = ((2 * x + 1) * aw / bw2) * ai;
    }

    cn = MIN(ai, bi);

    for (y = 0; y < bh; ++y) {
      const uchar* a0 = (A + (y * ah / bh) * al);
      uchar* dst0 = dst + y * bl;
      x = 0;

      for (; x < bw - 3; x += 4, dst0 += 4 * bi) {
        *(arrtype*)(dst0 + 0 * bi) = *(const arrtype*)(a0 + x_off[x + 0]);
        *(arrtype*)(dst0 + 1 * bi) = *(const arrtype*)(a0 + x_off[x + 1]);
        *(arrtype*)(dst0 + 2 * bi) = *(const arrtype*)(a0 + x_off[x + 2]);
        *(arrtype*)(dst0 + 3 * bi) = *(const arrtype*)(a0 + x_off[x + 3]);
      }

      for (; x < bw; ++x, dst0 += bi) {
        *(arrtype*)(dst0 + 0) = *(const arrtype*)(a0 + x_off[x + 0]);
      }
    }

    SYSFREE(int, x_off, bw);
  }
  break;

  case CC_INTER_LINEAR: {
    typedef ushort worktype;
    typedef uchar arrtype;
    int x, y;
    uchar* tmp_buf = 0;
    int* x_off0 = 0, * x_off1 = 0;
    worktype* x_buf_0 = 0, * x_buf_1 = 0;
    arrtype* x_alf0 = 0;
    const arrtype* a00 = 0, * a01 = 0;
    worktype* x_buf0, *x_buf1;
    int buf_size = bw * (2 * sizeof(int) + 2 * bi * sizeof(worktype) + sizeof(arrtype));
    SYSMALLOC(uchar, tmp_buf, buf_size);
    x_off0 = (int*)tmp_buf;
    x_off1 = x_off0 + bw;
    x_buf_0 = (worktype*)(x_off1 + bw);
    x_buf_1 = x_buf_0 + bw * bi;
    x_alf0 = (arrtype*)(x_buf_1 + bw * bi);
    //ASSERT(bw<nn);

    x_buf0 = x_buf_0, x_buf1 = x_buf_1;

    for (x = 0; x < bw; ++x) {
      double t = ((x + 0.5) * aw / bw);
      int it = (int)t, it0, it1, ia0;
      double tf = t - it, af;

      if (tf < 0.5) {
        it0 = MAX(0, it - 1);
        it1 = it;
        af = 0.5 - tf;
      }
      else {
        it0 = it;
        it1 = MIN(aw - 1, it + 1);
        af = 1.5 - tf;
      }

      ia0 = (int)(af * 255);
      x_alf0[x] = (arrtype)ia0;
      x_off0[x] = it0 * ai;
      x_off1[x] = it1 * ai;
    }

    for (y = 0; y < bh; ++y) {
      int sy0, sy1;
      arrtype ay0, ay1, ax0, ax1;
      {
        double t = ((y + 0.5) * ah / bh);
        int it = (int)t, it0, it1, ia0;
        double tf = t - it, af;

        if (tf < 0.5) {
          it0 = MAX(0, it - 1);
          it1 = it;
          af = 0.5 - tf;
        }
        else {
          it0 = it;
          it1 = MIN(ah - 1, it + 1);
          af = 1.5 - tf;
        }

        ia0 = (int)(af * 255);
        sy0 = it0;
        sy1 = it1;

        ay0 = ia0;
        ay1 = 255 - ia0;
      }
      {
        const arrtype* as0, *as1, * a_0 = A + sy0 * al, * a_1 = A + sy1 * al;
        arrtype* dst0 = dst + y * bl;
        worktype* xb0, *xb1;
#define _OP(_j)  (xb0[_j] = as0[_j]*ax0 + as1[_j]*ax1)

        if (a00 != a_0) {
          if (a01 == a_0) {
            T_SWAP(const arrtype*, a00, a01);
            T_SWAP(worktype*, x_buf0, x_buf1);
          }
          else {
            a00 = a_0;
            xb0 = x_buf0;
            x = 0;

            for (; x < bw; ++x, xb0 += __cn) {
              ax0 = x_alf0[x], ax1 = 255 - ax0, as0 = a_1 + x_off0[x], as1 = a_1 + x_off1[x];
              OPTREPT(0, _OP);
            }
          }
        }

        if (a01 != a_1) {
          a01 = a_1;
          xb0 = x_buf1;
          x = 0;

          for (; x < bw; ++x, xb0 += __cn) {
            ax0 = x_alf0[x], ax1 = 255 - ax0, as0 = a_1 + x_off0[x], as1 = a_1 + x_off1[x];
            OPTREPT(0, _OP);
          }
        }

#undef _OP
        xb0 = x_buf0, xb1 = x_buf1;
        x = 0;
#define _OP(_j)  (dst0[_j] = (xb0[_j]*ay0 + xb1[_j]*ay1)>>16)

        for (; x < bw - 3; x += 4, dst0 += 4 * bi, xb0 += 4 * __cn, xb1 += 4 * __cn) {
          OPTREPT(0, _OP);
          OPTREPT(1, _OP);
          OPTREPT(2, _OP);
          OPTREPT(3, _OP);
        }

        for (; x < bw; ++x, dst0 += bi, xb0 += __cn, xb1 += __cn) {
          //PUTYPIX(0);
          OPTREPT(0, _OP);
#undef _OP
        }
      }
    }

    SYSFREE(uchar, tmp_buf, buf_size);
  }

  break;

  default:
    ASSERT(0);
    break;
  }
}

#undef OPTREPT

