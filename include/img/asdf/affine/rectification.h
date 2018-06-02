#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#define IMPLEMENTATION 2
#define min(a, b) ((a <= b) ? a : b)
#define max(a, b) ((a >= b) ? a : b)
class Rectification {
public:
void AffineRectification(IplImage *inImage, IplImage *outImage,
CvMat *a, CvMat *b, CvMat *c, CvMat *d,
CvMat *Ha, CvMat *H);
void MetricRectification(IplImage *inImage, IplImage *outImage,
CvMat *a, CvMat *b, CvMat *c, CvMat *d,
float shiftX, float shiftY);
void Array2CvMat(float *arr, CvMat *cvArr, int row, int column);
void CvMat2Array(CvMat *cvArr, float *arr, int row, int column);
private:
void CalculateVanishingLine(CvMat *a, CvMat *b, CvMat *c, CvMat *d,
CvMat *vanishLine);
};