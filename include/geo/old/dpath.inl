
/////////////////////////////////////////////////////////////////////////////
typedef struct dpath_t {
  FPOINT* pt;
  int* ln;
  int* flag;
  int n, c;
  double matrix[9];
} dpath_t;
int dpath_reset(dpath_t* p, bool initmatrix)
{
  p->n = p->c = 0;
  if (initmatrix) {
    affine_init(p->matrix);
  }
  return 0;
}
int dpath_free(dpath_t* p)
{
  FREE(p->pt);
  FREE(p->ln);
  FREE(p->flag);
  p->n = p->c = 0;
  return 0;
}
int dpath_npt(dpath_t* p)
{
  int i, npt=0;
  for (i=0; i<p->c; ++i) {
    npt += p->ln[i];
  }
  return npt;
}
int dpath_close_flag(dpath_t* p, int flag)
{
  if (p->n > 0) {
    if (p->c == 0 || (p->ln[p->c - 1] < p->n)) {
      int npt = dpath_npt(p);
      int n = p->n - npt;
      p->c++;
      MYREALLOC(p->ln, p->c);
      MYREALLOC(p->flag, p->c);
      p->flag[p->c - 1] = flag;
      p->ln[p->c - 1] = p->n - npt;
    }
  }
  return 0;
}
int dpath_close(dpath_t* p)
{
  return dpath_close_flag(p, 1);
}
FPOINT* dpath_alloc_push(dpath_t* g, int n)
{
  int oldn = g->n;
  g->n += n;
  MYREALLOC(g->pt, g->n);
  return g->pt + oldn;
}
#define DPATH_BEG(n) { FPOINT* _Path_0 = dpath_alloc_push(g, n), *_Path_00 = g->pt, *_Path_1 = _Path_0+n;
#define DPATH_VEC2(_x, _y) {double tx=_x, ty = _y; affine_transform(g->matrix, &tx, &ty); if ((_Path_00==_Path_0) || fabs(tx - _Path_0[-1].x)>0.001 || fabs(ty - _Path_0[-1].y)>0.001) {(_Path_0->x = tx, _Path_0->y = ty, ++_Path_0);} }
#define DPATH_VEC(pt) DPATH_VEC2(pt.x, pt.y)
#define DPATH_END() ASSERT(_Path_0<=_Path_1); g->n -= _Path_1-_Path_0;}
int dpath_lineto(dpath_t* g, double x, double y)
{
  DPATH_BEG(1);
  DPATH_VEC2(x, y);
  DPATH_END();
  return 0;
}
int dpath_moveto(dpath_t* p, double x, double y) {
  dpath_close_flag(p, 0);
  dpath_lineto(p, x, y);
  return 0;
}
int dpath_addpts(dpath_t* g, int n, const FPOINT* pt)
{
  int n1 = g->n;
  g->n += n;
  MYREALLOC(g->pt, g->n);
  MEMCPY(g->pt + n1, pt, n);
  return 0;
}
int dpath_addpts2(dpath_t* g, const FPOINT* pt, int n, const int* ln, const int* flag, int c)
{
  int n1 = g->n, c1 = g->c, i, n0=0;
  g->n += n;
  g->c += c;
  MYREALLOC(g->pt, g->n);
  MEMCPY(g->pt + n1, pt, n);
  MYREALLOC(g->ln, g->c);
  MEMCPY(g->ln + c1, ln, c);
  MYREALLOC(g->flag, g->c);
  MEMCPY(g->flag + c1, flag, c);
  if(c>0) {
    for (i=0; i<c1; ++i) {
      n0 += g->ln[i];
    }
    g->ln[c1] += n1-n0;
  }
  return 0;
}

int dpath_addpts3(dpath_t* g, const void* pt, int n, const int* ln, const int* flag, int c, int type, int step) {
  char* p = (char*)pt;
  int n1 = g->n, c1 = g->c, i, n0=0;
  FPOINT* pt1;
  g->n += n;
  g->c += c;
  MYREALLOC(g->pt, g->n);
  MYREALLOC(g->ln, g->c);
  MEMCPY(g->ln + c1, ln, c);
  MYREALLOC(g->flag, g->c);
  MEMCPY(g->flag + c1, flag, c);
  pt1 = g->pt + n1;
  if(c>0) {
    for (i=0; i<c1; ++i) {
      n0 += g->ln[i];
    }
    g->ln[c1] += n1-n0;
  }
switch (type) {
#define _POLYGON_COPYDATA(GCTYPE, TYPE) case GCTYPE: { ASSERT(step>=sizeof(TYPE)); for (i=0; i<n; ++i, p+=step) {double tx = ((TYPE*)(p))[0], ty = ((TYPE*)(p))[1];affine_transform(g->matrix, &tx, &ty);;pt1[i].x = tx;pt1[i].y = ty;} } break
  _POLYGON_COPYDATA(GC_INT8, char);
  _POLYGON_COPYDATA(GC_INT16, short);
  _POLYGON_COPYDATA(GC_INT32, int);
  _POLYGON_COPYDATA(GC_FLOAT, float);
  _POLYGON_COPYDATA(GC_DOUBLE, double);
#undef _POLYGON_COPYDATA
  }
  return 0;
}
void dpath_triangle(dpath_t* g, FPOINT a, FPOINT b, FPOINT c)
{
  DPATH_BEG(4);
  DPATH_VEC(a);
  DPATH_VEC(b);
  DPATH_VEC(c);
  DPATH_END();
}
void dpath_quad(dpath_t* g, FPOINT a, FPOINT b, FPOINT c, FPOINT d)
{
  DPATH_BEG(4);
  DPATH_VEC(a);
  DPATH_VEC(b);
  DPATH_VEC(c);
  DPATH_VEC(d);
  DPATH_END();
}
void dpath_rect(dpath_t* g, double x, double y, double w, double h)
{
  FPOINT a, b, c, d;
  PTSET(a, x, y);
  PTSET(c, x + w, y + h);
  PTSET(b, c.x, a.y);
  PTSET(d, a.x, c.y);
  dpath_quad(g, a, b, c, d);
}
void dpath_arc(dpath_t* g, double x, double y, double rx, double ry, double amin, double amax)
{
  int i, num_segments;
  if (rx == 0.0 || ry == 0.0) {
    dpath_lineto(g, x, y);
  }
  else {
    while(amax<amin) {
      amax+=2*PI;
    }
    while(amin>2*PI && amax>2*PI) {
      amin-=2*PI;
      amax-=2*PI;
    }
    num_segments = (int)BOUND(((rx+ry) * 0.2*fabs(amax - amin)), 4, 64);
    DPATH_BEG((num_segments + 1));
    for (i = 0; i < num_segments; i++) {
      double a = amin + ((float)i / (float)num_segments) * (amax - amin);
      DPATH_VEC2(x + cos(a) * rx, y + sin(a) * ry);
    }
    DPATH_END();
  }
}