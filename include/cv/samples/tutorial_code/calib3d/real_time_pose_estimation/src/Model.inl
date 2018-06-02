/*
 * Model.h
 *
 *  Created on: Apr 9, 2014
 *      Author: edgar
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

class Model
{
public:
  Model();
  virtual ~Model();

  std::vector<CPoint2f> get_points2d_in() const { return list_points2d_in_; }
  std::vector<CPoint2f> get_points2d_out() const { return list_points2d_out_; }
  std::vector<CPoint3f> get_points3d() const { return list_points3d_in_; }
  std::vector<KeyPoint> get_keypoints() const { return list_keypoints_; }
  img_t get_descriptors() const { return descriptors_; }
  int get_numDescriptors() const { return descriptors_.rows; }


  void add_correspondence(const CPoint2f &point2d, const CPoint3f &point3d);
  void add_outlier(const CPoint2f &point2d);
  void add_descriptor(const img_t &descriptor);
  void add_keypoint(const KeyPoint &kp);


  void save(const std::string path);
  void load(const std::string path);


private:
  /** The current number of correspondecnes */
  int n_correspondences_;
  /** The list of 2D points on the model surface */
  std::vector<KeyPoint> list_keypoints_;
  /** The list of 2D points on the model surface */
  std::vector<CPoint2f> list_points2d_in_;
  /** The list of 2D points outside the model surface */
  std::vector<CPoint2f> list_points2d_out_;
  /** The list of 3D points on the model surface */
  std::vector<CPoint3f> list_points3d_in_;
  /** The list of 2D points descriptors */
  img_t descriptors_;
};

#endif /* OBJECTMODEL_H_ */
