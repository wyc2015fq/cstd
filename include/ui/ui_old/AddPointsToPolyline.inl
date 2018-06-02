
typedef struct VectorLine {
  DPOINT Point1, Point2;
} VectorLine;

// <summary>
// Finds which side of a line the point is
// </summary>
// <param name="PointToBeEvaluated">Evaluation point</param>
// <param name="StartPointOnLine">Startpoint of line</param>
// <param name="EndPointOnLine">Endpoint on line</param>
// <returns>-1 for a point to the right, 0 for a point on the line, +1 for a point to the left</returns>
// <remarks></remarks>
static int WhichSide(DPOINT PointToBeEvaluated, DPOINT StartPointOnLine, DPOINT EndPointOnLine)
{
  double ReturnvalueEquation = 0;
  ReturnvalueEquation = ((PointToBeEvaluated.y - StartPointOnLine.y) * (EndPointOnLine.x - StartPointOnLine.x)) - ((EndPointOnLine.y - StartPointOnLine.y) * (PointToBeEvaluated.x - StartPointOnLine.x));

  if (ReturnvalueEquation > 0) {
    return -1;
  }
  else if (ReturnvalueEquation == 0) {
    return 0;
  }
  else {
    return 1;
  }
}

// <summary>
// Returns the distance from a point to a line
// </summary>
// <param name="LinePoint1"></param>
// <param name="LinePoint2"></param>
// <param name="TestPoint"></param>
// <returns></returns>
// <remarks></remarks>
static double DistanceFromLine(DPOINT LinePoint1, DPOINT LinePoint2, DPOINT TestPoint)
{
  double d = 0;
  d = fabs((LinePoint2.x - LinePoint1.x) * (LinePoint1.y - TestPoint.y) - (LinePoint1.x - TestPoint.x) * (LinePoint2.y - LinePoint1.y));
  d = d / sqrt(pow((LinePoint2.x - LinePoint1.x), 2) + pow((LinePoint2.y - LinePoint1.y), 2));
  return d;
}

static double DistanceBetweenPoints(DPOINT Point1, DPOINT Point2)
{
  return sqrt(pow((Point1.x - Point2.x), 2) + pow((Point1.y - Point2.y), 2));
}

// <summary>
// Calculate the angel in degrees between point 1 and 3 at point 2
// </summary>
// <param name="Point1"></param>
// <param name="Point2"></param>
// <param name="Point3"></param>
// <returns></returns>
// <remarks></remarks>
static double Angles(DPOINT Point1, DPOINT Point2, DPOINT Point3)
{
  double result = 0;
  double a = 0;
  double b = 0;
  double c = 0;
  c = DistanceBetweenPoints(Point1, Point3);
  b = DistanceBetweenPoints(Point1, Point2);
  a = DistanceBetweenPoints(Point2, Point3);
  result = acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * b * a));
  return result;
}

// <summary>
// Calculates the Normal vector at point 1
// </summary>
// <param name="Point1"></param>
// <param name="point2"></param>
// <returns></returns>
// <remarks></remarks>
static DPOINT Normal2D(DPOINT Point1, DPOINT point2, double theta)
{
  DPOINT p = {0};

  p.x = cos(theta) * (point2.x - Point1.x) - sin(theta) * (point2.y - Point1.y) + Point1.x;
  p.y = sin(theta) * (point2.x - Point1.x) + cos(theta) * (point2.y - Point1.y) + Point1.y;
  return p;
}


static double DistanceFromLine2(DPOINT TestPoint, VectorLine vector1, VectorLine vector2)
{

  int FirstTest = 0;
  int SecondTest = 0;

  FirstTest = WhichSide(TestPoint, vector1.Point1, vector1.Point2);
  SecondTest = WhichSide(TestPoint, vector2.Point1, vector2.Point2);

  if (FirstTest != SecondTest) {
    return DistanceFromLine(vector1.Point1, vector2.Point1, TestPoint);
  }
  else {
    return DBL_MAX;
  }

}

static int CalculateAllAngles(INT n, const DPOINT* OriginalPointCollection, VectorLine* result)
{
  int i, j = 0;

  for (i = 0; i <= n - 1; i++) {
    VectorLine NewVectorLine = {0};

    if (i == 0) {
      NewVectorLine.Point2 = Normal2D(OriginalPointCollection[i], OriginalPointCollection[i + 1], PI / 2);
      NewVectorLine.Point1 = OriginalPointCollection[i];
      result[j++] = (NewVectorLine);
    }
    else if (i == n - 1) {
      NewVectorLine.Point2 = Normal2D(OriginalPointCollection[i], OriginalPointCollection[i - 1], 3 * PI / 2);
      NewVectorLine.Point1 = OriginalPointCollection[i];
      result[j++] = (NewVectorLine);
    }
    else {
      int PreviousAngle, NextAngle;
      double angl;

      NewVectorLine.Point1 = OriginalPointCollection[i];
      angl = Angles(OriginalPointCollection[i - 1], OriginalPointCollection[i], OriginalPointCollection[i + 1]);

      PreviousAngle = 0;
      NextAngle = 0;
      PreviousAngle = WhichSide(result[i - 1].Point2, OriginalPointCollection[i - 1], OriginalPointCollection[i]);
      NextAngle = WhichSide(OriginalPointCollection[i + 1], OriginalPointCollection[i - 1], OriginalPointCollection[i]);

      if (PreviousAngle == NextAngle) {
        NewVectorLine.Point2 = Normal2D(OriginalPointCollection[i], OriginalPointCollection[i + 1] , angl / 2);
      }
      else {
        NewVectorLine.Point2 = Normal2D(OriginalPointCollection[i], OriginalPointCollection[i + 1], (2 * PI - angl) / 2);
      }

      result[j++] = (NewVectorLine);
    }
  }

  return j;
}


// <summary>
// Insert a new point to the line
// </summary>
// <param name="result"></param>
// <param name="NewPoint"></param>
// <returns></returns>
// <remarks></remarks>
static int InsertPoint(int n, DPOINT* result, DPOINT NewPoint)
{
  VectorLine* VectorLinesCalc = 0;
  double min_distance = DBL_MAX;
  //For Each p As DPOINT In result
  //    min_distance += DistanceBetweenPoints(NewPoint, p)
  //Next
  double temp_distance = 0;
  int i, index = 0;

  VectorLinesCalc = MALLOC(VectorLine, n);
  CalculateAllAngles(n, result, VectorLinesCalc);

  for (i = 0; i <= n - 2; i++) {
    temp_distance = DistanceFromLine2(NewPoint, VectorLinesCalc[i], VectorLinesCalc[i + 1]);

    if (temp_distance < min_distance) {
      min_distance = temp_distance;
      index = i + 1;
    }
  }

  if (DistanceBetweenPoints(result[0], NewPoint) < min_distance) {
    min_distance = DistanceBetweenPoints(result[0], NewPoint);
    index = 0;
  }

  if (DistanceBetweenPoints(result[n - 1], NewPoint) < min_distance) {
    index = -1;
    min_distance = DistanceBetweenPoints(result[n - 1], NewPoint);
  }

  if (!(index == -1)) {
    //dPOLYGON2_insert(result, index, NewPoint);
    MEMMOVE(result + index + 1, result + index, n - index);
    result[index] = NewPoint;
  }
  else {
    //dPOLYGON2_add(result, NewPoint);
    result[n] = NewPoint;
  }

  FREE(VectorLinesCalc);

  return index;
}
