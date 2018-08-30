#ifndef _INIT_INL_
#define _INIT_INL_
#include "_faceDetect.h"

//iHaar2 * iHaarBuf = 0;
//int HAARBUFLEN = 0;

int pplight_memsize(void* p0) {
  pplight_param_t* p = (pplight_param_t*)p0;
  int h = p->height, w = p->width;
  p->dstlen = p->height*p->dststep;
  p->buflen = ARRSIZE( float, ( h + 13 ) * ( w + 13 ) + h * w + ( h + 13 ) );
  return 1;
}


int lbpex_memsize( void* p0 ) {
  lbpex_param_t* p;
  p = (lbpex_param_t*)p0;
  if (0==p->select_type) {
    p->featurelen = 99999;//init_size();
  } else if (1==p->select_type) {
    p->featurelen = FEAT_SIZE1*sizeof(FEAT_T);
  } else if (2==p->select_type) {
    p->featurelen = FEAT_SIZE2*sizeof(FEAT_T);
  } else if (3==p->select_type) {
    p->featurelen = FEAT_SIZE3*sizeof(FEAT_T);
  } else if (23==p->select_type) {//3338
    p->featurelen = (FEAT_SIZE2+FEAT_SIZE3)*sizeof(FEAT_T);
  }
  {
    int yh, yw, yl, ysize, exx, aah, aaw, aal, srch, srcw;
    srch = p->height, srcw = p->width;
    yh = srch + 1, yw = srcw + 1, ysize = yh * yw;
    exx = ( ( 2 * 6 - 1 ) * 3 ), yl = yw + exx;
    aah = srch + exx, aaw = srcw + exx, aal = aaw;
    p->buflen = ARRSIZE( int, ( yh + exx ) * ( yl ) ) + ARRSIZE( unsigned char, ysize );
    if ( p->fillborder ) {
      p->buflen += ARRSIZE( unsigned char, ( aah ) * ( aal ) );
    }
    if (p->do_pp) {
      pplight_param_t pp;
      COPY_MEMBER3(p, &pp, height, width, srcstep);
      pp.dststep = p->width;
      pp.srccn = 1;
      pplight_memsize(&pp);
      p->buflen = pp.dstlen + MAX(p->buflen, pp.buflen);
    }
  }
  return 0;
}

int facerecog_memsize( void* p0 ) {
  facerecog_param_t* p = (facerecog_param_t*)p0;
  lut_detect_param_t pp;
  lbpex_param_t lp;
  int pplen, lplen;
  COPY_MEMBER9(p, &pp, height, width, maxoutlen, roi, casnum, cas[0], cas[1], cas[2], cas[3]);
  pp.sc = FR_I2F(p->isc), pp.ss = FR_I2F(p->iss), pp.thd = FR_I2F(p->ithd);
  pp.ssmin = FR_I2F(p->issmin), pp.ssmax = FR_I2F(p->issmax);
  detect_memsize(&pp);
  if (T_GRAY!=p->datacode) {
    pp.buflen += pp.height*pp.width;
  }
  //if (p->mode & FR_TRANSPOSE) {
    //pp.buflen += pp.height*pp.width;
  //}
  p->distlen = p->featnum;
  lp.height=100, lp.width=100;
  lp.select_type = 2, lp.fillborder = 1, lp.do_pp = 1;
  lp.select_type = (p->mode&FR_FEAT_23) ? 23 : 2;
  lbpex_memsize(&lp);
  pplen = pp.buflen+pp.maxoutlen*sizeof(XRECT);
  lplen = lp.buflen;
  p->featurelen = lp.featurelen;
  p->buflen = MAX(pplen, lplen);
  p->buflen += (int)((p->height* p->width)*4*FR_I2F(p->isc));
  if (p->mode & FR_RECOG_TEST) {
    p->buflen = MAX(p->buflen, 2*lp.featurelen+lp.buflen);
  }
  p->buflen = 8*(1<<20);
  //#define pd(_m)  printf(#_m"=%d\r\n", _m)
  //#define px(_m)  printf(#_m"=0x%08x\r\n", _m)
  //pd(p->buflen), pd(p->maxoutlen), pd(p->height), pd(p->featurelen), px(p), px(&p->buf), px(&p->buflen);
  return 0;
}

int detect_set_cas(lut_detect_param_t* p, const void* cas[], int casnum) {
  if (casnum>0) {
    int i, j, cx, cy;
    HAARCASCADE* ca;
    i=0, j=0;
    for (; i<casnum; ++i) {
      ca = (HAARCASCADE*)(cas[i]);
      if (ca && 0!=ca->w && 0!=ca->h) {
        cx = ca->w, cy = ca->h;
        p->cas[j] = ca;
        break;
      }
    }
    for (j=i; i<casnum; ++i) {
     ca = (HAARCASCADE*)(cas[i]);
      if (ca && ca->w==cx && ca->h==cy) {
        p->cas[j] = ca;
        ++j;
      }
    }
    casnum = j;
  }
  p->casnum = casnum;
  return casnum;
}

int detect_init(void* p0, const void* cas[], int casnum, int height, int width,
                const unsigned char* data, int datastep,
                const unsigned char* mask, int maskstep,
                double sc, double ssmin, double ssmax, double ss, int stepxy, double thd,
                int mincnt, const XRECT* roi, XRECT* out, int maxoutlen, int is_trans, int is_flip, int is_flop ) {
  lut_detect_param_t* p = (lut_detect_param_t*)p0;
  ASSERT(casnum <= MAX_CAS_NUM);
  //memset(p, 0, sizeof(lut_detect_param_t));
  casnum = MIN(casnum, MAX_CAS_NUM);
  ssmin = MAX( ssmin, 0.01 );
  p->fastmode = 0;
  //sc = 0.25;
  //ssmin = 1.f;
  //ssmax = 100.f;
  sc = sc/ssmin;
  ssmin = 1.f;
  if ( sc < 0.0001f ) {
    sc = MAX( sc, 1.1f / ssmin );
    p->fastmode = 1;
  }
  if (roi && roi->w && roi->h &&
    (roi->x+roi->w) < width && 
    (roi->y+roi->h) < height
    ) {
    p->roi = *roi;
  } else {
    p->roi.x = p->roi.y = p->roi.w = p->roi.h = 0;
  }
  memcpy((void*)(p->cas), cas, sizeof(void*)*casnum);
  p->height = height;
  p->width = width;
  p->casnum = casnum;
  p->datastep = datastep;
  p->maskstep = maskstep;
  p->is_trans = is_trans;
  p->is_flip = is_flip;
  p->is_flop = is_flop;
  p->ssmax = ssmax;
  p->ssmin = ssmin;
  p->thd = thd;
  p->data = data;
  p->mask = mask;
  p->out = out;
  p->ss = ss;
  p->sc = sc;
  p->mincnt = mincnt;
  p->stepxy = stepxy;
  p->maxoutlen = maxoutlen;
  //p->isc    = SHIFT_TOINT(sc    );
  //p->issmin = SHIFT_TOINT(ssmin );
  //p->issmax = SHIFT_TOINT(ssmax );
  //p->iss    = SHIFT_TOINT(ss    );
  //p->ithd   = SHIFT_TOINT(thd   );
  detect_memsize(p);
  return 1;
}

void* FixCascade1( const void * cas, void* pstage, void* pih) {
  HAARCASCADE * ca = ( HAARCASCADE* ) cas;
  if (cas) {
    fStage* fs = ( fStage * ) ( ca + 1 );
    *(fStage**)pstage = fs;
    *(fHaar**)pih = ( fHaar * ) ( fs + ca->stagelen );
  }
  return ( void* ) ca;
}

int cas_ihlen(const void* cas[], int casnum) {
  int i, n;
  n = 0;
  for (i=0; i<casnum; ++i) {
    HAARCASCADE * ca = ( HAARCASCADE* ) cas[i];
    if (ca) {
      n += ca->ihlen;
    }
  }
  return n;
}

int detect_memsize( void* p0 ) {
  lut_detect_param_t* p = (lut_detect_param_t*)p0;
  int ihlen, h, w;
  detect_set_cas(p, p->cas, p->casnum);
  ihlen = cas_ihlen(p->cas, p->casnum);
  h = XFLOOR(p->height*p->sc);
  w = XFLOOR(p->width*p->sc);
  // 确保每个分类器cx, cy都一样
  p->buflen = DETECT_MEMSIZE( ihlen, h, w, p->maxoutlen );
  return 1;
}

int cascadesize(const void* p) {
  return CASCADESIZE(p);
}
#endif // _INIT_INL_
