
int lutpass_32s(const int* tmpSamp, const iHaar* ih, const iStage* stage0, int stagelen, const iHaarWork3* pWirk3, int invvar2, double* pscore)
{
  int ispassed = 0;
  const iHaar* ih2 = ih;
  const iStage* stage = stage0;
  int iscore = 0;
  int f = 0, l = stagelen;
  int pp = 0, qq, slen;
  const iHaar* ih21;
  const iHaarWork3* pWirk21;
  int haarval, id, rt0, rt1;
#ifdef __PC

  register int p00, p10, p20, p30;
  register int p01, p11, p21, p31;
#else

  register short p00, p10, p20, p30;
  register short p01, p11, p21, p31;
#endif
  slen = (stage + f) ->len;
  qq = pp + slen;
  iscore = 0.f;
  ih21 = ih2 + pp;
  pWirk21 = pWirk3 + pp;

  if (!CAISNESTING(ih21, stage)) {
    for (; f != l; ++f) {
      qq = pp + (stage + f) ->len;
      iscore = 0;
      ih21 = ih2 + pp;
      pWirk21 = pWirk3 + pp;

      for (; pp < qq; ++pp, ++ih21, ++pWirk21) {
#include "boostpass_nesting_int_fun3_for.inl"
      }

      if (iscore < (stage + f) ->thd) {
        break;
      }
    }
  }
  else {
    for (; pp != qq; ++pp, ++ih21, ++pWirk21) {
#include "boostpass_nesting_int_fun3_for.inl"
    }

    if (iscore >= (stage + f) ->thd) {
      ++f;

      for (; f != l; ++f) {
        qq = pp + (stage + f) ->len;
        ih21 = ih2 + pp;
        haarval = iscore;
        id = (((haarval - ih21->minval) * (ih21->binval)) >> (SHIFT_2));
        iscore = (ih21) ->val[ BOUND(id, 0, 31) ];
        ++pp;
        ih21 = ih2 + pp;
        pWirk21 = pWirk3 + pp;

        for (; pp < qq; ++pp, ++ih21, ++pWirk21) {
#include "boostpass_nesting_int_fun3_for.inl"
        }

        if (iscore < (stage + f) ->thd) {
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
