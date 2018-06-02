

#ifndef MAX
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )
#endif

#ifndef MIN
#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#endif

#define use_deprecated
#define use_int32

#define CLIPPER_VERSION "6.1.3"

#include <cmath>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <ostream>
#include <functional>


enum ClipType { ctIntersection, ctUnion, ctDifference, ctXor };
enum PolyType { ptSubject, ptClip };
//By far the most widely used winding rules for polygon filling are
//EvenOdd & NonZero (GDI, GDI+, XLib, OpenGL, Cairo, AGG, Quartz, SVG, Gr32)
//Others rules include Positive, Negative and ABS_GTR_EQ_TWO (only in OpenGL)
//see http://glprogramming.com/red/chapter11.html
enum PolyFillType { pftEvenOdd, pftNonZero, pftPositive, pftNegative };


#ifdef use_int32
typedef int cInt;
typedef unsigned int cUInt;
static cInt const loRange = 46340;
static cInt const hiRange = 46340;
#else
typedef signed __int64 cInt;
typedef unsigned __int64 cUInt;
static cInt const loRange = 0x3FFFFFFF;
static cInt const hiRange = 0x3FFFFFFFFFFFFFFFL;
typedef unsigned __int64 uint64;
#endif

static double const pi = 3.141592653589793238;
static double const two_pi = pi * 2;
static double const def_arc_tolerance = 0.25;

enum Direction { dRightToLeft, dLeftToRight };

static int const Unassigned = -1;  //edge not currently 'owning' a solution
static int const Skip = -2;        //edge that would otherwise close a path

#define HORIZONTAL (-1.0E+40)
#define TOLERANCE  (1.0e-20)
#define NEAR_ZERO(val)   (((val) > -TOLERANCE) && ((val) < TOLERANCE))

struct IntPoint {
  cInt X;
  cInt Y;
#ifdef use_xyz
  cInt Z;
  IntPoint(cInt x = 0, cInt y = 0, cInt z = 0): X(x), Y(y), Z(z) {};
#else
  IntPoint(cInt x = 0, cInt y = 0): X(x), Y(y) {};
#endif

  friend inline bool operator== (const IntPoint& a, const IntPoint& b) {
    return a.X == b.X && a.Y == b.Y;
  }
  friend inline bool operator!= (const IntPoint& a, const IntPoint& b) {
    return a.X != b.X  || a.Y != b.Y;
  }
};

struct DoublePoint {
  double X;
  double Y;
  DoublePoint(double x = 0, double y = 0) : X(x), Y(y) {}
  DoublePoint(IntPoint ip) : X((double)ip.X), Y((double)ip.Y) {}
};

typedef std::vector< IntPoint > Path;
typedef std::vector< Path > Paths;

#ifdef use_xyz
typedef void (*TZFillCallback)(IntPoint& z1, IntPoint& z2, IntPoint& pt);
#endif

enum InitOptions {ioReverseSolution = 1, ioStrictlySimple = 2, ioPreserveCollinear = 4};
enum JoinType {jtSquare, jtRound, jtMiter};
enum EndType {etClosedPolygon, etClosedLine, etOpenButt, etOpenSquare, etOpenRound};
#ifdef use_deprecated
enum EndType_ {etClosed, etButt = 2, etSquare, etRound};
#endif

struct IntRect {
  cInt left;
  cInt top;
  cInt right;
  cInt bottom;
};

//enums that are used internally ...
enum EdgeSide { esLeft = 1, esRight = 2};

//forward declarations (for stuff used internally) ...
struct TEdge;
struct IntersectNode;
struct LocalMinima;
struct Scanbeam;
struct OutPt;
struct OutRec;
struct Join;

typedef std::vector < OutRec* > PolyOutList;
typedef std::vector < TEdge* > EdgeList;
typedef std::vector < Join* > JoinList;
typedef std::vector < IntersectNode* > IntersectList;

class clipperException : public std::exception
{
public:
  clipperException(const char* description): m_descr(description) {}
  virtual ~clipperException() throw() {}
  virtual const char* what() const throw() {
    return m_descr.c_str();
  }
private:
  std::string m_descr;
};

#include "PolyTree.inl"

struct TEdge {
  IntPoint Bot;
  IntPoint Curr;
  IntPoint Top;
  IntPoint Delta;
  double Dx;
  PolyType PolyTyp;
  EdgeSide Side;
  int WindDelta; //1 or -1 depending on winding direction
  int WindCnt;
  int WindCnt2; //winding count of the opposite polytype
  int OutIdx;
  TEdge* Next;
  TEdge* Prev;
  TEdge* NextInLML;
  TEdge* NextInAEL;
  TEdge* PrevInAEL;
  TEdge* NextInSEL;
  TEdge* PrevInSEL;
};

struct IntersectNode {
  TEdge*          Edge1;
  TEdge*          Edge2;
  IntPoint        Pt;
};

struct LocalMinima {
  cInt          Y;
  TEdge*        LeftBound;
  TEdge*        RightBound;
  LocalMinima*  Next;
};

struct OutPt;

struct OutRec {
  int       Idx;
  bool      IsHole;
  bool      IsOpen;
  OutRec*   FirstLeft;  //see comments in clipper.pas
  PolyNode* PolyNd;
  OutPt*    Pts;
  OutPt*    BottomPt;
};

struct OutPt {
  int       Idx;
  IntPoint  Pt;
  OutPt*    Next;
  OutPt*    Prev;
};

struct Join {
  OutPt*    OutPt1;
  OutPt*    OutPt2;
  IntPoint  OffPt;
};

inline cInt Round(double val)
{
  if ((val < 0)) {
    return (cInt)(val - 0.5);
  }
  else {
    return (cInt)(val + 0.5);
  }
}


inline cInt Abs(cInt val)
{
  return val < 0 ? -val : val;
}


#ifndef use_int32
#include "Int128.inl"
#endif


// Miscellaneous global functions


double Area(const Path& poly)
{
  int size = (int)poly.size();

  if (size < 3) {
    return 0;
  }

  double a = 0;

  for (int i = 0, j = size - 1; i < size; ++i) {
    a += ((double)poly[j].X + poly[i].X) * ((double)poly[j].Y - poly[i].Y);
    j = i;
  }

  return -a * 0.5;
}

double Area(const OutRec& outRec)
{
  OutPt* op = outRec.Pts;

  if (!op) {
    return 0;
  }

  double a = 0;

  do {
    a += (double)(op->Prev->Pt.X + op->Pt.X) * (double)(op->Prev->Pt.Y - op->Pt.Y);
    op = op->Next;
  }
  while (op != outRec.Pts);

  return a * 0.5;
}

bool Orientation(const Path& poly)
{
  return Area(poly) >= 0;
}



bool PointIsVertex(const IntPoint& Pt, OutPt* pp)
{
  OutPt* pp2 = pp;

  do {
    if (pp2->Pt == Pt) {
      return true;
    }

    pp2 = pp2->Next;
  }
  while (pp2 != pp);

  return false;
}


int PointInPolygon(const IntPoint& pt, const Path& path)
{
  //returns 0 if false, +1 if true, -1 if pt ON polygon boundary
  //http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.88.5498&rep=rep1&type=pdf
  int result = 0;
  size_t cnt = path.size();

  if (cnt < 3) {
    return 0;
  }

  IntPoint ip = path[0];

  for (size_t i = 1; i <= cnt; ++i) {
    IntPoint ipNext = (i == cnt ? path[0] : path[i]);

    if (ipNext.Y == pt.Y) {
      if ((ipNext.X == pt.X) || (ip.Y == pt.Y &&
          ((ipNext.X > pt.X) == (ip.X < pt.X)))) {
        return -1;
      }
    }

    if ((ip.Y < pt.Y) != (ipNext.Y < pt.Y)) {
      if (ip.X >= pt.X) {
        if (ipNext.X > pt.X) {
          result = 1 - result;
        }
        else {
          double d = (double)(ip.X - pt.X) * (ipNext.Y - pt.Y) -
              (double)(ipNext.X - pt.X) * (ip.Y - pt.Y);

          if (!d) {
            return -1;
          }

          if ((d > 0) == (ipNext.Y > ip.Y)) {
            result = 1 - result;
          }
        }
      }
      else {
        if (ipNext.X > pt.X) {
          double d = (double)(ip.X - pt.X) * (ipNext.Y - pt.Y) -
              (double)(ipNext.X - pt.X) * (ip.Y - pt.Y);

          if (!d) {
            return -1;
          }

          if ((d > 0) == (ipNext.Y > ip.Y)) {
            result = 1 - result;
          }
        }
      }
    }

    ip = ipNext;
  }

  return result;
}


int PointInPolygon(const IntPoint& pt, OutPt* op)
{
  //returns 0 if false, +1 if true, -1 if pt ON polygon boundary
  //http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.88.5498&rep=rep1&type=pdf
  int result = 0;
  OutPt* startOp = op;

  for (;;) {
    if (op->Next->Pt.Y == pt.Y) {
      if ((op->Next->Pt.X == pt.X) || (op->Pt.Y == pt.Y &&
          ((op->Next->Pt.X > pt.X) == (op->Pt.X < pt.X)))) {
        return -1;
      }
    }

    if ((op->Pt.Y < pt.Y) != (op->Next->Pt.Y < pt.Y)) {
      if (op->Pt.X >= pt.X) {
        if (op->Next->Pt.X > pt.X) {
          result = 1 - result;
        }
        else {
          double d = (double)(op->Pt.X - pt.X) * (op->Next->Pt.Y - pt.Y) -
              (double)(op->Next->Pt.X - pt.X) * (op->Pt.Y - pt.Y);

          if (!d) {
            return -1;
          }

          if ((d > 0) == (op->Next->Pt.Y > op->Pt.Y)) {
            result = 1 - result;
          }
        }
      }
      else {
        if (op->Next->Pt.X > pt.X) {
          double d = (double)(op->Pt.X - pt.X) * (op->Next->Pt.Y - pt.Y) -
              (double)(op->Next->Pt.X - pt.X) * (op->Pt.Y - pt.Y);

          if (!d) {
            return -1;
          }

          if ((d > 0) == (op->Next->Pt.Y > op->Pt.Y)) {
            result = 1 - result;
          }
        }
      }
    }

    op = op->Next;

    if (startOp == op) {
      break;
    }
  }

  return result;
}


bool Poly2ContainsPoly1(OutPt* OutPt1, OutPt* OutPt2)
{
  OutPt* op = OutPt1;

  do {
    int res = PointInPolygon(op->Pt, OutPt2);

    if (res >= 0) {
      return res != 0;
    }

    op = op->Next;
  }
  while (op != OutPt1);

  return true;
}


bool SlopesEqual(const TEdge& e1, const TEdge& e2, bool UseFullInt64Range)
{
#ifndef use_int32

  if (UseFullInt64Range) {
    return Int128Mul(e1.Delta.Y, e2.Delta.X) == Int128Mul(e1.Delta.X, e2.Delta.Y);
  }
  else
#endif
    return e1.Delta.Y * e2.Delta.X == e1.Delta.X * e2.Delta.Y;
}


bool SlopesEqual(const IntPoint pt1, const IntPoint pt2,
    const IntPoint pt3, bool UseFullInt64Range)
{
#ifndef use_int32

  if (UseFullInt64Range) {
    return Int128Mul(pt1.Y - pt2.Y, pt2.X - pt3.X) == Int128Mul(pt1.X - pt2.X, pt2.Y - pt3.Y);
  }
  else
#endif
    return (pt1.Y - pt2.Y) * (pt2.X - pt3.X) == (pt1.X - pt2.X) * (pt2.Y - pt3.Y);
}


bool SlopesEqual(const IntPoint pt1, const IntPoint pt2,
    const IntPoint pt3, const IntPoint pt4, bool UseFullInt64Range)
{
#ifndef use_int32

  if (UseFullInt64Range) {
    return Int128Mul(pt1.Y - pt2.Y, pt3.X - pt4.X) == Int128Mul(pt1.X - pt2.X, pt3.Y - pt4.Y);
  }
  else
#endif
    return (pt1.Y - pt2.Y) * (pt3.X - pt4.X) == (pt1.X - pt2.X) * (pt3.Y - pt4.Y);
}


inline bool IsHorizontal(TEdge& e)
{
  return e.Delta.Y == 0;
}


inline double GetDx(const IntPoint pt1, const IntPoint pt2)
{
  return (pt1.Y == pt2.Y) ?
      HORIZONTAL : (double)(pt2.X - pt1.X) / (pt2.Y - pt1.Y);
}


inline void SetDx(TEdge& e)
{
  e.Delta.X = (e.Top.X - e.Bot.X);
  e.Delta.Y = (e.Top.Y - e.Bot.Y);

  if (e.Delta.Y == 0) {
    e.Dx = HORIZONTAL;
  }
  else {
    e.Dx = (double)(e.Delta.X) / e.Delta.Y;
  }
}


inline void SwapSides(TEdge& Edge1, TEdge& Edge2)
{
  EdgeSide Side =  Edge1.Side;
  Edge1.Side = Edge2.Side;
  Edge2.Side = Side;
}


inline void SwapPolyIndexes(TEdge& Edge1, TEdge& Edge2)
{
  int OutIdx =  Edge1.OutIdx;
  Edge1.OutIdx = Edge2.OutIdx;
  Edge2.OutIdx = OutIdx;
}


inline cInt TopX(TEdge& edge, const cInt currentY)
{
  return (currentY == edge.Top.Y) ?
      edge.Top.X : edge.Bot.X + Round(edge.Dx * (currentY - edge.Bot.Y));
}


bool IntersectPoint(TEdge& Edge1, TEdge& Edge2,
    IntPoint& ip, bool UseFullInt64Range)
{
#ifdef use_xyz
  ip.Z = 0;
#endif
  double b1, b2;

  //nb: with very large coordinate values, it's possible for SlopesEqual() to
  //return false but for the edge.Dx value be equal due to double precision rounding.
  if (SlopesEqual(Edge1, Edge2, UseFullInt64Range) || Edge1.Dx == Edge2.Dx) {
    if (Edge2.Bot.Y > Edge1.Bot.Y) {
      ip = Edge2.Bot;
    }
    else {
      ip = Edge1.Bot;
    }

    return false;
  }
  else if (Edge1.Delta.X == 0) {
    ip.X = Edge1.Bot.X;

    if (IsHorizontal(Edge2)) {
      ip.Y = Edge2.Bot.Y;
    }
    else {
      b2 = Edge2.Bot.Y - (Edge2.Bot.X / Edge2.Dx);
      ip.Y = Round(ip.X / Edge2.Dx + b2);
    }
  }
  else if (Edge2.Delta.X == 0) {
    ip.X = Edge2.Bot.X;

    if (IsHorizontal(Edge1)) {
      ip.Y = Edge1.Bot.Y;
    }
    else {
      b1 = Edge1.Bot.Y - (Edge1.Bot.X / Edge1.Dx);
      ip.Y = Round(ip.X / Edge1.Dx + b1);
    }
  }
  else {
    b1 = Edge1.Bot.X - Edge1.Bot.Y * Edge1.Dx;
    b2 = Edge2.Bot.X - Edge2.Bot.Y * Edge2.Dx;
    double q = (b2 - b1) / (Edge1.Dx - Edge2.Dx);
    ip.Y = Round(q);

    if (fabs(Edge1.Dx) < fabs(Edge2.Dx)) {
      ip.X = Round(Edge1.Dx * q + b1);
    }
    else {
      ip.X = Round(Edge2.Dx * q + b2);
    }
  }

  if (ip.Y < Edge1.Top.Y || ip.Y < Edge2.Top.Y) {
    if (Edge1.Top.Y > Edge2.Top.Y) {
      ip.Y = Edge1.Top.Y;
    }
    else {
      ip.Y = Edge2.Top.Y;
    }

    if (fabs(Edge1.Dx) < fabs(Edge2.Dx)) {
      ip.X = TopX(Edge1, ip.Y);
    }
    else {
      ip.X = TopX(Edge2, ip.Y);
    }
  }

  return true;
}


void ReversePolyPtLinks(OutPt* pp)
{
  if (!pp) {
    return;
  }

  OutPt* pp1, *pp2;
  pp1 = pp;

  do {
    pp2 = pp1->Next;
    pp1->Next = pp1->Prev;
    pp1->Prev = pp2;
    pp1 = pp2;
  }
  while (pp1 != pp);
}


void DisposeOutPts(OutPt*& pp)
{
  if (pp == 0) {
    return;
  }

  pp->Prev->Next = 0;

  while (pp) {
    OutPt* tmpPp = pp;
    pp = pp->Next;
    delete tmpPp;
  }
}


inline void InitEdge(TEdge* e, TEdge* eNext, TEdge* ePrev, const IntPoint& Pt)
{
  memset(e, 0, sizeof(TEdge));
  e->Next = eNext;
  e->Prev = ePrev;
  e->Curr = Pt;
  e->OutIdx = Unassigned;
}


void InitEdge2(TEdge& e, PolyType Pt)
{
  if (e.Curr.Y >= e.Next->Curr.Y) {
    e.Bot = e.Curr;
    e.Top = e.Next->Curr;
  }
  else {
    e.Top = e.Curr;
    e.Bot = e.Next->Curr;
  }

  SetDx(e);
  e.PolyTyp = Pt;
}


TEdge* RemoveEdge(TEdge* e)
{
  //removes e from double_linked_list (but without removing from memory)
  e->Prev->Next = e->Next;
  e->Next->Prev = e->Prev;
  TEdge* result = e->Next;
  e->Prev = 0; //flag as removed (see ClipperBase.Clear)
  return result;
}


inline void ReverseHorizontal(TEdge& e)
{
  //swap horizontal edges' Top and Bottom x's so they follow the natural
  //progression of the bounds - ie so their xbots will align with the
  //adjoining lower edge. [Helpful in the ProcessHorizontal() method.]
  cInt tmp = e.Top.X;
  e.Top.X = e.Bot.X;
  e.Bot.X = tmp;
#ifdef use_xyz
  tmp = e.Top.Z;
  e.Top.Z = e.Bot.Z;
  e.Bot.Z = tmp;
#endif
}


void SwapPoints(IntPoint& pt1, IntPoint& pt2)
{
  IntPoint tmp = pt1;
  pt1 = pt2;
  pt2 = tmp;
}


bool GetOverlapSegment(IntPoint pt1a, IntPoint pt1b, IntPoint pt2a,
    IntPoint pt2b, IntPoint& pt1, IntPoint& pt2)
{
  //precondition: segments are Collinear.
  if (Abs(pt1a.X - pt1b.X) > Abs(pt1a.Y - pt1b.Y)) {
    if (pt1a.X > pt1b.X) {
      SwapPoints(pt1a, pt1b);
    }

    if (pt2a.X > pt2b.X) {
      SwapPoints(pt2a, pt2b);
    }

    if (pt1a.X > pt2a.X) {
      pt1 = pt1a;
    }
    else {
      pt1 = pt2a;
    }

    if (pt1b.X < pt2b.X) {
      pt2 = pt1b;
    }
    else {
      pt2 = pt2b;
    }

    return pt1.X < pt2.X;
  }
  else {
    if (pt1a.Y < pt1b.Y) {
      SwapPoints(pt1a, pt1b);
    }

    if (pt2a.Y < pt2b.Y) {
      SwapPoints(pt2a, pt2b);
    }

    if (pt1a.Y < pt2a.Y) {
      pt1 = pt1a;
    }
    else {
      pt1 = pt2a;
    }

    if (pt1b.Y > pt2b.Y) {
      pt2 = pt1b;
    }
    else {
      pt2 = pt2b;
    }

    return pt1.Y > pt2.Y;
  }
}


bool FirstIsBottomPt(const OutPt* btmPt1, const OutPt* btmPt2)
{
  OutPt* p = btmPt1->Prev;

  while ((p->Pt == btmPt1->Pt) && (p != btmPt1)) {
    p = p->Prev;
  }

  double dx1p = fabs(GetDx(btmPt1->Pt, p->Pt));
  p = btmPt1->Next;

  while ((p->Pt == btmPt1->Pt) && (p != btmPt1)) {
    p = p->Next;
  }

  double dx1n = fabs(GetDx(btmPt1->Pt, p->Pt));

  p = btmPt2->Prev;

  while ((p->Pt == btmPt2->Pt) && (p != btmPt2)) {
    p = p->Prev;
  }

  double dx2p = fabs(GetDx(btmPt2->Pt, p->Pt));
  p = btmPt2->Next;

  while ((p->Pt == btmPt2->Pt) && (p != btmPt2)) {
    p = p->Next;
  }

  double dx2n = fabs(GetDx(btmPt2->Pt, p->Pt));
  return (dx1p >= dx2p && dx1p >= dx2n) || (dx1n >= dx2p && dx1n >= dx2n);
}


OutPt* GetBottomPt(OutPt* pp)
{
  OutPt* dups = 0;
  OutPt* p = pp->Next;

  while (p != pp) {
    if (p->Pt.Y > pp->Pt.Y) {
      pp = p;
      dups = 0;
    }
    else if (p->Pt.Y == pp->Pt.Y && p->Pt.X <= pp->Pt.X) {
      if (p->Pt.X < pp->Pt.X) {
        dups = 0;
        pp = p;
      }
      else {
        if (p->Next != pp && p->Prev != pp) {
          dups = p;
        }
      }
    }

    p = p->Next;
  }

  if (dups) {
    //there appears to be at least 2 vertices at BottomPt so ...
    while (dups != p) {
      if (!FirstIsBottomPt(p, dups)) {
        pp = dups;
      }

      dups = dups->Next;

      while (dups->Pt != pp->Pt) {
        dups = dups->Next;
      }
    }
  }

  return pp;
}


bool FindSegment(OutPt*& pp, bool UseFullInt64Range,
    IntPoint& pt1, IntPoint& pt2)
{
  //OutPt1 & OutPt2 => the overlap segment (if the function returns true)
  if (!pp) {
    return false;
  }

  OutPt* pp2 = pp;
  IntPoint pt1a = pt1, pt2a = pt2;

  do {
    if (SlopesEqual(pt1a, pt2a, pp->Pt, pp->Prev->Pt, UseFullInt64Range) &&
        SlopesEqual(pt1a, pt2a, pp->Pt, UseFullInt64Range) &&
        GetOverlapSegment(pt1a, pt2a, pp->Pt, pp->Prev->Pt, pt1, pt2)) {
      return true;
    }

    pp = pp->Next;
  }
  while (pp != pp2);

  return false;
}


bool Pt2IsBetweenPt1AndPt3(const IntPoint pt1,
    const IntPoint pt2, const IntPoint pt3)
{
  if ((pt1 == pt3) || (pt1 == pt2) || (pt3 == pt2)) {
    return false;
  }
  else if (pt1.X != pt3.X) {
    return (pt2.X > pt1.X) == (pt2.X < pt3.X);
  }
  else {
    return (pt2.Y > pt1.Y) == (pt2.Y < pt3.Y);
  }
}


OutPt* InsertPolyPtBetween(OutPt* p1, OutPt* p2, const IntPoint Pt)
{
  if (p1 == p2) {
    throw "JoinError";
  }

  OutPt* result = new OutPt;
  result->Pt = Pt;

  if (p2 == p1->Next) {
    p1->Next = result;
    p2->Prev = result;
    result->Next = p2;
    result->Prev = p1;
  }
  else {
    p2->Next = result;
    p1->Prev = result;
    result->Next = p1;
    result->Prev = p2;
  }

  return result;
}


bool HorzSegmentsOverlap(const IntPoint& pt1a, const IntPoint& pt1b,
    const IntPoint& pt2a, const IntPoint& pt2b)
{
  //precondition: both segments are horizontal
  if ((pt1a.X > pt2a.X) == (pt1a.X < pt2b.X)) {
    return true;
  }
  else if ((pt1b.X > pt2a.X) == (pt1b.X < pt2b.X)) {
    return true;
  }
  else if ((pt2a.X > pt1a.X) == (pt2a.X < pt1b.X)) {
    return true;
  }
  else if ((pt2b.X > pt1a.X) == (pt2b.X < pt1b.X)) {
    return true;
  }
  else if ((pt1a.X == pt2a.X) && (pt1b.X == pt2b.X)) {
    return true;
  }
  else if ((pt1a.X == pt2b.X) && (pt1b.X == pt2a.X)) {
    return true;
  }
  else {
    return false;
  }
}


void RangeTest(const IntPoint& Pt, bool& useFullRange)
{
  if (useFullRange) {
    if (Pt.X > hiRange || Pt.Y > hiRange || -Pt.X > hiRange || -Pt.Y > hiRange) {
      throw "Coordinate outside allowed range";
    }
  }
  else if (Pt.X > loRange || Pt.Y > loRange || -Pt.X > loRange || -Pt.Y > loRange) {
    useFullRange = true;
    RangeTest(Pt, useFullRange);
  }
}


TEdge* FindNextLocMin(TEdge* E)
{
  for (;;) {
    while (E->Bot != E->Prev->Bot || E->Curr == E->Top) {
      E = E->Next;
    }

    if (!IsHorizontal(*E) && !IsHorizontal(*E->Prev)) {
      break;
    }

    while (IsHorizontal(*E->Prev)) {
      E = E->Prev;
    }

    TEdge* E2 = E;

    while (IsHorizontal(*E)) {
      E = E->Next;
    }

    if (E->Top.Y == E->Prev->Bot.Y) {
      continue;  //ie just an intermediate horz.
    }

    if (E2->Prev->Bot.X < E->Bot.X) {
      E = E2;
    }

    break;
  }

  return E;
}


void ReversePath(Path& p)
{
  std::reverse(p.begin(), p.end());
}


void ReversePaths(Paths& p)
{
  for (Paths::size_type i = 0; i < p.size(); ++i) {
    ReversePath(p[i]);
  }
}


// ClipperOffset support functions ...
DoublePoint GetUnitNormal(const IntPoint& pt1, const IntPoint& pt2)
{
  if (pt2.X == pt1.X && pt2.Y == pt1.Y) {
    return DoublePoint(0, 0);
  }

  double Dx = (double)(pt2.X - pt1.X);
  double dy = (double)(pt2.Y - pt1.Y);
  double f = 1 * 1.0 / sqrt(Dx * Dx + dy * dy);
  Dx *= f;
  dy *= f;
  return DoublePoint(dy, -Dx);
}

OutRec* GetLowermostRec(OutRec* outRec1, OutRec* outRec2)
{
  //work out which polygon fragment has the correct hole state ...
  if (!outRec1->BottomPt) {
    outRec1->BottomPt = GetBottomPt(outRec1->Pts);
  }

  if (!outRec2->BottomPt) {
    outRec2->BottomPt = GetBottomPt(outRec2->Pts);
  }

  OutPt* OutPt1 = outRec1->BottomPt;
  OutPt* OutPt2 = outRec2->BottomPt;

  if (OutPt1->Pt.Y > OutPt2->Pt.Y) {
    return outRec1;
  }
  else if (OutPt1->Pt.Y < OutPt2->Pt.Y) {
    return outRec2;
  }
  else if (OutPt1->Pt.X < OutPt2->Pt.X) {
    return outRec1;
  }
  else if (OutPt1->Pt.X > OutPt2->Pt.X) {
    return outRec2;
  }
  else if (OutPt1->Next == OutPt1) {
    return outRec2;
  }
  else if (OutPt2->Next == OutPt2) {
    return outRec1;
  }
  else if (FirstIsBottomPt(OutPt1, OutPt2)) {
    return outRec1;
  }
  else {
    return outRec2;
  }
}


bool Param1RightOfParam2(OutRec* outRec1, OutRec* outRec2)
{
  do {
    outRec1 = outRec1->FirstLeft;

    if (outRec1 == outRec2) {
      return true;
    }
  }
  while (outRec1);

  return false;
}



inline bool IsMinima(TEdge* e)
{
  return e  && (e->Prev->NextInLML != e) && (e->Next->NextInLML != e);
}


inline bool IsMaxima(TEdge* e, const cInt Y)
{
  return e && e->Top.Y == Y && !e->NextInLML;
}


inline bool IsIntermediate(TEdge* e, const cInt Y)
{
  return e->Top.Y == Y && e->NextInLML;
}


TEdge* GetMaximaPair(TEdge* e)
{
  TEdge* result = 0;

  if ((e->Next->Top == e->Top) && !e->Next->NextInLML) {
    result = e->Next;
  }
  else if ((e->Prev->Top == e->Top) && !e->Prev->NextInLML) {
    result = e->Prev;
  }

  if (result && (result->OutIdx == Skip ||
      //result is false if both NextInAEL & PrevInAEL are nil & not horizontal ...
      (result->NextInAEL == result->PrevInAEL && !IsHorizontal(*result)))) {
    return 0;
  }

  return result;
}

TEdge* GetNextInAEL(TEdge* e, Direction dir)
{
  return dir == dLeftToRight ? e->NextInAEL : e->PrevInAEL;
}


void GetHorzDirection(TEdge& HorzEdge, Direction& Dir, cInt& Left, cInt& Right)
{
  if (HorzEdge.Bot.X < HorzEdge.Top.X) {
    Left = HorzEdge.Bot.X;
    Right = HorzEdge.Top.X;
    Dir = dLeftToRight;
  }
  else {
    Left = HorzEdge.Top.X;
    Right = HorzEdge.Bot.X;
    Dir = dRightToLeft;
  }
}

bool IntersectListSort(IntersectNode* node1, IntersectNode* node2)
{
  return node2->Pt.Y < node1->Pt.Y;
}


inline bool EdgesAdjacent(const IntersectNode& inode)
{
  return (inode.Edge1->NextInSEL == inode.Edge2) ||
      (inode.Edge1->PrevInSEL == inode.Edge2);
}

int PointCount(OutPt* Pts)
{
  if (!Pts) {
    return 0;
  }

  int result = 0;
  OutPt* p = Pts;

  do {
    result++;
    p = p->Next;
  }
  while (p != Pts);

  return result;
}



void SwapIntersectNodes(IntersectNode& int1, IntersectNode& int2)
{
  //just swap the contents (because fIntersectNodes is a single-linked-list)
  IntersectNode inode = int1; //gets a copy of Int1
  int1.Edge1 = int2.Edge1;
  int1.Edge2 = int2.Edge2;
  int1.Pt = int2.Pt;
  int2.Edge1 = inode.Edge1;
  int2.Edge2 = inode.Edge2;
  int2.Pt = inode.Pt;
}


inline bool E2InsertsBeforeE1(TEdge& e1, TEdge& e2)
{
  if (e2.Curr.X == e1.Curr.X) {
    if (e2.Top.Y > e1.Top.Y) {
      return e2.Top.X < TopX(e1, e2.Top.Y);
    }
    else {
      return e1.Top.X > TopX(e2, e1.Top.Y);
    }
  }
  else {
    return e2.Curr.X < e1.Curr.X;
  }
}


bool GetOverlap(const cInt a1, const cInt a2, const cInt b1, const cInt b2,
    cInt& Left, cInt& Right)
{
  if (a1 < a2) {
    if (b1 < b2) {
      Left = MAX(a1, b1);
      Right = MIN(a2, b2);
    }
    else {
      Left = MAX(a1, b2);
      Right = MIN(a2, b1);
    }
  }
  else {
    if (b1 < b2) {
      Left = MAX(a2, b1);
      Right = MIN(a1, b2);
    }
    else {
      Left = MAX(a2, b2);
      Right = MIN(a1, b1);
    }
  }

  return Left < Right;
}


inline void UpdateOutPtIdxs(OutRec& outrec)
{
  OutPt* op = outrec.Pts;

  do {
    op->Idx = outrec.Idx;
    op = op->Prev;
  }
  while (op != outrec.Pts);
}


OutPt* DupOutPt(OutPt* outPt, bool InsertAfter)
{
  OutPt* result = new OutPt;
  result->Pt = outPt->Pt;
  result->Idx = outPt->Idx;

  if (InsertAfter) {
    result->Next = outPt->Next;
    result->Prev = outPt;
    outPt->Next->Prev = result;
    outPt->Next = result;
  }
  else {
    result->Prev = outPt->Prev;
    result->Next = outPt;
    outPt->Prev->Next = result;
    outPt->Prev = result;
  }

  return result;
}


bool JoinHorz(OutPt* op1, OutPt* op1b, OutPt* op2, OutPt* op2b,
    const IntPoint Pt, bool DiscardLeft)
{
  Direction Dir1 = (op1->Pt.X > op1b->Pt.X ? dRightToLeft : dLeftToRight);
  Direction Dir2 = (op2->Pt.X > op2b->Pt.X ? dRightToLeft : dLeftToRight);

  if (Dir1 == Dir2) {
    return false;
  }

  //When DiscardLeft, we want Op1b to be on the Left of Op1, otherwise we
  //want Op1b to be on the Right. (And likewise with Op2 and Op2b.)
  //So, to facilitate this while inserting Op1b and Op2b ...
  //when DiscardLeft, make sure we're AT or RIGHT of Pt before adding Op1b,
  //otherwise make sure we're AT or LEFT of Pt. (Likewise with Op2b.)
  if (Dir1 == dLeftToRight) {
    while (op1->Next->Pt.X <= Pt.X &&
        op1->Next->Pt.X >= op1->Pt.X && op1->Next->Pt.Y == Pt.Y) {
      op1 = op1->Next;
    }

    if (DiscardLeft && (op1->Pt.X != Pt.X)) {
      op1 = op1->Next;
    }

    op1b = DupOutPt(op1, !DiscardLeft);

    if (op1b->Pt != Pt) {
      op1 = op1b;
      op1->Pt = Pt;
      op1b = DupOutPt(op1, !DiscardLeft);
    }
  }
  else {
    while (op1->Next->Pt.X >= Pt.X &&
        op1->Next->Pt.X <= op1->Pt.X && op1->Next->Pt.Y == Pt.Y) {
      op1 = op1->Next;
    }

    if (!DiscardLeft && (op1->Pt.X != Pt.X)) {
      op1 = op1->Next;
    }

    op1b = DupOutPt(op1, DiscardLeft);

    if (op1b->Pt != Pt) {
      op1 = op1b;
      op1->Pt = Pt;
      op1b = DupOutPt(op1, DiscardLeft);
    }
  }

  if (Dir2 == dLeftToRight) {
    while (op2->Next->Pt.X <= Pt.X &&
        op2->Next->Pt.X >= op2->Pt.X && op2->Next->Pt.Y == Pt.Y) {
      op2 = op2->Next;
    }

    if (DiscardLeft && (op2->Pt.X != Pt.X)) {
      op2 = op2->Next;
    }

    op2b = DupOutPt(op2, !DiscardLeft);

    if (op2b->Pt != Pt) {
      op2 = op2b;
      op2->Pt = Pt;
      op2b = DupOutPt(op2, !DiscardLeft);
    };
  }
  else {
    while (op2->Next->Pt.X >= Pt.X &&
        op2->Next->Pt.X <= op2->Pt.X && op2->Next->Pt.Y == Pt.Y) {
      op2 = op2->Next;
    }

    if (!DiscardLeft && (op2->Pt.X != Pt.X)) {
      op2 = op2->Next;
    }

    op2b = DupOutPt(op2, DiscardLeft);

    if (op2b->Pt != Pt) {
      op2 = op2b;
      op2->Pt = Pt;
      op2b = DupOutPt(op2, DiscardLeft);
    };
  };

  if ((Dir1 == dLeftToRight) == DiscardLeft) {
    op1->Prev = op2;
    op2->Next = op1;
    op1b->Next = op2b;
    op2b->Prev = op1b;
  }
  else {
    op1->Next = op2;
    op2->Prev = op1;
    op1b->Prev = op2b;
    op2b->Next = op1b;
  }

  return true;
}



static OutRec* ParseFirstLeft(OutRec* FirstLeft)
{
  while (FirstLeft && !FirstLeft->Pts) {
    FirstLeft = FirstLeft->FirstLeft;
  }

  return FirstLeft;
}

#include "Clipper_impl.inl"
#include "ClipperOffset.inl"

void SimplifyPolygon(const Path& in_poly, Paths& out_polys, PolyFillType fillType = pftEvenOdd)
{
  Clipper c;
  c.StrictlySimple(true);
  c.AddPath(in_poly, ptSubject, true);
  c.Execute(ctUnion, out_polys, fillType, fillType);
}


void SimplifyPolygons(const Paths& in_polys, Paths& out_polys, PolyFillType fillType = pftEvenOdd)
{
  Clipper c;
  c.StrictlySimple(true);
  c.AddPaths(in_polys, ptSubject, true);
  c.Execute(ctUnion, out_polys, fillType, fillType);
}


void SimplifyPolygons(Paths& polys, PolyFillType fillType = pftEvenOdd)
{
  SimplifyPolygons(polys, polys, fillType);
}


inline double DistanceSqrd(const IntPoint& pt1, const IntPoint& pt2)
{
  double Dx = ((double)pt1.X - pt2.X);
  double dy = ((double)pt1.Y - pt2.Y);
  return (Dx * Dx + dy * dy);
}


double DistanceFromLineSqrd(
    const IntPoint& pt, const IntPoint& ln1, const IntPoint& ln2)
{
  //The equation of a line in general form (Ax + By + C = 0)
  //given 2 points (x¹,y¹) & (x²,y²) is ...
  //(y¹ - y²)x + (x² - x¹)y + (y² - y¹)x¹ - (x² - x¹)y¹ = 0
  //A = (y¹ - y²); B = (x² - x¹); C = (y² - y¹)x¹ - (x² - x¹)y¹
  //perpendicular distance of point (x³,y³) = (Ax³ + By³ + C)/Sqrt(A² + B²)
  //see http://en.wikipedia.org/wiki/Perpendicular_distance
  double A = double(ln1.Y - ln2.Y);
  double B = double(ln2.X - ln1.X);
  double C = A * ln1.X  + B * ln1.Y;
  C = A * pt.X + B * pt.Y - C;
  return (C * C) / (A * A + B * B);
}


bool SlopesNearCollinear(const IntPoint& pt1,
    const IntPoint& pt2, const IntPoint& pt3, double distSqrd)
{
  return DistanceFromLineSqrd(pt2, pt1, pt3) < distSqrd;
}


bool PointsAreClose(IntPoint pt1, IntPoint pt2, double distSqrd)
{
  double Dx = (double)pt1.X - pt2.X;
  double dy = (double)pt1.Y - pt2.Y;
  return ((Dx * Dx) + (dy * dy) <= distSqrd);
}


OutPt* ExcludeOp(OutPt* op)
{
  OutPt* result = op->Prev;
  result->Next = op->Next;
  op->Next->Prev = result;
  result->Idx = 0;
  return result;
}


void CleanPolygon(const Path& in_poly, Path& out_poly, double distance = 1.415)
{
  //distance = proximity in units/pixels below which vertices
  //will be stripped. Default ~= sqrt(2).
  size_t i;
  size_t size = in_poly.size();

  if (size == 0) {
    out_poly.clear();
    return;
  }

  OutPt* outPts = new OutPt[size];

  for (i = 0; i < size; ++i) {
    outPts[i].Pt = in_poly[i];
    outPts[i].Next = &outPts[(i + 1) % size];
    outPts[i].Next->Prev = &outPts[i];
    outPts[i].Idx = 0;
  }

  double distSqrd = distance * distance;
  OutPt* op = &outPts[0];

  while (op->Idx == 0 && op->Next != op->Prev) {
    if (PointsAreClose(op->Pt, op->Prev->Pt, distSqrd)) {
      op = ExcludeOp(op);
      size--;
    }
    else if (PointsAreClose(op->Prev->Pt, op->Next->Pt, distSqrd)) {
      ExcludeOp(op->Next);
      op = ExcludeOp(op);
      size -= 2;
    }
    else if (SlopesNearCollinear(op->Prev->Pt, op->Pt, op->Next->Pt, distSqrd)) {
      op = ExcludeOp(op);
      size--;
    }
    else {
      op->Idx = 1;
      op = op->Next;
    }
  }

  if (size < 3) {
    size = 0;
  }

  out_poly.resize(size);

  for (i = 0; i < size; ++i) {
    out_poly[i] = op->Pt;
    op = op->Next;
  }

  delete [] outPts;
}


void CleanPolygon(Path& poly, double distance = 1.415)
{
  CleanPolygon(poly, poly, distance);
}


void CleanPolygons(const Paths& in_polys, Paths& out_polys, double distance = 1.415)
{
  for (Paths::size_type i = 0; i < in_polys.size(); ++i) {
    CleanPolygon(in_polys[i], out_polys[i], distance);
  }
}


void CleanPolygons(Paths& polys, double distance = 1.415)
{
  CleanPolygons(polys, polys, distance);
}


void Minkowski(const Path& poly, const Path& path,
    Paths& solution, bool isSum, bool isClosed)
{
  int delta = (isClosed ? 1 : 0);
  size_t polyCnt = poly.size();
  size_t pathCnt = path.size();
  Paths pp;
  pp.reserve(pathCnt);

  if (isSum)
    for (size_t i = 0; i < pathCnt; ++i) {
      Path p;
      p.reserve(polyCnt);

      for (size_t j = 0; j < poly.size(); ++j) {
        p.push_back(IntPoint(path[i].X + poly[j].X, path[i].Y + poly[j].Y));
      }

      pp.push_back(p);
    }
  else
    for (size_t i = 0; i < pathCnt; ++i) {
      Path p;
      p.reserve(polyCnt);

      for (size_t j = 0; j < poly.size(); ++j) {
        p.push_back(IntPoint(path[i].X - poly[j].X, path[i].Y - poly[j].Y));
      }

      pp.push_back(p);
    }

  Paths quads;
  quads.reserve((pathCnt + delta) * (polyCnt + 1));

  for (size_t i = 0; i < pathCnt - 1 + delta; ++i)
    for (size_t j = 0; j < polyCnt; ++j) {
      Path quad;
      quad.reserve(4);
      quad.push_back(pp[i % pathCnt][j % polyCnt]);
      quad.push_back(pp[(i + 1) % pathCnt][j % polyCnt]);
      quad.push_back(pp[(i + 1) % pathCnt][(j + 1) % polyCnt]);
      quad.push_back(pp[i % pathCnt][(j + 1) % polyCnt]);

      if (!Orientation(quad)) {
        ReversePath(quad);
      }

      quads.push_back(quad);
    }

  Clipper c;
  c.AddPaths(quads, ptSubject, true);
  c.Execute(ctUnion, solution, pftNonZero, pftNonZero);
}


void MinkowskiSum(const Path& pattern, const Path& path, Paths& solution, bool pathIsClosed)
{
  Minkowski(pattern, path, solution, true, pathIsClosed);
}


void MinkowskiSum(const Path& pattern, const Paths& paths, Paths& solution,
    PolyFillType pathFillType, bool pathIsClosed)
{
  Clipper c;

  for (size_t i = 0; i < paths.size(); ++i) {
    Paths tmp;
    Minkowski(pattern, paths[i], tmp, true, pathIsClosed);
    c.AddPaths(tmp, ptSubject, true);
  }

  if (pathIsClosed) {
    c.AddPaths(paths, ptClip, true);
  }

  c.Execute(ctUnion, solution, pathFillType, pathFillType);
}


void MinkowskiDiff(const Path& poly1, const Path& poly2, Paths& solution)
{
  Minkowski(poly1, poly2, solution, false, true);
}


enum NodeType {ntAny, ntOpen, ntClosed};

void AddPolyNodeToPolygons(const PolyNode& polynode, NodeType nodetype, Paths& paths)
{
  bool match = true;

  if (nodetype == ntClosed) {
    match = !polynode.IsOpen();
  }
  else if (nodetype == ntOpen) {
    return;
  }

  if (!polynode.Contour.empty() && match) {
    paths.push_back(polynode.Contour);
  }

  for (int i = 0; i < polynode.ChildCount(); ++i) {
    AddPolyNodeToPolygons(*polynode.Childs[i], nodetype, paths);
  }
}


void PolyTreeToPaths(const PolyTree& polytree, Paths& paths)
{
  paths.resize(0);
  paths.reserve(polytree.Total());
  AddPolyNodeToPolygons(polytree, ntAny, paths);
}


void ClosedPathsFromPolyTree(const PolyTree& polytree, Paths& paths)
{
  paths.resize(0);
  paths.reserve(polytree.Total());
  AddPolyNodeToPolygons(polytree, ntClosed, paths);
}


void OpenPathsFromPolyTree(PolyTree& polytree, Paths& paths)
{
  paths.resize(0);
  paths.reserve(polytree.Total());

  //Open paths are top level only, so ...
  for (int i = 0; i < polytree.ChildCount(); ++i)
    if (polytree.Childs[i]->IsOpen()) {
      paths.push_back(polytree.Childs[i]->Contour);
    }
}


std::ostream& operator <<(std::ostream& s, const IntPoint& p)
{
  //s << "(" << p.X << "," << p.Y << ")";
  return s;
}


std::ostream& operator <<(std::ostream& s, const Path& p)
{
  if (p.empty()) {
    return s;
  }

  Path::size_type last = p.size() - 1;

  for (Path::size_type i = 0; i < last; i++) {
    //s << "(" << p[i].X << "," << p[i].Y << "), ";
  }

  //s << "(" << p[last].X << "," << p[last].Y << ")\n";
  return s;
}


std::ostream& operator <<(std::ostream& s, const Paths& p)
{
  for (Paths::size_type i = 0; i < p.size(); i++) {
    s << p[i];
  }

  s << "\n";
  return s;
}


#ifdef use_deprecated

void OffsetPaths(const Paths& in_polys, Paths& out_polys,
    double delta, JoinType jointype, EndType_ endtype, double limit = 0)
{
  ClipperOffset co(limit, limit);
  co.AddPaths(in_polys, jointype, (EndType)endtype);
  co.Execute(out_polys, delta);
}


#endif

