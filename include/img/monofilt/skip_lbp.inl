
//  0,  1,  2
//  3,  4,  5
//  6,  7,  8
// SKIP_FILTER(srch, srcw, y, yl, 1, img2, yw, 1, LBP_EX_FT);
void skip_lbp( int srch, int srcw, const TYPE *y0, int yl0, unsigned char* img2, int yw, const unsigned char* __lbpmap0 ) {
  int A0, A1, B0, B1;
  int i, j, k;
  int umap[9];
  int yl=srcw+2;
  TYPE* y1=(TYPE*)pmalloc((srch+2)*(srcw+2)*sizeof(TYPE));
  TYPE* y=y1+yl+1;
  copy_replicate_border(srch, srcw, y0, yl0*sizeof(TYPE), (srch+2), (srcw+2), y1, yl*sizeof(TYPE), sizeof(TYPE), 1, 1, 0);
  //imwrite("aaaa.bmp", (srch+2), (srcw+2), y1, yl*sizeof(TYPE), sizeof(TYPE));
  
  //FPRINT2D("y1.txt", "%3d,", "\n", (srch+2), (srcw+2), y1, yl, 1);
  k=0;
  for (i=-1; i<=1; ++i) {
    for (j=-1; j<=1; ++j) {
      umap[k++] = i*yl+j;
    }
  }
  for ( A1 = 0, B1 = 0; A1 < srch * yl; A1 += yl, B1 += yw ) {
    for ( A0 = A1, B0 = B1; A0 < srcw + A1; A0 += 1, B0 += 1 ) {
      //LBP_EX_FT((y+A0), yl, 1, (img2+B0));
#define EVELV(_A, a)   (*(_A+umap[a]))

      // lbpÂË²¨Ëã×Ó
      {
        unsigned char p = ( unsigned char ) 0;
        const TYPE* yy = ( y + A0 );
        TYPE v5;
        int v1, v2, v3, v4, v6, v7, v8, v9;

        v5 = ( EVELV( yy, 4 ) );
        v1 = ( EVELV( yy, 0 ) >= v5 ) << 7;
        v2 = ( EVELV( yy, 1 ) >= v5 ) << 6;
        v3 = ( EVELV( yy, 2 ) >= v5 ) << 5;
        v4 = ( EVELV( yy, 5 ) >= v5 ) << 4;
        v6 = ( EVELV( yy, 8 ) >= v5 ) << 3;
        v7 = ( EVELV( yy, 7 ) >= v5 ) << 2;
        v8 = ( EVELV( yy, 6 ) >= v5 ) << 1;
        v9 = ( EVELV( yy, 3 ) >= v5 );
#undef EVELV
        p = ( unsigned char ) ( v1 | v2 | v3 | v4 | v6 | v7 | v8 | v9 );
        //*(img2+B0) = p;
        *( img2 + B0 ) = __lbpmap0[ p ];
      }

    }
  }
  pfree(y1);
}


//Returns LBP histogram (256 bins) of picture X.
//
//             1  2   4
// weights = 128  0   8
//            64 32  16
//
//the size of X must be at least 3x3 pixels

int LBP3(const IplImage* X, const unsigned char* mapping3, IplImage* hst) {
  skip_lbp(X->height, X->width, (TYPE*)X->imageData, X->widthStep/sizeof(TYPE), (unsigned char*)hst->imageData, hst->widthStep, mapping3);
  return 0;
}
#undef TYPE
#undef skip_lbp
#undef LBP3
