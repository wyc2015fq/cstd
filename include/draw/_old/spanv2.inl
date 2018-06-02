
#define BOOLOPT_DEF_DEF(BOOLOPT_DEF) \
  BOOLOPT_DEF(NUL) \
  BOOLOPT_DEF(AND) \
  BOOLOPT_DEF(OR) \
  BOOLOPT_DEF(XOR) \
  BOOLOPT_DEF(DIFF) \
BOOLOPT_DEF(COPY)

typedef enum {
#define BOOLOPT_DEF(name) RGN_##name ,
  BOOLOPT_DEF_DEF(BOOLOPT_DEF)
#undef BOOLOPT_DEF
} boolopt;


typedef struct {
  int b, e, y, cover;
} span_t;
typedef struct {
  span_t* s;
  int* pos;
  int n;
  int l;
  int max_l;
  IRECT rc;//bound box
} spanv_t;
span_t* span_push(span_t* sl, span_t* spanb, span_t* spane, int b, int e, int cover)
{
  if (cover > 0) {
    if (sl > spanb) {
      ASSERT(sl[-1].e <= b);
    }
    if (sl > spanb && b == sl[-1].e && cover == sl[-1].cover) {
      sl[-1].e = e;
    }
    else {
      ASSERT(sl != spane);
      sl->b = b;
      sl->e = e;
      sl->cover = cover;
      ++sl;
    }
  }
  return sl;
}
int spanv_free(spanv_t* s)
{
  FREE(s->s);
  memset(s, 0, sizeof(*s));
  return 0;
}
int spanv_resets(spanv_t* s, int n)
{
  for (; n--;) {
    memset(s->pos, 0, sizeof(int)*s->n);
    s->n = s->l = 0;
    ++s;
  }
  return 0;
}
int spanv_frees(spanv_t* s, int n)
{
  for (; n--;) {
    spanv_free(s++);
  }
  return 0;
}
int spanv_setsize(spanv_t* s, IRECT rc, int max_l)
{
  int n = RCH(&rc);
  MYREALLOC(s->pos, n + 1);
  MEMSET(s->pos, 0, n + 1);
  MYREALLOC(s->s, max_l);
  s->l = 0;
  s->max_l = max_l;
  s->n = n;
  s->rc = rc;
  return 0;
}
int bf_spanv_setsize(buf_t* bf, spanv_t* s, IRECT rc, int max_l)
{
  int n = RCH(&rc);
  BF_MALLOC(s->pos, n + 1);
  MEMSET(s->pos, 0, n + 1);
  BF_MALLOC(s->s, max_l);
  s->l = 0;
  s->max_l = max_l;
  s->n = n;
  s->rc = rc;
  return s->max_l;
}
void bf_spanv_free(buf_t* bf, spanv_t* s)
{
  BF_MALLOC(s->s, s->max_l);
  BF_MALLOC(s->pos, s->n + 1);
}
int draw2d_poly_brush(img_t* im, const IRECT* pclip, const DPOINT* pt0, const int* lens, int n, brush_t* br, bool flip_y)
{
  int w = im->w, h = im->h;
  scancell s[1] = {0};
  cell_aa* cells;
  int y, cellslen, maxcells = 0;
  IRECT clip = iRectClip(pclip, 0, 0, w, h), rect = {0};
  int is_irc = is_int_rect(pt0, lens, n, &rect);
  buf_t bf[1] = {0};
  IRECT rc = {0};
  uchar* buf = NULL;
  uchar* covers = NULL;
  COLOR* ptr_x = NULL;
  ASSERT(im->c == 4);
  if (is_irc) {
    rect = iRectInter(clip, rect);
    multialloc(&buf, &covers, RCW(&rect), NULL);
    memset(covers, 255, RCW(&rect));
    for (y = rect.t; y < rect.b; ++y) {
      ptr_x = (COLOR*)(im->tt.data + (flip_y ? (h - y - 1) : y) * im->s + rect.l * 4);
      br->fillline(br, ptr_x, rect.l, rect.r, y, covers);
    }
    FREE(buf);
    return 0;
  }
  maxcells = scancell_get_maxsize(pt0, lens, n, &clip, &rc);
  MYREALLOC(s->cells, maxcells);
  s->maxcells = maxcells;
  s->celllen = 0;
  scancell_set_d(s, pt0, lens, n, &clip);
  if (NULL == s->cells) {
    return 0;
  }
  cells = s->cells, cellslen = s->celllen;
  //max_y = BOUND(cells[cellslen - 1].y, rc.t, rc.b - 1);
  while (cells->y < rc.t) {
    ++cells;
  }
  multialloc(&buf, &covers, RCW(&rc), NULL);
  for (y = rc.t; y < rc.b; ++y) {
    int cover = 0;
    COLOR* ptr_x = (COLOR*)(im->tt.data + (flip_y ? (h - y - 1) : y) * im->s);
    while (y == cells->y) {
      const cell_aa* cur_cell = cells;
      int x = cur_cell->x, area = cur_cell->area;
      unsigned char alpha = 0;
      int b = x, len = 0;
      cover += cur_cell->cover;
      //accumulate all cells with the same X
      while (y == cells->y) {
        cur_cell = ++cells;
        if (cur_cell->x != x || cur_cell->y != y) {
          break;
        }
        area += cur_cell->area;
        cover += cur_cell->cover;
      }
      if (area) {
        area = (cover << (poly_subpixel_shift + 1)) - area;
        CALCULATE_ALPHA(alpha, area);
        if (alpha && x >= clip.l && x < clip.r) {
          covers[len++] = alpha;
          b = x;
        }
        x++;
      }
      if (x < cur_cell->x) {
        area = cover << (poly_subpixel_shift + 1);
        CALCULATE_ALPHA(alpha, area);
        if (alpha) {
          int x_end = MIN(cur_cell->x, w);
          x = MAX(x, 0);
          memset(covers + len, alpha, x_end - x);
          len += x_end - x;
        }
      }
      br->fillline(br, ptr_x + b, b, b + len, y, covers);
    }
  }
  FREE(s->cells);
  FREE(buf);
  return 0;
}
int spanv_set_poly(spanv_t* sp, img_t* im, const IRECT* pclip, const DPOINT* pt0, const int* lens, int n)
{
  int w = im->w, h = im->h;
  scancell s[1] = {0};
  cell_aa* cells;
  int y, cellslen, maxcells = 0;
  IRECT clip = iRectClip(pclip, 0, 0, w, h), rect = {0};
  int is_irc = is_int_rect(pt0, lens, n, &rect);
  buf_t bf[1] = {0};
  IRECT rc = {0};
  span_t* span, *spanb, *spane;
  if (is_irc) {
    rect = iRectInter(clip, rect);
    spanv_setsize(sp, rect, RCH(&rect));
    for (y = 0; y < sp->n; sp->pos[++y] = sp->l) {
      sp->l = span_push(sp->s + sp->pos[y], sp->s + sp->pos[y], sp->s + sp->max_l, rect.l, rect.r, 255) - sp->s;
    }
    return 0;
  }
  maxcells = scancell_get_maxsize(pt0, lens, n, &clip, &rc);
  spanv_setsize(sp, rc, maxcells * 2);
  sp->l = 0;
  MYREALLOC(s->cells, maxcells);
  s->maxcells = maxcells;
  s->celllen = 0;
  scancell_set_d(s, pt0, lens, n, &clip);
  if (NULL == s->cells) {
    return 0;
  }
  cells = s->cells, cellslen = s->celllen;
  //max_y = BOUND(cells[cellslen - 1].y, rc.t, rc.b - 1);
  while (cells->y < rc.t) {
    ++cells;
  }
  span = sp->s;
  spane = sp->s + sp->max_l;
  for (y = rc.t; y < rc.b; sp->pos[(++y) - rc.t] = sp->l) {
    int cover = 0;
    spanb = span;
    while (y == cells->y) {
      const cell_aa* cur_cell = cells;
      int x = cur_cell->x, x_end, area = cur_cell->area;
      unsigned char alpha = 0;
      cover += cur_cell->cover;
      //accumulate all cells with the same X
      while (y == cells->y) {
        cur_cell = ++cells;
        if (cur_cell->x != x || cur_cell->y != y) {
          break;
        }
        area += cur_cell->area;
        cover += cur_cell->cover;
      }
      if (area) {
        area = (cover << (poly_subpixel_shift + 1)) - area;
        CALCULATE_ALPHA(alpha, area);
        if (alpha && x >= clip.l && x < clip.r) {
          span = span_push(span, spanb, spane, x, x + 1, alpha);
        }
        x++;
      }
      if (x < cur_cell->x) {
        area = cover << (poly_subpixel_shift + 1);
        CALCULATE_ALPHA(alpha, area);
        if (alpha) {
          x = MAX(x, 0);
          x_end = MIN(cur_cell->x, w);
          span = span_push(span, spanb, spane, x, x_end, alpha);
        }
      }
    }
    sp->l = span - sp->s;
  }
  ASSERT(sp->pos[0] == 0);
  FREE(s->cells);
  return 0;
}
int spanv_set_rect(spanv_t* sp, img_t* im, const IRECT* pclip, double x, double y, double w, double h)
{
  DPOINT pt[4];
  int n = vcgen_rect(pt, 0, 4, x, y, x + w, y + h);
  return spanv_set_poly(sp, im, pclip, pt, &n, 1);
}
int spanv_set_rectR(spanv_t* sp, img_t* im, const IRECT* pclip, IRECT rc)
{
  return spanv_set_rect(sp, im, pclip, rc.l, rc.t, RCW(&rc), RCH(&rc));
}
#define RCIS_VALID(rc) ((rc).l < (rc).r && (rc).t < (rc).b)
span_t* span_neg(const span_t* span1, const span_t* span1e, int xb0, int xe0, span_t* sl, span_t* spane)
{
  int xb = xb0;
  span_t* spanb = sl;
  for (; span1 != span1e; ++span1) {
    if (xb0 <= span1->e) {
      int xe = MAX(xb0, span1->b);
      if (xb < xe) {
        sl = span_push(sl, spanb, spane, xb, xe, cover_mask);
      }
      if (xe < span1->e && span1->cover < cover_mask) {
        sl = span_push(sl, spanb, spane, xe, span1->e, cover_mask - span1->cover);
      }
      xb = span1->e;
    }
  }
  if (xb < xe0) {
    sl = span_push(sl, spanb, spane, xb, xe0, cover_mask);
  }
  return sl;
}
int spanv_neg(spanv_t* sp, const spanv_t* sp1, IRECT rc)
{
  int y;
  spanv_setsize(sp, rc, (sp1->l + sp->n) * 2);
  {
    const int* pos1 = sp1->pos - sp1->rc.t + rc.t;
    for (y = 0; y < sp->n; sp->pos[++y] = sp->l) {
      int yy = y + rc.t;
      if (yy < sp1->rc.t || yy >= sp1->rc.b) {
        span_push(sp->s + sp->pos[y], sp->s + sp->pos[y], sp->s + sp->max_l, rc.l, rc.r, cover_mask);
        sp->l++;
      }
      else {
        sp->l = span_neg(sp1->s + pos1[y], sp1->s + pos1[y + 1], rc.l, rc.r, sp->s + sp->pos[y], sp->s + sp->max_l) - sp->s;
      }
    }
  }
  return 0;
}
span_t* span_copy(const span_t* span1, const span_t* span1e, span_t* sl, span_t* spane)
{
  int l = span1e-span1;
  ASSERT(l <= (spane - sl));
  MEMCPY(sl, span1, l);
  return sl + l;
}
span_t* span_and(const span_t* span1, const span_t* span1e, const span_t* span2, const span_t* span2e, span_t* sl, span_t* spane)
{
  int cover;
  span_t* spanb = sl;
  while (span1 != span1e && span2 != span2e) {
    int xb = MAX(span1->b, span2->b);
    int xe = MIN(span1->e, span2->e);
    if (xb < xe) {
      //cover = (span1->cover * span2->cover) / cover_mask;
      cover = MIN(span1->cover, span2->cover);
      sl = span_push(sl, spanb, spane, xb, xe, cover);
    }
    if (span1->e < span2->e) {
      ++span1;
    }
    else if (span2->e < span1->e) {
      ++span2;
    }
    else {
      ++span1;
      ++span2;
    }
  }
  return sl;
}
int spanv_and(spanv_t* sp, const spanv_t* sp1, const spanv_t* sp2)
{
  int y;
  IRECT rc;
  if (sp1->n <= 0) {
    return 0;
  }
  if (sp2->n <= 0) {
    return 0;
  }
  rc = iRectInter(sp1->rc, sp2->rc);
  if (!RCIS_VALID(rc)) {
    return 0;
  }
  {
    const int* pos1 = sp1->pos - sp1->rc.t + rc.t;
    const int* pos2 = sp2->pos - sp2->rc.t + rc.t;
    spanv_setsize(sp, rc, (sp1->l + sp2->l + sp->n * 2));
    for (y = 0; y < sp->n; sp->pos[++y] = sp->l) {
      sp->l = span_and(sp1->s + pos1[y], sp1->s + pos1[y + 1], sp2->s + pos2[y], sp2->s + pos2[y + 1], sp->s + sp->pos[y], sp->s + sp->max_l) - sp->s;
    }
  }
  return 1;
}
int spanv_copy(spanv_t* sp, const spanv_t* sp1)
{
  spanv_setsize(sp, sp1->rc, sp1->l);
  MEMCPY(sp->pos, sp1->pos, sp1->n + 1);
  MEMCPY(sp->s, sp1->s, sp1->l);
  sp->l = sp1->l;
  return 0;
}
span_t* span_sub(const span_t* span1, const span_t* span1e, const span_t* span2, const span_t* span2e, span_t* sl, span_t* spane)
{
  int cover, xb, xe;
  span_t* spanb = sl;
  span_t sp1 = {0};
  while (span1 != span1e && span2 != span2e) {
    if (sp1.b == sp1.e) {
      sp1 = *span1++;
    }
    xb = MAX(sp1.b, span2->b);
    xe = MIN(sp1.e, span2->e);
    if (xb < xe) {
      int xb1 = MIN(sp1.b, span2->b);
      int xe1 = MAX(sp1.e, span2->e);
      if (sp1.b < xb) {
        sl = span_push(sl, spanb, spane, sp1.b, xb, sp1.cover);
      }
      if (1) {
        cover = MIN(sp1.cover, cover_mask - span2->cover);
        //sl = span_push(sl, spanb, spane, xb, xe, cover);
      }
      if (xe < xe1) {
        if (sp1.e > span2->e) {
          sp1.b = xe;
          ++span2;
        }
        else {
          sp1.b = sp1.e = 0;
        }
      }
      else {
        sp1.b = sp1.e = 0;
        ++span2;
      }
    }
    else {
      if (sp1.e < span2->e) {
        sl = span_push(sl, spanb, spane, sp1.b, sp1.e, sp1.cover);
        sp1.b = sp1.e = 0;
      }
      else {
        ++span2;
      }
    }
  }
  if (sp1.b < sp1.e) {
    sl = span_push(sl, spanb, spane, sp1.b, sp1.e, sp1.cover);
  }
  if (span1 < span1e) {
    sl = span_copy(span1, span1e, sl, spane);
  }
  return sl;
}
int spanv_sub(spanv_t* sp, const spanv_t* sp1, const spanv_t* sp2)
{
  int y, h1;
  IRECT rc = sp1->rc;
  const int* pos1 = sp1->pos - sp1->rc.t + rc.t;
  const int* pos2 = sp2->pos - sp2->rc.t + rc.t;
  h1 = MIN(rc.b, sp2->rc.b) - rc.t;
  spanv_setsize(sp, rc, sp1->l * 2);
  for (y = 0; y < sp->n; sp->pos[++y] = sp->l) {
    int yy = y + rc.t;
    if (yy < sp2->rc.t || yy >= sp2->rc.b) {
      sp->l = span_copy(sp1->s + pos1[y], sp1->s + pos1[y + 1], sp->s + sp->pos[y], sp->s + sp->max_l) - sp->s;
    }
    else {
#if 0
      span_t span[256] = {0};
      span_t* spane = span_neg(sp2->s + pos2[y], sp2->s + pos2[y + 1], rc.l, rc.r, span, span + countof(span));
      sp->l = span_and(sp1->s + pos1[y], sp1->s + pos1[y + 1], span, spane, sp->s + sp->pos[y], sp->s + sp->max_l) - sp->s;
#else
      sp->l = span_sub(sp1->s + pos1[y], sp1->s + pos1[y + 1], sp2->s + pos2[y], sp2->s + pos2[y + 1], sp->s + sp->pos[y], sp->s + sp->max_l) - sp->s;
#endif
    }
  }
  return 0;
}
span_t* span_or(const span_t* span1, const span_t* span1e, const span_t* span2, const span_t* span2e, span_t* sl, span_t* spane, int isxor)
{
  int xb, xe, cover;
  span_t sp1 = {0}, sp2 = {0};
  span_t* spanb = sl;
  while (1) {
    if (sp1.b == sp1.e) {
      if (span1 != span1e) {
        sp1 = *span1++;
      }
      else {
        break;
      }
    }
    if (sp2.b == sp2.e) {
      if (span2 != span2e) {
        sp2 = *span2++;
      }
      else {
        break;
      }
    }
    xb = MAX(sp1.b, sp2.b);
    xe = MIN(sp1.e, sp2.e);
    if (xb < xe) {
      int xb1 = MIN(sp1.b, sp2.b);
      int xe1 = MAX(sp1.e, sp2.e);
      if (xb1 < xb) {
        cover = sp1.b < sp2.b ? sp1.cover : sp2.cover;
        sl = span_push(sl, spanb, spane, xb1, xb, cover);
      }
      //cover = cover_mask - ((cover_mask-sp1.cover) * (cover_mask-sp2.cover)) / cover_mask;
      if (isxor) {
        cover = cover_mask - MAX(sp1.cover, sp2.cover);
      }
      else {
        cover = MAX(sp1.cover, sp2.cover);
      }
      ASSERT(cover >= 0 && cover <= cover_mask);
      sl = span_push(sl, spanb, spane, xb, xe, cover);
      if (xe < xe1) {
        if (sp1.e > sp2.e) {
          sp1.b = xe;
          sp2.b = sp2.e = 0;
        }
        else {
          sp2.b = xe;
          sp1.b = sp1.e = 0;
        }
      }
      else {
        sp1.b = sp1.e = 0;
        sp2.b = sp2.e = 0;
      }
    }
    else {
      if (sp1.b < sp2.b) {
        sl = span_push(sl, spanb, spane, sp1.b, sp1.e, sp1.cover);
        sp1.b = sp1.e = 0;
      }
      else {
        sl = span_push(sl, spanb, spane, sp2.b, sp2.e, sp2.cover);
        sp2.b = sp2.e = 0;
      }
    }
  }
  if (sp1.b < sp1.e) {
    sl = span_push(sl, spanb, spane, sp1.b, sp1.e, sp1.cover);
  }
  if (sp2.b < sp2.e) {
    sl = span_push(sl, spanb, spane, sp2.b, sp2.e, sp2.cover);
  }
  for (; span1 != span1e; ++span1) {
    sl = span_push(sl, spanb, spane, span1->b, span1->e, span1->cover);
  }
  for (; span2 != span2e; ++span2) {
    sl = span_push(sl, spanb, spane, span2->b, span2->e, span2->cover);
  }
  ASSERT(span1 == span1e);
  ASSERT(span2 == span2e);
  return sl;
}
int spanv_or(spanv_t* sp, const spanv_t* sp1, const spanv_t* sp2, int isxor)
{
  int y;
  IRECT rc = {0};
  if (sp2->n <= 0) {
    return spanv_copy(sp, sp1);
  }
  if (sp1->n <= 0) {
    return spanv_copy(sp, sp2);
  }
  rc = iRectUnion(sp1->rc, sp2->rc);
  if (!RCIS_VALID(rc)) {
    return 0;
  }
  spanv_setsize(sp, rc, (sp1->l + sp2->l + sp->n) * 2);
  {
    const int* pos1 = sp1->pos - sp1->rc.t + rc.t;
    const int* pos2 = sp2->pos - sp2->rc.t + rc.t;
    for (y = 0; y < sp->n; sp->pos[++y] = sp->l) {
      int yy = y + rc.t;
      bool in1 = yy >= sp1->rc.t && yy < sp1->rc.b;
      bool in2 = yy >= sp2->rc.t && yy < sp2->rc.b;
      if (in1 && in2) {
        sp->l = span_or(sp1->s + pos1[y], sp1->s + pos1[y + 1], sp2->s + pos2[y], sp2->s + pos2[y + 1], sp->s + sp->pos[y], sp->s + sp->max_l, isxor) - sp->s;
      }
      else if (in1) {
        sp->l = span_copy(sp1->s + pos1[y], sp1->s + pos1[y + 1], sp->s + sp->pos[y], sp->s + sp->max_l) - sp->s;
      }
      else if (in2) {
        sp->l = span_copy(sp2->s + pos2[y], sp2->s + pos2[y + 1], sp->s + sp->pos[y], sp->s + sp->max_l) - sp->s;
      }
    }
  }
  return 1;
}
int spanv_boolopt(spanv_t* sp, const spanv_t* sp1, const spanv_t* sp2, boolopt opt) {
  switch (opt) {
  case RGN_AND:
    spanv_and(sp, sp1, sp2);
    break;
  case RGN_OR:
    spanv_or(sp, sp1, sp2, 0);
    break;
  case RGN_XOR:
    spanv_or(sp, sp1, sp2, 1);
    break;
  case RGN_DIFF:
    spanv_sub(sp, sp1, sp2);
    break;
  case RGN_COPY:
    spanv_copy(sp, sp1);
    break;
  default:
    break;
  }
  return 0;
}
int point_in_spanv(const spanv_t* sl, int x, int y)
{
  int hot = iPtInRect(&sl->rc, x, y);
  if (hot) {
    const int* pos = sl->pos + y - sl->rc.t + y;
    const span_t* span = sl->s + pos[0];
    const span_t* spane = sl->s + pos[1];
    for (; span != spane; ++span) {
      if (span->b <= x && x < span->e) {
        return 1;
      }
    }
  }
  return 0;
}