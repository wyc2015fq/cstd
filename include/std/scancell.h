
//
typedef unsigned char cover_type; //cover_type
enum cover_scale_e {
  cover_shift = 8, //cover_shift
  cover_size = 1 << cover_shift, //cover_size
  cover_mask = cover_size - 1, //cover_mask
  cover_none = 0, //cover_none
  cover_full = cover_mask //cover_full
};
//poly_subpixel_scale_e
// These constants determine the subpixel accuracy, to be more precise,
// the number of bits of the fractional part of the coordinates.
// The possible coordinate capacity in bits can be calculated by formula:
// sizeof(int) * 8 - poly_subpixel_shift, i.e, for 32-bit integers and
// 8-bits fractional part the capacity is 24 bits.
enum poly_subpixel_scale_e {
  poly_subpixel_shift = 8, //poly_subpixel_shift
  poly_subpixel_scale = 1 << poly_subpixel_shift, //poly_subpixel_scale
  poly_subpixel_mask = poly_subpixel_scale - 1, //poly_subpixel_mask
};
//path_flags_e
enum path_flags_e {
  path_flags_none = 0, //path_flags_none
  path_flags_ccw = 0x10, //path_flags_ccw
  path_flags_cw = 0x20, //path_flags_cw
  path_flags_close = 0x40, //path_flags_close
  path_flags_mask = 0xF0 //path_flags_mask
};
enum aa_scale_e {
  aa_shift = 8,
  aa_scale = 1 << aa_shift,
  aa_mask = aa_scale - 1,
  aa_scale2 = aa_scale * 2,
  aa_mask2 = aa_scale2 - 1
};

#define is_vertex(c) (c >= path_move_to && c < path_end_poly)
#define is_drawing(c) (c >= path_line_to && c < path_end_poly)
#define is_stop(c) (c == path_stop)
#define is_move_to(c) (c == path_move_to)
#define is_line_to(c) (c == path_line_to)
#define is_curve(c) (c == path_curve3 || c == path_curve4)
#define is_curve3(c) (c == path_curve3)
#define is_curve4(c) (c == path_curve4)
#define is_end_poly(c) ((c & path_mask) == path_end_poly)
#define is_close(c) ((c & ~(path_flags_cw | path_flags_ccw)) == (path_end_poly | path_flags_close))
#define is_next_poly(c) (is_stop(c) || is_move_to(c) || is_end_poly(c))
#define is_cw(c) ((c & path_flags_cw) != 0)
#define is_ccw(c) ((c & path_flags_ccw) != 0)
#define is_oriented(c) ((c & (path_flags_cw | path_flags_ccw)) != 0)
#define is_closed(c) ((c & path_flags_close) != 0)
#define get_close_flag(c) (c & path_flags_close)
#define clear_orientation(c) (c & ~(path_flags_cw | path_flags_ccw))
#define get_orientation(c) (c & (path_flags_cw | path_flags_ccw))
#define set_orientation(c, o) (clear_orientation(c) | o)
enum cell_block_scale_e {
  cell_block_shift = 12,
  cell_block_size = 1 << cell_block_shift,
  cell_block_mask = cell_block_size - 1,
  cell_block_pool = 256,
  cell_block_limit = 1024,
  cell_aa_max = 0x7FFFFFFF,
};
typedef struct cell_aa {
  int x, y;
  //int packed_coord;
  int cover, area;
} cell_aa;

typedef struct scancell {
  int celllen;
  int maxcells;
  cell_aa m_curr_cell;
  cell_aa* cells;
} scancell;
#define set_curr_cell1(_x, _y) (m_curr_cell.x=_x, m_curr_cell.y=_y, out[outlen++]=m_curr_cell)
#define set_curr_cell2(s, _x, _y) if (_x!=s->m_curr_cell.x || _y!=s->m_curr_cell.y) { s->cells[s->celllen]=s->m_curr_cell, s->m_curr_cell.x=_x, s->m_curr_cell.y=_y, s->m_curr_cell.cover=0, s->m_curr_cell.area=0, s->celllen++; ASSERT(s->celllen<s->maxcells); }
CC_INLINE int set_curr_cell(scancell* s, int _x, int _y)
{
  if (_x != s->m_curr_cell.x || _y != s->m_curr_cell.y) {
    //if (ttt) printf("%d %d %d %d %d\n", s->celllen, s->m_curr_cell.x, s->m_curr_cell.y, s->m_curr_cell.cover, s->m_curr_cell.area);
    if (cell_aa_max != s->m_curr_cell.x && cell_aa_max != s->m_curr_cell.y) {
      s->cells[s->celllen] = s->m_curr_cell;
      s->celllen++;
    }
    s->m_curr_cell.x = _x, s->m_curr_cell.y = _y, s->m_curr_cell.cover = 0, s->m_curr_cell.area = 0;
    if (s->celllen >= s->maxcells) {
      ASSERT(s->celllen < s->maxcells);
    }
  }
  return 0;
}
// ey 第几行
// x1 x2 第几列到第几列
// y1 y2 行的分数部分
CC_INLINE int render_hline(int ey, int x1, int y1, int x2, int y2, scancell* s)
{
  int ex1 = x1 >> poly_subpixel_shift;
  int ex2 = x2 >> poly_subpixel_shift;
  int fx1 = x1 & poly_subpixel_mask;
  int fx2 = x2 & poly_subpixel_mask;
  int delta, p, first, dx;
  int incr, lift, mod, rem;
  //trivial case. Happens often
  if (y1 == y2) {
    set_curr_cell(s, ex2, ey);
    return s->celllen;
  }
  //everything is located in a single cell. That is easy!
  if (ex1 == ex2) {
    delta = y2 - y1;
    s->m_curr_cell.cover += delta;
    s->m_curr_cell.area += (fx1 + fx2) * delta;
    return s->celllen;
  }
  //ok, we'll have to render a run of adjacent cells on the same
  //hline...
  p = (poly_subpixel_scale - fx1) * (y2 - y1);
  first = poly_subpixel_scale;
  incr = 1;
  dx = x2 - x1;
  if (dx < 0) {
    p = fx1 * (y2 - y1);
    first = 0;
    incr = -1;
    dx = -dx;
  }
  delta = p / dx;
  mod = p % dx;
  if (mod < 0) {
    delta--;
    mod += dx;
  }
  s->m_curr_cell.cover += delta;
  s->m_curr_cell.area += (fx1 + first) * delta;
  ex1 += incr;
  set_curr_cell(s, ex1, ey);
  y1 += delta;
  if (ex1 != ex2) {
    p = poly_subpixel_scale * (y2 - y1 + delta);
    lift = p / dx;
    rem = p % dx;
    if (rem < 0) {
      lift--;
      rem += dx;
    }
    mod -= dx;
    while (ex1 != ex2) {
      delta = lift;
      mod += rem;
      if (mod >= 0) {
        mod -= dx;
        delta++;
      }
      s->m_curr_cell.cover += delta;
      s->m_curr_cell.area += poly_subpixel_scale * delta;
      y1 += delta;
      ex1 += incr;
      set_curr_cell(s, ex1, ey);
    }
  }
  delta = y2 - y1;
  s->m_curr_cell.cover += delta;
  s->m_curr_cell.area += (fx2 + poly_subpixel_scale - first) * delta;
  return s->celllen;
}
#define upscale(v) iround((v) * poly_subpixel_scale)
CC_INLINE int render_line(int x1, int y1, int x2, int y2, scancell* s)
{
  enum dx_limit_e { dx_limit = 16384 << poly_subpixel_shift };
  int p, rem, mod, lift, delta, first, incr;
  int dx = x2 - x1, dy, ex1, ey1, ey2, fy1, fy2, x_from, x_to;
  if (dx >= dx_limit || dx <= -dx_limit) {
    int cx = (x1 + x2) >> 1;
    int cy = (y1 + y2) >> 1;
    render_line(x1, y1, cx, cy, s);
    render_line(cx, cy, x2, y2, s);
  }
  dy = y2 - y1;
  ex1 = x1 >> poly_subpixel_shift;
  //ex2 = x2 >> poly_subpixel_shift;
  ey1 = y1 >> poly_subpixel_shift;
  ey2 = y2 >> poly_subpixel_shift;
  fy1 = y1 & poly_subpixel_mask;
  fy2 = y2 & poly_subpixel_mask;
  set_curr_cell(s, ex1, ey1);
  //everything is on a single hline
  if (ey1 == ey2) {
    render_hline(ey1, x1, fy1, x2, fy2, s);
    return s->celllen;
  }
  //Vertical line - we have to calculate start and end cells,
  //and then - the common values of the area and coverage for
  //all cells of the line. We know exactly there's only one
  //cell, so, we don't have to call render_hline().
  incr = 1;
  if (dx == 0) {
    int ex = x1 >> poly_subpixel_shift;//整数部分
    int two_fx = (x1 - (ex << poly_subpixel_shift)) << 1;//小数部分的2倍
    int area;
    first = poly_subpixel_scale; //256
    if (dy < 0) {
      first = 0;
      incr = -1;
    }
    x_from = x1;
    //render_hline(ey1, x_from, fy1, x_from, first);
    delta = first - fy1; // 到下一条横线的增量
    s->m_curr_cell.cover += delta;
    s->m_curr_cell.area += two_fx * delta;
    ey1 += incr;
    set_curr_cell(s, ex, ey1);
    delta = first + first - poly_subpixel_scale;
    area = two_fx * delta;
    while (ey1 != ey2) {
      //render_hline(ey1, x_from, poly_subpixel_scale - first, x_from, first);
      s->m_curr_cell.cover = delta;
      s->m_curr_cell.area = area;
      ey1 += incr;
      set_curr_cell(s, ex, ey1);
    }
    //render_hline(ey1, x_from, poly_subpixel_scale - first, x_from, fy2);
    delta = fy2 - poly_subpixel_scale + first;
    s->m_curr_cell.cover += delta;
    s->m_curr_cell.area += two_fx * delta;
    return s->celllen;
  }
  //ok, we have to render several hlines
  p = (poly_subpixel_scale - fy1) * dx;
  first = poly_subpixel_scale;
  if (dy < 0) {
    p = fy1 * dx;
    first = 0;
    incr = -1;
    dy = -dy;
  }
  delta = p / dy;
  mod = p % dy;
  if (mod < 0) {
    delta--;
    mod += dy;
  }
  x_from = x1 + delta;
  render_hline(ey1, x1, fy1, x_from, first, s);
  ey1 += incr;
  set_curr_cell(s, x_from >> poly_subpixel_shift, ey1);
  if (ey1 != ey2) {
    p = poly_subpixel_scale * dx;
    lift = p / dy;
    rem = p % dy;
    if (rem < 0) {
      lift--;
      rem += dy;
    }
    mod -= dy;
    while (ey1 != ey2) {
      delta = lift;
      mod += rem;
      if (mod >= 0) {
        mod -= dy;
        delta++;
      }
      x_to = x_from + delta;
      render_hline(ey1, x_from, poly_subpixel_scale - first, x_to, first, s);
      x_from = x_to;
      ey1 += incr;
      set_curr_cell(s, x_from >> poly_subpixel_shift, ey1);
    }
  }
  render_hline(ey1, x_from, poly_subpixel_scale - first, x2, fy2, s);
  return s->celllen;
}
//BYTE m_gamma[256];
#define M_GAMMA(_C) (_C)
#if 0
#define CALCULATE_ALPHA(alpha, area) { \
    int cover = area >> (poly_subpixel_shift * 2 + 1 - aa_shift); \
    if (cover < 0) { cover = -cover; } \
    if (cover > aa_mask) { cover = aa_mask; } \
    alpha = cover; \
  }
#else
#define CALCULATE_ALPHA(alpha, area) alpha = calculate_alpha1(area, fill_even_odd)
CC_INLINE unsigned calculate_alpha1(int area, BOOL fill_even_odd)
{
  //enum { m_filling_rule = fill_non_zero };
  //enum { m_filling_rule = fill_even_odd };
  int cover = area >> (poly_subpixel_shift * 2 + 1 - aa_shift);
  if (cover < 0) {
    cover = -cover;
  }
  if (fill_even_odd) {
    cover &= aa_mask2;
    if (cover > aa_scale) {
      cover = aa_scale2 - cover;
    }
  }
  if (cover > aa_mask) {
    cover = aa_mask;
  }
  return M_GAMMA(cover);
}
#endif
CC_INLINE int gamma_pow(int* m_gamma, double g)
{
  int i;
  for (i = 0; i < aa_scale; i++) {
    m_gamma[i] = uround(pow(i * 1. / aa_mask, g) * aa_mask);
  }
  return 0;
}
CC_INLINE int gamma_none(int* m_gamma)
{
  int i;
  for (i = 0; i < aa_scale; i++) {
    m_gamma[i] = i;
  }
  return 0;
}
typedef double coord_type;
CC_INLINE double mul_div(double a, double b, double c)
{
  return a * b / c;
}
CC_INLINE int line_clip_y(FPOINT* pt, const DRECT* clip_box, coord_type x1, coord_type y1, coord_type x2, coord_type y2, unsigned f1, unsigned f2)
{
  FPOINT* pt0 = pt;
  double _x1 = clip_box->l;
  double _y1 = clip_box->t;
  double _x2 = clip_box->r;
  double _y2 = clip_box->b;
  f1 &= 10;
  f2 &= 10;
  if ((f1 | f2) == 0) {
    // Fully visible
    *pt0++ = fPOINT(x1, y1);
    *pt0++ = fPOINT(x2, y2);
    return 2;
  }
  else {
    coord_type tx1, ty1, tx2, ty2;
    if (f1 == f2) {
      // Invisible by Y
      return 0;
    }
    tx1 = x1, ty1 = y1, tx2 = x2, ty2 = y2;
    if (f1 & 8) { // y1 < clip.y1
      tx1 = x1 + mul_div(_y1 - y1, x2 - x1, y2 - y1);
      ty1 = _y1;
    }
    if (f1 & 2) { // y1 > clip.y2
      tx1 = x1 + mul_div(_y2 - y1, x2 - x1, y2 - y1);
      ty1 = _y2;
    }
    if (f2 & 8) { // y2 < clip.y1
      tx2 = x1 + mul_div(_y1 - y1, x2 - x1, y2 - y1);
      ty2 = _y1;
    }
    if (f2 & 2) { // y2 > clip.y2
      tx2 = x1 + mul_div(_y2 - y1, x2 - x1, y2 - y1);
      ty2 = _y2;
    }
    *pt0++ = fPOINT(tx1, ty1);
    *pt0++ = fPOINT(tx2, ty2);
    return 2;
  }
}
CC_INLINE unsigned clipping_flags_x(double x, const DRECT* clip_box)
{
  return (x > clip_box->r) | ((x < clip_box->l) << 2);
}
CC_INLINE unsigned clipping_flags_y(double y, const DRECT* clip_box)
{
  return ((y > clip_box->b) << 1) | ((y < clip_box->t) << 3);
}
CC_INLINE unsigned clipping_flags(double x, double y, const DRECT* clip_box)
{
  return (x > clip_box->r) | ((y > clip_box->b) << 1) | ((x < clip_box->l) << 2) | ((y < clip_box->t) << 3);
}
CC_INLINE int clip_line_to(FPOINT* pt, const DRECT* clip_box, double m_x1, double m_y1, coord_type x2, coord_type y2)
{
  double _x1 = clip_box->l;
  double _y1 = clip_box->t;
  double _x2 = clip_box->r;
  double _y2 = clip_box->b;
  unsigned int f1, f2, f3, f4;
  coord_type x1, y1, y3, y4;
  unsigned int m_f1 = clipping_flags(m_x1, m_y1, clip_box);
  FPOINT* pt0 = pt;
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
    y3 = y1 + mul_div(_x2 - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    pt += line_clip_y(pt, clip_box, x1, y1, _x2, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, _x2, y3, _x2, y2, f3, f2);
    break;
  case 2: // x1 > clip.x2
    y3 = y1 + mul_div(_x2 - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    pt += line_clip_y(pt, clip_box, _x2, y1, _x2, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, _x2, y3, x2, y2, f3, f2);
    break;
  case 3: // x1 > clip.x2 && x2 > clip.x2
    pt += line_clip_y(pt, clip_box, _x2, y1, _x2, y2, f1, f2);
    break;
  case 4: // x2 < clip.x1
    y3 = y1 + mul_div(_x1 - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    pt += line_clip_y(pt, clip_box, x1, y1, _x1, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, _x1, y3, _x1, y2, f3, f2);
    break;
  case 6: // x1 > clip.x2 && x2 < clip.x1
    y3 = y1 + mul_div(_x2 - x1, y2 - y1, x2 - x1);
    y4 = y1 + mul_div(_x1 - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    f4 = clipping_flags_y(y4, clip_box);
    pt += line_clip_y(pt, clip_box, _x2, y1, _x2, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, _x2, y3, _x1, y4, f3, f4);
    pt += line_clip_y(pt, clip_box, _x1, y4, _x1, y2, f4, f2);
    break;
  case 8: // x1 < clip.x1
    y3 = y1 + mul_div(_x1 - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    pt += line_clip_y(pt, clip_box, _x1, y1, _x1, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, _x1, y3, x2, y2, f3, f2);
    break;
  case 9: // x1 < clip.x1 && x2 > clip.x2
    y3 = y1 + mul_div(_x1 - x1, y2 - y1, x2 - x1);
    y4 = y1 + mul_div(_x2 - x1, y2 - y1, x2 - x1);
    f3 = clipping_flags_y(y3, clip_box);
    f4 = clipping_flags_y(y4, clip_box);
    pt += line_clip_y(pt, clip_box, _x1, y1, _x1, y3, f1, f3);
    pt += line_clip_y(pt, clip_box, _x1, y3, _x2, y4, f3, f4);
    pt += line_clip_y(pt, clip_box, _x2, y4, _x2, y2, f4, f2);
    break;
  case 12: // x1 < clip.x1 && x2 < clip.x1
    pt += line_clip_y(pt, clip_box, _x1, y1, _x1, y2, f1, f2);
    break;
  }
  m_f1 = f2;
  m_x1 = x2;
  m_y1 = y2;
  return pt - pt0;
}
CC_INLINE int scancell_free(scancell* s)
{
  FREE(s->cells);
  //GFREE(s->lines);
  return 0;
}
CC_INLINE int cell_sort(cell_aa* cells, int len) {
  cell_aa* cells_begin = cells;
  cell_aa* cells_end = cells + len;
#define cell_aa_less(_a, _b) (((_a).y<(_b).y) || ((_a).y==(_b).y && (_a).x<(_b).x))
  //#define packcell(_a) (unsigned __int64)((((unsigned __int64)((_a).y))<<32)|((_a).x))
  //#define cell_aa_less(_a, _b) (packcell(_a)<packcell(_b))
  QSORT(cells_begin, cells_end, cell_aa_less, cell_aa);
#undef cell_aa_less
  return 0;
}
CC_INLINE int cell_quicksort(cell_aa* cells0, int len0) {
  int* len = NULL;
  cell_aa* cells = NULL;
  cell_aa** aa = NULL;
  int i, h, y, miny = cell_aa_max, maxy = 0, pos = 0, celllen = 0, sz;
  for (i = 0; i<len0; ++i) {
    if (cells0[i].y<cell_aa_max) {
      y = cells0[i].y;
      miny = MIN(miny, y);
      maxy = MAX(maxy, y);
    }
  }
  h = maxy - miny + 1;
  sz = sizeof(cell_aa)*len0 + h * sizeof(int) + h * sizeof(cell_aa*);
  cells = (cell_aa*)malloc(sz);
  len = (int*)(cells + len0);
  aa = (cell_aa**)(len + h);
  MEMSET(len, 0, h);
  for (i = 0; i<len0; ++i) {
    if (cells0[i].y<cell_aa_max) {
      y = cells0[i].y - miny;
      len[y]++;
    }
  }
  pos = 0;
  for (i = 0; i<h; ++i) {
    aa[i] = cells + pos;
    pos += len[i];
  }
  celllen = pos;
  MEMSET(len, 0, h);
  for (i = 0; i<len0; ++i) {
    if (cells0[i].y<cell_aa_max) {
      y = cells0[i].y - miny;
      aa[y][len[y]++] = cells0[i];
    }
  }
  for (i = 0; i<h; ++i) {
    if (len[i]>0) {
      cell_aa* cells_begin = aa[i];
      cell_aa* cells_end = aa[i] + len[i];
#define cell_aa_less(_a, _b) ((_a).x<(_b).x)
      QSORT(cells_begin, cells_end, cell_aa_less, cell_aa);
#undef cell_aa_less
    }
  }
  MEMCPY(cells0, cells, celllen);
  FREE(cells);
  return 0;
}
CC_INLINE int scancell_get_maxsize(const FPOINT* pt0, const int* lens, int n, const IRECT* pclip, IRECT* prc)
{
  typedef FPOINT point_type;
  const point_type* pt;
  int i, j, len, npt = 0, maxcells = 0, n3, k, t;
  FPOINT pt3[10];
  DRECT rd[1] = { 0 };
  IRECT rc = { 0 };
  *rd = dRECT(pclip->l, pclip->t, pclip->r, pclip->b);
  if (n <= 0 || NULL == pt0) {
    return 0;
  }
  rc = iRECT(pt0->x, pt0->y, pt0->x, pt0->y);
  prc = prc ? prc : &rc;
  for (pt = pt0, j = 0; j < n; ++j) {
    len = abs(lens[j]);
    for (i = 0; i < len; ++i) {
      int j = (i + 1) % len;
      //lt.x = MIN(lt.x, pt[i].x);
      //lt.y = MIN(lt.y, pt[i].y);
      //rb.x = MAX(rb.x, pt[i].x);
      //rb.y = MAX(rb.y, pt[i].y);
      prc->l = (int)MIN(prc->l, pt[i].x);
      prc->t = (int)MIN(prc->t, pt[i].y);
      prc->r = (int)MAX(prc->r, pt[i].x + 1);
      prc->b = (int)MAX(prc->b, pt[i].y + 1);
      n3 = clip_line_to(pt3, rd, pt[i].x, pt[i].y, pt[j].x, pt[j].y);
      for (k = 0; k < n3; k += 2) {
        t = (int)ceil(fabs(pt3[k].x - pt3[k + 1].x) + fabs(pt3[k].y - pt3[k + 1].y));
        maxcells += (t)+5;
      }
    }
    pt += len;
  }
  *prc = iRectInter(*prc, *pclip);
  return maxcells;
}
CC_INLINE int scancell_set_d(scancell* s, const FPOINT* pt0, const int* lens, int n, const IRECT* pclip, IRECT* prc)
{
  typedef FPOINT point_type;
  const point_type* pt;
  int i, j, len, maxcells = 0, n3, k;
  FPOINT pt3[10];
  DRECT rd[1];
  IRECT rc = { 0 };
  *rd = dRECT(pclip->l, pclip->t, pclip->r, pclip->b);
  prc = prc ? prc : &rc;
  maxcells = scancell_get_maxsize(pt0, lens, n, pclip, prc);
  if (0 >= maxcells) {
    return 0;
  }
  MYREALLOC(s->cells, maxcells);
  ASSERT(s->cells);
  //memset(s->cells, 0, sizeof(cell_aa)*maxcells);
  s->celllen = 0;
  s->maxcells = maxcells;
#define INICELL(_c) ((_c)->x=(_c)->y=cell_aa_max,(_c)->cover=(_c)->area=0)
  INICELL(&s->m_curr_cell);
#undef INICELL

  for (pt = pt0, j = 0; j < n; ++j) {
    len = abs(lens[j]);
    for (i = 0; i < len; ++i) {
      int j = (i + 1) % len;
      n3 = clip_line_to(pt3, rd, pt[i].x, pt[i].y, pt[j].x, pt[j].y);
      for (k = 0; k < n3; k += 2) {
        render_line(upscale(pt3[k].x), upscale(pt3[k].y), upscale(pt3[k + 1].x), upscale(pt3[k + 1].y), s);
      }
    }
    pt += len;
  }
  set_curr_cell(s, cell_aa_max, cell_aa_max);
  //cell_sort(s->cells, s->celllen);
  cell_quicksort(s->cells, s->celllen);
  return 0;
}
CC_INLINE int ClipLine(const IRECT* pclip, IPOINT* pt1, IPOINT* pt2)
{
  int result = 0;
  int x1, y1, x2, y2;
  int c1, c2;
  int left = pclip->l, top = pclip->t;
  int right = pclip->r - 1, bottom = pclip->b - 1;
  if (!pt1 || !pt2) {
    return (1);
  }
  if (right < 0 || bottom < 0) {
    return (1);
  }
  x1 = pt1->x;
  y1 = pt1->y;
  x2 = pt2->x;
  y2 = pt2->y;
  c1 = (x1 < left) + (x1 > right) * 2 + (y1 < top) * 4 + (y1 > bottom) * 8;
  c2 = (x2 < left) + (x2 > right) * 2 + (y2 < top) * 4 + (y2 > bottom) * 8;
  if ((c1 & c2) == 0 && (c1 | c2) != 0) {
    int a;
    if (c1 & 12) {
      a = c1 < 8 ? 0 : bottom;
      x1 += (int)(((int64)(a - y1)) * (x2 - x1) / (y2 - y1));
      y1 = a;
      c1 = (x1 < 0) + (x1 > right) * 2;
    }
    if (c2 & 12) {
      a = c2 < 8 ? 0 : bottom;
      x2 += (int)(((int64)(a - y2)) * (x2 - x1) / (y2 - y1));
      y2 = a;
      c2 = (x2 < 0) + (x2 > right) * 2;
    }
    if ((c1 & c2) == 0 && (c1 | c2) != 0) {
      if (c1) {
        a = c1 == 1 ? 0 : right;
        y1 += (int)(((int64)(a - x1)) * (y2 - y1) / (x2 - x1));
        x1 = a;
        c1 = 0;
      }
      if (c2) {
        a = c2 == 1 ? 0 : right;
        y2 += (int)(((int64)(a - x2)) * (y2 - y1) / (x2 - x1));
        x2 = a;
        c2 = 0;
      }
    }
    assert((c1 & c2) != 0 || (x1 | y1 | x2 | y2) >= 0);
    pt1->x = x1;
    pt1->y = y1;
    pt2->x = x2;
    pt2->y = y2;
  }
  result = (c1 | c2) == 0;
  if (0 == result) {
    pt1->x = 0;
    pt1->y = 0;
    pt2->x = 0;
    pt2->y = 0;
  }
  return result;
}
CC_INLINE int bf_scancell_set_d(scancell* s, const FPOINT* pt0, const int* lens, int n, const IRECT* pclip)
{
  typedef FPOINT point_type;
  const point_type* pt;
  int i, j, len, n3, k;
  FPOINT pt3[10];
  DRECT rd[1];
  *rd = dRECT(pclip->l, pclip->t, pclip->r, pclip->b);
  ASSERT(s->cells);
#define INICELL(_c) ((_c)->x=(_c)->y=cell_aa_max,(_c)->cover=(_c)->area=0)
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
  set_curr_cell(s, cell_aa_max, cell_aa_max);
#define cell_aa_less(_a, _b) (((_a).y<(_b).y) || ((_a).y==(_b).y && (_a).x<(_b).x))
  //#define packcell(_a) (unsigned __int64)((((unsigned __int64)((_a).y))<<32)|((_a).x))
  //#define cell_aa_less(_a, _b) (packcell(_a)<packcell(_b))
  QSORT(s->cells, s->cells + s->celllen, cell_aa_less, cell_aa);
#undef cell_aa_less
  return 0;
}
int is_int_rect(const FPOINT* pt0, const int* lens, int n, IRECT* prc)
{
  if (1 == n && 4 == abs(lens[0])) {
    int i;
    IPOINT ipt[4], pt1, pt3;
    for (i = 0; i < 4; ++i) {
      double x = ipt[i].x = (int)pt0[i].x;
      double y = ipt[i].y = (int)pt0[i].y;
      double t = fabs(x - pt0[i].x) + fabs(y - pt0[i].y);
      if (t > 0.0001) {
        return 0;
      }
    }
    PTSET(pt1, ipt[0].x, ipt[2].y);
    PTSET(pt3, ipt[2].x, ipt[0].y);
    *prc = iRECT(MIN(pt1.x, pt3.x), MIN(pt1.y, pt3.y), MAX(pt1.x, pt3.x), MAX(pt1.y, pt3.y));
    if ((pt1.x == ipt[1].x && pt1.y == ipt[1].y) && (pt3.x == ipt[3].x && pt3.y == ipt[3].y)) {
      return 1;
    }
    if ((pt1.x == ipt[3].x && pt1.y == ipt[3].y) && (pt3.x == ipt[1].x && pt3.y == ipt[1].y)) {
      return 1;
    }
  }
  return 0;
}
