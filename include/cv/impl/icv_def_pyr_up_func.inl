
{
  worktype* buffer = (worktype*)buf;
  worktype* rows[PU_SZ];
  int y, top_row = 0;
  int Wd = size.w * 2, Wdn = Wd * Cs, Wn = size.w * Cs;
  int buffer_step = Wdn;
  int pu_sz = PU_SZ * buffer_step;
  int fst = 0, lst = size.h <= PU_SZ / 2 ? size.h : PU_SZ / 2 + 1;

  assert(Cs == 1 || Cs == 3);
  srcstep /= sizeof(src[0]);
  dststep /= sizeof(dst[0]);

  /* main loop */
  for (y = 0; y < size.h; y++, dst += 2 * dststep) {
    int x, y1, k = top_row;
    worktype* row0, *row1, *row2;
    arrtype* dst1;

    /* assign rows pointers */
    for (y1 = 0; y1 < PU_SZ; y1++) {
      rows[y1] = buffer + k;
      k += buffer_step;
      k &= k < pu_sz ? -1 : 0;
    }

    row0 = rows[0];
    row1 = rows[1];
    row2 = rows[2];
    dst1 = dst + dststep;

    /* fill new buffer rows with filtered source (horizontal conv) */
    if (Cs == 1)
      if (size.w > PU_SZ / 2)
        for (y1 = fst; y1 < lst; y1++, src += srcstep) {
          worktype* row = rows[y1];

          /* process left & right bounds */
          row[0] = PU_LT(src[0], src[1]);
          row[1] = PU_LT_ZI(src[0], src[1]);
          row[size.w * 2 - 2] = PU_RB(src[size.w - 2],
              src[size.w - 1]);
          row[size.w * 2 - 1] = PU_RB_ZI(src[size.w - 1]);

          /* other points */
          for (x = 1; x < size.w - 1; x++) {
            row[2 * x] = PU_FILTER(src[x - 1], src[x], src[x + 1]);
            row[2 * x + 1] = PU_FILTER_ZI(src[x], src[x + 1]);
          }
        }
      else                /* size.w <= PU_SZ/2 */
        for (y1 = fst; y1 < lst; y1++, src += srcstep) {
          worktype* row = rows[y1];
          worktype val = src[0];

          row[0] = PU_SINGULAR(val);
          row[1] = PU_SINGULAR_ZI(val);
        }
    else                    /* Cs == 3 */
      for (y1 = fst; y1 < lst; y1++, src += srcstep) {
        worktype* row = rows[y1];

        if (size.w > PU_SZ / 2) {
          int c;

          for (c = 0; c < 3; c++) {
            /* process left & right bounds  */
            row[c] = PU_LT(src[c], src[3 + c]);
            row[3 + c] = PU_LT_ZI(src[c], src[3 + c]);
            row[Wn * 2 - 6 + c] = PU_RB(src[Wn - 6 + c], src[Wn - 3 + c]);
            row[Wn * 2 - 3 + c] = PU_RB_ZI(src[Wn - 3 + c]);
          }

          /* other points */
          for (x = 3; x < Wn - 3; x += 3) {
            row[2 * x] = PU_FILTER(src[x - 3], src[x], src[x + 3]);
            row[2 * x + 3] = PU_FILTER_ZI(src[x], src[x + 3]);

            row[2 * x + 1] = PU_FILTER(src[x - 2], src[x + 1], src[x + 4]);
            row[2 * x + 4] = PU_FILTER_ZI(src[x + 1], src[x + 4]);

            row[2 * x + 2] = PU_FILTER(src[x - 1], src[x + 2], src[x + 5]);
            row[2 * x + 5] = PU_FILTER_ZI(src[x + 2], src[x + 5]);
          }
        }
        else {          /* size.w <= PU_SZ/2 */
          int c;

          for (c = 0; c < 3; c++) {
            row[c] = PU_SINGULAR(src[c]);
            row[3 + c] = PU_SINGULAR_ZI(src[c]);
          }
        }
      }

    /* second pass. Do vertical conv and write results do destination image */
    if (y > 0) {
      if (y < size.h - PU_SZ / 2) {
        for (x = 0; x < Wdn; x++) {
          dst[x] = (arrtype)_pu_scale_(PU_FILTER(row0[x], row1[x], row2[x]));
          dst1[x] = (arrtype)_pu_scale_(PU_FILTER_ZI(row1[x], row2[x]));
        }

        top_row += buffer_step;
        top_row &= top_row < pu_sz ? -1 : 0;
      }
      else                /* bottom */
        for (x = 0; x < Wdn; x++) {
          dst[x] = (arrtype)_pu_scale_(PU_RB(row0[x], row1[x]));
          dst1[x] = (arrtype)_pu_scale_(PU_RB_ZI(row1[x]));
        }
    }
    else {
      if (size.h > PU_SZ / 2)  /* top */
        for (x = 0; x < Wdn; x++) {
          dst[x] = (arrtype)_pu_scale_(PU_LT(row0[x], row1[x]));
          dst1[x] = (arrtype)_pu_scale_(PU_LT_ZI(row0[x], row1[x]));
        }
      else                /* size.h <= PU_SZ/2 */
        for (x = 0; x < Wdn; x++) {
          dst[x] = (arrtype)_pu_scale_(PU_SINGULAR(row0[x]));
          dst1[x] = (arrtype)_pu_scale_(PU_SINGULAR_ZI(row0[x]));
        }

      fst = PU_SZ - 1;
    }

    lst = y < size.h - PU_SZ / 2 - 1 ? PU_SZ : size.h + PU_SZ / 2 - y - 1;
  }


}
#undef _pu_scale_