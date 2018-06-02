
#define ISINF(x)  (x)>(INF)||(x)<(-INF)


typedef struct {
  const char* name;
  double minv;
  double maxv;
  bool isint;
}
MathParameter_t;

typedef struct {
  MathParameter_t* para;
  int npara;
  int q;
  float arg[20];
  const char* formula;
} CyclicCurve;

typedef int (*CyclicCurveGetPoint_f)(CyclicCurve* c, double x, FPOINT* pt, FPOINT* dpt);

#define MathParameter(type, name, minv, maxv, xx)  { #name, minv, maxv, xx},
#define MathParameterDef(type, name, minv, maxv, xx)  const type name = (type)c->arg[i++];

static int GCD(int a, int b)
{
  int temp;
  while (b != 0)
  {
    temp = b;
    b = a % b;
    a = temp;
  }
  return MAX(1, a);
}

static int LCD(int a, int b)
{
  return a * b / GCD(a, b);
}



static double AngleBetweenLines(double slope1, double slope2)
{
  double angle = 0;
  if (ISINF(slope1) || ISINF(slope2))
  {
    if (ISINF(slope1)) {
      angle = M_PI / 2 - atan(slope2);
    }
    if (ISINF(slope2)) {
      angle = M_PI / 2 - atan(slope1);
    }
    
  }
  else
  {
    double large = MAX(slope2, slope1);
    double small = MIN(slope2, slope1);
    angle = fabs( atan((large - small ) / (1 + large * small)));
  }
  
  return angle;
}

static double Distance(FPOINT p1, FPOINT p2)
{
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

static double Slope(FPOINT start, FPOINT end)
{
  return (end.y - start.y) / (end.x - start.x);
  
}

static FPOINT  AtDistanceFromPointOnLine(FPOINT p, double dx, double dy, double distance)
{
  //Shortening of variables
  double r = sqrt(dx*dx + dy*dy);
  double d = distance;
  double deltaY = dy*d/r ;
  double deltaX = dx*d/r;
  FPOINT destination = fPOINT(p.x +  deltaX, p.y +  deltaY);
  
  return destination;
}

int vcgen_curvebase_impl(FPOINT* Points, int ipos, CyclicCurve* curve, CyclicCurveGetPoint_f fun,
                  double a, const FPOINT* pa, double b, const FPOINT* pb, double tolerance, int level)
{
  //FPOINT pa, pb, dpa, dpb;
  double c = (a+b)/2, cross, area;
  FPOINT pc[1], da, db;
  double PointDistance = 0;
  fun(curve, c, pc, NULL);
  da.x = pc->x - pa->x;
  da.y = pc->y - pa->y;
  db.x = pc->x - pb->x;
  db.y = pc->y - pb->y;
  cross = (da.x*db.y - da.y*db.x);
  area = fabs(cross);
  //Check if endpoints are close enough to approximate with a straight line.
  //optimization
  if (area < tolerance || level>10) {
    vcgen_line_to(pb->x, pb->y);
  } else {
    ipos = vcgen_curvebase_impl(Points, ipos, curve, fun, a, pa, c, pc, tolerance, level+1);
    ipos = vcgen_curvebase_impl(Points, ipos, curve, fun, c, pc, b, pb, tolerance, level+1);
  }
  return ipos;
}
int vcgen_curvebase_div(FPOINT* Points, int ipos, CyclicCurve* curve, CyclicCurveGetPoint_f fun, double tolerance) {
  int seg = 4;
  int i, len = seg*curve->q;
  double b, a = (-1)*2*M_PI/len;
  FPOINT pa[1], pb[1];
  fun(curve, a, pa, NULL);
  tolerance = BOUND(tolerance, 1, 100);
  tolerance *= tolerance;
  for (i=0; i<len; ++i) {
    b = i*2*M_PI/len;
    fun(curve, b, pb, NULL);
    ipos = vcgen_curvebase_impl(Points, ipos, curve, fun, a, pa, b, pb, tolerance, 0);
    a = b;
    *pa = *pb;
  }
  return ipos;
}
int vcgen_curvebase_inc(FPOINT* Points, int ipos, CyclicCurve* curve, CyclicCurveGetPoint_f fun, int seg, int maxlen) {
  int i, len = seg*curve->q;
  len = BOUND(len, 8, maxlen);
  if (Points) {
    for (i=0; i<len; ++i) {
      double x = i*2*M_PI/len;
      fun(curve, x, Points+i, NULL);
    }
  }
  return len;
}
#if 0
int BezierBuilder(FPOINT* Points, BYTE* Types, int ipos, CyclicCurve* curve, CyclicCurveGetPoint_f fun,
                  double a, double b, const FPOINT* pa, const FPOINT* pb, const FPOINT* dpa, const FPOINT* dpb, double tolerance)
{
  //FPOINT pa, pb, dpa, dpb;
  double c = (a+b)/2, cross;
  FPOINT pc[1], dpc[1], da, db;
  double distance, PointDistance = 0;
  fun(curve, c, pc, dpc);
  da.x = pc->x - pa->x;
  da.y = pc->y - pa->y;
  db.x = pc->x - pb->x;
  db.y = pc->y - pb->y;
  cross = (da.x*db.y - da.y*db.x);
  //fun(curve, a, &pa, &dpa);
  //fun(curve, b, &pb, &dpb);
  //The distance between the endpoints is calculated.
  //distance =  Distance(pa, pb);
  
  //Check if endpoints are close enough to approximate with a straight line.
  //optimization
  if (fabs(cross) < tolerance) {
    vcgen_line_to(pb->x, pb.y);
  } else {
    //The slope of the line connecting the endpoints of the segment is detemined.
    double slope = Slope(pa, pb);
    //The Angles between the slope at the endpoints and the endpoint connecting lines is determined from the slopes.
    double angleA = AngleBetweenLines(slope, dpa.y/dpa.x);
    double angleB = AngleBetweenLines(slope, dpb.y/dpb.x);
    
    //The length of the segments connecting the Endpoints to the control points is determined via the law of sines
    double distanceIn = distance / 3; //our extra condition
    double lengthAlongLineA = distanceIn / (sin(M_PI / 2 - angleA)); //Law of Sines
    double lengthAlongLineB = distanceIn / (sin(M_PI / 2 - angleB)); //Law of Sines
    bool IsStable = distance > lengthAlongLineA && distance > lengthAlongLineB;
    
    //The Control points are determined by moving along the line conecting the enpoints to the control points the distance to the control points.
    FPOINT controlPointA = AtDistanceFromPointOnLine(pa, dpa.x, dpa.y, lengthAlongLineA);
    FPOINT controlPointB = AtDistanceFromPointOnLine(pb, -dpb.x, -dpb.y, lengthAlongLineB);
    
    //The  BezierSegment is created from the control points and the endpoints.
    mBs = new BezierSegment(controlPointA, controlPointB, pb, true);
    
    //Mark if Bezier segment is Stable
    PointDistance = Distance(pa, pb);
  }
  return ipos;
}

#endif




#define EpicycloidParameter(MathParameter) \
  MathParameter(int, D, 1, 16, true) \
  MathParameter(int, N, 1, 16, true) \
  MathParameter(double, R, 20, 200, false ) \
MathParameter(double, P, -M_PI, M_PI, false)

int Epicycloid(CyclicCurve* c, double t, FPOINT* pt, FPOINT* dpt) {
  static MathParameter_t Parameter[] = {
    EpicycloidParameter(MathParameter)
  };
  int i = 0;
  EpicycloidParameter(MathParameterDef);
  {
    int gcd = GCD(N, D);
    double p = (double)N / gcd;
    double q = (double)D / gcd;
    double k = p / q;
    t *= q;
    if (pt) {
      pt->x = R * ((k + 1) * cos(t + P) - cos((k + 1) * t + P));
      pt->y = R * ((k + 1) * sin(t + P) - sin((k + 1) * t + P));
    }
    if (dpt) {
      dpt->x = -R * ((k + 1) * sin(t + P) - (k + 1) * sin((k + 1) * t + P));
      dpt->y = R * ((k + 1) * cos(t + P) - (k + 1) * cos((k + 1) * t + P));
    }
    c->q = q;
    c->para = Parameter;
    c->npara = countof(Parameter);
    c->formula = 
      "x=R*((k+1)*cos(t+P)-cos((k+1)*t+P))\n"
      "y=R*((k+1)*sin(t+P)-sin((k+1)*t+P))\n"
      ;
  }
  return 0;
}

#define EpitrochoidParameter(MathParameter) \
  MathParameter(int, D, 1, 16, true) \
  MathParameter(int, N, 1, 16, true) \
  MathParameter(double, R, 20, 200, false ) \
  MathParameter (double, M, -8, 8, false ) \
MathParameter(double, P, -M_PI, M_PI, false)


int Epitrochoid(CyclicCurve* c, double t, FPOINT* pt, FPOINT* dpt) {
  static MathParameter_t Parameter[] = {
    EpitrochoidParameter(MathParameter)
  };
  int i = 0;
  EpitrochoidParameter(MathParameterDef);
  {
    int gcd = GCD(N, D);
    double p = (double)N / gcd;
    double q = (double)D / gcd;
    double k = (double)p / q;
    
    t *= q;
    if(pt){
      pt->x=R*((k+1)*cos(t+P)-pow(2,M)*cos((k+1)*t+P));
      pt->y=R*((k+1)*sin(t+P)-pow(2,M)*sin((k+1)*t+P));
    }
    if(dpt){
      dpt->x=-R*((k+1)*sin(t+P)-(k+1)*pow(2,M)*sin((k+1)*t+P));
      dpt->y=R*(k+1)*cos(t+P)-R*(k+1)*pow(2,M)*cos((k+1)*t+P);
    }
    c->q = q;
    c->para = Parameter;
    c->npara = countof(Parameter);
    c->formula = 
      "x=R*((k+1)*cos(t+P)-pow(2,M)*cos((k+1)*t+P))\n"
      "y=R*((k+1)*sin(t+P)-pow(2,M)*sin((k+1)*t+P))\n"
      ;
  }
  return 0;
}


#define HypocycloidParameter(MathParameter) \
  MathParameter(int, D, 1, 16, true) \
  MathParameter(int, N, 1, 16, true) \
  MathParameter(double, R, 0, 100, false) \
  MathParameter(double, P, -M_PI, M_PI, false) \

int Hypocycloid(CyclicCurve* c, double t, FPOINT* pt, FPOINT* dpt) {
static MathParameter_t Parameter[] = {
  // 100 + F1 * F1 + F2 * F2 + F3*F3;
  HypocycloidParameter(MathParameter)
};
int i = 0;
HypocycloidParameter(MathParameterDef);
{
  int gcd = GCD(N, D);//GCD for Greatest common Denominator
  double p = (double)N / gcd;
  double q = (double)D / gcd;
  double m = (double)p / q - 1;
  t *= q;
  if (pt) {
    pt->x=R*m*cos(t+P)+R*cos(m*t+P);
    pt->y=R*m*sin(t+P)-R*sin(m*t+P);
  }
  if (dpt) {
    dpt->x = -R * m * sin(t + P) - m * R * sin(m * t + P);
    dpt->y = R * m * cos(t + P) - R * m * cos(m * t + P);
  }
  
  c->q = q;
  c->para = Parameter;
  c->npara = countof(Parameter);
  c->formula = 
    "x=R*m*cos(t+P)+R*cos(m*t+P)\n"
    "y=R*m*sin(t+P)-R*sin(m*t+P)\n"
    ;
}
return 0;
}

#define HypotrochoidParameter(MathParameter) \
  MathParameter(int, D, 1, 16, true) \
  MathParameter(int, N, 1, 16, true) \
  MathParameter(double, R, 0, 100, false) \
  MathParameter(double, M, -4, 8, false) \
MathParameter(double, P, -M_PI, M_PI, false)

int Hypotrochoid(CyclicCurve* c, double t, FPOINT* pt, FPOINT* dpt) {
  static MathParameter_t Parameter[] = {
    // 100 + F1 * F1 + F2 * F2 + F3*F3;
    HypotrochoidParameter(MathParameter)
  };
  int i = 0;
  HypotrochoidParameter(MathParameterDef);
  {
    int gcd = GCD(N, D);
    double p = (double)N / gcd;
    double q = (double)D / gcd;
    double k = (double)p / q;
    t *= q;
    if (pt) {
      pt->x=R*((k-1)*cos(t+P)+pow(2,M)*cos((k-1)*t+P));
      pt->y=R*((k-1)*sin(t+P)-pow(2,M)*sin((k-1)*t+P));
    }
    if (dpt) {
      dpt->x = -R * ((k - 1) * sin(t + P) + (k - 1) * pow(2, M) * sin((k - 1) * t + P));
      dpt->y = R * ((k - 1) * cos(t + P) - (k - 1) * pow(2, M) * cos((k - 1) * t + P));
    }
    
    c->q = q;
    c->para = Parameter;
    c->npara = countof(Parameter);
    c->formula = 
      "x=R*((k-1)*cos(t+P)+pow(2,M)*cos((k-1)*t+P))\n"
      "y=R*((k-1)*sin(t+P)-pow(2,M)*sin((k-1)*t+P))\n"
      ;
  }
  return 0;
}

#define LissajousParameter(MathParameter) \
  MathParameter(int, A, 2, 20, true) \
  MathParameter(int, B, 2, 20, true) \
  MathParameter(double, Delta, -M_PI, M_PI, false) \
  MathParameter(int, Alpha, 1, 20, true) \
MathParameter(int, Beta, 1, 10, true)

int Lissajous(CyclicCurve* c, double t, FPOINT* pt, FPOINT* dpt) {
  static MathParameter_t Parameter[] = {
    // 100 + F1 * F1 + F2 * F2 + F3*F3;
    LissajousParameter(MathParameter)
  };
  int i = 0;
  LissajousParameter(MathParameterDef);
  {
    int gcd = GCD(Alpha,Beta);
    double p = (double)Alpha / gcd;
    double q = (double)Beta / gcd;
    t *= q;
    
    if (pt) {
      pt->x=A*sin(p*t+Delta);
      pt->y=B*sin(q*t);
    }
    if (dpt) {
      dpt->x = A * p * cos(p * t + Delta);
      dpt->y = B * q * cos(q * t);
    }
    c->q = q;
    c->para = Parameter;
    c->npara = countof(Parameter);
    c->formula = 
      "x=A*sin(p*t+Delta)\n"
      "y=B*sin(q*t)\n"
      ;
  }
  return 0;
}

#define RoseParameter(MathParameter) \
  MathParameter(int, D, 1, 20, true) \
  MathParameter(int, N, 1, 20, true) \
MathParameter(double, A, 1, 100, false)


int Rose(CyclicCurve* c, double t, FPOINT* pt, FPOINT* dpt) {
  static MathParameter_t Parameter[] = {
    // 100 + F1 * F1 + F2 * F2 + F3*F3;
    RoseParameter(MathParameter)
  };
  int i = 0;
  RoseParameter(MathParameterDef);
  {
    int gcd = GCD(N, D);
    double p = (double)N / gcd;
    double q = (double)D / gcd;
    double omega = p / q;
    double r = A * sin(omega * t);
    t*=q;
    if(pt) {
      pt->x = r * cos(t);
      pt->y = r * sin(t);
    }
    if (dpt) {
      r = omega * A * cos(omega * t);
      dpt->x = r * cos(t);
      dpt->y = r * sin(t);
    }
    c->q = q;
    c->para = Parameter;
    c->npara = countof(Parameter);
    c->formula = "r=A * sin(omega * t)\n";
  }
  
  return 0;
}



#define FarrisWheelParameter(MathParameter) \
  MathParameter(int, F1,-3, 20, true) \
  MathParameter(int, F2, -3, 20, true) \
  MathParameter(double, W1,-0.5, 1, false ) \
  MathParameter(double, W2, -0.5, 1, false) \
  MathParameter(double, W3, -0.5, 1, false) \
  MathParameter(int, F3, -3, 30, true) \
  MathParameter(double, R, 10, 200, false) \
  MathParameter(double, P, -0.5,1,false) \
  MathParameter(double, P1, -0.5, 1, false) \
  MathParameter(double, P2, -0.5, 1, false) \
MathParameter(double, P3, -0.5, 1, false)

int FarrisWheel(CyclicCurve* c, double t, FPOINT* pt, FPOINT* dpt) {
  static MathParameter_t Parameter[] = {
    // 100 + F1 * F1 + F2 * F2 + F3*F3;
    FarrisWheelParameter(MathParameter)
  };
  int i = 0;
  FarrisWheelParameter(MathParameterDef);
  
  {
    double maxRadius = fabs(W1) + fabs(W2) + fabs(W3);
    double scaleFactor = R / maxRadius;
    double pp1PI = (P + P1) * M_PI;
    double pp2PI = (P + P2) * M_PI;
    double pp3PI = (P + P3) * M_PI;
    if (pt) {
      pt->x = scaleFactor * (W1 * cos(F1 * t + pp1PI) + W2 * cos(F2 * t + pp2PI) + W3 * cos(F3 * t + pp3PI));
      pt->y = scaleFactor * (W1 * sin(F1 * t + pp1PI) + W2 * sin(F2 * t + pp2PI) + W3 * sin(F3 * t + pp3PI));
    }
    if (dpt) {
      dpt->x = scaleFactor * (-F1 * W1 * sin(F1 * t + pp1PI) - F2 * W2 * sin(F2 * t + pp2PI) - F3 * W3 * sin(F3 * t + pp3PI));
      dpt->y = scaleFactor * (F1 * W1 * cos(F1 * t + pp1PI) + F2 * W2 * cos(F2 * t + pp2PI) + F3 * W3 * cos(F3 * t + pp3PI));
    }
    c->q = 4;
    c->para = Parameter;
    c->npara = countof(Parameter);
    c->formula = 
      "x=x\n"
      "y=y\n"
      ;
  }
  return 0;
}
