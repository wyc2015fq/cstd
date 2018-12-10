
#if 0
//#include "precomp.hpp"

const float EPS = 1.0e-4f;

static void findCircle3pts(FPoint *pts, FPoint &center, float &radius)
{
    // two edges of the triangle v1, v2
    FPoint v1 = pts[1] - pts[0];
    FPoint v2 = pts[2] - pts[0];

    // center is intersection of midperpendicular lines of the two edges v1, v2
    // a1*x + b1*y = c1 where a1 = v1.x, b1 = v1.y
    // a2*x + b2*y = c2 where a2 = v2.x, b2 = v2.y
    FPoint midPoint1 = (pts[0] + pts[1]) / 2.0f;
    float c1 = midPoint1.x * v1.x + midPoint1.y * v1.y;
    FPoint midPoint2 = (pts[0] + pts[2]) / 2.0f;
    float c2 = midPoint2.x * v2.x + midPoint2.y * v2.y;
    float det = v1.x * v2.y - v1.y * v2.x;
    float cx = (c1 * v2.y - c2 * v1.y) / det;
    float cy = (v1.x * c2 - v2.x * c1) / det;
    center.x = (float)cx;
    center.y = (float)cy;
    cx -= pts[0].x;
    cy -= pts[0].y;
    radius = (float)(std::sqrt(cx *cx + cy * cy)) + EPS;
}

template<typename PT>
static void findThirdPoint(const PT *pts, int i, int j, FPoint &center, float &radius)
{
    center.x = (float)(pts[j].x + pts[i].x) / 2.0f;
    center.y = (float)(pts[j].y + pts[i].y) / 2.0f;
    float dx = (float)(pts[j].x - pts[i].x);
    float dy = (float)(pts[j].y - pts[i].y);
    radius = (float)norm(fPoint(dx, dy)) / 2.0f + EPS;

    for (int k = 0; k < j; ++k)
    {
        dx = center.x - (float)pts[k].x;
        dy = center.y - (float)pts[k].y;
        if (norm(fPoint(dx, dy)) < radius)
        {
            continue;
        }
        else
        {
            FPoint ptsf[3];
            ptsf[0] = (FPoint)pts[i];
            ptsf[1] = (FPoint)pts[j];
            ptsf[2] = (FPoint)pts[k];
            findCircle3pts(ptsf, center, radius);
        }
    }
}


template<typename PT>
void findSecondPoint(const PT *pts, int i, FPoint &center, float &radius)
{
    center.x = (float)(pts[0].x + pts[i].x) / 2.0f;
    center.y = (float)(pts[0].y + pts[i].y) / 2.0f;
    float dx = (float)(pts[0].x - pts[i].x);
    float dy = (float)(pts[0].y - pts[i].y);
    radius = (float)norm(fPoint(dx, dy)) / 2.0f + EPS;

    for (int j = 1; j < i; ++j)
    {
        dx = center.x - (float)pts[j].x;
        dy = center.y - (float)pts[j].y;
        if (norm(fPoint(dx, dy)) < radius)
        {
            continue;
        }
        else
        {
            findThirdPoint(pts, i, j, center, radius);
        }
    }
}


template<typename PT>
static void findMinEnclosingCircle(const PT *pts, int count, FPoint &center, float &radius)
{
    center.x = (float)(pts[0].x + pts[1].x) / 2.0f;
    center.y = (float)(pts[0].y + pts[1].y) / 2.0f;
    float dx = (float)(pts[0].x - pts[1].x);
    float dy = (float)(pts[0].y - pts[1].y);
    radius = (float)norm(fPoint(dx, dy)) / 2.0f + EPS;

    for (int i = 2; i < count; ++i)
    {
        dx = (float)pts[i].x - center.x;
        dy = (float)pts[i].y - center.y;
        float d = (float)norm(fPoint(dx, dy));
        if (d < radius)
        {
            continue;
        }
        else
        {
            findSecondPoint(pts, i, center, radius);
        }
    }
}


// see Welzl, Emo. Smallest enclosing disks (balls and ellipsoids). Springer Berlin Heidelberg, 1991.
void minEnclosingCircle( const img_t* _points, FPoint& _center, float& _radius )
{
    CC_INSTRUMENT_REGION()

    img_t*  points = _points->getMat();
    int count = points->checkVector(2);
    int depth = points->depth();
    CC_Assert(count >= 0 && (depth == CC_32F || depth == CC_32S));

    _center.x = _center.y = 0.f;
    _radius = 0.f;

    if( count == 0 )
        return;

    bool is_float = depth == CC_32F;
    const IPoint* ptsi = points->ptr<IPoint>();
    const FPoint* ptsf = points->ptr<FPoint>();

    switch (count)
    {
        case 1:
        {
            _center = (is_float) ? ptsf[0] : fPoint((float)ptsi[0].x, (float)ptsi[0].y);
            _radius = EPS;
            break;
        }
        case 2:
        {
            FPoint p1 = (is_float) ? ptsf[0] : fPoint((float)ptsi[0].x, (float)ptsi[0].y);
            FPoint p2 = (is_float) ? ptsf[1] : fPoint((float)ptsi[1].x, (float)ptsi[1].y);
            _center.x = (p1.x + p2.x) / 2.0f;
            _center.y = (p1.y + p2.y) / 2.0f;
            _radius = (float)(norm(p1 - p2) / 2.0) + EPS;
            break;
        }
        default:
        {
            FPoint center;
            float radius = 0.f;
            if (is_float)
            {
                findMinEnclosingCircle<FPoint>(ptsf, count, center, radius);
                #if 0
                    for (size_t m = 0; m < count; ++m)
                    {
                        float d = (float)norm(ptsf[m] - center);
                        if (d > radius)
                        {
                            printf("error!\n");
                        }
                    }
                #endif
            }
            else
            {
                findMinEnclosingCircle<IPoint>(ptsi, count, center, radius);
                #if 0
                    for (size_t m = 0; m < count; ++m)
                    {
                        double dx = ptsi[m].x - center.x;
                        double dy = ptsi[m].y - center.y;
                        double d = std::sqrt(dx * dx + dy * dy);
                        if (d > radius)
                        {
                            printf("error!\n");
                        }
                    }
                #endif
            }
            _center = center;
            _radius = radius;
            break;
        }
    }
}


// calculates length of a curve (e.g. contour perimeter)
double arcLength( const img_t* _curve, bool is_closed )
{
    CC_INSTRUMENT_REGION()

    img_t*  curve = _curve.getMat();
    int count = curve.checkVector(2);
    int depth = curve.depth();
    CC_Assert( count >= 0 && (depth == CC_32F || depth == CC_32S));
    double perimeter = 0;

    int i;

    if( count <= 1 )
        return 0.;

    bool is_float = depth == CC_32F;
    int last = is_closed ? count-1 : 0;
    const IPoint* pti = curve.ptr<IPoint>();
    const FPoint* ptf = curve.ptr<FPoint>();

    FPoint prev = is_float ? ptf[last] : fPoint((float)pti[last].x,(float)pti[last].y);

    for( i = 0; i < count; i++ )
    {
        FPoint p = is_float ? ptf[i] : fPoint((float)pti[i].x,(float)pti[i].y);
        float dx = p.x - prev.x, dy = p.y - prev.y;
        perimeter += std::sqrt(dx*dx + dy*dy);

        prev = p;
    }

    return perimeter;
}

// area of a whole sequence
double contourArea( const img_t* _contour, bool oriented )
{
    CC_INSTRUMENT_REGION()

    img_t*  contour = _contour.getMat();
    int npoints = contour.checkVector(2);
    int depth = contour.depth();
    CC_Assert(npoints >= 0 && (depth == CC_32F || depth == CC_32S));

    if( npoints == 0 )
        return 0.;

    double a00 = 0;
    bool is_float = depth == CC_32F;
    const IPoint* ptsi = contour.ptr<IPoint>();
    const FPoint* ptsf = contour.ptr<FPoint>();
    FPoint prev = is_float ? ptsf[npoints-1] : fPoint((float)ptsi[npoints-1].x, (float)ptsi[npoints-1].y);

    for( int i = 0; i < npoints; i++ )
    {
        FPoint p = is_float ? ptsf[i] : fPoint((float)ptsi[i].x, (float)ptsi[i].y);
        a00 += (double)prev.x * p.y - (double)prev.y * p.x;
        prev = p;
    }

    a00 *= 0.5;
    if( !oriented )
        a00 = fabs(a00);

    return a00;
}
#endif

CRotatedRect fitEllipse( const img_t* points)
{
    CC_INSTRUMENT_REGION()

    int i, n = points->checkVector(2);
    int depth = points->depth();
    CC_Assert( n >= 0 && (depth == CC_32F || depth == CC_32S));

    CRotatedRect box;

    if( n < 5 )
        CC_Error( CC_StsBadSize, "There should be at least 5 points to fit the ellipse" );

    // New fitellipse algorithm, contributed by Dr. Daniel Weiss
    FPoint c = fPoint(0,0);
    double gfp[5] = {0}, rp[5] = {0}, t;
    const double min_eps = 1e-8;
    bool is_float = depth == CC_32F;
    const IPoint* ptsi = img_ptr(IPoint, points);
    const FPoint* ptsf = img_ptr(FPoint, points);
    img_t tmp_[10];
    img_t* tmp = tmp_;
    //AutoBuffer<double> _Ad(n*5), _bd(n);
    double* _Ad = MALLOC(double, n * 5);
    double* _bd = MALLOC(double, n);
    double *Ad = _Ad, *bd = _bd;

    // first fit for parameters A - E
    img_t*  A = imcreate2(tmp++, n, 5, CC_64F, Ad );
    img_t*  b = imcreate2(tmp++, n, 1, CC_64F, bd );
    img_t*  x = imcreate2(tmp++, 5, 1, CC_64F, gfp );

    for( i = 0; i < n; i++ )
    {
        FPoint p = is_float ? ptsf[i] : fPoint((float)ptsi[i].x, (float)ptsi[i].y);
        c += p;
    }
    c.x /= n;
    c.y /= n;

    for( i = 0; i < n; i++ )
    {
        FPoint p = is_float ? ptsf[i] : fPoint((float)ptsi[i].x, (float)ptsi[i].y);
        p -= c;

        bd[i] = 10000.0; // 1.0?
        Ad[i*5] = -(double)p.x * p.x; // A - C signs inverted as proposed by APP
        Ad[i*5 + 1] = -(double)p.y * p.y;
        Ad[i*5 + 2] = -(double)p.x * p.y;
        Ad[i*5 + 3] = p.x;
        Ad[i*5 + 4] = p.y;
    }

    cv:solve(A, b, x, DECOMP_SVD);

    // now use general-form parameters A - E to find the ellipse center:
    // differentiate general form wrt x/y to get two equations for cx and cy
    A = img_t* ( 2, 2, CC_64F, Ad );
    b = img_t* ( 2, 1, CC_64F, bd );
    x = img_t* ( 2, 1, CC_64F, rp );
    Ad[0] = 2 * gfp[0];
    Ad[1] = Ad[2] = gfp[2];
    Ad[3] = 2 * gfp[1];
    bd[0] = gfp[3];
    bd[1] = gfp[4];
    solve( A, b, x, DECOMP_SVD );

    // re-fit for parameters A - C with those center coordinates
    A = img_t* ( n, 3, CC_64F, Ad );
    b = img_t* ( n, 1, CC_64F, bd );
    x = img_t* ( 3, 1, CC_64F, gfp );
    for( i = 0; i < n; i++ )
    {
        FPoint p = is_float ? ptsf[i] : fPoint((float)ptsi[i].x, (float)ptsi[i].y);
        p -= c;
        bd[i] = 1.0;
        Ad[i * 3] = (p.x - rp[0]) * (p.x - rp[0]);
        Ad[i * 3 + 1] = (p.y - rp[1]) * (p.y - rp[1]);
        Ad[i * 3 + 2] = (p.x - rp[0]) * (p.y - rp[1]);
    }
    solve(A, b, x, DECOMP_SVD);

    // store angle and radii
    rp[4] = -0.5 * atan2(gfp[2], gfp[1] - gfp[0]); // convert from APP angle usage
    if( fabs(gfp[2]) > min_eps )
        t = gfp[2]/sin(-2.0 * rp[4]);
    else // ellipse is rotated by an integer multiple of pi/2
        t = gfp[1] - gfp[0];
    rp[2] = fabs(gfp[0] + gfp[1] - t);
    if( rp[2] > min_eps )
        rp[2] = std::sqrt(2.0 / rp[2]);
    rp[3] = fabs(gfp[0] + gfp[1] + t);
    if( rp[3] > min_eps )
        rp[3] = std::sqrt(2.0 / rp[3]);

    box.center.x = (float)rp[0] + c.x;
    box.center.y = (float)rp[1] + c.y;
    box.size.width = (float)(rp[2]*2);
    box.size.height = (float)(rp[3]*2);
    if( box.size.width > box.size.height )
    {
        float tmp;
        CC_SWAP( box.size.width, box.size.height, tmp );
        box.angle = (float)(90 + rp[4]*180/CC_PI);
    }
    if( box.angle < -180 )
        box.angle += 360;
    if( box.angle > 360 )
        box.angle -= 360;

    FREE(_Ad);
    FREE(_bd);
    return box;
}

#if 0

CRotatedRect fitEllipseAMS( const img_t* _points )
{
    img_t*  points = _points->getMat();
    int i, n = points->checkVector(2);
    int depth = points->depth();
    CC_Assert( n >= 0 && (depth == CC_32F || depth == CC_32S));

    CRotatedRect box;

    if( n < 5 )
        CC_Error( CC_StsBadSize, "There should be at least 5 points to fit the ellipse" );

    FPoint c(0,0);

    bool is_float = depth == CC_32F;
    const IPoint* ptsi = points->ptr<IPoint>();
    const FPoint* ptsf = points->ptr<FPoint>();

    img_t*  A( n, 6, CC_64F);
    Matx<double, 6, 6> DM;
    Matx<double, 5, 5> M;
    Matx<double, 5, 1> pVec;
    Matx<double, 6, 1> coeffs;

    double x0, y0, a, b, theta;

    for( i = 0; i < n; i++ )
    {
        FPoint p = is_float ? ptsf[i] : fPoint((float)ptsi[i].x, (float)ptsi[i].y);
        c += p;
    }
    c.x /= (float)n;
    c.y /= (float)n;

    for( i = 0; i < n; i++ )
    {
        FPoint p = is_float ? ptsf[i] : fPoint((float)ptsi[i].x, (float)ptsi[i].y);
        p -= c;

        A.at<double>(i,0) = (double)(p.x)*(p.x);
        A.at<double>(i,1) = (double)(p.x)*(p.y);
        A.at<double>(i,2) = (double)(p.y)*(p.y);
        A.at<double>(i,3) = (double)p.x;
        A.at<double>(i,4) = (double)p.y;
        A.at<double>(i,5) = 1.0;
    }
    mulTransposed( A, DM, true, noArray(), 1.0, -1 );
    DM *= (1.0/n);
    double dnm = ( DM(2,5)*(DM(0,5) + DM(2,5)) - (DM(1,5)*DM(1,5)) );
    double ddm =  (4.*(DM(0,5) + DM(2,5))*( (DM(0,5)*DM(2,5)) - (DM(1,5)*DM(1,5))));
    double ddmm = (2.*(DM(0,5) + DM(2,5))*( (DM(0,5)*DM(2,5)) - (DM(1,5)*DM(1,5))));

    M(0,0)=((-DM(0,0) + DM(0,2) + DM(0,5)*DM(0,5))*(DM(1,5)*DM(1,5)) + (-2*DM(0,1)*DM(1,5) + DM(0,5)*(DM(0,0) \
            - (DM(0,5)*DM(0,5)) + (DM(1,5)*DM(1,5))))*DM(2,5) + (DM(0,0) - (DM(0,5)*DM(0,5)))*(DM(2,5)*DM(2,5))) / ddm;
    M(0,1)=((DM(1,5)*DM(1,5))*(-DM(0,1) + DM(1,2) + DM(0,5)*DM(1,5)) + (DM(0,1)*DM(0,5) - ((DM(0,5)*DM(0,5)) + 2*DM(1,1))*DM(1,5) + \
            (DM(1,5)*DM(1,5)*DM(1,5)))*DM(2,5) + (DM(0,1) - DM(0,5)*DM(1,5))*(DM(2,5)*DM(2,5))) / ddm;
    M(0,2)=(-2*DM(1,2)*DM(1,5)*DM(2,5) - DM(0,5)*(DM(2,5)*DM(2,5))*(DM(0,5) + DM(2,5)) + DM(0,2)*dnm + \
            (DM(1,5)*DM(1,5))*(DM(2,2) + DM(2,5)*(DM(0,5) + DM(2,5))))/ddm;
    M(0,3)=(DM(1,5)*(DM(1,5)*DM(2,3) - 2*DM(1,3)*DM(2,5)) + DM(0,3)*dnm) / ddm;
    M(0,4)=(DM(1,5)*(DM(1,5)*DM(2,4) - 2*DM(1,4)*DM(2,5)) + DM(0,4)*dnm) / ddm;
    M(1,0)=(-(DM(0,2)*DM(0,5)*DM(1,5)) + (2*DM(0,1)*DM(0,5) - DM(0,0)*DM(1,5))*DM(2,5))/ddmm;
    M(1,1)=(-(DM(0,1)*DM(1,5)*DM(2,5)) + DM(0,5)*(-(DM(1,2)*DM(1,5)) + 2*DM(1,1)*DM(2,5)))/ddmm;
    M(1,2)=(-(DM(0,2)*DM(1,5)*DM(2,5)) + DM(0,5)*(-(DM(1,5)*DM(2,2)) + 2*DM(1,2)*DM(2,5)))/ddmm;
    M(1,3)=(-(DM(0,3)*DM(1,5)*DM(2,5)) + DM(0,5)*(-(DM(1,5)*DM(2,3)) + 2*DM(1,3)*DM(2,5)))/ddmm;
    M(1,4)=(-(DM(0,4)*DM(1,5)*DM(2,5)) + DM(0,5)*(-(DM(1,5)*DM(2,4)) + 2*DM(1,4)*DM(2,5)))/ddmm;
    M(2,0)=(-2*DM(0,1)*DM(0,5)*DM(1,5) + (DM(0,0) + (DM(0,5)*DM(0,5)))*(DM(1,5)*DM(1,5)) + DM(0,5)*(-(DM(0,5)*DM(0,5)) \
            + (DM(1,5)*DM(1,5)))*DM(2,5) - (DM(0,5)*DM(0,5))*(DM(2,5)*DM(2,5)) + DM(0,2)*(-(DM(1,5)*DM(1,5)) + DM(0,5)*(DM(0,5) + DM(2,5)))) / ddm;
    M(2,1)=((DM(0,5)*DM(0,5))*(DM(1,2) - DM(1,5)*DM(2,5)) + (DM(1,5)*DM(1,5))*(DM(0,1) - DM(1,2) + DM(1,5)*DM(2,5)) \
            + DM(0,5)*(DM(1,2)*DM(2,5) + DM(1,5)*(-2*DM(1,1) + (DM(1,5)*DM(1,5)) - (DM(2,5)*DM(2,5))))) / ddm;
    M(2,2)=((DM(0,5)*DM(0,5))*(DM(2,2) - (DM(2,5)*DM(2,5))) + (DM(1,5)*DM(1,5))*(DM(0,2) - DM(2,2) + (DM(2,5)*DM(2,5))) + \
             DM(0,5)*(-2*DM(1,2)*DM(1,5) + DM(2,5)*((DM(1,5)*DM(1,5)) + DM(2,2) - (DM(2,5)*DM(2,5))))) / ddm;
    M(2,3)=((DM(1,5)*DM(1,5))*(DM(0,3) - DM(2,3)) + (DM(0,5)*DM(0,5))*DM(2,3) + DM(0,5)*(-2*DM(1,3)*DM(1,5) + DM(2,3)*DM(2,5))) / ddm;
    M(2,4)=((DM(1,5)*DM(1,5))*(DM(0,4) - DM(2,4)) + (DM(0,5)*DM(0,5))*DM(2,4) + DM(0,5)*(-2*DM(1,4)*DM(1,5) + DM(2,4)*DM(2,5))) / ddm;
    M(3,0)=DM(0,3);
    M(3,1)=DM(1,3);
    M(3,2)=DM(2,3);
    M(3,3)=DM(3,3);
    M(3,4)=DM(3,4);
    M(4,0)=DM(0,4);
    M(4,1)=DM(1,4);
    M(4,2)=DM(2,4);
    M(4,3)=DM(3,4);
    M(4,4)=DM(4,4);

    if (fabs(determinant(M)) > 1.0e-10) {
            img_t*  eVal, eVec;
            eigenNonSymmetric(M, eVal, eVec);

            // Select the eigen vector {a,b,c,d,e} which has the lowest eigenvalue
            int minpos = 0;
            double normi, normEVali, normMinpos, normEValMinpos;
            normMinpos = sqrt(eVec.at<double>(minpos,0)*eVec.at<double>(minpos,0) + eVec.at<double>(minpos,1)*eVec.at<double>(minpos,1) + \
                              eVec.at<double>(minpos,2)*eVec.at<double>(minpos,2) + eVec.at<double>(minpos,3)*eVec.at<double>(minpos,3) + \
                              eVec.at<double>(minpos,4)*eVec.at<double>(minpos,4) );
            normEValMinpos = eVal.at<double>(minpos,0) * normMinpos;
            for (i=1; i<5; i++) {
                normi = sqrt(eVec.at<double>(i,0)*eVec.at<double>(i,0) + eVec.at<double>(i,1)*eVec.at<double>(i,1) + \
                             eVec.at<double>(i,2)*eVec.at<double>(i,2) + eVec.at<double>(i,3)*eVec.at<double>(i,3) + \
                             eVec.at<double>(i,4)*eVec.at<double>(i,4) );
                normEVali = eVal.at<double>(i,0) * normi;
                if (normEVali < normEValMinpos) {
                    minpos = i;
                    normMinpos=normi;
                    normEValMinpos=normEVali;
                }
            };

            pVec(0) =eVec.at<double>(minpos,0) / normMinpos;
            pVec(1) =eVec.at<double>(minpos,1) / normMinpos;
            pVec(2) =eVec.at<double>(minpos,2) / normMinpos;
            pVec(3) =eVec.at<double>(minpos,3) / normMinpos;
            pVec(4) =eVec.at<double>(minpos,4) / normMinpos;

            coeffs(0) =pVec(0) ;
            coeffs(1) =pVec(1) ;
            coeffs(2) =pVec(2) ;
            coeffs(3) =pVec(3) ;
            coeffs(4) =pVec(4) ;
            coeffs(5) =-pVec(0) *DM(0,5)-pVec(1) *DM(1,5)-coeffs(2) *DM(2,5);

        // Check that an elliptical solution has been found. AMS sometimes produces Parabolic solutions.
        bool is_ellipse = (coeffs(0)  < 0 && \
                           coeffs(2)  < (coeffs(1) *coeffs(1) )/(4.*coeffs(0) ) && \
                           coeffs(5)  > (-(coeffs(2) *(coeffs(3) *coeffs(3) )) + coeffs(1) *coeffs(3) *coeffs(4)  - coeffs(0) *(coeffs(4) *coeffs(4) )) / \
                                        ((coeffs(1) *coeffs(1) ) - 4*coeffs(0) *coeffs(2) )) || \
                          (coeffs(0)  > 0 && \
                           coeffs(2)  > (coeffs(1) *coeffs(1) )/(4.*coeffs(0) ) && \
                           coeffs(5)  < (-(coeffs(2) *(coeffs(3) *coeffs(3) )) + coeffs(1) *coeffs(3) *coeffs(4)  - coeffs(0) *(coeffs(4) *coeffs(4) )) / \
                                        ( (coeffs(1) *coeffs(1) ) - 4*coeffs(0) *coeffs(2) ));
        if (is_ellipse) {
            double u1 = pVec(2) *pVec(3) *pVec(3)  - pVec(1) *pVec(3) *pVec(4)  + pVec(0) *pVec(4) *pVec(4)  + pVec(1) *pVec(1) *coeffs(5) ;
            double u2 = pVec(0) *pVec(2) *coeffs(5) ;
            double l1 = sqrt(pVec(1) *pVec(1)  + (pVec(0)  - pVec(2) )*(pVec(0)  - pVec(2) ));
            double l2 = pVec(0)  + pVec(2) ;
            double l3 = pVec(1) *pVec(1)  - 4.0*pVec(0) *pVec(2) ;
            double p1 = 2.0*pVec(2) *pVec(3)  - pVec(1) *pVec(4) ;
            double p2 = 2.0*pVec(0) *pVec(4) -(pVec(1) *pVec(3) );

            x0 = p1/l3 + c.x;
            y0 = p2/l3 + c.y;
            a = std::sqrt(2.)*sqrt((u1 - 4.0*u2)/((l1 - l2)*l3));
            b = std::sqrt(2.)*sqrt(-1.0*((u1 - 4.0*u2)/((l1 + l2)*l3)));
            if (pVec(1)  == 0) {
                if (pVec(0)  < pVec(2) ) {
                    theta = 0;
                } else {
                    theta = CC_PI/2.;
                }
            } else {
                theta = CC_PI/2. + 0.5*std::atan2(pVec(1) , (pVec(0)  - pVec(2) ));
            }

            box.center.x = (float)x0; // +c.x;
            box.center.y = (float)y0; // +c.y;
            box.size.width = (float)(2.0*a);
            box.size.height = (float)(2.0*b);
            if( box.size.width > box.size.height )
            {
                float tmp;
                CC_SWAP( box.size.width, box.size.height, tmp );
                box.angle = (float)(90 + theta*180/CC_PI);
            } else {
                box.angle = (float)(fmod(theta*180/CC_PI,180.0));
            };


        } else {
            box = fitEllipseDirect( points );
        }
    } else {
        box = fitEllipse( points );
    }

    return box;
}

CRotatedRect fitEllipseDirect( const img_t* _points )
{
    img_t*  points = _points->getMat();
    int i, n = points->checkVector(2);
    int depth = points->depth();
    CC_Assert( n >= 0 && (depth == CC_32F || depth == CC_32S));

    CRotatedRect box;

    if( n < 5 )
        CC_Error( CC_StsBadSize, "There should be at least 5 points to fit the ellipse" );

    FPoint c(0,0);

    bool is_float = (depth == CC_32F);
    const IPoint*   ptsi = points->ptr<IPoint>();
    const FPoint* ptsf = points->ptr<FPoint>();

    img_t*  A( n, 6, CC_64F);
    Matx<double, 6, 6> DM;
    Matx33d M, TM, Q;
    Matx<double, 3, 1> pVec;

    double x0, y0, a, b, theta, Ts;

    for( i = 0; i < n; i++ )
    {
        FPoint p = is_float ? ptsf[i] : fPoint((float)ptsi[i].x, (float)ptsi[i].y);
        c += p;
    }
    c.x /= (float)n;
    c.y /= (float)n;

    for( i = 0; i < n; i++ )
    {
        FPoint p = is_float ? ptsf[i] : fPoint((float)ptsi[i].x, (float)ptsi[i].y);
        p -= c;

        A.at<double>(i,0) = (double)(p.x)*(p.x);
        A.at<double>(i,1) = (double)(p.x)*(p.y);
        A.at<double>(i,2) = (double)(p.y)*(p.y);
        A.at<double>(i,3) = (double)p.x;
        A.at<double>(i,4) = (double)p.y;
        A.at<double>(i,5) = 1.0;
    }
    mulTransposed( A, DM, true, noArray(), 1.0, -1 );
    DM *= (1.0/n);

    TM(0,0) = DM(0,5)*DM(3,5)*DM(4,4) - DM(0,5)*DM(3,4)*DM(4,5) - DM(0,4)*DM(3,5)*DM(5,4) + \
              DM(0,3)*DM(4,5)*DM(5,4) + DM(0,4)*DM(3,4)*DM(5,5) - DM(0,3)*DM(4,4)*DM(5,5);
    TM(0,1) = DM(1,5)*DM(3,5)*DM(4,4) - DM(1,5)*DM(3,4)*DM(4,5) - DM(1,4)*DM(3,5)*DM(5,4) + \
              DM(1,3)*DM(4,5)*DM(5,4) + DM(1,4)*DM(3,4)*DM(5,5) - DM(1,3)*DM(4,4)*DM(5,5);
    TM(0,2) = DM(2,5)*DM(3,5)*DM(4,4) - DM(2,5)*DM(3,4)*DM(4,5) - DM(2,4)*DM(3,5)*DM(5,4) + \
              DM(2,3)*DM(4,5)*DM(5,4) + DM(2,4)*DM(3,4)*DM(5,5) - DM(2,3)*DM(4,4)*DM(5,5);
    TM(1,0) = DM(0,5)*DM(3,3)*DM(4,5) - DM(0,5)*DM(3,5)*DM(4,3) + DM(0,4)*DM(3,5)*DM(5,3) - \
              DM(0,3)*DM(4,5)*DM(5,3) - DM(0,4)*DM(3,3)*DM(5,5) + DM(0,3)*DM(4,3)*DM(5,5);
    TM(1,1) = DM(1,5)*DM(3,3)*DM(4,5) - DM(1,5)*DM(3,5)*DM(4,3) + DM(1,4)*DM(3,5)*DM(5,3) - \
              DM(1,3)*DM(4,5)*DM(5,3) - DM(1,4)*DM(3,3)*DM(5,5) + DM(1,3)*DM(4,3)*DM(5,5);
    TM(1,2) = DM(2,5)*DM(3,3)*DM(4,5) - DM(2,5)*DM(3,5)*DM(4,3) + DM(2,4)*DM(3,5)*DM(5,3) - \
              DM(2,3)*DM(4,5)*DM(5,3) - DM(2,4)*DM(3,3)*DM(5,5) + DM(2,3)*DM(4,3)*DM(5,5);
    TM(2,0) = DM(0,5)*DM(3,4)*DM(4,3) - DM(0,5)*DM(3,3)*DM(4,4) - DM(0,4)*DM(3,4)*DM(5,3) + \
              DM(0,3)*DM(4,4)*DM(5,3) + DM(0,4)*DM(3,3)*DM(5,4) - DM(0,3)*DM(4,3)*DM(5,4);
    TM(2,1) = DM(1,5)*DM(3,4)*DM(4,3) - DM(1,5)*DM(3,3)*DM(4,4) - DM(1,4)*DM(3,4)*DM(5,3) + \
              DM(1,3)*DM(4,4)*DM(5,3) + DM(1,4)*DM(3,3)*DM(5,4) - DM(1,3)*DM(4,3)*DM(5,4);
    TM(2,2) = DM(2,5)*DM(3,4)*DM(4,3) - DM(2,5)*DM(3,3)*DM(4,4) - DM(2,4)*DM(3,4)*DM(5,3) + \
              DM(2,3)*DM(4,4)*DM(5,3) + DM(2,4)*DM(3,3)*DM(5,4) - DM(2,3)*DM(4,3)*DM(5,4);

    Ts=(-(DM(3,5)*DM(4,4)*DM(5,3)) + DM(3,4)*DM(4,5)*DM(5,3) + DM(3,5)*DM(4,3)*DM(5,4) - \
          DM(3,3)*DM(4,5)*DM(5,4)  - DM(3,4)*DM(4,3)*DM(5,5) + DM(3,3)*DM(4,4)*DM(5,5));

    M(0,0) = (DM(2,0) + (DM(2,3)*TM(0,0) + DM(2,4)*TM(1,0) + DM(2,5)*TM(2,0))/Ts)/2.;
    M(0,1) = (DM(2,1) + (DM(2,3)*TM(0,1) + DM(2,4)*TM(1,1) + DM(2,5)*TM(2,1))/Ts)/2.;
    M(0,2) = (DM(2,2) + (DM(2,3)*TM(0,2) + DM(2,4)*TM(1,2) + DM(2,5)*TM(2,2))/Ts)/2.;
    M(1,0) = -DM(1,0) - (DM(1,3)*TM(0,0) + DM(1,4)*TM(1,0) + DM(1,5)*TM(2,0))/Ts;
    M(1,1) = -DM(1,1) - (DM(1,3)*TM(0,1) + DM(1,4)*TM(1,1) + DM(1,5)*TM(2,1))/Ts;
    M(1,2) = -DM(1,2) - (DM(1,3)*TM(0,2) + DM(1,4)*TM(1,2) + DM(1,5)*TM(2,2))/Ts;
    M(2,0) = (DM(0,0) + (DM(0,3)*TM(0,0) + DM(0,4)*TM(1,0) + DM(0,5)*TM(2,0))/Ts)/2.;
    M(2,1) = (DM(0,1) + (DM(0,3)*TM(0,1) + DM(0,4)*TM(1,1) + DM(0,5)*TM(2,1))/Ts)/2.;
    M(2,2) = (DM(0,2) + (DM(0,3)*TM(0,2) + DM(0,4)*TM(1,2) + DM(0,5)*TM(2,2))/Ts)/2.;

    if (fabs(determinant(M)) > 1.0e-10) {
        img_t*  eVal, eVec;
        eigenNonSymmetric(M, eVal, eVec);

        // Select the eigen vector {a,b,c} which satisfies 4ac-b^2 > 0
        double cond[3];
        cond[0]=(4.0 * eVec.at<double>(0,0) * eVec.at<double>(0,2) - eVec.at<double>(0,1) * eVec.at<double>(0,1));
        cond[1]=(4.0 * eVec.at<double>(1,0) * eVec.at<double>(1,2) - eVec.at<double>(1,1) * eVec.at<double>(1,1));
        cond[2]=(4.0 * eVec.at<double>(2,0) * eVec.at<double>(2,2) - eVec.at<double>(2,1) * eVec.at<double>(2,1));
        if (cond[0]<cond[1]) {
            i = (cond[1]<cond[2]) ? 2 : 1;
        } else {
            i = (cond[0]<cond[2]) ? 2 : 0;
        }
        double norm = std::sqrt(eVec.at<double>(i,0)*eVec.at<double>(i,0) + eVec.at<double>(i,1)*eVec.at<double>(i,1) + eVec.at<double>(i,2)*eVec.at<double>(i,2));
        if (((eVec.at<double>(i,0)<0.0  ? -1 : 1) * (eVec.at<double>(i,1)<0.0  ? -1 : 1) * (eVec.at<double>(i,2)<0.0  ? -1 : 1)) <= 0.0) {
                norm=-1.0*norm;
            }
        pVec(0) =eVec.at<double>(i,0)/norm; pVec(1) =eVec.at<double>(i,1)/norm;pVec(2) =eVec.at<double>(i,2)/norm;

    //  Q = (TM . pVec)/Ts;
        Q(0,0) = (TM(0,0)*pVec(0) +TM(0,1)*pVec(1) +TM(0,2)*pVec(2) )/Ts;
        Q(0,1) = (TM(1,0)*pVec(0) +TM(1,1)*pVec(1) +TM(1,2)*pVec(2) )/Ts;
        Q(0,2) = (TM(2,0)*pVec(0) +TM(2,1)*pVec(1) +TM(2,2)*pVec(2) )/Ts;

    // We compute the ellipse properties in the shifted coordinates as doing so improves the numerical accuracy.

        double u1 = pVec(2)*Q(0,0)*Q(0,0) - pVec(1)*Q(0,0)*Q(0,1) + pVec(0)*Q(0,1)*Q(0,1) + pVec(1)*pVec(1)*Q(0,2);
        double u2 = pVec(0)*pVec(2)*Q(0,2);
        double l1 = sqrt(pVec(1)*pVec(1) + (pVec(0) - pVec(2))*(pVec(0) - pVec(2)));
        double l2 = pVec(0) + pVec(2) ;
        double l3 = pVec(1)*pVec(1) - 4*pVec(0)*pVec(2) ;
        double p1 = 2*pVec(2)*Q(0,0) - pVec(1)*Q(0,1);
        double p2 = 2*pVec(0)*Q(0,1) - pVec(1)*Q(0,0);

        x0 = p1/l3 + c.x;
        y0 = p2/l3 + c.y;
        a = sqrt(2.)*sqrt((u1 - 4.0*u2)/((l1 - l2)*l3));
        b = sqrt(2.)*sqrt(-1.0*((u1 - 4.0*u2)/((l1 + l2)*l3)));
        if (pVec(1)  == 0) {
            if (pVec(0)  < pVec(2) ) {
                theta = 0;
            } else {
                theta = CC_PI/2.;
            }
        } else {
                theta = CC_PI/2. + 0.5*std::atan2(pVec(1) , (pVec(0)  - pVec(2) ));
        }

        box.center.x = (float)x0;
        box.center.y = (float)y0;
        box.size.width = (float)(2.0*a);
        box.size.height = (float)(2.0*b);
        if( box.size.width > box.size.height )
        {
            float tmp;
            CC_SWAP( box.size.width, box.size.height, tmp );
            box.angle = (float)(fmod((90 + theta*180/CC_PI),180.0)) ;
        } else {
            box.angle = (float)(fmod(theta*180/CC_PI,180.0));
        };
    } else {
        box = fitEllipse( points );
    }
    return box;
}



// Calculates bounding rectagnle of a point set or retrieves already calculated
static Rect pointSetBoundingRect( const img_t* & points )
{
    int npoints = points->checkVector(2);
    int depth = points->depth();
    CC_Assert(npoints >= 0 && (depth == CC_32F || depth == CC_32S));

    int  xmin = 0, ymin = 0, xmax = -1, ymax = -1, i;
    bool is_float = depth == CC_32F;

    if( npoints == 0 )
        return Rect();

    const IPoint* pts = points->ptr<IPoint>();
    IPoint pt = pts[0];

#if CC_SSE4_2
    if(checkHardwareSupport(CC_CPU_SSE4_2))
    {
        if( !is_float )
        {
            __m128i minval, maxval;
            minval = maxval = _mm_loadl_epi64((const __m128i*)(&pt)); //min[0]=pt.x, min[1]=pt.y

            for( i = 1; i < npoints; i++ )
            {
                __m128i ptXY = _mm_loadl_epi64((const __m128i*)&pts[i]);
                minval = _mm_min_epi32(ptXY, minval);
                maxval = _mm_max_epi32(ptXY, maxval);
            }
            xmin = _mm_cvtsi128_si32(minval);
            ymin = _mm_cvtsi128_si32(_mm_srli_si128(minval, 4));
            xmax = _mm_cvtsi128_si32(maxval);
            ymax = _mm_cvtsi128_si32(_mm_srli_si128(maxval, 4));
        }
        else
        {
            __m128 minvalf, maxvalf, z = _mm_setzero_ps(), ptXY = _mm_setzero_ps();
            minvalf = maxvalf = _mm_loadl_pi(z, (const __m64*)(&pt));

            for( i = 1; i < npoints; i++ )
            {
                ptXY = _mm_loadl_pi(ptXY, (const __m64*)&pts[i]);

                minvalf = _mm_min_ps(minvalf, ptXY);
                maxvalf = _mm_max_ps(maxvalf, ptXY);
            }

            float xyminf[2], xymaxf[2];
            _mm_storel_pi((__m64*)xyminf, minvalf);
            _mm_storel_pi((__m64*)xymaxf, maxvalf);
            xmin = cvFloor(xyminf[0]);
            ymin = cvFloor(xyminf[1]);
            xmax = cvFloor(xymaxf[0]);
            ymax = cvFloor(xymaxf[1]);
        }
    }
    else
#endif
    {
        if( !is_float )
        {
            xmin = xmax = pt.x;
            ymin = ymax = pt.y;

            for( i = 1; i < npoints; i++ )
            {
                pt = pts[i];

                if( xmin > pt.x )
                    xmin = pt.x;

                if( xmax < pt.x )
                    xmax = pt.x;

                if( ymin > pt.y )
                    ymin = pt.y;

                if( ymax < pt.y )
                    ymax = pt.y;
            }
        }
        else
        {
            Cv32suf v;
            // init values
            xmin = xmax = CC_TOGGLE_FLT(pt.x);
            ymin = ymax = CC_TOGGLE_FLT(pt.y);

            for( i = 1; i < npoints; i++ )
            {
                pt = pts[i];
                pt.x = CC_TOGGLE_FLT(pt.x);
                pt.y = CC_TOGGLE_FLT(pt.y);

                if( xmin > pt.x )
                    xmin = pt.x;

                if( xmax < pt.x )
                    xmax = pt.x;

                if( ymin > pt.y )
                    ymin = pt.y;

                if( ymax < pt.y )
                    ymax = pt.y;
            }

            v.i = CC_TOGGLE_FLT(xmin); xmin = cvFloor(v.f);
            v.i = CC_TOGGLE_FLT(ymin); ymin = cvFloor(v.f);
            // because right and bottom sides of the bounding rectangle are not inclusive
            // (note +1 in width and height calculation below), cvFloor is used here instead of cvCeil
            v.i = CC_TOGGLE_FLT(xmax); xmax = cvFloor(v.f);
            v.i = CC_TOGGLE_FLT(ymax); ymax = cvFloor(v.f);
        }
    }

    return Rect(xmin, ymin, xmax - xmin + 1, ymax - ymin + 1);
}


static Rect maskBoundingRect( const img_t* & img )
{
    CC_Assert( img.depth() <= CC_8S && img.channels() == 1 );

    Size size = img.size();
    int xmin = size.width, ymin = -1, xmax = -1, ymax = -1, i, j, k;

    for( i = 0; i < size.height; i++ )
    {
        const uchar* _ptr = img.ptr(i);
        const uchar* ptr = (const uchar*)alignPtr(_ptr, 4);
        int have_nz = 0, k_min, offset = (int)(ptr - _ptr);
        j = 0;
        offset = MIN(offset, size.width);
        for( ; j < offset; j++ )
            if( _ptr[j] )
            {
                have_nz = 1;
                break;
            }
        if( j < offset )
        {
            if( j < xmin )
                xmin = j;
            if( j > xmax )
                xmax = j;
        }
        if( offset < size.width )
        {
            xmin -= offset;
            xmax -= offset;
            size.width -= offset;
            j = 0;
            for( ; j <= xmin - 4; j += 4 )
                if( *((int*)(ptr+j)) )
                    break;
            for( ; j < xmin; j++ )
                if( ptr[j] )
                {
                    xmin = j;
                    if( j > xmax )
                        xmax = j;
                    have_nz = 1;
                    break;
                }
            k_min = MAX(j-1, xmax);
            k = size.width - 1;
            for( ; k > k_min && (k&3) != 3; k-- )
                if( ptr[k] )
                    break;
            if( k > k_min && (k&3) == 3 )
            {
                for( ; k > k_min+3; k -= 4 )
                    if( *((int*)(ptr+k-3)) )
                        break;
            }
            for( ; k > k_min; k-- )
                if( ptr[k] )
                {
                    xmax = k;
                    have_nz = 1;
                    break;
                }
            if( !have_nz )
            {
                j &= ~3;
                for( ; j <= k - 3; j += 4 )
                    if( *((int*)(ptr+j)) )
                        break;
                for( ; j <= k; j++ )
                    if( ptr[j] )
                    {
                        have_nz = 1;
                        break;
                    }
            }
            xmin += offset;
            xmax += offset;
            size.width += offset;
        }
        if( have_nz )
        {
            if( ymin < 0 )
                ymin = i;
            ymax = i;
        }
    }

    if( xmin >= size.width )
        xmin = ymin = 0;
    return Rect(xmin, ymin, xmax - xmin + 1, ymax - ymin + 1);
}



Rect boundingRect(const img_t* array)
{
    CC_INSTRUMENT_REGION()

    img_t*  m = array.getMat();
    return m.depth() <= CC_8U ? maskBoundingRect(m) : pointSetBoundingRect(m);
}

////////////////////////////////////////////// C API ///////////////////////////////////////////

CC_IMPL int
cvMinEnclosingCircle( const void* array, CvPoint2D32f * _center, float *_radius )
{
    AutoBuffer<double> abuf;
    img_t*  points = cvarrToMat(array, false, false, 0, &abuf);
    FPoint center;
    float radius;

    minEnclosingCircle(points, center, radius);
    if(_center)
        *_center = center;
    if(_radius)
        *_radius = radius;
    return 1;
}

static void
icvMemCopy( double **buf1, double **buf2, double **buf3, int *b_max )
{
    CC_Assert( (*buf1 != NULL || *buf2 != NULL) && *buf3 != NULL );

    int bb = *b_max;
    if( *buf2 == NULL )
    {
        *b_max = 2 * (*b_max);
        *buf2 = (double *)cvAlloc( (*b_max) * sizeof( double ));

        memcpy( *buf2, *buf3, bb * sizeof( double ));

        *buf3 = *buf2;
        cvFree( buf1 );
        *buf1 = NULL;
    }
    else
    {
        *b_max = 2 * (*b_max);
        *buf1 = (double *) cvAlloc( (*b_max) * sizeof( double ));

        memcpy( *buf1, *buf3, bb * sizeof( double ));

        *buf3 = *buf1;
        cvFree( buf2 );
        *buf2 = NULL;
    }
}


/* area of a contour sector */
static double icvContourSecArea( CvSeq * contour, CvSlice slice )
{
    CvPoint pt;                 /*  pointer to points   */
    CvPoint pt_s, pt_e;         /*  first and last points  */
    CvSeqReader reader;         /*  points reader of contour   */

    int p_max = 2, p_ind;
    int lpt, flag, i;
    double a00;                 /* unnormalized moments m00    */
    double xi, yi, xi_1, yi_1, x0, y0, dxy, sk, sk1, t;
    double x_s, y_s, nx, ny, dx, dy, du, dv;
    double eps = 1.e-5;
    double *p_are1, *p_are2, *p_are;
    double area = 0;

    CC_Assert( contour != NULL && CC_IS_SEQ_POINT_SET( contour ));

    lpt = cvSliceLength( slice, contour );
    /*if( n2 >= n1 )
        lpt = n2 - n1 + 1;
    else
        lpt = contour->total - n1 + n2 + 1;*/

    if( contour->total <= 0 || lpt <= 2 )
        return 0.;

    a00 = x0 = y0 = xi_1 = yi_1 = 0;
    sk1 = 0;
    flag = 0;
    dxy = 0;
    p_are1 = (double *) cvAlloc( p_max * sizeof( double ));

    p_are = p_are1;
    p_are2 = NULL;

    cvStartReadSeq( contour, &reader, 0 );
    cvSetSeqReaderPos( &reader, slice.start_index );
    CC_READ_SEQ_ELEM( pt_s, reader );
    p_ind = 0;
    cvSetSeqReaderPos( &reader, slice.end_index );
    CC_READ_SEQ_ELEM( pt_e, reader );

/*    normal coefficients    */
    nx = pt_s.y - pt_e.y;
    ny = pt_e.x - pt_s.x;
    cvSetSeqReaderPos( &reader, slice.start_index );

    while( lpt-- > 0 )
    {
        CC_READ_SEQ_ELEM( pt, reader );

        if( flag == 0 )
        {
            xi_1 = (double) pt.x;
            yi_1 = (double) pt.y;
            x0 = xi_1;
            y0 = yi_1;
            sk1 = 0;
            flag = 1;
        }
        else
        {
            xi = (double) pt.x;
            yi = (double) pt.y;

/****************   edges intersection examination   **************************/
            sk = nx * (xi - pt_s.x) + ny * (yi - pt_s.y);
            if( (fabs( sk ) < eps && lpt > 0) || sk * sk1 < -eps )
            {
                if( fabs( sk ) < eps )
                {
                    dxy = xi_1 * yi - xi * yi_1;
                    a00 = a00 + dxy;
                    dxy = xi * y0 - x0 * yi;
                    a00 = a00 + dxy;

                    if( p_ind >= p_max )
                        icvMemCopy( &p_are1, &p_are2, &p_are, &p_max );

                    p_are[p_ind] = a00 / 2.;
                    p_ind++;
                    a00 = 0;
                    sk1 = 0;
                    x0 = xi;
                    y0 = yi;
                    dxy = 0;
                }
                else
                {
/*  define intersection point    */
                    dv = yi - yi_1;
                    du = xi - xi_1;
                    dx = ny;
                    dy = -nx;
                    if( fabs( du ) > eps )
                        t = ((yi_1 - pt_s.y) * du + dv * (pt_s.x - xi_1)) /
                            (du * dy - dx * dv);
                    else
                        t = (xi_1 - pt_s.x) / dx;
                    if( t > eps && t < 1 - eps )
                    {
                        x_s = pt_s.x + t * dx;
                        y_s = pt_s.y + t * dy;
                        dxy = xi_1 * y_s - x_s * yi_1;
                        a00 += dxy;
                        dxy = x_s * y0 - x0 * y_s;
                        a00 += dxy;
                        if( p_ind >= p_max )
                            icvMemCopy( &p_are1, &p_are2, &p_are, &p_max );

                        p_are[p_ind] = a00 / 2.;
                        p_ind++;

                        a00 = 0;
                        sk1 = 0;
                        x0 = x_s;
                        y0 = y_s;
                        dxy = x_s * yi - xi * y_s;
                    }
                }
            }
            else
                dxy = xi_1 * yi - xi * yi_1;

            a00 += dxy;
            xi_1 = xi;
            yi_1 = yi;
            sk1 = sk;

        }
    }

    xi = x0;
    yi = y0;
    dxy = xi_1 * yi - xi * yi_1;

    a00 += dxy;

    if( p_ind >= p_max )
        icvMemCopy( &p_are1, &p_are2, &p_are, &p_max );

    p_are[p_ind] = a00 / 2.;
    p_ind++;

    // common area calculation
    area = 0;
    for( i = 0; i < p_ind; i++ )
        area += fabs( p_are[i] );

    if( p_are1 != NULL )
        cvFree( &p_are1 );
    else if( p_are2 != NULL )
        cvFree( &p_are2 );

    return area;
}


/* external contour area function */
CC_IMPL double
cvContourArea( const void *array, CvSlice slice, int oriented )
{
    double area = 0;

    CvContour contour_header;
    CvSeq* contour = 0;
    CvSeqBlock block;

    if( CC_IS_SEQ( array ))
    {
        contour = (CvSeq*)array;
        if( !CC_IS_SEQ_POLYLINE( contour ))
            CC_Error( CC_StsBadArg, "Unsupported sequence type" );
    }
    else
    {
        contour = cvPointSeqFromMat( CC_SEQ_KIND_CURVE, array, &contour_header, &block );
    }

    if( cvSliceLength( slice, contour ) == contour->total )
    {
        AutoBuffer<double> abuf;
        img_t*  points = cvarrToMat(contour, false, false, 0, &abuf);
        return contourArea( points, oriented !=0 );
    }

    if( CC_SEQ_ELTYPE( contour ) != CC_32SC2 )
        CC_Error( CC_StsUnsupportedFormat,
        "Only curves with integer coordinates are supported in case of contour slice" );
    area = icvContourSecArea( contour, slice );
    return oriented ? area : fabs(area);
}


/* calculates length of a curve (e.g. contour perimeter) */
CC_IMPL  double
cvArcLength( const void *array, CvSlice slice, int is_closed )
{
    double perimeter = 0;

    int i, j = 0, count;
    const int N = 16;
    float buf[N];
    CvMat buffer = cvMat( 1, N, CC_32F, buf );
    CvSeqReader reader;
    CvContour contour_header;
    CvSeq* contour = 0;
    CvSeqBlock block;

    if( CC_IS_SEQ( array ))
    {
        contour = (CvSeq*)array;
        if( !CC_IS_SEQ_POLYLINE( contour ))
            CC_Error( CC_StsBadArg, "Unsupported sequence type" );
        if( is_closed < 0 )
            is_closed = CC_IS_SEQ_CLOSED( contour );
    }
    else
    {
        is_closed = is_closed > 0;
        contour = cvPointSeqFromMat(
                                    CC_SEQ_KIND_CURVE | (is_closed ? CC_SEQ_FLAG_CLOSED : 0),
                                    array, &contour_header, &block );
    }

    if( contour->total > 1 )
    {
        int is_float = CC_SEQ_ELTYPE( contour ) == CC_32FC2;

        cvStartReadSeq( contour, &reader, 0 );
        cvSetSeqReaderPos( &reader, slice.start_index );
        count = cvSliceLength( slice, contour );

        count -= !is_closed && count == contour->total;

        // scroll the reader by 1 point
        reader.prev_elem = reader.ptr;
        CC_NEXT_SEQ_ELEM( sizeof(CvPoint), reader );

        for( i = 0; i < count; i++ )
        {
            float dx, dy;

            if( !is_float )
            {
                CvPoint* pt = (CvPoint*)reader.ptr;
                CvPoint* prev_pt = (CvPoint*)reader.prev_elem;

                dx = (float)pt->x - (float)prev_pt->x;
                dy = (float)pt->y - (float)prev_pt->y;
            }
            else
            {
                CvPoint2D32f* pt = (CvPoint2D32f*)reader.ptr;
                CvPoint2D32f* prev_pt = (CvPoint2D32f*)reader.prev_elem;

                dx = pt->x - prev_pt->x;
                dy = pt->y - prev_pt->y;
            }

            reader.prev_elem = reader.ptr;
            CC_NEXT_SEQ_ELEM( contour->elem_size, reader );
            // Bugfix by Axel at rubico.com 2010-03-22, affects closed slices only
            // wraparound not handled by CC_NEXT_SEQ_ELEM
            if( is_closed && i == count - 2 )
                cvSetSeqReaderPos( &reader, slice.start_index );

            buffer.data.fl[j] = dx * dx + dy * dy;
            if( ++j == N || i == count - 1 )
            {
                buffer.cols = j;
                cvPow( &buffer, &buffer, 0.5 );
                for( ; j > 0; j-- )
                    perimeter += buffer.data.fl[j-1];
            }
        }
    }

    return perimeter;
}


CC_IMPL CvBox2D
cvFitEllipse2( const CvArr* array )
{
    AutoBuffer<double> abuf;
    img_t*  points = cvarrToMat(array, false, false, 0, &abuf);
    return fitEllipse(points);
}

/* Calculates bounding rectagnle of a point set or retrieves already calculated */
CC_IMPL  CvRect
cvBoundingRect( CvArr* array, int update )
{
    CvRect  rect;
    CvContour contour_header;
    CvSeq* ptseq = 0;
    CvSeqBlock block;

    CvMat stub, *mat = 0;
    int calculate = update;

    if( CC_IS_SEQ( array ))
    {
        ptseq = (CvSeq*)array;
        if( !CC_IS_SEQ_POINT_SET( ptseq ))
            CC_Error( CC_StsBadArg, "Unsupported sequence type" );

        if( ptseq->header_size < (int)sizeof(CvContour))
        {
            update = 0;
            calculate = 1;
        }
    }
    else
    {
        mat = cvGetMat( array, &stub );
        if( CC_MAT_TYPE(mat->type) == CC_32SC2 ||
            CC_MAT_TYPE(mat->type) == CC_32FC2 )
        {
            ptseq = cvPointSeqFromMat(CC_SEQ_KIND_GENERIC, mat, &contour_header, &block);
            mat = 0;
        }
        else if( CC_MAT_TYPE(mat->type) != CC_8UC1 &&
                CC_MAT_TYPE(mat->type) != CC_8SC1 )
            CC_Error( CC_StsUnsupportedFormat,
                "The image/matrix format is not supported by the function" );
        update = 0;
        calculate = 1;
    }

    if( !calculate )
        return ((CvContour*)ptseq)->rect;

    if( mat )
    {
        rect = maskBoundingRect(cvarrToMat(mat));
    }
    else if( ptseq->total )
    {
        AutoBuffer<double> abuf;
        rect = pointSetBoundingRect(cvarrToMat(ptseq, false, false, 0, &abuf));
    }
    if( update )
        ((CvContour*)ptseq)->rect = rect;
    return rect;
}

/* End of file. */

#endif
