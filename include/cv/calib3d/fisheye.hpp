#ifndef FISHEYE_INTERNAL_H
#define FISHEYE_INTERNAL_H


namespace cv { namespace internal {

struct CC_EXPORTS IntrinsicParams
{
    Vec2d f;
    Vec2d c;
    Vec4d k;
    double alpha;
    std::vector<uchar> isEstimate;

    IntrinsicParams();
    IntrinsicParams(Vec2d f, Vec2d c, Vec4d k, double alpha = 0);
    IntrinsicParams operator+(const CvMat& a);
    IntrinsicParams& operator =(const CvMat& a);
    void Init(const Vec2d& f, const Vec2d& c, const Vec4d& k = Vec4d(0,0,0,0), const double& alpha = 0);
};

void projectPoints(const CvArr* objectPoints, CvArr* imagePoints,
                   const CvArr* _rvec,const CvArr* _tvec,
                   const IntrinsicParams& param, CvArr* jacobian);

void ComputeExtrinsicRefine(const CvMat& imagePoints, const CvMat& objectPoints, CvMat& rvec,
                            CvMat&  tvec, CvMat& J, const int MaxIter,
                            const IntrinsicParams& param, const double thresh_cond);
CC_EXPORTS CvMat ComputeHomography(CvMat m, CvMat M);

CC_EXPORTS CvMat NormalizePixels(const CvMat& imagePoints, const IntrinsicParams& param);

void InitExtrinsics(const CvMat& _imagePoints, const CvMat& _objectPoints, const IntrinsicParams& param, CvMat& omckk, CvMat& Tckk);

void CalibrateExtrinsics(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints,
                         const IntrinsicParams& param, const int check_cond,
                         const double thresh_cond, InputOutputArray omc, InputOutputArray Tc);

void ComputeJacobians(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints,
                      const IntrinsicParams& param,  const CvArr* omc, const CvArr* Tc,
                      const int& check_cond, const double& thresh_cond, CvMat& JJ2_inv, CvMat& ex3);

CC_EXPORTS void  EstimateUncertainties(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints,
                           const IntrinsicParams& params, const CvArr* omc, const CvArr* Tc,
                           IntrinsicParams& errors, Vec2d& std_err, double thresh_cond, int check_cond, double& rms);

void dAB(const CvArr* A, const CvArr* B, CvArr* dABdA, CvArr* dABdB);

void JRodriguesMatlab(const CvMat& src, CvMat& dst);

void compose_motion(const CvArr* _om1, const CvArr* _T1, const CvArr* _om2, const CvArr* _T2,
                    CvMat& om3, CvMat& T3, CvMat& dom3dom1, CvMat& dom3dT1, CvMat& dom3dom2,
                    CvMat& dom3dT2, CvMat& dT3dom1, CvMat& dT3dT1, CvMat& dT3dom2, CvMat& dT3dT2);

double median(const CvMat& row);

Vec3d median3d(const CvArr* m);

}}

#endif
