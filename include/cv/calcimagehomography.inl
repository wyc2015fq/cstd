
/****************************************************************************************\

   calculate image homography 
   
\****************************************************************************************/

CC_IMPL void
cvCalcImageHomography(float* line, CPoint3D32f* _center,
                       float* _intrinsic, float* _homography)
{
    CC_FUNCNAME("cvCalcImageHomography");

    __BEGIN__;

    double norm_xy, norm_xz, xy_sina, xy_cosa, xz_sina, xz_cosa, nx1, plane_dist;
    float _ry[3], _rz[3], _r_trans[9];
    img_t rx = cvMat(1, 3, CC_32F, line);
    img_t ry = cvMat(1, 3, CC_32F, _ry);
    img_t rz = cvMat(1, 3, CC_32F, _rz);
    img_t r_trans = cvMat(3, 3, CC_32F, _r_trans);
    img_t center = cvMat(3, 1, CC_32F, _center);

    float _sub[9];
    img_t sub = cvMat(3, 3, CC_32F, _sub);
    float _t_trans[3];
    img_t t_trans = cvMat(3, 1, CC_32F, _t_trans);

    img_t intrinsic = cvMat(3, 3, CC_32F, _intrinsic);
    img_t homography = cvMat(3, 3, CC_32F, _homography);

    if(!line || !_center || !_intrinsic || !_homography)
        CC_ERROR(CC_StsNullPtr, "");

    norm_xy = cvSqrt(line[0] * line[0] + line[1] * line[1]);
    xy_cosa = line[0] / norm_xy;
    xy_sina = line[1] / norm_xy;

    norm_xz = cvSqrt(line[0] * line[0] + line[2] * line[2]);
    xz_cosa = line[0] / norm_xz;
    xz_sina = line[2] / norm_xz;

    nx1 = -xz_sina;

    _rz[0] = (float)(xy_cosa * nx1);
    _rz[1] = (float)(xy_sina * nx1);
    _rz[2] = (float)xz_cosa;
    cvScale(&rz, &rz, 1./cvNorm(&rz,0,CC_L2));

    /*  new axe  y  */
    cvCrossProduct(&rz, &rx, &ry);
    cvScale(&ry, &ry, 1./cvNorm(&ry, 0, CC_L2));

    /*  transpone rotation matrix    */
    memcpy(&_r_trans[0], line, 3*sizeof(float));
    memcpy(&_r_trans[3], _ry, 3*sizeof(float));
    memcpy(&_r_trans[6], _rz, 3*sizeof(float));

    /*  calculate center distanse from arm plane  */
    plane_dist = cvDotProduct(&center, &rz);

    /* calculate (I - r_trans)*center */
    cvSetIdentity(&sub);
    cvSub(&sub, &r_trans, &sub);
    cvMatMul(&sub, &center, &t_trans); 

    cvMatMul(&t_trans, &rz, &sub);
    cvScaleAdd(&sub, cRealScalar(1./plane_dist), &r_trans, &sub); /* ? */

    cvMatMul(&intrinsic, &sub, &r_trans);
    cvInvert(&intrinsic, &sub, CC_SVD);
    cvMatMul(&r_trans, &sub, &homography);

    __END__;
}



