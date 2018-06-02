
#undef _L
#undef _U

/*
    This is stright forward port v2 of Matlab calibration engine by Jean-Yves Bouguet
    that is (in a large extent) based on the paper:
    Z. Zhang. "A flexible new technique for camera calibration".
    IEEE Transactions on Pattern Analysis and Machine Intelligence, 22(11):1330-1334, 2000.

    The 1st initial port was done by Valery Mosyagin.
*/

static void
icvGaussNewton(const img_t* J, const img_t* err, img_t* delta,
                img_t* JtJ=0, img_t* JtErr=0, img_t* JtJW=0, img_t* JtJV=0)
{
    int i = 0;
    img_t im[5] = {0};
    img_t* _temp_JtJ = im+i++;
    img_t* _temp_JtErr = im+i++;
    img_t* _temp_JtJW = im+i++;
    img_t* _temp_JtJV = im+i++;
    
    CC_FUNCNAME("icvGaussNewton");

    __BEGIN__;

    if(!JtJ)
    {
        CC_CALL(cvCreateMat(_temp_JtJ, J->cols, J->cols, J->tid));
        JtJ = _temp_JtJ;
    }

    if(!JtErr)
    {
        CC_CALL(cvCreateMat(_temp_JtErr, J->cols, 1, J->tid));
        JtErr = _temp_JtErr;
    }

    if(!JtJW)
    {
        CC_CALL(cvCreateMat(_temp_JtJW, J->cols, 1, J->tid));
        JtJW = _temp_JtJW;
    }

    if(!JtJV)
    {
        CC_CALL(cvCreateMat(_temp_JtJV, J->cols, J->cols, J->tid));
        JtJV = _temp_JtJV;
    }

    cvMulTransposed(J, JtJ, 1);
    cvGEMM(J, err, 1, 0, 0, JtErr, CC_GEMM_A_T);
    cvSVD(JtJ, JtJW, 0, JtJV, CC_SVD_MODIFY_A + CC_SVD_V_T);
    cvSVBkSb(JtJW, JtJV, JtJV, JtErr, delta, CC_SVD_U_T + CC_SVD_V_T);

    __END__;

    imfrees2(im);
}


/*static double calc_repr_err(const double* object_points, int o_step,
                             const double* image_points,
                             const double* h, int count)
{
    double err = 0;
    for(int i = 0; i < count; i++)
    {
        double X = object_points[i*o_step], Y = object_points[i*o_step + 1];
        double x0 = image_points[i*2], y0 = image_points[i*2 + 1];
        double d = 1./(h[6]*X + h[7]*Y + h[8]);
        double x = (h[0]*X + h[1]*Y + h[2])*d;
        double y = (h[3]*X + h[4]*Y + h[5])*d;
        err += fabs(x - x0) + fabs(y - y0);
    }
    return err;
}*/


// finds perspective transformation H between the object plane and image plane,
// so that (sxi,syi,s) ~ H*(Xi,Yi,1)
CC_IMPL void
cvFindHomography(const img_t* object_points, const img_t* image_points, img_t* __H)
{
    IM_BEGIN(im, 10);
    
    CC_FUNCNAME("cvFindHomography");

    __BEGIN__;

    int h_type;
    int i, k, count, count2;
    CPoint2D64f *m, *M;
    CPoint2D64f cm = {0,0}, sm = {0,0};
    double inv_Hnorm[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };
    double H[9];
    img_t _inv_Hnorm = cvMat1(3, 3, CC_64F, 1, inv_Hnorm);
    img_t _H = cvMat1(3, 3, CC_64F, 1, H);
    double LtL[9*9], LW[9], LV[9*9];
    img_t* _Lp;
    double* L;
    img_t _LtL = cvMat1(9, 9, CC_64F, 1, LtL);
    img_t _LW = cvMat1(9, 1, CC_64F, 1, LW);
    img_t _LV = cvMat1(9, 9, CC_64F, 1, LV);
    img_t _Hrem = cvMat1(3, 3, CC_64F, 1, LV + 8*9);

    h_type = CC_MAT_DEPTH(__H);
    if(h_type != CC_32F && h_type != CC_64F)
        CC_ERROR(CC_StsUnsupportedFormat, "Homography matrix must have 32fC1 or 64fC1 type");
    if(__H->rows != 3 || __H->cols != 3)
        CC_ERROR(CC_StsBadSize, "Homography matrix must be 3x3");

    count = MAX(image_points->cols, image_points->rows);
    count2 = MAX(object_points->cols, object_points->rows);
    if(count != count2)
        CC_ERROR(CC_StsUnmatchedSizes, "Numbers of image and object points do not match");

    img_t* _m = cvSetMat(im++, 1, count, CC_64F, 2);
    CC_CALL(cvConvertPointsHomogenious(image_points, _m));
    m = (CPoint2D64f*)_m->tt.data;
    
    img_t* _M = cvSetMat(im++, 1, count, CC_64F, 2);
    CC_CALL(cvConvertPointsHomogenious(object_points, _M));
    M = (CPoint2D64f*)_M->tt.data;

    // calculate the normalization transformation Hnorm.
    for(i = 0; i < count; i++)
        cm.x += m[i].x, cm.y += m[i].y;
   
    cm.x /= count; cm.y /= count;

    for(i = 0; i < count; i++)
    {
        double x = m[i].x - cm.x;
        double y = m[i].y - cm.y;
        sm.x += fabs(x); sm.y += fabs(y);
    }

    sm.x /= count; sm.y /= count;
    inv_Hnorm[0] = sm.x;
    inv_Hnorm[4] = sm.y;
    inv_Hnorm[2] = cm.x;
    inv_Hnorm[5] = cm.y;
    sm.x = 1./sm.x;
    sm.y = 1./sm.y;

    img_t* _L = im++;
    CC_CALL(_Lp = cvCreateMat(_L, 2*count, 9, CC_64F, 1));
    L = _L->tt.db;

    for(i = 0; i < count; i++, L += 18)
    {
        double x = -(m[i].x - cm.x)*sm.x, y = -(m[i].y - cm.y)*sm.y;
        L[0] = L[9 + 3] = M[i].x;
        L[1] = L[9 + 4] = M[i].y;
        L[2] = L[9 + 5] = 1;
        L[9 + 0] = L[9 + 1] = L[9 + 2] = L[3] = L[4] = L[5] = 0;
        L[6] = x*M[i].x;
        L[7] = x*M[i].y;
        L[8] = x;
        L[9 + 6] = y*M[i].x;
        L[9 + 7] = y*M[i].y;
        L[9 + 8] = y;
    }

    if(count > 4)
    {
        cvMulTransposed(_L, &_LtL, 1);
        _Lp = &_LtL;
    }

    _LW.rows = MIN(count*2, 9);
    cvSVD(_Lp, &_LW, 0, &_LV, CC_SVD_MODIFY_A + CC_SVD_V_T);
    cvScale(&_Hrem, &_Hrem, 1./_Hrem.tt.db[8]);
    cvMatMul(&_inv_Hnorm, &_Hrem, &_H);

    if(count > 4)
    {
        // reuse the available storage for jacobian and other vars
        img_t _J = cvMat1(2*count, 8, CC_64F, 1, _L->tt.db);
        img_t _err = cvMat1(2*count, 1, CC_64F, 1, _L->tt.db + 2*count*8);
        img_t _JtJ = cvMat1(8, 8, CC_64F, 1, LtL);
        img_t _JtErr = cvMat1(8, 1, CC_64F, 1, LtL + 8*8);
        img_t _JtJW = cvMat1(8, 1, CC_64F, 1, LW);
        img_t _JtJV = cvMat1(8, 8, CC_64F, 1, LV);
        img_t _Hinnov = cvMat1(8, 1, CC_64F, 1, LV + 8*8);

        for(k = 0; k < 10; k++)
        {
            double* J = _J.tt.db, *err = _err.tt.db;
            for(i = 0; i < count; i++, J += 16, err += 2)
            {
                double di = 1./(H[6]*M[i].x + H[7]*M[i].y + 1.);
                double _xi = (H[0]*M[i].x + H[1]*M[i].y + H[2])*di;
                double _yi = (H[3]*M[i].x + H[4]*M[i].y + H[5])*di;
                err[0] = m[i].x - _xi;
                err[1] = m[i].y - _yi;
                J[0] = M[i].x*di;
                J[1] = M[i].y*di;
                J[2] = di;
                J[8+3] = M[i].x;
                J[8+4] = M[i].y;
                J[8+5] = di;
                J[6] = -J[0]*_xi;
                J[7] = -J[1]*_xi;
                J[8+6] = -J[8+3]*_yi;
                J[8+7] = -J[8+4]*_yi;
                J[3] = J[4] = J[5] = J[8+0] = J[8+1] = J[8+2] = 0.;
            }

            icvGaussNewton(&_J, &_err, &_Hinnov, &_JtJ, &_JtErr, &_JtJW, &_JtJV);

            for(i = 0; i < 8; i++)
                H[i] += _Hinnov.tt.db[i];
        }
    }

    cvConvert(&_H, __H);

    __END__;

    IM_END(im);
}

CC_IMPL int
cvRodrigues2(const img_t* src, img_t* dst, img_t* jacobian CC_DEFAULT(NULL))
{
    int result = 0;
    
    CC_FUNCNAME("cvRogrigues2");

    __BEGIN__;

    TypeId type;
    int elem_size;
    int i, k;
    double J[27];
    img_t _J = cvMat1(3, 9, CC_64F, 1, J);

    type = CC_MAT_DEPTH(src);
    elem_size = CC_TYPE_SIZE(type);

    if(type != CC_32F && type != CC_64F)
        CC_ERROR(CC_StsUnsupportedFormat, "The matrices must have 32f or 64f data type");

    if(!CC_ARE_DEPTHS_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "All the matrices must have the same data type");

    if(jacobian)
    {
        if(!CC_ARE_DEPTHS_EQ(src, jacobian) || CC_MAT_CN(jacobian) != 1)
            CC_ERROR(CC_StsUnmatchedFormats, "Jacobian must have 32fC1 or 64fC1 datatype");

        if((jacobian->rows != 9 || jacobian->cols != 3) &&
            (jacobian->rows != 3 || jacobian->cols != 9))
            CC_ERROR(CC_StsBadSize, "Jacobian must be 3x9 or 9x3");
    }

    if(src->cols == 1 || src->rows == 1)
    {
        double rx, ry, rz, theta;
        int step = src->rows > 1 ? src->step / elem_size : 1;

        if(src->rows + src->cols*CC_MAT_CN(src) - 1 != 3)
            CC_ERROR(CC_StsBadSize, "Input matrix must be 1x3, 3x1 or 3x3");

        if(dst->rows != 3 || dst->cols != 3 || CC_MAT_CN(dst) != 1)
            CC_ERROR(CC_StsBadSize, "Output matrix must be 3x3, single-channel floating point matrix");

        if(type == CC_32F)
        {
            rx = src->tt.fl[0];
            ry = src->tt.fl[step];
            rz = src->tt.fl[step*2];
        }
        else
        {
            rx = src->tt.db[0];
            ry = src->tt.db[step];
            rz = src->tt.db[step*2];
        }
        theta = sqrt(rx*rx + ry*ry + rz*rz);

        if(theta < DBL_EPSILON)
        {
            cvSetIdentity(dst);

            if(jacobian)
            {
                memset(J, 0, sizeof(J));
                J[5] = J[15] = J[19] = -1;
                J[7] = J[11] = J[21] = 1;
            }
        }
        else
        {
            const double I[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
            
            double c = cos(theta);
            double s = sin(theta);
            double c1 = 1. - c;
            double itheta = theta ? 1./theta : 0.;

            rx *= itheta; ry *= itheta; rz *= itheta;

            double rrt[] = { rx*rx, rx*ry, rx*rz, rx*ry, ry*ry, ry*rz, rx*rz, ry*rz, rz*rz };
            double _r_x_[] = { 0, -rz, ry, rz, 0, -rx, -ry, rx, 0 };
            double R[9];
            img_t _R = cvMat1(3, 3, CC_64F, 1, R);

            // R = cos(theta)*I + (1 - cos(theta))*r*rT + sin(theta)*[r_x]
            // where [r_x] is [0 -rz ry; rz 0 -rx; -ry rx 0]
            for(k = 0; k < 9; k++)
                R[k] = c*I[k] + c1*rrt[k] + s*_r_x_[k];

            cvConvert(&_R, dst);
            
            if(jacobian)
            {
                double drrt[] = { rx+rx, ry, rz, ry, 0, 0, rz, 0, 0,
                                  0, rx, 0, rx, ry+ry, rz, 0, rz, 0,
                                  0, 0, rx, 0, 0, ry, rx, ry, rz+rz };
                double d_r_x_[] = { 0, 0, 0, 0, 0, -1, 0, 1, 0,
                                    0, 0, 1, 0, 0, 0, -1, 0, 0,
                                    0, -1, 0, 1, 0, 0, 0, 0, 0 };
                for(i = 0; i < 3; i++)
                {
                    double ri = i == 0 ? rx : i == 1 ? ry : rz;
                    double a0 = -s*ri, a1 = (s - 2*c1*itheta)*ri, a2 = c1*itheta;
                    double a3 = (c - s*itheta)*ri, a4 = s*itheta;
                    for(k = 0; k < 9; k++)
                        J[i*9+k] = a0*I[k] + a1*rrt[k] + a2*drrt[i*9+k] +
                                   a3*_r_x_[k] + a4*d_r_x_[i*9+k];
                }
            }
        }
    }
    else if(src->cols == 3 && src->rows == 3)
    {
        double R[9], U[9], V[9], W[3], rx, ry, rz;
        img_t _R = cvMat1(3, 3, CC_64F, 1, R);
        img_t _U = cvMat1(3, 3, CC_64F, 1, U);
        img_t _V = cvMat1(3, 3, CC_64F, 1, V);
        img_t _W = cvMat1(3, 1, CC_64F, 1, W);
        double theta, s, c;
        int step = dst->rows > 1 ? dst->step / elem_size : 1;
        
        if((dst->rows != 1 || dst->cols*CC_MAT_CN(dst) != 3) &&
            (dst->rows != 3 || dst->cols != 1 || CC_MAT_CN(dst) != 1))
            CC_ERROR(CC_StsBadSize, "Output matrix must be 1x3 or 3x1");

        cvConvert(src, &_R);
        if(!cvCheckArr(&_R, CC_CHECK_RANGE+CC_CHECK_QUIET, -100, 100))
        {
            cvZeroMat(dst);
            if(jacobian)
                cvZeroMat(jacobian);
            EXIT;
        }
        
        cvSVD(&_R, &_W, &_U, &_V, CC_SVD_MODIFY_A + CC_SVD_U_T + CC_SVD_V_T);
        cvGEMM(&_U, &_V, 1, 0, 0, &_R, CC_GEMM_A_T);
        
        rx = R[7] - R[5];
        ry = R[2] - R[6];
        rz = R[3] - R[1];

        s = sqrt((rx*rx + ry*ry + rz*rz)*0.25);
        c = (R[0] + R[4] + R[8] - 1)*0.5;
        c = c > 1. ? 1. : c < -1. ? -1. : c;
        theta = acos(c);

        if(s < 1e-5)
        {
            double t;

            if(c > 0)
                rx = ry = rz = 0;
            else
            {
                t = (R[0] + 1)*0.5;
                rx = theta*sqrt(MAX(t,0.));
                t = (R[4] + 1)*0.5;
                ry = theta*sqrt(MAX(t,0.))*(R[1] < 0 ? -1. : 1.);
                t = (R[8] + 1)*0.5;
                rz = theta*sqrt(MAX(t,0.))*(R[2] < 0 ? -1. : 1.);
            }

            if(jacobian)
            {
                memset(J, 0, sizeof(J));
                if(c > 0)
                {
                    J[5] = J[15] = J[19] = -0.5;
                    J[7] = J[11] = J[21] = 0.5;
                }
            }
        }
        else
        {
            double vth = 1/(2*s);
            
            if(jacobian)
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
                    vth, 0, 0, rx, 0,
                    0, vth, 0, ry, 0,
                    0, 0, vth, rz, 0,
                    0, 0, 0, 0, 1
                };
                double domegadvar2[] = 
                {
                    theta, 0, 0, rx*vth,
                    0, theta, 0, ry*vth,
                    0, 0, theta, rz*vth
                };
        
                img_t _dvardR = cvMat1(5, 9, CC_64F, 1, dvardR);
                img_t _dvar2dvar = cvMat1(4, 5, CC_64F, 1, dvar2dvar);
                img_t _domegadvar2 = cvMat1(3, 4, CC_64F, 1, domegadvar2);
                double t0[3*5];
                img_t _t0 = cvMat1(3, 5, CC_64F, 1, t0);
        
                cvMatMul(&_domegadvar2, &_dvar2dvar, &_t0);
                cvMatMul(&_t0, &_dvardR, &_J);

                // transpose every row of _J (treat the rows as 3x3 matrices)
                CC_SWAP(J[1], J[3], t); CC_SWAP(J[2], J[6], t); CC_SWAP(J[5], J[7], t);
                CC_SWAP(J[10], J[12], t); CC_SWAP(J[11], J[15], t); CC_SWAP(J[14], J[16], t);
                CC_SWAP(J[19], J[21], t); CC_SWAP(J[20], J[24], t); CC_SWAP(J[23], J[25], t);
            }

            vth *= theta;
            rx *= vth; ry *= vth; rz *= vth;
        }

        if(type == CC_32F)
        {
            dst->tt.fl[0] = (float)rx;
            dst->tt.fl[step] = (float)ry;
            dst->tt.fl[step*2] = (float)rz;
        }
        else
        {
            dst->tt.db[0] = rx;
            dst->tt.db[step] = ry;
            dst->tt.db[step*2] = rz;
        }
    }

    if(jacobian)
    {
        if(type == CC_32F)
        {
            if(jacobian->rows == _J.rows)
                cvConvert(&_J, jacobian);
            else
            {
                float Jf[3*9];
                img_t _Jf = cvMat1(_J.rows, _J.cols, CC_32F, 1, Jf);
                cvConvert(&_J, &_Jf);
                cvTranspose(&_Jf, jacobian);
            }
        }
        else if(jacobian->rows == _J.rows)
            cvCopy(&_J, jacobian);
        else
            cvTranspose(&_J, jacobian);
    }

    result = 1;

    __END__;

    return result;
}

CC_IMPL void
cvProjectPoints2(const img_t* obj_points,
                  const img_t* r_vec,
                  const img_t* t_vec,
                  const img_t* A,
                  const img_t* dist_coeffs,
                  img_t* img_points, img_t* dpdr,
                  img_t* dpdt, img_t* dpdf,
                  img_t* dpdc, img_t* dpdk)
{
    int i = 0;
    IM_BEGIN(im, 16);
    
    CC_FUNCNAME("cvProjectPoints2");

    __BEGIN__;

    int i, j, count;
    int calc_derivatives;
    const CPoint3D64f* M;
    CPoint2D64f* m;
    double r[3], R[9], dRdr[27], t[3], a[9], k[4] = {0,0,0,0}, fx, fy, cx, cy;
    img_t _r, _t, _a = cvMat1(3, 3, CC_64F, 1, a), _k;
    img_t _R = cvMat1(3, 3, CC_64F, 1, R), _dRdr = cvMat1(3, 9, CC_64F, 1, dRdr);
    double *dpdr_p = 0, *dpdt_p = 0, *dpdk_p = 0, *dpdf_p = 0, *dpdc_p = 0;
    int dpdr_step = 0, dpdt_step = 0, dpdk_step = 0, dpdf_step = 0, dpdc_step = 0;

    count = MAX(obj_points->rows, obj_points->cols);

    img_t* _M = im++;
    if(CC_IS_CONT_MAT(obj_points) && CC_MAT_TYPE(obj_points) == CC_64F &&
        (obj_points->rows == 1 && CC_MAT_CN(obj_points) == 3 ||
        obj_points->rows == count && CC_MAT_CN(obj_points)*obj_points->cols == 3)) {
        cvConvertTo(obj_points, _M, CC_64F);
    } else
    {
        CC_CALL(cvSetMat(_M, 1, count, CC_64F, 3));
        CC_CALL(cvConvertPointsHomogenious(obj_points, _M));
    }

    img_t* _m = im++;
    if(CC_IS_CONT_MAT(img_points) && CC_MAT_TYPE(img_points) == CC_64F &&
        (img_points->rows == 1 && CC_MAT_CN(img_points) == 2 ||
        img_points->rows == count && CC_MAT_CN(img_points)*img_points->cols == 2))
        cvConvertTo(img_points, _m, CC_64F);
    else
        cvSetMat(_m, 1, count, CC_64F, 2);

    M = (CPoint3D64f*)_M->tt.db;
    m = (CPoint2D64f*)_m->tt.db;

    if(CC_MAT_TYPE(r_vec) != CC_64F && CC_MAT_TYPE(r_vec) != CC_32F ||
        (r_vec->rows != 1 && r_vec->cols != 1 ||
        r_vec->rows*r_vec->cols*CC_MAT_CN(r_vec) != 3) &&
        (r_vec->rows != 3 && r_vec->cols != 3 || CC_MAT_CN(r_vec) != 1))
        CC_ERROR(CC_StsBadArg, "Rotation must be represented by 1x3 or 3x1 "
                  "floating-point rotation vector, or 3x3 rotation matrix");

    if(r_vec->rows == 3 && r_vec->cols == 3)
    {
        _r = cvMat1(3, 1, CC_64F, 1, r);
        CC_CALL(cvRodrigues2(r_vec, &_r));
        CC_CALL(cvRodrigues2(&_r, &_R, &_dRdr));
        cvCopy(r_vec, &_R);
    }
    else
    {
        _r = cvMat1(r_vec->rows, r_vec->cols, CC_64F, CC_MAT_CN(r_vec), r);
        CC_CALL(cvConvert(r_vec, &_r));
        CC_CALL(cvRodrigues2(&_r, &_R, &_dRdr));
    }

    if(CC_MAT_TYPE(t_vec) != CC_64F && CC_MAT_TYPE(t_vec) != CC_32F ||
        t_vec->rows != 1 && t_vec->cols != 1 ||
        t_vec->rows*t_vec->cols*CC_MAT_CN(t_vec) != 3)
        CC_ERROR(CC_StsBadArg,
            "Translation vector must be 1x3 or 3x1 floating-point vector");

    _t = cvMat1(t_vec->rows, t_vec->cols, CC_64F, CC_MAT_CN(t_vec), t);
    CC_CALL(cvConvert(t_vec, &_t));

    if(CC_MAT_TYPE(A) != CC_64F && CC_MAT_TYPE(A) != CC_32F ||
        A->rows != 3 || A->cols != 3)
        CC_ERROR(CC_StsBadArg, "Instrinsic parameters must be 3x3 floating-point matrix");

    CC_CALL(cvConvert(A, &_a));
    fx = a[0]; fy = a[4];
    cx = a[2]; cy = a[5];

    if(dist_coeffs)
    {
        if( CC_MAT_TYPE(dist_coeffs) != CC_64F &&
            CC_MAT_TYPE(dist_coeffs) != CC_32F ||
            dist_coeffs->rows != 1 && dist_coeffs->cols != 1 ||
            dist_coeffs->rows*dist_coeffs->cols*CC_MAT_CN(dist_coeffs) != 4)
            CC_ERROR(CC_StsBadArg,
                "Distortion coefficients must be 1x4 or 4x1 floating-point vector");

        _k = cvMat1(dist_coeffs->rows, dist_coeffs->cols, CC_64F, CC_MAT_CN(dist_coeffs), k);
        CC_CALL(cvConvert(dist_coeffs, &_k));
    }
    
    img_t* _dpdr = im++;
    if(dpdr)
    {
        if( CC_MAT_TYPE(dpdr) != CC_32F &&
            CC_MAT_TYPE(dpdr) != CC_64F ||
            dpdr->rows != count*2 || dpdr->cols != 3)
            CC_ERROR(CC_StsBadArg, "dp/drot must be 2Nx3 floating-point matrix");

        if(CC_MAT_TYPE(dpdr) == CC_64F && 1==CC_MAT_CN(dpdr))
            cvConvertTo(dpdr, _dpdr, CC_64F);
        else
            cvSetMat(_dpdr, 2*count, 3, CC_64F);
        dpdr_p = _dpdr->tt.db;
        dpdr_step = _dpdr->step/sizeof(dpdr_p[0]);
    }

    img_t* _dpdt = im++;
    if(dpdt)
    {
        if(!(dpdt) ||
            CC_MAT_TYPE(dpdt) != CC_32F &&
            CC_MAT_TYPE(dpdt) != CC_64F ||
            dpdt->rows != count*2 || dpdt->cols != 3)
            CC_ERROR(CC_StsBadArg, "dp/dT must be 2Nx3 floating-point matrix");

        if(CC_MAT_TYPE(dpdt) == CC_64F && 1==CC_MAT_CN(dpdt))
            cvConvertTo(dpdt, _dpdt, CC_64F);
        else
            cvSetMat(_dpdt, 2*count, 3, CC_64F, 1);
        dpdt_p = _dpdt->tt.db;
        dpdt_step = _dpdt->step/sizeof(dpdt_p[0]);
    }

    img_t* _dpdf = im++;
    if(dpdf)
    {
        if(!(dpdf) ||
            CC_MAT_TYPE(dpdf) != CC_32F && CC_MAT_TYPE(dpdf) != CC_64F ||
            dpdf->rows != count*2 || dpdf->cols != 2)
            CC_ERROR(CC_StsBadArg, "dp/df must be 2Nx2 floating-point matrix");

        if(CC_MAT_TYPE(dpdf) == CC_64F)
            cvConvertTo(dpdf, _dpdf, CC_64F);
        else
            cvSetMat(_dpdf, 2*count, 2, CC_64F, 1);
        dpdf_p = _dpdf->tt.db;
        dpdf_step = _dpdf->step/sizeof(dpdf_p[0]);
    }

    
    img_t* _dpdc = im++;
    if(dpdc)
    {
        if(!(dpdc) ||
            CC_MAT_TYPE(dpdc) != CC_32F && CC_MAT_TYPE(dpdc) != CC_64F ||
            dpdc->rows != count*2 || dpdc->cols != 2)
            CC_ERROR(CC_StsBadArg, "dp/dc must be 2Nx2 floating-point matrix");

        if(CC_MAT_TYPE(dpdc) == CC_64F)
            cvConvertTo(dpdc, _dpdc, CC_64F);
        else
            cvSetMat(_dpdc, 2*count, 2, CC_64F, 1);
        dpdc_p = _dpdc->tt.db;
        dpdc_step = _dpdc->step/sizeof(dpdc_p[0]);
    }

    img_t* _dpdk = im++;
    if(dpdk)
    {
        if(!(dpdk) ||
            CC_MAT_TYPE(dpdk) != CC_32F && CC_MAT_TYPE(dpdk) != CC_64F ||
            dpdk->rows != count*2 || (dpdk->cols != 4 && dpdk->cols != 2))
            CC_ERROR(CC_StsBadArg, "dp/df must be 2Nx4 or 2Nx2 floating-point matrix");

        if(!dist_coeffs)
            CC_ERROR(CC_StsNullPtr, "dist_coeffs is NULL while dpdk is not");

        if(CC_MAT_TYPE(dpdk) == CC_64F)
            cvConvertTo(dpdk, _dpdk, CC_64F);
        else
            cvSetMat(_dpdk, dpdk->rows, dpdk->cols, CC_64F, 1);
        dpdk_p = _dpdk->tt.db;
        dpdk_step = _dpdk->step/sizeof(dpdk_p[0]);
    }

    calc_derivatives = dpdr || dpdt || dpdf || dpdc || dpdk;

    for(i = 0; i < count; i++)
    {
        double X = M[i].x, Y = M[i].y, Z = M[i].z;
        double x = R[0]*X + R[1]*Y + R[2]*Z + t[0];
        double y = R[3]*X + R[4]*Y + R[5]*Z + t[1];
        double z = R[6]*X + R[7]*Y + R[8]*Z + t[2];
        double r2, r4, a1, a2, a3, cdist;
        double xd, yd;

        z = z ? 1./z : 1;
        x *= z; y *= z;

        r2 = x*x + y*y;
        r4 = r2*r2;
        a1 = 2*x*y;
        a2 = r2 + 2*x*x;
        a3 = r2 + 2*y*y;
        cdist = 1 + k[0]*r2 + k[1]*r4;
        xd = x*cdist + k[2]*a1 + k[3]*a2;
        yd = y*cdist + k[2]*a3 + k[3]*a1;

        m[i].x = xd*fx + cx;
        m[i].y = yd*fy + cy;

        if(calc_derivatives)
        {
            if(dpdc_p)
            {
                dpdc_p[0] = 1; dpdc_p[1] = 0;
                dpdc_p[dpdc_step] = 0;
                dpdc_p[dpdc_step+1] = 1;
                dpdc_p += dpdc_step*2;
            }

            if(dpdf_p)
            {
                dpdf_p[0] = xd; dpdf_p[1] = 0;
                dpdf_p[dpdf_step] = 0;
                dpdf_p[dpdf_step+1] = yd;
                dpdf_p += dpdf_step*2;
            }

            if(dpdk_p)
            {
                dpdk_p[0] = fx*x*r2;
                dpdk_p[1] = fx*x*r4;
                dpdk_p[dpdk_step] = fy*y*r2;
                dpdk_p[dpdk_step+1] = fy*y*r4;
                if(_dpdk->cols > 2)
                {
                    dpdk_p[2] = fx*a1;
                    dpdk_p[3] = fx*a2;
                    dpdk_p[dpdk_step+2] = fy*a3;
                    dpdk_p[dpdk_step+3] = fy*a1;
                }
                dpdk_p += dpdk_step*2;
            }

            if(dpdt_p)
            {
                double dxdt[] = { z, 0, -x*z }, dydt[] = { 0, z, -y*z };
                for(j = 0; j < 3; j++)
                {
                    double dr2dt = 2*x*dxdt[j] + 2*y*dydt[j];
                    double dcdist_dt = k[0]*dr2dt + 2*k[1]*r2*dr2dt;
                    double da1dt = 2*(x*dydt[j] + y*dxdt[j]);
                    double dmxdt = fx*(dxdt[j]*cdist + x*dcdist_dt +
                                k[2]*da1dt + k[3]*(dr2dt + 2*x*dxdt[j]));
                    double dmydt = fy*(dydt[j]*cdist + y*dcdist_dt +
                                k[2]*(dr2dt + 2*y*dydt[j]) + k[3]*da1dt);
                    dpdt_p[j] = dmxdt;
                    dpdt_p[dpdt_step+j] = dmydt;
                }
                dpdt_p += dpdt_step*2;
            }

            if(dpdr_p)
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
                for(j = 0; j < 3; j++)
                {
                    double dxdr = z*(dx0dr[j] - x*dz0dr[j]);
                    double dydr = z*(dy0dr[j] - y*dz0dr[j]);
                    double dr2dr = 2*x*dxdr + 2*y*dydr;
                    double dcdist_dr = k[0]*dr2dr + 2*k[1]*r2*dr2dr;
                    double da1dr = 2*(x*dydr + y*dxdr);
                    double dmxdr = fx*(dxdr*cdist + x*dcdist_dr +
                                k[2]*da1dr + k[3]*(dr2dr + 2*x*dxdr));
                    double dmydr = fy*(dydr*cdist + y*dcdist_dr +
                                k[2]*(dr2dr + 2*y*dydr) + k[3]*da1dr);
                    dpdr_p[j] = dmxdr;
                    dpdr_p[dpdr_step+j] = dmydr;
                }
                dpdr_p += dpdr_step*2;
            }
        }
    }

    if(_m != img_points)
        cvConvertPointsHomogenious(_m, img_points);
    if(dpdr)
        cvConvert(_dpdr, dpdr);
    if(dpdt)
        cvConvert(_dpdt, dpdt);
    if(dpdf)
        cvConvert(_dpdf, dpdf);
    if(dpdc)
        cvConvert(_dpdc, dpdc);
    if(dpdk)
        cvConvert(_dpdk, dpdk);

    __END__;

    IM_END(im);
}

CC_IMPL void
cvFindExtrinsicCameraParams2a(const img_t* obj_points,
                  const img_t* img_points, const img_t* A,
                  const img_t* dist_coeffs,
                  img_t* r_vec, img_t* t_vec, int use_extrinsic_guess CC_DEFAULT(0))
{
    const int max_iter = 20;
    int i = 0;
    img_t im[10] = {0};
    img_t *_M = im+i++;
    img_t *_Mxy = im+i++;
    img_t *_m = im+i++;
    img_t *_mn = im+i++;
    img_t *_L = im+i++;
    img_t *_J = im+i++;
    
    CC_FUNCNAME("cvFindExtrinsicCameraParams2");

    __BEGIN__;

    int j, count;
    double a[9], k[4] = { 0, 0, 0, 0 }, R[9], ifx, ify, cx, cy;
    double Mc[3] = {0, 0, 0}, MM[9], U[9], V[9], W[3];
    double JtJ[6*6], JtErr[6], JtJW[6], JtJV[6*6], delta[6], param[6];
    CPoint3D64f* M = 0;
    CPoint2D64f *m = 0, *mn = 0;
    img_t _a = cvMat1(3, 3, CC_64F, 1, a);
    img_t _R = cvMat1(3, 3, CC_64F, 1, R);
    img_t _r = cvMat1(3, 1, CC_64F, 1, param);
    img_t _t = cvMat1(3, 1, CC_64F, 1, param + 3);
    img_t _Mc = cvMat1(1, 3, CC_64F, 1, Mc);
    img_t _MM = cvMat1(3, 3, CC_64F, 1, MM);
    img_t _U = cvMat1(3, 3, CC_64F, 1, U);
    img_t _V = cvMat1(3, 3, CC_64F, 1, V);
    img_t _W = cvMat1(3, 1, CC_64F, 1, W);
    img_t _JtJ = cvMat1(6, 6, CC_64F, 1, JtJ);
    img_t _JtErr = cvMat1(6, 1, CC_64F, 1, JtErr);
    img_t _JtJW = cvMat1(6, 1, CC_64F, 1, JtJW);
    img_t _JtJV = cvMat1(6, 6, CC_64F, 1, JtJV);
    img_t _delta = cvMat1(6, 1, CC_64F, 1, delta);
    img_t _param = cvMat1(6, 1, CC_64F, 1, param);
    img_t _dpdr, _dpdt;

    if(!(obj_points) || !(img_points) || !(A) || !(r_vec) || !(t_vec))
        CC_ERROR(CC_StsBadArg, "One of required arguments is not a valid matrix");

    count = MAX(obj_points->cols, obj_points->rows);
    CC_CALL(cvCreateMat(_M, 1, count, CC_64F, 3));
    CC_CALL(cvCreateMat(_Mxy, 1, count, CC_64F, 2));
    CC_CALL(cvCreateMat(_m, 1, count, CC_64F, 2));
    CC_CALL(cvCreateMat(_mn, 1, count, CC_64F, 2));
    M = (CPoint3D64f*)_M->tt.db;
    m = (CPoint2D64f*)_m->tt.db;
    mn = (CPoint2D64f*)_mn->tt.db;

    CC_CALL(cvConvertPointsHomogenious(obj_points, _M));
    CC_CALL(cvConvertPointsHomogenious(img_points, _m));
    CC_CALL(cvConvert(A, &_a));

    if(dist_coeffs)
    {
        img_t _k;
        if(!(dist_coeffs) ||
            CC_MAT_TYPE(dist_coeffs) != CC_64F &&
            CC_MAT_TYPE(dist_coeffs) != CC_32F ||
            dist_coeffs->rows != 1 && dist_coeffs->cols != 1 ||
            dist_coeffs->rows*dist_coeffs->cols*CC_MAT_CN(dist_coeffs) != 4)
            CC_ERROR(CC_StsBadArg,
                "Distortion coefficients must be 1x4 or 4x1 floating-point vector");

        _k = cvMat1(dist_coeffs->rows, dist_coeffs->cols, CC_64F, CC_MAT_CN(dist_coeffs), k);
        CC_CALL(cvConvert(dist_coeffs, &_k));
    }

    if(CC_MAT_TYPE(r_vec) != CC_64F && CC_MAT_TYPE(r_vec) != CC_32F ||
        r_vec->rows != 1 && r_vec->cols != 1 ||
        r_vec->rows*r_vec->cols*CC_MAT_CN(r_vec) != 3)
        CC_ERROR(CC_StsBadArg, "Rotation vector must be 1x3 or 3x1 floating-point vector");

    if(CC_MAT_TYPE(t_vec) != CC_64F && CC_MAT_TYPE(t_vec) != CC_32F ||
        t_vec->rows != 1 && t_vec->cols != 1 ||
        t_vec->rows*t_vec->cols*CC_MAT_CN(t_vec) != 3)
        CC_ERROR(CC_StsBadArg,
            "Translation vector must be 1x3 or 3x1 floating-point vector");

    ifx = 1./a[0]; ify = 1./a[4];
    cx = a[2]; cy = a[5];

    // normalize image points
    // (unapply the intrinsic matrix transformation and distortion)
    for(i = 0; i < count; i++)
    {
        double x = (m[i].x - cx)*ifx, y = (m[i].y - cy)*ify, x0 = x, y0 = y;

        // compensate distortion iteratively
        if(dist_coeffs)
            for(j = 0; j < 5; j++)
            {
                double r2 = x*x + y*y;
                double icdist = 1./(1 + k[0]*r2 + k[1]*r2*r2);
                double delta_x = 2*k[2]*x*y + k[3]*(r2 + 2*x*x);
                double delta_y = k[2]*(r2 + 2*y*y) + 2*k[3]*x*y;
                x = (x0 - delta_x)*icdist;
                y = (y0 - delta_y)*icdist;
            }
        mn[i].x = x; mn[i].y = y;

        // calc mean(M)
        Mc[0] += M[i].x;
        Mc[1] += M[i].y;
        Mc[2] += M[i].z;
    }

    Mc[0] /= count;
    Mc[1] /= count;
    Mc[2] /= count;

    cvReshape(_M, _M, 1, count);
    cvMulTransposed(_M, &_MM, 1, &_Mc);
    cvSVD(&_MM, &_W, 0, &_V, CC_SVD_MODIFY_A + CC_SVD_V_T);

    // initialize extrinsic parameters
    if(W[2]/W[1] < 1e-3 || count < 4)
    {
        // a planar structure case (all M's lie in the same plane)
        double tt[3], h[9], h1_norm, h2_norm;
        img_t* R_transform = &_V;
        img_t T_transform = cvMat1(3, 1, CC_64F, 1, tt);
        img_t _H = cvMat1(3, 3, CC_64F, 1, h);
        img_t _h1, _h2, _h3;

        if(V[2]*V[2] + V[5]*V[5] < 1e-10)
            cvSetIdentity(R_transform);

        if(cvDet(R_transform) < 0)
            cvScale(R_transform, R_transform, -1);

        cvGEMM(R_transform, &_Mc, -1, 0, 0, &T_transform, CC_GEMM_B_T);

        for(i = 0; i < count; i++)
        {
            const double* Rp = R_transform->tt.db;
            const double* Tp = T_transform.tt.db;
            const double* src = _M->tt.db + i*3;
            double* dst = _Mxy->tt.db + i*2;

            dst[0] = Rp[0]*src[0] + Rp[1]*src[1] + Rp[2]*src[2] + Tp[0];
            dst[1] = Rp[3]*src[0] + Rp[4]*src[1] + Rp[5]*src[2] + Tp[1];
        }

        cvFindHomography(_Mxy, _mn, &_H);

        cvGetCol(&_H, &_h1, 0);
        _h2 = _h1; _h2.tt.db++;
        _h3 = _h2; _h3.tt.db++;
        h1_norm = sqrt(h[0]*h[0] + h[3]*h[3] + h[6]*h[6]);
        h2_norm = sqrt(h[1]*h[1] + h[4]*h[4] + h[7]*h[7]);

        cvScale(&_h1, &_h1, 1./h1_norm);
        cvScale(&_h2, &_h2, 1./h2_norm);
        cvScale(&_h3, &_t, 2./(h1_norm + h2_norm));
        cvCrossProduct(&_h1, &_h2, &_h3);

        cvRodrigues2(&_H, &_r);
        cvRodrigues2(&_r, &_H);
        cvMatMulAdd(&_H, &T_transform, &_t, &_t);
        cvMatMul(&_H, R_transform, &_R);
        cvRodrigues2(&_R, &_r);
    }
    else
    {
        // non-planar structure. Use DLT method
        double* L;
        double LL[12*12], LW[12], LV[12*12], sc;
        img_t _LL = cvMat1(12, 12, CC_64F, 1, LL);
        img_t _LW = cvMat1(12, 1, CC_64F, 1, LW);
        img_t _LV = cvMat1(12, 12, CC_64F, 1, LV);
        img_t _RRt, _RR, _tt;

        CC_CALL(cvCreateMat(_L, 2*count, 12, CC_64F));
        L = _L->tt.db;

        for(i = 0; i < count; i++, L += 24)
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

        cvMulTransposed(_L, &_LL, 1);
        cvSVD(&_LL, &_LW, 0, &_LV, CC_SVD_MODIFY_A + CC_SVD_V_T);
        _RRt = cvMat1(3, 4, CC_64F, 1, LV + 11*12);
        cvGetCols(&_RRt, &_RR, 0, 3);
        cvGetCol(&_RRt, &_tt, 3);
        if(cvDet(&_RR) < 0)
            cvScale(&_RRt, &_RRt, -1);
        sc = cvNorm(&_RR);
        cvSVD(&_RR, &_W, &_U, &_V, CC_SVD_MODIFY_A + CC_SVD_U_T + CC_SVD_V_T);
        cvGEMM(&_U, &_V, 1, 0, 0, &_R, CC_GEMM_A_T);
        cvScale(&_tt, &_t, cvNorm(&_R)/sc);
        cvRodrigues2(&_R, &_r);
        imfree(_L);
    }

    CC_CALL(cvCreateMat(_J, 2*count, 6, CC_64F, 1));
    cvGetCols(_J, &_dpdr, 0, 3);
    cvGetCols(_J, &_dpdt, 3, 6);

    // refine extrinsic parameters using iterative algorithm
    for(i = 0; i < max_iter; i++)
    {
        double n1, n2;
        cvReshape(_mn, _mn, 2, 1);
        cvProjectPoints2(_M, &_r, &_t, &_a, dist_coeffs,
                          _mn, &_dpdr, &_dpdt, 0, 0, 0);
        cvSub(_m, _mn, _mn);
        cvReshape(_mn, _mn, 1, 2*count);

        cvMulTransposed(_J, &_JtJ, 1);
        cvGEMM(_J, _mn, 1, 0, 0, &_JtErr, CC_GEMM_A_T);
        cvSVD(&_JtJ, &_JtJW, 0, &_JtJV, CC_SVD_MODIFY_A + CC_SVD_V_T);
        if(JtJW[5]/JtJW[0] < 1e-12)
            break;
        cvSVBkSb(&_JtJW, &_JtJV, &_JtJV, &_JtErr,
                  &_delta, CC_SVD_U_T + CC_SVD_V_T);
        cvAdd(&_delta, &_param, &_param);
        n1 = cvNorm(&_delta);
        n2 = cvNorm(&_param);
        if(n1/n2 < 1e-10)
            break;
    }

    _r = cvMat1(r_vec->rows, r_vec->cols, CC_64F, CC_MAT_CN(r_vec), param);
    _t = cvMat1(t_vec->rows, t_vec->cols, CC_64F, CC_MAT_CN(t_vec), param + 3);

    cvConvert(&_r, r_vec);
    cvConvert(&_t, t_vec);

    __END__;

    imfrees2(im);
}


#if 0


static void
icvInitIntrinsicParams2D(const img_t* obj_points,
                          const img_t* img_points,
                          const img_t* point_counts,
                          CSize image_size,
                          img_t* intrinsic_matrix,
                          double aspect_ratio)
{
    img_t _A[1] = {0}, _b[1] = {0};
    
    CC_FUNCNAME("icvInitIntrinsicParams2D");

    __BEGIN__;

    int i, j, pos, img_count;
    double a[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };
    double H[9], AtA[4], AtAW[2], AtAV[4], Atb[2], f[2];
    img_t _a = cvMat1(3, 3, CC_64F, 1, a);
    img_t _H = cvMat1(3, 3, CC_64F, 1, H);
    img_t _AtA = cvMat1(2, 2, CC_64F, 1, AtA);
    img_t _AtAW = cvMat1(2, 1, CC_64F, 1, AtAW);
    img_t _AtAV = cvMat1(2, 2, CC_64F, 1, AtAV);
    img_t _Atb = cvMat1(2, 1, CC_64F, 1, Atb);
    img_t _f = cvMat1(2, 1, CC_64F, 1, f);
    int obj_points_cn = CC_MAT_CN(obj_points);
    int img_points_cn = CC_MAT_CN(img_points);

    assert(CC_MAT_TYPE(point_counts) == CC_32S && CC_MAT_CN(point_counts)==1 && 
            CC_IS_MAT_CONT(point_counts));
    img_count = point_counts->rows + point_counts->cols - 1;

    if(!(CC_MAT_TYPE(obj_points) == CC_32F && 3==obj_points_cn) &&
        !(CC_MAT_TYPE(obj_points) == CC_64F && 3==obj_points_cn) ||
        !(CC_MAT_TYPE(img_points) == CC_32F && 2==img_points_cn) &&
        !(CC_MAT_TYPE(img_points) == CC_64F && 2==img_points_cn))
        CC_ERROR(CC_StsUnsupportedFormat, "Both object points and image points must be 2D");

    if(obj_points->rows != 1 || img_points->rows != 1)
        CC_ERROR(CC_StsBadSize, "object points and image points must be a single-row matrices");

    CC_CALL(cvCreateMat(_A, 2*img_count, 2, CC_64F));
    CC_CALL(cvCreateMat(_b, 2*img_count, 1, CC_64F));
    a[2] = (image_size.width - 1)*0.5;
    a[5] = (image_size.height - 1)*0.5;

    // extract vanishing points in order to obtain initial value for the focal length
    for(i = 0, pos = 0; i < img_count; i++)
    {
        double* Ap = _A->tt.db + i*4;
        double* bp = _b->tt.db + i*2;
        int count = point_counts->tt.i[i];
        double h[3], v[3], d1[3], d2[3];
        double n[4] = {0,0,0,0};
        img_t _m, _M;
        cvGetCols(obj_points, &_M, pos, pos + count);
        cvGetCols(img_points, &_m, pos, pos + count);
        pos += count;

        CC_CALL(cvFindHomography(&_M, &_m, &_H));
        
        H[0] -= H[6]*a[2]; H[1] -= H[7]*a[2]; H[2] -= H[8]*a[2];
        H[3] -= H[6]*a[5]; H[4] -= H[7]*a[5]; H[5] -= H[8]*a[5];

        for(j = 0; j < 3; j++)
        {
            double t0 = H[j*3], t1 = H[j*3+1];
            h[j] = t0; v[j] = t1;
            d1[j] = (t0 + t1)*0.5;
            d2[j] = (t0 - t1)*0.5;
            n[0] += t0*t0; n[1] += t1*t1;
            n[2] += d1[j]*d1[j]; n[3] += d2[j]*d2[j];
        }

        for(j = 0; j < 4; j++)
            n[j] = 1./sqrt(n[j]);

        for(j = 0; j < 3; j++)
        {
            h[j] *= n[0]; v[j] *= n[1];
            d1[j] *= n[2]; d2[j] *= n[3];
        }

        Ap[0] = h[0]*v[0]; Ap[1] = h[1]*v[1];
        Ap[2] = d1[0]*d2[0]; Ap[3] = d1[1]*d2[1];
        bp[0] = -h[2]*v[2]; bp[1] = -d1[2]*d2[2];
    }

    // while it is not about gradient descent search,
    // the formula is the same: f = inv(At*A)*At*b
    icvGaussNewton(_A, _b, &_f, &_AtA, &_Atb, &_AtAW, &_AtAV);
    a[0] = sqrt(fabs(1./f[0]));
    a[4] = sqrt(fabs(1./f[1]));
    if(aspect_ratio != 0)
    {
        double tf = (a[0] + a[4])/(aspect_ratio + 1.);
        a[0] = aspect_ratio*tf;
        a[4] = tf;
    }

    cvConvert(&_a, intrinsic_matrix);

    __END__;

    imfree(_A);
    imfree(_b);
}

#define CC_CALIB_USE_INTRINSIC_GUESS  1
#define CC_CALIB_FIX_ASPECT_RATIO     2
#define CC_CALIB_FIX_PRINCIPAL_POINT  4
#define CC_CALIB_ZERO_TANGENT_DIST    8


/* finds intrinsic and extrinsic camera parameters
   from a few views of known calibration pattern */
CC_IMPL void
cvCalibrateCamera2(const img_t* obj_points,
                    const img_t* img_points,
                    const img_t* point_counts,
                    CSize image_size,
                    img_t* A, img_t* dist_coeffs,
                    img_t* r_vecs, img_t* t_vecs,
                    int flags)
{
    double alpha_smooth = 0.4;
    int i = 0;
    img_t im[16] = {0};
    img_t *counts = im+i++;
    img_t *_M = im+i++;
    img_t *_m = im+i++;
    img_t *_Ji = im+i++;
    img_t *_Je = im+i++;
    img_t *_JtJ = im+i++;
    img_t *_JtErr = im+i++;
    img_t *_JtJW = im+i++;
    img_t *_JtJV = im+i++;
    img_t *_param = im+i++;
    img_t *_param_innov = im+i++;
    img_t *_err = im+i++;
    
    CC_FUNCNAME("cvCalibrateCamera2");

    __BEGIN__;

    double a[9];
    img_t _a = cvMat1(3, 3, CC_64F, 1, a), _k;
    img_t _Mi, _mi, _ri, _ti, _part;
    img_t _dpdr, _dpdt, _dpdf, _dpdc, _dpdk;
    img_t _sr_part = cvMat1(1, 3, CC_64F), _st_part = cvMat1(1, 3, CC_64F), _r_part, _t_part;
    int i, j, pos, iter, img_count, count = 0, max_count = 0, total = 0, param_count;
    TypeId r_type, t_type;
    int r_step = 0, t_step = 0, cn, dims;
    int output_r_matrices = 0;
    double aspect_ratio = 0.;

    if(!(obj_points) || !(img_points) ||
        !(point_counts) || !(A) || !(dist_coeffs))
        CC_ERROR(CC_StsBadArg, "One of required vector arguments is not a valid matrix");

    if(image_size.width <= 0 || image_size.height <= 0)
        CC_ERROR(CC_StsOutOfRange, "image width and height must be positive");

    if(!(CC_MAT_TYPE(point_counts) == CC_32S && 1==CC_MAT_CN(point_counts)) ||
        point_counts->rows != 1 && point_counts->cols != 1)
        CC_ERROR(CC_StsUnsupportedFormat,
            "the array of point counters must be 1-dimensional integer vector");

    CC_CALL(cvCreateMat(counts, point_counts->rows, point_counts->width, CC_32S, 1));
    cvCopy(point_counts, counts);

    img_count = counts->rows + counts->cols - 1;

    if(r_vecs)
    {
        r_type = CC_MAT_DEPTH(r_vecs);
        r_step = r_vecs->rows == 1 ? 3*CC_TYPE_SIZE(r_type) : r_vecs->step;
        cn = CC_MAT_CN(r_vecs);
        if(!(r_vecs) || r_type != CC_32F && r_type != CC_64F ||
            (r_vecs->rows != img_count || r_vecs->cols*cn != 3 && r_vecs->cols*cn != 9) &&
            (r_vecs->rows != 1 || r_vecs->cols != img_count || cn != 3))
            CC_ERROR(CC_StsBadArg, "the output array of rotation vectors must be 3-channel "
                "1xn or nx1 array or 1-channel nx3 or nx9 array, where n is the number of views");
        output_r_matrices = r_vecs->rows == img_count && r_vecs->cols*cn == 9;
    }

    if(t_vecs)
    {
        t_type = CC_MAT_DEPTH(t_vecs);
        t_step = t_vecs->rows == 1 ? 3*CC_TYPE_SIZE(t_type) : t_vecs->step;
        cn = CC_MAT_CN(t_vecs);
        if(!(t_vecs) || t_type != CC_32F && t_type != CC_64F ||
            (t_vecs->rows != img_count || t_vecs->cols*cn != 3) &&
            (t_vecs->rows != 1 || t_vecs->cols != img_count || cn != 3))
            CC_ERROR(CC_StsBadArg, "the output array of translation vectors must be 3-channel "
                "1xn or nx1 array or 1-channel nx3 array, where n is the number of views");
    }

    if(CC_MAT_TYPE(A) != CC_32F && CC_MAT_TYPE(A) != CC_64F ||
        A->rows != 3 || A->cols != 3)
        CC_ERROR(CC_StsBadArg,
            "Intrinsic parameters must be 3x3 floating-point matrix");

    if(CC_MAT_TYPE(dist_coeffs) != CC_32F &&
        CC_MAT_TYPE(dist_coeffs) != CC_64F ||
        (dist_coeffs->rows != 4 || dist_coeffs->cols != 1) &&
        (dist_coeffs->cols != 4 || dist_coeffs->rows != 1))
        CC_ERROR(CC_StsBadArg,
            "Distortion coefficients must be 4x1 or 1x4 floating-point matrix");

    for(i = 0; i < img_count; i++)
    {
        int temp_count = counts->tt.i[i];
        if(temp_count < 4)
        {
            char buf[100];
            sprintf(buf, "The number of points in the view #%d is <4", i);
            CC_ERROR(CC_StsOutOfRange, buf);
        }
        max_count = MAX(max_count, temp_count);
        total += temp_count;
    }

    dims = CC_MAT_CN(obj_points)*(obj_points->rows == 1 ? 1 : obj_points->cols);

    if(CC_MAT_TYPE(obj_points) != CC_32F &&
        CC_MAT_TYPE(obj_points) != CC_64F ||
        (obj_points->rows != total || dims != 3 && dims != 2) &&
        (obj_points->rows != 1 || obj_points->cols != total || (dims != 3 && dims != 2)))
        CC_ERROR(CC_StsBadArg, "Object points must be 1xn or nx1, 2- or 3-channel matrix, "
                                "or nx3 or nx2 single-channel matrix");

    cn = CC_MAT_CN(img_points);
    if(CC_MAT_TYPE(img_points) != CC_32F &&
        CC_MAT_TYPE(img_points) != CC_64F ||
        (img_points->rows != total || img_points->cols*cn != 2) &&
        (img_points->rows != 1 || img_points->cols != total || cn != 2))
        CC_ERROR(CC_StsBadArg, "Image points must be 1xn or nx1, 2-channel matrix, "
                                "or nx2 single-channel matrix");

    CC_CALL(cvCreateMat(_M, 1, total, CC_64F, 3));
    CC_CALL(cvCreateMat(_m, 1, total, CC_64F, 2));

    CC_CALL(cvConvertPointsHomogenious(obj_points, _M));
    CC_CALL(cvConvertPointsHomogenious(img_points, _m));

    param_count = 8 + img_count*6;
    CC_CALL(cvCreateMat(_param, param_count, 1, CC_64F, 1));
    CC_CALL(cvCreateMat(_param_innov, param_count, 1, CC_64F, 1));
    CC_CALL(cvCreateMat(_JtJ, param_count, param_count, CC_64F, 1));
    CC_CALL(cvCreateMat(_JtErr, param_count, 1, CC_64F, 1));
    CC_CALL(cvCreateMat(_JtJW, param_count, 1, CC_64F, 1));
    CC_CALL(cvCreateMat(_JtJV, param_count, param_count, CC_64F, 1));
    CC_CALL(cvCreateMat(_Ji, max_count*2, 8, CC_64F, 1));
    CC_CALL(cvCreateMat(_Je, max_count*2, 6, CC_64F, 1));
    CC_CALL(cvCreateMat(_err, max_count*2, 1, CC_64F, 1));
    
    cvGetCols(_Je, &_dpdr, 0, 3);
    cvGetCols(_Je, &_dpdt, 3, 6);
    cvGetCols(_Ji, &_dpdf, 0, 2);
    cvGetCols(_Ji, &_dpdc, 2, 4);
    cvGetCols(_Ji, &_dpdk, 4, flags & CC_CALIB_ZERO_TANGENT_DIST ? 6 : 8);
    cvZero(_Ji);

    // 1. initialize intrinsic parameters
    if(flags & CC_CALIB_USE_INTRINSIC_GUESS)
    {
        cvConvert(A, &_a);
        if(a[0] <= 0 || a[4] <= 0)
            CC_ERROR(CC_StsOutOfRange, "Focal length (fx and fy) must be positive");
        if(a[2] < 0 || a[2] >= image_size.width ||
            a[5] < 0 || a[5] >= image_size.height)
            CC_ERROR(CC_StsOutOfRange, "Principal point must be within the image");
        if(fabs(a[1]) > 1e-5)
            CC_ERROR(CC_StsOutOfRange, "Non-zero skew is not supported by the function");
        if(fabs(a[3]) > 1e-5 || fabs(a[6]) > 1e-5 ||
            fabs(a[7]) > 1e-5 || fabs(a[8]-1) > 1e-5)
            CC_ERROR(CC_StsOutOfRange,
                "The intrinsic matrix must have [fx 0 cx; 0 fy cy; 0 0 1] shape");
        a[1] = a[3] = a[6] = a[7] = 0.;
        a[8] = 1.;

        if(flags & CC_CALIB_FIX_ASPECT_RATIO)
            aspect_ratio = a[0]/a[4];
    }
    else
    {
        if(dims == 3)
        {
            double4 mean, sdv;
            cvAvgSdv(_M, mean, sdv);
            if(fabs(mean[2]) > 1e-5 && fabs(mean[2] - 1) > 1e-5 ||
                fabs(sdv[2]) > 1e-5)
                CC_ERROR(CC_StsBadArg,
                "For non-planar calibration rigs the initial intrinsic matrix must be specified");
        }
        for(i = 0; i < total; i++)
            ((CPoint3D64f*)(_M->tt.db + i*3))->z = 0.;

        if(flags & CC_CALIB_FIX_ASPECT_RATIO)
        {
            aspect_ratio = cvmGet(A,0,0);
            aspect_ratio /= cvmGet(A,1,1);
            if(aspect_ratio < 0.01 || aspect_ratio > 100)
                CC_ERROR(CC_StsOutOfRange,
                    "The specified aspect ratio (=a(0,0)/a(1,1)) is incorrect");
        }
        icvInitIntrinsicParams2D(_M, _m, counts, image_size, &_a, aspect_ratio);
    }

    _k = cvMat1(dist_coeffs->rows, dist_coeffs->cols, CC_64F, 1, _param->tt.db + 4);
    cvZero(&_k);

    // 2. initialize extrinsic parameters
    for(i = 0, pos = 0; i < img_count; i++, pos += count)
    {
        count = counts->tt.i[i];
        _ri = cvMat1(1, 3, CC_64F, 1, _param->tt.db + 8 + i*6);
        _ti = cvMat1(1, 3, CC_64F, 1, _param->tt.db + 8 + i*6 + 3);

        cvGetCols(_M, &_Mi, pos, pos + count);
        cvGetCols(_m, &_mi, pos, pos + count);
        cvFindExtrinsicCameraParams2(&_Mi, &_mi, &_a, &_k, &_ri, &_ti);
    }

    _param->tt.db[0] = a[0];
    _param->tt.db[1] = a[4];
    _param->tt.db[2] = a[2];
    _param->tt.db[3] = a[5];

    // 3. run the optimization
    for(iter = 0; iter < 30; iter++)
    {
        double* jj = _JtJ->tt.db;
        double change;

        for(i = 0, pos = 0; i < img_count; i++, pos += count)
        {
            count = counts->tt.i[i];
            _ri = cvMat1(1, 3, CC_64F, 1, _param->tt.db + 8 + i*6);
            _ti = cvMat1(1, 3, CC_64F, 1, _param->tt.db + 8 + i*6 + 3);

            cvGetCols(_M, &_Mi, pos, pos + count);
            _mi = cvMat1(count*2, 1, CC_64F, 1, _m->tt.db + pos*2);

            _dpdr->rows = _dpdt->rows = _dpdf->rows = _dpdc->rows = _dpdk->rows = count*2;

            _err->cols = 1;
            _err->rows = count*2;
            cvReshape(_err, _err, 2, count);
            cvProjectPoints2(&_Mi, &_ri, &_ti, &_a, &_k, _err, &_dpdr, &_dpdt, &_dpdf,
                              flags & CC_CALIB_FIX_PRINCIPAL_POINT ? 0 : &_dpdc, &_dpdk);

            // alter dpdf in case if only one of the focal
            // parameters (fy) is independent variable
            if(flags & CC_CALIB_FIX_ASPECT_RATIO)
                for(j = 0; j < count; j++)
                {
                    double* dpdf_j = (double*)(_dpdf->tt.data + j*_dpdf->step*2);
                    dpdf_j[1] = dpdf_j[0]*aspect_ratio;
                    dpdf_j[0] = 0.;
                }

            cvReshape(_err, _err, 1, count*2);
            cvSub(&_mi, _err, _err);
            
            _Je->rows = _Ji->rows = count*2;
            
            cvGetSubRect(_JtJ, &_part, cRect(0,0,8,8));
            cvGEMM(_Ji, _Ji, 1, &_part, i > 0, &_part, CC_GEMM_A_T);

            cvGetSubRect(_JtJ, &_part, cRect(8+i*6,8+i*6,6,6));
            cvMulTransposed(_Je, &_part, 1);
            
            cvGetSubRect(_JtJ, &_part, cRect(8+i*6,0,6,8));
            cvGEMM(_Ji, _Je, 1, 0, 0, &_part, CC_GEMM_A_T);

            cvGetRows(_JtErr, &_part, 0, 8);
            cvGEMM(_Ji, _err, 1, &_part, i > 0, &_part, CC_GEMM_A_T);

            cvGetRows(_JtErr, &_part, 8 + i*6, 8 + (i+1)*6);
            cvGEMM(_Je, _err, 1, 0, 0, &_part, CC_GEMM_A_T);
        }

        // make the matrix JtJ exactly symmetrical and add missing zeros
        for(i = 0; i < param_count; i++)
        {
            int mj = i < 8 ? param_count : ((i - 8)/6)*6 + 14;
            for(j = i+1; j < mj; j++)
                jj[j*param_count + i] = jj[i*param_count + j];
            for(; j < param_count; j++)
                jj[j*param_count + i] = jj[i*param_count + j] = 0;
        }

        cvSVD(_JtJ, _JtJW, 0, _JtJV, CC_SVD_MODIFY_A + CC_SVD_V_T);
        cvSVBkSb(_JtJW, _JtJV, _JtJV, _JtErr, _param_innov, CC_SVD_U_T + CC_SVD_V_T);

        cvScale(_param_innov, _param_innov, 1. - pow(1. - alpha_smooth, iter + 1.));
        cvGetRows(_param_innov, &_part, 0, 4);
        change = cvNorm(&_part);
        cvGetRows(_param, &_part, 0, 4);
        change /= cvNorm(&_part);

        if(flags & CC_CALIB_FIX_PRINCIPAL_POINT)
            _param_innov->tt.db[2] = _param_innov->tt.db[3] = 0.;

        if(flags & CC_CALIB_ZERO_TANGENT_DIST)
            _param_innov->tt.db[6] = _param_innov->tt.db[7] = 0.;

        cvAdd(_param, _param_innov, _param);

        if(flags & CC_CALIB_FIX_ASPECT_RATIO)
            _param->tt.db[0] = _param->tt.db[1]*aspect_ratio;
        
        a[0] = _param->tt.db[0];
        a[4] = _param->tt.db[1];
        a[2] = _param->tt.db[2];
        a[5] = _param->tt.db[3];

        if(change < FLT_EPSILON)
            break;
    }

    cvConvert(&_a, A);
    cvConvert(&_k, dist_coeffs);

    _r_part = cvMat1(output_r_matrices ? 3 : 1, 3, r_type);
    _t_part = cvMat1(1, 3, t_type);
    for(i = 0; i < img_count; i++)
    {
        if(r_vecs)
        {
            _sr_part->tt.db = _param->tt.db + 8 + i*6;
            _r_part->tt.data = r_vecs->tt.data + i*r_step;
            if(!output_r_matrices)
                cvConvert(&_sr_part, &_r_part);
            else
            {
                cvRodrigues2(&_sr_part, &_a);
                cvConvert(&_a, &_r_part);
            }
        }
        if(t_vecs)
        {
            _st_part->tt.db = _param->tt.db + 8 + i*6 + 3;
            _t_part->tt.data = t_vecs->tt.data + i*t_step;
            cvConvert(&_st_part, &_t_part);
        }
    }

    __END__;

    imfrees2(im);
}

#endif

/* Rodrigues transform */
#define CC_RODRIGUES_M2V  0
#define CC_RODRIGUES_V2M  1

/* Converts rotation_matrix matrix to rotation_matrix vector or vice versa */
CC_INLINE void  cvRodrigues(img_t* rotation_matrix, img_t* rotation_vector,
                             img_t* jacobian CC_DEFAULT(0), int conv_type CC_DEFAULT(CC_RODRIGUES_M2V))
{
    if(conv_type == CC_RODRIGUES_V2M)
        cvRodrigues2(rotation_vector, rotation_matrix, jacobian);
    else
        cvRodrigues2(rotation_matrix, rotation_vector, jacobian);
}

#if 0

/* Does reprojection of 3d object points to the view plane */
CC_INLINE void  cvProjectPoints(int point_count, CPoint3D64f* _object_points,
    double* _rotation_vector, double*  _translation_vector,
    double* focal_length, CPoint2D64f principal_point,
    double* _distortion, CPoint2D64f* _image_points,
    double* _deriv_points_rotation_matrix,
    double* _deriv_points_translation_vect,
    double* _deriv_points_focal,
    double* _deriv_points_principal_point,
    double* _deriv_points_distortion_coeffs)
{
    img_t object_points = cvMat1(point_count, 1, CC_64F, 3, _object_points);
    img_t image_points = cvMat1(point_count, 1, CC_64F, 2, _image_points);
    img_t rotation_vector = cvMat1(3, 1, CC_64F, 1, _rotation_vector);
    img_t translation_vector = cvMat1(3, 1, CC_64F, 1, _translation_vector);
    double a[9];
    img_t camera_matrix = cvMat1(3, 3, CC_64F, 1, a);
    img_t dist_coeffs = cvMat1(4, 1, CC_64F, 1, _distortion);
    img_t dpdr = cvMat1(2*point_count, 3, CC_64F, 1, _deriv_points_rotation_matrix);
    img_t dpdt = cvMat1(2*point_count, 3, CC_64F, 1, _deriv_points_translation_vect);
    img_t dpdf = cvMat1(2*point_count, 2, CC_64F, 1, _deriv_points_focal);
    img_t dpdc = cvMat1(2*point_count, 2, CC_64F, 1, _deriv_points_principal_point);
    img_t dpdk = cvMat1(2*point_count, 4, CC_64F, 1, _deriv_points_distortion_coeffs);

    a[0] = focal_length[0]; a[4] = focal_length[1];
    a[2] = principal_point.x; a[5] = principal_point.y;
    a[1] = a[3] = a[6] = a[7] = 0.;
    a[8] = 1.;

    cvProjectPoints2(&object_points, &rotation_vector, &translation_vector,
                      &camera_matrix, &dist_coeffs, &image_points,
                      &dpdr, &dpdt, &dpdf, &dpdc, &dpdk);
}


#endif
