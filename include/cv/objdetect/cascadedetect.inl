
#include "cascadedetect.hpp"


//#include "objdetect_c.h"
//#include "opencl_kernels_objdetect.hpp"


#if 0
template<typename _Tp> void copyVectorToUMat(const std::vector<_Tp>& v, UMat& um)
{
    if(v.empty())
        um.release();
    Mat(1, (int)(v.size()*sizeof(v[0])), CC_8U, (void*)&v[0]).copyTo(um);
}
#endif

void groupRectangles(std::vector<CRect>& rectList, int groupThreshold, double eps,
                     std::vector<int>* weights, std::vector<double>* levelWeights)
{
    CC_INSTRUMENT_REGION()

    if( groupThreshold <= 0 || rectList.empty() )
    {
        if( weights )
        {
            size_t i, sz = rectList.size();
            weights->resize(sz);
            for( i = 0; i < sz; i++ )
                (*weights)[i] = 1;
        }
        return;
    }

    std::vector<int> labels;
    int nclasses = partition(rectList, labels, SimilarRects(eps));

    std::vector<CRect> rrects(nclasses);
    std::vector<int> rweights(nclasses, 0);
    std::vector<int> rejectLevels(nclasses, 0);
    std::vector<double> rejectWeights(nclasses, DBL_MIN);
    int i, j, nlabels = (int)labels.size();
    for( i = 0; i < nlabels; i++ )
    {
        int cls = labels[i];
        rrects[cls].x += rectList[i].x;
        rrects[cls].y += rectList[i].y;
        rrects[cls].width += rectList[i].width;
        rrects[cls].height += rectList[i].height;
        rweights[cls]++;
    }

    bool useDefaultWeights = false;

    if ( levelWeights && weights && !weights->empty() && !levelWeights->empty() )
    {
        for( i = 0; i < nlabels; i++ )
        {
            int cls = labels[i];
            if( (*weights)[i] > rejectLevels[cls] )
            {
                rejectLevels[cls] = (*weights)[i];
                rejectWeights[cls] = (*levelWeights)[i];
            }
            else if( ( (*weights)[i] == rejectLevels[cls] ) && ( (*levelWeights)[i] > rejectWeights[cls] ) )
                rejectWeights[cls] = (*levelWeights)[i];
        }
    }
    else
        useDefaultWeights = true;

    for( i = 0; i < nclasses; i++ )
    {
        CRect r = rrects[i];
        float s = 1.f/rweights[i];
        saturate_cast(rrects[i].x, r.x*s);
        saturate_cast(rrects[i].y, r.y*s);
        saturate_cast(rrects[i].width, r.width*s);
        saturate_cast(rrects[i].height, r.height*s);
    }

    rectList.clear();
    if( weights )
        weights->clear();
    if( levelWeights )
        levelWeights->clear();

    for( i = 0; i < nclasses; i++ )
    {
        CRect r1 = rrects[i];
        int n1 = rweights[i];
        double w1 = rejectWeights[i];
        int l1 = rejectLevels[i];

        // filter out rectangles which don't have enough similar rectangles
        if( n1 <= groupThreshold )
            continue;
        // filter out small face rectangles inside large rectangles
        for( j = 0; j < nclasses; j++ )
        {
            int n2 = rweights[j];

            if( j == i || n2 <= groupThreshold )
                continue;
            CRect r2 = rrects[j];

            int dx, dy;
            saturate_cast( dx, r2.width * eps );
            saturate_cast( dy, r2.height * eps );

            if( i != j &&
                r1.x >= r2.x - dx &&
                r1.y >= r2.y - dy &&
                r1.x + r1.width <= r2.x + r2.width + dx &&
                r1.y + r1.height <= r2.y + r2.height + dy &&
                (n2 > MAX(3, n1) || n1 < 3) )
                break;
        }

        if( j == nclasses )
        {
            rectList.push_back(r1);
            if( weights )
                weights->push_back(useDefaultWeights ? n1 : l1);
            if( levelWeights )
                levelWeights->push_back(w1);
        }
    }
}

class MeanshiftGrouping
{
public:
    MeanshiftGrouping(const CPoint3d& densKer, const std::vector<CPoint3d>& posV,
        const std::vector<double>& wV, double eps, int maxIter = 20)
    {
        densityKernel = densKer;
        weightsV = wV;
        positionsV = posV;
        positionsCount = (int)posV.size();
        meanshiftV.resize(positionsCount);
        distanceV.resize(positionsCount);
        iterMax = maxIter;
        modeEps = eps;

        for (unsigned i = 0; i<positionsV.size(); i++)
        {
            meanshiftV[i] = getNewValue(positionsV[i]);
            distanceV[i] = moveToMode(meanshiftV[i]);
            meanshiftV[i] -= positionsV[i];
        }
    }

    void getModes(std::vector<CPoint3d>& modesV, std::vector<double>& resWeightsV, const double eps)
    {
        size_t i, j;
        for (i=0; i <distanceV.size(); i++)
        {
            bool is_found = false;
            for(j=0; j<modesV.size(); j++)
            {
                if ( getDistance(distanceV[i], modesV[j]) < eps)
                {
                    is_found=true;
                    break;
                }
            }
            if (!is_found)
            {
                modesV.push_back(distanceV[i]);
            }
        }

        resWeightsV.resize(modesV.size());

        for (i=0; i<modesV.size(); i++)
        {
            resWeightsV[i] = getResultWeight(modesV[i]);
        }
    }

protected:
    std::vector<CPoint3d> positionsV;
    std::vector<double> weightsV;

    CPoint3d densityKernel;
    int positionsCount;

    std::vector<CPoint3d> meanshiftV;
    std::vector<CPoint3d> distanceV;
    int iterMax;
    double modeEps;

    CPoint3d getNewValue(const CPoint3d& inPt) const
    {
        CPoint3d resPoint = {.0};
        CPoint3d ratPoint = {.0};
        size_t i;
        for (i=0; i<positionsV.size(); i++)
        {
            CPoint3d aPt= positionsV[i];
            CPoint3d bPt = inPt;
            CPoint3d sPt = densityKernel;

            sPt.x *= exp(aPt.z);
            sPt.y *= exp(aPt.z);

            aPt.x /= sPt.x;
            aPt.y /= sPt.y;
            aPt.z /= sPt.z;

            bPt.x /= sPt.x;
            bPt.y /= sPt.y;
            bPt.z /= sPt.z;

            double w = (weightsV[i])*exp(-(dot(aPt-bPt, aPt-bPt))/2)/sqrt(dot(sPt, cPoint3d(1,1,1)));

            resPoint += w*aPt;

            ratPoint.x += w/sPt.x;
            ratPoint.y += w/sPt.y;
            ratPoint.z += w/sPt.z;
        }
        resPoint.x /= ratPoint.x;
        resPoint.y /= ratPoint.y;
        resPoint.z /= ratPoint.z;
        return resPoint;
    }

    double getResultWeight(const CPoint3d& inPt) const
    {
        double sumW=0;
        for (size_t i=0; i<positionsV.size(); i++)
        {
            CPoint3d aPt = positionsV[i];
            CPoint3d sPt = densityKernel;

            sPt.x *= exp(aPt.z);
            sPt.y *= exp(aPt.z);

            aPt -= inPt;

            aPt.x /= sPt.x;
            aPt.y /= sPt.y;
            aPt.z /= sPt.z;

            sumW+=(weightsV[i])*exp(-(dot(aPt, aPt))/2)/sqrt(dot(sPt, cPoint3d(1,1,1)));
        }
        return sumW;
    }

    CPoint3d moveToMode(CPoint3d aPt) const
    {
        CPoint3d bPt;
        for (int i = 0; i<iterMax; i++)
        {
            bPt = aPt;
            aPt = getNewValue(bPt);
            if ( getDistance(aPt, bPt) <= modeEps )
            {
                break;
            }
        }
        return aPt;
    }

    double getDistance(CPoint3d p1, CPoint3d p2) const
    {
        CPoint3d ns = densityKernel;
        ns.x *= exp(p2.z);
        ns.y *= exp(p2.z);
        p2 -= p1;
        p2.x /= ns.x;
        p2.y /= ns.y;
        p2.z /= ns.z;
        return dot(p2, p2);
    }
};

//new grouping function with using meanshift
static void groupRectangles_meanshift(std::vector<CRect>& rectList, double detectThreshold, std::vector<double>& foundWeights,
                                      std::vector<double>& scales, CSize winDetSize)
{
    int detectionCount = (int)rectList.size();
    std::vector<CPoint3d> hits(detectionCount), resultHits;
    std::vector<double> hitWeights(detectionCount), resultWeights;
    CPoint2d hitCenter;
    int i;

    for (i=0; i < detectionCount; i++)
    {
        hitWeights[i] = foundWeights[i];
        hitCenter = (tl(rectList[i]) + br(rectList[i]))*(0.5); //center of rectangles
        hits[i] = cPoint3d(hitCenter.x, hitCenter.y, log(scales[i]));
    }

    rectList.clear();
    foundWeights.clear();

    double logZ = log(1.3);
    CPoint3d smothing = {8, 16, logZ};

    MeanshiftGrouping msGrouping(smothing, hits, hitWeights, 1e-5, 100);

    msGrouping.getModes(resultHits, resultWeights, 1);

    for (i=0; i < resultHits.size(); ++i)
    {

        double scale = exp(resultHits[i].z);
        hitCenter.x = resultHits[i].x;
        hitCenter.y = resultHits[i].y;
        CSize s;
        s.width = (int)(winDetSize.width * scale);
        s.height = (int)(winDetSize.height * scale);
        CRect resultRect = cRect( int(hitCenter.x-s.width/2), int(hitCenter.y-s.height/2),
            int(s.width), int(s.height) );

        if (resultWeights[i] > detectThreshold)
        {
            rectList.push_back(resultRect);
            foundWeights.push_back(resultWeights[i]);
        }
    }
}

void groupRectangles(std::vector<CRect>& rectList, int groupThreshold, double eps)
{
    CC_INSTRUMENT_REGION()

    groupRectangles(rectList, groupThreshold, eps, 0, 0);
}

void groupRectangles(std::vector<CRect>& rectList, std::vector<int>& weights, int groupThreshold, double eps)
{
    CC_INSTRUMENT_REGION()

    groupRectangles(rectList, groupThreshold, eps, &weights, 0);
}
//used for cascade detection algorithm for ROC-curve calculating
void groupRectangles(std::vector<CRect>& rectList, std::vector<int>& rejectLevels,
                     std::vector<double>& levelWeights, int groupThreshold, double eps)
{
    CC_INSTRUMENT_REGION()

    groupRectangles(rectList, groupThreshold, eps, &rejectLevels, &levelWeights);
}
//can be used for HOG detection algorithm only
void groupRectangles_meanshift(std::vector<CRect>& rectList, std::vector<double>& foundWeights,
                               std::vector<double>& foundScales, double detectThreshold, CSize winDetSize)
{
    CC_INSTRUMENT_REGION()

    groupRectangles_meanshift(rectList, detectThreshold, foundWeights, foundScales, winDetSize);
}


bool FeatureEvaluator::read(const CFileNode*, CSize _origWinSize)
{
    origWinSize = _origWinSize;
    localSize = lbufSize = cSize(0, 0);
    if (NULL == scaleData)
        scaleData = new std::vector<ScaleData>();
    else
        scaleData->clear();
    return true;
}

FeatureEvaluator* FeatureEvaluator::clone() const { return new FeatureEvaluator(); }
int FeatureEvaluator::getFeatureType() const {return -1;}
bool FeatureEvaluator::setWindow(CPoint, int) { return true; }
#if 0
void FeatureEvaluator::getUMats(std::vector<UMat>& bufs)
{
    if (!(sbufFlag & USBUF_VALID))
    {
        sbuf->copyTo(usbuf);
        sbufFlag |= USBUF_VALID;
    }

    bufs.clear();
    bufs.push_back(uscaleData);
    bufs.push_back(usbuf);
    bufs.push_back(ufbuf);
}

void FeatureEvaluator::getMats()
{
    if (!(sbufFlag & SBUF_VALID))
    {
        usbuf->copyTo(sbuf);
        sbufFlag |= SBUF_VALID;
    }
}
#endif

float FeatureEvaluator::calcOrd(int) const { return 0.; }
int FeatureEvaluator::calcCat(int) const { return 0; }

bool FeatureEvaluator::updateScaleData( CSize imgsz, const std::vector<float>& _scales )
{
    if( NULL==scaleData )
        scaleData = new std::vector<ScaleData>();

    size_t i, nscales = _scales.size();
    bool recalcOptFeatures = nscales != scaleData->size();
    scaleData->resize(nscales);

    int layer_dy = 0;
    CPoint layer_ofs = cPoint(0,0);
    CSize prevBufSize = sbufSize;
    sbufSize.width = MAX(sbufSize.width, (int)alignSize(cRound(imgsz.width/_scales[0]) + 31, 32));
    recalcOptFeatures = recalcOptFeatures || sbufSize.width != prevBufSize.width;

    for( i = 0; i < nscales; i++ )
    {
        FeatureEvaluator::ScaleData& s = scaleData->at(i);
        if( !recalcOptFeatures && fabs(s.scale - _scales[i]) > FLT_EPSILON*100*_scales[i] )
            recalcOptFeatures = true;
        float sc = _scales[i];
        CSize sz;
        sz.width = cRound(imgsz.width/sc);
        sz.height = cRound(imgsz.height/sc);
        s.ystep = sc >= 2 ? 1 : 2;
        s.scale = sc;
        s.szi = cSize(sz.width+1, sz.height+1);

        if( i == 0 )
        {
            layer_dy = s.szi.height;
        }

        if( layer_ofs.x + s.szi.width > sbufSize.width )
        {
            layer_ofs = cPoint(0, layer_ofs.y + layer_dy);
            layer_dy = s.szi.height;
        }
        s.layer_ofs = layer_ofs.y*sbufSize.width + layer_ofs.x;
        layer_ofs.x += s.szi.width;
    }

    layer_ofs.y += layer_dy;
    sbufSize.height = MAX(sbufSize.height, layer_ofs.y);
    recalcOptFeatures = recalcOptFeatures || sbufSize.height != prevBufSize.height;
    return recalcOptFeatures;
}


bool FeatureEvaluator::setImage( const img_t* _image, const std::vector<float>& _scales )
{
    CC_INSTRUMENT_REGION()

    CSize imgsz = cvGetSize(_image);
    bool recalcOptFeatures = updateScaleData(imgsz, _scales);

    size_t i, nscales = scaleData->size();
    if (nscales == 0)
    {
        return false;
    }
    CSize sz0 = scaleData->at(0).szi;
    sz0 = cSize(MAX(rbuf->cols, (int)alignSize(sz0.width, 16)), MAX(rbuf->rows, sz0.height));
#if 0
    if (recalcOptFeatures)
    {
        computeOptFeatures();
        copyVectorToUMat(*scaleData, uscaleData);
    }

    if (_image.isUMat() && localSize.area() > 0)
    {
        usbuf->create(sbufSize.height*nchannels, sbufSize.width, CC_32S);
        urbuf->create(sz0, CC_8U);

        for (i = 0; i < nscales; i++)
        {
            const ScaleData& s = scaleData->at(i);
            UMat dst(urbuf, cRect(0, 0, s.szi.width - 1, s.szi.height - 1));
            resize(_image, dst, dst.size(), 1. / s.scale, 1. / s.scale, INTER_LINEAR);
            computeChannels((int)i, dst);
        }
        sbufFlag = USBUF_VALID;
    }
    else
#endif
    {
        const img_t* image = _image;
        cvSetMat(sbuf, sbufSize.height*nchannels, sbufSize.width, CC_32S);
        cvSetMat(rbuf, sz0.h, sz0.w, CC_8U);

        for (i = 0; i < nscales; i++)
        {
            const ScaleData& s = scaleData->at(i);
            img_t dst = cvMat(s.szi.height - 1, s.szi.width - 1, CC_8U, rbuf->tt.data);
            resize(image, &dst, cvGetSize(&dst), 1. / s.scale, 1. / s.scale, CC_INTER_LINEAR);
            computeChannels((int)i, &dst);
        }
        sbufFlag = SBUF_VALID;
    }

    return true;
}

//----------------------------------------------  HaarEvaluator ---------------------------------------

bool HaarEvaluator::Feature :: read( const CFileNode* node )
{
    CFileNode rnode = node[CC_RECTS];
    FileNodeIterator it = rnode.begin(), it_end = rnode.end();

    int ri;
    for( ri = 0; ri < RECT_NUM; ri++ )
    {
        rect[ri].r = cRect();
        rect[ri].weight = 0.f;
    }

    for(ri = 0; it != it_end; ++it, ri++)
    {
        FileNodeIterator it2 = (*it).begin();
        it2 >> rect[ri].r.x >> rect[ri].r.y >>
            rect[ri].r.width >> rect[ri].r.height >> rect[ri].weight;
    }

    tilted = (int)node[CC_TILTED] != 0;
    return true;
}

HaarEvaluator::HaarEvaluator()
{
    optfeaturesPtr = 0;
    pwin = 0;
    localSize = cSize(4, 2);
    lbufSize = cSize(0, 0);
    nchannels = 0;
    tofs = 0;
    sqofs = 0;
    varianceNormFactor = 0;
    hasTiltedFeatures = false;
}

HaarEvaluator::~HaarEvaluator()
{
}

bool HaarEvaluator::read(const CFileNode* node, CSize _origWinSize)
{
    if (!FeatureEvaluator::read(node, _origWinSize))
        return false;
    size_t i, n = node.size();
    CC_Assert(n > 0);
    if(features.empty())
        features = makePtr<std::vector<Feature> >();
    if(optfeatures.empty())
        optfeatures = makePtr<std::vector<OptFeature> >();
    if (optfeatures_lbuf.empty())
        optfeatures_lbuf = makePtr<std::vector<OptFeature> >();
    features->resize(n);
    FileNodeIterator it = node.begin();
    hasTiltedFeatures = false;
    std::vector<Feature>& ff = *features;
    sbufSize = cSize(0, 0);
    ufbuf.release();

    for(i = 0; i < n; i++, ++it)
    {
        if(!ff[i].read(*it))
            return false;
        if( ff[i].tilted )
            hasTiltedFeatures = true;
    }
    nchannels = hasTiltedFeatures ? 3 : 2;
    normrect = cRect(1, 1, origWinSize.width - 2, origWinSize.height - 2);

    localSize = lbufSize = cSize(0, 0);
    if (ocl::haveOpenCL())
    {
        if (ocl::Device::getDefault().isAMD() || ocl::Device::getDefault().isIntel())
        {
            localSize = cSize(8, 8);
            lbufSize = cSize(origWinSize.width + localSize.width,
                            origWinSize.height + localSize.height);
            if (lbufSize.area() > 1024)
                lbufSize = cSize(0, 0);
        }
    }

    return true;
}

Ptr<FeatureEvaluator> HaarEvaluator::clone() const
{
    Ptr<HaarEvaluator> ret = makePtr<HaarEvaluator>();
    *ret = *this;
    return ret;
}


void HaarEvaluator::computeChannels(int scaleIdx, const img_t* img)
{
    CC_INSTRUMENT_REGION()

    const ScaleData& s = scaleData->at(scaleIdx);
    sqofs = hasTiltedFeatures ? sbufSize.area() * 2 : sbufSize.area();

    if (img.isUMat())
    {
        int sx = s.layer_ofs % sbufSize.width;
        int sy = s.layer_ofs / sbufSize.width;
        int sqy = sy + (sqofs / sbufSize.width);
        UMat sum(usbuf, cRect(sx, sy, s.szi.width, s.szi.height));
        UMat sqsum(usbuf, cRect(sx, sqy, s.szi.width, s.szi.height));
        sqsum.flags = (sqsum.flags & ~UMat::DEPTH_MASK) | CC_32S;

        if (hasTiltedFeatures)
        {
            int sty = sy + (tofs / sbufSize.width);
            UMat tilted(usbuf, cRect(sx, sty, s.szi.width, s.szi.height));
            integral(img, sum, sqsum, tilted, CC_32S, CC_32S);
        }
        else
        {
            UMatData* u = sqsum.u;
            integral(img, sum, sqsum, noArray(), CC_32S, CC_32S);
            CC_Assert(sqsum.u == u && sqsum.size() == s.szi && sqsum.type()==CC_32S);
        }
    }
    else
    {
        Mat sum(s.szi, CC_32S, sbuf->ptr<int>() + s.layer_ofs, sbuf->step);
        Mat sqsum(s.szi, CC_32S, sum.ptr<int>() + sqofs, sbuf->step);

        if (hasTiltedFeatures)
        {
            Mat tilted(s.szi, CC_32S, sum.ptr<int>() + tofs, sbuf->step);
            integral(img, sum, sqsum, tilted, CC_32S, CC_32S);
        }
        else
            integral(img, sum, sqsum, noArray(), CC_32S, CC_32S);
    }
}

void HaarEvaluator::computeOptFeatures()
{
    CC_INSTRUMENT_REGION()

    if (hasTiltedFeatures)
        tofs = sbufSize.area();

    int sstep = sbufSize.width;
    CC_SUM_OFS( nofs[0], nofs[1], nofs[2], nofs[3], 0, normrect, sstep );

    size_t fi, nfeatures = features->size();
    const std::vector<Feature>& ff = *features;
    optfeatures->resize(nfeatures);
    optfeaturesPtr = &(*optfeatures)[0];
    for( fi = 0; fi < nfeatures; fi++ )
        optfeaturesPtr[fi].setOffsets( ff[fi], sstep, tofs );
    optfeatures_lbuf->resize(nfeatures);

    for( fi = 0; fi < nfeatures; fi++ )
        optfeatures_lbuf->at(fi).setOffsets(ff[fi], lbufSize.width > 0 ? lbufSize.width : sstep, tofs);

    copyVectorToUMat(*optfeatures_lbuf, ufbuf);
}

bool HaarEvaluator::setWindow( CPoint pt, int scaleIdx )
{
    const ScaleData& s = getScaleData(scaleIdx);

    if( pt.x < 0 || pt.y < 0 ||
        pt.x + origWinSize.width >= s.szi.width ||
        pt.y + origWinSize.height >= s.szi.height )
        return false;

    pwin = &sbuf->at<int>(pt) + s.layer_ofs;
    const int* pq = (const int*)(pwin + sqofs);
    int valsum = CALC_SUM_OFS(nofs, pwin);
    unsigned valsqsum = (unsigned)(CALC_SUM_OFS(nofs, pq));

    double area = normrect.area();
    double nf = area * valsqsum - (double)valsum * valsum;
    if( nf > 0. )
    {
        nf = sqrt(nf);
        varianceNormFactor = (float)(1./nf);
        return area*varianceNormFactor < 1e-1;
    }
    else
    {
        varianceNormFactor = 1.f;
        return false;
    }
}


void HaarEvaluator::OptFeature::setOffsets( const Feature& _f, int step, int _tofs )
{
    weight[0] = _f.rect[0].weight;
    weight[1] = _f.rect[1].weight;
    weight[2] = _f.rect[2].weight;

    if( _f.tilted )
    {
        CC_TILTED_OFS( ofs[0][0], ofs[0][1], ofs[0][2], ofs[0][3], _tofs, _f.rect[0].r, step );
        CC_TILTED_OFS( ofs[1][0], ofs[1][1], ofs[1][2], ofs[1][3], _tofs, _f.rect[1].r, step );
        CC_TILTED_OFS( ofs[2][0], ofs[2][1], ofs[2][2], ofs[2][3], _tofs, _f.rect[2].r, step );
    }
    else
    {
        CC_SUM_OFS( ofs[0][0], ofs[0][1], ofs[0][2], ofs[0][3], 0, _f.rect[0].r, step );
        CC_SUM_OFS( ofs[1][0], ofs[1][1], ofs[1][2], ofs[1][3], 0, _f.rect[1].r, step );
        CC_SUM_OFS( ofs[2][0], ofs[2][1], ofs[2][2], ofs[2][3], 0, _f.rect[2].r, step );
    }
}

CRect HaarEvaluator::getNormRect() const
{
    return normrect;
}

int HaarEvaluator::getSquaresOffset() const
{
    return sqofs;
}

//----------------------------------------------  LBPEvaluator -------------------------------------
bool LBPEvaluator::Feature :: read(const CFileNode* node )
{
    CFileNode rnode = node[CC_RECT];
    FileNodeIterator it = rnode.begin();
    it >> rect.x >> rect.y >> rect.width >> rect.height;
    return true;
}

LBPEvaluator::LBPEvaluator()
{
    features = makePtr<std::vector<Feature> >();
    optfeatures = makePtr<std::vector<OptFeature> >();
    scaleData = makePtr<std::vector<ScaleData> >();
    optfeaturesPtr = 0;
    pwin = 0;
}

LBPEvaluator::~LBPEvaluator()
{
}

bool LBPEvaluator::read( const CFileNode* node, CSize _origWinSize )
{
    if (!FeatureEvaluator::read(node, _origWinSize))
        return false;
    if(features.empty())
        features = makePtr<std::vector<Feature> >();
    if(optfeatures.empty())
        optfeatures = makePtr<std::vector<OptFeature> >();
    if (optfeatures_lbuf.empty())
        optfeatures_lbuf = makePtr<std::vector<OptFeature> >();

    features->resize(node.size());
    optfeaturesPtr = 0;
    FileNodeIterator it = node.begin(), it_end = node.end();
    std::vector<Feature>& ff = *features;
    for(int i = 0; it != it_end; ++it, i++)
    {
        if(!ff[i].read(*it))
            return false;
    }
    nchannels = 1;
    localSize = lbufSize = cSize(0, 0);
    if (ocl::haveOpenCL())
        localSize = cSize(8, 8);

    return true;
}

Ptr<FeatureEvaluator> LBPEvaluator::clone() const
{
    Ptr<LBPEvaluator> ret = makePtr<LBPEvaluator>();
    *ret = *this;
    return ret;
}

void LBPEvaluator::computeChannels(int scaleIdx, const img_t* _img)
{
    const ScaleData& s = scaleData->at(scaleIdx);

    if (_img.isUMat())
    {
        int sx = s.layer_ofs % sbufSize.width;
        int sy = s.layer_ofs / sbufSize.width;
        UMat sum(usbuf, cRect(sx, sy, s.szi.width, s.szi.height));
        integral(_img, sum, noArray(), noArray(), CC_32S);
    }
    else
    {
        Mat sum(s.szi, CC_32S, sbuf->ptr<int>() + s.layer_ofs, sbuf->step);
        integral(_img, sum, noArray(), noArray(), CC_32S);
    }
}

void LBPEvaluator::computeOptFeatures()
{
    int sstep = sbufSize.width;

    size_t fi, nfeatures = features->size();
    const std::vector<Feature>& ff = *features;
    optfeatures->resize(nfeatures);
    optfeaturesPtr = &(*optfeatures)[0];
    for( fi = 0; fi < nfeatures; fi++ )
        optfeaturesPtr[fi].setOffsets( ff[fi], sstep );
    copyVectorToUMat(*optfeatures, ufbuf);
}


void LBPEvaluator::OptFeature::setOffsets( const Feature& _f, int step )
{
    CRect tr = _f.rect;
    int w0 = tr.width;
    int h0 = tr.height;

    CC_SUM_OFS( ofs[0], ofs[1], ofs[4], ofs[5], 0, tr, step );
    tr.x += 2*w0;
    CC_SUM_OFS( ofs[2], ofs[3], ofs[6], ofs[7], 0, tr, step );
    tr.y += 2*h0;
    CC_SUM_OFS( ofs[10], ofs[11], ofs[14], ofs[15], 0, tr, step );
    tr.x -= 2*w0;
    CC_SUM_OFS( ofs[8], ofs[9], ofs[12], ofs[13], 0, tr, step );
}


bool LBPEvaluator::setWindow( CPoint pt, int scaleIdx )
{
    CC_Assert(0 <= scaleIdx && scaleIdx < (int)scaleData->size());
    const ScaleData& s = scaleData->at(scaleIdx);

    if( pt.x < 0 || pt.y < 0 ||
        pt.x + origWinSize.width >= s.szi.width ||
        pt.y + origWinSize.height >= s.szi.height )
        return false;

    pwin = &sbuf->at<int>(pt) + s.layer_ofs;
    return true;
}


Ptr<FeatureEvaluator> FeatureEvaluator::create( int featureType )
{
    return featureType == HAAR ? Ptr<FeatureEvaluator>(new HaarEvaluator) :
        featureType == LBP ? Ptr<FeatureEvaluator>(new LBPEvaluator) :
        Ptr<FeatureEvaluator>();
}

//---------------------------------------- Classifier Cascade --------------------------------------------

CascadeClassifierImpl::CascadeClassifierImpl()
{
#ifdef HAVE_OPENCL
    tryOpenCL = false;
#endif
}

CascadeClassifierImpl::~CascadeClassifierImpl()
{
}

bool CascadeClassifierImpl::empty() const
{
    return !oldCascade && data.stages.empty();
}

bool CascadeClassifierImpl::load(const String& filename)
{
    oldCascade.release();
    data = Data();
    featureEvaluator.release();

    FileStorage fs(filename, FileStorage::READ);
    if( !fs.isOpened() )
        return false;

    if( read_(fs.getFirstTopLevelNode()) )
        return true;

    fs.release();

    oldCascade.reset((CvHaarClassifierCascade*)cvLoad(filename.c_str(), 0, 0, 0));
    return !oldCascade.empty();
}

void CascadeClassifierImpl::read(const CFileNode* node)
{
    read_(node);
}

int CascadeClassifierImpl::runAt( Ptr<FeatureEvaluator>& evaluator, CPoint pt, int scaleIdx, double& weight )
{
    CC_INSTRUMENT_REGION()

    assert( !oldCascade &&
           (data.featureType == FeatureEvaluator::HAAR ||
            data.featureType == FeatureEvaluator::LBP ||
            data.featureType == FeatureEvaluator::HOG) );

    if( !evaluator->setWindow(pt, scaleIdx) )
        return -1;
    if( data.maxNodesPerTree == 1 )
    {
        if( data.featureType == FeatureEvaluator::HAAR )
            return predictOrderedStump<HaarEvaluator>( *this, evaluator, weight );
        else if( data.featureType == FeatureEvaluator::LBP )
            return predictCategoricalStump<LBPEvaluator>( *this, evaluator, weight );
        else
            return -2;
    }
    else
    {
        if( data.featureType == FeatureEvaluator::HAAR )
            return predictOrdered<HaarEvaluator>( *this, evaluator, weight );
        else if( data.featureType == FeatureEvaluator::LBP )
            return predictCategorical<LBPEvaluator>( *this, evaluator, weight );
        else
            return -2;
    }
}

void CascadeClassifierImpl::setMaskGenerator(const Ptr<MaskGenerator>& _maskGenerator)
{
    maskGenerator=_maskGenerator;
}
Ptr<CascadeClassifierImpl::MaskGenerator> CascadeClassifierImpl::getMaskGenerator()
{
    return maskGenerator;
}

Ptr<BaseCascadeClassifier::MaskGenerator> createFaceDetectionMaskGenerator()
{
#ifdef HAVE_TEGRA_OPTIMIZATION
    if (tegra::useTegra())
        return tegra::getCascadeClassifierMaskGenerator();
#endif
    return Ptr<BaseCascadeClassifier::MaskGenerator>();
}

class CascadeClassifierInvoker : public ParallelLoopBody
{
public:
    CascadeClassifierInvoker( CascadeClassifierImpl& _cc, int _nscales, int _nstripes,
                              const FeatureEvaluator::ScaleData* _scaleData,
                              const int* _stripeSizes, std::vector<CRect>& _vec,
                              std::vector<int>& _levels, std::vector<double>& _weights,
                              bool outputLevels, const Mat& _mask, Mutex* _mtx)
    {
        classifier = &_cc;
        nscales = _nscales;
        nstripes = _nstripes;
        scaleData = _scaleData;
        stripeSizes = _stripeSizes;
        rectangles = &_vec;
        rejectLevels = outputLevels ? &_levels : 0;
        levelWeights = outputLevels ? &_weights : 0;
        mask = _mask;
        mtx = _mtx;
    }

    void operator()(const Range& range) const
    {
        CC_INSTRUMENT_REGION()

        Ptr<FeatureEvaluator> evaluator = classifier->featureEvaluator->clone();
        double gypWeight = 0.;
        CSize origWinSize = classifier->data.origWinSize;

        for( int scaleIdx = 0; scaleIdx < nscales; scaleIdx++ )
        {
            const FeatureEvaluator::ScaleData& s = scaleData[scaleIdx];
            float scalingFactor = s.scale;
            int yStep = s.ystep;
            int stripeSize = stripeSizes[scaleIdx];
            int y0 = range.start*stripeSize;
            CSize szw = s.getWorkingSize(origWinSize);
            int y1 = MIN(range.end*stripeSize, szw.height);
            CSize winSize(cRound(origWinSize.width * scalingFactor),
                         cRound(origWinSize.height * scalingFactor));

            for( int y = y0; y < y1; y += yStep )
            {
                for( int x = 0; x < szw.width; x += yStep )
                {
                    int result = classifier->runAt(evaluator, cPoint(x, y), scaleIdx, gypWeight);
                    if( rejectLevels )
                    {
                        if( result == 1 )
                            result = -(int)classifier->data.stages.size();
                        if( classifier->data.stages.size() + result == 0 )
                        {
                            mtx->lock();
                            rectangles->push_back(cRect(cRound(x*scalingFactor),
                                                       cRound(y*scalingFactor),
                                                       winSize.width, winSize.height));
                            rejectLevels->push_back(-result);
                            levelWeights->push_back(gypWeight);
                            mtx->unlock();
                        }
                    }
                    else if( result > 0 )
                    {
                        mtx->lock();
                        rectangles->push_back(cRect(cRound(x*scalingFactor),
                                                   cRound(y*scalingFactor),
                                                   winSize.width, winSize.height));
                        mtx->unlock();
                    }
                    if( result == 0 )
                        x += yStep;
                }
            }
        }
    }

    CascadeClassifierImpl* classifier;
    std::vector<CRect>* rectangles;
    int nscales, nstripes;
    const FeatureEvaluator::ScaleData* scaleData;
    const int* stripeSizes;
    std::vector<int> *rejectLevels;
    std::vector<double> *levelWeights;
    std::vector<float> scales;
    Mat mask;
    Mutex* mtx;
};


struct getRect { CRect operator ()(const CvAvgComp& e) const { return e.rect; } };
struct getNeighbors { int operator ()(const CvAvgComp& e) const { return e.neighbors; } };

#ifdef HAVE_OPENCL
bool CascadeClassifierImpl::ocl_detectMultiScaleNoGrouping( const std::vector<float>& scales,
                                                            std::vector<CRect>& candidates )
{
    int featureType = getFeatureType();
    std::vector<UMat> bufs;
    featureEvaluator->getUMats(bufs);
    CSize localsz = featureEvaluator->getLocalSize();
    if( localsz.area() == 0 )
        return false;
    CSize lbufSize = featureEvaluator->getLocalBufSize();
    size_t localsize[] = { (size_t)localsz.width, (size_t)localsz.height };
    const int grp_per_CU = 12;
    size_t globalsize[] = { grp_per_CU*ocl::Device::getDefault().maxComputeUnits()*localsize[0], localsize[1] };
    bool ok = false;

    ufacepos.create(1, MAX_FACES*3+1, CC_32S);
    UMat ufacepos_count(ufacepos, cRect(0, 0, 1, 1));
    ufacepos_count.setTo(Scalar::all(0));

    if( ustages.empty() )
    {
        copyVectorToUMat(data.stages, ustages);
        if (!data.stumps.empty())
            copyVectorToUMat(data.stumps, unodes);
        else
            copyVectorToUMat(data.nodes, unodes);
        copyVectorToUMat(data.leaves, uleaves);
        if( !data.subsets.empty() )
            copyVectorToUMat(data.subsets, usubsets);
    }

    int nstages = (int)data.stages.size();
    int splitstage_ocl = 1;

    if( featureType == FeatureEvaluator::HAAR )
    {
        Ptr<HaarEvaluator> haar = featureEvaluator.dynamicCast<HaarEvaluator>();
        if( haar.empty() )
            return false;

        if( haarKernel.empty() )
        {
            String opts;
            if (lbufSize.area())
                opts = format("-D LOCAL_SIZE_X=%d -D LOCAL_SIZE_Y=%d -D SUM_BUF_SIZE=%d -D SUM_BUF_STEP=%d -D NODE_COUNT=%d -D SPLIT_STAGE=%d -D N_STAGES=%d -D MAX_FACES=%d -D HAAR",
                              localsz.width, localsz.height, lbufSize.area(), lbufSize.width, data.maxNodesPerTree, splitstage_ocl, nstages, MAX_FACES);
            else
                opts = format("-D LOCAL_SIZE_X=%d -D LOCAL_SIZE_Y=%d -D NODE_COUNT=%d -D SPLIT_STAGE=%d -D N_STAGES=%d -D MAX_FACES=%d -D HAAR",
                              localsz.width, localsz.height, data.maxNodesPerTree, splitstage_ocl, nstages, MAX_FACES);
            haarKernel.create("runHaarClassifier", ocl::objdetect::cascadedetect_oclsrc, opts);
            if( haarKernel.empty() )
                return false;
        }

        CRect normrect = haar->getNormRect();
        int sqofs = haar->getSquaresOffset();

        haarKernel.args((int)scales.size(),
                        ocl::KernelArg::PtrReadOnly(bufs[0]), // scaleData
                        ocl::KernelArg::ReadOnlyNoSize(bufs[1]), // sum
                        ocl::KernelArg::PtrReadOnly(bufs[2]), // optfeatures

                        // cascade classifier
                        ocl::KernelArg::PtrReadOnly(ustages),
                        ocl::KernelArg::PtrReadOnly(unodes),
                        ocl::KernelArg::PtrReadOnly(uleaves),

                        ocl::KernelArg::PtrWriteOnly(ufacepos), // positions
                        normrect, sqofs, data.origWinSize);
        ok = haarKernel.run(2, globalsize, localsize, true);
    }
    else if( featureType == FeatureEvaluator::LBP )
    {
        if (data.maxNodesPerTree > 1)
            return false;

        Ptr<LBPEvaluator> lbp = featureEvaluator.dynamicCast<LBPEvaluator>();
        if( lbp.empty() )
            return false;

        if( lbpKernel.empty() )
        {
            String opts;
            if (lbufSize.area())
                opts = format("-D LOCAL_SIZE_X=%d -D LOCAL_SIZE_Y=%d -D SUM_BUF_SIZE=%d -D SUM_BUF_STEP=%d -D SPLIT_STAGE=%d -D N_STAGES=%d -D MAX_FACES=%d -D LBP",
                              localsz.width, localsz.height, lbufSize.area(), lbufSize.width, splitstage_ocl, nstages, MAX_FACES);
            else
                opts = format("-D LOCAL_SIZE_X=%d -D LOCAL_SIZE_Y=%d -D SPLIT_STAGE=%d -D N_STAGES=%d -D MAX_FACES=%d -D LBP",
                              localsz.width, localsz.height, splitstage_ocl, nstages, MAX_FACES);
            lbpKernel.create("runLBPClassifierStumpSimple", ocl::objdetect::cascadedetect_oclsrc, opts);
            if( lbpKernel.empty() )
                return false;
        }

        int subsetSize = (data.ncategories + 31)/32;
        lbpKernel.args((int)scales.size(),
                       ocl::KernelArg::PtrReadOnly(bufs[0]), // scaleData
                       ocl::KernelArg::ReadOnlyNoSize(bufs[1]), // sum
                       ocl::KernelArg::PtrReadOnly(bufs[2]), // optfeatures

                       // cascade classifier
                       ocl::KernelArg::PtrReadOnly(ustages),
                       ocl::KernelArg::PtrReadOnly(unodes),
                       ocl::KernelArg::PtrReadOnly(usubsets),
                       subsetSize,

                       ocl::KernelArg::PtrWriteOnly(ufacepos), // positions
                       data.origWinSize);

        ok = lbpKernel.run(2, globalsize, localsize, true);
    }

    if( ok )
    {
        Mat facepos = ufacepos.getMat(ACCESS_READ);
        const int* fptr = facepos.ptr<int>();
        int nfaces = fptr[0];
        nfaces = MIN(nfaces, (int)MAX_FACES);

        for( int i = 0; i < nfaces; i++ )
        {
            const FeatureEvaluator::ScaleData& s = featureEvaluator->getScaleData(fptr[i*3 + 1]);
            candidates.push_back(cRect(cRound(fptr[i*3 + 2]*s.scale),
                                      cRound(fptr[i*3 + 3]*s.scale),
                                      cRound(data.origWinSize.width*s.scale),
                                      cRound(data.origWinSize.height*s.scale)));
        }
    }
    return ok;
}
#endif

bool CascadeClassifierImpl::isOldFormatCascade() const
{
    return !oldCascade.empty();
}

int CascadeClassifierImpl::getFeatureType() const
{
    return featureEvaluator->getFeatureType();
}

CSize CascadeClassifierImpl::getOriginalWindowSize() const
{
    return data.origWinSize;
}

void* CascadeClassifierImpl::getOldCascade()
{
    return oldCascade;
}

static void detectMultiScaleOldFormat( const Mat& image, Ptr<CvHaarClassifierCascade> oldCascade,
                                       std::vector<CRect>& objects,
                                       std::vector<int>& rejectLevels,
                                       std::vector<double>& levelWeights,
                                       std::vector<CvAvgComp>& vecAvgComp,
                                       double scaleFactor, int minNeighbors,
                                       int flags, CSize minObjectSize, CSize maxObjectSize,
                                       bool outputRejectLevels = false )
{
    MemStorage storage(cvCreateMemStorage(0));
    CvMat _image = image;
    CvSeq* _objects = cvHaarDetectObjectsForROC( &_image, oldCascade, storage, rejectLevels, levelWeights, scaleFactor,
                                                 minNeighbors, flags, minObjectSize, maxObjectSize, outputRejectLevels );
    Seq<CvAvgComp>(_objects).copyTo(vecAvgComp);
    objects.resize(vecAvgComp.size());
    std::transform(vecAvgComp.begin(), vecAvgComp.end(), objects.begin(), getRect());
}

void CascadeClassifierImpl::detectMultiScaleNoGrouping( const img_t* _image, std::vector<CRect>& candidates,
                                                    std::vector<int>& rejectLevels, std::vector<double>& levelWeights,
                                                    double scaleFactor, CSize minObjectSize, CSize maxObjectSize,
                                                    bool outputRejectLevels )
{
    CC_INSTRUMENT_REGION()

    CSize imgsz = _image.size();
    CSize originalWindowSize = getOriginalWindowSize();

    if( maxObjectSize.height == 0 || maxObjectSize.width == 0 )
        maxObjectSize = imgsz;

    // If a too small image patch is entering the function, break early before any processing
    if( (imgsz.height < originalWindowSize.height) || (imgsz.width < originalWindowSize.width) )
        return;

    std::vector<float> all_scales, scales;
    all_scales.reserve(1024);
    scales.reserve(1024);

    // First calculate all possible scales for the given image and model, then remove undesired scales
    // This allows us to cope with single scale detections (minSize == maxSize) that do not fall on precalculated scale
    for( double factor = 1; ; factor *= scaleFactor )
    {
        CSize windowSize( cRound(originalWindowSize.width*factor), cRound(originalWindowSize.height*factor) );
        if( windowSize.width > imgsz.width || windowSize.height > imgsz.height )
            break;
        all_scales.push_back((float)factor);
    }

    // This will capture allowed scales and a minSize==maxSize scale, if it is in the precalculated scales
    for( size_t index = 0; index < all_scales.size(); index++){
        CSize windowSize( cRound(originalWindowSize.width*all_scales[index]), cRound(originalWindowSize.height*all_scales[index]) );
        if( windowSize.width > maxObjectSize.width || windowSize.height > maxObjectSize.height)
            break;
        if( windowSize.width < minObjectSize.width || windowSize.height < minObjectSize.height )
            continue;
        scales.push_back(all_scales[index]);
    }

    // If minSize and maxSize parameter are equal and scales is not filled yet, then the scale was not available in the precalculated scales
    // In that case we want to return the most fitting scale (closest corresponding scale using L2 distance)
    if( scales.empty() && !all_scales.empty() ){
        std::vector<double> distances;
        // Calculate distances
        for(size_t v = 0; v < all_scales.size(); v++){
            CSize windowSize( cRound(originalWindowSize.width*all_scales[v]), cRound(originalWindowSize.height*all_scales[v]) );
            double d = (minObjectSize.width - windowSize.width) * (minObjectSize.width - windowSize.width)
                       + (minObjectSize.height - windowSize.height) * (minObjectSize.height - windowSize.height);
            distances.push_back(d);
        }
        // Take the index of lowest value
        // Use that index to push the correct scale parameter
        size_t iMin=0;
        for(size_t i = 0; i < distances.size(); ++i){
            if(distances[iMin] > distances[i])
                    iMin=i;
        }
        scales.push_back(all_scales[iMin]);
    }

    candidates.clear();
    rejectLevels.clear();
    levelWeights.clear();

#ifdef HAVE_OPENCL
    bool use_ocl = tryOpenCL && ocl::useOpenCL() &&
         OCL_FORCE_CHECK(_image.isUMat()) &&
         featureEvaluator->getLocalSize().area() > 0 &&
         (data.minNodesPerTree == data.maxNodesPerTree) &&
         !isOldFormatCascade() &&
         maskGenerator.empty() &&
         !outputRejectLevels;
#endif

    Mat grayImage;
    _InputArray gray;

    if (_image.channels() > 1)
        cvtColor(_image, grayImage, COLOR_BGR2GRAY);
    else if (_image.isMat())
        grayImage = _image.getMat();
    else
        _image.copyTo(grayImage);
    gray = grayImage;

    if( !featureEvaluator->setImage(gray, scales) )
        return;

#ifdef HAVE_OPENCL
    // OpenCL code
    CC_OCL_RUN(use_ocl, ocl_detectMultiScaleNoGrouping( scales, candidates ))

    if (use_ocl)
        tryOpenCL = false;
#endif

    // CPU code
    featureEvaluator->getMats();
    {
        Mat currentMask;
        if (maskGenerator)
            currentMask = maskGenerator->generateMask(gray.getMat());

        size_t i, nscales = scales.size();
        cv::AutoBuffer<int> stripeSizeBuf(nscales);
        int* stripeSizes = stripeSizeBuf;
        const FeatureEvaluator::ScaleData* s = &featureEvaluator->getScaleData(0);
        CSize szw = s->getWorkingSize(data.origWinSize);
        int nstripes = cvCeil(szw.width/32.);
        for( i = 0; i < nscales; i++ )
        {
            szw = s[i].getWorkingSize(data.origWinSize);
            stripeSizes[i] = MAX((szw.height/s[i].ystep + nstripes-1)/nstripes, 1)*s[i].ystep;
        }

        CascadeClassifierInvoker invoker(*this, (int)nscales, nstripes, s, stripeSizes,
                                         candidates, rejectLevels, levelWeights,
                                         outputRejectLevels, currentMask, &mtx);
        parallel_for_(Range(0, nstripes), invoker);
    }
}


void CascadeClassifierImpl::detectMultiScale( const img_t* _image, std::vector<CRect>& objects,
                                          std::vector<int>& rejectLevels,
                                          std::vector<double>& levelWeights,
                                          double scaleFactor, int minNeighbors,
                                          int flags, CSize minObjectSize, CSize maxObjectSize,
                                          bool outputRejectLevels )
{
    CC_INSTRUMENT_REGION()

    CC_Assert( scaleFactor > 1 && _image.depth() == CC_8U );

    if( empty() )
        return;

    if( isOldFormatCascade() )
    {
        Mat image = _image.getMat();
        std::vector<CvAvgComp> fakeVecAvgComp;
        detectMultiScaleOldFormat( image, oldCascade, objects, rejectLevels, levelWeights, fakeVecAvgComp, scaleFactor,
                                   minNeighbors, flags, minObjectSize, maxObjectSize, outputRejectLevels );
    }
    else
    {
        detectMultiScaleNoGrouping( _image, objects, rejectLevels, levelWeights, scaleFactor, minObjectSize, maxObjectSize,
                                    outputRejectLevels );
        const double GROUP_EPS = 0.2;
        if( outputRejectLevels )
        {
            groupRectangles( objects, rejectLevels, levelWeights, minNeighbors, GROUP_EPS );
        }
        else
        {
            groupRectangles( objects, minNeighbors, GROUP_EPS );
        }
    }
}

void CascadeClassifierImpl::detectMultiScale( const img_t* _image, std::vector<CRect>& objects,
                                          double scaleFactor, int minNeighbors,
                                          int flags, CSize minObjectSize, CSize maxObjectSize)
{
    CC_INSTRUMENT_REGION()

    std::vector<int> fakeLevels;
    std::vector<double> fakeWeights;
    detectMultiScale( _image, objects, fakeLevels, fakeWeights, scaleFactor,
        minNeighbors, flags, minObjectSize, maxObjectSize );
}

void CascadeClassifierImpl::detectMultiScale( const img_t* _image, std::vector<CRect>& objects,
                                          std::vector<int>& numDetections, double scaleFactor,
                                          int minNeighbors, int flags, CSize minObjectSize,
                                          CSize maxObjectSize )
{
    CC_INSTRUMENT_REGION()

    Mat image = _image.getMat();
    CC_Assert( scaleFactor > 1 && image.depth() == CC_8U );

    if( empty() )
        return;

    std::vector<int> fakeLevels;
    std::vector<double> fakeWeights;
    if( isOldFormatCascade() )
    {
        std::vector<CvAvgComp> vecAvgComp;
        detectMultiScaleOldFormat( image, oldCascade, objects, fakeLevels, fakeWeights, vecAvgComp, scaleFactor,
                                   minNeighbors, flags, minObjectSize, maxObjectSize );
        numDetections.resize(vecAvgComp.size());
        std::transform(vecAvgComp.begin(), vecAvgComp.end(), numDetections.begin(), getNeighbors());
    }
    else
    {
        detectMultiScaleNoGrouping( image, objects, fakeLevels, fakeWeights, scaleFactor, minObjectSize, maxObjectSize );
        const double GROUP_EPS = 0.2;
        groupRectangles( objects, numDetections, minNeighbors, GROUP_EPS );
    }
}


CascadeClassifierImpl::Data::Data()
{
    stageType = featureType = ncategories = maxNodesPerTree = minNodesPerTree = 0;
}

bool CascadeClassifierImpl::Data::read(const CFileNode &root)
{
    static const float THRESHOLD_EPS = 1e-5f;

    // load stage params
    String stageTypeStr = (String)root[CC_STAGE_TYPE];
    if( stageTypeStr == CC_BOOST )
        stageType = BOOST;
    else
        return false;

    String featureTypeStr = (String)root[CC_FEATURE_TYPE];
    if( featureTypeStr == CC_HAAR )
        featureType = FeatureEvaluator::HAAR;
    else if( featureTypeStr == CC_LBP )
        featureType = FeatureEvaluator::LBP;
    else if( featureTypeStr == CC_HOG )
    {
        featureType = FeatureEvaluator::HOG;
        CC_Error(Error::StsNotImplemented, "HOG cascade is not supported in 3.0");
    }
    else
        return false;

    origWinSize.width = (int)root[CC_WIDTH];
    origWinSize.height = (int)root[CC_HEIGHT];
    CC_Assert( origWinSize.height > 0 && origWinSize.width > 0 );

    // load feature params
    CFileNode fn = root[CC_FEATURE_PARAMS];
    if( fn.empty() )
        return false;

    ncategories = fn[CC_MAX_CAT_COUNT];
    int subsetSize = (ncategories + 31)/32,
        nodeStep = 3 + ( ncategories>0 ? subsetSize : 1 );

    // load stages
    fn = root[CC_STAGES];
    if( fn.empty() )
        return false;

    stages.reserve(fn.size());
    classifiers.clear();
    nodes.clear();
    stumps.clear();

    FileNodeIterator it = fn.begin(), it_end = fn.end();
    minNodesPerTree = INT_MAX;
    maxNodesPerTree = 0;

    for( int si = 0; it != it_end; si++, ++it )
    {
        CFileNode fns = *it;
        Stage stage;
        stage.threshold = (float)fns[CC_STAGE_THRESHOLD] - THRESHOLD_EPS;
        fns = fns[CC_WEAK_CLASSIFIERS];
        if(fns.empty())
            return false;
        stage.ntrees = (int)fns.size();
        stage.first = (int)classifiers.size();
        stages.push_back(stage);
        classifiers.reserve(stages[si].first + stages[si].ntrees);

        FileNodeIterator it1 = fns.begin(), it1_end = fns.end();
        for( ; it1 != it1_end; ++it1 ) // weak trees
        {
            CFileNode fnw = *it1;
            CFileNode internalNodes = fnw[CC_INTERNAL_NODES];
            CFileNode leafValues = fnw[CC_LEAF_VALUES];
            if( internalNodes.empty() || leafValues.empty() )
                return false;

            DTree tree;
            tree.nodeCount = (int)internalNodes.size()/nodeStep;
            minNodesPerTree = MIN(minNodesPerTree, tree.nodeCount);
            maxNodesPerTree = MAX(maxNodesPerTree, tree.nodeCount);

            classifiers.push_back(tree);

            nodes.reserve(nodes.size() + tree.nodeCount);
            leaves.reserve(leaves.size() + leafValues.size());
            if( subsetSize > 0 )
                subsets.reserve(subsets.size() + tree.nodeCount*subsetSize);

            FileNodeIterator internalNodesIter = internalNodes.begin(), internalNodesEnd = internalNodes.end();

            for( ; internalNodesIter != internalNodesEnd; ) // nodes
            {
                DTreeNode node;
                node.left = (int)*internalNodesIter; ++internalNodesIter;
                node.right = (int)*internalNodesIter; ++internalNodesIter;
                node.featureIdx = (int)*internalNodesIter; ++internalNodesIter;
                if( subsetSize > 0 )
                {
                    for( int j = 0; j < subsetSize; j++, ++internalNodesIter )
                        subsets.push_back((int)*internalNodesIter);
                    node.threshold = 0.f;
                }
                else
                {
                    node.threshold = (float)*internalNodesIter; ++internalNodesIter;
                }
                nodes.push_back(node);
            }

            internalNodesIter = leafValues.begin(), internalNodesEnd = leafValues.end();

            for( ; internalNodesIter != internalNodesEnd; ++internalNodesIter ) // leaves
                leaves.push_back((float)*internalNodesIter);
        }
    }

    if( maxNodesPerTree == 1 )
    {
        int nodeOfs = 0, leafOfs = 0;
        size_t nstages = stages.size();
        for( size_t stageIdx = 0; stageIdx < nstages; stageIdx++ )
        {
            const Stage& stage = stages[stageIdx];

            int ntrees = stage.ntrees;
            for( int i = 0; i < ntrees; i++, nodeOfs++, leafOfs+= 2 )
            {
                const DTreeNode& node = nodes[nodeOfs];
                stumps.push_back(Stump(node.featureIdx, node.threshold,
                                       leaves[leafOfs], leaves[leafOfs+1]));
            }
        }
    }

    return true;
}


bool CascadeClassifierImpl::read_(const CFileNode* root)
{
#ifdef HAVE_OPENCL
    tryOpenCL = true;
    haarKernel = ocl::Kernel();
    lbpKernel = ocl::Kernel();
#endif
    ustages.release();
    unodes.release();
    uleaves.release();
    if( !data.read(root) )
        return false;

    // load features
    featureEvaluator = FeatureEvaluator::create(data.featureType);
    CFileNode fn = root[CC_FEATURES];
    if( fn.empty() )
        return false;

    return featureEvaluator->read(fn, data.origWinSize);
}

template<> void DefaultDeleter<CvHaarClassifierCascade>::operator ()(CvHaarClassifierCascade* obj) const
{ cvReleaseHaarClassifierCascade(&obj); }


BaseCascadeClassifier::~BaseCascadeClassifier()
{
}

CascadeClassifier::CascadeClassifier() {}
CascadeClassifier::CascadeClassifier(const String& filename)
{
    load(filename);
}

CascadeClassifier::~CascadeClassifier()
{
}

bool CascadeClassifier::empty() const
{
    return cc.empty() || cc->empty();
}

bool CascadeClassifier::load( const String& filename )
{
    cc = makePtr<CascadeClassifierImpl>();
    if(!cc->load(filename))
        cc.release();
    return !empty();
}

bool CascadeClassifier::read(const CFileNode &root)
{
    Ptr<CascadeClassifierImpl> ccimpl = makePtr<CascadeClassifierImpl>();
    bool ok = ccimpl->read_(root);
    if( ok )
        cc = ccimpl.staticCast<BaseCascadeClassifier>();
    else
        cc.release();
    return ok;
}

void clipObjects(CSize sz, std::vector<CRect>& objects,
                 std::vector<int>* a, std::vector<double>* b)
{
    size_t i, j = 0, n = objects.size();
    CRect win0 = cRect(0, 0, sz.width, sz.height);
    if(a)
    {
        CC_Assert(a->size() == n);
    }
    if(b)
    {
        CC_Assert(b->size() == n);
    }

    for( i = 0; i < n; i++ )
    {
        CRect r = win0 & objects[i];
        if( r.area() > 0 )
        {
            objects[j] = r;
            if( i > j )
            {
                if(a) a->at(j) = a->at(i);
                if(b) b->at(j) = b->at(i);
            }
            j++;
        }
    }

    if( j < n )
    {
        objects.resize(j);
        if(a) a->resize(j);
        if(b) b->resize(j);
    }
}

void CascadeClassifier::detectMultiScale( const img_t* image,
                      CC_OUT std::vector<CRect>& objects,
                      double scaleFactor,
                      int minNeighbors, int flags,
                      CSize minSize,
                      CSize maxSize )
{
    CC_INSTRUMENT_REGION()

    CC_Assert(!empty());
    cc->detectMultiScale(image, objects, scaleFactor, minNeighbors, flags, minSize, maxSize);
    clipObjects(image.size(), objects, 0, 0);
}

void CascadeClassifier::detectMultiScale( const img_t* image,
                      CC_OUT std::vector<CRect>& objects,
                      CC_OUT std::vector<int>& numDetections,
                      double scaleFactor,
                      int minNeighbors, int flags,
                      CSize minSize, CSize maxSize )
{
    CC_INSTRUMENT_REGION()

    CC_Assert(!empty());
    cc->detectMultiScale(image, objects, numDetections,
                         scaleFactor, minNeighbors, flags, minSize, maxSize);
    clipObjects(image.size(), objects, &numDetections, 0);
}

void CascadeClassifier::detectMultiScale( const img_t* image,
                      CC_OUT std::vector<CRect>& objects,
                      CC_OUT std::vector<int>& rejectLevels,
                      CC_OUT std::vector<double>& levelWeights,
                      double scaleFactor,
                      int minNeighbors, int flags,
                      CSize minSize, CSize maxSize,
                      bool outputRejectLevels )
{
    CC_INSTRUMENT_REGION()

    CC_Assert(!empty());
    cc->detectMultiScale(image, objects, rejectLevels, levelWeights,
                         scaleFactor, minNeighbors, flags,
                         minSize, maxSize, outputRejectLevels);
    clipObjects(image.size(), objects, &rejectLevels, &levelWeights);
}

bool CascadeClassifier::isOldFormatCascade() const
{
    CC_Assert(!empty());
    return cc->isOldFormatCascade();
}

CSize CascadeClassifier::getOriginalWindowSize() const
{
    CC_Assert(!empty());
    return cc->getOriginalWindowSize();
}

int CascadeClassifier::getFeatureType() const
{
    CC_Assert(!empty());
    return cc->getFeatureType();
}

void* CascadeClassifier::getOldCascade()
{
    CC_Assert(!empty());
    return cc->getOldCascade();
}

void CascadeClassifier::setMaskGenerator(const Ptr<BaseCascadeClassifier::MaskGenerator>& maskGenerator)
{
    CC_Assert(!empty());
    cc->setMaskGenerator(maskGenerator);
}

Ptr<BaseCascadeClassifier::MaskGenerator> CascadeClassifier::getMaskGenerator()
{
    CC_Assert(!empty());
    return cc->getMaskGenerator();
}


#if 0
#endif
