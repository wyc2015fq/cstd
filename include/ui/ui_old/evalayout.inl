
typedef struct evadata_t {
  char type;
  uchar per; // 百分比
  short data;
} evadata_t;

typedef struct evalayout {
  evadata_t* ro;
  evadata_t* co;
  evadata_t* id;
  SIZE margin;
  SIZE gap;
  int rows, cols;
} evalayout;

int evalayout_set(evalayout* lay, str_t s)
{
  int i, j, k, t;
  str_t s0 = s;
  str_t s1 = STRsplit(s, '\n', " \t\r\n", &s);
  lay->rows = lay->cols = 0;
  sscanf(s1.s, "EvaLayout,%d,%d,%d,%d", &lay->margin.w, &lay->margin.h, &lay->gap.w, &lay->gap.h);
  s0 = s;

  for (j = 0; s.l > 0; ++j) {
    str_t s1 = STRsplit(s, '\n', "\r\n", &s);
    s1 = STRtrim(s1, " \t\r\n");

    if (0 < s1.l) {
      for (i = 0; 0 < s1.l; ++i) {
        STRsplit(s1, ',', " \t", &s1);
      }

      if (i > lay->cols) {
        lay->cols = i;
      }

      lay->rows++;
    }
  }

  lay->id = (evadata_t*)my_realloc(lay->id, 0, sizeof(evadata_t) * lay->cols * lay->rows);
  lay->ro = lay->id + (lay->cols - 1) * (lay->rows - 1);
  lay->co = lay->ro + (lay->rows - 1);

  for (i = 0; i < (lay->cols - 1) * (lay->rows - 1); ++i) {
    lay->id[i].type = LAY_NULL;
    lay->id[i].data = 0;
    lay->id[i].per = 0;
  }

  s = s0;

  for (j = 0; 0 < s.l;) {
    str_t s2, s1 = STRsplit(s, '\n', " \t\r\n", &s);

    if (0 < s1.l) {
      j++;
    }

    for (i = 0; 0 < s1.l && i < lay->cols; ++i) {
      s2 = STRsplit(s1, ',', " \t", &s1);
      t = k = 0;

      if (1 == j || 0 == i) {
        if (0 < s2.l) {
          if (!isnmb(s2.s[0])) {
            t = s2.s[0];
            k = atoi(s2.s + 1);
          }
          else {
            k = atoi(s2.s);
          }
        }
        else {
          t = LAY_HEADER_ADAPT;
          k = 0 == j ? 20 : 100;
        }

        if (i > 0) {
          lay->co[i - 1].data = k;
          lay->co[i - 1].type = t;
        }
        else if (j > 1) {
          lay->ro[j - 2].data = k;
          lay->ro[j - 2].type = t;
        }
      }
      else {
        if (0 < s2.l) {
          if (!isnmb(s2.s[0])) {
            t = s2.s[0];
            k = atoi(s2.s + 1);
          }
          else {
            t = 0;
            k = atoi(s2.s);
            ASSERT(k >= 0);
          }
        }
        else {
          t = LAY_NULL;
          k = 0;
        }

        lay->id[(j - 2) * (lay->cols - 1) + (i - 1)].type = t;
        lay->id[(j - 2) * (lay->cols - 1) + (i - 1)].data = k;
      }

    }
  }

  lay->rows--;
  lay->cols--;
  return 1;
}

int evalayout_setsize_1d(int n, evadata_t* ro, int gap, int cx)
{
  int i, cn = 0;
  cx -= gap * (n - 1);

  for (i = 0; i < n; ++i) {
    if (ro[i].type != LAY_HEADER_EXPAND) {
      cx -= ro[i].data;
    }
    else {
      cn += ro[i].data;
    }
  }

  if (100 > cn) {
    cn = 0;

    for (i = 0; i < n; ++i) {
      if (ro[i].type == LAY_HEADER_EXPAND) {
        ro[i].data = ro[i].per;
        cn += ro[i].per;
      }
    }
  }

  if (1 > cn) {
    cn = 0;

    for (i = 0; i < n; ++i) {
      if (ro[i].type == LAY_HEADER_EXPAND) {
        ro[i].data = ro[i].per = 1;
        cn += ro[i].per;
      }
    }
  }

  for (i = 0; i < n; ++i) {
    if (ro[i].type == LAY_HEADER_EXPAND) {
      int d = ro[i].data;
      ro[i].data = cx * d / cn;
      //ro[i].per = 100 * d / cn;
    }
  }

  return 0;
}

// pt 左上角坐标
int evalayout_getrect(evalayout* lay0, int pos, RECT rc, uictrl** out, POINT hitpt, POINT* phit)
{
  evalayout* lay = lay0 + pos;
  int i, j, k, al = lay->cols, ret = 0;
  POINT pt = RCLT(&rc);
  RECT rc0;

  if (out) {
    evalayout_setsize_1d(lay->cols, lay->co, lay->gap.w, RCW(&rc) - lay->margin.w * 2);
    evalayout_setsize_1d(lay->rows, lay->ro, lay->gap.h, RCH(&rc) - lay->margin.h * 2);
  }

  pt.x += lay->margin.w;
  pt.y += lay->margin.h;

  *phit = iPOINT(-1, -1);

  for (i = 0; i < lay->rows; pt.y += lay->ro[i].data + lay->gap.h, ++i) {
    POINT pt0 = pt, pt1;

    for (j = 0; j < lay->cols; pt0.x += lay->co[j].data + lay->gap.w, ++j) {
      int idx, t = lay->id[i * al + j].type;

      if (0 == t) {
        idx = lay->id[i * al + j].data;
        pt1.x = pt0.x + lay->co[j].data;
        pt1.y = pt0.y + lay->ro[i].data;

        for (k = j + 1; k < lay->cols && LAY_EXPAND_HORIZONTAL == lay->id[i * al + k].type; ++k) {
          pt1.x += lay->co[k].data + lay->gap.w;
        }

        for (k = i + 1; k < lay->rows && LAY_EXPAND_VERTICAL == lay->id[k * al + j].type; ++k) {
          pt1.y += lay->ro[k].data + lay->gap.h;
        }

        rc0 = iRECT(pt0.x, pt0.y, pt1.x, pt1.y);

        // 只判断左边界和上边界 5 = (1<<0 | 1<<2)
        if (0 == (ret & 5)) {
          ret = PtInRectBorder(&rc0, hitpt, lay->gap.w, lay->gap.h, lay->gap.w, lay->gap.h);
        }

        if (out) {
          out[idx]->rc = rc0;
        }

        if (phit->x < 0 && 1 & ret) {
          phit->x = j - 1;
        }

        if (phit->y < 0 && 4 & ret) {
          phit->y = i - 1;
        }
      }
    }
  }

  return ret;
}
