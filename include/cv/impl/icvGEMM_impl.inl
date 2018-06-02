{
  static arrtype zerof[] = {0, 0, 0, 0};

  int i = 0;
  int len = 0;
  CSize d_size = cSize(D_cols, D_rows);
  CSize a_size = flags & CC_GEMM_A_T ? cSize(n, D_rows) : cSize(D_rows, n);
  size_t d_step = D_step, a_step = A_step, b_step = B_step, c_step = C_step;

  if (beta == 0) {
    C = 0;
  }

  if (C) {
    if ((flags & CC_GEMM_C_T) != 0 && C == D) {
      cvTranspose(D_cols, D_rows, C, C_step, D, D_step, sizeof(arrtype));
      C = D;
      C_step = D_step;
      flags &= ~CC_GEMM_C_T;
    }
  }
  else
  {
    C_step = 0;
  }

  if (flags == 0 && 2 <= len && len <= 4 && (len == d_size.w || len == d_size.h)) {
    arrtype* d = D;
    const arrtype* a = A, *b = B, *c = C;

    if (!c) {
      c = zerof;
    }

    switch (len) {
    case 2:
      if (len == d_size.w && b != d) {
        for (i = 0; i < d_size.h; i++, d += d_step, a += a_step, c += c_step) {
          arrtype t0 = a[0] * b[0] + a[1] * b[b_step];
          arrtype t1 = a[0] * b[1] + a[1] * b[b_step + 1];
          d[0] = (arrtype)(t0 * alpha + c[0] * beta);
          d[1] = (arrtype)(t1 * alpha + c[1] * beta);
        }
      }
      else if (a != d) {
        int c_step0 = 1;

        if (c == zerof) {
          c_step0 = 0;
          c_step = 1;
        }

        for (i = 0; i < d_size.w; i++, d++, b++, c += c_step0) {
          arrtype t0 = a[0] * b[0] + a[1] * b[b_step];
          arrtype t1 = a[a_step] * b[0] + a[a_step + 1] * b[b_step];
          d[0] = (arrtype)(t0 * alpha + c[0] * beta);
          d[d_step] = (arrtype)(t1 * alpha + c[c_step] * beta);
        }
      }
      else {
        break;
      }

      break;

    case 3:
      if (len == d_size.w && b != d) {
        for (i = 0; i < d_size.h; i++, d += d_step, a += a_step, c += c_step) {
          arrtype t0 = a[0] * b[0] + a[1] * b[b_step] + a[2] * b[b_step * 2];
          arrtype t1 = a[0] * b[1] + a[1] * b[b_step + 1] + a[2] * b[b_step * 2 + 1];
          arrtype t2 = a[0] * b[2] + a[1] * b[b_step + 2] + a[2] * b[b_step * 2 + 2];
          d[0] = (arrtype)(t0 * alpha + c[0] * beta);
          d[1] = (arrtype)(t1 * alpha + c[1] * beta);
          d[2] = (arrtype)(t2 * alpha + c[2] * beta);
        }
      }
      else if (a != d) {
        int c_step0 = 1;

        if (c == zerof) {
          c_step0 = 0;
          c_step = 1;
        }

        for (i = 0; i < d_size.w; i++, d++, b++, c += c_step0) {
          arrtype t0 = a[0] * b[0] + a[1] * b[b_step] + a[2] * b[b_step * 2];
          arrtype t1 = a[a_step] * b[0] + a[a_step + 1] * b[b_step] + a[a_step + 2] * b[b_step * 2];
          arrtype t2 = a[a_step * 2] * b[0] + a[a_step * 2 + 1] * b[b_step] + a[a_step * 2 + 2] * b[b_step * 2];

          d[0] = (arrtype)(t0 * alpha + c[0] * beta);
          d[d_step] = (arrtype)(t1 * alpha + c[c_step] * beta);
          d[d_step * 2] = (arrtype)(t2 * alpha + c[c_step * 2] * beta);
        }
      }
      else {
        break;
      }

      break;

    case 4:
      if (len == d_size.w && b != d) {
        for (i = 0; i < d_size.h; i++, d += d_step, a += a_step, c += c_step) {
          arrtype t0 = a[0] * b[0] + a[1] * b[b_step] + a[2] * b[b_step * 2] + a[3] * b[b_step * 3];
          arrtype t1 = a[0] * b[1] + a[1] * b[b_step + 1] + a[2] * b[b_step * 2 + 1] + a[3] * b[b_step * 3 + 1];
          arrtype t2 = a[0] * b[2] + a[1] * b[b_step + 2] + a[2] * b[b_step * 2 + 2] + a[3] * b[b_step * 3 + 2];
          arrtype t3 = a[0] * b[3] + a[1] * b[b_step + 3] + a[2] * b[b_step * 2 + 3] + a[3] * b[b_step * 3 + 3];
          d[0] = (arrtype)(t0 * alpha + c[0] * beta);
          d[1] = (arrtype)(t1 * alpha + c[1] * beta);
          d[2] = (arrtype)(t2 * alpha + c[2] * beta);
          d[3] = (arrtype)(t3 * alpha + c[3] * beta);
        }
      }
      else if (len <= 16 && a != d) {
        int c_step0 = 1;

        if (c == zerof) {
          c_step0 = 0;
          c_step = 1;
        }

        for (i = 0; i < d_size.w; i++, d++, b++, c += c_step0) {
          arrtype t0 = a[0] * b[0] + a[1] * b[b_step] + a[2] * b[b_step * 2] + a[3] * b[b_step * 3];
          arrtype t1 = a[a_step] * b[0] + a[a_step + 1] * b[b_step] + a[a_step + 2] * b[b_step * 2] + a[a_step + 3] * b[b_step * 3];
          arrtype t2 = a[a_step * 2] * b[0] + a[a_step * 2 + 1] * b[b_step] + a[a_step * 2 + 2] * b[b_step * 2] + a[a_step * 2 + 3] * b[b_step * 3];
          arrtype t3 = a[a_step * 3] * b[0] + a[a_step * 3 + 1] * b[b_step] + a[a_step * 3 + 2] * b[b_step * 2] + a[a_step * 3 + 3] * b[b_step * 3];
          d[0] = (arrtype)(t0 * alpha + c[0] * beta);
          d[d_step] = (arrtype)(t1 * alpha + c[c_step] * beta);
          d[d_step * 2] = (arrtype)(t2 * alpha + c[c_step * 2] * beta);
          d[d_step * 3] = (arrtype)(t3 * alpha + c[c_step * 3] * beta);
        }
      }
      else {
        break;
      }

      break;
    }

  }
  else {
    worktype* buffer = 0;

    if (D == A || D == B) {
      int buf_size = d_size.w * d_size.h;
      buffer = MALLOC(worktype, buf_size);
      D = buffer;
    }

    if ((d_size.w == 1 || len == 1) && !(flags & CC_GEMM_B_T) && (B_step == n)) {
      B_step = d_size.w == 1 ? 0 : 1;
      flags |= CC_GEMM_B_T;
    }

    if (d_size.h <= block_lin_size / 2 || d_size.w <= block_lin_size / 2 || len <= 10 || d_size.w <= block_lin_size && d_size.h <= block_lin_size && len <= block_lin_size) {
      const arrtype* _a_data = A;
      const arrtype* _b_data = B;
      const arrtype* _c_data = C;
      arrtype* _d_data = D;
      size_t _a_step = A_step, _b_step = B_step, _c_step = C_step, _d_step = D_step;
      CSize _a_size = a_size, _d_size = d_size;
      //size_t c_step, arrtype* d_data, size_t d_step, CSize a_size, CSize d_size;
      //icvGEMMSingleMul_64f(A, A_step, B, B_step, C, C_step, D, D_step, a_size, d_size, alpha, beta, flags);
#include "icv_def_gemm_single_mul.inl"
    }
    else
    {
      arrtype* block_buffer = 0;
      int is_a_t = flags & CC_GEMM_A_T;
      int is_b_t = flags & CC_GEMM_B_T;
      int dk0_1, dk0_2;
      int a_buf_size = 0, b_buf_size, d_buf_size;
      arrtype* a_buf = 0;
      arrtype* b_buf = 0;
      arrtype* d_buf = 0;
      int i, j, k, di = 0, dj = 0, dk = 0;
      int dm0, dn0, dk0;
      int a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
      //CvGEMMBlockMulFunc block_mul_func = (CvGEMMBlockMulFunc)block_mul_tab[type];
      //CvGEMMStoreFunc store_func = (CvGEMMStoreFunc)store_tab[type];

      if (!is_a_t) {
        a_step0 = A_step, a_step1 = 1;
      }
      else
      { a_step0 = 1, a_step1 = A_step; }

      if (!is_b_t) {
        b_step0 = B_step, b_step1 = 1;
      }
      else
      { b_step0 = 1, b_step1 = B_step; }

      if (!C) {
        c_step0 = c_step1 = 0;
        flags &= ~CC_GEMM_C_T;
      }
      else if (!(flags & CC_GEMM_C_T)) {
        c_step0 = C_step, c_step1 = 1;
      }
      else
      { c_step0 = 1, c_step1 = C_step; }

      dm0 = MIN(block_lin_size, d_size.h);
      dn0 = MIN(block_lin_size, d_size.w);
      dk0_1 = block_size / dm0;
      dk0_2 = block_size / dn0;
      dk0 = MAX(dk0_1, dk0_2);
      dk0 = MIN(dk0, len);

      if (dk0 * dm0 > block_size) {
        dm0 = block_size / dk0;
      }

      if (dk0 * dn0 > block_size) {
        dn0 = block_size / dk0;
      }

      dk0_1 = (dn0 + dn0 / 8 + 2) & -2;
      b_buf_size = (dk0 + dk0 / 8 + 1) * dk0_1;
      d_buf_size = (dk0 + dk0 / 8 + 1) * dk0_1;

      if (is_a_t) {
        a_buf_size = (dm0 + dm0 / 8 + 1) * ((dk0 + dk0 / 8 + 2) & -2);
        flags &= ~CC_GEMM_A_T;
      }

      block_buffer = MALLOC(worktype, a_buf_size + b_buf_size + d_buf_size);
      d_buf = block_buffer;
      b_buf = d_buf + d_buf_size;

      if (is_a_t) {
        a_buf = b_buf + b_buf_size;
      }

      for (i = 0; i < d_size.h; i += di) {
        di = dm0;

        if (i + di >= d_size.h || 8 * (i + di) + di > 8 * d_size.h) {
          di = d_size.h - i;
        }

        for (j = 0; j < d_size.w; j += dj) {
          arrtype* _d = D + i * D_step + j;
          const arrtype* _c = C + i * c_step0 + j * c_step1;
          int _d_step = D_step;
          dj = dn0;

          if (j + dj >= d_size.w || 8 * (j + dj) + dj > 8 * d_size.w) {
            dj = d_size.w - j;
          }

          flags &= 15;

          if (dk0 < len) {
            _d = d_buf;
            _d_step = dj;
          }

          for (k = 0; k < len; k += dk) {
            const arrtype* _a = A + i * a_step0 + k * a_step1;
            int _a_step = A_step;
            const arrtype* _b = B + k * b_step0 + j * b_step1;
            int _b_step = B_step;
            CSize a_bl_size;

            dk = dk0;

            if (k + dk >= len || 8 * (k + dk) + dk > 8 * len) {
              dk = len - k;
            }

            if (!is_a_t) {
              a_bl_size.w = dk, a_bl_size.h = di;
            }
            else {
              a_bl_size.w = di, a_bl_size.h = dk;
            }

            if (a_buf && is_a_t) {
              int t;
              _a_step = dk;
              icvGEMM_TransposeBlock(_a, A_step * sizeof(arrtype), a_buf, _a_step * sizeof(arrtype), a_bl_size, sizeof(arrtype));
              CC_SWAP(a_bl_size.w, a_bl_size.h, t);
              _a = a_buf;
            }

            if (dj < d_size.w) {
              CSize b_size;

              if (!is_b_t) {
                b_size.w = dj, b_size.h = dk;
              }
              else {
                b_size.w = dk, b_size.h = dj;
              }

              _b_step = b_size.w;
              icvGEMM_CopyBlock(_b, B_step * sizeof(arrtype), b_buf, _b_step * sizeof(arrtype), b_size, sizeof(arrtype));
              _b = b_buf;
            }

            if (dk0 < len) {
              const arrtype* _a_data = _a;
              const arrtype* _b_data = _b;
              double* _d_data = _d;
              CSize _a_size = a_bl_size, _d_size = cSize(dj, di);
              //icvGEMMBlockMul_64f(_a, _a_step, _b, _b_step, _d, _d_step, a_bl_size, cSize(dj, di), flags);
#include "icv_def_gemm_block_mul.inl"
            }
            else {
              //icvGEMMSingleMul_64f(_a, _a_step, _b, _b_step, _c, C_step, _d, _d_step, a_bl_size, cSize(dj, di), alpha, beta, flags);
              const arrtype* _a_data = _a;
              const arrtype* _b_data = _b;
              const arrtype* _c_data = _c;
              arrtype* _d_data = _d;
              size_t _c_step = C_step;
              CSize _a_size = a_bl_size, _d_size = cSize(dj, di);
#include "icv_def_gemm_single_mul.inl"
            }

            flags |= 16;
          }

          if (dk0 < len) {
            const arrtype* _c_data = _c;
            size_t _c_step = C_step;
            const double* d_buf = _d;
            size_t d_buf_step = _d_step;
            arrtype* _d_data = D + i * D_step + j;
            size_t _d_step = D_step;
            CSize _d_size = cSize(dj, di);
            //icvGEMMStore_64f(_c, C_step, _d, _d_step, D + i*D_step + j, D_step, cSize(dj,di), alpha, beta, flags);
#include "icv_def_gemm_store.inl"
          }
        }
      }

      if (block_buffer) {
        FREE(block_buffer);
      }
    }
    if (buffer) {
      FREE(buffer);
    }
  }

}