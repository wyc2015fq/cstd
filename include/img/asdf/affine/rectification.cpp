#include "rectification.h"
void Rectification::AffineRectification(IplImage*inImage,IplImage*outImage,
                                        CvMat *a, CvMat *b, CvMat *c, CvMat *d,
                                        CvMat *Ha, CvMat *H)
{
  
    // get the distorted image's 4 point positions
    // homogeneous representation of the 4 given points in the distorted image
    uchar *inData;
  uchar *outData;
  int inHeight, inWidth, inStep, inChannels;
  int outHeight, outWidth, outStep, outChannels;
  int i, j, k;
  char outImageName[80];
  CvMat *vanishLine;
  CvMat *Hv2i, *invH; // matrices for affine rectification
  vanishLine = cvCreateMat(3, 1, CC_32FC1);
  Hv2i = cvCreateMat(3, 3, CC_32FC1);
  invH = cvCreateMat(3, 3, CC_32FC1);
  // get the input image data
  inHeight = inImage->height;
  inWidth = inImage->width;
  inStep = inImage->widthStep;
  inChannels = inImage->nChannels;
  inData = (uchar *)inImage->imageData;
  // create the affinely rectified image
  outHeight = inHeight;
  outWidth = inWidth;
  outChannels = inChannels;
  outStep = outImage->widthStep;
  outData = (uchar *)outImage->imageData;
  // initialize result image
  for(i = 0; i < outHeight; i++){
    for(j = 0; j < outWidth; j++){
      for(k = 0; k < outChannels; k++){
        outData[i*outStep + j*outChannels + k] = 0;
      }
    }
  }
  // get vanishing line
  CalculateVanishingLine(a, b, c, d, vanishLine);
  // vanish2Inf : vanish line to infinity line
  float vl1 = cvmGet(vanishLine, 0, 0);
  float vl2 = cvmGet(vanishLine, 1, 0);
  float vl3 = cvmGet(vanishLine, 2, 0);
  float vanish2Inf[9] = {1, 0, 0,
    0, 1, 0,
    vl1, vl2, vl3};
  // Hv2i : vanish line to infinity line
  Array2CvMat(vanish2Inf, Hv2i, 3, 3);
  // H = Ha * Hv2i : affine rectification matrix
  cvMatMul(Ha, Hv2i, H);
  cvInvert(H, invH);
  // apply the transform to get the target image
  // -----------------------------------------------------
  // out(x') = in(x) : has 2 implementation forms
  // case 1 : out(Hx) = in(x)
  // case 2 : out(x') = inv(H)x'
  float h[9];
  if(IMPLEMENTATION == 1){ // case 1 : out(Hx) = in(x)
    CvMat2Array(H, h, 3, 3);
  }else{ // case 2 : x = inv(H)x'
    CvMat2Array(invH, h, 3, 3);
  }
  int ii, jj;
  float x1, x2, x3;
  for(i = 0; i < inHeight-3; i++){ // case 1: i, j: x, ii, jj: x', x' = Hx
    for(j = 0; j < inWidth; j++){// case 2: i, j: x', ii, jj: x, x = invHx'
      for(k = 0; k < inChannels; k++){ // x: distorted, x': undistorted
        x1 = h[0] * j + h[1] * i + h[2];
        x2 = h[3] * j + h[4] * i + h[5];
        x3 = h[6] * j + h[7] * i + 1;
        ii = min(outHeight - 1, max(0, (int)(x2 / x3)));
        jj = min(outWidth - 1, max(0, (int)(x1 / x3)));
        if(IMPLEMENTATION == 1) // case 1 : out(Hx) = in(x)
          outData[ii*outStep + jj*outChannels + k]
          = inData[i*inStep + j*inChannels + k];
        else // case 2 : out(x') = in(inv(H)x')
          outData[i*outStep + j*outChannels + k]
          = inData[ii*inStep + jj*inChannels + k];
      }
    }
  }
  // create windows
  cvNamedWindow("input image", CC_WINDOW_AUTOSIZE);
  cvNamedWindow("output image", CC_WINDOW_AUTOSIZE);
  // show the images
  cvShowImage("input image", inImage);
  cvShowImage("output image", outImage);
  // wait for a key
  cvWaitKey(0);
  cvDestroyWindow("input image");
  cvDestroyWindow("output image");
  // release matrices
  cvReleaseMat(&vanishLine);
  cvReleaseMat(&Hv2i);
  cvReleaseMat(&invH);
  // write output image
  sprintf(outImageName, "result1.jpg");
  if(!cvSaveImage(outImageName, outImage)){
    printf("Could not save: %s\n",outImageName);
  }
}
void Rectification::MetricRectification(IplImage *inImage, IplImage *outImage,
                                        CvMat *a, CvMat *b, CvMat *c, CvMat *d,
                                        float shiftX, float shiftY)
{
  uchar *inData;
  uchar *outData;
  int inHeight, inWidth, inStep, inChannels;
  int outHeight, outWidth, outStep, outChannels;
  int i, j, k;
  char outImageName[80];
  CvMat *m1, *l1, *m2, *l2; // 2 pairs of right angle lines
  CvMat Mat, Vec, *S_Vec; // matrix and vectors to calculate S_Mat
  CvMat *S, *U, *D2, *V; // matrices for SVD of S_Mat
  CvMat *D, *A, *temp, *U_t; // matrices to reconstruct A_Mat
  CvMat *H, *invH; // affine transform H, and its inverse
  m1 = cvCreateMat(3, 1, CC_32FC1);
  l1 = cvCreateMat(3, 1, CC_32FC1);
  m2 = cvCreateMat(3, 1, CC_32FC1);
  l2 = cvCreateMat(3, 1, CC_32FC1);
  S_Vec = cvCreateMat(2, 1, CC_32FC1);
  S = cvCreateMat(2, 2, CC_32FC1);
  U = cvCreateMat(2, 2, CC_32FC1);
  D2 = cvCreateMat(2, 2, CC_32FC1);
  V = cvCreateMat(2, 2, CC_32FC1);
  A = cvCreateMat(2, 2, CC_32FC1);
  D = cvCreateMat(2, 2, CC_32FC1);
  temp = cvCreateMat(2, 2, CC_32FC1);
  U_t = cvCreateMat(2, 2, CC_32FC1);
  H = cvCreateMat(3, 3, CC_32FC1);
  invH = cvCreateMat(3, 3, CC_32FC1);
  // get the input image data
  inHeight = inImage->height;
  inWidth = inImage->width;
  inStep = inImage->widthStep;
  inChannels = inImage->nChannels;
  inData = (uchar *)inImage->imageData;
  // create the affinely rectified image
  outHeight = inHeight;
  outWidth = inWidth;
  outChannels = inChannels;
  outStep = outImage->widthStep;
  outData = (uchar *)outImage->imageData;
  // initialize result image
  for(i = 0; i < outHeight; i++){
    for(j = 0; j < outWidth; j++){
      for(k = 0; k < outChannels; k++){
        outData[i*outStep + j*outChannels + k] = 0;
      }
    }
  }
  // get 2 pairs of orthogonal lines to calculate the affine transform
  cvCrossProduct(a, b, m1);
  cvCrossProduct(b, d, l1);
  cvCrossProduct(a, d, m2);
  cvCrossProduct(b, c, l2);
  // affine transform : H = [A t; 0 0 1]
  // S = A * A^T
  // calculate S (2 by 2)
  // -------------------------------------------------------------------
  // (m1, l1) & (m2, l2) are the orthogonal line pairs in the affinely
  // rectified image.
  // (l1(1)m1(1), l1(1)m1(2) + l1(2)m1(1), l1(2)m1(2))s = 0
  // (l2(1)m2(1), l2(1)m2(2) + l2(2)m2(1), l2(2)m2(2))s = 0
  // if we let s22 as 1, then above equations have the following form
  // [l1(1)m1(1) l1(2)m1(1)+l1(1)m1(2)] [s11] = [-l1(2)m1(2)]
  // [l2(1)m2(1) l2(2)m2(1)+l2(1)m2(2)] [s12] [-l2(2)m2(2)]
  float mat1, mat2, mat3, mat4, vec1, vec2;
  mat1 = cvmGet(m1, 0, 0)*cvmGet(l1, 0, 0);
  mat2 = cvmGet(m1, 1, 0)*cvmGet(l1, 0, 0)
    + cvmGet(m1, 0, 0)*cvmGet(l1, 1, 0);
  mat3 = cvmGet(m2, 0, 0)*cvmGet(l2, 0, 0);
  mat4 = cvmGet(m2, 1, 0)*cvmGet(l2, 0, 0)
    + cvmGet(m2, 0, 0)*cvmGet(l2, 1, 0);
  vec1 = cvmGet(m1, 1, 0)*cvmGet(l1, 1, 0);
  vec2 = cvmGet(m2, 1, 0)*cvmGet(l2, 1, 0);
  float mat[4] = {mat1, mat2,
    mat3, mat4};
  float vec[2] = {-vec1, -vec2};
  Mat = cvMat(2, 2 , CC_32FC1, mat);
  Vec = cvMat(2, 1 , CC_32FC1, vec);
  cvSolve(&Mat, &Vec, S_Vec);
  // S matrix and SVD
  float s_mat[4] = {cvmGet(S_Vec, 0, 0), cvmGet(S_Vec, 1, 0),
    cvmGet(S_Vec, 1, 0), 1}; // symmetric matrix
  Array2CvMat(s_mat, S, 2, 2);
  cvSVD(S, D2, U, V, CC_SVD_U_T|CC_SVD_V_T);
  // S_Mat = U^T D2 V : opencv setting
  // affine transform : A_Mat = U^T D V
  cvPow(D2, D, 0.5);
  cvTranspose(U, U_t);
  cvMatMul(U_t, D, temp);
  cvMatMul(temp, V, A);
  // apply the transform to get the target image
  // -----------------------------------------------------
  // x' = H x
  float h[9] = {cvmGet(A, 0, 0), cvmGet(A, 0, 1), shiftX,
    cvmGet(A, 1, 0), cvmGet(A, 1, 1), shiftY,
    0, 0, 1};
  Array2CvMat(h, H, 3, 3);
  cvInvert(H, invH);
  if(IMPLEMENTATION == 1){ // case 1 : out(inv(H)x') = in(x')
    CvMat2Array(invH, h, 3, 3);
  }else{ // case 2 : out(x) = in(Hx)
    CvMat2Array(H, h, 3, 3);
  }
  int ii, jj;
  float x1, x2, x3;
  for(i = 0; i < inHeight-3; i++){ // case 1: i, j: x, ii, jj: x', x' = invHx
    for(j = 0; j < inWidth; j++){// case 2: i, j: x', ii, jj: x, x = Hx'
      for(k = 0; k < inChannels; k++){ // x: distorted, x': undistorted
        x1 = h[0] * j + h[1] * i + h[2];
        x2 = h[3] * j + h[4] * i + h[5];
        x3 = h[6] * j + h[7] * i + 1;
        ii = min(outHeight - 1, max(0, (int)(x2 / x3)));
        jj = min(outWidth - 1, max(0, (int)(x1 / x3)));
        if(IMPLEMENTATION == 1) // case 1 : out(inv(H)x') = in(x')
          outData[ii*outStep + jj*outChannels + k]
          = inData[i*inStep + j*inChannels + k];
        else // case 2 : out(x) = in(Hx)
          outData[i*outStep + j*outChannels + k]
          = inData[ii*inStep + jj*inChannels + k];
      }
    }
  }
  // create windows
  cvNamedWindow("input image", CC_WINDOW_AUTOSIZE);
  cvNamedWindow("output image", CC_WINDOW_AUTOSIZE);
  // show the images
  cvShowImage("input image", inImage);
  cvShowImage("output image", outImage);
  // wait for a key
  cvWaitKey(0);
  cvDestroyWindow("input image");
  cvDestroyWindow("output image");
  // release matrices
  cvReleaseMat(&m1); cvReleaseMat(&l1); cvReleaseMat(&m2); cvReleaseMat(&l2);
  cvReleaseMat(&S_Vec); cvReleaseMat(&S);
  cvReleaseMat(&U); cvReleaseMat(&D2); cvReleaseMat(&V);
  cvReleaseMat(&D); cvReleaseMat(&A); cvReleaseMat(&U_t);
  cvReleaseMat(&temp);
  cvReleaseMat(&H); cvReleaseMat(&invH);
  // write output image
  sprintf(outImageName, "result2.jpg");
  if(!cvSaveImage(outImageName, outImage)){
    printf("Could not save: %s\n",outImageName);
  }
}

//
// function : CalculateVanishingLine
// usage : CalculateVanishingLine(a, b, c, d, vanishingLine);
// ------------------------------------------------------------------
// This function produces the vanishing line corresponding the give
// 4 points, a, b, c, and d.
//
void Rectification::CalculateVanishingLine(CvMat *a, CvMat *b,
                                           CvMat *c, CvMat *d,
                                           CvMat *vanishLine)
{
  CvMat *line_ab, *line_cd, *line_ac, *line_bd;
  CvMat *vanishPt1, *vanishPt2;
  line_ab = cvCreateMat(3, 1, CC_32FC1);
  line_cd = cvCreateMat(3, 1, CC_32FC1);
  line_ac = cvCreateMat(3, 1, CC_32FC1);
  line_bd = cvCreateMat(3, 1, CC_32FC1);
  vanishPt1 = cvCreateMat(3, 1, CC_32FC1);
  vanishPt2 = cvCreateMat(3, 1, CC_32FC1);
  // get 4 lines to calculate the 2 vanishing points
  cvCrossProduct(a, b, line_ab); // the line that passes the pt a & b
  cvCrossProduct(c, d, line_cd);
  cvCrossProduct(a, c, line_ac);
  cvCrossProduct(b, d, line_bd);
  // get vanishing points
  cvCrossProduct(line_ab, line_cd, vanishPt1);
  cvCrossProduct(line_ac, line_bd, vanishPt2);
  // get vanishing line
  cvCrossProduct(vanishPt1, vanishPt2, vanishLine);
}
void Rectification::Array2CvMat(float *arr, CvMat *cvArr, int row,
                                int column) {
  int i, j;
  for(i = 0; i < row; i++){
    for(j = 0; j < column; j++){
      cvmSet(cvArr, i, j, arr[i*column + j]);
    }
  }
}
void Rectification::CvMat2Array(CvMat *cvArr, float *arr, int row,
                                int column) {
  int i, j;
  for(i = 0; i < row; i++){
    for(j = 0; j < column; j++){
      arr[i*column + j] = cvmGet(cvArr, i, j);
    }
  }
}