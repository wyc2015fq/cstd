
/************************************************************************************\
       Some backward compatibility stuff, to be moved to legacy or compat module
\************************************************************************************/


//////////////////////////////////////////////////////////////////////////////////////////
class CC_EXPORTS CLevMarq
{
public:
    CLevMarq();
    CLevMarq( int nparams, int nerrs, CTermCriteria criteria=
              cTermCriteria(CC_TERMCRIT_EPS+CC_TERMCRIT_ITER,30,DBL_EPSILON),
              bool completeSymmFlag=false );
    ~CLevMarq();
    void init( int nparams, int nerrs, CTermCriteria criteria=
              cTermCriteria(CC_TERMCRIT_EPS+CC_TERMCRIT_ITER,30,DBL_EPSILON),
              bool completeSymmFlag=false );
    bool update( const img_t*& param, img_t* J, img_t* err );
    bool updateAlt( const img_t* param, img_t* JtJ, img_t* JtErr, double*& errNorm );

    void clear();
    void step();
    enum { DONE=0, STARTED=1, CALC_J=2, CHECK_ERR=3 };

    img_t* mask;
    img_t* prevParam;
    img_t* param;
    img_t* J;
    img_t* err;
    img_t* JtJ;
    img_t* JtJN;
    img_t* JtErr;
    img_t* JtJV;
    img_t* JtJW;
    img_t _im[20];
    double prevErrNorm, errNorm;
    int lambdaLg10;
    CTermCriteria criteria;
    int state;
    int iters;
    bool completeSymmFlag;
    CDecompTypes solveMethod;
};
////////////////// Levenberg-Marquardt engine (the old variant) ////////////////////////

CLevMarq::CLevMarq()
{
    lambdaLg10 = 0; state = DONE;
    criteria = cTermCriteria(0,0,0);
    iters = 0;
    completeSymmFlag = false;
    errNorm = prevErrNorm = DBL_MAX;
    solveMethod = CC_DECOMP_SVD;
}

CLevMarq::CLevMarq( int nparams, int nerrs, CTermCriteria criteria0, bool _completeSymmFlag )
{
    BZERO(_im, 20);
    init(nparams, nerrs, criteria0, _completeSymmFlag);
}

void CLevMarq::clear()
{
  imfrees2(_im);
}

CLevMarq::~CLevMarq()
{
    clear();
}

void CLevMarq::init( int nparams, int nerrs, CTermCriteria criteria0, bool _completeSymmFlag )
{
	imfrees2(_im);

	img_t* im = _im;
	memset(_im, 0, sizeof(_im));
	mask = im++;
	prevParam = im++;
	param = im++;
	J = im++;
	err = im++;
	JtJ = im++;
	JtJN = im++;
	JtErr = im++;
	JtJV = im++;
	JtJW = im++;

    if( !param || param->rows != nparams || nerrs != (err ? err->rows : 0) )
        clear();
    cvCreateMat( mask, nparams, 1, CC_8U );
    cvSet(mask, cScalarAll(1));
    cvCreateMat( prevParam, nparams, 1, CC_64F );
    cvCreateMat( param, nparams, 1, CC_64F );
    cvCreateMat( JtJ, nparams, nparams, CC_64F );
    cvCreateMat( JtErr, nparams, 1, CC_64F );
    if( nerrs > 0 )
    {
        cvCreateMat( J, nerrs, nparams, CC_64F );
        cvCreateMat( err, nerrs, 1, CC_64F );
    }
    errNorm = prevErrNorm = DBL_MAX;
    lambdaLg10 = -3;
    criteria = criteria0;
    if( criteria.type & CC_TERMCRIT_ITER )
        criteria.max_iter = MIN(MAX(criteria.max_iter,1),1000);
    else
        criteria.max_iter = 30;
    if( criteria.type & CC_TERMCRIT_EPS )
        criteria.epsilon = MAX(criteria.epsilon, 0);
    else
        criteria.epsilon = DBL_EPSILON;
    state = STARTED;
    iters = 0;
    completeSymmFlag = _completeSymmFlag;
    solveMethod = CC_DECOMP_SVD;
}

bool CLevMarq::update( const img_t*& _param, img_t* matJ, img_t* _err )
{
    matJ = _err = 0;

    assert( !imempty(err) );
    if( state == DONE )
    {
        _param = param;
        return false;
    }

    if( state == STARTED )
    {
        _param = param;
        cvZeroMat( J );
        cvZeroMat( err );
        matJ = J;
        _err = err;
        state = CALC_J;
        return true;
    }

    if( state == CALC_J )
    {
        cvMulTransposed( J, JtJ, 1 );
        cvGEMM( J, err, 1, 0, 0, JtErr, CC_GEMM_A_T );
        cvCopy( param, prevParam );
        step();
        if( iters == 0 )
            prevErrNorm = cvNorm(err, 0, CC_L2);
        _param = param;
        cvZeroMat( err );
        _err = err;
        state = CHECK_ERR;
        return true;
    }

    assert( state == CHECK_ERR );
    errNorm = cvNorm( err, 0, CC_L2 );
    if( errNorm > prevErrNorm )
    {
        if( ++lambdaLg10 <= 16 )
        {
            step();
            _param = param;
            cvZeroMat( err );
            _err = err;
            state = CHECK_ERR;
            return true;
        }
    }

    lambdaLg10 = MAX(lambdaLg10-1, -16);
    if( ++iters >= criteria.max_iter ||
        cvNorm(param, prevParam, CC_RELATIVE_L2) < criteria.epsilon )
    {
        _param = param;
        state = DONE;
        return true;
    }

    prevErrNorm = errNorm;
    _param = param;
    cvZeroMat(J);
    matJ = J;
    _err = err;
    state = CALC_J;
    return true;
}


bool CLevMarq::updateAlt( const img_t* _param, img_t* _JtJ, img_t* _JtErr, double*& _errNorm )
{
    CC_Assert( !err );
    if( state == DONE )
    {
        _param = param;
        return false;
    }

    if( state == STARTED )
    {
        _param = param;
        cvZeroMat( JtJ );
        cvZeroMat( JtErr );
        errNorm = 0;
        _JtJ = JtJ;
        _JtErr = JtErr;
        _errNorm = &errNorm;
        state = CALC_J;
        return true;
    }

    if( state == CALC_J )
    {
        cvCopy( param, prevParam );
        step();
        _param = param;
        prevErrNorm = errNorm;
        errNorm = 0;
        _errNorm = &errNorm;
        state = CHECK_ERR;
        return true;
    }

    assert( state == CHECK_ERR );
    if( errNorm > prevErrNorm )
    {
        if( ++lambdaLg10 <= 16 )
        {
            step();
            _param = param;
            errNorm = 0;
            _errNorm = &errNorm;
            state = CHECK_ERR;
            return true;
        }
    }

    lambdaLg10 = MAX(lambdaLg10-1, -16);
    if( ++iters >= criteria.max_iter ||
        cvNorm(param, prevParam, CC_RELATIVE_L2) < criteria.epsilon )
    {
        _param = param;
        _JtJ = JtJ;
        _JtErr = JtErr;
        state = DONE;
        return false;
    }

    prevErrNorm = errNorm;
    cvZeroMat( JtJ );
    cvZeroMat( JtErr );
    _param = param;
    _JtJ = JtJ;
    _JtErr = JtErr;
    state = CALC_J;
    return true;
}
template<typename T> CC_IMPL int
countNonZero(const std::vector<T>& cols) {
  img_t mat = cvMat(cols.size(), 1, (TypeId)TypeIdTrais<T>::id, 1);
  return cvCountNonZero(&mat);
}
static int countNonZero(const uchar* cols, int ncols) {
  int i, nNonZero = 0;
  for (i=0; i<ncols; ++i) {
    nNonZero += cols[i]!=0;
  }
  return nNonZero;
}

static void subMatrix(const img_t* src, img_t* dst, const uchar* cols, int ncols,
                      const uchar* rows, int nrows) {
    int nonzeros_cols = countNonZero(cols, ncols);
    img_t tmp[1] = {0};
    img_t stmp[1] = {0};
    img_t dtmp[1] = {0};
    int i, j;
    cvSetMat(tmp, src->rows, nonzeros_cols, CC_64F);

    for (i = 0, j = 0; i < ncols; i++)
    {
        if (cols[i])
        {
            //src.col(i).copyTo(tmp.col(j));
            cvCopy(cvGetCol(src, stmp, i), cvGetCol(tmp, dtmp, j));
            j++;
        }
    }

    int nonzeros_rows  = countNonZero(rows, nrows);
    cvSetMat1(dst, nonzeros_rows, nonzeros_cols, CC_64FC1);
    for (i = 0, j = 0; i < nrows; i++)
    {
        if (rows[i])
        {
            //tmp.row(i).copyTo(dst.row(j++));
            cvCopy(cvGetRow(tmp, stmp, i), cvGetRow(dst, dtmp, j));
            ++j;
        }
    }
    imfree(tmp);
}



void CLevMarq::step()
{
    const double LOG10 = log(10.);
    double lambda = exp(lambdaLg10*LOG10);
    int nparams = param->rows;

    img_t *_JtJ = (JtJ);
    img_t *_mask = (mask);

    int nparams_nz = cvCountNonZero(_mask);
    if(!JtJN || JtJN->rows != nparams_nz) {
        // prevent re-allocation in every step
        (cvCreateMat( JtJN, nparams_nz, nparams_nz, CC_64F ));
        (cvCreateMat( JtJV, nparams_nz, 1, CC_64F ));
        (cvCreateMat( JtJW, nparams_nz, 1, CC_64F ));
    }

    img_t *_JtJN = (JtJN);
    img_t *_JtErr = (JtJV);
    img_t *nonzero_param = (JtJW);
    IM_DEF(tmp, 2);

    uchar cols1[] = {1, 1};
    subMatrix((JtErr), _JtErr, cols1, countof(cols1), _mask->tt.data, CC_MAT_TOTAL(_mask));
    subMatrix(_JtJ, _JtJN, _mask->tt.data, CC_MAT_TOTAL(_mask), _mask->tt.data, CC_MAT_TOTAL(_mask));

    if( !err )
        completeSymm( _JtJN, completeSymmFlag );

#if 1
    //_JtJN.diag() *= 1. + lambda;
    img_t* diag = cvGetDiag(_JtJN, tmp++);
    cvMulS1(diag, 1. + lambda, diag);
#else
    _JtJN.diag() += lambda;
#endif
    solve(_JtJN, _JtErr, nonzero_param, solveMethod);

    int j = 0;
    for( int i = 0; i < nparams; i++ )
        param->tt.db[i] = prevParam->tt.db[i] - (mask->tt.data[i] ? nonzero_param->tt.db[j++] : 0);
}


CC_IMPL int cRANSACUpdateNumIters( double p, double ep, int modelPoints, int maxIters )
{
    return RANSACUpdateNumIters(p, ep, modelPoints, maxIters);
}

CC_IMPL int cvFindHomography( const img_t* src, const img_t* dst, img_t* __H, int method,
                              double ransacReprojThreshold, img_t* mask, int maxIters,
                              double confidence)
{
    IM_BEGIN(im, 10);
    if( CC_MAT_CN(src) == 1 && (src->rows == 2 || src->rows == 3) && src->cols > 3 )
        src = transpose(src, im++);
    if( CC_MAT_CN(dst) == 1 && (dst->rows == 2 || dst->rows == 3) && dst->cols > 3 )
        dst = transpose(dst, im++);

    if ( maxIters < 0 )
        maxIters = 0;
    if ( maxIters > 2000 )
        maxIters = 2000;

    if ( confidence < 0 )
        confidence = 0;
    if ( confidence > 1 )
        confidence = 1;

    img_t *H = (__H);
    img_t *H0 = findHomography(im++, src, dst, method, ransacReprojThreshold, mask, maxIters, confidence);

    if( imempty(H0) )
    {
        img_t *Hz = (__H);
        cvSetReal(Hz, 0);
    } else {
      cvConvertTo(H0, H, H->tid);
    }
    IM_END(im);
    return 1;
}


CC_IMPL int cvFindFundamentalMat( const img_t* points1, const img_t* points2,
                                  img_t* fmatrix, int method,
                                  double param1, double param2, img_t* _mask )
{
    int ret = 0;
    IM_BEGIN(im, 10);
    const img_t *m1 = (points1), *m2 = (points2);

    if( CC_MAT_CN(m1) == 1 && (m1->rows == 2 || m1->rows == 3) && m1->cols > 3 )
        m1 = transpose(m1, im++);
    if( CC_MAT_CN(m2) == 1 && (m2->rows == 2 || m2->rows == 3) && m2->cols > 3 )
        m2 = transpose(m2, im++);

    img_t *FM = (fmatrix), *mask = (_mask);
    img_t *FM0 = findFundamentalMat(im++, m1, m2, method, param1, param2, mask);

    if( imempty(FM0) )
    {
        img_t *FM0z = (fmatrix);
        cvSetReal(FM0z, 0);
        ret = 0;
    } else {
      CC_Assert( FM0->cols == 3 && FM0->rows % 3 == 0 && FM->cols == 3 && FM->rows % 3 == 0 && CC_MAT_CN(FM) == 1 );
      IM_DEF(tmp, 4);
      img_t *FM1 = cvGetRows(FM, tmp++, 0, MIN(FM0->rows, FM->rows));
      cvConvertTo(cvGetRows(FM0, tmp++, 0, FM1->rows), FM1, FM1->tid);
      ret = FM1->rows / 3;
    }
    IM_END(im);
    return ret;
}


CC_IMPL img_t* cvComputeCorrespondEpilines( const img_t* points, int pointImageID,
                                          const img_t* fmatrix, img_t* _lines )
{
    img_t *lines0 = _lines;
    IM_BEGIN(im, 10);
    const img_t *pt = (points), *fm = (fmatrix);
    img_t *lines = (_lines);
    IM_DEF(tmp, 10);

    if( CC_MAT_CN(pt) == 1 && (pt->rows == 2 || pt->rows == 3) && pt->cols > 3 )
        pt = transpose(pt, im++);

    computeCorrespondEpilines(pt, pointImageID, fm, lines);

    bool tflag = CC_MAT_CN(lines0) == 1 && lines0->rows == 3 && lines0->cols > 3;
    lines = cvReshape(lines, tmp++, CC_MAT_CN(lines0), (tflag ? lines0->cols : lines0->rows));

    if( tflag )
    {
        CC_Assert( lines->rows == lines0->cols && lines->cols == lines0->rows );
        if( lines0->tid == lines->tid )
            transpose( lines, lines0 );
        else
        {
            transpose( lines, lines );
            cvConvertTo( lines, lines0, lines0->tid );
        }
    }
    else
    {
        CC_Assert( CC_ARE_SIZES_EQ(lines, lines0) );
        if( lines != lines0 )
            cvConvertTo(lines, lines0, lines0->tid);
    }
    IM_END(im);
    return lines0;
}

CC_IMPL void cvConvertPointsHomogeneous( const img_t* _src, img_t* _dst )
{
    const img_t *src = (_src);
    img_t *dst0 = _dst;
    IM_BEGIN(im, 10);

    int d0 = CC_MAT_CN(src) > 1 ? CC_MAT_CN(src) : MIN(src->cols, src->rows);

    if( CC_MAT_CN(src) == 1 && src->cols > d0 )
        src = cvTranspose(_src, im++);

    int d1 = CC_MAT_CN(dst0) > 1 ? CC_MAT_CN(dst0) : MIN(dst0->cols, dst0->rows);

    img_t *dst = cvCloneMat(im++, _dst);
    if( d0 == d1 )
        cvCloneMat(dst, src);
    else if( d0 < d1 )
        convertPointsToHomogeneous(src, dst);
    else
        convertPointsFromHomogeneous(src, dst);

    bool tflag = CC_MAT_CN(dst0) == 1 && dst0->cols > d1;
    dst = cvReshape(dst, dst, CC_MAT_CN(dst0), (tflag ? dst0->cols : dst0->rows));

    if( tflag )
    {
        CC_Assert( dst->rows == dst0->cols && dst->cols == dst0->rows );
        if( dst0->tid == dst->tid )
            cvTranspose( dst, dst0 );
        else
        {
            cvTranspose( dst, dst );
            cvConvertTo( dst, dst0, dst0->tid );
        }
    }
    else
    {
        CC_Assert( CC_ARE_SIZES_EQ(dst, dst0) );
        if( dst->tt.data != dst0->tt.data )
            cvConvertTo(dst, dst0, dst0->tid);
    }
    IM_END(im);
}

