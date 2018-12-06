
#define MAP_AND_MUL(_N, _F, _P, _M, _W)                                    \
do {                                                                       \
  int _I;                                                                  \
  for (_I = 0; _I < _N; ++_I) {                                            \
    *(_F + _I) = _P[*(_M + _I)];/* *(*w++); */                             \
  }                                                                        \
} while(0)

#define EVELV(_A, a, b, c, d)   (*(_A+umap[a]) - *(_A+umap[b]) - *(_A+umap[c]) + *(_A+umap[d]))

// lbp滤波算子
#define LBP_EX_FT(_A, srcstep, cn, B)                                      \
do {                                                                       \
    unsigned char p = (unsigned char)0;                                    \
    int v5 = (int)EVELV(_A, 5, 6, 9, 10);                                  \
    p |= (unsigned char)( EVELV(_A,   0,  1,  4,  5) >= v5 ); p <<= 1;     \
    p |= (unsigned char)( EVELV(_A,   1,  2,  5,  6) >= v5 ); p <<= 1;     \
    p |= (unsigned char)( EVELV(_A,   2,  3,  6,  7) >= v5 ); p <<= 1;     \
    p |= (unsigned char)( EVELV(_A,   6,  7, 10, 11) >= v5 ); p <<= 1;     \
    p |= (unsigned char)( EVELV(_A,  10, 11, 14, 15) >= v5 ); p <<= 1;     \
    p |= (unsigned char)( EVELV(_A,   9, 10, 13, 14) >= v5 ); p <<= 1;     \
    p |= (unsigned char)( EVELV(_A,   8,  9, 12, 13) >= v5 ); p <<= 1;     \
    p |= (unsigned char)( EVELV(_A,   4,  5,  8,  9) >= v5 );              \
    *(B) = __lbpmap0[p];                                                   \
} while(0)

// 初始化滤波算子
/*  0,  1,  2,  3, */
/*  4,  5,  6,  7, */
/*  8,  9, 10, 11, */
/* 12, 13, 14, 15, */
void lbp_ex_init( int k0, int al, int ai, int* u ) {
  int i;
  int k2 = k0 + k0 + 1;
  int pos = -( k0 + k2 ) * ( ai + al );
  ai *= k2;
  al *= k2;
  for ( i = 0; i < 4; ++i, pos += al ) {
    u[ i * 4 + 0 ] = pos;
    u[ i * 4 + 1 ] = pos + ai;
    u[ i * 4 + 2 ] = pos + ai + ai;
    u[ i * 4 + 3 ] = pos + ai + ai + ai;
  }
}



/* 最高位最多乘2^7，所以<<操作最多7次， */
/* 最后一次按位与即可完成+1或+0的操作(即1*2^0或者0*2^0)。 */
#define SKIP_BLOCK_HIST(h, w, src, srcstep, cn, B, bin, rect, n)              \
do {                                                                          \
  int _N=(n)*bin, _bin;                                                       \
  for (_bin=0; _bin<_N; rect+=4, _bin+=bin) {                                 \
    int y = rect[0], x = rect[1], cy=rect[2], cx=rect[3];                     \
    int pos = y*(srcstep)+x*(cn);                                             \
    SKIP_HIST(cy, cx, (src+pos), srcstep, cn, (B+_bin));                      \
  }                                                                           \
} while(0)

#define INT2 unsigned char
#define INTINT real

// SKIP_FILTER(srch, srcw, y, yl, 1, img2, yw, 1, LBP_EX_FT);
void skip_filter_lbp_ex_ft( int srch, int srcw, const int *y, int yl, unsigned char* img2, int yw, const int* umap, const unsigned char* __lbpmap0 ) {
  int A0, A1, _B0, B1;
  for ( A1 = 0, B1 = 0; A1 < ( ( srch ) * ( yl ) ); A1 += ( yl ), B1 += ( yw ) ) {
    for ( A0 = A1, _B0 = B1; A0 < ( srcw ) * ( 1 ) + A1; A0 += ( 1 ), _B0 += ( 1 ) ) {
      //LBP_EX_FT((y+A0), yl, 1, (img2+_B0));
#define EVELV(_A, a, b, c, d)   (*(_A+umap[a]) - *(_A+umap[b]) - *(_A+umap[c]) + *(_A+umap[d]))

      // lbp滤波算子
      {
        unsigned char p = ( unsigned char ) 0;
        const int* yy = ( y + A0 );
        int v1, v2, v3, v4, v5, v6, v7, v8, v9;
#if 1

        v5 = ( EVELV( yy, 5, 6, 9, 10 ) );
        v1 = ( EVELV( yy, 0, 1, 4, 5 ) >= v5 ) << 7;
        v2 = ( EVELV( yy, 1, 2, 5, 6 ) >= v5 ) << 6;
        v3 = ( EVELV( yy, 2, 3, 6, 7 ) >= v5 ) << 5;
        v4 = ( EVELV( yy, 6, 7, 10, 11 ) >= v5 ) << 4;
        v6 = ( EVELV( yy, 10, 11, 14, 15 ) >= v5 ) << 3;
        v7 = ( EVELV( yy, 9, 10, 13, 14 ) >= v5 ) << 2;
        v8 = ( EVELV( yy, 8, 9, 12, 13 ) >= v5 ) << 1;
        v9 = ( EVELV( yy, 4, 5, 8, 9 ) >= v5 );
#else

        int _A[ 16 ];
        _A[ 0 ] = *( yy + umap[ 0 ] );
        _A[ 1 ] = *( yy + umap[ 1 ] );
        _A[ 2 ] = *( yy + umap[ 2 ] );
        _A[ 3 ] = *( yy + umap[ 3 ] );
        _A[ 4 ] = *( yy + umap[ 4 ] );
        _A[ 5 ] = *( yy + umap[ 5 ] );
        _A[ 6 ] = *( yy + umap[ 6 ] );
        _A[ 7 ] = *( yy + umap[ 7 ] );
        _A[ 8 ] = *( yy + umap[ 8 ] );
        _A[ 9 ] = *( yy + umap[ 9 ] );
        _A[ 10 ] = *( yy + umap[ 10 ] );
        _A[ 11 ] = *( yy + umap[ 11 ] );
        _A[ 12 ] = *( yy + umap[ 12 ] );
        _A[ 13 ] = *( yy + umap[ 13 ] );
        _A[ 14 ] = *( yy + umap[ 14 ] );
        _A[ 15 ] = *( yy + umap[ 15 ] );
#define EVELV1(_A, a, b, c, d)   (_A[a] - _A[b] - _A[c] + _A[d])

        v5 = ( EVELV1( _A, 5, 6, 9, 10 ) );
        v1 = ( EVELV1( _A, 0, 1, 4, 5 ) >= v5 ) << 7;
        v2 = ( EVELV1( _A, 1, 2, 5, 6 ) >= v5 ) << 6;
        v3 = ( EVELV1( _A, 2, 3, 6, 7 ) >= v5 ) << 5;
        v4 = ( EVELV1( _A, 6, 7, 10, 11 ) >= v5 ) << 4;
        v6 = ( EVELV1( _A, 10, 11, 14, 15 ) >= v5 ) << 3;
        v7 = ( EVELV1( _A, 9, 10, 13, 14 ) >= v5 ) << 2;
        v8 = ( EVELV1( _A, 8, 9, 12, 13 ) >= v5 ) << 1;
        v9 = ( EVELV1( _A, 4, 5, 8, 9 ) >= v5 );
#endif

        p = ( unsigned char ) ( v1 | v2 | v3 | v4 | v6 | v7 | v8 | v9 );
        //*(img2+_B0) = p;
        *( img2 + _B0 ) = __lbpmap0[ p ];
      }

    }
  }
}
#undef EVELV
#undef EVELV1

//
// 多bin的lbp滤波
int lbp_ex_ft_all( int srch, int srcw, const INT2* src, int srcstep, int cn, FEAT_T* me, int fillborder, int select_type ) {
  FEAT_T * it = me;
  int i, yh = srch + 1, yw = srcw + 1;
  int ysize = yh * yw;
  unsigned char* img2;
  int exx = ( ( 2 * FN - 1 ) * 3 );
  int yl = yw + exx;
  int aah = srch + exx, aaw = srcw + exx;
  int aal = aaw;
  int* y1, * y;
  int exx2 = ( exx / 2 );
  int umap[ 16 ];
  int all_size = init_size(select_type);
  y1 = MALLOC( int, ( yh + exx ) * ( yl ) );
  MEMSET( y1, 0, ( yh + exx ) * ( yl ) );
  y = y1 + exx2 * yl + exx2;

  if (NULL==src) return all_size;
  {
    //FPRINT2D(".\\src.txt", "%d\n", srch, srcw, src, srcstep, 1);
    if ( fillborder ) {
      INT2 * AA;
      AA = MALLOC( INT2, ( aah ) * ( aal )*2 );
      MEMSET( AA, 0, ( aah ) * ( aal ) );
      COPY_REPLICATE_BORDER( srch, srcw, src, srcstep, aah, aaw, AA, aal, 1, exx2, exx2 );
      //PRINT(aah, aaw, Asrc, aal, 1, ".\\Asrc.txt", "%d\n");
      //SKIP_COPY(srch, srcw, src, srcstep, 1, Asrc, aal, 1);
      //SKIP_COPYEX(srch, srcw, src, srcstep, 1, exx, exx, Asrc, aal, 1);
      SKIP_INTEGRAL( aah, aaw, AA, aal, 1, y1, yl, 1 );
      if ( AA != ( INT2* ) me ) {
        FREE( AA );
      } else {
        MEMSET( AA, 0, ( aah ) * ( aal ) );
      }
    } else {
      int* y2 = y1 + yl * exx2 + exx2;
      SKIP_INTEGRAL( srch, srcw, src, srcstep, 1, y2, yl, 1 );
      COPY_REPLICATE_BORDER( srch + 1, srcw + 1, y2, yl, ( yh + exx ), yl, y1, yl, 1, exx2, exx2 );
    }
  }
  img2 = MALLOC( unsigned char, ysize );
  //Mati& me = *this;
  for ( i = MINFN; i < (int)FN; ++i ) {
    //int ex=(2*i+1)*3+1;
    //int eex = ex*1;
    const unsigned char* __lbpmap0 = lbpmap[ i ];
    lbp_ex_init( i, yl, 1, umap );
    MEMSET( img2, 0, ysize );
#if 0

    {
      utime_start( _start_time );
      int i;
      for ( i = 40; i--; ) {
        SKIP_FILTER( srch, srcw, y, yl, 1, img2, srcw, 1, LBP_EX_FT );
      }
      printf( "time = %f\n", utime_elapsed( _start_time ) );
    }
#else

    {
      //utime_start(_start_time);
      skip_filter_lbp_ex_ft( srch, srcw, y, yl, img2, srcw, umap, __lbpmap0 );
      //printf("time = %f\n", utime_elapsed(_start_time));
    }
    //FPRINT2D("F:\\public\\test.txt", "%3d, ", 10, 10, img2+0*srcw+40, srcw, 1);
    //FPRINT2D("F:\\public\\test_lbp.txt", "%3d, ", 10, 10, img2+20*srcw+20, srcw, 1);
    //imwrite(".\\test_lbp.bmp", srch, srcw, img2, srcw, 1);
#endif
    // SKIP_BLOCK_HIST(srch, srcw, img2, yw, 1, it, bin, rect, len);
    // SKIP_BLOCK_HIST(h, w, src, srcstep, cn, B, bin, rect, n)
    {
      int _bin;
      lbp_Feature_List_T* lbp_Feature_List=get_lbp_Feature_List(select_type);
      int bin = lbp_Feature_List[ i ].bin;
      int len = lbp_Feature_List[ i ].len;
      const unsigned char* rect = lbp_Feature_List[ i ].ptr;
      //utime_start(_start_time);
      for ( _bin = 0; _bin < len; ++_bin, rect += 4, it += bin ) {
        int y = rect[ 0 ], x = rect[ 1 ], cy = rect[ 2 ], cx = rect[ 3 ];
        unsigned char* pos = img2 + y * ( srcw ) + x;
        memset(it, 0, sizeof(FEAT_T)*64);
        SKIP_HIST( cy, cx, ( pos ), srcw, 1, ( it ) );
        //assert(y+cy<51);
        //if (17==me[482]) {
        //FPRINT2D("F:\\public\\test.txt", "%3d, ", cy, cx, pos, srcw, 1);
        //}
      }
      //printf("time = %f\n", utime_elapsed(_start_time));
      //printf("\n");
      //it += bin*len;
    }
  }
  FREE( y1 );
  FREE( img2 );
  return all_size;
}


// 多bin的lbp滤波 lbp_ex_ft_all
int lbp_ex_ft_16s( int srch, int srcw, const INT2* src, int srcstep, int cn, FEAT_T* me,
                  int isall, int fillborder, int lbp_Feature_idyw_size, const int* lbp_Feature_idyw, int select_type ) {
  int it = 0;
  int i, yh = srch + 1, yw = srcw + 1;
  int ysize = yh * yw;
  unsigned char* img2;
  int exx = ( ( 2 * FN - 1 ) * 3 );
  int yl = yw + exx;
  int aah = srch + exx, aaw = srcw + exx;
  int aal = aaw;
  int* y1, * y;
  int exx2 = ( exx / 2 );
  int umap[ 16 ];
  int it2[ 64 ];
  int iw = 0;
  int retsize = isall ? init_size(select_type) : lbp_Feature_idyw_size;
  lbp_Feature_List_T* lbp_Feature_List=get_lbp_Feature_List(select_type);
  if ( !me ) {
    return retsize;
  }
  //utime_start(_start_time);
  y1 = MALLOC( int, ( yh + exx ) * ( yl ) );
  MEMSET( y1, 0, ( yh + exx ) * ( yl ) );
  y = y1 + exx2 * yl + exx2;

  {
    //FPRINT2D(".\\src.txt", "%d\n", srch, srcw, src, srcstep, 1);
    if ( fillborder ) {
      INT2 * AA;
      AA = MALLOC( INT2, ( aah ) * ( aal ) );
      MEMSET( AA, 0, ( aah ) * ( aal ) );
      COPY_REPLICATE_BORDER( srch, srcw, src, srcstep, aah, aaw, AA, aal, 1, exx2, exx2 );
      //PRINT(aah, aaw, Asrc, aal, 1, ".\\Asrc.txt", "%d\n");
      SKIP_INTEGRAL( aah, aaw, AA, aal, 1, y1, yl, 1 );
      FREE( AA );
    } else {
      int* y2 = y1 + yl * exx2 + exx2;
      SKIP_INTEGRAL( srch, srcw, src, srcstep, 1, y2, yl, 1 );
      COPY_REPLICATE_BORDER( srch + 1, srcw + 1, y2, yl, ( yh + exx ), yl, y1, yl, 1, exx2, exx2 );
    }
  }
  img2 = MALLOC( unsigned char, ysize );
  //Mati& me = *this;
  for ( i = MINFN; i < (int)FN; ++i ) {
    //int ex = ( 2 * i + 1 ) * 3 + 1;
    int bin = lbp_Feature_List[ i ].bin;
    int len = lbp_Feature_List[ i ].len;
    const unsigned char* rect = lbp_Feature_List[ i ].ptr;
    //int eex = ex*1;
    const unsigned char* __lbpmap0 = lbpmap[ i ];
    lbp_ex_init( i, yl, 1, umap );
    MEMSET( img2, 0, ysize );
#if 0

    SKIP_FILTER( srch, srcw, y, yl, 1, img2, yw, 1, LBP_EX_FT );
#else

    skip_filter_lbp_ex_ft( srch, srcw, y, yl, img2, yw, umap, __lbpmap0 );
#endif

    if ( isall ) {
      //SKIP_BLOCK_HIST(srch, srcw, img2, yw, 1, it, bin, rect, len);
      int n = ( len ) * bin, _bin;
      for ( _bin = 0; _bin < n; rect += 4, _bin += bin ) {
        int y = rect[ 0 ], x = rect[ 1 ], cy = rect[ 2 ], cx = rect[ 3 ];
        int pos = y * ( yw ) + x * ( cn );
        SKIP_HIST( cy, cx, ( src + pos ), yw, 1, ( me + _bin ) );
      }
      me += n;
    } else {
      int _bin;
      for ( _bin = 0; _bin < len && iw < lbp_Feature_idyw_size; ++_bin, rect += 4, it += bin ) {
        int id = lbp_Feature_idyw_sorted[ iw ];
        int itend = it + bin;
        if ( id >= it && id < itend ) {
          int y = rect[ 0 ], x = rect[ 1 ], cy = rect[ 2 ], cx = rect[ 3 ];
          unsigned char* pos = img2 + y * ( yw ) + x;
          MEMSET( it2, 0, 64 );
          SKIP_HIST( cy, cx, ( pos ), yw, 1, ( it2 ) );
          for ( ; ( id = lbp_Feature_idyw_sorted[ iw ] ) < itend && iw < lbp_Feature_idyw_size; ++iw ) {
            me[ iw ] = it2[ id - it ];
          }
        }
      }
    }
  }
  FREE( img2 );
  FREE( y1 );
  //printf("%f\n", utime_elapsed(_start_time));
  return retsize;
}

static const unsigned char lbp_Feature_List_3_2[] = {
#include "./txt/lbp_Feature_List_3_2.txt"
//#include "d:/code/cstd/src/monofilt/featurelist3_10x10.txt"
};

#define LBP59_FT(src, srcstep, cn, B)  do { LBP_FT(src, srcstep, cn, B); *(B) = lbpmap[0][*(B)]; } while(0)

int new_rect_lbp_hist_feature( int srch, int srcw, const INT2* src, int srcstep, int cn, FEAT_T* me, int fillborder ) {
  int bin = 59;
  int len = sizeof( lbp_Feature_List_3_2 ) / ( sizeof( lbp_Feature_List_3_2[ 0 ] ) * 4 );
  const unsigned char* ptr = lbp_Feature_List_3_2;
  if ( me ) {
    FEAT_T * it = ( FEAT_T* ) me;
    int yh = srch, yw = srcw;
    int ysize = yh * yw;
    INT2* img2;
    int exx = 1;
    //int yl = yw + exx;
    int aah = srch + exx + exx, aaw = srcw + exx + exx;
    int aal = aaw;
    INT2* AA;
    //int exx2 = ( exx / 2 );
    AA = MALLOC( INT2, ( aah ) * ( aal ) );
    img2 = MALLOC( INT2, ysize );
    MEMSET( AA, 0, ( aah ) * ( aal ) );
    MEMSET( img2, 0, ysize );
    {
      COPY_REPLICATE_BORDER( srch, srcw, src, srcstep, aah, aaw, AA, aal, 1, exx, exx );
      //SKIP_FILTER( srch, srcw, ( AA + aal + 1 ), aal, 1, img2, yw, 1, LBP_FT );
      //FPRINT2D("lbp.txt", "%3d,", "\r\n", 100, 100, img2, yw, 1);
      //imwrite("src.bmp", srch, srcw, (AA+aal+1), aal, 1);
      //imwrite("lbp.bmp", srch, srcw, img2, yw, 1);
      MEMSET( img2, 0, ysize );
      //SKIP_FILTER( srch, srcw, ( AA + aal + 1 ), aal, 1, img2, yw, 1, LBP59_FT );
      SKIP_FILTER( srch-2, srcw-2, ( AA + aal + 1 + aal + 1 ), aal, 1, img2+yw+1, yw, 1, LBP59_FT );
      //FPRINT2D("lbp59.txt", "%3d,", "\r\n", 100, 100, img2, yw, 1);
      
      //SKIP_BLOCK_HIST( srch, srcw, img2, yw, 1, it, bin, ptr, len );
 {
  int n=(len)*bin, _bin;
  const unsigned char* rect = ptr;
  for (_bin=0; _bin<n; rect+=4, _bin+=bin) {
    int y = rect[0], x = rect[1], cy=rect[2], cx=rect[3];   
    int pos = y*(yw)+x;
    SKIP_HIST(cy, cx, (img2+pos), srcstep, cn, (it+_bin));
  }
}
    }
    {
      int i;
      it = ( FEAT_T* ) me;
      for ( i = 0; i < bin*len; ++i ) {
        me[ i ] = ( FEAT_T ) it[ i ];
      }
    }
    //FPRINT1D("feat.txt", "%3d,\r\n", bin*len, me, 1);
    free( AA );
    free( img2 );
  }
  return bin*len;
}

int new_block_lbp_hist_feature( int ah, int aw, const unsigned char* A, int al, int ai,
                                int bin, int m, int n, FEAT_T* B ) {
  int sz = bin * m * n;
  if ( B ) {
    int ms = ah / m, ns = aw / n, i;
    for ( i = 0; i < sz; ++i ) {
      B[ i ] = 0;
    }
    if ( 59 == bin ) {
      IMBLOCK_FILTER_HIST( ah, aw, A, al, ai, B, bin, m, n, ms, ns, 1, 1, LBP59_FT, unsigned char );
    } else {
      IMBLOCK_FILTER_HIST( ah, aw, A, al, ai, B, 8, m, n, ms, ns, 1, 1, LBP8BIN_FT, unsigned char );
    }
  }
  return sz;
}
#undef LBP59_FT

int pplight_process(void* p0) {
  int ret;
  pplight_param_t* p = (pplight_param_t*)p0;
#ifdef _PPLIGHT_32F_INL_
  int is_call_pplight_32f=0;
  if (is_call_pplight_32f) {
    ret = pplight_32f(p->height, p->width, p->src, p->srcstep, p->srccn, p->dst, p->dststep, 1, p->buf );
  } else 
#endif // _PPLIGHT_32F_INL_
  {
    ret = pplight_16s(p->height, p->width, p->src, p->srcstep, p->srccn, p->dst, p->dststep, 1, p->buf );
  }
  //imwrite("./pp_16s.bmp", p->height, p->width, p->dst, p->dststep, 1);
  //imwrite("./pp_32f.bmp", p->height, p->width, p->dst, p->dststep, 1);
  return ret;
}

//  函数功能：对输入的灰度图像pp预处理
//  参数说明：
//  [ah, aw, A, al, ai] - 输入图片（规一化图像）
//  [ah, aw, B, bl, bi] - 输出图片
//  函数返回：无庖?
//  使用说明：
int CPM_PpLight( int h, int w, const unsigned char* A, int al, int ai, unsigned char* B, int bl, int bi ) {
  pplight_param_t p;
  int ret=0;
  p.height = h, p.width = w, p.src = A, p.srcstep = al, p.srccn = ai, p.dst = B, p.dststep = bl;
  pplight_memsize(&p);
  p.buf = MALLOC(char, p.buflen);
  if (p.buf) {
    ret = pplight_process(&p);
    FREE(p.buf);
  }
  return ret;
}


int new_pp_block_lbp_hist_feature( int ah, int aw, const unsigned char* A, int al, int ai,
                                   int bin, int m, int n, FEAT_T* B ) {
  int sz = bin * m * n;
  if ( B ) {
    unsigned char * _A = MALLOC( unsigned char, ah * aw );
    memset( _A, 0, ah * aw );
    CPM_PpLight( ah, aw, A, al, ai, _A, aw, 1 );
    //SKIP_COPY(ah, aw, A, al, ai, _A, aw, 1);
    //imwrite(".\\aaa.bmp", ah, aw, _A, aw, 1);
    new_block_lbp_hist_feature( ah, aw, _A, aw, 1, bin, m, n, B );
    FREE( _A );
  }
  return sz;
}

// 加特征选择的多bin的lbp滤波，只用于100×100的图片
int lbp_ex_ft( int srch, int srcw, const INT2* src, int srcstep, int cn, FEAT_T* featptr,
               int isall, int fillborder, int lbp_Feature_idyw_size, const int* lbp_Feature_idyw, int select_type ) {
  //int i, yh = srch + 1, yw = srcw + 1;
  int i, all_size = init_size(select_type);
  int _size;
  _size = isall ? all_size : lbp_Feature_idyw_size;
  //if (src) imwrite("./asdf.bmp", srch, srcw, src, srcstep, cn);
  if ( !featptr ) {
    return _size;
  }

  {
    if ( isall ) {
      FEAT_T * me;
      me = ( FEAT_T* ) ( featptr );
      MEMSET( me, 0, all_size );
      lbp_ex_ft_all( srch, srcw, src, srcstep, cn, me, fillborder, select_type );
      for ( i = 0; i < all_size; ++i ) {
        featptr[ i ] = ( FEAT_T ) me[ i ];
      }
    } else {
#ifdef __PC
#if 1
      FEAT_T* me;
      me = MALLOC( FEAT_T, all_size );
      MEMSET( me, 0, all_size );
      lbp_ex_ft_all( srch, srcw, src, srcstep, cn, me, fillborder, select_type );
      for ( i = 0; i < _size; ++i ) {
        int idx = lbp_Feature_idyw[ i ];
        featptr[ i ] = ( FEAT_T ) me[ idx ];
      }
      FREE( me );
#else
      FEAT_T* me;
      me = MALLOC( short, lbp_Feature_idyw_size );
      MEMSET( me, 0, lbp_Feature_idyw_size );
      lbp_ex_ft_16s( srch, srcw, src, srcstep, cn, me,
                     0, 1, lbp_Feature_idyw_size, lbp_Feature_idyw, select_type );
      for ( i = 0; i < lbp_Feature_idyw_size; ++i ) {
        featptr[ i ] = ( real ) me[ i ];
      }
      FREE( me );
#endif
#else
      FEAT_T* me;
      //utime_start(_start_count);
      me = ( FEAT_T* ) ( featptr );
      lbp_ex_ft_16s( srch, srcw, src, srcstep, cn, me,
                     0, 1, lbp_Feature_idyw_size, lbp_Feature_idyw, select_type );
      //printf("lbp_ex_ft_16s %f\n", utime_elapsed(_start_count));
#endif

    }
  }
  return _size;
}

#ifdef __cplusplus
//#include "lbpfilter.hpp"
#endif

int new_pp_lbp_ex_ft( int ah, int aw, const unsigned char* A, int al, int ai, FEAT_T* B,
                      int isall, int fillborder, int do_pp, int lbp_Feature_idyw_size, const int* lbp_Feature_idyw, int select_type ) {
  int sz;
  if ( !B ) {
    sz = lbp_ex_ft( ah, aw, 0, aw, 1, 0, isall, fillborder, lbp_Feature_idyw_size, lbp_Feature_idyw, select_type );
  } else {
    if ( do_pp ) {
      INT2 * _A;
      //utime_start(_start_count);
      _A = MALLOC( INT2, ah * aw );
      memset( _A, 0, ah * aw * sizeof( INT2 ) );
#ifdef __PC
      CPM_PpLight( ah, aw, A, al, ai, _A, aw, 1 ); A = _A;
#else
      //CPM_PpLight2(ah, aw, A, al, ai, _A, aw, 1);
#endif
      //imwrite(".\\pp.bmp", ah, aw, _A, al, ai);
      sz = lbp_ex_ft( ah, aw, A, aw, 1, B, isall, fillborder, lbp_Feature_idyw_size, lbp_Feature_idyw, select_type );
      FREE( _A );
      //printf("lbp_ex_ft %f\n", utime_elapsed(_start_count));
    } else {
      sz = lbp_ex_ft( ah, aw, A, aw, 1, B, isall, fillborder, lbp_Feature_idyw_size, lbp_Feature_idyw, select_type );
    }
  }
  return sz;
}
typedef enum feature_type {
  LBP8BIN = 0,         
  LBP59BIN,            
  PPLBP8BIN,           
  PPLBP59BIN,          
  BOOSTLBP59BIN,       
  PPBOOSTLBP59BIN,     
  PPBOOSTLBPXBIN,      
  PPBOOSTLBPXBINFIX,   
  PPLBPXBIN,           
  LBP59RECTBIN,        
  PPLBP59BIN2,
  PPLGBP8BINCOMPRESS,
  BOOSTLBPXBIN,        
  PPBOOSTLBPXBINABS,   
  EDGEGRADLBP,         
  BOOSTEDGEGRADLBP,    
  MAX_FEAT
} feature_type;

int CPM_GetFeature1( int srch, int srcw, const unsigned char* src, int srcstep, int cn, int type, void* featptr ) {
  int len = 0;
  int select_type=2;
  switch ( type ) {
  case LBP8BIN:
    len = sizeof(FEAT_T)*new_block_lbp_hist_feature( srch, srcw, src, srcstep, cn, 8, 10, 10, (FEAT_T*)featptr );
    break;
  case LBP59BIN:
    len = sizeof(FEAT_T)*new_block_lbp_hist_feature( srch, srcw, src, srcstep, cn, 59, 10, 10, (FEAT_T *)featptr );
    break;
  case PPLBP8BIN:
    len = sizeof(FEAT_T)*new_pp_block_lbp_hist_feature( srch, srcw, src, srcstep, cn, 8, 10, 10, (FEAT_T *)featptr );
    break;
  case PPLBP59BIN:
    len = sizeof(FEAT_T)*new_pp_block_lbp_hist_feature( srch, srcw, src, srcstep, cn, 59, 10, 10, (FEAT_T *)featptr );
    break;
  case BOOSTLBP59BIN:
    len = 0;
    break;
  case PPBOOSTLBP59BIN:
    len = 0;
    break;
  case PPBOOSTLBPXBIN:
    if ( 100 == srch && 100 == srcw ) {
      len = sizeof(FEAT_T)*new_pp_lbp_ex_ft( srch, srcw, src, srcstep, cn, (FEAT_T *)featptr, 0, 0, 1, countof(lbp_Feature_idyw_chi_2443), lbp_Feature_idyw_chi_2443, select_type );
    }
    break;
  case BOOSTLBPXBIN:
    if ( 100 == srch && 100 == srcw ) {
      len = sizeof(FEAT_T)*new_pp_lbp_ex_ft( srch, srcw, src, srcstep, cn, (FEAT_T *)featptr, 0, 0, 0, countof(lbp_Feature_idyw_chi_2443), lbp_Feature_idyw_chi_2443, select_type );
    }
    break;
  case PPBOOSTLBPXBINFIX:
    if ( 100 == srch && 100 == srcw ) {
      len = sizeof(FEAT_T)*new_pp_lbp_ex_ft( srch, srcw, src, srcstep, cn, (FEAT_T *)featptr, 0, 1, 1, countof(lbp_Feature_idyw_chi_2443), lbp_Feature_idyw_chi_2443, select_type );
    }
    break;
  case PPLBPXBIN:
    if ( 100 == srch && 100 == srcw ) {
      len = sizeof(FEAT_T)*new_pp_lbp_ex_ft( srch, srcw, src, srcstep, cn, (FEAT_T *)featptr, 1, 1, 1, countof(lbp_Feature_idyw_chi_2443), lbp_Feature_idyw_chi_2443, select_type );
    }
    break;
  case PPBOOSTLBPXBINABS:
    if ( 100 == srch && 100 == srcw ) {
      len = sizeof(FEAT_T)*new_pp_lbp_ex_ft( srch, srcw, src, srcstep, cn, (FEAT_T *)featptr, 0, 0, 1, countof(lbp_Feature_idyw_abs_2165), lbp_Feature_idyw_abs_2165, select_type );
    }
    break;
  case LBP59RECTBIN:
    //int new_rect_lbp_hist_feature(int srch, int srcw, const INT2* src, int srcstep, int cn, int* me, int fillborder)
    if ( 100 == srch && 100 == srcw ) {
      len = sizeof(FEAT_T)*new_rect_lbp_hist_feature( srch, srcw, src, srcstep, cn, (FEAT_T *)featptr, 1 );
    }
    break;
#ifdef USE_FEATURE_GEN

  case PPLBP59BIN2:
    // feature_gen(int ah, int aw, const unsigned char* A, int al, int ai, int type, float* featptr);
    //int new_rect_lbp_hist_feature(int srch, int srcw, const INT2* src, int srcstep, int cn, int* me, int fillborder)
    if ( 100 == srch && 100 == srcw ) {
      feature_gen_init();
      if ( fun_feature_gen ) {
        len = fun_feature_gen( srch, srcw, src, srcstep, cn, PPLBP59BIN2, (float *)featptr );
      }
    }
    break;
  case PPLGBP8BINCOMPRESS:
    // feature_gen(int ah, int aw, const unsigned char* A, int al, int ai, int type, float* featptr);
    //int new_rect_lbp_hist_feature(int srch, int srcw, const INT2* src, int srcstep, int cn, int* me, int fillborder)
    if ( 80 == srch && 80 == srcw ) {
      feature_gen_init();
      if ( fun_feature_gen2 ) {
        len = fun_feature_gen2( srch, srcw, src, srcstep, cn, PPLGBP8BINCOMPRESS, (float *)featptr );
      }
    }
    break;
#endif

  default:
    len = 0;
    break;
  }
  return len;
}
typedef enum distance_type {
  CPM_CHISQUARE,    
  CPM_COSINE,       
  CPM_HISTINTER,    
  CPM_ABSSUB        
} distance_type;

typedef struct RECOALGO {
  int feat_type;   
  int feat_dims;   
  int std_height;  
  int std_width;   
  int std_cn;      
  int is_dist;     
  double dist_min; 
  double dist_max; 
  double dist_th;  
  int dist_type;   
}
RECOALGO;
#if 0
static struct RECOALGO info[ MAX_FEAT ] =   //
  {
    0, 100, 100, 1, 1, 0, 8000, CPM_CHISQUARE,  /* LBP8BIN = 0,       8个bin的lbp特征*/
    0, 100, 100, 1, 1, 0, 8000, CPM_CHISQUARE,  /* LBP59BIN,          59个bin的lbp特征*/
    0, 100, 100, 1, 1, 0, 8000, CPM_CHISQUARE,  /* PPLBP8BIN,         加了预处理的8个bin的lbp特征*/
    0, 100, 100, 1, 1, 0, 8000, CPM_CHISQUARE,  /* PPLBP59BIN,        加了预处理的59个bin的lbp特征*/
    0, 100, 100, 1, 1, 0, 8000, CPM_CHISQUARE,  /* BOOSTLBP59BIN,     加了boost特征选取的59个bin的lbp特征（废弃不用）*/
    0, 100, 100, 1, 1, 0, 8000, CPM_CHISQUARE,  /* PPBOOSTLBP59BIN,   加了boost特征选取的59个bin的lbp特征（废弃不用）*/
    0, 100, 100, 1, 0, 0, 50000, CPM_HISTINTER,  /* PPBOOSTLBPXBIN,    加了boost特征选取的X个bin的lbp特征（要求100×100的图像）*/
    0, 100, 100, 1, 0, 0, 50000, CPM_HISTINTER,  /* PPBOOSTLBPXBINFIX, 加了boost特征选取的X个bin的lbp特征(fix bug)（要求100×100的图像）*/
    0, 100, 100, 1, 1, 0, 8000, CPM_CHISQUARE,  /* PPLBPXBIN,         没加了boost特征选取的59个bin的lbp特征（要求100×100的图像）*/
    0, 100, 100, 1, 1, 0, 8000, CPM_CHISQUARE,  /* LBP59RECTBIN,      用于图像质量判断的特征（要求100×100的图像）*/
    0, 100, 100, 1, 1, 0, 8000, CPM_CHISQUARE,  /* PPLBP59BIN2,       */
    0, 80, 80, 1, 1, 0, 300000, CPM_CHISQUARE,  /* PPLGBP8BINCOMPRESS,*/
  };
#else
static struct RECOALGO info1[] =   //
  {
    PPLGBP8BINCOMPRESS, 64000, 80, 80, 1, 1, 0, 288550, 0.52f, CPM_CHISQUARE,  /* RECOGNITION_0 */
    PPLGBP8BINCOMPRESS, 64000, 80, 80, 1, 0, 0, 294320, 0.52f, CPM_HISTINTER,  /* RECOGNITION_1 */
    PPLGBP8BINCOMPRESS, 64000, 80, 80, 1, 0, 0, 397680, 0.52f, CPM_ABSSUB,  /* RECOGNITION_2 */
    PPBOOSTLBPXBIN, 2443, 100, 100, 1, 1, 0, 8000, 0.51f, CPM_CHISQUARE,  /* RECOGNITION_3 */
    LBP59RECTBIN, 13806, 100, 100, 1, 1, 0, 100000, 0.51f, CPM_ABSSUB,  /* RECOGNITION_4 */
    BOOSTLBPXBIN, 2443, 100, 100, 1, 1, 0, 12154, 0.52f, CPM_CHISQUARE,  /* RECOGNITION_5 */
    PPLBP59BIN, 5900, 100, 100, 1, 1, 0, 11719, 0.52f, CPM_HISTINTER,  /* RECOGNITION_6 */
    PPBOOSTLBPXBINABS, 2165, 100, 100, 1, 1, 0, 25000, 0.52f, CPM_ABSSUB,  /* RECOGNITION_3 */
  };
static struct RECOALGO info[] =   //
  {
    PPLGBP8BINCOMPRESS, 64000, 80, 80, 1, 1, 0, 288550, 0.52f, CPM_CHISQUARE,  /* RECOGNITION_0 */
    PPLGBP8BINCOMPRESS, 64000, 80, 80, 1, 0, 0, 294320, 0.52f, CPM_HISTINTER,  /* RECOGNITION_1 */
    PPLGBP8BINCOMPRESS, 64000, 80, 80, 1, 0, 0, 397680, 0.52f, CPM_ABSSUB,  /* RECOGNITION_2 */
    PPBOOSTLBPXBIN, 2443, 100, 100, 1, 1, 0, 8000, 0.51f, CPM_CHISQUARE,  /* RECOGNITION_3 */
    LBP59RECTBIN, 13806, 100, 100, 1, 1, 0, 100000, 0.51f, CPM_ABSSUB,  /* RECOGNITION_4 */
    BOOSTLBPXBIN, 2443, 100, 100, 1, 1, 0, 12154, 0.52f, CPM_CHISQUARE,  /* RECOGNITION_5 */
    PPLBP59BIN, 5900, 100, 100, 1, 1, 0, 11719, 0.52f, CPM_HISTINTER,  /* RECOGNITION_6 */
    PPBOOSTLBPXBINABS, 2165, 100, 100, 1, 1, 0, 8301, 0.52f, CPM_ABSSUB,  /* RECOGNITION_3 */
  };
#endif

int lbpex_process( void* p0 ) {
  int it, i, yh, yw, yl, ysize, exx, aah, aaw, aal, exx2, iw;
  int srch, srcw, srcstep, cn, isall, fillborder;
  int umap[ 16 ];
  int it2[ 64 ];
  //int* lbp_Feature_idyw;
  unsigned char* img2;
  int* y1, * y;
  const unsigned char* src;
  FEAT_T* me;
  lbpex_param_t* p;
  const int* lbp_Feature_idyw=0;
  lbp_Feature_List_T* lbp_Feature_List;
  buf_t bf[1]={0};
  p = (lbpex_param_t*)p0;
  lbp_Feature_List=get_lbp_Feature_List(p->select_type);
  srch = p->height, srcw = p->width, srcstep = p->srcstep;
  isall = !(p->select_type), fillborder = p->fillborder;
  src = p->src;
  cn = p->srccn;
  me = p->feature;

  if (p->isall) {
    p->featurelen = init_size(p->select_type);
  } else {
    lbp_Feature_idyw = get_lbp_Feature_idyw(p->select_type, &p->featurelen);
  }
#if 0
  {
    static int iswsorted = 0;
    if (!iswsorted) {
      QSORT(lbp_Feature_idyw, lbp_Feature_idyw+p->featurelen/sizeof(FEAT_T), LESS, int);
      FPRINT1D("lbp_Feature_idyw_sort.txt", "%d,\r\n", p->featurelen/sizeof(FEAT_T), lbp_Feature_idyw, 1);
      iswsorted = 1;
    }
  }
#endif
  bfinit(bf, p->buf, p->buflen);
  if (p->do_pp) {
    pplight_param_t pp;
    pp.height = p->height;
    pp.width = p->width;
    pp.srcstep = p->srcstep;
    pp.srccn = p->srccn;
    pp.src = p->src;
    pp.dststep = p->width;
    pp.srccn = 1;
    pplight_memsize(&pp);
    BFMALLOC(bf, pp.dst, pp.dstlen);
    BFMALLOC(bf, pp.buf, pp.buflen);
    pplight_process(&pp);
    src = pp.dst;
    //imwrite("./src.bmp", 100, 100, pp.src, 100, 1);
    //cvShowImage("src", 100, 100, pp.src, 100, 1); cvWaitKey(-1);
    //imwrite("./asdf.bmp", 100, 100, src, 100, 1);
    srcstep = pp.dststep;
    cn = 1;
    BFFREE(bf, pp.buf);
    //BUFFREE(unsigned char, pp.dstlen);
  }
  it = 0, yh = srch + 1, yw = srcw + 1, ysize = yh * yw;
  exx = ( ( 2 * FN - 1 ) * 3 );
  yl = yw + exx;
  aah = srch + exx, aaw = srcw + exx;
  aal = aaw;
  exx2 = ( exx / 2 );
  iw = 0;
  //retsize = isall ? init_size() : lbp_Feature_idyw_size;

  BFMALLOC( bf, y1, ( yh + exx ) * ( yl ) );
  MEMSET( y1, 0, ( yh + exx ) * ( yl ) );
  y = y1 + exx2 * yl + exx2;
  {
    fillborder=0;
    //FPRINT2D(".\\src.txt", "%d\n", srch, srcw, src, srcstep, 1);
    if ( fillborder ) {
      INT2 * AA;
      BFMALLOC( bf, AA, ( aah ) * ( aal ) );
      MEMSET( AA, 0, ( aah ) * ( aal ) );
      COPY_REPLICATE_BORDER( srch, srcw, src, srcstep, aah, aaw, AA, aal, 1, exx2, exx2 );
      //PRINT(aah, aaw, Asrc, aal, 1, ".\\Asrc.txt", "%d\n");
      SKIP_INTEGRAL( aah, aaw, AA, aal, 1, y1, yl, 1 );
      BFFREE( bf, AA);
    } else {
      int* y2 = y1 + yl * exx2 + exx2;
      SKIP_INTEGRAL( srch, srcw, src, srcstep, 1, y2, yl, 1 );
      COPY_REPLICATE_BORDER( srch + 1, srcw + 1, y2, yl, ( yh + exx ), yl, y1, yl, 1, exx2, exx2 );
    }
  }
  BFMALLOC( bf, img2, ysize );
  //Mati& me = *this;
  for ( i = MINFN; i < (int)FN; ++i ) {
    //int ex = ( 2 * i + 1 ) * 3 + 1;
    int bin = lbp_Feature_List[ i ].bin;
    int len = lbp_Feature_List[ i ].len;
    const unsigned char* rect = lbp_Feature_List[ i ].ptr;
    //int eex = ex*1;
    const unsigned char* __lbpmap0 = lbpmap[ i ];
    lbp_ex_init( i, yl, 1, umap );
    MEMSET( img2, 0, ysize );
#if 0

    SKIP_FILTER( srch, srcw, y, yl, 1, img2, yw, 1, LBP_EX_FT );
#else

    skip_filter_lbp_ex_ft( srch, srcw, y, yl, img2, yw, umap, __lbpmap0 );
#endif

    if ( isall ) {
      //SKIP_BLOCK_HIST(srch, srcw, img2, yw, 1, it, bin, rect, len);
      int n = ( len ) * bin, _bin;
      for ( _bin = 0; _bin < n; rect += 4, _bin += bin ) {
        int y = rect[ 0 ], x = rect[ 1 ], cy = rect[ 2 ], cx = rect[ 3 ];
        int pos = y * ( yw ) + x * ( cn );
        SKIP_HIST( cy, cx, ( src + pos ), yw, 1, ( me + _bin ) );
      }
      me += n;
    } else {
#if 1
      int _bin, lbp_Feature_idyw_size = p->featurelen/sizeof(FEAT_T);
      for ( _bin = 0; _bin < len && iw <  lbp_Feature_idyw_size; ++_bin, rect += 4, it += bin ) {
        int id = lbp_Feature_idyw[ iw ];
        int itend = it + bin;
        if ( id >= it && id < itend ) {
          int y = rect[ 0 ], x = rect[ 1 ], cy = rect[ 2 ], cx = rect[ 3 ];
          unsigned char* pos = img2 + y * ( yw ) + x;
          MEMSET( it2, 0, 64 );
          SKIP_HIST( cy, cx, ( pos ), yw, 1, ( it2 ) );
          for ( ; ( id = lbp_Feature_idyw[ iw ] ) < itend && iw < lbp_Feature_idyw_size; ++iw ) {
            int t = it2[ id - it ];
            ASSERT(iw<lbp_Feature_idyw_size);
            //if (t>255) { t>>=1; } if (t>255) { t>>=1; } if (t>255) { t>>=1; } if (t>255) { printf("%d\n", t); }
            me[ iw ] = t > FEAT_MAX ? FEAT_MAX : t;
          }
        }
      }
#else
      int tmp[100000];
      int n = ( len ) * bin, _bin;
      for ( _bin = 0; _bin < n; rect += 4, _bin += bin ) {
        int y = rect[ 0 ], x = rect[ 1 ], cy = rect[ 2 ], cx = rect[ 3 ];
        int pos = y * ( yw ) + x * ( cn );
        SKIP_HIST( cy, cx, ( src + pos ), yw, 1, ( me + _bin ) );
      }
      me += n;
#endif
    }
  }
  BFFREE( bf, img2 );
  BFFREE( bf, y1);
  
  //printf("%f\n", utime_elapsed(_start_time));
  return 1;
}

int lbpex_process23( void* p0 ) {
  lbpex_param_t* p = (lbpex_param_t*)p0;
  if (23==p->select_type) {
    FEAT_T* me = p->feature;
    p->select_type = 2;
    lbpex_process(p0);
    p->feature = me+2165;
    p->select_type = 3;
    lbpex_process(p0);
    p->feature = me;
    p->select_type = 23;
//#define MONOFILT_FEAT3 monofilt_feat3
#ifdef MONOFILT_FEAT3
    //#pragma comment(lib,"libmonofilt.lib")
  } else if (24==p->select_type) {
    int monofilt_feat3( int srch, int srcw, const unsigned char* src, int srcstep, int cn, int type, void* featptr, int featlistlen, const unsigned char* featlist );

    const unsigned char monofilt_block[]={
#include "../monofilt/分块.txt"
  };
    FEAT_T* me = p->feature;
    int len1, len2, len3, flen=(sizeof(monofilt_block)/3)/4;
    p->select_type = 2;
    lbpex_process(p0);
    len1 = MONOFILT_FEAT3(100, 100, p->src, 100, 1, 0, me+2165, flen, monofilt_block)/sizeof(FEAT_T);
    len2 = MONOFILT_FEAT3(90, 100, p->src+0*100, 100, 1, 0, me+2165+len1, flen, monofilt_block+flen*4)/sizeof(FEAT_T);
    len3 = MONOFILT_FEAT3(50, 100, p->src+0*100, 100, 1, 0, me+2165+len1+len2, flen, monofilt_block+flen*4+flen*4)/sizeof(FEAT_T);
    //cvShowImage("asdf", 90, 100, p->src+10*100, 100, 1); cvWaitKey(-1);
    //cvShowImage("asdf", 50, 100, p->src+0*100, 100, 1); cvWaitKey(-1);
    p->featurelen = (2165+len1+len2+len3)*sizeof(FEAT_T);
#endif
  } else {
    lbpex_process(p0);
  }
  return 0;
}
// 函数功能：对输入的灰度图像提取特征
// 参数说明：
// 输入：
// [ah, aw, A, al, ai] - 一副规一化图像或pp处理后的图像
// type                - 特征类型
// 输出：
// featptr             - 输出特征数组，指针由右外部分配，指针为NULL时返回所需real的个数
// 返回 - 特征数组大小
// 注意：只有PPBOOSTLBPXBIN特征要100×100的图像，其他特征不需要
// 函数返回：如果只给图像高和宽，返回特征向量维度
// 使用说明：

//#include "edgegradLBP.inl"
int CPM_GetFeature( int srch, int srcw, const unsigned char* src, int srcstep, int cn, int type, void* featptr ) {
  lbpex_param_t lp={0};
#ifdef _EDGEGRADLBP_INL_
  if (EDGEGRADLBP==type) {
    if (STDIMGH==srch && STDIMGW==srcw && 1==cn) {
      return edgegradLBP(src, srcstep, featptr)*sizeof(FEAT_T);
    }
    return 0;
  }
  if (BOOSTEDGEGRADLBP==type) {
    if (STDIMGH==srch && STDIMGW==srcw && 1==cn) {
      return boostedgegradLBP(src, srcstep, featptr)*sizeof(FEAT_T);
    }
    return 0;
  }
#endif // _EDGEGRADLBP_INL_
  if (STDIMGH==srch && STDIMGW==srcw && 1==cn) {
    lp.height = STDIMGH, lp.width = STDIMGW, lp.src = src, lp.srcstep = srcstep, lp.srccn = 1;
    lp.feature = (FEAT_T *)featptr;//, lp.featurelen = p->featurelen;
    lp.isall=0, lp.select_type = 2, lp.fillborder = 1, lp.do_pp = 1;
    //lp.buf = buf, lp.buflen = buflen;
    lbpex_memsize(&lp);
    if (featptr) {
      lp.buf = MALLOC(unsigned char, lp.buflen);
      lbpex_process23(&lp);
      FREE(lp.buf);
    }
  }
  return lp.featurelen;
}

// 函数功能：取输入识别算法类型ID的特征信息
// 参数说明：
// 输入：
// type                - 特征类型
// 输出：识别算法结构的指针，NULL表示没有这个特征的信息
const struct RECOALGO* CPM_GetRecoAlgoInfo( int type ) {
#if 0
  static int RECOALGO_inited = 0;
  if ( 0 == RECOALGO_inited ) {
    RECOALGO_init();
    RECOALGO_inited = 1;
  }
  if ( type < 0 || type > sizeof( info ) / sizeof( RECOALGO ) ) {
    return NULL;
  }
  return info + type;
#else
  //static RECOALGO info7 = {PPBOOSTLBPXBINABS, 2165, 100, 100, 1, 1, 0, 8301, 0.52f, CPM_ABSSUB};
  static RECOALGO info7 = {PPBOOSTLBPXBINABS, 2165, 100, 100, 1, 1, 0, 25000, 0.52f, CPM_ABSSUB};
  info7.feat_dims = CPM_GetFeature( 100, 100, NULL, 0, 1, info7.feat_type, NULL );
  return &info7;
#endif
}


static int RECOALGO_init() {
  int i;
  static int isdist[ 4 ];
  isdist[ CPM_CHISQUARE ] = 1;
  isdist[ CPM_COSINE ] = 0;
  isdist[ CPM_HISTINTER ] = 0;
  isdist[ CPM_ABSSUB ] = 1;
  for ( i = 0; i < (int)( sizeof( info ) / sizeof( RECOALGO ) ); ++i ) {
    int ah = info[ i ].std_height;
    int aw = info[ i ].std_width;
    info[ i ].dist_type = CPM_CHISQUARE;
    info[ i ].feat_type = LBP59BIN;
    info[ i ].feat_dims = CPM_GetFeature( ah, aw, NULL, 0, 1, info[ i ].feat_type, NULL );
    info[ i ].is_dist = isdist[ info[ i ].dist_type ];
  }
  return 0;
}

static const uint g_hashfeat2165_mid[32]=  {355, 297, 409, 463, 455, 415, 622, 350,
508, 336, 590, 339, 361, 1080, 1051, 1218,
673, 1115, 883, 425, 640, 2975, 2382, 2263,
1830, 2657, 2594, 3456, 2643, 4613, 5008, 2413,
}; // 中位数

uint hashfeat2165(const FEAT_T* feat) {
  uint h=0, j;
  const uint* mid = g_hashfeat2165_mid;
  uint cnt[32] = {0};
  for (j=0; j<FEAT_SIZE2; ++j) {
    int k = j*32/FEAT_SIZE2;
    cnt[k] += feat[j];
  }
  for (j=0; j<32; ++j) {
    h = (h<<1)|(mid[j]>cnt[j]);
  }
  return h;
}