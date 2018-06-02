
typedef struct mLineSegment {
  DPOINT2 start_point, end_point;
} mLineSegment;

bool is_points_equal(DPOINT2 p1, DPOINT2 p2) // check if given two points are equal
{
  double dDeff_X = fabs(p1.x - p2.x);
  double dDeff_Y = fabs(p1.y - p2.y);

  if ((dDeff_X < 0.00001) && (dDeff_Y < 0.00001)) { // if they're to near to each other, we can say they're equal
    return true;
  }
  else {
    return false;
  }
}

mLineSegment make_mLineSegment(DPOINT2 start, DPOINT2 end) // constructor
{
  mLineSegment ret;
  ret.start_point = start;
  ret.end_point = end;
  return ret;
}

double get_lenght(mLineSegment s)  // find line's lenght
{
  double d = (s.end_point.x - s.start_point.x) * (s.end_point.x - s.start_point.x);
  d += (s.end_point.y - s.start_point.y) * (s.end_point.y - s.start_point.y);
  d = sqrt(d);
  return d;
}

double get_x_max(mLineSegment s) // maximum x point
{
  return MAX(s.start_point.x, s.end_point.x);
}
double get_x_min(mLineSegment s) // minumum x point
{
  return  MIN(s.start_point.x, s.end_point.x);
}
double get_y_max(mLineSegment s) // maximum y point
{
  return MAX(s.start_point.y, s.end_point.y);
}
double get_y_min(mLineSegment s) // minumum y point
{
  return  MIN(s.start_point.y, s.end_point.y);
}


enum VertexType {
  ErrorPoint,
  ConvexPoint,
  ConcavePoint
};
enum PolygonDirection {
  Unknown1,
  Clockwise1,
  Count_Clockwise1
};

int po_vertex_index(int n, const DPOINT2* po, DPOINT2 p) // returns given points vertex index
{
  int i;

  for (i = 0; i < n; i++) {
    if ((fabs(po[i].x - p.x) + fabs(po[i].y - p.y)) < 0.0001) {
      return i;
    }
  }

  return -1;
}

BOOL po_is_vertex(int n, const DPOINT2* po, DPOINT2 p) // check given point is vertex of the polygon
{
  if (po_vertex_index(n, po, p) != -1) {
    return true;
  }
  else {
    return false;
  }
}

double po_area(int n, const DPOINT2* points)    // calculates the given points area
{
  double area = 0;
  int i, j;

  for (i = 0; i < n; i++) {
    j = (i + 1) % n;
    area += (points[i].x * points[j].y);
    area -= (points[i].y * points[j].x);
  }

  area /= 2;
  return area;
}

static int get_direction(int n, const DPOINT2* points) // find given points direction
{
  int i, j, k, count, len;

  if (n < 3) {
    return Unknown1;  // we need at least 3 points!
  }

  len = n;
  count = 0;

  for (i = 0; i < len; i++) { // loop through points
    double cross_product;
    //    i         j       k
    // current -> next -> next
    j = (i + 1) % len;  // next point
    k = (i + 2) % len;  // next point's next point

    // calculate cross products
    cross_product = (points[j].x - points[i].x) * (points[k].y - points[j].y);
    cross_product = cross_product - ((points[j].y - points[i].y) * (points[k].x - points[j].x));

    if (cross_product > 0) { // if cross product > 0
      count++;  // increment in positive
    }
    else {
      count--;  // increment in negative
    }
  }

  if (count < 0) {            // if negative
    return Count_Clockwise1;  // counter-clockwise
  }
  else if (count > 0) {       // if positive
    return Clockwise1;  // clockwise
  }
  else {
    return Unknown1;  // if 0, unknown!
  }
}

static void reverse_direction(int n, DPOINT2* points) // reverses given points directions
{
  int i;
  DPOINT2 tmp;    // temp. location

  for (i = 0; i < n / 2; ++i) { // copy points to temp. location
    CV_SWAP(points[i], points[n - 1 - i], tmp);
  }
}

BOOL is_point_in_line(DPOINT2 p, mLineSegment line) // is given point lies in given line?
{
  double line_lenght, s;
  double Ax, Ay, Bx, By, Cx, Cy;
  Bx = line.end_point.x;
  By = line.end_point.y;
  Ax = line.start_point.x;
  Ay = line.start_point.y;
  Cx = p.x;
  Cy = p.y;

  line_lenght = get_lenght(line);

  s = fabs(((Ay - Cy) * (Bx - Ax) - (Ax - Cx) * (By - Ay)) / (line_lenght * line_lenght));

  if (fabs(s - 0) < 0.00001) { // does it lie in or very near to line?
    if (is_points_equal(p, line.start_point) || is_points_equal(p, line.end_point)) {
      return true;
    }
    else if ((Cx < get_x_max(line)) && (Cx > get_x_min(line)) && (Cy < get_y_max(line)) && (Cy > get_y_min(line))) {
      return true;
    }
  }

  // if CS:IP reaches here, means point does not lie in the line
  return false;
}

#include "geo/point.inl"

BOOL is_point_in_triangle(int n, const DPOINT2* triangle, DPOINT2 p) // check if given point lies in a given triangle
{
  int i;
  mLineSegment l1, l2, l3;
  double area0, area1, area2;
  DPOINT2 point[3];

  if (n != 3) { // check if we have exact-3 points of triangle
    return false;
  }

  for (i = 0; i < n; i++) { // loop triangles vertexes
    if (is_points_equal(p, triangle[i])) { // given point is equal or very near to one of given triangles vertices
      return true;
    }
  }

  // check line segments of given triangle
  l1.start_point = triangle[0], l1.end_point = triangle[1];
  l2.start_point = triangle[1], l2.end_point = triangle[2];
  l3.start_point = triangle[2], l3.end_point = triangle[0];

  if (is_point_in_line(p, l1) || is_point_in_line(p, l2) || is_point_in_line(p, l3)) { // if point lies in one of triangles lines
    return true;
  }

  // check interior triangle area
  area0 = po_area(3, ARRINIT3(point, triangle[0], triangle[1], p));
  area1 = po_area(3, ARRINIT3(point, triangle[1], triangle[2], p));
  area2 = po_area(3, ARRINIT3(point, triangle[2], triangle[0], p));

  if ((area0 > 0) && (area1 > 0) && (area2 > 0)) {
    return true;
  }
  else if ((area0 < 0) && (area1 < 0) && (area2 < 0)) {
    return true;
  }

  // if CS:IP reaches here, this means given point passed test and not in anywhere in the triangle
  return false;
}

BOOL is_ear(int n, const DPOINT2* po, const int* index, int ip) // checks if given vertice is in a ear
{
  int i;
  DPOINT2 point[3];
  int ip0 = (ip + n - 1) % n, ip1 = (ip + n + 1) % n;
  point[0] = po[index[ip0]]; // find previous adjacent point
  point[1] = po[index[ip]];
  point[2] = po[index[ip1]]; // find next adjacent point

  //mPolygon m = new mPolygon(updated_vertices); // init. a polygon from the current vertices
  if (po_area(3, point) < 0) { // and it's a convex point
    for (i = 0; i < n; i++) { // loop through all other vertices
      if (i != ip && i != ip0 && i != ip1) {
        // if pt is not equal to checked vertice or its's next and prev adjacent vertices
        if (is_point_in_triangle(3, point, po[index[i]])) {
          // check pt lies in triangle
          // if another vertice lies in this triangle, then this is not an ear
          return false;
        }
      }
    }
  }
  else { // concave
    return false;  // we cannot make ears from concave points
  }

  return true;    // if CS:IP reaches here, this means vertice passed the test and is an ear
}


int po_triangulate(int n, const DPOINT2* po, int* ears) // constructure
{
  int i;
  int* index = 0;
  int nears = 0;

  if (n < 3) { // polygon needs to have at least 3 points
    return 0;
  }

  index = MALLOC(int, n);

  for (i = 0; i < n; ++i) {
    index[i] = i;
  }

  // check input vertices direction
  // we need counter clockwise
  if (get_direction(n, po) == Clockwise1) {
    for (i = 0; i < n; ++i) {
      index[i] = n - 1 - i;
    }
  }

  while (n >= 3) {
    int ie = n - 1;

    for (i = 0; i < n; ++i) {
      if (is_ear(n, po, index, i)) {             // check if we can get an ear from that vertice
        ie = i;
        break;      // good we found one
      }
    }

    ASSERT(ie >= 0);
    // removes the vertice from updates_vertices list
    ears[nears * 3 + 0] = index[(ie + n - 1) % n]; // find the vertices next neighboor
    ears[nears * 3 + 1] = index[ie];
    ears[nears * 3 + 2] = index[(ie + n + 1) % n]; // find the vertices previous neighboor
    ++nears;
    MEMCPY(index + ie, index + ie + 1, n - 1 - ie);
    n--;
  }

  FREE(index);
  return nears;
}

int po_traverse(int n, const int* tr_index, int* clrs) // travers the triangulated polygons list for assinging 3-colors
{
  int i, nn = n;
  int map33[3 * 3] = {
    -1, 2, 1,
    2, -1, 0,
    1, 0, -1,
  };

  for (i = 0; i < n; ++i) {
    clrs[tr_index[i * 3 + 0]] = -1;
    clrs[tr_index[i * 3 + 1]] = -1;
    clrs[tr_index[i * 3 + 2]] = -1;
  }

  // directly assign last polygons vertex's colors
  clrs[tr_index[0]] = 0;
  clrs[tr_index[1]] = 1;
  clrs[tr_index[2]] = 2;
  nn = n - 1;

  for (; nn > 0;) {
    for (i = 1; i < n; ++i) {
      int t0 = tr_index[i * 3 + 0], t1 = tr_index[i * 3 + 1], t2 = tr_index[i * 3 + 2];

      if ((clrs[t0] >= 0) + (clrs[t1] >= 0) + (clrs[t2] >= 0) == 2) {
        --nn;

        if (clrs[t0] < 0) {
          clrs[t0] = map33[clrs[t1] * 3 + clrs[t2]];
        }
        else if (clrs[t1] < 0) {
          clrs[t1] = map33[clrs[t2] * 3 + clrs[t0]];
        }
        else if (clrs[t2] < 0) {
          clrs[t2] = map33[clrs[t0] * 3 + clrs[t1]];
        }
      }
    }
  }

  return 0;
}


#include "Triangle.inl"

int test_ArtGalleryProblem1(img_t* im)
{
  // drawing related
  //Pen polygon_pen = new Pen(Black, 2);
  COLOR brushes[] = { BlueViolet, Salmon, Yellow, };
  COLOR brushes2[] = { Red, Green, Blue, };
  enum {nn = 20};
  DPOINT2 po[nn];
  int tr2[3 * nn];
  int iclrs[nn] = {0};
  int i, ntr2 = 0;
  int n = nn;
  //srand(0);
  pt_random_4(nn, po, im->w / 2., im->h / 2., MIN(im->w, im->h) / 5., MIN(im->w, im->h) / 5);
  ntr2 = po_triangulate(n, po, tr2);
  po_traverse(ntr2, tr2, iclrs);

  imdrawaa_poly(im, po, n, 1, 0, Black, 1);

  // for every 3 triangle, use different color
  for (i = 0; i < ntr2; ++i) {
    DTRIANGLE2 dtr2 = pt2_make_triangle(po[tr2[i * 3 + 0]], po[tr2[i * 3 + 1]], po[tr2[i * 3 + 2]]);
    imdrawaa_triangles(im, &dtr2, 1, brushes[i % 3], Black, 1);
  }

  for (i = 0; i < n; ++i) {
    imdrawaa_circle(im, po[i].x, po[i].y, 5, brushes2[iclrs[i]], Black, 1);
  }

  return 0;
}
