
/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

static int hal_LU32f(float* A, size_t astep, int m, float* b, size_t bstep, int n)
{
    CC_INSTRUMENT_REGION()

    int output=0;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, (A, astep, m, b, bstep, n, &output))
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    return output;
}

static int hal_LU64f(double* A, size_t astep, int m, double* b, size_t bstep, int n)
{
    CC_INSTRUMENT_REGION()

    int output=0;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, (A, astep, m, b, bstep, n, &output))
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    return output;
}

// <limits>
#define CC_EPSILON(_Tp)  (sizeof(_Tp)==sizeof(float) ? FLT_EPSILON : DBL_EPSILON)


static int hal_Cholesky32f(float* A, size_t astep, int m, float* b, size_t bstep, int n)
{
    CC_INSTRUMENT_REGION()

    int output = 0;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, (A, astep, m, b, bstep, n, &output))
	  //CALL_HAL_RET(Cholesky32f, openblas_Cholesky32f, output, (A, astep, m, b, bstep, n, &output))
#if 1
    if (1) {
      bool ret;
      //utime_start(_start_time);
      ret = CholImpl1(A, astep, m, b, bstep, n);
      //printf("%fms\n", 1000*utime_elapsed(_start_time));
      return ret;
    }
#endif
    return CholImpl(A, astep, m, b, bstep, n);
}

static int hal_Cholesky64f(double* A, size_t astep, int m, double* b, size_t bstep, int n)
{
    CC_INSTRUMENT_REGION()

    int output = 0;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, (A, astep, m, b, bstep, n, &output))
    return CholImpl(A, astep, m, b, bstep, n);
}

static int hal_QR32f(float* A, size_t astep, int m, int n, int k, float* b, size_t bstep, float* hFactors)
{
    CC_INSTRUMENT_REGION()

    int output=0;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, (A, astep, m, n, k, b, bstep, hFactors, &output));
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    return output;
}

static int hal_QR64f(double* A, size_t astep, int m, int n, int k, double* b, size_t bstep, double* hFactors)
{
    CC_INSTRUMENT_REGION()

    int output = 0;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, (A, astep, m, n, k, b, bstep, hFactors, &output))
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    return output;
}

#if 0

//=============================================================================
// for compatibility with 3.0

int LU(float* A, size_t astep, int m, float* b, size_t bstep, int n)
{
    return LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

int LU(double* A, size_t astep, int m, double* b, size_t bstep, int n)
{
    return LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

bool Cholesky(float* A, size_t astep, int m, float* b, size_t bstep, int n)
{
    return CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky(double* A, size_t astep, int m, double* b, size_t bstep, int n)
{
    return CholImpl(A, astep, m, b, bstep, n);
}

#endif
