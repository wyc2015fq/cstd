/*
 * PnPProblem.h
 *
 *  Created on: Mar 28, 2014
 *      Author: Edgar Riba
 */

#ifndef PNPPROBLEM_H_
#define PNPPROBLEM_H_

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Mesh.h"
#include "ModelRegistration.h"

class PnPProblem
{

public:
  explicit PnPProblem(const double param[]);  // custom constructor
  virtual ~PnPProblem();

  bool backproject2DPoint(const Mesh *mesh, const CPoint2f &point2d, CPoint3f &point3d);
  bool intersect_MollerTrumbore(Ray &R, Triangle &T, double *out);
  std::vector<CPoint2f> verify_points(Mesh *mesh);
  CPoint2f backproject3DPoint(const CPoint3f &point3d);
  bool estimatePose(const std::vector<CPoint3f> &list_points3d, const std::vector<CPoint2f> &list_points2d, int flags);
  void estimatePoseRANSAC( const std::vector<CPoint3f> &list_points3d, const std::vector<CPoint2f> &list_points2d,
                           int flags, img_t &inliers,
                           int iterationsCount, float reprojectionError, double confidence );

  img_t get_A_matrix() const { return _A_matrix; }
  img_t get_R_matrix() const { return _R_matrix; }
  img_t get_t_matrix() const { return _t_matrix; }
  img_t get_P_matrix() const { return _P_matrix; }

  void set_P_matrix( const img_t &R_matrix, const img_t &t_matrix);

private:
  /** The calibration matrix */
  img_t _A_matrix;
  /** The computed rotation matrix */
  img_t _R_matrix;
  /** The computed translation matrix */
  img_t _t_matrix;
  /** The computed projection matrix */
  img_t _P_matrix;
};

// Functions for Möller–Trumbore intersection algorithm
CPoint3f CROSS(CPoint3f v1, CPoint3f v2);
double DOT(CPoint3f v1, CPoint3f v2);
CPoint3f SUB(CPoint3f v1, CPoint3f v2);

#endif /* PNPPROBLEM_H_ */
