typedef enum EdgeColor_ {
  BLACK = 0,
  RED = 1,
  GREEN = 2,
  YELLOW = 3,
  BLUE = 4,
  MAGENTA = 5,
  CYAN = 6,
  WHITE = 7
} EdgeColor;
enum {LinearSegment, QuadraticSegment, CubicSegment};
typedef struct SignedDistance_ {
  double distance;
  double dot;
  double param;
} SignedDistance;
typedef struct EdgeSegment_ {
  int type;
  EdgeColor color;
  DPOINT lastpt;
  DPOINT pt[4];
} EdgeSegment;
typedef struct Contour_ {
  int len;
  EdgeSegment* edges;
} Contour;
typedef struct Shape_ {
  int len;
  Contour* contours;
} Shape;
/// A floating-point RGB pixel.
typedef struct FloatRGB_ {
  float b, g, r;
} FloatRGB;
CC_INLINE FloatRGB mixRGB(FloatRGB a, FloatRGB b, double weight)
{
  FloatRGB c;
  c.r = ((1. - weight) * a.r + weight * b.r);
  c.g = ((1. - weight) * a.g + weight * b.g);
  c.b = ((1. - weight) * a.b + weight * b.b);
  return c;
}
int readCharS(const char** input)
{
  int c = '\0';
  do {
    c = *(*input)++;
  }
  while (c == ' ' || c == '\t' || c == '\r' || c == '\n');
  if (!c) {
    --c;
    return EOF;
  }
  return c;
}
int readCoordS(const char** input, DPOINT* coord)
{
  int read = 0;
  int result = sscanf(*input, "%lf,%lf%n", &coord->x, &coord->y, &read);
  *input += read;
  return result;
}
static int readControlPoints(const char** input, DPOINT* output)
{
  int result = readCoordS(input, output);
  if (result == 2) {
    switch (readCharS(input)) {
    case ')':
      return 1;
    case ';':
      break;
    default:
      return -1;
    }
    result = readCoordS(input, output + 1);
    if (result == 2 && readCharS(input) == ')') {
      return 2;
    }
  }
  else if (result != 1 && readCharS(input) == ')') {
    return 0;
  }
  return -1;
}
int Edge_setLine(EdgeSegment* edge, DPOINT p0, DPOINT p1, EdgeColor color)
{
  edge->type = LinearSegment;
  edge->pt[0] = p0;
  edge->pt[1] = p1;
  edge->color = color;
  return 0;
}
int Edge_setQuadratic(EdgeSegment* edge, DPOINT p0, DPOINT p1, DPOINT p2, EdgeColor color)
{
  edge->type = QuadraticSegment;
  edge->pt[0] = p0;
  edge->pt[1] = p1;
  edge->pt[2] = p2;
  edge->color = color;
  return 0;
}
int Edge_setCubic(EdgeSegment* edge, DPOINT p0, DPOINT p1, DPOINT p2, DPOINT p3, EdgeColor color)
{
  edge->type = CubicSegment;
  edge->pt[0] = p0;
  edge->pt[1] = p1;
  edge->pt[2] = p2;
  edge->pt[3] = p3;
  edge->color = color;
  return 0;
}
int Contour_addLine(Contour* contour, DPOINT p0, DPOINT p1, EdgeColor color)
{
  EdgeSegment* edge;
  edge = (EdgeSegment*)myAllocPush(contour->edges, contour->len, 1);
  Edge_setLine(edge, p0, p1, color);
  return 0;
}
int Contour_addQuadratic(Contour* contour, DPOINT p0, DPOINT p1, DPOINT p2, EdgeColor color)
{
  EdgeSegment* edge;
  edge = (EdgeSegment*)myAllocPush(contour->edges, contour->len, 1);
  Edge_setQuadratic(edge, p0, p1, p2, color);
  return 0;
}
int Contour_addCubic(Contour* contour, DPOINT p0, DPOINT p1, DPOINT p2, DPOINT p3, EdgeColor color)
{
  EdgeSegment* edge;
  edge = (EdgeSegment*)myAllocPush(contour->edges, contour->len, 1);
  Edge_setCubic(edge, p0, p1, p2, p3, color);
  return 0;
}
static bool readContour(const char** input, Contour* output, const DPOINT* first, int terminator, bool* colorsSpecified)
{
  DPOINT p[4], start;
  int c, result;
  if (first) {
    p[0] = *first;
  }
  else {
    int result = readCoordS(input, p);
    if (result != 2) {
      return result != 1 && readCharS(input) == terminator;
    }
  }
  start = p[0];
  c = '\0';
  while ((c = readCharS(input)) != terminator) {
    bool parenthesis;
    EdgeColor color;
    if (c != ';') {
      return false;
    }
    parenthesis = false;
    color = WHITE;
    result = readCoordS(input, p + 1);
    if (result == 2) {
      Contour_addLine(output, p[0], p[1], color);
      p[0] = p[1];
      continue;
    }
    else if (result == 1) {
      return false;
    }
    else {
      int controlPoints = 0;
      switch ((c = readCharS(input))) {
      case '#':
        Contour_addLine(output, p[0], start, color);
        p[0] = start;
        continue;
      case ';':
        goto FINISH_EDGE;
      case '(':
        goto READ_CONTROL_POINTS;
      case 'C':
      case 'c':
        color = CYAN;
        *colorsSpecified = true;
        break;
      case 'M':
      case 'm':
        color = MAGENTA;
        *colorsSpecified = true;
        break;
      case 'Y':
      case 'y':
        color = YELLOW;
        *colorsSpecified = true;
        break;
      case 'W':
      case 'w':
        color = WHITE;
        *colorsSpecified = true;
        break;
      default:
        return c == terminator;
      }
      switch (readCharS(input)) {
      case ';':
        goto FINISH_EDGE;
      case '(':
READ_CONTROL_POINTS:
        if ((controlPoints = readControlPoints(input, p + 1)) < 0) {
          return false;
        }
        break;
      default:
        return false;
      }
      if (readCharS(input) != ';') {
        return false;
      }
FINISH_EDGE:
      result = readCoordS(input, p + 1 + controlPoints);
      if (result != 2) {
        if (result == 1) {
          return false;
        }
        else {
          if (readCharS(input) == '#') {
            p[1 + controlPoints] = start;
          }
          else {
            return false;
          }
        }
      }
      switch (controlPoints) {
      case 0:
        Contour_addLine(output, p[0], p[1], color);
        p[0] = p[1];
        continue;
      case 1:
        Contour_addQuadratic(output, p[0], p[1], p[2], color);
        p[0] = p[2];
        continue;
      case 2:
        Contour_addCubic(output, p[0], p[1], p[2], p[3], color);
        p[0] = p[3];
        continue;
      }
    }
  }
  return true;
}
bool readShapeDescription(const char* input, Shape* output, bool* colorsSpecified)
{
  DPOINT p;
  int result, i, c;
  bool locColorsSpec = false;
  output->len = 0;
  result = readCoordS(&input, &p);
  if (result == 2) {
    Contour* contour = (Contour*)myAllocPush(output->contours, output->len, 1);
    return readContour(&input, contour, &p, EOF, &locColorsSpec);
  }
  else if (result == 1) {
    return false;
  }
  else {
    c = readCharS(&input);
    if (c == '@') {
      for (i = 0; i < sizeof("invert-y") - 1; ++i) {
        if (input[i] != "invert-y"[i]) {
          return false;
        }
      }
      input += sizeof("invert-y") - 1;
      c = readCharS(&input);
    }
    for (; c == '{'; c = readCharS(&input)) {
      Contour* contour = (Contour*)myAllocPush(output->contours, output->len, 1);
      if (!readContour(&input, contour, NULL, '}', &locColorsSpec)) {
        return false;
      }
    }
    if (colorsSpecified) {
      *colorsSpecified = locColorsSpec;
    }
    return c == EOF;
  }
}
#define LARGE_VALUE 1e240
DPOINT dPOINT_add(DPOINT a, DPOINT b)
{
  return dPOINT(a.x + b.x, a.y + b.y);
}
DPOINT dPOINT_sub(DPOINT a, DPOINT b)
{
  return dPOINT(a.x - b.x, a.y - b.y);
}
double dPOINT_crossProduct(DPOINT a, DPOINT b)
{
  return a.x * b.y - a.y * b.x;
}
double dPOINT_dotProduct(DPOINT a, DPOINT b)
{
  return a.x * b.x + a.y * b.y;
}
double dPOINT_length(DPOINT a)
{
  return sqrt(a.x * a.x + a.y * a.y);
}
double dPOINT_direction(DPOINT a)
{
  return atan2(a.y, a.x);
}
DPOINT dPOINT_normalize(DPOINT a, BOOL allowZero/* = false */)
{
  double len = dPOINT_length(a);
  if (len == 0) {
    return dPOINT(0, !allowZero);
  }
  return dPOINT(a.x / len, a.y / len);
}
DPOINT dPOINT_getOrthogonal(DPOINT a, bool polarity /* = true*/)
{
  return polarity ? dPOINT(-a.y, a.x) : dPOINT(a.y, -a.x);
}
DPOINT dPOINT_getOrthonormal(DPOINT a, bool polarity /* = true*/, bool allowZero /* = false */)
{
  double len = dPOINT_length(a);
  if (len == 0) {
    return polarity ? dPOINT(0, !allowZero) : dPOINT(0, -!allowZero);
  }
  return polarity ? dPOINT(-a.y / len, a.x / len) : dPOINT(a.y / len, -a.x / len);
}
DPOINT dPOINT_project(DPOINT a, DPOINT vector, bool positive/* = false*/)
{
  DPOINT n = dPOINT_normalize(a, true);
  double t = dPOINT_dotProduct(vector, n);
  if (positive && t <= 0) {
    return dPOINT(0, 0);
  }
  return dPOINT(t * n.x, t * n.y);
}
int solveQuadratic(double x[2], double a, double b, double c)
{
  double dscr;
  if (fabs(a) < 1e-14) {
    if (fabs(b) < 1e-14) {
      if (c == 0) {
        return -1;
      }
      return 0;
    }
    x[0] = -c / b;
    return 1;
  }
  dscr = b * b - 4 * a * c;
  if (dscr > 0) {
    dscr = sqrt(dscr);
    x[0] = (-b + dscr) / (2 * a);
    x[1] = (-b - dscr) / (2 * a);
    return 2;
  }
  else if (dscr == 0) {
    x[0] = -b / (2 * a);
    return 1;
  }
  else {
    return 0;
  }
}
int solveCubicNormed(double* x, double a, double b, double c)
{
  double a2 = a * a;
  double q = (a2 - 3 * b) / 9;
  double r = (a * (2 * a2 - 9 * b) + 27 * c) / 54;
  double r2 = r * r;
  double q3 = q * q * q;
  double A, B;
  if (r2 < q3) {
    double t = r / sqrt(q3);
    if (t < -1) {
      t = -1;
    }
    if (t > 1) {
      t = 1;
    }
    t = acos(t);
    a /= 3;
    q = -2 * sqrt(q);
    x[0] = q * cos(t / 3) - a;
    x[1] = q * cos((t + 2 * M_PI) / 3) - a;
    x[2] = q * cos((t - 2 * M_PI) / 3) - a;
    return 3;
  }
  else {
    A = -pow(fabs(r) + sqrt(r2 - q3), 1 / 3.);
    if (r < 0) {
      A = -A;
    }
    B = A == 0 ? 0 : q / A;
    a /= 3;
    x[0] = (A + B) - a;
    x[1] = -0.5 * (A + B) - a;
    x[2] = 0.5 * sqrt(3.) * (A - B);
    if (fabs(x[2]) < 1e-14) {
      return 2;
    }
    return 1;
  }
}
int solveCubic(double x[3], double a, double b, double c, double d)
{
  if (fabs(a) < 1e-14) {
    return solveQuadratic(x, b, c, d);
  }
  return solveCubicNormed(x, b / a, c / a, d / a);
}
int SignedDistance_set(SignedDistance* sd, double distance, double dot/* = 1*/, double param/* = 0*/)
{
  sd->distance = distance;
  sd->dot = dot;
  sd->param = param;
  return 1;
}
//#define nonZeroSign(n) (2*((n) > (0))-1)
double nonZeroSign(double n)
{
  return (2 * ((n) > (0)) - 1);
}
int signedDistance(const EdgeSegment* edge, DPOINT origin, SignedDistance* sd)
{
  int i;
  const DPOINT* p = edge->pt;
  switch (edge->type) {
  case LinearSegment: {
    DPOINT aq = dPOINT_sub(origin, p[0]);
    DPOINT ab = dPOINT_sub(p[1], p[0]);
    double param = dPOINT_dotProduct(aq, ab) / dPOINT_dotProduct(ab, ab);
    DPOINT eq = dPOINT_sub(p[param > .5], origin);
    double endpointDistance = dPOINT_length(eq);
    double t;
    if (param > 0 && param < 1) {
      double orthoDistance = dPOINT_dotProduct(dPOINT_getOrthonormal(ab, false, false), aq);
      if (fabs(orthoDistance) < endpointDistance) {
        return SignedDistance_set(sd, orthoDistance, 0, param);
      }
    }
    t = dPOINT_crossProduct(aq, ab);
    SignedDistance_set(sd, nonZeroSign(t)*endpointDistance, fabs(dPOINT_dotProduct(dPOINT_normalize(ab, false), dPOINT_normalize(eq, false))), param);
    return 1;
  }
  case QuadraticSegment: {
    DPOINT qa = dPOINT_sub(p[0], origin);
    DPOINT qc = dPOINT_sub(p[2], origin);
    DPOINT ab = dPOINT_sub(p[1], p[0]);
    DPOINT bc = dPOINT_sub(p[2], p[1]);
    DPOINT br = dPOINT_sub(bc, ab);
    double a = dPOINT_dotProduct(br, br);
    double b = 3 * dPOINT_dotProduct(ab, br);
    double c = 2 * dPOINT_dotProduct(ab, ab) + dPOINT_dotProduct(qa, br);
    double d = dPOINT_dotProduct(qa, ab);
    double t[3];
    int solutions = solveCubic(t, a, b, c, d);
    double minDistance = nonZeroSign(dPOINT_crossProduct(ab, qa)) * dPOINT_length(qa); // distance from A
    double param = -dPOINT_dotProduct(qa, ab) / dPOINT_dotProduct(ab, ab);
    {
      double distance = nonZeroSign(dPOINT_crossProduct(bc, qc)) * dPOINT_length(qc); // distance from B
      if (fabs(distance) < fabs(minDistance)) {
        minDistance = distance;
        param = dPOINT_dotProduct(dPOINT_sub(origin, p[1]), bc) / dPOINT_dotProduct(bc, bc);
      }
    }
    for (i = 0; i < solutions; ++i) {
      if (t[i] > 0 && t[i] < 1) {
        DPOINT endpoint = dPOINT(p[0].x + 2 * t[i] * ab.x + t[i] * t[i] * br.x, p[0].y + 2 * t[i] * ab.y + t[i] * t[i] * br.y);
        DPOINT oe = dPOINT_sub(endpoint, origin);
        double distance = nonZeroSign(dPOINT_crossProduct(dPOINT_sub(p[2], p[0]), oe)) * dPOINT_length(oe);
        if (fabs(distance) <= fabs(minDistance)) {
          minDistance = distance;
          param = t[i];
        }
      }
    }
    if (param >= 0 && param <= 1) {
      return SignedDistance_set(sd, minDistance, 0, param);
    }
    if (param < .5) {
      return SignedDistance_set(sd, minDistance, fabs(dPOINT_dotProduct(dPOINT_normalize(ab, false), dPOINT_normalize(qa, false))), param);
    }
    else {
      return SignedDistance_set(sd, minDistance, fabs(dPOINT_dotProduct(dPOINT_normalize(bc, false), dPOINT_normalize(qc, false))), param);
    }
  }
  case CubicSegment: {
    return 1;
  }
  }
  return 0;
}
bool SignedDistance_less(SignedDistance* a, SignedDistance* b)
{
  return fabs(a->distance) < fabs(b->distance) || (fabs(a->distance) == fabs(b->distance) && a->dot < b->dot);
}
static int pointBounds(DPOINT p, DRECT* prc)
{
  if (p.x < prc->l) {
    prc->l = p.x;
  }
  if (p.y < prc->b) {
    prc->b = p.y;
  }
  if (p.x > prc->r) {
    prc->r = p.x;
  }
  if (p.y > prc->t) {
    prc->t = p.y;
  }
  return 0;
}
float fmix(float a, float b, float weight)
{
  return (((1.f) - weight) * a + weight * b);
}
double dmix(double a, double b, double weight)
{
  return ((1. - weight) * a + weight * b);
}
DPOINT dPOINT_mix(DPOINT a, DPOINT b, double weight)
{
  double t = 1 - weight;
  return dPOINT(t * a.x + weight * b.x, t * a.y + weight * b.y);
}
DPOINT Edge_point(const EdgeSegment* edge, double param)
{
  const DPOINT* p = edge->pt;
  DPOINT p12;
  switch (edge->type) {
  case LinearSegment:
    return dPOINT_mix(p[0], p[1], param);
  case QuadraticSegment:
    return dPOINT_mix(dPOINT_mix(p[0], p[1], param), dPOINT_mix(p[1], p[2], param), param);
  case CubicSegment:
    p12 = dPOINT_mix(p[1], p[2], param);
    return dPOINT_mix(dPOINT_mix(dPOINT_mix(p[0], p[1], param), p12, param), dPOINT_mix(p12, dPOINT_mix(p[2], p[3], param), param), param);
  }
  return dPOINT(0, 0);
}
DPOINT Edge_direction(const EdgeSegment* edge, double param)
{
  const DPOINT* p = edge->pt;
  switch (edge->type) {
  case LinearSegment:
    return dPOINT_sub(p[1], p[0]);
  case QuadraticSegment:
    return dPOINT_mix(dPOINT_sub(p[1], p[0]), dPOINT_sub(p[2], p[1]), param);
  case CubicSegment:
    return dPOINT_mix(dPOINT_mix(dPOINT_sub(p[1], p[0]), dPOINT_sub(p[2], p[1]), param), dPOINT_mix(dPOINT_sub(p[2], p[1]), dPOINT_sub(p[3], p[2]), param), param);
  }
  return dPOINT(0, 0);
}
EdgeSegment* Edge_splitInThirds(const EdgeSegment* edge, EdgeSegment* ebuf, EdgeSegment** part1, EdgeSegment** part2, EdgeSegment** part3)
{
  const DPOINT* p = edge->pt;
  EdgeColor color = edge->color;
  switch (edge->type) {
  case LinearSegment:
    Edge_setLine(*part1 = ebuf++, p[0], Edge_point(edge, 1 / 3.), color);
    Edge_setLine(*part2 = ebuf++, Edge_point(edge, 1 / 3.), Edge_point(edge, 2 / 3.), color);
    Edge_setLine(*part3 = ebuf++, Edge_point(edge, 2 / 3.), p[1], color);
    return ebuf;
  case QuadraticSegment:
    Edge_setQuadratic(*part1 = ebuf++, p[0], dPOINT_mix(p[0], p[1], 1 / 3.), Edge_point(edge, 1 / 3.), color);
    Edge_setQuadratic(*part2 = ebuf++, Edge_point(edge, 1 / 3.), dPOINT_mix(dPOINT_mix(p[0], p[1], 5 / 9.), dPOINT_mix(p[1], p[2], 4 / 9.), .5), Edge_point(edge, 2 / 3.), color);
    Edge_setQuadratic(*part3 = ebuf++, Edge_point(edge, 2 / 3.), dPOINT_mix(p[1], p[2], 2 / 3.), p[2], color);
    return ebuf;
  case CubicSegment:
    Edge_setCubic(*part1 = ebuf++, p[0], dPOINT_mix(p[0], p[1], 1 / 3.), dPOINT_mix(dPOINT_mix(p[0], p[1], 1 / 3.), dPOINT_mix(p[1], p[2], 1 / 3.), 1 / 3.), Edge_point(edge, 1 / 3.), color);
    Edge_setCubic(*part2 = ebuf++, Edge_point(edge, 1 / 3.),
        dPOINT_mix(dPOINT_mix(dPOINT_mix(p[0], p[1], 1 / 3.), dPOINT_mix(p[1], p[2], 1 / 3.), 1 / 3.), dPOINT_mix(dPOINT_mix(p[1], p[2], 1 / 3.), dPOINT_mix(p[2], p[3], 1 / 3.), 1 / 3.), 2 / 3.),
        dPOINT_mix(dPOINT_mix(dPOINT_mix(p[0], p[1], 2 / 3.), dPOINT_mix(p[1], p[2], 2 / 3.), 2 / 3.), dPOINT_mix(dPOINT_mix(p[1], p[2], 2 / 3.), dPOINT_mix(p[2], p[3], 2 / 3.), 2 / 3.), 1 / 3.),
        Edge_point(edge, 2 / 3.), color);
    Edge_setCubic(*part3 = ebuf++, Edge_point(edge, 2 / 3.), dPOINT_mix(dPOINT_mix(p[1], p[2], 2 / 3.), dPOINT_mix(p[2], p[3], 2 / 3.), 2 / 3.), dPOINT_mix(p[2], p[3], 2 / 3.), p[3], color);
    return ebuf;
  }
  return ebuf;
}
int Edge_bounds(const EdgeSegment* edge, DRECT* prc)
{
  const DPOINT* p = edge->pt;
  DPOINT bot, a0, a1, a2;
  double params[2];
  int solutions, i;
  switch (edge->type) {
  case LinearSegment:
    pointBounds(p[0], prc);
    pointBounds(p[1], prc);
    break;
  case QuadraticSegment:
    pointBounds(p[0], prc);
    pointBounds(p[2], prc);
    bot = dPOINT_sub(dPOINT_sub(p[1], p[0]), dPOINT_sub(p[2], p[1]));
    if (bot.x) {
      double param = (p[1].x - p[0].x) / bot.x;
      if (param > 0 && param < 1) {
        pointBounds(Edge_point(edge, param), prc);
      }
    }
    if (bot.y) {
      double param = (p[1].y - p[0].y) / bot.y;
      if (param > 0 && param < 1) {
        pointBounds(Edge_point(edge, param), prc);
      }
    }
    break;
  case CubicSegment:
    pointBounds(p[0], prc);
    pointBounds(p[3], prc);
    a0 = dPOINT_sub(p[1], p[0]);
    a1 = dPOINT(2 * p[2].x - p[1].x - a0.x, 2 * p[2].y - p[1].y - a0.y);
    a2 = dPOINT(p[3].x - 3 * p[2].x + 3 * p[1].x - p[0].x, p[3].y - 3 * p[2].y + 3 * p[1].y - p[0].y);
    solutions = solveQuadratic(params, a2.x, a1.x, a0.x);
    for (i = 0; i < solutions; ++i) {
      if (params[i] > 0 && params[i] < 1) {
        pointBounds(Edge_point(edge, params[i]), prc);
      }
    }
    solutions = solveQuadratic(params, a2.y, a1.y, a0.y);
    for (i = 0; i < solutions; ++i) {
      if (params[i] > 0 && params[i] < 1) {
        pointBounds(Edge_point(edge, params[i]), prc);
      }
    }
    break;
  }
  return 0;
}
int Shape_bounds(const Shape* shape, DRECT* prc)
{
  int i, j;
  for (i = 0; i < shape->len; ++i) {
    Contour* contour = shape->contours + i;
    for (j = 0; j < contour->len; ++j) {
      EdgeSegment* edge = contour->edges + j;
      Edge_bounds(edge, prc);
    }
  }
  return 0;
}
static bool isCorner(DPOINT aDir, DPOINT bDir, double crossThreshold)
{
  return dPOINT_dotProduct(aDir, bDir) <= 0 || fabs(dPOINT_crossProduct(aDir, bDir)) > crossThreshold;
}
static int switchColor(EdgeColor* color, uint64* seed, EdgeColor banned/* = BLACK*/)
{
  int shifted;
  EdgeColor combined = (EdgeColor)(*color & banned);
  if (combined == RED || combined == GREEN || combined == BLUE) {
    *color = (EdgeColor)(combined ^ WHITE);
    return 0;
  }
  if (*color == BLACK || *color == WHITE) {
    static const EdgeColor start[3] = { CYAN, MAGENTA, YELLOW };
    *color = start[*seed % 3];
    *seed /= 3;
    return 0;
  }
  shifted = *color << (1 + (*seed & 1));
  *color = (EdgeColor)((shifted | shifted >> 3)&WHITE);
  *seed >>= 1;
  return 0;
}
int edgeColoringSimple(Shape* shape, double angleThreshold, uint64 seed)
{
  double crossThreshold = sin(angleThreshold);
  int i, j, n = 0, contour_i, corner;
  int corners[100];
  for (contour_i = 0; contour_i < shape->len; ++contour_i) {
    Contour* contour = shape->contours + contour_i;
    // Identify corners
    n = 0;
    if (contour->len > 0) {
      DPOINT prevDirection = Edge_direction(contour->edges + contour->len - 1, 1);
      for (j = 0; j < contour->len; ++j) {
        EdgeSegment* edge = contour->edges + j;
        if (isCorner(dPOINT_normalize(prevDirection, false), dPOINT_normalize(Edge_direction(edge, 0), false), crossThreshold)) {
          corners[n++] = j;
        }
        prevDirection = Edge_direction(edge, 1);
      }
    }
    // Smooth contour
    if (0 == n) {
      for (j = 0; j < contour->len; ++j) {
        EdgeSegment* edge = contour->edges + j;
        edge->color = WHITE;
      }
    }
    else if (1 == n) {
      // "Teardrop" case
      EdgeColor colors[3] = { WHITE, WHITE };
      switchColor(&colors[0], &seed, BLACK);
      colors[2] = colors[0];
      switchColor(&colors[2], &seed, BLACK);
      corner = corners[0];
      if (contour->len >= 3) {
        int m = contour->len;
        for (i = 0; i < m; ++i) {
          contour->edges[(corner + i) % m].color = (colors + 1)[(int)(3 + 2.875 * i / (m - 1) - 1.4375 + .5) - 3];
        }
      }
      else if (contour->len >= 1) {
        // Less than three edge segments for three colors => edges must be split
        EdgeSegment* parts[7] = {0};
        EdgeSegment ebuf_[7] = {0};
        EdgeSegment* ebuf = ebuf_;
        ebuf = Edge_splitInThirds(contour->edges, ebuf, &parts[0 + 3 * corner], &parts[1 + 3 * corner], &parts[2 + 3 * corner]);
        if (contour->len >= 2) {
          ebuf = Edge_splitInThirds(contour->edges + 1, ebuf, &parts[3 - 3 * corner], &parts[4 - 3 * corner], &parts[5 - 3 * corner]);
          parts[0]->color = parts[1]->color = colors[0];
          parts[2]->color = parts[3]->color = colors[1];
          parts[4]->color = parts[5]->color = colors[2];
        }
        else {
          parts[0]->color = colors[0];
          parts[1]->color = colors[1];
          parts[2]->color = colors[2];
        }
        contour->len = 0;
        MYREALLOC(contour->edges, 7);
        for (i = 0; parts[i]; ++i) {
          contour->edges[contour->len++] = *parts[i];
        }
      }
    }
    // Multiple corners
    else {
      int cornerCount = n;
      int spline = 0;
      int start = corners[0];
      int m = contour->len;
      EdgeColor initialColor, color = WHITE;
      switchColor(&color, &seed, BLACK);
      initialColor = color;
      for (i = 0; i < m; ++i) {
        int index = (start + i) % m;
        if (spline + 1 < cornerCount && corners[spline + 1] == index) {
          ++spline;
          switchColor(&color, &seed, (EdgeColor)((spline == cornerCount - 1)*initialColor));
        }
        contour->edges[index].color = color;
      }
    }
  }
  return 0;
}
int Edge_distanceToPseudoDistance(const EdgeSegment* edge, SignedDistance* distance, DPOINT origin)
{
  double param = distance->param;
  if (param < 0) {
    DPOINT dir = dPOINT_normalize(Edge_direction(edge, 0), false);
    DPOINT aq = dPOINT_sub(origin, Edge_point(edge, 0));
    double ts = dPOINT_dotProduct(aq, dir);
    if (ts < 0) {
      double pseudoDistance = dPOINT_crossProduct(aq, dir);
      if (fabs(pseudoDistance) <= fabs(distance->distance)) {
        distance->distance = pseudoDistance;
        distance->dot = 0;
      }
    }
  }
  else if (param > 1) {
    DPOINT dir = dPOINT_normalize(Edge_direction(edge, 1), false);
    DPOINT bq = dPOINT_sub(origin, Edge_point(edge, 1));
    double ts = dPOINT_dotProduct(bq, dir);
    if (ts > 0) {
      double pseudoDistance = dPOINT_crossProduct(bq, dir);
      if (fabs(pseudoDistance) <= fabs(distance->distance)) {
        distance->distance = pseudoDistance;
        distance->dot = 0;
      }
    }
  }
  return 0;
}
#define MEDIAN(a, b, c) MAX(MIN(a, b), MIN(MAX(a, b), c))
float fmin(float a, float b)
{
  return MIN(a, b);
}
float fmax(float a, float b)
{
  return MAX(a, b);
}
float fmedian(float a, float b, float c)
{
  return fmax(MIN(a, b), fmin(MAX(a, b), c));
}
int generateSDF(img_t* output, const Shape* shape, double range, DPOINT scale, DPOINT translate, bool inverseYAxis)
{
  int i, j, x, y, w = output->w, h = output->h;
  for (y = 0; y < h; ++y) {
    int row = inverseYAxis ? h - y - 1 : y;
    float* output1 = (float*)(output->tt.data + output->s * row);
    for (x = 0; x < w; ++x) {
      DPOINT p = dPOINT((x + .5) / scale.x - translate.x, (y + .5) / scale.y - translate.y);
      SignedDistance minDistance[1] = {LARGE_VALUE, 1, 0};
      for (i = 0; i < shape->len; ++i) {
        Contour* contour = shape->contours + i;
        for (j = 0; j < contour->len; ++j) {
          EdgeSegment* edge = contour->edges + j;
          SignedDistance distance[1] = {LARGE_VALUE, 1, 0};
          signedDistance(edge, p, distance);
          if (SignedDistance_less(distance, minDistance)) {
            *minDistance = *distance;
          }
        }
      }
      output1[x] = (float)(minDistance->distance / range + .5);
    }
  }
  return 0;
}
int generatePseudoSDF(img_t* output, const Shape* shape, double range, DPOINT scale, DPOINT translate, bool inverseYAxis)
{
  int i, j, x, y, w = output->w, h = output->h;
  for (y = 0; y < h; ++y) {
    int row = inverseYAxis ? h - y - 1 : y;
    float* output1 = (float*)(output->tt.data + output->s * row);
    for (x = 0; x < w; ++x) {
      DPOINT p = dPOINT((x + .5) / scale.x - translate.x, (y + .5) / scale.y - translate.y);
      SignedDistance minDistance[1] = {LARGE_VALUE, 1, 0};
      const EdgeSegment* nearEdge = NULL;
      for (i = 0; i < shape->len; ++i) {
        Contour* contour = shape->contours + i;
        for (j = 0; j < contour->len; ++j) {
          EdgeSegment* edge = contour->edges + j;
          SignedDistance distance[1] = {LARGE_VALUE, 1, 0};
          signedDistance(edge, p, distance);
          if (SignedDistance_less(distance, minDistance)) {
            *minDistance = *distance;
            nearEdge = edge;
          }
        }
      }
      if (nearEdge) {
        Edge_distanceToPseudoDistance(nearEdge, minDistance, p);
      }
      output1[x] = (float)(minDistance->distance / range + .5);
    }
  }
  return 0;
}
CC_INLINE bool pixelClash(FloatRGB a, FloatRGB b, double threshold)
{
  float aa, ab, ba, bb, ac, bc;
  // Only consider pair where both are on the inside or both are on the outside
  bool aIn = (a.r > .5f) + (a.g > .5f) + (a.b > .5f) >= 2;
  bool bIn = (b.r > .5f) + (b.g > .5f) + (b.b > .5f) >= 2;
  if (aIn != bIn) {
    return false;
  }
  // If the change is 0 <-> 1 or 2 <-> 3 channels and not 1 <-> 1 or 2 <-> 2, it is not a clash
  if ((a.r > .5f && a.g > .5f && a.b > .5f) || (a.r < .5f && a.g < .5f && a.b < .5f)
      || (b.r > .5f && b.g > .5f && b.b > .5f) || (b.r < .5f && b.g < .5f && b.b < .5f)) {
    return false;
  }
  // Find which color is which: _a, _b = the changing channels, _c = the remaining one
  if ((a.r > .5f) != (b.r > .5f) && (a.r < .5f) != (b.r < .5f)) {
    aa = a.r, ba = b.r;
    if ((a.g > .5f) != (b.g > .5f) && (a.g < .5f) != (b.g < .5f)) {
      ab = a.g, bb = b.g;
      ac = a.b, bc = b.b;
    }
    else if ((a.b > .5f) != (b.b > .5f) && (a.b < .5f) != (b.b < .5f)) {
      ab = a.b, bb = b.b;
      ac = a.g, bc = b.g;
    }
    else {
      return false; // this should never happen
    }
  }
  else if ((a.g > .5f) != (b.g > .5f) && (a.g < .5f) != (b.g < .5f)
      && (a.b > .5f) != (b.b > .5f) && (a.b < .5f) != (b.b < .5f)) {
    aa = a.g, ba = b.g;
    ab = a.b, bb = b.b;
    ac = a.r, bc = b.r;
  }
  else {
    return false;
  }
  // Find if the channels are in fact discontinuous
  return (fabsf(aa - ba) >= threshold)
      && (fabsf(ab - bb) >= threshold)
      && fabsf(ac - .5f) >= fabsf(bc - .5f); // Out of the pair, only flag the pixel farther from a shape edge
}
int msdfErrorCorrection(img_t* output, DPOINT threshold)
{
  int* clashes = NULL;
  int i, n = 0, x, y, w = output->w, h = output->h;
  FloatRGB* output1 = (FloatRGB*)output->tt.data;
  MYREALLOC(clashes, h * w * 2);
  for (y = 0; y < h; ++y) {
    for (x = 0; x < w; ++x) {
      int pos = y * w + x;
      if ((x > 0 && pixelClash(output1[pos], output1[pos - 1], threshold.x))
          || (x < w - 1 && pixelClash(output1[pos], output1[pos + 1], threshold.x))
          || (y > 0 && pixelClash(output1[pos], output1[pos - w], threshold.y))
          || (y < h - 1 && pixelClash(output1[pos], output1[pos + w], threshold.y))) {
        clashes[n++] = x;
        clashes[n++] = y;
      }
    }
  }
  for (i = 0; i < n; i += 2) {
    FloatRGB* pixel = &output1[clashes[i] + clashes[i + 1] * w];
    float med = fmedian(pixel->r, pixel->g, pixel->b);
    pixel->r = med, pixel->g = med, pixel->b = med;
  }
  FREE(clashes);
  return 0;
}
int generateMSDF(img_t* output, const Shape* shape, double range, DPOINT scale, DPOINT translate, double edgeThreshold, bool inverseYAxis)
{
  int i, j, x, y, w = output->w, h = output->h;
  for (y = 0; y < h; ++y) {
    int row = inverseYAxis ? h - y - 1 : y;
    FloatRGB* output1 = (FloatRGB*)(output->tt.data + output->s * row);
    for (x = 0; x < w; ++x) {
      DPOINT p = dPOINT((x + .5) / scale.x - translate.x, (y + .5) / scale.y - translate.y);
      typedef struct {
        SignedDistance minDistance[1];
        const EdgeSegment* nearEdge;
      } RGBSignedDistance;
      RGBSignedDistance r, g, b;
      r.minDistance->distance = g.minDistance->distance = b.minDistance->distance = LARGE_VALUE;
      r.nearEdge = g.nearEdge = b.nearEdge = NULL;
      for (i = 0; i < shape->len; ++i) {
        Contour* contour = shape->contours + i;
        for (j = 0; j < contour->len; ++j) {
          EdgeSegment* edge = contour->edges + j;
          SignedDistance distance[1] = {LARGE_VALUE, 1, 0};
          signedDistance(edge, p, distance);
          if (edge->color & RED && SignedDistance_less(distance, r.minDistance)) {
            *r.minDistance = *distance;
            r.nearEdge = edge;
          }
          if (edge->color & GREEN && SignedDistance_less(distance, g.minDistance)) {
            *g.minDistance = *distance;
            g.nearEdge = edge;
          }
          if (edge->color & BLUE && SignedDistance_less(distance, b.minDistance)) {
            *b.minDistance = *distance;
            b.nearEdge = edge;
          }
        }
      }
      if (r.nearEdge) {
        Edge_distanceToPseudoDistance(r.nearEdge, r.minDistance, p);
      }
      if (g.nearEdge) {
        Edge_distanceToPseudoDistance(g.nearEdge, g.minDistance, p);
      }
      if (b.nearEdge) {
        Edge_distanceToPseudoDistance(b.nearEdge, b.minDistance, p);
      }
      output1[x].r = (float)(r.minDistance->distance / range + .5);
      output1[x].g = (float)(g.minDistance->distance / range + .5);
      output1[x].b = (float)(b.minDistance->distance / range + .5);
    }
  }
  if (edgeThreshold > 0) {
    DPOINT pt = dPOINT(edgeThreshold / (scale.x * range), edgeThreshold / (scale.y * range));
    msdfErrorCorrection(output, pt);
  }
  return 0;
}
static float sample(const img_t* bitmap, DPOINT pos)
{
  int w = bitmap->w, h = bitmap->h;
  float* pix = (float*)bitmap->tt.data;
  double x = pos.x * w - .5;
  double y = pos.y * h - .5;
  int l = (int) floor(x);
  int b = (int) floor(y);
  int r = l + 1;
  int t = b + 1;
  double lr = x - l;
  double bt = y - b;
  l = BOUND(l, 0, w - 1), r = BOUND(r, 0, w - 1);
  b = BOUND(b, 0, h - 1), t = BOUND(t, 0, h - 1);
  return fmix(fmix(pix[l + b * w], pix[r + b * w], lr), fmix(pix[l + t * w], pix[r + t * w], lr), bt);
}
static FloatRGB sampleRGB(const img_t* bitmap, DPOINT pos)
{
  int w = bitmap->w, h = bitmap->h;
  FloatRGB* pix = (FloatRGB*)bitmap->tt.data;
  double x = pos.x * w - .5;
  double y = pos.y * h - .5;
  int l = (int) floor(x);
  int b = (int) floor(y);
  int r = l + 1;
  int t = b + 1;
  double lr = x - l;
  double bt = y - b;
  l = BOUND(l, 0, w - 1), r = BOUND(r, 0, w - 1);
  b = BOUND(b, 0, h - 1), t = BOUND(t, 0, h - 1);
  return mixRGB(mixRGB(pix[l + b * w], pix[r + b * w], lr), mixRGB(pix[l + t * w], pix[r + t * w], lr), bt);
}
static float distVal(float dist, double pxRange)
{
  float ret;
  if (!pxRange) {
    return dist > .5;
  }
  ret = (dist - .5) * pxRange + .5;
  return BOUND(ret, 0.f, 1.f);
}
int renderSDF(img_t* output, const img_t* sdf, double pxRange)
{
  int x, y, w = output->w, h = output->h;
  pxRange *= (double)(w + h) / (sdf->w + sdf->h);
  for (y = 0; y < h; ++y) {
    float* output1 = (float*)(output->tt.data + output->s * y);
    for (x = 0; x < w; ++x) {
      float s = sample(sdf, dPOINT((x + .5) / w, (y + .5) / h));
      output1[x] = distVal(s, pxRange);
    }
  }
  return 0;
}
int renderSDFRGB(img_t* output, const img_t* sdf, double pxRange)
{
  int x, y, w = output->w, h = output->h;
  pxRange *= (double)(w + h) / (sdf->w + sdf->h);
  for (y = 0; y < h; ++y) {
    FloatRGB* output1 = (FloatRGB*)(output->tt.data + output->s * y);
    for (x = 0; x < w; ++x) {
      float s = sample(sdf, dPOINT((x + .5) / w, (y + .5) / h));
      float v = distVal(s, pxRange);
      output1[x].r = v;
      output1[x].g = v;
      output1[x].b = v;
    }
  }
  return 0;
}
int renderMSDF(img_t* output, const img_t* msdf, double pxRange)
{
  int x, y, w = output->w, h = output->h;
  pxRange *= (double)(w + h) / (msdf->w + msdf->h);
  for (y = 0; y < h; ++y) {
    float* output1 = (float*)(output->tt.data + output->s * y);
    for (x = 0; x < w; ++x) {
      FloatRGB s = sampleRGB(msdf, dPOINT((x + .5) / w, (y + .5) / h));
      output1[x] = distVal(fmedian(s.r, s.g, s.b), pxRange);
    }
  }
  return 0;
}
int renderMSDFRGB(img_t* output, const img_t* msdf, double pxRange)
{
  int x, y, w = output->w, h = output->h;
  pxRange *= (double)(w + h) / (msdf->w + msdf->h);
  for (y = 0; y < h; ++y) {
    FloatRGB* output1 = (FloatRGB*)(output->tt.data + output->s * y);
    for (x = 0; x < w; ++x) {
      FloatRGB s = sampleRGB(msdf, dPOINT((x + .5) / w, (y + .5) / h));
      output1[x].r = distVal(s.r, pxRange);
      output1[x].g = distVal(s.g, pxRange);
      output1[x].b = distVal(s.b, pxRange);
    }
  }
  return 0;
}
int simulate8bit(img_t* bitmap)
{
  int x, y, w = bitmap->w, h = bitmap->h;
  for (y = 0; y < h; ++y) {
    float* bitmap1 = (float*)(bitmap->tt.data + bitmap->s * y);
    for (x = 0; x < w; ++x) {
      int v = (int)(bitmap1[x] * 0x100);
      v = BOUND(v, 0, 0xff);
      bitmap1[x] = v / 255.f;
    }
  }
  return 0;
}
int simulate8bitRGB(img_t* bitmap)
{
  int x, y, w = bitmap->w, h = bitmap->h;
  for (y = 0; y < h; ++y) {
    FloatRGB* bitmap1 = (FloatRGB*)(bitmap->tt.data + bitmap->s * y);
    for (x = 0; x < w; ++x) {
      int r, g, b;
      r = (int)(bitmap1[x].r * 0x100);
      g = (int)(bitmap1[x].g * 0x100);
      b = (int)(bitmap1[x].b * 0x100);
      r = BOUND(r, 0, 0xff);
      g = BOUND(g, 0, 0xff);
      b = BOUND(b, 0, 0xff);
      bitmap1[x].r = r / 255.f;
      bitmap1[x].g = g / 255.f;
      bitmap1[x].b = b / 255.f;
    }
  }
  return 0;
}

