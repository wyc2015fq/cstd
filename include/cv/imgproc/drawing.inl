
#ifndef _DRAWING_INL_
#define _DRAWING_INL_

#include "../core/saturate.hpp"
static const int MAX_THICKNESS = 32767;

struct CPolyEdge
{
    //CPolyEdge() : y0(0), y1(0), x(0), dx(0), next(0) {}
    //CPolyEdge(int _y0, int _y1, int _x, int _dx) : y0(_y0), y1(_y1), x(_x), dx(_dx) {}

    int y0, y1;
    int x, dx;
    CPolyEdge *next;
};

static int
CollectPolyEdges( img_t* img, const CPoint* v, int npts, CPolyEdge* edges, const void* color, int line_type,
                  int shift, CPoint offset=cPoint(0, 0) );

static void
FillEdgeCollection( img_t* img, CPolyEdge* edges, int max_edges_count, const void* color );

static void
PolyLine( img_t* img, const CPoint* v, int npts, bool closed,
          const void* color, int thickness, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) );

static void
FillConvexPoly( img_t* img, const CPoint* v, int npts,
                const void* color, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) );

/****************************************************************************************\
*                                   Lines                                                *
\****************************************************************************************/

static bool clipLine( int width, int height, CPoint& pt1, CPoint& pt2 )
{
    CC_INSTRUMENT_REGION()

    int c1, c2;
    int right = width-1, bottom = height-1;

    if( width <= 0 || height <= 0 )
        return false;

    int &x1 = pt1.x, &y1 = pt1.y, &x2 = pt2.x, &y2 = pt2.y;
    c1 = (x1 < 0) + (x1 > right) * 2 + (y1 < 0) * 4 + (y1 > bottom) * 8;
    c2 = (x2 < 0) + (x2 > right) * 2 + (y2 < 0) * 4 + (y2 > bottom) * 8;

    if( (c1 & c2) == 0 && (c1 | c2) != 0 )
    {
        int a;
        if( c1 & 12 )
        {
            a = c1 < 8 ? 0 : bottom;
            x1 +=  (a - y1) * (x2 - x1) / (y2 - y1);
            y1 = a;
            c1 = (x1 < 0) + (x1 > right) * 2;
        }
        if( c2 & 12 )
        {
            a = c2 < 8 ? 0 : bottom;
            x2 += (a - y2) * (x2 - x1) / (y2 - y1);
            y2 = a;
            c2 = (x2 < 0) + (x2 > right) * 2;
        }
        if( (c1 & c2) == 0 && (c1 | c2) != 0 )
        {
            if( c1 )
            {
                a = c1 == 1 ? 0 : right;
                y1 += (a - x1) * (y2 - y1) / (x2 - x1);
                x1 = a;
                c1 = 0;
            }
            if( c2 )
            {
                a = c2 == 1 ? 0 : right;
                y2 += (a - x2) * (y2 - y1) / (x2 - x1);
                x2 = a;
                c2 = 0;
            }
        }

        assert( (c1 & c2) != 0 || (x1 | y1 | x2 | y2) >= 0 );
    }

    return (c1 | c2) == 0;
}

static bool clipLine( int x, int y, int width, int height, CPoint& pt1, CPoint& pt2 )
{
    pt1.x -= x;
    pt1.y -= y;
    pt2.x -= x;
    pt2.y -= y;
    bool inside = clipLine(width, height, pt1, pt2);
    pt1.x += x;
    pt1.y += y;
    pt2.x += x;
    pt2.y += y;

    return inside;
}

struct LineIterator
{
public:
    /** @brief intializes the iterator

    creates iterators for the line connecting pt1 and pt2
    the line will be clipped on the image boundaries
    the line is 8-connected or 4-connected
    If leftToRight=true, then the iteration is always done
    from the left-most point to the right most,
    not to depend on the ordering of pt1 and pt2 parameters
    */
    LineIterator( const img_t* img, CPoint pt1, CPoint pt2,
                  int connectivity = 8, bool leftToRight = false );
    /** @brief returns pointer to the current pixel
    */
    inline
    uchar* operator *()
    {
        return ptr;
    }

    /** @brief prefix increment operator (++it). shifts iterator to the next pixel
    */
    LineIterator& operator ++()
    {
        int mask = err < 0 ? -1 : 0;
        err += minusDelta + (plusDelta & mask);
        ptr += minusStep + (plusStep & mask);
        return *this;
    }
    /** @brief postfix increment operator (it++). shifts iterator to the next pixel
    */
    LineIterator operator ++(int)
    {
        LineIterator it = *this;
        ++(*this);
        return it;
    }
    /** @brief returns coordinates of the current pixel
    */
    CPoint pos() const;

    uchar* ptr;
    const uchar* ptr0;
    int step, elemSize;
    int err, count;
    int minusDelta, plusDelta;
    int minusStep, plusStep;
};

/*
   Initializes line iterator.
   Returns number of points on the line or negative number if error.
*/
LineIterator::LineIterator(const img_t* img, CPoint pt1, CPoint pt2,
                           int connectivity, bool left_to_right)
{
    count = -1;

    CC_Assert( connectivity == 8 || connectivity == 4 );

    if( (unsigned)pt1.x >= (unsigned)(img->cols) ||
        (unsigned)pt2.x >= (unsigned)(img->cols) ||
        (unsigned)pt1.y >= (unsigned)(img->rows) ||
        (unsigned)pt2.y >= (unsigned)(img->rows) )
    {
        if( !clipLine( img->width, img->height, pt1, pt2 ) )
        {
            ptr = img->data;
            err = plusDelta = minusDelta = plusStep = minusStep = count = 0;
            return;
        }
    }

    int bt_pix0 = (int)(img->c), bt_pix = bt_pix0;
    size_t istep = img->step;

    int dx = pt2.x - pt1.x;
    int dy = pt2.y - pt1.y;
    int s = dx < 0 ? -1 : 0;

    if( left_to_right )
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

    ptr = (uchar*)(img->data + pt1.y * istep + pt1.x * bt_pix0);

    s = dy < 0 ? -1 : 0;
    dy = (dy ^ s) - s;
    istep = (istep ^ s) - s;

    s = dy > dx ? -1 : 0;

    /* conditional swaps */
    dx ^= dy & s;
    dy ^= dx & s;
    dx ^= dy & s;

    bt_pix ^= istep & s;
    istep ^= bt_pix & s;
    bt_pix ^= istep & s;

    if( connectivity == 8 )
    {
        assert( dx >= 0 && dy >= 0 );

        err = dx - (dy + dy);
        plusDelta = dx + dx;
        minusDelta = -(dy + dy);
        plusStep = (int)istep;
        minusStep = bt_pix;
        count = dx + 1;
    }
    else /* connectivity == 4 */
    {
        assert( dx >= 0 && dy >= 0 );

        err = 0;
        plusDelta = (dx + dx) + (dy + dy);
        minusDelta = -(dy + dy);
        plusStep = (int)istep - bt_pix;
        minusStep = bt_pix;
        count = dx + dy + 1;
    }

    this->ptr0 = img->tt.data;
    this->step = (int)img->step;
    this->elemSize = bt_pix0;
}

static void
Line( img_t* img, CPoint pt1, CPoint pt2,
      const void* _color, int connectivity = 8 )
{
    if( connectivity == 0 )
        connectivity = 8;
    else if( connectivity == 1 )
        connectivity = 4;

    LineIterator iterator(img, pt1, pt2, connectivity, true);
    int i, count = iterator.count;
    int pix_size = img->c;
    const uchar* color = (const uchar*)_color;

    for( i = 0; i < count; i++, ++iterator )
    {
        uchar* ptr = *iterator;
        if( pix_size == 1 )
            ptr[0] = color[0];
        else if( pix_size == 3 )
        {
            ptr[0] = color[0];
            ptr[1] = color[1];
            ptr[2] = color[2];
        }
        else
            memcpy( *iterator, color, pix_size );
    }
}

/* Correction table depent on the slope */
static const uchar SlopeCorrTable[] = {
    181, 181, 181, 182, 182, 183, 184, 185, 187, 188, 190, 192, 194, 196, 198, 201,
    203, 206, 209, 211, 214, 218, 221, 224, 227, 231, 235, 238, 242, 246, 250, 254
};

/* Gaussian for antialiasing filter */
static const int FilterTable[] = {
    168, 177, 185, 194, 202, 210, 218, 224, 231, 236, 241, 246, 249, 252, 254, 254,
    254, 254, 252, 249, 246, 241, 236, 231, 224, 218, 210, 202, 194, 185, 177, 168,
    158, 149, 140, 131, 122, 114, 105, 97, 89, 82, 75, 68, 62, 56, 50, 45,
    40, 36, 32, 28, 25, 22, 19, 16, 14, 12, 11, 9, 8, 7, 5, 5
};

static void
LineAA( img_t* img, CPoint pt1, CPoint pt2, const void* color )
{
    int dx, dy;
    int ecount, scount = 0;
    int slope;
    int ax, ay;
    int x_step, y_step;
    int i, j;
    int ep_table[9];
    int cb = ((uchar*)color)[0], cg = ((uchar*)color)[1], cr = ((uchar*)color)[2], ca = ((uchar*)color)[3];
    int _cb, _cg, _cr, _ca;
    int nch = CC_MAT_CN(img);
    uchar* ptr = img->tt.data;
    size_t step = img->step;
    int width = img->w, height = img->height;

    if( !((nch == 1 || nch == 3 || nch == 4) && CC_MAT_DEPTH(img) == CC_8U) )
    {
        Line(img, cPoint((int)(pt1.x<<XY_SHIFT), (int)(pt1.y<<XY_SHIFT)), cPoint((int)(pt2.x<<XY_SHIFT), (int)(pt2.y<<XY_SHIFT)), color);
        return;
    }

    pt1.x -= XY_ONE*2;
    pt1.y -= XY_ONE*2;
    pt2.x -= XY_ONE*2;
    pt2.y -= XY_ONE*2;
    ptr += img->step*2 + 2*nch;

    width = ((width - 5) << XY_SHIFT) + 1;
    height = ((height - 5) << XY_SHIFT) + 1;

    if( !clipLine( width, height, pt1, pt2 ))
        return;

    dx = pt2.x - pt1.x;
    dy = pt2.y - pt1.y;

    j = dx < 0 ? -1 : 0;
    ax = (dx ^ j) - j;
    i = dy < 0 ? -1 : 0;
    ay = (dy ^ i) - i;

    if( ax > ay )
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
        y_step = (dy << XY_SHIFT) / (ax | 1);
        pt2.x += XY_ONE;
        ecount = (int)((pt2.x >> XY_SHIFT) - (pt1.x >> XY_SHIFT));
        j = -(pt1.x & (XY_ONE - 1));
        pt1.y += ((y_step * j) >> XY_SHIFT) + (XY_ONE >> 1);
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

        x_step = (dx << XY_SHIFT) / (ay | 1);
        y_step = XY_ONE;
        pt2.y += XY_ONE;
        ecount = (int)((pt2.y >> XY_SHIFT) - (pt1.y >> XY_SHIFT));
        j = -(pt1.y & (XY_ONE - 1));
        pt1.x += ((x_step * j) >> XY_SHIFT) + (XY_ONE >> 1);
        slope = (x_step >> (XY_SHIFT - 5)) & 0x3f;
        slope ^= (x_step < 0 ? 0x3f : 0);

        /* Get 4-bit fractions for end-point adjustments */
        i = (pt1.y >> (XY_SHIFT - 7)) & 0x78;
        j = (pt2.y >> (XY_SHIFT - 7)) & 0x78;
    }

    slope = (slope & 0x20) ? 0x100 : SlopeCorrTable[slope];

    /* Calc end point correction table */
    {
        int t0 = slope << 7;
        int t1 = ((0x78 - (int)i) | 4) * slope;
        int t2 = ((int)j | 4) * slope;

        ep_table[0] = 0;
        ep_table[8] = slope;
        ep_table[1] = ep_table[3] = ((((j - i) & 0x78) | 4) * slope >> 8) & 0x1ff;
        ep_table[2] = (t1 >> 8) & 0x1ff;
        ep_table[4] = ((((j - i) + 0x80) | 4) * slope >> 8) & 0x1ff;
        ep_table[5] = ((t1 + t0) >> 8) & 0x1ff;
        ep_table[6] = (t2 >> 8) & 0x1ff;
        ep_table[7] = ((t2 + t0) >> 8) & 0x1ff;
    }

    if( nch == 3 )
    {
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
        if( ax > ay )
        {
            ptr += (pt1.x >> XY_SHIFT) * 3;

            while( ecount >= 0 )
            {
                uchar *tptr = ptr + ((pt1.y >> XY_SHIFT) - 1) * step;

                int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 +
                                       (((ecount >= 2) + 1) & (ecount | 2))];
                int a, dist = (pt1.y >> (XY_SHIFT - 5)) & 31;

                a = (ep_corr * FilterTable[dist + 32] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += step;
                a = (ep_corr * FilterTable[dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += step;
                a = (ep_corr * FilterTable[63 - dist] >> 8) & 0xff;
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

            while( ecount >= 0 )
            {
                uchar *tptr = ptr + ((pt1.x >> XY_SHIFT) - 1) * 3;

                int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 +
                                       (((ecount >= 2) + 1) & (ecount | 2))];
                int a, dist = (pt1.x >> (XY_SHIFT - 5)) & 31;

                a = (ep_corr * FilterTable[dist + 32] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += 3;
                a = (ep_corr * FilterTable[dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += 3;
                a = (ep_corr * FilterTable[63 - dist] >> 8) & 0xff;
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
    else if(nch == 1)
    {
        #define  ICC_PUT_POINT()            \
        {                                   \
            _cb = tptr[0];                  \
            _cb += ((cb - _cb)*a + 127)>> 8;\
            tptr[0] = (uchar)_cb;           \
        }

        if( ax > ay )
        {
            ptr += (pt1.x >> XY_SHIFT);

            while( ecount >= 0 )
            {
                uchar *tptr = ptr + ((pt1.y >> XY_SHIFT) - 1) * step;

                int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 +
                                       (((ecount >= 2) + 1) & (ecount | 2))];
                int a, dist = (pt1.y >> (XY_SHIFT - 5)) & 31;

                a = (ep_corr * FilterTable[dist + 32] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += step;
                a = (ep_corr * FilterTable[dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += step;
                a = (ep_corr * FilterTable[63 - dist] >> 8) & 0xff;
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

            while( ecount >= 0 )
            {
                uchar *tptr = ptr + ((pt1.x >> XY_SHIFT) - 1);

                int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 +
                                       (((ecount >= 2) + 1) & (ecount | 2))];
                int a, dist = (pt1.x >> (XY_SHIFT - 5)) & 31;

                a = (ep_corr * FilterTable[dist + 32] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr++;
                a = (ep_corr * FilterTable[dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr++;
                a = (ep_corr * FilterTable[63 - dist] >> 8) & 0xff;
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
            _cg = tptr[1];                  \
            _cg += ((cg - _cg)*a + 127)>> 8;\
            _cr = tptr[2];                  \
            _cr += ((cr - _cr)*a + 127)>> 8;\
            _ca = tptr[3];                  \
            _ca += ((ca - _ca)*a + 127)>> 8;\
            tptr[0] = (uchar)_cb;           \
            tptr[1] = (uchar)_cg;           \
            tptr[2] = (uchar)_cr;           \
            tptr[3] = (uchar)_ca;           \
        }
        if( ax > ay )
        {
            ptr += (pt1.x >> XY_SHIFT) * 4;

            while( ecount >= 0 )
            {
                uchar *tptr = ptr + ((pt1.y >> XY_SHIFT) - 1) * step;

                int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 +
                                       (((ecount >= 2) + 1) & (ecount | 2))];
                int a, dist = (pt1.y >> (XY_SHIFT - 5)) & 31;

                a = (ep_corr * FilterTable[dist + 32] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += step;
                a = (ep_corr * FilterTable[dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += step;
                a = (ep_corr * FilterTable[63 - dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                pt1.y += y_step;
                ptr += 4;
                scount++;
                ecount--;
            }
        }
        else
        {
            ptr += (pt1.y >> XY_SHIFT) * step;

            while( ecount >= 0 )
            {
                uchar *tptr = ptr + ((pt1.x >> XY_SHIFT) - 1) * 4;

                int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 +
                                       (((ecount >= 2) + 1) & (ecount | 2))];
                int a, dist = (pt1.x >> (XY_SHIFT - 5)) & 31;

                a = (ep_corr * FilterTable[dist + 32] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += 4;
                a = (ep_corr * FilterTable[dist] >> 8) & 0xff;
                ICC_PUT_POINT();
                ICC_PUT_POINT();

                tptr += 4;
                a = (ep_corr * FilterTable[63 - dist] >> 8) & 0xff;
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
Line2( img_t* img, CPoint pt1, CPoint pt2, const void* color)
{
    int dx, dy;
    int ecount;
    int ax, ay;
    int i, j;
    int x, y;
    int x_step, y_step;
    int cb = ((uchar*)color)[0];
    int cg = ((uchar*)color)[1];
    int cr = ((uchar*)color)[2];
    int pix_size = CC_ELEM_SIZE_(img);
    uchar *ptr = img->tt.data, *tptr;
    size_t step = img->step;
    int width = img->width;
    int height = img->height;

    //assert( img && (nch == 1 || nch == 3) && CC_MAT_DEPTH(img) == CC_8U );

    int sizeScaled_width = ((int)width) << XY_SHIFT;
    int sizeScaled_height = ((int)height) << XY_SHIFT;
    if( !clipLine( sizeScaled_width, sizeScaled_height, pt1, pt2 ))
        return;

    dx = pt2.x - pt1.x;
    dy = pt2.y - pt1.y;

    j = dx < 0 ? -1 : 0;
    ax = (dx ^ j) - j;
    i = dy < 0 ? -1 : 0;
    ay = (dy ^ i) - i;

    if( ax > ay )
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
        y_step = (dy << XY_SHIFT) / (ax | 1);
        ecount = (int)((pt2.x - pt1.x) >> XY_SHIFT);
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

        x_step = (dx << XY_SHIFT) / (ay | 1);
        y_step = XY_ONE;
        ecount = (int)((pt2.y - pt1.y) >> XY_SHIFT);
    }

    pt1.x += (XY_ONE >> 1);
    pt1.y += (XY_ONE >> 1);

    if( pix_size == 3 )
    {
        #define  ICC_PUT_POINT(_x,_y)   \
        x = (_x); y = (_y);             \
        if( 0 <= x && x < width && \
            0 <= y && y < height ) \
        {                               \
            tptr = ptr + y*step + x*3;  \
            tptr[0] = (uchar)cb;        \
            tptr[1] = (uchar)cg;        \
            tptr[2] = (uchar)cr;        \
        }

        ICC_PUT_POINT((int)((pt2.x + (XY_ONE >> 1)) >> XY_SHIFT),
                      (int)((pt2.y + (XY_ONE >> 1)) >> XY_SHIFT));

        if( ax > ay )
        {
            pt1.x >>= XY_SHIFT;

            while( ecount >= 0 )
            {
                ICC_PUT_POINT((int)(pt1.x), (int)(pt1.y >> XY_SHIFT));
                pt1.x++;
                pt1.y += y_step;
                ecount--;
            }
        }
        else
        {
            pt1.y >>= XY_SHIFT;

            while( ecount >= 0 )
            {
                ICC_PUT_POINT((int)(pt1.x >> XY_SHIFT), (int)(pt1.y));
                pt1.x += x_step;
                pt1.y++;
                ecount--;
            }
        }

        #undef ICC_PUT_POINT
    }
    else if( pix_size == 1 )
    {
        #define  ICC_PUT_POINT(_x,_y) \
        x = (_x); y = (_y);           \
        if( 0 <= x && x < width && \
            0 <= y && y < height ) \
        {                           \
            tptr = ptr + y*step + x;\
            tptr[0] = (uchar)cb;    \
        }

        ICC_PUT_POINT((int)((pt2.x + (XY_ONE >> 1)) >> XY_SHIFT),
                      (int)((pt2.y + (XY_ONE >> 1)) >> XY_SHIFT));

        if( ax > ay )
        {
            pt1.x >>= XY_SHIFT;

            while( ecount >= 0 )
            {
                ICC_PUT_POINT((int)(pt1.x), (int)(pt1.y >> XY_SHIFT));
                pt1.x++;
                pt1.y += y_step;
                ecount--;
            }
        }
        else
        {
            pt1.y >>= XY_SHIFT;

            while( ecount >= 0 )
            {
                ICC_PUT_POINT((int)(pt1.x >> XY_SHIFT), (int)(pt1.y));
                pt1.x += x_step;
                pt1.y++;
                ecount--;
            }
        }

        #undef ICC_PUT_POINT
    }
    else
    {
        #define  ICC_PUT_POINT(_x,_y)   \
        x = (_x); y = (_y);             \
        if( 0 <= x && x < width && \
            0 <= y && y < height ) \
        {                               \
            tptr = ptr + y*step + x*pix_size;\
            for( j = 0; j < pix_size; j++ ) \
                tptr[j] = ((uchar*)color)[j]; \
        }

        ICC_PUT_POINT((int)((pt2.x + (XY_ONE >> 1)) >> XY_SHIFT),
                      (int)((pt2.y + (XY_ONE >> 1)) >> XY_SHIFT));

        if( ax > ay )
        {
            pt1.x >>= XY_SHIFT;

            while( ecount >= 0 )
            {
                ICC_PUT_POINT((int)(pt1.x), (int)(pt1.y >> XY_SHIFT));
                pt1.x++;
                pt1.y += y_step;
                ecount--;
            }
        }
        else
        {
            pt1.y >>= XY_SHIFT;

            while( ecount >= 0 )
            {
                ICC_PUT_POINT((int)(pt1.x >> XY_SHIFT), (int)(pt1.y));
                pt1.x += x_step;
                pt1.y++;
                ecount--;
            }
        }

        #undef ICC_PUT_POINT
    }
}


/****************************************************************************************\
*                   Antialiazed Elliptic Arcs via Antialiazed Lines                      *
\****************************************************************************************/

static const float SinTable[] =
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
sincos( int angle, float& cosval, float& sinval )
{
    angle += (angle < 0 ? 360 : 0);
    sinval = SinTable[angle];
    cosval = SinTable[450 - angle];
}

static int ellipse2Poly( double center_x, double center_y, double axes_width, double axes_height, int angle, int arc_start, int arc_end,
                   int delta, CPoint2d* pts )
{
    CC_INSTRUMENT_REGION()

    float alpha, beta;
    int i, npts = 0;

    while( angle < 0 )
        angle += 360;
    while( angle > 360 )
        angle -= 360;

    if( arc_start > arc_end )
    {
        i = arc_start;
        arc_start = arc_end;
        arc_end = i;
    }
    while( arc_start < 0 )
    {
        arc_start += 360;
        arc_end += 360;
    }
    while( arc_end > 360 )
    {
        arc_end -= 360;
        arc_start -= 360;
    }
    if( arc_end - arc_start > 360 )
    {
        arc_start = 0;
        arc_end = 360;
    }
    sincos( angle, alpha, beta );

    for( i = arc_start; i < arc_end + delta; i += delta )
    {
        double x, y;
        angle = i;
        if( angle > arc_end )
            angle = arc_end;
        if( angle < 0 )
            angle += 360;

        x = axes_width * SinTable[450-angle];
        y = axes_height * SinTable[angle];
        CPoint2d pt;
        pt.x = center_x + x * alpha - y * beta;
        pt.y = center_y + x * beta + y * alpha;
        pts[npts++] = (pt);
    }

    // If there are no points, it's a zero-size polygon
    if( npts == 1) {
        CPoint2d center = {center_x, center_y};
        pts[0] = center;
        pts[1] = center;
        npts = 2;
    }
    return npts;
}

/*
   constructs polygon that represents elliptic arc.
*/
static int ellipse2Poly( double center_x, double center_y, double axes_width, double axes_height, int angle, int arcStart, int arcEnd,
                   int delta, CC_OUT CPoint* pts )
{
    CPoint2d _pts[361];
    int i, npts=0, n_pts = ellipse2Poly(center_x, center_y, axes_width, axes_height, angle, arcStart, arcEnd, delta, _pts);
    CPoint prevPt = {INT_MIN, INT_MIN};
    for (i = 0; i < n_pts; ++i)
    {
        CPoint pt;
        pt.x = cRound(_pts[i].x);
        pt.y = cRound(_pts[i].y);
        if (pt.x != prevPt.x || pt.y != prevPt.y) {
            pts[npts++] = (pt);
            prevPt = pt;
        }
    }

    // If there are no points, it's a zero-size polygon
    if (npts == 1) {
        CPoint center = {cRound(center_x), cRound(center_y)};
        pts[0] = center;
        pts[1] = center;
        npts = 2;
    }
    return npts;
}


static void
EllipseEx( img_t* img, int center_x, int center_y, int axes_width, int axes_height,
           int angle, int arc_start, int arc_end,
           const void* color, int thickness, int line_type )
{
    axes_width = ABS(axes_width), axes_height = ABS(axes_height);
    int delta = (int)((MAX(axes_width,axes_height)+(XY_ONE>>1))>>XY_SHIFT);
    delta = delta < 3 ? 90 : delta < 10 ? 30 : delta < 15 ? 18 : 5;

    CPoint2d _v[361];
    int i, nv = 0, n_v = ellipse2Poly( (double)center_x, (double)center_y, (double)axes_width, (double)axes_height, angle, arc_start, arc_end, delta, _v );

    CPoint v[361];
    CPoint prevPt = {-1, -1};
    
    for (i = 0; i < n_v; ++i)
    {
        CPoint pt;
        pt.x = (int)cRound(_v[i].x / XY_ONE) << XY_SHIFT;
        pt.y = (int)cRound(_v[i].y / XY_ONE) << XY_SHIFT;
        pt.x += cRound(_v[i].x - pt.x);
        pt.y += cRound(_v[i].y - pt.y);
        if (pt.x != prevPt.x || pt.y != prevPt.y) {
            v[nv++] = (pt);
            prevPt = pt;
        }
    }

    // If there are no points, it's a zero-size polygon
    if (nv == 1) {
        v[0].x = v[1].x = center_x;
        v[0].y = v[1].y = center_y;
        nv = 2;
    }

    if( thickness >= 0 )
        PolyLine( img, &v[0], nv, false, color, thickness, line_type, XY_SHIFT );
    else if( arc_end - arc_start >= 360 )
        FillConvexPoly( img, &v[0], nv, color, line_type, XY_SHIFT );
    else
    {
        v[nv].x = center_x;
        v[nv].y = center_y;
        nv++;
        CPolyEdge* edges = MALLOC(CPolyEdge, nv);
        int edges_count = CollectPolyEdges( img,  &v[0], nv, edges, color, line_type, XY_SHIFT );
        FillEdgeCollection( img, edges, edges_count, color );
        FREE(edges);
    }
}


/****************************************************************************************\
*                                Polygons filling                                        *
\****************************************************************************************/

/* helper macros: filling horizontal row */
#define ICC_HLINE( ptr, xl, xr, color, pix_size )            \
{                                                            \
    uchar* hline_ptr = (uchar*)(ptr) + (xl)*(pix_size);      \
    uchar* hline_max_ptr = (uchar*)(ptr) + (xr)*(pix_size);  \
                                                             \
    for( ; hline_ptr <= hline_max_ptr; hline_ptr += (pix_size))\
    {                                                        \
        int hline_j;                                         \
        for( hline_j = 0; hline_j < (pix_size); hline_j++ )  \
        {                                                    \
            hline_ptr[hline_j] = ((uchar*)color)[hline_j];   \
        }                                                    \
    }                                                        \
}


/* filling convex polygon. v - array of vertices, ntps - number of points */
static void
FillConvexPoly( img_t* img, const CPoint* v, int npts, const void* color, int line_type, int shift )
{
    struct
    {
        int idx, di;
        int x, dx;
        int ye;
    }
    edge[2];

    int delta = 1 << shift >> 1;
    int i, y, imin = 0;
    int edges = npts;
    int xmin, xmax, ymin, ymax;
    uchar* ptr = img->tt.data;
    CSize size = cvGetSize(img);
    int pix_size = CC_ELEM_SIZE_(img);
    CPoint p0;
    int delta1, delta2;

    if( line_type < CC_AA )
        delta1 = delta2 = XY_ONE >> 1;
    else
        delta1 = XY_ONE - 1, delta2 = 0;

    p0 = v[npts - 1];
    p0.x <<= XY_SHIFT - shift;
    p0.y <<= XY_SHIFT - shift;

    assert( 0 <= shift && shift <= XY_SHIFT );
    xmin = xmax = v[0].x;
    ymin = ymax = v[0].y;

    for( i = 0; i < npts; i++ )
    {
        CPoint p = v[i];
        if( p.y < ymin )
        {
            ymin = p.y;
            imin = i;
        }

        ymax = MAX( ymax, p.y );
        xmax = MAX( xmax, p.x );
        xmin = MIN( xmin, p.x );

        p.x <<= XY_SHIFT - shift;
        p.y <<= XY_SHIFT - shift;

        if( line_type <= 8 )
        {
            if( shift == 0 )
            {
                CPoint pt0, pt1;
                pt0.x = (int)(p0.x >> XY_SHIFT);
                pt0.y = (int)(p0.y >> XY_SHIFT);
                pt1.x = (int)(p.x >> XY_SHIFT);
                pt1.y = (int)(p.y >> XY_SHIFT);
                Line( img, pt0, pt1, color, line_type );
            }
            else {
              Line2( img, p0, p, color );
            }
        }
        else {
            LineAA( img, p0, p, color );
        }
        p0 = p;
    }

    xmin = (xmin + delta) >> shift;
    xmax = (xmax + delta) >> shift;
    ymin = (ymin + delta) >> shift;
    ymax = (ymax + delta) >> shift;

    if( npts < 3 || (int)xmax < 0 || (int)ymax < 0 || (int)xmin >= size.width || (int)ymin >= size.height )
        return;

    ymax = MIN( ymax, size.height - 1 );
    edge[0].idx = edge[1].idx = imin;

    edge[0].ye = edge[1].ye = y = (int)ymin;
    edge[0].di = 1;
    edge[1].di = npts - 1;

    ptr += img->step*y;

    do
    {
        if( line_type < CC_AA || y < (int)ymax || y == (int)ymin )
        {
            for( i = 0; i < 2; i++ )
            {
                if( y >= edge[i].ye )
                {
                    int idx0 = edge[i].idx, di = edge[i].di;
                    int idx = idx0 + di;
                    if (idx >= npts) idx -= npts;
                    int ty = 0;

                    for (; edges-- > 0; )
                    {
                        ty = (int)((v[idx].y + delta) >> shift);
                        if (ty > y)
                        {
                            int xs = v[idx0].x;
                            int xe = v[idx].x;
                            if (shift != XY_SHIFT)
                            {
                                xs <<= XY_SHIFT - shift;
                                xe <<= XY_SHIFT - shift;
                            }

                            edge[i].ye = ty;
                            edge[i].dx = ((xe - xs)*2 + (ty - y)) / (2 * (ty - y));
                            edge[i].x = xs;
                            edge[i].idx = idx;
                            break;
                        }
                        idx0 = idx;
                        idx += di;
                        if (idx >= npts) idx -= npts;
                    }
                }
            }
        }

        if (y >= 0)
        {
            int left = 0, right = 1;
            if (edge[0].x > edge[1].x)
            {
                left = 1, right = 0;
            }

            int xx1 = (int)((edge[left].x + delta1) >> XY_SHIFT);
            int xx2 = (int)((edge[right].x + delta2) >> XY_SHIFT);

            if( xx2 >= 0 && xx1 < size.width )
            {
                if( xx1 < 0 )
                    xx1 = 0;
                if( xx2 >= size.width )
                    xx2 = size.width - 1;
                ICC_HLINE( ptr, xx1, xx2, color, pix_size );
            }
        }
        else
        {
            // TODO optimize scan for negative y
        }

        edge[0].x += edge[0].dx;
        edge[1].x += edge[1].dx;
        ptr += img->step;
    }
    while( ++y <= (int)ymax );
}


/******** Arbitrary polygon **********/

static int
CollectPolyEdges( img_t* img, const CPoint* v, int count, CPolyEdge* edges,
                  const void* color, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0), CPoint offset )
{
    int i, delta = offset.y + ((1 << shift) >> 1), edges_count = 0;
    CPoint pt0 = v[count-1], pt1;
    pt0.x = (pt0.x + offset.x) << (XY_SHIFT - shift);
    pt0.y = (pt0.y + delta) >> shift;

    for( i = 0; i < count; i++, pt0 = pt1 )
    {
        CPoint t0, t1;
        CPolyEdge edge;

        pt1 = v[i];
        pt1.x = (pt1.x + offset.x) << (XY_SHIFT - shift);
        pt1.y = (pt1.y + delta) >> shift;

        if( line_type < CC_AA )
        {
            t0.y = pt0.y; t1.y = pt1.y;
            t0.x = (pt0.x + (XY_ONE >> 1)) >> XY_SHIFT;
            t1.x = (pt1.x + (XY_ONE >> 1)) >> XY_SHIFT;
            Line( img, t0, t1, color, line_type );
        }
        else
        {
            t0.x = pt0.x; t1.x = pt1.x;
            t0.y = pt0.y << XY_SHIFT;
            t1.y = pt1.y << XY_SHIFT;
            LineAA( img, t0, t1, color );
        }

        if( pt0.y == pt1.y )
            continue;

        if( pt0.y < pt1.y )
        {
            edge.y0 = (int)(pt0.y);
            edge.y1 = (int)(pt1.y);
            edge.x = pt0.x;
        }
        else
        {
            edge.y0 = (int)(pt1.y);
            edge.y1 = (int)(pt0.y);
            edge.x = pt1.x;
        }
        edge.dx = (pt1.x - pt0.x) / (pt1.y - pt0.y);
        edges[edges_count++] = (edge);
    }
    return edges_count;
}

CC_INLINE bool CmpEdges(const CPolyEdge& e1, const CPolyEdge& e2)
{
  return e1.y0 - e2.y0 ? e1.y0 < e2.y0 :
  e1.x - e2.x ? e1.x < e2.x : e1.dx < e2.dx;
}


/**************** helper macros and functions for sequence/contour processing ***********/

static void
FillEdgeCollection( img_t* img, CPolyEdge* edges, int edges_count, const void* color )
{
    CPolyEdge tmp;
    int i, y, total = edges_count;
    CSize size = cvGetSize(img);
    CPolyEdge* e;
    int y_max = INT_MIN, y_min = INT_MAX;
    int x_max = INT_MIN, x_min = INT_MAX;
    int pix_size = CC_ELEM_SIZE_(img);

    if( total < 2 )
        return;

    for( i = 0; i < total; i++ )
    {
        CPolyEdge* e1 = edges+i;
        assert( e1->y0 < e1->y1 );
        // Determine x-coordinate of the end of the edge.
        // (This is not necessary x-coordinate of any vertex in the array.)
        int x1 = e1->x + (e1->y1 - e1->y0) * e1->dx;
        y_min = MIN( y_min, e1->y0 );
        y_max = MAX( y_max, e1->y1 );
        x_min = MIN( x_min, e1->x );
        x_max = MAX( x_max, e1->x );
        x_min = MIN( x_min, x1 );
        x_max = MAX( x_max, x1 );
    }

    if( y_max < 0 || y_min >= size.height || x_max < 0 || x_min >= ((int)size.width<<XY_SHIFT) )
        return;

    QSORT( edges, edges+edges_count, CmpEdges, CPolyEdge );

    // start drawing
    tmp.y0 = INT_MAX;
    edges[edges_count++] = (tmp); // after this point we do not add
                          // any elements to edges, thus we can use pointers
    i = 0;
    tmp.next = 0;
    e = &edges[i];
    y_max = MIN( y_max, size.height );

    for( y = e->y0; y < y_max; y++ )
    {
        CPolyEdge *last, *prelast, *keep_prelast;
        int sort_flag = 0;
        int draw = 0;
        int clipline = y < 0;

        prelast = &tmp;
        last = tmp.next;
        while( last || e->y0 == y )
        {
            if( last && last->y1 == y )
            {
                // exclude edge if y reachs its lower point
                prelast->next = last->next;
                last = last->next;
                continue;
            }
            keep_prelast = prelast;
            if( last && (e->y0 > y || last->x < e->x) )
            {
                // go to the next edge in active list
                prelast = last;
                last = last->next;
            }
            else if( i < total )
            {
                // insert new edge into active list if y reachs its upper point
                prelast->next = e;
                e->next = last;
                prelast = e;
                e = &edges[++i];
            }
            else
                break;

            if( draw )
            {
                if( !clipline )
                {
                    // convert x's from fixed-point to image coordinates
                    uchar *timg = img_row(uchar, img, y);
                    int x1, x2;

                    if (keep_prelast->x > prelast->x)
                    {
                        x1 = (int)((prelast->x + XY_ONE - 1) >> XY_SHIFT);
                        x2 = (int)(keep_prelast->x >> XY_SHIFT);
                    }
                    else
                    {
                        x1 = (int)((keep_prelast->x + XY_ONE - 1) >> XY_SHIFT);
                        x2 = (int)(prelast->x >> XY_SHIFT);
                    }

                    // clip and draw the line
                    if( x1 < size.width && x2 >= 0 )
                    {
                        if( x1 < 0 )
                            x1 = 0;
                        if( x2 >= size.width )
                            x2 = size.width - 1;
                        ICC_HLINE( timg, x1, x2, color, pix_size );
                    }
                }
                keep_prelast->x += keep_prelast->dx;
                prelast->x += prelast->dx;
            }
            draw ^= 1;
        }

        // sort edges (using bubble sort)
        keep_prelast = 0;

        do
        {
            prelast = &tmp;
            last = tmp.next;

            while( last != keep_prelast && last->next != 0 )
            {
                CPolyEdge *te = last->next;

                // swap edges
                if( last->x > te->x )
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
        while( sort_flag && keep_prelast != tmp.next && keep_prelast != &tmp );
    }
}


/* draws simple or filled circle */
static void
Circle( img_t* img, CPoint center, int radius, const void* color, int fill )
{
    CSize size = cvGetSize(img);
    size_t step = img->step;
    int pix_size = CC_ELEM_SIZE_(img);
    uchar* ptr = img->tt.data;
    int err = 0, dx = radius, dy = 0, plus = 1, minus = (radius << 1) - 1;
    int inside = center.x >= radius && center.x < size.width - radius &&
        center.y >= radius && center.y < size.height - radius;

    #define ICC_PUT_POINT( ptr, x )     \
        memcpy( ptr + (x)*pix_size, color, pix_size );

    while( dx >= dy )
    {
        int mask;
        int y11 = center.y - dy, y12 = center.y + dy, y21 = center.y - dx, y22 = center.y + dx;
        int x11 = center.x - dx, x12 = center.x + dx, x21 = center.x - dy, x22 = center.x + dy;

        if( inside )
        {
            uchar *tptr0 = ptr + y11 * step;
            uchar *tptr1 = ptr + y12 * step;

            if( !fill )
            {
                ICC_PUT_POINT( tptr0, x11 );
                ICC_PUT_POINT( tptr1, x11 );
                ICC_PUT_POINT( tptr0, x12 );
                ICC_PUT_POINT( tptr1, x12 );
            }
            else
            {
                ICC_HLINE( tptr0, x11, x12, color, pix_size );
                ICC_HLINE( tptr1, x11, x12, color, pix_size );
            }

            tptr0 = ptr + y21 * step;
            tptr1 = ptr + y22 * step;

            if( !fill )
            {
                ICC_PUT_POINT( tptr0, x21 );
                ICC_PUT_POINT( tptr1, x21 );
                ICC_PUT_POINT( tptr0, x22 );
                ICC_PUT_POINT( tptr1, x22 );
            }
            else
            {
                ICC_HLINE( tptr0, x21, x22, color, pix_size );
                ICC_HLINE( tptr1, x21, x22, color, pix_size );
            }
        }
        else if( x11 < size.width && x12 >= 0 && y21 < size.height && y22 >= 0 )
        {
            if( fill )
            {
                x11 = MAX( x11, 0 );
                x12 = MIN( x12, size.width - 1 );
            }

            if( (unsigned)y11 < (unsigned)size.height )
            {
                uchar *tptr = ptr + y11 * step;

                if( !fill )
                {
                    if( x11 >= 0 )
                        ICC_PUT_POINT( tptr, x11 );
                    if( x12 < size.width )
                        ICC_PUT_POINT( tptr, x12 );
                }
                else
                    ICC_HLINE( tptr, x11, x12, color, pix_size );
            }

            if( (unsigned)y12 < (unsigned)size.height )
            {
                uchar *tptr = ptr + y12 * step;

                if( !fill )
                {
                    if( x11 >= 0 )
                        ICC_PUT_POINT( tptr, x11 );
                    if( x12 < size.width )
                        ICC_PUT_POINT( tptr, x12 );
                }
                else
                    ICC_HLINE( tptr, x11, x12, color, pix_size );
            }

            if( x21 < size.width && x22 >= 0 )
            {
                if( fill )
                {
                    x21 = MAX( x21, 0 );
                    x22 = MIN( x22, size.width - 1 );
                }

                if( (unsigned)y21 < (unsigned)size.height )
                {
                    uchar *tptr = ptr + y21 * step;

                    if( !fill )
                    {
                        if( x21 >= 0 )
                            ICC_PUT_POINT( tptr, x21 );
                        if( x22 < size.width )
                            ICC_PUT_POINT( tptr, x22 );
                    }
                    else
                        ICC_HLINE( tptr, x21, x22, color, pix_size );
                }

                if( (unsigned)y22 < (unsigned)size.height )
                {
                    uchar *tptr = ptr + y22 * step;

                    if( !fill )
                    {
                        if( x21 >= 0 )
                            ICC_PUT_POINT( tptr, x21 );
                        if( x22 < size.width )
                            ICC_PUT_POINT( tptr, x22 );
                    }
                    else
                        ICC_HLINE( tptr, x21, x22, color, pix_size );
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
ThickLine( img_t* img, CPoint p0, CPoint p1, const void* color,
           int thickness, int line_type, int flags, int shift )
{
    static const double INV_XY_ONE = 1./XY_ONE;

    p0.x <<= XY_SHIFT - shift;
    p0.y <<= XY_SHIFT - shift;
    p1.x <<= XY_SHIFT - shift;
    p1.y <<= XY_SHIFT - shift;

    if( thickness <= 1 )
    {
        if( line_type < CC_AA )
        {
            if( line_type == 1 || line_type == 4 || shift == 0 )
            {
                p0.x = (p0.x + (XY_ONE>>1)) >> XY_SHIFT;
                p0.y = (p0.y + (XY_ONE>>1)) >> XY_SHIFT;
                p1.x = (p1.x + (XY_ONE>>1)) >> XY_SHIFT;
                p1.y = (p1.y + (XY_ONE>>1)) >> XY_SHIFT;
                Line( img, p0, p1, color, line_type );
            }
            else
                Line2( img, p0, p1, color );
        }
        else
            LineAA( img, p0, p1, color );
    }
    else
    {
        CPoint pt[4], dp = {0,0};
        double dx = (p0.x - p1.x)*INV_XY_ONE, dy = (p1.y - p0.y)*INV_XY_ONE;
        double r = dx * dx + dy * dy;
        int i, oddThickness = thickness & 1;
        thickness <<= XY_SHIFT - 1;

        if( fabs(r) > DBL_EPSILON )
        {
            r = (thickness + oddThickness*XY_ONE*0.5)/sqrt(r);
            dp.x = cRound( dy * r );
            dp.y = cRound( dx * r );

            pt[0].x = p0.x + dp.x;
            pt[0].y = p0.y + dp.y;
            pt[1].x = p0.x - dp.x;
            pt[1].y = p0.y - dp.y;
            pt[2].x = p1.x - dp.x;
            pt[2].y = p1.y - dp.y;
            pt[3].x = p1.x + dp.x;
            pt[3].y = p1.y + dp.y;

            FillConvexPoly( img, pt, 4, color, line_type, XY_SHIFT );
        }

        for( i = 0; i < 2; i++ )
        {
            if( flags & (i+1) )
            {
                if( line_type < CC_AA )
                {
                    CPoint center;
                    center.x = (int)((p0.x + (XY_ONE>>1)) >> XY_SHIFT);
                    center.y = (int)((p0.y + (XY_ONE>>1)) >> XY_SHIFT);
                    Circle( img, center, (thickness + (XY_ONE>>1)) >> XY_SHIFT, color, 1 );
                }
                else
                {
                    EllipseEx( img, p0.x, p0.y, thickness, thickness,
                               0, 0, 360, color, -1, line_type );
                }
            }
            p0 = p1;
        }
    }
}


static void
PolyLine( img_t* img, const CPoint* v, int count, bool is_closed,
          const void* color, int thickness,
          int line_type, int shift )
{
    if( !v || count <= 0 )
        return;

    int i = is_closed ? count - 1 : 0;
    int flags = 2 + !is_closed;
    CPoint p0;
    CC_Assert( 0 <= shift && shift <= XY_SHIFT && thickness >= 0 );

    p0 = v[i];
    for( i = !is_closed; i < count; i++ )
    {
        CPoint p = v[i];
        ThickLine( img, p0, p, color, thickness, line_type, flags, shift );
        p0 = p;
        flags = 2;
    }
}

/****************************************************************************************\
*                              External functions                                        *
\****************************************************************************************/

static void line( img_t* img, CPoint pt1, CPoint pt2, CScalar color,
           int thickness = 1, int line_type = CC_LINE_8, int shift = 0 )
{
    CC_INSTRUMENT_REGION()

    if( line_type == CC_AA && CC_MAT_DEPTH(img) != CC_8U )
        line_type = 8;

    CC_Assert( 0 <= thickness && thickness <= MAX_THICKNESS );
    CC_Assert( 0 <= shift && shift <= XY_SHIFT );

    double buf[4];
    cvScalarToRawData( &color, buf, CC_MAT_DEPTH(img), CC_MAT_CN(img), 0 );
    ThickLine( img, pt1, pt2, buf, thickness, line_type, 3, shift );
}

CC_INLINE double cPointDist(IPOINT pt1, IPOINT pt2) {
  double dx = pt1.x-pt2.x;
  double dy = pt1.y-pt2.y;
  return sqrt(dx*dx + dy*dy);
}

static void arrowedLine(img_t* img, CPoint pt1, CPoint pt2, const CScalar& color,
           int thickness, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0), double tipLength CC_DEFAULT(0) )
{
    CC_INSTRUMENT_REGION()

    const double tipSize = cPointDist(pt1, pt2)*tipLength; // Factor to normalize the size of the tip depending on the length of the arrow

    line(img, pt1, pt2, color, thickness, line_type, shift);

    const double angle = atan2( (double) pt1.y - pt2.y, (double) pt1.x - pt2.x );

    CPoint p = {cRound(pt2.x + tipSize * cos(angle + CC_PI / 4)),
      cRound(pt2.y + tipSize * sin(angle + CC_PI / 4))};
    line(img, p, pt2, color, thickness, line_type, shift);

    p.x = cRound(pt2.x + tipSize * cos(angle - CC_PI / 4));
    p.y = cRound(pt2.y + tipSize * sin(angle - CC_PI / 4));
    line(img, p, pt2, color, thickness, line_type, shift);
}

static void rectangle( img_t* img, CPoint pt1, CPoint pt2,
                const CScalar& color, int thickness = 1,
                int lineType = CC_LINE_8, int shift = 0 )
{
    CC_INSTRUMENT_REGION()

    if( lineType == CC_AA && CC_MAT_DEPTH(img) != CC_8U )
        lineType = 8;

    CC_Assert( thickness <= MAX_THICKNESS );
    CC_Assert( 0 <= shift && shift <= XY_SHIFT );

    double buf[4];
    cvScalarToRawData(&color, buf, CC_MAT_DEPTH(img), CC_MAT_CN(img), 0);

    CPoint pt[4];

    pt[0] = pt1;
    pt[1].x = pt2.x;
    pt[1].y = pt1.y;
    pt[2] = pt2;
    pt[3].x = pt1.x;
    pt[3].y = pt2.y;

    if( thickness >= 0 )
        PolyLine( img, pt, 4, true, buf, thickness, lineType, shift );
    else
        FillConvexPoly( img, pt, 4, buf, lineType, shift );
}

static 
void rectangle( img_t* img, CRect rec, const CScalar& color, int thickness CC_DEFAULT(1),
                int lineType CC_DEFAULT(CC_LINE_8), int shift CC_DEFAULT(0) )
{
    CC_INSTRUMENT_REGION()

    CC_Assert( 0 <= shift && shift <= XY_SHIFT );
    if( (rec.width*rec.height) > 0 )
        rectangle( img, cPoint(rec.x, rec.y),
        cPoint(rec.x + rec.width - (1<<shift), rec.y + rec.height - (1<<shift)),
                   color, thickness, lineType, shift );
}


//! Possible set of marker types used for the drawMarker function
enum MarkerTypes
{
    MARKER_CROSS = 0,           //!< A crosshair marker shape
    MARKER_TILTED_CROSS = 1,    //!< A 45 degree tilted crosshair marker shape
    MARKER_STAR = 2,            //!< A star marker shape, combination of cross and tilted cross
    MARKER_DIAMOND = 3,         //!< A diamond marker shape
    MARKER_SQUARE = 4,          //!< A square marker shape
    MARKER_TRIANGLE_UP = 5,     //!< An upwards pointing triangle marker shape
    MARKER_TRIANGLE_DOWN = 6    //!< A downwards pointing triangle marker shape
};

/* ----------------------------------------------------------------------------------------- */
/* ADDING A SET OF PREDEFINED MARKERS WHICH COULD BE USED TO HIGHLIGHT POSITIONS IN AN IMAGE */
/* ----------------------------------------------------------------------------------------- */

static void drawMarker(img_t* img, CPoint position, const CScalar& color, MarkerTypes markerType, int markerSize, int thickness, int line_type)
{
    switch(markerType)
    {
    // The cross marker case
    case MARKER_CROSS:
        line(img, cPoint(position.x-(markerSize/2), position.y), cPoint(position.x+(markerSize/2), position.y), color, thickness, line_type);
        line(img, cPoint(position.x, position.y-(markerSize/2)), cPoint(position.x, position.y+(markerSize/2)), color, thickness, line_type);
        break;

    // The tilted cross marker case
    case MARKER_TILTED_CROSS:
        line(img, cPoint(position.x-(markerSize/2), position.y-(markerSize/2)), cPoint(position.x+(markerSize/2), position.y+(markerSize/2)), color, thickness, line_type);
        line(img, cPoint(position.x+(markerSize/2), position.y-(markerSize/2)), cPoint(position.x-(markerSize/2), position.y+(markerSize/2)), color, thickness, line_type);
        break;

    // The star marker case
    case MARKER_STAR:
        line(img, cPoint(position.x-(markerSize/2), position.y), cPoint(position.x+(markerSize/2), position.y), color, thickness, line_type);
        line(img, cPoint(position.x, position.y-(markerSize/2)), cPoint(position.x, position.y+(markerSize/2)), color, thickness, line_type);
        line(img, cPoint(position.x-(markerSize/2), position.y-(markerSize/2)), cPoint(position.x+(markerSize/2), position.y+(markerSize/2)), color, thickness, line_type);
        line(img, cPoint(position.x+(markerSize/2), position.y-(markerSize/2)), cPoint(position.x-(markerSize/2), position.y+(markerSize/2)), color, thickness, line_type);
        break;

    // The diamond marker case
    case MARKER_DIAMOND:
        line(img, cPoint(position.x, position.y-(markerSize/2)), cPoint(position.x+(markerSize/2), position.y), color, thickness, line_type);
        line(img, cPoint(position.x+(markerSize/2), position.y), cPoint(position.x, position.y+(markerSize/2)), color, thickness, line_type);
        line(img, cPoint(position.x, position.y+(markerSize/2)), cPoint(position.x-(markerSize/2), position.y), color, thickness, line_type);
        line(img, cPoint(position.x-(markerSize/2), position.y), cPoint(position.x, position.y-(markerSize/2)), color, thickness, line_type);
        break;

    // The square marker case
    case MARKER_SQUARE:
        line(img, cPoint(position.x-(markerSize/2), position.y-(markerSize/2)), cPoint(position.x+(markerSize/2), position.y-(markerSize/2)), color, thickness, line_type);
        line(img, cPoint(position.x+(markerSize/2), position.y-(markerSize/2)), cPoint(position.x+(markerSize/2), position.y+(markerSize/2)), color, thickness, line_type);
        line(img, cPoint(position.x+(markerSize/2), position.y+(markerSize/2)), cPoint(position.x-(markerSize/2), position.y+(markerSize/2)), color, thickness, line_type);
        line(img, cPoint(position.x-(markerSize/2), position.y+(markerSize/2)), cPoint(position.x-(markerSize/2), position.y-(markerSize/2)), color, thickness, line_type);
        break;

    // The triangle up marker case
    case MARKER_TRIANGLE_UP:
        line(img, cPoint(position.x-(markerSize/2), position.y+(markerSize/2)), cPoint(position.x+(markerSize/2), position.y+(markerSize/2)), color, thickness, line_type);
        line(img, cPoint(position.x+(markerSize/2), position.y+(markerSize/2)), cPoint(position.x, position.y-(markerSize/2)), color, thickness, line_type);
        line(img, cPoint(position.x, position.y-(markerSize/2)), cPoint(position.x-(markerSize/2), position.y+(markerSize/2)), color, thickness, line_type);
        break;

    // The triangle down marker case
    case MARKER_TRIANGLE_DOWN:
        line(img, cPoint(position.x-(markerSize/2), position.y-(markerSize/2)), cPoint(position.x+(markerSize/2), position.y-(markerSize/2)), color, thickness, line_type);
        line(img, cPoint(position.x+(markerSize/2), position.y-(markerSize/2)), cPoint(position.x, position.y+(markerSize/2)), color, thickness, line_type);
        line(img, cPoint(position.x, position.y+(markerSize/2)), cPoint(position.x-(markerSize/2), position.y-(markerSize/2)), color, thickness, line_type);
        break;

    // If any number that doesn't exist is entered as marker type, draw a cross marker, to avoid crashes
    default:
        drawMarker(img, position, color, MARKER_CROSS, markerSize, thickness, line_type);
        break;
    }
}

static void circle( img_t* img, CPoint center, int radius,
             const CScalar& color, int thickness = 1,
             int line_type = CC_LINE_8, int shift = 0 )
{
    CC_INSTRUMENT_REGION()

    

    if( line_type == CC_AA && CC_MAT_DEPTH(img) != CC_8U )
        line_type = 8;

    CC_Assert( radius >= 0 && thickness <= MAX_THICKNESS &&
        0 <= shift && shift <= XY_SHIFT );

    double buf[4];
    cvScalarToRawData(&color, buf, CC_MAT_DEPTH(img), CC_MAT_CN(img), 0);

    if( thickness > 1 || line_type != CC_LINE_8 || shift > 0 )
    {
        int s = XY_SHIFT - shift;
        int _radius = radius << s;
        EllipseEx( img, center.x << s, center.y << s, _radius, _radius,
                   0, 0, 360, buf, thickness, line_type );
    }
    else
        Circle( img, center, radius, buf, thickness < 0 );
}


static void ellipse( img_t* img, int center_x, int center_y, int axes_width, int axes_height,
              double angle, double start_angle, double end_angle,
              const CScalar& color, int thickness, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    CC_INSTRUMENT_REGION()

    

    if( line_type == CC_AA && CC_MAT_DEPTH(img) != CC_8U )
        line_type = 8;

    CC_Assert( axes_width >= 0 && axes_height >= 0 &&
        thickness <= MAX_THICKNESS && 0 <= shift && shift <= XY_SHIFT );

    double buf[4];
    cvScalarToRawData(&color, buf, CC_MAT_DEPTH(img), CC_MAT_CN(img), 0);

    int _angle = cRound(angle);
    int _start_angle = cRound(start_angle);
    int _end_angle = cRound(end_angle);
    int s = XY_SHIFT - shift;

    EllipseEx( img, center_x<<s, center_y<<s, axes_width<<s, axes_height<<s,
      _angle, _start_angle, _end_angle, buf, thickness, line_type );
}

typedef struct RotatedRect
{
    FPOINT center; //< the rectangle mass center
    float width, height;    //< width and height of the rectangle
    float angle;    //< the rotation angle. When the angle is 0, 90, 180, 270 etc., the rectangle becomes an up-right rectangle.
} RotatedRect;
static void
ellipse(img_t* img, const RotatedRect& box, const CScalar& color,
             int thickness, int lineType)
{
    CC_INSTRUMENT_REGION()

    if( lineType == CC_AA && CC_MAT_DEPTH(img) != CC_8U )
        lineType = 8;

    CC_Assert( box.width >= 0 && box.height >= 0 &&
               thickness <= MAX_THICKNESS );

    double buf[4];
    cvScalarToRawData(&color, buf, CC_MAT_DEPTH(img), CC_MAT_CN(img), 0);

    int _angle = cRound(box.angle);
    CPoint center = {cRound(box.center.x), cRound(box.center.y)};
    center.x = (center.x << XY_SHIFT) + cRound((box.center.x - center.x)*XY_ONE);
    center.y = (center.y << XY_SHIFT) + cRound((box.center.y - center.y)*XY_ONE);
    int axes_width = cRound(box.width);
    int axes_height = cRound(box.height);
    axes_width  = (axes_width  << (XY_SHIFT - 1)) + cRound((box.width - axes_width)*(XY_ONE>>1));
    axes_height = (axes_height << (XY_SHIFT - 1)) + cRound((box.height - axes_height)*(XY_ONE>>1));
    EllipseEx( img, center.x, center.y, axes_width, axes_height, _angle, 0, 360, buf, thickness, lineType );
}

static void fillConvexPoly( img_t* img, const CPoint* pts, int npts,
                     const CScalar& color, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    CC_INSTRUMENT_REGION()

    if( !pts || npts <= 0 )
        return;

    if( line_type == CC_AA && CC_MAT_DEPTH(img) != CC_8U )
        line_type = 8;

    double buf[4];
    CC_Assert( 0 <= shift && shift <=  XY_SHIFT );
    cvScalarToRawData(&color, buf, CC_MAT_DEPTH(img), CC_MAT_CN(img), 0);
    FillConvexPoly( img, pts, npts, buf, line_type, shift );
}

static 
void fillPoly( img_t* img, const CPoint** pts, const int* npts, int ncontours,
               const CScalar& color, int line_type CC_DEFAULT(CC_LINE_8),
               int shift CC_DEFAULT(0), CPoint offset CC_DEFAULT(cPoint(0, 0)) )
{
    CC_INSTRUMENT_REGION()

    if( line_type == CC_AA && CC_MAT_DEPTH(img) != CC_8U )
        line_type = 8;

    CC_Assert( pts && npts && ncontours >= 0 && 0 <= shift && shift <= XY_SHIFT );

    double buf[4];
    cvScalarToRawData(&color, buf, CC_MAT_DEPTH(img), CC_MAT_CN(img), 0);

    CPolyEdge* edges = NULL;

    int i, total = 0, edges_count = 0;
    for( i = 0; i < ncontours; i++ )
        total += npts[i];

    edges = MALLOC( CPolyEdge, total + 1 );
    for (i = 0; i < ncontours; i++)
    {
        edges_count += CollectPolyEdges(img, pts[i], npts[i], edges+edges_count, buf, line_type, shift, offset);
    }

    FillEdgeCollection(img, edges, edges_count, buf);
    FREE(edges);
}

static 
void polylines( img_t* img, const CPoint* const* pts, const int* npts, int ncontours, bool isClosed,
                const CScalar& color, int thickness, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    CC_INSTRUMENT_REGION()

    if( line_type == CC_AA && CC_MAT_DEPTH(img) != CC_8U )
        line_type = 8;

    CC_Assert( pts && npts && ncontours >= 0 &&
               0 <= thickness && thickness <= MAX_THICKNESS &&
               0 <= shift && shift <= XY_SHIFT );

    double buf[4];
    cvScalarToRawData(&color, buf, CC_MAT_DEPTH(img), CC_MAT_CN(img), 0 );

    for( int i = 0; i < ncontours; i++ )
    {
        PolyLine( img, pts[i], npts[i], isClosed, buf, thickness, line_type, shift );
    }
}


enum { FONT_SIZE_SHIFT=8, FONT_ITALIC_ALPHA=(1 << 8),
       FONT_ITALIC_DIGIT=(2 << 8), FONT_ITALIC_PUNCT=(4 << 8),
       FONT_ITALIC_BRACES=(8 << 8), FONT_HAVE_GREEK=(16 << 8),
       FONT_HAVE_CYRILLIC=(32 << 8) };

static const int HersheyPlain[] = {
(5 + 4*16) + FONT_HAVE_GREEK,
199, 214, 217, 233, 219, 197, 234, 216, 221, 222, 228, 225, 211, 224, 210, 220,
200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 212, 213, 191, 226, 192,
215, 190, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 193, 84,
194, 85, 86, 87, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
195, 223, 196, 88 };

static const int HersheyPlainItalic[] = {
(5 + 4*16) + FONT_ITALIC_ALPHA + FONT_HAVE_GREEK,
199, 214, 217, 233, 219, 197, 234, 216, 221, 222, 228, 225, 211, 224, 210, 220,
200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 212, 213, 191, 226, 192,
215, 190, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 193, 84,
194, 85, 86, 87, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
195, 223, 196, 88 };

static const int HersheyComplexSmall[] = {
(6 + 7*16) + FONT_HAVE_GREEK,
1199, 1214, 1217, 1275, 1274, 1271, 1272, 1216, 1221, 1222, 1219, 1232, 1211, 1231, 1210, 1220,
1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1209, 1212, 2213, 1241, 1238, 1242,
1215, 1273, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 1011, 1012, 1013,
1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023, 1024, 1025, 1026, 1223, 1084,
1224, 1247, 586, 1249, 1101, 1102, 1103, 1104, 1105, 1106, 1107, 1108, 1109, 1110, 1111,
1112, 1113, 1114, 1115, 1116, 1117, 1118, 1119, 1120, 1121, 1122, 1123, 1124, 1125, 1126,
1225, 1229, 1226, 1246 };

static const int HersheyComplexSmallItalic[] = {
(6 + 7*16) + FONT_ITALIC_ALPHA + FONT_HAVE_GREEK,
1199, 1214, 1217, 1275, 1274, 1271, 1272, 1216, 1221, 1222, 1219, 1232, 1211, 1231, 1210, 1220,
1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1209, 1212, 1213, 1241, 1238, 1242,
1215, 1273, 1051, 1052, 1053, 1054, 1055, 1056, 1057, 1058, 1059, 1060, 1061, 1062, 1063,
1064, 1065, 1066, 1067, 1068, 1069, 1070, 1071, 1072, 1073, 1074, 1075, 1076, 1223, 1084,
1224, 1247, 586, 1249, 1151, 1152, 1153, 1154, 1155, 1156, 1157, 1158, 1159, 1160, 1161,
1162, 1163, 1164, 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176,
1225, 1229, 1226, 1246 };

static const int HersheySimplex[] = {
(9 + 12*16) + FONT_HAVE_GREEK,
2199, 714, 717, 733, 719, 697, 734, 716, 721, 722, 728, 725, 711, 724, 710, 720,
700, 701, 702, 703, 704, 705, 706, 707, 708, 709, 712, 713, 691, 726, 692,
715, 690, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513,
514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 693, 584,
694, 2247, 586, 2249, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611,
612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626,
695, 723, 696, 2246 };

static const int HersheyDuplex[] = {
(9 + 12*16) + FONT_HAVE_GREEK,
2199, 2714, 2728, 2732, 2719, 2733, 2718, 2727, 2721, 2722, 2723, 2725, 2711, 2724, 2710, 2720,
2700, 2701, 2702, 2703, 2704, 2705, 2706, 2707, 2708, 2709, 2712, 2713, 2730, 2726, 2731,
2715, 2734, 2501, 2502, 2503, 2504, 2505, 2506, 2507, 2508, 2509, 2510, 2511, 2512, 2513,
2514, 2515, 2516, 2517, 2518, 2519, 2520, 2521, 2522, 2523, 2524, 2525, 2526, 2223, 2084,
2224, 2247, 587, 2249, 2601, 2602, 2603, 2604, 2605, 2606, 2607, 2608, 2609, 2610, 2611,
2612, 2613, 2614, 2615, 2616, 2617, 2618, 2619, 2620, 2621, 2622, 2623, 2624, 2625, 2626,
2225, 2229, 2226, 2246 };

static const int HersheyComplex[] = {
(9 + 12*16) + FONT_HAVE_GREEK + FONT_HAVE_CYRILLIC,
2199, 2214, 2217, 2275, 2274, 2271, 2272, 2216, 2221, 2222, 2219, 2232, 2211, 2231, 2210, 2220,
2200, 2201, 2202, 2203, 2204, 2205, 2206, 2207, 2208, 2209, 2212, 2213, 2241, 2238, 2242,
2215, 2273, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025, 2026, 2223, 2084,
2224, 2247, 587, 2249, 2101, 2102, 2103, 2104, 2105, 2106, 2107, 2108, 2109, 2110, 2111,
2112, 2113, 2114, 2115, 2116, 2117, 2118, 2119, 2120, 2121, 2122, 2123, 2124, 2125, 2126,
2225, 2229, 2226, 2246, 2801, 2802, 2803, 2804, 2805, 2806, 2807, 2808, 2809, 2810, 2811,
2812, 2813, 2814, 2815, 2816, 2817, 2818, 2819, 2820, 2821, 2822, 2823, 2824, 2825, 2826,
2827, 2828, 2829, 2830, 2831, 2832, 2901, 2902, 2903, 2904, 2905, 2906, 2907, 2908, 2909,
2910, 2911, 2912, 2913, 2914, 2915, 2916, 2917, 2918, 2919, 2920, 2921, 2922, 2923, 2924,
2925, 2926, 2927, 2928, 2929, 2930, 2931, 2932};

static const int HersheyComplexItalic[] = {
(9 + 12*16) + FONT_ITALIC_ALPHA + FONT_ITALIC_DIGIT + FONT_ITALIC_PUNCT +
FONT_HAVE_GREEK + FONT_HAVE_CYRILLIC,
2199, 2764, 2778, 2782, 2769, 2783, 2768, 2777, 2771, 2772, 2219, 2232, 2211, 2231, 2210, 2220,
2750, 2751, 2752, 2753, 2754, 2755, 2756, 2757, 2758, 2759, 2212, 2213, 2241, 2238, 2242,
2765, 2273, 2051, 2052, 2053, 2054, 2055, 2056, 2057, 2058, 2059, 2060, 2061, 2062, 2063,
2064, 2065, 2066, 2067, 2068, 2069, 2070, 2071, 2072, 2073, 2074, 2075, 2076, 2223, 2084,
2224, 2247, 587, 2249, 2151, 2152, 2153, 2154, 2155, 2156, 2157, 2158, 2159, 2160, 2161,
2162, 2163, 2164, 2165, 2166, 2167, 2168, 2169, 2170, 2171, 2172, 2173, 2174, 2175, 2176,
2225, 2229, 2226, 2246 };

static const int HersheyTriplex[] = {
(9 + 12*16) + FONT_HAVE_GREEK,
2199, 3214, 3228, 3232, 3219, 3233, 3218, 3227, 3221, 3222, 3223, 3225, 3211, 3224, 3210, 3220,
3200, 3201, 3202, 3203, 3204, 3205, 3206, 3207, 3208, 3209, 3212, 3213, 3230, 3226, 3231,
3215, 3234, 3001, 3002, 3003, 3004, 3005, 3006, 3007, 3008, 3009, 3010, 3011, 3012, 3013,
2014, 3015, 3016, 3017, 3018, 3019, 3020, 3021, 3022, 3023, 3024, 3025, 3026, 2223, 2084,
2224, 2247, 587, 2249, 3101, 3102, 3103, 3104, 3105, 3106, 3107, 3108, 3109, 3110, 3111,
3112, 3113, 3114, 3115, 3116, 3117, 3118, 3119, 3120, 3121, 3122, 3123, 3124, 3125, 3126,
2225, 2229, 2226, 2246 };

static const int HersheyTriplexItalic[] = {
(9 + 12*16) + FONT_ITALIC_ALPHA + FONT_ITALIC_DIGIT +
FONT_ITALIC_PUNCT + FONT_HAVE_GREEK,
2199, 3264, 3278, 3282, 3269, 3233, 3268, 3277, 3271, 3272, 3223, 3225, 3261, 3224, 3260, 3270,
3250, 3251, 3252, 3253, 3254, 3255, 3256, 3257, 3258, 3259, 3262, 3263, 3230, 3226, 3231,
3265, 3234, 3051, 3052, 3053, 3054, 3055, 3056, 3057, 3058, 3059, 3060, 3061, 3062, 3063,
2064, 3065, 3066, 3067, 3068, 3069, 3070, 3071, 3072, 3073, 3074, 3075, 3076, 2223, 2084,
2224, 2247, 587, 2249, 3151, 3152, 3153, 3154, 3155, 3156, 3157, 3158, 3159, 3160, 3161,
3162, 3163, 3164, 3165, 3166, 3167, 3168, 3169, 3170, 3171, 3172, 3173, 3174, 3175, 3176,
2225, 2229, 2226, 2246 };

static const int HersheyScriptSimplex[] = {
(9 + 12*16) + FONT_ITALIC_ALPHA + FONT_HAVE_GREEK,
2199, 714, 717, 733, 719, 697, 734, 716, 721, 722, 728, 725, 711, 724, 710, 720,
700, 701, 702, 703, 704, 705, 706, 707, 708, 709, 712, 713, 691, 726, 692,
715, 690, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563,
564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 693, 584,
694, 2247, 586, 2249, 651, 652, 653, 654, 655, 656, 657, 658, 659, 660, 661,
662, 663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673, 674, 675, 676,
695, 723, 696, 2246 };

static const int HersheyScriptComplex[] = {
(9 + 12*16) + FONT_ITALIC_ALPHA + FONT_ITALIC_DIGIT + FONT_ITALIC_PUNCT + FONT_HAVE_GREEK,
2199, 2764, 2778, 2782, 2769, 2783, 2768, 2777, 2771, 2772, 2219, 2232, 2211, 2231, 2210, 2220,
2750, 2751, 2752, 2753, 2754, 2755, 2756, 2757, 2758, 2759, 2212, 2213, 2241, 2238, 2242,
2215, 2273, 2551, 2552, 2553, 2554, 2555, 2556, 2557, 2558, 2559, 2560, 2561, 2562, 2563,
2564, 2565, 2566, 2567, 2568, 2569, 2570, 2571, 2572, 2573, 2574, 2575, 2576, 2223, 2084,
2224, 2247, 586, 2249, 2651, 2652, 2653, 2654, 2655, 2656, 2657, 2658, 2659, 2660, 2661,
2662, 2663, 2664, 2665, 2666, 2667, 2668, 2669, 2670, 2671, 2672, 2673, 2674, 2675, 2676,
2225, 2229, 2226, 2246 };


static const int* getFontData(enum CC_FONT_FACE fontFace)
{
    bool isItalic = (fontFace & CC_FONT_ITALIC) != 0;
    const int* ascii = 0;

    switch( fontFace )
    {
    case CC_FONT_HERSHEY_SIMPLEX:
        ascii = HersheySimplex;
        break;
    case CC_FONT_HERSHEY_PLAIN:
        ascii = !isItalic ? HersheyPlain : HersheyPlainItalic;
        break;
    case CC_FONT_HERSHEY_DUPLEX:
        ascii = HersheyDuplex;
        break;
    case CC_FONT_HERSHEY_COMPLEX:
        ascii = !isItalic ? HersheyComplex : HersheyComplexItalic;
        break;
    case CC_FONT_HERSHEY_TRIPLEX:
        ascii = !isItalic ? HersheyTriplex : HersheyTriplexItalic;
        break;
    case CC_FONT_HERSHEY_COMPLEX_SMALL:
        ascii = !isItalic ? HersheyComplexSmall : HersheyComplexSmallItalic;
        break;
    case CC_FONT_HERSHEY_SCRIPT_SIMPLEX:
        ascii = HersheyScriptSimplex;
        break;
    case CC_FONT_HERSHEY_SCRIPT_COMPLEX:
        ascii = HersheyScriptComplex;
        break;
    default:
        CC_Error( CC_StsOutOfRange, "Unknown font type" );
    }
    return ascii;
}

CC_INLINE void readCheck(int &c, int &i, const char* text, int fontFace)
{
    int leftBoundary = ' ', rightBoundary = 127;

    if(c >= 0x80 && fontFace == CC_FONT_HERSHEY_COMPLEX)
    {
        if(c == 0xD0 && (uchar)text[i + 1] >= 0x90 && (uchar)text[i + 1] <= 0xBF)
        {
            c = (uchar)text[++i] - 17;
            leftBoundary = 127;
            rightBoundary = 175;
        }
        else if(c == 0xD1 && (uchar)text[i + 1] >= 0x80 && (uchar)text[i + 1] <= 0x8F)
        {
            c = (uchar)text[++i] + 47;
            leftBoundary = 175;
            rightBoundary = 191;
        }
        else
        {
            if(c >= 0xC0 && text[i+1] != 0) //2 bytes utf
                i++;

            if(c >= 0xE0 && text[i+1] != 0) //3 bytes utf
                i++;

            if(c >= 0xF0 && text[i+1] != 0) //4 bytes utf
                i++;

            if(c >= 0xF8 && text[i+1] != 0) //5 bytes utf
                i++;

            if(c >= 0xFC && text[i+1] != 0) //6 bytes utf
                i++;

            c = '?';
        }
    }

    if(c >= rightBoundary || c < leftBoundary)
        c = '?';
}

//extern const char* g_HersheyGlyphs[];
#include "hershey_fonts.inl"

static void putText( img_t* img, const char* text, int text_size, CPoint org,
              enum CC_FONT_FACE fontFace, double fontScale, CScalar color,
              int thickness CC_DEFAULT(1), int line_type CC_DEFAULT(CC_LINE_8), bool bottomLeftOrigin CC_DEFAULT(false))

{
    CC_INSTRUMENT_REGION()

    if ( !text || (0==*text) )
    {
        return;
    }
    
    text_size = text_size < 0 ? strlen(text) : text_size;
    const int* ascii = getFontData(fontFace);

    double buf[4];
    cvScalarToRawData(&color, buf, CC_MAT_DEPTH(img), CC_MAT_CN(img), 0);

    int base_line = -(ascii[0] & 15);
    int hscale = cRound(fontScale*XY_ONE), vscale = hscale;

    if( line_type == CC_AA && CC_MAT_DEPTH(img) != CC_8U )
        line_type = 8;

    if( bottomLeftOrigin )
        vscale = -vscale;

    int view_x = (int)org.x << XY_SHIFT;
    int view_y = ((int)org.y << XY_SHIFT) + base_line*vscale;
    CPoint pts[1 << 10];
    const char **faces = g_HersheyGlyphs;

    for( int i = 0; i < text_size; i++ )
    {
        int c = (uchar)text[i];
        CPoint p;

        readCheck(c, i, text, fontFace);

        const char* ptr = faces[ascii[(c-' ')+1]];
        p.x = (uchar)ptr[0] - 'R';
        p.y = (uchar)ptr[1] - 'R';
        int dx = p.y*hscale;
        view_x -= p.x*hscale;
        int npts = (0);

        for( ptr += 2;; )
        {
            if( *ptr == ' ' || !*ptr )
            {
                if( npts > 1 )
                    PolyLine( img, &pts[0], npts, false, buf, thickness, line_type, XY_SHIFT );
                if( !*ptr++ )
                    break;
                npts = (0);
            }
            else
            {
                p.x = (uchar)ptr[0] - 'R';
                p.y = (uchar)ptr[1] - 'R';
                ptr += 2;
                pts[npts++] = cPoint(p.x*hscale + view_x, p.y*vscale + view_y);
            }
        }
        view_x += dx;
    }
}
static 
void putText( img_t* img, const char* text, CPoint org,
              enum CC_FONT_FACE fontFace, double fontScale, CScalar color,
              int thickness = 1, int line_type = CC_LINE_8, bool bottomLeftOrigin = false ) {
  putText( img, text, -1, org, fontFace, fontScale, color, thickness, line_type, bottomLeftOrigin);
}
static CSize getTextSize( const char* text, int text_size, enum CC_FONT_FACE fontFace, double fontScale, int thickness, int* _base_line = NULL)
{
    CSize size;
    double view_x = 0;
    const char **faces = g_HersheyGlyphs;
    const int* ascii = getFontData(fontFace);

    int base_line = (ascii[0] & 15);
    int cap_line = (ascii[0] >> 4) & 15;
    size.height = cRound((cap_line + base_line)*fontScale + (thickness+1)/2);
    text_size = text_size < 0 ? strlen(text) : text_size;

    for( int i = 0; i < (int)text_size; i++ )
    {
        int c = (uchar)text[i];
        CPoint p;

        readCheck(c, i, text, fontFace);

        const char* ptr = faces[ascii[(c-' ')+1]];
        p.x = (uchar)ptr[0] - 'R';
        p.y = (uchar)ptr[1] - 'R';
        view_x += (p.y - p.x)*fontScale;
    }

    size.width = cRound(view_x + thickness);
    if( _base_line )
        *_base_line = cRound(base_line*fontScale + thickness*0.5);
    return size;
}

static CSize getTextSize( const char* text, enum CC_FONT_FACE fontFace, double fontScale, int thickness, int* _base_line = NULL) {
  return getTextSize( text, -1, fontFace, fontScale, thickness, _base_line);
}
static void fillConvexPoly(img_t* img, const img_t* points,
                        const CScalar& color, int lineType, int shift)
{
    CC_INSTRUMENT_REGION()
    CC_Assert(cvCheckVector(points, 2, CC_32S) >= 0);
    fillConvexPoly(img, (CPoint*)img_ptr(int, points), points->rows*points->cols*CC_MAT_CN(points)/2, color, lineType, shift);
}
static 
void fillPoly(img_t* img, const img_t* pts, int ncontours, const CScalar& color, int lineType, int shift, CPoint offset)
{
    CC_INSTRUMENT_REGION()
    int i;
    if( ncontours == 0 )
        return;
    CPoint** ptsptr = MALLOC(CPoint*, ncontours);
    int* npts = MALLOC(int, ncontours);

    for( i = 0; i < ncontours; i++ )
    {
        const img_t* p = pts+i;
        CC_Assert(cvCheckVector(p, 2, CC_32S) >= 0);
        ptsptr[i] = (CPoint*)p->data;
        npts[i] = p->rows*p->cols*CC_MAT_CN(p)/2;
    }
    fillPoly(img, (const CPoint**)ptsptr, npts, (int)ncontours, color, lineType, shift, offset);
    FREE(ptsptr);
    FREE(npts);
}
static 
void polylines(img_t* img, const img_t* pts, int ncontours,
                   bool isClosed, const CScalar& color,
                   int thickness, int lineType, int shift )
{
    CC_INSTRUMENT_REGION()
    int i;
    if( ncontours == 0 )
        return;
    CPoint** ptsptr = MALLOC(CPoint*, ncontours);
    int* npts = MALLOC(int, ncontours);

    for( i = 0; i < ncontours; i++ )
    {
        const img_t* p = pts+i;
        if( CC_MAT_TOTAL(p) == 0 )
        {
            ptsptr[i] = NULL;
            npts[i] = 0;
            continue;
        }
        CC_Assert(cvCheckVector(p, 2, CC_32S) >= 0);
        ptsptr[i] = (CPoint*)p->data;
        npts[i] = p->rows*p->cols*CC_MAT_CN(p)/2;
    }
    polylines(img, (const CPoint**)ptsptr, npts, (int)ncontours, isClosed, color, thickness, lineType, shift);
}


static const int CodeDeltas[8][2] =
{ {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1} };

#define CC_ADJUST_EDGE_COUNT( count, seq )  \
    ((count) -= ((count) == (seq)->total && !CC_IS_SEQ_CLOSED(seq)))

#if 0
static void
cvDrawContours( void* _img, CvSeq* contour,
                CScalar _externalColor, CScalar _holeColor,
                int  maxLevel, int thickness,
                int line_type, CPoint _offset )
{
    CvSeq *contour0 = contour, *h_next = 0;
    CvTreeNodeIterator iterator;
    std::vector<CPolyEdge> edges;
    std::vector<CPoint> pts;
    CScalar externalColor = _externalColor, holeColor = _holeColor;
    
    CPoint offset = _offset;
    double ext_buf[4], hole_buf[4];

    if( line_type == CC_AA && CC_MAT_DEPTH(img) != CC_8U )
        line_type = 8;

    if( !contour )
        return;

    CC_Assert( thickness <= MAX_THICKNESS );

    scalarToRawData( externalColor, ext_buf, CC_MAT_DEPTH(img), 0 );
    scalarToRawData( holeColor, hole_buf, CC_MAT_DEPTH(img), 0 );

    maxLevel = MAX(maxLevel, INT_MIN+2);
    maxLevel = MIN(maxLevel, INT_MAX-1);

    if( maxLevel < 0 )
    {
        h_next = contour->h_next;
        contour->h_next = 0;
        maxLevel = -maxLevel+1;
    }

    cvInitTreeNodeIterator( &iterator, contour, maxLevel );
    while( (contour = (CvSeq*)cvNextTreeNode( &iterator )) != 0 )
    {
        CvSeqReader reader;
        int i, count = contour->total;
        int elem_type = CC_MAT_DEPTH(contour->flags);
        void* clr = (contour->flags & CC_SEQ_FLAG_HOLE) == 0 ? ext_buf : hole_buf;

        cvStartReadSeq( contour, &reader, 0 );
        if( thickness < 0 )
            pts.resize(0);

        if( CC_IS_SEQ_CHAIN_CONTOUR( contour ))
        {
            CPoint pt = ((CChain*)contour)->origin;
            CPoint prev_pt = pt;
            char prev_code = reader.ptr ? reader.ptr[0] : '\0';

            prev_pt += offset;

            for( i = 0; i < count; i++ )
            {
                char code;
                CC_READ_SEQ_ELEM( code, reader );

                assert( (code & ~7) == 0 );

                if( code != prev_code )
                {
                    prev_code = code;
                    if( thickness >= 0 )
                        ThickLine( img, prev_pt, pt, clr, thickness, line_type, 2, 0 );
                    else
                        pts.push_back(pt);
                    prev_pt = pt;
                }

                pt.x += CodeDeltas[(int)code][0];
                pt.y += CodeDeltas[(int)code][1];
            }

            if( thickness >= 0 )
                ThickLine( img, prev_pt,
                    cPoint(((CChain*)contour)->origin) + offset,
                    clr, thickness, line_type, 2, 0 );
            else
                CollectPolyEdges(img, &pts[0], (int)pts.size(),
                                     edges, ext_buf, line_type, 0, offset);
        }
        else if( CC_IS_SEQ_POLYLINE( contour ))
        {
            CC_Assert( elem_type == CC_32SC2 );
            CPoint pt1, pt2;
            int shift = 0;

            count -= !CC_IS_SEQ_CLOSED(contour);
            CC_READ_SEQ_ELEM( pt1, reader );
            pt1 += offset;
            if( thickness < 0 )
                pts.push_back(pt1);

            for( i = 0; i < count; i++ )
            {
                CC_READ_SEQ_ELEM( pt2, reader );
                pt2 += offset;
                if( thickness >= 0 )
                    ThickLine( img, pt1, pt2, clr, thickness, line_type, 2, shift );
                else
                    pts.push_back(pt2);
                pt1 = pt2;
            }
            if( thickness < 0 )
                CollectPolyEdges( img, &pts[0], (int)pts.size(),
                                      edges, ext_buf, line_type, 0, cPoint() );
        }
    }

    if( thickness < 0 )
        FillEdgeCollection( img, edges, ext_buf );

    if( h_next && contour0 )
        contour0->h_next = h_next;
}

static void addChildContour(const img_t* contours,
                            size_t ncontours,
                            const Vec4i* hierarchy,
                            int i, std::vector<CvSeq>& seq,
                            std::vector<CvSeqBlock>& block)
{
    for( ; i >= 0; i = hierarchy[i][0] )
    {
        const img_t* ci = contours+i;
        cvMakeSeqHeaderForArray(CC_SEQ_POLYGON, sizeof(CvSeq), sizeof(CPoint),
                                !ci.empty() ? (void*)ci->tt.data : 0, (int)ci CC_MAT_TOTAL(),
                                &seq[i], &block[i] );

        int h_next = hierarchy[i][0], h_prev = hierarchy[i][1],
            v_next = hierarchy[i][2], v_prev = hierarchy[i][3];
        seq[i].h_next = (0 <= h_next && h_next < (int)ncontours) ? &seq[h_next] : 0;
        seq[i].h_prev = (0 <= h_prev && h_prev < (int)ncontours) ? &seq[h_prev] : 0;
        seq[i].v_next = (0 <= v_next && v_next < (int)ncontours) ? &seq[v_next] : 0;
        seq[i].v_prev = (0 <= v_prev && v_prev < (int)ncontours) ? &seq[v_prev] : 0;

        if( v_next >= 0 )
            addChildContour(contours, ncontours, hierarchy, v_next, seq, block);
    }
}

static void drawContours( img_t* image, InputArrayOfArrays _contours,
                   int contourIdx, const CScalar& color, int thickness,
                   int lineType, const img_t* _hierarchy,
                   int maxLevel, CPoint offset )
{
    CC_INSTRUMENT_REGION()

    img_t image = _image, hierarchy = _hierarchy;
    img_t _cimage = image;

    size_t ncontours = _contours CC_MAT_TOTAL();
    size_t i = 0, first = 0, last = ncontours;
    std::vector<CvSeq> seq;
    std::vector<CvSeqBlock> block;

    if( !last )
        return;

    seq.resize(last);
    block.resize(last);

    for( i = first; i < last; i++ )
        seq[i].first = 0;

    if( contourIdx >= 0 )
    {
        CC_Assert( 0 <= contourIdx && contourIdx < (int)last );
        first = contourIdx;
        last = contourIdx + 1;
    }

    for( i = first; i < last; i++ )
    {
        img_t ci = _contours.getMat((int)i);
        if( ci.empty() )
            continue;
        int npoints = ci cvCheckVector(2, CC_32S);
        CC_Assert( npoints > 0 );
        cvMakeSeqHeaderForArray( CC_SEQ_POLYGON, sizeof(CvSeq), sizeof(CPoint),
                                 ci->tt.data, npoints, &seq[i], &block[i] );
    }

    if( hierarchy.empty() || maxLevel == 0 )
        for( i = first; i < last; i++ )
        {
            seq[i].h_next = i < last-1 ? &seq[i+1] : 0;
            seq[i].h_prev = i > first ? &seq[i-1] : 0;
        }
    else
    {
        size_t count = last - first;
        CC_Assert(hierarchy CC_MAT_TOTAL() == ncontours && hierarchy->tid == CC_32SC4 );
        const Vec4i* h = hierarchy.ptr<Vec4i>();

        if( count == ncontours )
        {
            for( i = first; i < last; i++ )
            {
                int h_next = h[i][0], h_prev = h[i][1],
                    v_next = h[i][2], v_prev = h[i][3];
                seq[i].h_next = (size_t)h_next < count ? &seq[h_next] : 0;
                seq[i].h_prev = (size_t)h_prev < count ? &seq[h_prev] : 0;
                seq[i].v_next = (size_t)v_next < count ? &seq[v_next] : 0;
                seq[i].v_prev = (size_t)v_prev < count ? &seq[v_prev] : 0;
            }
        }
        else
        {
            int child = h[first][2];
            if( child >= 0 )
            {
                addChildContour(_contours, ncontours, h, child, seq, block);
                seq[first].v_next = &seq[child];
            }
        }
    }

    cvDrawContours( &_cimage, &seq[first], color, color, contourIdx >= 0 ?
                   -maxLevel : maxLevel, thickness, lineType, offset );
}
#endif

static int
cvClipLine( int width, int height, CPoint* pt1, CPoint* pt2 )
{
    CC_Assert( pt1 && pt2 );
    return clipLine( width, height, *(CPoint*)pt1, *(CPoint*)pt2 );
}


static int
cvEllipse2Poly( CPoint center, CSize axes, int angle,
                int arc_start, int arc_end, CPoint* pts, int delta )
{
    return ellipse2Poly( center.x, center.y, axes.w, axes.h, angle, arc_start, arc_end, delta, pts );
}

static CScalar
cvColorToScalar( double packed_color, TypeId type, int cn )
{
    CScalar scalar;

    if( type == CC_8U )
    {
        int icolor = cRound( packed_color );
        if( cn > 1 )
        {
            scalar.val[0] = icolor & 255;
            scalar.val[1] = (icolor >> 8) & 255;
            scalar.val[2] = (icolor >> 16) & 255;
            scalar.val[3] = (icolor >> 24) & 255;
        }
        else
        {
            scalar.val[0] = int_to_uchar( icolor );
            scalar.val[1] = scalar.val[2] = scalar.val[3] = 0;
        }
    }
    else if( type == CC_8S )
    {
        int icolor = cRound( packed_color );
        if( cn > 1 )
        {
            scalar.val[0] = (char)icolor;
            scalar.val[1] = (char)(icolor >> 8);
            scalar.val[2] = (char)(icolor >> 16);
            scalar.val[3] = (char)(icolor >> 24);
        }
        else
        {
            scalar.val[0] = int_to_schar( icolor );
            scalar.val[1] = scalar.val[2] = scalar.val[3] = 0;
        }
    }
    else
    {
        switch( cn )
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

#if 0
static int
cvInitLineIterator( const img_t* img, CPoint pt1, CPoint pt2,
                    CLineIterator* iterator, int connectivity,
                    int left_to_right )
{
    CC_Assert( iterator != 0 );
    LineIterator li(cvarrToMat(img), pt1, pt2, connectivity, left_to_right!=0);

    iterator->err = li.err;
    iterator->minus_delta = li.minusDelta;
    iterator->plus_delta = li.plusDelta;
    iterator->minus_step = li.minusStep;
    iterator->plus_step = li.plusStep;
    iterator->ptr = li.ptr;

    return li.count;
}
#endif

static void
cvLine( img_t* img, CPoint pt1, CPoint pt2, CScalar color,
        int thickness, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    line( img, pt1, pt2, color, thickness, line_type, shift );
}

static void
cvRectangle( img_t* img, CPoint pt1, CPoint pt2,
             CScalar color, int thickness,
             int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    rectangle( img, pt1, pt2, color, thickness, line_type, shift );
}

static void
cvRectangleR( img_t* img, CRect rec,
              CScalar color, int thickness,
              int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    
    rectangle( img, rec, color, thickness, line_type, shift );
}

static void
cvCircle( img_t* img, CPoint center, int radius,
          CScalar color, int thickness, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    
    circle( img, center, radius, color, thickness, line_type, shift );
}

static void
cvEllipse( img_t* img, CPoint center, CSize axes,
           double angle, double start_angle, double end_angle,
           CScalar color, int thickness, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    
    ellipse( img, center.x, center.y, axes.w, axes.h, angle, start_angle, end_angle,
        color, thickness, line_type, shift );
}

static void
cvFillConvexPoly( img_t* img, const CPoint *pts, int npts,
                  CScalar color, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    
    fillConvexPoly( img, (const CPoint*)pts, npts,
                        color, line_type, shift );
}

static void
cvFillPoly( img_t* img, const CPoint **pts, const int *npts, int ncontours,
            CScalar color, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    fillPoly( img, (const CPoint**)pts, npts, (int)ncontours, color, line_type, shift, cPoint(0, 0) );
}

static void
cvPolyLine( img_t* img, CPoint **pts, const int *npts,
            int ncontours, int closed, CScalar color,
            int thickness, int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0) )
{
    polylines( img, (const CPoint**)pts, npts, ncontours,
                   closed != 0, color, thickness, line_type, shift );
}

#if 0
static void
cvPutText( img_t* img, const char *text, CPoint org, const CFont *_font, CScalar color )
{
    CC_Assert( text != 0 && _font != 0);
    putText( img, text, org, _font->font_face, (_font->hscale+_font->vscale)*0.5,
                color, _font->thickness, _font->line_type,
                CC_IS_IMAGE(_img) && ((img_t*)_img)->origin != 0 );
}

static void
cvInitFont( CFont *font, int font_face, double hscale, double vscale,
            double shear, int thickness, int line_type )
{
    CC_Assert( font != 0 && hscale > 0 && vscale > 0 && thickness >= 0 );

    font->ascii = getFontData(font_face);
    font->font_face = font_face;
    font->hscale = (float)hscale;
    font->vscale = (float)vscale;
    font->thickness = thickness;
    font->shear = (float)shear;
    font->greek = font->cyrillic = 0;
    font->line_type = line_type;
}

static void
cvGetTextSize( const char *text, const CFont *_font, CSize *_size, int *_base_line )
{
    CC_Assert(text != 0 && _font != 0);
    CSize size = getTextSize( text, _font->font_face, (_font->hscale + _font->vscale)*0.5,
                                     _font->thickness, _base_line );
    if( _size )
        *_size = size;
}
#endif

#endif // _DRAWING_INL_
