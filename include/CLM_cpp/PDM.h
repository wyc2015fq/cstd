
#ifndef __PDM_h_
#define __PDM_h_

//===========================================================================
// A linear 3D Point Distribution Model (constructed using Non-Rigid structure from motion or PCA)
// Only describes the model but does not contain an instance of it (no local or global parameters are stored here)
// Contains the utility functions to help manipulate the model

class PDM
{
public:

  // The 3D mean shape vector of the PDM [x1,..,xn,y1,...yn,z1,...,zn]
  Mat mean_shape; //double

  // Principal components or variation bases of the model,
  Mat princ_comp;//double

  // Eigenvalues (variances) corresponding to the bases
  Mat eigen_values;//double

  PDM() {
    ;
  }

  // A copy constructor
  PDM(const PDM& other) {

    // Make sure the matrices are allocated properly
    this->mean_shape = other.mean_shape.clone();
    this->princ_comp = other.princ_comp.clone();
    this->eigen_values = other.eigen_values.clone();
  }

  void Read(const char* location);

  // Number of vertices
  inline int NumberOfPoints() const {
    return mean_shape.rows / 3;
  }

  // Listing the number of modes of variation
  inline int NumberOfModes() const {
    return princ_comp.cols;
  }

  void Clamp(Mat& params_local, Vec6d& params_global, const CLMParameters& params);

  // Compute shape in object space (3D)
  void CalcShape3D(Mat& out_shape, const Mat& params_local) const;

  // Compute shape in image space (2D)
  void CalcShape2D(Mat& out_shape, const Mat& params_local, const Vec6d& params_global) const;

  // provided the bounding box of a face and the local parameters (with optional rotation), generates the global parameters that can generate the face with the provided bounding box
  void CalcParams(Vec6d& out_params_global, const CRectd& bounding_box, const Mat& params_local, const Vec3d rotation = cVec3d(0.0));

  // Provided the landmark location compute global and local parameters best fitting it (can provide optional rotation for potentially better results)
  void CalcParams(Vec6d& out_params_global, Mat& out_params_local, const Mat& landmark_locations, const Vec3d rotation = cVec3d(0.0));

  // provided the model parameters, compute the bounding box of a face
  void CalcBoundingBox(CRect& out_bounding_box, const Vec6d& params_global, const Mat& params_local);

  // Helpers for computing Jacobians, and Jacobians with the weight matrix
  void ComputeRigidJacobian(const Mat& params_local, const Vec6d& params_global, Mat& Jacob, const Mat W, Mat& Jacob_t_w);
  void ComputeJacobian(const Mat& params_local, const Vec6d& params_global, Mat& Jacobian, const Mat& W, Mat& Jacob_t_w);

  // Given the current parameters, and the computed delta_p compute the updated parameters
  void UpdateModelParameters(const Mat& delta_p, Mat& params_local, Vec6d& params_global);

};
//===========================================================================

#endif
