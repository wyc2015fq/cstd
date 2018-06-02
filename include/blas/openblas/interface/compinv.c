
#if !defined(XDOUBLE) || !defined(QUAD_PRECISION)
static __inline void compinv(FLOAT *b, FLOAT ar, FLOAT ai) {

#ifndef UNIT
  FLOAT ratio, den;

  if (
#ifdef XDOUBLE
      (fabsl(ar)) >= (fabsl(ai))
#elif defined DOUBLE
      (fabs (ar)) >= (fabs (ai))
#else
      (fabsf(ar)) >= (fabsf(ai))
#endif
      ) {
    ratio = ai / ar;
    den   = (FLOAT)(ONE / (ar * (ONE + ratio * ratio)));
    ar =  den;
    ai = -ratio * den;
  } else {
    ratio = ar / ai;
    den   = (FLOAT)(ONE /(ai * (ONE + ratio * ratio)));
    ar =  ratio * den;
    ai = -den;
  }
  b[0] = ar;
  b[1] = ai;
#else
  b[0] = ONE;
  b[1] = ZERO;
#endif
}
#endif
