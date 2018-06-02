


// A constructor from destination shape and triangulation
PAW::PAW(const Mat& destination_shape, const Mat& triangulation)
{
  // Initialise some variables directly
  this->destination_landmarks = destination_shape;
  this->triangulation = triangulation;

  int num_points = destination_shape.rows / 2;

  int num_tris = triangulation.rows;

  // Pre-compute the rest
  alpha = Mat(num_tris, 3, CV_64F);
  beta = Mat(num_tris, 3, CV_64F);

  Mat xs = mi(destination_shape, cRect(0, 0, 1, num_points));
  Mat ys = mi(destination_shape, cRect(0, num_points, 1, num_points));
  double* xs_ptr = img_ptr(double, &xs);
  double* ys_ptr = img_ptr(double, &ys);
  double* alpha_ptr = img_ptr(double, &alpha);
  double* beta_ptr = img_ptr(double, &beta);
  int* triangulation_ptr = img_ptr(int, &triangulation);

  // Create a vector representation of the control points
  double *destination_points = NULL;
  MYREALLOC(destination_points, num_tris*10);

  for (int tri = 0; tri < num_tris; ++tri) {
    int j = triangulation_ptr[3*tri+0];
    int k = triangulation_ptr[3*tri+1];
    int l = triangulation_ptr[3*tri+2];

    double c1 = ys_ptr[l] - ys_ptr[j];
    double c2 = xs_ptr[l] - xs_ptr[j];
    double c4 = ys_ptr[k] - ys_ptr[j];
    double c3 = xs_ptr[k] - xs_ptr[j];

    double c5 = c3 * c1 - c2 * c4;

    alpha_ptr[3*tri+0] = (ys_ptr[j] * c2 - xs_ptr[j] * c1) / c5;
    alpha_ptr[3*tri+1] = c1 / c5;
    alpha_ptr[3*tri+2] = -c2 / c5;

    beta_ptr[3*tri+0] = (xs_ptr[j] * c4 - ys_ptr[j] * c3) / c5;
    beta_ptr[3*tri+1] = -c4 / c5;
    beta_ptr[3*tri+2] = c3 / c5;

    // Add points corresponding to triangles as optimisation
    double* triangle_points = destination_points + tri*(10);

    triangle_points[0] = xs_ptr[j];
    triangle_points[1] = ys_ptr[j];
    triangle_points[2] = xs_ptr[k];
    triangle_points[3] = ys_ptr[k];
    triangle_points[4] = xs_ptr[l];
    triangle_points[5] = ys_ptr[l];

    Vec3d xs_three(triangle_points[0], triangle_points[2], triangle_points[4]);
    Vec3d ys_three(triangle_points[1], triangle_points[3], triangle_points[5]);

    double min_x, max_x, min_y, max_y;
    minMaxIdx(xs_three, &min_x, &max_x);
    minMaxIdx(ys_three, &min_y, &max_y);

    triangle_points[6] = max_x;
    triangle_points[7] = max_y;

    triangle_points[8] = min_x;
    triangle_points[9] = min_y;
  }

  double max_x;
  double max_y;

  minMaxLoc(xs, &min_x, &max_x);
  minMaxLoc(ys, &min_y, &max_y);

  int w = (int)(max_x - min_x + 1.5);
  int h = (int)(max_y - min_y + 1.5);

  // Round the min_x and min_y for simplicity?

  pixel_mask = Mat::zeros(h, w, CV_8U);
  triangle_id = Mat(h, w, CV_32S, -1);

  int curr_tri = -1;
  int* triangle_id_ptr = img_ptr(int, &triangle_id);
  uchar* pixel_mask_ptr = img_ptr(uchar, &pixel_mask);
  for (int y = 0; y < pixel_mask.rows; y++) {
    for (int x = 0; x < pixel_mask.cols; x++) {
      curr_tri = findTriangle(cPoint2d(x + min_x, y + min_y), destination_points, num_tris, curr_tri);

      // If there is a triangle at this location
      if (curr_tri != -1) {
        triangle_id_ptr[y*triangle_id.cols+x] = curr_tri;
        pixel_mask_ptr[y*w+x] = 1;
      }
    }
  }

  // Preallocate maps and coefficients
  coefficients.create(num_tris, 6, CV_64F);
  map_x.create(pixel_mask.rows, pixel_mask.cols, CV_32F);
  map_y.create(pixel_mask.rows, pixel_mask.cols, CV_32F);
  FREE(destination_points);
}

// Manually define min and max values
PAW::PAW(const Mat& destination_shape, const Mat& triangulation, double in_min_x, double in_min_y, double in_max_x, double in_max_y)
{
  // Initialise some variables directly
  this->destination_landmarks = destination_shape;
  this->triangulation = triangulation;

  int num_points = destination_shape.rows / 2;

  int num_tris = triangulation.rows;

  // Pre-compute the rest
  alpha = Mat(num_tris, 3, CV_64F);
  beta = Mat(num_tris, 3, CV_64F);

  Mat xs = mi(destination_shape, cRect(0, 0, 1, num_points));
  Mat ys = mi(destination_shape, cRect(0, num_points, 1, num_points));
  double* xs_ptr = img_ptr(double, &xs);
  double* ys_ptr = img_ptr(double, &ys);
  double* alpha_ptr = img_ptr(double, &alpha);
  double* beta_ptr = img_ptr(double, &beta);
  int* triangulation_ptr = img_ptr(int, &triangulation);

  // Create a vector representation of the control points
  double* destination_points = NULL;
  MYREALLOC(destination_points, num_tris*10);

  for (int tri = 0; tri < num_tris; ++tri) {
    int j = triangulation_ptr[3*tri+0];
    int k = triangulation_ptr[3*tri+1];
    int l = triangulation_ptr[3*tri+2];

    double c1 = ys_ptr[l] - ys_ptr[j];
    double c2 = xs_ptr[l] - xs_ptr[j];
    double c4 = ys_ptr[k] - ys_ptr[j];
    double c3 = xs_ptr[k] - xs_ptr[j];

    double c5 = c3 * c1 - c2 * c4;

    alpha_ptr[tri*3+0] = (ys_ptr[j] * c2 - xs_ptr[j] * c1) / c5;
    alpha_ptr[tri*3+1] = c1 / c5;
    alpha_ptr[tri*3+2] = -c2 / c5;

    beta_ptr[tri*3+0] = (xs_ptr[j] * c4 - ys_ptr[j] * c3) / c5;
    beta_ptr[tri*3+1] = -c4 / c5;
    beta_ptr[tri*3+2] = c3 / c5;

    // Add points corresponding to triangles as optimisation
    double* triangle_points = destination_points + tri*(10);

    triangle_points[0] = xs_ptr[j];
    triangle_points[1] = ys_ptr[j];
    triangle_points[2] = xs_ptr[k];
    triangle_points[3] = ys_ptr[k];
    triangle_points[4] = xs_ptr[l];
    triangle_points[5] = ys_ptr[l];

    Vec3d xs_three(triangle_points[0], triangle_points[2], triangle_points[4]);
    Vec3d ys_three(triangle_points[1], triangle_points[3], triangle_points[5]);

    double min_x, max_x, min_y, max_y;
    minMaxIdx(xs_three, &min_x, &max_x);
    minMaxIdx(ys_three, &min_y, &max_y);

    triangle_points[6] = max_x;
    triangle_points[7] = max_y;

    triangle_points[8] = min_x;
    triangle_points[9] = min_y;
  }

  double max_x;
  double max_y;

  min_x = in_min_x;
  min_y = in_min_y;

  max_x = in_max_x;
  max_y = in_max_y;

  int w = (int)(max_x - min_x + 1.5);
  int h = (int)(max_y - min_y + 1.5);

  // Round the min_x and min_y for simplicity?

  pixel_mask = Mat::zeros(h, w, CV_8U);
  triangle_id = Mat(h, w, CV_32S, -1);

  int* triangle_id_ptr = img_ptr(int, &triangle_id);
  uchar* pixel_mask_ptr = img_ptr(uchar, &pixel_mask);

  int curr_tri = -1;

  for (int y = 0; y < pixel_mask.rows; y++) {
    for (int x = 0; x < pixel_mask.cols; x++) {
      curr_tri = findTriangle(cPoint2d(x + min_x, y + min_y), destination_points, num_tris, curr_tri);

      // If there is a triangle at this location
      if (curr_tri != -1) {
        triangle_id_ptr[y*triangle_id.cols+x] = curr_tri;
        pixel_mask_ptr[y*w+x] = 1;
      }
    }
  }

  // Preallocate maps and coefficients
  coefficients.create(num_tris, 6, CV_64F);
  map_x.create(pixel_mask.rows, pixel_mask.cols, CV_32F);
  map_y.create(pixel_mask.rows, pixel_mask.cols, CV_32F);
  FREE(destination_points);
}

//===========================================================================
void PAW::Read(FILE* stream)
{

  fread((char*)&number_of_pixels, 4, 1, stream);
  fread((char*)&min_x, 8, 1, stream);
  fread((char*)&min_y, 8, 1, stream);

  ReadMatBin(stream, destination_landmarks, CV_64F);

  ReadMatBin(stream, triangulation, CV_32S);

  ReadMatBin(stream, triangle_id, CV_32S);

  ReadMatBin(stream, pixel_mask, CV_8U);

  ReadMatBin(stream, alpha, CV_64F);

  ReadMatBin(stream, beta, CV_64F);

  map_x.create(pixel_mask.rows, pixel_mask.cols, CV_32F);
  map_y.create(pixel_mask.rows, pixel_mask.cols, CV_32F);

  coefficients.create(this->NumberOfTriangles(), 6, CV_64F);

  source_landmarks = destination_landmarks;
}

//=============================================================================
// cropping from the source image to the destination image using the shape in s, used to determine if shape fitting converged successfully
void PAW::Warp(const Mat& image_to_warp, Mat& destination_image, const Mat& landmarks_to_warp)
{

  // set the current shape
  source_landmarks = landmarks_to_warp.clone();

  // prepare the mapping coefficients using the current shape
  this->CalcCoeff();

  // Do the actual mapping computation (where to warp from)
  this->WarpRegion(map_x, map_y);

  // Do the actual warp (with bi-linear interpolation)
  remap(image_to_warp, destination_image, map_x, map_y, CC_INTER_LINEAR);

}


//=============================================================================
// Calculate the warping coefficients
void PAW::CalcCoeff()
{
  int p = this->NumberOfLandmarks();
  int* triangulation_ptr = img_ptr(int, &triangulation);
  double* source_landmarks_ptr = img_ptr(double, &source_landmarks);
  double* coefficients_ptr = img_ptr(double, &coefficients);
  double* alpha_ptr = img_ptr(double, &alpha);
  double* beta_ptr = img_ptr(double, &beta);
  for (int l = 0; l < this->NumberOfTriangles(); l++) {

    int i = triangulation_ptr[3*l+0];
    int j = triangulation_ptr[3*l+1];
    int k = triangulation_ptr[3*l+2];

    double c1 = source_landmarks_ptr[i    ];
    double c2 = source_landmarks_ptr[j    ] - c1;
    double c3 = source_landmarks_ptr[k    ] - c1;
    double c4 = source_landmarks_ptr[i + p];
    double c5 = source_landmarks_ptr[j + p] - c4;
    double c6 = source_landmarks_ptr[k + p] - c4;

    // Get a pointer to the coefficient we will be precomputing
    double* coeff = coefficients_ptr+l*coefficients.cols;

    // Extract the relevant alphas and betas
    double* c_alpha = alpha_ptr + l*alpha.cols;
    double* c_beta  = beta_ptr + l*alpha.cols;

    coeff[0] = c1 + c2 * c_alpha[0] + c3 * c_beta[0];
    coeff[1] =      c2 * c_alpha[1] + c3 * c_beta[1];
    coeff[2] =      c2 * c_alpha[2] + c3 * c_beta[2];
    coeff[3] = c4 + c5 * c_alpha[0] + c6 * c_beta[0];
    coeff[4] =      c5 * c_alpha[1] + c6 * c_beta[1];
    coeff[5] =      c5 * c_alpha[2] + c6 * c_beta[2];
  }
}

//======================================================================
// Compute the mapping coefficients
void PAW::WarpRegion(Mat& mapx, Mat& mapy)
{
  // The coefficients corresponding to the current triangle
  double* a = NULL;

  // Current triangle being processed
  int k = -1;
  float* mapx_ptr = img_ptr(float, &mapx);
  float* mapy_ptr = img_ptr(float, &mapy);
  uchar* pixel_mask_ptr = img_ptr(uchar, &pixel_mask);
  for (int y = 0; y < pixel_mask.rows; y++) {
    float* xp = mapx_ptr+y*pixel_mask.cols;
    float* yp = mapy_ptr+y*pixel_mask.cols;
    uchar* mp = pixel_mask_ptr + y*pixel_mask.cols;
    int*   tp = img_row(int, &triangle_id, y);
    double yi = double(y) + min_y;

    for (int x = 0; x < pixel_mask.cols; x++) {
      double xi = double(x) + min_x;

      if (*mp == 0) {
        *xp = -1;
        *yp = -1;
      }
      else {
        // triangle corresponding to the current pixel
        int j = *tp;

        // If it is different from the previous triangle point to new coefficients
        // This will always be the case in the first iteration, hence a will not point to nothing
        if (j != k) {
          // Update the coefficient pointer if a new triangle is being processed
          a = img_row(double, &coefficients, j);
          k = j;
        }

        //ap is now the pointer to the coefficients
        double* ap = a;

        //look at the first coefficient (and increment). first coefficient is an x offset
        double xo = *ap++;
        //second coefficient is an x scale as a function of x
        xo += *ap++ * xi;
        //third coefficient ap(2) is an x scale as a function of y
        *xp = float(xo + *ap++ * yi);

        //then fourth coefficient ap(3) is a y offset
        double yo = *ap++;
        //fifth coeff adds coeff[4]*x to y
        yo += *ap++ * xi;
        //final coeff adds coeff[5]*y to y
        *yp = float(yo + *ap++ * yi);

      }

      mp++;
      tp++;
      xp++;
      yp++;
    }
  }
}

// ============================================================
// Helper functions to determine which point a triangle lies in
// ============================================================

// Is the point (x0,y0) on same side as a half-plane defined by (x1,y1), (x2, y2), and (x3, y3)
bool sameSide(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3)
{

  double x = (x3 - x2) * (y0 - y2) - (x0 - x2) * (y3 - y2);
  double y = (x3 - x2) * (y1 - y2) - (x1 - x2) * (y3 - y2);

  return x * y >= 0;

}

// if point (x0, y0) is on same side for all three half-planes it is in a triangle
bool pointInTriangle(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3)
{
  bool same_1 = sameSide(x0, y0, x1, y1, x2, y2, x3, y3);
  bool same_2 = sameSide(x0, y0, x2, y2, x1, y1, x3, y3);
  bool same_3 = sameSide(x0, y0, x3, y3, x1, y1, x2, y2);

  return same_1 && same_2 && same_3;

}

// Find if a given point lies in the triangles
int PAW::findTriangle(const CPoint2d& point, const double* control_points, int num_tris, int guess) const
{

  int tri = -1;

  double x0 = point.x;
  double y0 = point.y;

  // Allow a guess for speed (so as not to go through all triangles)
  if (guess != -1) {
    const double* ptr = control_points+10*guess;
    bool in_triangle = pointInTriangle(x0, y0, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

    if (in_triangle) {
      return guess;
    }
  }


  for (int i = 0; i < num_tris; ++i) {
    const double* ptr = control_points+10*i;
    double max_x = ptr[6];
    double max_y = ptr[7];

    double min_x = ptr[8];
    double min_y = ptr[9];

    // Skip the check if the point is outside the bounding box of the triangle

    if (max_x < x0 || min_x > x0 || max_y < y0 || min_y > y0) {
      continue;
    }

    bool in_triangle = pointInTriangle(x0, y0, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

    if (in_triangle) {
      tri = i;
      break;
    }
  }

  return tri;
}