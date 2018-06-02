


#include "fisheye.hpp"

namespace cv { namespace
{
    struct JacobianRow
    {
        Vec2d df, dc;
        Vec4d dk;
        Vec3d dom, dT;
        double dalpha;
    };

    void subMatrix(const img_t& src, img_t& dst, const std::vector<uchar>& cols, const std::vector<uchar>& rows);
}}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// fisheye::projectPoints

void fisheye::projectPoints(const img_t* objectPoints, img_t* imagePoints, const Affine3d& affine,
    const img_t* K, const img_t* D, double alpha, img_t* jacobian)
{
    CC_INSTRUMENT_REGION()

    projectPoints(objectPoints, imagePoints, affine.rvec(), affine.translation(), K, D, alpha, jacobian);
}

void fisheye::projectPoints(const img_t* objectPoints, img_t* imagePoints, const img_t* _rvec,
        const img_t* _tvec, const img_t* _K, const img_t* _D, double alpha, img_t* jacobian)
{
    CC_INSTRUMENT_REGION()

    // will support only 3-channel data now for points
    CC_Assert(objectPoints->tid == CC_32FC3 || objectPoints->tid == CC_64FC3);
    imagePoints cvSetMat(objectPoints.size(), CC_MAKETYPE(objectPoints CC_MAT_DEPTH(), 2));
    size_t n = objectPoints CC_MAT_TOTAL();

    CC_Assert(_rvec CC_MAT_TOTAL() * _rvec CC_MAT_CN() == 3 && (_rvec CC_MAT_DEPTH() == CC_32F || _rvec CC_MAT_DEPTH() == CC_64F));
    CC_Assert(_tvec CC_MAT_TOTAL() * _tvec CC_MAT_CN() == 3 && (_tvec CC_MAT_DEPTH() == CC_32F || _tvec CC_MAT_DEPTH() == CC_64F));
    CC_Assert(_tvec CC_IS_CONT_MAT() && _rvec CC_IS_CONT_MAT());

    Vec3d om = _rvec CC_MAT_DEPTH() == CC_32F ? (Vec3d)*_rvec.ptr<Vec3f>() : *_rvec.ptr<Vec3d>();
    Vec3d T  = _tvec CC_MAT_DEPTH() == CC_32F ? (Vec3d)*_tvec.ptr<Vec3f>() : *_tvec.ptr<Vec3d>();

    CC_Assert(_K.size() == CSize(3,3) && (_K->tid == CC_32F || _K->tid == CC_64F) && _D->tid == _K->tid && _D CC_MAT_TOTAL() == 4);

    Vec2d f, c;
    if (_K CC_MAT_DEPTH() == CC_32F)
    {

        Matx33f K = _K;
        f = Vec2f(K(0, 0), K(1, 1));
        c = Vec2f(K(0, 2), K(1, 2));
    }
    else
    {
        Matx33d K = _K;
        f = Vec2d(K(0, 0), K(1, 1));
        c = Vec2d(K(0, 2), K(1, 2));
    }

    Vec4d k = _D CC_MAT_DEPTH() == CC_32F ? (Vec4d)*_D.ptr<Vec4f>(): *_D.ptr<Vec4d>();

    JacobianRow *Jn = 0;
    if (jacobian.needed())
    {
        int nvars = 2 + 2 + 1 + 4 + 3 + 3; // f, c, alpha, k, om, T,
        jacobian cvSetMat(2*(int)n, nvars, CC_64F);
        Jn = jacobian.ptr<JacobianRow>(0);
    }

    Matx33d R;
    Matx<double, 3, 9> dRdom;
    Rodrigues(om, R, dRdom);
    Affine3d aff(om, T);

    const Vec3f* Xf = objectPoints.ptr<Vec3f>();
    const Vec3d* Xd = objectPoints.ptr<Vec3d>();
    Vec2f *xpf = imagePoints.ptr<Vec2f>();
    Vec2d *xpd = imagePoints.ptr<Vec2d>();

    for(size_t i = 0; i < n; ++i)
    {
        Vec3d Xi = objectPoints CC_MAT_DEPTH() == CC_32F ? (Vec3d)Xf[i] : Xd[i];
        Vec3d Y = aff*Xi;

        Vec2d x(Y[0]/Y[2], Y[1]/Y[2]);

        double r2 = x.dot(x);
        double r = sqrt(r2);

        // Angle of the incoming ray:
        double theta = atan(r);

        double theta2 = theta*theta, theta3 = theta2*theta, theta4 = theta2*theta2, theta5 = theta4*theta,
                theta6 = theta3*theta3, theta7 = theta6*theta, theta8 = theta4*theta4, theta9 = theta8*theta;

        double theta_d = theta + k[0]*theta3 + k[1]*theta5 + k[2]*theta7 + k[3]*theta9;

        double inv_r = r > 1e-8 ? 1.0/r : 1;
        double cdist = r > 1e-8 ? theta_d * inv_r : 1;

        Vec2d xd1 = x * cdist;
        Vec2d xd3(xd1[0] + alpha*xd1[1], xd1[1]);
        Vec2d final_point(xd3[0] * f[0] + c[0], xd3[1] * f[1] + c[1]);

        if (objectPoints CC_MAT_DEPTH() == CC_32F)
            xpf[i] = final_point;
        else
            xpd[i] = final_point;

        if (jacobian.needed())
        {
            //Vec3d Xi = pdepth == CC_32F ? (Vec3d)Xf[i] : Xd[i];
            //Vec3d Y = aff*Xi;
            double dYdR[] = { Xi[0], Xi[1], Xi[2], 0, 0, 0, 0, 0, 0,
                              0, 0, 0, Xi[0], Xi[1], Xi[2], 0, 0, 0,
                              0, 0, 0, 0, 0, 0, Xi[0], Xi[1], Xi[2] };

            Matx33d dYdom_data = Matx<double, 3, 9>(dYdR) * dRdom.t();
            const Vec3d *dYdom = (Vec3d*)dYdom_data.val;

            Matx33d dYdT_data = Matx33d::eye();
            const Vec3d *dYdT = (Vec3d*)dYdT_data.val;

            //Vec2d x(Y[0]/Y[2], Y[1]/Y[2]);
            Vec3d dxdom[2];
            dxdom[0] = (1.0/Y[2]) * dYdom[0] - x[0]/Y[2] * dYdom[2];
            dxdom[1] = (1.0/Y[2]) * dYdom[1] - x[1]/Y[2] * dYdom[2];

            Vec3d dxdT[2];
            dxdT[0]  = (1.0/Y[2]) * dYdT[0] - x[0]/Y[2] * dYdT[2];
            dxdT[1]  = (1.0/Y[2]) * dYdT[1] - x[1]/Y[2] * dYdT[2];

            //double r2 = x.dot(x);
            Vec3d dr2dom = 2 * x[0] * dxdom[0] + 2 * x[1] * dxdom[1];
            Vec3d dr2dT  = 2 * x[0] *  dxdT[0] + 2 * x[1] *  dxdT[1];

            //double r = sqrt(r2);
            double drdr2 = r > 1e-8 ? 1.0/(2*r) : 1;
            Vec3d drdom = drdr2 * dr2dom;
            Vec3d drdT  = drdr2 * dr2dT;

            // Angle of the incoming ray:
            //double theta = atan(r);
            double dthetadr = 1.0/(1+r2);
            Vec3d dthetadom = dthetadr * drdom;
            Vec3d dthetadT  = dthetadr *  drdT;

            //double theta_d = theta + k[0]*theta3 + k[1]*theta5 + k[2]*theta7 + k[3]*theta9;
            double dtheta_ddtheta = 1 + 3*k[0]*theta2 + 5*k[1]*theta4 + 7*k[2]*theta6 + 9*k[3]*theta8;
            Vec3d dtheta_ddom = dtheta_ddtheta * dthetadom;
            Vec3d dtheta_ddT  = dtheta_ddtheta * dthetadT;
            Vec4d dtheta_ddk  = Vec4d(theta3, theta5, theta7, theta9);

            //double inv_r = r > 1e-8 ? 1.0/r : 1;
            //double cdist = r > 1e-8 ? theta_d / r : 1;
            Vec3d dcdistdom = inv_r * (dtheta_ddom - cdist*drdom);
            Vec3d dcdistdT  = inv_r * (dtheta_ddT  - cdist*drdT);
            Vec4d dcdistdk  = inv_r *  dtheta_ddk;

            //Vec2d xd1 = x * cdist;
            Vec4d dxd1dk[2];
            Vec3d dxd1dom[2], dxd1dT[2];
            dxd1dom[0] = x[0] * dcdistdom + cdist * dxdom[0];
            dxd1dom[1] = x[1] * dcdistdom + cdist * dxdom[1];
            dxd1dT[0]  = x[0] * dcdistdT  + cdist * dxdT[0];
            dxd1dT[1]  = x[1] * dcdistdT  + cdist * dxdT[1];
            dxd1dk[0]  = x[0] * dcdistdk;
            dxd1dk[1]  = x[1] * dcdistdk;

            //Vec2d xd3(xd1[0] + alpha*xd1[1], xd1[1]);
            Vec4d dxd3dk[2];
            Vec3d dxd3dom[2], dxd3dT[2];
            dxd3dom[0] = dxd1dom[0] + alpha * dxd1dom[1];
            dxd3dom[1] = dxd1dom[1];
            dxd3dT[0]  = dxd1dT[0]  + alpha * dxd1dT[1];
            dxd3dT[1]  = dxd1dT[1];
            dxd3dk[0]  = dxd1dk[0]  + alpha * dxd1dk[1];
            dxd3dk[1]  = dxd1dk[1];

            Vec2d dxd3dalpha(xd1[1], 0);

            //final jacobian
            Jn[0].dom = f[0] * dxd3dom[0];
            Jn[1].dom = f[1] * dxd3dom[1];

            Jn[0].dT = f[0] * dxd3dT[0];
            Jn[1].dT = f[1] * dxd3dT[1];

            Jn[0].dk = f[0] * dxd3dk[0];
            Jn[1].dk = f[1] * dxd3dk[1];

            Jn[0].dalpha = f[0] * dxd3dalpha[0];
            Jn[1].dalpha = 0; //f[1] * dxd3dalpha[1];

            Jn[0].df = Vec2d(xd3[0], 0);
            Jn[1].df = Vec2d(0, xd3[1]);

            Jn[0].dc = Vec2d(1, 0);
            Jn[1].dc = Vec2d(0, 1);

            //step to jacobian rows for next point
            Jn += 2;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// fisheye::distortPoints

void fisheye::distortPoints(const img_t* undistorted, img_t* distorted, const img_t* K, const img_t* D, double alpha)
{
    CC_INSTRUMENT_REGION()

    // will support only 2-channel data now for points
    CC_Assert(undistorted->tid == CC_32FC2 || undistorted->tid == CC_64FC2);
    distorted cvSetMat(undistorted.size(), undistorted->tid);
    size_t n = undistorted CC_MAT_TOTAL();

    CC_Assert(K.size() == CSize(3,3) && (K->tid == CC_32F || K->tid == CC_64F) && D CC_MAT_TOTAL() == 4);

    Vec2d f, c;
    if (K CC_MAT_DEPTH() == CC_32F)
    {
        Matx33f camMat = K;
        f = Vec2f(camMat(0, 0), camMat(1, 1));
        c = Vec2f(camMat(0, 2), camMat(1, 2));
    }
    else
    {
        Matx33d camMat = K;
        f = Vec2d(camMat(0, 0), camMat(1, 1));
        c = Vec2d(camMat(0 ,2), camMat(1, 2));
    }

    Vec4d k = D CC_MAT_DEPTH() == CC_32F ? (Vec4d)*D.ptr<Vec4f>(): *D.ptr<Vec4d>();

    const Vec2f* Xf = undistorted.ptr<Vec2f>();
    const Vec2d* Xd = undistorted.ptr<Vec2d>();
    Vec2f *xpf = distorted.ptr<Vec2f>();
    Vec2d *xpd = distorted.ptr<Vec2d>();

    for(size_t i = 0; i < n; ++i)
    {
        Vec2d x = undistorted CC_MAT_DEPTH() == CC_32F ? (Vec2d)Xf[i] : Xd[i];

        double r2 = x.dot(x);
        double r = sqrt(r2);

        // Angle of the incoming ray:
        double theta = atan(r);

        double theta2 = theta*theta, theta3 = theta2*theta, theta4 = theta2*theta2, theta5 = theta4*theta,
                theta6 = theta3*theta3, theta7 = theta6*theta, theta8 = theta4*theta4, theta9 = theta8*theta;

        double theta_d = theta + k[0]*theta3 + k[1]*theta5 + k[2]*theta7 + k[3]*theta9;

        double inv_r = r > 1e-8 ? 1.0/r : 1;
        double cdist = r > 1e-8 ? theta_d * inv_r : 1;

        Vec2d xd1 = x * cdist;
        Vec2d xd3(xd1[0] + alpha*xd1[1], xd1[1]);
        Vec2d final_point(xd3[0] * f[0] + c[0], xd3[1] * f[1] + c[1]);

        if (undistorted CC_MAT_DEPTH() == CC_32F)
            xpf[i] = final_point;
        else
            xpd[i] = final_point;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// fisheye::undistortPoints

void fisheye::undistortPoints( const img_t* distorted, img_t* undistorted, const img_t* K, const img_t* D, const img_t* R, const img_t* P)
{
    CC_INSTRUMENT_REGION()

    // will support only 2-channel data now for points
    CC_Assert(distorted->tid == CC_32FC2 || distorted->tid == CC_64FC2);
    undistorted cvSetMat(distorted.size(), distorted->tid);

    CC_Assert(P.empty() || P.size() == CSize(3, 3) || P.size() == CSize(4, 3));
    CC_Assert(R.empty() || R.size() == CSize(3, 3) || R CC_MAT_TOTAL() * R CC_MAT_CN() == 3);
    CC_Assert(D CC_MAT_TOTAL() == 4 && K.size() == CSize(3, 3) && (K CC_MAT_DEPTH() == CC_32F || K CC_MAT_DEPTH() == CC_64F));

    Vec2d f, c;
    if (K CC_MAT_DEPTH() == CC_32F)
    {
        Matx33f camMat = K;
        f = Vec2f(camMat(0, 0), camMat(1, 1));
        c = Vec2f(camMat(0, 2), camMat(1, 2));
    }
    else
    {
        Matx33d camMat = K;
        f = Vec2d(camMat(0, 0), camMat(1, 1));
        c = Vec2d(camMat(0, 2), camMat(1, 2));
    }

    Vec4d k = D CC_MAT_DEPTH() == CC_32F ? (Vec4d)*D.ptr<Vec4f>(): *D.ptr<Vec4d>();

    Matx33d RR = Matx33d::eye();
    if (!R.empty() && R CC_MAT_TOTAL() * R CC_MAT_CN() == 3)
    {
        Vec3d rvec;
        R.convertTo(rvec, CC_64F);
        RR = Affine3d(rvec).rotation();
    }
    else if (!R.empty() && R.size() == CSize(3, 3))
        R.convertTo(RR, CC_64F);

    if(!P.empty())
    {
        Matx33d PP;
        P.colRange(0, 3).convertTo(PP, CC_64F);
        RR = PP * RR;
    }

    // start undistorting
    const Vec2f* srcf = distorted.ptr<Vec2f>();
    const Vec2d* srcd = distorted.ptr<Vec2d>();
    Vec2f* dstf = undistorted.ptr<Vec2f>();
    Vec2d* dstd = undistorted.ptr<Vec2d>();

    size_t n = distorted CC_MAT_TOTAL();
    int sdepth = distorted CC_MAT_DEPTH();

    for(size_t i = 0; i < n; i++ )
    {
        Vec2d pi = sdepth == CC_32F ? (Vec2d)srcf[i] : srcd[i];  // image point
        Vec2d pw((pi[0] - c[0])/f[0], (pi[1] - c[1])/f[1]);      // world point

        double scale = 1.0;

        double theta_d = sqrt(pw[0]*pw[0] + pw[1]*pw[1]);

        // the current camera model is only valid up to 180° FOV
        // for larger FOV the loop below does not converge
        // clip values so we still get plausible results for super fisheye images > 180°
        theta_d = min(max(-CC_PI/2., theta_d), CC_PI/2.);

        if (theta_d > 1e-8)
        {
            // compensate distortion iteratively
            double theta = theta_d;
            for(int j = 0; j < 10; j++ )
            {
                double theta2 = theta*theta, theta4 = theta2*theta2, theta6 = theta4*theta2, theta8 = theta6*theta2;
                theta = theta_d / (1 + k[0] * theta2 + k[1] * theta4 + k[2] * theta6 + k[3] * theta8);
            }

            scale = std::tan(theta) / theta_d;
        }

        Vec2d pu = pw * scale; //undistorted point

        // reproject
        Vec3d pr = RR * Vec3d(pu[0], pu[1], 1.0); // rotated point optionally multiplied by new camera matrix
        Vec2d fi(pr[0]/pr[2], pr[1]/pr[2]);       // final

        if( sdepth == CC_32F )
            dstf[i] = fi;
        else
            dstd[i] = fi;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// fisheye::undistortPoints

void fisheye::initUndistortRectifyMap( const img_t* K, const img_t* D, const img_t* R, const img_t* P,
    const CSize& size, int m1type, img_t* map1, img_t* map2 )
{
    CC_INSTRUMENT_REGION()

    CC_Assert( m1type == CC_16SC2 || m1type == CC_32F || m1type <=0 );
    map1 cvSetMat( size, m1type <= 0 ? CC_16SC2 : m1type );
    map2 cvSetMat( size, map1->tid == CC_16SC2 ? CC_16UC1 : CC_32F );

    CC_Assert((K CC_MAT_DEPTH() == CC_32F || K CC_MAT_DEPTH() == CC_64F) && (D CC_MAT_DEPTH() == CC_32F || D CC_MAT_DEPTH() == CC_64F));
    CC_Assert((P.empty() || P CC_MAT_DEPTH() == CC_32F || P CC_MAT_DEPTH() == CC_64F) && (R.empty() || R CC_MAT_DEPTH() == CC_32F || R CC_MAT_DEPTH() == CC_64F));
    CC_Assert(K.size() == CSize(3, 3) && (D.empty() || D CC_MAT_TOTAL() == 4));
    CC_Assert(R.empty() || R.size() == CSize(3, 3) || R CC_MAT_TOTAL() * R CC_MAT_CN() == 3);
    CC_Assert(P.empty() || P.size() == CSize(3, 3) || P.size() == CSize(4, 3));

    Vec2d f, c;
    if (K CC_MAT_DEPTH() == CC_32F)
    {
        Matx33f camMat = K;
        f = Vec2f(camMat(0, 0), camMat(1, 1));
        c = Vec2f(camMat(0, 2), camMat(1, 2));
    }
    else
    {
        Matx33d camMat = K;
        f = Vec2d(camMat(0, 0), camMat(1, 1));
        c = Vec2d(camMat(0, 2), camMat(1, 2));
    }

    Vec4d k = Vec4d::all(0);
    if (!D.empty())
        k = D CC_MAT_DEPTH() == CC_32F ? (Vec4d)*D.ptr<Vec4f>(): *D.ptr<Vec4d>();

    Matx33d RR  = Matx33d::eye();
    if (!R.empty() && R CC_MAT_TOTAL() * R CC_MAT_CN() == 3)
    {
        Vec3d rvec;
        R.convertTo(rvec, CC_64F);
        RR = Affine3d(rvec).rotation();
    }
    else if (!R.empty() && R.size() == CSize(3, 3))
        R.convertTo(RR, CC_64F);

    Matx33d PP = Matx33d::eye();
    if (!P.empty())
        P.colRange(0, 3).convertTo(PP, CC_64F);

    Matx33d iR = (PP * RR).inv(DECOMP_SVD);

    for( int i = 0; i < size.height; ++i)
    {
        float* m1f = map1.ptr<float>(i);
        float* m2f = map2.ptr<float>(i);
        short*  m1 = (short*)m1f;
        ushort* m2 = (ushort*)m2f;

        double _x = i*iR(0, 1) + iR(0, 2),
               _y = i*iR(1, 1) + iR(1, 2),
               _w = i*iR(2, 1) + iR(2, 2);

        for( int j = 0; j < size.width; ++j)
        {
            double x = _x/_w, y = _y/_w;

            double r = sqrt(x*x + y*y);
            double theta = atan(r);

            double theta2 = theta*theta, theta4 = theta2*theta2, theta6 = theta4*theta2, theta8 = theta4*theta4;
            double theta_d = theta * (1 + k[0]*theta2 + k[1]*theta4 + k[2]*theta6 + k[3]*theta8);

            double scale = (r == 0) ? 1.0 : theta_d / r;
            double u = f[0]*x*scale + c[0];
            double v = f[1]*y*scale + c[1];

            if( m1type == CC_16SC2 )
            {
                int iu = saturate_cast<int>(u*CC_INTER_TAB_SIZE);
                int iv = saturate_cast<int>(v*CC_INTER_TAB_SIZE);
                m1[j*2+0] = (short)(iu >> CC_INTER_BITS);
                m1[j*2+1] = (short)(iv >> CC_INTER_BITS);
                m2[j] = (ushort)((iv & (CC_INTER_TAB_SIZE-1))*CC_INTER_TAB_SIZE + (iu & (CC_INTER_TAB_SIZE-1)));
            }
            else if( m1type == CC_32FC1 )
            {
                m1f[j] = (float)u;
                m2f[j] = (float)v;
            }

            _x += iR(0, 0);
            _y += iR(1, 0);
            _w += iR(2, 0);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// fisheye::undistortImage

void fisheye::undistortImage(const img_t* distorted, img_t* undistorted,
        const img_t* K, const img_t* D, const img_t* Knew, const CSize& new_size)
{
    CC_INSTRUMENT_REGION()

    CSize size = new_size.area() != 0 ? new_size : distorted.size();

    img_t map1, map2;
    fisheye::initUndistortRectifyMap(K, D, Matx33d::eye(), Knew, size, CC_16SC2, map1, map2 );
    remap(distorted, undistorted, map1, map2, CC_INTER_LINEAR, CC_BORDER_CONSTANT);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// fisheye::estimateNewCameraMatrixForUndistortRectify

void fisheye::estimateNewCameraMatrixForUndistortRectify(const img_t* K, const img_t* D, const CSize &image_size, const img_t* R,
    img_t* P, double balance, const CSize& new_size, double fov_scale)
{
    CC_INSTRUMENT_REGION()

    CC_Assert( K.size() == CSize(3, 3)       && (K CC_MAT_DEPTH() == CC_32F || K CC_MAT_DEPTH() == CC_64F));
    CC_Assert(D.empty() || ((D CC_MAT_TOTAL() == 4) && (D CC_MAT_DEPTH() == CC_32F || D CC_MAT_DEPTH() == CC_64F)));

    int w = image_size.width, h = image_size.height;
    balance = MIN(MAX(balance, 0.0), 1.0);

    img_t points(1, 8, CC_64FC2);
    Vec2d* pptr = points.ptr<Vec2d>();
    pptr[0] = Vec2d(0, 0);
    pptr[1] = Vec2d(w/2, 0);
    pptr[2] = Vec2d(w, 0);
    pptr[3] = Vec2d(w, h/2);
    pptr[4] = Vec2d(w, h);
    pptr[5] = Vec2d(w/2, h);
    pptr[6] = Vec2d(0, h);
    pptr[7] = Vec2d(0, h/2);

#if 0
    const int N = 10;
    img_t points(1, N * 4, CC_64FC2);
    Vec2d* pptr = points.ptr<Vec2d>();
    for(int i = 0, k = 0; i < 10; ++i)
    {
        pptr[k++] = Vec2d(w/2,   0) - Vec2d(w/8,   0) + Vec2d(w/4/N*i,   0);
        pptr[k++] = Vec2d(w/2, h-1) - Vec2d(w/8, h-1) + Vec2d(w/4/N*i, h-1);
        pptr[k++] = Vec2d(0,   h/2) - Vec2d(0,   h/8) + Vec2d(0,   h/4/N*i);
        pptr[k++] = Vec2d(w-1, h/2) - Vec2d(w-1, h/8) + Vec2d(w-1, h/4/N*i);
    }
#endif

    fisheye::undistortPoints(points, points, K, D, R);
    Scalar center_mass = mean(points);
    Vec2d cn(center_mass.val);

    double aspect_ratio = (K CC_MAT_DEPTH() == CC_32F) ? K.at<float >(0,0)/K.at<float> (1,1)
                                                : K.at<double>(0,0)/K.at<double>(1,1);

    // convert to identity ratio
    cn[0] *= aspect_ratio;
    for(size_t i = 0; i < points CC_MAT_TOTAL(); ++i)
        pptr[i][1] *= aspect_ratio;

    double minx = DBL_MAX, miny = DBL_MAX, maxx = -DBL_MAX, maxy = -DBL_MAX;
    for(size_t i = 0; i < points CC_MAT_TOTAL(); ++i)
    {
        if(i!=1 && i!=5){
            minx = MIN(minx, std::abs(pptr[i][0]-cn[0]));
        }
        if(i!=3 && i!=7){
            miny = MIN(miny, std::abs(pptr[i][1]-cn[1]));
        }
        maxy = MAX(maxy, std::abs(pptr[i][1]-cn[1]));
        maxx = MAX(maxx, std::abs(pptr[i][0]-cn[0]));
    }

#if 0
    double minx = -DBL_MAX, miny = -DBL_MAX, maxx = DBL_MAX, maxy = DBL_MAX;
    for(size_t i = 0; i < points CC_MAT_TOTAL(); ++i)
    {
        if (i % 4 == 0) miny = MAX(miny, pptr[i][1]);
        if (i % 4 == 1) maxy = MIN(maxy, pptr[i][1]);
        if (i % 4 == 2) minx = MAX(minx, pptr[i][0]);
        if (i % 4 == 3) maxx = MIN(maxx, pptr[i][0]);
    }
#endif

    double f1 = w * 0.5/(minx);
    double f2 = w * 0.5/(maxx);
    double f3 = h * 0.5 * aspect_ratio/(miny);
    double f4 = h * 0.5 * aspect_ratio/(maxy);

    double fmax = MAX(f1, f3);
    double fmin = MIN(f2, f4);

    double f = balance * fmin + (1.0 - balance) * fmax;
    f *= fov_scale > 0 ? 1.0/fov_scale : 1.0;

    Vec2d new_f(f, f), new_c = -cn * f + Vec2d(w, h * aspect_ratio) * 0.5;

    // restore aspect ratio
    new_f[1] /= aspect_ratio;
    new_c[1] /= aspect_ratio;

    if (new_size.area() > 0)
    {
        double rx = new_size.width /(double)image_size.width;
        double ry = new_size.height/(double)image_size.height;

        new_f[0] *= rx;  new_f[1] *= ry;
        new_c[0] *= rx;  new_c[1] *= ry;
    }

    img_t(Matx33d(new_f[0], 0, new_c[0],
                0, new_f[1], new_c[1],
                0,        0,       1)).convertTo(P, P.empty() ? K->tid : P->tid);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// fisheye::stereoRectify

void fisheye::stereoRectify( const img_t* K1, const img_t* D1, const img_t* K2, const img_t* D2, const CSize& imageSize,
        const img_t* _R, const img_t* _tvec, img_t* R1, img_t* R2, img_t* P1, img_t* P2,
        img_t* Q, int flags, const CSize& newImageSize, double balance, double fov_scale)
{
    CC_INSTRUMENT_REGION()

    CC_Assert((_R.size() == CSize(3, 3) || _R CC_MAT_TOTAL() * _R CC_MAT_CN() == 3) && (_R CC_MAT_DEPTH() == CC_32F || _R CC_MAT_DEPTH() == CC_64F));
    CC_Assert(_tvec CC_MAT_TOTAL() * _tvec CC_MAT_CN() == 3 && (_tvec CC_MAT_DEPTH() == CC_32F || _tvec CC_MAT_DEPTH() == CC_64F));


    img_t aaa = _tvec cvReshape(,3, 1);

    Vec3d rvec; // Rodrigues vector
    if (_R.size() == CSize(3, 3))
    {
        Matx33d rmat;
        _R.convertTo(rmat, CC_64F);
        rvec = Affine3d(rmat).rvec();
    }
    else if (_R CC_MAT_TOTAL() * _R CC_MAT_CN() == 3)
        _R.convertTo(rvec, CC_64F);

    Vec3d tvec;
    _tvec.convertTo(tvec, CC_64F);

    // rectification algorithm
    rvec *= -0.5;              // get average rotation

    Matx33d r_r;
    Rodrigues(rvec, r_r);  // rotate cameras to same orientation by averaging

    Vec3d t = r_r * tvec;
    Vec3d uu(t[0] > 0 ? 1 : -1, 0, 0);

    // calculate global Z rotation
    Vec3d ww = t.cross(uu);
    double nw = norm(ww);
    if (nw > 0.0)
        ww *= acos(fabs(t[0])/norm(t))/nw;

    Matx33d wr;
    Rodrigues(ww, wr);

    // apply to both views
    Matx33d ri1 = wr * r_r.t();
    img_t(ri1, false).convertTo(R1, R1.empty() ? CC_64F : R1->tid);
    Matx33d ri2 = wr * r_r;
    img_t(ri2, false).convertTo(R2, R2.empty() ? CC_64F : R2->tid);
    Vec3d tnew = ri2 * tvec;

    // calculate projection/camera matrices. these contain the relevant rectified image internal params (fx, fy=fx, cx, cy)
    Matx33d newK1, newK2;
    estimateNewCameraMatrixForUndistortRectify(K1, D1, imageSize, R1, newK1, balance, newImageSize, fov_scale);
    estimateNewCameraMatrixForUndistortRectify(K2, D2, imageSize, R2, newK2, balance, newImageSize, fov_scale);

    double fc_new = MIN(newK1(1,1), newK2(1,1));
    CPoint2d cc_new[2] = { Vec2d(newK1(0, 2), newK1(1, 2)), Vec2d(newK2(0, 2), newK2(1, 2)) };

    // Vertical focal length must be the same for both images to keep the epipolar constraint use fy for fx also.
    // For simplicity, set the principal points for both cameras to be the average
    // of the two principal points (either one of or both x- and y- coordinates)
    if( flags & CALIB_ZERO_DISPARITY )
        cc_new[0] = cc_new[1] = (cc_new[0] + cc_new[1]) * 0.5;
    else
        cc_new[0].y = cc_new[1].y = (cc_new[0].y + cc_new[1].y)*0.5;

    img_t(Matx34d(fc_new, 0, cc_new[0].x, 0,
                0, fc_new, cc_new[0].y, 0,
                0,      0,           1, 0), false).convertTo(P1, P1.empty() ? CC_64F : P1->tid);

    img_t(Matx34d(fc_new, 0, cc_new[1].x, tnew[0]*fc_new, // baseline * focal length;,
                0, fc_new, cc_new[1].y,              0,
                0,      0,           1,              0), false).convertTo(P2, P2.empty() ? CC_64F : P2->tid);

    if (Q.needed())
        img_t(Matx44d(1, 0, 0,           -cc_new[0].x,
                    0, 1, 0,           -cc_new[0].y,
                    0, 0, 0,            fc_new,
                    0, 0, -1./tnew[0], (cc_new[0].x - cc_new[1].x)/tnew[0]), false).convertTo(Q, Q.empty() ? CC_64F : Q CC_MAT_DEPTH());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// fisheye::calibrate

double fisheye::calibrate(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints, const CSize& image_size,
                                    InputOutputArray K, InputOutputArray D, OutputArrayOfArrays rvecs, OutputArrayOfArrays tvecs,
                                    int flags , TermCriteria criteria)
{
    CC_INSTRUMENT_REGION()

    CC_Assert(!objectPoints.empty() && !imagePoints.empty() && objectPoints CC_MAT_TOTAL() == imagePoints CC_MAT_TOTAL());
    CC_Assert(objectPoints->tid == CC_32FC3 || objectPoints->tid == CC_64FC3);
    CC_Assert(imagePoints->tid == CC_32FC2 || imagePoints->tid == CC_64FC2);
    CC_Assert(K.empty() || (K.size() == CSize(3,3)));
    CC_Assert(D.empty() || (D CC_MAT_TOTAL() == 4));
    CC_Assert(rvecs.empty() || (rvecs CC_MAT_CN() == 3));
    CC_Assert(tvecs.empty() || (tvecs CC_MAT_CN() == 3));

    CC_Assert((!K.empty() && !D.empty()) || !(flags & CALIB_USE_INTRINSIC_GUESS));

    using namespace internal;
    //-------------------------------Initialization
    IntrinsicParams finalParam;
    IntrinsicParams currentParam;
    IntrinsicParams errors;

    finalParam.isEstimate[0] = 1;
    finalParam.isEstimate[1] = 1;
    finalParam.isEstimate[2] = flags & CALIB_FIX_PRINCIPAL_POINT ? 0 : 1;
    finalParam.isEstimate[3] = flags & CALIB_FIX_PRINCIPAL_POINT ? 0 : 1;
    finalParam.isEstimate[4] = flags & CALIB_FIX_SKEW ? 0 : 1;
    finalParam.isEstimate[5] = flags & CALIB_FIX_K1 ? 0 : 1;
    finalParam.isEstimate[6] = flags & CALIB_FIX_K2 ? 0 : 1;
    finalParam.isEstimate[7] = flags & CALIB_FIX_K3 ? 0 : 1;
    finalParam.isEstimate[8] = flags & CALIB_FIX_K4 ? 0 : 1;

    const int recompute_extrinsic = flags & CALIB_RECOMPUTE_EXTRINSIC ? 1: 0;
    const int check_cond = flags & CALIB_CHECK_COND ? 1 : 0;

    const double alpha_smooth = 0.4;
    const double thresh_cond = 1e6;
    double change = 1;
    Vec2d err_std;

    Matx33d _K;
    Vec4d _D;
    if (flags & CALIB_USE_INTRINSIC_GUESS)
    {
        K.convertTo(_K, CC_64FC1);
        D.convertTo(_D, CC_64FC1);
        finalParam.Init(Vec2d(_K(0,0), _K(1, 1)),
                        Vec2d(_K(0,2), _K(1, 2)),
                        Vec4d(flags & CALIB_FIX_K1 ? 0 : _D[0],
                              flags & CALIB_FIX_K2 ? 0 : _D[1],
                              flags & CALIB_FIX_K3 ? 0 : _D[2],
                              flags & CALIB_FIX_K4 ? 0 : _D[3]),
                        _K(0, 1) / _K(0, 0));
    }
    else
    {
        finalParam.Init(Vec2d(max(image_size.width, image_size.height) / CC_PI, max(image_size.width, image_size.height) / CC_PI),
                        Vec2d(image_size.width  / 2.0 - 0.5, image_size.height / 2.0 - 0.5));
    }

    errors.isEstimate = finalParam.isEstimate;

    std::vector<Vec3d> omc(objectPoints CC_MAT_TOTAL()), Tc(objectPoints CC_MAT_TOTAL());

    CalibrateExtrinsics(objectPoints, imagePoints, finalParam, check_cond, thresh_cond, omc, Tc);


    //-------------------------------Optimization
    for(int iter = 0; ; ++iter)
    {
        if ((criteria.type == 1 && iter >= criteria.maxCount)  ||
            (criteria.type == 2 && change <= criteria.epsilon) ||
            (criteria.type == 3 && (change <= criteria.epsilon || iter >= criteria.maxCount)))
            break;

        double alpha_smooth2 = 1 - std::pow(1 - alpha_smooth, iter + 1.0);

        img_t JJ2, ex3;
        ComputeJacobians(objectPoints, imagePoints, finalParam, omc, Tc, check_cond,thresh_cond, JJ2, ex3);

        img_t G;
        solve(JJ2, ex3, G);
        currentParam = finalParam + alpha_smooth2*G;

        change = norm(Vec4d(currentParam.f[0], currentParam.f[1], currentParam.c[0], currentParam.c[1]) -
                Vec4d(finalParam.f[0], finalParam.f[1], finalParam.c[0], finalParam.c[1]))
                / norm(Vec4d(currentParam.f[0], currentParam.f[1], currentParam.c[0], currentParam.c[1]));

        finalParam = currentParam;

        if (recompute_extrinsic)
        {
            CalibrateExtrinsics(objectPoints,  imagePoints, finalParam, check_cond,
                                    thresh_cond, omc, Tc);
        }
    }

    //-------------------------------Validation
    double rms;
    EstimateUncertainties(objectPoints, imagePoints, finalParam,  omc, Tc, errors, err_std, thresh_cond,
                              check_cond, rms);

    //-------------------------------
    _K = Matx33d(finalParam.f[0], finalParam.f[0] * finalParam.alpha, finalParam.c[0],
            0,                    finalParam.f[1], finalParam.c[1],
            0,                                  0,               1);

    if (K.needed()) img_t(_K).convertTo(K, K.empty() ? CC_64FC1 : K->tid);
    if (D.needed()) img_t(finalParam.k).convertTo(D, D.empty() ? CC_64FC1 : D->tid);
    if (rvecs.isMatVector())
    {
        int N = (int)objectPoints CC_MAT_TOTAL();

        if(rvecs.empty())
            rvecs cvSetMat(N, 1, CC_64FC3);

        if(tvecs.empty())
            tvecs cvSetMat(N, 1, CC_64FC3);

        for(int i = 0; i < N; i++ )
        {
            rvecs cvSetMat(3, 1, CC_64F, i, true);
            tvecs cvSetMat(3, 1, CC_64F, i, true);
            memcpy(rvecs.getMat(i)->tt.data, omc[i].val, sizeof(Vec3d));
            memcpy(tvecs.getMat(i)->tt.data, Tc[i].val, sizeof(Vec3d));
        }
    }
    else
    {
        if (rvecs.needed()) img_t(omc).convertTo(rvecs, rvecs.empty() ? CC_64FC3 : rvecs->tid);
        if (tvecs.needed()) img_t(Tc).convertTo(tvecs, tvecs.empty() ? CC_64FC3 : tvecs->tid);
    }

    return rms;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// fisheye::stereoCalibrate

double fisheye::stereoCalibrate(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints1, InputArrayOfArrays imagePoints2,
                                    InputOutputArray K1, InputOutputArray D1, InputOutputArray K2, InputOutputArray D2, CSize imageSize,
                                    img_t* R, img_t* T, int flags, TermCriteria criteria)
{
    CC_INSTRUMENT_REGION()

    CC_Assert(!objectPoints.empty() && !imagePoints1.empty() && !imagePoints2.empty());
    CC_Assert(objectPoints CC_MAT_TOTAL() == imagePoints1 CC_MAT_TOTAL() || imagePoints1 CC_MAT_TOTAL() == imagePoints2 CC_MAT_TOTAL());
    CC_Assert(objectPoints->tid == CC_32FC3 || objectPoints->tid == CC_64FC3);
    CC_Assert(imagePoints1->tid == CC_32FC2 || imagePoints1->tid == CC_64FC2);
    CC_Assert(imagePoints2->tid == CC_32FC2 || imagePoints2->tid == CC_64FC2);

    CC_Assert(K1.empty() || (K1.size() == CSize(3,3)));
    CC_Assert(D1.empty() || (D1 CC_MAT_TOTAL() == 4));
    CC_Assert(K2.empty() || (K1.size() == CSize(3,3)));
    CC_Assert(D2.empty() || (D1 CC_MAT_TOTAL() == 4));

    CC_Assert((!K1.empty() && !K2.empty() && !D1.empty() && !D2.empty()) || !(flags & CALIB_FIX_INTRINSIC));

    //-------------------------------Initialization

    const int threshold = 50;
    const double thresh_cond = 1e6;
    const int check_cond = 1;

    int n_points = (int)objectPoints.getMat(0) CC_MAT_TOTAL();
    int n_images = (int)objectPoints CC_MAT_TOTAL();

    double change = 1;

    internal::IntrinsicParams intrinsicLeft;
    internal::IntrinsicParams intrinsicRight;

    internal::IntrinsicParams intrinsicLeft_errors;
    internal::IntrinsicParams intrinsicRight_errors;

    Matx33d _K1, _K2;
    Vec4d _D1, _D2;
    if (!K1.empty()) K1.convertTo(_K1, CC_64FC1);
    if (!D1.empty()) D1.convertTo(_D1, CC_64FC1);
    if (!K2.empty()) K2.convertTo(_K2, CC_64FC1);
    if (!D2.empty()) D2.convertTo(_D2, CC_64FC1);

    std::vector<Vec3d> rvecs1(n_images), tvecs1(n_images), rvecs2(n_images), tvecs2(n_images);

    if (!(flags & CALIB_FIX_INTRINSIC))
    {
        calibrate(objectPoints, imagePoints1, imageSize, _K1, _D1, rvecs1, tvecs1, flags, TermCriteria(3, 20, 1e-6));
        calibrate(objectPoints, imagePoints2, imageSize, _K2, _D2, rvecs2, tvecs2, flags, TermCriteria(3, 20, 1e-6));
    }

    intrinsicLeft.Init(Vec2d(_K1(0,0), _K1(1, 1)), Vec2d(_K1(0,2), _K1(1, 2)),
                       Vec4d(_D1[0], _D1[1], _D1[2], _D1[3]), _K1(0, 1) / _K1(0, 0));

    intrinsicRight.Init(Vec2d(_K2(0,0), _K2(1, 1)), Vec2d(_K2(0,2), _K2(1, 2)),
                        Vec4d(_D2[0], _D2[1], _D2[2], _D2[3]), _K2(0, 1) / _K2(0, 0));

    if ((flags & CALIB_FIX_INTRINSIC))
    {
        internal::CalibrateExtrinsics(objectPoints,  imagePoints1, intrinsicLeft, check_cond, thresh_cond, rvecs1, tvecs1);
        internal::CalibrateExtrinsics(objectPoints,  imagePoints2, intrinsicRight, check_cond, thresh_cond, rvecs2, tvecs2);
    }

    intrinsicLeft.isEstimate[0] = flags & CALIB_FIX_INTRINSIC ? 0 : 1;
    intrinsicLeft.isEstimate[1] = flags & CALIB_FIX_INTRINSIC ? 0 : 1;
    intrinsicLeft.isEstimate[2] = flags & CALIB_FIX_INTRINSIC ? 0 : 1;
    intrinsicLeft.isEstimate[3] = flags & CALIB_FIX_INTRINSIC ? 0 : 1;
    intrinsicLeft.isEstimate[4] = flags & (CALIB_FIX_SKEW | CALIB_FIX_INTRINSIC) ? 0 : 1;
    intrinsicLeft.isEstimate[5] = flags & (CALIB_FIX_K1 | CALIB_FIX_INTRINSIC) ? 0 : 1;
    intrinsicLeft.isEstimate[6] = flags & (CALIB_FIX_K2 | CALIB_FIX_INTRINSIC) ? 0 : 1;
    intrinsicLeft.isEstimate[7] = flags & (CALIB_FIX_K3 | CALIB_FIX_INTRINSIC) ? 0 : 1;
    intrinsicLeft.isEstimate[8] = flags & (CALIB_FIX_K4 | CALIB_FIX_INTRINSIC) ? 0 : 1;

    intrinsicRight.isEstimate[0] = flags & CALIB_FIX_INTRINSIC ? 0 : 1;
    intrinsicRight.isEstimate[1] = flags & CALIB_FIX_INTRINSIC ? 0 : 1;
    intrinsicRight.isEstimate[2] = flags & CALIB_FIX_INTRINSIC ? 0 : 1;
    intrinsicRight.isEstimate[3] = flags & CALIB_FIX_INTRINSIC ? 0 : 1;
    intrinsicRight.isEstimate[4] = flags & (CALIB_FIX_SKEW | CALIB_FIX_INTRINSIC) ? 0 : 1;
    intrinsicRight.isEstimate[5] = flags & (CALIB_FIX_K1 | CALIB_FIX_INTRINSIC) ? 0 : 1;
    intrinsicRight.isEstimate[6] = flags & (CALIB_FIX_K2 | CALIB_FIX_INTRINSIC) ? 0 : 1;
    intrinsicRight.isEstimate[7] = flags & (CALIB_FIX_K3 | CALIB_FIX_INTRINSIC) ? 0 : 1;
    intrinsicRight.isEstimate[8] = flags & (CALIB_FIX_K4 | CALIB_FIX_INTRINSIC) ? 0 : 1;

    intrinsicLeft_errors.isEstimate = intrinsicLeft.isEstimate;
    intrinsicRight_errors.isEstimate = intrinsicRight.isEstimate;

    std::vector<uchar> selectedParams;
    std::vector<int> tmp(6 * (n_images + 1), 1);
    selectedParams.insert(selectedParams.end(), intrinsicLeft.isEstimate.begin(), intrinsicLeft.isEstimate.end());
    selectedParams.insert(selectedParams.end(), intrinsicRight.isEstimate.begin(), intrinsicRight.isEstimate.end());
    selectedParams.insert(selectedParams.end(), tmp.begin(), tmp.end());

    //Init values for rotation and translation between two views
    img_t om_list(1, n_images, CC_64FC3), T_list(1, n_images, CC_64FC3);
    img_t om_ref, R_ref, T_ref, R1, R2;
    for (int image_idx = 0; image_idx < n_images; ++image_idx)
    {
        Rodrigues(rvecs1[image_idx], R1);
        Rodrigues(rvecs2[image_idx], R2);
        R_ref = R2 * R1.t();
        T_ref = img_t(tvecs2[image_idx]) - R_ref * img_t(tvecs1[image_idx]);
        Rodrigues(R_ref, om_ref);
        om_ref cvReshape(,3, 1).copyTo(om_list.col(image_idx));
        T_ref cvReshape(,3, 1).copyTo(T_list.col(image_idx));
    }
    Vec3d omcur = internal::median3d(om_list);
    Vec3d Tcur  = internal::median3d(T_list);

    img_t J = img_t::zeros(4 * n_points * n_images, 18 + 6 * (n_images + 1), CC_64FC1),
            e = img_t::zeros(4 * n_points * n_images, 1, CC_64FC1), Jkk, ekk;

    for(int iter = 0; ; ++iter)
    {
        if ((criteria.type == 1 && iter >= criteria.maxCount)  ||
            (criteria.type == 2 && change <= criteria.epsilon) ||
            (criteria.type == 3 && (change <= criteria.epsilon || iter >= criteria.maxCount)))
            break;

        J cvSetMat(4 * n_points * n_images, 18 + 6 * (n_images + 1), CC_64FC1);
        e cvSetMat(4 * n_points * n_images, 1, CC_64FC1);
        Jkk cvSetMat(4 * n_points, 18 + 6 * (n_images + 1), CC_64FC1);
        ekk cvSetMat(4 * n_points, 1, CC_64FC1);

        img_t omr, Tr, domrdomckk, domrdTckk, domrdom, domrdT, dTrdomckk, dTrdTckk, dTrdom, dTrdT;

        for (int image_idx = 0; image_idx < n_images; ++image_idx)
        {
            Jkk = img_t::zeros(4 * n_points, 18 + 6 * (n_images + 1), CC_64FC1);

            img_t object  = objectPoints.getMat(image_idx).clone();
            img_t imageLeft  = imagePoints1.getMat(image_idx).clone();
            img_t imageRight  = imagePoints2.getMat(image_idx).clone();
            img_t jacobians, projected;

            //left camera jacobian
            img_t rvec = img_t(rvecs1[image_idx]);
            img_t tvec  = img_t(tvecs1[image_idx]);
            internal::projectPoints(object, projected, rvec, tvec, intrinsicLeft, jacobians);
            img_t(img_t((imageLeft - projected).t()) cvReshape(,1, 1).t()).copyTo(ekk cvGetRows(,0, 2 * n_points));
            jacobians.colRange(8, 11).copyTo(Jkk.colRange(24 + image_idx * 6, 27 + image_idx * 6) cvGetRows(,0, 2 * n_points));
            jacobians.colRange(11, 14).copyTo(Jkk.colRange(27 + image_idx * 6, 30 + image_idx * 6) cvGetRows(,0, 2 * n_points));
            jacobians.colRange(0, 2).copyTo(Jkk.colRange(0, 2) cvGetRows(,0, 2 * n_points));
            jacobians.colRange(2, 4).copyTo(Jkk.colRange(2, 4) cvGetRows(,0, 2 * n_points));
            jacobians.colRange(4, 8).copyTo(Jkk.colRange(5, 9) cvGetRows(,0, 2 * n_points));
            jacobians.col(14).copyTo(Jkk.col(4) cvGetRows(,0, 2 * n_points));

            //right camera jacobian
            internal::compose_motion(rvec, tvec, omcur, Tcur, omr, Tr, domrdomckk, domrdTckk, domrdom, domrdT, dTrdomckk, dTrdTckk, dTrdom, dTrdT);
            rvec = img_t(rvecs2[image_idx]);
            tvec  = img_t(tvecs2[image_idx]);

            internal::projectPoints(object, projected, omr, Tr, intrinsicRight, jacobians);
            img_t(img_t((imageRight - projected).t()) cvReshape(,1, 1).t()).copyTo(ekk cvGetRows(,2 * n_points, 4 * n_points));
            img_t dxrdom = jacobians.colRange(8, 11) * domrdom + jacobians.colRange(11, 14) * dTrdom;
            img_t dxrdT = jacobians.colRange(8, 11) * domrdT + jacobians.colRange(11, 14)* dTrdT;
            img_t dxrdomckk = jacobians.colRange(8, 11) * domrdomckk + jacobians.colRange(11, 14) * dTrdomckk;
            img_t dxrdTckk = jacobians.colRange(8, 11) * domrdTckk + jacobians.colRange(11, 14) * dTrdTckk;

            dxrdom.copyTo(Jkk.colRange(18, 21) cvGetRows(,2 * n_points, 4 * n_points));
            dxrdT.copyTo(Jkk.colRange(21, 24) cvGetRows(,2 * n_points, 4 * n_points));
            dxrdomckk.copyTo(Jkk.colRange(24 + image_idx * 6, 27 + image_idx * 6) cvGetRows(,2 * n_points, 4 * n_points));
            dxrdTckk.copyTo(Jkk.colRange(27 + image_idx * 6, 30 + image_idx * 6) cvGetRows(,2 * n_points, 4 * n_points));
            jacobians.colRange(0, 2).copyTo(Jkk.colRange(9 + 0, 9 + 2) cvGetRows(,2 * n_points, 4 * n_points));
            jacobians.colRange(2, 4).copyTo(Jkk.colRange(9 + 2, 9 + 4) cvGetRows(,2 * n_points, 4 * n_points));
            jacobians.colRange(4, 8).copyTo(Jkk.colRange(9 + 5, 9 + 9) cvGetRows(,2 * n_points, 4 * n_points));
            jacobians.col(14).copyTo(Jkk.col(9 + 4) cvGetRows(,2 * n_points, 4 * n_points));

            //check goodness of sterepair
            double abs_max  = 0;
            for (int i = 0; i < 4 * n_points; i++)
            {
                if (fabs(ekk.at<double>(i)) > abs_max)
                {
                    abs_max = fabs(ekk.at<double>(i));
                }
            }

            CC_Assert(abs_max < threshold); // bad stereo pair

            Jkk.copyTo(J cvGetRows(,image_idx * 4 * n_points, (image_idx + 1) * 4 * n_points));
            ekk.copyTo(e cvGetRows(,image_idx * 4 * n_points, (image_idx + 1) * 4 * n_points));
        }

        Vec6d oldTom(Tcur[0], Tcur[1], Tcur[2], omcur[0], omcur[1], omcur[2]);

        //update all parameters
        subMatrix(J, J, selectedParams, std::vector<uchar>(J->rows, 1));
        int a = countNonZero(intrinsicLeft.isEstimate);
        int b = countNonZero(intrinsicRight.isEstimate);
        img_t deltas;
        solve(J.t() * J, J.t()*e, deltas);
        if (a > 0)
            intrinsicLeft = intrinsicLeft + deltas cvGetRows(,0, a);
        if (b > 0)
            intrinsicRight = intrinsicRight + deltas cvGetRows(,a, a + b);
        omcur = omcur + Vec3d(deltas cvGetRows(,a + b, a + b + 3));
        Tcur = Tcur + Vec3d(deltas cvGetRows(,a + b + 3, a + b + 6));
        for (int image_idx = 0; image_idx < n_images; ++image_idx)
        {
            rvecs1[image_idx] = img_t(img_t(rvecs1[image_idx]) + deltas cvGetRows(,a + b + 6 + image_idx * 6, a + b + 9 + image_idx * 6));
            tvecs1[image_idx] = img_t(img_t(tvecs1[image_idx]) + deltas cvGetRows(,a + b + 9 + image_idx * 6, a + b + 12 + image_idx * 6));
        }

        Vec6d newTom(Tcur[0], Tcur[1], Tcur[2], omcur[0], omcur[1], omcur[2]);
        change = norm(newTom - oldTom) / norm(newTom);
    }

    double rms = 0;
    const Vec2d* ptr_e = e.ptr<Vec2d>();
    for (size_t i = 0; i < e CC_MAT_TOTAL() / 2; i++)
    {
        rms += ptr_e[i][0] * ptr_e[i][0] + ptr_e[i][1] * ptr_e[i][1];
    }

    rms /= ((double)e CC_MAT_TOTAL() / 2.0);
    rms = sqrt(rms);

    _K1 = Matx33d(intrinsicLeft.f[0], intrinsicLeft.f[0] * intrinsicLeft.alpha, intrinsicLeft.c[0],
                                       0,                       intrinsicLeft.f[1], intrinsicLeft.c[1],
                                       0,                                        0,                 1);

    _K2 = Matx33d(intrinsicRight.f[0], intrinsicRight.f[0] * intrinsicRight.alpha, intrinsicRight.c[0],
                                        0,                        intrinsicRight.f[1], intrinsicRight.c[1],
                                        0,                                          0,                  1);

    img_t _R;
    Rodrigues(omcur, _R);

    if (K1.needed()) img_t(_K1).convertTo(K1, K1.empty() ? CC_64FC1 : K1->tid);
    if (K2.needed()) img_t(_K2).convertTo(K2, K2.empty() ? CC_64FC1 : K2->tid);
    if (D1.needed()) img_t(intrinsicLeft.k).convertTo(D1, D1.empty() ? CC_64FC1 : D1->tid);
    if (D2.needed()) img_t(intrinsicRight.k).convertTo(D2, D2.empty() ? CC_64FC1 : D2->tid);
    if (R.needed()) _R.convertTo(R, R.empty() ? CC_64FC1 : R->tid);
    if (T.needed()) img_t(Tcur).convertTo(T, T.empty() ? CC_64FC1 : T->tid);

    return rms;
}

namespace cv{ namespace {
void subMatrix(const img_t& src, img_t& dst, const std::vector<uchar>& cols, const std::vector<uchar>& rows)
{
    CC_Assert(src CC_MAT_CN() == 1);

    int nonzeros_cols = countNonZero(cols);
    img_t tmp(src->rows, nonzeros_cols, CC_64F);

    for (int i = 0, j = 0; i < (int)cols.size(); i++)
    {
        if (cols[i])
        {
            src.col(i).copyTo(tmp.col(j++));
        }
    }

    int nonzeros_rows  = countNonZero(rows);
    dst cvSetMat(nonzeros_rows, nonzeros_cols, CC_64F);
    for (int i = 0, j = 0; i < (int)rows.size(); i++)
    {
        if (rows[i])
        {
            tmp.row(i).copyTo(dst.row(j++));
        }
    }
}

}}

internal::IntrinsicParams::IntrinsicParams():
    f(Vec2d::all(0)), c(Vec2d::all(0)), k(Vec4d::all(0)), alpha(0), isEstimate(9,0)
{
}

internal::IntrinsicParams::IntrinsicParams(Vec2d _f, Vec2d _c, Vec4d _k, double _alpha):
    f(_f), c(_c), k(_k), alpha(_alpha), isEstimate(9,0)
{
}

internal::IntrinsicParams internal::IntrinsicParams::operator+(const img_t& a)
{
    CC_Assert(a->tid == CC_64FC1);
    IntrinsicParams tmp;
    const double* ptr = a img_ptr(double, );

    int j = 0;
    tmp.f[0]    = this->f[0]    + (isEstimate[0] ? ptr[j++] : 0);
    tmp.f[1]    = this->f[1]    + (isEstimate[1] ? ptr[j++] : 0);
    tmp.c[0]    = this->c[0]    + (isEstimate[2] ? ptr[j++] : 0);
    tmp.alpha   = this->alpha   + (isEstimate[4] ? ptr[j++] : 0);
    tmp.c[1]    = this->c[1]    + (isEstimate[3] ? ptr[j++] : 0);
    tmp.k[0]    = this->k[0]    + (isEstimate[5] ? ptr[j++] : 0);
    tmp.k[1]    = this->k[1]    + (isEstimate[6] ? ptr[j++] : 0);
    tmp.k[2]    = this->k[2]    + (isEstimate[7] ? ptr[j++] : 0);
    tmp.k[3]    = this->k[3]    + (isEstimate[8] ? ptr[j++] : 0);

    tmp.isEstimate = isEstimate;
    return tmp;
}

internal::IntrinsicParams& internal::IntrinsicParams::operator =(const img_t& a)
{
    CC_Assert(a->tid == CC_64FC1);
    const double* ptr = a img_ptr(double, );

    int j = 0;

    this->f[0]  = isEstimate[0] ? ptr[j++] : 0;
    this->f[1]  = isEstimate[1] ? ptr[j++] : 0;
    this->c[0]  = isEstimate[2] ? ptr[j++] : 0;
    this->c[1]  = isEstimate[3] ? ptr[j++] : 0;
    this->alpha = isEstimate[4] ? ptr[j++] : 0;
    this->k[0]  = isEstimate[5] ? ptr[j++] : 0;
    this->k[1]  = isEstimate[6] ? ptr[j++] : 0;
    this->k[2]  = isEstimate[7] ? ptr[j++] : 0;
    this->k[3]  = isEstimate[8] ? ptr[j++] : 0;

    return *this;
}

void internal::IntrinsicParams::Init(const Vec2d& _f, const Vec2d& _c, const Vec4d& _k, const double& _alpha)
{
    this->c = _c;
    this->f = _f;
    this->k = _k;
    this->alpha = _alpha;
}

void internal::projectPoints(const img_t* objectPoints, img_t* imagePoints,
                   const img_t* _rvec,const img_t* _tvec,
                   const IntrinsicParams& param, img_t* jacobian)
{
    CC_INSTRUMENT_REGION()

    CC_Assert(!objectPoints.empty() && objectPoints->tid == CC_64FC3);
    Matx33d K(param.f[0], param.f[0] * param.alpha, param.c[0],
                       0,               param.f[1], param.c[1],
                       0,                        0,         1);
    fisheye::projectPoints(objectPoints, imagePoints, _rvec, _tvec, K, param.k, param.alpha, jacobian);
}

void internal::ComputeExtrinsicRefine(const img_t& imagePoints, const img_t& objectPoints, img_t& rvec,
                            img_t&  tvec, img_t& J, const int MaxIter,
                            const IntrinsicParams& param, const double thresh_cond)
{
    CC_Assert(!objectPoints.empty() && objectPoints->tid == CC_64FC3);
    CC_Assert(!imagePoints.empty() && imagePoints->tid == CC_64FC2);
    Vec6d extrinsics(rvec.at<double>(0), rvec.at<double>(1), rvec.at<double>(2),
                    tvec.at<double>(0), tvec.at<double>(1), tvec.at<double>(2));
    double change = 1;
    int iter = 0;

    while (change > 1e-10 && iter < MaxIter)
    {
        std::vector<CPoint2d> x;
        img_t jacobians;
        projectPoints(objectPoints, x, rvec, tvec, param, jacobians);

        img_t ex = imagePoints - img_t(x).t();
        ex = ex cvReshape(,1, 2);

        J = jacobians.colRange(8, 14).clone();

        CSVD svd(J, CSVD::NO_UV);
        double condJJ = svd.w.at<double>(0)/svd.w.at<double>(5);

        if (condJJ > thresh_cond)
            change = 0;
        else
        {
            Vec6d param_innov;
            solve(J, ex cvReshape(,1, (int)ex CC_MAT_TOTAL()), param_innov, DECOMP_SVD + CC_DECOMP_NORMAL);

            Vec6d param_up = extrinsics + param_innov;
            change = norm(param_innov)/norm(param_up);
            extrinsics = param_up;
            iter = iter + 1;

            rvec = img_t(Vec3d(extrinsics.val));
            tvec = img_t(Vec3d(extrinsics.val+3));
        }
    }
}

img_t internal::ComputeHomography(img_t m, img_t M)
{
    CC_INSTRUMENT_REGION()

    int Np = m->cols;

    if (m->rows < 3)
    {
        vconcat(m, img_t::ones(1, Np, CC_64FC1), m);
    }
    if (M->rows < 3)
    {
        vconcat(M, img_t::ones(1, Np, CC_64FC1), M);
    }

    divide(m, img_t::ones(3, 1, CC_64FC1) * m.row(2), m);
    divide(M, img_t::ones(3, 1, CC_64FC1) * M.row(2), M);

    img_t ax = m.row(0).clone();
    img_t ay = m.row(1).clone();

    double mxx = mean(ax)[0];
    double myy = mean(ay)[0];

    ax = ax - mxx;
    ay = ay - myy;

    double scxx = mean(abs(ax))[0];
    double scyy = mean(abs(ay))[0];

    img_t Hnorm (Matx33d( 1/scxx,        0.0,     -mxx/scxx,
                         0.0,     1/scyy,     -myy/scyy,
                         0.0,        0.0,           1.0 ));

    img_t inv_Hnorm (Matx33d( scxx,     0,   mxx,
                                    0,  scyy,   myy,
                                    0,     0,     1 ));
    img_t mn =  Hnorm * m;

    img_t L = img_t::zeros(2*Np, 9, CC_64FC1);

    for (int i = 0; i < Np; ++i)
    {
        for (int j = 0; j < 3; j++)
        {
            L.at<double>(2 * i, j) = M.at<double>(j, i);
            L.at<double>(2 * i + 1, j + 3) = M.at<double>(j, i);
            L.at<double>(2 * i, j + 6) = -mn.at<double>(0,i) * M.at<double>(j, i);
            L.at<double>(2 * i + 1, j + 6) = -mn.at<double>(1,i) * M.at<double>(j, i);
        }
    }

    if (Np > 4) L = L.t() * L;
    CSVD svd(L);
    img_t hh = svd.vt.row(8) / svd.vt.row(8).at<double>(8);
    img_t Hrem = hh cvReshape(,1, 3);
    img_t H = inv_Hnorm * Hrem;

    if (Np > 4)
    {
        img_t hhv = H cvReshape(,1, 9)(Rect(0, 0, 1, 8)).clone();
        for (int iter = 0; iter < 10; iter++)
        {
            img_t mrep = H * M;
            img_t J = img_t::zeros(2 * Np, 8, CC_64FC1);
            img_t MMM;
            divide(M, img_t::ones(3, 1, CC_64FC1) * mrep(Rect(0, 2, mrep->cols, 1)), MMM);
            divide(mrep, img_t::ones(3, 1, CC_64FC1) * mrep(Rect(0, 2, mrep->cols, 1)), mrep);
            img_t m_err = m(Rect(0,0, m->cols, 2)) - mrep(Rect(0,0, mrep->cols, 2));
            m_err = img_t(m_err.t()) cvReshape(,1, m_err->cols * m_err->rows);
            img_t MMM2, MMM3;
            multiply(img_t::ones(3, 1, CC_64FC1) * mrep(Rect(0, 0, mrep->cols, 1)), MMM, MMM2);
            multiply(img_t::ones(3, 1, CC_64FC1) * mrep(Rect(0, 1, mrep->cols, 1)), MMM, MMM3);

            for (int i = 0; i < Np; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    J.at<double>(2 * i, j)         = -MMM.at<double>(j, i);
                    J.at<double>(2 * i + 1, j + 3) = -MMM.at<double>(j, i);
                }

                for (int j = 0; j < 2; ++j)
                {
                    J.at<double>(2 * i, j + 6)     = MMM2.at<double>(j, i);
                    J.at<double>(2 * i + 1, j + 6) = MMM3.at<double>(j, i);
                }
            }
            divide(M, img_t::ones(3, 1, CC_64FC1) * mrep(Rect(0,2,mrep->cols,1)), MMM);
            img_t hh_innov = (J.t() * J).inv() * (J.t()) * m_err;
            img_t hhv_up = hhv - hh_innov;
            img_t tmp;
            vconcat(hhv_up, img_t::ones(1,1,CC_64FC1), tmp);
            img_t H_up = tmp cvReshape(,1,3);
            hhv = hhv_up;
            H = H_up;
        }
    }
    return H;
}

img_t internal::NormalizePixels(const img_t& imagePoints, const IntrinsicParams& param)
{
    CC_INSTRUMENT_REGION()

    CC_Assert(!imagePoints.empty() && imagePoints->tid == CC_64FC2);

    img_t distorted((int)imagePoints CC_MAT_TOTAL(), 1, CC_64FC2), undistorted;
    const Vec2d* ptr   = imagePoints.ptr<Vec2d>();
    Vec2d* ptr_d = distorted.ptr<Vec2d>();
    for (size_t i = 0; i < imagePoints CC_MAT_TOTAL(); ++i)
    {
        ptr_d[i] = (ptr[i] - param.c).mul(Vec2d(1.0 / param.f[0], 1.0 / param.f[1]));
        ptr_d[i][0] -= param.alpha * ptr_d[i][1];
    }
    fisheye::undistortPoints(distorted, undistorted, Matx33d::eye(), param.k);
    return undistorted;
}

void internal::InitExtrinsics(const img_t& _imagePoints, const img_t& _objectPoints, const IntrinsicParams& param, img_t& omckk, img_t& Tckk)
{
    CC_Assert(!_objectPoints.empty() && _objectPoints->tid == CC_64FC3);
    CC_Assert(!_imagePoints.empty() && _imagePoints->tid == CC_64FC2);

    img_t imagePointsNormalized = NormalizePixels(_imagePoints, param) cvReshape(,1).t();
    img_t objectPoints = _objectPoints cvReshape(,1).t();
    img_t objectPointsMean, covObjectPoints;
    img_t Rckk;
    int Np = imagePointsNormalized->cols;
    calcCovarMatrix(objectPoints, covObjectPoints, objectPointsMean, COVAR_NORMAL | COVAR_COLS);
    CSVD svd(covObjectPoints);
    img_t R(svd.vt);
    if (norm(R(Rect(2, 0, 1, 2))) < 1e-6)
        R = img_t::eye(3,3, CC_64FC1);
    if (determinant(R) < 0)
        R = -R;
    img_t T = -R * objectPointsMean;
    img_t X_new = R * objectPoints + T * img_t::ones(1, Np, CC_64FC1);
    img_t H = ComputeHomography(imagePointsNormalized, X_new(Rect(0,0,X_new->cols,2)));
    double sc = .5 * (norm(H.col(0)) + norm(H.col(1)));
    H = H / sc;
    img_t u1 = H.col(0).clone();
    u1  = u1 / norm(u1);
    img_t u2 = H.col(1).clone() - u1.dot(H.col(1).clone()) * u1;
    u2 = u2 / norm(u2);
    img_t u3 = u1.cross(u2);
    img_t RRR;
    hconcat(u1, u2, RRR);
    hconcat(RRR, u3, RRR);
    Rodrigues(RRR, omckk);
    Rodrigues(omckk, Rckk);
    Tckk = H.col(2).clone();
    Tckk = Tckk + Rckk * T;
    Rckk = Rckk * R;
    Rodrigues(Rckk, omckk);
}

void internal::CalibrateExtrinsics(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints,
                         const IntrinsicParams& param, const int check_cond,
                         const double thresh_cond, InputOutputArray omc, InputOutputArray Tc)
{
    CC_Assert(!objectPoints.empty() && (objectPoints->tid == CC_32FC3 || objectPoints->tid == CC_64FC3));
    CC_Assert(!imagePoints.empty() && (imagePoints->tid == CC_32FC2 || imagePoints->tid == CC_64FC2));
    CC_Assert(omc->tid == CC_64FC3 || Tc->tid == CC_64FC3);

    if (omc.empty()) omc cvSetMat(1, (int)objectPoints CC_MAT_TOTAL(), CC_64FC3);
    if (Tc.empty()) Tc cvSetMat(1, (int)objectPoints CC_MAT_TOTAL(), CC_64FC3);

    const int maxIter = 20;

    for(int image_idx = 0; image_idx < (int)imagePoints CC_MAT_TOTAL(); ++image_idx)
    {
        img_t omckk, Tckk, JJ_kk;
        img_t image, object;

        objectPoints.getMat(image_idx).convertTo(object,  CC_64FC3);
        imagePoints.getMat (image_idx).convertTo(image, CC_64FC2);

        bool imT = image->rows < image->cols;
        bool obT = object->rows < object->cols;

        InitExtrinsics(imT ? image.t() : image, obT ? object.t() : object, param, omckk, Tckk);

        ComputeExtrinsicRefine(!imT ? image.t() : image, !obT ? object.t() : object, omckk, Tckk, JJ_kk, maxIter, param, thresh_cond);
        if (check_cond)
        {
            CSVD svd(JJ_kk, CSVD::NO_UV);
            CC_Assert(svd.w.at<double>(0) / svd.w.at<double>((int)svd.w CC_MAT_TOTAL() - 1) < thresh_cond);
        }
        omckk cvReshape(,3,1).copyTo(omc.col(image_idx));
        Tckk cvReshape(,3,1).copyTo(Tc.col(image_idx));
    }
}

void internal::ComputeJacobians(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints,
                      const IntrinsicParams& param,  const img_t* omc, const img_t* Tc,
                      const int& check_cond, const double& thresh_cond, img_t& JJ2, img_t& ex3)
{
    CC_Assert(!objectPoints.empty() && (objectPoints->tid == CC_32FC3 || objectPoints->tid == CC_64FC3));
    CC_Assert(!imagePoints.empty() && (imagePoints->tid == CC_32FC2 || imagePoints->tid == CC_64FC2));

    CC_Assert(!omc.empty() && omc->tid == CC_64FC3);
    CC_Assert(!Tc.empty() && Tc->tid == CC_64FC3);

    int n = (int)objectPoints CC_MAT_TOTAL();

    JJ2 = img_t::zeros(9 + 6 * n, 9 + 6 * n, CC_64FC1);
    ex3 = img_t::zeros(9 + 6 * n, 1, CC_64FC1 );

    for (int image_idx = 0; image_idx < n; ++image_idx)
    {
        img_t image, object;
        objectPoints.getMat(image_idx).convertTo(object, CC_64FC3);
        imagePoints.getMat (image_idx).convertTo(image, CC_64FC2);

        bool imT = image->rows < image->cols;
        img_t om(omc.col(image_idx)), T(Tc.col(image_idx));

        std::vector<CPoint2d> x;
        img_t jacobians;
        projectPoints(object, x, om, T, param, jacobians);
        img_t exkk = (imT ? image.t() : image) - img_t(x);

        img_t A(jacobians->rows, 9, CC_64FC1);
        jacobians.colRange(0, 4).copyTo(A.colRange(0, 4));
        jacobians.col(14).copyTo(A.col(4));
        jacobians.colRange(4, 8).copyTo(A.colRange(5, 9));

        A = A.t();

        img_t B = jacobians.colRange(8, 14).clone();
        B = B.t();

        JJ2(Rect(0, 0, 9, 9)) += A * A.t();
        JJ2(Rect(9 + 6 * image_idx, 9 + 6 * image_idx, 6, 6)) = B * B.t();

        JJ2(Rect(9 + 6 * image_idx, 0, 6, 9)) = A * B.t();
        JJ2(Rect(0, 9 + 6 * image_idx, 9, 6)) = JJ2(Rect(9 + 6 * image_idx, 0, 6, 9)).t();

        ex3 cvGetRows(,0, 9) += A * exkk cvReshape(,1, 2 * exkk->rows);
        ex3 cvGetRows(,9 + 6 * image_idx, 9 + 6 * (image_idx + 1)) = B * exkk cvReshape(,1, 2 * exkk->rows);

        if (check_cond)
        {
            img_t JJ_kk = B.t();
            CSVD svd(JJ_kk, CSVD::NO_UV);
            CC_Assert(svd.w.at<double>(0) / svd.w.at<double>(svd.w->rows - 1) < thresh_cond);
        }
    }

    std::vector<uchar> idxs(param.isEstimate);
    idxs.insert(idxs.end(), 6 * n, 1);

    subMatrix(JJ2, JJ2, idxs, idxs);
    subMatrix(ex3, ex3, std::vector<uchar>(1, 1), idxs);
}

void internal::EstimateUncertainties(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints,
                           const IntrinsicParams& params, const img_t* omc, const img_t* Tc,
                           IntrinsicParams& errors, Vec2d& std_err, double thresh_cond, int check_cond, double& rms)
{
    CC_Assert(!objectPoints.empty() && (objectPoints->tid == CC_32FC3 || objectPoints->tid == CC_64FC3));
    CC_Assert(!imagePoints.empty() && (imagePoints->tid == CC_32FC2 || imagePoints->tid == CC_64FC2));

    CC_Assert(!omc.empty() && omc->tid == CC_64FC3);
    CC_Assert(!Tc.empty() && Tc->tid == CC_64FC3);

    int total_ex = 0;
    for (int image_idx = 0; image_idx < (int)objectPoints CC_MAT_TOTAL(); ++image_idx)
    {
        total_ex += (int)objectPoints.getMat(image_idx) CC_MAT_TOTAL();
    }
    img_t ex(total_ex, 1, CC_64FC2);
    int insert_idx = 0;
    for (int image_idx = 0; image_idx < (int)objectPoints CC_MAT_TOTAL(); ++image_idx)
    {
        img_t image, object;
        objectPoints.getMat(image_idx).convertTo(object, CC_64FC3);
        imagePoints.getMat (image_idx).convertTo(image, CC_64FC2);

        bool imT = image->rows < image->cols;

        img_t om(omc.col(image_idx)), T(Tc.col(image_idx));

        std::vector<CPoint2d> x;
        projectPoints(object, x, om, T, params, NULL);
        img_t ex_ = (imT ? image.t() : image) - img_t(x);
        ex_.copyTo(ex cvGetRows(,insert_idx, insert_idx + ex_->rows));
        insert_idx += ex_->rows;
    }

    meanStdDev(ex, NULL, std_err);
    std_err *= sqrt((double)ex CC_MAT_TOTAL()/((double)ex CC_MAT_TOTAL() - 1.0));

    Vec<double, 1> sigma_x;
    meanStdDev(ex cvReshape(,1, 1), NULL, sigma_x);
    sigma_x  *= sqrt(2.0 * (double)ex CC_MAT_TOTAL()/(2.0 * (double)ex CC_MAT_TOTAL() - 1.0));

    img_t JJ2, ex3;
    ComputeJacobians(objectPoints, imagePoints, params, omc, Tc, check_cond, thresh_cond, JJ2, ex3);

    sqrt(JJ2.inv(), JJ2);

    errors = 3 * sigma_x(0) * JJ2.diag();
    rms = sqrt(norm(ex, NORM_L2SQR)/ex CC_MAT_TOTAL());
}

void internal::dAB(const img_t* A, const img_t* B, img_t* dABdA, img_t* dABdB)
{
    CC_Assert(A->cols == B->rows);
    CC_Assert(A->tid == CC_64FC1 && B->tid == CC_64FC1);

    int p = A->rows;
    int n = A->cols;
    int q = B->cols;

    dABdA cvSetMat(p * q, p * n, CC_64FC1);
    dABdB cvSetMat(p * q, q * n, CC_64FC1);

    dABdA = img_t::zeros(p * q, p * n, CC_64FC1);
    dABdB = img_t::zeros(p * q, q * n, CC_64FC1);

    for (int i = 0; i < q; ++i)
    {
        for (int j = 0; j < p; ++j)
        {
            int ij = j + i * p;
            for (int k = 0; k < n; ++k)
            {
                int kj = j + k * p;
                dABdA.at<double>(ij, kj) = B.at<double>(k, i);
            }
        }
    }

    for (int i = 0; i < q; ++i)
    {
        A.copyTo(dABdB cvGetRows(,i * p, i * p + p).colRange(i * n, i * n + n));
    }
}

void internal::JRodriguesMatlab(const img_t& src, img_t& dst)
{
    img_t tmp(src->cols, src->rows, src->tid);
    if (src->rows == 9)
    {
        img_t(src.row(0).t()).copyTo(tmp.col(0));
        img_t(src.row(1).t()).copyTo(tmp.col(3));
        img_t(src.row(2).t()).copyTo(tmp.col(6));
        img_t(src.row(3).t()).copyTo(tmp.col(1));
        img_t(src.row(4).t()).copyTo(tmp.col(4));
        img_t(src.row(5).t()).copyTo(tmp.col(7));
        img_t(src.row(6).t()).copyTo(tmp.col(2));
        img_t(src.row(7).t()).copyTo(tmp.col(5));
        img_t(src.row(8).t()).copyTo(tmp.col(8));
    }
    else
    {
        img_t(src.col(0).t()).copyTo(tmp.row(0));
        img_t(src.col(1).t()).copyTo(tmp.row(3));
        img_t(src.col(2).t()).copyTo(tmp.row(6));
        img_t(src.col(3).t()).copyTo(tmp.row(1));
        img_t(src.col(4).t()).copyTo(tmp.row(4));
        img_t(src.col(5).t()).copyTo(tmp.row(7));
        img_t(src.col(6).t()).copyTo(tmp.row(2));
        img_t(src.col(7).t()).copyTo(tmp.row(5));
        img_t(src.col(8).t()).copyTo(tmp.row(8));
    }
    dst = tmp.clone();
}

void internal::compose_motion(const img_t* _om1, const img_t* _T1, const img_t* _om2, const img_t* _T2,
                    img_t& om3, img_t& T3, img_t& dom3dom1, img_t& dom3dT1, img_t& dom3dom2,
                    img_t& dom3dT2, img_t& dT3dom1, img_t& dT3dT1, img_t& dT3dom2, img_t& dT3dT2)
{
    img_t om1 = _om1;
    img_t om2 = _om2;
    img_t T1 = _T1 cvReshape(,1, 3);
    img_t T2 = _T2 cvReshape(,1, 3);

    //% Rotations:
    img_t R1, R2, R3, dR1dom1(9, 3, CC_64FC1), dR2dom2;
    Rodrigues(om1, R1, dR1dom1);
    Rodrigues(om2, R2, dR2dom2);
    JRodriguesMatlab(dR1dom1, dR1dom1);
    JRodriguesMatlab(dR2dom2, dR2dom2);
    R3 = R2 * R1;
    img_t dR3dR2, dR3dR1;
    dAB(R2, R1, dR3dR2, dR3dR1);
    img_t dom3dR3;
    Rodrigues(R3, om3, dom3dR3);
    JRodriguesMatlab(dom3dR3, dom3dR3);
    dom3dom1 = dom3dR3 * dR3dR1 * dR1dom1;
    dom3dom2 = dom3dR3 * dR3dR2 * dR2dom2;
    dom3dT1 = img_t::zeros(3, 3, CC_64FC1);
    dom3dT2 = img_t::zeros(3, 3, CC_64FC1);

    //% Translations:
    img_t T3t = R2 * T1;
    img_t dT3tdR2, dT3tdT1;
    dAB(R2, T1, dT3tdR2, dT3tdT1);
    img_t dT3tdom2 = dT3tdR2 * dR2dom2;
    T3 = T3t + T2;
    dT3dT1 = dT3tdT1;
    dT3dT2 = img_t::eye(3, 3, CC_64FC1);
    dT3dom2 = dT3tdom2;
    dT3dom1 = img_t::zeros(3, 3, CC_64FC1);
}

double internal::median(const img_t& row)
{
    CC_Assert(row->tid == CC_64FC1);
    CC_Assert(!row.empty() && row->rows == 1);
    img_t tmp = row.clone();
    sort(tmp, tmp, 0);
    if ((int)tmp CC_MAT_TOTAL() % 2) return tmp.at<double>((int)tmp CC_MAT_TOTAL() / 2);
    else return 0.5 *(tmp.at<double>((int)tmp CC_MAT_TOTAL() / 2) + tmp.at<double>((int)tmp CC_MAT_TOTAL() / 2 - 1));
}

Vec3d internal::median3d(const img_t* m)
{
    CC_Assert(m CC_MAT_DEPTH() == CC_64F && m->rows == 1);
    img_t M = img_t(m.t()) cvReshape(,1).t();
    return Vec3d(median(M.row(0)), median(M.row(1)), median(M.row(2)));
}
