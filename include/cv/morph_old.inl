#ifndef _MORPH_INL_
#define _MORPH_INL_


static int morph_element_init(int cols, int rows, int element_shape, IPOINT anchor, uchar* element, int step, IPOINT* el_sparse)
{
  int i, j, k=0;
  int r = 0, c = 0;
  double inv_r2 = 0;
  
  if(anchor.x == -1)
    anchor.x = cols/2;
  
  if(anchor.y == -1)
    anchor.y = rows/2;
  
  if((unsigned)anchor.x >= (unsigned)cols || (unsigned)anchor.y >= (unsigned)rows)
    CC_ERROR(CC_StsOutOfRange, "anchor is outside of element");
  
  if(element_shape != CC_MORPH_RECT && element_shape != CC_MORPH_CROSS && element_shape != CC_MORPH_ELLIPSE)
    CC_ERROR(CC_StsBadArg, "Unknown/unsupported element shape");
  
  if(rows == 1 || cols == 1)
    element_shape = CC_MORPH_RECT;
  
  if(element_shape == CC_MORPH_ELLIPSE)
  {
    r = rows/2;
    c = cols/2;
    inv_r2 = r ? 1./((double)r*r) : 0;
  }
  
  for(i = 0; i < rows; i++)
  {
    uchar* ptr = element + i*step;
    int j1 = 0, j2 = 0, jx, t = 0;
    
    if(element_shape == CC_MORPH_RECT || (element_shape == CC_MORPH_CROSS && i == anchor.y)) {
      j2 = cols;
    } else if(element_shape == CC_MORPH_CROSS) {
      j1 = anchor.x, j2 = j1 + 1;
    } else {
      int dy = i - r;
      if(abs(dy) <= r) {
        int dx = cRound(c*sqrt(((double)r*r - dy*dy)*inv_r2));
        j1 = MAX(c - dx, 0);
        j2 = MIN(c + dx + 1, cols);
      }
    }
    
    for(j = 0, jx = j1; j < cols;) {
      for(; j < jx; j++) {
        if (element) {
          ptr[j] = (uchar)t;
        }
        if (t) {
          if (el_sparse) {
            el_sparse[k] = iPOINT(j-anchor.x, i-anchor.y);
          }
          ++k;
        }
      }
      if(jx == j2)
        jx = cols, t = 0;
      else
        jx = j2, t = 1;
    }
  }
  return k;
}


/****************************************************************************************\
                     Basic Morphological Operations: Erosion & Dilation
\****************************************************************************************/

// ICC_MORPH_RECT_ROW(Erode, 8u, uchar, int, CC_CALC_MIN_8U)
// ICC_MORPH_RECT_ROW(Dilate, 8u, uchar, int, CC_CALC_MAX_8U)
// ICC_MORPH_RECT_ROW(Erode, 32f, int, int, CC_CALC_MIN)
// ICC_MORPH_RECT_ROW(Dilate, 32f, int, int, CC_CALC_MAX)
// 
// ICC_MORPH_RECT_COL(Erode, 8u, uchar, int, CC_CALC_MIN_8U, CC_NOP)
// ICC_MORPH_RECT_COL(Dilate, 8u, uchar, int, CC_CALC_MAX_8U, CC_NOP)
// ICC_MORPH_RECT_COL(Erode, 32f, int, int, CC_CALC_MIN, CC_TOGGLE_FLT)
// ICC_MORPH_RECT_COL(Dilate, 32f, int, int, CC_CALC_MAX, CC_TOGGLE_FLT)


// static CStatus icvErodeRectRow_8u(const uchar* src, uchar* dst, CvMorphology* state);
// static CStatus icvErodeRectRow_32f(const int* src, int* dst, CvMorphology* state);
// static CStatus icvDilateRectRow_8u(const uchar* src, uchar* dst, CvMorphology* state);
// static CStatus icvDilateRectRow_32f(const int* src, int* dst, CvMorphology* state);
// 
// static CStatus icvErodeRectCol_8u(const uchar* src, int srcstep, uchar* dst, int dststep,int count, CvMorphology* state);
// static CStatus icvErodeRectCol_32f(const int* src, int srcstep, int* dst, int dststep,int count, CvMorphology* state);
// static CStatus icvDilateRectCol_8u(const uchar* src, int srcstep, uchar* dst, int dststep,int count, CvMorphology* state);
// static CStatus icvDilateRectCol_32f(const int* src, int srcstep, int* dst, int dststep,int count, CvMorphology* state);

//#define icv_morph_any(name, flavor, arrtype, worktype, update_extr_macro, toggle_macro)
//static CStatus icv##name##Any_##flavor(const arrtype* src, int srcstep, arrtype* dst, int dststep, int count, CvMorphology* state)

// ICC_MORPH_ANY(Erode, 8u, uchar, int, CC_CALC_MIN, CC_NOP)
// ICC_MORPH_ANY(Dilate, 8u, uchar, int, CC_CALC_MAX, CC_NOP)
// ICC_MORPH_ANY(Erode, 32f, int, int, CC_CALC_MIN, CC_TOGGLE_FLT)
// ICC_MORPH_ANY(Dilate, 32f, int, int, CC_CALC_MAX, CC_TOGGLE_FLT)


static CStatus cvErode_8u(const uchar* src, int srcstep, uchar* dst, int dststep, int cn, CSize size, CSize ksize, int element_shape, IPOINT anchor, int borderType) {
  typedef uchar arrtype;
  //typedef int worktype;
#define update_extr_macro CC_CALC_MIN
#define toggle_macro CC_NOP
#include "impl/icv_morph_any.inl"
  return CC_OK;
}

static CStatus cvDilate_8u(const uchar* src, int srcstep, uchar* dst, int dststep, int cn, CSize size, CSize ksize, int element_shape, IPOINT anchor, int borderType) {
  typedef uchar arrtype;
  //typedef int worktype;
#define update_extr_macro CC_CALC_MAX
#define toggle_macro CC_NOP
#include "impl/icv_morph_any.inl"
  return CC_OK;
}

#if 1
static int imerode(const img_t* im, img_t* im2, CSize ksize, int elem_shape, IPOINT anchor, int borderType) {
  //imsetsize();
  //SKIP_FILTER_RC( h, w, xx0, xxl, 1, xx, w, 1, 3, 3, zz, GAUSS_I7X7, GAUSS_I7X7 );
  img_t im1[1] = {0};
  CSize size;
  if (im==im2) {
    imclone2(im, im1);
    im = im1;
  }
  imsetsize(im2, im->h, im->w, im->c, 1);
  size = iSIZE(im->w, im->h);
  cvErode_8u(im->tt.data, im->s, im2->tt.data, im2->s, im2->c, size, ksize, elem_shape, anchor, borderType);
  imfree(im1);
  return 0;
}
static int imdilate(const img_t* im, img_t* im2, CSize ksize, int elem_shape, IPOINT anchor, int borderType) {
  //imsetsize();
  //SKIP_FILTER_RC( h, w, xx0, xxl, 1, xx, w, 1, 3, 3, zz, GAUSS_I7X7, GAUSS_I7X7 );
  img_t im1[1] = {0};
  CSize size;
  if (im==im2) {
    imclone2(im, im1);
    im = im1;
  }
  imsetsize(im2, im->h, im->w, im->c, 1);
  size = cSize(im->w, im->h);
  cvDilate_8u(im->tt.data, im->s, im2->tt.data, im2->s, im2->c, size, ksize, elem_shape, anchor, borderType);
  imfree(im1);
  return 0;
}
#endif


#if 0
// static CStatus icvErodeAny_32f(const int* src, int srcstep, int* dst, int dststep,int count, CvMorphology* state);
// static CStatus icvDilateAny_8u(const uchar* src, int srcstep, uchar* dst, int dststep,int count, CvMorphology* state);
// static CStatus icvDilateAny_32f(const int* src, int srcstep, int* dst, int dststep,int count, CvMorphology* state);

int CvMorphology::fill_cyclic_buffer(const uchar* src, int src_step,
                                      int y0, int y1, int y2)
{
    int i, y = y0, bsz1 = border_tab_sz1, bsz = border_tab_sz;
    int pix_size = CC_ELEM_SIZE(src_type);
    int width_n = (prev_x_range.end_index - prev_x_range.start_index)*pix_size;

    if(src_type != CC_32F)
        return CvBaseImageFilter::fill_cyclic_buffer(src, src_step, y0, y1, y2);

    // fill the cyclic buffer
    for(; buf_count < buf_max_count && y < y2; buf_count++, y++, src += src_step)
    {
        uchar* trow = is_separable ? buf_end : buf_tail;

        for(i = 0; i < width_n; i += sizeof(int))
        {
            int t = *(int*)(src + i);
            *(int*)(trow + i + bsz1) = CC_TOGGLE_FLT(t);
        }

        if(border_mode != IMG_BORDER_CONSTANT)
        {
            for(i = 0; i < bsz1; i++)
            {
                int j = border_tab[i];
                trow[i] = trow[j];
            }
            for(; i < bsz; i++)
            {
                int j = border_tab[i];
                trow[i + width_n] = trow[j];
            }
        }
        else
        {
            const uchar *bt = (uchar*)border_tab; 
            for(i = 0; i < bsz1; i++)
                trow[i] = bt[i];

            for(; i < bsz; i++)
                trow[i + width_n] = bt[i];
        }

        if(is_separable)
            x_func(trow, buf_tail, this);

        buf_tail += buf_step;
        if(buf_tail >= buf_end)
            buf_tail = buf_start;
    }

    return y - y0;
}

/////////////////////////////////// External Interface /////////////////////////////////////


CC_IMPL void cvMorphologyEx(const void* src, void* dst,
                void* temp, IplConvKernel* element, int op, int iterations)
{
    CC_FUNCNAME("cvMorhologyEx");

    __BEGIN__;

    if((op == CC_MOP_GRADIENT ||
        (op == CC_MOP_TOPHAT || op == CC_MOP_BLACKHAT) && src == dst) && temp == 0)
        CC_ERROR(CC_HeaderIsNull, "temp image required");

    if(temp == src || temp == dst)
        CC_ERROR(CC_HeaderIsNull, "temp image is equal to src or dst");

    switch (op)
    {
    case CC_MOP_OPEN:
        CC_CALL(cvErode(src, dst, element, iterations));
        CC_CALL(cvDilate(dst, dst, element, iterations));
        break;
    case CC_MOP_CLOSE:
        CC_CALL(cvDilate(src, dst, element, iterations));
        CC_CALL(cvErode(dst, dst, element, iterations));
        break;
    case CC_MOP_GRADIENT:
        CC_CALL(cvErode(src, temp, element, iterations));
        CC_CALL(cvDilate(src, dst, element, iterations));
        CC_CALL(cvSub(dst, temp, dst));
        break;
    case CC_MOP_TOPHAT:
        if(src != dst)
            temp = dst;
        CC_CALL(cvErode(src, temp, element, iterations));
        CC_CALL(cvDilate(temp, temp, element, iterations));
        CC_CALL(cvSub(src, temp, dst));
        break;
    case CC_MOP_BLACKHAT:
        if(src != dst)
            temp = dst;
        CC_CALL(cvDilate(src, temp, element, iterations));
        CC_CALL(cvErode(temp, temp, element, iterations));
        CC_CALL(cvSub(temp, src, dst));
        break;
    default:
        CC_ERROR(CC_StsBadArg, "unknown morphological operation");
    }

    __END__;
}

#endif



#endif // _MORPH_INL_
