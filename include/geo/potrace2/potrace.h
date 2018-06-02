
// #region additional Class definitions
//----------------------Potrace Constants and aux functions
const int POTRACE_CORNER = 1;
const int POTRACE_CURVETO = 2;
//static double COS179 = cos(179 * CC_PI / 180);
static double COS179 = -0.99984769515639127;
/// <summary>
/// area of largest path to be ignored
/// </summary>
static int turdsize = 2;     
/// <summary>
///  corner threshold
/// </summary>
static double alphamax = 1.0;     
/// <summary>
///  use curve optimization
///  optimize the path p, replacing sequences of Bezier segments by a
///  single segment when possible.
/// </summary>
static bool curveoptimizing = true;
/// <summary>
/// curve optimization tolerance
/// </summary>
static double opttolerance = 0.2;     

/* integer arithmetic */
#define SIGN(x)  ((x) > 0 ? 1 : (x) < 0 ? -1 : 0)
//#define ABS(a)   ((a) > 0 ? (a) : -(a))
//#define MIN(int a, int b) { return ((a) < (b) ? (a) : (b)); }
//static int MAX(int a, int b) { return ((a) > (b) ? (a) : (b)); }
//static int SQ(int a) { return ((a) * (a)); }
//static int cu(int a) { return ((a) * (a) * (a)); }

static int mod(int a, int n)
{
  return a >= n ? a % n : a >= 0 ? a : n - 1 - (-1 - a) % n;
}
static int floordiv(int a, int n)
{
  return a >= 0 ? a / n : -1 - (-1 - a) / n;
}

/* ---------------------------------------------------------------------- */
typedef enum direction{North,East,South,West,} direction;
/// <summary>
/// Kind of DCURVE : Line or Bezier
/// </summary>
typedef enum CurveKind{Line,Bezier}CurveKind;

typedef struct SumStruct { int x, y, xy, x2, y2; }SumStruct;
typedef struct privcurve
{
  int n;            /* number of segments */
  int* tag;         /* tag[n]: POTRACE_CORNER or POTRACE_CURVETO */
  DPOINT* ControlPoints;/* c[n][i]: control points. c[n][0] is unused for tag[n]=POTRACE_CORNER */
  DPOINT* vertex;      /* for POTRACE_CORNER, this equals c[1] (*c)[3]; /* c[n][i]: control points. c[n][0] is unused for tag[n]=POTRACE_CORNER */
  double* alpha;    /* only for POTRACE_CURVETO */
  double* alpha0;   /* "uncropped" alpha parameter - for debug output only */
  double* beta;
} privcurve;

int privcurve_free(privcurve* p) {
  FREE(p->tag);
  FREE(p->ControlPoints);
  FREE(p->vertex);
  FREE(p->alpha);
  FREE(p->alpha0);
  FREE(p->beta);
  return 0;
}
int privcurve_setsize(privcurve* p, int n)
{
  p->n = n;
  p->tag = MALLOC(int, n);
  p->ControlPoints = MALLOC(DPOINT, n*3);
  p->vertex = MALLOC(DPOINT, n);
  p->alpha = MALLOC(double, n);
  p->alpha0 = MALLOC(double, n);
  p->beta = MALLOC(double, n);
  return 0; 
}

/// <summary>
/// Holds the information about der produced curves
/// </summary>
typedef struct DCURVE
{
  /// <summary>
  /// Bezier or Line
  /// </summary>
  int Kind;
  /// <summary>
  /// Startpoint
  /// </summary>
  DPOINT A;
  /// <summary>
  /// ControlPoint
  /// </summary>
  DPOINT ControlPointA;
  /// <summary>
  /// ControlPoint
  /// </summary>
  DPOINT ControlPointB;
  /// <summary>
  /// Endpoint
  /// </summary>
  DPOINT B;
} DCURVE;

/// <summary>
/// Creates a curve
/// </summary>
/// <param name="Kind"></param>
/// <param name="A">Startpoint</param>
/// <param name="ControlPointA">Controlpoint</param>
/// <param name="ControlPointB">Controlpoint</param>
/// <param name="B">Endpoint</param>
DCURVE dCURVE(int Kind, DPOINT A, DPOINT ControlPointA, DPOINT ControlPointB, DPOINT B)
{
  DCURVE c={0};
  c.Kind = Kind;
  c.A = A;
  c.B = B;
  c.ControlPointA = ControlPointA;
  c.ControlPointB = ControlPointB;
  c.B = B;
  return c;
}

/// <summary>
/// The monoton_interval_t defines a structure related to an IPOINT array.
/// For each index i,j with 
/// from &lt;= i &lt; j &lt;= to 
/// in a cyclic sense is iPointArray[i].y &lt;= iPointArray[j].y if Increasing is true,
/// else is iPointArray[i].y &gt;= iPointArray[j].y.
/// </summary>
typedef struct monoton_interval_t
{
  bool Increasing;
  int from, to;
  int CurrentID; // only used by Invert
}monoton_interval_t;

typedef struct Path
{
  int area;
  monoton_interval_t* monoton_intervals;
  int monoton_intervals_count;
  IPOINT* pt;
  int pt_length;
  int* Lon;
  SumStruct* Sums;
  int* po;
  int po_Length;
  privcurve Curves[1];
  privcurve OptimizedCurves[1];
  privcurve FCurves[1];
} Path;

monoton_interval_t make_monoton_interval(bool Increasing, int from, int to)
{
  monoton_interval_t mo={0};
  mo.Increasing = Increasing;
  mo.from = from;
  mo.to = to;
  return mo;
}
int monoton_interval_Min(const monoton_interval_t* mo)
{
  if (mo->Increasing) return mo->from;
  return mo->to;
}
int monoton_interval_Max(const monoton_interval_t* mo)
{
  if (!mo->Increasing) return mo->from;
  return mo->to;
}

int monoton_interval_MinY(const monoton_interval_t* mo, IPOINT* Pts)
{
  return Pts[monoton_interval_Min(mo)].y;
}
int monoton_interval_MaxY(const monoton_interval_t* mo, IPOINT* Pts)
{
  return Pts[monoton_interval_Max(mo)].y;
}

void monoton_interval_ResetCurrentID(monoton_interval_t* mo, int modulo)
{
  if (!mo->Increasing)
    mo->CurrentID = mod(monoton_interval_Min(mo) + 1, modulo);
  else
    mo->CurrentID = monoton_interval_Min(mo);
}
