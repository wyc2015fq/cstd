#define UPDATE_PROGRESS(xx)
#define SETPOINT2v(_pt, _v) ((_pt).x = _v[0], (_pt).y = _v[1])
#define SETPOINT3v(_pt, _v) ((_pt).x = _v[0], (_pt).y = _v[1], (_pt).z = _v[2])
#define SETPOINT3v_(_pt, _v) ((_pt).x = _v[0], (_pt).y = _v[1], (_pt).z = _v[2], (_pt).w = 1)
//#define InsertTuple(newNormals, numPoles, ptr) (newNormals[numPoles].x = ptr[0], newNormals[numPoles].y = ptr[1], newNormals[numPoles].z = ptr[2])
#define NORM3(v) sqrt( v[0] * v[0] + v[1] * v[1] + v[2] * v[2] )

//static float NORM3(const float3 v) { return static_cast<float> (sqrt( v[0] * v[0] + v[1] * v[1] + v[2] * v[2] ) );};

//#define InsertNextCell(ptr, n, v) memcpy(ptr, v, n*sizeof(*ptr)), ptr += n
typedef struct gcobj_t {
  ImVertex* vertex;
  short* index;
  int vertex_num;
  int index_num;
  int vertex_size;
  int index_size;
  GcMode mode;
} gcobj_t;
int gcobj_free(gcobj_t* o)
{
  FREE(o->vertex);
  FREE(o->index);
  return 0;
}
int gcobj_set_size(gcobj_t* o, GcMode mode, int vertex_size, int index_size)
{
  o->mode = mode;
  o->vertex_size = vertex_size;
  o->index_size = index_size;
  MYREALLOC(o->vertex, vertex_size);
  MYREALLOC(o->index, index_size);
  return 0;
}
int gcobj_add_vertex_fv(gcobj_t* o, const float* x, const float* n, float* tc)
{
  ImVertex* v = o->vertex + o->vertex_num;
  ASSERT(o->vertex_num < o->vertex_size);
  if (x) {
    SETPOINT3v_(v->pos, x);
  }
  if (n) {
    SETPOINT3v_(v->n, n);
  }
  if (tc) {
    SETPOINT2v(v->uv, tc);
  }
  v->col = _RGBA(255, 255, 255, 255);
  o->vertex_num++;
  return 0;
}
int gcobj_add_vertex_dv(gcobj_t* o, const double* x, const double* n, double* tc)
{
  ImVertex* v = o->vertex + o->vertex_num;
  ASSERT(o->vertex_num < o->vertex_size);
  if (x) {
    SETPOINT3v_(v->pos, x);
  }
  if (n) {
    SETPOINT3v_(v->n, n);
  }
  if (tc) {
    SETPOINT2v(v->uv, tc);
  }
  v->col = _RGBA(255, 255, 255, 255);
  o->vertex_num++;
  return 0;
}
int gcobj_add_index_iv(gcobj_t* o, int n, int* index)
{
  int i;
  ASSERT(o->index_num + n <= o->index_size);
  for (i = 0; i < n; ++i) {
    o->index[o->index_num++] = index[i];
  }
  return 0;
}
int gcobj_add_index_i4(gcobj_t* o, int i0, int i1, int i2, int i3)
{
  int i = o->index_num;
  ASSERT(i + 4 <= o->index_size);
  o->index[i + 0] = i0;
  o->index[i + 1] = i1;
  o->index[i + 2] = i2;
  o->index[i + 3] = i3;
  o->index_num += 4;
  return 0;
}
int gcobj_printf(gcobj_t* o)
{
  int i;
#define printf_point3(pt) printf("(%g,%g,%g)", (pt).x, (pt).y, (pt).z)
  for (i = 0; i < o->vertex_num; ++i) {
    printf_point3(o->vertex[i].pos);
    printf_point3(o->vertex[i].n);
    printf("\n");
  }
  for (i = 0; i < o->index_num; ++i) {
    printf("%d\n", o->index[i]);
  }
  return 0;
}
int cube1(gcobj_t* o)
{
  float n[] = {
    0.0F, 0.0F, 1.0F,
    0.0F, 0.0F, -1.0F,
    0.0F, 1.0F, 0.0F,
    0.0F, -1.0F, 0.0F,
    1.0F, 0.0F, 0.0F,
    -1.0F, 0.0F, 0.0F,
  };
  float x[] = {
    //0----------------------------
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    //1----------------------------
    -0.1f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    //2----------------------------
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    //3----------------------------
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    //4----------------------------
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    //5----------------------------
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
  };
  int i, j, vn = o->vertex_num;
  gcobj_set_size(o, GC_QUADS, vn + 24, 6 * 4);
  for (i = 0; i < 6; ++i) {
    for (j = 0; j < 4; ++j) {
      gcobj_add_vertex_fv(o, x + 12 * i + 3 * j, n + 3 * i, NULL);
    }
    j = i * 4 + vn;
    gcobj_add_index_i4(o, j + 0, j + 1, j + 2, j + 3);
  }
  return 0;
}
int cube(gcobj_t* o, double XLength, double YLength, double ZLength, double Center[3])
{
  double x[3], n[3], tc[3];
  //int numPolys=6, numPts=24;
  int i, j, k;
  int pts[4];
  double _Center[3] = {0, 0, 0};
  gcobj_set_size(o, GC_QUADS, o->vertex_num + 24, 6 * 4);
  Center = Center ? Center : _Center;
  x[0] = Center[0] - XLength / 2.0;
  n[0] = (-1.0), n[1] = n[2] = 0.0;
  for (i = 0; i < 2; i++, x[0] += XLength, n[0] += 2.0) {
    x[1] = Center[1] - YLength / 2.0;
    for (j = 0; j < 2; j++, x[1] += YLength) {
      tc[1] = x[1] + 0.5;
      x[2] = Center[2] - ZLength / 2.0;
      for (k = 0; k < 2; k++, x[2] += ZLength) {
        tc[0] = (x[2] + 0.5) * (1 - 2 * i);
        gcobj_add_vertex_dv(o, x, n, tc);
      }
    }
  }
  pts[0] = 0, pts[1] = 1, pts[2] = 3, pts[3] = 2;
  gcobj_add_index_iv(o, 4, pts);
  pts[0] = 4, pts[1] = 6, pts[2] = 7, pts[3] = 5;
  gcobj_add_index_iv(o, 4, pts);
  for (x[1] = Center[1] - YLength / 2.0, n[1] = (-1.0), n[0] = n[2] = 0.0, i = 0;
      i < 2; i++, x[1] += YLength, n[1] += 2.0) {
    for (x[0] = Center[0] - XLength / 2.0, j = 0; j < 2;
        j++, x[0] += XLength) {
      tc[0] = (x[0] + 0.5) * (2 * i - 1);
      for (x[2] = Center[2] - ZLength / 2.0, k = 0; k < 2;
          k++, x[2] += ZLength) {
        tc[1] = (x[2] + 0.5) * -1;
        gcobj_add_vertex_dv(o, x, n, tc);
      }
    }
  }
  pts[0] = 8, pts[1] = 10, pts[2] = 11, pts[3] = 9;
  gcobj_add_index_iv(o, 4, pts);
  pts[0] = 12, pts[1] = 13, pts[2] = 15, pts[3] = 14;
  gcobj_add_index_iv(o, 4, pts);
  for (x[2] = Center[2] - ZLength / 2.0, n[2] = (-1.0), n[0] = n[1] = 0.0, i = 0;
      i < 2; i++, x[2] += ZLength, n[2] += 2.0) {
    for (x[1] = Center[1] - YLength / 2.0, j = 0; j < 2;
        j++, x[1] += YLength) {
      tc[1] = x[1] + 0.5;
      for (x[0] = Center[0] - XLength / 2.0, k = 0; k < 2;
          k++, x[0] += XLength) {
        tc[0] = (x[0] + 0.5) * (2 * i - 1);
        gcobj_add_vertex_dv(o, x, n, tc);
      }
    }
  }
  pts[0] = 16, pts[1] = 18, pts[2] = 19, pts[3] = 17;
  gcobj_add_index_iv(o, 4, pts);
  pts[0] = 20, pts[1] = 21, pts[2] = 23, pts[3] = 22;
  gcobj_add_index_iv(o, 4, pts);
  return 1;
}
#if 0
res = res < 4 ? 4 : res;
Radius = 0.5;
Center[0] = 0.0;
Center[1] = 0.0;
Center[2] = 0.0;
ThetaResolution = res;
PhiResolution = res;
StartTheta = 0.0;
EndTheta = 360.0;
StartPhi = 0.0;
EndPhi = 180.0;
LatLongTessellation = 0;
#endif
int sphere(gcobj_t* o, double Radius, double Center[3], int ThetaResolution, int PhiResolution, double StartTheta, double EndTheta, double StartPhi, double EndPhi)
{
  int i, j;
  int jStart, jEnd, numOffset;
  int numPts, numPolys;
  double x[3], n[3], deltaPhi, deltaTheta, phi, theta, radius, norm;
  double startTheta, endTheta, startPhi, endPhi;
  int base, thetaResolution, phiResolution;
  int pts[4];
  int piece = 6;//outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER());
  int numPieces = 6;//outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES());
  int localThetaResolution;
  double localStartTheta;
  double localEndTheta;
  int start, end;
  int numPoles;
  if (numPieces > ThetaResolution) {
    numPieces = ThetaResolution;
  }
  if (piece >= numPieces) {
    // Although the super class should take care of this,
    // it cannot hurt to check here.
    return 1;
  }
  // I want to modify the ivars resoultion start theta and end theta,
  // so I will make local copies of them. THese might be able to be merged
  // with the other copies of them, ...
  localThetaResolution = ThetaResolution;
  localStartTheta = StartTheta;
  localEndTheta = EndTheta;
  while (localEndTheta < localStartTheta) {
    localEndTheta += 360.0;
  }
  deltaTheta = (localEndTheta - localStartTheta) / localThetaResolution;
  // Change the ivars based on pieces.
  start = piece * localThetaResolution / numPieces;
  end = (piece + 1) * localThetaResolution / numPieces;
  localEndTheta = localStartTheta + (double)(end) * deltaTheta;
  localStartTheta = localStartTheta + (double)(start) * deltaTheta;
  localThetaResolution = end - start;
  // Set things up; allocate memory
  printf("SphereSource Executing piece index %d of %d pieces.", piece, numPieces);
  numPts = PhiResolution * localThetaResolution + 2;
  // creating triangles
  numPolys = PhiResolution * 2 * localThetaResolution;
  gcobj_set_size(o, GC_TRIANGLES, numPts, numPolys * 4);
  //newPoints->Allocate(numPts);
  //newNormals->Allocate(3*numPts);
  //newPolys->Allocate(newPolys->EstimateSize(numPolys, 3));
  // Create sphere
  // Create north pole if needed
  if (StartPhi <= 0.0) {
    x[0] = Center[0];
    x[1] = Center[1];
    x[2] = Center[2] + Radius;
    n[0] = n[1] = 0.0, n[2] = 1.0;
    gcobj_add_vertex_dv(o, x, n, NULL);
  }
  // Create south pole if needed
  if (EndPhi >= 180.0) {
    x[0] = Center[0];
    x[1] = Center[1];
    x[2] = Center[2] - Radius;
    n[0] = n[1] = 0.0, n[2] = -1.0;
    gcobj_add_vertex_dv(o, x, n, NULL);
  }
  numPoles = o->vertex_num;
  UPDATE_PROGRESS(0.10);
  // Check data, determine increments, and convert to radians
  startTheta = (localStartTheta < localEndTheta ? localStartTheta : localEndTheta);
  startTheta *= M_PI / 180.0;
  endTheta = (localEndTheta > localStartTheta ? localEndTheta : localStartTheta);
  endTheta *= M_PI / 180.0;
  startPhi = (StartPhi < EndPhi ? StartPhi : EndPhi);
  startPhi *= M_PI / 180.0;
  endPhi = (EndPhi > StartPhi ? EndPhi : StartPhi);
  endPhi *= M_PI / 180.0;
  phiResolution = PhiResolution - numPoles;
  deltaPhi = (endPhi - startPhi) / (PhiResolution - 1);
  thetaResolution = localThetaResolution;
  if (fabs(localStartTheta - localEndTheta) < 360.0) {
    ++localThetaResolution;
  }
  deltaTheta = (endTheta - startTheta) / thetaResolution;
  jStart = (StartPhi <= 0.0 ? 1 : 0);
  jEnd = (EndPhi >= 180.0 ? PhiResolution - 1 : PhiResolution);
  // Create intermediate points
  for (i = 0; i < localThetaResolution; i++) {
    theta = localStartTheta * M_PI / 180.0 + i * deltaTheta;
    for (j = jStart; j < jEnd; j++) {
      phi = startPhi + j * deltaPhi;
      radius = Radius * sin((double)phi);
      n[0] = radius * cos((double)theta);
      n[1] = radius * sin((double)theta);
      n[2] = Radius * cos((double)phi);
      x[0] = n[0] + Center[0];
      x[1] = n[1] + Center[1];
      x[2] = n[2] + Center[2];
      if ((norm = NORM3(n)) == 0.0) {
        norm = 1.0;
      }
      n[0] /= norm;
      n[1] /= norm;
      n[2] /= norm;
      gcobj_add_vertex_dv(o, x, n, NULL);
    }
    UPDATE_PROGRESS(0.10 + 0.50 * i / static_cast<float>(localThetaResolution));
  }
  // Generate mesh connectivity
  base = phiResolution * localThetaResolution;
  if (fabs(localStartTheta - localEndTheta) < 360.0) {
    --localThetaResolution;
  }
  if (StartPhi <= 0.0) { // around north pole
    for (i = 0; i < localThetaResolution; i++) {
      pts[0] = phiResolution * i + numPoles;
      pts[1] = (phiResolution * (i + 1) % base) + numPoles;
      pts[2] = 0;
      gcobj_add_index_iv(o, 3, pts);
    }
  }
  if (EndPhi >= 180.0) { // around south pole
    numOffset = phiResolution - 1 + numPoles;
    for (i = 0; i < localThetaResolution; i++) {
      pts[0] = phiResolution * i + numOffset;
      pts[2] = ((phiResolution * (i + 1)) % base) + numOffset;
      pts[1] = numPoles - 1;
      gcobj_add_index_iv(o, 3, pts);
    }
  }
  UPDATE_PROGRESS(0.70);
  // bands in-between poles
  for (i = 0; i < localThetaResolution; i++) {
    for (j = 0; j < (phiResolution - 1); j++) {
      pts[0] = phiResolution * i + j + numPoles;
      pts[1] = pts[0] + 1;
      pts[2] = ((phiResolution * (i + 1) + j) % base) + numPoles + 1;
      gcobj_add_index_iv(o, 3, pts);
      pts[1] = pts[2];
      pts[2] = pts[1] - 1;
      gcobj_add_index_iv(o, 3, pts);
    }
    UPDATE_PROGRESS(0.70 + 0.30 * i / static_cast<double>(localThetaResolution));
  }
  return 0;
}
#define VTK_CELL_SIZE 512
// GC_TRIANGLE_STRIP;
int cone(gcobj_t* o, double Height, double Radius, int Resolution, int Capping)
{
  double angle;
  int numLines, numPolys, numPts;
  double x[3], xbot;
  int i;
  int pts[VTK_CELL_SIZE];
  // for streaming
  int piece;
  int numPieces;
  int maxPieces;
  int start, end;
  int createBottom;
  piece = 0;//outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER());
  if (piece >= Resolution && !(piece == 0 && Resolution == 0)) {
    return 0;
  }
  numPieces = 1;//outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES());
  maxPieces = Resolution != 0 ? Resolution : 1;
  if (numPieces > maxPieces) {
    numPieces = maxPieces;
  }
  if (piece >= maxPieces) {
    // Super class should do this for us,
    // but I put this condition in any way.
    return 1;
  }
  start = maxPieces * piece / numPieces;
  end = (maxPieces * (piece + 1) / numPieces) - 1;
  createBottom = (Capping && (start == 0));
  if (Resolution) {
    angle = 2.0 * M_PI / Resolution;
  }
  else {
    angle = 0.0;
  }
  // Set things up; allocate memory
  //
  switch (Resolution) {
  case 0:
    numPts = 2;
    numLines = 1;
    break;
  case 1:
  case 2:
    numPts = 2 * Resolution + 1;
    numPolys = Resolution;
    break;
  default:
    if (createBottom) {
      // piece 0 has cap.
      numPts = Resolution + 1;
      numPolys = end - start + 2;
    }
    else {
      numPts = end - start + 3;
      numPolys = end - start + 2;
    }
    break;
  }
  gcobj_set_size(o, GC_TRIANGLES, numPts, numPolys * 4);
  // Create cone
  x[0] = Height / 2.0; // zero-centered
  x[1] = 0.0;
  x[2] = 0.0;
  pts[0] = o->vertex_num;
  gcobj_add_vertex_dv(o, x, NULL, NULL);
  xbot = -Height / 2.0;
  switch (Resolution) {
  case 0:
    x[0] = xbot;
    x[1] = 0.0;
    x[2] = 0.0;
    pts[1] = o->vertex_num;
    gcobj_add_vertex_dv(o, x, NULL, NULL);
    gcobj_add_index_iv(o, 2, pts);
    break;
  case 2: // fall through this case to use the code in case 1
    x[0] = xbot;
    x[1] = 0.0;
    x[2] = -Radius;
    pts[1] = o->vertex_num;
    gcobj_add_vertex_dv(o, x, NULL, NULL);
    x[0] = xbot;
    x[1] = 0.0;
    x[2] = Radius;
    pts[2] = o->vertex_num;
    gcobj_add_vertex_dv(o, x, NULL, NULL);
    gcobj_add_index_iv(o, 3, pts);
    //VTK_FALLTHROUGH;
  case 1:
    x[0] = xbot;
    x[1] = -Radius;
    x[2] = 0.0;
    pts[1] = o->vertex_num;
    gcobj_add_vertex_dv(o, x, NULL, NULL);
    x[0] = xbot;
    x[1] = Radius;
    x[2] = 0.0;
    pts[2] = o->vertex_num;
    gcobj_add_vertex_dv(o, x, NULL, NULL);
    gcobj_add_index_iv(o, 3, pts);
    break;
  default: // General case: create Resolution triangles and single cap
    // create the bottom.
    if (createBottom) {
      for (i = 0; i < Resolution; i++) {
        x[0] = xbot;
        x[1] = Radius * cos(i * angle);
        x[2] = Radius * sin(i * angle);
        // Reverse the order
        pts[Resolution - i - 1] = o->vertex_num;
        gcobj_add_vertex_dv(o, x, NULL, NULL);
      }
      gcobj_add_index_iv(o, Resolution, pts);
    }
    pts[0] = 0;
    if (! createBottom) {
      // we need to create the points also
      x[0] = xbot;
      x[1] = Radius * cos(start * angle);
      x[2] = Radius * sin(start * angle);
      gcobj_add_vertex_dv(o, x, NULL, NULL);
      pts[1] = o->vertex_num;
      for (i = start; i <= end; ++i) {
        x[1] = Radius * cos((i + 1) * angle);
        x[2] = Radius * sin((i + 1) * angle);
        pts[2] = o->vertex_num;
        gcobj_add_vertex_dv(o, x, NULL, NULL);
        gcobj_add_index_iv(o, 3, pts);
        pts[1] = pts[2];
      }
    }
    else {
      // bottom and points have already been created.
      for (i = start; i <= end; i++) {
        pts[1] = i + 1;
        pts[2] = i + 2;
        if (pts[2] > Resolution) {
          pts[2] = 1;
        }
        gcobj_add_index_iv(o, 3, pts);
      }
    } // createBottom
  } //switch
  return 0;
}

