
#ifndef _TEST_FACERECOG_H_
#define _TEST_FACERECOG_H_
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <direct.h>
#include <string.h>
//#include "cstd.h"
#include "cascadearr.h"
#include "DSP_feat.h"

//#include "SCIO_Export.h"
//#include "../../include/utime.h"
//#include "debug.h"


#if !defined WIN32 && !defined __linux__

//#define dsp_malloc(_x) malloc(_x)
//#define dsp_free(_x) free(_x)

#define DSP_MALLOC(_TYPE, _N)             (_TYPE*)dsp_malloc((unsigned int)(sizeof(_TYPE)*(_N)))
#define DSP_FREE(ptr)                     dsp_free(ptr)

#define UNINIT_CAS(_ARRNAME) dsp_free(p##_ARRNAME)

#define CAS(NAME) FixCascade((const HAARCASCADE*)NAME)
#define INIT_CAS(_ARRNAME)  unsigned int* p##_ARRNAME = (unsigned int*)dsp_memclone(_ARRNAME, sizeof(_ARRNAME))

static long fsize3( FILE* stream ) {
  long pos, size;
  pos = ftell( stream );
  fseek( stream, 0, SEEK_END );
  size = ftell( stream );
  fseek( stream, pos, SEEK_SET );
  return size;
}
#define BUF_LOAD  dsp_buf_load
static int dsp_buf_load(const char* fname, buf_t* bf) {
  int len, readed_len;
  FILE* pf;
  printf("fname = %s", fname);
  pf = fopen(fname, "rb");
  if (pf) {
    len = fsize3(pf);
    printf("dsp_buf_load = %d\n", len);
    bf->data = (uchar*)dsp_malloc(len);
    readed_len = fread(bf->data, 1, len, pf);
    fclose(pf);
  }
  return 0;
}

#else
#define DSP_MALLOC MALLOC
#define DSP_FREE   FREE

#define face_recog_open() (0)
#define face_recog_close() (0)
#define face_recog_process(_x) facerecog_process(_x)
#define dsp_malloc(_x) malloc(_x)
#define dsp_free(_x) free(_x)

#ifdef USE_FIX_FLOAT
#define CAS(NAME) FixCascade((const HAARCASCADE*)NAME)
#define INIT_CAS(_ARRNAME)  unsigned int* p##_ARRNAME = _ARRNAME
#else
#define CAS(NAME) FixCascade((const HAARCASCADE*)NAME##_f)
#define INIT_CAS(_ARRNAME)  unsigned int* p##_ARRNAME = _ARRNAME##_f
#endif

#define UNINIT_CAS(_ARRNAME)
#define BUF_LOAD  buf_load
#endif


#define cap_delete cam_stop
#define disp_delete( disp )
#define disp_show cvShowImage
#define cap_getFrame(cap, _A, _H, _W, _L, _I) cam_QueryFrame(cap, *(_H), *(_W), *(_A), *(_L), *(_I))
#define cap_create  cam_start
#define disp_create( _name )  (cvNamedWindow( _name, 1 ), _name)
#define DBG printf

int maxfeatnum=0;
int featnum=0;
int featstep=sizeof(SPFeatInfo);
SPFeatInfo* featlib=NULL;

//unsigned int vis_nesting_face0701[];
//unsigned int VisAllEye_nesting[];

//#define MAXFEATNUM        2000 // 总共多少张
#define MAXFEATNUMPERMAN  20   // 每人多少张
#define FEATID(_F)  ((_F)->m_PID.m_ID)
#define FEATLIB_FILENAME  ".\\facelib.dat"
int load_facelib1() {
  FILE* pf = fopen(FEATLIB_FILENAME, "rb");
  FREE(featlib);
  if (pf) {
    fread(&featnum, sizeof(int), 1, pf);
    if (featnum>0) {
      featlib = MALLOC(SPFeatInfo, featnum);
      memset(featlib, 0, featnum*featstep);
      fread(featlib, featnum * featstep, 1, pf);
    } else {
      featnum=0;
    }
    maxfeatnum = featnum;
    fclose(pf);
  }
  return 0;
}

int save_facelib1() {
  FILE* pf = fopen(FEATLIB_FILENAME, "wb");
  if (pf) {
    fwrite(&featnum, sizeof(int), 1, pf);
    fwrite(featlib, featnum*featstep, 1, pf);
    fclose(pf);
  }
  return 0;
}

#endif // _TEST_FACERECOG_H_
