
typedef struct CConnectedComp
{
    double area;    /* area of the connected component  */
    CScalar value; /* average color of the connected component */
    CRect rect;    /* ROI of the component  */
    //CvSeq* contour; /* optional component boundary (the contour might have child contours corresponding to the holes)*/
}
CConnectedComp;

/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:    cvMeanShift
//    Purpose: MeanShift algorithm
//    Context:
//    Parameters:
//      imgProb     - 2D object probability distribution
//      windowIn    - CRect of CAMSHIFT Window intial size
//      numIters    - If CAMSHIFT iterates this many times, stop
//      windowOut   - Location, height and width of converged CAMSHIFT window
//      len         - If != NULL, return equivalent len
//      width       - If != NULL, return equivalent width
//      itersUsed   - Returns number of iterations CAMSHIFT took to converge
//    Returns:
//      The function itself returns the area found
//    Notes:
//F*/
CC_IMPL int
cvMeanShift(const img_t* mat, CRect windowIn,
            CTermCriteria criteria, CConnectedComp* comp)
{
    CMoments moments;
    int    i = 0, eps;
    img_t cur_win;
    CRect cur_rect = windowIn;

    CC_FUNCNAME("cvMeanShift");

    if(comp)
        comp->rect = windowIn;

    moments.m00 = moments.m10 = moments.m01 = 0;

    __BEGIN__;

    if(CC_MAT_CN(mat) > 1)
        CC_ERROR(CC_BadNumChannels, cvUnsupportedFormat);

    if(windowIn.height <= 0 || windowIn.width <= 0)
        CC_ERROR(CC_StsBadArg, "Input window has non-positive sizes");

    if(windowIn.x < 0 || windowIn.x + windowIn.width > mat->cols ||
        windowIn.y < 0 || windowIn.y + windowIn.height > mat->rows)
        CC_ERROR(CC_StsBadArg, "Initial window is not inside the image ROI");

    CC_CALL(criteria = cvCheckTermCriteria(criteria, 1., 100));

    eps = cRound(criteria.epsilon * criteria.epsilon);

    for(i = 0; i < criteria.max_iter; i++)
    {
        int dx, dy, nx, ny;
        double inv_m00;

        CC_CALL(cvGetSubRect(mat, &cur_win, cur_rect)); 
        CC_CALL(cvMoments(&cur_win, &moments));

        /* Calculating center of mass */
        if(fabs(moments.m00) < DBL_EPSILON)
            break;

        inv_m00 = moments.inv_sqrt_m00*moments.inv_sqrt_m00;
        dx = cRound(moments.m10 * inv_m00 - windowIn.width*0.5);
        dy = cRound(moments.m01 * inv_m00 - windowIn.height*0.5);

        nx = cur_rect.x + dx;
        ny = cur_rect.y + dy;

        if(nx < 0)
            nx = 0;
        else if(nx + cur_rect.width > mat->cols)
            nx = mat->cols - cur_rect.width;

        if(ny < 0)
            ny = 0;
        else if(ny + cur_rect.height > mat->rows)
            ny = mat->rows - cur_rect.height;

        dx = nx - cur_rect.x;
        dy = ny - cur_rect.y;
        cur_rect.x = nx;
        cur_rect.y = ny;

        /* Check for coverage centers mass & window */
        if(dx*dx + dy*dy < eps)
            break;
    }

    __END__;

    if(comp)
    {
        comp->rect = cur_rect;
        comp->area = (float)moments.m00;
    }

    return i;
}


/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:    cvCamShift
//    Purpose: CAMSHIFT algorithm
//    Context:
//    Parameters:
//      imgProb     - 2D object probability distribution
//      windowIn    - CRect of CAMSHIFT Window intial size
//      criteria    - criteria of stop finding window
//      windowOut   - Location, height and width of converged CAMSHIFT window
//      orientation - If != NULL, return distribution orientation
//      len         - If != NULL, return equivalent len
//      width       - If != NULL, return equivalent width
//      area        - sum of all elements in result window
//      itersUsed   - Returns number of iterations CAMSHIFT took to converge
//    Returns:
//      The function itself returns the area found
//    Notes:
//F*/
CC_IMPL int
cvCamShift(const img_t* mat, CRect windowIn,
            CTermCriteria criteria,
            CConnectedComp* _comp,
            CBox2D* box)
{
    const int TOLERANCE = 10;
    CMoments moments;
    double m00 = 0, m10, m01, mu20, mu11, mu02, inv_m00;
    double a, b, c, xc, yc;
    double rotate_a, rotate_c;
    double theta = 0, square;
    double cs, sn;
    double length = 0, width = 0;
    int itersUsed = 0;
    CConnectedComp comp;
    img_t cur_win;

    CC_FUNCNAME("cvCamShift");

    comp.rect = windowIn;

    __BEGIN__;

    CC_CALL(itersUsed = cvMeanShift(mat, windowIn, criteria, &comp));
    windowIn = comp.rect;

    windowIn.x -= TOLERANCE;
    if(windowIn.x < 0)
        windowIn.x = 0;

    windowIn.y -= TOLERANCE;
    if(windowIn.y < 0)
        windowIn.y = 0;

    windowIn.width += 2 * TOLERANCE;
    if(windowIn.x + windowIn.width > mat->width)
        windowIn.width = mat->width - windowIn.x;

    windowIn.height += 2 * TOLERANCE;
    if(windowIn.y + windowIn.height > mat->height)
        windowIn.height = mat->height - windowIn.y;

    CC_CALL(cvGetSubRect(mat, &cur_win, windowIn));

    /* Calculating moments in new center mass */
    CC_CALL(cvMoments(&cur_win, &moments));

    m00 = moments.m00;
    m10 = moments.m10;
    m01 = moments.m01;
    mu11 = moments.mu11;
    mu20 = moments.mu20;
    mu02 = moments.mu02;

    if(fabs(m00) < DBL_EPSILON)
        EXIT;

    inv_m00 = 1. / m00;
    xc = cRound(m10 * inv_m00 + windowIn.x);
    yc = cRound(m01 * inv_m00 + windowIn.y);
    a = mu20 * inv_m00;
    b = mu11 * inv_m00;
    c = mu02 * inv_m00;

    /* Calculating width & height */
    square = sqrt(4 * b * b + (a - c) * (a - c));

    /* Calculating orientation */
    theta = atan2(2 * b, a - c + square);

    /* Calculating width & length of figure */
    cs = cos(theta);
    sn = sin(theta);

    rotate_a = cs * cs * mu20 + 2 * cs * sn * mu11 + sn * sn * mu02;
    rotate_c = sn * sn * mu20 - 2 * cs * sn * mu11 + cs * cs * mu02;
    length = sqrt(rotate_a * inv_m00) * 4;
    width = sqrt(rotate_c * inv_m00) * 4;

    /* In case, when tetta is 0 or 1.57... the Length & Width may be exchanged */
    if(length < width)
    {
        double t;
        
        CC_SWAP(length, width, t);
        CC_SWAP(cs, sn, t);
        theta = CC_PI*0.5 - theta;
    }

    /* Saving results */
    if(_comp || box)
    {
        int t0, t1;
        int _xc = cRound(xc);
        int _yc = cRound(yc);

        t0 = cRound(fabs(length * cs));
        t1 = cRound(fabs(width * sn));

        t0 = MAX(t0, t1) + 2;
        comp.rect.width = MIN(t0, (mat->width - _xc) * 2);

        t0 = cRound(fabs(length * sn));
        t1 = cRound(fabs(width * cs));

        t0 = MAX(t0, t1) + 2;
        comp.rect.height = MIN(t0, (mat->height - _yc) * 2);

        comp.rect.x = MAX(0, _xc - comp.rect.width / 2);
        comp.rect.y = MAX(0, _yc - comp.rect.height / 2);

        comp.rect.width = MIN(mat->width - comp.rect.x, comp.rect.width);
        comp.rect.height = MIN(mat->height - comp.rect.y, comp.rect.height);
        comp.area = (float) m00;
    }

    __END__;

    if(_comp)
        *_comp = comp;
    
    if(box)
    {
        box->size.height = (float)length;
        box->size.width = (float)width;
        box->angle = (float)(theta*180./CC_PI);
        box->center = cPoint2D32f(comp.rect.x + comp.rect.width*0.5f,
                                    comp.rect.y + comp.rect.height*0.5f);
    }

    return itersUsed;
}


