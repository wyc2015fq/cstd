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
#include <vector>
// Helper classes
typedef IPOINT TPos;
struct TRect {
  int x, y;
  int w, h;
  int n; // Original index of this subrect, before sorting
  int nTex; // Texture in which this subrect will be placed.
};
typedef TRect TSubRect;
TRect tRect(int _x, int _y, int _w, int _h)
{
  TRect t;
  t.w = (_w > 0 ? _w : 0), t.h = (_h > 0 ? _h : 0);
  t.x = _x;
  t.y = _y;
  t.n = 0;
  t.nTex = 0;
  return t;
}
TSubRect tSubRect(int _w, int _h, int _n)
{
  TSubRect t;
  t = tRect(0, 0, _w, _h);
  t.n = (_n), t.nTex = (0);
  return t;
}
bool Contains(const TRect* s, const TPos& p)
{
  return (p.x >= s->x && p.y >= s->y &&
      p.x < (s->x + s->w) && p.y < (s->y + s->h));
}
bool Contains(const TRect* s, const TRect& r)
{
  return (r.x >= s->x && r.y >= s->y &&
      (r.x + r.w) <= (s->x + s->w) && (r.y + r.h) <= (s->y + s->h));
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
class CRectPlacement
{
public:
  // ---------------------
  typedef std::vector<TPos> CPosArray;
  typedef std::vector<TRect> CRectArray;
  // ---------------------
  CRectPlacement() {
    Init();
  }
  ~CRectPlacement() {
    End();
  }
  void Init(int w = 1, int h = 1);
  void End();
  bool IsOk() const {
    return m_size.w > 0;
  }
  int GetW() const {
    return m_size.w;
  }
  int GetH() const {
    return m_size.h;
  }
  long GetArea() const {
    return m_area;
  }
  long GetTotalArea() const {
    return m_size.w * m_size.h;
  }
  bool AddAtEmptySpotAutoGrow(TRect* pRect, int maxW, int maxH);
private:
  TRect m_size;
  CRectArray m_vRects;
  CPosArray m_vPositions;
  long m_area;
  // ---------------------
  bool IsFree(const TRect& r) const;
  void AddPosition(const TPos& p);
  void AddRect(const TRect& r);
  bool AddAtEmptySpot(TRect& r);
};
void CRectPlacement::Init(int w, int h)
{
  End();
  m_size = tRect(0, 0, w, h);
  m_vPositions.push_back(iPOINT(0, 0));
  m_area = 0;
}
// --------------------------------------------------------------------------------
// Name :
// Description :
// --------------------------------------------------------------------------------
void CRectPlacement::End()
{
  m_vPositions.clear();
  m_vRects.clear();
  m_size.w = 0;
}
// --------------------------------------------------------------------------------
// Name : IsFree
// Description : Check if the given rectangle is partially or totally used
// --------------------------------------------------------------------------------
bool CRectPlacement::IsFree(const TRect& r) const
{
  if (!Contains(&m_size, r)) {
    return false;
  }
  for (CRectArray::const_iterator it = m_vRects.begin(); it != m_vRects.end(); ++it) {
    if (Intersects(it, r)) {
      return false;
    }
  }
  return true;
}
// --------------------------------------------------------------------------------
// Name : AddPosition
// Description : Add new anchor point
// --------------------------------------------------------------------------------
void CRectPlacement::AddPosition(const TPos& p)
{
  // Try to insert anchor as close as possible to the top left corner
  // So it will be tried first
  bool bFound = false;
  CPosArray::iterator it;
  for (it = m_vPositions.begin();
      !bFound && it != m_vPositions.end();
      ++it) {
    if (p.x + p.y < it->x + it->y) {
      bFound = true;
    }
  }
  if (bFound) {
    m_vPositions.insert(it, p);
  }
  else {
    m_vPositions.push_back(p);
  }
}
// --------------------------------------------------------------------------------
// Name : AddRect
// Description : Add the given rect and updates anchor points
// --------------------------------------------------------------------------------
void CRectPlacement::AddRect(const TRect& r)
{
  m_vRects.push_back(r);
  m_area += r.w * r.h;
  // Add two new anchor points
  AddPosition(iPOINT(r.x, r.y + r.h));
  AddPosition(iPOINT(r.x + r.w, r.y));
}
// --------------------------------------------------------------------------------
// Name : AddAtEmptySpot
// Description : Add the given rectangle
// --------------------------------------------------------------------------------
bool CRectPlacement::AddAtEmptySpot(TRect& r)
{
  int x, y;
  // Find a valid spot among available anchors.
  bool bFound = false;
  CPosArray::iterator it;
  for (it = m_vPositions.begin(); !bFound && it != m_vPositions.end(); ++it) {
    TRect Rect = tRect(it->x, it->y, r.w, r.h);
    if (IsFree(Rect)) {
      r = Rect;
      bFound = true;
      break; // Don't let the loop increase the iterator.
    }
  }
  if (bFound) {
    // Remove the used anchor point
    m_vPositions.erase(it);
    // Sometimes, anchors end up displaced from the optimal position
    // due to irregular sizes of the subrects.
    // So, try to adjut it up & left as much as possible.
    for (x = 1; x <= r.x; x++) {
      if (!IsFree(tRect(r.x - x, r.y, r.w, r.h))) {
        break;
      }
    }
    for (y = 1; y <= r.y; y++) {
      if (!IsFree(tRect(r.x, r.y - y, r.w, r.h))) {
        break;
      }
    }
    if (y > x) {
      r.y -= y - 1;
    }
    else {
      r.x -= x - 1;
    }
    AddRect(r);
  }
  return bFound;
}
// --------------------------------------------------------------------------------
// Name : AddAtEmptySpotAutoGrow
// Description : Add a rectangle of the given size, growing our area if needed
// Area grows only until the max given.
// Returns the placement of the rect in the rect's x,y coords
// --------------------------------------------------------------------------------
bool CRectPlacement::AddAtEmptySpotAutoGrow(TRect* pRect, int maxW, int maxH)
{
  if (pRect->w <= 0) {
    return true;
  }
  int orgW = m_size.w;
  int orgH = m_size.h;
  // Try to add it in the existing space
  while (!AddAtEmptySpot(*pRect)) {
    int pw = m_size.w;
    int ph = m_size.h;
    // Sanity check - if area is complete.
    if (pw >= maxW && ph >= maxH) {
      m_size.w = orgW;
      m_size.h = orgH;
      return false;
    }
    // Try growing the smallest dim
    if (pw < maxW && (pw < ph || ((pw == ph) && (pRect->w >= pRect->h)))) {
      m_size.w = pw * 2;
    }
    else {
      m_size.h = ph * 2;
    }
    if (AddAtEmptySpot(*pRect)) {
      break;
    }
    // Try growing the other dim instead
    if (pw != m_size.w) {
      m_size.w = pw;
      if (ph < maxW) {
        m_size.h = ph * 2;
      }
    }
    else {
      m_size.h = ph;
      if (pw < maxW) {
        m_size.w = pw * 2;
      }
    }
    if (pw != m_size.w || ph != m_size.h)
      if (AddAtEmptySpot(*pRect)) {
        break;
      }
    // Grow both if possible, and reloop.
    m_size.w = pw;
    m_size.h = ph;
    if (pw < maxW) {
      m_size.w = pw * 2;
    }
    if (ph < maxH) {
      m_size.h = ph * 2;
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
// --------------------------------------------------------------------------------
typedef std::vector<TSubRect> CSubRectArray;
typedef std::vector<CRectPlacement> CTextureArray;
// --------------------------------------------------------------------------------
// Name : CreateTextures
// Description : Create array of textures containing all subrects
// --------------------------------------------------------------------------------
void CreateTextures(CTextureArray& vecTextures, CSubRectArray& vecSubRects, int maxTexW, int maxTexH)
{
  int j = 0;
  // Sort the subRects based on dimensions, larger dimension goes first.
  std::sort(vecSubRects.begin(), vecSubRects.end(), Greater);
  // Generate the first texture
  vecTextures.clear();
  vecTextures.push_back(CRectPlacement());
  // Add all subrects
  for (CSubRectArray::iterator it = vecSubRects.begin(); it != vecSubRects.end(); ++it, ++j) {
    // We make sure we leave one pixel between subrects, so texels don't bleed with bilinear.
    TRect r = tRect(0, 0, it->w + 1, it->h + 1);
    // If an existing texture has actual space
    bool bPlaced = false;
    for (int i = 0; !bPlaced && i < vecTextures.size(); i++) {
      bPlaced = vecTextures[i].AddAtEmptySpotAutoGrow(&r, maxTexW, maxTexH);
      if (bPlaced) {
        it->nTex = i;
      }
    }
    // Try starting a new texture and fit the rect in there
    if (!bPlaced) {
      vecTextures.push_back(CRectPlacement());
      bPlaced = vecTextures[vecTextures.size() - 1].AddAtEmptySpotAutoGrow(&r, maxTexW, maxTexH);
      if (bPlaced) {
        it->nTex = vecTextures.size() - 1;
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
}
#include "ui/window.inl"
#include "draw/imdraw.inl"
raster_font_t songti_12[] = {
#include "font/songti_12.txt"
};
// --------------------------------------------------------------------------------
// Name : main
// Description : heh
// --------------------------------------------------------------------------------
int test_rect_packing()
{
#define MAX_TEXTURE_W 512
#define MAX_TEXTURE_H 512
#define MIN_SUBRECT 15
#define MAX_SUBRECT 50
  CTextureArray vecTextures;
  CSubRectArray vecSubRects;
  img_t im[10] = {0};
  int i, n = 0;
  for (i = 0; i < 10; ++i) {
    imsetsize(im + i, MAX_TEXTURE_H, MAX_TEXTURE_W, 4, 1);
    imdraw_fill(im + i, Color_White);
  }
  if (0) {
    int NUM_SUBRECTS = 500;
    vecSubRects.clear();
    // Let's just fill this with random stuff
    for (int i = 0; i < NUM_SUBRECTS; i++)
      vecSubRects.push_back(tSubRect((rand() % MAX_SUBRECT) + MIN_SUBRECT,
          (rand() % MAX_SUBRECT) + MIN_SUBRECT, i));
  }
  if (1) {
    int NUM_SUBRECTS = 1500;
    font_t fo[1] = {0};
    int i, n = 0;
    font_bit_t bit[1] = {0};
    //vecSubRects_num = 2500;
    font_init_memraster(fo, "songti", songti_12);
    //MYREALLOC(vecSubRects, vecSubRects_num);
    for (i = 255; i < (1 << 16) && n < NUM_SUBRECTS; ++i) {
      if (fo->getbit(fo, i, bit)) {
        int sdf = 0;
        //font_bit_print(i, bit);
        vecSubRects.push_back(tSubRect(bit->bx, bit->by, n));
        ++n;
      }
    }
  }
  CreateTextures(vecTextures, vecSubRects, MAX_TEXTURE_W, MAX_TEXTURE_H);
  {
    for (CSubRectArray::const_iterator it = vecSubRects.begin(); it != vecSubRects.end(); ++it) {
      printf("Subrect %d (originally %d), size %dx%d, goes into texture %d at pos %d,%d\n",
          it - vecSubRects.begin(), it->n, it->w, it->h, it->nTex, it->x, it->y);
      imdraw_rect(im + it->nTex, 0, iRECT2(it->x, it->y, it->w, it->h), 0, Color_Red, 1, 0);
      n = MAX(n, it->nTex + 1);
    }
  }
  printf("Created a total of %d textures\n", vecTextures.size());
  {
    for (CTextureArray::const_iterator it = vecTextures.begin(); it != vecTextures.end(); ++it) {
      printf(" Texture %d, size %dx%d, Coverage %d / %d (%d%%)\n",
          it - vecTextures.begin(), it->GetW(), it->GetH(),
          it->GetArea(), it->GetTotalArea(), it->GetArea() * 100 / it->GetTotalArea());
    }
  }
  if (1) {
  }
  imshow(im + 0);
  imshow(im + 1);
  imshow(im + 2);
  //imshow(im+3);
  cvWaitKey(-1);
  return 0;
}

