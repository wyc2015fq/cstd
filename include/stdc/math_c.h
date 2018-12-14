

#ifndef _STDC_MATH_C_H_
#define _STDC_MATH_C_H_

#ifdef _WIN32
#if _MSC_VER < 1300


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
  if (fabs(x) > 1e-4)
  {
    // x is large enough that the obvious evaluation is OK
    return log(1.0 + x);
  }
  
  // Use Taylor approx. log(1 + x) = x - x^2/2 with error roughly x^3/3
  // Since |x| < 10^-4, |x|^3 < 10^-12, relative error less than 10^-8
  
  return (-0.5 * x + 1.0) * x;
}
double log1p(const double x)
{
  if (fabs(x) > 1e-4)
  {
    // x is large enough that the obvious evaluation is OK
    return log(1.0 + x);
  }
  return(x * (x * (2 * x - 3) + 6)) / 6;
}

double gsl_log1p (const double x)
{
  volatile double y, z;
  y = 1 + x;
  z = y - 1;
  return log(y) - (z - x) / y ;  /* cancels errors with IEEE arithmetic */
}

double expm1a(double x)
{
  if(fabs(x) < 1e-5)
    return x + 0.5 * x * x;
  else
    return exp(x) - 1.0;
}

#define GSL_DBL_EPSILON        2.2204460492503131e-16
#define M_LN2      0.69314718055994530941723212146      /* ln(2) */
double gsl_expm1(const double x)
{
  /* FIXME: this should be improved */
  
  if (fabs(x) < M_LN2)
  {
    /* Compute the taylor series S = x + (1/2!) x^2 + (1/3!) x^3 + ... */
    
    double i = 1.0;
    double sum = x;
    double term = x / 1.0;
    
    do
    {
      i++ ;
      term *= x/i;
      sum += term;
    }
    while (fabs(term) > fabs(sum) * GSL_DBL_EPSILON) ;
    
    return sum ;
  }
  else
  {
    return exp(x) - 1;
  }
}


//http://svn.r-project.org/R/trunk/src/nmath/expm1.c

double expm1(double x)
{
  double y, a = fabs(x);
  if (a < DBL_EPSILON) return x;
  if (a > 0.697) return exp(x) - 1;  /* negligible cancellation */
  
  if (a > 1e-8)
    y = exp(x) - 1;
  else /* Taylor expansion, more accurate in this range */
    y = (x / 2 + 1) * x;
  
  /* Newton step for solving   log(1 + y) = x   for y : */
  /* WARNING: does not work for y ~ -1: bug in 1.5.0 */
  y -= (1 + y) * (log1p (y) - x);
  return y;
}
#endif
#endif

#endif // _STDC_MATH_C_H_
