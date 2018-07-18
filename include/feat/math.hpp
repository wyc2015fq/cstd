
#include <math.h>

#define Math_min(a, b)     ( (a) < (b) ? (a) : (b) )
#define Math_max(a, b)     ( (a) > (b) ? (a) : (b) )
#undef min
#undef max

typedef char int8;
typedef short int16;
typedef int int32;
typedef unsigned char uchar;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef float float32;
typedef double float64;


struct _Math {
	const double PI = 3.1415926535857;
	template <typename T> inline T abs(const T& a) { return a < 0 ? -a : a; }
	template <typename T> inline T exp(const T& a) { return (T)::exp(a); }
	int round(double a) { return (int)::round(a);  }
};
static _Math Math;

