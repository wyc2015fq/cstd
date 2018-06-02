
#if 0

HAARCASCADE* cascade2haarcascade(const CASCADE* ca)
{
  int i, j;
  HAARCASCADE* cas = (HAARCASCADE*) malloc(sizeof(HAARCASCADE) + ca->stagelen * sizeof(fStage) + ca->weaklen * sizeof(fHaar));
  fStage* stage = (fStage*)(cas + 1);
  fHaar* ihaar = (fHaar*)(stage + ca->stagelen);
  cas->w = ca->w, cas->h = ca->h;
  cas->ih = ihaar, cas->ihlen = ca->weaklen;
  cas->stage = stage, cas->stagelen = ca->stagelen;

  for (i = 0; i < ca->stagelen; ++i) {
    stage[ i ].len = ca->stage[ i ].len;
    stage[ i ].thd = ca->stage[ i ].thd;
  }

  for (i = 0; i < ca->weaklen; ++i) {
    ihaar[ i ].n = 2;
    ihaar[ i ].ti = 0;
#define RECTCOPY(a, b) ((a).x = (b).x,(a).y = (b).y,(a).w = (b).w,(a).h = (b).h,(a).d = (b).d)

    RECTCOPY(ihaar[ i ].ft0, ca->haar[ i ].ft0);
    RECTCOPY(ihaar[ i ].ft1, ca->haar[ i ].ft1);
    ihaar[ i ].minval = ca->lut[ i ].minval;
    ihaar[ i ].binval = ca->lut[ i ].binval;

    for (j = 0; j < LUT_BIN; ++j) {
      ihaar[ i ].val[ j ] = ca->lut[ i ].val[ j ];
    }
  }

  return cas;
}

#undef XROUND1
#define XROUND1 XFLOOR
//#include "chalg.h"
int convet_to_int(const HAARCASCADE* ca0)
{
  int i, j;
  HAARCASCADE* ca = (HAARCASCADE*) ca0;

  if (CAISHAVEFLOAT(ca)) {
    ca->ih[ 0 ].ti = (unsigned char) - 1;
    {
      fStage* src = ca->stage;
      iStage* dst = (iStage*) ca->stage;

      for (i = 0; i < ca->stagelen; ++i) {
        dst[ i ].thd = XROUND1(SHIFT_TOINT(src[ i ].thd));
      }
    }
    {
      fHaar* src = ca->ih;
      iHaar* dst = (iHaar*) ca->ih;

      for (i = 0; i < ca->ihlen; ++i, ++src, ++dst) {
        dst->minval = XROUND1(SHIFT_TOINT(src->minval));
        dst->binval = XROUND(SHIFT_TOINT(src->binval));

        for (j = 0; j < LUT_BIN; ++j) {
          dst->val[ j ] = XROUND1(SHIFT_TOINT(src->val[ j ]));
        }
      }
    }
  }

  return 0;
}

int cascade2mem(const CASCADE* ca0, const char* filename, int addr, int is_int)
{
  unsigned char* mem = 0;
  int len = 0;
  int isdat = (NULL != strstr(filename, ".dat"));
  HAARCASCADE* ca = cascade2haarcascade(ca0);

  assert(sizeof(fHaar) == sizeof(iHaar));
  assert(sizeof(fStage) == sizeof(iStage));

  if (is_int) {
    convet_to_int(ca);
  }

  len = sizeof(HAARCASCADE) + sizeof(STAGE) * ca->stagelen + sizeof(iHaar) * ca->ihlen;
  mem = malloc(len);
  assert(mem);
  memcpy(mem, ca, sizeof(HAARCASCADE));
  memcpy(mem + sizeof(HAARCASCADE), ca->stage, sizeof(fStage) * ca->stagelen);
  memcpy(mem + sizeof(HAARCASCADE) + sizeof(fStage) * ca->stagelen, ca->ih, sizeof(fHaar) * ca->ihlen);
  {
    FILE* fout = fopen(filename, "wb");

    if (fout) {
      data2men1(fout, mem, len, addr, isdat);
      fclose(fout);
    }
  }

  free(mem);
  free(ca);
  return 0;
}
void cascade2hpp(const CASCADE* ca, const char* path)
{
  int i;
  FILE* pf;
  char name[ 256 ];
  size_t weaksize, featsize;
  char* weakptr, * featptr;
  splitname(path, name);
  pf = fopen(path, "wb");

  fprintf(pf, "\n#ifndef _%s_\n", name);
  fprintf(pf, "#define _%s_\n", name);
  fprintf(pf, "#include \"../_faceDetect.h\"\n\n");

  fprintf(pf, "STATIC STAGE %s_STAGE[] = {\n", name);

  for (i = 0; i < ca->stagelen; ++i) {
    fprintf(pf, _D(d) _DF() ENDL, ca->stage[ i ].len, ca->stage[ i ].thd);
  }

  fprintf(pf, "};\n\n");
  fprintf(pf, "DSP_STATIC iHaar %s_iHaar[] = {\n", name);
  weaksize = WEAK_FUN(ca).elemsize ;
  featsize = FEAT_FUN(ca).elemsize;
  weakptr = (char*)(ca->weakptr);
  featptr = (char*)(ca->featptr);

  for (i = 0; i < ca->weaklen; ++i, weakptr += weaksize, featptr += featsize) {
    //fprintf( pf, "%6d ", ca->id[ i ] );
    if (FEAT_FUN(ca).print) {
      FEAT_FUN(ca).print(pf, featptr);
    }

    if (WEAK_FUN(ca).print) {
      WEAK_FUN(ca).print(pf, weakptr);
    }

    fprintf(pf, ENDL);
  }

  fprintf(pf, "};\n\n");

  fprintf(pf, "DSP_STATIC HAARCASCADE %s_cascade = {", name);
  fprintf(pf, "%2d, %2d, %2d, %s_STAGE, %s_iHaar", ca->w, ca->h, ca->stagelen, name, name);
  fprintf(pf, "};\n\n");

  fprintf(pf, "#endif _%s_\n", name);
  //fflush(pf);
  fclose(pf);
}

#endif
