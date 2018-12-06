#include "_faceDetect.h"
//#include "img/imgio.inl"
//#define USE_FIX_FLOAT1



int swaplr_img(int h, int w, unsigned char* a, int al) {
  int i, j, t;
  for (i=0; i<h; ++i, a+=al) {
    for (j=0; j<(w/2); ++j) {
      CC_SWAP(a[j], a[w-j-1], t);
    }
  }
  return 0;
}

int mattranspose(int h, int w, const unsigned char* A, int al, unsigned char* B, int bl) {
  int i, j;
  for (i=0; i<h; ++i) for (j=0; j<w; ++j) B[j*bl+i] = A[i*al+j];
  return 0;
}
int matsum_u8(int h, int w, const unsigned char* A, int al) {
  int i, j, s=0;
  for (i=0; i<h; ++i) for (j=0; j<w; ++j) s += A[i*al+j];
  return s;
}
int matfix2_u8(int h, int w, unsigned char* A, int al, int thd) {
  int i, j, s=matsum_u8(h, w, A, al);
  if (w*h) {
    s /= w*h;
    for (i=0; i<h; ++i) for (j=0; j<w; ++j)
      if (A[i*al+j]>thd)
        A[i*al+j]=s;
  }
  return s;
}
int matfix_u8(int h, int w, unsigned char* A, int al, int thd) {
  int i, j, k, jj;
  for (i=0; i<h; ++i) {
    unsigned char* A0 = A + i*al;
    if (A0[0]>thd)A0[0] = thd;
    if (A0[w-1]>thd)A0[w-1] = thd;
    for (j=1, k=w-2; j<k; ) {
      if (A0[j]>thd) {
        if (j>7) {
          jj=((j+1)&7);
          jj = MAX(jj, 1);
        } else {
          jj = 1;
        }
        A0[j] = A0[j-jj];
      } else {++j;}
      if (A0[k]>thd) {
        if (w-k>7) {
          jj=((w-k)&7);
          jj = MAX(jj, 1);
        } else {
          jj = 1;
        }
        A0[k] = A0[k+jj];
      } else {++k;}
    }
  }
  return 0;
}
#ifdef USE_FIX_FLOAT
#define LUTPASS lutpass_1
#define get_invvar get_invvar_1
#else
#define LUTPASS lutpass_0
#define get_invvar get_invvar_0
#endif
//int rectcnt=0;
int lut_detect_pass(const unsigned char* A, int al, const void* ca0) {
  const HAARCASCADE* ca = (const HAARCASCADE*)ca0;
  int h=ca->h, w=ca->w, ispassed, stp = w+1, area2;
  FIX_FLOAT* inter = MALLOC(FIX_FLOAT, (h+1)*(w+1));
  FIX_FLOAT* inter1 = MALLOC(FIX_FLOAT, (h+1)*(w+1));
  fHaarWork* pWork = MALLOC( fHaarWork, ca->ihlen );
  double score=0;
  double invvar;
  fStage* stage;
  fHaar* ih;
  FixCascade1(ca0, &stage, &ih);
  cvIntegralImage_C1R( h, w, A, al, inter, stp, inter1, stp, 0, 0 );  //0.1875 //
  area2 = ( ca->w * ca->h );
  invvar = get_invvar( inter, inter1, ca->h * stp, ca->w, area2 );
  setoffset3( ca->ihlen, ih, pWork, stp, 1, 1.f, 1.f, !CAISHAVEFLOAT(ca, ih));                 //0.275
  ispassed = LUTPASS( inter, ih, stage, ca->stagelen, pWork, (int)invvar, &score );
  FREE(inter);
  FREE(inter1);
  FREE(pWork);
  return ispassed;
}
int g_cntrc = 0;
int g_outlen = 0;
int lut_detect( const void* alg ) {
  int i, outlen, cx, cy, h, w, h2, w2;
  int height, width, stepxy, mincnt, datastep, maxoutlen;
  double ssmin, ssmax, ss, thd;
  const unsigned char* data, *mask;
  unsigned char* tmpimg;
  XRECT* outrc, r;
  detect_info_t * p;
  const HAARCASCADE** ca;
  FIX_FLOAT * inter, *inter1;
  int cascnt[8]={0,0,0,0};
  int cntrc=0;
  const fStage* stage[4]={0};
  const fHaar* ih[4]={0};
  //XRECT tttrc[200]={0};
  p = ( detect_info_t* ) alg;
  ca = (const HAARCASCADE**)p->cas;
  
  data = p->data;
  mask = p->mask;

  r = p->roi;
  if (r.w) {
    data = data + r.y*p->datastep + r.x;
    if (NULL != mask) {
      mask = mask + r.y*p->maskstep + r.x;
    }
  }
  if (p->is_trans & 1) {
    int t;
    //imresize_impl()
    IMRESIZE1( p->height, p->width, data, p->datastep, 1, p->h, p->w, p->tmpimg0, 1, p->h );
    //IMRESIZE_AREA( p->height, p->width, data, p->datastep, 1, p->h, p->w, p->tmpimg0, 1, p->h );
    //FLOP2D(p->w, p->h, p->tmpimg0, p->h, 1, t);
    //cvShowImage("data", p->height, p->width, data, p->datastep, 1);
    //cvShowImage("tmpimg0", p->w, p->h, p->tmpimg0, p->h, 1); cvWaitKey(-1);
    //imwrite("./asdf.bmp", p->w, p->h, p->tmpimg0, p->h, 1);
    if ( mask ) {
      IMRESIZE1( p->height, p->width, mask, p->maskstep, 1, p->h, p->w, p->tmpmask0, 1, p->h );
      //FLOP2D(p->w, p->h, p->tmpmask0, p->h, 1, t);
    }
    CC_SWAP(p->h, p->w, t);
    CC_SWAP(p->height, p->width, t);
  } else {
    IMRESIZE1( p->height, p->width, data, p->datastep, 1, p->h, p->w, p->tmpimg0, p->w, 1 );
    if ( mask ) {
      IMRESIZE1( p->height, p->width, mask, p->maskstep, 1, p->h, p->w, p->tmpmask0, p->w, 1 );
    }
  }
  if (p->is_flip) {
    int t;
    FLIP2D(p->h, p->w, p->tmpimg0, p->w, 1, t);
    //cvShowImage("asdf", p->h, p->w, p->tmpimg0, p->w, 1); cvWaitKey(-1);
    t=0;
  }
  //if (ca[0]->w==20)   cvShowImage("asdf", p->h, p->w, p->tmpimg0, p->w, 1); cvWaitKey(-1);
  if (p->is_flop) {
    int t;
    FLOP2D(p->h, p->w, p->tmpimg0, p->w, 1, t);
    //cvShowImage("asdf", p->h, p->w, p->tmpimg0, p->w, 1); cvWaitKey(-1);
    t=0;
  }
  //ShowImage("asdf", p->h, p->w, p->tmpimg0, p->w, 1); WaitKey(-1);
  //ret = lut_detect_1( alg );
  data = p->tmpimg0;
  
  maxoutlen = p->maxoutlen;
  outrc = p->out;
  //outrc = tttrc;

  //const int is_nesting = ( 0 == ( ca->ih[ ca->stage[ 0 ].outlen ].ft0.w ) );
  inter = p->inter;
  inter1 = p->inter1;
  height = p->h;
  width = p->w;
  stepxy = p->stepxy;
  mincnt = p->mincnt;
  ssmin = p->ssmin;
  ssmax = p->ssmax;
  ss = p->ss;
  thd = p->thd;
  datastep = p->w;
  //int maskstep = p->w;
  //const unsigned char* mask = p->tmpmask0;
  tmpimg = p->tmpimg;
  //unsigned char* tmpmask = p->tmpmask;
  
  cx = ca[0]->w, cy = ca[0]->h;
  h = XFLOOR( height / ssmin );
  w = XFLOOR( width / ssmin );
  h2 = ( height );
  w2 = ( width );
  if (p->casnum>1) {
    //matfix_u8(h2, w2, data, datastep, 200);
    //imwrite(".\\aaa.bmp", h2, w2, data, datastep, 1);
  }
  //{img_t im[1] = {0};IMINIT(im, 240, 320, data, datastep, 1); imwrite(".\\aaa.bmp", im);}
  //cvShowImage(".\\aaa.bmp", h2, w2, data, datastep, 1); cvWaitKey(-1);
  //siz = ( h + 1 ) * ( w + 1 );
  //STS_set(&sts0, CLK_gethtime());
  
  for (i=0; i<p->casnum; ++i) {
    stage[i] = ( const fStage * ) ( ca[i] + 1 );
    ih[i] = ( const fHaar * ) ( stage[i] + ca[i]->stagelen );
    //setoffset3( ca[i]->ihlen, ih[i], (struct fHaarWork *)p->pWirk[i], 0, 1, 1.f, 1.f, 1 );                 //0.275
  }
  //printf("w=%d h=%d cx=%d maxoutlen=%d ssmax=%f ssmin=%f\n", w, h, cx, maxoutlen, ssmax, ssmin);
  //if (20!=ca[0]->w) {return 0;}
  //return 0;
  //STS_delta(&sts0, CLK_gethtime());
#ifdef USE_FIX_FLOAT1
  
  for(outlen = 0; outlen < maxoutlen && w >= cx && h >= cy && issmax > issmin;
    issmin = SHIFT_MUL( issmin, iss ), w = SHIFT_DIV( width, issmin ), h = SHIFT_DIV( height, issmin ) )
#else
  for(outlen = 0; outlen < maxoutlen && w >= cx && h >= cy && ssmax > ssmin;
    ssmin *= ss, w = XFLOOR( width / ssmin ), h = XFLOOR( height / ssmin ) )
#endif
  {
    int stp, x, y, ranx = w - cx, rany = h - cy;
    int area2 = ( cx * cy );
    int stepxy1 = stepxy > 0 ? stepxy : 1; //用ssmin自适应，不适合缩放图像方式
    stp = ( w + 1 );
    //siz = ( h + 1 ) * stp;
    //FILL( siz, inter, 0 );
    //FILL( siz, inter1, 0 );
    //memset( tmpimg, 0, h * w );
    
    //STS_delta(&sts2, CLK_gethtime());
    IMRESIZE1( h2, w2, data, datastep, 1, h, w, tmpimg, w, 1 );                //0.13
    //h2=h, w2=w, datastep=w;memcpy(data, tmpimg, h*w);
#if 0
    if ( 1 ) {
      static int i = 0;
      char buf[ 256 ];
      _snprintf( buf, 256, "./out/aaa%03d.bmp", i );
      imwrite( buf, h, w, tmpimg, w, 1 );
      ++i;
    }
#endif
    //printf("w=%d h=%d maxoutlen=%d ssmax=%f ssmin=%f\n", w, h, maxoutlen, ssmax, ssmin);
    // IMWRITE("aaa.bmp", h, w, tmpimg, w, 1);

    //cvShowImagePal(".\\aaa.bmp", h, w, tmpimg, w, 1, 0); cvWaitKey(-1);
    //SKIP_INTEGRAL_SQ(h, w, tmpimg, w, 1, inter, stp, HAAR_CN);
    cvIntegralImage_C1R( h, w, tmpimg, w, inter, stp, inter1, stp, 0, 0 );  //0.1875 //
    //FPRINT2D("aaa.txt", "%d,", "\n", h, w, tmpimg, w, 1);
    //FPRINT2D("bbb.txt", "%d,", "\n", h, w, inter, w+1, 1);
    for (i=0; i<p->casnum; ++i) {
      setoffset3( ca[i]->ihlen, ih[i], (struct fHaarWork *)p->pWirk[i], stp, 1, 1.f, 1.f, 1 );           //0.275
    }
    //printf("(iHaarWork3*)p->pWirk[0] %d\n", ((iHaarWork3*)p->pWirk[0])->ft0.p0);
    for ( y = 0; outlen < maxoutlen && y <= rany; y += stepxy1 ) {
      for ( x = 0; outlen < maxoutlen && x <= ranx; x += stepxy1 ) {
#if 0
        int d=(h+w)/6, x1=ABS(x+cx/2-w/2), y1=ABS(y+cy/2-h/2);
        if ((x1+y1)>d) {
          continue;
        }
#endif
        if ( !mask || mask[ XFLOOR( y * ssmin ) * p->maskstep + XFLOOR( x * ssmin ) ] ) {
          int ispassed = 0;
          FIX_FLOAT *tmpSamp = inter + y * stp + x;
          FIX_FLOAT *tmpSamp1 = inter1 + y * stp + x;
          FIX_FLOAT invvar;
          double score;
          invvar = get_invvar( tmpSamp, tmpSamp1, cy * stp, cx, area2 );

#if 0
          for (i=0; i<p->casnum; ++i) {
            ispassed = LUTPASS( tmpSamp, ih[i], stage[i], ca[i]->stagelen, p->pWirk[i], invvar, &score );
            if (ispassed) {
              break;
            }
          }
#else
          //printf("(iHaarWork3*)p->pWirk[0] %d\n", ((iHaarWork3*)p->pWirk[0])->ft0.p0);
          //if (p->casnum>1)  ++rectcnt;
          ispassed = LUTPASS( tmpSamp, ih[0], stage[0], ca[0]->stagelen, p->pWirk[0], invvar, &score );
          if (ispassed && p->casnum>1) {
            //int ispassed2 = lut_detect_pass(tmpimg+y*w+x, w, ca[0]);
            //static int iii=0; char buf[256]; i=0, iii++;
            //_snprintf(buf, 256, ".\\eye\\%05d_%d.bmp", iii, i);
            //imwriterect(buf, tmpimg, w, 1, x, y, cx, cy);
            for (i=1; i<p->casnum; ++i) {
              ispassed = LUTPASS( tmpSamp, ih[i], stage[i], ca[i]->stagelen, p->pWirk[i], invvar, &score );
              if (ispassed) {
                //outrc[ outlen ].id = i;
                //int iii=0; char buf[256];_snprintf(buf, 256, ".\\out\\eye_%05d.bmp", iii++);
                //imwriterect(buf, tmpimg, w, 1, x, y, cx, cy);
                //if (cx>20) { int asdf=0; }
                //cascnt[i]++;
                break;
              }
            }
          }
#endif
          ++cntrc;
          if ( ispassed && outlen<maxoutlen ) {
            //printf("%d, outlen=%d y=%d x=%d %08x %08x %f\n", sizeof(XRECT), outlen, iiy, iix, outrc, p->out, ssmin);
            //提高精度，避免出现检测框右偏现象
            outrc[ outlen ].x = XROUND( (x+1) * ssmin );
            outrc[ outlen ].y = XROUND( (y) * ssmin );
            outrc[ outlen ].w = XROUND( cx * ssmin );
            outrc[ outlen ].h = XROUND( cy * ssmin );
            //if (p->is_trans && cx==cy) {  outrc[ outlen ].x+=1; }
            
#if 0
            if (ca[0]->w>0) {
              static int m=0;
              char buf[256];
              _snprintf(buf, 256, "./out/out%d.bmp",m++);
              imwriterect( buf, tmpimg, w, 1, x, y, cx, cy );
              //cvShowImage("ttt1", h, w, tmpimg, w, 1);
              cvShowImage("ttt", cy, cx, tmpimg+y*w+x, w, 1);cvWaitKey(-1);
            }
#endif
            outrc[ outlen ].score = (int)score;
            outrc[ outlen ].count = 1;
            ++outlen;
          }
        }
      }
    }
#if 0
    if ( 1 ) {
      static int i = 0;
      static int len2 = 0;
      char buf[ 256 ];
      if (0==outlen) {
        len2 = outlen;
      }
      if (len2!=outlen) {
        _snprintf( buf, 256, "./out/bbb%03d_%03d.bmp", i, (outlen-len2) );
        imwrite( buf, h, w, tmpimg, w, 1 );
        len2 = outlen;
      }
      ++i;
    }
#endif
    //printf("fff %f\n", utime_elapsed(_start_count));
    // STS_delta(&sts0, CLK_gethtime());//0.037558685446
    //break;
  }
  if (ca[0]->w==ca[0]->h) {
    g_cntrc = cntrc;
    g_outlen = outlen;
  }
  //logprintf("cntrc=%d ", cntrc);
  //logprintf("outlen=%d ", outlen);
  //printf(" r%d ", cntrc);
  {
    double sc = p->sc;
	  int buflen=0;
#if 0
    if (p->casnum>1) {
      int i, j=0, c[4]={0}; for (i=0; i<outlen; ++i) {c[outrc[ i ].id]++;}
      if (c[1]>c[2]) { for (i=0; i<outlen; ++i) {if (outrc[i].id==1) outrc[j++] = outrc[i];} outlen = j;  }
      if (c[2]>c[1]) { for (i=0; i<outlen; ++i) {if (outrc[i].id==2) outrc[j++] = outrc[i];} outlen = j;  }
    }
#endif
    //PXRECT
	outlen = rect_partition(outrc, outrc + outlen, thd, mincnt, 1, p->rect_partition_buf, &buflen);
#if 0
    if (ca[0]->w>20) {
      if (0==outlen || 1) {
        static int ii=0;
        char buf[256];
        _snprintf(buf, 256, ".\\out\\leye_%d.bmp", ii++);
        imwrite(buf, p->h, p->w, p->tmpimg0, p->w, 1);
      }
    }
#endif
    for ( i = 0; i < outlen; ++i ) {
      outrc[ i ].x = XROUND( outrc[ i ].x / sc );
      outrc[ i ].y = XROUND( outrc[ i ].y / sc );
      outrc[ i ].w = XROUND( outrc[ i ].w / sc );
      outrc[ i ].h = XROUND( outrc[ i ].h / sc );
    }
    for ( i = 0; i < outlen; ++i ) {
      p->out[i] = outrc[ i ];
    }
  }
  if (p->is_flip) {
    int i;
    for (i=0; i<outlen; ++i) {
      p->out[i].y = p->height - p->out[i].y - p->out[i].h;
    }
  }
  if (p->is_flop) {
    int i;
    for (i=0; i<outlen; ++i) {
      p->out[i].x = p->width - p->out[i].x - p->out[i].w;
    }
  }
  if (p->is_trans) {
    int i, t;
    for (i=0; i<outlen; ++i) {
#ifdef TRANX
      p->out[i].x = p->width - p->out[i].x - p->out[i].w;
#endif
      RECT_TRANS(p->out[i], t);
    }
  }
  p->outlen = outlen;
  if (r.w && p->outlen) {
    for (i = 0; i<p->outlen; ++i) {
      p->out[i].x += r.x;
      p->out[i].y += r.y;
    }
  }
  ASSERT(p->outlen<=p->maxoutlen);
  memset(p->out + p->outlen, 0, (p->maxoutlen-p->outlen)*sizeof(XRECT));
  return outlen;
}
