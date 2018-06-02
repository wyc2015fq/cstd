
CC_IMPL void
cvCanny(const img_t* src, img_t* dst,
         double low_thresh, double high_thresh, int aperture_size)
{
    static const int sec_tab[] = { 1, 3, 0, 0, 2, 2, 2, 2 };
    img_t dx[1] = {0};
    img_t dy[1] = {0};
    void *buffer = 0;
    uchar **stack_top, **stack_bottom = 0;

    CC_FUNCNAME("cvCanny");

    __BEGIN__;    
    CSize size;
    int flags = aperture_size;
    int low, high;
    int* mag_buf[3];
    uchar* map;
    int mapstep, maxsize;
    int i, j;
    img_t mag_row[1];

    if(CC_MAT_TYPE(src) != CC_8U || CC_MAT_CN(src)!=1 ||
        CC_MAT_TYPE(dst) != CC_8U || CC_MAT_CN(dst)!=1)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(low_thresh > high_thresh)
    {
        double t;
        CC_SWAP(low_thresh, high_thresh, t);
    }

    aperture_size &= INT_MAX;
    if((aperture_size & 1) == 0 || aperture_size < 3 || aperture_size > 7)
        CC_ERROR(CC_StsBadFlag, "");

    size = cvGetMatSize(src);

    cvCreateMat(dx, size.height, size.width, CC_16S, 1);
    cvCreateMat(dy, size.height, size.width, CC_16S, 1);
    cvSobel(src, dx, 1, 0, aperture_size);
    cvSobel(src, dy, 0, 1, aperture_size);

    if(flags & CC_CANNY_L2_GRADIENT)
    {
        suf32_t ul, uh;
        ul.f = (float)low_thresh;
        uh.f = (float)high_thresh;

        low = ul.i;
        high = uh.i;
    }
    else
    {
        low = cFloor(low_thresh);
        high = cFloor(high_thresh);
    }

    CC_CALL(buffer = cAlloc((size.width+2)*(size.height+2) +
                                (size.width+2)*3*sizeof(int)));

    mag_buf[0] = (int*)buffer;
    mag_buf[1] = mag_buf[0] + size.width + 2;
    mag_buf[2] = mag_buf[1] + size.width + 2;
    map = (uchar*)(mag_buf[2] + size.width + 2);
    mapstep = size.width + 2;

    maxsize = MAX(1 << 10, size.width*size.height/10);
    CC_CALL(stack_top = stack_bottom = (uchar**)cAlloc(maxsize*sizeof(stack_top[0])));

    memset(mag_buf[0], 0, (size.width+2)*sizeof(int));
    memset(map, 1, mapstep);
    memset(map + mapstep*(size.height + 1), 1, mapstep);

    /* sector numbers 
       (Top-Left Origin)

        1   2   3
         *  *  * 
          * * *  
        0*******0
          * * *  
         *  *  * 
        3   2   1
    */

    #define CANNY_PUSH(d)    *(d) = (uchar)2, *stack_top++ = (d)
    #define CANNY_POP(d)     (d) = *--stack_top

    cvMat(mag_row, 1, size.width, CC_32F);

    // calculate magnitude and angle of gradient, perform non-maxima supression.
    // fill the map with one of the following values:
    //   0 - the pixel might belong to an edge
    //   1 - the pixel can not belong to an edge
    //   2 - the pixel does belong to an edge
    for(i = 0; i <= size.height; i++)
    {
        int* _mag = mag_buf[(i > 0) + 1] + 1;
        float* _magf = (float*)_mag;
        const short* _dx = (short*)(dx->tt.data + dx->step*i);
        const short* _dy = (short*)(dy->tt.data + dy->step*i);
        uchar* _map;
        int x, y;
        int magstep1, magstep2;
        int prev_flag = 0;

        if(i < size.height)
        {
            const int icvFilterSobelVert_8u16s_C1R_p = 0;
            _mag[-1] = _mag[size.width] = 0;

            if(!(flags & CC_CANNY_L2_GRADIENT)) {
                for(j = 0; j < size.width; j++)
                    _mag[j] = abs(_dx[j]) + abs(_dy[j]);
            } else if(icvFilterSobelVert_8u16s_C1R_p != 0) // check for IPP
            {
                // use vectorized sqrt
                mag_row->tt.fl = _magf;
                for(j = 0; j < size.width; j++)
                {
                    x = _dx[j]; y = _dy[j];
                    _magf[j] = (float)((double)x*x + (double)y*y);
                }
                cvPow(mag_row, mag_row, 0.5);
            }
            else
            {
                for(j = 0; j < size.width; j++)
                {
                    x = _dx[j]; y = _dy[j];
                    _magf[j] = (float)sqrt((double)x*x + (double)y*y);
                }
            }
        }
        else
            memset(_mag-1, 0, (size.width + 2)*sizeof(int));

        // at the very beginning we do not have a complete ring
        // buffer of 3 magnitude rows for non-maxima suppression
        if(i == 0)
            continue;

        _map = map + mapstep*i + 1;
        _map[-1] = _map[size.width] = 1;
        
        _mag = mag_buf[1] + 1; // take the central row
        _dx = (short*)(dx->tt.data + dx->step*(i-1));
        _dy = (short*)(dy->tt.data + dy->step*(i-1));
        
        magstep1 = (int)(mag_buf[2] - mag_buf[1]);
        magstep2 = (int)(mag_buf[0] - mag_buf[1]);

        if((stack_top - stack_bottom) + size.width > maxsize)
        {
            uchar** new_stack_bottom;
            maxsize = MAX(maxsize * 3/2, maxsize + size.width);
            CC_CALL(new_stack_bottom = (uchar**)cAlloc(maxsize * sizeof(stack_top[0])));
            memcpy(new_stack_bottom, stack_bottom, (stack_top - stack_bottom)*sizeof(stack_top[0]));
            stack_top = new_stack_bottom + (stack_top - stack_bottom);
            cFree(&stack_bottom);
            stack_bottom = new_stack_bottom;
        }

        for(j = 0; j < size.width; j++)
        {
            #define CANNY_SHIFT 15
            #define TG22  (int)(0.4142135623730950488016887242097*(1<<CANNY_SHIFT) + 0.5)

            x = _dx[j];
            y = _dy[j];
            int s = x ^ y;
            int m = _mag[j];

            x = abs(x);
            y = abs(y);
            if(m > low)
            {
                int tg22x = x * TG22;
                int tg67x = tg22x + ((x + x) << CANNY_SHIFT);

                y <<= CANNY_SHIFT;

                if(y < tg22x)
                {
                    if(m > _mag[j-1] && m >= _mag[j+1])
                    {
                        if(m > high && !prev_flag && _map[j-mapstep] != 2)
                        {
                            CANNY_PUSH(_map + j);
                            prev_flag = 1;
                        }
                        else
                            _map[j] = (uchar)0;
                        continue;
                    }
                }
                else if(y > tg67x)
                {
                    if(m > _mag[j+magstep2] && m >= _mag[j+magstep1])
                    {
                        if(m > high && !prev_flag && _map[j-mapstep] != 2)
                        {
                            CANNY_PUSH(_map + j);
                            prev_flag = 1;
                        }
                        else
                            _map[j] = (uchar)0;
                        continue;
                    }
                }
                else
                {
                    s = s < 0 ? -1 : 1;
                    if(m > _mag[j+magstep2-s] && m > _mag[j+magstep1+s])
                    {
                        if(m > high && !prev_flag && _map[j-mapstep] != 2)
                        {
                            CANNY_PUSH(_map + j);
                            prev_flag = 1;
                        }
                        else
                            _map[j] = (uchar)0;
                        continue;
                    }
                }
            }
            prev_flag = 0;
            _map[j] = (uchar)1;
        }

        // scroll the ring buffer
        _mag = mag_buf[0];
        mag_buf[0] = mag_buf[1];
        mag_buf[1] = mag_buf[2];
        mag_buf[2] = _mag;
    }

    // now track the edges (hysteresis thresholding)
    while(stack_top > stack_bottom)
    {
        uchar* m;
        if((stack_top - stack_bottom) + 8 > maxsize)
        {
            uchar** new_stack_bottom;
            maxsize = MAX(maxsize * 3/2, maxsize + 8);
            CC_CALL(new_stack_bottom = (uchar**)cAlloc(maxsize * sizeof(stack_top[0])));
            memcpy(new_stack_bottom, stack_bottom, (stack_top - stack_bottom)*sizeof(stack_top[0]));
            stack_top = new_stack_bottom + (stack_top - stack_bottom);
            cFree(&stack_bottom);
            stack_bottom = new_stack_bottom;
        }

        CANNY_POP(m);
    
        if(!m[-1])
            CANNY_PUSH(m - 1);
        if(!m[1])
            CANNY_PUSH(m + 1);
        if(!m[-mapstep-1])
            CANNY_PUSH(m - mapstep - 1);
        if(!m[-mapstep])
            CANNY_PUSH(m - mapstep);
        if(!m[-mapstep+1])
            CANNY_PUSH(m - mapstep + 1);
        if(!m[mapstep-1])
            CANNY_PUSH(m + mapstep - 1);
        if(!m[mapstep])
            CANNY_PUSH(m + mapstep);
        if(!m[mapstep+1])
            CANNY_PUSH(m + mapstep + 1);
    }

    // the final pass, form the final image
    for(i = 0; i < size.height; i++)
    {
        const uchar* _map = map + mapstep*(i+1) + 1;
        uchar* _dst = dst->tt.data + dst->step*i;
        
        for(j = 0; j < size.width; j++)
            _dst[j] = (uchar)-(_map[j] >> 1);
    }

    __END__;

    imfree(dx);
    imfree(dy);
    cFree(&buffer);
    cFree(&stack_bottom);
}


