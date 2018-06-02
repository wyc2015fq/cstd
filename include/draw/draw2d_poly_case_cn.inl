#define PUT_PIX(dst, _alpha, _x, _y) { GETCLR(_x, _y); PUTCLR(dst, _alpha, _x, _y); }
if (nvtx > 0 && vtx)
{
  int i;
  for (i = 0; i < nvtx; ++i) {
    int x = (int)vtx[i].pos.x;
    int y = (int)vtx[i].pos.y;
    GETCLR_INIT(y);
    GETCLR(x, y);
    vtx[i].col = *(COLOR*)pclr;
  }
}
if (nrcuv > 0 && prcuv && tex1)
{
  IMARGDEFTT;
  int i, j, x, y;
  void* buf = NULL;
  int* xoff = NULL;
  int* yoff = NULL;
  IRECT clip = {0, 0, w, h}, rect;
  if (pclip) {
    clip = iRectInter(clip, *pclip);
  }
  for (i = 0; i < nrcuv; ++i) {
    IRECT drc = iRECT((int)prcuv[i].a.x, (int)prcuv[i].a.y, (int)prcuv[i].c.x, (int)prcuv[i].c.y);
    int dw = RCW(&drc), dh = RCH(&drc);
    ImVec2 uv_a = prcuv[i].uv_a;
    ImVec2 uv_c = prcuv[i].uv_c;
    float uv_w = (uv_c.x - uv_a.x) / dw;
    float uv_h = (uv_c.y - uv_a.y) / dh;
    rect = iRectInter(clip, drc);
    multialloc(&buf, &xoff, dw*sizeof(int), &yoff, dh*sizeof(int), NULL);
    for (j = 0; j < dw; ++j) {
      xoff[j] = (int)((uv_a.x + uv_w * j) * tex1->w);
    }
    for (j = 0; j < dh; ++j) {
      yoff[j] = (int)((uv_a.y + uv_h * j) * tex1->h);
    }
    if (RCH(&rect) > 0 && RCW(&rect) > 0) {
      switch (tex1->bpp) {
      case 1:
        if (alphamode) {
          for (y = rect.t; y < rect.b; ++y) {
            uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
            uchar* ptr_x = ptr + rect.l * 4;
            uchar* psrc = tex1->data + yoff[y - drc.t] * tex1->s;
            uchar* bit = psrc;
            GETCLR_INIT(y);
            for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
              int k = xoff[x - drc.l];
              if ((bit[k >> 3] >> (7 - (k & 7))) & 1) {
                PUT_PIX(ptr_x, 255, x, y);
              }
            }
          }
        } else {
          for (y = rect.t; y < rect.b; ++y) {
            uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
            uint32* ptr_x = (uint32*)(ptr + rect.l * 4);
            uchar* psrc = tex1->data + yoff[y - drc.t] * tex1->s;
            uchar* bit = psrc;
            GETCLR_INIT(y);
            for (x = rect.l; x < rect.r; ++x) {
              int k = xoff[x - drc.l];
              if ((bit[k >> 3] >> (7 - (k & 7))) & 1) {
                *ptr_x++ = 0xffffffff;
              }
            }
          }
        }
        break;
      case 8:
        if (alphamode) {
          for (y = rect.t; y < rect.b; ++y) {
            uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
            uchar* ptr_x = ptr + rect.l * 4;
            uchar* psrc = tex1->data + yoff[y - drc.t] * tex1->s;
            GETCLR_INIT(y);
            for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
              uchar alpha = psrc[xoff[x - drc.l]];
              PUT_PIX(ptr_x, alpha, x, y);
            }
          }
        } else {
          for (y = rect.t; y < rect.b; ++y) {
            uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
            uint32* ptr_x = (uint32*)(ptr + rect.l * 4);
            uchar* psrc = tex1->data + yoff[y - drc.t] * tex1->s;
            GETCLR_INIT(y);
            for (x = rect.l; x < rect.r; ++x) {
              uchar gry = psrc[xoff[x - drc.l]];
              *ptr_x++ = _RGB(gry, gry, gry);
            }
          }
        }
        break;
      case 24:
        if (alphamode) {
          for (y = rect.t; y < rect.b; ++y) {
            uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
            uchar* ptr_x = (uchar*)(ptr + rect.l * 4);
            uchar* psrc = (uchar*)(tex1->data + yoff[y - drc.t] * tex1->s);
            GETCLR_INIT(y);
            for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
              uchar* clr1 = psrc+3*xoff[x - drc.l];
              uchar alpha = clr1[0];
              PUT_PIX(ptr_x, alpha, x, y);
            }
          }
        } else {
          //show_bitmap("asdf", tex1->h, tex1->w, tex1->data, tex1->s, PF_32bppARGB, 1, 1);waitkey(-1);
          for (y = rect.t; y < rect.b; ++y) {
            uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
            uint32* ptr_x = (uint32*)(ptr + rect.l * 4);
            uchar* psrc = (uchar*)(tex1->data + yoff[y - drc.t] * tex1->s);
            for (x = rect.l; x < rect.r; ++x) {
              uchar* clr1 = psrc+3*xoff[x - drc.l];
              *ptr_x++ = _RGB(clr1[0], clr1[1], clr1[2]);
            }
          }
        }
        break;
      case 32:
        if (alphamode) {
          for (y = rect.t; y < rect.b; ++y) {
            uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
            uchar* ptr_x = (uchar*)(ptr + rect.l * 4);
            uint32* psrc = (uint32*)(tex1->data + yoff[y - drc.t] * tex1->s);
            GETCLR_INIT(y);
            for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
              uint32 clr1 = psrc[xoff[x - drc.l]];
              uchar alpha = GetAV(clr1);
              PUT_PIX(ptr_x, alpha, x, y);
            }
          }
        } else {
          //show_bitmap("asdf", tex1->h, tex1->w, tex1->data, tex1->s, PF_32bppARGB, 1, 1);waitkey(-1);
          for (y = rect.t; y < rect.b; ++y) {
            uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
            uchar* ptr_x = (uchar*)(ptr + rect.l * 4);
            uint32* psrc = (uint32*)(tex1->data + yoff[y - drc.t] * tex1->s);
            for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
              const uchar* pclr = (const uchar*)&psrc[xoff[x - drc.l]];
              PIXBLEND3(ptr_x, pclr[3], x, y);
            }
          }
        }
        break;
      default:
        ASSERT(0);
        break;
      }
    }
  }
  FREE(buf);
}
if (n > 0 && im && (im->h * im->w * im->c) > 0)
{
  IMARGDEFTT;
  scancell s[1] = {0};
  cell_aa* cells;
  int i = 0, cellslen, scan_y = 0, min_y, max_y;
  IRECT clip = {0, 0, w, h}, rect = {0};
  int is_irc = is_int_rect(pt0, lens, n, &rect);
  if (pclip) {
    clip = iRectInter(clip, *pclip);
  }
  if (is_irc) {
    int x, y;
    rect = iRectInter(clip, rect);
    for (y = rect.t; y < rect.b; ++y) {
      uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
      uchar* ptr_x = ptr + rect.l * 4;
      GETCLR_INIT(y);
      for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
        PUT_PIX(ptr_x, 255, x, y);
      }
    }
    return 0;
  }
  //SCANCELL_SET(s);
  scancell_set_d(s, pt0, lens, n, &clip);
  if (NULL == s->cells) {
    return 0;
  }
  cells = s->cells, cellslen = s->celllen;
  max_y = BOUND(cells[cellslen - 1].y, 0, h - 1);
  while (cells->y < 0) {
    ++cells;
  }
  min_y = BOUND(cells[0].y, 0, h - 1);
  for (scan_y = min_y; scan_y <= max_y;) {
    int cover = 0;
    int y = scan_y;
    uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
    while (scan_y == cells->y) {
      const cell_aa* cur_cell = cells;
      int x = cur_cell->x, x_end, area = cur_cell->area;
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
            for (; x < x_end; ++x, ptr_x += cn) {
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
#undef GETCLR
#undef GETCLR_INIT

