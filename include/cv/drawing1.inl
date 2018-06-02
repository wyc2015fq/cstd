
#ifndef _DRAWING_INL_
#define _DRAWING_INL_
#include "cvtype.inl"


#define CC_DRAWING_STORAGE_BLOCK ((1 << 12) - 256)

/* Moves iterator1 to the next line point */
#define CC_NEXT_LINE_POINT(line_iterator)                       \
{                                                               \
    int _line_iterator_mask = (line_iterator).err < 0 ? -1 : 0; \
    (line_iterator).err += (line_iterator).minus_delta +        \
        ((line_iterator).plus_delta & _line_iterator_mask);     \
    (line_iterator).ptr += (line_iterator).minus_step +         \
        ((line_iterator).plus_step & _line_iterator_mask);      \
}

typedef struct CPolyEdge
{
    int x, dx;
    union
    {
        struct CPolyEdge *next;
        int y0;
    };
    int y1;
}
CPolyEdge;

static void
icvPolyLine(img_t* img, CPoint *v, int count, int closed,
             const void* color, int thickness CC_DEFAULT(1), int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0));

static void
icvFillConvexPoly(img_t* img, CPoint* v, int npts,
                   const void* color, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0));

/****************************************************************************************\
*                                   Lines                                                *
\****************************************************************************************/

CC_IMPL int
cvClipLine(CSize img_size, CPoint* pt1, CPoint* pt2)
{
    int x1, y1, x2, y2, c1, c2;
    int result = 0, right, bottom;

    CC_FUNCNAME("cvClipLine");

    __BEGIN__;

    right = img_size.width-1, bottom = img_size.height-1;

    if(!pt1 || !pt2)
        CC_ERROR(CC_StsNullPtr, "One of point pointers is NULL");

    if(right < 0 || bottom < 0)
        CC_ERROR(CC_StsOutOfRange, "Image width or height are negative");

    x1 = pt1->x; y1 = pt1->y; x2 = pt2->x; y2 = pt2->y;
    c1 = (x1 < 0) + (x1 > right) * 2 + (y1 < 0) * 4 + (y1 > bottom) * 8;
    c2 = (x2 < 0) + (x2 > right) * 2 + (y2 < 0) * 4 + (y2 > bottom) * 8;

    if((c1 & c2) == 0 && (c1 | c2) != 0)
    {
        int a;

        if(c1 & 12)
        {
            a = c1 < 8 ? 0 : bottom;
            x1 += (int) (((int64) (a - y1)) * (x2 - x1) / (y2 - y1));
            y1 = a;
            c1 = (x1 < 0) + (x1 > right) * 2;
        }
        if(c2 & 12)
        {
            a = c2 < 8 ? 0 : bottom;
            x2 += (int) (((int64) (a - y2)) * (x2 - x1) / (y2 - y1));
            y2 = a;
            c2 = (x2 < 0) + (x2 > right) * 2;
        }
        if((c1 & c2) == 0 && (c1 | c2) != 0)
        {
            if(c1)
            {
                a = c1 == 1 ? 0 : right;
                y1 += (int) (((int64) (a - x1)) * (y2 - y1) / (x2 - x1));
                x1 = a;
                c1 = 0;
            }
            if(c2)
            {
                a = c2 == 1 ? 0 : right;
                y2 += (int) (((int64) (a - x2)) * (y2 - y1) / (x2 - x1));
                x2 = a;
                c2 = 0;
            }
        }

        assert((c1 & c2) != 0 || (x1 | y1 | x2 | y2) >= 0);

        pt1->x = x1;
        pt1->y = y1;
        pt2->x = x2;
        pt2->y = y2;
    }

    result = (c1 | c2) == 0;

    __END__;

    return result;
}


/* 
   Initializes line iterator1.
   Returns number of points on the line or negative number if error.
*/
CC_IMPL int
cvInitLineIterator(const img_t* mat, CPoint pt1, CPoint pt2,
                    CLineIterator* iterator1, int connectivity CC_DEFAULT(8),
                    int left_to_right CC_DEFAULT(0))
{
    int dx, dy, s, bt_pix, bt_pix0, step;
    int count = -1;
    
    CC_FUNCNAME("cvInitLineIterator");

    __BEGIN__;
    
    if(!iterator1)
        CC_ERROR(CC_StsNullPtr, "Pointer to the iterator1 state is NULL");

    if(connectivity != 8 && connectivity != 4)
        CC_ERROR(CC_StsBadArg, "Connectivity must be 8 or 4");

    if((unsigned)pt1.x >= (unsigned)(mat->width) ||
        (unsigned)pt2.x >= (unsigned)(mat->width) ||
        (unsigned)pt1.y >= (unsigned)(mat->height) ||
        (unsigned)pt2.y >= (unsigned)(mat->height))
        CC_ERROR(CC_StsBadPoint,
            "One of the ending points is outside of the image, use cvClipLine");

    bt_pix0 = bt_pix = mat->c;
    step = mat->step;

    dx = pt2.x - pt1.x;
    dy = pt2.y - pt1.y;
    s = dx < 0 ? -1 : 0;

    if(left_to_right)
    {
        dx = (dx ^ s) - s;
        dy = (dy ^ s) - s;
        pt1.x ^= (pt1.x ^ pt2.x) & s;
        pt1.y ^= (pt1.y ^ pt2.y) & s;
    }
    else
    {
        dx = (dx ^ s) - s;
        bt_pix = (bt_pix ^ s) - s;
    }

    iterator1->ptr = (uchar*)(mat->tt.data + pt1.y * step + pt1.x * bt_pix0);

    s = dy < 0 ? -1 : 0;
    dy = (dy ^ s) - s;
    step = (step ^ s) - s;

    s = dy > dx ? -1 : 0;
    
    /* conditional swaps */
    dx ^= dy & s;
    dy ^= dx & s;
    dx ^= dy & s;

    bt_pix ^= step & s;
    step ^= bt_pix & s;
    bt_pix ^= step & s;

    if(connectivity == 8)
    {
        assert(dx >= 0 && dy >= 0);
        
        iterator1->err = dx - (dy + dy);
        iterator1->plus_delta = dx + dx;
        iterator1->minus_delta = -(dy + dy);
        iterator1->plus_step = step;
        iterator1->minus_step = bt_pix;
        count = dx + 1;
    }
    else /* connectivity == 4 */
    {
        assert(dx >= 0 && dy >= 0);
        
        iterator1->err = 0;
        iterator1->plus_delta = (dx + dx) + (dy + dy);
        iterator1->minus_delta = -(dy + dy);
        iterator1->plus_step = step - bt_pix;
        iterator1->minus_step = bt_pix;
        count = dx + dy + 1;
    }

    __END__;

    return count;
}

static void
icvLine(img_t* mat, CPoint pt1, CPoint pt2,
         const void* color, int connectivity CC_DEFAULT(8) )
{
    if(cvClipLine(cvGetMatSize(mat), &pt1, &pt2))
    {
        CLineIterator iterator1;
        int pix_size = CC_ELEM_SIZE1(mat);
        int i, count;
        
        if(connectivity == 0)
            connectivity = 8;
        if(connectivity == 1)
            connectivity = 4;
        
        count = cvInitLineIterator(mat, pt1, pt2, &iterator1, connectivity, 1);

        for(i = 0; i < count; i++)
        {
            CC_MEMCPY_AUTO(iterator1.ptr, color, pix_size);
            CC_NEXT_LINE_POINT(iterator1);
        }
    }
}


/* Correction table depent on the slope */
static const uchar icvSlopeCorrTable[] = {
    181, 181, 181, 182, 182, 183, 184, 185, 187, 188, 190, 192, 194, 196, 198, 201,
    203, 206, 209, 211, 214, 218, 221, 224, 227, 231, 235, 238, 242, 246, 250, 254
};

/* Gaussian for antialiasing filter */
static const int icvFilterTable[] = {
    168, 177, 185, 194, 202, 210, 218, 224, 231, 236, 241, 246, 249, 252, 254, 254,
    254, 254, 252, 249, 246, 241, 236, 231, 224, 218, 210, 202, 194, 185, 177, 168,
    158, 149, 140, 131, 122, 114, 105, 97, 89, 82, 75, 68, 62, 56, 50, 45,
    40, 36, 32, 28, 25, 22, 19, 16, 14, 12, 11, 9, 8, 7, 5, 5
};

static void
icvLineAA(img_t* img, CPoint pt1, CPoint pt2,
           const void* color)
{
    int dx, dy;
    int ecount, scount = 0;
    int slope;
    int ax, ay;
    int x_step, y_step;
    int i, j;
    int ep_table[9];
    int cb = ((uchar*)color)[0], cg = ((uchar*)color)[1], cr = ((uchar*)color)[2];
    int _cb, _cg, _cr;
    int nch = CC_MAT_CN(img);
    uchar* ptr = (uchar*)(img->tt.data);
    int step = img->step;
    CSize size = cvGetMatSize(img);

    assert(img && (nch == 1 || nch == 3) && CC_MAT_DEPTH(img) == CC_8U);

    pt1.x -= XY_ONE*2;
    pt1.y -= XY_ONE*2;
    pt2.x -= XY_ONE*2;
    pt2.y -= XY_ONE*2;
    ptr += img->step*2 + 2*nch;

    size.width = ((size.width - 5) << XY_SHIFT) + 1;
    size.height = ((size.height - 5) << XY_SHIFT) + 1;

    if(!cvClipLine(size, &pt1, &pt2))
        return;

    dx = pt2.x - pt1.x;
    dy = pt2.y - pt1.y;

    j = dx < 0 ? -1 : 0;
    ax = (dx ^ j) - j;
    i = dy < 0 ? -1 : 0;
    ay = (dy ^ i) - i;

    if(ax > ay)
    {
        dx = ax;
        dy = (dy ^ j) - j;
        pt1.x ^= pt2.x & j;
        pt2.x ^= pt1.x & j;
        pt1.x ^= pt2.x & j;
        pt1.y ^= pt2.y & j;
        pt2.y ^= pt1.y & j;
        pt1.y ^= pt2.y & j;

        x_step = XY_ONE;
        y_step = (int) (((int64) dy << XY_SHIFT) / (ax | 1));
        pt2.x += XY_ONE;
        ecount = (pt2.x >> XY_SHIFT) - (pt1.x >> XY_SHIFT);
        j = -(pt1.x & (XY_ONE - 1));
        pt1.y += (int) ((((int64) y_step) * j) >> XY_SHIFT) + (XY_ONE >> 1);
        slope = (y_step >> (XY_SHIFT - 5)) & 0x3f;
        slope ^= (y_step < 0 ? 0x3f : 0);

        /* Get 4-bit fractions for end-point adjustments */
        i = (pt1.x >> (XY_SHIFT - 7)) & 0x78;
        j = (pt2.x >> (XY_SHIFT - 7)) & 0x78;
    }
    else
    {
        dy = ay;
        dx = (dx ^ i) - i;
        pt1.x ^= pt2.x & i;
        pt2.x ^= pt1.x & i;
        pt1.x ^= pt2.x & i;
        pt1.y ^= pt2.y & i;
        pt2.y ^= pt1.y & i;
        pt1.y ^= pt2.y & i;

        x_step = (int) (((int64) dx << XY_SHIFT) / (ay | 1));
        y_step = XY_ONE;
        pt2.y += XY_ONE;
        ecount = (pt2.y >> XY_SHIFT) - (pt1.y >> XY_SHIFT);
        j = -(pt1.y & (XY_ONE - 1));
        pt1.x += (int) ((((int64) x_step) * j) >> XY_SHIFT) + (XY_ONE >> 1);
        slope = (x_step >> (XY_SHIFT - 5)) & 0x3f;
        slope ^= (x_step < 0 ? 0x3f : 0);

        /* Get 4-bit fractions for end-point adjustments */
        i = (pt1.y >> (XY_SHIFT - 7)) & 0x78;
        j = (pt2.y >> (XY_SHIFT - 7)) & 0x78;
    }

    slope = (slope & 0x20) ? 0x100 : icvSlopeCorrTable[slope];

    /* Calc end point correction table */
    {
        int t0 = slope << 7;
        int t1 = ((0x78 - i) | 4) * slope;
        int t2 = (j | 4) * slope;

        ep_table[0] = 0;
        ep_table[8] = slope;
        ep_table[1] = ep_table[3] = ((((j - i) & 0x78) | 4) * slope >> 8) & 0x1ff;
        ep_table[2] = (t1 >> 8) & 0x1ff;
        ep_table[4] = ((((j - i) + 0x80) | 4) * slope >> 8) & 0x1ff;
        ep_table[5] = ((t1 + t0) >> 8) & 0x1ff;
        ep_table[6] = (t2 >> 8) & 0x1ff;
        ep_table[7] = ((t2 + t0) >> 8) & 0x1ff;
    }

    if(nch == 3)
    {
#undef ICC_PUT_POINT
        #define  ICC_PUT_POINT()            \
        {                                   \
            _cb = tptr[0];                  \
            _cb += ((cb - _cb)*a + 127)>> 8;\
            _cg = tptr[1];                  \
            _cg += ((cg - _cg)*a + 127)>> 8;\
            _cr = tptr[2];                  \
            _cr += ((cr - _cr)*a + 127)>> 8;\
            tptr[0] = (uchar)_cb;           \
            tptr[1] = (uchar)_cg;           \
            tptr[2] = (uchar)_cr;           \
        }
        if(ax > ay)
        {
            ptr += (pt1.x >> XY_SHIFT) * 3;

            while(ecount >= 0)
            {
                uchar *tptr = ptr + ((pt1.y >> XY_SHIFT) - 1) * step;

                int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 +
                                       (((ecount >= 2) + 1) & (ecount | 2))];
                int a, dist = (pt1.y >> (XY_SHIFT - 5)) & 31;

                a = (ep_corr * icvFilterTable[dist + 32] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += step;
                a = (ep_corr * icvFilterTable[dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += step;
                a = (ep_corr * icvFilterTable[63 - dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                pt1.y += y_step;
                ptr += 3;
                scount++;
                ecount--;
            }
        }
        else
        {
            ptr += (pt1.y >> XY_SHIFT) * step;

            while(ecount >= 0)
            {
                uchar *tptr = ptr + ((pt1.x >> XY_SHIFT) - 1) * 3;

                int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 +
                                       (((ecount >= 2) + 1) & (ecount | 2))];
                int a, dist = (pt1.x >> (XY_SHIFT - 5)) & 31;

                a = (ep_corr * icvFilterTable[dist + 32] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += 3;
                a = (ep_corr * icvFilterTable[dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += 3;
                a = (ep_corr * icvFilterTable[63 - dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                pt1.x += x_step;
                ptr += step;
                scount++;
                ecount--;
            }
        }
        #undef ICC_PUT_POINT
    }
    else
    {
        #define  ICC_PUT_POINT()            \
        {                                   \
            _cb = tptr[0];                  \
            _cb += ((cb - _cb)*a + 127)>> 8;\
            tptr[0] = (uchar)_cb;           \
        }

        if(ax > ay)
        {
            ptr += (pt1.x >> XY_SHIFT);

            while(ecount >= 0)
            {
                uchar *tptr = ptr + ((pt1.y >> XY_SHIFT) - 1) * step;

                int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 +
                                       (((ecount >= 2) + 1) & (ecount | 2))];
                int a, dist = (pt1.y >> (XY_SHIFT - 5)) & 31;

                a = (ep_corr * icvFilterTable[dist + 32] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += step;
                a = (ep_corr * icvFilterTable[dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += step;
                a = (ep_corr * icvFilterTable[63 - dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                pt1.y += y_step;
                ptr++;
                scount++;
                ecount--;
            }
        }
        else
        {
            ptr += (pt1.y >> XY_SHIFT) * step;

            while(ecount >= 0)
            {
                uchar *tptr = ptr + ((pt1.x >> XY_SHIFT) - 1);

                int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 +
                                       (((ecount >= 2) + 1) & (ecount | 2))];
                int a, dist = (pt1.x >> (XY_SHIFT - 5)) & 31;

                a = (ep_corr * icvFilterTable[dist + 32] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr++;
                a = (ep_corr * icvFilterTable[dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr++;
                a = (ep_corr * icvFilterTable[63 - dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                pt1.x += x_step;
                ptr += step;
                scount++;
                ecount--;
            }
        }
        #undef ICC_PUT_POINT
    }
}


static void
icvLine2(img_t* img, CPoint pt1, CPoint pt2, const void* color)
{
    int dx, dy;
    int ecount;
    int ax, ay;
    int i, j;
    int x_step, y_step;
    int cb = ((uchar*)color)[0], cg = ((uchar*)color)[1], cr = ((uchar*)color)[2];
    int pix_size = CC_ELEM_SIZE1(img);
    uchar *ptr = (uchar*)(img->tt.data), *tptr;
    int step = img->step;
    CSize size = cvGetMatSize(img);

    //assert(img && (nch == 1 || nch == 3) && CC_MAT_DEPTH(img) == CC_8U);

    pt1.x -= XY_ONE*2;
    pt1.y -= XY_ONE*2;
    pt2.x -= XY_ONE*2;
    pt2.y -= XY_ONE*2;
    ptr += img->step*2 + 2*pix_size;

    size.width = ((size.width - 5) << XY_SHIFT) + 1;
    size.height = ((size.height - 5) << XY_SHIFT) + 1;

    if(!cvClipLine(size, &pt1, &pt2))
        return;

    dx = pt2.x - pt1.x;
    dy = pt2.y - pt1.y;

    j = dx < 0 ? -1 : 0;
    ax = (dx ^ j) - j;
    i = dy < 0 ? -1 : 0;
    ay = (dy ^ i) - i;

    if(ax > ay)
    {
        dx = ax;
        dy = (dy ^ j) - j;
        pt1.x ^= pt2.x & j;
        pt2.x ^= pt1.x & j;
        pt1.x ^= pt2.x & j;
        pt1.y ^= pt2.y & j;
        pt2.y ^= pt1.y & j;
        pt1.y ^= pt2.y & j;

        x_step = XY_ONE;
        y_step = (int) (((int64) dy << XY_SHIFT) / (ax | 1));
        ecount = (pt2.x - pt1.x) >> XY_SHIFT;
    }
    else
    {
        dy = ay;
        dx = (dx ^ i) - i;
        pt1.x ^= pt2.x & i;
        pt2.x ^= pt1.x & i;
        pt1.x ^= pt2.x & i;
        pt1.y ^= pt2.y & i;
        pt2.y ^= pt1.y & i;
        pt1.y ^= pt2.y & i;

        x_step = (int) (((int64) dx << XY_SHIFT) / (ay | 1));
        y_step = XY_ONE;
        ecount = (pt2.y - pt1.y) >> XY_SHIFT;
    }

    pt1.x += (XY_ONE >> 1);
    pt1.y += (XY_ONE >> 1);

    if(pix_size == 3)
    {
        #define  ICC_PUT_POINT()    \
        {                           \
            tptr[0] = (uchar)cb;    \
            tptr[1] = (uchar)cg;    \
            tptr[2] = (uchar)cr;    \
        }
        
        tptr = ptr + ((pt2.x + (XY_ONE >> 1))>> XY_SHIFT)*3 +
            ((pt2.y + (XY_ONE >> 1)) >> XY_SHIFT)*step;
        ICC_PUT_POINT();
        
        if(ax > ay)
        {
            ptr += (pt1.x >> XY_SHIFT) * 3;

            while(ecount >= 0)
            {
                tptr = ptr + (pt1.y >> XY_SHIFT) * step;
                ICC_PUT_POINT();
                pt1.y += y_step;
                ptr += 3;
                ecount--;
            }
        }
        else
        {
            ptr += (pt1.y >> XY_SHIFT) * step;

            while(ecount >= 0)
            {
                tptr = ptr + (pt1.x >> XY_SHIFT) * 3;
                ICC_PUT_POINT();
                pt1.x += x_step;
                ptr += step;
                ecount--;
            }
        }

        #undef ICC_PUT_POINT
    }
    else if(pix_size == 1)
    {
        #define  ICC_PUT_POINT()            \
        {                                   \
            tptr[0] = (uchar)cb;            \
        }

        tptr = ptr + ((pt2.x + (XY_ONE >> 1))>> XY_SHIFT) +
            ((pt2.y + (XY_ONE >> 1)) >> XY_SHIFT)*step;
        ICC_PUT_POINT();

        if(ax > ay)
        {
            ptr += (pt1.x >> XY_SHIFT);

            while(ecount >= 0)
            {
                tptr = ptr + (pt1.y >> XY_SHIFT) * step;
                ICC_PUT_POINT();
                pt1.y += y_step;
                ptr++;
                ecount--;
            }
        }
        else
        {
            ptr += (pt1.y >> XY_SHIFT) * step;

            while(ecount >= 0)
            {
                tptr = ptr + (pt1.x >> XY_SHIFT);
                ICC_PUT_POINT();
                pt1.x += x_step;
                ptr += step;
                ecount--;
            }
        }
        #undef ICC_PUT_POINT
    }
    else
    {
        #define  ICC_PUT_POINT()                \
            for(j = 0; j < pix_size; j++)     \
                tptr[j] = ((uchar*)color)[j];

        tptr = ptr + ((pt2.x + (XY_ONE >> 1))>> XY_SHIFT)*pix_size +
            ((pt2.y + (XY_ONE >> 1)) >> XY_SHIFT)*step;
        ICC_PUT_POINT();
        
        if(ax > ay)
        {
            ptr += (pt1.x >> XY_SHIFT) * pix_size;

            while(ecount >= 0)
            {
                tptr = ptr + (pt1.y >> XY_SHIFT) * step;
                ICC_PUT_POINT();
                pt1.y += y_step;
                ptr += pix_size;
                ecount--;
            }
        }
        else
        {
            ptr += (pt1.y >> XY_SHIFT) * step;

            while(ecount >= 0)
            {
                tptr = ptr + (pt1.x >> XY_SHIFT) * pix_size;
                ICC_PUT_POINT();
                pt1.x += x_step;
                ptr += step;
                ecount--;
            }
        }

        #undef ICC_PUT_POINT
    }
}


/****************************************************************************************\
*                   Antialiazed Elliptic Arcs via Antialiazed Lines                      *
\****************************************************************************************/

static const float icvSinTable[] =
    { 0.0000000f, 0.0174524f, 0.0348995f, 0.0523360f, 0.0697565f, 0.0871557f,
    0.1045285f, 0.1218693f, 0.1391731f, 0.1564345f, 0.1736482f, 0.1908090f,
    0.2079117f, 0.2249511f, 0.2419219f, 0.2588190f, 0.2756374f, 0.2923717f,
    0.3090170f, 0.3255682f, 0.3420201f, 0.3583679f, 0.3746066f, 0.3907311f,
    0.4067366f, 0.4226183f, 0.4383711f, 0.4539905f, 0.4694716f, 0.4848096f,
    0.5000000f, 0.5150381f, 0.5299193f, 0.5446390f, 0.5591929f, 0.5735764f,
    0.5877853f, 0.6018150f, 0.6156615f, 0.6293204f, 0.6427876f, 0.6560590f,
    0.6691306f, 0.6819984f, 0.6946584f, 0.7071068f, 0.7193398f, 0.7313537f,
    0.7431448f, 0.7547096f, 0.7660444f, 0.7771460f, 0.7880108f, 0.7986355f,
    0.8090170f, 0.8191520f, 0.8290376f, 0.8386706f, 0.8480481f, 0.8571673f,
    0.8660254f, 0.8746197f, 0.8829476f, 0.8910065f, 0.8987940f, 0.9063078f,
    0.9135455f, 0.9205049f, 0.9271839f, 0.9335804f, 0.9396926f, 0.9455186f,
    0.9510565f, 0.9563048f, 0.9612617f, 0.9659258f, 0.9702957f, 0.9743701f,
    0.9781476f, 0.9816272f, 0.9848078f, 0.9876883f, 0.9902681f, 0.9925462f,
    0.9945219f, 0.9961947f, 0.9975641f, 0.9986295f, 0.9993908f, 0.9998477f,
    1.0000000f, 0.9998477f, 0.9993908f, 0.9986295f, 0.9975641f, 0.9961947f,
    0.9945219f, 0.9925462f, 0.9902681f, 0.9876883f, 0.9848078f, 0.9816272f,
    0.9781476f, 0.9743701f, 0.9702957f, 0.9659258f, 0.9612617f, 0.9563048f,
    0.9510565f, 0.9455186f, 0.9396926f, 0.9335804f, 0.9271839f, 0.9205049f,
    0.9135455f, 0.9063078f, 0.8987940f, 0.8910065f, 0.8829476f, 0.8746197f,
    0.8660254f, 0.8571673f, 0.8480481f, 0.8386706f, 0.8290376f, 0.8191520f,
    0.8090170f, 0.7986355f, 0.7880108f, 0.7771460f, 0.7660444f, 0.7547096f,
    0.7431448f, 0.7313537f, 0.7193398f, 0.7071068f, 0.6946584f, 0.6819984f,
    0.6691306f, 0.6560590f, 0.6427876f, 0.6293204f, 0.6156615f, 0.6018150f,
    0.5877853f, 0.5735764f, 0.5591929f, 0.5446390f, 0.5299193f, 0.5150381f,
    0.5000000f, 0.4848096f, 0.4694716f, 0.4539905f, 0.4383711f, 0.4226183f,
    0.4067366f, 0.3907311f, 0.3746066f, 0.3583679f, 0.3420201f, 0.3255682f,
    0.3090170f, 0.2923717f, 0.2756374f, 0.2588190f, 0.2419219f, 0.2249511f,
    0.2079117f, 0.1908090f, 0.1736482f, 0.1564345f, 0.1391731f, 0.1218693f,
    0.1045285f, 0.0871557f, 0.0697565f, 0.0523360f, 0.0348995f, 0.0174524f,
    0.0000000f, -0.0174524f, -0.0348995f, -0.0523360f, -0.0697565f, -0.0871557f,
    -0.1045285f, -0.1218693f, -0.1391731f, -0.1564345f, -0.1736482f, -0.1908090f,
    -0.2079117f, -0.2249511f, -0.2419219f, -0.2588190f, -0.2756374f, -0.2923717f,
    -0.3090170f, -0.3255682f, -0.3420201f, -0.3583679f, -0.3746066f, -0.3907311f,
    -0.4067366f, -0.4226183f, -0.4383711f, -0.4539905f, -0.4694716f, -0.4848096f,
    -0.5000000f, -0.5150381f, -0.5299193f, -0.5446390f, -0.5591929f, -0.5735764f,
    -0.5877853f, -0.6018150f, -0.6156615f, -0.6293204f, -0.6427876f, -0.6560590f,
    -0.6691306f, -0.6819984f, -0.6946584f, -0.7071068f, -0.7193398f, -0.7313537f,
    -0.7431448f, -0.7547096f, -0.7660444f, -0.7771460f, -0.7880108f, -0.7986355f,
    -0.8090170f, -0.8191520f, -0.8290376f, -0.8386706f, -0.8480481f, -0.8571673f,
    -0.8660254f, -0.8746197f, -0.8829476f, -0.8910065f, -0.8987940f, -0.9063078f,
    -0.9135455f, -0.9205049f, -0.9271839f, -0.9335804f, -0.9396926f, -0.9455186f,
    -0.9510565f, -0.9563048f, -0.9612617f, -0.9659258f, -0.9702957f, -0.9743701f,
    -0.9781476f, -0.9816272f, -0.9848078f, -0.9876883f, -0.9902681f, -0.9925462f,
    -0.9945219f, -0.9961947f, -0.9975641f, -0.9986295f, -0.9993908f, -0.9998477f,
    -1.0000000f, -0.9998477f, -0.9993908f, -0.9986295f, -0.9975641f, -0.9961947f,
    -0.9945219f, -0.9925462f, -0.9902681f, -0.9876883f, -0.9848078f, -0.9816272f,
    -0.9781476f, -0.9743701f, -0.9702957f, -0.9659258f, -0.9612617f, -0.9563048f,
    -0.9510565f, -0.9455186f, -0.9396926f, -0.9335804f, -0.9271839f, -0.9205049f,
    -0.9135455f, -0.9063078f, -0.8987940f, -0.8910065f, -0.8829476f, -0.8746197f,
    -0.8660254f, -0.8571673f, -0.8480481f, -0.8386706f, -0.8290376f, -0.8191520f,
    -0.8090170f, -0.7986355f, -0.7880108f, -0.7771460f, -0.7660444f, -0.7547096f,
    -0.7431448f, -0.7313537f, -0.7193398f, -0.7071068f, -0.6946584f, -0.6819984f,
    -0.6691306f, -0.6560590f, -0.6427876f, -0.6293204f, -0.6156615f, -0.6018150f,
    -0.5877853f, -0.5735764f, -0.5591929f, -0.5446390f, -0.5299193f, -0.5150381f,
    -0.5000000f, -0.4848096f, -0.4694716f, -0.4539905f, -0.4383711f, -0.4226183f,
    -0.4067366f, -0.3907311f, -0.3746066f, -0.3583679f, -0.3420201f, -0.3255682f,
    -0.3090170f, -0.2923717f, -0.2756374f, -0.2588190f, -0.2419219f, -0.2249511f,
    -0.2079117f, -0.1908090f, -0.1736482f, -0.1564345f, -0.1391731f, -0.1218693f,
    -0.1045285f, -0.0871557f, -0.0697565f, -0.0523360f, -0.0348995f, -0.0174524f,
    -0.0000000f, 0.0174524f, 0.0348995f, 0.0523360f, 0.0697565f, 0.0871557f,
    0.1045285f, 0.1218693f, 0.1391731f, 0.1564345f, 0.1736482f, 0.1908090f,
    0.2079117f, 0.2249511f, 0.2419219f, 0.2588190f, 0.2756374f, 0.2923717f,
    0.3090170f, 0.3255682f, 0.3420201f, 0.3583679f, 0.3746066f, 0.3907311f,
    0.4067366f, 0.4226183f, 0.4383711f, 0.4539905f, 0.4694716f, 0.4848096f,
    0.5000000f, 0.5150381f, 0.5299193f, 0.5446390f, 0.5591929f, 0.5735764f,
    0.5877853f, 0.6018150f, 0.6156615f, 0.6293204f, 0.6427876f, 0.6560590f,
    0.6691306f, 0.6819984f, 0.6946584f, 0.7071068f, 0.7193398f, 0.7313537f,
    0.7431448f, 0.7547096f, 0.7660444f, 0.7771460f, 0.7880108f, 0.7986355f,
    0.8090170f, 0.8191520f, 0.8290376f, 0.8386706f, 0.8480481f, 0.8571673f,
    0.8660254f, 0.8746197f, 0.8829476f, 0.8910065f, 0.8987940f, 0.9063078f,
    0.9135455f, 0.9205049f, 0.9271839f, 0.9335804f, 0.9396926f, 0.9455186f,
    0.9510565f, 0.9563048f, 0.9612617f, 0.9659258f, 0.9702957f, 0.9743701f,
    0.9781476f, 0.9816272f, 0.9848078f, 0.9876883f, 0.9902681f, 0.9925462f,
    0.9945219f, 0.9961947f, 0.9975641f, 0.9986295f, 0.9993908f, 0.9998477f,
    1.0000000f
};


static void
icvSinCos(int angle, float *cosval, float *sinval)
{
    angle += (angle < 0 ? 360 : 0);
    *sinval = icvSinTable[angle];
    *cosval = icvSinTable[450 - angle];
}

/* 
   constructs polygon that represents elliptic arc.
*/
CC_IMPL int
cvEllipse2Poly(CPoint center, CSize axes, int angle,
                int arc_start, int arc_end, CPoint* pts, int delta)
{
    float alpha, beta;
    double size_a = axes.width, size_b = axes.height;
    double cx = center.x, cy = center.y;
    CPoint *pts_origin = pts;
    int i;

    while(angle < 0)
        angle += 360;
    while(angle > 360)
        angle -= 360;

    if(arc_start > arc_end)
    {
        i = arc_start;
        arc_start = arc_end;
        arc_end = i;
    }
    while(arc_start < 0)
    {
        arc_start += 360;
        arc_end += 360;
    }
    while(arc_end > 360)
    {
        arc_end -= 360;
        arc_start -= 360;
    }
    if(arc_end - arc_start > 360)
    {
        arc_start = 0;
        arc_end = 360;
    }
    icvSinCos(angle, &alpha, &beta);

    for(i = arc_start; i < arc_end + delta; i += delta)
    {
        double x, y;
        angle = i;
        if(angle > arc_end)
            angle = arc_end;
        if(angle < 0)
            angle += 360;
        
        x = size_a * icvSinTable[450-angle];
        y = size_b * icvSinTable[angle];
        pts->x = cRound(cx + x * alpha - y * beta);
        pts->y = cRound(cy - x * beta - y * alpha);
        pts += i == arc_start || pts->x != pts[-1].x || pts->y != pts[-1].y;
    }

    i = (int)(pts - pts_origin);
    for(; i < 2; i++)
        pts_origin[i] = pts_origin[i-1];
    return i;
}

/******** Arbitrary polygon **********/

static void
icvCollectPolyEdges(img_t* img, const img_t* v, CPolyEdge* edges, CRect* prect,
                     const void* color, int line_type, int shift, CPoint offset CC_DEFAULT(iPOINT(0,0)))
{
    int  i, count = v->h*v->w;
    CRect bounds = *prect;
    int delta = offset.y + (shift ? 1 << (shift - 1) : 0);
    TypeId elem_type = CC_MAT_DEPTH(v);
    const CPoint* pv = (CPoint*)v->tt.data;

    for(i = 0; i < count; i++)
    {
        CPoint pt0, pt1, t0, t1;
        CPolyEdge* edge = edges + i;
        pt0 = pv[i];
        pt1 = pv[(i+1)%count];
        
        if(elem_type == CC_32S)
        {
            pt0.x = (pt0.x + offset.x) << (XY_SHIFT - shift);
            pt0.y = (pt0.y + delta) >> shift;
            pt1.x = (pt1.x + offset.x) << (XY_SHIFT - shift);
            pt1.y = (pt1.y + delta) >> shift;
        }
        else
        {
            suf32_t x, y;
            assert(shift == 0);

            x.i = pt0.x; y.i = pt0.y;
            pt0.x = cRound((x.f + offset.x) * XY_ONE);
            pt0.y = cRound(y.f + offset.y);
            x.i = pt1.x; y.i = pt1.y;
            pt1.x = cRound((x.f + offset.x) * XY_ONE);
            pt1.y = cRound(y.f + offset.y);
        }

        if(line_type < CC_AA)
        {
            t0.y = pt0.y; t1.y = pt1.y;
            t0.x = (pt0.x + (XY_ONE >> 1)) >> XY_SHIFT;
            t1.x = (pt1.x + (XY_ONE >> 1)) >> XY_SHIFT;
            icvLine(img, t0, t1, color, line_type);
        }
        else
        {
            t0.x = pt0.x; t1.x = pt1.x;
            t0.y = pt0.y << XY_SHIFT;
            t1.y = pt1.y << XY_SHIFT;
            icvLineAA(img, t0, t1, color);
        }

        if(pt0.y == pt1.y)
            continue;

        if(pt0.y > pt1.y)
            CC_SWAP(pt0, pt1, t0);

        bounds.y = MIN(bounds.y, pt0.y);
        bounds.height = MAX(bounds.height, pt1.y);

        if(pt0.x < pt1.x)
        {
            bounds.x = MIN(bounds.x, pt0.x);
            bounds.width = MAX(bounds.width, pt1.x);
        }
        else
        {
            bounds.x = MIN(bounds.x, pt1.x);
            bounds.width = MAX(bounds.width, pt0.x);
        }

        edge->y0 = pt0.y;
        edge->y1 = pt1.y;
        edge->x = pt0.x;
        edge->dx = (pt1.x - pt0.x) / (pt1.y - pt0.y);
        assert(edge->y0 < edge->y1);
    }

    *prect = bounds;
}

static int
icvCmpEdges(const CPolyEdge* _e1, const CPolyEdge* _e2, void* p)
{
    CPolyEdge *e1 = (CPolyEdge*)_e1, *e2 = (CPolyEdge*)_e2;
    return e1->y0 - e2->y0 ? e1->y0 - e2->y0 :
           e1->x - e2->x ? e1->x - e2->x : e1->dx - e2->dx;
}

/**************** helper macros and functions for sequence/contour processing ***********/
#undef ICC_HLINE
/* helper macros: filling horizontal row */
#define ICC_HLINE(ptr, xl, xr, color, pix_size)                \
{                                                              \
    uchar* hline_ptr = (uchar*)(ptr) + (xl)*(pix_size);        \
    uchar* hline_max_ptr = (uchar*)(ptr) + (xr)*(pix_size);    \
                                                               \
    for(; hline_ptr <= hline_max_ptr; hline_ptr += (pix_size)) \
    {                                                          \
        int hline_j;                                           \
        for(hline_j = 0; hline_j < (pix_size); hline_j++)      \
        {                                                      \
            hline_ptr[hline_j] = ((uchar*)color)[hline_j];     \
        }                                                      \
    }                                                          \
}

static void
icvFillEdgeCollection(img_t* img, CPolyEdge* edges, int total, CRect rect, const void* color)
{
    CPolyEdge tmp;
    int i, y;
    CSize size = cvGetMatSize(img);
    CPolyEdge* e;
    int y_max = INT_MIN;
    int pix_size = CC_ELEM_SIZE1(img);

    __BEGIN__;
    
    memset(&tmp, 0, sizeof(tmp));
    
    /* check parameters */
    if(total < 2 || rect.height < 0 || rect.y >= size.height ||
        rect.width < 0 || rect.x >= size.width)
        EXIT;

    //cvSeqSort((CvSeq*)edges, icvCmpEdges, 0);
#define ICVCMPEDGES(a, b)  icvCmpEdges(&(a), &(b), 0)
    QSORT(edges, (edges+total), ICVCMPEDGES, CPolyEdge);
#undef ICVCMPEDGES
    //qsort();
    //cvStartReadSeq((CvSeq*)edges, &reader);

#ifdef _DEBUG
    e = &tmp;
    tmp.y0 = INT_MIN;
#endif

    for(i = 0; i < total; i++)
    {
        CPolyEdge* e1 = edges + i;

#ifdef _DEBUG
        assert(e1->y0 < e1->y1 && (i == 0 || icvCmpEdges(e, e1, 0) <= 0));
        e = e1;
#endif
        y_max = MAX(y_max, e1->y1);
    }

    /* start drawing */
    tmp.y0 = INT_MAX;

    i = 0;
    tmp.next = 0;
    //cvStartReadSeq((CvSeq*)edges, &reader);
    e = edges;
    y_max = MIN(y_max, size.height);

    for(y = e->y0; y < y_max; y++)
    {
        CPolyEdge *last, *prelast, *keep_prelast;
        int sort_flag = 0;
        int draw = 0;
        int clipline = y < 0;

        prelast = &tmp;
        last = tmp.next;
        while(last || e->y0 == y)
        {
            if(last && last->y1 == y)
            {
                /* exlude edge if y reachs its lower point */
                prelast->next = last->next;
                last = last->next;
                continue;
            }
            keep_prelast = prelast;
            if(last && (e->y0 > y || last->x < e->x))
            {
                /* go to the next edge in active list */
                prelast = last;
                last = last->next;
            }
            else if(i < total)
            {
                /* insert new edge into active list if y reachs its upper point */
                prelast->next = e;
                e->next = last;
                prelast = e;
                //CC_NEXT_SEQ_ELEM(edges->elem_size, reader);
                e++;
                i++;
            }
            else
                break;

            if(draw)
            {
                if(!clipline)
                {
                    /* convert x's from fixed-point to image coordinates */
                    uchar *timg = (uchar*)(img->tt.data) + y * img->step;
                    int x1 = keep_prelast->x;
                    int x2 = prelast->x;

                    if(x1 > x2)
                    {
                        int t = x1;

                        x1 = x2;
                        x2 = t;
                    }

                    x1 = (x1 + XY_ONE - 1) >> XY_SHIFT;
                    x2 = x2 >> XY_SHIFT;

                    /* clip and draw the line */
                    if(x1 < size.width && x2 >= 0)
                    {
                        if(x1 < 0)
                            x1 = 0;
                        if(x2 >= size.width)
                            x2 = size.width - 1;
                        ICC_HLINE(timg, x1, x2, color, pix_size);
                    }
                }
                keep_prelast->x += keep_prelast->dx;
                prelast->x += prelast->dx;
            }
            draw ^= 1;
        }

        /* sort edges (bubble sort on list) */
        keep_prelast = 0;

        do
        {
            prelast = &tmp;
            last = tmp.next;

            while(last != keep_prelast && last->next != 0)
            {
                CPolyEdge *te = last->next;

                /* swap edges */
                if(last->x > te->x)
                {
                    prelast->next = te;
                    last->next = te->next;
                    te->next = last;
                    prelast = te;
                    sort_flag = 1;
                }
                else
                {
                    prelast = last;
                    last = te;
                }
            }
            keep_prelast = prelast;
        }
        while(sort_flag && keep_prelast != tmp.next && keep_prelast != &tmp);
    }

    __END__;
}



static void
icvEllipseEx(img_t* img, CPoint center, CSize axes,
              int angle, int arc_start, int arc_end,
              const void* color, int thickness CC_DEFAULT(1), int line_type CC_DEFAULT(8))
{
    CPoint v[1 << 8];
    int count, delta;
    CC_FUNCNAME("icvEllipseEx");
    
    __BEGIN__;


    if(axes.width < 0 || axes.height < 0)
        CC_ERROR(CC_StsBadSize, "");

    delta = (MAX(axes.width,axes.height)+(XY_ONE>>1))>>XY_SHIFT;
    delta = delta < 3 ? 90 : delta < 10 ? 30 : delta < 15 ? 18 : 5;

    count = cvEllipse2Poly(center, axes, angle, arc_start, arc_end, v, delta);

    if(thickness >= 0)
    {
        icvPolyLine(img, v, count, 0, color, thickness, line_type, XY_SHIFT);
    }
    else if(arc_end - arc_start >= 360)
    {
        icvFillConvexPoly(img, v, count, color, line_type, XY_SHIFT);
    }
    else
    {
        CPolyEdge* edges;
        CRect rect = {0};
        img_t vtx[1] = {0};
        CC_CALL(edges = (CPolyEdge*)cAlloc(sizeof(CPolyEdge)*count));
        v[count++] = center;

        cvMat(vtx, count, 1, CC_32S, 2, v, CC_AUTOSTEP);
        CC_CALL(icvCollectPolyEdges(img, vtx, edges, &rect, color, line_type, XY_SHIFT, iPOINT(0,0)));
        CC_CALL(icvFillEdgeCollection(img, edges, count, rect, color));
        cFree(&edges);
    }

    __END__;

}


/****************************************************************************************\
*                                Polygons filling                                        * 
\****************************************************************************************/


/* filling convex polygon. v - array of vertices, ntps - number of points */
static void
icvFillConvexPoly(img_t* img, CPoint *v, int npts, const void* color, int line_type , int shift )
{
    struct
    {
        int idx, di;
        int x, dx, ye;
    }
    edge[2];

    int delta = shift ? 1 << (shift - 1) : 0;
    int i, y, imin = 0, left = 0, right = 1, x1, x2;
    int edges = npts;
    int xmin, xmax, ymin, ymax;
    uchar* ptr = img->tt.data;
    CSize size = cvGetMatSize(img);
    int pix_size = CC_ELEM_SIZE1(img);
    CPoint p0;
    int delta1, delta2;

    if(line_type < CC_AA)
        delta1 = delta2 = XY_ONE >> 1;
        //delta1 = 0, delta2 = XY_ONE - 1;
    else
        delta1 = XY_ONE - 1, delta2 = 0;

    p0 = v[npts - 1];
    p0.x <<= XY_SHIFT - shift;
    p0.y <<= XY_SHIFT - shift;

    assert(0 <= shift && shift <= XY_SHIFT);
    xmin = xmax = v[0].x;
    ymin = ymax = v[0].y;

    for(i = 0; i < npts; i++)
    {
        CPoint p = v[i];
        if(p.y < ymin)
        {
            ymin = p.y;
            imin = i;
        }

        ymax = MAX(ymax, p.y);
        xmax = MAX(xmax, p.x);
        xmin = MIN(xmin, p.x);

        p.x <<= XY_SHIFT - shift;
        p.y <<= XY_SHIFT - shift;

        if(line_type <= 8)
        {
            if(shift == 0)
            {
                CPoint pt0, pt1;
                pt0.x = p0.x >> XY_SHIFT;
                pt0.y = p0.y >> XY_SHIFT;
                pt1.x = p.x >> XY_SHIFT;
                pt1.y = p.y >> XY_SHIFT;
                icvLine(img, pt0, pt1, color, line_type);
            }
            else
                icvLine2(img, p0, p, color);
        }
        else
            icvLineAA(img, p0, p, color);
        p0 = p;
    }

    xmin = (xmin + delta) >> shift;
    xmax = (xmax + delta) >> shift;
    ymin = (ymin + delta) >> shift;
    ymax = (ymax + delta) >> shift;

    if(npts < 3 || xmax < 0 || ymax < 0 || xmin >= size.width || ymin >= size.height)
        return;

    ymax = MIN(ymax, size.height - 1);
    edge[0].idx = edge[1].idx = imin;

    edge[0].ye = edge[1].ye = y = ymin;
    edge[0].di = 1;
    edge[1].di = npts - 1;

    ptr += img->step*y;

    do
    {
        if(line_type < CC_AA || y < ymax || y == ymin)
        {
            for(i = 0; i < 2; i++)
            {
                if(y >= edge[i].ye)
                {
                    int idx = edge[i].idx, di = edge[i].di;
                    int xs = 0, xe, ye, ty = 0;

                    for(;;)
                    {
                        ty = (v[idx].y + delta) >> shift;
                        if(ty > y || edges == 0)
                            break;
                        xs = v[idx].x;
                        idx += di;
                        idx -= ((idx < npts) - 1) & npts;   /* idx -= idx >= npts ? npts : 0 */
                        edges--;
                    }

                    ye = ty;
                    xs <<= XY_SHIFT - shift;
                    xe = v[idx].x << (XY_SHIFT - shift);

                    /* no more edges */
                    if(y >= ye)
                        return;

                    edge[i].ye = ye;
                    edge[i].dx = ((xe - xs)*2 + (ye - y)) / (2 * (ye - y));
                    edge[i].x = xs;
                    edge[i].idx = idx;
                }
            }
        }

        if(edge[left].x > edge[right].x)
        {
            left ^= 1;
            right ^= 1;
        }

        x1 = edge[left].x;
        x2 = edge[right].x;

        if(y >= 0)
        {
            int xx1 = (x1 + delta1) >> XY_SHIFT;
            int xx2 = (x2 + delta2) >> XY_SHIFT;

            if(xx2 >= 0 && xx1 < size.width)
            {
                if(xx1 < 0)
                    xx1 = 0;
                if(xx2 >= size.width)
                    xx2 = size.width - 1;
                ICC_HLINE(ptr, xx1, xx2, color, pix_size);
            }
        }

        x1 += edge[left].dx;
        x2 += edge[right].dx;

        edge[left].x = x1;
        edge[right].x = x2;
        ptr += img->step;
    }
    while(++y <= ymax);
}


/* draws simple or filled circle */
static void
icvCircle(img_t* img, CPoint center, int radius, const void* color, int fill)
{
    CSize size = cvGetMatSize(img);
    int step = img->step;
    int pix_size = CC_ELEM_SIZE1(img);
    uchar* ptr = (uchar*)(img->tt.data);
    int err = 0, dx = radius, dy = 0, plus = 1, minus = (radius << 1) - 1;
    int inside = center.x >= radius && center.x < size.width - radius &&
        center.y >= radius && center.y < size.height - radius;

    #define ICC_PUT_POINT(ptr, x)     \
        CC_MEMCPY_CHAR(ptr + (x)*pix_size, color, pix_size);

    while(dx >= dy)
    {
        int mask;
        int y11 = center.y - dy, y12 = center.y + dy, y21 = center.y - dx, y22 = center.y + dx;
        int x11 = center.x - dx, x12 = center.x + dx, x21 = center.x - dy, x22 = center.x + dy;

        if(inside)
        {
            uchar *tptr0 = ptr + y11 * step;
            uchar *tptr1 = ptr + y12 * step;
            
            if(!fill)
            {
                ICC_PUT_POINT(tptr0, x11);
                ICC_PUT_POINT(tptr1, x11);
                ICC_PUT_POINT(tptr0, x12);
                ICC_PUT_POINT(tptr1, x12);
            }
            else
            {
                ICC_HLINE(tptr0, x11, x12, color, pix_size);
                ICC_HLINE(tptr1, x11, x12, color, pix_size);
            }

            tptr0 = ptr + y21 * step;
            tptr1 = ptr + y22 * step;

            if(!fill)
            {
                ICC_PUT_POINT(tptr0, x21);
                ICC_PUT_POINT(tptr1, x21);
                ICC_PUT_POINT(tptr0, x22);
                ICC_PUT_POINT(tptr1, x22);
            }
            else
            {
                ICC_HLINE(tptr0, x21, x22, color, pix_size);
                ICC_HLINE(tptr1, x21, x22, color, pix_size);
            }
        }
        else if(x11 < size.width && x12 >= 0 && y21 < size.height && y22 >= 0)
        {
            if(fill)
            {
                x11 = MAX(x11, 0);
                x12 = MIN(x12, size.width - 1);
            }
            
            if((unsigned)y11 < (unsigned)size.height)
            {
                uchar *tptr = ptr + y11 * step;

                if(!fill)
                {
                    if(x11 >= 0)
                        ICC_PUT_POINT(tptr, x11);
                    if(x12 < size.width)
                        ICC_PUT_POINT(tptr, x12);
                }
                else
                    ICC_HLINE(tptr, x11, x12, color, pix_size);
            }

            if((unsigned)y12 < (unsigned)size.height)
            {
                uchar *tptr = ptr + y12 * step;

                if(!fill)
                {
                    if(x11 >= 0)
                        ICC_PUT_POINT(tptr, x11);
                    if(x12 < size.width)
                        ICC_PUT_POINT(tptr, x12);
                }
                else
                    ICC_HLINE(tptr, x11, x12, color, pix_size);
            }

            if(x21 < size.width && x22 >= 0)
            {
                if(fill)
                {
                    x21 = MAX(x21, 0);
                    x22 = MIN(x22, size.width - 1);
                }

                if((unsigned)y21 < (unsigned)size.height)
                {
                    uchar *tptr = ptr + y21 * step;

                    if(!fill)
                    {
                        if(x21 >= 0)
                            ICC_PUT_POINT(tptr, x21);
                        if(x22 < size.width)
                            ICC_PUT_POINT(tptr, x22);
                    }
                    else
                        ICC_HLINE(tptr, x21, x22, color, pix_size);
                }

                if((unsigned)y22 < (unsigned)size.height)
                {
                    uchar *tptr = ptr + y22 * step;

                    if(!fill)
                    {
                        if(x21 >= 0)
                            ICC_PUT_POINT(tptr, x21);
                        if(x22 < size.width)
                            ICC_PUT_POINT(tptr, x22);
                    }
                    else
                        ICC_HLINE(tptr, x21, x22, color, pix_size);
                }
            }
        }
        dy++;
        err += plus;
        plus += 2;

        mask = (err <= 0) - 1;

        err -= minus & mask;
        dx += mask;
        minus -= mask & 2;
    }

    #undef  ICC_PUT_POINT
}


static void
icvThickLine(img_t* img, CPoint p0, CPoint p1, const void* color,
              int thickness , int line_type, int flags, int shift)
{
    static const double INV_XY_ONE = 1./XY_ONE;

    p0.x <<= XY_SHIFT - shift;
    p0.y <<= XY_SHIFT - shift;
    p1.x <<= XY_SHIFT - shift;
    p1.y <<= XY_SHIFT - shift;

    if(thickness <= 1)
    {
        if(line_type < CC_AA)
        {
            if(line_type == 1 || line_type == 4 || shift == 0)
            {
                p0.x = (p0.x + (XY_ONE>>1)) >> XY_SHIFT;
                p0.y = (p0.y + (XY_ONE>>1)) >> XY_SHIFT;
                p1.x = (p1.x + (XY_ONE>>1)) >> XY_SHIFT;
                p1.y = (p1.y + (XY_ONE>>1)) >> XY_SHIFT;
                icvLine(img, p0, p1, color, line_type);
            }
            else
                icvLine2(img, p0, p1, color);
        }
        else
            icvLineAA(img, p0, p1, color);
    }
    else
    {
        CPoint pt[4], dp = {0,0};
        double dx = (p0.x - p1.x)*INV_XY_ONE, dy = (p1.y - p0.y)*INV_XY_ONE;
        double r = dx * dx + dy * dy;
        int i;
        thickness <<= XY_SHIFT - 1;

        if(fabs(r) > DBL_EPSILON)
        {
            r = thickness * ccInvSqrt((float) r);
            dp.x = cRound(dy * r);
            dp.y = cRound(dx * r);
        }

        pt[0].x = p0.x + dp.x;
        pt[0].y = p0.y + dp.y;
        pt[1].x = p0.x - dp.x;
        pt[1].y = p0.y - dp.y;
        pt[2].x = p1.x - dp.x;
        pt[2].y = p1.y - dp.y;
        pt[3].x = p1.x + dp.x;
        pt[3].y = p1.y + dp.y;

        icvFillConvexPoly(img, pt, 4, color, line_type, XY_SHIFT);

        for(i = 0; i < 2; i++)
        {
            if(flags & (i+1))
            {
                if(line_type < CC_AA)
                {
                    CPoint center;
                    center.x = (p0.x + (XY_ONE>>1)) >> XY_SHIFT;
                    center.y = (p0.y + (XY_ONE>>1)) >> XY_SHIFT;
                    icvCircle(img, center, thickness >> XY_SHIFT, color, 1); 
                }
                else
                {
                    icvEllipseEx(img, p0, cSize(thickness, thickness),
                                  0, 0, 360, color, -1, line_type);
                }
            }
            p0 = p1;
        }
    }
}


static void
icvPolyLine(img_t* img, CPoint *v, int count, int is_closed,
             const void* color, int thickness ,
             int line_type , int shift )
{
    CC_FUNCNAME("icvPolyLine");

    __BEGIN__;
    
    if(count > 0)
    {
        int i = is_closed ? count - 1 : 0;
        int flags = 2 + !is_closed;
        CPoint p0;
        assert(0 <= shift && shift <= XY_SHIFT);
        assert(img && thickness >= 0); 
        assert(v && count >= 0);

        if(!v)
            CC_ERROR(CC_StsNullPtr, "");

        p0 = v[i];
        for(i = !is_closed; i < count; i++)
        {
            CPoint p = v[i];
            icvThickLine(img, p0, p, color, thickness, line_type, flags, shift);
            p0 = p;
            flags = 2;
        }
    }

    __END__;
}

/****************************************************************************************\
*                              External functions                                        *
\****************************************************************************************/

CC_IMPL CScalar cvColorToScalar(double packed_color, TypeId type, int cn)
{
    CScalar scalar;
    
    if(type == CC_8U)
    {
        int icolor = cRound(packed_color);
        if(cn > 1)
        {
            scalar.val[0] = icolor & 255;
            scalar.val[1] = (icolor >> 8) & 255;
            scalar.val[2] = (icolor >> 16) & 255;
            scalar.val[3] = (icolor >> 24) & 255;
        }
        else
        {
            scalar.val[0] = CC_CAST_8U(icolor);
            scalar.val[1] = scalar.val[2] = scalar.val[3] = 0;
        }
    }
    else if(type == CC_8S)
    {
        int icolor = cRound(packed_color);
        if(cn > 1)
        {
            scalar.val[0] = (char)icolor;
            scalar.val[1] = (char)(icolor >> 8);
            scalar.val[2] = (char)(icolor >> 16);
            scalar.val[3] = (char)(icolor >> 24);
        }
        else
        {
            scalar.val[0] = CC_CAST_8S(icolor);
            scalar.val[1] = scalar.val[2] = scalar.val[3] = 0;
        }
    }
    else
    {
        switch(cn)
        {
        case 1:
            scalar.val[0] = packed_color;
            scalar.val[1] = scalar.val[2] = scalar.val[3] = 0;
            break;
        case 2:
            scalar.val[0] = scalar.val[1] = packed_color;
            scalar.val[2] = scalar.val[3] = 0;
            break;
        case 3:
            scalar.val[0] = scalar.val[1] = scalar.val[2] = packed_color;
            scalar.val[3] = 0;
            break;
        default:
            scalar.val[0] = scalar.val[1] =
                scalar.val[2] = scalar.val[3] = packed_color;
            break;
        }
    }

    return scalar;
}


CC_IMPL void
cvLine(img_t* mat, CPoint pt1, CPoint pt2, CScalar color,
        int thickness CC_DEFAULT(1), int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0))
{
    int coi = 0;
    double buf[4];
    CC_FUNCNAME("cvLine");

    __BEGIN__;

    if(line_type == CC_AA && CC_MAT_DEPTH(mat) != CC_8U)
        line_type = 8;

    if(coi != 0)
        CC_ERROR(CC_BadCOI, cvUnsupportedFormat);

    if((unsigned)thickness > 255)
        CC_ERROR(CC_StsOutOfRange, "");

    if(shift < 0 || XY_SHIFT < shift)
        CC_ERROR(CC_StsOutOfRange, "shift must be between 0 and 16");

    CC_CALL(cvScalarToRawData(&color, buf, mat->tid, CC_MAT_CN(mat), 0));
    icvThickLine(mat, pt1, pt2, buf, thickness, line_type, 3, shift); 

    __END__;
}

CC_IMPL void
cvRectangle(img_t* mat, CPoint pt1, CPoint pt2,
             CScalar color, int thickness CC_DEFAULT(1),
             int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0))
{
    CPoint pt[4];
    double buf[4];
    int coi = 0;

    CC_FUNCNAME("cvRectangle");

    __BEGIN__;

    if(thickness < -1 || thickness > 255)
        CC_ERROR(CC_StsOutOfRange, "");

    if(line_type == CC_AA && CC_MAT_DEPTH(mat) != CC_8U)
        line_type = 8;

    if(coi != 0)
        CC_ERROR(CC_BadCOI, cvUnsupportedFormat);

    if(shift < 0 || XY_SHIFT < shift)
        CC_ERROR(CC_StsOutOfRange, "shift must be between 0 and 16");

    CC_CALL(cvScalarToRawData(&color, buf, mat->tid, CC_MAT_CN(mat), 0));

    pt[0] = pt1;
    pt[1].x = pt2.x;
    pt[1].y = pt1.y;
    pt[2] = pt2;
    pt[3].x = pt1.x;
    pt[3].y = pt2.y;

    if(thickness >= 0)
        icvPolyLine(mat, pt, 4, 1, buf, thickness, line_type, shift);
    else
        icvFillConvexPoly(mat, pt, 4, buf, line_type, shift);

    __END__;
}

CC_IMPL void
cvRectangle2(img_t* mat, CRect rect, CScalar color, int thickness CC_DEFAULT(1),
             int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0)) {
  cvRectangle(mat, cPoint(rect.x, rect.y), cPoint(rect.x+rect.w, rect.y+rect.h), color, thickness,
    line_type, shift);
}


CC_IMPL void
cvCircle(img_t* mat, CPoint center, int radius,
          CScalar color, int thickness CC_DEFAULT(1), int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0))
{
    int coi = 0;
    double buf[4];
    CC_FUNCNAME("cvCircle");

    __BEGIN__;
    if(line_type == CC_AA && CC_MAT_DEPTH(mat) != CC_8U)
        line_type = 8;

    if(coi != 0)
        CC_ERROR(CC_BadCOI, cvUnsupportedFormat);

    if(radius < 0)
        CC_ERROR(CC_StsOutOfRange, "");

    if(thickness < -1 || thickness > 255)
        CC_ERROR(CC_StsOutOfRange, "");

    if(shift < 0 || XY_SHIFT < shift)
        CC_ERROR(CC_StsOutOfRange, "shift must be between 0 and 16");

    CC_CALL(cvScalarToRawData(&color, buf, mat->tid, CC_MAT_CN(mat), 0));

    if(thickness > 1 || line_type >= CC_AA)
    {
        center.x <<= XY_SHIFT - shift;
        center.y <<= XY_SHIFT - shift;
        radius <<= XY_SHIFT - shift;
        icvEllipseEx(mat, center, cSize(radius, radius),
                      0, 0, 360, buf, thickness, line_type);
    }
    else
    {
        icvCircle(mat, center, radius, buf, thickness < 0);
    }

    __END__;
}


CC_IMPL void
cvEllipse(img_t* mat, CPoint center, CSize axes,
           double angle, double start_angle, double end_angle,
           CScalar color, int thickness CC_DEFAULT(1), int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0))
{
    int coi = 0;
    double buf[4];
    CC_FUNCNAME("cvEllipse");

    __BEGIN__;


    

    if(line_type == CC_AA && CC_MAT_DEPTH(mat) != CC_8U)
        line_type = 8;

    if(coi != 0)
        CC_ERROR(CC_BadCOI, cvUnsupportedFormat);

    if(axes.width < 0 || axes.height < 0)
        CC_ERROR(CC_StsOutOfRange, "");

    if(thickness < -1 || thickness > 255)
        CC_ERROR(CC_StsOutOfRange, "");

    if(shift < 0 || XY_SHIFT < shift)
        CC_ERROR(CC_StsOutOfRange, "shift must be between 0 and 16");

    CC_CALL(cvScalarToRawData(&color, buf, mat->tid, CC_MAT_CN(mat), 0));

    {
        int _angle = cRound(angle);
        int _start_angle = cRound(start_angle);
        int _end_angle = cRound(end_angle);
        center.x <<= XY_SHIFT - shift;
        center.y <<= XY_SHIFT - shift;
        axes.width <<= XY_SHIFT - shift;
        axes.height <<= XY_SHIFT - shift;

        CC_CALL(icvEllipseEx(mat, center, axes, _angle, _start_angle,
                               _end_angle, buf, thickness, line_type));
    }

    __END__;
}


CC_IMPL void
cvFillConvexPoly(img_t* mat, CPoint *pts, int npts, CScalar color, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0))
{
    int coi = 0;
    double buf[4];
    CC_FUNCNAME("cvFillConvexPoly");

    __BEGIN__;

    if(line_type == CC_AA && CC_MAT_DEPTH(mat) != CC_8U)
        line_type = 8;

    if(coi != 0)
        CC_ERROR(CC_BadCOI, cvUnsupportedFormat);

    if(!pts)
        CC_ERROR(CC_StsNullPtr, "");

    if(npts <= 0)
        CC_ERROR(CC_StsOutOfRange, "");

    if(shift < 0 || XY_SHIFT < shift)
        CC_ERROR(CC_StsOutOfRange, "shift must be between 0 and 16");

    CC_CALL(cvScalarToRawData(&color, buf, mat->tid, CC_MAT_CN(mat), 0));
    icvFillConvexPoly(mat, pts, npts, buf, line_type, shift);

    __END__;
}


CC_IMPL void
cvFillPoly(img_t* mat, CPoint **pts, int *npts, int contours,
            CScalar color, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0))
{
    double buf[4];
    CC_FUNCNAME("cvFillPoly");

    __BEGIN__;

    if(line_type == CC_AA && CC_MAT_DEPTH(mat) != CC_8U)
        line_type = 8;

    if(contours <= 0)
        CC_ERROR(CC_StsBadArg, "");

    if(!pts)
        CC_ERROR(CC_StsNullPtr, "");

    if(npts <= 0)
        CC_ERROR(CC_StsNullPtr, "");

    if(shift < 0 || XY_SHIFT < shift)
        CC_ERROR(CC_StsOutOfRange, "shift must be between 0 and 16");

    CC_CALL(cvScalarToRawData(&color, buf, mat->tid, CC_MAT_CN(mat), 0));

    {
        CPolyEdge* edges = 0;
        int i, j=0, count = 0;
        CRect rect = {0};
        for(i = 0; i < contours; i++)
        {
            if(!pts[i])
                CC_ERROR(CC_StsNullPtr, "");

            if(npts[i] < 0)
                CC_ERROR(CC_StsOutOfRange, "");
            
            count += npts[i];
        }

        CC_CALL(edges = (CPolyEdge*)cAlloc( sizeof(CPolyEdge) * count ));
        BZERO(edges, count);

        for(i = 0; i < contours; i++)
        {
            img_t vtx[1] = {0};
            cvMat(vtx, npts[i], 1, CC_32S, 2, pts[i], CC_AUTOSTEP);
            CC_CALL(icvCollectPolyEdges(mat, vtx, edges+j, &rect, buf, line_type, shift, iPOINT(0, 0)));
            j += npts[i];
        }

        CC_CALL(icvFillEdgeCollection(mat, edges, count, rect, buf));
        cFree(&edges);
    }

    __END__;
}



CC_IMPL void
cvPolyLine(img_t* mat, CPoint **pts, int *npts,
            int contours, int closed, CScalar color,
            int thickness CC_DEFAULT(1), int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0))
{
    int coi = 0, i;
    double buf[4];
    CC_FUNCNAME("cvPolyLine");

    __BEGIN__;

    if(line_type == CC_AA && CC_MAT_DEPTH(mat) != CC_8U)
        line_type = 8;

    if(coi != 0)
        CC_ERROR(CC_BadCOI, cvUnsupportedFormat);

    if(contours <= 0)
        CC_ERROR(CC_StsBadArg, "");

    if(thickness < -1 || thickness > 255)
        CC_ERROR(CC_StsBadArg, "");

    if(!pts)
        CC_ERROR(CC_StsNullPtr, "");

    if(npts <= 0)
        CC_ERROR(CC_StsNullPtr, "");

    if(shift < 0 || XY_SHIFT < shift)
        CC_ERROR(CC_StsOutOfRange, "shift must be between 0 and 16");

    CC_CALL(cvScalarToRawData(&color, buf, mat->tid, CC_MAT_CN(mat), 0));

    for(i = 0; i < contours; i++)
        icvPolyLine(mat, pts[i], npts[i], closed, buf, thickness, line_type, shift);

    __END__;
}


#define CC_FONT_SIZE_SHIFT     8
#define CC_FONT_ITALIC_ALPHA   (1 << 8)
#define CC_FONT_ITALIC_DIGIT   (2 << 8)
#define CC_FONT_ITALIC_PUNCT   (4 << 8)
#define CC_FONT_ITALIC_BRACES  (8 << 8)
#define CC_FONT_HAVE_GREEK     (16 << 8)
#define CC_FONT_HAVE_CYRILLIC  (32 << 8)

static const int icvHersheyPlain[] = {
(5 + 4*16) + CC_FONT_HAVE_GREEK,
199, 214, 217, 233, 219, 197, 234, 216, 221, 222, 228, 225, 211, 224, 210, 220,
200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 212, 213, 191, 226, 192,
215, 190, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 193, 84,
194, 85, 86, 87, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
195, 223, 196, 88 };

static const int icvHersheyPlainItalic[] = {
(5 + 4*16) + CC_FONT_ITALIC_ALPHA + CC_FONT_HAVE_GREEK,
199, 214, 217, 233, 219, 197, 234, 216, 221, 222, 228, 225, 211, 224, 210, 220,
200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 212, 213, 191, 226, 192,
215, 190, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 193, 84,
194, 85, 86, 87, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
195, 223, 196, 88 };

static const int icvHersheyComplexSmall[] = {
(6 + 7*16) + CC_FONT_HAVE_GREEK,
1199, 1214, 1217, 1275, 1274, 1271, 1272, 1216, 1221, 1222, 1219, 1232, 1211, 1231, 1210, 1220,
1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1209, 1212, 2213, 1241, 1238, 1242,
1215, 1273, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 1011, 1012, 1013,
1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023, 1024, 1025, 1026, 1223, 1084,
1224, 1247, 586, 1249, 1101, 1102, 1103, 1104, 1105, 1106, 1107, 1108, 1109, 1110, 1111,
1112, 1113, 1114, 1115, 1116, 1117, 1118, 1119, 1120, 1121, 1122, 1123, 1124, 1125, 1126,
1225, 1229, 1226, 1246 };

static const int icvHersheyComplexSmallItalic[] = {
(6 + 7*16) + CC_FONT_ITALIC_ALPHA + CC_FONT_HAVE_GREEK,
1199, 1214, 1217, 1275, 1274, 1271, 1272, 1216, 1221, 1222, 1219, 1232, 1211, 1231, 1210, 1220,
1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1209, 1212, 1213, 1241, 1238, 1242,
1215, 1273, 1051, 1052, 1053, 1054, 1055, 1056, 1057, 1058, 1059, 1060, 1061, 1062, 1063,
1064, 1065, 1066, 1067, 1068, 1069, 1070, 1071, 1072, 1073, 1074, 1075, 1076, 1223, 1084,
1224, 1247, 586, 1249, 1151, 1152, 1153, 1154, 1155, 1156, 1157, 1158, 1159, 1160, 1161,
1162, 1163, 1164, 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176,
1225, 1229, 1226, 1246 };

static const int icvHersheySimplex[] = {
(9 + 12*16) + CC_FONT_HAVE_GREEK,
2199, 714, 717, 733, 719, 697, 734, 716, 721, 722, 728, 725, 711, 724, 710, 720,
700, 701, 702, 703, 704, 705, 706, 707, 708, 709, 712, 713, 691, 726, 692,
715, 690, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513,
514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 693, 584,
694, 2247, 586, 2249, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611,
612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626,
695, 723, 696, 2246 };

static const int icvHersheyDuplex[] = {
(9 + 12*16) + CC_FONT_HAVE_GREEK,
2199, 2714, 2728, 2732, 2719, 2733, 2718, 2727, 2721, 2722, 2723, 2725, 2711, 2724, 2710, 2720,
2700, 2701, 2702, 2703, 2704, 2705, 2706, 2707, 2708, 2709, 2712, 2713, 2730, 2726, 2731,
2715, 2734, 2501, 2502, 2503, 2504, 2505, 2506, 2507, 2508, 2509, 2510, 2511, 2512, 2513,
2514, 2515, 2516, 2517, 2518, 2519, 2520, 2521, 2522, 2523, 2524, 2525, 2526, 2223, 2084,
2224, 2247, 587, 2249, 2601, 2602, 2603, 2604, 2605, 2606, 2607, 2608, 2609, 2610, 2611,
2612, 2613, 2614, 2615, 2616, 2617, 2618, 2619, 2620, 2621, 2622, 2623, 2624, 2625, 2626,
2225, 2229, 2226, 2246 };

static const int icvHersheyComplex[] = {
(9 + 12*16) + CC_FONT_HAVE_GREEK + CC_FONT_HAVE_CYRILLIC,
2199, 2214, 2217, 2275, 2274, 2271, 2272, 2216, 2221, 2222, 2219, 2232, 2211, 2231, 2210, 2220,
2200, 2201, 2202, 2203, 2204, 2205, 2206, 2207, 2208, 2209, 2212, 2213, 2241, 2238, 2242,
2215, 2273, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025, 2026, 2223, 2084,
2224, 2247, 587, 2249, 2101, 2102, 2103, 2104, 2105, 2106, 2107, 2108, 2109, 2110, 2111,
2112, 2113, 2114, 2115, 2116, 2117, 2118, 2119, 2120, 2121, 2122, 2123, 2124, 2125, 2126,
2225, 2229, 2226, 2246 };

static const int icvHersheyComplexItalic[] = {
(9 + 12*16) + CC_FONT_ITALIC_ALPHA + CC_FONT_ITALIC_DIGIT + CC_FONT_ITALIC_PUNCT +
CC_FONT_HAVE_GREEK + CC_FONT_HAVE_CYRILLIC,
2199, 2764, 2778, 2782, 2769, 2783, 2768, 2777, 2771, 2772, 2219, 2232, 2211, 2231, 2210, 2220,
2750, 2751, 2752, 2753, 2754, 2755, 2756, 2757, 2758, 2759, 2212, 2213, 2241, 2238, 2242,
2765, 2273, 2051, 2052, 2053, 2054, 2055, 2056, 2057, 2058, 2059, 2060, 2061, 2062, 2063,
2064, 2065, 2066, 2067, 2068, 2069, 2070, 2071, 2072, 2073, 2074, 2075, 2076, 2223, 2084,
2224, 2247, 587, 2249, 2151, 2152, 2153, 2154, 2155, 2156, 2157, 2158, 2159, 2160, 2161,
2162, 2163, 2164, 2165, 2166, 2167, 2168, 2169, 2170, 2171, 2172, 2173, 2174, 2175, 2176,
2225, 2229, 2226, 2246 };

static const int icvHersheyTriplex[] = {
(9 + 12*16) + CC_FONT_HAVE_GREEK,
2199, 3214, 3228, 3232, 3219, 3233, 3218, 3227, 3221, 3222, 3223, 3225, 3211, 3224, 3210, 3220,
3200, 3201, 3202, 3203, 3204, 3205, 3206, 3207, 3208, 3209, 3212, 3213, 3230, 3226, 3231,
3215, 3234, 3001, 3002, 3003, 3004, 3005, 3006, 3007, 3008, 3009, 3010, 3011, 3012, 3013,
2014, 3015, 3016, 3017, 3018, 3019, 3020, 3021, 3022, 3023, 3024, 3025, 3026, 2223, 2084,
2224, 2247, 587, 2249, 3101, 3102, 3103, 3104, 3105, 3106, 3107, 3108, 3109, 3110, 3111,
3112, 3113, 3114, 3115, 3116, 3117, 3118, 3119, 3120, 3121, 3122, 3123, 3124, 3125, 3126,
2225, 2229, 2226, 2246 };

static const int icvHersheyTriplexItalic[] = {
(9 + 12*16) + CC_FONT_ITALIC_ALPHA + CC_FONT_ITALIC_DIGIT +
CC_FONT_ITALIC_PUNCT + CC_FONT_HAVE_GREEK,
2199, 3264, 3278, 3282, 3269, 3233, 3268, 3277, 3271, 3272, 3223, 3225, 3261, 3224, 3260, 3270,
3250, 3251, 3252, 3253, 3254, 3255, 3256, 3257, 3258, 3259, 3262, 3263, 3230, 3226, 3231,
3265, 3234, 3051, 3052, 3053, 3054, 3055, 3056, 3057, 3058, 3059, 3060, 3061, 3062, 3063,
2064, 3065, 3066, 3067, 3068, 3069, 3070, 3071, 3072, 3073, 3074, 3075, 3076, 2223, 2084,
2224, 2247, 587, 2249, 3151, 3152, 3153, 3154, 3155, 3156, 3157, 3158, 3159, 3160, 3161,
3162, 3163, 3164, 3165, 3166, 3167, 3168, 3169, 3170, 3171, 3172, 3173, 3174, 3175, 3176,
2225, 2229, 2226, 2246 };

static const int icvHersheyScriptSimplex[] = {
(9 + 12*16) + CC_FONT_ITALIC_ALPHA + CC_FONT_HAVE_GREEK,
2199, 714, 717, 733, 719, 697, 734, 716, 721, 722, 728, 725, 711, 724, 710, 720,
700, 701, 702, 703, 704, 705, 706, 707, 708, 709, 712, 713, 691, 726, 692,
715, 690, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563,
564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 693, 584,
694, 2247, 586, 2249, 651, 652, 653, 654, 655, 656, 657, 658, 659, 660, 661,
662, 663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673, 674, 675, 676,
695, 723, 696, 2246 };

static const int icvHersheyScriptComplex[] = {
(9 + 12*16) + CC_FONT_ITALIC_ALPHA + CC_FONT_ITALIC_DIGIT + CC_FONT_ITALIC_PUNCT + CC_FONT_HAVE_GREEK,
2199, 2764, 2778, 2782, 2769, 2783, 2768, 2777, 2771, 2772, 2219, 2232, 2211, 2231, 2210, 2220,
2750, 2751, 2752, 2753, 2754, 2755, 2756, 2757, 2758, 2759, 2212, 2213, 2241, 2238, 2242,
2215, 2273, 2551, 2552, 2553, 2554, 2555, 2556, 2557, 2558, 2559, 2560, 2561, 2562, 2563,
2564, 2565, 2566, 2567, 2568, 2569, 2570, 2571, 2572, 2573, 2574, 2575, 2576, 2223, 2084,
2224, 2247, 586, 2249, 2651, 2652, 2653, 2654, 2655, 2656, 2657, 2658, 2659, 2660, 2661,
2662, 2663, 2664, 2665, 2666, 2667, 2668, 2669, 2670, 2671, 2672, 2673, 2674, 2675, 2676,
2225, 2229, 2226, 2246 };

/* Font structure */
typedef struct CFont
{
    int         font_face; /* =CC_FONT_* */
    const int*  ascii; /* font data and metrics */
    const int*  greek;
    const int*  cyrillic;
    float       hscale, vscale;
    float       shear; /* slope coefficient: 0 - normal, >0 - italic */
    int         thickness; /* letters thickness */
    float       dx; /* horizontal interval between letters */
    int         line_type;
}
CFont;

CC_IMPL void
cvInitFont(CFont *font, int font_face, double hscale, double vscale,
            double shear CC_DEFAULT(0), int thickness CC_DEFAULT(1), int line_type CC_DEFAULT(8))
{
    CC_FUNCNAME("cvInitFont");

    __BEGIN__;

    int is_italic = font_face & CC_FONT_ITALIC;

    if(!font)
        CC_ERROR(CC_StsNullPtr, "");

    if(hscale <= 0 || vscale <= 0 || thickness < 0)
        CC_ERROR(CC_StsOutOfRange, "");

    switch((font_face & 7))
    {
    case CC_FONT_HERSHEY_SIMPLEX:
        font->ascii = icvHersheySimplex;
        break;
    case CC_FONT_HERSHEY_PLAIN:
        font->ascii = !is_italic ? icvHersheyPlain : icvHersheyPlainItalic;
        break;
    case CC_FONT_HERSHEY_DUPLEX:
        font->ascii = icvHersheyDuplex;
        break;
    case CC_FONT_HERSHEY_COMPLEX:
        font->ascii = !is_italic ? icvHersheyComplex : icvHersheyComplexItalic;
        break;
    case CC_FONT_HERSHEY_TRIPLEX:
        font->ascii = !is_italic ? icvHersheyTriplex : icvHersheyTriplexItalic;
        break;
    case CC_FONT_HERSHEY_COMPLEX_SMALL:
        font->ascii = !is_italic ? icvHersheyComplexSmall : icvHersheyComplexSmallItalic;
        break;
    case CC_FONT_HERSHEY_SCRIPT_SIMPLEX:
        font->ascii = icvHersheyScriptSimplex;
        break;
    case CC_FONT_HERSHEY_SCRIPT_COMPLEX:
        font->ascii = icvHersheyScriptComplex;
        break;
    default:
        CC_ERROR(CC_StsOutOfRange, "Unknown font type");
    }

    font->font_face = font_face;
    font->hscale = (float)hscale;
    font->vscale = (float)vscale;
    font->thickness = thickness;
    font->shear = (float)shear;
    font->greek = font->cyrillic = 0;
    font->line_type = line_type;

    __END__;
}

CC_INLINE CFont cFont(double scale, int thickness CC_DEFAULT(1))
{
    CFont font;
    cvInitFont(&font, CC_FONT_HERSHEY_PLAIN, scale, scale, 0, thickness, CC_AA);
    return font;
}

#include "outtext.inl"

CC_IMPL void
cvPutText(img_t* mat, const char *text, CPoint org, const CFont *font, CScalar color)
{
    CC_FUNCNAME("cvPutText");

    __BEGIN__;

    int view_x, view_y;
    int coi = 0;
    int top_bottom = 0, base_line;
    int hscale, vscale, default_shear, italic_shear;
    int thickness CC_DEFAULT(2), line_type;
    
    double buf[4];
    CPoint pt[1 << 10];
    int count;
    CFont font_[1] = {0};

    int i;
    const char **faces = icvHersheyGlyphs;
    if (!font) {
      double scale = 4;
      font = font_;
      cvInitFont(font_, CC_FONT_HERSHEY_PLAIN, scale, scale, 0, thickness, CC_AA);
    }

    if(coi != 0)
        CC_ERROR(CC_BadCOI, cvUnsupportedFormat);

    if(!text || !font || !font->ascii)
        CC_ERROR(CC_StsNullPtr, "");

    CC_CALL(cvScalarToRawData(&color, buf, mat->tid, CC_MAT_CN(mat), 0));
    base_line = -(font->ascii[0] & 15);
    hscale = cRound(font->hscale*XY_ONE);
    vscale = cRound(font->vscale*XY_ONE);
    default_shear = cRound(font->shear*font->vscale*XY_ONE);
    italic_shear = !(font->font_face & CC_FONT_ITALIC) ? 0 : cRound(font->vscale*.25*XY_ONE);
    thickness = font->thickness;
    line_type = font->line_type;

    if(line_type == CC_AA && CC_MAT_DEPTH(mat) != CC_8U)
        line_type = 8;

    if(top_bottom)
        vscale = -vscale;

    view_x = org.x << XY_SHIFT;
    view_y = (org.y << XY_SHIFT) + base_line*vscale;

    for(i = 0; text[i] != '\0'; i++)
    {
        int c = (uchar)text[i];
        int dx, shear = default_shear;
        const char* ptr;
        CPoint p;

        if(c > 128 || c < ' ')
            c = '?';

        if(italic_shear)
        {
            if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
            {
                if(!(font->ascii[0] & CC_FONT_ITALIC_ALPHA))
                    shear += italic_shear;
            }
            else if('0' <= c && c <= '9')
            {
                if(!(font->ascii[0] & CC_FONT_ITALIC_DIGIT))
                    shear += italic_shear;
            }
            else if(c < 'A')
            {
                if(!(font->ascii[0] & CC_FONT_ITALIC_PUNCT))
                    shear += italic_shear;
            }
            else
            {
                shear += italic_shear;
            }
        }

        ptr = faces[font->ascii[(c-' ')+1]];
        p.x = (unsigned char)ptr[0] - 'R';
        p.y = (unsigned char)ptr[1] - 'R';
        dx = p.y*hscale;
        view_x -= p.x*hscale;
        count = 0;

        for(ptr += 2;;)
        {
            if(*ptr == ' ' || !*ptr)
            {
                if(count > 1)
                    icvPolyLine(mat, pt, count, 0, buf, thickness, line_type, XY_SHIFT); 
                if(!*ptr++)
                    break;
                count = 0;
            }
            else
            {
                p.x = (unsigned char)ptr[0] - 'R';
                p.y = (unsigned char)ptr[1] - 'R';
                ptr += 2;
                pt[count].x = p.x*hscale - p.y*shear + view_x;
                pt[count++].y = p.y*vscale + view_y;
            }
        }
        view_x += dx;
    }

    __END__;
}

CC_IMPL void
cvGetTextSize(const char *text, const CFont *font, CSize *size, int *_base_line)
{
    CC_FUNCNAME("cvGetTextSize");

    __BEGIN__;
    
    float view_x = 0;
    int base_line, cap_line;

    int i;
    const char **faces = icvHersheyGlyphs;

    if(!text || !font || !font->ascii || !size)
        CC_ERROR(CC_StsNullPtr, "");

    base_line = (font->ascii[0] & 15);
    cap_line = (font->ascii[0] >> 4) & 15;
    if(_base_line)
        *_base_line = cRound(base_line*font->vscale);
    size->height = cRound((cap_line + base_line)*font->vscale + font->thickness);

    for(i = 0; text[i] != '\0'; i++)
    {
        int c = (uchar)text[i];
        const char* ptr;
        CPoint p;

        if(c > 128 || c < ' ')
            c = '?';

        ptr = faces[font->ascii[(c-' ')+1]];
        p.x = (unsigned char)ptr[0] - 'R';
        p.y = (unsigned char)ptr[1] - 'R';
        view_x += (p.y - p.x)*font->hscale;
    }

    size->width = cRound(view_x + font->thickness);

    __END__;
}

//CSize getTextSize(const char *text, ) {  CFont font = {0};  cvInitFont(&font, CC_FONT_HERSHEY_PLAIN, scale, scale, 0, thickness, CC_AA);  cvGetTextSize(text, &font, );}

//static const CPoint icvCodeDeltas[8] = { {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1} };

#define CC_ADJUST_EDGE_COUNT(count, seq)  \
    ((count) -= ((count) == (seq)->total && !CC_IS_SEQ_CLOSED(seq)))

#if 0
CC_IMPL void
cvDrawContours(void* img, CvSeq*  contour,
                CScalar externalColor, CScalar holeColor, 
                int  maxLevel, int thickness CC_DEFAULT(1),
                int line_type, CPoint offset)
{
    CvSeq *contour0 = contour, *h_next = 0;
    CvMemStorage* st = 0;
    CvSeq* tseq = 0;
    CvContour* edges = 0;
    CvSeqWriter writer;
    CvTreeNodeIterator iterator1;

    CC_FUNCNAME("cvDrawContours");

    __BEGIN__;

    int coi = 0;
    
    double ext_buf[4], hole_buf[4];

    

    if(line_type == CC_AA && CC_MAT_DEPTH(mat) != CC_8U)
        line_type = 8;

    if(!contour)
        EXIT;

    if(coi != 0)
        CC_ERROR(CC_BadCOI, cvUnsupportedFormat);

    if(thickness < -1 || thickness > 255)
        CC_ERROR(CC_StsOutOfRange, "");

    CC_CALL(cvScalarToRawData(&externalColor, ext_buf, mat->tid, 0));
    CC_CALL(cvScalarToRawData(&holeColor, hole_buf, mat->tid, 0));

    if(maxLevel < 0)
    {
        h_next = contour->h_next;
        contour->h_next = 0;
        maxLevel = -maxLevel+1;
    }

    if(thickness < 0)
    {
        if(contour->storage)
            st = cvCreateChildMemStorage(contour->storage);
        else
            st = cvCreateMemStorage(CC_DRAWING_STORAGE_BLOCK);
        tseq = cvCreateSeq(0, sizeof(CvContour), sizeof(CPoint), st);
        edges = (CvContour*)cvCreateSeq(0, sizeof(CvContour), sizeof(CPolyEdge), st);
    }

    memset(&writer, 0, sizeof(writer));

    cvInitTreeNodeIterator(&iterator1, contour, maxLevel);
    
    while((contour = (CvSeq*)cvNextTreeNode(&iterator1)) != 0)
    {
        CvSeqReader reader;
        int i, count = contour->total;
        int elem_type = CC_MAT_DEPTH(contour->flags);
        void* clr = (contour->flags & CC_SEQ_FLAG_HOLE) == 0 ? ext_buf : hole_buf;

        cvStartReadSeq(contour, &reader, 0);

        if(CC_IS_SEQ_CHAIN_CONTOUR(contour))
        {
            CPoint pt = ((CChain*)contour)->origin;
            CPoint prev_pt = pt;
            char prev_code = reader.ptr ? reader.ptr[0] : '\0';

            if(thickness < 0)
            {
                cvClearSeq(tseq);
                cvStartAppendToSeq(tseq, &writer);
                CC_WRITE_SEQ_ELEM(pt, writer);
            }

            prev_pt.x += offset.x;
            prev_pt.y += offset.y;

            for(i = 0; i < count; i++)
            {
                char code;
                CC_READ_SEQ_ELEM(code, reader);

                assert((code & ~7) == 0);

                if(code != prev_code)
                {
                    prev_code = code;
                    if(thickness >= 0)
                    {
                        icvThickLine(mat, prev_pt, pt, clr, thickness, line_type, 2, 0);
                    }
                    else
                    {
                        CC_WRITE_SEQ_ELEM(pt, writer);
                    }
                    prev_pt = pt;
                }
            
                pt.x += icvCodeDeltas[(int)code].x;
                pt.y += icvCodeDeltas[(int)code].y;
            }

            if(thickness >= 0)
            {
                icvThickLine(mat, prev_pt, ((CChain*)contour)->origin,
                              clr, thickness, line_type, 2, 0);
            }
            else
            {
                CC_WRITE_SEQ_ELEM(pt, writer);
                cvEndWriteSeq(&writer);
                CC_CALL(icvCollectPolyEdges(mat, tseq, edges, ext_buf, line_type, 0));
            }
        }
        else if(CC_IS_SEQ_POLYLINE(contour))
        {
            if(thickness >= 0)
            {
                CPoint pt1, pt2;
                int shift = 0;
                
                count -= !CC_IS_SEQ_CLOSED(contour);
                if(elem_type == CC_32SC2)
                {
                    CC_READ_SEQ_ELEM(pt1, reader);
                    pt1.x += offset.x;
                    pt1.y += offset.y;
                }
                else
                {
                    CPoint2D32f pt1f;
                    CC_READ_SEQ_ELEM(pt1f, reader);
                    pt1.x = cRound((pt1f.x + offset.x) * XY_ONE);
                    pt1.y = cRound((pt1f.y + offset.y) * XY_ONE);
                    shift = XY_SHIFT;
                }

                for(i = 0; i < count; i++)
                {
                    if(elem_type == CC_32SC2)
                    {
                        CC_READ_SEQ_ELEM(pt2, reader);
                        pt2.x += offset.x;
                        pt2.y += offset.y;
                    }
                    else
                    {
                        CPoint2D32f pt2f;
                        CC_READ_SEQ_ELEM(pt2f, reader);
                        pt2.x = cRound(pt2f.x * XY_ONE);
                        pt2.y = cRound(pt2f.y * XY_ONE);
                    }
                    icvThickLine(mat, pt1, pt2, clr, thickness, line_type, 2, shift);
                    pt1 = pt2;
                }
            }
            else
            {
                CC_CALL(icvCollectPolyEdges(mat, contour, edges, ext_buf, line_type, 0, offset));
            }
        }
    }

    if(thickness < 0)
    {
        CC_CALL(icvFillEdgeCollection(mat, edges, ext_buf));
    }

    __END__;

    if(h_next && contour0)
        contour0->h_next = h_next;

    cvReleaseMemStorage(&st);
}

#endif



#endif // _DRAWING_INL_
