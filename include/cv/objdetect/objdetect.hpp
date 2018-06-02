
#ifndef OPENCC_OBJDETECT_HPP
#define OPENCC_OBJDETECT_HPP

//#include "opencv2/core.hpp"

/**
@defgroup objdetect Object Detection

Haar Feature-based Cascade Classifier for Object Detection
----------------------------------------------------------

The object detector described below has been initially proposed by Paul Viola @cite Viola01 and
improved by Rainer Lienhart @cite Lienhart02 .

First, a classifier (namely a *cascade of boosted classifiers working with haar-like features*) is
trained with a few hundred sample views of a particular object (i.e., a face or a car), called
positive examples, that are scaled to the same size (say, 20x20), and negative examples - arbitrary
images of the same size.

After a classifier is trained, it can be applied to a region of interest (of the same size as used
during the training) in an input image. The classifier outputs a "1" if the region is likely to show
the object (i.e., face/car), and "0" otherwise. To search for the object in the whole image one can
move the search window across the image and check every location using the classifier. The
classifier is designed so that it can be easily "resized" in order to be able to find the objects of
interest at different sizes, which is more efficient than resizing the image itself. So, to find an
object of an unknown size in the image the scan procedure should be done several times at different
scales.

The word "cascade" in the classifier name means that the resultant classifier consists of several
simpler classifiers (*stages*) that are applied subsequently to a region of interest until at some
stage the candidate is rejected or all the stages are passed. The word "boosted" means that the
classifiers at every stage of the cascade are complex themselves and they are built out of basic
classifiers using one of four different boosting techniques (weighted voting). Currently Discrete
Adaboost, Real Adaboost, Gentle Adaboost and Logitboost are supported. The basic classifiers are
decision-tree classifiers with at least 2 leaves. Haar-like features are the input to the basic
classifiers, and are calculated as described below. The current algorithm uses the following
Haar-like features:

![image](pics/haarfeatures.png)

The feature used in a particular classifier is specified by its shape (1a, 2b etc.), position within
the region of interest and the scale (this scale is not the same as the scale used at the detection
stage, though these two scales are multiplied). For example, in the case of the third line feature
(2c) the response is calculated as the difference between the sum of image pixels under the
rectangle covering the whole feature (including the two white stripes and the black stripe in the
middle) and the sum of the image pixels under the black stripe multiplied by 3 in order to
compensate for the differences in the size of areas. The sums of pixel values over a rectangular
regions are calculated rapidly using integral images (see below and the integral description).

To see the object detector at work, have a look at the facedetect demo:
<https://github.com/opencv/opencv/tree/master/samples/cpp/dbt_face_detection.cpp>

The following reference is for the detection part only. There is a separate application called
opencv_traincascade that can train a cascade of boosted classifiers from a set of samples.

@note In the new C++ interface it is also possible to use LBP (local binary pattern) features in
addition to Haar-like features. .. [Viola01] Paul Viola and Michael J. Jones. Rapid Object Detection
using a Boosted Cascade of Simple Features. IEEE CVPR, 2001. The paper is available online at
<http://research.microsoft.com/en-us/um/people/viola/Pubs/Detect/violaJones_CVPR2001.pdf>

@{
    @defgroup objdetect_c C API
@}
 */

typedef struct CHaarClassifierCascade CHaarClassifierCascade;

//! @addtogroup objdetect
//! @{

///////////////////////////// Object Detection ////////////////////////////

//! class for grouping object candidates, detected by Cascade Classifier, HOG etc.
//! instance of the class is to be passed to partition (see cxoperations.hpp)
class SimilarRects
{
public:
    SimilarRects(double _eps) : eps(_eps) {}
    inline bool operator()(const CRect& r1, const CRect& r2) const
    {
        double delta = eps * (MIN(r1.width, r2.width) + MIN(r1.height, r2.height)) * 0.5;
        return abs(r1.x - r2.x) <= delta &&
            abs(r1.y - r2.y) <= delta &&
            abs(r1.x + r1.width - r2.x - r2.width) <= delta &&
            abs(r1.y + r1.height - r2.y - r2.height) <= delta;
    }
    double eps;
};

/** @brief Groups the object candidate rectangles.

@param rectList Input/output vector of rectangles. Output vector includes retained and grouped
rectangles. (The Python list is not modified in place.)
@param groupThreshold Minimum possible number of rectangles minus 1. The threshold is used in a
group of rectangles to retain it.
@param eps Relative difference between sides of the rectangles to merge them into a group.

The function is a wrapper for the generic function partition . It clusters all the input rectangles
using the rectangle equivalence criteria that combines rectangles with similar sizes and similar
locations. The similarity is defined by eps. When eps=0 , no clustering is done at all. If
\f$\texttt{eps}\rightarrow +\inf\f$ , all the rectangles are put in one cluster. Then, the small
clusters containing less than or equal to groupThreshold rectangles are rejected. In each other
cluster, the average rectangle is computed and put into the output rectangle list.
 */
CC_EXPORTS   void groupRectangles(std::vector<CRect>& rectList, int groupThreshold, double eps = 0.2);
/** @overload */
CC_EXPORTS_W void groupRectangles(CC_IN_OUT std::vector<CRect>& rectList, CC_OUT std::vector<int>& weights,
                                  int groupThreshold, double eps = 0.2);
/** @overload */
CC_EXPORTS   void groupRectangles(std::vector<CRect>& rectList, int groupThreshold,
                                  double eps, std::vector<int>* weights, std::vector<double>* levelWeights );
/** @overload */
CC_EXPORTS   void groupRectangles(std::vector<CRect>& rectList, std::vector<int>& rejectLevels,
                                  std::vector<double>& levelWeights, int groupThreshold, double eps = 0.2);
/** @overload */
CC_EXPORTS   void groupRectangles_meanshift(std::vector<CRect>& rectList, std::vector<double>& foundWeights,
                                            std::vector<double>& foundScales,
                                            double detectThreshold = 0.0, CSize winDetSize = cSize(64, 128));

//template<> CC_EXPORTS void CHaarClassifierCascade::operator ()(CHaarClassifierCascade* obj) const;

enum { CASCADE_DO_CANNY_PRUNING    = 1,
       CASCADE_SCALE_IMAGE         = 2,
       CASCADE_FIND_BIGGEST_OBJECT = 4,
       CASCADE_DO_ROUGH_SEARCH     = 8
     };

class CC_EXPORTS_W BaseCascadeClassifier : public Algorithm
{
public:
    virtual ~BaseCascadeClassifier();
    virtual bool empty() const = 0;
    virtual bool load( const char* filename ) = 0;
    virtual void detectMultiScale( const img_t* image,
                           CC_OUT std::vector<CRect>& objects,
                           double scaleFactor,
                           int minNeighbors, int flags,
                           CSize minSize, CSize maxSize ) = 0;

    virtual void detectMultiScale( const img_t* image,
                           CC_OUT std::vector<CRect>& objects,
                           CC_OUT std::vector<int>& numDetections,
                           double scaleFactor,
                           int minNeighbors, int flags,
                           CSize minSize, CSize maxSize ) = 0;

    virtual void detectMultiScale( const img_t* image,
                                   CC_OUT std::vector<CRect>& objects,
                                   CC_OUT std::vector<int>& rejectLevels,
                                   CC_OUT std::vector<double>& levelWeights,
                                   double scaleFactor,
                                   int minNeighbors, int flags,
                                   CSize minSize, CSize maxSize,
                                   bool outputRejectLevels ) = 0;

    virtual bool isOldFormatCascade() const = 0;
    virtual CSize getOriginalWindowSize() const = 0;
    virtual int getFeatureType() const = 0;
    virtual void* getOldCascade() = 0;

    class CC_EXPORTS MaskGenerator
    {
    public:
        virtual ~MaskGenerator() {}
        virtual img_t* generateMask(const img_t* src, img_t* dst)=0;
        virtual void initializeMask(const img_t* /*src*/) { }
    };
    virtual void setMaskGenerator(const MaskGenerator* maskGenerator) = 0;
    virtual MaskGenerator* getMaskGenerator() = 0;
};

/** @example facedetect.cpp
*/
/** @brief Cascade classifier class for object detection.
 */
class CC_EXPORTS_W CascadeClassifier
{
public:
    CC_WRAP CascadeClassifier();
    /** @brief Loads a classifier from a file.

    @param filename Name of the file from which the classifier is loaded.
     */
    CC_WRAP CascadeClassifier(const char* filename);
    ~CascadeClassifier();
    /** @brief Checks whether the classifier has been loaded.
    */
    CC_WRAP bool empty() const;
    /** @brief Loads a classifier from a file.

    @param filename Name of the file from which the classifier is loaded. The file may contain an old
    HAAR classifier trained by the haartraining application or a new cascade classifier trained by the
    traincascade application.
     */
    CC_WRAP bool load( const char* filename );
    /** @brief Reads a classifier from a CvFileStorage node.

    @note The file may contain a new cascade classifier (trained traincascade application) only.
     */
    CC_WRAP bool read( const CFileNode& node );

    /** @brief Detects objects of different sizes in the input image. The detected objects are returned as a list
    of rectangles.

    @param image Matrix of the type CC_8U containing an image where objects are detected.
    @param objects Vector of rectangles where each rectangle contains the detected object, the
    rectangles may be partially outside the original image.
    @param scaleFactor Parameter specifying how much the image size is reduced at each image scale.
    @param minNeighbors Parameter specifying how many neighbors each candidate rectangle should have
    to retain it.
    @param flags Parameter with the same meaning for an old cascade as in the function
    cvHaarDetectObjects. It is not used for a new cascade.
    @param minSize Minimum possible object size. Objects smaller than that are ignored.
    @param maxSize Maximum possible object size. Objects larger than that are ignored. If `maxSize == minSize` model is evaluated on single scale.

    The function is parallelized with the TBB library.

    @note
       -   (Python) A face detection example using cascade classifiers can be found at
            opencv_source_code/samples/python/facedetect.py
    */
    CC_WRAP void detectMultiScale( const img_t* image,
                          CC_OUT std::vector<CRect>& objects,
                          double scaleFactor = 1.1,
                          int minNeighbors = 3, int flags = 0,
                          CSize minSize = cSize(0, 0),
                          CSize maxSize = cSize(0, 0) );

    /** @overload
    @param image Matrix of the type CC_8U containing an image where objects are detected.
    @param objects Vector of rectangles where each rectangle contains the detected object, the
    rectangles may be partially outside the original image.
    @param numDetections Vector of detection numbers for the corresponding objects. An object's number
    of detections is the number of neighboring positively classified rectangles that were joined
    together to form the object.
    @param scaleFactor Parameter specifying how much the image size is reduced at each image scale.
    @param minNeighbors Parameter specifying how many neighbors each candidate rectangle should have
    to retain it.
    @param flags Parameter with the same meaning for an old cascade as in the function
    cvHaarDetectObjects. It is not used for a new cascade.
    @param minSize Minimum possible object size. Objects smaller than that are ignored.
    @param maxSize Maximum possible object size. Objects larger than that are ignored. If `maxSize == minSize` model is evaluated on single scale.
    */
    CC_WRAP_AS(detectMultiScale2) void detectMultiScale( const img_t* image,
                          CC_OUT std::vector<CRect>& objects,
                          CC_OUT std::vector<int>& numDetections,
                          double scaleFactor=1.1,
                          int minNeighbors=3, int flags=0,
                          CSize minSize=CSize(),
                          CSize maxSize=CSize() );

    /** @overload
    This function allows you to retrieve the final stage decision certainty of classification.
    For this, one needs to set `outputRejectLevels` on true and provide the `rejectLevels` and `levelWeights` parameter.
    For each resulting detection, `levelWeights` will then contain the certainty of classification at the final stage.
    This value can then be used to separate strong from weaker classifications.

    A code sample on how to use it efficiently can be found below:
    @code
    Mat img;
    vector<double> weights;
    vector<int> levels;
    vector<CRect> detections;
    CascadeClassifier model("/path/to/your/model.xml");
    model.detectMultiScale(img, detections, levels, weights, 1.1, 3, 0, cSize(0, 0), cSize(0, 0), true);
    cerr << "Detection " << detections[0] << " with weight " << weights[0] << endl;
    @endcode
    */
    CC_WRAP_AS(detectMultiScale3) void detectMultiScale( const img_t* image,
                                  CC_OUT std::vector<CRect>& objects,
                                  CC_OUT std::vector<int>& rejectLevels,
                                  CC_OUT std::vector<double>& levelWeights,
                                  double scaleFactor = 1.1,
                                  int minNeighbors = 3, int flags = 0,
                                  CSize minSize = cSize(0, 0),
                                  CSize maxSize = cSize(0, 0),
                                  bool outputRejectLevels = false );

    CC_WRAP bool isOldFormatCascade() const;
    CC_WRAP CSize getOriginalWindowSize() const;
    CC_WRAP int getFeatureType() const;
    void* getOldCascade();

    CC_WRAP static bool convert(const char* oldcascade, const char* newcascade);

    void setMaskGenerator(const BaseCascadeClassifier::MaskGenerator* maskGenerator);
    BaseCascadeClassifier::MaskGenerator* getMaskGenerator();

    BaseCascadeClassifier* cc;
};

CC_EXPORTS BaseCascadeClassifier::MaskGenerator* createFaceDetectionMaskGenerator();

//////////////// HOG (Histogram-of-Oriented-Gradients) Descriptor and Object Detector //////////////

//! struct for detection region of interest (ROI)
struct DetectionROI
{
   //! scale(size) of the bounding box
   double scale;
   //! set of requrested locations to be evaluated
   std::vector<CPoint> locations;
   //! vector that will contain confidence values for each location
   std::vector<double> confidences;
};

/**@example peopledetect.cpp
 */
struct CC_EXPORTS_W HOGDescriptor
{
public:
    enum { L2Hys = 0
         };
    enum { DEFAULT_NLEVELS = 64
         };

    CC_WRAP HOGDescriptor() : nbins(9), derivAperture(1), winSigma(-1),
        histogramNormType(HOGDescriptor::L2Hys), L2HysThreshold(0.2), gammaCorrection(true),
        free_coef(-1.f), nlevels(HOGDescriptor::DEFAULT_NLEVELS), signedGradient(false)
    {winSize = iSIZE(64,128), blockSize = iSIZE(16,16), blockStride = iSIZE(8,8), cellSize = iSIZE(8,8);}

    CC_WRAP HOGDescriptor(CSize _winSize, CSize _blockSize, CSize _blockStride,
                  CSize _cellSize, int _nbins, int _derivAperture=1, double _winSigma=-1,
                  int _histogramNormType=HOGDescriptor::L2Hys,
                  double _L2HysThreshold=0.2, bool _gammaCorrection=false,
                  int _nlevels=HOGDescriptor::DEFAULT_NLEVELS, bool _signedGradient=false)
    : winSize(_winSize), blockSize(_blockSize), blockStride(_blockStride), cellSize(_cellSize),
    nbins(_nbins), derivAperture(_derivAperture), winSigma(_winSigma),
    histogramNormType(_histogramNormType), L2HysThreshold(_L2HysThreshold),
    gammaCorrection(_gammaCorrection), free_coef(-1.f), nlevels(_nlevels), signedGradient(_signedGradient)
    {}

    CC_WRAP HOGDescriptor(const char* filename)
    {
        load(filename);
    }

    HOGDescriptor(const HOGDescriptor& d)
    {
        d.copyTo(*this);
    }

    virtual ~HOGDescriptor() {}

    CC_WRAP size_t getDescriptorSize() const;
    CC_WRAP bool checkDetectorSize() const;
    CC_WRAP double getWinSigma() const;

    CC_WRAP virtual void setSVMDetector(const img_t* _svmdetector);

    virtual bool read(CFileNode& fn);
    virtual void write(CvFileStorage& fs, const char* objname) const;

    CC_WRAP virtual bool load(const char* filename, const char* objname = NULL);
    CC_WRAP virtual void save(const char* filename, const char* objname = NULL) const;
    virtual void copyTo(HOGDescriptor& c) const;

    CC_WRAP virtual void compute(const img_t* img,
                         CC_OUT std::vector<float>& descriptors,
                         CSize winStride = cSize(0, 0), CSize padding = cSize(0, 0),
                         const std::vector<CPoint>& locations = std::vector<CPoint>()) const;

    //! with found weights output
    CC_WRAP virtual void detect(const img_t* img, CC_OUT std::vector<CPoint>& foundLocations,
                        CC_OUT std::vector<double>& weights,
                        double hitThreshold = 0, CSize winStride = cSize(0, 0),
                        CSize padding = cSize(0, 0),
                        const std::vector<CPoint>& searchLocations = std::vector<CPoint>()) const;
    //! without found weights output
    virtual void detect(const img_t* img, CC_OUT std::vector<CPoint>& foundLocations,
                        double hitThreshold = 0, CSize winStride = cSize(0, 0),
                        CSize padding = cSize(0, 0),
                        const std::vector<CPoint>& searchLocations=std::vector<CPoint>()) const;

    //! with result weights output
    CC_WRAP virtual void detectMultiScale(const img_t* img, CC_OUT std::vector<CRect>& foundLocations,
                                  CC_OUT std::vector<double>& foundWeights, double hitThreshold = 0,
                                  CSize winStride = cSize(0, 0), CSize padding = cSize(0, 0), double scale = 1.05,
                                  double finalThreshold = 2.0,bool useMeanshiftGrouping = false) const;
    //! without found weights output
    virtual void detectMultiScale(const img_t* img, CC_OUT std::vector<CRect>& foundLocations,
                                  double hitThreshold = 0, CSize winStride = cSize(0, 0),
                                  CSize padding = cSize(0, 0), double scale = 1.05,
                                  double finalThreshold = 2.0, bool useMeanshiftGrouping = false) const;

    CC_WRAP virtual void computeGradient(const img_t* img, CC_OUT img_t* grad, CC_OUT img_t* angleOfs,
                                 CSize paddingTL = cSize(0, 0), CSize paddingBR = cSize(0, 0)) const;

    CC_WRAP static std::vector<float> getDefaultPeopleDetector();
    CC_WRAP static std::vector<float> getDaimlerPeopleDetector();

    CC_PROP CSize winSize;
    CC_PROP CSize blockSize;
    CC_PROP CSize blockStride;
    CC_PROP CSize cellSize;
    CC_PROP int nbins;
    CC_PROP int derivAperture;
    CC_PROP double winSigma;
    CC_PROP int histogramNormType;
    CC_PROP double L2HysThreshold;
    CC_PROP bool gammaCorrection;
    CC_PROP std::vector<float> svmDetector;
    //UMat oclSvmDetector;
    float free_coef;
    CC_PROP int nlevels;
    CC_PROP bool signedGradient;


    //! evaluate specified ROI and return confidence value for each location
    virtual void detectROI(const img_t* img, const std::vector<CPoint> &locations,
                                   CC_OUT std::vector<CPoint>& foundLocations, CC_OUT std::vector<double>& confidences,
                                   double hitThreshold = 0, CSize winStride = cSize(0, 0),
                                   CSize padding = cSize(0, 0)) const;

    //! evaluate specified ROI and return confidence value for each location in multiple scales
    virtual void detectMultiScaleROI(const img_t* img,
                                                       CC_OUT std::vector<CRect>& foundLocations,
                                                       std::vector<DetectionROI>& locations,
                                                       double hitThreshold = 0,
                                                       int groupThreshold = 0) const;

    //! read/parse Dalal's alt model file
    void readALTModel(str_t modelfile);
    void groupRectangles(std::vector<CRect>& rectList, std::vector<double>& weights, int groupThreshold, double eps) const;
};

//! @} objdetect



#include "detection_based_tracker.hpp"

#ifndef DISABLE_OPENCC_24_COMPATIBILITY
#include "objdetect_c.h"
#endif

#endif
