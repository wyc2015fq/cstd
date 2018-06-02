
if (dc->isaa)
{
  void* pt0 = (void*)args;

  if (n > 0) {
    scancell s[1] = {0};
    const cell_aa* cells;
    int cellslen, scan_y = 0, min_y, max_y;

    switch (type) {
    case DCT_INT:
      scancell_set_i(s, (IPOINT*)pt0, lens, n, clip.l, clip.t, clip.r, clip.b, shift);
      break;

    case DCT_FLOAT:
      scancell_set_f(s, (FPOINT*)pt0, lens, n, clip.l, clip.t, clip.r, clip.b);
      break;

    case DCT_DOUBLE:
      scancell_set_d(s, (DPOINT*)pt0, lens, n, clip.l, clip.t, clip.r, clip.b);
      break;
    }

    //SCANCELL_SET(s);
    cells = s->cells, cellslen = s->celllen;
    max_y = BOUND(cells[cellslen - 1].y, clip.t, clip.b - 1);

    while (cells->y < 0) {
      ++cells;
    }

    min_y = BOUND(cells[0].y, clip.t, clip.b - 1);

    for (scan_y = min_y; scan_y <= max_y;) {
      int cover = 0;
      int y = scan_y;
      uchar* ptr = img + y * step;

      while (scan_y == cells->y) {
        const cell_aa* cur_cell = cells;
        int x = cur_cell->x, x_end;
        int area = cur_cell->area;
        unsigned char alpha = 0;
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
          alpha = calculate_alpha((cover << (poly_subpixel_shift + 1)) - area);

          if (alpha && x >= clip.l && x < clip.r) {
            ptr_x = ptr + x * cn;
            GET_PIX(pclr, x, y);
            alpha = (alpha * pclr[3]) >> 8;
            PUT_PIX(ptr_x, pclr, alpha);
          }

          x++;
        }

        if (x < cur_cell->x) {
          alpha = calculate_alpha(cover << (poly_subpixel_shift + 1));

          if (alpha) {
            x = MAX(x, 0);
            x_end = MIN(cur_cell->x, clip.r);
            ptr_x = ptr + x * cn;

            for (; x < x_end; ++x, ptr_x += cn) {
              GET_PIX(pclr, x, y);
              alpha = (alpha * pclr[3]) >> 8;
              PUT_PIX(ptr_x, pclr, alpha);
            }
          }
        }
      }

      ++scan_y;
    }

    scancell_free(s);
  }
}
else
{
  switch (shape) {
  case DCT_LINE: {
    IPOINT* pt = (IPOINT*)args;
    IPOINT pt1 = iPOINT(dc->pt.x + dc->org.x, dc->pt.y + dc->org.y);
    IPOINT pt2 = iPOINT(pt->x + dc->org.x, pt->y + dc->org.y);
    ThickLine(dc->im, pt1, pt2, dc->clrLine, dc->wline, CC_AA, 3, 0);
  }
  break;

  case DCT_RECT: {
    IRECT rc = *(IRECT*)args;
    IRECT rc1 = iRectInter(rc, clip);
    int x, y, xbeg = rc1.l - rc.l, xend = rc1.r - rc.l;
    //const uchar* pclr = 0;

    for (y = rc1.t - rc.t; y < rc1.b - rc.t; ++y) {
      uchar* ptr_x = im->tt.data + y * im->s + rc1.l * im->c;

      for (x = xbeg; x < xend; ++x, ptr_x += im->c) {
        GET_PIX(pclr, x, y);
        PUT_PIX(ptr_x, pclr, pclr[3]);
      }
    }
  }
  break;

  case DCT_ELLIPSE: {
    IRECT rc = *(IRECT*)args;
    int wline = dc->wline;
    int x1 = rc.l, x2 = rc.r, y1 = rc.t, y2 = rc.b;
    int xa, xb;
    int x, cx = (x1 + x2), w = (x2 - x1);
    int y, cy = (y1 + y2), h = (y2 - y1);
    __int64 h2 = h * h, w2 = w * w;
    __int64 hw2 = h2 * w2;
    __int64 h3 = (h - wline) * (h - wline), w3 = (w - wline) * (w - wline);
    __int64 hw3 = h3 * w3;
    COLOR clrFill = dc->clrFill;
    COLOR clrLine = dc->clrLine;
    const uchar* pclr = (const uchar*) & (clrFill);
    const uchar* pclr1 = (const uchar*) & (clrLine);
    uchar* pix_x1, *pix_x2;
    uchar alpha = pclr[3];
    uchar alpha1 = pclr1[3];

    if (cy & 1) {
      xa = x1, xb = x2;

      if (clrLine) {
        int xt = (x2 - wline);
        y = (cy / 2);
        pix_x1 = im->tt.data + y * im->s + xa * im->c;
        pix_x2 = im->tt.data + y * im->s + xt * im->c;

        for (x = xa; x < xa + wline; ++x, ++xt, pix_x1 += im->c, pix_x2 += im->c) {
          PUT_PIX(pix_x1, pclr1, alpha1);
          PUT_PIX(pix_x2, pclr1, alpha1);
        }

        xa = x1 + wline, xb = x2 - wline;
      }

      if (clrFill) {
        pix_x1 = im->tt.data + (cy / 2) * im->s + xa * im->c;

        for (x = xa; x < xb; ++x, pix_x1 += im->c) {
          GET_PIX(pclr, x, y);
          PUT_PIX(pix_x1, pclr, alpha);
        }
      }
    }

    for (y = cy / 2; y-- > y1;) {
      int y1 = cy - 1 - y;
      uchar* pix_y1 = im->tt.data + y * im->s;
      uchar* pix_y2 = im->tt.data + y1 * im->s;

      for (x = x1; x1 < x2; ++x, ++x1, --x2) {
        int dy = 2 * y - cy + 1, dx = 2 * x - cx + 1;
        __int64 d = dy * dy * w2 + dx * dx * h2;

        if (d <= hw2) {
          break;
        }
      }

      xa = x1, xb = x2;

      if (clrLine) {
        int x3 = x1, x4 = x2, xt;
        uchar* pix_x3, *pix_x4;

        for (x = x1; x3 < x4; ++x, ++x3, --x4) {
          int dy = 2 * y - cy + 1, dx = 2 * x - cx + 1;
          __int64 d = dy * dy * w2 + dx * dx * h2;

          if (d < hw3) {
            break;
          }
        }

        pix_x1 = pix_y1 + x1 * im->c;
        pix_x2 = pix_y2 + x1 * im->c;
        pix_x3 = pix_y1 + x4 * im->c;
        pix_x4 = pix_y2 + x4 * im->c;
        xt = x4;

        for (x = x1; x < x3; ++x, ++xt, pix_x1 += im->c, pix_x2 += im->c, pix_x3 += im->c, pix_x4 += im->c) {
          PUT_PIX(pix_x1, pclr1, alpha1);
          PUT_PIX(pix_x2, pclr1, alpha1);
          PUT_PIX(pix_x3, pclr1, alpha1);
          PUT_PIX(pix_x4, pclr1, alpha1);
        }

        xa = x3, xb = x4;
      }

      if (clrFill) {
        pix_x1 = pix_y1 + xa * im->c;
        pix_x2 = pix_y2 + xa * im->c;

        for (x = xa; x < xb; ++x, pix_x1 += im->c, pix_x2 += im->c) {
          GET_PIX(pclr, x, y);
          PUT_PIX(pix_x1, pclr, alpha);
          GET_PIX(pclr, x, y1);
          PUT_PIX(pix_x2, pclr, alpha);
        }
      }
    }
  }
  break;

  default:
    break;
  }
}

#undef GET_PIX
