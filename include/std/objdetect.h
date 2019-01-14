
#ifndef _OBJDETECT_H_
#define _OBJDETECT_H_

typedef struct XRECT {
  int x, y, w, h;
  int score;
  int count;
}
XRECT;

#define MAX_CAS_NUM (4)
#ifndef USE_FIX_FLOAT
#define USE_FIX_FLOAT 1
#endif // USE_FIX_FLOAT
#ifdef USE_FIX_FLOAT
typedef int FIX_FLOAT;
#else
typedef double FIX_FLOAT;
#endif

typedef struct lut_detect_param_t {
  //????
  int height, width, datastep, maskstep, stepxy,
      mincnt, fastmode, maxoutlen, casnum;
  int is_trans, is_flop, is_flip; // ??????????
  double sc, ssmin, ssmax, ss, thd;
  const unsigned char* data, *mask;
  void const* cas[MAX_CAS_NUM];
  XRECT roi;
  //???
  int outlen;
  XRECT* out;
  int buflen;
  unsigned char* buf;
  //
  //int isc, issmin, issmax, iss, ithd;
  int h, w; //h=height*sc; w=width*sc;
  void* pWirk[MAX_CAS_NUM]; // = MALLOC( iHaarWork, ca->ihlen );
  FIX_FLOAT* inter; // = MALLOC( FIX_FLOAT, ( h + 1 ) * ( w + 1 ) );
  FIX_FLOAT* inter1; // = MALLOC( FIX_FLOAT, ( h + 1 ) * ( w + 1 ) );
  unsigned char* tmpimg0; // = MALLOC( unsigned char, w * h );
  unsigned char* tmpmask0; // = MALLOC( unsigned char, w * h );
  unsigned char* tmpimg; // = MALLOC( unsigned char, w * h );
  unsigned char* tmpmask; // = MALLOC( unsigned char, w * h );
  unsigned char* rect_partition_buf;
} lut_detect_param_t;


typedef struct pplight_param_t {
  int height, width, srcstep, srccn, dststep;
  const unsigned char* src;
  int dstlen;
  unsigned char* dst;
  int buflen;
  char* buf;
} pplight_param_t;

typedef struct lbpex_param_t {
  //
  int height, width, srcstep, srccn;
  const unsigned char* src;
  int do_pp; // pplight
  int fillborder; //
  //lbp????
  int select_type;
  int isall;
  //int* lbp_Feature_idyw; // len=[featurelen]
  //
  int featurelen;
  unsigned char* feature;
  int buflen;
  unsigned char* buf;
} lbpex_param_t;

#endif // _OBJDETECT_H_

