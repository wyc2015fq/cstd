/*
 * Utils.h
 *
 *  Created on: Mar 28, 2014
 *      Author: Edgar Riba
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>

#include "PnPProblem.h"

// Draw a text with the question point
void drawQuestion(img_t image, CPoint3f point, Scalar color);

// Draw a text with the number of entered points
void drawText(img_t image, std::string text, Scalar color);

// Draw a text with the number of entered points
void drawText2(img_t image, std::string text, Scalar color);

// Draw a text with the frame ratio
void drawFPS(img_t image, double fps, Scalar color);

// Draw a text with the frame ratio
void drawConfidence(img_t image, double confidence, Scalar color);

// Draw a text with the number of entered points
void drawCounter(img_t image, int n, int n_max, Scalar color);

// Draw the points and the coordinates
void drawPoints(img_t image, std::vector<CPoint2f> &list_points_2d, std::vector<CPoint3f> &list_points_3d, Scalar color);

// Draw only the 2D points
void draw2DPoints(img_t image, std::vector<CPoint2f> &list_points, Scalar color);

// Draw an arrow into the image
void drawArrow(img_t image, CPoint2i p, CPoint2i q, Scalar color, int arrowMagnitude = 9, int thickness=1, int line_type=8, int shift=0);

// Draw the 3D coordinate axes
void draw3DCoordinateAxes(img_t image, const std::vector<CPoint2f> &list_points2d);

// Draw the object mesh
void drawObjectMesh(img_t image, const Mesh *mesh, PnPProblem *pnpProblem, Scalar color);

// Computes the norm of the translation error
double get_translation_error(const img_t &t_true, const img_t &t);

// Computes the norm of the rotation error
double get_rotation_error(const img_t &R_true, const img_t &R);

// Converts a given Rotation Matrix to Euler angles
img_t rot2euler(const img_t & rotationMatrix);

// Converts a given Euler angles to Rotation Matrix
img_t euler2rot(const img_t & euler);

// Converts a given string to an integer
int StringToInt ( const std::string &Text );

// Converts a given float to a string
std::string FloatToString ( float Number );

// Converts a given integer to a string
std::string IntToString ( int Number );

#endif /* UTILS_H_ */
