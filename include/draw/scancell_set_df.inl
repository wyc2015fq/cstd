{

  const point_type* pt;
  int i, j, len, maxcells = 0, n3, k, t;
  DPOINT pt3[10];
  rect_d rd[1];
  //DPOINT lt, rb;
  //rb = lt = pt0[0];

  rect_init(rd, left, top, right, bottom);

  for (pt = pt0, j = 0; j < n; ++j) {
    len = lens[j];

    for (i = 0; i < len; ++i) {
      int j = (i + 1) % len;
      //lt.x = MIN(lt.x, pt[i].x);
      //lt.y = MIN(lt.y, pt[i].y);
      //rb.x = MAX(rb.x, pt[i].x);
      //rb.y = MAX(rb.y, pt[i].y);
      n3 = clip_line_to(pt3, rd, pt[i].x, pt[i].y, pt[j].x, pt[j].y);

      for (k = 0; k < n3; k += 2) {
        t = (int)ceil(fabs(pt3[k].x - pt3[k + 1].x) + fabs(pt3[k].y - pt3[k + 1].y));
        maxcells += (t) + 5;
      }
    }

    pt += len;
  }

  //lt.x = BOUND(lt.x, left, right);
  //lt.y = BOUND(lt.y, top, bottom);
  //rb.x = BOUND(rb.x, left, right);
  //rb.y = BOUND(rb.y, top, bottom);
  if (0 >= maxcells) {
    return 0;
  }

  GREALLOC(cell_aa, s->cells, maxcells);
  ASSERT(s->cells);
  memset(s->cells, 0, sizeof(cell_aa)*maxcells);
  s->celllen = 0;
  s->maxcells = maxcells;
#define INICELL(_c) ((_c)->x=(_c)->y=0x7fffffff,(_c)->cover=(_c)->area=0)
  INICELL(&s->m_curr_cell);
#undef INICELL

  for (pt = pt0, j = 0; j < n; ++j) {
    len = lens[j];

    for (i = 0; i < len; ++i) {
      int j = (i + 1) % len;
      n3 = clip_line_to(pt3, rd, pt[i].x, pt[i].y, pt[j].x, pt[j].y);

      for (k = 0; k < n3; k += 2) {
        render_line(upscale(pt3[k].x), upscale(pt3[k].y), upscale(pt3[k + 1].x), upscale(pt3[k + 1].y), s);
      }
    }

    pt += len;
  }

  set_curr_cell(s, 0x7FFFFFFF, 0x7FFFFFFF);
#define cell_aa_less(_a, _b) (((_a).y<(_b).y) || ((_a).y==(_b).y && (_a).x<(_b).x))
  //#define packcell(_a) (unsigned __int64)((((unsigned __int64)((_a).y))<<32)|((_a).x))
  //#define cell_aa_less(_a, _b) (packcell(_a)<packcell(_b))
  QSORT(s->cells, s->cells + s->celllen, cell_aa_less, cell_aa);
#undef cell_aa_less
}