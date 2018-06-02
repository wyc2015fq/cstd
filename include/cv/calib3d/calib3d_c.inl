

/** @addtogroup calib3d_c
  @{
  */

/****************************************************************************************\
*                      Camera Calibration, Pose Estimation and Stereo                    *
\****************************************************************************************/

typedef struct CvPOSITObject CvPOSITObject;

/* Allocates and initializes CvPOSITObject structure before doing cvPOSIT */
CVAPI(CvPOSITObject*)  cvCreatePOSITObject( CPoint3D32f* points, int point_count );


/* Runs POSIT (POSe from ITeration) algorithm for determining 3d position of
   an object given its model and projection in a weak-perspective case */
CVAPI(void)  cvPOSIT(  CvPOSITObject* posit_object, CPoint2D32f* image_points,
                       double focal_length, CTermCriteria criteria,
                       float* rotation_matrix, float* translation_vector);

/* Releases CvPOSITObject structure */
CVAPI(void)  cvReleasePOSITObject( CvPOSITObject**  posit_object );

/* updates the number of RANSAC iterations */
CVAPI(int) cvRANSACUpdateNumIters( double p, double err_prob,
                                   int model_points, int max_iters );

CVAPI(void) cvConvertPointsHomogeneous( const img_t* src, img_t* dst );

/* Calculates fundamental matrix given a set of corresponding points */
#define CC_FM_7POINT 1
#define CC_FM_8POINT 2

#define CC_LMEDS 4
#define CC_RANSAC 8

#define CC_FM_LMEDS_ONLY  CC_LMEDS
#define CC_FM_RANSAC_ONLY CC_RANSAC
#define CC_FM_LMEDS CC_LMEDS
#define CC_FM_RANSAC CC_RANSAC

enum
{
    CC_ITERATIVE = 0,
    CC_EPNP = 1, // F.Moreno-Noguer, V.Lepetit and P.Fua "EPnP: Efficient Perspective-n-Point Camera Pose Estimation"
    CC_P3P = 2, // X.S. Gao, X.-R. Hou, J. Tang, H.-F. Chang; "Complete Solution Classification for the Perspective-Three-Point Problem"
    CC_DLS = 3 // Joel A. Hesch and Stergios I. Roumeliotis. "A Direct Least-Squares (DLS) Method for PnP"
};

CVAPI(int) cvFindFundamentalMat( const img_t* points1, const img_t* points2,
                                 img_t* fundamental_matrix,
                                 int method CC_DEFAULT(CC_FM_RANSAC),
                                 double param1 CC_DEFAULT(3.), double param2 CC_DEFAULT(0.99),
                                 img_t* status CC_DEFAULT(NULL) );

/* For each input point on one of images
   computes parameters of the corresponding
   epipolar line on the other image */
CVAPI(void) cvComputeCorrespondEpilines( const img_t* points,
                                         int which_image,
                                         const img_t* fundamental_matrix,
                                         img_t* correspondent_lines );

/* Triangulation functions */

CVAPI(void) cvTriangulatePoints(img_t* projMatr1, img_t* projMatr2,
                                img_t* projPoints1, img_t* projPoints2,
                                img_t* points4D);

CVAPI(void) cvCorrectMatches(img_t* F, img_t* points1, img_t* points2,
                             img_t* new_points1, img_t* new_points2);


/* Computes the optimal new camera matrix according to the free scaling parameter alpha:
   alpha=0 - only valid pixels will be retained in the undistorted image
   alpha=1 - all the source image pixels will be retained in the undistorted image
*/
CVAPI(void) cvGetOptimalNewCameraMatrix( const img_t* camera_matrix,
                                         const img_t* dist_coeffs,
                                         CSize image_size, double alpha,
                                         img_t* new_camera_matrix,
                                         CSize new_imag_size CC_DEFAULT(cSize(0,0)),
                                         CRect* valid_pixel_ROI CC_DEFAULT(0),
                                         int center_principal_point CC_DEFAULT(0));

/* Converts rotation vector to rotation matrix or vice versa */
CVAPI(int) cvRodrigues2( const img_t* src, img_t* dst,
                         img_t* jacobian CC_DEFAULT(0) );

/* Finds perspective transformation between the object plane and image (view) plane */
CVAPI(int) cvFindHomography( const img_t* src_points,
                             const img_t* dst_points,
                             img_t* homography,
                             int method CC_DEFAULT(0),
                             double ransacReprojThreshold CC_DEFAULT(3),
                             img_t* mask CC_DEFAULT(0),
                             int maxIters CC_DEFAULT(2000),
                             double confidence CC_DEFAULT(0.995));

/* Computes RQ decomposition for 3x3 matrices */
CVAPI(void) cvRQDecomp3x3( const img_t *matrixM, img_t *matrixR, img_t *matrixQ,
                           img_t *matrixQx CC_DEFAULT(NULL),
                           img_t *matrixQy CC_DEFAULT(NULL),
                           img_t *matrixQz CC_DEFAULT(NULL),
                           CPoint3D64f *eulerAngles CC_DEFAULT(NULL));

/* Computes projection matrix decomposition */
CVAPI(void) cvDecomposeProjectionMatrix( const img_t *projMatr, img_t *calibMatr,
                                         img_t *rotMatr, img_t *posVect,
                                         img_t *rotMatrX CC_DEFAULT(NULL),
                                         img_t *rotMatrY CC_DEFAULT(NULL),
                                         img_t *rotMatrZ CC_DEFAULT(NULL),
                                         CPoint3D64f *eulerAngles CC_DEFAULT(NULL));

/* Computes d(AB)/dA and d(AB)/dB */
CVAPI(void) cvCalcMatMulDeriv( const img_t* A, const img_t* B, img_t* dABdA, img_t* dABdB );

/* Computes r3 = rodrigues(rodrigues(r2)*rodrigues(r1)),
   t3 = rodrigues(r2)*t1 + t2 and the respective derivatives */
CVAPI(void) cvComposeRT( const img_t* _rvec1, const img_t* _tvec1,
                         const img_t* _rvec2, const img_t* _tvec2,
                         img_t* _rvec3, img_t* _tvec3,
                         img_t* dr3dr1 CC_DEFAULT(0), img_t* dr3dt1 CC_DEFAULT(0),
                         img_t* dr3dr2 CC_DEFAULT(0), img_t* dr3dt2 CC_DEFAULT(0),
                         img_t* dt3dr1 CC_DEFAULT(0), img_t* dt3dt1 CC_DEFAULT(0),
                         img_t* dt3dr2 CC_DEFAULT(0), img_t* dt3dt2 CC_DEFAULT(0) );

/* Projects object points to the view plane using
   the specified extrinsic and intrinsic camera parameters */
CVAPI(void) cvProjectPoints2( const img_t* object_points, const img_t* rotation_vector,
                              const img_t* translation_vector, const img_t* camera_matrix,
                              const img_t* distortion_coeffs, img_t* image_points,
                              img_t* dpdrot CC_DEFAULT(NULL), img_t* dpdt CC_DEFAULT(NULL),
                              img_t* dpdf CC_DEFAULT(NULL), img_t* dpdc CC_DEFAULT(NULL),
                              img_t* dpddist CC_DEFAULT(NULL),
                              double aspect_ratio CC_DEFAULT(0));

/* Finds extrinsic camera parameters from
   a few known corresponding point pairs and intrinsic parameters */
CVAPI(void) cvFindExtrinsicCameraParams2( const img_t* object_points,
                                          const img_t* image_points,
                                          const img_t* camera_matrix,
                                          const img_t* distortion_coeffs,
                                          img_t* rotation_vector,
                                          img_t* translation_vector,
                                          int use_extrinsic_guess CC_DEFAULT(0) );

/* Computes initial estimate of the intrinsic camera parameters
   in case of planar calibration target (e.g. chessboard) */
CVAPI(void) cvInitIntrinsicParams2D( const img_t* object_points,
                                     const img_t* image_points,
                                     const img_t* npoints, CSize image_size,
                                     img_t* camera_matrix,
                                     double aspect_ratio CC_DEFAULT(1.) );

#define CC_CALIB_CB_ADAPTIVE_THRESH  1
#define CC_CALIB_CB_NORMALIZE_IMAGE  2
#define CC_CALIB_CB_FILTER_QUADS     4
#define CC_CALIB_CB_FAST_CHECK       8

// Performs a fast check if a chessboard is in the input image. This is a workaround to
// a problem of cvFindChessboardCorners being slow on images with no chessboard
// - src: input image
// - size: chessboard size
// Returns 1 if a chessboard can be in this image and findChessboardCorners should be called,
// 0 if there is no chessboard, -1 in case of error
CVAPI(int) cvCheckChessboard(img_t* src, CSize size);

    /* Detects corners on a chessboard calibration pattern */
CVAPI(int) cvFindChessboardCorners( const void* image, CSize pattern_size,
                                    CPoint2D32f* corners,
                                    int* corner_count CC_DEFAULT(NULL),
                                    int flags CC_DEFAULT(CC_CALIB_CB_ADAPTIVE_THRESH+CC_CALIB_CB_NORMALIZE_IMAGE) );

/* Draws individual chessboard corners or the whole chessboard detected */
CVAPI(void) cvDrawChessboardCorners( img_t* image, CSize pattern_size,
                                     CPoint2D32f* corners,
                                     int count, int pattern_was_found );

#define CC_CALIB_USE_INTRINSIC_GUESS  1
#define CC_CALIB_FIX_ASPECT_RATIO     2
#define CC_CALIB_FIX_PRINCIPAL_POINT  4
#define CC_CALIB_ZERO_TANGENT_DIST    8
#define CC_CALIB_FIX_FOCAL_LENGTH 16
#define CC_CALIB_FIX_K1  32
#define CC_CALIB_FIX_K2  64
#define CC_CALIB_FIX_K3  128
#define CC_CALIB_FIX_K4  2048
#define CC_CALIB_FIX_K5  4096
#define CC_CALIB_FIX_K6  8192
#define CC_CALIB_RATIONAL_MODEL 16384
#define CC_CALIB_THIN_PRISM_MODEL 32768
#define CC_CALIB_FIX_S1_S2_S3_S4  65536
#define CC_CALIB_TILTED_MODEL  262144
#define CC_CALIB_FIX_TAUX_TAUY  524288

#define CC_CALIB_NINTRINSIC 18

/* Finds intrinsic and extrinsic camera parameters
   from a few views of known calibration pattern */
CVAPI(double) cvCalibrateCamera2( const img_t* object_points,
                                const img_t* image_points,
                                const img_t* point_counts,
                                CSize image_size,
                                img_t* camera_matrix,
                                img_t* distortion_coeffs,
                                img_t* rotation_vectors CC_DEFAULT(NULL),
                                img_t* translation_vectors CC_DEFAULT(NULL),
                                int flags CC_DEFAULT(0),
                                CTermCriteria term_crit CC_DEFAULT(cTermCriteria(
                                    CC_TERMCRIT_ITER+CC_TERMCRIT_EPS,30,DBL_EPSILON)) );

/* Computes various useful characteristics of the camera from the data computed by
   cvCalibrateCamera2 */
CVAPI(void) cvCalibrationMatrixValues( const img_t *camera_matrix,
                                CSize image_size,
                                double aperture_width CC_DEFAULT(0),
                                double aperture_height CC_DEFAULT(0),
                                double *fovx CC_DEFAULT(NULL),
                                double *fovy CC_DEFAULT(NULL),
                                double *focal_length CC_DEFAULT(NULL),
                                CPoint2D64f *principal_point CC_DEFAULT(NULL),
                                double *pixel_aspect_ratio CC_DEFAULT(NULL));

#define CC_CALIB_FIX_INTRINSIC  256
#define CC_CALIB_SAME_FOCAL_LENGTH 512

/* Computes the transformation from one camera coordinate system to another one
   from a few correspondent views of the same calibration target. Optionally, calibrates
   both cameras */
CVAPI(double) cvStereoCalibrate( const img_t* object_points, const img_t* image_points1,
                               const img_t* image_points2, const img_t* npoints,
                               img_t* camera_matrix1, img_t* dist_coeffs1,
                               img_t* camera_matrix2, img_t* dist_coeffs2,
                               CSize image_size, img_t* R, img_t* T,
                               img_t* E CC_DEFAULT(0), img_t* F CC_DEFAULT(0),
                               int flags CC_DEFAULT(CC_CALIB_FIX_INTRINSIC),
                               CTermCriteria term_crit CC_DEFAULT(cTermCriteria(
                                   CC_TERMCRIT_ITER+CC_TERMCRIT_EPS,30,1e-6)) );

#define CC_CALIB_ZERO_DISPARITY 1024

/* Computes 3D rotations (+ optional shift) for each camera coordinate system to make both
   views parallel (=> to make all the epipolar lines horizontal or vertical) */
CVAPI(void) cvStereoRectify( const img_t* camera_matrix1, const img_t* camera_matrix2,
                             const img_t* dist_coeffs1, const img_t* dist_coeffs2,
                             CSize image_size, const img_t* R, const img_t* T,
                             img_t* R1, img_t* R2, img_t* P1, img_t* P2,
                             img_t* Q CC_DEFAULT(0),
                             int flags CC_DEFAULT(CC_CALIB_ZERO_DISPARITY),
                             double alpha CC_DEFAULT(-1),
                             CSize new_image_size CC_DEFAULT(cSize(0,0)),
                             CRect* valid_pix_ROI1 CC_DEFAULT(0),
                             CRect* valid_pix_ROI2 CC_DEFAULT(0));

/* Computes rectification transformations for uncalibrated pair of images using a set
   of point correspondences */
CVAPI(int) cvStereoRectifyUncalibrated( const img_t* points1, const img_t* points2,
                                        const img_t* F, CSize img_size,
                                        img_t* H1, img_t* H2,
                                        double threshold CC_DEFAULT(5));



/* stereo correspondence parameters and functions */

#define CC_STEREO_BM_NORMALIZED_RESPONSE  0
#define CC_STEREO_BM_XSOBEL               1

/* Block matching algorithm structure */
typedef struct CvStereoBMState
{
    // pre-filtering (normalization of input images)
    int preFilterType; // =CC_STEREO_BM_NORMALIZED_RESPONSE now
    int preFilterSize; // averaging window size: ~5x5..21x21
    int preFilterCap; // the output of pre-filtering is clipped by [-preFilterCap,preFilterCap]

    // correspondence using Sum of Absolute Difference (SAD)
    int SADWindowSize; // ~5x5..21x21
    int minDisparity;  // minimum disparity (can be negative)
    int numberOfDisparities; // maximum disparity - minimum disparity (> 0)

    // post-filtering
    int textureThreshold;  // the disparity is only computed for pixels
                           // with textured enough neighborhood
    int uniquenessRatio;   // accept the computed disparity d* only if
                           // SAD(d) >= SAD(d*)*(1 + uniquenessRatio/100.)
                           // for any d != d*+/-1 within the search range.
    int speckleWindowSize; // disparity variation window
    int speckleRange; // acceptable range of variation in window

    int trySmallerWindows; // if 1, the results may be more accurate,
                           // at the expense of slower processing
    CRect roi1, roi2;
    int disp12MaxDiff;

    // temporary buffers
    img_t* preFilteredImg0;
    img_t* preFilteredImg1;
    img_t* slidingSumBuf;
    img_t* cost;
    img_t* disp;
} CvStereoBMState;

#define CC_STEREO_BM_BASIC 0
#define CC_STEREO_BM_FISH_EYE 1
#define CC_STEREO_BM_NARROW 2

CVAPI(CvStereoBMState*) cvCreateStereoBMState(int preset CC_DEFAULT(CC_STEREO_BM_BASIC),
                                              int numberOfDisparities CC_DEFAULT(0));

CVAPI(void) cvReleaseStereoBMState( CvStereoBMState** state );

CVAPI(void) cvFindStereoCorrespondenceBM( const img_t* left, const img_t* right,
                                          img_t* disparity, CvStereoBMState* state );

CVAPI(CRect) cvGetValidDisparityROI( CRect roi1, CRect roi2, int minDisparity,
                                      int numberOfDisparities, int SADWindowSize );

CVAPI(void) cvValidateDisparity( img_t* disparity, const img_t* cost,
                                 int minDisparity, int numberOfDisparities,
                                 int disp12MaxDiff CC_DEFAULT(1) );

/* Reprojects the computed disparity image to the 3D space using the specified 4x4 matrix */
CVAPI(void)  cvReprojectImageTo3D( const img_t* disparityImage,
                                   img_t* _3dImage, const img_t* Q,
                                   int handleMissingValues CC_DEFAULT(0) );

/** @} calib3d_c */

#ifdef __cplusplus
} // extern "C"


#endif
