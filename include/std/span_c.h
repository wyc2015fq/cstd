
#ifndef _STD_SPAN_C_H_
#define _STD_SPAN_C_H_


#define BOOLOPT_DEF_DEF(BOOLOPT_DEF) \
  BOOLOPT_DEF(NUL) \
  BOOLOPT_DEF(AND) \
  BOOLOPT_DEF(OR) \
  BOOLOPT_DEF(XOR) \
  BOOLOPT_DEF(DIFF) \
BOOLOPT_DEF(COPY)

typedef enum {
#define BOOLOPT_DEF(name) Rgn_##name ,
  BOOLOPT_DEF_DEF(BOOLOPT_DEF)
#undef BOOLOPT_DEF
} boolopt;

typedef struct {
  int b, e, y, cover;
} span_t;

typedef struct {
  span_t* s;
  int l;
  int max_l;
  IRECT rc;//bound box
} spanv_t;
span_t* span_push(span_t* sl, span_t* spanb, span_t* spane, int b, int e, int y, int cover)
{
  if (cover > 0) {
    if (sl > spanb) {
      ASSERT(sl[-1].e <= b);
      ASSERT(sl[-1].y == y);
    }
    if (sl > spanb && b == sl[-1].e && cover == sl[-1].cover) {
      sl[-1].e = e;
    }
    else {
      ASSERT(sl < spane);
      sl->b = b;
      sl->e = e;
      sl->y = y;
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
    s->l = 0;
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
  MYREALLOC(s->s, max_l);
  s->l = 0;
  s->max_l = max_l;
  s->rc = rc;
  return 0;
}
int bf_spanv_setsize(buf_t* bf, spanv_t* s, IRECT rc, int max_l)
{
  BF_MALLOC(s->s, max_l);
  s->l = 0;
  s->max_l = max_l;
  s->rc = rc;
  return s->max_l;
}
void bf_spanv_free(buf_t* bf, spanv_t* s)
{
  BF_MALLOC(s->s, s->max_l);
}
#define RCIS_VALID(rc) ((rc).l < (rc).r && (rc).t < (rc).b)
const span_t* span_end(int y, const span_t* sl, const span_t* se) {
  for (; sl<se && sl->y <= y; ++sl) {}
  return sl;
}
span_t* span_copy(span_t** psl, span_t* spane, int y, const span_t** pspan1, const span_t* span1e)
{
  int l;
  span_t* sl = *psl;
  const span_t* span1 = *pspan1;
  span1 = span_end(y - 1, span1, span1e);
  span1e = span_end(y, span1, span1e);
  l = span1e - span1;
  ASSERT(l <= (spane - sl));
  MEMCPY(sl, span1, l);
  *psl = sl + l;
  *pspan1 = span1e;
  return sl + l;
}
span_t* span_neg(span_t** psl, span_t* spane, int y, const span_t** pspan1, const span_t* span1e, int xb0, int xe0)
{
  int xb = xb0;
  span_t* sl = *psl;
  span_t* spanb = sl;
  const span_t* span1 = *pspan1;
  span1e = span_end(y, span1, span1e);
  for (; span1 != span1e; ++span1) {
    if (xb0 <= span1->e) {
      int xe = MAX(xb0, span1->b);
      if (xb < xe) {
        sl = span_push(sl, spanb, spane, xb, xe, y, cover_mask);
      }
      if (xe < span1->e && span1->cover < cover_mask) {
        sl = span_push(sl, spanb, spane, xe, span1->e, y, cover_mask - span1->cover);
      }
      xb = span1->e;
    }
  }
  if (xb < xe0) {
    sl = span_push(sl, spanb, spane, xb, xe0, y, cover_mask);
  }
  *psl = sl;
  *pspan1 = span1e;
  return sl;
}
int spanv_neg(spanv_t* sp, const spanv_t* sp1, IRECT rc)
{
  int y;
  span_t* span;
  const span_t *span1 = sp1->s;
  spanv_setsize(sp, rc, (sp1->l) * 2 + RCH(&rc));
  span = sp->s;
  for (y = rc.t; y < rc.b; ++y) {
    if (y < sp1->rc.t || y >= sp1->rc.b) {
      span = span_push(span, span, sp1->s + sp1->l, rc.l, rc.r, y, cover_mask);
    }
    else {
      span_neg(&span, sp->s + sp->max_l, y, &span1, sp1->s + sp1->l, rc.l, rc.r);
    }
  }
  sp->l = span - sp->s;
  return 0;
}
span_t* span_and(span_t** psl, span_t* spane, int y, const span_t** pspan1, const span_t* span1e, const span_t** pspan2, const span_t* span2e)
{
  int cover;
  span_t* sl = *psl;
  span_t* spanb = sl;
  const span_t* span1 = *pspan1;
  const span_t* span2 = *pspan2;
  span1e = span_end(y, span1, span1e);
  span2e = span_end(y, span2, span2e);
  while (span1 != span1e && span2 != span2e) {
    int xb = MAX(span1->b, span2->b);
    int xe = MIN(span1->e, span2->e);
    if (xb < xe) {
      //cover = (span1->cover * span2->cover) / cover_mask;
      cover = MIN(span1->cover, span2->cover);
      sl = span_push(sl, spanb, spane, xb, xe, y, cover);
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
  *psl = sl;
  *pspan1 = span1e;
  *pspan2 = span2e;
  return sl;
}
int spanv_and(spanv_t* sp, const spanv_t* sp1, const spanv_t* sp2)
{
  int y;
  IRECT rc;
  span_t* span;
  const span_t *span1 = sp1->s;
  const span_t *span2 = sp2->s;
  if (sp1->l <= 0 || sp2->l <= 0) {
    sp->l = 0;
    return 0;
  }
  rc = iRectInter(sp1->rc, sp2->rc);
  if (!RCIS_VALID(rc)) {
    return 0;
  }
  spanv_setsize(sp, rc, (sp1->l + sp2->l + RCH(&rc) * 2));
  span = sp->s;
  for (y = rc.t; y < rc.b; ++y) {
    span_and(&span, sp->s + sp->max_l, y, &span1, sp1->s + sp1->l, &span2, sp2->s + sp2->l);
  }
  sp->l = span - sp->s;
  return 1;
}
int spanv_copy(spanv_t* sp, const spanv_t* sp1)
{
  spanv_setsize(sp, sp1->rc, sp1->l);
  MEMCPY(sp->s, sp1->s, sp1->l);
  sp->l = sp1->l;
  sp->rc = sp1->rc;
  return 0;
}
span_t* span_sub(span_t** psl, span_t* spane, int y, const span_t** pspan1, const span_t* span1e, const span_t** pspan2, const span_t* span2e)
{
  int xb, xe;
  span_t sp1 = { 0 };
  span_t* sl = *psl;
  span_t* spanb = sl;
  const span_t* span1 = *pspan1;
  const span_t* span2 = *pspan2;
  span1e = span_end(y, span1, span1e);
  span2e = span_end(y, span2, span2e);
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
        sl = span_push(sl, spanb, spane, sp1.b, xb, y, sp1.cover);
      }
      if (1) {
        //cover = MIN(sp1.cover, cover_mask - span2->cover);
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
        sl = span_push(sl, spanb, spane, sp1.b, sp1.e, y, sp1.cover);
        sp1.b = sp1.e = 0;
      }
      else {
        ++span2;
      }
    }
  }
  if (sp1.b < sp1.e) {
    sl = span_push(sl, spanb, spane, sp1.b, sp1.e, y, sp1.cover);
  }
  if (span1 < span1e) {
    span_copy(&sl, spane, y, &span1, span1e);
  }
  *psl = sl;
  *pspan1 = span1e;
  *pspan2 = span2e;
  return sl;
}
int spanv_sub(spanv_t* sp, const spanv_t* sp1, const spanv_t* sp2)
{
  int y;
  IRECT rc = sp1->rc;
  span_t* span;
  const span_t *span1 = sp1->s;
  const span_t *span2 = sp2->s;
  //h1 = MIN(rc.b, sp2->rc.b) - rc.t;
  spanv_setsize(sp, rc, sp1->l * 2);
  span = sp->s;
  for (y = rc.t; y < rc.b; ++y) {
    if (y < sp2->rc.t || y >= sp2->rc.b) {
      span_copy(&span, sp->s + sp->max_l, y, &span1, sp1->s + sp1->l);
    }
    else {
#if 0
      span_t span[256] = { 0 };
      span_t* spane = span_neg(sp2->s + pos2[y], sp2->s + pos2[y + 1], rc.l, rc.r, span, span + countof(span));
      sp->l = span_and(sp1->s + pos1[y], sp1->s + pos1[y + 1], span, spane, sp->s + sp->max_l, sp->s + sp->max_l) - sp->s;
#else
      span_sub(&span, sp->s + sp->max_l, y, &span1, sp1->s + sp1->l, &span2, sp2->s + sp2->l);
#endif
    }
  }
  sp->l = span - sp->s;
  return 0;
}
span_t* span_or(span_t** psl, span_t* spane, int y, const span_t** pspan1, const span_t* span1e, const span_t** pspan2, const span_t* span2e, int isxor)
{
  int xb, xe, cover;
  span_t sp1 = { 0 }, sp2 = { 0 };
  span_t* sl = *psl;
  span_t* spanb = sl;
  const span_t* span1 = *pspan1;
  const span_t* span2 = *pspan2;
  span1e = span_end(y, span1, span1e);
  span2e = span_end(y, span2, span2e);
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
        sl = span_push(sl, spanb, spane, xb1, xb, y, cover);
      }
      //cover = cover_mask - ((cover_mask-sp1.cover) * (cover_mask-sp2.cover)) / cover_mask;
      if (isxor) {
        cover = cover_mask - MAX(sp1.cover, sp2.cover);
      }
      else {
        cover = MAX(sp1.cover, sp2.cover);
      }
      ASSERT(cover >= 0 && cover <= cover_mask);
      sl = span_push(sl, spanb, spane, xb, xe, y, cover);
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
        sl = span_push(sl, spanb, spane, sp1.b, sp1.e, y, sp1.cover);
        sp1.b = sp1.e = 0;
      }
      else {
        sl = span_push(sl, spanb, spane, sp2.b, sp2.e, y, sp2.cover);
        sp2.b = sp2.e = 0;
      }
    }
  }
  if (sp1.b < sp1.e) {
    sl = span_push(sl, spanb, spane, sp1.b, sp1.e, y, sp1.cover);
  }
  if (sp2.b < sp2.e) {
    sl = span_push(sl, spanb, spane, sp2.b, sp2.e, y, sp2.cover);
  }
  for (; span1 != span1e; ++span1) {
    sl = span_push(sl, spanb, spane, span1->b, span1->e, y, span1->cover);
  }
  for (; span2 != span2e; ++span2) {
    sl = span_push(sl, spanb, spane, span2->b, span2->e, y, span2->cover);
  }
  ASSERT(span1 == span1e);
  ASSERT(span2 == span2e);
  *psl = sl;
  *pspan1 = span1e;
  *pspan2 = span2e;
  return sl;
}
int spanv_or(spanv_t* sp, const spanv_t* sp1, const spanv_t* sp2, int isxor)
{
  int y;
  IRECT rc = { 0 };
  span_t* span;
  const span_t *span1 = sp1->s;
  const span_t *span2 = sp2->s;
  if (sp2->l <= 0) {
    return spanv_copy(sp, sp1);
  }
  if (sp1->l <= 0) {
    return spanv_copy(sp, sp2);
  }
  rc = iRectUnion(sp1->rc, sp2->rc);
  if (!RCIS_VALID(rc)) {
    return 0;
  }
  spanv_setsize(sp, rc, (sp1->l + sp2->l + RCH(&rc)) * 2);
  span = sp->s;
  for (y = rc.t; y < rc.b; ++y) {
    bool in1 = y >= sp1->rc.t && y < sp1->rc.b;
    bool in2 = y >= sp2->rc.t && y < sp2->rc.b;
    if (in1 && in2) {
      span_or(&span, sp->s + sp->max_l, y, &span1, sp1->s + sp1->l, &span2, sp2->s + sp2->l, isxor);
    }
    else if (in1) {
      span_copy(&span, sp->s + sp->max_l, y, &span1, sp1->s + sp1->l);
    }
    else if (in2) {
      span_copy(&span, sp->s + sp->max_l, y, &span2, sp2->s + sp2->l);
    }
  }
  sp->l = span - sp->s;
  return 1;
}
int spanv_boolopt(spanv_t* sp, const spanv_t* sp1, const spanv_t* sp2, boolopt opt) {
  switch (opt) {
  case Rgn_AND:
    spanv_and(sp, sp1, sp2);
    break;
  case Rgn_OR:
    spanv_or(sp, sp1, sp2, 0);
    break;
  case Rgn_XOR:
    spanv_or(sp, sp1, sp2, 1);
    break;
  case Rgn_DIFF:
    spanv_sub(sp, sp1, sp2);
    break;
  case Rgn_COPY:
    spanv_copy(sp, sp1);
    break;
  default:
    break;
  }
  return 0;
}
int point_in_spanv(const spanv_t* sp, int x, int y)
{
  int hot = iPtInRect(&sp->rc, x, y);
  if (hot) {
    const span_t* span = sp->s;
    const span_t* spane = sp->s + sp->max_l;
    for (; span != spane && span->y <= y; ++span) {
      if (span->y == y && span->b <= x && x < span->e) {
        return 1;
      }
    }
  }
  return 0;
}



#endif // _STD_SPAN_C_H_
