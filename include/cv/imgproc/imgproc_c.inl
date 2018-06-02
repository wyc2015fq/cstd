

#ifndef OPENCC_IMGPROC_IMGPROC_C_H
#define OPENCC_IMGPROC_IMGPROC_C_H

#include "opencv2/imgproc/types_c.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup imgproc_c
@{
*/

/*********************** Background statistics accumulation *****************************/

/** @brief Adds image to accumulator
@see accumulate
*/
CVAPI(void)  cvAcc( const img_t* image, img_t* sum,
                   const img_t* mask CC_DEFAULT(NULL) );

/** @brief Adds squared image to accumulator
@see accumulateSquare
*/
CVAPI(void)  cvSquareAcc( const img_t* image, img_t* sqsum,
                         const img_t* mask CC_DEFAULT(NULL) );

/** @brief Adds a product of two images to accumulator
@see accumulateProduct
*/
CVAPI(void)  cvMultiplyAcc( const img_t* image1, const img_t* image2, img_t* acc,
                           const img_t* mask CC_DEFAULT(NULL) );

/** @brief Adds image to accumulator with weights: acc = acc*(1-alpha) + image*alpha
@see accumulateWeighted
*/
CVAPI(void)  cvRunningAvg( const img_t* image, img_t* acc, double alpha,
                          const img_t* mask CC_DEFAULT(NULL) );

/****************************************************************************************\
*                                    Image Processing                                    *
\****************************************************************************************/

/** Copies source 2D array inside of the larger destination array and
   makes a border of the specified type (IMG_BORDER_*) around the copied area. */
CVAPI(void) cvCopyMakeBorder( const img_t* src, img_t* dst, CPoint offset,
                              int bordertype, CScalar value CC_DEFAULT(cScalarAll(0)));

/** @brief Smooths the image in one of several ways.

@param src The source image
@param dst The destination image
@param smoothtype Type of the smoothing, see SmoothMethod_c
@param size1 The first parameter of the smoothing operation, the aperture width. Must be a
positive odd number (1, 3, 5, ...)
@param size2 The second parameter of the smoothing operation, the aperture height. Ignored by
CC_MEDIAN and CC_BILATERAL methods. In the case of simple scaled/non-scaled and Gaussian blur if
size2 is zero, it is set to size1. Otherwise it must be a positive odd number.
@param sigma1 In the case of a Gaussian parameter this parameter may specify Gaussian \f$\sigma\f$
(standard deviation). If it is zero, it is calculated from the kernel size:
\f[\sigma  = 0.3 (n/2 - 1) + 0.8  \quad   \text{where}   \quad  n= \begin{array}{l l} \mbox{\texttt{size1} for horizontal kernel} \\ \mbox{\texttt{size2} for vertical kernel} \end{array}\f]
Using standard sigma for small kernels ( \f$3\times 3\f$ to \f$7\times 7\f$ ) gives better speed. If
sigma1 is not zero, while size1 and size2 are zeros, the kernel size is calculated from the
sigma (to provide accurate enough operation).
@param sigma2 additional parameter for bilateral filtering

@see GaussianBlur, blur, medianBlur, bilateralFilter.
 */
CVAPI(void) cvSmooth( const img_t* src, img_t* dst,
                      int smoothtype CC_DEFAULT(CC_GAUSSIAN),
                      int size1 CC_DEFAULT(3),
                      int size2 CC_DEFAULT(0),
                      double sigma1 CC_DEFAULT(0),
                      double sigma2 CC_DEFAULT(0));

/** @brief Convolves an image with the kernel.

@param src input image.
@param dst output image of the same size and the same number of channels as src.
@param kernel convolution kernel (or rather a correlation kernel), a single-channel floating point
matrix; if you want to apply different kernels to different channels, split the image into
separate color planes using split and process them individually.
@param anchor anchor of the kernel that indicates the relative position of a filtered point within
the kernel; the anchor should lie within the kernel; default value (-1,-1) means that the anchor
is at the kernel center.

@see filter2D
 */
CVAPI(void) cvFilter2D( const img_t* src, img_t* dst, const img_t* kernel,
                        CPoint anchor CC_DEFAULT(cPoint(-1,-1)));

/** @brief Finds integral image: SUM(X,Y) = sum(x<X,y<Y)I(x,y)
@see integral
*/
CVAPI(void) cvIntegral( const img_t* image, img_t* sum,
                       img_t* sqsum CC_DEFAULT(NULL),
                       img_t* tilted_sum CC_DEFAULT(NULL));

/** @brief Smoothes the input image with gaussian kernel and then down-samples it.

   dst_width = floor(src_width/2)[+1],
   dst_height = floor(src_height/2)[+1]
   @see pyrDown
*/
CVAPI(void)  cvPyrDown( const img_t* src, img_t* dst,
                        int filter CC_DEFAULT(CC_GAUSSIAN_5x5) );

/** @brief Up-samples image and smoothes the result with gaussian kernel.

   dst_width = src_width*2,
   dst_height = src_height*2
   @see pyrUp
*/
CVAPI(void)  cvPyrUp( const img_t* src, img_t* dst,
                      int filter CC_DEFAULT(CC_GAUSSIAN_5x5) );

/** @brief Builds pyramid for an image
@see buildPyramid
*/
CVAPI(img_t**) cvCreatePyramid( const img_t* img, int extra_layers, double rate,
                                const CSize* layer_sizes CC_DEFAULT(0),
                                img_t* bufarr CC_DEFAULT(0),
                                int calc CC_DEFAULT(1),
                                int filter CC_DEFAULT(CC_GAUSSIAN_5x5) );

/** @brief Releases pyramid */
CVAPI(void)  cvReleasePyramid( img_t*** pyramid, int extra_layers );


/** @brief Filters image using meanshift algorithm
@see pyrMeanShiftFiltering
*/
CVAPI(void) cvPyrMeanShiftFiltering( const img_t* src, img_t* dst,
    double sp, double sr, int max_level CC_DEFAULT(1),
    CTermCriteria termcrit CC_DEFAULT(cTermCriteria(CC_TERMCRIT_ITER+CC_TERMCRIT_EPS,5,1)));

/** @brief Segments image using seed "markers"
@see watershed
*/
CVAPI(void) cvWatershed( const img_t* image, img_t* markers );

/** @brief Calculates an image derivative using generalized Sobel

   (aperture_size = 1,3,5,7) or Scharr (aperture_size = -1) operator.
   Scharr can be used only for the first dx or dy derivative
@see Sobel
*/
CVAPI(void) cvSobel( const img_t* src, img_t* dst,
                    int xorder, int yorder,
                    int aperture_size CC_DEFAULT(3));

/** @brief Calculates the image Laplacian: (d2/dx + d2/dy)I
@see Laplacian
*/
CVAPI(void) cvLaplace( const img_t* src, img_t* dst,
                      int aperture_size CC_DEFAULT(3) );

/** @brief Converts input array pixels from one color space to another
@see cvtColor
*/
CVAPI(void)  cvCvtColor( const img_t* src, img_t* dst, int code );


/** @brief Resizes image (input array is resized to fit the destination array)
@see resize
*/
CVAPI(void)  cvResize( const img_t* src, img_t* dst,
                       int interpolation CC_DEFAULT( CC_INTER_LINEAR ));

/** @brief Warps image with affine transform
@note ::cvGetQuadrangleSubPix is similar to ::cvWarpAffine, but the outliers are extrapolated using
replication border mode.
@see warpAffine
*/
CVAPI(void)  cvWarpAffine( const img_t* src, img_t* dst, const img_t* map_matrix,
                           int flags CC_DEFAULT(CC_INTER_LINEAR+CC_WARP_FILL_OUTLIERS),
                           CScalar fillval CC_DEFAULT(cScalarAll(0)) );

/** @brief Computes affine transform matrix for mapping src[i] to dst[i] (i=0,1,2)
@see getAffineTransform
*/
CVAPI(img_t*) cvGetAffineTransform( const CPoint2D32f * src,
                                    const CPoint2D32f * dst,
                                    img_t * map_matrix );

/** @brief Computes rotation_matrix matrix
@see getRotationMatrix2D
*/
CVAPI(img_t*)  cv2DRotationMatrix( CPoint2D32f center, double angle,
                                   double scale, img_t* map_matrix );

/** @brief Warps image with perspective (projective) transform
@see warpPerspective
*/
CVAPI(void)  cvWarpPerspective( const img_t* src, img_t* dst, const img_t* map_matrix,
                                int flags CC_DEFAULT(CC_INTER_LINEAR+CC_WARP_FILL_OUTLIERS),
                                CScalar fillval CC_DEFAULT(cScalarAll(0)) );

/** @brief Computes perspective transform matrix for mapping src[i] to dst[i] (i=0,1,2,3)
@see getPerspectiveTransform
*/
CVAPI(img_t*) cvGetPerspectiveTransform( const CPoint2D32f* src,
                                         const CPoint2D32f* dst,
                                         img_t* map_matrix );

/** @brief Performs generic geometric transformation using the specified coordinate maps
@see remap
*/
CVAPI(void)  cvRemap( const img_t* src, img_t* dst,
                      const img_t* mapx, const img_t* mapy,
                      int flags CC_DEFAULT(CC_INTER_LINEAR+CC_WARP_FILL_OUTLIERS),
                      CScalar fillval CC_DEFAULT(cScalarAll(0)) );

/** @brief Converts mapx & mapy from floating-point to integer formats for cvRemap
@see convertMaps
*/
CVAPI(void)  cvConvertMaps( const img_t* mapx, const img_t* mapy,
                            img_t* mapxy, img_t* mapalpha );

/** @brief Performs forward or inverse log-polar image transform
@see logPolar
*/
CVAPI(void)  cvLogPolar( const img_t* src, img_t* dst,
                         CPoint2D32f center, double M,
                         int flags CC_DEFAULT(CC_INTER_LINEAR+CC_WARP_FILL_OUTLIERS));

/** Performs forward or inverse linear-polar image transform
@see linearPolar
*/
CVAPI(void)  cvLinearPolar( const img_t* src, img_t* dst,
                         CPoint2D32f center, double maxRadius,
                         int flags CC_DEFAULT(CC_INTER_LINEAR+CC_WARP_FILL_OUTLIERS));

/** @brief Transforms the input image to compensate lens distortion
@see undistort
*/
CVAPI(void) cvUndistort2( const img_t* src, img_t* dst,
                          const img_t* camera_matrix,
                          const img_t* distortion_coeffs,
                          const img_t* new_camera_matrix CC_DEFAULT(0) );

/** @brief Computes transformation map from intrinsic camera parameters
   that can used by cvRemap
*/
CVAPI(void) cvInitUndistortMap( const img_t* camera_matrix,
                                const img_t* distortion_coeffs,
                                img_t* mapx, img_t* mapy );

/** @brief Computes undistortion+rectification map for a head of stereo camera
@see initUndistortRectifyMap
*/
CVAPI(void) cvInitUndistortRectifyMap( const img_t* camera_matrix,
                                       const img_t* dist_coeffs,
                                       const img_t *R, const img_t* new_camera_matrix,
                                       img_t* mapx, img_t* mapy );

/** @brief Computes the original (undistorted) feature coordinates
   from the observed (distorted) coordinates
@see undistortPoints
*/
CVAPI(void) cvUndistortPoints( const img_t* src, img_t* dst,
                               const img_t* camera_matrix,
                               const img_t* dist_coeffs,
                               const img_t* R CC_DEFAULT(0),
                               const img_t* P CC_DEFAULT(0));

/** @brief Returns a structuring element of the specified size and shape for morphological operations.

@note the created structuring element IplConvKernel\* element must be released in the end using
`cvReleaseStructuringElement(&element)`.

@param cols Width of the structuring element
@param rows Height of the structuring element
@param anchor_x x-coordinate of the anchor
@param anchor_y y-coordinate of the anchor
@param shape element shape that could be one of the MorphShapes_c
@param values integer array of cols*rows elements that specifies the custom shape of the
structuring element, when shape=CC_SHAPE_CUSTOM.

@see getStructuringElement
 */
 CVAPI(IplConvKernel*)  cvCreateStructuringElementEx(
            int cols, int  rows, int  anchor_x, int  anchor_y,
            int shape, int* values CC_DEFAULT(NULL) );

/** @brief releases structuring element
@see cvCreateStructuringElementEx
*/
CVAPI(void)  cvReleaseStructuringElement( IplConvKernel** element );

/** @brief erodes input image (applies minimum filter) one or more times.
   If element pointer is NULL, 3x3 rectangular element is used
@see erode
*/
CVAPI(void)  cvErode( const img_t* src, img_t* dst,
                      IplConvKernel* element CC_DEFAULT(NULL),
                      int iterations CC_DEFAULT(1) );

/** @brief dilates input image (applies maximum filter) one or more times.

   If element pointer is NULL, 3x3 rectangular element is used
@see dilate
*/
CVAPI(void)  cvDilate( const img_t* src, img_t* dst,
                       IplConvKernel* element CC_DEFAULT(NULL),
                       int iterations CC_DEFAULT(1) );

/** @brief Performs complex morphological transformation
@see morphologyEx
*/
CVAPI(void)  cvMorphologyEx( const img_t* src, img_t* dst,
                             img_t* temp, IplConvKernel* element,
                             int operation, int iterations CC_DEFAULT(1) );

/** @brief Calculates all spatial and central moments up to the 3rd order
@see moments
*/
CVAPI(void) cvMoments( const img_t* arr, CMoments* moments, int binary CC_DEFAULT(0));

/** @brief Retrieve spatial moments */
CVAPI(double)  cvGetSpatialMoment( CMoments* moments, int x_order, int y_order );
/** @brief Retrieve central moments */
CVAPI(double)  cvGetCentralMoment( CMoments* moments, int x_order, int y_order );
/** @brief Retrieve normalized central moments */
CVAPI(double)  cvGetNormalizedCentralMoment( CMoments* moments,
                                             int x_order, int y_order );

/** @brief Calculates 7 Hu's invariants from precalculated spatial and central moments
@see HuMoments
*/
CVAPI(void) cvGetHuMoments( CMoments*  moments, CHuMoments*  hu_moments );

/*********************************** data sampling **************************************/

/** @brief Fetches pixels that belong to the specified line segment and stores them to the buffer.

   Returns the number of retrieved points.
@see LineSegmentDetector
*/
CVAPI(int)  cvSampleLine( const img_t* image, CPoint pt1, CPoint pt2, void* buffer,
                          int connectivity CC_DEFAULT(8));

/** @brief Retrieves the rectangular image region with specified center from the input array.

 dst(x,y) <- src(x + center.x - dst_width/2, y + center.y - dst_height/2).
 Values of pixels with fractional coordinates are retrieved using bilinear interpolation
@see getRectSubPix
*/
CVAPI(void)  cvGetRectSubPix( const img_t* src, img_t* dst, CPoint2D32f center );


/** @brief Retrieves quadrangle from the input array.

    matrixarr = ( a11  a12 | b1 )   dst(x,y) <- src(A[x y]' + b)
                ( a21  a22 | b2 )   (bilinear interpolation is used to retrieve pixels
                                     with fractional coordinates)
@see cvWarpAffine
*/
CVAPI(void)  cvGetQuadrangleSubPix( const img_t* src, img_t* dst,
                                    const img_t* map_matrix );

/** @brief Measures similarity between template and overlapped windows in the source image
   and fills the resultant image with the measurements
@see matchTemplate
*/
CVAPI(void)  cvMatchTemplate( const img_t* image, const img_t* templ,
                              img_t* result, int method );

/** @brief Computes earth mover distance between
   two weighted point sets (called signatures)
@see EMD
*/
CVAPI(float)  cvCalcEMD2( const img_t* signature1,
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

/** @brief Retrieves outer and optionally inner boundaries of white (non-zero) connected
   components in the black (zero) background
@see findContours, cvStartFindContours, cvFindNextContour, cvSubstituteContour, cvEndFindContours
*/
CVAPI(int)  cvFindContours( img_t* image, CvMemStorage* storage, CvSeq** first_contour,
                            int header_size CC_DEFAULT(sizeof(CvContour)),
                            int mode CC_DEFAULT(CC_RETR_LIST),
                            int method CC_DEFAULT(CC_CHAIN_APPROX_SIMPLE),
                            CPoint offset CC_DEFAULT(cPoint(0,0)));

/** @brief Initializes contour retrieving process.

   Calls cvStartFindContours.
   Calls cvFindNextContour until null pointer is returned
   or some other condition becomes true.
   Calls cvEndFindContours at the end.
@see cvFindContours
*/
CVAPI(CvContourScanner)  cvStartFindContours( img_t* image, CvMemStorage* storage,
                            int header_size CC_DEFAULT(sizeof(CvContour)),
                            int mode CC_DEFAULT(CC_RETR_LIST),
                            int method CC_DEFAULT(CC_CHAIN_APPROX_SIMPLE),
                            CPoint offset CC_DEFAULT(cPoint(0,0)));

/** @brief Retrieves next contour
@see cvFindContours
*/
CVAPI(CvSeq*)  cvFindNextContour( CvContourScanner scanner );


/** @brief Substitutes the last retrieved contour with the new one

   (if the substitutor is null, the last retrieved contour is removed from the tree)
@see cvFindContours
*/
CVAPI(void)   cvSubstituteContour( CvContourScanner scanner, CvSeq* new_contour );


/** @brief Releases contour scanner and returns pointer to the first outer contour
@see cvFindContours
*/
CVAPI(CvSeq*)  cvEndFindContours( CvContourScanner* scanner );

/** @brief Approximates Freeman chain(s) with a polygonal curve.

This is a standalone contour approximation routine, not represented in the new interface. When
cvFindContours retrieves contours as Freeman chains, it calls the function to get approximated
contours, represented as polygons.

@param src_seq Pointer to the approximated Freeman chain that can refer to other chains.
@param storage Storage location for the resulting polylines.
@param method Approximation method (see the description of the function :ocvFindContours ).
@param parameter Method parameter (not used now).
@param minimal_perimeter Approximates only those contours whose perimeters are not less than
minimal_perimeter . Other chains are removed from the resulting structure.
@param recursive Recursion flag. If it is non-zero, the function approximates all chains that can
be obtained from chain by using the h_next or v_next links. Otherwise, the single input chain is
approximated.
@see cvStartReadChainPoints, cvReadChainPoint
 */
CVAPI(CvSeq*) cvApproxChains( CvSeq* src_seq, CvMemStorage* storage,
                            int method CC_DEFAULT(CC_CHAIN_APPROX_SIMPLE),
                            double parameter CC_DEFAULT(0),
                            int  minimal_perimeter CC_DEFAULT(0),
                            int  recursive CC_DEFAULT(0));

/** @brief Initializes Freeman chain reader.

   The reader is used to iteratively get coordinates of all the chain points.
   If the Freeman codes should be read as is, a simple sequence reader should be used
@see cvApproxChains
*/
CVAPI(void) cvStartReadChainPoints( CChain* chain, CvChainPtReader* reader );

/** @brief Retrieves the next chain point
@see cvApproxChains
*/
CVAPI(CPoint) cvReadChainPoint( CvChainPtReader* reader );


/****************************************************************************************\
*                            Contour Processing and Shape Analysis                       *
\****************************************************************************************/

/** @brief Approximates a single polygonal curve (contour) or
   a tree of polygonal curves (contours)
@see approxPolyDP
*/
CVAPI(CvSeq*)  cvApproxPoly( const void* src_seq,
                             int header_size, CvMemStorage* storage,
                             int method, double eps,
                             int recursive CC_DEFAULT(0));

/** @brief Calculates perimeter of a contour or length of a part of contour
@see arcLength
*/
CVAPI(double)  cvArcLength( const void* curve,
                            CSlice slice CC_DEFAULT(CC_WHOLE_SEQ),
                            int is_closed CC_DEFAULT(-1));

/** same as cvArcLength for closed contour
*/
CC_INLINE double cvContourPerimeter( const void* contour )
{
    return cvArcLength( contour, CC_WHOLE_SEQ, 1 );
}


/** @brief Calculates contour bounding rectangle (update=1) or
   just retrieves pre-calculated rectangle (update=0)
@see boundingRect
*/
CVAPI(CRect)  cvBoundingRect( img_t* points, int update CC_DEFAULT(0) );

/** @brief Calculates area of a contour or contour segment
@see contourArea
*/
CVAPI(double)  cvContourArea( const img_t* contour,
                              CSlice slice CC_DEFAULT(CC_WHOLE_SEQ),
                              int oriented CC_DEFAULT(0));

/** @brief Finds minimum area rotated rectangle bounding a set of points
@see minAreaRect
*/
CVAPI(CBox2D)  cvMinAreaRect2( const img_t* points,
                                CvMemStorage* storage CC_DEFAULT(NULL));

/** @brief Finds minimum enclosing circle for a set of points
@see minEnclosingCircle
*/
CVAPI(int)  cvMinEnclosingCircle( const img_t* points,
                                  CPoint2D32f* center, float* radius );

/** @brief Compares two contours by matching their moments
@see matchShapes
*/
CVAPI(double)  cvMatchShapes( const void* object1, const void* object2,
                              int method, double parameter CC_DEFAULT(0));

/** @brief Calculates exact convex hull of 2d point set
@see convexHull
*/
CVAPI(CvSeq*) cvConvexHull2( const img_t* input,
                             void* hull_storage CC_DEFAULT(NULL),
                             int orientation CC_DEFAULT(CC_CLOCKWISE),
                             int return_points CC_DEFAULT(0));

/** @brief Checks whether the contour is convex or not (returns 1 if convex, 0 if not)
@see isContourConvex
*/
CVAPI(int)  cvCheckContourConvexity( const img_t* contour );


/** @brief Finds convexity defects for the contour
@see convexityDefects
*/
CVAPI(CvSeq*)  cvConvexityDefects( const img_t* contour, const img_t* convexhull,
                                   CvMemStorage* storage CC_DEFAULT(NULL));

/** @brief Fits ellipse into a set of 2d points
@see fitEllipse
*/
CVAPI(CBox2D) cvFitEllipse2( const img_t* points );

/** @brief Finds minimum rectangle containing two given rectangles */
CVAPI(CRect)  cvMaxRect( const CRect* rect1, const CRect* rect2 );

/** @brief Finds coordinates of the box vertices */
CVAPI(void) cvBoxPoints( CBox2D box, CPoint2D32f pt[4] );

/** @brief Initializes sequence header for a matrix (column or row vector) of points

   a wrapper for cvMakeSeqHeaderForArray (it does not initialize bounding rectangle!!!) */
CVAPI(CvSeq*) cvPointSeqFromMat( int seq_kind, const img_t* mat,
                                 CvContour* contour_header,
                                 CvSeqBlock* block );

/** @brief Checks whether the point is inside polygon, outside, on an edge (at a vertex).

   Returns positive, negative or zero value, correspondingly.
   Optionally, measures a signed distance between
   the point and the nearest polygon edge (measure_dist=1)
@see pointPolygonTest
*/
CVAPI(double) cvPointPolygonTest( const img_t* contour,
                                  CPoint2D32f pt, int measure_dist );

/****************************************************************************************\
*                                  Histogram functions                                   *
\****************************************************************************************/

/** @brief Creates a histogram.

The function creates a histogram of the specified size and returns a pointer to the created
histogram. If the array ranges is 0, the histogram bin ranges must be specified later via the
function cvSetHistBinRanges. Though cvCalcHist and cvCalcBackProject may process 8-bit images
without setting bin ranges, they assume they are equally spaced in 0 to 255 bins.

@param dims Number of histogram dimensions.
@param sizes Array of the histogram dimension sizes.
@param type Histogram representation format. CC_HIST_ARRAY means that the histogram data is
represented as a multi-dimensional dense array CvMatND. CC_HIST_SPARSE means that histogram data
is represented as a multi-dimensional sparse array CvSparseMat.
@param ranges Array of ranges for the histogram bins. Its meaning depends on the uniform parameter
value. The ranges are used when the histogram is calculated or backprojected to determine which
histogram bin corresponds to which value/tuple of values from the input image(s).
@param uniform Uniformity flag. If not zero, the histogram has evenly spaced bins and for every
\f$0<=i<cDims\f$ ranges[i] is an array of two numbers: lower and upper boundaries for the i-th
histogram dimension. The whole range [lower,upper] is then split into dims[i] equal parts to
determine the i-th input tuple value ranges for every histogram bin. And if uniform=0 , then the
i-th element of the ranges array contains dims[i]+1 elements: \f$\texttt{lower}_0,
\texttt{upper}_0, \texttt{lower}_1, \texttt{upper}_1 = \texttt{lower}_2,
...
\texttt{upper}_{dims[i]-1}\f$ where \f$\texttt{lower}_j\f$ and \f$\texttt{upper}_j\f$ are lower
and upper boundaries of the i-th input tuple value for the j-th bin, respectively. In either
case, the input values that are beyond the specified range for a histogram bin are not counted
by cvCalcHist and filled with 0 by cvCalcBackProject.
 */
CVAPI(CvHistogram*)  cvCreateHist( int dims, int* sizes, int type,
                                   float** ranges CC_DEFAULT(NULL),
                                   int uniform CC_DEFAULT(1));

/** @brief Sets the bounds of the histogram bins.

This is a standalone function for setting bin ranges in the histogram. For a more detailed
description of the parameters ranges and uniform, see the :ocvCalcHist function that can initialize
the ranges as well. Ranges for the histogram bins must be set before the histogram is calculated or
the backproject of the histogram is calculated.

@param hist Histogram.
@param ranges Array of bin ranges arrays. See :ocvCreateHist for details.
@param uniform Uniformity flag. See :ocvCreateHist for details.
 */
CVAPI(void)  cvSetHistBinRanges( CvHistogram* hist, float** ranges,
                                int uniform CC_DEFAULT(1));

/** @brief Makes a histogram out of an array.

The function initializes the histogram, whose header and bins are allocated by the user.
cvReleaseHist does not need to be called afterwards. Only dense histograms can be initialized this
way. The function returns hist.

@param dims Number of the histogram dimensions.
@param sizes Array of the histogram dimension sizes.
@param hist Histogram header initialized by the function.
@param data Array used to store histogram bins.
@param ranges Histogram bin ranges. See cvCreateHist for details.
@param uniform Uniformity flag. See cvCreateHist for details.
 */
CVAPI(CvHistogram*)  cvMakeHistHeaderForArray(
                            int  dims, int* sizes, CvHistogram* hist,
                            float* data, float** ranges CC_DEFAULT(NULL),
                            int uniform CC_DEFAULT(1));

/** @brief Releases the histogram.

The function releases the histogram (header and the data). The pointer to the histogram is cleared
by the function. If \*hist pointer is already NULL, the function does nothing.

@param hist Double pointer to the released histogram.
 */
CVAPI(void)  cvReleaseHist( CvHistogram** hist );

/** @brief Clears the histogram.

The function sets all of the histogram bins to 0 in case of a dense histogram and removes all
histogram bins in case of a sparse array.

@param hist Histogram.
 */
CVAPI(void)  cvClearHist( CvHistogram* hist );

/** @brief Finds the minimum and maximum histogram bins.

The function finds the minimum and maximum histogram bins and their positions. All of output
arguments are optional. Among several extremas with the same value the ones with the minimum index
(in the lexicographical order) are returned. In case of several maximums or minimums, the earliest
in the lexicographical order (extrema locations) is returned.

@param hist Histogram.
@param min_value Pointer to the minimum value of the histogram.
@param max_value Pointer to the maximum value of the histogram.
@param min_idx Pointer to the array of coordinates for the minimum.
@param max_idx Pointer to the array of coordinates for the maximum.
 */
CVAPI(void)  cvGetMinMaxHistValue( const CvHistogram* hist,
                                   float* min_value, float* max_value,
                                   int* min_idx CC_DEFAULT(NULL),
                                   int* max_idx CC_DEFAULT(NULL));


/** @brief Normalizes the histogram.

The function normalizes the histogram bins by scaling them so that the sum of the bins becomes equal
to factor.

@param hist Pointer to the histogram.
@param factor Normalization factor.
 */
CVAPI(void)  cvNormalizeHist( CvHistogram* hist, double factor );


/** @brief Thresholds the histogram.

The function clears histogram bins that are below the specified threshold.

@param hist Pointer to the histogram.
@param threshold Threshold level.
 */
CVAPI(void)  cvThreshHist( CvHistogram* hist, double threshold );


/** Compares two histogram */
CVAPI(double)  cvCompareHist( const CvHistogram* hist1,
                              const CvHistogram* hist2,
                              int method);

/** @brief Copies a histogram.

The function makes a copy of the histogram. If the second histogram pointer \*dst is NULL, a new
histogram of the same size as src is created. Otherwise, both histograms must have equal types and
sizes. Then the function copies the bin values of the source histogram to the destination histogram
and sets the same bin value ranges as in src.

@param src Source histogram.
@param dst Pointer to the destination histogram.
 */
CVAPI(void)  cvCopyHist( const CvHistogram* src, CvHistogram** dst );


/** @brief Calculates bayesian probabilistic histograms
   (each or src and dst is an array of _number_ histograms */
CVAPI(void)  cvCalcBayesianProb( CvHistogram** src, int number,
                                CvHistogram** dst);

/** @brief Calculates array histogram
@see calcHist
*/
CVAPI(void)  cvCalcArrHist( img_t** arr, CvHistogram* hist,
                            int accumulate CC_DEFAULT(0),
                            const img_t* mask CC_DEFAULT(NULL) );

/** @overload */
CC_INLINE  void  cvCalcHist( img_t** image, CvHistogram* hist,
                             int accumulate CC_DEFAULT(0),
                             const img_t* mask CC_DEFAULT(NULL) )
{
    cvCalcArrHist( (img_t**)image, hist, accumulate, mask );
}

/** @brief Calculates back project
@see cvCalcBackProject, calcBackProject
*/
CVAPI(void)  cvCalcArrBackProject( img_t** image, img_t* dst,
                                   const CvHistogram* hist );

#define  cvCalcBackProject(image, dst, hist) cvCalcArrBackProject((img_t**)image, dst, hist)


/** @brief Locates a template within an image by using a histogram comparison.

The function calculates the back projection by comparing histograms of the source image patches with
the given histogram. The function is similar to matchTemplate, but instead of comparing the raster
patch with all its possible positions within the search window, the function CalcBackProjectPatch
compares histograms. See the algorithm diagram below:

![image](pics/backprojectpatch.png)

@param image Source images (though, you may pass img_t\*\* as well).
@param dst Destination image.
@param range
@param hist Histogram.
@param method Comparison method passed to cvCompareHist (see the function description).
@param factor Normalization factor for histograms that affects the normalization scale of the
destination image. Pass 1 if not sure.

@see cvCalcBackProjectPatch
 */
CVAPI(void)  cvCalcArrBackProjectPatch( img_t** image, img_t* dst, CSize range,
                                        CvHistogram* hist, int method,
                                        double factor );

#define  cvCalcBackProjectPatch( image, dst, range, hist, method, factor ) \
     cvCalcArrBackProjectPatch( (img_t**)image, dst, range, hist, method, factor )


/** @brief Divides one histogram by another.

The function calculates the object probability density from two histograms as:

\f[\texttt{disthist} (I)= \forkthree{0}{if \(\texttt{hist1}(I)=0\)}{\texttt{scale}}{if \(\texttt{hist1}(I) \ne 0\) and \(\texttt{hist2}(I) > \texttt{hist1}(I)\)}{\frac{\texttt{hist2}(I) \cdot \texttt{scale}}{\texttt{hist1}(I)}}{if \(\texttt{hist1}(I) \ne 0\) and \(\texttt{hist2}(I) \le \texttt{hist1}(I)\)}\f]

@param hist1 First histogram (the divisor).
@param hist2 Second histogram.
@param dst_hist Destination histogram.
@param scale Scale factor for the destination histogram.
 */
CVAPI(void)  cvCalcProbDensity( const CvHistogram* hist1, const CvHistogram* hist2,
                                CvHistogram* dst_hist, double scale CC_DEFAULT(255) );

/** @brief equalizes histogram of 8-bit single-channel image
@see equalizeHist
*/
CVAPI(void)  cvEqualizeHist( const img_t* src, img_t* dst );


/** @brief Applies distance transform to binary image
@see distanceTransform
*/
CVAPI(void)  cvDistTransform( const img_t* src, img_t* dst,
                              int distance_type CC_DEFAULT(CC_DIST_L2),
                              int mask_size CC_DEFAULT(3),
                              const float* mask CC_DEFAULT(NULL),
                              img_t* labels CC_DEFAULT(NULL),
                              int labelType CC_DEFAULT(CC_DIST_LABEL_CCOMP));


/** @brief Applies fixed-level threshold to grayscale image.

   This is a basic operation applied before retrieving contours
@see threshold
*/
CVAPI(double)  cvThreshold( const img_t*  src, img_t*  dst,
                            double  threshold, double  max_value,
                            int threshold_type );

/** @brief Applies adaptive threshold to grayscale image.

   The two parameters for methods CC_ADAPTIVE_THRESH_MEAN_C and
   CC_ADAPTIVE_THRESH_GAUSSIAN_C are:
   neighborhood size (3, 5, 7 etc.),
   and a constant subtracted from mean (...,-3,-2,-1,0,1,2,3,...)
@see adaptiveThreshold
*/
CVAPI(void)  cvAdaptiveThreshold( const img_t* src, img_t* dst, double max_value,
                                  int adaptive_method CC_DEFAULT(CC_ADAPTIVE_THRESH_MEAN_C),
                                  int threshold_type CC_DEFAULT(CC_THRESH_BINARY),
                                  int block_size CC_DEFAULT(3),
                                  double param1 CC_DEFAULT(5));

/** @brief Fills the connected component until the color difference gets large enough
@see floodFill
*/
CVAPI(void)  cvFloodFill( img_t* image, CPoint seed_point,
                          CScalar new_val, CScalar lo_diff CC_DEFAULT(cScalarAll(0)),
                          CScalar up_diff CC_DEFAULT(cScalarAll(0)),
                          CConnectedComp* comp CC_DEFAULT(NULL),
                          int flags CC_DEFAULT(4),
                          img_t* mask CC_DEFAULT(NULL));

/****************************************************************************************\
*                                  Feature detection                                     *
\****************************************************************************************/

/** @brief Runs canny edge detector
@see Canny
*/
CVAPI(void)  cvCanny( const img_t* image, img_t* edges, double threshold1,
                      double threshold2, int  aperture_size CC_DEFAULT(3) );

/** @brief Calculates constraint image for corner detection

   Dx^2 * Dyy + Dxx * Dy^2 - 2 * Dx * Dy * Dxy.
   Applying threshold to the result gives coordinates of corners
@see preCornerDetect
*/
CVAPI(void) cvPreCornerDetect( const img_t* image, img_t* corners,
                               int aperture_size CC_DEFAULT(3) );

/** @brief Calculates eigen values and vectors of 2x2
   gradient covariation matrix at every image pixel
@see cornerEigenValsAndVecs
*/
CVAPI(void)  cvCornerEigenValsAndVecs( const img_t* image, img_t* eigenvv,
                                       int block_size, int aperture_size CC_DEFAULT(3) );

/** @brief Calculates minimal eigenvalue for 2x2 gradient covariation matrix at
   every image pixel
@see cornerMinEigenVal
*/
CVAPI(void)  cvCornerMinEigenVal( const img_t* image, img_t* eigenval,
                                  int block_size, int aperture_size CC_DEFAULT(3) );

/** @brief Harris corner detector:

   Calculates det(M) - k*(trace(M)^2), where M is 2x2 gradient covariation matrix for each pixel
@see cornerHarris
*/
CVAPI(void)  cvCornerHarris( const img_t* image, img_t* harris_response,
                             int block_size, int aperture_size CC_DEFAULT(3),
                             double k CC_DEFAULT(0.04) );

/** @brief Adjust corner position using some sort of gradient search
@see cornerSubPix
*/
CVAPI(void)  cvFindCornerSubPix( const img_t* image, CPoint2D32f* corners,
                                 int count, CSize win, CSize zero_zone,
                                 CTermCriteria  criteria );

/** @brief Finds a sparse set of points within the selected region
   that seem to be easy to track
@see goodFeaturesToTrack
*/
CVAPI(void)  cvGoodFeaturesToTrack( const img_t* image, img_t* eig_image,
                                    img_t* temp_image, CPoint2D32f* corners,
                                    int* corner_count, double  quality_level,
                                    double  min_distance,
                                    const img_t* mask CC_DEFAULT(NULL),
                                    int block_size CC_DEFAULT(3),
                                    int use_harris CC_DEFAULT(0),
                                    double k CC_DEFAULT(0.04) );

/** @brief Finds lines on binary image using one of several methods.

   line_storage is either memory storage or 1 x _max number of lines_ img_t, its
   number of columns is changed by the function.
   method is one of CC_HOUGH_*;
   rho, theta and threshold are used for each of those methods;
   param1 ~ line length, param2 ~ line gap - for probabilistic,
   param1 ~ srn, param2 ~ stn - for multi-scale
@see HoughLines
*/
CVAPI(CvSeq*)  cvHoughLines2( img_t* image, void* line_storage, int method,
                              double rho, double theta, int threshold,
                              double param1 CC_DEFAULT(0), double param2 CC_DEFAULT(0),
                              double min_theta CC_DEFAULT(0), double max_theta CC_DEFAULT(CC_PI));

/** @brief Finds circles in the image
@see HoughCircles
*/
CVAPI(CvSeq*) cvHoughCircles( img_t* image, void* circle_storage,
                              int method, double dp, double min_dist,
                              double param1 CC_DEFAULT(100),
                              double param2 CC_DEFAULT(100),
                              int min_radius CC_DEFAULT(0),
                              int max_radius CC_DEFAULT(0));

/** @brief Fits a line into set of 2d or 3d points in a robust way (M-estimator technique)
@see fitLine
*/
CVAPI(void)  cvFitLine( const img_t* points, int dist_type, double param,
                        double reps, double aeps, float* line );

/****************************************************************************************\
*                                     Drawing                                            *
\****************************************************************************************/

/****************************************************************************************\
*       Drawing functions work with images/matrices of arbitrary type.                   *
*       For color images the channel order is BGR[A]                                     *
*       Antialiasing is supported only for 8-bit image now.                              *
*       All the functions include parameter color that means rgb value (that may be      *
*       constructed with CC_RGB macro) for color images and brightness                   *
*       for grayscale images.                                                            *
*       If a drawn figure is partially or completely outside of the image, it is clipped.*
\****************************************************************************************/

#define CC_RGB( r, g, b )  cScalar( (b), (g), (r), 0 )
#define CC_FILLED -1

#define CC_AA 16

/** @brief Draws 4-connected, 8-connected or antialiased line segment connecting two points
@see line
*/
CVAPI(void)  cvLine( img_t* img, CPoint pt1, CPoint pt2,
                     CScalar color, int thickness CC_DEFAULT(1),
                     int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) );

/** @brief Draws a rectangle given two opposite corners of the rectangle (pt1 & pt2)

   if thickness<0 (e.g. thickness == CC_FILLED), the filled box is drawn
@see rectangle
*/
CVAPI(void)  cvRectangle( img_t* img, CPoint pt1, CPoint pt2,
                          CScalar color, int thickness CC_DEFAULT(1),
                          int line_type CC_DEFAULT(8),
                          int shift CC_DEFAULT(0));

/** @brief Draws a rectangle specified by a CRect structure
@see rectangle
*/
CVAPI(void)  cvRectangleR( img_t* img, CRect r,
                           CScalar color, int thickness CC_DEFAULT(1),
                           int line_type CC_DEFAULT(8),
                           int shift CC_DEFAULT(0));


/** @brief Draws a circle with specified center and radius.

   Thickness works in the same way as with cvRectangle
@see circle
*/
CVAPI(void)  cvCircle( img_t* img, CPoint center, int radius,
                       CScalar color, int thickness CC_DEFAULT(1),
                       int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0));

/** @brief Draws ellipse outline, filled ellipse, elliptic arc or filled elliptic sector

   depending on _thickness_, _start_angle_ and _end_angle_ parameters. The resultant figure
   is rotated by _angle_. All the angles are in degrees
@see ellipse
*/
CVAPI(void)  cvEllipse( img_t* img, CPoint center, CSize axes,
                        double angle, double start_angle, double end_angle,
                        CScalar color, int thickness CC_DEFAULT(1),
                        int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0));

CC_INLINE  void  cvEllipseBox( img_t* img, CBox2D box, CScalar color,
                               int thickness CC_DEFAULT(1),
                               int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    CSize axes;
    axes.width = cRound(box.size.width*0.5);
    axes.height = cRound(box.size.height*0.5);

    cvEllipse( img, cvPointFrom32f( box.center ), axes, box.angle,
               0, 360, color, thickness, line_type, shift );
}

/** @brief Fills convex or monotonous polygon.
@see fillConvexPoly
*/
CVAPI(void)  cvFillConvexPoly( img_t* img, const CPoint* pts, int npts, CScalar color,
                               int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0));

/** @brief Fills an area bounded by one or more arbitrary polygons
@see fillPoly
*/
CVAPI(void)  cvFillPoly( img_t* img, CPoint** pts, const int* npts,
                         int contours, CScalar color,
                         int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) );

/** @brief Draws one or more polygonal curves
@see polylines
*/
CVAPI(void)  cvPolyLine( img_t* img, CPoint** pts, const int* npts, int contours,
                         int is_closed, CScalar color, int thickness CC_DEFAULT(1),
                         int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) );

#define cvDrawRect cvRectangle
#define cvDrawLine cvLine
#define cvDrawCircle cvCircle
#define cvDrawEllipse cvEllipse
#define cvDrawPolyLine cvPolyLine

/** @brief Clips the line segment connecting *pt1 and *pt2
   by the rectangular window

   (0<=x<img_size.width, 0<=y<img_size.height).
@see clipLine
*/
CVAPI(int) cvClipLine( CSize img_size, CPoint* pt1, CPoint* pt2 );

/** @brief Initializes line iterator.

Initially, line_iterator->ptr will point to pt1 (or pt2, see left_to_right description) location in
the image. Returns the number of pixels on the line between the ending points.
@see LineIterator
*/
CVAPI(int)  cvInitLineIterator( const img_t* image, CPoint pt1, CPoint pt2,
                                CLineIterator* line_iterator,
                                int connectivity CC_DEFAULT(8),
                                int left_to_right CC_DEFAULT(0));

#define CC_NEXT_LINE_POINT( line_iterator )                     \
{                                                               \
    int _line_iterator_mask = (line_iterator).err < 0 ? -1 : 0; \
    (line_iterator).err += (line_iterator).minus_delta +        \
        ((line_iterator).plus_delta & _line_iterator_mask);     \
    (line_iterator).ptr += (line_iterator).minus_step +         \
        ((line_iterator).plus_step & _line_iterator_mask);      \
}


#define CC_FONT_HERSHEY_SIMPLEX         0
#define CC_FONT_HERSHEY_PLAIN           1
#define CC_FONT_HERSHEY_DUPLEX          2
#define CC_FONT_HERSHEY_COMPLEX         3
#define CC_FONT_HERSHEY_TRIPLEX         4
#define CC_FONT_HERSHEY_COMPLEX_SMALL   5
#define CC_FONT_HERSHEY_SCRIPT_SIMPLEX  6
#define CC_FONT_HERSHEY_SCRIPT_COMPLEX  7

#define CC_FONT_ITALIC                 16

#define CC_FONT_VECTOR0    CC_FONT_HERSHEY_SIMPLEX


/** Font structure */
typedef struct CvFont
{
  const char* nameFont;   //Qt:nameFont
  CScalar color;       //Qt:ColorFont -> cScalar(blue_component, green_component, red_component[, alpha_component])
    int         font_face;    //Qt: bool italic         /** =CC_FONT_* */
    const int*  ascii;      //!< font data and metrics
    const int*  greek;
    const int*  cyrillic;
    float       hscale, vscale;
    float       shear;      //!< slope coefficient: 0 - normal, >0 - italic
    int         thickness;    //!< Qt: weight               /** letters thickness */
    float       dx;       //!< horizontal interval between letters
    int         line_type;    //!< Qt: PointSize
}
CvFont;

/** @brief Initializes font structure (OpenCV 1.x API).

The function initializes the font structure that can be passed to text rendering functions.

@param font Pointer to the font structure initialized by the function
@param font_face Font name identifier. See HersheyFonts and corresponding old CC_* identifiers.
@param hscale Horizontal scale. If equal to 1.0f , the characters have the original width
depending on the font type. If equal to 0.5f , the characters are of half the original width.
@param vscale Vertical scale. If equal to 1.0f , the characters have the original height depending
on the font type. If equal to 0.5f , the characters are of half the original height.
@param shear Approximate tangent of the character slope relative to the vertical line. A zero
value means a non-italic font, 1.0f means about a 45 degree slope, etc.
@param thickness Thickness of the text strokes
@param line_type Type of the strokes, see line description

@sa cvPutText
 */
CVAPI(void)  cvInitFont( CvFont* font, int font_face,
                         double hscale, double vscale,
                         double shear CC_DEFAULT(0),
                         int thickness CC_DEFAULT(1),
                         int line_type CC_DEFAULT(8));

CC_INLINE CvFont cvFont( double scale, int thickness CC_DEFAULT(1) )
{
    CvFont font;
    cvInitFont( &font, CC_FONT_HERSHEY_PLAIN, scale, scale, 0, thickness, CC_AA );
    return font;
}

/** @brief Renders text stroke with specified font and color at specified location.
   CvFont should be initialized with cvInitFont
@see cvInitFont, cvGetTextSize, cvFont, putText
*/
CVAPI(void)  cvPutText( img_t* img, const char* text, CPoint org,
                        const CvFont* font, CScalar color );

/** @brief Calculates bounding box of text stroke (useful for alignment)
@see getTextSize
*/
CVAPI(void)  cvGetTextSize( const char* text_string, const CvFont* font,
                            CSize* text_size, int* baseline );

/** @brief Unpacks color value

if arrtype is CC_8UC?, _color_ is treated as packed color value, otherwise the first channels
(depending on arrtype) of destination scalar are set to the same value = _color_
*/
CVAPI(CScalar)  cvColorToScalar( double packed_color, int arrtype );

/** @brief Returns the polygon points which make up the given ellipse.

The ellipse is define by the box of size 'axes' rotated 'angle' around the 'center'. A partial
sweep of the ellipse arc can be done by spcifying arc_start and arc_end to be something other than
0 and 360, respectively. The input array 'pts' must be large enough to hold the result. The total
number of points stored into 'pts' is returned by this function.
@see ellipse2Poly
*/
CVAPI(int) cvEllipse2Poly( CPoint center, CSize axes,
                 int angle, int arc_start, int arc_end, CPoint * pts, int delta );

/** @brief Draws contour outlines or filled interiors on the image
@see drawContours
*/
CVAPI(void)  cvDrawContours( img_t *img, CvSeq* contour,
                             CScalar external_color, CScalar hole_color,
                             int max_level, int thickness CC_DEFAULT(1),
                             int line_type CC_DEFAULT(8),
                             CPoint offset CC_DEFAULT(cPoint(0,0)));

/** @} */

#ifdef __cplusplus
}
#endif

#endif
