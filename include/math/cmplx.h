
#ifndef _COMP_H_
#define _COMP_H_

#include <math.h>
#include <float.h>
#include <ymath.h>

#define REAL_IS_DOUBLE

#ifdef REAL_IS_LONG_DOUBLE
typedef long double _Ty;
// CLASS _Ctr<long double>
#define _Cosh(_X, _Y)     (_LCosh(_X, _Y))
#define _Infv(_Ty)        (_LInf._L)
#define _Exp(_P, _Y, _E)  (_LExp(_P, _Y, _E))
#define _Isinf(_X)        (_LDtest(&_X) == _INFCODE)
#define _Isnan(_X)        (_LDtest(&_X) == _NANCODE)
#define _Nanv()           (_LNan._L)
#define _Sinh(_X, _Y)     (_LSinh(_X, _Y))
#define atan2(_Y, _X)     (atan2l(_Y, _X))
#define cos(_X)           (cosl(_X))
#define exp(_X)           (expl(_X))
#define ldexp(_R, _E)     (ldexpl(_R, _E))
#define log(_X)           (logl(_X))
#define pow(_X, _Y)       (powl(_X, _Y))
#define sin(_X)           (sinl(_X))
#define sqrt(_X)          (sqrtl(_X))
#endif

#ifdef REAL_IS_DOUBLE
typedef double _Ty;
#define _Cosh(_X, _Y)     (_Cosh(_X, _Y))
#define _Infv(_Ty)        (_Inf._D)
#define _Exp(_P, _Y, _E)  (_Exp(_P, _Y, _E))
#define _Isinf(_X)        (_Dtest(&_X) == _INFCODE)
#define _Isnan(_X)        (_Dtest(&_X) == _NANCODE)
#define _Nanv()           (_Nan._D)
#define _Sinh(_X, _Y)     (_Sinh(_X, _Y))
#define atan2(_Y, _X)     (atan2(_Y, _X))
#define cos(_X)           (cos(_X))
#define exp(_X)           (exp(_X))
#define ldexp(_R, _E)     (ldexp(_R, _E))
#define log(_X)           (log(_X))
#define pow(_X, _Y)       (pow(_X, _Y))
#define sin(_X)           (sin(_X))
#define sqrt(_X)          (sqrt(_X))
#endif

#ifdef REAL_IS_FLOAT
typedef float _Ty;
#define _Cosh(_X, _Y)     (_FCosh(_X, _Y))
#define _Infv(_Ty)        (_FInf._F)
#define _Exp(_P, _Y, _E)  (_FExp(_P, _Y, _E))
#define _Isinf(_X)        (_FDtest(&_X) == _INFCODE)
#define _Isnan(_X)        (_FDtest(&_X) == _NANCODE)
#define _Nanv()           (_FNan._F)
#define _Sinh(_X, _Y)     (_FSinh(_X, _Y))
#define atan2(_Y, _X)     (atan2f(_Y, _X))
#define cos(_X)           (cosf(_X))
#define exp(_X)           (expf(_X))
#define ldexp(_R, _E)     (ldexpf(_R, _E))
#define log(_X)           (logf(_X))
#define pow(_X, _Y)       (powf(_X, _Y))
#define sin(_X)           (sinf(_X))
#define sqrt(_X)          (sqrtf(_X))
#endif

typedef struct cmplx {
  _Ty re, im;
}
cmplx;

// imag
#define imag(_X) (_X).im
// real
#define real(_X) (_X).re

cmplx CMPLX(_Ty _RE, _Ty _IM)
{
  cmplx _X;
  _X.re = _RE;
  _X.im = _IM;
  return _X;
}

// _Fabs
_Ty _Fabs(cmplx _X, int* _Pexp)
{
  *_Pexp = 0;
  _Ty _A = real(_X);
  _Ty _B = imag(_X);

  if (_Isnan(_A)) {
    return (_A);
  }
  else if (_Isnan(_B)) {
    return (_B);
  }
  else {
    if (_A < 0) {
      _A = -_A;
    }

    if (_B < 0) {
      _B = -_B;
    }

    if (_A < _B) {
      _Ty _W = _A;
      _A = _B, _B = _W;
    }

    if (_A == 0 || _Isinf(_A)) {
      return (_A);
    }

    if (1 <= _A) {
      * _Pexp = 2, _A = _A * 0.25, _B = _B * 0.25;
    }
    else {
      *_Pexp = -2, _A = _A * 4, _B = _B * 4;
    }

    _Ty _W = _A - _B;

    if (_W == _A) {
      return (_A);
    }
    else if (_B < _W) {
      const _Ty _Q = _A / _B;
      return (_A + _B
          / (_Q + sqrt(_Q * _Q + 1)));
    }
    else {
      static const _Ty _R2 = (const _Ty) 1.4142135623730950488L;
      static const _Ty _Xh = (const _Ty) 2.4142L;
      static const _Ty _Xl = (const _Ty) 0.0000135623730950488016887L;
      const _Ty _Q = _W / _B;
      const _Ty _R = (_Q + 2) * _Q;
      const _Ty _S = _R / (_R2 + sqrt(_R + 2))
          + _Xl + _Q + _Xh;
      return (_A + _B / _S);
    }
  }
}
// operator+

_Ty abs(cmplx _X)
{
  int _Xexp;
  _Ty _Rho = _Fabs(_X, &_Xexp);

  if (_Xexp == 0) {
    return (_Rho);
  }
  else {
    return (ldexp(_Rho, _Xexp));
  }
}
// arg

_Ty arg(cmplx _X)
{
  return (atan2(imag(_X), real(_X)));
}
// conjg

cmplx cos(cmplx _X)
{
  return (CMPLX(_Cosh(imag(_X), cos(real(_X))), -_Sinh(imag(_X), sin(real(_X)))));
}
// cosh


cmplx cosh(cmplx _X)
{
  return (CMPLX(
      _Cosh(real(_X), cos(imag(_X))),
      _Sinh(real(_X), sin(imag(_X)))));
}
// exp

cmplx exp(cmplx _X)
{
  _Ty _Re(real(_X)), _Im(real(_X));
  _Exp(&_Re, cos(imag(_X)), 0);
  _Exp(&_Im, sin(imag(_X)), 0);
  return (CMPLX(_Re, _Im));
}
// log

cmplx log(cmplx _X)
{
  int _Xexp;
  _Ty _Rho = _Fabs(_X, &_Xexp);

  if (_Isnan(_Rho)) {
    return (CMPLX(_Rho, _Rho));
  }
  else {
    static const _Ty _Cm = 22713.0 / 32768.0;
    static const _Ty _Cl = (const _Ty) 1.428606820309417232e-6L;
    _Ty _Xn = _Xexp;
    cmplx _W = CMPLX(_Rho == 0 ? -_Infv(_Rho)
        : _Isinf(_Rho) ? _Rho
        : log(_Rho) + _Xn * _Cl + _Xn * _Cm,
        atan2(imag(_X), real(_X)));
    return (_W);
  }
}
// log10

cmplx log10(cmplx _X)
{
  cmplx _W = log(_X);
  real(_W) *= (_Ty) 0.4342944819032518276511289L;
  imag(_W) *= (_Ty) 0.4342944819032518276511289L;
  return _W;
}

// norm
#define norm(_X) ((real(_X) * real(_X) + imag(_X) * imag(_X)))

// polar
#define polar(_Rho, _Theta) (CMPLX(_Rho * cos(_Theta), _Rho * sin(_Theta)))

cmplx sin(cmplx _X)
{
  return (CMPLX(
      _Cosh(imag(_X), sin(real(_X))),
      _Sinh(imag(_X), cos(real(_X)))));
}
// sinh

cmplx sinh(cmplx _X)
{
  return (CMPLX(
      _Sinh(real(_X), cos(imag(_X))),
      _Cosh(real(_X), sin(imag(_X)))));
}
// sqrt

cmplx sqrt(cmplx _X)
{
  int _Xexp;
  _Ty _Rho = _Fabs(_X, &_Xexp);

  if (_Xexp == 0) {
    return (CMPLX(_Rho, _Rho));
  }
  else {
    _Ty _Remag = ldexp(real(_X) < 0
        ? - real(_X) : real(_X), -_Xexp);
    _Rho = ldexp(sqrt(
        2 * (_Remag + _Rho)), _Xexp / 2 - 1);

    if (0 <= real(_X)) {
      return (CMPLX(_Rho, imag(_X) / (2 * _Rho)));
    }
    else if (imag(_X) < 0) {
      return (CMPLX(-imag(_X) / (2 * _Rho), -_Rho));
    }
    else
      return (CMPLX(imag(_X) / (2 * _Rho),
          _Rho));
  }
}

#if 0
template <class _Ty>
inline
_Ty _Pow_int_(_Ty _X, int _Y)
{
  unsigned int _N;

  if (_Y >= 0) {
    _N = _Y;
  }
  else {
    _N = -_Y;
  }

  for (_Ty _Z = _Ty(1); ; _X *= _X) {
    if ((_N & 1) != 0) {
      _Z *= _X;
    }

    if ((_N >>= 1) == 0) {
      return (_Y < 0 ? _Ty(1) / _Z : _Z);
    }
  }
}

cmplx pow(cmplx _X, int _Y)
{
  if (imag(_X) == 0) {
    return (CMPLX(pow(real(_X), _Y), 0));
  }
  else {
    return (_Pow_int_(cmplx(_X), _Y));
  }
}

cmplx pow(cmplx _X, const _Ty& _Y)
{
  if (imag(_X) == 0) {
    return (CMPLX(pow(real(_X), _Y), 0));
  }
  else {
    return (exp(_Y * log(_X)));
  }
}

cmplx pow(cmplx _X, cmplx _Y)
{
  if (imag(_Y) == 0) {
    return (pow(_X, real(_Y)));
  }
  else if (imag(_X) == 0) {
    return (CMPLX(pow(real(_X), _Y)));
  }
  else {
    return (exp(_Y * log(_X)));
  }
}

CMPLX pow(const _Ty& _X, _Y)
{
  if (imag(_Y) == 0) {
    return (CMPLX(pow(_X, real(_Y))));
  }
  else {
    return (exp(_Y * log(_X)));
  }
}
// sin

#endif

#endif // _COMP_H_
