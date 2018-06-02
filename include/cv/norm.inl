

/****************************************************************************************\
*                                         N o r m                                        *
\****************************************************************************************/

#define ICC_NORM_CASE(_op_,                \
    _update_op_, worktype, len)            \
                                            \
    for(; x <= (len) - 4; x += 4)         \
    {                                       \
        worktype t0 = (src)[x];             \
        worktype t1 = (src)[x+1];           \
        t0 = _op_(t0);                      \
        t1 = _op_(t1);                      \
        norm = _update_op_(norm, t0);     \
        norm = _update_op_(norm, t1);     \
                                            \
        t0 = (src)[x+2];                    \
        t1 = (src)[x+3];                    \
        t0 = _op_(t0);                      \
        t1 = _op_(t1);                      \
        norm = _update_op_(norm, t0);     \
        norm = _update_op_(norm, t1);     \
    }                                       \
                                            \
    for(; x < (len); x++)                 \
    {                                       \
        worktype t0 = (src)[x];             \
        t0 = (worktype)_op_(t0);            \
        norm = _update_op_(norm, t0);     \
    }


#define ICC_NORM_COI_CASE(_op_,            \
    _update_op_, worktype, len, cn)        \
                                            \
    for(; x < (len); x++)                 \
    {                                       \
        worktype t0 = (src)[x*(cn)];        \
        t0 = (worktype)_op_(t0);            \
        norm = _update_op_(norm, t0);     \
    }


#define ICC_NORM_DIFF_CASE(_op_,           \
    _update_op_, worktype, len)            \
                                            \
    for(; x <= (len) - 4; x += 4)         \
    {                                       \
        worktype t0 = (src1)[x] - (src2)[x];\
        worktype t1 = (src1)[x+1]-(src2)[x+1];\
                                            \
        t0 = _op_(t0);                      \
        t1 = _op_(t1);                      \
                                            \
        norm = _update_op_(norm, t0);     \
        norm = _update_op_(norm, t1);     \
                                            \
        t0 = (src1)[x+2] - (src2)[x+2];     \
        t1 = (src1)[x+3] - (src2)[x+3];     \
                                            \
        t0 = _op_(t0);                      \
        t1 = _op_(t1);                      \
                                            \
        norm = _update_op_(norm, t0);     \
        norm = _update_op_(norm, t1);     \
    }                                       \
                                            \
    for(; x < (len); x++)                 \
    {                                       \
        worktype t0 = (src1)[x] - (src2)[x];\
        t0 = (worktype)_op_(t0);            \
        norm = _update_op_(norm, t0);     \
    }


#define ICC_NORM_DIFF_COI_CASE(_op_, _update_op_, worktype, len, cn) \
    for(; x < (len); x++)                                     \
    {                                                           \
        worktype t0 = (src1)[x*(cn)] - (src2)[x*(cn)];          \
        t0 = (worktype)_op_(t0);                                \
        norm = _update_op_(norm, t0);                         \
    }


/*
 	The algorithm and its multiple variations below
    below accumulates the norm by blocks of size "block_size".
    Each block may span across multiple lines and it is
    not necessary aligned by row boundaries. Within a block
    the norm is accumulated to intermediate light-weight
    type (worktype). It really makes sense for 8u, 16s, 16u types
    and L1 & L2 norms, where worktype==int and normtype==int64.
    In other cases a simpler algorithm is used
*/
#define  ICC_DEF_NORM_NOHINT_BLOCK_FUNC_2D(name, _op_, _update_op_, \
    post_func, arrtype, normtype, worktype, block_size)        \
IPCVAPI_IMPL(CStatus, name, (const arrtype* src, int step,   \
    CSize size, double* _norm), (src, step, size, _norm))    \
{                                                               \
    int remaining = block_size;                                 \
    normtype total_norm = 0;                                    \
    worktype norm = 0;                                          \
    step /= sizeof(src[0]);                                     \
                                                                \
    for(; size.height--; src += step)                         \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                 \
        {                                                       \
            int limit = MIN(remaining, size.width - x);       \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_NORM_CASE(_op_, _update_op_, worktype, limit);\
            if(remaining == 0)                                \
            {                                                   \
                remaining = block_size;                         \
                total_norm += (normtype)norm;                   \
                norm = 0;                                       \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    total_norm += (normtype)norm;                               \
    *_norm = post_func((double)total_norm);                     \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_NOHINT_FUNC_2D(name, _op_, _update_op_,  \
    post_func, arrtype, normtype, worktype, block_size)        \
IPCVAPI_IMPL(CStatus, name, (const arrtype* src, int step,   \
    CSize size, double* _norm), (src, step, size, _norm))    \
{                                                               \
    normtype norm = 0;                                          \
    step /= sizeof(src[0]);                                     \
                                                                \
    for(; size.height--; src += step)                         \
    {                                                           \
        int x = 0;                                              \
        ICC_NORM_CASE(_op_, _update_op_, worktype, size.width); \
    }                                                           \
                                                                \
    *_norm = post_func((double)norm);                           \
    return CC_OK;                                               \
}


/*
   In IPP only 32f flavors of norm functions are with hint.
   For float worktype==normtype==double, thus the block algorithm,
   described above, is not necessary.
 */
#define  ICC_DEF_NORM_HINT_FUNC_2D(name, _op_, _update_op_,    \
    post_func, arrtype, normtype, worktype, block_size)        \
IPCVAPI_IMPL(CStatus, name, (const arrtype* src, int step,   \
    CSize size, double* _norm, CvHintAlgorithm hint),     \
    (src, step, size, _norm, cvAlgHintAccurate))               \
{                                                               \
    normtype norm = 0;                                          \
    step /= sizeof(src[0]);                                     \
                                                                \
    for(; size.height--; src += step)                         \
    {                                                           \
        int x = 0;                                              \
        ICC_NORM_CASE(_op_, _update_op_, worktype, size.width); \
    }                                                           \
                                                                \
    *_norm = post_func((double)norm);                           \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_NOHINT_BLOCK_FUNC_2D_COI(name, _op_,     \
    _update_op_, post_func, arrtype,                            \
    normtype, worktype, block_size)                            \
static CStatus C_STDCALL name(const arrtype* src, int step,  \
                CSize size, int cn, int coi, double* _norm)   \
{                                                               \
    int remaining = block_size;                                 \
    normtype total_norm = 0;                                    \
    worktype norm = 0;                                          \
    step /= sizeof(src[0]);                                     \
    src += coi - 1;                                             \
                                                                \
    for(; size.height--; src += step)                         \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                 \
        {                                                       \
            int limit = MIN(remaining, size.width - x);       \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_NORM_COI_CASE(_op_, _update_op_,               \
                               worktype, limit, cn);           \
            if(remaining == 0)                                \
            {                                                   \
                remaining = block_size;                         \
                total_norm += (normtype)norm;                   \
                norm = 0;                                       \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    total_norm += (normtype)norm;                               \
    *_norm = post_func((double)total_norm);                     \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_NOHINT_FUNC_2D_COI(name, _op_,           \
    _update_op_, post_func,                                     \
    arrtype, normtype, worktype, block_size)                   \
static CStatus C_STDCALL name(const arrtype* src, int step,  \
                CSize size, int cn, int coi, double* _norm)   \
{                                                               \
    normtype norm = 0;                                          \
    step /= sizeof(src[0]);                                     \
    src += coi - 1;                                             \
                                                                \
    for(; size.height--; src += step)                         \
    {                                                           \
        int x = 0;                                              \
        ICC_NORM_COI_CASE(_op_, _update_op_,                   \
                           worktype, size.width, cn);          \
    }                                                           \
                                                                \
    *_norm = post_func((double)norm);                           \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_DIFF_NOHINT_BLOCK_FUNC_2D(name, _op_,    \
    _update_op_, post_func, arrtype,                            \
    normtype, worktype, block_size)                            \
IPCVAPI_IMPL(CStatus, name,(const arrtype* src1, int step1,  \
    const arrtype* src2, int step2, CSize size, double* _norm),\
   (src1, step1, src2, step2, size, _norm))                     \
{                                                               \
    int remaining = block_size;                                 \
    normtype total_norm = 0;                                    \
    worktype norm = 0;                                          \
    step1 /= sizeof(src1[0]);                                   \
    step2 /= sizeof(src2[0]);                                   \
                                                                \
    for(; size.height--; src1 += step1, src2 += step2)        \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                 \
        {                                                       \
            int limit = MIN(remaining, size.width - x);       \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_NORM_DIFF_CASE(_op_, _update_op_,              \
                                worktype, limit);              \
            if(remaining == 0)                                \
            {                                                   \
                remaining = block_size;                         \
                total_norm += (normtype)norm;                   \
                norm = 0;                                       \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    total_norm += (normtype)norm;                               \
    *_norm = post_func((double)total_norm);                     \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_DIFF_NOHINT_FUNC_2D(name, _op_,          \
    _update_op_, post_func,                                     \
    arrtype, normtype, worktype, block_size)                   \
IPCVAPI_IMPL(CStatus, name,(const arrtype* src1, int step1,  \
    const arrtype* src2, int step2, CSize size, double* _norm),\
    (src1, step1, src2, step2, size, _norm))                  \
{                                                               \
    normtype norm = 0;                                          \
    step1 /= sizeof(src1[0]);                                   \
    step2 /= sizeof(src2[0]);                                   \
                                                                \
    for(; size.height--; src1 += step1, src2 += step2)        \
    {                                                           \
        int x = 0;                                              \
        ICC_NORM_DIFF_CASE(_op_, _update_op_,                  \
                            worktype, size.width);             \
    }                                                           \
                                                                \
    *_norm = post_func((double)norm);                           \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_DIFF_HINT_FUNC_2D(name, _op_,            \
    _update_op_, post_func,                                     \
    arrtype, normtype, worktype, block_size)                   \
IPCVAPI_IMPL(CStatus, name,(const arrtype* src1, int step1,  \
    const arrtype* src2, int step2, CSize size, double* _norm, \
    CvHintAlgorithm hint),                                 \
    (src1, step1, src2, step2, size, _norm, cvAlgHintAccurate))\
{                                                               \
    normtype norm = 0;                                          \
    step1 /= sizeof(src1[0]);                                   \
    step2 /= sizeof(src2[0]);                                   \
                                                                \
    for(; size.height--; src1 += step1, src2 += step2)        \
    {                                                           \
        int x = 0;                                              \
        ICC_NORM_DIFF_CASE(_op_, _update_op_,                  \
                            worktype, size.width);             \
    }                                                           \
                                                                \
    *_norm = post_func((double)norm);                           \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_DIFF_NOHINT_BLOCK_FUNC_2D_COI(name, _op_,\
    _update_op_, post_func, arrtype,                            \
    normtype, worktype, block_size)                            \
static CStatus C_STDCALL name(const arrtype* src1, int step1,\
    const arrtype* src2, int step2, CSize size,                \
    int cn, int coi, double* _norm)                            \
{                                                               \
    int remaining = block_size;                                 \
    normtype total_norm = 0;                                    \
    worktype norm = 0;                                          \
    step1 /= sizeof(src1[0]);                                   \
    step2 /= sizeof(src2[0]);                                   \
    src1 += coi - 1;                                            \
    src2 += coi - 1;                                            \
                                                                \
    for(; size.height--; src1 += step1, src2 += step2)        \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                 \
        {                                                       \
            int limit = MIN(remaining, size.width - x);       \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_NORM_DIFF_COI_CASE(_op_, _update_op_,          \
                                    worktype, limit, cn);      \
            if(remaining == 0)                                \
            {                                                   \
                remaining = block_size;                         \
                total_norm += (normtype)norm;                   \
                norm = 0;                                       \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    total_norm += (normtype)norm;                               \
    *_norm = post_func((double)total_norm);                     \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_DIFF_NOHINT_FUNC_2D_COI(name, _op_,      \
    _update_op_, post_func,                                     \
    arrtype, normtype, worktype, block_size)                   \
static CStatus C_STDCALL name(const arrtype* src1, int step1,\
    const arrtype* src2, int step2, CSize size,                \
    int cn, int coi, double* _norm)                            \
{                                                               \
    normtype norm = 0;                                          \
    step1 /= sizeof(src1[0]);                                   \
    step2 /= sizeof(src2[0]);                                   \
    src1 += coi - 1;                                            \
    src2 += coi - 1;                                            \
                                                                \
    for(; size.height--; src1 += step1, src2 += step2)        \
    {                                                           \
        int x = 0;                                              \
        ICC_NORM_DIFF_COI_CASE(_op_, _update_op_,              \
                                worktype, size.width, cn);     \
    }                                                           \
                                                                \
    *_norm = post_func((double)norm);                           \
    return CC_OK;                                               \
}


/****************************************************************************************\
*                             N o r m   with    M A S K                                  *
\****************************************************************************************/

#define ICC_NORM_MASK_CASE(_op_,               \
        _update_op_, worktype, len)            \
{                                               \
    for(; x <= (len) - 2; x += 2)             \
    {                                           \
        worktype t0;                            \
        if(mask[x])                           \
        {                                       \
            t0 = (src)[x];                      \
            t0 = _op_(t0);                      \
            norm = _update_op_(norm, t0);     \
        }                                       \
        if(mask[x+1])                         \
        {                                       \
            t0 = (src)[x+1];                    \
            t0 = _op_(t0);                      \
            norm = _update_op_(norm, t0);     \
        }                                       \
    }                                           \
                                                \
    for(; x < (len); x++)                     \
        if(mask[x])                           \
        {                                       \
            worktype t0 = (src)[x];             \
            t0 = _op_(t0);                      \
            norm = _update_op_(norm, t0);     \
        }                                       \
}


#define ICC_NORM_DIFF_MASK_CASE(_op_, _update_op_, worktype, len) \
{                                               \
    for(; x <= (len) - 2; x += 2)             \
    {                                           \
        worktype t0;                            \
        if(mask[x])                           \
        {                                       \
            t0 = (src1)[x] - (src2)[x];         \
            t0 = _op_(t0);                      \
            norm = _update_op_(norm, t0);     \
        }                                       \
        if(mask[x+1])                         \
        {                                       \
            t0 = (src1)[x+1] - (src2)[x+1];     \
            t0 = _op_(t0);                      \
            norm = _update_op_(norm, t0);     \
        }                                       \
    }                                           \
                                                \
    for(; x < (len); x++)                     \
        if(mask[x])                           \
        {                                       \
            worktype t0 = (src1)[x] - (src2)[x];\
            t0 = _op_(t0);                      \
            norm = _update_op_(norm, t0);     \
        }                                       \
}


#define ICC_NORM_MASK_COI_CASE(_op_, _update_op_, worktype, len, cn) \
{                                               \
    for(; x < (len); x++)                     \
        if(mask[x])                           \
        {                                       \
            worktype t0 = (src)[x*(cn)];        \
            t0 = _op_(t0);                      \
            norm = _update_op_(norm, t0);     \
        }                                       \
}


#define ICC_NORM_DIFF_MASK_COI_CASE(_op_, _update_op_, worktype, len, cn)\
{                                               \
    for(; x < (len); x++)                     \
        if(mask[x])                           \
        {                                       \
            worktype t0 = (src1)[x*(cn)] - (src2)[x*(cn)];  \
            t0 = _op_(t0);                      \
            norm = _update_op_(norm, t0);     \
        }                                       \
}


#define  ICC_DEF_NORM_MASK_NOHINT_BLOCK_FUNC_2D(name, _op_,    \
    _update_op_, post_func, arrtype,                            \
    normtype, worktype, block_size)                            \
IPCVAPI_IMPL(CStatus, name, (const arrtype* src, int step,   \
    const uchar* mask, int maskstep, CSize size, double* _norm),\
    (src, step, mask, maskstep, size, _norm))                  \
{                                                               \
    int remaining = block_size;                                 \
    normtype total_norm = 0;                                    \
    worktype norm = 0;                                          \
    step /= sizeof(src[0]);                                     \
                                                                \
    for(; size.height--; src += step, mask += maskstep)       \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                 \
        {                                                       \
            int limit = MIN(remaining, size.width - x);       \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_NORM_MASK_CASE(_op_, _update_op_,              \
                                worktype, limit);              \
            if(remaining == 0)                                \
            {                                                   \
                remaining = block_size;                         \
                total_norm += (normtype)norm;                   \
                norm = 0;                                       \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    total_norm += (normtype)norm;                               \
    *_norm = post_func((double)total_norm);                     \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_MASK_NOHINT_FUNC_2D(name, _op_, _update_op_,\
    post_func, arrtype, normtype, worktype, block_size)        \
IPCVAPI_IMPL(CStatus, name, (const arrtype* src, int step,   \
    const uchar* mask, int maskstep, CSize size, double* _norm),\
    (src, step, mask, maskstep, size, _norm))                  \
{                                                               \
    normtype norm = 0;                                          \
    step /= sizeof(src[0]);                                     \
                                                                \
    for(; size.height--; src += step, mask += maskstep)       \
    {                                                           \
        int x = 0;                                              \
        ICC_NORM_MASK_CASE(_op_, _update_op_,                  \
                            worktype, size.width);             \
    }                                                           \
                                                                \
    *_norm = post_func((double)norm);                           \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_MASK_NOHINT_BLOCK_FUNC_2D_COI(name, _op_,\
                _update_op_, post_func, arrtype,                \
                normtype, worktype, block_size)                \
static CStatus C_STDCALL name(const arrtype* src, int step,  \
    const uchar* mask, int maskstep, CSize size,               \
    int cn, int coi, double* _norm)                            \
{                                                               \
    int remaining = block_size;                                 \
    normtype total_norm = 0;                                    \
    worktype norm = 0;                                          \
    step /= sizeof(src[0]);                                     \
    src += coi - 1;                                             \
                                                                \
    for(; size.height--; src += step, mask += maskstep)       \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                 \
        {                                                       \
            int limit = MIN(remaining, size.width - x);       \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_NORM_MASK_COI_CASE(_op_, _update_op_,          \
                                    worktype, limit, cn);      \
            if(remaining == 0)                                \
            {                                                   \
                remaining = block_size;                         \
                total_norm += (normtype)norm;                   \
                norm = 0;                                       \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    total_norm += (normtype)norm;                               \
    *_norm = post_func((double)total_norm);                     \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_MASK_NOHINT_FUNC_2D_COI(name, _op_,      \
    _update_op_, post_func,                                     \
    arrtype, normtype, worktype, block_size)                   \
static CStatus C_STDCALL name(const arrtype* src, int step,  \
    const uchar* mask, int maskstep, CSize size,               \
    int cn, int coi, double* _norm)                            \
{                                                               \
    normtype norm = 0;                                          \
    step /= sizeof(src[0]);                                     \
    src += coi - 1;                                             \
                                                                \
    for(; size.height--; src += step, mask += maskstep)       \
    {                                                           \
        int x = 0;                                              \
        ICC_NORM_MASK_COI_CASE(_op_, _update_op_,              \
                                worktype, size.width, cn);     \
    }                                                           \
                                                                \
    *_norm = post_func((double)norm);                           \
    return CC_OK;                                               \
}



#define  ICC_DEF_NORM_DIFF_MASK_NOHINT_BLOCK_FUNC_2D(name,     \
    _op_, _update_op_, post_func, arrtype,                      \
    normtype, worktype, block_size)                            \
IPCVAPI_IMPL(CStatus, name,(const arrtype* src1, int step1,  \
    const arrtype* src2, int step2, const uchar* mask,          \
    int maskstep, CSize size, double* _norm),                 \
    (src1, step1, src2, step2, mask, maskstep, size, _norm))   \
{                                                               \
    int remaining = block_size;                                 \
    normtype total_norm = 0;                                    \
    worktype norm = 0;                                          \
    step1 /= sizeof(src1[0]);                                   \
    step2 /= sizeof(src2[0]);                                   \
                                                                \
    for(; size.height--; src1 += step1, src2 += step2,         \
                          mask += maskstep)                    \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                 \
        {                                                       \
            int limit = MIN(remaining, size.width - x);       \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_NORM_DIFF_MASK_CASE(_op_, _update_op_,         \
                                     worktype, limit);         \
            if(remaining == 0)                                \
            {                                                   \
                remaining = block_size;                         \
                total_norm += (normtype)norm;                   \
                norm = 0;                                       \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    total_norm += (normtype)norm;                               \
    *_norm = post_func((double)total_norm);                     \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_DIFF_MASK_NOHINT_FUNC_2D(name, _op_,     \
    _update_op_, post_func,                                     \
    arrtype, normtype, worktype, block_size)                   \
IPCVAPI_IMPL(CStatus, name,(const arrtype* src1, int step1,  \
    const arrtype* src2, int step2, const uchar* mask,          \
    int maskstep, CSize size, double* _norm),                 \
    (src1, step1, src2, step2, mask, maskstep, size, _norm))   \
{                                                               \
    normtype norm = 0;                                          \
    step1 /= sizeof(src1[0]);                                   \
    step2 /= sizeof(src2[0]);                                   \
                                                                \
    for(; size.height--; src1 += step1, src2 += step2,         \
                          mask += maskstep)                    \
    {                                                           \
        int x = 0;                                              \
        ICC_NORM_DIFF_MASK_CASE(_op_, _update_op_,             \
                                 worktype, size.width);        \
    }                                                           \
                                                                \
    *_norm = post_func((double)norm);                           \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_DIFF_MASK_NOHINT_BLOCK_FUNC_2D_COI(name, \
    _op_, _update_op_, post_func, arrtype,                      \
    normtype, worktype, block_size)                            \
static CStatus C_STDCALL name(const arrtype* src1, int step1,\
    const arrtype* src2, int step2, const uchar* mask,          \
    int maskstep, CSize size, int cn, int coi, double* _norm) \
{                                                               \
    int remaining = block_size;                                 \
    normtype total_norm = 0;                                    \
    worktype norm = 0;                                          \
    step1 /= sizeof(src1[0]);                                   \
    step2 /= sizeof(src2[0]);                                   \
    src1 += coi - 1;                                            \
    src2 += coi - 1;                                            \
                                                                \
    for(; size.height--; src1 += step1, src2 += step2,         \
                          mask += maskstep)                    \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                 \
        {                                                       \
            int limit = MIN(remaining, size.width - x);       \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_NORM_DIFF_MASK_COI_CASE(_op_, _update_op_,     \
                                    worktype, limit, cn);      \
            if(remaining == 0)                                \
            {                                                   \
                remaining = block_size;                         \
                total_norm += (normtype)norm;                   \
                norm = 0;                                       \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    total_norm += (normtype)norm;                               \
    *_norm = post_func((double)total_norm);                     \
    return CC_OK;                                               \
}


#define  ICC_DEF_NORM_DIFF_MASK_NOHINT_FUNC_2D_COI(name, _op_, \
    _update_op_, post_func,                                     \
    arrtype, normtype, worktype, block_size)                   \
static CStatus C_STDCALL name(const arrtype* src1, int step1,\
    const arrtype* src2, int step2, const uchar* mask,          \
    int maskstep, CSize size, int cn, int coi, double* _norm) \
{                                                               \
    normtype norm = 0;                                          \
    step1 /= sizeof(src1[0]);                                   \
    step2 /= sizeof(src2[0]);                                   \
    src1 += coi - 1;                                            \
    src2 += coi - 1;                                            \
                                                                \
    for(; size.height--; src1 += step1, src2 += step2,         \
                          mask += maskstep)                    \
    {                                                           \
        int x = 0;                                              \
        ICC_NORM_DIFF_MASK_COI_CASE(_op_, _update_op_,         \
                                     worktype, size.width, cn);\
    }                                                           \
                                                                \
    *_norm = post_func((double)norm);                           \
    return CC_OK;                                               \
}


//////////////////////////////////// The macros expanded /////////////////////////////////


#define ICC_DEF_NORM_FUNC_ALL_C(flavor, _abs_, _abs_diff_, arrtype, worktype)\
                                                                            \
ICC_DEF_NORM_NOHINT_FUNC_2D(icvNorm_Inf_##flavor##_C1R,                    \
    _abs_, MAX, CC_NOP, arrtype, worktype, worktype, 0)                    \
                                                                            \
ICC_DEF_NORM_NOHINT_FUNC_2D_COI(icvNorm_Inf_##flavor##_CnCR,               \
    _abs_, MAX, CC_NOP, arrtype, worktype, worktype, 0)                    \
                                                                            \
ICC_DEF_NORM_DIFF_NOHINT_FUNC_2D(icvNormDiff_Inf_##flavor##_C1R,           \
    _abs_diff_, MAX, CC_NOP, arrtype, worktype, worktype, 0)               \
                                                                            \
ICC_DEF_NORM_DIFF_NOHINT_FUNC_2D_COI(icvNormDiff_Inf_##flavor##_CnCR,      \
    _abs_diff_, MAX, CC_NOP, arrtype, worktype, worktype, 0)               \
                                                                            \
ICC_DEF_NORM_MASK_NOHINT_FUNC_2D(icvNorm_Inf_##flavor##_C1MR,              \
    _abs_, MAX, CC_NOP, arrtype, worktype, worktype, 0)                    \
                                                                            \
ICC_DEF_NORM_MASK_NOHINT_FUNC_2D_COI(icvNorm_Inf_##flavor##_CnCMR,         \
    _abs_, MAX, CC_NOP, arrtype, worktype, worktype, 0)                    \
                                                                            \
ICC_DEF_NORM_DIFF_MASK_NOHINT_FUNC_2D(icvNormDiff_Inf_##flavor##_C1MR,     \
    _abs_diff_, MAX, CC_NOP, arrtype, worktype, worktype, 0)               \
                                                                            \
ICC_DEF_NORM_DIFF_MASK_NOHINT_FUNC_2D_COI(icvNormDiff_Inf_##flavor##_CnCMR,\
    _abs_diff_, MAX, CC_NOP, arrtype, worktype, worktype, 0)


ICC_DEF_NORM_FUNC_ALL_C(8u, CC_NOP, CC_IABS, uchar, int)
ICC_DEF_NORM_FUNC_ALL_C(16u, CC_NOP, CC_IABS, ushort, int)
ICC_DEF_NORM_FUNC_ALL_C(16s, CC_IABS, CC_IABS, short, int)
// there is no protection from overflow
// (otherwise we had to do everything in int64's or double's)
ICC_DEF_NORM_FUNC_ALL_C(32s, CC_IABS, CC_IABS, int, int)
ICC_DEF_NORM_FUNC_ALL_C(32f, fabs, fabs, float, double)
ICC_DEF_NORM_FUNC_ALL_C(64f, fabs, fabs, double, double)

#define ICC_DEF_NORM_FUNC_ALL_L1(flavor, _abs_, _abs_diff_, hintp_func, nohint_func,\
                                  arrtype, normtype, worktype, block_size)         \
                                                                                    \
ICC_DEF_NORM_##hintp_func##_FUNC_2D(icvNorm_L1_##flavor##_C1R,                     \
    _abs_, CC_ADD, CC_NOP, arrtype, normtype, worktype, block_size)                \
                                                                                    \
ICC_DEF_NORM_##nohint_func##_FUNC_2D_COI(icvNorm_L1_##flavor##_CnCR,               \
    _abs_, CC_ADD, CC_NOP, arrtype, normtype, worktype, block_size)                \
                                                                                    \
ICC_DEF_NORM_DIFF_##hintp_func##_FUNC_2D(icvNormDiff_L1_##flavor##_C1R,            \
    _abs_diff_, CC_ADD, CC_NOP, arrtype, normtype, worktype, block_size)           \
                                                                                    \
ICC_DEF_NORM_DIFF_##nohint_func##_FUNC_2D_COI(icvNormDiff_L1_##flavor##_CnCR,      \
    _abs_diff_, CC_ADD, CC_NOP, arrtype, normtype, worktype, block_size)           \
                                                                                    \
ICC_DEF_NORM_MASK_##nohint_func##_FUNC_2D(icvNorm_L1_##flavor##_C1MR,              \
    _abs_, CC_ADD, CC_NOP, arrtype, normtype, worktype, block_size)                \
                                                                                    \
ICC_DEF_NORM_MASK_##nohint_func##_FUNC_2D_COI(icvNorm_L1_##flavor##_CnCMR,         \
    _abs_, CC_ADD, CC_NOP, arrtype, normtype, worktype, block_size)                \
                                                                                    \
ICC_DEF_NORM_DIFF_MASK_##nohint_func##_FUNC_2D(icvNormDiff_L1_##flavor##_C1MR,     \
    _abs_diff_, CC_ADD, CC_NOP, arrtype, normtype, worktype, block_size)           \
                                                                                    \
ICC_DEF_NORM_DIFF_MASK_##nohint_func##_FUNC_2D_COI(icvNormDiff_L1_##flavor##_CnCMR,\
    _abs_diff_, CC_ADD, CC_NOP, arrtype, normtype, worktype, block_size)


ICC_DEF_NORM_FUNC_ALL_L1(8u, CC_NOP, CC_IABS, NOHINT_BLOCK, NOHINT_BLOCK,
                          uchar, int64, int, 1 << 23)
ICC_DEF_NORM_FUNC_ALL_L1(16u, CC_NOP, CC_IABS, NOHINT_BLOCK, NOHINT_BLOCK,
                          ushort, int64, int, 1 << 15)
ICC_DEF_NORM_FUNC_ALL_L1(16s, CC_IABS, CC_IABS, NOHINT_BLOCK, NOHINT_BLOCK,
                          short, int64, int, 1 << 15)
// there is no protection from overflow on abs() stage.
// (otherwise we had to do everything in int64's or double's)
ICC_DEF_NORM_FUNC_ALL_L1(32s, fabs, fabs, NOHINT, NOHINT,
                          int, double, double, INT_MAX)
ICC_DEF_NORM_FUNC_ALL_L1(32f, fabs, fabs, HINT, NOHINT,
                          float, double, double, INT_MAX)
ICC_DEF_NORM_FUNC_ALL_L1(64f, fabs, fabs, NOHINT, NOHINT,
                          double, double, double, INT_MAX)


#define ICC_DEF_NORM_FUNC_ALL_L2(flavor, hintp_func, nohint_func, arrtype,         \
                                  normtype, worktype, block_size, sqr_macro)       \
                                                                                    \
ICC_DEF_NORM_##hintp_func##_FUNC_2D(icvNorm_L2_##flavor##_C1R,                     \
    sqr_macro, CC_ADD, sqrt, arrtype, normtype, worktype, block_size)              \
                                                                                    \
ICC_DEF_NORM_##nohint_func##_FUNC_2D_COI(icvNorm_L2_##flavor##_CnCR,               \
    sqr_macro, CC_ADD, sqrt, arrtype, normtype, worktype, block_size)              \
                                                                                    \
ICC_DEF_NORM_DIFF_##hintp_func##_FUNC_2D(icvNormDiff_L2_##flavor##_C1R,            \
    sqr_macro, CC_ADD, sqrt, arrtype, normtype, worktype, block_size)              \
                                                                                    \
ICC_DEF_NORM_DIFF_##nohint_func##_FUNC_2D_COI(icvNormDiff_L2_##flavor##_CnCR,      \
    sqr_macro, CC_ADD, sqrt, arrtype, normtype, worktype, block_size)              \
                                                                                    \
ICC_DEF_NORM_MASK_##nohint_func##_FUNC_2D(icvNorm_L2_##flavor##_C1MR,              \
    sqr_macro, CC_ADD, sqrt, arrtype, normtype, worktype, block_size)              \
                                                                                    \
ICC_DEF_NORM_MASK_##nohint_func##_FUNC_2D_COI(icvNorm_L2_##flavor##_CnCMR,         \
    sqr_macro, CC_ADD, sqrt, arrtype, normtype, worktype, block_size)              \
                                                                                    \
ICC_DEF_NORM_DIFF_MASK_##nohint_func##_FUNC_2D(icvNormDiff_L2_##flavor##_C1MR,     \
    sqr_macro, CC_ADD, sqrt, arrtype, normtype, worktype, block_size)              \
                                                                                    \
ICC_DEF_NORM_DIFF_MASK_##nohint_func##_FUNC_2D_COI(icvNormDiff_L2_##flavor##_CnCMR,\
    sqr_macro, CC_ADD, sqrt, arrtype, normtype, worktype, block_size)


ICC_DEF_NORM_FUNC_ALL_L2(8u, NOHINT_BLOCK, NOHINT_BLOCK, uchar,
                          int64, int, 1 << 15, CC_SQR_8U)
ICC_DEF_NORM_FUNC_ALL_L2(16u, NOHINT, NOHINT, ushort,
                          double, double, INT_MAX, CC_SQR)
ICC_DEF_NORM_FUNC_ALL_L2(16s, NOHINT, NOHINT, short,
                          double, double, INT_MAX, CC_SQR)
// there is no protection from overflow on abs() stage.
// (otherwise we had to do everything in int64's or double's)
ICC_DEF_NORM_FUNC_ALL_L2(32s, NOHINT, NOHINT, int,
                          double, double, INT_MAX, CC_SQR)
ICC_DEF_NORM_FUNC_ALL_L2(32f, HINT, NOHINT, float,
                          double, double, INT_MAX, CC_SQR)
ICC_DEF_NORM_FUNC_ALL_L2(64f, NOHINT, NOHINT, double,
                          double, double, INT_MAX, CC_SQR)


#define ICC_DEF_INIT_NORM_TAB_2D(FUNCNAME, FLAG)              \
static void icvInit##FUNCNAME##FLAG##Table(CvFuncTable  tab)  \
{                                                               \
    tab[CC_8U] = (void*)icv##FUNCNAME##_8u_##FLAG;       \
    tab[CC_8S] = 0;                                      \
    tab[CC_16U] = (void*)icv##FUNCNAME##_16u_##FLAG;     \
    tab[CC_16S] = (void*)icv##FUNCNAME##_16s_##FLAG;     \
    tab[CC_32S] = (void*)icv##FUNCNAME##_32s_##FLAG;     \
    tab[CC_32F] = (void*)icv##FUNCNAME##_32f_##FLAG;     \
    tab[CC_64F] = (void*)icv##FUNCNAME##_64f_##FLAG;     \
}

ICC_DEF_INIT_NORM_TAB_2D(Norm_Inf, C1R)
ICC_DEF_INIT_NORM_TAB_2D(Norm_L1, C1R)
ICC_DEF_INIT_NORM_TAB_2D(Norm_L2, C1R)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_Inf, C1R)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_L1, C1R)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_L2, C1R)

ICC_DEF_INIT_NORM_TAB_2D(Norm_Inf, CnCR)
ICC_DEF_INIT_NORM_TAB_2D(Norm_L1, CnCR)
ICC_DEF_INIT_NORM_TAB_2D(Norm_L2, CnCR)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_Inf, CnCR)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_L1, CnCR)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_L2, CnCR)

ICC_DEF_INIT_NORM_TAB_2D(Norm_Inf, C1MR)
ICC_DEF_INIT_NORM_TAB_2D(Norm_L1, C1MR)
ICC_DEF_INIT_NORM_TAB_2D(Norm_L2, C1MR)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_Inf, C1MR)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_L1, C1MR)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_L2, C1MR)

ICC_DEF_INIT_NORM_TAB_2D(Norm_Inf, CnCMR)
ICC_DEF_INIT_NORM_TAB_2D(Norm_L1, CnCMR)
ICC_DEF_INIT_NORM_TAB_2D(Norm_L2, CnCMR)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_Inf, CnCMR)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_L1, CnCMR)
ICC_DEF_INIT_NORM_TAB_2D(NormDiff_L2, CnCMR)


static void icvInitNormTabs(CvFuncTable* norm_tab, CvFuncTable* normmask_tab)
{
    icvInitNorm_InfC1RTable(norm_tab[0]);
    icvInitNorm_L1C1RTable(norm_tab[1]);
    icvInitNorm_L2C1RTable(norm_tab[2]);
    icvInitNormDiff_InfC1RTable(norm_tab[3]);
    icvInitNormDiff_L1C1RTable(norm_tab[4]);
    icvInitNormDiff_L2C1RTable(norm_tab[5]);

    icvInitNorm_InfCnCRTable(norm_tab[6]);
    icvInitNorm_L1CnCRTable(norm_tab[7]);
    icvInitNorm_L2CnCRTable(norm_tab[8]);
    icvInitNormDiff_InfCnCRTable(norm_tab[9]);
    icvInitNormDiff_L1CnCRTable(norm_tab[10]);
    icvInitNormDiff_L2CnCRTable(norm_tab[11]);

    icvInitNorm_InfC1MRTable(normmask_tab[0]);
    icvInitNorm_L1C1MRTable(normmask_tab[1]);
    icvInitNorm_L2C1MRTable(normmask_tab[2]);
    icvInitNormDiff_InfC1MRTable(normmask_tab[3]);
    icvInitNormDiff_L1C1MRTable(normmask_tab[4]);
    icvInitNormDiff_L2C1MRTable(normmask_tab[5]);

    icvInitNorm_InfCnCMRTable(normmask_tab[6]);
    icvInitNorm_L1CnCMRTable(normmask_tab[7]);
    icvInitNorm_L2CnCMRTable(normmask_tab[8]);
    icvInitNormDiff_InfCnCMRTable(normmask_tab[9]);
    icvInitNormDiff_L1CnCMRTable(normmask_tab[10]);
    icvInitNormDiff_L2CnCMRTable(normmask_tab[11]);
}


CC_IMPL  double
cvNorm(const img_t* mat2, const img_t* mat1 CC_DEFAULT(NULL),
       int normType CC_DEFAULT(CC_L2), const img_t* mask CC_DEFAULT(NULL))
{
    static CvFuncTable norm_tab[12];
    static CvFuncTable normmask_tab[12];
    static int inittab = 0;

    double  norm = 0, norm_diff = 0;

    CC_FUNCNAME("cvNorm");

    __BEGIN__;

    TypeId type;
    int cn, is_relative;
    CSize size;
    int mat2_flag = CC_MAT_CONT_FLAG;
    int mat1_step, mat2_step, mask_step = 0;
    int coi = 0, coi2 = 0;

    if(!mat1)
    {
        mat1 = mat2;
        mat2 = 0;
    }

    is_relative = mat2 && (normType & CC_RELATIVE);
    normType &= ~CC_RELATIVE;

    switch(normType)
    {
    case CC_C:
    case CC_L1:
    case CC_L2:
    case CC_DIFF_C:
    case CC_DIFF_L1:
    case CC_DIFF_L2:
        normType = (normType & 7) >> 1;
        break;
    default:
        CC_ERROR(CC_StsBadFlag, "");
    }

    /* light variant */
    if(!mask)
    {
        if(mat2)
        {
            if(!CC_ARE_TYPES_EQ(mat1, mat2))
                CC_ERROR(CC_StsUnmatchedFormats, "");

            if(!CC_ARE_SIZES_EQ(mat1, mat2))
                CC_ERROR(CC_StsUnmatchedSizes, "");

            mat2_flag = CC_IS_MAT_CONT(mat2);
        }

        size = cvGetMatSize(mat1);
        type = CC_MAT_DEPTH(mat1);
        cn = CC_MAT_CN(mat1);

        if(CC_IS_MAT_CONT(mat1) && mat2_flag)
        {
            size.width *= size.height;

            if(size.width <= CC_MAX_INLINE_MAT_OP_SIZE && normType == 2 /* CC_L2 */)
            {
                if(type == CC_32F)
                {
                    const float* src1data = mat1->tt.fl;
                    int size0 = size.width *= cn;
                
                    if(!mat2 || is_relative)
                    {
                        do
                        {
                            double t = src1data[size.width-1];
                            norm += t*t;
                        }
                        while(--size.width);
                    }

                    if(mat2)
                    {
                        const float* src2data = mat2->tt.fl;
                        size.width = size0;

                        do
                        {
                            double t = src1data[size.width-1] - src2data[size.width-1];
                            norm_diff += t*t;
                        }
                        while(--size.width);

                        if(is_relative)
                            norm = norm_diff/(norm + DBL_EPSILON);
                        else
                            norm = norm_diff;
                    }
                    norm = sqrt(norm);
                    EXIT;
                }

                if(type == CC_64F)
                {
                    const double* src1data = mat1->tt.db;
                    int size0 = size.width *= cn;

                    if(!mat2 || is_relative)
                    {
                        do
                        {
                            double t = src1data[size.width-1];
                            norm += t*t;
                        }
                        while(--size.width);
                    }

                    if(mat2)
                    {
                        const double* src2data = mat2->tt.db;
                        size.width = size0;

                        do
                        {
                            double t = src1data[size.width-1] - src2data[size.width-1];
                            norm_diff += t*t;
                        }
                        while(--size.width);

                        if(is_relative)
                            norm = norm_diff/(norm + DBL_EPSILON);
                        else
                            norm = norm_diff;
                    }
                    norm = sqrt(norm);
                    EXIT;
                }
            }
            size.height = 1;
            mat1_step = mat2_step = CC_STUB_STEP;
        }
        else
        {
            mat1_step = mat1->step;
            mat2_step = mat2 ? mat2->step : 0;
        }
    } else {
        if(mat2)
        {
            if(!CC_ARE_TYPES_EQ(mat1, mat2))
                CC_ERROR(CC_StsUnmatchedFormats, "");

            if(!CC_ARE_SIZES_EQ(mat1, mat2))
                CC_ERROR(CC_StsUnmatchedSizes, "");

            if(coi != coi2 && CC_MAT_CN(mat1) > 1)
                CC_ERROR(CC_BadCOI, "");

            mat2_flag = CC_IS_MAT_CONT(mat2);
        }

        size = cvGetMatSize(mat1);
        type = CC_MAT_DEPTH(mat1);
        cn = CC_MAT_CN(mat1);
        mat1_step = mat1->step;
        mat2_step = mat2 ? mat2->step : 0;

        if(!mask && CC_IS_MAT_CONT(mat1) && mat2_flag)
        {
            size.width *= size.height;
            size.height = 1;
            mat1_step = mat2_step = CC_STUB_STEP;
        }
    }

    if(!inittab)
    {
        icvInitNormTabs(norm_tab, normmask_tab);
        inittab = 1;
    }

    if(!mask)
    {
        if(cn == 1 || coi == 0)
        {
            int pass_hint = type == CC_32F && normType != 0;
            size.width *= cn;

            if(!mat2 || is_relative)
            {
                if(!pass_hint)
                {
                    CvFunc2D_1A1P func;
                    CC_GET_FUNC_PTR(func, (CvFunc2D_1A1P)norm_tab[normType][type]);

                    IPPI_CALL(func(mat1->tt.data, mat1_step, size, &norm));
                }
                else
                {
                    CvFunc2D_1A1P1I func;
                    CC_GET_FUNC_PTR(func, (CvFunc2D_1A1P1I)norm_tab[normType][type]);

                    IPPI_CALL(func(mat1->tt.data, mat1_step, size, &norm, cvAlgHintAccurate));
                }
            }
        
            if(mat2)
            {
                if(!pass_hint)
                {
                    CvFunc2D_2A1P func;
                    CC_GET_FUNC_PTR(func, (CvFunc2D_2A1P)norm_tab[3 + normType][type]);

                    IPPI_CALL(func(mat1->tt.data, mat1_step, mat2->tt.data, mat2_step,
                                     size, &norm_diff));
                }
                else
                {
                    CvFunc2D_2A1P1I func;
                    CC_GET_FUNC_PTR(func, (CvFunc2D_2A1P1I)norm_tab[3 + normType][type]);

                    IPPI_CALL(func(mat1->tt.data, mat1_step, mat2->tt.data, mat2_step,
                                     size, &norm_diff, cvAlgHintAccurate));
                }

                if(is_relative)
                    norm = norm_diff/(norm + DBL_EPSILON);
                else
                    norm = norm_diff;
            }
        }
        else
        {
            if(!mat2 || is_relative)
            {
                CvFunc2DnC_1A1P func;
                CC_GET_FUNC_PTR(func, (CvFunc2DnC_1A1P)norm_tab[6 + normType][type]);

                IPPI_CALL(func(mat1->tt.data, mat1_step, size, cn, coi, &norm));
            }
        
            if(mat2)
            {
                CvFunc2DnC_2A1P func;
                CC_GET_FUNC_PTR(func, (CvFunc2DnC_2A1P)norm_tab[9 + normType][type]);

                IPPI_CALL(func(mat1->tt.data, mat1_step, mat2->tt.data, mat2_step,
                                 size, cn, coi, &norm_diff));

                if(is_relative)
                    norm = norm_diff/(norm + DBL_EPSILON);
                else
                    norm = norm_diff;
            }
        }
    }
    else
    {
        img_t maskstub;
        const img_t *matmask = mask;

        if(CC_MAT_CN(mat1) > 1 && coi == 0)
            CC_ERROR(CC_StsBadArg, "");

        CC_CALL(matmask = cvGetMat(matmask, &maskstub));

        if(!CC_IS_MASK_ARR(matmask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mat1, matmask))
            CC_ERROR(CC_StsUnmatchedSizes, "");
        
        mask_step = matmask->step;

        if(CC_IS_MAT_CONT(mat1) && mat2_flag && CC_IS_MAT_CONT(matmask))
        {
            size.width *= size.height;
            size.height = 1;
            mat1_step = mat2_step = mask_step = CC_STUB_STEP;
        }

        if(CC_MAT_CN(mat1) == 1 || coi == 0)
        {
            if(!mat2 || is_relative)
            {
                CvFunc2D_2A1P func;
                CC_GET_FUNC_PTR(func,
                    (CvFunc2D_2A1P)normmask_tab[normType][type]);

                IPPI_CALL(func(mat1->tt.data, mat1_step,
                                 matmask->tt.data, mask_step, size, &norm));
            }
        
            if(mat2)
            {
                CvFunc2D_3A1P func;
                CC_GET_FUNC_PTR(func,
                    (CvFunc2D_3A1P)normmask_tab[3 + normType][type]);

                IPPI_CALL(func(mat1->tt.data, mat1_step, mat2->tt.data, mat2_step,
                                 matmask->tt.data, mask_step, size, &norm_diff));

                if(is_relative)
                    norm = norm_diff/(norm + DBL_EPSILON);
                else
                    norm = norm_diff;
            }
        }
        else
        {
            if(!mat2 || is_relative)
            {
                CvFunc2DnC_2A1P func;
                CC_GET_FUNC_PTR(func,
                    (CvFunc2DnC_2A1P)normmask_tab[6 + normType][type]);

                IPPI_CALL(func(mat1->tt.data, mat1_step,
                                 matmask->tt.data, mask_step,
                                 size, cn, coi, &norm));
            }
        
            if(mat2)
            {
                CvFunc2DnC_3A1P func;
                CC_GET_FUNC_PTR(func,
                    (CvFunc2DnC_3A1P)normmask_tab[9 + normType][type]);

                IPPI_CALL(func(mat1->tt.data, mat1_step,
                                 mat2->tt.data, mat2_step,
                                 matmask->tt.data, mask_step,
                                 size, cn, coi, &norm_diff));

                if(is_relative)
                    norm = norm_diff/(norm + DBL_EPSILON);
                else
                    norm = norm_diff;
            }
        }
    }

    __END__;

    return norm;
}


