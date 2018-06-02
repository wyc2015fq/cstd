#include "rectification.h"
int main() 
{
  // declaration
  Rectification Rec;
  IplImage *inImage = 0;
  IplImage *outImage = 0;
  int outHeight, outWidth, outChannels;
  char inImageName[80];
  // load an image
  //printf("Type input image name : ");
  //scanf("%s", inImageName);
  //inImage = cvLoadImage(inImageName, 1);
  inImage = cvLoadImage("12.bmp", 1);
  if(!inImage){
    printf("Could not load image file: %s\n", inImageName);
    exit(0);
  }
  // create the output image
  outHeight = inImage->height;
  outWidth = inImage->width;
  outChannels = inImage->nChannels;
  outImage = cvCreateImage(cvSize(outWidth, outHeight),IMG_DEPTH_8U, outChannels);
  /////////////////////////////////////////////
  // affine rectification //
  /////////////////////////////////////////////
  // get the distorted image's 4 point positions
  int inX1, inY1, inX2, inY2, inX3, inY3, inX4, inY4;
  
#if 1
  {
    float aaa1[]={
      144, 68,
        358, 33,
        356, 326,
        63 , 321
    };
    float aaa[]={
      0, 0,
        100, 0,
        0 , 100,
        100, 100,
    };
    float* aa=aaa;
    inX1=*aa++, inY1=*aa++;
    inX2=*aa++, inY2=*aa++;
    inX4=*aa++, inY4=*aa++;
    inX3=*aa++, inY3=*aa++;
    
  }
  
#else
  printf("\n Type the 1st point in the distorted image (x1, y1) : ");
  scanf("%d, %d", &inX1, &inY1);
  printf("\n Type the 2nd point in the distorted image (x2, y2) : ");
  scanf("%d, %d", &inX2, &inY2);
  printf("\n Type the 3rd point in the distorted image (x3, y3) : ");
  scanf("%d, %d", &inX3, &inY3);
  printf("\n Type the 4th point in the distorted image (x4, y4) : ");
  scanf("%d, %d", &inX4, &inY4);
#endif
  // homogeneous representation of the 4 given points in the distorted image
  float aArr[3] = {inX1, inY1, 1};
  float bArr[3] = {inX2, inY2, 1};
  float cArr[3] = {inX3, inY3, 1};
  float dArr[3] = {inX4, inY4, 1};
  CvMat *a, *b, *c, *d;
  a = cvCreateMat(3, 1, CC_32FC1);
  b = cvCreateMat(3, 1, CC_32FC1);
  c = cvCreateMat(3, 1, CC_32FC1);
  d = cvCreateMat(3, 1, CC_32FC1);
  Rec.Array2CvMat(aArr, a, 3, 1);
  Rec.Array2CvMat(bArr, b, 3, 1);
  Rec.Array2CvMat(cArr, c, 3, 1);
  Rec.Array2CvMat(dArr, d, 3, 1);
  // for affine rectification, we apply a matrix
  // s.t. H = Ha*[1 0 0; 0 1 0; l1 l2 l3],
  // where Ha is an affine transform and [l1 l2 l3] is the vanishing line.
  // also, we can select any Ha for the affine rectification.
  // so, following affine transform is chosen to fit the result image
  // on the given window.
  // affine transfom for affine rectification
  float affineXform[9] = {2,7,0,0,2,0,0,0,1};
  CvMat Ha = cvMat(3, 3, CC_32FC1, affineXform);
  CvMat *H; // affine rectification matrix
  H = cvCreateMat(3, 3, CC_32FC1);
  Rec.AffineRectification(inImage, outImage, a, b, c, d, &Ha, H);
  /////////////////////////////////////////////
  // metric rectification //
  /////////////////////////////////////////////
  cvCopyImage (outImage, inImage);
  // pixel shift for the affine transform in metric rectification
  float shiftX = 70, shiftY = 60;
  // calculate the corresponding 4 points
  CvMat *aa, *bb, *cc, *dd;
  // corresponding 4 points in affinely rectified image
  aa = cvCreateMat(3, 1, CC_32FC1);
  bb = cvCreateMat(3, 1, CC_32FC1);
  cc = cvCreateMat(3, 1, CC_32FC1);
  dd = cvCreateMat(3, 1, CC_32FC1);
  cvMatMul(H, a, aa); // x' = Hx, where H is the affine rectification matrix
  cvMatMul(H, b, bb);
  cvMatMul(H, c, cc);
  cvMatMul(H, d, dd);
  Rec.MetricRectification(inImage, outImage, aa, bb, cc, dd, shiftX, shiftY);
  // release matrices
  cvReleaseMat(&a); cvReleaseMat(&b); cvReleaseMat(&c); cvReleaseMat(&d);
  cvReleaseMat(&aa); cvReleaseMat(&bb); cvReleaseMat(&cc); cvReleaseMat(&dd);
  cvReleaseMat(&H);
  // release the images
  cvReleaseImage(&inImage);
  cvReleaseImage(&outImage);
  return 0;
}