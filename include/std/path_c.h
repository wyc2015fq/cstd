
#ifndef _PATH_INL_
#define _PATH_INL_

#include "gdi_c.h"
#include "vcgen_c.h"
#include "string_c.h"
#include "iconv_c.h"
//#include "StringFormat.inl"

typedef struct {
  int Size;
  int Count;
  FPOINT* Points;
  BYTE* Types; // PathPointType
  FillMode fill;
  int curlen; /* whether the next drawing action starts a new figure */
} Path;

#define PATH_LOOP_BEGIN1  {\
int i, pos=0;  \
for (i=1; i<Count; ++i) {  \
  int end = 0, closed = 0;  \
  if (Types[i] & PathPointTypeCloseSubpath) {  \
    closed = true;  \
    end = i + 1;  \
  } else if (PathPointTypeStart==(Types[i] & PathPointTypePathTypeMask) || i==(Count-1)) {  \
    closed = false;  \
    end = i + (i==(Count-1));  \
  }  \
  if (end>0) {


#define PATH_LOOP_BEGIN  { \
int i, pos=0;  \
  for (i=1; i<Count; ++i) { \
    int t = Types[i] & PathPointTypePathTypeMask; \
    if ((i==(Count-1) || PathPointTypeStart==t)) { \
      int end = 1+i-(PathPointTypeStart==t); \
      bool closed = !!(Types[end-1] & PathPointTypeCloseSubpath);

#define PATH_LOOP_END      pos = end;  }  } }


#define PATH_COPY_BEGIN(path, path2) {  \
Path flatcopy[1] = {0}; \
if (path==path2) { PathClone(path, flatcopy); path = flatcopy; } {

#define PATH_COPY_END()  } PathFree(flatcopy); }

#define PATH_FLATCOPY_BEGIN {  \
Path flatcopy[1] = {0}; \
if (!PathIsFlatten(path)) { \
  PathFlatten(path, flatness, flatcopy); \
  path = flatcopy; \
} else if (path==path2) { \
  PathClone(path, flatcopy); \
path = flatcopy; } {int _Count = path->Count

#define PATH_FLATCOPY_END  } PathFree(flatcopy); }


#define is_newfigure
void PathFree(Path* path) {
  if (path->Points) {
    free(path->Points);
  }
  if (path->Types) {
    free(path->Types);
  }
  path->Types = NULL;
  path->Points = NULL;
  path->Count = 0;
}
static void PathFrees(Path* path, unsigned count) {
  for (;count--;) {
    PathFree(path++);
  }
}
#define PathSetSize(path, n)  PathSetSize_(path, n, __FILE__, __LINE__)
#define PathAddSize(path, n)  PathSetSize_(path, n + path->Count, __FILE__, __LINE__)
static Status PathSetSize_(Path* path, int n, const char* file, int line) {
  int old_count = path->Count;
  if (n>0) {
    if (n > path->Size) {
      path->Points = (FPOINT*)realloc(path->Points, sizeof(FPOINT)*n);
      if (NULL==path->Points) {
        return OutOfMemory;
      }
      path->Types = (BYTE*)realloc(path->Types, sizeof(BYTE)*n);
      if (NULL==path->Types) {
        return OutOfMemory;
      }
      path->Size = n;
    }
  } else {
    PathFree(path);
  }
  return Ok;
}

typedef struct path_list_node_t path_list_node_t;
struct path_list_node_t {
  FPOINT pt;
  BYTE type; /* PathPointTypeStart or PathPointTypeLine */
  path_list_node_t *next;
};

/* init list */
static BOOL init_path_list(path_list_node_t **node, float x, float y)
{
  *node = (path_list_node_t*)malloc(sizeof(path_list_node_t));
  if(!*node)
    return FALSE;
  
  (*node)->pt.x = x;
  (*node)->pt.y = y;
  (*node)->type = PathPointTypeStart;
  (*node)->next = NULL;
  
  return TRUE;
}

/* free all nodes including argument */
static void free_path_list(path_list_node_t *node)
{
  path_list_node_t *n = node;
  
  while(n) {
    n = n->next;
    free(node);
    node = n;
  }
}

/* Add a node after 'node' */
/*
* Returns
*  pointer on success
*  NULL    on allocation problems
*/
static path_list_node_t* add_path_list_node(path_list_node_t *node, float x, float y, BOOL type)
{
  path_list_node_t *n;
  
  n = (path_list_node_t*)malloc(sizeof(path_list_node_t));
  if(!n)
    return NULL;
  
  n->pt.x  = x;
  n->pt.y  = y;
  n->type  = type;
  n->next  = node->next;
  node->next = n;
  
  return n;
}

/* returns element count */
static int path_list_count(path_list_node_t *node)
{
  int count = 1;
  
  while((node = node->next))
    ++count;
  
  return count;
}
//#include "stb/stb_truetype.h"

Status PathSetPoint(Path *path, int i, float x, float y, PathPointType type) {
  path->Points[i].x = x;
  path->Points[i].y = y;
  path->Types[i] = path->curlen ? type : PathPointTypeStart;
  path->curlen++;
  return Ok;
}
static Status PathAddPoint(Path *path, float x, float y, PathPointType type) {
  int old_count;
  if(!path)
    return InvalidParameter;
  
  old_count = path->Count;
  if(Ok!=PathAddSize(path, 1))
    return OutOfMemory;
  
  path->Points[old_count].x = x;
  path->Points[old_count].y = y;
  path->Types[old_count] = (path->curlen ? type : PathPointTypeStart);
  path->curlen++;
  path->Count = old_count + 1;
  return Ok;
}

static Status PathAddPathImpl(Path *path, const FPOINT* Points, const BYTE* Types, int Count, BOOL connect) {
  int old_count = path->Count;
  
  TRACE("(%p, %p, %d)\n", path, Points, connect);
  
  if(!path || !Points)
    return InvalidParameter;
  
  if(Ok!=PathAddSize(path, Count))
    return OutOfMemory;
  
  memcpy(&path->Points[old_count], Points, Count * sizeof(FPOINT));
  memcpy(&path->Types[old_count], Types, Count);
  
  if(!path->curlen || !connect) {
    path->Types[old_count] = PathPointTypeStart;
  }
  
  path->Count = old_count + Count;
  path->curlen += Count;
  return Ok;
}
static Status PathAddPath(Path *path, const Path* addingPath, BOOL connect)
{
  return PathAddPathImpl(path, addingPath->Points, addingPath->Types, addingPath->Count, connect);
}
static Status PathMoveTo(Path *path, float x, float y) {
  return PathAddPoint(path, x, y, PathPointTypeStart);
}
static Status PathLineTo(Path *path, float x, float y) {
  return PathAddPoint(path, x, y, PathPointTypeLine);
}
//Status PathBezierTo(Path *path, float x, float y) {  return PathAddPoint(path, x, y, PathPointTypeBezier);}
//Status PathQuadTo(Path *path, float x, float y) {  return PathAddPoint(path, x, y, PathPointTypeBezier2);}
static Status PathQuadTo(Path *path, float x1, float y1, float x2, float y2) {
  FPOINT Points[] = {x1, y1, x2, y2, };
  BYTE Types[] = {PathPointTypeBezier2, PathPointTypeLine };
  return PathAddPathImpl(path, Points, Types, 2, true);
}
#define PathCurveTo PathCubicTo
static Status PathCubicTo(Path *path, float x1, float y1, float x2, float y2, float x3, float y3) {
  FPOINT Points[] = {x1, y1, x2, y2, x3, y3, };
  BYTE Types[] = {PathPointTypeBezier3, PathPointTypeBezier3, PathPointTypeLine };
  return PathAddPathImpl(path, Points, Types, 3, true);
}
static Status PathRemoveSame(Path* path) {
  path->Count = path_remove_same(path->Points, path->Types, path->Count, 0.001);
  return Ok;
}
static Status PathCloseFigure(Path* path)
{
  TRACE("(%p)\n", path);
  
  if(!path)
    return InvalidParameter;
  
  if(path->Count > 0) {
    PathRemoveSame(path);
    path->Types[path->Count - 1] |= PathPointTypeCloseSubpath;
  }
  path->curlen = 0;
  return Ok;
}

#define DPATH_BEG(n) { int _old_count = path->Count; if (Ok!=PathAddSize(path, (n))) {return OutOfMemory;}
#define DPATH_VEC(_x, _y, _cmd)  path->Points[_old_count].x = _x, path->Points[_old_count].y = _y, path->Types[_old_count] = _cmd; ++_old_count;
#define DPATH_END()  ASSERT(_old_count<=path->Size); path->Types[path->Count] = path->curlen ? PathPointTypeLine : PathPointTypeStart; path->curlen += _old_count - path->Count; path->Count = _old_count;}
static Status PathAddArc2(Path* path, double x, double y, double rx, double ry, double startAngle, double sweepAngle)
{
  int count, old_count = path->Count;
  if (rx == 0.0 || ry == 0.0) {
    return PathLineTo(path, x, y);
  }
  count = vcgen_arc(NULL, 0, x, y, rx, ry, startAngle, sweepAngle);
  if (Ok!=PathAddSize(path, count)) {
    return OutOfMemory;
  }
  count = vcgen_arc(path->Points + path->Count, 0, x, y, rx, ry, startAngle, sweepAngle);
  MEMSET(path->Types + old_count, PathPointTypeLine, count);
  if(0==old_count || 0==path->curlen) {
    path->Types[old_count] = PathPointTypeStart;
  }
  path->curlen += count;
  path->Count += count;
  if (fabs(sweepAngle-360)<0.001) {
    path->Types[old_count] = PathPointTypeStart;
    path->Types[path->Count - 1] |= PathPointTypeCloseSubpath;
    path->curlen = 0;
  }
  return Ok;
}
static Status PathAddArcToPoint(Path* path, float x1, float y1, float x2, float y2, float radius, bool m_cw) {
  FCIRCLE c[2] = {{x1, y1, radius}, {x2, y2, radius}};
  FPOINT p[2] = {0};
  if (cici2_intersection_point(c, c+1, p)) {
    float x = p[!m_cw].x, y = p[!m_cw].y;
    float startAngle = atan2(y1 - y, x1 - x);
    float endAngle = atan2(y2 - y, x2 - x);
    float sweepAngle = endAngle - startAngle;
    startAngle = rad2deg(startAngle);
    sweepAngle = rad2deg(sweepAngle);
    return PathAddArc2(path, x, y, radius, radius, startAngle, sweepAngle);
  }
  return Ok;
}
static Status PathAddArc(Path *path, float x, float y, float w, float h, float startAngle, float sweepAngle) {
  float w2 = w/2, h2 = h/2;
  return PathAddArc2(path, x+w2, y+h2, w2, h2, startAngle, sweepAngle);
}
static Status PathAddArc_(Path *path, float x, float y, float w, float h, float startAngle, float sweepAngle)
{
  int count, old_count;
  
  TRACE("(%p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n",
    path, x, y, w, h, startAngle, sweepAngle);
  
  if(!path)
    return InvalidParameter;
  if (w<0.0001 && h<0.0001) {
    return PathLineTo(path, x, y);
  } else if(w<0.0001 && h<0.0001) {
    PathLineTo(path, x, y);
    return PathLineTo(path, x+w, y+h);
  }
  
  count = arc2polybezier(NULL, NULL, x, y, w, h, startAngle, sweepAngle);
  
  if(count == 0)
    return Ok;
  
  old_count = path->Count;
  
  if(Ok!=PathAddSize(path, count))
    return OutOfMemory;
  
  arc2polybezier(&path->Points[old_count], &path->Types[old_count], x, y, w, h, startAngle, sweepAngle);
  
  path->Count = old_count + count;
  path->Types[old_count] = path->curlen ? PathPointTypeLine : PathPointTypeStart;
  path->curlen += count;
  return Ok;
}
static Status PathAddArc3(Path *path, float x1, float y1, float x2, float y2, float startAngle, float sweepAngle) {
  return PathAddArc(path, x1, y1, x2-x1, y2-y1, startAngle, sweepAngle);
}

Status PathAddArcR(Path *path, IRECT rc, float startAngle, float sweepAngle) {
  return PathAddArc3(path, rc.l, rc.t, rc.r, rc.b, startAngle, sweepAngle);
}
// nw---ne
// sw---se
static int PathAddRoundRectangle(Path* path, float x, float y, float w, float h, float nw, float ne, float se, float sw)
{
  float l = x, t = y, r = x+w, b = y+h;
  path->curlen = 0;
  PathAddArc3(path, r - ne, t, r, t + ne, -90, 90);
  PathAddArc3(path, r - se, b - se, r, b, 0, 90);
  PathAddArc3(path, l, b - sw, l + sw, b, 90, 90);
  PathAddArc3(path, l, t, l + nw, t + nw, 180, 90);
  PathCloseFigure(path);
  return 0;
}
static Status PathAddRoundPoly(Path* path, const FPOINT* pt, int ptlen, bool m_closed, double radius) {
  int old_count = path->Count;
  int count = vcgen_round_poly(NULL, 0, pt, ptlen, m_closed, radius);
  if (Ok!=PathAddSize(path, count)) {
    return OutOfMemory;
  }
  path->Count = vcgen_round_poly(path->Points, old_count, pt, ptlen, m_closed, radius);
  if (path->Count>old_count) {
    memset(path->Types + old_count, PathPointTypeLine, path->Count - old_count);
    path->Types[old_count] = PathPointTypeStart;
    if (m_closed) {
      path->Types[path->Count-1] |= PathPointTypeCloseSubpath;
    }
  }
  return Ok;
}

static Status PathAddArcI(Path *path, int x1, int y1, int x2,
                   int y2, float startAngle, float sweepAngle)
{
  TRACE("(%p, %d, %d, %d, %d, %.2f, %.2f)\n",
    path, x1, y1, x2, y2, startAngle, sweepAngle);
  
  return PathAddArc(path,(float)x1,(float)y1,(float)x2,(float)y2,startAngle,sweepAngle);
}

static Status PathAddBezier(Path *path, float x1, float y1, float x2,
                     float y2, float x3, float y3, float x4, float y4)
{
  int old_count;
  
  TRACE("(%p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n",
    path, x1, y1, x2, y2, x3, y3, x4, y4);
  
  if(!path)
    return InvalidParameter;
  
  old_count = path->Count;
  
  if(Ok!=PathAddSize(path, 4))
    return OutOfMemory;
  
  path->Points[old_count].x = x1;
  path->Points[old_count].y = y1;
  path->Points[old_count + 1].x = x2;
  path->Points[old_count + 1].y = y2;
  path->Points[old_count + 2].x = x3;
  path->Points[old_count + 2].y = y3;
  path->Points[old_count + 3].x = x4;
  path->Points[old_count + 3].y = y4;
  path->Types[old_count] = (path->curlen ? PathPointTypeLine : PathPointTypeStart);
  path->Types[old_count + 1] = PathPointTypeBezier;
  path->Types[old_count + 2] = PathPointTypeBezier;
  path->Types[old_count + 3] = PathPointTypeBezier;
  path->Count = old_count + 4;
  path->curlen += 4;
  return Ok;
}
static Status PathAddBezierI(Path *path, int x1, int y1, int x2,
                      int y2, int x3, int y3, int x4, int y4)
{
  TRACE("(%p, %d, %d, %d, %d, %d, %d, %d, %d)\n",
    path, x1, y1, x2, y2, x3, y3, x4, y4);
  
  return PathAddBezier(path,(float)x1,(float)y1,(float)x2,(float)y2,(float)x3,(float)y3,
    (float)x4,(float)y4);
}
static Status PathAddBeziers(Path *path, const FPOINT *points, int count)
{
  int i, old_count;
  
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  if(!path || !points || ((count - 1) % 3))
    return InvalidParameter;
  
  old_count = path->Count;
  
  if(Ok!=PathAddSize(path, count))
    return OutOfMemory;
  
  for(i = 0; i < count; i++) {
    path->Points[old_count + i].x = points[i].x;
    path->Points[old_count + i].y = points[i].y;
    path->Types[old_count + i] = PathPointTypeBezier;
  }
  path->Count = old_count + count;
  path->Types[old_count] = (path->curlen ? PathPointTypeLine : PathPointTypeStart);
  path->curlen += count;
  return Ok;
}

static Status PointI2F(FPOINT** pptsF, const IPOINT* points, int count) {
  int i;
  FPOINT *ptsf = (FPOINT*)realloc(*pptsF, sizeof(FPOINT) * count);
  if(!ptsf)
    return OutOfMemory;
  
  for(i = 0; i < count; i++) {
    ptsf[i].x = (float)points[i].x;
    ptsf[i].y = (float)points[i].y;
  }
  *pptsF = ptsf;
  return Ok;
}
static Status PathAddBeziersI(Path *path, const IPOINT *points, int count)
{
  FPOINT *ptsF = NULL;
  Status ret;
  
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  if(!points || ((count - 1) % 3))
    return InvalidParameter;
  
  ret = PointI2F(&ptsF, points, count);
  if(!ptsF)
    return OutOfMemory;
  
  ret = PathAddBeziers(path, ptsF, count);
  free(ptsF);
  
  return ret;
}
/* Calculates Bezier points from cardinal spline points. */
void calc_curve_bezier(const FPOINT *pts, float tension, float *x1,
                       float *y1, float *x2, float *y2)
{
  float xdiff, ydiff;
  
  /* calculate tangent */
  xdiff = pts[2].x - pts[0].x;
  ydiff = pts[2].y - pts[0].y;
  
  /* apply tangent to get control points */
  *x1 = pts[1].x - tension * xdiff;
  *y1 = pts[1].y - tension * ydiff;
  *x2 = pts[1].x + tension * xdiff;
  *y2 = pts[1].y + tension * ydiff;
}

static Status PathAddClosedCurve2(Path *path, const FPOINT *points,
                           int count, float tension)
{
  int i, len_pt = (count + 1)*3-2;
  FPOINT *pt;
  FPOINT *pts;
  float x1, x2, y1, y2;
  Status status;
  
  TRACE("(%p, %p, %d, %.2f)\n", path, points, count, tension);
  
  if(!path || !points || count <= 1)
    return InvalidParameter;
  
  pt = (FPOINT*)malloc((len_pt + count + 1) * sizeof(FPOINT));
  pts = pt + len_pt;
  if(!pt) {
    free(pt);
    return OutOfMemory;
  }
  
  /* copy source points to extend with the last one */
  memcpy(pts, points, sizeof(FPOINT)*count);
  pts[count] = pts[0];
  
  tension = tension * TENSION_CONST;
  
  for(i = 0; i < count-1; i++) {
    calc_curve_bezier(&(pts[i]), tension, &x1, &y1, &x2, &y2);
    
    pt[3*i+2].x = x1;
    pt[3*i+2].y = y1;
    pt[3*i+3].x = pts[i+1].x;
    pt[3*i+3].y = pts[i+1].y;
    pt[3*i+4].x = x2;
    pt[3*i+4].y = y2;
  }
  
  /* points [len_pt-2] and [0] are calculated
  separately to connect splines properly */
  pts[0] = points[count-1];
  pts[1] = points[0]; /* equals to start and end of a resulting path */
  pts[2] = points[1];
  
  calc_curve_bezier(pts, tension, &x1, &y1, &x2, &y2);
  pt[len_pt-2].x = x1;
  pt[len_pt-2].y = y1;
  pt[0].x = pts[1].x;
  pt[0].y = pts[1].y;
  pt[1].x = x2;
  pt[1].y = y2;
  /* close path */
  pt[len_pt-1].x = pt[0].x;
  pt[len_pt-1].y = pt[0].y;
  
  status = PathAddBeziers(path, pt, len_pt);
  
  /* close figure */
  if(status == Ok) {
    path->Types[path->Count - 1] |= PathPointTypeCloseSubpath;
    path->curlen = 0;
  }
  
  free(pt);
  
  return status;
}

static Status PathAddClosedCurve2I(Path *path, const IPOINT *points,
                            int count, float tension)
{
  FPOINT *ptf = NULL;
  Status status;
  
  TRACE("(%p, %p, %d, %.2f)\n", path, points, count, tension);
  
  if(!path || !points || count <= 1)
    return InvalidParameter;
  
  status = PointI2F(&ptf, points, count);
  if(!ptf)
    return OutOfMemory;
  
  status = PathAddClosedCurve2(path, ptf, count, tension);
  
  free(ptf);
  
  return status;
}

static Status PathAddClosedCurve(Path *path, const FPOINT *points,
                          int count)
{
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  return PathAddClosedCurve2(path, points, count, 1.0);
}

static Status PathAddClosedCurveI(Path *path, const IPOINT *points,
                           int count)
{
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  return PathAddClosedCurve2I(path, points, count, 1.0);
}

/* Calculates Bezier points from cardinal spline endpoints. */
void calc_curve_bezier_endp(float xend, float yend, float xadj, float yadj,
                            float tension, float *x, float *y)
{
  /* tangent at endpoints is the line from the endpoint to the adjacent point */
  *x = gdip_round(tension * (xadj - xend) + xend);
  *y = gdip_round(tension * (yadj - yend) + yend);
}

static Status PathAddCurve2(Path *path, const FPOINT *points, int count,
                     float tension)
{
  int i, len_pt = count*3-2;
  FPOINT *pt;
  float x1, x2, y1, y2;
  Status status;
  
  TRACE("(%p, %p, %d, %.2f)\n", path, points, count, tension);
  
  if(!path || !points || count <= 1)
    return InvalidParameter;
  
  pt = (FPOINT*)malloc(sizeof(FPOINT) * len_pt);
  if(!pt)
    return OutOfMemory;
  
  tension = tension * TENSION_CONST;
  
  calc_curve_bezier_endp(points[0].x, points[0].y, points[1].x, points[1].y,
    tension, &x1, &y1);
  
  pt[0].x = points[0].x;
  pt[0].y = points[0].y;
  pt[1].x = x1;
  pt[1].y = y1;
  
  for(i = 0; i < count-2; i++) {
    calc_curve_bezier(&(points[i]), tension, &x1, &y1, &x2, &y2);
    
    pt[3*i+2].x = x1;
    pt[3*i+2].y = y1;
    pt[3*i+3].x = points[i+1].x;
    pt[3*i+3].y = points[i+1].y;
    pt[3*i+4].x = x2;
    pt[3*i+4].y = y2;
  }
  
  calc_curve_bezier_endp(points[count-1].x, points[count-1].y,
    points[count-2].x, points[count-2].y, tension, &x1, &y1);
  
  pt[len_pt-2].x = x1;
  pt[len_pt-2].y = y1;
  pt[len_pt-1].x = points[count-1].x;
  pt[len_pt-1].y = points[count-1].y;
  
  status = PathAddBeziers(path, pt, len_pt);
  
  free(pt);
  
  return status;
}

static Status PathAddCurve(Path *path, const FPOINT *points, int count)
{
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  if(!path || !points || count <= 1)
    return InvalidParameter;
  
  return PathAddCurve2(path, points, count, 1.0);
}

static Status PathAddCurve2I(Path *path, const IPOINT *points,
                      int count, float tension)
{
  FPOINT *ptf;
  int i;
  Status status;
  
  TRACE("(%p, %p, %d, %.2f)\n", path, points, count, tension);
  
  if(!path || !points || count <= 1)
    return InvalidParameter;
  
  ptf = (FPOINT*)malloc(sizeof(FPOINT)*count);
  if(!ptf)
    return OutOfMemory;
  
  for(i = 0; i < count; i++) {
    ptf[i].x = (float)points[i].x;
    ptf[i].y = (float)points[i].y;
  }
  
  status = PathAddCurve2(path, ptf, count, tension);
  
  free(ptf);
  
  return status;
}

static Status PathAddCurveI(Path *path, const IPOINT *points, int count)
{
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  if(!path || !points || count <= 1)
    return InvalidParameter;
  
  return PathAddCurve2I(path, points, count, 1.0);
}

static Status PathAddCurve3(Path *path, const FPOINT *points,
                     int count, int offset, int nseg, float tension)
{
  TRACE("(%p, %p, %d, %d, %d, %.2f)\n", path, points, count, offset, nseg, tension);
  
  if(!path || !points || offset + 1 >= count || count - offset < nseg + 1)
    return InvalidParameter;
  
  return PathAddCurve2(path, &points[offset], nseg + 1, tension);
}

static Status PathAddCurve3I(Path *path, const IPOINT *points,
                      int count, int offset, int nseg, float tension)
{
  TRACE("(%p, %p, %d, %d, %d, %.2f)\n", path, points, count, offset, nseg, tension);
  
  if(!path || !points || offset + 1 >= count || count - offset < nseg + 1)
    return InvalidParameter;
  
  return PathAddCurve2I(path, &points[offset], nseg + 1, tension);
}

static Status PathAddEllipse(Path *path, float x, float y, float width, float height)
{
  int old_count, numpts;
  
  TRACE("(%p, %.2f, %.2f, %.2f, %.2f)\n", path, x, y, width, height);
  
  if(!path)
    return InvalidParameter;
  
  old_count = path->Count;
  
  if(Ok!=PathAddSize(path, MAX_ARC_PTS))
    return OutOfMemory;
  
  if((numpts = arc2polybezier(&path->Points[old_count], &path->Types[old_count], x, y, width,
    height, 0.0, 360.0)) != MAX_ARC_PTS) {
    ERR("expected %d points but got %d\n", MAX_ARC_PTS, numpts);
    return GenericError;
  }
  
  /* An ellipse is an intrinsic figure (always is its own subpath). */
  path->Types[old_count] = PathPointTypeStart;
  path->Types[old_count + MAX_ARC_PTS - 1] |= PathPointTypeCloseSubpath;
  path->Count = old_count + MAX_ARC_PTS;
  path->curlen = 0;
  return Ok;
}

static Status PathAddEllipseI(Path *path, int x, int y, int width,
                       int height)
{
  TRACE("(%p, %d, %d, %d, %d)\n", path, x, y, width, height);
  
  return PathAddEllipse(path,(float)x,(float)y,(float)width,(float)height);
}

static Status PathAddLine2(Path *path, const FPOINT *points,
                    int count)
{
  int i, old_count;
  
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  if(!path || !points)
    return InvalidParameter;
  
  old_count = path->Count;
  if(!PathAddSize(path, count))
    return OutOfMemory;
  
  
  for(i = 0; i < count; i++) {
    path->Points[old_count + i].x = points[i].x;
    path->Points[old_count + i].y = points[i].y;
    path->Types[old_count + i] = PathPointTypeLine;
  }
  if(!path->curlen) {
    path->Types[old_count] = PathPointTypeStart;
  }
  path->curlen += count;
  path->Count = old_count + count;
  return Ok;
}

static Status PathAddLine2I(Path *path, const IPOINT *points, int count)
{
  FPOINT *pointsF;
  int i;
  Status status;
  
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  if(count <= 0)
    return InvalidParameter;
  
  pointsF = (FPOINT*)malloc(sizeof(FPOINT) * count);
  if(!pointsF)    return OutOfMemory;
  
  for(i = 0;i < count; i++) {
    pointsF[i].x = (float)points[i].x;
    pointsF[i].y = (float)points[i].y;
  }
  
  status = PathAddLine2(path, pointsF, count);
  
  free(pointsF);
  
  return status;
}

static Status PathAddLine(Path *path, float x1, float y1, float x2, float y2)
{
  int old_count;
  
  TRACE("(%p, %.2f, %.2f, %.2f, %.2f)\n", path, x1, y1, x2, y2);
  
  if(!path)
    return InvalidParameter;
  
  old_count = path->Count;
  if(Ok!=PathAddSize(path, 2))
    return OutOfMemory;
  
  path->Points[old_count].x = x1;
  path->Points[old_count].y = y1;
  path->Points[old_count + 1].x = x2;
  path->Points[old_count + 1].y = y2;
  
  path->Types[old_count] =
    (path->curlen ? PathPointTypeLine : PathPointTypeStart);
  path->Types[old_count + 1] = PathPointTypeLine;
  path->Count = old_count + 2;
  path->curlen += 2;
  return Ok;
}

static Status PathAddLineI(Path *path, int x1, int y1, int x2, int y2)
{
  TRACE("(%p, %d, %d, %d, %d)\n", path, x1, y1, x2, y2);
  return PathAddLine(path, (float)x1, (float)y1, (float)x2, (float)y2);
}

static Status PathCloseFigures(Path* path)
{
  int i;
  
  TRACE("(%p)\n", path);
  
  if(!path)
    return InvalidParameter;
  
  for(i = 1; i < path->Count; i++) {
    if(path->Types[i] == PathPointTypeStart) {
      path->Types[i-1] |= PathPointTypeCloseSubpath;
    }
  }
  
  path->curlen = 0;
  
  return Ok;
}

static Status PathAddPie(Path *path, float x, float y, float width, float height,
                  float startAngle, float sweepAngle)
{
  Status status;
  int count, old_count;
  
  TRACE("(%p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n",
    path, x, y, width, height, startAngle, sweepAngle);
  
  if(!path)
    return InvalidParameter;
  
  old_count = path->Count;
  /* on zero width/height only start point added */
  if(width <= 1e-7 || height <= 1e-7) {
    if(Ok!=PathAddSize(path, 1)) {
      return OutOfMemory;
    }
    path->Points[old_count].x = x + width  / 2.0;
    path->Points[old_count].y = y + height / 2.0;
    path->Types[old_count] = PathPointTypeStart | PathPointTypeCloseSubpath;
    path->Count = old_count + 1;
    return InvalidParameter;
  }
  
  count = arc2polybezier(NULL, NULL, x, y, width, height, startAngle, sweepAngle);
  
  if(count == 0)
    return Ok;
  
  /* one spline is already added as a line endpoint */
  if(Ok!=PathAddSize(path, count - 1)) {
    return OutOfMemory;
  }
  arc2polybezier(path->Points + path->Count, path->Types + path->Count, x, y, width, height, startAngle, sweepAngle);
  
  status = PathAddLine(path, x + width/2, y + height/2, path->Points[path->Count].x, path->Points[path->Count].y);
  if(status != Ok) {
    return status;
  }
  
  PathCloseFigure(path);
  return status;
}

static Status PathAddPieI(Path *path, int x, int y, int width, int height,
                   float startAngle, float sweepAngle)
{
  TRACE("(%p, %d, %d, %d, %d, %.2f, %.2f)\n",
    path, x, y, width, height, startAngle, sweepAngle);
  
  return PathAddPie(path, (float)x, (float)y, (float)width, (float)height, startAngle, sweepAngle);
}

CC_INLINE void arr2fpoint(FPOINT* pt, const void* ps, TypeId type, int step, int count) {
  int i;
  const char* p = (const char*)ps;
  switch (type) {
#define GCTYPEDEF(NAME, b, c, TYPE) case NAME: { ASSERT(step>=(int)sizeof(TYPE)); for (i=0; i<count; ++i, p+=step) {pt[i].x = ((const TYPE*)p)[0], pt[i].y = ((const TYPE*)p)[1];} } break;
    TYPEDEF_DEF(GCTYPEDEF)
#undef GCTYPEDEF
  default:
    ASSERT(0);
    break;
  }
}
static Status PathAddPolygonX(Path *path, const void* pt, const int* lens, int n, TypeId type, int step) {
  int i, count = 0, old_count, pos;
  
  TRACE("(%p, %p, %p, %d)\n", path, pt, lens, n);
  
  if(!path || !pt)
    return InvalidParameter;
  
  for (i=0; i<n; ++i) {
    count += abs(lens[i]);
  }
  if (0==count) {
    return Ok;
  }
  old_count = path->Count;
  if(Ok!=PathAddSize(path, count))
    return OutOfMemory;
  
  arr2fpoint(path->Points + old_count, pt, type, step, count);
  memset(&path->Types[old_count], PathPointTypeLine, count);
  
  /* A polygon is an intrinsic figure */
  pos = old_count;
  for (i=0; i<n; ++i) {
    if (lens[i]) {
      int len = abs(lens[i]);
      path->Types[pos] = PathPointTypeStart;
      if (lens[i]>0) {
        path->Types[pos + len - 1] |= PathPointTypeCloseSubpath;
      }
      pos += len;
    }
  }
  path->Count = old_count + count;
  path->curlen = 0;
  return Ok;
}
static Status PathAddLines(Path *path, const FPOINT *points, int count)
{
  count = -abs(count);
  return PathAddPolygonX(path, points, &count, 1, CC_FLOAT, sizeof(*points));
}
static Status PathAddPolygon(Path *path, const FPOINT *points, int count, bool closed)
{
  if (!closed) {
    count = -abs(count);
  }
  return PathAddPolygonX(path, points, &count, 1, CC_FLOAT, sizeof(*points));
}

static Status PathAddPolygonI(Path *path, const IPOINT *points, int count, bool closed)
{
  if (!closed) {
    count = -abs(count);
  }
  return PathAddPolygonX(path, points, &count, 1, CC_INT32, sizeof(*points));
}

static Status PathCreate(FillMode fill, Path *path)
{
  TRACE("(%d, %p)\n", fill, path);
  
  if(!path)
    return InvalidParameter;
  
  (path)->fill = fill;
  (path)->curlen = 0;
  
  return Ok;
}

static Status PathCreate2(const FPOINT* points,
                   const BYTE* types, int count, FillMode fill, Path *path)
{
  TRACE("(%p, %p, %d, %d, %p)\n", points, types, count, fill, path);
  
  if(!path)
    return InvalidParameter;
  
  if(!path)  return OutOfMemory;
  
  if(Ok!=PathSetSize(path, count)) {
    return OutOfMemory;
  }
  
  memcpy((path)->Points, points, count * sizeof(FPOINT));
  memcpy((path)->Types, types, count);
  (path)->Count = count;
  (path)->fill = fill;
  (path)->curlen = 0;
  
  return Ok;
}

static Status PathCreatePath2I(const IPOINT* points,
                        const BYTE* types, int count, FillMode fill, Path *path)
{
  FPOINT *ptF = NULL;
  Status ret;
  
  TRACE("(%p, %p, %d, %d, %p)\n", points, types, count, fill, path);
  
  ret = PointI2F(&ptF, points, count);
  ret = PathCreate2(ptF, types, count, fill, path);
  free(ptF);
  return ret;
}
static Status PathTranslate(Path *path, float offsetX, float offsetY) {
  int i;
  for (i=0; i<path->Count; ++i) {
    path->Points[i].x += offsetX;
    path->Points[i].y += offsetY;
  }
  return Ok;
}
static Status PathScale(Path *path, float scaleX, float scaleY) {
  int i;
  for (i=0; i<path->Count; ++i) {
    path->Points[i].x *= scaleX;
    path->Points[i].y *= scaleY;
  }
  return Ok;
}
static Status PathTransform(Path *path, const float *matrix)
{
  TRACE("(%p, %p)\n", path, matrix);
  
  if(!path)
    return InvalidParameter;
  
  if(path->Count == 0)
    return Ok;
  
  return M23TransformPoints(matrix, path->Points, path->Count);
}

static Status PathCloneImpl(const FPOINT* Points, const BYTE* Types, int Count, Path* clone)
{
  TRACE("(%p, %p)\n", Points, clone);
  
  if(!Points || !clone)
    return InvalidParameter;

  if (clone->Points==Points && clone->Types==Types && clone->Count==Count) {
    return Ok;
  }
  
  if(Ok!=PathSetSize(clone, Count)) {
    return OutOfMemory;
  }
  
  memcpy(clone->Points, Points, Count * sizeof(FPOINT));
  memcpy(clone->Types, Types, Count);
  clone->Count = Count;
  return Ok;
}

static Status PathClone(const Path* path, Path* clone)
{
  if (clone==path) {
    return Ok;
  }
  return PathCloneImpl(path->Points, path->Types, path->Count, clone);
}

static bool PathIsFlattenImpl(const BYTE* Types, int Count) {
  int i;
  for (i=0; i<Count; ++i) {
    int t = Types[i]&PathPointTypePathTypeMask;
    if (PathPointTypeStart!=t && PathPointTypeLine!=t) {
      return false;
    }
  }
  return true;
}
static bool PathPrint(const Path *path) {
  int i;
  for (i=0; i<path->Count; ++i) {
    printf("%d: %d %.1f %.1f\n", i, path->Types[i], path->Points[i].x, path->Points[i].y);
  }
  return 0;
}
static bool PathIsFlatten(const Path *path) {
  return PathIsFlattenImpl(path->Types, path->Count);
}
static Status PathFlatten(const Path *path, double flatness, Path *path2)
{
  Status status;
  int n;
  if (PathIsFlatten(path)) {
    if (path!=path2) {
      PathClone(path, path2);
    }
    return Ok;
  } else {
    PATH_COPY_BEGIN(path, path2);
    n = vcgen_flatten(NULL, NULL, 0, path->Points, path->Types, path->Count, flatness);
    status = PathSetSize(path2, n);
    if (Ok==status) {
      path2->Count = vcgen_flatten(path2->Points, path2->Types, 0, path->Points, path->Types, path->Count, flatness);
    }
    PATH_COPY_END();
    return status;
  }
  return status;
}
static Status PathFlatten2(const FPOINT* Points, const BYTE* Types, int Count, double flatness, Path *path2) {
  Path path[1] = {0};
  path->Points = (FPOINT*)Points;
  path->Types = (BYTE*)Types;
  path->Count = Count;
  return PathFlatten(path, flatness, path2);
}

/* Windows expands the bounding box to the maximum possible bounding box
* for a given pen.  For example, if a line join can extend past the point
* it's joining by x units, the bounding box is extended by x units in every
* direction (even though this is too conservative for most cases). */
static Status PathGetBounds(const Path* path, FRECT* bounds, const PenStyle* pen, const float *matrix)
{
  FPOINT * points, temp_pts[4];
  int count, i;
  float path_width = 1.0, low_x, low_y, high_x, high_y;
  
  TRACE("(%p, %p, %p, %p)\n", path, bounds, pen, matrix);
  
  /* float and pen can be null. */
  if(!path || !bounds)
    return InvalidParameter;
  
  /* If path is empty just return. */
  count = path->Count;
  if(count == 0) {
    bounds->l = bounds->t = bounds->r = bounds->b = 0.0;
    return Ok;
  }
  
  points = path->Points;
  
  low_x = high_x = points[0].x;
  low_y = high_y = points[0].y;
  
  for(i = 1; i < count; i++) {
    float x = points[i].x, y = points[i].y;
    low_x  = MIN(low_x,  x);
    low_y  = MIN(low_y,  y);
    high_x = MAX(high_x, x);
    high_y = MAX(high_y, y);
  }
  
  /* This looks unusual but it's the only way I can imitate windows. */
  if (matrix) {
    temp_pts[0].x = low_x;
    temp_pts[0].y = low_y;
    temp_pts[1].x = low_x;
    temp_pts[1].y = high_y;
    temp_pts[2].x = high_x;
    temp_pts[2].y = high_y;
    temp_pts[3].x = high_x;
    temp_pts[3].y = low_y;
    
    M23TransformPoints(matrix, temp_pts, 4);
    high_x = low_x = temp_pts[0].x;
    high_y = low_y = temp_pts[0].y;
    
    for(i = 1; i < 4; i++) {
      float x = temp_pts[i].x, y = temp_pts[i].y;
      low_x = MIN(low_x, x);
      low_y = MIN(low_y, y);
      high_x = MAX(high_x, x);
      high_y = MAX(high_y, y);
    }
  }
  
  if(pen) {
    path_width = pen->lineWidth / 2.0;
    
    if(count > 2)
      path_width = MAX(path_width,  pen->lineWidth * pen->miterLimit / 2.0);
    /* FIXME: this should probably also check for the startCap */
    if(pen->endCap & LineCapNoAnchor)
      path_width = MAX(path_width,  pen->lineWidth * 2.2);
    
    low_x -= path_width;
    low_y -= path_width;
    high_x += path_width;
    high_y += path_width;
  }
  
  bounds->l = low_x;
  bounds->t = low_y;
  bounds->r = high_x;
  bounds->b = high_y;
  
  return Ok;
}

static Status PathGetBoundsI(const Path* path, IRECT* bounds, const PenStyle* pen, const float *matrix)
{
  Status ret;
  FRECT boundsF;
  
  TRACE("(%p, %p, %p, %p)\n", path, bounds, pen, matrix);
  
  ret = PathGetBounds(path,&boundsF,pen, matrix);
  
  if(ret == Ok) {
    bounds->l      = gdip_round(boundsF.l);
    bounds->t      = gdip_round(boundsF.t);
    bounds->r      = gdip_round(boundsF.r);
    bounds->b      = gdip_round(boundsF.b);
  }
  return ret;
}

static Status PathReversePath(Path* path)
{
  int i, j, count;
  int start = 0; /* position in reversed path */
  Path revpath[1] = {0};
  
  TRACE("(%p)\n", path);
  
  if(!path)
    return InvalidParameter;
  
  count = path->Count;
  
  if(count == 0) return Ok;
  
  if(!PathSetSize(revpath, count)) {
    return OutOfMemory;
  }
  
  for(i = 0; i < count; i++) {
    /* find next start point */
    if(path->Types[count-i-1] == PathPointTypeStart) {
      for(j = start; j <= i; j++) {
        revpath->Points[j] = path->Points[count-j-1];
        revpath->Types[j] = path->Types[count-j-1];
      }
      /* mark start point */
      revpath->Types[start] = PathPointTypeStart;
      /* set 'figure' endpoint type */
      if(i-start > 1) {
        revpath->Types[i] = path->Types[count-start-1] & ~PathPointTypePathTypeMask;
        revpath->Types[i] |= revpath->Types[i-1];
      }
      else {
        revpath->Types[i] = path->Types[start];
      }
      
      start = i+1;
    }
  }
  revpath->Count = count;
  memcpy(path->Points, revpath->Points, sizeof(FPOINT)*count);
  memcpy(path->Types,  revpath->Types,  sizeof(BYTE)*count);
  PathFree(revpath);
  return Ok;
}

/*****************************************************************************
* GdipIsVisiblePathPoint [GDIPLUS.@]
*/
static Status PathIsVisiblePoint(Path* path, float x, float y, BOOL *result)
{
#if 0
  Region *region;
  HRGN hrgn;
  Status status;
  
  if(!path || !result) return InvalidParameter;
  
  status = GdipCreateRegionPath(path, &region);
  if(status != Ok)
    return status;
  
  status = GdipGetRegionHRgn(region, graphics, &hrgn);
  if(status != Ok) {
    GdipDeleteRegion(region);
    return status;
  }
  
  *result = PtInRegion(hrgn, gdip_round(x), gdip_round(y));
  
  DeleteObject(hrgn);
  GdipDeleteRegion(region);
  
#endif
  ASSERT(0);
  return Ok;
}

static Status PathStartPathFigure(Path *path)
{
  TRACE("(%p)\n", path);
  
  if(!path)
    return InvalidParameter;
  
  path->curlen = 0;
  return Ok;
}

static Status PathReset(Path *path)
{
  TRACE("(%p)\n", path);
  
  if(!path)
    return InvalidParameter;
  
  path->Count = 0;
  path->curlen = 0;
  path->fill = FillModeAlternate;
  return Ok;
}

static Status PathSetPathFillMode(Path *path, FillMode fill)
{
  TRACE("(%p, %d)\n", path, fill);
  
  if(!path)
    return InvalidParameter;
  
  path->fill = fill;
  return Ok;
}

static Status PathAddLineSegments(Path* path, const FPOINT* Points, int Count) {
  int i, old_count;
  TRACE("(%p, %p, %d)\n", path, Points, Count);
  
  /* count == 0 - verified condition  */
  if(!path || !Points || Count == 0)
    return InvalidParameter;
  
  /* make a backup copy */
  old_count = path->Count;
  if (Ok!=PathAddSize(path, Count)) {
    return OutOfMemory;
  }
  
  MEMCPY(path->Points + old_count, Points, Count);
  for(i = 0; i < Count; i++) {
    path->Types[old_count + i] = i&1 ? PathPointTypeLine : PathPointTypeStart;
  }
  path->Count = old_count + Count;
  path->curlen = Count&1;
  return Ok;
}
#define PathAddRectangle2(path, rc)  PathAddRectangle(path, rc.l, rc.t, RCW(&rc), RCH(&rc))
#define PathAddRect PathAddRectangle
static Status PathAddRectangle(Path *path, float x, float y, float width, float height)
{
  int old_count;
  BYTE Types[4] = {PathPointTypeStart, PathPointTypeLine, PathPointTypeLine, PathPointTypeLine|PathPointTypeCloseSubpath};
  
  TRACE("(%p, %.2f, %.2f, %.2f, %.2f)\n", path, x, y, width, height);
  
  if(!path)
    return InvalidParameter;
  
  /* rectangle should start as new path */
  old_count = path->Count;
  if (Ok!=PathAddSize(path, 4)) {
    return OutOfMemory;
  }
  path->Count = vcgen_rect(path->Points, old_count, x, y, x+width, y+height);
  MEMCPY(path->Types + old_count, Types, 4);
  path->curlen = 0;
  return Ok;
}

static Status PathAddRectangleI(Path *path, int x, int y, int width, int height)
{
  TRACE("(%p, %d, %d, %d, %d)\n", path, x, y, width, height);
  
  return PathAddRectangle(path,(float)x,(float)y,(float)width,(float)height);
}
static Status PathAddRectangles(Path *path, const FRECT *rects, int count)
{
  int i, old_count;
  BYTE Types[4] = {PathPointTypeStart, PathPointTypeLine, PathPointTypeLine, PathPointTypeLine|PathPointTypeCloseSubpath};
  
  TRACE("(%p, %p, %d)\n", path, rects, count);
  
  /* count == 0 - verified condition  */
  if(!path || !rects || count == 0)
    return InvalidParameter;
  
  if(count < 0)
    return OutOfMemory;
  
  /* make a backup copy */
  old_count = path->Count;
  if (Ok!=PathAddSize(path, 4*count)) {
    return OutOfMemory;
  }
  
  for(i = 0; i < count; i++) {
    const FRECT* r = rects + i;
    MEMCPY(path->Types + old_count, Types, 4);
    old_count = vcgen_rect(path->Points, old_count, r->l, r->t, r->r, r->b);
  }
  path->Count = old_count;
  path->curlen = 0;
  return Ok;
}

static Status PathAddRectanglesI(Path *path, const IRECT *rects, int count)
{
  FRECT *rectsF = NULL;
  Status status;
  int i;
  
  TRACE("(%p, %p, %d)\n", path, rects, count);
  
  if(!rects || count == 0)
    return InvalidParameter;
  
  if(count < 0)
    return OutOfMemory;
  
  rectsF = (FRECT*)malloc(sizeof(FRECT)*count);
  
  for(i = 0;i < count;i++) {
    rectsF[i].l = (float)rects[i].l;
    rectsF[i].t = (float)rects[i].t;
    rectsF[i].r = (float)rects[i].r;
    rectsF[i].b = (float)rects[i].b;
  }
  
  status = PathAddRectangles(path, rectsF, count);
  free(rectsF);
  
  return status;
}

static Status PathSetPathMarker(Path* path)
{
  int count;
  
  TRACE("(%p)\n", path);
  
  if(!path)
    return InvalidParameter;
  
  count = path->Count;
  
  /* set marker flag */
  if(count > 0)
    path->Types[count-1] |= PathPointTypePathMarker;
  
  return Ok;
}

static Status PathClearPathMarkers(Path* path)
{
  int count;
  int i;
  
  TRACE("(%p)\n", path);
  
  if(!path)
    return InvalidParameter;
  
  count = path->Count;
  
  for(i = 0; i < count - 1; i++) {
    path->Types[i] &= ~PathPointTypePathMarker;
  }
  
  return Ok;
}

static Status PathWindingModeOutline(Path *path, float *matrix, float flatness)
{
  FIXME("stub: %p, %p, %.2f\n", path, matrix, flatness);
  return NotImplemented;
}

static float fromfixedpoint(const fixed_t v)
{
  float f = ((float)v.fract) / (1<<(sizeof(v.fract)*8));
  f += v.value;
  return f;
}

static int ttffont_get_glyph_pathW(const ttffont_t* f, uint code, const float* matrix, Path* path) {
  char obuf0[1<<12];
  int i, olen = countof(obuf0);
  int len = ttffont_get_glyph(f, code, obuf0, olen);
  const char* obuf = obuf0;
  const char* obuf1 = obuf+len;
  int old_count = path->Count;
  int ipos = old_count, Count = len/sizeof(pointi8);
  
  PathSetSize(path, old_count + Count);
  while(obuf < obuf1) {
    ttpolygon* oh = (ttpolygon*)obuf;
    const char* opoly = obuf + sizeof(ttpolygon);
    const char* epoly = obuf + oh->cb;
    //if(flip_y) y = -y;
    path->Points[ipos].x = oh->start.x;
    path->Points[ipos].y = oh->start.y;
    path->Types[ipos++] = PathPointTypeStart;
    //path.move_to(value_type(dbl_to_int26p6(x)), value_type(dbl_to_int26p6(y)));
    while(opoly < epoly) {
      ttpoly* op = (ttpoly*)opoly;
      PathPointType type = PathPointTypeLine;
      switch (op->type) {
      case TT_LINE:
        type = PathPointTypeLine;
        break;
      case TT_QSPLINE:
        type = PathPointTypeBezier2;
        break;
      case TT_CSPLINE:
        type = PathPointTypeBezier3;
        break;
      default: ASSERT(0); break;
      }
      for (i = 0; i < op->npt; i++) {
        path->Points[ipos].x = op->pt[i].x;
        path->Points[ipos].y = op->pt[i].y;
        path->Types[ipos] = (1<op->npt && i==op->npt-1) ? PathPointTypeLine : type;
        ++ipos;
      }
      opoly += sizeof(uchar) * 2 + sizeof(pointi8) * op->npt;
    }
    oh->cb = opoly - obuf;
    obuf = opoly;
    ASSERT(obuf<=obuf1);
    path->Count = ipos;
    PathCloseFigure(path);
  }
  path->Count = ipos;
  if (matrix) {
    M23TransformPoints(matrix, path->Points + old_count, path->Count-old_count);
  }
  return len;
}


#if 0

typedef struct {
  Path *path;
  float maxY;
  float scale;
  float ascent;
  float matrix[6];
} format_string_args;

static Status format_string_callback(const ttffont_t* f, const WCHAR *string, int index, int length,
                                     const FRECT *rect, const StringFormat *format,
                                     int lineno, const FRECT *bounds, int *underlined_indexes,
                                     int underlined_index_count, void *priv)
{
  format_string_args *args = (format_string_args*)priv;
  Path *path = args->path;
  Status status = Ok;
  float x = rect->l + (bounds->l - rect->l) * args->scale;
  float y = rect->t + (bounds->t - rect->t) * args->scale;
  int i;
  
  if (underlined_index_count) {
    FIXME("hotkey underlines not drawn yet\n");
  }
  
  if (y + RCH(bounds) * args->scale > args->maxY) {
    args->maxY = y + RCH(bounds) * args->scale;
  }
  //M23TransformPoints(matrix, path->Points, path->Count);
  for (i = index; i < length; ++i) {
    int len, ofs = path->Count;
    len = ttffont_get_glyph_path(f, string[i], path, NULL);
    //x += gm.gmCellIncX * args->scale;
    //y += gm.gmCellIncY * args->scale;
  }
  
  return status;
}

static Status gdip_format_string(const WCHAR *string, INT length, const ttffont_t* f, const FRECT *rect,
                          const StringFormat *format, gdip_format_string_callback callback, void *user_data)
{
  WCHAR* stringdup;
  int sum = 0, height = 0, fit, fitcpy, i, j, lret, nwidth, nheight, lineend, lineno = 0;
  FRECT bounds;
  StringAlignment halign;
  Status status = Ok;
  FSIZE size;
  HotkeyPrefix hkprefix;
  INT *hotkeyprefix_offsets=NULL;
  INT hotkeyprefix_count=0;
  INT hotkeyprefix_pos=0, hotkeyprefix_end_pos=0;
  int seen_prefix=0;
  
  if(length == -1) {
    length = wstr_len(string);
  }
  
  stringdup = (wchar_t*)malloc((length + 1) * sizeof(WCHAR));
  if(!stringdup) return OutOfMemory;
  
  nwidth = RCW(rect);
  nheight = RCH(rect);
  
  if (format)
    hkprefix = format->hkprefix;
  else
    hkprefix = HotkeyPrefixNone;
  
  if (hkprefix == HotkeyPrefixShow) {
    for (i=0; i<length; i++) {
      if (string[i] == '&')
        hotkeyprefix_count++;
    }
  }
  
  if (hotkeyprefix_count)
    hotkeyprefix_offsets = (int*)malloc(sizeof(INT) * hotkeyprefix_count);
  
  hotkeyprefix_count = 0;
  
  for(i = 0, j = 0; i < length; i++) {
    /* FIXME: This makes the indexes passed to callback inaccurate. */
    if(!isprintW(string[i]) && (string[i] != '\n'))
      continue;
    
    /* FIXME: tabs should be handled using tabstops from stringformat */
    if (string[i] == '\t')
      continue;
    
    if (seen_prefix && hkprefix == HotkeyPrefixShow && string[i] != '&') {
      hotkeyprefix_offsets[hotkeyprefix_count++] = j;
    } else if (!seen_prefix && hkprefix != HotkeyPrefixNone && string[i] == '&') {
      seen_prefix = 1;
      continue;
    }
    
    seen_prefix = 0;
    stringdup[j] = string[i];
    j++;
  }
  
  length = j;
  
  if (format) halign = format->align;
  else halign = StringAlignmentNear;
  
  while(sum < length) {
    GetTextExtentExPointW(hdc, stringdup + sum, length - sum, nwidth, &fit, NULL, &size);
    fitcpy = fit;
    
    if(fit == 0) {
      break;
    }
    
    for(lret = 0; lret < fit; lret++) {
      if(*(stringdup + sum + lret) == '\n') {
        break;
      }
    }
    
    /* Line break code (may look strange, but it imitates windows). */
    if(lret < fit) {
      lineend = fit = lret;    /* this is not an off-by-one error */
    } else if(fit < (length - sum)) {
      if (*(stringdup + sum + fit) == ' ') {
        while(*(stringdup + sum + fit) == ' ')
          fit++;
      } else {
        while(*(stringdup + sum + fit - 1) != ' ') {
          fit--;
          
          if(*(stringdup + sum + fit) == '\t') {
            break;
          }
          
          if(fit == 0) {
            fit = fitcpy;
            break;
          }
        }
      }
      lineend = fit;
      while(*(stringdup + sum + lineend - 1) == ' ' || *(stringdup + sum + lineend - 1) == '\t') {
        lineend--;
      }
    } else {
      lineend = fit;
    }
    
    GetTextExtentExPointW(hdc, stringdup + sum, lineend,
      nwidth, &j, NULL, &size);
    
    bounds.Width = size.cx;
    
    if(height + size.cy > nheight)
      bounds.Height = nheight - (height + size.cy);
    else
      bounds.Height = size.cy;
    
    bounds.Y = rect->Y + height;
    
    switch (halign)
    {
    case StringAlignmentNear:
    default:
      bounds.X = rect->X;
      break;
    case StringAlignmentCenter:
      bounds.X = rect->X + (rect->Width/2) - (bounds.Width/2);
      break;
    case StringAlignmentFar:
      bounds.X = rect->X + rect->Width - bounds.Width;
      break;
    }
    
    for (hotkeyprefix_end_pos=hotkeyprefix_pos; hotkeyprefix_end_pos<hotkeyprefix_count; hotkeyprefix_end_pos++) {
      if (hotkeyprefix_offsets[hotkeyprefix_end_pos] >= sum + lineend)
        break;
    }
    
    status = callback(stringdup, sum, lineend,
      font, rect, format, lineno, &bounds,
      &hotkeyprefix_offsets[hotkeyprefix_pos],
      hotkeyprefix_end_pos-hotkeyprefix_pos, user_data);
    
    if (status != Ok)
      break;
    
    sum += fit + (lret < fitcpy ? 1 : 0);
    height += size.cy;
    lineno++;
    
    hotkeyprefix_pos = hotkeyprefix_end_pos;
    
    if(height > nheight) {
      break;
    }
    
    /* Stop if this was a linewrap (but not if it was a linebreak). */
    if ((lret == fitcpy) && format &&
      (format->attr & (StringFormatFlagsNoWrap | StringFormatFlagsLineLimit))) {
      break;
    }
  }
  
  free(stringdup);
  free(hotkeyprefix_offsets);
  return status;
}

static Status PathAddString(Path* path, const WCHAR* string, int length, const ttffont_t* f, int style, float scale, const FRECT* layoutRect, const StringFormat* format)
{
  Status status;
  int old_count = path->Count;
  format_string_args args[1] = {0};
  int i;
  UINT16 native_height;
  FRECT scaled_layout_rect[1];
  
  TRACE("(%p, %s, %d, %p, %d, %f, %p, %p)\n", path, debugstr_w(string), length, f, style, layoutRect, format);
  if (!path || !string || !f || !layoutRect || !format)
    return InvalidParameter;
  
  scaled_layout_rect->l = layoutRect->l * scale;
  scaled_layout_rect->t = layoutRect->t * scale;
  scaled_layout_rect->r = layoutRect->r * scale;
  scaled_layout_rect->b = layoutRect->b * scale;
  
  args->path = path;
  args->maxY = 0;
  args->scale = scale;
  //args.ascent = textmetric.tmAscent * args.scale;
  status = gdip_format_string(string, length, NULL, scaled_layout_rect, format, format_string_callback, args);
  
  if (format && format->vertalign == StringAlignmentCenter && layoutRect->t + args.maxY < RCH(layoutRect))
  {
    float inc = RCH(layoutRect) + layoutRect->t - args->maxY;
    inc /= 2;
    for (i = old_count; i < path->Count; ++i) {
      path->Points[i].y += inc;
    }
  } else if (format && format->vertalign == StringAlignmentFar) {
    float inc = RCH(layoutRect) + layoutRect->t - args->maxY;
    for (i = old_count; i < path->Count; ++i) {
      path->Points[i].y += inc;
    }
  }
  return status;
}
static Status PathAddStringI(Path* path, const WCHAR* string, int length, const FontFamily* family, int style, float emSize, const IRECT* layoutRect, const StringFormat* format)
{
  if (layoutRect)
  {
    FRECT layoutRectF = {
      (float)layoutRect->x,
        (float)layoutRect->y,
        (float)layoutRect->Width,
        (float)layoutRect->Height
    };
    return PathAddString(path, string, length, family, style, emSize, &layoutRectF, format);
  }
  return InvalidParameter;
}

#endif


static char* nextcharA(unsigned int* pc, const char* text, const char* text_end) {
  if (text<text_end) {
    const uchar* s = (const uchar*)text;
    *pc = *s++;
    if (*pc >= 0x80) {
      *pc = ((*pc)<<8) | (*s++);
    }
    return (char*)s;
  }
  *pc = 0;
  return (char*)(text + 1);
}

static char* nextcharW(unsigned int* pc, const char* text, const char* text_end) {
  if (text<text_end) {
    *pc = *(wchar_t*)text;
    return (char*)(text+2);
  }
  *pc = 0;
  return (char*)(text+2);
}



typedef char* (*nextchar_f)(unsigned int* pc, const char* text, const char* text_end);

// Simple word-wrapping for English, not full-featured. Please submit failing cases!
// FIXME: Much possible improvements (don't cut things like "word !", "word!!!" but cut within "word,,,,", more sensible support for punctuations, support for Unicode punctuations, etc.)
// For references, possible wrap point marked with ^
// "aaa bbb, ccc,ddd. eee fff. ggg!"
// ^ ^ ^ ^ ^__ ^ ^
// List of hardcoded separators: .,;!?'"
// Skip extra blanks after a line returns (that includes not counting them in width computation)
// e.g. "Hello world" --> "Hello" "World"
// Cut words that cannot possibly fit within one line.
// e.g.: "The tropical fish" with ~5 characters worth of width --> "The tr" "opical" "fish"
const char* CalcWordWrapPosition(const FontInfo* ff, const char* text, const char* text_end, nextchar_f nextchar, float wrap_width, float scale)
{
  float line_width = 0.0f;
  float word_width = 0.0f;
  float blank_width = 0.0f;
  const char* word_end = text;
  const char* prev_word_end = NULL;
  bool inside_word = true;
  const char* s = text;
  float HalfXAdvance = ff->FallbackXAdvance / 2;
  while (s < text_end) {
    unsigned int c = 0;
    const char* next_s;
    float char_width;
    next_s = nextchar(&c, s, text_end);
    if (c == 0) {
      break;
    }
    if (c < 32) {
      if (c == '\n') {
        line_width = word_width = blank_width = 0.0f;
        inside_word = true;
        s = next_s;
        continue;
      }
      if (c == '\r') {
        s = next_s;
        continue;
      }
    }
    if (ff->IndexXAdvance) {
      char_width = (int)c < ff->IndexXAdvance_Size ? ff->IndexXAdvance[c] : ff->FallbackXAdvance;
    }
    else {
      char_width = c < 128 ? HalfXAdvance : ff->FallbackXAdvance;
    }
    char_width *= scale;
    if (ImCharIsSpace(c)) {
      if (inside_word) {
        line_width += blank_width;
        blank_width = 0.0f;
      }
      blank_width += char_width;
      inside_word = false;
    }
    else {
      word_width += char_width;
      if (inside_word) {
        word_end = next_s;
      }
      else {
        prev_word_end = word_end;
        line_width += word_width + blank_width;
        word_width = blank_width = 0.0f;
      }
      // Allow wrapping after punctuation.
      inside_word = !(c == '.' || c == ',' || c == ';' || c == '!' || c == '?' || c == '\"');
    }
    // We ignore blank width at the end of the line (they can be skipped)
    if (line_width + word_width > wrap_width) {
      // Words that cannot possibly fit within an entire line will be cut anywhere.
      if (word_width < wrap_width) {
        s = prev_word_end ? prev_word_end : word_end;
      }
      break;
    }
    s = next_s;
  }
  return s;
}

FSIZE CalcTextSize(const FontInfo* ff, const char* text, const char* text_end, nextchar_f nextchar, const char** remaining, float max_width, float wrap_width, float line_height, float scale)
{
  FSIZE text_size = fSize(0, 0);
  float char_width, line_width = 0.0f;
  const bool word_wrap_enabled = (wrap_width > 0.0f);
  const char* word_wrap_eol = NULL;
  const char* s;
  const char* prev_s;
  unsigned int c;
  float HalfXAdvance = ff->FallbackXAdvance / 2;
  if (NULL == text) {
    text = "test";
  }
  s = text;
  while (s < text_end) {
    if (word_wrap_enabled) {
      // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
      if (!word_wrap_eol) {
        word_wrap_eol = CalcWordWrapPosition(ff, s, text_end, nextchar, wrap_width - line_width, scale);
        if (word_wrap_eol == s) { // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
          word_wrap_eol++; // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
        }
      }
      if (s >= word_wrap_eol) {
        if (text_size.x < line_width) {
          text_size.x = line_width;
        }
        text_size.y += line_height;
        line_width = 0.0f;
        word_wrap_eol = NULL;
        // Wrapping skips upcoming blanks
        while (s < text_end) {
          c = 0;
          s = nextchar(&c, s, text_end);
          if (ImCharIsSpace(c)) {
          }
          else if (c == '\n') {
            break;
          }
          else {
            break;
          }
        }
        continue;
      }
    }
    // Decode and advance source
    prev_s = s;
    c = 0;
    s = nextchar(&c, s, text_end);
    if (c == 0) {
      break;
    }
    if (c < 32) {
      if (c == '\n') {
        text_size.x = MAX(text_size.x, line_width);
        text_size.y += line_height;
        line_width = 0.0f;
        continue;
      }
      if (c == '\r') {
        continue;
      }
    }
    if (ff->IndexXAdvance) {
      char_width = (int)c < ff->IndexXAdvance_Size ? ff->IndexXAdvance[c] : ff->FallbackXAdvance;
    }
    else {
      char_width = c < 128 ? HalfXAdvance : ff->FallbackXAdvance;
    }
    char_width *= scale;
    if (line_width + char_width >= (max_width)) {
      s = prev_s;
      break;
    }
    line_width += char_width;
  }
  if (text_size.x < line_width) {
    text_size.x = line_width;
  }
  if (line_width > 0 || text_size.y == 0.0f) {
    text_size.y += line_height;
  }
  if (remaining) {
    *remaining = s;
  }
  return text_size;
}

int RanderStringFormat(const FontInfo* ff, const char* text, const char* text_end,
  nextchar_f nextchar, get_glyph_f get_glyph, void* userdata, FPOINT pos, const FRECT* pclip_rect,
  float wrap_width, float scale, float angle)
{
  float x, y;
  float line_height = ff->size * scale;
  float rotate[6] = { 0 };
  bool word_wrap_enabled = (wrap_width > 0.0f);
  const char* word_wrap_eol = NULL;
  const char* s;
  FRECT clip_rect = pclip_rect ? *pclip_rect : fRECT2(pos.x, pos.y, 9999, 9999);
  float HalfXAdvance = ff->FallbackXAdvance / 2;
  //StringAlignment halign = format ? format->align : StringAlignmentNear;
  if (!text_end) {
    text_end = text + strlen(text);
  }
  // Align to be pixel perfect
  pos.x = (float)(int)pos.x;
  pos.y = (float)(int)pos.y;
  x = pos.x;
  y = pos.y;
  if (y > clip_rect.b) {
    return 0;
  }
  M23Set(rotate);
  M23Rotate(rotate, angle, MatrixOrderPrepend);
  M23Scale(rotate, scale, scale, MatrixOrderPrepend);

  word_wrap_enabled = (wrap_width > 0.0f);
  // Skip non-visible lines
  s = text;
  if (!word_wrap_enabled && y + line_height < clip_rect.t) {
    while (s < text_end && *s != '\n') { // Fast-forward to next line
      s++;
    }
  }
  // Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
  while (s < text_end) {
    unsigned int c = 0;
    float char_width;
    if (word_wrap_enabled) {
      // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
      if (!word_wrap_eol) {
        word_wrap_eol = CalcWordWrapPosition(ff, s, text_end, nextchar, wrap_width - (x - pos.x), scale);
        if (word_wrap_eol == s) { // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
          word_wrap_eol++; // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
        }
      }
      if (s >= word_wrap_eol) {
        pos.x += line_height * rotate[2] / scale;
        pos.y += line_height * rotate[3] / scale;
        x = pos.x;
        y = pos.y;
        word_wrap_eol = NULL;
        // Wrapping skips upcoming blanks
        while (s < text_end) {
          c = 0;
          s = nextchar(&c, s, text_end);
          if (ImCharIsSpace(c)) {
          }
          else if (c == '\n') {
            break;
          }
          else {
            break;
          }
        }
        continue;
      }
    }
    // Decode and advance source
    c = 0;
    s = nextchar(&c, s, text_end);
    if (c == 0) {
      break;
    }
    if (c < 32) {
      if (c == '\n') {
        x = pos.x;
        y += line_height;
        if (y > clip_rect.b) {
          break;
        }
        if (!word_wrap_enabled && y + line_height < clip_rect.t) {
          while (s < text_end && *s != '\n') { // Fast-forward to next line
            s++;
          }
        }
        continue;
      }
      if (c == '\r') {
        continue;
      }
    }
    if (c != ' ' && c != '\t' && get_glyph) {
      rotate[4] = x;
      rotate[5] = y;
      get_glyph(c, rotate, userdata);
    }
    char_width = 0.0f;
    if (ff->IndexXAdvance) {
      char_width = (int)c < ff->IndexXAdvance_Size ? ff->IndexXAdvance[c] : ff->FallbackXAdvance;
    }
    else {
      char_width = c < 128 ? HalfXAdvance : ff->FallbackXAdvance;
    }
    x += char_width * rotate[0];
    y += char_width * rotate[1];
  }
  // Give back unused vertices
  return 0;
}



typedef struct {
  Path *path;
  const ttffont_t* font;
} path_get_glyph_args;


static int path_get_glyphW(uint code, const float* matrix, void* userdata) {
  path_get_glyph_args* arg = (path_get_glyph_args*)userdata;
  ttffont_get_glyph_pathW(arg->font, code, matrix, arg->path);
  return 0;
}

static Status PathAddString(Path* path, const WCHAR* string, int length, const ttffont_t* f, float wrap_width, float scale, float angle, float x, float y, const FRECT* layoutRect, const StringFormat* format)
{
  path_get_glyph_args arg[1] = {0};
  FPOINT pos = {x, y};
  arg->path = path;
  arg->font = f;
  length = length<0 ? wcslen(string) : length;
  RanderStringFormat(f->info, (char*)string, (char*)(string+length), nextcharW, path_get_glyphW, arg, pos, layoutRect, wrap_width, scale, angle);
  return Ok;
}

static Status PathAddStringA(Path* path, const char* string, int length, const ttffont_t* f, float wrap_width, float scale, float angle, float x, float y, const FRECT* layoutRect, const StringFormat* format)
{
  wchar_t* wstring = NULL;
  int wlength=0;
  length = length<0 ? strlen(string) : length;
  wstring = (wchar_t*)malloc((length+1)*2);
  wlength = iconv_mbstowcsz(wstring, string, length);
  PathAddString(path, wstring, wlength, f, wrap_width, scale, angle, x, y, layoutRect, format);
  free(wstring);
  return Ok;
}

static Status PathAddStringI(Path* path, const WCHAR* string, int length, const ttffont_t* f, float wrap_width, float scale, float angle, float x, float y, const IRECT* layoutRect, const StringFormat* format)
{
  FRECT* playoutRectF = NULL;
  FRECT layoutRectF;
  if (layoutRect) {
    layoutRectF = RECT2RECT(fRECT, float, layoutRect);
    playoutRectF = &layoutRectF;
  }
  return PathAddString(path, string, length, f, wrap_width, scale, angle, x, y, playoutRectF, format);
}
static int GetPathLens(const BYTE* Types, int Count, int* plen) {
  int j = 0;
  PATH_LOOP_BEGIN;
  if (plen) {
    plen[j] = end - pos;
  }
  ++j;
  PATH_LOOP_END;
  return j;
}
static int GetPathLens2(const BYTE* Types, int Count, int** pplen) {
  int nlen = GetPathLens(Types, Count, NULL);
  if (pplen) {
    MYREALLOC(*pplen, nlen);
    nlen = GetPathLens(Types, Count, *pplen);
  }
  return nlen;
}
static int PathGetMaxSubpathLen(const BYTE* Types, int Count) {
  int maxlen = 0;
  PATH_LOOP_BEGIN;
  maxlen = MAX(maxlen, end - pos);
  PATH_LOOP_END;
  return maxlen;
}
static int vcgen_dash2(FPOINT* Points1, BYTE* Types1, int ipos, const FPOINT* Points, const BYTE* Types, int Count, const float* m_dashes, int m_num_dashes, double m_dash_start) {
  PATH_LOOP_BEGIN;
  ipos = vcgen_dash1(Points1, Types1, ipos, Points + pos, end-pos, closed, m_dashes, m_num_dashes, m_dash_start);
  PATH_LOOP_END;
  return ipos;
}
static Status PathDash(const Path* path, float flatness, const float* m_dashes, int m_num_dashes, double m_dash_start, Path* path2) {
  Status status = Ok;
  int maxptlen;
  PATH_FLATCOPY_BEGIN;
  maxptlen = vcgen_dash2(NULL, NULL, 0, path->Points, path->Types, path->Count, m_dashes, m_num_dashes, m_dash_start);
  status = PathSetSize(path2, maxptlen);
  if (Ok==status) {
    path2->Count = vcgen_dash2(path2->Points, path2->Types, 0, path->Points, path->Types, path->Count, m_dashes, m_num_dashes, m_dash_start);
    ASSERT(path2->Count<=maxptlen);
  }
  PATH_FLATCOPY_END;
  return status;
}
static int vcgen_dash_stroke(FPOINT* Points, BYTE* Types, int ipos, int maxptlen, const FPOINT* pt, int ptlen, bool closed, const PenStyle* penstyle, const float* m_dashes, int m_num_dashes, double m_dash_start) {
  if (m_dashes && m_num_dashes>1) {
    FPOINT pt0[1000];
    int lens[100] = {0};
    int i, k = 0, n = vcgen_dash(pt, ptlen, closed, m_dashes, m_num_dashes, m_dash_start, countof(pt0), countof(lens), pt0, lens);
    
    for (i = 0; i < n; i++) {
      ipos = vcgen_stroke(Points, Types, ipos, NULL, pt0 + k, lens[i], 0, penstyle);
      k += lens[i];
    }
  } else {
    ipos = vcgen_stroke(Points, Types, ipos, NULL, pt, ptlen, closed, penstyle);
  }
  return ipos;
}
static int vcgen_dash_stroke2(FPOINT* Points2, BYTE* Types2, int ipos, int maxptlen, const FPOINT* Points, const BYTE* Types, int Count, float flatness, const PenStyle* penstyle, const float* m_dashes, int m_num_dashes, double m_dash_start) {
  PATH_LOOP_BEGIN;
  ipos = vcgen_dash_stroke(Points2, Types2, ipos, maxptlen, Points + pos, end-pos, closed, penstyle, m_dashes, m_num_dashes, m_dash_start);
  PATH_LOOP_END;
  return ipos;
}

static int vcgen_stroke2(FPOINT* Points2, BYTE* Types2, int ipos, const FPOINT* Points, const BYTE* Types, int Count, const PenStyle* penstyle) {
  PATH_LOOP_BEGIN;
  ipos = vcgen_stroke(Points2, Types2, ipos, NULL, Points + pos, end-pos, closed, penstyle);
  PATH_LOOP_END;
  return ipos;
}

static Status PathDashStroke(const Path* path, float flatness, const PenStyle* penstyle, const float* m_dashes, int m_num_dashes, double m_dash_start, Path* path2) {
  Status status = Ok;
  PATH_FLATCOPY_BEGIN;
  PenStyle penstyle1[1] = {0};
  if (NULL==penstyle) {
    penstyle = penstyle1;
    PenStyle_Init(penstyle1);
  }
  {
    int maxptlen = vcgen_dash_stroke2(NULL, NULL, 0, 0, path->Points, path->Types, path->Count, flatness, penstyle, m_dashes, m_num_dashes, m_dash_start);
    status = PathSetSize(path2, maxptlen);
    if (Ok==status) {
      path2->Count = vcgen_dash_stroke2(path2->Points, path2->Types, 0, maxptlen, path->Points, path->Types, path->Count, flatness, penstyle, m_dashes, m_num_dashes, m_dash_start);
    }
  }
  PATH_FLATCOPY_END;
  return status;
}

static Status PathStroke(const Path* path, float flatness, const PenStyle* penstyle, Path* path2) {
  int maxptlen;
  PATH_FLATCOPY_BEGIN;
  PenStyle penstyle1[1] = {0};
  if (NULL==penstyle) {
    penstyle = penstyle1;
    PenStyle_Init(penstyle1);
  }
  maxptlen = 8 + path->Count * 2 * (int)MAX(10, penstyle->lineWidth);
  PathSetSize(path2, maxptlen);
  path2->Count = vcgen_stroke2(path2->Points, path2->Types, 0, path->Points, path->Types, path->Count, penstyle);
  ASSERT(path2->Count<=maxptlen);
  PATH_FLATCOPY_END;
  return Ok;
}
static int vcgen_smooth_path1(const FPOINT* Points, const BYTE* Types, int Count, double m_smooth_value, FPOINT* Points1, BYTE* Types1) {
  int ipos = 0;
  PATH_LOOP_BEGIN;
  ipos = vcgen_smooth_poly1(end-pos, Points + pos, closed, m_smooth_value, Points1, Types1, ipos);
  PATH_LOOP_END;
  return ipos;
}
static Status PathSmooth1(const Path* path, double flatness, double m_smooth_value, Path* path2) {
  int maxptlen;
  PATH_FLATCOPY_BEGIN;
  maxptlen = 3*path->Count;
  path2->Count = 0;
  PathSetSize(path2, maxptlen);
  path2->Count = vcgen_smooth_path1(path->Points, path->Types, path->Count, m_smooth_value, path2->Points, path2->Types);
  ASSERT(path2->Count<=maxptlen);
  PATH_FLATCOPY_END;
  return Ok;
}

#endif // _PATH_INL_
