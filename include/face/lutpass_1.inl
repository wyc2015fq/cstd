// 定点版本
static int lutpass_1( const FIX_FLOAT* tmpSamp, const void* ih, const void* stage0, int stagelen,
              const void* pWirk, FIX_FLOAT invvar2, double* pscore ) {
  const iHaar * ih2 = ( const iHaar * ) ih;
  const iStage * stage = ( const iStage * ) stage0;
  const iHaarWork3 * pWirk3 = ( const iHaarWork3 * ) pWirk;
  int iscore = 0, ispassed = 0;
  int f = 0, l = stagelen;
  int pp = 0, qq, slen;
  const iHaar * ih21;
  const iHaarWork3 * pWirk21;
  int haarval, id, rt0, rt1;
#ifdef __PC
  
  register int p00, p10, p20, p30;
  register int p01, p11, p21, p31;
#else
  
  register short p00, p10, p20, p30;
  register short p01, p11, p21, p31;
#endif
	UNUSED(pscore);
  //if (var2<0) {    printf("%d\n", var2);  }
  slen = ( stage + f ) ->len;
  qq = pp + slen;
  iscore = 0;
  //ih21 = ( qq < HAARBUFLEN ) ? ( PIHAAR2 ) iHaarBuf + pp : ( ih2 + pp );
  ih21 = ih2 + pp;
  pWirk21 = pWirk3 + pp;
  
  if ( !CAISNESTING(ih21, stage) ) {
    for ( ; f != l; ++f ) {
      qq = pp + ( stage + f ) ->len;
      iscore = 0;
      ih21 = ih2 + pp;
      pWirk21 = pWirk3 + pp;
      for ( ; pp < qq; ++pp, ++ih21, ++pWirk21 ) {
#include "boostpass_nesting_int_fun3_for.inl"
      }
      
      if ( iscore < ( stage + f ) ->thd ) {
        break;
      }
    }
  } else {
    //logset("定点.log");
    //logclear();
#define PRINTVAL()  logprintf("id=%3d val=%f vel=%f idf=%f min=%f bin=%f\n", id, ( ih21 ) ->val[ id ]*1./(1<<13), haarval*1./(1<<13), ( ( haarval - ih21->minval ) * ( ih21->binval ) )*1./(1<<26), ih21->minval*1./(1<<13), ih21->binval*1./(1<<13))
    
    //logprintf("invvar=%f\n", invvar2*1./(1<<13));
    for ( ; pp != qq; ++pp, ++ih21, ++pWirk21 ) {
#include "boostpass_nesting_int_fun3_for.inl"
      //PRINTVAL();
      //printf("__LINE__ %d\n", __LINE__);
    //printf("iscore %d %d %d\n", iscore, pWirk21->ft0.p0, sizeof(fHaarWork));
    }
    //printf("__LINE__ %d\n", __LINE__);
    //printf("iscore %d\n", iscore);
    //logprintf("lv=%3d len=%3d thd=%f sco=%f\n", f, ( stage + f ) ->len, ( stage + f ) ->thd*1./(1<<13), iscore*1./(1<<13));
    if ( iscore >= ( stage + f ) ->thd ) {
      ++f;
      for ( ; f != l; ++f ) {
        //printf("__LINE__ %d\n", __LINE__);
        //  int iii=1;
        //FILE* pf=NULL;
        //#define LUT_VAL21(x, lut)  (((((x)) - (lut)->minval) * (lut)->binval) >> (_SHIFT_2)) & (LUT_BIN_1)
        qq = pp + ( stage + f ) ->len;
        //iscore = LUT_VAL2( iscore, ( ih2 + pp ) );
        ih21 = ih2 + pp;
        haarval = iscore;
        id = ( ( ( haarval - ih21->minval ) * ( ih21->binval )  ) >> ( _SHIFT_2 ) );
        id = MAX(id, 0);
        id = MIN(id, 31);
        iscore = ( ih21 ) ->val[ id ];
        //PRINTVAL();
        ++pp;
        ih21 = ih2 + pp;
        pWirk21 = pWirk3 + pp;
        for ( ; pp < qq; ++pp, ++ih21, ++pWirk21 ) {
#include "boostpass_nesting_int_fun3_for.inl"
          //PRINTVAL();
        }
        
        //logprintf("lv=%3d len=%3d thd=%f sco=%f\n", f, ( stage + f ) ->len, ( stage + f ) ->thd*1./(1<<13), iscore*1./(1<<13));
          //if (pf) fclose(pf);
        if ( iscore < ( stage + f ) ->thd ) {
          break;
        }
      }
    }
  }
#undef PRINTVAL
  ispassed = l == f;
  
  // if ( ispassed ) {
     //*pscore = iscore * 1.f / SHIFT1;
  // }
  return ispassed;
}
