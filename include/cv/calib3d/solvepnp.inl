
// #include "upnp.h"
// #include "dls.h"
// #include "epnp.h"
// #include "p3p.h"
// #include "calib3d_c.h"

enum { CC_SOLVEPNP_ITERATIVE = 0,
       CC_SOLVEPNP_EPNP      = 1, //!< EPnP: Efficient Perspective-n-Point Camera Pose Estimation @cite lepetit2009epnp
       CC_SOLVEPNP_P3P       = 2, //!< Complete Solution Classification for the Perspective-Three-Point Problem @cite gao2003complete
       CC_SOLVEPNP_DLS       = 3, //!< A Direct Least-Squares (DLS) Method for PnP  @cite hesch2011direct
       CC_SOLVEPNP_UPNP      = 4  //!< Exhaustive Linearization for Robust Camera Pose and Focal Length Estimation @cite penate2013exhaustive

};

static bool solvePnP1( const img_t* opoints, const img_t* ipoints, const img_t* cameraMatrix,
               const img_t* distCoeffs, img_t* rvec, img_t* tvec,
               bool useExtrinsicGuess CC_DEFAULT(false), int flags CC_DEFAULT(CC_SOLVEPNP_ITERATIVE))
{
    bool result = false;
    int npoints = MAX(cvCheckVector(opoints, 3, CC_32F), cvCheckVector(opoints, 3, CC_64F));
    CC_ASSERT( npoints >= 0 && npoints == MAX(cvCheckVector(ipoints, 2, CC_32F), cvCheckVector(ipoints, 2, CC_64F)) );

    if( flags != CC_SOLVEPNP_ITERATIVE )
        useExtrinsicGuess = false;

    IM_BEGIN(im, 10);
    if( useExtrinsicGuess )
    {
        TypeId rtype = CC_MAT_DEPTH(rvec), ttype = CC_MAT_DEPTH(tvec);
        CSize rsize = cvGetSize(rvec), tsize = cvGetSize(tvec);
        CC_ASSERT( (rtype == CC_32F || rtype == CC_64F) &&
                   (ttype == CC_32F || ttype == CC_64F) );
        CC_ASSERT( (rsize.w*rsize.h == 3) && (tsize.w*tsize.h == 3) );
    }
    else
    {
        cvSetMat(rvec, 3, 1, CC_64F);
        cvSetMat(tvec, 3, 1, CC_64F);
    }

    if (flags == CC_SOLVEPNP_EPNP || flags == CC_SOLVEPNP_DLS || flags == CC_SOLVEPNP_UPNP)
    {
#if 0
        img_t undistortedPoints[1] = {0};
        undistortPoints(ipoints, undistortedPoints, cameraMatrix, distCoeffs);
        epnp PnP(cameraMatrix, opoints, undistortedPoints);

        img_t *R = im_++;
        PnP.compute_pose(R, tvec);
        Rodrigues(R, rvec);
        result = true;
#endif
    }
    else if (flags == CC_SOLVEPNP_P3P)
    {
#if 0
        CC_Assert( npoints == 4);
        img_t undistortedPoints;
        undistortPoints(ipoints, undistortedPoints, cameraMatrix, distCoeffs);
        p3p P3Psolver(cameraMatrix);

        img_t R;
        result = P3Psolver.solve(R, tvec, opoints, undistortedPoints);
        if (result)
            Rodrigues(R, rvec);
#endif
    }
    else if (flags == CC_SOLVEPNP_ITERATIVE)
    {
        const img_t* c_objectPoints = opoints, *c_imagePoints = ipoints;
        const img_t *c_cameraMatrix = cameraMatrix, *c_distCoeffs = distCoeffs;
        img_t *c_rvec = rvec, *c_tvec = tvec;
        cvFindExtrinsicCameraParams2(c_objectPoints, c_imagePoints, c_cameraMatrix,
                                     (c_distCoeffs && c_distCoeffs->rows*c_distCoeffs->cols) ? c_distCoeffs : 0,
                                     c_rvec, c_tvec, useExtrinsicGuess );
        result = true;
    }
    /*else if (flags == CC_SOLVEPNP_DLS)
    {
        img_t undistortedPoints;
        undistortPoints(ipoints, undistortedPoints, cameraMatrix, distCoeffs);

        dls PnP(opoints, undistortedPoints);

        img_t R, rvec = _rvec, tvec = _tvec;
        bool result = PnP.compute_pose(R, tvec);
        if (result)
            Rodrigues(R, rvec);
        return result;
    }
    else if (flags == CC_SOLVEPNP_UPNP)
    {
        upnp PnP(cameraMatrix, opoints, ipoints);

        img_t R, rvec = _rvec, tvec = _tvec;
        PnP.compute_pose(R, tvec);
        Rodrigues(R, rvec);
        return true;
    }*/
    else
        CC_Error(CC_StsBadArg, "The flags argument must be one of CC_SOLVEPNP_ITERATIVE, CC_SOLVEPNP_P3P, CC_SOLVEPNP_EPNP or CC_SOLVEPNP_DLS");

    IM_END(im);
    return result;
}

#if 0
#define MATDEF(name, rows, cols, type, cn, ptr, step)  img_t _##name [1] = {0}, *name = ptr ? cvInitMatHeader(_##name, rows, cols, type, cn, ptr, step) : NULL

bool icvSolvePnP( const void* opoints, const void* ipoints,
               const void* cameraMatrix, const void* distCoeffs,
               void* rvec, void* tvec, bool useExtrinsicGuess, int flags, int len, TypeId type ) {

  int elem_size = CC_ELEM_SIZE(type);
  MATDEF(opoints_, len, 3, type, 1, opoints, 3*elem_size);
  MATDEF(ipoints_, len, 2, type, 1, ipoints, 2*elem_size);
  MATDEF(cameraMatrix_, 3, 3, type, 1, cameraMatrix, 3*elem_size);
  MATDEF(distCoeffs_, 3, 3, type, 1, distCoeffs, 3*elem_size);
  MATDEF(rvec_, 3, 1, type, 1, rvec, 1*elem_size);
  MATDEF(tvec_, 3, 1, type, 1, tvec, 1*elem_size);
  return solvePnP1(opoints_, ipoints_, cameraMatrix_, distCoeffs ? distCoeffs_ : NULL, rvec_, tvec_, useExtrinsicGuess, flags);
}


class PnPRansacCallback : public PointSetRegistrator::Callback
{

public:

    PnPRansacCallback(img_t _cameraMatrix=img_t(3,3,CC_64F), img_t _distCoeffs=img_t(4,1,CC_64F), int _flags=CC_SOLVEPNP_ITERATIVE,
            bool _useExtrinsicGuess=false, img_t _rvec=img_t(), img_t _tvec=img_t() )
        : cameraMatrix(_cameraMatrix), distCoeffs(_distCoeffs), flags(_flags), useExtrinsicGuess(_useExtrinsicGuess),
          rvec(_rvec), tvec(_tvec) {}

    /* Pre: True */
    /* Post: compute _model with given points and return number of found models */
    int runKernel( const img_t* _m1, const img_t* _m2, img_t* _model ) const
    {
        img_t opoints = _m1, ipoints = _m2;

        bool correspondence = solvePnP( _m1, _m2, cameraMatrix, distCoeffs,
                                            rvec, tvec, useExtrinsicGuess, flags );

        img_t _local_model;
        hconcat(rvec, tvec, _local_model);
        _local_model.copyTo(_model);

        return correspondence;
    }

    /* Pre: True */
    /* Post: fill _err with projection errors */
    void computeError( const img_t* _m1, const img_t* _m2, const img_t* _model, img_t* _err ) const
    {

        img_t opoints = _m1, ipoints = _m2, model = _model;

        int i, count = opoints cvCheckVector(3);
        img_t _rvec = model.col(0);
        img_t _tvec = model.col(1);


        img_t projpoints(count, 2, CC_32FC1);
        projectPoints(opoints, _rvec, _tvec, cameraMatrix, distCoeffs, projpoints);

        const CPoint2f* ipoints_ptr = ipoints(CPoint2f*)(->tt.data);
        const CPoint2f* projpoints_ptr = projpoints(CPoint2f*)(->tt.data);

        _err cvSetMat(count, 1, CC_32FC1);
        float* err = _img_ptr(float, err);

        for ( i = 0; i < count; ++i)
            err[i] = (float)norm( Matx21f(ipoints_ptr[i] - projpoints_ptr[i]), NORM_L2SQR );

    }


    img_t cameraMatrix;
    img_t distCoeffs;
    int flags;
    bool useExtrinsicGuess;
    img_t rvec;
    img_t tvec;
};

bool solvePnPRansac(const img_t* _opoints, const img_t* _ipoints,
                        const img_t* _cameraMatrix, const img_t* _distCoeffs,
                        img_t* _rvec, img_t* _tvec, bool useExtrinsicGuess,
                        int iterationsCount, float reprojectionError, double confidence,
                        img_t* _inliers, int flags)
{
    CC_INSTRUMENT_REGION()

    img_t opoints0 = _opoints, ipoints0 = _ipoints;
    img_t opoints, ipoints;
    if( opoints0 CC_MAT_DEPTH() == CC_64F || !opoints0 CC_IS_CONT_MAT() )
        opoints0.convertTo(opoints, CC_32F);
    else
        opoints = opoints0;
    if( ipoints0 CC_MAT_DEPTH() == CC_64F || !ipoints0 CC_IS_CONT_MAT() )
        ipoints0.convertTo(ipoints, CC_32F);
    else
        ipoints = ipoints0;

    int npoints = MAX(opoints cvCheckVector(3, CC_32F), opoints cvCheckVector(3, CC_64F));
    CC_Assert( npoints >= 0 && npoints == MAX(ipoints cvCheckVector(2, CC_32F), ipoints cvCheckVector(2, CC_64F)) );

    CC_Assert(opoints CC_IS_CONT_MAT());
    CC_Assert(opoints CC_MAT_DEPTH() == CC_32F || opoints CC_MAT_DEPTH() == CC_64F);
    CC_Assert((opoints->rows == 1 && opoints CC_MAT_CN() == 3) || opoints->cols*opoints CC_MAT_CN() == 3);
    CC_Assert(ipoints CC_IS_CONT_MAT());
    CC_Assert(ipoints CC_MAT_DEPTH() == CC_32F || ipoints CC_MAT_DEPTH() == CC_64F);
    CC_Assert((ipoints->rows == 1 && ipoints CC_MAT_CN() == 2) || ipoints->cols*ipoints CC_MAT_CN() == 2);

    _rvec cvSetMat(3, 1, CC_64FC1);
    _tvec cvSetMat(3, 1, CC_64FC1);

    img_t rvec = useExtrinsicGuess ? _rvec : img_t(3, 1, CC_64FC1);
    img_t tvec = useExtrinsicGuess ? _tvec : img_t(3, 1, CC_64FC1);
    img_t cameraMatrix = _cameraMatrix, distCoeffs = _distCoeffs;

    int model_points = 5;
    int ransac_kernel_method = CC_SOLVEPNP_EPNP;

    if( npoints == 4 )
    {
        model_points = 4;
        ransac_kernel_method = CC_SOLVEPNP_P3P;
    }

    Ptr<PointSetRegistrator::Callback> cb; // pointer to callback
    cb = makePtr<PnPRansacCallback>( cameraMatrix, distCoeffs, ransac_kernel_method, useExtrinsicGuess, rvec, tvec);

    double param1 = reprojectionError;                // reprojection error
    double param2 = confidence;                       // confidence
    int param3 = iterationsCount;                     // number maximum iterations

    img_t _local_model(3, 2, CC_64FC1);
    img_t _mask_local_inliers(1, opoints->rows, CC_8UC1);

    // call Ransac
    int result = createRANSACPointSetRegistrator(cb, model_points,
        param1, param2, param3)->run(opoints, ipoints, _local_model, _mask_local_inliers);

    if( result > 0 )
    {
        vector<CPoint3d> opoints_inliers;
        vector<CPoint2d> ipoints_inliers;
        opoints = opoints cvReshape(,3);
        ipoints = ipoints cvReshape(,2);
        opoints.convertTo(opoints_inliers, CC_64F);
        ipoints.convertTo(ipoints_inliers, CC_64F);

        const uchar* mask = _mask_local_inliers img_ptr(uchar, );
        int npoints1 = compressElems(&opoints_inliers[0], mask, 1, npoints);
        compressElems(&ipoints_inliers[0], mask, 1, npoints);

        opoints_inliers.resize(npoints1);
        ipoints_inliers.resize(npoints1);
        result = solvePnP(opoints_inliers, ipoints_inliers, cameraMatrix,
                          distCoeffs, rvec, tvec, false, flags == CC_SOLVEPNP_P3P ? CC_SOLVEPNP_EPNP : flags) ? 1 : -1;
    }

    if( result <= 0 || _local_model->rows <= 0)
    {
        _rvec.assign(rvec);    // output rotation vector
        _tvec.assign(tvec);    // output translation vector

        if( _inliers.needed() )
            _inliers.release();

        return false;
    }
    else
    {
        _rvec.assign(_local_model.col(0));    // output rotation vector
        _tvec.assign(_local_model.col(1));    // output translation vector
    }

    if(_inliers.needed())
    {
        img_t _local_inliers;
        for (int i = 0; i < npoints; ++i)
        {
            if((int)_mask_local_inliers.at<uchar>(i) != 0) // inliers mask
                _local_inliers.push_back(i);    // output inliers vector
        }
        _local_inliers.copyTo(_inliers);
    }
    return true;
}
#endif

