
//===========================================================================
// Clamping the parameter values to be within 3 standard deviations
void PDM::Clamp(Mat& local_params, Vec6d& params_global, const CLMParameters& parameters)
{
  double n_sigmas = 3;
  const double* e_it  = img_ptr(double, &this->eigen_values);
  float* p_it =  img_ptr(float, &local_params);
  float* p_it_end =  p_it + local_params.rows * local_params.cols;

  double v;

  // go over all parameters
  for (; p_it != p_it_end; ++p_it, ++e_it) {
    // Work out the maximum value
    v = n_sigmas * sqrt(*e_it);

    // if the values is too extreme clamp it
    if (fabs(*p_it) > v) {
      // Dealing with positive and negative cases
      if (*p_it > 0.0) {
        *p_it = v;
      }
      else {
        *p_it = -v;
      }
    }
  }

  // do not let the pose get out of hand
  if (parameters.limit_pose) {
    if (params_global[1] > M_PI / 2) {
      params_global[1] = M_PI / 2;
    }

    if (params_global[1] < -M_PI / 2) {
      params_global[1] = -M_PI / 2;
    }

    if (params_global[2] > M_PI / 2) {
      params_global[2] = M_PI / 2;
    }

    if (params_global[2] < -M_PI / 2) {
      params_global[2] = -M_PI / 2;
    }

    if (params_global[3] > M_PI / 2) {
      params_global[3] = M_PI / 2;
    }

    if (params_global[3] < -M_PI / 2) {
      params_global[3] = -M_PI / 2;
    }
  }
}
//===========================================================================
// Compute the 3D representation of shape (in object space) using the local parameters
void PDM::CalcShape3D(Mat& out_shape, const Mat& p_local) const
{
  out_shape.create(mean_shape.rows, mean_shape.cols, CV_64F);
  out_shape = mean_shape + princ_comp * p_local;
}

//===========================================================================
// Get the 2D shape (in image space) from global and local parameters
void PDM::CalcShape2D(Mat& out_shape, const Mat& params_local, const Vec6d& params_global) const
{
  int n = this->NumberOfPoints();

  double s = params_global[0]; // scaling factor
  double tx = params_global[4]; // x offset
  double ty = params_global[5]; // y offset

  // get the rotation matrix from the euler angles
  Vec3d euler(params_global[1], params_global[2], params_global[3]);
  Matx33d currRot = Euler2RotationMatrix(euler);

  // get the 3D shape of the object
  Mat Shape_3D = mean_shape + princ_comp * params_local;

  // create the 2D shape matrix (if it has not been defined yet)
  if ((out_shape.rows != mean_shape.rows) || (out_shape.cols = 1)) {
    out_shape.create(2 * n, 1, CV_64F);
  }

  double* Shape_3D_ptr = img_ptr(double, &Shape_3D);
  double* out_shape_ptr = img_ptr(double, &out_shape);
  // for every vertex
  for (int i = 0; i < n; i++) {
    // Transform this using the weak-perspective mapping to 2D from 3D
    out_shape_ptr[i + 0] = s * (currRot(0, 0) * Shape_3D_ptr[i] + currRot(0, 1) * Shape_3D_ptr[i + n] + currRot(0, 2) * Shape_3D_ptr[i + n * 2]) + tx;
    out_shape_ptr[i + n] = s * (currRot(1, 0) * Shape_3D_ptr[i] + currRot(1, 1) * Shape_3D_ptr[i + n] + currRot(1, 2) * Shape_3D_ptr[i + n * 2]) + ty;
  }
}

//===========================================================================
// provided the bounding box of a face and the local parameters (with optional rotation), generates the global parameters that can generate the face with the provided bounding box
// This all assumes that the bounding box describes face from left outline to right outline of the face and chin to eyebrows
void PDM::CalcParams(Vec6d& out_params_global, const CRectd& bounding_box, const Mat& params_local, const Vec3d rotation)
{
  // get the shape instance based on local params
  Mat current_shape(mean_shape.rows, mean_shape.cols, CV_64F);

  CalcShape3D(current_shape, params_local);

  // rotate the shape
  Matx33d rotation_matrix = Euler2RotationMatrix(rotation);

  Mat reshaped = current_shape.reshape(1, 3);

  Mat rotated_shape = (Mat(rotation_matrix) * reshaped);

  // Get the width of expected shape
  double min_x;
  double max_x;
  minMaxLoc(rotated_shape.row(0), &min_x, &max_x);

  double min_y;
  double max_y;
  minMaxLoc(rotated_shape.row(1), &min_y, &max_y);

  double width = abs(min_x - max_x);
  double height = abs(min_y - max_y);

  double scaling = ((bounding_box.width / width) + (bounding_box.height / height)) / 2;

  // The estimate of face center also needs some correction
  double tx = bounding_box.x + bounding_box.width / 2;
  double ty = bounding_box.y + bounding_box.height / 2;

  // Correct it so that the bounding box is just around the minimum and maximum point in the initialised face
  tx = tx - scaling * (min_x + max_x) / 2;
  ty = ty - scaling * (min_y + max_y) / 2;

  out_params_global = cVec6d(scaling, rotation[0], rotation[1], rotation[2], tx, ty);
}

//===========================================================================
// provided the model parameters, compute the bounding box of a face
// The bounding box describes face from left outline to right outline of the face and chin to eyebrows
void PDM::CalcBoundingBox(CRect& out_bounding_box, const Vec6d& params_global, const Mat& params_local)
{
  // get the shape instance based on local params
  Mat current_shape;
  CalcShape2D(current_shape, params_local, params_global);

  // Get the width of expected shape
  double min_x;
  double max_x;
  minMaxLoc(mi(current_shape, cRect(0, 0, 1, this->NumberOfPoints())), &min_x, &max_x);

  double min_y;
  double max_y;
  minMaxLoc(mi(current_shape, cRect(0, this->NumberOfPoints(), 1, this->NumberOfPoints())), &min_y, &max_y);

  double width = abs(min_x - max_x);
  double height = abs(min_y - max_y);

  out_bounding_box = cRect((int)min_x, (int)min_y, (int)width, (int)height);
}

//===========================================================================
// Calculate the PDM's Jacobian over rigid parameters (rotation, translation and scaling), the additional input W represents trust for each of the landmarks and is part of Non-Uniform RLMS
void PDM::ComputeRigidJacobian(const Mat& p_local, const Vec6d& params_global, Mat& Jacob, const Mat W, Mat& Jacob_t_w)
{

  // number of verts
  int n = this->NumberOfPoints();

  Jacob.create(n * 2, 6, CV_32F);

  float X, Y, Z;

  float s = (float)params_global[0];

  Mat shape_3D_d;
  Mat p_local_d;
  p_local.convertTo(p_local_d, CV_64F);
  this->CalcShape3D(shape_3D_d, p_local_d);

  Mat shape_3D;
  shape_3D_d.convertTo(shape_3D, CV_32F);

  // Get the rotation matrix
  Vec3d euler(params_global[1], params_global[2], params_global[3]);
  Matx33d currRot = Euler2RotationMatrix(euler);

  float r11 = (float) currRot(0, 0);
  float r12 = (float) currRot(0, 1);
  float r13 = (float) currRot(0, 2);
  float r21 = (float) currRot(1, 0);
  float r22 = (float) currRot(1, 1);
  float r23 = (float) currRot(1, 2);
  float r31 = (float) currRot(2, 0);
  float r32 = (float) currRot(2, 1);
  float r33 = (float) currRot(2, 2);

  float* Jx = img_ptr(float, &Jacob);
  float* Jy = Jx + n * 6;
  float* shape_3D_ptr = img_ptr(float, &shape_3D);
  int i;
  for (i = 0; i < n; i++) {
    X = shape_3D_ptr[i];
    Y = shape_3D_ptr[i + n];
    Z = shape_3D_ptr[i + n * 2];

    // The rigid jacobian from the axis angle rotation matrix approximation using small angle assumption (R * R')
    // where R' = [1, -wz, wy
    //             wz, 1, -wx
    //             -wy, wx, 1]
    // And this is derived using the small angle assumption on the axis angle rotation matrix parametrisation

    // scaling term
    *Jx++ = (X  * r11 + Y * r12 + Z * r13);
    *Jy++ = (X  * r21 + Y * r22 + Z * r23);

    // rotation terms
    *Jx++ = (s * (Y * r13 - Z * r12));
    *Jy++ = (s * (Y * r23 - Z * r22));
    *Jx++ = (-s * (X * r13 - Z * r11));
    *Jy++ = (-s * (X * r23 - Z * r21));
    *Jx++ = (s * (X * r12 - Y * r11));
    *Jy++ = (s * (X * r22 - Y * r21));

    // translation terms
    *Jx++ = 1.0f;
    *Jy++ = 0.0f;
    *Jx++ = 0.0f;
    *Jy++ = 1.0f;

  }

  Mat Jacob_w = Mat::zeros(Jacob.rows, Jacob.cols, CV_32F);

  Jx =  img_ptr(float, &Jacob);
  Jy =  Jx + n * 6;

  float* Jx_w =  img_ptr(float, &Jacob_w);
  float* Jy_w =  Jx_w + n * 6;
  float* W_ptr = img_ptr(float, &W);
  // Iterate over all Jacobian values and multiply them by the weight in diagonal of W
  for (i = 0; i < n; i++) {
    float w_x = W_ptr[i*W.cols+i];
    float w_y = W_ptr[(i + n)*W.cols+ i + n];

    for (int j = 0; j < Jacob.cols; ++j) {
      *Jx_w++ = *Jx++ * w_x;
      *Jy_w++ = *Jy++ * w_y;
    }
  }

  Jacob_t_w = Jacob_w.t();
}

//===========================================================================
// Calculate the PDM's Jacobian over all parameters (rigid and non-rigid), the additional input W represents trust for each of the landmarks and is part of Non-Uniform RLMS
void PDM::ComputeJacobian(const Mat& params_local, const Vec6d& params_global, Mat& Jacobian, const Mat& W, Mat& Jacob_t_w)
{

  // number of vertices
  int n = this->NumberOfPoints();

  // number of non-rigid parameters
  int m = this->NumberOfModes();

  Jacobian.create(n * 2, 6 + m, CV_32F);

  float X, Y, Z;

  float s = (float) params_global[0];

  Mat shape_3D_d;
  Mat p_local_d;
  params_local.convertTo(p_local_d, CV_64F);
  this->CalcShape3D(shape_3D_d, p_local_d);

  Mat shape_3D;
  shape_3D_d.convertTo(shape_3D, CV_32F);

  Vec3d euler(params_global[1], params_global[2], params_global[3]);
  Matx33d currRot = Euler2RotationMatrix(euler);

  float r11 = (float) currRot(0, 0);
  float r12 = (float) currRot(0, 1);
  float r13 = (float) currRot(0, 2);
  float r21 = (float) currRot(1, 0);
  float r22 = (float) currRot(1, 1);
  float r23 = (float) currRot(1, 2);
  float r31 = (float) currRot(2, 0);
  float r32 = (float) currRot(2, 1);
  float r33 = (float) currRot(2, 2);

  float* Jx =  img_ptr(float, &Jacobian);
  float* Jy =  Jx + n * (6 + m);
  const double* Vx =  img_ptr(double, &this->princ_comp);
  const double* Vy =  Vx + n * m;
  const double* Vz =  Vy + n * m;
  float* shape_3D_ptr = img_ptr(float, &shape_3D);
  for (int i = 0; i < n; i++) {

    X = shape_3D_ptr[i];
    Y = shape_3D_ptr[i + n];
    Z = shape_3D_ptr[i + n * 2];

    // The rigid jacobian from the axis angle rotation matrix approximation using small angle assumption (R * R')
    // where R' = [1, -wz, wy
    //             wz, 1, -wx
    //             -wy, wx, 1]
    // And this is derived using the small angle assumption on the axis angle rotation matrix parametrisation

    // scaling term
    *Jx++ = (X  * r11 + Y * r12 + Z * r13);
    *Jy++ = (X  * r21 + Y * r22 + Z * r23);

    // rotation terms
    *Jx++ = (s * (Y * r13 - Z * r12));
    *Jy++ = (s * (Y * r23 - Z * r22));
    *Jx++ = (-s * (X * r13 - Z * r11));
    *Jy++ = (-s * (X * r23 - Z * r21));
    *Jx++ = (s * (X * r12 - Y * r11));
    *Jy++ = (s * (X * r22 - Y * r21));

    // translation terms
    *Jx++ = 1.0f;
    *Jy++ = 0.0f;
    *Jx++ = 0.0f;
    *Jy++ = 1.0f;

    for (int j = 0; j < m; j++, ++Vx, ++Vy, ++Vz) {
      // How much the change of the non-rigid parameters (when object is rotated) affect 2D motion
      *Jx++ = (float)(s * (r11 * (*Vx) + r12 * (*Vy) + r13 * (*Vz)));
      *Jy++ = (float)(s * (r21 * (*Vx) + r22 * (*Vy) + r23 * (*Vz)));
    }
  }

  // Adding the weights here
  Mat Jacob_w = Jacobian.clone();

  if (trace(W)[0] != W.rows) {
    Jx =  img_ptr(float, &Jacobian);
    Jy =  Jx + n * (6 + m);

    float* Jx_w = img_ptr(float, &Jacob_w);
    float* Jy_w = Jx_w + n * (6 + m);
    float* W_ptr = img_ptr(float, &W);
    // Iterate over all Jacobian values and multiply them by the weight in diagonal of W
    for (int i = 0; i < n; i++) {
      float w_x = W_ptr[i*W.cols+i];
      float w_y = W_ptr[(i + n)*W.cols+ i + n];

      for (int j = 0; j < Jacobian.cols; ++j) {
        *Jx_w++ = *Jx++ * w_x;
        *Jy_w++ = *Jy++ * w_y;
      }
    }
  }

  Jacob_t_w = Jacob_w.t();

}

//===========================================================================
// Updating the parameters (more details in my thesis)
void PDM::UpdateModelParameters(const Mat& delta_p, Mat& params_local, Vec6d& params_global)
{
  // The scaling and translation parameters can be just added
  float* delta_p_ptr = img_ptr(float, &delta_p);
  params_global[0] += (double)delta_p_ptr[0];
  params_global[4] += (double)delta_p_ptr[4];
  params_global[5] += (double)delta_p_ptr[5];

  // get the original rotation matrix
  Vec3d eulerGlobal(params_global[1], params_global[2], params_global[3]);
  Matx33d R1 = Euler2RotationMatrix(eulerGlobal);

  // construct R' = [1, -wz, wy
  //               wz, 1, -wx
  //               -wy, wx, 1]
  Matx33d R2 = Matx33d::eye();

  R2(1, 2) = -1.0 * (R2(2, 1) = (double)delta_p_ptr[1]);
  R2(2, 0) = -1.0 * (R2(0, 2) = (double)delta_p_ptr[2]);
  R2(0, 1) = -1.0 * (R2(1, 0) = (double)delta_p_ptr[3]);

  // Make sure it's orthonormal
  Orthonormalise(R2);

  // Combine rotations
  Matx33d R3 = R1 * R2;

  // Extract euler angle (through axis angle first to make sure it's legal)
  Vec3d axis_angle = RotationMatrix2AxisAngle(R3);
  Vec3d euler = AxisAngle2Euler(axis_angle);

  params_global[1] = euler[0];
  params_global[2] = euler[1];
  params_global[3] = euler[2];

  // Local parameter update, just simple addition
  if (delta_p.rows > 6) {
    params_local = params_local + mi(delta_p, cRect(0, 6, 1, this->NumberOfModes()));
  }

}

void PDM::CalcParams(Vec6d& out_params_global, Mat& out_params_local, const Mat& landmark_locations, const Vec3d rotation)
{

  int m = this->NumberOfModes();
  int n = this->NumberOfPoints();
  int i;
  Mat visi_ind_2D(n * 2, 1, CV_32S, 1);
  Mat visi_ind_3D(3 * n, 1, CV_32S, 1);
  double* landmark_locations_ptr = img_ptr(double, &landmark_locations);
  int* visi_ind_2D_ptr = img_ptr(int, &visi_ind_2D);
  int* visi_ind_3D_ptr = img_ptr(int, &visi_ind_3D);
  for (i = 0; i < n; ++i) {
    // If the landmark is invisible indicate this
    if (landmark_locations_ptr[i] == 0) {
      visi_ind_2D_ptr[i] = 0;
      visi_ind_2D_ptr[i + n] = 0;
      visi_ind_3D_ptr[i] = 0;
      visi_ind_3D_ptr[i + n] = 0;
      visi_ind_3D_ptr[i + 2 * n] = 0;
    }
  }

  // As this might be subsampled have special versions
  Mat M(0, mean_shape.cols, CV_64F, 0.0);
  Mat V(0, princ_comp.cols, CV_64F, 0.0);

  for (i = 0; i < n * 3; ++i) {
    if (visi_ind_3D_ptr[i] == 1) {
      vconcat(M, this->mean_shape.row(i), M);
      vconcat(V, this->princ_comp.row(i), V);
    }
  }

  Mat m_old = this->mean_shape.clone();
  Mat v_old = this->princ_comp.clone();

  this->mean_shape = M;
  this->princ_comp = V;

  // The new number of points
  n  = M.rows / 3;

  // Extract the relevant landmark locations
  Mat landmark_locs_vis(n * 2, 1, CV_64F, 0.0);
  int k = 0;
  double* landmark_locs_vis_ptr = img_ptr(double, &landmark_locs_vis);
  for (i = 0; i < visi_ind_2D.rows; ++i) {
    if (visi_ind_2D_ptr[i] == 1) {
      landmark_locs_vis_ptr[k] = landmark_locations_ptr[i];
      k++;
    }
  }

  // Compute the initial global parameters
  double min_x;
  double max_x;
  minMaxLoc(mi(landmark_locations, cRect(0, 0, 1, this->NumberOfPoints())), &min_x, &max_x);

  double min_y;
  double max_y;
  minMaxLoc(mi(landmark_locations, cRect(0, this->NumberOfPoints(), 1, this->NumberOfPoints())), &min_y, &max_y);

  double width = abs(min_x - max_x);
  double height = abs(min_y - max_y);

  CRect model_bbox;
  CalcBoundingBox(model_bbox, cVec6d(1.0, 0.0, 0.0, 0.0, 0.0, 0.0), Mat::zeros(this->NumberOfModes(), 1, CV_64F));

  CRect bbox = cRect((int)min_x, (int)min_y, (int)width, (int)height);

  double scaling = ((width / model_bbox.width) + (height / model_bbox.height)) / 2;

  Vec3d rotation_init = rotation;
  Matx33d R = Euler2RotationMatrix(rotation_init);
  Vec2d translation((min_x + max_x) / 2.0, (min_y + max_y) / 2.0);

  Mat loc_params(this->NumberOfModes(), 1, CV_32F, 0.0);
  Vec6d glob_params(scaling, rotation_init[0], rotation_init[1], rotation_init[2], translation[0], translation[1]);

  // get the 3D shape of the object
  Mat loc_params_d;
  loc_params.convertTo(loc_params_d, CV_64F);
  Mat shape_3D = M + V * loc_params_d;

  Mat curr_shape(2 * n, 1, CV_64F);
  double* curr_shape_ptr = img_ptr(double, &curr_shape);
  double* shape_3D_ptr = img_ptr(double, &shape_3D);
  // for every vertex
  for (i = 0; i < n; i++) {
    // Transform this using the weak-perspective mapping to 2D from 3D
    curr_shape_ptr[i + 0] = scaling * (R(0, 0) * shape_3D_ptr[i] + R(0, 1) * shape_3D_ptr[i + n] + R(0, 2) * shape_3D_ptr[i + n * 2]) + translation[0];
    curr_shape_ptr[i + n] = scaling * (R(1, 0) * shape_3D_ptr[i] + R(1, 1) * shape_3D_ptr[i + n] + R(1, 2) * shape_3D_ptr[i + n * 2]) + translation[1];
  }

  double currError = norm(curr_shape - landmark_locs_vis);

  Mat regularisations = Mat::zeros(1, 6 + m, CV_32F);

  double reg_factor = 1;

  CHECK_OR_PUT(eigen_values);
  // Setting the regularisation to the inverse of eigenvalues
  //Mat(reg_factor / this->eigen_values).copyTo(regularisations(Rect(6, 0, m, 1)));
  copyTo((reg_factor / this->eigen_values), regularisations, 6, 0, m, 1);
  Mat regTerm_d = Mat::diag(regularisations.t());
  regTerm_d.convertTo(regularisations, CV_32F);

  Mat WeightMatrix = Mat::eye(n * 2, n * 2, CV_32F);

  int not_improved_in = 0;

  if (V.rows*V.cols>0) {
  for (size_t i = 0; i < 1000; ++i) {
    // get the 3D shape of the object
    Mat loc_params_d;
    loc_params.convertTo(loc_params_d, CV_64F);
    shape_3D = M + V * loc_params_d;

    shape_3D = shape_3D.reshape(1, 3);

    Matx23d R_2D(R(0, 0), R(0, 1), R(0, 2), R(1, 0), R(1, 1), R(1, 2));

    Mat curr_shape_2D = scaling * shape_3D.t() * Mat(R_2D).t();
    curr_shape_2D.col(0) = curr_shape_2D.col(0) + translation(0);
    curr_shape_2D.col(1) = curr_shape_2D.col(1) + translation(1);

    curr_shape_2D = Mat(curr_shape_2D.t()).reshape(1, n * 2);

    Mat error_resid;
    Mat(landmark_locs_vis - curr_shape_2D).convertTo(error_resid, CV_32F);

    Mat J, J_w_t;
    this->ComputeJacobian(loc_params, glob_params, J, WeightMatrix, J_w_t);

    // projection of the meanshifts onto the jacobians (using the weighted Jacobian, see Baltrusaitis 2013)
    Mat J_w_t_m = J_w_t * error_resid;

    // Add the regularisation term
    //J_w_t_m(Rect(0, 6, 1, m)) = J_w_t_m(Rect(0, 6, 1, m)) - regularisations(Rect(6, 6, m, m)) * loc_params;
	copyTo(mi(J_w_t_m, cRect(0, 6, 1, m)) - mi(regularisations, cRect(6, 6, m, m)) * loc_params, J_w_t_m, 0, 6, 1, m);

    Mat Hessian = J_w_t * J;

    // Add the Tikhonov regularisation
    Hessian = Hessian + regularisations;

    // Solve for the parameter update (from Baltrusaitis 2013 based on eq (36) Saragih 2011)
    Mat param_update(J_w_t_m.rows, J_w_t_m.cols, CV_32F);
#ifdef _USE_OPENCV
    solve(Hessian, J_w_t_m, param_update, CC_CHOLESKY);
#else
    icvSolve(Hessian.data, Hessian.step, J_w_t_m.data, J_w_t_m.step, param_update.data, param_update.step, Hessian.rows, Hessian.cols, CC_32F, CC_CHOLESKY);
#endif

    // To not overshoot, have the gradient decent rate a bit smaller
    param_update = 0.5 * param_update;

    UpdateModelParameters(param_update, loc_params, glob_params);

    scaling = glob_params[0];
    rotation_init[0] = glob_params[1];
    rotation_init[1] = glob_params[2];
    rotation_init[2] = glob_params[3];

    translation[0] = glob_params[4];
    translation[1] = glob_params[5];

    R = Euler2RotationMatrix(rotation_init);

    R_2D(0, 0) = R(0, 0);
    R_2D(0, 1) = R(0, 1);
    R_2D(0, 2) = R(0, 2);
    R_2D(1, 0) = R(1, 0);
    R_2D(1, 1) = R(1, 1);
    R_2D(1, 2) = R(1, 2);

    curr_shape_2D = scaling * shape_3D.t() * Mat(R_2D).t();
    curr_shape_2D.col(0) = curr_shape_2D.col(0) + translation(0);
    curr_shape_2D.col(1) = curr_shape_2D.col(1) + translation(1);

    curr_shape_2D = Mat(curr_shape_2D.t()).reshape(1, n * 2);

    double error = norm(curr_shape_2D - landmark_locs_vis);

    if (0.999 * currError < error) {
      not_improved_in++;

      if (not_improved_in == 5) {
        break;
      }
    }

    currError = error;

  }
  }

  out_params_global = glob_params;
  loc_params.convertTo(out_params_local, CC_64F);

  this->mean_shape = m_old;
  this->princ_comp = v_old;


}

void PDM::Read(const char* location)
{

  FILE* pdmLoc = fopen(location, "rb");


  SkipComments(pdmLoc);

  // Reading mean values
  ReadMat(pdmLoc, mean_shape);

  SkipComments(pdmLoc);

  // Reading principal components
  ReadMat(pdmLoc, princ_comp);

  SkipComments(pdmLoc);

  // Reading eigenvalues
  ReadMat(pdmLoc, eigen_values);
  fclose(pdmLoc);
}
