
{
  worktype*  buffer = (worktype*)buf;  /* pointer to temporary buffer */
  worktype*  rows[PD_SZ]; /* array of rows pointers. dim(rows) is PD_SZ */
  int  y, top_row = 0;
  int  Wd = size.w / 2, Wdn = Wd * Cs;
  int  buffer_step = Wdn;
  int  pd_sz = (PD_SZ + 1) * buffer_step;
  int  fst = 0, lst = size.h <= PD_SZ / 2 ? size.h : PD_SZ / 2 + 1;

  assert(Cs == 1 || Cs == 3);
  srcstep /= sizeof(src[0]);
  dststep /= sizeof(dst[0]);

  /* main loop */
  for (y = 0; y < size.h; y += 2, dst += dststep) {
    /* set first and last indices of buffer rows which are need to be filled */
    int x, y1, k = top_row;
    int x1 = buffer_step;
    worktype* row01, *row23, *row4;

    /* assign rows pointers */
    for (y1 = 0; y1 < PD_SZ; y1++) {
      rows[y1] = buffer + k;
      k += buffer_step;
      k &= k < pd_sz ? -1 : 0;
    }

    row01 = rows[0];
    row23 = rows[2];
    row4  = rows[4];

    /* fill new buffer rows with filtered source (horizontal conv) */
    if (Cs == 1) {
      if (size.w > PD_SZ / 2)
        for (y1 = fst; y1 < lst; y1++, src += srcstep) {
          worktype* row = rows[y1];

          /* process left & right bounds */
          row[0]    = PD_LT(src[0], src[1], src[2]);
          row[Wd - 1] = PD_RB(src[Wd * 2 - 4], src[Wd * 2 - 3],
              src[Wd * 2 - 2], src[Wd * 2 - 1]);

          /* other points (even) */
          for (x = 1; x < Wd - 1; x++) {
            row[x] = PD_FILTER(src[2 * x - 2], src[2 * x - 1], src[2 * x],
                src[2 * x + 1], src[2 * x + 2]);
          }
        }
      else
        for (y1 = fst; y1 < lst; y1++, src += srcstep) {
          rows[y1][0] = PD_SINGULAR(src[0], src[1]);
        }
    }
    else { /* Cs == 3 */
      for (y1 = fst; y1 < lst; y1++, src += srcstep) {
        worktype* row = rows[y1];

        if (size.w > PD_SZ / 2) {
          int c;

          for (c = 0; c < 3; c++) {
            /* process left & right bounds  */
            row[c] = PD_LT(src[c], src[3 + c], src[6 + c]);
            row[Wdn - 3 + c] = PD_RB(src[Wdn * 2 - 12 + c], src[Wdn * 2 - 9 + c],
                src[Wdn * 2 - 6 + c], src[Wdn * 2 - 3 + c]);
          }

          /* other points (even) */
          for (x = 3; x < Wdn - 3; x += 3) {
            row[x]   = PD_FILTER(src[2 * x - 6], src[2 * x - 3], src[2 * x],
                src[2 * x + 3], src[2 * x + 6]);
            row[x + 1] = PD_FILTER(src[2 * x - 5], src[2 * x - 2], src[2 * x + 1],
                src[2 * x + 4], src[2 * x + 7]);
            row[x + 2] = PD_FILTER(src[2 * x - 4], src[2 * x - 1], src[2 * x + 2],
                src[2 * x + 5], src[2 * x + 8]);
          }
        }
        else { /* size.w <= PD_SZ/2 */
          row[0] = PD_SINGULAR(src[0], src[3]);
          row[1] = PD_SINGULAR(src[1], src[4]);
          row[2] = PD_SINGULAR(src[2], src[5]);
        }
      }
    }

    /* second pass. Do vertical conv and write results do destination image */
    if (y > 0) {
      if (y < size.h - PD_SZ / 2) {
        for (x = 0; x < Wdn; x++, x1++) {
          dst[x] = (arrtype)_pd_scale_(PD_FILTER(row01[x],  row01[x1],
              row23[x], row23[x1], row4[x]));
        }

        top_row += 2 * buffer_step;
        top_row &= top_row < pd_sz ? -1 : 0;
      }
      else /* bottom */
        for (x = 0; x < Wdn; x++, x1++)
          dst[x] = (arrtype)_pd_scale_(PD_RB(row01[x], row01[x1],
              row23[x], row23[x1]));
    }
    else {
      if (size.h > PD_SZ / 2) { /* top */
        for (x = 0; x < Wdn; x++, x1++) {
          dst[x] = (arrtype)_pd_scale_(PD_LT(row01[x], row01[x1], row23[x]));
        }
      }
      else { /* size.h <= PD_SZ/2 */
        for (x = 0; x < Wdn; x++, x1++) {
          dst[x] = (arrtype)_pd_scale_(PD_SINGULAR(row01[x], row01[x1]));
        }
      }

      fst = PD_SZ - 2;
    }

    lst = y + 2 + PD_SZ / 2 < size.h ? PD_SZ : size.h - y;
  }
}

#undef _pd_scale_
