
/**
Computes the matrix for the projection onto a tilted image sensor
\param tauX angular parameter rotation around x-axis
\param tauY angular parameter rotation around y-axis
\param matTilt if not NULL returns the matrix
\f[
\vecthreethree{R_{33}(\tau_x, \tau_y)}{0}{-R_{13}((\tau_x, \tau_y)}
{0}{R_{33}(\tau_x, \tau_y)}{-R_{23}(\tau_x, \tau_y)}
{0}{0}{1} R(\tau_x, \tau_y)
\f]
where
\f[
R(\tau_x, \tau_y) =
\vecthreethree{\cos(\tau_y)}{0}{-\sin(\tau_y)}{0}{1}{0}{\sin(\tau_y)}{0}{\cos(\tau_y)}
\vecthreethree{1}{0}{0}{0}{\cos(\tau_x)}{\sin(\tau_x)}{0}{-\sin(\tau_x)}{\cos(\tau_x)} =
\vecthreethree{\cos(\tau_y)}{\sin(\tau_y)\sin(\tau_x)}{-\sin(\tau_y)\cos(\tau_x)}
{0}{\cos(\tau_x)}{\sin(\tau_x)}
{\sin(\tau_y)}{-\cos(\tau_y)\sin(\tau_x)}{\cos(\tau_y)\cos(\tau_x)}.
\f]
\param dMatTiltdTauX if not NULL it returns the derivative of matTilt with
respect to \f$\tau_x\f$.
\param dMatTiltdTauY if not NULL it returns the derivative of matTilt with
respect to \f$\tau_y\f$.
\param invMatTilt if not NULL it returns the inverse of matTilt
**/

#include "math/math_def.txt"
#include "math/mat3.inl"

static void computeTiltProjectionMatrix(double tauX,
    double tauY,
    double9 matTilt = 0,
    double9 dMatTiltdTauX = 0,
    double9 dMatTiltdTauY = 0,
    double9 invMatTilt = 0)
{
    double cTauX = cos(tauX);
    double sTauX = sin(tauX);
    double cTauY = cos(tauY);
    double sTauY = sin(tauY);
    double9 matRotX = {1,0,0,0,cTauX,sTauX,0,-sTauX,cTauX};
    double9 matRotY = {cTauY,0,-sTauY,0,1,0,sTauY,0,cTauY};
    double33 matRotXY;
    double9 matProjZ;
    //matRotXY = matRotY * matRotX;
    M3MUL(matRotXY[0], matRotY, matRotX);
    SET9(matProjZ, matRotXY[2][2],0,-matRotXY[0][2],0,matRotXY[2][2],-matRotXY[1][2],0,0,1);

    if (matTilt)
    {
        // Matrix for trapezoidal distortion of tilted image sensor
        // matTilt = matProjZ * matRotXY;
        M3MUL(matTilt, matProjZ, matRotXY[0]);
    }
    if (dMatTiltdTauX)
    {
        // Derivative with respect to tauX
        double9 dMatProjZdTauX, tmp = {0,0,0,0,-sTauX,cTauX,0,-cTauX,-sTauX}, tmp1;
        double33 dMatRotXYdTauX;
        // dMatRotXYdTauX = matRotY * tmp;
        M3MUL(dMatRotXYdTauX[0], matRotY, tmp);
        SET9(dMatProjZdTauX, dMatRotXYdTauX[2][2],0,-dMatRotXYdTauX[0][2], 0,dMatRotXYdTauX[2][2],-dMatRotXYdTauX[1][2],0,0,0);
        // dMatTiltdTauX = (matProjZ * dMatRotXYdTauX) + (dMatProjZdTauX * matRotXY);
        M3MUL(tmp, matProjZ, dMatRotXYdTauX[0]);
        M3MUL(tmp1, dMatProjZdTauX, matRotXY[0]);
        V9ADDV(dMatTiltdTauX, tmp, tmp1);
        
    }
    if (dMatTiltdTauY)
    {
        // Derivative with respect to tauY
        double9 tmp, tmp1, dMatProjZdTauY;
        double33 dMatRotXYdTauY;
        SET9(tmp, -sTauY,0,-cTauY,0,0,0,cTauY,0,-sTauY);
        M3MUL(dMatRotXYdTauY[0], tmp, matRotX);
        SET9(dMatProjZdTauY, dMatRotXYdTauY[2][2],0,-dMatRotXYdTauY[0][2],
          0,dMatRotXYdTauY[2][2],-dMatRotXYdTauY[1][2],0,0,0);
        //dMatTiltdTauY = (matProjZ * dMatRotXYdTauY) + (dMatProjZdTauY * matRotXY);
        M3MUL(tmp, matProjZ, dMatRotXYdTauY[0]);
        M3MUL(tmp1, dMatProjZdTauY, matRotXY[0]);
        V9ADDV(dMatTiltdTauY, tmp, tmp1);
    }
    if (invMatTilt)
    {
        double inv = 1./matRotXY[2][2], t;
        double9 invMatProjZ;
        SET9(invMatProjZ, inv,0,inv*matRotXY[0][2],0,inv,inv*matRotXY[1][2],0,0,1);
        //invMatTilt = matRotXY.t()*invMatProjZ;
        M3TRANS(matRotXY[0], t);
        M3MUL(invMatTilt, matRotXY[0], invMatProjZ);
    }
}

#include "cv/core/saturate.hpp"

#if 0
#include "cv/core/matx.hpp"

template <typename FLOAT>
void computeTiltProjectionMatrix(FLOAT tauX,
    FLOAT tauY,
    Matx<FLOAT, 3, 3>* matTilt = 0,
    Matx<FLOAT, 3, 3>* dMatTiltdTauX = 0,
    Matx<FLOAT, 3, 3>* dMatTiltdTauY = 0,
    Matx<FLOAT, 3, 3>* invMatTilt = 0)
{
    FLOAT cTauX = cos(tauX);
    FLOAT sTauX = sin(tauX);
    FLOAT cTauY = cos(tauY);
    FLOAT sTauY = sin(tauY);
    Matx<FLOAT, 3, 3> matRotX = Matx<FLOAT, 3, 3>(1,0,0,0,cTauX,sTauX,0,-sTauX,cTauX);
    Matx<FLOAT, 3, 3> matRotY = Matx<FLOAT, 3, 3>(cTauY,0,-sTauY,0,1,0,sTauY,0,cTauY);
    Matx<FLOAT, 3, 3> matRotXY = matRotY * matRotX;
    Matx<FLOAT, 3, 3> matProjZ = Matx<FLOAT, 3, 3>(matRotXY(2,2),0,-matRotXY(0,2),0,matRotXY(2,2),-matRotXY(1,2),0,0,1);
    if (matTilt)
    {
        // Matrix for trapezoidal distortion of tilted image sensor
        *matTilt = matProjZ * matRotXY;
    }
    if (dMatTiltdTauX)
    {
        // Derivative with respect to tauX
        Matx<FLOAT, 3, 3> dMatRotXYdTauX = matRotY * Matx<FLOAT, 3, 3>(0,0,0,0,-sTauX,cTauX,0,-cTauX,-sTauX);
        Matx<FLOAT, 3, 3> dMatProjZdTauX = Matx<FLOAT, 3, 3>(dMatRotXYdTauX(2,2),0,-dMatRotXYdTauX(0,2),
          0,dMatRotXYdTauX(2,2),-dMatRotXYdTauX(1,2),0,0,0);
        *dMatTiltdTauX = (matProjZ * dMatRotXYdTauX) + (dMatProjZdTauX * matRotXY);
    }
    if (dMatTiltdTauY)
    {
        // Derivative with respect to tauY
        Matx<FLOAT, 3, 3> dMatRotXYdTauY = Matx<FLOAT, 3, 3>(-sTauY,0,-cTauY,0,0,0,cTauY,0,-sTauY) * matRotX;
        Matx<FLOAT, 3, 3> dMatProjZdTauY = Matx<FLOAT, 3, 3>(dMatRotXYdTauY(2,2),0,-dMatRotXYdTauY(0,2),
          0,dMatRotXYdTauY(2,2),-dMatRotXYdTauY(1,2),0,0,0);
        *dMatTiltdTauY = (matProjZ * dMatRotXYdTauY) + (dMatProjZdTauY * matRotXY);
    }
    if (invMatTilt)
    {
        FLOAT inv = 1./matRotXY(2,2);
        Matx<FLOAT, 3, 3> invMatProjZ = Matx<FLOAT, 3, 3>(inv,0,inv*matRotXY(0,2),0,inv,inv*matRotXY(1,2),0,0,1);
        *invMatTilt = matRotXY.t()*invMatProjZ;
    }
}
#endif

static void cvGetDefaultNewCameraMatrix( const img_t* cameraMatrix, CSize imgsize,
                               bool centerPrincipalPoint, img_t* newCameraMatrix )
{
    if( !centerPrincipalPoint && cameraMatrix->tid == CC_64F ) {
        cvCloneMat(newCameraMatrix, cameraMatrix);
        return ;
    }

    cvConvertTo(cameraMatrix, newCameraMatrix, CC_64F);
    if( centerPrincipalPoint )
    {
        img_ptr(double, newCameraMatrix)[2] = (imgsize.width-1)*0.5;
        img_ptr(double, newCameraMatrix)[5] = (imgsize.height-1)*0.5;
    }
    return ;
}

static void initUndistortRectifyMap1(const img_t* cameraMatrix, const img_t* _distCoeffs,
                              const img_t* matR, const img_t* newCameraMatrix,
                              CSize size, TypeCnId m1type, img_t* map1, img_t* map2 )
{
    int i = 0;
    img_t im[10] = {0};
    img_t* _im = im;
    if( m1type <= 0 )
        m1type = CC_16SC2;
    CC_Assert( m1type == CC_16SC2 || m1type == CC_32FC1 || m1type == CC_32FC2 );
    cvSetMat1( map1, size.h, size.w, m1type );
    if( m1type != CC_32FC2 )
    {
        cvSetMat1( map2, size.h, size.w, m1type == CC_16SC2 ? CC_16UC1 : CC_32FC1 );
    }
    else {
        imfree(map2);
    }

#if 0
    img_t* R = cvSetMat(_im++, 3, 3, CC_64F);
    cvEye(R);
    cvCloneMat(A, cameraMatrix);

    if( !imempty(newCameraMatrix) ) {
        cvCloneMat(Ar, newCameraMatrix);
    } else {
        cvGetDefaultNewCameraMatrix( A, size, true, Ar );
    }

    if( !imempty(matR) )
        cvCloneMat(R, matR);

    if( !imempty(_distCoeffs) ) {
        cvCloneMat(distCoeffs, _distCoeffs);
    } else
    {
        cvSetMat(distCoeffs, 14, 1, CC_64F);
        cvZero(distCoeffs);
    }

    CC_ASSERT( A->h==3 && A->w==3 && CC_ARE_SIZES_EQ(A, R) );
    CC_ASSERT( (Ar->h==3 && Ar->w==3) || (Ar->h==3 && Ar->w==4) );
    Mat_<double> iR = (Ar.colRange(0,3)*R).inv(DECOMP_LU);
    const double* ir = &iR(0,0);

    double u0 = A(0, 2),  v0 = A(1, 2);
    double fx = A(0, 0),  fy = A(1, 1);

    CC_Assert( distCoeffs.size() == CSize(1, 4) || distCoeffs.size() == CSize(4, 1) ||
               distCoeffs.size() == CSize(1, 5) || distCoeffs.size() == CSize(5, 1) ||
               distCoeffs.size() == CSize(1, 8) || distCoeffs.size() == CSize(8, 1) ||
               distCoeffs.size() == CSize(1, 12) || distCoeffs.size() == CSize(12, 1) ||
               distCoeffs.size() == CSize(1, 14) || distCoeffs.size() == CSize(14, 1));

    if( distCoeffs->rows != 1 && !distCoeffs CC_IS_CONT_MAT() )
        distCoeffs = distCoeffs.t();

    const double* const distPtr = distCoeffs img_ptr(double, );
    double k1 = distPtr[0];
    double k2 = distPtr[1];
    double p1 = distPtr[2];
    double p2 = distPtr[3];
    double k3 = distCoeffs->cols + distCoeffs->rows - 1 >= 5 ? distPtr[4] : 0.;
    double k4 = distCoeffs->cols + distCoeffs->rows - 1 >= 8 ? distPtr[5] : 0.;
    double k5 = distCoeffs->cols + distCoeffs->rows - 1 >= 8 ? distPtr[6] : 0.;
    double k6 = distCoeffs->cols + distCoeffs->rows - 1 >= 8 ? distPtr[7] : 0.;
    double s1 = distCoeffs->cols + distCoeffs->rows - 1 >= 12 ? distPtr[8] : 0.;
    double s2 = distCoeffs->cols + distCoeffs->rows - 1 >= 12 ? distPtr[9] : 0.;
    double s3 = distCoeffs->cols + distCoeffs->rows - 1 >= 12 ? distPtr[10] : 0.;
    double s4 = distCoeffs->cols + distCoeffs->rows - 1 >= 12 ? distPtr[11] : 0.;
    double tauX = distCoeffs->cols + distCoeffs->rows - 1 >= 14 ? distPtr[12] : 0.;
    double tauY = distCoeffs->cols + distCoeffs->rows - 1 >= 14 ? distPtr[13] : 0.;

    // Matrix for trapezoidal distortion of tilted image sensor
    Matx33d matTilt = Matx33d::eye();
    detail::computeTiltProjectionMatrix(tauX, tauY, &matTilt);

    for( int i = 0; i < size.height; i++ )
    {
        float* m1f = map1.ptr<float>(i);
        float* m2f = map2.empty() ? 0 : map2.ptr<float>(i);
        short* m1 = (short*)m1f;
        ushort* m2 = (ushort*)m2f;
        double _x = i*ir[1] + ir[2], _y = i*ir[4] + ir[5], _w = i*ir[7] + ir[8];

        for( int j = 0; j < size.width; j++, _x += ir[0], _y += ir[3], _w += ir[6] )
        {
            double w = 1./_w, x = _x*w, y = _y*w;
            double x2 = x*x, y2 = y*y;
            double r2 = x2 + y2, _2xy = 2*x*y;
            double kr = (1 + ((k3*r2 + k2)*r2 + k1)*r2)/(1 + ((k6*r2 + k5)*r2 + k4)*r2);
            double xd = (x*kr + p1*_2xy + p2*(r2 + 2*x2) + s1*r2+s2*r2*r2);
            double yd = (y*kr + p1*(r2 + 2*y2) + p2*_2xy + s3*r2+s4*r2*r2);
            Vec3d vecTilt = matTilt*Vec3d(xd, yd, 1);
            double invProj = vecTilt(2) ? 1./vecTilt(2) : 1;
            double u = fx*invProj*vecTilt(0) + u0;
            double v = fy*invProj*vecTilt(1) + v0;
            if( m1type == CC_16SC2 )
            {
                int iu = saturate_cast<int>(u*CC_INTER_TAB_SIZE);
                int iv = saturate_cast<int>(v*CC_INTER_TAB_SIZE);
                m1[j*2] = (short)(iu >> CC_INTER_BITS);
                m1[j*2+1] = (short)(iv >> CC_INTER_BITS);
                m2[j] = (ushort)((iv & (CC_INTER_TAB_SIZE-1))*CC_INTER_TAB_SIZE + (iu & (CC_INTER_TAB_SIZE-1)));
            }
            else if( m1type == CC_32FC1 )
            {
                m1f[j] = (float)u;
                m2f[j] = (float)v;
            }
            else
            {
                m1f[j*2] = (float)u;
                m1f[j*2+1] = (float)v;
            }
        }
    }
#endif
    imfrees2(im);
}


static img_t* getDefaultNewCameraMatrix( img_t* newCameraMatrix, const img_t* cameraMatrix, CSize imgsize,
                               bool centerPrincipalPoint)
{
    if( !centerPrincipalPoint && CC_MAT_TYPE(cameraMatrix) == CC_64F ) {
        cvCloneMat(newCameraMatrix, cameraMatrix);
        return newCameraMatrix;
    }

    cvConvertTo(cameraMatrix, newCameraMatrix, CC_64F);
    if( centerPrincipalPoint )
    {
        img_ptr(double, newCameraMatrix)[2] = (imgsize.width-1)*0.5;
        img_ptr(double, newCameraMatrix)[5] = (imgsize.height-1)*0.5;
    }
    return newCameraMatrix;
}

static void initUndistortRectifyMap( const img_t* _cameraMatrix, const img_t* _distCoeffs,
                              const img_t* _matR, const img_t* _newCameraMatrix,
                              CSize size, TypeCnId m1type, img_t* _map1, img_t* _map2 )
{
    const img_t *cameraMatrix = _cameraMatrix;
    const img_t *matR = _matR, *newCameraMatrix = _newCameraMatrix;

    IM_BEGIN(im, 10);
    if( m1type <= 0 )
        m1type = CC_16SC2;
    CC_Assert( m1type == CC_16SC2 || m1type == CC_32FC1 || m1type == CC_32FC2 );
    cvSetMat1( _map1, size.height, size.width, m1type );
    img_t *map1 = _map1, *map2 = NULL;
    if( m1type != CC_32FC2 )
    {
        cvSetMat1( _map2, size.height, size.width, m1type == CC_16SC2 ? CC_16UC1 : CC_32FC1 );
        map2 = _map2;
    }
    else
        imfree(_map2);

    img_t* R = cvSetEye(im++, 3, 3, CC_64F);
    img_t* A = im++;
    img_t* Ar = im++;
    img_t* distCoeffs = im++;
    cvConvertTo(cameraMatrix, A, CC_64F);

    if( !imempty(newCameraMatrix) )
        cvConvertTo(newCameraMatrix, Ar, CC_64F);
    else
        getDefaultNewCameraMatrix( Ar, A, size, true );

    if( !imempty(matR) )
        cvConvertTo(matR, R, CC_64F);

    if( !imempty(_distCoeffs) )
        cvConvertTo(_distCoeffs, distCoeffs, CC_64F);
    else
    {
        cvSetTo(distCoeffs, 14, 1, CC_64F, 0.);
    }

    CC_Assert( CC_ARE_SIZES_EQSZ(A, 3,3) && CC_ARE_SIZES_EQ(A, R) );
    CC_Assert( CC_ARE_SIZES_EQSZ(Ar, 3,3) || CC_ARE_SIZES_EQSZ(Ar, 3, 4) );
    img_t tmp[1] = {0};
    cvGetCols(Ar, tmp, 0,3);
    img_t* t1 = im++;
    img_t* iR = im++;
    matmul(tmp, R, t1);
    invert(t1, iR, CC_DECOMP_LU);
    const double* ir = img_at(double, iR, 0,0);

    double u0 = *img_at(double, A, 0, 2),  v0 = *img_at(double, A, 1, 2);
    double fx = *img_at(double, A, 0, 0),  fy = *img_at(double, A, 1, 1);

    CC_Assert( CC_ARE_SIZES_EQSZ2(distCoeffs, 1, 4) ||
               CC_ARE_SIZES_EQSZ2(distCoeffs, 1, 5) ||
               CC_ARE_SIZES_EQSZ2(distCoeffs, 1, 8) ||
               CC_ARE_SIZES_EQSZ2(distCoeffs, 1, 12) ||
               CC_ARE_SIZES_EQSZ2(distCoeffs, 1, 14));

    if( distCoeffs->rows != 1 && !CC_IS_CONT_MAT(distCoeffs) )
        cvT(distCoeffs, distCoeffs);

    const double* const distPtr = img_ptr(double, distCoeffs);
    double k1 = distPtr[0];
    double k2 = distPtr[1];
    double p1 = distPtr[2];
    double p2 = distPtr[3];
    double k3 = distCoeffs->cols + distCoeffs->rows - 1 >= 5 ? distPtr[4] : 0.;
    double k4 = distCoeffs->cols + distCoeffs->rows - 1 >= 8 ? distPtr[5] : 0.;
    double k5 = distCoeffs->cols + distCoeffs->rows - 1 >= 8 ? distPtr[6] : 0.;
    double k6 = distCoeffs->cols + distCoeffs->rows - 1 >= 8 ? distPtr[7] : 0.;
    double s1 = distCoeffs->cols + distCoeffs->rows - 1 >= 12 ? distPtr[8] : 0.;
    double s2 = distCoeffs->cols + distCoeffs->rows - 1 >= 12 ? distPtr[9] : 0.;
    double s3 = distCoeffs->cols + distCoeffs->rows - 1 >= 12 ? distPtr[10] : 0.;
    double s4 = distCoeffs->cols + distCoeffs->rows - 1 >= 12 ? distPtr[11] : 0.;
    double tauX = distCoeffs->cols + distCoeffs->rows - 1 >= 14 ? distPtr[12] : 0.;
    double tauY = distCoeffs->cols + distCoeffs->rows - 1 >= 14 ? distPtr[13] : 0.;

    // Matrix for trapezoidal distortion of tilted image sensor
    double9 matTilt;
    eye3(matTilt);
    computeTiltProjectionMatrix(tauX, tauY, matTilt);

    for( int i = 0; i < size.height; i++ )
    {
        float* m1f = img_row(float, map1, i);
        float* m2f = imempty(map2) ? 0 : img_row(float, map2, i);
        short* m1 = (short*)m1f;
        ushort* m2 = (ushort*)m2f;
        double _x = i*ir[1] + ir[2], _y = i*ir[4] + ir[5], _w = i*ir[7] + ir[8];

        for( int j = 0; j < size.width; j++, _x += ir[0], _y += ir[3], _w += ir[6] )
        {
            double w = 1./_w, x = _x*w, y = _y*w;
            double x2 = x*x, y2 = y*y;
            double r2 = x2 + y2, _2xy = 2*x*y;
            double kr = (1 + ((k3*r2 + k2)*r2 + k1)*r2)/(1 + ((k6*r2 + k5)*r2 + k4)*r2);
            double xd = (x*kr + p1*_2xy + p2*(r2 + 2*x2) + s1*r2+s2*r2*r2);
            double yd = (y*kr + p1*(r2 + 2*y2) + p2*_2xy + s3*r2+s4*r2*r2);
            double3 vecTilt;
            double3 tmp = {xd, yd, 1};
            matmulvec3(vecTilt, matTilt, tmp, false);
            double invProj = vecTilt[2] ? 1./vecTilt[2] : 1;
            double u = fx*invProj*vecTilt[0] + u0;
            double v = fy*invProj*vecTilt[1] + v0;
            if( m1type == CC_16SC2 )
            {
                int iu, iv;
                iu = double_int(u*CC_INTER_TAB_SIZE);
                iv = double_int(v*CC_INTER_TAB_SIZE);
                m1[j*2] = int_short(iu >> CC_INTER_BITS);
                m1[j*2+1] = int_short(iv >> CC_INTER_BITS);
                m2[j] = (ushort)((iv & (CC_INTER_TAB_SIZE-1))*CC_INTER_TAB_SIZE + (iu & (CC_INTER_TAB_SIZE-1)));
            }
            else if( m1type == CC_32FC1 )
            {
                m1f[j] = (float)u;
                m2f[j] = (float)v;
            }
            else
            {
                m1f[j*2] = (float)u;
                m1f[j*2+1] = (float)v;
            }
        }
    }
    IM_END(im);
}

static void undistort( const img_t* _src, img_t* dst, const img_t* _cameraMatrix,
               const img_t* _distCoeffs, const img_t* _newCameraMatrix )
{
    CC_INSTRUMENT_REGION()
    const img_t *src = _src, *cameraMatrix = _cameraMatrix;
    const img_t *newCameraMatrix = _newCameraMatrix;

    IM_BEGIN(im, 10);
    cvSetMat( dst, src->rows, src->cols, src->tid );

    CC_Assert( dst->tt.data != src->tt.data );

    int stripe_size0 = MIN(MAX(1, (1 << 12) / MAX(src->cols, 1)), src->rows);

    img_t *map1 = cvSetMat1(im++, stripe_size0, src->cols, CC_16SC2), *map2 = cvSetMat1(im++, stripe_size0, src->cols, CC_16UC1);

    img_t *A=im++, *Ar=im++, *I = cvSetEye(im++, 3, 3, CC_64F);

    cvConvertTo(cameraMatrix, A, CC_64F);
    img_t *distCoeffs = im++;
    if( !imempty(distCoeffs) )
        cvConvertTo(_distCoeffs, distCoeffs, CC_64F);
    else
    {
        cvSetTo(distCoeffs, 5, 1, CC_64F, 0);
    }

    if( !imempty(newCameraMatrix) )
        cvConvertTo(newCameraMatrix, Ar, CC_64F);
    else
        cvCopy(A, Ar);

    double v0 = *img_at(double, Ar, 1, 2);
    for( int y = 0; y < src->rows; y += stripe_size0 )
    {
        int stripe_size = MIN( stripe_size0, src->rows - y );
        *img_at(double, Ar, 1, 2) = v0 - y;
        img_t tmp[3];
        img_t *map1_part = cvGetRows(map1, tmp+0, 0, stripe_size),
            *map2_part = cvGetRows(map2, tmp+1, 0, stripe_size),
            *dst_part = cvGetRows(dst, tmp+2, y, y + stripe_size);

        initUndistortRectifyMap( A, distCoeffs, I, Ar, cSize(src->cols, stripe_size),
                                 CC_MAT_TYPECN(map1_part), map1_part, map2_part );
        //remap( src, dst_part, map1_part, map2_part, CC_INTER_LINEAR, CC_BORDER_CONSTANT );
        cvRemap( src, dst_part, map1_part, map2_part, CC_INTER_LINEAR+CC_WARP_FILL_OUTLIERS );
    }
    
    IM_END(im);
}

CC_IMPL void
cvUndistort2( const img_t* src, img_t* dst, const img_t* Aarr, const img_t* dist_coeffs, const img_t* newAarr )
{
    img_t *dst0 = dst;
    const img_t *A = (Aarr), *distCoeffs = (dist_coeffs), *newA = NULL;
    if( newAarr )
        newA = (newAarr);

    CC_Assert( CC_ARE_SIZES_EQ(src, dst) && CC_ARE_TYPES_EQ(src, dst) );
    undistort( src, dst, A, distCoeffs, newA );
}


CC_IMPL void cvInitUndistortMap( const img_t* Aarr, const img_t* dist_coeffs,
                                 img_t* mapxarr, img_t* mapyarr )
{
    const img_t *A = (Aarr), *distCoeffs = (dist_coeffs);
    img_t *mapx = (mapxarr), *mapy = NULL, *mapx0 = mapx, *mapy0 = NULL;

    if( mapyarr )
        mapy0 = mapy = (mapyarr);

    initUndistortRectifyMap( A, distCoeffs, NULL, A,
                                 cvGetSize(mapx), CC_MAT_TYPECN(mapx), mapx, mapy );
    CC_Assert( mapx0->tt.data == mapx->tt.data && mapy0->tt.data == mapy->tt.data );
}

static void cvInitUndistortRectifyMap( const img_t* Aarr, const img_t* dist_coeffs,
    const img_t *Rarr, const img_t* ArArr, img_t* mapxarr, img_t* mapyarr )
{
    const img_t *A = (Aarr), *distCoeffs = NULL, *R=NULL, *Ar=NULL;
    img_t *mapx = (mapxarr), *mapy=NULL, *mapx0 = mapx, *mapy0=NULL;

    if( mapyarr )
        mapy0 = mapy = (mapyarr);

    if( dist_coeffs )
        distCoeffs = (dist_coeffs);
    if( Rarr )
        R = (Rarr);
    if( ArArr )
        Ar = (ArArr);

    initUndistortRectifyMap( A, distCoeffs, R, Ar, cvGetSize(mapx), CC_MAT_TYPECN(mapx), mapx, mapy );
    CC_Assert( mapx0->tt.data == mapx->tt.data && mapy0->tt.data == mapy->tt.data );
}

static void cvUndistortPoints( const img_t* _src, img_t* _dst, const img_t* _cameraMatrix,
                   const img_t* _distCoeffs,
                   const img_t* matR CC_DEFAULT(0), const img_t* matP CC_DEFAULT(0))
{
    double A[3][3], RR[3][3], k[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    img_t matA[1] = {0}, _Dk[1] = {0}, _RR[1] = {0};
    double9 invMatTilt;
    cvMat(matA, 3, 3, CC_64F, 1, A[0]);
    cvMat(_RR, 3, 3, CC_64F, 1, RR[0]);
    eye3(invMatTilt);

    CC_Assert( (_src) && (_dst) &&
        (_src->rows == 1 || _src->cols == 1) &&
        (_dst->rows == 1 || _dst->cols == 1) &&
        _src->cols + _src->rows - 1 == _dst->rows + _dst->cols - 1 &&
        (CC_MAT_TYPECN(_src) == CC_32FC2 || CC_MAT_TYPECN(_src) == CC_64FC2) &&
        (CC_MAT_TYPECN(_dst) == CC_32FC2 || CC_MAT_TYPECN(_dst) == CC_64FC2));

    CC_Assert( (_cameraMatrix) &&
        _cameraMatrix->rows == 3 && _cameraMatrix->cols == 3 );

    cvConvert( _cameraMatrix, matA );

    int iters = 0;

    if( _distCoeffs )
    {
        CC_Assert( (_distCoeffs) &&
            (_distCoeffs->rows == 1 || _distCoeffs->cols == 1) &&
            (_distCoeffs->rows*_distCoeffs->cols == 4 ||
             _distCoeffs->rows*_distCoeffs->cols == 5 ||
             _distCoeffs->rows*_distCoeffs->cols == 8 ||
             _distCoeffs->rows*_distCoeffs->cols == 12 ||
             _distCoeffs->rows*_distCoeffs->cols == 14));

        cvMat( _Dk, _distCoeffs->rows, _distCoeffs->cols, CC_64F, CC_MAT_CN(_distCoeffs), k);

        cvConvert( _distCoeffs, _Dk );
        iters = 5;
        if (k[12] != 0 || k[13] != 0)
            computeTiltProjectionMatrix(k[12], k[13], NULL, NULL, NULL, invMatTilt);
    }

    if( matR )
    {
        CC_Assert( (matR) && matR->rows == 3 && matR->cols == 3 );
        cvConvert( matR, _RR );
    }
    else
        cvSetIdentity(_RR);

    if( matP )
    {
        double PP[3][3];
        img_t _P3x3[1]={0}, _PP[1]={0};
        cvMat(_PP, 3, 3, CC_64F, 1, PP[0]);
        CC_Assert( (matP) && matP->rows == 3 && (matP->cols == 3 || matP->cols == 4));
        cvConvert( cvGetCols(matP, _P3x3, 0, 3), _PP );
        cvMatMul( _PP, _RR, _RR );
    }

    const CPoint2D32f* srcf = (const CPoint2D32f*)_src->tt.data;
    const CPoint2D64f* srcd = (const CPoint2D64f*)_src->tt.data;
    CPoint2D32f* dstf = (CPoint2D32f*)_dst->tt.data;
    CPoint2D64f* dstd = (CPoint2D64f*)_dst->tt.data;
    TypeCnId stype = CC_MAT_TYPECN(_src);
    TypeCnId dtype = CC_MAT_TYPECN(_dst);
    int sstep = _src->rows == 1 ? 1 : _src->step/CC_TYPE_SIZE(_src->tid);
    int dstep = _dst->rows == 1 ? 1 : _dst->step/CC_TYPE_SIZE(_dst->tid);

    double fx = A[0][0];
    double fy = A[1][1];
    double ifx = 1./fx;
    double ify = 1./fy;
    double cx = A[0][2];
    double cy = A[1][2];

    int n = _src->rows + _src->cols - 1;
    for( int i = 0; i < n; i++ )
    {
        double x, y, x0 = 0, y0 = 0;
        if( stype == CC_32FC2 )
        {
            x = srcf[i*sstep].x;
            y = srcf[i*sstep].y;
        }
        else
        {
            x = srcd[i*sstep].x;
            y = srcd[i*sstep].y;
        }

        x = (x - cx)*ifx;
        y = (y - cy)*ify;

        if( iters ) {
            // compensate tilt distortion
            double3 vecUntilt;
            double3 tmp = {x, y, 1};
            matmulvec3(vecUntilt, invMatTilt, tmp, false);
            double invProj = vecUntilt[2] ? 1./vecUntilt[2] : 1;
            x0 = x = invProj * vecUntilt[0];
            y0 = y = invProj * vecUntilt[1];
        }

        // compensate distortion iteratively
        for( int j = 0; j < iters; j++ )
        {
            double r2 = x*x + y*y;
            double icdist = (1 + ((k[7]*r2 + k[6])*r2 + k[5])*r2)/(1 + ((k[4]*r2 + k[1])*r2 + k[0])*r2);
            double deltaX = 2*k[2]*x*y + k[3]*(r2 + 2*x*x)+ k[8]*r2+k[9]*r2*r2;
            double deltaY = k[2]*(r2 + 2*y*y) + 2*k[3]*x*y+ k[10]*r2+k[11]*r2*r2;
            x = (x0 - deltaX)*icdist;
            y = (y0 - deltaY)*icdist;
        }

        double xx = RR[0][0]*x + RR[0][1]*y + RR[0][2];
        double yy = RR[1][0]*x + RR[1][1]*y + RR[1][2];
        double ww = 1./(RR[2][0]*x + RR[2][1]*y + RR[2][2]);
        x = xx*ww;
        y = yy*ww;

        if( dtype == CC_32FC2 )
        {
            dstf[i*dstep].x = (float)x;
            dstf[i*dstep].y = (float)y;
        }
        else
        {
            dstd[i*dstep].x = x;
            dstd[i*dstep].y = y;
        }
    }
}


static void undistortPoints( const img_t* _src, img_t* dst,
                          const img_t* _cameraMatrix,
                          const img_t* _distCoeffs,
                          const img_t* _Rmat CC_DEFAULT(NULL),
                          const img_t* _Pmat CC_DEFAULT(NULL))
{
    const img_t *src = _src, *cameraMatrix = _cameraMatrix;
    const img_t *distCoeffs = _distCoeffs, *R = _Rmat, *P = _Pmat;

    CC_Assert( CC_IS_CONT_MAT(src) && (src->tid == CC_32F || src->tid == CC_64F) &&
              ((src->rows == 1 && CC_MAT_CN(src) == 2) || src->cols*CC_MAT_CN(src) == 2));

    cvSetMat(dst, src->rows, src->cols, src->tid);

    const img_t *_csrc = src, *_ccameraMatrix = cameraMatrix;
    img_t *_cdst = dst;
    IM_BEGIN(im, 4);
    img_t *pR=0, *pP=0, *pD=0;
    if( !imempty(R) )
        pR = cvCloneMat(im++, R);
    if( !imempty(P) )
        pP = cvCloneMat(im++, P);
    if( !imempty(distCoeffs) )
        pD = cvCloneMat(im++, distCoeffs);
    cvUndistortPoints(_csrc, _cdst, _ccameraMatrix, pD, pR, pP);
    IM_END(im);
}


static CPoint2f mapPointSpherical(const CPoint2f& p, float alpha, double4 J, int projType)
{
    double x = p.x, y = p.y;
    double beta = 1 + 2*alpha;
    double v = x*x + y*y + 1, iv = 1/v;
    double u = sqrt(beta*v + alpha*alpha);

    double k = (u - alpha)*iv;
    double kv = (v*beta/u - (u - alpha)*2)*iv*iv;
    double kx = kv*x, ky = kv*y;

    if( projType == CC_PROJ_SPHERICAL_ORTHO )
    {
        if(J) {
            SET4(J, kx*x + k, kx*y, ky*x, ky*y + k);
        }
        return fPOINT((x*k), (y*k));
    }
    if( projType == CC_PROJ_SPHERICAL_EQRECT )
    {
        // equirectangular
        double iR = 1/(alpha + 1);
        double x1 = MAX(MIN(x*k*iR, 1.), -1.);
        double y1 = MAX(MIN(y*k*iR, 1.), -1.);

        if(J)
        {
            double fx1 = iR/sqrt(1 - x1*x1);
            double fy1 = iR/sqrt(1 - y1*y1);
            SET4(J, fx1*(kx*x + k), fx1*ky*x, fy1*kx*y, fy1*(ky*y + k));
        }
        return fPOINT(asin(x1), asin(y1));
    }
    CC_Error(CC_StsBadArg, "Unknown projection type");
    return fPOINT(0,0);
}


static CPoint2f invMapPointSpherical(CPoint2f _p, float alpha, int projType)
{
    static int avgiter = 0, avgn = 0;

    double eps = 1e-12;
    double2 p={_p.x, _p.y}, q = {_p.x, _p.y}, err = {0};
    double4 J = {0};
    int i, maxiter = 5;

    for( i = 0; i < maxiter; i++ )
    {
        CPoint2f p1 = mapPointSpherical(fPOINT(q[0], q[1]), alpha, J, projType);
        //err = Vec2d(p1.x, p1.y) - p;
        SET2(err, p1.x - p[0], p1.y - p[1]);
        if( err[0]*err[0] + err[1]*err[1] < eps )
            break;

        double4 JtJ = {J[0]*J[0] + J[2]*J[2], J[0]*J[1] + J[2]*J[3],
          J[0]*J[1] + J[2]*J[3], J[1]*J[1] + J[3]*J[3]};
        double d = JtJ[0]*JtJ[3] - JtJ[1]*JtJ[2];
        d = d ? 1./d : 0;
        double4 iJtJ = {JtJ[3]*d, -JtJ[1]*d, -JtJ[2]*d, JtJ[0]*d};
        double2 JtErr = {J[0]*err[0] + J[2]*err[1], J[1]*err[0] + J[3]*err[1]};

        //q -= Vec2d(iJtJ[0]*JtErr[0] + iJtJ[1]*JtErr[1], iJtJ[2]*JtErr[0] + iJtJ[3]*JtErr[1]);
        q[0] -= iJtJ[0]*JtErr[0] + iJtJ[1]*JtErr[1];
        q[1] -= iJtJ[2]*JtErr[0] + iJtJ[3]*JtErr[1];
        //Matx22d J(kx*x + k, kx*y, ky*x, ky*y + k);
        //q -= Vec2d((J.t()*J).inv()*(J.t()*err));
    }

    if( i < maxiter )
    {
        avgiter += i;
        avgn++;
        if( avgn == 1500 )
            printf("avg iters = %g\n", (double)avgiter/avgn);
    }

    return i < maxiter ? fPOINT(q[0], q[1]) : fPOINT(-FLT_MAX, -FLT_MAX);
}

//#include "detail\distortion_model.hpp"

static float initWideAngleProjMap( const img_t* _cameraMatrix0, const img_t* _distCoeffs0,
                            CSize imageSize, int destImageWidth, TypeCnId m1type,
                            img_t* map1, img_t* map2, int projType, double _alpha )
{
    float scale = 0;
    IM_BEGIN(im, 5);
    IM_DEF(tmp, 10);
    img_t *cameraMatrix0 = (img_t*)_cameraMatrix0, *distCoeffs0 = (img_t*)_distCoeffs0;
    double k[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0}, M[9]={0,0,0,0,0,0,0,0,0};
    img_t *distCoeffs = cvMat(tmp++, distCoeffs0->rows, distCoeffs0->cols, CC_64F, CC_MAT_CN(distCoeffs0), k);
    img_t *cameraMatrix = cvMat(tmp++, 3,3,CC_64F,1,M);
    CPoint2f scenter = {(float)*img_at(double, cameraMatrix, 0,2), (float)*img_at(double, cameraMatrix,1,2)};
    CPoint2f dcenter = {(destImageWidth-1)*0.5f, 0.f};
    float xmin = FLT_MAX, xmax = -FLT_MAX, ymin = FLT_MAX, ymax = -FLT_MAX;
    int N = 9;
    CPoint2f uvec_data[1], vvec_data[1];
    img_t* uvec = cvMat(tmp++, 1, 2, CC_32F, 1, uvec_data);
    img_t* vvec = cvMat(tmp++, 1, 2, CC_32F, 1, vvec_data);
    double9 I_data = {0};
    img_t *I = cvMat(tmp++,3,3,CC_64F,1,I_data);
    cvEye(I);
    float alpha = (float)_alpha;

    int ndcoeffs = distCoeffs0->cols*distCoeffs0->rows*CC_MAT_CN(distCoeffs0);
    CC_Assert((distCoeffs0->cols == 1 || distCoeffs0->rows == 1) &&
              (ndcoeffs == 4 || ndcoeffs == 5 || ndcoeffs == 8 || ndcoeffs == 12 || ndcoeffs == 14));
    CC_Assert(CC_ARE_SIZES_EQSZ(cameraMatrix0,3,3));
    cvConvertTo(distCoeffs0,distCoeffs,CC_64F);
    cvConvertTo(cameraMatrix0,cameraMatrix,CC_64F);

    alpha = MIN(alpha, 0.999f);

    for( int i = 0; i < N; i++ ) {
        for( int j = 0; j < N; j++ ) {
            CPoint2f p = fPOINT((float)j*imageSize.width/(N-1), (float)i*imageSize.height/(N-1));
            uvec_data[0] = p;
            undistortPoints(uvec, vvec, cameraMatrix, distCoeffs, I, I);
            CPoint2f q = mapPointSpherical(vvec_data[0], alpha, 0, projType);
            if( xmin > q.x ) xmin = q.x;
            if( xmax < q.x ) xmax = q.x;
            if( ymin > q.y ) ymin = q.y;
            if( ymax < q.y ) ymax = q.y;
        }
    }

    scale = (float)MIN(dcenter.x/fabs(xmax), dcenter.x/fabs(xmin));
    CSize dsize = {destImageWidth, cCeil(MAX(scale*fabs(ymin)*2, scale*fabs(ymax)*2))};
    dcenter.y = (dsize.height - 1)*0.5f;

    img_t *mapxy = cvSetMat(im++, dsize.height, dsize.width, CC_32F, 2);
    double k1 = k[0], k2 = k[1], k3 = k[2], p1 = k[3], p2 = k[4], k4 = k[5], k5 = k[6], k6 = k[7], s1 = k[8], s2 = k[9], s3 = k[10], s4 = k[11];
    double fx = *img_at(double,cameraMatrix,0,0), fy = *img_at(double,cameraMatrix,1,1), cx = scenter.x, cy = scenter.y;
    double9 matTilt = {0};
    computeTiltProjectionMatrix(k[12], k[13], matTilt);

    for( int y = 0; y < dsize.height; y++ )
    {
        CPoint2f* mxy = (CPoint2f*)img_row(float, mapxy, y);
        for( int x = 0; x < dsize.width; x++ )
        {
            CPoint2f p = {(x - dcenter.x)*(1.f/scale), (y - dcenter.y)*(1.f/scale)};
            CPoint2f q = invMapPointSpherical(p, alpha, projType);
            if( q.x <= -FLT_MAX && q.y <= -FLT_MAX )
            {
                mxy[x] = fPOINT(-1.f, -1.f);
                continue;
            }
            double x2 = q.x*q.x, y2 = q.y*q.y;
            double r2 = x2 + y2, _2xy = 2*q.x*q.y;
            double kr = 1 + ((k3*r2 + k2)*r2 + k1)*r2/(1 + ((k6*r2 + k5)*r2 + k4)*r2);
            double xd = (q.x*kr + p1*_2xy + p2*(r2 + 2*x2) + s1*r2+ s2*r2*r2);
            double yd = (q.y*kr + p1*(r2 + 2*y2) + p2*_2xy + s3*r2+ s4*r2*r2);
            double3 vecTilt;
            matmulvec3Impl(vecTilt, matTilt, xd, yd, 1);
            double invProj = vecTilt[2] ? 1./vecTilt[2] : 1;
            double u = fx*invProj*vecTilt[0] + cx;
            double v = fy*invProj*vecTilt[1] + cy;

            mxy[x] = fPOINT((float)u, (float)v);
        }
    }

    if(m1type == CC_32FC2)
    {
        cvCopy(mapxy, map1);
        imfree(map2);
    }
    else {
        convertMaps(mapxy, NULL, map1, map2, m1type, false);
    }

    IM_END(im);
    return scale;
}


