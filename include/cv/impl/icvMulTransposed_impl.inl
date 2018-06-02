{
#define load_macro CC_NOP
  enum { gemm_level = 100 }; // boundary above which GEMM is faster.

  // if (src == dst || stype == dtype && (dst->cols >= gemm_level && dst->rows >= gemm_level && src->cols >= gemm_level && src->rows >= gemm_level)) {
  //   if (deltaarr) {
  //     CC_CALL(src2 = cvCreateMat(src->rows, src->cols, src->tid));
  //     srctype* src2 = 0;
  //     cvRepeat(delta, src2);
  //     cvSub(src, src2, src2);
  //     src = src2;
  //   }
  //   cvGEMM(src, src, scale, 0, 0, dst, order == 0 ? CC_GEMM_B_T : CC_GEMM_A_T);
  // }

  if (order) {
    // icvMulTransposedR_8u32f
    int i, j, k;
    dsttype* tdst = dst;
    dsttype* col_buf = 0;
    dsttype* delta_buf = 0;
    int buf_size = size.h;

    if (delta && delta_cols < size.w) {
      assert(delta_cols == 1);
      buf_size += 4 * buf_size;
    }

    col_buf = MALLOC(dsttype, buf_size);

    if (delta && delta_cols < size.w) {
      delta_buf = col_buf + size.h;

      for (i = 0; i < size.h; i++) {
        delta_buf[i * 4] = delta_buf[i * 4 + 1] = delta_buf[i * 4 + 2] = delta_buf[i * 4 + 3] = delta[i * deltastep];
      }

      delta = delta_buf;
      deltastep = deltastep ? 4 : 0;
    }

    if (!delta) {
      for (i = 0; i < size.w; i++, tdst += dststep) {
        for (k = 0; k < size.h; k++) {
          col_buf[k] = src[k * srcstep + i];
        }

        for (j = i; j <= size.w - 4; j += 4) {
          double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
          const srctype* tsrc = src + j;

          for (k = 0; k < size.h; k++, tsrc += srcstep) {
            double a = col_buf[k];
            s0 += a * load_macro(tsrc[0]);
            s1 += a * load_macro(tsrc[1]);
            s2 += a * load_macro(tsrc[2]);
            s3 += a * load_macro(tsrc[3]);
          }

          tdst[j] = (dsttype)(s0 * scale);
          tdst[j + 1] = (dsttype)(s1 * scale);
          tdst[j + 2] = (dsttype)(s2 * scale);
          tdst[j + 3] = (dsttype)(s3 * scale);
        }

        for (; j < size.w; j++) {
          double s0 = 0;
          const srctype* tsrc = src + j;

          for (k = 0; k < size.h; k++, tsrc += srcstep) {
            s0 += col_buf[k] * tsrc[0];
          }

          tdst[j] = (dsttype)(s0 * scale);
        }
      }
    }
    else {
      for (i = 0; i < size.w; i++, tdst += dststep) {
        if (!delta_buf) {
          for (k = 0; k < size.h; k++) {
            col_buf[k] = load_macro(src[k * srcstep + i]) - delta[k * deltastep + i];
          }
        }
        else {
          for (k = 0; k < size.h; k++) {
            col_buf[k] = load_macro(src[k * srcstep + i]) - delta_buf[k * deltastep];
          }
        }

        for (j = i; j <= size.w - 4; j += 4) {
          double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
          const srctype* tsrc = src + j;
          const dsttype* d = delta_buf ? delta_buf : delta + j;

          for (k = 0; k < size.h; k++, tsrc += srcstep, d += deltastep) {
            double a = col_buf[k];
            s0 += a * (load_macro(tsrc[0]) - d[0]);
            s1 += a * (load_macro(tsrc[1]) - d[1]);
            s2 += a * (load_macro(tsrc[2]) - d[2]);
            s3 += a * (load_macro(tsrc[3]) - d[3]);
          }

          tdst[j] = (dsttype)(s0 * scale);
          tdst[j + 1] = (dsttype)(s1 * scale);
          tdst[j + 2] = (dsttype)(s2 * scale);
          tdst[j + 3] = (dsttype)(s3 * scale);
        }

        for (; j < size.w; j++) {
          double s0 = 0;
          const srctype* tsrc = src + j;
          const dsttype* d = delta_buf ? delta_buf : delta + j;

          for (k = 0; k < size.h; k++, tsrc += srcstep, d += deltastep) {
            s0 += col_buf[k] * (load_macro(tsrc[0]) - d[0]);
          }

          tdst[j] = (dsttype)(s0 * scale);
        }
      }
    }

    /* fill the lower part of the destination matrix */
    for (i = 1; i < size.w; i++) {
      for (j = 0; j < i; j++) {
        dst[dststep * i + j] = dst[dststep * j + i];
      }
    }

    if (col_buf) {
      FREE(col_buf);
    }
  }
  else {
    // icvMulTransposedL_8u32f
    int i, j, k;
    dsttype* tdst = dst;

    if (!delta) {
      for (i = 0; i < size.h; i++, tdst += dststep) {
        for (j = i; j < size.h; j++) {
          double s = 0;
          const srctype* tsrc1 = src + i * srcstep;
          const srctype* tsrc2 = src + j * srcstep;

          for (k = 0; k <= size.w - 4; k += 4)
            s += tsrc1[k] * tsrc2[k] + tsrc1[k + 1] * tsrc2[k + 1] +
            tsrc1[k + 2] * tsrc2[k + 2] + tsrc1[k + 3] * tsrc2[k + 3];

          for (; k < size.w; k++) {
            s += tsrc1[k] * tsrc2[k];
          }

          tdst[j] = (dsttype)(s * scale);
        }
      }
    }
    else {
      dsttype* row_buf = 0;
      int local_alloc = 0;
      int buf_size = size.w;
      dsttype delta_buf[4];
      int delta_shift = delta_cols == size.w ? 4 : 0;

      row_buf = MALLOC(dsttype, buf_size);

      for (i = 0; i < size.h; i++, tdst += dststep) {
        const srctype* tsrc1 = src + i * srcstep;
        const dsttype* tdelta1 = delta + i * deltastep;

        if (delta_cols < size.w)
          for (k = 0; k < size.w; k++) {
            row_buf[k] = tsrc1[k] - tdelta1[0];
          }
        else
          for (k = 0; k < size.w; k++) {
            row_buf[k] = tsrc1[k] - tdelta1[k];
          }

        for (j = i; j < size.h; j++) {
          double s = 0;
          const srctype* tsrc2 = src + j * srcstep;
          const dsttype* tdelta2 = delta + j * deltastep;

          if (delta_cols < size.w) {
            delta_buf[0] = delta_buf[1] = delta_buf[2] = delta_buf[3] = tdelta2[0];
            tdelta2 = delta_buf;
          }

          for (k = 0; k <= size.w - 4; k += 4, tdelta2 += delta_shift) {
            s += row_buf[k] * (load_macro(tsrc2[k]) - tdelta2[0]) +
                row_buf[k + 1] * (load_macro(tsrc2[k + 1]) - tdelta2[1]) +
                row_buf[k + 2] * (load_macro(tsrc2[k + 2]) - tdelta2[2]) +
                row_buf[k + 3] * (load_macro(tsrc2[k + 3]) - tdelta2[3]);
          }

          for (; k < size.w; k++, tdelta2++) {
            s += row_buf[k] * (load_macro(tsrc2[k]) - tdelta2[0]);
          }

          tdst[j] = (dsttype)(s * scale);
        }
      }

      if (row_buf) {
        FREE(row_buf);
      }
    }

    /* fill the lower part of the destination matrix */
    for (j = 0; j < size.h - 1; j++) {
      for (i = j; i < size.h; i++) {
        dst[dststep * i + j] = dst[dststep * j + i];
      }
    }
  }
}
#undef load_macro
