#ifndef _CLIPPER_C_INL_
#define _CLIPPER_C_INL_

struct PolyNode_t
{
  IntPoint* Contour;
  int Contour_size;
  PolyNode_t** Childs;
  int Childs_size;
  PolyNode_t* Parent;
  bool m_IsOpen;
  JoinType m_jointype;
  EndType m_endtype;
};
struct PolyTree_t: public PolyNode_t
{
  PolyNode_t** AllNodes;
  int AllNodes_size;
};

void SimplifyPolygon1(const IPath& in_poly, Paths& out_polys, PolyFillType fillType = pftEvenOdd);
void SimplifyPolygons1(const Paths& in_polys, Paths& out_polys, PolyFillType fillType = pftEvenOdd);
void SimplifyPolygons1(Paths& polys, PolyFillType fillType = pftEvenOdd);
void CleanPolygon_(const IPath& in_poly, IPath& out_poly, double distance = 1.415);
void CleanPolygon_(IPath& poly, double distance = 1.415);
void CleanPolygons1(const Paths& in_polys, Paths& out_polys, double distance = 1.415);
void CleanPolygons1(Paths& polys, double distance = 1.415);
void MinkowskiSum_(const IPath& pattern, const IPath& path, Paths& solution, bool pathIsClosed);
void MinkowskiSum_(const IPath& pattern, const Paths& paths, Paths& solution, bool pathIsClosed);
void MinkowskiDiff_(const IPath& poly1, const IPath& poly2, Paths& solution);
void PolyTreeToPaths(const PolyTree_t* polytree, Paths& paths);
void ClosedPathsFromPolyTree(const PolyTree_t* polytree, Paths& paths);
void OpenPathsFromPolyTree(PolyTree_t* polytree, Paths& paths);

void ReversePath_(IntPoint* poly, int size)
{
  int i;
  IntPoint t;
  for (i=0; i<size/2; ++i) {
    CC_SWAP(poly[i], poly[size-1-i], t);
  }
}
void ReversePaths_(Paths& p);
struct TEdge_t;
struct IntersectNode_t;
struct LocalMinimum_t;
struct Scanbeam_t;
struct OutPt_t;
struct OutRec_t;
struct Join_t;
typedef std::vector < OutRec_t* > PolyOutList_t;
typedef std::vector < TEdge_t* > EdgeList_t;
typedef std::vector < Join_t* > JoinList_t;
typedef std::vector < IntersectNode_t* > IntersectList_t;
typedef std::vector<LocalMinimum_t> MinimaList;

typedef std::priority_queue<cInt> ScanbeamList;

struct Clipper_t
{
  LocalMinimum_t* m_CurrentLM;
  MinimaList m_MinimaList;
  bool m_UseFullRange;
  EdgeList_t m_edges;
  bool m_PreserveCollinear;
  bool m_HasOpenPaths;

  //set the callback function for z value filling on intersections (otherwise Z is 0)
  PolyOutList_t m_PolyOuts;
  JoinList_t m_Joins;
  JoinList_t m_GhostJoins;
  IntersectList_t m_IntersectList;
  ClipType m_ClipType;
  ScanbeamList m_Scanbeam;
  TEdge_t* m_ActiveEdges;
  TEdge_t* m_SortedEdges;
  bool m_ExecuteLocked;
  PolyFillType m_ClipFillType;
  PolyFillType m_SubjFillType;
  bool m_ReverseOutput;
  bool m_UsingPolyTree;
  bool m_StrictSimple;
#ifdef use_xyz
  ZFillCallback m_ZFill; //custom callback
#endif
};
struct ClipperOffset_t
{
  ClipperOffset_t(double miterLimit = 2.0, double roundPrecision = 0.25);
  ~ClipperOffset_t();
  void Clear();
  double MiterLimit;
  double ArcTolerance;

  Paths m_destPolys;
  IPath m_srcPoly;
  IPath m_destPoly;
  std::vector<DoublePoint> m_normals;
  double m_delta, m_sinA, m_sin, m_cos;
  double m_miterLim, m_StepsPerRad;
  IntPoint m_lowest;
  PolyNode_t* m_polyNodes;
  void FixOrientations();
  void DoOffset(double delta);
  void OffsetPoint(int j, int& k, JoinType jointype);
  void DoSquare(int j, int k);
  void DoMiter(int j, int k, double r);
  void DoRound(int j, int k);
};
struct TEdge_t {
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
  TEdge_t* Next;
  TEdge_t* Prev;
  TEdge_t* NextInLML;
  TEdge_t* NextInAEL;
  TEdge_t* PrevInAEL;
  TEdge_t* NextInSEL;
  TEdge_t* PrevInSEL;
};
struct IntersectNode_t {
  TEdge_t* Edge1;
  TEdge_t* Edge2;
  IntPoint Pt;
};
struct LocalMinimum_t {
  cInt Y;
  TEdge_t* LeftBound;
  TEdge_t* RightBound;
};
struct OutPt_t;
struct OutRec_t {
  int Idx;
  bool IsHole;
  bool IsOpen;
  OutRec_t* FirstLeft; //see comments in clipper.pas
  PolyNode_t* PolyNd;
  OutPt_t* Pts;
  OutPt_t* BottomPt;
};
struct OutPt_t {
  int Idx;
  IntPoint Pt;
  OutPt_t* Next;
  OutPt_t* Prev;
};
struct Join_t {
  OutPt_t* OutPt1;
  OutPt_t* OutPt2;
  IntPoint OffPt;
};
struct LocMinSorter_t {
  inline bool operator()(const LocalMinimum_t& locMin1, const LocalMinimum_t& locMin2) {
    return locMin2.Y < locMin1.Y;
  }
};
PolyNode_t* PolyTree_GetFirst(const PolyTree_t* s)
{
  if (s->Childs_size>0) {
    return s->Childs[0];
  }
  else {
    return 0;
  }
}
int PolyTree_Total(const PolyTree_t* s)
{
  int result = (int)s->AllNodes_size;
  //with negative offsets, ignore the hidden outer polygon ...
  if (result > 0 && s->Childs[0] != s->AllNodes[0]) {
    result--;
  }
  return result;
}
// PolyNode_t methods ...
PolyNode_t* newPolyNode_t()
{
  PolyNode_t* s = NULL;
  MYREALLOC(s, 1);
  MEMSET(s, 0, 1);
  return s;
}
void freePolyNode_t(PolyNode_t* s) {
  if (s) {
    int i;
    for (i=0; i<s->Childs_size; ++i) {
      freePolyNode_t(s->Childs[i]);
      s->Childs[i] = NULL;
    }
    FREE(s->Contour);
    FREE(s);
  }
}
void PolyNode_AddChild(PolyNode_t* s, PolyNode_t* child)
{
  unsigned cnt = (unsigned)s->Childs_size++;
  MYREALLOC(s->Childs, s->Childs_size);
  s->Childs[cnt] = (child);
  child->Parent = s;
}
bool PolyNode_IsHole(const PolyNode_t* s)
{
  bool result = true;
  const PolyNode_t* node = s->Parent;
  while (node) {
    result = !result;
    node = node->Parent;
  }
  return result;
}
void PolyTree_Clear(PolyTree_t* s)
{
  for (int i = 0; i < s->AllNodes_size; ++i) {
    freePolyNode_t(s->AllNodes[i]);
    s->AllNodes[i] = NULL;
  }
  s->AllNodes_size = (0);
  s->Childs_size = (0);
}
void PolyTree_Free(PolyTree_t* s) {
  freePolyNode_t(s);
  PolyTree_Clear(s);
  FREE(s->AllNodes);
}
// PolyTree_t methods ...
#ifndef use_int32
// Int128_t struct (enables safe math on signed 64bit integers)
// eg Int128_t val1((long64)9223372036854775807); //ie 2^63 -1
// Int128_t val2((long64)9223372036854775807);
// Int128_t val3 = val1 * val2;
// val3.AsString => "85070591730234615847396907784232501249" (8.5e+37)

struct Int128_t
{
  ulong64 lo;
  long64 hi;
  Int128_t(long64 _lo = 0) {
    lo = (ulong64)_lo;
    if (_lo < 0) {
      hi = -1;
    }
    else {
      hi = 0;
    }
  }
  Int128_t(const Int128_t& val): lo(val.lo), hi(val.hi) {}
  Int128_t(const long64& _hi, const ulong64& _lo): lo(_lo), hi(_hi) {}
  Int128_t& operator = (const long64& val) {
    lo = (ulong64)val;
    if (val < 0) {
      hi = -1;
    }
    else {
      hi = 0;
    }
    return *this;
  }
  bool operator == (const Int128_t& val) const {
    return (hi == val.hi && lo == val.lo);
  }
  bool operator != (const Int128_t& val) const {
    return !(*this == val);
  }
  bool operator > (const Int128_t& val) const {
    if (hi != val.hi) {
      return hi > val.hi;
    }
    else {
      return lo > val.lo;
    }
  }
  bool operator < (const Int128_t& val) const {
    if (hi != val.hi) {
      return hi < val.hi;
    }
    else {
      return lo < val.lo;
    }
  }
  bool operator >= (const Int128_t& val) const {
    return !(*this < val);
  }
  bool operator <= (const Int128_t& val) const {
    return !(*this > val);
  }
  Int128_t& operator += (const Int128_t& rhs) {
    lo += rhs.lo;
    hi += rhs.hi + (lo < rhs.lo);
    return *this;
  }
  Int128_t operator + (const Int128_t& rhs) const {
    Int128_t result(*this);
    result += rhs;
    return result;
  }
  Int128_t& operator -= (const Int128_t& rhs) {
    *this += -rhs;
    return *this;
  }
  Int128_t operator - (const Int128_t& rhs) const {
    Int128_t result(*this);
    result -= rhs;
    return result;
  }
  Int128_t operator-() const { //unary negation
    if (lo == 0) {
      return Int128_t(-hi, 0);
    }
    else {
      return Int128_t(~hi, ~lo + 1);
    }
  }
  operator double() const {
    const double shift64 = 18446744073709551616.0; //2^64
    if (hi < 0) {
      if (lo == 0) {
        return (double)hi * shift64;
      }
      else {
        return -(double)(~lo + ~hi * shift64);
      }
    }
    else {
      return (double)(lo + hi * shift64);
    }
  }
};
Int128_t int128mul(long64 lhs, long64 rhs)
{
  bool negate = (lhs < 0) != (rhs < 0);
  if (lhs < 0) {
    lhs = -lhs;
  }
  ulong64 int1Hi = ulong64(lhs) >> 32;
  ulong64 int1Lo = ulong64(lhs & 0xFFFFFFFF);
  if (rhs < 0) {
    rhs = -rhs;
  }
  ulong64 int2Hi = ulong64(rhs) >> 32;
  ulong64 int2Lo = ulong64(rhs & 0xFFFFFFFF);
  //nb: see comments in clipper.pas
  ulong64 a = int1Hi * int2Hi;
  ulong64 b = int1Lo * int2Lo;
  ulong64 c = int1Hi * int2Lo + int1Lo * int2Hi;
  Int128_t tmp;
  tmp.hi = long64(a + (c >> 32));
  tmp.lo = long64(c << 32);
  tmp.lo += long64(b);
  if (tmp.lo < b) {
    tmp.hi++;
  }
  if (negate) {
    tmp = -tmp;
  }
  return tmp;
};
#endif
double Area(const OutRec_t& outRec)
{
  OutPt_t* op = outRec.Pts;
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
bool PointIsVertex(const IntPoint& Pt, OutPt_t* pp)
{
  OutPt_t* pp2 = pp;
  do {
    if (pp2->Pt == Pt) {
      return true;
    }
    pp2 = pp2->Next;
  }
  while (pp2 != pp);
  return false;
}
int PointInPolygon1(const IntPoint& pt, const IntPoint* path, int size)
{
  //returns 0 if false, +1 if true, -1 if pt ON polygon boundary
  //See "The Point in Polygon Problem for Arbitrary Polygons" by Hormann & Agathos
  //http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.88.5498&rep=rep1&type=pdf
  int result = 0;
  size_t cnt = size;
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
int PointInPolygon1(const IntPoint& pt, OutPt_t* op)
{
  //returns 0 if false, +1 if true, -1 if pt ON polygon boundary
  int result = 0;
  OutPt_t* startOp = op;
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
bool Poly2ContainsPoly1(OutPt_t* OutPt1, OutPt_t* OutPt2)
{
  OutPt_t* op = OutPt1;
  do {
    //nb: PointInPolygon1 returns 0 if false, +1 if true, -1 if pt on polygon
    int res = PointInPolygon1(op->Pt, OutPt2);
    if (res >= 0) {
      return res > 0;
    }
    op = op->Next;
  }
  while (op != OutPt1);
  return true;
}
//----------------------------------------------------------------------
bool SlopesEqual1(const TEdge_t& e1, const TEdge_t& e2, bool UseFullInt64Range)
{
#ifndef use_int32
  if (UseFullInt64Range) {
    return int128mul(e1.Delta.Y, e2.Delta.X) == int128mul(e1.Delta.X, e2.Delta.Y);
  }
  else
#endif
    return e1.Delta.Y * e2.Delta.X == e1.Delta.X * e2.Delta.Y;
}
bool SlopesEqual1(const IntPoint pt1, const IntPoint pt2,
    const IntPoint pt3, bool UseFullInt64Range)
{
#ifndef use_int32
  if (UseFullInt64Range) {
    return int128mul(pt1.Y - pt2.Y, pt2.X - pt3.X) == int128mul(pt1.X - pt2.X, pt2.Y - pt3.Y);
  }
  else
#endif
    return (pt1.Y - pt2.Y) * (pt2.X - pt3.X) == (pt1.X - pt2.X) * (pt2.Y - pt3.Y);
}
bool SlopesEqual1(const IntPoint pt1, const IntPoint pt2,
    const IntPoint pt3, const IntPoint pt4, bool UseFullInt64Range)
{
#ifndef use_int32
  if (UseFullInt64Range) {
    return int128mul(pt1.Y - pt2.Y, pt3.X - pt4.X) == int128mul(pt1.X - pt2.X, pt3.Y - pt4.Y);
  }
  else
#endif
    return (pt1.Y - pt2.Y) * (pt3.X - pt4.X) == (pt1.X - pt2.X) * (pt3.Y - pt4.Y);
}
inline bool IsHorizontal(TEdge_t& e)
{
  return e.Delta.Y == 0;
}
inline double GetDx1(const IntPoint pt1, const IntPoint pt2)
{
  return (pt1.Y == pt2.Y) ?
      HORIZONTAL : (double)(pt2.X - pt1.X) / (pt2.Y - pt1.Y);
}
//---------------------------------------------------------------------------
inline void SetDx(TEdge_t& e)
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
//---------------------------------------------------------------------------
inline void SwapSides(TEdge_t& Edge1, TEdge_t& Edge2)
{
  EdgeSide Side = Edge1.Side;
  Edge1.Side = Edge2.Side;
  Edge2.Side = Side;
}
inline void SwapPolyIndexes(TEdge_t& Edge1, TEdge_t& Edge2)
{
  int OutIdx = Edge1.OutIdx;
  Edge1.OutIdx = Edge2.OutIdx;
  Edge2.OutIdx = OutIdx;
}
inline cInt TopX(TEdge_t& edge, const cInt currentY)
{
  return (currentY == edge.Top.Y) ?
      edge.Top.X : edge.Bot.X + Round(edge.Dx * (currentY - edge.Bot.Y));
}
void IntersectPoint(TEdge_t& Edge1, TEdge_t& Edge2, IntPoint& ip)
{
#ifdef use_xyz
  ip.Z = 0;
#endif
  double b1, b2;
  if (Edge1.Dx == Edge2.Dx) {
    ip.Y = Edge1.Curr.Y;
    ip.X = TopX(Edge1, ip.Y);
    return;
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
  //finally, don't allow 'ip' to be BELOW curr.Y (ie bottom of scanbeam) ...
  if (ip.Y > Edge1.Curr.Y) {
    ip.Y = Edge1.Curr.Y;
    //use the more vertical edge to derive X ...
    if (fabs(Edge1.Dx) > fabs(Edge2.Dx)) {
      ip.X = TopX(Edge2, ip.Y);
    }
    else {
      ip.X = TopX(Edge1, ip.Y);
    }
  }
}
void ReversePolyPtLinks(OutPt_t* pp)
{
  if (!pp) {
    return;
  }
  OutPt_t* pp1, *pp2;
  pp1 = pp;
  do {
    pp2 = pp1->Next;
    pp1->Next = pp1->Prev;
    pp1->Prev = pp2;
    pp1 = pp2;
  }
  while (pp1 != pp);
}
void DisposeOutPts(OutPt_t*& pp)
{
  if (pp == 0) {
    return;
  }
  pp->Prev->Next = 0;
  while (pp) {
    OutPt_t* tmpPp = pp;
    pp = pp->Next;
    delete tmpPp;
  }
}
inline void InitEdge(TEdge_t* e, TEdge_t* eNext, TEdge_t* ePrev, const IntPoint& Pt)
{
  memset(e, 0, sizeof(TEdge_t));
  e->Next = eNext;
  e->Prev = ePrev;
  e->Curr = Pt;
  e->OutIdx = Unassigned;
}
void InitEdge2(TEdge_t& e, PolyType Pt)
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
TEdge_t* RemoveEdge(TEdge_t* e)
{
  //removes e from double_linked_list (but without removing from memory)
  e->Prev->Next = e->Next;
  e->Next->Prev = e->Prev;
  TEdge_t* result = e->Next;
  e->Prev = 0; //flag as removed (see Clipper_t.Clear)
  return result;
}
inline void ReverseHorizontal(TEdge_t& e)
{
  //swap horizontal edges' Top and Bottom x's so they follow the natural
  //progression of the bounds - ie so their xbots will align with the
  //adjoining lower edge. [Helpful in the ProcessHorizontal() method.]
  Swap(e.Top.X, e.Bot.X);
#ifdef use_xyz
  Swap(e.Top.Z, e.Bot.Z);
#endif
}
void SwapPoints1(IntPoint& pt1, IntPoint& pt2)
{
  IntPoint tmp = pt1;
  pt1 = pt2;
  pt2 = tmp;
}
bool GetOverlapSegment1(IntPoint pt1a, IntPoint pt1b, IntPoint pt2a,
    IntPoint pt2b, IntPoint& pt1, IntPoint& pt2)
{
  //precondition: segments are Collinear.
  if (Abs(pt1a.X - pt1b.X) > Abs(pt1a.Y - pt1b.Y)) {
    if (pt1a.X > pt1b.X) {
      SwapPoints1(pt1a, pt1b);
    }
    if (pt2a.X > pt2b.X) {
      SwapPoints1(pt2a, pt2b);
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
      SwapPoints1(pt1a, pt1b);
    }
    if (pt2a.Y < pt2b.Y) {
      SwapPoints1(pt2a, pt2b);
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
bool FirstIsBottomPt(const OutPt_t* btmPt1, const OutPt_t* btmPt2)
{
  OutPt_t* p = btmPt1->Prev;
  while ((p->Pt == btmPt1->Pt) && (p != btmPt1)) {
    p = p->Prev;
  }
  double dx1p = fabs(GetDx1(btmPt1->Pt, p->Pt));
  p = btmPt1->Next;
  while ((p->Pt == btmPt1->Pt) && (p != btmPt1)) {
    p = p->Next;
  }
  double dx1n = fabs(GetDx1(btmPt1->Pt, p->Pt));
  p = btmPt2->Prev;
  while ((p->Pt == btmPt2->Pt) && (p != btmPt2)) {
    p = p->Prev;
  }
  double dx2p = fabs(GetDx1(btmPt2->Pt, p->Pt));
  p = btmPt2->Next;
  while ((p->Pt == btmPt2->Pt) && (p != btmPt2)) {
    p = p->Next;
  }
  double dx2n = fabs(GetDx1(btmPt2->Pt, p->Pt));
  return (dx1p >= dx2p && dx1p >= dx2n) || (dx1n >= dx2p && dx1n >= dx2n);
}
OutPt_t* GetBottomPt(OutPt_t* pp)
{
  OutPt_t* dups = 0;
  OutPt_t* p = pp->Next;
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
bool Pt2IsBetweenPt1AndPt3_(const IntPoint pt1,
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
bool HorzSegmentsOverlap_(cInt seg1a, cInt seg1b, cInt seg2a, cInt seg2b)
{
  if (seg1a > seg1b) {
    Swap(seg1a, seg1b);
  }
  if (seg2a > seg2b) {
    Swap(seg2a, seg2b);
  }
  return (seg1a < seg2b) && (seg2a < seg1b);
}
void Clipper_DisposeLocalMinimaList(Clipper_t* s)
{
  s->m_MinimaList.clear();
  s->m_CurrentLM = s->m_MinimaList.begin();
}
void Clipper_PopLocalMinima(Clipper_t* s)
{
  if (s->m_CurrentLM == s->m_MinimaList.end()) {
    return;
  }
  ++(s->m_CurrentLM);
}
void Clipper_Clear(Clipper_t* s)
{
  Clipper_DisposeLocalMinimaList(s);
  for (int i = 0; i < s->m_edges.size(); ++i) {
    //for each edge array in turn, find the first used edge and
    //check for and remove any hiddenPts in each edge in the array.
    TEdge_t* edges = s->m_edges[i];
    delete [] edges;
  }
  s->m_edges.clear();
  s->m_UseFullRange = false;
  s->m_HasOpenPaths = false;
}
void Clipper_Free(Clipper_t* s) {
  Clipper_Clear(s);
}
void RangeTest_(const IntPoint& Pt, bool& useFullRange)
{
  if (useFullRange) {
    if (Pt.X > hiRange || Pt.Y > hiRange || -Pt.X > hiRange || -Pt.Y > hiRange) {
      throw "Coordinate outside allowed range";
    }
  }
  else if (Pt.X > loRange || Pt.Y > loRange || -Pt.X > loRange || -Pt.Y > loRange) {
    useFullRange = true;
    RangeTest_(Pt, useFullRange);
  }
}
TEdge_t* FindNextLocMin(TEdge_t* E)
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
    TEdge_t* E2 = E;
    while (IsHorizontal(*E)) {
      E = E->Next;
    }
    if (E->Top.Y == E->Prev->Bot.Y) {
      continue; //ie just an intermediate horz.
    }
    if (E2->Prev->Bot.X < E->Bot.X) {
      E = E2;
    }
    break;
  }
  return E;
}
TEdge_t* Clipper_ProcessBound(Clipper_t* s, TEdge_t* E, bool NextIsForward)
{
  TEdge_t* Result = E;
  TEdge_t* Horz = 0;
  if (E->OutIdx == Skip) {
    //if edges still remain in the current bound beyond the skip edge then
    //create another LocMin and call ProcessBound once more
    if (NextIsForward) {
      while (E->Top.Y == E->Next->Bot.Y) {
        E = E->Next;
      }
      //don't include top horizontals when parsing a bound a second time,
      //they will be contained in the opposite bound ...
      while (E != Result && IsHorizontal(*E)) {
        E = E->Prev;
      }
    }
    else {
      while (E->Top.Y == E->Prev->Bot.Y) {
        E = E->Prev;
      }
      while (E != Result && IsHorizontal(*E)) {
        E = E->Next;
      }
    }
    if (E == Result) {
      if (NextIsForward) {
        Result = E->Next;
      }
      else {
        Result = E->Prev;
      }
    }
    else {
      //there are more edges in the bound beyond result starting with E
      if (NextIsForward) {
        E = Result->Next;
      }
      else {
        E = Result->Prev;
      }
      LocalMinimum_t locMin;
      locMin.Y = E->Bot.Y;
      locMin.LeftBound = 0;
      locMin.RightBound = E;
      E->WindDelta = 0;
      Result = Clipper_ProcessBound(s, E, NextIsForward);
      s->m_MinimaList.push_back(locMin);
    }
    return Result;
  }
  TEdge_t* EStart;
  if (IsHorizontal(*E)) {
    //We need to be careful with open paths because this may not be a
    //true local minima (ie E may be following a skip edge).
    //Also, consecutive horz. edges may start heading left before going right.
    if (NextIsForward) {
      EStart = E->Prev;
    }
    else {
      EStart = E->Next;
    }
    if (EStart->OutIdx != Skip) {
      if (IsHorizontal(*EStart)) { //ie an adjoining horizontal skip edge
        if (EStart->Bot.X != E->Bot.X && EStart->Top.X != E->Bot.X) {
          ReverseHorizontal(*E);
        }
      }
      else if (EStart->Bot.X != E->Bot.X) {
        ReverseHorizontal(*E);
      }
    }
  }
  EStart = E;
  if (NextIsForward) {
    while (Result->Top.Y == Result->Next->Bot.Y && Result->Next->OutIdx != Skip) {
      Result = Result->Next;
    }
    if (IsHorizontal(*Result) && Result->Next->OutIdx != Skip) {
      //nb: at the top of a bound, horizontals are added to the bound
      //only when the preceding edge attaches to the horizontal's left vertex
      //unless a Skip edge is encountered when that becomes the top divide
      Horz = Result;
      while (IsHorizontal(*Horz->Prev)) {
        Horz = Horz->Prev;
      }
      if (Horz->Prev->Top.X == Result->Next->Top.X) {
        if (!NextIsForward) {
          Result = Horz->Prev;
        }
      }
      else if (Horz->Prev->Top.X > Result->Next->Top.X) {
        Result = Horz->Prev;
      }
    }
    while (E != Result) {
      E->NextInLML = E->Next;
      if (IsHorizontal(*E) && E != EStart &&
          E->Bot.X != E->Prev->Top.X) {
        ReverseHorizontal(*E);
      }
      E = E->Next;
    }
    if (IsHorizontal(*E) && E != EStart && E->Bot.X != E->Prev->Top.X) {
      ReverseHorizontal(*E);
    }
    Result = Result->Next; //move to the edge just beyond current bound
  }
  else {
    while (Result->Top.Y == Result->Prev->Bot.Y && Result->Prev->OutIdx != Skip) {
      Result = Result->Prev;
    }
    if (IsHorizontal(*Result) && Result->Prev->OutIdx != Skip) {
      Horz = Result;
      while (IsHorizontal(*Horz->Next)) {
        Horz = Horz->Next;
      }
      if (Horz->Next->Top.X == Result->Prev->Top.X) {
        if (!NextIsForward) {
          Result = Horz->Next;
        }
      }
      else if (Horz->Next->Top.X > Result->Prev->Top.X) {
        Result = Horz->Next;
      }
    }
    while (E != Result) {
      E->NextInLML = E->Prev;
      if (IsHorizontal(*E) && E != EStart && E->Bot.X != E->Next->Top.X) {
        ReverseHorizontal(*E);
      }
      E = E->Prev;
    }
    if (IsHorizontal(*E) && E != EStart && E->Bot.X != E->Next->Top.X) {
      ReverseHorizontal(*E);
    }
    Result = Result->Prev; //move to the edge just beyond current bound
  }
  return Result;
}
bool Clipper_AddPath(Clipper_t* s, const IPath& pg, PolyType PolyTyp, bool Closed)
{
#ifdef use_lines
  if (!Closed && PolyTyp == ptClip) {
    throw clipperException("AddPath: Open paths must be subject.");
  }
#else
  if (!Closed) {
    throw clipperException("AddPath: Open paths have been disabled.");
  }
#endif
  int highI = (int)pg.size() - 1;
  if (Closed) {
    while (highI > 0 && (pg[highI] == pg[0])) {
      --highI;
    }
  }
  while (highI > 0 && (pg[highI] == pg[highI - 1])) {
    --highI;
  }
  if ((Closed && highI < 2) || (!Closed && highI < 1)) {
    return false;
  }
  //create a new edge array ...
  TEdge_t* edges = new TEdge_t [highI + 1];
  bool IsFlat = true;
  //1. Basic (first) edge initialization ...
  try {
    edges[1].Curr = pg[1];
    RangeTest_(pg[0], s->m_UseFullRange);
    RangeTest_(pg[highI], s->m_UseFullRange);
    InitEdge(&edges[0], &edges[1], &edges[highI], pg[0]);
    InitEdge(&edges[highI], &edges[0], &edges[highI - 1], pg[highI]);
    for (int i = highI - 1; i >= 1; --i) {
      RangeTest_(pg[i], s->m_UseFullRange);
      InitEdge(&edges[i], &edges[i + 1], &edges[i - 1], pg[i]);
    }
  }
  catch (...) {
    delete [] edges;
    throw; //range test fails
  }
  TEdge_t* eStart = &edges[0];
  //2. Remove duplicate vertices, and (when closed) collinear edges ...
  TEdge_t* E = eStart, *eLoopStop = eStart;
  for (;;) {
    //nb: allows matching start and end points when not Closed ...
    if (E->Curr == E->Next->Curr && (Closed || E->Next != eStart)) {
      if (E == E->Next) {
        break;
      }
      if (E == eStart) {
        eStart = E->Next;
      }
      E = RemoveEdge(E);
      eLoopStop = E;
      continue;
    }
    if (E->Prev == E->Next) {
      break; //only two vertices
    }
    else if (Closed &&
        SlopesEqual1(E->Prev->Curr, E->Curr, E->Next->Curr, s->m_UseFullRange) &&
        (!s->m_PreserveCollinear ||
            !Pt2IsBetweenPt1AndPt3_(E->Prev->Curr, E->Curr, E->Next->Curr))) {
      //Collinear edges are allowed for open paths but in closed paths
      //the default is to merge adjacent collinear edges into a single edge.
      //However, if the PreserveCollinear property is enabled, only overlapping
      //collinear edges (ie spikes) will be removed from closed paths.
      if (E == eStart) {
        eStart = E->Next;
      }
      E = RemoveEdge(E);
      E = E->Prev;
      eLoopStop = E;
      continue;
    }
    E = E->Next;
    if ((E == eLoopStop) || (!Closed && E->Next == eStart)) {
      break;
    }
  }
  if ((!Closed && (E == E->Next)) || (Closed && (E->Prev == E->Next))) {
    delete [] edges;
    return false;
  }
  if (!Closed) {
    s->m_HasOpenPaths = true;
    eStart->Prev->OutIdx = Skip;
  }
  //3. Do second stage of edge initialization ...
  E = eStart;
  do {
    InitEdge2(*E, PolyTyp);
    E = E->Next;
    if (IsFlat && E->Curr.Y != eStart->Curr.Y) {
      IsFlat = false;
    }
  }
  while (E != eStart);
  //4. Finally, add edge bounds to LocalMinima list ...
  //Totally flat paths must be handled differently when adding them
  //to LocalMinima list to avoid endless loops etc ...
  if (IsFlat) {
    if (Closed) {
      delete [] edges;
      return false;
    }
    E->Prev->OutIdx = Skip;
    if (E->Prev->Bot.X < E->Prev->Top.X) {
      ReverseHorizontal(*E->Prev);
    }
    LocalMinimum_t locMin;
    locMin.Y = E->Bot.Y;
    locMin.LeftBound = 0;
    locMin.RightBound = E;
    locMin.RightBound->Side = esRight;
    locMin.RightBound->WindDelta = 0;
    while (E->Next->OutIdx != Skip) {
      E->NextInLML = E->Next;
      if (E->Bot.X != E->Prev->Top.X) {
        ReverseHorizontal(*E);
      }
      E = E->Next;
    }
    s->m_MinimaList.push_back(locMin);
    s->m_edges.push_back(edges);
    return true;
  }
  s->m_edges.push_back(edges);
  bool leftBoundIsForward;
  TEdge_t* EMin = 0;
  //workaround to avoid an endless loop in the while loop below when
  //open paths have matching start and end points ...
  if (E->Prev->Bot == E->Prev->Top) {
    E = E->Next;
  }
  for (;;) {
    E = FindNextLocMin(E);
    if (E == EMin) {
      break;
    }
    else if (!EMin) {
      EMin = E;
    }
    //E and E.Prev now share a local minima (left aligned if horizontal).
    //Compare their slopes to find which starts which bound ...
    LocalMinimum_t locMin;
    locMin.Y = E->Bot.Y;
    if (E->Dx < E->Prev->Dx) {
      locMin.LeftBound = E->Prev;
      locMin.RightBound = E;
      leftBoundIsForward = false; //Q.nextInLML = Q.prev
    }
    else {
      locMin.LeftBound = E;
      locMin.RightBound = E->Prev;
      leftBoundIsForward = true; //Q.nextInLML = Q.next
    }
    locMin.LeftBound->Side = esLeft;
    locMin.RightBound->Side = esRight;
    if (!Closed) {
      locMin.LeftBound->WindDelta = 0;
    }
    else if (locMin.LeftBound->Next == locMin.RightBound) {
      locMin.LeftBound->WindDelta = -1;
    }
    else {
      locMin.LeftBound->WindDelta = 1;
    }
    locMin.RightBound->WindDelta = -locMin.LeftBound->WindDelta;
    E = Clipper_ProcessBound(s, locMin.LeftBound, leftBoundIsForward);
    if (E->OutIdx == Skip) {
      E = Clipper_ProcessBound(s, E, leftBoundIsForward);
    }
    TEdge_t* E2 = Clipper_ProcessBound(s, locMin.RightBound, !leftBoundIsForward);
    if (E2->OutIdx == Skip) {
      E2 = Clipper_ProcessBound(s, E2, !leftBoundIsForward);
    }
    if (locMin.LeftBound->OutIdx == Skip) {
      locMin.LeftBound = 0;
    }
    else if (locMin.RightBound->OutIdx == Skip) {
      locMin.RightBound = 0;
    }
    s->m_MinimaList.push_back(locMin);
    if (!leftBoundIsForward) {
      E = E2;
    }
  }
  return true;
}
bool Clipper_AddPaths(Clipper_t* s, const Paths& ppg, PolyType PolyTyp, bool Closed)
{
  bool result = false;
  for (Paths::size_type i = 0; i < ppg.size(); ++i) {
    if (Clipper_AddPath(s, ppg[i], PolyTyp, Closed)) {
      result = true;
    }
  }
  return result;
}
void Clipper_ResetBase(Clipper_t* s)
{
  s->m_CurrentLM = s->m_MinimaList.begin();
  if (s->m_CurrentLM == s->m_MinimaList.end()) {
    return; //ie nothing to process
  }
  std::sort(s->m_MinimaList.begin(), s->m_MinimaList.end(), LocMinSorter_t());
  //reset all edges ...
  for (LocalMinimum_t* lm = s->m_MinimaList.begin(); lm != s->m_MinimaList.end(); ++lm) {
    TEdge_t* e = lm->LeftBound;
    if (e) {
      e->Curr = e->Bot;
      e->Side = esLeft;
      e->OutIdx = Unassigned;
    }
    e = lm->RightBound;
    if (e) {
      e->Curr = e->Bot;
      e->Side = esRight;
      e->OutIdx = Unassigned;
    }
  }
}
IntRect Clipper_GetBounds(Clipper_t* s)
{
  IntRect result;
  LocalMinimum_t* lm = s->m_MinimaList.begin();
  if (lm == s->m_MinimaList.end()) {
    result.left = result.top = result.right = result.bottom = 0;
    return result;
  }
  result.left = lm->LeftBound->Bot.X;
  result.top = lm->LeftBound->Bot.Y;
  result.right = lm->LeftBound->Bot.X;
  result.bottom = lm->LeftBound->Bot.Y;
  while (lm != s->m_MinimaList.end()) {
    result.bottom = MAX(result.bottom, lm->LeftBound->Bot.Y);
    TEdge_t* e = lm->LeftBound;
    for (;;) {
      TEdge_t* bottomE = e;
      while (e->NextInLML) {
        if (e->Bot.X < result.left) {
          result.left = e->Bot.X;
        }
        if (e->Bot.X > result.right) {
          result.right = e->Bot.X;
        }
        e = e->NextInLML;
      }
      result.left = MIN(result.left, e->Bot.X);
      result.right = MAX(result.right, e->Bot.X);
      result.left = MIN(result.left, e->Top.X);
      result.right = MAX(result.right, e->Top.X);
      result.top = MIN(result.top, e->Top.Y);
      if (bottomE == lm->LeftBound) {
        e = lm->RightBound;
      }
      else {
        break;
      }
    }
    ++lm;
  }
  return result;
}
// TClipper methods ...
void Clipper_Init(Clipper_t* s, int initOptions = 0)
{
  // Clipper_t struct methods ...
  s->m_CurrentLM = s->m_MinimaList.begin(); //begin() == end() here
  s->m_UseFullRange = false;

  s->m_ActiveEdges = 0;
  s->m_SortedEdges = 0;
  s->m_ExecuteLocked = false;
  s->m_UseFullRange = false;
  s->m_ReverseOutput = ((initOptions & ioReverseSolution) != 0);
  s->m_StrictSimple = ((initOptions & ioStrictlySimple) != 0);
  s->m_PreserveCollinear = ((initOptions & ioPreserveCollinear) != 0);
  s->m_HasOpenPaths = false;
#ifdef use_xyz
  s->m_ZFill = 0;
#endif
}
#ifdef use_xyz
void Clipper_ZFillFunction(Clipper_t* s, ZFillCallback zFillFunc)
{
  s->m_ZFill = zFillFunc;
}
#endif
void Clipper_InsertScanbeam(Clipper_t* s, const cInt Y)
{
  //if (!m_Scanbeam.empty() && Y == m_Scanbeam.top()) return;// avoid duplicates.
  s->m_Scanbeam.push(Y);
}
cInt Clipper_PopScanbeam(Clipper_t* s)
{
  const cInt Y = s->m_Scanbeam.top();
  s->m_Scanbeam.pop();
  while (!s->m_Scanbeam.empty() && Y == s->m_Scanbeam.top()) {
    s->m_Scanbeam.pop(); // Pop duplicates.
  }
  return Y;
}
void Clipper_Reset(Clipper_t* s)
{
  Clipper_ResetBase(s);
  s->m_Scanbeam = ScanbeamList();
  s->m_ActiveEdges = 0;
  s->m_SortedEdges = 0;
  int i;
  for (i=0; i < s->m_MinimaList.size(); ++i) {
    LocalMinimum_t* lm = s->m_MinimaList.begin() + i;
    Clipper_InsertScanbeam(s, lm->Y);
  }
}

bool Clipper_IsEvenOddFillType(const Clipper_t* s, const TEdge_t& edge)
{
  if (edge.PolyTyp == ptSubject) {
    return s->m_SubjFillType == pftEvenOdd;
  }
  else {
    return s->m_ClipFillType == pftEvenOdd;
  }
}
bool Clipper_IsEvenOddAltFillType(const Clipper_t* s, const TEdge_t& edge)
{
  if (edge.PolyTyp == ptSubject) {
    return s->m_ClipFillType == pftEvenOdd;
  }
  else {
    return s->m_SubjFillType == pftEvenOdd;
  }
}
bool Clipper_IsContributing(const Clipper_t* s, const TEdge_t& edge)
{
  PolyFillType pft, pft2;
  if (edge.PolyTyp == ptSubject) {
    pft = s->m_SubjFillType;
    pft2 = s->m_ClipFillType;
  }
  else {
    pft = s->m_ClipFillType;
    pft2 = s->m_SubjFillType;
  }
  switch (pft) {
  case pftEvenOdd:
    //return false if a subj line has been flagged as inside a subj polygon
    if (edge.WindDelta == 0 && edge.WindCnt != 1) {
      return false;
    }
    break;
  case pftNonZero:
    if (Abs(edge.WindCnt) != 1) {
      return false;
    }
    break;
  case pftPositive:
    if (edge.WindCnt != 1) {
      return false;
    }
    break;
  default: //pftNegative
    if (edge.WindCnt != -1) {
      return false;
    }
  }
  switch (s->m_ClipType) {
  case ctIntersection:
    switch (pft2) {
    case pftEvenOdd:
    case pftNonZero:
      return (edge.WindCnt2 != 0);
    case pftPositive:
      return (edge.WindCnt2 > 0);
    default:
      return (edge.WindCnt2 < 0);
    }
    break;
  case ctUnion:
    switch (pft2) {
    case pftEvenOdd:
    case pftNonZero:
      return (edge.WindCnt2 == 0);
    case pftPositive:
      return (edge.WindCnt2 <= 0);
    default:
      return (edge.WindCnt2 >= 0);
    }
    break;
  case ctDifference:
    if (edge.PolyTyp == ptSubject)
      switch (pft2) {
      case pftEvenOdd:
      case pftNonZero:
        return (edge.WindCnt2 == 0);
      case pftPositive:
        return (edge.WindCnt2 <= 0);
      default:
        return (edge.WindCnt2 >= 0);
      }
    else
      switch (pft2) {
      case pftEvenOdd:
      case pftNonZero:
        return (edge.WindCnt2 != 0);
      case pftPositive:
        return (edge.WindCnt2 > 0);
      default:
        return (edge.WindCnt2 < 0);
      }
    break;
  case ctXor:
    if (edge.WindDelta == 0) //XOr always contributing unless open
      switch (pft2) {
      case pftEvenOdd:
      case pftNonZero:
        return (edge.WindCnt2 == 0);
      case pftPositive:
        return (edge.WindCnt2 <= 0);
      default:
        return (edge.WindCnt2 >= 0);
      }
    else {
      return true;
    }
    break;
  default:
    return true;
  }
}
void Clipper_SetWindingCount(Clipper_t* s, TEdge_t& edge)
{
  TEdge_t* e = edge.PrevInAEL;
  //find the edge of the same polytype that immediately preceeds 'edge' in AEL
  while (e && ((e->PolyTyp != edge.PolyTyp) || (e->WindDelta == 0))) {
    e = e->PrevInAEL;
  }
  if (!e) {
    edge.WindCnt = (edge.WindDelta == 0 ? 1 : edge.WindDelta);
    edge.WindCnt2 = 0;
    e = s->m_ActiveEdges; //ie get ready to calc WindCnt2
  }
  else if (edge.WindDelta == 0 && s->m_ClipType != ctUnion) {
    edge.WindCnt = 1;
    edge.WindCnt2 = e->WindCnt2;
    e = e->NextInAEL; //ie get ready to calc WindCnt2
  }
  else if (Clipper_IsEvenOddFillType(s, edge)) {
    //EvenOdd filling ...
    if (edge.WindDelta == 0) {
      //are we inside a subj polygon ...
      bool Inside = true;
      TEdge_t* e2 = e->PrevInAEL;
      while (e2) {
        if (e2->PolyTyp == e->PolyTyp && e2->WindDelta != 0) {
          Inside = !Inside;
        }
        e2 = e2->PrevInAEL;
      }
      edge.WindCnt = (Inside ? 0 : 1);
    }
    else {
      edge.WindCnt = edge.WindDelta;
    }
    edge.WindCnt2 = e->WindCnt2;
    e = e->NextInAEL; //ie get ready to calc WindCnt2
  }
  else {
    //nonZero, Positive or Negative filling ...
    if (e->WindCnt * e->WindDelta < 0) {
      //prev edge is 'decreasing' WindCount (WC) toward zero
      //so we're outside the previous polygon ...
      if (Abs(e->WindCnt) > 1) {
        //outside prev poly but still inside another.
        //when reversing direction of prev poly use the same WC
        if (e->WindDelta * edge.WindDelta < 0) {
          edge.WindCnt = e->WindCnt;
        }
        //otherwise continue to 'decrease' WC ...
        else {
          edge.WindCnt = e->WindCnt + edge.WindDelta;
        }
      }
      else
        //now outside all polys of same polytype so set own WC ...
      {
        edge.WindCnt = (edge.WindDelta == 0 ? 1 : edge.WindDelta);
      }
    }
    else {
      //prev edge is 'increasing' WindCount (WC) away from zero
      //so we're inside the previous polygon ...
      if (edge.WindDelta == 0) {
        edge.WindCnt = (e->WindCnt < 0 ? e->WindCnt - 1 : e->WindCnt + 1);
      }
      //if wind direction is reversing prev then use same WC
      else if (e->WindDelta * edge.WindDelta < 0) {
        edge.WindCnt = e->WindCnt;
      }
      //otherwise add to WC ...
      else {
        edge.WindCnt = e->WindCnt + edge.WindDelta;
      }
    }
    edge.WindCnt2 = e->WindCnt2;
    e = e->NextInAEL; //ie get ready to calc WindCnt2
  }
  //update WindCnt2 ...
  if (Clipper_IsEvenOddAltFillType(s, edge)) {
    //EvenOdd filling ...
    while (e != &edge) {
      if (e->WindDelta != 0) {
        edge.WindCnt2 = (edge.WindCnt2 == 0 ? 1 : 0);
      }
      e = e->NextInAEL;
    }
  }
  else {
    //nonZero, Positive or Negative filling ...
    while (e != &edge) {
      edge.WindCnt2 += e->WindDelta;
      e = e->NextInAEL;
    }
  }
}
inline bool E2InsertsBeforeE1(TEdge_t& e1, TEdge_t& e2)
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
void Clipper_InsertEdgeIntoAEL(Clipper_t* s, TEdge_t* edge, TEdge_t* startEdge)
{
  if (!s->m_ActiveEdges) {
    edge->PrevInAEL = 0;
    edge->NextInAEL = 0;
    s->m_ActiveEdges = edge;
  }
  else if (!startEdge && E2InsertsBeforeE1(*s->m_ActiveEdges, *edge)) {
    edge->PrevInAEL = 0;
    edge->NextInAEL = s->m_ActiveEdges;
    s->m_ActiveEdges->PrevInAEL = edge;
    s->m_ActiveEdges = edge;
  }
  else {
    if (!startEdge) {
      startEdge = s->m_ActiveEdges;
    }
    while (startEdge->NextInAEL &&
      !E2InsertsBeforeE1(*startEdge->NextInAEL , *edge)) {
      startEdge = startEdge->NextInAEL;
    }
    edge->NextInAEL = startEdge->NextInAEL;
    if (startEdge->NextInAEL) {
      startEdge->NextInAEL->PrevInAEL = edge;
    }
    edge->PrevInAEL = startEdge;
    startEdge->NextInAEL = edge;
  }
}

void Clipper_AddEdgeToSEL(Clipper_t* s, TEdge_t* edge)
{
  //SEL pointers in PEdge are reused to build a list of horizontal edges.
  //However, we don't need to worry about order with horizontal edge processing.
  if (!s->m_SortedEdges) {
    s->m_SortedEdges = edge;
    edge->PrevInSEL = 0;
    edge->NextInSEL = 0;
  }
  else {
    edge->NextInSEL = s->m_SortedEdges;
    edge->PrevInSEL = 0;
    s->m_SortedEdges->PrevInSEL = edge;
    s->m_SortedEdges = edge;
  }
}
OutRec_t* Clipper_CreateOutRec(Clipper_t* s)
{
  OutRec_t* result = new OutRec_t;
  result->IsHole = false;
  result->IsOpen = false;
  result->FirstLeft = 0;
  result->Pts = 0;
  result->BottomPt = 0;
  result->PolyNd = 0;
  s->m_PolyOuts.push_back(result);
  result->Idx = (int)s->m_PolyOuts.size() - 1;
  return result;
}
void Clipper_SetHoleState(Clipper_t* s, TEdge_t* e, OutRec_t* outrec)
{
  bool IsHole = false;
  TEdge_t* e2 = e->PrevInAEL;
  while (e2) {
    if (e2->OutIdx >= 0 && e2->WindDelta != 0) {
      IsHole = !IsHole;
      if (! outrec->FirstLeft) {
        outrec->FirstLeft = s->m_PolyOuts[e2->OutIdx];
      }
    }
    e2 = e2->PrevInAEL;
  }
  if (IsHole) {
    outrec->IsHole = true;
  }
}
OutPt_t* Clipper_AddOutPt(Clipper_t* s, TEdge_t* e, const IntPoint& pt)
{
  bool ToFront = (e->Side == esLeft);
  if (e->OutIdx < 0) {
    OutRec_t* outRec = Clipper_CreateOutRec(s);
    outRec->IsOpen = (e->WindDelta == 0);
    OutPt_t* newOp = new OutPt_t;
    outRec->Pts = newOp;
    newOp->Idx = outRec->Idx;
    newOp->Pt = pt;
    newOp->Next = newOp;
    newOp->Prev = newOp;
    if (!outRec->IsOpen) {
      Clipper_SetHoleState(s, e, outRec);
    }
    e->OutIdx = outRec->Idx;
    return newOp;
  }
  else {
    OutRec_t* outRec = s->m_PolyOuts[e->OutIdx];
    //OutRec_t.Pts is the 'Left-most' point & OutRec_t.Pts.Prev is the 'Right-most'
    OutPt_t* op = outRec->Pts;
    if (ToFront && (pt == op->Pt)) {
      return op;
    }
    else if (!ToFront && (pt == op->Prev->Pt)) {
      return op->Prev;
    }
    OutPt_t* newOp = new OutPt_t;
    newOp->Idx = outRec->Idx;
    newOp->Pt = pt;
    newOp->Next = op;
    newOp->Prev = op->Prev;
    newOp->Prev->Next = newOp;
    op->Prev = newOp;
    if (ToFront) {
      outRec->Pts = newOp;
    }
    return newOp;
  }
}
void Clipper_CopyAELToSEL(Clipper_t* s)
{
  TEdge_t* e = s->m_ActiveEdges;
  s->m_SortedEdges = e;
  while (e) {
    e->PrevInSEL = e->PrevInAEL;
    e->NextInSEL = e->NextInAEL;
    e = e->NextInAEL;
  }
}
void Clipper_AddJoin(Clipper_t* s, OutPt_t* op1, OutPt_t* op2, const IntPoint OffPt)
{
  Join_t* j = new Join_t;
  j->OutPt1 = op1;
  j->OutPt2 = op2;
  j->OffPt = OffPt;
  s->m_Joins.push_back(j);
}
void Clipper_ClearJoins(Clipper_t* s)
{
  for (int i = 0; i < s->m_Joins.size(); i++) {
    delete s->m_Joins[i];
  }
  s->m_Joins.resize(0);
}
void Clipper_ClearGhostJoins(Clipper_t* s)
{
  for (int i = 0; i < s->m_GhostJoins.size(); i++) {
    delete s->m_GhostJoins[i];
  }
  s->m_GhostJoins.resize(0);
}
void Clipper_AddGhostJoin(Clipper_t* s, OutPt_t* op, const IntPoint OffPt)
{
  Join_t* j = new Join_t;
  j->OutPt1 = op;
  j->OutPt2 = 0;
  j->OffPt = OffPt;
  s->m_GhostJoins.push_back(j);
}

#ifdef use_xyz
void Clipper_SetZ(Clipper_t* s, IntPoint& pt, TEdge_t& e1, TEdge_t& e2)
{
  if (pt.Z != 0 || !m_ZFill) {
    return;
  }
  else if (pt == e1.Bot) {
    pt.Z = e1.Bot.Z;
  }
  else if (pt == e1.Top) {
    pt.Z = e1.Top.Z;
  }
  else if (pt == e2.Bot) {
    pt.Z = e2.Bot.Z;
  }
  else if (pt == e2.Top) {
    pt.Z = e2.Top.Z;
  }
  else {
    (*m_ZFill)(e1.Bot, e1.Top, e2.Bot, e2.Top, pt);
  }
}
#endif

OutPt_t* Clipper_AddLocalMinPoly(Clipper_t* s, TEdge_t* e1, TEdge_t* e2, const IntPoint& Pt)
{
  OutPt_t* result;
  TEdge_t* e, *prevE;
  if (IsHorizontal(*e2) || (e1->Dx > e2->Dx)) {
    result = Clipper_AddOutPt(s, e1, Pt);
    e2->OutIdx = e1->OutIdx;
    e1->Side = esLeft;
    e2->Side = esRight;
    e = e1;
    if (e->PrevInAEL == e2) {
      prevE = e2->PrevInAEL;
    }
    else {
      prevE = e->PrevInAEL;
    }
  }
  else {
    result = Clipper_AddOutPt(s, e2, Pt);
    e1->OutIdx = e2->OutIdx;
    e1->Side = esRight;
    e2->Side = esLeft;
    e = e2;
    if (e->PrevInAEL == e1) {
      prevE = e1->PrevInAEL;
    }
    else {
      prevE = e->PrevInAEL;
    }
  }
  if (prevE && prevE->OutIdx >= 0 &&
    (TopX(*prevE, Pt.Y) == TopX(*e, Pt.Y)) &&
    SlopesEqual1(*e, *prevE, s->m_UseFullRange) &&
    (e->WindDelta != 0) && (prevE->WindDelta != 0)) {
    OutPt_t* outPt = Clipper_AddOutPt(s, prevE, Pt);
    Clipper_AddJoin(s, result, outPt, e->Top);
  }
  return result;
}
bool Param1RightOfParam2(OutRec_t* outRec1, OutRec_t* outRec2)
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
OutRec_t* GetLowermostRec(OutRec_t* outRec1, OutRec_t* outRec2)
{
  //work out which polygon fragment has the correct hole state ...
  if (!outRec1->BottomPt) {
    outRec1->BottomPt = GetBottomPt(outRec1->Pts);
  }
  if (!outRec2->BottomPt) {
    outRec2->BottomPt = GetBottomPt(outRec2->Pts);
  }
  OutPt_t* OutPt1 = outRec1->BottomPt;
  OutPt_t* OutPt2 = outRec2->BottomPt;
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

void Clipper_AppendPolygon(Clipper_t* s, TEdge_t* e1, TEdge_t* e2)
{
  //get the start and ends of both output polygons ...
  OutRec_t* outRec1 = s->m_PolyOuts[e1->OutIdx];
  OutRec_t* outRec2 = s->m_PolyOuts[e2->OutIdx];
  OutRec_t* holeStateRec;
  if (Param1RightOfParam2(outRec1, outRec2)) {
    holeStateRec = outRec2;
  }
  else if (Param1RightOfParam2(outRec2, outRec1)) {
    holeStateRec = outRec1;
  }
  else {
    holeStateRec = GetLowermostRec(outRec1, outRec2);
  }
  //get the start and ends of both output polygons and
  //join e2 poly onto e1 poly and delete pointers to e2 ...
  OutPt_t* p1_lft = outRec1->Pts;
  OutPt_t* p1_rt = p1_lft->Prev;
  OutPt_t* p2_lft = outRec2->Pts;
  OutPt_t* p2_rt = p2_lft->Prev;
  EdgeSide Side;
  //join e2 poly onto e1 poly and delete pointers to e2 ...
  if (e1->Side == esLeft) {
    if (e2->Side == esLeft) {
      //z y x a b c
      ReversePolyPtLinks(p2_lft);
      p2_lft->Next = p1_lft;
      p1_lft->Prev = p2_lft;
      p1_rt->Next = p2_rt;
      p2_rt->Prev = p1_rt;
      outRec1->Pts = p2_rt;
    }
    else {
      //x y z a b c
      p2_rt->Next = p1_lft;
      p1_lft->Prev = p2_rt;
      p2_lft->Prev = p1_rt;
      p1_rt->Next = p2_lft;
      outRec1->Pts = p2_lft;
    }
    Side = esLeft;
  }
  else {
    if (e2->Side == esRight) {
      //a b c z y x
      ReversePolyPtLinks(p2_lft);
      p1_rt->Next = p2_rt;
      p2_rt->Prev = p1_rt;
      p2_lft->Next = p1_lft;
      p1_lft->Prev = p2_lft;
    }
    else {
      //a b c x y z
      p1_rt->Next = p2_lft;
      p2_lft->Prev = p1_rt;
      p1_lft->Prev = p2_rt;
      p2_rt->Next = p1_lft;
    }
    Side = esRight;
  }
  outRec1->BottomPt = 0;
  if (holeStateRec == outRec2) {
    if (outRec2->FirstLeft != outRec1) {
      outRec1->FirstLeft = outRec2->FirstLeft;
    }
    outRec1->IsHole = outRec2->IsHole;
  }
  outRec2->Pts = 0;
  outRec2->BottomPt = 0;
  outRec2->FirstLeft = outRec1;
  int OKIdx = e1->OutIdx;
  int ObsoleteIdx = e2->OutIdx;
  e1->OutIdx = Unassigned; //nb: safe because we only get here via AddLocalMaxPoly
  e2->OutIdx = Unassigned;
  TEdge_t* e = s->m_ActiveEdges;
  while (e) {
    if (e->OutIdx == ObsoleteIdx) {
      e->OutIdx = OKIdx;
      e->Side = Side;
      break;
    }
    e = e->NextInAEL;
  }
  outRec2->Idx = outRec1->Idx;
}
void Clipper_AddLocalMaxPoly(Clipper_t* s, TEdge_t* e1, TEdge_t* e2, const IntPoint& Pt)
{
  Clipper_AddOutPt(s, e1, Pt);
  if (e2->WindDelta == 0) {
    Clipper_AddOutPt(s, e2, Pt);
  }
  if (e1->OutIdx == e2->OutIdx) {
    e1->OutIdx = Unassigned;
    e2->OutIdx = Unassigned;
  }
  else if (e1->OutIdx < e2->OutIdx) {
    Clipper_AppendPolygon(s, e1, e2);
  }
  else {
    Clipper_AppendPolygon(s, e2, e1);
  }
}
void Clipper_IntersectEdges(Clipper_t* s, TEdge_t* e1, TEdge_t* e2, IntPoint& Pt)
{
  bool e1Contributing = (e1->OutIdx >= 0);
  bool e2Contributing = (e2->OutIdx >= 0);
#ifdef use_xyz
  SetZ(Pt, *e1, *e2);
#endif
#ifdef use_lines
  //if either edge is on an OPEN path ...
  if (e1->WindDelta == 0 || e2->WindDelta == 0) {
    //ignore subject-subject open path intersections UNLESS they
    //are both open paths, AND they are both 'contributing maximas' ...
    if (e1->WindDelta == 0 && e2->WindDelta == 0) {
      return;
    }
    //if intersecting a subj line with a subj poly ...
    else if (e1->PolyTyp == e2->PolyTyp &&
        e1->WindDelta != e2->WindDelta && m_ClipType == ctUnion) {
      if (e1->WindDelta == 0) {
        if (e2Contributing) {
          Clipper_AddOutPt(s, e1, Pt);
          if (e1Contributing) {
            e1->OutIdx = Unassigned;
          }
        }
      }
      else {
        if (e1Contributing) {
          Clipper_AddOutPt(s, e2, Pt);
          if (e2Contributing) {
            e2->OutIdx = Unassigned;
          }
        }
      }
    }
    else if (e1->PolyTyp != e2->PolyTyp) {
      //toggle subj open path OutIdx on/off when Abs(clip.WndCnt) == 1 ...
      if ((e1->WindDelta == 0) && abs(e2->WindCnt) == 1 &&
          (s->m_ClipType != ctUnion || e2->WindCnt2 == 0)) {
        Clipper_AddOutPt(s, e1, Pt);
        if (e1Contributing) {
          e1->OutIdx = Unassigned;
        }
      }
      else if ((e2->WindDelta == 0) && (abs(e1->WindCnt) == 1) &&
          (s->m_ClipType != ctUnion || e1->WindCnt2 == 0)) {
        Clipper_AddOutPt(s, e2, Pt);
        if (e2Contributing) {
          e2->OutIdx = Unassigned;
        }
      }
    }
    return;
  }
#endif
  //update winding counts...
  //assumes that e1 will be to the Right of e2 ABOVE the intersection
  if (e1->PolyTyp == e2->PolyTyp) {
    if (Clipper_IsEvenOddFillType(s, *e1)) {
      int oldE1WindCnt = e1->WindCnt;
      e1->WindCnt = e2->WindCnt;
      e2->WindCnt = oldE1WindCnt;
    }
    else {
      if (e1->WindCnt + e2->WindDelta == 0) {
        e1->WindCnt = -e1->WindCnt;
      }
      else {
        e1->WindCnt += e2->WindDelta;
      }
      if (e2->WindCnt - e1->WindDelta == 0) {
        e2->WindCnt = -e2->WindCnt;
      }
      else {
        e2->WindCnt -= e1->WindDelta;
      }
    }
  }
  else {
    if (!Clipper_IsEvenOddFillType(s, *e2)) {
      e1->WindCnt2 += e2->WindDelta;
    }
    else {
      e1->WindCnt2 = (e1->WindCnt2 == 0) ? 1 : 0;
    }
    if (!Clipper_IsEvenOddFillType(s, *e1)) {
      e2->WindCnt2 -= e1->WindDelta;
    }
    else {
      e2->WindCnt2 = (e2->WindCnt2 == 0) ? 1 : 0;
    }
  }
  PolyFillType e1FillType, e2FillType, e1FillType2, e2FillType2;
  if (e1->PolyTyp == ptSubject) {
    e1FillType = s->m_SubjFillType;
    e1FillType2 = s->m_ClipFillType;
  }
  else {
    e1FillType = s->m_ClipFillType;
    e1FillType2 = s->m_SubjFillType;
  }
  if (e2->PolyTyp == ptSubject) {
    e2FillType = s->m_SubjFillType;
    e2FillType2 = s->m_ClipFillType;
  }
  else {
    e2FillType = s->m_ClipFillType;
    e2FillType2 = s->m_SubjFillType;
  }
  cInt e1Wc, e2Wc;
  switch (e1FillType) {
  case pftPositive:
    e1Wc = e1->WindCnt;
    break;
  case pftNegative:
    e1Wc = -e1->WindCnt;
    break;
  default:
    e1Wc = Abs(e1->WindCnt);
  }
  switch (e2FillType) {
  case pftPositive:
    e2Wc = e2->WindCnt;
    break;
  case pftNegative:
    e2Wc = -e2->WindCnt;
    break;
  default:
    e2Wc = Abs(e2->WindCnt);
  }
  if (e1Contributing && e2Contributing) {
    if ((e1Wc != 0 && e1Wc != 1) || (e2Wc != 0 && e2Wc != 1) ||
        (e1->PolyTyp != e2->PolyTyp && s->m_ClipType != ctXor)) {
      Clipper_AddLocalMaxPoly(s, e1, e2, Pt);
    }
    else {
      Clipper_AddOutPt(s, e1, Pt);
      Clipper_AddOutPt(s, e2, Pt);
      SwapSides(*e1 , *e2);
      SwapPolyIndexes(*e1 , *e2);
    }
  }
  else if (e1Contributing) {
    if (e2Wc == 0 || e2Wc == 1) {
      Clipper_AddOutPt(s, e1, Pt);
      SwapSides(*e1, *e2);
      SwapPolyIndexes(*e1, *e2);
    }
  }
  else if (e2Contributing) {
    if (e1Wc == 0 || e1Wc == 1) {
      Clipper_AddOutPt(s, e2, Pt);
      SwapSides(*e1, *e2);
      SwapPolyIndexes(*e1, *e2);
    }
  }
  else if ((e1Wc == 0 || e1Wc == 1) && (e2Wc == 0 || e2Wc == 1)) {
    //neither edge is currently contributing ...
    cInt e1Wc2, e2Wc2;
    switch (e1FillType2) {
    case pftPositive:
      e1Wc2 = e1->WindCnt2;
      break;
    case pftNegative :
      e1Wc2 = -e1->WindCnt2;
      break;
    default:
      e1Wc2 = Abs(e1->WindCnt2);
    }
    switch (e2FillType2) {
    case pftPositive:
      e2Wc2 = e2->WindCnt2;
      break;
    case pftNegative:
      e2Wc2 = -e2->WindCnt2;
      break;
    default:
      e2Wc2 = Abs(e2->WindCnt2);
    }
    if (e1->PolyTyp != e2->PolyTyp) {
      Clipper_AddLocalMinPoly(s, e1, e2, Pt);
    }
    else if (e1Wc == 1 && e2Wc == 1)
      switch (s->m_ClipType) {
      case ctIntersection:
        if (e1Wc2 > 0 && e2Wc2 > 0) {
          Clipper_AddLocalMinPoly(s, e1, e2, Pt);
        }
        break;
      case ctUnion:
        if (e1Wc2 <= 0 && e2Wc2 <= 0) {
          Clipper_AddLocalMinPoly(s, e1, e2, Pt);
        }
        break;
      case ctDifference:
        if (((e1->PolyTyp == ptClip) && (e1Wc2 > 0) && (e2Wc2 > 0)) ||
            ((e1->PolyTyp == ptSubject) && (e1Wc2 <= 0) && (e2Wc2 <= 0))) {
          Clipper_AddLocalMinPoly(s, e1, e2, Pt);
        }
        break;
      case ctXor:
        Clipper_AddLocalMinPoly(s, e1, e2, Pt);
      }
    else {
      SwapSides(*e1, *e2);
    }
  }
}
void Clipper_InsertLocalMinimaIntoAEL(Clipper_t* s, const cInt botY)
{
  while (s->m_CurrentLM != s->m_MinimaList.end() && (s->m_CurrentLM->Y == botY)) {
    TEdge_t* lb = s->m_CurrentLM->LeftBound;
    TEdge_t* rb = s->m_CurrentLM->RightBound;
    Clipper_PopLocalMinima(s);
    OutPt_t* Op1 = 0;
    if (!lb) {
      //nb: don't insert LB into either AEL or SEL
      Clipper_InsertEdgeIntoAEL(s, rb, 0);
      Clipper_SetWindingCount(s, *rb);
      if (Clipper_IsContributing(s, *rb)) {
        Op1 = Clipper_AddOutPt(s, rb, rb->Bot);
      }
    }
    else if (!rb) {
      Clipper_InsertEdgeIntoAEL(s, lb, 0);
      Clipper_SetWindingCount(s, *lb);
      if (Clipper_IsContributing(s, *lb)) {
        Op1 = Clipper_AddOutPt(s, lb, lb->Bot);
      }
      Clipper_InsertScanbeam(s, lb->Top.Y);
    }
    else {
      Clipper_InsertEdgeIntoAEL(s, lb, 0);
      Clipper_InsertEdgeIntoAEL(s, rb, lb);
      Clipper_SetWindingCount(s, *lb);
      rb->WindCnt = lb->WindCnt;
      rb->WindCnt2 = lb->WindCnt2;
      if (Clipper_IsContributing(s, *lb)) {
        Op1 = Clipper_AddLocalMinPoly(s, lb, rb, lb->Bot);
      }
      Clipper_InsertScanbeam(s, lb->Top.Y);
    }
    if (rb) {
      if (IsHorizontal(*rb)) {
        Clipper_AddEdgeToSEL(s, rb);
      }
      else {
        Clipper_InsertScanbeam(s, rb->Top.Y);
      }
    }
    if (!lb || !rb) {
      continue;
    }
    //if any output polygons share an edge, they'll need joining later ...
    if (Op1 && IsHorizontal(*rb) &&
      s->m_GhostJoins.size() > 0 && (rb->WindDelta != 0)) {
      for (JoinList_t::size_type i = 0; i < s->m_GhostJoins.size(); ++i) {
        Join_t* jr = s->m_GhostJoins[i];
        //if the horizontal Rb and a 'ghost' horizontal overlap, then convert
        //the 'ghost' join to a real join ready for later ...
        if (HorzSegmentsOverlap_(jr->OutPt1->Pt.X, jr->OffPt.X, rb->Bot.X, rb->Top.X)) {
          Clipper_AddJoin(s, jr->OutPt1, Op1, jr->OffPt);
        }
      }
    }
    if (lb->OutIdx >= 0 && lb->PrevInAEL &&
      lb->PrevInAEL->Curr.X == lb->Bot.X &&
      lb->PrevInAEL->OutIdx >= 0 &&
      SlopesEqual1(*lb->PrevInAEL, *lb, s->m_UseFullRange) &&
      (lb->WindDelta != 0) && (lb->PrevInAEL->WindDelta != 0)) {
      OutPt_t* Op2 = Clipper_AddOutPt(s, lb->PrevInAEL, lb->Bot);
      Clipper_AddJoin(s, Op1, Op2, lb->Top);
    }
    if (lb->NextInAEL != rb) {
      if (rb->OutIdx >= 0 && rb->PrevInAEL->OutIdx >= 0 &&
        SlopesEqual1(*rb->PrevInAEL, *rb, s->m_UseFullRange) &&
        (rb->WindDelta != 0) && (rb->PrevInAEL->WindDelta != 0)) {
        OutPt_t* Op2 = Clipper_AddOutPt(s, rb->PrevInAEL, rb->Bot);
        Clipper_AddJoin(s, Op1, Op2, rb->Top);
      }
      TEdge_t* e = lb->NextInAEL;
      if (e) {
        while (e != rb) {
          //nb: For calculating winding counts etc, Clipper_IntersectEdges() assumes
          //that param1 will be to the Right of param2 ABOVE the intersection ...
          Clipper_IntersectEdges(s, rb , e , lb->Curr); //order important here
          e = e->NextInAEL;
        }
      }
    }
  }
}
void Clipper_DeleteFromSEL(Clipper_t* s, TEdge_t* e)
{
  TEdge_t* SelPrev = e->PrevInSEL;
  TEdge_t* SelNext = e->NextInSEL;
  if (!SelPrev && !SelNext && (e != s->m_SortedEdges)) {
    return; //already deleted
  }
  if (SelPrev) {
    SelPrev->NextInSEL = SelNext;
  }
  else {
    s->m_SortedEdges = SelNext;
  }
  if (SelNext) {
    SelNext->PrevInSEL = SelPrev;
  }
  e->NextInSEL = 0;
  e->PrevInSEL = 0;
}
void GetHorzDirection(TEdge_t& HorzEdge, Direction& Dir, cInt& Left, cInt& Right)
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
TEdge_t* GetMaximaPair(TEdge_t* e)
{
  TEdge_t* result = 0;
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
TEdge_t* GetNextInAEL(TEdge_t* e, Direction dir)
{
  return dir == dLeftToRight ? e->NextInAEL : e->PrevInAEL;
}
void Clipper_DeleteFromAEL(Clipper_t* s, TEdge_t* e)
{
  TEdge_t* AelPrev = e->PrevInAEL;
  TEdge_t* AelNext = e->NextInAEL;
  if (!AelPrev && !AelNext && (e != s->m_ActiveEdges)) {
    return; //already deleted
  }
  if (AelPrev) {
    AelPrev->NextInAEL = AelNext;
  }
  else {
    s->m_ActiveEdges = AelNext;
  }
  if (AelNext) {
    AelNext->PrevInAEL = AelPrev;
  }
  e->NextInAEL = 0;
  e->PrevInAEL = 0;
}
void Clipper_SwapPositionsInAEL(Clipper_t* s, TEdge_t* Edge1, TEdge_t* Edge2)
{
  //check that one or other edge hasn't already been removed from AEL ...
  if (Edge1->NextInAEL == Edge1->PrevInAEL ||
    Edge2->NextInAEL == Edge2->PrevInAEL) {
    return;
  }
  if (Edge1->NextInAEL == Edge2) {
    TEdge_t* Next = Edge2->NextInAEL;
    if (Next) {
      Next->PrevInAEL = Edge1;
    }
    TEdge_t* Prev = Edge1->PrevInAEL;
    if (Prev) {
      Prev->NextInAEL = Edge2;
    }
    Edge2->PrevInAEL = Prev;
    Edge2->NextInAEL = Edge1;
    Edge1->PrevInAEL = Edge2;
    Edge1->NextInAEL = Next;
  }
  else if (Edge2->NextInAEL == Edge1) {
    TEdge_t* Next = Edge1->NextInAEL;
    if (Next) {
      Next->PrevInAEL = Edge2;
    }
    TEdge_t* Prev = Edge2->PrevInAEL;
    if (Prev) {
      Prev->NextInAEL = Edge1;
    }
    Edge1->PrevInAEL = Prev;
    Edge1->NextInAEL = Edge2;
    Edge2->PrevInAEL = Edge1;
    Edge2->NextInAEL = Next;
  }
  else {
    TEdge_t* Next = Edge1->NextInAEL;
    TEdge_t* Prev = Edge1->PrevInAEL;
    Edge1->NextInAEL = Edge2->NextInAEL;
    if (Edge1->NextInAEL) {
      Edge1->NextInAEL->PrevInAEL = Edge1;
    }
    Edge1->PrevInAEL = Edge2->PrevInAEL;
    if (Edge1->PrevInAEL) {
      Edge1->PrevInAEL->NextInAEL = Edge1;
    }
    Edge2->NextInAEL = Next;
    if (Edge2->NextInAEL) {
      Edge2->NextInAEL->PrevInAEL = Edge2;
    }
    Edge2->PrevInAEL = Prev;
    if (Edge2->PrevInAEL) {
      Edge2->PrevInAEL->NextInAEL = Edge2;
    }
  }
  if (!Edge1->PrevInAEL) {
    s->m_ActiveEdges = Edge1;
  }
  else if (!Edge2->PrevInAEL) {
    s->m_ActiveEdges = Edge2;
  }
}
void Clipper_UpdateEdgeIntoAEL(Clipper_t* s, TEdge_t*& e)
{
  if (!e->NextInLML) throw
    clipperException("UpdateEdgeIntoAEL: invalid call");
  e->NextInLML->OutIdx = e->OutIdx;
  TEdge_t* AelPrev = e->PrevInAEL;
  TEdge_t* AelNext = e->NextInAEL;
  if (AelPrev) {
    AelPrev->NextInAEL = e->NextInLML;
  }
  else {
    s->m_ActiveEdges = e->NextInLML;
  }
  if (AelNext) {
    AelNext->PrevInAEL = e->NextInLML;
  }
  e->NextInLML->Side = e->Side;
  e->NextInLML->WindDelta = e->WindDelta;
  e->NextInLML->WindCnt = e->WindCnt;
  e->NextInLML->WindCnt2 = e->WindCnt2;
  e = e->NextInLML;
  e->Curr = e->Bot;
  e->PrevInAEL = AelPrev;
  e->NextInAEL = AelNext;
  if (!IsHorizontal(*e)) {
    Clipper_InsertScanbeam(s, e->Top.Y);
  }
}

//------------------------------------------------------------------------
// ******************************************************************************
// * Notes: Horizontal edges (HEs) at scanline intersections (ie at the Top or *
// * Bottom of a scanbeam) are processed as if layered. The order in which HEs *
// * are processed doesn't matter. HEs intersect with other HE Bot.Xs only [#] *
// * (or they could intersect with Top.Xs only, ie EITHER Bot.Xs OR Top.Xs), *
// * and with other non-horizontal edges [*]. Once these intersections are *
// * processed, intermediate HEs then 'promote' the Edge above (NextInLML) into *
// * the AEL. These 'promoted' edges may in turn intersect [%] with other HEs. *
// ******************************************************************************
void Clipper_ProcessHorizontal(Clipper_t* s, TEdge_t* horzEdge, bool isTopOfScanbeam)
{
  Direction dir;
  cInt horzLeft, horzRight;
  GetHorzDirection(*horzEdge, dir, horzLeft, horzRight);
  TEdge_t* eLastHorz = horzEdge, *eMaxPair = 0;
  while (eLastHorz->NextInLML && IsHorizontal(*eLastHorz->NextInLML)) {
    eLastHorz = eLastHorz->NextInLML;
  }
  if (!eLastHorz->NextInLML) {
    eMaxPair = GetMaximaPair(eLastHorz);
  }
  for (;;) {
    bool IsLastHorz = (horzEdge == eLastHorz);
    TEdge_t* e = GetNextInAEL(horzEdge, dir);
    while (e) {
      //Break if we've got to the end of an intermediate horizontal edge ...
      //nb: Smaller Dx's are to the right of larger Dx's ABOVE the horizontal.
      if (e->Curr.X == horzEdge->Top.X && horzEdge->NextInLML &&
          e->Dx < horzEdge->NextInLML->Dx) {
        break;
      }
      TEdge_t* eNext = GetNextInAEL(e, dir); //saves eNext for later
      if ((dir == dLeftToRight && e->Curr.X <= horzRight) ||
          (dir == dRightToLeft && e->Curr.X >= horzLeft)) {
        //so far we're still in range of the horizontal Edge but make sure
        //we're at the last of consec. horizontals when matching with eMaxPair
        if (e == eMaxPair && IsLastHorz) {
          if (horzEdge->OutIdx >= 0) {
            OutPt_t* op1 = Clipper_AddOutPt(s, horzEdge, horzEdge->Top);
            TEdge_t* eNextHorz = s->m_SortedEdges;
            while (eNextHorz) {
              if (eNextHorz->OutIdx >= 0 &&
                  HorzSegmentsOverlap_(horzEdge->Bot.X,
                      horzEdge->Top.X, eNextHorz->Bot.X, eNextHorz->Top.X)) {
                OutPt_t* op2 = Clipper_AddOutPt(s, eNextHorz, eNextHorz->Bot);
                Clipper_AddJoin(s, op2, op1, eNextHorz->Top);
              }
              eNextHorz = eNextHorz->NextInSEL;
            }
            Clipper_AddGhostJoin(s, op1, horzEdge->Bot);
            Clipper_AddLocalMaxPoly(s, horzEdge, eMaxPair, horzEdge->Top);
          }
          Clipper_DeleteFromAEL(s, horzEdge);
          Clipper_DeleteFromAEL(s, eMaxPair);
          return;
        }
        else if (dir == dLeftToRight) {
          IntPoint Pt = IntPoint(e->Curr.X, horzEdge->Curr.Y);
          Clipper_IntersectEdges(s, horzEdge, e, Pt);
        }
        else {
          IntPoint Pt = IntPoint(e->Curr.X, horzEdge->Curr.Y);
          Clipper_IntersectEdges(s, e, horzEdge, Pt);
        }
        Clipper_SwapPositionsInAEL(s, horzEdge, e);
      }
      else if ((dir == dLeftToRight && e->Curr.X >= horzRight) ||
          (dir == dRightToLeft && e->Curr.X <= horzLeft)) {
        break;
      }
      e = eNext;
    } //end while
    if (horzEdge->NextInLML && IsHorizontal(*horzEdge->NextInLML)) {
      Clipper_UpdateEdgeIntoAEL(s, horzEdge);
      if (horzEdge->OutIdx >= 0) {
        Clipper_AddOutPt(s, horzEdge, horzEdge->Bot);
      }
      GetHorzDirection(*horzEdge, dir, horzLeft, horzRight);
    }
    else {
      break;
    }
  } //end for (;;)
  if (horzEdge->NextInLML) {
    if (horzEdge->OutIdx >= 0) {
      OutPt_t* op1 = Clipper_AddOutPt(s, horzEdge, horzEdge->Top);
      if (isTopOfScanbeam) {
        Clipper_AddGhostJoin(s, op1, horzEdge->Bot);
      }
      Clipper_UpdateEdgeIntoAEL(s, horzEdge);
      if (horzEdge->WindDelta == 0) {
        return;
      }
      //nb: HorzEdge is no longer horizontal here
      TEdge_t* ePrev = horzEdge->PrevInAEL;
      TEdge_t* eNext = horzEdge->NextInAEL;
      if (ePrev && ePrev->Curr.X == horzEdge->Bot.X &&
          ePrev->Curr.Y == horzEdge->Bot.Y && ePrev->WindDelta != 0 &&
          (ePrev->OutIdx >= 0 && ePrev->Curr.Y > ePrev->Top.Y &&
              SlopesEqual1(*horzEdge, *ePrev, s->m_UseFullRange))) {
        OutPt_t* op2 = Clipper_AddOutPt(s, ePrev, horzEdge->Bot);
        Clipper_AddJoin(s, op1, op2, horzEdge->Top);
      }
      else if (eNext && eNext->Curr.X == horzEdge->Bot.X &&
          eNext->Curr.Y == horzEdge->Bot.Y && eNext->WindDelta != 0 &&
          eNext->OutIdx >= 0 && eNext->Curr.Y > eNext->Top.Y &&
          SlopesEqual1(*horzEdge, *eNext, s->m_UseFullRange)) {
        OutPt_t* op2 = Clipper_AddOutPt(s, eNext, horzEdge->Bot);
        Clipper_AddJoin(s, op1, op2, horzEdge->Top);
      }
    }
    else {
      Clipper_UpdateEdgeIntoAEL(s, horzEdge);
    }
  }
  else {
    if (horzEdge->OutIdx >= 0) {
      Clipper_AddOutPt(s, horzEdge, horzEdge->Top);
    }
    Clipper_DeleteFromAEL(s, horzEdge);
  }
}

void Clipper_ProcessHorizontals(Clipper_t* s, bool IsTopOfScanbeam)
{
  TEdge_t* horzEdge = s->m_SortedEdges;
  while (horzEdge) {
    Clipper_DeleteFromSEL(s, horzEdge);
    Clipper_ProcessHorizontal(s, horzEdge, IsTopOfScanbeam);
    horzEdge = s->m_SortedEdges;
  }
}

void Clipper_DisposeIntersectNodes(Clipper_t* s)
{
  for (size_t i = 0; i < s->m_IntersectList.size(); ++i) {
    delete s->m_IntersectList[i];
  }
  s->m_IntersectList.clear();
}
void Clipper_SwapPositionsInSEL(Clipper_t* s, TEdge_t* Edge1, TEdge_t* Edge2)
{
  if (!(Edge1->NextInSEL) && !(Edge1->PrevInSEL)) {
    return;
  }
  if (!(Edge2->NextInSEL) && !(Edge2->PrevInSEL)) {
    return;
  }
  if (Edge1->NextInSEL == Edge2) {
    TEdge_t* Next = Edge2->NextInSEL;
    if (Next) {
      Next->PrevInSEL = Edge1;
    }
    TEdge_t* Prev = Edge1->PrevInSEL;
    if (Prev) {
      Prev->NextInSEL = Edge2;
    }
    Edge2->PrevInSEL = Prev;
    Edge2->NextInSEL = Edge1;
    Edge1->PrevInSEL = Edge2;
    Edge1->NextInSEL = Next;
  }
  else if (Edge2->NextInSEL == Edge1) {
    TEdge_t* Next = Edge1->NextInSEL;
    if (Next) {
      Next->PrevInSEL = Edge2;
    }
    TEdge_t* Prev = Edge2->PrevInSEL;
    if (Prev) {
      Prev->NextInSEL = Edge1;
    }
    Edge1->PrevInSEL = Prev;
    Edge1->NextInSEL = Edge2;
    Edge2->PrevInSEL = Edge1;
    Edge2->NextInSEL = Next;
  }
  else {
    TEdge_t* Next = Edge1->NextInSEL;
    TEdge_t* Prev = Edge1->PrevInSEL;
    Edge1->NextInSEL = Edge2->NextInSEL;
    if (Edge1->NextInSEL) {
      Edge1->NextInSEL->PrevInSEL = Edge1;
    }
    Edge1->PrevInSEL = Edge2->PrevInSEL;
    if (Edge1->PrevInSEL) {
      Edge1->PrevInSEL->NextInSEL = Edge1;
    }
    Edge2->NextInSEL = Next;
    if (Edge2->NextInSEL) {
      Edge2->NextInSEL->PrevInSEL = Edge2;
    }
    Edge2->PrevInSEL = Prev;
    if (Edge2->PrevInSEL) {
      Edge2->PrevInSEL->NextInSEL = Edge2;
    }
  }
  if (!Edge1->PrevInSEL) {
    s->m_SortedEdges = Edge1;
  }
  else if (!Edge2->PrevInSEL) {
    s->m_SortedEdges = Edge2;
  }
}

void Clipper_BuildIntersectList(Clipper_t* s, const cInt topY)
{
  if (!s->m_ActiveEdges) {
    return;
  }
  //prepare for sorting ...
  TEdge_t* e = s->m_ActiveEdges;
  s->m_SortedEdges = e;
  while (e) {
    e->PrevInSEL = e->PrevInAEL;
    e->NextInSEL = e->NextInAEL;
    e->Curr.X = TopX(*e, topY);
    e = e->NextInAEL;
  }
  //bubblesort ...
  bool isModified;
  do {
    isModified = false;
    e = s->m_SortedEdges;
    while (e->NextInSEL) {
      TEdge_t* eNext = e->NextInSEL;
      IntPoint Pt;
      if (e->Curr.X > eNext->Curr.X) {
        IntersectPoint(*e, *eNext, Pt);
        IntersectNode_t* newNode = new IntersectNode_t;
        newNode->Edge1 = e;
        newNode->Edge2 = eNext;
        newNode->Pt = Pt;
        s->m_IntersectList.push_back(newNode);
        Clipper_SwapPositionsInSEL(s, e, eNext);
        isModified = true;
      }
      else {
        e = eNext;
      }
    }
    if (e->PrevInSEL) {
      e->PrevInSEL->NextInSEL = 0;
    }
    else {
      break;
    }
  }
  while (isModified);
  s->m_SortedEdges = 0; //important
}
void Clipper_ProcessIntersectList(Clipper_t* s)
{
  for (int i = 0; i < s->m_IntersectList.size(); ++i) {
    IntersectNode_t* iNode = s->m_IntersectList[i];
    {
      Clipper_IntersectEdges(s, iNode->Edge1, iNode->Edge2, iNode->Pt);
      Clipper_SwapPositionsInAEL(s, iNode->Edge1 , iNode->Edge2);
    }
    delete iNode;
  }
  s->m_IntersectList.clear();
}
bool IntersectListSort_(IntersectNode_t* node1, IntersectNode_t* node2)
{
  return node2->Pt.Y < node1->Pt.Y;
}
inline bool EdgesAdjacent(const IntersectNode_t& inode)
{
  return (inode.Edge1->NextInSEL == inode.Edge2) ||
    (inode.Edge1->PrevInSEL == inode.Edge2);
}

bool Clipper_FixupIntersectionOrder(Clipper_t* s)
{
  //pre-condition: intersections are sorted Bottom-most first.
  //Now it's crucial that intersections are made only between adjacent edges,
  //so to ensure this the order of intersections may need adjusting ...
  Clipper_CopyAELToSEL(s);
  std::sort(s->m_IntersectList.begin(), s->m_IntersectList.end(), IntersectListSort_);
  size_t cnt = s->m_IntersectList.size();
  for (size_t i = 0; i < cnt; ++i) {
    if (!EdgesAdjacent(*s->m_IntersectList[i])) {
      size_t j = i + 1;
      while (j < cnt && !EdgesAdjacent(*s->m_IntersectList[j])) {
        j++;
      }
      if (j == cnt) {
        return false;
      }
      std::swap(s->m_IntersectList[i], s->m_IntersectList[j]);
    }
    Clipper_SwapPositionsInSEL(s, s->m_IntersectList[i]->Edge1, s->m_IntersectList[i]->Edge2);
  }
  return true;
}
bool Clipper_ProcessIntersections(Clipper_t* s, const cInt topY)
{
  if (!s->m_ActiveEdges) {
    return true;
  }
  try {
    Clipper_BuildIntersectList(s, topY);
    size_t IlSize = s->m_IntersectList.size();
    if (IlSize == 0) {
      return true;
    }
    if (IlSize == 1 || Clipper_FixupIntersectionOrder(s)) {
      Clipper_ProcessIntersectList(s);
    }
    else {
      return false;
    }
  }
  catch (...) {
    s->m_SortedEdges = 0;
    Clipper_DisposeIntersectNodes(s);
    throw clipperException("ProcessIntersections error");
  }
  s->m_SortedEdges = 0;
  return true;
}

inline bool IsMinima(TEdge_t* e)
{
  return e && (e->Prev->NextInLML != e) && (e->Next->NextInLML != e);
}
inline bool IsMaxima(TEdge_t* e, const cInt Y)
{
  return e && e->Top.Y == Y && !e->NextInLML;
}
inline bool IsIntermediate(TEdge_t* e, const cInt Y)
{
  return e->Top.Y == Y && e->NextInLML;
}

void Clipper_DoMaxima(Clipper_t* s, TEdge_t* e)
{
  TEdge_t* eMaxPair = GetMaximaPair(e);
  if (!eMaxPair) {
    if (e->OutIdx >= 0) {
      Clipper_AddOutPt(s, e, e->Top);
    }
    Clipper_DeleteFromAEL(s, e);
    return;
  }
  TEdge_t* eNext = e->NextInAEL;
  while (eNext && eNext != eMaxPair) {
    Clipper_IntersectEdges(s, e, eNext, e->Top);
    Clipper_SwapPositionsInAEL(s, e, eNext);
    eNext = e->NextInAEL;
  }
  if (e->OutIdx == Unassigned && eMaxPair->OutIdx == Unassigned) {
    Clipper_DeleteFromAEL(s, e);
    Clipper_DeleteFromAEL(s, eMaxPair);
  }
  else if (e->OutIdx >= 0 && eMaxPair->OutIdx >= 0) {
    if (e->OutIdx >= 0) {
      Clipper_AddLocalMaxPoly(s, e, eMaxPair, e->Top);
    }
    Clipper_DeleteFromAEL(s, e);
    Clipper_DeleteFromAEL(s, eMaxPair);
  }
#ifdef use_lines
  else if (e->WindDelta == 0) {
    if (e->OutIdx >= 0) {
      Clipper_AddOutPt(s, e, e->Top);
      e->OutIdx = Unassigned;
    }
    DeleteFromAEL(e);
    if (eMaxPair->OutIdx >= 0) {
      Clipper_AddOutPt(s, eMaxPair, e->Top);
      eMaxPair->OutIdx = Unassigned;
    }
    DeleteFromAEL(eMaxPair);
  }
#endif
  else {
    throw clipperException("DoMaxima error");
  }
}
void Clipper_ProcessEdgesAtTopOfScanbeam(Clipper_t *s, const cInt topY)
{
  TEdge_t* e = s->m_ActiveEdges;
  while (e) {
    //1. process maxima, treating them as if they're 'bent' horizontal edges,
    // but exclude maxima with horizontal edges. nb: e can't be a horizontal.
    bool IsMaximaEdge = IsMaxima(e, topY);
    if (IsMaximaEdge) {
      TEdge_t* eMaxPair = GetMaximaPair(e);
      IsMaximaEdge = (!eMaxPair || !IsHorizontal(*eMaxPair));
    }
    if (IsMaximaEdge) {
      TEdge_t* ePrev = e->PrevInAEL;
      Clipper_DoMaxima(s, e);
      if (!ePrev) {
        e = s->m_ActiveEdges;
      }
      else {
        e = ePrev->NextInAEL;
      }
    }
    else {
      //2. promote horizontal edges, otherwise update Curr.X and Curr.Y ...
      if (IsIntermediate(e, topY) && IsHorizontal(*e->NextInLML)) {
        Clipper_UpdateEdgeIntoAEL(s, e);
        if (e->OutIdx >= 0) {
          Clipper_AddOutPt(s, e, e->Bot);
        }
        Clipper_AddEdgeToSEL(s, e);
      }
      else {
        e->Curr.X = TopX(*e, topY);
        e->Curr.Y = topY;
      }
      if (s->m_StrictSimple) {
        TEdge_t* ePrev = e->PrevInAEL;
        if ((e->OutIdx >= 0) && (e->WindDelta != 0) && ePrev && (ePrev->OutIdx >= 0) &&
          (ePrev->Curr.X == e->Curr.X) && (ePrev->WindDelta != 0)) {
          IntPoint pt = e->Curr;
#ifdef use_xyz
          SetZ(pt, *ePrev, *e);
#endif
          OutPt_t* op = Clipper_AddOutPt(s, ePrev, pt);
          OutPt_t* op2 = Clipper_AddOutPt(s, e, pt);
          Clipper_AddJoin(s, op, op2, pt); //StrictlySimple (type-3) join
        }
      }
      e = e->NextInAEL;
    }
  }
  //3. Process horizontals at the Top of the scanbeam ...
  Clipper_ProcessHorizontals(s, true);
  //4. Promote intermediate vertices ...
  e = s->m_ActiveEdges;
  while (e) {
    if (IsIntermediate(e, topY)) {
      OutPt_t* op = 0;
      if (e->OutIdx >= 0) {
        op = Clipper_AddOutPt(s, e, e->Top);
      }
      Clipper_UpdateEdgeIntoAEL(s, e);
      //if output polygons share an edge, they'll need joining later ...
      TEdge_t* ePrev = e->PrevInAEL;
      TEdge_t* eNext = e->NextInAEL;
      if (ePrev && ePrev->Curr.X == e->Bot.X &&
        ePrev->Curr.Y == e->Bot.Y && op &&
        ePrev->OutIdx >= 0 && ePrev->Curr.Y > ePrev->Top.Y &&
        SlopesEqual1(*e, *ePrev, s->m_UseFullRange) &&
        (e->WindDelta != 0) && (ePrev->WindDelta != 0)) {
        OutPt_t* op2 = Clipper_AddOutPt(s, ePrev, e->Bot);
        Clipper_AddJoin(s, op, op2, e->Top);
      }
      else if (eNext && eNext->Curr.X == e->Bot.X &&
        eNext->Curr.Y == e->Bot.Y && op &&
        eNext->OutIdx >= 0 && eNext->Curr.Y > eNext->Top.Y &&
        SlopesEqual1(*e, *eNext, s->m_UseFullRange) &&
        (e->WindDelta != 0) && (eNext->WindDelta != 0)) {
        OutPt_t* op2 = Clipper_AddOutPt(s, eNext, e->Bot);
        Clipper_AddJoin(s, op, op2, e->Top);
      }
    }
    e = e->NextInAEL;
  }
}


OutRec_t* Clipper_GetOutRec(Clipper_t* s, int Idx)
{
  OutRec_t* outrec = s->m_PolyOuts[Idx];
  while (outrec != s->m_PolyOuts[outrec->Idx]) {
    outrec = s->m_PolyOuts[outrec->Idx];
  }
  return outrec;
}

void SwapIntersectNodes(IntersectNode_t& int1, IntersectNode_t& int2)
{
  //just swap the contents (because fIntersectNodes is a single-linked-list)
  IntersectNode_t inode = int1; //gets a copy of Int1
  int1.Edge1 = int2.Edge1;
  int1.Edge2 = int2.Edge2;
  int1.Pt = int2.Pt;
  int2.Edge1 = inode.Edge1;
  int2.Edge2 = inode.Edge2;
  int2.Pt = inode.Pt;
}
bool GetOverlap_(const cInt a1, const cInt a2, const cInt b1, const cInt b2,
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
inline void UpdateOutPtIdxs(OutRec_t& outrec)
{
  OutPt_t* op = outrec.Pts;
  do {
    op->Idx = outrec.Idx;
    op = op->Prev;
  }
  while (op != outrec.Pts);
}
//----------------------------------------------------------------------
OutPt_t* DupOutPt(OutPt_t* outPt, bool InsertAfter)
{
  OutPt_t* result = new OutPt_t;
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
bool JoinHorz(OutPt_t* op1, OutPt_t* op1b, OutPt_t* op2, OutPt_t* op2b,
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
//----------------------------------------------------------------------
static OutRec_t* ParseFirstLeft(OutRec_t* FirstLeft)
{
  while (FirstLeft && !FirstLeft->Pts) {
    FirstLeft = FirstLeft->FirstLeft;
  }
  return FirstLeft;
}

bool Clipper_JoinPoints(Clipper_t* s, Join_t* j, OutRec_t* outRec1, OutRec_t* outRec2)
{
  OutPt_t* op1 = j->OutPt1, *op1b;
  OutPt_t* op2 = j->OutPt2, *op2b;
  //There are 3 kinds of joins for output polygons ...
  //1. Horizontal joins where Join_t.OutPt1 & Join_t.OutPt2 are a vertices anywhere
  //along (horizontal) collinear edges (& Join_t.OffPt is on the same horizontal).
  //2. Non-horizontal joins where Join_t.OutPt1 & Join_t.OutPt2 are at the same
  //location at the Bottom of the overlapping segment (& Join_t.OffPt is above).
  //3. StrictSimple joins where edges touch but are not collinear and where
  //Join_t.OutPt1, Join_t.OutPt2 & Join_t.OffPt all share the same point.
  bool isHorizontal = (j->OutPt1->Pt.Y == j->OffPt.Y);
  if (isHorizontal && (j->OffPt == j->OutPt1->Pt) &&
      (j->OffPt == j->OutPt2->Pt)) {
    //Strictly Simple join ...
    if (outRec1 != outRec2) {
      return false;
    }
    op1b = j->OutPt1->Next;
    while (op1b != op1 && (op1b->Pt == j->OffPt)) {
      op1b = op1b->Next;
    }
    bool reverse1 = (op1b->Pt.Y > j->OffPt.Y);
    op2b = j->OutPt2->Next;
    while (op2b != op2 && (op2b->Pt == j->OffPt)) {
      op2b = op2b->Next;
    }
    bool reverse2 = (op2b->Pt.Y > j->OffPt.Y);
    if (reverse1 == reverse2) {
      return false;
    }
    if (reverse1) {
      op1b = DupOutPt(op1, false);
      op2b = DupOutPt(op2, true);
      op1->Prev = op2;
      op2->Next = op1;
      op1b->Next = op2b;
      op2b->Prev = op1b;
      j->OutPt1 = op1;
      j->OutPt2 = op1b;
      return true;
    }
    else {
      op1b = DupOutPt(op1, true);
      op2b = DupOutPt(op2, false);
      op1->Next = op2;
      op2->Prev = op1;
      op1b->Prev = op2b;
      op2b->Next = op1b;
      j->OutPt1 = op1;
      j->OutPt2 = op1b;
      return true;
    }
  }
  else if (isHorizontal) {
    //treat horizontal joins differently to non-horizontal joins since with
    //them we're not yet sure where the overlapping is. OutPt1.Pt & OutPt2.Pt
    //may be anywhere along the horizontal edge.
    op1b = op1;
    while (op1->Prev->Pt.Y == op1->Pt.Y && op1->Prev != op1b && op1->Prev != op2) {
      op1 = op1->Prev;
    }
    while (op1b->Next->Pt.Y == op1b->Pt.Y && op1b->Next != op1 && op1b->Next != op2) {
      op1b = op1b->Next;
    }
    if (op1b->Next == op1 || op1b->Next == op2) {
      return false; //a flat 'polygon'
    }
    op2b = op2;
    while (op2->Prev->Pt.Y == op2->Pt.Y && op2->Prev != op2b && op2->Prev != op1b) {
      op2 = op2->Prev;
    }
    while (op2b->Next->Pt.Y == op2b->Pt.Y && op2b->Next != op2 && op2b->Next != op1) {
      op2b = op2b->Next;
    }
    if (op2b->Next == op2 || op2b->Next == op1) {
      return false; //a flat 'polygon'
    }
    cInt Left, Right;
    //Op1 --> Op1b & Op2 --> Op2b are the extremites of the horizontal edges
    if (!GetOverlap_(op1->Pt.X, op1b->Pt.X, op2->Pt.X, op2b->Pt.X, Left, Right)) {
      return false;
    }
    //DiscardLeftSide: when overlapping edges are joined, a spike will created
    //which needs to be cleaned up. However, we don't want Op1 or Op2 caught up
    //on the discard Side as either may still be needed for other joins ...
    IntPoint Pt;
    bool DiscardLeftSide;
    if (op1->Pt.X >= Left && op1->Pt.X <= Right) {
      Pt = op1->Pt;
      DiscardLeftSide = (op1->Pt.X > op1b->Pt.X);
    }
    else if (op2->Pt.X >= Left && op2->Pt.X <= Right) {
      Pt = op2->Pt;
      DiscardLeftSide = (op2->Pt.X > op2b->Pt.X);
    }
    else if (op1b->Pt.X >= Left && op1b->Pt.X <= Right) {
      Pt = op1b->Pt;
      DiscardLeftSide = op1b->Pt.X > op1->Pt.X;
    }
    else {
      Pt = op2b->Pt;
      DiscardLeftSide = (op2b->Pt.X > op2->Pt.X);
    }
    j->OutPt1 = op1;
    j->OutPt2 = op2;
    return JoinHorz(op1, op1b, op2, op2b, Pt, DiscardLeftSide);
  }
  else {
    //nb: For non-horizontal joins ...
    // 1. Jr.OutPt1.Pt.Y == Jr.OutPt2.Pt.Y
    // 2. Jr.OutPt1.Pt > Jr.OffPt.Y
    //make sure the polygons are correctly oriented ...
    op1b = op1->Next;
    while ((op1b->Pt == op1->Pt) && (op1b != op1)) {
      op1b = op1b->Next;
    }
    bool Reverse1 = ((op1b->Pt.Y > op1->Pt.Y) ||
        !SlopesEqual1(op1->Pt, op1b->Pt, j->OffPt, s->m_UseFullRange));
    if (Reverse1) {
      op1b = op1->Prev;
      while ((op1b->Pt == op1->Pt) && (op1b != op1)) {
        op1b = op1b->Prev;
      }
      if ((op1b->Pt.Y > op1->Pt.Y) ||
          !SlopesEqual1(op1->Pt, op1b->Pt, j->OffPt, s->m_UseFullRange)) {
        return false;
      }
    };
    op2b = op2->Next;
    while ((op2b->Pt == op2->Pt) && (op2b != op2)) {
      op2b = op2b->Next;
    }
    bool Reverse2 = ((op2b->Pt.Y > op2->Pt.Y) ||
        !SlopesEqual1(op2->Pt, op2b->Pt, j->OffPt, s->m_UseFullRange));
    if (Reverse2) {
      op2b = op2->Prev;
      while ((op2b->Pt == op2->Pt) && (op2b != op2)) {
        op2b = op2b->Prev;
      }
      if ((op2b->Pt.Y > op2->Pt.Y) ||
          !SlopesEqual1(op2->Pt, op2b->Pt, j->OffPt, s->m_UseFullRange)) {
        return false;
      }
    }
    if ((op1b == op1) || (op2b == op2) || (op1b == op2b) ||
        ((outRec1 == outRec2) && (Reverse1 == Reverse2))) {
      return false;
    }
    if (Reverse1) {
      op1b = DupOutPt(op1, false);
      op2b = DupOutPt(op2, true);
      op1->Prev = op2;
      op2->Next = op1;
      op1b->Next = op2b;
      op2b->Prev = op1b;
      j->OutPt1 = op1;
      j->OutPt2 = op1b;
      return true;
    }
    else {
      op1b = DupOutPt(op1, true);
      op2b = DupOutPt(op2, false);
      op1->Next = op2;
      op2->Prev = op1;
      op1b->Prev = op2b;
      op2b->Next = op1b;
      j->OutPt1 = op1;
      j->OutPt2 = op1b;
      return true;
    }
  }
}
//----------------------------------------------------------------------
void Clipper_FixupFirstLefts2(Clipper_t* s, OutRec_t* OldOutRec, OutRec_t* NewOutRec)
{
  //reassigns FirstLeft WITHOUT testing if NewOutRec contains the polygon
  for (int i = 0; i < s->m_PolyOuts.size(); ++i) {
    OutRec_t* outRec = s->m_PolyOuts[i];
    if (outRec->FirstLeft == OldOutRec) {
      outRec->FirstLeft = NewOutRec;
    }
  }
}
void Clipper_FixupFirstLefts1(Clipper_t* s, OutRec_t* OldOutRec, OutRec_t* NewOutRec)
{
  //tests if NewOutRec contains the polygon before reassigning FirstLeft
  for (int i = 0; i < s->m_PolyOuts.size(); ++i) {
    OutRec_t* outRec = s->m_PolyOuts[i];
    if (!outRec->Pts || !outRec->FirstLeft) {
      continue;
    }
    OutRec_t* firstLeft = ParseFirstLeft(outRec->FirstLeft);
    if (firstLeft == OldOutRec) {
      if (Poly2ContainsPoly1(outRec->Pts, NewOutRec->Pts)) {
        outRec->FirstLeft = NewOutRec;
      }
    }
  }
}
//----------------------------------------------------------------------
void Clipper_JoinCommonEdges(Clipper_t* s)
{
  for (int i = 0; i < s->m_Joins.size(); i++) {
    Join_t* join = s->m_Joins[i];
    OutRec_t* outRec1 = Clipper_GetOutRec(s, join->OutPt1->Idx);
    OutRec_t* outRec2 = Clipper_GetOutRec(s, join->OutPt2->Idx);
    if (!outRec1->Pts || !outRec2->Pts) {
      continue;
    }
    //get the polygon fragment with the correct hole state (FirstLeft)
    //before calling JoinPoints() ...
    OutRec_t* holeStateRec;
    if (outRec1 == outRec2) {
      holeStateRec = outRec1;
    }
    else if (Param1RightOfParam2(outRec1, outRec2)) {
      holeStateRec = outRec2;
    }
    else if (Param1RightOfParam2(outRec2, outRec1)) {
      holeStateRec = outRec1;
    }
    else {
      holeStateRec = GetLowermostRec(outRec1, outRec2);
    }
    if (!Clipper_JoinPoints(s, join, outRec1, outRec2)) {
      continue;
    }
    if (outRec1 == outRec2) {
      //instead of joining two polygons, we've just created a new one by
      //splitting one polygon into two.
      outRec1->Pts = join->OutPt1;
      outRec1->BottomPt = 0;
      outRec2 = Clipper_CreateOutRec(s);
      outRec2->Pts = join->OutPt2;
      //update all OutRec2.Pts Idx's ...
      UpdateOutPtIdxs(*outRec2);
      //We now need to check every OutRec_t.FirstLeft pointer. If it points
      //to OutRec1 it may need to point to OutRec2 instead ...
      if (s->m_UsingPolyTree)
        for (int j = 0; j < s->m_PolyOuts.size() - 1; j++) {
          OutRec_t* oRec = s->m_PolyOuts[j];
          if (!oRec->Pts || ParseFirstLeft(oRec->FirstLeft) != outRec1 ||
              oRec->IsHole == outRec1->IsHole) {
            continue;
          }
          if (Poly2ContainsPoly1(oRec->Pts, join->OutPt2)) {
            oRec->FirstLeft = outRec2;
          }
        }
      if (Poly2ContainsPoly1(outRec2->Pts, outRec1->Pts)) {
        //outRec2 is contained by outRec1 ...
        outRec2->IsHole = !outRec1->IsHole;
        outRec2->FirstLeft = outRec1;
        //fixup FirstLeft pointers that may need reassigning to OutRec1
        if (s->m_UsingPolyTree) {
          Clipper_FixupFirstLefts2(s, outRec2, outRec1);
        }
        if ((outRec2->IsHole ^ s->m_ReverseOutput) == (Area(*outRec2) > 0)) {
          ReversePolyPtLinks(outRec2->Pts);
        }
      }
      else if (Poly2ContainsPoly1(outRec1->Pts, outRec2->Pts)) {
        //outRec1 is contained by outRec2 ...
        outRec2->IsHole = outRec1->IsHole;
        outRec1->IsHole = !outRec2->IsHole;
        outRec2->FirstLeft = outRec1->FirstLeft;
        outRec1->FirstLeft = outRec2;
        //fixup FirstLeft pointers that may need reassigning to OutRec1
        if (s->m_UsingPolyTree) {
          Clipper_FixupFirstLefts2(s, outRec1, outRec2);
        }
        if ((outRec1->IsHole ^ s->m_ReverseOutput) == (Area(*outRec1) > 0)) {
          ReversePolyPtLinks(outRec1->Pts);
        }
      }
      else {
        //the 2 polygons are completely separate ...
        outRec2->IsHole = outRec1->IsHole;
        outRec2->FirstLeft = outRec1->FirstLeft;
        //fixup FirstLeft pointers that may need reassigning to OutRec2
        if (s->m_UsingPolyTree) {
          Clipper_FixupFirstLefts1(s, outRec1, outRec2);
        }
      }
    }
    else {
      //joined 2 polygons together ...
      outRec2->Pts = 0;
      outRec2->BottomPt = 0;
      outRec2->Idx = outRec1->Idx;
      outRec1->IsHole = holeStateRec->IsHole;
      if (holeStateRec == outRec2) {
        outRec1->FirstLeft = outRec2->FirstLeft;
      }
      outRec2->FirstLeft = outRec1;
      //fixup FirstLeft pointers that may need reassigning to OutRec1
      if (s->m_UsingPolyTree) {
        Clipper_FixupFirstLefts2(s, outRec2, outRec1);
      }
    }
  }
}
void Clipper_FixupOutPolygon(Clipper_t *s, OutRec_t& outrec)
{
  //FixupOutPolygon() - removes duplicate points and simplifies consecutive
  //parallel edges by removing the middle vertex.
  OutPt_t* lastOK = 0;
  outrec.BottomPt = 0;
  OutPt_t* pp = outrec.Pts;
  for (;;) {
    if (pp->Prev == pp || pp->Prev == pp->Next) {
      DisposeOutPts(pp);
      outrec.Pts = 0;
      return;
    }
    //test for duplicate points and collinear edges ...
    if ((pp->Pt == pp->Next->Pt) || (pp->Pt == pp->Prev->Pt) ||
      (SlopesEqual1(pp->Prev->Pt, pp->Pt, pp->Next->Pt, s->m_UseFullRange) &&
      (!s->m_PreserveCollinear ||
      !Pt2IsBetweenPt1AndPt3_(pp->Prev->Pt, pp->Pt, pp->Next->Pt)))) {
      lastOK = 0;
      OutPt_t* tmp = pp;
      pp->Prev->Next = pp->Next;
      pp->Next->Prev = pp->Prev;
      pp = pp->Prev;
      delete tmp;
    }
    else if (pp == lastOK) {
      break;
    }
    else {
      if (!lastOK) {
        lastOK = pp;
      }
      pp = pp->Next;
    }
  }
  outrec.Pts = pp;
}
// Miscellaneous public functions
void Clipper_DoSimplePolygons(Clipper_t* s)
{
  int i = 0;
  while (i < s->m_PolyOuts.size()) {
    OutRec_t* outrec = s->m_PolyOuts[i++];
    OutPt_t* op = outrec->Pts;
    if (!op || outrec->IsOpen) {
      continue;
    }
    do { //for each Pt in Polygon until duplicate found do ...
      OutPt_t* op2 = op->Next;
      while (op2 != outrec->Pts) {
        if ((op->Pt == op2->Pt) && op2->Next != op && op2->Prev != op) {
          //split the polygon into two ...
          OutPt_t* op3 = op->Prev;
          OutPt_t* op4 = op2->Prev;
          op->Prev = op4;
          op4->Next = op;
          op2->Prev = op3;
          op3->Next = op2;
          outrec->Pts = op;
          OutRec_t* outrec2 = Clipper_CreateOutRec(s);
          outrec2->Pts = op2;
          UpdateOutPtIdxs(*outrec2);
          if (Poly2ContainsPoly1(outrec2->Pts, outrec->Pts)) {
            //OutRec2 is contained by OutRec1 ...
            outrec2->IsHole = !outrec->IsHole;
            outrec2->FirstLeft = outrec;
            if (s->m_UsingPolyTree) {
              Clipper_FixupFirstLefts2(s, outrec2, outrec);
            }
          }
          else if (Poly2ContainsPoly1(outrec->Pts, outrec2->Pts)) {
            //OutRec1 is contained by OutRec2 ...
            outrec2->IsHole = outrec->IsHole;
            outrec->IsHole = !outrec2->IsHole;
            outrec2->FirstLeft = outrec->FirstLeft;
            outrec->FirstLeft = outrec2;
            if (s->m_UsingPolyTree) {
              Clipper_FixupFirstLefts2(s, outrec, outrec2);
            }
          }
          else {
            //the 2 polygons are separate ...
            outrec2->IsHole = outrec->IsHole;
            outrec2->FirstLeft = outrec->FirstLeft;
            if (s->m_UsingPolyTree) {
              Clipper_FixupFirstLefts1(s, outrec, outrec2);
            }
          }
          op2 = op; //ie get ready for the Next iteration
        }
        op2 = op2->Next;
      }
      op = op->Next;
    }
    while (op != outrec->Pts);
  }
}
bool Clipper_ExecuteInternal(Clipper_t* s)
{
  bool succeeded = true;
  try {
    Clipper_Reset(s);
    if (s->m_CurrentLM == s->m_MinimaList.end()) {
      return true;
    }
    cInt botY = Clipper_PopScanbeam(s);
    do {
      Clipper_InsertLocalMinimaIntoAEL(s, botY);
      Clipper_ClearGhostJoins(s);
      Clipper_ProcessHorizontals(s, false);
      if (s->m_Scanbeam.empty()) {
        break;
      }
      cInt topY = Clipper_PopScanbeam(s);
      succeeded = Clipper_ProcessIntersections(s, topY);
      if (!succeeded) {
        break;
      }
      Clipper_ProcessEdgesAtTopOfScanbeam(s, topY);
      botY = topY;
    }
    while (!s->m_Scanbeam.empty() || s->m_CurrentLM != s->m_MinimaList.end());
  }
  catch (...) {
    succeeded = false;
  }
  if (succeeded) {
    size_t i;
    //fix orientations ...
    for (i = 0; i < s->m_PolyOuts.size(); ++i) {
      OutRec_t* outRec = s->m_PolyOuts[i];
      if (!outRec->Pts || outRec->IsOpen) {
        continue;
      }
      if ((outRec->IsHole ^ s->m_ReverseOutput) == (Area(*outRec) > 0)) {
        ReversePolyPtLinks(outRec->Pts);
      }
    }
    if (!s->m_Joins.empty()) {
      Clipper_JoinCommonEdges(s);
    }
    //unfortunately FixupOutPolygon() must be done after JoinCommonEdges()
    for (i = 0; i < s->m_PolyOuts.size(); ++i) {
      OutRec_t* outRec = s->m_PolyOuts[i];
      if (outRec->Pts && !outRec->IsOpen) {
        Clipper_FixupOutPolygon(s, *outRec);
      }
    }
    if (s->m_StrictSimple) {
      Clipper_DoSimplePolygons(s);
    }
  }
  Clipper_ClearJoins(s);
  Clipper_ClearGhostJoins(s);
  return succeeded;
}
int PointCount(OutPt_t* Pts)
{
  if (!Pts) {
    return 0;
  }
  int result = 0;
  OutPt_t* p = Pts;
  do {
    result++;
    p = p->Next;
  }
  while (p != Pts);
  return result;
}
void Clipper_BuildResult(Clipper_t* s, Paths& polys)
{
  polys.reserve(s->m_PolyOuts.size());
  for (int i = 0; i < s->m_PolyOuts.size(); ++i) {
    if (!s->m_PolyOuts[i]->Pts) {
      continue;
    }
    IPath pg;
    OutPt_t* p = s->m_PolyOuts[i]->Pts->Prev;
    int cnt = PointCount(p);
    if (cnt < 2) {
      continue;
    }
    pg.reserve(cnt);
    for (int i = 0; i < cnt; ++i) {
      pg.push_back(p->Pt);
      p = p->Prev;
    }
    polys.push_back(pg);
  }
}
void Clipper_DisposeOutRec(Clipper_t* s, int index)
{
  OutRec_t* outRec = s->m_PolyOuts[index];
  if (outRec->Pts) {
    DisposeOutPts(outRec->Pts);
  }
  delete outRec;
  s->m_PolyOuts[index] = 0;
}

void Clipper_DisposeAllOutRecs(Clipper_t* s)
{
  for (int i = 0; i < s->m_PolyOuts.size(); ++i) {
    Clipper_DisposeOutRec(s, i);
  }
  s->m_PolyOuts.clear();
}
bool Clipper_Execute(Clipper_t* s, ClipType clipType, Paths& solution,
    PolyFillType subjFillType = pftEvenOdd, PolyFillType clipFillType = pftEvenOdd)
{
  if (s->m_ExecuteLocked) {
    return false;
  }
  if (s->m_HasOpenPaths) {
    throw clipperException("Error: PolyTree_t struct is need for open path clipping.");
  }
  s->m_ExecuteLocked = true;
  solution.resize(0);
  s->m_SubjFillType = subjFillType;
  s->m_ClipFillType = clipFillType;
  s->m_ClipType = clipType;
  s->m_UsingPolyTree = false;
  bool succeeded = Clipper_ExecuteInternal(s);
  if (succeeded) {
    Clipper_BuildResult(s, solution);
  }
  Clipper_DisposeAllOutRecs(s);
  s->m_ExecuteLocked = false;
  return succeeded;
}
void Clipper_FixHoleLinkage(Clipper_t* s, OutRec_t& outrec)
{
  //skip OutRecs that (a) contain outermost polygons or
  //(b) already have the correct owner/child linkage ...
  if (!outrec.FirstLeft ||
    (outrec.IsHole != outrec.FirstLeft->IsHole &&
    outrec.FirstLeft->Pts)) {
    return;
  }
  OutRec_t* orfl = outrec.FirstLeft;
  while (orfl && ((orfl->IsHole == outrec.IsHole) || !orfl->Pts)) {
    orfl = orfl->FirstLeft;
  }
  outrec.FirstLeft = orfl;
}

void Clipper_BuildResult2(Clipper_t *s, PolyTree_t* polytree)
{
  size_t i, n = s->m_PolyOuts.size();
  PolyTree_Clear(polytree);
  MYREALLOC(polytree->AllNodes, n);
  polytree->AllNodes_size = n;
  //add each output polygon/contour to polytree ...
  for (i = 0; i < n; i++) {
    OutRec_t* outRec = s->m_PolyOuts[i];
    int cnt = PointCount(outRec->Pts);
    if ((outRec->IsOpen && cnt < 2) || (!outRec->IsOpen && cnt < 3)) {
      continue;
    }
    Clipper_FixHoleLinkage(s, *outRec);
    PolyNode_t* pn = newPolyNode_t();
    //nb: polytree takes ownership of all the PolyNodes_t
    polytree->AllNodes[i] = (pn);
    outRec->PolyNd = pn;
    pn->Parent = 0;
    MYREALLOC(pn->Contour, cnt);
    OutPt_t* op = outRec->Pts->Prev;
    for (int j = 0; j < cnt; j++) {
      pn->Contour[pn->Contour_size++] = (op->Pt);
      op = op->Prev;
    }
  }
  //fixup PolyNode_t links etc ...
  MYREALLOC(polytree->Childs, n);
  polytree->Childs_size = (n);
  for (i = 0; i < s->m_PolyOuts.size(); i++) {
    OutRec_t* outRec = s->m_PolyOuts[i];
    if (!outRec->PolyNd) {
      continue;
    }
    if (outRec->IsOpen) {
      outRec->PolyNd->m_IsOpen = true;
      PolyNode_AddChild(polytree, outRec->PolyNd);
    }
    else if (outRec->FirstLeft && outRec->FirstLeft->PolyNd) {
      PolyNode_AddChild(outRec->FirstLeft->PolyNd, outRec->PolyNd);
    }
    else {
      PolyNode_AddChild(polytree, outRec->PolyNd);
    }
  }
}

bool Clipper_Execute(Clipper_t* s, ClipType clipType, PolyTree_t* polytree,
    PolyFillType subjFillType = pftEvenOdd, PolyFillType clipFillType = pftEvenOdd)
{
  if (s->m_ExecuteLocked) {
    return false;
  }
  s->m_ExecuteLocked = true;
  s->m_SubjFillType = subjFillType;
  s->m_ClipFillType = clipFillType;
  s->m_ClipType = clipType;
  s->m_UsingPolyTree = true;
  bool succeeded = Clipper_ExecuteInternal(s);
  if (succeeded) {
    Clipper_BuildResult2(s, polytree);
  }
  Clipper_DisposeAllOutRecs(s);
  s->m_ExecuteLocked = false;
  return succeeded;
}

// ClipperOffset_t support functions ...
DoublePoint GetUnitNormal_(const IntPoint& pt1, const IntPoint& pt2)
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
// ClipperOffset_t struct
ClipperOffset_t::ClipperOffset_t(double miterLimit, double arcTolerance)
{
  m_polyNodes = NULL;
  this->MiterLimit = miterLimit;
  this->ArcTolerance = arcTolerance;
  m_lowest.X = -1;
}
ClipperOffset_t::~ClipperOffset_t()
{
  Clear();
}
void ClipperOffset_t::Clear()
{
  freePolyNode_t(m_polyNodes);
  m_polyNodes = NULL;
  m_lowest.X = -1;
}
void ClipperOffset_AddPath(ClipperOffset_t* s, const IPath& path, JoinType joinType, EndType endType)
{
  int highI = (int)path.size() - 1;
  if (highI < 0) {
    return;
  }
  s->m_polyNodes = newPolyNode_t();
  PolyNode_t* newNode = newPolyNode_t();
  newNode->m_jointype = joinType;
  newNode->m_endtype = endType;
  //strip duplicate points from path and also get index to the lowest point ...
  if (endType == etClosedLine || endType == etClosedPolygon)
    while (highI > 0 && path[0] == path[highI]) {
      highI--;
    }
  MYREALLOC(newNode->Contour, highI + 1);
  newNode->Contour_size = highI + 1;
  newNode->Contour[0] = (path[0]);
  int j = 0, k = 0;
  for (int i = 1; i <= highI; i++) {
    if (newNode->Contour[j] != path[i]) {
      j++;
      newNode->Contour[j] = (path[i]);
      if (path[i].Y > newNode->Contour[k].Y ||
          (path[i].Y == newNode->Contour[k].Y &&
              path[i].X < newNode->Contour[k].X)) {
        k = j;
      }
    }
  }
  newNode->Contour_size = j+1;
  if (endType == etClosedPolygon && j < 2) {
    delete newNode;
    return;
  }
  PolyNode_AddChild(s->m_polyNodes, newNode);
  //if this path's lowest pt is lower than all the others then update m_lowest
  if (endType != etClosedPolygon) {
    return;
  }
  if (s->m_lowest.X < 0) {
    s->m_lowest = IntPoint(s->m_polyNodes->Childs_size - 1, k);
  }
  else {
    IntPoint ip = s->m_polyNodes->Childs[(int)s->m_lowest.X]->Contour[(int)s->m_lowest.Y];
    if (newNode->Contour[k].Y > ip.Y ||
        (newNode->Contour[k].Y == ip.Y &&
            newNode->Contour[k].X < ip.X)) {
      s->m_lowest = IntPoint(s->m_polyNodes->Childs_size - 1, k);
    }
  }
}
void ClipperOffset_AddPaths(ClipperOffset_t* s, const Paths& paths, JoinType joinType, EndType endType)
{
  for (Paths::size_type i = 0; i < paths.size(); ++i) {
    ClipperOffset_AddPath(s, paths[i], joinType, endType);
  }
}
void ClipperOffset_t::FixOrientations()
{
  //fixup orientations of all closed paths if the orientation of the
  //closed path with the lowermost vertex is wrong ...
  if (m_lowest.X >= 0 &&
      !Orientation(m_polyNodes->Childs[(int)m_lowest.X]->Contour, m_polyNodes->Childs[(int)m_lowest.X]->Contour_size)) {
    for (int i = 0; i < m_polyNodes->Childs_size; ++i) {
      PolyNode_t& node = *m_polyNodes->Childs[i];
      if (node.m_endtype == etClosedPolygon ||
          (node.m_endtype == etClosedLine && Orientation(node.Contour, node.Contour_size))) {
        ReversePath_(node.Contour, node.Contour_size);
      }
    }
  }
  else {
    for (int i = 0; i < m_polyNodes->Childs_size; ++i) {
      PolyNode_t& node = *m_polyNodes->Childs[i];
      if (node.m_endtype == etClosedLine && !Orientation(node.Contour, node.Contour_size)) {
        ReversePath_(node.Contour, node.Contour_size);
      }
    }
  }
}
void ClipperOffset_Execute(ClipperOffset_t* s, Paths& solution, double delta)
{
  solution.clear();
  s->FixOrientations();
  s->DoOffset(delta);
  //now clean up 'corners' ...
  Clipper_t clpr;
  Clipper_Init(&clpr, 0);
  Clipper_AddPaths(&clpr, s->m_destPolys, ptSubject, true);
  if (delta > 0) {
    Clipper_Execute(&clpr, ctUnion, solution, pftPositive, pftPositive);
  }
  else {
    IntRect r = Clipper_GetBounds(&clpr);
    IPath outer(4);
    outer[0] = IntPoint(r.left - 10, r.bottom + 10);
    outer[1] = IntPoint(r.right + 10, r.bottom + 10);
    outer[2] = IntPoint(r.right + 10, r.top - 10);
    outer[3] = IntPoint(r.left - 10, r.top - 10);
    Clipper_AddPath(&clpr, outer, ptSubject, true);
    clpr.m_ReverseOutput = (true);
    Clipper_Execute(&clpr, ctUnion, solution, pftNegative, pftNegative);
    if (solution.size() > 0) {
      solution.erase(solution.begin());
    }
  }
  Clipper_Clear(&clpr);
}
void ClipperOffset_Execute(ClipperOffset_t* s, PolyTree_t* solution, double delta)
{
  PolyTree_Clear(solution);
  s->FixOrientations();
  s->DoOffset(delta);
  //now clean up 'corners' ...
  Clipper_t clpr;
  Clipper_AddPaths(&clpr, s->m_destPolys, ptSubject, true);
  if (delta > 0) {
    Clipper_Execute(&clpr, ctUnion, solution, pftPositive, pftPositive);
  }
  else {
    IntRect r = Clipper_GetBounds(&clpr);
    IPath outer(4);
    outer[0] = IntPoint(r.left - 10, r.bottom + 10);
    outer[1] = IntPoint(r.right + 10, r.bottom + 10);
    outer[2] = IntPoint(r.right + 10, r.top - 10);
    outer[3] = IntPoint(r.left - 10, r.top - 10);
    Clipper_AddPath(&clpr, outer, ptSubject, true);
    clpr.m_ReverseOutput = (true);
    Clipper_Execute(&clpr, ctUnion, solution, pftNegative, pftNegative);
    //remove the outer PolyNode_t rectangle ...
    if (solution->Childs_size == 1 && solution->Childs[0]->Childs_size > 0) {
      PolyNode_t* outerNode = solution->Childs[0];
      MYREALLOC(solution->Childs, outerNode->Childs_size);
      solution->Childs[0] = outerNode->Childs[0];
      solution->Childs[0]->Parent = outerNode->Parent;
      for (int i = 1; i < outerNode->Childs_size; ++i) {
        PolyNode_AddChild(solution, outerNode->Childs[i]);
      }
    }
    else {
      PolyTree_Clear(solution);
    }
  }
  Clipper_Clear(&clpr);
}
void ClipperOffset_t::DoOffset(double delta)
{
  m_destPolys.clear();
  m_delta = delta;
  //if Zero offset, just copy any CLOSED polygons to m_p and return ...
  if (NEAR_ZERO(delta)) {
    m_destPolys.reserve(m_polyNodes->Childs_size);
    for (int i = 0; i < m_polyNodes->Childs_size; i++) {
      PolyNode_t& node = *m_polyNodes->Childs[i];
      if (node.m_endtype == etClosedPolygon) {
        IPath pg(node.Contour, node.Contour + node.Contour_size);
        m_destPolys.push_back(pg);
      }
    }
    return;
  }
  //see offset_triginometry3.svg in the documentation folder ...
  if (MiterLimit > 2) {
    m_miterLim = 2 / (MiterLimit * MiterLimit);
  }
  else {
    m_miterLim = 0.5;
  }
  double y;
  if (ArcTolerance <= 0.0) {
    y = def_arc_tolerance;
  }
  else if (ArcTolerance > fabs(delta) * def_arc_tolerance) {
    y = fabs(delta) * def_arc_tolerance;
  }
  else {
    y = ArcTolerance;
  }
  //see offset_triginometry2.svg in the documentation folder ...
  double steps = pi / acos(1 - y / fabs(delta));
  if (steps > fabs(delta) * pi) {
    steps = fabs(delta) * pi; //ie excessive precision check
  }
  m_sin = sin(two_pi / steps);
  m_cos = cos(two_pi / steps);
  m_StepsPerRad = steps / two_pi;
  if (delta < 0.0) {
    m_sin = -m_sin;
  }
  m_destPolys.reserve(m_polyNodes->Childs_size * 2);
  for (int i = 0; i < m_polyNodes->Childs_size; i++) {
    PolyNode_t& node = *m_polyNodes->Childs[i];
    m_srcPoly = IPath(node.Contour, node.Contour+node.Contour_size);
    int len = (int)m_srcPoly.size();
    if (len == 0 || (delta <= 0 && (len < 3 || node.m_endtype != etClosedPolygon))) {
      continue;
    }
    m_destPoly.clear();
    if (len == 1) {
      if (node.m_jointype == jtRound) {
        double X = 1.0, Y = 0.0;
        for (cInt j = 1; j <= steps; j++) {
          m_destPoly.push_back(IntPoint(
              Round(m_srcPoly[0].X + X * delta),
              Round(m_srcPoly[0].Y + Y * delta)));
          double X2 = X;
          X = X * m_cos - m_sin * Y;
          Y = X2 * m_sin + Y * m_cos;
        }
      }
      else {
        double X = -1.0, Y = -1.0;
        for (int j = 0; j < 4; ++j) {
          m_destPoly.push_back(IntPoint(
              Round(m_srcPoly[0].X + X * delta),
              Round(m_srcPoly[0].Y + Y * delta)));
          if (X < 0) {
            X = 1;
          }
          else if (Y < 0) {
            Y = 1;
          }
          else {
            X = -1;
          }
        }
      }
      m_destPolys.push_back(m_destPoly);
      continue;
    }
    //build m_normals ...
    m_normals.clear();
    m_normals.reserve(len);
    for (int j = 0; j < len - 1; ++j) {
      m_normals.push_back(GetUnitNormal_(m_srcPoly[j], m_srcPoly[j + 1]));
    }
    if (node.m_endtype == etClosedLine || node.m_endtype == etClosedPolygon) {
      m_normals.push_back(GetUnitNormal_(m_srcPoly[len - 1], m_srcPoly[0]));
    }
    else {
      m_normals.push_back(DoublePoint(m_normals[len - 2]));
    }
    if (node.m_endtype == etClosedPolygon) {
      int k = len - 1;
      for (int j = 0; j < len; ++j) {
        OffsetPoint(j, k, node.m_jointype);
      }
      m_destPolys.push_back(m_destPoly);
    }
    else if (node.m_endtype == etClosedLine) {
      int j, k = len - 1;
      for (j = 0; j < len; ++j) {
        OffsetPoint(j, k, node.m_jointype);
      }
      m_destPolys.push_back(m_destPoly);
      m_destPoly.clear();
      //re-build m_normals ...
      DoublePoint n = m_normals[len - 1];
      for (j = len - 1; j > 0; j--) {
        m_normals[j] = DoublePoint(-m_normals[j - 1].X, -m_normals[j - 1].Y);
      }
      m_normals[0] = DoublePoint(-n.X, -n.Y);
      k = 0;
      for (j = len - 1; j >= 0; j--) {
        OffsetPoint(j, k, node.m_jointype);
      }
      m_destPolys.push_back(m_destPoly);
    }
    else {
      int j, k = 0;
      for (j = 1; j < len - 1; ++j) {
        OffsetPoint(j, k, node.m_jointype);
      }
      IntPoint pt1;
      if (node.m_endtype == etOpenButt) {
        j = len - 1;
        pt1 = IntPoint((cInt)Round(m_srcPoly[j].X + m_normals[j].X *
            delta), (cInt)Round(m_srcPoly[j].Y + m_normals[j].Y * delta));
        m_destPoly.push_back(pt1);
        pt1 = IntPoint((cInt)Round(m_srcPoly[j].X - m_normals[j].X *
            delta), (cInt)Round(m_srcPoly[j].Y - m_normals[j].Y * delta));
        m_destPoly.push_back(pt1);
      }
      else {
        j = len - 1;
        k = len - 2;
        m_sinA = 0;
        m_normals[j] = DoublePoint(-m_normals[j].X, -m_normals[j].Y);
        if (node.m_endtype == etOpenSquare) {
          DoSquare(j, k);
        }
        else {
          DoRound(j, k);
        }
      }
      //re-build m_normals ...
      for (j = len - 1; j > 0; j--) {
        m_normals[j] = DoublePoint(-m_normals[j - 1].X, -m_normals[j - 1].Y);
      }
      m_normals[0] = DoublePoint(-m_normals[1].X, -m_normals[1].Y);
      k = len - 1;
      for (j = k - 1; j > 0; --j) {
        OffsetPoint(j, k, node.m_jointype);
      }
      if (node.m_endtype == etOpenButt) {
        pt1 = IntPoint((cInt)Round(m_srcPoly[0].X - m_normals[0].X * delta),
            (cInt)Round(m_srcPoly[0].Y - m_normals[0].Y * delta));
        m_destPoly.push_back(pt1);
        pt1 = IntPoint((cInt)Round(m_srcPoly[0].X + m_normals[0].X * delta),
            (cInt)Round(m_srcPoly[0].Y + m_normals[0].Y * delta));
        m_destPoly.push_back(pt1);
      }
      else {
        k = 1;
        m_sinA = 0;
        if (node.m_endtype == etOpenSquare) {
          DoSquare(0, 1);
        }
        else {
          DoRound(0, 1);
        }
      }
      m_destPolys.push_back(m_destPoly);
    }
  }
}
void ClipperOffset_t::OffsetPoint(int j, int& k, JoinType jointype)
{
  //cross product ...
  m_sinA = (m_normals[k].X * m_normals[j].Y - m_normals[j].X * m_normals[k].Y);
  if (fabs(m_sinA * m_delta) < 1.0) {
    //dot product ...
    double cosA = (m_normals[k].X * m_normals[j].X + m_normals[j].Y * m_normals[k].Y);
    if (cosA > 0) { // angle => 0 degrees
      m_destPoly.push_back(IntPoint(Round(m_srcPoly[j].X + m_normals[k].X * m_delta),
          Round(m_srcPoly[j].Y + m_normals[k].Y * m_delta)));
      return;
    }
    //else angle => 180 degrees
  }
  else if (m_sinA > 1.0) {
    m_sinA = 1.0;
  }
  else if (m_sinA < -1.0) {
    m_sinA = -1.0;
  }
  if (m_sinA * m_delta < 0) {
    m_destPoly.push_back(IntPoint(Round(m_srcPoly[j].X + m_normals[k].X * m_delta),
        Round(m_srcPoly[j].Y + m_normals[k].Y * m_delta)));
    m_destPoly.push_back(m_srcPoly[j]);
    m_destPoly.push_back(IntPoint(Round(m_srcPoly[j].X + m_normals[j].X * m_delta),
        Round(m_srcPoly[j].Y + m_normals[j].Y * m_delta)));
  }
  else
    switch (jointype) {
    case jtMiter: {
      double r = 1 + (m_normals[j].X * m_normals[k].X +
          m_normals[j].Y * m_normals[k].Y);
      if (r >= m_miterLim) {
        DoMiter(j, k, r);
      }
      else {
        DoSquare(j, k);
      }
      break;
    }
    case jtSquare:
      DoSquare(j, k);
      break;
    case jtRound:
      DoRound(j, k);
      break;
    }
  k = j;
}
void ClipperOffset_t::DoSquare(int j, int k)
{
  double dx = tan(atan2(m_sinA,
      m_normals[k].X * m_normals[j].X + m_normals[k].Y * m_normals[j].Y) / 4);
  m_destPoly.push_back(IntPoint(
      Round(m_srcPoly[j].X + m_delta * (m_normals[k].X - m_normals[k].Y * dx)),
      Round(m_srcPoly[j].Y + m_delta * (m_normals[k].Y + m_normals[k].X * dx))));
  m_destPoly.push_back(IntPoint(
      Round(m_srcPoly[j].X + m_delta * (m_normals[j].X + m_normals[j].Y * dx)),
      Round(m_srcPoly[j].Y + m_delta * (m_normals[j].Y - m_normals[j].X * dx))));
}
void ClipperOffset_t::DoMiter(int j, int k, double r)
{
  double q = m_delta / r;
  m_destPoly.push_back(IntPoint(Round(m_srcPoly[j].X + (m_normals[k].X + m_normals[j].X) * q),
      Round(m_srcPoly[j].Y + (m_normals[k].Y + m_normals[j].Y) * q)));
}
void ClipperOffset_t::DoRound(int j, int k)
{
  double a = atan2(m_sinA,
      m_normals[k].X * m_normals[j].X + m_normals[k].Y * m_normals[j].Y);
  int t = (int)Round(m_StepsPerRad * fabs(a));
  int steps = MAX(t, 1);
  double X = m_normals[k].X, Y = m_normals[k].Y, X2;
  for (int i = 0; i < steps; ++i) {
    m_destPoly.push_back(IntPoint(
        Round(m_srcPoly[j].X + X * m_delta),
        Round(m_srcPoly[j].Y + Y * m_delta)));
    X2 = X;
    X = X * m_cos - m_sin * Y;
    Y = X2 * m_sin + Y * m_cos;
  }
  m_destPoly.push_back(IntPoint(
      Round(m_srcPoly[j].X + m_normals[j].X * m_delta),
      Round(m_srcPoly[j].Y + m_normals[j].Y * m_delta)));
}

void ReversePaths_(Paths& p)
{
  for (Paths::size_type i = 0; i < p.size(); ++i) {
    ReversePath(p[i]);
  }
}
void SimplifyPolygon1(const IPath& in_poly, Paths& out_polys, PolyFillType fillType)
{
  Clipper_t c;
  Clipper_Init(&c);
  c.m_StrictSimple = (true);
  Clipper_AddPath(&c, in_poly, ptSubject, true);
  Clipper_Execute(&c, ctUnion, out_polys, fillType, fillType);
  Clipper_Free(&c);
}
void SimplifyPolygons1(const Paths& in_polys, Paths& out_polys, PolyFillType fillType)
{
  Clipper_t c;
  Clipper_Init(&c);
  c.m_StrictSimple = (true);
  Clipper_AddPaths(&c, in_polys, ptSubject, true);
  Clipper_Execute(&c, ctUnion, out_polys, fillType, fillType);
  Clipper_Free(&c);
}
void SimplifyPolygons1(Paths& polys, PolyFillType fillType)
{
  SimplifyPolygons1(polys, polys, fillType);
}
inline double DistanceSqrd_(const IntPoint& pt1, const IntPoint& pt2)
{
  double Dx = ((double)pt1.X - pt2.X);
  double dy = ((double)pt1.Y - pt2.Y);
  return (Dx * Dx + dy * dy);
}
double DistanceFromLineSqrd_(
    const IntPoint& pt, const IntPoint& ln1, const IntPoint& ln2)
{
  //The equation of a line in general form (Ax + By + C = 0)
  //given 2 points (x?y? & (x?y? is ...
  //(y?- y?x + (x?- x?y + (y?- y?x?- (x?- x?y?= 0
  //A = (y?- y?; B = (x?- x?; C = (y?- y?x?- (x?- x?y?
  //perpendicular distance of point (x?y? = (Ax?+ By?+ C)/Sqrt(A?+ B?
  //see http://en.wikipedia.org/wiki/Perpendicular_distance
  double A = double(ln1.Y - ln2.Y);
  double B = double(ln2.X - ln1.X);
  double C = A * ln1.X + B * ln1.Y;
  C = A * pt.X + B * pt.Y - C;
  return (C * C) / (A * A + B * B);
}
//---------------------------------------------------------------------------
bool SlopesNearCollinear_(const IntPoint& pt1,
    const IntPoint& pt2, const IntPoint& pt3, double distSqrd)
{
  //this function is more accurate when the point that's geometrically
  //between the other 2 points is the one that's tested for distance.
  //ie makes it more likely to pick up 'spikes' ...
  if (Abs(pt1.X - pt2.X) > Abs(pt1.Y - pt2.Y)) {
    if ((pt1.X > pt2.X) == (pt1.X < pt3.X)) {
      return DistanceFromLineSqrd_(pt1, pt2, pt3) < distSqrd;
    }
    else if ((pt2.X > pt1.X) == (pt2.X < pt3.X)) {
      return DistanceFromLineSqrd_(pt2, pt1, pt3) < distSqrd;
    }
    else {
      return DistanceFromLineSqrd_(pt3, pt1, pt2) < distSqrd;
    }
  }
  else {
    if ((pt1.Y > pt2.Y) == (pt1.Y < pt3.Y)) {
      return DistanceFromLineSqrd_(pt1, pt2, pt3) < distSqrd;
    }
    else if ((pt2.Y > pt1.Y) == (pt2.Y < pt3.Y)) {
      return DistanceFromLineSqrd_(pt2, pt1, pt3) < distSqrd;
    }
    else {
      return DistanceFromLineSqrd_(pt3, pt1, pt2) < distSqrd;
    }
  }
}
bool PointsAreClose_(IntPoint pt1, IntPoint pt2, double distSqrd)
{
  double Dx = (double)pt1.X - pt2.X;
  double dy = (double)pt1.Y - pt2.Y;
  return ((Dx * Dx) + (dy * dy) <= distSqrd);
}
OutPt_t* ExcludeOp_(OutPt_t* op)
{
  OutPt_t* result = op->Prev;
  result->Next = op->Next;
  op->Next->Prev = result;
  result->Idx = 0;
  return result;
}
void CleanPolygon_(const IPath& in_poly, IPath& out_poly, double distance)
{
  size_t i;
  //distance = proximity in units/pixels below which vertices
  //will be stripped. Default ~= sqrt(2).
  size_t size = in_poly.size();
  if (size == 0) {
    out_poly.clear();
    return;
  }
  OutPt_t* outPts = new OutPt_t[size];
  for (i = 0; i < size; ++i) {
    outPts[i].Pt = in_poly[i];
    outPts[i].Next = &outPts[(i + 1) % size];
    outPts[i].Next->Prev = &outPts[i];
    outPts[i].Idx = 0;
  }
  double distSqrd = distance * distance;
  OutPt_t* op = &outPts[0];
  while (op->Idx == 0 && op->Next != op->Prev) {
    if (PointsAreClose_(op->Pt, op->Prev->Pt, distSqrd)) {
      op = ExcludeOp_(op);
      size--;
    }
    else if (PointsAreClose_(op->Prev->Pt, op->Next->Pt, distSqrd)) {
      ExcludeOp_(op->Next);
      op = ExcludeOp_(op);
      size -= 2;
    }
    else if (SlopesNearCollinear_(op->Prev->Pt, op->Pt, op->Next->Pt, distSqrd)) {
      op = ExcludeOp_(op);
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
void CleanPolygon_(IPath& poly, double distance)
{
  CleanPolygon_(poly, poly, distance);
}
void CleanPolygons1(const Paths& in_polys, Paths& out_polys, double distance)
{
  for (Paths::size_type i = 0; i < in_polys.size(); ++i) {
    CleanPolygon_(in_polys[i], out_polys[i], distance);
  }
}
void CleanPolygons1(Paths& polys, double distance)
{
  CleanPolygons1(polys, polys, distance);
}
void Minkowski_(const IPath& poly, const IPath& path, Paths& solution, bool isSum, bool isClosed)
{
  int delta = (isClosed ? 1 : 0);
  size_t polyCnt = poly.size();
  size_t pathCnt = path.size();
  Paths pp;
  pp.reserve(pathCnt);
  if (isSum) {
    for (size_t i = 0; i < pathCnt; ++i) {
      IPath p;
      p.reserve(polyCnt);
      for (size_t j = 0; j < poly.size(); ++j) {
        p.push_back(IntPoint(path[i].X + poly[j].X, path[i].Y + poly[j].Y));
      }
      pp.push_back(p);
    }
  } else {
    for (size_t i = 0; i < pathCnt; ++i) {
      IPath p;
      p.reserve(polyCnt);
      for (size_t j = 0; j < poly.size(); ++j) {
        p.push_back(IntPoint(path[i].X - poly[j].X, path[i].Y - poly[j].Y));
      }
      pp.push_back(p);
    }
  }
  solution.clear();
  solution.reserve((pathCnt + delta) * (polyCnt + 1));
  for (size_t i = 0; i < pathCnt - 1 + delta; ++i) {
    for (size_t j = 0; j < polyCnt; ++j) {
      IPath quad;
      quad.reserve(4);
      quad.push_back(pp[i % pathCnt][j % polyCnt]);
      quad.push_back(pp[(i + 1) % pathCnt][j % polyCnt]);
      quad.push_back(pp[(i + 1) % pathCnt][(j + 1) % polyCnt]);
      quad.push_back(pp[i % pathCnt][(j + 1) % polyCnt]);
      if (!Orientation(quad)) {
        ReversePath(quad);
      }
      solution.push_back(quad);
    }
  }
}
void MinkowskiSum_(const IPath& pattern, const IPath& path, Paths& solution, bool pathIsClosed)
{
  Minkowski_(pattern, path, solution, true, pathIsClosed);
  Clipper_t c;
  Clipper_Init(&c);
  Clipper_AddPaths(&c, solution, ptSubject, true);
  Clipper_Execute(&c, ctUnion, solution, pftNonZero, pftNonZero);
  Clipper_Free(&c);
}
void TranslatePath_(const IPath& input, IPath& output, IntPoint delta)
{
  //precondition: input != output
  output.resize(input.size());
  for (size_t i = 0; i < input.size(); ++i) {
    output[i] = IntPoint(input[i].X + delta.X, input[i].Y + delta.Y);
  }
}
void MinkowskiSum_(const IPath& pattern, const Paths& paths, Paths& solution, bool pathIsClosed)
{
  Clipper_t c;
  Clipper_Init(&c);
  for (size_t i = 0; i < paths.size(); ++i) {
    Paths tmp;
    Minkowski_(pattern, paths[i], tmp, true, pathIsClosed);
    Clipper_AddPaths(&c, tmp, ptSubject, true);
    if (pathIsClosed) {
      IPath tmp2;
      TranslatePath_(paths[i], tmp2, pattern[0]);
      Clipper_AddPath(&c, tmp2, ptClip, true);
    }
  }
  Clipper_Execute(&c, ctUnion, solution, pftNonZero, pftNonZero);
  Clipper_Free(&c);
}
void MinkowskiDiff_(const IPath& poly1, const IPath& poly2, Paths& solution)
{
  Minkowski_(poly1, poly2, solution, false, true);
  Clipper_t c;
  Clipper_Init(&c);
  Clipper_AddPaths(&c, solution, ptSubject, true);
  Clipper_Execute(&c, ctUnion, solution, pftNonZero, pftNonZero);
  Clipper_Free(&c);
}

void AddPolyNodeToPaths_(const PolyNode_t* polynode, NodeType nodetype, Paths& paths)
{
  bool match = true;
  if (nodetype == ntClosed) {
    match = !polynode->m_IsOpen;
  }
  else if (nodetype == ntOpen) {
    return;
  }
  if (polynode->Contour_size>0 && match) {
    paths.push_back(IPath(polynode->Contour, polynode->Contour+polynode->Contour_size));
  }
  for (int i = 0; i < polynode->Childs_size; ++i) {
    AddPolyNodeToPaths_(polynode->Childs[i], nodetype, paths);
  }
}
void PolyTreeToPaths(const PolyTree_t* polytree, Paths& paths)
{
  paths.resize(0);
  paths.reserve(PolyTree_Total(polytree));
  AddPolyNodeToPaths_(polytree, ntAny, paths);
}
void ClosedPathsFromPolyTree(const PolyTree_t* polytree, Paths& paths)
{
  paths.resize(0);
  paths.reserve(PolyTree_Total(polytree));
  AddPolyNodeToPaths_(polytree, ntClosed, paths);
}
void OpenPathsFromPolyTree(PolyTree_t* polytree, Paths& paths)
{
  paths.resize(0);
  paths.reserve(PolyTree_Total(polytree));
  //Open paths are top level only, so ...
  for (int i = 0; i < polytree->Childs_size; ++i) {
    if (polytree->Childs[i]->m_IsOpen) {
      PolyNode_t* pn = polytree->Childs[i];
      paths.push_back(IPath(pn->Contour, pn->Contour+pn->Contour_size));
    }
  }
}


#endif // _CLIPPER_C_INL_
