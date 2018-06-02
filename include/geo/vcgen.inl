
#ifndef _VCGEN_INL_
#define _VCGEN_INL_
#include "m23.inl"
#include "geo.inl"

#define vcgen_line_to(_x, _y)        if (Points) { Points[ipos].x = _x; Points[ipos].y = _y;} ++ipos
#define vcgen_addpoint(_x, _y, _t)   if (Points) { Points[ipos].x = _x; Points[ipos].y = _y; Types[ipos] = _t; } ++ipos
#define move_to(_x, _y, rel)         { if (rel) {pre_x += _x; pre_y += _y; } else {pre_x = _x; pre_y = _y; } vcgen_addpoint(pre_x, pre_y, PathPointTypeStart); }
#define line_to(_x, _y, rel)         { if (rel) {pre_x += _x; pre_y += _y; } else {pre_x = _x; pre_y = _y; } vcgen_addpoint(pre_x, pre_y, PathPointTypeLine); }
#define vline_to(_y, rel)            { if (rel) {pre_y += _y; } else {pre_y = _y; } vcgen_addpoint(pre_x, pre_y, PathPointTypeLine); }
#define hline_to(_x, rel)            { if (rel) {pre_x += _x; } else {pre_x = _x; } vcgen_addpoint(pre_x, pre_y, PathPointTypeLine); }
#define quad_to(x1, y1, _x, _y, rel) { float _x1=x1, _y1=y1; if (rel) {_x1+=pre_x; _y1+=pre_y; pre_x += _x; pre_y += _y; } else {pre_x = _x; pre_y = _y; } vcgen_addpoint(_x1, _y1, PathPointTypeBezier2); vcgen_addpoint(pre_x, pre_y, PathPointTypeLine); }
#define quadT_to(_x, _y, rel)        { if (rel) {pre_x += _x; pre_y += _y; } else {pre_x = _x; pre_y = _y; } vcgen_addpoint(pre_x, pre_y, PathPointTypeBezier2); }
#define cubic_to(x1, y1, x2, y2, _x, _y, rel)   { float _x1=x1, _y1=y1, _x2=x2, _y2=y2; if (rel) {_x1+=pre_x; _y1+=pre_y; _x2+=pre_x; _y2+=pre_y; pre_x += _x; pre_y += _y; } else {pre_x = _x; pre_y = _y; } vcgen_addpoint(_x1, _y1, PathPointTypeBezier3); vcgen_addpoint(_x2, _y2, PathPointTypeBezier3); vcgen_addpoint(pre_x, pre_y, PathPointTypeLine); }
#define cubicS_to(x1, y1, _x, _y, rel) { float _x1=x1, _y1=y1; if (rel) {_x1+=pre_x; _y1+=pre_y; pre_x += _x; pre_y += _y; } else {pre_x = _x; pre_y = _y; } vcgen_addpoint(_x1, _y1, PathPointTypeBezier3); vcgen_addpoint(pre_x, pre_y, PathPointTypeBezier3); }
#define close_subpath()   if (Types && ipos>0) {Types[ipos-1]|=PathPointTypeCloseSubpath;}


//三次贝塞尔曲线
CC_INLINE float bezier3funcX(float uu,FPOINT *controlP) {  
  float part0 = controlP[0].x * uu * uu * uu;  
  float part1 = 3 * controlP[1].x * uu * uu * (1 - uu);  
  float part2 = 3 * controlP[2].x * uu * (1 - uu) * (1 - uu);  
  float part3 = controlP[3].x * (1 - uu) * (1 - uu) * (1 - uu);     
  return part0 + part1 + part2 + part3;   
}      
CC_INLINE float bezier3funcY(float uu, FPOINT *controlP){  
  float part0 = controlP[0].y * uu * uu * uu;  
  float part1 = 3 * controlP[1].y * uu * uu * (1 - uu);  
  float part2 = 3 * controlP[2].y * uu * (1 - uu) * (1 - uu);  
  float part3 = controlP[3].y * (1 - uu) * (1 - uu) * (1 - uu);     
  return part0 + part1 + part2 + part3;   
}

// 穿过已知点画平滑曲线（3次贝塞尔曲线）
//控制点收缩系数 ，经调试0.6较好
static int createCurve(const FPOINT *originPoint, int originCount, FPOINT* curvePoint, int maxCurveCount){  
  int i, curveCount=0;
  float scale = 0.6;
  FPOINT* midpoints = (FPOINT*)pmalloc(sizeof(FPOINT*)*originCount);
  FPOINT* extrapoints = (FPOINT*)pmalloc(sizeof(FPOINT*)* 2 * originCount);
  //生成中点       
  for(i = 0 ;i < originCount ; i++){      
    int nexti = (i + 1) % originCount;  
    midpoints[i].x = (originPoint[i].x + originPoint[nexti].x)/2.0;  
    midpoints[i].y = (originPoint[i].y + originPoint[nexti].y)/2.0;  
  }      
  
  //平移中点  
  for(i = 0 ;i < originCount ; i++){
    int offsetx, offsety, extraindex, addx, addy, extranexti;
    int nexti = (i + 1) % originCount;  
    int backi = (i + originCount - 1) % originCount;
    FPOINT midinmid;  
    midinmid.x = (midpoints[i].x + midpoints[backi].x)/2.0;  
    midinmid.y = (midpoints[i].y + midpoints[backi].y)/2.0;  
    offsetx = originPoint[i].x - midinmid.x;  
    offsety = originPoint[i].y - midinmid.y;  
    extraindex = 2 * i;  
    extrapoints[extraindex].x = midpoints[backi].x + offsetx;  
    extrapoints[extraindex].y = midpoints[backi].y + offsety;  
    //朝 originPoint[i]方向收缩   
    addx = (extrapoints[extraindex].x - originPoint[i].x) * scale;  
    addy = (extrapoints[extraindex].y - originPoint[i].y) * scale;  
    extrapoints[extraindex].x = originPoint[i].x + addx;  
    extrapoints[extraindex].y = originPoint[i].y + addy;  
    
    extranexti = (extraindex + 1)%(2 * originCount);  
    extrapoints[extranexti].x = midpoints[i].x + offsetx;  
    extrapoints[extranexti].y = midpoints[i].y + offsety;  
    //朝 originPoint[i]方向收缩   
    addx = (extrapoints[extranexti].x - originPoint[i].x) * scale;  
    addy = (extrapoints[extranexti].y - originPoint[i].y) * scale;  
    extrapoints[extranexti].x = originPoint[i].x + addx;  
    extrapoints[extranexti].y = originPoint[i].y + addy;  
    
  }      
   
  //生成4控制点，产生贝塞尔曲线  
  for(i = 0 ;i < originCount ; i++) {
    FPOINT tempP, controlPoint[4];
    int extraindex, extranexti, nexti;
    float u;
    controlPoint[0] = originPoint[i];
    extraindex = 2 * i;
    controlPoint[1] = extrapoints[extraindex + 1];
    extranexti = (extraindex + 2) % (2 * originCount);
    controlPoint[2] = extrapoints[extranexti];
    nexti = (i + 1) % originCount;
    controlPoint[3] = originPoint[nexti];
    u = 1;
    while(u >= 0){
      int px = bezier3funcX(u,controlPoint);
      int py = bezier3funcY(u,controlPoint);
      // u的步长决定曲线的疏密
      u -= 0.005;
      tempP.x = px;
      tempP.y = py;
      //存入曲线点
      if (curvePoint) {
        curvePoint[curveCount] = (tempP);  
      }
      ++curveCount;
    }
  }
  pfree(extrapoints);
  pfree(midpoints);
  return curveCount;
}  

/* We plot the curve as if it is on a circle then stretch the points.  This
* adjusts the angles so that when we stretch the points they will end in the
* right place. This is only complicated because atan and atan2 do not behave
* conveniently. */
static void unstretch_angle(float * angle, float rad_x, float rad_y)
{
  float stretched;
  int revs_off;
  
  *angle = deg2rad(*angle);
  
  if(fabs(cos(*angle)) < 0.00001 || fabs(sin(*angle)) < 0.00001)
    return;
  
  stretched = gdiplus_atan2(sin(*angle) / fabs(rad_y), cos(*angle) / fabs(rad_x));
  revs_off = gdip_round(*angle / (2.0 * M_PI)) - gdip_round(stretched / (2.0 * M_PI));
  stretched += ((float)revs_off) * M_PI * 2.0;
  *angle = stretched;
}

static int path_remove_same(FPOINT* Points, BYTE* Types, int Count, double eps) {
  int i=1, j=0;
  eps = BOUND(eps, 0.0001, 2);
  if (Count>0) {
    for (; i<Count; ++i) {
      if (Types[i]!=Types[j] || (fabs(Points[i].x-Points[j].x)+fabs(Points[i].y-Points[j].y))>eps) {
        ++j;
        Points[j] = Points[i];
        Types[j] = Types[i];
      }
    }
    Count = j+1;
  }
  return Count;
}

/* Calculates the bezier points needed to fill in the arc portion starting at
* angle start and ending at end.  These two angles should be no more than 90
* degrees from each other.  x1, y1, x2, y2 describes the bounding box (upper
* left and width and height).  Angles must be in radians. write_first indicates
* that the first bezier point should be written out (usually this is false).
* pt is the array of PointFs that gets written to.
**/
static void add_arc_part(FPOINT* Points, BYTE* Types, float x1, float y1, float x2, float y2, float start, float end, BOOL write_first)
{
  float center_x, center_y, rad_x, rad_y, cos_start, cos_end,
    sin_start, sin_end, a, half;
  
  rad_x = x2 / 2.0;
  rad_y = y2 / 2.0;
  center_x = x1 + rad_x;
  center_y = y1 + rad_y;
  
  cos_start = cos(start);
  cos_end = cos(end);
  sin_start = sin(start);
  sin_end = sin(end);
  
  half = (end - start) / 2.0;
  a = 4.0 / 3.0 * (1 - cos(half)) / sin(half);
  
  if (write_first) {
    Points[0].x = cos_start * rad_x + center_x;
    Points[0].y = sin_start * rad_y + center_y;
    Types[0] = PathPointTypeLine;
  }
  Points[1].x = (cos_start - a * sin_start) * rad_x + center_x;
  Points[1].y = (sin_start + a * cos_start) * rad_y + center_y;
  Points[2].x = (cos_end + a * sin_end) * rad_x + center_x;
  Points[2].y = (sin_end - a * cos_end) * rad_y + center_y;
  Points[3].x = cos_end * rad_x + center_x;
  Points[3].y = sin_end * rad_y + center_y;
  Types[1] = PathPointTypeLine;
  Types[2] = PathPointTypeLine;
  Types[1] = PathPointTypeBezier3;
  Types[2] = PathPointTypeBezier3;
  Types[3] = PathPointTypeLine;
}

/* Stores the bezier points that correspond to the arc in points.  If points is
* null, just return the number of points needed to represent the arc. */
static int arc2polybezier(FPOINT* Points, BYTE* Types, float x, float y, float w, float h, float startAngle, float sweepAngle)
{
  int i;
  float end_angle, start_angle, endAngle;
  endAngle = startAngle + sweepAngle;
  unstretch_angle(&startAngle, w / 2.0, h / 2.0);
  unstretch_angle(&endAngle, w / 2.0, h / 2.0);
  
  /* start_angle and end_angle are the iterative variables */
  start_angle = startAngle;
  
  for(i = 0; i < MAX_ARC_PTS - 1; i += 3) {
    /* check if we've overshot the end angle */
    if( sweepAngle > 0.0 ) {
      if (start_angle >= endAngle)
        break;
      end_angle = MIN(start_angle + M_PI/2, endAngle);
    } else {
      if (start_angle <= endAngle)
        break;
      end_angle = MAX(start_angle - M_PI/2, endAngle);
    }
    
    if (Points) {
      add_arc_part(Points+i, Types+i, x, y, w, h, start_angle, end_angle, 0==i);
    }
    
    start_angle += M_PI/2 * (sweepAngle < 0.0 ? -1.0 : 1.0);
  }
  
  if (i == 0) {
    return 0;
  }
  return i+1;
}

//vertex_dist
// Vertex (x, y) with the distance to the next one. The last vertex has
// distance between the last and the first points if the polygon is closed
// and 0.0 if it's a polyline.
typedef struct {
  double x;
  double y;
  double dist;
} vertex_dist;
CC_INLINE BOOL vdist(vertex_dist* v, const vertex_dist* val)
{
  BOOL ret = (v->dist = calc_distance(v->x, v->y, val->x, val->y)) > vertex_dist_epsilon;
  if (!ret) {
    v->dist = 1.0 / vertex_dist_epsilon;
  }
  return ret;
}
CC_INLINE double ploylen(int n, const DPOINT* pt, int closed)
{
  int i, j;
  double ret = 0;
  for (i = 0; i < n - !closed; ++i) {
    j = (i + 1) % n;
    ret += calc_distance(pt[i].x, pt[i].y, pt[j].x, pt[j].y);
  }
  return ret;
}
static int vertex_dist_init2(int n, const FPOINT* pt, BOOL closed, vertex_dist* vpt, double* pdistsum)
{
  int i, j, ipos=0, ret;
  double distsum=0, dist;
  if (n>0) {
    for (i = 0; i < n-!closed; ++i) {
      j = (i+1)%n;
      ret = (dist = calc_distance(pt[i].x, pt[i].y, pt[j].x, pt[j].y)) > vertex_dist_epsilon;
      if (!ret) {
        dist = 1.0 / vertex_dist_epsilon;
      } else {
        if (!closed && (i==(n-1))) {
          dist = 0;
        }
        distsum += dist;
        if (vpt) {
          vpt[ipos].x = pt[i].x;
          vpt[ipos].y = pt[i].y;
          vpt[ipos].dist = dist;
        }
        ++ipos;
      }
    }
    if (vpt && !closed) {
      i = n-1;
      vpt[ipos].x = pt[i].x;
      vpt[ipos].y = pt[i].y;
      vpt[ipos].dist = 0;
      ++ipos;
    }
  }
  if (pdistsum) {
    *pdistsum = distsum;
  }
  return ipos;
}

////////////////////////////////////////////////////////////////////////////
// math_stroke
CC_INLINE void miter_limit_theta(PenStyle* s, double t)
{
  s->miterLimit = 1.0 / sin(t * 0.5) ;
}

CC_INLINE int vcgen_calc_arc(FPOINT* Points, int ipos, const PenStyle* s, double x, double y, double dx1, double dy1, double dx2, double dy2, double m_width)
{
  int m_width_sign = CC_SIGN(m_width);
  double m_width_abs = fabs(m_width);
  double a1, a2, da;
  int i, n;
  enum {calc_arc_min = 3, calc_arc_max = 32};
  if (NULL==Points) {
    n = 40;
    n = BOUND(n, calc_arc_min, calc_arc_max);
    ipos += n;
    return ipos;
  }
  a1 = atan2(dy1 * m_width_sign, dx1 * m_width_sign);
  a2 = atan2(dy2 * m_width_sign, dx2 * m_width_sign);
  da = a1 - a2;
  da = acos(m_width_abs / (m_width_abs + 0.125 / s->approxScale)) * 2;
  if (Points) {
    vcgen_line_to(x + dx1, y + dy1);
  }
  if (m_width_sign > 0) {
    if (a1 > a2) {
      a2 += 2 * CC_PI;
    }
    n = (int)((a2 - a1) / da);
    n = BOUND(n, calc_arc_min, calc_arc_max);
    da = (a2 - a1) / (n + 1);
    a1 += da;
    for (i = 0; i < n; i++) {
      vcgen_line_to(x + cos(a1) * m_width, y + sin(a1) * m_width);
      a1 += da;
    }
  }
  else {
    if (a1 < a2) {
      a2 -= 2 * CC_PI;
    }
    n = (int)((a1 - a2) / da);
    n = BOUND(n, calc_arc_min, calc_arc_max);
    da = (a1 - a2) / (n + 1);
    a1 -= da;
    for (i = 0; i < n; i++) {
      vcgen_line_to(x + cos(a1) * m_width, y + sin(a1) * m_width);
      a1 -= da;
    }
  }
  vcgen_line_to(x + dx2, y + dy2);
  return ipos;
}
CC_INLINE int vcgen_calc_miter(FPOINT* Points, int ipos, const PenStyle* s, const vertex_dist* v0, const vertex_dist* v1, const vertex_dist* v2,
    double dx1, double dy1, double dx2, double dy2, LineJoin line_join, double mlimit, double dbevel, double m_width)
{
  int m_width_sign = CC_SIGN(m_width);
  double m_width_abs = fabs(m_width);
  double xi = v1->x;
  double yi = v1->y;
  double di = 1;
  double lim = m_width_abs * mlimit;
  BOOL miter_limit_exceeded = TRUE; // Assume the worst
  BOOL intersection_failed = TRUE; // Assume the worst
  if (calc_intersection(v0->x + dx1, v0->y - dy1,
      v1->x + dx1, v1->y - dy1,
      v1->x + dx2, v1->y - dy2,
      v2->x + dx2, v2->y - dy2,
      &xi, &yi)) {
    // Calculation of the intersection succeeded
    //---------------------
    di = calc_distance(v1->x, v1->y, xi, yi);
    if (di <= lim) {
      // Inside the miter limit
      //---------------------
      vcgen_line_to(xi, yi);
      miter_limit_exceeded = FALSE;
    }
    intersection_failed = FALSE;
  }
  else {
    // Calculation of the intersection failed, most probably
    // the three points lie one straight line.
    // First check if v0 and v2 lie on the opposite sides of vector:
    // (v1->x, v1->y) -> (v1->x+dx1, v1->y-dy1), that is, the perpendicular
    // to the line determined by vertices v0 and v1->
    // This condition determines whether the next line segments continues
    // the previous one or goes back.
    //----------------
    double x2 = v1->x + dx1;
    double y2 = v1->y - dy1;
    if ((cross_product(v0->x, v0->y, v1->x, v1->y, x2, y2) < 0.0) ==
        (cross_product(v1->x, v1->y, v2->x, v2->y, x2, y2) < 0.0)) {
      // This case means that the next segment continues
      // the previous one (straight line)
      //-----------------
      vcgen_line_to(v1->x + dx1, v1->y - dy1);
      miter_limit_exceeded = FALSE;
    }
  }
  if (miter_limit_exceeded) {
    // Miter limit exceeded
    //------------------------
    switch (line_join) {
    case LineJoinMiterRevert:
      // For the compatibility with SVG, PDF, etc,
      // we use a simple bevel join instead of
      // "smart" bevel
      //-------------------
      vcgen_line_to(v1->x + dx1, v1->y - dy1);
      vcgen_line_to(v1->x + dx2, v1->y - dy2);
      break;
    case LineJoinMiterRound:
      ipos = vcgen_calc_arc(Points, ipos, s, v1->x, v1->y, dx1, -dy1, dx2, -dy2, m_width);
      break;
    default:
      // If no miter-revert, calculate new dx1, dy1, dx2, dy2
      //----------------
      if (intersection_failed) {
        mlimit *= m_width_sign;
        vcgen_line_to(v1->x + dx1 + dy1 * mlimit, v1->y - dy1 + dx1 * mlimit);
        vcgen_line_to(v1->x + dx2 - dy2 * mlimit, v1->y - dy2 - dx2 * mlimit);
      }
      else {
        double x1 = v1->x + dx1;
        double y1 = v1->y - dy1;
        double x2 = v1->x + dx2;
        double y2 = v1->y - dy2;
        di = (lim - dbevel) / (di - dbevel);
        vcgen_line_to(x1 + (xi - x1) * di, y1 + (yi - y1) * di);
        vcgen_line_to(x2 + (xi - x2) * di, y2 + (yi - y2) * di);
      }
      break;
    }
  }
  return ipos;
}
CC_INLINE int vcgen_calc_cap(FPOINT* Points, int ipos, const PenStyle* s, const vertex_dist* v0, const vertex_dist* v1, double len, LineCap m_cap, double m_width)
{
  int m_width_sign = CC_SIGN(m_width);
  double m_width_abs = fabs(m_width);
  double dx1;
  double dy1;
  double dx2 = 0;
  double dy2 = 0;
  if (fabs(len) < 0.000001) {
    return 0;
  }
  dx1 = (v1->y - v0->y) / len;
  dy1 = (v1->x - v0->x) / len;
  dx1 *= m_width;
  dy1 *= m_width;
  switch (m_cap) {
  case LineCapRound:
    {
      double da = acos(m_width_abs / (m_width_abs + 0.125 / s->approxScale)) * 2;
      double a1;
      int i;
      int n = (int)(CC_PI / da);
      da = CC_PI / (n + 1);
      vcgen_line_to(v0->x - dx1, v0->y + dy1);
      if (m_width_sign < 0) {
        dy1 = -dy1;
        dx1 = -dx1;
        da = -da;
      }
      a1 = atan2(dy1, -dx1);
      a1 += da;
      for (i = 0; i < n; i++) {
        vcgen_line_to(v0->x + cos(a1) * m_width, v0->y + sin(a1) * m_width);
        a1 += da;
      }
      vcgen_line_to(v0->x + dx1, v0->y - dy1);
    }
    break;
  case LineCapTriangle:
    dx2 = dy1 * m_width_sign;
    dy2 = dx1 * m_width_sign;
    vcgen_line_to(v0->x - dx1, v0->y + dy1);
    vcgen_line_to(v0->x - dx2, v0->y - dy2);
    vcgen_line_to(v0->x + dx1, v0->y - dy1);
    break;
  default:
  case LineCapSquare:
  case LineCapFlat:
  case LineCapNoAnchor:
    {
      if (m_cap == LineCapSquare) {
        dx2 = dy1 * m_width_sign;
        dy2 = dx1 * m_width_sign;
      }
      vcgen_line_to(v0->x - dx1 - dx2, v0->y + dy1 - dy2);
      vcgen_line_to(v0->x + dx1 - dx2, v0->y - dy1 - dy2);
    }
    break;
  }
  return ipos;
}
CC_INLINE int vcgen_calc_join(FPOINT* Points, int ipos, const PenStyle* s, const vertex_dist* v0, const vertex_dist* v1, const vertex_dist* v2, double len1, double len2, double m_width)
{
  double m_width_abs = fabs(m_width);
  double dx1, dy1, dx2, dy2, cp;
  InnerJoin m_inner_join = s->innerJoin;
  LineJoin m_line_join = s->lineJoin;
  //ASSERT(len1 > 0.00001 && len2 > 0.00001);
  //if (len1<0.00001 || len2<0.00001) return 0;
  dx1 = m_width * (v1->y - v0->y) / len1;
  dy1 = m_width * (v1->x - v0->x) / len1;
  dx2 = m_width * (v2->y - v1->y) / len2;
  dy2 = m_width * (v2->x - v1->x) / len2;
  cp = cross_product(v0->x, v0->y, v1->x, v1->y, v2->x, v2->y);
  if (cp != 0 && (cp > 0) == (m_width > 0)) {
    // Inner join
    //---------------
    double limit = ((len1 < len2) ? len1 : len2) / m_width_abs;
    if (limit < s->innerMiterLimit) {
      limit = s->innerMiterLimit;
    }
    switch (m_inner_join) {
    default: // inner_bevel
      vcgen_line_to(v1->x + dx1, v1->y - dy1);
      vcgen_line_to(v1->x + dx2, v1->y - dy2);
      break;
    case InnerJoinMiter:
      ipos = vcgen_calc_miter(Points, ipos, s, v0, v1, v2, dx1, dy1, dx2, dy2, LineJoinMiterRevert, limit, 0, m_width);
      break;
    case InnerJoinJag:
    case InnerJoinRound:
      cp = (dx1 - dx2) * (dx1 - dx2) + (dy1 - dy2) * (dy1 - dy2);
      if (cp < len1 * len1 && cp < len2 * len2) {
        ipos = vcgen_calc_miter(Points, ipos, s, v0, v1, v2, dx1, dy1, dx2, dy2, LineJoinMiterRevert, limit, 0, m_width);
      }
      else {
        if (m_inner_join == InnerJoinJag) {
          vcgen_line_to(v1->x + dx1, v1->y - dy1);
          vcgen_line_to(v1->x, v1->y);
          vcgen_line_to(v1->x + dx2, v1->y - dy2);
        }
        else {
          vcgen_line_to(v1->x + dx1, v1->y - dy1);
          vcgen_line_to(v1->x, v1->y);
          ipos = vcgen_calc_arc(Points, ipos, s, v1->x, v1->y, dx2, -dy2, dx1, -dy1, m_width);
          vcgen_line_to(v1->x, v1->y);
          vcgen_line_to(v1->x + dx2, v1->y - dy2);
        }
      }
      break;
    }
  }
  else {
    double dx = (dx1 + dx2) / 2;
    double dy = (dy1 + dy2) / 2;
    double dbevel = sqrt(dx * dx + dy * dy);
    double m_width_eps = m_width / 1024.0;
    if (m_line_join == LineJoinRound || m_line_join == LineJoinBevel) {
      if (s->approxScale * (m_width_abs - dbevel) < m_width_eps) {
        if (calc_intersection(v0->x + dx1, v0->y - dy1, v1->x + dx1, v1->y - dy1,
            v1->x + dx2, v1->y - dy2, v2->x + dx2, v2->y - dy2, &dx, &dy)) {
          vcgen_line_to(dx, dy);
        }
        else {
          vcgen_line_to(v1->x + dx1, v1->y - dy1);
        }
        return ipos;
      }
    }
    switch (m_line_join) {
    case LineJoinMiter:
    case LineJoinMiterRevert:
    case LineJoinMiterRound:
      ipos = vcgen_calc_miter(Points, ipos, s, v0, v1, v2, dx1, dy1, dx2, dy2, m_line_join, s->miterLimit, dbevel, m_width);
      break;
    case LineJoinRound:
      ipos = vcgen_calc_arc(Points, ipos, s, v1->x, v1->y, dx1, -dy1, dx2, -dy2, m_width);
      break;
    default: // Bevel join
      vcgen_line_to(v1->x + dx1, v1->y - dy1);
      vcgen_line_to(v1->x + dx2, v1->y - dy2);
      break;
    }
  }
  return ipos;
}

CC_INLINE int vcgen_stroke_close(FPOINT* Points, BYTE* Types, int ipos, int* lens, const PenStyle* s, const vertex_dist* vpt, int vpt_size, const double* wline) {
  int i, start = ipos;
  const vertex_dist* v0, *v1, *v2;
  double wline1 = wline[0];
  double wline2 = wline[1];
  if (vpt_size < 2) {
    //return 0;
  }
  if (fabs(wline1)>0) {
    for (i = 0; i < vpt_size; ++i) {
      v0 = vpt + (i - 1 + vpt_size) % vpt_size;
      v1 = vpt + i;
      v2 = vpt + (i + 1) % vpt_size;
      ipos = vcgen_calc_join(Points, ipos, s, v0, v1, v2, v0->dist, v1->dist, wline1);
    }
  } else {
    if (Points) {
      for (i = 0; i < vpt_size; ++i) {
        Points[ipos+i].x = vpt[i].x;
        Points[ipos+i].y = vpt[i].y;
      }
    }
    ipos += vpt_size;
  }
  if (Types && start<ipos) {
    memset(Types+start, PathPointTypeLine, ipos-start);
    Types[start] = PathPointTypeStart;
    Types[ipos-1] |= PathPointTypeCloseSubpath;
  }
  if (lens) {
    lens[0] = ipos-start;
  }
  start = ipos;
  if (fabs(wline2)>0) {
    for (i = 0; i < vpt_size; ++i) {
      int j = (vpt_size - 1 - i);
      v0 = vpt + (j + 1) % vpt_size;
      v1 = vpt + j;
      v2 = vpt + (j - 1 + vpt_size) % vpt_size;
      ipos = vcgen_calc_join(Points, ipos, s, v0, v1, v2, v1->dist, v2->dist, wline2);
    }
  } else {
    if (Points) {
      for (i = 0; i < vpt_size; ++i) {
        int j = (vpt_size - 1 - i);
        Points[ipos+i].x = vpt[j].x;
        Points[ipos+i].y = vpt[j].y;
      }
    }
    ipos += vpt_size;
  }
  if (Types && start<ipos) {
    memset(Types+start, PathPointTypeLine, ipos-start);
    Types[start] = PathPointTypeStart;
    Types[ipos-1] |= PathPointTypeCloseSubpath;
  }
  if (lens) {
    lens[1] = ipos-start;
  }
  return ipos;
}
CC_INLINE int vcgen_stroke_open(FPOINT* Points, BYTE* Types, int ipos, const PenStyle* s, const vertex_dist* vpt, int vpt_size, double wline)
{
  int i, start = ipos;
  double wline1 = wline * 0.5;
  if (vpt_size < 2 || fabs(wline)<0.0001) {
    return 0;
  }
  ipos = vcgen_calc_cap(Points, ipos, s, vpt, vpt + 1, vpt->dist, s->startCap, wline1);
  for (i = 1; i < vpt_size - 1; ++i) {
    ipos = vcgen_calc_join(Points, ipos, s, vpt + i - 1, vpt + i, vpt + i +1, vpt[i - 1].dist, vpt[i].dist, wline1);
  }
  ipos = vcgen_calc_cap(Points, ipos, s, vpt + vpt_size - 1, vpt + vpt_size - 2, vpt[vpt_size - 2].dist, s->endCap, wline1);
  for (i = vpt_size - 2; i > 0; --i) {
    ipos = vcgen_calc_join(Points, ipos, s, vpt + i + 1, vpt + i, vpt + i - 1, vpt[i].dist, vpt[i-1].dist, wline1);
  }
  if (Types && start<ipos) {
    memset(Types+start, PathPointTypeLine, ipos-start);
    Types[start] = PathPointTypeStart;
    Types[ipos-1] |= PathPointTypeCloseSubpath;
  }
  return ipos;
}
static int vcgen_stroke(FPOINT* Points, BYTE* Types, int ipos, int* lens, const FPOINT* pt, int n, BOOL closed, const PenStyle* s) {
  int p, k = 0, vpt_size=0;
  double wline = s->lineWidth;
  vertex_dist* vpt = NULL;
  double wline0[3] = {wline*0.5, wline*0.5};
  double wline1[3] = {wline, 0, wline};
  if (NULL==Points) {
    ipos += 8 + n * 2 * (int)MAX(10, s->lineWidth);
    return ipos;
  }
  vpt = (vertex_dist*)pmalloc(n*sizeof(*vpt));
  vpt_size = vertex_dist_init2(n, pt, closed, vpt, NULL);
  if (vpt_size<=0) {
    return ipos;
  }
  if (closed) {
    PenAlignment penAlignment = s->penAlignment;
    const double* wlines = wline0;
    //penAlignment = PenAlignmentCenter;
    switch (penAlignment) {
    case PenAlignmentCenter: //指定 Pen 对象以理论的线条为中心。
      wlines = wline0;
      break;
    case PenAlignmentLeft: //指定将 Pen 定位于理论的线条的左侧。
      wlines = wline1;
      break;
    case PenAlignmentRight: //指定将 Pen 定位于理论的线条的右侧。
      wlines = wline1+1;
      break;
    case PenAlignmentInset: //指定 Pen 被定位于理论的线条内。
      p = fpolygon_area(n, pt)>0;
      wlines = wline1+p;
      break;
    case PenAlignmentOutset: //指定将 Pen 定位于理论的线条外。
      p = fpolygon_area(n, pt)<0;
      wlines = wline1+p;
      break;
    }
    ipos = vcgen_stroke_close(Points, Types, ipos, lens, s, vpt, vpt_size, wlines);
  } else {
    int start = ipos;
    //指定 Pen 对象以理论的线条为中心。
    ipos = vcgen_stroke_open(Points, Types, ipos, s, vpt, vpt_size, wline);
    if (lens) {
      lens[0] = ipos - start;
    }
  }
  pfree(vpt);
  return ipos;
}
// 虚线
CC_INLINE int vcgen_dash1(FPOINT* Points, BYTE* Types, int ipos, const FPOINT* pt, int n, BOOL m_closed, const float* m_dashes, int m_num_dashes, double m_dash_start) {
  double             m_total_dash_len=0;
  double             m_shorten=0;
  double             m_curr_dash_start=0;
  int           m_curr_dash=0;
  double             m_curr_rest=0;
  double             distsum=0;
  const vertex_dist* m_v1=0;
  const vertex_dist* m_v2=0;
  int i=0, vpt_size;
  vertex_dist* vpt = NULL;
  m_num_dashes = m_num_dashes&(~1);
  if(m_num_dashes < 2 || n < 2) {
    return 0;
  }
  for (i = 0; i < m_num_dashes; ++i) {
    m_total_dash_len += m_dashes[i];
  }
  if (NULL==Points) {
    vpt_size = vertex_dist_init2(n, pt, m_closed, NULL, &distsum);
    ipos += (distsum >0 ? (1 + distsum/m_total_dash_len) : 0)*m_num_dashes;
    ipos += n + 10;
    return ipos;
  }
  vpt = (vertex_dist*)pmalloc(n*sizeof(*vpt));
  vpt_size = vertex_dist_init2(n, pt, m_closed, vpt, &distsum);
  if(vpt_size < 2) {
    pfree(vpt);
    return 0;
  }
  i = 1;
  m_v1 = &vpt[0];
  m_v2 = &vpt[1];
  m_curr_rest = m_v1->dist;
  if (Points) {
    Points[ipos].x = m_v1->x;
    Points[ipos].y = m_v1->y;
    Types[ipos] = PathPointTypeStart;
  }
  ++ipos;
  // calc_dash_start
  if (m_dash_start >= 0.0) {
    double ds = m_dash_start;
    m_curr_dash = 0;
    m_curr_dash_start = 0.0;
    while(ds > 0.0) {
      if(ds > m_dashes[m_curr_dash]) {
        ds -= m_dashes[m_curr_dash];
        ++m_curr_dash;
        m_curr_dash_start = 0.0;
        if(m_curr_dash >= m_num_dashes) m_curr_dash = 0;
      }
      else
      {
        m_curr_dash_start = ds;
        ds = 0.0;
      }
    }
  }
   
  for (;i<vpt_size;) {
    double dash_rest = m_dashes[m_curr_dash] - m_curr_dash_start;
    BYTE cmd = (m_curr_dash & 1) ? PathPointTypeStart : PathPointTypeLine;
    if(m_curr_rest > dash_rest) {
      m_curr_rest -= dash_rest;
      m_curr_dash = (m_curr_dash+1)%m_num_dashes;
      m_curr_dash_start = 0.0;
      if (Points) {
        Points[ipos].x = m_v2->x - (m_v2->x - m_v1->x) * m_curr_rest / m_v1->dist;
        Points[ipos].y = m_v2->y - (m_v2->y - m_v1->y) * m_curr_rest / m_v1->dist;
        Types[ipos] = cmd;
      }
      ++ipos;
    } else {
      m_curr_dash_start += m_curr_rest;
      if (Points) {
        Points[ipos].x = m_v2->x;
        Points[ipos].y = m_v2->y;
        Types[ipos] = cmd;
      }
      ++ipos;
      ++i;
      m_v1 = m_v2;
      m_curr_rest = m_v1->dist;
      m_v2 = vpt + i%vpt_size;
    }
  }
  // vertex
  pfree(vpt);
  return ipos;
}
CC_INLINE int vcgen_dash(const FPOINT* pt, int n, int m_closed, const float* m_dashes, int m_num_dashes, double m_dash_start, int maxout, int maxlens, FPOINT* out, int* lens)
{
  int i;
  FPOINT* pt0 = out;
  const FPOINT* spt = pt;
  int sptlen = n;
  int lens_count = 0;
  double m_total_dash_len;
  double m_curr_dash_start;
  double m_curr_rest;
  double m_v1_dist;
  const FPOINT* m_v1;
  const FPOINT* m_v2;
  int m_curr_dash = 0;
  m_total_dash_len = 0.0;
  m_curr_dash_start = 0.0;
  if (m_num_dashes < 2 || sptlen < 2) {
    return 0;
  }
  for (i = 0; i < m_num_dashes; ++i) {
    m_total_dash_len += m_dashes[i];
  }
  i = 1;
  m_v1 = &spt[0];
  m_v2 = &spt[1];
  m_v1_dist = calc_distance(m_v1->x, m_v1->y, m_v2->x, m_v2->y);
  m_curr_rest = m_v1_dist;
  if (m_dash_start >= 0.0) {
    double ds = m_dash_start;
    m_curr_dash = 0;
    m_curr_dash_start = 0.0;
    while (ds > 0.0) {
      if (ds > m_dashes[m_curr_dash]) {
        ds -= m_dashes[m_curr_dash];
        ++m_curr_dash;
        m_curr_dash_start = 0.0;
        if (m_curr_dash >= m_num_dashes) {
          m_curr_dash = 0;
        }
      }
      else {
        m_curr_dash_start = ds;
        ds = 0.0;
      }
    }
  }
  if (!(m_curr_dash & 1)) {
    *pt0++ = fPOINT(m_v1->x, m_v1->y);
  }
  for (; 1;) {
    // 到下一虚线点距离
    double dash_rest = m_dashes[m_curr_dash] - m_curr_dash_start;
    if (m_curr_rest > dash_rest) {
      m_curr_rest -= dash_rest;
      ++m_curr_dash;
      if (m_curr_dash >= m_num_dashes) {
        m_curr_dash = 0;
      }
      m_curr_dash_start = 0.0;
      *pt0++ = fPOINT(m_v2->x - (m_v2->x - m_v1->x) * m_curr_rest / m_v1_dist, m_v2->y - (m_v2->y - m_v1->y) * m_curr_rest / m_v1_dist);
      if (m_curr_dash & 1) {
        lens[lens_count] = pt0 - out;
        //printf("%d, %f\n", lens[lens_count], ploylen(lens[lens_count], out, 0));
        out = pt0;
        lens_count++;
        ASSERT(lens_count < maxlens);
      }
    }
    else {
      m_curr_dash_start += m_curr_rest;
      if (!(m_curr_dash & 1)) {
        *pt0++ = fPOINT(m_v2->x, m_v2->y);
      }
      //printf("ppp\n");
      ++i;
      m_v1 = m_v2;
      if (m_closed) {
        if (i > sptlen) {
          break;
        }
        m_v2 = &spt[(i >= sptlen) ? 0 : i];
      }
      else {
        if (i >= sptlen) {
          break;
        }
        m_v2 = &spt[i];
      }
      m_v1_dist = calc_distance(m_v1->x, m_v1->y, m_v2->x, m_v2->y);
      m_curr_rest = m_v1_dist;
    }
  }
  lens[lens_count++] = pt0 - out;
  return lens_count;
}
static int vcgen_set_types(BYTE* Types, int Count) {
  if (Count>0) {
    memset(Types, PathPointTypeLine, Count);
    Types[0] |= PathPointTypeStart;
    Types[Count-1] = PathPointTypeLine|PathPointTypeCloseSubpath;
  }
  return 0;
}
#define vcgen_rectR(pt, rc) vcgen_rect(pt, 0, 4, (rc)->left, (rc)->top, (rc)->right, (rc)->bottom)
CC_INLINE int vcgen_rect(FPOINT* pt, int i, double x1, double y1, double x2, double y2)
{
  PTSET(pt[i + 0], x1, y1);
  PTSET(pt[i + 1], x2, y1);
  PTSET(pt[i + 2], x2, y2);
  PTSET(pt[i + 3], x1, y2);
  i += 4;
  return i;
}
CC_INLINE int path_rect(FPOINT* Points, BYTE* Types, int ipos, double x1, double y1, double x2, double y2) {
  if (Points) {
    PTSET(Points[ipos + 0], x1, y1);
    PTSET(Points[ipos + 1], x2, y1);
    PTSET(Points[ipos + 2], x2, y2);
    PTSET(Points[ipos + 3], x1, y2);
    Types[ipos + 0] = PathPointTypeStart;
    Types[ipos + 1] = PathPointTypeLine;
    Types[ipos + 2] = PathPointTypeLine;
    Types[ipos + 3] = PathPointTypeLine|PathPointTypeCloseSubpath;
  }
  return ipos + 4;
}
CC_INLINE int vcgen_rect_f(FPOINT* pt, int i, float x1, float y1, float x2, float y2)
{
  PTSET(pt[i + 0], x1, y1);
  PTSET(pt[i + 1], x2, y1);
  PTSET(pt[i + 2], x2, y2);
  PTSET(pt[i + 3], x1, y2);
  i += 4;
  return i;
}
CC_INLINE int vcgen_rect2(FPOINT* pt, double x1, double y1, double x2, double y2, double bw, BOOL dir)
{
  if (dir) {
    PTSET(pt[0], x1 + bw, y1 + bw);
    PTSET(pt[1], x1 + bw, y2 - bw);
    PTSET(pt[2], x2 - bw, y2 - bw);
    PTSET(pt[3], x2 - bw, y1 + bw);
  }
  else {
    PTSET(pt[0], x1 + bw, y1 + bw);
    PTSET(pt[1], x2 - bw, y1 + bw);
    PTSET(pt[2], x2 - bw, y2 - bw);
    PTSET(pt[3], x1 + bw, y2 - bw);
  }
  return 4;
}
CC_INLINE int vcgen_strokerect(FPOINT* pt, int* lens, double x1, double y1, double x2, double y2, double bw)
{
  PTSET(pt[0], x1, y1);
  PTSET(pt[1], x2, y1);
  PTSET(pt[2], x2, y2);
  PTSET(pt[3], x1, y2);
  PTSET(pt[4], x1 + bw, y1 + bw);
  PTSET(pt[5], x1 + bw, y2 - bw);
  PTSET(pt[6], x2 - bw, y2 - bw);
  PTSET(pt[7], x2 - bw, y1 + bw);
  lens[0] = 4;
  lens[1] = 4;
  return 2;
}

#define vcgen_ellipseR(pt, m_num, rc) vcgen_ellipse(pt, m_num, 0, ((rc)->l+(rc)->r)*0.5,((rc)->t+(rc)->b)*0.5,((rc)->r-(rc)->l)*0.5,((rc)->b-(rc)->t)*0.5)
CC_INLINE int vcgen_ellipse(FPOINT* out, int m_num, BOOL m_cw, double m_x, double m_y, double m_rx, double m_ry)
{
  double m_scale = 1;
  int m_step = 0;
  double ra = (fabs(m_rx) + fabs(m_ry)) / 2;
  double da = acos(ra / (ra + 0.125 / m_scale)) * 2;
  int m_num1 = (int)(4 * CC_PI / da)/2;
  enum {min_ellipse_edge = 4};
  m_num1 = MAX(m_num1, min_ellipse_edge);
  m_num = BOUND(m_num, min_ellipse_edge, m_num1);
  if (out) {
    int m_step = 0;
    for (; m_step < m_num; ++m_step) {
      double angle = 2.0 * CC_PI * m_step / m_num;
      if (m_cw) {
        angle = 2.0 * CC_PI - angle;
      }
      out[m_step].x = m_x + cos(angle) * m_rx;
      out[m_step].y = m_y + sin(angle) * m_ry;
    }
  }
  return m_num;
}
#if 0
CC_INLINE int vcgen_ellipse2(double cx, double cy, double axes_height, double axes_width, int angle, int arc_start, int arc_end, int delta, FPOINT* pts)
{
  float alpha, beta;
  double size_a = axes_width, size_b = axes_height;
  FPOINT* pts_origin = pts;
  int i;
  while (angle < 0) {
    angle += 360;
  }
  while (angle > 360) {
    angle -= 360;
  }
  if (arc_start > arc_end) {
    i = arc_start;
    arc_start = arc_end;
    arc_end = i;
  }
  while (arc_start < 0) {
    arc_start += 360;
    arc_end += 360;
  }
  while (arc_end > 360) {
    arc_end -= 360;
    arc_start -= 360;
  }
  if (arc_end - arc_start > 360) {
    arc_start = 0;
    arc_end = 360;
  }
  icvSinCos(angle, &alpha, &beta);
  for (i = arc_start; i < arc_end + delta; i += delta) {
    double x, y;
    angle = i;
    if (angle > arc_end) {
      angle = arc_end;
    }
    if (angle < 0) {
      angle += 360;
    }
    x = size_a * iSinTable[ 450 - angle ];
    y = size_b * iSinTable[ angle ];
    pts->x = (cx + x * alpha - y * beta);
    pts->y = (cy - x * beta - y * alpha);
    pts += i == arc_start || pts->x != pts[ -1 ].x || pts->y != pts[ -1 ].y;
  }
  i = (int)(pts - pts_origin);
  for (; i < 2; i++) {
    pts_origin[ i ] = pts_origin[ i - 1 ];
  }
  return i;
}
#endif
// 曲线
CC_INLINE int vcgen_arc(FPOINT* Points, int ipos, double x, double y, double rx, double ry, double startAngle, double sweepAngle) {
  int i, num_segments;
  double amin = deg2rad(startAngle), amax = deg2rad(startAngle + sweepAngle), adis;
  while(amin>2*CC_PI && amax>2*CC_PI) {
    amin -= 2*CC_PI;
    amax -= 2*CC_PI;
  }
  num_segments = (int)(sqrt(rx+ry) * 0.5*fabs(amax - amin));
  num_segments = BOUND(num_segments, 4, 64);
  if (Points) {
    adis = amax - amin;
    for (i = 0; i < num_segments; i++) {
      double a = amin + (i * adis / num_segments);
      vcgen_line_to(x + cos(a) * rx, y + sin(a) * ry);
    }
    vcgen_line_to(x + cos(amax) * rx, y + sin(amax) * ry);
  } else {
    ipos += num_segments+1;
  }
  return ipos;
}
#define vcgen_roundrect1(pt, i, num, x1, y1, x2, y2, r) vcgen_roundrect(pt, i, num, x1, y1, x2, y2, r, r, r, r, r, r, r, r)
#define vcgen_roundrectR(pt, i, num, rc, r) vcgen_roundrect(pt, i, num, (rc)->left, (rc)->top, (rc)->right, (rc)->bottom, r, r, r, r, r, r, r, r)
#define vcgen_roundrectR2(pt, i, num, rc, rx1, ry1, rx2, ry2, rx3, ry3, rx4, ry4) vcgen_roundrect(pt, i, num, (rc)->left, (rc)->top, (rc)->right, (rc)->bottom, rx1, ry1, rx2, ry2, rx3, ry3, rx4, ry4)
// 圆角矩形
CC_INLINE int vcgen_roundrect(FPOINT* Points, int ipos, double x1, double y1, double x2, double y2,
    double rx1, double ry1, double rx2, double ry2,
    double rx3, double ry3, double rx4, double ry4)
{
  double m_x1, m_y1, m_x2, m_y2, m_rx1, m_ry1, m_rx2, m_ry2, m_rx3, m_ry3, m_rx4, m_ry4;
  m_x1 = x1;
  m_y1 = y1;
  m_x2 = x2;
  m_y2 = y2;
  if (x1 > x2) {
    m_x1 = x2;
    m_x2 = x1;
  }
  if (y1 > y2) {
    m_y1 = y2;
    m_y2 = y1;
  }
  m_rx1 = rx1;
  m_ry1 = ry1;
  m_rx2 = rx2;
  m_ry2 = ry2;
  m_rx3 = rx3;
  m_ry3 = ry3;
  m_rx4 = rx4;
  m_ry4 = ry4;
  //int normalize_radius()
  {
    double dx = fabs(m_x2 - m_x1);
    double dy = fabs(m_y2 - m_y1);
    double k = 1.0;
    double t;
    t = dx / (m_rx1 + m_rx2);
    if (t < k) {
      k = t;
    }
    t = dx / (m_rx3 + m_rx4);
    if (t < k) {
      k = t;
    }
    t = dy / (m_ry1 + m_ry2);
    if (t < k) {
      k = t;
    }
    t = dy / (m_ry3 + m_ry4);
    if (t < k) {
      k = t;
    }
    if (k < 1.0) {
      m_rx1 *= k;
      m_ry1 *= k;
      m_rx2 *= k;
      m_ry2 *= k;
      m_rx3 *= k;
      m_ry3 *= k;
      m_rx4 *= k;
      m_ry4 *= k;
    }
  }
  ipos = vcgen_arc(Points, ipos, m_x1 + m_rx1, m_y1 + m_ry1, m_rx1, m_ry1, 180, 90);
  ipos = vcgen_arc(Points, ipos, m_x2 - m_rx2, m_y1 + m_ry2, m_rx2, m_ry2, 270, 90);
  ipos = vcgen_arc(Points, ipos, m_x2 - m_rx3, m_y2 - m_ry3, m_rx3, m_ry3, 0, 90);
  ipos = vcgen_arc(Points, ipos, m_x1 + m_rx4, m_y2 - m_ry4, m_rx4, m_ry4, 90, 90);
  return ipos;
}

static int vcgen_round_join_add_arc(FPOINT* Points, int ipos, const vertex_dist* v0, const vertex_dist* v1, const vertex_dist* v2, double line1Length, double line2Length, double radius)
{
  double f1 = atan2(v0->y - v1->y, v0->x - v1->x);
  double f2 = atan2(v2->y - v1->y, v2->x - v1->x);
  double alfa = f2 - f1;
  double s = radius / tan(alfa / 2);
  double newRadius = radius;
  if (fabs(alfa)<0.001 || fabs(alfa-CC_PI)<0.001) {
    return ipos;
  }
  
  if ((fabs(s) > line1Length / 2) || (fabs(s) > line2Length / 2))
  {
    if (s < 0)
      s = -MIN(line1Length / 2, line2Length / 2);
    else
      s = MIN(line1Length / 2, line2Length / 2);
    newRadius = s * tan(alfa / 2);
  }
  
  
  {
    double circleCenterAngle = f1 + alfa / 2;
  double cs = newRadius / sin(alfa / 2);
  int signs = CC_SIGN(s);
  double circleCenterx = (float)(v1->x + signs * cs * cos(circleCenterAngle));
  double circleCentery = (float)(v1->y + signs * cs * sin(circleCenterAngle));
  double NewX2 = v1->x + fabs(s) * cos(f1);
  double NewY2 = v1->y + fabs(s) * sin(f1);
  double NewX1 = v1->x + fabs(s) * cos(f2);
  double NewY1 = v1->y + fabs(s) * sin(f2);
  double firstAngle = atan2(NewY2 - circleCentery, NewX2 - circleCenterx);
  double secondAngle = atan2(NewY1 - circleCentery, NewX1 - circleCenterx);
  double startAngle = firstAngle;
  double sweepAngle = secondAngle - firstAngle;
  if (sweepAngle > CC_PI) {
    sweepAngle = -(2 * CC_PI - sweepAngle);
  } else {
    if (sweepAngle < -CC_PI) {
      sweepAngle = (2 * CC_PI + sweepAngle);
    }
  }
  
  return vcgen_arc(Points, ipos, circleCenterx, circleCentery, newRadius, newRadius, rad2deg(startAngle), rad2deg(sweepAngle));
  }
}
static 
int vcgen_round_poly(FPOINT* Points, int ipos, const FPOINT* pt, int ptlen, BOOL m_closed, double radius)
{
  int i = 0;
  const vertex_dist* v0, *v1, *v2;
  vertex_dist* vpt = (vertex_dist*)pmalloc(sizeof(vertex_dist)*ptlen);
  int vpt_size = vertex_dist_init2(ptlen, pt, m_closed, vpt, NULL);
  if (m_closed) {
    for (i=0; i<vpt_size; ++i) {
      v0 = vpt + (i - 1 + vpt_size) % vpt_size;
      v1 = vpt + i;
      v2 = vpt + (i + 1) % vpt_size;
      ipos = vcgen_round_join_add_arc(Points, ipos, v0, v1, v2, v0->dist, v1->dist, radius);
    }
  } else {
    for (i = 1; i < vpt_size - 1; ++i) {
      ipos = vcgen_round_join_add_arc(Points, ipos, vpt + i - 1, vpt + i, vpt + i +1, vpt[i - 1].dist, vpt[i].dist, radius);
    }
  }
  pfree(vpt);
  return ipos;
}

static const double curve_distance_epsilon = 1e-30;
static const double curve_collinearity_epsilon = 1e-30;
static const double curve_angle_tolerance_epsilon = 0.01;
enum curve_recursion_limit_e { curve_recursion_limit = 32 };
// 3点贝塞尔
CC_INLINE int vcgen_curve3_inc(FPOINT* Points, int ipos, double x1, double y1, double x2, double y2, double x3, double y3)
{
  double dx1, dy1, dx2, dy2, len, subdivide_step1, subdivide_step2, tmpx, tmpy;
  double m_scale = 1., m_fx, m_fy, m_dfx, m_dfy, m_ddfx, m_ddfy;
  int j, n, n1;
  dx1 = x2 - x1, dy1 = y2 - y1;
  dx2 = x3 - x2, dy2 = y3 - y2;
  len = sqrt(dx1 * dx1 + dy1 * dy1) + sqrt(dx2 * dx2 + dy2 * dy2);
  n = uround(len * 0.25 * m_scale);
  n = MAX(n, 4);
  n1 = n + ipos + 1;
  if (NULL == Points) {
    return n1;
  }
  subdivide_step1 = 1.0 / n;
  subdivide_step2 = subdivide_step1 * subdivide_step1;
  tmpx = (x1 - x2 * 2.0 + x3) * subdivide_step2;
  tmpy = (y1 - y2 * 2.0 + y3) * subdivide_step2;
  m_fx = x1, m_fy = y1;
  m_dfx = tmpx + (x2 - x1) * (2.0 * subdivide_step1);
  m_dfy = tmpy + (y2 - y1) * (2.0 * subdivide_step1);
  m_ddfx = tmpx * 2.0, m_ddfy = tmpy * 2.0;
  Points[ipos] = fPOINT(x1, y1);
  for (j = ipos + 1; j < n1; ++j) {
    m_fx += m_dfx, m_fy += m_dfy;
    m_dfx += m_ddfx, m_dfy += m_ddfy;
    Points[j] = fPOINT(m_fx, m_fy);
  }
  Points[j] = fPOINT(x3, y3);
  return j + 1;
}
typedef struct _curve_div_t {
  double m_approximation_scale;
  double m_distance_tolerance_square;
  double m_angle_tolerance;
  double m_cusp_limit;
} curve_div_t;

CC_INLINE int curve3_div_recursive_bezier(FPOINT* Points, int ipos, double x1, double y1, double x2, double y2, double x3, double y3, unsigned level, curve_div_t* cv)
{
  // Calculate all the mid-points of the line segments
  double x12 = (x1 + x2) / 2;
  double y12 = (y1 + y2) / 2;
  double x23 = (x2 + x3) / 2;
  double y23 = (y2 + y3) / 2;
  double x123 = (x12 + x23) / 2;
  double y123 = (y12 + y23) / 2;
  double dx = x3 - x1;
  double dy = y3 - y1;
  double d = fabs(((x2 - x3) * dy - (y2 - y3) * dx));
  double da;
  if (level > curve_recursion_limit) {
    return ipos;
  }
  if (d > curve_collinearity_epsilon) {
    // Regular case
    if (d * d <= cv->m_distance_tolerance_square * (dx * dx + dy * dy)) {
      // If the curvature doesn't exceed the distance_tolerance value
      // we tend to finish subdivisions.
      if (cv->m_angle_tolerance < curve_angle_tolerance_epsilon) {
        vcgen_line_to(x123, y123);
        return ipos;
      }
      // Angle & Cusp Condition
      da = fabs(atan2(y3 - y2, x3 - x2) - atan2(y2 - y1, x2 - x1));
      if (da >= CC_PI) {
        da = 2 * CC_PI - da;
      }
      if (da < cv->m_angle_tolerance) {
        // Finally we can stop the recursion
        vcgen_line_to(x123, y123);
        return ipos;
      }
    }
  }
  else {
    // Collinear case
    da = dx * dx + dy * dy;
    if (da == 0) {
      d = calc_sq_distance(x1, y1, x2, y2);
    }
    else {
      d = ((x2 - x1) * dx + (y2 - y1) * dy) / da;
      if (d > 0 && d < 1) {
        // Simple collinear case, 1---2---3
        // We can leave just two endpoints
        return 0;
      }
      if (d <= 0) {
        d = calc_sq_distance(x2, y2, x1, y1);
      }
      else if (d >= 1) {
        d = calc_sq_distance(x2, y2, x3, y3);
      }
      else {
        d = calc_sq_distance(x2, y2, x1 + d * dx, y1 + d * dy);
      }
    }
    if (d < cv->m_distance_tolerance_square) {
      vcgen_line_to(x2, y2);
      return ipos;
    }
  }
  // Continue subdivision
  ipos = curve3_div_recursive_bezier(Points, ipos, x1, y1, x12, y12, x123, y123, level + 1, cv);
  ipos = curve3_div_recursive_bezier(Points, ipos, x123, y123, x23, y23, x3, y3, level + 1, cv);
  return ipos;
}
// 最多32个点
CC_INLINE int vcgen_curve3_div(FPOINT* Points, int ipos, double x1, double y1, double x2, double y2, double x3, double y3)
{
  curve_div_t cv[1] = {0};
  cv->m_angle_tolerance = 0.2;
  cv->m_approximation_scale = 1.;
  cv->m_distance_tolerance_square = 0.5 / cv->m_approximation_scale;
  cv->m_distance_tolerance_square *= cv->m_distance_tolerance_square;
  vcgen_line_to(x1, y1);
  curve3_div_recursive_bezier(Points, ipos, x1, y1, x2, y2, x3, y3, 0, cv);
  vcgen_line_to(x3, y3);
  return ipos;
}
CC_INLINE int vcgen_curve4_inc(FPOINT* Points, int ipos, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
  int m_num_steps;
  int m_step;
  double m_scale = 1.;
  double m_fx, m_dfx, m_ddfx, m_dddfx;
  double m_fy, m_dfy, m_ddfy, m_dddfy;
  double subdivide_step1;
  double subdivide_step2;
  double subdivide_step3;
  double pre1, pre2, pre4, pre5;
  double tmp1x, tmp2x;
  double tmp1y, tmp2y;
  double dx1 = x2 - x1, dy1 = y2 - y1;
  double dx2 = x3 - x2, dy2 = y3 - y2;
  double dx3 = x4 - x3, dy3 = y4 - y3;
  double len = (sqrt(dx1 * dx1 + dy1 * dy1) + sqrt(dx2 * dx2 + dy2 * dy2) + sqrt(dx3 * dx3 + dy3 * dy3)) * 0.25 * m_scale;
  m_num_steps = uround(len);
  if (m_num_steps < 4) {
    m_num_steps = 4;
  }
  subdivide_step1 = 1.0 / m_num_steps;
  subdivide_step2 = subdivide_step1 * subdivide_step1;
  subdivide_step3 = subdivide_step1 * subdivide_step1 * subdivide_step1;
  pre1 = 3.0 * subdivide_step1;
  pre2 = 3.0 * subdivide_step2;
  pre4 = 6.0 * subdivide_step2;
  pre5 = 6.0 * subdivide_step3;
  tmp1x = x1 - x2 * 2.0 + x3;
  tmp1y = y1 - y2 * 2.0 + y3;
  tmp2x = (x2 - x3) * 3.0 - x1 + x4;
  tmp2y = (y2 - y3) * 3.0 - y1 + y4;
  m_fx = x1, m_fy = y1;
  m_dfx = (x2 - x1) * pre1 + tmp1x * pre2 + tmp2x * subdivide_step3;
  m_dfy = (y2 - y1) * pre1 + tmp1y * pre2 + tmp2y * subdivide_step3;
  m_ddfx = tmp1x * pre4 + tmp2x * pre5;
  m_ddfy = tmp1y * pre4 + tmp2y * pre5;
  m_dddfx = tmp2x * pre5;
  m_dddfy = tmp2y * pre5;
  vcgen_line_to(x1, y1);
  for (m_step = 1; m_step < m_num_steps; ++m_step) {
    m_fx += m_dfx, m_fy += m_dfy;
    m_dfx += m_ddfx, m_dfy += m_ddfy;
    m_ddfx += m_dddfx, m_ddfy += m_dddfy;
    vcgen_line_to(m_fx, m_fy);
  }
  vcgen_line_to(x4, y4);
  return ipos;
}
CC_INLINE int curve4_div_recursive_bezier(FPOINT* Points, int ipos, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, unsigned level, curve_div_t* cv)
{
  // Calculate all the mid-points of the line segments
  double x12 = (x1 + x2) / 2;
  double y12 = (y1 + y2) / 2;
  double x23 = (x2 + x3) / 2;
  double y23 = (y2 + y3) / 2;
  double x34 = (x3 + x4) / 2;
  double y34 = (y3 + y4) / 2;
  double x123 = (x12 + x23) / 2;
  double y123 = (y12 + y23) / 2;
  double x234 = (x23 + x34) / 2;
  double y234 = (y23 + y34) / 2;
  double x1234 = (x123 + x234) / 2;
  double y1234 = (y123 + y234) / 2;
  // Try to approximate the full cubic curve by a single straight line
  //------------------
  double dx = x4 - x1;
  double dy = y4 - y1;
  double d2 = fabs(((x2 - x4) * dy - (y2 - y4) * dx));
  double d3 = fabs(((x3 - x4) * dy - (y3 - y4) * dx));
  double da1, da2, k;
  if (level > curve_recursion_limit) {
    return ipos;
  }
  switch (((int)(d2 > curve_collinearity_epsilon) << 1) + (int)(d3 > curve_collinearity_epsilon)) {
  case 0:
    // All collinear OR p1==p4
    //----------------------
    k = dx * dx + dy * dy;
    if (k == 0) {
      d2 = calc_sq_distance(x1, y1, x2, y2);
      d3 = calc_sq_distance(x4, y4, x3, y3);
    }
    else {
      k = 1 / k;
      da1 = x2 - x1;
      da2 = y2 - y1;
      d2 = k * (da1 * dx + da2 * dy);
      da1 = x3 - x1;
      da2 = y3 - y1;
      d3 = k * (da1 * dx + da2 * dy);
      if (d2 > 0 && d2 < 1 && d3 > 0 && d3 < 1) {
        // Simple collinear case, 1---2---3---4
        // We can leave just two endpoints
        return 0;
      }
      if (d2 <= 0) {
        d2 = calc_sq_distance(x2, y2, x1, y1);
      }
      else if (d2 >= 1) {
        d2 = calc_sq_distance(x2, y2, x4, y4);
      }
      else {
        d2 = calc_sq_distance(x2, y2, x1 + d2 * dx, y1 + d2 * dy);
      }
      if (d3 <= 0) {
        d3 = calc_sq_distance(x3, y3, x1, y1);
      }
      else if (d3 >= 1) {
        d3 = calc_sq_distance(x3, y3, x4, y4);
      }
      else {
        d3 = calc_sq_distance(x3, y3, x1 + d3 * dx, y1 + d3 * dy);
      }
    }
    if (d2 > d3) {
      if (d2 < cv->m_distance_tolerance_square) {
        vcgen_line_to(x2, y2);
        return ipos;
      }
    }
    else {
      if (d3 < cv->m_distance_tolerance_square) {
        vcgen_line_to(x3, y3);
        return ipos;
      }
    }
    break;
  case 1:
    // p1,p2,p4 are collinear, p3 is significant
    //----------------------
    if (d3 * d3 <= cv->m_distance_tolerance_square * (dx * dx + dy * dy)) {
      if (cv->m_angle_tolerance < curve_angle_tolerance_epsilon) {
        vcgen_line_to(x23, y23);
        return ipos;
      }
      // Angle Condition
      //----------------------
      da1 = fabs(atan2(y4 - y3, x4 - x3) - atan2(y3 - y2, x3 - x2));
      if (da1 >= CC_PI) {
        da1 = 2 * CC_PI - da1;
      }
      if (da1 < cv->m_angle_tolerance) {
        vcgen_line_to(x2, y2);
        vcgen_line_to(x3, y3);
        return ipos;
      }
      if (cv->m_cusp_limit != 0.0) {
        if (da1 > cv->m_cusp_limit) {
          vcgen_line_to(x3, y3);
          return ipos;
        }
      }
    }
    break;
  case 2:
    // p1,p3,p4 are collinear, p2 is significant
    //----------------------
    if (d2 * d2 <= cv->m_distance_tolerance_square * (dx * dx + dy * dy)) {
      if (cv->m_angle_tolerance < curve_angle_tolerance_epsilon) {
        vcgen_line_to(x23, y23);
          return ipos;
      }
      // Angle Condition
      //----------------------
      da1 = fabs(atan2(y3 - y2, x3 - x2) - atan2(y2 - y1, x2 - x1));
      if (da1 >= CC_PI) {
        da1 = 2 * CC_PI - da1;
      }
      if (da1 < cv->m_angle_tolerance) {
        vcgen_line_to(x2, y2);
        vcgen_line_to(x3, y3);
          return ipos;
      }
      if (cv->m_cusp_limit != 0.0) {
        if (da1 > cv->m_cusp_limit) {
          vcgen_line_to(x2, y2);
          return ipos;
        }
      }
    }
    break;
  case 3:
    // Regular case
    //-----------------
    if ((d2 + d3) * (d2 + d3) <= cv->m_distance_tolerance_square * (dx * dx + dy * dy)) {
      // If the curvature doesn't exceed the distance_tolerance value
      // we tend to finish subdivisions.
      //----------------------
      if (cv->m_angle_tolerance < curve_angle_tolerance_epsilon) {
        vcgen_line_to(x23, y23);
          return ipos;
      }
      // Angle & Cusp Condition
      //----------------------
      k = atan2(y3 - y2, x3 - x2);
      da1 = fabs(k - atan2(y2 - y1, x2 - x1));
      da2 = fabs(atan2(y4 - y3, x4 - x3) - k);
      if (da1 >= CC_PI) {
        da1 = 2 * CC_PI - da1;
      }
      if (da2 >= CC_PI) {
        da2 = 2 * CC_PI - da2;
      }
      if (da1 + da2 < cv->m_angle_tolerance) {
        // Finally we can stop the recursion
        //----------------------
        vcgen_line_to(x23, y23);
          return ipos;
      }
      if (cv->m_cusp_limit != 0.0) {
        if (da1 > cv->m_cusp_limit) {
          vcgen_line_to(x2, y2);
          return ipos;
        }
        if (da2 > cv->m_cusp_limit) {
          vcgen_line_to(x3, y3);
          return ipos;
        }
      }
    }
    break;
  }
  // Continue subdivision
  //----------------------
  ipos = curve4_div_recursive_bezier(Points, ipos, x1, y1, x12, y12, x123, y123, x1234, y1234, level + 1, cv);
  ipos = curve4_div_recursive_bezier(Points, ipos, x1234, y1234, x234, y234, x34, y34, x4, y4, level + 1, cv);
  return ipos;
}
CC_INLINE int vcgen_curve4_div(FPOINT* Points, int ipos, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
  curve_div_t cv[1] = {0};
  int n = 0;
  cv->m_angle_tolerance = 0.2;
  cv->m_approximation_scale = 1.;
  cv->m_distance_tolerance_square = 0.5 / cv->m_approximation_scale;
  cv->m_distance_tolerance_square *= cv->m_distance_tolerance_square;
  vcgen_line_to(x1, y1);
  ipos = curve4_div_recursive_bezier(Points, ipos, x1, y1, x2, y2, x3, y3, x4, y4, 0, cv);
  vcgen_line_to(x4, y4);
  return n;
}
CC_INLINE int vcgen_curve(FPOINT* Points, int ipos, int npt, const FPOINT* pt)
{
  if (3 == npt) {
    ipos = vcgen_curve3_inc(Points, ipos, pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y);
  }
  else if (4 == npt) {
    ipos = vcgen_curve4_inc(Points, ipos, pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y, pt[3].x, pt[3].y);
  }
  else {
    if (Points) {
      memcpy(Points + ipos, pt, npt * sizeof(FPOINT));
    }
    ipos += npt;
  }
  return ipos;
}
CC_INLINE int vcgen_arrowhead_head(FPOINT* Points, int ipos, double head_d1, double head_d2, double head_d3, double head_d4) {
  if (Points) {
    FPOINT* coord = Points + ipos;
    coord[0].x  = -head_d1;
    coord[0].y  = 0.0;
    coord[1].x  = head_d2 + head_d4;
    coord[1].y  = -head_d3;
    coord[2].x  = head_d2;
    coord[2].y  = 0.0;
    coord[3].x  = head_d2 + head_d4;
    coord[3].y  = head_d3;
  }
  ipos += 4;
  return ipos;
}
CC_INLINE int vcgen_arrowhead_tail(FPOINT* Points, int ipos, double tail_d1, double tail_d2, double tail_d3, double tail_d4) {
  if (Points) {
    FPOINT* coord = Points + ipos;
    coord[0].x  =  tail_d1;
    coord[0].y  =  0.0;
    coord[1].x  =  tail_d1 - tail_d4;
    coord[1].y  =  tail_d3;
    coord[2].x  = -tail_d2 - tail_d4;
    coord[2].y  =  tail_d3;
    coord[3].x  = -tail_d2;
    coord[3].y  =  0.0;
    coord[4].x  = -tail_d2 - tail_d4;
    coord[4].y  = -tail_d3;
    coord[5].x =  tail_d1 - tail_d4;
    coord[5].y = -tail_d3;
  }
  return 6;
}
static int trans_marker(FPOINT* pt, int n, double x1, double y1, double x2, double y2) {
  float m_mtx[6];
  double a = atan2(y2 - y1, x2 - x1);
  a = rad2deg(a);
  M23Set(m_mtx);
  M23Rotate(m_mtx, a, MatrixOrderAppend);
  M23Translate(m_mtx, x1, y1, MatrixOrderAppend);
  M23TransformPoints(m_mtx, pt, n);
  return 0;
}
static int trans_marker2(const FPOINT* pt, int n, FPOINT* pt1, int len1, FPOINT* pt2, int len2) {
  if (pt1) {
    //trans_marker(pt1, len1, pt[n-2].x, pt[n-2].y, pt[n-1].x, pt[n-1].y);
    trans_marker(pt1, len1, pt[n-1].x, pt[n-1].y, pt[n-2].x, pt[n-2].y);
    //trans_marker(pt1, len1, pt[0].x, pt[0].y, pt[1].x, pt[1].y);
  }
  if (pt2) {
    trans_marker(pt2, len2, pt[0].x, pt[0].y, pt[1].x, pt[1].y);
    //trans_marker(pt2, len2, pt[n-1].x, pt[n-1].y, pt[n-2].x, pt[n-2].y);
  }
  return 0;
}

static int vcgen_path_point(FPOINT* Points, BYTE* Types, int ipos, const path_point* pp, int npp)
{
  int i;
  if (Points) {
    for (i=0; i<npp; ++i) {
      Points[ipos + i].x = pp[i].x;
      Points[ipos + i].y = pp[i].y;
      Types[ipos + i] = pp[i].cmd;
    }
  }
  return ipos + npp;
}
static double vertex_dist_init(int n, const FPOINT* pt, vertex_dist* vpt)
{
  int i, ret;
  double dist, d;
  double m_kindex;
  FPOINT val;
  vpt[0].x = pt[0].x;
  vpt[0].y = pt[0].y;
  vpt[0].dist = 0;
  val = pt[0];
  for (i = 1; i < n; ++i) {
    vpt[i].x = pt[i].x;
    vpt[i].y = pt[i].y;
    ret = (vpt[i].dist = calc_distance(pt[i].x, pt[i].y, val.x, val.y)) > vertex_dist_epsilon;
    if (!ret) {
      vpt[i].dist = 1.0 / vertex_dist_epsilon;
    }
    val = pt[i];
  }
  if (n > 2) {
    if (vpt[n - 2].dist * 10.0 < vpt[n - 3].dist) {
      d = vpt[n - 3].dist + vpt[n - 2].dist;
      vpt[n - 2] = vpt[n - 1];
      vpt[n - 2].dist = d;
    }
  }
  dist = 0.0;
  for (i = 0; i < n; i++) {
    vertex_dist* v = vpt+i;
    d = v->dist;
    v->dist = dist;
    dist += d;
  }
  m_kindex = (n - 1) / dist;
  return m_kindex;
}
static int trans_single_path_transform(int n, const vertex_dist* vpt, double m_base_length, double m_kindex, BOOL m_preserve_x_scale, double* x, double* y)
{
  int i, j, k;
  double x1 = 0.0, y1 = 0.0, dx = 1.0, dy = 1.0, d = 0.0, dd = 1.0, x2, y2;
  if (m_base_length > 1e-10) {
    *x *= vpt[n - 1].dist / m_base_length;
  }
  if (*x < 0.0) {
    // Extrapolation on the left
    x1 = vpt[0].x;
    y1 = vpt[0].y;
    dx = vpt[1].x - x1;
    dy = vpt[1].y - y1;
    dd = vpt[1].dist - vpt[0].dist;
    d = *x;
  }
  else if (*x > vpt[n - 1].dist) {
    // Extrapolation on the right
    i = n - 2;
    j = n - 1;
    x1 = vpt[j].x;
    y1 = vpt[j].y;
    dx = x1 - vpt[i].x;
    dy = y1 - vpt[i].y;
    dd = vpt[j].dist - vpt[i].dist;
    d = *x - vpt[j].dist;
  }
  else {
    // Interpolation
    i = 0;
    j = n - 1;
    if (m_preserve_x_scale) {
      for (i = 0; (j - i) > 1;) {
        if (*x < vpt[k = (i + j) >> 1].dist) {
          j = k;
        }
        else {
          i = k;
        }
      }
      d = vpt[i].dist;
      dd = vpt[j].dist - d;
      d = *x - d;
    }
    else {
      i = (unsigned)(*x * m_kindex);
      j = i + 1;
      dd = vpt[j].dist - vpt[i].dist;
      d = ((*x * m_kindex) - i) * dd;
    }
    x1 = vpt[i].x;
    y1 = vpt[i].y;
    dx = vpt[j].x - x1;
    dy = vpt[j].y - y1;
  }
  x2 = x1 + dx * d / dd;
  y2 = y1 + dy * d / dd;
  *x = x2 - *y * dy / dd;
  *y = y2 + *y * dx / dd;
  return 0;
}

static int trans_single_path(int n, const FPOINT* pt, double m_base_length, BOOL m_preserve_x_scale, FPOINT* Points, int Count) {
  vertex_dist* vpt = (vertex_dist*)pmalloc(sizeof(vertex_dist)*n);
  double m_kindex = vertex_dist_init(n, pt, vpt);
  int i;
  for (i=0; i<Count; ++i) {
    double x = Points[i].x;
    double y = Points[i].y;
    trans_single_path_transform(n, vpt, 0, m_kindex, true, &x, &y);
    Points[i].x = x;
    Points[i].y = y;
  }
  pfree(vpt);
  return 0;
}

//------------------------------------------------------------------------
static void trans_double_path_transform1(int vertices_size, const vertex_dist* vertices, double kindex, double kx, BOOL m_preserve_x_scale, double *x, double* y)
{
  double x1 = 0.0;
  double y1 = 0.0;
  double dx = 1.0;
  double dy = 1.0;
  double d  = 0.0;
  double dd = 1.0;
  *x *= kx;
  if(*x < 0.0)
  {
    // Extrapolation on the left
    //--------------------------
    x1 = vertices[0].x;
    y1 = vertices[0].y;
    dx = vertices[1].x - x1;
    dy = vertices[1].y - y1;
    dd = vertices[1].dist - vertices[0].dist;
    d  = *x;
  }
  else
    if(*x > vertices[vertices_size - 1].dist)
    {
      // Extrapolation on the right
      //--------------------------
      unsigned i = vertices_size - 2;
      unsigned j = vertices_size - 1;
      x1 = vertices[j].x;
      y1 = vertices[j].y;
      dx = x1 - vertices[i].x;
      dy = y1 - vertices[i].y;
      dd = vertices[j].dist - vertices[i].dist;
      d  = *x - vertices[j].dist;
    }
    else
    {
      // Interpolation
      //--------------------------
      unsigned i = 0;
      unsigned j = vertices_size - 1;
      if(m_preserve_x_scale)
      {
        unsigned k;
        for(i = 0; (j - i) > 1; ) 
        {
          if(*x < vertices[k = (i + j) >> 1].dist) 
          {
            j = k; 
          }
          else 
          {
            i = k;
          }
        }
        d  = vertices[i].dist;
        dd = vertices[j].dist - d;
        d  = *x - d;
      }
      else
      {
        i = (unsigned)(*x * kindex);
        j = i + 1;
        dd = vertices[j].dist - vertices[i].dist;
        d = ((*x * kindex) - i) * dd;
      }
      x1 = vertices[i].x;
      y1 = vertices[i].y;
      dx = vertices[j].x - x1;
      dy = vertices[j].y - y1;
    }
    *x = x1 + dx * d / dd;
    *y = y1 + dy * d / dd;
}


static int trans_double_path(int n1, const FPOINT* pt1, int n2, const FPOINT* pt2, double m_base_length, double m_base_height, BOOL m_preserve_x_scale, FPOINT* Points, int Count) {
  vertex_dist* vpt1 = (vertex_dist*)pmalloc(sizeof(vertex_dist)*(n1+n2));
  vertex_dist* vpt2 = vpt1 + n1;
  double m_kindex1 = vertex_dist_init(n1, pt1, vpt1);
  double m_kindex2 = vertex_dist_init(n2, pt2, vpt2);
  double x, y, x1, y1, x2, y2, dd;
  int i;
  m_base_height = MAX(m_base_height, 1);
  for (i=0; i<Count; ++i) {
    x = Points[i].x;
    y = Points[i].y;
    if(m_base_length > 1e-10) {
      x *= vpt1[n1 - 1].dist / m_base_length;
    }
    x1 = x;
    y1 = y;
    x2 = x;
    y2 = y;
    dd = vpt2[n2 - 1].dist / vpt1[n1 - 1].dist;
    
    trans_double_path_transform1(n1, vpt1, m_kindex1, 1.0, m_preserve_x_scale, &x1, &y1);
    trans_double_path_transform1(n1, vpt2, m_kindex2, dd,  m_preserve_x_scale, &x2, &y2);
    
    Points[i].x = x1 + y * (x2 - x1) / m_base_height;
    Points[i].y = y1 + y * (y2 - y1) / m_base_height;
  }
  pfree(vpt1);
  return 0;
}
//------------------------------------------------------------------------
static void vcgen_smooth_poly1_calculate(const vertex_dist* v0,const vertex_dist* v1,const vertex_dist* v2,const vertex_dist* v3, double m_smooth_value, FPOINT* m_ctrl1, FPOINT* m_ctrl2)
{
  double k1 = v0->dist / (v0->dist + v1->dist);
  double k2 = v1->dist / (v1->dist + v2->dist);
  
  double xm1 = v0->x + (v2->x - v0->x) * k1;
  double ym1 = v0->y + (v2->y - v0->y) * k1;
  double xm2 = v1->x + (v3->x - v1->x) * k2;
  double ym2 = v1->y + (v3->y - v1->y) * k2;
  
  m_ctrl1->x = v1->x + m_smooth_value * (v2->x - xm1);
  m_ctrl1->y = v1->y + m_smooth_value * (v2->y - ym1);
  m_ctrl2->x = v2->x + m_smooth_value * (v1->x - xm2);
  m_ctrl2->y = v2->y + m_smooth_value * (v1->y - ym2);
}
static int vcgen_smooth_poly1(int n, const FPOINT* pt1, BOOL m_closed, double m_smooth_value, FPOINT* Points, BYTE* Types, int i)
{
  vertex_dist* vpt = (vertex_dist*)pmalloc(sizeof(vertex_dist)*n);
  //vertex_storage vpt;
  //double         m_smooth_value = 0.5;
  //status_e       m_status;
  int       j = 0;
  FPOINT m_ctrl1;
  FPOINT m_ctrl2;
  BYTE closeflag = m_closed ? PathPointTypeCloseSubpath : 0;
  n = vertex_dist_init2(n, pt1, m_closed, vpt, NULL);
  
  if(n <  2) {
    return 0;
  }
  if(n == 2)
  {
    if (Points) {
      Points[0].x = vpt[0].x;
      Points[0].y = vpt[0].y;
      Points[1].x = vpt[1].x;
      Points[1].y = vpt[1].y;
      Types[0] = PathPointTypeStart;
      Types[1] = PathPointTypeLine | closeflag;
    }
    return 2;
  }
  
  if(m_closed) {
    for (j = 0; j<n; ++j) {
      vcgen_smooth_poly1_calculate(vpt + (j+n-1)%n, vpt + (j+n)%n,
        vpt + (j+n+1)%n, vpt + (j+n+2)%n, m_smooth_value, &m_ctrl1, &m_ctrl2);
      if (Points) {
        Points[i].x = vpt[j].x;
        Points[i].y = vpt[j].y;
        Points[i + 1] = m_ctrl1;
        Points[i + 2] = m_ctrl2;
        Types[i] = ((j == 0) ? PathPointTypeStart : PathPointTypeLine);
        Types[i + 1] = PathPointTypeBezier3;
        Types[i + 2] = PathPointTypeBezier3;
      }
      i+=3;
    }
    if (Points) {
      Types[i-1] |= PathPointTypeCloseSubpath;
    }
  } else {
    for (j = 0; j<n-1; ++j) {
      vcgen_smooth_poly1_calculate(vpt + (j+n-1)%n, vpt + (j+n)%n,
        vpt + (j+n+1)%n, vpt + (j+n+2)%n, m_smooth_value, &m_ctrl1, &m_ctrl2);
      if (j==0) {
        if (Points) {
          Points[i].x = vpt[j].x;
          Points[i].y = vpt[j].y;
          Points[i + 1] = m_ctrl2;
          Types[i] = PathPointTypeStart;
          Types[i + 1] = PathPointTypeBezier2;
        }
        i+=2;
      } else if (j<(n-2)) {
        if (Points) {
          Points[i].x = vpt[j].x;
          Points[i].y = vpt[j].y;
          Points[i + 1] = m_ctrl1;
          Points[i + 2] = m_ctrl2;
          Types[i] = PathPointTypeLine;
          Types[i + 1] = PathPointTypeBezier3;
          Types[i + 2] = PathPointTypeBezier3;
        }
        i+=3;
      } else {
        if (Points) {
          Points[i].x = vpt[j].x;
          Points[i].y = vpt[j].y;
          Points[i + 1] = m_ctrl1;
          Points[i + 2].x = vpt[j+1].x;
          Points[i + 2].y = vpt[j+1].y;
          Types[i] = PathPointTypeLine;
          Types[i + 1] = PathPointTypeBezier2;
          Types[i + 2] = PathPointTypeLine;
        }
        i+=3;
      }
    }
  }
  
  pfree(vpt);
  return i;
}

#define path_point_set(pp, C, X, Y) ((pp)->cmd=C,(pp)->x=X,(pp)->y=Y)
// bounding_rect_single
CC_INLINE BOOL path_bounding_rect(path_point* pp, int n, FPOINT* pt)
{
  int i;
  pt[0] = fPOINT(pp[0].x, pp[0].y);
  pt[1] = fPOINT(pp[0].x, pp[0].y);
  for (i = 1; i < n; ++i) {
    pt[0].x = MIN(pt[0].x, pp[i].x);
    pt[0].y = MIN(pt[0].y, pp[i].y);
    pt[1].x = MAX(pt[1].x, pp[i].x);
    pt[1].y = MAX(pt[1].y, pp[i].y);
  }
  return pt[0].x <= pt[1].x && pt[0].y <= pt[1].y;
}
CC_INLINE int point_setstr(const char* s0, char** ps, const DPOINT* ptbase, DPOINT* pt, int isref)
{
  char* s = (char*)s0;
  double d;
  if (isdigit(*s) || '+' == *s || '-' == *s) {
    if (isref) {
      d = strtod(s, &s);
      pt->x = ptbase->x + d;
      for (; *s == ' ' || *s == '\t' || *s == '\n' || *s == '\r' || *s == ','; ++s);
      d = strtod(s, &s);
      for (; *s == ' ' || *s == '\t' || *s == '\n' || *s == '\r' || *s == ','; ++s);
      pt->y = ptbase->y + d;
    }
    else {
      d = strtod(s, &s);
      pt->x = d;
      for (; *s == ' ' || *s == '\t' || *s == '\n' || *s == '\r' || *s == ','; ++s);
      d = strtod(s, &s);
      for (; *s == ' ' || *s == '\t' || *s == '\n' || *s == '\r' || *s == ','; ++s);
      pt->y = d;
    }
    *ps = s;
    return 1;
  }
  return 0;
}
#if 0

CC_INLINE int vcgen_path_str(const char* str, path_point* pp, int n)
{
  int i = 0, cmd = 0, isref = 0;
  char* s = (char*)str;
  DPOINT pt, ptbase = {0}, pts[5];
  for (; *s;) {
    if ('M' == *s || 'm' == *s) {
      isref = 'm' == *s++;
      point_setstr(s, &s, &ptbase, &pt, isref);
      path_point_set(pp + i, path_move_to, pt.x, pt.y), ++i;
      ptbase = pt;
    }
    else if ('L' == *s || 'l' == *s) {
      isref = 'l' == *s++;
      point_setstr(s, &s, &ptbase, &pt, isref);
      path_point_set(pp + i, path_line_to, pt.x, pt.y), ++i;
      ptbase = pt;
    }
    else if ('H' == *s || 'h' == *s) {
      isref = 'h' == *s++;
      point_setstr(s, &s, &ptbase, &pt, isref);
      pt.y = ptbase.y;
      path_point_set(pp + i, path_hline_to, pt.x, pt.y), ++i;
      ptbase = pt;
    }
    else if ('V' == *s || 'v' == *s) {
      isref = 'v' == *s++;
      pt.x = ptbase.x;
      pt.y = strtod(s, &s);
      if (isref) {
        pt.y += ptbase.y;
      }
      path_point_set(pp + i, path_vline_to, pt.x, pt.y), ++i;
      ptbase = pt;
    }
    else if ('Z' == *s || 'z' == *s) {
      ++s;
      path_point_set(pp + i, path_close, 0, 0), ++i;
    }
    else if ('C' == *s || 'c' == *s) {
      isref = 'c' == *s++;
      cmd = path_curve3;
      point_setstr(s, &s, &ptbase, &pts[0], isref);
      point_setstr(s, &s, &ptbase, &pts[1], isref);
      if (point_setstr(s, &s, &ptbase, &pts[2], isref)) {
        cmd = path_curve4;
      }
      path_point_set(pp + i, cmd, pts[0].x, pts[0].y), ++i;
      path_point_set(pp + i, cmd, pts[1].x, pts[1].y), ++i;
      ptbase = pts[1];
      if (cmd == path_curve4) {
        path_point_set(pp + i, cmd, pts[2].x, pts[2].y), ++i;
        ptbase = pts[2];
      }
    }
    else {
      ASSERT(0);
    }
  }
  return i;
}
#endif
// bspline
//////////////////////////////////////////////////////////////////////////////////
// 输出系数 m_am[m_num]
CC_INLINE int bspline_set(int m_num, const double* m_x, const double* m_y, double* m_am)
{
  int m_last_idx = -1;
  if (m_num > 2) {
    double* al = 0;
    int i, k, n1;
    double* temp;
    double* r;
    double* s;
    double h, p, d, f, e;
    for (k = 0; k < m_num; k++) {
      m_am[k] = 0.0;
    }
    n1 = 3 * m_num;
    al = MALLOC(double, n1);
    temp = &al[0];
    for (k = 0; k < n1; k++) {
      temp[k] = 0.0;
    }
    r = temp + m_num;
    s = temp + m_num * 2;
    n1 = m_num - 1;
    d = m_x[1] - m_x[0];
    e = (m_y[1] - m_y[0]) / d;
    for (k = 1; k < n1; k++) {
      h = d;
      d = m_x[k + 1] - m_x[k];
      f = e;
      e = (m_y[k + 1] - m_y[k]) / d;
      al[k] = d / (d + h);
      r[k] = 1.0 - al[k];
      s[k] = 6.0 * (e - f) / (h + d);
    }
    for (k = 1; k < n1; k++) {
      p = 1.0 / (r[k] * al[k - 1] + 2.0);
      al[k] *= -p;
      s[k] = (s[k] - r[k] * s[k - 1]) * p;
    }
    m_am[n1] = 0.0;
    al[n1 - 1] = s[n1 - 1];
    m_am[n1 - 1] = al[n1 - 1];
    for (k = n1 - 2, i = 0; i < m_num - 2; i++, k--) {
      al[k] = al[k] * al[k + 1] + s[k];
      m_am[k] = al[k];
    }
    FREE(al);
  }
  return 0;
}
CC_INLINE void bspline_bsearch(int n, const double* x, double x0, int* i)
{
  int j = n - 1;
  int k;
  for (*i = 0; (j - *i) > 1;) {
    if (x0 < x[k = (*i + j) >> 1]) {
      j = k;
    }
    else {
      *i = k;
    }
  }
}
CC_INLINE double bspline_interpolation(int m_num, const double* m_x, const double* m_y, const double* m_am, double x, int i)
{
  int j = i + 1;
  double d = m_x[i] - m_x[j];
  double h = x - m_x[j];
  double r = m_x[i] - x;
  double p = d * d / 6.0;
  return (m_am[j] * r * r * r + m_am[i] * h * h * h) / 6.0 / d +
      ((m_y[j] - m_am[j] * p) * r + (m_y[i] - m_am[i] * p) * h) / d;
}
CC_INLINE double bspline_extrapolation_left(int m_num, const double* m_x, const double* m_y, const double* m_am, double x)
{
  double d = m_x[1] - m_x[0];
  return (-d * m_am[1] / 6 + (m_y[1] - m_y[0]) / d) * (x - m_x[0]) + m_y[0];
}
CC_INLINE double bspline_extrapolation_right(int m_num, const double* m_x, const double* m_y, const double* m_am, double x)
{
  double d = m_x[m_num - 1] - m_x[m_num - 2];
  double ret = (d * m_am[m_num - 2] / 6 + (m_y[m_num - 1] - m_y[m_num - 2]) / d) *
      (x - m_x[m_num - 1]) + m_y[m_num - 1];
  return ret;
}
CC_INLINE double bspline_get(int m_num, const double* m_x, const double* m_y, const double* m_am, double x)
{
  if (m_num > 2) {
    int i;
    // Extrapolation on the left
    if (x < m_x[0]) {
      return bspline_extrapolation_left(m_num, m_x, m_y, m_am, x);
    }
    // Extrapolation on the right
    if (x >= m_x[m_num - 1]) {
      return bspline_extrapolation_right(m_num, m_x, m_y, m_am, x);
    }
    // Interpolation
    bspline_bsearch(m_num, m_x, x, &i);
    return bspline_interpolation(m_num, m_x, m_y, m_am, x, i);
  }
  return 0.0;
}
CC_INLINE double bspline_get_stateful(int m_num, const double* m_x, const double* m_y, const double* m_am, double x, int* pm_last_idx)
{
  if (m_num > 2) {
    // Extrapolation on the left
    if (x < m_x[0]) {
      return bspline_extrapolation_left(m_num, m_x, m_y, m_am, x);
    }
    // Extrapolation on the right
    if (x >= m_x[m_num - 1]) {
      return bspline_extrapolation_right(m_num, m_x, m_y, m_am, x);
    }
    if (*pm_last_idx >= 0) {
      // Check if x is not in current range
      if (x < m_x[*pm_last_idx] || x > m_x[*pm_last_idx + 1]) {
        // Check if x between next points (most probably)
        if (*pm_last_idx < m_num - 2 &&
            x >= m_x[*pm_last_idx + 1] &&
            x <= m_x[*pm_last_idx + 2]) {
          ++*pm_last_idx;
        }
        else if (*pm_last_idx > 0 &&
            x >= m_x[*pm_last_idx - 1] &&
            x <= m_x[*pm_last_idx]) {
          // x is between pevious points
          --*pm_last_idx;
        }
        else {
          // Else perform full search
          bspline_bsearch(m_num, m_x, x, &*pm_last_idx);
        }
      }
      return bspline_interpolation(m_num, m_x, m_y, m_am, x, *pm_last_idx);
    }
    else {
      // Interpolation
      bspline_bsearch(m_num, m_x, x, &*pm_last_idx);
      return bspline_interpolation(m_num, m_x, m_y, m_am, x, *pm_last_idx);
    }
  }
  return 0.0;
}
// Class-helper for calculation gamma-correction arrays. A gamma-correction
// array is an array of 256 unsigned chars that determine the actual values
// of Anti-Aliasing for each pixel coverage value from 0 to 255. If all the
// values in the array are equal to its index, i.e. 0,1,2,3,... there's
// no gamma-correction. Class agg::polyfill allows you to use custom
// gamma-correction arrays. You can calculate it using any approach, and
// struct gamma_spline allows you to calculate almost any reasonable shape
// of the gamma-curve with using only 4 values - kx1, ky1, kx2, ky2.
//
// kx2
// +----------------------------------+
// | | | . |
// | | | . | ky2
// | | . ------|
// | | . |
// | | . |
// |----------------.|----------------|
// | . | |
// | . | |
// |-------. | |
// ky1 | . | | |
// | . | | |
// +----------------------------------+
// kx1
//
// Each value can be in range [0...2]. Value 1.0 means one quarter of the
// bounding rectangle. Function values() calculates the curve by these
// 4 values. After calling it one can get the gamma-array with call gamma().
// Class also supports the vertex source interface, i.e rewind() and
// vertex(). It's made for convinience and used in struct gamma_ctrl.
// Before calling rewind/vertex one must set the bounding box
// box() using pixel coordinates.
typedef struct gamma_spline_t {
  unsigned char m_gamma[256];
  double m_x[4];
  double m_y[4];
  double m_am[4];
} gamma_spline_t;
CC_INLINE double gamma_spline_y(const gamma_spline_t* g, double x)
{
  double val;
  if (x < 0.0) {
    x = 0.0;
  }
  if (x > 1.0) {
    x = 1.0;
  }
  val = bspline_get(4, g->m_x, g->m_y, g->m_am, x);
  if (val < 0.0) {
    val = 0.0;
  }
  if (val > 1.0) {
    val = 1.0;
  }
  return val;
}
CC_INLINE void gamma_spline_set_values(gamma_spline_t* g, double kx1, double ky1, double kx2, double ky2)
{
  int i;
  if (kx1 < 0.001) {
    kx1 = 0.001;
  }
  if (kx1 > 1.999) {
    kx1 = 1.999;
  }
  if (ky1 < 0.001) {
    ky1 = 0.001;
  }
  if (ky1 > 1.999) {
    ky1 = 1.999;
  }
  if (kx2 < 0.001) {
    kx2 = 0.001;
  }
  if (kx2 > 1.999) {
    kx2 = 1.999;
  }
  if (ky2 < 0.001) {
    ky2 = 0.001;
  }
  if (ky2 > 1.999) {
    ky2 = 1.999;
  }
  g->m_x[0] = 0.0;
  g->m_y[0] = 0.0;
  g->m_x[1] = kx1 * 0.25;
  g->m_y[1] = ky1 * 0.25;
  g->m_x[2] = 1.0 - kx2 * 0.25;
  g->m_y[2] = 1.0 - ky2 * 0.25;
  g->m_x[3] = 1.0;
  g->m_y[3] = 1.0;
  bspline_set(4, g->m_x, g->m_y, g->m_am);
  for (i = 0; i < 256; i++) {
    g->m_gamma[i] = (unsigned char)(gamma_spline_y(g, (i) / 255.0) * 255.0);
  }
}
CC_INLINE void gamma_spline_get_values(const gamma_spline_t* g, double* kx1, double* ky1, double* kx2, double* ky2)
{
  *kx1 = g->m_x[1] * 4.0;
  *ky1 = g->m_y[1] * 4.0;
  *kx2 = (1.0 - g->m_x[2]) * 4.0;
  *ky2 = (1.0 - g->m_y[2]) * 4.0;
}
CC_INLINE int vcgen_gamma_spline(FPOINT* pt, const gamma_spline_t* g, double x1, double y1, double x2, double y2)
{
  int n = 0;
  double cur_x = 0.0;
  if (x2 > x1) {
    pt[n++] = fPOINT(x1, y2);
    cur_x += 1.0 / (x2 - x1);
    for (; cur_x < 1.0;) {
      double y = gamma_spline_y(g, cur_x) * (y2 - y1);
      pt[n++] = fPOINT(x1 + cur_x * (x2 - x1), y2 - y);
      cur_x += 1.0 / (x2 - x1);
    }
  }
  return n;
}

//////////////////////////////////////////////////////////////////////////////////
// http://zh.wikipedia.org/wiki/%E5%BE%B7%E5%8D%A1%E6%96%AF%E7%89%B9%E9%87%8C%E5%A5%A5%E7%AE%97%E6%B3%95
CC_INLINE int vcgen_bspline(int n, FPOINT* pt, int m, const FPOINT* m_src_vertices, int m_closed)
{
  //bspline m_spline_x;
  //bspline m_spline_y;
  int i, j;
  double* mem;
  double* m_x_am, *m_x_x, *m_x_y;
  double* m_y_am, *m_y_x, *m_y_y;
  double m_cur_abscissa;
  int m_num = m_closed ? m + 8 : m;
  int m_x_last_idx = -1;
  int m_y_last_idx = -1;
  if (m < 2) {
    return 0;
  }
  else if (m == 2) {
    pt[0] = m_src_vertices[0];
    pt[1] = m_src_vertices[1];
    return 2;
  }
  mem = MALLOC(double, m_num * 6);
  m_x_am = mem;
  m_x_x = mem + m_num * 1;
  m_x_y = mem + m_num * 2;
  m_y_am = mem + m_num * 3;
  m_y_x = mem + m_num * 4;
  m_y_y = mem + m_num * 5;
  j = 0;
  if (m_closed) {
    for (j = 0; j < m + 8; ++j) {
      int mm = (m + m - 4 + j) % m;
      m_x_x[j] = j, m_x_y[j] = m_src_vertices[mm].x;
      m_y_x[j] = j, m_y_y[j] = m_src_vertices[mm].y;
    }
  }
  else {
    for (j = 0; j < m; ++j) {
      m_x_x[j] = j, m_x_y[j] = m_src_vertices[j].x;
      m_y_x[j] = j, m_y_y[j] = m_src_vertices[j].y;
    }
  }
  bspline_set(m_num, m_x_x, m_x_y, m_x_am);
  bspline_set(m_num, m_y_x, m_y_y, m_y_am);
  for (i = 0; i < n - 1; ++i) {
    if (m_closed) {
      m_cur_abscissa = 4. + 1. * (m) * i / (n - 1);
    }
    else {
      m_cur_abscissa = 1. * (m - 1) * i / (n - 1);
    }
    pt[i].x = bspline_get_stateful(m_num, m_x_x, m_x_y, m_x_am, m_cur_abscissa, &m_x_last_idx);
    pt[i].y = bspline_get_stateful(m_num, m_y_x, m_y_y, m_y_am, m_cur_abscissa, &m_y_last_idx);
  }
  if (!m_closed) {
    pt[i++] = m_src_vertices[m - 1];
  }
  n = i;
  FREE(mem);
  return n;
}

/* PathFlatten helper */
/*
* Used to recursively flatten single Bezier curve
* Parameters:
*  - start   : pointer to start point node;
*  - (x2, y2): first control point;
*  - (x3, y3): second control point;
*  - end     : pointer to end point node
*  - flatness: admissible error of linear approximation.
*
* Return value:
*  TRUE : success
*  FALSE: out of memory
*
* TODO: used quality criteria should be revised to match native as
*       closer as possible.
*/
static int vcgen_bezier2(FPOINT* Points, BYTE* Types, int ipos, float x1, float y1, float x2, float y2, float x3, float y3, float flatness, int level)
{
  // midpoint
  float mx = (x1 + 2 * x2 + x3) / 4;
  float my = (y1 + 2 * y2 + y3) / 4;
  // versus directly drawn line
  float dx = (x1 + x3) / 2 - mx;
  float dy = (y1 + y3) / 2 - my;
  
  if (level > 16) { // 65536 segments on one curve better be enough!
    return 0;
  }
  if (dx * dx + dy * dy > flatness) {
    ipos = vcgen_bezier2(Points, Types, ipos, x1, y1, (x1 + x2) / 2.0f, (y1 + y2) / 2.0f, mx, my, flatness, level + 1);
    ipos = vcgen_bezier2(Points, Types, ipos, mx, my, (x2 + x3) / 2.0f, (y2 + y3) / 2.0f, x3, y3, flatness, level + 1);
  } else {
    if (Points) {
      Points[ipos].x = mx;
      Points[ipos].y = my;
      Types[ipos] = PathPointTypeLine;
    }
    ++ipos;
  }
  return ipos;
}

static int vcgen_bezier3(FPOINT* Points, BYTE* Types, int ipos, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float flatness)
{
  float mx12 = (x1 + x2) / 2.0;
  float my12 = (y1 + y2) / 2.0;
  float mx23 = (x2 + x3) / 2.0;
  float my23 = (y2 + y3) / 2.0;
  float mx34 = (x3 + x4) / 2.0;
  float my34 = (y3 + y4) / 2.0;

  float mx1223 = (mx12 + mx23) / 2.0;
  float my1223 = (my12 + my23) / 2.0;
  float mx2334 = (mx23 + mx34) / 2.0;
  float my2334 = (my23 + my34) / 2.0;
  float mx = (mx1223 + mx2334) / 2.0;
  float my = (my1223 + my2334) / 2.0;
  float dx13 = (x1 + x3) / 2.0 - mx1223;
  float dy13 = (y1 + y3) / 2.0 - my1223;
  float dx24 = (x2 + x4) / 2.0 - mx2334;
  float dy24 = (y2 + y4) / 2.0 - my2334;
  float dx14 = (x1 + x4) / 2.0 - mx;
  float dy14 = (y1 + y4) / 2.0 - my;
  float d = (dx13*dx13 + dy13*dy13 + dx24*dx24 + dy24*dy24 + dx14*dx14 + dy14*dy14)/3;
  
  /* check flatness as a half of distance between middle point and a linearized path */
  if(d<flatness) {
    /* add a middle point */
    if (Points) {
      Points[ipos].x = mx;
      Points[ipos].y = my;
      Types[ipos] = PathPointTypeLine;
    }
    ++ipos;
  } else {
    /* do the same with halves */
    ipos = vcgen_bezier3(Points, Types, ipos, x1, y1, mx12, my12, mx1223, my1223, mx, my, flatness);
    ipos = vcgen_bezier3(Points, Types, ipos, mx, my, mx2334, my2334, mx34, my34, x4, y4, flatness);
  }  
  return ipos;
}

static int vcgen_flatten(FPOINT* Points1, BYTE* Types1, int ipos, const FPOINT* Points, const BYTE* Types, int Count, float flatness) {
  int i = 1, startidx=0;
  FPOINT pt, pt_prev, pt_next;
  flatness = BOUND(flatness, 0.01, 1);
  pt_prev = pt = Points[0];
  if (Points1) {
    Points1[ipos] = pt;
    Types1[ipos] = PathPointTypeStart;
  }
  ++ipos;
  
  for(; i < Count; ++i) {
    BYTE type = Types[i] & PathPointTypePathTypeMask;
    pt = Points[i];
    
    /* save last start point index */
    if(type == PathPointTypeStart) {
      startidx = i;
    }
    
    /* always add line points and start points */
    switch (type) {
    case PathPointTypeStart:
    case PathPointTypeLine:
      if (Points1) {
        Points1[ipos] = pt;
        Types1[ipos] = Types[i];
      }
      ++ipos;
      pt_prev = pt;
      break;
    case PathPointTypeBezier2:
      if(Types[i] & PathPointTypeCloseSubpath) {
        pt_next = Points[startidx];
      } else {
        pt_next = Points[i+1];
        if (PathPointTypeBezier2==(Types[i+1] & PathPointTypePathTypeMask)) {
          pt_next.x = (pt_next.x + pt.x)/2;
          pt_next.y = (pt_next.y + pt.y)/2;
        }
      }
      /* flatten curve */
      ipos = vcgen_bezier2(Points1, Types1, ipos, pt_prev.x, pt_prev.y, pt.x, pt.y, pt_next.x, pt_next.y, flatness, 0);
#if 0
      if (Points1) {
        Points1[ipos] = pt_next;
        Types1[ipos] = (Types[i] & ~PathPointTypePathTypeMask) | PathPointTypeLine;
      }
      ++ipos;
#endif
      pt_prev = pt_next;
      break;
    case PathPointTypeBezier3:
      if (i<(Count-1)) {
        FPOINT pt1 = Points[i+1];
        if(Types[i+1] & PathPointTypeCloseSubpath) {
          pt_next = Points[startidx];
        } else {
          pt_next = Points[i+2];
          if (PathPointTypeBezier3==(Types[i+2] & PathPointTypePathTypeMask)) {
            pt_next.x = (pt_next.x + pt1.x)/2;
            pt_next.y = (pt_next.y + pt1.y)/2;
          }
        }
        /* flatten curve */
        ipos = vcgen_bezier3(Points1, Types1, ipos, pt_prev.x, pt_prev.y, pt.x, pt.y, pt1.x, pt1.y, pt_next.x, pt_next.y, flatness);
#if 0
        if (Points1) {
          Points1[ipos] = pt_next;
          Types1[ipos] = (Types[i] & ~PathPointTypePathTypeMask) | PathPointTypeLine;
        }
        ++ipos;
#else
        if (Types1) {
          Types1[ipos-1] |= Types[i] & ~PathPointTypePathTypeMask;
        }
#endif
        ++i;
        pt_prev = pt_next;
      }
      break;
    default:
      ASSERT(0);
      break;
    }
  }
  return ipos;
}

#if 0
static void add_bevel_point(const FPOINT *endpoint, const FPOINT *nextpoint,
                            const PenStyle* pen, int right_side, path_list_node_t **last_point)
{
  float segment_dy = nextpoint->y-endpoint->y;
  float segment_dx = nextpoint->x-endpoint->x;
  float segment_length = sqrtf(segment_dy*segment_dy + segment_dx*segment_dx);
  float distance = pen->lineWidth/2.0;
  float bevel_dx, bevel_dy;
  
  if (right_side)
  {
    bevel_dx = -distance * segment_dy / segment_length;
    bevel_dy = distance * segment_dx / segment_length;
  }
  else
  {
    bevel_dx = distance * segment_dy / segment_length;
    bevel_dy = -distance * segment_dx / segment_length;
  }
  
  *last_point = add_path_list_node(*last_point, endpoint->x + bevel_dx,
    endpoint->y + bevel_dy, PathPointTypeLine);
}

static void widen_joint(const FPOINT *p1, const FPOINT *p2, const FPOINT *p3,
                        const PenStyle* pen, path_list_node_t **last_point)
{
  switch (pen->lineJoin)
  {
  case LineJoinMiter:
  case LineJoinMiterClipped:
    if ((p2->x - p1->x) * (p3->y - p1->y) > (p2->y - p1->y) * (p3->x - p1->x))
    {
      float distance = pen->lineWidth/2.0;
      float length_0 = sqrtf((p2->x-p1->x)*(p2->x-p1->x)+(p2->y-p1->y)*(p2->y-p1->y));
      float length_1 = sqrtf((p3->x-p2->x)*(p3->x-p2->x)+(p3->y-p2->y)*(p3->y-p2->y));
      float dx0 = distance * (p2->x - p1->x) / length_0;
      float dy0 = distance * (p2->y - p1->y) / length_0;
      float dx1 = distance * (p3->x - p2->x) / length_1;
      float dy1 = distance * (p3->y - p2->y) / length_1;
      float det = (dy0*dx1 - dx0*dy1);
      float dx = (dx0*dx1*(dx0-dx1) + dy0*dy0*dx1 - dy1*dy1*dx0)/det;
      float dy = (dy0*dy1*(dy0-dy1) + dx0*dx0*dy1 - dx1*dx1*dy0)/det;
      if (dx*dx + dy*dy < pen->miterLimit*pen->miterLimit * distance*distance)
      {
        *last_point = add_path_list_node(*last_point, p2->x + dx, p2->y + dy, PathPointTypeLine);
        break;
      }
      else if (pen->lineJoin == LineJoinMiter)
      {
        static int once;
        if (!once++)
          FIXME("should add a clipped corner\n");
      }
      /* else fall-through */
    }
    /* else fall-through */
  default:
  case LineJoinBevel:
    add_bevel_point(p2, p1, pen, 1, last_point);
    add_bevel_point(p2, p3, pen, 0, last_point);
    break;
  }
}

static void widen_cap(const FPOINT *endpoint, const FPOINT *nextpoint,
                      const PenStyle *pen, LineCap cap, CustomLineCap *custom, int add_first_points,
                      int add_last_point, path_list_node_t **last_point)
{
  switch (cap)
  {
  default:
  case LineCapFlat:
    if (add_first_points)
      add_bevel_point(endpoint, nextpoint, pen, 1, last_point);
    if (add_last_point)
      add_bevel_point(endpoint, nextpoint, pen, 0, last_point);
    break;
  case LineCapSquare:
    {
      float segment_dy = nextpoint->y-endpoint->y;
      float segment_dx = nextpoint->x-endpoint->x;
      float segment_length = sqrtf(segment_dy*segment_dy + segment_dx*segment_dx);
      float distance = pen->lineWidth/2.0;
      float bevel_dx, bevel_dy;
      float extend_dx, extend_dy;
      
      extend_dx = -distance * segment_dx / segment_length;
      extend_dy = -distance * segment_dy / segment_length;
      
      bevel_dx = -distance * segment_dy / segment_length;
      bevel_dy = distance * segment_dx / segment_length;
      
      if (add_first_points)
        *last_point = add_path_list_node(*last_point, endpoint->x + extend_dx + bevel_dx,
        endpoint->y + extend_dy + bevel_dy, PathPointTypeLine);
      
      if (add_last_point)
        *last_point = add_path_list_node(*last_point, endpoint->x + extend_dx - bevel_dx,
        endpoint->y + extend_dy - bevel_dy, PathPointTypeLine);
      
      break;
    }
  case LineCapRound:
    {
      float segment_dy = nextpoint->y-endpoint->y;
      float segment_dx = nextpoint->x-endpoint->x;
      float segment_length = sqrtf(segment_dy*segment_dy + segment_dx*segment_dx);
      float distance = pen->lineWidth/2.0;
      float dx, dy, dx2, dy2;
      const float control_point_distance = (float)(0.5522847498307935); /* 4/3 * (sqrt(2) - 1) */
      
      if (add_first_points)
      {
        dx = -distance * segment_dx / segment_length;
        dy = -distance * segment_dy / segment_length;
        
        dx2 = dx * control_point_distance;
        dy2 = dy * control_point_distance;
        
        /* first 90-degree arc */
        *last_point = add_path_list_node(*last_point, endpoint->x + dy,
          endpoint->y - dx, PathPointTypeLine);
        
        *last_point = add_path_list_node(*last_point, endpoint->x + dy + dx2,
          endpoint->y - dx + dy2, PathPointTypeBezier);
        
        *last_point = add_path_list_node(*last_point, endpoint->x + dx + dy2,
          endpoint->y + dy - dx2, PathPointTypeBezier);
        
        /* midpoint */
        *last_point = add_path_list_node(*last_point, endpoint->x + dx,
          endpoint->y + dy, PathPointTypeBezier);
        
        /* second 90-degree arc */
        *last_point = add_path_list_node(*last_point, endpoint->x + dx - dy2,
          endpoint->y + dy + dx2, PathPointTypeBezier);
        
        *last_point = add_path_list_node(*last_point, endpoint->x - dy + dx2,
          endpoint->y + dx + dy2, PathPointTypeBezier);
        
        *last_point = add_path_list_node(*last_point, endpoint->x - dy,
          endpoint->y + dx, PathPointTypeBezier);
      }
      break;
    }
  }
}

static void widen_open_figure(Path *path, const PenStyle *pen, int start, int end,
                              path_list_node_t **last_point, CustomLineCap *customstart, CustomLineCap *customend)
{
  int i;
  path_list_node_t *prev_point;
  
  if (end <= start)
    return;
  
  prev_point = *last_point;
  
  widen_cap(&path->Points[start], &path->Points[start+1],
    pen, pen->startCap, customstart, FALSE, TRUE, last_point);
  
  for (i=start+1; i<end; i++)
    widen_joint(&path->Points[i-1], &path->Points[i],
    &path->Points[i+1], pen, last_point);
  
  widen_cap(&path->Points[end], &path->Points[end-1],
    pen, pen->endCap, customend, TRUE, TRUE, last_point);
  
  for (i=end-1; i>start; i--)
    widen_joint(&path->Points[i+1], &path->Points[i],
    &path->Points[i-1], pen, last_point);
  
  widen_cap(&path->Points[start], &path->Points[start+1],
    pen, pen->startCap, customstart, TRUE, FALSE, last_point);
  
  prev_point->next->type = PathPointTypeStart;
  (*last_point)->type |= PathPointTypeCloseSubpath;
}

static void widen_closed_figure(Path *path, const PenStyle *pen, int start, int end,
                                path_list_node_t **last_point)
{
  int i;
  path_list_node_t *prev_point;
  
  if (end <= start+1)
    return;
  
  /* left outline */
  prev_point = *last_point;
  
  widen_joint(&path->Points[end], &path->Points[start],
    &path->Points[start+1], pen, last_point);
  
  for (i=start+1; i<end; i++)
    widen_joint(&path->Points[i-1], &path->Points[i],
    &path->Points[i+1], pen, last_point);
  
  widen_joint(&path->Points[end-1], &path->Points[end],
    &path->Points[start], pen, last_point);
  
  prev_point->next->type = PathPointTypeStart;
  (*last_point)->type |= PathPointTypeCloseSubpath;
  
  /* right outline */
  prev_point = *last_point;
  
  widen_joint(&path->Points[start], &path->Points[end],
    &path->Points[end-1], pen, last_point);
  
  for (i=end-1; i>start; i--)
    widen_joint(&path->Points[i+1], &path->Points[i],
    &path->Points[i-1], pen, last_point);
  
  widen_joint(&path->Points[start+1], &path->Points[start],
    &path->Points[end], pen, last_point);
  
  prev_point->next->type = PathPointTypeStart;
  (*last_point)->type |= PathPointTypeCloseSubpath;
}

Status PathWiden(const Path *path, const PenStyle *pen, float flatness, CustomLineCap *customstart, CustomLineCap *customend, Path* path2)
{
  Status status = Ok;
  Path flat_path[1]={0};
  path_list_node_t *points=NULL, *last_point=NULL;
  int i, subpath_start=0, new_length;
  BYTE type;
  
  TRACE("(%p,%p,%p,%0.2f)\n", path, pen, path2, flatness);
  
  if (!path || !pen)
    return InvalidParameter;
  
  if (path->Count <= 1)
    return OutOfMemory;
  
  status = PathFlatten(path, flatness, flat_path);
  
  if (status == Ok && !init_path_list(&points, 314.0, 22.0))
    status = OutOfMemory;
  
  if (status == Ok)
  {
    last_point = points;
    
    if (pen->endCap > LineCapRound)
      FIXME("unimplemented end cap %x\n", pen->endCap);
    
    if (pen->startCap > LineCapRound)
      FIXME("unimplemented start cap %x\n", pen->startCap);
    
    if (pen->dashCap != DashCapFlat)
      FIXME("unimplemented dash cap %d\n", pen->dashCap);
    
    if (pen->lineJoin == LineJoinRound)
      FIXME("unimplemented line join %d\n", pen->lineJoin);
    
    if (pen->dashStyle != DashStyleSolid)
      FIXME("unimplemented dash style %d\n", pen->dashStyle);
    
    if (pen->penAlignment != PenAlignmentCenter)
      FIXME("unimplemented pen alignment %d\n", pen->penAlignment);
    
    for (i=0; i < flat_path->Count; i++)
    {
      type = flat_path->Types[i];
      
      if ((type&PathPointTypePathTypeMask) == PathPointTypeStart)
        subpath_start = i;
      
      if ((type&PathPointTypeCloseSubpath) == PathPointTypeCloseSubpath)
      {
        widen_closed_figure(flat_path, pen, subpath_start, i, &last_point);
      }
      else if (i == flat_path->Count-1 ||
        (flat_path->Types[i+1]&PathPointTypePathTypeMask) == PathPointTypeStart)
      {
        widen_open_figure(flat_path, pen, subpath_start, i, &last_point, customstart, customend);
      }
    }
    
    new_length = path_list_count(points)-1;
    
    if (!lengthen_path(path2, new_length))
      status = OutOfMemory;
  }
  
  if (status == Ok)
  {
    path2->Count = new_length;
    
    last_point = points->next;
    for (i = 0; i < new_length; i++)
    {
      path2->Points[i] = last_point->pt;
      path2->Types[i] = last_point->type;
      last_point = last_point->next;
    }
    
    path2->fill = FillModeWinding;
  }
  
  free_path_list(points);
  PathDelete(flat_path);
  return status;
}
#endif
#endif // _VCGEN_INL_
