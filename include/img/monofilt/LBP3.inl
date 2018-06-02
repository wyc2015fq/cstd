

int copy_replicate_border(int src_h, int src_w, const void* src0, int srcstep,
                           int dst_h, int dst_w, void* dst0, int dststep, int cn,
                           int top1, int left1, int fillzero)
{
  int i, j, isrc=0, idst=0;
  int left = (left1)*(cn), top = top1;
  int src_width = (src_w)*(cn), src_height = src_h;
  int dst_width = (dst_w)*(cn), dst_height = dst_h;
  const uchar * src = (const uchar *)src0;
  uchar * dst = (uchar *)dst0;
  for( i = 0; i < dst_height; i++, idst += (dststep) ) {
    if (fillzero) {
      if( i >= top && i < top + src_height - 1 ) {
        memcpy( dst + idst + left, src + isrc, (src_width)*sizeof(*(src)) );
      } else {
        memset( dst + idst + left, 0, (src_width)*sizeof(*(src)) );
      }
      for( j = left - 1; j >= 0; j-- )
        *(dst+idst+j) = 0;
      for( j = left + src_width; j < (dst_width); j++ )
        *(dst+idst+j) = 0;
    } else {
      memcpy( dst + idst + left, src + isrc, (src_width)*sizeof(*(src)) );
      //COPY( (src_width), (src + isrc), (dst + idst + left) );
      for( j = left - 1; j >= 0; j-- )
        *(dst+idst+j) = *(dst+idst+j + cn);
      for( j = left + src_width; j < (dst_width); j++ )
        *(dst+idst+j) = *(dst+idst+j - cn);
    }
    if( i >= top && i < top + src_height - 1 )
      isrc += srcstep;
  }
  return 0;
}

#include "complex.inl"

typedef unsigned char u8;
typedef double f64;
#define CONCAT(A, B)  A##B

#define TYPE u8
#define skip_lbp skip_lbp_u8
#define LBP3 LBP3_u8
#include "skip_lbp.inl"

#define TYPE f64
#define skip_lbp skip_lbp_f64
#define LBP3 LBP3_f64
#include "skip_lbp.inl"
//#include "cxdxt.inl"
