

#ifndef _CC_H_
#define _CC_H_

#ifdef __IMG_H__
#define HAVE_IPL
#endif

#ifndef SKIP_INCLUDES
  #if defined(_CH_)
    #pragma package <chopencv>
    #include <chdl.h>
    LOAD_CHDL(cv)
  #endif
#endif

#include "cxcore.h"
#include "cvtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************\
*                                    Image Processing                                    *
\****************************************************************************************/

/* Copies source 2D array inside of the larger destination array and
   makes a border of the specified type (IMG_BORDER_*) around the copied area. */
CVAPI(void) cvCopyMakeBorder(const img_t* src, img_t* dst, CPoint offset,
                              int bordertype, CScalar value CC_DEFAULT(cScalarAll(0)));

#define CC_BLUR_NO_SCALE 0
#define CC_BLUR  1
#define CC_GAUSSIAN  2
#define CC_MEDIAN 3
#define CC_BILATERAL 4

/* Smoothes array (removes noise) */
CVAPI(void) cvSmooth(const img_t* src, img_t* dst,
                      int smoothtype CC_DEFAULT(CC_GAUSSIAN),
                      int param1 CC_DEFAULT(3),
                      int param2 CC_DEFAULT(0),
                      double param3 CC_DEFAULT(0),
                      double param4 CC_DEFAULT(0));

/* Convolves the image with the kernel */
CVAPI(void) cvFilter2D(const img_t* src, img_t* dst, const img_t* kernel,
                        CPoint anchor CC_DEFAULT(cPoint(-1,-1)));

/* Finds integral image: SUM(X,Y) = sum(x<X,y<Y)I(x,y) */
CVAPI(void) cvIntegral(const img_t* image, img_t* sum,
                       img_t* sqsum CC_DEFAULT(NULL),
                       img_t* tilted_sum CC_DEFAULT(NULL));

/*
   Smoothes the input image with gaussian kernel and then down-samples it.
   dst_width = floor(src_width/2)[+1],
   dst_height = floor(src_height/2)[+1]
*/
CVAPI(void)  cvPyrDown(const img_t* src, img_t* dst,
                        int filter CC_DEFAULT(CC_GAUSSIAN_5x5));

/* 
   Up-samples image and smoothes the result with gaussian kernel.
   dst_width = src_width*2,
   dst_height = src_height*2
*/
CVAPI(void)  cvPyrUp(const img_t* src, img_t* dst,
                      int filter CC_DEFAULT(CC_GAUSSIAN_5x5));


/* Builds the whole pyramid at once. Output array of img_t headers (levels[*])
   is initialized with the headers of subsequent pyramid levels */
/*CVAPI  void  cvCalcPyramid(const img_t* src, img_t* container,
                              img_t* levels, int level_count,
                              int filter CC_DEFAULT(CC_GAUSSIAN_5x5));*/


/* Splits color or grayscale image into multiple connected components
   of nearly the same color/brightness using modification of Burt algorithm.
   comp with contain a pointer to sequence (CvSeq)
   of connected components (CConnectedComp) */
CVAPI(void) cvPyrSegmentation(img_t* src, img_t* dst,
                              CvMemStorage* storage, CvSeq** comp,
                              int level, double threshold1,
                              double threshold2);

/* Filters image using meanshift algorithm */
CVAPI(void) cvPyrMeanShiftFiltering(const img_t* src, img_t* dst, 
    double sp, double sr, int max_level CC_DEFAULT(1),
    CTermCriteria termcrit CC_DEFAULT(cTermCriteria(CC_TERMCRIT_ITER+CC_TERMCRIT_EPS,5,1)));

/* Segments image using seed "markers" */
CVAPI(void) cvWatershed(const img_t* image, img_t* markers);

#define CC_INPAINT_NS      0
#define CC_INPAINT_TELEA   1

/* Inpaints the selected region in the image */
CVAPI(void) cvInpaint(const img_t* src, const img_t* inpaint_mask,
                       img_t* dst, double inpaintRange, int flags);

#define CC_SCHARR -1
#define CC_MAX_SOBEL_KSIZE 7

/* Calculates an image derivative using generalized Sobel
   (aperture_size = 1,3,5,7) or Scharr (aperture_size = -1) operator.
   Scharr can be used only for the first dx or dy derivative */
CVAPI(void) cvSobel(const img_t* src, img_t* dst,
                    int xorder, int yorder,
                    int aperture_size CC_DEFAULT(3));

/* Calculates the image Laplacian: (d2/dx + d2/dy)I */
CVAPI(void) cvLaplace(const img_t* src, img_t* dst,
                      int aperture_size CC_DEFAULT(3));

/* Constants for color conversion */
#define  CC_BGR2BGRA    0
#define  CC_RGB2RGBA    CC_BGR2BGRA

#define  CC_BGRA2BGR    1
#define  CC_RGBA2RGB    CC_BGRA2BGR

#define  CC_BGR2RGBA    2
#define  CC_RGB2BGRA    CC_BGR2RGBA

#define  CC_RGBA2BGR    3
#define  CC_BGRA2RGB    CC_RGBA2BGR

#define  CC_BGR2RGB     4
#define  CC_RGB2BGR     CC_BGR2RGB

#define  CC_BGRA2RGBA   5
#define  CC_RGBA2BGRA   CC_BGRA2RGBA

#define  CC_BGR2GRAY    6
#define  CC_RGB2GRAY    7
#define  CC_GRAY2BGR    8
#define  CC_GRAY2RGB    CC_GRAY2BGR
#define  CC_GRAY2BGRA   9
#define  CC_GRAY2RGBA   CC_GRAY2BGRA
#define  CC_BGRA2GRAY   10
#define  CC_RGBA2GRAY   11

#define  CC_BGR2BGR565  12
#define  CC_RGB2BGR565  13
#define  CC_BGR5652BGR  14
#define  CC_BGR5652RGB  15
#define  CC_BGRA2BGR565 16
#define  CC_RGBA2BGR565 17
#define  CC_BGR5652BGRA 18
#define  CC_BGR5652RGBA 19

#define  CC_GRAY2BGR565 20
#define  CC_BGR5652GRAY 21

#define  CC_BGR2BGR555  22
#define  CC_RGB2BGR555  23
#define  CC_BGR5552BGR  24
#define  CC_BGR5552RGB  25
#define  CC_BGRA2BGR555 26
#define  CC_RGBA2BGR555 27
#define  CC_BGR5552BGRA 28
#define  CC_BGR5552RGBA 29

#define  CC_GRAY2BGR555 30
#define  CC_BGR5552GRAY 31

#define  CC_BGR2XYZ     32
#define  CC_RGB2XYZ     33
#define  CC_XYZ2BGR     34
#define  CC_XYZ2RGB     35

#define  CC_BGR2YCrCb   36
#define  CC_RGB2YCrCb   37
#define  CC_YCrCb2BGR   38
#define  CC_YCrCb2RGB   39

#define  CC_BGR2HSV     40
#define  CC_RGB2HSV     41

#define  CC_BGR2Lab     44
#define  CC_RGB2Lab     45

#define  CC_BayerBG2BGR 46
#define  CC_BayerGB2BGR 47
#define  CC_BayerRG2BGR 48
#define  CC_BayerGR2BGR 49

#define  CC_BayerBG2RGB CC_BayerRG2BGR
#define  CC_BayerGB2RGB CC_BayerGR2BGR
#define  CC_BayerRG2RGB CC_BayerBG2BGR
#define  CC_BayerGR2RGB CC_BayerGB2BGR

#define  CC_BGR2Luv     50
#define  CC_RGB2Luv     51
#define  CC_BGR2HLS     52
#define  CC_RGB2HLS     53

#define  CC_HSV2BGR     54
#define  CC_HSV2RGB     55

#define  CC_Lab2BGR     56
#define  CC_Lab2RGB     57
#define  CC_Luv2BGR     58
#define  CC_Luv2RGB     59
#define  CC_HLS2BGR     60
#define  CC_HLS2RGB     61

#define  CC_COLORCVT_MAX  100

/* Converts input array pixels from one color space to another */
CVAPI(void)  cvCvtColor(const img_t* src, img_t* dst, int code);

#define  CC_INTER_NN        0
#define  CC_INTER_LINEAR    1
#define  CC_INTER_CUBIC     2
#define  CC_INTER_AREA      3

#define  CC_WARP_FILL_OUTLIERS 8
#define  CC_WARP_INVERSE_MAP  16

/* Resizes image (input array is resized to fit the destination array) */
CVAPI(void)  cvResize(const img_t* src, img_t* dst,
                       int interpolation CC_DEFAULT(CC_INTER_LINEAR));

/* Warps image with affine transform */ 
CVAPI(void)  cvWarpAffine(const img_t* src, img_t* dst, const img_t* map_matrix,
                           int flags CC_DEFAULT(CC_INTER_LINEAR+CC_WARP_FILL_OUTLIERS),
                           CScalar fillval CC_DEFAULT(cScalarAll(0)));

/* Computes affine transform matrix for mapping src[i] to dst[i] (i=0,1,2) */
CVAPI(img_t*) cvGetAffineTransform(const CPoint2D32f * src, 
                                    const CPoint2D32f * dst, 
                                    img_t * map_matrix);

/* Computes rotation_matrix matrix */
CVAPI(img_t*)  cv2DRotationMatrix(CPoint2D32f center, double angle,
                                   double scale, img_t* map_matrix);

/* Warps image with perspective (projective) transform */
CVAPI(void)  cvWarpPerspective(const img_t* src, img_t* dst, const img_t* map_matrix,
                                int flags CC_DEFAULT(CC_INTER_LINEAR+CC_WARP_FILL_OUTLIERS),
                                CScalar fillval CC_DEFAULT(cScalarAll(0)));

/* Computes perspective transform matrix for mapping src[i] to dst[i] (i=0,1,2,3) */
CVAPI(img_t*) cvGetPerspectiveTransform(const CPoint2D32f* src,
                                         const CPoint2D32f* dst,
                                         img_t* map_matrix);

/* Performs generic geometric transformation using the specified coordinate maps */
CVAPI(void)  cvRemap(const img_t* src, img_t* dst,
                      const img_t* mapx, const img_t* mapy,
                      int flags CC_DEFAULT(CC_INTER_LINEAR+CC_WARP_FILL_OUTLIERS),
                      CScalar fillval CC_DEFAULT(cScalarAll(0)));

/* Performs forward or inverse log-polar image transform */
CVAPI(void)  cvLogPolar(const img_t* src, img_t* dst,
                         CPoint2D32f center, double M,
                         int flags CC_DEFAULT(CC_INTER_LINEAR+CC_WARP_FILL_OUTLIERS));

#define  CC_SHAPE_RECT      0
#define  CC_SHAPE_CROSS     1
#define  CC_SHAPE_ELLIPSE   2
#define  CC_SHAPE_CUSTOM    100

/* creates structuring element used for morphological operations */
CVAPI(IplConvKernel*)  cvCreateStructuringElementEx(
            int cols, int  rows, int  anchor_x, int  anchor_y,
            int shape, int* values CC_DEFAULT(NULL));

/* releases structuring element */
CVAPI(void)  cvReleaseStructuringElement(IplConvKernel** element);

/* erodes input image (applies minimum filter) one or more times.
   If element pointer is NULL, 3x3 rectangular element is used */
CVAPI(void)  cvErode(const img_t* src, img_t* dst,
                      IplConvKernel* element CC_DEFAULT(NULL),
                      int iterations CC_DEFAULT(1));

/* dilates input image (applies maximum filter) one or more times.
   If element pointer is NULL, 3x3 rectangular element is used */
CVAPI(void)  cvDilate(const img_t* src, img_t* dst,
                       IplConvKernel* element CC_DEFAULT(NULL),
                       int iterations CC_DEFAULT(1));

#define CC_MOP_OPEN         2
#define CC_MOP_CLOSE        3
#define CC_MOP_GRADIENT     4
#define CC_MOP_TOPHAT       5
#define CC_MOP_BLACKHAT     6

/* Performs complex morphological transformation */
CVAPI(void)  cvMorphologyEx(const img_t* src, img_t* dst,
                             img_t* temp, IplConvKernel* element,
                             int operation, int iterations CC_DEFAULT(1));

/* Calculates all spatial and central moments up to the 3rd order */
CVAPI(void) cvMoments(const img_t* arr, CMoments* moments, int binary CC_DEFAULT(0));

/* Retrieve particular spatial, central or normalized central moments */
CVAPI(double)  cvGetSpatialMoment(CMoments* moments, int x_order, int y_order);
CVAPI(double)  cvGetCentralMoment(CMoments* moments, int x_order, int y_order);
CVAPI(double)  cvGetNormalizedCentralMoment(CMoments* moments,
                                             int x_order, int y_order);

/* Calculates 7 Hu's invariants from precalculated spatial and central moments */
CVAPI(void) cvGetHuMoments(CMoments*  moments, CHuMoments*  hu_moments);

/*********************************** data sampling **************************************/

/* Fetches pixels that belong to the specified line segment and stores them to the buffer.
   Returns the number of retrieved points. */
CVAPI(int)  cvSampleLine(const img_t* image, CPoint pt1, CPoint pt2, void* buffer,
                          int connectivity CC_DEFAULT(8));

/* Retrieves the rectangular image region with specified center from the input array.
 dst(x,y) <- src(x + center.x - dst_width/2, y + center.y - dst_height/2).
 Values of pixels with fractional coordinates are retrieved using bilinear interpolation*/
CVAPI(void)  cvGetRectSubPix(const img_t* src, img_t* dst, CPoint2D32f center);


/* Retrieves quadrangle from the input array.
    matrixarr = (a11  a12 | b1)   dst(x,y) <- src(A[x y]' + b)
                (a21  a22 | b2)   (bilinear interpolation is used to retrieve pixels
                                     with fractional coordinates)
*/
CVAPI(void)  cvGetQuadrangleSubPix(const img_t* src, img_t* dst,
                                    const img_t* map_matrix);

/* Methods for comparing two array */
#define  CC_TM_SQDIFF        0
#define  CC_TM_SQDIFF_NORMED 1
#define  CC_TM_CCORR         2
#define  CC_TM_CCORR_NORMED  3
#define  CC_TM_CCOEFF        4
#define  CC_TM_CCOEFF_NORMED 5

/* Measures similarity between template and overlapped windows in the source image
   and fills the resultant image with the measurements */
CVAPI(void)  cvMatchTemplate(const img_t* image, const img_t* templ,
                              img_t* result, int method);

/* Computes earth mover distance between
   two weighted point sets (called signatures) */
CVAPI(float)  cvCalcEMD2(const img_t* signature1,
                          const img_t* signature2,
                          int distance_type,
                          CvDistanceFunction distance_func CC_DEFAULT(NULL),
                          const img_t* cost_matrix CC_DEFAULT(NULL),
                          img_t* flow CC_DEFAULT(NULL),
                          float* lower_bound CC_DEFAULT(NULL),
                          void* userdata CC_DEFAULT(NULL));

/****************************************************************************************\
*                              Contours retrieving                                       *
\****************************************************************************************/

/* Retrieves outer and optionally inner boundaries of white (non-zero) connected
   components in the black (zero) background */
CVAPI(int)  cvFindContours(img_t* image, CvMemStorage* storage, CvSeq** first_contour,
                            int header_size CC_DEFAULT(sizeof(CvContour)),
                            int mode CC_DEFAULT(CC_RETR_LIST),
                            int method CC_DEFAULT(CC_CHAIN_APPROX_SIMPLE),
                            CPoint offset CC_DEFAULT(cPoint(0,0)));


/* Initalizes contour retrieving process.
   Calls cvStartFindContours.
   Calls cvFindNextContour until null pointer is returned
   or some other condition becomes true.
   Calls cvEndFindContours at the end. */
CVAPI(CvContourScanner)  cvStartFindContours(img_t* image, CvMemStorage* storage,
                            int header_size CC_DEFAULT(sizeof(CvContour)),
                            int mode CC_DEFAULT(CC_RETR_LIST),
                            int method CC_DEFAULT(CC_CHAIN_APPROX_SIMPLE),
                            CPoint offset CC_DEFAULT(cPoint(0,0)));

/* Retrieves next contour */
CVAPI(CvSeq*)  cvFindNextContour(CvContourScanner scanner);


/* Substitutes the last retrieved contour with the new one
   (if the substitutor is null, the last retrieved contour is removed from the tree) */
CVAPI(void)   cvSubstituteContour(CvContourScanner scanner, CvSeq* new_contour);


/* Releases contour scanner and returns pointer to the first outer contour */
CVAPI(CvSeq*)  cvEndFindContours(CvContourScanner* scanner);

/* Approximates a single Freeman chain or a tree of chains to polygonal curves */
CVAPI(CvSeq*) cvApproxChains(CvSeq* src_seq, CvMemStorage* storage,
                            int method CC_DEFAULT(CC_CHAIN_APPROX_SIMPLE),
                            double parameter CC_DEFAULT(0),
                            int  minimal_perimeter CC_DEFAULT(0),
                            int  recursive CC_DEFAULT(0));


/* Initalizes Freeman chain reader.
   The reader is used to iteratively get coordinates of all the chain points.
   If the Freeman codes should be read as is, a simple sequence reader should be used */
CVAPI(void) cvStartReadChainPoints(CChain* chain, CvChainPtReader* reader);

/* Retrieves the next chain point */
CVAPI(CPoint) cvReadChainPoint(CvChainPtReader* reader);


/****************************************************************************************\
*                                  Motion Analysis                                       *
\****************************************************************************************/

/************************************ optical flow ***************************************/

/* Calculates optical flow for 2 images using classical Lucas & Kanade algorithm */
CVAPI(void)  cvCalcOpticalFlowLK(const img_t* prev, const img_t* curr,
                                  CSize win_size, img_t* velx, img_t* vely);

/* Calculates optical flow for 2 images using block matching algorithm */
CVAPI(void)  cvCalcOpticalFlowBM(const img_t* prev, const img_t* curr,
                                  CSize block_size, CSize shift_size,
                                  CSize max_range, int use_previous,
                                  img_t* velx, img_t* vely);

/* Calculates Optical flow for 2 images using Horn & Schunck algorithm */
CVAPI(void)  cvCalcOpticalFlowHS(const img_t* prev, const img_t* curr,
                                  int use_previous, img_t* velx, img_t* vely,
                                  double lambda, CTermCriteria criteria);

#define  CC_LKFLOW_PYR_A_READY       1
#define  CC_LKFLOW_PYR_B_READY       2
#define  CC_LKFLOW_INITIAL_GUESSES   4

/* It is Lucas & Kanade method, modified to use pyramids.
   Also it does several iterations to get optical flow for
   every point at every pyramid level.
   Calculates optical flow between two images for certain set of points (i.e.
   it is a "sparse" optical flow, which is opposite to the previous 3 methods) */
CVAPI(void)  cvCalcOpticalFlowPyrLK(const img_t*  prev, const img_t*  curr,
                                     img_t*  prev_pyr, img_t*  curr_pyr,
                                     const CPoint2D32f* prev_features,
                                     CPoint2D32f* curr_features,
                                     int       count,
                                     CSize    win_size,
                                     int       level,
                                     char*     status,
                                     float*    track_error,
                                     CTermCriteria criteria,
                                     int       flags);


/* Modification of a previous sparse optical flow algorithm to calculate
   affine flow */
/*CVAPI  void  cvCalcAffineFlowPyrLK(const img_t*  prev, const img_t*  curr,
                                     img_t*  prev_pyr, img_t*  curr_pyr,
                                     CPoint2D32f* prev_features,
                                     CPoint2D32f* curr_features,
                                     float*  matrices, int  count,
                                     CSize  win_size, int  level,
                                     char*  status, float* track_error,
                                     CTermCriteria criteria, int flags);*/

/********************************* motion templates *************************************/

/****************************************************************************************\
*        All the motion template functions work only with single channel images.         *
*        Silhouette image must have depth IMG_DEPTH_8U or IMG_DEPTH_8S                   *
*        Motion history image must have depth IMG_DEPTH_32F,                             *
*        Gradient mask - IMG_DEPTH_8U or IMG_DEPTH_8S,                                   *
*        Motion orientation image - IMG_DEPTH_32F                                        *
*        Segmentation mask - IMG_DEPTH_32F                                               *
*        All the angles are in degrees, all the times are in milliseconds                *
\****************************************************************************************/

/* Updates motion history image given motion silhouette */
CVAPI(void)    cvUpdateMotionHistory(const img_t* silhouette, img_t* mhi,
                                      double timestamp, double duration);

/* Calculates gradient of the motion history image and fills
   a mask indicating where the gradient is valid */
CVAPI(void)    cvCalcMotionGradient(const img_t* mhi, img_t* mask, img_t* orientation,
                                     double delta1, double delta2,
                                     int aperture_size CC_DEFAULT(3));

/* Calculates average motion direction within a selected motion region 
   (region can be selected by setting ROIs and/or by composing a valid gradient mask
   with the region mask) */
CVAPI(double)  cvCalcGlobalOrientation(const img_t* orientation, const img_t* mask,
                                        const img_t* mhi, double timestamp,
                                        double duration);

/* Splits a motion history image into a few parts corresponding to separate independent motions
   (e.g. left hand, right hand) */
CVAPI(CvSeq*)  cvSegmentMotion(const img_t* mhi, img_t* seg_mask,
                                CvMemStorage* storage,
                                double timestamp, double seg_thresh);

/*********************** Background statistics accumulation *****************************/

/* Adds image to accumulator */
CVAPI(void)  cvAcc(const img_t* image, img_t* sum,
                    const img_t* mask CC_DEFAULT(NULL));

/* Adds squared image to accumulator */
CVAPI(void)  cvSquareAcc(const img_t* image, img_t* sqsum,
                          const img_t* mask CC_DEFAULT(NULL));

/* Adds a product of two images to accumulator */
CVAPI(void)  cvMultiplyAcc(const img_t* image1, const img_t* image2, img_t* acc,
                            const img_t* mask CC_DEFAULT(NULL));

/* Adds image to accumulator with weights: acc = acc*(1-alpha) + image*alpha */
CVAPI(void)  cvRunningAvg(const img_t* image, img_t* acc, double alpha,
                           const img_t* mask CC_DEFAULT(NULL));


/****************************************************************************************\
*                                       Tracking                                         *
\****************************************************************************************/

/* Implements CAMSHIFT algorithm - determines object position, size and orientation
   from the object histogram back project (extension of meanshift) */
CVAPI(int)  cvCamShift(const img_t* prob_image, CRect  window,
                       CTermCriteria criteria, CConnectedComp* comp,
                       CBox2D* box CC_DEFAULT(NULL));

/* Implements MeanShift algorithm - determines object position
   from the object histogram back project */
CVAPI(int)  cvMeanShift(const img_t* prob_image, CRect  window,
                        CTermCriteria criteria, CConnectedComp* comp);

/* Creates ConDensation filter state */
CVAPI(CvConDensation*)  cvCreateConDensation(int dynam_params,
                                             int measure_params,
                                             int sample_count);

/* Releases ConDensation filter state */
CVAPI(void)  cvReleaseConDensation(CvConDensation** condens);

/* Updates ConDensation filter by time (predict future state of the system) */
CVAPI(void)  cvConDensUpdateByTime(CvConDensation* condens);

/* Initializes ConDensation filter samples  */
CVAPI(void)  cvConDensInitSampleSet(CvConDensation* condens, img_t* lower_bound, img_t* upper_bound);

/* Creates Kalman filter and sets A, B, Q, R and state to some initial values */
CVAPI(CvKalman*) cvCreateKalman(int dynam_params, int measure_params,
                                int control_params CC_DEFAULT(0));

/* Releases Kalman filter state */
CVAPI(void)  cvReleaseKalman(CvKalman** kalman);

/* Updates Kalman filter by time (predicts future state of the system) */
CVAPI(const img_t*)  cvKalmanPredict(CvKalman* kalman,
                                     const img_t* control CC_DEFAULT(NULL));

/* Updates Kalman filter by measurement
   (corrects state of the system and internal matrices) */
CVAPI(const img_t*)  cvKalmanCorrect(CvKalman* kalman, const img_t* measurement);

/****************************************************************************************\
*                              Planar subdivisions                                       *
\****************************************************************************************/

/* Initializes Delaunay triangulation */
CVAPI(void)  cvInitSubdivDelaunay2D(CvSubdiv2D* subdiv, CRect rect);

/* Creates new subdivision */
CVAPI(CvSubdiv2D*)  cvCreateSubdiv2D(int subdiv_type, int header_size,
                                      int vtx_size, int quadedge_size,
                                      CvMemStorage* storage);

/************************* high-level subdivision functions ***************************/

/* Simplified Delaunay diagram creation */
CC_INLINE  CvSubdiv2D* cvCreateSubdivDelaunay2D(CRect rect, CvMemStorage* storage)
{
    CvSubdiv2D* subdiv = cvCreateSubdiv2D(CC_SEQ_KIND_SUBDIV2D, sizeof(*subdiv),
                         sizeof(CvSubdiv2DPoint), sizeof(CvQuadEdge2D), storage);

    cvInitSubdivDelaunay2D(subdiv, rect);
    return subdiv;
}


/* Inserts new point to the Delaunay triangulation */
CVAPI(CvSubdiv2DPoint*)  cvSubdivDelaunay2DInsert(CvSubdiv2D* subdiv, CPoint2D32f pt);

/* Locates a point within the Delaunay triangulation (finds the edge
   the point is left to or belongs to, or the triangulation point the given
   point coinsides with */
CVAPI(CvSubdiv2DPointLocation)  cvSubdiv2DLocate(
                               CvSubdiv2D* subdiv, CPoint2D32f pt,
                               CvSubdiv2DEdge* edge,
                               CvSubdiv2DPoint** vertex CC_DEFAULT(NULL));

/* Calculates Voronoi tesselation (i.e. coordinates of Voronoi points) */
CVAPI(void)  cvCalcSubdivVoronoi2D(CvSubdiv2D* subdiv);


/* Removes all Voronoi points from the tesselation */
CVAPI(void)  cvClearSubdivVoronoi2D(CvSubdiv2D* subdiv);


/* Finds the nearest to the given point vertex in subdivision. */
CVAPI(CvSubdiv2DPoint*) cvFindNearestPoint2D(CvSubdiv2D* subdiv, CPoint2D32f pt);


/************ Basic quad-edge navigation and operations ************/

CC_INLINE  CvSubdiv2DEdge  cvSubdiv2DNextEdge(CvSubdiv2DEdge edge)
{
    return  CC_SUBDIV2D_NEXT_EDGE(edge);
}


CC_INLINE  CvSubdiv2DEdge  cvSubdiv2DRotateEdge(CvSubdiv2DEdge edge, int rotate)
{
    return  (edge & ~3) + ((edge + rotate) & 3);
}

CC_INLINE  CvSubdiv2DEdge  cvSubdiv2DSymEdge(CvSubdiv2DEdge edge)
{
    return edge ^ 2;
}

CC_INLINE  CvSubdiv2DEdge  cvSubdiv2DGetEdge(CvSubdiv2DEdge edge, CvNextEdgeType type)
{
    CvQuadEdge2D* e = (CvQuadEdge2D*)(edge & ~3);
    edge = e->next[(edge + (int)type) & 3];
    return  (edge & ~3) + ((edge + ((int)type >> 4)) & 3);
}


CC_INLINE  CvSubdiv2DPoint*  cvSubdiv2DEdgeOrg(CvSubdiv2DEdge edge)
{
    CvQuadEdge2D* e = (CvQuadEdge2D*)(edge & ~3);
    return (CvSubdiv2DPoint*)e->pt[edge & 3];
}


CC_INLINE  CvSubdiv2DPoint*  cvSubdiv2DEdgeDst(CvSubdiv2DEdge edge)
{
    CvQuadEdge2D* e = (CvQuadEdge2D*)(edge & ~3);
    return (CvSubdiv2DPoint*)e->pt[(edge + 2) & 3];
}


CC_INLINE  double  cvTriangleArea(CPoint2D32f a, CPoint2D32f b, CPoint2D32f c)
{
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}


/****************************************************************************************\
*                            Contour Processing and Shape Analysis                       *
\****************************************************************************************/

#define CC_POLY_APPROX_DP 0

/* Approximates a single polygonal curve (contour) or
   a tree of polygonal curves (contours) */
CVAPI(CvSeq*)  cvApproxPoly(const void* src_seq,
                             int header_size, CvMemStorage* storage,
                             int method, double parameter,
                             int parameter2 CC_DEFAULT(0));

#define CC_DOMINANT_IPAN 1

/* Finds high-curvature points of the contour */
CVAPI(CvSeq*) cvFindDominantPoints(CvSeq* contour, CvMemStorage* storage,
                                   int method CC_DEFAULT(CC_DOMINANT_IPAN),
                                   double parameter1 CC_DEFAULT(0),
                                   double parameter2 CC_DEFAULT(0),
                                   double parameter3 CC_DEFAULT(0),
                                   double parameter4 CC_DEFAULT(0));

/* Calculates perimeter of a contour or length of a part of contour */
CVAPI(double)  cvArcLength(const void* curve,
                            CSlice slice CC_DEFAULT(CC_WHOLE_SEQ),
                            int is_closed CC_DEFAULT(-1));
#define cvContourPerimeter(contour) cvArcLength(contour, CC_WHOLE_SEQ, 1)

/* Calculates contour boundning rectangle (update=1) or
   just retrieves pre-calculated rectangle (update=0) */
CVAPI(CRect)  cvBoundingRect(img_t* points, int update CC_DEFAULT(0));

/* Calculates area of a contour or contour segment */
CVAPI(double)  cvContourArea(const img_t* contour,
                              CSlice slice CC_DEFAULT(CC_WHOLE_SEQ));

/* Finds minimum area rotated rectangle bounding a set of points */
CVAPI(CBox2D)  cvMinAreaRect2(const img_t* points,
                                CvMemStorage* storage CC_DEFAULT(NULL));

/* Finds minimum enclosing circle for a set of points */
CVAPI(int)  cvMinEnclosingCircle(const img_t* points,
                                  CPoint2D32f* center, float* radius);

#define CC_CONTOURS_MATCH_I1  1
#define CC_CONTOURS_MATCH_I2  2
#define CC_CONTOURS_MATCH_I3  3

/* Compares two contours by matching their moments */
CVAPI(double)  cvMatchShapes(const void* object1, const void* object2,
                              int method, double parameter CC_DEFAULT(0));

/* Builds hierarhical representation of a contour */
CVAPI(CvContourTree*)  cvCreateContourTree(const CvSeq* contour,
                                            CvMemStorage* storage,
                                            double threshold);

/* Reconstruct (completelly or partially) contour a from contour tree */
CVAPI(CvSeq*)  cvContourFromContourTree(const CvContourTree* tree,
                                         CvMemStorage* storage,
                                         CTermCriteria criteria);

/* Compares two contour trees */
#define  CC_CONTOUR_TREES_MATCH_I1  1

CVAPI(double)  cvMatchContourTrees(const CvContourTree* tree1,
                                    const CvContourTree* tree2,
                                    int method, double threshold);

/* Calculates histogram of a contour */
CVAPI(void)  cvCalcPGH(const CvSeq* contour, CvHistogram* hist);

#define CC_CLOCKWISE         1
#define CC_COUNTER_CLOCKWISE 2

/* Calculates exact convex hull of 2d point set */
CVAPI(CvSeq*) cvConvexHull2(const img_t* input,
                             void* hull_storage CC_DEFAULT(NULL),
                             int orientation CC_DEFAULT(CC_CLOCKWISE),
                             int return_points CC_DEFAULT(0));

/* Checks whether the contour is convex or not (returns 1 if convex, 0 if not) */
CVAPI(int)  cvCheckContourConvexity(const img_t* contour);

/* Finds convexity defects for the contour */
CVAPI(CvSeq*)  cvConvexityDefects(const img_t* contour, const img_t* convexhull,
                                   CvMemStorage* storage CC_DEFAULT(NULL));

/* Fits ellipse into a set of 2d points */
CVAPI(CBox2D) cvFitEllipse2(const img_t* points);

/* Finds minimum rectangle containing two given rectangles */
CVAPI(CRect)  cvMaxRect(const CRect* rect1, const CRect* rect2);

/* Finds coordinates of the box vertices */
CVAPI(void) cvBoxPoints(CBox2D box, CPoint2D32f pt[4]);

/* Initializes sequence header for a matrix (column or row vector) of points -
   a wrapper for cvMakeSeqHeaderForArray (it does not initialize bounding rectangle!!!) */
CVAPI(CvSeq*) cvPointSeqFromMat(int seq_kind, const img_t* mat,
                                 CvContour* contour_header,
                                 CvSeqBlock* block);

/* Checks whether the point is inside polygon, outside, on an edge (at a vertex).
   Returns positive, negative or zero value, correspondingly.
   Optionally, measures a signed distance between
   the point and the nearest polygon edge (measure_dist=1) */
CVAPI(double) cvPointPolygonTest(const img_t* contour,
                                  CPoint2D32f pt, int measure_dist);

/****************************************************************************************\
*                                  Histogram functions                                   *
\****************************************************************************************/

/* Creates new histogram */
CVAPI(CvHistogram*)  cvCreateHist(int dims, int* sizes, int type,
                                   float** ranges CC_DEFAULT(NULL),
                                   int uniform CC_DEFAULT(1));

/* Assignes histogram bin ranges */
CVAPI(void)  cvSetHistBinRanges(CvHistogram* hist, float** ranges,
                                int uniform CC_DEFAULT(1));

/* Creates histogram header for array */
CVAPI(CvHistogram*)  cvMakeHistHeaderForArray(
                            int  dims, int* sizes, CvHistogram* hist,
                            float* data, float** ranges CC_DEFAULT(NULL),
                            int uniform CC_DEFAULT(1));

/* Releases histogram */
CVAPI(void)  cvReleaseHist(CvHistogram** hist);

/* Clears all the histogram bins */
CVAPI(void)  cvClearHist(CvHistogram* hist);

/* Finds indices and values of minimum and maximum histogram bins */
CVAPI(void)  cvGetMinMaxHistValue(const CvHistogram* hist,
                                   float* min_value, float* max_value,
                                   int* min_idx CC_DEFAULT(NULL),
                                   int* max_idx CC_DEFAULT(NULL));


/* Normalizes histogram by dividing all bins by sum of the bins, multiplied by <factor>.
   After that sum of histogram bins is equal to <factor> */
CVAPI(void)  cvNormalizeHist(CvHistogram* hist, double factor);


/* Clear all histogram bins that are below the threshold */
CVAPI(void)  cvThreshHist(CvHistogram* hist, double threshold);

#define CC_COMP_CORREL        0
#define CC_COMP_CHISQR        1
#define CC_COMP_INTERSECT     2
#define CC_COMP_BHATTACHARYYA 3

/* Compares two histogram */
CVAPI(double)  cvCompareHist(const CvHistogram* hist1,
                              const CvHistogram* hist2,
                              int method);

/* Copies one histogram to another. Destination histogram is created if
   the destination pointer is NULL */
CVAPI(void)  cvCopyHist(const CvHistogram* src, CvHistogram** dst);


/* Calculates bayesian probabilistic histograms
   (each or src and dst is an array of <number> histograms */
CVAPI(void)  cvCalcBayesianProb(CvHistogram** src, int number,
                                CvHistogram** dst);

/* Calculates array histogram */
CVAPI(void)  cvCalcArrHist(img_t** arr, CvHistogram* hist,
                            int accumulate CC_DEFAULT(0),
                            const img_t* mask CC_DEFAULT(NULL));

CC_INLINE  void  cvCalcHist(img_t** image, CvHistogram* hist,
                             int accumulate CC_DEFAULT(0),
                             const img_t* mask CC_DEFAULT(NULL))
{
    cvCalcArrHist((img_t**)image, hist, accumulate, mask);
}

/* Calculates back project */
CVAPI(void)  cvCalcArrBackProject(img_t** image, img_t* dst,
                                   const CvHistogram* hist);
#define  cvCalcBackProject(image, dst, hist) cvCalcArrBackProject((img_t**)image, dst, hist)


/* Does some sort of template matching but compares histograms of
   template and each window location */
CVAPI(void)  cvCalcArrBackProjectPatch(img_t** image, img_t* dst, CSize range,
                                        CvHistogram* hist, int method,
                                        double factor);
#define  cvCalcBackProjectPatch(image, dst, range, hist, method, factor) \
     cvCalcArrBackProjectPatch((img_t**)image, dst, range, hist, method, factor)


/* calculates probabilistic density (divides one histogram by another) */
CVAPI(void)  cvCalcProbDensity(const CvHistogram* hist1, const CvHistogram* hist2,
                                CvHistogram* dst_hist, double scale CC_DEFAULT(255));

/* equalizes histogram of 8-bit single-channel image */
CVAPI(void)  cvEqualizeHist(const img_t* src, img_t* dst);


#define  CC_VALUE  1
#define  CC_ARRAY  2
/* Updates active contour in order to minimize its cummulative
   (internal and external) energy. */
CVAPI(void)  cvSnakeImage(const img_t* image, CPoint* points,
                           int  length, float* alpha,
                           float* beta, float* gamma,
                           int coeff_usage, CSize  win,
                           CTermCriteria criteria, int calc_gradient CC_DEFAULT(1));

/* Calculates the cooficients of the homography matrix */
CVAPI(void)  cvCalcImageHomography(float* line, CPoint3D32f* center,
                                    float* intrinsic, float* homography);

#define CC_DIST_MASK_3   3
#define CC_DIST_MASK_5   5
#define CC_DIST_MASK_PRECISE 0

/* Applies distance transform to binary image */
CVAPI(void)  cvDistTransform(const img_t* src, img_t* dst,
                              int distance_type CC_DEFAULT(CC_DIST_L2),
                              int mask_size CC_DEFAULT(3),
                              const float* mask CC_DEFAULT(NULL),
                              img_t* labels CC_DEFAULT(NULL));


/* Types of thresholding */
#define CC_THRESH_BINARY      0  /* value = value > threshold ? max_value : 0       */
#define CC_THRESH_BINARY_INV  1  /* value = value > threshold ? 0 : max_value       */
#define CC_THRESH_TRUNC       2  /* value = value > threshold ? threshold : value   */
#define CC_THRESH_TOZERO      3  /* value = value > threshold ? value : 0           */
#define CC_THRESH_TOZERO_INV  4  /* value = value > threshold ? 0 : value           */
#define CC_THRESH_MASK        7

#define CC_THRESH_OTSU        8  /* use Otsu algorithm to choose the optimal threshold value;
                                    combine the flag with one of the above CC_THRESH_* values */

/* Applies fixed-level threshold to grayscale image.
   This is a basic operation applied before retrieving contours */
CVAPI(void)  cvThreshold(const img_t*  src, img_t*  dst,
                          double  threshold, double  max_value,
                          int threshold_type);

#define CC_ADAPTIVE_THRESH_MEAN_C  0
#define CC_ADAPTIVE_THRESH_GAUSSIAN_C  1

/* Applies adaptive threshold to grayscale image.
   The two parameters for methods CC_ADAPTIVE_THRESH_MEAN_C and
   CC_ADAPTIVE_THRESH_GAUSSIAN_C are:
   neighborhood size (3, 5, 7 etc.),
   and a constant subtracted from mean (...,-3,-2,-1,0,1,2,3,...) */
CVAPI(void)  cvAdaptiveThreshold(const img_t* src, img_t* dst, double max_value,
                                  int adaptive_method CC_DEFAULT(CC_ADAPTIVE_THRESH_MEAN_C),
                                  int threshold_type CC_DEFAULT(CC_THRESH_BINARY),
                                  int block_size CC_DEFAULT(3),
                                  double param1 CC_DEFAULT(5));

#define CC_FLOODFILL_FIXED_RANGE (1 << 16)
#define CC_FLOODFILL_MASK_ONLY   (1 << 17)

/* Fills the connected component until the color difference gets large enough */
CVAPI(void)  cvFloodFill(img_t* image, CPoint seed_point,
                          CScalar new_val, CScalar lo_diff CC_DEFAULT(cScalarAll(0)),
                          CScalar up_diff CC_DEFAULT(cScalarAll(0)),
                          CConnectedComp* comp CC_DEFAULT(NULL),
                          int flags CC_DEFAULT(4),
                          img_t* mask CC_DEFAULT(NULL));

/****************************************************************************************\
*                                  Feature detection                                     *
\****************************************************************************************/

#define CC_CANNY_L2_GRADIENT  (1 << 31)

/* Runs canny edge detector */
CVAPI(void)  cvCanny(const img_t* image, img_t* edges, double threshold1,
                      double threshold2, int  aperture_size CC_DEFAULT(3));

/* Calculates constraint image for corner detection
   Dx^2 * Dyy + Dxx * Dy^2 - 2 * Dx * Dy * Dxy.
   Applying threshold to the result gives coordinates of corners */
CVAPI(void) cvPreCornerDetect(const img_t* image, img_t* corners,
                              int aperture_size CC_DEFAULT(3));

/* Calculates eigen values and vectors of 2x2
   gradient covariation matrix at every image pixel */
CVAPI(void)  cvCornerEigenValsAndVecs(const img_t* image, img_t* eigenvv,
                                      int block_size, int aperture_size CC_DEFAULT(3));

/* Calculates minimal eigenvalue for 2x2 gradient covariation matrix at
   every image pixel */
CVAPI(void)  cvCornerMinEigenVal(const img_t* image, img_t* eigenval,
                                 int block_size, int aperture_size CC_DEFAULT(3));

/* Harris corner detector:
   Calculates det(M) - k*(trace(M)^2), where M is 2x2 gradient covariation matrix for each pixel */
CVAPI(void)  cvCornerHarris(const img_t* image, img_t* harris_responce,
                             int block_size, int aperture_size CC_DEFAULT(3),
                             double k CC_DEFAULT(0.04));

/* Adjust corner position using some sort of gradient search */
CVAPI(void)  cvFindCornerSubPix(const img_t* image, CPoint2D32f* corners,
                                 int count, CSize win, CSize zero_zone,
                                 CTermCriteria  criteria);

/* Finds a sparse set of points within the selected region
   that seem to be easy to track */
CVAPI(void)  cvGoodFeaturesToTrack(const img_t* image, img_t* eig_image,
                                   img_t* temp_image, CPoint2D32f* corners,
                                   int* corner_count, double  quality_level,
                                   double  min_distance,
                                   const img_t* mask CC_DEFAULT(NULL),
                                   int block_size CC_DEFAULT(3),
                                   int use_harris CC_DEFAULT(0),
                                   double k CC_DEFAULT(0.04));

#define CC_HOUGH_STANDARD 0
#define CC_HOUGH_PROBABILISTIC 1
#define CC_HOUGH_MULTI_SCALE 2
#define CC_HOUGH_GRADIENT 3

/* Finds lines on binary image using one of several methods.
   line_storage is either memory storage or 1 x <max number of lines> img_t, its
   number of columns is changed by the function.
   method is one of CC_HOUGH_*;
   rho, theta and threshold are used for each of those methods;
   param1 ~ line length, param2 ~ line gap - for probabilistic,
   param1 ~ srn, param2 ~ stn - for multi-scale */
CVAPI(CvSeq*)  cvHoughLines2(img_t* image, void* line_storage, int method, 
                              double rho, double theta, int threshold,
                              double param1 CC_DEFAULT(0), double param2 CC_DEFAULT(0));

/* Finds circles in the image */
CVAPI(CvSeq*) cvHoughCircles(img_t* image, void* circle_storage,
                              int method, double dp, double min_dist,
                              double param1 CC_DEFAULT(100),
                              double param2 CC_DEFAULT(100),
                              int min_radius CC_DEFAULT(0),
                              int max_radius CC_DEFAULT(0));

/* Fits a line into set of 2d or 3d points in a robust way (M-estimator technique) */
CVAPI(void)  cvFitLine(const img_t* points, int dist_type, double param,
                        double reps, double aeps, float* line);

/****************************************************************************************\
*                         Haar-like Object Detection functions                           *
\****************************************************************************************/

/* Loads haar classifier cascade from a directory.
   It is obsolete: convert your cascade to xml and use cvLoad instead */
CVAPI(CvHaarClassifierCascade*) cvLoadHaarClassifierCascade(
                    const char* directory, CSize orig_window_size);

CVAPI(void) cvReleaseHaarClassifierCascade(CvHaarClassifierCascade** cascade);

#define CC_HAAR_DO_CANNY_PRUNING 1
#define CC_HAAR_SCALE_IMAGE      2

CVAPI(CvSeq*) cvHaarDetectObjects(const img_t* image,
                     CvHaarClassifierCascade* cascade,
                     CvMemStorage* storage, double scale_factor CC_DEFAULT(1.1),
                     int min_neighbors CC_DEFAULT(3), int flags CC_DEFAULT(0),
                     CSize min_size CC_DEFAULT(cSize(0,0)));

/* sets images for haar classifier cascade */
CVAPI(void) cvSetImagesForHaarClassifierCascade(CvHaarClassifierCascade* cascade,
                                                const img_t* sum, const img_t* sqsum,
                                                const img_t* tilted_sum, double scale);

/* runs the cascade on the specified window */
CVAPI(int) cvRunHaarClassifierCascade(CvHaarClassifierCascade* cascade,
                                      CPoint pt, int start_stage CC_DEFAULT(0));

/****************************************************************************************\
*                     Camera Calibration and Rectification functions                     *
\****************************************************************************************/

/* transforms the input image to compensate lens distortion */
CVAPI(void) cvUndistort2(const img_t* src, img_t* dst,
                          const img_t* intrinsic_matrix,
                          const img_t* distortion_coeffs);

/* computes transformation map from intrinsic camera parameters
   that can used by cvRemap */
CVAPI(void) cvInitUndistortMap(const img_t* intrinsic_matrix,
                                const img_t* distortion_coeffs,
                                img_t* mapx, img_t* mapy);

/* converts rotation vector to rotation matrix or vice versa */
CVAPI(int) cvRodrigues2(const img_t* src, img_t* dst,
                         img_t* jacobian CC_DEFAULT(0));

/* finds perspective transformation between the object plane and image (view) plane */
CVAPI(void) cvFindHomography(const img_t* src_points,
                              const img_t* dst_points,
                              img_t* homography);

/* projects object points to the view plane using
   the specified extrinsic and intrinsic camera parameters */
CVAPI(void) cvProjectPoints2(const img_t* object_points, const img_t* rotation_vector,
                              const img_t* translation_vector, const img_t* intrinsic_matrix,
                              const img_t* distortion_coeffs, img_t* image_points,
                              img_t* dpdrot CC_DEFAULT(NULL), img_t* dpdt CC_DEFAULT(NULL),
                              img_t* dpdf CC_DEFAULT(NULL), img_t* dpdc CC_DEFAULT(NULL),
                              img_t* dpddist CC_DEFAULT(NULL));

/* finds extrinsic camera parameters from
   a few known corresponding point pairs and intrinsic parameters */
CVAPI(void) cvFindExtrinsicCameraParams2(const img_t* object_points,
                                          const img_t* image_points,
                                          const img_t* intrinsic_matrix,
                                          const img_t* distortion_coeffs,
                                          img_t* rotation_vector,
                                          img_t* translation_vector);

#define CC_CALIB_USE_INTRINSIC_GUESS  1
#define CC_CALIB_FIX_ASPECT_RATIO     2
#define CC_CALIB_FIX_PRINCIPAL_POINT  4
#define CC_CALIB_ZERO_TANGENT_DIST    8

/* finds intrinsic and extrinsic camera parameters
   from a few views of known calibration pattern */
CVAPI(void) cvCalibrateCamera2(const img_t* object_points,
                                const img_t* image_points,
                                const img_t* point_counts,
                                CSize image_size,
                                img_t* intrinsic_matrix,
                                img_t* distortion_coeffs,
                                img_t* rotation_vectors CC_DEFAULT(NULL),
                                img_t* translation_vectors CC_DEFAULT(NULL),
                                int flags CC_DEFAULT(0));

#define CC_CALIB_CB_ADAPTIVE_THRESH  1
#define CC_CALIB_CB_NORMALIZE_IMAGE  2
#define CC_CALIB_CB_FILTER_QUADS     4 

/* Detects corners on a chessboard calibration pattern */
CVAPI(int) cvFindChessboardCorners(const void* image, CSize pattern_size,
                                    CPoint2D32f* corners,
                                    int* corner_count CC_DEFAULT(NULL),
                                    int flags CC_DEFAULT(CC_CALIB_CB_ADAPTIVE_THRESH));

/* Draws individual chessboard corners or the whole chessboard detected */
CVAPI(void) cvDrawChessboardCorners(img_t* image, CSize pattern_size,
                                     CPoint2D32f* corners,
                                     int count, int pattern_was_found);

typedef struct CvPOSITObject CvPOSITObject;

/* Allocates and initializes CvPOSITObject structure before doing cvPOSIT */
CVAPI(CvPOSITObject*)  cvCreatePOSITObject(CPoint3D32f* points, int point_count);


/* Runs POSIT (POSe from ITeration) algorithm for determining 3d position of
   an object given its model and projection in a weak-perspective case */
CVAPI(void)  cvPOSIT(CvPOSITObject* posit_object, CPoint2D32f* image_points,
                       double focal_length, CTermCriteria criteria,
                       CvMatr32f rotation_matrix, CvVect32f translation_vector);

/* Releases CvPOSITObject structure */
CVAPI(void)  cvReleasePOSITObject(CvPOSITObject**  posit_object);


/****************************************************************************************\
*                                 Epipolar Geometry                                      *
\****************************************************************************************/

CVAPI(void) cvConvertPointsHomogenious(const img_t* src, img_t* dst);

/* Calculates fundamental matrix given a set of corresponding points */
#define CC_FM_7POINT 1
#define CC_FM_8POINT 2
#define CC_FM_LMEDS_ONLY  4
#define CC_FM_RANSAC_ONLY 8
#define CC_FM_LMEDS (CC_FM_LMEDS_ONLY + CC_FM_8POINT)
#define CC_FM_RANSAC (CC_FM_RANSAC_ONLY + CC_FM_8POINT)
CVAPI(int) cvFindFundamentalMat(const img_t* points1, const img_t* points2,
                                 img_t* fundamental_matrix,
                                 int method CC_DEFAULT(CC_FM_RANSAC),
                                 double param1 CC_DEFAULT(1.), double param2 CC_DEFAULT(0.99),
                                 img_t* status CC_DEFAULT(NULL));

/* For each input point on one of images
   computes parameters of the corresponding
   epipolar line on the other image */
CVAPI(void) cvComputeCorrespondEpilines(const img_t* points,
                                         int which_image,
                                         const img_t* fundamental_matrix,
                                         img_t* correspondent_lines);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include "cv.hpp"
#endif

/****************************************************************************************\
*                                 Backward compatibility                                 *
\****************************************************************************************/

#ifndef CC_NO_BACKWARD_COMPATIBILITY
#include "cvcompat.h"
#endif

#endif /*_CC_H_*/
