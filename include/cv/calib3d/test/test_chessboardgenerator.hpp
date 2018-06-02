#ifndef CC_CHESSBOARDGENERATOR_H143KJTVYM389YTNHKFDHJ89NYVMO3VLMEJNTBGUEIYVCM203P
#define CC_CHESSBOARDGENERATOR_H143KJTVYM389YTNHKFDHJ89NYVMO3VLMEJNTBGUEIYVCM203P

#include "opencv2/calib3d.hpp"

namespace cv
{

class ChessBoardGenerator
{
public:
    double sensorWidth;
    double sensorHeight;
    size_t squareEdgePointsNum;
    double min_cos;
    mutable double cov;
    CvSize patternSize;
    int rendererResolutionMultiplier;

    ChessBoardGenerator(const CvSize& patternSize = CvSize(8, 6));
    CvMat operator()(const CvMat& bg, const CvMat& camMat, const CvMat& distCoeffs, std::vector<Point2f>& corners) const;
    CvMat operator()(const CvMat& bg, const CvMat& camMat, const CvMat& distCoeffs, const Size2f& squareSize, std::vector<Point2f>& corners) const;
    CvMat operator()(const CvMat& bg, const CvMat& camMat, const CvMat& distCoeffs, const Size2f& squareSize, const Point3f& pos, std::vector<Point2f>& corners) const;
    CvSize cornersSize() const;

    mutable std::vector<Point3f> corners3d;
private:
    void generateEdge(const Point3f& p1, const Point3f& p2, std::vector<Point3f>& out) const;
    CvMat generateChessBoard(const CvMat& bg, const CvMat& camMat, const CvMat& distCoeffs,
        const Point3f& zero, const Point3f& pb1, const Point3f& pb2,
        float sqWidth, float sqHeight, const std::vector<Point3f>& whole, std::vector<Point2f>& corners) const;
    void generateBasis(Point3f& pb1, Point3f& pb2) const;

    CvMat rvec, tvec;
};

}


#endif
