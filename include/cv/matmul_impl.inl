

static CStatus C_STDCALL
CCFUNC(icvGEMMSingleMul)(const arrtype* a_data, size_t a_step,
                         const arrtype* b_data, size_t b_step,
                         const arrtype* c_data, size_t c_step,
                         arrtype* d_data, size_t d_step,
                         CSize a_size, CSize d_size,
                         double alpha, double beta, int flags)
{
    int i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    const arrtype *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    arrtype* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= sizeof(a_data[0]);
    b_step /= sizeof(b_data[0]);
    c_step /= sizeof(c_data[0]);
    d_step /= sizeof(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    if(!c_data)
        c_step0 = c_step1 = 0;
    else if(!(flags & CC_GEMM_C_T))
        c_step0 = c_step, c_step1 = 1;
    else
        c_step0 = 1, c_step1 = c_step;

    if(flags & CC_GEMM_A_T)
    {
        CC_SWAP(a_step0, a_step1, t_step);
        n = a_size.height;
        if(a_step > 1 && n > 1)
            a_buf = (arrtype*)cvStackAlloc(n*sizeof(a_data[0]));
    }

    if(n == 1) /* external product */
    {
        arrtype* b_buf = 0;

        if(a_step > 1)
        {
            a_buf = (arrtype*)cvStackAlloc(drows*sizeof(a_data[0]));
            for(k = 0; k < drows; k++)
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        if(b_step > 1)
        {
            b_buf = (arrtype*)cvStackAlloc(d_size.width*sizeof(b_buf[0]));
            for(j = 0; j < d_size.width; j++)
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        for(i = 0; i < drows; i++, _c_data += c_step0,
                                    d_data += d_step)
        {
            worktype al = muls(to_worktype(a_data[i]), alpha);
            c_data = _c_data;
            for(j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1)
            {
                worktype s0 = mul(al, to_worktype(b_data[j]));
                worktype s1 = mul(al, to_worktype(b_data[j+1]));
                if(c_data)
                {
                    mulsadd2(s0, to_worktype(c_data[0]), beta);
                    mulsadd2(s1, to_worktype(c_data[c_step1]), beta);
                }
                d_data[j] = to_arrtype(s0);
                d_data[j+1] = to_arrtype(s1);
            }

            for(; j < d_size.width; j++, c_data += c_step1)
            {
                worktype s0 = mul(al, to_worktype(b_data[j]));
                if(c_data)
                {
                    mulsadd2(s0, to_worktype(c_data[0]), beta);
                }
                d_data[j] = to_arrtype(s0);
            }
        }
    }
    else if(flags & CC_GEMM_B_T) /* A * Bt */
    {
        for(i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step)
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            if(a_buf)
            {
                for(k = 0; k < n; k++)
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for(j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1)
            {
                worktype s0=worktype0, s1=worktype0, s2=worktype0, s3=worktype0;

                for(k = 0; k <= n - 4; k += 4)
                {
                    muladd2(s0, to_worktype(a_data[k]), to_worktype(b_data[k]));
                    muladd2(s1, to_worktype(a_data[k+1]), to_worktype(b_data[k+1]));
                    muladd2(s2, to_worktype(a_data[k+2]), to_worktype(b_data[k+2]));
                    muladd2(s3, to_worktype(a_data[k+3]), to_worktype(b_data[k+3]));
                }

                for(; k < n; k++) {
                    muladd2(s0, to_worktype(a_data[k]), to_worktype(b_data[k]));
                }
                mulsadd2(s0, add(add(s0, s1), add(s2, s3)), alpha);

                if(c_data) {
                    mulsadd2(s0, to_worktype(c_data[0]), beta);
                }
                d_data[j] = to_arrtype(s0);
            }
        }
    }
    else if(d_size.width*sizeof(d_data[0]) <= 1600)
    {
        for(i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step)
        {
            a_data = _a_data, c_data = _c_data;

            if(a_buf)
            {
                for(k = 0; k < n; k++)
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for(j = 0; j <= m - 4; j += 4, c_data += 4*c_step1)
            {
                const arrtype* b = _b_data + j;
                worktype s0=worktype0, s1=worktype0, s2=worktype0, s3=worktype0;

                for(k = 0; k < n; k++, b += b_step)
                {
                    worktype a=to_worktype(a_data[k]);
                    muladd2(s0, a, to_worktype(b[0]));
                    muladd2(s1, a, to_worktype(b[1]));
                    muladd2(s2, a, to_worktype(b[2]));
                    muladd2(s3, a, to_worktype(b[3]));
                }

                s0 = muls(s0, alpha);
                s1 = muls(s1, alpha);
                s2 = muls(s2, alpha);
                s3 = muls(s3, alpha);

                if(c_data)
                {
                    mulsadd2(s0, to_worktype(c_data[0]), beta);
                    mulsadd2(s1, to_worktype(c_data[c_step1]), beta);
                    mulsadd2(s2, to_worktype(c_data[c_step1*2]), beta);
                    mulsadd2(s3, to_worktype(c_data[c_step1*3]), beta);
                }
                d_data[j] = to_arrtype(s0);
                d_data[j+1] = to_arrtype(s1);
                d_data[j+2] = to_arrtype(s2);
                d_data[j+3] = to_arrtype(s3);
            }

            for(; j < m; j++, c_data += c_step1)
            {
                const arrtype* b = _b_data + j;
                worktype s0=worktype0;

                for(k = 0; k < n; k++, b += b_step) {
                    muladd2(s0, to_worktype(a_data[k]), to_worktype(b[0]));
                }

                s0 = muls(s0, alpha);
                if(c_data) {
                    mulsadd2(s0, to_worktype(c_data[0]), beta);
                }
                d_data[j] = to_arrtype(s0);
            }
        }
    }
    else
    {
        worktype* d_buf = (worktype*)cvStackAlloc(m*sizeof(d_buf[0]));

        for(i = 0; i < drows; i++, _a_data += a_step0,
                                            _c_data += c_step0,
                                            d_data += d_step)
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            if(a_buf)
            {
                for(k = 0; k < n; k++)
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            for(j = 0; j < m; j++)
                d_buf[j] = worktype0;

            for(k = 0; k < n; k++, b_data += b_step)
            {
                worktype al=to_worktype(a_data[k]);

                for(j = 0; j <= m - 4; j += 4)
                {
                    muladd2(d_buf[j], to_worktype(b_data[j]), al);
                    muladd2(d_buf[j+1], to_worktype(b_data[j+1]), al);
                    muladd2(d_buf[j+2], to_worktype(b_data[j+2]), al);
                    muladd2(d_buf[j+3], to_worktype(b_data[j+3]), al);
                }

                for(; j < m; j++) {
                    muladd2(d_buf[j], to_worktype(b_data[j]), al);
                }
            }

            if(!c_data)
                for(j = 0; j < m; j++)
                    d_data[j] = to_arrtype(muls(d_buf[j], alpha));
            else
                for(j = 0; j < m; j++, c_data += c_step1)
                {
                    worktype t = muls(d_buf[j], alpha);
                    mulsadd2(t, to_worktype(c_data[0]), beta);
                    d_data[j] = to_arrtype(t);
                }
        }
    }
    return CC_OK;
}



static CStatus C_STDCALL
CCFUNC(icvGEMMBlockMul)(const arrtype* a_data, size_t a_step,
                        const arrtype* b_data, size_t b_step,
                        worktype* d_data, size_t d_step,
                        CSize a_size, CSize d_size, int flags)
{
    int i, j, k, n = a_size.width, m = d_size.width;
    const arrtype *_a_data = a_data, *_b_data = b_data;
    arrtype* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    int do_acc = flags & 16;

    a_step /= sizeof(a_data[0]);
    b_step /= sizeof(b_data[0]);
    d_step /= sizeof(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    if(flags & CC_GEMM_A_T)
    {
        CC_SWAP(a_step0, a_step1, t_step);
        n = a_size.height;
        a_buf = (arrtype*)cvStackAlloc(n*sizeof(a_data[0]));
    }

    if(flags & CC_GEMM_B_T)
    {
        /* second operand is transposed */
        for(i = 0; i < d_size.height; i++, _a_data += a_step0,
                                            d_data += d_step)
        {
            a_data = _a_data; b_data = _b_data;

            if(a_buf)
            {
                for(k = 0; k < n; k++)
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for(j = 0; j < d_size.width; j++, b_data += b_step)
            {
                worktype s0 = do_acc ? d_data[j]: worktype0, s1=worktype0;
                for(k = 0; k <= n - 2; k += 2)
                {
                    muladd2(s0, to_worktype(a_data[k]), to_worktype(b_data[k]));
                    muladd2(s1, to_worktype(a_data[k+1]), to_worktype(b_data[k+1]));
                }

                for(; k < n; k++)
                    muladd2(s0, to_worktype(a_data[k]), to_worktype(b_data[k]));

                d_data[j] = add(s0, s1);
            }
        }
    }
    else
    {
        for(i = 0; i < d_size.height; i++, _a_data += a_step0,
                                            d_data += d_step)
        {
            a_data = _a_data, b_data = _b_data;

            if(a_buf)
            {
                for(k = 0; k < n; k++)
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for(j = 0; j <= m - 4; j += 4)
            {
                worktype s0, s1, s2, s3;
                const arrtype* b = b_data + j;

                if(do_acc)
                {
                    s0 = d_data[j]; s1 = d_data[j+1];
                    s2 = d_data[j+2]; s3 = d_data[j+3];
                }
                else
                    s0 = s1 = s2 = s3 = worktype0;

                for(k = 0; k < n; k++, b += b_step)
                {
                    worktype a=to_worktype(a_data[k]);
                    muladd2(s0, a, b[0]);
                    muladd2(s1, a, b[1]);
                    muladd2(s2, a, b[2]);
                    muladd2(s3, a, b[3]);
                }

                d_data[j] = s0; d_data[j+1] = s1;
                d_data[j+2] = s2; d_data[j+3] = s3;
            }

            for(; j < m; j++)
            {
                const arrtype* b = b_data + j;
                worktype s0 = do_acc ? d_data[j] : worktype0;

                for(k = 0; k < n; k++, b += b_step)
                    muladd2(s0, to_worktype(a_data[k]), b[0]);

                d_data[j] = s0;
            }
        }
    }

    return CC_OK;
}


static CStatus C_STDCALL
CCFUNC(icvGEMMStore)(const arrtype* c_data, size_t c_step,
                       const worktype* d_buf, size_t d_buf_step,
                       arrtype* d_data, size_t d_step, CSize d_size,
                       double alpha, double beta, int flags)
{
    const arrtype* _c_data = c_data;
    int j;
    size_t c_step0, c_step1;

    c_step /= sizeof(c_data[0]);
    d_buf_step /= sizeof(d_buf[0]);
    d_step /= sizeof(d_data[0]);

    if(!c_data)
        c_step0 = c_step1 = 0;
    else if(!(flags & CC_GEMM_C_T))
        c_step0 = c_step, c_step1 = 1;
    else
        c_step0 = 1, c_step1 = c_step;

    for(; d_size.height--; _c_data += c_step0,
                            d_buf += d_buf_step,
                            d_data += d_step)
    {
        if(_c_data)
        {
            c_data = _c_data;
            for(j = 0; j <= d_size.width - 4; j += 4, c_data += 4*c_step1)
            {
                worktype t0, t1;
                t0 = muls(d_buf[j], alpha);
                t1 = muls(d_buf[j+1], alpha);
                mulsadd2(t0, to_worktype(c_data[0]), beta);
                mulsadd2(t1, to_worktype(c_data[c_step1]), beta);
                d_data[j] = to_arrtype(t0);
                d_data[j+1] = to_arrtype(t1);
                t0 = muls(d_buf[j+2], alpha);
                t1 = muls(d_buf[j+3], alpha);
                mulsadd2(t0, to_worktype(c_data[c_step1*2]), beta);
                mulsadd2(t1, to_worktype(c_data[c_step1*3]), beta);
                d_data[j+2] = to_arrtype(t0);
                d_data[j+3] = to_arrtype(t1);
            }
            for(; j < d_size.width; j++, c_data += c_step1)
            {
                worktype t0 = muls(d_buf[j], alpha);
                mulsadd2(t0, c_data[0], beta);
                d_data[j] = to_arrtype(t0);
            }
        }
        else
        {
            for(j = 0; j <= d_size.width - 4; j += 4)
            {
                worktype t0 = muls(d_buf[j], alpha);
                worktype t1 = muls(d_buf[j+1], alpha);
                d_data[j] = to_arrtype(t0);
                d_data[j+1] = to_arrtype(t1);
                t0 = muls(d_buf[j+2], alpha);
                t1 = muls(d_buf[j+3], alpha);
                d_data[j+2] = to_arrtype(t0);
                d_data[j+3] = to_arrtype(t1);
            }
            for(; j < d_size.width; j++)
                d_data[j] = to_arrtype(muls(d_buf[j], alpha));
        }
    }
    return CC_OK;
}

#undef CCFUNC
#undef arrtype
#undef worktype
#undef worktype0
#undef to_worktype
#undef to_arrtype
#undef muladd2
#undef mulsadd2
#undef add
#undef mul
#undef muls
