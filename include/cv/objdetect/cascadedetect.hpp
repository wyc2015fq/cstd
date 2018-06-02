
#include "objdetect.hpp"
#include <vector>




/** @brief Splits an element set into equivalency classes.

The generic function partition implements an \f$O(N^2)\f$ algorithm for splitting a set of \f$N\f$ elements
into one or more equivalency classes, as described in
<http://en.wikipedia.org/wiki/Disjoint-set_data_structure> . The function returns the number of
equivalency classes.
@param _vec Set of elements stored as a vector.
@param labels Output vector of labels. It contains as many elements as vec. Each label labels[i] is
a 0-based cluster index of `vec[i]`.
@param predicate Equivalence predicate (pointer to a boolean function of two arguments or an
instance of the class that has the method bool operator()(const _Tp& a, const _Tp& b) ). The
predicate returns true when the elements are certainly in the same class, and returns false if they
may or may not be in the same class.
@ingroup core_cluster
*/
template<typename _Tp, class _EqPredicate> int
partition( const std::vector<_Tp>& _vec, std::vector<int>& labels,
          _EqPredicate predicate=_EqPredicate())
{
    int i, j, N = (int)_vec.size();
    const _Tp* vec = &_vec[0];

    const int PARENT=0;
    const int RANK=1;

    std::vector<int> _nodes(N*2);
    int (*nodes)[2] = (int(*)[2])&_nodes[0];

    // The first O(N) pass: create N single-vertex trees
    for(i = 0; i < N; i++)
    {
        nodes[i][PARENT]=-1;
        nodes[i][RANK] = 0;
    }

    // The main O(N^2) pass: merge connected components
    for( i = 0; i < N; i++ )
    {
        int root = i;

        // find root
        while( nodes[root][PARENT] >= 0 )
            root = nodes[root][PARENT];

        for( j = 0; j < N; j++ )
        {
            if( i == j || !predicate(vec[i], vec[j]))
                continue;
            int root2 = j;

            while( nodes[root2][PARENT] >= 0 )
                root2 = nodes[root2][PARENT];

            if( root2 != root )
            {
                // unite both trees
                int rank = nodes[root][RANK], rank2 = nodes[root2][RANK];
                if( rank > rank2 )
                    nodes[root2][PARENT] = root;
                else
                {
                    nodes[root][PARENT] = root2;
                    nodes[root2][RANK] += rank == rank2;
                    root = root2;
                }
                CC_Assert( nodes[root][PARENT] < 0 );

                int k = j, parent;

                // compress the path from node2 to root
                while( (parent = nodes[k][PARENT]) >= 0 )
                {
                    nodes[k][PARENT] = root;
                    k = parent;
                }

                // compress the path from node to root
                k = i;
                while( (parent = nodes[k][PARENT]) >= 0 )
                {
                    nodes[k][PARENT] = root;
                    k = parent;
                }
            }
        }
    }

    // Final O(N) pass: enumerate classes
    labels.resize(N);
    int nclasses = 0;

    for( i = 0; i < N; i++ )
    {
        int root = i;
        while( nodes[root][PARENT] >= 0 )
            root = nodes[root][PARENT];
        // re-use the rank as the class label
        if( nodes[root][RANK] >= 0 )
            nodes[root][RANK] = ~nclasses++;
        labels[i] = ~nodes[root][RANK];
    }

    return nclasses;
}

//#include "../core/ocl.hpp"

void clipObjects(CSize sz, std::vector<CRect>& objects,
                 std::vector<int>* a, std::vector<double>* b);

class FeatureEvaluator
{
public:
    enum
    {
        HAAR = 0,
        LBP  = 1,
        HOG  = 2
    };

    struct ScaleData
    {
        ScaleData() { scale = 0.f; layer_ofs = ystep = 0; }
        CSize getWorkingSize(CSize winSize) const
        {
            return cSize(MAX(szi.width - winSize.width, 0),
                        MAX(szi.height - winSize.height, 0));
        }

        float scale;
        CSize szi;
        int layer_ofs, ystep;
    };

    FeatureEvaluator() {
      BZERO(sbuf);
      BZERO(rbuf);
    }
    virtual ~FeatureEvaluator() {
      imfree(sbuf);
      imfree(rbuf);
    }

    virtual bool read(const CFileNode* node, CSize origWinSize);
    virtual FeatureEvaluator* clone() const;
    virtual int getFeatureType() const;
    int getNumChannels() const { return nchannels; }

    virtual bool setImage(const img_t* img, const std::vector<float>& scales);
    virtual bool setWindow(CPoint p, int scaleIdx);
    const ScaleData& getScaleData(int scaleIdx) const
    {
        CC_Assert( 0 <= scaleIdx && scaleIdx < (int)scaleData->size());
        return scaleData->at(scaleIdx);
    }
    //virtual void getUMats(std::vector<UMat>& bufs);
    //virtual void getMats();

    CSize getLocalSize() const { return localSize; }
    CSize getLocalBufSize() const { return lbufSize; }

    virtual float calcOrd(int featureIdx) const;
    virtual int calcCat(int featureIdx) const;

    static FeatureEvaluator* create(int type);

protected:
    enum { SBUF_VALID=1, USBUF_VALID=2 };
    int sbufFlag;

    bool updateScaleData( CSize imgsz, const std::vector<float>& _scales );
    virtual void computeChannels( int, const img_t* ) {}
    virtual void computeOptFeatures() {}

    CSize origWinSize, sbufSize, localSize, lbufSize;
    int nchannels;
    img_t sbuf[1];
    img_t rbuf[1];
    //UMat urbuf, usbuf, ufbuf, uscaleData;

    std::vector<ScaleData>* scaleData;
};


class CascadeClassifierImpl : public BaseCascadeClassifier
{
public:
    CascadeClassifierImpl();
    virtual ~CascadeClassifierImpl();

    bool empty() const;
    bool load( const char* filename );
    void read( const CFileNode* node );
    bool read_( const CFileNode* node );
    void detectMultiScale( const img_t* image,
                          CC_OUT std::vector<CRect>& objects,
                          double scaleFactor = 1.1,
                          int minNeighbors = 3, int flags = 0,
                          CSize minSize = cSize(0, 0),
                          CSize maxSize = cSize(0, 0) );

    void detectMultiScale( const img_t* image,
                          CC_OUT std::vector<CRect>& objects,
                          CC_OUT std::vector<int>& numDetections,
                          double scaleFactor=1.1,
                          int minNeighbors=3, int flags=0,
                          CSize minSize=CSize(),
                          CSize maxSize=CSize() );

    void detectMultiScale( const img_t* image,
                          CC_OUT std::vector<CRect>& objects,
                          CC_OUT std::vector<int>& rejectLevels,
                          CC_OUT std::vector<double>& levelWeights,
                          double scaleFactor = 1.1,
                          int minNeighbors = 3, int flags = 0,
                          CSize minSize = cSize(0, 0),
                          CSize maxSize = cSize(0, 0),
                          bool outputRejectLevels = false );


    bool isOldFormatCascade() const;
    CSize getOriginalWindowSize() const;
    int getFeatureType() const;
    void* getOldCascade();

    void setMaskGenerator(const MaskGenerator* maskGenerator);
    MaskGenerator* getMaskGenerator();

protected:
    enum { SUM_ALIGN = 64 };

    bool detectSingleScale( const img_t* image, CSize processingRectSize,
                            int yStep, double factor, std::vector<CRect>& candidates,
                            std::vector<int>& rejectLevels, std::vector<double>& levelWeights,
                            CSize sumSize0, bool outputRejectLevels = false );
#ifdef HAVE_OPENCL
    bool ocl_detectMultiScaleNoGrouping( const std::vector<float>& scales,
                                         std::vector<CRect>& candidates );
#endif
    void detectMultiScaleNoGrouping( const img_t* image, std::vector<CRect>& candidates,
                                    std::vector<int>& rejectLevels, std::vector<double>& levelWeights,
                                    double scaleFactor, CSize minObjectSize, CSize maxObjectSize,
                                    bool outputRejectLevels = false );

    enum { MAX_FACES = 10000 };
    enum { BOOST = 0 };
    enum { DO_CANNY_PRUNING    = CASCADE_DO_CANNY_PRUNING,
        SCALE_IMAGE         = CASCADE_SCALE_IMAGE,
        FIND_BIGGEST_OBJECT = CASCADE_FIND_BIGGEST_OBJECT,
        DO_ROUGH_SEARCH     = CASCADE_DO_ROUGH_SEARCH
    };

    friend class CascadeClassifierInvoker;
    friend class SparseCascadeClassifierInvoker;

    template<class FEval>
    friend int predictOrdered( CascadeClassifierImpl& cascade, FeatureEvaluator* featureEvaluator, double& weight);

    template<class FEval>
    friend int predictCategorical( CascadeClassifierImpl& cascade, FeatureEvaluator* featureEvaluator, double& weight);

    template<class FEval>
    friend int predictOrderedStump( CascadeClassifierImpl& cascade, FeatureEvaluator* featureEvaluator, double& weight);

    template<class FEval>
    friend int predictCategoricalStump( CascadeClassifierImpl& cascade, FeatureEvaluator* featureEvaluator, double& weight);

    int runAt( FeatureEvaluator*& feval, CPoint pt, int scaleIdx, double& weight );

    class Data
    {
    public:
        struct DTreeNode
        {
            int featureIdx;
            float threshold; // for ordered features only
            int left;
            int right;
        };

        struct DTree
        {
            int nodeCount;
        };

        struct Stage
        {
            int first;
            int ntrees;
            float threshold;
        };

        struct Stump
        {
            Stump() : featureIdx(0), threshold(0), left(0), right(0) { }
            Stump(int _featureIdx, float _threshold, float _left, float _right)
            : featureIdx(_featureIdx), threshold(_threshold), left(_left), right(_right) {}

            int featureIdx;
            float threshold;
            float left;
            float right;
        };

        Data();

        bool read(const CFileNode* node);

        int stageType;
        int featureType;
        int ncategories;
        int minNodesPerTree, maxNodesPerTree;
        CSize origWinSize;

        std::vector<Stage> stages;
        std::vector<DTree> classifiers;
        std::vector<DTreeNode> nodes;
        std::vector<float> leaves;
        std::vector<int> subsets;
        std::vector<Stump> stumps;
    };

    Data data;
    FeatureEvaluator* featureEvaluator;
    CvHaarClassifierCascade* oldCascade;

    MaskGenerator* maskGenerator;
    //UMat ugrayImage;
    //UMat ufacepos, ustages, unodes, uleaves, usubsets;
#ifdef HAVE_OPENCL
    ocl::Kernel haarKernel, lbpKernel;
    bool tryOpenCL;
#endif

    //Mutex mtx;
};

#define CC_CASCADE_PARAMS "cascadeParams"
#define CC_STAGE_TYPE     "stageType"
#define CC_FEATURE_TYPE   "featureType"
#define CC_HEIGHT         "height"
#define CC_WIDTH          "width"

#define CC_STAGE_NUM    "stageNum"
#define CC_STAGES       "stages"
#define CC_STAGE_PARAMS "stageParams"

#define CC_BOOST            "BOOST"
#define CC_MAX_DEPTH        "maxDepth"
#define CC_WEAK_COUNT       "maxWeakCount"
#define CC_STAGE_THRESHOLD  "stageThreshold"
#define CC_WEAK_CLASSIFIERS "weakClassifiers"
#define CC_INTERNAL_NODES   "internalNodes"
#define CC_LEAF_VALUES      "leafValues"

#define CC_FEATURES       "features"
#define CC_FEATURE_PARAMS "featureParams"
#define CC_MAX_CAT_COUNT  "maxCatCount"

#define CC_HAAR   "HAAR"
#define CC_RECTS  "rects"
#define CC_TILTED "tilted"

#define CC_LBP  "LBP"
#define CC_RECT "rect"

#define CC_HOG  "HOG"

#define CC_SUM_PTRS( p0, p1, p2, p3, sum, rect, step )                    \
    /* (x, y) */                                                          \
    (p0) = sum + (rect).x + (step) * (rect).y,                            \
    /* (x + w, y) */                                                      \
    (p1) = sum + (rect).x + (rect).width + (step) * (rect).y,             \
    /* (x + w, y) */                                                      \
    (p2) = sum + (rect).x + (step) * ((rect).y + (rect).height),          \
    /* (x + w, y + h) */                                                  \
    (p3) = sum + (rect).x + (rect).width + (step) * ((rect).y + (rect).height)

#define CC_TILTED_PTRS( p0, p1, p2, p3, tilted, rect, step )                        \
    /* (x, y) */                                                                    \
    (p0) = tilted + (rect).x + (step) * (rect).y,                                   \
    /* (x - h, y + h) */                                                            \
    (p1) = tilted + (rect).x - (rect).height + (step) * ((rect).y + (rect).height), \
    /* (x + w, y + w) */                                                            \
    (p2) = tilted + (rect).x + (rect).width + (step) * ((rect).y + (rect).width),   \
    /* (x + w - h, y + w + h) */                                                    \
    (p3) = tilted + (rect).x + (rect).width - (rect).height                         \
           + (step) * ((rect).y + (rect).width + (rect).height)

#define CALC_SUM_(p0, p1, p2, p3, offset) \
    ((p0)[offset] - (p1)[offset] - (p2)[offset] + (p3)[offset])

#define CALC_SUM(rect,offset) CALC_SUM_((rect)[0], (rect)[1], (rect)[2], (rect)[3], offset)

#define CC_SUM_OFS( p0, p1, p2, p3, sum, rect, step )                 \
/* (x, y) */                                                          \
(p0) = sum + (rect).x + (step) * (rect).y,                            \
/* (x + w, y) */                                                      \
(p1) = sum + (rect).x + (rect).width + (step) * (rect).y,             \
/* (x + w, y) */                                                      \
(p2) = sum + (rect).x + (step) * ((rect).y + (rect).height),          \
/* (x + w, y + h) */                                                  \
(p3) = sum + (rect).x + (rect).width + (step) * ((rect).y + (rect).height)

#define CC_TILTED_OFS( p0, p1, p2, p3, tilted, rect, step )                     \
/* (x, y) */                                                                    \
(p0) = tilted + (rect).x + (step) * (rect).y,                                   \
/* (x - h, y + h) */                                                            \
(p1) = tilted + (rect).x - (rect).height + (step) * ((rect).y + (rect).height), \
/* (x + w, y + w) */                                                            \
(p2) = tilted + (rect).x + (rect).width + (step) * ((rect).y + (rect).width),   \
/* (x + w - h, y + w + h) */                                                    \
(p3) = tilted + (rect).x + (rect).width - (rect).height                         \
+ (step) * ((rect).y + (rect).width + (rect).height)

#define CALC_SUM_(p0, p1, p2, p3, offset) \
((p0)[offset] - (p1)[offset] - (p2)[offset] + (p3)[offset])

#define CALC_SUM(rect,offset) CALC_SUM_((rect)[0], (rect)[1], (rect)[2], (rect)[3], offset)

#define CALC_SUM_OFS_(p0, p1, p2, p3, ptr) \
((ptr)[p0] - (ptr)[p1] - (ptr)[p2] + (ptr)[p3])

#define CALC_SUM_OFS(rect, ptr) CALC_SUM_OFS_((rect)[0], (rect)[1], (rect)[2], (rect)[3], ptr)

//----------------------------------------------  HaarEvaluator ---------------------------------------
class HaarEvaluator : public FeatureEvaluator
{
public:
    struct Feature
    {
        Feature();
        bool read( const CFileNode* node );

        bool tilted;

        enum { RECT_NUM = 3 };
        struct
        {
            CRect r;
            float weight;
        } rect[RECT_NUM];
    };

    struct OptFeature
    {
        OptFeature();

        enum { RECT_NUM = Feature::RECT_NUM };
        float calc( const int* pwin ) const;
        void setOffsets( const Feature& _f, int step, int tofs );

        int ofs[RECT_NUM][4];
        float weight[4];
    };

    HaarEvaluator();
    virtual ~HaarEvaluator();

    virtual bool read( const CFileNode* node, CSize origWinSize);
    virtual FeatureEvaluator* clone() const;
    virtual int getFeatureType() const { return FeatureEvaluator::HAAR; }

    virtual bool setWindow(CPoint p, int scaleIdx);
    CRect getNormRect() const;
    int getSquaresOffset() const;

    float operator()(int featureIdx) const
    { return optfeaturesPtr[featureIdx].calc(pwin) * varianceNormFactor; }
    virtual float calcOrd(int featureIdx) const
    { return (*this)(featureIdx); }

protected:
    virtual void computeChannels( int i, const img_t* img );
    virtual void computeOptFeatures();

    std::vector<Feature>* features;
    std::vector<OptFeature>* optfeatures;
    std::vector<OptFeature>* optfeatures_lbuf;
    bool hasTiltedFeatures;

    int tofs, sqofs;
    vec4i nofs;
    CRect normrect;
    const int* pwin;
    OptFeature* optfeaturesPtr; // optimization
    float varianceNormFactor;
};

inline HaarEvaluator::Feature :: Feature()
{
    tilted = false;
    rect[0].r = rect[1].r = rect[2].r = CRect();
    rect[0].weight = rect[1].weight = rect[2].weight = 0;
}

inline HaarEvaluator::OptFeature :: OptFeature()
{
    weight[0] = weight[1] = weight[2] = 0.f;

    ofs[0][0] = ofs[0][1] = ofs[0][2] = ofs[0][3] =
    ofs[1][0] = ofs[1][1] = ofs[1][2] = ofs[1][3] =
    ofs[2][0] = ofs[2][1] = ofs[2][2] = ofs[2][3] = 0;
}

inline float HaarEvaluator::OptFeature :: calc( const int* ptr ) const
{
    float ret = weight[0] * CALC_SUM_OFS(ofs[0], ptr) +
                weight[1] * CALC_SUM_OFS(ofs[1], ptr);

    if( weight[2] != 0.0f )
        ret += weight[2] * CALC_SUM_OFS(ofs[2], ptr);

    return ret;
}

//----------------------------------------------  LBPEvaluator -------------------------------------
class LBPEvaluator : public FeatureEvaluator
{
public:
    struct Feature
    {
        Feature();
        Feature( int x, int y, int _block_w, int _block_h  )
                 {rect = cRect(x, y, _block_w, _block_h);}

        bool read(const CFileNode* node );

        CRect rect; // weight and height for block
    };

    struct OptFeature
    {
        OptFeature();

        int calc( const int* pwin ) const;
        void setOffsets( const Feature& _f, int step );
        int ofs[16];
    };

    LBPEvaluator();
    virtual ~LBPEvaluator();

    virtual bool read( const CFileNode* node, CSize origWinSize );
    virtual FeatureEvaluator* clone() const;
    virtual int getFeatureType() const { return FeatureEvaluator::LBP; }

    virtual bool setWindow(CPoint p, int scaleIdx);

    int operator()(int featureIdx) const
    { return optfeaturesPtr[featureIdx].calc(pwin); }
    virtual int calcCat(int featureIdx) const
    { return (*this)(featureIdx); }
protected:
    virtual void computeChannels( int i, const img_t* img );
    virtual void computeOptFeatures();

    std::vector<Feature>* features;
    std::vector<OptFeature>* optfeatures;
    std::vector<OptFeature>* optfeatures_lbuf;
    OptFeature* optfeaturesPtr; // optimization

    const int* pwin;
};


inline LBPEvaluator::Feature :: Feature()
{
    rect = CRect();
}

inline LBPEvaluator::OptFeature :: OptFeature()
{
    for( int i = 0; i < 16; i++ )
        ofs[i] = 0;
}

inline int LBPEvaluator::OptFeature :: calc( const int* p ) const
{
    int cval = CALC_SUM_OFS_( ofs[5], ofs[6], ofs[9], ofs[10], p );

    return (CALC_SUM_OFS_( ofs[0], ofs[1], ofs[4], ofs[5], p ) >= cval ? 128 : 0) |   // 0
           (CALC_SUM_OFS_( ofs[1], ofs[2], ofs[5], ofs[6], p ) >= cval ? 64 : 0) |    // 1
           (CALC_SUM_OFS_( ofs[2], ofs[3], ofs[6], ofs[7], p ) >= cval ? 32 : 0) |    // 2
           (CALC_SUM_OFS_( ofs[6], ofs[7], ofs[10], ofs[11], p ) >= cval ? 16 : 0) |  // 5
           (CALC_SUM_OFS_( ofs[10], ofs[11], ofs[14], ofs[15], p ) >= cval ? 8 : 0)|  // 8
           (CALC_SUM_OFS_( ofs[9], ofs[10], ofs[13], ofs[14], p ) >= cval ? 4 : 0)|   // 7
           (CALC_SUM_OFS_( ofs[8], ofs[9], ofs[12], ofs[13], p ) >= cval ? 2 : 0)|    // 6
           (CALC_SUM_OFS_( ofs[4], ofs[5], ofs[8], ofs[9], p ) >= cval ? 1 : 0);
}


//----------------------------------------------  predictor functions -------------------------------------

template<class FEval>
inline int predictOrdered( CascadeClassifierImpl& cascade,
                           FeatureEvaluator* _featureEvaluator, double& sum )
{
    CC_INSTRUMENT_REGION()

    int nstages = (int)cascade.data.stages.size();
    int nodeOfs = 0, leafOfs = 0;
    FEval& featureEvaluator = (FEval&)*_featureEvaluator;
    float* cascadeLeaves = &cascade.data.leaves[0];
    CascadeClassifierImpl::Data::DTreeNode* cascadeNodes = &cascade.data.nodes[0];
    CascadeClassifierImpl::Data::DTree* cascadeWeaks = &cascade.data.classifiers[0];
    CascadeClassifierImpl::Data::Stage* cascadeStages = &cascade.data.stages[0];

    for( int si = 0; si < nstages; si++ )
    {
        CascadeClassifierImpl::Data::Stage& stage = cascadeStages[si];
        int wi, ntrees = stage.ntrees;
        sum = 0;

        for( wi = 0; wi < ntrees; wi++ )
        {
            CascadeClassifierImpl::Data::DTree& weak = cascadeWeaks[stage.first + wi];
            int idx = 0, root = nodeOfs;

            do
            {
                CascadeClassifierImpl::Data::DTreeNode& node = cascadeNodes[root + idx];
                double val = featureEvaluator(node.featureIdx);
                idx = val < node.threshold ? node.left : node.right;
            }
            while( idx > 0 );
            sum += cascadeLeaves[leafOfs - idx];
            nodeOfs += weak.nodeCount;
            leafOfs += weak.nodeCount + 1;
        }
        if( sum < stage.threshold )
            return -si;
    }
    return 1;
}

template<class FEval>
inline int predictCategorical( CascadeClassifierImpl& cascade,
                               FeatureEvaluator* _featureEvaluator, double& sum )
{
    CC_INSTRUMENT_REGION()

    int nstages = (int)cascade.data.stages.size();
    int nodeOfs = 0, leafOfs = 0;
    FEval& featureEvaluator = (FEval&)*_featureEvaluator;
    size_t subsetSize = (cascade.data.ncategories + 31)/32;
    int* cascadeSubsets = &cascade.data.subsets[0];
    float* cascadeLeaves = &cascade.data.leaves[0];
    CascadeClassifierImpl::Data::DTreeNode* cascadeNodes = &cascade.data.nodes[0];
    CascadeClassifierImpl::Data::DTree* cascadeWeaks = &cascade.data.classifiers[0];
    CascadeClassifierImpl::Data::Stage* cascadeStages = &cascade.data.stages[0];

    for(int si = 0; si < nstages; si++ )
    {
        CascadeClassifierImpl::Data::Stage& stage = cascadeStages[si];
        int wi, ntrees = stage.ntrees;
        sum = 0;

        for( wi = 0; wi < ntrees; wi++ )
        {
            CascadeClassifierImpl::Data::DTree& weak = cascadeWeaks[stage.first + wi];
            int idx = 0, root = nodeOfs;
            do
            {
                CascadeClassifierImpl::Data::DTreeNode& node = cascadeNodes[root + idx];
                int c = featureEvaluator(node.featureIdx);
                const int* subset = &cascadeSubsets[(root + idx)*subsetSize];
                idx = (subset[c>>5] & (1 << (c & 31))) ? node.left : node.right;
            }
            while( idx > 0 );
            sum += cascadeLeaves[leafOfs - idx];
            nodeOfs += weak.nodeCount;
            leafOfs += weak.nodeCount + 1;
        }
        if( sum < stage.threshold )
            return -si;
    }
    return 1;
}

template<class FEval>
inline int predictOrderedStump( CascadeClassifierImpl& cascade,
                                FeatureEvaluator* _featureEvaluator, double& sum )
{
    CC_INSTRUMENT_REGION()

    CC_Assert(!cascade.data.stumps.empty());
    FEval& featureEvaluator = (FEval&)*_featureEvaluator;
    const CascadeClassifierImpl::Data::Stump* cascadeStumps = &cascade.data.stumps[0];
    const CascadeClassifierImpl::Data::Stage* cascadeStages = &cascade.data.stages[0];

    int nstages = (int)cascade.data.stages.size();
    double tmp = 0;

    for( int stageIdx = 0; stageIdx < nstages; stageIdx++ )
    {
        const CascadeClassifierImpl::Data::Stage& stage = cascadeStages[stageIdx];
        tmp = 0;

        int ntrees = stage.ntrees;
        for( int i = 0; i < ntrees; i++ )
        {
            const CascadeClassifierImpl::Data::Stump& stump = cascadeStumps[i];
            double value = featureEvaluator(stump.featureIdx);
            tmp += value < stump.threshold ? stump.left : stump.right;
        }

        if( tmp < stage.threshold )
        {
            sum = (double)tmp;
            return -stageIdx;
        }
        cascadeStumps += ntrees;
    }

    sum = (double)tmp;
    return 1;
}

template<class FEval>
inline int predictCategoricalStump( CascadeClassifierImpl& cascade,
                                    FeatureEvaluator* _featureEvaluator, double& sum )
{
    CC_INSTRUMENT_REGION()

    CC_Assert(!cascade.data.stumps.empty());
    int nstages = (int)cascade.data.stages.size();
    FEval& featureEvaluator = (FEval&)*_featureEvaluator;
    size_t subsetSize = (cascade.data.ncategories + 31)/32;
    const int* cascadeSubsets = &cascade.data.subsets[0];
    const CascadeClassifierImpl::Data::Stump* cascadeStumps = &cascade.data.stumps[0];
    const CascadeClassifierImpl::Data::Stage* cascadeStages = &cascade.data.stages[0];

    double tmp = 0;
    for( int si = 0; si < nstages; si++ )
    {
        const CascadeClassifierImpl::Data::Stage& stage = cascadeStages[si];
        int wi, ntrees = stage.ntrees;
        tmp = 0;

        for( wi = 0; wi < ntrees; wi++ )
        {
            const CascadeClassifierImpl::Data::Stump& stump = cascadeStumps[wi];
            int c = featureEvaluator(stump.featureIdx);
            const int* subset = &cascadeSubsets[wi*subsetSize];
            tmp += (subset[c>>5] & (1 << (c & 31))) ? stump.left : stump.right;
        }

        if( tmp < stage.threshold )
        {
            sum = tmp;
            return -si;
        }

        cascadeStumps += ntrees;
        cascadeSubsets += ntrees*subsetSize;
    }

    sum = (double)tmp;
    return 1;
}

