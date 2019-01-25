#ifndef _MATH_VC6_H_
#define _MATH_VC6_H_


#ifndef _HUGE_ENUF
#define _HUGE_ENUF  1e+300  // _HUGE_ENUF*_HUGE_ENUF must overflow
#endif

#define INFINITY   ((float)(_HUGE_ENUF * _HUGE_ENUF))
//#define HUGE_VAL   ((double)INFINITY)
#define HUGE_VALF  ((float)INFINITY)
#define HUGE_VALL  ((long double)INFINITY)
#define NAN        ((float)(INFINITY * 0.0F))


double log1pa(const double x)
{
  if (fabs(x) > 1e-4) {
    // x is large enough that the obvious evaluation is OK
    return log(1.0 + x);
  }
  // Use Taylor approx. log(1 + x) = x - x^2/2 with error roughly x^3/3
  // Since |x| < 10^-4, |x|^3 < 10^-12, relative error less than 10^-8
  return (-0.5 * x + 1.0) * x;
}
double log1p(const double x)
{
  if (fabs(x) > 1e-4) {
    // x is large enough that the obvious evaluation is OK
    return log(1.0 + x);
  }
  return(x * (x * (2 * x - 3) + 6)) / 6;
}

double gsl_log1p(const double x)
{
  volatile double y, z;
  y = 1 + x;
  z = y - 1;
  return log(y) - (z - x) / y;  /* cancels errors with IEEE arithmetic */
}

double expm1a(double x)
{
  if (fabs(x) < 1e-5) {
    return x + 0.5 * x * x;
  }
  else {
    return exp(x) - 1.0;
  }
}

#define GSL_DBL_EPSILON        2.2204460492503131e-16
#define M_LN2      0.69314718055994530941723212146      /* ln(2) */
double gsl_expm1(const double x)
{
  /* FIXME: this should be improved */
  if (fabs(x) < M_LN2) {
    /* Compute the taylor series S = x + (1/2!) x^2 + (1/3!) x^3 + ... */
    double i = 1.0;
    double sum = x;
    double term = x / 1.0;
    do {
      i++;
      term *= x / i;
      sum += term;
    } while (fabs(term) > fabs(sum) * GSL_DBL_EPSILON);
    return sum;
  }
  else {
    return exp(x) - 1;
  }
}


//http://svn.r-project.org/R/trunk/src/nmath/expm1.c

double expm1(double x)
{
  double y, a = fabs(x);
  if (a < DBL_EPSILON) { return x; }
  if (a > 0.697) { return exp(x) - 1; }  /* negligible cancellation */
  if (a > 1e-8) {
    y = exp(x) - 1;
  }
  else { /* Taylor expansion, more accurate in this range */
    y = (x / 2 + 1) * x;
  }
  /* Newton step for solving   log(1 + y) = x   for y : */
  /* WARNING: does not work for y ~ -1: bug in 1.5.0 */
  y -= (1 + y) * (log1p(y) - x);
  return y;
}
#if 0
CC_INLINE double expm1(double x)
{
  if (fabs(x) < LOGE2L) {
    x *= LOG2EL;
    __asm {
      fld x
      f2xm1
      fstp x
    };
    return x;
  }
  else {
    return exp(x) - 1.0;
  }
}
CC_INLINE double expm1(double x)
{
  if (fabs(x) < 1e-5) {
    return x + 0.5 * x * x;
  }
  else {
    return exp(x) - 1.0;
  }
}
#endif // 0

#define nextafter _nextafter
CC_INLINE float nextafterf(float x, float y)
{
  union {
    float f;
    unsigned int i;
  } u;
  if (_isnan(y) || _isnan(x)) {
    return x + y;
  }
  if (x == y)
    /* nextafter (0.0, -O.0) should return -0.0.  */
  {
    return y;
  }
  u.f = x;
  if (x == 0.0F) {
    u.i = 1;
    return y > 0.0F ? u.f : -u.f;
  }
  if (((x > 0.0F) ^ (y > x)) == 0) {
    u.i++;
  }
  else {
    u.i--;
  }
  return u.f;
}
//TODO: implement actual floating point log1pf, expm1f
CC_INLINE float log1pf(float x)
{
  return (float)log1p(x);
}
CC_INLINE float expm1f(float x)
{
  return (float)expm1(x);
}
//Based on asinhf routine  from cygwin
/* asinh(x) = copysign(log(fabs(x) + sqrt(x * x + 1.0)), x) */
CC_INLINE float asinhf(float x)
{
  float z;
  if (!_finite(x)) {
    return x;
  }
  z = fabsf(x);
  /* Use log1p to avoid cancellation with small x. Put
  x * x in denom, so overflow is harmless.
  asinh(x) = log1p (x + sqrt (x * x + 1.0) - 1.0)
  = log1p (x + x * x / (sqrt (x * x + 1.0) + 1.0))  */
  z = (float)log1p(z + z * z / (sqrt(z * z + 1.0) + 1.0));
  return (x > 0.0 ? z : -z);
}
//Based on asinh routine  from cygwin
/* asinh(x) = copysign(log(fabs(x) + sqrt(x * x + 1.0)), x) */
CC_INLINE double asinh(double x)
{
  double z;
  if (!_finite(x)) {
    return x;
  }
  z = fabs(x);
  /* Use log1p to avoid cancellation with small x. Put
  x * x in denom, so overflow is harmless.
  asinh(x) = log1p (x + sqrt (x * x + 1.0) - 1.0)
  = log1p (x + x * x / (sqrt (x * x + 1.0) + 1.0))  */
  z = log1p(z + z * z / (sqrt(z * z + 1.0) + 1.0));
  return (x > 0.0 ? z : -z);
}
//Based on routine  from cygwin
/* acosh(x) = log (x + sqrt(x * x - 1)) */
CC_INLINE double acosh(double x)
{
  if (_isnan(x)) {
    return x;
  }
  if (x < 1.0) {
    errno = EDOM;
    return NaN();
  }
  if (x > 4294967296.f)
    /*  Avoid overflow (and unnecessary calculation when
    sqrt (x * x - 1) == x). GCC optimizes by replacing
    the long double M_LN2 const with a fldln2 insn.  */
  {
    return log(x) + 6.9314718055994530941723E-1L;
  }
  /* Since  x >= 1, the arg to log will always be greater than
  the fyl2xp1 limit (approx 0.29) so just use logl. */
  return log(x + sqrt((x + 1.0) * (x - 1.0)));
}
//Based on routine  from cygwin
/* acosh(x) = log (x + sqrt(x * x - 1)) */
CC_INLINE float acoshf(float x)
{
  if (_isnan(x)) {
    return x;
  }
  if (x < 1.0f) {
    errno = EDOM;
    return (float)NaN();
  }
  if (x > 4294967296.f)
    /*  Avoid overflow (and unnecessary calculation when
    sqrt (x * x - 1) == x). GCC optimizes by replacing
    the long double M_LN2 const with a fldln2 insn.  */
  {
    return (float)(log(x) + 6.9314718055994530941723E-1L);
  }
  /* Since  x >= 1, the arg to log will always be greater than
  the fyl2xp1 limit (approx 0.29) so just use logl. */
  return (float)(log(x + sqrt((x + 1.0) * (x - 1.0))));
}
//Based on routine  from cygwin
/* atanh (x) = 0.5 * log ((1.0 + x)/(1.0 - x)) */
CC_INLINE double atanh(double x)
{
  double z;
  if (_isnan(x)) {
    return x;
  }
  z = fabs(x);
  if (z == 1.0) {
    //errno  = ERANGE;
    return (x > 0 ? Inf() : -Inf());
  }
  if (z > 1.0) {
    //errno = EDOM;
    return NaN();
  }
  /* Rearrange formula to avoid precision loss for small x.
  atanh(x) = 0.5 * log ((1.0 + x)/(1.0 - x))
  = 0.5 * log1p ((1.0 + x)/(1.0 - x) - 1.0)
  = 0.5 * log1p ((1.0 + x - 1.0 + x) /(1.0 - x))
  = 0.5 * log1p ((2.0 * x ) / (1.0 - x))  */
  z = 0.5 * log1p((z + z) / (1.0 - z));
  return x >= 0 ? z : -z;
}
//Based on routine  from cygwin
/* atanh (x) = 0.5 * log ((1.0 + x)/(1.0 - x)) */
CC_INLINE float atanhf(float x)
{
  float z;
  if (_isnan(x)) {
    return x;
  }
  z = fabsf(x);
  if (z == 1.0) {
    //errno  = ERANGE;
    return (float)(x > 0 ? Inf() : -Inf());
  }
  if (z > 1.0) {
    //errno = EDOM;
    return (float)NaN();
  }
  /* Rearrange formula to avoid precision loss for small x.
  atanh(x) = 0.5 * log ((1.0 + x)/(1.0 - x))
  = 0.5 * log1p ((1.0 + x)/(1.0 - x) - 1.0)
  = 0.5 * log1p ((1.0 + x - 1.0 + x) /(1.0 - x))
  = 0.5 * log1p ((2.0 * x ) / (1.0 - x))  */
  z = (float)(0.5 * log1p((z + z) / (1.0 - z)));
  return x >= 0 ? z : -z;
}

#endif // _MATH_VC6_H_
