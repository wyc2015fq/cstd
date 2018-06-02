if (n>0) {
  IMARGDEFTT;
  scancell s[1]={0};
  cell_aa* cells;
  int i=0, cellslen, scan_y=0, min_y, max_y;
  IRECT clip = {0, 0, w, h};
  if (pclip) {
    clip = iRectInter(clip, *pclip);
  }
  SCANCELL_SET(s);
  if (NULL==s->cells) return 0;
  cells = s->cells, cellslen = s->celllen;
  max_y = BOUND(cells[cellslen - 1].y, 0, h - 1);

  while (cells->y < 0) {
    ++cells;
  }

  min_y = BOUND(cells[0].y, 0, h-1);
  
  for (scan_y = min_y; scan_y <= max_y;) {
    int cover = 0;
    int y = scan_y;
    uchar* ptr = img + y * step;

    while (scan_y == cells->y) {
      const cell_aa* cur_cell = cells;
      int x = cur_cell->x, x_end, area = cur_cell->area;
      unsigned char alpha=0;
      uchar* ptr_x;
      cover += cur_cell->cover;

      //accumulate all cells with the same X
      while (scan_y == cells->y) {
        cur_cell = ++cells;

        if (cur_cell->x != x || cur_cell->y != scan_y) {
          break;
        }

        area += cur_cell->area;
        cover += cur_cell->cover;
      }

      if (area) {
        area = (cover << (poly_subpixel_shift + 1)) - area;
        CALCULATE_ALPHA(alpha, area);
        alpha = br->alpha_lut[alpha];

        if (alpha && x >= 0 && x < w) {
          GETCLR_INIT(y);
          ptr_x = ptr + x * cn;
          PUT_PIX(ptr_x, alpha, x, y);
        }

        x++;
      }
      if (x < cur_cell->x) {
        area = cover << (poly_subpixel_shift + 1);
        CALCULATE_ALPHA(alpha, area);
        alpha = br->alpha_lut[alpha];

        if (alpha) {
          x = MAX(x, 0);
          x_end = MIN(cur_cell->x, w);
          ptr_x = ptr + x * cn;
          {
            GETCLR_INIT(y);
            for (; x < x_end; ++x, ptr_x+=cn) {
              PUT_PIX(ptr_x, alpha, x, y);
            }
          }
        }
      }
    }

    ++scan_y;
  }

  scancell_free(s);
}

#undef PUT_PIX