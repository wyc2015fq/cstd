/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/


#include <functional>
#include <limits>

using namespace cv;

// common

namespace
{
    double toRad(double a)
    {
        return a * CC_PI / 180.0;
    }

    bool notNull(float v)
    {
        return fabs(v) > std::numeric_limits<float>::epsilon();
    }

    class GeneralizedHoughBase
    {
    protected:
        GeneralizedHoughBase();
        virtual ~GeneralizedHoughBase() {}

        void setTemplateImpl(const img_t* templ, Point templCenter);
        void setTemplateImpl(const img_t* edges, const img_t* dx, const img_t* dy, Point templCenter);

        void detectImpl(const img_t* image, img_t* positions, img_t* votes);
        void detectImpl(const img_t* edges, const img_t* dx, const img_t* dy, img_t* positions, img_t* votes);

        virtual void processTempl() = 0;
        virtual void processImage() = 0;

        int cannyLowThresh_;
        int cannyHighThresh_;
        double minDist_;
        double dp_;

        CSize templSize_;
        Point templCenter_;
        img_t templEdges_;
        img_t templDx_;
        img_t templDy_;

        CSize imageSize_;
        img_t imageEdges_;
        img_t imageDx_;
        img_t imageDy_;

        std::vector<Vec4f> posOutBuf_;
        std::vector<Vec3i> voteOutBuf_;

    private:
        void calcEdges(const img_t* src, img_t& edges, img_t& dx, img_t& dy);
        void filterMinDist();
        void convertTo(img_t* positions, img_t* votes);
    };

    GeneralizedHoughBase::GeneralizedHoughBase()
    {
        cannyLowThresh_ = 50;
        cannyHighThresh_ = 100;
        minDist_ = 1.0;
        dp_ = 1.0;
    }

    void GeneralizedHoughBase::calcEdges(const img_t* _src, img_t& edges, img_t& dx, img_t& dy)
    {
        img_t *src = _src;

        CC_Assert( src->tid == CC_8UC1 );
        CC_Assert( cannyLowThresh_ > 0 && cannyLowThresh_ < cannyHighThresh_ );

        Canny(src, edges, cannyLowThresh_, cannyHighThresh_);
        Sobel(src, dx, CC_32F, 1, 0);
        Sobel(src, dy, CC_32F, 0, 1);
    }

    void GeneralizedHoughBase::setTemplateImpl(const img_t* templ, Point templCenter)
    {
        calcEdges(templ, templEdges_, templDx_, templDy_);

        if (templCenter == Point(-1, -1))
            templCenter = Point(templEdges_->cols / 2, templEdges_->rows / 2);

        templSize_ = templEdges_.size();
        templCenter_ = templCenter;

        processTempl();
    }

    void GeneralizedHoughBase::setTemplateImpl(const img_t* edges, const img_t* dx, const img_t* dy, Point templCenter)
    {
        edges.copyTo(templEdges_);
        dx.copyTo(templDx_);
        dy.copyTo(templDy_);

        CC_Assert( templEdges_->tid == CC_8UC1 );
        CC_Assert( templDx_->tid == CC_32FC1 && templDx_.size() == templEdges_.size() );
        CC_Assert( templDy_->tid == templDx_->tid && templDy_.size() == templEdges_.size() );

        if (templCenter == Point(-1, -1))
            templCenter = Point(templEdges_->cols / 2, templEdges_->rows / 2);

        templSize_ = templEdges_.size();
        templCenter_ = templCenter;

        processTempl();
    }

    void GeneralizedHoughBase::detectImpl(const img_t* image, img_t* positions, img_t* votes)
    {
        calcEdges(image, imageEdges_, imageDx_, imageDy_);

        imageSize_ = imageEdges_.size();

        posOutBuf_.clear();
        voteOutBuf_.clear();

        processImage();

        if (!posOutBuf_.empty())
        {
            if (minDist_ > 1)
                filterMinDist();
            convertTo(positions, votes);
        }
        else
        {
            positions.release();
            if (votes.needed())
                votes.release();
        }
    }

    void GeneralizedHoughBase::detectImpl(const img_t* edges, const img_t* dx, const img_t* dy, img_t* positions, img_t* votes)
    {
        edges.copyTo(imageEdges_);
        dx.copyTo(imageDx_);
        dy.copyTo(imageDy_);

        CC_Assert( imageEdges_->tid == CC_8UC1 );
        CC_Assert( imageDx_->tid == CC_32FC1 && imageDx_.size() == imageEdges_.size() );
        CC_Assert( imageDy_->tid == imageDx_->tid && imageDy_.size() == imageEdges_.size() );

        imageSize_ = imageEdges_.size();

        posOutBuf_.clear();
        voteOutBuf_.clear();

        processImage();

        if (!posOutBuf_.empty())
        {
            if (minDist_ > 1)
                filterMinDist();
            convertTo(positions, votes);
        }
        else
        {
            positions.release();
            if (votes.needed())
                votes.release();
        }
    }

    class Vec3iGreaterThanIdx
    {
    public:
        Vec3iGreaterThanIdx( const Vec3i* _arr ) : arr(_arr) {}
        bool operator()(size_t a, size_t b) const { return arr[a][0] > arr[b][0]; }
        const Vec3i* arr;
    };

    void GeneralizedHoughBase::filterMinDist()
    {
        size_t oldSize = posOutBuf_.size();
        const bool hasVotes = !voteOutBuf_.empty();

        CC_Assert( !hasVotes || voteOutBuf_.size() == oldSize );

        std::vector<Vec4f> oldPosBuf(posOutBuf_);
        std::vector<Vec3i> oldVoteBuf(voteOutBuf_);

        std::vector<size_t> indexies(oldSize);
        for (size_t i = 0; i < oldSize; ++i)
            indexies[i] = i;
        std::sort(indexies.begin(), indexies.end(), Vec3iGreaterThanIdx(&oldVoteBuf[0]));

        posOutBuf_.clear();
        voteOutBuf_.clear();

        const int cellSize = cRound(minDist_);
        const int gridWidth = (imageSize_.width + cellSize - 1) / cellSize;
        const int gridHeight = (imageSize_.height + cellSize - 1) / cellSize;

        std::vector< std::vector<CPoint2f> > grid(gridWidth * gridHeight);

        const double minDist2 = minDist_ * minDist_;

        for (size_t i = 0; i < oldSize; ++i)
        {
            const size_t ind = indexies[i];

            CPoint2f p(oldPosBuf[ind][0], oldPosBuf[ind][1]);

            bool good = true;

            const int xCell = static_cast<int>(p.x / cellSize);
            const int yCell = static_cast<int>(p.y / cellSize);

            int x1 = xCell - 1;
            int y1 = yCell - 1;
            int x2 = xCell + 1;
            int y2 = yCell + 1;

            // boundary check
            x1 = MAX(0, x1);
            y1 = MAX(0, y1);
            x2 = MIN(gridWidth - 1, x2);
            y2 = MIN(gridHeight - 1, y2);

            for (int yy = y1; yy <= y2; ++yy)
            {
                for (int xx = x1; xx <= x2; ++xx)
                {
                    const std::vector<CPoint2f>& m = grid[yy * gridWidth + xx];

                    for(size_t j = 0; j < m.size(); ++j)
                    {
                        const CPoint2f d = p - m[j];

                        if (d.ddot(d) < minDist2)
                        {
                            good = false;
                            goto break_out;
                        }
                    }
                }
            }

            break_out:

            if(good)
            {
                grid[yCell * gridWidth + xCell].push_back(p);

                posOutBuf_.push_back(oldPosBuf[ind]);
                if (hasVotes)
                    voteOutBuf_.push_back(oldVoteBuf[ind]);
            }
        }
    }

    void GeneralizedHoughBase::convertTo(img_t* positions, img_t* votes)
    {
        const int total = static_cast<int>(posOutBuf_.size());
        const bool hasVotes = !voteOutBuf_.empty();

        CC_Assert( !hasVotes || voteOutBuf_.size() == posOutBuf_.size() );

        _positions cvSetMat(1, total, CC_32FC4);
        img_t positions = _positions;
        img_t(1, total, CC_32FC4, &posOutBuf_[0]).copyTo(positions);

        if (_votes.needed())
        {
            if (!hasVotes)
            {
                _votes.release();
            }
            else
            {
                _votes cvSetMat(1, total, CC_32SC3);
                img_t votes = _votes;
                img_t(1, total, CC_32SC3, &voteOutBuf_[0]).copyTo(votes);
            }
        }
    }
}

// GeneralizedHoughBallard

namespace
{
    class GeneralizedHoughBallardImpl : public GeneralizedHoughBallard, private GeneralizedHoughBase
    {
    public:
        GeneralizedHoughBallardImpl();

        void setTemplate(const img_t* templ, Point templCenter) { setTemplateImpl(templ, templCenter); }
        void setTemplate(const img_t* edges, const img_t* dx, const img_t* dy, Point templCenter) { setTemplateImpl(edges, dx, dy, templCenter); }

        void detect(const img_t* image, img_t* positions, img_t* votes) { detectImpl(image, positions, votes); }
        void detect(const img_t* edges, const img_t* dx, const img_t* dy, img_t* positions, img_t* votes) { detectImpl(edges, dx, dy, positions, votes); }

        void setCannyLowThresh(int cannyLowThresh) { cannyLowThresh_ = cannyLowThresh; }
        int getCannyLowThresh() const { return cannyLowThresh_; }

        void setCannyHighThresh(int cannyHighThresh) { cannyHighThresh_ = cannyHighThresh; }
        int getCannyHighThresh() const { return cannyHighThresh_; }

        void setMinDist(double minDist) { minDist_ = minDist; }
        double getMinDist() const { return minDist_; }

        void setDp(double dp) { dp_ = dp; }
        double getDp() const { return dp_; }

        void setMaxBufferSize(int) {  }
        int getMaxBufferSize() const { return 0; }

        void setLevels(int levels) { levels_ = levels; }
        int getLevels() const { return levels_; }

        void setVotesThreshold(int votesThreshold) { votesThreshold_ = votesThreshold; }
        int getVotesThreshold() const { return votesThreshold_; }

    private:
        void processTempl();
        void processImage();

        void calcHist();
        void findPosInHist();

        int levels_;
        int votesThreshold_;

        std::vector< std::vector<Point> > r_table_;
        img_t hist_;
    };

    GeneralizedHoughBallardImpl::GeneralizedHoughBallardImpl()
    {
        levels_ = 360;
        votesThreshold_ = 100;
    }

    void GeneralizedHoughBallardImpl::processTempl()
    {
        CC_Assert( levels_ > 0 );

        const double thetaScale = levels_ / 360.0;

        r_table_.resize(levels_ + 1);
        std::for_each(r_table_.begin(), r_table_.end(), std::mem_fun_ref(&std::vector<Point>::clear));

        for (int y = 0; y < templSize_.height; ++y)
        {
            const uchar* edgesRow = templEdges_.ptr(y);
            const float* dxRow = templDx_.ptr<float>(y);
            const float* dyRow = templDy_.ptr<float>(y);

            for (int x = 0; x < templSize_.width; ++x)
            {
                const Point p(x, y);

                if (edgesRow[x] && (notNull(dyRow[x]) || notNull(dxRow[x])))
                {
                    const float theta = fastAtan2(dyRow[x], dxRow[x]);
                    const int n = cRound(theta * thetaScale);
                    r_table_[n].push_back(p - templCenter_);
                }
            }
        }
    }

    void GeneralizedHoughBallardImpl::processImage()
    {
        calcHist();
        findPosInHist();
    }

    void GeneralizedHoughBallardImpl::calcHist()
    {
        CC_INSTRUMENT_REGION()

        CC_Assert( imageEdges_->tid == CC_8UC1 );
        CC_Assert( imageDx_->tid == CC_32FC1 && imageDx_.size() == imageSize_);
        CC_Assert( imageDy_->tid == imageDx_->tid && imageDy_.size() == imageSize_);
        CC_Assert( levels_ > 0 && r_table_.size() == static_cast<size_t>(levels_ + 1) );
        CC_Assert( dp_ > 0.0 );

        const double thetaScale = levels_ / 360.0;
        const double idp = 1.0 / dp_;

        hist_ cvSetMat(cCeil(imageSize_.height * idp) + 2, cCeil(imageSize_.width * idp) + 2, CC_32SC1);
        hist_.setTo(0);

        const int rows = hist_->rows - 2;
        const int cols = hist_->cols - 2;

        for (int y = 0; y < imageSize_.height; ++y)
        {
            const uchar* edgesRow = imageEdges_.ptr(y);
            const float* dxRow = imageDx_.ptr<float>(y);
            const float* dyRow = imageDy_.ptr<float>(y);

            for (int x = 0; x < imageSize_.width; ++x)
            {
                const Point p(x, y);

                if (edgesRow[x] && (notNull(dyRow[x]) || notNull(dxRow[x])))
                {
                    const float theta = fastAtan2(dyRow[x], dxRow[x]);
                    const int n = cRound(theta * thetaScale);

                    const std::vector<Point>& r_row = r_table_[n];

                    for (size_t j = 0; j < r_row.size(); ++j)
                    {
                        Point c = p - r_row[j];

                        c.x = cRound(c.x * idp);
                        c.y = cRound(c.y * idp);

                        if (c.x >= 0 && c.x < cols && c.y >= 0 && c.y < rows)
                            ++hist_.at<int>(c.y + 1, c.x + 1);
                    }
                }
            }
        }
    }

    void GeneralizedHoughBallardImpl::findPosInHist()
    {
        CC_Assert( votesThreshold_ > 0 );

        const int histRows = hist_->rows - 2;
        const int histCols = hist_->cols - 2;

        for(int y = 0; y < histRows; ++y)
        {
            const int* prevRow = hist_.ptr<int>(y);
            const int* curRow = hist_.ptr<int>(y + 1);
            const int* nextRow = hist_.ptr<int>(y + 2);

            for(int x = 0; x < histCols; ++x)
            {
                const int votes = curRow[x + 1];

                if (votes > votesThreshold_ && votes > curRow[x] && votes >= curRow[x + 2] && votes > prevRow[x + 1] && votes >= nextRow[x + 1])
                {
                    posOutBuf_.push_back(Vec4f(static_cast<float>(x * dp_), static_cast<float>(y * dp_), 1.0f, 0.0f));
                    voteOutBuf_.push_back(Vec3i(votes, 0, 0));
                }
            }
        }
    }
}

Ptr<GeneralizedHoughBallard> createGeneralizedHoughBallard()
{
    return makePtr<GeneralizedHoughBallardImpl>();
}

// GeneralizedHoughGuil

namespace
{
    class GeneralizedHoughGuilImpl : public GeneralizedHoughGuil, private GeneralizedHoughBase
    {
    public:
        GeneralizedHoughGuilImpl();

        void setTemplate(const img_t* templ, Point templCenter) { setTemplateImpl(templ, templCenter); }
        void setTemplate(const img_t* edges, const img_t* dx, const img_t* dy, Point templCenter) { setTemplateImpl(edges, dx, dy, templCenter); }

        void detect(const img_t* image, img_t* positions, img_t* votes) { detectImpl(image, positions, votes); }
        void detect(const img_t* edges, const img_t* dx, const img_t* dy, img_t* positions, img_t* votes) { detectImpl(edges, dx, dy, positions, votes); }

        void setCannyLowThresh(int cannyLowThresh) { cannyLowThresh_ = cannyLowThresh; }
        int getCannyLowThresh() const { return cannyLowThresh_; }

        void setCannyHighThresh(int cannyHighThresh) { cannyHighThresh_ = cannyHighThresh; }
        int getCannyHighThresh() const { return cannyHighThresh_; }

        void setMinDist(double minDist) { minDist_ = minDist; }
        double getMinDist() const { return minDist_; }

        void setDp(double dp) { dp_ = dp; }
        double getDp() const { return dp_; }

        void setMaxBufferSize(int maxBufferSize) { maxBufferSize_ = maxBufferSize; }
        int getMaxBufferSize() const { return maxBufferSize_; }

        void setXi(double xi) { xi_ = xi; }
        double getXi() const { return xi_; }

        void setLevels(int levels) { levels_ = levels; }
        int getLevels() const { return levels_; }

        void setAngleEpsilon(double angleEpsilon) { angleEpsilon_ = angleEpsilon; }
        double getAngleEpsilon() const { return angleEpsilon_; }

        void setMinAngle(double minAngle) { minAngle_ = minAngle; }
        double getMinAngle() const { return minAngle_; }

        void setMaxAngle(double maxAngle) { maxAngle_ = maxAngle; }
        double getMaxAngle() const { return maxAngle_; }

        void setAngleStep(double angleStep) { angleStep_ = angleStep; }
        double getAngleStep() const { return angleStep_; }

        void setAngleThresh(int angleThresh) { angleThresh_ = angleThresh; }
        int getAngleThresh() const { return angleThresh_; }

        void setMinScale(double minScale) { minScale_ = minScale; }
        double getMinScale() const { return minScale_; }

        void setMaxScale(double maxScale) { maxScale_ = maxScale; }
        double getMaxScale() const { return maxScale_; }

        void setScaleStep(double scaleStep) { scaleStep_ = scaleStep; }
        double getScaleStep() const { return scaleStep_; }

        void setScaleThresh(int scaleThresh) { scaleThresh_ = scaleThresh; }
        int getScaleThresh() const { return scaleThresh_; }

        void setPosThresh(int posThresh) { posThresh_ = posThresh; }
        int getPosThresh() const { return posThresh_; }

    private:
        void processTempl();
        void processImage();

        int maxBufferSize_;
        double xi_;
        int levels_;
        double angleEpsilon_;

        double minAngle_;
        double maxAngle_;
        double angleStep_;
        int angleThresh_;

        double minScale_;
        double maxScale_;
        double scaleStep_;
        int scaleThresh_;

        int posThresh_;

        struct ContourPoint
        {
            CPoint2d pos;
            double theta;
        };

        struct Feature
        {
            ContourPoint p1;
            ContourPoint p2;

            double alpha12;
            double d12;

            CPoint2d r1;
            CPoint2d r2;
        };

        void buildFeatureList(const img_t& edges, const img_t& dx, const img_t& dy, std::vector< std::vector<Feature> >& features, CPoint2d center = CPoint2d());
        void getContourPoints(const img_t& edges, const img_t& dx, const img_t& dy, std::vector<ContourPoint>& points);

        void calcOrientation();
        void calcScale(double angle);
        void calcPosition(double angle, int angleVotes, double scale, int scaleVotes);

        std::vector< std::vector<Feature> > templFeatures_;
        std::vector< std::vector<Feature> > imageFeatures_;

        std::vector< std::pair<double, int> > angles_;
        std::vector< std::pair<double, int> > scales_;
    };

    double clampAngle(double a)
    {
        double res = a;

        while (res > 360.0)
            res -= 360.0;
        while (res < 0)
            res += 360.0;

        return res;
    }

    bool angleEq(double a, double b, double eps = 1.0)
    {
        return (fabs(clampAngle(a - b)) <= eps);
    }

    GeneralizedHoughGuilImpl::GeneralizedHoughGuilImpl()
    {
        maxBufferSize_ = 1000;
        xi_ = 90.0;
        levels_ = 360;
        angleEpsilon_ = 1.0;

        minAngle_ = 0.0;
        maxAngle_ = 360.0;
        angleStep_ = 1.0;
        angleThresh_ = 15000;

        minScale_ = 0.5;
        maxScale_ = 2.0;
        scaleStep_ = 0.05;
        scaleThresh_ = 1000;

        posThresh_ = 100;
    }

    void GeneralizedHoughGuilImpl::processTempl()
    {
        buildFeatureList(templEdges_, templDx_, templDy_, templFeatures_, templCenter_);
    }

    void GeneralizedHoughGuilImpl::processImage()
    {
        buildFeatureList(imageEdges_, imageDx_, imageDy_, imageFeatures_);

        calcOrientation();

        for (size_t i = 0; i < angles_.size(); ++i)
        {
            const double angle = angles_[i].first;
            const int angleVotes = angles_[i].second;

            calcScale(angle);

            for (size_t j = 0; j < scales_.size(); ++j)
            {
                const double scale = scales_[j].first;
                const int scaleVotes = scales_[j].second;

                calcPosition(angle, angleVotes, scale, scaleVotes);
            }
        }
    }

    void GeneralizedHoughGuilImpl::buildFeatureList(const img_t& edges, const img_t& dx, const img_t& dy, std::vector< std::vector<Feature> >& features, CPoint2d center)
    {
        CC_Assert( levels_ > 0 );

        const double maxDist = sqrt((double) templSize_.width * templSize_.width + templSize_.height * templSize_.height) * maxScale_;

        const double alphaScale = levels_ / 360.0;

        std::vector<ContourPoint> points;
        getContourPoints(edges, dx, dy, points);

        features.resize(levels_ + 1);
        std::for_each(features.begin(), features.end(), std::mem_fun_ref(&std::vector<Feature>::clear));
        std::for_each(features.begin(), features.end(), std::bind2nd(std::mem_fun_ref(&std::vector<Feature>::reserve), maxBufferSize_));

        for (size_t i = 0; i < points.size(); ++i)
        {
            ContourPoint p1 = points[i];

            for (size_t j = 0; j < points.size(); ++j)
            {
                ContourPoint p2 = points[j];

                if (angleEq(p1.theta - p2.theta, xi_, angleEpsilon_))
                {
                    const CPoint2d d = p1.pos - p2.pos;

                    Feature f;

                    f.p1 = p1;
                    f.p2 = p2;

                    f.alpha12 = clampAngle(fastAtan2((float)d.y, (float)d.x) - p1.theta);
                    f.d12 = norm(d);

                    if (f.d12 > maxDist)
                        continue;

                    f.r1 = p1.pos - center;
                    f.r2 = p2.pos - center;

                    const int n = cRound(f.alpha12 * alphaScale);

                    if (features[n].size() < static_cast<size_t>(maxBufferSize_))
                        features[n].push_back(f);
                }
            }
        }
    }

    void GeneralizedHoughGuilImpl::getContourPoints(const img_t& edges, const img_t& dx, const img_t& dy, std::vector<ContourPoint>& points)
    {
        CC_Assert( edges->tid == CC_8UC1 );
        CC_Assert( dx->tid == CC_32FC1 && dx.size == edges.size );
        CC_Assert( dy->tid == dx->tid && dy.size == edges.size );

        points.clear();
        points.reserve(edges.size().area());

        for (int y = 0; y < edges->rows; ++y)
        {
            const uchar* edgesRow = edges.ptr(y);
            const float* dxRow = dx.ptr<float>(y);
            const float* dyRow = dy.ptr<float>(y);

            for (int x = 0; x < edges->cols; ++x)
            {
                if (edgesRow[x] && (notNull(dyRow[x]) || notNull(dxRow[x])))
                {
                    ContourPoint p;

                    p.pos = CPoint2d(x, y);
                    p.theta = fastAtan2(dyRow[x], dxRow[x]);

                    points.push_back(p);
                }
            }
        }
    }

    void GeneralizedHoughGuilImpl::calcOrientation()
    {
        CC_Assert( levels_ > 0 );
        CC_Assert( templFeatures_.size() == static_cast<size_t>(levels_ + 1) );
        CC_Assert( imageFeatures_.size() == templFeatures_.size() );
        CC_Assert( minAngle_ >= 0.0 && minAngle_ < maxAngle_ && maxAngle_ <= 360.0 );
        CC_Assert( angleStep_ > 0.0 && angleStep_ < 360.0 );
        CC_Assert( angleThresh_ > 0 );

        const double iAngleStep = 1.0 / angleStep_;
        const int angleRange = cCeil((maxAngle_ - minAngle_) * iAngleStep);

        std::vector<int> OHist(angleRange + 1, 0);
        for (int i = 0; i <= levels_; ++i)
        {
            const std::vector<Feature>& templRow = templFeatures_[i];
            const std::vector<Feature>& imageRow = imageFeatures_[i];

            for (size_t j = 0; j < templRow.size(); ++j)
            {
                Feature templF = templRow[j];

                for (size_t k = 0; k < imageRow.size(); ++k)
                {
                    Feature imF = imageRow[k];

                    const double angle = clampAngle(imF.p1.theta - templF.p1.theta);
                    if (angle >= minAngle_ && angle <= maxAngle_)
                    {
                        const int n = cRound((angle - minAngle_) * iAngleStep);
                        ++OHist[n];
                    }
                }
            }
        }

        angles_.clear();

        for (int n = 0; n < angleRange; ++n)
        {
            if (OHist[n] >= angleThresh_)
            {
                const double angle = minAngle_ + n * angleStep_;
                angles_.push_back(std::make_pair(angle, OHist[n]));
            }
        }
    }

    void GeneralizedHoughGuilImpl::calcScale(double angle)
    {
        CC_Assert( levels_ > 0 );
        CC_Assert( templFeatures_.size() == static_cast<size_t>(levels_ + 1) );
        CC_Assert( imageFeatures_.size() == templFeatures_.size() );
        CC_Assert( minScale_ > 0.0 && minScale_ < maxScale_ );
        CC_Assert( scaleStep_ > 0.0 );
        CC_Assert( scaleThresh_ > 0 );

        const double iScaleStep = 1.0 / scaleStep_;
        const int scaleRange = cCeil((maxScale_ - minScale_) * iScaleStep);

        std::vector<int> SHist(scaleRange + 1, 0);

        for (int i = 0; i <= levels_; ++i)
        {
            const std::vector<Feature>& templRow = templFeatures_[i];
            const std::vector<Feature>& imageRow = imageFeatures_[i];

            for (size_t j = 0; j < templRow.size(); ++j)
            {
                Feature templF = templRow[j];

                templF.p1.theta += angle;

                for (size_t k = 0; k < imageRow.size(); ++k)
                {
                    Feature imF = imageRow[k];

                    if (angleEq(imF.p1.theta, templF.p1.theta, angleEpsilon_))
                    {
                        const double scale = imF.d12 / templF.d12;
                        if (scale >= minScale_ && scale <= maxScale_)
                        {
                            const int s = cRound((scale - minScale_) * iScaleStep);
                            ++SHist[s];
                        }
                    }
                }
            }
        }

        scales_.clear();

        for (int s = 0; s < scaleRange; ++s)
        {
            if (SHist[s] >= scaleThresh_)
            {
                const double scale = minScale_ + s * scaleStep_;
                scales_.push_back(std::make_pair(scale, SHist[s]));
            }
        }
    }

    void GeneralizedHoughGuilImpl::calcPosition(double angle, int angleVotes, double scale, int scaleVotes)
    {
        CC_Assert( levels_ > 0 );
        CC_Assert( templFeatures_.size() == static_cast<size_t>(levels_ + 1) );
        CC_Assert( imageFeatures_.size() == templFeatures_.size() );
        CC_Assert( dp_ > 0.0 );
        CC_Assert( posThresh_ > 0 );

        const double sinVal = sin(toRad(angle));
        const double cosVal = cos(toRad(angle));
        const double idp = 1.0 / dp_;

        const int histRows = cCeil(imageSize_.height * idp);
        const int histCols = cCeil(imageSize_.width * idp);

        img_t DHist(histRows + 2, histCols + 2, CC_32SC1, Scalar::all(0));

        for (int i = 0; i <= levels_; ++i)
        {
            const std::vector<Feature>& templRow = templFeatures_[i];
            const std::vector<Feature>& imageRow = imageFeatures_[i];

            for (size_t j = 0; j < templRow.size(); ++j)
            {
                Feature templF = templRow[j];

                templF.p1.theta += angle;

                templF.r1 *= scale;
                templF.r2 *= scale;

                templF.r1 = CPoint2d(cosVal * templF.r1.x - sinVal * templF.r1.y, sinVal * templF.r1.x + cosVal * templF.r1.y);
                templF.r2 = CPoint2d(cosVal * templF.r2.x - sinVal * templF.r2.y, sinVal * templF.r2.x + cosVal * templF.r2.y);

                for (size_t k = 0; k < imageRow.size(); ++k)
                {
                    Feature imF = imageRow[k];

                    if (angleEq(imF.p1.theta, templF.p1.theta, angleEpsilon_))
                    {
                        CPoint2d c1, c2;

                        c1 = imF.p1.pos - templF.r1;
                        c1 *= idp;

                        c2 = imF.p2.pos - templF.r2;
                        c2 *= idp;

                        if (fabs(c1.x - c2.x) > 1 || fabs(c1.y - c2.y) > 1)
                            continue;

                        if (c1.y >= 0 && c1.y < histRows && c1.x >= 0 && c1.x < histCols)
                            ++DHist.at<int>(cRound(c1.y) + 1, cRound(c1.x) + 1);
                    }
                }
            }
        }

        for(int y = 0; y < histRows; ++y)
        {
            const int* prevRow = DHist.ptr<int>(y);
            const int* curRow = DHist.ptr<int>(y + 1);
            const int* nextRow = DHist.ptr<int>(y + 2);

            for(int x = 0; x < histCols; ++x)
            {
                const int votes = curRow[x + 1];

                if (votes > posThresh_ && votes > curRow[x] && votes >= curRow[x + 2] && votes > prevRow[x + 1] && votes >= nextRow[x + 1])
                {
                    posOutBuf_.push_back(Vec4f(static_cast<float>(x * dp_), static_cast<float>(y * dp_), static_cast<float>(scale), static_cast<float>(angle)));
                    voteOutBuf_.push_back(Vec3i(votes, scaleVotes, angleVotes));
                }
            }
        }
    }
}

Ptr<GeneralizedHoughGuil> createGeneralizedHoughGuil()
{
    return makePtr<GeneralizedHoughGuilImpl>();
}
