
void map_linear_1d(int n, uchar* pix, int cn, int ax, int b, int shift, int mask, const COLOR* clrs)
{
  int i;

  switch (cn) {
  case 2:
    for (i = 0; i < n; ++i) {
      COLOR c = clrs[(((i * ax) >> shift) + b)&mask];
      *((ushort*)pix) = (ushort)c;
      pix += 2;
    }

    break;

  case 3:
    for (i = 0; i < n; ++i) {
      COLOR c = clrs[(((i * ax) >> shift) + b)&mask];
      pix[0] = GetRV(c);
      pix[1] = GetGV(c);
      pix[2] = GetBV(c);
      pix += 3;
    }

    break;

  case 4:
    for (i = 0; i < n; ++i) {
      COLOR c = clrs[(((i * ax) >> shift) + b)&mask];
      *((uint32*)pix) = (uint32)c;
      pix += 4;
    }

    break;
  }
}
void immap_linear(img_t* im, int ax, int ay, int shift, int n_clrs, const COLOR* clrs)
{
  int j;
  int mask = n_clrs - 1;

  for (j = 0; j < im->h; j++) {
    uchar* p = im->tt.data + j * im->s;
    int b = (j * ay) >> shift;
    map_linear_1d(im->w, p, im->c, ax, b, shift, mask, clrs);
  }
}
void immap_linear_d(img_t* im, double dx, double dy, int n_clrs, const COLOR* clrs)
{
  int ax, ay, shift = 8;
  ax = (int)(dx * (1 << shift) + 0.5);
  ay = (int)(dy * (1 << shift) + 0.5);
  immap_linear(im, ax, ay, shift, n_clrs, clrs);
}

CV_INLINE int iRectClipLine(const IRECT* pclip, int* px1, int* py1, int* px2, int* py2)
{
  int result = 0;

  int x1, y1, x2, y2;
  int c1, c2;
  int l = pclip->l, t = pclip->t;
  int r = pclip->r - 1, b = pclip->b - 1;

  if (r < 0 || b < 0) {
    return (1);
  }

  x1 = *px1;
  y1 = *py1;
  x2 = *px2;
  y2 = *py2;
  c1 = (x1 < l) + (x1 > r) * 2 + (y1 < t) * 4 + (y1 > b) * 8;
  c2 = (x2 < l) + (x2 > r) * 2 + (y2 < t) * 4 + (y2 > b) * 8;

  if ((c1 & c2) == 0 && (c1 | c2) != 0) {
    int a;

    if (c1 & 12) {
      a = c1 < 8 ? 0 : b;
      x1 += (int)(((int64)(a - y1)) * (x2 - x1) / (y2 - y1));
      y1 = a;
      c1 = (x1 < 0) + (x1 > r) * 2;
    }

    if (c2 & 12) {
      a = c2 < 8 ? 0 : b;
      x2 += (int)(((int64)(a - y2)) * (x2 - x1) / (y2 - y1));
      y2 = a;
      c2 = (x2 < 0) + (x2 > r) * 2;
    }

    if ((c1 & c2) == 0 && (c1 | c2) != 0) {
      if (c1) {
        a = c1 == 1 ? 0 : r;
        y1 += (int)(((int64)(a - x1)) * (y2 - y1) / (x2 - x1));
        x1 = a;
        c1 = 0;
      }

      if (c2) {
        a = c2 == 1 ? 0 : r;
        y2 += (int)(((int64)(a - x2)) * (y2 - y1) / (x2 - x1));
        x2 = a;
        c2 = 0;
      }
    }

    assert((c1 & c2) != 0 || (x1 | y1 | x2 | y2) >= 0);

    *px1 = x1;
    *py1 = y1;
    *px2 = x2;
    *py2 = y2;
  }

  result = (c1 | c2) == 0;

  if (0 == result) {
    *px1 = 0;
    *py1 = 0;
    *px2 = 0;
    *py2 = 0;
  }

  return result;
}

CV_INLINE int mul_div(int a, int b, int c)
{
  return (int)(((int64)a) * b / c);
}
CV_INLINE int line_clip_y(IPOINT* pt, const IRECT* clip_box, int x1, int y1, int x2, int y2, unsigned f1, unsigned f2)
{
  IPOINT* pt0 = pt;
  f1 &= 10;
  f2 &= 10;

  if ((f1 | f2) == 0) {
    // Fully visible
    *pt0++ = iPOINT(x1, y1);
    *pt0++ = iPOINT(x2, y2);
    return 2;
  }
  else {
    int tx1, ty1, tx2, ty2;

    if (f1 == f2) {
      // Invisible by Y
      return 0;
    }

    tx1 = x1, ty1 = y1, tx2 = x2, ty2 = y2;

    if (f1 & 8) { // y1 < clip.y1
      tx1 = x1 + mul_div(clip_box->t - y1, x2 - x1, y2 - y1);
      ty1 = clip_box->t;
    }

    if (f1 & 2) { // y1 > clip.y2
      tx1 = x1 + mul_div(clip_box->b - y1, x2 - x1, y2 - y1);
      ty1 = clip_box->b;
    }

    if (f2 & 8) { // y2 < clip.y1
      tx2 = x1 + mul_div(clip_box->t - y1, x2 - x1, y2 - y1);
      ty2 = clip_box->t;
    }

    if (f2 & 2) { // y2 > clip.y2
      tx2 = x1 + mul_div(clip_box->b - y1, x2 - x1, y2 - y1);
      ty2 = clip_box->b;
    }

    *pt0++ = iPOINT(tx1, ty1);
    *pt0++ = iPOINT(tx2, ty2);
    return 2;
  }
}
CV_INLINE unsigned clipping_flags_x(int x, const IRECT* clip_box)
{
  return (x > clip_box->r) | ((x < clip_box->l) << 2);
}
CV_INLINE unsigned clipping_flags_y(int y, const IRECT* clip_box)
{
  return ((y > clip_box->b) << 1) | ((y < clip_box->t) << 3);
}
CV_INLINE unsigned clipping_flags(int x, int y, const IRECT* clip_box)
{
  return (x > clip_box->r) | ((y > clip_box->b) << 1) | ((x < clip_box->l) << 2) | ((y < clip_box->t) << 3);
}
CV_INLINE int clip_line_to(IPOINT* pt, const IRECT* clip_box, int m_x1, int m_y1, int x2, int y2)
{
  unsigned int f1, f2, f3, f4;
  int x1, y1, y3, y4;
  unsigned int m_f1 = clipping_flags(m_x1, m_y1, clip_box);
  IPOINT* pt0 = pt;
  f2 = clipping_flags(x2, y2, clip_box);

  if ((m_f1 & 10) == (f2 & 10) && (m_f1 & 10) != 0) {
    // Invisible by Y
    m_x1 = x2;
    m_y1 = y2;
    m_f1 = f2;
    return 0;
  }

  x1 = m_x1, y1 = m_y1;
  f1 = m_f1;

  switch (((f1 & 5) << 1) | (f2 & 5)) {
  case 0: // Visible by X
    pt += line_clip_y(pt, clip_box, x1, y1, x2, y2, f1, f2);
    break;

  case 1: // x2 > clip.x2
    y3 = y1 + mul_div(clip_box->r - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    pt += line_clip_y(pt, clip_box, x1, y1, clip_box->r, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, clip_box->r, y3, clip_box->r, y2, f3, f2);
    break;

  case 2: // x1 > clip.x2
    y3 = y1 + mul_div(clip_box->r - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    pt += line_clip_y(pt, clip_box, clip_box->r, y1, clip_box->r, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, clip_box->r, y3, x2, y2, f3, f2);
    break;

  case 3: // x1 > clip.x2 && x2 > clip.x2
    pt += line_clip_y(pt, clip_box, clip_box->r, y1, clip_box->r, y2, f1, f2);
    break;

  case 4: // x2 < clip.x1
    y3 = y1 + mul_div(clip_box->l - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    pt += line_clip_y(pt, clip_box, x1, y1, clip_box->l, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, clip_box->l, y3, clip_box->l, y2, f3, f2);
    break;

  case 6: // x1 > clip.x2 && x2 < clip.x1
    y3 = y1 + mul_div(clip_box->r - x1, y2 - y1, x2 - x1);
    y4 = y1 + mul_div(clip_box->l - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    f4 = clipping_flags_y(y4, clip_box);
    pt += line_clip_y(pt, clip_box, clip_box->r, y1, clip_box->r, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, clip_box->r, y3, clip_box->l, y4, f3, f4);
    pt += line_clip_y(pt, clip_box, clip_box->l, y4, clip_box->l, y2, f4, f2);
    break;

  case 8: // x1 < clip.x1
    y3 = y1 + mul_div(clip_box->l - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    pt += line_clip_y(pt, clip_box, clip_box->l, y1, clip_box->l, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, clip_box->l, y3, x2, y2, f3, f2);
    break;

  case 9: // x1 < clip.x1 && x2 > clip.x2
    y3 = y1 + mul_div(clip_box->l - x1, y2 - y1, x2 - x1);
    y4 = y1 + mul_div(clip_box->r - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    f4 = clipping_flags_y(y4, clip_box);
    pt += line_clip_y(pt, clip_box, clip_box->l, y1, clip_box->l, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, clip_box->l, y3, clip_box->r, y4, f3, f4);
    pt += line_clip_y(pt, clip_box, clip_box->r, y4, clip_box->r, y2, f4, f2);
    break;

  case 12: // x1 < clip.x1 && x2 < clip.x1
    pt += line_clip_y(pt, clip_box, clip_box->l, y1, clip_box->l, y2, f1, f2);
    break;
  }

  m_f1 = f2;
  m_x1 = x2;
  m_y1 = y2;
  return pt - pt0;
}
typedef ushort zbuftype_t;
typedef struct ZVertex2 {
  int x, y, z; // fix16
  ushort s, t; // tex for fix16
  COLOR clr;
} ZVertex2;
typedef struct ZRender {
  int h, w;
  int cn;
  int zbuf_step;
  int data_step;
  uchar* data;
  zbuftype_t* zbuf;
  IRECT* pclip;
} ZRender;

void drawVertexPrimitiveList(const S2DVertex* vertices, u32 vertexCount,
    const void* indexList, u32 primitiveCount,
    E_PRIMITIVE_TYPE pType, E_TYPE iType)
{
  return;
}
void Z_line(ZRender* zb, ZVertex2* p0, ZVertex2* p1)
{
  int i, steps, increx, increy, x, y, step = zb->data_step;
  int x1 = p0->x, y1 = p0->y, x2 = p1->x, y2 = p1->y;
  IRECT clip = iRectClip(zb->pclip, 0, 0, zb->w, zb->h);
  uchar* ptr;
  COLOR clr = p0->clr;
  iRectClipLine(&clip, &x1, &y1, &x2, &y2);
  steps = (int)(MAX(ABS(x2 - x1), ABS(y2 - y1)));

  if (0 == steps) {
    return ;
  }

  ptr = zb->data + x1 * zb->cn + y1 * zb->data_step;
  x = x1 & 0xffff, y = y1 & 0xffff;
  increx = (F2FIX(x2 - x1)) / steps;
  increy = (F2FIX(y2 - y1)) / steps;

  for (i = 0; i < steps; ++i) {
    int ix = x >> 16, iy = y >> 16;
    ptr[ix + iy * step] = clr;
    x += increx, y += increy;
  }

  return ;
}
