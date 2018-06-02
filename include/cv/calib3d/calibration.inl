
//#include "detail/distortion_model.hpp"
//#include "calib3d_c.h"

/*
    This is stright-forward port v3 of Matlab calibration engine by Jean-Yves Bouguet
    that is (in a large extent) based on the paper:
    Z. Zhang. "A flexible new technique for camera calibration".
    IEEE Transactions on Pattern Analysis and Machine Intelligence, 22(11):1330-1334, 2000.
    The 1st initial port was done by Valery Mosyagin.
*/

// reimplementation of dAB.m
CC_IMPL void cvCalcMatMulDeriv( const img_t* A, const img_t* B, img_t* dABdA, img_t* dABdB )
{
    int i, j, M, N, L;
    int bstep;

    CC_Assert( (A) && (B) );
    CC_Assert( CC_ARE_TYPES_EQ(A, B) &&
        (CC_MAT_TYPE(A) == CC_32F || CC_MAT_TYPE(A) == CC_64F) );
    CC_Assert( A->cols == B->rows );

    M = A->rows;
    L = A->cols;
    N = B->cols;
    bstep = B->step/CC_TYPE_SIZE(B->tid);

    if( dABdA )
    {
        CC_Assert( CC_ARE_TYPES_EQ(A, dABdA) &&
            dABdA->rows == A->rows*B->cols && dABdA->cols == A->rows*A->cols );
    }

    if( dABdB )
    {
        CC_Assert( CC_ARE_TYPES_EQ(A, dABdB) &&
            dABdB->rows == A->rows*B->cols && dABdB->cols == B->rows*B->cols );
    }

    if( CC_MAT_TYPE(A) == CC_32F )
    {
        for( i = 0; i < M*N; i++ )
        {
            int i1 = i / N,  i2 = i % N;

            if( dABdA )
            {
                float* dcda = (float*)(dABdA->tt.data + dABdA->step*i);
                const float* b = (const float*)B->tt.data + i2;

                for( j = 0; j < M*L; j++ )
                    dcda[j] = 0;
                for( j = 0; j < L; j++ )
                    dcda[i1*L + j] = b[j*bstep];
            }

            if( dABdB )
            {
                float* dcdb = (float*)(dABdB->tt.data + dABdB->step*i);
                const float* a = (const float*)(A->tt.data + A->step*i1);

                for( j = 0; j < L*N; j++ )
                    dcdb[j] = 0;
                for( j = 0; j < L; j++ )
                    dcdb[j*N + i2] = a[j];
            }
        }
    }
    else
    {
        for( i = 0; i < M*N; i++ )
        {
            int i1 = i / N,  i2 = i % N;

            if( dABdA )
            {
                double* dcda = (double*)(dABdA->tt.data + dABdA->step*i);
                const double* b = (const double*)B->tt.data + i2;

                for( j = 0; j < M*L; j++ )
                    dcda[j] = 0;
                for( j = 0; j < L; j++ )
                    dcda[i1*L + j] = b[j*bstep];
            }

            if( dABdB )
            {
                double* dcdb = (double*)(dABdB->tt.data + dABdB->step*i);
                const double* a = (const double*)(A->tt.data + A->step*i1);

                for( j = 0; j < L*N; j++ )
                    dcdb[j] = 0;
                for( j = 0; j < L; j++ )
                    dcdb[j*N + i2] = a[j];
            }
        }
    }
}

#if 0

// reimplementation of compose_motion.m
CC_IMPL void cvComposeRT( const img_t* _rvec1, const img_t* _tvec1,
             const img_t* _rvec2, const img_t* _tvec2,
             img_t* _rvec3, img_t* _tvec3,
             img_t* dr3dr1, img_t* dr3dt1,
             img_t* dr3dr2, img_t* dr3dt2,
             img_t* dt3dr1, img_t* dt3dt1,
             img_t* dt3dr2, img_t* dt3dt2 )
{
    double _r1[3], _r2[3];
    double _R1[9], _d1[9*3], _R2[9], _d2[9*3];
    img_t r1 = cvMat1(3,1,CC_64F,_r1), r2 = cvMat1(3,1,CC_64F,_r2);
    img_t R1 = cvMat1(3,3,CC_64F,_R1), R2 = cvMat1(3,3,CC_64F,_R2);
    img_t dR1dr1 = cvMat1(9,3,CC_64F,_d1), dR2dr2 = cvMat1(9,3,CC_64F,_d2);

    CC_Assert( (_rvec1) && (_rvec2) );

    CC_Assert( CC_MAT_TYPE(_rvec1->tid) == CC_32F ||
               CC_MAT_TYPE(_rvec1->tid) == CC_64F );

    CC_Assert( _rvec1->rows == 3 && _rvec1->cols == 1 && CC_ARE_SIZES_EQ(_rvec1, _rvec2) );

    cvConvert( _rvec1, &r1 );
    cvConvert( _rvec2, &r2 );

    cvRodrigues2( &r1, &R1, &dR1dr1 );
    cvRodrigues2( &r2, &R2, &dR2dr2 );

    if( _rvec3 || dr3dr1 || dr3dr2 )
    {
        double _r3[3], _R3[9], _dR3dR1[9*9], _dR3dR2[9*9], _dr3dR3[9*3];
        double _W1[9*3], _W2[3*3];
        img_t r3 = cvMat1(3,1,CC_64F,_r3), R3 = cvMat1(3,3,CC_64F,_R3);
        img_t dR3dR1 = cvMat1(9,9,CC_64F,_dR3dR1), dR3dR2 = cvMat1(9,9,CC_64F,_dR3dR2);
        img_t dr3dR3 = cvMat1(3,9,CC_64F,_dr3dR3);
        img_t W1 = cvMat1(3,9,CC_64F,_W1), W2 = cvMat1(3,3,CC_64F,_W2);

        cvMatMul( &R2, &R1, &R3 );
        cvCalcMatMulDeriv( &R2, &R1, &dR3dR2, &dR3dR1 );

        cvRodrigues2( &R3, &r3, &dr3dR3 );

        if( _rvec3 )
            cvConvert( &r3, _rvec3 );

        if( dr3dr1 )
        {
            cvMatMul( &dr3dR3, &dR3dR1, &W1 );
            cvMatMul( &W1, &dR1dr1, &W2 );
            cvConvert( &W2, dr3dr1 );
        }

        if( dr3dr2 )
        {
            cvMatMul( &dr3dR3, &dR3dR2, &W1 );
            cvMatMul( &W1, &dR2dr2, &W2 );
            cvConvert( &W2, dr3dr2 );
        }
    }

    if( dr3dt1 )
        cvZero( dr3dt1 );
    if( dr3dt2 )
        cvZero( dr3dt2 );

    if( _tvec3 || dt3dr2 || dt3dt1 )
    {
        double _t1[3], _t2[3], _t3[3], _dxdR2[3*9], _dxdt1[3*3], _W3[3*3];
        img_t t1 = cvMat1(3,1,CC_64F,_t1), t2 = cvMat1(3,1,CC_64F,_t2);
        img_t t3 = cvMat1(3,1,CC_64F,_t3);
        img_t dxdR2 = cvMat1(3, 9, CC_64F, _dxdR2);
        img_t dxdt1 = cvMat1(3, 3, CC_64F, _dxdt1);
        img_t W3 = cvMat1(3, 3, CC_64F, _W3);

        CC_Assert( (_tvec1) && (_tvec2) );
        CC_Assert( CC_ARE_SIZES_EQ(_tvec1, _tvec2) && CC_ARE_SIZES_EQ(_tvec1, _rvec1) );

        cvConvert( _tvec1, &t1 );
        cvConvert( _tvec2, &t2 );
        cvMatMulAdd( &R2, &t1, &t2, &t3 );

        if( _tvec3 )
            cvConvert( &t3, _tvec3 );

        if( dt3dr2 || dt3dt1 )
        {
            cvCalcMatMulDeriv( &R2, &t1, &dxdR2, &dxdt1 );
            if( dt3dr2 )
            {
                cvMatMul( &dxdR2, &dR2dr2, &W3 );
                cvConvert( &W3, dt3dr2 );
            }
            if( dt3dt1 )
                cvConvert( &dxdt1, dt3dt1 );
        }
    }

    if( dt3dt2 )
        cvSetIdentity( dt3dt2 );
    if( dt3dr1 )
        cvZero( dt3dr1 );
}

CC_IMPL int cvRodrigues2( const img_t* src, img_t* dst, img_t* jacobian )
{
    int depth, elem_size;
    int i, k;
    double J[27];
    img_t matJ = cvMat1( 3, 9, CC_64F, J );

    if( !(src) )
        CC_Error( !src ? CC_StsNullPtr : CC_StsBadArg, "Input argument is not a valid matrix" );

    if( !(dst) )
        CC_Error( !dst ? CC_StsNullPtr : CC_StsBadArg,
        "The first output argument is not a valid matrix" );

    depth = CC_MAT_DEPTH(src->tid);
    elem_size = CC_TYPE_SIZE(depth);

    if( depth != CC_32F && depth != CC_64F )
        CC_Error( CC_StsUnsupportedFormat, "The matrices must have 32f or 64f data type" );

    if( !CC_ARE_DEPTHS_EQ(src, dst) )
        CC_Error( CC_StsUnmatchedFormats, "All the matrices must have the same data type" );

    if( jacobian )
    {
        if( !(jacobian) )
            CC_Error( CC_StsBadArg, "Jacobian is not a valid matrix" );

        if( !CC_ARE_DEPTHS_EQ(src, jacobian) || CC_MAT_CN(jacobian->tid) != 1 )
            CC_Error( CC_StsUnmatchedFormats, "Jacobian must have 32fC1 or 64fC1 datatype" );

        if( (jacobian->rows != 9 || jacobian->cols != 3) &&
            (jacobian->rows != 3 || jacobian->cols != 9))
            CC_Error( CC_StsBadSize, "Jacobian must be 3x9 or 9x3" );
    }

    if( src->cols == 1 || src->rows == 1 )
    {
        int step = src->rows > 1 ? src->step / elem_size : 1;

        if( src->rows + src->cols*CC_MAT_CN(src->tid) - 1 != 3 )
            CC_Error( CC_StsBadSize, "Input matrix must be 1x3, 3x1 or 3x3" );

        if( dst->rows != 3 || dst->cols != 3 || CC_MAT_CN(dst->tid) != 1 )
            CC_Error( CC_StsBadSize, "Output matrix must be 3x3, single-channel floating point matrix" );

        CPoint3d r;
        if( depth == CC_32F )
        {
            r.x = src->data.fl[0];
            r.y = src->data.fl[step];
            r.z = src->data.fl[step*2];
        }
        else
        {
            r.x = src->tt.db[0];
            r.y = src->tt.db[step];
            r.z = src->tt.db[step*2];
        }

        double theta = norm(r);

        if( theta < DBL_EPSILON )
        {
            cvSetIdentity( dst );

            if( jacobian )
            {
                memset( J, 0, sizeof(J) );
                J[5] = J[15] = J[19] = -1;
                J[7] = J[11] = J[21] = 1;
            }
        }
        else
        {
            double c = cos(theta);
            double s = sin(theta);
            double c1 = 1. - c;
            double itheta = theta ? 1./theta : 0.;

            r *= itheta;

            Matx33d rrt( r.x*r.x, r.x*r.y, r.x*r.z, r.x*r.y, r.y*r.y, r.y*r.z, r.x*r.z, r.y*r.z, r.z*r.z );
            Matx33d r_x(    0, -r.z,  r.y,
                          r.z,    0, -r.x,
                         -r.y,  r.x,    0 );

            // R = cos(theta)*I + (1 - cos(theta))*r*rT + sin(theta)*[r_x]
            Matx33d R = c*Matx33d::eye() + c1*rrt + s*r_x;

            img_t(R).convertTo(cvarrToMat(dst), dst->tid);

            if( jacobian )
            {
                const double I[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
                double drrt[] = { r.x+r.x, r.y, r.z, r.y, 0, 0, r.z, 0, 0,
                                  0, r.x, 0, r.x, r.y+r.y, r.z, 0, r.z, 0,
                                  0, 0, r.x, 0, 0, r.y, r.x, r.y, r.z+r.z };
                double d_r_x_[] = { 0, 0, 0, 0, 0, -1, 0, 1, 0,
                                    0, 0, 1, 0, 0, 0, -1, 0, 0,
                                    0, -1, 0, 1, 0, 0, 0, 0, 0 };
                for( i = 0; i < 3; i++ )
                {
                    double ri = i == 0 ? r.x : i == 1 ? r.y : r.z;
                    double a0 = -s*ri, a1 = (s - 2*c1*itheta)*ri, a2 = c1*itheta;
                    double a3 = (c - s*itheta)*ri, a4 = s*itheta;
                    for( k = 0; k < 9; k++ )
                        J[i*9+k] = a0*I[k] + a1*rrt.val[k] + a2*drrt[i*9+k] +
                                   a3*r_x.val[k] + a4*d_r_x_[i*9+k];
                }
            }
        }
    }
    else if( src->cols == 3 && src->rows == 3 )
    {
        Matx33d U, Vt;
        Vec3d W;
        double theta, s, c;
        int step = dst->rows > 1 ? dst->step / elem_size : 1;

        if( (dst->rows != 1 || dst->cols*CC_MAT_CN(dst->tid) != 3) &&
            (dst->rows != 3 || dst->cols != 1 || CC_MAT_CN(dst->tid) != 1))
            CC_Error( CC_StsBadSize, "Output matrix must be 1x3 or 3x1" );

        Matx33d R = cvarrToMat(src);

        if( !checkRange(R, true, NULL, -100, 100) )
        {
            cvZero(dst);
            if( jacobian )
                cvZero(jacobian);
            return 0;
        }

        CSVD::compute(R, W, U, Vt);
        R = U*Vt;

        CPoint3d r(R(2, 1) - R(1, 2), R(0, 2) - R(2, 0), R(1, 0) - R(0, 1));

        s = sqrt((r.x*r.x + r.y*r.y + r.z*r.z)*0.25);
        c = (R(0, 0) + R(1, 1) + R(2, 2) - 1)*0.5;
        c = c > 1. ? 1. : c < -1. ? -1. : c;
        theta = acos(c);

        if( s < 1e-5 )
        {
            double t;

            if( c > 0 )
                r = CPoint3d(0, 0, 0);
            else
            {
                t = (R(0, 0) + 1)*0.5;
                r.x = sqrt(MAX(t,0.));
                t = (R(1, 1) + 1)*0.5;
                r.y = sqrt(MAX(t,0.))*(R(0, 1) < 0 ? -1. : 1.);
                t = (R(2, 2) + 1)*0.5;
                r.z = sqrt(MAX(t,0.))*(R(0, 2) < 0 ? -1. : 1.);
                if( fabs(r.x) < fabs(r.y) && fabs(r.x) < fabs(r.z) && (R(1, 2) > 0) != (r.y*r.z > 0) )
                    r.z = -r.z;
                theta /= norm(r);
                r *= theta;
            }

            if( jacobian )
            {
                memset( J, 0, sizeof(J) );
                if( c > 0 )
                {
                    J[5] = J[15] = J[19] = -0.5;
                    J[7] = J[11] = J[21] = 0.5;
                }
            }
        }
        else
        {
            double vth = 1/(2*s);

            if( jacobian )
            {
                double t, dtheta_dtr = -1./s;
                // var1 = [vth;theta]
                // var = [om1;var1] = [om1;vth;theta]
                double dvth_dtheta = -vth*c/s;
                double d1 = 0.5*dvth_dtheta*dtheta_dtr;
                double d2 = 0.5*dtheta_dtr;
                // dvar1/dR = dvar1/dtheta*dtheta/dR = [dvth/dtheta; 1] * dtheta/dtr * dtr/dR
                double dvardR[5*9] =
                {
                    0, 0, 0, 0, 0, 1, 0, -1, 0,
                    0, 0, -1, 0, 0, 0, 1, 0, 0,
                    0, 1, 0, -1, 0, 0, 0, 0, 0,
                    d1, 0, 0, 0, d1, 0, 0, 0, d1,
                    d2, 0, 0, 0, d2, 0, 0, 0, d2
                };
                // var2 = [om;theta]
                double dvar2dvar[] =
                {
                    vth, 0, 0, r.x, 0,
                    0, vth, 0, r.y, 0,
                    0, 0, vth, r.z, 0,
                    0, 0, 0, 0, 1
                };
                double domegadvar2[] =
                {
                    theta, 0, 0, r.x*vth,
                    0, theta, 0, r.y*vth,
                    0, 0, theta, r.z*vth
                };

                img_t _dvardR = cvMat1( 5, 9, CC_64FC1, dvardR );
                img_t _dvar2dvar = cvMat1( 4, 5, CC_64FC1, dvar2dvar );
                img_t _domegadvar2 = cvMat1( 3, 4, CC_64FC1, domegadvar2 );
                double t0[3*5];
                img_t _t0 = cvMat1( 3, 5, CC_64FC1, t0 );

                cvMatMul( &_domegadvar2, &_dvar2dvar, &_t0 );
                cvMatMul( &_t0, &_dvardR, &matJ );

                // transpose every row of matJ (treat the rows as 3x3 matrices)
                CC_SWAP(J[1], J[3], t); CC_SWAP(J[2], J[6], t); CC_SWAP(J[5], J[7], t);
                CC_SWAP(J[10], J[12], t); CC_SWAP(J[11], J[15], t); CC_SWAP(J[14], J[16], t);
                CC_SWAP(J[19], J[21], t); CC_SWAP(J[20], J[24], t); CC_SWAP(J[23], J[25], t);
            }

            vth *= theta;
            r *= vth;
        }

        if( depth == CC_32F )
        {
            dst->data.fl[0] = (float)r.x;
            dst->data.fl[step] = (float)r.y;
            dst->data.fl[step*2] = (float)r.z;
        }
        else
        {
            dst->tt.db[0] = r.x;
            dst->tt.db[step] = r.y;
            dst->tt.db[step*2] = r.z;
        }
    }

    if( jacobian )
    {
        if( depth == CC_32F )
        {
            if( jacobian->rows == matJ->rows )
                cvConvert( &matJ, jacobian );
            else
            {
                float Jf[3*9];
                img_t _Jf = cvMat1( matJ->rows, matJ->cols, CC_32FC1, Jf );
                cvConvert( &matJ, &_Jf );
                cvTranspose( &_Jf, jacobian );
            }
        }
        else if( jacobian->rows == matJ->rows )
            cvCopy( &matJ, jacobian );
        else
            cvTranspose( &matJ, jacobian );
    }

    return 1;
}

#endif


static const char* cvDistCoeffErr = "Distortion coefficients must be 1x4, 4x1, 1x5, 5x1, 1x8, 8x1, 1x12, 12x1, 1x14 or 14x1 floating-point vector";

CC_IMPL void cvProjectPoints2( const img_t* objectPoints,
                  const img_t* r_vec,
                  const img_t* t_vec,
                  const img_t* A,
                  const img_t* distCoeffs,
                  img_t* imagePoints, img_t* dpdr,
                  img_t* dpdt, img_t* dpdf,
                  img_t* dpdc, img_t* dpdk,
                  double aspectRatio )
{
    IM_BEGIN(im, 20);
    img_t *matM = NULL, *_m = NULL;
    img_t *_dpdr = NULL, *_dpdt = NULL, *_dpdc = NULL, *_dpdf = NULL, *_dpdk = NULL;

    int i, j, count;
    int calc_derivatives;
    const CPoint3D64f* M;
    CPoint2D64f* m;
    double r[3], R[9], dRdr[27], t[3], a[9], k[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0}, fx, fy, cx, cy;
    double9 matTilt = {EYE3DEF(1)};
    double9 dMatTiltdTauX = {0,0,0,0,0,0,0,-1,0};
    double9 dMatTiltdTauY = {0,0,0,0,0,0,1,0,0};
    img_t _r, _t, _a = cvMat1( 3, 3, CC_64F, 1, a ), _k;
    img_t matR = cvMat1( 3, 3, CC_64F, 1, R ), _dRdr = cvMat1( 3, 9, CC_64F, 1, dRdr );
    double *dpdr_p = 0, *dpdt_p = 0, *dpdk_p = 0, *dpdf_p = 0, *dpdc_p = 0;
    int dpdr_step = 0, dpdt_step = 0, dpdk_step = 0, dpdf_step = 0, dpdc_step = 0;
    bool fixedAspectRatio = aspectRatio > FLT_EPSILON;

    if( !(objectPoints) || !(r_vec) || !(t_vec) || !(A) ||
        /*!(distCoeffs) ||*/ !(imagePoints) )
        CC_Error( CC_StsBadArg, "One of required arguments is not a valid matrix" );

    int total = objectPoints->rows * objectPoints->cols * CC_MAT_CN(objectPoints);
    if(total % 3 != 0)
    {
        //we have stopped support of homogeneous coordinates because it cause ambiguity in interpretation of the input data
        CC_Error( CC_StsBadArg, "Homogeneous coordinates are not supported" );
    }
    count = total / 3;

    if( CC_IS_CONT_MAT(objectPoints) &&
        (CC_MAT_DEPTH(objectPoints) == CC_32F || CC_MAT_DEPTH(objectPoints) == CC_64F)&&
        ((objectPoints->rows == 1 && CC_MAT_CN(objectPoints) == 3) ||
        (objectPoints->rows == count && CC_MAT_CN(objectPoints)*objectPoints->cols == 3) ||
        (objectPoints->rows == 3 && CC_MAT_CN(objectPoints) == 1 && objectPoints->cols == count)))
    {
        matM = cvSetMat( im++, objectPoints->rows, objectPoints->cols, CC_64F, CC_MAT_CN(objectPoints) );
        cvConvert(objectPoints, matM);
    }
    else
    {
//        matM = cvSetMat( 1, count, CC_64FC3 );
//        cvConvertPointsHomogeneous( objectPoints, matM );
        CC_Error( CC_StsBadArg, "Homogeneous coordinates are not supported" );
    }

    if( CC_IS_CONT_MAT(imagePoints) &&
        (CC_MAT_DEPTH(imagePoints) == CC_32F || CC_MAT_DEPTH(imagePoints) == CC_64F) &&
        ((imagePoints->rows == 1 && CC_MAT_CN(imagePoints) == 2) ||
        (imagePoints->rows == count && CC_MAT_CN(imagePoints)*imagePoints->cols == 2) ||
        (imagePoints->rows == 2 && CC_MAT_CN(imagePoints) == 1 && imagePoints->cols == count)))
    {
        _m = cvSetMat( im++, imagePoints->rows, imagePoints->cols, CC_64F, CC_MAT_CN(imagePoints) );
        cvConvert(imagePoints, _m);
    }
    else
    {
//        _m = cvSetMat( 1, count, CC_64FC2 );
        CC_Error( CC_StsBadArg, "Homogeneous coordinates are not supported" );
    }

    M = (CPoint3D64f*)matM->tt.db;
    m = (CPoint2D64f*)_m->tt.db;

    if( (CC_MAT_DEPTH(r_vec) != CC_64F && CC_MAT_DEPTH(r_vec) != CC_32F) ||
        (((r_vec->rows != 1 && r_vec->cols != 1) ||
        r_vec->rows*r_vec->cols*CC_MAT_CN(r_vec) != 3) &&
        ((r_vec->rows != 3 && r_vec->cols != 3) || CC_MAT_CN(r_vec) != 1)))
        CC_Error( CC_StsBadArg, "Rotation must be represented by 1x3 or 3x1 "
                  "floating-point rotation vector, or 3x3 rotation matrix" );

    if( r_vec->rows == 3 && r_vec->cols == 3 )
    {
        _r = cvMat1( 3, 1, CC_64F, 1, r );
        cvRodrigues2( r_vec, &_r );
        cvRodrigues2( &_r, &matR, &_dRdr );
        cvCopy( r_vec, &matR );
    }
    else
    {
        _r = cvMat1( r_vec->rows, r_vec->cols, CC_64F, CC_MAT_CN(r_vec), r );
        cvConvert( r_vec, &_r );
        cvRodrigues2( &_r, &matR, &_dRdr );
    }

    if( (CC_MAT_DEPTH(t_vec) != CC_64F && CC_MAT_DEPTH(t_vec) != CC_32F) ||
        (t_vec->rows != 1 && t_vec->cols != 1) ||
        t_vec->rows*t_vec->cols*CC_MAT_CN(t_vec) != 3 )
        CC_Error( CC_StsBadArg,
            "Translation vector must be 1x3 or 3x1 floating-point vector" );

    _t = cvMat1( t_vec->rows, t_vec->cols, CC_64F, CC_MAT_CN(t_vec), t );
    cvConvert( t_vec, &_t );

    if( (CC_MAT_TYPE(A) != CC_64F && CC_MAT_TYPE(A) != CC_32F) ||
        A->rows != 3 || A->cols != 3 )
        CC_Error( CC_StsBadArg, "Instrinsic parameters must be 3x3 floating-point matrix" );

    cvConvert( A, &_a );
    fx = a[0]; fy = a[4];
    cx = a[2]; cy = a[5];

    if( fixedAspectRatio )
        fx = fy*aspectRatio;

    if( distCoeffs )
    {
        if( !(distCoeffs) ||
            (CC_MAT_DEPTH(distCoeffs) != CC_64F &&
            CC_MAT_DEPTH(distCoeffs) != CC_32F) ||
            (distCoeffs->rows != 1 && distCoeffs->cols != 1) ||
            (distCoeffs->rows*distCoeffs->cols*CC_MAT_CN(distCoeffs) != 4 &&
            distCoeffs->rows*distCoeffs->cols*CC_MAT_CN(distCoeffs) != 5 &&
            distCoeffs->rows*distCoeffs->cols*CC_MAT_CN(distCoeffs) != 8 &&
            distCoeffs->rows*distCoeffs->cols*CC_MAT_CN(distCoeffs) != 12 &&
            distCoeffs->rows*distCoeffs->cols*CC_MAT_CN(distCoeffs) != 14) )
            CC_Error( CC_StsBadArg, cvDistCoeffErr );

        _k = cvMat1( distCoeffs->rows, distCoeffs->cols, CC_64F, CC_MAT_CN(distCoeffs), k );
        cvConvert( distCoeffs, &_k );
        if(k[12] != 0 || k[13] != 0)
        {
          computeTiltProjectionMatrix(k[12], k[13], matTilt, dMatTiltdTauX, dMatTiltdTauY);
        }
    }

    if( dpdr )
    {
        if( !(dpdr) ||
            (CC_MAT_TYPE(dpdr) != CC_32F && CC_MAT_TYPE(dpdr) != CC_64F) ||
            dpdr->rows != count*2 || dpdr->cols != 3 )
            CC_Error( CC_StsBadArg, "dp/drot must be 2Nx3 floating-point matrix" );

        if( CC_MAT_TYPE(dpdr) == CC_64F )
        {
            _dpdr = cvCloneMat(im++, dpdr);
        }
        else
            _dpdr = cvSetMat( im++, 2*count, 3, CC_64F );
        dpdr_p = _dpdr->tt.db;
        dpdr_step = _dpdr->step/sizeof(dpdr_p[0]);
    }

    if( dpdt )
    {
        if( !(dpdt) ||
            (CC_MAT_TYPE(dpdt) != CC_32FC1 &&
            CC_MAT_TYPE(dpdt) != CC_64FC1) ||
            dpdt->rows != count*2 || dpdt->cols != 3 )
            CC_Error( CC_StsBadArg, "dp/dT must be 2Nx3 floating-point matrix" );

        if( CC_MAT_TYPE(dpdt) == CC_64F )
        {
            _dpdt = cvCloneMat(im++, dpdt);
        }
        else
            _dpdt = cvSetMat1( im++, 2*count, 3, CC_64FC1 );
        dpdt_p = _dpdt->tt.db;
        dpdt_step = _dpdt->step/sizeof(dpdt_p[0]);
    }

    if( dpdf )
    {
        if( !(dpdf) ||
            (CC_MAT_TYPE(dpdf) != CC_32F && CC_MAT_TYPE(dpdf) != CC_64F) ||
            dpdf->rows != count*2 || dpdf->cols != 2 )
            CC_Error( CC_StsBadArg, "dp/df must be 2Nx2 floating-point matrix" );

        if( CC_MAT_TYPE(dpdf) == CC_64F )
        {
            _dpdf = cvCloneMat(im++, dpdf);
        }
        else
            _dpdf = cvSetMat1( im++, 2*count, 2, CC_64FC1 );
        dpdf_p = _dpdf->tt.db;
        dpdf_step = _dpdf->step/sizeof(dpdf_p[0]);
    }

    if( dpdc )
    {
        if( !(dpdc) ||
            (CC_MAT_TYPE(dpdc) != CC_32F && CC_MAT_TYPE(dpdc) != CC_64F) ||
            dpdc->rows != count*2 || dpdc->cols != 2 )
            CC_Error( CC_StsBadArg, "dp/dc must be 2Nx2 floating-point matrix" );

        if( CC_MAT_TYPE(dpdc) == CC_64FC1 )
        {
            _dpdc = cvCloneMat(im++, dpdc);
        }
        else
            _dpdc = cvSetMat( im++, 2*count, 2, CC_64F );
        dpdc_p = _dpdc->tt.db;
        dpdc_step = _dpdc->step/sizeof(dpdc_p[0]);
    }

    if( dpdk )
    {
        if( !(dpdk) ||
            (CC_MAT_TYPE(dpdk) != CC_32F && CC_MAT_TYPE(dpdk) != CC_64F) ||
            dpdk->rows != count*2 || (dpdk->cols != 14 && dpdk->cols != 12 && dpdk->cols != 8 && dpdk->cols != 5 && dpdk->cols != 4 && dpdk->cols != 2) )
            CC_Error( CC_StsBadArg, "dp/df must be 2Nx14, 2Nx12, 2Nx8, 2Nx5, 2Nx4 or 2Nx2 floating-point matrix" );

        if( !distCoeffs )
            CC_Error( CC_StsNullPtr, "distCoeffs is NULL while dpdk is not" );

        if( CC_MAT_TYPE(dpdk) == CC_64F )
        {
            _dpdk = cvCloneMat(im++, dpdk);
        }
        else
            _dpdk = cvSetMat( im++, dpdk->rows, dpdk->cols, CC_64F );
        dpdk_p = _dpdk->tt.db;
        dpdk_step = _dpdk->step/sizeof(dpdk_p[0]);
    }

    calc_derivatives = dpdr || dpdt || dpdf || dpdc || dpdk;

    for( i = 0; i < count; i++ )
    {
        double X = M[i].x, Y = M[i].y, Z = M[i].z;
        double x = R[0]*X + R[1]*Y + R[2]*Z + t[0];
        double y = R[3]*X + R[4]*Y + R[5]*Z + t[1];
        double z = R[6]*X + R[7]*Y + R[8]*Z + t[2];
        double r2, r4, r6, a1, a2, a3, cdist, icdist2;
        double xd, yd, xd0, yd0, invProj;
        double3 vecTilt;
        double3 dVecTilt;
        double22 dMatTilt;
        double2 dXdYd;

        z = z ? 1./z : 1;
        x *= z; y *= z;

        r2 = x*x + y*y;
        r4 = r2*r2;
        r6 = r4*r2;
        a1 = 2*x*y;
        a2 = r2 + 2*x*x;
        a3 = r2 + 2*y*y;
        cdist = 1 + k[0]*r2 + k[1]*r4 + k[4]*r6;
        icdist2 = 1./(1 + k[5]*r2 + k[6]*r4 + k[7]*r6);
        xd0 = x*cdist*icdist2 + k[2]*a1 + k[3]*a2 + k[8]*r2+k[9]*r4;
        yd0 = y*cdist*icdist2 + k[2]*a3 + k[3]*a1 + k[10]*r2+k[11]*r4;

        // additional distortion by projecting onto a tilt plane
        
        //vecTilt = matTilt*Vec3d(xd0, yd0, 1);
        matvec3(vecTilt, matTilt, xd0, yd0, 1);
        invProj = vecTilt[2] ? 1./vecTilt[2] : 1;
        xd = invProj * vecTilt[0];
        yd = invProj * vecTilt[1];

        m[i].x = xd*fx + cx;
        m[i].y = yd*fy + cy;

        if( calc_derivatives )
        {
            if( dpdc_p )
            {
                dpdc_p[0] = 1; dpdc_p[1] = 0; // dp_xdc_x; dp_xdc_y
                dpdc_p[dpdc_step] = 0;
                dpdc_p[dpdc_step+1] = 1;
                dpdc_p += dpdc_step*2;
            }

            if( dpdf_p )
            {
                if( fixedAspectRatio )
                {
                    dpdf_p[0] = 0; dpdf_p[1] = xd*aspectRatio; // dp_xdf_x; dp_xdf_y
                    dpdf_p[dpdf_step] = 0;
                    dpdf_p[dpdf_step+1] = yd;
                }
                else
                {
                    dpdf_p[0] = xd; dpdf_p[1] = 0;
                    dpdf_p[dpdf_step] = 0;
                    dpdf_p[dpdf_step+1] = yd;
                }
                dpdf_p += dpdf_step*2;
            }
            for (int row = 0; row < 2; ++row)
                for (int col = 0; col < 2; ++col)
                    dMatTilt[row][col] = matTilt[row*3+col]*vecTilt[2]
                      - matTilt[2*3+col]*vecTilt[row];
            double invProjSquare = (invProj*invProj);
            //dMatTilt *= invProjSquare;
            scale4(dMatTilt[0], dMatTilt[0], invProjSquare);
            if( dpdk_p )
            {
                matvec2(dXdYd, *dMatTilt, x*icdist2*r2, y*icdist2*r2);
                dpdk_p[0] = fx*dXdYd[0];
                dpdk_p[dpdk_step] = fy*dXdYd[1];
                matvec2(dXdYd, *dMatTilt, x*icdist2*r4, y*icdist2*r4);
                dpdk_p[1] = fx*dXdYd[0];
                dpdk_p[dpdk_step+1] = fy*dXdYd[1];
                if( _dpdk->cols > 2 )
                {
                    matvec2(dXdYd, *dMatTilt, a1, a3);
                    dpdk_p[2] = fx*dXdYd[0];
                    dpdk_p[dpdk_step+2] = fy*dXdYd[1];
                    matvec2(dXdYd, *dMatTilt, a2, a1);
                    dpdk_p[3] = fx*dXdYd[0];
                    dpdk_p[dpdk_step+3] = fy*dXdYd[1];
                    if( _dpdk->cols > 4 )
                    {
                        matvec2(dXdYd, *dMatTilt, x*icdist2*r6, y*icdist2*r6);
                        dpdk_p[4] = fx*dXdYd[0];
                        dpdk_p[dpdk_step+4] = fy*dXdYd[1];

                        if( _dpdk->cols > 5 )
                        {
                            matvec2(dXdYd, *dMatTilt, x*cdist*(-icdist2)*icdist2*r2, y*cdist*(-icdist2)*icdist2*r2);
                            dpdk_p[5] = fx*dXdYd[0];
                            dpdk_p[dpdk_step+5] = fy*dXdYd[1];
                            //dXdYd = dMatTilt*vec2;
                            matvec2(dXdYd, *dMatTilt, x*cdist*(-icdist2)*icdist2*r4, y*cdist*(-icdist2)*icdist2*r4);
                            dpdk_p[6] = fx*dXdYd[0];
                            dpdk_p[dpdk_step+6] = fy*dXdYd[1];
                            //dXdYd = dMatTilt*vec2;
                            matvec2(dXdYd, *dMatTilt, x*cdist*(-icdist2)*icdist2*r6, y*cdist*(-icdist2)*icdist2*r6);
                            dpdk_p[7] = fx*dXdYd[0];
                            dpdk_p[dpdk_step+7] = fy*dXdYd[1];
                            if( _dpdk->cols > 8 )
                            {
                                //dXdYd = dMatTilt*Vec2d(r2, 0);
                                matvec2(dXdYd, *dMatTilt, r2, 0);
                                dpdk_p[8] = fx*dXdYd[0]; //s1
                                dpdk_p[dpdk_step+8] = fy*dXdYd[1]; //s1
                                //dXdYd = dMatTilt*Vec2d(r4, 0);
                                matvec2(dXdYd, *dMatTilt, r4, 0);
                                dpdk_p[9] = fx*dXdYd[0]; //s2
                                dpdk_p[dpdk_step+9] = fy*dXdYd[1]; //s2
                                //dXdYd = dMatTilt*Vec2d(0, r2);
                                matvec2(dXdYd, *dMatTilt, 0, r2);
                                dpdk_p[10] = fx*dXdYd[0];//s3
                                dpdk_p[dpdk_step+10] = fy*dXdYd[1]; //s3
                                //dXdYd = dMatTilt*Vec2d(0, r4);
                                matvec2(dXdYd, *dMatTilt, 0, r4);
                                dpdk_p[11] = fx*dXdYd[0];//s4
                                dpdk_p[dpdk_step+11] = fy*dXdYd[1]; //s4
                                if( _dpdk->cols > 12 )
                                {
                                    //dVecTilt = dMatTiltdTauX * Vec3d(xd0, yd0, 1);
                                    matvec3(dVecTilt, dMatTiltdTauX, xd0, yd0, 1);
                                    dpdk_p[12] = fx * invProjSquare * ( dVecTilt[0] * vecTilt[2] - dVecTilt[2] * vecTilt[0]);
                                    dpdk_p[dpdk_step+12] = fy*invProjSquare * ( dVecTilt[1] * vecTilt[2] - dVecTilt[2] * vecTilt[1]);
                                    //dVecTilt = dMatTiltdTauY * Vec3d(xd0, yd0, 1);
                                    matvec3(dVecTilt, dMatTiltdTauY, xd0, yd0, 1);
                                    dpdk_p[13] = fx * invProjSquare * ( dVecTilt[0] * vecTilt[2] - dVecTilt[2] * vecTilt[0]);
                                    dpdk_p[dpdk_step+13] = fy * invProjSquare * ( dVecTilt[1] * vecTilt[2] - dVecTilt[2] * vecTilt[1]);
                                }
                            }
                        }
                    }
                }
                dpdk_p += dpdk_step*2;
            }

            if( dpdt_p )
            {
                double dxdt[] = { z, 0, -x*z }, dydt[] = { 0, z, -y*z };
                for( j = 0; j < 3; j++ )
                {
                    double dr2dt = 2*x*dxdt[j] + 2*y*dydt[j];
                    double dcdist_dt = k[0]*dr2dt + 2*k[1]*r2*dr2dt + 3*k[4]*r4*dr2dt;
                    double dicdist2_dt = -icdist2*icdist2*(k[5]*dr2dt + 2*k[6]*r2*dr2dt + 3*k[7]*r4*dr2dt);
                    double da1dt = 2*(x*dydt[j] + y*dxdt[j]);
                    double dmxdt = (dxdt[j]*cdist*icdist2 + x*dcdist_dt*icdist2 + x*cdist*dicdist2_dt +
                                       k[2]*da1dt + k[3]*(dr2dt + 4*x*dxdt[j]) + k[8]*dr2dt + 2*r2*k[9]*dr2dt);
                    double dmydt = (dydt[j]*cdist*icdist2 + y*dcdist_dt*icdist2 + y*cdist*dicdist2_dt +
                                       k[2]*(dr2dt + 4*y*dydt[j]) + k[3]*da1dt + k[10]*dr2dt + 2*r2*k[11]*dr2dt);
                    //dXdYd = dMatTilt*Vec2d(dmxdt, dmydt);
                    matvec2(dXdYd, *dMatTilt, dmxdt, dmydt);
                    dpdt_p[j] = fx*dXdYd[0];
                    dpdt_p[dpdt_step+j] = fy*dXdYd[1];
                }
                dpdt_p += dpdt_step*2;
            }

            if( dpdr_p )
            {
                double dx0dr[] =
                {
                    X*dRdr[0] + Y*dRdr[1] + Z*dRdr[2],
                    X*dRdr[9] + Y*dRdr[10] + Z*dRdr[11],
                    X*dRdr[18] + Y*dRdr[19] + Z*dRdr[20]
                };
                double dy0dr[] =
                {
                    X*dRdr[3] + Y*dRdr[4] + Z*dRdr[5],
                    X*dRdr[12] + Y*dRdr[13] + Z*dRdr[14],
                    X*dRdr[21] + Y*dRdr[22] + Z*dRdr[23]
                };
                double dz0dr[] =
                {
                    X*dRdr[6] + Y*dRdr[7] + Z*dRdr[8],
                    X*dRdr[15] + Y*dRdr[16] + Z*dRdr[17],
                    X*dRdr[24] + Y*dRdr[25] + Z*dRdr[26]
                };
                for( j = 0; j < 3; j++ )
                {
                    double dxdr = z*(dx0dr[j] - x*dz0dr[j]);
                    double dydr = z*(dy0dr[j] - y*dz0dr[j]);
                    double dr2dr = 2*x*dxdr + 2*y*dydr;
                    double dcdist_dr = (k[0] + 2*k[1]*r2 + 3*k[4]*r4)*dr2dr;
                    double dicdist2_dr = -icdist2*icdist2*(k[5] + 2*k[6]*r2 + 3*k[7]*r4)*dr2dr;
                    double da1dr = 2*(x*dydr + y*dxdr);
                    double dmxdr = (dxdr*cdist*icdist2 + x*dcdist_dr*icdist2 + x*cdist*dicdist2_dr +
                                       k[2]*da1dr + k[3]*(dr2dr + 4*x*dxdr) + (k[8] + 2*r2*k[9])*dr2dr);
                    double dmydr = (dydr*cdist*icdist2 + y*dcdist_dr*icdist2 + y*cdist*dicdist2_dr +
                                       k[2]*(dr2dr + 4*y*dydr) + k[3]*da1dr + (k[10] + 2*r2*k[11])*dr2dr);
                    //dXdYd = dMatTilt*Vec2d(dmxdr, dmydr);
                    matvec2(dXdYd, *dMatTilt, dmxdr, dmydr);
                    dpdr_p[j] = fx*dXdYd[0];
                    dpdr_p[dpdr_step+j] = fy*dXdYd[1];
                }
                dpdr_p += dpdr_step*2;
            }
        }
    }

    if( _m != imagePoints )
        cvConvert( _m, imagePoints );

    if( _dpdr != dpdr )
        cvConvert( _dpdr, dpdr );

    if( _dpdt != dpdt )
        cvConvert( _dpdt, dpdt );

    if( _dpdf != dpdf )
        cvConvert( _dpdf, dpdf );

    if( _dpdc != dpdc )
        cvConvert( _dpdc, dpdc );

    if( _dpdk != dpdk )
        cvConvert( _dpdk, dpdk );
    
    IM_END(im);
}

CC_IMPL void cvFindExtrinsicCameraParams2( const img_t* objectPoints,
                  const img_t* imagePoints, const img_t* A,
                  const img_t* distCoeffs, img_t* rvec, img_t* tvec,
                  int useExtrinsicGuess )
{
    const int max_iter = 20;
    IM_BEGIN(im, 20);
    img_t *matM=NULL, *_Mxy=NULL, *_m=NULL, *_mn=NULL, *matL=NULL;

    int i, count;
    double a[9], ar[9]={1,0,0,0,1,0,0,0,1}, R[9];
    double MM[9], U[9], V[9], W[3];
    CScalar Mc;
    double param[6];
    img_t matA = cvMat1( 3, 3, CC_64F, 1, a );
    img_t _Ar = cvMat1( 3, 3, CC_64F, 1, ar );
    img_t matR = cvMat1( 3, 3, CC_64F, 1, R );
    img_t _r = cvMat1( 3, 1, CC_64F, 1, param );
    img_t _t = cvMat1( 3, 1, CC_64F, 1, param + 3 );
    img_t _Mc = cvMat1( 1, 3, CC_64F, 1, Mc.val );
    img_t _MM = cvMat1( 3, 3, CC_64F, 1, MM );
    img_t matU = cvMat1( 3, 3, CC_64F, 1, U );
    img_t matV = cvMat1( 3, 3, CC_64F, 1, V );
    img_t matW = cvMat1( 3, 1, CC_64F, 1, W );
    img_t _param = cvMat1( 6, 1, CC_64F, 1, param );
    img_t _dpdr, _dpdt;

    CC_Assert( (objectPoints) && (imagePoints) && (A) && (rvec) && (tvec) );

    count = MAX(objectPoints->cols, objectPoints->rows);
    matM = cvSetMat( im++, 1, count, CC_64F, 3 );
    _m = cvSetMat( im++, 1, count, CC_64F, 2 );
    cvConvertPointsHomogeneous( objectPoints, matM );
    cvConvertPointsHomogeneous( imagePoints, _m );
    cvConvert( A, &matA );

    CC_Assert( ((rvec->tid) == CC_64F || rvec->tid == CC_32F) &&
        (rvec->rows == 1 || rvec->cols == 1) && rvec->rows*rvec->cols*CC_MAT_CN(rvec) == 3 );

    CC_Assert( ((tvec->tid) == CC_64F || (tvec->tid) == CC_32F) &&
        (tvec->rows == 1 || tvec->cols == 1) && tvec->rows*tvec->cols*CC_MAT_CN(tvec) == 3 );

    _mn = cvSetMat1( im++, 1, count, CC_64FC2 );
    _Mxy = cvSetMat1( im++, 1, count, CC_64FC2 );

    // normalize image points
    // (unapply the intrinsic matrix transformation and distortion)
    cvUndistortPoints( _m, _mn, &matA, distCoeffs, 0, &_Ar );

    if( useExtrinsicGuess )
    {
        img_t _r_temp = cvMat1(rvec->rows, rvec->cols, CC_64F, CC_MAT_CN(rvec), param );
        img_t _t_temp = cvMat1(tvec->rows, tvec->cols, CC_64F, CC_MAT_CN(tvec), param + 3);
        cvConvert( rvec, &_r_temp );
        cvConvert( tvec, &_t_temp );
    }
    else
    {
        cvAvg(matM, Mc.val);
        cvReshape( matM, matM, 1, count );
        cvMulTransposed( matM, &_MM, 1, &_Mc );
        cvSVD( &_MM, &matW, 0, &matV, CC_SVD_MODIFY_A + CC_SVD_V_T );

        // initialize extrinsic parameters
        if( W[2]/W[1] < 1e-3 || count < 4 )
        {
            // a planar structure case (all M's lie in the same plane)
            double tt[3], h[9], h1_norm, h2_norm;
            img_t* R_transform = &matV;
            img_t T_transform = cvMat1( 3, 1, CC_64F, 1, tt );
            img_t matH = cvMat1( 3, 3, CC_64F, 1, h );
            img_t _h1, _h2, _h3;

            if( V[2]*V[2] + V[5]*V[5] < 1e-10 )
                cvSetIdentity( R_transform );

            if( cvDet(R_transform) < 0 )
                cvScale( R_transform, R_transform, -1 );

            cvGEMM( R_transform, &_Mc, -1, 0, 0, &T_transform, CC_GEMM_B_T );

            for( i = 0; i < count; i++ )
            {
                const double* Rp = R_transform->tt.db;
                const double* Tp = T_transform.tt.db;
                const double* src = matM->tt.db + i*3;
                double* dst = _Mxy->tt.db + i*2;

                dst[0] = Rp[0]*src[0] + Rp[1]*src[1] + Rp[2]*src[2] + Tp[0];
                dst[1] = Rp[3]*src[0] + Rp[4]*src[1] + Rp[5]*src[2] + Tp[1];
            }

            cvFindHomography( _Mxy, _mn, &matH );

            if( cvCheckArr(&matH, CC_CHECK_QUIET) )
            {
                cvGetCol( &matH, &_h1, 0 );
                _h2 = _h1; _h2.tt.db++;
                _h3 = _h2; _h3.tt.db++;
                h1_norm = sqrt(h[0]*h[0] + h[3]*h[3] + h[6]*h[6]);
                h2_norm = sqrt(h[1]*h[1] + h[4]*h[4] + h[7]*h[7]);

                cvScale( &_h1, &_h1, 1./MAX(h1_norm, DBL_EPSILON) );
                cvScale( &_h2, &_h2, 1./MAX(h2_norm, DBL_EPSILON) );
                cvScale( &_h3, &_t, 2./MAX(h1_norm + h2_norm, DBL_EPSILON));
                cvCrossProduct( &_h1, &_h2, &_h3 );

                cvRodrigues2( &matH, &_r );
                cvRodrigues2( &_r, &matH );
                cvMatMulAdd( &matH, &T_transform, &_t, &_t );
                cvMatMul( &matH, R_transform, &matR );
            }
            else
            {
                cvSetIdentity( &matR );
                cvZeroMat( &_t );
            }

            cvRodrigues2( &matR, &_r );
        }
        else
        {
            // non-planar structure. Use DLT method
            double* L;
            double LL[12*12], LW[12], LV[12*12], sc;
            img_t _LL = cvMat1( 12, 12, CC_64F, 1, LL );
            img_t _LW = cvMat1( 12, 1, CC_64F, 1, LW );
            img_t _LV = cvMat1( 12, 12, CC_64F, 1, LV );
            img_t _RRt, _RR, _tt;
            CPoint3D64f* M = (CPoint3D64f*)matM->tt.db;
            CPoint2D64f* mn = (CPoint2D64f*)_mn->tt.db;

            cvSetMat( matL, 2*count, 12, CC_64F );
            L = matL->tt.db;

            for( i = 0; i < count; i++, L += 24 )
            {
                double x = -mn[i].x, y = -mn[i].y;
                L[0] = L[16] = M[i].x;
                L[1] = L[17] = M[i].y;
                L[2] = L[18] = M[i].z;
                L[3] = L[19] = 1.;
                L[4] = L[5] = L[6] = L[7] = 0.;
                L[12] = L[13] = L[14] = L[15] = 0.;
                L[8] = x*M[i].x;
                L[9] = x*M[i].y;
                L[10] = x*M[i].z;
                L[11] = x;
                L[20] = y*M[i].x;
                L[21] = y*M[i].y;
                L[22] = y*M[i].z;
                L[23] = y;
            }

            cvMulTransposed( matL, &_LL, 1 );
            cvSVD( &_LL, &_LW, 0, &_LV, CC_SVD_MODIFY_A + CC_SVD_V_T );
            _RRt = cvMat1( 3, 4, CC_64F, 1, LV + 11*12 );
            cvGetCols( &_RRt, &_RR, 0, 3 );
            cvGetCol( &_RRt, &_tt, 3 );
            if( cvDet(&_RR) < 0 )
                cvScale( &_RRt, &_RRt, -1 );
            sc = cvNorm(&_RR);
            cvSVD( &_RR, &matW, &matU, &matV, CC_SVD_MODIFY_A + CC_SVD_U_T + CC_SVD_V_T );
            cvGEMM( &matU, &matV, 1, 0, 0, &matR, CC_GEMM_A_T );
            cvScale( &_tt, &_t, cvNorm(&matR)/sc );
            cvRodrigues2( &matR, &_r );
        }
    }

    cvReshape( matM, matM, 3, 1 );
    cvReshape( _mn, _mn, 2, 1 );

    // refine extrinsic parameters using iterative algorithm
    CLevMarq solver( 6, count*2, cTermCriteria(CC_TERMCRIT_EPS+CC_TERMCRIT_ITER,max_iter,FLT_EPSILON), true);
    cvCopy( &_param, solver.param );

    for(;;)
    {
        img_t *matJ = 0, *_err = 0;
		const img_t *__param = 0;
        bool proceed = solver.update( __param, matJ, _err );
        cvCopy( __param, &_param );
        if( !proceed || !_err )
            break;
        cvReshape( _err, _err, 2, 1 );
        if( matJ )
        {
            cvGetCols( matJ, &_dpdr, 0, 3 );
            cvGetCols( matJ, &_dpdt, 3, 6 );
            cvProjectPoints2( matM, &_r, &_t, &matA, distCoeffs,
                              _err, &_dpdr, &_dpdt, 0, 0, 0 );
        }
        else
        {
            cvProjectPoints2( matM, &_r, &_t, &matA, distCoeffs,
                              _err, 0, 0, 0, 0, 0 );
        }
        cvSub(_err, _m, _err);
        cvReshape( _err, _err, 1, 2*count );
    }
    cvCopy( solver.param, &_param );

    _r = cvMat1( rvec->rows, rvec->cols, CC_64F, CC_MAT_CN(rvec), param );
    _t = cvMat1( tvec->rows, tvec->cols, CC_64F, CC_MAT_CN(tvec), param + 3 );

    cvConvert( &_r, rvec );
    cvConvert( &_t, tvec );
    IM_END(im);
}

#if 0
CC_IMPL void cvInitIntrinsicParams2D( const img_t* objectPoints,
                         const img_t* imagePoints, const img_t* npoints,
                         CSize imageSize, img_t* cameraMatrix,
                         double aspectRatio )
{
    Ptr<img_t> matA, _b, _allH;

    int i, j, pos, nimages, ni = 0;
    double a[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };
    double H[9], f[2];
    img_t _a = cvMat1( 3, 3, CC_64F, a );
    img_t matH = cvMat1( 3, 3, CC_64F, H );
    img_t _f = cvMat1( 2, 1, CC_64F, f );

    assert( CC_MAT_TYPE(npoints->tid) == CC_32SC1 &&
            _CONT(npoints->tid) );
    nimages = npoints->rows + npoints->cols - 1;

    if( (CC_MAT_TYPE(objectPoints->tid) != CC_32FC3 &&
        CC_MAT_TYPE(objectPoints->tid) != CC_64FC3) ||
        (CC_MAT_TYPE(imagePoints->tid) != CC_32FC2 &&
        CC_MAT_TYPE(imagePoints->tid) != CC_64FC2) )
        CC_Error( CC_StsUnsupportedFormat, "Both object points and image points must be 2D" );

    if( objectPoints->rows != 1 || imagePoints->rows != 1 )
        CC_Error( CC_StsBadSize, "object points and image points must be a single-row matrices" );

    matA.reset(cvSetMat( 2*nimages, 2, CC_64F ));
    _b.reset(cvSetMat( 2*nimages, 1, CC_64F ));
    a[2] = (!imageSize.width) ? 0.5 : (imageSize.width)*0.5;
    a[5] = (!imageSize.height) ? 0.5 : (imageSize.height)*0.5;
    _allH.reset(cvSetMat( nimages, 9, CC_64F ));

    // extract vanishing points in order to obtain initial value for the focal length
    for( i = 0, pos = 0; i < nimages; i++, pos += ni )
    {
        double* Ap = matA->tt.db + i*4;
        double* bp = _b->tt.db + i*2;
        ni = npoints->data.i[i];
        double h[3], v[3], d1[3], d2[3];
        double n[4] = {0,0,0,0};
        img_t _m, matM;
        cvGetCols( objectPoints, &matM, pos, pos + ni );
        cvGetCols( imagePoints, &_m, pos, pos + ni );

        cvFindHomography( &matM, &_m, &matH );
        memcpy( _allH->tt.db + i*9, H, sizeof(H) );

        H[0] -= H[6]*a[2]; H[1] -= H[7]*a[2]; H[2] -= H[8]*a[2];
        H[3] -= H[6]*a[5]; H[4] -= H[7]*a[5]; H[5] -= H[8]*a[5];

        for( j = 0; j < 3; j++ )
        {
            double t0 = H[j*3], t1 = H[j*3+1];
            h[j] = t0; v[j] = t1;
            d1[j] = (t0 + t1)*0.5;
            d2[j] = (t0 - t1)*0.5;
            n[0] += t0*t0; n[1] += t1*t1;
            n[2] += d1[j]*d1[j]; n[3] += d2[j]*d2[j];
        }

        for( j = 0; j < 4; j++ )
            n[j] = 1./sqrt(n[j]);

        for( j = 0; j < 3; j++ )
        {
            h[j] *= n[0]; v[j] *= n[1];
            d1[j] *= n[2]; d2[j] *= n[3];
        }

        Ap[0] = h[0]*v[0]; Ap[1] = h[1]*v[1];
        Ap[2] = d1[0]*d2[0]; Ap[3] = d1[1]*d2[1];
        bp[0] = -h[2]*v[2]; bp[1] = -d1[2]*d2[2];
    }

    cvSolve( matA, _b, &_f, CC_NORMAL + CC_SVD );
    a[0] = sqrt(fabs(1./f[0]));
    a[4] = sqrt(fabs(1./f[1]));
    if( aspectRatio != 0 )
    {
        double tf = (a[0] + a[4])/(aspectRatio + 1.);
        a[0] = aspectRatio*tf;
        a[4] = tf;
    }

    cvConvert( &_a, cameraMatrix );
}

static void subMatrix(const img_t& src, img_t& dst, const std::vector<uchar>& cols,
                      const std::vector<uchar>& rows) {
    int nonzeros_cols = countNonZero(cols);
    img_t tmp(src->rows, nonzeros_cols, CC_64FC1);

    for (int i = 0, j = 0; i < (int)cols.size(); i++)
    {
        if (cols[i])
        {
            src.col(i).copyTo(tmp.col(j++));
        }
    }

    int nonzeros_rows  = countNonZero(rows);
    dst cvSetMat(nonzeros_rows, nonzeros_cols, CC_64FC1);
    for (int i = 0, j = 0; i < (int)rows.size(); i++)
    {
        if (rows[i])
        {
            tmp.row(i).copyTo(dst.row(j++));
        }
    }
}

static double cvCalibrateCamera2Internal( const img_t* objectPoints,
                    const img_t* imagePoints, const img_t* npoints,
                    CSize imageSize, img_t* cameraMatrix, img_t* distCoeffs,
                    img_t* rvecs, img_t* tvecs, img_t* stdDevs,
                    img_t* perViewErrors, int flags, CTermCriteria termCrit )
{
    const int NINTRINSIC = CC_CALIB_NINTRINSIC;
    double reprojErr = 0;

    Matx33d A;
    double k[14] = {0};
    img_t matA = cvMat1(3, 3, CC_64F, A.val), _k;
    int i, nimages, maxPoints = 0, ni = 0, pos, total = 0, nparams, npstep, cn;
    double aspectRatio = 0.;

    // 0. check the parameters & allocate buffers
    if( !(objectPoints) || !(imagePoints) ||
        !(npoints) || !(cameraMatrix) || !(distCoeffs) )
        CC_Error( CC_StsBadArg, "One of required vector arguments is not a valid matrix" );

    if( imageSize.width <= 0 || imageSize.height <= 0 )
        CC_Error( CC_StsOutOfRange, "image width and height must be positive" );

    if( CC_MAT_TYPE(npoints->tid) != CC_32SC1 ||
        (npoints->rows != 1 && npoints->cols != 1) )
        CC_Error( CC_StsUnsupportedFormat,
            "the array of point counters must be 1-dimensional integer vector" );
    if(flags & CC_CALIB_TILTED_MODEL)
    {
        //when the tilted sensor model is used the distortion coefficients matrix must have 14 parameters
        if (distCoeffs->cols*distCoeffs->rows != 14)
            CC_Error( CC_StsBadArg, "The tilted sensor model must have 14 parameters in the distortion matrix" );
    }
    else
    {
        //when the thin prism model is used the distortion coefficients matrix must have 12 parameters
        if(flags & CC_CALIB_THIN_PRISM_MODEL)
            if (distCoeffs->cols*distCoeffs->rows != 12)
                CC_Error( CC_StsBadArg, "Thin prism model must have 12 parameters in the distortion matrix" );
    }

    nimages = npoints->rows*npoints->cols;
    npstep = npoints->rows == 1 ? 1 : npoints->step/CC_TYPE_SIZE(npoints->tid);

    if( rvecs )
    {
        cn = CC_MAT_CN(rvecs->tid);
        if( !(rvecs) ||
            (CC_MAT_DEPTH(rvecs->tid) != CC_32F && CC_MAT_DEPTH(rvecs->tid) != CC_64F) ||
            ((rvecs->rows != nimages || (rvecs->cols*cn != 3 && rvecs->cols*cn != 9)) &&
            (rvecs->rows != 1 || rvecs->cols != nimages || cn != 3)) )
            CC_Error( CC_StsBadArg, "the output array of rotation vectors must be 3-channel "
                "1xn or nx1 array or 1-channel nx3 or nx9 array, where n is the number of views" );
    }

    if( tvecs )
    {
        cn = CC_MAT_CN(tvecs->tid);
        if( !(tvecs) ||
            (CC_MAT_DEPTH(tvecs->tid) != CC_32F && CC_MAT_DEPTH(tvecs->tid) != CC_64F) ||
            ((tvecs->rows != nimages || tvecs->cols*cn != 3) &&
            (tvecs->rows != 1 || tvecs->cols != nimages || cn != 3)) )
            CC_Error( CC_StsBadArg, "the output array of translation vectors must be 3-channel "
                "1xn or nx1 array or 1-channel nx3 array, where n is the number of views" );
    }

    if( stdDevs )
    {
        cn = CC_MAT_CN(stdDevs->tid);
        if( !(stdDevs) ||
            (CC_MAT_DEPTH(stdDevs->tid) != CC_32F && CC_MAT_DEPTH(stdDevs->tid) != CC_64F) ||
            ((stdDevs->rows != (nimages*6 + NINTRINSIC) || stdDevs->cols*cn != 1) &&
            (stdDevs->rows != 1 || stdDevs->cols != (nimages*6 + NINTRINSIC) || cn != 1)) )
#define STR__(x) #x
#define STR_(x) STR__(x)
            CC_Error( CC_StsBadArg, "the output array of standard deviations vectors must be 1-channel "
                "1x(n*6 + NINTRINSIC) or (n*6 + NINTRINSIC)x1 array, where n is the number of views,"
                " NINTRINSIC = " STR_(CC_CALIB_NINTRINSIC));
    }

    if( (CC_MAT_TYPE(cameraMatrix->tid) != CC_32FC1 &&
        CC_MAT_TYPE(cameraMatrix->tid) != CC_64FC1) ||
        cameraMatrix->rows != 3 || cameraMatrix->cols != 3 )
        CC_Error( CC_StsBadArg,
            "Intrinsic parameters must be 3x3 floating-point matrix" );

    if( (CC_MAT_TYPE(distCoeffs->tid) != CC_32FC1 &&
        CC_MAT_TYPE(distCoeffs->tid) != CC_64FC1) ||
        (distCoeffs->cols != 1 && distCoeffs->rows != 1) ||
        (distCoeffs->cols*distCoeffs->rows != 4 &&
        distCoeffs->cols*distCoeffs->rows != 5 &&
        distCoeffs->cols*distCoeffs->rows != 8 &&
        distCoeffs->cols*distCoeffs->rows != 12 &&
        distCoeffs->cols*distCoeffs->rows != 14) )
        CC_Error( CC_StsBadArg, cvDistCoeffErr );

    for( i = 0; i < nimages; i++ )
    {
        ni = npoints->data.i[i*npstep];
        if( ni < 4 )
        {
            CC_Error_( CC_StsOutOfRange, ("The number of points in the view #%d is < 4", i));
        }
        maxPoints = MAX( maxPoints, ni );
        total += ni;
    }

    img_t matM( 1, total, CC_64FC3 );
    img_t _m( 1, total, CC_64FC2 );
    img_t allErrors(1, total, CC_64FC2);

    if(CC_MAT_CN(objectPoints->tid) == 3) {
        cvarrToMat(objectPoints).convertTo(matM, CC_64F);
    } else {
        convertPointsHomogeneous(cvarrToMat(objectPoints), matM);
    }

    if(CC_MAT_CN(imagePoints->tid) == 2) {
        cvarrToMat(imagePoints).convertTo(_m, CC_64F);
    } else {
        convertPointsHomogeneous(cvarrToMat(imagePoints), _m);
    }

    nparams = NINTRINSIC + nimages*6;
    img_t _Ji( maxPoints*2, NINTRINSIC, CC_64FC1, Scalar(0));
    img_t _Je( maxPoints*2, 6, CC_64FC1 );
    img_t _err( maxPoints*2, 1, CC_64FC1 );

    _k = cvMat1( distCoeffs->rows, distCoeffs->cols, CC_MAKETYPECN(CC_64F,CC_MAT_CN(distCoeffs->tid)), k);
    if( distCoeffs->rows*distCoeffs->cols*CC_MAT_CN(distCoeffs->tid) < 8 )
    {
        if( distCoeffs->rows*distCoeffs->cols*CC_MAT_CN(distCoeffs->tid) < 5 )
            flags |= CALIB_FIX_K3;
        flags |= CALIB_FIX_K4 | CALIB_FIX_K5 | CALIB_FIX_K6;
    }
    const double minValidAspectRatio = 0.01;
    const double maxValidAspectRatio = 100.0;

    // 1. initialize intrinsic parameters & LM solver
    if( flags & CALIB_USE_INTRINSIC_GUESS )
    {
        cvConvert( cameraMatrix, &matA );
        if( A(0, 0) <= 0 || A(1, 1) <= 0 )
            CC_Error( CC_StsOutOfRange, "Focal length (fx and fy) must be positive" );
        if( A(0, 2) < 0 || A(0, 2) >= imageSize.width ||
            A(1, 2) < 0 || A(1, 2) >= imageSize.height )
            CC_Error( CC_StsOutOfRange, "Principal point must be within the image" );
        if( fabs(A(0, 1)) > 1e-5 )
            CC_Error( CC_StsOutOfRange, "Non-zero skew is not supported by the function" );
        if( fabs(A(1, 0)) > 1e-5 || fabs(A(2, 0)) > 1e-5 ||
            fabs(A(2, 1)) > 1e-5 || fabs(A(2,2)-1) > 1e-5 )
            CC_Error( CC_StsOutOfRange,
                "The intrinsic matrix must have [fx 0 cx; 0 fy cy; 0 0 1] shape" );
        A(0, 1) = A(1, 0) = A(2, 0) = A(2, 1) = 0.;
        A(2, 2) = 1.;

        if( flags & CALIB_FIX_ASPECT_RATIO )
        {
            aspectRatio = A(0, 0)/A(1, 1);

            if( aspectRatio < minValidAspectRatio || aspectRatio > maxValidAspectRatio )
                CC_Error( CC_StsOutOfRange,
                    "The specified aspect ratio (= cameraMatrix[0][0] / cameraMatrix[1][1]) is incorrect" );
        }
        cvConvert( distCoeffs, &_k );
    }
    else
    {
        Scalar mean, sdv;
        meanStdDev(matM, mean, sdv);
        if( fabs(mean[2]) > 1e-5 || fabs(sdv[2]) > 1e-5 )
            CC_Error( CC_StsBadArg,
            "For non-planar calibration rigs the initial intrinsic matrix must be specified" );
        for( i = 0; i < total; i++ )
            matM.at<CPoint3d>(i).z = 0.;

        if( flags & CALIB_FIX_ASPECT_RATIO )
        {
            aspectRatio = cvmGet(cameraMatrix,0,0);
            aspectRatio /= cvmGet(cameraMatrix,1,1);
            if( aspectRatio < minValidAspectRatio || aspectRatio > maxValidAspectRatio )
                CC_Error( CC_StsOutOfRange,
                    "The specified aspect ratio (= cameraMatrix[0][0] / cameraMatrix[1][1]) is incorrect" );
        }
        img_t _matM(matM), m(_m);
        cvInitIntrinsicParams2D( &_matM, &m, npoints, imageSize, &matA, aspectRatio );
    }

    CvLevMarq solver( nparams, 0, termCrit );

    if(flags & CALIB_USE_LU) {
        solver.solveMethod = DECOMP_LU;
    }
    else if(flags & CALIB_USE_QR) {
        solver.solveMethod = DECOMP_QR;
    }

    {
    double* param = solver.param->tt.db;
    uchar* mask = solver.mask->tt.data;

    param[0] = A(0, 0); param[1] = A(1, 1); param[2] = A(0, 2); param[3] = A(1, 2);
    std::copy(k, k + 14, param + 4);

    if(flags & CALIB_FIX_ASPECT_RATIO)
        mask[0] = 0;
    if( flags & CC_CALIB_FIX_FOCAL_LENGTH )
        mask[0] = mask[1] = 0;
    if( flags & CC_CALIB_FIX_PRINCIPAL_POINT )
        mask[2] = mask[3] = 0;
    if( flags & CC_CALIB_ZERO_TANGENT_DIST )
    {
        param[6] = param[7] = 0;
        mask[6] = mask[7] = 0;
    }
    if( !(flags & CALIB_RATIONAL_MODEL) )
        flags |= CALIB_FIX_K4 + CALIB_FIX_K5 + CALIB_FIX_K6;
    if( !(flags & CC_CALIB_THIN_PRISM_MODEL))
        flags |= CALIB_FIX_S1_S2_S3_S4;
    if( !(flags & CC_CALIB_TILTED_MODEL))
        flags |= CALIB_FIX_TAUX_TAUY;

    mask[ 4] = !(flags & CALIB_FIX_K1);
    mask[ 5] = !(flags & CALIB_FIX_K2);
    mask[ 8] = !(flags & CALIB_FIX_K3);
    mask[ 9] = !(flags & CALIB_FIX_K4);
    mask[10] = !(flags & CALIB_FIX_K5);
    mask[11] = !(flags & CALIB_FIX_K6);

    if(flags & CALIB_FIX_S1_S2_S3_S4)
    {
        mask[12] = 0;
        mask[13] = 0;
        mask[14] = 0;
        mask[15] = 0;
    }
    if(flags & CALIB_FIX_TAUX_TAUY)
    {
        mask[16] = 0;
        mask[17] = 0;
    }
    }

    // 2. initialize extrinsic parameters
    for( i = 0, pos = 0; i < nimages; i++, pos += ni )
    {
        img_t _ri, _ti;
        ni = npoints->data.i[i*npstep];

        cvGetRows( solver.param, &_ri, NINTRINSIC + i*6, NINTRINSIC + i*6 + 3 );
        cvGetRows( solver.param, &_ti, NINTRINSIC + i*6 + 3, NINTRINSIC + i*6 + 6 );

        img_t _Mi(matM.colRange(pos, pos + ni));
        img_t _mi(_m.colRange(pos, pos + ni));

        cvFindExtrinsicCameraParams2( &_Mi, &_mi, &matA, &_k, &_ri, &_ti );
    }

    // 3. run the optimization
    for(;;)
    {
        const img_t* _param = 0;
        img_t *_JtJ = 0, *_JtErr = 0;
        double* _errNorm = 0;
        bool proceed = solver.updateAlt( _param, _JtJ, _JtErr, _errNorm );
        double *param = solver.param->tt.db, *pparam = solver.prevParam->tt.db;
        bool calcJ = solver.state == CvLevMarq::CALC_J || (!proceed && stdDevs);

        if( flags & CALIB_FIX_ASPECT_RATIO )
        {
            param[0] = param[1]*aspectRatio;
            pparam[0] = pparam[1]*aspectRatio;
        }

        A(0, 0) = param[0]; A(1, 1) = param[1]; A(0, 2) = param[2]; A(1, 2) = param[3];
        std::copy(param + 4, param + 4 + 14, k);

        if ( !proceed && !stdDevs && !perViewErrors )
            break;
        else if ( !proceed && stdDevs )
            cvZero(_JtJ);

        reprojErr = 0;

        for( i = 0, pos = 0; i < nimages; i++, pos += ni )
        {
            img_t _ri, _ti;
            ni = npoints->data.i[i*npstep];

            cvGetRows( solver.param, &_ri, NINTRINSIC + i*6, NINTRINSIC + i*6 + 3 );
            cvGetRows( solver.param, &_ti, NINTRINSIC + i*6 + 3, NINTRINSIC + i*6 + 6 );

            img_t _Mi(matM.colRange(pos, pos + ni));
            img_t _mi(_m.colRange(pos, pos + ni));
            img_t _me(allErrors.colRange(pos, pos + ni));

            _Je.resize(ni*2); _Ji.resize(ni*2); _err.resize(ni*2);
            img_t _dpdr(_Je.colRange(0, 3));
            img_t _dpdt(_Je.colRange(3, 6));
            img_t _dpdf(_Ji.colRange(0, 2));
            img_t _dpdc(_Ji.colRange(2, 4));
            img_t _dpdk(_Ji.colRange(4, NINTRINSIC));
            img_t _mp(_err cvReshape(,2, 1));

            if( calcJ )
            {
                 cvProjectPoints2( &_Mi, &_ri, &_ti, &matA, &_k, &_mp, &_dpdr, &_dpdt,
                                  (flags & CALIB_FIX_FOCAL_LENGTH) ? 0 : &_dpdf,
                                  (flags & CALIB_FIX_PRINCIPAL_POINT) ? 0 : &_dpdc, &_dpdk,
                                  (flags & CALIB_FIX_ASPECT_RATIO) ? aspectRatio : 0);
            }
            else
                cvProjectPoints2( &_Mi, &_ri, &_ti, &matA, &_k, &_mp );

            cvSub( &_mp, &_mi, &_mp );
            if (perViewErrors || stdDevs)
                cvCopy(&_mp, &_me);

            if( calcJ )
            {
                img_t JtJ(cvarrToMat(_JtJ)), JtErr(cvarrToMat(_JtErr));

                // see HZ: (A6.14) for details on the structure of the Jacobian
                JtJ(Rect(0, 0, NINTRINSIC, NINTRINSIC)) += _Ji.t() * _Ji;
                JtJ(Rect(NINTRINSIC + i * 6, NINTRINSIC + i * 6, 6, 6)) = _Je.t() * _Je;
                JtJ(Rect(NINTRINSIC + i * 6, 0, 6, NINTRINSIC)) = _Ji.t() * _Je;

                JtErr cvGetRows(,0, NINTRINSIC) += _Ji.t() * _err;
                JtErr cvGetRows(,NINTRINSIC + i * 6, NINTRINSIC + (i + 1) * 6) = _Je.t() * _err;
            }

            reprojErr += norm(_err, NORM_L2SQR);
        }
        if( _errNorm )
            *_errNorm = reprojErr;

        if( !proceed )
        {
            if( stdDevs )
            {
                img_t mask = cvarrToMat(solver.mask);
                int nparams_nz = countNonZero(mask);
                img_t JtJinv, JtJN;
                JtJN cvSetMat(nparams_nz, nparams_nz, CC_64F);
                subMatrix(cvarrToMat(_JtJ), JtJN, mask, mask);
                completeSymm(JtJN, false);
                invert(JtJN, JtJinv, DECOMP_SVD);
                //sigma2 is deviation of the noise
                //see any papers about variance of the least squares estimator for
                //detailed description of the variance estimation methods
                double sigma2 = norm(allErrors, NORM_L2SQR) / (total - nparams_nz);
                img_t stdDevsM = cvarrToMat(stdDevs);
                int j = 0;
                for ( int s = 0; s < nparams; s++ )
                    if( mask.data[s] )
                    {
                        stdDevsM.at<double>(s) = sqrt(JtJinv.at<double>(j,j) * sigma2);
                        j++;
                    }
                    else
                        stdDevsM.at<double>(s) = 0.;
            }
            break;
        }
    }

    // 4. store the results
    cvConvert( &matA, cameraMatrix );
    cvConvert( &_k, distCoeffs );

    for( i = 0, pos = 0; i < nimages; i++ )
    {
        img_t src, dst;
        if( perViewErrors )
        {
            ni = npoints->data.i[i*npstep];
            perViewErrors->tt.db[i] = sqrt(norm(allErrors.colRange(pos, pos + ni),
                                                           NORM_L2SQR) / ni);
            pos+=ni;
        }

        if( rvecs )
        {
            src = cvMat1( 3, 1, CC_64F, solver.param->tt.db + NINTRINSIC + i*6 );
            if( rvecs->rows == nimages && rvecs->cols*CC_MAT_CN(rvecs->tid) == 9 )
            {
                dst = cvMat1( 3, 3, CC_MAT_DEPTH(rvecs->tid),
                    rvecs->tt.data + rvecs->step*i );
                cvRodrigues2( &src, &matA );
                cvConvert( &matA, &dst );
            }
            else
            {
                dst = cvMat1( 3, 1, CC_MAT_DEPTH(rvecs->tid), rvecs->rows == 1 ?
                    rvecs->tt.data + i*CC_TYPE_SIZE(rvecs->tid) :
                    rvecs->tt.data + rvecs->step*i );
                cvConvert( &src, &dst );
            }
        }
        if( tvecs )
        {
            src = cvMat1( 3, 1, CC_64F, solver.param->tt.db + NINTRINSIC + i*6 + 3 );
            dst = cvMat1( 3, 1, CC_MAT_DEPTH(tvecs->tid), tvecs->rows == 1 ?
                    tvecs->tt.data + i*CC_TYPE_SIZE(tvecs->tid) :
                    tvecs->tt.data + tvecs->step*i );
            cvConvert( &src, &dst );
         }
    }

    return sqrt(reprojErr/total);
}


/* finds intrinsic and extrinsic camera parameters
   from a few views of known calibration pattern */
CC_IMPL double cvCalibrateCamera2( const img_t* objectPoints,
                    const img_t* imagePoints, const img_t* npoints,
                    CSize imageSize, img_t* cameraMatrix, img_t* distCoeffs,
                    img_t* rvecs, img_t* tvecs, int flags, CTermCriteria termCrit )
{
    return cvCalibrateCamera2Internal(objectPoints, imagePoints, npoints, imageSize, cameraMatrix,
                                      distCoeffs, rvecs, tvecs, NULL, NULL, flags, termCrit);
}

void cvCalibrationMatrixValues( const img_t *calibMatr, CSize imgSize,
    double apertureWidth, double apertureHeight, double *fovx, double *fovy,
    double *focalLength, CPoint2D64f *principalPoint, double *pasp )
{
    /* Validate parameters. */
    if(calibMatr == 0)
        CC_Error(CC_StsNullPtr, "Some of parameters is a NULL pointer!");

    if(!(calibMatr))
        CC_Error(CC_StsUnsupportedFormat, "Input parameters must be a matrices!");

    double dummy;
    CPoint2d pp;
    calibrationMatrixValues(cvarrToMat(calibMatr), imgSize, apertureWidth, apertureHeight,
            fovx ? *fovx : dummy,
            fovy ? *fovy : dummy,
            focalLength ? *focalLength : dummy,
            pp,
            pasp ? *pasp : dummy);

    if(principalPoint)
        *principalPoint = cvPoint2D64f(pp.x, pp.y);
}


//////////////////////////////// Stereo Calibration ///////////////////////////////////

static int dbCmp( const void* _a, const void* _b )
{
    double a = *(const double*)_a;
    double b = *(const double*)_b;

    return (a > b) - (a < b);
}


double cvStereoCalibrate( const img_t* _objectPoints, const img_t* _imagePoints1,
                        const img_t* _imagePoints2, const img_t* _npoints,
                        img_t* _cameraMatrix1, img_t* _distCoeffs1,
                        img_t* _cameraMatrix2, img_t* _distCoeffs2,
                        CSize imageSize, img_t* matR, img_t* matT,
                        img_t* matE, img_t* matF,
                        int flags,
                        CTermCriteria termCrit )
{
    const int NINTRINSIC = 18;
    Ptr<img_t> npoints, err, J_LR, Je, Ji, imagePoints[2], objectPoints, RT0;
    double reprojErr = 0;

    double A[2][9], dk[2][14]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0}}, rlr[9];
    img_t K[2], Dist[2], om_LR, T_LR;
    img_t R_LR = cvMat1(3, 3, CC_64F, rlr);
    int i, k, p, ni = 0, ofs, nimages, pointsTotal, maxPoints = 0;
    int nparams;
    bool recomputeIntrinsics = false;
    double aspectRatio[2] = {0,0};

    CC_Assert( (_imagePoints1) && (_imagePoints2) &&
               (_objectPoints) && (_npoints) &&
               (matR) && (matT) );

    CC_Assert( CC_ARE_TYPES_EQ(_imagePoints1, _imagePoints2) &&
               CC_ARE_DEPTHS_EQ(_imagePoints1, _objectPoints) );

    CC_Assert( (_npoints->cols == 1 || _npoints->rows == 1) &&
               CC_MAT_TYPE(_npoints->tid) == CC_32SC1 );

    nimages = _npoints->cols + _npoints->rows - 1;
    npoints.reset(cvSetMat( _npoints->rows, _npoints->cols, _npoints->tid ));
    cvCopy( _npoints, npoints );

    for( i = 0, pointsTotal = 0; i < nimages; i++ )
    {
        maxPoints = MAX(maxPoints, npoints->data.i[i]);
        pointsTotal += npoints->data.i[i];
    }

    objectPoints.reset(cvSetMat( _objectPoints->rows, _objectPoints->cols,
                                    CC_64FC(CC_MAT_CN(_objectPoints->tid))));
    cvConvert( _objectPoints, objectPoints );
    cvReshape( objectPoints, objectPoints, 3, 1 );

    for( k = 0; k < 2; k++ )
    {
        const img_t* points = k == 0 ? _imagePoints1 : _imagePoints2;
        const img_t* cameraMatrix = k == 0 ? _cameraMatrix1 : _cameraMatrix2;
        const img_t* distCoeffs = k == 0 ? _distCoeffs1 : _distCoeffs2;

        int cn = CC_MAT_CN(_imagePoints1->tid);
        CC_Assert( (CC_MAT_DEPTH(_imagePoints1->tid) == CC_32F ||
                CC_MAT_DEPTH(_imagePoints1->tid) == CC_64F) &&
               ((_imagePoints1->rows == pointsTotal && _imagePoints1->cols*cn == 2) ||
                (_imagePoints1->rows == 1 && _imagePoints1->cols == pointsTotal && cn == 2)) );

        K[k] = cvMat1(3,3,CC_64F,A[k]);
        Dist[k] = cvMat1(1,14,CC_64F,dk[k]);

        imagePoints[k].reset(cvSetMat( points->rows, points->cols, CC_64FC(CC_MAT_CN(points->tid))));
        cvConvert( points, imagePoints[k] );
        cvReshape( imagePoints[k], imagePoints[k], 2, 1 );

        if( flags & (CC_CALIB_FIX_INTRINSIC|CC_CALIB_USE_INTRINSIC_GUESS|
            CC_CALIB_FIX_ASPECT_RATIO|CC_CALIB_FIX_FOCAL_LENGTH) )
            cvConvert( cameraMatrix, &K[k] );

        if( flags & (CC_CALIB_FIX_INTRINSIC|CC_CALIB_USE_INTRINSIC_GUESS|
            CC_CALIB_FIX_K1|CC_CALIB_FIX_K2|CC_CALIB_FIX_K3|CC_CALIB_FIX_K4|CC_CALIB_FIX_K5|CC_CALIB_FIX_K6) )
        {
            img_t tdist = cvMat1( distCoeffs->rows, distCoeffs->cols,
                CC_MAKETYPECN(CC_64F,CC_MAT_CN(distCoeffs->tid)), Dist[k]->tt.db );
            cvConvert( distCoeffs, &tdist );
        }

        if( !(flags & (CC_CALIB_FIX_INTRINSIC|CC_CALIB_USE_INTRINSIC_GUESS)))
        {
            cvCalibrateCamera2( objectPoints, imagePoints[k],
                npoints, imageSize, &K[k], &Dist[k], NULL, NULL, flags );
        }
    }

    if( flags & CC_CALIB_SAME_FOCAL_LENGTH )
    {
        static const int avg_idx[] = { 0, 4, 2, 5, -1 };
        for( k = 0; avg_idx[k] >= 0; k++ )
            A[0][avg_idx[k]] = A[1][avg_idx[k]] = (A[0][avg_idx[k]] + A[1][avg_idx[k]])*0.5;
    }

    if( flags & CC_CALIB_FIX_ASPECT_RATIO )
    {
        for( k = 0; k < 2; k++ )
            aspectRatio[k] = A[k][0]/A[k][4];
    }

    recomputeIntrinsics = (flags & CC_CALIB_FIX_INTRINSIC) == 0;

    err.reset(cvSetMat( maxPoints*2, 1, CC_64F ));
    Je.reset(cvSetMat( maxPoints*2, 6, CC_64F ));
    J_LR.reset(cvSetMat( maxPoints*2, 6, CC_64F ));
    Ji.reset(cvSetMat( maxPoints*2, NINTRINSIC, CC_64F ));
    cvZero( Ji );

    // we optimize for the inter-camera R(3),t(3), then, optionally,
    // for intrinisic parameters of each camera ((fx,fy,cx,cy,k1,k2,p1,p2) ~ 8 parameters).
    nparams = 6*(nimages+1) + (recomputeIntrinsics ? NINTRINSIC*2 : 0);

    // storage for initial [om(R){i}|t{i}] (in order to compute the median for each component)
    RT0.reset(cvSetMat( 6, nimages, CC_64F ));

    CvLevMarq solver( nparams, 0, termCrit );

    if(flags & CALIB_USE_LU) {
        solver.solveMethod = DECOMP_LU;
    }

    if( recomputeIntrinsics )
    {
        uchar* imask = solver.mask->tt.data + nparams - NINTRINSIC*2;
        if( !(flags & CC_CALIB_RATIONAL_MODEL) )
            flags |= CC_CALIB_FIX_K4 | CC_CALIB_FIX_K5 | CC_CALIB_FIX_K6;
        if( !(flags & CC_CALIB_THIN_PRISM_MODEL) )
            flags |= CC_CALIB_FIX_S1_S2_S3_S4;
        if( !(flags & CC_CALIB_TILTED_MODEL) )
            flags |= CC_CALIB_FIX_TAUX_TAUY;
        if( flags & CC_CALIB_FIX_ASPECT_RATIO )
            imask[0] = imask[NINTRINSIC] = 0;
        if( flags & CC_CALIB_FIX_FOCAL_LENGTH )
            imask[0] = imask[1] = imask[NINTRINSIC] = imask[NINTRINSIC+1] = 0;
        if( flags & CC_CALIB_FIX_PRINCIPAL_POINT )
            imask[2] = imask[3] = imask[NINTRINSIC+2] = imask[NINTRINSIC+3] = 0;
        if( flags & CC_CALIB_ZERO_TANGENT_DIST )
            imask[6] = imask[7] = imask[NINTRINSIC+6] = imask[NINTRINSIC+7] = 0;
        if( flags & CC_CALIB_FIX_K1 )
            imask[4] = imask[NINTRINSIC+4] = 0;
        if( flags & CC_CALIB_FIX_K2 )
            imask[5] = imask[NINTRINSIC+5] = 0;
        if( flags & CC_CALIB_FIX_K3 )
            imask[8] = imask[NINTRINSIC+8] = 0;
        if( flags & CC_CALIB_FIX_K4 )
            imask[9] = imask[NINTRINSIC+9] = 0;
        if( flags & CC_CALIB_FIX_K5 )
            imask[10] = imask[NINTRINSIC+10] = 0;
        if( flags & CC_CALIB_FIX_K6 )
            imask[11] = imask[NINTRINSIC+11] = 0;
        if( flags & CC_CALIB_FIX_S1_S2_S3_S4 )
        {
            imask[12] = imask[NINTRINSIC+12] = 0;
            imask[13] = imask[NINTRINSIC+13] = 0;
            imask[14] = imask[NINTRINSIC+14] = 0;
            imask[15] = imask[NINTRINSIC+15] = 0;
        }
        if( flags & CC_CALIB_FIX_TAUX_TAUY )
        {
            imask[16] = imask[NINTRINSIC+16] = 0;
            imask[17] = imask[NINTRINSIC+17] = 0;
        }
    }

    /*
       Compute initial estimate of pose
       For each image, compute:
          R(om) is the rotation matrix of om
          om(R) is the rotation vector of R
          R_ref = R(om_right) * R(om_left)'
          T_ref_list = [T_ref_list; T_right - R_ref * T_left]
          om_ref_list = {om_ref_list; om(R_ref)]
       om = median(om_ref_list)
       T = median(T_ref_list)
    */
    for( i = ofs = 0; i < nimages; ofs += ni, i++ )
    {
        ni = npoints->data.i[i];
        img_t objpt_i;
        double _om[2][3], r[2][9], t[2][3];
        img_t om[2], R[2], T[2], imgpt_i[2];

        objpt_i = cvMat1(1, ni, CC_64FC3, objectPoints->tt.db + ofs*3);
        for( k = 0; k < 2; k++ )
        {
            imgpt_i[k] = cvMat1(1, ni, CC_64FC2, imagePoints[k]->tt.db + ofs*2);
            om[k] = cvMat1(3, 1, CC_64F, _om[k]);
            R[k] = cvMat1(3, 3, CC_64F, r[k]);
            T[k] = cvMat1(3, 1, CC_64F, t[k]);

            // FIXME: here we ignore activePoints[k] because of
            // the limited API of cvFindExtrnisicCameraParams2
            cvFindExtrinsicCameraParams2( &objpt_i, &imgpt_i[k], &K[k], &Dist[k], &om[k], &T[k] );
            cvRodrigues2( &om[k], &R[k] );
            if( k == 0 )
            {
                // save initial om_left and T_left
                solver.param->tt.db[(i+1)*6] = _om[0][0];
                solver.param->tt.db[(i+1)*6 + 1] = _om[0][1];
                solver.param->tt.db[(i+1)*6 + 2] = _om[0][2];
                solver.param->tt.db[(i+1)*6 + 3] = t[0][0];
                solver.param->tt.db[(i+1)*6 + 4] = t[0][1];
                solver.param->tt.db[(i+1)*6 + 5] = t[0][2];
            }
        }
        cvGEMM( &R[1], &R[0], 1, 0, 0, &R[0], CC_GEMM_B_T );
        cvGEMM( &R[0], &T[0], -1, &T[1], 1, &T[1] );
        cvRodrigues2( &R[0], &T[0] );
        RT0->tt.db[i] = t[0][0];
        RT0->tt.db[i + nimages] = t[0][1];
        RT0->tt.db[i + nimages*2] = t[0][2];
        RT0->tt.db[i + nimages*3] = t[1][0];
        RT0->tt.db[i + nimages*4] = t[1][1];
        RT0->tt.db[i + nimages*5] = t[1][2];
    }

    // find the medians and save the first 6 parameters
    for( i = 0; i < 6; i++ )
    {
        qsort( RT0->tt.db + i*nimages, nimages, CC_TYPE_SIZE(RT0->tid), dbCmp );
        solver.param->tt.db[i] = nimages % 2 != 0 ? RT0->tt.db[i*nimages + nimages/2] :
            (RT0->tt.db[i*nimages + nimages/2 - 1] + RT0->tt.db[i*nimages + nimages/2])*0.5;
    }

    if( recomputeIntrinsics )
        for( k = 0; k < 2; k++ )
        {
            double* iparam = solver.param->tt.db + (nimages+1)*6 + k*NINTRINSIC;
            if( flags & CC_CALIB_ZERO_TANGENT_DIST )
                dk[k][2] = dk[k][3] = 0;
            iparam[0] = A[k][0]; iparam[1] = A[k][4]; iparam[2] = A[k][2]; iparam[3] = A[k][5];
            iparam[4] = dk[k][0]; iparam[5] = dk[k][1]; iparam[6] = dk[k][2];
            iparam[7] = dk[k][3]; iparam[8] = dk[k][4]; iparam[9] = dk[k][5];
            iparam[10] = dk[k][6]; iparam[11] = dk[k][7];
            iparam[12] = dk[k][8];
            iparam[13] = dk[k][9];
            iparam[14] = dk[k][10];
            iparam[15] = dk[k][11];
            iparam[16] = dk[k][12];
            iparam[17] = dk[k][13];
        }

    om_LR = cvMat1(3, 1, CC_64F, solver.param->tt.db);
    T_LR = cvMat1(3, 1, CC_64F, solver.param->tt.db + 3);

    for(;;)
    {
        const img_t* param = 0;
        img_t tmpimagePoints;
        img_t *JtJ = 0, *JtErr = 0;
        double *_errNorm = 0;
        double _omR[3], _tR[3];
        double _dr3dr1[9], _dr3dr2[9], /*_dt3dr1[9],*/ _dt3dr2[9], _dt3dt1[9], _dt3dt2[9];
        img_t dr3dr1 = cvMat1(3, 3, CC_64F, _dr3dr1);
        img_t dr3dr2 = cvMat1(3, 3, CC_64F, _dr3dr2);
        //img_t dt3dr1 = cvMat1(3, 3, CC_64F, _dt3dr1);
        img_t dt3dr2 = cvMat1(3, 3, CC_64F, _dt3dr2);
        img_t dt3dt1 = cvMat1(3, 3, CC_64F, _dt3dt1);
        img_t dt3dt2 = cvMat1(3, 3, CC_64F, _dt3dt2);
        img_t om[2], T[2], imgpt_i[2];
        img_t dpdrot_hdr, dpdt_hdr, dpdf_hdr, dpdc_hdr, dpdk_hdr;
        img_t *dpdrot = &dpdrot_hdr, *dpdt = &dpdt_hdr, *dpdf = 0, *dpdc = 0, *dpdk = 0;

        if( !solver.updateAlt( param, JtJ, JtErr, _errNorm ))
            break;
        reprojErr = 0;

        cvRodrigues2( &om_LR, &R_LR );
        om[1] = cvMat1(3,1,CC_64F,_omR);
        T[1] = cvMat1(3,1,CC_64F,_tR);

        if( recomputeIntrinsics )
        {
            double* iparam = solver.param->tt.db + (nimages+1)*6;
            double* ipparam = solver.prevParam->tt.db + (nimages+1)*6;
            dpdf = &dpdf_hdr;
            dpdc = &dpdc_hdr;
            dpdk = &dpdk_hdr;
            if( flags & CC_CALIB_SAME_FOCAL_LENGTH )
            {
                iparam[NINTRINSIC] = iparam[0];
                iparam[NINTRINSIC+1] = iparam[1];
                ipparam[NINTRINSIC] = ipparam[0];
                ipparam[NINTRINSIC+1] = ipparam[1];
            }
            if( flags & CC_CALIB_FIX_ASPECT_RATIO )
            {
                iparam[0] = iparam[1]*aspectRatio[0];
                iparam[NINTRINSIC] = iparam[NINTRINSIC+1]*aspectRatio[1];
                ipparam[0] = ipparam[1]*aspectRatio[0];
                ipparam[NINTRINSIC] = ipparam[NINTRINSIC+1]*aspectRatio[1];
            }
            for( k = 0; k < 2; k++ )
            {
                A[k][0] = iparam[k*NINTRINSIC+0];
                A[k][4] = iparam[k*NINTRINSIC+1];
                A[k][2] = iparam[k*NINTRINSIC+2];
                A[k][5] = iparam[k*NINTRINSIC+3];
                dk[k][0] = iparam[k*NINTRINSIC+4];
                dk[k][1] = iparam[k*NINTRINSIC+5];
                dk[k][2] = iparam[k*NINTRINSIC+6];
                dk[k][3] = iparam[k*NINTRINSIC+7];
                dk[k][4] = iparam[k*NINTRINSIC+8];
                dk[k][5] = iparam[k*NINTRINSIC+9];
                dk[k][6] = iparam[k*NINTRINSIC+10];
                dk[k][7] = iparam[k*NINTRINSIC+11];
                dk[k][8] = iparam[k*NINTRINSIC+12];
                dk[k][9] = iparam[k*NINTRINSIC+13];
                dk[k][10] = iparam[k*NINTRINSIC+14];
                dk[k][11] = iparam[k*NINTRINSIC+15];
                dk[k][12] = iparam[k*NINTRINSIC+16];
                dk[k][13] = iparam[k*NINTRINSIC+17];
            }
        }

        for( i = ofs = 0; i < nimages; ofs += ni, i++ )
        {
            ni = npoints->data.i[i];
            img_t objpt_i, _part;

            om[0] = cvMat1(3,1,CC_64F,solver.param->tt.db+(i+1)*6);
            T[0] = cvMat1(3,1,CC_64F,solver.param->tt.db+(i+1)*6+3);

            if( JtJ || JtErr )
                cvComposeRT( &om[0], &T[0], &om_LR, &T_LR, &om[1], &T[1], &dr3dr1, 0,
                             &dr3dr2, 0, 0, &dt3dt1, &dt3dr2, &dt3dt2 );
            else
                cvComposeRT( &om[0], &T[0], &om_LR, &T_LR, &om[1], &T[1] );

            objpt_i = cvMat1(1, ni, CC_64FC3, objectPoints->tt.db + ofs*3);
            err->rows = Je->rows = J_LR->rows = Ji->rows = ni*2;
            cvReshape( err, &tmpimagePoints, 2, 1 );

            cvGetCols( Ji, &dpdf_hdr, 0, 2 );
            cvGetCols( Ji, &dpdc_hdr, 2, 4 );
            cvGetCols( Ji, &dpdk_hdr, 4, NINTRINSIC );
            cvGetCols( Je, &dpdrot_hdr, 0, 3 );
            cvGetCols( Je, &dpdt_hdr, 3, 6 );

            for( k = 0; k < 2; k++ )
            {
                double l2err;
                imgpt_i[k] = cvMat1(1, ni, CC_64FC2, imagePoints[k]->tt.db + ofs*2);

                if( JtJ || JtErr )
                    cvProjectPoints2( &objpt_i, &om[k], &T[k], &K[k], &Dist[k],
                            &tmpimagePoints, dpdrot, dpdt, dpdf, dpdc, dpdk,
                            (flags & CC_CALIB_FIX_ASPECT_RATIO) ? aspectRatio[k] : 0);
                else
                    cvProjectPoints2( &objpt_i, &om[k], &T[k], &K[k], &Dist[k], &tmpimagePoints );
                cvSub( &tmpimagePoints, &imgpt_i[k], &tmpimagePoints );

                l2err = cvNorm( &tmpimagePoints, 0, CC_L2 );

                if( JtJ || JtErr )
                {
                    int iofs = (nimages+1)*6 + k*NINTRINSIC, eofs = (i+1)*6;
                    assert( JtJ && JtErr );

                    if( k == 1 )
                    {
                        // d(err_{x|y}R) ~ de3
                        // convert de3/{dr3,dt3} => de3{dr1,dt1} & de3{dr2,dt2}
                        for( p = 0; p < ni*2; p++ )
                        {
                            img_t de3dr3 = cvMat1( 1, 3, CC_64F, Je->tt.data + Je->step*p );
                            img_t de3dt3 = cvMat1( 1, 3, CC_64F, de3dr3->tt.db + 3 );
                            img_t de3dr2 = cvMat1( 1, 3, CC_64F, J_LR->tt.data + J_LR->step*p );
                            img_t de3dt2 = cvMat1( 1, 3, CC_64F, de3dr2->tt.db + 3 );
                            double _de3dr1[3], _de3dt1[3];
                            img_t de3dr1 = cvMat1( 1, 3, CC_64F, _de3dr1 );
                            img_t de3dt1 = cvMat1( 1, 3, CC_64F, _de3dt1 );

                            cvMatMul( &de3dr3, &dr3dr1, &de3dr1 );
                            cvMatMul( &de3dt3, &dt3dt1, &de3dt1 );

                            cvMatMul( &de3dr3, &dr3dr2, &de3dr2 );
                            cvMatMulAdd( &de3dt3, &dt3dr2, &de3dr2, &de3dr2 );

                            cvMatMul( &de3dt3, &dt3dt2, &de3dt2 );

                            cvCopy( &de3dr1, &de3dr3 );
                            cvCopy( &de3dt1, &de3dt3 );
                        }

                        cvGetSubRect( JtJ, &_part, cRect(0, 0, 6, 6) );
                        cvGEMM( J_LR, J_LR, 1, &_part, 1, &_part, CC_GEMM_A_T );

                        cvGetSubRect( JtJ, &_part, cRect(eofs, 0, 6, 6) );
                        cvGEMM( J_LR, Je, 1, 0, 0, &_part, CC_GEMM_A_T );

                        cvGetRows( JtErr, &_part, 0, 6 );
                        cvGEMM( J_LR, err, 1, &_part, 1, &_part, CC_GEMM_A_T );
                    }

                    cvGetSubRect( JtJ, &_part, cRect(eofs, eofs, 6, 6) );
                    cvGEMM( Je, Je, 1, &_part, 1, &_part, CC_GEMM_A_T );

                    cvGetRows( JtErr, &_part, eofs, eofs + 6 );
                    cvGEMM( Je, err, 1, &_part, 1, &_part, CC_GEMM_A_T );

                    if( recomputeIntrinsics )
                    {
                        cvGetSubRect( JtJ, &_part, cRect(iofs, iofs, NINTRINSIC, NINTRINSIC) );
                        cvGEMM( Ji, Ji, 1, &_part, 1, &_part, CC_GEMM_A_T );
                        cvGetSubRect( JtJ, &_part, cRect(iofs, eofs, NINTRINSIC, 6) );
                        cvGEMM( Je, Ji, 1, &_part, 1, &_part, CC_GEMM_A_T );
                        if( k == 1 )
                        {
                            cvGetSubRect( JtJ, &_part, cRect(iofs, 0, NINTRINSIC, 6) );
                            cvGEMM( J_LR, Ji, 1, &_part, 1, &_part, CC_GEMM_A_T );
                        }
                        cvGetRows( JtErr, &_part, iofs, iofs + NINTRINSIC );
                        cvGEMM( Ji, err, 1, &_part, 1, &_part, CC_GEMM_A_T );
                    }
                }

                reprojErr += l2err*l2err;
            }
        }
        if(_errNorm)
            *_errNorm = reprojErr;
    }

    cvRodrigues2( &om_LR, &R_LR );
    if( matR->rows == 1 || matR->cols == 1 )
        cvConvert( &om_LR, matR );
    else
        cvConvert( &R_LR, matR );
    cvConvert( &T_LR, matT );

    if( recomputeIntrinsics )
    {
        cvConvert( &K[0], _cameraMatrix1 );
        cvConvert( &K[1], _cameraMatrix2 );

        for( k = 0; k < 2; k++ )
        {
            img_t* distCoeffs = k == 0 ? _distCoeffs1 : _distCoeffs2;
            img_t tdist = cvMat1( distCoeffs->rows, distCoeffs->cols,
                CC_MAKETYPECN(CC_64F,CC_MAT_CN(distCoeffs->tid)), Dist[k]->tt.db );
            cvConvert( &tdist, distCoeffs );
        }
    }

    if( matE || matF )
    {
        double* t = T_LR->tt.db;
        double tx[] =
        {
            0, -t[2], t[1],
            t[2], 0, -t[0],
            -t[1], t[0], 0
        };
        img_t Tx = cvMat1(3, 3, CC_64F, tx);
        double e[9], f[9];
        img_t E = cvMat1(3, 3, CC_64F, e);
        img_t F = cvMat1(3, 3, CC_64F, f);
        cvMatMul( &Tx, &R_LR, &E );
        if( matE )
            cvConvert( &E, matE );
        if( matF )
        {
            double ik[9];
            img_t iK = cvMat1(3, 3, CC_64F, ik);
            cvInvert(&K[1], &iK);
            cvGEMM( &iK, &E, 1, 0, 0, &E, CC_GEMM_A_T );
            cvInvert(&K[0], &iK);
            cvMatMul(&E, &iK, &F);
            cvConvertScale( &F, matF, fabs(f[8]) > 0 ? 1./f[8] : 1 );
        }
    }

    return sqrt(reprojErr/(pointsTotal*2));
}


static void
icvGetRectangles( const img_t* cameraMatrix, const img_t* distCoeffs,
                 const img_t* R, const img_t* newCameraMatrix, CSize imgSize,
                 Rect_<float>& inner, Rect_<float>& outer )
{
    const int N = 9;
    int x, y, k;
    Ptr<img_t> _pts(cvSetMat(1, N*N, CC_32FC2));
    CPoint2D32f* pts = (CPoint2D32f*)(_pts->tt.data);

    for( y = k = 0; y < N; y++ )
        for( x = 0; x < N; x++ )
            pts[k++] = cPoint2D32f((float)x*imgSize.width/(N-1),
                                    (float)y*imgSize.height/(N-1));

    cvUndistortPoints(_pts, _pts, cameraMatrix, distCoeffs, R, newCameraMatrix);

    float iX0=-FLT_MAX, iX1=FLT_MAX, iY0=-FLT_MAX, iY1=FLT_MAX;
    float oX0=FLT_MAX, oX1=-FLT_MAX, oY0=FLT_MAX, oY1=-FLT_MAX;
    // find the inscribed rectangle.
    // the code will likely not work with extreme rotation matrices (R) (>45%)
    for( y = k = 0; y < N; y++ )
        for( x = 0; x < N; x++ )
        {
            CPoint2D32f p = pts[k++];
            oX0 = MIN(oX0, p.x);
            oX1 = MAX(oX1, p.x);
            oY0 = MIN(oY0, p.y);
            oY1 = MAX(oY1, p.y);

            if( x == 0 )
                iX0 = MAX(iX0, p.x);
            if( x == N-1 )
                iX1 = MIN(iX1, p.x);
            if( y == 0 )
                iY0 = MAX(iY0, p.y);
            if( y == N-1 )
                iY1 = MIN(iY1, p.y);
        }
    inner = Rect_<float>(iX0, iY0, iX1-iX0, iY1-iY0);
    outer = Rect_<float>(oX0, oY0, oX1-oX0, oY1-oY0);
}


void cvStereoRectify( const img_t* _cameraMatrix1, const img_t* _cameraMatrix2,
                      const img_t* _distCoeffs1, const img_t* _distCoeffs2,
                      CSize imageSize, const img_t* matR, const img_t* matT,
                      img_t* _R1, img_t* _R2, img_t* _P1, img_t* _P2,
                      img_t* matQ, int flags, double alpha, CSize newImgSize,
                      CRect* roi1, CRect* roi2 )
{
    double _om[3], _t[3], _uu[3]={0,0,0}, _r_r[3][3], _pp[3][4];
    double _ww[3], _wr[3][3], _z[3] = {0,0,0}, _ri[3][3];
    Rect_<float> inner1, inner2, outer1, outer2;

    img_t om  = cvMat1(3, 1, CC_64F, _om);
    img_t t   = cvMat1(3, 1, CC_64F, _t);
    img_t uu  = cvMat1(3, 1, CC_64F, _uu);
    img_t r_r = cvMat1(3, 3, CC_64F, _r_r);
    img_t pp  = cvMat1(3, 4, CC_64F, _pp);
    img_t ww  = cvMat1(3, 1, CC_64F, _ww); // temps
    img_t wR  = cvMat1(3, 3, CC_64F, _wr);
    img_t Z   = cvMat1(3, 1, CC_64F, _z);
    img_t Ri  = cvMat1(3, 3, CC_64F, _ri);
    double nx = imageSize.width, ny = imageSize.height;
    int i, k;

    if( matR->rows == 3 && matR->cols == 3 )
        cvRodrigues2(matR, &om);          // get vector rotation
    else
        cvConvert(matR, &om); // it's already a rotation vector
    cvConvertScale(&om, &om, -0.5); // get average rotation
    cvRodrigues2(&om, &r_r);        // rotate cameras to same orientation by averaging
    cvMatMul(&r_r, matT, &t);

    int idx = fabs(_t[0]) > fabs(_t[1]) ? 0 : 1;
    double c = _t[idx], nt = cvNorm(&t, 0, CC_L2);
    _uu[idx] = c > 0 ? 1 : -1;

    // calculate global Z rotation
    cvCrossProduct(&t,&uu,&ww);
    double nw = cvNorm(&ww, 0, CC_L2);
    if (nw > 0.0)
        cvConvertScale(&ww, &ww, acos(fabs(c)/nt)/nw);
    cvRodrigues2(&ww, &wR);

    // apply to both views
    cvGEMM(&wR, &r_r, 1, 0, 0, &Ri, CC_GEMM_B_T);
    cvConvert( &Ri, _R1 );
    cvGEMM(&wR, &r_r, 1, 0, 0, &Ri, 0);
    cvConvert( &Ri, _R2 );
    cvMatMul(&Ri, matT, &t);

    // calculate projection/camera matrices
    // these contain the relevant rectified image internal params (fx, fy=fx, cx, cy)
    double fc_new = DBL_MAX;
    CPoint2D64f cc_new[2] = {{0,0}, {0,0}};

    for( k = 0; k < 2; k++ ) {
        const img_t* A = k == 0 ? _cameraMatrix1 : _cameraMatrix2;
        const img_t* Dk = k == 0 ? _distCoeffs1 : _distCoeffs2;
        double dk1 = Dk && Dk->tt.data ? cvmGet(Dk, 0, 0) : 0;
        double fc = cvmGet(A,idx^1,idx^1);
        if( dk1 < 0 ) {
            fc *= 1 + dk1*(nx*nx + ny*ny)/(4*fc*fc);
        }
        fc_new = MIN(fc_new, fc);
    }

    for( k = 0; k < 2; k++ )
    {
        const img_t* A = k == 0 ? _cameraMatrix1 : _cameraMatrix2;
        const img_t* Dk = k == 0 ? _distCoeffs1 : _distCoeffs2;
        CPoint2D32f _pts[4];
        CPoint3D32f _pts_3[4];
        img_t pts = cvMat1(1, 4, CC_32FC2, _pts);
        img_t pts_3 = cvMat1(1, 4, CC_32FC3, _pts_3);

        for( i = 0; i < 4; i++ )
        {
            int j = (i<2) ? 0 : 1;
            _pts[i].x = (float)((i % 2)*(nx));
            _pts[i].y = (float)(j*(ny));
        }
        cvUndistortPoints( &pts, &pts, A, Dk, 0, 0 );
        cvConvertPointsHomogeneous( &pts, &pts_3 );

        //Change camera matrix to have cc=[0,0] and fc = fc_new
        double _a_tmp[3][3];
        img_t A_tmp  = cvMat1(3, 3, CC_64F, _a_tmp);
        _a_tmp[0][0]=fc_new;
        _a_tmp[1][1]=fc_new;
        _a_tmp[0][2]=0.0;
        _a_tmp[1][2]=0.0;
        cvProjectPoints2( &pts_3, k == 0 ? _R1 : _R2, &Z, &A_tmp, 0, &pts );
        CScalar avg = cvAvg(&pts);
        cc_new[k].x = (nx)/2 - avg.val[0];
        cc_new[k].y = (ny)/2 - avg.val[1];
    }

    // vertical focal length must be the same for both images to keep the epipolar constraint
    // (for horizontal epipolar lines -- TBD: check for vertical epipolar lines)
    // use fy for fx also, for simplicity

    // For simplicity, set the principal points for both cameras to be the average
    // of the two principal points (either one of or both x- and y- coordinates)
    if( flags & CC_CALIB_ZERO_DISPARITY )
    {
        cc_new[0].x = cc_new[1].x = (cc_new[0].x + cc_new[1].x)*0.5;
        cc_new[0].y = cc_new[1].y = (cc_new[0].y + cc_new[1].y)*0.5;
    }
    else if( idx == 0 ) // horizontal stereo
        cc_new[0].y = cc_new[1].y = (cc_new[0].y + cc_new[1].y)*0.5;
    else // vertical stereo
        cc_new[0].x = cc_new[1].x = (cc_new[0].x + cc_new[1].x)*0.5;

    cvZero( &pp );
    _pp[0][0] = _pp[1][1] = fc_new;
    _pp[0][2] = cc_new[0].x;
    _pp[1][2] = cc_new[0].y;
    _pp[2][2] = 1;
    cvConvert(&pp, _P1);

    _pp[0][2] = cc_new[1].x;
    _pp[1][2] = cc_new[1].y;
    _pp[idx][3] = _t[idx]*fc_new; // baseline * focal length
    cvConvert(&pp, _P2);

    alpha = MIN(alpha, 1.);

    icvGetRectangles( _cameraMatrix1, _distCoeffs1, _R1, _P1, imageSize, inner1, outer1 );
    icvGetRectangles( _cameraMatrix2, _distCoeffs2, _R2, _P2, imageSize, inner2, outer2 );

    {
    newImgSize = newImgSize.width*newImgSize.height != 0 ? newImgSize : imageSize;
    double cx1_0 = cc_new[0].x;
    double cy1_0 = cc_new[0].y;
    double cx2_0 = cc_new[1].x;
    double cy2_0 = cc_new[1].y;
    double cx1 = newImgSize.width*cx1_0/imageSize.width;
    double cy1 = newImgSize.height*cy1_0/imageSize.height;
    double cx2 = newImgSize.width*cx2_0/imageSize.width;
    double cy2 = newImgSize.height*cy2_0/imageSize.height;
    double s = 1.;

    if( alpha >= 0 )
    {
        double s0 = MAX(MAX(MAX((double)cx1/(cx1_0 - inner1.x), (double)cy1/(cy1_0 - inner1.y)),
                            (double)(newImgSize.width - cx1)/(inner1.x + inner1.width - cx1_0)),
                        (double)(newImgSize.height - cy1)/(inner1.y + inner1.height - cy1_0));
        s0 = MAX(MAX(MAX(MAX((double)cx2/(cx2_0 - inner2.x), (double)cy2/(cy2_0 - inner2.y)),
                         (double)(newImgSize.width - cx2)/(inner2.x + inner2.width - cx2_0)),
                     (double)(newImgSize.height - cy2)/(inner2.y + inner2.height - cy2_0)),
                 s0);

        double s1 = MIN(MIN(MIN((double)cx1/(cx1_0 - outer1.x), (double)cy1/(cy1_0 - outer1.y)),
                            (double)(newImgSize.width - cx1)/(outer1.x + outer1.width - cx1_0)),
                        (double)(newImgSize.height - cy1)/(outer1.y + outer1.height - cy1_0));
        s1 = MIN(MIN(MIN(MIN((double)cx2/(cx2_0 - outer2.x), (double)cy2/(cy2_0 - outer2.y)),
                         (double)(newImgSize.width - cx2)/(outer2.x + outer2.width - cx2_0)),
                     (double)(newImgSize.height - cy2)/(outer2.y + outer2.height - cy2_0)),
                 s1);

        s = s0*(1 - alpha) + s1*alpha;
    }

    fc_new *= s;
    cc_new[0] = cvPoint2D64f(cx1, cy1);
    cc_new[1] = cvPoint2D64f(cx2, cy2);

    cvmSet(_P1, 0, 0, fc_new);
    cvmSet(_P1, 1, 1, fc_new);
    cvmSet(_P1, 0, 2, cx1);
    cvmSet(_P1, 1, 2, cy1);

    cvmSet(_P2, 0, 0, fc_new);
    cvmSet(_P2, 1, 1, fc_new);
    cvmSet(_P2, 0, 2, cx2);
    cvmSet(_P2, 1, 2, cy2);
    cvmSet(_P2, idx, 3, s*cvmGet(_P2, idx, 3));

    if(roi1)
    {
        *roi1 = Rect(cCeil((inner1.x - cx1_0)*s + cx1),
                     cCeil((inner1.y - cy1_0)*s + cy1),
                     cFloor(inner1.width*s), cFloor(inner1.height*s))
            & Rect(0, 0, newImgSize.width, newImgSize.height);
    }

    if(roi2)
    {
        *roi2 = Rect(cCeil((inner2.x - cx2_0)*s + cx2),
                     cCeil((inner2.y - cy2_0)*s + cy2),
                     cFloor(inner2.width*s), cFloor(inner2.height*s))
            & Rect(0, 0, newImgSize.width, newImgSize.height);
    }
    }

    if( matQ )
    {
        double q[] =
        {
            1, 0, 0, -cc_new[0].x,
            0, 1, 0, -cc_new[0].y,
            0, 0, 0, fc_new,
            0, 0, -1./_t[idx],
            (idx == 0 ? cc_new[0].x - cc_new[1].x : cc_new[0].y - cc_new[1].y)/_t[idx]
        };
        img_t Q = cvMat1(4, 4, CC_64F, q);
        cvConvert( &Q, matQ );
    }
}


void cvGetOptimalNewCameraMatrix( const img_t* cameraMatrix, const img_t* distCoeffs,
                                  CSize imgSize, double alpha,
                                  img_t* newCameraMatrix, CSize newImgSize,
                                  CRect* validPixROI, int centerPrincipalPoint )
{
    Rect_<float> inner, outer;
    newImgSize = newImgSize.width*newImgSize.height != 0 ? newImgSize : imgSize;

    double M[3][3];
    img_t matM = cvMat1(3, 3, CC_64F, M);
    cvConvert(cameraMatrix, &matM);

    if( centerPrincipalPoint )
    {
        double cx0 = M[0][2];
        double cy0 = M[1][2];
        double cx = (newImgSize.width)*0.5;
        double cy = (newImgSize.height)*0.5;

        icvGetRectangles( cameraMatrix, distCoeffs, 0, cameraMatrix, imgSize, inner, outer );
        double s0 = MAX(MAX(MAX((double)cx/(cx0 - inner.x), (double)cy/(cy0 - inner.y)),
                                      (double)cx/(inner.x + inner.width - cx0)),
                             (double)cy/(inner.y + inner.height - cy0));
        double s1 = MIN(MIN(MIN((double)cx/(cx0 - outer.x), (double)cy/(cy0 - outer.y)),
                                      (double)cx/(outer.x + outer.width - cx0)),
                             (double)cy/(outer.y + outer.height - cy0));
        double s = s0*(1 - alpha) + s1*alpha;

        M[0][0] *= s;
        M[1][1] *= s;
        M[0][2] = cx;
        M[1][2] = cy;

        if( validPixROI )
        {
            inner = Rect_<float>((float)((inner.x - cx0)*s + cx),
                                     (float)((inner.y - cy0)*s + cy),
                                     (float)(inner.width*s),
                                     (float)(inner.height*s));
            Rect r(cCeil(inner.x), cCeil(inner.y), cFloor(inner.width), cFloor(inner.height));
            r &= Rect(0, 0, newImgSize.width, newImgSize.height);
            *validPixROI = r;
        }
    }
    else
    {
        // Get inscribed and circumscribed rectangles in normalized
        // (independent of camera matrix) coordinates
        icvGetRectangles( cameraMatrix, distCoeffs, 0, 0, imgSize, inner, outer );

        // Projection mapping inner rectangle to viewport
        double fx0 = (newImgSize.width) / inner.width;
        double fy0 = (newImgSize.height) / inner.height;
        double cx0 = -fx0 * inner.x;
        double cy0 = -fy0 * inner.y;

        // Projection mapping outer rectangle to viewport
        double fx1 = (newImgSize.width) / outer.width;
        double fy1 = (newImgSize.height) / outer.height;
        double cx1 = -fx1 * outer.x;
        double cy1 = -fy1 * outer.y;

        // Interpolate between the two optimal projections
        M[0][0] = fx0*(1 - alpha) + fx1*alpha;
        M[1][1] = fy0*(1 - alpha) + fy1*alpha;
        M[0][2] = cx0*(1 - alpha) + cx1*alpha;
        M[1][2] = cy0*(1 - alpha) + cy1*alpha;

        if( validPixROI )
        {
            icvGetRectangles( cameraMatrix, distCoeffs, 0, &matM, imgSize, inner, outer );
            Rect r = inner;
            r &= Rect(0, 0, newImgSize.width, newImgSize.height);
            *validPixROI = r;
        }
    }

    cvConvert(&matM, newCameraMatrix);
}


CC_IMPL int cvStereoRectifyUncalibrated(
    const img_t* _points1, const img_t* _points2,
    const img_t* F0, CSize imgSize,
    img_t* _H1, img_t* _H2, double threshold )
{
    Ptr<img_t> _m1, _m2, _lines1, _lines2;

    int i, j, npoints;
    double cx, cy;
    double u[9], v[9], w[9], f[9], h1[9], h2[9], h0[9], e2[3];
    img_t E2 = cvMat1( 3, 1, CC_64F, e2 );
    img_t U = cvMat1( 3, 3, CC_64F, u );
    img_t V = cvMat1( 3, 3, CC_64F, v );
    img_t W = cvMat1( 3, 3, CC_64F, w );
    img_t F = cvMat1( 3, 3, CC_64F, f );
    img_t H1 = cvMat1( 3, 3, CC_64F, h1 );
    img_t H2 = cvMat1( 3, 3, CC_64F, h2 );
    img_t H0 = cvMat1( 3, 3, CC_64F, h0 );

    CPoint2D64f* m1;
    CPoint2D64f* m2;
    CPoint3D64f* lines1;
    CPoint3D64f* lines2;

    CC_Assert( (_points1) && (_points2) &&
        CC_ARE_SIZES_EQ(_points1, _points2) );

    npoints = _points1->rows * _points1->cols * CC_MAT_CN(_points1->tid) / 2;

    _m1.reset(cvSetMat( _points1->rows, _points1->cols, CC_64FC(CC_MAT_CN(_points1->tid)) ));
    _m2.reset(cvSetMat( _points2->rows, _points2->cols, CC_64FC(CC_MAT_CN(_points2->tid)) ));
    _lines1.reset(cvSetMat( 1, npoints, CC_64FC3 ));
    _lines2.reset(cvSetMat( 1, npoints, CC_64FC3 ));

    cvConvert( F0, &F );

    cvSVD( (img_t*)&F, &W, &U, &V, CC_SVD_U_T + CC_SVD_V_T );
    W->tt.db[8] = 0.;
    cvGEMM( &U, &W, 1, 0, 0, &W, CC_GEMM_A_T );
    cvMatMul( &W, &V, &F );

    cx = cRound( (imgSize.width)*0.5 );
    cy = cRound( (imgSize.height)*0.5 );

    cvZero( _H1 );
    cvZero( _H2 );

    cvConvert( _points1, _m1 );
    cvConvert( _points2, _m2 );
    cvReshape( _m1, _m1, 2, 1 );
    cvReshape( _m2, _m2, 2, 1 );

    m1 = (CPoint2D64f*)_m1->tt.data;
    m2 = (CPoint2D64f*)_m2->tt.data;
    lines1 = (CPoint3D64f*)_lines1->tt.data;
    lines2 = (CPoint3D64f*)_lines2->tt.data;

    if( threshold > 0 )
    {
        cvComputeCorrespondEpilines( _m1, 1, &F, _lines1 );
        cvComputeCorrespondEpilines( _m2, 2, &F, _lines2 );

        // measure distance from points to the corresponding epilines, mark outliers
        for( i = j = 0; i < npoints; i++ )
        {
            if( fabs(m1[i].x*lines2[i].x +
                     m1[i].y*lines2[i].y +
                     lines2[i].z) <= threshold &&
                fabs(m2[i].x*lines1[i].x +
                     m2[i].y*lines1[i].y +
                     lines1[i].z) <= threshold )
            {
                if( j < i )
                {
                    m1[j] = m1[i];
                    m2[j] = m2[i];
                }
                j++;
            }
        }

        npoints = j;
        if( npoints == 0 )
            return 0;
    }

    _m1->cols = _m2->cols = npoints;
    memcpy( E2->tt.db, U->tt.db + 6, sizeof(e2));
    cvScale( &E2, &E2, e2[2] > 0 ? 1 : -1 );

    double t[] =
    {
        1, 0, -cx,
        0, 1, -cy,
        0, 0, 1
    };
    img_t T = cvMat1(3, 3, CC_64F, t);
    cvMatMul( &T, &E2, &E2 );

    int mirror = e2[0] < 0;
    double d = MAX(sqrt(e2[0]*e2[0] + e2[1]*e2[1]),DBL_EPSILON);
    double alpha = e2[0]/d;
    double beta = e2[1]/d;
    double r[] =
    {
        alpha, beta, 0,
        -beta, alpha, 0,
        0, 0, 1
    };
    img_t R = cvMat1(3, 3, CC_64F, r);
    cvMatMul( &R, &T, &T );
    cvMatMul( &R, &E2, &E2 );
    double invf = fabs(e2[2]) < 1e-6*fabs(e2[0]) ? 0 : -e2[2]/e2[0];
    double k[] =
    {
        1, 0, 0,
        0, 1, 0,
        invf, 0, 1
    };
    img_t K = cvMat1(3, 3, CC_64F, k);
    cvMatMul( &K, &T, &H2 );
    cvMatMul( &K, &E2, &E2 );

    double it[] =
    {
        1, 0, cx,
        0, 1, cy,
        0, 0, 1
    };
    img_t iT = cvMat1( 3, 3, CC_64F, it );
    cvMatMul( &iT, &H2, &H2 );

    memcpy( E2->tt.db, U->tt.db + 6, sizeof(e2));
    cvScale( &E2, &E2, e2[2] > 0 ? 1 : -1 );

    double e2_x[] =
    {
        0, -e2[2], e2[1],
       e2[2], 0, -e2[0],
       -e2[1], e2[0], 0
    };
    double e2_111[] =
    {
        e2[0], e2[0], e2[0],
        e2[1], e2[1], e2[1],
        e2[2], e2[2], e2[2],
    };
    img_t E2_x = cvMat1(3, 3, CC_64F, e2_x);
    img_t E2_111 = cvMat1(3, 3, CC_64F, e2_111);
    cvMatMulAdd(&E2_x, &F, &E2_111, &H0 );
    cvMatMul(&H2, &H0, &H0);
    img_t E1=cvMat1(3, 1, CC_64F, V->tt.db+6);
    cvMatMul(&H0, &E1, &E1);

    cvPerspectiveTransform( _m1, _m1, &H0 );
    cvPerspectiveTransform( _m2, _m2, &H2 );
    img_t A = cvMat1( 1, npoints, CC_64FC3, lines1 ), BxBy, B;
    double x[3];
    img_t X = cvMat1( 3, 1, CC_64F, x );
    cvConvertPointsHomogeneous( _m1, &A );
    cvReshape( &A, &A, 1, npoints );
    cvReshape( _m2, &BxBy, 1, npoints );
    cvGetCol( &BxBy, &B, 0 );
    cvSolve( &A, &B, &X, CC_SVD );

    double ha[] =
    {
        x[0], x[1], x[2],
        0, 1, 0,
        0, 0, 1
    };
    img_t Ha = cvMat1(3, 3, CC_64F, ha);
    cvMatMul( &Ha, &H0, &H1 );
    cvPerspectiveTransform( _m1, _m1, &Ha );

    if( mirror )
    {
        double mm[] = { -1, 0, cx*2, 0, -1, cy*2, 0, 0, 1 };
        img_t MM = cvMat1(3, 3, CC_64F, mm);
        cvMatMul( &MM, &H1, &H1 );
        cvMatMul( &MM, &H2, &H2 );
    }

    cvConvert( &H1, _H1 );
    cvConvert( &H2, _H2 );

    return 1;
}

void reprojectImageTo3D( const img_t* _disparity, img_t* __3dImage,
                        const img_t* _Qmat, bool handleMissingValues, int dtype )
{
    CC_INSTRUMENT_REGION()

    img_t disparity = _disparity, Q = _Qmat;
    int stype = disparity->tid;

    CC_Assert( stype == CC_8UC1 || stype == CC_16SC1 ||
               stype == CC_32SC1 || stype == CC_32FC1 );
    CC_Assert( Q.size() == CSize(4,4) );

    if( dtype < 0 )
        dtype = CC_32FC3;
    else
    {
        dtype = CC_MAKETYPECN(CC_MAT_DEPTH(dtype), 3);
        CC_Assert( dtype == CC_16SC3 || dtype == CC_32SC3 || dtype == CC_32FC3 );
    }

    __3dImage cvSetMat(disparity.size(), CC_MAKETYPECN(dtype, 3));
    img_t _3dImage = __3dImage;

    const double bigZ = 10000.;
    double q[4][4];
    img_t _Q(4, 4, CC_64F, q);
    Q.convertTo(_Q, CC_64F);

    int x, cols = disparity->cols;
    CC_Assert( cols >= 0 );

    std::vector<float> _sbuf(cols+1), _dbuf(cols*3+1);
    float* sbuf = &_sbuf[0], *dbuf = &_dbuf[0];
    double minDisparity = FLT_MAX;

    // NOTE: here we quietly assume that at least one pixel in the disparity map is not defined.
    // and we set the corresponding Z's to some fixed big value.
    if( handleMissingValues )
        minMaxIdx( disparity, &minDisparity, 0, 0, 0 );

    for( int y = 0; y < disparity->rows; y++ )
    {
        float *sptr = sbuf, *dptr = dbuf;
        double qx = q[0][1]*y + q[0][3], qy = q[1][1]*y + q[1][3];
        double qz = q[2][1]*y + q[2][3], qw = q[3][1]*y + q[3][3];

        if( stype == CC_8UC1 )
        {
            const uchar* sptr0 = disparity.ptr<uchar>(y);
            for( x = 0; x < cols; x++ )
                sptr[x] = (float)sptr0[x];
        }
        else if( stype == CC_16SC1 )
        {
            const short* sptr0 = disparity.ptr<short>(y);
            for( x = 0; x < cols; x++ )
                sptr[x] = (float)sptr0[x];
        }
        else if( stype == CC_32SC1 )
        {
            const int* sptr0 = disparity.ptr<int>(y);
            for( x = 0; x < cols; x++ )
                sptr[x] = (float)sptr0[x];
        }
        else
            sptr = (float*)disparity.ptr<float>(y);

        if( dtype == CC_32FC3 )
            dptr = _3dImage.ptr<float>(y);

        for( x = 0; x < cols; x++, qx += q[0][0], qy += q[1][0], qz += q[2][0], qw += q[3][0] )
        {
            double d = sptr[x];
            double iW = 1./(qw + q[3][2]*d);
            double X = (qx + q[0][2]*d)*iW;
            double Y = (qy + q[1][2]*d)*iW;
            double Z = (qz + q[2][2]*d)*iW;
            if( fabs(d-minDisparity) <= FLT_EPSILON )
                Z = bigZ;

            dptr[x*3] = (float)X;
            dptr[x*3+1] = (float)Y;
            dptr[x*3+2] = (float)Z;
        }

        if( dtype == CC_16SC3 )
        {
            short* dptr0 = _3dImage.ptr<short>(y);
            for( x = 0; x < cols*3; x++ )
            {
                int ival = cRound(dptr[x]);
                dptr0[x] = saturate_cast<short>(ival);
            }
        }
        else if( dtype == CC_32SC3 )
        {
            int* dptr0 = _3dImage.ptr<int>(y);
            for( x = 0; x < cols*3; x++ )
            {
                int ival = cRound(dptr[x]);
                dptr0[x] = ival;
            }
        }
    }
}


void cvReprojectImageTo3D( const img_t* disparityImage,
                           img_t* _3dImage, const img_t* matQ,
                           int handleMissingValues )
{
    img_t disp = cvarrToMat(disparityImage);
    img_t _3dimg = cvarrToMat(_3dImage);
    img_t mq = cvarrToMat(matQ);
    CC_Assert( disp.size() == _3dimg.size() );
    int dtype = _3dimg->tid;
    CC_Assert( dtype == CC_16SC3 || dtype == CC_32SC3 || dtype == CC_32FC3 );

    reprojectImageTo3D(disp, _3dimg, mq, handleMissingValues != 0, dtype );
}


CC_IMPL void
cvRQDecomp3x3( const img_t *matrixM, img_t *matrixR, img_t *matrixQ,
               img_t *matrixQx, img_t *matrixQy, img_t *matrixQz,
               CPoint3D64f *eulerAngles)
{
    double matM[3][3], matR[3][3], matQ[3][3];
    img_t M = cvMat1(3, 3, CC_64F, matM);
    img_t R = cvMat1(3, 3, CC_64F, matR);
    img_t Q = cvMat1(3, 3, CC_64F, matQ);
    double z, c, s;

    /* Validate parameters. */
    CC_Assert( (matrixM) && (matrixR) && (matrixQ) &&
        matrixM->cols == 3 && matrixM->rows == 3 &&
        CC_ARE_SIZES_EQ(matrixM, matrixR) && CC_ARE_SIZES_EQ(matrixM, matrixQ));

    cvConvert(matrixM, &M);

    /* Find Givens rotation Q_x for x axis (left multiplication). */
    /*
         ( 1  0  0 )
    Qx = ( 0  c  s ), c = m33/sqrt(m32^2 + m33^2), s = m32/sqrt(m32^2 + m33^2)
         ( 0 -s  c )
    */
    s = matM[2][1];
    c = matM[2][2];
    z = 1./sqrt(c * c + s * s + DBL_EPSILON);
    c *= z;
    s *= z;

    double _Qx[3][3] = { {1, 0, 0}, {0, c, s}, {0, -s, c} };
    img_t Qx = cvMat1(3, 3, CC_64F, _Qx);

    cvMatMul(&M, &Qx, &R);
    assert(fabs(matR[2][1]) < FLT_EPSILON);
    matR[2][1] = 0;

    /* Find Givens rotation for y axis. */
    /*
         ( c  0 -s )
    Qy = ( 0  1  0 ), c = m33/sqrt(m31^2 + m33^2), s = -m31/sqrt(m31^2 + m33^2)
         ( s  0  c )
    */
    s = -matR[2][0];
    c = matR[2][2];
    z = 1./sqrt(c * c + s * s + DBL_EPSILON);
    c *= z;
    s *= z;

    double _Qy[3][3] = { {c, 0, -s}, {0, 1, 0}, {s, 0, c} };
    img_t Qy = cvMat1(3, 3, CC_64F, _Qy);
    cvMatMul(&R, &Qy, &M);

    assert(fabs(matM[2][0]) < FLT_EPSILON);
    matM[2][0] = 0;

    /* Find Givens rotation for z axis. */
    /*
         ( c  s  0 )
    Qz = (-s  c  0 ), c = m22/sqrt(m21^2 + m22^2), s = m21/sqrt(m21^2 + m22^2)
         ( 0  0  1 )
    */

    s = matM[1][0];
    c = matM[1][1];
    z = 1./sqrt(c * c + s * s + DBL_EPSILON);
    c *= z;
    s *= z;

    double _Qz[3][3] = { {c, s, 0}, {-s, c, 0}, {0, 0, 1} };
    img_t Qz = cvMat1(3, 3, CC_64F, _Qz);

    cvMatMul(&M, &Qz, &R);
    assert(fabs(matR[1][0]) < FLT_EPSILON);
    matR[1][0] = 0;

    // Solve the decomposition ambiguity.
    // Diagonal entries of R, except the last one, shall be positive.
    // Further rotate R by 180 degree if necessary
    if( matR[0][0] < 0 )
    {
        if( matR[1][1] < 0 )
        {
            // rotate around z for 180 degree, i.e. a rotation matrix of
            // [-1,  0,  0],
            // [ 0, -1,  0],
            // [ 0,  0,  1]
            matR[0][0] *= -1;
            matR[0][1] *= -1;
            matR[1][1] *= -1;

            _Qz[0][0] *= -1;
            _Qz[0][1] *= -1;
            _Qz[1][0] *= -1;
            _Qz[1][1] *= -1;
        }
        else
        {
            // rotate around y for 180 degree, i.e. a rotation matrix of
            // [-1,  0,  0],
            // [ 0,  1,  0],
            // [ 0,  0, -1]
            matR[0][0] *= -1;
            matR[0][2] *= -1;
            matR[1][2] *= -1;
            matR[2][2] *= -1;

            cvTranspose( &Qz, &Qz );

            _Qy[0][0] *= -1;
            _Qy[0][2] *= -1;
            _Qy[2][0] *= -1;
            _Qy[2][2] *= -1;
        }
    }
    else if( matR[1][1] < 0 )
    {
        // ??? for some reason, we never get here ???

        // rotate around x for 180 degree, i.e. a rotation matrix of
        // [ 1,  0,  0],
        // [ 0, -1,  0],
        // [ 0,  0, -1]
        matR[0][1] *= -1;
        matR[0][2] *= -1;
        matR[1][1] *= -1;
        matR[1][2] *= -1;
        matR[2][2] *= -1;

        cvTranspose( &Qz, &Qz );
        cvTranspose( &Qy, &Qy );

        _Qx[1][1] *= -1;
        _Qx[1][2] *= -1;
        _Qx[2][1] *= -1;
        _Qx[2][2] *= -1;
    }

    // calculate the euler angle
    if( eulerAngles )
    {
        eulerAngles->x = acos(_Qx[1][1]) * (_Qx[1][2] >= 0 ? 1 : -1) * (180.0 / CC_PI);
        eulerAngles->y = acos(_Qy[0][0]) * (_Qy[2][0] >= 0 ? 1 : -1) * (180.0 / CC_PI);
        eulerAngles->z = acos(_Qz[0][0]) * (_Qz[0][1] >= 0 ? 1 : -1) * (180.0 / CC_PI);
    }

    /* Calulate orthogonal matrix. */
    /*
    Q = QzT * QyT * QxT
    */
    cvGEMM( &Qz, &Qy, 1, 0, 0, &M, CC_GEMM_A_T + CC_GEMM_B_T );
    cvGEMM( &M, &Qx, 1, 0, 0, &Q, CC_GEMM_B_T );

    /* Save R and Q matrices. */
    cvConvert( &R, matrixR );
    cvConvert( &Q, matrixQ );

    if( matrixQx )
        cvConvert(&Qx, matrixQx);
    if( matrixQy )
        cvConvert(&Qy, matrixQy);
    if( matrixQz )
        cvConvert(&Qz, matrixQz);
}


CC_IMPL void
cvDecomposeProjectionMatrix( const img_t *projMatr, img_t *calibMatr,
                             img_t *rotMatr, img_t *posVect,
                             img_t *rotMatrX, img_t *rotMatrY,
                             img_t *rotMatrZ, CPoint3D64f *eulerAngles)
{
    double tmpProjMatrData[16], tmpMatrixDData[16], tmpMatrixVData[16];
    img_t tmpProjMatr = cvMat1(4, 4, CC_64F, tmpProjMatrData);
    img_t tmpMatrixD = cvMat1(4, 4, CC_64F, tmpMatrixDData);
    img_t tmpMatrixV = cvMat1(4, 4, CC_64F, tmpMatrixVData);
    img_t tmpMatrixM;

    /* Validate parameters. */
    if(projMatr == 0 || calibMatr == 0 || rotMatr == 0 || posVect == 0)
        CC_Error(CC_StsNullPtr, "Some of parameters is a NULL pointer!");

    if(!(projMatr) || !(calibMatr) || !(rotMatr) || !(posVect))
        CC_Error(CC_StsUnsupportedFormat, "Input parameters must be a matrices!");

    if(projMatr->cols != 4 || projMatr->rows != 3)
        CC_Error(CC_StsUnmatchedSizes, "CSize of projection matrix must be 3x4!");

    if(calibMatr->cols != 3 || calibMatr->rows != 3 || rotMatr->cols != 3 || rotMatr->rows != 3)
        CC_Error(CC_StsUnmatchedSizes, "CSize of calibration and rotation matrices must be 3x3!");

    if(posVect->cols != 1 || posVect->rows != 4)
        CC_Error(CC_StsUnmatchedSizes, "CSize of position vector must be 4x1!");

    /* Compute position vector. */
    cvSetZero(&tmpProjMatr); // Add zero row to make matrix square.
    int i, k;
    for(i = 0; i < 3; i++)
        for(k = 0; k < 4; k++)
            cvmSet(&tmpProjMatr, i, k, cvmGet(projMatr, i, k));

    cvSVD(&tmpProjMatr, &tmpMatrixD, NULL, &tmpMatrixV, CC_SVD_MODIFY_A + CC_SVD_V_T);

    /* Save position vector. */
    for(i = 0; i < 4; i++)
        cvmSet(posVect, i, 0, cvmGet(&tmpMatrixV, 3, i)); // Solution is last row of V.

    /* Compute calibration and rotation matrices via RQ decomposition. */
    cvGetCols(projMatr, &tmpMatrixM, 0, 3); // M is first square matrix of P.

    CC_Assert(cvDet(&tmpMatrixM) != 0.0); // So far only finite cameras could be decomposed, so M has to be nonsingular [det(M) != 0].

    cvRQDecomp3x3(&tmpMatrixM, calibMatr, rotMatr, rotMatrX, rotMatrY, rotMatrZ, eulerAngles);
}



namespace cv
{

static void collectCalibrationData( InputArrayOfArrays objectPoints,
                                    InputArrayOfArrays imagePoints1,
                                    InputArrayOfArrays imagePoints2,
                                    img_t& objPtMat, img_t& imgPtMat1, img_t* imgPtMat2,
                                    img_t& npoints )
{
    int nimages = (int)objectPoints CC_MAT_TOTAL();
    int i, j = 0, ni = 0, total = 0;
    CC_Assert(nimages > 0 && nimages == (int)imagePoints1 CC_MAT_TOTAL() &&
        (!imgPtMat2 || nimages == (int)imagePoints2 CC_MAT_TOTAL()));

    for( i = 0; i < nimages; i++ )
    {
        ni = objectPoints.getMat(i) cvCheckVector(3, CC_32F);
        if( ni <= 0 )
            CC_Error(CC_StsUnsupportedFormat, "objectPoints should contain vector of vectors of points of type CPoint3f");
        int ni1 = imagePoints1.getMat(i) cvCheckVector(2, CC_32F);
        if( ni1 <= 0 )
            CC_Error(CC_StsUnsupportedFormat, "imagePoints1 should contain vector of vectors of points of type CPoint2f");
        CC_Assert( ni == ni1 );

        total += ni;
    }

    npoints cvSetMat(1, (int)nimages, CC_32S);
    objPtMat cvSetMat(1, (int)total, CC_32FC3);
    imgPtMat1 cvSetMat(1, (int)total, CC_32FC2);
    CPoint2f* imgPtData2 = 0;

    if( imgPtMat2 )
    {
        imgPtMat2->create(1, (int)total, CC_32FC2);
        imgPtData2 = imgPtMat2->ptr<CPoint2f>();
    }

    CPoint3f* objPtData = objPtMat(CPoint3f*)(->tt.data);
    CPoint2f* imgPtData1 = imgPtMat1(CPoint2f*)(->tt.data);

    for( i = 0; i < nimages; i++, j += ni )
    {
        img_t objpt = objectPoints.getMat(i);
        img_t imgpt1 = imagePoints1.getMat(i);
        ni = objpt cvCheckVector(3, CC_32F);
        npoints.at<int>(i) = ni;
        memcpy( objPtData + j, objpt->tt.data, ni*sizeof(objPtData[0]) );
        memcpy( imgPtData1 + j, imgpt1->tt.data, ni*sizeof(imgPtData1[0]) );

        if( imgPtData2 )
        {
            img_t imgpt2 = imagePoints2.getMat(i);
            int ni2 = imgpt2 cvCheckVector(2, CC_32F);
            CC_Assert( ni == ni2 );
            memcpy( imgPtData2 + j, imgpt2->tt.data, ni*sizeof(imgPtData2[0]) );
        }
    }
}

static img_t prepareCameraMatrix(img_t& cameraMatrix0, int rtype)
{
    img_t cameraMatrix = img_t::eye(3, 3, rtype);
    if( cameraMatrix0.size() == cameraMatrix.size() )
        cameraMatrix0.convertTo(cameraMatrix, rtype);
    return cameraMatrix;
}

static img_t prepareDistCoeffs(img_t& distCoeffs0, int rtype, int outputSize = 14)
{
    CC_Assert((int)distCoeffs0 CC_MAT_TOTAL() <= outputSize);
    img_t distCoeffs = img_t::zeros(distCoeffs0->cols == 1 ? CSize(1, outputSize) : CSize(outputSize, 1), rtype);
    if( distCoeffs0.size() == CSize(1, 4) ||
       distCoeffs0.size() == CSize(1, 5) ||
       distCoeffs0.size() == CSize(1, 8) ||
       distCoeffs0.size() == CSize(1, 12) ||
       distCoeffs0.size() == CSize(1, 14) ||
       distCoeffs0.size() == CSize(4, 1) ||
       distCoeffs0.size() == CSize(5, 1) ||
       distCoeffs0.size() == CSize(8, 1) ||
       distCoeffs0.size() == CSize(12, 1) ||
       distCoeffs0.size() == CSize(14, 1) )
    {
        img_t dstCoeffs(distCoeffs, Rect(0, 0, distCoeffs0->cols, distCoeffs0->rows));
        distCoeffs0.convertTo(dstCoeffs, rtype);
    }
    return distCoeffs;
}

} // namespace cv


void Rodrigues(const img_t* _src, img_t* _dst, img_t* _jacobian)
{
    CC_INSTRUMENT_REGION()

    img_t *src = _src;
    bool v2m = src->cols == 1 || src->rows == 1;
    _dst cvSetMat(3, v2m ? 3 : 1, src CC_MAT_DEPTH());
    img_t *dst = _dst;
    img_t _csrc = src, _cdst = dst, _cjacobian;
    if( _jacobian.needed() )
    {
        _jacobian cvSetMat(v2m ? CSize(9, 3) : CSize(3, 9), src CC_MAT_DEPTH());
        _cjacobian = _jacobian;
    }
    bool ok = cvRodrigues2(&_csrc, &_cdst, _jacobian.needed() ? &_cjacobian : 0) > 0;
    if( !ok )
        dst = Scalar(0);
}

void matMulDeriv( const img_t* _Amat, const img_t* _Bmat,
                      img_t* _dABdA, img_t* _dABdB )
{
    CC_INSTRUMENT_REGION()

    img_t A = _Amat, B = _Bmat;
    _dABdA cvSetMat(A->rows*B->cols, A->rows*A->cols, A->tid);
    _dABdB cvSetMat(A->rows*B->cols, B->rows*B->cols, A->tid);
    img_t matA = A, matB = B, c_dABdA = _dABdA, c_dABdB = _dABdB;
    cvCalcMatMulDeriv(&matA, &matB, &c_dABdA, &c_dABdB);
}


void composeRT( const img_t* _rvec1, const img_t* _tvec1,
                    const img_t* _rvec2, const img_t* _tvec2,
                    img_t* _rvec3, img_t* _tvec3,
                    img_t* _dr3dr1, img_t* _dr3dt1,
                    img_t* _dr3dr2, img_t* _dr3dt2,
                    img_t* _dt3dr1, img_t* _dt3dt1,
                    img_t* _dt3dr2, img_t* _dt3dt2 )
{
    img_t rvec1 = _rvec1, tvec1 = _tvec1;
    img_t rvec2 = _rvec2, tvec2 = _tvec2;
    int rtype = rvec1->tid;
    _rvec3 cvSetMat(rvec1.size(), rtype);
    _tvec3 cvSetMat(tvec1.size(), rtype);
    img_t rvec3 = _rvec3, tvec3 = _tvec3;

    img_t c_rvec1 = rvec1, c_tvec1 = tvec1, c_rvec2 = rvec2,
          c_tvec2 = tvec2, c_rvec3 = rvec3, c_tvec3 = tvec3;
    img_t c_dr3dr1, c_dr3dt1, c_dr3dr2, c_dr3dt2, c_dt3dr1, c_dt3dt1, c_dt3dr2, c_dt3dt2;
    img_t *p_dr3dr1=0, *p_dr3dt1=0, *p_dr3dr2=0, *p_dr3dt2=0, *p_dt3dr1=0, *p_dt3dt1=0, *p_dt3dr2=0, *p_dt3dt2=0;
#define CC_COMPOSE_RT_PARAM(name) \
    img_t name; \
    if (_ ## name.needed())\
    { \
        _ ## name cvSetMat(3, 3, rtype); \
        name = _ ## name; \
        p_ ## name = &(c_ ## name = name); \
    }

    CC_COMPOSE_RT_PARAM(dr3dr1); CC_COMPOSE_RT_PARAM(dr3dt1);
    CC_COMPOSE_RT_PARAM(dr3dr2); CC_COMPOSE_RT_PARAM(dr3dt2);
    CC_COMPOSE_RT_PARAM(dt3dr1); CC_COMPOSE_RT_PARAM(dt3dt1);
    CC_COMPOSE_RT_PARAM(dt3dr2); CC_COMPOSE_RT_PARAM(dt3dt2);
#undef CC_COMPOSE_RT_PARAM

    cvComposeRT(&c_rvec1, &c_tvec1, &c_rvec2, &c_tvec2, &c_rvec3, &c_tvec3,
                p_dr3dr1, p_dr3dt1, p_dr3dr2, p_dr3dt2,
                p_dt3dr1, p_dt3dt1, p_dt3dr2, p_dt3dt2);
}

#endif

static void
projectPoints( const img_t* opoints,
                        const img_t* rvec,
                        const img_t* tvec,
                        const img_t* cameraMatrix,
                        const img_t* distCoeffs,
                        img_t* ipoints,
                        img_t* jacobian,
                        double aspectRatio )
{
    int npoints = cvCheckVector(opoints, 3);
    TypeId depth = CC_MAT_DEPTH(opoints);
    CC_Assert(npoints >= 0 && (depth == CC_32F || depth == CC_64F));

    IM_DEF(tmp, 10);
    img_t *pdpdrot=0, *pdpdt=0, *pdpdf=0, *pdpdc=0, *pdpddist=0;

    cvSetMat(ipoints, npoints, 1, depth, 2);
    img_t *imagePoints = ipoints;
    img_t *c_imagePoints = imagePoints;
    const img_t *c_objectPoints = opoints;
    const img_t *c_cameraMatrix = cameraMatrix;
    const img_t *c_rvec = rvec;
    const img_t *c_tvec = tvec;

    double dc0buf[5]={0};
    img_t dc0 = cvMat1(5,1,CC_64F,1,dc0buf);
    if( imempty(distCoeffs) )
        distCoeffs = &dc0;
    const img_t *c_distCoeffs = distCoeffs;
    int ndistCoeffs = distCoeffs->rows + distCoeffs->cols - 1;

    if( jacobian )
    {
        cvSetMat(jacobian, npoints*2, 3+3+2+2+ndistCoeffs, CC_64F);
        pdpdrot = cvGetCols(jacobian, tmp++, 0, 3);
        pdpdt = cvGetCols(jacobian, tmp++, 3, 6);
        pdpdf = cvGetCols(jacobian, tmp++, 6, 8);
        pdpdc = cvGetCols(jacobian, tmp++, 8, 10);
        pdpddist = cvGetCols(jacobian, tmp++, 10, 10+ndistCoeffs);
    }

    cvProjectPoints2( c_objectPoints, c_rvec, c_tvec, c_cameraMatrix, c_distCoeffs,
                      c_imagePoints, pdpdrot, pdpdt, pdpdf, pdpdc, pdpddist, aspectRatio );
}

#if 0
img_t initCameraMatrix2D( InputArrayOfArrays objectPoints,
                                InputArrayOfArrays imagePoints,
                                CSize imageSize, double aspectRatio )
{
    CC_INSTRUMENT_REGION()

    img_t objPt, imgPt, npoints, cameraMatrix(3, 3, CC_64F);
    collectCalibrationData( objectPoints, imagePoints, NULL,
                            objPt, imgPt, 0, npoints );
    img_t _objPt = objPt, _imgPt = imgPt, _npoints = npoints, _cameraMatrix = cameraMatrix;
    cvInitIntrinsicParams2D( &_objPt, &_imgPt, &_npoints,
                             imageSize, &_cameraMatrix, aspectRatio );
    return cameraMatrix;
}



double calibrateCamera( InputArrayOfArrays _objectPoints,
                            InputArrayOfArrays _imagePoints,
                            CSize imageSize, InputCvArr* cameraMatrix, InputCvArr* distCoeffs,
                            OutputArrayOfArrays _rvecs, OutputArrayOfArrays _tvecs, int flags, TermCriteria criteria )
{
    CC_INSTRUMENT_REGION()

    return calibrateCamera(_objectPoints, _imagePoints, imageSize, _cameraMatrix, _distCoeffs,
                                         _rvecs, _tvecs, NULL, NULL, NULL, flags, criteria);
}

double calibrateCamera(InputArrayOfArrays _objectPoints,
                            InputArrayOfArrays _imagePoints,
                            CSize imageSize, InputCvArr* cameraMatrix, InputCvArr* distCoeffs,
                            OutputArrayOfArrays _rvecs, OutputArrayOfArrays _tvecs,
                            img_t* stdDeviationsIntrinsics,
                            img_t* stdDeviationsExtrinsics,
                            img_t* _perViewErrors, int flags, TermCriteria criteria )
{
    CC_INSTRUMENT_REGION()

    int rtype = CC_64F;
    img_t cameraMatrix = _cameraMatrix;
    cameraMatrix = prepareCameraMatrix(cameraMatrix, rtype);
    img_t distCoeffs = _distCoeffs;
    distCoeffs = (flags & CALIB_THIN_PRISM_MODEL) && !(flags & CALIB_TILTED_MODEL)  ? prepareDistCoeffs(distCoeffs, rtype, 12) :
                                                      prepareDistCoeffs(distCoeffs, rtype);
    if( !(flags & CALIB_RATIONAL_MODEL) &&
    (!(flags & CALIB_THIN_PRISM_MODEL)) &&
    (!(flags & CALIB_TILTED_MODEL)))
        distCoeffs = distCoeffs->rows == 1 ? distCoeffs.colRange(0, 5) : distCoeffs cvGetRows(,0, 5);

    int nimages = int(_objectPoints CC_MAT_TOTAL());
    CC_Assert( nimages > 0 );
    img_t objPt, imgPt, npoints, rvecM, tvecM, stdDeviationsM, errorsM;

    bool rvecs_needed = _rvecs.needed(), tvecs_needed = _tvecs.needed(),
            stddev_needed = stdDeviationsIntrinsics.needed(), errors_needed = _perViewErrors.needed(),
            stddev_ext_needed = stdDeviationsExtrinsics.needed();

    bool rvecs_mat_vec = _rvecs.isMatVector();
    bool tvecs_mat_vec = _tvecs.isMatVector();

    if( rvecs_needed )
    {
        _rvecs cvSetMat(nimages, 1, CC_64FC3);

        if(rvecs_mat_vec)
            rvecM cvSetMat(nimages, 3, CC_64F);
        else
            rvecM = _rvecs;
    }

    if( tvecs_needed )
    {
        _tvecs cvSetMat(nimages, 1, CC_64FC3);

        if(tvecs_mat_vec)
            tvecM cvSetMat(nimages, 3, CC_64F);
        else
            tvecM = _tvecs;
    }

    if( stddev_needed || stddev_ext_needed )
    {
        stdDeviationsM cvSetMat(nimages*6 + CC_CALIB_NINTRINSIC, 1, CC_64F);
    }

    if( errors_needed )
    {
        _perViewErrors cvSetMat(nimages, 1, CC_64F);
        errorsM = _perViewErrors;
    }

    collectCalibrationData( _objectPoints, _imagePoints, NULL,
                            objPt, imgPt, 0, npoints );
    img_t c_objPt = objPt, c_imgPt = imgPt, c_npoints = npoints;
    img_t c_cameraMatrix = cameraMatrix, c_distCoeffs = distCoeffs;
    img_t c_rvecM = rvecM, c_tvecM = tvecM, c_stdDev = stdDeviationsM, c_errors = errorsM;

    double reprojErr = cvCalibrateCamera2Internal(&c_objPt, &c_imgPt, &c_npoints, imageSize,
                                          &c_cameraMatrix, &c_distCoeffs,
                                          rvecs_needed ? &c_rvecM : NULL,
                                          tvecs_needed ? &c_tvecM : NULL,
                                          stddev_needed ? &c_stdDev : NULL,
                                          errors_needed ? &c_errors : NULL, flags, criteria );

    if( stddev_needed )
    {
        stdDeviationsIntrinsics cvSetMat(CC_CALIB_NINTRINSIC, 1, CC_64F);
        img_t stdDeviationsIntrinsicsMat = stdDeviationsIntrinsics;
        std::memcpy(stdDeviationsIntrinsicsMat->tt.data, stdDeviationsM->tt.data,
                    CC_CALIB_NINTRINSIC*sizeof(double));
    }

    if ( stddev_ext_needed )
    {
        stdDeviationsExtrinsics cvSetMat(nimages*6, 1, CC_64F);
        img_t stdDeviationsExtrinsicsMat = stdDeviationsExtrinsics;
        std::memcpy(stdDeviationsExtrinsicsMat->tt.data,
                    stdDeviationsM->tt.data + CC_CALIB_NINTRINSIC*sizeof(double),
                    nimages*6*sizeof(double));
    }

    // overly complicated and inefficient rvec/ tvec handling to support vector<img_t>
    for(int i = 0; i < nimages; i++ )
    {
        if( rvecs_needed && rvecs_mat_vec)
        {
            _rvecs cvSetMat(3, 1, CC_64F, i, true);
            img_t rv = _rvecs.getMat(i);
            memcpy(rv->tt.data, rvecM.ptr(i), 3*sizeof(double));
        }
        if( tvecs_needed && tvecs_mat_vec)
        {
            _tvecs cvSetMat(3, 1, CC_64F, i, true);
            img_t tv = _tvecs.getMat(i);
            memcpy(tv->tt.data, tvecM.ptr(i), 3*sizeof(double));
        }
    }

    cameraMatrix.copyTo(_cameraMatrix);
    distCoeffs.copyTo(_distCoeffs);

    return reprojErr;
}


void calibrationMatrixValues( const img_t* _cameraMatrix, CSize imageSize,
                                  double apertureWidth, double apertureHeight,
                                  double& fovx, double& fovy, double& focalLength,
                                  CPoint2d& principalPoint, double& aspectRatio )
{
    CC_INSTRUMENT_REGION()

    if(_cameraMatrix.size() != CSize(3, 3))
        CC_Error(CC_StsUnmatchedSizes, "CSize of cameraMatrix must be 3x3!");

    Matx33d K = _cameraMatrix;

    CC_DbgAssert(imageSize.width != 0 && imageSize.height != 0 && K(0, 0) != 0.0 && K(1, 1) != 0.0);

    /* Calculate pixel aspect ratio. */
    aspectRatio = K(1, 1) / K(0, 0);

    /* Calculate number of pixel per realworld unit. */
    double mx, my;
    if(apertureWidth != 0.0 && apertureHeight != 0.0) {
        mx = imageSize.width / apertureWidth;
        my = imageSize.height / apertureHeight;
    } else {
        mx = 1.0;
        my = aspectRatio;
    }

    /* Calculate fovx and fovy. */
    fovx = atan2(K(0, 2), K(0, 0)) + atan2(imageSize.width  - K(0, 2), K(0, 0));
    fovy = atan2(K(1, 2), K(1, 1)) + atan2(imageSize.height - K(1, 2), K(1, 1));
    fovx *= 180.0 / CC_PI;
    fovy *= 180.0 / CC_PI;

    /* Calculate focal length. */
    focalLength = K(0, 0) / mx;

    /* Calculate principle point. */
    principalPoint = CPoint2d(K(0, 2) / mx, K(1, 2) / my);
}

double stereoCalibrate( InputArrayOfArrays _objectPoints,
                          InputArrayOfArrays _imagePoints1,
                          InputArrayOfArrays _imagePoints2,
                          InputCvArr* cameraMatrix1, InputCvArr* distCoeffs1,
                          InputCvArr* cameraMatrix2, InputCvArr* distCoeffs2,
                          CSize imageSize, img_t* _Rmat, img_t* _Tmat,
                          img_t* _Emat, img_t* _Fmat, int flags ,
                          TermCriteria criteria)
{
    int rtype = CC_64F;
    img_t cameraMatrix1 = _cameraMatrix1;
    img_t cameraMatrix2 = _cameraMatrix2;
    img_t distCoeffs1 = _distCoeffs1;
    img_t distCoeffs2 = _distCoeffs2;
    cameraMatrix1 = prepareCameraMatrix(cameraMatrix1, rtype);
    cameraMatrix2 = prepareCameraMatrix(cameraMatrix2, rtype);
    distCoeffs1 = prepareDistCoeffs(distCoeffs1, rtype);
    distCoeffs2 = prepareDistCoeffs(distCoeffs2, rtype);

    if( !(flags & CALIB_RATIONAL_MODEL) &&
    (!(flags & CALIB_THIN_PRISM_MODEL)) &&
    (!(flags & CALIB_TILTED_MODEL)))
    {
        distCoeffs1 = distCoeffs1->rows == 1 ? distCoeffs1.colRange(0, 5) : distCoeffs1 cvGetRows(,0, 5);
        distCoeffs2 = distCoeffs2->rows == 1 ? distCoeffs2.colRange(0, 5) : distCoeffs2 cvGetRows(,0, 5);
    }

    _Rmat cvSetMat(3, 3, rtype);
    _Tmat cvSetMat(3, 1, rtype);

    img_t objPt, imgPt, imgPt2, npoints;

    collectCalibrationData( _objectPoints, _imagePoints1, _imagePoints2,
                            objPt, imgPt, &imgPt2, npoints );
    img_t c_objPt = objPt, c_imgPt = imgPt, c_imgPt2 = imgPt2, c_npoints = npoints;
    img_t c_cameraMatrix1 = cameraMatrix1, c_distCoeffs1 = distCoeffs1;
    img_t c_cameraMatrix2 = cameraMatrix2, c_distCoeffs2 = distCoeffs2;
    img_t c_matR = _Rmat, c_matT = _Tmat, c_matE, c_matF, *p_matE = 0, *p_matF = 0;

    if( _Emat.needed() )
    {
        _Emat cvSetMat(3, 3, rtype);
        p_matE = &(c_matE = _Emat);
    }
    if( _Fmat.needed() )
    {
        _Fmat cvSetMat(3, 3, rtype);
        p_matF = &(c_matF = _Fmat);
    }

    double err = cvStereoCalibrate(&c_objPt, &c_imgPt, &c_imgPt2, &c_npoints, &c_cameraMatrix1,
        &c_distCoeffs1, &c_cameraMatrix2, &c_distCoeffs2, imageSize,
        &c_matR, &c_matT, p_matE, p_matF, flags, criteria );

    cameraMatrix1.copyTo(_cameraMatrix1);
    cameraMatrix2.copyTo(_cameraMatrix2);
    distCoeffs1.copyTo(_distCoeffs1);
    distCoeffs2.copyTo(_distCoeffs2);

    return err;
}


void stereoRectify( const img_t* _cameraMatrix1, const img_t* _distCoeffs1,
                        const img_t* _cameraMatrix2, const img_t* _distCoeffs2,
                        CSize imageSize, const img_t* _Rmat, const img_t* _Tmat,
                        img_t* _Rmat1, img_t* _Rmat2,
                        img_t* _Pmat1, img_t* _Pmat2,
                        img_t* _Qmat, int flags,
                        double alpha, CSize newImageSize,
                        Rect* validPixROI1, Rect* validPixROI2 )
{
    img_t cameraMatrix1 = _cameraMatrix1, cameraMatrix2 = _cameraMatrix2;
    img_t distCoeffs1 = _distCoeffs1, distCoeffs2 = _distCoeffs2;
    img_t Rmat = _Rmat, Tmat = _Tmat;
    img_t c_cameraMatrix1 = cameraMatrix1;
    img_t c_cameraMatrix2 = cameraMatrix2;
    img_t c_distCoeffs1 = distCoeffs1;
    img_t c_distCoeffs2 = distCoeffs2;
    img_t c_R = Rmat, c_T = Tmat;

    int rtype = CC_64F;
    _Rmat1 cvSetMat(3, 3, rtype);
    _Rmat2 cvSetMat(3, 3, rtype);
    _Pmat1 cvSetMat(3, 4, rtype);
    _Pmat2 cvSetMat(3, 4, rtype);
    img_t c_R1 = _Rmat1, c_R2 = _Rmat2, c_P1 = _Pmat1, c_P2 = _Pmat2;
    img_t c_Q, *p_Q = 0;

    if( _Qmat.needed() )
    {
        _Qmat cvSetMat(4, 4, rtype);
        p_Q = &(c_Q = _Qmat);
    }

    img_t *p_distCoeffs1 = distCoeffs1.empty() ? NULL : &c_distCoeffs1;
    img_t *p_distCoeffs2 = distCoeffs2.empty() ? NULL : &c_distCoeffs2;
    cvStereoRectify( &c_cameraMatrix1, &c_cameraMatrix2, p_distCoeffs1, p_distCoeffs2,
        imageSize, &c_R, &c_T, &c_R1, &c_R2, &c_P1, &c_P2, p_Q, flags, alpha,
        newImageSize, (CRect*)validPixROI1, (CRect*)validPixROI2);
}

bool stereoRectifyUncalibrated( const img_t* _points1, const img_t* _points2,
                                    const img_t* _Fmat, CSize imgSize,
                                    img_t* _Hmat1, img_t* _Hmat2, double threshold )
{
    CC_INSTRUMENT_REGION()

    int rtype = CC_64F;
    _Hmat1 cvSetMat(3, 3, rtype);
    _Hmat2 cvSetMat(3, 3, rtype);
    img_t F = _Fmat;
    img_t points1 = _points1, points2 = _points2;
    img_t c_pt1 = points1, c_pt2 = points2;
    img_t c_F, *p_F=0, c_H1 = _Hmat1, c_H2 = _Hmat2;
    if( F.size() == CSize(3, 3) )
        p_F = &(c_F = F);
    return cvStereoRectifyUncalibrated(&c_pt1, &c_pt2, p_F, imgSize, &c_H1, &c_H2, threshold) > 0;
}

img_t getOptimalNewCameraMatrix( const img_t* _cameraMatrix,
                                       const img_t* _distCoeffs,
                                       CSize imgSize, double alpha, CSize newImgSize,
                                       Rect* validPixROI, bool centerPrincipalPoint )
{
    CC_INSTRUMENT_REGION()

    img_t cameraMatrix = _cameraMatrix, distCoeffs = _distCoeffs;
    img_t c_cameraMatrix = cameraMatrix, c_distCoeffs = distCoeffs;

    img_t newCameraMatrix(3, 3, CC_MAT_TYPE(c_cameraMatrix.type));
    img_t c_newCameraMatrix = newCameraMatrix;

    cvGetOptimalNewCameraMatrix(&c_cameraMatrix, &c_distCoeffs, imgSize,
                                alpha, &c_newCameraMatrix,
                                newImgSize, (CRect*)validPixROI, (int)centerPrincipalPoint);
    return newCameraMatrix;
}


Vec3d RQDecomp3x3( const img_t* _Mmat,
                           img_t* _Rmat,
                           img_t* _Qmat,
                           img_t* _Qx,
                           img_t* _Qy,
                           img_t* _Qz )
{
    CC_INSTRUMENT_REGION()

    img_t M = _Mmat;
    _Rmat cvSetMat(3, 3, M->tid);
    _Qmat cvSetMat(3, 3, M->tid);
    img_t Rmat = _Rmat;
    img_t Qmat = _Qmat;
    Vec3d eulerAngles;

    img_t matM = M, matR = Rmat, matQ = Qmat;
#define CC_RQDecomp3x3_PARAM(name) \
    img_t name; \
    img_t c_ ## name, *p ## name = NULL; \
    if( _ ## name.needed() ) \
    { \
        _ ## name cvSetMat(3, 3, M->tid); \
        name = _ ## name; \
        c_ ## name = name; p ## name = &c_ ## name; \
    }

    CC_RQDecomp3x3_PARAM(Qx);
    CC_RQDecomp3x3_PARAM(Qy);
    CC_RQDecomp3x3_PARAM(Qz);
#undef CC_RQDecomp3x3_PARAM
    cvRQDecomp3x3(&matM, &matR, &matQ, pQx, pQy, pQz, (CPoint3D64f*)&eulerAngles[0]);
    return eulerAngles;
}


void decomposeProjectionMatrix( const img_t* _projMatrix, img_t* _cameraMatrix,
                                    img_t* _rotMatrix, img_t* _transVect,
                                    img_t* _rotMatrixX, img_t* _rotMatrixY,
                                    img_t* _rotMatrixZ, img_t* _eulerAngles )
{
    CC_INSTRUMENT_REGION()

    img_t projMatrix = _projMatrix;
    int type = projMatrix->tid;
    _cameraMatrix cvSetMat(3, 3, type);
    _rotMatrix cvSetMat(3, 3, type);
    _transVect cvSetMat(4, 1, type);
    img_t cameraMatrix = _cameraMatrix;
    img_t rotMatrix = _rotMatrix;
    img_t transVect = _transVect;
    img_t c_projMatrix = projMatrix, c_cameraMatrix = cameraMatrix;
    img_t c_rotMatrix = rotMatrix, c_transVect = transVect;
    CPoint3D64f *p_eulerAngles = 0;

#define CC_decomposeProjectionMatrix_PARAM(name) \
    img_t name; \
    img_t c_ ## name, *p_ ## name = NULL; \
    if( _ ## name.needed() ) \
    { \
        _ ## name cvSetMat(3, 3, type); \
        name = _ ## name; \
        c_ ## name = name; p_ ## name = &c_ ## name; \
    }

    CC_decomposeProjectionMatrix_PARAM(rotMatrixX);
    CC_decomposeProjectionMatrix_PARAM(rotMatrixY);
    CC_decomposeProjectionMatrix_PARAM(rotMatrixZ);
#undef CC_decomposeProjectionMatrix_PARAM

    if( _eulerAngles.needed() )
    {
        _eulerAngles cvSetMat(3, 1, CC_64F, -1, true);
        p_eulerAngles = _eulerAngles.ptr<CPoint3D64f>();
    }

    cvDecomposeProjectionMatrix(&c_projMatrix, &c_cameraMatrix, &c_rotMatrix,
                                &c_transVect, p_rotMatrixX, p_rotMatrixY,
                                p_rotMatrixZ, p_eulerAngles);
}


namespace cv
{

static void adjust3rdMatrix(InputArrayOfArrays _imgpt1_0,
                            InputArrayOfArrays _imgpt3_0,
                            const img_t& cameraMatrix1, const img_t& distCoeffs1,
                            const img_t& cameraMatrix3, const img_t& distCoeffs3,
                            const img_t& R1, const img_t& R3, const img_t& P1, img_t& P3 )
{
    size_t n1 = _imgpt1_0 CC_MAT_TOTAL(), n3 = _imgpt3_0 CC_MAT_TOTAL();
    std::vector<CPoint2f> imgpt1, imgpt3;

    for( int i = 0; i < (int)MIN(n1, n3); i++ )
    {
        img_t pt1 = _imgpt1_0.getMat(i), pt3 = _imgpt3_0.getMat(i);
        int ni1 = pt1 cvCheckVector(2, CC_32F), ni3 = pt3 cvCheckVector(2, CC_32F);
        CC_Assert( ni1 > 0 && ni1 == ni3 );
        const CPoint2f* pt1data = pt1(CPoint2f*)(->tt.data);
        const CPoint2f* pt3data = pt3(CPoint2f*)(->tt.data);
        std::copy(pt1data, pt1data + ni1, std::back_inserter(imgpt1));
        std::copy(pt3data, pt3data + ni3, std::back_inserter(imgpt3));
    }

    undistortPoints(imgpt1, imgpt1, cameraMatrix1, distCoeffs1, R1, P1);
    undistortPoints(imgpt3, imgpt3, cameraMatrix3, distCoeffs3, R3, P3);

    double y1_ = 0, y2_ = 0, y1y1_ = 0, y1y2_ = 0;
    size_t n = imgpt1.size();

    for( size_t i = 0; i < n; i++ )
    {
        double y1 = imgpt3[i].y, y2 = imgpt1[i].y;

        y1_ += y1; y2_ += y2;
        y1y1_ += y1*y1; y1y2_ += y1*y2;
    }

    y1_ /= n;
    y2_ /= n;
    y1y1_ /= n;
    y1y2_ /= n;

    double a = (y1y2_ - y1_*y2_)/(y1y1_ - y1_*y1_);
    double b = y2_ - a*y1_;

    P3.at<double>(0,0) *= a;
    P3.at<double>(1,1) *= a;
    P3.at<double>(0,2) = P3.at<double>(0,2)*a;
    P3.at<double>(1,2) = P3.at<double>(1,2)*a + b;
    P3.at<double>(0,3) *= a;
    P3.at<double>(1,3) *= a;
}

}

float rectify3Collinear( const img_t* _cameraMatrix1, const img_t* _distCoeffs1,
                   const img_t* _cameraMatrix2, const img_t* _distCoeffs2,
                   const img_t* _cameraMatrix3, const img_t* _distCoeffs3,
                   InputArrayOfArrays _imgpt1,
                   InputArrayOfArrays _imgpt3,
                   CSize imageSize, const img_t* _Rmat12, const img_t* _Tmat12,
                   const img_t* _Rmat13, const img_t* _Tmat13,
                   img_t* _Rmat1, img_t* _Rmat2, img_t* _Rmat3,
                   img_t* _Pmat1, img_t* _Pmat2, img_t* _Pmat3,
                   img_t* _Qmat,
                   double alpha, CSize newImgSize,
                   Rect* roi1, Rect* roi2, int flags )
{
    // first, rectify the 1-2 stereo pair
    stereoRectify( _cameraMatrix1, _distCoeffs1, _cameraMatrix2, _distCoeffs2,
                   imageSize, _Rmat12, _Tmat12, _Rmat1, _Rmat2, _Pmat1, _Pmat2, _Qmat,
                   flags, alpha, newImgSize, roi1, roi2 );

    img_t R12 = _Rmat12, R13 = _Rmat13, T12 = _Tmat12, T13 = _Tmat13;

    _Rmat3 cvSetMat(3, 3, CC_64F);
    _Pmat3 cvSetMat(3, 4, CC_64F);

    img_t P1 = _Pmat1, P2 = _Pmat2;
    img_t R3 = _Rmat3, P3 = _Pmat3;

    // recompute rectification transforms for cameras 1 & 2.
    img_t om, r_r, r_r13;

    if( R13.size() != CSize(3,3) )
        Rodrigues(R13, r_r13);
    else
        R13.copyTo(r_r13);

    if( R12.size() == CSize(3,3) )
        Rodrigues(R12, om);
    else
        R12.copyTo(om);

    om *= -0.5;
    Rodrigues(om, r_r); // rotate cameras to same orientation by averaging
    Mat_<double> t12 = r_r * T12;

    int idx = fabs(t12(0,0)) > fabs(t12(1,0)) ? 0 : 1;
    double c = t12(idx,0), nt = norm(t12, CC_L2);
    Mat_<double> uu = Mat_<double>::zeros(3,1);
    uu(idx, 0) = c > 0 ? 1 : -1;

    // calculate global Z rotation
    Mat_<double> ww = t12.cross(uu), wR;
    double nw = norm(ww, CC_L2);
    ww *= acos(fabs(c)/nt)/nw;
    Rodrigues(ww, wR);

    // now rotate camera 3 to make its optical axis parallel to cameras 1 and 2.
    R3 = wR*r_r.t()*r_r13.t();
    Mat_<double> t13 = R3 * T13;

    P2.copyTo(P3);
    img_t t = P3.col(3);
    t13.copyTo(t);
    P3.at<double>(0,3) *= P3.at<double>(0,0);
    P3.at<double>(1,3) *= P3.at<double>(1,1);

    if( !_imgpt1.empty() && _imgpt3.empty() )
        adjust3rdMatrix(_imgpt1, _imgpt3, _cameraMatrix1, _distCoeffs1,
                        _cameraMatrix3, _distCoeffs3, _Rmat1, R3, P1, P3);

    return (float)((P3.at<double>(idx,3)/P3.at<double>(idx,idx))/
                   (P2.at<double>(idx,3)/P2.at<double>(idx,idx)));
}




#endif
