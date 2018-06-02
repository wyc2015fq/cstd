
#ifndef OPENCC_OBJDETECT_C_H
#define OPENCC_OBJDETECT_C_H

/** @addtogroup objdetect_c
  @{
  */

/****************************************************************************************\
*                         Haar-like Object Detection functions                           *
\****************************************************************************************/

#define CC_HAAR_MAGIC_VAL    0x42500000
#define CC_TYPE_NAME_HAAR    "opencv-haar-classifier"

#define CC_IS_HAAR_CLASSIFIER( haar )                                                    \
    ((haar) != NULL &&                                                                   \
    (((const CvHaarClassifierCascade*)(haar))->flags & CC_MAGIC_MASK)==CC_HAAR_MAGIC_VAL)

#define CC_HAAR_FEATURE_MAX  3
#define CC_HAAR_STAGE_MAX 1000

typedef struct CvHaarFeature
{
    int tilted;
    struct
    {
        CRect r;
        float weight;
    } rect[CC_HAAR_FEATURE_MAX];
} CvHaarFeature;

typedef struct CvHaarClassifier
{
    int count;
    CvHaarFeature* haar_feature;
    float* threshold;
    int* left;
    int* right;
    float* alpha;
} CvHaarClassifier;

typedef struct CvHaarStageClassifier
{
    int  count;
    float threshold;
    CvHaarClassifier* classifier;

    int next;
    int child;
    int parent;
} CvHaarStageClassifier;

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
} CvHaarClassifierCascade;

typedef struct CvAvgComp
{
    CRect rect;
    int neighbors;
} CvAvgComp;

/* Loads haar classifier cascade from a directory.
   It is obsolete: convert your cascade to xml and use cvLoad instead */
CvHaarClassifierCascade* cvLoadHaarClassifierCascade( const char* directory, CSize orig_window_size);

void cvReleaseHaarClassifierCascade( CvHaarClassifierCascade** cascade );

#define CC_HAAR_DO_CANNY_PRUNING    1
#define CC_HAAR_SCALE_IMAGE         2
#define CC_HAAR_FIND_BIGGEST_OBJECT 4
#define CC_HAAR_DO_ROUGH_SEARCH     8

void* cvHaarDetectObjects( const img_t* image,
                     CvHaarClassifierCascade* cascade,
                     double scale_factor CC_DEFAULT(1.1),
                     int min_neighbors CC_DEFAULT(3), int flags CC_DEFAULT(0),
                     CSize min_size CC_DEFAULT(cSize(0,0)), CSize max_size CC_DEFAULT(cSize(0,0)));

/* sets images for haar classifier cascade */
void cvSetImagesForHaarClassifierCascade( CvHaarClassifierCascade* cascade,
                                                const img_t* sum, const img_t* sqsum,
                                                const img_t* tilted_sum, double scale );

/* runs the cascade on the specified window */
int cvRunHaarClassifierCascade( const CvHaarClassifierCascade* cascade, CPoint pt, int start_stage CC_DEFAULT(0));

/** @} objdetect_c */

void* cvHaarDetectObjectsForROC( const img_t* image,
                     CvHaarClassifierCascade* cascade, 
                     std::vector<int>& rejectLevels, std::vector<double>& levelWeightds,
                     double scale_factor CC_DEFAULT(1.1),
                     int min_neighbors CC_DEFAULT(3), int flags CC_DEFAULT(0),
                     CSize min_size CC_DEFAULT(cSize(0, 0)), CSize max_size CC_DEFAULT(cSize(0, 0)),
                     bool outputRejectLevels CC_DEFAULT(false) );


#endif /* OPENCC_OBJDETECT_C_H */
