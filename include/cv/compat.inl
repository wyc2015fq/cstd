

/*
   A few macros and definitions for backward compatibility
   with the previous versions of OpenCV. They are obsolete and
   are likely to be removed in future. To check whether your code
   uses any of these, define CC_NO_BACKWARD_COMPATIBILITY before
   including cv.h.
*/

#ifndef _CVCOMPAT_H_
#define _CVCOMPAT_H_

#include <string.h>

#ifdef __cplusplus
    #define CC_UNREFERENCED(arg)
#else
    #define CC_UNREFERENCED(arg) arg
#endif

#define CvMatType int
#define CvDisMaskType int
#define CvMatArray img_t

#define CvThreshType int
#define CvAdaptiveThreshMethod int
#define CvCompareMethod int
#define CvFontFace int
#define CvPolyApproxMethod int
#define CvContoursMatchMethod int
#define CvContourTreesMatchMethod int
#define CvCoeffType int
#define CvRodriguesType int
#define CvElementShape int
#define CvMorphOp int
#define CvTemplMatchMethod int

#define CvPoint2D64d CPoint2D64f
#define CvPoint3D64d CPoint3D64f

#define  CC_MAT32F      CC_32FC1
#define  CC_MAT3x1_32F  CC_32FC1
#define  CC_MAT4x1_32F  CC_32FC1
#define  CC_MAT3x3_32F  CC_32FC1
#define  CC_MAT4x4_32F  CC_32FC1

#define  CC_MAT64D      CC_64FC1
#define  CC_MAT3x1_64D  CC_64FC1
#define  CC_MAT4x1_64D  CC_64FC1
#define  CC_MAT3x3_64D  CC_64FC1
#define  CC_MAT4x4_64D  CC_64FC1

#define  IMG_GAUSSIAN_5x5   7
#define  CvBox2D32f     CBox2D

/* allocation/deallocation macros */
#define cvCreateImageData   cvCreateData
#define cvReleaseImageData  cvReleaseData
#define cvSetImageData      cvSetData
#define cvGetImageRawData   cvGetRawData

#define cvmAlloc            cvCreateData
#define cvmFree             cvReleaseData
#define cvmAllocArray       cvCreateData
#define cvmFreeArray        cvReleaseData

#define cvIntegralImage     cvIntegral
#define cvMatchContours     cvMatchShapes


#define cvUpdateMHIByTime  cvUpdateMotionHistory

#define cvAccMask cvAcc
#define cvSquareAccMask cvSquareAcc
#define cvMultiplyAccMask cvMultiplyAcc
#define cvRunningAvgMask(imgY, imgU, mask, alpha) cvRunningAvg(imgY, imgU, alpha, mask)

#define cvSetHistThresh  cvSetHistBinRanges
#define cvCalcHistMask(img, mask, hist, doNotClear) cvCalcHist(img, hist, doNotClear, mask)

CC_INLINE img_t cvMatArray(int rows, int cols, int cn, TypeId type,
                            int count, void* data CC_DEFAULT(0))
{
    return cvMat(rows*count, cols, type, cn, data);
}


CC_INLINE double  cvSumPixels(const img_t* image)
{
    CScalar scalar = cvSum(image);
    return scalar.val[0];
}

CC_INLINE void  cvMean_StdDev(const img_t* image, double* mean, double* sdv,
                               const img_t* mask CC_DEFAULT(0))
{
    CScalar _mean, _sdv;
    cvAvgSdv(image, &_mean, &_sdv, mask);

    if(mean)
        *mean = _mean.val[0];

    if(sdv)
        *sdv = _sdv.val[0];
}


CC_INLINE void cvmPerspectiveProject(const img_t* mat, const img_t* src, img_t* dst)
{
    img_t tsrc, tdst;

    cvReshape(src, &tsrc, 3, 0);
    cvReshape(dst, &tdst, 3, 0);

    cvPerspectiveTransform(&tsrc, &tdst, mat);
}


CC_INLINE void cvFillImage(img_t* mat, double color)
{
    cvSet(mat, cvColorToScalar(color, cvGetElemType(mat)), 0);
}


#define cvCvtPixToPlane cvSplit
#define cvCvtPlaneToPix cvMerge

typedef struct CvRandState
{
    CRNG     state;    /* RNG state (the current seed and carry)*/
    int       disttype; /* distribution type */
    CScalar  param[2]; /* parameters of RNG */
}
CvRandState;


/* Changes RNG range while preserving RNG state */
CC_INLINE  void  cvRandSetRange(CvRandState* state, double param1,
                                 double param2, int index CC_DEFAULT(-1))
{
    if(!state)
    {
        cvError(CC_StsNullPtr, "cvRandSetRange", "Null pointer to RNG state", "cvcompat.h", 0);
        return;
    }

    if((unsigned)(index + 1) > 4)
    {
        cvError(CC_StsOutOfRange, "cvRandSetRange", "index is not in -1..3", "cvcompat.h", 0);
        return;
    }

    if(index < 0)
    {
        state->param[0].val[0] = state->param[0].val[1] =
        state->param[0].val[2] = state->param[0].val[3] = param1;
        state->param[1].val[0] = state->param[1].val[1] = 
        state->param[1].val[2] = state->param[1].val[3] = param2;
    }
    else
    {
        state->param[0].val[index] = param1;
        state->param[1].val[index] = param2;
    }
}


CC_INLINE  void  cvRandInit(CvRandState* state, double param1,
                             double param2, int seed,
                             int disttype CC_DEFAULT(CC_RAND_UNI))
{
    if(!state)
    {
        cvError(CC_StsNullPtr, "cvRandInit", "Null pointer to RNG state", "cvcompat.h", 0);
        return;
    }

    if(disttype != CC_RAND_UNI && disttype != CC_RAND_NORMAL)
    {
        cvError(CC_StsBadFlag, "cvRandInit", "Unknown distribution type", "cvcompat.h", 0);
        return;
    }

    state->state = (uint64)(seed ? seed : -1);
    state->disttype = disttype;
    cvRandSetRange(state, param1, param2, -1);
}


/* Fills array with random numbers */
CC_INLINE void cvRand(CvRandState* state, img_t* arr)
{
    if(!state)
    {
        cvError(CC_StsNullPtr, "cvRand", "Null pointer to RNG state", "cvcompat.h", 0);
        return;
    }
    cvRandArr(&state->state, arr, state->disttype, state->param[0], state->param[1]); 
}

#define cvRandNext(_state) cvRandInt(&(_state)->state)

CC_INLINE void cvbRand(CvRandState* state, float* dst, int len)
{
    img_t mat = cvMat(1, len, CC_32F, (void*)dst);
    cvRand(state, &mat);
}


CC_INLINE void  cvbCartToPolar(const float* y, const float* x,
                                float* magnitude, float* angle, int len)
{
    img_t mx = cvMat(1, len, CC_32F, (void*)x);
    img_t my = mx;
    img_t mm = mx;
    img_t ma = mx;

    my.tt.fl = (float*)y;
    mm.tt.fl = (float*)magnitude;
    ma.tt.fl = (float*)angle;

    cvCartToPolar(&mx, &my, &mm, angle ? &ma : NULL, 1);
}


CC_INLINE void  cvbFastArctan(const float* y, const float* x,
                               float* angle, int len)
{
    img_t mx = cvMat(1, len, CC_32F, (void*)x);
    img_t my = mx;
    img_t ma = mx;

    my.tt.fl = (float*)y;
    ma.tt.fl = (float*)angle;

    cvCartToPolar(&mx, &my, NULL, &ma, 1);
}


CC_INLINE  void  cvbSqrt(const float* x, float* y, int len)
{
    img_t mx = cvMat(1, len, CC_32F, (void*)x);
    img_t my = mx;
    my.tt.fl = (float*)y;

    cvPow(&mx, &my, 0.5);
}


CC_INLINE  void  cvbInvSqrt(const float* x, float* y, int len)
{
    img_t mx = cvMat(1, len, CC_32F, (void*)x);
    img_t my = mx;
    my.tt.fl = (float*)y;

    cvPow(&mx, &my, -0.5);
}


CC_INLINE  void  cvbReciprocal(const float* x, float* y, int len)
{
    img_t mx = cvMat(1, len, CC_32F, (void*)x);
    img_t my = mx;
    my.tt.fl = (float*)y;

    cvPow(&mx, &my, -1);
}


CC_INLINE  void  cvbFastExp(const float* x, double* y, int len)
{
    img_t mx = cvMat(1, len, CC_32F, (void*)x);
    img_t my = cvMat(1, len, CC_64F, y);
    cvExp(&mx, &my);
}


CC_INLINE  void  cvbFastLog(const double* x, float* y, int len)
{
    img_t mx = cvMat(1, len, CC_64F, (void*)x);
    img_t my = cvMat(1, len, CC_32F, y);
    cvLog(&mx, &my);
}


CC_INLINE  CRect  cvContourBoundingRect(void* point_set, int update CC_DEFAULT(0))
{
    return cvBoundingRect(point_set, update);
}


CC_INLINE double cvPseudoInverse(const img_t* src, img_t* dst)
{
    return cvInvert(src, dst, CC_SVD);
}

#define cvPseudoInv cvPseudoInverse

#define cvContourMoments(contour, moments) \
    cvMoments(contour, moments, 0)

#define cvGetPtrAt              cvPtr2D
#define cvGetAt                 cvGet2D
#define cvSetAt(arr,val,y,x)    cvSet2D((arr),(y),(x),(val))

#define cvMeanMask  cvMean
#define cvMean_StdDevMask(img,mask,mean,sdv) cvMean_StdDev(img,mean,sdv,mask)

#define cvNormMask(imgA,imgB,mask,normType) cvNorm(imgA,imgB,normType,mask)

#define cvMinMaxLocMask(img, mask, min_val, max_val, min_loc, max_loc) \
        cvMinMaxLoc(img, min_val, max_val, min_loc, max_loc, mask)

#define cvRemoveMemoryManager  cvSetMemoryManager

#define cvmSetZero(mat)               cvSetZero(mat)
#define cvmSetIdentity(mat)           cvSetIdentity(mat)
#define cvmAdd(src1, src2, dst)       cvAdd(src1, src2, dst, 0)
#define cvmSub(src1, src2, dst)       cvSub(src1, src2, dst, 0)
#define cvmCopy(src, dst)             cvCopy(src, dst, 0)
#define cvmMul(src1, src2, dst)       cvMatMulAdd(src1, src2, 0, dst)
#define cvmTranspose(src, dst)        cvT(src, dst)
#define cvmInvert(src, dst)           cvInv(src, dst)
#define cvmMahalanobis(vec1, vec2, mat) cvMahalanobis(vec1, vec2, mat)
#define cvmDotProduct(vec1, vec2)     cvDotProduct(vec1, vec2)
#define cvmCrossProduct(vec1, vec2,dst) cvCrossProduct(vec1, vec2, dst)
#define cvmTrace(mat)                 (cvTrace(mat)).val[0]
#define cvmMulTransposed(src, dst, order) cvMulTransposed(src, dst, order)
#define cvmEigenVV(mat, evec, eval, eps)   cvEigenVV(mat, evec, eval, eps)
#define cvmDet(mat)                   cvDet(mat)
#define cvmScale(src, dst, scale)     cvScale(src, dst, scale)

#define cvCopyImage(src, dst)         cvCopy(src, dst, 0)
#define cvReleaseMatHeader              cvReleaseMat

/* Calculates exact convex hull of 2d point set */
CC_INLINE void cvConvexHull(CPoint* points, int num_points,
                             CRect* CC_UNREFERENCED(bound_rect),
                             int orientation, int* hull, int* hullsize)
{
    img_t points1 = cvMat(1, num_points, CC_32SC2, points);
    img_t hull1 = cvMat(1, num_points, CC_32SC1, hull);
    
    cvConvexHull2(&points1, &hull1, orientation, 0);
    *hullsize = hull1.cols;
}

/* Calculates exact convex hull of 2d point set stored in a sequence */
#define cvContourConvexHull(contour, orientation, storage) \
    cvConvexHull2(contour, storage, orientation)

/* Calculates approximate convex hull of 2d point set */
#define cvConvexHullApprox(points, num_points, bound_rect, bandwidth,      \
                            orientation, hull, hullsize)                   \
cvConvexHull(points, num_points, bound_rect, orientation, hull, hullsize)

/* Calculates approximate convex hull of 2d point set stored in a sequence */
#define cvContourConvexHullApprox(contour, bandwidth, orientation, storage)   \
    cvConvexHull2(contour, storage, orientation)


CC_INLINE void cvMinAreaRect(CPoint* points, int n,
                              int CC_UNREFERENCED(left), int CC_UNREFERENCED(bottom),
                              int CC_UNREFERENCED(right), int CC_UNREFERENCED(top),
                              CPoint2D32f* anchor,
                              CPoint2D32f* vect1,
                              CPoint2D32f* vect2)
{
    img_t mat = cvMat(1, n, CC_32SC2, points);
    CBox2D box = cvMinAreaRect2(&mat, 0);
    CPoint2D32f pt[4];

    cvBoxPoints(box, pt);
    *anchor = pt[0];
    vect1->x = pt[1].x - pt[0].x;
    vect1->y = pt[1].y - pt[0].y;
    vect2->x = pt[3].x - pt[0].x;
    vect2->y = pt[3].y - pt[0].y;

    CC_UNREFERENCED((left, bottom, right, top));
}

typedef int CvDisType;
typedef int CvChainApproxMethod;
typedef int CvContourRetrievalMode;

CC_INLINE  void  cvFitLine3D(CPoint3D32f* points, int count, int dist,
                              void *param, float reps, float aeps, float* line)
{
    img_t mat = cvMat(1, count, CC_32FC3, points);
    float _param = param != NULL ? *(float*)param : 0.f;
    assert(dist != CC_DIST_USER);
    cvFitLine(&mat, dist, _param, reps, aeps, line);
}

/* Fits a line into set of 2d points in a robust way (M-estimator technique) */ 
CC_INLINE  void  cvFitLine2D(CPoint2D32f* points, int count, int dist,
                              void *param, float reps, float aeps, float* line)
{
    img_t mat = cvMat(1, count, CC_32FC2, points);
    float _param = param != NULL ? *(float*)param : 0.f;
    assert(dist != CC_DIST_USER);
    cvFitLine(&mat, dist, _param, reps, aeps, line);
}


CC_INLINE  void cvFitEllipse(const CPoint2D32f* points, int count, CBox2D* box)
{
    img_t mat = cvMat(1, count, CC_32FC2, (void*)points);
    *box = cvFitEllipse2(&mat);
}

/* Projects 2d points to one of standard coordinate planes
   (i.e. removes one of coordinates) */
CC_INLINE  void  cvProject3D(CPoint3D32f* points3D, int count,
                              CPoint2D32f* points2D,
                              int xIndx CC_DEFAULT(0),
                              int yIndx CC_DEFAULT(1))
{
    img_t src = cvMat(1, count, CC_32FC3, points3D);
    img_t dst = cvMat(1, count, CC_32FC2, points2D);
    float m[6] = {0,0,0,0,0,0};
    img_t M = cvMat(2, 3, CC_32F, m);
    
    assert((unsigned)xIndx < 3 && (unsigned)yIndx < 3);
    m[xIndx] = m[yIndx+3] = 1.f;

    cvTransform(&src, &dst, &M, NULL);
}


/* Retrieves value of the particular bin
   of x-dimensional (x=1,2,3,...) histogram */
#define cvQueryHistValue_1D(hist, idx0) \
    ((float)cvGetReal1D((hist)->bins, (idx0)))
#define cvQueryHistValue_2D(hist, idx0, idx1) \
    ((float)cvGetReal2D((hist)->bins, (idx0), (idx1)))
#define cvQueryHistValue_3D(hist, idx0, idx1, idx2) \
    ((float)cvGetReal3D((hist)->bins, (idx0), (idx1), (idx2)))
#define cvQueryHistValue_nD(hist, idx) \
    ((float)cvGetRealND((hist)->bins, (idx)))

/* Returns pointer to the particular bin of x-dimesional histogram.
   For sparse histogram the bin is created if it didn't exist before */
#define cvGetHistValue_1D(hist, idx0) \
    ((float*)cvPtr1D((hist)->bins, (idx0), 0))
#define cvGetHistValue_2D(hist, idx0, idx1) \
    ((float*)cvPtr2D((hist)->bins, (idx0), (idx1), 0))
#define cvGetHistValue_3D(hist, idx0, idx1, idx2) \
    ((float*)cvPtr3D((hist)->bins, (idx0), (idx1), (idx2), 0))
#define cvGetHistValue_nD(hist, idx) \
    ((float*)cvPtrND((hist)->bins, (idx), 0))


#define CC_IS_SET_ELEM_EXISTS CC_IS_SET_ELEM


CC_INLINE  int  cvHoughLines(img_t* image, double rho,
                              double theta, int threshold,
                              float* lines, int linesNumber)
{
    img_t linesMat = cvMat(1, linesNumber, CC_32FC2, lines); 
    cvHoughLines2(image, &linesMat, CC_HOUGH_STANDARD,
                   rho, theta, threshold, 0, 0);

    return linesMat.cols;
}


CC_INLINE  int  cvHoughLinesP(img_t* image, double rho,
                               double theta, int threshold,
                               int lineLength, int lineGap,
                               int* lines, int linesNumber)
{
    img_t linesMat = cvMat(1, linesNumber, CC_32SC4, lines); 
    cvHoughLines2(image, &linesMat, CC_HOUGH_PROBABILISTIC,
                   rho, theta, threshold, lineLength, lineGap);

    return linesMat.cols;
}


CC_INLINE  int  cvHoughLinesSDiv(img_t* image, double rho, int srn,
                                  double theta, int stn, int threshold,
                                  float* lines, int linesNumber)
{
    img_t linesMat = cvMat(1, linesNumber, CC_32FC2, lines); 
    cvHoughLines2(image, &linesMat, CC_HOUGH_MULTI_SCALE,
                   rho, theta, threshold, srn, stn);

    return linesMat.cols;
}



CC_INLINE int
cvFindChessBoardCornerGuesses(const void* arr, void* CC_UNREFERENCED(thresharr),
                               CvMemStorage * CC_UNREFERENCED(storage),
                               CSize pattern_size, CPoint2D32f * corners,
                               int *corner_count)
{
    return cvFindChessboardCorners(arr, pattern_size, corners,
                                    corner_count, CC_CALIB_CB_ADAPTIVE_THRESH);
}


/* Calibrates camera using multiple views of calibration pattern */
CC_INLINE void cvCalibrateCamera(int image_count, int* _point_counts,
    CSize image_size, CPoint2D32f* _image_points, CPoint3D32f* _object_points,
    float* _distortion_coeffs, float* _camera_matrix, float* _translation_vectors,
    float* _rotation_matrices, int flags)
{
    int i, total = 0;
    img_t point_counts = cvMat(image_count, 1, CC_32SC1, _point_counts);
    img_t image_points, object_points;
    img_t dist_coeffs = cvMat(4, 1, CC_32FC1, _distortion_coeffs);
    img_t camera_matrix = cvMat(3, 3, CC_32FC1, _camera_matrix);
    img_t rotation_matrices = cvMat(image_count, 9, CC_32FC1, _rotation_matrices);
    img_t translation_vectors = cvMat(image_count, 3, CC_32FC1, _translation_vectors);

    for(i = 0; i < image_count; i++)
        total += _point_counts[i];

    image_points = cvMat(total, 1, CC_32FC2, _image_points);
    object_points = cvMat(total, 1, CC_32FC3, _object_points);

    cvCalibrateCamera2(&object_points, &image_points, &point_counts, image_size,
        &camera_matrix, &dist_coeffs, &rotation_matrices, &translation_vectors,
        flags);
}


CC_INLINE void cvCalibrateCamera_64d(int image_count, int* _point_counts,
    CSize image_size, CPoint2D64f* _image_points, CPoint3D64f* _object_points,
    double* _distortion_coeffs, double* _camera_matrix, double* _translation_vectors,
    double* _rotation_matrices, int flags)
{
    int i, total = 0;
    img_t point_counts = cvMat(image_count, 1, CC_32SC1, _point_counts);
    img_t image_points, object_points;
    img_t dist_coeffs = cvMat(4, 1, CC_64FC1, _distortion_coeffs);
    img_t camera_matrix = cvMat(3, 3, CC_64FC1, _camera_matrix);
    img_t rotation_matrices = cvMat(image_count, 9, CC_64FC1, _rotation_matrices);
    img_t translation_vectors = cvMat(image_count, 3, CC_64FC1, _translation_vectors);

    for(i = 0; i < image_count; i++)
        total += _point_counts[i];

    image_points = cvMat(total, 1, CC_64FC2, _image_points);
    object_points = cvMat(total, 1, CC_64FC3, _object_points);

    cvCalibrateCamera2(&object_points, &image_points, &point_counts, image_size,
        &camera_matrix, &dist_coeffs, &rotation_matrices, &translation_vectors,
        flags);
}



/* Find 3d position of object given intrinsic camera parameters,
   3d model of the object and projection of the object into view plane */
CC_INLINE void cvFindExtrinsicCameraParams(int point_count,
    CSize CC_UNREFERENCED(image_size), CPoint2D32f* _image_points,
    CPoint3D32f* _object_points, float* focal_length,
    CPoint2D32f principal_point, float* _distortion_coeffs,
    float* _rotation_vector, float* _translation_vector)
{
    img_t image_points = cvMat(point_count, 1, CC_32FC2, _image_points);
    img_t object_points = cvMat(point_count, 1, CC_32FC3, _object_points);
    img_t dist_coeffs = cvMat(4, 1, CC_32FC1, _distortion_coeffs);
    float a[9];
    img_t camera_matrix = cvMat(3, 3, CC_32FC1, a);
    img_t rotation_vector = cvMat(1, 1, CC_32FC3, _rotation_vector);
    img_t translation_vector = cvMat(1, 1, CC_32FC3, _translation_vector);

    a[0] = focal_length[0]; a[4] = focal_length[1];
    a[2] = principal_point.x; a[5] = principal_point.y;
    a[1] = a[3] = a[6] = a[7] = 0.f;
    a[8] = 1.f;

    cvFindExtrinsicCameraParams2(&object_points, &image_points, &camera_matrix,
        &dist_coeffs, &rotation_vector, &translation_vector);
}


/* Variant of the previous function that takes double-precision parameters */
CC_INLINE void cvFindExtrinsicCameraParams_64d(int point_count,
    CSize CC_UNREFERENCED(image_size), CPoint2D64f* _image_points,
    CPoint3D64f* _object_points, double* focal_length,
    CPoint2D64f principal_point, double* _distortion_coeffs,
    double* _rotation_vector, double* _translation_vector)
{
    img_t image_points = cvMat(point_count, 1, CC_64FC2, _image_points);
    img_t object_points = cvMat(point_count, 1, CC_64FC3, _object_points);
    img_t dist_coeffs = cvMat(4, 1, CC_64FC1, _distortion_coeffs);
    double a[9];
    img_t camera_matrix = cvMat(3, 3, CC_64FC1, a);
    img_t rotation_vector = cvMat(1, 1, CC_64FC3, _rotation_vector);
    img_t translation_vector = cvMat(1, 1, CC_64FC3, _translation_vector);

    a[0] = focal_length[0]; a[4] = focal_length[1];
    a[2] = principal_point.x; a[5] = principal_point.y;
    a[1] = a[3] = a[6] = a[7] = 0.;
    a[8] = 1.;

    cvFindExtrinsicCameraParams2(&object_points, &image_points, &camera_matrix,
        &dist_coeffs, &rotation_vector, &translation_vector);
}



/* Simpler version of the previous function */
CC_INLINE void  cvProjectPointsSimple(int point_count, CPoint3D64f* _object_points,
    double* _rotation_matrix, double*  _translation_vector,
    double* _camera_matrix, double* _distortion, CPoint2D64f* _image_points)
{
    img_t object_points = cvMat(point_count, 1, CC_64FC3, _object_points);
    img_t image_points = cvMat(point_count, 1, CC_64FC2, _image_points);
    img_t rotation_matrix = cvMat(3, 3, CC_64FC1, _rotation_matrix);
    img_t translation_vector = cvMat(3, 1, CC_64FC1, _translation_vector);
    img_t camera_matrix = cvMat(3, 3, CC_64FC1, _camera_matrix);
    img_t dist_coeffs = cvMat(4, 1, CC_64FC1, _distortion);

    cvProjectPoints2(&object_points, &rotation_matrix, &translation_vector,
                      &camera_matrix, &dist_coeffs, &image_points,
                      0, 0, 0, 0, 0);
}


CC_INLINE void cvUnDistortOnce(const img_t* src, img_t* dst,
                                const float* intrinsic_matrix,
                                const float* distortion_coeffs,
                                int CC_UNREFERENCED(interpolate))
{
    img_t _a = cvMat(3, 3, CC_32F, (void*)intrinsic_matrix);
    img_t _k = cvMat(4, 1, CC_32F, (void*)distortion_coeffs);
    cvUndistort2(src, dst, &_a, &_k);
}


/* the two functions below have quite hackerish implementations, use with care
   (or, which is better, switch to cvUndistortInitMap and cvRemap instead */ 
CC_INLINE void cvUnDistortInit(const img_t* CC_UNREFERENCED(src),
                                img_t* undistortion_map,
                                const float* A, const float* k,
                                int CC_UNREFERENCED(interpolate))
{
    union { uchar* ptr; float* fl; } data;
    CSize sz;
    cvGetRawData(undistortion_map, &tt.data, 0, &sz);
    assert(sz.width >= 8);
    /* just save the intrinsic parameters to the map */
    tt.fl[0] = A[0]; tt.fl[1] = A[4];
    tt.fl[2] = A[2]; tt.fl[3] = A[5];
    tt.fl[4] = k[0]; tt.fl[5] = k[1];
    tt.fl[6] = k[2]; tt.fl[7] = k[3];
}

CC_INLINE void  cvUnDistort(const img_t* src, img_t* dst,
                             const img_t* undistortion_map,
                             int CC_UNREFERENCED(interpolate))
{
    union { uchar* ptr; float* fl; } data;
    float a[] = {0,0,0,0,0,0,0,0,1};
    CSize sz;
    cvGetRawData(undistortion_map, &tt.data, 0, &sz);
    assert(sz.width >= 8);
    a[0] = tt.fl[0]; a[4] = tt.fl[1];
    a[2] = tt.fl[2]; a[5] = tt.fl[3];
    cvUnDistortOnce(src, dst, a, tt.fl + 4, 1); 
}


CC_INLINE  float  cvCalcEMD(const float* signature1, int size1,
                             const float* signature2, int size2,
                             int dims, int dist_type CC_DEFAULT(CC_DIST_L2),
                             CvDistanceFunction dist_func CC_DEFAULT(0),
                             float* lower_bound CC_DEFAULT(0),
                             void* user_param CC_DEFAULT(0))
{
    img_t sign1 = cvMat(size1, dims + 1, CC_32FC1, (void*)signature1);
    img_t sign2 = cvMat(size2, dims + 1, CC_32FC1, (void*)signature2);

    return cvCalcEMD2(&sign1, &sign2, dist_type, dist_func, 0, 0, lower_bound, user_param); 
}


CC_INLINE  void  cvKMeans(int num_clusters, float** samples,
                           int num_samples, int vec_size,
                           CTermCriteria termcrit, int* cluster_idx)
{
    img_t* samples_mat = cvCreateMat(num_samples, vec_size, CC_32FC1);
    img_t cluster_idx_mat = cvMat(num_samples, 1, CC_32SC1, cluster_idx);
    int i;
    for(i = 0; i < num_samples; i++)
        memcpy(samples_mat->tt.fl + i*vec_size, samples[i], vec_size*sizeof(float));
    cvKMeans2(samples_mat, num_clusters, &cluster_idx_mat, termcrit);
    cvReleaseMat(&samples_mat);
}


CC_INLINE void  cvStartScanGraph(CvGraph* graph, CvGraphScanner* scanner,
                                  CvGraphVtx* vtx CC_DEFAULT(NULL),
                                  int mask CC_DEFAULT(CC_GRAPH_ALL_ITEMS))
{
    CvGraphScanner* temp_scanner;
    
    if(!scanner)
        cvError(CC_StsNullPtr, "cvStartScanGraph", "Null scanner pointer", "cvcompat.h", 0);

    temp_scanner = cvCreateGraphScanner(graph, vtx, mask);
    *scanner = *temp_scanner;
    cFree(&temp_scanner);
}


CC_INLINE  void  cvEndScanGraph(CvGraphScanner* scanner)
{
    if(!scanner)
        cvError(CC_StsNullPtr, "cvEndScanGraph", "Null scanner pointer", "cvcompat.h", 0);

    if(scanner->stack)
    {
        CvGraphScanner* temp_scanner = (CvGraphScanner*)cAlloc(sizeof(*temp_scanner));
        *temp_scanner = *scanner;
        cvReleaseGraphScanner(&temp_scanner);
        memset(scanner, 0, sizeof(*scanner));
    }
}


#define cvKalmanUpdateByTime  cvKalmanPredict
#define cvKalmanUpdateByMeasurement cvKalmanCorrect

/* old drawing functions */
CC_INLINE  void  cvLineAA(img_t* img, CPoint pt1, CPoint pt2,
                           double color, int scale CC_DEFAULT(0))
{
    cvLine(img, pt1, pt2, cvColorToScalar(color, cvGetElemType(img)), 1, CC_AA, scale);
}

CC_INLINE  void  cvCircleAA(img_t* img, CPoint center, int radius,
                             double color, int scale CC_DEFAULT(0))
{
    cvCircle(img, center, radius, cvColorToScalar(color, cvGetElemType(img)), 1, CC_AA, scale);
}

CC_INLINE  void  cvEllipseAA(img_t* img, CPoint center, CSize axes,
                              double angle, double start_angle,
                              double end_angle, double color,
                              int scale CC_DEFAULT(0))
{
    cvEllipse(img, center, axes, angle, start_angle, end_angle,
               cvColorToScalar(color, cvGetElemType(img)), 1, CC_AA, scale);
}

CC_INLINE  void  cvPolyLineAA(img_t* img, CPoint** pts, int* npts, int contours,
                               int is_closed, double color, int scale CC_DEFAULT(0))
{
    cvPolyLine(img, pts, npts, contours, is_closed,
                cvColorToScalar(color, cvGetElemType(img)),
                1, CC_AA, scale);
}


#define cvMake2DPoints cvConvertPointsHomogenious
#define cvMake3DPoints cvConvertPointsHomogenious

#define cvWarpPerspectiveQMatrix cvGetPerspectiveTransform

/****************************************************************************************\
*                                   Pixel Access Macros                                  *
\****************************************************************************************/

typedef struct _CvPixelPosition8u
{
    unsigned char*   currline;      /* pointer to the start of the current pixel line   */
    unsigned char*   topline;       /* pointer to the start of the top pixel line       */
    unsigned char*   bottomline;    /* pointer to the start of the first line           */
                                    /* which is below the image                         */
    int     x;                      /* current x coordinate (in pixels)               */
    int     width;                  /* width of the image  (in pixels)                */
    int     height;                 /* height of the image  (in pixels)               */
    int     step;                   /* distance between lines (in elements of single   */
                                    /* plane)                                          */
    int     step_arr[3];            /* array: (0, -step, step). It is used for        */
                                    /* vertical moving                                  */
} CvPixelPosition8u;

/* this structure differs from the above only in data type */
typedef struct _CvPixelPosition8s
{
    char*   currline;
    char*   topline;
    char*   bottomline;
    int     x;
    int     width;
    int     height;
    int     step;
    int     step_arr[3];
} CvPixelPosition8s;

/* this structure differs from the CvPixelPosition8u only in data type */
typedef struct _CvPixelPosition32f
{
    float*  currline;
    float*  topline;
    float*  bottomline;
    int     x;
    int     width;
    int     height;
    int     step;
    int     step_arr[3];
} CvPixelPosition32f;


/* Initialize one of the CvPixelPosition structures.   */
/*  pos    - initialized structure                     */
/*  origin - pointer to the left-top corner of the ROI */
/*  step   - width of the whole image in bytes         */
/*  roi    - width & height of the ROI                 */
/*  x, y   - initial position                          */
#define CC_INIT_PIXEL_POS(pos, origin, _step, roi, _x, _y, orientation)    \
    (                                                           \
    (pos).step = (_step)/sizeof((pos).currline[0]) * (orientation ? -1 : 1), \
    (pos).width = (roi).width,                                               \
    (pos).height = (roi).height,                                             \
    (pos).bottomline = (origin) + (pos).step*(pos).height,                   \
    (pos).topline = (origin) - (pos).step,                                   \
    (pos).step_arr[0] = 0,                                                   \
    (pos).step_arr[1] = -(pos).step,                                         \
    (pos).step_arr[2] = (pos).step,                                          \
    (pos).x = (_x),                                                          \
    (pos).currline = (origin) + (pos).step*(_y))


/* Move to specified point (absolute shift) */
/*  pos    - position structure               */
/*  x, y   - coordinates of the new position  */
/*  cs     - number of the image channels     */
#define CC_MOVE_TO(pos, _x, _y, cs)                                                   \
((pos).currline = (_y) >= 0 && (_y) < (pos).height ? (pos).topline + ((_y)+1)*(pos).step : 0, \
 (pos).x = (_x) >= 0 && (_x) < (pos).width ? (_x) : 0, (pos).currline + (_x) * (cs))

/* Get current coordinates                    */
/*  pos    - position structure               */
/*  x, y   - coordinates of the new position  */
/*  cs     - number of the image channels     */
#define CC_GET_CURRENT(pos, cs)  ((pos).currline + (pos).x * (cs))
 
/* Move by one pixel relatively to current position */
/*  pos    - position structure                     */
/*  cs     - number of the image channels           */
 
/* left */
#define CC_MOVE_LEFT(pos, cs) \
 (--(pos).x >= 0 ? (pos).currline + (pos).x*(cs) : 0)
 
/* right */
#define CC_MOVE_RIGHT(pos, cs) \
 (++(pos).x < (pos).width ? (pos).currline + (pos).x*(cs) : 0)
 
/* up */
#define CC_MOVE_UP(pos, cs) \
 (((pos).currline -= (pos).step) != (pos).topline ? (pos).currline + (pos).x*(cs) : 0)
 
/* down */
#define CC_MOVE_DOWN(pos, cs) \
 (((pos).currline += (pos).step) != (pos).bottomline ? (pos).currline + (pos).x*(cs) : 0)
 
/* left up */
#define CC_MOVE_LU(pos, cs) (CC_MOVE_LEFT(pos, cs), CC_MOVE_UP(pos, cs))
 
/* right up */
#define CC_MOVE_RU(pos, cs) (CC_MOVE_RIGHT(pos, cs), CC_MOVE_UP(pos, cs))
 
/* left down */
#define CC_MOVE_LD(pos, cs) (CC_MOVE_LEFT(pos, cs), CC_MOVE_DOWN(pos, cs))
 
/* right down */
#define CC_MOVE_RD(pos, cs) (CC_MOVE_RIGHT(pos, cs), CC_MOVE_DOWN(pos, cs))
 
 
 
/* Move by one pixel relatively to current position with wrapping when the position     */
/* achieves image boundary                                                              */
/*  pos    - position structure                                                         */
/*  cs     - number of the image channels                                               */
 
/* left */
#define CC_MOVE_LEFT_WRAP(pos, cs) \
 ((pos).currline + (--(pos).x >= 0 ? (pos).x : ((pos).x = (pos).width-1))*(cs))
 
/* right */
#define CC_MOVE_RIGHT_WRAP(pos, cs) \
 ((pos).currline + (++(pos).x < (pos).width ? (pos).x : ((pos).x = 0))*(cs))
 
/* up */
#define CC_MOVE_UP_WRAP(pos, cs) \
    ((((pos).currline -= (pos).step) != (pos).topline ? \
    (pos).currline : ((pos).currline = (pos).bottomline - (pos).step)) + (pos).x*(cs))
 
/* down */
#define CC_MOVE_DOWN_WRAP(pos, cs) \
    ((((pos).currline += (pos).step) != (pos).bottomline ? \
    (pos).currline : ((pos).currline = (pos).topline + (pos).step)) + (pos).x*(cs))
 
/* left up */
#define CC_MOVE_LU_WRAP(pos, cs) (CC_MOVE_LEFT_WRAP(pos, cs), CC_MOVE_UP_WRAP(pos, cs))
/* right up */
#define CC_MOVE_RU_WRAP(pos, cs) (CC_MOVE_RIGHT_WRAP(pos, cs), CC_MOVE_UP_WRAP(pos, cs))
/* left down */
#define CC_MOVE_LD_WRAP(pos, cs) (CC_MOVE_LEFT_WRAP(pos, cs), CC_MOVE_DOWN_WRAP(pos, cs))
/* right down */
#define CC_MOVE_RD_WRAP(pos, cs) (CC_MOVE_RIGHT_WRAP(pos, cs), CC_MOVE_DOWN_WRAP(pos, cs))
 
/* Numeric constants which used for moving in arbitrary direction  */
#define CC_SHIFT_NONE   2
#define CC_SHIFT_LEFT   1
#define CC_SHIFT_RIGHT  3
#define CC_SHIFT_UP     6
#define CC_SHIFT_DOWN  10
#define CC_SHIFT_LU     5
#define CC_SHIFT_RU     7
#define CC_SHIFT_LD     9
#define CC_SHIFT_RD    11
 
/* Move by one pixel in specified direction                                     */
/*  pos    - position structure                                                 */
/*  shift  - direction (it's value must be one of the CC_SHIFT_… constants) */
/*  cs     - number of the image channels                                       */
#define CC_MOVE_PARAM(pos, shift, cs)                                             \
    ((pos).currline += (pos).step_arr[(shift)>>2], (pos).x += ((shift)&3)-2,       \
    ((pos).currline != (pos).topline && (pos).currline != (pos).bottomline &&       \
    (pos).x >= 0 && (pos).x < (pos).width) ? (pos).currline + (pos).x*(cs) : 0)
 
/* Move by one pixel in specified direction with wrapping when the               */
/* position achieves image boundary                                              */
/*  pos    - position structure                                                  */
/*  shift  - direction (it's value must be one of the CC_SHIFT_… constants)  */
/*  cs     - number of the image channels                                        */
#define CC_MOVE_PARAM_WRAP(pos, shift, cs)                                        \
    ((pos).currline += (pos).step_arr[(shift)>>2],                                 \
    (pos).currline = ((pos).currline == (pos).topline ?                             \
    (pos).bottomline - (pos).step :                                                 \
    (pos).currline == (pos).bottomline ?                                            \
    (pos).topline + (pos).step : (pos).currline),                                   \
                                                                                    \
    (pos).x += ((shift)&3)-2,                                                       \
    (pos).x = ((pos).x < 0 ? (pos).width-1 : (pos).x >= (pos).width ? 0 : (pos).x), \
                                                                                    \
    (pos).currline + (pos).x*(cs))

#endif/*_CVCOMPAT_H_*/
