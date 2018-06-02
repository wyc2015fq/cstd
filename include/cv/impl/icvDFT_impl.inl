{
    void* buffer = 0;
    int local_alloc = 1;
    
    CC_FUNCNAME("cvDFT");

    __BEGIN__;

    int prev_len = 0, buf_size = 0, stage = 0;
    int nf = 0, inv = (flags & CC_DXT_INVERSE) != 0;
    int real_transform = 0;
    int complex_elem_size;
    int factors[34], inplace_transform = 0;
    int ipp_norm_flag = 0;
    int src_rows = size.h;
    int src_cols = size.w;
    int dst_rows = size.h;
    int dst_cols = size.w;
    uchar* src_data_ptr = (uchar*)src;
    uchar* dst_data_ptr = (uchar*)dst;

    complex_elem_size = elem_size*2;

    if(srccn == dstcn)
    {
        if(srccn > 2)
            CC_ERROR(CC_StsUnsupportedFormat,
            "Only 32fC1, 32fC2, 64fC1 and 64fC2 formats are supported");

        real_transform = srccn == 1;
        if(!real_transform)
            elem_size = complex_elem_size;
    }
    else if(!inv && srccn == 1 && dstcn == 2)
    {
        if((src_cols != 1 || dst_cols != 1 ||
            src_rows/2+1 != dst_rows && src_rows != dst_rows) &&
            (src_cols/2+1 != dst_cols || src_rows != dst_rows))
            CC_ERROR(CC_StsUnmatchedSizes, "");
        real_transform = 1;
    }
    else if(inv && srccn == 2 && dstcn == 1)
    {
        if((src_cols != 1 || dst_cols != 1 ||
            dst_rows/2+1 != src_rows && src_rows != dst_rows) &&
            (dst_cols/2+1 != src_cols || src_rows != dst_rows))
            CC_ERROR(CC_StsUnmatchedSizes, "");
        real_transform = 1;
    }
    else
        CC_ERROR(CC_StsUnmatchedFormats,
        "Incorrect or unsupported combination of input & output formats");

    if(src_cols == 1 && nonzero_rows > 0)
        CC_ERROR(CC_StsNotImplemented,
        "This mode (using nonzero_rows with a single-column matrix) breaks the function logic, so it is prohibited.\n"
        "For fast convolution/correlation use 2-column matrix or single-row matrix instead");

    // determine, which transform to do first - row-wise
    // (stage 0) or column-wise (stage 1) transform
    if(!(flags & CC_DXT_ROWS) && src_rows > 1 &&
        (src_cols == 1 && !(src_cols*elem_size*srccn==srcstep && dst_cols*elem_size*dstcn==dststep) ||
        src_cols > 1 && inv && real_transform))
        stage = 1;

    ipp_norm_flag = !(flags & CC_DXT_SCALE) ? 8 : (flags & CC_DXT_INVERSE) ? 2 : 1;

    for(;;)
    {
        double scale = 1;
        uchar* wave = 0;
        int* itab = 0;
        uchar* ptr;
        int i, len, count, sz = 0;
        int use_buf = 0, odd_real = 0;
        CvDFTFunc dft_func;

        if(stage == 0) // row-wise transform
        {
            len = !inv ? src_cols : dst_cols;
            count = src_rows;
            if(len == 1 && !(flags & CC_DXT_ROWS))
            {
                len = !inv ? src_rows : dst_rows;
                count = 1;
            }
            odd_real = real_transform && (len & 1);
        }
        else
        {
            len = dst_rows;
            count = !inv ? src_cols : dst_cols;
            sz = 2*len*complex_elem_size;
        }

        {
            if(len != prev_len)
                nf = icvDFTFactorize(len, factors);

            inplace_transform = factors[0] == factors[nf-1];
            sz += len*(complex_elem_size + sizeof(int));
            i = nf > 1 && (factors[0] & 1) == 0;
            if((factors[i] & 1) != 0 && factors[i] > 5)
                sz += (factors[i]+1)*complex_elem_size;

            if(stage == 0 && (src_data_ptr == dst_data_ptr && !inplace_transform || odd_real) ||
                stage == 1 && !inplace_transform)
            {
                use_buf = 1;
                sz += len*complex_elem_size;
            }
        }

        if(sz > buf_size)
        {
            prev_len = 0; // because we release the buffer, 
                          // force recalculation of
                          // twiddle factors and permutation table
            if(!local_alloc && buffer)
                cFree(&buffer);
            if(sz <= CC_MAX_LOCAL_DFT_SIZE)
            {
                buf_size = sz = CC_MAX_LOCAL_DFT_SIZE;
                buffer = cvStackAlloc(sz + 32);
                local_alloc = 1;
            }
            else
            {
                CC_CALL(buffer = cAlloc(sz + 32));
                buf_size = sz;
                local_alloc = 0;
            }
        }

        ptr = (uchar*)buffer;
        {
            wave = ptr;
            ptr += len*complex_elem_size;
            itab = (int*)ptr;
            ptr = (uchar*)cvAlignPtr(ptr + len*sizeof(int), 16);

            if(len != prev_len || (!inplace_transform && inv && real_transform))
                icvDFTInit(len, nf, factors, itab, complex_elem_size,
                            wave, stage == 0 && inv && real_transform);
            // otherwise reuse the tables calculated on the previous stage
        }

        if(stage == 0)
        {
            uchar* tmp_buf = 0;
            int dptr_offset = 0;
            int dst_full_len = len*elem_size;
            int _flags = inv + (srccn != dstcn ?
                         ICC_DFT_COMPLEX_INPUT_OR_OUTPUT : 0);
            if(use_buf)
            {
                tmp_buf = ptr;
                ptr += len*complex_elem_size;
                if(odd_real && !inv && len > 1 &&
                    !(_flags & ICC_DFT_COMPLEX_INPUT_OR_OUTPUT))
                    dptr_offset = elem_size;
            }

            if(!inv && (_flags & ICC_DFT_COMPLEX_INPUT_OR_OUTPUT))
                dst_full_len += (len & 1) ? elem_size : complex_elem_size;

            dft_func = dft_tbl[(!real_transform ? 0 : !inv ? 1 : 2) ];

            if(count > 1 && !(flags & CC_DXT_ROWS) && (!inv || !real_transform))
                stage = 1;
            else if(flags & CC_DXT_SCALE)
                scale = 1./(len * (flags & CC_DXT_ROWS ? 1 : count));

            if(nonzero_rows <= 0 || nonzero_rows > count)
                nonzero_rows = count;

            for(i = 0; i < nonzero_rows; i++)
            {
                const uchar* sptr = src_data_ptr + i*srcstep;
                uchar* dptr0 = dst_data_ptr + i*dststep;
                uchar* dptr = dptr0;

                if(tmp_buf)
                    dptr = tmp_buf;
                
                dft_func(sptr, dptr, len, nf, factors, itab, wave, len, ptr, _flags, scale);
                if(dptr != dptr0)
                    memcpy(dptr0, dptr + dptr_offset, dst_full_len);
            }

            for(; i < count; i++)
            {
                uchar* dptr0 = dst_data_ptr + i*dststep;
                memset(dptr0, 0, dst_full_len);
            }

            if(stage != 1)
                break;
            src = dst;
        }
        else
        {
            int a = 0, b = count;
            uchar *buf0, *buf1, *dbuf0, *dbuf1;
            uchar* sptr0 = src_data_ptr;
            uchar* dptr0 = dst_data_ptr;
            buf0 = ptr;
            ptr += len*complex_elem_size;
            buf1 = ptr;
            ptr += len*complex_elem_size;
            dbuf0 = buf0, dbuf1 = buf1;
            
            if(use_buf)
            {
                dbuf1 = ptr;
                dbuf0 = buf1;
                ptr += len*complex_elem_size;
            }

            dft_func = dft_tbl[0];

            if(real_transform && inv && src_cols > 1)
                stage = 0;
            else if(flags & CC_DXT_SCALE)
                scale = 1./(len * count);

            if(real_transform)
            {
                int even;
                a = 1;
                even = (count & 1) == 0;
                b = (count+1)/2;
                if(!inv)
                {
                    memset(buf0, 0, len*complex_elem_size);
                    icvCopyColumn(sptr0, srcstep, buf0, complex_elem_size, len, elem_size);
                    sptr0 += dstcn*elem_size;
                    if(even)
                    {
                        memset(buf1, 0, len*complex_elem_size);
                        icvCopyColumn(sptr0 + (count-2)*elem_size, srcstep,
                                       buf1, complex_elem_size, len, elem_size);
                    }
                }
                else if(srccn == 1)
                {
                    icvCopyColumn(sptr0, srcstep, buf0 + elem_size, elem_size, len, elem_size);
                    icvExpandCCS(buf0 + elem_size, len, elem_size);
                    if(even)
                    {
                        icvCopyColumn(sptr0 + (count-1)*elem_size, srcstep,
                                       buf1 + elem_size, elem_size, len, elem_size);
                        icvExpandCCS(buf1 + elem_size, len, elem_size);
                    }
                    sptr0 += elem_size;
                }
                else
                {
                    icvCopyColumn(sptr0, srcstep, buf0, complex_elem_size, len, complex_elem_size);
                    //memcpy(buf0 + elem_size, buf0, elem_size);
                    //icvExpandCCS(buf0 + elem_size, len, elem_size);
                    if(even)
                    {
                        icvCopyColumn(sptr0 + b*complex_elem_size, srcstep,
                                       buf1, complex_elem_size, len, complex_elem_size);
                        //memcpy(buf0 + elem_size, buf0, elem_size);
                        //icvExpandCCS(buf0 + elem_size, len, elem_size);
                    }
                    sptr0 += complex_elem_size;
                }
                
                if(even)
                    dft_func(buf1, dbuf1, len, nf, factors, itab, wave, len, ptr, inv, scale);

                dft_func(buf0, dbuf0, len, nf, factors, itab, wave, len, ptr, inv, scale);

                if(dstcn == 1)
                {
                    if(!inv)
                    {
                        // copy the half of output vector to the first/last column.
                        // before doing that, defgragment the vector
                        memcpy(dbuf0 + elem_size, dbuf0, elem_size);
                        icvCopyColumn(dbuf0 + elem_size, elem_size, dptr0,
                                       dststep, len, elem_size);
                        if(even)
                        {
                            memcpy(dbuf1 + elem_size, dbuf1, elem_size);
                            icvCopyColumn(dbuf1 + elem_size, elem_size,
                                           dptr0 + (count-1)*elem_size,
                                           dststep, len, elem_size);
                        }
                        dptr0 += elem_size;
                    }
                    else
                    {
                        // copy the real part of the complex vector to the first/last column
                        icvCopyColumn(dbuf0, complex_elem_size, dptr0, dststep, len, elem_size);
                        if(even)
                            icvCopyColumn(dbuf1, complex_elem_size, dptr0 + (count-1)*elem_size,
                                           dststep, len, elem_size);
                        dptr0 += elem_size;
                    }
                }
                else
                {
                    assert(!inv);
                    icvCopyColumn(dbuf0, complex_elem_size, dptr0,
                                   dststep, len, complex_elem_size);
                    if(even)
                        icvCopyColumn(dbuf1, complex_elem_size,
                                       dptr0 + b*complex_elem_size,
                                       dststep, len, complex_elem_size);
                    dptr0 += complex_elem_size;
                }
            }

            for(i = a; i < b; i += 2)
            {
                if(i+1 < b)
                {
                    icvCopyFrom2Columns(sptr0, srcstep, buf0, buf1, len, complex_elem_size);
                    dft_func(buf1, dbuf1, len, nf, factors, itab, wave, len, ptr, inv, scale);
                }
                else {
                    icvCopyColumn(sptr0, srcstep, buf0, complex_elem_size, len, complex_elem_size);
                }

                dft_func(buf0, dbuf0, len, nf, factors, itab, wave, len, ptr, inv, scale);

                if(i+1 < b)
                    icvCopyTo2Columns(dbuf0, dbuf1, dptr0, dststep, len, complex_elem_size);
                else
                    icvCopyColumn(dbuf0, complex_elem_size, dptr0, dststep, len, complex_elem_size);
                sptr0 += 2*complex_elem_size;
                dptr0 += 2*complex_elem_size;
            }

            if(stage != 0)
                break;
            src = dst;
        }
    }

    __END__;

    if(buffer && !local_alloc)
        cFree(&buffer);
}