
#ifndef _CVTYPES_H_
#define _CVTYPES_H_

#ifndef SKIP_INCLUDES
  #include <assert.h>
  #include <stdlib.h>
#endif

/* spatial and central moments */
typedef struct CMoments
{
    double  m00, m10, m01, m20, m11, m02, m30, m21, m12, m03; /* spatial moments */
    double  mu20, mu11, mu02, mu30, mu21, mu12, mu03; /* central moments */
    double  inv_sqrt_m00; /* m00 != 0 ? 1/sqrt(m00) : 0 */
}
CMoments;

/* Hu invariants */
typedef struct CHuMoments
{
    double hu1, hu2, hu3, hu4, hu5, hu6, hu7; /* Hu invariants */
}
CHuMoments;

/**************************** Connected Component  **************************************/

typedef struct CConnectedComp
{
    double area;    /* area of the connected component  */
    CScalar value; /* average color of the connected component */
    CRect rect;    /* ROI of the component  */
    CvSeq* contour; /* optional component boundary
                      (the contour might have child contours corresponding to the holes)*/
}
CConnectedComp;

/*
Internal structure that is used for sequental retrieving contours from the image.
It supports both hierarchical and plane variants of Suzuki algorithm.
*/
typedef struct _CvContourScanner* CvContourScanner;

/* contour retrieval mode */
#define CC_RETR_EXTERNAL 0
#define CC_RETR_LIST     1
#define CC_RETR_CCOMP    2
#define CC_RETR_TREE     3

/* contour approximation method */
#define CC_CHAIN_CODE               0
#define CC_CHAIN_APPROX_NONE        1
#define CC_CHAIN_APPROX_SIMPLE      2
#define CC_CHAIN_APPROX_TC89_L1     3
#define CC_CHAIN_APPROX_TC89_KCOS   4
#define CC_LINK_RUNS                5

/* Freeman chain reader state */
typedef struct CvChainPtReader
{
    CC_SEQ_READER_FIELDS()
    char      code;
    CPoint   pt;
    char      deltas[8][2];
}
CvChainPtReader;

/* initializes 8-element array for fast access to 3x3 neighborhood of a pixel */
#define  CC_INIT_3X3_DELTAS(deltas, step, nch)            \
    ((deltas)[0] =  (nch),  (deltas)[1] = -(step) + (nch),  \
     (deltas)[2] = -(step), (deltas)[3] = -(step) - (nch),  \
     (deltas)[4] = -(nch),  (deltas)[5] =  (step) - (nch),  \
     (deltas)[6] =  (step), (deltas)[7] =  (step) + (nch))

/* Contour tree header */
typedef struct CvContourTree
{
    CC_SEQUENCE_FIELDS()
    CPoint p1;            /* the first point of the binary tree root segment */
    CPoint p2;            /* the last point of the binary tree root segment */
}
CvContourTree;

/* Finds a sequence of convexity defects of given contour */
typedef struct CvConvexityDefect
{
    CPoint* start; /* point of the contour where the defect begins */
    CPoint* end; /* point of the contour where the defect ends */
    CPoint* depth_point; /* the farthest from the convex hull point within the defect */
    float depth; /* distance between the farthest point and the convex hull */
}
CvConvexityDefect;

/************ Data structures and related enumerations for Planar Subdivisions ************/

typedef size_t CvSubdiv2DEdge;

#define CC_QUADEDGE2D_FIELDS()     \
    int flags;                     \
    struct CvSubdiv2DPoint* pt[4]; \
    CvSubdiv2DEdge  next[4];

#define CC_SUBDIV2D_POINT_FIELDS()\
    int            flags;      \
    CvSubdiv2DEdge first;      \
    CPoint2D32f   pt;

#define CC_SUBDIV2D_VIRTUAL_POINT_FLAG (1 << 30)

typedef struct CvQuadEdge2D
{
    CC_QUADEDGE2D_FIELDS()
}
CvQuadEdge2D;

typedef struct CvSubdiv2DPoint
{
    CC_SUBDIV2D_POINT_FIELDS()
}
CvSubdiv2DPoint;

#define CC_SUBDIV2D_FIELDS()    \
    CC_GRAPH_FIELDS()           \
    int  quad_edges;            \
    int  is_geometry_valid;     \
    CvSubdiv2DEdge recent_edge; \
    CPoint2D32f  topleft;      \
    CPoint2D32f  bottomright;
    
typedef struct CvSubdiv2D
{
    CC_SUBDIV2D_FIELDS()
}
CvSubdiv2D;


typedef enum CvSubdiv2DPointLocation
{
    CC_PTLOC_ERROR = -2,
    CC_PTLOC_OUTSIDE_RECT = -1,
    CC_PTLOC_INSIDE = 0,
    CC_PTLOC_VERTEX = 1,
    CC_PTLOC_ON_EDGE = 2
}
CvSubdiv2DPointLocation;

typedef enum CvNextEdgeType
{
    CC_NEXT_AROUND_ORG   = 0x00,
    CC_NEXT_AROUND_DST   = 0x22,
    CC_PREV_AROUND_ORG   = 0x11,
    CC_PREV_AROUND_DST   = 0x33,
    CC_NEXT_AROUND_LEFT  = 0x13,
    CC_NEXT_AROUND_RIGHT = 0x31,
    CC_PREV_AROUND_LEFT  = 0x20,
    CC_PREV_AROUND_RIGHT = 0x02
}
CvNextEdgeType;

/* get the next edge with the same origin point (counterwise) */
#define  CC_SUBDIV2D_NEXT_EDGE(edge)  (((CvQuadEdge2D*)((edge) & ~3))->next[(edge)&3])


/* Defines for Distance Transform */
#define CC_DIST_USER    -1  /* User defined distance */
#define CC_DIST_L1      1   /* distance = |x1-x2| + |y1-y2| */
#define CC_DIST_L2      2   /* the simple euclidean distance */
#define CC_DIST_C       3   /* distance = max(|x1-x2|,|y1-y2|) */
#define CC_DIST_L12     4   /* L1-L2 metric: distance = 2(sqrt(1+x*x/2) - 1)) */
#define CC_DIST_FAIR    5   /* distance = c^2(|x|/c-log(1+|x|/c)), c = 1.3998 */
#define CC_DIST_WELSCH  6   /* distance = c^2/2(1-exp(-(x/c)^2)), c = 2.9846 */
#define CC_DIST_HUBER   7   /* distance = |x|<c ? x^2/2 : c(|x|-c/2), c=1.345 */


/* Filters used in pyramid decomposition */
typedef enum CvFilter
{
    CC_GAUSSIAN_5x5 = 7
}
CvFilter;

/****************************************************************************************/
/*                                    Older definitions                                 */
/****************************************************************************************/

typedef float*   CvVect32f;
typedef float*   CvMatr32f;
typedef double*  CvVect64d;
typedef double*  CvMatr64d;

typedef struct CvMatrix3
{
    float m[3][3];
}
CvMatrix3;


#ifdef __cplusplus
extern "C" {
#endif

typedef float (C_CDECL * CvDistanceFunction)(const float* a, const float* b, void* user_param);

#ifdef __cplusplus
}
#endif

typedef struct CvConDensation
{
    int MP;
    int DP;
    float* DynamMatr;       /* Matrix of the linear Dynamics system  */
    float* State;           /* Vector of State                       */
    int SamplesNum;         /* Number of the Samples                 */
    float** flSamples;      /* arr of the Sample Vectors             */
    float** flNewSamples;   /* temporary array of the Sample Vectors */
    float* flConfidence;    /* Confidence for each Sample            */
    float* flCumulative;    /* Cumulative confidence                 */
    float* Temp;            /* Temporary vector                      */
    float* RandomSample;    /* RandomVector to update sample set     */
    struct CvRandState* RandS; /* Array of structures to generate random vectors */
}
CvConDensation;

/*
standard Kalman filter (in G. Welch' and G. Bishop's notation):

  x(k)=A*x(k-1)+B*u(k)+w(k)  p(w)~N(0,Q)
  z(k)=H*x(k)+v(k),   p(v)~N(0,R)
*/
typedef struct CvKalman
{
    int MP;                     /* number of measurement vector dimensions */
    int DP;                     /* number of state vector dimensions */
    int CP;                     /* number of control vector dimensions */

    /* backward compatibility fields */
#if 1
    float* PosterState;         /* =state_pre->tt.fl */
    float* PriorState;          /* =state_post->tt.fl */
    float* DynamMatr;           /* =transition_matrix->tt.fl */
    float* MeasurementMatr;     /* =measurement_matrix->tt.fl */
    float* MNCovariance;        /* =measurement_noise_cov->tt.fl */
    float* PNCovariance;        /* =process_noise_cov->tt.fl */
    float* KalmGainMatr;        /* =gain->tt.fl */
    float* PriorErrorCovariance;/* =error_cov_pre->tt.fl */
    float* PosterErrorCovariance;/* =error_cov_post->tt.fl */
    float* Temp1;               /* temp1->tt.fl */
    float* Temp2;               /* temp2->tt.fl */
#endif

    img_t* state_pre;           /* predicted state (x'(k)):
                                    x(k)=A*x(k-1)+B*u(k) */
    img_t* state_post;          /* corrected state (x(k)):
                                    x(k)=x'(k)+K(k)*(z(k)-H*x'(k)) */
    img_t* transition_matrix;   /* state transition matrix (A) */
    img_t* control_matrix;      /* control matrix (B)
                                   (it is not used if there is no control)*/
    img_t* measurement_matrix;  /* measurement matrix (H) */
    img_t* process_noise_cov;   /* process noise covariance matrix (Q) */
    img_t* measurement_noise_cov; /* measurement noise covariance matrix (R) */
    img_t* error_cov_pre;       /* priori error estimate covariance matrix (P'(k)):
                                    P'(k)=A*P(k-1)*At + Q)*/
    img_t* gain;                /* Kalman gain matrix (K(k)):
                                    K(k)=P'(k)*Ht*inv(H*P'(k)*Ht+R)*/
    img_t* error_cov_post;      /* posteriori error estimate covariance matrix (P(k)):
                                    P(k)=(I-K(k)*H)*P'(k) */
    img_t* temp1;               /* temporary matrices */
    img_t* temp2;
    img_t* temp3;
    img_t* temp4;
    img_t* temp5;
}
CvKalman;


/*********************** Haar-like Object Detection structures **************************/
#define CC_HAAR_MAGIC_VAL    0x42500000
#define CC_TYPE_NAME_HAAR    "opencv-haar-classifier"

#define CC_IS_HAAR_CLASSIFIER(haar)                                                    \
    ((haar) != NULL &&                                                                   \
    (((const CvHaarClassifierCascade*)(haar))->flags & CC_MAGIC_MASK)==CC_HAAR_MAGIC_VAL)

#define CC_HAAR_FEATURE_MAX  3

typedef struct CvHaarFeature
{
    int  tilted;
    struct
    {
        CRect r;
        float weight;
    } rect[CC_HAAR_FEATURE_MAX];
}
CvHaarFeature;

typedef struct CvHaarClassifier
{
    int count;
    CvHaarFeature* haar_feature;
    float* threshold;
    int* left;
    int* right;
    float* alpha;
}
CvHaarClassifier;

typedef struct CvHaarStageClassifier
{
    int  count;
    float threshold;
    CvHaarClassifier* classifier;

    int next;
    int child;
    int parent;
}
CvHaarStageClassifier;

typedef struct CvHidHaarClassifierCascade CvHidHaarClassifierCascade;

typedef struct CvHaarClassifierCascade
{
    int  flags;
    int  count;
    CSize orig_window_size;
    CSize real_window_size;
    double scale;
    CvHaarStageClassifier* stage_classifier;
    CvHidHaarClassifierCascade* hid_cascade;
}
CvHaarClassifierCascade;

typedef struct CvAvgComp
{
    CRect rect;
    int neighbors;
}
CvAvgComp;

#endif /*_CVTYPES_H_*/


