
#if !ISDOUBLE
#define CCFUNC(NAME)  NAME ## _32f
#define T float
#define Align 1
#define RequireInitialization 1
#define dummy_precision 1e-5f
#define highest()  FLT_MAX
#define lowest()   -FLT_MAX
#else if TYPEID==CC_64F
#define CCFUNC(NAME)  NAME ## _64f
#define T double
#define Align 1
#define RequireInitialization 1
#define epsilon DBL_EPSILON
#define dummy_precision 1e-12
#define default_digits10_impl 0
#define highest()  DBL_MAX
#define lowest()   -DBL_MAX
#endif

struct complex {T re, im;};
#define real(c)  (c).re
#define imag(c)  (c).im
#define IsSigned   1
#define Alignment   4
#define Scalar T
#define Packet T
#define RealScalar T
#define UIntPtr(x) (size_t)(x)

#include "Core.inl"
#if 0
#include "LU.inl"
#include "Cholesky.inl"
#include "QR.inl"
#include "SVD.inl"
#include "Geometry.inl"
#include "Eigenvalues.inl"
#endif