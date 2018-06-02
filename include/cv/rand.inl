
/* ////////////////////////////////////////////////////////////////////
//
//  Filling img_t/img_t instances with random numbers
//
// */
typedef int64 CRNG;

CC_INLINE CRNG cRNG(int64 seed CC_DEFAULT(-1))
{
    CRNG rng = seed ? (uint64)seed : (uint64)(int64)-1;
    return rng;
}

CC_INLINE unsigned rng_int(CRNG* rng)
{
    (*rng) = (uint64)(unsigned)(*rng) * /*CC_RNG_COEFF*/ 4164903690U + (unsigned)((*rng) >> 32);
    return (unsigned)(*rng);
}

CC_INLINE float rng_float(CRNG* rng)    {
    return rng_int(rng)*2.3283064365386962890625e-10f;
}
CC_INLINE double rng_double(CRNG* rng) {
    unsigned t = rng_int(rng);
    return (((uint64)t << 32) | rng_int(rng)) * 5.4210108624275221700372640043497e-20;
}

CC_INLINE int    RNG_uniformi(CRNG* rng, int a, int b)       { return a == b ? a : (int)(rng_int(rng) % (b - a) + a); }
CC_INLINE float  RNG_uniformf(CRNG* rng, float a, float b)   { return rng_float(rng)*(b - a) + a; }
CC_INLINE double RNG_uniformd(CRNG* rng, double a, double b) { return rng_double(rng)*(b - a) + a; }

/* returns random 32-bit unsigned integer */
CC_INLINE unsigned cvRandInt(CRNG* rng)
{
    uint64 temp = *rng;
    temp = (uint64)(unsigned)temp*1554115554 + (temp >> 32);
    *rng = temp;
    return (unsigned)temp;
}

/* returns random floating-point number between 0 and 1 */
CC_INLINE double cvRandReal(CRNG* rng)
{
    return cvRandInt(rng)*2.3283064365386962890625e-10 /* 2^-32 */;
}


///////////////////////////// Functions Declaration //////////////////////////////////////

/*
   Multiply-with-carry generator is used here:
   temp = (A*X(n) + carry)
   X(n+1) = temp mod (2^32)
   carry = temp / (2^32)
*/
#define  ICC_RNG_NEXT(x)    ((uint64)(unsigned)(x)*1554115554 + ((x) >> 32))
#define  ICC_CVT_FLT(x)     (((unsigned)(x) >> 9)|CC_1F)
#define  ICC_1D             CC_BIG_INT(0x3FF0000000000000)
#define  ICC_CVT_DBL(x)     (((uint64)(unsigned)(x) << 20)|((x) >> 44)|ICC_1D)

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

#define ICC_IMPL_RAND_BITS(flavor, arrtype, cast_macro)               \
static CStatus C_STDCALL                                              \
icvRandBits_##flavor##_C1R(arrtype* arr, int step, CSize size,        \
                            uint64* state, const int* param)           \
{                                                                       \
    uint64 temp = *state;                                               \
    int small_flag = (param[12]|param[13]|param[14]|param[15]) <= 255;  \
    step /= sizeof(arr[0]);                                             \
                                                                        \
    for(; size.height--; arr += step)                                 \
    {                                                                   \
        int i, k = 3;                                                   \
        const int* p = param;                                           \
                                                                        \
        if(!small_flag)                                               \
        {                                                               \
            for(i = 0; i <= size.width - 4; i += 4)                   \
            {                                                           \
                unsigned t0, t1;                                        \
                                                                        \
                temp = ICC_RNG_NEXT(temp);                              \
                t0 = ((unsigned)temp & p[i + 12]) + p[i];               \
                temp = ICC_RNG_NEXT(temp);                              \
                t1 = ((unsigned)temp & p[i + 13]) + p[i+1];             \
                arr[i] = cast_macro((int)t0);                           \
                arr[i+1] = cast_macro((int)t1);                         \
                                                                        \
                temp = ICC_RNG_NEXT(temp);                              \
                t0 = ((unsigned)temp & p[i + 14]) + p[i+2];             \
                temp = ICC_RNG_NEXT(temp);                              \
                t1 = ((unsigned)temp & p[i + 15]) + p[i+3];             \
                arr[i+2] = cast_macro((int)t0);                         \
                arr[i+3] = cast_macro((int)t1);                         \
                                                                        \
                if(!--k)                                              \
                {                                                       \
                    k = 3;                                              \
                    p -= 12;                                            \
                }                                                       \
            }                                                           \
        }                                                               \
        else                                                            \
        {                                                               \
            for(i = 0; i <= size.width - 4; i += 4)                   \
            {                                                           \
                unsigned t0, t1, t;                                     \
                                                                        \
                temp = ICC_RNG_NEXT(temp);                              \
                t = (unsigned)temp;                                     \
                t0 = (t & p[i + 12]) + p[i];                            \
                t1 = ((t >> 8) & p[i + 13]) + p[i+1];                   \
                arr[i] = cast_macro((int)t0);                           \
                arr[i+1] = cast_macro((int)t1);                         \
                                                                        \
                t0 = ((t >> 16) & p[i + 14]) + p[i + 2];                \
                t1 = ((t >> 24) & p[i + 15]) + p[i + 3];                \
                arr[i+2] = cast_macro((int)t0);                         \
                arr[i+3] = cast_macro((int)t1);                         \
                                                                        \
                if(!--k)                                              \
                {                                                       \
                    k = 3;                                              \
                    p -= 12;                                            \
                }                                                       \
            }                                                           \
        }                                                               \
                                                                        \
        for(; i < size.width; i++)                                    \
        {                                                               \
            unsigned t0;                                                \
            temp = ICC_RNG_NEXT(temp);                                  \
                                                                        \
            t0 = ((unsigned)temp & p[i + 12]) + p[i];                   \
            arr[i] = cast_macro((int)t0);                               \
        }                                                               \
    }                                                                   \
                                                                        \
    *state = temp;                                                      \
    return CC_OK;                                                       \
}


#define ICC_IMPL_RAND(flavor, arrtype, worktype, cast_macro1, cast_macro2)\
static CStatus C_STDCALL                                              \
icvRand_##flavor##_C1R(arrtype* arr, int step, CSize size,            \
                        uint64* state, const double* param)            \
{                                                                       \
    uint64 temp = *state;                                               \
    step /= sizeof(arr[0]);                                             \
                                                                        \
    for(; size.height--; arr += step)                                 \
    {                                                                   \
        int i, k = 3;                                                   \
        const double* p = param;                                        \
                                                                        \
        for(i = 0; i <= size.width - 4; i += 4)                       \
        {                                                               \
            worktype f0, f1;                                            \
            suf32_t t0, t1;                                             \
                                                                        \
            temp = ICC_RNG_NEXT(temp);                                  \
            t0.u = ICC_CVT_FLT(temp);                                   \
            temp = ICC_RNG_NEXT(temp);                                  \
            t1.u = ICC_CVT_FLT(temp);                                   \
            f0 = cast_macro1(t0.f * p[i + 12] + p[i]);                \
            f1 = cast_macro1(t1.f * p[i + 13] + p[i + 1]);            \
            arr[i] = cast_macro2(f0);                                   \
            arr[i+1] = cast_macro2(f1);                                 \
                                                                        \
            temp = ICC_RNG_NEXT(temp);                                  \
            t0.u = ICC_CVT_FLT(temp);                                   \
            temp = ICC_RNG_NEXT(temp);                                  \
            t1.u = ICC_CVT_FLT(temp);                                   \
            f0 = cast_macro1(t0.f * p[i + 14] + p[i + 2]);            \
            f1 = cast_macro1(t1.f * p[i + 15] + p[i + 3]);            \
            arr[i+2] = cast_macro2(f0);                                 \
            arr[i+3] = cast_macro2(f1);                                 \
                                                                        \
            if(!--k)                                                  \
            {                                                           \
                k = 3;                                                  \
                p -= 12;                                                \
            }                                                           \
        }                                                               \
                                                                        \
        for(; i < size.width; i++)                                    \
        {                                                               \
            worktype f0;                                                \
            suf32_t t0;                                                 \
                                                                        \
            temp = ICC_RNG_NEXT(temp);                                  \
            t0.u = ICC_CVT_FLT(temp);                                   \
            f0 = cast_macro1(t0.f * p[i + 12] + p[i]);                \
            arr[i] = cast_macro2(f0);                                   \
        }                                                               \
    }                                                                   \
                                                                        \
    *state = temp;                                                      \
    return CC_OK;                                                       \
}


static CStatus C_STDCALL
icvRand_64f_C1R(double* arr, int step, CSize size,
                 uint64* state, const double* param)
{
    uint64 temp = *state;
    step /= sizeof(arr[0]);

    for(; size.height--; arr += step)
    {
        int i, k = 3;
        const double* p = param;

        for(i = 0; i <= size.width - 4; i += 4)
        {
            double f0, f1;
            suf64_t t0, t1;

            temp = ICC_RNG_NEXT(temp);
            t0.u = ICC_CVT_DBL(temp);
            temp = ICC_RNG_NEXT(temp);
            t1.u = ICC_CVT_DBL(temp);
            f0 = t0.f * p[i + 12] + p[i];
            f1 = t1.f * p[i + 13] + p[i + 1];
            arr[i] = f0;
            arr[i+1] = f1;

            temp = ICC_RNG_NEXT(temp);
            t0.u = ICC_CVT_DBL(temp);
            temp = ICC_RNG_NEXT(temp);
            t1.u = ICC_CVT_DBL(temp);
            f0 = t0.f * p[i + 14] + p[i + 2];
            f1 = t1.f * p[i + 15] + p[i + 3];
            arr[i+2] = f0;
            arr[i+3] = f1;

            if(!--k)
            {
                k = 3;
                p -= 12;
            }
        }

        for(; i < size.width; i++)
        {
            double f0;
            suf64_t t0;

            temp = ICC_RNG_NEXT(temp);
            t0.u = ICC_CVT_DBL(temp);
            f0 = t0.f * p[i + 12] + p[i];
            arr[i] = f0;
        }
    }

    *state = temp;
    return CC_OK;
}


/***************************************************************************************\
    The code below implements algorithm from the paper:

    G. Marsaglia and W.W. Tsang,
    The Monty Python method for generating random variables,
    ACM Transactions on Mathematical Software, Vol. 24, No. 3,
    Pages 341-350, September, 1998.
\***************************************************************************************/

static CStatus C_STDCALL
icvRandn_0_1_32f_C1R(float* arr, int len, uint64* state)
{
    uint64 temp = *state;
    int i;
    temp = ICC_RNG_NEXT(temp);

    for(i = 0; i < len; i++)
    {
        double x, y, v, ax, bx;

        for(;;)
        {
            x = ((int)temp)*1.167239e-9;
            temp = ICC_RNG_NEXT(temp);
            ax = fabs(x);
            v = 2.8658 - ax*(2.0213 - 0.3605*ax);
            y = ((unsigned)temp)*2.328306e-10;
            temp = ICC_RNG_NEXT(temp);

            if(y < v || ax < 1.17741)
                break;

            bx = x;
            x = bx > 0 ? 0.8857913*(2.506628 - ax) : -0.8857913*(2.506628 - ax);
            
            if(y > v + 0.0506)
                break;

            if(log(y) < .6931472 - .5*bx*bx)
            {
                x = bx;
                break;
            }

            if(log(1.8857913 - y) < .5718733-.5*x*x)
                break;

            do
            {
                v = ((int)temp)*4.656613e-10;
                x = -log(fabs(v))*.3989423;
                temp = ICC_RNG_NEXT(temp);
                y = -log(((unsigned)temp)*2.328306e-10);
                temp = ICC_RNG_NEXT(temp);
            }
            while(y+y < x*x);

            x = v > 0 ? 2.506628 + x : -2.506628 - x;
            break;
        }

        arr[i] = (float)x;
    }
    *state = temp;
    return CC_OK;
}


#define RAND_BUF_SIZE  96


#define ICC_IMPL_RANDN(flavor, arrtype, worktype, cast_macro1, cast_macro2)   \
static CStatus C_STDCALL                                                      \
icvRandn_##flavor##_C1R(arrtype* arr, int step, CSize size,                   \
                         uint64* state, const double* param)                   \
{                                                                               \
    float buffer[RAND_BUF_SIZE];                                                \
    step /= sizeof(arr[0]);                                                     \
                                                                                \
    for(; size.height--; arr += step)                                         \
    {                                                                           \
        int i, j, len = RAND_BUF_SIZE;                                          \
                                                                                \
        for(i = 0; i < size.width; i += RAND_BUF_SIZE)                        \
        {                                                                       \
            int k = 3;                                                          \
            const double* p = param;                                            \
                                                                                \
            if(i + len > size.width)                                          \
                len = size.width - i;                                           \
                                                                                \
            icvRandn_0_1_32f_C1R(buffer, len, state);                         \
                                                                                \
            for(j = 0; j <= len - 4; j += 4)                                  \
            {                                                                   \
                worktype f0, f1;                                                \
                                                                                \
                f0 = cast_macro1(buffer[j]*p[j+12] + p[j]);                   \
                f1 = cast_macro1(buffer[j+1]*p[j+13] + p[j+1]);               \
                arr[i+j] = cast_macro2(f0);                                     \
                arr[i+j+1] = cast_macro2(f1);                                   \
                                                                                \
                f0 = cast_macro1(buffer[j+2]*p[j+14] + p[j+2]);               \
                f1 = cast_macro1(buffer[j+3]*p[j+15] + p[j+3]);               \
                arr[i+j+2] = cast_macro2(f0);                                   \
                arr[i+j+3] = cast_macro2(f1);                                   \
                                                                                \
                if(--k == 0)                                                  \
                {                                                               \
                    k = 3;                                                      \
                    p -= 12;                                                    \
                }                                                               \
            }                                                                   \
                                                                                \
            for(; j < len; j++)                                               \
            {                                                                   \
                worktype f0 = cast_macro1(buffer[j]*p[j+12] + p[j]);          \
                arr[i+j] = cast_macro2(f0);                                     \
            }                                                                   \
        }                                                                       \
    }                                                                           \
                                                                                \
    return CC_OK;                                                               \
}


ICC_IMPL_RAND_BITS(8u, uchar, CC_CAST_8U)
ICC_IMPL_RAND_BITS(16u, ushort, CC_CAST_16U)
ICC_IMPL_RAND_BITS(16s, short, CC_CAST_16S)
ICC_IMPL_RAND_BITS(32s, int, CC_CAST_32S)

ICC_IMPL_RAND(8u, uchar, int, cFloor, CC_CAST_8U)
ICC_IMPL_RAND(16u, ushort, int, cFloor, CC_CAST_16U)
ICC_IMPL_RAND(16s, short, int, cFloor, CC_CAST_16S)
ICC_IMPL_RAND(32s, int, int, cFloor, CC_CAST_32S)
ICC_IMPL_RAND(32f, float, float, CC_CAST_32F, CC_NOP)

ICC_IMPL_RANDN(8u, uchar, int, cRound, CC_CAST_8U)
ICC_IMPL_RANDN(16u, ushort, int, cRound, CC_CAST_16U)
ICC_IMPL_RANDN(16s, short, int, cRound, CC_CAST_16S)
ICC_IMPL_RANDN(32s, int, int, cRound, CC_CAST_32S)
ICC_IMPL_RANDN(32f, float, float, CC_CAST_32F, CC_NOP)
ICC_IMPL_RANDN(64f, double, double, CC_CAST_64F, CC_NOP)

static void icvInitRandTable(CvFuncTable  fastrng_tab,
                              CvFuncTable  rng_tab,
                              CvFuncTable  normal_tab)
{
    fastrng_tab[CC_8U] = (void*)icvRandBits_8u_C1R;
    fastrng_tab[CC_8S] = 0;
    fastrng_tab[CC_16U] = (void*)icvRandBits_16u_C1R;
    fastrng_tab[CC_16S] = (void*)icvRandBits_16s_C1R;
    fastrng_tab[CC_32S] = (void*)icvRandBits_32s_C1R;

    rng_tab[CC_8U] = (void*)icvRand_8u_C1R;
    rng_tab[CC_8S] = 0;
    rng_tab[CC_16U] = (void*)icvRand_16u_C1R;
    rng_tab[CC_16S] = (void*)icvRand_16s_C1R;
    rng_tab[CC_32S] = (void*)icvRand_32s_C1R;
    rng_tab[CC_32F] = (void*)icvRand_32f_C1R;
    rng_tab[CC_64F] = (void*)icvRand_64f_C1R;

    normal_tab[CC_8U] = (void*)icvRandn_8u_C1R;
    normal_tab[CC_8S] = 0;
    normal_tab[CC_16U] = (void*)icvRandn_16u_C1R;
    normal_tab[CC_16S] = (void*)icvRandn_16s_C1R;
    normal_tab[CC_32S] = (void*)icvRandn_32s_C1R;
    normal_tab[CC_32F] = (void*)icvRandn_32f_C1R;
    normal_tab[CC_64F] = (void*)icvRandn_64f_C1R;
}


CC_IMPL void
cvRandArr(CRNG* rng, img_t* arr, CRandType disttype, CScalar param1, CScalar param2)
{
    static CvFuncTable rng_tab[2], fastrng_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvRandArr");

    __BEGIN__;

    img_t *mat = arr;
    TypeId type;
    int channels;
    double dparam[2][12];
    int iparam[2][12];
    void* param = dparam;
    int i, fast_int_mode = 0;
    int mat_step = 0;
    CSize size;
    CRNG _rng = 10;
    CvFunc2D_1A2P func = 0;

    if(!inittab)
    {
        icvInitRandTable(fastrng_tab, rng_tab[CC_RAND_UNI], rng_tab[CC_RAND_NORMAL]);
        inittab = 1;
    }

    if(!rng) {
      rng = &_rng;
    }

    {
        type = CC_MAT_DEPTH(mat);
        size = cvGetMatSize(mat);
        mat_step = mat->step;

        if(mat->height > 1 && CC_IS_MAT_CONT(mat))
        {
            size.width *= size.height;
            mat_step = CC_STUB_STEP;
            size.height = 1;
        }
    }

    
    channels = CC_MAT_CN(mat);
    size.width *= channels;

    if(disttype == CC_RAND_UNI)
    {
        if(type <= CC_32S)
        {
            for(i = 0, fast_int_mode = 1; i < channels; i++)
            {
                int t0 = iparam[0][i] = cCeil(param1.val[i]);
                int t1 = iparam[1][i] = cFloor(param2.val[i]) - t0;
                double diff = param1.val[i] - param2.val[i];

                fast_int_mode &= INT_MIN <= diff && diff <= INT_MAX && (t1 & (t1 - 1)) == 0;
            }
        }

        if(fast_int_mode)
        {
            for(i = 0; i < channels; i++)
                iparam[1][i]--;
        
            for(; i < 12; i++)
            {
                int t0 = iparam[0][i - channels];
                int t1 = iparam[1][i - channels];

                iparam[0][i] = t0;
                iparam[1][i] = t1;
            }

            CC_GET_FUNC_PTR(func, (CvFunc2D_1A2P)(fastrng_tab[type]));
            param = iparam;
        }
        else
        {
            for(i = 0; i < channels; i++)
            {
                double t0 = param1.val[i];
                double t1 = param2.val[i];

                dparam[0][i] = t0 - (t1 - t0);
                dparam[1][i] = t1 - t0;
            }

            CC_GET_FUNC_PTR(func, (CvFunc2D_1A2P)(rng_tab[0][type]));
        }
    }
    else if(disttype == CC_RAND_NORMAL)
    {
        for(i = 0; i < channels; i++)
        {
            double t0 = param1.val[i];
            double t1 = param2.val[i];

            dparam[0][i] = t0;
            dparam[1][i] = t1;
        }

        CC_GET_FUNC_PTR(func, (CvFunc2D_1A2P)(rng_tab[1][type]));
    }
    else
    {
        CC_ERROR(CC_StsBadArg, "Unknown distribution type");
    }

    if(!fast_int_mode)
    {
        for(i = channels; i < 12; i++)
        {
            double t0 = dparam[0][i - channels];
            double t1 = dparam[1][i - channels];

            dparam[0][i] = t0;
            dparam[1][i] = t1;
        }
    }

    IPPI_CALL(func(mat->tt.data, mat_step, size, rng, param));
    
    __END__;
}


