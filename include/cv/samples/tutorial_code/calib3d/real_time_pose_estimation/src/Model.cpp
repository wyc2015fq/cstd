/*
 * Model.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: edgar
 */

#include "Model.h"
#include "CsvWriter.h"

Model::Model() : list_points2d_in_(0), list_points2d_out_(0), list_points3d_in_(0)
{
  n_correspondences_ = 0;
}

Model::~Model()
{
  // TODO Auto-generated destructor stub
}

void Model::add_correspondence(const Point2f &point2d, const Point3f &point3d)
{
  list_points2d_in_.push_back(point2d);
  list_points3d_in_.push_back(point3d);
  n_correspondences_++;
}

void Model::add_outlier(const Point2f &point2d)
{
  list_points2d_out_.push_back(point2d);
}

void Model::add_descriptor(const CvMat &descriptor)
{
  descriptors_.push_back(descriptor);
}

void Model::add_keypoint(const KeyPoint &kp)
{
  list_keypoints_.push_back(kp);
}


/** Save a CSV file and fill the object mesh */
void Model::save(const std::string path)
{
  CvMat points3dmatrix = CvMat(list_points3d_in_);
  CvMat points2dmatrix = CvMat(list_points2d_in_);
  //CvMat keyPointmatrix = CvMat(list_keypoints_);

  FileStorage storage(path, FileStorage::WRITE);
  storage << "points_3d" << points3dmatrix;
  storage << "points_2d" << points2dmatrix;
  storage << "keypoints" << list_keypoints_;
  storage << "descriptors" << descriptors_;

  storage.release();
}

/** Load a YAML file using OpenCv functions **/
void Model::load(const std::string path)
{
  CvMat points3d_mat;

  FileStorage storage(path, FileStorage::READ);
  storage["points_3d"] >> points3d_mat;
  storage["descriptors"] >> descriptors_;

  points3d_mat.copyTo(list_points3d_in_);

  storage.release();

}
