/**
 * @file SBM_Sample
 * @brief Get a disparity map of two images
 * @author A. Huaman
 */

#include <iostream>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

const char *windowDisparity = "Disparity";

void readme();

/**
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )
{
  if( argc != 3 )
  { readme(); return -1; }

  //-- 1. Read the images
  CvMat imgLeft = imread( argv[1], IMREAD_GRAYSCALE );
  CvMat imgRight = imread( argv[2], IMREAD_GRAYSCALE );
  //-- And create the image in which we will save our disparities
  CvMat imgDisparity16S = CvMat( imgLeft.rows, imgLeft.cols, CC_16S );
  CvMat imgDisparity8U = CvMat( imgLeft.rows, imgLeft.cols, CC_8UC1 );

  if( imgLeft.empty() || imgRight.empty() )
  { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

  //-- 2. Call the constructor for StereoBM
  int ndisparities = 16*5;   /**< Range of disparity */
  int SADWindowSize = 21; /**< CvSize of the block window. Must be odd */

  Ptr<StereoBM> sbm = StereoBM::create( ndisparities, SADWindowSize );

  //-- 3. Calculate the disparity image
  sbm->compute( imgLeft, imgRight, imgDisparity16S );

  //-- Check its extreme values
  double minVal; double maxVal;

  minMaxLoc( imgDisparity16S, &minVal, &maxVal );

  printf("Min disp: %f Max value: %f \n", minVal, maxVal);

  //-- 4. Display it as a CC_8UC1 image
  imgDisparity16S.convertTo( imgDisparity8U, CC_8UC1, 255/(maxVal - minVal));

  namedWindow( windowDisparity, WINDOW_NORMAL );
  imshow( windowDisparity, imgDisparity8U );

  //-- 5. Save the image
  imwrite("SBM_sample.png", imgDisparity16S);

  waitKey(0);

  return 0;
}

/**
 * @function readme
 */
void readme()
{ std::cout << " Usage: ./SBMSample <imgLeft> <imgRight>" << std::endl; }
