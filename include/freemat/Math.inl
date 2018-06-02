
#if 0
#define complex_divide(ar, ai, br, bi, c0, c1)  { \
  double ratio, den, abr, abi, cr; \
  if ((ai == 0) && (bi == 0)) { c1 = 0; c0 = ar / br; \
  } else if (bi == 0) {  c0 = ar / br;  c1 = ai / br; \
  } else if ((ar == 0) && (bi == 0)) { c0 = 0;   c1 = ai / br; \
  } else if ((ai == 0) && (br == 0)) { c0 = 0; c1 = -ar / bi; \
  } else if ((ar == br) && (ai == bi)) { c0 = 1; c1 = 0; \
  } else { \
    if ((abr = br) < 0.) { \
      abr = - abr; \
    } \
    if ((abi = bi) < 0.) { \
      abi = - abi; \
    } \
    if (abr <= abi) { \
      if (abi == 0) { \
        if (ai != 0 || ar != 0) { abi = 1.; } \
        c1 = c0 = abi / abr; \
      } else { \
        ratio = br / bi ; \
        den = bi * (1 + ratio * ratio); \
        cr = (ar * ratio + ai) / den; \
        c1 = (ai * ratio - ar) / den; \
      } \
    } \
    else { \
      ratio = bi / br ; \
      den = br * (1 + ratio * ratio); \
      cr = (ar + ai * ratio) / den; \
      c1 = (ai - ar * ratio) / den; \
    } \
    c0 = cr; \
  } \
}


#define complex_recip(ar, ai, cr, ci) { \
  if (ai == 0) { ci = 0; cr = 1 / ar; \
  } else { complex_divide(1, 0, ar, ai, cr, ci); } \
}

#define complex_phase(ar, ai)   atan2((double)(ai), (double)(ar))



static BOOL complex_lt(data_type ar, data_type ai,
    data_type br, data_type bi)
{
  data_type mag_a = complex_abs(ar, ai);
  data_type mag_b = complex_abs(br, bi);

  if ((mag_b - mag_a) > feps(mag_a) * 4) {
    return TRUE;
  }

  if ((mag_a - mag_b) > feps(mag_b) * 4) {
    return FALSE;
  }

  return (complex_phase(ar, ai) < complex_phase(br, bi));
}


static BOOL complex_gt(data_type ar, data_type ai,
    data_type br, data_type bi)
{
  data_type mag_a = complex_abs(ar, ai);
  data_type mag_b = complex_abs(br, bi);

  if ((mag_b - mag_a) > feps(mag_a) * 4) {
    return FALSE;
  }

  if ((mag_a - mag_b) > feps(mag_b) * 4) {
    return TRUE;
  }

  return (complex_phase(ar, ai) > complex_phase(br, bi));
}


static BOOL complex_eq(data_type ar, data_type ai,
    data_type br, data_type bi)
{
  return ((ar == br) && (ai == bi));
}


static BOOL complex_ne(data_type ar, data_type ai,
    data_type br, data_type bi)
{
  return ((ar != br) || (ai != bi));
}


static BOOL complex_le(data_type ar, data_type ai,
    data_type br, data_type bi)
{
  return complex_eq(ar, ai, br, bi) || complex_lt(ar, ai, br, bi);
}


static BOOL complex_ge(data_type ar, data_type ai,
    data_type br, data_type bi)
{
  return complex_eq(ar, ai, br, bi) || complex_gt(ar, ai, br, bi);
}


static int complex_log(data_type xr, data_type xi, data_type* yr, data_type* yi)
{
  *yr = log(complex_abs(xr, xi));
  *yi = atan2(xi, xr);
  return 0;
}


static int complex_exp(data_type xr, data_type xi, data_type* yr, data_type* yi)
{
  *yr = exp(xr) * cos(xi);
  *yi = exp(xr) * sin(xi);
  return 0;
}


static int complex_sqrt(data_type xr, data_type xi, data_type* yr, data_type* yi)
{
  data_type tr, ti;

  if ((xr >= 0) && (xi == 0)) {
    *yr = sqrt(xr);
    *yi = 0;
    return 0;
  }

  if ((xr < 0) && (xi == 0)) {
    *yi = sqrt(-xr);
    *yr = 0;
    return 0;
  }

  complex_log(xr, xi, &tr, &ti);
  tr /= 2.0;
  ti /= 2.0;
  complex_exp(tr, ti, yr, yi);
  return 0;
}


static int complex_square(data_type xr, data_type xi, data_type* yr, data_type* yi)
{
  *yr = xr * xr - xi * xi;
  *yi = 2 * xr * xi;
  return 0;
}

static double powi(double a, int b)
{
  double p, x;
  int n;
  unsigned long u;

  p = 1;
  x = a;
  n = b;

  if (n != 0) {
    if (n < 0) {
      n = -n;
      x = 1 / x;
    }

    for (u = n; ;)  {
      if (u & 01) {
        p *= x;
      }

      if (u >>= 1) {
        x *= x;
      }
      else {
        break;
      }
    }
  }

  return(p);
}



static int powc(data_type ar, data_type ai, data_type br, data_type bi, data_type* cr, data_type* ci)
{
  data_type logr, logi, x, y;
  data_type mag = complex_abs(ar, ai);

  if (mag == 0) {
    cr = 0;
    ci = 0;
    return 0;
  }

  logr = log(mag);
  logi = atan2(ai, ar);

  x = exp(logr * br - logi * bi);
  y = logr * bi + logi * br;

  cr = x * cos(y);
  ci = x * sin(y);
  return 0;
}
#endif

#define OpAdd_func(v1, v2, v3)   (v3 = v1 + v2)
#define OpAdd_funcc(ar, ai, br, bi, cr, ci)   (cr = ar + br,  ci = ai + bi)

#define OpSubtract_func(v1, v2, v3)     (v3 = v1 - v2)
#define OpSubtract_funcc(ar, ai, br, bi, cr, ci)   (cr = ar - br,  ci = ai - bi)

#define OpNegate_func(v1, v2)     (v2 = -(v1))
#define OpNegate_funcc(ar, ai, cr, ci)   (cr = -ar, ci = -ai)

#define OpSubtract_func(v1, v2, v3)     (v3 = v1 - v2)
#define OpSubtract_funcc(ar, ai, br, bi, cr, ci)   (cr = ar - br,  ci = ai - bi)

#if 0

struct OpLessThan {
  
  static BOOL func(data_type v1, data_type v2) {
    return v1 < v2;
  }
  
  static BOOL func(data_type ar, data_type ai,
      data_type br, data_type bi) {
    return complex_lt(ar, ai, br, bi);
  }
};

struct OpLessEquals {
  
  static BOOL func(data_type v1, data_type v2) {
    return v1 <= v2;
  }
  
  static BOOL func(data_type ar, data_type ai,
      data_type br, data_type bi) {
    return complex_le(ar, ai, br, bi);
  }
};

struct OpEquals {
  
  static BOOL func(data_type v1, data_type v2) {
    return v1 == v2;
  }
  
  static BOOL func(data_type ar, data_type ai,
      data_type br, data_type bi) {

    return complex_eq(ar, ai, br, bi);
  }
};

struct OpNotEquals {
  
  static BOOL func(data_type v1, data_type v2) {
    return v1 != v2;
  }
  
  static BOOL func(data_type ar, data_type ai,
      data_type br, data_type bi) {
    return complex_ne(ar, ai, br, bi);
  }
};

struct OpGreaterThan {
  
  static BOOL func(data_type v1, data_type v2) {
    return v1 > v2;
  }
  
  static BOOL func(data_type ar, data_type ai,
      data_type br, data_type bi) {
    return complex_gt(ar, ai, br, bi);
  }
};

struct OpGreaterEquals {
  
  static BOOL func(data_type v1, data_type v2) {
    return v1 >= v2;
  }
  
  static BOOL func(data_type ar, data_type ai,
      data_type br, data_type bi) {
    return complex_ge(ar, ai, br, bi);
  }
};

struct OpAnd {
  static BOOL func(const BOOL& v1, const BOOL& v2) {
    return (v1 && v2);
  }
  static int func(const BOOL&, const BOOL&, const BOOL&, const BOOL&, BOOL&, BOOL&) {
    // Unused...
  }
};

struct OpOr {
  static BOOL func(const BOOL& v1, const BOOL& v2) {
    return (v1 || v2);
  }
  static int func(const BOOL&, const BOOL&, const BOOL&, const BOOL&, BOOL&, BOOL&) {
    // Unused...
  }
};

#endif

//DOCBLOCK operators_typerules

mxArray* toClass(mxArray* buf, const mxArray* A, DataClass Tclass, int isconplex) {
  if (A->classID==Tclass) {
    return (mxArray*)A;
  }
  mxConvertArray(buf, A, Tclass, isconplex);
  return buf;
}

#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { const TYPE* a = (const TYPE*)Acast->data; TYPE* c = (TYPE*)Ccast->data; for (i = 0; i < len; ++i) { c[i] = (TYPE)-a[i]; } } break;

int mxPlus(const mxArray* A, mxArray* C) {
  return mxCopyArray(C, A);
}
// C(i) = A(i)*v1 + v2
int mxNegate(const mxArray* A, mxArray* C) {
  DataClass out_type = TypeToSigned(A->classID);
  int i, len = mxGetNumberOfElements(A);
  int iscomplex = A->iscomplex;
  mxArray ttt[5] = {0};
  mxArray* Acast = toClass(ttt+0, A, out_type, iscomplex);
  mxArray* Ccast = ttt+1;
  mxSetNumericArray(Ccast, Acast->n_dims, Acast->dims, out_type, iscomplex, 0);
  len *= iscomplex ? 2 : 1;
  switch (out_type) {
   default:
    ASSERT(0);
    break;
    CLASSSIZEDEF_SIGNED(0, CLASSSIZEDEF);
  }
  mxConvertArray(C, Ccast, out_type, iscomplex);
  mxFreeArrays(ttt, 5);
  return 0;
}
#undef CLASSSIZEDEF

#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { const TYPE* a = (const TYPE*)Acast->data; TYPE _v1=(TYPE)v1, _v2=(TYPE)v2; TYPE* c = (TYPE*)Ccast->data; for (i = 0; i < len; ++i) { hh; } } break;

// C(i) = A(i)*v1 + v2
int mxAddS(const mxArray* A, double v1, double v2, mxArray* C) {
  DataClass via_type;
  DataClass out_type = ComputeTypes0(A->classID, ComputeTypes2(v1, v2, 0), 0, &via_type);
  int i, len = mxGetNumberOfElements(A);
  int iscomplex = A->iscomplex;
  mxArray ttt[5] = {0};
  mxArray* Acast = toClass(ttt+0, A, via_type, iscomplex);
  mxArray* Ccast = ttt+1;
  mxSetNumericArray(Ccast, Acast->n_dims, Acast->dims, via_type, iscomplex, 0);
  len *= iscomplex ? 2 : 1;
  if (0==v1) {
    switch (via_type) {
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      ASSERT(0);
      return 0;
      CLASSSIZEDEFNUMBER(c[i] = _v2, CLASSSIZEDEF);
    }
  } else if (1==v1) {
    if (0==v2) {
      switch (via_type) {
      case mxCELL_CLASS:
      case mxSTRUCT_CLASS:
        ASSERT(0);
        return 0;
        CLASSSIZEDEFNUMBER(c[i] = a[i], CLASSSIZEDEF);
      }
    } else {
      switch (via_type) {
      case mxCELL_CLASS:
      case mxSTRUCT_CLASS:
        ASSERT(0);
        return 0;
        CLASSSIZEDEFNUMBER(c[i] = a[i]+_v2, CLASSSIZEDEF);
      }
    }
  } else {
    switch (via_type) {
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      ASSERT(0);
      return 0;
      CLASSSIZEDEFNUMBER(c[i] = a[i]*_v1+_v2, CLASSSIZEDEF);
    }
  }
  mxConvertArray(C, Ccast, out_type, iscomplex);
  mxFreeArrays(ttt, 5);
  return 0;
}
int mxDotInv(double v1, const mxArray* A, mxArray* C) {
  DataClass out_type = mxSINGLE_CLASS==A->classID ? mxSINGLE_CLASS : mxDOUBLE_CLASS;
  int i, len = mxGetNumberOfElements(A);
  double v2 = 0;
  int iscomplex = A->iscomplex;
  mxArray ttt[5] = {0};
  mxArray* Acast = toClass(ttt+0, A, out_type, iscomplex);
  mxArray* Ccast = ttt+1;
  mxSetNumericArray(Ccast, Acast->n_dims, Acast->dims, out_type, iscomplex, 0);
  len *= iscomplex ? 2 : 1;
  switch (out_type) {
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    ASSERT(0);
    return 0;
    CLASSSIZEDEFNUMBER(c[i] = _v1/(a[i]), CLASSSIZEDEF);
  }
  mxConvertArray(C, Ccast, out_type, iscomplex);
  mxFreeArrays(ttt, 5);
  return 0;
}
#undef CLASSSIZEDEF

#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  { const TYPE* a = (const TYPE*)Acast->data; const TYPE* b = (const TYPE*)Bcast->data; TYPE* c = (TYPE*)Ccast->data; for (i = 0; i < len; ++i) { hh; } }

#define BOOLOP(NAME, hh) \
int NAME(const mxArray* A, const mxArray* B, mxArray* C) { \
  DataClass via_type = mxLOGICAL_CLASS; \
  int i, len = mxGetNumberOfElements(A); \
  mxArray ttt[5] = {0}; \
  mxArray* Acast = toClass(ttt+0, A, via_type, 0); \
  mxArray* Bcast = toClass(ttt+1, B, via_type, 0); \
  mxArray* Ccast = ttt+2; \
  mxSetNumericArray(Ccast, Acast->n_dims, Acast->dims, via_type, 0, 0); \
  CLASSSIZEDEF(hh, 0, logical, 0); \
  mxConvertArray(C, Ccast, mxLOGICAL_CLASS, 0); \
  mxFreeArrays(ttt, 5); \
  return 0; \
}

BOOLOP(mxOr, (c[i] = a[i] || b[i]))
BOOLOP(mxAnd, c[i] = a[i] && b[i])

#undef CLASSSIZEDEF

#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { const TYPE* a = (const TYPE*)Acast->data; const TYPE* b = (const TYPE*)Bcast->data; logical* c = (logical*)Ccast->data; for (i = 0; i < len; ++i) { hh; } } break;

#define CMPOP(NAME, hh) \
int NAME(const mxArray* A, const mxArray* B, mxArray* C) { \
  DataClass via_type = mxLOGICAL_CLASS; \
  DataClass out_type = ComputeTypes(A, B, 0, &via_type); \
  int i, len = mxGetNumberOfElements(A); \
  int check = A->dims[1]==B->dims[0]; \
  mxArray ttt[5] = {0}; \
  mxArray* Acast = toClass(ttt+0, A, via_type, 0); \
  mxArray* Bcast = toClass(ttt+1, B, via_type, 0); \
  mxArray* Ccast = ttt+2; \
  mxSetNumericArray(Ccast, Acast->n_dims, Acast->dims, mxLOGICAL_CLASS, 0, 0); \
  switch (via_type) { \
    case mxCELL_CLASS: \
    case mxSTRUCT_CLASS: \
      ASSERT(0); \
      return 0; \
    CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF); \
  } \
  mxConvertArray(C, Ccast, mxLOGICAL_CLASS, 0); \
  mxFreeArrays(ttt, 5); \
  return 0; \
}

CMPOP(mxEqu, c[i] = a[i] == b[i])
CMPOP(mxNeq, c[i] = a[i] != b[i])
CMPOP(mxLss, c[i] = a[i] <  b[i])
CMPOP(mxLeq, c[i] = a[i] <= b[i])
CMPOP(mxGtr, c[i] = a[i] >  b[i])
CMPOP(mxGeq, c[i] = a[i] >= b[i])
#undef CLASSSIZEDEF

#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { const TYPE* a = (const TYPE*)A->data; logical* c = (logical*)Ccast->data; for (i = 0; i < len; ++i) { c[i] = !(int)(a[i]); } } break;

int mxNot(const mxArray* A, mxArray* C) {
  DataClass via_type = mxLOGICAL_CLASS;
  int i, len = mxGetNumberOfElements(A);
  mxArray ttt[5] = {0};
  mxArray* Ccast = ttt;
  mxSetNumericArray(Ccast, A->n_dims, A->dims, mxLOGICAL_CLASS, 0, 0);
  switch (via_type) {
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    ASSERT(0);
    break;
    CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
  }
  mxConvertArray(C, Ccast, mxLOGICAL_CLASS, 0);
  mxFreeArrays(ttt, 5);
  return 0;
}
#undef CLASSSIZEDEF


#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { const TYPE* a = (const TYPE*)Acast->data; const TYPE* b = (const TYPE*)Bcast->data; TYPE* c = (TYPE*)Ccast->data; for (i = 0; i < len; ++i) { hh; } } break;

// Perform the operation via a typed intermediary
#define DOTOP(NAME, force_type, hh, hhc) \
int NAME(const mxArray* A, const mxArray* B, mxArray* C) { \
  DataClass via_type; \
  DataClass out_type = ComputeTypes(A, B, force_type, &via_type); \
  int i, len = mxGetNumberOfElements(A); \
  int iscomplex = A->iscomplex || B->iscomplex; \
  mxArray ttt[5] = {0}; \
  mxArray* Acast = toClass(ttt+0, A, via_type, iscomplex); \
  mxArray* Bcast = toClass(ttt+1, B, via_type, iscomplex); \
  mxArray* Ccast = ttt+2; \
  int issizeeq = NDim_cmp(Acast->dims, Bcast->dims); \
  mxSetNumericArray(Ccast, Acast->n_dims, Acast->dims, via_type, iscomplex, 0); \
  if (iscomplex) { \
    switch (via_type) { \
      case mxCELL_CLASS: \
      case mxSTRUCT_CLASS: \
        ASSERT(0); \
        return 0; \
      CLASSSIZEDEFNUMBER(hhc, CLASSSIZEDEF); \
    } \
  } else { \
    switch (via_type) { \
      case mxCELL_CLASS: \
      case mxSTRUCT_CLASS: \
        ASSERT(0); \
        return 0; \
      CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF); \
    } \
  } \
  mxConvertArray(C, Ccast, out_type, iscomplex); \
  mxFreeArrays(ttt, 5); \
  return 0; \
}

// Add two objects.
DOTOP(mxAdd, mxUNKNOWN_CLASS, c[i] = a[i] + b[i], (c[i*2+0] = a[i*2+0] + b[i*2+0], c[i*2+1] = a[i*2+1] + b[i*2+1]));
// Subtract two objects.
DOTOP(mxSub, mxUNKNOWN_CLASS, c[i] = a[i] - b[i], (c[i*2+0] = a[i*2+0] - b[i*2+0], c[i*2+1] = a[i*2+1] - b[i*2+1]));
// Element-wise multiplication.
DOTOP(mxDotMul, mxUNKNOWN_CLASS, c[i] = a[i] * b[i], complex_mul((c+i*2), (a+i*2), (b+i*2)));
// Element-wise right divide.
DOTOP(mxDotRDiv, mxDOUBLE_CLASS, c[i] = a[i] / b[i], complex_div((c+i*2), (a+i*2), (b+i*2)));
// Element-wise left divide.
DOTOP(mxDotLDiv, mxDOUBLE_CLASS, c[i] = b[i] / a[i], complex_div((c+i*2), (b+i*2), (a+i*2)));

#undef DOTOP
#undef CLASSSIZEDEF

int mxMul(const mxArray* A, const mxArray* B, mxArray* C) {
  DataClass via_type = Double;
  DataClass out_type = ComputeTypes(A, B, Double, 0);
  int i=0, len = mxGetNumberOfElements(A);
  int check = A->dims[1]==B->dims[0];
  int iscomplex = A->iscomplex || B->iscomplex;
  mxArray ttt[5] = {0};
  mxArray* Acast = toClass(ttt+0, A, via_type, iscomplex);
  mxArray* Bcast = toClass(ttt+1, B, via_type, iscomplex);
  mxArray* Ccast = ttt+2;
  int flag = CV_GEMM_B_T|(iscomplex ? CV_GEMM_COMPLEX : 0);
  mxSetNumericArray(Ccast, Acast->n_dims, Acast->dims, via_type, iscomplex, 0);
  mymatmul((double*)Bcast->data, Bcast->dims[0], (double*)Acast->data, Acast->dims[0], 
    (double*)Ccast->data, Ccast->dims[0], Bcast->dims[1], Acast->dims[0], Acast->dims[1], flag);
  //mxAddS(Ccast, v1, v2, Ccast);
  mxConvertArray(C, Ccast, out_type, iscomplex);
  mxFreeArrays(ttt, 5);
  return 0;
}

int mxRDiv(const mxArray* A, const mxArray* B, mxArray* C) {
  ASSERT(0);
  return 0;
}

int mxLDiv(const mxArray* A, const mxArray* B, mxArray* C) {
  ASSERT(0);
  return 0;
}

// Element-wise power.
//DOCBLOCK operators_dotpower
// Simplified test -- if A & B are both real
// then if A is negative and B is non-integer,
// then the complex case is used.
int mxDotPow(const mxArray* A, const mxArray* B, mxArray* C) {
  //DOTOP(mxDotPow, mxDOUBLE_CLASS, c[i] = pow(a[i], b[i]), complex_pow2((c+i*2), (b+i*2), (a+i*2)));
  DataClass via_type = Double;
  DataClass out_type = ComputeTypes(A, B, 1, 0);
  int i=0, len = mxGetNumberOfElements(A);
  int check = A->dims[1]==B->dims[0];
  int iscomplex = A->iscomplex || B->iscomplex;
  mxArray ttt[5] = {0};
  mxArray* Acast = toClass(ttt+0, A, via_type, iscomplex);
  mxArray* Bcast = toClass(ttt+1, B, via_type, iscomplex);
  mxArray* Ccast = ttt+2;
  typedef double TYPE;
  const TYPE* a = (const TYPE*)Acast->data;
  const TYPE* b = (const TYPE*)Bcast->data;
  int ii = mxSetNumericArray(Ccast, Acast->n_dims, Acast->dims, via_type, iscomplex, 0);
  TYPE* c = (TYPE*)Ccast->data;
  if (iscomplex) {
    for (i=0; i<len; ++i) {
      complex_pow2((c+i*2), (b+i*2), (a+i*2));
    }
  } else {
    for (i=0; i<len; ++i) {
      c[i] = pow(a[i], b[i]);
    }
  }
  //mxAddS(Ccast, v1, v2, Ccast);
  mxConvertArray(C, Ccast, out_type, iscomplex);
  mxFreeArrays(ttt, 5);
  return 0;
}


/**
 * We want to perform a matrix-matrix operation between two data objects.
 * The following checks are required:
 *  1. If A or B is a scalar, then return FALSE - this is really a
 *     vector operation, and the arguments should be passed to a
 *     vector checker (like VectorCheck).
 *  2. Both A & B must be numeric
 *  3. Both A & B must be the same type (if not, the lesser type is
 *     automatically promoted).
 *  4. Both A & B must be 2-Dimensional.
 *  5. A & B must be conformant, i.e. the number of columns in A must
 *     match the number of rows in B.
 */


int* FixupPivotVector(int nrows, int* piv, int p)
{
  // Adjust the pivot vector
  int* fullpivot = (int*) malloc(sizeof(int) * nrows);
  int i;

  for (i = 0; i < nrows; i++) {
    fullpivot[i] = i;
  }

  for (i = 0; i < p; i++) {
    if (piv[i] != (i + 1)) {
      int tmp = fullpivot[i];
      fullpivot[i] = fullpivot[piv[i] - 1];
      fullpivot[piv[i] - 1] = tmp;
    }
  }

  free(piv);
  return fullpivot;
}

// In all cases, l is m x n  and   u is n x n if m > n
//               l is m x m  and   u is m x n if m < n
//           or  l is m x p  and   u is p x n
int LUDecompose(int nargout, const mxArray* A, mxArray* B)
{
  int nrows, ncols, p;
  mxArray* l = B+0;
  mxArray* u = B+1;
  mxArray* piv = B+2;
  if (nargout > 3) {
    throw_Exception("illegal usage of lu function - for dense matrices, maximum number of outputs is 3");
  }

  nrows = mxGetM(A);
  ncols = mxGetN(A);
  p = MIN(nrows, ncols);
  
  mxSetNumericMatrix(l, nrows, p, A->classID, A->iscomplex);
  mxSetNumericMatrix(u, p, ncols, A->classID, A->iscomplex);

  switch (A->classID) {
  default:
    throw_Exception("unhandled type for lu function");
    break;
    
  case Float:
    if (mxIsAllReal(A)) {
      typedef float T;
#define lapack_fn sgetrf_
#include "impl/LUReal.inl"
    }
    else {
      typedef scomplex T1;
      typedef float T;
#define lapack_fn cgetrf_
#include "impl/LUComplex.inl"
    }
    break;
    
  case Double:
    if (mxIsAllReal(A)) {
      typedef double T;
#define lapack_fn dgetrf_
#include "impl/LUReal.inl"
    }
    else {
      typedef dcomplex T1;
      typedef double T;
#define lapack_fn zgetrf_
#include "impl/LUComplex.inl"
    }
  }

  return 0;
}

// Invert a square matrix - Should check for diagonal matrices
// as a special case
int mxInvert(const mxArray* A, mxArray* B)
{
  mxArray ttt[2] = {0};
  DataClass via_type = mxSINGLE_CLASS==A->classID ? mxSINGLE_CLASS : mxDOUBLE_CLASS;
  if (!mxIs2D(A)) {
    throw_Exception("Cannot invert N-dimensional arrays.");
  }
  
  if (!mxIsSquare(A)) {
    throw_Exception("Cannot invert non-square matrices");
  }

  if (mxIsEmpty(A)) {
    mxCopyArray(B, A);
    return 0;
  }
  
  A = toClass(ttt+0, A, via_type, A->iscomplex);
  if (mxIsScalar(A)) {
    mxSetDoubleScalar(ttt+1, 1.);
    mxDotRDiv(ttt+1, A, B);
  } else {
    switch (A->classID) {
    default:
      throw_Exception("Invert does not support this data class");
      break;
      
    case Float:
      if (mxIsAllReal(A)) {
        typedef float T;
#define getrf sgetrf_
#define getri sgetri_
#include "impl/InvertReal.inl"
      }
      else {
        typedef scomplex T;
#define getrf cgetrf_
#define getri cgetri_
#include "impl/InvertComplex.inl"
      }
      break;
      
    case Double:
      if (mxIsAllReal(A)) {
        typedef double T;
#define getrf dgetrf_
#define getri dgetri_
#include "impl/InvertReal.inl"
      }
      else {
        typedef dcomplex T;
#define getrf zgetrf_
#define getri zgetri_
#include "impl/InvertComplex.inl"
      }
      break;
    }
  }
  mxFreeArrays(ttt, 2);
  return 0;
}


int Multiply(const mxArray* A, const mxArray* B, mxArray* C) {
  DataClass out_type = ComputeTypes(A, B, Double, 0);
  int iscomplex = A->iscomplex || B->iscomplex;
  mxArray ttt[5] = {0};
  mxArray* C_out=C;
  int m = A->dims[0], n = B->dims[1], k = A->dims[1];
  if (mxIsScalar(A)) {
    double tt = mxGetScalar(A);
    return mxAddS(B, tt, 0, C);
  }
  if (mxIsScalar(B)) {
    double tt = mxGetScalar(B);
    return mxAddS(A, tt, 0, C);
  }
  if ((m == 0) || (n == 0) || (k == 0)) return 0;
  if (k!=B->dims[0]) {
    throw_Exception("Requested matrix multiplication requires arguments to be conformant.");
  }
  A = toClass(ttt+0, A, out_type, iscomplex);
  B = toClass(ttt+1, B, out_type, iscomplex);
  C = (C==A || C==B) ? (ttt+2) : C;
  mxSetNumericMatrix(C, m, n, out_type, iscomplex);
  
  switch (out_type) {
  default: throw_Exception("Unhandled case for matrix matrix multiply");
  case Float:
    if (!iscomplex) {
      typedef float T;
#define Tcomplexgemm sgemm_
#include "impl/MatrixMultiply.inl"
    } else {
      typedef scomplex T;
#define Tcomplexgemm cgemm_
#include "impl/MatrixMultiply.inl"
    }
    break;
  case Double:
    if (!iscomplex) {
      typedef double T;
#define Tcomplexgemm dgemm_
#include "impl/MatrixMultiply.inl"
    } else {
      typedef dcomplex T;
#define Tcomplexgemm zgemm_
#include "impl/MatrixMultiply.inl"
    }
    break;
  }
  if (C!=C_out) {
    mxConvertArray(C_out, C, out_type, iscomplex);
  }
  mxFreeArrays(ttt, 5);
  return 0;
}

int SolveLinearEq(const mxArray* A, const mxArray* B, mxArray* X) {
  mxArray ttt[5] = {0};
  DataClass out_type = (mxSINGLE_CLASS==A->classID && mxSINGLE_CLASS==B->classID) ? mxSINGLE_CLASS : mxDOUBLE_CLASS;
  int iscomplex = A->iscomplex || B->iscomplex;
  int m = A->dims[0], n = B->dims[1];
  if (!mxIsSquare(A) || (A->dims[0] != B->dims[0])) {
    throw_Exception("Solving Ax=B requires A be square (use least squares solver for rectangular matrices A");
  }
  A = toClass(ttt+0, A, out_type, iscomplex);
  B = toClass(ttt+1, B, out_type, iscomplex);
  mxSetNumericMatrix(X, m, n, out_type, iscomplex);
  switch (out_type) {
  default:
    throw_Exception("Unsupported data type for linear equation solver");
  case Float:
    if (!iscomplex) {
      typedef float T;
      typedef float T1;
      typedef int T2;
#define Tgesvx sgesvx_
#define Tlamch slamch
#include "impl/SolveLinearEq.inl"
    } else {
      typedef scomplex T;
      typedef float T1;
      typedef float T2;
#define Tgesvx cgesvx_
#define Tlamch slamch
#include "impl/SolveLinearEq.inl"
    }
    break;
  case Double:
    if (!iscomplex) {
      typedef double T;
      typedef double T1;
      typedef int T2;
#define Tgesvx dgesvx_
#define Tlamch dlamch
#include "impl/SolveLinearEq.inl"
    } else {
      typedef dcomplex T;
      typedef double T1;
      typedef double T2;
#define Tgesvx zgesvx_
#define Tlamch dlamch
#include "impl/SolveLinearEq.inl"
    }
  }
  mxFreeArrays(ttt, 5);
  return 0;
}

void changeStride(void* dst_, int dstStride, void* src_, int srcStride, int rowCount, int colCount, int elemsize) {
  int i;
  uchar* dst = (uchar*)dst_;
  uchar* src = (uchar*)src_;
  dstStride*=elemsize;
  srcStride*=elemsize;
  rowCount*=elemsize;
  for (i=0;i<colCount;i++) {
    memcpy(dst + i*dstStride, src + i*srcStride*elemsize, rowCount);
  }
}

// MxN * N x K = M x K
int SolveLeastSquares(const mxArray* A, const mxArray* B, mxArray* X) {
  mxArray ttt[5] = {0};
  DataClass out_type = (mxSINGLE_CLASS==A->classID && mxSINGLE_CLASS==B->classID) ? mxSINGLE_CLASS : mxDOUBLE_CLASS;
  int iscomplex = A->iscomplex || B->iscomplex;
  int m = A->dims[0], n = A->dims[1], k = B->dims[1];
  if (!mxIsSquare(A) || (A->dims[0] != B->dims[0])) {
    throw_Exception("Solving Ax=B in a least squares sense requires the number of rows in A and B to be the same.");
  }
  A = toClass(ttt+0, A, out_type, iscomplex);
  B = toClass(ttt+1, B, out_type, iscomplex);
  mxSetNumericMatrix(X, m, n, out_type, iscomplex);

  if (AnyNotFinite(A) || AnyNotFinite(B)) {
    throw_Exception("Solving Ax=b in a least squares sense does not currently support non-finite entries in A or B");
  }
  switch (out_type) {
  default:
    throw_Exception("Unsupported data type for linear equation solver");
    break;
  case Float:
    if (!iscomplex) {
      typedef float T;
      typedef float T1;
#define Tlamch slamch
#define Tgelsy sgelsy_
#include "impl/SolveLeastSquares.inl"
    } else {
      typedef scomplex T;
      typedef float T1;
#define Tlamch slamch
#define Tgelsy cgelsy_
#include "impl/SolveLeastSquares.inl"
    }
  case Double:
    if (!iscomplex) {
      typedef double T;
      typedef double T1;
#define Tlamch slamch
#define Tgelsy dgelsy_
#include "impl/SolveLeastSquares.inl"
    } else {
      typedef dcomplex T;
      typedef double T1;
#define Tlamch slamch
#define Tgelsy zgelsy_
#include "impl/SolveLeastSquares.inl"
    }
  }
  mxFreeArrays(ttt, 5);
  return 0;
}

/**
 * Matrix-matrix divide
 */
//DOCBLOCK operators_leftdivide
int LeftDivide(const mxArray* A, const mxArray* B, mxArray* C)
{
  // Process our arguments
  // Its really a vector product, pass...
  if (mxIsScalar(A)) {
    double tt = mxGetScalar(A);
    return mxDotInv(tt, B, C);
  }
  if (mxIsScalar(B))
  {
    double tt = mxGetScalar(B);
    return mxAddS(A, tt, 0, C);
  }

  if (mxIsSquare(A)) {
    SolveLinearEq(A, B, C);
  }
  else {
    SolveLeastSquares(A, B, C);
  }
  return 0;
}

/**
 * Matrix-matrix divide
 */
//DOCBLOCK operators_rightdivide
int RightDivide(const mxArray* A, const mxArray* B, mxArray* C)
{
  mxArray ttt[5];
  mxArray* At = ttt+0;
  mxArray* Bt = ttt+1;
  mxArray* Ct = ttt+2;
  if (mxIsScalar(A)) {
    double tt = mxGetScalar(A);
    return mxDotInv(tt, B, C);
  }
  if (mxIsScalar(B))
  {
    double tt = mxGetScalar(B);
    return mxAddS(A, tt, 0, C);
  }
  
  Transpose(B, Bt);
  Transpose(A, At);
  LeftDivide(Bt, At, Ct);
  Transpose(Ct, C);
  mxFreeArrays(ttt, 5);
  return 0;
}

int EigenDecomposeSymmetric(const mxArray* A, mxArray* V, mxArray* D) {
  mxArray ttt[5] = {0};
  int iscomplex = A->iscomplex;
  DataClass out_type = (mxSINGLE_CLASS==A->classID) ? mxSINGLE_CLASS : mxDOUBLE_CLASS;
  int n = A->dims[0];
  mxArray* D_out = ttt;
  mxArray* V_out = (A==D) ? ttt+1 : V;
  A = toClass(ttt+0, A, out_type, iscomplex);
  if (!mxIs2D(A)) {
    throw_Exception("Cannot apply eigendecomposition to N-Dimensional arrays.");
  }
  if (A->dims[0] != A->dims[1]) {
    throw_Exception("Cannot eigendecompose a non-square matrix.");
  }
  mxSetNumericMatrix(D_out, n, 1, out_type, iscomplex);
  if (V) {
    mxSetNumericMatrix(V_out, n, n, out_type, iscomplex);
  }
  // Create one square matrix to store the eigenvectors
  switch (out_type) {
  default: 
    throw_Exception("Unhandled type for symmetric eigendecomposition");
    break;
  case Float:
    if (!iscomplex) {
      typedef float T;
#define Tsyev ssyev_
#include "impl/realEigenDecomposeSymmetric.inl"
    } else {
      typedef scomplex T;
      typedef float T1;
#define Theev cheev_
#include "impl/complexEigenDecomposeSymmetric.inl"
    }
    break;
  case Double:
    if (!iscomplex) {
      typedef double T;
#define Tsyev dsyev_
#include "impl/realEigenDecomposeSymmetric.inl"
    } else {
      typedef dcomplex T;
      typedef double T1;
#define Theev zheev_
#include "impl/complexEigenDecomposeSymmetric.inl"
    }
    break;
  }
  if (D_out!=D && D) {
    mxCopyArray(D, D_out);
  }
  if (V_out!=V && D) {
    mxCopyArray(V, V_out);
  }
  mxFreeArrays(ttt, 5);
  return 0;
}

int EigenDecomposeFullGeneral(const mxArray* A, mxArray* V, mxArray* D, BOOL balanceFlag) {
  mxArray ttt[5] = {0};
  int iscomplex = A->iscomplex;
  DataClass out_type = (mxSINGLE_CLASS==A->classID) ? mxSINGLE_CLASS : mxDOUBLE_CLASS;
  int n = A->dims[0];
  mxArray* D_out = ttt;
  mxArray* V_out = (A==D) ? ttt+1 : V;
  A = toClass(ttt+0, A, out_type, iscomplex);
  if (!mxIs2D(A)) {
    throw_Exception("Cannot apply eigendecomposition to N-Dimensional arrays.");
  }
  if (A->dims[0] != A->dims[1]) {
    throw_Exception("Cannot eigendecompose a non-square matrix.");
  }
  mxSetNumericMatrix(D_out, n, 1, out_type, mxCOMPLEX);
  mxSetNumericMatrix(V_out, n, n, out_type, iscomplex);
  // Select the eigenvector decomposition routine based on A's type
  switch (out_type) {
  default:   throw_Exception("Unhandled type for eigendecomposition");
    break;
  case Float: 
    if (!iscomplex) {
      typedef float T;
#define Tgeevx sgeevx_
#include "impl/realEigenDecompose.inl"
      HandleEigenVectorsRealMatrix(D_out, V_out, n, D_out, V_out);
    } else {
      typedef scomplex T;
      typedef float T1;
#define Tgeevx cgeevx_
#include "impl/complexEigenDecompose.inl"
    }
    break;
  case Double:
    if (!iscomplex) {
      typedef double T;
#define Tgeevx dgeevx_
#include "impl/realEigenDecompose.inl"
      HandleEigenVectorsRealMatrix(D_out, V_out, n, D_out, V_out);
    } else {
      typedef dcomplex T;
      typedef double T1;
#define Tgeevx zgeevx_
#include "impl/complexEigenDecompose.inl"
    }
    break;
  }
  if (D_out!=D && D) {
    mxCopyArray(D, D_out);
  }
  if (V_out!=V && D) {
    mxCopyArray(V, V_out);
  }
  mxFreeArrays(ttt, 5);
  return 0;
}

//DOCBLOCK operators_hermitian

//DOCBLOCK operators_transpose


/**
 * Matrix-matrix power - These are the cases to consider:
 *   1. scalar-scalar defer to dotpower
 *   2. square-scalar - if an integer, iteratively multiply
 *          To raise A^N power, we do a successive factorization
 *          A -> A^2 -> A^4
 * 13 -> 8 + 4 + 2 + 1
 *  5 -> 4 + 2 + 1
 *  1 -> 1
 *
 * 23 -> 16 + 8 + 4 + 2 + 1
 *  7 -> 4 + 2 + 1
 *  3 -> 2 + 1
 *  1 -> 1
 *
 *
 *   3. scalar-square - Decompose
 */
//DOCBLOCK operators_matrixpower
// Commented out this test for the 4.0 release (reference file was lost).
//Tests
//$near#y1=x1^x2#(any(loopi==[2,18,64,50:52])||(any(loopj==[50:52])))

// Raises Scalar^Matrix
static int PowerScalarMatrix(const mxArray* A, const mxArray* B, mxArray* C)
{
  mxArray ttt[10] = {0};
  // Do an eigendecomposition of B
  mxArray* V = ttt+0;
  mxArray* D = ttt+1;
  mxArray* E = ttt+2;
  mxArray* F = ttt+3;
  mxArray* G = ttt+4;

  if (IsSymmetric(B)) {
    EigenDecomposeSymmetric(B, V, D);
  }
  else {
    EigenDecomposeFullGeneral(B, V, D, FALSE);
  }

  // Get the diagonal part of D
  GetDiagonal(D, 0, E);
  // Call the vector version of the exponential
  mxDotPow(A, E, F); // B, V, D, E, F
  // Construct a diagonal matrix from F
  DiagonalArray(F, 0, G); // B, V, D, G, E, F
  // The output is (V*G)/V
  Multiply(V, G, E); // B, V, D, E, F
  RightDivide(E, V, C); // B, D, F
  mxFreeArrays(ttt, 10);
  return 0;
}

// Raises Matrix^Scalar
static int PowerMatrixScalar(const mxArray* A, const mxArray* B, mxArray* C)
{
  mxArray ttt[10] = {0};
  // Do an eigendecomposition of A
  mxArray* V = ttt+0;
  mxArray* D = ttt+1;
  mxArray* E = ttt+2;
  mxArray* F = ttt+3;
  mxArray* G = ttt+4;

  if (IsSymmetric(A)) {
    EigenDecomposeSymmetric(A, V, D);  //A, B, V, D
  }
  else {
    EigenDecomposeFullGeneral(A, V, D, FALSE);
  }

  // Get the diagonal part of D
  GetDiagonal(D, 0, E); // A, B, V, D, E
  // Call the vector version of the exponential
  mxDotPow(E, B, F); // F, A, V, D
  // Construct a diagonal matrix from F
  DiagonalArray(F, 0, G); // G, A, V, D, F
  // The output is (V*G)/V
  Multiply(V, G, E); // A, V, D, E, F
  RightDivide(E, V, C); // C, A, D, F
  mxFreeArrays(ttt, 10);
  return 0;
}


int mxPower(const mxArray* A, const mxArray* B, mxArray* C)
{
  DataClass out_type = (mxSINGLE_CLASS==A->classID && mxSINGLE_CLASS==B->classID) ? mxSINGLE_CLASS : mxDOUBLE_CLASS;
  mxArray ttt[5] = {0};
  int iscomplex = A->iscomplex || B->iscomplex;

  if (mxIsScalar(A) && mxIsScalar(B)) {
    return mxDotPow(A, B, C);
  }

  if (!mxIs2D(A) || !mxIs2D(B)) {
    throw_Exception("Cannot apply exponential operator to N-Dimensional arrays.");
  }
  
  if (mxIsAllReal(B) && mxIsScalar(B) && (mxGetScalar(B) == -1)) {
    mxInvert(A, C);
    return 0;
  }
  
  // Both arguments must be square
  if (!(mxIsSquare(A) && mxIsSquare(B))) {
    throw_Exception("Power (^) operator can only be applied to scalar and square arguments.");
  }
  
  A = toClass(ttt+0, A, out_type, iscomplex);
  B = toClass(ttt+1, B, out_type, iscomplex);
  // OK - check for A a scalar - if so, do a decomposition of B
  if (mxIsScalar(A)) {
    PowerScalarMatrix(A, B, C);
  }
  else if (mxIsScalar(B)) {
    PowerMatrixScalar(A, B, C);
  }
  else {
    throw_Exception("One of the arguments to (^) must be a scalar.");
  }
  return 0;
}
