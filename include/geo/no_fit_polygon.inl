//NFP(no_fit_polygon) 算法
// 下面贴出的算法是根据轨迹法求取的，其中对于空洞并未处理。此算法的关键是采用了64位分数近似表示小数的办法来解决轨迹的追踪。因为小数会带来不可避免的误差，导致轨迹封闭失败。
//文件 IntBasic.h, 此文件中大部分系源自GRAPHICS GEMS并改写。
///// FileName: IntBasic.h
///// Function: Int Operation for Segment-Algorithms
///// Author: youlin.wang
///// Date: 2008-11-12
#ifndef __INTBASIC_H__
#define __INTBASIC_H__
#include <utility>
// sizes are based on the assumption that size of a char is 1 byte
// C++ standard says that sizeof(char) is 1, since sizeof is based
// on character sizes
// sizeof other types returns multiples of sizeof char
// sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long)
template <bool b, typename T, typename F>
struct IF {
};
template <typename T, typename F>
struct IF<true, T, F> {
  typedef T result;
};
template <typename T, typename F>
struct IF<false, T, F> {
  typedef F result;
};
typedef char StdInt8;
typedef IF < sizeof(short) == 2, short,
        IF < sizeof(int) == 2, int,
        IF < sizeof(long) == 2, long, long long
        >::result
        >::result
        >::result StdInt16;
typedef IF < sizeof(short) == 4, short,
        IF < sizeof(int) == 4, int,
        IF < sizeof(long) == 4, long, long long
        >::result
        >::result
        >::result StdInt32;
typedef IF < sizeof(short) == 8, short,
        IF < sizeof(int) == 8, int,
        IF < sizeof(long) == 8, long, long long
        >::result
        >::result
        >::result StdInt64;
#define COMPARE_SMALL -1
#define COMPARE_EQUAL 0
#define COMPARE_BIG 1
#define isCoordInRange(X) (((X) > -268435456) && ((X) < 268435456)) // 所有的端点坐标必须在这个范围内 
#define isSameSign(A, B) (((__int64)((unsigned __int64)A ^ (unsigned __int64)B)) >= 0) //如果A和B同符号则为真 
#define getMaxSaveMin(x1, x2, minV) (x1 >= x2 ? (minV = x2, x1) : (minV = x1, x2)) //返回最大值,并保存最小值在min中 
//32Bit整数点
struct Int32Point2 {
  Int32Point2()
    : x(0), y(0) {
  }
  Int32Point2(long ix, long iy)
    : x(ix), y(iy) {
  }
  /*long*/StdInt32 x, y;
} ;
inline bool operator==(const Int32Point2& p1, const Int32Point2& p2)
{
  return (p1.x == p2.x && p1.y == p2.y);
}
inline double nfpRound(double v , double inscr)
{
  return v > 0 ? v + inscr : v - inscr;
}
//基于32Bit整数点的线段
struct LongSegment2 {
  Int32Point2 first, last; /* defined to be ordered from "first" to "last" */
} ;
//64Bit分子和分母构成的分数.可表示沿着32Bit整数线段的参数,分子/分母
struct Int64Fraction {
  __int64 num, denom;
} ;
struct IntersectParam {
  Int64Fraction par[2]; /* par[0] 在第一个线段上的参数, par[1] 在第二个线段上的参数 */
  __int64 a, b, c, d; /* 保存这些会让叉积方向计算更快 */
} ;
inline Int32Point2 operator +(const Int32Point2& p1, const Int32Point2& p2)
{
  return Int32Point2(p1.x + p2.x, p1.y + p2.y);
}
inline Int32Point2 operator -(const Int32Point2& p1, const Int32Point2& p2)
{
  return Int32Point2(p1.x - p2.x, p1.y - p2.y);
}
inline __int64 long_crossProduct(Int32Point2 p, Int32Point2 p1, Int32Point2 p2)
{
  return (__int64)(p.x - p1.x) * (__int64)(p2.y - p1.y) - (__int64)(p.y - p1.y) * (__int64)(p2.x - p1.x);
}
inline __int64 long_crossProduct(Int32Point2 v1, Int32Point2 v2)
{
  return (__int64)v1.x * (__int64)v2.y - (__int64)v1.y * (__int64)v2.x;
}
inline bool isVertexTouchEdge(const std::pair<Int32Point2, Int32Point2>& dualEdgeOfVertex, const Int32Point2& edgeR)
{
  //concave vertex should be eliminated
  if (long_crossProduct(dualEdgeOfVertex.first, dualEdgeOfVertex.second) < 1) {
    return false;
  }
  return long_crossProduct(dualEdgeOfVertex.first, edgeR) >= 0 && long_crossProduct(edgeR, dualEdgeOfVertex.second) >= 0 ;
}
inline __int64 long_crossProduct(const LongSegment2& s1, const LongSegment2& s2)
{
  return long_crossProduct(s1.last - s1.first, s2.last - s1.first);
}
inline const Int32Point2& getMinYPoint(const LongSegment2& seg)
{
  return seg.first.y > seg.last.y ? seg.last : (seg.first.y < seg.last.y ? seg.first : (seg.first.x > seg.last.x ? seg.last : seg.first)) ;
}
inline void getBoundingValue(const LongSegment2& seg, long& oleft, long& oright, long& obottom, long& otop)
{
  if (seg.first.x > seg.last.x) {
    oleft = seg.last.x;
    oright = seg.first.x;
  }
  else {
    oleft = seg.first.x;
    oright = seg.last.x;
  }
  if (seg.first.y > seg.last.y) {
    obottom = seg.last.y;
    otop = seg.first.y;
  }
  else {
    otop = seg.last.y;
    obottom = seg.first.y;
  }
}
/* *********************************************************************
计算两个原始线段的交点.
返回值: 0 没有交点
1 一个交点
2 共线
交点参数返回在ipt中
 输入: s1, s2: 原始线段
********************************************************************* */
inline int SegIntersect(const LongSegment2* s1, const LongSegment2* s2, IntersectParam* ipt)
{
  __int64 a, b, c, d, tdet, sdet, det; /* parameter calculation variables */
  long max1, max2, min1, min2; /* bounding box check variables */
  Int32Point2 p1, p2, q1, q2; /* dereference a priori endpoints */
  p1 = s1->first;
  p2 = s1->last;
  q1 = s2->first;
  q2 = s2->last;
  /* 首先做包容盒测试 */
  max1 = getMaxSaveMin(p1.x, p2.x, min1);
  max2 = getMaxSaveMin(q1.x, q2.x, min2);
  if ((max1 < min2) || (min1 > max2)) {
    return(0);
  }
  max1 = getMaxSaveMin(p1.y, p2.y, min1);
  max2 = getMaxSaveMin(q1.y, q2.y, min2);
  if ((max1 < min2) || (min1 > max2)) {
    return(0);
  }
  /* 测试第二条线段的两个端点是否在第一个线段的两侧. 如果在同侧则不相交 */
  a = (__int64)(q1.x - p1.x) * (__int64)(p2.y - p1.y) - (__int64)(q1.y - p1.y) * (__int64)(p2.x - p1.x);
  b = (__int64)(q2.x - p1.x) * (__int64)(p2.y - p1.y) - (__int64)(q2.y - p1.y) * (__int64)(p2.x - p1.x);
  if (a != 0 && b != 0 && isSameSign(a, b)) {
    return(0);
  }
  /* 测试第一条线段的两个端点是否在第二条线段的两侧, 如果在同侧则不相交 */
  c = (__int64)(p1.x - q1.x) * (__int64)(q2.y - q1.y) - (__int64)(p1.y - q1.y) * (__int64)(q2.x - q1.x);
  d = (__int64)(p2.x - q1.x) * (__int64)(q2.y - q1.y) - (__int64)(p2.y - q1.y) * (__int64)(q2.x - q1.x);
  if (c != 0 && d != 0 && isSameSign(c, d)) {
    return(0);
  }
  /* At this point each tagLongSegment2 meets the line of the other. */
  det = a - b;
  if (det == 0) {
    return(2); /* 线段共线. */
  }
  /* 线段互相交叉,线段必然相交. 而且 因为线段不平行,a和b不会同时为0. c和d也不会同时为0 */
  tdet = -c;
  sdet = a;
  if (det < 0) { /* 参数的分母必须设为正值 */
    det = -det;
    sdet = -sdet;
    tdet = -tdet;
  }
  ipt->a = a;
  ipt->b = b;
  ipt->c = c;
  ipt->d = d;
  ipt->par[0].num = tdet;
  ipt->par[0].denom = det;
  ipt->par[1].num = sdet;
  ipt->par[1].denom = det;
  return(1);
}
#define LONGMASK 0xffffffff
/* *********************************************************************
64 bit multiplication function.
Multiply two long integers in1 and in2, returning the result in out.
********************************************************************* */
inline void mult128(__int64 in1, __int64 in2, unsigned __int64 out[2])
{
  unsigned long* x, *y, *z;
  unsigned __int64 temp;
  x = (unsigned long*) &in1;
  y = (unsigned long*) &in2;
  z = (unsigned long*) out;
  temp = (__int64)x[0] * (__int64)y[0];
  z[1] = temp >> 32;
  z[0] = temp & LONGMASK;
  temp = (__int64)x[0] * (__int64)y[1];
  z[2] = temp >> 32;
  z[1] += temp & LONGMASK;
  z[2] += z[1] >> 32;
  z[1] = z[1] & LONGMASK;
  temp = (__int64)x[1] * (__int64)y[0];
  z[2] += temp >> 32;
  z[1] += temp & LONGMASK;
  z[2] += z[1] >> 32;
  z[1] = z[1] & LONGMASK;
  z[3] = z[2] >> 32;
  z[2] = z[2] & LONGMASK;
  temp = (__int64)x[1] * (__int64)y[1];
  z[3] += temp >> 32;
  z[2] += temp & LONGMASK;
  z[3] += z[2] >> 32;
  z[2] = z[2] & LONGMASK;
}
/* *********************************************************************
Comparison primitive to test par1 <= par2.
Return LEFT if par1 < par2; return ON if par1 = par2;
return RIGHT if par1 > par2.
********************************************************************* */
inline int CompPrim(Int64Fraction* par1, Int64Fraction* par2)
{
  unsigned __int64 r1[2], r2[2];
  mult128(par1->num, par2->denom, r1);
  mult128(par2->num, par1->denom, r2);
  if (r1[1] != r2[1]) {
    if (r1[1] < r2[1]) {
      return(COMPARE_SMALL);
    }
    else {
      return(COMPARE_BIG);
    }
  }
  if (r1[0] != r2[0]) {
    if (r1[0] < r2[0]) {
      return(COMPARE_SMALL);
    }
    else {
      return(COMPARE_BIG);
    }
  }
  return(0);
}
/* *********************************************************************
Helper function for all parameter comparisons.
Returns LEFT if par1 < par2, ON if par1 = par2, and RIGHT otherwise.
Denominators must be positive.
********************************************************************* */
inline int CompHelp(const Int64Fraction* par1, const Int64Fraction* par2)
{
  Int64Fraction tpar1, tpar2;
  tpar1 = *par1;
  tpar2 = *par2;
  if (tpar1.num < 0) {
    if (tpar2.num >= 0) {
      return(COMPARE_SMALL);
    }
    tpar1.num = -tpar1.num;
    tpar2.num = -tpar2.num;
    return(CompPrim(&tpar2, &tpar1));
  }
  if (tpar2.num < 0) {
    return(COMPARE_BIG);
  }
  return(CompPrim(&tpar1, &tpar2));
}
#endif
//文件 NfpBasic.h 关键算法在此。
///// FileName: NfpBasic.h
///// Function: Algorithms for No-Fit-Polygon
///// Author: youlin.wang
///// Date: 2008-11-12
#ifndef __NFPBASIC_H__
#define __NFPBASIC_H__
#include "IntBasic.h"
#include <functional>
#include <vector>
#include <cassert>
namespace Nfp
{
const double RATIO_F2LONG = 1000;
const double RATIO_LONG2F = 1 / RATIO_F2LONG;
typedef std::vector<Int32Point2> LongPoint2List;
typedef std::vector<LongSegment2> LongSegment2List;
typedef LongSegment2List::const_iterator LongSegment2CIter;
typedef std::vector<LongSegment2CIter> LongSegment2CIterList;
struct FunObjBottomLeftLongSegment : public std::binary_function<const LongSegment2&, const LongSegment2&, bool> {
  bool operator()(const LongSegment2& sa, const LongSegment2& sb) const {
    return sa.first.y < sb.first.y || (sa.first.y == sb.first.y && sa.first.x < sb.first.x);
  }
};
template<class _Point>
class NfpCalculator
{
public:
  typedef typename std::vector<_Point> _PointList;
  typedef typename _PointList::const_iterator _PointCIter;
  template<class _VertexList>
  NfpCalculator(const _VertexList& iPointsOfPolygonS, const _VertexList& iPointsOfPolygonM) {
    for (_VertexList::const_iterator itFP = iPointsOfPolygonS.begin(); itFP != iPointsOfPolygonS.end(); ++itFP) {
      mPointListS.push_back(Int32Point2(long(nfpRound(itFP->x()*RATIO_F2LONG, 0.5)), long(nfpRound(itFP->y()*RATIO_F2LONG, 0.5))));
    }
    for (_VertexList::const_iterator itFP = iPointsOfPolygonM.begin(); itFP != iPointsOfPolygonM.end(); ++itFP) {
      mPointListM.push_back(Int32Point2(long(nfpRound(itFP->x()*RATIO_F2LONG, 0.5)), long(nfpRound(itFP->y()*RATIO_F2LONG, 0.5))));
    }
  }
  ~NfpCalculator(void) {
  }
  void computeNoFitPolygon(_PointList& oPointList);
private:
  LongSegment2CIter searchNextSegment(LongSegment2CIter itSeg, const Int64Fraction& currentPointParam, const LongSegment2CIterList& tabuList, Int64Fraction& nextPointParam);
  static _Point toDoublePoint(LongSegment2CIter seg, const Int64Fraction& f) {
    Int32Point2 v = seg->last - seg->first;
    double t = (double)f.num / (double)f.denom;
    return _Point((seg->first.x + v.x * t) * RATIO_LONG2F, (seg->first.y + v.y * t) * RATIO_LONG2F);
  }
  //获取点在线段上的参数
  static void getLongPointParamInSegment(LongSegment2CIter itSeg, const Int32Point2& p, Int64Fraction& f) {
    bool isHorSegment = itSeg->first.y == itSeg->last.y;
    if (isHorSegment) {
      f.num = p.x - itSeg->first.x;
      f.denom = itSeg->last.x - itSeg->first.x;
    }
    else {
      f.num = p.y - itSeg->first.y;
      f.denom = itSeg->last.y - itSeg->first.y;
    }
    if (f.denom < 0) {
      f.denom = -f.denom;
      f.num = -f.num;
    }
  }
  //计算可能相交的线段集合
  void getMaybeIntersectedSegmentList(LongSegment2CIter itSeg, LongSegment2CIterList& intersectSegList) const {
    long minY, minX, maxX, maxY, tminX, tminY, tmaxX, tmaxY;
    getBoundingValue(*itSeg, minX, maxX, minY, maxY);
    for (LongSegment2CIter it = mTraceList.begin(); it != mTraceList.end(); ++it) {
      getBoundingValue(*it, tminX, tmaxX, tminY, tmaxY);
      if (tminX > maxX || tmaxX < minX || tminY > maxY || tmaxY < minY) {
        continue;
      }
      else {
        intersectSegList.push_back(it);
      }
    }
  }
  void computeNfpTrace();
  const Int32Point2 longVertexS(int i) const {
    if (i == mPointListS.size()) {
      i = 0;
    }
    if (i == -1) {
      i = mPointListS.size() - 1;
    }
    return mPointListS[i] + mLongTranslationS;
  }
  const Int32Point2 longVertexM(int i) const {
    if (i == mPointListM.size()) {
      i = 0;
    }
    if (i == -1) {
      i = mPointListM.size() - 1;
    }
    return mPointListM[i] + mLongTranslationM;
  }
  const Int32Point2 longEdgeS(int i) const {
    return Int32Point2(longVertexS(i + 1) - longVertexS(i));
  }
  const Int32Point2 longEdgeM(int i) const {
    return Int32Point2(longVertexM(i + 1) - longVertexM(i));
  }
  const Int32Point2 longEdgeReversedS(int i) const {
    return Int32Point2(longVertexS(i) - longVertexS(i + 1));
  }
  const Int32Point2 longEdgeReversedM(int i) const {
    return Int32Point2(longVertexM(i) - longVertexM(i + 1));
  }
  const std::pair<Int32Point2, Int32Point2> longPairEdgeS(int i) const {
    return std::pair<Int32Point2, Int32Point2>(longVertexS(i) - longVertexS(i - 1), longVertexS(i + 1) - longVertexS(i));
  }
  const std::pair<Int32Point2, Int32Point2> longPairEdgeM(int i) const {
    return std::pair<Int32Point2, Int32Point2>(longVertexM(i) - longVertexM(i - 1), longVertexM(i + 1) - longVertexM(i));
  }
  const Int32Point2 longCenterS() const {
    return mLongTranslationS;
  }
  const Int32Point2 longCenterM() const {
    return mLongTranslationM;
  }
  void longTranslateS(const Int32Point2& trans) {
    mLongTranslationS.x += trans.x;
    mLongTranslationS.y += trans.y;
  }
  void longTranslateM(const Int32Point2& trans) {
    mLongTranslationM.x += trans.x;
    mLongTranslationM.y += trans.y;
  }
  LongPoint2List mPointListS;
  LongPoint2List mPointListM;
  Int32Point2 mLongTranslationS;
  Int32Point2 mLongTranslationM;
  LongSegment2List mTraceList;
};
template<class _Point>
void NfpCalculator<_Point>::computeNfpTrace()
{
  size_t mSize = mPointListM.size();
  size_t fSize = mPointListS.size();
  std::pair<Int32Point2, Int32Point2> dv;
  Int32Point2 redge;
  LongSegment2 tempSegment;
  for (size_t i = 0; i < mSize; ++i) {
    dv = longPairEdgeM(i);
    for (size_t j = 0; j < fSize; ++j) {
      redge = longEdgeReversedS(j);
      if (isVertexTouchEdge(dv, redge)) {
        longTranslateM(longVertexS(j) - longVertexM(i));
        tempSegment.first = longCenterM();
        tempSegment.last = tempSegment.first - redge;
        mTraceList.push_back(tempSegment);
      }
    }
  }
  for (size_t i = 0; i < mSize; ++i) {
    redge = longEdgeReversedM(i);
    for (size_t j = 0; j < fSize; ++j) {
      dv = longPairEdgeS(j);
      if (isVertexTouchEdge(dv, redge)) {
        longTranslateM(longVertexS(j) - longVertexM(i));
        tempSegment.first = longCenterM();
        tempSegment.last = tempSegment.first + redge;
        mTraceList.push_back(tempSegment);
      }
    }
  }
}
template<class _Point>
void NfpCalculator<_Point>::computeNoFitPolygon(_PointList& oPointList)
{
  computeNfpTrace();
  LongSegment2CIter itStart = std::min_element(mTraceList.begin(), mTraceList.end(), FunObjBottomLeftLongSegment());
  LongSegment2CIter itCurrent = itStart;
  Int64Fraction currentParam, nextParam;
  currentParam.denom = 1;
  currentParam.num = 0;
  LongSegment2CIterList tabulist;
  do {
    oPointList.push_back(toDoublePoint(itCurrent, currentParam));
    itCurrent = searchNextSegment(itCurrent, currentParam, tabulist, nextParam);
    assert(itCurrent != mTraceList.end());
    currentParam = nextParam;
  }
  while (itCurrent != itStart);
}
template<class _Point>
LongSegment2CIter NfpCalculator<_Point>::searchNextSegment(LongSegment2CIter itSeg, const Int64Fraction& currentPointParam, const LongSegment2CIterList& tabuList, Int64Fraction& nextPointParam)
{
  int intersectResult;//临时变量,保存相交测试的结果
  IntersectParam pInst;//临时变量,保存交点参数
  IntersectParam pInstOptimal;
  pInstOptimal.par[0].denom = 1;
  pInstOptimal.par[0].num = 2;
  Int64Fraction pColinearParam;
  pColinearParam.denom = 1;
  pColinearParam.num = 1;
  //确定可能相交的线段集合
  LongSegment2CIterList hasIntersectSegList;
  LongSegment2CIterList mayIntersectSegList;
  getMaybeIntersectedSegmentList(itSeg, mayIntersectSegList);
  //最佳单交点
  LongSegment2CIterList::const_iterator optimalSingle = mayIntersectSegList.end();
  //最佳共线交线段
  LongSegment2CIterList::const_iterator optimalColinear = mayIntersectSegList.end();
  //遍历可能相交的线段集合
  for (LongSegment2CIterList::const_iterator itIt = mayIntersectSegList.begin();
      itIt != mayIntersectSegList.end(); ++itIt) {
    //如果在禁忌表中,则不做测试
    if (std::find(tabuList.begin(), tabuList.end(), *itIt) != tabuList.end() || *itIt == itSeg) {
      continue;
    }
    //做相交测试
    intersectResult = SegIntersect(&(*itSeg), &(**itIt), &pInst);
    //一个交点
    if (intersectResult == 1) {
      //如果终点和终点相交，舍弃？
      if (pInst.par[0].denom == pInst.par[0].num) {
        if (itSeg->last == (*itIt)->last) {
          continue;
        }
      }
      else {
        //如果 不交在终点,而在左侧的舍弃
        if (long_crossProduct((*itIt)->last, itSeg->first, itSeg->last) < 1) { /*&& pInst.par[0].denom!=pInst.par[0].num)*/
          continue;
        }
      }
      //小于等于当前点的舍弃
      if (/*compareInt64Fraction*/CompHelp(¤tPointParam, &(pInst.par[0])) > 0) {
        continue;
      }
      int c =/*compareInt64Fraction*/CompHelp(&pInst.par[0], &pInstOptimal.par[0]);
      if (c < 0 || (c == 0 && long_crossProduct(**itIt, **optimalSingle) > 0)) {
        optimalSingle = itIt;
        pInstOptimal = pInst;
      }
    }
    else if (intersectResult == 2) {
      //判断共线线段的终点是否超出本线段,未超出则舍弃,超出则继续求交
      //和最长的共线交线段比较
      Int64Fraction f;
      getLongPointParamInSegment(itSeg, (*itIt)->last, f);
      if (/*compareInt64Fraction*/CompHelp(&f, &pColinearParam) > 0) {
        pColinearParam = f;
        optimalColinear = itIt;
      }
    }
    if (intersectResult > 0) {
      hasIntersectSegList.push_back(*itIt);
    }
  }
  //如果有单交点,并且是点在右侧,无需做共线测试即可返回
  if (optimalSingle != mayIntersectSegList.end() && long_crossProduct((*optimalSingle)->last, itSeg->first, itSeg->last) > 0) {
    nextPointParam = pInstOptimal.par[1];
    return *optimalSingle;
  }
  //没有共线交线段
  if (optimalColinear == mayIntersectSegList.end()) {
    //没有单交点
    if (optimalSingle == mayIntersectSegList.end()) {
      return mTraceList.end();
    }
    else { //返回单交点
      nextPointParam = pInstOptimal.par[1];
      return *optimalSingle;
    }
  }
  else {
    //判断共线交线段上的交线段
    LongSegment2CIterList newTabuList(tabuList);
    newTabuList.insert(newTabuList.end(), hasIntersectSegList.begin(), hasIntersectSegList.end());
    //新的当前线段为共线交线段,参数是当前线段的终点在共线线段上的参数. 因为两者相交且共线段更长,此参数必有效
    Int64Fraction f;
    getLongPointParamInSegment(*optimalColinear, itSeg->last, f);
    return searchNextSegment(*optimalColinear, f, newTabuList, nextPointParam);
  }
}
}
#endif
//文件 NfpPolygon.h 对多边形边链进行分类处理，加快多边形与点之间水平 / 垂直距离求取速度，对于由有很多曲线离散而来的多边形，效果尤为明显。
///// FileName: NfpPolygon.h
///// Author: youlin.wang
///// Date: 2008-12-12
#ifndef __NFPPOLYGON_H__
#define __NFPPOLYGON_H__
#include <set>
#include <functional>
#include <limits>
#include <algorithm>
#include "basicStruct.h"
#include "GeoBasic.h"
#include "Rect2.h"
typedef std::vector<size_t> IndexList;
typedef std::vector<IndexList> IndexListList;
//比较浮点数
inline int compareDouble(double d1, double d2)
{
  if (d1 > d2 + 1e-6) {
    return 1;
  }
  else if (d1 < d2 - 1e-6) {
    return -1;
  }
  return 0;
}
struct FunObjBottomLeftPoint : public std::binary_function<const Point2&, const Point2&, bool> {
  bool operator()(const Point2& pa, const Point2& pb) const {
    return pa.y() < pb.y() || (fabs(pa.y() - pb.y()) < DoubleZero && pa.x() < pb.x());
  }
};
struct FunObjLeftBottomPoint : public std::binary_function<const Point2&, const Point2&, bool> {
  bool operator()(const Point2& pa, const Point2& pb) const {
    return pa.x() < pb.x() || (fabs(pa.x() - pb.x()) < DoubleZero && pa.y() < pb.y());
  }
};
template<class _Point>
inline _Point vertexOfList(const std::vector<_Point>& iPointList, int i)
{
  if (i == -1) {
    return iPointList.back();
  }
  if (i == iPointList.size()) {
    return iPointList.front();
  }
  return iPointList[i];
}
//左单调链边 IndexListList _MonotoneEdgeLeft;
//右单调链边 IndexListList _MonotoneEdgeRight;
//左单调链边凸点 IndexList _ConvexLeft;
//右单调链边凸点 IndexList _ConvexRight;
template<class _Point>
inline void compute_monoX(const std::vector<_Point>& oriPointList, IndexListList& _MonotoneEdgeLeft
    , IndexListList& _MonotoneEdgeRight
    , IndexList& _ConvexLeft
    , IndexList& _ConvexRight)
{
  std::vector<_Point>::const_iterator itBL = std::min_element(oriPointList.begin(), oriPointList.end(), FunObjBottomLeftPoint());
  std::vector<_Point> iPointList;
  iPointList.insert(iPointList.begin(), itBL, oriPointList.end());
  iPointList.insert(iPointList.end(), oriPointList.begin(), itBL);
  size_t _OriginIndex = itBL - oriPointList.begin();
  //计算单调边和单调边凸点
  //简化计算左单调边链列表
  int preUpStopDown = -1;
  int curLeft = 0;
  _MonotoneEdgeLeft.push_back(IndexList());
  int curRight = -1;
  for (size_t i = 0; i < iPointList.size(); ++i) {
    //右边链
    int currentUpStopDown = compareDouble(vertexOfList(iPointList, i + 1).y(), vertexOfList(iPointList, i).y());
    if (currentUpStopDown == 1) { //当前是升序则该点总是要被加入
      if (preUpStopDown < 1) { //前面降或者平，意味着新链开始
        curRight++;
        _MonotoneEdgeRight.push_back(IndexList());
      }
      _MonotoneEdgeRight[curRight].push_back(i);
    }
    else if (preUpStopDown == 1) { //当前 平序或者降序 并且前面升序，则此点是最后一点
      _MonotoneEdgeRight[curRight].push_back(i);
    }
    //左边链
    if (currentUpStopDown == -1) { //当前链是降序则该点总是被加入
      if (preUpStopDown > -1) { //前面升或者平，意味着新链开始
        curLeft++;
        _MonotoneEdgeLeft.push_back(IndexList());
      }
      _MonotoneEdgeLeft[curLeft].push_back(i);
    }
    else if (preUpStopDown == -1) { //当前升序或者平序而前面降序，意味着该链结束
      _MonotoneEdgeLeft[curLeft].push_back(i);
    }
    preUpStopDown = currentUpStopDown;
  }
  //把左边链第一个放在最后一条链的后面
  size_t temp = _MonotoneEdgeLeft[0].front();
  _MonotoneEdgeLeft.erase(_MonotoneEdgeLeft.begin());
  _MonotoneEdgeLeft.back().push_back(temp);
  //计算左右凸点
  for (size_t i = 0; i < _MonotoneEdgeLeft.size(); ++i) {
    for (size_t j = 0; j < _MonotoneEdgeLeft[i].size(); ++j) {
      size_t id = _MonotoneEdgeLeft[i][j];
      if (crossProduct(vertexOfList(iPointList, id) - vertexOfList(iPointList, id - 1), vertexOfList(iPointList, id + 1) - vertexOfList(iPointList, id)) > 0) {
        _ConvexLeft.push_back((id + _OriginIndex) % iPointList.size());
      }
    }
  }
  for (size_t i = 0; i < _MonotoneEdgeRight.size(); ++i) {
    for (size_t j = 0; j < _MonotoneEdgeRight[i].size(); ++j) {
      size_t id = _MonotoneEdgeRight[i][j];
      if (crossProduct(vertexOfList(iPointList, id) - vertexOfList(iPointList, id - 1), vertexOfList(iPointList, id + 1) - vertexOfList(iPointList, id)) > 0) {
        _ConvexRight.push_back((id + _OriginIndex) % iPointList.size());
      }
    }
  }
  for (size_t i = 0; i < _MonotoneEdgeLeft.size(); ++i) {
    for (size_t j = 0; j < _MonotoneEdgeLeft[i].size(); ++j) {
      _MonotoneEdgeLeft[i][j] = (_MonotoneEdgeLeft[i][j] + _OriginIndex) % iPointList.size();
    }
  }
  for (size_t i = 0; i < _MonotoneEdgeRight.size(); ++i) {
    for (size_t j = 0; j < _MonotoneEdgeRight[i].size(); ++j) {
      _MonotoneEdgeRight[i][j] = (_MonotoneEdgeRight[i][j] + _OriginIndex) % iPointList.size();
    }
  }
}
//下单调链边 IndexListList _MonotoneEdgeBottom;
//上单调链边 IndexListList _MonotoneEdgeTop;
//下单调链边凸点 IndexList _ConvexBottom;
//上单调链边凸点 IndexList _ConvexTop;
template<class _Point>
inline void compute_monoY(const std::vector<_Point>& oriPointList, IndexListList& _MonotoneEdgeBottom
    , IndexListList& _MonotoneEdgeTop
    , IndexList& _ConvexBottom
    , IndexList& _ConvexTop)
{
  std::vector<_Point>::const_iterator itLB = std::min_element(oriPointList.begin(), oriPointList.end(), FunObjLeftBottomPoint());
  std::vector<_Point> iPointList;
  iPointList.insert(iPointList.begin(), itLB, oriPointList.end());
  iPointList.insert(iPointList.end(), oriPointList.begin(), itLB);
  size_t _OriginIndex = itLB - oriPointList.begin();
  //计算单调边和单调边凸点
  //简化计算下单调边链列表
  int preRightStopLeft = -1;
  int curUp = 0;
  _MonotoneEdgeTop.push_back(IndexList());
  int curDown = -1;
  for (size_t i = 0; i < iPointList.size(); ++i) {
    //下边链
    int currentRightStopLeft = compareDouble(vertexOfList(iPointList, i + 1).x(), vertexOfList(iPointList, i).x());
    if (currentRightStopLeft == 1) { //当前是升序则该点总是要被加入
      if (preRightStopLeft < 1) { //前面降或者平，意味着新链开始
        curDown++;
        _MonotoneEdgeBottom.push_back(IndexList());
      }
      _MonotoneEdgeBottom[curDown].push_back(i);
    }
    else if (preRightStopLeft == 1) { //当前 平序或者降序 并且前面升序，则此点是最后一点
      _MonotoneEdgeBottom[curDown].push_back(i);
    }
    //上边链
    if (currentRightStopLeft == -1) { //当前链是降序则该点总是被加入
      if (preRightStopLeft > -1) { //前面升或者平，意味着新链开始
        curUp++;
        _MonotoneEdgeTop.push_back(IndexList());
      }
      _MonotoneEdgeTop[curUp].push_back(i);
    }
    else if (preRightStopLeft == -1) { //当前升序或者平序而前面降序，意味着该链结束
      _MonotoneEdgeTop[curUp].push_back(i);
    }
    preRightStopLeft = currentRightStopLeft;
  }
  //把下边链第一个放在最后一条链的后面
  size_t temp = _MonotoneEdgeTop[0].front();
  _MonotoneEdgeTop.erase(_MonotoneEdgeTop.begin());
  _MonotoneEdgeTop.back().push_back(temp);
  //计算左右凸点
  for (size_t i = 0; i < _MonotoneEdgeBottom.size(); ++i) {
    for (size_t j = 0; j < _MonotoneEdgeBottom[i].size(); ++j) {
      size_t id = _MonotoneEdgeBottom[i][j];
      if (crossProduct(vertexOfList(iPointList, id) - vertexOfList(iPointList, id - 1), vertexOfList(iPointList, id + 1) - vertexOfList(iPointList, id)) > 0) {
        _ConvexBottom.push_back((id + _OriginIndex) % iPointList.size());
      }
    }
  }
  for (size_t i = 0; i < _MonotoneEdgeTop.size(); ++i) {
    for (size_t j = 0; j < _MonotoneEdgeTop[i].size(); ++j) {
      size_t id = _MonotoneEdgeTop[i][j];
      if (crossProduct(vertexOfList(iPointList, id) - vertexOfList(iPointList, id - 1), vertexOfList(iPointList, id + 1) - vertexOfList(iPointList, id)) > 0) {
        _ConvexTop.push_back((id + _OriginIndex) % iPointList.size());
      }
    }
  }
  for (size_t i = 0; i < _MonotoneEdgeBottom.size(); ++i) {
    for (size_t j = 0; j < _MonotoneEdgeBottom[i].size(); ++j) {
      _MonotoneEdgeBottom[i][j] = (_MonotoneEdgeBottom[i][j] + _OriginIndex) % iPointList.size();
    }
  }
  for (size_t i = 0; i < _MonotoneEdgeTop.size(); ++i) {
    for (size_t j = 0; j < _MonotoneEdgeTop[i].size(); ++j) {
      _MonotoneEdgeTop[i][j] = (_MonotoneEdgeTop[i][j] + _OriginIndex) % iPointList.size();
    }
  }
}
//// Function:
//// inline size_t countRight_binary_search(const Point2List& ptList,const IndexListList& rightlist,const Point2& p)
//// inline size_t countLeft_binary_search(const Point2List& ptList,const IndexListList& leftlist,const Point2& p)
//// inline size_t countTop_binary_search(const Point2List& ptList,const IndexListList& toplist,const Point2& p)
//// inline size_t countBottom_binary_search(const Point2List& ptList,const IndexListList& bottomlist,const Point2& p)
//// specification:: count_XXX to count the ray intersect with the mono-chain-edge.
//// here use the binary-search.actually it can not help a lot here,for the comparison-operation here is so cheap
//// that only when the vector is very big it can get obvious effect.
//// distanceList_XXX to get the distance list of ray intersecting with the mono-chain-edge.all same to count-function just get the distance list
inline size_t countRight_binary_search(const Point2List& ptList, const IndexListList& rightlist, const Point2& p)
{
  //Y递增
  size_t counts = 0;
  for (size_t i = 0; i < rightlist.size(); ++i) {
    size_t high = rightlist[i].size() - 1, low = 0, mid;
    if (p.y() >= ptList[rightlist[i].front()].y() && p.y() < ptList[rightlist[i].back()].y()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[rightlist[i][mid]].y() > p.y()) {
          high = mid;
        }
        else {
          low = mid;
        }
      }
      Point2 pa = ptList[rightlist[i][low]];
      Point2 pb = ptList[rightlist[i][high]];
      if ((p.x() - pa.x()) * (pb.y() - pa.y()) > (pb.x() - pa.x()) * (p.y() - pa.y())) {
        counts++;
      }
    }
  }
  return counts;
}
inline void distanceListRight_binary_search(const Point2List& ptList, const IndexListList& rightlist, const Point2& p, std::vector<double>& pointList)
{
  //Y递增
  for (size_t i = 0; i < rightlist.size(); ++i) {
    size_t high = rightlist[i].size() - 1, low = 0, mid;
    if (p.y() >= ptList[rightlist[i].front()].y() && p.y() < ptList[rightlist[i].back()].y()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[rightlist[i][mid]].y() > p.y()) {
          high = mid;
        }
        else {
          low = mid;
        }
      }
      const Point2& pa = ptList[rightlist[i][low]];
      const Point2& pb = ptList[rightlist[i][high]];
      pointList.push_back((pb.x() - pa.x()) * (p.y() - pa.y()) / (pb.y() - pa.y()) + pa.x() - p.x());
    }
  }
}
inline size_t countLeft_binary_search(const Point2List& ptList, const IndexListList& leftlist, const Point2& p)
{
  //Y递减
  size_t counts = 0;
  for (size_t i = 0; i < leftlist.size(); ++i) {
    size_t high = leftlist[i].size() - 1, low = 0, mid;
    if (p.y() >= ptList[leftlist[i].back()].y() && p.y() < ptList[leftlist[i].front()].y()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[leftlist[i][mid]].y() > p.y()) {
          low = mid;
        }
        else {
          high = mid;
        }
      }
      const Point2& pa = ptList[leftlist[i][low]];
      const Point2& pb = ptList[leftlist[i][high]];
      if ((p.x() - pa.x()) * (pb.y() - pa.y()) < (pb.x() - pa.x()) * (p.y() - pa.y())) {
        counts++;
      }
    }
  }
  return counts;
}
inline void distanceListLeft_binary_search(const Point2List& ptList, const IndexListList& leftlist, const Point2& p, std::vector<double>& disList)
{
  //Y递减
  for (size_t i = 0; i < leftlist.size(); ++i) {
    size_t high = leftlist[i].size() - 1, low = 0, mid;
    if (p.y() >= ptList[leftlist[i].back()].y() && p.y() < ptList[leftlist[i].front()].y()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[leftlist[i][mid]].y() > p.y()) {
          low = mid;
        }
        else {
          high = mid;
        }
      }
      const Point2& pa = ptList[leftlist[i][low]];
      const Point2& pb = ptList[leftlist[i][high]];
      disList.push_back((pb.x() - pa.x()) * (p.y() - pa.y()) / (pb.y() - pa.y()) + pa.x() - p.x());
    }
  }
}
inline size_t countBottom_binary_search(const Point2List& ptList, const IndexListList& bottomlist, const Point2& p)
{
  //X递增
  size_t counts = 0;
  for (size_t i = 0; i < bottomlist.size(); ++i) {
    size_t high = bottomlist[i].size() - 1, low = 0, mid;
    if (p.x() >= ptList[bottomlist[i].front()].x() && p.x() < ptList[bottomlist[i].back()].x()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[bottomlist[i][mid]].x() > p.x()) {
          high = mid;
        }
        else {
          low = mid;
        }
      }
      const Point2& pa = ptList[bottomlist[i][low]];
      const Point2& pb = ptList[bottomlist[i][high]];
      if ((p.y() - pa.y()) * (pb.x() - pa.x()) < (pb.y() - pa.y()) * (p.x() - pa.x())) {
        counts++;
      }
    }
  }
  return counts;
}
inline void distanceListBottom_binary_search(const Point2List& ptList, const IndexListList& bottomlist, const Point2& p, std::vector<double>& disList)
{
  //X递增
  for (size_t i = 0; i < bottomlist.size(); ++i) {
    size_t high = bottomlist[i].size() - 1, low = 0, mid;
    if (p.x() >= ptList[bottomlist[i].front()].x() && p.x() < ptList[bottomlist[i].back()].x()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[bottomlist[i][mid]].x() > p.x()) {
          high = mid;
        }
        else {
          low = mid;
        }
      }
      const Point2& pa = ptList[bottomlist[i][low]];
      const Point2& pb = ptList[bottomlist[i][high]];
      disList.push_back((pb.y() - pa.y()) * (p.x() - pa.x()) / (pb.x() - pa.x()) + pa.y() - p.y());
    }
  }
}
inline size_t countTop_binary_search(const Point2List& ptList, const IndexListList& toplist, const Point2& p)
{
  //X递减
  size_t counts = 0;
  for (size_t i = 0; i < toplist.size(); ++i) {
    size_t high = toplist[i].size() - 1, low = 0, mid;
    if (p.x() >= ptList[toplist[i].back()].x() && p.x() < ptList[toplist[i].front()].x()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[toplist[i][mid]].x() > p.x()) {
          low = mid;
        }
        else {
          high = mid;
        }
      }
      const Point2& pa = ptList[toplist[i][low]];
      const Point2& pb = ptList[toplist[i][high]];
      if ((p.y() - pa.y()) * (pb.x() - pa.x()) > (pb.y() - pa.y()) * (p.x() - pa.x())) {
        counts++;
      }
    }
  }
  return counts;
}
inline void distanceListTop_binary_search(const Point2List& ptList, const IndexListList& toplist, const Point2& p, std::vector<double>& disList)
{
  //X递减
  for (size_t i = 0; i < toplist.size(); ++i) {
    size_t high = toplist[i].size() - 1, low = 0, mid;
    if (p.x() >= ptList[toplist[i].back()].x() && p.x() < ptList[toplist[i].front()].x()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[toplist[i][mid]].x() > p.x()) {
          low = mid;
        }
        else {
          high = mid;
        }
      }
      const Point2& pa = ptList[toplist[i][low]];
      const Point2& pb = ptList[toplist[i][high]];
      disList.push_back((pb.y() - pa.y()) * (p.x() - pa.x()) / (pb.x() - pa.x()) + pa.y() - p.y());
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////
/////Get the min-Distance to Right/Left/Top/Bottom boundary of Polygon
/////
//the distance from a point inside polygon to the nearest right boundary of polygon
inline double disInteriorPointToRightBorder(const Point2List& ptList, const IndexListList& rightlist, const Point2& p)
{
  //Y递增
  double disMin = (std::numeric_limits<double>::max)();
  for (size_t i = 0; i < rightlist.size(); ++i) {
    size_t high = rightlist[i].size() - 1, low = 0, mid;
    if (p.y() >= ptList[rightlist[i].front()].y() && p.y() <= ptList[rightlist[i].back()].y()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[rightlist[i][mid]].y() > p.y()) {
          high = mid;
        }
        else {
          low = mid;
        }
      }
      const Point2& iPoint1 = ptList[rightlist[i][low]];
      const Point2& iPoint2 = ptList[rightlist[i][high]];
      double tempDis = (iPoint1.x() - iPoint2.x()) * (p.y() - iPoint2.y()) / (iPoint1.y() - iPoint2.y()) + iPoint2.x() - p.x();
      disMin = (std::min)(disMin, fabs(tempDis));
    }
  }
  return disMin;
}
//the distance from a point inside polygon to the nearest left boundary of polygon
inline double disInteriorPointToLeftBorder(const Point2List& ptList, const IndexListList& leftlist, const Point2& p)
{
  //Y递减
  double disMin = (std::numeric_limits<double>::max)();
  for (size_t i = 0; i < leftlist.size(); ++i) {
    size_t high = leftlist[i].size() - 1, low = 0, mid;
    if (p.y() >= ptList[leftlist[i].back()].y() && p.y() <= ptList[leftlist[i].front()].y()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[leftlist[i][mid]].y() > p.y()) {
          low = mid;
        }
        else {
          high = mid;
        }
      }
      const Point2& iPoint1 = ptList[leftlist[i][low]];
      const Point2& iPoint2 = ptList[leftlist[i][high]];
      double tempDis = (iPoint1.x() - iPoint2.x()) * (p.y() - iPoint2.y()) / (iPoint1.y() - iPoint2.y()) + iPoint2.x() - p.x();
      disMin = (std::min)(disMin, fabs(tempDis));
    }
  }
  return disMin;
}
//the distance from a point inside polygon to the nearest top boundary of polygon
inline double disInteriorPointToTopBorder(const Point2List& ptList, const IndexListList& toplist, const Point2& p)
{
  //x递减
  double disMin = (std::numeric_limits<double>::max)();
  for (size_t i = 0; i < toplist.size(); ++i) {
    size_t high = toplist[i].size() - 1, low = 0, mid;
    if (p.x() >= ptList[toplist[i].back()].x() && p.x() <= ptList[toplist[i].front()].x()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[toplist[i][mid]].x() > p.x()) {
          low = mid;
        }
        else {
          high = mid;
        }
      }
      const Point2& iPoint1 = ptList[toplist[i][low]];
      const Point2& iPoint2 = ptList[toplist[i][high]];
      double tempDis = (iPoint1.y() - iPoint2.y()) * (p.x() - iPoint2.x()) / (iPoint1.x() - iPoint2.x()) + iPoint2.y() - p.y();
      disMin = (std::min)(disMin, fabs(tempDis));
    }
  }
  return disMin;
}
//the distance from a point inside polygon to the nearest bottom boundary of polygon
inline double disInteriorPointToBottomBorder(const Point2List& ptList, const IndexListList& bottomlist, const Point2& p)
{
  //x递增
  double disMin = (std::numeric_limits<double>::max)();
  for (size_t i = 0; i < bottomlist.size(); ++i) {
    size_t high = bottomlist[i].size() - 1, low = 0, mid;
    if (p.x() >= ptList[bottomlist[i].front()].x() && p.x() <= ptList[bottomlist[i].back()].x()) {
      while (high - low != 1) {
        mid = (low + high) >> 1;
        if (ptList[bottomlist[i][mid]].x() > p.x()) {
          high = mid;
        }
        else {
          low = mid;
        }
      }
      const Point2& iPoint1 = ptList[bottomlist[i][low]];
      const Point2& iPoint2 = ptList[bottomlist[i][high]];
      double tempDis = (iPoint1.y() - iPoint2.y()) * (p.x() - iPoint2.x()) / (iPoint1.x() - iPoint2.x()) + iPoint2.y() - p.y();
      disMin = (std::min)(disMin, fabs(tempDis));
    }
  }
  return disMin;
}
namespace Nfp
{
//// class :
//// NfpPolygon
//// specification: This polygon is a trace polygon of a reference point that polygon B convolute a fixed polygon.
//// Note: It never transform its' real data when translate ,the transformation is keep in member "mTranslation".
//// Also there use mono-chain to make the point-polygon operation faster .
//// Function: this algorithms will make the polygon's intersection-computing and distance-computing very fast,it should be pre-computed.
//// and using the relative pos between the referent point and the no-fit polygon to determine whether two polygons intersects
//// (whether the referent point in the no-fit polygon) and distance of two polygon (distance from the reference point to no-fit polygon)
//// for example: two polygon both have 20 edges,it will test 20*20 times intersection of edges ,even using some bounding test to accelerate,
//// it will test 5*10 times averagely. if using nfp algorithms,we only need to test whether the point in the polygon ,and with mono-edge
//// technique together,it only need 1-2 times intersection test and several double-value comparison. but It should be pointed out that when
//// polygon have less edges ,for example:5-10 edges, it can not improve so much .
class NfpPolygon
{
  NfpPolygon()
    : mTranslation() {
  }
public:
  template<class _FwdIt>
  NfpPolygon(_FwdIt beginIt, _FwdIt endIt)
    : mTranslation() {
    //寻找左底点索引
    _FwdIt itBL = std::min_element(beginIt, endIt, FunObjBottomLeftPoint());
    mPointList.insert(mPointList.begin(), itBL, endIt);
    mPointList.insert(mPointList.end(), beginIt, itBL);
    mBoundingRect = Rect2(mPointList);
    computeMono();
  }
  //旋转180
  NfpPolygon* createInverse() {
    NfpPolygon* nfpI = new NfpPolygon();
    nfpI->mPointList = mPointList;
    nfpI->mBoundingRect = mBoundingRect;
    rotate180Point2List<Point2>(nfpI->mPointList, 0, 0);
    nfpI->computeMono();
    return nfpI;
  }
  ~NfpPolygon(void) {
  }
  void centerMoveTo(const Point2& cen) {
    Point2 offset = cen - mTranslation;
    translate(offset);
  }
  void translate(const Point2& offset) {
    mTranslation += offset;
    mBoundingRect.translate(offset);
  }
  const Rect2& boundingRect() const {
    return mBoundingRect;
  }
  bool checkInside(const Point2& p) const {
    if (!mBoundingRect.checkInside(p)) {
      return false;
    }
    if (mCompareDir) {
      return countLeft_binary_search(mPointList, mMonotoneEdgeLeft, p) != countRight_binary_search(mPointList, mMonotoneEdgeRight, p);
    }
    else {
      return countTop_binary_search(mPointList, mMonotoneEdgeTop, p) != countBottom_binary_search(mPointList, mMonotoneEdgeBottom, p);
    }
  }
  //the distance to compact the point form right to left
  double disCompactToLeftBoundary(const Point2& p) const {
    //only compact to the right-boundary of polygon
    //just get the max-negative distance to right
    double distance = (std::numeric_limits<double>::min)();
    for (size_t i = 0; i < mMonotoneEdgeRight.size(); ++i) {
      size_t high = mMonotoneEdgeRight[i].size() - 1, low = 0, mid;
      if (p.y() >= mPointList[mMonotoneEdgeRight[i].front()].y() && p.y() < mPointList[mMonotoneEdgeRight[i].back()].y()) {
        while (high - low != 1) {
          mid = (low + high) >> 1;
          if (mPointList[mMonotoneEdgeRight[i][mid]].y() > p.y()) {
            high = mid;
          }
          else {
            low = mid;
          }
        }
        const Point2& pa = mPointList[mMonotoneEdgeRight[i][low]];
        const Point2& pb = mPointList[mMonotoneEdgeRight[i][high]];
        double tempDis = (pb.x() - pa.x()) * (p.y() - pa.y()) / (pb.y() - pa.y()) + pa.x() - p.x();
        if (tempDis < 0 && tempDis > distance) {
          distance = tempDis;
        }
      }
    }
    return distance + 0.001;
  }
  //the distance to compact the point from top to bottom
  double disCompactToBottomBoundary(const Point2& p) const {
    double distance = (std::numeric_limits<double>::min)();
    for (size_t i = 0; i < mMonotoneEdgeTop.size(); ++i) {
      size_t high = mMonotoneEdgeTop[i].size() - 1, low = 0, mid;
      if (p.x() >= mPointList[mMonotoneEdgeTop[i].back()].x() && p.x() < mPointList[mMonotoneEdgeTop[i].front()].x()) {
        while (high - low != 1) {
          mid = (low + high) >> 1;
          if (mPointList[mMonotoneEdgeTop[i][mid]].x() > p.x()) {
            low = mid;
          }
          else {
            high = mid;
          }
        }
        const Point2& pa = mPointList[mMonotoneEdgeTop[i][low]];
        const Point2& pb = mPointList[mMonotoneEdgeTop[i][high]];
        double tempDis = (pb.y() - pa.y()) * (p.x() - pa.x()) / (pb.x() - pa.x()) + pa.y() - p.y();
        if (tempDis < 0 && tempDis > distance) {
          distance = tempDis;
        }
      }
    }
    return distance + 0.001;
  }
  double minDisInteriorToBorder(const Point2& p) {
    double l = disInteriorPointToLeftBorder(mPointList, mMonotoneEdgeLeft, p);
    double r = disInteriorPointToRightBorder(mPointList, mMonotoneEdgeRight, p);
    double t = disInteriorPointToTopBorder(mPointList, mMonotoneEdgeTop, p);
    double b = disInteriorPointToBottomBorder(mPointList, mMonotoneEdgeBottom, p);
    double hor = (l > r ? r : l);
    double ver = (t > b ? b : t);
    return ver > hor ? hor : ver;
  }
  double overlapArea(const Point2& p, double f = 0.2) {
    double l = disInteriorPointToLeftBorder(mPointList, mMonotoneEdgeLeft, p);
    double r = disInteriorPointToRightBorder(mPointList, mMonotoneEdgeRight, p);
    double t = disInteriorPointToTopBorder(mPointList, mMonotoneEdgeTop, p);
    double b = disInteriorPointToBottomBorder(mPointList, mMonotoneEdgeBottom, p);
    double hor = (l > r ? r : l);
    double ver = (t > b ? b : t);
    return ver * hor * (f * hor + 1.0);
  }
private:
  void computeMono() {
    mMonotoneEdgeBottom.clear();
    mMonotoneEdgeLeft.clear();
    mMonotoneEdgeRight.clear();
    mMonotoneEdgeTop.clear();
    mConvexBottom.clear();
    mConvexLeft.clear();
    mConvexRight.clear();
    mConvexTop.clear();
    compute_monoX(mPointList, mMonotoneEdgeLeft, mMonotoneEdgeRight, mConvexLeft, mConvexRight);
    compute_monoY(mPointList, mMonotoneEdgeBottom, mMonotoneEdgeTop, mConvexBottom, mConvexTop);
    size_t ih = 0, iv = 0;
    for (size_t i = 0; i < mMonotoneEdgeLeft.size(); ++i) {
      ih += mMonotoneEdgeLeft[i].size();
    }
    for (size_t i = 0; i < mMonotoneEdgeRight.size(); ++i) {
      ih += mMonotoneEdgeRight[i].size();
    }
    for (size_t i = 0; i < mMonotoneEdgeTop.size(); ++i) {
      iv += mMonotoneEdgeTop[i].size();
    }
    for (size_t i = 0; i < mMonotoneEdgeBottom.size(); ++i) {
      iv += mMonotoneEdgeBottom[i].size();
    }
    if (ih > iv) {
      mCompareDir = false;
    }
    else {
      mCompareDir = true;
    }
  }
  Point2List mPointList;
  Point2 mTranslation;
  Rect2 mBoundingRect;
  bool mCompareDir;
  ///////////////////////////
  //左单调链边
  IndexListList mMonotoneEdgeLeft;
  //右单调链边
  IndexListList mMonotoneEdgeRight;
  //左单调链边凸点
  IndexList mConvexLeft;
  //右单调链边凸点
  IndexList mConvexRight;
  //下单调链边
  IndexListList mMonotoneEdgeBottom;
  //上单调链边
  IndexListList mMonotoneEdgeTop;
  //下单调链边凸点
  IndexList mConvexBottom;
  //上单调链边凸点
  IndexList mConvexTop;
};
}
#endif
//文件 NfpManager.h 它只是个管理多边形NFP的东西，你可以按照自己需要来写。
///// FileName: NfpManager.h
///// Author: youlin.wang
///// Date: 2008-11-12
#ifndef __NESTPOLYGON_ALGORITHMS_H__
#define __NESTPOLYGON_ALGORITHMS_H__
#include "NfpBasic.h"
#include "NfpPolygon.h"
struct DestroyObject {
  template<typename T> void operator()(const T* ioPointer) const {
    delete ioPointer;
  }
};
namespace Nfp
{
class NfpPiece
{
  NfpPiece() {
  }
public:
  template<class _Piece>
  NfpPiece(const _Piece* pConst, const _Piece* pMobile)
    : mCountsConst(pConst->transformSize())
    , mCountsMobile(pMobile->transformSize()) {
    for (PointListCIter itC = pConst->polygonPointBeginIt(); itC != pConst->polygonPointEndIt(); ++itC) {
      for (PointListCIter itM = pMobile->polygonPointBeginIt(); itM != pMobile->polygonPointEndIt(); ++itM) {
        NfpCalculator<Point2> nfp(*itC, *itM);
        std::vector<Point2> points;
        nfp.computeNoFitPolygon(points);
        mNfpList.push_back(new NfpPolygon(points.begin(), points.end()));
      }
    }
  }
  NfpPolygon* polygonNfp(size_t transformIdC, size_t transformIdM) {
    return mNfpList[transformIdC * mCountsConst + transformIdM];
  }
  NfpPiece* createInverse() {
    NfpPiece* Infp = new NfpPiece();
    Infp->mCountsConst = mCountsMobile;
    Infp->mCountsMobile = mCountsConst;
    //注意顺序
    Infp->mNfpList.reserve(mCountsConst * mCountsMobile);
    for (size_t i = 0; i < mCountsMobile; ++i) {
      for (size_t j = 0; j < mCountsConst; ++j) {
        Infp->mNfpList.push_back(mNfpList[mCountsMobile * j + i]->createInverse());
      }
    }
    return Infp;
  }
  ~NfpPiece() {
    std::for_each(mNfpList.begin(), mNfpList.end(), DestroyObject());
  }
private:
  size_t mCountsConst;
  size_t mCountsMobile;
  std::vector<NfpPolygon* > mNfpList;
};
class FastPiece
{
public:
  template<class _Piece>
  FastPiece(const _Piece* p) {
    for (PointListCIter itC = p->polygonPointBeginIt(); itC != p->polygonPointEndIt(); ++itC) {
      mFastPolygonList.push_back(new NfpPolygon((*itC).begin(), (*itC).end()));
    }
  }
  NfpPolygon* fastPolygon(size_t transformIdC) const {
    return mFastPolygonList[transformIdC];
  }
  ~FastPiece() {
    std::for_each(mFastPolygonList.begin(), mFastPolygonList.end(), DestroyObject());
  }
private:
  std::vector<NfpPolygon* > mFastPolygonList;
};
class FastPieceManager
{
public:
  template<class _Piece>
  FastPieceManager(std::vector<_Piece*>& piecelist) {
    for (size_t i = 0; i < piecelist.size(); ++i) {
      mFastPieceList.push_back(new FastPiece(piecelist[i]));
    }
  }
  ~FastPieceManager(void) {
    std::for_each(mFastPieceList.begin(), mFastPieceList.end(), DestroyObject());
  }
  template<class VElement>
  NfpPolygon* fastPiece(const VElement& ve) const {
    return mFastPieceList[ve.typeID()]->fastPolygon(ve.transformID());
  }
private:
  std::vector<FastPiece* > mFastPieceList;
};
class NfpManager
{
public:
  template<class _Piece>
  NfpManager(std::vector<_Piece*>& piecelist) {
    mCount = piecelist.size();
    mNfpPieceList.reserve(mCount * mCount);
    for (size_t i = 0; i < mCount; ++i) {
      for (size_t j = 0; j < mCount; ++j) {
        if (j < i) { //copy and transform
          mNfpPieceList.push_back(mNfpPieceList[piecelist[j]->typeID()*mCount + piecelist[i]->typeID()]->createInverse());
        }
        else {
          mNfpPieceList.push_back(new NfpPiece(piecelist[i], piecelist[j]));
        }
      }
    }
  }
  ~NfpManager(void) {
    std::for_each(mNfpPieceList.begin(), mNfpPieceList.end(), DestroyObject());
  }
  NfpPolygon* polygonNfp(size_t typeIdC, size_t transformIdC, size_t typeIdM, size_t transformIdM) const {
    return mNfpPieceList[typeIdC * mCount + typeIdM]->polygonNfp(transformIdC, transformIdM);
  }
  template<class VElement>
  NfpPolygon* polygonNfp(const VElement& veC, const VElement& veM) const {
    return mNfpPieceList[veC.typeID() * mCount + veM.typeID()]->polygonNfp(veC.transformID(), veM.transformID());
  }
  // centerDiff=pM->center()-pC->center
  /* double distanceX(size_t typeIdC,size_t transformIdC,size_t typeIdM,size_t transformIdM,const Point2& centerDiff) const
  {
  return polygonNfp(typeIdC,transformIdC,typeIdM,transformIdM)->disCompactToLeftBoundary(centerDiff);
  }*/
  template<class VElement>
  double distanceX(const VElement& veC, const VElement& veM) const {
    return polygonNfp(veC, veM)->disCompactToLeftBoundary(veM.center() - veC.center());
  }
  template<class VElement>
  double distanceY(const VElement& veC, const VElement& veM) const {
    return polygonNfp(veC, veM)->disCompactToBottomBoundary(veM.center() - veC.center());
  }
  /* double distanceY(size_t typeIdC,size_t transformIdC,size_t typeIdM,size_t transformIdM,const Point2& centerDiff) const
  {
  return polygonNfp(typeIdC,transformIdC,typeIdM,transformIdM)->disCompactToBottomBoundary(centerDiff);
  }*/
  bool isOverlap(size_t typeIdC, size_t transformIdC, size_t typeIdM, size_t transformIdM, const Point2& centerDiff) const {
    return polygonNfp(typeIdC, transformIdC, typeIdM, transformIdM)->checkInside(centerDiff);
  }
  double inspersedDepth(size_t typeIdC, size_t transformIdC, size_t typeIdM, size_t transformIdM, const Point2& centerDiff) const {
    return polygonNfp(typeIdC, transformIdC, typeIdM, transformIdM)->minDisInteriorToBorder(centerDiff);
  }
  double overlapArea(size_t typeIdC, size_t transformIdC, size_t typeIdM, size_t transformIdM, const Point2& centerDiff) const {
    return polygonNfp(typeIdC, transformIdC, typeIdM, transformIdM)->overlapArea(centerDiff);
  }
  void getHorDistanceList(size_t typeIdC, size_t transformIdC, size_t typeIdM, size_t transformIdM, const Point2& centerDiff, std::set<double>& disList) const {
    //polygonNfp(pA,pB)->getHorDistanceList(pB->center()-pA->center(),disList);
  }
  void getVerDistanceList(size_t typeIdC, size_t transformIdC, size_t typeIdM, size_t transformIdM, const Point2& centerDiff, std::set<double>& disList) const {
    //polygonNfp(pA,pB)->getVerDistanceList(pB->center()-pA->center(),disList);
  }
private:
  unsigned mCount;
  std::vector<NfpPiece* > mNfpPieceList;
};
}
#endif
测试代码:
回复 jjqcat：
假设你的点类是XPoint, 你可以根据编译提示修正你的点类.
vector<XPoint> slist, mlist, oList;
...slist和mlist填充多边形顶点数据, 须保证不自交和无连续三点共线的顶点.
NfpCalculator<XPoint> nc(slist, mlist);
nc.computeNoFitPolygon(oList);
这是很久以前写的, 一直没去维护. 可能在某些边界条件下有bug.

