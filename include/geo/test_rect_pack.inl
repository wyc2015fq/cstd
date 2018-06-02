// Rectangle Packing,（合并多个图片，并使面积最小）
//原文：http://www.iguanademos.com/Jare/Articles.php?view=RectPlace
// ----------------------------------------------------------------------------------------
// Name : RectPlacement.cpp
// Description : A class that fits subrectangles into a power-of-2 rectangle
// (C) Copyright 2000-2002 by Javier Arevalo
// This code is pfree to use and modify for all purposes
// ----------------------------------------------------------------------------------------
// You have a bunch of rectangular pieces. You need to arrange them in a
// rectangular surface so that they don't overlap, keeping the total area of the
// rectangle as small as possible. This is fairly common when arranging characters
// in a bitmapped font, lightmaps for a 3D engine, and I guess other situations as
// well.
//
// The idea of this algorithm is that, as we add rectangles, we can pre-select
// "interesting" places where we can try to add the next rectangles. For optimal
// results, the rectangles should be added in order. I initially tried using area
// as a sorting criteria, but it didn't work well with very tall or very flat
// rectangles. I then tried using the longest dimension as a selector, and it
// worked much better. So much for intuition...
//
// These "interesting" places are just to the right and just below the currently
// added rectangle. The first rectangle, obviously, goes at the top left, the next
// one would go either to the right or below this one, and so on. It is a weird way
// to do it, but it seems to work very nicely.
//
// The way we search here is fairly brute-force, the fact being that for most off-
// line purposes the performance seems more than adequate. I have generated a
// japanese font with around 8500 characters and all the time was spent generating
// the bitmaps.
//
// Also, for all we care, we could grow the parent rectangle in a different way
// than power of two. It just happens that power of 2 is very convenient for
// graphics hardware textures.
//
// I'd be interested in hearing of other approaches to this problem. Make sure
// to post them on http://www.flipcode.com
// --------------------------------------------------------------------------------
#include "cstd.h"
// Helper classes
typedef IPOINT TPos;
struct TRect {
  int x, y;
  int w, h;
  int n; // Original index of this subrect, before sorting
  int nTex; // Texture in which this subrect will be placed.
};
typedef TRect TSubRect;
void tRectSet(TRect* t, int _x, int _y, int _w, int _h)
{
  t->w = (_w > 0 ? _w : 0);
  t->h = (_h > 0 ? _h : 0);
  t->x = _x;
  t->y = _y;
}
void tSubRect(TRect* t, int _w, int _h, int _n)
{
  tRectSet(t, 0, 0, _w, _h);
  t->n = (_n), t->nTex = (0);
}
bool Contains(const TRect* s, const TPos& p)
{
  return (p.x >= s->x && p.y >= s->y && p.x < (s->x + s->w) && p.y < (s->y + s->h));
}
bool Contains(const TRect* s, const TRect& r)
{
  return (r.x >= s->x && r.y >= s->y && (r.x + r.w) <= (s->x + s->w) && (r.y + r.h) <= (s->y + s->h));
}
bool Intersects(const TRect* s, const TRect& r)
{
  return s->w > 0 && s->h > 0 && r.w > 0 && r.h > 0 &&
      ((r.x + r.w) > s->x && r.x < (s->x + s->w) &&
          (r.y + r.h) > s->y && r.y < (s->y + s->h));
}
// Greater rect area. Not as good as the next heuristic
// static bool Greater(const TRect &a, const TRect &b) { return a.w*a.h > b.w*b.h; }
// Greater size in at least one dim.
static bool Greater(const TRect& a, const TRect& b)
{
  return (a.w > b.w && a.w > b.h) ||
      (a.h > b.w && a.h > b.h);
}
// --------------------------------------------------------------------------------
typedef struct {
  TRect m_size;
  TRect* m_vRects;
  int m_vRects_num;
  TPos* m_vPositions;
  int m_vPositions_num;
  long m_area;
} CRectPlacement;
bool CRectPlacement_IsOk(CRectPlacement* s)
{
  return s->m_size.w > 0;
}
int CRectPlacement_GetW(CRectPlacement* s)
{
  return s->m_size.w;
}
int CRectPlacement_GetH(CRectPlacement* s)
{
  return s->m_size.h;
}
long CRectPlacement_GetArea(CRectPlacement* s)
{
  return s->m_area;
}
long CRectPlacement_GetTotalArea(CRectPlacement* s)
{
  return s->m_size.w * s->m_size.h;
}
void CRectPlacement_End(CRectPlacement* s)
{
  FREE(s->m_vPositions);
  FREE(s->m_vRects);
  s->m_size.w = 0;
}
void CRectPlacement_Init(CRectPlacement* s, int w, int h)
{
  tRectSet(&s->m_size, 0, 0, w, h);
  *(TPos*)ImGui_AllocPush(s->m_vPositions, s->m_vPositions_num, 1) = (iPOINT(0, 0));
  s->m_area = 0;
}
// --------------------------------------------------------------------------------
// Name : IsFree
// Description : Check if the given rectangle is partially or totally used
// --------------------------------------------------------------------------------
bool CRectPlacement_IsFree(CRectPlacement* s, const TRect& r)
{
  int i;
  if (!Contains(&s->m_size, r)) {
    return false;
  }
  for (i = 0; i < s->m_vRects_num; ++i) {
    if (Intersects(s->m_vRects + i, r)) {
      return false;
    }
  }
  return true;
}
// --------------------------------------------------------------------------------
// Name : AddPosition
// Description : Add new anchor point
// --------------------------------------------------------------------------------
void CRectPlacement_AddPosition(CRectPlacement* s, TPos p)
{
  // Try to insert anchor as close as possible to the top left corner
  // So it will be tried first
  bool bFound = false;
  int i;
  TPos t;
  *(TPos*)ImGui_AllocPush(s->m_vPositions, s->m_vPositions_num, 1) = p;
  for (i = s->m_vPositions_num - 2; !bFound && (i >= 0); --i) {
    if (p.x + p.y < s->m_vPositions[i].x + s->m_vPositions[i].y) {
      CC_SWAP(s->m_vPositions[i], s->m_vPositions[i + 1], t);
    }
  }
}
// --------------------------------------------------------------------------------
// Name : AddRect
// Description : Add the given rect and updates anchor points
// --------------------------------------------------------------------------------
void CRectPlacement_AddRect(CRectPlacement* s, const TRect& r)
{
  *(TRect*)ImGui_AllocPush(s->m_vRects, s->m_vRects_num, 1) = r;
  s->m_area += r.w * r.h;
  // Add two new anchor points
  CRectPlacement_AddPosition(s, iPOINT(r.x, r.y + r.h));
  CRectPlacement_AddPosition(s, iPOINT(r.x + r.w, r.y));
}
// --------------------------------------------------------------------------------
// Name : AddAtEmptySpot
// Description : Add the given rectangle
// --------------------------------------------------------------------------------
bool CRectPlacement_AddAtEmptySpot(CRectPlacement* s, TRect& r)
{
  int x, y;
  // Find a valid spot among available anchors.
  bool bFound = false;
  int i;
  TRect Rect = {0}, rt;
  for (i = 0; !bFound && i < s->m_vPositions_num; ++i) {
    tRectSet(&Rect, s->m_vPositions[i].x, s->m_vPositions[i].y, r.w, r.h);
    if (CRectPlacement_IsFree(s, Rect)) {
      r = Rect;
      bFound = true;
      break; // Don't let the loop increase the iterator.
    }
  }
  if (bFound) {
    // Remove the used anchor point
    //m_vPositions.erase(i);
    --s->m_vPositions_num;
    MEMCPY(s->m_vPositions + i, s->m_vPositions + i + 1, s->m_vPositions_num - i);
    // Sometimes, anchors end up displaced from the optimal position
    // due to irregular sizes of the subrects.
    // So, try to adjut it up & left as much as possible.
    for (x = 1; x <= r.x; x++) {
      tRectSet(&rt, r.x - x, r.y, r.w, r.h);
      if (!CRectPlacement_IsFree(s, rt)) {
        break;
      }
    }
    for (y = 1; y <= r.y; y++) {
      tRectSet(&rt, r.x, r.y - y, r.w, r.h);
      if (!CRectPlacement_IsFree(s, rt)) {
        break;
      }
    }
    if (y > x) {
      r.y -= y - 1;
    }
    else {
      r.x -= x - 1;
    }
    CRectPlacement_AddRect(s, r);
  }
  return bFound;
}
// --------------------------------------------------------------------------------
// Name : AddAtEmptySpotAutoGrow
// Description : Add a rectangle of the given size, growing our area if needed
// Area grows only until the max given.
// Returns the placement of the rect in the rect's x,y coords
// --------------------------------------------------------------------------------
bool CRectPlacement_AddAtEmptySpotAutoGrow(CRectPlacement* s, TRect* pRect, int maxW, int maxH)
{
  if (pRect->w <= 0) {
    return true;
  }
  int orgW = s->m_size.w;
  int orgH = s->m_size.h;
  // Try to add it in the existing space
  while (!CRectPlacement_AddAtEmptySpot(s, *pRect)) {
    int pw = s->m_size.w;
    int ph = s->m_size.h;
    // Sanity check - if area is complete.
    if (pw >= maxW && ph >= maxH) {
      s->m_size.w = orgW;
      s->m_size.h = orgH;
      return false;
    }
    // Try growing the smallest dim
    if (pw < maxW && (pw < ph || ((pw == ph) && (pRect->w >= pRect->h)))) {
      s->m_size.w = pw * 2;
    }
    else {
      s->m_size.h = ph * 2;
    }
    if (CRectPlacement_AddAtEmptySpot(s, *pRect)) {
      break;
    }
    // Try growing the other dim instead
    if (pw != s->m_size.w) {
      s->m_size.w = pw;
      if (ph < maxW) {
        s->m_size.h = ph * 2;
      }
    }
    else {
      s->m_size.h = ph;
      if (pw < maxW) {
        s->m_size.w = pw * 2;
      }
    }
    if (pw != s->m_size.w || ph != s->m_size.h) {
      if (CRectPlacement_AddAtEmptySpot(s, *pRect)) {
        break;
      }
    }
    // Grow both if possible, and reloop.
    s->m_size.w = pw;
    s->m_size.h = ph;
    if (pw < maxW) {
      s->m_size.w = pw * 2;
    }
    if (ph < maxH) {
      s->m_size.h = ph * 2;
    }
  }
  return true;
}
// ----------------------------------------------------------------------------------------
// Name : Test.cpp
// Description : Test the RectPlacement class
// (C) Copyright 2000-2002 by Javier Arevalo
// This code is pfree to use and modify for all purposes
// ----------------------------------------------------------------------------------------
#include <stdio.h>
#include <algorithm>
// --------------------------------------------------------------------------------
// Name : CreateTextures
// Description : Create array of textures containing all subrects
// --------------------------------------------------------------------------------
int CreateTextures(CRectPlacement* vecTextures, int vecTextures_maxnum, TSubRect* vecSubRects, int vecSubRects_num, int maxTexW, int maxTexH)
{
  int i, j = 0;
  int vecTextures_num = 0;
  // Sort the subRects based on dimensions, larger dimension goes first.
  std::sort(vecSubRects, vecSubRects + vecSubRects_num, Greater);
  // Generate the first texture
  TSubRect* it = vecSubRects;
  TSubRect* it_end = vecSubRects + vecSubRects_num;
  // Add all subrects
  CRectPlacement_Init(vecTextures + vecTextures_num++, 1, 1);
  for (; it != it_end && vecTextures_num < vecTextures_maxnum; ++it, ++j) {
    // We make sure we leave one pixel between subrects, so texels don't bleed with bilinear.
    TRect r = {0};
    tRectSet(&r, 0, 0, it->w + 1, it->h + 1);
    // If an existing texture has actual space
    bool bPlaced = false;
    for (i = 0; !bPlaced && i < vecTextures_num; i++) {
      bPlaced = CRectPlacement_AddAtEmptySpotAutoGrow(vecTextures + i, &r, maxTexW, maxTexH);
      if (bPlaced) {
        it->nTex = i;
      }
    }
    // Try starting a new texture and fit the rect in there
    if (!bPlaced && vecTextures_num < vecTextures_maxnum) {
      CRectPlacement_Init(vecTextures + vecTextures_num++, 1, 1);
      bPlaced = CRectPlacement_AddAtEmptySpotAutoGrow(vecTextures + vecTextures_num - 1, &r, maxTexW, maxTexH);
      if (bPlaced) {
        it->nTex = vecTextures_num - 1;
      }
      else {
        printf("ERROR: Subrect is too big to fit in texture!", it->w, it->h);
      }
    }
    // If correctly placed in a texture, the coords are returned in r.x and r.y
    // Store them.
    if (bPlaced) {
      it->x = r.x;
      it->y = r.y;
    }
  }
  return vecTextures_num;
}
#include "ui/window.inl"
#include "draw/imdraw.inl"
int font_bit_print(int c, const font_bit_t* bit)
{
  int i, j;
  char aa[3] = {0};
  char* ss[] = {" ", "#" };
  if (c < 128) {
    aa[0] = c & 0xff;
  }
  else {
    aa[1] = c & 0xff;
    aa[0] = c >> 8;
  }
  printf("c = %s\n", aa);
  for (i = 0; i < bit->by; ++i) {
    uchar* ptr = bit->bit + i * bit->step;
    for (j = 0; j < bit->x; ++j) {
      //printf(ss[0]);
    }
    for (j = 0; j < bit->bx; ++j) {
      printf("%s", ss[((ptr[j >> 3] >> (7 - (j & 7))) & 1)]);
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}
int test_rect_pack_font()
{
  font_t fo[1] = {0};
  int i, n = 0;
  font_bit_t bit[1] = {0};
  font_init_memraster(fo, "songti", songti_12);
  for (i = 255; i < (1 << 16); ++i) {
    if (fo->getbit(fo, i, bit)) {
      int sdf = 0;
      //font_bit_print(i, bit);
      ++n;
    }
  }
  //k = sqrt(12*12*);
  return 0;
}
#include "img/imgio.inl"
#include "codec/huff.inl"
#include "img/fmt/lzw.inl"
#include "geo/test_packing.inl"
#define TSubRect_Greater()
int rect_pack(int n, TSubRect* rects, int max_w)
{
  //int i;
  //QSORT(rects, rects+n, _P, _Ty);
  //for (i=0; i<n; ++i) {}
  return 0;
}
int test_rect_pack()
{
  enum {
  };
  CRectPlacement vecTextures[10] = {0};
  TSubRect* vecSubRects = NULL;
  int NUM_SUBRECTS = 500;
  int MAX_TEXTURE_W = 512;
  int MAX_TEXTURE_H = 512;
  img_t im[10] = {0};
  int i, n = 0;
  int vecSubRects_num = 0;
  int vecTextures_num = 0;
  if (1) {
    test_packing();
    return 0;
  }
  if (0) {
    int MIN_SUBRECT = 15;
    int MAX_SUBRECT = 50;
    vecSubRects_num = 500;
    MYREALLOC(vecSubRects, vecSubRects_num);
    // Let's just fill this with random stuff
    for (i = 0; i < vecSubRects_num; i++) {
      tSubRect(vecSubRects + i, (rand() % MAX_SUBRECT) + MIN_SUBRECT, (rand() % MAX_SUBRECT) + MIN_SUBRECT, i);
    }
  }
  if (1) {
    font_t fo[1] = {0};
    int i, n = 0;
    font_bit_t bit[1] = {0};
    vecSubRects_num = 1500;
    font_init_memraster(fo, "songti", songti_12);
    MYREALLOC(vecSubRects, vecSubRects_num);
    for (i = 255; i < (1 << 16) && n < NUM_SUBRECTS; ++i) {
      if (fo->getbit(fo, i, bit)) {
        int sdf = 0;
        //font_bit_print(i, bit);
        tSubRect(vecSubRects + n, bit->bx, bit->by, n);
        ++n;
      }
    }
  }
  for (i = 0; i < 10; ++i) {
    imsetsize(im + i, MAX_TEXTURE_H, MAX_TEXTURE_W, 4, 1);
    imdraw_fill(im + i, Color_White);
  }
  vecTextures_num = CreateTextures(vecTextures, countof(vecTextures), vecSubRects, vecSubRects_num, MAX_TEXTURE_W, MAX_TEXTURE_H);
  {
    TSubRect* it = vecSubRects;
    TSubRect* it_end = vecSubRects + vecSubRects_num;
    for (; it != it_end; ++it) {
      printf("Subrect %d (originally %d), size %dx%d, goes into texture %d at pos %d,%d\n",
          it - vecSubRects, it->n, it->w, it->h, it->nTex, it->x, it->y);
      imdraw_rect(im + it->nTex, 0, iRECT2(it->x, it->y, it->w, it->h), 0, Color_Red, 1, 0);
      n = MAX(n, it->nTex + 1);
    }
  }
  printf("Created a total of %d textures\n", vecTextures_num);
  {
    CRectPlacement* it = vecTextures;
    CRectPlacement* it_end = vecTextures + vecTextures_num;
    for (; it != it_end; ++it) {
      printf(" Texture %d, size %dx%d, Coverage %d / %d (%d%%)\n",
          it - vecTextures, CRectPlacement_GetW(it), CRectPlacement_GetH(it),
          CRectPlacement_GetArea(it), CRectPlacement_GetTotalArea(it), CRectPlacement_GetArea(it) * 100 / CRectPlacement_GetTotalArea(it));
    }
  }
  if (1) {
  }
  imshow(im + 0);
  imshow(im + 1);
  imshow(im + 2);
  imshow(im + 3);
  cvWaitKey(-1);
  return 0;
}

