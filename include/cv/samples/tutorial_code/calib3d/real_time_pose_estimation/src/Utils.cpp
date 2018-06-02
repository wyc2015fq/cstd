/*
 * Utils.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: Edgar Riba
 */

#include <iostream>

#include "PnPProblem.h"
#include "ModelRegistration.h"
#include "Utils.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

// For text
int fontFace = FONT_ITALIC;
double fontScale = 0.75;
int thickness_font = 2;

// For circles
int lineType = 8;
int radius = 4;
double thickness_circ = -1;

// Draw a text with the question point
void drawQuestion(CvMat image, Point3f point, Scalar color)
{
  std::string x = IntToString((int)point.x);
  std::string y = IntToString((int)point.y);
  std::string z = IntToString((int)point.z);

  std::string text = " Where is point (" + x + ","  + y + "," + z + ") ?";
  putText(image, text, Point(25,50), fontFace, fontScale, color, thickness_font, 8);
}

// Draw a text with the number of entered points
void drawText(CvMat image, std::string text, Scalar color)
{
  putText(image, text, Point(25,50), fontFace, fontScale, color, thickness_font, 8);
}

// Draw a text with the number of entered points
void drawText2(CvMat image, std::string text, Scalar color)
{
  putText(image, text, Point(25,75), fontFace, fontScale, color, thickness_font, 8);
}

// Draw a text with the frame ratio
void drawFPS(CvMat image, double fps, Scalar color)
{
  std::string fps_str = IntToString((int)fps);
  std::string text = fps_str + " FPS";
  putText(image, text, Point(500,50), fontFace, fontScale, color, thickness_font, 8);
}

// Draw a text with the frame ratio
void drawConfidence(CvMat image, double confidence, Scalar color)
{
  std::string conf_str = IntToString((int)confidence);
  std::string text = conf_str + " %";
  putText(image, text, Point(500,75), fontFace, fontScale, color, thickness_font, 8);
}

// Draw a text with the number of entered points
void drawCounter(CvMat image, int n, int n_max, Scalar color)
{
  std::string n_str = IntToString(n);
  std::string n_max_str = IntToString(n_max);
  std::string text = n_str + " of " + n_max_str + " points";
  putText(image, text, Point(500,50), fontFace, fontScale, color, thickness_font, 8);
}

// Draw the points and the coordinates
void drawPoints(CvMat image, std::vector<Point2f> &list_points_2d, std::vector<Point3f> &list_points_3d, Scalar color)
{
  for (unsigned int i = 0; i < list_points_2d.size(); ++i)
  {
    Point2f point_2d = list_points_2d[i];
    Point3f point_3d = list_points_3d[i];

    // Draw Selected points
    circle(image, point_2d, radius, color, -1, lineType );

    std::string idx = IntToString(i+1);
    std::string x = IntToString((int)point_3d.x);
    std::string y = IntToString((int)point_3d.y);
    std::string z = IntToString((int)point_3d.z);
    std::string text = "P" + idx + " (" + x + "," + y + "," + z +")";

    point_2d.x = point_2d.x + 10;
    point_2d.y = point_2d.y - 10;
    putText(image, text, point_2d, fontFace, fontScale*0.5, color, thickness_font, 8);
  }
}

// Draw only the 2D points
void draw2DPoints(CvMat image, std::vector<Point2f> &list_points, Scalar color)
{
  for( size_t i = 0; i < list_points.size(); i++)
  {
    Point2f point_2d = list_points[i];

    // Draw Selected points
    circle(image, point_2d, radius, color, -1, lineType );
  }
}

// Draw an arrow into the image
void drawArrow(CvMat image, Point2i p, Point2i q, Scalar color, int arrowMagnitude, int thickness, int line_type, int shift)
{
  //Draw the principle line
  line(image, p, q, color, thickness, line_type, shift);
  const double PI = CC_PI;
  //compute the angle alpha
  double angle = atan2((double)p.y-q.y, (double)p.x-q.x);
  //compute the coordinates of the first segment
  p.x = (int) ( q.x +  arrowMagnitude * cos(angle + PI/4));
  p.y = (int) ( q.y +  arrowMagnitude * sin(angle + PI/4));
  //Draw the first segment
  line(image, p, q, color, thickness, line_type, shift);
  //compute the coordinates of the second segment
  p.x = (int) ( q.x +  arrowMagnitude * cos(angle - PI/4));
  p.y = (int) ( q.y +  arrowMagnitude * sin(angle - PI/4));
  //Draw the second segment
  line(image, p, q, color, thickness, line_type, shift);
}

// Draw the 3D coordinate axes
void draw3DCoordinateAxes(CvMat image, const std::vector<Point2f> &list_points2d)
{
  Scalar red(0, 0, 255);
  Scalar green(0,255,0);
  Scalar blue(255,0,0);
  Scalar black(0,0,0);

  Point2i origin = list_points2d[0];
  Point2i pointX = list_points2d[1];
  Point2i pointY = list_points2d[2];
  Point2i pointZ = list_points2d[3];

  drawArrow(image, origin, pointX, red, 9, 2);
  drawArrow(image, origin, pointY, blue, 9, 2);
  drawArrow(image, origin, pointZ, green, 9, 2);
  circle(image, origin, radius/2, black, -1, lineType );

}

// Draw the object mesh
void drawObjectMesh(CvMat image, const Mesh *mesh, PnPProblem *pnpProblem, Scalar color)
{
  std::vector<std::vector<int> > list_triangles = mesh->getTrianglesList();
  for( size_t i = 0; i < list_triangles.size(); i++)
  {
    std::vector<int> tmp_triangle = list_triangles.at(i);

    Point3f point_3d_0 = mesh->getVertex(tmp_triangle[0]);
    Point3f point_3d_1 = mesh->getVertex(tmp_triangle[1]);
    Point3f point_3d_2 = mesh->getVertex(tmp_triangle[2]);

    Point2f point_2d_0 = pnpProblem->backproject3DPoint(point_3d_0);
    Point2f point_2d_1 = pnpProblem->backproject3DPoint(point_3d_1);
    Point2f point_2d_2 = pnpProblem->backproject3DPoint(point_3d_2);

    line(image, point_2d_0, point_2d_1, color, 1);
    line(image, point_2d_1, point_2d_2, color, 1);
    line(image, point_2d_2, point_2d_0, color, 1);
  }
}

// Computes the norm of the translation error
double get_translation_error(const CvMat &t_true, const CvMat &t)
{
  return norm( t_true - t );
}

// Computes the norm of the rotation error
double get_rotation_error(const CvMat &R_true, const CvMat &R)
{
  CvMat error_vec, error_mat;
  error_mat = R_true * CvMat(R.inv()).mul(-1);
  Rodrigues(error_mat, error_vec);

  return norm(error_vec);
}

// Converts a given Rotation Matrix to Euler angles
CvMat rot2euler(const CvMat & rotationMatrix)
{
  CvMat euler(3,1,CC_64F);

  double m00 = rotationMatrix.at<double>(0,0);
  double m02 = rotationMatrix.at<double>(0,2);
  double m10 = rotationMatrix.at<double>(1,0);
  double m11 = rotationMatrix.at<double>(1,1);
  double m12 = rotationMatrix.at<double>(1,2);
  double m20 = rotationMatrix.at<double>(2,0);
  double m22 = rotationMatrix.at<double>(2,2);

  double x, y, z;

  // Assuming the angles are in radians.
  if (m10 > 0.998) { // singularity at north pole
    x = 0;
    y = CC_PI/2;
    z = atan2(m02,m22);
  }
  else if (m10 < -0.998) { // singularity at south pole
    x = 0;
    y = -CC_PI/2;
    z = atan2(m02,m22);
  }
  else
  {
    x = atan2(-m12,m11);
    y = asin(m10);
    z = atan2(-m20,m00);
  }

  euler.at<double>(0) = x;
  euler.at<double>(1) = y;
  euler.at<double>(2) = z;

  return euler;
}

// Converts a given Euler angles to Rotation Matrix
CvMat euler2rot(const CvMat & euler)
{
  CvMat rotationMatrix(3,3,CC_64F);

  double x = euler.at<double>(0);
  double y = euler.at<double>(1);
  double z = euler.at<double>(2);

  // Assuming the angles are in radians.
  double ch = cos(z);
  double sh = sin(z);
  double ca = cos(y);
  double sa = sin(y);
  double cb = cos(x);
  double sb = sin(x);

  double m00, m01, m02, m10, m11, m12, m20, m21, m22;

  m00 = ch * ca;
  m01 = sh*sb - ch*sa*cb;
  m02 = ch*sa*sb + sh*cb;
  m10 = sa;
  m11 = ca*cb;
  m12 = -ca*sb;
  m20 = -sh*ca;
  m21 = sh*sa*cb + ch*sb;
  m22 = -sh*sa*sb + ch*cb;

  rotationMatrix.at<double>(0,0) = m00;
  rotationMatrix.at<double>(0,1) = m01;
  rotationMatrix.at<double>(0,2) = m02;
  rotationMatrix.at<double>(1,0) = m10;
  rotationMatrix.at<double>(1,1) = m11;
  rotationMatrix.at<double>(1,2) = m12;
  rotationMatrix.at<double>(2,0) = m20;
  rotationMatrix.at<double>(2,1) = m21;
  rotationMatrix.at<double>(2,2) = m22;

  return rotationMatrix;
}

// Converts a given string to an integer
int StringToInt ( const std::string &Text )
{
   std::istringstream ss(Text);
   int result;
   return ss >> result ? result : 0;
}

// Converts a given float to a string
std::string FloatToString ( float Number )
{
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}

// Converts a given integer to a string
std::string IntToString ( int Number )
{
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}
