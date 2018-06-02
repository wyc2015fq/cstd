#ifndef _REGION_INL_
#define _REGION_INL_
#include "list.h"
typedef int64 INT64;
#define TRACE_ON(x) (1)
#define ERROR 0
// region_t type
#define NULLREGION 1
#define SIMPLEREGION 2
#define COMPLEXREGION 3

// region_t opt
#define Rgn_ERROR ERROR
//
#define WINDING 2
typedef float FLOAT;
typedef struct {
  FLOAT eM11;
  FLOAT eM12;
  FLOAT eM21;
  FLOAT eM22;
  FLOAT eDx;
  FLOAT eDy;
} mat2x3_t;
typedef struct {
  int size;
  int numRects;
  IRECT* rects;
  IRECT extents;
} region_t;
/* Undocumented value for DIB's iUsage: Indicates a mono DIB w/o pal entries */
#define DIB_PAL_MONO 2
CC_INLINE BOOL is_rect_empty(const IRECT* rect)
{
  return (rect->l >= rect->r || rect->t >= rect->b);
}
CC_INLINE BOOL intersect_rect(IRECT* dst, const IRECT* src1, const IRECT* src2)
{
  dst->l = MAX(src1->l, src2->l);
  dst->t = MAX(src1->t, src2->t);
  dst->r = MIN(src1->r, src2->r);
  dst->b = MIN(src1->b, src2->b);
  return !is_rect_empty(dst);
}
CC_INLINE void offset_rect(IRECT* rect, int offset_x, int offset_y)
{
  rect->l += offset_x;
  rect->t += offset_y;
  rect->r += offset_x;
  rect->b += offset_y;
}
CC_INLINE void order_rect(IRECT* rect)
{
  if (rect->l > rect->r) {
    int tmp = rect->l;
    rect->l = rect->r;
    rect->r = tmp;
  }
  if (rect->t > rect->b) {
    int tmp = rect->t;
    rect->t = rect->b;
    rect->b = tmp;
  }
}
CC_INLINE void get_bounding_rect(IRECT* rect, int x, int y, int width, int height)
{
  rect->l = x;
  rect->r = x + width;
  rect->t = y;
  rect->b = y + height;
  if (rect->l > rect->r) {
    int tmp = rect->l;
    rect->l = rect->r + 1;
    rect->r = tmp + 1;
  }
  if (rect->t > rect->b) {
    int tmp = rect->t;
    rect->t = rect->b + 1;
    rect->b = tmp + 1;
  }
}
CC_INLINE void reset_bounds(IRECT* bounds)
{
  bounds->l = bounds->t = INT_MAX;
  bounds->r = bounds->b = INT_MIN;
}
CC_INLINE void add_bounds_rect(IRECT* bounds, const IRECT* rect)
{
  if (is_rect_empty(rect)) {
    return;
  }
  bounds->l = MIN(bounds->l, rect->l);
  bounds->t = MIN(bounds->t, rect->t);
  bounds->r = MAX(bounds->r, rect->r);
  bounds->b = MAX(bounds->b, rect->b);
}
CC_INLINE int get_bitmap_stride(int width, int bpp)
{
  return ((width * bpp + 15) >> 3) & ~1;
}
CC_INLINE int get_dib_stride(int width, int bpp)
{
  return ((width * bpp + 31) >> 3) & ~3;
}
/* Check if two RECTs overlap. */
CC_INLINE BOOL overlapping(const IRECT* r1, const IRECT* r2)
{
  return (r1->r > r2->l && r1->l < r2->r &&
      r1->b > r2->t && r1->t < r2->b);
}
static BOOL add_rect(region_t* reg, int l, int t, int r, int b)
{
  IRECT* rect;
  if (reg->numRects >= reg->size) {
    IRECT* newrects = (IRECT*)prealloc(reg->rects, 2 * sizeof(IRECT) * reg->size);
    if (!newrects) {
      return FALSE;
    }
    reg->rects = newrects;
    reg->size *= 2;
  }
  rect = reg->rects + reg->numRects++;
  rect->l = l;
  rect->t = t;
  rect->r = r;
  rect->b = b;
  return TRUE;
}
// region_SetEmpty
CC_INLINE void region_SetEmpty(region_t* reg)
{
  reg->numRects = 0;
  reg->extents.l = reg->extents.t = reg->extents.r = reg->extents.b = 0;
}
CC_INLINE BOOL is_in_rect(const IRECT* rect, int x, int y)
{
  return (rect->r > x && rect->l <= x && rect->b > y && rect->t <= y);
}
// number of points to buffer before sending them off to scanlines() : Must be an even number
#define NUMPTSTOBUFFER 200
// used to allocate buffers for points and link the buffers together
struct point_block {
  IPOINT pts[NUMPTSTOBUFFER];
  int count;
  struct point_block* next;
};
static struct point_block* add_point(struct point_block* block, int x, int y) {
  if (block->count == NUMPTSTOBUFFER) {
    struct point_block* new_block = (struct point_block*)pmalloc(sizeof(*new_block));
    if (!new_block) {
      return NULL;
    }
    block->next = new_block;
    new_block->count = 0;
    new_block->next = NULL;
    block = new_block;
  }
  block->pts[block->count].x = x;
  block->pts[block->count].y = y;
  block->count++;
  return block;
}
static void free_point_blocks(struct point_block* block)
{
  while (block) {
    struct point_block* tmp = block->next;
    pfree(block);
    block = tmp;
  }
}
// This file contains a few macros to help track
// the edge of a filled object. The object is assumed
// to be filled in scanline order, and thus the
// algorithm used is an extension of Bresenham's line
// drawing algorithm which assumes that y is always the
// major axis.
// Since these pieces of code are the same for any filled shape,
// it is more convenient to gather the library in one
// place, but since these pieces of code are also in
// the inner loops of output primitives, procedure call
// overhead is out of the question.
// See the author for a derivation if needed.
//
// This structure contains all of the information needed
// to run the bresenham algorithm.
// The variables may be hardcoded into the declarations
// instead of using this structure to make use of
// register declarations.
//
struct bres_info {
  int minor_axis; /* minor axis */
  int d; /* decision variable */
  int m, m1; /* slope and slope+1 */
  int incr1, incr2; /* error increments */
};
// In scan converting polygons, we want to choose those pixels
// which are inside the polygon. Thus, we add .5 to the starting
// x coordinate for both l and r edges. Now we choose the
// first pixel which is inside the pgon for the l edge and the
// first pixel which is outside the pgon for the r edge.
// Draw the l pixel, but not the r.
//
// How to add .5 to the starting x coordinate:
// If the edge is moving to the r, then subtract dy from the
// error term from the general form of the algorithm.
// If the edge is moving to the l, then add dy to the error term.
//
// The reason for the difference between edges moving to the l
// and edges moving to the r is simple: If an edge is moving
// to the r, then we want the algorithm to flip immediately.
// If it is moving to the l, then we don't want it to flip until
// we traverse an entire pixel.
//
CC_INLINE void bres_init_polygon(int dy, int x1, int x2, struct bres_info* bres)
{
  int dx;
  // if the edge is horizontal, then it is ignored
  // and assumed not to be processed. Otherwise, do this stuff.
  //
  if (!dy) {
    return;
  }
  bres->minor_axis = x1;
  dx = x2 - x1;
  if (dx < 0) {
    bres->m = dx / dy;
    bres->m1 = bres->m - 1;
    bres->incr1 = -2 * dx + 2 * dy * bres->m1;
    bres->incr2 = -2 * dx + 2 * dy * bres->m;
    bres->d = 2 * bres->m * dy - 2 * dx - 2 * dy;
  }
  else {
    bres->m = dx / (dy);
    bres->m1 = bres->m + 1;
    bres->incr1 = 2 * dx - 2 * dy * bres->m1;
    bres->incr2 = 2 * dx - 2 * dy * bres->m;
    bres->d = -2 * bres->m * dy + 2 * dx;
  }
}
CC_INLINE void bres_incr_polygon(struct bres_info* bres)
{
  if (bres->m1 > 0) {
    if (bres->d > 0) {
      bres->minor_axis += bres->m1;
      bres->d += bres->incr1;
    }
    else {
      bres->minor_axis += bres->m;
      bres->d += bres->incr2;
    }
  }
  else {
    if (bres->d >= 0) {
      bres->minor_axis += bres->m1;
      bres->d += bres->incr1;
    }
    else {
      bres->minor_axis += bres->m;
      bres->d += bres->incr2;
    }
  }
}
// These are the data structures needed to scan
// convert regions. Two different scan conversion
// methods are available -- the even-odd method, and
// the winding number method.
// The even-odd rule states that a point is inside
// the polygon if a ray drawn from that point in any
// direction will pass through an odd number of
// path segments.
// By the winding number rule, a point is decided
// to be inside the polygon if a ray drawn from that
// point in any direction passes through a different
// number of clockwise and counter-clockwise path
// segments.
//
// These data structures are adapted somewhat from
// the algorithm in (Foley/Van Dam) for scan converting
// polygons.
// The basic algorithm is to start at the t (smallest y)
// of the polygon, stepping down to the b of
// the polygon by incrementing the y coordinate. We
// keep a list of edges which the current scanline crosses,
// sorted by x. This list is called the Active Edge Table (AET)
// As we change the y-coordinate, we update each entry in
// in the active edge table to reflect the edges new xcoord.
// This list must be sorted at each scanline in case
// two edges intersect.
// We also keep a data structure known as the Edge Table (ET),
// which keeps track of all the edges which the current
// scanline has not yet reached. The ET is basically a
// list of ScanLineList structures containing a list of
// edges which are entered at a given scanline. There is one
// ScanLineList per scanline at which an edge is entered.
// When we enter a new edge, we move it from the ET to the AET.
//
// From the AET, we can implement the even-odd rule as in
// (Foley/Van Dam).
// The winding number rule is a little trickier. We also
// keep the EdgeTableEntries in the AET linked by the
// nextWETE (winding EdgeTableEntry) link. This allows
// the edges to be linked just as before for updating
// purposes, but only uses the edges linked by the nextWETE
// link as edges representing spans of the polygon to
// drawn (as with the even-odd rule).
//
typedef struct edge_table_entry {
  struct list entry;
  struct list winding_entry;
  int ymax; /* ycoord at which we exit this edge. */
  struct bres_info bres; /* Bresenham info to run the edge */
  int ClockWise; /* flag for winding number rule */
} EdgeTableEntry;
typedef struct _ScanLineList {
  struct list edgelist;
  int scanline; /* the scanline represented */
  struct _ScanLineList* next; /* next in the list */
} ScanLineList;
typedef struct {
  int ymax; /* ymax for the polygon */
  int ymin; /* ymin for the polygon */
  ScanLineList scanlines; /* header node */
} EdgeTable;
/*
 * Here is a struct to help with storage allocation
 * so we can allocate a big chunk at a time, and then take
 * pieces from this heap when we need to.
 */
#define SLLSPERBLOCK 25
typedef struct _ScanLineListBlock {
  ScanLineList SLLs[SLLSPERBLOCK];
  struct _ScanLineListBlock* next;
} ScanLineListBlock;
/* Note the parameter order is different from the X11 equivalents */
#define Rgn_DEFAULT_RECTS 2
// region_get_type
CC_INLINE int region_get_type(const region_t* obj)
{
  switch (obj->numRects) {
  case 0:
    return NULLREGION;
  case 1:
    return SIMPLEREGION;
  default:
    return COMPLEXREGION;
  }
}
// region_DumpRegion
// Outputs the contents of a region_t
static void region_DumpRegion(const region_t* pReg)
{
  IRECT* pRect, *pRectEnd = pReg->rects + pReg->numRects;
  TRACE("Region %p: %d,%d - %d,%d %d rects\n", pReg, pReg->extents.l, pReg->extents.t, pReg->extents.r, pReg->extents.b, pReg->numRects);
  for (pRect = pReg->rects; pRect < pRectEnd; pRect++) {
    TRACE("\t%d,%d - %d,%d\n", pRect->l, pRect->t, pRect->r, pRect->b);
  }
  return;
}
// region_SetSize
// Initialize a new empty region.
static BOOL region_SetSize(region_t* pReg, int n)
{
  if (pReg->size < n) {
    if (!(pReg->rects = (IRECT*)prealloc(pReg->rects, n * sizeof(IRECT)))) {
      return FALSE;
    }
  }
  pReg->size = n;
  region_SetEmpty(pReg);
  return TRUE;
}
//
// region_free
static void region_free(region_t* pReg)
{
  pfree(pReg->rects);
}
// region_CopyRegion
static BOOL region_CopyRegion(region_t* dst, const region_t* src)
{
  if (dst != src) { /* don't want to copy to itself */
    if (dst->size < src->numRects) {
      IRECT* rects = (IRECT*)prealloc(dst->rects, src->numRects * sizeof(IRECT));
      if (!rects) {
        return FALSE;
      }
      dst->rects = rects;
      dst->size = src->numRects;
    }
    dst->numRects = src->numRects;
    dst->extents.l = src->extents.l;
    dst->extents.t = src->extents.t;
    dst->extents.r = src->extents.r;
    dst->extents.b = src->extents.b;
    memcpy(dst->rects, src->rects, src->numRects * sizeof(IRECT));
  }
  return TRUE;
}
// region_OffsetRegion
// Offset a region_t by x,y
//
static BOOL region_OffsetRegion(region_t* rgn, region_t* srcrgn, int x, int y)
{
  if (rgn != srcrgn) {
    if (!region_CopyRegion(rgn, srcrgn)) {
      return FALSE;
    }
  }
  if (x || y) {
    int nbox = rgn->numRects;
    IRECT* pbox = rgn->rects;
    if (nbox) {
      while (nbox--) {
        pbox->l += x;
        pbox->r += x;
        pbox->t += y;
        pbox->b += y;
        pbox++;
      }
      rgn->extents.l += x;
      rgn->extents.r += x;
      rgn->extents.t += y;
      rgn->extents.b += y;
    }
  }
  return TRUE;
}
// OffsetRgn (GDI32.@)
//
// Moves a region by the specified X- and Y-axis offsets.
//
// PARAMS
// obj [I] Region to offset.
// x [I] Offset r if positive or l if negative.
// y [I] Offset down if positive or up if negative.
//
// RETURNS
// Success:
// NULLREGION - The new region is empty.
// SIMPLEREGION - The new region can be represented by one rectangle.
// COMPLEXREGION - The new region can only be represented by more than
// one rectangle.
// Failure: ERROR
int region_OffsetRgn(region_t* obj, int x, int y)
{
  int ret;
  TRACE("%p %d,%d\n", obj, x, y);
  if (!obj) {
    return ERROR;
  }
  region_OffsetRegion(obj, obj, x, y);
  ret = region_get_type(obj);
  return ret;
}
//
// GetRgnBox (GDI32.@)
//
// Retrieves the bounding rectangle of the region. The bounding rectangle
// is the smallest rectangle that contains the entire region.
//
// PARAMS
// obj [I] Region to retrieve bounding rectangle from.
// rect [O] Rectangle that will receive the coordinates of the bounding
// rectangle.
//
// RETURNS
// NULLREGION - The new region is empty.
// SIMPLEREGION - The new region can be represented by one rectangle.
// COMPLEXREGION - The new region can only be represented by more than
// one rectangle.
//
int region_GetRgnBox(const region_t* obj, IRECT* rect)
{
  if (obj) {
    int ret;
    rect->l = obj->extents.l;
    rect->t = obj->extents.t;
    rect->r = obj->extents.r;
    rect->b = obj->extents.b;
    TRACE("%p (%d,%d-%d,%d)\n", obj, rect->l, rect->t, rect->r, rect->b);
    ret = region_get_type(obj);
    return ret;
  }
  return ERROR;
}
// region_SetRectRgn (GDI32.@)
//
// Sets a region to a simple rectangular region.
//
// PARAMS
// obj [I] Region to convert.
// l [I] Left coordinate of rectangle.
// t [I] Top coordinate of rectangle.
// r [I] Right coordinate of rectangle.
// b [I] Bottom coordinate of rectangle.
//
// RETURNS
// Success: Non-zero.
// Failure: Zero.
//
// NOTES
// Allows either or both l and t to be greater than r or b.
//
BOOL region_SetRectRgn(region_t* obj, int l, int t, int r, int b)
{
  /* Allocate 2 rects by default to reduce the number of reallocs */
  if (!region_SetSize(obj, Rgn_DEFAULT_RECTS)) {
    pfree(obj);
    return 0;
  }
  TRACE("%p %d,%d-%d,%d\n", obj, l, t, r, b);
  if (l > r) {
    int tmp = l;
    l = r;
    r = tmp;
  }
  if (t > b) {
    int tmp = t;
    t = b;
    b = tmp;
  }
  if ((l != r) && (t != b)) {
    obj->rects->l = obj->extents.l = l;
    obj->rects->t = obj->extents.t = t;
    obj->rects->r = obj->extents.r = r;
    obj->rects->b = obj->extents.b = b;
    obj->numRects = 1;
  }
  else {
    region_SetEmpty(obj);
  }
  return TRUE;
}
//
// region_SetRectRgnIndirect (GDI32.@)
//
// Creates a simple rectangular region.
//
// PARAMS
// rect [I] Coordinates of rectangular region.
//
// RETURNS
// Success: Handle to region.
// Failure: NULL.
int region_SetRectRgnIndirect(region_t* obj, const IRECT* rect)
{
  return region_SetRectRgn(obj, rect->l, rect->t, rect->r, rect->b);
}
// region_SetRoundRectRgn (GDI32.@)
// Creates a rectangular region with rounded corners.
//
// PARAMS
// l [I] Left coordinate of rectangle.
// t [I] Top coordinate of rectangle.
// r [I] Right coordinate of rectangle.
// b [I] Bottom coordinate of rectangle.
// ellipse_width [I] Width of the ellipse at each corner.
// ellipse_height [I] Height of the ellipse at each corner.
//
// RETURNS
// Success: Handle to region.
// Failure: NULL.
//
// NOTES
// If ellipse_width or ellipse_height is less than 2 logical units then
// it is treated as though region_SetRectRgn() was called instead.
int region_SetRoundRectRgn(region_t* obj, int left, int top, int right, int bottom, int ellipse_width, int ellipse_height)
{
  int a, b, i, x, y;
  INT64 asq, bsq, dx, dy, err;
  IRECT* rects = NULL;
  /* Make the dimensions sensible */
  if (left > right) {
    int tmp = left;
    left = right;
    right = tmp;
  }
  if (top > bottom) {
    int tmp = top;
    top = bottom;
    bottom = tmp;
  }
  /* the region is for the rectangle interior, but only at right and bottom for some reason */
  right--;
  bottom--;
  ellipse_width = MIN(right - left, abs(ellipse_width));
  ellipse_height = MIN(bottom - top, abs(ellipse_height));
  /* Check if we can do a normal rectangle instead */
  if ((ellipse_width < 2) || (ellipse_height < 2)) {
    return region_SetRectRgn(obj, left, top, right, bottom);
  }
  if (!region_SetSize(obj, ellipse_height)) {
    return 0;
  }
  rects = obj->rects;
  obj->numRects = ellipse_height;
  obj->extents.l = left;
  obj->extents.t = top;
  obj->extents.r = right;
  obj->extents.b = bottom;
  /* based on an algorithm by Alois Zingl */
  a = ellipse_width - 1;
  b = ellipse_height - 1;
  asq = (INT64)8 * a * a;
  bsq = (INT64)8 * b * b;
  dx = (INT64)4 * b * b * (1 - a);
  dy = (INT64)4 * a * a * (1 + (b % 2));
  err = dx + dy + a * a * (b % 2);
  x = 0;
  y = ellipse_height / 2;
  rects[y].l = left;
  rects[y].r = right;
  while (x <= ellipse_width / 2) {
    INT64 e2 = 2 * err;
    if (e2 >= dx) {
      x++;
      err += dx += bsq;
    }
    if (e2 <= dy) {
      y++;
      err += dy += asq;
      rects[y].l = left + x;
      rects[y].r = right - x;
    }
  }
  for (i = 0; i < ellipse_height / 2; i++) {
    rects[i].l = rects[b - i].l;
    rects[i].r = rects[b - i].r;
    rects[i].t = top + i;
    rects[i].b = rects[i].t + 1;
  }
  for (; i < ellipse_height; i++) {
    rects[i].t = bottom - ellipse_height + i;
    rects[i].b = rects[i].t + 1;
  }
  rects[ellipse_height / 2].t = top + ellipse_height / 2; /* extend to top of rectangle */
  TRACE("(%d,%d-%d,%d %dx%d): ret=%p\n", left, top, right, bottom, ellipse_width, ellipse_height, obj);
  return 0;
}
// CreateEllipticRgn (GDI32.@)
//
// Creates an elliptical region.
//
// PARAMS
// l [I] Left coordinate of bounding rectangle.
// t [I] Top coordinate of bounding rectangle.
// r [I] Right coordinate of bounding rectangle.
// b [I] Bottom coordinate of bounding rectangle.
//
// RETURNS
// Success: Handle to region.
// Failure: NULL.
//
// NOTES
// This is a special case of region_SetRoundRectRgn() where the width of the
// ellipse at each corner is equal to the width the rectangle and
// the same for the height.
int region_SetEllipticRgn(region_t* obj, int l, int t, int r, int b)
{
  return region_SetRoundRectRgn(obj, l, t, r, b, r - l, b - t);
}
// region_SetEllipticRgnIndirect (GDI32.@)
//
// Creates an elliptical region.
//
// PARAMS
// rect [I] Pointer to bounding rectangle of the ellipse.
//
// RETURNS
// Success: Handle to region.
// Failure: NULL.
//
// NOTES
// This is a special case of region_SetRoundRectRgn() where the width of the
// ellipse at each corner is equal to the width the rectangle and
// the same for the height.
//
int region_SetEllipticRgnIndirect(region_t* obj, const IRECT* rect)
{
  return region_SetRoundRectRgn(obj, rect->l, rect->t, rect->r, rect->b, rect->r - rect->l, rect->b - rect->t);
}
static void translate(IPOINT* pt, UINT count, const mat2x3_t* xform)
{
  while (count--) {
    double x = pt->x;
    double y = pt->y;
    pt->x = floor(x * xform->eM11 + y * xform->eM21 + xform->eDx + 0.5);
    pt->y = floor(x * xform->eM12 + y * xform->eM22 + xform->eDy + 0.5);
    pt++;
  }
}
// region_Coalesce
//
// Attempt to merge the rects in the current band with those in the
// previous one. Used only by region_RegionOp.
//
// Results:
// The new index for the previous band.
//
// Side Effects:
// If coalescing takes place:
// - rectangles in the previous band will have their b fields
// altered.
// - pReg->numRects will be decreased.
// region_t* pReg, /* Region to coalesce */
// int prevStart, /* Index of start of previous band */
// int curStart /* Index of start of current band */
static int region_Coalesce(region_t* pReg, int prevStart, int curStart)
{
  IRECT* pPrevRect; /* Current rect in previous band */
  IRECT* pCurRect; /* Current rect in current band */
  IRECT* pRegEnd; /* End of region */
  int curNumRects; /* Number of rectangles in current band */
  int prevNumRects; /* Number of rectangles in previous band */
  int bandtop; /* t coordinate for current band */
  pRegEnd = &pReg->rects[pReg->numRects];
  pPrevRect = &pReg->rects[prevStart];
  prevNumRects = curStart - prevStart;
  // Figure out how many rectangles are in the current band. Have to do
  // this because multiple bands could have been added in region_RegionOp
  // at the end when one region has been exhausted.
  pCurRect = &pReg->rects[curStart];
  bandtop = pCurRect->t;
  for (curNumRects = 0; (pCurRect != pRegEnd) && (pCurRect->t == bandtop); curNumRects++) {
    pCurRect++;
  }
  if (pCurRect != pRegEnd) {
    // If more than one band was added, we have to find the start
    // of the last band added so the next coalescing job can start
    // at the r place... (given when multiple bands are added,
    // this may be pointless -- see above).
    pRegEnd--;
    while (pRegEnd[-1].t == pRegEnd->t) {
      pRegEnd--;
    }
    curStart = pRegEnd - pReg->rects;
    pRegEnd = pReg->rects + pReg->numRects;
  }
  if ((curNumRects == prevNumRects) && (curNumRects != 0)) {
    pCurRect -= curNumRects;
    //The bands may only be coalesced if the b of the previous
    //matches the t scanline of the current.
    if (pPrevRect->b == pCurRect->t) {
      // Make sure the bands have rects in the same places. This
      // assumes that rects have been added in such a way that they
      // cover the most area possible. I.e. two rects in a band must
      // have some horizontal space between them.
      do {
        if ((pPrevRect->l != pCurRect->l) ||
            (pPrevRect->r != pCurRect->r)) {
          // The bands don't line up so they can't be coalesced.
          return (curStart);
        }
        pPrevRect++;
        pCurRect++;
        prevNumRects -= 1;
      }
      while (prevNumRects != 0);
      pReg->numRects -= curNumRects;
      pCurRect -= curNumRects;
      pPrevRect -= curNumRects;
      // The bands may be merged, so set the b of each rect
      // in the previous band to that of the corresponding rect in
      // the current band.
      do {
        pPrevRect->b = pCurRect->b;
        pPrevRect++;
        pCurRect++;
        curNumRects -= 1;
      }
      while (curNumRects != 0);
      // If only one band was added to the region, we have to backup
      // curStart to the start of the previous band.
      //
      // If more than one band was added to the region, copy the
      // other bands down. The assumption here is that the other bands
      // came from the same region as the current one and no further
      // coalescing can be done on them since it's all been done
      // already... curStart is already in the r place.
      //
      if (pCurRect == pRegEnd) {
        curStart = prevStart;
      }
      else {
        do {
          *pPrevRect++ = *pCurRect++;
        }
        while (pCurRect != pRegEnd);
      }
    }
  }
  return (curStart);
}
// region_RegionOp
//
// Apply an operation to two regions. Called by region_Union,
// region_Inverse, region_Subtract, region_Intersect...
//
// Results:
// None.
//
// Side Effects:
// The new region is overwritten.
//
// Notes:
// The idea behind this function is to view the two regions as sets.
// Together they cover a rectangle of area that this function divides
// into horizontal bands where points are covered only by one region
// or by both. For the first case, the nonOverlapFunc is called with
// each the band and the band's upper and lower extents. For the
// second, the overlapFunc is called to process the entire band. It
// is responsible for clipping the rectangles in the band, though
// this function provides the boundaries.
// At the end of each band, the new region is coalesced, if possible,
// to reduce the number of rectangles in the region.
//
//
static BOOL region_RegionOp(
    region_t* destReg, /* Place to store result */
    const region_t* reg1, /* First region in operation */
    const region_t* reg2, /* 2nd region in operation */
    BOOL (*overlapFunc)(region_t*, IRECT*, IRECT*, IRECT*, IRECT*, int, int), /* Function to call for over-lapping bands */
    BOOL (*nonOverlap1Func)(region_t*, IRECT*, IRECT*, int, int), /* Function to call for non-overlapping bands in region 1 */
    BOOL (*nonOverlap2Func)(region_t*, IRECT*, IRECT*, int, int) /* Function to call for non-overlapping bands in region 2 */
)
{
  region_t newReg[1] = {0};
  IRECT* r1; /* Pointer into first region */
  IRECT* r2; /* Pointer into 2d region */
  IRECT* r1End; /* End of 1st region */
  IRECT* r2End; /* End of 2d region */
  int ybot; /* Bottom of intersection */
  int ytop; /* Top of intersection */
  int prevBand; /* Index of start of previous band in newReg */
  int curBand; /* Index of start of current band in newReg */
  IRECT* r1BandEnd; /* End of current band in r1 */
  IRECT* r2BandEnd; /* End of current band in r2 */
  int t; /* Top of non-overlapping band */
  int bot; /* Bottom of non-overlapping band */
  // Initialization:
  // set r1, r2, r1End and r2End appropriately, preserve the important
  // parts of the destination region until the end in case it's one of
  // the two source regions, then mark the "new" region empty, allocating
  // another array of rectangles for it to use.
  //
  r1 = reg1->rects;
  r2 = reg2->rects;
  r1End = r1 + reg1->numRects;
  r2End = r2 + reg2->numRects;
  // Allocate a reasonable number of rectangles for the new region. The idea
  // is to allocate enough so the individual functions don't need to
  // reallocate and copy the array, which is time consuming, yet we don't
  // have to worry about using too much memory. I hope to be able to
  // nuke the Xrealloc() at the end of this function eventually.
  //
  if (!region_SetSize(newReg, MAX(reg1->numRects, reg2->numRects) * 2)) {
    return FALSE;
  }
  // Initialize ybot and ytop.
  // In the upcoming loop, ybot and ytop serve different functions depending
  // on whether the band being handled is an overlapping or non-overlapping
  // band.
  // In the case of a non-overlapping band (only one of the regions
  // has points in the band), ybot is the b of the most recent
  // intersection and thus clips the t of the rectangles in that band.
  // ytop is the t of the next intersection between the two regions and
  // serves to clip the b of the rectangles in the current band.
  // For an overlapping band (where the two regions intersect), ytop clips
  // the t of the rectangles of both regions and ybot clips the bottoms.
  //
  if (reg1->extents.t < reg2->extents.t) {
    ybot = reg1->extents.t;
  }
  else {
    ybot = reg2->extents.t;
  }
  // prevBand serves to mark the start of the previous band so rectangles
  // can be coalesced into larger rectangles. qv. miCoalesce, above.
  // In the beginning, there is no previous band, so prevBand == curBand
  // (curBand is set later on, of course, but the first band will always
  // start at index 0). prevBand and curBand must be indices because of
  // the possible expansion, and resultant moving, of the new region's
  // array of rectangles.
  //
  prevBand = 0;
  do {
    curBand = newReg->numRects;
    // This algorithm proceeds one source-band (as opposed to a
    // destination band, which is determined by where the two regions
    // intersect) at a time. r1BandEnd and r2BandEnd serve to mark the
    // rectangle after the last one in the current band for their
    // respective regions.
    //
    r1BandEnd = r1;
    while ((r1BandEnd != r1End) && (r1BandEnd->t == r1->t)) {
      r1BandEnd++;
    }
    r2BandEnd = r2;
    while ((r2BandEnd != r2End) && (r2BandEnd->t == r2->t)) {
      r2BandEnd++;
    }
    // First handle the band that doesn't intersect, if any.
    //
    // Note that attention is restricted to one band in the
    // non-intersecting region at once, so if a region has n
    // bands between the current position and the next place it overlaps
    // the other, this entire loop will be passed through n times.
    //
    if (r1->t < r2->t) {
      t = MAX(r1->t, ybot);
      bot = MIN(r1->b, r2->t);
      if ((t != bot) && (nonOverlap1Func != NULL)) {
        if (!nonOverlap1Func(newReg, r1, r1BandEnd, t, bot)) {
          return FALSE;
        }
      }
      ytop = r2->t;
    }
    else if (r2->t < r1->t) {
      t = MAX(r2->t, ybot);
      bot = MIN(r2->b, r1->t);
      if ((t != bot) && (nonOverlap2Func != NULL)) {
        if (!nonOverlap2Func(newReg, r2, r2BandEnd, t, bot)) {
          return FALSE;
        }
      }
      ytop = r1->t;
    }
    else {
      ytop = r1->t;
    }
    // If any rectangles got added to the region, try and coalesce them
    // with rectangles from the previous band. Note we could just do
    // this test in miCoalesce, but some machines incur a not
    // inconsiderable cost for function calls, so...
    //
    if (newReg->numRects != curBand) {
      prevBand = region_Coalesce(newReg, prevBand, curBand);
    }
    // Now see if we've hit an intersecting band. The two bands only
    // intersect if ybot > ytop
    //
    ybot = MIN(r1->b, r2->b);
    curBand = newReg->numRects;
    if (ybot > ytop) {
      if (!overlapFunc(newReg, r1, r1BandEnd, r2, r2BandEnd, ytop, ybot)) {
        return FALSE;
      }
    }
    if (newReg->numRects != curBand) {
      prevBand = region_Coalesce(newReg, prevBand, curBand);
    }
    // If we've finished with a band (b == ybot) we skip forward
    // in the region to the next band.
    //
    if (r1->b == ybot) {
      r1 = r1BandEnd;
    }
    if (r2->b == ybot) {
      r2 = r2BandEnd;
    }
  }
  while ((r1 != r1End) && (r2 != r2End));
  // Deal with whichever region still has rectangles l.
  //
  curBand = newReg->numRects;
  if (r1 != r1End) {
    if (nonOverlap1Func != NULL) {
      do {
        r1BandEnd = r1;
        while ((r1BandEnd < r1End) && (r1BandEnd->t == r1->t)) {
          r1BandEnd++;
        }
        if (!nonOverlap1Func(newReg, r1, r1BandEnd, MAX(r1->t, ybot), r1->b)) {
          return FALSE;
        }
        r1 = r1BandEnd;
      }
      while (r1 != r1End);
    }
  }
  else if ((r2 != r2End) && (nonOverlap2Func != NULL)) {
    do {
      r2BandEnd = r2;
      while ((r2BandEnd < r2End) && (r2BandEnd->t == r2->t)) {
        r2BandEnd++;
      }
      if (!nonOverlap2Func(newReg, r2, r2BandEnd, MAX(r2->t, ybot), r2->b)) {
        return FALSE;
      }
      r2 = r2BandEnd;
    }
    while (r2 != r2End);
  }
  if (newReg->numRects != curBand) {
    region_Coalesce(newReg, prevBand, curBand);
  }
  // A bit of cleanup. To keep regions from growing without bound,
  // we shrink the array of rectangles to match the new number of
  // rectangles in the region. This never goes to 0, however...
  region_free(destReg);
  *destReg = *newReg;
  return TRUE;
}
// region_UnionO
//
// Handle an overlapping band for the union operation. Picks the
// l-most rectangle each time and merges it into the region.
//
// Results:
// None.
//
// Side Effects:
// Rectangles are overwritten in pReg->rects and pReg->numRects will
// be changed.
//
//
static BOOL region_UnionO(region_t* pReg, IRECT* r1, IRECT* r1End,
    IRECT* r2, IRECT* r2End, int top, int bottom)
{
#define MERGERECT(r1) \
  if ((pReg->numRects != 0) && \
      (pReg->rects[pReg->numRects-1].t == top) && \
      (pReg->rects[pReg->numRects-1].b == bottom) && \
      (pReg->rects[pReg->numRects-1].r >= r1->l)) \
  { \
    if (pReg->rects[pReg->numRects-1].r < r1->r) \
      pReg->rects[pReg->numRects-1].r = r1->r; \
  } \
  else \
  { \
    if (!add_rect( pReg, r1->l, top, r1->r, bottom )) return FALSE; \
  } \
  r1++;
  while ((r1 != r1End) && (r2 != r2End)) {
    if (r1->l < r2->l) {
      MERGERECT(r1);
    }
    else {
      MERGERECT(r2);
    }
  }
  if (r1 != r1End) {
    do {
      MERGERECT(r1);
    }
    while (r1 != r1End);
  }
  else while (r2 != r2End) {
      MERGERECT(r2);
    }
  return TRUE;
#undef MERGERECT
}
//
// Region Union
//
//
// region_UnionNonO
//
// Handle a non-overlapping band for the union operation. Just
// Adds the rectangles into the region. Doesn't have to check for
// subsumption or anything.
//
// Results:
// None.
//
// Side Effects:
// pReg->numRects is incremented and the final rectangles overwritten
// with the rectangles we're passed.
static BOOL region_UnionNonO(region_t* pReg, IRECT* r, IRECT* rEnd, int t, int b)
{
  while (r != rEnd) {
    if (!add_rect(pReg, r->l, t, r->r, b)) {
      return FALSE;
    }
    r++;
  }
  return TRUE;
}
// region_UnionRegion
static BOOL region_UnionRegion(region_t* newReg, const region_t* reg1, const region_t* reg2)
{
  BOOL ret = TRUE;
  /* checks all the simple cases */
  // Region 1 and 2 are the same or region 1 is empty
  //
  if ((reg1 == reg2) || (!(reg1->numRects))) {
    if (newReg != reg2) {
      ret = region_CopyRegion(newReg, reg2);
    }
    return ret;
  }
  // if nothing to union (region 2 empty)
  //
  if (!(reg2->numRects)) {
    if (newReg != reg1) {
      ret = region_CopyRegion(newReg, reg1);
    }
    return ret;
  }
  // Region 1 completely subsumes region 2
  //
  if ((reg1->numRects == 1) &&
      (reg1->extents.l <= reg2->extents.l) &&
      (reg1->extents.t <= reg2->extents.t) &&
      (reg1->extents.r >= reg2->extents.r) &&
      (reg1->extents.b >= reg2->extents.b)) {
    if (newReg != reg1) {
      ret = region_CopyRegion(newReg, reg1);
    }
    return ret;
  }
  // Region 2 completely subsumes region 1
  //
  if ((reg2->numRects == 1) &&
      (reg2->extents.l <= reg1->extents.l) &&
      (reg2->extents.t <= reg1->extents.t) &&
      (reg2->extents.r >= reg1->extents.r) &&
      (reg2->extents.b >= reg1->extents.b)) {
    if (newReg != reg2) {
      ret = region_CopyRegion(newReg, reg2);
    }
    return ret;
  }
  if ((ret = region_RegionOp(newReg, reg1, reg2, region_UnionO, region_UnionNonO, region_UnionNonO))) {
    newReg->extents.l = MIN(reg1->extents.l, reg2->extents.l);
    newReg->extents.t = MIN(reg1->extents.t, reg2->extents.t);
    newReg->extents.r = MAX(reg1->extents.r, reg2->extents.r);
    newReg->extents.b = MAX(reg1->extents.b, reg2->extents.b);
  }
  return ret;
}
//
// Region Subtraction
//
//
// region_SubtractNonO1
//
// Deal with non-overlapping band for subtraction. Any parts from
// region 2 we discard. Anything from region 1 we add to the region.
//
// Results:
// None.
//
// Side Effects:
// pReg may be affected.
//
//
static BOOL region_SubtractNonO1(region_t* pReg, IRECT* r, IRECT* rEnd, int t, int b)
{
  while (r != rEnd) {
    if (!add_rect(pReg, r->l, t, r->r, b)) {
      return FALSE;
    }
    r++;
  }
  return TRUE;
}
//
// region_SubtractO
//
// Overlapping band subtraction. x1 is the l-most point not yet
// checked.
//
// Results:
// None.
//
// Side Effects:
// pReg may have rectangles added to it.
//
//
static BOOL region_SubtractO(region_t* pReg, IRECT* r1, IRECT* r1End,
    IRECT* r2, IRECT* r2End, int t, int b)
{
  int l = r1->l;
  while ((r1 != r1End) && (r2 != r2End)) {
    if (r2->r <= l) {
      /*
      * Subtrahend missed the boat: go to next subtrahend.
      */
      r2++;
    }
    else if (r2->l <= l) {
      /*
      * Subtrahend precedes minuend: nuke l edge of minuend.
      */
      l = r2->r;
      if (l >= r1->r) {
        /*
        * Minuend completely covered: advance to next minuend and
        * reset l fence to edge of new minuend.
        */
        r1++;
        if (r1 != r1End) {
          l = r1->l;
        }
      }
      else {
        /*
        * Subtrahend now used up since it doesn't extend beyond
        * minuend
        */
        r2++;
      }
    }
    else if (r2->l < r1->r) {
      /*
      * Left part of subtrahend covers part of minuend: add uncovered
      * part of minuend to region and skip to next subtrahend.
      */
      if (!add_rect(pReg, l, t, r2->l, b)) {
        return FALSE;
      }
      l = r2->r;
      if (l >= r1->r) {
        /*
        * Minuend used up: advance to new...
        */
        r1++;
        if (r1 != r1End) {
          l = r1->l;
        }
      }
      else {
        /*
        * Subtrahend used up
        */
        r2++;
      }
    }
    else {
      /*
      * Minuend used up: add any remaining piece before advancing.
      */
      if (r1->r > l) {
        if (!add_rect(pReg, l, t, r1->r, b)) {
          return FALSE;
        }
      }
      r1++;
      if (r1 != r1End) {
        l = r1->l;
      }
    }
  }
  /*
  * Add remaining minuend rectangles to region.
  */
  while (r1 != r1End) {
    if (!add_rect(pReg, l, t, r1->r, b)) {
      return FALSE;
    }
    r1++;
    if (r1 != r1End) {
      l = r1->l;
    }
  }
  return TRUE;
}
// region_SetExtents
// Re-calculate the extents of a region
static void region_SetExtents(region_t* pReg)
{
  IRECT* pRect, *pRectEnd, *pExtents;
  if (pReg->numRects == 0) {
    pReg->extents.l = 0;
    pReg->extents.t = 0;
    pReg->extents.r = 0;
    pReg->extents.b = 0;
    return;
  }
  pExtents = &pReg->extents;
  pRect = pReg->rects;
  pRectEnd = &pRect[pReg->numRects - 1];
  // Since pRect is the first rectangle in the region, it must have the
  // smallest t and since pRectEnd is the last rectangle in the region,
  // it must have the largest b, because of banding. Initialize l and
  // r from pRect and pRectEnd, resp., as good things to initialize them
  // to...
  //
  pExtents->l = pRect->l;
  pExtents->t = pRect->t;
  pExtents->r = pRectEnd->r;
  pExtents->b = pRectEnd->b;
  while (pRect <= pRectEnd) {
    if (pRect->l < pExtents->l) {
      pExtents->l = pRect->l;
    }
    if (pRect->r > pExtents->r) {
      pExtents->r = pRect->r;
    }
    pRect++;
  }
}
// region_SubtractRegion
//
// Subtract regS from regM and leave the result in regD.
// S stands for subtrahend, M for minuend and D for difference.
//
// Results:
// TRUE.
//
// Side Effects:
// regD is overwritten.
static BOOL region_SubtractRegion(region_t* regD, const region_t* regM, const region_t* regS)
{
  /* check for trivial reject */
  if ((!(regM->numRects)) || (!(regS->numRects)) ||
      (!overlapping(&regM->extents, &regS->extents))) {
    return region_CopyRegion(regD, regM);
  }
  if (!region_RegionOp(regD, regM, regS, region_SubtractO, region_SubtractNonO1, NULL)) {
    return FALSE;
  }
  // Can't alter newReg's extents before we call miRegionOp because
  // it might be one of the source regions and miRegionOp depends
  // on the extents of those regions being the unaltered. Besides, this
  // way there's no checking against rectangles that will be nuked
  // due to coalescing, so we have to examine fewer rectangles.
  //
  region_SetExtents(regD);
  return TRUE;
}
//
// region_XorRegion
//
static BOOL region_XorRegion(region_t* dr, const region_t* sra, const region_t* srb)
{
  region_t tra, trb;
  BOOL ret;
  if (!region_SetSize(&tra, sra->numRects + 1)) {
    return FALSE;
  }
  if ((ret = region_SetSize(&trb, srb->numRects + 1))) {
    ret = region_SubtractRegion(&tra, sra, srb) &&
        region_SubtractRegion(&trb, srb, sra) &&
        region_UnionRegion(dr, &tra, &trb);
    region_free(&trb);
  }
  region_free(&tra);
  return ret;
}
// region_UnionRectWithRegion
// Adds a rectangle to a region_t
static BOOL region_UnionRectWithRegion(const IRECT* rect, region_t* rgn)
{
  region_t region = {0};
  region.rects = &region.extents;
  region.numRects = 1;
  region.size = 1;
  region.extents = *rect;
  return region_UnionRegion(rgn, rgn, &region);
}
// region_SetRegionExt (GDI32.@)
//
// Creates a region as specified by the transformation data and region data.
//
// PARAMS
// lpXform [I] World-space to logical-space transformation data.
// dwCount [I] Size of the data pointed to by rgndata, in bytes.
// rgndata [I] Data that specifies the region.
//
// RETURNS
// Success: Handle to region.
// Failure: NULL.
//
#define region_SetLastError(x)
#define WARN(x)
int region_SetPolyPolygonRgn(region_t* obj, const IPOINT* Pts, const int* Count, int nbpolygons, int mode);
int region_boolopt(region_t* destObj, const region_t* src1Obj, const region_t* src2Obj, boolopt opt);
int region_SetRegionExt(region_t* obj, const mat2x3_t* lpXform, DWORD dwCount, const IRECT* rgndata)
{
  if (lpXform) {
    const IRECT* pCurRect, *pEndRect;
    region_t poly_hrgn[1] = {0};
    region_SetRectRgn(obj, 0, 0, 0, 0);
    pEndRect = rgndata + dwCount;
    for (pCurRect = rgndata; pCurRect < pEndRect; pCurRect++) {
      static const int count = 4;
      IPOINT pt[4];
      pt[0].x = pCurRect->l;
      pt[0].y = pCurRect->t;
      pt[1].x = pCurRect->r;
      pt[1].y = pCurRect->t;
      pt[2].x = pCurRect->r;
      pt[2].y = pCurRect->b;
      pt[3].x = pCurRect->l;
      pt[3].y = pCurRect->b;
      translate(pt, 4, lpXform);
      region_SetPolyPolygonRgn(poly_hrgn, pt, &count, 1, WINDING);
      region_boolopt(obj, obj, poly_hrgn, Rgn_OR);
    }
    region_free(poly_hrgn);
    return 0;
  }
  if (region_SetSize(obj, dwCount)) {
    const IRECT* pCurRect, *pEndRect;
    pEndRect = rgndata + dwCount;
    for (pCurRect = rgndata; pCurRect < pEndRect; pCurRect++) {
      if (pCurRect->l < pCurRect->r && pCurRect->t < pCurRect->b) {
        if (!region_UnionRectWithRegion(pCurRect, obj)) {
          return 0;
        }
      }
    }
  }
  else {
    return 0;
  }
  TRACE("%p %d %p returning %p\n", lpXform, dwCount, rgndata, obj);
  return 0;
}
//
// PtInRegion (GDI32.@)
//
// Tests whether the specified point is inside a region.
//
// PARAMS
// obj [I] Region to test.
// x [I] X-coordinate of point to test.
// y [I] Y-coordinate of point to test.
//
// RETURNS
// Non-zero if the point is inside the region or zero otherwise.
//
BOOL iPtInRegion(const region_t* obj, int x, int y)
{
  if (obj) {
    if (obj->numRects > 0 && is_in_rect(&obj->extents, x, y)) {
      int i;
      for (i = 0; i < obj->numRects; i++) {
        if (is_in_rect(&obj->rects[i], x, y)) {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}
//
// region_RectInRegion (GDI32.@)
//
// Tests if a rectangle is at least partly inside the specified region.
//
// PARAMS
// obj [I] Region to test.
// rect [I] Rectangle to test.
//
// RETURNS
// Non-zero if the rectangle is partially inside the region or
// zero otherwise.
//
BOOL region_RectInRegion(region_t* obj, const IRECT* rect)
{
  BOOL ret = FALSE;
  IRECT rc;
  /* swap the coordinates to make r >= l and b >= t */
  /* (region building rectangles are normalized the same way) */
  rc = *rect;
  order_rect(&rc);
  if (obj) {
    IRECT* pCurRect, *pRectEnd;
    /* this is (just) a useful optimization */
    if ((obj->numRects > 0) && overlapping(&obj->extents, &rc)) {
      for (pCurRect = obj->rects, pRectEnd = pCurRect + obj->numRects; pCurRect < pRectEnd; pCurRect++) {
        if (pCurRect->b <= rc.t) {
          continue; /* not far enough down yet */
        }
        if (pCurRect->t >= rc.b) {
          break; /* too far down */
        }
        if (pCurRect->r <= rc.l) {
          continue; /* not far enough over yet */
        }
        if (pCurRect->l >= rc.r) {
          continue;
        }
        ret = TRUE;
        break;
      }
    }
  }
  return ret;
}
//
// EqualRgn (GDI32.@)
//
// Tests whether one region is identical to another.
//
// PARAMS
// hrgn1 [I] The first region to compare.
// hrgn2 [I] The second region to compare.
//
// RETURNS
// Non-zero if both regions are identical or zero otherwise.
//
BOOL region_EqualRgn(region_t* obj1, region_t* obj2)
{
  int i;
  if (obj1->numRects != obj2->numRects
      || obj1->extents.l != obj2->extents.l
      || obj1->extents.r != obj2->extents.r
      || obj1->extents.t != obj2->extents.t
      || obj1->extents.b != obj2->extents.b
     ) {
    return FALSE;
  }
  for (i = 0; i < obj1->numRects; i++) {
    if (obj1->rects[i].l != obj2->rects[i].l
        || obj1->rects[i].r != obj2->rects[i].r
        || obj1->rects[i].t != obj2->rects[i].t
        || obj1->rects[i].b != obj2->rects[i].b) {
      return FALSE;
    }
  }
  return TRUE;
}
BOOL region_AddRect(region_t* obj, const IRECT* rect)
{
  BOOL ret;
  ret = region_UnionRectWithRegion(rect, obj);
  return ret;
}
// Region Intersection
//
//
// region_IntersectO
//
// Handle an overlapping band for region_Intersect.
//
// Results:
// None.
//
// Side Effects:
// Rectangles may be added to the region.
static BOOL region_IntersectO(region_t* pReg, IRECT* r1, IRECT* r1End,
    IRECT* r2, IRECT* r2End, int top, int bottom)
{
  int left, right;
  while ((r1 != r1End) && (r2 != r2End)) {
    left = MAX(r1->l, r2->l);
    right = MIN(r1->r, r2->r);
    // * If there's any overlap between the two rectangles, add that
    // * overlap to the new region.
    // * There's no need to check for subsumption because the only way
    // * such a need could arise is if some region has two rectangles
    // * right next to each other. Since that should never happen...
    //
    if (left < right) {
      if (!add_rect(pReg, left, top, right, bottom)) {
        return FALSE;
      }
    }
    // * Need to advance the pointers. Shift the one that extends
    // * to the right the least, since the other still has a chance to
    // * overlap with that region's next rectangle, if you see what I mean.
    //
    if (r1->r < r2->r) {
      r1++;
    }
    else if (r2->r < r1->r) {
      r2++;
    }
    else {
      r1++;
      r2++;
    }
  }
  return TRUE;
}
static BOOL region_RegionOp(
    region_t* destReg, /* Place to store result */
    const region_t* reg1, /* First region in operation */
    const region_t* reg2, /* 2nd region in operation */
    BOOL (*overlapFunc)(region_t*, IRECT*, IRECT*, IRECT*, IRECT*, int, int), /* Function to call for over-lapping bands */
    BOOL (*nonOverlap1Func)(region_t*, IRECT*, IRECT*, int, int), /* Function to call for non-overlapping bands in region 1 */
    BOOL (*nonOverlap2Func)(region_t*, IRECT*, IRECT*, int, int) /* Function to call for non-overlapping bands in region 2 */
);
// region_IntersectRegion
static BOOL region_IntersectRegion(region_t* newReg, const region_t* reg1, const region_t* reg2)
{
  /* check for trivial reject */
  if ((!(reg1->numRects)) || (!(reg2->numRects)) ||
      (!overlapping(&reg1->extents, &reg2->extents))) {
    newReg->numRects = 0;
  }
  else if (!region_RegionOp(newReg, reg1, reg2, region_IntersectO, NULL, NULL)) {
    return FALSE;
  }
  // Can't alter newReg's extents before we call miRegionOp because
  // it might be one of the source regions and miRegionOp depends
  // on the extents of those regions being the same. Besides, this
  // way there's no checking against rectangles that will be nuked
  // due to coalescing, so we have to examine fewer rectangles.
  //
  region_SetExtents(newReg);
  return TRUE;
}
// region_region_SetFrameRgn
//
// region_Set a region that is a frame around another region.
// Compute the intersection of the region moved in all 4 directions
// ( +x, -x, +y, -y) and subtract from the original.
// The result looks slightly better than in Windows :)
BOOL region_FrameRgn(region_t* destObj, region_t* srcObj, int x, int y)
{
  region_t tmprgn[1] = {0};
  BOOL bRet = FALSE;
  if (!srcObj) {
    return FALSE;
  }
  if (srcObj->numRects != 0) {
    if (!region_SetSize(tmprgn, srcObj->numRects)) {
      goto done;
    }
    if (!region_OffsetRegion(destObj, srcObj, -x, 0)) {
      goto done;
    }
    if (!region_OffsetRegion(tmprgn, srcObj, x, 0)) {
      goto done;
    }
    if (!region_IntersectRegion(destObj, destObj, tmprgn)) {
      goto done;
    }
    if (!region_OffsetRegion(tmprgn, srcObj, 0, -y)) {
      goto done;
    }
    if (!region_IntersectRegion(destObj, destObj, tmprgn)) {
      goto done;
    }
    if (!region_OffsetRegion(tmprgn, srcObj, 0, y)) {
      goto done;
    }
    if (!region_IntersectRegion(destObj, destObj, tmprgn)) {
      goto done;
    }
    if (!region_SubtractRegion(destObj, srcObj, destObj)) {
      goto done;
    }
    bRet = TRUE;
  }
done:
  region_free(tmprgn);
  if (destObj) {
    region_free(destObj);
  }
  region_free(srcObj);
  return bRet;
}
//
// region_boolopt (GDI32.@)
//
// Combines two regions with the specified operation and stores the result
// in the specified destination region.
//
// PARAMS
// hDest [I] The region that receives the combined result.
// hSrc1 [I] The first source region.
// hSrc2 [I] The second source region.
// mode [I] The way in which the source regions will be combined. See notes.
//
// RETURNS
// Success:
// NULLREGION - The new region is empty.
// SIMPLEREGION - The new region can be represented by one rectangle.
// COMPLEXREGION - The new region can only be represented by more than
// one rectangle.
// Failure: ERROR
//
// NOTES
// The two source regions can be the same region.
// The mode can be one of the following:
// | Rgn_AND - Intersection of the regions
// | Rgn_OR - Union of the regions
// | Rgn_XOR - Unions of the regions minus any intersection.
// | Rgn_DIFF - Difference (subtraction) of the regions.
//
int region_boolopt(region_t* destObj, const region_t* src1Obj, const region_t* src2Obj, boolopt opt)
{
  int result = ERROR;
  TRACE(" %p,%p -> %p mode=%x\n", src1Obj, src2Obj, destObj, opt);
  if (src1Obj) {
    TRACE("dump src1Obj:\n");
    if (TRACE_ON(region)) {
      region_DumpRegion(src1Obj);
    }
    if (opt == Rgn_COPY) {
      if (region_CopyRegion(destObj, src1Obj)) {
        result = region_get_type(destObj);
      }
    }
    else {
      if (src2Obj) {
        TRACE("dump src2Obj:\n");
        if (TRACE_ON(region)) {
          region_DumpRegion(src2Obj);
        }
        switch (opt) {
        case Rgn_AND:
          if (region_IntersectRegion(destObj, src1Obj, src2Obj)) {
            result = region_get_type(destObj);
          }
          break;
        case Rgn_OR:
          if (region_UnionRegion(destObj, src1Obj, src2Obj)) {
            result = region_get_type(destObj);
          }
          break;
        case Rgn_XOR:
          if (region_XorRegion(destObj, src1Obj, src2Obj)) {
            result = region_get_type(destObj);
          }
          break;
        case Rgn_DIFF:
          if (region_SubtractRegion(destObj, src1Obj, src2Obj)) {
            result = region_get_type(destObj);
          }
          break;
		  default:break;
        }
      }
    }
  }
  return result;
}

// region_MirrorRegion
static BOOL region_MirrorRegion(region_t* dst, region_t* src, int width)
{
  int i, start, end;
  IRECT extents;
  IRECT* rects = NULL;
  region_t tmp[1] = {0};
  region_SetSize(tmp, src->numRects);
  rects = tmp->rects;
  extents.l = width - src->extents.r;
  extents.r = width - src->extents.l;
  extents.t = src->extents.t;
  extents.b = src->extents.b;
  for (start = 0; start < src->numRects; start = end) {
    // find the end of the current band
    for (end = start + 1; end < src->numRects; end++) {
      if (src->rects[end].t != src->rects[end - 1].t) {
        break;
      }
    }
    for (i = 0; i < end - start; i++) {
      rects[start + i].l = width - src->rects[end - i - 1].r;
      rects[start + i].r = width - src->rects[end - i - 1].l;
      rects[start + i].t = src->rects[end - i - 1].t;
      rects[start + i].b = src->rects[end - i - 1].b;
    }
  }
  pfree(dst->rects);
  region_free(dst);
  *dst = *tmp;
  dst->rects = rects;
  dst->size = src->numRects;
  dst->numRects = src->numRects;
  dst->extents = extents;
  return TRUE;
}
// Poly Regions
#define LARGE_COORDINATE 0x7fffffff /* FIXME */
#define SMALL_COORDINATE 0x80000000

// Insert the given edge into the edge table.
// First we must find the correct bucket in the
// Edge table, then find the r slot in the
// bucket. Finally, we can insert it.
static void InsertEdgeInET(EdgeTable* ET, EdgeTableEntry* ETE,
    int scanline, ScanLineListBlock** SLLBlock, int* iSLLBlock)
{
  struct list* ptr;
  ScanLineList* pSLL, *pPrevSLL;
  ScanLineListBlock* tmpSLLBlock;
  // find the r bucket to put the edge into
  pPrevSLL = &ET->scanlines;
  pSLL = pPrevSLL->next;
  while (pSLL && (pSLL->scanline < scanline)) {
    pPrevSLL = pSLL;
    pSLL = pSLL->next;
  }
  // reassign pSLL (pointer to ScanLineList) if necessary
  if ((!pSLL) || (pSLL->scanline > scanline)) {
    if (*iSLLBlock > SLLSPERBLOCK - 1) {
      tmpSLLBlock = (ScanLineListBlock*)pmalloc(sizeof(ScanLineListBlock));
      if (!tmpSLLBlock) {
        WARN("Can't alloc SLLB\n");
        return;
      }
      (*SLLBlock)->next = tmpSLLBlock;
      tmpSLLBlock->next = NULL;
      *SLLBlock = tmpSLLBlock;
      *iSLLBlock = 0;
    }
    pSLL = &((*SLLBlock)->SLLs[(*iSLLBlock)++]);
    pSLL->next = pPrevSLL->next;
    list_init(&pSLL->edgelist);
    pPrevSLL->next = pSLL;
  }
  pSLL->scanline = scanline;
  // now insert the edge in the r bucket
  LIST_FOR_EACH(ptr, &pSLL->edgelist) {
    struct edge_table_entry* entry = LIST_ENTRY(ptr, struct edge_table_entry, entry);
    if (entry->bres.minor_axis >= ETE->bres.minor_axis) {
      break;
    }
  }
  list_add_before(ptr, &ETE->entry);
}
// This routine creates the edge table for scan converting polygons.
// The Edge Table (ET) looks like:
//
// EdgeTable
// --------
// | ymax | ScanLineLists
// |scanline|-->------------>-------------->...
// -------- |scanline| |scanline|
// |edgelist| |edgelist|
// --------- ---------
// | |
// | |
// V V
// list of ETEs list of ETEs
//
// where ETE is an EdgeTableEntry data structure,
// and there is one ScanLineList per scanline at
// which an edge is initially entered.
//
//
static void region_CreateEdgeTable(const int* Count, int nbpolygons, const IPOINT* pts, EdgeTable* ET,
    EdgeTableEntry* pETEs, ScanLineListBlock* pSLLBlock)
{
  const IPOINT* t, *b;
  const IPOINT* PrevPt, *CurrPt, *EndPt;
  int poly, count;
  int iSLLBlock = 0;
  int dy;
  /*
  * initialize the Edge Table.
  */
  ET->scanlines.next = NULL;
  ET->ymax = SMALL_COORDINATE;
  ET->ymin = LARGE_COORDINATE;
  pSLLBlock->next = NULL;
  EndPt = pts - 1;
  for (poly = 0; poly < nbpolygons; poly++) {
    count = Count[poly];
    EndPt += count;
    if (count < 2) {
      continue;
    }
    PrevPt = EndPt;
    // for each vertex in the array of points.
    // In this loop we are dealing with two vertices at
    // a time -- these make up one edge of the polygon.
    while (count--) {
      CurrPt = pts++;
      // find out which point is above and which is below.
      if (PrevPt->y > CurrPt->y) {
        b = PrevPt, t = CurrPt;
        pETEs->ClockWise = 0;
      }
      else {
        b = CurrPt, t = PrevPt;
        pETEs->ClockWise = 1;
      }
      // don't add horizontal edges to the Edge table.
      if (b->y != t->y) {
        pETEs->ymax = b->y - 1;
        // -1 so we don't get last scanline 
        // initialize integer edge algorithm
        dy = b->y - t->y;
        bres_init_polygon(dy, t->x, b->x, &pETEs->bres);
        InsertEdgeInET(ET, pETEs, t->y, &pSLLBlock, &iSLLBlock);
        if (PrevPt->y > ET->ymax) {
          ET->ymax = PrevPt->y;
        }
        if (PrevPt->y < ET->ymin) {
          ET->ymin = PrevPt->y;
        }
        pETEs++;
      }
      PrevPt = CurrPt;
    }
  }
}
// This routine moves EdgeTableEntries from the
// EdgeTable into the Active Edge Table,
// leaving them sorted by smaller x coordinate.
static void region_loadAET(struct list* AET, struct list* ETEs)
{
  struct edge_table_entry* ptr, *next, *entry;
  struct list* active;
  LIST_FOR_EACH_ENTRY_SAFE(ptr, next, ETEs, struct edge_table_entry, entry) {
    LIST_FOR_EACH(active, AET) {
      entry = LIST_ENTRY(active, struct edge_table_entry, entry);
      if (entry->bres.minor_axis >= ptr->bres.minor_axis) {
        break;
      }
    }
    list_remove(&ptr->entry);
    list_add_before(active, &ptr->entry);
  }
}
//
// region_computeWAET
//
// This routine links the AET by the
// nextWETE (winding EdgeTableEntry) link for
// use by the winding number rule. The final
// Active Edge Table (AET) might look something
// like:
//
// AET
// ---------- --------- ---------
// |ymax | |ymax | |ymax |
// | ... | |... | |... |
// |next |->|next |->|next |->...
// |nextWETE| |nextWETE| |nextWETE|
// --------- --------- ^--------
// | | |
// V-------------------> V---> ...
static void region_computeWAET(struct list* AET, struct list* WETE)
{
  struct edge_table_entry* active;
  int inside = 1;
  int isInside = 0;
  list_init(WETE);
  LIST_FOR_EACH_ENTRY(active, AET, struct edge_table_entry, entry) {
    if (active->ClockWise) {
      isInside++;
    }
    else {
      isInside--;
    }
    if ((!inside && !isInside) || (inside && isInside)) {
      list_add_tail(WETE, &active->winding_entry);
      inside = !inside;
    }
  }
}
// Just a simple insertion sort to sort the Active Edge Table.
static BOOL region_InsertionSort(struct list* AET)
{
  struct edge_table_entry* active, *next, *insert;
  BOOL changed = FALSE;
  LIST_FOR_EACH_ENTRY_SAFE(active, next, AET, struct edge_table_entry, entry) {
    LIST_FOR_EACH_ENTRY(insert, AET, struct edge_table_entry, entry) {
      if (insert == active) {
        break;
      }
      if (insert->bres.minor_axis > active->bres.minor_axis) {
        break;
      }
    }
    if (insert == active) {
      continue;
    }
    list_remove(&active->entry);
    list_add_before(&insert->entry, &active->entry);
    changed = TRUE;
  }
  return changed;
}

// Clean up our act.
static void region_FreeStorage(ScanLineListBlock* pSLLBlock)
{
  ScanLineListBlock* tmpSLLBlock;
  while (pSLLBlock) {
    tmpSLLBlock = pSLLBlock->next;
    pfree(pSLLBlock);
    pSLLBlock = tmpSLLBlock;
  }
}

// Create an array of rectangles from a list of points.
static BOOL region_PtsToRegion(struct point_block* FirstPtBlock, region_t* reg)
{
  IRECT* rects;
  IPOINT* pts;
  struct point_block* pb;
  int i;
  IRECT* extents;
  int numRects;
  extents = &reg->extents;
  for (pb = FirstPtBlock, numRects = 0; pb; pb = pb->next) {
    numRects += pb->count;
  }
  if (!region_SetSize(reg, numRects)) {
    return FALSE;
  }
  reg->size = numRects;
  rects = reg->rects - 1;
  numRects = 0;
  extents->l = LARGE_COORDINATE, extents->r = SMALL_COORDINATE;
  for (pb = FirstPtBlock; pb; pb = pb->next) {
    /* the loop uses 2 points per iteration */
    i = pb->count / 2;
    for (pts = pb->pts; i--; pts += 2) {
      if (pts->x == pts[1].x) {
        continue;
      }
      if (numRects && pts->x == rects->l && pts->y == rects->b &&
          pts[1].x == rects->r &&
          (numRects == 1 || rects[-1].t != rects->t) &&
          (i && pts[2].y > pts[1].y)) {
        rects->b = pts[1].y + 1;
        continue;
      }
      numRects++;
      rects++;
      rects->l = pts->x;
      rects->t = pts->y;
      rects->r = pts[1].x;
      rects->b = pts[1].y + 1;
      if (rects->l < extents->l) {
        extents->l = rects->l;
      }
      if (rects->r > extents->r) {
        extents->r = rects->r;
      }
    }
  }
  if (numRects) {
    extents->t = reg->rects->t;
    extents->b = rects->b;
  }
  else {
    extents->l = 0;
    extents->t = 0;
    extents->r = 0;
    extents->b = 0;
  }
  reg->numRects = numRects;
  return(TRUE);
}
// region_SetPolyPolygonRgn (GDI32.@)
int region_SetPolyPolygonRgn(region_t* obj, const IPOINT* Pts, const int* Count, int nbpolygons, int mode)
{
  int y; /* current scanline */
  struct list WETE, *pWETE; /* Winding Edge Table */
  ScanLineList* pSLL; /* current scanLineList */
  EdgeTable ET; /* header node for ET */
  struct list AET; /* header for AET */
  EdgeTableEntry* pETEs; /* EdgeTableEntries pool */
  ScanLineListBlock SLLBlock; /* header for scanlinelist */
  int fixWAET = FALSE;
  struct point_block FirstPtBlock, *block; /* PtBlock buffers */
  struct edge_table_entry* active, *next;
  int poly, total;
  TRACE("%p, count %d, polygons %d, mode %d\n", Pts, *Count, nbpolygons, mode);
  /* special case a rectangle */
  if (((nbpolygons == 1) && ((*Count == 4) ||
      ((*Count == 5) && (Pts[4].x == Pts[0].x) && (Pts[4].y == Pts[0].y)))) &&
      (((Pts[0].y == Pts[1].y) &&
          (Pts[1].x == Pts[2].x) &&
          (Pts[2].y == Pts[3].y) &&
          (Pts[3].x == Pts[0].x)) ||
          ((Pts[0].x == Pts[1].x) &&
              (Pts[1].y == Pts[2].y) &&
              (Pts[2].x == Pts[3].x) &&
              (Pts[3].y == Pts[0].y)))) {
    return region_SetRectRgn(obj, MIN(Pts[0].x, Pts[2].x), MIN(Pts[0].y, Pts[2].y), MAX(Pts[0].x, Pts[2].x), MAX(Pts[0].y, Pts[2].y));
  }
  for (poly = total = 0; poly < nbpolygons; poly++) {
    total += Count[poly];
  }
  if (!(pETEs = (EdgeTableEntry*)pmalloc(sizeof(EdgeTableEntry) * total))) {
    return 0;
  }
  region_CreateEdgeTable(Count, nbpolygons, Pts, &ET, pETEs, &SLLBlock);
  list_init(&AET);
  pSLL = ET.scanlines.next;
  block = &FirstPtBlock;
  FirstPtBlock.count = 0;
  FirstPtBlock.next = NULL;
  if (mode != WINDING) {
    // for each scanline
    for (y = ET.ymin; y < ET.ymax; y++) {
      // Add a new edge to the active edge table when we
      // get to the next edge.
      if (pSLL != NULL && y == pSLL->scanline) {
        region_loadAET(&AET, &pSLL->edgelist);
        pSLL = pSLL->next;
      }
      LIST_FOR_EACH_ENTRY_SAFE(active, next, &AET, struct edge_table_entry, entry) {
        block = add_point(block, active->bres.minor_axis, y);
        if (!block) {
          goto done;
        }
        if (active->ymax == y) { /* leaving this edge */
          list_remove(&active->entry);
        }
        else {
          bres_incr_polygon(&active->bres);
        }
      }
      region_InsertionSort(&AET);
    }
  }
  else {
    // for each scanline
    for (y = ET.ymin; y < ET.ymax; y++) {
      // Add a new edge to the active edge table when we get to the next edge.
      if (pSLL != NULL && y == pSLL->scanline) {
        region_loadAET(&AET, &pSLL->edgelist);
        region_computeWAET(&AET, &WETE);
        pSLL = pSLL->next;
      }
      pWETE = list_head(&WETE);
      // for each active edge
      LIST_FOR_EACH_ENTRY_SAFE(active, next, &AET, struct edge_table_entry, entry) {
        // add to the buffer only those edges that are in the Winding active edge table.
        if (pWETE == &active->winding_entry) {
          block = add_point(block, active->bres.minor_axis, y);
          if (!block) {
            goto done;
          }
          pWETE = list_next(&WETE, pWETE);
        }
        if (active->ymax == y) { /* leaving this edge */
          list_remove(&active->entry);
          fixWAET = TRUE;
        }
        else {
          bres_incr_polygon(&active->bres);
        }
      }
      // recompute the winding active edge table if we just resorted or have exited an edge.
      if (region_InsertionSort(&AET) || fixWAET) {
        region_computeWAET(&AET, &WETE);
        fixWAET = FALSE;
      }
    }
  }
  if (!region_PtsToRegion(&FirstPtBlock, obj)) {
    pfree(obj);
    goto done;
  }
done:
  region_FreeStorage(SLLBlock.next);
  free_point_blocks(FirstPtBlock.next);
  pfree(pETEs);
  return 0;
}
// region_SetPolygonRgn (GDI32.@)
int region_SetPolygonRgn(region_t* obj, const IPOINT* points, int count, int mode)
{
  return region_SetPolyPolygonRgn(obj, points, &count, 1, mode);
}
int region_set_poly(region_t* obj, const FPOINT* Pts, const int* Count, int nbpolygons, int mode)
{
  IPOINT* ipt = NULL;
  int ret, i, n = 0;
  for (i = 0; i < nbpolygons; ++i) {
    n += Count[i];
  }
  ipt = (IPOINT*)pmalloc(n * sizeof(IPOINT));
  for (i = 0; i < n; ++i) {
    ipt[i].x = (int)(Pts[i].x + 0.5);
    ipt[i].y = (int)(Pts[i].y + 0.5);
  }
  ret = region_SetPolyPolygonRgn(obj, ipt, Count, nbpolygons, mode);
  pfree(ipt);
  return ret;
}
// region_SetPolygonRgn (GDI32.@)
int region_set_poly1(region_t* obj, const FPOINT* points, int count, int mode)
{
  return region_set_poly(obj, points, &count, 1, mode);
}
int region_set_path(region_t* obj, const FPOINT* Points, const BYTE* Types, int Count, int mode)
{
  int* plen = NULL;
  int nlen = GetPathLens(Types, Count, NULL);
  MYREALLOC(plen, nlen);
  nlen = GetPathLens(Types, Count, plen);
  region_set_poly(obj, Points, plen, nlen, mode);
  FREE(plen);
  return 0;
}
#endif // _REGION_INL_
